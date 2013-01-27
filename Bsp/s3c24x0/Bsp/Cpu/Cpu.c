#include "Cpu.h"

/* Map Get From S3C2440_UM.PDF*/
/* But A Wrong Info in the map table.shit fuck samsung*/
#if(cfgCpuFrequency==27150)
#define cfgMDIV 173
#define cfgPDIV 2
#define cfgSDIV 2
#elif(cfgCpuFrequency==30400)
#define cfgMDIV 68
#define cfgPDIV 1
#define cfgSDIV 1
#elif(cfgCpuFrequency==40500)
#define cfgMDIV 127
#define cfgPDIV 2
#define cfgSDIV 1
#elif(cfgCpuFrequency==53200)
#define cfgMDIV 125
#define cfgPDIV 1
#define cfgSDIV 1
#endif
 
#if(cfgDivideRatio==1)
#define cfgHDIVN 0
#define cfgPDIVN 1
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==2)
#define cfgHDIVN 1
#define cfgPDIVN 0
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==3)
#define cfgHDIVN 1
#define cfgPDIVN 1
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==4)
#define cfgHDIVN 3
#define cfgPDIVN 0
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==5)
#define cfgHDIVN 3
#define cfgPDIVN 1
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==6)
#define cfgHDIVN 3
#define cfgPDIVN 0
#define cfgHCLK3_HALF 1
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==7)
#define cfgHDIVN 3
#define cfgPDIVN 1
#define cfgHCLK3_HALF 1
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==8)
#define cfgHDIVN 2
#define cfgPDIVN 0
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==9)
#define cfgHDIVN 2
#define cfgPDIVN 1
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 0
#elif(cfgDivideRatio==10)
#define cfgHDIVN 2
#define cfgPDIVN 0
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 1
#elif(cfgDivideRatio==11)
#define cfgHDIVN 2
#define cfgPDIVN 1
#define cfgHCLK3_HALF 0
#define cfgHCLK4_HALF 1
#endif
void CpuFrequencyInit(void) 
{
    /* Set U_LTIME[31:16] = 0xFFFF,M_LTIME[15:0]=0xFFFF*/
	LOCKTIME=0xFFFFFFFF;
    CLKDIVN=(cfgHDIVN<<1)|cfgPDIVN;
    /* As CAMDIVN's initial is 0 so ...*/
    CAMDIVN=(cfgHCLK4_HALF<<9)|(cfgHCLK3_HALF<<8);
    /* FCLK=Mpll=(2*m*Fin)/(p*2^s);
       FVCO=(2*m*Fin)/p,where 600MHZ<=FVCp<=1.2GHZ;
       200MHZ<=FCLK<=600MHZ;
       1<=MDIV<=248,1<=PDIV<=62
       Fin=12MHZ;
       m=(MDIV+8),p=(PDIV+2),s=SDIV */
    MPLLCON=(cfgMDIV<<12)|(cfgPDIV<<4)|(cfgSDIV);
}

#define cfgOS_TICKS_PER_SEC 64
void CpuInitOsTick(void) 
{
    /* Period = ((n+1)/128) second */
	TICNT = (1<<7)|1;
    /* RTC TICK IRQ mode*/
    INTMOD &= (~BIT_TICK);
    /* Clear The Pending bit*/
    SRCPND &= (~BIT_TICK);
    /* Enable RTC TICK interrupe*/
    INTMSK &= (~BIT_TICK);
}
void OS_Default_IRQ_Handler(void)
{
    extern void printk(char *fmt,...);
    printk("Non Implemented IRQ interrupt Handler!\r\n");
}
OSIsrHandlerType OSIsrVector[32]=
{
    OS_EINT0_Handler,
    OS_EINT1_Handler,
    OS_EINT2_Handler,
    OS_EINT3_Handler,
    OS_EINT4_7_Handler,
    OS_EINT8_23_Handler,
    OS_INT_CAM_Handler,
    OS_nBATT_FLT_Handler,
    OS_INT_TICK_Handler,
    OS_INT_WDTAC97_Handler,
    OS_INT_TIMER0_Handler,
    OS_INT_TIMER1_Handler,
    OS_INT_TIMER2_Handler,
    OS_INT_TIMER3_Handler,
    OS_INT_TIMER4_Handler,
    OS_INT_UART2_Handler,
    OS_INT_LCD_Handler,
    OS_INT_DMA0_Handler,
    OS_INT_DMA1_Handler,
    OS_INT_DMA2_Handler,
    OS_INT_DMA3_Handler,
    OS_INT_SDI_Handler,
    OS_INT_SPI0_Handler,
    OS_INT_UART1_Handler,
    OS_INT_NFCON_Handler,
    OS_INT_USBD_Handler,
    OS_INT_USBH_Handler,
    OS_INT_IIC_Handler,
    OS_INT_UART0_Handler,
    OS_INT_SPI1_Handler,
    OS_INT_RTC_Handler,
    OS_INT_ADC_Handler
};
/* ================================================================================ */
/*
mmu.c:
Copyright (C) 2009  david leels <davidontech@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.
*/


/*mask for page table base addr*/
#define PAGE_TABLE_L1_BASE_ADDR_MASK	(0xffffc000)

#define VIRT_TO_PTE_L1_INDEX(addr)	(((addr)&0xfff00000)>>18)

#define PTE_L1_SECTION_NO_CACHE_AND_WB	(0x0<<2)
#define PTE_L1_SECTION_DOMAIN_DEFAULT	(0x0<<5)
#define PTE_ALL_AP_L1_SECTION_DEFAULT	(0x1<<10)

