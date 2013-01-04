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
/* | File-name:        | Event.c                                      | */
/* |-------------------+----------------------------------------------| */
/* | Reference Manual: | (1)Specification of Operating System(V3.0.2) | */
/* |                   | AUTOSAR_SWS_OS.PDF                           | */
/* |                   |                                              | */
/* |                   | (2)Operating SystemSpecification 2.2.3(OSEK) | */
/* |                   | os223.doc                                    | */
/* |-------------------+----------------------------------------------| */

#include "Kernel.h"



#if(cfgOS_CONFORMANCE_CLASS ==ECC1 ||cfgOS_CONFORMANCE_CLASS ==ECC2)
/* try2ResumeTask() will just change xTaskID into running state
 * And its return value determines whether A Dispatch() should be do 
 * After this call, If true,do Dispatch, else not.
 */
BoolType try2ResumeTask(TaskType xTaskID)
{
    TcbType *pxTcb;
    PriorityType xPriority;
 
    pxTcb=tableGetTskTcb(xTaskID);
    xPriority=pxTcb->xPriority;

    OS_ENTER_CRITICAL();    /* Translate xTaskID into Running State */

    pxTcb->xState=RUNNING;
    listInsertTskAtTail(xPriority,xTaskID);
    
    if(xPriority > OSHighRdyPrio)
    {
        OSHighRdyPrio = xPriority;
        OSHighRdyTcb  = pxTcb;
        OSHighRdyTsk  = xTaskID;
#if (cfgOS_SCHEDULE_POLICY != NONE_PREEMPTIVE_SCHEDULE)
        /* if unequal, then must there is an high priority Task in ready State */
        if( 
#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
            (IsTskPreemptable(OSCurTsk))&&
#endif  /* In ISR level ,Task Switch was not Allowed */
            (OSIsr2Nesting == 0) 
            )
        {
            OS_EXIT_CRITICAL();
            return STD_TRUE;
        }
#endif
    }
    OS_EXIT_CRITICAL();
    return STD_FALSE;
}
/* try2SuspendCurrentTask() doesn't equal to try2TerminateCurrentTask().
 * This API just put current Task into waiting state when OSCurTsk
 * Called WaitEvent(), and no events had been set.*/
