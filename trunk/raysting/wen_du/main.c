/*
	TODO: 4wire 2wire option
		  change english to chinese.
		  furnish draw label function
*/

#include "stc51.h"
#include "utili.h"
#include "lcd.h"
#include "key.h"
#include "eeprom.h"
#include "window.h"
#include "scanner.h"
#include "math.h"
#include "wendu.h"
RUNDATA rdata;
SYSDATA sdata;
PRBDATA	prbdata;
char data strbuf[20];
uchar pos_strbuf = 0;
uchar key;

extern void pgmain_handler(uchar);	//running menu
extern void pgrconfig_handler(uchar);	//main menu of R config
extern void pgtconfig_handler(uchar);	//main menu of T config
extern void pgchset_handler(uchar);	//menu of channel probe setup
extern void pgprblist_handler(uchar);	//list of probe selection
extern void pgprbtypelist_handler(uchar); //select probe type
extern void pgprbset_handler(uchar);	//menu of probe setup
extern void pgprbconfig_handler(uchar);  //config of probe

extern void pgboottype_handler(uchar msg) ; //boot up config
#define SPIDLE	0
#define SPBUSY	1
uchar 	spSFlag=SPIDLE;
/*
void DBG(uchar a)
{ 
	spSFlag = SPBUSY;
	SBUF=a;
	while(spSFlag == SPBUSY)
		;
}
*/
void analog_timer()	interrupt 1
{
	Key_Scan();
}

void initiate_timer(void)
{
   //set serial port parameter (clock 11.0592M)
   //9600 baut rate 8 data non parity and 1 stop.
   SCON = 0x70;
   PCON = 0x00;

   //use timer 1 to be serial
   //timer count
   TH1 = 0xfd;

   //use timer 0 to be heart beat
   TH0 = -4608/256;   // 4608 , 4608/(11.0592M/12) = 0.005s = 5ms
   TL0 = -4608%256;

   TMOD = 0x21;
   TR1 = 1;

   ET0=1;
   EA=1;
   TR0 = 1;
}

uchar rcv_pos;
extern uchar data ch1buf[8];
void SerialHandler(void) interrupt 4 using 2
{
	//just handle serial interrupt 1
	if(TI)
	{
		TI = 0;
		spSFlag = SPIDLE;
	}
	if(RI)
	{
	/*
		if(rcv_pos < 8)
		{
			ch1buf[rcv_pos] = SBUF;
			rcv_pos++;
		}
		*/
		RI = 0;
	}
}
const LABEL code error = {LBL_HZ16,30,30,8,"出错,请重新启动..."};
void dead()
{
//	LCD_Cls();
//	LCD_PrintHz16( error.x, error.y, error.param);
	while(1)
		;
}

sbit KTT=P3^7;
#define	SET_NKTT
#define SET_PKTT
int   curr_ch = 1;	//index of current channel
int   curr_prb = 1;	//index of current probe selection


LABEL code bootup = {LBL_HZ16,30,30,7,"正在启动..."};
LABEL code modify = {LBL_HZ16,30,30,8,"正在更新数据..."};

LABEL code lbldbg = {LBL_HZ16,30,30,8,strbuf};
uchar nextwin = 0;


double get_double(uchar cmd)
{
	sm_write(cmd);
	ch1buf[0] = sm_read();
	ch1buf[1] = sm_read();
	ch1buf[2] = sm_read();
	ch1buf[3] = sm_read();
	ch1buf[4] = sm_read();
	if((uchar)(ch1buf[3]+ch1buf[2]+ch1buf[1]+ch1buf[0]) == ~(ch1buf[4]))
		return 1+(*((double*)ch1buf))*10000.0;
	return -9999.9;
}
/*
void debugwnd()
{
	 sjprintf(strbuf,"key:?",key);
	 wnd_msgbox(&lbldbg);

	 while(1)
	 {
		if(key != KEY_INVALID)
		{
			sjprintf(strbuf,"key:%c",key);
			wnd_msgbox(&lbldbg);

		}
	 }
}
*/
int ch_to_search = 1;
int ch_mod_page = 1;

