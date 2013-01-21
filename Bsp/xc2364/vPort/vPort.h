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

#include "Os_Cfg.h"
#include "XC23xxBREGS.h"
#define vPortStartHighRdy()                     \
    do{                                         \
        vPortSwitch2Task();                     \
    }while(0)

/* use the software interrupt to dispatch the high priority task */
#define vPortDispatch() __asm("TRAP #1")
#define vPortEnableInterrupt() __asm("BSET PSW_IEN")
#define vPortDisableInterrupt() __asm("BCLR PSW_IEN")

#define INIT_IPL = 0;
#define INIT_CCR = 0;

OsCpuSrType vPortGetMsr(void);

#define vPortSaveMsrAndDisableInterrupt(xMSR) 	\
		xMSR=vPortGetMsr();						\
		vPortDisableInterrupt()

void vPortRestoreMsr(OsCpuSrType xMSR);

#define vPortSaveContext()                      \
    __asm("push    MDH ");                      \
    __asm("push    MDL ");                      \
    __asm("push    r0 ");                       \
    __asm("push    r1 ");                       \
    __asm("push    r2 ");                       \
    __asm("push    r3 ");                       \
    __asm("push    r4 ");                       \
    __asm("push    r5 ");                       \
    __asm("push    r6 ");                       \
    __asm("push    r7 ");                       \
    __asm("push    r8 ");                       \
    __asm("push    r9 ");                       \
    __asm("push    r10 ");                      \
    __asm("push    r11 ");                      \
    __asm("push    r12 ");                      \
    __asm("push    r13 ");                      \
    __asm("push    r14 ");						\
    __asm("push    r15 ");

#define vPortSaveSP()                           \
    __asm("movw r11,_OSCurTcb");                \
    __asm("movw [r11],SP")

#define vPortRestoreContext()                   \
	__asm("pop     r15 ");                      \
    __asm("pop     r14 ");                      \
    __asm("pop     r13 ");                      \
    __asm("pop     r12 ");                      \
    __asm("pop     r11 ");                      \
    __asm("pop     r10 ");                      \
    __asm("pop     r9 ");                       \
    __asm("pop     r8 ");                       \
    __asm("pop     r7 ");                       \
    __asm("pop     r6 ");                       \
    __asm("pop     r5 ");                       \
    __asm("pop     r4 ");                       \
    __asm("pop     r3 ");                       \
    __asm("pop     r2 ");                       \
    __asm("pop     r1 ");                       \
    __asm("pop     r0 ");                       \
    __asm("pop     MDL ");                      \
    __asm("pop     MDH ");                      \
    __asm("reti")

#define vPortRestoreSP()                        \
    __asm("movw r11,_OSCurTcb");                \
    __asm("movw SP,[r11]")

#define vPortTickIsrClear() \
	RTC_ISNC_T14IR = 0

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

#define vPortStartCurRdyTsk()                       \
	__asm("movw r11,#0x0800");						\
    __asm("push r11");      /* IEN=1,BANK=0 */  	\
    __asm("movw r11,#@seg(_vPortPreActivateTask)");	\
    __asm("push r11");     							\
    __asm("movw r11,#@sof(_vPortPreActivateTask)");		\
    __asm("push r11");        					    \
    __asm("reti");

OsCpuIplType vPortGetIpl(void);

void vPortDispatcher(void);
void vPortSwitch2Task(void);
void vPortSetIpl(OsCpuIplType xIpl);

#endif /* _VPORT_H_ */
