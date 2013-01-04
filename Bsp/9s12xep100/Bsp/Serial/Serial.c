#include "Serial.h"
#include "stdio.h"
#include "stdlib.h"

#define  BUS_CLOCK		   CPU_FREQUENCY*1000000	   /* 总线频率   */
#define BAUD 9600                      /* 串口波特率 */

#define SCI_UNINITIALISED 0 
#define SCI_INITIALISED  1

static unsigned int g_sci_state = SCI_UNINITIALISED;
static void Sci_Init()
{
	SCI0BD = BUS_CLOCK/16/BAUD;   /* 设置SCI0波特率为9600 */
	SCI0CR1 = 0x00;       /* 设置SCI0为正常模式，八位数据位，无奇偶校验*/
	SCI0CR2 = 0x08;       /* 允许发送数据，禁止中断功能 */
}
static void Sci_Send(unsigned char data)
{
	while(!SCI0SR1_TDRE);       /* 等待发送数据寄存器（缓冲器）为空 */
	SCI0DRL = data;
}
void printk(char *fmt,...){
	unsigned int i=0;
    char *buf=NULL;
	int len;
	va_list args;
	if(SCI_UNINITIALISED==g_sci_state){
		Sci_Init();
		g_sci_state=SCI_INITIALISED;
	}
	va_start(args,fmt);
	buf=malloc(64);
	if(NULL==buf)return;		/* 分配内存失败，直接返回 */
	len = vsprintf((char*)buf,(char *)fmt,args)+1;
	while('\0'!=buf[i]&&i<len)  Sci_Send(buf[i++]) ;
	free(buf);
	va_end(args);
}
