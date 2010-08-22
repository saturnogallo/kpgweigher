#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "eeprom.h"
#include "window.h"
#include "utili.h"
#include "scanner.h"

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
	LoadFromEEPROM(); //load sdata structure
	if(IS_BORE_MODE) {
		display_buttons(KEY_BTN1,1);
	}else{
		display_buttons(KEY_BTN1,0);

	}
	LoadProbeData();
	if(IS_MODE_KTT)
		display_buttons(KEY_BTN2,1);
	else
		display_buttons(KEY_BTN2,0);
	display_buttons(KEY_BTN3,0); //probe type
	display_buttons(KEY_BTN4,0); //zero
}
char* getprbtype(uchar);
void pgmain_handler(uchar msg) {
	int i,old_ch;
	char *cptr;
	LABEL code chs[] = {
		{LBL_HZ12, 4, 3,3,strbuf},
		{LBL_HZ6X8,2,30,3,strbuf},
		{LBL_HZ6X8,2,40,3,strbuf},
		{LBL_HZ6X8,2,50,3,strbuf}
	};
	LABEL code values[] = {
		{LBL_HZ12, 40,3, 7,strbuf},
		{LBL_HZ6X8,40,30,10,strbuf},
		{LBL_HZ6X8,40,40,10,strbuf},
		{LBL_HZ6X8,40,50,10,strbuf}
	};
	LABEL code temps[] = {
		{LBL_HZ12, 140,3, 7,strbuf},
		{LBL_HZ6X8,140,30,10,strbuf},
		{LBL_HZ6X8,140,40,10,strbuf},
		{LBL_HZ6X8,140,50,10,strbuf}
	};
	
	//LABEL code usage = {LBL_HZ6X8,220,60,5,"usage"};

	if(msg == KEY_TAB) {
		if(IS_THERM_MODE)
		{
			nextwin = PG_TCONFIG;
		}else{
			nextwin = PG_RCONFIG;
		}
		return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
//		draw_label(&usage,SW_NORMAL);
//		curr_ch = 1;
		old_ch = curr_ch;
		for(i=0;i<sizeof(chs)/sizeof(LABEL);i++)//
		{
			sprintf(strbuf,"CH%02i:",curr_ch);
			draw_label(&chs[i],SW_NORMAL);
			INC_CH;
		}
		curr_ch = old_ch;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		DEC_CH;	
		old_ch = curr_ch;
		for(i=0;i<sizeof(chs)/sizeof(LABEL);i++)//
		{
			sprintf(strbuf,"CH%02i:",curr_ch);
			draw_label(&chs[i],SW_NORMAL);
			INC_CH;
		}
		curr_ch = old_ch;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {

		INC_CH;
		old_ch = curr_ch;
		for(i=0;i<sizeof(chs)/sizeof(LABEL);i++)//
		{
			sprintf(strbuf,"CH%02i:",curr_ch);
			draw_label(&chs[i],SW_NORMAL);
			INC_CH;
		}
		curr_ch = old_ch;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		old_ch = curr_ch;
		for(i=0;i<sizeof(chs)/sizeof(LABEL);i++)//
		{
//			sprintf(strbuf,"CH%02i:",curr_ch);
//			draw_label(&chs[i],SW_NORMAL);
			cptr = getprbtype(prbdata.type[sdata.id[curr_ch-1]]);
			if((sdata.id[curr_ch - 1] == INVALID_PROBE) ||\
				(rdata.temperature[curr_ch - 1] < -90000) ||\
				((IS_THERM_MODE) && (cptr[0] == 'P'))||\ 
				((IS_BORE_MODE) && (cptr[0] != 'P'))){
				sprintf(strbuf," -------");
				draw_label(&values[i],SW_NORMAL|SW_OVERLAP);
				draw_label(&temps[i],SW_NORMAL|SW_OVERLAP);
			}else{
				if(IS_THERM_MODE)
				{
					sprintf(strbuf,"%8f",rdata.reading[curr_ch - 1]);
					sprintf(strbuf+8," mV");
				}else{
					sprintf(strbuf,"%8f",rdata.reading[curr_ch - 1]);
					sprintf(strbuf+8," ohm");
				}
				draw_label(&values[i],SW_NORMAL|SW_OVERLAP);
				sprintf(strbuf," %8f",rdata.temperature[curr_ch - 1]);
				sprintf(strbuf+8," C,%c",cptr[0]);
				draw_label(&temps[i],SW_NORMAL|SW_OVERLAP);
			}
//			if(1 == rdata.scanmode)
//				break;
			INC_CH;
		}
		curr_ch = old_ch;
	}
}
//main configuration window of R
extern uint dlg_cnt;
extern double valuep;
void pgrconfig_handler(uchar msg) {
	uchar i;
	LABEL code banner = {LBL_HZ16,3,3,1,"铂电阻参数配置"};
	LABEL code calibrate = {LBL_HZ16,30,30,8,"内标准校准中..."};
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
		SaveToEEPROM();
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM2)
	{
		valuep = 0;
		while(valuep == 0){
			sprintf(strbuf,"请输入外标准(CH1)阻值");
			valuep = wnd_floatinput(valuep);
		}
		wnd_msgbox(&calibrate);
		sm_write(1);//set to channel 1
		sm_action(CMD_DMM_PKTT);
		sm_action(CMD_DMM_RS);
		dlg_cnt = sdata.ktime * ONESEC;
		while(dlg_cnt-- > 1)
			;		
		sm_action(CMD_DMM_VUPDATE);
		rdata.stdV = smget_double(CMDR_DMM_VOLT);
		sm_action(CMD_DMM_NKTT);
		dlg_cnt = sdata.ktime * ONESEC;
		while(dlg_cnt-- > 1)
			;		
		sm_action(CMD_DMM_VUPDATE);
		rdata.stdV = rdata.stdV + smget_double(CMDR_DMM_VOLT);
		sm_action(CMD_DMM_RX);
		dlg_cnt = sdata.ktime * ONESEC;
		while(dlg_cnt-- > 1)
			;		
		sm_action(CMD_DMM_VUPDATE);
		sdata.Rs1 = smget_double(CMDR_DMM_VOLT);
		sm_action(CMD_DMM_PKTT);
		dlg_cnt = sdata.ktime * ONESEC;
		while(dlg_cnt-- > 1)
			;		
		sm_action(CMD_DMM_VUPDATE);
		sdata.Rs1 = sdata.Rs1 + smget_double(CMDR_DMM_VOLT);
		
		sdata.Rs1 = sdata.Rs1 * valuep / rdata.stdV;

		SaveToEEPROM();
		
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
	uchar max_index = MAX_CH_NUM;//rdata.scanmode;
	uchar new_page = 0;
	int i,j;
	if(msg == KEY_TAB || msg == KEY_CE) {
		if(msg == KEY_TAB)
			SaveToEEPROM();
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
	if(msg == KEY_DOT)
	{
		sdata.id[curr_pos - 1] = INVALID_PROBE;
		SaveToEEPROM();
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
				if(sdata.id[j-1] == INVALID_PROBE){
					sprintf(strbuf,"CH%i:*",j);
				}else{
				//draw line item j
				sprintf(strbuf,"CH%i:%s",j,prbdata.name[(sdata.id[j-1])]);
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
	if(msg == KEY_CE || msg == KEY_TAB)
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
		nextwin = PG_PRBCONFIG;
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
	if(msg == KEY_CE)
	{
		nextwin = PG_CHSET;
		return;
	}
	if(msg == KEY_TAB || msg == KEY_OK)
	{
		sdata.id[curr_ch-1] = curr_pos - 1;
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
	LABEL code banner = {LBL_HZ16,3,3,8,strbuf};
	LABEL code options[] = {
		{LBL_HZ6X8,10,20,8,"1.PT100"},
		{LBL_HZ6X8,80,20,8,"2.PT25"},
		{LBL_HZ6X8,10,30,8,"3.K-TYPE"},
		{LBL_HZ6X8,80,30,8,"4.N-TYPE"},
		{LBL_HZ6X8,150,30,8,"5.E-TYPE"},
		{LBL_HZ6X8,10,40,8,"6.B-TYPE"},
		{LBL_HZ6X8,80,40,8,"7.J-TYPE"},
		{LBL_HZ6X8,150,40,8,"8.S-TYPE"},
		{LBL_HZ6X8,10,50,8,"9.R-TYPE"},
	};
//	LABEL code usage = {LBL_HZ6X8,100,55,5,"usage"};
	static int curr_sel = 1;
	static int last_sel = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(options)/sizeof(LABEL);
	uchar i;
	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
		//todo set the type of the current probe
		switch(msg)
		{
			case KEY_NUM1:
			       prbdata.type[curr_prb-1] = PRBTYPE_PT100;
			       break;
			case KEY_NUM2:
			       prbdata.type[curr_prb-1] = PRBTYPE_PT25;
			       break;
			default:
			       prbdata.type[curr_prb-1] = msg-KEY_NUM3+PRBTYPE_K;
			       break;
		}
		msg = KEY_OK;
		SaveProbeData();
	}

	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
		draw_label(&banner, SW_NORMAL);
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
char* getprbtype(uchar prbtype)
{
	switch(prbtype)
	{
		case PRBTYPE_PT25: 	return "PT  25";
		case PRBTYPE_PT100:	return "PT 100";
		case PRBTYPE_K:		return "K TYPE";
		case PRBTYPE_N:		return "N TYPE";
		case PRBTYPE_E:		return "E TYPE";
		case PRBTYPE_B:		return "B TYPE";
		case PRBTYPE_J:		return "J TYPE";
		case PRBTYPE_S:		return "S TYPE";
		case PRBTYPE_R:		return "R TYPE";
	}
	return "? TYPE";
}
//config r probe parameter
void pgprbconfig_handler(uchar msg) {
	LABEL code banner = {LBL_HZ12,3,3,8,strbuf};
	LABEL code snlbl = {LBL_HZ12,10,17,4,"1.序列号"};
	LABEL code snval = {LBL_HZ6X8,75,22,10,strbuf};
	LABEL code typelbl = {LBL_HZ12,10,31,3,"2.类型"};
	LABEL code typeval = {LBL_HZ6X8,59,36,10,strbuf};
	LABEL code paramlbl1 = {LBL_HZ12,130,31,3,"3.a"};
	LABEL code paramval1 = {LBL_HZ6X8,165,36,10,strbuf};
	LABEL code paramlbl2 = {LBL_HZ12,10,45,3,"4.b"};
	LABEL code paramval2 = {LBL_HZ6X8,45,50,10,strbuf};
	LABEL code paramlbl3 = {LBL_HZ12,130,45,3,"5.c"};
	LABEL code paramval3 = {LBL_HZ6X8,165,50,10,strbuf};

	
//	LABEL code usage = {LBL_HZ6X8,100,55,5,"usage"};
	
	if(msg == KEY_TAB || msg == KEY_OK) {
		SaveProbeData();
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == KEY_CE) {
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == MSG_INIT)
	{
//		prbdata.param1[curr_prb-1] = prbdata.param2[curr_prb-1] =prbdata.param3[curr_prb-1] = 1.1;
		LCD_Cls();
		sprintf(strbuf,"探头%i参数设置",curr_prb);
		draw_label(&banner,SW_NORMAL);
		sprintf(strbuf,":%s",prbdata.name[curr_prb-1]);
		draw_label(&snlbl,SW_NORMAL);
		draw_label(&snval,SW_NORMAL);
		sprintf(strbuf,":%s",getprbtype(prbdata.type[curr_prb-1]));
		draw_label(&typelbl,SW_NORMAL);
		draw_label(&typeval,SW_NORMAL);
		sprintf(strbuf,":%7f",prbdata.param1[curr_prb-1]);
		draw_label(&paramlbl1,SW_NORMAL);
		draw_label(&paramval1,SW_NORMAL);
		sprintf(strbuf,":%7f",prbdata.param2[curr_prb-1]);
		draw_label(&paramlbl2,SW_NORMAL);
		draw_label(&paramval2,SW_NORMAL);
		sprintf(strbuf,":%7f",prbdata.param3[curr_prb-1]);
		draw_label(&paramlbl3,SW_NORMAL);
		draw_label(&paramval3,SW_NORMAL);
		return;
	}
	if(msg == KEY_NUM1) {
		window_setup(7);
		sprintf(strbuf,"请输入探头序列号");
		wnd_sninput(prbdata.name[curr_prb-1]);
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == KEY_NUM2){
		nextwin = PG_PRBTYPELIST;
		return;
	}
	if(msg == KEY_NUM3 ){
		window_setup(8);
		sprintf(strbuf,"输入系数a");
		prbdata.param1[curr_prb-1] = wnd_floatinput(prbdata.param1[curr_prb-1]);
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == KEY_NUM4) {
		window_setup(8);
		sprintf(strbuf,"输入系数b");
		prbdata.param2[curr_prb-1] = wnd_floatinput(prbdata.param2[curr_prb-1]);
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == KEY_NUM5) {
		window_setup(8);
		sprintf(strbuf,"输入系数c");
		prbdata.param3[curr_prb-1] = wnd_floatinput(prbdata.param3[curr_prb-1]);
		nextwin = PG_PRBCONFIG;
		return;
	}
	return;
}


void pgboottype_handler(uchar msg) {
	uchar i;
	LABEL code banner = {LBL_HZ16,3,3,1,"请选择工作模式"};
	LABEL code options[] = {
		{LBL_HZ16, 10,23,1,"1.铂电阻测量"},
		{LBL_HZ16, 130,23,1,"2.铂电阻配置"},
		{LBL_HZ16, 10,45,1,"3.热电偶测量"},
		{LBL_HZ16, 130,45,8,"4.热电偶配置"},
	};
	
	static uchar curr_sel = 1;
	static uchar last_sel = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(options)/sizeof(LABEL);

	if(msg == MSG_INIT) {
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
		INIT_OPTIONS;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_NUM1) {
		SET_BORE_MODE;
		sm_action(CMD_DMM_RX);
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_NUM2) {
		SET_BORE_MODE;
		sm_action(CMD_DMM_RX);
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM3) {
		SET_THERM_MODE;
		sm_action(CMD_DMM_RS);
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_NUM4) {
		SET_THERM_MODE;
		sm_action(CMD_DMM_RS);
		nextwin = PG_TCONFIG;
		return;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS;
	}
}


