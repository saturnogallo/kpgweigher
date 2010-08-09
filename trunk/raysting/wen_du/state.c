#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "eeprom.h"
#include "stdio.h"
#include "window.h"
#include "utili.h"
#include "scanner.h"

void mystrcpy(char* dst, char* src, uchar len)
{
	while(len-- != 0)
	{
		*dst++ = *src++;
	}

}
extern uchar nextwin;
#define REFRESH_TABLES1	sprintf(strbuf,"%i",curr_pos);draw_label(&chvalue,SW_NORMAL);\
		if(new_page == 1){\
			for(i = min_option; i <= max_option; i++){\
				j = ((curr_pos-1)/max_option)*max_option + i;
#define REFRESH_TABLES2	draw_label(&(items[i-1]),SW_NORMAL);\
				if((curr_pos-1) % max_option == (i-1))\
					draw_label(&(items[i-1]),SW_REVERSE);\
			}\
		}else{\
			for(i = min_option; i <= max_option; i++) {\
				if((last_pos-1) % max_option == (i-1))\
					draw_label(&(items[i-1]),SW_REVERSE);\
				if((curr_pos-1) % max_option == (i-1))\
					draw_label(&(items[i-1]),SW_REVERSE);\
			}\
		}	

#define INC_TABLE	last_pos = curr_pos;	curr_pos++;\
			if(curr_pos > max_index) curr_pos = min_index;\
			if(last_pos == curr_pos)	return;\
			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;}

#define DEC_TABLE	last_pos = curr_pos;   curr_pos--;\
			if(curr_pos < min_index) curr_pos = max_index;\
			if(last_pos == curr_pos)  return;\
			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;}

#define KEY_TABLE	if((msg > KEY_NUM0) && (msg-KEY_NUM0)*max_option <= max_index)\
					{new_page = 1; curr_pos = (msg-KEY_NUM1)*max_option+1;}

#define INIT_OPTIONS	for(i = min_option;i <= max_option; i++) {\
				draw_label(&options[i-1], SW_NORMAL);\
			}	
#define REFRESH_OPTIONS	if(last_sel == curr_sel)\
				return;\
			for(i = min_option;i <= max_option; i++){\
				if(last_sel == i)\
					draw_label(&options[i-1], SW_REVERSE);\
				if(curr_sel == i)\
					draw_label(&options[i-1], SW_REVERSE);\
			}

#define INC_OPTION	last_sel = curr_sel;\
			curr_sel++;\
			if(curr_sel > max_option)\
				curr_sel = min_option;
#define DEC_OPTION	last_sel = curr_sel;\
			curr_sel--;\
			if(curr_sel < min_option)\
				curr_sel = max_option;

extern int   curr_ch;		//index of current channel
extern int   curr_prb;	//index of current probe selection


void State_Init() {
	LoadFromEEPROM();
	if(IS_BORE_MODE) {
		display_buttons(KEY_BTN1,1);
		LoadProbeData(1); //load probe definition page 1
	}
	if(IS_THERM_MODE) {
		display_buttons(KEY_BTN1,0);
		LoadProbeData(1 + THERM_PRB_OFFSET);
	}
	rdata.has_scanner = scanner_is_available();
	if(rdata.has_scanner)
		sdata.scanmode = scanner_get_mode();
	//todo : check the available of scanner and set the scanmode
	if(IS_MODE_KTT)
		display_buttons(KEY_BTN2,1);
	else
		display_buttons(KEY_BTN2,0);
	display_buttons(KEY_BTN3,0);
	display_buttons(KEY_BTN4,0);
}

