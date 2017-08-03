/**
  MSSP1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    i2c1.c

  @Summary
    This is the generated header file for the MSSP1 driver using 
    MPLAB(c) Code Configurator

  @Description
    This header file provides APIs for driver for MSSP1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16F15355
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB 	          :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "i2c1.h"
#include "../DESCENT.h"
#include "pin_manager.h"

#define I2C1_SLAVE_ADDRESS 0x22 
#define I2C1_SLAVE_MASK    0x7F

typedef enum
{
    SLAVE_NORMAL_DATA,
    SLAVE_DATA_ADDRESS,
} SLAVE_WRITE_DATA_TYPE;

/**
 Section: Global Variables
*/

volatile uint8_t    I2C1_slaveWriteData      = 0x55;

/**
 Section: Local Functions
*/
void I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS i2c_bus_state,  uint8_t dataI2C );



/**
  Prototype:        void I2C1_Initialize(void)
  Input:            none
  Output:           none
  Description:      I2C1_Initialize is an
                    initialization routine that takes inputs from the GUI.
  Comment:          
  Usage:            I2C1_Initialize();

*/
void I2C1_Initialize(void)
{
    // initialize the hardware
    // SMP Standard Speed; CKE disabled; 
    SSP1STAT = 0x80;
    // SSPEN enabled; CKP disabled; SSPM 7 Bit Polling; 
    SSP1CON1 = 0x26;
    // ACKEN disabled; GCEN disabled; PEN disabled; ACKDT acknowledge; RSEN disabled; RCEN disabled; SEN enabled; 
    SSP1CON2 = 0x01;
    // SBCDE disabled; BOEN disabled; SCIE disabled; PCIE disabled; DHEN disabled; SDAHT 300ns; AHEN disabled; 
    SSP1CON3 = 0x08;
    // SSPMSK 127; 
    SSP1MSK = (I2C1_SLAVE_MASK << 1);  // adjust UI mask for R/nW bit            
    // SSPADD 34; 
    SSP1ADD = (I2C1_SLAVE_ADDRESS << 1);  // adjust UI address for R/nW bit

    // clear the slave interrupt flag
    PIR3bits.SSP1IF = 0;
    // enable the master interrupt
    PIE3bits.SSP1IE = 1;

}

void I2C1_ISR ( void )
{
    uint8_t     i2c_data                = 0x55;


    // NOTE: The slave driver will always acknowledge
    //       any address match.

    PIR3bits.SSP1IF = 0;        // clear the slave interrupt flag
    i2c_data        = SSP1BUF;  // read SSPBUF to clear BF
    if(1 == SSP1STATbits.R_nW)
    {
        if((1 == SSP1STATbits.D_nA) && (1 == SSP1CON2bits.ACKSTAT))
        {
            // callback routine can perform any post-read processing
            I2C1_StatusCallback(I2C1_SLAVE_READ_COMPLETED, i2c_data);
        }
        else
        {
            // callback routine should write data into SSPBUF
            I2C1_StatusCallback(I2C1_SLAVE_READ_REQUEST, i2c_data);
        }
    }
    else if(0 == SSP1STATbits.D_nA)
    {
        // this is an I2C address

        // callback routine should prepare to receive data from the master
        I2C1_StatusCallback(I2C1_SLAVE_WRITE_REQUEST, i2c_data);
    }
    else
    {
        I2C1_slaveWriteData   = i2c_data;

        // callback routine should process I2C1_slaveWriteData from the master
        I2C1_StatusCallback(I2C1_SLAVE_WRITE_COMPLETED, i2c_data);
    }

    SSP1CON1bits.CKP    = 1;    // release SCL

} // end I2C1_ISR()



/**

 * Performs I2C Interupts states
*/

void I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS i2c_bus_state,  uint8_t dataI2C )
{
    switch (i2c_bus_state)
    {
        case I2C1_SLAVE_WRITE_REQUEST:
            1+1;
        break;


        case I2C1_SLAVE_WRITE_COMPLETED:
            LastI2C1_CMD = dataI2C;
            switch (LastI2C1_CMD){
            
                case (AliveCheck):
                    DEBUG_LED_SetHigh();
                    PutFloatInto_rVar(DEV_ID, LastI2C1_CMD);
                break;
            }
            
        break;

        case I2C1_SLAVE_READ_REQUEST:
            //READING BUFFERS, when OBC makes a read request to for certain bytes of last CMD
            switch (LastI2C1_CMD){
                case rVAR_u_c: //To Load data to SSPBUF so OBC can collect data from most recent read request
                    SSP1BUF = rVAR_u;
                break;
                case rVAR_0: //To Load data to SSPBUF so OBC can collect data from most recent read request
                    SSP1BUF = rVAR[0];
                    rVAR[0] = 0;
                break;
                case rVAR_1: //To Load data to SSPBUF so OBC can collect data from most recent read request
                    SSP1BUF = rVAR[1];
                    rVAR[1] = 0;
                break;
                case rVAR_2: //To Load data to SSPBUF so OBC can collect data from most recent read request
                    SSP1BUF = rVAR[2];
                    rVAR[2] = 0;
                break;
                case rVAR_3: //To Load data to SSPBUF so OBC can collect data from most recent read request
                    SSP1BUF = rVAR[3];
                    rVAR[3] = 0;
                break;
                case WriteMTQVoltage_Right: //To Load data to SSPBUF so OBC can collect data from most recent read request
                    SSP1BUF = wMTQv[0];
                    wVAR_u = 0x00;
                break;
               
                default:
                    SSP1BUF = LastI2C1_CMD;
                break;
            }
            break;

        case I2C1_SLAVE_READ_COMPLETED:
            1+1;
        break;
        
        default:
            1+1;
            break;

    } // end switch(i2c_bus_state)

}
    

