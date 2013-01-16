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

/* |--------------------------------------+----------------| */
/* | History:                             | Year-Month-Day | */
/* |--------------------------------------+----------------| */
/* | Creat this file and decide to Design |      2012-9-27 | */
/* | the AutoSAR OS.                      |                | */
/* |--------------------------------------+----------------| */


/* |-------------------+----------------------------------------------| */
/* | File-name:        | Task.c                                       | */
/* |-------------------+----------------------------------------------| */
/* | Reference Manual: | (1)Specification of Operating System(V3.0.2) | */
/* |                   | AUTOSAR_SWS_OS.PDF                           | */
/* |                   |                                              | */
/* |                   | (2)Operating SystemSpecification 2.2.3(OSEK) | */
/* |                   | os223.doc                                    | */
/* |-------------------+----------------------------------------------| */
#define OS_GLOBALS	/* generate the global values for kernel here  */
#include "Kernel.h"

#if((cfgOS_MULTIPLY_ACTIVATE == STD_TRUE) && (cfgOS_SUM_ACTIVATES > 0))
/* listInsertTskAtHead() and listInsertTskAtTail() are group static APIS,
   which will be included when queue method for per priority implemented.
   Then tasks will be scheduled according to the time when activated on 
   per priority.*/
void listInsertTskAtHead(PriorityType xPriority,TaskType xTaskID)
{
    TaskRefType xQueue;
    uint8_t     xHead;
    uint8_t     xSize;
    xQueue = OSTskRdyQueuePtr[xPriority]; /* Get the queue */
    xSize  = OSTskRdyQueueSize[xPriority]; /* Get the queue size */
    xHead  = OSTskRdyListHead[xPriority];  /* Get the queue head */
    xHead--;
    if(xHead >= xSize) 
    {   /* if out of range,set it to end of the queue */
        xHead = xSize-1;
    }
    xQueue[xHead] = xTaskID;
    OSTskRdyListHead[xPriority] = xHead;
}
void listInsertTskAtTail(PriorityType xPriority,TaskType xTaskID)
{
    TaskRefType xQueue;
    uint8_t     xHead;
    uint8_t     xTail;
    uint8_t     xSize;
    xQueue = OSTskRdyQueuePtr[xPriority]; /* Get the queue */
    xSize  = OSTskRdyQueueSize[xPriority]; /* Get the queue size */
    xHead  = OSTskRdyListHead[xPriority];  /* Get the queue head */
    xTail  = OSTskRdyListTail[xPriority];  /* Get the queue tail */
    if(xHead == xTail)
    {                           /* Empty */
        setReadyTableMap((xPriority));
    }
    xQueue[xTail] = xTaskID;
    xTail++;
    if(xTail >= xSize)   
    {   /* if out of range, set it to head of queue */
        xTail = 0;
    }
    OSTskRdyListTail[xPriority] = xTail;
}
/* When task terminate or release the usage of cpu ,
   it should be removed from the ready list or queue */
void listRemoveTskAtHead(PriorityType xPriority,TaskType xTaskID)
{
    uint8_t     xHead;
    uint8_t     xTail;
    uint8_t     xSize;
    (void)xTaskID;              /* Surpress The Warning. */
    xSize  = OSTskRdyQueueSize[xPriority];
    xHead  = OSTskRdyListHead[xPriority];
    xTail  = OSTskRdyListTail[xPriority];
  
    xHead++;
    if(xHead >= xSize)
    {
        xHead = 0;
    }
    OSTskRdyListHead[xPriority] = xHead;
    if(xHead == xTail)
    {                           /* Empty */
        clearReadyTableMap((xPriority));
    }
}
/* Get the first ready task in the list or queue according to proority */
TaskType listGetRdyTsk(PriorityType xPriority)
{
    uint8_t xHead;
    TaskRefType xQueue;
    xHead = OSTskRdyListHead[xPriority];
    xQueue = OSTskRdyQueuePtr[xPriority];
    return xQueue[xHead];
}
#endif  /* cfgOS_SUM_ACTIVATES && cfgOS_MULTIPLY_ACTIVATE */

