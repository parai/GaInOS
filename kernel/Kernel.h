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
#ifndef _KERNEL_H_
#define _KERNEL_H_
#include "Os.h"
#include "Osctrl.h"
#include "Task.h"
#include "Alarm.h"
#include "Event.h"
#include "Resource.h"
#include "Autosar.h"

/* Maybe Its IPL is not 0 if GetResource() share with ISR2 */
#define TASK_LEVEL  0u
/* Its IPL not equals 0, and OSIsr2Nesting is 0 */
#define ISR1_LEVEL  1u
/* Its IPL not equals 0, and also OSIsr2Nesting */
#define ISR2_LEVEL  2u
/* No matter What, When OSIsr2Nesting is 0,It's in Task Level 
   But There is no Way to judge Whether it is an ISR1_LEVEL,
   As If A Task has called GetResource(),Its IPL may changed*/
#define OSGetContextLevel() (OSIsr2Nesting)
void OS_ENTER_CRITICAL(void);
void OS_EXIT_CRITICAL(void);

#endif /* _KERNEL_H_ */

