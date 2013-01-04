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
	STD_FALSE,		/* vTask1 */
	STD_FALSE,		/* vTask2 */
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

uint8_t g_tp01=0xFF;
uint8_t g_tp04=0xFF;
uint32_t g_TestResult = 0;
#define TEST_TOTAL 11
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
    TaskStateType xState;
    EventMaskType xMask;
    /* For TP01 */
    g_tp01=0x01;
    /* TP02 */
    xStatus = GetTaskState(vTask2,&xState);
    OSTestCheck((E_OK==xStatus)&&(WAITING==xState),1);
    /* TP03 */
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((E_OK==xStatus)&&(0x00==xMask),2);
    /* TP04 */
    xStatus = SetEvent(vTask2,vEvent2);
    OSTestCheck((E_OK==xStatus)&&(0xff==g_tp04),3);
    /* TP05 */
    xStatus = GetTaskState(vTask2,&xState);
    OSTestCheck((E_OK==xStatus)&&(WAITING==xState),4);
    /* TP06 */
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((E_OK==xStatus)&&(vEvent2==xMask),5);
    /* TP07 */
    xStatus = SetEvent(vTask2,vEvent1);
    OSTestCheck((E_OK==xStatus)&&(0xff==g_tp04),6);
    /* TP08 */
    xStatus = GetTaskState(vTask2,&xState);
    OSTestCheck((E_OK==xStatus)&&(READY==xState),7);
    /* TP09 */
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((E_OK==xStatus)&&((vEvent1|vEvent2)==xMask),8);
    /* TP10 */
    xStatus = SetEvent(vTask2,vEvent3);
    OSTestCheck((E_OK==xStatus)&&(0xff==g_tp04),9);
    /* TP11 */
    xStatus = GetEvent(vTask2,&xMask);
    OSTestCheck((E_OK==xStatus)&&((vEvent1|vEvent2|vEvent3)==xMask),10);
	printk("vTask1 is running.\n");
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    StatusType xStatus;
    /* TP01 */
    xStatus = WaitEvent(vEvent1);
    OSTestCheck((E_OK==xStatus)&&(0x01==g_tp01),0);
    /* For TP04 */
    g_tp04=0x04;
	printk("vTask2 is running.\n");
	ShutdownOS(E_OK);
    (void)TerminateTask();
}

