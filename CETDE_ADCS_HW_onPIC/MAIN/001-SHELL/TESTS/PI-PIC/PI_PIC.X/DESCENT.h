/* 
 * File:   DESCENT.h
 * Author: HyperloopDWS
 *
 * Created on June 26, 2017, 7:07 AM
 */

#ifndef DESCENT_H
#define	DESCENT_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

    
    
float DEV_ID = 2019.00;    
    
    
//Driver I2C Commands
#define I2Cdelay_ms 2
#define BIG_I2Cdelay_ms 6
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


//I2C1 Globals
uint8_t LastI2C1_CMD = 0x00;
uint8_t rVAR[4];
uint8_t rVAR_u;
uint8_t wVAR[4];
uint8_t wVAR_u;
uint8_t wMTQv[6]; //0.Right, 1.Left, 2.Front, 3.Back, 4.Top, 

union {
    uint8_t BYTESarray [4];
    uint32_t floatValue;
} SerialLast;

void PutFloatInto_rVar (float data, uint8_t LastCMD);

    
    
    
    
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* DESCENT_H */

