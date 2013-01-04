#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
/* GaInOS Task Configuration */
static TaskStackType g_vTask1Stack[512/4];
static TaskStackType g_vTask2Stack[512/4];
static TaskStackType g_vTask3Stack[512/4];
static TaskStackType g_vTask4Stack[512/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask1Stack[512/4 -1]},
	{&g_vTask2Stack[512/4 -1]},
	{&g_vTask3Stack[512/4 -1]},
	{&g_vTask4Stack[512/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	2,		/* vTask1 */
	3,		/* vTask2 */
	1,		/* vTask3 */
	4,		/* vTask4 */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
	STD_FALSE,		/* vTask3 */
	STD_FALSE,		/* vTask4 */
};

#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
const uint8_t OSMaxActivateCountTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask1 */
	1,		/* vTask2 */
	1,		/* vTask3 */
	1,		/* vTask4 */
};
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
	STD_TRUE,		/* vTask3 */
	STD_TRUE,		/* vTask4 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	BASIC_TASK, 		/* vTask1 */
	EXTEND_TASK, 		/* vTask2 */
	EXTEND_TASK, 		/* vTask3 */
	BASIC_TASK, 		/* vTask4 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	TaskEntry(vTask1),
	TaskEntry(vTask2),
	TaskEntry(vTask3),
	TaskEntry(vTask4),
};

uint8_t g_tp01=0xFF;
uint8_t g_tp06=0xFF;
uint8_t g_tp07=0xFF;
uint8_t g_tp08=0xFF;
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
TASK(vTask1){
/* Add Your Task Code Here. */
    StatusType xStatus;
    TaskStateType xState;
    EventMaskType xMask;
    /* For TP01 */
    g_tp01 = 0x01;
    /* TP02 */
    xStatus = SetEvent(vTask2,vEvent2);
    xStatus += GetTaskState(vTask2,&xState);
    OSTestCheck((E_OK==xStatus)&&(WAITING==xState),1);
    /* TP03 */
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((E_OK==xStatus)&&(vEvent2==xMask),2);
    /* TP04 */
    xStatus = ActivateTask(vTask3);
    xStatus += GetTaskState(vTask3,&xState);
    OSTestCheck((E_OK==xStatus)&&(READY==xState),3);
    /* TP05 */
    xStatus = SetEvent(vTask3,vEvent3);
    xStatus += GetEvent(vTask3,&xMask);
    OSTestCheck((E_OK==xStatus)&&(vEvent3==xMask),4);
    /* TP06 */
    xStatus = SetEvent(vTask2,vEvent1);
    OSTestCheck((E_OK==xStatus)&&(0x06==g_tp06),5);
    /* For TP07 */
    g_tp07 = 0x07;
    /* TP08 */
    xStatus = ActivateTask(vTask4);
    OSTestCheck((E_OK==xStatus)&&(0x08==g_tp08),7);
	printk("vTask1 is running.\n");
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    /* TP01 */
    StatusType xStatus;
    xStatus = WaitEvent(vEvent1);
    OSTestCheck((E_OK==xStatus)&&(0x01==g_tp01),0);
    /* For TP06 */
    g_tp06=0x06;
    /* TP07 */
    xStatus = ClearEvent(vEvent1);
    xStatus += WaitEvent(vEvent1);
    OSTestCheck((E_OK==xStatus)&&(0x07==g_tp07),6);
	printk("vTask2 is running.\n");
	(void)TerminateTask();
}

TASK(vTask3){
/* Add Your Task Code Here. */
	printk("vTask3 is running.\n");
    ShutdownOS(E_OK);
	(void)TerminateTask();
}

TASK(vTask4){
/* Add Your Task Code Here. */
    StatusType xStatus;
    TaskStateType xState;
    /* For TP08 */
    g_tp08=0x08;
    /* TP09 */
    xStatus=SetEvent(vTask2,vEvent1);
    xStatus+= GetTaskState(vTask2,&xState);
    OSTestCheck((E_OK==xStatus)&&(READY == xState),8);
	printk("vTask4 is running.\n");
	(void)TerminateTask();
}

