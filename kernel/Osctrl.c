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
/* | Author: | Wang Fan(parai)   | */
/* |---------+-------------------| */
/* | Email:  | parai@foxmail.com | */
/* |---------+-------------------| */
/* | WorkOn: | Emacs23.3         | */
/* |---------+-------------------| */

/* |--------------------------------------+----------------| */
/* | History:                             | Year-Month-Day | */
/* |--------------------------------------+----------------| */
/* | Creat this file and decide to Design |      2012-9-27 | */
/* | the AutoSAR OS.                      |                | */
/* |--------------------------------------+----------------| */


/* |-------------------+----------------------------------------------| */
/* | File-name:        | Osctrl.c                                     | */
/* |-------------------+----------------------------------------------| */
/* | Reference Manual: | (1)Specification of Operating System(V3.0.2) | */
/* |                   | AUTOSAR_SWS_OS.PDF                           | */
/* |                   |                                              | */
/* |                   | (2)Operating SystemSpecification 2.2.3(OSEK) | */
/* |                   | os223.doc                                    | */
/* |-------------------+----------------------------------------------| */

#include "Kernel.h"

static void OSMiscInit(void)
{
    uint8_t i;
	/* Initialize Task Ready Map */
    OSReadyGrp = 0x00u;
    for(i=0;i<((cfgOS_MAX_PRIORITY>>3)+1);i++)
    {
        OSReadyTable[i]=0x00u;
    }
	/* Initialize Task Ready List */
    for(i=0;i<(cfgOS_MAX_PRIORITY+2);i++)
    {      
#if((cfgOS_MULTIPLY_ACTIVATE == STD_TRUE) && (cfgOS_SUM_ACTIVATES > 0))  
        /* Initialize For task ready queue method */
        OSTskRdyListHead[i]=0;
        OSTskRdyListTail[i]=0;
#else
        /* Initialize for task ready list method */
        OSTskRdyListHead[i]=INVALID_TASK; 
#endif
    }
	/* Initialize for alarm and counter */
#if(cfgOS_COUNTER_NUM>0)
    for(i=0;i<cfgOS_COUNTER_NUM;i++)
    {
        OSCntCurValueTable[i]=0;
#  if(cfgOS_USE_ALARM == STD_TRUE)
        OSCntAlmList[i]=INVALID_ALARM;
#  endif

#  if(cfgOS_SCHEDULE_TABLE_NUM>0)
        OsCntSchedTblList[i]=INVALID_SCHEDULE_TABLE;
#  endif
    }
#endif
#if(cfgOS_USE_ALARM == STD_TRUE)    
    for(i=0;i<cfgOS_ALARM_NUM;i++)
    {
        OSAlmInUseTable[i]=STD_FALSE;
    }
#endif  
	/* Initialize for event */
#if(cfgOS_CONFORMANCE_CLASS == ECC1 || cfgOS_CONFORMANCE_CLASS == ECC2)
    for(i=0;i<cfgOS_TASK_NUM;i++)
    {
        OSWaitEventTable[i]= NO_EVENT;
        OSSetEventTable[i] = NO_EVENT;
    } 
#endif       

	/* Initialize for normal resource and internal resource */
#if (cfgOS_USE_RESOURCE == STD_TRUE)
    for(i=0;i<cfgOS_RESOURCE_NUM;i++) 
    {
        OSResCtrlBlkTable[i].xSavedPriority=INVALID_PRIORITY;    
    }
#endif
#if (cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
    for (i=0; i<cfgOS_INTERNAL_RESOURCE_NUM; ++i)
    {
        OSInResSavedPrioTable[i]=INVALID_PRIORITY;
    }
#endif  
	/* Initialize for scheduler */
    OSCurTsk   = INVALID_TASK;
    OSCurTcb   = STD_NULL;

    OSHighRdyTsk  = INVALID_TASK;
    OSHighRdyTcb  = STD_NULL;
    OSHighRdyPrio = INVALID_PRIORITY;

	/* Initialize for ISR */
    OSRunning  = STD_FALSE;
    OSIsr2Nesting = 0u;
}

