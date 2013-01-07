#ifndef _CFGOBJ_H_
#define _CFGOBJ_H_

#include "Os.h"


/* GaInOS Counter And Alarm Configuration */
#define vCounter1 0
#define vAlarm1 0
extern const AlarmBaseType OSCounterBaseTable[cfgOS_COUNTER_NUM];
extern const AlarmClassType OSAlarmClassTable[cfgOS_ALARM_NUM];
extern const AlarmContainerType OSAlarmContainerTable[cfgOS_ALARM_NUM];
extern const CounterType OSAlarmOwnerTable[cfgOS_ALARM_NUM];

/* GaInOS Task Configuration */
extern const TaskStackRefType OSTaskStackTable[cfgOS_TASK_NUM];
extern const PriorityType     OSTaskInitPriorityTable[cfgOS_TASK_NUM];
extern const TaskEntryType    OSTaskEntryTable[cfgOS_TASK_NUM];
extern const BoolType         OSTaskAutoActivateTable[cfgOS_TASK_NUM];

#if (cfgOS_MULTIPLY_ACTIVATE == STD_TRUE)
extern const uint8_t OSMaxActivateCountTable[cfgOS_TASK_NUM];
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC2) || (cfgOS_CONFORMANCE_CLASS == ECC1)
extern const uint8_t OSTaskConfClassTable[cfgOS_TASK_NUM];
#endif

#if (cfgOS_SCHEDULE_POLICY == MIXED_PREEMPTIVE_SCHEDULE)
extern const BoolType OSTaskPreemtableTable[cfgOS_TASK_NUM];
#endif

#if (cfgOS_CONFORMANCE_CLASS == ECC1 || cfgOS_CONFORMANCE_CLASS == ECC2)
extern const uint8_t OSTskClsTypeTable[cfgOS_TASK_NUM];
#endif

#define vTask1 0
extern TASK( vTask1 );
#define vTask2 1
extern TASK( vTask2 );
#define vTask3 2
extern TASK( vTask3 );

#endif /* _CFGOBJ_H_ */
