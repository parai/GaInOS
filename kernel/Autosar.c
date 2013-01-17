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

/* |-------------------+----------------------------------------------| */
/* | File-name:        | Autosar.c                                    | */
/* |-------------------+----------------------------------------------| */
/* | Reference Manual: | (1)Specification of Operating System(V3.0.2) | */
/* |                   | AUTOSAR_SWS_OS.PDF                           | */
/* |                   |                                              | */
/* |                   | (2)Operating SystemSpecification 2.2.3(OSEK) | */
/* |                   | os223.doc                                    | */
/* |-------------------+----------------------------------------------| */
#include "Kernel.h"

/* Calculate the relative starting time */
static vDoAddSchedTblTick(TickType xCntCurValue,TickType xMaxAllowedValue ,TickType xTicks)
{
    if( xTicks < (xMaxAllowedValue - xCntCurValue) )
    {
        return (xCntCurValue + xTicks);
    }
    else
    {
        return (xCntCurValue + xTicks -xMaxAllowedValue);
    }
}
/* This is a special function will be called on the final delay of the schedule table*/
/* OS427: The Operating System shall allow the Final Delay for a single-shot schedule
   table to be zero.
   OS427:now is not supported,the final dalay cann't be zero.*/
void OSProcessScheduleTableFinalDelay(ScheduleTableType xSchedTblID)
{
    CounterType xCounterID;
    TickType xMaxAllowedValue;
    ScheduleTableType ScheduleTableID;
    
    if(IsSchedTbleRepeatable(xSchedTblID))
    {
        /* Reset Its control block and restart */
        tableGetSchedTblIterator(xSchedTblID)=0;
        tableGetSchedTblStartingTime(xSchedTblID)=tableGetCntCurValue(tableGetSchedTblDrivingCounter(xSchedTblID));
        OSMakeNextExpiryPointReady(xSchedTblID);
    }
    else 
    {
        (void)StopScheduleTable(xSchedTblID);
        ScheduleTableID = tableGetSchedTblNextSchedTbl(xSchedTblID);
        if(INVALID_SCHEDULE_TABLE != ScheduleTableID)
        {
            /* OS505: If OsScheduleTblSyncStrategy  of the schedule tables 
               <ScheduleTableID_From>  and <ScheduleTableID_To>   in a call of 
               NextScheduleTable() is  EXPLICIT  and the Operating System module  already 
               synchronizes <ScheduleTableID_From>,  NextScheduleTable() shall continue  
               synchonization after the start of processing <ScheduleTableID_To>.  */
            xCounterID=tableGetSchedTblDrivingCounter(ScheduleTableID);
            xMaxAllowedValue=tableGetCntMaxAllowed(xCounterID);
            listInsertSchedTblAtHead(xCounterID,ScheduleTableID);
            tableGetSchedTblStartingTime(ScheduleTableID)=tableGetCntCurValue(xCounterID);
            tableGetSchedTblIterator(ScheduleTableID)=0;
            tableGetSchedTblNextExpiryPointTime(ScheduleTableID)=vDoAddSchedTblTick( 
                tableGetSchedTblStartingTime(ScheduleTableID), /* starting time */
                xMaxAllowedValue,
                tableGetSchedTblOffset(ScheduleTableID,0)); /* relative offset to starting time */
            tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING; 
            tableGetSchedTblNextSchedTbl(ScheduleTableID)=INVALID_SCHEDULE_TABLE;
        }
    }
}

/* do adjust according synchronization strategy,or just reprepare schedule table's
   starting and next expiry point time.*/
void OSMakeNextExpiryPointReady(ScheduleTableType ScheduleTableID)
{
    /* When called,already in critial section */
    uint8_t xIterator;
    CounterType xCounterID;
    TickType xMaxAllowedValue;
    TickType xTmpTime1;
    TickType xTmpTime2;
    TickType xTmpTime3;
    xCounterID = tableGetSchedTblDrivingCounter(ScheduleTableID);
    xMaxAllowedValue = tableGetCntMaxAllowed(xCounterID);
    xIterator = tableGetSchedTblIterator(ScheduleTableID);
    xTmpTime1 = tableGetSchedTblStartingTime(ScheduleTableID);

    tableGetSchedTblNextExpiryPointTime(ScheduleTableID)=vDoAddSchedTblTick( 
        xTmpTime1,
        xMaxAllowedValue,
        tableGetSchedTblOffset(ScheduleTableID,xIterator));

    xTmpTime3=tableGetSchedTblDeviation(ScheduleTableID); /* xTmpTime3 = deviation */
    /* Should do an adjust */
    if(0u!=xTmpTime3)   
    {         
        if(SCHEDTBL_RETARD == tableGetSchedTblAdjustDerection(ScheduleTableID))
        {
            xTmpTime2=tableGetSchedTblMaxRetart(ScheduleTableID);
            /* adjust value for next expiry point is max(maxRetard,deviation) */
            /* xTmpTime2=vMax(xTmpTime2,xTmpTime3); */
            if(xTmpTime2 < xTmpTime3)
            {   
                /* Adjust value is maxRetard */
                /* Deviation remained which should be processed in ISR */
                xTmpTime3=xTmpTime3-xTmpTime2; 
            }
            else
            {
                /* Adjust value is the whole deviation */
                xTmpTime2=xTmpTime3;
                /* Deviation left which should be processed in ISR  is ZERO*/
                xTmpTime3=0u;
            }
            /* Now set status */
            if(xTmpTime3 <= tableGetSchedTblPrecision(ScheduleTableID))
            {
                tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
            }
            else
            {
                tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING;
            }
            /* Adjust Its starting time */            
            if(xTmpTime1 > xTmpTime2)
            {                   /* xStartingTime is bigger than adjust value */
                tableGetSchedTblStartingTime(ScheduleTableID) = xTmpTime1 - xTmpTime2;
            }
            else
            {
                tableGetSchedTblStartingTime(ScheduleTableID) = xMaxAllowedValue-xTmpTime1 + xTmpTime2; 
            }
            xTmpTime1 = tableGetSchedTblNextExpiryPointTime(ScheduleTableID);
            /* Adjust Its next expiry point time */
            if(xTmpTime1 > xTmpTime2)
            {                   /* xNextExpiryPointTime is bigger than adjust value */
                tableGetSchedTblNextExpiryPointTime(ScheduleTableID) = xTmpTime1 - xTmpTime2;
            }
            else
            {
                tableGetSchedTblNextExpiryPointTime(ScheduleTableID) = xMaxAllowedValue-xTmpTime1 + xTmpTime2; 
            }
            tableGetSchedTblDeviation(ScheduleTableID)=xTmpTime3;
        }
        else /* if(SCHEDTBL_ADVANCED == tableGetSchedTblAdjustDerection(ScheduleTableID)) */
        {
            xTmpTime2=tableGetSchedTblMaxAdvance(ScheduleTableID);
            /* adjust value for next expiry point is max(maxAdvance,deviation) */
            /* xTmpTime2=vMax(xTmpTime2,xTmpTime3); */
            if(xTmpTime2 < xTmpTime3)
            {   
                /* Adjust value is maxAdvance */
                /* Deviation remained which should be processed in ISR */
                xTmpTime3=xTmpTime3-xTmpTime2; 
            }
            else
            {
                /* Adjust value is the whole deviation */
                xTmpTime2=xTmpTime3;
                /* Deviation left which should be processed in ISR  is ZERO*/
                xTmpTime3=0u;
            }
            /* Now set status */
            if(xTmpTime3 <= tableGetSchedTblPrecision(ScheduleTableID))
            {
                tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
            }
            else
            {
                tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING;
            }
            /* Adjust Its starting time */            
            if(xTmpTime2 < (xMaxAllowedValue - xTmpTime1))
            {                  
                tableGetSchedTblStartingTime(ScheduleTableID) = xTmpTime2 + xTmpTime1;
            }
            else
            {
                xTmpTime1 = xMaxAllowedValue-xTmpTime1;
                tableGetSchedTblStartingTime(ScheduleTableID) = xTmpTime2 - xTmpTime1; 
            }
            xTmpTime1 = tableGetSchedTblNextExpiryPointTime(ScheduleTableID);
            /* Adjust Its next expiry point time */
            if(xTmpTime2 < (xMaxAllowedValue - xTmpTime1))
            {                  
                tableGetSchedTblNextExpiryPointTime(ScheduleTableID) = xTmpTime2 + xTmpTime1;
            }
            else
            {
                xTmpTime1 = xMaxAllowedValue-xTmpTime1;
                tableGetSchedTblNextExpiryPointTime(ScheduleTableID) = xTmpTime2 - xTmpTime1; 
            }
            tableGetSchedTblDeviation(ScheduleTableID)=xTmpTime3;
        }
    }
}
static void OSProcessScheduleTable(CounterType xCounterID)
{
    /* only the schedule tables in running state should be put into the list. */
    ScheduleTableType xSchedTblID;
    OS_ENTER_CRITICAL();
    xSchedTblID = listGetSchedTblHeadElement(xCounterID);
    while(xSchedTblID != INVALID_SCHEDULE_TABLE)
    {
        if(tableGetCntCurValue(xCounterID)==tableGetSchedTblNextExpiryPointTime(xSchedTblID))
        {
            tableGetSchedTblIterator(xSchedTblID)+=1;
            /* Do an Action */
            tableDoSchedTblAction(xSchedTblID,tableGetSchedTblIterator(xSchedTblID)-1);
            /* Bug Here.
             if Action is OSProcessScheduleTableFinalDelay() and the xSchedTblID has been 
             stopped by that Action. So The Call OSMakeNextExpiryPointReady() has no meaning.
             So Should Combine it with Action.*/
            /* OSMakeNextExpiryPointReady(xSchedTblID); */
         }
        xSchedTblID=listGetSchedTblNextElement(xSchedTblID);
    }
    OS_EXIT_CRITICAL();
}
/* |-------------------+------------------------------------------------------------| */
/* | Service name:     | GetApplicationID                                           | */
/* |-------------------+------------------------------------------------------------| */
/* | Syntax:           | ApplicationType GetApplicationID (void)                    | */
/* | Service ID:       | OSServiceId_GetApplicationID                               | */
/* | Sync/Async:       | Sync                                                       | */
/* | Reentrancy:       | Yes                                                        | */
/* | Parameters (in):  | None                                                       | */
/* | Parameters (out): | None                                                       | */
/* |-------------------+------------------------------------------------------------| */
/* | Return value:     | <Identifier of running OS-Application> or                  | */
/* |                   | INVALID_OSAPPLICATION                                      | */
/* |-------------------+------------------------------------------------------------| */
/* | Description:      | OS261: GetApplicationID() shall return the application     | */
/* |                   | identifier to which the executing Task/OsIsr/hook belongs. | */
/* |                   | OS262: If no OS-Application is running, GetApplicationID() | */
/* |                   | shall return INVALID_OSAPPLICATION.                        | */
/* |-------------------+------------------------------------------------------------| */
/* | Caveats:          | None                                                       | */
/* | Configuration:    | Available in Scalability Classes 3 and 4                   | */
/* |-------------------+------------------------------------------------------------| */
ApplicationType GetApplicationID(void)  
{
    return INVALID_OSAPPLICATION;
}

