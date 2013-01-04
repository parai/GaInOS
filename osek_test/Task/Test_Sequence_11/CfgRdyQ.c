#include "Os.h"

#if (cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY )
#    if(cfgOS_SUM_ACTIVATES > 0)

static TaskType g_RdyQueueOfPriority0[2];
static TaskType g_RdyQueueOfPriority1[3];
static TaskType g_RdyQueueOfPriority2[4];
static TaskType g_RdyQueueOfPriority3[3];
static TaskType g_RdyQueueOfPriority4[2];
static TaskType g_RdyQueueOfPriority5[2];
static TaskType g_RdyQueueOfPriority6[2];
static TaskType g_RdyQueueOfPriority7[2];
static TaskType g_RdyQueueOfPriority8[2];
static TaskType g_RdyQueueOfPriority9[2];
static TaskType g_RdyQueueOfPriority10[2];
static TaskType g_RdyQueueOfInvalidPriority[1]={INVALID_TASK};

const TaskRefType OSTskRdyQueuePtr[cfgOS_MAX_PRIORITY+2]=
{
	g_RdyQueueOfPriority0,
	g_RdyQueueOfPriority1,
	g_RdyQueueOfPriority2,
	g_RdyQueueOfPriority3,
	g_RdyQueueOfPriority4,
	g_RdyQueueOfPriority5,
	g_RdyQueueOfPriority6,
	g_RdyQueueOfPriority7,
	g_RdyQueueOfPriority8,
	g_RdyQueueOfPriority9,
	g_RdyQueueOfPriority10,
	g_RdyQueueOfInvalidPriority
};

const uint8_t OSTskRdyQueueSize[cfgOS_MAX_PRIORITY+2]=
{
	2,		/* Priority 0*/
	3,		/* Priority 1*/
	4,		/* Priority 2*/
	3,		/* Priority 3*/
	2,		/* Priority 4*/
	2,		/* Priority 5*/
	2,		/* Priority 6*/
	2,		/* Priority 7*/
	2,		/* Priority 8*/
	2,		/* Priority 9*/
	2,		/* Priority 10*/
	1		/* Invalid Priority */
};

#    endif
#endif

