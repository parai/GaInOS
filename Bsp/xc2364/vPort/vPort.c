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

static void vPortTaskIdle(void)
{
    /* Wait Untill a task was in ready state */
    vPortEnableInterrupt();
    /* vPortSetIpl(0); */
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
    OSTaskEntryTable[OSCurTsk]();
    OS_ASSERT(STD_FALSE);
}
OsCpuSrType vPortGetMsr(void)
{
	__asm("movw r2,PSW");
}
void vPortRestoreMsr(OsCpuSrType xMSR)
{
	__asm("movw PSW,r2");
}
void vPortSetIpl(OsCpuIplType xIpl)
{
	OsCpuSrType xMsr;
	xMsr=vPortGetMsr();
	vPortRestoreMsr( (xMsr&0x0FFF)|(xIpl<<12));
}
OsCpuIplType vPortGetIpl(void)
{
	OsCpuIplType xIpl;
	xIpl=vPortGetMsr();
	return((xIpl&0xF000)>>12);
}
void vPortSwitch2Task(void)
{
    OSCurTsk = OSHighRdyTsk;
    while(OSCurTsk == INVALID_TASK)
    {
        vPortTaskIdle();
    }
    OSCurTcb = OSHighRdyTcb;

    if(READY == OSCurTcb->xState)
    {
        OSCurTcb->xState = RUNNING;
        vPortRestoreSP();
        vPortStartCurRdyTsk();
    }
    else
    {
      vPortRestoreSP();
      vPortRestoreContext();
    }
}
/* vector ( id = 1, fill = "_vPortDispatcher"); */
void vPortDispatcher(void)
{
	vPortSaveContext();
    if(RUNNING == OSCurTcb->xState || WAITING == OSCurTcb->xState)
    {
        vPortSaveSP();
    }

    vPortSwitch2Task();
}
#include "Serial.h"
/* Only One Interrupt:counter T14 overflow of RTC should be Enabled.
 * Or An Error.Interrupt Node is 0x6E(110).
 * Set It In GaInOS.lsl File:"vector ( id = 110, fill = "_OSTickISR" );"
 * */
/* |--------------+-------------------| */
/* | On Enter ISR |                   | */
/* |--------------+-------------------| */
/* | PSW          |                   | */
/* | CSP          |                   | */
/* | IP           | <-SP              | */
/* |--------------+-------------------| */
/* | R15          | should be saved   | */
/* | ...          | for task,manually | */
/* | R1           |                   | */
/* |--------------+-------------------| */
void OSTickISR(void)
{  
    vPortEnterISR();
#if(cfgOS_COUNTER_NUM > 0)
    (void)IncrementCounter(0u);  /* System Counter */
#endif
    vPortTickIsrClear();

    vPortLeaveISR();
}

