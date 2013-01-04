#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
#include "Gpt.h"

/* GaInOS Resource Configuration */
const PriorityType OSResCeilPrioTable[cfgOS_RESOURCE_NUM]=
{
	cfgOS_MAX_PRIORITY,		 /* RES_SCHEDULER */
	5,		/* vRes1 */
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
	3,		/* vTask2 */
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
	EXTEND_TASK, 		/* vTask2 */
};
#endif

const TaskEntryType OSTaskEntryTable[cfgOS_TASK_NUM]=
{
	TaskEntry(vTask1),
	TaskEntry(vTask2),
};

static void EnableInterrupt(uint8_t xIsr)
{
    (void)xIsr;                 /* Surpress the Warning */
     Gpt_Init();     
     Gpt_StartTimer(GPT_TIMER1,5000); /*5ms ISR2*/
     Gpt_StartTimer(GPT_TIMER2,5000); /*5ms ISR3*/
}
uint8_t gxIsrFlag = 0x00;
uint32_t g_TestResult = 0;
#define TEST_TOTAL 15
void StartupHook(void)
{
    TestHead("Task",TEST_TOTAL);
}
void ShutdownHook ( StatusType xError)
{
    TestEnd(g_TestResult,TEST_TOTAL);
}

TASK(vTask1){
/* Add Your Task Code Here. */
    StatusType xStatus;
    EventMaskType xMask;
    EnableInterrupt(ISR2|ISR3);  /*Enable ISR2 and ISR3*/

    /* TP01 */
    xStatus = SetEvent(INVALID_TASK,vEvent1);
    OSTestCheck((xStatus == E_OS_ID),0);
    /* TP02 */
    xStatus = SetEvent(vTask1,vEvent1);
    OSTestCheck((xStatus == E_OS_ACCESS),1);
    /* TP03 */
    xStatus = SetEvent(vTask2,vEvent1);
    OSTestCheck((xStatus == E_OS_STATE),2);
    /* TP04 */
    xStatus = ClearEvent(vEvent1);
    OSTestCheck((xStatus == E_OS_ACCESS),3);
    /* trigger interrupt ISR2  */
    while(gxIsrFlag&ISR2==0);

    /* trigger interrupt ISR3  */
    while(gxIsrFlag&ISR3==0);
    
    /* TP09 */
    xStatus = GetEvent(INVALID_TASK,&xMask);
    OSTestCheck((xStatus == E_OS_ID),8);
    /* TP10 */
    xStatus = GetEvent(vTask1,&xMask);
    OSTestCheck((xStatus == E_OS_ACCESS),9);
    /* TP11 */
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((xStatus == E_OS_STATE),10);
    /* TP12 */
    xStatus = WaitEvent(vEvent1);
    OSTestCheck((xStatus == E_OS_ACCESS),11);
	printk("vTask1 is running.\n");
    (void)ChainTask(vTask2);
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP13 */
    xStatus = GetResource(vRes1);
    OSTestCheck((xStatus == E_OK),12);
    /* TP14 */
    xStatus = WaitEvent(vEvent1);
    OSTestCheck((xStatus == E_OS_RESOURCE),13);
    /* TP15 */
    xStatus = ReleaseResource(vRes1);
    OSTestCheck((xStatus == E_OK),14);
	printk("vTask2 is running.\n");
    ShutdownOS(E_OK);
	(void)TerminateTask();
}

static void ISR2Test(void)
{
    StatusType xStatus;
    /* TP05 */
    xStatus = ClearEvent(vEvent1);
    OSTestCheck((xStatus == E_OS_CALLEVEL),4);
    /* TP06 */
    xStatus = WaitEvent(vEvent1);
    OSTestCheck((xStatus == E_OS_CALLEVEL),5);

    gxIsrFlag |= ISR2;
    Gpt_StopTimer(GPT_TIMER1); 
}
static void ISR3Test(void)
{
    StatusType xStatus;
    /* TP07 */
    xStatus = ClearEvent(vEvent1);
    OSTestCheck((xStatus == E_OS_CALLEVEL),6);
    /* TP08 */
    xStatus = WaitEvent(vEvent1);
    OSTestCheck((xStatus == E_OS_CALLEVEL),7);

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

