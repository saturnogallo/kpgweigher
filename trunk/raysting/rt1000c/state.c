#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "menu.h"
#include "eeprom.h"

extern void State_Display();
extern ulong code Sector[10][4];

#define RS_REG1		Sector[0][0]	//[0]
#define RS_REG2		Sector[0][1]
#define RS_REG3		Sector[0][2]
#define RS_REG4		Sector[0][3]	
#define DELAY_REG 	Sector[1][0]	//[1]
#define SAMPLE_REG 	Sector[1][1]	//[2]
#define RS232_REG 	Sector[1][2]	//[3]
#define RATIO_REG 	Sector[1][3]	//[4]
#define CALI_REG1	Sector[2][0]	//[0]
#define CALI_REG2	Sector[2][1]
#define CALI_REG3	Sector[2][2]
#define CALI_REG4	Sector[2][3]	
void LoadFromEEPROM()
{
	uchar* arr = (uchar*)&rdata;
	*arr++ = byte_read(RS_REG1);
	*arr++ = byte_read(RS_REG2);
	*arr++ = byte_read(RS_REG3);
	*arr++ = byte_read(RS_REG4);
	*arr++ = byte_read(DELAY_REG);
	*arr++ = byte_read(SAMPLE_REG);
	*arr++ = byte_read(RS232_REG);
	*arr++ = byte_read(RATIO_REG);
	*arr++ = byte_read(CALI_REG1);
	*arr++ = byte_read(CALI_REG2);
	*arr++ = byte_read(CALI_REG3);
	*arr++ = byte_read(CALI_REG4);
}
void SaveToEEPROM()
{
	uchar* arr = (uchar*)&rdata;
	SectorErase(RS_REG1);
	SectorErase(DELAY_REG);
	SectorErase(CALI_REG1);

	byte_write(RS_REG1,*arr++);
	byte_write(RS_REG2,*arr++);
	byte_write(RS_REG3,*arr++);
	byte_write(RS_REG4,*arr++);
	byte_write(DELAY_REG,*arr++);
	byte_write(SAMPLE_REG,*arr++);
	byte_write(RS232_REG,*arr++);
	byte_write(RATIO_REG,*arr++);
	byte_write(CALI_REG1,*arr++);
	byte_write(CALI_REG2,*arr++);
	byte_write(CALI_REG3,*arr++);
	byte_write(CALI_REG4,*arr++);
}
void State_Init()
{
	rdata.StateId = PG_MAIN;
	rdata.pos_len = 0;
	rdata.Rs = 1.0;
	rdata.Is = 1.0;
	rdata.Rx = 0.0;
	rdata.Ix = 1.0;
	rdata.Delay = 10;
	rdata.delay_cnt = rdata.Delay << DELAY_MULTIPLE;
	rdata.Samplerate = 1;
	rdata.Baudrate = 1;
	rdata.Ratio = KEY_SCA10;
	rdata.cali_ratio = 1;
	LoadFromEEPROM();
	display_scale();
	State_Display();
}