/* doSchedule() will do a Task Switch to a high priority task
 * in ready state.It don't care wether OSCurTask are
 * Preemptable or not,or INVALID_TASK.
 * Note OSCurTsk also may be the highest priority Task in READY state. 
 * in ready or running state*/
void doSchedule(void)
{
    /* The Situation that when the SEVERAL_TASKS_PER_PRIORITY Policy was adopted,
     * there may be a task or more with the same xPriority as OSCurTsk in the
     * ready state*/
    if(OSCurTsk == OSHighRdyTsk)
    {   /* higher priority task in ready or running state */
        /* Search the table map to find  OSHighPriority*/
        OS_ENTER_CRITICAL();
#if (cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY)
        if(listTskRdyIsEmpty(OSCurTcb->xPriority))
        { /* if OSTskRdyListHead[xPriority] is Empty, */
#endif    /* Find out an Runnable Task */
            OSHighRdyPrio = tableRdyMapFindHighPriority();
#if (cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY)
        } /* if OSTcbRdyGrpList[xPriority] is not Empty, */
#endif    /* make the head task runnable */
        /* Get The Tcb with the highest Priority*/
        OSHighRdyTsk = listGetRdyTsk(OSHighRdyPrio);
        OSHighRdyTcb = &OSTcbTable[OSHighRdyTsk];
        OS_EXIT_CRITICAL();
    }
    Dispatch();
}
/* try2TerminateCurrentTask() is a function only change the state of
 * Current Task if The condition to termiante the task was fullfilled,
 * don't do Schedule()*/
void try2TerminateCurrentTask(void)
{
    PriorityType xPriority;

    OS_ENTER_CRITICAL();
    xPriority=OSCurTcb->xPriority;

#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
    OSCurTcb->xActivateCount--;
    /* The Judge Bellow has been omitted, AS if the Policy Multiply Activate
     * was Enabled, Only Task BCC2's xActivateCount can be increeased,
     * So No Problem to Terminate the Task if its xactivatecount is 0*/
    /* if( (OSTaskCLass[OSCurTask]== BCC2)) */
    /* {  */             /* Only Basic Task(BCC2) can be multiply activated*/
    if(0u == OSCurTcb->xActivateCount)
    {            /*Trurly The Condition to Terminate Current Task  is OK*/
#endif               /* Atomic Operation To terminate Current Task */
        listRemoveTskAtHead(xPriority,OSCurTsk);
        OSCurTcb->xState=SUSPENDED;
        /* Terminate Done! */    
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
    }
    else  /* xActivateCount hasn't reached ZERO,So the task cann't be terminated, */
    {     /* The task should be reset to ready state */
        /* Only Reset Current Task's State, Then the Task can be restarted later
         * When its Condition is fullfilled*/
        OSCurTcb->xState=READY;
        OSCurTcb->pxStack=OSTaskStackTable[OSCurTsk].pxStackTop;
#  if(cfgOS_SUM_ACTIVATES > 0)
        listRemoveTskAtHead(xPriority,OSCurTsk);
#  endif
    }
#endif
    OS_EXIT_CRITICAL();
}

/* Like try2TerminateCurrentTask(), try2ActivateTask() also only change the 
 * the state of xTaskID, don't do Schedule() */
