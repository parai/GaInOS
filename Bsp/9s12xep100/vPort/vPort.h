/* Copyright 2012, Fan Wang(Parai)
 *
 * This file is part of GaInOS.
 *
 * GaInOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *             
 * Linking GaInOS statically or dynamically with other modules is making a
 * combined work based on GaInOS. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of GaInOS give
 * you permission to combine GaInOS program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with GaInOS solely through the GaInOS defined interface. 
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for GaInOS and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of GaInOS are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 * 
 * GaInOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GaInOS. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _VPORT_H_
#define _VPORT_H_

#include "derivative.h"
#include "Os_Cfg.h"

#define vPortStartHighRdy()                     \
    do{                                         \
        vPortSwitch2Task();                     \
    }while(0)

/* use the software interrupt to dispatch the high priority task */
#define vPortDispatch() __asm("swi")
#define vPortEnableInterrupt()  __asm("cli")
#define vPortDisableInterrupt() __asm("sei")

#define INIT_IPL = 0;
#define INIT_CCR = 0;

#define vPortSaveMsrAndDisableInterrupt(xMSR)   \
    asm tpa; asm sei;asm staa xMSR

#define vPortRestoreMsr(xMSR)                   \
    asm ldaa xMSR; asm tap

#define vxPPAGE $15				/* The address of register PPAGE */
#define vxRPAGE $16
#define vxEPAGE $17
#define vxGPAGE $10
#define vPortRestoreSP()                        \
    asm("ldx OSCurTcb");                        \
    asm("lds 0,x")

#define vPortSaveContext()						\
	asm("ldaa 15");								\
	asm("psha")

#define vPortSaveSP()                           \
    asm("ldx OSCurTcb");                        \
    asm("sts 0,x")

#define vPortRestoreContext()					\
	asm("pula");								\
	asm("staa 15");								\
	asm("nop");									\
	asm("rti")

#define vPortSaveContext1()						\
	ldaa 15;									\
	psha

#define vPortRestoreContext1()					\
	pula;										\
	staa 15;									\
	nop;										\
	rti

#define vPortTickIsrClear()                                     \
    do{                                                         \
        CRGFLG &= 0xEF;			/* clear the interrupt flag */  \
    }while(0)

#define vPortEnterISR()                                                 \
    vPortSaveContext();                                                 \
                                                                        \
    if(INVALID_TASK != OSCurTsk)                                        \
    {                                                                   \
        if(0x00u == OSIsr2Nesting)                                      \
        {                                                               \
            if(RUNNING == OSCurTcb->xState || WAITING == OSCurTcb->xState) \
            {                                                           \
                vPortSaveSP();                                          \
            }                                                           \
        }                                                               \
    }                                                                   \
    OSEnterISR()

#define vPortLeaveISR()                         \
    OSExitISR();                                \
    vPortRestoreContext()


/* |----------------+----------------------------------------------| */
/* | Return Address | An Address when return from ISR              | */
/* |----------------+----------------------------------------------| */
/* | Y              | Registers  in this Group will saved into     | */
/* | X              | the Current used Satck Automatically when    | */
/* | A              | an interrupt or "swi" was generated,so there | */
/* | B              | is no need to save these registers.          | */
/* | CCR(IPL)       |                                              | */
/* |----------------+----------------------------------------------| */

#define vPortStartCurRdyTsk()                           \
    asm("LDY #vPortPreActivateTask");                   \
    asm("PSHY");         /* vPortPreActivateTask() */   \
    asm("LEAS -6,SP");                                  \
    /* IPL=0,All Irp Enable, Disable opcode STOP */     \
    asm("LDD #0x0080");                                 \
    asm("PSHD");                                        \
    asm("RTI");

OsCpuIplType vPortGetIpl(void);

#pragma CODE_SEG __NEAR_SEG NON_BANKED
void vPortSwitch2Task(void);
void vPortSetIpl(uint8_t xIpl);
#pragma CODE_SEG DEFAULT
#endif /* _VPORT_H_ */
