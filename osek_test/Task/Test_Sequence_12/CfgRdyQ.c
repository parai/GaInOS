#include "Os.h"

#if (cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY )
#    if(cfgOS_SUM_ACTIVATES > 0)

static TaskType g_RdyQueueOfPriority0[2];
static TaskType g_RdyQueueOfPriority1[8];
static TaskType g_RdyQueueOfPriority2[3];
static TaskType g_RdyQueueOfPriority3[2];
static TaskType g_RdyQueueOfPriority4[2];
static TaskType g_RdyQueueOfPriority5[2];
static TaskType g_RdyQueueOfInvalidPriority[1]={INVALID_TASK};

const TaskRefType OSTskRdyQueuePtr[cfgOS_MAX_PRIORITY+2]=
{
	g_RdyQueueOfPriority0,
	g_RdyQueueOfPriority1,
	g_RdyQueueOfPriority2,
	g_RdyQueueOfPriority3,
	g_RdyQueueOfPriority4,
	g_RdyQueueOfPriority5,
	g_RdyQueueOfInvalidPriority
};

const uint8_t OSTskRdyQueueSize[cfgOS_MAX_PRIORITY+2]=
{
	2,		/* Priority 0*/
	8,		/* Priority 1*/
	3,		/* Priority 2*/
	2,		/* Priority 3*/
	2,		/* Priority 4*/
	2,		/* Priority 5*/
	1		/* Invalid Priority */
};

#    endif
#endif

