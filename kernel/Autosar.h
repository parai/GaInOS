#ifndef _AUTOSAR_H_
#define _AUTOSAR_H_
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
#endif

#endif /* _AUTOSAR_H_ */
