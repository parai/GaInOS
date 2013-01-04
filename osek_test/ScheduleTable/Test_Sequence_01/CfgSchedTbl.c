#include "Os.h"

static void vSchedTbl1_CmdEp0(void)
{
	(void)ActivateTask(vTask1);
}
static void vSchedTbl1_CmdEp1(void)
{
	(void)ActivateTask(vTask2);
}
static void vSchedTbl1_CmdEp2(void)
{
	(void)ActivateTask(vTask3);
}
static void vSchedTbl1_CmdEp3(void)
{
	(void)ActivateTask(vTask4);
}
static void vSchedTbl1_CmdEp4(void)
{
	(void)ActivateTask(vTask3);
}
static OsSchedTblCmdType g_vSchedTbl1[5]=
{
	{
		10,
		vSchedTbl1_CmdEp0
	},
	{
		20,
		vSchedTbl1_CmdEp1
	},
	{
		30,
		vSchedTbl1_CmdEp2
	},
	{
		40,
		vSchedTbl1_CmdEp3
	},
	{
		50,
		vSchedTbl1_CmdEp4
	},
};
static void vSchedTbl2_CmdEp0(void)
{
	(void)SetEvent(vTask3,vTask3Event2);
}
static void vSchedTbl2_CmdEp1(void)
{
	(void)SetEvent(vTask1,vTask1Event3);
}
static void vSchedTbl2_CmdEp2(void)
{
	(void)SetEvent(vTask1,vTask1Event3);
}
static OsSchedTblCmdType g_vSchedTbl2[3]=
{
	{
		10,
		vSchedTbl2_CmdEp0
	},
	{
		20,
		vSchedTbl2_CmdEp1
	},
	{
		30,
		vSchedTbl2_CmdEp2
	},
};
OsSchedTblCmdType* OSScheduleTable[cfgOS_SCHEDULE_TABLE_NUM]=
{
	g_vSchedTbl1,
	g_vSchedTbl2,
}
