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
/* | File-name:        | Alarm.c                                      | */
/* |-------------------+----------------------------------------------| */
/* | Reference Manual: | (1)Specification of Operating System(V3.0.2) | */
/* |                   | AUTOSAR_SWS_OS.PDF                           | */
/* |                   |                                              | */
/* |                   | (2)Operating SystemSpecification 2.2.3(OSEK) | */
/* |                   | os223.doc                                    | */
/* |-------------------+----------------------------------------------| */
#include "Kernel.h"
#if (cfgOS_ALARM_NUM >0)
/* listCntInsert():
 * Will Insert xAlarmID to the place according to xAlarmValue,
 * The situation That an overflow of xAlarmValue should be considered.
 * An overflowed xAlarmValue is bigger than no overflowed one.
 * So When the Insert was done, the listCnt was Sorted by xAlarmValue
 * from smaller to bigger.*/
static void listCntInsert(CounterType xCounterID, AlarmType xAlarmID )
{
    AlarmType pxNext;
    AlarmType pxPrev;
    TickType xAlarmValue  = 0u;
    TickType xCntCurValue = 0u;

    xAlarmValue  = tableGetAlmValue(xAlarmID);
    xCntCurValue = tableGetCntCurValue(xCounterID);

    pxPrev =  INVALID_ALARM; 
    pxNext =  listGetCntHeadElement(xCounterID);
        
    if( xAlarmValue < xCntCurValue )
    {   /* It's an overflowed xAlarmValue,So Skip all the no overflowed one*/
        while( (pxNext != INVALID_ALARM) &&
               (tableGetAlmValue(pxNext) > xCntCurValue ))
        {
            pxPrev = pxNext;
            pxNext = listGetCntNextElement(pxNext);
        }

    }
    while( (pxNext != INVALID_ALARM ) &&
           (tableGetAlmValue(pxNext) < xAlarmValue))
    {   /*  Find untill pxNext's xAlarmValue is bigger that xAlarmID's xAlarmValue,
         *  Tast is just the place to insert xAlarmID*/
        pxPrev = pxNext;
        pxNext = listGetCntNextElement(pxNext);
    }     
    if(pxPrev == INVALID_ALARM)
    {                           /* Should insert it At Head */
        listCntInsertAtHead(xCounterID , xAlarmID); 
    }
    else if(pxNext == INVALID_ALARM )
    {                           /* Should insert it At Tail */
        listCntInsertAtTail(pxPrev , xAlarmID);
    }
    else
    {                           /* Should insert it between pxPrev,pxNext */
        listCntInsertBetween(pxPrev,pxNext,xAlarmID);
    }
}
static void listCntRemove(CounterType xCounterID ,AlarmType xAlarmID)
{
    AlarmType pxNext;
    AlarmType pxPrev;
    pxPrev=listGetCntPrevElement(xAlarmID);
    pxNext=listGetCntNextElement(xAlarmID);
    if(pxPrev == INVALID_ALARM)
    {                           /* Should remove it At Head */
        listCntRemoveAtHead(xCounterID); 
    }
    else if(pxNext == INVALID_ALARM )
    {                           /* Should remove it At Tail */
        listCntRemoveAtTail(pxPrev);
    }
    else
    {                           /* Should remove it between pxPrev,pxNext */
        listCntRemoveBetween(pxPrev,pxNext);
    } 
}
/* |------------------+------------------------------------------------------------------| */
/* | Syntax:          | StatusType GetAlarmBase (AlarmType <AlarmID>,                    | */
/* |                  | AlarmBaseRefType <Info> )                                        | */
/* |------------------+------------------------------------------------------------------| */
/* | Parameter (In):  | AlarmID: Reference to alarm                                      | */
/* |------------------+------------------------------------------------------------------| */
/* | Parameter (Out): | Info: Reference to structure with constants of the alarm base.   | */
/* |------------------+------------------------------------------------------------------| */
/* | Description:     | The system service GetAlarmBase reads the alarm base             | */
/* |                  | characteristics. The return value <Info> is a structure in which | */
/* |                  | the information of data type AlarmBaseType is stored.            | */
/* |------------------+------------------------------------------------------------------| */
/* | Particularities: | Allowed on task level, ISR, and in several hook routines (see    | */
/* |                  | Figure 12-1).                                                    | */
/* |------------------+------------------------------------------------------------------| */
/* | Status:          | Standard:No error, E_OK                                          | */
/* |                  | Extended:Alarm <AlarmID> is invalid, E_OS_ID                     | */
/* |------------------+------------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                           | */
/* |------------------+------------------------------------------------------------------| */
StatusType GetAlarmBase ( AlarmType xAlarmID, AlarmBaseRefType pxInfo )
{
    StatusType xRet=E_OK;
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if ( xAlarmID > (cfgOS_ALARM_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    *pxInfo = listGetAlarmBase(xAlarmID);
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)    
  Error_Exit:
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_GetAlarmBase);
        OSErrorSetParam1(xAlarmID);
        OSErrorSetParam2(pxInfo);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
#endif  /* cfgOS_STATUS_LEVEL */
    return xRet;
}
/* |------------------+------------------------------------------------------------------| */
/* | Syntax:          | StatusType GetAlarm ( AlarmType <AlarmID>,TickRefType <Tick>)    | */
/* |------------------+------------------------------------------------------------------| */
/* | Parameter (In):  | AlarmID:Reference to an alarm                                    | */
/* |------------------+------------------------------------------------------------------| */
/* | Parameter (Out): | Tick:Relative value in ticks before the alarm <AlarmID> expires. | */
/* |------------------+------------------------------------------------------------------| */
/* | Description:     | The system service GetAlarm returns the relative value in ticks  | */
/* |                  | before the alarm <AlarmID> expires.                              | */
/* |------------------+------------------------------------------------------------------| */
/* | Particularities: | 1.It is up to the application to decide whether for example a    | */
/* |                  | CancelAlarm may still be useful.                                 | */
/* |                  | 2.If <AlarmID> is not in use, <Tick> is not defined.             | */
/* |                  | 3.Allowed on task level, ISR, and in several hook routines (see  | */
/* |                  | Figure 12-1).                                                    | */
/* |------------------+------------------------------------------------------------------| */
/* | Status:          | Standard: No error, E_OK                                         | */
/* |                  | Alarm <AlarmID> is not used, E_OS_NOFUNC                         | */
/* |                  | Extended:  Alarm <AlarmID> is invalid, E_OS_ID                   | */
/* |------------------+------------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                           | */
/* |------------------+------------------------------------------------------------------| */
StatusType GetAlarm ( AlarmType xAlarmID ,TickRefType pxTick )
{
    StatusType xRet=E_OK;
    TickType xAlarmValue = 0u;
    TickType xCntCurValue = 0u;
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if ( xAlarmID > (cfgOS_ALARM_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    if (STD_FALSE == IsAlmInUse(xAlarmID))
    {
        xRet = E_OS_NOFUNC;
        goto Error_Exit;
    }
    xAlarmValue = tableGetAlmValue(xAlarmID);
    /* Now xAlarmID is no use, So use it To Store xCounterID, The Owner of xAlarmID */
    xAlarmID = tableGetCounterID(xAlarmID);
    xCntCurValue =tableGetCntCurValue(xAlarmID);
    if(xAlarmValue > xCntCurValue)
    {
        *pxTick = xAlarmValue - xCntCurValue;
    }
    else
    {
        *pxTick = tableGetCntMaxAllowed(xAlarmID) + xAlarmValue -xCntCurValue;
    }
  Error_Exit:
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_GetAlarm);
        OSErrorSetParam1(xAlarmID);
        OSErrorSetParam2(pxTick);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}
/* |------------------+-----------------------------------------------------------------| */
/* | Syntax:          | StatusType SetRelAlarm ( AlarmType <AlarmID>,                   | */
/* |                  | TickType <increment>,                                           | */
/* |                  | TickType <cycle> )                                              | */
/* |------------------+-----------------------------------------------------------------| */
/* | Parameter (In):  | AlarmID:Reference to the alarm element                          | */
/* |                  | increment:Relative value in ticks                               | */
/* |                  | cycle:Cycle value in case of cyclic alarm. In case of single    | */
/* |                  | alarms, cycle shall be zero.                                    | */
/* |------------------+-----------------------------------------------------------------| */
/* | Parameter (Out): | none                                                            | */
/* |------------------+-----------------------------------------------------------------| */
/* | Description:     | The system service occupies the alarm <AlarmID> element.        | */
/* |                  | After <increment> ticks have elapsed, the task assigned         | */
/* |                  | to the alarm <AlarmID> is activated or the assigned event       | */
/* |                  | (only for extended tasks) is set or the alarm-callback          | */
/* |                  | routine is called.                                              | */
/* |------------------+-----------------------------------------------------------------| */
/* | Particularities: | 1.The behaviour of <increment> equal to 0 is up to the          | */
/* |                  | implementation.                                                 | */
/* |                  | 2.If the relative value <increment> is very small, the alarm    | */
/* |                  | may expire, and the task may become ready or the alarm-callback | */
/* |                  | may be called before the system service returns to the user.    | */
/* |                  | 3.If <cycle> is unequal zero, the alarm element is logged on    | */
/* |                  | again immediately after expiry with the relative value <cycle>. | */
/* |                  | 4.The alarm <AlarmID> must not already be in use.               | */
/* |                  | 5.To change values of alarms already in use the alarm shall be  | */
/* |                  | cancelled first.                                                | */
/* |                  | 6.If the alarm is already in use, this call will be ignored and | */
/* |                  | the error E_OS_STATE is returned.                               | */
/* |                  | 7.Allowed on task level and in ISR, but not in hook routines.   | */
/* |------------------+-----------------------------------------------------------------| */
/* | Status:          | Standard:                                                       | */
/* |                  | 1.No error, E_OK                                                | */
/* |                  | 2.Alarm <AlarmID> is already in use, E_OS_STATE                 | */
/* |                  | Extended:                                                       | */
/* |                  | 1.Alarm <AlarmID> is invalid, E_OS_ID                           | */
/* |                  | 2.Value of <increment> outside of the admissible limits         | */
/* |                  | (lower than zero or greater than maxallowedvalue), E_OS_VALUE   | */
/* |                  | 3.Value of <cycle> unequal to 0 and outside of the admissible   | */
/* |                  | counter limits (less than mincycle or greater than              | */
/* |                  | maxallowedvalue), E_OS_VALUE                                    | */
/* |------------------+-----------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2; Events only ECC1, ECC2                  | */
/* |------------------+-----------------------------------------------------------------| */
StatusType SetRelAlarm ( AlarmType xAlarmID , TickType xIncrement ,TickType xCycle )
{
    StatusType xRet = E_OK;
    CounterType xCounterID;
    TickType xMaxAllowedValue;

#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if ( xAlarmID > (cfgOS_ALARM_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
/* OS304: If in a call to SetRelAlarm() the parameter ¡°increment¡± is set to zero, 
   the service shall return E_OS_VALUE in standard and extended status . */
    if(xIncrement == 0)
    {
        xRet = E_OS_VALUE;
        goto Error_Exit;
    }
    if (IsAlmInUse(xAlarmID))
    {
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
    xCounterID = tableGetCounterID(xAlarmID);  
    xMaxAllowedValue=tableGetCntMaxAllowed(xCounterID);
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if( (xIncrement > xMaxAllowedValue)
        ||(xCycle > xMaxAllowedValue)
        ||((xCycle != 0) &&(xCycle < tableGetCntMinCycle(xCounterID)))
        )
    {
        xRet = E_OS_VALUE;
        goto Error_Exit;
    }
#endif
    DoUseAlm(xAlarmID) ; /* No check Error, use it */
    tableSetAlmCycle(xAlarmID,xCycle); /* Set Its xCycle */
    /* As xCycle has already been stored, So use it as temp */
    xCycle=vDoAddCounterTick(tableGetCntCurValue(xCounterID),xMaxAllowedValue,xIncrement);
    tableSetAlmValue(xAlarmID,xCycle); /* Set Its xAlarmValue */
    OS_ENTER_CRITICAL();
    /* Insert xAlarmID to proper place accordint to xAlarmValue */
    listCntInsert(xCounterID,xAlarmID); 
    OS_EXIT_CRITICAL();
  Error_Exit:
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_SetRelAlarm);
        OSErrorSetParam1(xAlarmID);
        OSErrorSetParam2(xIncrement);
        OSErrorSetParam3(xCycle);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}
/* |------------------+-----------------------------------------------------------------| */
/* | Syntax:          | StatusType SetAbsAlarm (AlarmType <AlarmID>,                    | */
/* |                  | TickType <start>,                                               | */
/* |                  | TickType <cycle> )                                              | */
/* |------------------+-----------------------------------------------------------------| */
/* | Parameter (In):  | AlarmID:Reference to the alarm element                          | */
/* |                  | start:Absolute value in ticks                                   | */
/* |                  | cycle:Cycle value in case of cyclic alarm. In case of           | */
/* |                  | single alarms, cycle shall be zero.                             | */
/* |------------------+-----------------------------------------------------------------| */
/* | Parameter (Out): | none                                                            | */
/* |------------------+-----------------------------------------------------------------| */
/* | Description:     | The system service occupies the alarm <AlarmID> element.        | */
/* |                  | When <start> ticks are reached, the task assigned to the alarm  | */
/* |                  | <AlarmID> is activated or the assigned event (only for extended | */
/* |                  | tasks) is set or the alarm-callback routine is called.          | */
/* |------------------+-----------------------------------------------------------------| */
/* | Particularities: | 1.If the absolute value <start> is very close to the current    | */
/* |                  | counter value, the alarm may expire, and the task may become    | */
/* |                  | ready or the alarm-callback may be called before the system     | */
/* |                  | service returns to the user.                                    | */
/* |                  | 2.If the absolute value <start> already was reached before      | */
/* |                  | the system call, the alarm shall only expire when the           | */
/* |                  | absolute value <start> is reached again, i.e. after the next    | */
/* |                  | overrun of the counter.                                         | */
/* |                  | 3.If <cycle> is unequal zero, the alarm element is logged on    | */
/* |                  | again immediately after expiry with the relative value <cycle>. | */
/* |                  | 4.The alarm <AlarmID> shall not already be in use.              | */
/* |                  | 5.To change values of alarms already in use the alarm shall be  | */
/* |                  | cancelled first.                                                | */
/* |                  | 6.If the alarm is already in use, this call will be ignored and | */
/* |                  | the error E_OS_STATE is returned.                               | */
/* |                  | 7.Allowed on task level and in ISR, but not in hook routines.   | */
/* |------------------+-----------------------------------------------------------------| */
/* | Status:          | Standard:                                                       | */
/* |                  | 1.No error, E_OK                                                | */
/* |                  | 2.Alarm <AlarmID> is already in use, E_OS_STATE                 | */
/* |                  | Extended:                                                       | */
/* |                  | 1.Alarm <AlarmID> is invalid, E_OS_ID                           | */
/* |                  | 2.Value of <start> outside of the admissible counter limit      | */
/* |                  | (less than zero or greater than maxallowedvalue), E_OS_VALUE    | */
/* |                  | 3.Value of <cycle> unequal to 0 and outside of the admissible   | */
/* |                  | counter limits (less than mincycle or greater than              | */
/* |                  | maxallowedvalue), E_OS_VALUE                                    | */
/* |------------------+-----------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2; Events only ECC1, ECC2                  | */
/* |------------------+-----------------------------------------------------------------| */
StatusType SetAbsAlarm ( AlarmType xAlarmID , TickType xStart ,TickType xCycle )
{
    StatusType xRet = E_OK;
    CounterType xCounterID=INVALID_COUNTER;

#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
		TickType xMaxAllowedValue =0u;
    if ( xAlarmID > (cfgOS_ALARM_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif

    if (IsAlmInUse(xAlarmID))
    {
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
    xCounterID = tableGetCounterID(xAlarmID);  
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
		xMaxAllowedValue=tableGetCntMaxAllowed(xCounterID);
    if( (xStart > xMaxAllowedValue)
        ||(xCycle > xMaxAllowedValue)
        ||((xCycle != 0) &&(xCycle < tableGetCntMinCycle(xCounterID)))
        )
    {
        xRet = E_OS_VALUE;
        goto Error_Exit;
    }
#endif
    DoUseAlm(xAlarmID) ; /* No check Error, use it */
    tableSetAlmCycle(xAlarmID,xCycle); /* Set Its xCycle */
    tableSetAlmValue(xAlarmID,xStart); /* Set Its xAlarmValue */
    OS_ENTER_CRITICAL();
    /* Insert xAlarmID to proper place accordint to xAlarmValue */
    listCntInsert(xCounterID,xAlarmID); 
    OS_EXIT_CRITICAL();
  Error_Exit:
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_SetAbsAlarm);
        OSErrorSetParam1(xAlarmID);
        OSErrorSetParam2(xStart);
        OSErrorSetParam3(xCycle);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;

}
/* |------------------+-------------------------------------------------------------| */
/* | Syntax:          | StatusType CancelAlarm ( AlarmType <AlarmID> )              | */
/* |------------------+-------------------------------------------------------------| */
/* | Parameter (In):  | AlarmID:Reference to an alarm                               | */
/* |------------------+-------------------------------------------------------------| */
/* | Parameter (Out): | none                                                        | */
/* |------------------+-------------------------------------------------------------| */
/* | Description:     | The system service cancels the alarm <AlarmID>.             | */
/* |------------------+-------------------------------------------------------------| */
/* | Particularities: | Allowed on task level and in ISR, but not in hook routines. | */
/* |------------------+-------------------------------------------------------------| */
/* | Status:          | Standard:                                                   | */
/* |                  | 1.No error, E_OK                                            | */
/* |                  | 2.Alarm <AlarmID> not in use, E_OS_NOFUNC                   | */
/* |                  | Extended: 1.Alarm <AlarmID> is invalid, E_OS_ID             | */
/* |------------------+-------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                      | */
/* |------------------+-------------------------------------------------------------| */
StatusType CancelAlarm ( AlarmType xAlarmID )
{
    StatusType xRet = E_OK;
#if (cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND )
    if ( xAlarmID > (cfgOS_ALARM_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    if (!IsAlmInUse(xAlarmID))  /* if alarm is not in use */
    {
        xRet = E_OS_NOFUNC;
        goto Error_Exit;
    }
    OS_ENTER_CRITICAL();
    listCntRemove(tableGetCounterID(xAlarmID), xAlarmID);
    DoCancleAlm(xAlarmID);
    OS_EXIT_CRITICAL();
  Error_Exit:
#if(cfgOS_ERROR_HOOK == 1)
    if(E_OK != xRet)
    {
        OS_ENTER_CRITICAL();
        OSErrorSetServiceId(OSServiceId_CancelAlarm);
        OSErrorSetParam1(xAlarmID);
        ErrorHook(xRet);
        OS_EXIT_CRITICAL();
    }
#endif
    return xRet;
}

void OSProcessAlarm( CounterType xCounterID )
{
    TickType xCntCurValue;
    AlarmType xAlarmID;
    AlarmType xNextAlarm;
    xCntCurValue = tableGetCntCurValue(xCounterID);
    OS_ENTER_CRITICAL();
    xAlarmID = listGetCntHeadElement(xCounterID);
    while(xAlarmID!=INVALID_ALARM)
    {
        if(tableGetAlmValue(xAlarmID) == xCntCurValue)
        {
            switch(tableGetAlmClsType(xAlarmID))
            {
                case ALARM_CALLBACK :
                    ((TaskEntryType)(OSAlarmContainerTable[xAlarmID]))();
                    break;
                case ALARM_TASK :
                    (void)ActivateTask((TaskType)(OSAlarmContainerTable[xAlarmID]));
                    break;
#if(cfgOS_CONFORMANCE_CLASS == ECC1 || cfgOS_CONFORMANCE_CLASS == ECC2)
                case ALARM_EVENT :
                    (void)SetEvent((TaskType)(((uint32_t)OSAlarmContainerTable[xAlarmID])>>16),
                              (EventMaskType)(((uint32_t)OSAlarmContainerTable[xAlarmID])&0x00FF));
                    break;
#endif
                default:
                    break;
            }
            /* Firstly,Find out next one */
            xNextAlarm=listGetCntNextElement(xAlarmID);
            /* then,Alarm Post Process */
            listCntRemove(xCounterID,xAlarmID);
            if(tableGetAlmCycle(xAlarmID) != 0){
                tableGetAlmValue(xAlarmID)=vDoAddCounterTick(xCntCurValue,
                                                        tableGetCntMaxAllowed(xCounterID),
                                                        tableGetAlmCycle(xAlarmID));
                listCntInsert(xCounterID ,xAlarmID);
            }
            else{
                DoCancleAlm(xAlarmID);
            }
            /* Store */
            xAlarmID=xNextAlarm;
        }
        else
        {
            /* Exit This Function */
            break;
        }
    }
    OS_EXIT_CRITICAL();
}

#endif  /* cfgOS_ALARM_NUM*/




 
 


