#include "Cpu.h"
#include "SCS.h"
#define DEBUG_SIM /* 使用仿真模式调试内核 ，屏蔽while等待状态语句 */
void CpuUnlockProtecReg(void)
{
    uword uwPASSWORD;

    SCU_SLC = 0xAAAA;                    /* command 0 */
    SCU_SLC = 0x5554;                    /* command 1 */

    uwPASSWORD = SCU_SLS & 0x00FF;
    uwPASSWORD = (~uwPASSWORD) & 0x00FF;

    SCU_SLC = 0x9600 | uwPASSWORD;       /* command 2 */
    SCU_SLC = 0x0000;                    /* command 3 */

}
void CpuLockProtecReg(void)
{
    uword uwPASSWORD;

    SCU_SLC = 0xAAAA;                    /* command 0 */
    SCU_SLC = 0x5554;                    /* command 1 */

    uwPASSWORD = SCU_SLS & 0x00FF;
    uwPASSWORD = (~uwPASSWORD) & 0x00FF;

    SCU_SLC = 0x9600 | uwPASSWORD;       /* command 2 */
    SCU_SLC = 0x1800;                    /* command 3; new PASSWOR is 0x00 */

    uwPASSWORD = SCU_SLS & 0x00FF;
    uwPASSWORD = (~uwPASSWORD) & 0x00FF;
    SCU_SLC = 0x8E00 | uwPASSWORD;       /* command 4 */

}

void CpuFrequencyInit(void){
    Scs_ErrorType Error;

    CpuUnlockProtecReg();
    Scs_EnableHighPrecOsc(1U);

/*
 * For application and internal application resets, the complete PLL 
 * configuration could be avoided
 * The entry from application resets and internal application reset 
 * is covered in the following differentiation in int/ext clock in
 * lock/unlocked state.
 */

    if  ((SCU_PLLSTAT & 0x0004) == 0x0004)       /* fR derived from Internal clock */
    {
        /* Normal startup state during boot and the clock */
        /* has to be in the next step configured on the external crystal */
        /* use XTAL/VCO, count XTAL clock */

        Scs_InitTimer();      /*  initialize CCU6 timer T13 for SCS driver */

        /* perform transition from base mode to normal mode, check for error */
        Error = Scs_GoFromBaseModeToNormalMode();
        if(Error)
        {
            for(;;)      /* this part of code should not be reached */
            {
                NOP();
            }
        }
        /* fR derived from external crystal clock + VCO is locked */
        if ((SCU_PLLSTAT & 0x1009) == 0x1009)      
        {
            /* usually after an application reset where clock need not be configured again. */
            /* check K2/P/N values and decide whether these values have to be adapted based */
            /* on application needs. */
            NOP();
            /* usually the PLL losss of Lock TRAP schould be enabled here. */
        }
        else       /* fR derived from external crystal clock + VCO is not locked */
        {
            /* estimate the K1 value and the current frequency */
            /* reduce K2/P/N values in steps so that the frequency */
            /* jumps is limited to 20MHz or factor of 5 whichever is minimum */
            NOP();
        }
    }
    CpuLockProtecReg();
}

void CpuInitOsTick(void){
    volatile unsigned int uwTemp;

    CpuUnlockProtecReg();
    /*  ----------------------------------------------------------------------- */
    /*  Configuration of the RTC Kernal */
    /*  ----------------------------------------------------------------------- */

    RTC_KSCCFG     =  0x0003;      /* Module Enable */

    uwTemp         =  RTC_KSCCFG;  /* dummy read to avoid pipeline effects */

    SCU_RTCCLKCON  =  0x0005;     /*  RTC working in synchronous mode with osc. */
    /*                               freq. as a input freq. */

    /* ----------------------------------------------------------------------- */
    /* Configuration of the RTC Counter: */
    /* ----------------------------------------------------------------------- */
    /* - RTC input frequency is 8000.000 kHz */
    /* - the input clock is divided by factor 32 */
    /* - the input clock is divided by factor 8 */
    /* - counter is started after initialization */
    /* - RTC count clock frequency is 31.250 kHz */
	#ifndef DEBUG_SIM
    while(!(RTC_CON & 0x8000));    /* wait until access is possible */
	#endif
    RTC_CON        =  0x0002;      /* load RTC control register */

    /* - overflow period on T14:  5.0240 [ms] */
    /* - overflow period on CNT0: 5.1446 [s] */
    /* - overflow period on CNT1: 5.4875 [min] */
    /* - overflow period on CNT2: 5.8534 [h] */
    /* - overflow period on CNT3: 8.3248 [months] */

    RTC_T14REL     =  0xFF63;      // load T14 count/reload register
    RTC_RTCL       =  0x0000;      // load RTC low register
    RTC_RELL       =  0x0000;      // load RTC reload low register
    RTC_RTCH       =  0x0000;      // load RTC high register
    RTC_RELH       =  0x0000;      // load RTC reload high register

    /* ----------------------------------------------------------------------- */
    /* Configuration of the used RTC Interrupts: */
    /* ----------------------------------------------------------------------- */
    /* RTC service request node configuration: */
    /* - RTC interrupt priority level (ILVL) = 1 */
    /* - RTC interrupt group level (GLVL) = 0 */
    /* - RTC group priority extension (GPX) = 0 */

    RTC_IC         =  0x0044;

    RTC_ISNC       =  0x0001;      /* load RTC interrupt sub node register */

    RTC_CON_RUN    =  1;           /* starts the counter */
    CpuLockProtecReg();
}
