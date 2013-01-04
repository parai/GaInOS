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
	1,		/* vTask1 */
	2,		/* vTask2 */
	2,		/* vTask3 */
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
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
	STD_TRUE,		/* vTask3 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	BASIC_TASK, 		/* vTask1 */
	EXTEND_TASK, 		/* vTask2 */
	EXTEND_TASK, 		/* vTask3 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	TaskEntry(vTask1),
	TaskEntry(vTask2),
	TaskEntry(vTask3),
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
static uint8_t g_tp02 = 0xFF;

TASK( vTask1 )
{
    StatusType xStatus;
    TaskType xTaskID;
    TaskStateType xState;
    /* TP00 */
    xStatus = ActivateTask(vTask2);
    OSTestCheck((E_OK == xStatus)&&(g_tp00 == 0x00),0);
	OSTestCheck((g_tp02 == 0x02),0);
	ShutdownOS(E_OK);
	(void)TerminateTask();
  	
}

TASK( vTask2 )
{
    StatusType xStatus;
    EventMaskType xEventMask;
    /* For TP00 */
    g_tp00 = 0x00;
    /* TP01 */
	xStatus = GetEvent(vTask2 ,&xEventMask);
    OSTestCheck((E_OK == xStatus)&&(0x00 == xEventMask),1);
	/* TP02 -> TC09 */
	xStatus = ActivateTask(vTask3);
	OSTestCheck((E_OK == xStatus)&&(0xFF == g_tp02),2);
  
    (void)TerminateTask();
}

TASK( vTask3 )
{
    StatusType xStatus;
    EventMaskType xEventMask;
	/* For TP02 */
	g_tp02 = 0x02;
	/* TP03 */
	xStatus = GetEvent(vTask3 ,&xEventMask);
    OSTestCheck((E_OK == xStatus)&&(0x00 == xEventMask),3);

    (void)TerminateTask();
}
