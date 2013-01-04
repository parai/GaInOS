#include "CfgObj.h"
#include "Serial.h"

/* GaInOS Counter And Alarm Configuration */
const AlarmBaseType OSCounterBaseTable[cfgOS_COUNTER_NUM]=
{
	{	/* vCounter1 */
		1000,		/* xMaxAllowedValue */
		1,		/* xTicksPerBase */
		1,		/* xMinCycle */
	},
};

#if(cfgOS_USE_ALARM==STD_TRUE)
const AlarmClassType OSAlarmClassTable[cfgOS_ALARM_NUM]=
{
	ALARM_TASK,		/* vAlarm1 */
	ALARM_TASK,		/* vAlarm2 */
	ALARM_TASK,		/* vAlarm3 */
};

const CounterType OSAlarmOwnerTable[cfgOS_ALARM_NUM]=
{
	vCounter1,		/* vAlarm1 */
	vCounter1,		/* vAlarm2 */
	vCounter1,		/* vAlarm3 */
};

const AlarmContainerType OSAlarmContainerTable[cfgOS_ALARM_NUM]=
{
	(VoidType) vTask4,		/* vAlarm1 */
	(VoidType) vTask5,		/* vAlarm2 */
	(VoidType) vTask6,		/* vAlarm3 */
};

#endif/* GaInOS Task Configuration */
static TaskStackType g_vTask1Stack[512/4];
static TaskStackType g_vTask2Stack[512/4];
static TaskStackType g_vTask3Stack[512/4];
static TaskStackType g_vTask4Stack[512/4];
static TaskStackType g_vTask5Stack[512/4];
static TaskStackType g_vTask6Stack[512/4];
static TaskStackType g_vTaskStartStack[512/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask1Stack[512/4 -1]},
	{&g_vTask2Stack[512/4 -1]},
	{&g_vTask3Stack[512/4 -1]},
	{&g_vTask4Stack[512/4 -1]},
	{&g_vTask5Stack[512/4 -1]},
	{&g_vTask6Stack[512/4 -1]},
	{&g_vTaskStartStack[512/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask1 */
	2,		/* vTask2 */
	3,		/* vTask3 */
	4,		/* vTask4 */
	5,		/* vTask5 */
	6,		/* vTask6 */
	7,		/* vTaskStart */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
	STD_FALSE,		/* vTask3 */
	STD_FALSE,		/* vTask4 */
	STD_FALSE,		/* vTask5 */
	STD_FALSE,		/* vTask6 */
	STD_TRUE,		/* vTaskStart */
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
	1,		/* vTaskStart */
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
	STD_TRUE,		/* vTaskStart */
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
	BASIC_TASK, 		/* vTaskStart */
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
	TaskEntry(vTaskStart),
};

TASK(vTask1){
/* Add Your Task Code Here. */

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

TASK(vTaskStart){
/* Add Your Task Code Here. */

	printk("vTaskStart is running.\n");
	(void)TerminateTask();
}

