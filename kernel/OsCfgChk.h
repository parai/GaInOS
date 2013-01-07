#ifndef _OSCFGCHK_H_
#define _OSCFGCHK_H_

#ifndef  cfgOS_CONFORMANCE_CLASS
#   error "Error:cfgOS_CONFORMANCE_CLASS for GaInOS was not defined!"
#endif
#ifndef  cfgOS_TASK_NUM
#   error "Error:cfgOS_TASK_NUM for GaInOS was not defined!"
#endif
#ifndef  cfgOS_MAX_IPL
#   error "Error:cfgOS_MAX_IPL for GaInOS was not defined!"
#endif
#ifndef  cfgOS_MAX_PRIORITY
#   error "Error:cfgOS_MAX_PRIORITY for GaInOS was not defined!"
#endif
#ifndef  cfgOS_USE_ALARM
#   error "Error:cfgOS_USE_ALARM for GaInOS was not defined!"
#else
#   ifndef  cfgOS_COUNTER_NUM
#   error "Error:cfgOS_COUNTER_NUM for GaInOS was not defined!"
#   endif
#   ifndef  cfgOS_ALARM_NUM
#   error "Error:cfgOS_ALARM_NUM for GaInOS was not defined!"
#   endif
#endif
#ifndef  cfgOS_USE_RESOURCE
#   error "Error:cfgOS_USE_RESOURCE for GaInOS was not defined!"
#else
#   ifndef  cfgOS_RESOURCE_NUM
#   error "Error:cfgOS_RESOURCE_NUM for GaInOS was not defined!"
#   endif
#   ifndef  cfgOS_ISR_USE_RES
#   error "Error:cfgOS_ISR_USE_RES for GaInOS was not defined!"
#   endif
#endif
#ifndef  cfgOS_USE_INTERNAL_RESOURCE
#   error "Error:cfgOS_USE_INTERNAL_RESOURCE for GaInOS was not defined!"
#else
#   ifndef  cfgOS_INTERNAL_RESOURCE_NUM
#   error "Error:cfgOS_INTERNAL_RESOURCE_NUM for GaInOS was not defined!"
#   endif
#   ifndef  cfgOS_TASK_WITH_IN_RES_NUM
#   error "Error:cfgOS_TASK_WITH_IN_RES_NUM for GaInOS was not defined!"
#   endif
#endif
#ifndef  cfgOS_SCHEDULE_POLICY
#   error "Error:cfgOS_SCHEDULE_POLICY for GaInOS was not defined!"
#endif
#ifndef  cfgOS_STATUS_LEVEL
#   error "Error:cfgOS_STATUS_LEVEL for GaInOS was not defined!"
#endif
#ifndef cfgOS_STACK_USAGE_CHECK
#  define cfgOS_STACK_USAGE_CHECK 0
#endif

#ifndef cfgOS_HOOK_IN_KERNEL
#  define cfgOS_HOOK_IN_KERNEL 0
#endif
#ifndef cfgOS_SHUT_DOWN_HOOK
#  define cfgOS_SHUT_DOWN_HOOK 0
#endif
#ifndef cfgOS_START_UP_HOOK
#  define cfgOS_START_UP_HOOK  0
#endif
#ifndef cfgOS_ERROR_HOOK
#  define cfgOS_ERROR_HOOK     0
#endif
#ifndef cfgOS_PRE_TASK_HOOK
#  define cfgOS_PRE_TASK_HOOK  0
#endif
#ifndef cfgOS_POST_TASK_HOOK
#  define cfgOS_POST_TASK_HOOK 0
#endif

/* if ONLY_ONE_TASK_PER_PRIORITY, only one Task per Priority
 * if SEVERAL_TASKS_PER_PRIORITY, allow more than one Tasks per Priority 
 */
#if  (cfgOS_CONFORMANCE_CLASS == BCC2) || (cfgOS_CONFORMANCE_CLASS == ECC2)
#define cfgOS_TASK_PER_PRIORITY  SEVERAL_TASKS_PER_PRIORITY
#define cfgOS_MULTIPLY_ACTIVATE  STD_TRUE /* Multiple requests of activation was allowed */
#elif(cfgOS_CONFORMANCE_CLASS == BCC1) || (cfgOS_CONFORMANCE_CLASS == ECC1)
#define cfgOS_TASK_PER_PRIORITY  ONLY_ONE_TASK_PER_PRIORITY
#define cfgOS_MULTIPLY_ACTIVATE STD_FALSE /* Multiple requests of activation was not allowed */
#endif

#if(cfgOS_CONFORMANCE_CLASS == ECC1 || cfgOS_CONFORMANCE_CLASS == BCC1)
#    define BASIC_TASK  BCC1
#    define EXTEND_TASK ECC1
#elif(cfgOS_CONFORMANCE_CLASS == ECC2 || cfgOS_CONFORMANCE_CLASS == BCC2)
#    define BASIC_TASK  BCC2
#    define EXTEND_TASK ECC2
#endif
/* if cfgOS_SUM_ACTIVATES > 0,then the method conformed to OSEK will be used,
   a Queue will used to record the parallel activates.
   Or Only record the Activates's times.
*/
#if(cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
#   ifndef  cfgOS_SUM_ACTIVATES
#   error "Error:cfgOS_SUM_ACTIVATES for GaInOS was not defined!"
#   endif
#else
#   ifdef cfgOS_SUM_ACTIVATES
#   undef cfgOS_SUM_ACTIVATES
#   endif
#   ifndef  cfgOS_SUM_ACTIVATES
#   define cfgOS_SUM_ACTIVATES 0
#   endif
#endif

#endif /* _OSCFGCHK_H_ */