void pgmain_handler(uchar msg) {
	int i,old_ch;
	LABEL code chs[] = {
		{LBL_HZ12, 2, 3,3,strbuf},
		{LBL_HZ6X8,2,30,3,strbuf},
		{LBL_HZ6X8,2,40,3,strbuf},
		{LBL_HZ6X8,2,50,3,strbuf}
	};
	LABEL code values[] = {
		{LBL_HZ12, 40,3, 8,strbuf},
		{LBL_HZ6X8,40,30,8,strbuf},
		{LBL_HZ6X8,40,40,8,strbuf},
		{LBL_HZ6X8,40,50,8,strbuf}
	};
	LABEL code temps[] = {
		{LBL_HZ12, 140,3, 8,strbuf},
		{LBL_HZ6X8,140,30,8,strbuf},
		{LBL_HZ6X8,140,40,8,strbuf},
		{LBL_HZ6X8,140,50,8,strbuf}
	};
	
	//LABEL code usage = {LBL_HZ6X8,220,60,5,"usage"};

	if(msg == KEY_TAB) {
		if(IS_THERM_MODE)
			nextwin = PG_TCONFIG;
		else
			nextwin = PG_RCONFIG;
		return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
//		draw_label(&usage,SW_NORMAL);
		curr_ch = 1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		DEC_CH;	
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {

		INC_CH;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		old_ch = curr_ch;
		for(i=0;i<sizeof(chs)/sizeof(LABEL);i++)//
		{
			sprintf(strbuf,"CH%02i:",curr_ch);
			draw_label(&chs[i],SW_NORMAL);
			if( sdata.id[curr_ch - 1] == INVALID_PROBE ) {
				sprintf(strbuf,"*");
			}else{
			
				if(IS_THERM_MODE)
					sprintf(strbuf,"%8f mV",rdata.reading[curr_ch - 1]);
				else
					sprintf(strbuf,"%8f ohm",rdata.reading[curr_ch - 1]);
				draw_label(&values[i],SW_NORMAL);
				sprintf(strbuf,"%6f C",rdata.temperature[curr_ch - 1]);
				draw_label(&temps[i],SW_NORMAL);
				
			}
			if(1 == sdata.scanmode)
				break;
			INC_CH;
		}
		curr_ch = old_ch;
	}
}
//main configuration window of R
void pgrconfig_handler(uchar msg) {
	uchar i;
	LABEL code banner = {LBL_HZ16,3,3,1,"铂电阻参数配置"};
	LABEL code calibrate = {LBL_HZ16,30,30,1,"内标准校准中..."};
	LABEL code klbl = {LBL_HZ6X8, 88,28, 9,strbuf};
	LABEL code options[] = {
		{LBL_HZ16, 10,23,1,"1.电流换向"},
		{LBL_HZ16, 130,23,8,"2.内标准校准"},
		{LBL_HZ16, 10,45,8,"3.通道探头选择"},
		{LBL_HZ16,130,45,8,"4.设置探头参数"}
	};

//	LABEL code usage = {LBL_HZ6X8,100,55,5,"usage"};
	
	static uchar curr_sel = 1;
	static uchar last_sel = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(options)/sizeof(LABEL);

	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
		nextwin = PG_MAIN;
		return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
		sprintf(strbuf,"(%i)",sdata.ktime);
		draw_label(&klbl, SW_NORMAL);
		INIT_OPTIONS;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_NUM1) {
		window_setup(4);
		sprintf(strbuf,"电流换向时间设置");
		sdata.ktime = wnd_intinput(sdata.ktime);
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM2)
	{
		wnd_msgbox(&calibrate);
		//todo do calibration
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM3) {
		nextwin = PG_CHSET;
		return;
	}
	if(msg == KEY_NUM4) {
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS;
	}
}
//main configuration window of T
void pgtconfig_handler(uchar msg) {
	uchar i;
   	LABEL code banner = {LBL_HZ16,3,3,7,"热电偶参数配置"};
	LABEL code options[] = {
		{LBL_HZ16,40,25,8,"1.选择各通道探头"},
		{LBL_HZ16,40,45,8,"2.设置各探头参数"}
	};
	//LABEL code usage = {LBL_HZ6X8,100,55,5,"(1-9:select，TAB:return)"};
	static uchar curr_sel = 1;
	static uchar last_sel = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(options)/sizeof(LABEL);
	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
		nextwin = PG_MAIN;
		return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
//		draw_label(&usage, SW_NORMAL);
		INIT_OPTIONS;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_NUM1) {
		nextwin = PG_CHSET;
		return;
	}
	if(msg == KEY_NUM2) {
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS;
	}
	return;
}
//channel probe setup
void pgchset_handler(uchar msg) {
	LABEL code banner =    {LBL_HZ16,3,3,4,"通道设置"};
	//LABEL code usage = {LBL_HZ6X8,100,55,5,"usage"};
	LABEL code chvalue = {LBL_HZ6X8,130,3,2,strbuf};
	LABEL code items[] = {
		{LBL_HZ6X8, 10, 30, 10, strbuf},
		{LBL_HZ6X8, 130,30, 10, strbuf},
		{LBL_HZ6X8, 10, 40, 10, strbuf},
		{LBL_HZ6X8, 130,40, 10, strbuf},
		{LBL_HZ6X8, 10, 50, 10, strbuf},
		{LBL_HZ6X8, 130,50, 10, strbuf}
	};

	static uint curr_pos = 0;
	static uint last_pos = 0;
	const uchar code min_option = 1;
	const uchar code max_option = sizeof(items)/sizeof(LABEL);
	const uchar code min_index = 1;
	uchar max_index = sdata.scanmode;
	uchar new_page = 0;
	int i,j;
	if(msg == KEY_TAB || msg == KEY_CE) {
		if(IS_BORE_MODE)
			nextwin = PG_RCONFIG;
		else
			nextwin = PG_TCONFIG;
		return;
	}

	if(msg == KEY_UP) {
		DEC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {
		INC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_OK) {	//show window to select the probe
		curr_ch = curr_pos;
		nextwin = PG_PRBLIST;
		return;
	}
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
		KEY_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
//		draw_label(&usage, SW_NORMAL);
		curr_pos = 1;
		new_page = 1;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_TABLES1;
				if(sdata.id[j-1] == INVALID_PROBE)	{
					sprintf(strbuf,"CH%i:*",j);
				}else{
			//draw line item j
//	       	if(IS_THERM_MODE){
//				LoadProbeData(curr_pos + THERM_PRB_OFFSET);
//			}else{
//				LoadProbeData(curr_pos);
//			}
//				sprintf(strbuf,"%s",prbdata.name[(sdata.id[j]-1) % (PRBS_PER_SECTOR)]);
					sprintf(strbuf,"%s",prbdata.name[j-1]);
				}
		REFRESH_TABLES2;
	}
	return;
}
//select probe to edit.
void pgprbset_handler(uchar msg) {
	LABEL code banner = {LBL_HZ16,3,3,8,"请选择要配置的探头"};
	LABEL code chvalue = {LBL_HZ6X8,180,3,2,strbuf};
	LABEL code items[] = {
		{LBL_HZ6X8, 10, 30, 10, strbuf},
		{LBL_HZ6X8, 130,30, 10, strbuf},
		{LBL_HZ6X8, 10, 40, 10, strbuf},
		{LBL_HZ6X8, 130,40, 10, strbuf},
		{LBL_HZ6X8, 10, 50, 10, strbuf},
		{LBL_HZ6X8, 130,50, 10, strbuf}
	};
	//LABEL code usage = {LBL_HZ6X8,100,55,5,"usage"};
	static uint curr_pos = 1;
	static uint last_pos = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(items)/sizeof(LABEL);
	const uchar min_index = 1;
	const uchar max_index = MAX_PROBE_NUM;
	int i,j;
	uchar new_page = 1;
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
		KEY_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_TAB || msg == KEY_CE)
	{
		if(IS_THERM_MODE)
			nextwin = PG_TCONFIG;
		else
			nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_OK)
	{
		curr_prb = curr_pos;
		if(IS_THERM_MODE)
			nextwin = PG_TPRBCONFIG;
		else
			nextwin = PG_RPRBCONFIG;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
//		draw_label(&usage, SW_NORMAL);
		curr_pos = 1;
		new_page = 1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		DEC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {
		INC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_TABLES1;	
			sprintf(strbuf,"%s",prbdata.name[j-1]);
			draw_label(&(items[i-1]),SW_NORMAL);
		REFRESH_TABLES2;
	}
	return;
}
//select probe for channel setup
void pgprblist_handler(uchar msg) {
	LABEL code banner = {LBL_HZ16,3,3,8,strbuf};
	LABEL code chvalue = {LBL_HZ6X8,180,3,2,strbuf};
	LABEL code items[] = {
		{LBL_HZ6X8, 10, 30, 10, strbuf},
		{LBL_HZ6X8, 130,30, 10, strbuf},
		{LBL_HZ6X8, 10, 40, 10, strbuf},
		{LBL_HZ6X8, 130,40, 10, strbuf},
		{LBL_HZ6X8, 10, 50, 10, strbuf},
		{LBL_HZ6X8, 130,50, 10, strbuf}
	};
	//LABEL code usage = {LBL_HZ6X8,100,55,5,"usage"};
	static uint curr_pos = 1;
	static uint last_pos = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(items)/sizeof(LABEL);
	const uchar min_index = 1;
	const uchar max_index = MAX_PROBE_NUM;
	uchar i,j;
	uchar new_page = 1;
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
		KEY_TABLE;
		msg = MSG_REFRESH;
	}

	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK)
	{
		sdata.id[curr_ch-1] = curr_pos-1;
		nextwin = PG_CHSET;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
		draw_label(&banner, SW_NORMAL);
//		draw_label(&usage, SW_NORMAL);
		curr_pos = 1;
		new_page = 1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		DEC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {
		INC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_TABLES1;	
			sprintf(strbuf,"%s",prbdata.name[j-1]);
			draw_label(&(items[i-1]),SW_NORMAL);
		REFRESH_TABLES2;
	}
	return;
}
//select probe type list
void pgprbtypelist_handler(uchar msg) {
	LABEL code banner = {LBL_HZ16,3,3,8,"prb type config"};
	LABEL code options[] = {
		{LBL_HZ16,30,20,5,"PT100"},
		{LBL_HZ16,30,20,5,"PT25"},
		{LBL_HZ16,130,20,5,"K-TYPE"},
		{LBL_HZ16,30,40,5,"R-TYPE"},
		{LBL_HZ16,130,40,5,"S-TYPE"},
	};
	LABEL code usage = {LBL_HZ6X8,100,55,5,"usage"};
	static uchar curr_sel = 1;
	static uchar last_sel = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(options)/sizeof(LABEL);
	uchar i;
	if(msg >= KEY_NUM1 && msg <= KEY_NUM5) {
		//todo set the type of the current probe
		switch(msg)
		{
			case KEY_NUM1:
			       prbdata.type[curr_prb-1] = PRBTYPE_PT100;
			       break;
			case KEY_NUM2:
			       prbdata.type[curr_prb-1] = PRBTYPE_PT25;
			       break;
			case KEY_NUM3:
			       prbdata.type[curr_prb-1] = PRBTYPE_K;
			       break;
		}
		msg = KEY_OK;
	}

	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
		if(prbdata.type[curr_prb-1] && PRBTYPE_BIT)
			nextwin = PG_RPRBCONFIG;
		else
			nextwin = PG_TPRBCONFIG;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
		draw_label(&usage, SW_NORMAL);
		INIT_OPTIONS;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		DEC_OPTION;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {
		INC_OPTION;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS;
	}
	return;
}

