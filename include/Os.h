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

#ifndef _OS_H_
#define _OS_H_


/* |-----------------------------| */
/* | 5.1.2 Header file structure | */
/* |-----------------------------| */
#include "Std_Types.h"
#include "MemMap.h"
#include "Os_Cfg.h"
#include "OsCfgChk.h"
#include "vPort.h"

#include "Interrupt.h"

#ifndef OS_ASSERT
#define OS_ASSERT(condition)                    \
    do{                                         \
        if(STD_FALSE == (condition))            \
        {                                       \
            for(;;);                            \
        }                                       \
    }while(0)
#endif
#define INVALID_PRIORITY (cfgOS_MAX_PRIORITY+1)
#define INVALID_TASK     (cfgOS_TASK_NUM)
#define INVALID_COUNTER  UINT8_INVALID
#define INVALID_ALARM    UINT8_INVALID
#define INVALID_RESOURCE UINT8_INVALID
#define NO_EVENT         (0x0000u)



#ifdef OS_GLOBALS
#define OS_EXT 
#else
#define OS_EXT extern
#endif

/* ====================================================================================== */
/*                            Self Define Types for implementation                        */
/* ====================================================================================== */
/* TaskStackType For stack manipulation */
typedef uint32_t TaskStackType;
/* A pointer to TaskStackType */
typedef struct{
    TaskStackType * pxStackTop;
#if(cfgOS_STACK_USAGE_CHECK == 1)
    TaskStackType * pxStackBottom;
#endif
} TaskStackRefType;
/* Task Priority Type */
typedef uint8_t PriorityType;
/* Task Entry Point Type */
typedef void (*TaskEntryType)(void);
typedef struct _TaskControlBlockTag
{
    TaskStackType*   pxStack;   /* Every time on activate, reload from OSTaskStack*/
	PriorityType     xPriority;	/* Shouldn't be bigger than 63 */
	TaskStateType    xState;    
#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
	uint8_t          xActivateCount;
#endif
#if (cfgOS_USE_RESOURCE == STD_TRUE)
    ResourceType    xCurResID;     /*  The ResID Occupied recently*/
#endif
}TcbType;

#define    ALARM_CALLBACK 0u           /* this alarm-callback routine is called */
#define    ALARM_TASK     1u           /* this task is activated */
#define    ALARM_EVENT    2u           /* an event is set for this task */
typedef uint8_t AlarmClassType;
/* An container that can contain TaskID ,call-back's entry or EventID */
/* For 16 bit Cpu , the Banked policy may be used,
 * So May be type(void *) cann't contain an function entry address*/
typedef  VoidType AlarmContainerType;
/* Alarm Control Block */
typedef struct _AlmCtrlBlkTag
{
    TickType        xAlarmValue; /* The Time It will expire */
    TickType        xCycle;
    AlarmType       pxNext;
    AlarmType       pxPrev;
}AlmCtrlBlkType;

typedef struct{
    PriorityType xSavedPriority; /* Save the xTaskID's Previous xPriority */
    ResourceType xPrevResID;     /* The Previous ResID occupied by xTask or ISR2 */
}ResCtrlBlkType;

#define SCHEDTBL_ADVANCED  0u
#define SCHEDTBL_RETARD    1u
/* control block for schedule table */
typedef struct{
    ScheduleTableStatusType xSchedTblStatus;
    /* used to interate all of the expiry point,save the info of locatoion */
    uint8_t xIterator;
    /* For effective process,record the the absolute time of the next expiry point */
    TickType xSchedTblNextExpiryPointTime;
    /* store the the time when the schedule table should be started to be processed */
    TickType xSchedTblStartingTime;
    /* For explicitly synchronous schedule tables,record the direction shoud 
       do adjust:SCHEDTBL_ADVANCED SCHEDTBL_RETARD */
    uint8_t xAdjustDirection;
    /* For explicitly synchronous schedule tables,record the deviation */
    TickType xDeviation;
    /* -If xNextOrPrevScheduleTable is valid,and the owner of this container 
       is in state running,then this variant indicates the next schedule table 
       should be started after the final delay of the owner.
       -If xNextOrPrevScheduleTable is valid,and the owner of this container
       is in state next,then this variant indicates the previous schedule table 
       whitch will ends after its final delay the the owner starts.*/
    ScheduleTableType xNextOrPrevScheduleTable;
    /* to organise all of the schedule tables assigned to one counter */
    ScheduleTableType pxNextSchedTbl;
    ScheduleTableType pxPrevSchedTbl;
}OsSchedTblCtrlBlkType;
typedef struct
{
    TickType xOsScheduleTableDuration;
    uint8_t  xOsScheduleTableRepeating; /* Bool Type */
    ApplicationType xOsScheduleTableAccessingApplication;
    CounterType xOsScheduleTableCounterRef;
    uint8_t xOsScheduleTableAutostartable;
    OsScheduleTableAutostartType xOsScheduleTableAutostartType;
    TickType xOsScheduleTableAutostartValue;  
    OsScheduleTblSyncStrategyType xOsScheduleTableSyncStrategy;  
    TickType xOsScheduleTableMaxAdvance;
    TickType xOsScheduleTableMaxRetard;
    TickType xOsScheduleTableExplicitPrecision; 
}OsScheduleTableType;