#define PTE_L1_SECTION_PADDR_BASE_MASK	(0xfff00000)
#define PTE_BITS_L1_SECTION				(0x2)

#define L1_PTR_BASE_ADDR			0x30700000
#define PHYSICAL_MEM_ADDR			0x30000000
#define VIRTUAL_MEM_ADDR			0x30000000
#define MEM_MAP_SIZE				0x00800000
#define PHYSICAL_IO_ADDR			0x48000000
#define VIRTUAL_IO_ADDR				0x48000000
#define IO_MAP_SIZE					0x18000000
#define VIRTUAL_VECTOR_ADDR			0x0
#define PHYSICAL_VECTOR_ADDR		0x30000000
#ifdef __GCC__
void start_mmu(void){
	unsigned int ttb=L1_PTR_BASE_ADDR;
	asm (
		"mcr p15,0,%0,c2,c0,0\n"    /* set base address of page table*/
		"mvn r0,#0\n"                  
		"mcr p15,0,r0,c3,c0,0\n"    /* enable all region access*/

		"mov r0,#0x1\n"
		"mcr p15,0,r0,c1,c0,0\n"    /* set back to control register */
		"mov r0,r0\n"
		"mov r0,r0\n"
		"mov r0,r0\n"
		:
		: "r" (ttb)
		:"r0"
	);  
}
#else
/*
start_mmu
	EXPORT start_mmu
    ;asm volatile("mov r0,#L1_PTR_BASE_ADDR 
    LDR r0,=0x30700000
    mcr p15,0,r0,c2,c0,0          ;set base address of page table
    mvn r0,#0                  
    mcr p15,0,r0,c3,c0,0          ;enable all region access

	mrc p15,0,r0,c1,c0,0
    orr r0,r0,#0x1
    mcr p15,0,r0,c1,c0,0          ;set back to control register 
	bx  lr
*/
#if 0
void start_mmu(void){
    /* asm volatile("mov r0,#L1_PTR_BASE_ADDR */
    __asm volatile("LDR r0,=0x30700000");
    __asm volatile("mcr p15,0,r0,c2,c0,0");          /* set base address of page table*/
    __asm volatile("mvn r0,#0");                  
    __asm volatile("mcr p15,0,r0,c3,c0,0");          /* enable all region access*/

	__asm volatile("mrc p15,0,r0,c1,c0,0");
    __asm volatile("orr r0,r0,#0x1");
    __asm volatile("mcr p15,0,r0,c1,c0,0");         /* set back to control register */	
}
#endif
#endif
unsigned int gen_l1_pte(unsigned int paddr){
	return (paddr&PTE_L1_SECTION_PADDR_BASE_MASK)|\
										PTE_BITS_L1_SECTION;
}

unsigned int gen_l1_pte_addr(unsigned int baddr,\
										unsigned int vaddr){
	return (baddr&PAGE_TABLE_L1_BASE_ADDR_MASK)|\
								VIRT_TO_PTE_L1_INDEX(vaddr);
}

void init_sys_mmu(void){
	unsigned int pte;
	unsigned int pte_addr;
	int j;

	for(j=0;j<MEM_MAP_SIZE>>20;j++){
		pte=gen_l1_pte(PHYSICAL_VECTOR_ADDR+(j<<20));
		pte|=PTE_ALL_AP_L1_SECTION_DEFAULT;
		pte|=PTE_L1_SECTION_NO_CACHE_AND_WB;
		pte|=PTE_L1_SECTION_DOMAIN_DEFAULT;
		pte_addr=gen_l1_pte_addr(L1_PTR_BASE_ADDR,\
								VIRTUAL_VECTOR_ADDR+(j<<20));
		*(volatile unsigned int *)pte_addr=pte;
	}
	for(j=0;j<MEM_MAP_SIZE>>20;j++){
		pte=gen_l1_pte(PHYSICAL_MEM_ADDR+(j<<20));
		pte|=PTE_ALL_AP_L1_SECTION_DEFAULT;
		pte|=PTE_L1_SECTION_NO_CACHE_AND_WB;
		pte|=PTE_L1_SECTION_DOMAIN_DEFAULT;
		pte_addr=gen_l1_pte_addr(L1_PTR_BASE_ADDR,\
								VIRTUAL_MEM_ADDR+(j<<20));
		*(volatile unsigned int *)pte_addr=pte;
	}
	for(j=0;j<IO_MAP_SIZE>>20;j++){
		pte=gen_l1_pte(PHYSICAL_IO_ADDR+(j<<20));
		pte|=PTE_ALL_AP_L1_SECTION_DEFAULT;
		pte|=PTE_L1_SECTION_NO_CACHE_AND_WB;
		pte|=PTE_L1_SECTION_DOMAIN_DEFAULT;
		pte_addr=gen_l1_pte_addr(L1_PTR_BASE_ADDR,\
								VIRTUAL_IO_ADDR+(j<<20));
		*(volatile unsigned int *)pte_addr=pte;
	}
}

void remap_l1(unsigned int paddr,unsigned int vaddr,int size){
	unsigned int pte;
	unsigned int pte_addr;
	for(;size>0;size-=1<<20){
		pte=gen_l1_pte(paddr);
		pte_addr=gen_l1_pte_addr(L1_PTR_BASE_ADDR,vaddr);
		*(volatile unsigned int *)pte_addr=pte;
	}
}
