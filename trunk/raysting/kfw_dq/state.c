#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "eeprom.h"
#include "stdio.h"

extern void State_Display();
extern ulong code Sector[10][4];

#define RS_SECTBASE		Sector[0][0]	//[0]

void LoadFromEEPROM()
{
	int i = RS_SECTBASE;
	uchar* arr = (uchar*)&rdata;

	for(i=0;i<sizeof(RUNDATA);i++)
	{
		*arr++ = byte_read(i++);
	}
}
void SaveToEEPROM()
{
	int i = RS_SECTBASE;
	uchar* arr = (uchar*)&rdata;
	SectorErase(RS_SECTBASE);

	for(i=0;i<sizeof(RUNDATA);i++)
	{
		byte_write(i++,*arr++);
	}
}
void State_Init()
{
	rdata.StateId = PG_MAIN;
	rdata.pos_len = 0;
	rdata.Delay = 10;
	rdata.delay_cnt = rdata.Delay << DELAY_MULTIPLE;

	rdata.R0 = 0.0;
	rdata.Rs1 = 20000000;
	rdata.Rcali[RANGE_20m] = 0.02;	
	rdata.Rcali[RANGE_200m] = 0.2;		
	rdata.Rcali[RANGE_2] = 2;
	rdata.Rcali[RANGE_20] = 20;
	rdata.Rcali[RANGE_200] = 200;
	rdata.Rcali[RANGE_2k] = 2000;
	rdata.Rcali[RANGE_20k] = 20000;
	rdata.Rcali[RANGE_200k] = 200000;
	rdata.Rcali[RANGE_2M] = 2000000;
	rdata.Rcali[RANGE_20M] = 20000000;

	rdata.Pheight = rdata.Pwidth = rdata.Pradius = 1;
	rdata.Rauto = AUTO_OFF;
	rdata.Rktt = KTT_OFF;
	rdata.Plength = 1;
	rdata.Ptype = PSET_NONE;
	rdata.Pvalue = 1;
	rdata.Temp = 25;
	rdata.Range = RANGE_20M;
	rdata.Baudrate = 0;
	rdata.Current = CURRENT_1;
	sprintf(rdata.tempbuf,"       ");
//	LoadFromEEPROM();
	display_buttons(KEY_BTN1,rdata.Rauto);
	display_buttons(KEY_BTN2,rdata.Rktt);
	State_Display();
}