/* How to organise Schedule Table,As every Expiry Potint do different things,
   So no better data struct than an data cmd array*/
typedef struct
{
    TickType xOffset;
    void (*pxOsSchedTblCmd)(void);
} OsSchedTblCmdType;

/* ========================================================================================= */
/*                                    Global Variant Declaration                             */
/* ========================================================================================= */
OS_EXT AppModeType OSAppMode;
OS_EXT BoolType    OSRunning;
/* Task Id to identify the Current running task,alse OSCurTcb */
OS_EXT TaskType OSCurTsk;
OS_EXT TcbType* OSCurTcb;  
/* Task Id ti identify the Next Task can be started when OSCurTsk terminates if
 * OSNextTsk is not Invalid */
OS_EXT TaskType OSHighRdyTsk;
OS_EXT TcbType* OSHighRdyTcb;
/* Task Priority to identify The next Priority which has task or tasks in ready state
 * And OSNextTsk is the biggest one but OSCurTsk's priority */
OS_EXT PriorityType OSHighRdyPrio;
/* |------------------------------------------------------------------------| */
/* | I learned it from ucOS_II:                                             | */
/* | That build an hash table thet with the using of the variant OSReadyGrp | */
/* | and OSReadyTable can quickly find out highest priority task or task    | */
/* | group with the same priority, which is in the state of "READY",The     | */
/* | variant OSReadyTable and OSReadyGrp was defined in "Os.h".             | */
/* | And Only (0~ 63) is the scope of priority.                             | */
/* |------------------------------------------------------------------------| */
OS_EXT uint8_t OSReadyGrp;
OS_EXT uint8_t OSReadyTable[(cfgOS_MAX_PRIORITY>>3) + 1];

/* |-----------------------------------------------------------| */
/* | OSTskRdyListHead/Tasl:                                    | */
/* |-----------------------------------------------------------| */
/* | IF cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY: | */
/* | that is to say that The Policy that several tasks which   | */
/* | with the same priority are allowed to co-exist.           | */
/* | So OSTcbRdyListHead/Tail[xPriority] is a TRUE list which  | */
/* | sort the tasks that whoes state are READY or RUNNING ,    | */
/* | and its value is always a Pointer to the list Head        | */
/* |-----------------------------------------------------------| */
/* | IF cfgOS_TASK_PER_PRIORITY == ONLY_ONE_TASK_PER_PRIORITY: | */
/* | that is to say that The Policy that only one task per     | */
/* | Priority can be started,So it's not allowed that          | */
/* | several tasks has the same priority                       | */
/* |-----------------------------------------------------------| */
/* The one more OSTskRdyListHead[] and OSTcbTable[] are 
   allocated for INVALID_TASK */
OS_EXT TaskType  OSTskRdyListHead[cfgOS_MAX_PRIORITY+2];

#if (cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY )
#    if(cfgOS_SUM_ACTIVATES == 0)
OS_EXT TaskType  OSTskRdyListTail[cfgOS_MAX_PRIORITY+1];
OS_EXT TaskType  OSTskRdyListNext[cfgOS_TASK_NUM+1];
#    elif(cfgOS_SUM_ACTIVATES > 0)
extern const TaskRefType OSTskRdyQueuePtr[cfgOS_MAX_PRIORITY+2];
extern const uint8_t     OSTskRdyQueueSize[cfgOS_MAX_PRIORITY+2];
/* Now OSTskRdyListHead used for Queue Management */
OS_EXT uint8_t     OSTskRdyListTail[cfgOS_MAX_PRIORITY+2];
#    endif
#endif

OS_EXT TcbType   OSTcbTable[cfgOS_TASK_NUM+1];

#if(cfgOS_COUNTER_NUM>0)
/* As one or Several Alarms can be asigned to one counter,
 * So there is a need of list or something else to Process
 * and manage all the alarms asigned to the counter,
 * all the alarms should be sorted by the next time when it 
 * expires. 
 * Counter Alarm List*/
OS_EXT AlarmType OSCntAlmList[cfgOS_COUNTER_NUM];
OS_EXT TickType  OSCntCurValueTable[cfgOS_COUNTER_NUM];
#endif


