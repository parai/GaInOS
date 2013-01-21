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
/* Free the csa consumed by StartOS or OSExitISR(),and goto __vPortSwitch2Task. */
#define vPortSwitch2Task()                                          \
    {                                                               \
        __asm volatile("movh.a	a11,#@his(__vPortSwitch2Task)");    \
        __asm volatile("lea	a11,[a11]@los(__vPortSwitch2Task)");    \
        __nop();                                                    \
        __asm volatile( "ret" );                                    \
    }

/* Some problem with this method */
#define vPortStartHighRdy()  vPortSwitch2Task()

/* use the software interrupt to dispatch the high priority task */
#define vPortDispatch() __syscall(0)
#define vPortEnableInterrupt()  __enable()
#define vPortDisableInterrupt() __disable()

#define vPortSaveMsrAndDisableInterrupt(xMSR)   \
	xMSR = (OsCpuSrType)__mfcr(ICR);__disable()

#define vPortRestoreMsr(xMSR) __mtcr(ICR,xMSR)

#define vPortGetIpl() (OsCpuIplType)(__mfcr(ICR)&0xFF)

#define vPortSetIpl(xIpl) __mtcr(ICR,((__mfcr(ICR)&(0xFFFFFF00))|xIpl))
/* CSA Manipulation. */
#define vPortCSA_TO_ADDRESS( pCSA )			( ( unsigned long * )( ( ( ( pCSA ) & 0x000F0000 ) << 12 ) | ( ( ( pCSA ) & 0x0000FFFF ) << 6 ) ) )
#define vPortADDRESS_TO_CSA( pAddress )		( ( unsigned long )( ( ( ( (unsigned long)( pAddress ) ) & 0xF0000000 ) >> 12 ) | ( ( ( unsigned long )( pAddress ) & 0x003FFFC0 ) >> 6 ) ) )



#define vPortSaveContext()  __svlcx()
#define vPortSaveSP()                               \
    OSCurTcb->pxStack=(TaskStackType*)__mfcr(PCXI)

#define vPortRestoreSP()                                \
    {                                                   \
        __dsync();                                      \
        __mtcr(PCXI, (unsigned long)OSCurTcb->pxStack); \
        __isync();                                      \
    }

#define vPortRestoreContext()                   \
    {                                           \
        __nop();                                \
        __rslcx();                              \
        __nop();                                \
        __asm volatile( "rfe" );                \
    }

#define vPort_STM_INT0   0xFD
#define vPort_STM_INT1   0xFC
#define vPortTickIsr0Clear() STM_ISRR.B.CMP0IRR = 1
#define vPortTickIsr1Clear() STM_ISRR.B.CMP1IRR = 1

#define vPortEnterISR()                                                 \
    /*vPortSaveContext();*/                                             \
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

#define vPortStartCurRdyTsk()                                           \
    {                                                                   \
        /* load task's stack */                                         \
        __asm volatile("movh.a	a10,#@his(OSCurTcb)");                  \
        __asm volatile("ld.a	a10,[a10]@los(OSCurTcb)");              \
        __asm volatile("ld.a	a10,[a10]");                            \
        vPortSetIpl(0);                                                 \
/* As the link info in PCXI maybe invalid caused by vPortReclaimCSA()   \
   or already saved by OSCurTsk.So Should Clear It */                   \
        __mtcr(PCXI,0);                                                 \
        __isync();                                                      \
        vPortPreActivateTask();                                         \
    }


void vPortDispatcher(void);
void __vPortSwitch2Task(void);


#endif /* _VPORT_H_ */
