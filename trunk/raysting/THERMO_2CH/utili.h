#ifndef __UTILI_H
#define __UTILI_H


#include "define.h"

void delay (uint us) ;
void delay1 (uint ms);
char lowc(uchar x);
char highc(uchar x);
#define KEY_INVALID     0xff													  
#define KEY_MODE	0
#define KEY_PROBE	1
#define KEY_ZERO	2
#define KEY_LEFT	KEY_ZERO
#define KEY_DIGI	3
#define KEY_RIGHT	KEY_DIGI
#define KEY_CHAN	4
#define KEY_UP	        KEY_CHAN
#define KEY_KTT		5
#define KEY_DN		KEY_KTT
#define KEY_OK		6


/*
 *	Probe data structure definition
 */
#define MAX_CH_NUM	 2

#define PRBTYPE_PT100	0x00  
#define PRBTYPE_PT25	0x01  
#define PRBTYPE_PT1000  0x02  

#define PRBTYPE_K 	   	0x03
#define PRBTYPE_N		0x04
#define PRBTYPE_E		0x05
#define PRBTYPE_B		0x06
#define PRBTYPE_J		0x07
#define PRBTYPE_S		0x08
#define PRBTYPE_T		0x09
#define PRBTYPE_R		0x0A

typedef eeprom struct _PRBDATA
{
	char param1[MAX_CH_NUM][8]; //+0.000E+0
	char param2[MAX_CH_NUM][8]; //+0.000E+0
	char param3[MAX_CH_NUM][8]; //+0.000E+0                        
	char rtp[MAX_CH_NUM][8];	//0000.0000
}PRBDATA;

#define MODE_TEMP			1 //display of temperature
#define MODE_BORE			2 //display of R value
#define MODE_THERM			4 //display of V value
                  
#define PORT_NAV                        0


#define CHAN_1				0
#define CHAN_2				1
#define CHAN_ALL			2
#define CHAN_MINUS			3
typedef eeprom struct _SYSDATA
{
	double          R0;  	 //zero offset
	double          V0;  	 //zero offset
	double          Rs1; 	 //jiao-zheng zhi for PT100
	double          Rs2; 	 //for PT1000              
	u8             	ktime;	 //time for switch
	u8		type_bore;  //probe type of channel 1 and channel 2 for bore case
	u8              type_therm; //probe type of channel 1 and channel 2 for therm case
	u8		mode;	 //mode
	u8		kttmode; //ktt mode
	u8		fcmode;	 //F or C
	u8		chan;	 //channel mode
}SYSDATA;               

typedef struct _RUNDATA
{
	double param1[MAX_CH_NUM];
	double param2[MAX_CH_NUM];
	double param3[MAX_CH_NUM];                         
	double rtp[MAX_CH_NUM];

	double  reading[MAX_CH_NUM];		  //reading on each channel
	double  temperature[MAX_CH_NUM];	  //temperature result on each channel

	double 	Rx;		  //final result
	double  stdV;		  //voltage on stdV;
	double  display;
}RUNDATA;


extern RUNDATA rdata;
extern SYSDATA eeprom sysdata;
extern PRBDATA eeprom rprbdata;	//probe data for channels

#define ONEMS           (u16)20833
#define ONESEC	        (u16)20833000
//#define ONESECBIT       14

//PORTB.7 RX, PORTB.6 RS, PORTB.5 1MA, PORTB.4. 0.1MA,
//PORTB.3 PT100, PORTB.2 PT1000, PORTB.1 CH1,  PORB.0 CH2    
#define RLYDELAY     60*ONEMS                             
#define IS_MODE_KTT  sysdata.kttmode==1
#define IS_THERM_MODE (sysdata.mode & MODE_THERM)
#define IS_BORE_MODE (sysdata.mode & MODE_BORE)
#define SET_PKTT     {PORTA.6 = 1; sleepms(RLYDELAY);}              //PWM_FB
#define SET_NKTT     {PORTA.6 = 0; sleepms(RLYDELAY);}              //PWM_FB
#define SET_TOCH2    {PORTA.5 = 0; sleepms(RLYDELAY);PORTA.5 = 1;} //MK_DIR
#define SET_TOCH1    {PORTA.4 = 0; sleepms(RLYDELAY);PORTA.4 = 1;} //MT_CLK     

#define SET_TORS     {PORTD.2 = 0; sleepms(RLYDELAY);PORTD.2 = 1;} //SENSE1
#define SET_TORX     {PORTD.3 = 0; sleepms(RLYDELAY);PORTD.3 = 1;} //SENSE2  
#define SET_TOPT1000 {PORTD.4 = 0; sleepms(RLYDELAY);PORTD.4 = 1;} //LED1
#define SET_TOPT100  {PORTD.5 = 0; sleepms(RLYDELAY);PORTD.5 = 1;} //LED2
#define SET_TO1MA    {PORTD.6 = 0; sleepms(RLYDELAY);PORTD.6 = 1;} //LED3
#define SET_TOP1MA   {PORTD.7 = 0; sleepms(RLYDELAY);PORTD.7 = 1;} //LED4
                     
#define LED_CH1         0x2000
#define LED_CH2         0x4000
#define LED_TEMP        0x0001
#define LED_BORE        0x0002
#define LED_THERM       0x0004
#define LED_PT100       0x0008
#define LED_PT1000      0x0010
#define LED_K           0x0020
#define LED_N           0x0040
#define LED_E           0x0080
#define LED_B           0x0100
#define LED_J           0x0200
#define LED_S           0x0400
#define LED_T           0x0800
#define LED_R           0x1000 

extern void DBG(uchar);
extern void navto120mv();
extern void navto1v();
extern void scanner_set_mode();
#include "stdio.h"
#endif
