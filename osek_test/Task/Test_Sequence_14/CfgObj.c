#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
/* GaInOS Task Configuration */
static TaskStackType g_vTask1Stack[256/4];
static TaskStackType g_vTask2Stack[256/4];
static TaskStackType g_vTask3Stack[256/4];
static TaskStackType g_vTask4Stack[256/4];
static TaskStackType g_vTask5Stack[256/4];
static TaskStackType g_vTask6Stack[256/4];
static TaskStackType g_vTask7Stack[256/4];
static TaskStackType g_vTask8Stack[256/4];
static TaskStackType g_vTask9Stack[256/4];
static TaskStackType g_vTask10Stack[256/4];
static TaskStackType g_vTask11Stack[256/4];
static TaskStackType g_vTask12Stack[256/4];
static TaskStackType g_vTask13Stack[256/4];
static TaskStackType g_vTask14Stack[256/4];
static TaskStackType g_vTask15Stack[256/4];
static TaskStackType g_vTask16Stack[256/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask1Stack[256/4 -1]},
	{&g_vTask2Stack[256/4 -1]},
	{&g_vTask3Stack[256/4 -1]},
	{&g_vTask4Stack[256/4 -1]},
	{&g_vTask5Stack[256/4 -1]},
	{&g_vTask6Stack[256/4 -1]},
	{&g_vTask7Stack[256/4 -1]},
	{&g_vTask8Stack[256/4 -1]},
	{&g_vTask9Stack[256/4 -1]},
	{&g_vTask10Stack[256/4 -1]},
	{&g_vTask11Stack[256/4 -1]},
	{&g_vTask12Stack[256/4 -1]},
	{&g_vTask13Stack[256/4 -1]},
	{&g_vTask14Stack[256/4 -1]},
	{&g_vTask15Stack[256/4 -1]},
	{&g_vTask16Stack[256/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	16,		/* vTask1 */
	15,		/* vTask2 */
	14,		/* vTask3 */
	13,		/* vTask4 */
	12,		/* vTask5 */
	11,		/* vTask6 */
	10,		/* vTask7 */
	9,		/* vTask8 */
	8,		/* vTask9 */
	7,		/* vTask10 */
	6,		/* vTask11 */
	5,		/* vTask12 */
	4,		/* vTask13 */
	3,		/* vTask14 */
	2,		/* vTask15 */
	1,		/* vTask16 */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
	STD_FALSE,		/* vTask3 */
	STD_FALSE,		/* vTask4 */
	STD_FALSE,		/* vTask5 */
	STD_FALSE,		/* vTask6 */
	STD_FALSE,		/* vTask7 */
	STD_FALSE,		/* vTask8 */
	STD_FALSE,		/* vTask9 */
	STD_FALSE,		/* vTask10 */
	STD_FALSE,		/* vTask11 */
	STD_FALSE,		/* vTask12 */
	STD_FALSE,		/* vTask13 */
	STD_FALSE,		/* vTask14 */
	STD_FALSE,		/* vTask15 */
	STD_FALSE,		/* vTask16 */
};

#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
const uint8_t OSMaxActivateCountTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask1 */
	1,		/* vTask2 */
	1,		/* vTask3 */
	1,		/* vTask4 */
	1,		/* vTask5 */
	1,		/* vTask6 */
	1,		/* vTask7 */
	1,		/* vTask8 */
	1,		/* vTask9 */
	1,		/* vTask10 */
	1,		/* vTask11 */
	1,		/* vTask12 */
	1,		/* vTask13 */
	1,		/* vTask14 */
	1,		/* vTask15 */
	1,		/* vTask16 */
};
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
	STD_TRUE,		/* vTask3 */
	STD_TRUE,		/* vTask4 */
	STD_TRUE,		/* vTask5 */
	STD_TRUE,		/* vTask6 */
	STD_TRUE,		/* vTask7 */
	STD_TRUE,		/* vTask8 */
	STD_TRUE,		/* vTask9 */
	STD_TRUE,		/* vTask10 */
	STD_TRUE,		/* vTask11 */
	STD_TRUE,		/* vTask12 */
	STD_TRUE,		/* vTask13 */
	STD_TRUE,		/* vTask14 */
	STD_TRUE,		/* vTask15 */
	STD_TRUE,		/* vTask16 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	EXTEND_TASK, 		/* vTask1 */
	EXTEND_TASK, 		/* vTask2 */
	EXTEND_TASK, 		/* vTask3 */
	EXTEND_TASK, 		/* vTask4 */
	EXTEND_TASK, 		/* vTask5 */
	EXTEND_TASK, 		/* vTask6 */
	EXTEND_TASK, 		/* vTask7 */
	EXTEND_TASK, 		/* vTask8 */
	EXTEND_TASK, 		/* vTask9 */
	EXTEND_TASK, 		/* vTask10 */
	EXTEND_TASK, 		/* vTask11 */
	EXTEND_TASK, 		/* vTask12 */
	EXTEND_TASK, 		/* vTask13 */
	EXTEND_TASK, 		/* vTask14 */
	EXTEND_TASK, 		/* vTask15 */
	EXTEND_TASK, 		/* vTask16 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	TaskEntry(vTask1),
	TaskEntry(vTask2),
	TaskEntry(vTask3),
	TaskEntry(vTask4),
	TaskEntry(vTask5),
	TaskEntry(vTask6),
	TaskEntry(vTask7),
	TaskEntry(vTask8),
	TaskEntry(vTask9),
	TaskEntry(vTask10),
	TaskEntry(vTask11),
	TaskEntry(vTask12),
	TaskEntry(vTask13),
	TaskEntry(vTask14),
	TaskEntry(vTask15),
	TaskEntry(vTask16),
};
uint32_t g_TestResult = 0;
#define TEST_TOTAL 17
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
    /* TP01 */
    xStatus  = ActivateTask(vTask2);
    xStatus += ActivateTask(vTask3);
    xStatus += ActivateTask(vTask4);
    xStatus += ActivateTask(vTask5);
    xStatus += ActivateTask(vTask6);
    xStatus += ActivateTask(vTask7);
    xStatus += ActivateTask(vTask8);
    xStatus += ActivateTask(vTask9);
    xStatus += ActivateTask(vTask10);
    xStatus += ActivateTask(vTask11);
    xStatus += ActivateTask(vTask12);
    xStatus += ActivateTask(vTask13);
    xStatus += ActivateTask(vTask14);
    xStatus += ActivateTask(vTask15);
    xStatus += ActivateTask(vTask16);
    OSTestCheck((xStatus==E_OK),0);
    /* TP02 */
    xStatus  = ClearEvent(vTask1Event1);
    xStatus += ClearEvent(vTask1Event2);
    xStatus += ClearEvent(vTask1Event3);
    xStatus += ClearEvent(vTask1Event4);
    xStatus += ClearEvent(vTask1Event5);
    xStatus += ClearEvent(vTask1Event6);
    xStatus += ClearEvent(vTask1Event7);
    xStatus += ClearEvent(vTask1Event8);
    OSTestCheck((xStatus==E_OK),1);
	printk("vTask1 is running.\n");
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP03 */
    xStatus  = ClearEvent(vTask2Event1);
    xStatus += ClearEvent(vTask2Event2);
    xStatus += ClearEvent(vTask2Event3);
    xStatus += ClearEvent(vTask2Event4);
    xStatus += ClearEvent(vTask2Event5);
    xStatus += ClearEvent(vTask2Event6);
    xStatus += ClearEvent(vTask2Event7);
    xStatus += ClearEvent(vTask2Event8);
    OSTestCheck((xStatus==E_OK),2);
	printk("vTask2 is running.\n");
	(void)TerminateTask();
}

