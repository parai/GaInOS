		AREA vPort, CODE, READONLY
		PRESERVE8
		ARM	
		;;DEFINE OF FUNCTION
		EXPORT vPortDispatcher
		EXPORT vPortSwitch2Task
		EXPORT vPortSaveMsrAndDisableIrq
		EXPORT vPortRestoreMsr
		;;GLOBAL FUNCTION
		IMPORT vPortTaskIdle
		IMPORT vPortPreActivateTask
		;;GLOBAL DATA
		IMPORT OSCurTsk
		IMPORT OSHighRdyTsk
		IMPORT OSCurTcb
		IMPORT OSHighRdyTcb
		IMPORT G_INVALID_TASK
	
Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UND        EQU     0x1B
Mode_SYS        EQU     0x1F

I_Bit           EQU     0x80            ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40            ; when F bit is set, FIQ is disabled
;;OsCpuSrType vPortSaveMsrAndDisableIrq(void)
vPortSaveMsrAndDisableIrq
	STMDB	SP!, {R1}			;; Push R1.
	MRS		R0, CPSR			;; Get CPSR.
	MOV		R1,R0
	ORR		R1, R1, #0xC0		;; Disable IRQ, FIQ.
	MSR		CPSR_cxsf, R1		;; Write back modified value.	
	LDMIA	SP!, {R1}			;; Pop R1.
	BX		LR

;;void vPortRestoreMsr(OsCpuSrType xMSR)
vPortRestoreMsr
	MSR		CPSR_cxsf, R0
	BX		LR
;;void vPortSwitch2Task(void)
;; Make sure vPortSwitch2Task() was executed in USER MODE,
;; Or May be an fault happened
vPortSwitch2Task
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
	MSR 	CPSR_cxsf, #Mode_USR  ;IRQ FIQ Enable,USER Mode
	LDR		R0, =vPortPreActivateTask
	BX		R0
    ;;}
    ;;else
not_in_ready_state
    ;;{
    ;;  vPortRestoreSP();
	LDR		SP, [R0]
    ;;  vPortRestoreContext() 
	LDMFD 	SP!, {R1-R12,LR}
	LDMFD 	SP!, {R0}		;POP CPSR
	MSR 	CPSR_cxsf, R0
	LDMFD 	SP!, {R0}		;POP R0
	BX		LR
    ;;}

;; When the context was saved,It looks as below in the stack.
;; |------------+-----------------|
;; | R0         | <- High Address |
;; | CPSR       |                 |		
;; | LR(PC)     |                 |
;; | R12        |                 |
;; | ...        |                 |
;; | R1         |<- SP            |
;; |------------+-----------------|
;;; As vPortDispatcher() is critical method so that the way
;; void vPortDispatcher(void)		
vPortDispatcher
		;; Save R0 For use
		STMFD	SP!, {R0}		;Push R0
		MRS		R0,  CPSR       ;Push CPSR
		STMFD	SP!, {R0}
		;; Enter Critical Section,Just Disable Interrupt
		MSR		CPSR_cxsf, #Mode_USR:OR:F_Bit:OR:I_Bit
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
		STMFD	SP!, {R1-R12,LR}    ;R0-R12 PC
		LDR		R0, =OSCurTcb
		LDR		R0, [R0]
		STR		SP, [R0]
not_in_waiting_state
		;; }
		LDR		R0, =vPortSwitch2Task
		BX		R0	
		
		END
		
