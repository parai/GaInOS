#include "Kernel.h"

#if(cfgOS_SCHEDULE_TABLE_NUM > 0)
static void vSchedTbl1_CmdEp0(void)
{
	(void)ActivateTask(vTask1);
	OSMakeNextExpiryPointReady(vSchedTbl1);
}
static void vSchedTbl1_CmdEp1(void)
{
	(void)ActivateTask(vTask2);
	OSMakeNextExpiryPointReady(vSchedTbl1);
}
static void vSchedTbl1_CmdEp2(void)
{
	(void)ActivateTask(vTask3);
	OSMakeNextExpiryPointReady(vSchedTbl1);
}
static void vSchedTbl1_CmdEp3(void)
{
	OSProcessScheduleTableFinalDelay(vSchedTbl1);
}
static const OsSchedTblCmdType g_vSchedTbl1[4]=
{
	{
		250,
		vSchedTbl1_CmdEp0
	},
	{
		450,
		vSchedTbl1_CmdEp1
	},
	{
		650,
		vSchedTbl1_CmdEp2
	},
	{
		800,
		vSchedTbl1_CmdEp3
	}
};
static void vSchedTbl2_CmdEp0(void)
{
	(void)ActivateTask(vTask4);
	OSMakeNextExpiryPointReady(vSchedTbl2);
}
static void vSchedTbl2_CmdEp1(void)
{
	(void)ActivateTask(vTask5);
	OSMakeNextExpiryPointReady(vSchedTbl2);
}
static void vSchedTbl2_CmdEp2(void)
{
	(void)ActivateTask(vTask6);
	OSMakeNextExpiryPointReady(vSchedTbl2);
}
static void vSchedTbl2_CmdEp3(void)
{
	OSProcessScheduleTableFinalDelay(vSchedTbl2);
}
static const OsSchedTblCmdType g_vSchedTbl2[4]=
{
	{
		300,
		vSchedTbl2_CmdEp0
	},
	{
		500,
		vSchedTbl2_CmdEp1
	},
	{
		700,
		vSchedTbl2_CmdEp2
	},
	{
		850,
		vSchedTbl2_CmdEp3
	}
};
const OsSchedTblCmdType* OSScheduleTable[cfgOS_SCHEDULE_TABLE_NUM]=
{
	g_vSchedTbl1,
	g_vSchedTbl2
};
const OsScheduleTableType OSScheduleTableInfo[cfgOS_SCHEDULE_TABLE_NUM]=
{
	{		/* vSchedTbl1 */
		800,		/* xOsScheduleTableDuration */
		STD_TRUE,		/* xOsScheduleTableRepeating */
		INVALID_OSAPPLICATION, 		/* xOsSchTblAccessingApplication */
		vCounter1,		/* xOsScheduleTableCounterRef */
		STD_TRUE,		/* xOsScheduleTableAutostart */
		RELATIVE,		/* xOsScheduleTableAutostartType */
		50,		/* xOsScheduleTableAutostartValue */
		EXPLICIT,		/* xOsScheduleTableSyncStrategy */
		5,		/* xOsScheduleTableMaxAdvance */
		10,		/* xOsScheduleTableMaxRetard */
		1		/* xOsScheduleTableExplicitPrecision */
	},
	{		/* vSchedTbl2 */
		850,		/* xOsScheduleTableDuration */
		STD_TRUE,		/* xOsScheduleTableRepeating */
		INVALID_OSAPPLICATION, 		/* xOsSchTblAccessingApplication */
		vCounter1,		/* xOsScheduleTableCounterRef */
		STD_TRUE,		/* xOsScheduleTableAutostart */
		ABSOLUTE,		/* xOsScheduleTableAutostartType */
		100,		/* xOsScheduleTableAutostartValue */
		EXPLICIT,		/* xOsScheduleTableSyncStrategy */
		1,		/* xOsScheduleTableMaxAdvance */
		4,		/* xOsScheduleTableMaxRetard */
		2		/* xOsScheduleTableExplicitPrecision */
	}
};
#endif  /* cfgOS_SCHEDULE_TABLE_NUM */
