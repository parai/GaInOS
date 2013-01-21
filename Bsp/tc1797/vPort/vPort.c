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
/* |---------+-------------------| */
/* | Name    | GaIn OS           | */
/* |---------+-------------------| */
/* | Author: | Wang Fan(parai)   | */
/* |---------+-------------------| */
/* | Email:  | parai@foxmail.com | */
/* |---------+-------------------| */
/* | WorkOn: | Emacs23.3         | */
/* |---------+-------------------| */
#include "Kernel.h"


/* CSA manipulation macros. */
#define vPortCSA_FCX_MASK					( 0x000FFFFFUL )

/* OS Interrupt and Trap mechanisms. */
#define vPortRESTORE_PSW_MASK				( ~( 0x000000FFUL ) )
#define vPortSYSCALL_TRAP					( 6 )

/* Each CSA contains 16 words of data. */
#define vPortNUM_WORDS_IN_CSA				( 16 )
static void vPortTaskIdle(void)
{
    /* Wait Untill a task was in ready state */
	vPortEnableInterrupt();
	vPortSetIpl(0);
	for(;;)
	{
		if(OSHighRdyTsk != INVALID_TASK)
		{
			OSCurTsk = OSHighRdyTsk;
			break;
		}
	}
	vPortDisableInterrupt();
	/* If NONE_PREEMPTIVE,just return to vPortSwitch2Task() */
}

void vPortPreActivateTask(void)
{
#if(cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
    if(OSCurTsk < cfgOS_TASK_WITH_IN_RES_NUM)
    {
        /* Do Get The Internal Resource */
        GetInResource();
    }
#endif
#if(cfgOS_PRE_TASK_HOOK == 1)
    OS_ENTER_CRITICAL();
    PreTaskHook();
    OS_EXIT_CRITICAL();
#endif
    __enable();
    OSTaskEntryTable[OSCurTsk]();
    OS_ASSERT(STD_FALSE);
}

void __vPortSwitch2Task(void)
{
	__disable();
    OSCurTsk = OSHighRdyTsk;
    while(OSCurTsk == INVALID_TASK)
	{
        vPortTaskIdle();
	}
    OSCurTcb = OSHighRdyTcb;

    if(READY == OSCurTcb->xState)
    {
        OSCurTcb->xState = RUNNING;
        vPortStartCurRdyTsk();
    }
    else
    {
        vPortRestoreSP();
        vPortRestoreContext();
    }
}

/*
 * When a task is deleted, it is yielded permanently until the IDLE task
 * has an opportunity to reclaim the memory that that task was using.
 * Typically, the memory used by a task is the TCB and Stack but in the
 * TriCore this includes the CSAs that were consumed as part of the Call
 * Stack. These CSAs can only be returned to the Globally Free Pool when
 * they are not part of the current Call Stack, hence, delaying the
 * reclamation until the IDLE task is freeing the task's other resources.
 * This function uses the head of the linked list of CSAs (from when the
 * task yielded for the last time) and finds the tail (the very bottom of
 * the call stack) and inserts this list at the head of the Free list,
 * attaching the existing Free List to the tail of the reclaimed call stack.
 *
 * NOTE: the IDLE task needs processing time to complete this function
 * and in heavily loaded systems, the Free CSAs may be consumed faster
 * than they can be freed assuming that tasks are being spawned and
 * deleted frequently.
 */
/* In FreeRTOS,This API is called when in idle task.*/
void vPortReclaimCSA( unsigned long pxHeadCSA )
{
    unsigned long  pxTailCSA, pxFreeCSA;
    unsigned long *pulNextCSA;

	pxHeadCSA = pxHeadCSA & vPortCSA_FCX_MASK;

	/* Mask off everything in the CSA link field other than the address.  If
       the	address is NULL, then the CSA is not linking anywhere and there is
       nothing	to do. */
	pxTailCSA = pxHeadCSA;

	/* Convert the link value to contain just a raw address and store this
       in a local variable. */
	pulNextCSA = vPortCSA_TO_ADDRESS( pxTailCSA );

	/* Iterate over the CSAs that were consumed as part of the task.  The
       first field in the CSA is the pointer to then next CSA.  Mask off
       everything in the pointer to the next CSA, other than the link address.
       If this is NULL, then the CSA currently being pointed to is the last in
       the chain. */
	while( 0UL != ( pulNextCSA[ 0 ] & vPortCSA_FCX_MASK ) )
	{
		/* Clear all bits of the pointer to the next in the chain, other
           than the address bits themselves. */
		pulNextCSA[ 0 ] = pulNextCSA[ 0 ] & vPortCSA_FCX_MASK;

		/* Move the pointer to point to the next CSA in the list. */
		pxTailCSA = pulNextCSA[ 0 ];

		/* Update the local pointer to the CSA. */
		pulNextCSA = vPortCSA_TO_ADDRESS( pxTailCSA );
	}

	{
		/* Look up the current free CSA head. */
		__dsync();
		pxFreeCSA = __mfcr( FCX );

		/* Join the current Free onto the Tail of what is being reclaimed. */
		pulNextCSA[ 0 ] = pxFreeCSA;

		/* Move the head of the reclaimed into the Free. */
		__dsync();
		__mtcr( FCX, pxHeadCSA );
		__isync();
	}
	/* Here When return,the link info in PCXI is of no use.
	 * It has been put to FCX*/
}

__trap(3) void vPortContextTrap(void)
{
	/* If Context Error, Deadloop. */
	for(;;);
}

__trap(vPortSYSCALL_TRAP) void vPortDispatcher(void)
{
 
    if(RUNNING == OSCurTcb->xState || WAITING == OSCurTcb->xState)
    {
        vPortSaveContext();
        vPortSaveSP();
    }
    else
    {
        /* Free the csa used by task OSCurTsk or maybe the preIdle */
        vPortReclaimCSA(__mfcr(PCXI));
    }
    
    /* Don't consume CSA.So just Jump*/
    __asm("j __vPortSwitch2Task");
}

#if 1
/* Add "vector ( id = 1, fill="OSTickISR0" );" in Project.lsl*/
__interrupt(vPort_STM_INT0)
void OSTickISR0(void)
{
	vPortEnterISR();
#if(cfgOS_COUNTER_NUM >0)
    (void)IncrementCounter(0);		/* Process the first counter,Default as system counter */
#endif
	vPortTickIsr0Clear();

	vPortLeaveISR();
}
/* Add "vector ( id = 2, fill="OSTickISR1" );" in Project.lsl*/
__interrupt(vPort_STM_INT1)
void OSTickISR1(void)
{
	vPortEnterISR();
#if(cfgOS_COUNTER_NUM >1)
    (void)IncrementCounter(1);		/* Process the first counter,Default as system counter */
#endif
	vPortTickIsr1Clear();

	vPortLeaveISR();
}
#endif

