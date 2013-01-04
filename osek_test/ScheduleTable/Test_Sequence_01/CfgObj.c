#include "CfgObj.h"
#include "Serial.h"

/* GaInOS Task Configuration */
static TaskStackType g_vTask3Stack[512/4];
static TaskStackType g_vTask1Stack[512/4];
static TaskStackType g_vTask2Stack[512/4];
static TaskStackType g_vTask4Stack[512/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask3Stack[512/4 -1]},
	{&g_vTask1Stack[512/4 -1]},
	{&g_vTask2Stack[512/4 -1]},
	{&g_vTask4Stack[512/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	3,		/* vTask3 */
	1,		/* vTask1 */
	2,		/* vTask2 */
	4,		/* vTask4 */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask3 */
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
	STD_TRUE,		/* vTask4 */
};

#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
const uint8_t OSMaxActivateCountTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask3 */
	1,		/* vTask1 */
	1,		/* vTask2 */
	1,		/* vTask4 */
};
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask3 */
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
	STD_TRUE,		/* vTask4 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	EXTEND_TASK, 		/* vTask3 */
	EXTEND_TASK, 		/* vTask1 */
	BASIC_TASK, 		/* vTask2 */
	BASIC_TASK, 		/* vTask4 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	TaskEntry(vTask3),
	TaskEntry(vTask1),
	TaskEntry(vTask2),
	TaskEntry(vTask4),
};

TASK(vTask3){
/* Add Your Task Code Here. */

	printk("vTask3 is running.\n");
	(void)TerminateTask();
}

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

TASK(vTask4){
/* Add Your Task Code Here. */

	printk("vTask4 is running.\n");
	(void)TerminateTask();
}

