#ifndef _TESTINFO_H_
#define _TESTINFO_H_

#include "Serial.h"

#define PASSED  0
#define FAILED  1

#define OSTestCheck(condition,pos)                      \
    do{                                                 \
        if((condition))                                 \
        {                                               \
            TestPosition(&g_TestResult,(pos),PASSED);   \
        }                                               \
        else                                            \
        {                                               \
            TestPosition(&g_TestResult,(pos),FAILED);   \
        }                                               \
    }while(0)

void TestHead(char * xTestInfo,unsigned int xCaseNr);
void TestPosition(unsigned long * pxResultMap,unsigned int pos,unsigned int res);
void TestEnd(unsigned long xResultMap,unsigned int xCaseNr);

#endif /* _TESTINFO_H_ */