BoolType try2ActivateTask( TaskType xTaskID )
{
	PriorityType xPriority;
    TcbType* pxTcb;
    /* Get the xPriority of the Task which will be activated */
    pxTcb=&OSTcbTable[xTaskID];
    xPriority = tableGetTskInitPrio(xTaskID);
    OS_ENTER_CRITICAL();        /* The Operation below should be atomic */
    if(pxTcb->xState == SUSPENDED)
    {        
#if(cfgOS_TASK_PER_PRIORITY == ONLY_ONE_TASK_PER_PRIORITY)
        /* Only One Task Per Priority */
        /* So if another Task with the Same Priority has try to be 
         * activated ,It's an error and not allowed, so Return.
         * It's an configuration error.*/
        OS_ASSERT(listTskRdyIsEmpty(xPriority));
#endif 
        listInsertTskAtTail(xPriority,xTaskID);
        pxTcb->pxStack = tableGetTskInitStkTop(xTaskID);
        pxTcb->xPriority = xPriority;
        pxTcb->xState  = READY;
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
        pxTcb->xActivateCount = 1;
#endif

#if(cfgOS_CONFORMANCE_CLASS == ECC1 || cfgOS_CONFORMANCE_CLASS ==ECC2)
        /* Don't Check Wether xTaskID is Extended Task or not,
         * As the judge waste more time then only do tableResetWaitEvent()*/
        tableResetWaitEvent(xTaskID);
#endif
        /* if the newly activated task was the highest priority,then do an indication
         * to doSchedule(), that is to update OSHighPriority and OSHighPrioTcb */
        if((xPriority>OSHighRdyPrio) || (OSHighRdyPrio==INVALID_PRIORITY)) 
        {                           
            OSHighRdyPrio = xPriority;
            OSHighRdyTcb  = pxTcb;
            OSHighRdyTsk  = xTaskID;
#if (cfgOS_SCHEDULE_POLICY != NONE_PREEMPTIVE_SCHEDULE)
            if( 
                (OSGetContextLevel() == TASK_LEVEL)
# if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
                &&IsTskPreemptable(OSCurTsk)
# endif  /* In ISR level ,Task Switch was not Allowed */
                )
            {
                OS_EXIT_CRITICAL();
                return STD_TRUE;
            }
#endif        
        }
    }
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
    else                        /* pxTcb->xState != SUSPENDED */
    {   /* the activated xTaskID has already been in ready or waiting state,As its priority
         * not the highest of all tasks in ready state or xTaskID is OSCurTsk*/
        if(pxTcb->xActivateCount  < tableGetTskMaxActCnt(xTaskID))
        {						/* Only For BCC2 */
            pxTcb->xActivateCount++;
#  if(cfgOS_SUM_ACTIVATES > 0)
            listInsertTskAtTail(xPriority,xTaskID);
#  endif
        }
    }
#endif
    OS_EXIT_CRITICAL();         /* Activate Done! */
    return STD_FALSE;
}

/**************************************************
#include <stdio.h>
int main(int argc, char *argv[])
{
    unsigned int i;
    printf("static uint8_t tableUnMap[256]=\n{");
    for (i=0; i <= 0xff; ++i)
    {
        if(i%16==0) printf("\n\t");
        if(i&(1u<<7))
            printf("7,");
        else if(i&(1u<<6))
            printf("6,");
        else if(i&(1u<<5))
            printf("5,");
        else if(i&(1u<<4))
            printf("4,");
        else if(i&(1u<<3))
            printf("3,");
        else if(i&(1u<<2))
            printf("2,");
        else if(i&(1u<<1))
            printf("1,");
        else if(i&(1u<<0))
            printf("0,");
        else printf("0,");      
    }
    printf("\n}\n");
    return 0;
}
used to generate the map like ucos,but inverted from low to high
*******************************************************/

/* use OSReadyGrp and OSReadyTable[] to find the highest priority with a task or a group
 * of tasks with the same priority that is in the ready state.
 * Common: the biggest the number it is ,the highest the proority it is. 
 */