static void OSTskTcbInit(void)
{
    uint8_t i;
    PriorityType xPriority;
    TcbType* pxTcb; 
    for(i=0;i<(cfgOS_TASK_NUM);i++) /* i is xTaskID */
    {
        pxTcb     = &OSTcbTable[i];
#if(cfgOS_USE_RESOURCE == STD_TRUE)
        pxTcb->xCurResID= INVALID_RESOURCE;
#endif 
        /* If the task i was configured autostart,then do activate it */
        if(isTskAutoAcitvateTrue(i))
        {  
            xPriority = tableGetTskInitPrio(i);
            pxTcb->pxStack   = tableGetTskInitStkTop(i);
            pxTcb->xPriority = xPriority;        
            pxTcb->xState    = READY;
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
            pxTcb->xActivateCount = 1;
#endif
            /* Inseart task i at tail and set the ready map if empty */
            listInsertTskAtTail(xPriority,i);
            /* Find out the highest priority ready task */
            if( (xPriority>OSHighRdyPrio) || (INVALID_PRIORITY == OSHighRdyPrio) )
            {
                OSHighRdyPrio  = xPriority;
                OSHighRdyTsk   = i;
                OSHighRdyTcb   = pxTcb;
            }
        }
        else  
        {
            /* When configure system ONLY_ONE_TASK_PER_PRIORITY,make
               sure only one task per priority,or maybe an error 
            */
            pxTcb->xState = SUSPENDED;
        }
    }
}
#if(cfgOS_SCHEDULE_TABLE_NUM>0)
static void OSSchedTblInit(void)
{
    ScheduleTableType i;
    for(i=0;i<cfgOS_SCHEDULE_TABLE_NUM;i++)
    {
        if(IsSchedTbleAutostartable(i))
        {
            if(ABSOLUTE == tableGetSchedTblAutostartType(i))
            {
                (void)StartScheduleTableAbs(i,tableGetSchedTblAutostartValue(i));
            }
            else if(RELATIVE == tableGetSchedTblAutostartType(i))
            {
                (void)StartScheduleTableRel(i,tableGetSchedTblAutostartValue(i));
            }
            else if(SYNCHRON == tableGetSchedTblAutostartType(i))
            {
                (void)StartScheduleTableSynchron(i);
            }
        }
        else
        {
            tableGetSchedTblStatus(i)=SCHEDULETABLE_STOPPED;
        }
    }
}
#endif
/* |------------------+------------------------------------------------------| */
/* | Syntax:          | void StartOS ( AppModeType <Mode> )                  | */
/* |------------------+------------------------------------------------------| */
/* | Parameter (In):  | Mode:application mode                                | */
/* |------------------+------------------------------------------------------| */
/* | Parameter (Out): | none                                                 | */
/* |------------------+------------------------------------------------------| */
/* | Description:     | The user can call this system service to start the   | */
/* |                  | operating system in a specific mode, see chapter 5   | */
/* |                  | (os223.doc), Application modes.                      | */
/* |------------------+------------------------------------------------------| */
/* | Particularities: | Only allowed outside of the operating system,        | */
/* |                  | therefore implementation specific restrictions may   | */
/* |                  | apply. See also chapter 11.3, System start-up,       | */
/* |                  | especially with respect to systems where OSEK and    | */
/* |                  | OSEKtime coexist. This call does not need to return. | */
/* |------------------+------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                               | */
/* |------------------+------------------------------------------------------| */
extern void CpuInitOsTick(void);
void StartOS ( AppModeType xAppMode )
{
	OSAppMode=xAppMode;		/* Save the Mode */
	OSMiscInit();				/* Initialize miscellaneous variables */
	OSTskTcbInit();			/* Initialize Task contorl block */
#if(cfgOS_SCHEDULE_TABLE_NUM>0)
    OSSchedTblInit();
#endif
	CpuInitOsTick();			/* Initialize System Tick ISR */

	OSRunning = STD_TRUE;                   
#if(cfgOS_START_UP_HOOK == 1)
	StartupHook();			/* Call Start up hook */
#endif
	OSStartHighRdy();			/* Start the first high ready priority task if exists */
    /* OS424: The first call to StartOS() (for starting the Operating System) shall not
       return. */
}
/* |------------------+------------------------------------------------------------------| */
/* | Syntax:          | void ShutdownOS ( StatusType <Error> )                           | */
/* |------------------+------------------------------------------------------------------| */
/* | Parameter (In):  | Error:error occurred                                             | */
/* |------------------+------------------------------------------------------------------| */
/* | Parameter (Out): | none                                                             | */
/* |------------------+------------------------------------------------------------------| */
/* | Description:     | 1.The user can call this system service to abort                 | */
/* |                  | the overall system (e.g. emergency off). The                     | */
/* |                  | operating system also calls this function internally,            | */
/* |                  | if it has reached an undefined internal state and is             | */
/* |                  | no longer ready to run.                                          | */
/* |                  | 2.If a ShutdownHook is configured the hook routine               | */
/* |                  | ShutdownHook is always called (with <Error> as argument)         | */
/* |                  | before shutting down the operating system.                       | */
/* |                  | 3.If ShutdownHook returns, further behaviour of ShutdownOS       | */
/* |                  | is implementation specific.                                      | */
/* |                  | 4.In case of a system where OSEK OS and OSEKtime OS coexist,     | */
/* |                  | ShutdownHook has to return. <Error> needs to be a valid          | */
/* |                  | error code supported by OSEK OS.                                 | */
/* |                  | 5.In case of a system where OSEK OS and OSEKtime OS coexist,     | */
/* |                  | <Error> might also be a value accepted by OSEKtime OS.           | */
/* |                  | In this case, if enabled by an OSEKtime configuration parameter, | */
/* |                  | OSEKtime OS will be shut down after OSEK OS shutdown.            | */
/* |------------------+------------------------------------------------------------------| */
/* | Particularities: | After this service the operating system is shut down.            | */
/* |                  | Allowed at task level, ISR level, in ErrorHook and StartupHook,  | */
/* |                  | and also called internally by the operating system.              | */
/* |                  | If the operating system calls ShutdownOS it never uses E_OK      | */
/* |                  | as the passed parameter value.                                   | */
/* |------------------+------------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                           | */
/* |------------------+------------------------------------------------------------------| */
void ShutdownOS( StatusType xError )
{
#if (cfgOS_SHUT_DOWN_HOOK == 1)
	ShutdownHook(xError);
#endif
/* OS425: If ShutdownOS() is called and ShutdownHook() returns then the operating
   system shall disable all interrupts and enter an endless loop. */
    vPortDisableInterrupt();
	for ( ; ; )
    {
        /* Dead lopp here */
    }

}
/* For Category 2 ISR, It can Call OS APIs, So ISR2 should notify the OS.
 * So OSEnterISR2() just do this, notify the OS that an ISR2 was in serviced,
 * So no rescheduling was allowed untill the end of ISR2*/
void OSEnterISR(void)
{
    if(OSIsr2Nesting < 0xFF)
    {
        OSIsr2Nesting++ ;
    }
}
/* So When is the End of ISR2?
 * Answer is That when An ISR2 Call OSExitISR2().
 * Now Scheduling was reallowed.
 * Note,OSEnterISR2() and OSExitISR2() should be paired */
void OSExitISR(void)
{
    if(OSIsr2Nesting >0)
    {
        OSIsr2Nesting--;
    }
#if (cfgOS_SCHEDULE_POLICY != NONE_PREEMPTIVE_SCHEDULE)
    if(0x00u == OSIsr2Nesting)
    {
        if(OSCurTsk != OSHighRdyTsk)
        {
#   if(cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
            if(IsTskPreemptable(OSCurTsk))
#   endif
            {
                vPortSwitch2Task();
            }
        }
    }
#endif
}
