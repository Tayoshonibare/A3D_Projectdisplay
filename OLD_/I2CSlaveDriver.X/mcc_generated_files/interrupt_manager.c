#include "interrupt_manager.h"
#include "mcc.h"

void interrupt INTERRUPT_InterruptManager (void)
{
    // interrupt handler
    if(INTCONbits.PEIE == 1 && PIE3bits.SSP1IE == 1 && PIR3bits.SSP1IF == 1)
    {
        I2C1_ISR();
    }
    else if(INTCONbits.PEIE == 1 && PIE3bits.BCL2IE == 1 && PIR3bits.BCL2IF == 1)
    {
        I2C2_BusCollisionISR();
    }
    else if(INTCONbits.PEIE == 1 && PIE3bits.SSP2IE == 1 && PIR3bits.SSP2IF == 1)
    {
        I2C2_ISR();
    }
    else
    {
        //Unhandled Interrupt
    }
}
/**
 End of File
*/
