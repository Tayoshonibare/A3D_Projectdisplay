/*unit Powerboardfirmwarev0006;
//notes:
/Watch dog timer is turned on with a 1:128 postscaler this
should allow for at time out period of 2.3 seconds or so
----------------------------------*/


//control port/ttl port definition
#define PS1           PORTD.F4
#define PS2           PORTD.F5
#define PS3           PORTD.F6
#define PS4           PORTD.F7
#define busenable     PORTD.F2
#define chargeenable  PORTD.F3
#define STAT1         PORTD.F1
#define STAT2         PORTD.F0

//#define SCLK          PORTC.F3
//#define SDA           PORTC.F4


//analog channel definition
#define getSolCel_1      Adc_Read(12)
#define getSolCel_2      Adc_Read(10)
#define getSolCel_3      Adc_Read(8)
#define getSolCel_4      Adc_Read(9)
#define getSolCel_5      Adc_Read(11)
#define getSolCel_6      Adc_Read(13)
#define getSolVolt       Adc_Read(2)
#define getBattVolt      Adc_Read(0)
#define getBattDraw      Adc_Read(1)


#define turn_on_charger  PORTD=0b00000100
#define active_on_bus      PORTD=0b00001100

//#define TMR1intervalH (65535-50000)/256
//#define TMR1intervalL (65535-50000)%256
//#define I2Ctimeout 1

//Built in functions from MikroC
#define Lo(param) ((char *)&param)[0]
#define Hi(param) ((char *)&param)[1]
#define Higher(param) ((char *)&param)[2]
#define Highest(param) ((char *)&param)[3]

#define lo(param) ((char *)&param)[0]
#define hi(param) ((char *)&param)[1]
#define higher(param) ((char *)&param)[2]
#define highest(param) ((char *)&param)[3]






/*******************************************************************************
* Gloabl Variables
*******************************************************************************/
char junk; // a place to dump extraneous data ie Address after it is checked
unsigned char ctrlbyte;
unsigned char length_msb, length_lsb,check_msb,check_lsb,chargestatus,lasterror=0;
char Datain[20];
unsigned char dataout[25];
unsigned int length, checkcalc=0, checkreceived,i ;
unsigned int BattVolt, Battdraw, Sol_1, Sol_2, Sol_3, Sol_4, Sol_5, Sol_6, Sol_volt;
unsigned long RTCcounts=0, TIMEOUT;
unsigned char powerstat;
////////////////////////////////////////////////////////////////////////////////


char readi2cbyte(void);
//void S_writei2cbyte(unsigned char data_out);
void S_PUTS( char * wrptr);
void readADC (void);
void clearwdt(void);
void howdy (void);

void powerswitch(char *datain);
void solarpanelstatus(void);
void batterystatus(void);
void errorstatus(void);
void commanderror(void);



