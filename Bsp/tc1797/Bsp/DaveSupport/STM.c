//****************************************************************************
// @Module        System Timer (STM)
// @Filename      STM.c
// @Project       DAvE_Bsp.dav
//----------------------------------------------------------------------------
// @Controller    Infineon TC1797
//
// @Compiler      Tasking 3.1
//
// @Codegenerator 1.0
//
// @Description   This file contains functions that use the STM module.
//
//----------------------------------------------------------------------------
// @Date          2013/1/20 09:34:57
//
//****************************************************************************

// USER CODE BEGIN (STM_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "dave.h"

// USER CODE BEGIN (STM_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (STM_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (STM_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (STM_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************


// USER CODE BEGIN (STM_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (STM_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (STM_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (STM_General,9)

// USER CODE END


//****************************************************************************
// @Function      void STM_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the STM function 
//                library. It is assumed that the SFRs used by this library 
//                are in their reset state. 
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2013/1/20
//
//****************************************************************************

// USER CODE BEGIN (Init,1)

// USER CODE END

void STM_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of the Module Clock:
  ///  -----------------------------------------------------------------------
  ///  - enable the STM module
  ///  - Application Reset resets the STM module
  ///  - STM clock is System clock / 7 (= 12.86 MHz; 77.78 ns )

  MAIN_vResetENDINIT();
  STM_CLC.U      = 0x00000700;   // load clock control register
  MAIN_vSetENDINIT();


  ///  -----------------------------------------------------------------------
  ///  STM Resolution And Range:
  ///  -----------------------------------------------------------------------
  ///  STM_TIMER_0     bits 0 ... 31 :
  ///  - resolution = 0.08 us
  ///  - range = 5.57 min

  ///  STM_TIMER_1     bits 4 ... 35 :
  ///  - resolution = 1.24 us
  ///  - range = 1.48 h

  ///  STM_TIMER_2     bits 8 ... 39 :
  ///  - resolution = 19.91 us
  ///  - range = 23.75 h

  ///  STM_TIMER_3     bits 12 ... 43 :
  ///  - resolution = 318.58 us
  ///  - range = 15.84 days

  ///  STM_TIMER_4     bits 16 ... 47 :
  ///  - resolution = 5.10 ms
  ///  - range = 253.39 days

  ///  STM_TIMER_5     bits 20 ... 51 :
  ///  - resolution = 81.56 ms
  ///  - range = 11.11 years

  ///  STM_TIMER_6     bits 32 ... 55 :
  ///  - resolution = 5.57 min
  ///  - range = 177.72 years

  ///  STM_TIMER_7     bits 32 ... 55 :
  ///  - resolution = 5.57 min
  ///  - range = 177.72 years

  ///  -----------------------------------------------------------------------
  ///  STM Compare 0 configuration:
  ///  -----------------------------------------------------------------------
  ///  - lowest bit number of STM which is compared with CMP0 is 7
  ///  - 10 bit(s) in register CMP0 are used for the compare operation with 
  ///    STM
  ///  - required compare value of CMP0 is 0x000003E8
  ///  - real compare value of CMP0 is 0x000003E8
  ///  - request on compare match with CMP0 is enabled and located to STMIR0

  STM_CMP0.U     = 0x000003E8;   // load compare register 0

  ///  -----------------------------------------------------------------------
  ///  STM Compare 1 configuration:
  ///  -----------------------------------------------------------------------
  ///  - lowest bit number of STM which is compared with CMP1 is 7
  ///  - 10 bit(s) in register CMP1 are used for the compare operation with 
  ///    STM
  ///  - required compare value of CMP1 is 0x000003E8
  ///  - real compare value of CMP1 is 0x000003E8
  ///  - request on compare match with CMP1 is enabled and located to STMIR1

  STM_CMP1.U     = 0x000003E8;   // load compare register 1

  STM_CMCON.U    = 0x07090709;   // load compare match control register
  STM_ICR.U      = 0x00000051;   // load interrupt control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used STM Interrupts:
  ///  -----------------------------------------------------------------------
  ///  - SRN0 service request node configuration:
  ///  - SRN0 interrupt priority level (SRPN) = 1
  ///  - SRN0 CPU interrupt is selected

  STM_SRC0.U     = 0x00001001;   // set service request control register

  ///  - SRN1 service request node configuration:
  ///  - SRN1 interrupt priority level (SRPN) = 2
  ///  - SRN1 CPU interrupt is selected

  STM_SRC1.U     = 0x00001002;   // set service request control register


  // USER CODE BEGIN (Init,3)

  // USER CODE END

} //  End of function STM_vInit


//****************************************************************************
// @Function      void STM_viSRN0(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine 0 of STM. It is 
//                called if the selected compare match is pending.
//                Please note that you have to add application specific code 
//                to this function.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2013/1/20
//
//****************************************************************************

// USER CODE BEGIN (SRN0,1)
#if 0
// USER CODE END

void INTERRUPT (STM_INT0) STM_viSRN0(void)
{
  // USER CODE BEGIN (SRN0,2)

  // USER CODE END

  if(STM_ICR.B.CMP0IR == 1)   // if compare match of CMP0 is pending
  {
    // USER CODE BEGIN (SRN0,3)

    // USER CODE END

    STM_ISRR.B.CMP0IRR = 1;  // clear request bit of CMP0
  }

  // USER CODE BEGIN (SRN0,5)

  // USER CODE END

} //  End of function STM_viSRN0


//****************************************************************************
// @Function      void STM_viSRN1(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine 1 of STM. It is 
//                called if the selected compare match is pending.
//                Please note that you have to add application specific code 
//                to this function.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2013/1/20
//
//****************************************************************************

// USER CODE BEGIN (SRN1,1)

// USER CODE END

void INTERRUPT (STM_INT1) STM_viSRN1(void)
{
  // USER CODE BEGIN (SRN1,2)

  // USER CODE END

  if(STM_ICR.B.CMP1IR == 1)   // if compare match of CMP1 is pending
  {
    // USER CODE BEGIN (SRN1,4)

    // USER CODE END

    STM_ISRR.B.CMP1IRR = 1;  // clear request bit of CMP1
  }

  // USER CODE BEGIN (SRN1,5)

  // USER CODE END

} //  End of function STM_viSRN1



// USER CODE BEGIN (STM_General,10)
#endif
// USER CODE END

