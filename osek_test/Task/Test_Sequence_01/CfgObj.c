#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
#include "Gpt.h"

/* GaInOS Resource Configuration */
const PriorityType OSResCeilPrioTable[cfgOS_RESOURCE_NUM]=
{
	cfgOS_MAX_PRIORITY,		 /* RES_SCHEDULER */
};

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
	STD_TRUE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
};
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == ECC1)
const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM] =
{
	BASIC_TASK, 		/* vTask1 */
	BASIC_TASK, 		/* vTask2 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	TaskEntry(vTask1),
	TaskEntry(vTask2),
};

uint32_t g_TestResult = 0;
void StartupHook(void)
{
    TestHead("Task",19);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,19);
}

static void EnableInterrupt(uint8_t xIsr)
{
    (void)xIsr;                 /* Surpress the Warning */
     Gpt_Init();     
     Gpt_StartTimer(GPT_TIMER1,5000); /*5ms ISR2*/
     Gpt_StartTimer(GPT_TIMER2,5000); /*5ms ISR3*/
}
uint8_t gxIsrFlag = 0x00;
TASK( vTask1 )
{
    StatusType xStatus;
    TaskStateType xState;
    EnableInterrupt(ISR2|ISR3);  /*Enable ISR2 and ISR3*/
    /* TP00 -> TC01 */
    /* Call ActivateTask() from task-level with invalid task ID (task does not exist) */
    xStatus=ActivateTask(INVALID_TASK);
    OSTestCheck((E_OS_ID == xStatus),0);
    
    /* TP01 -> TC40 */
    /* Call GetTaskState() with invalid task ID (task does not exist) */
    xStatus= GetTaskState(INVALID_TASK,&xState);
    OSTestCheck((E_OS_ID == xStatus),1);
    /* TP02 -> TC24 */
    /* Call ChainTask() from task- level. Task-ID is invalid (does not exist). */
    xStatus=ChainTask(INVALID_TASK);
    OSTestCheck((E_OS_ID == xStatus),2);
    /* TP03 */
    xStatus=ActivateTask(vTask2);
    OSTestCheck((E_OK == xStatus),3);
#if(cfgOS_SCHEDULE_POLICY == NONE_PREEMPTIVE_SCHEDULE)
    xStatus=Schedule();                 /* Force rescheduling */
    OSTestCheck((E_OK == xStatus),3);
#endif
    /* TP07 */
    xStatus = GetResource(RES_SCHEDULER);
    OSTestCheck((E_OK == xStatus),7);
    /* TP08 -> TC22 */
    /* Call TerminateTask() while still occupying a resource */
    xStatus = TerminateTask();
    OSTestCheck((E_OS_RESOURCE == xStatus),8);
    /* TP09 -> TC27 */
    /* Call ChainTask() while still occupying a resource */
    xStatus = ChainTask(vTask2);
    OSTestCheck((E_OS_RESOURCE == xStatus),9);
    /* TP10 */
    xStatus = ReleaseResource(RES_SCHEDULER);
    OSTestCheck((E_OK == xStatus),10);
    while((gxIsrFlag&ISR2) == 0u); /* Wait Untill ISR2 has been triggered */
    while((gxIsrFlag&ISR3) == 0u); /* Wait Untill ISR3 has been triggered */
    ShutdownOS(E_OK);
    (void)TerminateTask();		
}

TASK( vTask2 )
{
    StatusType xStatus;
    /* TP04 -> TC10 */
    /* Call ActivateTask() on ready basic task which has reached max.number of activations */
    xStatus = ActivateTask(vTask1);
    OSTestCheck((E_OS_LIMIT == xStatus),4);
    /* TP05 -> TC15 */
    /* Call ActivateTask() on running basic task which has reached max.number of activations */
    xStatus = ActivateTask(vTask2);
    OSTestCheck((E_OS_LIMIT == xStatus),5);
    /* TP06 -> TC30 */
    /* Call ChainTask() on ready basic task which has reached max.number of activations */
    xStatus = ChainTask(vTask1);
    OSTestCheck((E_OS_LIMIT == xStatus),6);
    (void)TerminateTask();
}
 
static void ISR2Test(void)
{
    StatusType xStatus;
    TaskType   xTaskID;
    /* TP11 -> TC20 */
    /* Call TerminateTask() From ISR Category 1 */
    xStatus = TerminateTask();
    OSTestCheck((E_OS_CALLEVEL == xStatus),11);
    /* TP12 -> TC25 */
    /* Call ChainTask() From ISR category 1 */
    xStatus = ChainTask(vTask2);
    OSTestCheck((E_OS_CALLEVEL == xStatus),12);
    /* TP13 -> TC35 */
    /* Call Schedule() From ISR category 1 */
    xStatus = Schedule();
    OSTestCheck((E_OS_CALLEVEL == xStatus),13);
    /* TP14 -> TC37 */
    /* Call GetTaskID() From ISR category 1 */
    xStatus = GetTaskID(&xTaskID);
    OSTestCheck((E_OS_CALLEVEL == xStatus),14);

    gxIsrFlag |= ISR2;
    Gpt_StopTimer(GPT_TIMER1); 
}
static void ISR3Test(void)
{
    StatusType xStatus;
    TaskType   xTaskID;
    /* TP15 -> TC21 */
    /* Call TerminateTask() From ISR Category 2 */
    xStatus = TerminateTask();
    OSTestCheck((E_OS_CALLEVEL == xStatus),15);
    /* TP16 -> TC26 */
    /* Call ChainTask() From ISR category 2 */
    xStatus = ChainTask(vTask2);
    OSTestCheck((E_OS_CALLEVEL == xStatus),16);
    /* TP17 -> TC36 */
    /* Call Schedule() From ISR category 2 */
    xStatus = Schedule();
    OSTestCheck((E_OS_CALLEVEL == xStatus),17);
    /* TP18 -> TC38 */
    /* Call GetTaskID() From ISR category 2 */
    xStatus = GetTaskID(&xTaskID);
    OSTestCheck((E_OS_CALLEVEL == xStatus),18);
    gxIsrFlag |= ISR3;
    Gpt_StopTimer(GPT_TIMER2);
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 ISR(ISR2)
{
    EnterISR1();
    ISR2Test();
    LeaveISR1();
    
}
interrupt VectorNumber_Vpit1 ISR(ISR3)
{
    /*In ISR Category 2,Local Variant Allocation Was Not Allowed */
    EnterISR2();
    ISR3Test();
    LeaveISR2();
}
#pragma CODE_SEG DEFAULT
