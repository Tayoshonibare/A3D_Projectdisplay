 #ifndef _PWM3_H
 #define _PWM3_H
 
 /**
   Section: Included Files
 */

 #include <xc.h>
 #include <stdint.h>

 #ifdef __cplusplus  // Provide C++ Compatibility

     extern "C" {

 #endif

 /**
   Section: Macro Declarations
 */

 #define PWM3_INITIALIZE_DUTY_VALUE    9

 /**
   Section: PWM Module APIs
 */

 /**
   @Summary
     Initializes the PWM3

   @Description
     This routine initializes the EPWM3_Initialize
     This routine must be called before any other PWM3 routine is called.
     This routine should only be called once during system initialization.

   @Preconditions
     None

   @Param
     None

   @Returns
     None

   @Comment
     

  @Example
     <code>
     uint16_t dutycycle;

     PWM3_Initialize();
     PWM3_LoadDutyValue(dutycycle);
     </code>
  */
 void PWM3_Initialize(void);

 /**
   @Summary
     Loads 16-bit duty cycle.

   @Description
     This routine loads the 16 bit duty cycle value.

   @Preconditions
     PWM3_Initialize() function should have been called 
         before calling this function.

   @Param
     Pass 16bit duty cycle value.

   @Returns
     None

   @Example
     <code>
     uint16_t dutycycle;

     PWM3_Initialize();
     PWM3_LoadDutyValue(dutycycle);
     </code>
 */
 void PWM3_LoadDutyValue(uint16_t dutyValue);
 
 /**
   @Summary
     Loads 8-bit duty cycle percentage.

   @Description
     This routine loads the 8 bit duty cycle value.

   @Preconditions
     PWM3_Initialize() function should have been called 
         before calling this function.

   @Param
     Pass 8bit duty cycle value.

   @Returns
     None

   @Example
     <code>
     uint8_t dutycycle;

     PWM3_Initialize();
     PWM3_LoadDutyValue(percentage);
     </code>
 */
  void PWM3_LoadDutyValue_percentage(uint8_t dutypercentage);
 
 #ifdef __cplusplus  // Provide C++ Compatibility

     }

 #endif

 #endif	//PWM3_H
 /**
  End of File
 */