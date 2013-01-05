#include "Os.h"

#if (cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY )
#    if(cfgOS_SUM_ACTIVATES > 0)

static TaskType g_RdyQueueOfPriority0[2];
static TaskType g_RdyQueueOfPriority1[4];
static TaskType g_RdyQueueOfPriority2[4];
static TaskType g_RdyQueueOfPriority3[4];
static TaskType g_RdyQueueOfPriority4[2];
static TaskType g_RdyQueueOfPriority5[2];
static TaskType g_RdyQueueOfPriority6[2];
static TaskType g_RdyQueueOfPriority7[2];
static TaskType g_RdyQueueOfPriority8[2];
static TaskType g_RdyQueueOfPriority9[2];
static TaskType g_RdyQueueOfPriority10[2];
static TaskType g_RdyQueueOfPriority11[2];
static TaskType g_RdyQueueOfPriority12[2];
static TaskType g_RdyQueueOfPriority13[2];
static TaskType g_RdyQueueOfPriority14[2];
static TaskType g_RdyQueueOfPriority15[2];
static TaskType g_RdyQueueOfPriority16[2];
static TaskType g_RdyQueueOfPriority17[2];
static TaskType g_RdyQueueOfPriority18[2];
static TaskType g_RdyQueueOfPriority19[2];
static TaskType g_RdyQueueOfPriority20[2];
static TaskType g_RdyQueueOfPriority21[2];
static TaskType g_RdyQueueOfPriority22[2];
static TaskType g_RdyQueueOfPriority23[2];
static TaskType g_RdyQueueOfPriority24[2];
static TaskType g_RdyQueueOfPriority25[2];
static TaskType g_RdyQueueOfPriority26[2];
static TaskType g_RdyQueueOfPriority27[2];
static TaskType g_RdyQueueOfPriority28[2];
static TaskType g_RdyQueueOfPriority29[2];
static TaskType g_RdyQueueOfPriority30[2];
static TaskType g_RdyQueueOfPriority31[2];
static TaskType g_RdyQueueOfPriority32[2];
static TaskType g_RdyQueueOfPriority33[2];
static TaskType g_RdyQueueOfPriority34[2];
static TaskType g_RdyQueueOfPriority35[2];
static TaskType g_RdyQueueOfPriority36[2];
static TaskType g_RdyQueueOfPriority37[2];
static TaskType g_RdyQueueOfPriority38[2];
static TaskType g_RdyQueueOfPriority39[2];
static TaskType g_RdyQueueOfPriority40[2];
static TaskType g_RdyQueueOfPriority41[2];
static TaskType g_RdyQueueOfPriority42[2];
static TaskType g_RdyQueueOfPriority43[2];
static TaskType g_RdyQueueOfPriority44[2];
static TaskType g_RdyQueueOfPriority45[2];
static TaskType g_RdyQueueOfPriority46[2];
static TaskType g_RdyQueueOfPriority47[2];
static TaskType g_RdyQueueOfPriority48[2];
static TaskType g_RdyQueueOfPriority49[2];
static TaskType g_RdyQueueOfPriority50[2];
static TaskType g_RdyQueueOfPriority51[2];
static TaskType g_RdyQueueOfPriority52[2];
static TaskType g_RdyQueueOfPriority53[2];
static TaskType g_RdyQueueOfPriority54[2];
static TaskType g_RdyQueueOfPriority55[2];
static TaskType g_RdyQueueOfPriority56[2];
static TaskType g_RdyQueueOfPriority57[2];
static TaskType g_RdyQueueOfPriority58[2];
static TaskType g_RdyQueueOfPriority59[2];
static TaskType g_RdyQueueOfPriority60[2];
static TaskType g_RdyQueueOfPriority61[2];
static TaskType g_RdyQueueOfPriority62[2];
static TaskType g_RdyQueueOfPriority63[2];
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
	g_RdyQueueOfPriority11,
	g_RdyQueueOfPriority12,
	g_RdyQueueOfPriority13,
	g_RdyQueueOfPriority14,
	g_RdyQueueOfPriority15,
	g_RdyQueueOfPriority16,
	g_RdyQueueOfPriority17,
	g_RdyQueueOfPriority18,
	g_RdyQueueOfPriority19,
	g_RdyQueueOfPriority20,
	g_RdyQueueOfPriority21,
	g_RdyQueueOfPriority22,
	g_RdyQueueOfPriority23,
	g_RdyQueueOfPriority24,
	g_RdyQueueOfPriority25,
	g_RdyQueueOfPriority26,
	g_RdyQueueOfPriority27,
	g_RdyQueueOfPriority28,
	g_RdyQueueOfPriority29,
	g_RdyQueueOfPriority30,
	g_RdyQueueOfPriority31,
	g_RdyQueueOfPriority32,
	g_RdyQueueOfPriority33,
	g_RdyQueueOfPriority34,
	g_RdyQueueOfPriority35,
	g_RdyQueueOfPriority36,
	g_RdyQueueOfPriority37,
	g_RdyQueueOfPriority38,
	g_RdyQueueOfPriority39,
	g_RdyQueueOfPriority40,
	g_RdyQueueOfPriority41,
	g_RdyQueueOfPriority42,
	g_RdyQueueOfPriority43,
	g_RdyQueueOfPriority44,
	g_RdyQueueOfPriority45,
	g_RdyQueueOfPriority46,
	g_RdyQueueOfPriority47,
	g_RdyQueueOfPriority48,
	g_RdyQueueOfPriority49,
	g_RdyQueueOfPriority50,
	g_RdyQueueOfPriority51,
	g_RdyQueueOfPriority52,
	g_RdyQueueOfPriority53,
	g_RdyQueueOfPriority54,
	g_RdyQueueOfPriority55,
	g_RdyQueueOfPriority56,
	g_RdyQueueOfPriority57,
	g_RdyQueueOfPriority58,
	g_RdyQueueOfPriority59,
	g_RdyQueueOfPriority60,
	g_RdyQueueOfPriority61,
	g_RdyQueueOfPriority62,
	g_RdyQueueOfPriority63,
	g_RdyQueueOfInvalidPriority
};

