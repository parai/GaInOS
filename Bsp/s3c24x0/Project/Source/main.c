#include "Os.h"
#include "s3c2440.h"
void dely(uint32_t tt)
{
   uint32_t i;
   for(;tt>0;tt--)
   {
     for(i=0;i<10000;i++){}
   }
}
int main(void) {
    
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
#if 0  
    StartOS(OSDEFAULTAPPMODE);
    for(;;) {
    }
#endif    
}
