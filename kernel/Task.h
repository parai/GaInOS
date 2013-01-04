/* Copyright 2012, Fan Wang(Parai)
 *
 * This file is part of GaInOS.
 *
 * GaInOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *             
 * Linking GaInOS statically or dynamically with other modules is making a
 * combined work based on GaInOS. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of GaInOS give
 * you permission to combine GaInOS program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with GaInOS solely through the GaInOS defined interface. 
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for GaInOS and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of GaInOS are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 * 
 * GaInOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GaInOS. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _TASK_H_
#define _TASK_H_
/* |----------------+----------------------------------------------------------------------| */
/* | Task concept   |                                                                      | */
/* |----------------+----------------------------------------------------------------------| */
/* | Basic Tasks    | Basic tasks only release the processor, if                           | */
/* |                | (1)they terminate,                                                   | */
/* |                | (2)the OSEK operating system switches to a higher-priority task      | */
/* |                | (3)an interrupt occurs which causes the processor to switch to an    | */
/* |                | interrupt service routine (ISR).                                     | */
/* |----------------+----------------------------------------------------------------------| */
/* | Extended Tasks | Extended tasks are distinguished from basic tasks by being allowed   | */
/* |                | to use the operating system call WaitEvent, which may result in a    | */
/* |                | waiting state (see chapter 7, Event mechanism, and chapter 13.5.3.4, | */
/* |                | WaitEvent). The waiting state allows the processor to be released    | */
/* |                | and to be reassigned to a lower-priority task without the need to    | */
/* |                | terminate the running extended task.                                 | */
/* |----------------+----------------------------------------------------------------------| */

/* |------------------+------------------------------------------------------------------| */
/* | Task state model |                                                                  | */
/* |------------------+------------------------------------------------------------------| */
/* | running          | In the running state, the CPU is assigned to the task, so that   | */
/* |                  | its instructions can be executed. Only one task can be in this   | */
/* |                  | state at any point in time, while all the other states can be    | */
/* |                  | adopted simultaneously by several tasks.                         | */
/* |------------------+------------------------------------------------------------------| */
/* | ready            | All functional prerequisites for a transition into the running   | */
/* |                  | state exist, and the task only waits for allocation of the       | */
/* |                  | processor. The scheduler decides which ready task is executed    | */
/* |                  | next.                                                            | */
/* |------------------+------------------------------------------------------------------| */
/* | waiting          | A task cannot continue execution because it shall wait for at    | */
/* |                  | least one event (This state is only for extended tasks).         | */
/* |------------------+------------------------------------------------------------------| */
/* | suspended        | In the suspended state the task is passive and can be activated. | */
/* |------------------+------------------------------------------------------------------| */


/* |------------+-----------------------------------------------------------------------| */
/* | OSTaskClass| Conformance classes for Task                                          | */
/* |------------+-----------------------------------------------------------------------| */
/* | BCC1:      | only basic tasks, limited to one activation request per task and one  | */
/* |            | task per priority, while all tasks have different priorities          | */
/* |------------+-----------------------------------------------------------------------| */
/* | BCC2:      | like BCC1, plus more than one task per priority possible and multiple | */
/* |            | requesting of task activation allowed                                 | */
/* |------------+-----------------------------------------------------------------------| */
/* | ECC1       | like BCC1, plus extended tasks                                        | */
/* |------------+-----------------------------------------------------------------------| */
/* | ECC2       | like ECC1, plus more than one task per priority possible and multiple | */
/* |            | requesting of task activation allowed for basic tasks                 | */
/* |------------+-----------------------------------------------------------------------| */

/* |-------------------------------------------------------------| */
/* | Figure 3-3 The minimum requirements for Conformance Classes | */
/* |----------------------+---------------+------+------------------+--------------| */
/* |                      | BCC1          | BCC2 | ECC1             | ECC2         | */
/* |----------------------+---------------+------+------------------+--------------| */
/* | Multiple requesting  | no            | yes  | BT:no            | BT:yes       | */
/* | of task activation   |               |      | ET:no            | ET:no        | */
/* |----------------------+---------------+------+------------------+--------------| */
/* | Number of tasks      |                      | 16                              | */
/* | which are not in the | 8                    | (any combination of BT/ET)      | */
/* | suspended state      |                      |                                 | */
/* |----------------------+---------------+------+------------------+--------------| */
/* | More than one task   | no            | yes  | no               | yes          | */
/* | per priority         |               |      | (both BT/ET)     | (both BT/ET) | */
/* |----------------------+---------------+------+------------------+--------------| */
/* | Number of            |    ----              |                  8              | */
/* | events per task      |                      |                                 | */
/* |----------------------+----------------------+---------------------------------| */
/* | Number of task       |        8             |                  16             | */
/* | priorities           |                      |                                 | */
/* |----------------------+----------------------+---------------------------------| */
/* | Resources            | RES_SCHEDULER |  8 (including RES_SCHEDULER)           | */
/* |----------------------+---------------+----------------------------------------| */
/* | Internal resources   |             2                                          | */
/* |----------------------+--------------------------------------------------------| */
/* | Alarm                |             1                                          | */
/* |----------------------+--------------------------------------------------------| */
/* | Application Mode     |             1                                          | */
/* |----------------------+--------------------------------------------------------| */

