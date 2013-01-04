#include "Serial.h"
#include <stm32f10x.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

#define SCI_UNINITIALISED 0 
#define SCI_INITIALISED  1

static unsigned int g_sci_state = SCI_UNINITIALISED;

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void Sci_Init (void) {
#ifndef __DBG_ITM
  int i;

  RCC->APB2ENR |=  (   1UL <<  0);         /* enable clock Alternate Function */
  AFIO->MAPR   &= ~(   1UL <<  2);         /* clear USART1 remap              */

  RCC->APB2ENR |=  (   1UL <<  2);         /* enable GPIOA clock              */
  GPIOA->CRH   &= ~(0xFFUL <<  4);         /* clear PA9, PA10                 */
  GPIOA->CRH   |=  (0x0BUL <<  4);         /* USART1 Tx (PA9) output push-pull*/
  GPIOA->CRH   |=  (0x04UL <<  8);         /* USART1 Rx (PA10) input floating */

  RCC->APB2ENR |=  (   1UL << 14);         /* enable USART1 clock             */

  /* 115200 baud, 8 data bits, 1 stop bit, no flow control */
  USART1->CR1   = 0x002C;                  /* enable RX, TX                   */
  USART1->CR2   = 0x0000;
  USART1->CR3   = 0x0000;                  /* no flow control                 */
  USART1->BRR   = 0x0271;
  for (i = 0; i < 0x1000; i++) __NOP();    /* avoid unwanted output           */
  USART1->CR1  |= 0x2000;                    /* enable USART                   */

#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void Sci_Send(int chr) {

#ifdef __DBG_ITM
    ITM_SendChar(chr);
#else
  while (!(USART1->SR & USART_SR_TXE));
  USART1->DR = (chr & 0x1FF);
#endif
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
