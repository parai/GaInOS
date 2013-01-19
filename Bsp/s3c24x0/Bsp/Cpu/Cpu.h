#ifndef _CPU_H_
#define _CPU_H_

#include "s3c2440.h"

/* uint in 10kHZ */
#define cfgCpuFrequency 40500
#define cfgDivideRatio 9
/* Here cfgDivideRatio:
    if 1,then FCLK:HCLK:PCLK=1:1:2;
    if 2,then FCLK:HCLK:PCLK=1:2:2;
    if 3,then FCLK:HCLK:PCLK=1:2:4;
    if 4,then FCLK:HCLK:PCLK=1:3:3;
    if 5,then FCLK:HCLK:PCLK=1:3:6;
    if 6,then FCLK:HCLK:PCLK=1:6:6;
    if 7,then FCLK:HCLK:PCLK=1:6:12;
    if 8,then FCLK:HCLK:PCLK=1:4:4;
    if 9,then FCLK:HCLK:PCLK=1:4:8;
    if 10,then FCLK:HCLK:PCLK=1:8:8;
    if 11,then FCLK:HCLK:PCLK=1:8:16;
*/

#if(cfgDivideRatio==1)
#define vPCLK_OUT  cfgCpuFrequency/2
#elif(cfgDivideRatio==2)
#define vPCLK_OUT  cfgCpuFrequency/2
#elif(cfgDivideRatio==3)
#define vPCLK_OUT  cfgCpuFrequency/4
#elif(cfgDivideRatio==4)
#define vPCLK_OUT  cfgCpuFrequency/3
#elif(cfgDivideRatio==5)
#define vPCLK_OUT  cfgCpuFrequency/6
#elif(cfgDivideRatio==6)
#define vPCLK_OUT  cfgCpuFrequency/6
#elif(cfgDivideRatio==7)
#define vPCLK_OUT  cfgCpuFrequency/12
#elif(cfgDivideRatio==8)
#define vPCLK_OUT  cfgCpuFrequency/4
#elif(cfgDivideRatio==9)
#define vPCLK_OUT  cfgCpuFrequency/8
#elif(cfgDivideRatio==10)
#define vPCLK_OUT  cfgCpuFrequency/8
#elif(cfgDivideRatio==11)
#define vPCLK_OUT  cfgCpuFrequency/16
#endif
void CpuFrequencyInit(void);


#endif /* _CPU_H_ */
