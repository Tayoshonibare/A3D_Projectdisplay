#ifndef __SYSTEM_H
#define __SYSTEM_H

    /* Microcontroller MIPs (FCY) */
    #define _XTAL_FREQ      32000000L //Frequency is 32MHz
    #define SYS_FREQ        _XTAL_FREQ
    #define FCY             SYS_FREQ/4

    //#define SYS_FREQ        500000L
    //#define FCY             SYS_FREQ/4
    void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

#endif
