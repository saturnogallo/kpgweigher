#include <mega128.h>
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "eeprom.h"
#include "window.h"
#include "utili.h"
#include "scanner.h"

extern uchar nextwin;  //next window ID
extern MSG_HANDLER      lp_listvalue;  //prepare the string value of Ith item
extern MSG_HANDLER      lp_listclear;  //handler for item remove action
LABEL flash main_mark = {LBL_HZ12, 1, 20, 1, ">"};
LABEL flash main_options[] = {
	{LBL_HZ16, 10,3,  8, strbuf	},
	{LBL_HZ12, 10,20, 8, strbuf	},
	{LBL_HZ12, 10,36, 8, strbuf	},
	{LBL_HZ12, 10,48, 8, strbuf	}
};
void display_buttons(uchar pos,uchar state) //update the LED status
{
}
extern uchar ch_to_search;  //current channel selected start from 1
//display the result of channel
void result_of_channel(uchar ch)
{
	char i,j;
	while(ch > MAX_CH_NUM)  //for rollback display id25 = id1
		ch = ch - MAX_CH_NUM;
	sprintf(strbuf,"        "); //8 space
	if(ch == 0)
		return;

	ch = ch - 1; //0 based
	        
	sprintf(strbuf,"CH%2i:      ",ch+1);
	if((sysdata.id[ch] == INVALID_PROBE) || 
           (prbdata.type[sysdata.id[ch]] == PRBTYPE_INVALID)) //no display for invalid channel
	{
		return;
	}
	j = sysdata.id[ch];
	if(prbdata.type[j] == PRBTYPE_PT100 || prbdata.type[j] == PRBTYPE_PT25)
	{
		if(sysdata.dispmode & DISP_RATIO_MODE)
		{
			sprintf(strbuf,"CH%2i:%7f ",ch+1,GETRS()/rundata.reading[ch]);
		}
		if(sysdata.dispmode & DISP_TEMP_MODE)
		{
			if(sysdata.dispmode & DISP_FMODE)
				sprintf(strbuf,"CH%2i:%7f F",ch+1,CeliusToF(rundata.temperature[ch]));
			else
				sprintf(strbuf,"CH%2i:%7f C",ch+1,rundata.temperature[ch]);
		}
		if(sysdata.dispmode & DISP_RES_MODE)
		{
			sprintf(strbuf,"CH%2i:%7f ohm",ch+1,rundata.reading[ch]);
		}
	}
}                                 
//update LED display
void update_shortcuts()
{
		if(sysdata.dispmode & DISP_FMODE)
			display_buttons(KEY_BTN1, 1);
		else
			display_buttons(KEY_BTN1, 0);

		if(sysdata.kttmode & 0x01)
			display_buttons(KEY_BTN2, 1);
		else
			display_buttons(KEY_BTN2, 0);

		if(rundata.runstop & 0x01)
			display_buttons(KEY_BTN4, 1);
		else
			display_buttons(KEY_BTN4, 0);

}
void pg_main_handler(uchar msg)
{               
	static uchar curr_sel = 1;
	uchar i;
	if(msg == KEY_BTN1) //F C mode switch
	{
		sysdata.dispmode = sysdata.dispmode ^ DISP_FMODE; //switch the wen biao
		update_shortcuts();
		msg = MSG_INIT;
	}
	if(msg == KEY_BTN2) //KTT mode switch
	{
		sysdata.kttmode = 1 ^ sysdata.kttmode ; 
		update_shortcuts();
                return; //no need to update display
	}
	if(msg == KEY_BTN3) //??
	{
		update_shortcuts();
		msg = MSG_INIT;
	}
	if(msg == KEY_BTN4) //run stop
	{
		rundata.runstop = 1 ^ rundata.runstop;
		update_shortcuts();
		return; //no need to update display
	}

        if(msg == MSG_INIT)
	{
		LCD_Cls();
		result_of_channel(ch_to_search);
		draw_label(&main_options[0],SW_NORMAL);
		draw_label(&main_mark,SW_NORMAL);
		for(i = 0; i<3 ; i++)
		{
			result_of_channel(curr_sel+i);
			draw_label(&main_options[i+1],SW_NORMAL);
		}
		msg = MSG_REFRESH;
	}
	if(msg == KEY_TAB)
	{
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_DN) //page down
	{
		curr_sel = curr_sel + 3;
		if(curr_sel > MAX_CH_NUM)
			curr_sel = curr_sel - MAX_CH_NUM;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) //page up
	{
		if(curr_sel > 3)
			curr_sel = curr_sel - 3;
		else
			curr_sel = curr_sel + MAX_CH_NUM - 3;
		msg = MSG_REFRESH;
	}

	if(msg == KEY_OK) //move down
	{
		curr_sel++;
		if(curr_sel > MAX_CH_NUM)
			curr_sel = 1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_CE) //move up
	{
		if(curr_sel > 1)
			curr_sel--;
		else
			curr_sel = MAX_CH_NUM;
	}
	if(msg == MSG_REFRESH)
	{
		result_of_channel(ch_to_search);
		draw_label(&main_options[0],SW_NORMAL);
		draw_label(&main_mark,SW_NORMAL);		
		for(i = 0; i<3 ; i++)
		{
			if(((curr_sel+i) == ch_to_search) || ((curr_sel+i) == (ch_to_search+MAX_CH_NUM))) //no need to update old values here
			{
				result_of_channel(curr_sel+i);
				draw_label(&main_options[i+1],SW_NORMAL);
			}
		}
	}
}
LABEL flash boot_banner = {LBL_HZ12,3,3,8,"  "};	
LABEL flash boot_options[] = {
 	{LBL_HZ16, 10, 23,7,"1.开始测量"},
 	{LBL_HZ16, 130,23,7,"2.通道配置"},
 	{LBL_HZ16, 10, 45,7,"3.探头参数"},
 	{LBL_HZ16, 130,45,7,"4.系统设置"}
};       
void pg_boot_handler(uchar i)
{
	uchar msg;
	while(1)
	{
		msg = wnd_menu(&boot_banner,boot_options,4,4);
		switch(msg)
		{
			case 1:		nextwin = PG_MAIN;	return;
			case 2:		nextwin = PG_CH_SEL;	return;
			case 3:		nextwin = PG_PRB_SEL;	return;
			case 4:		nextwin = PG_SYSTEM;	return;
		}
	}
}
void buf2name(uchar i, uchar *buf) //copy name from buffer to eeprom
{
	u8 j=0;
    for(j=0;j<8;j++)
    {
        prbdata.name[i][j] = buf[j];
        if(buf[j] == '\0')
            break;

        if((buf[j] >= '0' && buf[j] <= '9') || (buf[j] >= 'A' && buf[j] <= 'Z'))
            continue;
        prbdata.name[i][j] = '\0'; //avoid no alphabit values
        break;
    }
}
void name2buf(uchar i, uchar *buf) //copy name from eeprom to buffer
{
    u8 j=0;
    for(j=0;j<8;j++)
    {
        buf[j] = prbdata.name[i][j];
        if(buf[j] == '\0')
            break;

        if((buf[j] >= '0' && buf[j] <= '9') || (buf[j] >= 'A' && buf[j] <= 'Z'))
            continue;
        buf[0] = '\0'; //avoid no alphabit values
        break;
    }
}
void clear_probe(uchar i) //clear probe name based on index start from 1
{
	if((i <= MAX_PRB_NUM) && (i >= 1))
	{
		prbdata.type[i-1] = PRBTYPE_INVALID;
	}
}
void clear_channel(uchar ch) //clear probe name on channel start from 1
{
	if((ch <= MAX_CH_NUM) && (ch >= 1))
	{
		sysdata.id[ch-1] = INVALID_PROBE;
	}
}

