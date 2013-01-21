#include "CfgObj.h"
#include "Serial.h"


/* GaInOS Resource Configuration */
const PriorityType OSResCeilPrioTable[cfgOS_RESOURCE_NUM]=
{
	cfgOS_MAX_PRIORITY,		 /* RES_SCHEDULER */
	25,		/* vRes1 */
};

/* GaInOS Counter And Alarm Configuration */
const AlarmBaseType OSCounterBaseTable[cfgOS_COUNTER_NUM]=
{
	{	/* vCounter1 */
		65535,		/* xMaxAllowedValue */
		1,		/* xTicksPerBase */
		1		/* xMinCycle */
	},
	{	/* vCounter2 */
		65535,		/* xMaxAllowedValue */
		1,		/* xTicksPerBase */
		1		/* xMinCycle */
	},
};

/* GaInOS Task Configuration */
static TaskStackType g_vTask1Stack[512/4];
static TaskStackType g_vTask2Stack[512/4];
static TaskStackType g_vTask3Stack[512/4];
static TaskStackType g_vTask4Stack[512/4];
static TaskStackType g_vTask5Stack[512/4];
static TaskStackType g_vTask6Stack[512/4];
static TaskStackType g_vTaskIdleStack[512/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask1Stack[512/4 -1]},
	{&g_vTask2Stack[512/4 -1]},
	{&g_vTask3Stack[512/4 -1]},
	{&g_vTask4Stack[512/4 -1]},
	{&g_vTask5Stack[512/4 -1]},
	{&g_vTask6Stack[512/4 -1]},
	{&g_vTaskIdleStack[512/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	7,		/* vTask1 */
	2,		/* vTask2 */
	3,		/* vTask3 */
	4,		/* vTask4 */
	5,		/* vTask5 */
	6,		/* vTask6 */
	0,		/* vTaskIdle */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
	STD_FALSE,		/* vTask3 */
	STD_FALSE,		/* vTask4 */
	STD_FALSE,		/* vTask5 */
	STD_FALSE,		/* vTask6 */
	STD_TRUE,		/* vTaskIdle */
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
	1,		/* vTaskIdle */
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
	STD_TRUE,		/* vTaskIdle */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	EXTEND_TASK, 		/* vTask1 */
	BASIC_TASK, 		/* vTask2 */
	BASIC_TASK, 		/* vTask3 */
	BASIC_TASK, 		/* vTask4 */
	BASIC_TASK, 		/* vTask5 */
	BASIC_TASK, 		/* vTask6 */
	BASIC_TASK, 		/* vTaskIdle */
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
	TaskEntry(vTaskIdle),
};

TASK(vTask1){
/* Add Your Task Code Here. */
	WaitEvent(vTask1Event1);
	ClearEvent(vTask1Event1);
	printk("vTask1 is running.\r\n");
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */

	printk("vTask2 is running.\r\n");
	(void)TerminateTask();
}

TASK(vTask3){
/* Add Your Task Code Here. */

	printk("vTask3 is running.\r\n");
	(void)TerminateTask();
}

TASK(vTask4){
/* Add Your Task Code Here. */

	printk("vTask4 is running.\r\n");
	(void)TerminateTask();
}

TASK(vTask5){
/* Add Your Task Code Here. */
	GetResource(vRes1);
	SetEvent(vTask1,vTask1Event1);
	printk("vTask5 is running.\r\n");
	ReleaseResource(vRes1);
	(void)TerminateTask();
}

TASK(vTask6){
/* Add Your Task Code Here. */

	printk("vTask6 is running.\r\n");
	(void)TerminateTask();
}

TASK(vTaskIdle){
/* Add Your Task Code Here. */

	printk("vTaskIdle is running.\r\n");
	for(;;);
	(void)TerminateTask();
}

