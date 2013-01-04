#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
/* GaInOS Task Configuration */
static TaskStackType g_vTask1Stack[512/4];
static TaskStackType g_vTask2Stack[512/4];
static TaskStackType g_vTask3Stack[512/4];

const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM]=
{
	{&g_vTask1Stack[512/4 -1]},
	{&g_vTask2Stack[512/4 -1]},
	{&g_vTask3Stack[512/4 -1]},
};

const PriorityType OSTaskInitPriorityTable[cfgOS_TASK_NUM]=
{
	2,		/* vTask1 */
	1,		/* vTask2 */
	1,		/* vTask3 */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
	STD_FALSE,		/* vTask3 */
};

#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
const uint8_t OSMaxActivateCountTable[cfgOS_TASK_NUM]=
{
	1,		/* vTask1 */
	3,		/* vTask2 */
	3,		/* vTask3 */
};
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM]=
{
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
	STD_TRUE,		/* vTask3 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	BASIC_TASK, 		/* vTask1 */
	BASIC_TASK, 		/* vTask2 */
	BASIC_TASK, 		/* vTask3 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	TaskEntry(vTask1),
	TaskEntry(vTask2),
	TaskEntry(vTask3),
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

TASK( vTask1 )
{
    StatusType xStatus;
    /* TP01 */
    /* A preempted task is considered to be the first task in the ready queue of its
       current priority. */
    /* 预期执行结果是：
       vTask2 is running.
       vTask3 is running.
       vTask2 is running.
       vTask2 is running.
       vTask3 is running.
       vTask3 is running.
     */
    xStatus  = ActivateTask(vTask2);
    xStatus += ActivateTask(vTask3);
    xStatus += ActivateTask(vTask2);
    xStatus += ActivateTask(vTask2);
    xStatus += ActivateTask(vTask3);
    xStatus += ActivateTask(vTask3);
    OSTestCheck((E_OK == xStatus),0);
    ShutdownOS(E_OK);
	(void)TerminateTask();
}

TASK( vTask2 )
{
    printk("vTask2 is running.\n");
    (void)TerminateTask();
}

TASK( vTask3 )
{
    printk("vTask3 is running.\n");
    (void)TerminateTask();
}
