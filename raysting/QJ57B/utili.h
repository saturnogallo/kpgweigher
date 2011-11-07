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
//shortcut key
#define KEY_BTN1	'a' //F C mode switch
#define KEY_BTN2	'b'	//for KTT indicator
#define KEY_BTN3	'c'
#define KEY_BTN4  	'd' //run stop
//key for relay
#define KEY_KTT		'i'
#define KEY_RS		'j'
//'k'
//'l'


#define PG_BOOT     	1
#define PG_MAIN      	2
#define PG_CH_SEL   	3
#define PG_PRB_SEL   	4
#define PG_PRB_PARAM	5
#define PG_SYSTEM     	6

/*
 *	Probe data structure definition
 */
#define MAX_PRB_NUM		24
#define MAX_CH_NUM		24

#define PRBTYPE_PT100	0xf1
#define PRBTYPE_PT25	0xf2
#define PRBTYPE_INC(id)		prbdata.type[id-1] += 1; if(prbdata.type[id-1] > PRBTYPE_PT25) {prbdata.type[id-1] = PRBTYPE_PT100;}
#define PRBTYPE_INVALID	0x80

#define PRBCURR_PT01	0
#define PRBCURR_PT02	1
#define PRBCURR_PT05	2
#define PRBCURR_PT1		3
#define PRBCURR_1		4
#define PRBCURR_3		5
#define PRBCURR_10		6
#define PRBCURR_SQR		0x80

#define PRBCURR_INC(id)	prbdata.current[id-1] += 1; if( (0x7F & prbdata.current[id-1]) > PRBCURR_10) { prbdata.current[id-1] = PRBCURR_PT01}

#define PRBS_PER_SECTOR	MAX_PRB_NUM
typedef eeprom struct _PRBDATA
{
	double paramA[PRBS_PER_SECTOR];
	double paramB[PRBS_PER_SECTOR];
	double paramC[PRBS_PER_SECTOR];                      
	double rtp[PRBS_PER_SECTOR];
	char  name[PRBS_PER_SECTOR][8];	        //probe serials
	uchar current[PRBS_PER_SECTOR];		//probe current
	uchar type[PRBS_PER_SECTOR];		//probe type
}PRBDATA;

#define IS_MODE_KTT	sysdata.kttmode == 1
#define SET_MODE_KTT	sysdata.kttmode = 1; display_buttons(KEY_BTN2,0)
#define CLR_MODE_KTT	sysdata.kttmode = 0; display_buttons(KEY_BTN2,1)


#define INC_DISPCH	curr_dispch += 1 ;  if(curr_dispch > MAX_CH_NUM){ curr_dispch = 1;}
#define DEC_DISPCH	curr_dispch -= 1 ;  if(curr_dispch == 0){  curr_dispch = MAX_CH_NUM;  }
#define INC_CH	curr_ch += 1 ;  if(curr_ch > MAX_CH_NUM){ curr_ch = 1;}
#define DEC_CH	curr_ch -= 1 ;  if(curr_ch == 0){  curr_ch = MAX_CH_NUM;  }

#define DISP_FMODE		0x80	//F & C display
#define DISP_RES_MODE	0x00
#define DISP_TEMP_MODE	0x01
#define DISP_RATIO_MODE	0x02
#define INVALID_PROBE	0xff

typedef eeprom struct _SYSDATA
{
	double          R0;  					//zero offset
	double          V0;  					//zero offset
	double          Rs; 	 				//nei biaozhun
	double			Rs_paramA; 				//temp coef A for Rs
	double			Rs_paramB; 				//temp coef B for Rs
	double 			RoomT; 					//temperature in room
	int             ktime;					//time for switch current
	uchar           id[MAX_CH_NUM];         //probe index of each channel start from 1
	uchar           dispmode;				//mode for display , R/T/Raito, F/C
	uchar           kttmode;    			//mode for ktt
	uchar			scanner;				//scanner type
}SYSDATA;               
#define GETRS()	(sysdata.Rs*(1+sysdata.Rs_paramA*(sysdata.RoomT-20)+sysdata.Rs_paramB*(sysdata.RoomT-20)*(sysdata.RoomT-20)))
typedef struct _RUNDATA
{
	double  reading[MAX_CH_NUM];		  //reading on each channel
	double  temperature[MAX_CH_NUM];	  //temperature result on each channel in C

	double 	Rx;			  //final result
	uchar			runstop;				//run or stop
}RUNDATA;


extern RUNDATA rundata;
extern SYSDATA eeprom sysdata;
extern PRBDATA eeprom prbdata;	//probe data for T mode

void State_Init();
void display_buttons(uchar pos,uchar val);           

double buf2double();
int buf2byte();

#define ONESEC	        100000
extern void DBG(uchar);

void SwitchWindow(uchar page);

#define LBVAR_SN		0x01*8
#define LBVAR_TYPE		0x02*8
#define LBVAR_CURR		0x03*8
#define LBVAR_SQR		0x04*8
#define LBVAR_RTP		0x05*8
#define LBVAR_COEFA		0x06*8
#define LBVAR_COEFB		0x07*8
#define LBVAR_COEFC		0x08*8
#define	LBVAR_KTIME		0x09*8
#define LBVAR_SYSTEMP	0x0A*8
#define LBVAR_FCMODE	0x0B*8
#define LBVAR_SCANNER	0x0C*8


#include "stdio.h"
#endif
