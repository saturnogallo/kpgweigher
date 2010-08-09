#ifndef __UTILI_H
#define __UTILI_H


#include "typedef.h"

#define HASCH1		01
#define HASCH2		02

void delay (uint us) ;
void delay1 (uint ms);
uchar strlen(uchar *s);
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

#define PG_START     	1
#define PG_MAIN      	2
#define PG_RCONFIG   	3
#define PG_TCONFIG   	4
#define PG_CALIBRATION	5
#define PG_KTIME     	6
#define PG_CHSET     	7
#define PG_PRBLIST   	8
#define PG_PRBTYPELIST	9
#define PG_PRBSET 	10
#define PG_RPRBCONFIG	11
#define PG_TPRBCONFIG	12

#define PI	3.14159

/*
 *	Probe data structure definition
 */
#define PRBS_PER_SECTOR	12
#define THERM_PRB_OFFSET 24	//offset for therm probe position

#define PRBTYPE_PT100	0x11
#define PRBTYPE_PT25	0x12
#define PRBTYPE_K	0x03
#define PRBTYPE_BIT	0x10
typedef struct _PRBDATA
{
	double param1[PRBS_PER_SECTOR];
	double param2[PRBS_PER_SECTOR];
	double param3[PRBS_PER_SECTOR];
	double param4[PRBS_PER_SECTOR];
	char  name[PRBS_PER_SECTOR][10];	//probe serials
	uchar type[PRBS_PER_SECTOR];		//probe type

}PRBDATA;
#define MODE_BIT	0x01
#define KTT_BIT	0x40

#define IS_MODE_KTT	(sdata.mode && KTT_BIT) == KTT_BIT
#define SET_MODE_KTT	sdata.mode |= KTT_BIT
#define CLR_MODE_KTT	sdata.mode &= ~kTT_BIT
#define TOGGLE_MODE_KTT	sdata.mode ^= KTT_BIT

#define IS_BORE_MODE	(sdata.mode && MODE_BIT) == 0
#define SET_BORE_MODE	sdata.mode &= ~MODE_BIT
#define IS_THERM_MODE	(sdata.mode && MODE_BIT) == 1
#define SET_THERM_MODE	sdata.mode |= MODE_BIT
#define TOGGLE_MODE_BORE sdata.mode ^= MODE_BIT

#define INC_CH	curr_ch += 1 ;  if(curr_ch > sdata.scanmode) curr_ch = 1;
#define DEC_CH	curr_ch -= 1 ;  if(curr_ch == 0)  curr_ch = sdata.scanmode;
#define MAX_CH_NUM	24
#define MAX_PROBE_NUM	24
#define INVALID_PROBE	0xff
typedef struct _SYSDATA
{
	double 	Rs1;	  	//jiao-zheng zhi
	double  V0;	  	//voltage offset value
	int   ktime;	  	//time for switch
	int 	scanmode;	//channel definition of scanner
	uchar	mode;	  	//do BORE or thermo test, has KTT or not
	uchar 	id[MAX_CH_NUM];	//probe index of each channel
}SYSDATA;
typedef struct _RUNDATA
{

	double  reading[MAX_CH_NUM];		  //reading on each channel
	double  temperature[MAX_CH_NUM];	  //temperature result on each channel

	double 	Rx;		  //final result
	double  Temp1820;	  //reading of 18b20
	double  stdV;		  //voltage on stdV;
	uchar	has_scanner;				  //whether scanner is available	
}RUNDATA;


extern RUNDATA rdata;
extern SYSDATA sdata;
extern PRBDATA prbdata;	//probe data


void LoadFromEEPROM();
void SaveToEEPROM();		//save system data into EEPROM
void State_Init();
void display_buttons(uchar pos,uchar val);
double buf2double();
int buf2byte();
void LCD_Print8X16(uchar x, uchar y,uchar *s);

void sm_Init();
void sm_read(uchar pos,uchar ch);
void sm_write(uchar value);

extern void DBG(uchar);
extern void Init_18b20();
extern void get_18b20();

void SwitchWindow(uchar page);

extern char strbuf[]; 	//global string buffer
extern xdata char databuf[];
extern uchar pos_strbuf;	//length of string buffer
extern uchar pos_databuf;
int sjprintf(char *s, const char *fmt, ...);
#endif
