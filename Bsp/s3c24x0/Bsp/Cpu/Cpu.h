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

typedef void (*OSIsrHandlerType)(void);
extern OSIsrHandlerType OSIsrVector[32];

#define     OS_EINT0_Handler		OS_Default_IRQ_Handler
#define     OS_EINT1_Handler		OS_Default_IRQ_Handler
#define     OS_EINT2_Handler		OS_Default_IRQ_Handler
#define     OS_EINT3_Handler		OS_Default_IRQ_Handler
#define     OS_EINT4_7_Handler		OS_Default_IRQ_Handler
#define     OS_EINT8_23_Handler		OS_Default_IRQ_Handler
#define     OS_INT_CAM_Handler		OS_Default_IRQ_Handler
#define     OS_nBATT_FLT_Handler	OS_Default_IRQ_Handler
extern void OSTickISR(void);
#define     OS_INT_TICK_Handler		OSTickISR
#define     OS_INT_WDTAC97_Handler	OS_Default_IRQ_Handler
#define     OS_INT_TIMER0_Handler	OS_Default_IRQ_Handler
#define     OS_INT_TIMER1_Handler	OS_Default_IRQ_Handler
#define     OS_INT_TIMER2_Handler	OS_Default_IRQ_Handler
#define     OS_INT_TIMER3_Handler	OS_Default_IRQ_Handler
#define     OS_INT_TIMER4_Handler	OS_Default_IRQ_Handler
#define     OS_INT_UART2_Handler	OS_Default_IRQ_Handler
#define     OS_INT_LCD_Handler		OS_Default_IRQ_Handler
#define     OS_INT_DMA0_Handler		OS_Default_IRQ_Handler
#define     OS_INT_DMA1_Handler		OS_Default_IRQ_Handler
#define     OS_INT_DMA2_Handler		OS_Default_IRQ_Handler
#define     OS_INT_DMA3_Handler		OS_Default_IRQ_Handler
#define     OS_INT_SDI_Handler		OS_Default_IRQ_Handler
#define     OS_INT_SPI0_Handler		OS_Default_IRQ_Handler
#define     OS_INT_UART1_Handler	OS_Default_IRQ_Handler
#define     OS_INT_NFCON_Handler	OS_Default_IRQ_Handler
#define     OS_INT_USBD_Handler		OS_Default_IRQ_Handler
#define     OS_INT_USBH_Handler		OS_Default_IRQ_Handler
#define     OS_INT_IIC_Handler		OS_Default_IRQ_Handler
#define     OS_INT_UART0_Handler	OS_Default_IRQ_Handler
#define     OS_INT_SPI1_Handler		OS_Default_IRQ_Handler
#define     OS_INT_RTC_Handler		OS_Default_IRQ_Handler
#define     OS_INT_ADC_Handler		OS_Default_IRQ_Handler
/* ================================================================ */
/* Copy from leeos:www.leeos.org */
void init_sys_mmu(void);
void start_mmu(void);
#endif /* _CPU_H_ */
