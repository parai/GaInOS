#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
#include "Gpt.h"
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
	1,		/* vTask1 */
	2,		/* vTask2 */
	3,		/* vTask3 */
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
	1,		/* vTask2 */
	1,		/* vTask3 */
};
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM]=
{
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
	STD_FALSE,		/* vTask3 */
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
#define TEST_TOTAL 3
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}
static uint8_t g_tp00 = 0xFF;
static uint8_t g_tp01 = 0xFF;

TASK( vTask1 )
{
    StatusType xStatus;
    TaskStateType xState;
    /* TP00 -> TC02 */
    /* Call ActivateTask() from non-preemptive task on suspended basic task */
    xStatus = ActivateTask(vTask2);
    OSTestCheck((xStatus == E_OK)&&(g_tp00 == 0xFF),0);
    /* TP01 -> TC02 */
    /* Call ActivateTask() from non-preemptive task on suspended basic task */
    xStatus = ActivateTask(vTask3);
    OSTestCheck((xStatus == E_OK)&&(g_tp01 == 0xFF),1);
    /* TP02 */
    g_tp00 = 0xFF;
    g_tp01 = 0xFF;
    xStatus = Schedule();
    OSTestCheck((xStatus == E_OK)&&(g_tp00 == 0x00)
                &&(g_tp01 == 0x01),2);

    ShutdownOS(E_OK);
    (void)TerminateTask();		
}

TASK( vTask2 )
{
    StatusType xStatus;
    /* For TP00 */
    g_tp00 = 0x00;
    (void)TerminateTask();
}

TASK( vTask3 )
{
    /* For TP01 */
    g_tp01 = 0x01;
    (void)TerminateTask();
}


