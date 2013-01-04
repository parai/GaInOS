#include "CfgObj.h"
#include "Serial.h"
#include "testinfo.h"

/* GaInOS Resource Configuration */
const PriorityType OSResCeilPrioTable[cfgOS_RESOURCE_NUM]=
{
	5,		/* vRes1 */
};

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
	&TaskEntry(vTask1),
	&TaskEntry(vTask2),
	&TaskEntry(vTask3),
};
uint8_t g_tp02=0xff;
uint8_t g_tp04=0xff;
uint8_t g_tp06=0xff;
uint32_t g_TestResult = 0;
#define TEST_TOTAL 6
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
    /* TP01 */
    xStatus = GetResource(vRes1);
    OSTestCheck((E_OK==xStatus),0);
    /* TP02 */
    xStatus = ActivateTask(vTask2);
    OSTestCheck((E_OK==xStatus)&&(0xFF==g_tp02),1);    
    /* TP03 */
    xStatus = Schedule();
    OSTestCheck((E_OS_RESOURCE == xStatus),2);
    /* TP04 */
    xStatus = ActivateTask(vTask3);
    OSTestCheck((E_OK==xStatus)&&(0xFF==g_tp04),3);    
    /* TP05 */
    xStatus = Schedule();
    OSTestCheck((E_OS_RESOURCE == xStatus),4);
    /* TP06 */
    xStatus = ReleaseResource(vRes1);
    (void)Schedule();
    OSTestCheck((E_OK==xStatus)&&(0x06==g_tp06),5);

	printk("vTask1 is running.\n");
    
    ShutdownOS(E_OK);
	(void)TerminateTask();
}

TASK(vTask2){
/* Add Your Task Code Here. */
    /* For TP02 */
    g_tp02=0x02;
	printk("vTask2 is running.\n");
	(void)TerminateTask();
}

TASK(vTask3){
/* Add Your Task Code Here. */
    /* For TP04 */
    g_tp04=0x04;
    /* For TP06 */
    g_tp06=0x06;
	printk("vTask3 is running.\n");
	(void)TerminateTask();
}