#if 1                           /* Use ucOS methon,recommended */
PriorityType tableRdyMapFindHighPriority(void)
{
    uint8_t x;
    uint8_t y;
    static uint8_t tableUnMap[256]=
        {
            0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
            4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
            5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
            5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        };
    if( OSReadyGrp != 0x00u)
    {
        y=tableUnMap[OSReadyGrp];
    }
    else
    {
        return INVALID_PRIORITY; /* No Task is in RUNNING or READY state */
    }
    x=tableUnMap[OSReadyTable[y]];
 
    return (PriorityType)((y<<3)+x);
}
#else  /* use rtThread method */
PriorityType tableRdyMapFindHighPriority(void)
{
    uint8_t x;
    uint8_t y;
    static uint8_t tableUnMap[16]=
        {
            0u, 0u, 1u, 1u,                /* 0 , 1 , 2 , 3  */
            2u, 2u, 2u, 2u,                /* 4 , 5 , 6 , 7  */
            3u, 3u, 3u, 3u,                /* 8 , 9 , 10, 11 */
            3u, 3u, 3u, 3u                 /* 12 ,13, 14, 15 */
        };
   
#if(cfgOS_MAX_PRIORITY > 31)
    if( (OSReadyGrp&0xF0u) != 0x00u )
    {
        y=4+tableUnMap[OSReadyGrp>>4];
    }
    else
#endif
        if( (OSReadyGrp&0x0Fu) != 0x00u)
        {
            y=tableUnMap[OSReadyGrp&0x0Fu];
        }
        else
        {
            return INVALID_PRIORITY; /* No Task is in RUNNING or READY state */
        }
    
    x=OSReadyTable[y];
    if( (x&0xF0u) != 0x00u )
    {
        x=4+tableUnMap[x>>4];
    }
    else 
    {
        x=tableUnMap[x&0x0Fu];
    }
    return (PriorityType)((y<<3)+x);
}
#endif  /* End of tableRdyMapFindHighPriority */
/* |------------------+------------------------------------------------------------| */
/* | Syntax:          | StatusType ActivateTask ( TaskType <TaskID> )              | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (In):  | TaskID: Task reference                                     | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (Out): | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Description:     | The task <TaskID> is transferred from the suspended state  | */
/* |                  | into the ready state. The operating system ensures that    | */
/* |                  | the task code is being executed from the first statement.  | */
/* |------------------+------------------------------------------------------------| */
/* | Particularities: | 1) The service may be called from interrupt level and from | */
/* |                  | task level (see Figure 12-1(os223.doc)).                   | */
/* |                  | 2) Rescheduling after the call to ActivateTask depends on  | */
/* |                  | the place it is called from (ISR, non preemptable task,    | */
/* |                  | preemptable task).                                         | */
/* |                  | 3)If E_OS_LIMIT is returned the activation is ignored.     | */
/* |                  | 4)When an extended task is transferred from suspended      | */
/* |                  | state into ready state all its events are cleared.         | */
/* |------------------+------------------------------------------------------------| */
/* | Status:          | Standard:                                                  | */
/* |                  | 1) No error, E_OK                                          | */
/* |                  | 2) Too many task activations of <TaskID>, E_OS_LIMIT       | */
/* |                  | Extended:                                                  | */
/* |                  | 1) Task <TaskID> is invalid, E_OS_ID                       | */
/* |------------------+------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                     | */
/* |------------------+------------------------------------------------------------| */
StatusType ActivateTask ( TaskType xTaskID )
{
    StatusType xRet=E_OK;

    OS_ASSERT( STD_TRUE == OSRunning );

    /* As the Tasks's ID are sequential from 0 to (OS_TASK_NUM-1),
     * if the parameter xTaskID are not in the scope ,then an 
     * error happened
     */
#if( cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if( xTaskID > (cfgOS_TASK_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif

    if(tableGetTskState(xTaskID) != SUSPENDED)
    {   /* Task has already been in state ready or running */
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
        if(tableGetTskActCnt(xTaskID)  >= tableGetTskMaxActCnt(xTaskID))
        {
#endif
            xRet = E_OS_LIMIT;
            goto Error_Exit;
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
        }
#endif
    }
    /* Really Activate the Task */
    if(STD_TRUE == try2ActivateTask(xTaskID))
    {
#if(cfgOS_POST_TASK_HOOK == 1)
        OS_ENTER_CRITICAL();
        PostTaskHook();
        OS_EXIT_CRITICAL();
#endif
        Dispatch();
#if(cfgOS_PRE_TASK_HOOK == 1)
        OS_ENTER_CRITICAL();
        PreTaskHook();
        OS_EXIT_CRITICAL();
#endif
    }
  Error_Exit:
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_ActivateTask);
        OSErrorSetParam1(xTaskID);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}