int dlg_cnt = 0;
uchar phase = 0;
uchar bore_phase = 0;
int dlg_rscheck_cnt = 0;
double valuep;
//state machine of therm type
//phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
//phase 1:get the reading and update the display 
//phase 2: move to next channel
void therm_state()
{
	if(phase == 0)
	{
		if((prbdata.type[ch_to_search - 1] <= PRBTYPE_R) && (prbdata.type[ch_to_search - 1] >= PRBTYPE_K))
		{
				if(rdata.scanmode > 1){
					sm_action(ch_to_search);	
					dlg_cnt = ONESEC;
				}
				phase = 1;
				return;
		}else{
			rdata.temperature[ch_to_search - 1] = -9999;
			phase = 2;
		}
	}
	if(phase == 1)
	{
//		sm_action(0x90 + prbdata.type[ch_to_search-1]); //set probe type
//		sm_action(CMD_DMM_TUPDATE);
		sm_action(CMD_DMM_VUPDATE);
		rdata.reading[ch_to_search - 1] = get_double(CMDR_DMM_VOLT);
		rdata.temperature[ch_to_search - 1] = MValueToTValue(get_double(CMDR_DMM_TEMP));
		phase = 2;
	}
	if(phase == 2)
	{
		pgmain_handler(MSG_REFRESH);
		ch_to_search++;
		phase = 0;
	}
}
//state machine of bore measure
//phase0 the search the current channel and switch to it if apply, switch to rs, pktt, add delay, if not ktt to phase 2 else to phase 1, otherwise to phase n
//phase1 get reading of rs+, set to nktt, add delay 
//phase2 get reading of rs-, switch to rx, add delay, if  ktt to phase 3 else to phase 4
//phase3 get reading of rx-, set to pktt, add delay
//phase4 get reading of rx+, switch to rx, caculate
//phase5 update display and move to next channel
void bore_state()
{
	if(phase == 0)
	{
		if((prbdata.type[ch_to_search - 1] <= PRBTYPE_PT25) && (prbdata.type[ch_to_search - 1] >= PRBTYPE_PT100))
		{
			if(rdata.scanmode > 1)
				sm_action(ch_to_search);	
			SET_PKTT;
			sm_action(CMD_DMM_RX);
			dlg_cnt =  ONESEC * sdata.ktime;
			if(IS_MODE_KTT)
				phase = 1;
			else
				phase = 2;
			return;
		}else{
			rdata.temperature[ch_to_search - 1] = -9999;
			phase = 5;
		}

	}
	if(phase == 1)
	{
		sm_action(CMD_DMM_VUPDATE);
		rdata.stdV = get_double(CMDR_DMM_VOLT);
		SET_NKTT;
		dlg_cnt =  ONESEC * sdata.ktime;
		phase = 2;
		return;
	}
	if(phase == 2)
	{
		sm_action(CMD_DMM_VUPDATE);

		if(IS_MODE_KTT)
		{
			rdata.stdV = (rdata.stdV + get_double(CMDR_DMM_VOLT));
			phase = 3;
		}else{
			rdata.stdV = get_double(CMDR_DMM_VOLT);
			phase = 4;
		}
		sm_action(CMD_DMM_RX);
		dlg_cnt = ONESEC * sdata.ktime;
		return;	
	}
	if(phase == 3)
	{
		sm_action(CMD_DMM_VUPDATE);
		valuep = get_double(CMDR_DMM_VOLT);
		SET_PKTT;
		dlg_cnt = ONESEC * sdata.ktime;
		phase = 4;
		return;	

	}
	if(phase == 4)
	{
		sm_action(CMD_DMM_VUPDATE);
		if(IS_MODE_KTT){
			valuep = (valuep + get_double(CMDR_DMM_VOLT));
		}else{
			valuep = get_double(CMDR_DMM_VOLT);
		}
		if(rdata.stdV != 0)
		{
			rdata.reading[ch_to_search-1] = valuep*sdata.Rs1/rdata.stdV;
			rdata.temperature[ch_to_search-1] = RValueToTValue(rdata.reading[ch_to_search-1],ch_to_search-1);
		}else{
				rdata.temperature[ch_to_search-1] = -9999;
		}
		phase = 5;
	}
	if(phase == 5)
	{
		pgmain_handler(MSG_REFRESH);
		ch_to_search++;
		phase = 0;
	}
}
void main()
{
	 IE = 0;//close int
	 nextwin = 0;

	 LCD_Init();

	 wnd_msgbox(&bootup);
	 
	 Key_Init();
//	 Init_18b20();


	 /* Initiate timer */
	 initiate_timer();

	 sm_Init();
	 IE = 0x92;//enable serial int and timer0 interrupt
/*
	 while(1)
	 {
		sm_action(CMD_DMM_VUPDATE);
		//sm_wait_done(CMDR_DMM_STATE);
		valuep = get_double(CMDR_DMM_VOLT)*10000.0;

		 sm_write(CMD_SCAN_UPDATE);
		 sm_write(CMDR_SCAN_CHNUM);
		 rdata.scanmode = sm2_read();
 		 sm_write(CMDR_SCAN_CHCURR);
		 nextwin = sm2_read();
		sprintf(strbuf,"%.4f,%i,%i",valuep,(int)nextwin,(int)rdata.scanmode);
		wnd_msgbox(&lbldbg);		
	 }
*/
/*
	 while(1)
	 {
//		sprintf(strbuf,"step1");
//		wnd_msgbox(&lbldbg);
		sm_action(CMD_DMM_VUPDATE);
		//sm_wait_done(CMDR_DMM_STATE);
		valuep = get_double(CMDR_DMM_VOLT)*10000.0;
		sprintf(strbuf,"%.4f",valuep);
		//sprintf(strbuf,"%i,%i,%i,%i,%i",(int)ch1buf[3],(int)ch1buf[2],(int)ch1buf[1],(int)ch1buf[0],(int)ch1buf[4]);
		wnd_msgbox(&lbldbg);		
	 }
*/


	 State_Init();	
	
	 SET_BORE_MODE;
	 //dmm init
	 SET_PKTT;
	 sm_action(CMD_DMM_RS);
	 for(curr_ch = 0; curr_ch <MAX_CH_NUM;curr_ch++)
	 {
	 	if(sdata.id[curr_ch] >= MAX_CH_NUM)
			sdata.id[curr_ch] = INVALID_PROBE;
	 }
	 for(curr_ch = 0; curr_ch <MAX_CH_NUM;curr_ch++)
	 {
		 prbdata.name[curr_ch][7] = '\0';
		 for(ch_mod_page = 0; ch_mod_page < 7; ch_mod_page++)
		 {
		 	if(prbdata.name[curr_ch][ch_mod_page] == '\0')
				continue;
			 if( (prbdata.name[curr_ch][ch_mod_page] >='0') && (prbdata.name[curr_ch][ch_mod_page] <='9'))
			 	continue;
			 if( (prbdata.name[curr_ch][ch_mod_page] >='A') && (prbdata.name[curr_ch][ch_mod_page] <='Z'))
			 	continue;
	 		prbdata.name[curr_ch][ch_mod_page] = '*';
  		 }
		if((prbdata.type[curr_ch] >= PRBTYPE_K) && (prbdata.type[curr_ch] <= PRBTYPE_R))
			continue;
		if((prbdata.type[curr_ch] >= PRBTYPE_PT100) && (prbdata.type[curr_ch] <= PRBTYPE_PT25))
			continue;
		prbdata.type[curr_ch] = PRBTYPE_INVALID;
	 }	 
	 /*
	 sm_write(CMD_SCAN_UPDATE);
	 sm2_wait_done(CMDR_SCAN_STATE);
	 sm_write(CMDR_SCAN_CHNUM);
	 rdata.scanmode = sm2_read();
	 if(rdata.scanmode > 1)
	 	rdata.has_scanner = 1;
	 else
	 	rdata.has_scanner = 0;
	 */
	 rdata.scanmode = 1;
	 rdata.has_scanner = 0;

/*
	 if(rdata.has_scanner){
		scanner_set_channel(ch_to_search); //channel for measure
	 }
*/
	 nextwin = PG_BOOTTYPE;
	 
	 key = KEY_INVALID;
	 curr_ch = 1; //channel for display
	 while(1)
	 {
 	 	if(nextwin != 0)
		{
			SwitchWindow(nextwin);
			(*curr_window)(MSG_INIT);
			nextwin = 0;
		}
		if(key != KEY_INVALID)
		{
			if((key == KEY_BTN1)||(key == KEY_BTN2)||(key == KEY_BTN3)||(key == KEY_BTN4))
			{
				if(curr_window == pgmain_handler)
				{
					LCD_Cls();
					wnd_msgbox(&modify);
				}
				if(key == KEY_BTN1) //mode switch
				{
					if(IS_BORE_MODE){
						SET_THERM_MODE;
					    sm_action(CMD_DMM_RS);
						display_buttons(KEY_BTN1,1);
					}else{
						SET_BORE_MODE;
						sm_action(CMD_DMM_RX);
						display_buttons(KEY_BTN1,0);
					}
					SaveToEEPROM();
					dlg_rscheck_cnt = 1;
					phase = 0;
					dlg_cnt = 0;
					SET_PKTT;
				}
				if(key == KEY_BTN2) //auto ktt or not
				{

					if((IS_MODE_KTT)){
						CLR_MODE_KTT;
						display_buttons(KEY_BTN2,1);
					}else{
						SET_MODE_KTT;
						SET_PKTT;
						display_buttons(KEY_BTN2,0);
					}
					SaveToEEPROM();
					dlg_rscheck_cnt = 1;
				}
				if(key == KEY_BTN3) //thermal probe type
				{
					display_buttons(KEY_BTN3,1);
					if((prbdata.type[curr_ch-1] >= PRBTYPE_K) &&\
					   (prbdata.type[curr_ch-1] <= PRBTYPE_R))
					{
						if(prbdata.type[curr_ch-1] == PRBTYPE_R)
							prbdata.type[curr_ch-1] = PRBTYPE_K;
						else
							prbdata.type[curr_ch-1] +=1;
						SaveProbeData();
					}
					display_buttons(KEY_BTN3,0);
					
				}
				if(key == KEY_BTN4) //remove zero
				{
					display_buttons(KEY_BTN4,1);
					sm_write(CMD_DMM_ZERO);
					sm_wait_done(CMDR_DMM_STATE);
					display_buttons(KEY_BTN4,0);
				}
				if(curr_window == pgmain_handler)
				{
					LCD_Cls();
				}
			}else{
				(*curr_window)(key);
			}
			key = KEY_INVALID;
	  	}else{
			if(curr_window != pgmain_handler)
				continue;
			sprintf(strbuf,"ph:%i,dl:%i",(int)phase,(int)dlg_cnt);
			LCD_Print6X8(10, 20,strbuf);
			if(dlg_cnt > 1)
			{
				dlg_cnt--;
				continue;
			}
			if((IS_THERM_MODE))			
			{
				therm_state();
			}else{
				bore_state();
			}
			if(ch_to_search > rdata.scanmode)
				ch_to_search = 1;
			
		}
	}	
}



WINDOW code wins[]={
	{PG_MAIN,	pgmain_handler},	//running menu
	{PG_RCONFIG,	pgrconfig_handler},	//main menu of R config
	{PG_TCONFIG,	pgtconfig_handler},	//main menu of T config
	{PG_CHSET,	pgchset_handler},	//menu of channel probe setup
	{PG_PRBLIST,	pgprblist_handler},	//list of probe selection
	{PG_PRBTYPELIST,pgprbtypelist_handler}, //select probe type
	{PG_PRBSET,	pgprbset_handler},	//menu of probe setup
	{PG_PRBCONFIG,	pgprbconfig_handler},  //config of r probe
	{PG_BOOTTYPE,	pgboottype_handler}	//config of t probe
};


void SwitchWindow(uchar page)
{
	u8 i;
	for(i = 0; i<sizeof(wins)/sizeof(WINDOW);i++)
	{
		if(wins[i].page_id == page)
		{
			curr_window = wins[i].handler;
			return;
		}
	}
}