/* |-------------------+------------------------------------------------------------| */
/* | Service name:     | GetISRID                                                   | */
/* |-------------------+------------------------------------------------------------| */
/* | Syntax:           | ISRType GetISRID (void)                                    | */
/* | Service ID:       | OSServiceId_GetISRID                                       | */
/* | Sync/Async:       | Sync                                                       | */
/* | Reentrancy:       | Yes                                                        | */
/* | Parameters (in):  | None                                                       | */
/* | Parameters (out): | None                                                       | */
/* |-------------------+------------------------------------------------------------| */
/* | Return value:     | <Identifier of running OsIsr> OR                           | */
/* |                   | INVALID_ISR                                                | */
/* |-------------------+------------------------------------------------------------| */
/* | Description:      | OS263: If called from category 2 OsIsr (or Hook routines   | */
/* |                   | called inside a category 2 OsIsr), GetISRID() shall return | */
/* |                   | the identifier of the currently executing OsIsr.           | */
/* |                   | OS264: If its caller is not a category 2 OsIsr (or Hook    | */
/* |                   | routines called inside a category 2 OsIsr), GetISRID()     | */
/* |                   | shall return INVALID_ISR.                                  | */
/* |-------------------+------------------------------------------------------------| */
/* | Caveats:          | None                                                       | */
/* | Configuration:    | Available in all Scalability Classes.                      | */
/* |-------------------+------------------------------------------------------------| */
ISRType GetISRID (void) 
{
    return INVALID_ISR;
}

/* |-------------------+------------------------------------------------------------------------| */
/* | Service name:     | CallTrustedFunction                                                    | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Syntax:           | StatusType CallTrustedFunction                                         | */
/* |                   | (                                                                      | */
/* |                   | TrustedFunctionIndexType FunctionIndex,                                | */
/* |                   | TrustedFunctionParameterRefType FunctionParams                         | */
/* |                   | )                                                                      | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Service ID:       | OSServiceId_CallTrustedFunction                                        | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Sync/Async:       | Depends on called function. If called function                         | */
/* |                   | is synchronous then service is synchronous.                            | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Reentrancy:       | May cause rescheduling. Yes                                            | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Parameters (in):  | FunctionIndex: Index of the function to be called.                     | */
/* |                   | FunctionParams or NULL: Pointer to the parameters                      | */
/* |                   | for the function ¨C specified by the FunctionIndex                      | */
/* |                   | - to be called. If no parameters are provided,                         | */
/* |                   | a NULL pointer has to be passed.                                       | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Parameters (out): | None                                                                   | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Return value:     | E_OK: No Error                                                         | */
/* |                   | E_OS_SERVICEID:No function defined for this index                      | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Description:      | OS265: If <FunctionIndex> is a defined function index,                 | */
/* |                   | CallTrustedFunction() shall switch the processor into                  | */
/* |                   | privileged mode AND shall call the function <FunctionIndex>            | */
/* |                   | out of a list of implementation specific trusted functions             | */
/* |                   | AND shall return E_OK after completion.                                | */
/* |                   | OS312: The called trusted function must conform to the                 | */
/* |                   | following C prototype: void TRUSTED_<name_of_the_trusted_service>      | */
/* |                   | ( TrustedFunctionIndexType,TrustedFunctionParameterRefType);           | */
/* |                   | (The arguments are the same as the arguments of CallTrustedFunction).  | */
/* |                   | OS266: When the function <FunctionIndex> is called, it shall get       | */
/* |                   | the same permissions (access rights) as the associated trusted         | */
/* |                   | OS-Application.                                                        | */
/* |                   | OS364: If the trusted function is called from a Category 2 OsIsr       | */
/* |                   | context it shall continue to run on the same interrupt priority        | */
/* |                   | and be allowed to call all system services defined for Category        | */
/* |                   | 2 OsIsr (see table in chapter 7.7.3.2).                                | */
/* |                   | OS365: If the trusted function is called from a task context it shall  | */
/* |                   | continue to run on the same priority and be allowed to call all system | */
/* |                   | services defined for tasks (see table in chapter 7.7.3.2).             | */
/* |                   | OS292: If the function index <FunctionIndex> is undefined,             | */
/* |                   | CallTrustedFunction() shall return E_OS_SERVICEID.                     | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Caveats:          | Normally, a user will not directly call this service, but it will      | */
/* |                   | be part of some standard interface, e.g. a standard I/O interface.     | */
/* |                   | It is the duty of the called trusted function to check rights of       | */
/* |                   | passed parameters, especially if parameters are interpreted as out     | */
/* |                   | parameters.                                                            | */
/* |-------------------+------------------------------------------------------------------------| */
/* | Configuration:    | Available in Scalability Classes 3 and 4                               | */
/* |-------------------+------------------------------------------------------------------------| */
StatusType CallTrustedFunction(TrustedFunctionIndexType FunctionIndex,
                               TrustedFunctionParameterRefType FunctionParams) 
{
    return E_OK;
}

/* |-------------------+------------------------------------------------------------| */
/* | Service name:     | CheckISRMemoryAccess                                       | */
/* |-------------------+------------------------------------------------------------| */
/* | Syntax:           | AccessType CheckISRMemoryAccess(                           | */
/* |                   | ISRType ISRID,                                             | */
/* |                   | MemoryStartAddressType Address,                            | */
/* |                   | MemorySizeType  Size)                                      | */
/* |-------------------+------------------------------------------------------------| */
/* | Service ID:       | OSServiceId_CheckISRMemoryAccess                           | */
/* | Sync/Async:       | Sync                                                       | */
/* | Reentrancy:       | Yes                                                        | */
/* |-------------------+------------------------------------------------------------| */
/* | Parameters (in):  | ISRID    OsIsr reference                                   | */
/* |                   | Address  Start of memory area                              | */
/* |                   | Size     Size of memory area                               | */
/* |-------------------+------------------------------------------------------------| */
/* | Parameters (out): | None                                                       | */
/* |-------------------+------------------------------------------------------------| */
/* | Return value:     | Value which contains the access rights to the memory area. | */
/* |-------------------+------------------------------------------------------------| */
/* | Description:      | OS267: If the OsIsr reference <ISRID> is valid,            | */
/* |                   | CheckISRMemoryAccess() shall return the access rights of   | */
/* |                   | the OsIsr on the specified memory area.                    | */
/* |                   | OS313: If an access right (e.g. ¡°read¡±) is not valid for   | */
/* |                   | the whole specified memory area CheckISRMemoryAccess()     | */
/* |                   | shall yield no access regarding this right.                | */
/* |                   | OS268: If the OsIsr reference <ISRID> is not valid,        | */
/* |                   | CheckISRMemoryAccess() shall yield no access rights.       | */
/* |-------------------+------------------------------------------------------------| */
/* | Caveats:          | None                                                       | */
/* | Configuration:    | Available in Scalability Classes 3 and 4                   | */
/* |-------------------+------------------------------------------------------------| */
AccessType CheckISRMemoryAccess(TaskType TaskID,
                                MemoryStartAddressType Address,MemorySizeType Size)
{
    return 0;
}

