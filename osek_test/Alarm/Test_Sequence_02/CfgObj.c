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

uint8_t g_tp06 = 0xff;
uint8_t g_tp10 = 0xff;

uint32_t g_TestResult = 0;
#define TEST_TOTAL 10
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
	xStatus = GetAlarmBase(vAlarm1,&xAlarmBase);
	OSTestCheck((E_OK==xStatus),0);
	/* TP02 */
	xStatus = GetAlarm(vAlarm1,&xTick);
	OSTestCheck((E_OS_NOFUNC==xStatus),1);
	/* TP03 */
	xStatus = CancelAlarm(vAlarm1);
	OSTestCheck((E_OS_NOFUNC==xStatus),2);
	/* TP04 */
	/* Initialize  alarm counter */
	xStatus = SetAbsAlarm(vAlarm1,1,2);
	OSTestCheck((E_OK==xStatus),3);
	/* TP05 */
	xStatus = SetAbsAlarm(vAlarm1,3,0);
	OSTestCheck((E_OS_STATE==xStatus),4);
	/* TP06 */
	/* Trigger counter interrupt */
	while(0xff==g_tp06);
#if(cfgOS_SCHEDULE_POLICY==NONE_PREEMPTIVE_SCHEDULE)
	(void)Schedule();
#endif
	OSTestCheck((0x06==g_tp06),5);
	/* TP07 */
	xStatus = CancelAlarm(vAlarm1);
	xStatus+= SetRelAlarm(vAlarm1,1,0);
	OSTestCheck((E_OK==xStatus),6);
	/* TP08 */
	xStatus = SetRelAlarm(vAlarm1,2,0);
	OSTestCheck((E_OS_STATE==xStatus),7);
	/* TP09 */
	xStatus = GetAlarm(vAlarm1,&xTick);
	OSTestCheck((E_OK==xStatus)&&(1u==xTick),8);
	/* TP10 */
	/* Trigger counter interrupt */
	while(0xff==g_tp10);
	OSTestCheck((0x10==g_tp10),9);
	printk("vTask1 is running.\n");
	ShutdownOS(E_OK);
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */

	if(0xff==g_tp06){
		/* For TP06 */
		g_tp06=0x06;
	}
	else{
		/* For TP10 */	
		g_tp10=0x10;
	}
	printk("vTask2 is running.\n");
	(void)TerminateTask();
}

