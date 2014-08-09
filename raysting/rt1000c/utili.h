#ifndef __UTILI_H
#define __UTILI_H

#include "typedef.h"

void delay (uint us) ;
void delay1 (uint ms);
uchar strlen(uchar *s);
 
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
#define KEY_SCA10	'a'
#define KEY_SCA100	'b'
#define KEY_SCA1K	'c'
#define KEY_SCA10K  'd'
											
#define PG_MAIN		1
#define PG_MENU1	2
#define PG_MENU2	3
#define PG_SETRS	4
#define PG_SETDELAY	5
//#define PG_SET232	6
#define PG_ZERO		6
#define PG_SETRATE	7
#define PG_CALIBRATE 8
//#define PG_HELP		8
#define BUF_MAX		8

#define DELAY_MULTIPLE	10
#define DELAY_MASK		0x000003ff
typedef struct _RUNDATA
{
	double 	Rs;
	uchar	Delay;
	uchar	Samplerate;
	uchar	Baudrate;
	uchar 	Ratio;
	double	cali_ratio;

	double 	Is;
	double 	Ix;
	double 	Rx;
	uchar	delay_cnt;
	uchar	StateId;
	uchar	pos_len;
	uchar	placeholder;
	char	tempbuf[BUF_MAX];
	
}RUNDATA;

extern RUNDATA rdata;
void SaveToEEPROM();
void State_Change(uchar);
void State_Display();
void State_Init();
void display_scale();
double buf2double();
int buf2byte();
void LCD_Print8X16(uchar x, uchar y,uchar *s);
#endif
