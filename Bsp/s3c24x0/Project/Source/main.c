#include "Os.h"
#include "Cpu.h"
#include "Serial.h"

int main(void) {
    CpuFrequencyInit();
    Sci_Init();
    printk("Welcome to GaInOS's world!\r\n");
    __asm("SWI 0");
    StartOS(OSDEFAULTAPPMODE);
    for(;;) {    
    }    
}
