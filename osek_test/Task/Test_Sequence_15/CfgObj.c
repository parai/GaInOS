#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
/* GaInOS Task Configuration */
static TaskStackType g_vTask1Stack[512/4];
static TaskStackType g_vTask2Stack[512/4];
static TaskStackType g_vTask3Stack[512/4];
static TaskStackType g_vTask4Stack[512/4];
static TaskStackType g_vTask5Stack[512/4];
static TaskStackType g_vTask6Stack[512/4];
static TaskStackType g_vTask7Stack[512/4];
static TaskStackType g_vTask8Stack[512/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask1Stack[512/4 -1]},
	{&g_vTask2Stack[512/4 -1]},
	{&g_vTask3Stack[512/4 -1]},
	{&g_vTask4Stack[512/4 -1]},
	{&g_vTask5Stack[512/4 -1]},
	{&g_vTask6Stack[512/4 -1]},
	{&g_vTask7Stack[512/4 -1]},
	{&g_vTask8Stack[512/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	8,		/* vTask1 */
	8,		/* vTask2 */
	6,		/* vTask3 */
	5,		/* vTask4 */
	4,		/* vTask5 */
	3,		/* vTask6 */
	2,		/* vTask7 */
	1,		/* vTask8 */
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
};
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM]=
{
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
	STD_FALSE,		/* vTask3 */
	STD_FALSE,		/* vTask4 */
	STD_FALSE,		/* vTask5 */
	STD_FALSE,		/* vTask6 */
	STD_FALSE,		/* vTask7 */
	STD_FALSE,		/* vTask8 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	BASIC_TASK, 		/* vTask1 */
	BASIC_TASK, 		/* vTask2 */
	BASIC_TASK, 		/* vTask3 */
	BASIC_TASK, 		/* vTask4 */
	BASIC_TASK, 		/* vTask5 */
	BASIC_TASK, 		/* vTask6 */
	BASIC_TASK, 		/* vTask7 */
	BASIC_TASK, 		/* vTask8 */
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
};
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
    /* TP01 */
    xStatus  = ActivateTask(vTask2);
    xStatus += ActivateTask(vTask3);
    xStatus += ActivateTask(vTask4);
    xStatus += ActivateTask(vTask5);
    xStatus += ActivateTask(vTask6);
    xStatus += ActivateTask(vTask7);
    xStatus += ActivateTask(vTask8);
	OSTestCheck((xStatus==E_OK),0);
	
	printk("vTask1 is running.\n");
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */

	printk("vTask2 is running.\n");
	(void)TerminateTask();
}

TASK(vTask3){
/* Add Your Task Code Here. */

	printk("vTask3 is running.\n");
	(void)TerminateTask();
}

TASK(vTask4){
/* Add Your Task Code Here. */

	printk("vTask4 is running.\n");
	(void)TerminateTask();
}

TASK(vTask5){
/* Add Your Task Code Here. */

	printk("vTask5 is running.\n");
	(void)TerminateTask();
}

TASK(vTask6){
/* Add Your Task Code Here. */

	printk("vTask6 is running.\n");
	(void)TerminateTask();
}

TASK(vTask7){
/* Add Your Task Code Here. */

	printk("vTask7 is running.\n");
	(void)TerminateTask();
}

TASK(vTask8){
/* Add Your Task Code Here. */

	printk("vTask8 is running.\n");
	ShutdownOS(E_OK);
	(void)TerminateTask();
}