#include "Os_cfg.h"

/* |----------------------------------------+---------| */
/* | Macro Function Table                   | Comment | */
/* |----------------------------------------+---------| */
/* | tableGetTskInitStkTop(xTaskID)         |         | */
/* |----------------------------------------+---------| */
/* | tableGetTskInitPrio(xTaskID)           |         | */
/* |----------------------------------------+---------| */
/* | listGetRdyTsk(xPriority)               |         | */
/* |----------------------------------------+---------| */
/* | tableGetTskState(xTaskID)              |         | */
/* |----------------------------------------+---------| */
/* | tableGetTskClsType(xTaskID)            |         | */
/* |----------------------------------------+---------| */
/* | tableGetTskMaxActCnt(xTaskID)          |         | */
/* |----------------------------------------+---------| */
/* | tableGetTskActCnt(xTaskID)             |         | */
/* |----------------------------------------+---------| */
/* | IsTskPreemptable(xTaskID)              |         | */
/* |----------------------------------------+---------| */
/* | tableGetTskTcb(xTaskID)                |         | */
/* |----------------------------------------+---------| */
/* | isTskAutoAcitvateTrue(xTaskID)         |         | */
/* |----------------------------------------+---------| */
/* | listTskRdyIsEmpty(xPriority)           |         | */
/* |----------------------------------------+---------| */
/* | setReadyTableMap(xPriority)            |         | */
/* |----------------------------------------+---------| */
/* | clearReadyTableMap(xPriority)          |         | */
/* |----------------------------------------+---------| */
/* | listInsertTskAtHead(xPriority,xTaskID) |         | */
/* |----------------------------------------+---------| */
/* | listInsertTskAtTail(xPriority,xTaskID) |         | */
/* |----------------------------------------+---------| */
/* | listRemoveTskAtHead(xPriority,xTaskID) |         | */
/* |----------------------------------------+---------| */


/* Macro tableGetTskInitStkTop() will Get The xTaskID's initial stack top 
   configured in OSTaskStackTable[] */
#define tableGetTskInitStkTop(xTaskID)          \
    (OSTaskStackTable[(xTaskID)].pxStackTop)
/* Macro tableGetTskInitStkTop() will Get The xTaskID's initial priority 
   configured in OSTaskInitPriorityTable[] */
#define tableGetTskInitPrio(xTaskID)            \
    (OSTaskInitPriorityTable[(xTaskID)])
/* Macro isTskAutoAcitvateTrue() will judge wether the xTaskID is Auto
   Activated or not*/
#if(cfgOS_SUM_ACTIVATES == 0 || cfgOS_MULTIPLY_ACTIVATE == STD_FALSE)   
/* listGetRdyTsk() will get a Task At head of xPriority rdy list,
   if xPriority has task(s) in ready or running state , the xTaskID will 
   be returned, or INVALID_TASK*/
#define listGetRdyTsk(xPriority)                \
    (OSTskRdyListHead[(xPriority)])
#endif  /* cfgOS_SUM_ACTIVATES || cfgOS_MULTIPLY_ACTIVATE */

/* Macro will get xTaskID's xState */
#define tableGetTskState(xTaskID)               \
    (OSTcbTable[(xTaskID)].xState)
/* Macro will get xTaskID's Conformance Class Type */
#define tableGetTskClsType(xTaskID)             \
    (OSTskClsTypeTable[(xTaskID)])

#if(cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
/* Macro will get xTaskID's current xActivateCount */
#define tableGetTskActCnt(xTaskID)              \
    (OSTcbTable[(xTaskID)].xActivateCount)
#define tableGetTskMaxActCnt(xTaskID)           \
    (OSMaxActivateCountTable[(xTaskID)])
#endif

#define tableGetTskTcb(xTaskID)                 \
    (&OSTcbTable[(xTaskID)])

#if(cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
/* Macro IsTskPreemptable() will judge wether the xTaskID was Preemptable or not */
#define IsTskPreemptable(xTaskID)                   \
    (STD_TRUE == OSTaskPreemtableTable[(xTaskID)])
#endif
#define isTskAutoAcitvateTrue(xTaskID)                  \
    ( OSTaskAutoActivateTable[(xTaskID)] == STD_TRUE )
/* Macro listTskRdyIsEmpty() will judge wether there is a task or several tasks 
   in state READY or RUNNING, with the xPriority*/
#if(cfgOS_SUM_ACTIVATES == 0 || cfgOS_MULTIPLY_ACTIVATE == STD_FALSE)
#define listTskRdyIsEmpty(xPriority)                \
    (OSTskRdyListHead[(xPriority)] == INVALID_TASK)