void State_Change(uchar key)
{
	unsigned char i;
	if(rdata.StateId == PG_MAIN) {
		if(key == KEY_TAB){ //show menu
			rdata.StateId = PG_MENU1;
			rdata.pos_len = PG_SETRS; // set the first option of menu
		}
		return;
	}
	if(rdata.StateId == PG_MENU1) {
		switch(key)
		{
		case  KEY_NUM1: 
			rdata.StateId = PG_SETRS;
			rdata.pos_len = 0;
			return;

		case  KEY_NUM2: 
			rdata.StateId = PG_SETDELAY;
			rdata.pos_len = 0;
			return;

		case  KEY_NUM3: 
			rdata.StateId = PG_ZERO;
			rdata.pos_len = rdata.Baudrate;
			return;

		case  KEY_NUM4: 
			rdata.StateId = PG_SETRATE;
			rdata.pos_len = 0;
			return;

		case  KEY_NUM5: 
			rdata.StateId = PG_CALIBRATE;
			rdata.pos_len = 0;
			return;

		case  KEY_NUM6:
			rdata.StateId = PG_MAIN;
			rdata.pos_len = 0;
			return;
			
		case  KEY_DN:
			if(rdata.pos_len == PG_SETRS)
				rdata.pos_len = PG_CALIBRATE;
			return;
		case  KEY_UP:
			if(rdata.pos_len == PG_CALIBRATE)
				rdata.pos_len = PG_SETRS;
			return;
		case KEY_OK:
			rdata.StateId = rdata.pos_len;
			rdata.pos_len = 0;
			return;	
		default:
			break;
		}
		rdata.StateId = PG_MAIN;
		rdata.pos_len = 0;
		return;
	}
	if(rdata.StateId == PG_MENU2) {
		return;
	}
	if(rdata.StateId == PG_SETRS)  {
		if((key >= KEY_NUM0) && (key <= KEY_NUM9) ) {
			if(rdata.pos_len < BUF_MAX){
				rdata.tempbuf[rdata.pos_len] = key;
				rdata.pos_len++;
			}
			return;
		}
		if((key == KEY_DOT) && (rdata.pos_len > 0)){
			for(i=0;i<rdata.pos_len;i++)
			{
				if(KEY_DOT == rdata.tempbuf[i]) //there is a dot existed
					return;
			}
			if(rdata.pos_len < (BUF_MAX-1)){
				rdata.tempbuf[rdata.pos_len] = key;
				rdata.pos_len++;
			}
			return;
		}
		if((key == KEY_OK) || (key == KEY_TAB))
		{
			if((key == KEY_OK) && (rdata.pos_len > 0))
			{
				rdata.Rs = buf2double();
				SaveToEEPROM();
			}
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_MENU1;
		}
		if(key == KEY_CE) {									   
			rdata.pos_len = 0;
			return;
		}
		return;
	}
	if(rdata.StateId == PG_SETDELAY) {
		if((key >= KEY_NUM0) && (key <= KEY_NUM9)) {
			if(rdata.pos_len < 2){
				rdata.tempbuf[rdata.pos_len] = key;
				rdata.pos_len++;
			}
			return;
		}
		if((key == KEY_OK) || (key == KEY_TAB))
		{
			if((key == KEY_OK) && (rdata.pos_len > 0))
			{
				rdata.Delay = buf2byte();
				if(rdata.Delay < 3)
					rdata.Delay = 3;
				SaveToEEPROM();
			}
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_MENU1;
			return;
		}
		if(key == KEY_CE) {
			rdata.pos_len = 0;
			return;
		}
		return;
	}
	/*
	if(rdata.StateId == PG_SET232) { //list box with None,2400,4800,9600,12800
		if(key == KEY_UP) {
			rdata.pos_len++;
			if(rdata.pos_len == 5)
				rdata.pos_len = 0;
			return;
		}
		if(key == KEY_DN) {
			if(rdata.pos_len == 0)
				rdata.pos_len = 4;
			else
				rdata.pos_len--;
			return;
		}

		if((key <= KEY_NUM1) && (key <= KEY_NUM5)){
			rdata.pos_len = key - KEY_NUM1;
			key = KEY_OK;
		}
		if((key == KEY_OK) || (key == KEY_TAB))
		{
			if(key == KEY_OK)
			{
				rdata.Baudrate = rdata.pos_len;
				SaveToEEPROM();
			}
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_MENU1;
			return;
		}
		if(key == KEY_CE) {
			rdata.pos_len = rdata.Baudrate;						
		}
		return;
	}
	*/
	if(rdata.StateId == PG_SETRATE) {
		if((key >= KEY_NUM0) && (key <= KEY_NUM9) )
		{
			if(rdata.pos_len < 1){
				rdata.tempbuf[rdata.pos_len] = key;
				rdata.pos_len++;
			}
			return;
		}
		if((key == KEY_OK) || (key == KEY_TAB))
		{
			if(key == KEY_OK)
			{
				rdata.Samplerate = buf2byte();
				SaveToEEPROM();
			}
			
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_MENU1;

		}
		if(key == KEY_CE)
		{
			rdata.pos_len = 0;
		}
		return;
	}
	/*
	if(rdata.StateId == PG_HELP) {
		rdata.pos_len = rdata.StateId;
		rdata.StateId = PG_MENU1;
		return;
	}
	*/
	return;
}