void prb_name_on_probe(uchar i) //probe name based on index start from 1
{
	sprintf(strbuf,"*");
	if((i <= MAX_PRB_NUM) && (i >= 1))
	{
		if(prbdata.type[i-1] != PRBTYPE_INVALID)
		{
			name2buf(i-1,strbuf);
		}
	}
}
void prb_name_on_channel(uchar ch) //probe name installed on channel start from 1
{
	sprintf(strbuf,"*");
	if((ch <= MAX_CH_NUM) && (ch >= 1))
	{
		if(sysdata.id[ch-1] != INVALID_PROBE)
		{
			prb_name_on_probe(sysdata.id[ch-1]);
		}
	}
}
LABEL flash chsel_banner = {LBL_HZ12,3,3,8,"选择要配置的通道"};	
LABEL flash chprb_banner = {LBL_HZ12,3,3,8,strbuf};	
void pg_ch_sel_handler(uchar kmsg)
{
	uchar msg = 1;
	uchar pmsg = 1;
	uchar ch;
	while(1)
	{
		lp_listvalue = prb_name_on_channel;
		lp_listclear = clear_channel;
		msg = wnd_listbox(&chsel_banner,MAX_CH_NUM,msg);
		if(msg >= 1 && msg <= MAX_CH_NUM)
		{
			ch = msg;
			sprintf(strbuf,"选择通道%i使用的探头",ch);
			lp_listvalue = prb_name_on_probe;
			lp_listclear = 0;
			pmsg = wnd_listbox(&chprb_banner,MAX_PRB_NUM,pmsg);
			if(pmsg >= 1 && pmsg <= MAX_PRB_NUM)
			{
				sysdata.id[ch-1] = pmsg-1; //set probe id to channel
			}
		}else{
			nextwin = PG_BOOT;
			return;
		}
	}
}                  

