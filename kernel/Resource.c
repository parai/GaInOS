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

#include "Kernel.h"

#if (cfgOS_USE_RESOURCE == STD_TRUE)
/* |------------------+-----------------------------------------------------------| */
/* | Syntax:          | StatusType GetResource ( ResourceType <ResID> )           | */
/* |------------------+-----------------------------------------------------------| */
/* | Parameter (In):  | ResID:Reference to resource                               | */
/* |------------------+-----------------------------------------------------------| */
/* | Parameter (Out): | none                                                      | */
/* |------------------+-----------------------------------------------------------| */
/* | Description:     | This call serves to enter critical sections in the code   | */
/* |                  | that are assigned to the resource referenced by <ResID>.  | */
/* |                  | A critical section shall always be left using             | */
/* |                  | ReleaseResource.                                          | */
/* |------------------+-----------------------------------------------------------| */
/* | Particularities: | 1.The OSEK priority ceiling protocol for resource         | */
/* |                  | management is described in chapter 8.5.                   | */
/* |                  | 2.Nested resource occupation is only allowed if the       | */
/* |                  | inner critical sections are completely executed within    | */
/* |                  | the surrounding critical section (strictly stacked,       | */
/* |                  | see chapter 8.2, Restrictions when using resources).      | */
/* |                  | Nested occupation of one and the same resource is         | */
/* |                  | also forbidden!                                           | */
/* |                  | 3.It is recommended that corresponding calls to           | */
/* |                  | GetResource and ReleaseResource appear within the         | */
/* |                  | same function.                                            | */
/* |                  | 4.It is not allowed to use services which are points      | */
/* |                  | of rescheduling for non preemptable tasks (TerminateTask, | */
/* |                  | ChainTask, Schedule and WaitEvent, see chapter 4.6.2)     | */
/* |                  | in critical sections. Additionally, critical sections     | */
/* |                  | are to be left before completion of an interrupt service  | */
/* |                  | routine.                                                  | */
/* |                  | 5.Generally speaking, critical sections should be short.  | */
/* |                  | 6.The service may be called from an ISR and from task     | */
/* |                  | level (see Figure 12-1).                                  | */
/* |------------------+-----------------------------------------------------------| */
/* | Status:          | Standard:1.No error, E_OK                                 | */
/* |                  | Extended:1.Resource <ResID> is invalid, E_OS_ID           | */
/* |                  | 2.Attempt to get a resource which is already occupied     | */
/* |                  | by any task or ISR, or the statically assigned priority   | */
/* |                  | of the calling task or interrupt routine is higher than   | */
/* |                  | the calculated ceiling priority, E_OS_ACCESS              | */
/* |------------------+-----------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                    | */
/* |------------------+-----------------------------------------------------------| */
StatusType GetResource (ResourceType xResID)
{
    StatusType   xRet = E_OK;
    PriorityType xOldPriority;
    PriorityType xNewPriority;

#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    if(xResID > (cfgOS_RESOURCE_NUM-1)){
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    if( tableResIsInUse(xResID)){
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }
#endif

    xNewPriority = tableGetResCeilPrio(xResID);

#if(cfgOS_ISR_USE_RES == STD_TRUE)
    if(OSGetContextLevel() == TASK_LEVEL)      /* TASK_LEVEL */
    {
#endif
        xOldPriority = OSCurTcb->xPriority;
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
        if(xOldPriority > xNewPriority)
        {
            xRet = E_OS_ACCESS;
            goto Error_Exit;
        }
#endif
        OS_ENTER_CRITICAL();
#if(cfgOS_ISR_USE_RES == STD_TRUE)
        if(xNewPriority > cfgOS_MAX_PRIORITY)
        {
            OSSetIpl(xNewPriority - cfgOS_MAX_PRIORITY);
        }
        else
        {
#endif
            /* User Must Make Sure No Tasks Was Configured in xNewPriority,
             * That MEANS no Task could run in xNewPriority, Except these Tasks 
             * Witch will use the Resource<xResID>*/
            OS_ASSERT(listTskRdyIsEmpty(xNewPriority));
            doChangeTskPrioFromTo(xOldPriority,xNewPriority);
#if(cfgOS_ISR_USE_RES == STD_TRUE)
        }
#endif
        tableSetResSavedPrio(xResID, xOldPriority);
        tableSetResPrevResID(xResID, OSCurTcb->xCurResID);
        OSCurTcb->xCurResID = xResID;
        if(xNewPriority > OSHighRdyPrio)
        {
            OSHighRdyPrio = xNewPriority;
            OSHighRdyTsk  = OSCurTsk;
            OSHighRdyTcb  = OSCurTcb;
        }
        OS_EXIT_CRITICAL();
#if(cfgOS_ISR_USE_RES == STD_TRUE)
    }
    else                        /* ISR2_LEVEL */
    {
        xOldPriority = cfgOS_MAX_PRIORITY + OSGetIpl();
#   if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
        if(xOldPriority > xNewPriority)
        {
            xRet = E_OS_ACCESS;
            goto Error_Exit;
        }
#   endif
        OS_ENTER_CRITICAL();
        OSSetIpl(xNewPriority - cfgOS_MAX_PRIORITY);
        tableSetResSavedPrio(xResID, xOldPriority);
        OS_EXIT_CRITICAL();
    }
#endif
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
  Error_Exit:
#endif
#if(cfgOS_ERROR_HOOK == 1)
    if(xRet != E_OK)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_GetResource);
        OSErrorSetParam1(xResID);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}

