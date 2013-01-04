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

/* |--------------------------------------+----------------| */
/* | History:                             | Year-Month-Day | */
/* |--------------------------------------+----------------| */
/* | Creat this file and decide to Design |      2012-9-27 | */
/* | the AutoSAR OS.                      |                | */
/* |--------------------------------------+----------------| */


/* |-------------------+----------------------------------------------| */
/* | File-name:        | Alarm.h                                      | */
/* |-------------------+----------------------------------------------| */
/* | Reference Manual: | (1)Specification of Operating System(V3.0.2) | */
/* |                   | AUTOSAR_SWS_OS.PDF                           | */
/* |                   |                                              | */
/* |                   | (2)Operating SystemSpecification 2.2.3(OSEK) | */
/* |                   | os223.doc                                    | */
/* |-------------------+----------------------------------------------| */
#ifndef _ALARM_H_
#define _ALARM_H_

/* Get the owner of Alarm(xAlarmID), It's an Counter ID */
#define tableGetCounterID(xAlarmID) ( OSAlarmOwnerTable[(xAlarmID)] )

/* Get the Base info of xAlarmID, In fact the info belongs to xAlarmID's
 * Owner xCounterID.Its Type is AlarmBaseType. */
#define listGetAlarmBase(xAlarmID)                      \
    (OSCounterBaseTable[tableGetCounterID(xAlarmID)])

/* Get xMaxAllowedValue of  the xCounterID,It's Stored in type AlarmBaseType */
#define tableGetCntMaxAllowed(xCounterID)                   \
    ( OSCounterBaseTable[(xCounterID)].xMaxAllowedValue )

/*  Get xMinCycle of  the xCounterID,It's Stored in type AlarmBaseType*/
#define tableGetCntMinCycle(xCounterID)             \
    ( OSCounterBaseTable[(xCounterID)].xMinCycle )

/* Set The data area xCycle of xAlarmID,Container Type is  AlmCtrlBlkType */
#define tableSetAlmCycle(xAlarmID,xCyc)         \
    do{                                         \
        OSAlarmTable[(xAlarmID)].xCycle =xCyc;  \
    }while(0)

/* Set The data area xAlarmValue of xAlarmID,Container Type is  AlmCtrlBlkType */
#define tableSetAlmValue(xAlarmID,xAlmValue)                \
    do{                                                     \
        OSAlarmTable[(xAlarmID)].xAlarmValue = xAlmValue;   \
    }while(0)

/* Get The data area xCycle of xAlarmID,Container Type is  AlmCtrlBlkType */
#define tableGetAlmCycle(xAlarmID)              \
    ( OSAlarmTable[(xAlarmID)].xCycle )

/* Set The data area xAlmValue of xAlarmID,Container Type is  AlmCtrlBlkType */
#define tableGetAlmValue(xAlarmID)              \
    ( OSAlarmTable[(xAlarmID)].xAlarmValue )

/* Do the Increment of the xCounterID's Current Value by one  */
#define tableIncCntCurValue(xCounterID)                         \
    do{                                                         \
        OSCntCurValueTable[(xCounterID)] += 1;                  \
        if(tableGetCntMaxAllowed(xCounterID)                    \
           < OSCntCurValueTable[(xCounterID)])                  \
        {                                                       \
            OSCntCurValueTable[(xCounterID)] = 0;               \
        }                                                       \
    }while(0)

/* Get Current Value of xCounterID */
#define tableGetCntCurValue(xCounterID)         \
    (OSCntCurValueTable[(xCounterID)])

/* Do judge whether there is no xAlarmID in use which is assigned to xCounterID.
 * TRUE: NO xAlarmID assigned to xCounterID in use
 * FALSE: there is/are at least one xAlarmID assigned to xCounterID in use */
#define listCntIsEmpty(xCounterID)              \
    ( INVALID_ALARM == OSCntAlmList[(xCounterID)] )
/* Inseart the xAlarmID at head of the list of xCounterID,That's to say the 
 * xAlarmID will expire first */
