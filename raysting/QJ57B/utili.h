#ifndef __UTILI_H
#define __UTILI_H


#include "typedef.h"
#include "global.h"

void delay (uint us) ;
void delay1 (uint ms);
char lowc(uchar x);
char highc(uchar x);
#define KEY_INVALID '-'													  
#define KEY_NUM0	'0'
#define KEY_NUM1	'1'
#define KEY_NUM2	'2'
#define KEY_NUM3	'3'
#define KEY_NUM4	'4'
#define KEY_NUM5	'5'
#define KEY_NUM6	'6'
#define KEY_NUM7	'7'
#define KEY_NUM8	'8'
#define KEY_NUM9	'9'
#define KEY_DOT		'.'
#define KEY_UP		'U'
#define KEY_DN		'D'
#define KEY_CE		'C'
#define KEY_OK		'O'
#define KEY_TAB		'T'
#define KEY_BTN1	'a'
#define KEY_BTN2	'b'
#define KEY_BTN3	'c'
#define KEY_BTN4  	'd'
#define KEY_KTT		'i'
#define KEY_RS		'j'
//'k'
//'l'


#define PG_START     	        1
#define PG_MAIN      	        2
#define PG_CFGPRB_SELECT        3
#define PG_CFGPRB_SET  	        4
#define PG_NOUSE1	        5
#define PG_CFGCH_PRBLIST   	6
#define PG_CFGCH_SELECT   	7       //select channel to config
#define PG_NOUSE2           	8
#define PG_PRBTYPELIST	        9
#define PG_NOUSE3 	        10
#define PG_NOUSE4       	11
//#define PG_TPRBCONFIG	12
#define PG_BOOTTYPE	13

/*
 *	Probe data structure definition
 */
#define PRBS_PER_SECTOR	24
#define THERM_PRB_OFFSET 24	//offset for therm probe position

#define PRBTYPE_PT100	0xf1
#define PRBTYPE_PT25	0xf2
#define PRBTYPE_K	0x03
#define PRBTYPE_N	0x04
#define PRBTYPE_E	0x05
#define PRBTYPE_B	0x06
#define PRBTYPE_J	0x07
#define PRBTYPE_S	0x08
#define PRBTYPE_R	0x09
#define PRBTYPE_BIT	0x10
#define PRBTYPE_INVALID	0x80
typedef eeprom struct _PRBDATA
{
	double param1[PRBS_PER_SECTOR];
	double param2[PRBS_PER_SECTOR];
	double param3[PRBS_PER_SECTOR];                      
	double rtp[PRBS_PER_SECTOR];
	char  name[PRBS_PER_SECTOR][8];	        //probe serials
	uchar type[PRBS_PER_SECTOR];		//probe type
}PRBDATA;

#define IS_MODE_KTT	sysdata.kttmode == 1
#define SET_MODE_KTT	sysdata.kttmode = 1; display_buttons(KEY_BTN2,0)
#define CLR_MODE_KTT	sysdata.kttmode = 0; display_buttons(KEY_BTN2,1)

#define IS_BORE_MODE	sysdata.prbmode == 1
#define SET_BORE_MODE	sysdata.prbmode = 1; scanner_set_mode(); display_buttons(KEY_BTN1,0)
#define IS_THERM_MODE	sysdata.prbmode == 0
#define SET_THERM_MODE	sysdata.prbmode = 0; scanner_set_mode(); display_buttons(KEY_BTN1,1)

#define INC_DISPCH	curr_dispch += 1 ;  if(curr_dispch > MAX_CH_NUM){ curr_dispch = 1;}
#define DEC_DISPCH	curr_dispch -= 1 ;  if(curr_dispch == 0){  curr_dispch = MAX_CH_NUM;  }
#define INC_CH	curr_ch += 1 ;  if(curr_ch > MAX_CH_NUM){ curr_ch = 1;}
#define DEC_CH	curr_ch -= 1 ;  if(curr_ch == 0){  curr_ch = MAX_CH_NUM;  }

#define MAX_CH_NUM	24
#define MAX_PROBE_NUM	24

#define INVALID_PROBE	0xff

typedef eeprom struct _SYSDATA
{
	double          R0;  //zero offset
	double          V0;  //zero offset
	double          Rs1; //jiao-zheng zhi
	int             ktime;//time for switch
	uchar 	        tid[MAX_CH_NUM];	//probe index of each channel for T mode
	uchar           rid[MAX_CH_NUM];        //probe index of each channel for R mode
	uchar           prbmode;
	uchar           kttmode;                    
}SYSDATA;               

typedef struct _RUNDATA
{
	double  reading[MAX_CH_NUM];		  //reading on each channel
	double  temperature[MAX_CH_NUM];	  //temperature result on each channel

	double 	Rx;		  //final result
	double  stdV;		  //voltage on stdV;
}RUNDATA;


extern RUNDATA rdata;
extern SYSDATA eeprom sysdata;
extern PRBDATA eeprom tprbdata;	//probe data for T mode
extern PRBDATA eeprom rprbdata;	//probe data for R mode

void State_Init();
void display_buttons(uchar pos,uchar val);           

double buf2double();
int buf2byte();

#define ONESEC	        100000
//#define ONESECBIT       14

extern void DBG(uchar);

void SwitchWindow(uchar page);

char* rname2b(u8 i);
char* tname2b(u8 i);
#include "stdio.h"
#endif
