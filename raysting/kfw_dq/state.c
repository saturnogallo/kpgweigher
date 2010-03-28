#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "eeprom.h"
#include "stdio.h"


extern void State_Display();
extern ulong code Sector[10][4];

#define RS_SECTBASE		Sector[0][0]	//[0]

extern uchar rcv_pos;
long ltemp;

void LoadFromEEPROM()
{
	uint i = RS_SECTBASE;
	uchar* arr = (uchar*)&rdata;

	for(i=RS_SECTBASE;i<(RS_SECTBASE+sizeof(RUNDATA));i++)
	{
		*arr++ = byte_read(i);
	}
}
void SaveToEEPROM()
{
	uint i = RS_SECTBASE;
	uchar* arr = (uchar*)&rdata;
	SectorErase(Sector[0][0]);

	for(i=RS_SECTBASE;i<(RS_SECTBASE+sizeof(RUNDATA));i++)
	{
		byte_write(i,*arr++);
	}
}
extern void decode_key();
extern uchar keykeep;
void State_Init()
{
	rdata.StateId = PG_MAIN;
	rdata.pos_len = 0;
	rdata.Delay = 10;
	rdata.delay_cnt = rdata.Delay << DELAY_MULTIPLE;

	rdata.R0 = 0.0;
	rdata.Rs1 = 1;

	rdata.Rcali[RANGE_200u] = 20000;
	rdata.Rcali[RANGE_2mo] = 10000;
	rdata.Rcali[RANGE_20mo] = 1000;	
	rdata.Rcali[RANGE_200m] = 1000;		
	rdata.Rcali[RANGE_2] = 100;
	rdata.Rcali[RANGE_20] = 10;
	rdata.Rcali[RANGE_200] = 10;
	rdata.Rcali[RANGE_2k] = 1;
	rdata.Rcali[RANGE_20k] = 1;
	rdata.Rcali[RANGE_200k] = 1;


	rdata.Pheight = rdata.Pwidth = rdata.Pradius = 1;
	rdata.Rauto = AUTO_OFF;
	rdata.Rktt = KTT_OFF;
	rdata.Plength = 1;
	rdata.Ptype = PSET_NONE;
	rdata.Pvalue = 1;
	rdata.Temp = 25;
	rdata.Range = RANGE_200k;
	rdata.Baudrate = 0;
	rdata.Current = CURRENT_1;
	sprintf(rdata.tempbuf,"         ");
	decode_key();
//	if(keykeep == KEY_OK)
//		SaveToEEPROM();
//	LoadFromEEPROM();
	display_buttons(KEY_BTN1,rdata.Rauto);
	display_buttons(KEY_BTN2,rdata.Rktt);
	rdata.StateId = PG_MAIN;
	rdata.pos_len = 0;
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
			sprintf(rdata.tempbuf,"        ");
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
			rdata.StateId = PG_HELP;
			rdata.pos_len = 0;
			return;
			
		case  KEY_DN:
			if(rdata.pos_len == PG_HELP){
					rdata.pos_len =PG_RANGE;
					return;
			}
			if(rdata.pos_len == PG_RANGE){
					rdata.pos_len =PG_CALISET ;
					return;
			}
			if(rdata.pos_len == PG_CALISET){
					rdata.pos_len =PG_MSG_RZERO ;
					return;
			}
			if(rdata.pos_len == PG_MSG_RZERO){
					rdata.pos_len =PG_PSET ;
					return;
			}
			if(rdata.pos_len == PG_PSET){
					rdata.pos_len =PG_HELP ;
					return;
			}
			return;
		case  KEY_UP:
			if(rdata.pos_len == PG_RANGE){
					rdata.pos_len =PG_HELP;
					return;
			}
			if(rdata.pos_len == PG_CALISET){
					rdata.pos_len = PG_RANGE ;
					return;
			}
			if(rdata.pos_len == PG_MSG_RZERO){
					rdata.pos_len = PG_CALISET;
					return;
			}
			if(rdata.pos_len == PG_PSET){
					rdata.pos_len = PG_MSG_RZERO ;
					return;
			}
			if(rdata.pos_len == PG_HELP){
					rdata.pos_len = PG_PSET;
					return;
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
			if(rdata.pos_len >= RANGE_200k)
				rdata.pos_len = RANGE_200u;
			else
				rdata.pos_len++;
			return;
		}
		if(key == KEY_DN) {
			if(rdata.pos_len == RANGE_200u)
				rdata.pos_len = RANGE_200k;
			else
				rdata.pos_len--;
			return;
		}

		if((key >= KEY_NUM0) && (key <= KEY_NUM9)){
			rdata.pos_len = RANGE_200u + key - KEY_NUM0;
			key = KEY_OK;
		}
		if((key == KEY_OK) || (key == KEY_TAB)||(key == KEY_CE))
		{
			if(key == KEY_OK)
			{
				rdata.Range = rdata.pos_len;
				DBG(CMD_RANGE_BASE+rdata.Range);
				SaveToEEPROM();
			}
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_MENU1;
			return;
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
		if((key == KEY_OK) || (key == KEY_TAB)||(key == KEY_CE))
		{
			if((key == KEY_OK) && (rdata.pos_len > 0))
			{
				//conduct calibration here
				while(1)
				{
					rcv_pos = 0;
					DBG(CMD_QUERY);
					while(rcv_pos <8)
						;
					if(get_voltage() == 0)
						continue;
					
					rdata.Rcali[rdata.Range] = ch1val/buf2double();
					SaveToEEPROM();
					rcv_pos = 0;
					break;
				}
			}
			rdata.pos_len = rdata.StateId;
			rdata.StateId = PG_MENU1;
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
		if(key == KEY_DN)
		{
			if(rdata.pos_len >= 4)
				rdata.pos_len = 0;
			else
				rdata.pos_len++;
			return;
		}
		if(key == KEY_UP)
		{
			if(rdata.pos_len == 0)
				rdata.pos_len = 4;
			else
				rdata.pos_len--;
			return;
		}

		if(key == KEY_OK)
			key = KEY_NUM1 + rdata.pos_len;

		if((key >= KEY_NUM1) && (key <= KEY_NUM4))
		{
			if(key == KEY_NUM1)
				rdata.StateId = PG_PSET_L;
			if(key == KEY_NUM2)
			{
				rdata.StateId = PG_PSET_W;
				rdata.Ptype = PSET_SQUARE;
			}
			if(key == KEY_NUM3)
			{
				rdata.StateId = PG_PSET_H;
				rdata.Ptype = PSET_SQUARE;
			}
			if(key == KEY_NUM4)
			{
				rdata.StateId = PG_PSET_R;
				rdata.Ptype = PSET_RADIUS;
			}

			rdata.pos_len = 0;
			sprintf(rdata.tempbuf,"         ");
			return;
		}
		if(key == KEY_NUM5)
		{
			rdata.Ptype = PSET_NONE;
			rdata.pos_len = 0;
			return;
		}
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