/* |-------------------+------------------------------------------------------------| */
/* | Service name:     | CheckTaskMemoryAccess                                      | */
/* |-------------------+------------------------------------------------------------| */
/* | Syntax:           | AccessType CheckTaskMemoryAccess                           | */
/* |                   | (TaskType TaskID,                                          | */
/* |                   | MemoryStartAddressType Address,                            | */
/* |                   | MemorySizeType Size)                                       | */
/* |-------------------+------------------------------------------------------------| */
/* | Service ID:       | OSServiceId_CheckTaskMemoryAccess                          | */
/* | Sync/Async:       | Sync                                                       | */
/* | Reentrancy:       | Yes                                                        | */
/* |-------------------+------------------------------------------------------------| */
/* | Parameters (in):  | TaskID  Task reference                                     | */
/* |                   | Address Start of memory area                               | */
/* |                   | Size    Size of memory areaNone                            | */
/* |-------------------+------------------------------------------------------------| */
/* | Parameters (out): | None                                                       | */
/* |-------------------+------------------------------------------------------------| */
/* | Return value:     | Value which contains the access rights to the memory area. | */
/* |-------------------+------------------------------------------------------------| */
/* | Description:      | OS269: If the Task reference <TaskID> is valid,            | */
/* |                   | CheckTaskMemoryAccess() shall return the access rights     | */
/* |                   | of the task on the specified memory area.                  | */
/* |                   | OS314: If an access right (e.g. ¡°read¡±) is not valid for   | */
/* |                   | the whole specified memory area CheckTaskMemoryAccess()    | */
/* |                   | shall yield no access regarding this right.                | */
/* |                   | OS270: If the Task reference <TaskID> is not valid,        | */
/* |                   | CheckTaskMemoryAccess() shall yield no access rights.      | */
/* |-------------------+------------------------------------------------------------| */
/* | Caveats:          | None                                                       | */
/* | Configuration:    | Available in Scalability Classes 3 and 4                   | */
/* |-------------------+------------------------------------------------------------| */
AccessType CheckTaskMemoryAccess(TaskType TaskID,
                                 MemoryStartAddressType Address,MemorySizeType Size)
{
    return 0;
}

