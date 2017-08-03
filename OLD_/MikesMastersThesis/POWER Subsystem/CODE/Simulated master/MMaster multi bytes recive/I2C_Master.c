#include "p24fj64ga002.h"
// the above include path may be different for each user.  If a compile
// time error appears then check the path for the file above and edit
// the include statement above.

#include<i2c.h>


#define XTFREQ          7372800         		//On-board Crystal frequency
#define PLLMODE         2               		//On-chip PLL setting
#define FCY             XTFREQ*PLLMODE        //Instruction Cycle Frequency
#define BAUDRATE         9600       
#define BRGVAL          ((FCY/BAUDRATE)/16)-1 
#define I2CBAUD			400000				// i2c clock running at 400khz


//_CONFIG1(JTAGEN_OFF&GCP_OFF&GWRP_OFF  &COE_OFF& ICS_PGx1  &FWDTEN_OFF );
//_CONFIG2(IESO_OFF &FNOSC_PRIPLL&FCKSM_CSDCMD&OSCIOFNC_OFF &I2C1SEL_SEC &POSCMOD_XT );

//
char buffins[100]="testing123";

void delay(void)
{
	int var1,var2;
	for(var1=0;var1!=100;var1++)
	{
		for(var2=0;var2!=10000;var2++);
	}
}

void solarcheck(void);
void commcheck(void);
void battcheck(void);
void UARTwritestring(char *stooring);
void config_initialize_UART(void);
void main(void)
{
	CLKDIVbits.RCDIV = 0;
	RPINR18bits.U1RXR = 9;	// Make Pin RP9 U1RX
	RPOR4bits.RP8R = 3;		// Make Pin RP8 U1TX

	TRISB = 0x0300;
	U1BRG  = BRGVAL;
	U1MODE = 0x8000; 		// Reset UART to 8-n-1, alt pins, and enable 
	U1STA  = 0x0440; 		// Reset status register and enable TX & RX


	PADCFG1 = 0xFF;			// Make analog pins digital 
	LATB  = 0xF000;			//Toggle LED's
	TRISB = 0x00;			// Configure LED pins as output
  
	unsigned int config2, config1;
  	//unsigned char *wrptr;
  	//unsigned char tx_data[] = {0x00,0xFF,0x00,0xFF,};
//	char check=0,ZZ=90;  
//	unsigned int checksum;
//	unsigned char i=0,X;
	
//	char Datain[20];
//	unsigned char ctrlbyte ='Z';
//	unsigned char length_msb='Z', length_lsb='Z',check_msb='Z',check_lsb='Z';
//	unsigned int length, checkcalc=0, checkreceived, lasterror=0;

  /* Baud rate is set for 100 Khz */
  	config2 = 0x90;
  /* Configure I2C for 7 bit address mode */
	config1 = 0xF200; //(I2C_ON & I2C_IDLE_CON & I2C_CLK_HLD &
//             	I2C_IPMI_DIS & I2C_7BIT_ADD &
//             	I2C_SLW_DIS & I2C_SM_DIS &
//            	I2C_GCALL_DIS & I2C_STR_DIS &
//             	I2C_NACK & I2C_ACK_DIS & I2C_RCV_DIS &
//             	I2C_STOP_DIS & I2C_RESTART_DIS &
//             	I2C_START_DIS);
	OpenI2C2(config1,config2); 	

config_initialize_UART();
UARTwritestring(buffins);  	
while(1){ 
commcheck();
battcheck();
solarcheck();


   }
return;
}

