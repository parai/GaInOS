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
#define TEST_TOTAL 7
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}

static uint8_t g_tp03 = 0xFF;

TASK( vTask1 )
{
    StatusType xStatus;
    TaskType xTaskID;
    TaskStateType xState;
    /* TP00 -> TC39 */
    /* Call  GetTaskID() from task */
    xStatus = GetTaskID(&xTaskID);
    OSTestCheck((E_OK == xStatus)&&(vTask1 == xTaskID),0);
    /* TP01 -> TC41 */
    /* Call  GetTaskState() */
    xStatus = GetTaskState(vTask1,&xState);
    OSTestCheck((E_OK == xStatus)&&(RUNNING == xState),1);
    /* TP02 -> TC41 */
    xStatus = GetTaskState(vTask2,&xState);
    OSTestCheck((E_OK == xStatus)&&(SUSPENDED == xState),2);
    /* TP03 */
    xStatus = ActivateTask(vTask2);
#if(cfgOS_SCHEDULE_POLICY == NONE_PREEMPTIVE_SCHEDULE)
    xStatus = Schedule();
#endif
    OSTestCheck((E_OK == xStatus)&&(g_tp03 == 0x03),3);
    /* TP05 -> TC28 */
    /* Call  ChainTask() on suspended task */
    (void)ChainTask(vTask3);
}

TASK( vTask2 )
{
    StatusType xStatus;
    TaskStateType xState;
    /* For TP03 */
    g_tp03 = 0x03;
    /* TP04 -> TC41 */
    xStatus = GetTaskState(vTask1,&xState);
    OSTestCheck((E_OK == xStatus)&&(READY == xState),4);
  
    (void)TerminateTask();
}

TASK( vTask3 )
{
    static uint8_t actCnt = 0x00;
    actCnt++;
    if(actCnt == 0x01)
    {
        OSTestCheck((STD_TRUE),5); /* For TP05 */
        /* TP06 -> TC29*/
        /* Call  ChainTask() on  running task */
        (void)ChainTask(vTask3);
    }
    if(actCnt == 0x02)
    {
        OSTestCheck((STD_TRUE),6); /* For TP06 */
    }
    ShutdownOS(E_OK);
    /* TC23 */
    /* Call  TerminateTask() */
    (void)TerminateTask();
}