/*******************************************************************************
interupt function
*******************************************************************************/
void interrupt (void)
{
INTCON.GIE=0;   /* turn of further interrupts*/

/*------------------------------------------------------------------------------
Timer counter increment (for future use)
------------------------------------------------------------------------------*/
//if (PIR1==0x01 ||PIR1==0x09){
// //if (RTCcounts%10==0){PS1=~PS1; }
// PS1=~PS1;
//    RTCcounts=RTCcounts+1;
////   T1CON = 0b00000000;
//    TMR1H  = TMR01intervalH;
//    TMR1L  = TMR1intervalL;
////    T1CON = 0b00000001;
////    PIR1.TMR1IF=0;
//   }
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
I2C STATE MACHINE
------------------------------------------------------------------------------*/
//else if(PIR1==0x08 ||PIR1==0x09){
PS2=~PS2;
 if(!SSPSTAT.F2 && !SSPSTAT.F5){  /* checks i2c state: for our protocol only
                                     state 1 works slave recieve and last packet
                                     was and address other states are interpreted as errors*/

   //PORTC.F7=1;     /* flick a bit to indicate  recive mode has started.*/
   checkcalc=0;    /* reset the checksum as some is done in a loop*/
   junk=readi2cbyte();     /*  empty the SSPBUFF  OF THE ADDRESS*/


   ctrlbyte=readi2cbyte();    /* read control byte  */
   length_msb=readi2cbyte();  /* read the high byte of length */
   length_lsb=readi2cbyte();  /* read the low byte of sentence length*/
   length=length_msb*0x100+length_lsb;  /* calculate length for the data loop*/
   if (length<20){                      /* check to see if its smaller then predefined size*/
      for (i=0;i<(length-5);i++){        /*-5 accounts for ctlbyte length & checksum */
       Datain[i]=readi2cbyte();           /*read in data bytes into an array*/
       checkcalc=checkcalc+Datain[i] ;    /*add them up as a checksum now;  to save a loop later*/
      }
   }
   else{
    lasterror=0xE1;       /*error condtion sentence is to big */
    errorstatus();
   // TRISD=~0xE1;          /* flash the error on the test port */
   }

   check_msb   =readi2cbyte();  /* read the high byte of checksum*/
   check_lsb   =readi2cbyte();  /* read the low byte of checksum*/
   /* check the checksum first turn the recived one back to an int */
   checkreceived=(unsigned int)check_msb*0x100 +(unsigned int)check_lsb ;
   /* then  add up all the recived bytes*/
   checkcalc=ctrlbyte+length_msb+length_lsb+  checkcalc  ;

   if (checkcalc==checkreceived){
   /* this is the main heart of the isr. thus far, the slave is synced with the master
   the datachecksum is ok and the sentecnce is an approriate length
   now is the time to interperate the ctrlbyte  and utillize the data recived
     */

           switch   (ctrlbyte)   {
           case 0x00: howdy();                  break;
           case 0x02: powerswitch(Datain);      break;
           case 0x04: solarpanelstatus();       break;
           case 0x08: batterystatus();          break;
           case 0xE0: errorstatus();            break;
           default  : commanderror();           break;
                                 }
   }
   else  if (lasterror!=0xE1){  /*check to see if this is not because of previous error*/
    lasterror=0xE4 ;       /*error condtion Checksum wrong*/
    errorstatus();
      }
 }

 else  if(SSPSTAT.F2&&!SSPSTAT.F5){
 /* checks i2c state: for our protocol only state 3 works slave transmit and
 last packet was and address other states are interpreted as errors*/
  S_PUTS(dataout) ;
 }
 else if(SSPSTAT.F2){
      lasterror=0xE3;
      errorstatus();
 }
 else if(!SSPSTAT.F2){
      lasterror=0xE2;
      errorstatus();
 }
 else{
      lasterror=0xEE;
      errorstatus();
 }
//  }
/*----------------------------------------------------------------------------*/

/*  Reset interrupts and before leaving interrupt function*/
 PIR1=0;
 INTCON.GIE=1;
}
////////////////////////////////////////////////////////////////////////////////




/*******************************************************************************
* Main
*******************************************************************************/
void main (void)
{
/*WDT setup */
 OPTION_REG=0b00001000; // see note on WDT chapter 26 in Family guide
 OPTION_REG=0b00001111;
/*------------------------------------------------------------------------------
|configuration of i2c on MSSP port
------------------------------------------------------------------------------*/



   //(*pic16)
     TRISC.F3=1;// configure clock as input
     TRISC.F4=1;// configure data as input

     SSPADD   =0xE6;
     SSPSTAT  =0b10000000;
     SSPCON   =0b00110110; // see section 10.4 pic 16f7x7 data sheet for specifics
  //   SSPCON2.SEN =1;

   //18 serries has two available i2c ports [4 registers] (SSP1CON,SSP1CON2) &(SSP2CON,SSP2CON2)
   // although their configuration is similar if not identical to the following
//------------------------------------------------------------------------------

/*------------------------------------------------------------------------------
|configuration of Timer (for future use)
------------------------------------------------------------------------------*/

//    TMR1H  = TMR1intervalH;
//    TMR1L  = TMR1intervalL;
//    T1CON = 0b00000001;       /* timer turned on and works off of internal osc*/
//------------------------------------------------------------------------------

/*-------------`-----------------------------------------------------------------
|interupt setup
------------------------------------------------------------------------------*/
   //(*pic16)
     INTCON=0;      // Turn off interrupts while configuring them (to be safe)
     Delay_ms(500); // startup delay to allow crystal to stabilize
     PIE1=0x08;     // PIE1.SSPIF=1; turn on only I2C interrupt
     //PIE1=0x09    // PIE1.TMR1IF=1 & PIE1.SSPIF=1;; turn on timer & I2C interrupt
     PIE2=0x00;     // turn off all other interrups in different register
     INTCON.PEIE=1; // turn on interrupts from PIE1&PIE2 registers
//------------------------------------------------------------------------------


/*------------------------------------------------------------------------------
|ADC & IO port setup
------------------------------------------------------------------------------*/
ADCON1 = 0x00;  // Configure analog inputs and Vref
TRISA = 0b00000111;
TRISB = 0b00111111;
TRISD = 0b00000011;  // set direction of control outputs and charge stat inputs

/*------------------------------------------------------------------------------
|Main
------------------------------------------------------------------------------*/
readADC();
//if (BattVolt<=574) {
//     PORTD=0b11111000;
//     delay_ms(1000);
//   }
//else
    {
     INTCON.GIE=1;
     PORTD=0b11110100;
    do
      {
        Delay_ms(1000); //Waiting loop
       {
        INTCON.GIE=0;
        readADC();
        INTCON.GIE=1;
        if (BattVolt>630){PORTD=PORTD&0b11110111;}         // turn off charger
        else if (BattVolt<=600){PORTD=PORTD|0b00001000;}   // turn on charger
        }
//        else if (BattVolt<=570){PORTD=PORTD|0b01111100;}

         clearwdt();
      }while (1);
    }

}

