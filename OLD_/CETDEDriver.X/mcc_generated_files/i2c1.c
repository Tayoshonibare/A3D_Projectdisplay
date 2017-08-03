#include <xc.h>
#include "i2c1.h"
#include "pin_manager.h"
//#include "adc.h"
#include "mcc.h"
#include "eusart1.h"
#include "../DESCENT_Variales.h"






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
void I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS i2c_bus_state, uint8_t dataI2C);



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
    // SMP High Speed; CKE disabled; 
    SSP1STAT = 0x00;
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
    i2c_data = SSP1BUF;  // read SSPBUF to clear BF
       if(1 == SSP1STATbits.R_nW) {
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
        I2C1_StatusCallback(I2C1_SLAVE_WRITE_COMPLETED, i2c_data);
        
    }

    SSP1CON1bits.CKP    = 1;    // release SCL

} // end I2C1_ISR()



void I2C1_StatusCallback(I2C1_SLAVE_DRIVER_STATUS i2c_bus_state,  uint8_t dataI2C ){
     uint16_t SensorValue_16;
    switch (i2c_bus_state) {
        case I2C1_SLAVE_WRITE_REQUEST:
            break;

        case I2C1_SLAVE_WRITE_COMPLETED:
            OBC_LastCMD = dataI2C;
            switch (OBC_LastCMD){
                 case AliveCheck:   
                    //float DevID = 2019;
                    torVAR(2019);
                    //SerialDataExchange (AliveCheck);
                    rVAR_u = AliveCheck;
                break;
                
                case ReadSunSensorVoltage_Right:
                   rVAR_u = ReadSunSensorVoltage_Right;
                   int16_t val = 1;//ADC_GetConversion(SunSensorR);
                   rVAR[1] = ((int) val & 0xFFFF) >> 14;
                   rVAR[0] = ( (int) val & 0x3FFF) >> 6;
                break;
                
                case ReadMTQVoltage_Right:
                   rVAR_u = ReadMTQVoltage_Right; //Update last read command
                   int16_t val = 1;//(int16_t) ADC_GetConversion(MTQVoltageR);
                   val = (val & 0xFFC0) >> 6; // LSB first to MSB first
                   
                   float R1 = 4.7, R2 = 6.8; //Voltage dividing resistors on channel
                   int16_t val16 = (float) ((val/1023.00)*3.33)*((R1+R2)/R2) * ((32768.00)/5.5) ;
                   rVAR[1] = (int) val16 >> 8;
                   rVAR[0] = (int) val16;
                break;
                
                case WriteMTQVoltage_Right:
                    wVAR_u = WriteMTQVoltage_Right; //Update last write command
                    WriteCMD = 1;
                break; 
                
                /***********GYRO READS BEGIN***************/
                case uGx:
                    LastSerialCMD = uGx;
                    rVAR_u = uGx;
                    SERIAL_BUFFcount = 0;
                    EUSART1_Write(LastSerialCMD);            
                break;
                 case rGx:
                    LastSerialCMD = rGx;
                    rVAR_u = rGx;
                    putinrVAR();
                break;
                 case uGy:
                    LastSerialCMD = uGy;
                    rVAR_u = uGy;
                    SERIAL_BUFFcount = 0;
                    EUSART1_Write(LastSerialCMD);            
                break;
                 case rGy:
                    LastSerialCMD = rGy;
                    rVAR_u = rGy;
                    putinrVAR();
                break;
                 case uGz:
                    LastSerialCMD = uGz;
                    rVAR_u = uGz;
                    SERIAL_BUFFcount = 0;
                    EUSART1_Write(LastSerialCMD);            
                break;
                 case rGz:
                    LastSerialCMD = rGz;
                    rVAR_u = rGz;
                    putinrVAR();
                break;
                /***********GYRO READS END*****************/
                
                /***********MAG READS BEGIN***************/
                case uMx:
                    LastSerialCMD = uMx;
                    rVAR_u = uMx;
                    SERIAL_BUFFcount = 0;
                    EUSART1_Write(LastSerialCMD);            
                break;
                 case rMx:
                    LastSerialCMD = rMx;
                    rVAR_u = rMx;
                    putinrVAR();
                break;
                 case uMy:
                    LastSerialCMD = uMy;
                    rVAR_u = uMy;
                    SERIAL_BUFFcount = 0;
                    EUSART1_Write(LastSerialCMD);            
                break;
                 case rMy:
                    LastSerialCMD = rMy;
                    rVAR_u = rMy;
                    putinrVAR();
                break;
                
                 case uMz:
                    LastSerialCMD = uMz;
                    rVAR_u = uMz;
                    SERIAL_BUFFcount = 0;
                    EUSART1_Write(LastSerialCMD);            
                break;
                 case rMz:
                    LastSerialCMD = rMz;
                    rVAR_u = rMz;
                    putinrVAR();
                break;
                /**********MAG READS END***************/
                
                
                
                
            }
            
            break;

        case I2C1_SLAVE_READ_REQUEST: 
            //READING BUFFERS
            switch (OBC_LastCMD){
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
                    SSP1BUF = OBC_LastCMD;
                break;
            }
            break;

        case I2C1_SLAVE_READ_COMPLETED:
            break;
        default:;

    } // end switch(i2c_bus_state)

}


void torVAR (float data){
    union {
        float FloatValue;
        unsigned char bytesOfFloatValue[4];
        } floatUnion;
                        
        floatUnion.FloatValue = data;
                        
        uint8_t xount =0;
        for (xount=0; xount<4; ++xount){
            rVAR[xount] = floatUnion.bytesOfFloatValue[xount];
        }
}

void putinrVAR (){
    SerialLast.BYTESarray[0] = SERIAL_BUFF[0];
    SerialLast.BYTESarray[1] = SERIAL_BUFF[1];
    SerialLast.BYTESarray[2] = SERIAL_BUFF[2];
    SerialLast.BYTESarray[3] = SERIAL_BUFF[3];
                    
    //Update I2C Registery
    rVAR[0] = SerialLast.BYTESarray[1]; //0x41
    rVAR[1] = SerialLast.BYTESarray[2]; //0xA1
    rVAR[2] = SerialLast.BYTESarray[3]; //0x85
    rVAR[3] = SerialLast.BYTESarray[0]; //0x1F

}
