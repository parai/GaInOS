#ifndef _CFGOBJ_H_
#define _CFGOBJ_H_

#include "Os.h"


/* GaInOS Resource Configuration */
#define vRes1 0
#define vRes2 1
#define vRes3 2
#define vRes4 3
#define vRes5 4
#define vRes6 5
#define vResA 6
extern const PriorityType OSResCeilPrioTable[cfgOS_RESOURCE_NUM];
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

#define ISR2 0x01
#define ISR3 0x02

#define vTask1 0
extern TASK( vTask1 );
#define vTask2 1
extern TASK( vTask2 );

#endif /* _CFGOBJ_H_ */
