#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
/* GaInOS Counter And Alarm Configuration */
const AlarmBaseType OSCounterBaseTable[cfgOS_COUNTER_NUM]=
{
	{	/* vCounter1 */
		16,		/* xMaxAllowedValue */
		1,		/* xTicksPerBase */
		2,		/* xMinCycle */
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
	(VoidType) vTask1,		/* vAlarm1 */
};

/* GaInOS Task Configuration */
static TaskStackType g_vTask1Stack[512/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask1Stack[512/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask1 */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
};

#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
const uint8_t OSMaxActivateCountTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask1 */
};
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	BASIC_TASK, 		/* vTask1 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	&TaskEntry(vTask1),
};

uint32_t g_TestResult = 0;
#define TEST_TOTAL 14
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
	AlarmBaseType xAlarmBase;
	TickType xTick;
	/* TP01 */
	xStatus = GetAlarmBase(INVALID_ALARM,&xAlarmBase);
	OSTestCheck((E_OS_ID==xStatus),0);
	/* TP02 */
	xStatus = GetAlarm(INVALID_ALARM,&xTick);
	OSTestCheck((E_OS_ID==xStatus),1);
	/* TP03 */
	xStatus = GetAlarmBase(vAlarm1,&xAlarmBase);
	OSTestCheck((E_OK==xStatus),2);
	/* TP04 */
	xStatus = SetRelAlarm(INVALID_ALARM,0,0);
	OSTestCheck((E_OS_ID==xStatus),3);
	/* TP05 */
	xStatus = SetRelAlarm(vAlarm1,-1,0);
	OSTestCheck((E_OS_VALUE==xStatus),4);
	/* TP06 */
	xStatus = SetRelAlarm(vAlarm1,xAlarmBase.xMaxAllowedValue+1,0);
	OSTestCheck((E_OS_VALUE==xStatus),5);
	/* TP07 */
	xStatus = SetRelAlarm(vAlarm1,0,xAlarmBase.xMinCycle-1);
	OSTestCheck((E_OS_VALUE==xStatus),6);
	/* TP08 */
	xStatus = SetRelAlarm(vAlarm1,0,xAlarmBase.xMaxAllowedValue+1);
	OSTestCheck((E_OS_VALUE==xStatus),7);
	/* TP09 */
	xStatus = SetAbsAlarm(INVALID_ALARM,0,0);
	OSTestCheck((E_OS_ID==xStatus),8);
	/* TP10 */
	xStatus = SetAbsAlarm(vAlarm1,-1,0);
	OSTestCheck((E_OS_VALUE==xStatus),9);
	/* TP11 */
	xStatus = SetAbsAlarm(vAlarm1,xAlarmBase.xMaxAllowedValue+1,0);
	OSTestCheck((E_OS_VALUE==xStatus),10);
	/* TP12 */
	xStatus = SetAbsAlarm(vAlarm1,0,xAlarmBase.xMinCycle-1);
	OSTestCheck((E_OS_VALUE==xStatus),11);
	/* TP13 */
	xStatus = SetAbsAlarm(vAlarm1,0,xAlarmBase.xMaxAllowedValue+1);
	OSTestCheck((E_OS_VALUE==xStatus),12);
	/* TP14 */
	xStatus = CancelAlarm(INVALID_ALARM);
	OSTestCheck((E_OS_ID==xStatus),13);
	printk("vTask1 is running.\n");
	ShutdownOS(E_OK);
	(void)TerminateTask();
}

