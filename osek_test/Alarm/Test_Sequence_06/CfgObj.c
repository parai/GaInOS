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
	ALARM_EVENT,		/* vAlarm1 */
};

const CounterType OSAlarmOwnerTable[cfgOS_ALARM_NUM]=
{
	vCounter1,		/* vAlarm1 */
};

const AlarmContainerType OSAlarmContainerTable[cfgOS_ALARM_NUM]=
{
	(VoidType)((VoidType)vTask2<<16U)|((VoidType)vEvent1),		/* vAlarm1 */
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
	EXTEND_TASK, 		/* vTask2 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	&TaskEntry(vTask1),
	&TaskEntry(vTask2),
};

uint8_t g_tp03 = 0xff;
uint8_t g_tp04 = 0xff;
uint32_t g_TestResult = 0;
#define TEST_TOTAL 5
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
    EventMaskType xMask;
    TaskStateType xState;
    TickType xCurTick;
    /* TP01 */
    xStatus = ActivateTask(vTask2);
    xCurTick = OSCntCurValueTable[vCounter1];
    xStatus += SetRelAlarm(vAlarm1,1,0);
    OSTestCheck((E_OK==xStatus),0);
    /* TP02 */
    /* increment alarm counter */
    while(OSCntCurValueTable[vCounter1] == xCurTick);
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((E_OK==xStatus)&&(vEvent1 == xMask),1);
    /* TP03 */
    xStatus = Schedule();
    OSTestCheck((E_OK==xStatus)&&(0x03==g_tp03),2);
    /* For TP04 */
    g_tp04 = 0x04;
    /* TP05 */
    xCurTick = OSCntCurValueTable[vCounter1];
    xStatus = SetRelAlarm(vAlarm1,1,0);
    /* increment alarm value */
    while(OSCntCurValueTable[vCounter1] == xCurTick);
    xStatus += GetTaskState(vTask2,&xState);
    OSTestCheck((E_OK==xStatus)&&(READY == xState),4);
	printk("vTask1 is running.\n");
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* For TP03 */
    g_tp03 = 0x03;
    /* TP04 */
    xStatus = ClearEvent(vEvent1);
    xStatus += WaitEvent(vEvent1);
    OSTestCheck((E_OK==xStatus)&&(0x04==g_tp04),3);
	printk("vTask2 is running.\n");
    ShutdownOS(E_OK);
	(void)TerminateTask();
}

