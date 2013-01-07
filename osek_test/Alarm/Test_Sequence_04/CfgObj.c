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
	STD_TRUE,		/* vTask1 */
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
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
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
uint8_t g_tp01 = 0xff;
uint32_t g_TestResult = 0;
#define TEST_TOTAL 1
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}


TASK(vTask1){
/* Add Your Task Code Here. */
    StatusType xStatus;
    TaskStateType xState;
    /* TP01 */
    (void)SetRelAlarm(vAlarm1,1,0);
    /* Increment alarm counter. */
    do
    {
        xStatus = GetTaskState(vTask2,&xState);
    }
	while(SUSPENDED==xState);
    OSTestCheck((E_OK==xStatus)&&(READY==xState),0);
	printk("vTask1 is running.\n");
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    /* For TP01 */
    g_tp01=0x01;
	printk("vTask2 is running.\n");
    ShutdownOS(E_OK);
	(void)TerminateTask();
}