/* |------------------+--------------------------------------------------------------| */
/* | Syntax:          | StatusType TerminateTask ( void )                            | */
/* |------------------+--------------------------------------------------------------| */
/* | Parameter (In):  | none                                                         | */
/* |------------------+--------------------------------------------------------------| */
/* | Parameter (Out): | none                                                         | */
/* |------------------+--------------------------------------------------------------| */
/* | Description:     | This service causes the termination of the calling task. The | */
/* |                  | calling task is transferred from the running state into the  | */
/* |                  | suspended state.                                             | */
/* |------------------+--------------------------------------------------------------| */
/* | Particularities: | 1) An internal resource assigned to the calling task is      | */
/* |                  | automatically released. Other resources occupied by the task | */
/* |                  | shall have been released before the call to TerminateTask.   | */
/* |                  | If a resource is still occupied in standard status the       | */
/* |                  | behaviour is undefined.                                      | */
/* |                  | 2) If the call was successful, TerminateTask does not return | */
/* |                  | to the call level and the status can not be evaluated.       | */
/* |                  | 3) If the version with extended status is used, the service  | */
/* |                  | returns in case of error, and provides a status which can be | */
/* |                  | evaluated in the application.                                | */
/* |                  | 4) If the service TerminateTask is called successfully, it   | */
/* |                  | enforces a rescheduling.                                     | */
/* |                  | 5) Ending a task function without call to TerminateTask or   | */
/* |                  | ChainTask is strictly forbidden and may leave the system in  | */
/* |                  | an undefined state.                                          | */
/* |------------------+--------------------------------------------------------------| */
/* | Status:          | Standard:                                                    | */
/* |                  | 1)No return to call level                                    | */
/* |                  | Extended:                                                    | */
/* |                  | 1) Task still occupies resources, E_OS_RESOURCE              | */
/* |                  | 2) Call at interrupt level, E_OS_CALLEVEL                    | */
/* |------------------+--------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                       | */
/* |------------------+--------------------------------------------------------------| */
StatusType TerminateTask ( void )
{
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    StatusType xRet = E_OK;
#endif    
    OS_ASSERT( STD_TRUE==OSRunning );

#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    if(OSGetContextLevel() != TASK_LEVEL)
    {
        xRet = E_OS_CALLEVEL;
        goto Error_Exit;
    }
# if (cfgOS_USE_RESOURCE == STD_TRUE)
    if(OSCurTcb->xCurResID != INVALID_RESOURCE)
    {
        xRet = E_OS_RESOURCE;
        goto Error_Exit;
    }
# endif
#endif

#if (cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
    if (OSCurTsk < cfgOS_TASK_WITH_IN_RES_NUM)
    {

        ReleaseInResource();
    }
#endif

#if(cfgOS_POST_TASK_HOOK == 1)
    OS_ENTER_CRITICAL();
    PostTaskHook();
    OS_EXIT_CRITICAL();
#endif
    /* terminate current task and do schedule */
    try2TerminateCurrentTask();
    doSchedule();
    OS_ASSERT(STD_FALSE);       /* Shouldn't Return here */

#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
  Error_Exit:
#if(cfgOS_ERROR_HOOK == 1)
    if(xRet != E_OK)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_TerminateTask);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