TASK(vTask3){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP04 */
    xStatus  = ClearEvent(vTask3Event1);
    xStatus += ClearEvent(vTask3Event2);
    xStatus += ClearEvent(vTask3Event3);
    xStatus += ClearEvent(vTask3Event4);
    xStatus += ClearEvent(vTask3Event5);
    xStatus += ClearEvent(vTask3Event6);
    xStatus += ClearEvent(vTask3Event7);
    xStatus += ClearEvent(vTask3Event8);
    OSTestCheck((xStatus==E_OK),3);
	printk("vTask3 is running.\n");
	(void)TerminateTask();
}

TASK(vTask4){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP05 */
    xStatus  = ClearEvent(vTask4Event1);
    xStatus += ClearEvent(vTask4Event2);
    xStatus += ClearEvent(vTask4Event3);
    xStatus += ClearEvent(vTask4Event4);
    xStatus += ClearEvent(vTask4Event5);
    xStatus += ClearEvent(vTask4Event6);
    xStatus += ClearEvent(vTask4Event7);
    xStatus += ClearEvent(vTask4Event8);
    OSTestCheck((xStatus==E_OK),4);
	printk("vTask4 is running.\n");
	(void)TerminateTask();
}

TASK(vTask5){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP06 */
    xStatus  = ClearEvent(vTask5Event1);
    xStatus += ClearEvent(vTask5Event2);
    xStatus += ClearEvent(vTask5Event3);
    xStatus += ClearEvent(vTask5Event4);
    xStatus += ClearEvent(vTask5Event5);
    xStatus += ClearEvent(vTask5Event6);
    xStatus += ClearEvent(vTask5Event7);
    xStatus += ClearEvent(vTask5Event8);
    OSTestCheck((xStatus==E_OK),5);
	printk("vTask5 is running.\n");
	(void)TerminateTask();
}

