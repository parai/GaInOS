#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
/* GaInOS Counter And Alarm Configuration */
const AlarmBaseType OSCounterBaseTable[cfgOS_COUNTER_NUM]=
{
	{	/* vCounter1 */
		16,		/* xMaxAllowedValue */
		1,		/* xTicksPerBase */
		1,		/* xMinCycle */
	},
};

const AlarmClassType OSAlarmClassTable[cfgOS_ALARM_NUM]=
{
	ALARM_TASK,		/* vAlarm1 */
};

const CounterType OSAlarmOwnerTable[cfgOS_ALARM_NUM]=
{
	vCounter1,		/* vAlarm1 */
};

const AlarmContainerType OSAlarmContainerTable[cfgOS_ALARM_NUM]=
{
	(VoidType) vTask2,		/* vAlarm1 */
};

/* GaInOS Task Configuration */
static TaskStackType g_vTask1Stack[512/4];
static TaskStackType g_vTask2Stack[512/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask1Stack[512/4 -1]},
	{&g_vTask2Stack[512/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask1 */
	2,		/* vTask2 */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
};

#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
const uint8_t OSMaxActivateCountTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask1 */
	1,		/* vTask2 */
};
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	BASIC_TASK, 		/* vTask1 */
	BASIC_TASK, 		/* vTask2 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	&TaskEntry(vTask1),
	&TaskEntry(vTask2),
};

uint32_t g_TestResult = 0;
#define TEST_TOTAL 20
void StartupHook(void)
{
    static uint8_t actCnt=0;
    StatusType xStatus;
    actCnt++;
    if(1==actCnt)
    {
        TestHead("Task",TEST_TOTAL);
        /* TP01 */
        xStatus = ActivateTask(vTask1);
        OSTestCheck((E_OK==xStatus),0);
    }
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}
void PreTaskHook(void)
{
    static uint8_t actCnt=0;
    StatusType xStatus;
    TaskType xTaskID;
    TaskStateType xState;
    actCnt++;
    if(1==actCnt)
    {
        /* TP02 */
        xStatus = GetTaskID(&xTaskID);
        OSTestCheck((E_OK==xStatus)&&(vTask1==xTaskID),1);
        /* TP03 */
        xStatus = GetTaskState(vTask1,&xState);
        OSTestCheck((E_OK==xStatus)&&(RUNNING==xState),2);
        /* TP04 */
        xStatus = GetTaskState(vTask2,&xState);
        OSTestCheck((E_OK==xStatus)&&(SUSPENDED==xState),3);
    }
    else if(2==actCnt)
    {
        /* TP08 */
        xStatus = GetTaskID(&xTaskID);
        OSTestCheck((E_OK==xStatus)&&(vTask2==xTaskID),7);
        /* TP09 */
        xStatus = GetTaskState(vTask1,&xState);
        OSTestCheck((E_OK==xStatus)&&(READY==xState),8);
        /* TP010 */
        xStatus = GetTaskState(vTask2,&xState);
        OSTestCheck((E_OK==xStatus)&&(RUNNING==xState),9);
    }
    else if(3==actCnt)
    {
        /* TP15 */
        xStatus = GetTaskID(&xTaskID);
        OSTestCheck((E_OK==xStatus)&&(vTask1==xTaskID),14);
        /* TP16 */
        xStatus = GetTaskState(vTask1,&xState);
        OSTestCheck((E_OK==xStatus)&&(RUNNING==xState),15);
        /* TP17 */
        xStatus = GetTaskState(vTask2,&xState);
        OSTestCheck((E_OK==xStatus)&&(SUSPENDED==xState),16);
    }
}
void PostTaskHook(void)
{
    static uint8_t actCnt=0;
    StatusType xStatus;
    TaskType xTaskID;
    TaskStateType xState;
    actCnt++;
    if(1==actCnt)
    {
        /* TP05 */
        xStatus = GetTaskID(&xTaskID);
        OSTestCheck((E_OK==xStatus)&&(vTask1==xTaskID),4);
        /* TP06 */
        xStatus = GetTaskState(vTask1,&xState);
        OSTestCheck((E_OK==xStatus)&&(RUNNING==xState),5);
        /* TP07 */
        xStatus = GetTaskState(vTask2,&xState);
        OSTestCheck((E_OK==xStatus)&&(READY==xState),6);
    }
    else if(2==actCnt)
    {
        /* TP12 */
        xStatus = GetTaskID(&xTaskID);
        OSTestCheck((E_OK==xStatus)&&(vTask2==xTaskID),11);
        /* TP13 */
        xStatus = GetTaskState(vTask1,&xState);
        OSTestCheck((E_OK==xStatus)&&(READY==xState),12);
        /* TP14 */
        xStatus = GetTaskState(vTask2,&xState);
        OSTestCheck((E_OK==xStatus)&&(RUNNING==xState),13);  
    }
    else if(3==actCnt)
    {
        /* TP18 */
        xStatus = GetTaskID(&xTaskID);
        OSTestCheck((E_OK==xStatus)&&(vTask1==xTaskID),17);
        /* TP19 */
        xStatus = GetTaskState(vTask1,&xState);
        OSTestCheck((E_OK==xStatus)&&(RUNNING==xState),18);
        /* TP20 */
        xStatus = GetTaskState(vTask2,&xState);
        OSTestCheck((E_OK==xStatus)&&(SUSPENDED==xState),19);
        ShutdownOS(E_OK);
    }
}
void ErrorHook(StatusType xError)
{
    switch(xError)
    {
        case E_OS_NOFUNC:
            printk("xError is E_OS_NOFUNC.\n");
            break;
        default:
            break;
    }
}
TASK(vTask1){
/* Add Your Task Code Here. */
    (void)ActivateTask(vTask2);
    (void)Schedule();
	printk("vTask1 is running.\n");
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    StatusType xStatus;
    TickType xTick;
    /* TP11 */
    xStatus = GetAlarm(vAlarm1,&xTick);
    OSTestCheck((E_OS_NOFUNC==xStatus),10);
	printk("vTask2 is running.\n");
	(void)TerminateTask();
}

