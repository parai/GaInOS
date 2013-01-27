		AREA vPort, CODE, READONLY
		PRESERVE8
		ARM	
		;;DEFINE OF FUNCTION
		EXPORT vPortDispatcher
		EXPORT vPortSwitch2Task
		EXPORT vPortSaveMsrAndDisableIrq
		EXPORT vPortRestoreMsr
		EXPORT vPortIrqHandler
		;;GLOBAL FUNCTION
		IMPORT vPortTaskIdle
		IMPORT vPortPreActivateTask
		;;GLOBAL DATA
		IMPORT OSCurTsk
		IMPORT OSHighRdyTsk
		IMPORT OSCurTcb
		IMPORT OSHighRdyTcb
		IMPORT G_INVALID_TASK
		IMPORT OSIsr2Nesting
		IMPORT OSIsrVector
		IMPORT OSEnterISR
		IMPORT OSExitISR
	
Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UND        EQU     0x1B
Mode_SYS        EQU     0x1F

I_Bit           EQU     0x80            ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40            ; when F bit is set, FIQ is disabled
INTOFFSET       EQU  0x4a000014	
;;OsCpuSrType vPortSaveMsrAndDisableIrq(void)
vPortSaveMsrAndDisableIrq
	PRESERVE8
	STMDB	SP!, {R1}			;; Push R1.
	MRS		R0, CPSR			;; Get CPSR.
	MOV		R1,R0
	ORR		R1, R1, #0xC0		;; Disable IRQ, FIQ.
	MSR		CPSR_cxsf, R1		;; Write back modified value.	
	LDMIA	SP!, {R1}			;; Pop R1.
	BX		LR

;;void vPortRestoreMsr(OsCpuSrType xMSR)
vPortRestoreMsr
	PRESERVE8	
	MSR		CPSR_cxsf, R0
	BX		LR
		
;;void vPortSwitch2Task(void)
;; Make sure vPortSwitch2Task() was executed in USER MODE,
;; Or May be an fault happened
vPortSwitch2Task
	PRESERVE8	
	;;OSCurTsk = OSHighRdyTsk;
	LDR     R0, =OSHighRdyTsk
	LDRB	R0, [R0]
	LDR     R1, =OSCurTsk
	STRB	R0, [R1]
return_from_vPortTaskIdle
    ;;while(OSCurTsk == INVALID_TASK)
	LDR		R1, =G_INVALID_TASK
	LDRB	R1, [R1]
	CMP		R0,R1
	BNE		not_invalid_task
invalid_task
    ;;{
    ;;    vPortTaskIdle();
	LDR		R0, =vPortTaskIdle
	BX		R0
	BAL return_from_vPortTaskIdle
    ;;}
