#include <xc.h>
#include "adc.h"
#include "mcc.h"

/**
  Section: Macro Declarations
*/

#define ACQ_US_DELAY 5

/**
  Section: ADC Module APIs
*/

void ADC_Initialize(void)
{
    // set the ADC to the options selected in the User Interface
    
    // GOnDONE stop; ADON enabled; CHS ANA0; 
    ADCON0 = 0x01;
    
    // ADFM left; ADPREF external; ADCS FOSC/2; 
    ADCON1 = 0x02;
    
    // ADACT disabled; 
    ADACT = 0x00;
    
    // ADRESL 0; 
    ADRESL = 0x00;
    
    // ADRESH 0; 
    ADRESH = 0x00;
    
}

void ADC_SelectChannel(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;    
    // Turn on the ADC module
    ADCON0bits.ADON = 1;  
}

void ADC_StartConversion()
{
    // Start the conversion
    ADCON0bits.GOnDONE = 1;
}


bool ADC_IsConversionDone()
{
    // Start the conversion
    return (!ADCON0bits.GOnDONE);
}

adc_result_t ADC_GetConversionResult(void)
{
    // Conversion finished, return the result
    return ((ADRESH << 8) + ADRESL);
}

adc_result_t ADC_GetConversion(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;    

    // Turn on the ADC module
    ADCON0bits.ADON = 1;
    // Acquisition time delay
    __delay_us(ACQ_US_DELAY);

    // Start the conversion
    ADCON0bits.GOnDONE = 1;

    // Wait for the conversion to finish
    while (ADCON0bits.GOnDONE)
    {
    }
    // Conversion finished, return the result
    return ((ADRESH << 8) + ADRESL);

}

/**
 End of File
*/