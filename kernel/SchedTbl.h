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

/* |---------+-------------------| */
/* | Author: | Wang Fan(parai)   | */
/* |---------+-------------------| */
/* | Email:  | parai@foxmail.com | */
/* |---------+-------------------| */
/* | WorkOn: | Emacs23.3         | */
/* |---------+-------------------| */
#ifndef _SCHEDTBL_H_
#define _SCHEDTBL_H_
/* ================== OSSchedTblCtrlBlkTable ================================== */
/* Get the Iterator of the schedule table */
#define tableGetSchedTblIterator(xId)           \
    (OSSchedTblCtrlBlkTable[(xId)].xIterator)
/* Get The Status of The Schedule Table xId */
#define tableGetSchedTblStatus(xId)                 \
    (OSSchedTblCtrlBlkTable[(xId)].xSchedTblStatus)
/* Get The Starting time of The Schedule Table xId */
#define tableGetSchedTblStartingTime(xId)                   \
    (OSSchedTblCtrlBlkTable[(xId)].xSchedTblStartingTime)
/* Get The direction shoud do adjust */
#define tableGetSchedTblAdjustDerection(xId)            \
    (OSSchedTblCtrlBlkTable[(xId)].xAdjustDirection)
/* Get the deviation between driving counter and synchronization counter */
#define tableGetSchedTblDeviation(xId)          \
    (OSSchedTblCtrlBlkTable[(xId)].xDeviation)
/* Get Next schedule table whitch should be in state next */
#define tableGetSchedTblNextSchedTbl(xId)                       \
    (OSSchedTblCtrlBlkTable[(xId)].xNextOrPrevScheduleTable)
/* Get previous schedule table whitch should be in state running but
   has a schedule table in state next*/
#define tableGetSchedTblPrevSchedTbl(xId)                       \
    (OSSchedTblCtrlBlkTable[(xId)].xNextOrPrevScheduleTable)
/* Get the next absolute time of next expire point of schedule table */
#define tableGetSchedTblNextExpiryPointTime(xSchedTblID)            \
    (OSSchedTblCtrlBlkTable[(xSchedTblID)].xSchedTblNextExpiryPointTime)
/* Get current value of schedule,infact do calculate */
#define tableGetSchedTblCurValue(xStartingTime,xCntCurValue,xMaxAllowedValue) \
    ((xCntCurValue) > (xStartingTime)?((xCntCurValue)-(xStartingTime)): \
     ((xMaxAllowedValue)-(xStartingTime)+(xCntCurValue)))
#define tableGetSchedTblSyncStrategy(xId)                       \
    (OSScheduleTableInfo[(xId)].xOsScheduleTableSyncStrategy)  
/* Get The first element Schedule Table of the list counter xCounterID */
#define listGetSchedTblHeadElement(xCounterID)  \
    (OsCntSchedTblList[(xCounterID)])
/* Get The next schedule table */
#define listGetSchedTblNextElement(xSchedTblID) \
    (OSSchedTblCtrlBlkTable[(xSchedTblID)].pxNextSchedTbl)
/* Get the previous schedule table */
#define listGetSchedTblPrevElement(xSchedTblID) \
    (OSSchedTblCtrlBlkTable[(xSchedTblID)].pxPrevSchedTbl)
/* Insert the xSchedTblID at head of the counter list xCounterID*/
#define listInsertSchedTblAtHead(xCounterID,xSchedTblID)                \
    do{                                                                 \
        listGetSchedTblNextElement(xSchedTblID)=listGetSchedTblHeadElement(xCounterID); \
        listGetSchedTblPrevElement(xSchedTblID)=INVALID_SCHEDULE_TABLE; \
        if(listGetSchedTblHeadElement(xCounterID)!=INVALID_SCHEDULE_TABLE) \
        {                                                               \
            listGetSchedTblPrevElement(listGetSchedTblHeadElement(xCounterID))=(xSchedTblID); \
        }                                                               \
        listGetSchedTblHeadElement(xCounterID)=(xSchedTblID);           \
    }while(0)
