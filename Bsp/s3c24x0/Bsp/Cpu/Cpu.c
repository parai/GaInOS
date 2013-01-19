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

void CpuInitOsTick(void) 
{

}
