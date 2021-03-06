/**
  MSSP1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    i2c1.h

  @Summary
    This is the generated header file for the MSSP1 driver using MPLAB(c) Code Configurator

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

#ifndef _I2C1_H
#define _I2C1_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  I2C Slave Driver Status

  @Summary
    Defines the different status that the slave driver has
    detected over the i2c bus.

  @Description
    This defines the different status that the slave driver has
    detected over the i2c bus. The status is passed to the
    I2C1_StatusCallback() callback function that is implemented by
    the user of the slave driver as a parameter to inform the user
    that there was a change in the status of the driver due to
    transactions on the i2c bus. User of the slave driver can use these
    to manage the read or write buffers.

 */
        
#define I2C1_SLAVE_ADDRESS 0x22 
#define I2C1_SLAVE_MASK    0x7F

        
        //Driver I2C Commands
#define I2Cdelay_ms 2
#define AliveCheck 0xFF
#define rVAR_u_c 0x25
#define rVAR_0 0x01
#define rVAR_1 0x02
#define rVAR_2 0x03
#define rVAR_3 0x04
#define wVAR_0 0x05
#define wVAR_1 0x06
#define wVAR_2 0x07
#define wVAR_3 0x08
#define ReadSunSensorVoltage_Right 0x20
#define ReadMTQVoltage_Right 0x23
#define WriteMTQVoltage_Right 0x24

#define uGx 0x52
#define uGy 0x62
#define uGz 0x72
#define uMx 0x82
#define uMy 0x92
#define uMz 0xA2

#define rGx 0x81
#define rGy 0x91
#define rGz 0xA1
#define rMx 0xB1
#define rMy 0xC1
#define rMz 0xD1
#define rAx 0x22
#define rAy 0x32
#define rAz 0x42
//Driver I2C Commands END
        
        
        
        
        
        
        
        
        
        
        
        
        
        

typedef enum
{
    I2C1_SLAVE_WRITE_REQUEST,
    I2C1_SLAVE_READ_REQUEST,
    I2C1_SLAVE_WRITE_COMPLETED,
    I2C1_SLAVE_READ_COMPLETED,
} I2C1_SLAVE_DRIVER_STATUS;

#define I2C1_SLAVE_DEFAULT_ADDRESS          0x22

/**
    @Summary
        Initializes and enables the i2c slave instance : 1

    @Description
        This routine initializes the i2c slave driver instance for : 1
        index, making it ready for clients to open and use it.

    @Preconditions
        None

    @Param
        None

    @Returns
        None

    @Example
        <code>
            // initialize the i2c slave driver
            I2C1_Initialize();

        </code>
*/

void I2C1_Initialize(void);

/**
   @Summary
        This function process the I2C interrupts generated by
        bus activity

    @Description
        This function calls a callback function with 1 of 4
        possible parameters.
            I2C1_SLAVE_WRITE_REQUEST
            I2C1_SLAVE_READ_REQUEST
            I2C1_SLAVE_WRITE_COMPLETED
            I2C1_SLAVE_READ_COMPLETED

        The callback function should contain application specific
        code to process I2C bus activity from the I2C master.
        A basic EEPROM emulator is provided as an example.
 */

void I2C1_ISR ( void );
/**
   @Summary
        This varible contains the last data written to the I2C slave
*/

extern volatile uint8_t    I2C1_slaveWriteData;

void PerformOBCI2C_CMD (uint8_t i2c_data);

uint8_t OBC_LastCMD = 0x00;
uint8_t rVAR[4], rVAR_u, wVAR[4], wVAR_u = 0x00;
uint8_t wMTQv[6]; //0.Right, 1.Left, 2.Front, 3.Back, 4.Top, 5.Bottom
uint8_t WriteCMD = 0;

void SerialDataExchange (uint8_t Serial_CMD);
void putinrVAR ();

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  // _I2C1_H
