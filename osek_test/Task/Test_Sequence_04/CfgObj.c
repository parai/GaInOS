#include "CfgObj.h"
#include "Serial.h"

#include "testinfo.h"
#include "Gpt.h"
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
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	EXTEND_TASK, 		/* vTask1 */
	EXTEND_TASK, 		/* vTask2 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	TaskEntry(vTask1),
	TaskEntry(vTask2),
};
uint32_t g_TestResult = 0;
#define TEST_TOTAL 4
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}
static uint8_t g_tp00 = 0xFF;

TASK( vTask1 )
{
    StatusType xStatus;
    EventMaskType xMask;
    /* TP00 -> TC06 */
    /* Call ActivateTask() from non-preemptive task on suspended extended task */
    xStatus = ActivateTask(vTask2);
    OSTestCheck((xStatus == E_OK)&&(g_tp00 == 0xFF),0);
 
    /* TP01 */
    xStatus = GetEvent(vTask1,&xMask);
    OSTestCheck((E_OK == xStatus)&&(NO_EVENT == xMask),1);

    /* TP02 */
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((E_OK == xStatus)&&(NO_EVENT == xMask),2);

    /* TP03 */
    xStatus = Schedule();
    OSTestCheck((E_OK == xStatus)&&(g_tp00 == 0x00),3);
    ShutdownOS(E_OK);
    (void)TerminateTask();		
}

TASK( vTask2 )
{
    /* For TP00 */
    g_tp00 = 0x00;
    (void)TerminateTask();
}