#if(cfgOS_USE_ALARM == STD_TRUE)
OS_EXT BoolType  OSAlmInUseTable[cfgOS_ALARM_NUM];
OS_EXT AlmCtrlBlkType   OSAlarmTable[cfgOS_ALARM_NUM];
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC1 || cfgOS_CONFORMANCE_CLASS ==ECC2)
OS_EXT EventMaskType OSWaitEventTable[cfgOS_TASK_NUM];
OS_EXT EventMaskType OSSetEventTable[cfgOS_TASK_NUM];
#endif

#if(cfgOS_SCHEDULE_TABLE_NUM>0)
OS_EXT OsSchedTblCtrlBlkType OSSchedTblCtrlBlkTable[cfgOS_SCHEDULE_TABLE_NUM];
OS_EXT ScheduleTableType OsCntSchedTblList[cfgOS_COUNTER_NUM];
#endif

#if (cfgOS_USE_RESOURCE == STD_TRUE)
OS_EXT ResCtrlBlkType OSResCtrlBlkTable[cfgOS_RESOURCE_NUM];
#endif
#if(cfgOS_USE_INTERNAL_RESOURCE == STD_TRUE)
OS_EXT PriorityType   OSInResSavedPrioTable[cfgOS_INTERNAL_RESOURCE_NUM];
#endif
OS_EXT uint8_t   OSIsr2Nesting;
#if(cfgOS_ERROR_HOOK == 1)
OS_EXT uint8_t   OSErrorServiceId;
OS_EXT VoidType  OSErrorParam1;
OS_EXT VoidType  OSErrorParam2;
OS_EXT VoidType  OSErrorParam3;
#endif

#define Dispatch()       vPortDispatch()  
#define OSStartHighRdy() vPortStartHighRdy()
#define EnterISR2()      vPortEnterISR()
#define LeaveISR2()      vPortLeaveISR()
#define EnterISR1()      OSEnterISR()
#define LeaveISR1()      OSExitISR()
#define OSSetIpl(xIpl)   vPortSetIpl((xIpl))
#define OSGetIpl()       vPortGetIpl()


/* ====================================================================================== */
/*                   Macro  Define (OS Service ID)                                        */
/* ====================================================================================== */
#define OSServiceId_ActivateTask    0
#define OSServiceId_TerminateTask   1
#define OSServiceId_ChainTask       2
#define OSServiceId_Schedule        3
#define OSServiceId_GetTaskID       4
#define OSServiceId_GetTaskState    5
#define OSServiceId_GetAlarmBase    6
#define OSServiceId_GetAlarm        7
#define OSServiceId_SetRelAlarm     8
#define OSServiceId_SetAbsAlarm     9
#define OSServiceId_CancelAlarm     10
#define OSServiceId_SetEvent        11 
#define OSServiceId_ClearEvent      12
#define OSServiceId_GetEvent        13
#define OSServiceId_WaitEvent       14
#define OSServiceId_GetResource     15
#define OSServiceId_ReleaseResource 16
#define OSServiceId_StartOS         17
#define OSServiceId_ShutdownOS      18
#if(cfgOS_ERROR_HOOK == 1)
#define OSErrorGetServiceId() ((OSServiceIdType)(OSErrorServiceId))
#define OSErrorSetServiceId(xSerId)                 \
    do{                                             \
        OSErrorServiceId=(OSServiceIdType)(xSerId); \
    }while(0)
#define OSErrorSetParam1(xParam1) OSErrorParam1 =(VoidType)(xParam1)
#define OSErrorSetParam2(xParam2) OSErrorParam2 =(VoidType)(xParam2)
#define OSErrorSetParam3(xParam3) OSErrorParam3 =(VoidType)(xParam3)