/* |------------------+------------------------------------------------------------| */
/* | Syntax:          | StatusType ReleaseResource ( ResourceType <ResID> )        | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (In):  | ResID:Reference to resource                                | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (Out): | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Description:     | ReleaseResource is the counterpart of GetResource and      | */
/* |                  | serves to leave critical sections in the code that are     | */
/* |                  | assigned to the resource referenced by <ResID>.            | */
/* |------------------+------------------------------------------------------------| */
/* | Particularities: | For information on nesting conditions, see particularities | */
/* |                  | of GetResource.                                            | */
/* |                  | The service may be called from an ISR and from task level  | */
/* |                  | (see Figure 12-1).                                         | */
/* |------------------+------------------------------------------------------------| */
/* | Status:          | Standard: No error, E_OK                                   | */
/* |                  | Extended: Resource <ResID> is invalid, E_OS_ID             | */
/* |                  | Attempt to release a resource which is not occupied by     | */
/* |                  | any task or ISR, or another resource shall be released     | */
/* |                  | before, E_OS_NOFUNC                                        | */
/* |                  | Attempt to release a resource which has a lower ceiling    | */
/* |                  | priority than the statically assigned priority of the      | */
/* |                  | calling task or interrupt routine, E_OS_ACCESS             | */
/* |------------------+------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                     | */
/* |------------------+------------------------------------------------------------| */
StatusType ReleaseResource ( ResourceType xResID )
{
    PriorityType xOldPriority;
    PriorityType xNewPriority;
    StatusType   xRet=E_OK;
    xOldPriority = tableGetResCeilPrio(xResID);
    xNewPriority = tableGetResSavedPrio(xResID);
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    if(xResID > (cfgOS_RESOURCE_NUM-1)){
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    if( tableResIsInUse(xResID)==STD_FALSE ){
        xRet = E_OS_NOFUNC;
        goto Error_Exit;
    }
    if(xNewPriority > xOldPriority) {
        xRet = E_OS_ACCESS;
        goto Error_Exit;
    }
#endif

#if(cfgOS_ISR_USE_RES == STD_TRUE)
    if(OSGetContextLevel() == TASK_LEVEL)      /* TASK_LEVEL */
    {
#endif       
        OS_ENTER_CRITICAL();
        if(xOldPriority > cfgOS_MAX_PRIORITY)
        {
            OSSetIpl(0);
        }
        else
        {
            doChangeTskPrioFromTo(xOldPriority , xNewPriority);
            OS_ASSERT(listTskRdyIsEmpty(xOldPriority));
        }
        OSCurTcb->xCurResID = tableGetResPrevResID(xResID);    
        tableSetResSavedPrio(xResID,INVALID_PRIORITY); /* Release Resource */
        if(xOldPriority == OSHighRdyPrio)
        {
            OSHighRdyPrio = tableRdyMapFindHighPriority();
            OSHighRdyTsk  = listGetRdyTsk(OSHighRdyPrio);
            OSHighRdyTcb  = &OSTcbTable[OSHighRdyTsk];
        }
        OS_EXIT_CRITICAL();
#if(cfgOS_ISR_USE_RES == STD_TRUE)
    }
    else                        /* Called ISR2 LEVEL */
    {
        OS_ENTER_CRITICAL();
        OSSetIpl(xNewPriority - cfgOS_MAX_PRIORITY);
        tableSetResSavedPrio(xResID, INVALID_PRIORITY);
        OS_EXIT_CRITICAL();
    }
#endif
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
  Error_Exit:
#endif
#if(cfgOS_ERROR_HOOK == 1)
    if(xRet != E_OK)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_GetResource);
        OSErrorSetParam1(xResID);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}

