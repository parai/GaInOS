#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
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
#define TEST_TOTAL 9
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}
static uint8_t g_tp00 = 0xFF;
static uint8_t g_tp03 = 0xFF;
static uint8_t g_tp07 = 0xFF;
TASK( vTask1 )
{
    StatusType xStatus;
    TaskStateType xState;
    /* TP00 */
    /* Call ActivateTask() from preemptive task on suspended extended task which has
       higher priority than running task.  */
    xStatus = ActivateTask(vTask2);
    OSTestCheck((E_OK == xStatus),0);
    xStatus += Schedule();       /* Force Scheduling */
    OSTestCheck((xStatus == E_OK)&&(g_tp00 == 0x00),0);
    
    /* For TP03 */
    g_tp03 = 0x03;
    /* TP04 */
    xStatus = GetTaskState(vTask2,&xState);
    OSTestCheck((E_OK == xStatus)&&(WAITING == xState),4);
    /* TP05 -> TC19 */
    /* Call  ActivateTask() on waiting extended task */
    xStatus = ActivateTask(vTask2);
    OSTestCheck((E_OS_LIMIT == xStatus),5);
    /* TP06 -> TC33 */
    /* Call  ChainTask() on waiting extended task */
    xStatus = ChainTask(vTask2);
    OSTestCheck((E_OS_LIMIT == xStatus),6);

    /* TP07 */
    xStatus = SetEvent(vTask2,vEvent2);
    OSTestCheck((E_OK==xStatus),7);
    xStatus += Schedule();
    OSTestCheck((E_OK==xStatus)&&(0x07 == g_tp07),7);

    ShutdownOS(E_OK);
    (void)TerminateTask();		
}

TASK( vTask2 )
{
    StatusType xStatus;
    EventMaskType xMask; 
    /* For TP00 */
    g_tp00 = 0x00;

    /* TP01 -> TC11 */
    /* Call  ActivateTask() on ready extended task */
    xStatus = ActivateTask(vTask1);
    OSTestCheck((E_OS_LIMIT == xStatus),1);
    /* TP02 -> TC16 */
    /* Call  ActivateTask() on running extended task */
    xStatus = ActivateTask(vTask2);
    OSTestCheck((E_OS_LIMIT == xStatus),2);
    /* TP03 */
    xStatus = WaitEvent(vEvent2);
    OSTestCheck((xStatus == E_OK)&&(g_tp03 == 0x03),3);
    /* For TP07 */
    g_tp07 = 0x07;
    /* TP08 -> TC31*/
    /* Call  ChainTask() on ready extended task */
    xStatus = ChainTask(vTask1);
    OSTestCheck((E_OS_LIMIT == xStatus),8);
    (void)TerminateTask();
}


