#include "Os.h"
#include "Cpu.h"
#include "Serial.h"

int main(void) {
    /* Here must be in supervisor mode*/
    CpuFrequencyInit();
    /*make run in ram */
    init_sys_mmu();
    start_mmu();
    Sci_Init();
    printk("Welcome to GaInOS!\r\n");
    StartOS(OSDEFAULTAPPMODE);
    for(;;) {    
    }    
}
