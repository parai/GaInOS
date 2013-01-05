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
	1,		/* vTask1 */
	2,		/* vTask2 */
	2,		/* vTask3 */
	3,		/* vTask4 */
};

const BoolType OSTaskAutoActivateTable[cfgOS_TASK_NUM]=
{
	STD_FALSE,		/* vTask1 */
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
	BASIC_TASK, 		/* vTask3 */
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
uint32_t g_TestResult = 0;
#define TEST_TOTAL 5
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}

static uint8_t g_tp01 = 0xFF;
static uint8_t g_tp02 = 0xFF;
static uint8_t g_tp03 = 0xFF;

TASK( vTask1 )
{
    StatusType xStatus;

    /* TP02 */
    xStatus = ActivateTask(vTask3);
#if(cfgOS_SCHEDULE_POLICY == NONE_PREEMPTIVE_SCHEDULE)
    xStatus = Schedule();
#endif
    OSTestCheck((E_OK == xStatus)&&(g_tp02 == 0x02),2);

	ShutdownOS(E_OK);
	(void)TerminateTask();
  	
}

TASK( vTask2 )
{
    StatusType xStatus;
    EventMaskType xEventMask;
    /* TP00 */
    xStatus = ActivateTask(vTask1);
    OSTestCheck((E_OK == xStatus),0);
    /* TP01 */
    /* A task beeing released from the waiting state is treated like the newest task in
       the ready queue of its priority. */
    xStatus = WaitEvent(vEvent2);
    OSTestCheck((E_OK == xStatus)&&(g_tp01 == 0x01),1);

    (void)TerminateTask();
}

TASK( vTask3 )
{
    StatusType xStatus;

	/* For TP02 */
	g_tp02 = 0x02;

    /* TP03 */
    xStatus = ActivateTask(vTask4);
#if(cfgOS_SCHEDULE_POLICY == NONE_PREEMPTIVE_SCHEDULE)
    xStatus = Schedule();
#endif
    OSTestCheck((E_OK == xStatus)&&(g_tp03 == 0x03),3);
    /* For TP01 */
    g_tp01 = 0x01;
    (void)TerminateTask();
}

TASK( vTask4 )
{
    StatusType xStatus;
    /* For TP03 */
    g_tp03 = 0x03;
    /* TP04 */
    /* A task beeing released from the waiting state is treated like the newest task in
       the ready queue of its priority. */
    xStatus = SetEvent(vTask2,vEvent2);
    OSTestCheck((E_OK == xStatus),4);
    
    (void)TerminateTask();
}
