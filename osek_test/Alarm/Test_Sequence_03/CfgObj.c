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
	(VoidType)((VoidType)vTask2<<16U)|((VoidType)vEvent2),		/* vAlarm1 */
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
	STD_TRUE,		/* vTask2 */
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
	EXTEND_TASK, 		/* vTask2 */
};
#endif
uint8_t g_tp01 = 0xff;
uint8_t g_tp04 = 0xff;
uint8_t g_tp05 = 0xff;
uint8_t g_wait = 0xff;
uint32_t g_TestResult = 0;
#define TEST_TOTAL 9
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}
const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	&TaskEntry(vTask1),
	&TaskEntry(vTask2),
};

TASK(vTask1){
/* Add Your Task Code Here. */
    StatusType xStatus;
    TickType xTick;
    /* For TP01 */
    g_tp01=0x01;
    /* TP02 */
    /* Initialize alarm counter */
  	xStatus = SetAbsAlarm(vAlarm1,1,1);
	OSTestCheck((E_OK==xStatus),1);
	/* TP03 */
	xStatus = SetAbsAlarm(vAlarm1,3,0);
	OSTestCheck((E_OS_STATE==xStatus),2); 
    /* For TP04 */
    while(0x04!=g_wait)
    {                           /* Force Scheduling */
        (void)Schedule();
    }
    g_tp04=0x04;
    g_wait = 0xff;
    /* For TP05 */
    while(0x05!=g_wait)
    {                           /* Force Scheduling */
        (void)Schedule();
    }
    g_tp05 = 0x05;
    g_wait = 0xff;
    /* TP06 */
    xStatus = CancelAlarm(vAlarm1);
    OSTestCheck((E_OK==xStatus),5);
    /* TP07 */
	xStatus = SetRelAlarm(vAlarm1,1,0);
	OSTestCheck((E_OK==xStatus),6);
	/* TP08 */
	xStatus = SetRelAlarm(vAlarm1,2,0);
	OSTestCheck((E_OS_STATE==xStatus),7); 
	/* TP09 */
	xStatus = GetAlarm(vAlarm1,&xTick);
	OSTestCheck((E_OK==xStatus)&&(1u==xTick),8);
    while(0x09!=g_wait)
    {
        (void)Schedule();
    }
    
	printk("vTask1 is running.\n");
    ShutdownOS(E_OK);
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP01 */
    xStatus = WaitEvent(vEvent2);
    OSTestCheck((xStatus == E_OK)&&(0x01==g_tp01),0);
    /* For TP04 */
    g_wait=0x04;
    /* TP04 */
    xStatus = ClearEvent(vEvent2);
    xStatus += WaitEvent(vEvent2);
    OSTestCheck((xStatus == E_OK)&&(0x04==g_tp04),3);
    /* For TP05 */
    g_wait = 0x05;
    /* TP05 */
    xStatus = ClearEvent(vEvent2);
    xStatus += WaitEvent(vEvent2);
    OSTestCheck((xStatus == E_OK)&&(0x05==g_tp05),4);
    /* For TP09 */
    g_wait=0x09;
	printk("vTask2 is running.\n");
	(void)TerminateTask();
}