/*******************************************************************************
* Watchdog reset function
*******************************************************************************/
void clearwdt(void){
asm {
     CLRWDT
    }
}
////////////////////////////////////////////////////////////////////////////////

//*****************************************************************************
//Slave Read
//*****************************************************************************
char readi2cbyte(void)
 {
  while(!SSPSTAT.BF );    // wait till the buffer is full
  PIR1.SSPIF=0;          // reset the overflow interupt(sspov is tied to this)
  SSPSTAT.BF=0;
  return (SSPBUF);       // return databyte in buffer
 }
//*****************************************************************************

//*****************************************************************************
//Slave Write
//*****************************************************************************

void S_PUTS(char *wrptr) {
 while((*wrptr!=0xFF && *wrptr+1!=0x00)) {
  SSPBUF=(*wrptr++);
  SSPCON.CKP=1;
  while(SSPSTAT.BF);
 }
}

//******************************************************************************
////////////////////////////////////////////////////////////////////////////////



//*****************************************************************************
//ADC Read
//*****************************************************************************
void readADC (void)
{

BattVolt = getBattVolt;
Battdraw = getBattDraw;
Sol_1    = getSolCel_1;
Sol_2    = getSolCel_2;
Sol_3    = getSolCel_3;
Sol_4    = getSolCel_4;
Sol_5    = getSolCel_5;
Sol_6    = getSolCel_6;
Sol_volt = getSolVolt;
powerstat=PORTD;
chargestatus='G';


}
//******************************************************************************

//*****************************************************************************
//COMUNICATIONS TEST
//*****************************************************************************
void howdy (void){
     char k;
     unsigned int check2;

  dataout[0]=0x00;
  dataout[1]=0x00;
  dataout[2]=0x0A;
  dataout[3]='H';
  dataout[4]='O';
  dataout[5]='W';
  dataout[6]='D';
  dataout[7]='Y';
  dataout[8]='x';//null to end string
  dataout[9]='y';
  dataout[10]=0xFF;//null to end string
  dataout[11]=0x00;

  check2=0;
  k=0;
  while(dataout[k]!='x' && dataout[k+1]!='y')
  {
    check2=check2 + dataout[k];
    k++;
  }
  dataout[k]=Hi(check2);
	dataout[k+1]=Lo(check2);
 }
//******************************************************************************