#define OSError_ActivateTask_TaskID()   ((TaskType) (OSErrorParam1))
#define OSError_ChainTask_TaskID()      ((TaskType) (OSErrorParam1))
#define OSError_GetTaskState_TaskID()   ((TaskType) (OSErrorParam1))
#define OSError_GetAlarmBase_AlarmID()  ((AlarmType)(OSErrorParam1))
#define OSError_GetAlarm_AlarmID()      ((AlarmType)(OSErrorParam1))
#define OSError_SetRelAlarm_AlarmID()   ((AlarmType)(OSErrorParam1))
#define OSError_SetRelAlarm_Increment() ((TickType) (OSErrorParam2))
#define OSError_SetRelAlarm_Cycle()     ((TickType) (OSErrorParam3))
#define OSError_SetAbsAlarm_AlarmID()   ((AlarmType)(OSErrorParam1))
#define OSError_SetAbsAlarm_Increment() ((TickType) (OSErrorParam2))
#define OSError_SetAbsAlarm_Cycle()     ((TickType) (OSErrorParam3))
#define OSError_CancelAlarm_AlarmID()   ((AlarmType)(OSErrorParam1))
#define OSError_SetEvent_TaskID()       ((TaskType) (OSErrorParam1))
#define OSError_ClearEvent_Mask()       ((EventMaskType)(OSErrorParam1))
#define OSError_GetEvent_TaskID()       ((TaskType) (OSErrorParam1))
#define OSError_WaitEvent_Mask()        ((EventMaskType)(OSErrorParam1))
#define OSError_GetResource_ResID()     ((ResourceType)(OSErrorParam1))
#define OSError_ReleaseResource_ResID() ((ResourceType)(OSErrorParam1))
#define OSError_StartOS_Mode()          ((AppModeType)(OSErrorParam1))
#define OSError_ShutdownOS_Error()      ((StatusType) (OSErrorParam1))

#endif  /* cfgOS_ERROR_HOOK */
/* ====================================================================================== */
/*                   Function  Declaration(OS API)                                        */
/* ====================================================================================== */
#define ISR(IsrName)  void IsrMain##IsrName(void)
#define IsrEntry(IsrName) &IsrMain##IsrName

#define DeclareTask(xTaskName)  TaskType xTaskName
#define DeclareAlarm(AlarmName) AlarmType AlarmName
#define TASK(TaskName) void TaskMain##TaskName(void)
#define TaskEntry(TaskName)   &TaskMain##TaskName

#define ALARMCALLBACK(AlarmCallbackRoutineName) \
    void AlarmCbk##AlarmCallbackRoutineName(void)
#define AlarmCallBackEntry(AlarmCallbackRoutineName) \
    &AlarmCbk##AlarmCallbackRoutineName

#define DeclareEvent( EventName ) EventType EventName 
#define DeclareResource( ResourceName ) ResourceType ResourceName

StatusType ActivateTask ( TaskType xTaskID );
StatusType TerminateTask( void );
StatusType ChainTask    ( TaskType xTaskID );
StatusType Schedule     ( void );
StatusType GetTaskID    ( TaskRefType pxTaskType );
StatusType GetTaskState ( TaskType xTaskID,TaskStateRefType pxState );

StatusType IncrementCounter(CounterType CounterID);
#if(cfgOS_USE_ALARM == STD_TRUE)
StatusType GetAlarmBase( AlarmType xAlarmID, AlarmBaseRefType pxInfo );
StatusType GetAlarm    ( AlarmType xAlarmID ,TickRefType pxTick );
StatusType SetRelAlarm ( AlarmType xAlarmID , TickType xIncrement ,TickType xCycle );
StatusType SetAbsAlarm ( AlarmType xAlarmID , TickType xStart ,TickType xCycle );
StatusType CancelAlarm ( AlarmType xAlarmID );
#endif  
#if(cfgOS_SCHEDULE_TABLE_NUM>0)
StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID,TickType Offset);
StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID,TickType Start);
StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID);
StatusType StopScheduleTable(ScheduleTableType ScheduleTableID);
StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID,TickType Value);
StatusType SetScheduleTableAsync(ScheduleTableType ScheduleTableID);
StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From,
                             ScheduleTableType ScheduleTableID_To);
#endif
#if (cfgOS_CONFORMANCE_CLASS == ECC1 || cfgOS_CONFORMANCE_CLASS ==ECC2)
StatusType SetEvent  ( TaskType xTaskID , EventMaskType pxMask );
StatusType ClearEvent( EventMaskType xMask );
StatusType GetEvent  ( TaskType xTaskID , EventMaskRefType pxEvent );
StatusType WaitEvent ( EventMaskType xMask );
#endif
#if (cfgOS_USE_RESOURCE ==  STD_TRUE)
StatusType GetResource (ResourceType xResID);
StatusType ReleaseResource ( ResourceType xResID );
#endif

void StartOS ( AppModeType xMode );
void ShutdownOS( StatusType xError );

#if (cfgOS_SHUT_DOWN_HOOK == 1)
void ShutdownHook ( StatusType xError);
#endif
#if (cfgOS_START_UP_HOOK == 1)
void StartupHook(void);
#endif
#if (cfgOS_ERROR_HOOK == 1)
void ErrorHook(StatusType xError);
#endif
#if (cfgOS_PRE_TASK_HOOK == 1)
void PreTaskHook(void);
#endif
#if (cfgOS_POST_TASK_HOOK == 1)
void PostTaskHook(void);
#endif

void OSEnterISR(void);
void OSExitISR(void);
#include "CfgObj.h"
#endif /* _OS_H_ */

