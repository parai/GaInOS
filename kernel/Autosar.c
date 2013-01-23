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
