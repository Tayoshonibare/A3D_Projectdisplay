#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

//Driver I2C Commands
#define I2Cdelay_ms 2
#define AliveCheck 0xFF
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

#define rGx 0x81
#define rGy 0x91
#define rGz 0xA1
#define rMx 0xB1
#define rMy 0xC1
#define rMz 0xD1
#define rAx 0x22
#define rAy 0x32
#define rAz 0x42




//Global Variable Decelarations
uint8_t rVAR[4] = {0x01, 0x02, 0x03, 0x04};
int err = 0; //Global Error flag
int errorLEDpin =4;

//General Function Decelarations
void LEDdetectError(int errortag, int ledblinkcycles);
void LEDerror_indicator (void);
void LED_indicator (int blinkcycle);

//I2C Driver Decelarations
float ReadCommand(uint8_t Command);
int MTQWriteCommand (uint8_t Command, float voltage);
void AliveCheck_fun();
void ReadSunSensorVoltage_Right_fun();
void ReadMTQVoltage_Right_fun();
void WriteMTQVoltage_Right_fun();
void ReadGyro_x();
void ReadGyro_y();
void ReadGyro_z();
void ReadMag_x();
void ReadMag_y();
void ReadMag_z();


int Driver_fd;//file descriptor
int Driver_address = 0x22;


int main (void){
	printf("-----------------------------------------------------------");
	printf ("\n\nSTARTING DEMO PROGRAM...\n");
	wiringPiSetup () ;
	pinMode (errorLEDpin, OUTPUT) ; //LED used to debug
	
	//Setup I2C bus
	printf("\nInitiating I2C bus on address = 0x%x", Driver_address ) ;
	Driver_fd = wiringPiI2CSetup (Driver_address); //Set Slave I2C address of driver
	LEDdetectError(Driver_fd, 1);
	
	//Call some I2C commands for demonstration
	AliveCheck_fun();
	//ReadSunSensorVoltage_Right_fun();
	//ReadMTQVoltage_Right_fun();
	//WriteMTQVoltage_Right_fun();
	/*************READ GYROS******************/
	ReadGyro_x();
	ReadGyro_y();
	ReadGyro_z();
	printf("\n-----------------------------");
	/***********READ Magnetometers***********/
	ReadMag_x();
	ReadMag_y();
	ReadMag_z();
	printf("\n-----------------------------");
	
	
	
	printf("\n-----------------------------------------------------------");
	printf ("\n\n");
	return 0 ;
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

void ReadSunSensorVoltage_Right_fun(){
	int value = ReadCommand(ReadSunSensorVoltage_Right);
	float valuef = ((float)value/1024.00) * 3.33;
	printf ("\nRight Sun Sensor Analog = %d/%d", value, (int)pow(2,10));
	printf ("\t\tRight Sun Sensor Voltage is = %f", valuef);
}
void ReadMTQVoltage_Right_fun(){
	int value = ReadCommand(ReadMTQVoltage_Right);
	float valuef = ((float)value/pow(2,15)) * 5.5;
	printf ("\nRight MTQ Analog  = %d/%d", value, (int)pow(2,15));
	printf ("\t\t\tRight MTQ Voltage  = %f", valuef);
}
void WriteMTQVoltage_Right_fun(){
	int value = MTQWriteCommand(WriteMTQVoltage_Right, 2.2);
	printf ("\nRight MTQ Voltage is set to =  %d/%d", value,(int)pow(2,8));
	float valuef = ((float)value/pow(2,8)) * 5.5;
	printf ("\t\tRight MTQ Voltage is set to = %f", valuef);
}

/**********READ GYRO FUNCTIONS*************/
void ReadGyro_x () {
	float rGx_rps = ReadCommand(rGx);
	printf ("\nGyro X  = %f rad/sec", rGx_rps);	
}
void ReadGyro_y () {
	float rGy_rps = ReadCommand(rGy);
	printf ("\nGyro Y  = %f rad/sec", rGy_rps);	
}
void ReadGyro_z () {
	float rGz_rps = ReadCommand(rGz);
	printf ("\nGyro Z  = %f rad/sec", rGz_rps);	
}
/**********READ Magnetometer FUNCTIONS*************/
void ReadMag_x () {
	float rMx_ut = ReadCommand(rMx);
	printf ("\nMTM X  = %f uT", rMx_ut);	
}
void ReadMag_y () {
	float rMy_ut = ReadCommand(rMy);
	printf ("\nMTM Y  = %f uT", rMy_ut);	
}
void ReadMag_z () {
	float rMz_ut = ReadCommand(rMz);
	printf ("\nMTM Z  = %f uT", rMz_ut);	
}



/**********FUNDERMENTAL FUNCTIONS*************/

float ReadCommand (uint8_t Command){
	wiringPiI2CWrite (Driver_fd, Command); //Wirte command to make driver ready
	unsigned char ret[4];
	int i = 0;
	for (i=0; i<4; ++i){
		wiringPiI2CWrite (Driver_fd, rVAR[i]); delay(I2Cdelay_ms); //Send read buffer command
		ret[i] = wiringPiI2CRead (Driver_fd); delay(I2Cdelay_ms);
		//printf ("\nDATA  = %02x", ret[i]);
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

int MTQWriteCommand (uint8_t Command, float voltage){
	wiringPiI2CWrite (Driver_fd, Command); //Send MTQ write cmd to driver to get it ready
	uint8_t MTQvolt = (voltage/5.5) * pow(2,8);
	
	wiringPiI2CWrite (Driver_fd, MTQvolt); delay(I2Cdelay_ms); //Send read buffer command
	wiringPiI2CWrite (Driver_fd, Command); 
	int f = wiringPiI2CRead (Driver_fd); delay(I2Cdelay_ms);
	

	return f;
}	