TASK(vTask6){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP07 */
    xStatus  = ClearEvent(vTask6Event1);
    xStatus += ClearEvent(vTask6Event2);
    xStatus += ClearEvent(vTask6Event3);
    xStatus += ClearEvent(vTask6Event4);
    xStatus += ClearEvent(vTask6Event5);
    xStatus += ClearEvent(vTask6Event6);
    xStatus += ClearEvent(vTask6Event7);
    xStatus += ClearEvent(vTask6Event8);
    OSTestCheck((xStatus==E_OK),6);
	printk("vTask6 is running.\n");
	(void)TerminateTask();
}

TASK(vTask7){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP08 */
    xStatus  = ClearEvent(vTask7Event1);
    xStatus += ClearEvent(vTask7Event2);
    xStatus += ClearEvent(vTask7Event3);
    xStatus += ClearEvent(vTask7Event4);
    xStatus += ClearEvent(vTask7Event5);
    xStatus += ClearEvent(vTask7Event6);
    xStatus += ClearEvent(vTask7Event7);
    xStatus += ClearEvent(vTask7Event8);
    OSTestCheck((xStatus==E_OK),7);
	printk("vTask7 is running.\n");
	(void)TerminateTask();
}

TASK(vTask8){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP09 */
    xStatus  = ClearEvent(vTask8Event1);
    xStatus += ClearEvent(vTask8Event2);
    xStatus += ClearEvent(vTask8Event3);
    xStatus += ClearEvent(vTask8Event4);
    xStatus += ClearEvent(vTask8Event5);
    xStatus += ClearEvent(vTask8Event6);
    xStatus += ClearEvent(vTask8Event7);
    xStatus += ClearEvent(vTask8Event8);
    OSTestCheck((xStatus==E_OK),8);
	printk("vTask8 is running.\n");
	(void)TerminateTask();
}

TASK(vTask9){
/* Add Your Task Code Here. */
    /* TP10 */
    StatusType xStatus;
    xStatus  = ClearEvent(vTask9Event1);
    xStatus += ClearEvent(vTask9Event2);
    xStatus += ClearEvent(vTask9Event3);
    xStatus += ClearEvent(vTask9Event4);
    xStatus += ClearEvent(vTask9Event5);
    xStatus += ClearEvent(vTask9Event6);
    xStatus += ClearEvent(vTask9Event7);
    xStatus += ClearEvent(vTask9Event8);
    OSTestCheck((xStatus==E_OK),9);
	printk("vTask9 is running.\n");
	(void)TerminateTask();
}

