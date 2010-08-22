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
#define PG_PRBCONFIG	11
//#define PG_TPRBCONFIG	12
#define PG_BOOTTYPE	13
#define PI	3.14159

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
typedef struct _PRBDATA
{
	double param1[PRBS_PER_SECTOR];
	double param2[PRBS_PER_SECTOR];
	double param3[PRBS_PER_SECTOR];
	char  name[PRBS_PER_SECTOR][8];	//probe serials
	uchar type[PRBS_PER_SECTOR];		//probe type

}PRBDATA;
#define MODE_BIT	0x01
#define KTT_BIT		0x40

#define IS_MODE_KTT	(sdata.mode & KTT_BIT) == KTT_BIT
#define SET_MODE_KTT	sdata.mode |= KTT_BIT
#define CLR_MODE_KTT	sdata.mode &= ~KTT_BIT
//#define TOGGLE_MODE_KTT	sdata.mode ^= KTT_BIT

#define IS_BORE_MODE	(sdata.mode & MODE_BIT) == 0
#define SET_BORE_MODE	sdata.mode &= ~MODE_BIT
#define IS_THERM_MODE	(sdata.mode & MODE_BIT) == 1
#define SET_THERM_MODE	sdata.mode |= MODE_BIT
//#define TOGGLE_MODE_BORE sdata.mode ^= MODE_BIT

#define INC_CH	curr_ch += 1 ;  if(curr_ch > MAX_CH_NUM){ curr_ch = 1;}
#define DEC_CH	curr_ch -= 1 ;  if(curr_ch == 0){  curr_ch = MAX_CH_NUM;}
#define MAX_CH_NUM	24
#define MAX_PROBE_NUM	24
#define INVALID_PROBE	0xff



typedef struct _SYSDATA
{
	uchar 	mode;
	double Rs1; //jiao-zheng zhi
	int ktime;//time for switch
	
	uchar 	id[MAX_CH_NUM];	//probe index of each channel


}SYSDATA;
typedef struct _RUNDATA
{
	uchar	has_scanner;  //whether scanner is available	
	int 	scanmode;	  //channel definition of scanner

	double  reading[MAX_CH_NUM];		  //reading on each channel
	double  temperature[MAX_CH_NUM];	  //temperature result on each channel

	double 	Rx;		  //final result
//	double  Temp1820;	  //reading of 18b20
	double  stdV;		  //voltage on stdV;
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
uchar sm_read(uchar final);
void sm_write(uchar value);
void sm_action(uchar cmd);

void sm_wait_done(uchar cmd);
double smget_double(uchar cmd);
uchar sm2_read(uchar final);
void sm2_wait_done(uchar cmd);
double smget_double(uchar cmd);
void sm2_action(uchar cmd);

#define HEAD_MARK	0xCC
#define ONESEC	2
#define CMD_SCAN_SETCH_MAX	24	//set channel command 1 to 24
#define CMD_SCAN_UPDATE		0x70    //update the status of scanner
#define CMD_SCAN_PKTT		0x71
#define CMD_SCAN_NKTT		0x73
#define CMDR_SCAN_CHTYPE	0x74	//scanner channel type 2 wire or 4 wire
#define CMDR_SCAN_CHCURR	0x76	//current channel index
#define CMDR_SCAN_CHNUM		0x78	//current channel number
#define CMDR_SCAN_STATE		0x72    //read ch_action

#define SCAN_RESERVE	0x00
#define SCAN_ACTION	0x40
#define	SCAN_QUERY	0x80
#define RET_ERROR	0xC0
#define STATE_MASK	0xC0
#define	WIRE_2		0x00
#define WIRE_4		0x20
#define	WIRE_MASK	0x20
#define POS_MASK	0x1f

#define CMD_INVALID		0xfe
#define CMD_ACCEPT		0xA5
#define CMD_DMM_PKTT		0x81	//switch ktt to positive
#define CMD_DMM_NKTT		0x83	//switch ktt to negative
#define CMD_DMM_RX		0x85	//switch rx,rs
#define CMD_DMM_RS		0x87	//switch rx,rs
#define CMD_DMM_ZERO		0x89	//set the current voltage to zero
#define CMD_DMM_VUPDATE		0x8b	//update the volt reading
#define CMD_DMM_TUPDATE		0x8d	//update the temp reading
#define CMD_DMM_TYPE_K		0x93		
#define CMD_DMM_TYPE_N		0x94		
#define CMD_DMM_TYPE_E		0x95		
#define CMD_DMM_TYPE_B		0x96		
#define CMD_DMM_TYPE_J		0x97		
#define CMD_DMM_TYPE_S		0x98		
#define CMD_DMM_TYPE_R		0x99		
//data read
#define CMDR_DMM_VOLT		0x84	//read dmm voltage
#define CMDR_DMM_TEMP		0x86	//read converted result
#define CMDR_DMM_STATE		0x88	//read state of dmm_action

extern void DBG(uchar);
extern void Init_18b20();
extern void get_18b20();

void SwitchWindow(uchar page);

extern char data strbuf[]; 	//global string buffer
extern char data databuf[];
extern uchar pos_strbuf;	//length of string buffer
extern uchar pos_databuf;
#include "stdio.h"
#endif
