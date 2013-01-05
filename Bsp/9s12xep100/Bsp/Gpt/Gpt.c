#include "Cpu.h"
#include "Gpt.h"
volatile unsigned int* const TimerPeriod = (unsigned int*)(REG_BASE + 0x00000348);
void Gpt_Init(void)
{
    PITCE    = 0x00; /*Disable All Timers*/ 
    PITINTE  = 0x00; /*Disable All Timers's Interrupt*/ 
    PITMTLD0 = CPU_FREQUENCY-1; 
    PITMTLD1 = CPU_FREQUENCY-1;
    PITCFLMT = 0x80; /*Enable PIT Module*/ 
}
/*time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS.*/
void Gpt_StartTimer(unsigned char xTimerID,unsigned int xPeriod)
{
    /*xPeriod is in uint us,so Max is 65535us = 65ms*/
    TimerPeriod[xTimerID*2] = xPeriod;
    PITTF   |= 1 << xTimerID;
    PITINTE |= 1 << xTimerID;    
    PITCE   |= 1 << xTimerID;       
}
void Gpt_StopTimer(unsigned char xTimerID)
{
    PITCE   &= ~(1 << xTimerID);
    PITINTE &= ~(1 << xTimerID); 
}