const uint8_t OSTskRdyQueueSize[cfgOS_MAX_PRIORITY+2]=
{
	2,		/* Priority 0*/
	4,		/* Priority 1*/
	4,		/* Priority 2*/
	4,		/* Priority 3*/
	2,		/* Priority 4*/
	2,		/* Priority 5*/
	2,		/* Priority 6*/
	2,		/* Priority 7*/
	2,		/* Priority 8*/
	2,		/* Priority 9*/
	2,		/* Priority 10*/
	2,		/* Priority 11*/
	2,		/* Priority 12*/
	2,		/* Priority 13*/
	2,		/* Priority 14*/
	2,		/* Priority 15*/
	2,		/* Priority 16*/
	2,		/* Priority 17*/
	2,		/* Priority 18*/
	2,		/* Priority 19*/
	2,		/* Priority 20*/
	2,		/* Priority 21*/
	2,		/* Priority 22*/
	2,		/* Priority 23*/
	2,		/* Priority 24*/
	2,		/* Priority 25*/
	2,		/* Priority 26*/
	2,		/* Priority 27*/
	2,		/* Priority 28*/
	2,		/* Priority 29*/
	2,		/* Priority 30*/
	2,		/* Priority 31*/
	2,		/* Priority 32*/
	2,		/* Priority 33*/
	2,		/* Priority 34*/
	2,		/* Priority 35*/
	2,		/* Priority 36*/
	2,		/* Priority 37*/
	2,		/* Priority 38*/
	2,		/* Priority 39*/
	2,		/* Priority 40*/
	2,		/* Priority 41*/
	2,		/* Priority 42*/
	2,		/* Priority 43*/
	2,		/* Priority 44*/
	2,		/* Priority 45*/
	2,		/* Priority 46*/
	2,		/* Priority 47*/
	2,		/* Priority 48*/
	2,		/* Priority 49*/
	2,		/* Priority 50*/
	2,		/* Priority 51*/
	2,		/* Priority 52*/
	2,		/* Priority 53*/
	2,		/* Priority 54*/
	2,		/* Priority 55*/
	2,		/* Priority 56*/
	2,		/* Priority 57*/
	2,		/* Priority 58*/
	2,		/* Priority 59*/
	2,		/* Priority 60*/
	2,		/* Priority 61*/
	2,		/* Priority 62*/
	2,		/* Priority 63*/
	1		/* Invalid Priority */
};

#    endif
#endif

