#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include "system.h"
#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
void InitializeSystem (void){
    TRISCbits.TRISC7 = 0; //configure pin 18 as output
    for (int i = 0; i<5; ++i){
        PORTCbits.RC7 = 0;  //set pin 18 to low
        __delay_ms(500);
        PORTCbits.RC7 = 1;  //set pin 18 to high
        __delay_ms(500);
    }
}

