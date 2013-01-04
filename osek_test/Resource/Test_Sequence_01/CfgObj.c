#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"
#include "Gpt.h"

/* GaInOS Resource Configuration */
const PriorityType OSResCeilPrioTable[cfgOS_RESOURCE_NUM]=
{
	5,		/* vRes1 */
	6,		/* vRes2 */
	7,		/* vRes3 */
	8,		/* vRes4 */
	9,		/* vRes5 */
	10,		/* vRes6 */
	0,		/* vResA */
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
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
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
	&TaskEntry(vTask1),
	&TaskEntry(vTask2),
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
#define TEST_TOTAL 7
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
    EnableInterrupt(ISR2|ISR3);
    /* TP01 */
    xStatus = GetResource(vResA);
    OSTestCheck((E_OS_ACCESS == xStatus),0);
    /* TP02 */
    xStatus = GetResource(INVALID_RESOURCE);
    OSTestCheck((E_OS_ID==xStatus),1);
    /* TP03 */
    xStatus  = GetResource(vRes1);
    xStatus += GetResource(vRes2);
    xStatus += GetResource(vRes3);
    xStatus += GetResource(vRes4);
    xStatus += GetResource(vRes5);
    xStatus += GetResource(vRes6);
    xStatus += ReleaseResource(vRes6);
    xStatus += ReleaseResource(vRes5);
    xStatus += ReleaseResource(vRes4);
    xStatus += ReleaseResource(vRes3);
    xStatus += ReleaseResource(vRes2);
    xStatus += ReleaseResource(vRes1);
    OSTestCheck((E_OK==xStatus),2);
    /* trigger interrupt ISR2 */
    while(gxIsrFlag&ISR2==0);
    /* trigger interrupt ISR3 */
    while(gxIsrFlag&ISR3==0);
	printk("vTask1 is running.\n");
    ShutdownOS(E_OK);
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */

	printk("vTask2 is running.\n");
	(void)TerminateTask();
}
/*
    用例描述说：中断不能够获取资源，但实际上OSEK规范描述中中断是
可以获取资源的，所以对TP04~TP07的报错进行忽略，PASS
*/
static void ISR2Test(void)
{
    StatusType xStatus;
    /* TP04 */
    xStatus = GetResource(vRes1);
    OSTestCheck((E_OS_CALLEVEL == xStatus),3);
    /* TP05 */
    xStatus = ReleaseResource(vRes1);
    OSTestCheck((E_OS_CALLEVEL == xStatus),4);
    gxIsrFlag |= ISR2;
    Gpt_StopTimer(GPT_TIMER1); 
}
static void ISR3Test(void)
{
    StatusType xStatus;
    /* TP06 */
    xStatus = GetResource(vRes1);
    OSTestCheck((E_OS_CALLEVEL == xStatus),5);
    /* TP07 */
    xStatus = ReleaseResource(vRes1);
    OSTestCheck((E_OS_CALLEVEL == xStatus),6);
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

