#include "DESCENT.h"


  

void PutFloatInto_rVar (float data, uint8_t LastCMD){
    union {
        float FloatValue;
        unsigned char bytesOfFloatValue[4];
        } floatUnion;
                        
        floatUnion.FloatValue = data;
                        
        uint8_t xount =0;
        for (xount=0; xount<4; ++xount){
            rVAR[xount] = floatUnion.bytesOfFloatValue[xount];
        }
        rVAR_u = LastCMD;
        
        
}