void commcheck(void){
	char check=0,ZZ=90;  
	unsigned int checksum;
	unsigned char i=0,X;
	
	unsigned char Datain[20];
	unsigned char ctrlbyte ='Z';
	unsigned char length_msb='Z', length_lsb='Z',check_msb='Z',check_lsb='Z';
	unsigned int length, checkcalc=0, checkreceived, lasterror=0;

	IdleI2C2();
	StartI2C2();
    /* Wait till Start sequence is completed */
 	while(I2C2CONbits.SEN );
    /* Write Slave address and set master for transmission */
 	check = MasterWriteI2C2(0xE6); 
    /* Wait till address is transmitted */
	if (check==0 ){
		checksum=5;
		check_msb=checksum/256;
		check_lsb=checksum%256;
		LATB=0xA000;	
		IdleI2C2(); 		
		MasterWriteI2C2(0x00); // ctrlbyte 
		IdleI2C2();			
 		MasterWriteI2C2(0x00);// length_msb
		IdleI2C2();			
		MasterWriteI2C2(0x05);// length_lsb
		IdleI2C2();			
	    MasterWriteI2C2(check_msb);
		IdleI2C2();			
		MasterWriteI2C2(check_lsb);
		IdleI2C2();			
		StopI2C2();	
	}

delay();
//get12cdata ();

	IdleI2C2();
  	StartI2C2();
	while(I2C2CONbits.SEN );
	check = MasterWriteI2C2(0xE7);//  Write Slave address and set master for recive 
	if (check==0 ){
		LATB=0x5000;	
		IdleI2C2(); 
	
		ctrlbyte=	MasterReadI2C2();
 		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length_msb=MasterReadI2C2();  /* read the high byte of length */
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length_lsb=MasterReadI2C2();  /* read the low byte of sentence length*/
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length=length_msb*0x100+length_lsb;  /* calculate length for the data loop*/
   		if (length<20){                      /* check to see if its smaller then predefined size*/
			X=length_lsb-5;
      		for (i=0;i<(X);i++){
      			Datain[i]=MasterReadI2C2();        /*read in data bytes into an array*/
				AckI2C2();		
				while(I2C1CONbits.ACKEN == 1);
				IdleI2C2();
				checkcalc=checkcalc+Datain[i] ;    /*add them up as a checksum now;  to save a loop later*/
   			}
		
		}
		else{
	
			length_msb=0;
			length_lsb=8;
			Datain[0]='B';
			Datain[1]='A';
			Datain[2]='D';
		
		}
    	
		check_msb   =MasterReadI2C2();  /* read the high byte of checksum*/
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();
	    check_lsb   =MasterReadI2C2(); /* read the low byte of checksum*/
		NotAckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();
		StopI2C2();
  	
	}		
	else{
		ZZ=-check;//0x02;
		LATB=0x0000;
		CloseI2C2();
	}
		
	while(!U1STAbits.TRMT);
	U1TXREG = '"';
	while(!U1STAbits.TRMT);		// Echo Back Received Character with quotes
	U1TXREG = ctrlbyte;
//	while(!U1STAbits.TRMT);
//	U1TXREG = length_msb;
//	while(!U1STAbits.TRMT);
//	U1TXREG = length_lsb; 	

for(i=0;i<length_lsb-5;i++){
	while(!U1STAbits.TRMT);
	U1TXREG = Datain[i]; 		
	}
	while(!U1STAbits.TRMT);
	U1TXREG = '"'; 
//	while(!U1STAbits.TRMT);
//	U1TXREG = check_msb; 	
//	while(!U1STAbits.TRMT);
//	U1TXREG = check_lsb; 

	while(!U1STAbits.TRMT);
	U1TXREG = 10; 
	while(!U1STAbits.TRMT);
	U1TXREG = 13; 
	delay();

}
void solarcheck(void)
{
	char check=0,ZZ=90;  
	unsigned int checksum;
	unsigned char i=0,X;
	
	unsigned char Datain[20];
	unsigned char ctrlbyte ='Z';
	unsigned char length_msb='Z', length_lsb='Z',check_msb='Z',check_lsb='Z';
	unsigned int length, checkcalc=0, checkreceived, lasterror=0;

	IdleI2C2();
	StartI2C2();
    /* Wait till Start sequence is completed */
 	while(I2C2CONbits.SEN );
    /* Write Slave address and set master for transmission */
 	check = MasterWriteI2C2(0xE6); 
    /* Wait till address is transmitted */
	if (check==0 ){
		checksum=9;
		check_msb=checksum/256;
		check_lsb=checksum%256;
		LATB=0xA000;	
		IdleI2C2(); 		
		MasterWriteI2C2(0x04); // ctrlbyte 
		IdleI2C2();			
 		MasterWriteI2C2(0x00);// length_msb
		IdleI2C2();			
		MasterWriteI2C2(0x05);// length_lsb
		IdleI2C2();			
	    MasterWriteI2C2(check_msb);
		IdleI2C2();			
		MasterWriteI2C2(check_lsb);
		IdleI2C2();			
		StopI2C2();	
	}
delay();
//get12cdata ();

	IdleI2C2();
  	StartI2C2();
	while(I2C2CONbits.SEN );
	check = MasterWriteI2C2(0xE7);//  Write Slave address and set master for recive 
	if (check==0 ){
		LATB=0x5000;	
		IdleI2C2(); 
	
		ctrlbyte=	MasterReadI2C2();
 		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length_msb=MasterReadI2C2();  /* read the high byte of length */
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length_lsb=MasterReadI2C2();  /* read the low byte of sentence length*/
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length=length_msb*0x100+length_lsb;  /* calculate length for the data loop*/
   		if (length<20){                      /* check to see if its smaller then predefined size*/
			X=length_lsb-5;
      		for (i=0;i<(X);i++){
      			Datain[i]=MasterReadI2C2();        /*read in data bytes into an array*/
				AckI2C2();		
				while(I2C1CONbits.ACKEN == 1);
				IdleI2C2();
				checkcalc=checkcalc+Datain[i] ;    /*add them up as a checksum now;  to save a loop later*/
   			}
		
		}
		else{
	
			length_msb=0;
			length_lsb=8;
			Datain[0]='B';
			Datain[1]='A';
			Datain[2]='D';
		
		}
    	
		check_msb   =MasterReadI2C2();  /* read the high byte of checksum*/
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();
	    check_lsb   =MasterReadI2C2(); /* read the low byte of checksum*/
		NotAckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();
		StopI2C2();
  	
	}		
	else{
		ZZ=-check;//0x02;
		LATB=0x0000;
		CloseI2C2();
	}
unsigned int temp;	
double sc1=0.00,sc2=0.00,sc3=0.00,sc4=0.00,sc5=0.00,sc6=0.00,volt=0.00;
	
	

	temp=  (unsigned int) Datain[0]*0x100+ (unsigned int) Datain[1];
	sc1=(double) temp* 0.05035400390625; //3.3/1024.0/.064

	temp=  (unsigned int) Datain[2]*0x100+ (unsigned int) Datain[3];
	sc2=(double) temp* 0.05035400390625; //3.3/1024.0/.064
	
	temp=  (unsigned int) Datain[4]*0x100+ (unsigned int) Datain[5];
	sc3=(double) temp* 0.05035400390625; //3.3/1024.0/.064
	
	temp=  (unsigned int) Datain[6]*0x100+ (unsigned int) Datain[7];
	sc4=(double) temp* 0.05035400390625; //3.3/1024.0/.064
	
	temp=  (unsigned int) Datain[8]*0x100+ (unsigned int) Datain[9];
	sc5=(double) temp* 0.05035400390625; //3.3/1024.0/.064
	
	temp=  (unsigned int) Datain[10]*0x100+ (unsigned int) Datain[11];
	sc6=(double) temp* 0.05035400390625; //3.3/1024.0/.064


	temp=   (unsigned int) Datain[12]*0x100 +  (unsigned int) Datain[13];
	volt=(double)temp* 0.0064453125; //3.3/1024.0*2


sprintf(buffins, "SC1:%5.3f SC2:%5.3f SC3:%5.3f SC4:%5.3f SC5:%5.3f ,SC6:%5.3f SolarVolt:%5.3f\r\n",sc1,sc2,sc3,sc4,sc5,sc6,volt);
UARTwritestring(buffins); 
delay();
 
}


