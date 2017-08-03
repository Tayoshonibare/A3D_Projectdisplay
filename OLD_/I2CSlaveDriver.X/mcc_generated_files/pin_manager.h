#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set SunSensorR aliases
#define SunSensorR_TRIS               TRISAbits.TRISA0
#define SunSensorR_LAT                LATAbits.LATA0
#define SunSensorR_PORT               PORTAbits.RA0
#define SunSensorR_WPU                WPUAbits.WPUA0
#define SunSensorR_OD                ODCONAbits.ODCA0
#define SunSensorR_ANS                ANSELAbits.ANSA0
#define SunSensorR_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define SunSensorR_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define SunSensorR_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define SunSensorR_GetValue()           PORTAbits.RA0
#define SunSensorR_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define SunSensorR_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define SunSensorR_SetPullup()      do { WPUAbits.WPUA0 = 1; } while(0)
#define SunSensorR_ResetPullup()    do { WPUAbits.WPUA0 = 0; } while(0)
#define SunSensorR_SetPushPull()    do { ODCONAbits.ODCA0 = 1; } while(0)
#define SunSensorR_SetOpenDrain()   do { ODCONAbits.ODCA0 = 0; } while(0)
#define SunSensorR_SetAnalogMode()  do { ANSELAbits.ANSA0 = 1; } while(0)
#define SunSensorR_SetDigitalMode() do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set MTQVoltageR aliases
#define MTQVoltageR_TRIS               TRISAbits.TRISA1
#define MTQVoltageR_LAT                LATAbits.LATA1
#define MTQVoltageR_PORT               PORTAbits.RA1
#define MTQVoltageR_WPU                WPUAbits.WPUA1
#define MTQVoltageR_OD                ODCONAbits.ODCA1
#define MTQVoltageR_ANS                ANSELAbits.ANSA1
#define MTQVoltageR_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define MTQVoltageR_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define MTQVoltageR_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define MTQVoltageR_GetValue()           PORTAbits.RA1
#define MTQVoltageR_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define MTQVoltageR_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define MTQVoltageR_SetPullup()      do { WPUAbits.WPUA1 = 1; } while(0)
#define MTQVoltageR_ResetPullup()    do { WPUAbits.WPUA1 = 0; } while(0)
#define MTQVoltageR_SetPushPull()    do { ODCONAbits.ODCA1 = 1; } while(0)
#define MTQVoltageR_SetOpenDrain()   do { ODCONAbits.ODCA1 = 0; } while(0)
#define MTQVoltageR_SetAnalogMode()  do { ANSELAbits.ANSA1 = 1; } while(0)
#define MTQVoltageR_SetDigitalMode() do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set RA3 procedures
#define RA3_SetHigh()    do { LATAbits.LATA3 = 1; } while(0)
#define RA3_SetLow()   do { LATAbits.LATA3 = 0; } while(0)
#define RA3_Toggle()   do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define RA3_GetValue()         PORTAbits.RA3
#define RA3_SetDigitalInput()   do { TRISAbits.TRISA3 = 1; } while(0)
#define RA3_SetDigitalOutput()  do { TRISAbits.TRISA3 = 0; } while(0)
#define RA3_SetPullup()     do { WPUAbits.WPUA3 = 1; } while(0)
#define RA3_ResetPullup()   do { WPUAbits.WPUA3 = 0; } while(0)
#define RA3_SetAnalogMode() do { ANSELAbits.ANSA3 = 1; } while(0)
#define RA3_SetDigitalMode()do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set MTQVoltageRL_dirRIN aliases
#define MTQVoltageRL_dirRIN_TRIS               TRISAbits.TRISA6
#define MTQVoltageRL_dirRIN_LAT                LATAbits.LATA6
#define MTQVoltageRL_dirRIN_PORT               PORTAbits.RA6
#define MTQVoltageRL_dirRIN_WPU                WPUAbits.WPUA6
#define MTQVoltageRL_dirRIN_OD                ODCONAbits.ODCA6
#define MTQVoltageRL_dirRIN_ANS                ANSELAbits.ANSA6
#define MTQVoltageRL_dirRIN_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define MTQVoltageRL_dirRIN_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define MTQVoltageRL_dirRIN_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define MTQVoltageRL_dirRIN_GetValue()           PORTAbits.RA6
#define MTQVoltageRL_dirRIN_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define MTQVoltageRL_dirRIN_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define MTQVoltageRL_dirRIN_SetPullup()      do { WPUAbits.WPUA6 = 1; } while(0)
#define MTQVoltageRL_dirRIN_ResetPullup()    do { WPUAbits.WPUA6 = 0; } while(0)
#define MTQVoltageRL_dirRIN_SetPushPull()    do { ODCONAbits.ODCA6 = 1; } while(0)
#define MTQVoltageRL_dirRIN_SetOpenDrain()   do { ODCONAbits.ODCA6 = 0; } while(0)
#define MTQVoltageRL_dirRIN_SetAnalogMode()  do { ANSELAbits.ANSA6 = 1; } while(0)
#define MTQVoltageRL_dirRIN_SetDigitalMode() do { ANSELAbits.ANSA6 = 0; } while(0)

