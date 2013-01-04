#ifndef _CFGOBJ_H_
#define _CFGOBJ_H_

#include "Os.h"

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
#define vTask1Event1 0x1
#define vTask1Event2 0x2
#define vTask1Event3 0x4
#define vTask1Event4 0x8
#define vTask1Event5 0x10
#define vTask1Event6 0x20
#define vTask1Event7 0x40
#define vTask1Event8 0x80
extern TASK( vTask1 );
#define vTask2 1
#define vTask2Event1 0x1
#define vTask2Event2 0x2
#define vTask2Event3 0x4
#define vTask2Event4 0x8
#define vTask2Event5 0x10
#define vTask2Event6 0x20
#define vTask2Event7 0x40
#define vTask2Event8 0x80
extern TASK( vTask2 );
#define vTask3 2
#define vTask3Event1 0x1
#define vTask3Event2 0x2
#define vTask3Event3 0x4
#define vTask3Event4 0x8
#define vTask3Event5 0x10
#define vTask3Event6 0x20
#define vTask3Event7 0x40
#define vTask3Event8 0x80
extern TASK( vTask3 );
#define vTask4 3
#define vTask4Event1 0x1
#define vTask4Event2 0x2
#define vTask4Event3 0x4
#define vTask4Event4 0x8
#define vTask4Event5 0x10
#define vTask4Event6 0x20
#define vTask4Event7 0x40
#define vTask4Event8 0x80
extern TASK( vTask4 );
#define vTask5 4
#define vTask5Event1 0x1
#define vTask5Event2 0x2
#define vTask5Event3 0x4
#define vTask5Event4 0x8
#define vTask5Event5 0x10
#define vTask5Event6 0x20
#define vTask5Event7 0x40
#define vTask5Event8 0x80
extern TASK( vTask5 );
#define vTask6 5
#define vTask6Event1 0x1
#define vTask6Event2 0x2
#define vTask6Event3 0x4
#define vTask6Event4 0x8
#define vTask6Event5 0x10
#define vTask6Event6 0x20
#define vTask6Event7 0x40
#define vTask6Event8 0x80
extern TASK( vTask6 );
#define vTask7 6
#define vTask7Event1 0x1
#define vTask7Event2 0x2
#define vTask7Event3 0x4
#define vTask7Event4 0x8
#define vTask7Event5 0x10
#define vTask7Event6 0x20
#define vTask7Event7 0x40
#define vTask7Event8 0x80
extern TASK( vTask7 );
#define vTask8 7
#define vTask8Event1 0x1
#define vTask8Event2 0x2
#define vTask8Event3 0x4
#define vTask8Event4 0x8
#define vTask8Event5 0x10
#define vTask8Event6 0x20
#define vTask8Event7 0x40
#define vTask8Event8 0x80
extern TASK( vTask8 );
#define vTask9 8
#define vTask9Event1 0x1
#define vTask9Event2 0x2
#define vTask9Event3 0x4
#define vTask9Event4 0x8
#define vTask9Event5 0x10
#define vTask9Event6 0x20
#define vTask9Event7 0x40
#define vTask9Event8 0x80
extern TASK( vTask9 );
#define vTask10 9
#define vTask10Event1 0x1
#define vTask10Event2 0x2
#define vTask10Event3 0x4
#define vTask10Event4 0x8
#define vTask10Event5 0x10
#define vTask10Event6 0x20
#define vTask10Event7 0x40
#define vTask10Event8 0x80
extern TASK( vTask10 );
#define vTask11 10
#define vTask11Event1 0x1
#define vTask11Event2 0x2
#define vTask11Event3 0x4
#define vTask11Event4 0x8
#define vTask11Event5 0x10
#define vTask11Event6 0x20
#define vTask11Event7 0x40
#define vTask11Event8 0x80
extern TASK( vTask11 );
#define vTask12 11
#define vTask12Event1 0x1
#define vTask12Event2 0x2
#define vTask12Event3 0x4
#define vTask12Event4 0x8
#define vTask12Event5 0x10
#define vTask12Event6 0x20
#define vTask12Event7 0x40
#define vTask12Event8 0x80
extern TASK( vTask12 );
#define vTask13 12
#define vTask13Event1 0x1
#define vTask13Event2 0x2
#define vTask13Event3 0x4
#define vTask13Event4 0x8
#define vTask13Event5 0x10
#define vTask13Event6 0x20
#define vTask13Event7 0x40
#define vTask13Event8 0x80
extern TASK( vTask13 );
#define vTask14 13
#define vTask14Event1 0x1
#define vTask14Event2 0x2
#define vTask14Event3 0x4
#define vTask14Event4 0x8
#define vTask14Event5 0x10
#define vTask14Event6 0x20
#define vTask14Event7 0x40
#define vTask14Event8 0x80
extern TASK( vTask14 );
#define vTask15 14
#define vTask15Event1 0x1
#define vTask15Event2 0x2
#define vTask15Event3 0x4
#define vTask15Event4 0x8
#define vTask15Event5 0x10
#define vTask15Event6 0x20
#define vTask15Event7 0x40
#define vTask15Event8 0x80
extern TASK( vTask15 );
#define vTask16 15
#define vTask16Event1 0x1
#define vTask16Event2 0x2
#define vTask16Event3 0x4
#define vTask16Event4 0x8
#define vTask16Event5 0x10
#define vTask16Event6 0x20
#define vTask16Event7 0x40
#define vTask16Event8 0x80
extern TASK( vTask16 );

#endif /* _CFGOBJ_H_ */


