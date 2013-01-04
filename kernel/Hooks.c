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
/* | File-name:        | Hooks.c                                      | */
/* |-------------------+----------------------------------------------| */
/* | Reference Manual: | (1)Specification of Operating System(V3.0.2) | */
/* |                   | AUTOSAR_SWS_OS.PDF                           | */
/* |                   |                                              | */
/* |                   | (2)Operating SystemSpecification 2.2.3(OSEK) | */
/* |                   | os223.doc                                    | */
/* |-------------------+----------------------------------------------| */

#include "Os.h"


/* |---------------------------------------------------------------------| */
/* | Those hook routines are                                             | */
/* | 1)called by the operating system, in a special context depending on | */
/* | the implementation of the operating system                          | */
/* | 2)higher prior than all tasks                                       | */
/* | 3)not interrupted by category 2 interrupt routines.                 | */
/* | 4)part of the operating system                                      | */
/* | 5)implemented by the user with user defined functionality           | */
/* | 6)standardised in interface, but not standardised in functionality  | */
/* | (environment and behaviour of the hook routine itself), therefore   | */
/* | usually hook routines are not portable                              | */
/* | 7)are only allowed to use a subset of API functions                 | */
/* | (see Figure 12-1). mandatory, but configurable via OIL              | */
/* |---------------------------------------------------------------------| */


#if ( cfgOS_HOOK_IN_KERNEL == 1 )

#if(cfgOS_START_UP_HOOK == 1)
/* |------------------+------------------------------------------------------------| */
/* | Syntax           | void StartupHook ( void )                                  | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (In):  | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (Out): | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Description:     | This hook routine is called by the operating system at     | */
/* |                  | the end of the operating system initialisation and         | */
/* |                  | before the scheduler is running. At this time the          | */
/* |                  | application can initialise device drivers etc.             | */
/* |------------------+------------------------------------------------------------| */
/* | Particularities: | See chapter 11.1 for general description of hook routines. | */
/* |------------------+------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                     | */
/* |------------------+------------------------------------------------------------| */
void StartupHook(void)
{
}
#endif

#if (cfgOS_SHUT_DOWN_HOOK == 1)
/* |------------------+------------------------------------------------------| */
/* | Syntax:          | void ShutdownHook ( StatusType <Error> )             | */
/* |------------------+------------------------------------------------------| */
/* | Parameter (In):  | Error:error occurred                                 | */
/* |------------------+------------------------------------------------------| */
/* | Parameter (Out): | none                                                 | */
/* |------------------+------------------------------------------------------| */
/* | Description:     | This hook routine is called by the operating system  | */
/* |                  | when the OS service ShutdownOS has been called.      | */
/* |                  | This routine is called during the operating system   | */
/* |                  | shut down.                                           | */
/* |------------------+------------------------------------------------------| */
/* | Particularities: | ShutdownHook is a hook routine for user defined      | */
/* |                  | shutdown functionality, see chapter 11.4(os223.doc). | */
/* |------------------+------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                               | */
/* |------------------+------------------------------------------------------| */
void ShutdownHook ( StatusType xError)
{


}
#endif

#if(cfgOS_ERROR_HOOK == 1)
/* |------------------+------------------------------------------------------------| */
/* | Syntax           | void ErrorHook (StatusType <Error> )                       | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (In):  | Error:error occurred                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (Out): | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Description:     | 1.This hook routine is called by the operating system at   | */
/* |                  | the end of a system service which returns StatusType not   | */
/* |                  | equal E_OK. It is called before returning to the task      | */
/* |                  | level.                                                     | */
/* |                  | 2.This hook routine is called when an alarm expires and    | */
/* |                  | an error is detected during task activation or event       | */
/* |                  | setting.                                                   | */
/* |                  | 3.The ErrorHook is not called, if a system service         | */
/* |                  | called from ErrorHook does not return E_OK as status       | */
/* |                  | value. Any error by calling of system services from        | */
/* |                  | the ErrorHook can only be detected by evaluating           | */
/* |                  | the status value.                                          | */
/* |------------------+------------------------------------------------------------| */
/* | Particularities: | See chapter 11.1 for general description of hook routines. | */
/* |------------------+------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                     | */
/* |------------------+------------------------------------------------------------| */
void ErrorHook(StatusType xError)
{

}
#endif

#if(cfgOS_PRE_TASK_HOOK == 1)
/* |------------------+------------------------------------------------------------| */
/* | Syntax           | void PreTaskHook ( void )                                  | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (In):  | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (Out): | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Description:     | This hook routine is called by the operating system        | */
/* |                  | before executing a new task, but after the transition      | */
/* |                  | of the task to the running state (to allow evaluation      | */
/* |                  | of the TaskID by GetTaskID).                               | */
/* |------------------+------------------------------------------------------------| */
/* | Particularities: | See chapter 11.1 for general description of hook routines. | */
/* |------------------+------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                     | */
/* |------------------+------------------------------------------------------------| */
void PreTaskHook(void)
{

}
#endif

#if(cfgOS_POST_TASK_HOOK == 1)
/* |------------------+------------------------------------------------------------| */
/* | Syntax           | void PostTaskHook ( void )                                 | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (In):  | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Parameter (Out): | none                                                       | */
/* |------------------+------------------------------------------------------------| */
/* | Description:     | This hook routine is called by the operating system        | */
/* |                  | after executing the current task, but before leaving       | */
/* |                  | the task's running state (to allow evaluation of the       | */
/* |                  | TaskID by GetTaskID).                                      | */
/* |------------------+------------------------------------------------------------| */
/* | Particularities: | See chapter 11.1 for general description of hook routines. | */
/* |------------------+------------------------------------------------------------| */
/* | Conformance:     | BCC1, BCC2, ECC1, ECC2                                     | */
/* |------------------+------------------------------------------------------------| */
void PostTaskHook(void)
{

}
#endif

#endif