// get/set MTQVoltageRL_dirFIN aliases
#define MTQVoltageRL_dirFIN_TRIS               TRISAbits.TRISA7
#define MTQVoltageRL_dirFIN_LAT                LATAbits.LATA7
#define MTQVoltageRL_dirFIN_PORT               PORTAbits.RA7
#define MTQVoltageRL_dirFIN_WPU                WPUAbits.WPUA7
#define MTQVoltageRL_dirFIN_OD                ODCONAbits.ODCA7
#define MTQVoltageRL_dirFIN_ANS                ANSELAbits.ANSA7
#define MTQVoltageRL_dirFIN_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define MTQVoltageRL_dirFIN_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define MTQVoltageRL_dirFIN_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define MTQVoltageRL_dirFIN_GetValue()           PORTAbits.RA7
#define MTQVoltageRL_dirFIN_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define MTQVoltageRL_dirFIN_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define MTQVoltageRL_dirFIN_SetPullup()      do { WPUAbits.WPUA7 = 1; } while(0)
#define MTQVoltageRL_dirFIN_ResetPullup()    do { WPUAbits.WPUA7 = 0; } while(0)
#define MTQVoltageRL_dirFIN_SetPushPull()    do { ODCONAbits.ODCA7 = 1; } while(0)
#define MTQVoltageRL_dirFIN_SetOpenDrain()   do { ODCONAbits.ODCA7 = 0; } while(0)
#define MTQVoltageRL_dirFIN_SetAnalogMode()  do { ANSELAbits.ANSA7 = 1; } while(0)
#define MTQVoltageRL_dirFIN_SetDigitalMode() do { ANSELAbits.ANSA7 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()    do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()   do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()   do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()         PORTBbits.RB0
#define RB0_SetDigitalInput()   do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()  do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()     do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()   do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode() do { ANSELBbits.ANSB0 = 1; } while(0)
#define RB0_SetDigitalMode()do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set RB1 procedures
#define RB1_SetHigh()    do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()   do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()   do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()         PORTBbits.RB1
#define RB1_SetDigitalInput()   do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()  do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()     do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()   do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode() do { ANSELBbits.ANSB1 = 1; } while(0)
#define RB1_SetDigitalMode()do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()    do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()   do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()   do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()         PORTCbits.RC3
#define RC3_SetDigitalInput()   do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()  do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetPullup()     do { WPUCbits.WPUC3 = 1; } while(0)
#define RC3_ResetPullup()   do { WPUCbits.WPUC3 = 0; } while(0)
#define RC3_SetAnalogMode() do { ANSELCbits.ANSC3 = 1; } while(0)
#define RC3_SetDigitalMode()do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()    do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()   do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()   do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()         PORTCbits.RC4
#define RC4_SetDigitalInput()   do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()  do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()     do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()   do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode() do { ANSELCbits.ANSC4 = 1; } while(0)
#define RC4_SetDigitalMode()do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()    do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()   do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()   do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()         PORTCbits.RC5
#define RC5_SetDigitalInput()   do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()  do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()     do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()   do { WPUCbits.WPUC5 = 0; } while(0)
#define RC5_SetAnalogMode() do { ANSELCbits.ANSC5 = 1; } while(0)
#define RC5_SetDigitalMode()do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()     do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()   do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode() do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set DEBUG_LED aliases
#define DEBUG_LED_TRIS               TRISCbits.TRISC7
#define DEBUG_LED_LAT                LATCbits.LATC7
#define DEBUG_LED_PORT               PORTCbits.RC7
#define DEBUG_LED_WPU                WPUCbits.WPUC7
#define DEBUG_LED_OD                ODCONCbits.ODCC7
#define DEBUG_LED_ANS                ANSELCbits.ANSC7
#define DEBUG_LED_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define DEBUG_LED_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define DEBUG_LED_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define DEBUG_LED_GetValue()           PORTCbits.RC7
#define DEBUG_LED_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define DEBUG_LED_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define DEBUG_LED_SetPullup()      do { WPUCbits.WPUC7 = 1; } while(0)
#define DEBUG_LED_ResetPullup()    do { WPUCbits.WPUC7 = 0; } while(0)
#define DEBUG_LED_SetPushPull()    do { ODCONCbits.ODCC7 = 1; } while(0)
#define DEBUG_LED_SetOpenDrain()   do { ODCONCbits.ODCC7 = 0; } while(0)
#define DEBUG_LED_SetAnalogMode()  do { ANSELCbits.ANSC7 = 1; } while(0)
#define DEBUG_LED_SetDigitalMode() do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/