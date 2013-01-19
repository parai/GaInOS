#include "Os.h"
#include "Cpu.h"
#include "Serial.h"
void dely(uint32_t tt)
{
   uint32_t i;
   for(;tt>0;tt--)
   {
     for(i=0;i<10000;i++){}
   }
}
int main(void) {
#if 0      
   GPBCON = 0x00555555;
   GPBDAT = 0x0000;
   GPBDAT = 0x0ffffff;
   
   while(1)
   {
   
     GPBDAT = 0x0000;
     dely(120);
     GPBDAT = 0x0ffffff;
     dely(120);
   }
#else
    CpuFrequencyInit();
    Sci_Init();
    printk("Start GaInOS,Welcome!\r\n");
    StartOS(OSDEFAULTAPPMODE);
    for(;;) {
    }
#endif    
}