void State_Change(uchar key)
{
	if(rdata.StateId == PG_MAIN) {
		if(key == KEY_TAB){ //show menu
			rdata.StateId = PG_MENU1;
			rdata.pos_len = PG_RANGE; // set the first option of menu
		}
		return;
	}
	if(rdata.StateId == PG_MENU1) {
		if(key == KEY_OK)
			key = rdata.pos_len + KEY_NUM1;
		switch(key)
		{
		case  KEY_NUM1: 
			rdata.StateId = PG_RANGE;
			rdata.pos_len = rdata.Range;
			return;

		case  KEY_NUM2: 
			rdata.StateId = PG_CALISET;
			rdata.pos_len = 0;
			sprintf(rdata.tempbuf,"       ");
			return;

		case  KEY_NUM3: 
			rdata.StateId = PG_MSG_RZERO;
			rdata.pos_len = 0;
			return;

		case  KEY_NUM4: 
			rdata.StateId = PG_PSET;
			rdata.pos_len = 0;
			return;

		case  KEY_NUM5: 
			rdata.StateId = PG_SET232;
			rdata.pos_len = rdata.Baudrate;
			return;

		case  KEY_NUM6:
			rdata.StateId = PG_HELP;
			rdata.pos_len = 0;
			return;
			
		case  KEY_DN:
			if(rdata.pos_len == 5){
					rdata.pos_len =0 ;
			}else{
				rdata.pos_len++;
			}
			return;
		case  KEY_UP:
			if(rdata.pos_len == 0){
				rdata.pos_len = 5;
			}else{
				rdata.pos_len--;
			}
			return;
		default:
			;
		}
		rdata.StateId = PG_MAIN;
		rdata.pos_len = 0;
		return;
	}
	if(rdata.StateId == PG_MENU2) {
		return;
	}
	if(rdata.StateId == PG_RANGE)  {
		if(key == KEY_UP) {
			if(rdata.pos_len >= RANGE_20M)
				rdata.pos_len = RANGE_20m;
			else
				rdata.pos_len++;
			return;
		}
		if(key == KEY_DN) {
			if(rdata.pos_len == RANGE_20m)
				rdata.pos_len = RANGE_20M;
			else
				rdata.pos_len--;
			return;
		}

		if((key >= KEY_NUM0) && (key <= KEY_NUM9)){
			rdata.pos_len = RANGE_20m + key - KEY_NUM0;
			key = KEY_OK;
		}
		if((key == KEY_OK) || (key == KEY_TAB))
		{
			if(key == KEY_OK)
			{
				rdata.Range = rdata.pos_len;
				SaveToEEPROM();
			}
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_MENU1;
			return;
		}
		if(key == KEY_CE) {
			rdata.pos_len = rdata.Range;
		}
		return;
	}
	if(rdata.StateId == PG_CALISET) { //input new calibration value
		if(((key >= KEY_NUM0) && (key <= KEY_NUM9)) || (key == KEY_DOT)) {
			if(rdata.pos_len < 8){
				rdata.tempbuf[rdata.pos_len] = key;
				rdata.pos_len++;
			}
			return;
		}
		if((key == KEY_OK) || (key == KEY_TAB))
		{
			if((key == KEY_OK) && (rdata.pos_len > 0))
			{
				rdata.Rcali[rdata.Range] = buf2double();
				SaveToEEPROM();
				//conduct calibration here
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
	if(rdata.StateId == PG_SET232) { //list box with None,2400,9600
		if(key == KEY_UP) {
			if(rdata.pos_len >= BAUDRATE_NONE)
				rdata.pos_len = 0;
			else
				rdata.pos_len++;
			return;
		}
		if(key == KEY_DN) {
			if(rdata.pos_len == BAUDRATE_NONE)
				rdata.pos_len = BAUDRATE_9600;
			else
				rdata.pos_len--;
			return;
		}

		if((key >= KEY_NUM1) && (key <= KEY_NUM3)){
			rdata.pos_len = BAUDRATE_NONE + key - KEY_NUM1;
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
	if(rdata.StateId == PG_RZERO) {
		//trigger the zero process
		return;
	}
	if(rdata.StateId == PG_HELP) {
		if((key >= KEY_NUM1) && (key <= KEY_NUM3))
		{
			if(key == KEY_NUM1)
				rdata.StateId = PG_HELP_PREC;
			if(key == KEY_NUM2)
				rdata.StateId = PG_HELP_ADDR;
			if(key == KEY_NUM3)
				rdata.StateId = PG_HELP_SET;

			return;

		}
		rdata.pos_len = rdata.StateId;
		rdata.StateId = PG_MENU1;
		return;
	}

	if(rdata.StateId == PG_PSET) {
		if(key == KEY_UP)
		{
			if(rdata.pos_len >= 4)
				rdata.pos_len = 0;
			else
				rdata.pos_len++;
			return;
		}
		if(key == KEY_DN)
		{
			if(rdata.pos_len == 0)
				rdata.pos_len = 4;
			else
				rdata.pos_len--;
			return;
		}

		if(key == KEY_OK)
			key = KEY_NUM1 + rdata.pos_len;

		if((key >= KEY_NUM1) && (key <= KEY_NUM3))
		{
			if(key == KEY_NUM1)
				rdata.StateId = PG_PSET_L;
			if(key == KEY_NUM2)
				rdata.StateId = PG_PSET_W;
			if(key == KEY_NUM3)
				rdata.StateId = PG_PSET_H;
			if(key == KEY_NUM4)
				rdata.StateId = PG_PSET_R;

			rdata.pos_len = 0;
			sprintf(rdata.tempbuf,"       ");
			return;
		}
		if(key == KEY_NUM5)
			rdata.Ptype = PSET_NONE;
		rdata.pos_len = rdata.StateId;
		rdata.StateId = PG_MENU1;
		return;
	}
	if((rdata.StateId >= PG_PSET_L) && (rdata.StateId <= PG_PSET_H)) {
		if(((key >= KEY_NUM0) && (key <= KEY_NUM9)) || (key == KEY_DOT)) {
			if(rdata.pos_len < 8){
				rdata.tempbuf[rdata.pos_len] = key;
				rdata.pos_len++;
			}
			return;
		}
		if(key == KEY_OK)
		{
			if(rdata.pos_len > 0)
			{
				if(rdata.StateId == PG_PSET_L){
					rdata.Plength = buf2double();
				}
				if(rdata.StateId == PG_PSET_W){
					rdata.Ptype = PSET_SQUARE;
					rdata.Pwidth = buf2double();
				}
				if(rdata.StateId == PG_PSET_H){
					rdata.Ptype = PSET_SQUARE;
					rdata.Pheight = buf2double();
				}
				if(rdata.StateId == PG_PSET_R){
					rdata.Ptype = PSET_RADIUS;
					rdata.Pradius = buf2double();
				}
				SaveToEEPROM();
			}
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_PSET;
			return;
		}
		if(key == KEY_TAB)
		{
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_MENU1;
			return;
		}
		if(key == KEY_CE) {
			rdata.pos_len = 0;
			rdata.StateId = PG_PSET;
			return;
		}
		return;
	}	
	if((rdata.StateId >= PG_HELP_ADDR) && (rdata.StateId <= PG_HELP_SET)) {
		rdata.pos_len = rdata.StateId;
		if(key == KEY_TAB)
			rdata.StateId = PG_MENU1;
		else
			rdata.StateId = PG_HELP;
		return;
	}

	return;
}
