 /**
   Section: Included Files
 */

 #include <xc.h>
 #include "pwm3.h"

 /**
   Section: PWM Module APIs
 */

 void PWM3_Initialize(void)
 {
     // Set the PWM to the options selected in the MPLAB(c) Code Configurator.
     // PWM3POL active_hi; PWM3EN enabled; 
     PWM3CON = 0x80;
     
     // DC 2; 
     PWM3DCH = 0x02;
     
     // DC 1; 
     PWM3DCL = 0x40;
     
 }

 void PWM3_LoadDutyValue(uint16_t dutyValue)
 {
     // Writing to 8 MSBs of PWM duty cycle in PWMDCH register
     PWM3DCH = (dutyValue & 0x03FC)>>2;
     
     // Writing to 2 LSBs of PWM duty cycle in PWMDCL register
     PWM3DCL = (dutyValue & 0x0003)<<6;
 }
 
  void PWM3_LoadDutyValue_percentage(uint8_t dutypercentage) 
  {    
      uint8_t PMWDC_Value = (dutypercentage/100.00) * 39.00; 
      // Writing byte to PWMDCH register
     PWM3DCH = PMWDC_Value >> 2;
     PWM3DCL = PMWDC_Value << 6;
 }
 /**
  End of File
 */