#else
#define listTskRdyIsEmpty(xPriority)                \
    (OSTskRdyListHead[(xPriority)] == OSTskRdyListTail[(xPriority)])
#endif

#define setReadyTableMap(xPriority)                                     \
    do{                                                                 \
        OSReadyGrp |= ( 1 << ((xPriority)>>3 ) );                       \
        OSReadyTable[(xPriority)>>3] |= ( 1 << ((xPriority)&0x07u) );   \
    }while(0)

#define clearReadyTableMap(xPriority)                                   \
    do{                                                                 \
        OSReadyTable[(xPriority)>>3] &= ~( 1 << ((xPriority)&0x07u) );  \
        if(0x00u == OSReadyTable[(xPriority)>>3])                       \
        {                                                               \
            OSReadyGrp &= ~( 1<<((xPriority)>>3) );                     \
        }                                                               \
    }while(0)

#if(cfgOS_SUM_ACTIVATES == 0 || cfgOS_MULTIPLY_ACTIVATE == STD_FALSE)
/* Macro listInsertTskAtHead() will insert a task to the ready list,
   when the list of xPriority is Empty */
#if(cfgOS_TASK_PER_PRIORITY == ONLY_ONE_TASK_PER_PRIORITY)
#define listInsertTskAtHead(xPriority,xTaskID)      \
    do{                                             \
        OSTskRdyListHead[(xPriority)]=(xTaskID);    \
    }while(0)
#elif(cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY)
#define listInsertTskAtHead(xPriority,xTaskID)                          \
    do{                                                                 \
        OSTskRdyListNext[(xTaskID)]  = OSTskRdyListHead[(xPriority)];   \
        OSTskRdyListHead[(xPriority)]=(xTaskID);                        \
    }while(0)
#endif
/* Macro listInsertTskAtTail() will inseart a task to the ready list.
   when OS was Configured in SEVERAL_TASKS_PER_PRIORITY mode,
   Then A task will be insert at the tail.
   In ONLY_ONE_TASK_PER_PRIORITY mode, It's the same with listInsertTskAtHead().*/
#if(cfgOS_TASK_PER_PRIORITY == ONLY_ONE_TASK_PER_PRIORITY)
#define listInsertTskAtTail(xPriority,xTaskID)      \
    do{                                             \
        OSTskRdyListHead[(xPriority)]=(xTaskID);    \
        setReadyTableMap((xPriority));              \
    }while(0)
#elif(cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY)
#define listInsertTskAtTail(xPriority,xTaskID)                          \
    do{                                                                 \
        if(listTskRdyIsEmpty(xPriority))                                \
        {                                                               \
            listInsertTskAtHead(xPriority,xTaskID);                     \
            setReadyTableMap((xPriority));                              \
            OSTskRdyListTail[(xPriority)]=(xTaskID);                    \
        }                                                               \
        else                                                            \
        {                                                               \
            OSTskRdyListNext[(xTaskID)]  =INVALID_TASK;                 \
            OSTskRdyListNext[ OSTskRdyListTail[(xPriority)] ] = xTaskID; \
            OSTskRdyListTail[(xPriority)]=(xTaskID);                    \
        }                                                               \
    }while(0)
#endif  
/* Macro listRemoveTskAtHead() will delete the fisrst element At rdy list
 */
#if(cfgOS_TASK_PER_PRIORITY == ONLY_ONE_TASK_PER_PRIORITY)
#define listRemoveTskAtHead(xPriority,xTaskID)      \
    do{                                             \
        OSTskRdyListHead[(xPriority)]=INVALID_TASK; \
        clearReadyTableMap((xPriority));            \
    }while(0)
#elif(cfgOS_TASK_PER_PRIORITY == SEVERAL_TASKS_PER_PRIORITY)
#define listRemoveTskAtHead(xPriority,xTaskID)                      \
    do{                                                             \
        OSTskRdyListHead[(xPriority)]=OSTskRdyListNext[(xTaskID)];  \
        if(listTskRdyIsEmpty((xPriority)))                          \
        {                                                           \
            clearReadyTableMap((xPriority));                        \
        }                                                           \
    }while(0)
#endif
#endif  /* cfgOS_SUM_ACTIVATES */

extern PriorityType tableRdyMapFindHighPriority(void);
extern BoolType try2ActivateTask( TaskType xTaskID );
extern void try2TerminateCurrentTask(void);
extern void doSchedule(void);

#if((cfgOS_SUM_ACTIVATES > 0) && (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE))
extern void listInsertTskAtHead(PriorityType xPriority,TaskType xTaskID);
extern void listInsertTskAtTail(PriorityType xPriority,TaskType xTaskID);
extern void listRemoveTskAtHead(PriorityType xPriority,TaskType xTaskID);
extern TaskType listGetRdyTsk(PriorityType xPriority);
#endif  /* cfgOS_SUM_ACTIVATES && cfgOS_MULTIPLY_ACTIVATE */

#endif /* _TASK_H_ */
