#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

/* Baseline devices don't have interrupts. Note that some PIC16's 
 * are baseline devices.  Unfortunately the baseline detection macro is 
 * _PIC12 */
#ifndef _PIC12
void EnableInterupts(void){
    PIE3bits.SSP1IE = 1;    //Enables the MSSP interrupt
    PIE3bits.SSP2IE = 1;    //Enables the MSSP interrupt
    
    INTCONbits.GIE = 1;     //Global Interrupt Enable bit
    INTCONbits.PEIE = 1;    //enables active perhiperals interrupts
    

}




void interrupt isr(void){
    INTCONbits.GIE = 0;     //turn off further interrupts
    PORTCbits.RC7 = 0;
    //Determine source of interrupt
    if (PIR3bits.SSP1IF == 1){          //If cause is: The Transmission/Reception/Bus Condition is complete
        PIR3bits.SSP1IF = 0;
    }
    else {}

    //Clear interrupt flag bits
    
    
     INTCONbits.GIE = 1;     //Global Interrupt Enable bit to activate interrupts again
}
#endif