not_invalid_task	
    ;;OSCurTcb = OSHighRdyTcb;
	LDR     R0, =OSHighRdyTcb
	LDR		R0, [R0]
	LDR     R1, =OSCurTcb
	STR		R0, [R1]
    ;;if(READY == OSCurTcb->xState)
	LDRB	R2, [R0,#5]
	CMP		R2, #1
	BNE		not_in_ready_state
in_ready_state
    ;;{
    ;;    OSCurTcb->xState = RUNNING;
	MOV		R1, #0
	STRB	R1, [R0,#5]
    ;;    vPortRestoreSP();
	LDR		SP, [R0]
    ;;    vPortStartCurRdyTsk();	
	MSR 	CPSR_cxsf, #Mode_SVC  ;IRQ FIQ Enable,SVC Mode
	LDR		R0, =vPortPreActivateTask
	BX		R0
    ;;}
    ;;else
not_in_ready_state
    ;;{
    ;;  vPortRestoreSP();
	LDR		SP, [R0]
    ;;  vPortRestoreContext()
	LDMFD 	SP!, {R0}  
	MSR 	SPSR_cxsf, R0
	LDMFD 	SP!, {R0-R12, LR, PC}^
	;;}

;; When the context was saved,It looks as below in the stack.
;; |------------+-----------------|
;; | PC         | <- High Address |
;; | LR         |                 |		
;; | R12        |                 |
;; | ...        |                 |
;; | R0         |                 |
;; | SPSR       |<- SP            |
;; |------------+-----------------|
;;; As vPortDispatcher() is critical method so that the way
;; void vPortDispatcher(void)		
vPortDispatcher
		PRESERVE8
        ;; now in svc mode
		;; Save R0 For use
		STMFD	SP!, {R0}		;Push R0
		;; if(RUNNING == OSCurTcb->xState || WAITING == OSCurTcb->xState)
		;; {
		LDR	 	R0, =OSCurTcb
		LDR 	R0, [R0]
		LDRB 	R0, [R0,#5]
		CMP  	R0, #0  ;;RUNNING
		BEQ  	save_context_and_sp
not_in_running_state
		CMP 	R0, #2  ;;WAITING
		BNE 	not_in_waiting_state
save_context_and_sp
		LDMFD   SP!,{R0}		    ;pop R0
		STMFD	SP!, {LR}           ;PC
		STMFD	SP!, {R0-R12, LR}   ;R0-R12 LR
		MRS		R0,  SPSR           ;Push SPSR
		STMFD	SP!, {R0}
		LDR		R0, =OSCurTcb
		LDR		R0, [R0]
		STR		SP, [R0]
not_in_waiting_state
		;; }
		LDR		R0, =vPortSwitch2Task
		BX		R0	

vPortIrqHandler
		PRESERVE8
;;vPortEnterISR();
		; Save Context To task SVC stack
		; vPortSaveContext();
		STMFD   SP, {R0-R2}			; We will use R0 R1 R2 as temporary registers
	;now R0 R1 R2 have all saved to IRQ's stack before,but no adjust to SP
	;----------------------------------------------------------------------------
	;   R0--SP
	;	R1--PC 
	;   R2--SPSR
	;------------------------------------------------------------------------
		SUB     R0, SP,#12
		SUB     R1, LR, #4              ;Adjust PC for return address to task
		MRS     R2, SPSR				; Copy SPSR (Task CPSR)
		MSR     CPSR_cxsf, #Mode_SVC:OR:F_Bit:OR:I_Bit   ;Change to SVC mode,that is task's mode
									; SAVE TASK''S CONTEXT ONTO OLD TASK''S STACK									
		STMFD   SP!, {R1}				; Push task''s PC 
		STMFD   SP!, {R3-R12, LR}		; Push task''s LR,R12-R4	
		LDMFD   R0!, {R3-R5}			; Load Task''s R0-R2 from IRQ stack 
		STMFD   SP!, {R3-R5}			; Push Task''s R0-R2 to SVC stack
		STMFD   SP!, {R2}				; Push task''s SPSR
		
	;if(OSCurTsk != INVALID_TASK)
    ;{
		LDR	 	R0, =OSCurTsk
		LDR 	R0, [R0]
		LDR		R1, =G_INVALID_TASK
		LDRB	R1, [R1]
		CMP		R0,R1
		BNE		not_to_save
invalid_task2
	;    if(0x00u == OSIsr2Nesting)
    ;    {
		LDR	 	R0, =OSIsr2Nesting
		LDR 	R0, [R0]  
		CMP     R0,#0
		BNE     not_to_save
	;        if(RUNNING == OSCurTcb->xState || WAITING == OSCurTcb->xState)
    ;        {
    ;        save context
		LDR	 	R0, =OSCurTcb
		LDR 	R0, [R0]
		LDRB 	R0, [R0,#5]
		CMP  	R0, #0  ;;RUNNING
		BEQ  	save_sp
		;; not in running state
		CMP 	R0, #2  ;;WAITING
		BNE 	not_to_save
save_sp
		LDR		R0, =OSCurTcb
		LDR		R0, [R0]
		STR		SP, [R0]
not_to_save
    ;       }
	;    }
    ;}
	;OSEnterISR
	LDR		R0, =OSEnterISR
	BX		R0
	
	LDR     R0, =INTOFFSET
    LDR     R0, [R0]
       
    LDR     R1, =OSIsrVector
	LDR     R0, [R1, R0, LSL#2]            ; Call OS_CPU_IRQ_ISR_handler(); 
	BX		R0
;;vPortLeaveISR()
	LDR		R0, =OSExitISR
	BX		R0
	;;  vPortRestoreContext()
	LDMFD 	SP!, {R0}  
	MSR 	SPSR_cxsf, R0
	LDMFD 	SP!, {R0-R12, LR, PC}^
	;return
	
start_mmu
	EXPORT start_mmu
    ;asm volatile("mov r0,#L1_PTR_BASE_ADDR")
    LDR r0,=0x30700000
    mcr p15,0,r0,c2,c0,0          ;set base address of page table
    mvn r0,#0                  
    mcr p15,0,r0,c3,c0,0          ;enable all region access

	mrc p15,0,r0,c1,c0,0
    orr r0,r0,#0x1
    mcr p15,0,r0,c1,c0,0          ;set back to control register 
	bx  lr

		END
		