void try2SuspendCurrentTask(void)
{
    PriorityType xPriority;
    xPriority = OSCurTcb -> xPriority;
    OS_ENTER_CRITICAL();    /* Translate OSCurTsk into Waiting State */
    listRemoveTskAtHead(xPriority,OSCurTsk);
    OSCurTcb->xState=WAITING;
    OS_EXIT_CRITICAL();
}
/* |------------------+----------------------------------------------------------| */
/* | Syntax:          | StatusType SetEvent ( TaskType <TaskID>                  | */
/* |                  | EventMaskType <Mask> )                                   | */
/* |------------------+----------------------------------------------------------| */
/* | Parameter (In):  | TaskID:Reference to the task for which one or several    | */
/* |                  | events are to be set.                                    | */
/* |                  | Mask:Mask of the events to be set                        | */
/* |------------------+----------------------------------------------------------| */
/* | Parameter (Out): | none                                                     | */
/* |------------------+----------------------------------------------------------| */
/* | Description:     | 1.The service may be called from an interrupt service    | */
/* |                  | routine and from the task level, but not from hook       | */
/* |                  | routines.                                                | */
/* |                  | 2.The events of task <TaskID> are set according to the   | */
/* |                  | event mask <Mask>. Calling SetEvent causes the task      | */
/* |                  | <TaskID> to be transferred to the ready state, if it     | */
/* |                  | was waiting for at least one of the events specified     | */
/* |                  | in <Mask>.                                               | */
/* |------------------+----------------------------------------------------------| */
/* | Particularities: | Any events not set in the event mask remain unchanged.   | */
/* |------------------+----------------------------------------------------------| */
/* | Status:          | Standard: 1.No error, E_OK                               | */
/* |                  | Extended: 2.Task <TaskID> is invalid, E_OS_ID            | */
/* |                  | 3.Referenced task is no extended task, E_OS_ACCESS       | */
/* |                  | 4.Events can not be set as the referenced task is in the | */
/* |                  | suspended state, E_OS_STATE                              | */
/* |------------------+----------------------------------------------------------| */
/* | Conformance:     | ECC1, ECC2                                               | */
/* |------------------+----------------------------------------------------------| */
StatusType SetEvent ( TaskType xTaskID , EventMaskType xMask )
{
    StatusType xRet = E_OK;
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if(xTaskID > (cfgOS_TASK_NUM -1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#if (cfgOS_CONFORMANCE_CLASS == ECC1)
    if(tableGetTskClsType(xTaskID) != ECC1){
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }
#elif(cfgOS_CONFORMANCE_CLASS == ECC2)
    if(tableGetTskClsType(xTaskID) != ECC2){
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }    
#endif
    if(tableGetTskState(xTaskID) == SUSPENDED){
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
#endif

    tableSetEvent(xTaskID,xMask);
    if( (tableGetEvent(xTaskID) & tableGetWaitEvent(xTaskID)) != 0u)
    {                           /* See Particularities 2 */
        tableResetWaitEvent(xTaskID);
        if(STD_TRUE == try2ResumeTask(xTaskID))
        {
            Dispatch();
        }
    }
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
Error_Exit:
#endif
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_SetEvent);
        OSErrorSetParam1(xTaskID);
        OSErrorSetParam2(xMask);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}
/* |------------------+---------------------------------------------------------| */
/* | Syntax:          | StatusType ClearEvent ( EventMaskType <Mask> )          | */
/* |------------------+---------------------------------------------------------| */
/* | Parameter (In)   | Mask:Mask of the events to be cleared                   | */
/* |------------------+---------------------------------------------------------| */
/* | Parameter (Out)  | none                                                    | */
/* |------------------+---------------------------------------------------------| */
/* | Description:     | The events of the extended task calling ClearEvent are  | */
/* |                  | cleared according to the event mask <Mask>.             | */
/* |------------------+---------------------------------------------------------| */
/* | Particularities: | The system service ClearEvent is restricted to extended | */
/* |                  | tasks which own the event.                              | */
/* |------------------+---------------------------------------------------------| */
/* | Status:          | Standard: 1.No error, E_OK                              | */
/* |                  | Extended: 1.Call not from extended task, E_OS_ACCESS    | */
/* |                  | 2.Call at interrupt level, E_OS_CALLEVEL                | */
/* |------------------+---------------------------------------------------------| */
/* | Conformance:     | ECC1, ECC2                                              | */
/* |------------------+---------------------------------------------------------| */
StatusType ClearEvent ( EventMaskType xMask )
{
    StatusType xRet = E_OK;
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if(OSIsr2Nesting > 0)
    {
        xRet = E_OS_CALLEVEL;
        goto Error_Exit;
    }
#if (cfgOS_CONFORMANCE_CLASS == ECC1)
    if(tableGetTskClsType(OSCurTsk) != ECC1){
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }
#elif(cfgOS_CONFORMANCE_CLASS == ECC2)
    if(tableGetTskClsType(OSCurTsk) != ECC2){
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }    
#endif
#endif
    tableClearEvent(OSCurTsk,xMask);
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
Error_Exit:
#endif
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_ClearEvent);
        OSErrorSetParam1(xMask);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}

/* |------------------+--------------------------------------------------------------| */
/* | Syntax:          | StatusType GetEvent ( TaskType <TaskID>                      | */
/* |                  | EventMaskRefType <Event> )                                   | */
/* |------------------+--------------------------------------------------------------| */
/* | Parameter (In):  | TaskID:Task whose event mask is to be returned.              | */
/* |------------------+--------------------------------------------------------------| */
/* | Parameter (Out): | Event:Reference to the memory of the return data.            | */
/* |------------------+--------------------------------------------------------------| */
/* | Description:     | 1.This service returns the current state of all event bits   | */
/* |                  | of the task <TaskID>, not the events that the task is        | */
/* |                  | waiting for.                                                 | */
/* |                  | 2.The service may be called from interrupt service routines, | */
/* |                  | task level and some hook routines (see Figure 12-1).         | */
/* |                  | 3.The current status of the event mask of task <TaskID> is   | */
/* |                  | copied to <Event>.                                           | */
/* |------------------+--------------------------------------------------------------| */
/* | Particularities: | The referenced task shall be an extended task.               | */
/* |------------------+--------------------------------------------------------------| */
/* | Status:          | Standard: No error, E_OK                                     | */
/* |                  | Extended: Task <TaskID> is invalid, E_OS_ID                  | */
/* |                  | Referenced task <TaskID> is not an extended task,            | */
/* |                  | E_OS_ACCESS                                                  | */
/* |                  | Referenced task <TaskID> is in the suspended state,          | */
/* |                  | E_OS_STATE                                                   | */
/* |------------------+--------------------------------------------------------------| */
/* | Conformance:     | ECC1, ECC2                                                   | */
/* |------------------+--------------------------------------------------------------| */
StatusType GetEvent ( TaskType xTaskID , EventMaskRefType pxEvent )
{
    StatusType xRet = E_OK;
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if(xTaskID > (cfgOS_TASK_NUM -1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#if (cfgOS_CONFORMANCE_CLASS == ECC1)
    if(tableGetTskClsType(xTaskID) != ECC1){
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }
#elif(cfgOS_CONFORMANCE_CLASS == ECC2)
    if(tableGetTskClsType(xTaskID) != ECC2){
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }    
#endif
    if(tableGetTskState(xTaskID) == SUSPENDED){
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
#endif
    *pxEvent=tableGetEvent(xTaskID);
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
Error_Exit:
#endif
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_GetEvent);
        OSErrorSetParam1(xTaskID);
        OSErrorSetParam2(pxEvent);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}


/* |------------------+------------------------------------------------------------| */
/* | Syntax:          | StatusType WaitEvent ( EventMaskType <Mask> )              | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (In):  | Mask:Mask of the events waited for.                        | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (Out): | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Description:     | The state of the calling task is set to waiting, unless    | */
/* |                  | at least one of the events specified in <Mask> has         | */
/* |                  | already been set.                                          | */
/* |------------------+------------------------------------------------------------| */
/* | Particularities: | 1.This call enforces rescheduling, if the wait condition   | */
/* |                  | occurs. If rescheduling takes place, the internal resource | */
/* |                  | of the task is released while the task is in the waiting   | */
/* |                  | state.                                                     | */
/* |                  | 2.This service shall only be called from the extended task | */
/* |                  | owning the event.                                          | */
/* |------------------+------------------------------------------------------------| */
/* | Status:          | Standard:No error, E_OK                                    | */
/* |                  | Extended:Calling task is not an extended task, E_OS_ACCESS | */
/* |                  | Calling task occupies resources, E_OS_RESOURCE             | */
/* |                  | Call at interrupt level, E_OS_CALLEVEL                     | */
/* |------------------+------------------------------------------------------------| */
/* | Conformance:     | ECC1, ECC2                                                 | */
/* |------------------+------------------------------------------------------------| */
StatusType WaitEvent( EventMaskType xMask )
{
    StatusType xRet = E_OK;
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    if(OSGetContextLevel() != TASK_LEVEL){
        xRet = E_OS_CALLEVEL;
        goto Error_Exit;
    }
    
#  if (cfgOS_CONFORMANCE_CLASS == ECC1) || (cfgOS_CONFORMANCE_CLASS == ECC2)
    if(tableGetTskClsType(OSCurTsk) != EXTEND_TASK){
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }    
#  endif

#  if (cfgOS_USE_RESOURCE == STD_TRUE)
    if(OSCurTcb->xCurResID != INVALID_RESOURCE)
    {
        xRet = E_OS_RESOURCE;
        goto Error_Exit;
    }
#  endif
#endif
    if((xMask & tableGetEvent(OSCurTsk)) == 0x0000u)
    { /* No Events specified in xMask has been set */
        tableSetWaitEvent(OSCurTsk,xMask);
#if (cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
		if (OSCurTsk < cfgOS_TASK_WITH_IN_RES_NUM)
		{
			ReleaseInResource();
		}
#endif
        try2SuspendCurrentTask();
        doSchedule();
#if (cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
		if (OSCurTsk < cfgOS_TASK_WITH_IN_RES_NUM)
		{
			GetInResource();
		}
#endif
    }
    /* else, has at least one event specified in xMask has already been set,
     * just return  */
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
Error_Exit:
#endif
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_WaitEvent);
        OSErrorSetParam1(xMask);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}
#endif /* cfgOS_CONFORMANCE_CLASS ==ECC1 ||cfgOS_CONFORMANCE_CLASS ==ECC2 */

