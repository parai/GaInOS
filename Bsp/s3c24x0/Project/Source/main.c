#include "Os.h"
#include "Cpu.h"
#include "Serial.h"

int main(void) {
    CpuFrequencyInit();
    Sci_Init();
    printk("Start GaInOS,Welcome,Parai!\r\n");
    //__asm("SWI 0");
    StartOS(OSDEFAULTAPPMODE);
    for(;;) {
    }    
}