#define listCntInsertAtHead(xCounterID , xAlarmID)                  \
    do{                                                             \
        OSAlarmTable[(xAlarmID)].pxNext=OSCntAlmList[(xCounterID)]; \
        OSAlarmTable[(xAlarmID)].pxPrev=INVALID_ALARM;              \
        if(!listCntIsEmpty(xCounterID))                             \
        {                                                           \
            OSAlarmTable[                                           \
                (OSCntAlmList[(xCounterID)])                        \
                ].pxPrev=xAlarmID;                                  \
        }                                                           \
        OSCntAlmList[(xCounterID)] = xAlarmID;                      \
    }while(0)

/* Remove the xAlarmID which is the head elements of xCounterID,
 * That's to say a cancle of xAlarmID */
#define listCntRemoveAtHead(xCounterID)                                 \
    do{                                                                 \
        if(INVALID_ALARM!=listGetCntNextElement(OSCntAlmList[(xCounterID)])) \
        {                                                               \
            listGetCntPrevElement(listGetCntNextElement(OSCntAlmList[(xCounterID)]))=INVALID_ALARM; \
        }                                                               \
        OSCntAlmList[(xCounterID)]=listGetCntNextElement(OSCntAlmList[(xCounterID)]); \
    }while(0)

/* Inseart xAlarmID at tail of the list of xCounterID */
#define listCntInsertAtTail(pxTail , xAlarmID)          \
     do{                                                \
         OSAlarmTable[(pxTail)].pxNext=(xAlarmID);      \
         OSAlarmTable[(xAlarmID)].pxNext=INVALID_ALARM; \
         OSAlarmTable[(xAlarmID)].pxPrev=pxTail;        \
     }while(0)
/* Remove the xAlarmID which is the hail elements of xCounterID,
 * That's to say a cancle of xAlarmID */
#define listCntRemoveAtTail(pxTail)                     \
    do{                                                 \
        OSAlarmTable[(pxTail)].pxNext=INVALID_ALARM;    \
    }while(0)
/* Inseart xAlarmID just after pPrev and before pNext */
#define listCntInsertBetween(pPrev,pNext,xAlarmID)  \
    do{                                             \
        OSAlarmTable[(pPrev)].pxNext =(xAlarmID);   \
        OSAlarmTable[(pNext)].pxPrev =(xAlarmID);   \
        OSAlarmTable[(xAlarmID)].pxNext=pNext;      \
        OSAlarmTable[(xAlarmID)].pxPrev=pPrev;      \
    }while(0)

/* Remove the Element(xAlarmID) whish is between pPrev and pNext */
#define listCntRemoveBetween(pPrev,pNext)       \
    do{                                         \
        OSAlarmTable[(pPrev)].pxNext =pNext;    \
        OSAlarmTable[(pNext)].pxPrev =pPrev;    \
    }while(0)

/* Get the Head Element of list of xCounterID */
#define listGetCntHeadElement(xCounterID)       \
    ( OSCntAlmList[(xCounterID)] )
/* Get the Previoue Element of pxAlmCtrlBlk in the list of xCounterID who owns pxAlmCtrlBlk */
#define listGetCntPrevElement(xAlarmID)         \
    ( OSAlarmTable[(xAlarmID)].pxPrev )
/* Get the Next Element of pxAlmCtrlBlk in the list of xCounterID who owns pxAlmCtrlBlk */
#define listGetCntNextElement(xAlarmID)         \
    ( OSAlarmTable[(xAlarmID)].pxNext )

#define IsAlmInUse(xAlarmID)                    \
    (STD_TRUE == OSAlmInUseTable[(xAlarmID)])

#define DoUseAlm(xAlarmID)                      \
    do{                                         \
        OSAlmInUseTable[(xAlarmID)]= STD_TRUE;  \
    }while(0)
#define DoCancleAlm(xAlarmID)                   \
    do{                                         \
    OSAlmInUseTable[(xAlarmID)] =STD_FALSE;     \
    }while(0)

#define tableGetAlmClsType(xAlarmID)            \
    (OSAlarmClassTable[(xAlarmID)])
#if(cfgOS_USE_ALARM == STD_TRUE)
void OSProcessAlarm( CounterType xCounterID ); 
#endif 
#endif /* _ALARM_H_ */
