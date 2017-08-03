/* 
 * File:   DESCENT_Variales.h
 * Author: HyperloopDWS
 *
 * Created on May 29, 2017, 7:49 AM
 */

#ifndef DESCENT_VARIALES_H
#define	DESCENT_VARIALES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    bool SerialCMDDone = 1;
    
    union {
        uint8_t BYTESarray [4];
        uint32_t floatValue;
    } SerialLast;
    
    union {
        uint32_t FloatValue;
        unsigned char bytesOfFloatValue[4];
    } floatUnion;
    
    void torVAR (float data);
    uint8_t LastSerialCMD = 0x2A;
    
    uint8_t SERIAL_BUFF[6], SERIAL_BUFFcount;
    
    uint8_t CheckXORchecksum(uint8_t A, uint8_t B, uint8_t C, uint8_t D);
    
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* DESCENT_VARIALES_H */

