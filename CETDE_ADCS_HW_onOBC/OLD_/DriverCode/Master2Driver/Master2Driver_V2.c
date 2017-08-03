#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <smbus.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

//Driver I2C Commands
#define Driver_address 0x22
#define ALIVECHECK_CMD 	0x02
#define getGYROXA_CMD 	0x03
#define getGYROYA_CMD 	0x04
#define getGYROZA_CMD 	0x05
#define getGYROXB_CMD 	0x06
#define getGYROYB_CMD 	0x07
#define getGYROZB_CMD 	0x08
#define	getSunSensor_R_CMD	0x09
#define	getSunSensor_L_CMD	0x0A
#define	getSunSensor_BA_CMD	0x0B
#define	getSunSensor_F_CMD	0x0C
#define	getSunSensor_T_CMD	0x0D
#define	getSunSensor_Bu_CMD	0x0E


//Global Variable Decelarations
int err = 0; //Global Error flag
int errorLEDpin =4;

//General Function Decelarations
void LEDdetectError(int errortag, int ledblinkcycles);
void LEDerror_indicator (void);
void LED_indicator (int blinkcycle);

//I2C Driver Decelarations
int driver_i2c_Begin();
void driver_i2c_Write_Byte(int fd, __u8 address, __u8 value);



int Driver_fd;//file descriptor


int main (void){
	printf ("\n\nStarting Demonstraion Program...\n\n");
	wiringPiSetup () ;
	pinMode (errorLEDpin, OUTPUT) ; //LED used to debug
	
	printf("\nInitiating I2C bus on address = 0x%x", Driver_address );
	Driver_fd = driver_i2c_Begin();
	LEDdetectError(Driver_fd, 1);
	
	
	
	close (Driver_fd); printf ("\n");
	return (0);
}

// Open a connection to the Driver, Returns a file id
int driver_i2c_Begin(){
	int fd;
	char *fileName = "/dev/i2c-1";
	
	// Open port for reading and writing
	if ((fd = open(fileName, O_RDWR)) < 0)
		exit(1);
	
	// Set the port options and set the address of the device
	if (ioctl(fd, I2C_SLAVE, Driver_address) < 0) {					
		close(fd);
		exit(1);
	}
	
	printf("\nI2C Initialization sucessful on address = 0x%x\n", Driver_address );
	return fd;
}


//General Function  Implementations below
void LEDdetectError(int errortag, int ledblinkcycles){
// If therre is an error LED stays ON, else blinks the number of blink cycle times
	if ( errortag == -1) {
		while (1){
			digitalWrite (errorLEDpin, HIGH);
		}
	}
	else {
		int i=0;
	for (i=0; i<ledblinkcycles; ++i){
		digitalWrite (errorLEDpin, HIGH); delay(500);
		digitalWrite (errorLEDpin, LOW); delay(500);
	}
	digitalWrite (errorLEDpin, HIGH); delay(1000);
	digitalWrite (errorLEDpin, LOW);
	}
}

//Write a byte to the driver
void driver_i2c_Write_Byte(int fd, __u8 address, __u8 value)
{
	if (i2c_smbus_write_byte_data(fd, address, value) < 0) {
		close(fd);
		exit(1);
	}
}



