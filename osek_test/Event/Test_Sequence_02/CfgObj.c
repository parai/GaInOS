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
	STD_FALSE,		/* vTask1 */
	STD_TRUE,		/* vTask2 */
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
static uint8_t g_tp02=0xff;
static uint8_t g_tp08=0xff;
TASK(vTask1){
/* Add Your Task Code Here. */
    StatusType xStatus;
    EventMaskType xMask;
    /* For TP02 */
    g_tp02=0x02;
    /* TP03 */
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((E_OK==xStatus)&&(0x00==xMask),2);
    /* TP04 */
    xStatus = SetEvent(vTask1,vEvent1);
    OSTestCheck((E_OK==xStatus),3);
    /* TP05 */
    xStatus = GetEvent(vTask1,&xMask);
    OSTestCheck((E_OK==xStatus)&&(vEvent1==xMask),4);  
    /* TP06 */
    xStatus = WaitEvent(vEvent1);
    OSTestCheck((E_OK==xStatus),5);
    /* TP07 */
    xStatus = ClearEvent(vEvent1);
    OSTestCheck((E_OK==xStatus),6);
    /* TP08 */
    xStatus = SetEvent(vTask2,vEvent2);
#if(cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
    (void)Schedule();
#endif
    OSTestCheck((E_OK==xStatus)&&(0x08==g_tp08),7);
	printk("vTask1 is running.\n");
	ShutdownOS(E_OK);
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    StatusType xStatus;
    EventMaskType xMask;
    /* TP01 */
    (void)ActivateTask(vTask1);
    xStatus = GetEvent(vTask1,&xMask);
    OSTestCheck((E_OK==xStatus)&&(0u==xMask),0);  
    /* TP02 */
    xStatus = WaitEvent(vEvent2);
    OSTestCheck((E_OK==xStatus)&&(0x02==g_tp02),1);
    /* For TP08 */
    g_tp08=0x08;
	printk("vTask2 is running.\n");
	(void)TerminateTask();
}