#endif         
}
/* |------------------+-------------------------------------------------------------| */
/* | Syntax:          | StatusType ChainTask ( TaskType <TaskID> )                  | */
/* |------------------+-------------------------------------------------------------| */
/* | Parameter (In):  | TaskID Reference to the sequential succeeding task to       | */
/* |                  | be activated.                                               | */
/* |------------------+-------------------------------------------------------------| */
/* | Parameter (Out): | none                                                        | */
/* |------------------+-------------------------------------------------------------| */
/* | Description:     | This service causes the termination of the calling task.    | */
/* |                  | After termination of the calling task a succeeding task     | */
/* |                  | <TaskID> is activated. Using this service, it ensures       | */
/* |                  | that the succeeding task starts to run at the earliest      | */
/* |                  | after the calling task has been terminated.                 | */
/* |------------------+-------------------------------------------------------------| */
/* | Particularities: | 1. If the succeeding task is identical with the current     | */
/* |                  | task, this does not result in multiple requests. The task   | */
/* |                  | is not transferred to the suspended state, but will         | */
/* |                  | immediately become ready again.                             | */
/* |                  | 2. An internal resource assigned to the calling task is     | */
/* |                  | automatically released, even if the succeeding task is      | */
/* |                  | identical with the current task. Other resources occupied   | */
/* |                  | by the calling shall have been released before ChainTask    | */
/* |                  | is called. If a resource is still occupied in standard      | */
/* |                  | status the behaviour is undefined.                          | */
/* |                  | 3. If called successfully, ChainTask does not return to     | */
/* |                  | the call level and the status can not be evaluated.         | */
/* |                  | 4. In case of error the service returns to the calling      | */
/* |                  | task and provides a status which can then be evaluated      | */
/* |                  | in the application.                                         | */
/* |                  | 5.If the service ChainTask is called successfully, this     | */
/* |                  | enforces a rescheduling.                                    | */
/* |                  | 6. Ending a task function without call to TerminateTask     | */
/* |                  | or ChainTask is strictly forbidden and may leave the system | */
/* |                  | in an undefined state.                                      | */
/* |                  | 7. If E_OS_LIMIT is returned the activation is ignored.     | */
/* |                  | 8. When an extended task is transferred from suspended      | */
/* |                  | state into ready state all its events are cleared.          | */
/* |------------------+-------------------------------------------------------------| */
/* | Status:          | Standard:                                                   | */
/* |                  | 1. No return to call level                                  | */
/* |                  | 2. Too many task activations of <TaskID>, E_OS_LIMIT        | */
/* |                  | Extended:                                                   | */
/* |                  | 1. Task <TaskID> is invalid, E_OS_ID                        | */
/* |                  | 2. Calling task still occupies resources, E_OS_RESOURCE     | */
/* |                  | 3. Call at interrupt level, E_OS_CALLEVEL                   | */
/* |------------------+-------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                      | */
/* |------------------+-------------------------------------------------------------| */
StatusType ChainTask ( TaskType xTaskID )
{
    StatusType xRet = E_OK;
    /* As the Tasks's ID are sequential from 0 to (OS_TASK_NUM-1),
     * if the parameter xTaskID are not in the scope ,then an 
     * error happened
     */
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    if( xTaskID > (cfgOS_TASK_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    if(OSGetContextLevel() != TASK_LEVEL)
    {
        xRet = E_OS_CALLEVEL;
        goto Error_Exit;
    }
#   if (cfgOS_USE_RESOURCE == STD_TRUE)
    if(OSCurTcb->xCurResID != INVALID_RESOURCE)
    {
        xRet = E_OS_RESOURCE;
        goto Error_Exit;
    }
#   endif
#endif

    if(tableGetTskState(xTaskID) != SUSPENDED)
    {   /* Task has already been in state ready or running */
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
        if(tableGetTskActCnt(xTaskID)  >= tableGetTskMaxActCnt(xTaskID))
        {
#endif
            xRet = E_OS_LIMIT;
            goto Error_Exit;
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
        }
#endif
    }

#if (cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
    if (OSCurTsk < cfgOS_TASK_WITH_IN_RES_NUM)
    {
        ReleaseInResource();
    }
#endif

#if(cfgOS_POST_TASK_HOOK == 1)
    OS_ENTER_CRITICAL();
    PostTaskHook();
    OS_EXIT_CRITICAL();
#endif
    /* Do Termination of OSCurTask */
    if( xTaskID != OSCurTsk )
    {
        try2TerminateCurrentTask();
        if(STD_TRUE == try2ActivateTask(xTaskID))
        {   /* IF The Activated xTaskID was the highest Priority Task */
            Dispatch();
            OS_ASSERT(STD_FALSE);
        }
    }
    else /* if( xTaskID == OSCurTsk ) */
    {                           /* see  Particularities: 1 */
        /* Only Rest Current Task's State, Then the Task can be restarted later
         * When its Condition is fullfilled*/
        OS_ENTER_CRITICAL();
        OSCurTcb->xState=READY;
        OSCurTcb->pxStack=tableGetTskInitStkTop(xTaskID);
        OS_EXIT_CRITICAL();
    }
    doSchedule();/* Now A Task Switch will be performed */ 
    OS_ASSERT(STD_FALSE);
  Error_Exit:
#if(cfgOS_ERROR_HOOK == 1)
    if(xRet != E_OK)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_TerminateTask);
        OSErrorSetParam1(xTaskID);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}
/* |------------------+-------------------------------------------------------------| */
/* | Syntax:          | StatusType Schedule ( void )                                | */
/* |------------------+-------------------------------------------------------------| */
/* | Parameter (In):  | none                                                        | */
/* |------------------+-------------------------------------------------------------| */
/* | Parameter (Out): | none                                                        | */
/* |------------------+-------------------------------------------------------------| */
/* | Description:     | If a higher-priority task is ready, the internal resource   | */
/* |                  | of the task is released, the current task is put into the   | */
/* |                  | ready state, its context is saved and the higher-priority   | */
/* |                  | task is executed. Otherwise the calling task is continued.  | */
/* |------------------+-------------------------------------------------------------| */
/* | Particularities: | Rescheduling only takes place if the task an internal       | */
/* |                  | resource is assigned to the calling task during             | */
/* |                  | system generation. For these tasks, Schedule enables a      | */
/* |                  | processor assignment to other tasks with lower or equal     | */
/* |                  | priority than the ceiling priority of the internal resource | */
/* |                  | and higher priority than the priority of the calling task   | */
/* |                  | in application-specific locations. When returning from      | */
/* |                  | Schedule, the internal resource has been taken again.       | */
/* |                  | This service has no influence on tasks with no internal     | */
/* |                  | resource assigned (preemptable tasks).                      | */
/* |------------------+-------------------------------------------------------------| */
/* | Status:          | Standard:                                                   | */
/* |                  | 1. No error, E_OK                                           | */
/* |                  | Extended:                                                   | */
/* |                  | 1. Call at interrupt level, E_OS_CALLEVEL                   | */
/* |                  | 2. Calling task occupies resources, E_OS_RESOURCE           | */
/* |------------------+-------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                      | */
/* |------------------+-------------------------------------------------------------| */
StatusType Schedule ( void )
{
    StatusType xRet = E_OK;
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    if(OSGetContextLevel() != TASK_LEVEL)
    {
        xRet = E_OS_CALLEVEL;
        goto Error_Exit;
    }
#  if (cfgOS_USE_RESOURCE == STD_TRUE)
    if(OSCurTcb->xCurResID != INVALID_RESOURCE)
    {
        xRet = E_OS_RESOURCE;
        goto Error_Exit;
    }
#  endif
#endif

    if(OSCurTsk != OSHighRdyTsk)
    {
#if(cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
        if(OSCurTsk < cfgOS_TASK_WITH_IN_RES_NUM)
        {
            ReleaseInResource();
        }
#endif
#if(cfgOS_POST_TASK_HOOK == 1)
        OS_ENTER_CRITICAL();
        PostTaskHook();
        OS_EXIT_CRITICAL();
#endif
        Dispatch();
#if(cfgOS_PRE_TASK_HOOK == 1)
        OS_ENTER_CRITICAL();
        PreTaskHook();
        OS_EXIT_CRITICAL();
#endif
#if(cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
        if(OSCurTsk < cfgOS_TASK_WITH_IN_RES_NUM)
        {
            GetInResource();
        }
#endif        
    }
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
  Error_Exit:
#endif
#if(cfgOS_ERROR_HOOK == 1)
    if(xRet != E_OK)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_Schedule);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}