/* |-------------------+-------------------------------------------------------| */
/* | Service name:     | CheckObjectAccess                                     | */
/* |-------------------+-------------------------------------------------------| */
/* | Syntax:           | ObjectAccessType CheckObjectAccess                    | */
/* |                   | (ApplicationType ApplID,                              | */
/* |                   | ObjectTypeType ObjectType,                            | */
/* |                   | ...)                                                  | */
/* |-------------------+-------------------------------------------------------| */
/* | Service ID:       | OSServiceId_CheckObjectAccess                         | */
/* | Sync/Async:       | Sync                                                  | */
/* | Reentrancy:       | Yes                                                   | */
/* | Parameters (in):  | ApplID  OS-Application identifier                     | */
/* |                   | ObjectType Type of the following parameter            | */
/* |                   | ...  The object to be examined                        | */
/* | Parameters (out): | None                                                  | */
/* |-------------------+-------------------------------------------------------| */
/* | Return value:     | ACCESS    if the ApplID has access to the object      | */
/* |                   | NO_ACCESS otherwise                                   | */
/* |-------------------+-------------------------------------------------------| */
/* | Description:      | OS271: If the OS-Application <ApplID> has access      | */
/* |                   | to the queried object, CheckObjectAccess() shall      | */
/* |                   | return ACCESS.                                        | */
/* |                   | OS272: If the OS-Application <ApplID> has no          | */
/* |                   | access to the queried object, CheckObjectAccess()     | */
/* |                   | shall return NO_ACCESS.                               | */
/* |                   | OS423: If the object to be examined is not a valid    | */
/* |                   | object OR <ApplID> is invalid OR <ObjectType> is      | */
/* |                   | invalid THEN the the CheckObjectAccess() shall return | */
/* |                   | NO_ACCESS.                                            | */
/* |                   | OS318: If the object type is OBJECT_RESOURCE AND the  | */
/* |                   | object to be examined is the RES_SCHEDULER            | */
/* |                   | CheckObjectAccess() shall always return ACCESS.       | */
/* |-------------------+-------------------------------------------------------| */
/* | Caveats:          | None                                                  | */
/* | Configuration:    | Available in Scalability Classes 3 and 4              | */
/* |-------------------+-------------------------------------------------------| */
ObjectAccessType CheckObjectAccess(ApplicationType ApplID,
                                   ObjectTypeType ObjectType,void* pxObject)
{
			return NO_ACCESS;
}
/* |-------------------+-------------------------------------------------------------------| */
/* | Service name:     | CheckObjectOwnership                                              | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Syntax:           | ApplicationType CheckObjectOwnership                              | */
/* |                   | (ObjectTypeType ObjectType,                                       | */
/* |                   | ...)                                                              | */
/* | Service ID:       | OSServiceId_CheckObjectOwnership                                  | */
/* | Sync/Async:       | Sync                                                              | */
/* | Reentrancy:       | Yes                                                               | */
/* | Parameters (in):  | ObjectType  Type of the following parameter                       | */
/* |                   | ...  The object to be examined                                    | */
/* | Parameters (out): | None                                                              | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Return value:     | <OS-Application>:  The service returns the OS-Application         | */
/* |                   | to which the object ObjectType belongs or                         | */
/* |                   | INVALID_OSAPPLICATION:If the object does not exists the           | */
/* |                   | service returns                                                   | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Description:      | OS273: If the specified object ObjectType exists,                 | */
/* |                   | CheckObjectOwnership() shall return the identifier of the         | */
/* |                   | OS-Application to which the object belongs.                       | */
/* |                   | OS274: If the specified object ObjectType is invalid OR           | */
/* |                   | the argument of the type (the ¡°...¡±) is invalid ,                 | */
/* |                   | CheckObjectOwnership() shall return INVALID_OSAPPLICATION.        | */
/* |                   | OS319: If the object to be examined is the RES_SCHEDULER          | */
/* |                   | CheckObjectOwnership() shall always return INVALID_OSAPPLICATION. | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Caveats:          | None                                                              | */
/* | Configuration:    | Available in Scalability Classes 3 and 4                          | */
/* |-------------------+-------------------------------------------------------------------| */
ApplicationType CheckObjectOwnership(ObjectTypeType ObjectType,void* pxObject)
{
			return INVALID_OSAPPLICATION;
}
/* |-------------------+---------------------------------------------------------------| */
/* | Service name:     | StartScheduleTableRel                                         | */
/* |-------------------+---------------------------------------------------------------| */
/* | Syntax:           | StatusType StartScheduleTableRel                              | */
/* |                   | (ScheduleTableType ScheduleTableID,                           | */
/* |                   | TickType Offset)                                              | */
/* |-------------------+---------------------------------------------------------------| */
/* | Service ID:       | OSServiceId_StartScheduleTableRel                             | */
/* | Sync/Async:       | Sync                                                          | */
/* | Reentrancy:       | Yes                                                           | */
/* | Parameters (in):  | ScheduleTableID:Schedule table to be started                  | */
/* |                   | Offset:Number of ticks on the counter before the the schedule | */
/* |                   | table processing is started                                   | */
/* | Parameters (out): | None                                                          | */
/* |-------------------+---------------------------------------------------------------| */
/* | Return value:     | E_OK:No error                                                 | */
/* |                   | E_OS_ID(only in extended status):ScheduleTableID not valid.   | */
/* |                   | E_OS_VALUE(only in extended status):Offset is greater than    | */
/* |                   | (OsCounterMaxAllowedValue InitialOffset) or is equal to 0.    | */
/* |                   | E_OS_STATE:Schedule table was already started.                | */
/* |-------------------+---------------------------------------------------------------| */
/* | Description:      | OS275: If the schedule table <ScheduleTableID> is not valid,  | */
/* |                   | StartScheduleTableRel() shall return E_OS_ID.                 | */
/* |                   | OS452: If the schedule table <ScheduleTableID> is             | */
/* |                   | implicitely synchronized (OsScheduleTblSyncStrategy =         | */
/* |                   | IMPLICIT), StartScheduleTableRel() shall return E_OS_ID.      | */
/* |                   | OS332: If <Offset> is zero StartScheduleTableRel() shall      | */
/* |                   | return E_OS_VALUE.                                            | */
/* |                   | OS276: If the offset <Offset> is greater than                 | */
/* |                   | OsCounterMaxAllowedValue of the underlying counter minus      | */
/* |                   | the Initial Offset, StartScheduleTableRel() shall return      | */
/* |                   | E_OS_VALUE.                                                   | */
/* |                   | OS277: If the schedule table <ScheduleTableID> is not in the  | */
/* |                   | state SCHEDULETABLE_STOPPED, StartScheduleTableRel() shall    | */
/* |                   | return E_OS_STATE.                                            | */
/* |                   | OS278: If its input parameters are valid and the state of     | */
/* |                   | schedule table <ScheduleTableID> is SCHEDULETABLE_STOPPED,    | */
/* |                   | then StartScheduleTableRel() shall start the processing of a  | */
/* |                   | schedule table <ScheduleTableID>. The Initial Expiry Point    | */
/* |                   | shall be processed after <Offset> + Initial Offset ticks      | */
/* |                   | have elapsed on the underlying counter. The state of          | */
/* |                   | <ScheduleTableID> to SCHEDULETABLE_RUNNING.                   | */
/* |-------------------+---------------------------------------------------------------| */
/* | Caveats:          | None                                                          | */
/* | Configuration:    | Available in all Scalability Classes.                         | */
/* |-------------------+---------------------------------------------------------------| */
StatusType StartScheduleTableRel(ScheduleTableType ScheduleTableID,
                                 TickType Offset)
{
    StatusType xRet=E_OK;
    CounterType xCounterID=INVALID_COUNTER;
    TickType xMaxAllowedValue =0u;

    xCounterID=tableGetSchedTblDrivingCounter(ScheduleTableID);
    xMaxAllowedValue=tableGetCntMaxAllowed(xCounterID);
#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
    if(ScheduleTableID > (cfgOS_SCHEDULE_TABLE_NUM-1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    if(IMPLICIT == tableGetSchedTblSyncStrategy(ScheduleTableID))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    /* Subtract the initial offset */
    xTemp=xMaxAllowedValue-tableGetSchedTblInitialOffset(ScheduleTableID);
    if((Offset > xTemp)||(0u==Offset))
    {
        xRet = E_OS_VALUE;
        goto Error_Exit;
    }
#endif
    if(SCHEDULETABLE_STOPPED != tableGetSchedTblStatus(ScheduleTableID))
    {
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
    OS_ENTER_CRITICAL();
    listInsertSchedTblAtHead(xCounterID,ScheduleTableID);
    tableGetSchedTblStartingTime(ScheduleTableID)=vDoAddSchedTblTick(   
        tableGetCntCurValue(xCounterID), /* time now */
        xMaxAllowedValue,
        Offset);                /* relative offset to time now */
    tableGetSchedTblIterator(ScheduleTableID)=0;
    tableGetSchedTblNextExpiryPointTime(ScheduleTableID)=vDoAddSchedTblTick( 
        tableGetSchedTblStartingTime(ScheduleTableID), /* starting time */
        xMaxAllowedValue,
        tableGetSchedTblOffset(ScheduleTableID,0)); /* relative offset to starting time */
    tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING;
    /* Do not judge its sync-strategy EXPLICIT or IMPLICIT */
    tableGetSchedTblDeviation(ScheduleTableID)=0;
    tableGetSchedTblNextSchedTbl(ScheduleTableID)=INVALID_SCHEDULE_TABLE;
    OS_EXIT_CRITICAL();
    
  Error_Exit:
    return xRet;
}

/* |-------------------+----------------------------------------------------------------| */
/* | Service name:     | StartScheduleTableAbs                                          | */
/* |-------------------+----------------------------------------------------------------| */
/* | Syntax:           | StatusType StartScheduleTableAbs                               | */
/* |                   | (ScheduleTableType ScheduleTableID,                            | */
/* |                   | TickType Start)                                                | */
/* | Service ID:       | OSServiceId_StartScheduleTableAbs                              | */
/* | Sync/Async:       | Sync                                                           | */
/* | Reentrancy:       | Yes                                                            | */
/* | Parameters (in):  | ScheduleTableID:Schedule table to be started                   | */
/* |                   | Start:Absolute counter tick value at which the                 | */
/* |                   | schedule table is started.                                     | */
/* | Parameters (out): | None                                                           | */
/* |-------------------+----------------------------------------------------------------| */
/* | Return value:     | E_OK:No error                                                  | */
/* |                   | E_OS_ID(only in extended status):ScheduleTableID not valid.    | */
/* |                   | E_OS_VALUE(only in extended status):Tickvalue is greater       | */
/* |                   | than OsCounterMaxAllowedValue.                                 | */
/* |                   | E_OS_STATE:Schedule table was already started.                 | */
/* |-------------------+----------------------------------------------------------------| */
/* | Description:      | OS348: If the schedule table <ScheduleTableID> is not valid,   | */
/* |                   | StartScheduleTableAbs() shall return E_OS_ID.                  | */
/* |                   | OS349: If the <Tickvalue> is greater than the                  | */
/* |                   | OsCounterMaxAllowedValue of the underlying counter,            | */
/* |                   | StartScheduleTableAbs() shall return E_OS_VALUE.               | */
/* |                   | OS350: If the schedule table <ScheduleTableID> is not in the   | */
/* |                   | state SCHEDULETABLE_STOPPED, StartScheduleTableAbs() shall     | */
/* |                   | return E_OS_STATE.                                             | */
/* |                   | OS351: If its input parameters are valid and <ScheduleTableID> | */
/* |                   | is in the state SCHEDULETABLE_STOPPED, StartScheduleTableAbs() | */
/* |                   | shall start the processing of schedule table <ScheduleTableID> | */
/* |                   | at count value <Start> and shall set the state of              | */
/* |                   | <ScheduleTableID> to SCHEDULETABLE_RUNNING. The Initial Expiry | */
/* |                   | Point will be processed when the underlying counter equals     | */
/* |                   | <Start>+Initial Offset.                                        | */
/* |-------------------+----------------------------------------------------------------| */
/* | Caveats:          | None                                                           | */
/* | Configuration:    | Available in all Scalability Classes.                          | */
/* |-------------------+----------------------------------------------------------------| */
StatusType StartScheduleTableAbs(ScheduleTableType ScheduleTableID,
                                 TickType Start)
{
    StatusType xRet=E_OK;
    CounterType xCounterID=INVALID_COUNTER;

    xCounterID=tableGetSchedTblDrivingCounter(ScheduleTableID);
#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
    if(ScheduleTableID > (cfgOS_SCHEDULE_TABLE_NUM-1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    if(Start > tableGetCntMaxAllowed(xCounterID))
    {
        xRet = E_OS_VALUE;
        goto Error_Exit;
    }
#endif
    if(SCHEDULETABLE_STOPPED != tableGetSchedTblStatus(ScheduleTableID))
    {
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
    OS_ENTER_CRITICAL();
    listInsertSchedTblAtHead(xCounterID,ScheduleTableID);
    tableGetSchedTblStartingTime(ScheduleTableID)=Start;
    tableGetSchedTblIterator(ScheduleTableID)=0;
    tableGetSchedTblNextExpiryPointTime(ScheduleTableID)=vDoAddSchedTblTick( 
        Start,                  /* absolute starting time */
        tableGetCntMaxAllowed(xCounterID),
        tableGetSchedTblOffset(ScheduleTableID,0)); /* relative offset to absolute starting time */
    tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING;
    tableGetSchedTblDeviation(ScheduleTableID)=0;
    tableGetSchedTblNextSchedTbl(ScheduleTableID)=INVALID_SCHEDULE_TABLE;
    OS_EXIT_CRITICAL();
    
  Error_Exit:
    return xRet;
}
/* |-------------------+---------------------------------------------------------------| */
/* | Service name:     | StopScheduleTable                                             | */
/* |-------------------+---------------------------------------------------------------| */
/* | Syntax:           | StatusType StopScheduleTable                                  | */
/* |                   | (ScheduleTableType ScheduleTableID)                           | */
/* | Service ID:       | OSServiceId_StopScheduleTable                                 | */
/* | Sync/Async:       | Sync                                                          | */
/* | Reentrancy:       | Yes                                                           | */
/* | Parameters (in):  | ScheduleTableID:Schedule table to be stopped None             | */
/* | Parameters (out): | None                                                          | */
/* |-------------------+---------------------------------------------------------------| */
/* | Return value:     | E_OK:no error                                                 | */
/* |                   | E_OS_ID(only in EXTENDED status):ScheduleTableID not valid.   | */
/* |                   | E_OS_NOFUNC:Schedule table was already stopped                | */
/* |-------------------+---------------------------------------------------------------| */
/* | Description:      | OS279: If the schedule table identifier <ScheduleTableID>     | */
/* |                   | is not valid, StopScheduleTable() shall return E_OS_ID.       | */
/* |                   | OS280: If the schedule table with identifier                  | */
/* |                   | <ScheduleTableID> is in state SCHEDULETABLE_STOPPED,          | */
/* |                   | StopScheduleTable() shall return E_OS_NOFUNC.                 | */
/* |                   | OS281: If its input parameters are valid, StopScheduleTable() | */
/* |                   | shall set the state of <ScheduleTableID> to                   | */
/* |                   | SCHEDULETABLE_STOPPED and (stop the schedule table            | */
/* |                   | <ScheduleTableID> from processing any further expiry points   | */
/* |                   | and) shall return E_OK.                                       | */
/* |-------------------+---------------------------------------------------------------| */
/* | Caveats:          | None                                                          | */
/* | Configuration:    | Available in all Scalability Classes.                         | */
/* |-------------------+---------------------------------------------------------------| */
StatusType StopScheduleTable(ScheduleTableType ScheduleTableID)
{
    StatusType xRet=E_OK;
    CounterType xCounterID=INVALID_COUNTER;

#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
    if(ScheduleTableID > (cfgOS_SCHEDULE_TABLE_NUM-1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif

    if(SCHEDULETABLE_STOPPED == tableGetSchedTblStatus(ScheduleTableID))
    {
        xRet = E_OS_NOFUNC;
        goto Error_Exit;
    }
    xCounterID=tableGetSchedTblDrivingCounter(ScheduleTableID);
    OS_ENTER_CRITICAL();
    if(SCHEDULETABLE_NEXT == tableGetSchedTblStatus(ScheduleTableID))
    {
        tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_STOPPED;
        /* Stop Its Prev Schedule table which in state running or waiting if it is valid */
        ScheduleTableID = tableGetSchedTblPrevSchedTbl(ScheduleTableID);
        if(INVALID_SCHEDULE_TABLE != ScheduleTableID)
        {
            tableGetSchedTblNextSchedTbl(ScheduleTableID)=INVALID_SCHEDULE_TABLE;
        }
    }
    else
    {
        if(SCHEDULETABLE_WAITING != tableGetSchedTblStatus(ScheduleTableID))
        {
            listRemoveSchedTbl(xCounterID,ScheduleTableID);
        }
        tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_STOPPED;
        /* Stop Its Next Schedule table which in state next if it is valid */
        ScheduleTableID = tableGetSchedTblNextSchedTbl(ScheduleTableID);
        if(INVALID_SCHEDULE_TABLE != ScheduleTableID)
        {
            tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_STOPPED;
        }
    }
    OS_EXIT_CRITICAL();
    
  Error_Exit:
    return xRet;
}
/* |-------------------+-----------------------------------------------------------------| */
/* | Service name:     | NextScheduleTable                                               | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Syntax:           | StatusType NextScheduleTable                                    | */
/* |                   | (ScheduleTableType ScheduleTableID_From,                        | */
/* |                   | ScheduleTableType ScheduleTableID_To)                           | */
/* | Service ID:       | OSServiceId_NextScheduleTable                                   | */
/* | Sync/Async:       | Sync                                                            | */
/* | Reentrancy:       | Yes                                                             | */
/* | Parameters (in):  | ScheduleTableID_From:Schedule table                             | */
/* |                   | ScheduleTableID_To:Schedule table that provides its             | */
/* |                   | series of expiry points                                         | */
/* | Parameters (out): | None                                                            | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Return value:     | E_OK:No error                                                   | */
/* |                   | E_OS_ID (only in EXTENDED status):ScheduleTableID_From or       | */
/* |                   | ScheduleTableID_To not valid.                                   | */
/* |                   | E_OS_NOFUNC:ScheduleTableID_From not started.                   | */
/* |                   | E_OS_STATE (only in EXTENDED status):ScheduleTableID_To         | */
/* |                   | is started or next.                                             | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Description:      | OS282: If the input parameter <ScheduleTableID_From> or         | */
/* |                   | <ScheduleTableID_To> is not valid, NextScheduleTable()          | */
/* |                   | shall return E_OS_ID.                                           | */
/* |                   | OS330: If schedule table <ScheduleTableID_To> is driven         | */
/* |                   | by different counter than schedule table                        | */
/* |                   | <ScheduleTableID_From> then NextScheduleTable() shall           | */
/* |                   | return an error E_OS_ID.                                        | */
/* |                   | OS283: If the schedule table <ScheduleTableID_From> is          | */
/* |                   | in state SCHEDULETABLE_STOPPED OR in state                      | */
/* |                   | SCHEDULETABLE_NEXT, NextScheduleTable() shall leave the         | */
/* |                   | state of <ScheduleTable_From> and <ScheduleTable_To>            | */
/* |                   | unchanged and return E_OS_NOFUNC..                              | */
/* |                   | OS309: If the schedule table <ScheduleTableID_To> is not        | */
/* |                   | in state SCHEDULETABLE_STOPPED, NextScheduleTable()             | */
/* |                   | shall leave the state of <ScheduleTable_From> and               | */
/* |                   | <ScheduleTable_To> unchanged and return E_OS_STATE.             | */
/* |                   | OS284: If the input parameters are valid then                   | */
/* |                   | NextScheduleTable() shall start the processing of schedule      | */
/* |                   | table <ScheduleTableID_To> <ScheduleTableID_From>.              | */
/* |                   | FinalDelay ticks after the Final Expiry Point on                | */
/* |                   | <ScheduleTableID_From> is processed and shall return            | */
/* |                   | E_OK. The Initial Expiry Point on <ScheduleTableID_To>          | */
/* |                   | shall be processed at <ScheduleTableID_From>.Final Delay        | */
/* |                   | + <ScheduleTable_To>.Initial Offset ticks after the Final       | */
/* |                   | Expiry Point on <ScheduleTableID_From> is processed .           | */
/* |                   | OS324: If the input parameters are valid AND the                | */
/* |                   | <ScheduleTableID_From> already has a ¡°next¡± schedule          | */
/* |                   | table then the <ScheduleTableID_To> shall replace the           | */
/* |                   | previous ¡°next¡± schedule table and the old ¡°next¡± schedule  | */
/* |                   | table state becomes SCHEDULETABLE_STOPPED.                      | */
/* |                   | OS363: The synchronization strategy of the <ScheduleTableID_To> | */
/* |                   | shall come into effect when the Operating System processes the  | */
/* |                   | first expiry point of <ScheduleTableID_To>.                     | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Caveats:          | OS453: If the <ScheduleTableID_From> is stopped, the ¡°next¡±   | */
/* |                   | schedule table does not start and its state changes to          | */
/* |                   | SCHEDULETABLE_STOPPED.                                          | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Configuration:    | Available in all Scalability Classes.                           | */
/* |-------------------+-----------------------------------------------------------------| */
StatusType NextScheduleTable(ScheduleTableType ScheduleTableID_From,
                             ScheduleTableType ScheduleTableID_To)
{
    StatusType xRet=E_OK;

#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
    if((ScheduleTableID_From > (cfgOS_SCHEDULE_TABLE_NUM-1)) ||
       (ScheduleTableID_To > (cfgOS_SCHEDULE_TABLE_NUM-1)))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    if(tableGetSchedTblDrivingCounter(ScheduleTableID_From)!=   \
       tableGetSchedTblDrivingCounter(ScheduleTableID_To))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    /* OS484: If  OsScheduleTblSyncStrategy  of <ScheduleTableID_To>  in a call of 
       NextScheduleTable() is not equal to the  OsScheduleTblSyncStrategy  of 
       <ScheduleTableID_From>  then  NextScheduleTable() shall return  E_OS_ID. */
    if(tableGetSchedTblSyncStrategy(ScheduleTableID_From)!=   \
       tableGetSchedTblSyncStrategy(ScheduleTableID_To))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    if((SCHEDULETABLE_STOPPED == tableGetSchedTblStatus(ScheduleTableID_From))||
       (SCHEDULETABLE_NEXT    == tableGetSchedTblStatus(ScheduleTableID_From)))
    {
        xRet = E_OS_NOFUNC;
        goto Error_Exit;
    }
    if((SCHEDULETABLE_STOPPED != tableGetSchedTblStatus(ScheduleTableID_To)))
    {
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
    OS_ENTER_CRITICAL();
    if(INVALID_SCHEDULE_TABLE != tableGetSchedTblNextSchedTbl(ScheduleTableID_From))
    {
        tableGetSchedTblStatus(tableGetSchedTblNextSchedTbl(ScheduleTableID_From)) \
            =SCHEDULETABLE_STOPPED;
    }
    tableGetSchedTblNextSchedTbl(ScheduleTableID_From)= ScheduleTableID_To;
    tableGetSchedTblPrevSchedTbl(ScheduleTableID_To)  = ScheduleTableID_From;
    tableGetSchedTblStatus(ScheduleTableID_To) = SCHEDULETABLE_NEXT;
    OS_EXIT_CRITICAL();
  Error_Exit:
    return xRet;
}


/* |-------------------+---------------------------------------------------------| */
/* | Service name:     | StartScheduleTableSynchron                              | */
/* |-------------------+---------------------------------------------------------| */
/* | Syntax:           | StatusType StartScheduleTableSynchron                   | */
/* |                   | (ScheduleTableType ScheduleTableID)                     | */
/* | Sync/Async:       | Sync                                                    | */
/* | Reentrancy:       | Yes                                                     | */
/* | Parameters (in):  | ScheduleTableID:Schedule table to be started            | */
/* | Parameters (out): | None                                                    | */
/* |-------------------+---------------------------------------------------------| */
/* | Return value:     | E_OK:No Error                                           | */
/* |                   | E_OS_ID (only in EXTENDED status):ScheduleTableID       | */
/* |                   | not valid.                                              | */
/* |                   | E_OS_STATE:Schedule table was already started.          | */
/* |-------------------+---------------------------------------------------------| */
/* | Description:      | OS387: If the schedule table <ScheduleTableID> is       | */
/* |                   | not valid OR the schedule table <ScheduleTableID>       | */
/* |                   | is not explicitly synchronized                          | */
/* |                   | (OsScheduleTblSyncStrategy = EXPLICIT)                  | */
/* |                   | StartScheduleTableSynchron() shall return E_OS_ID.      | */
/* |                   | OS388: If the schedule table <ScheduleTableID> is       | */
/* |                   | not in the state SCHEDULETABLE_STOPPED, the service     | */
/* |                   | shall return E_OS_STATE.                                | */
/* |                   | OS389: If <ScheduleTableID> is valid,                   | */
/* |                   | StartScheduleTableSynchron() shall set the state of     | */
/* |                   | <ScheduleTableID> to SCHEDULETABLE_WAITING and start    | */
/* |                   | the processing of schedule table <ScheduleTableID>      | */
/* |                   | after the synchronization count of the schedule table   | */
/* |                   | is set via SyncScheduleTable(). The Initial Expiry      | */
/* |                   | Point shall processed when (Duration-SyncValue)+        | */
/* |                   | InitialOffset ticks have elapsed on the synchronization | */
/* |                   | counter where:                                          | */
/* |                   | ` Duration is <ScheduleTableID>.OsScheduleTableDuration | */
/* |                   | ` SyncValue is the <Value> parameter passed to the      | */
/* |                   | SyncScheduleTable()                                     | */
/* |                   | ` InitialOffset is the shortest expiry point offset in  | */
/* |                   | <ScheduleTableID>                                       | */
/* |-------------------+---------------------------------------------------------| */
/* | Caveats:          | None                                                    | */
/* | Configuration:    | Available in Scalability Classes 2 and 4                | */
/* |-------------------+---------------------------------------------------------| */
StatusType StartScheduleTableSynchron(ScheduleTableType ScheduleTableID)
{
    StatusType xRet = E_OK;
#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
    if(ScheduleTableID > (cfgOS_SCHEDULE_TABLE_NUM-1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }

    if(EXPLICIT != tableGetSchedTblSyncStrategy(ScheduleTableID))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    if(SCHEDULETABLE_STOPPED != tableGetSchedTblStatus(ScheduleTableID))
    {
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
    tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_WAITING;
  Error_Exit:
    return xRet;
}

/* |-------------------+--------------------------------------------------------------| */
/* | Service name:     | SyncScheduleTable                                            | */
/* |-------------------+--------------------------------------------------------------| */
/* | Syntax:           | StatusType SyncScheduleTable                                 | */
/* |                   | (ScheduleTableType ScheduleTableID,                          | */
/* |                   | TickType Value)                                              | */
/* | Service ID:       | OSServiceId_SyncScheduleTable                                | */
/* | Sync/Async:       | Sync                                                         | */
/* | Reentrancy:       | Yes                                                          | */
/* | Parameters (in):  | ScheduleTableID:Schedule table                               | */
/* |                   | Value:The current value of the synchronization counter.      | */
/* | Parameters (out): | None                                                         | */
/* |-------------------+--------------------------------------------------------------| */
/* | Return value:     | E_OK:No errors                                               | */
/* |                   | E_OS_ID (only in EXTENDED status):The ScheduleTableID was    | */
/* |                   | not valid or schedule table can not be synchronized          | */
/* |                   | (OsScheduleTblSyncStrategy not set or                        | */
/* |                   | OsScheduleTblSyncStrategy = IMPLICIT).                       | */
/* |                   | E_OS_VALUE (only in EXETENDED status):The <Value> is out     | */
/* |                   | of range.                                                    | */
/* |                   | E_OS_STATE (only in EXTENDED status):The state of            | */
/* |                   | schedule table <ScheduleTableID> is equal to                 | */
/* |                   | SCHEDULETABLE_STOPPED.                                       | */
/* |-------------------+--------------------------------------------------------------| */
/* | Description:      | OS454: If the <ScheduleTableID> is not valid OR schedule     | */
/* |                   | table can not be explicitely synchronized                    | */
/* |                   | (OsScheduleTblSyncStrategy is not equal to EXPLICIT)         | */
/* |                   | the service shall return E_OS_ID.                            | */
/* |                   | OS455: If the <Value> is greater than the                    | */
/* |                   | OsScheduleTableDuration, SyncScheduleTableAbs() shall        | */
/* |                   | return E_OS_VALUE.                                           | */
/* |                   | OS456: If the state of the schedule table <ScheduleTableID>  | */
/* |                   | is equal to SCHEDULETABLE_STOPPED or SCHEDULETABLE_NEXT the  | */
/* |                   | service shall return E_OS_STATE.                             | */
/* |                   | OS457: If the parameters are valid, the service provides the | */
/* |                   | operating system with the current synchronization count for  | */
/* |                   | the given schedule table. (It is used to synchronize the     | */
/* |                   | processing of the schedule table to the synchronization      | */
/* |                   | counter.)                                                    | */
/* |-------------------+--------------------------------------------------------------| */
/* | Caveats:          | None                                                         | */
/* | Configuration:    | Available in Scalability Classes 2 and 4                     | */
/* |-------------------+--------------------------------------------------------------| */
StatusType SyncScheduleTable(ScheduleTableType ScheduleTableID,TickType Value)
{
    StatusType xRet = E_OK;
    CounterType xCounterID;
    TickType xMaxAllowedValue;
    TickType xTmpTime1;
    TickType xTmpTime2;
    TickType xTmpTime3;
#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
    if(ScheduleTableID > (cfgOS_SCHEDULE_TABLE_NUM-1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }

    if(EXPLICIT != tableGetSchedTblSyncStrategy(ScheduleTableID))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
    if(Value > tableGetSchedTblDuration(ScheduleTableID))
    {
        xRet = E_OS_VALUE;
        goto Error_Exit;
    }
    if((SCHEDULETABLE_STOPPED == tableGetSchedTblStatus(ScheduleTableID))||
       (SCHEDULETABLE_NEXT    == tableGetSchedTblStatus(ScheduleTableID)))
    {
        xRet = E_OS_STATE;
        goto Error_Exit;
    }
#endif
    xCounterID = tableGetSchedTblDrivingCounter(ScheduleTableID);
    xMaxAllowedValue = tableGetCntMaxAllowed(xCounterID);
    OS_ENTER_CRITICAL();
    if(SCHEDULETABLE_WAITING == tableGetSchedTblStatus(ScheduleTableID))
    {                           /* start it */
		listInsertSchedTblAtHead(xCounterID,ScheduleTableID);
        /* relative start it at the offset (duration-value) */
		tableGetSchedTblStartingTime(ScheduleTableID)=vDoAddSchedTblTick( 
			tableGetCntCurValue(xCounterID),
			xMaxAllowedValue,
			tableGetSchedTblDuration(ScheduleTableID)-Value);
		tableGetSchedTblIterator(ScheduleTableID)=0;
		tableGetSchedTblNextExpiryPointTime(ScheduleTableID)=vDoAddSchedTblTick( 
			tableGetSchedTblStartingTime(ScheduleTableID),
			xMaxAllowedValue,
			tableGetSchedTblOffset(ScheduleTableID,0));
        /* Set status */
		tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
        tableGetSchedTblDeviation(ScheduleTableID)=0;
    }
    else                        /* running state,perform Synchronization */
    {
        /* Fisrt Get Current value of Schedule Table */
        xTmpTime1 = tableGetSchedTblStartingTime(ScheduleTableID);
        xTmpTime2 = tableGetCntCurValue(xCounterID);
        xTmpTime3 = tableGetSchedTblCurValue(xTmpTime1,xTmpTime2,xMaxAllowedValue);
        /* Calculate the deviation */
        if(Value > xTmpTime3)
        {
            xTmpTime3=Value-xTmpTime3; /* xTmpTime3 = deviation */
            /* Now set status */
            if(xTmpTime3 <= tableGetSchedTblPrecision(ScheduleTableID))
            {
                tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
            }
            else
            {
                tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING;
            }
            tableGetSchedTblDeviation(ScheduleTableID)=xTmpTime3;
            tableGetSchedTblAdjustDerection(ScheduleTableID)=SCHEDTBL_RETARD;
        }
        else if(Value < xTmpTime3)
        {
            xTmpTime3=xTmpTime3 - Value; /* xTmpTime3 = Deviation */
            /* Now set status */
            if(xTmpTime3 <= tableGetSchedTblPrecision(ScheduleTableID))
            {
                tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
            }
            else
            {
                tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING;
            }
            tableGetSchedTblDeviation(ScheduleTableID)=xTmpTime3;
            tableGetSchedTblAdjustDerection(ScheduleTableID)=SCHEDTBL_ADVANCED;
        }
        else
        {
            tableGetSchedTblStatus(ScheduleTableID)=SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS;
        }
    }
    OS_EXIT_CRITICAL();
#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
  Error_Exit:
#endif
    return xRet;
}

/* |-------------------+----------------------------------------------------------------| */
/* | Service name:     | SetScheduleTableAsync                                          | */
/* |-------------------+----------------------------------------------------------------| */
/* | Syntax:           | StatusType SetScheduleTableAsync(                              | */
/* |                   | ScheduleTableType ScheduleTableID)                             | */
/* | Service ID:       | OSServiceId_SetScheduleTableAsync                              | */
/* | Sync/Async:       | Sync                                                           | */
/* | Reentrancy:       | Yes                                                            | */
/* | Parameters (in):  | ScheduleTableID:Name of schedule for which status is requested | */
/* | Parameters (out): | None                                                           | */
/* |-------------------+----------------------------------------------------------------| */
/* | Return value:     | E_OK:no error                                                  | */
/* |                   | E_OS_ID (only in EXTENDED status)):Invalid ScheduleTableID     | */
/* |-------------------+----------------------------------------------------------------| */
/* | Description:      | OS300: If OsScheduleTblSyncStrategy of <ScheduleTableID>       | */
/* |                   | equals to EXPLICIT then SetScheduleTableAsync() shall set      | */
/* |                   | the status of <ScheduleTableID> to SCHEDULETABLE_RUNNING.      | */
/* |                   | OS362: If SetScheduleTableAsync() is called for a running      | */
/* |                   | schedule table the OS shall stop further synchronization       | */
/* |                   | until a SyncScheduleTable() call is made.                      | */
/* |                   | OS323: If SetScheduleTableAsync() is called for a running      | */
/* |                   | schedule table the OS shall continue to process expiry         | */
/* |                   | points on the schedule table.                                  | */
/* |                   | OS458: If OsScheduleTblSyncStrategy of <ScheduleTableID> is    | */
/* |                   | not equal to EXPLICIT then the service call shall return       | */
/* |                   | E_OS_ID                                                        | */
/* |-------------------+----------------------------------------------------------------| */
/* | Caveats:          | None                                                           | */
/* | Configuration:    | Available in Scalability Classes 2 and 4                       | */
/* |-------------------+----------------------------------------------------------------| */
StatusType SetScheduleTableAsync(ScheduleTableType ScheduleTableID)
{
    StatusType xRet = E_OK;
#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
    if(ScheduleTableID > (cfgOS_SCHEDULE_TABLE_NUM-1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }

    if(EXPLICIT != tableGetSchedTblSyncStrategy(ScheduleTableID))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    OS_ENTER_CRITICAL();
    if(SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS == tableGetSchedTblStatus(ScheduleTableID))
    { 
        tableGetSchedTblDeviation(ScheduleTableID)=0;
    }
    OS_EXIT_CRITICAL();
#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
  Error_Exit:
#endif
		return xRet;
}

/* |-------------------+-------------------------------------------------------------------| */
/* | Service name:     | GetScheduleTableStatus                                            | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Syntax:           | StatusType GetScheduleTableStatus                                 | */
/* |                   | (ScheduleTableType ScheduleTableID,                               | */
/* |                   | ScheduleTableStatusRefType ScheduleStatus)                        | */
/* | Service ID:       | OSServiceId_GetScheduleTableStatus                                | */
/* | Sync/Async:       | Sync                                                              | */
/* | Reentrancy:       | Yes                                                               | */
/* | Parameters (in):  | ScheduleTableID:Schedule table for which status                   | */
/* |                   | is requested                                                      | */
/* | Parameters (out): | ScheduleStatus:Reference to ScheduleStatusType                    | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Return value:     | E_OK:no error                                                     | */
/* |                   | E_OS_ID(only in extended status):Invalid                          | */
/* |                   | ScheduleTableID                                                   | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Description:      | OS289: If the schedule table <ScheduleTableID> is NOT started,    | */
/* |                   | GetScheduleTableStatus() shall pass back SCHEDULETABLE_STOPPED    | */
/* |                   | via the reference parameter <ScheduleStatus> AND shall return     | */
/* |                   | E_OK.                                                             | */
/* |                   | OS353: If the schedule table <ScheduleTableID> was used in a      | */
/* |                   | NextScheduleTable() call AND waits for the end of the current     | */
/* |                   | schedule table, GetScheduleTableStatus() shall return             | */
/* |                   | SCHEDULETABLE_NEXT via the reference parameter <ScheduleStatus>   | */
/* |                   | AND shall return E_OK.                                            | */
/* |                   | OS354: If the schedule table <ScheduleTableID> is configured with | */
/* |                   | explicit synchronization AND no synchronization count was         | */
/* |                   | provided to the Operating System, GetScheduleTableStatus() shall  | */
/* |                   | return SCHEDULETABLE_WAITING via the reference parameter          | */
/* |                   | <ScheduleStatus> AND shall return E_OK.                           | */
/* |                   | OS290: If the schedule table <ScheduleTableID> is started AND     | */
/* |                   | synchronous, GetScheduleTableStatus() shall pass back             | */
/* |                   | SCHEDULETABLE_RUNNING_AND_SYNCHRONOUS via the reference parameter | */
/* |                   | <ScheduleStatus> AND shall return E_OK.                           | */
/* |                   | OS291: If the schedule table <ScheduleTableID> is started AND NOT | */
/* |                   | synchronous (deviation is not within the precision interval OR    | */
/* |                   | the schedule table has been set asynchronous),                    | */
/* |                   | GetScheduleTableStatus() shall pass back SCHEDULETABLE_RUNNING    | */
/* |                   | via the reference parameter ScheduleStatus AND shall return E_OK. | */
/* |                   | OS293: If the identifier <ScheduleTableID> is NOT valid,          | */
/* |                   | GetScheduleTableStatus() shall return E_OS_ID.                    | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Caveats:          | None                                                              | */
/* | Configuration:    | Available in all Scalability Classes.                             | */
/* |-------------------+-------------------------------------------------------------------| */
StatusType GetScheduleTableStatus(ScheduleTableType ScheduleTableID,
                                  ScheduleTableStatusRefType ScheduleStatus)
{
    StatusType xRet = E_OK;
#if(cfgOS_STATUS_LEVEL==OS_STATUS_EXTEND)
    if(ScheduleTableID > (cfgOS_SCHEDULE_TABLE_NUM-1))
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    *ScheduleStatus=tableGetSchedTblStatus(ScheduleTableID);
    return xRet;
}

/* |-------------------+----------------------------------------------------------------| */
/* | Service name:     | IncrementCounter                                               | */
/* |-------------------+----------------------------------------------------------------| */
/* | Syntax:           | StatusType IncrementCounter(CounterType CounterID)             | */
/* | Service ID:       | OSServiceId_IncrementCounter                                   | */
/* | Sync/Async:       | Sync, may cause rescheduling.                                  | */
/* | Reentrancy:       | Yes                                                            | */
/* | Parameters (in):  | CounterID:The Counter to be incremented                        | */
/* | Parameters (out): | None                                                           | */
/* |-------------------+----------------------------------------------------------------| */
/* | Return value:     | E_OK:No errors                                                 | */
/* |                   | E_OS_ID (only in EXTENDED status):The CounterID was not valid  | */
/* |                   | or counter is implemented in hardware and can not be           | */
/* |                   | incremented by software.                                       | */
/* |-------------------+----------------------------------------------------------------| */
/* | Description:      | OS285: If the input parameter <CounterID> is not valid OR the  | */
/* |                   | counter is a hardware counter, IncrementCounter() shall return | */
/* |                   | E_OS_ID.                                                       | */
/* |                   | OS286: If its input parameter is valid, IncrementCounter()     | */
/* |                   | shall increment the counter <CounterID> by one (if any alarm   | */
/* |                   | connected to this counter expires, the given action, e.g. task | */
/* |                   | activation, is done) and shall return E_OK.                    | */
/* |                   | OS321: If an error happens during the execution of an alarm    | */
/* |                   | action, e.g. E_OS_LIMIT caused by a task activation, the error | */
/* |                   | hook(s) are called, but the IncrementCounter() service itself  | */
/* |                   | will return E_OK.                                              | */
/* |-------------------+----------------------------------------------------------------| */
/* | Caveats:          | if called from a task, rescheduling may take place.            | */
/* | Configuration:    | Available in all Scalability Classes.                          | */
/* |-------------------+----------------------------------------------------------------| */
StatusType IncrementCounter(CounterType CounterID)
{
    StatusType xRet = E_OK;
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
    if ( CounterID > (cfgOS_COUNTER_NUM - 1) )
    {
        xRet = E_OS_ID;
        goto Error_Exit;
    }
#endif
    tableIncCntCurValue(CounterID);
#if(cfgOS_USE_ALARM==STD_TRUE)
    OSProcessAlarm(CounterID);
#endif
#if(cfgOS_SCHEDULE_TABLE_NUM>0)
    OSProcessScheduleTable(CounterID);
#endif
#if(cfgOS_STATUS_LEVEL == OS_STATUS_EXTEND)
  Error_Exit:
#endif
    return xRet;
}
/* |-------------------+-----------------------------------------------------------------| */
/* | Service name:     | GetCounterValue                                                 | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Syntax:           | StatusType GetCounterValue(CounterType CounterID,               | */
/* |                   | TickRefType Value)                                              | */
/* | Service ID:       | OSServiceId_GetCounterValue                                     | */
/* | Sync/Async:       | Sync                                                            | */
/* | Reentrancy:       | Yes                                                             | */
/* | Parameters (in):  | CounterID:The Counter which tick value should be read           | */
/* | Parameters (out): | Value:Contains the current tick value of the counter            | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Return value:     | E_OK:no error                                                   | */
/* |                   | E_OS_ID(only in extended status):The <CounterID> was not valid. | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Description:      | OS376: If the input parameter <CounterID> is not valid, the     | */
/* |                   | service should return E_OS_ID.                                  | */
/* |                   | OS377: If its input parameter is valid, GetCounterValue() shall | */
/* |                   | return the current tick value of the counter via <Value> and    | */
/* |                   | return E_OK.                                                    | */
/* |-------------------+-----------------------------------------------------------------| */
/* | Caveats:          | Note that for counters of OsCounterType = HARDWARE the real     | */
/* |                   | timer value (the ¨C possibly adjusted - hardware value, see      | */
/* |                   | OS384) is returned, whereas for counters of OsCounterType =     | */
/* |                   | SOFTWARE the current ¡°software¡± tick value is returned.         | */
/* | Configuration:    | Available in all Scalability Classes.                           | */
/* |-------------------+-----------------------------------------------------------------| */
StatusType GetCounterValue(CounterType CounterID,TickRefType pxValue)
{
    return E_OK;
}
/* |-------------------+-------------------------------------------------------------------| */
/* | Service name:     | GetElapsedCounterValue                                            | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Syntax:           | StatusType GetElapsedCounterValue(CounterType CounterID,          | */
/* |                   | TickRefType Value,TickRefType ElapsedValue)                       | */
/* | Service ID:       | OSServiceId_GetElapsedCounterValue                                | */
/* | Sync/Async:       | Sync                                                              | */
/* | Reentrancy:       | Yes                                                               | */
/* | Parameters (in):  | CounterID:The Counter to be read                                  | */
/* |                   | Value:The previously read tick value of the counter               | */
/* | Parameters (out): | Value:Contains the current tick value of the counter              | */
/* |                   | ElapsedValue:The difference to the previous read value            | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Return value:     | E_OK:No error                                                     | */
/* |                   | E_OS_ID(only in extended status):The CounterID was not valid      | */
/* |                   | E_OS_VALUE(only in extended status):The given Value was not valid | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Description:      | OS381: If the input parameter <CounterID> is not valid the        | */
/* |                   | service will return E_OS_ID.                                      | */
/* |                   | OS391: If the <Value> is larger than the max allowed value        | */
/* |                   | of the <CounterID>, the service will return E_OS_VALUE.           | */
/* |                   | OS382: If its input parameter are valid, GetElapsedCounterValue() | */
/* |                   | shall return the number of elapsed ticks since the given <Value>  | */
/* |                   | value via <ElapsedValue> and shall return E_OK.                   | */
/* |                   | OS460: In the <Value> parameter the current tick value of the     | */
/* |                   | counter is returned.                                              | */
/* |-------------------+-------------------------------------------------------------------| */
/* | Caveats:          | If the timer already passed the <Value> value a second            | */
/* |                   | (or multiple) time, the result returned is wrong. The reason      | */
/* |                   | is that the service can not detect such a relative overflow.      | */
/* | Configuration:    | Available in all Scalability Classes.                             | */
/* |-------------------+-------------------------------------------------------------------| */
StatusType GetElapsedCounterValue(CounterType CounterID,
                                  TickRefType Value,TickRefType ElapsedValue)
{
    return E_OK;
}

/* |-------------------+--------------------------------------------------------------------| */
/* | Service name:     | TerminateApplication                                               | */
/* |-------------------+--------------------------------------------------------------------| */
/* | Syntax:           | StatusType TerminateApplication(RestartType RestartOption)         | */
/* | Service ID:       | OSServiceId_TerminateApplication                                   | */
/* | Sync/Async:       | Normally does not return to the caller, except called in the       | */
/* |                   | wrong context: sync.                                               | */
/* | Reentrancy:       | Yes                                                                | */
/* | Parameters (in):  | RestartOption:Either RESTART for doing a restart of the            | */
/* |                   | OS-Application or NO_RESTART if OS-Application shall not be        | */
/* |                   | restarted.                                                         | */
/* | Parameters (out): | None                                                               | */
/* |-------------------+--------------------------------------------------------------------| */
/* | Return value:     | E_OS_CALLEVEL:Called in the wrong context.                         | */
/* |                   | E_OS_VALUE:RestartOption is neither RESTART nor NO_RESTART.        | */
/* |-------------------+--------------------------------------------------------------------| */
/* | Description:      | OS287: If called from allowed context ¨C see table [7.7.3.2.1] --,  | */
/* |                   | TerminateApplication() shall terminate the calling OS-Application  | */
/* |                   | (i.e. to kill all tasks, disable the interrupt sources of those    | */
/* |                   | OsIsrs which belong to the OS- Application and free all other OS   | */
/* |                   | resources associated with the application).                        | */
/* |                   | OS288: If called from wrong context, TerminateApplication() shall  | */
/* |                   | return E_OS_CALLEVEL.                                              | */
/* |                   | OS459: If the <RestartOption> is invalid, the service shall retrun | */
/* |                   | E_OS_VALUE.                                                        | */
/* |                   | OS346: If RestartOption equals RESTART, TerminateApplication()     | */
/* |                   | shall activate the configured OsRestartTask of the terminated      | */
/* |                   | OS-Application.                                                    | */
/* |-------------------+--------------------------------------------------------------------| */
/* | Caveats:          | If no applications are configured the implementation shall make    | */
/* |                   | sure that this service is not available.                           | */
/* | Configuration:    | Available in Scalability Classes 3 and 4                           | */
/* |-------------------+--------------------------------------------------------------------| */
StatusType TerminateApplication(RestartType RestartOption)
{
    return E_OK;
}
