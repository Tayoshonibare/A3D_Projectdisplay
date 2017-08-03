#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

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




//Global Variable Decelarations
uint8_t rVAR[4] = {0x01, 0x02, 0x03, 0x04};
int err = 0; //Global Error flag
int errorLEDpin =4;

//General Function Decelarations
void LEDdetectError(int errortag, int ledblinkcycles);
void LEDerror_indicator (void);
void LED_indicator (int blinkcycle);

//I2C Driver Decelarations
void UpdateCommand (uint8_t Command);
float ReadCommand(uint8_t Command);
int MTQWriteCommand (uint8_t Command, float voltage);
void AliveCheck_fun();
float ReadGyro_x();
float ReadGyro_y();
float ReadGyro_z();
float ReadMag_x();
float ReadMag_y();
float ReadMag_z();

int Driver_fd;//file descriptor
int Driver_address = 0x22;


int main (void){
	wiringPiSetup () ;
	
	//Local Variables declarations
	unsigned int StartTime = millis();
	unsigned int LastSensorAskTime = millis();
	
	
	//Main program below
	printf("-----------------------------------------------------------");
	printf ("\n\nSTARTING DEMO PROGRAM...\n");
	pinMode (errorLEDpin, OUTPUT) ; //LED used to debug
	
	//Setup I2C bus
	printf("\nInitiating I2C bus on address = 0x%x", Driver_address ) ;
	Driver_fd = wiringPiI2CSetup (Driver_address); //Set Slave I2C address of driver
	LEDdetectError(Driver_fd, 1);
	
	//Call some I2C commands for demonstration
	AliveCheck_fun();
	printf ("\nSetup took: %d ms\n", millis() - StartTime);
	
	
	//Main loop below

	while (1==1){	
	/*************READ GYROS******************/
	
	printf ("%.5f, ", ReadGyro_x());
	printf ("%.5f, ", ReadGyro_y());
	printf ("%.5f, ", ReadGyro_z());
	printf(" || ");
	printf ("%.5f, ", ReadMag_x());
	printf ("%.5f, ", ReadMag_y());
	printf ("%.5f, ", ReadMag_z());
	printf(" || ");
	
	
	printf(", %d\n", millis() - LastSensorAskTime);
	
	
	LastSensorAskTime = millis();
	}
	
	
	
	printf("\n-----------------------------------------------------------");
	printf ("\n\n");
	return 0 ;
}




void AliveCheck_fun () {
	int VersionNumber = (int) ReadCommand(AliveCheck);
	printf ("\nAlive Check - Version Number = %d", VersionNumber);
	if (VersionNumber != 2019){
		printf ("\nIncorrect Version number, Could not connect to Driver...Driver not alive\n\n");
		exit(0);
	}
	printf("\t\tConnected to Driver");
	printf("\n-----------------------------------------------------------");
}


/**********READ GYRO FUNCTIONS*************/
float ReadGyro_x () {
	UpdateCommand(uGx);
	float rGx_rps = ReadCommand(rGx);
	return rGx_rps;
}
float ReadGyro_y () {
	UpdateCommand(uGy);
	float rGy_rps = ReadCommand(rGy);
	return rGy_rps;
}
float ReadGyro_z () {
	UpdateCommand(uGz);
	float rGz_rps = ReadCommand(rGz);	
	return rGz_rps;
}
/**********READ MAG FUNCTIONS*************/
float ReadMag_x () {
	UpdateCommand(uMx);
	float rMx_uT = ReadCommand(rMx);
	return rMx_uT;
}
float ReadMag_y () {
	UpdateCommand(uMy);
	float rMy_uT = ReadCommand(rMy);
	return rMy_uT;
}
float ReadMag_z () {
	UpdateCommand(uMz);
	float rMz_uT = ReadCommand(rMz);	
	return rMz_uT;
}




/**********FUNDERMENTAL FUNCTIONS*************/

float ReadCommand (uint8_t Command){
	delay(BIG_I2Cdelay_ms);
	
	//Send command to PIC driver
	wiringPiI2CWrite (Driver_fd, Command); delay(I2Cdelay_ms); //Wirte command to make driver ready


	//Get new data from I2C
	unsigned char ret[4];
	int i = 0;
	//printf ("\n");
	for (i=0; i<4; ++i){
		wiringPiI2CWrite (Driver_fd, rVAR[i]); delay(I2Cdelay_ms); //Send read buffer command
		ret[i] = wiringPiI2CRead (Driver_fd); delay(I2Cdelay_ms); 
		//printf ("%02x, ", ret[i]);
	}
	
	//Compile data from bytes to float
	union {
        float FloatValue;
        unsigned char bytesOfFloatValue[4];
    } floatUnion;
    
			/*floatUnion.bytesOfFloatValue[3] = 0x41;//ret[0];
			floatUnion.bytesOfFloatValue[2] = 0xa1;//ret[1];
			floatUnion.bytesOfFloatValue[1] = 0x85;//ret[2];
			floatUnion.bytesOfFloatValue[0] = 0x1F;//ret[3];
			printf ("\nFLOAT B  = %f", floatUnion.FloatValue);*/
 
			floatUnion.bytesOfFloatValue[3] = ret[2]; 
			floatUnion.bytesOfFloatValue[2] = ret[1]; 
			floatUnion.bytesOfFloatValue[1] = ret[0]; 
			floatUnion.bytesOfFloatValue[0] = ret[3]; 
			//printf ("\nFLOAT A  = %f", floatUnion.FloatValue);
			

			
	return (floatUnion.FloatValue);
}	





void UpdateCommand (uint8_t Command){
	//Send command to PIC driver
	delay(BIG_I2Cdelay_ms);
	wiringPiI2CWrite (Driver_fd, Command); //Wirte command 
	delay(I2Cdelay_ms); 
}	





int MTQWriteCommand (uint8_t Command, float voltage){
	wiringPiI2CWrite (Driver_fd, Command); //Send MTQ write cmd to driver to get it ready
	uint8_t MTQvolt = (voltage/5.5) * pow(2,8);
	
	wiringPiI2CWrite (Driver_fd, MTQvolt); delay(I2Cdelay_ms); //Send read buffer command
	wiringPiI2CWrite (Driver_fd, Command); 
	int f = wiringPiI2CRead (Driver_fd); delay(I2Cdelay_ms);
	

	return f;
}	



void LEDdetectError(int errortag, int ledblinkcycles){
// If therre is an error LED stays ON, else blinks the number of blink cycle times
	if ( errortag < 0) {
		while (1){
			digitalWrite (errorLEDpin, HIGH);
			printf ("\nError, Could not connrct to Driver");
			exit(0);
		}
	}
	else {
		int i=0;
	for (i=0; i<ledblinkcycles; ++i){
		digitalWrite (errorLEDpin, HIGH); delay(250);
		digitalWrite (errorLEDpin, LOW); delay(250);
	}
	digitalWrite (errorLEDpin, HIGH); delay(500);
	digitalWrite (errorLEDpin, LOW);
	}
}