//config r probe parameter
void pgrprbconfig_handler(uchar msg) {
	LABEL code banner = {LBL_HZ16,3,3,8,"R prb set"};
	LABEL code snlbl = {LBL_HZ16,30,20,10,strbuf};
	LABEL code typelbl = {LBL_HZ16,30,20,10,strbuf};
	LABEL code paramlbl1 = {LBL_HZ16,30,20,10,strbuf};
	LABEL code paramlbl2 = {LBL_HZ16,30,20,10,strbuf};
	LABEL code paramlbl3 = {LBL_HZ16,30,20,10,strbuf};
	LABEL code paramlbl4 = {LBL_HZ16,30,20,10,strbuf};
	
	LABEL code usage = {LBL_HZ6X8,100,55,5,"usage"};
	
	if(msg == KEY_TAB || msg == KEY_OK) {
		SaveToEEPROM();
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == KEY_CE) {
		LoadProbeData(curr_prb);
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		draw_label(&banner,SW_NORMAL);
		draw_label(&usage,SW_NORMAL);
		sprintf(strbuf,"1.sn (%s)",prbdata.name[curr_prb-1]);
		draw_label(&snlbl,SW_NORMAL);
		sprintf(strbuf,"2.type (%i)",prbdata.type[curr_prb-1]);
		draw_label(&typelbl,SW_NORMAL);
		sprintf(strbuf,"3.param1 (%f)",prbdata.param1[curr_prb-1]);
		draw_label(&paramlbl1,SW_NORMAL);
		sprintf(strbuf,"3.param2 (%f)",prbdata.param1[curr_prb-1]);
		draw_label(&paramlbl2,SW_NORMAL);
		sprintf(strbuf,"3.param3 (%f)",prbdata.param1[curr_prb-1]);
		draw_label(&paramlbl3,SW_NORMAL);
		sprintf(strbuf,"3.param4 (%f)",prbdata.param1[curr_prb-1]);
		draw_label(&paramlbl4,SW_NORMAL);
		msg = MSG_REFRESH;
	}
	if(msg == KEY_NUM1) {
		window_setup(4);
		sprintf(strbuf,"sn input");
		wnd_sninput(prbdata.name[curr_prb-1]);
		return;
	}
	if(msg == KEY_NUM2){
		nextwin = PG_PRBTYPELIST;
		return;
	}
	if(msg == KEY_NUM3 ){
		window_setup(8);
		sprintf(strbuf,"p1 input");
		prbdata.param1[curr_prb-1] = wnd_floatinput(prbdata.param1[curr_prb-1]);
		return;
	}
	if(msg == KEY_NUM4) {
		window_setup(8);
		sprintf(strbuf,"p2 input");
		prbdata.param2[curr_prb-1] = wnd_floatinput(prbdata.param2[curr_prb-1]);
		return;
	}
	if(msg == KEY_NUM5) {
		window_setup(8);
		sprintf(strbuf,"p3 input");
		prbdata.param3[curr_prb-1] = wnd_floatinput(prbdata.param3[curr_prb-1]);
		return;
	}
	if(msg == KEY_NUM6) {
		window_setup(8);
		sprintf(strbuf,"p4 input");
		prbdata.param4[curr_prb-1] = wnd_floatinput(prbdata.param4[curr_prb-1]);
		return;
	}
	
	return;
}

//config t probe parameter
void pgtprbconfig_handler(uchar msg) {
	return;
}