static u8 curr_prb_sel = 1; //current probe param index, 1 based
LABEL flash prbparam_banner = {LBL_HZ12,3,3,8,strbuf};	
LABEL flash prbparam_options[] = {
	{ LBVAR_SN   + LBL_HZ12,10,17,6,strbuf}, //"1.序列号"
	{ LBVAR_TYPE + LBL_HZ12,130,17,6,strbuf},//"2.类型"
	{ LBVAR_CURR + LBL_HZ12,10,45,6,strbuf}, //"3.电流"
	{ LBVAR_SQR  + LBL_HZ12,130,45,6,strbuf},//"4.倍功率"
	{ LBVAR_RTP  + LBL_HZ12,10,17,4,strbuf}, //"5.Rtp"  
	{ LBVAR_COEFA+ LBL_HZ12,130,17,6,strbuf},//"6.系数A"
	{ LBVAR_COEFB+ LBL_HZ12,130,45,6,strbuf},//"7.系数B"
	{ LBVAR_COEFC+ LBL_HZ12,130,45,6,strbuf} //"8.系数C"
};	
LABEL flash prbcurr_banner = {LBL_HZ16,3,3,8,"请选择电流"};
LABEL flash prbcurr_options[] = {
	{LBL_HZ6X8,10,20,8,"1:0.01mA"},
	{LBL_HZ6X8,80,20,8,"2:0.02mA"},
	{LBL_HZ6X8,10,30,8,"3:0.05mA"},
	{LBL_HZ6X8,80,30,8,"4.0.1mA"},
	{LBL_HZ6X8,150,30,8,"5:1mA"},
	{LBL_HZ6X8,10,40,8,"6:3mA"},
	{LBL_HZ6X8,80,40,8,"7:10mA"}
};                            

