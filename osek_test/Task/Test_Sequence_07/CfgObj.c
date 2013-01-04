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
	2,		/* vTask1 */
	2,		/* vTask2 */
	2,		/* vTask3 */
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
#define TEST_TOTAL 8
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}

static uint8_t g_tp03 = 0xFF;
static uint8_t g_tp06 = 0xFF;
static uint8_t g_tp07 = 0xFF;

TASK( vTask1 )
{
    StatusType xStatus;
    static uint8_t actCnt = 0x00;
    actCnt++;
    if(actCnt == 0x03)
    {
        /* For TP07 */
        g_tp07 = 0x07;
    }
    if(actCnt == 0x01)
    {  
        /* TP00 */
        xStatus = ActivateTask(vTask2);
        OSTestCheck((E_OK == xStatus),0);
        /* TP01 -> TC12 */
        /* Call  ActivateTask() from non-preemptive task on ready basic task
        which has not reached max. number of activations */
        xStatus = ActivateTask(vTask2);
        OSTestCheck((E_OK == xStatus),1);
        /* TP03 */
        xStatus = Schedule();       /* Force Scheduling */
        OSTestCheck((xStatus == E_OK)&&(g_tp03 == 0x03),3);

        /* TP04 -> TC17 */
        xStatus = ActivateTask(vTask1);
        OSTestCheck((E_OK == xStatus),4);
        /* TP05 */
        xStatus = ActivateTask(vTask3);
        OSTestCheck((E_OK == xStatus),5);
        /* TP06 */
        (void)ChainTask(vTask3);
             
    }
    /* TP06 */
    OSTestCheck((g_tp06 == 0x06),6); 
    /* TP07 */
    if(actCnt == 0x02)
    {
        xStatus = ActivateTask(vTask1);
        OSTestCheck((E_OK == xStatus),7);
        (void)TerminateTask();
    }
    OSTestCheck((0x07 == g_tp07),7);
    ShutdownOS(E_OK);
    (void)TerminateTask();		
}

TASK( vTask2 )
{
    static uint8_t actCnt=0x00;
    actCnt++;
    if(actCnt == 0x02)
    {
        /* For TP03 */
        g_tp03 = 0x03;
    }
    (void)TerminateTask();
}

TASK( vTask3 )
{
    static uint8_t actCnt = 0x00;
    actCnt++;
    if(0x02 == actCnt)
    {
        /* For TP06 */
        g_tp06 = 0x06;
    }
    (void)TerminateTask();
}
