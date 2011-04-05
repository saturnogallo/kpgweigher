#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "eeprom.h"
#include "window.h"
#include "utili.h"
#include "scanner.h"

extern uchar prb_type[];
extern double prb_param1[];
extern double prb_param2[];
extern double prb_param3[];
extern char   prb_name[];
extern uchar  prb_type[];

double valuep;
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

extern int   curr_ch;		//index of current channel in menu
extern int   curr_prb;	//index of current probe selection in menu

char* getprbtype(uchar);
void pgmain_handler(uchar msg) {
	int i,old_ch;
	char *cptr;
	LABEL code chs[] = {
		{LBL_HZ12, 4, 3,3,strbuf}, {LBL_HZ6X8,2,30,3,strbuf}, {LBL_HZ6X8,2,40,3,strbuf}, {LBL_HZ6X8,2,50,3,strbuf}
	};
	LABEL code values[] = {
		{LBL_HZ12, 40,3, 7,strbuf}, {LBL_HZ6X8,40,30,10,strbuf}, {LBL_HZ6X8,40,40,10,strbuf}, {LBL_HZ6X8,40,50,10,strbuf}
	};
	LABEL code temps[] = {
		{LBL_HZ12, 140,3, 7,strbuf}, {LBL_HZ6X8,140,30,10,strbuf}, {LBL_HZ6X8,140,40,10,strbuf}, {LBL_HZ6X8,140,50,10,strbuf}
	};
	
	
	if(msg == KEY_TAB) {
		if(IS_THERM_MODE)
		{
			irun[IRUN_NEW_PG] = PG_TCONFIG;
		}else{
			irun[IRUN_NEW_PG] = PG_RCONFIG;
		}
		return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
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
			cptr = getprbtype(prb_type[ch_prbid[curr_ch-1]]);
			if((ch_prbid[curr_ch - 1] == INVALID_PROBE) ||\
				(vreading[curr_ch - 1] < -90000) ||\
				((IS_THERM_MODE) && (cptr[0] == 'P'))||\ 
				((IS_BORE_MODE) && (cptr[0] != 'P'))){
				sprintf(strbuf," -------");
				draw_label(&values[i],SW_NORMAL|SW_OVERLAP);
				draw_label(&temps[i],SW_NORMAL|SW_OVERLAP);
			}else{
				if(IS_THERM_MODE)
				{
					sprintf(strbuf,"%8f",vreading[curr_ch - 1]);
					sprintf(strbuf+8," mV");
				}else{
					sprintf(strbuf,"%8f",vreading[curr_ch - 1]);
					sprintf(strbuf+8," ohm");
				}
				draw_label(&values[i],SW_NORMAL|SW_OVERLAP);
				sprintf(strbuf," %8f", reading[curr_ch - 1]);
				sprintf(strbuf+8," C,%c",cptr[0]);
				draw_label(&temps[i],SW_NORMAL|SW_OVERLAP);
			}
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
	LABEL code klbl = {LBL_HZ6X8, 88,28, 9,strbuf}; //label for ktt display
	LABEL code options[] = {
		{LBL_HZ16, 10,23,1,"1.电流换向"},
		{LBL_HZ16,130,23,8,"2.内标准校准"},
		{LBL_HZ16, 10,45,8,"3.通道探头选择"},
		{LBL_HZ16,130,45,8,"4.设置探头参数"}
	};

	static uchar curr_sel = 1;
	static uchar last_sel = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(options)/sizeof(LABEL);

	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
		irun[IRUN_NEW_PG] = PG_MAIN;
		return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
		sprintf(strbuf,"(%i)",irun[IRUN_KTIME]);
		draw_label(&klbl, SW_NORMAL);
		INIT_OPTIONS;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_NUM1) {
		window_setup(4); //max data length
		sprintf(strbuf,"电流换向时间设置");
		if(1 == wnd_intinput())
			send_byte(SRUN_NEWKTT,buf2byte());
		else
			irun[IRUN_NEW_PG] = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM2)
	{
		valuep = 0;
		sprintf(strbuf,"请输入外标准(CH1)阻值");
		if(1 == wnd_floatinput(valuep))
		{
			valuep = buf2double();
			send_double(SRUN_CALIB,(uchar*)&valuep,4);
			wnd_msgbox(&calibrate);
		}else{
			irun[IRUN_NEW_PG] = PG_RCONFIG;
		}
		return;
	}
	if(msg == KEY_NUM3) {
		irun[IRUN_NEW_PG] = PG_CHSET;
		return;
	}
	if(msg == KEY_NUM4) {
		irun[IRUN_NEW_PG] = PG_PRBSET;
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
		{LBL_HZ16,40,25,8,"1.选择各通道探头"}, {LBL_HZ16,40,45,8,"2.设置各探头参数"}
	};
	static uchar curr_sel = 1;
	static uchar last_sel = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(options)/sizeof(LABEL);
	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
		irun[IRUN_NEW_PG] = PG_MAIN;
		return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
		INIT_OPTIONS;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_NUM1) {
		irun[IRUN_NEW_PG] = PG_CHSET;
		return;
	}
	if(msg == KEY_NUM2) {
		irun[IRUN_NEW_PG] = PG_PRBSET;
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
	uchar max_index = MAX_CH_NUM;
	uchar new_page = 0;
	int i,j;
	if(msg == KEY_TAB || msg == KEY_CE) {
		if(irun[IRUN_MODE] == THERM_MODE)
			irun[IRUN_NEW_PG] = PG_TCONFIG;
		else
			irun[IRUN_NEW_PG] = PG_RCONFIG;
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
		irun[IRUN_NEW_PG] = PG_PRBLIST;
		return;
	}
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
		KEY_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DOT)
	{
		msg = INVALID_PROBE;
		send_double(SRUN_CHPRB_BASE + curr_pos,&msg,1);
		return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
		curr_pos = 1;
		new_page = 1;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_TABLES1;
				if(ch_prbid[j-1] == INVALID_PROBE){
					sprintf(strbuf,"CH%i:*",j);
				}else{ //draw line item j
					sprintf(strbuf,"CH%i:%s",j,prb_name[(ch_prbid[j-1])]);
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
		if(irun[IRUN_MODE] == THERM_MODE) //thermal mode
			irun[IRUN_NEW_PG] = PG_TCONFIG;
		else
			irun[IRUN_NEW_PG] = PG_RCONFIG;
		return;
	}
	if(msg == KEY_OK)
	{
		curr_prb = curr_pos;
		irun[IRUN_NEW_PG] = PG_PRBCONFIG;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		draw_label(&banner, SW_NORMAL);
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
			sprintf(strbuf,"%s",prb_name[j-1]);
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
		irun[IRUN_NEW_PG] = PG_CHSET;
		return;
	}
	if(msg == KEY_TAB || msg == KEY_OK)
	{
		send_byte(SRUN_CHPRB_BASE+curr_ch, curr_pos);
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
		draw_label(&banner, SW_NORMAL);
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
			sprintf(strbuf,"%s",prb_name[j-1]);
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
	static int curr_sel = 1;
	static int last_sel = 1;
	const uchar min_option = 1;
	const uchar max_option = sizeof(options)/sizeof(LABEL);
	uchar i;
	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
		if(msg == KEY_NUM1)
		{
		    send_byte(SRUN_SET_PRBT,PRBTYPE_PT100);
		}
		else if(msg == KEY_NUM2)
		{
		    send_byte(SRUN_SET_PRBT,PRBTYPE_PT25);
		}else{
			send_byte(SRUN_SET_PRBT,msg-KEY_NUM3+PRBTYPE_K); 
		}
		return;
	}

	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
		irun[IRUN_NEW_PG] = PG_PRBCONFIG;
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
	
	
	if(msg == KEY_TAB || msg == KEY_OK) {
		irun[IRUN_NEW_PG] = PG_PRBSET;
		return;
	}
	if(msg == KEY_CE) {
		irun[IRUN_NEW_PG] = PG_PRBSET;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		sprintf(strbuf,"探头%i参数设置",curr_prb);
		draw_label(&banner,SW_NORMAL);
		sprintf(strbuf,":%s",prb_name[curr_prb-1]);
		draw_label(&snlbl,SW_NORMAL);
		draw_label(&snval,SW_NORMAL);
		sprintf(strbuf,":%s",getprbtype(prb_type[curr_prb-1]));
		draw_label(&typelbl,SW_NORMAL);
		draw_label(&typeval,SW_NORMAL);
		sprintf(strbuf,":%7f",prb_param1[curr_prb-1]);
		draw_label(&paramlbl1,SW_NORMAL);
		draw_label(&paramval1,SW_NORMAL);
		_printf(strbuf,":%7f",prb_param2[curr_prb-1]);
		draw_label(&paramlbl2,SW_NORMAL);
		draw_label(&paramval2,SW_NORMAL);
		sprintf(strbuf,":%7f",prb_param3[curr_prb-1]);
		draw_label(&paramlbl3,SW_NORMAL);
		draw_label(&paramval3,SW_NORMAL);
		return;
	}
	if(msg == KEY_NUM1) {
		window_setup(7);
		sprintf(strbuf,"请输入探头序列号");
		if(1 == wnd_sninput(&(prb_name[(curr_prb-1)<<3])))
			send_double(SRUN_PRBNM_BASE+curr_prb,&prb_name[(curr_prb-1)<<3],8);
		return;
	}
	if(msg == KEY_NUM2){
		irun[IRUN_NEW_PG] = PG_PRBTYPELIST;
		return;
	}
	if(msg == KEY_NUM3 ){
		window_setup(8);
		sprintf(strbuf,"输入系数a");
		valuep = prb_param1[curr_prb-1];
		if(wnd_floatinput(valuep) == 1)
			send_double(SRUN_PRBP1_BASE + curr_prb,(uchar*)&valuep,4);
		return;
	}
	if(msg == KEY_NUM4) {
		window_setup(8);
		sprintf(strbuf,"输入系数b");
		valuep = prb_param2[curr_prb-1];
		if(wnd_floatinput(valuep))
			send_double(SRUN_PRBP2_BASE + curr_prb,(uchar*)&valuep,4);
		return;
	}
	if(msg == KEY_NUM5) {
		window_setup(8);
		sprintf(strbuf,"输入系数c");
		valuep = prb_param3[curr_prb-1];
		if(wnd_floatinput(valuep))
			send_double(SRUN_PRBP3_BASE + curr_prb,(uchar*)&valuep,4);
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
		send_byte(SRUN_SET_MODE,BORE_MODE);
		return;
	}
	if(msg == KEY_NUM2) {
		irun[IRUN_NEW_PG] = PG_TCONFIG;
		return;
	}
	if(msg == KEY_NUM3) {
		send_byte(SRUN_SET_MODE,THERM_MODE);
		return;
	}
	if(msg == KEY_NUM4) {
		irun[IRUN_NEW_PG] = PG_TCONFIG;
		return;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS;
	}
}