void pg_prb_param_handler(uchar kmsg)
{

	u8 msg = 1;
	u8 *buf;
	double f;
	while(1)
	{	
		sprintf(strbuf,"探头%i参数",curr_prb_sel);
		msg = wnd_menu(&prbparam_banner,prbparam_options,8,4);
		if(msg == 1) {
			window_setup(8);
			sprintf(strbuf,"请输入序列号:");
			buf = wnd_strinput(); 
			buf2name(curr_prb_sel-1,buf); //copy buf to name
			continue;
		}
		if(msg == 2) {
			PRBTYPE_INC(curr_prb_sel);// switch the probe type
			continue;
		}
		if(msg == 3) {
			msg = wnd_menu(&prbcurr_banner,prbcurr_options,7,7);
			if((msg >= 1) && (msg <= (PRBCURR_10+1)))//save the current
				prbdata.current[curr_prb_sel-1] = (prbdata.current[curr_prb_sel-1] & PRBCURR_SQR) + (msg - 1);
			msg = 3;
			continue;
		}
		if(msg == 4) {
			prbdata.current[curr_prb_sel-1] = (prbdata.current[curr_prb_sel-1] ^ PRBCURR_SQR) ; //switch the sqr
			continue;
		}
		
		if(msg == 5) {
			f = prbdata.rtp[curr_prb_sel-1]; //load the current rtp
			sprintf(strbuf,"请输入探头RTP:");
			f = wnd_floatinput(f);
			prbdata.rtp[curr_prb_sel-1] = f;// save the new rtp
			continue;
		}
		if(msg == 6) {
			f = prbdata.paramA[curr_prb_sel-1];//load the current A
			sprintf(strbuf,"请输入探头系数A:");
			f = wnd_floatinput(f);
			prbdata.paramA[curr_prb_sel-1] = f;//save the new A
			continue;
		}
		if(msg == 7) {
			f = prbdata.paramB[curr_prb_sel-1];//load the current B
			sprintf(strbuf,"请输入探头系数B:");
			f = wnd_floatinput(f);
			prbdata.paramB[curr_prb_sel-1] = f;//save the new B
			continue;
		}
		if(msg == 8) {
			f = prbdata.paramC[curr_prb_sel-1];//load the current C
			sprintf(strbuf,"请输入探头系数C:");
			f = wnd_floatinput(f);
			prbdata.paramC[curr_prb_sel-1] = f;//save the new C
			return;
		}
		nextwin = PG_PRB_SEL;
		return;
	}
}
LABEL flash prbsel_banner = {LBL_HZ12,3,3,8,"选择要配置的探头"};	
void pg_prb_sel_handler(uchar kmsg)
{
	uchar msg=1;
	lp_listvalue = prb_name_on_probe;
	lp_listclear = clear_probe;
	msg = wnd_listbox(&prbsel_banner,MAX_CH_NUM,curr_prb_sel);
	if(msg >= 1 && msg <= MAX_PRB_NUM)
	{
		curr_prb_sel = msg;
		nextwin = PG_PRB_PARAM;
		return;
	}else{
		nextwin = PG_BOOT;
		return;
	}
}
LABEL flash system_banner = {LBL_HZ12,3,3,8,"系统设置"};	
LABEL flash system_options[] = {
        { LBL_HZ12,10,17,4,"1.内标准阻值"},
	{ LBVAR_KTIME  + LBL_HZ12,130,17,4,strbuf},//"2.换向时间"
	{ LBVAR_SYSTEMP+ LBL_HZ12,10,45,4,strbuf},//"3.环境温度"
	{ LBVAR_FCMODE + LBL_HZ12,130,45,4,strbuf},//"4.显示温标"
	{ LBVAR_SCANNER+ LBL_HZ12,10,31,4,strbuf}//"5.扫描器类型"
};	

void pg_system_handler(uchar kmsg)
{
	u8 msg = 1;
	float f;
	while(1)
	{	
		msg = wnd_menu(&system_banner,system_options,5,5);
		if(msg == 1) {
			window_setup(8);
			sprintf(strbuf,"请输入内标准阻值:");
			sysdata.Rs = wnd_floatinput(sysdata.Rs);
			continue;
		}
		if(msg == 2) {
			sprintf(strbuf,"输入电流换向时间:");
			sysdata.ktime = wnd_uintinput(sysdata.ktime);
			continue;
		}
		if(msg == 3) {                        
		        if(sysdata.dispmode ^ DISP_FMODE)
		        {
			        sprintf(strbuf,"输入环境温度: F");
        			sysdata.RoomT = FToCelius(wnd_floatinput(CeliusToF(sysdata.RoomT)));
        		}else{
			        sprintf(strbuf,"输入环境温度: C");
        			sysdata.RoomT = wnd_floatinput(sysdata.RoomT);
        		}
			continue;
		}
		if(msg == 4) {
			sysdata.dispmode = sysdata.dispmode ^ DISP_FMODE; //switch the wen biao
			continue;
		}
		if(msg == 5) {
			sprintf(strbuf,"选择扫描器:1.MI,2.GUIDL,3.RAYSTING");
			msg = wnd_uintinput(sysdata.scanner);
			if((msg < 1) || (msg >3) )
				msg = 1;
			continue;
		}
		nextwin = PG_BOOT;
		return;
	}
	
}