#endif  /* cfgOS_USE_RESOURCE */

#if (cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
/* For Internal Resource */
/* Hint:Internal Resource is really Special,So cann't treate it as normal resource Which 
   can be got or released by task with the calling of GetResource() or ReleaseResource().
   Only some tasks may have one and only one internal resource,So When Configure,These Task's 
   xTaskID should be sequencial from 0 to cfgOS_TASK_WITH_IN_RES, cfgOS_TASK_WITH_IN_RES_NUM
   is a macro define that indicates how many tasks use internal resource.
   So The implementation will be easier.
*/
void GetInResource(void)
{
    ResourceType xResID;
    PriorityType xOldPriority;
    PriorityType xNewPriority;
    xResID = OSTskInResIdTable[OSCurTsk];
    /* When A Task with an internal translates into running state
       The Internal Resource must has been releasd*/
    OS_ASSERT(OSInResSavedPrioTable[xResID] == INVALID_PRIORITY);

    xOldPriority = OSCurTcb->xPriority;
    xNewPriority = OSInResCeilPrioTable[xResID];

    OS_ASSERT(xNewPriority > xOldPriority);
    OS_ASSERT(listTskRdyIsEmpty(xNewPriority));

    OS_ENTER_CRITICAL();
    OSInResSavedPrioTable[xResID] = OSCurTcb->xPriority;
    doChangeTskPrioFromTo(xOldPriority,xNewPriority);

    if(xNewPriority > OSHighRdyPrio)
    {
        OSHighRdyPrio = xNewPriority;
        OSHighRdyTsk  = OSCurTsk;
        OSHighRdyTcb  = OSCurTcb;
    }
    OS_EXIT_CRITICAL();
}
void ReleaseInResource(void)
{
    ResourceType xResID;
    PriorityType xOldPriority;
    PriorityType xNewPriority;

    xResID = OSTskInResIdTable[OSCurTsk];
 
    xOldPriority = OSCurTcb->xPriority;
    xNewPriority = OSInResSavedPrioTable[xResID];
    
    OS_ASSERT(xNewPriority < xOldPriority);

    OS_ENTER_CRITICAL();
    doChangeTskPrioFromTo(xOldPriority,xNewPriority);
    OS_ASSERT(listTskRdyIsEmpty(xOldPriority));
    OSInResSavedPrioTable[xResID]=INVALID_PRIORITY; /* Release */
    if(xOldPriority == OSHighRdyPrio)
    {
        OSHighRdyPrio = tableRdyMapFindHighPriority();
        OSHighRdyTsk  = listGetRdyTsk(OSHighRdyPrio);
        OSHighRdyTcb  = &OSTcbTable[OSHighRdyTsk];
    }
    OS_EXIT_CRITICAL();
}
#endif  /* End  of Internal Resource */

