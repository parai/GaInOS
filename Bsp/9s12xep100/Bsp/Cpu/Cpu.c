

#include "Cpu.h"

#define  OSC_CLOCK		   16000000	   /* 晶振频率   */

void CpuFrequencyInit(void) 
{
    CRGINT = 0;                  //关中断
    CLKSEL_PLLSEL = 0;           //在未初始化PLL前不使用PLL的输出作为CPU时钟
    
  #if(CPU_FREQUENCY == 40) 
    SYNR = 4;
  #elif(CPU_FREQUENCY == 32)
    SYNR = 3;     
  #elif(CPU_FREQUENCY == 24)
    SYNR = 2;
  #endif 

    REFDV = 1;                   //PLLCLK=2×OSCCLK×(SYNR+1)/(REFDV+1)＝64MHz ,fbus=32M
    PLLCTL_PLLON = 1;            //开PLL
    while (CRGFLG_LOCK == 0);    //等待PLL锁定频率
    CLKSEL_PLLSEL = 1;           //选择系统时钟由PLL产生

}

void CpuInitOsTick(void) 
{
  CRGINT_RTIE=1;       /* 使能实时中断 */
  RTICTL = 0x70;       /* 设置实时中断的时间间隔为32.768ms */
   /* 中断周期=1/16 x 10E-6 x （0+1）x 2E（7+9）=0.004096s=4.096ms  */
}