void battcheck(void){
	char check=0,ZZ=90;  
	unsigned int checksum;
	unsigned char i=0,X;
	
	unsigned char Datain[20];
	unsigned char ctrlbyte ='Z';
	unsigned char length_msb='Z', length_lsb='Z',check_msb='Z',check_lsb='Z';
	unsigned int length, checkcalc=0, checkreceived, lasterror=0;

	IdleI2C2();
	StartI2C2();
    /* Wait till Start sequence is completed */
 	while(I2C2CONbits.SEN );
    /* Write Slave address and set master for transmission */
 	check = MasterWriteI2C2(0xE6); 
    /* Wait till address is transmitted */
	if (check==0 ){
		checksum=13;
		check_msb=checksum/256;
		check_lsb=checksum%256;
		LATB=0xA000;	
		IdleI2C2(); 		
		MasterWriteI2C2(0x08); // ctrlbyte 
		IdleI2C2();			
 		MasterWriteI2C2(0x00);// length_msb
		IdleI2C2();			
		MasterWriteI2C2(0x05);// length_lsb
		IdleI2C2();			
	    MasterWriteI2C2(check_msb);
		IdleI2C2();			
		MasterWriteI2C2(check_lsb);
		IdleI2C2();			
		StopI2C2();	
			}

delay();
//get12cdata ();

	IdleI2C2();
  	StartI2C2();
	while(I2C2CONbits.SEN );
	check = MasterWriteI2C2(0xE7);//  Write Slave address and set master for recive 
	if (check==0 ){
		LATB=0x5000;	
		IdleI2C2(); 
	
		ctrlbyte=	MasterReadI2C2();
 		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length_msb=MasterReadI2C2();  /* read the high byte of length */
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length_lsb=MasterReadI2C2();  /* read the low byte of sentence length*/
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();

		length=length_msb*0x100+length_lsb;  /* calculate length for the data loop*/
   		if (length<20){                      /* check to see if its smaller then predefined size*/
			X=length_lsb-5;
      		for (i=0;i<(X);i++){
      			Datain[i]=MasterReadI2C2();        /*read in data bytes into an array*/
				AckI2C2();		
				while(I2C1CONbits.ACKEN == 1);
				IdleI2C2();
				checkcalc=checkcalc+Datain[i] ;    /*add them up as a checksum now;  to save a loop later*/
   			}
		
		}
		else{
	
			length_msb=0;
			length_lsb=8;
			Datain[0]='B';
			Datain[1]='A';
			Datain[2]='D';
		
		}
    	
		check_msb   =MasterReadI2C2();  /* read the high byte of checksum*/
		AckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();
	    check_lsb   =MasterReadI2C2(); /* read the low byte of checksum*/
		NotAckI2C2();		
		while(I2C1CONbits.ACKEN == 1);
		IdleI2C2();
		StopI2C2();
  	
	}		
	else{
		ZZ=-check;//0x02;
		LATB=0x0000;
		CloseI2C2();
	}
		
	unsigned int temp;
	double Battvolt=0.00,Battdraw=0.00;
	
	temp=   (unsigned int) Datain[1]*0x100 +  (unsigned int) Datain[2];
	Battvolt=(double)temp* 0.0064453125; //3.3/1024.0*2
	temp=  (unsigned int) Datain[3]*0x100+ (unsigned int) Datain[4];
	Battdraw=(double) temp* 0.05035400390625; //3.3/1024.0/.064

	sprintf(buffins, "Charge Status:Off BattVolt:%5.3f BattDraw:%5.3f\r\n",Battvolt, Battdraw);	
	UARTwritestring(buffins);
	delay();
}


