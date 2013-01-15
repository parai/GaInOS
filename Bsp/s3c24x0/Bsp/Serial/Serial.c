#include "Serial.h"
#include "s3c2440.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define BUS_CLOCK		   CPU_FREQUENCY*1000000	   /* 总线频率   */
#define BAUD 9600                      /* 串口波特率 */

#define SCI_UNINITIALISED 0 
#define SCI_INITIALISED  1

#define TXD0READY   (1<<2)  /* 发送数据状态OK */
#define RXD0READY   (1)     /* 接收数据状态OK */

static unsigned int g_sci_state = SCI_UNINITIALISED;
void Sci_Init(void)
{
	GPHCON |= 0xa0;    /* GPH2,GPH3 used as TXD0,RXD0 */
	GPHUP     = 0x0;   /* GPH2,GPH3内部上拉 */
	ULCON0  = 0x03;    /* 8N1         */
	UCON0     = 0x05;  /* 查询方式为轮询或中断;时钟选择为PCLK */
	UFCON0 = 0x00;     /* 不使用FIFO */
	UMCON0 = 0x00;     /* 不使用流控 */
	UBRDIV0 = 12;      /* 波特率为57600,PCLK=12Mhz */

}
void Sci_Send(unsigned char chr)
{
	while( ! (UTRSTAT0 & TXD0READY) );
	UTXH0 = chr;
}
void DebugInfo(char *info)
{
	if(SCI_UNINITIALISED==g_sci_state){
		Sci_Init();
		g_sci_state=SCI_INITIALISED;
	}
	while((*info)!='\0')
	{
		Sci_Send(*info);
	}
}
void printk(char* fmt,...)
{
	unsigned int i=0;
#if 1	
    char *buf=NULL;
#else
	char buf[64];
#endif	
	int len;
	va_list args;
	if(SCI_UNINITIALISED==g_sci_state){
		Sci_Init();
		g_sci_state=SCI_INITIALISED;
	}
	va_start(args,fmt);
#if 1	
	buf=malloc(64);
#endif	
	if(NULL==buf)return;		/* 分配内存失败，直接返回 */
	len = vsprintf((char*)buf,(char *)fmt,args)+1;
	while('\0'!=buf[i]&&i<len)  Sci_Send(buf[i++]) ;
	free(buf);
	va_end(args);
}