TASK(vTask10){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP11 */
    xStatus  = ClearEvent(vTask10Event1);
    xStatus += ClearEvent(vTask10Event2);
    xStatus += ClearEvent(vTask10Event3);
    xStatus += ClearEvent(vTask10Event4);
    xStatus += ClearEvent(vTask10Event5);
    xStatus += ClearEvent(vTask10Event6);
    xStatus += ClearEvent(vTask10Event7);
    xStatus += ClearEvent(vTask10Event8);
    OSTestCheck((xStatus==E_OK),10);
	printk("vTask10 is running.\n");
	(void)TerminateTask();
}

TASK(vTask11){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP12 */
    xStatus  = ClearEvent(vTask11Event1);
    xStatus += ClearEvent(vTask11Event2);
    xStatus += ClearEvent(vTask11Event3);
    xStatus += ClearEvent(vTask11Event4);
    xStatus += ClearEvent(vTask11Event5);
    xStatus += ClearEvent(vTask11Event6);
    xStatus += ClearEvent(vTask11Event7);
    xStatus += ClearEvent(vTask11Event8);
    OSTestCheck((xStatus==E_OK),11);
	printk("vTask11 is running.\n");
	(void)TerminateTask();
}

TASK(vTask12){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP13 */
    xStatus  = ClearEvent(vTask12Event1);
    xStatus += ClearEvent(vTask12Event2);
    xStatus += ClearEvent(vTask12Event3);
    xStatus += ClearEvent(vTask12Event4);
    xStatus += ClearEvent(vTask12Event5);
    xStatus += ClearEvent(vTask12Event6);
    xStatus += ClearEvent(vTask12Event7);
    xStatus += ClearEvent(vTask12Event8);
    OSTestCheck((xStatus==E_OK),12);
	printk("vTask12 is running.\n");
	(void)TerminateTask();
}

TASK(vTask13){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP14 */
    xStatus  = ClearEvent(vTask13Event1);
    xStatus += ClearEvent(vTask13Event2);
    xStatus += ClearEvent(vTask13Event3);
    xStatus += ClearEvent(vTask13Event4);
    xStatus += ClearEvent(vTask13Event5);
    xStatus += ClearEvent(vTask13Event6);
    xStatus += ClearEvent(vTask13Event7);
    xStatus += ClearEvent(vTask13Event8);
    OSTestCheck((xStatus==E_OK),13);
	printk("vTask13 is running.\n");
	(void)TerminateTask();
}

TASK(vTask14){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP15 */
    xStatus  = ClearEvent(vTask14Event1);
    xStatus += ClearEvent(vTask14Event2);
    xStatus += ClearEvent(vTask14Event3);
    xStatus += ClearEvent(vTask14Event4);
    xStatus += ClearEvent(vTask14Event5);
    xStatus += ClearEvent(vTask14Event6);
    xStatus += ClearEvent(vTask14Event7);
    xStatus += ClearEvent(vTask14Event8);
    OSTestCheck((xStatus==E_OK),14);
	printk("vTask14 is running.\n");
	(void)TerminateTask();
}

TASK(vTask15){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP16 */
    xStatus  = ClearEvent(vTask15Event1);
    xStatus += ClearEvent(vTask15Event2);
    xStatus += ClearEvent(vTask15Event3);
    xStatus += ClearEvent(vTask15Event4);
    xStatus += ClearEvent(vTask15Event5);
    xStatus += ClearEvent(vTask15Event6);
    xStatus += ClearEvent(vTask15Event7);
    xStatus += ClearEvent(vTask15Event8);
    OSTestCheck((xStatus==E_OK),15);
	printk("vTask15 is running.\n");
	(void)TerminateTask();
}

TASK(vTask16){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP17 */
    xStatus  = ClearEvent(vTask16Event1);
    xStatus += ClearEvent(vTask16Event2);
    xStatus += ClearEvent(vTask16Event3);
    xStatus += ClearEvent(vTask16Event4);
    xStatus += ClearEvent(vTask16Event5);
    xStatus += ClearEvent(vTask16Event6);
    xStatus += ClearEvent(vTask16Event7);
    xStatus += ClearEvent(vTask16Event8);
    OSTestCheck((xStatus==E_OK),16);
	printk("vTask16 is running.\n");

    ShutdownOS(E_OK);
	(void)TerminateTask();
}

