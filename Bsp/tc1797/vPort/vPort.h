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
/* Free the csa consumed by StartOS,and goto __vPortSwitch2Task.
   MayBe The main() also consumed some csa,but ignoe and waste them.*/
#define vPortStartHighRdy()                                             \
    {                                                                   \
        __asm("movh.a	a11,#@his(__vPortSwitch2Task)");                \
        __asm("lea	a11,[a11]@los(__vPortSwitch2Task)");                \
        __nop();                                                        \
        __asm( "ret" );                                                 \
    }

#define vPortSwitch2Task()                      \
    {                                                                   \
        __asm("movh.a	a11,#@his(__vPortSwitch2Task)");                \
        __asm("lea	a11,[a11]@los(__vPortSwitch2Task)");                \
        __nop();                                                        \
        __asm( "ret" );                                                 \
    }

/* use the software interrupt to dispatch the high priority task */
#define vPortDispatch() __syscall(0)
#define vPortEnableInterrupt()  __enable()
#define vPortDisableInterrupt() __disable()

#define vPortSaveMsrAndDisableInterrupt(xMSR) \
	xMSR = (OsCpuSrType)__mfcr(ICR);__disable()

#define vPortRestoreMsr(xMSR) __mtcr(ICR,xMSR)

#define vPortGetIpl() (OsCpuIplType)(__mfcr(ICR)&0xFF)

#define vPortSetIpl(xIpl) __mtcr(ICR,((__mfcr(ICR)&(0xFFFFFF00))|xIpl))
/* CSA Manipulation. */
#define vPortCSA_TO_ADDRESS( pCSA )			( ( unsigned long * )( ( ( ( pCSA ) & 0x000F0000 ) << 12 ) | ( ( ( pCSA ) & 0x0000FFFF ) << 6 ) ) )
#define vPortADDRESS_TO_CSA( pAddress )		( ( unsigned long )( ( ( ( (unsigned long)( pAddress ) ) & 0xF0000000 ) >> 12 ) | ( ( ( unsigned long )( pAddress ) & 0x003FFFC0 ) >> 6 ) ) )



#define vPortSaveContext()  __svlcx()
#define vPortSaveSP()                           \
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
        __asm( "rfe" );                         \
    }

#define vPort_STM_INT0 0x01
#define vPort_STM_INT1 0x02
#define vPort_CPU0INT    0x0F
#define vPortTickIsr0Clear() STM_ISRR.B.CMP0IRR = 1
#define vPortTickIsr1Clear() STM_ISRR.B.CMP1IRR = 1

#define vPortEnterISR()                                                 \
    /*vPortSaveContext();*/                                             \
    if(0x00u == OSIsr2Nesting)                                          \
    {                                                                   \
        if(RUNNING == OSCurTcb->xState || WAITING == OSCurTcb->xState)  \
        {                                                               \
            vPortSaveSP();                                              \
        }                                                               \
    }                                                                   \
    OSEnterISR()

#define vPortLeaveISR()                         \
    OSExitISR();                                \
    vPortRestoreContext()


#if 1
void vPortMallocCSAandStartCurRdyTsk(void);
/* use instruction call to malloc csa for Current ready task.more fast.*/
#define vPortStartCurRdyTsk()                           \
	{	                                                \
		__asm("call vPortMallocCSAandStartCurRdyTsk");	\
	}
#else
/* This Method is copyed from FreeRTOS,but not effective.*/
#define vPortStartCurRdyTsk()                                           \
    {                                                                   \
        unsigned long *pulUpperCSA = STD_NULL;                          \
        __disable();                                                    \
        /* DSync to ensure that buffering is not a problem. */          \
        __dsync();                                                      \
        /* Consume one free CSAs. */                                    \
        pulUpperCSA = vPortCSA_TO_ADDRESS( __mfcr( FCX ) );             \
        /* Check that we have successfully reserved two CSAs. */        \
        if(STD_NULL != pulUpperCSA)                                     \
        {                                                               \
            /* Remove the one consumed CSAs from the free CSA list. */  \
            __dsync();                                                  \
            __mtcr( FCX, pulUpperCSA[ 0 ] );                            \
            __isync();                                                  \
        }                                                               \
        else                                                            \
        {                                                               \
            /* Simply trigger a context list depletion trap. */         \
            __svlcx();                                                  \
        }                                                               \
        /* Clear Its Link Info,Make it to be tail*/                     \
        pulUpperCSA[ 0 ]=0;                                             \
        /* Upper Context. */                                            \
        pulUpperCSA[ 2 ] = ( unsigned long )OSCurTcb->pxStack;		/* A10;	Stack Return aka Stack Pointer */ \
        pulUpperCSA[ 1 ] = vPortSYSTEM_PROGRAM_STATUS_WORD;		    /* PSW	*/ \
        __dsync();                                                      \
        __mtcr( PCXI, vPortINITIAL_PCXI_UPPER_CONTEXT_WORD | vPortADDRESS_TO_CSA( pulUpperCSA ) ); \
        /* Prepare To Satrt The Task */                                 \
        __asm("movh.a	a11,#@his(vPortPreActivateTask)");              \
        __asm("lea	a11,[a11]@los(vPortPreActivateTask)");              \
        __isync();                                                      \
        __nop();                                                        \
        /* Return to the first task selected to execute. */             \
        __asm volatile( "ret" );                                        \
    }
#endif

void vPortDispatcher(void);
void __vPortSwitch2Task(void);


#endif /* _VPORT_H_ */
