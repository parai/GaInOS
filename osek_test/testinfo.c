
#include "testinfo.h"

void TestHead(char * xTestInfo,unsigned int xCaseNr)
{
    printk("=====================   START   ===================== \n");
    printk("Test For %s, Test Point Number is %d.\n",xTestInfo,xCaseNr);
    
}
void TestPosition(unsigned long * pxResultMap,unsigned int pos,unsigned int res)
{
    *pxResultMap |= (unsigned long)res<<pos;
}
void TestEnd(unsigned long xResultMap,unsigned int xCaseNr)
{
    unsigned int i;
    for(i=0;i<xCaseNr;i++)
    {
        if((xResultMap & (1u<<i )) != 0)
        {
            printk("Test Point < %2d > FAILED!\n",i+1);
        }
        else
        {
            printk("Test Point < %2d > PASSED!\n",i+1);
        }
    }
    if(xResultMap == 0)
    {
        printk("This Conformance Test PASSED!\n");
    }
    else
    {
        printk("This Conformance Test FAILED!\n");
    }
    printk("=====================   END   =====================\n");
}