void config_initialize_UART(void)
{
	unsigned int baudvalue;
	unsigned int U1MODEvalue;
	unsigned int U1STAvalue;

	CloseUART1();

	baudvalue  = BRGVAL;
	U1MODEvalue = 0x8000; 		// Reset UART to 8-n-1, alt pins, and enable 
	U1STAvalue  = 0x0440; 		// Reset status register and enable TX & RX


//	ConfigIntUART1 (UART_RX_INT_DIS&UART_TX_INT_DIS);
//	U1MODEvalue =UART_EN & UART_IDLE_CON &UART_EN_WAKE& UART_DIS_LOOPBACK &	UART_DIS_ABAUD&UART_NO_PAR_8BIT &UART_1STOPBIT & UART_ALTRX_ALTTX;
//	U1STAvalue =UART_TX_PIN_NORMAL&UART_TX_ENABLE&UART_ADR_DETECT_DIS&UART_RX_OVERRUN_CLEAR;
//	OpenUART1 (U1MODEvalue, U1STAvalue, baudvalue);
	OpenUART1 (U1MODEvalue, U1STAvalue, baudvalue);
}


void UARTwritestring(char *stooring)
{
				int stooring_index=0;
				while(stooring[stooring_index]!=0){		//loop until null string terminator.
					WriteUART1(stooring[stooring_index++]);			//write charachter to the uart
					while(BusyUART1());					//spin until the uart is finished with the last char.
				};
}

