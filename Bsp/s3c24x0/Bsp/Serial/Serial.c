#include "Serial.h"
#include "Cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define cfgBAUDRATE 38400                     

#define TXD0READY   (1<<2)  /* 发送数据状态OK */
#define RXD0READY   (1)     /* 接收数据状态OK */

void Sci_Init(void)
{
	GPHCON |= 0xa0;    /* GPH2,GPH3 used as TXD0,RXD0 */
	GPHUP     = 0x0;   /* GPH2,GPH3内部上拉 */
	ULCON0  = 0x03;    /* 8N1         */
	UCON0     = 0x05;  /* 查询方式为轮询或中断;时钟选择为PCLK */
	UFCON0 = 0x00;     /* 不使用FIFO */
	UMCON0 = 0x00;     /* 不使用流控 */
    /* UBRDIVn=(int)(UART clock)/(baud rate*16)-1*/
	UBRDIV0 = (vPCLK_OUT*10000)/(cfgBAUDRATE*16)-1;      /* 波特率为57600,PCLK=12Mhz */
}
void Sci_Send(unsigned char chr)
{
	while( ! (UTRSTAT0 & TXD0READY) );
	UTXH0 = chr;
}
void printk(char* fmt,...)
{
	unsigned int i=0;
#if 0	
    char *buf=NULL;
#else
	char buf[64];
#endif	
	int len;
	va_list args;
	va_start(args,fmt);
#if 0	
	buf=malloc(64);
    if(NULL==buf)return;		/* 分配内存失败，直接返回 */
#endif	
	len = vsprintf((char*)buf,(char *)fmt,args)+1;
	while('\0'!=buf[i]&&i<len)  Sci_Send(buf[i++]) ;
	free(buf);
	va_end(args);
}