//*****************************************************************************
//POWER DISTRIBUTION CONTROL
//*****************************************************************************
 void powerswitch(char *datain){
  char k;
  unsigned int check2;

  char power;
  power=PORTD;
  
  if (datain[0]==0xFF){ power.F4=1; } else if (datain[0]=0x11){power.F4=0;}
  if (datain[1]==0xFF){ power.F5=1; } else if (datain[1]=0x11){power.F5=0;}
  if (datain[2]==0xFF){ power.F6=1; } else if (datain[2]=0x11){power.F6=0;}
  if (datain[3]==0xFF){ power.F7=1; } else if (datain[3]=0x11){power.F7=0;}

  PORTD=power ;

  dataout[0]=0x02;
  dataout[1]=0x00;
  dataout[2]=0x09;
  if (PS1) {dataout[3]=0xFF;} else{dataout[3]=0x11;}
  if (PS2) {dataout[4]=0xFF;} else{dataout[4]=0x11;}
  if (PS3) {dataout[5]=0xFF;} else{dataout[5]=0x11;}
  if (PS4) {dataout[6]=0xFF;} else{dataout[6]=0x11;}
  dataout[7]='x';
  dataout[8]='y';
  dataout[10]=0xFF;//null to end string
  dataout[11]=0x00;

  check2=0;
  k=0;
  while(dataout[k]!='x' && dataout[k+1]!='y')
  {
    check2=check2 + dataout[k];
    k++;
  }
  dataout[k]=Hi(check2);
	dataout[k+1]=Lo(check2);
  
  }
//******************************************************************************


//*****************************************************************************
//SOLAR POWER STATUS
//*****************************************************************************
void solarpanelstatus(void){
  char k;
  unsigned int check2;
  dataout[0]=0x04;
  dataout[1]=0x00;
  dataout[2]=0x13;
  dataout[3]=Hi(Sol_1);
  dataout[4]=Lo(Sol_1);
  dataout[5]=Hi(Sol_2);
  dataout[6]=Lo(Sol_2);
  dataout[7]=Hi(Sol_3);
  dataout[8]=Lo(Sol_3);
  dataout[9]=Hi(Sol_4);
  dataout[10]=Lo(Sol_4);
  dataout[11]=Hi(Sol_5);
  dataout[12]=Lo(Sol_5);
  dataout[13]=Hi(Sol_6);
  dataout[14]=Lo(Sol_6);
  dataout[15]=Hi(Sol_volt);
  dataout[16]=Lo(Sol_volt);
  dataout[17]='x';
  dataout[18]='y';
  dataout[19]=0xFF;//null to end string
  dataout[20]=0x00;

    check2=0;
  k=0;
  while(dataout[k]!='x' && dataout[k+1]!='y')
  {
    check2=check2 + dataout[k];
    k++;
  }
  dataout[k]=Hi(check2);
	dataout[k+1]=Lo(check2);

  }
//******************************************************************************


//*****************************************************************************
//BATTERY STATUS
//*****************************************************************************
void batterystatus(void){
  char k;
  unsigned int check2;
  dataout[0]=0x08;
  dataout[1]=0x00;
  dataout[2]=0x0A;
  dataout[3]=chargestatus;
  dataout[4]=Hi(BattVolt);
  dataout[5]=lo(BattVolt);
  dataout[6]=Hi(Battdraw);
  dataout[7]=Lo(Battdraw);

  dataout[8]='x';
  dataout[9]='y';
  dataout[10]=0xFF;//null to end string
  dataout[11]=0x00;

    check2=0;
  k=0;
  while(dataout[k]!='x' && dataout[k+1]!='y')
  {
    check2=check2 + dataout[k];
    k++;
  }
  dataout[k]=Hi(check2);
	dataout[k+1]=Lo(check2);

  }
//******************************************************************************


//*****************************************************************************
//ERROR STATUS
//*****************************************************************************
void errorstatus(void){
char k;
  unsigned int check2;
  dataout[0]=0xE0;
  dataout[1]=0x00;
  dataout[2]=0x06;
  dataout[3]=lasterror;
  dataout[4]='x';
  dataout[5]='y';
  dataout[6]=0xFF;//null to end string
  dataout[7]=0x00;

    check2=0;
  k=0;
  while(dataout[k]!='x' && dataout[k+1]!='y')
  {
    check2=check2 + dataout[k];
    k++;
  }
  dataout[k]=Hi(check2);
	dataout[k+1]=Lo(check2);
}
//******************************************************************************


//*****************************************************************************
//POWER DISTRIBUTION CONTROL
//*****************************************************************************
void commanderror(void){
  dataout[0]=0xE0;
  dataout[1]=0x00;
  dataout[2]=0x06;
  dataout[3]=0xE7;// bad ctrlbyte error
  dataout[4]=0x01;// checksum MSB
  dataout[5]=0xCD;// checksum LSB
  dataout[6]=0xFF;//null to end string
  dataout[7]=0x00;
}
//******************************************************************************



