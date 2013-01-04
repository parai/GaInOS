#include "Kernel.h"

#if(cfgOS_SCHEDULE_TABLE_NUM > 0)
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
}
static void vSchedTbl2_CmdEp1(void)
{
	(void)ActivateTask(vTask5);
}
static void vSchedTbl2_CmdEp2(void)
{
	(void)ActivateTask(vTask6);
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
		EXPLICIT		/* XOSSCHEDULETABLESYNCSTRATEGY */
	},
	{		/* VSCHEDTBL2 */
		850,		/* XOSSCHEDULETABLEDURATION */
		STD_FALSE,		/* XOSSCHEDULETABLEREPEATING */
		INVALID_OSAPPLICATION, 		/* XOSSCHTBLACCESSINGAPPLICATION */
		VCOUNTER1,		/* XOSSCHEDULETABLECOUNTERREF */
		STD_TRUE,		/* XOSSCHEDULETABLEAUTOSTART */
		ABSOLUTE,		/* XOSSCHEDULETABLEAUTOSTARTTYPE */
		100,		/* XOSSCHEDULETABLEAUTOSTARTVALUE */
		IMPLICIT		/* XOSSCHEDULETABLESYNCSTRATEGY */
	}
};
#ENDIF  /* CFGOS_SCHEDULE_TABLE_NUM */