void display_var(uchar type)
{
	switch(type)
	{
		case LBVAR_SN	 	:
			name2buf(curr_prb_sel-1,databuf);
			sprintf(strbuf,"1.序列号%s",databuf);
			return;
		case LBVAR_TYPE	 	:
			sprintf(strbuf,"2.类型");
			if(prbdata.type[curr_prb_sel-1] == PRBTYPE_PT25)
				sprintf(strbuf,"2.类型PT25");
			if(prbdata.type[curr_prb_sel-1] == PRBTYPE_PT100)
				sprintf(strbuf,"2.类型PT100");
			return;
		case LBVAR_CURR	 	:
			sprintf(strbuf,"3.电流");
			if((prbdata.type[curr_prb_sel-1] & 0x7f) == PRBCURR_PT01)
				sprintf(strbuf,"3.电流0.01mA");
			if((prbdata.type[curr_prb_sel-1] & 0x7f) == PRBCURR_PT02)
				sprintf(strbuf,"3.电流0.02mA");
			if((prbdata.type[curr_prb_sel-1] & 0x7f) == PRBCURR_PT05)
				sprintf(strbuf,"3.电流0.05mA");
			if((prbdata.type[curr_prb_sel-1] & 0x7f) == PRBCURR_PT1)
				sprintf(strbuf,"3.电流0.1mA");
			if((prbdata.type[curr_prb_sel-1] & 0x7f) == PRBCURR_1)
				sprintf(strbuf,"3.电流1mA");
			if((prbdata.type[curr_prb_sel-1] & 0x7f) == PRBCURR_3)
				sprintf(strbuf,"3.电流3mA");
			if((prbdata.type[curr_prb_sel-1] & 0x7f) == PRBCURR_10)
				sprintf(strbuf,"3.电流10mA");
			return;
		case LBVAR_SQR		: 
			if(prbdata.type[curr_prb_sel-1] & PRBCURR_SQR)
				sprintf(strbuf,"4.倍功率:开");
			else
				sprintf(strbuf,"4.倍功率:关");
			return;
		case LBVAR_RTP		: 
			sprintf(strbuf,"5.Rtp:%7f",prbdata.rtp[curr_prb_sel-1]);
			return;
		case LBVAR_COEFA	:	 
			sprintf(strbuf,"6.系数A:%7f",prbdata.paramA[curr_prb_sel-1]);
			return;
		case LBVAR_COEFB	:	 
			sprintf(strbuf,"7.系数B:%7f",prbdata.paramB[curr_prb_sel-1]);
			return;
		case LBVAR_COEFC	:	 
			sprintf(strbuf,"8.系数C:%7f",prbdata.paramC[curr_prb_sel-1]);
			return;
		case LBVAR_KTIME	:	 
			sprintf(strbuf,"2.换向时间:%i",sysdata.ktime);
			return;
		case LBVAR_SYSTEMP	:                
			if(sysdata.dispmode & DISP_FMODE)
                		sprintf(strbuf,"3.环境温度:%.1f F",CeliusToF(sysdata.RoomT));
			else
                		sprintf(strbuf,"3.环境温度:%.1f C",sysdata.RoomT);
			return;
		case LBVAR_FCMODE	:
			if(sysdata.dispmode & DISP_FMODE)
				sprintf(strbuf,"4.显示温标:F");
			else
				sprintf(strbuf,"4.显示温标:C");
			return;
		case LBVAR_SCANNER	:
			if(sysdata.scanner == 1)
				sprintf(strbuf,"5.扫描器:MI");
			if(sysdata.scanner == 2)
				sprintf(strbuf,"5.扫描器:GUIDLINE");
			if(sysdata.scanner == 3)
				sprintf(strbuf,"5.扫描器:RAYSTING");
				
	}
}
void State_Init() {
	lp_ownerdraw = display_var;
	display_buttons(KEY_BTN1,0); //display type
	display_buttons(KEY_BTN2,sysdata.kttmode); //ktt
	display_buttons(KEY_BTN3,0); //zero
	display_buttons(KEY_BTN4,rundata.runstop); //run stop status
}

