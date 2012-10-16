#ifndef __UTILI_H
#define __UTILI_H


#include "define.h"

void delay (uint us) ;
void delay1 (uint ms);
char lowc(uchar x);
char highc(uchar x);
#define KEY_INVALID '-'													  
#define KEY_MODE	'.'
#define KEY_PROBE	'U'
#define KEY_ZERO	'D'
#define KEY_LEFT	KEY_ZERO
#define KEY_DIGI	'C'
#define KEY_RIGHT	KEY_DIGI
#define KEY_CHAN	'O'
#define KEY_UP	KEY_CHAN
#define KEY_KTT		'T'
#define KEY_DN		KEY_KTT
#define KEY_OK		'K'


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
	u8 param1[MAX_CH_NUM][8]; //+0.000E+0
	u8 param2[MAX_CH_NUM][8]; //+0.000E+0
	u8 param3[MAX_CH_NUM][8]; //+0.000E+0                        
	u8 rtp[MAX_CH_NUM][8];	//0000.0000
}PRBDATA;

#define MODE_TEMP			1
#define MODE_BORE			2
#define MODE_THERMO			4

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
	u8				prbtype; //probe type
	u8				mode;	 //mode
	u8				kttmode; //ktt mode
	u8				fcmode;	 //F or C
	u8				chan;	 //channel mode
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

#define ONEMS           (u16)10000
#define ONESEC	        100000
//#define ONESECBIT       14

extern void DBG(uchar);
extern void navto120mv();
extern void navto1v();
#include "stdio.h"
#endif