/* |------------------+---------------------------------------------------------| */
/* | Syntax:          | StatusType GetTaskID ( TaskRefType <TaskID> )           | */
/* |------------------+---------------------------------------------------------| */
/* | Parameter (In):  | none                                                    | */
/* |------------------+---------------------------------------------------------| */
/* | Parameter (Out): | TaskID Reference to the task which is currently running | */
/* |------------------+---------------------------------------------------------| */
/* | Description:     | GetTaskID returns the information about the TaskID of   | */
/* |                  | the task which is currently running.                    | */
/* |------------------+---------------------------------------------------------| */
/* | Particularities: | 1. Allowed on task level, ISR level and in several hook | */
/* |                  | routines (see Figure 12-1(os223)).                      | */
/* |                  | 2. This service is intended to be used by library       | */
/* |                  | functions and hook routines.                            | */
/* |                  | 3. If <TaskID> can't be evaluated (no task currently    | */
/* |                  | running), the service returns INVALID_TASK as TaskType. | */
/* |------------------+---------------------------------------------------------| */
/* | Status:          | Standard:  No error, E_OK                               | */
/* |                  | Extended:  No error, E_OK                               | */
/* |------------------+---------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                  | */
/* |------------------+---------------------------------------------------------| */
StatusType GetTaskID ( TaskRefType pxTask )
{
    *pxTask=OSCurTsk;
	return E_OK;
}
/* |------------------+-------------------------------------------------------| */
/* | Syntax:          | StatusType GetTaskState ( TaskType <TaskID>,          | */
/* |                  | TaskStateRefType <State> )                            | */
/* |------------------+-------------------------------------------------------| */
/* | Parameter (In):  | TaskID: Task reference                                | */
/* |------------------+-------------------------------------------------------| */
/* | Parameter (Out): | State: Reference to the state of the task <TaskID>    | */
/* |------------------+-------------------------------------------------------| */
/* | Description:     | Returns the state of a task (running, ready, waiting, | */
/* |                  | suspended) at the time of calling GetTaskState.       | */
/* |------------------+-------------------------------------------------------| */
/* | Particularities: | The service may be called from interrupt service      | */
/* |                  | routines, task level, and some hook routines (see     | */
/* |                  | Figure 12-1(os223.doc)). When a call is made from a   | */
/* |                  | task in a full preemptive system, the result may      | */
/* |                  | already be incorrect at the time of evaluation.       | */
/* |                  | When the service is called for a task, which is       | */
/* |                  | activated more than once, the state is set to         | */
/* |                  | running if any instance of the task is running.       | */
/* |------------------+-------------------------------------------------------| */
/* | Status:          | Standard: No error, E_OK                              | */
/* |                  | Extended: Task <TaskID> is invalid, E_OS_ID           | */
/* |------------------+-------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                | */
/* |------------------+-------------------------------------------------------| */
StatusType GetTaskState ( TaskType xTaskID,TaskStateRefType pxState )
{
    StatusType xRet = E_OK;
    TaskStateType xState;
    /* As the Tasks's ID are sequential from 0 to (OS_TASK_NUM-1),
     * if the parameter xTaskID are not in the scope ,then an 
     * error happened
     */
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    if( xTaskID > (cfgOS_TASK_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    xState = tableGetTskState(xTaskID);
    if(RUNNING == xState)
    {
        if(OSCurTsk != xTaskID)
        {
            xState = READY;        
        }
    }
    *pxState = xState;
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
  Error_Exit:
#endif
#if(cfgOS_ERROR_HOOK == 1)
    if(xRet != E_OK)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_GetTaskState);
        OSErrorSetParam1(xTaskID);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}