/* Remove the Schedule table from the counter list  */
#define listRemoveSchedTbl(xCounterID,xSchedTblID)                      \
    do{                                                                 \
        if(listGetSchedTblPrevElement((xSchedTblID))==INVALID_SCHEDULE_TABLE) \
        {     /* Remove it at Head */                                   \
            if(listGetSchedTblNextElement((xSchedTblID))!=INVALID_SCHEDULE_TABLE) \
            {                                                           \
                listGetSchedTblPrevElement(listGetSchedTblNextElement((xSchedTblID)))= \
                    INVALID_SCHEDULE_TABLE;                             \
            }                                                           \
            listGetSchedTblHeadElement((xCounterID))=listGetSchedTblNextElement((xSchedTblID)); \
        }                                                               \
        else if(listGetSchedTblNextElement((xSchedTblID))==INVALID_SCHEDULE_TABLE) \
        {    /* Remove it at Tail */                                    \
            listGetSchedTblNextElement(listGetSchedTblPrevElement((xSchedTblID)))= \
                INVALID_SCHEDULE_TABLE;                                 \
        }                                                               \
        else    /* Remove it between prev and next */                   \
        {                                                               \
            listGetSchedTblPrevElement(listGetSchedTblNextElement((xSchedTblID)))= \
                listGetSchedTblPrevElement((xSchedTblID));              \
            listGetSchedTblNextElement(listGetSchedTblPrevElement((xSchedTblID)))= \
                listGetSchedTblNextElement((xSchedTblID));              \
        }                                                               \
    }while(0)
/* ============================== OSScheduleTable ==================================== */
/* Get the initial offset value of the schedule table xId */
#define tableGetSchedTblInitialOffset(xId)      \
    (OSScheduleTable[(xId)][0].xOffset)
/* Get The synchronous strategy */
/* Get the offset ticks of the schedule table at the iterator */
#define tableGetSchedTblOffset(xSchedTblID,xIterator)       \
    (OSScheduleTable[(xSchedTblID)][(xIterator)].xOffset)
/* Do the action of the schedule table */
#define tableDoSchedTblAction(xSchedTblID,xIterator)                    \
    do{                                                                 \
        (OSScheduleTable[(xSchedTblID)][(xIterator)].pxOsSchedTblCmd)(); \
    }while(0)
/* ============================= OSScheduleTableInfo ================================= */
/* Do judge whether the schedule is repeatable */
#define IsSchedTbleRepeatable(xSchedTblID)                              \
    (OSScheduleTableInfo[(xSchedTblID)].xOsScheduleTableRepeating==STD_TRUE)
/* Get the counter used to drive this schedule table <xId> */
#define tableGetSchedTblDrivingCounter(xId)     \
    (OSScheduleTableInfo[(xId)].xOsScheduleTableCounterRef)

#define IsSchedTbleAutostartable(xSchedTblID)                           \
     (OSScheduleTableInfo[(xSchedTblID)].xOsScheduleTableAutostartable==STD_TRUE)

#define tableGetSchedTblAutostartType(xSchedTblID)                      \
    (OSScheduleTableInfo[(xSchedTblID)].xOsScheduleTableAutostartType)

#define tableGetSchedTblAutostartValue(xSchedTblID)                     \
    (OSScheduleTableInfo[(xSchedTblID)].xOsScheduleTableAutostartValue)

#define tableGetSchedTblDuration(xSchedTblID)                       \
    (OSScheduleTableInfo[(xSchedTblID)].xOsScheduleTableDuration)

#define tableGetSchedTblMaxAdvance(xSchedTblID)  \
    (OSScheduleTableInfo[(xSchedTblID)].xOsScheduleTableMaxAdvance)

#define tableGetSchedTblMaxRetart(xSchedTblID)  \
    (OSScheduleTableInfo[(xSchedTblID)].xOsScheduleTableMaxRetard)

#define tableGetSchedTblPrecision(xSchedTblID)  \
    (OSScheduleTableInfo[(xSchedTblID)].xOsScheduleTableExplicitPrecision)

#if(cfgOS_SCHEDULE_TABLE_NUM>0)
void OSProcessScheduleTableFinalDelay(ScheduleTableType xSchedTblID);
void OSMakeNextExpiryPointReady(ScheduleTableType ScheduleTableID);
void OSProcessScheduleTable(CounterType xCounterID);
#endif


#endif /* _SCHEDTBL_H_ */
