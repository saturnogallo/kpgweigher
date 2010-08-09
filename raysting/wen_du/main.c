/*
	TODO: 4wire 2wire option
		  change english to chinese.
		  furnish draw label function
*/

#include "stc51.h"
#include "utili.h"
#include "lcd.h"
#include "key.h"
#include "stdio.h"
#include "eeprom.h"
#include "window.h"
#include "scanner.h"
#include "math.h"
//#include "wendu.h"
RUNDATA rdata;
SYSDATA sdata;
PRBDATA	prbdata;
char strbuf[20];
uchar pos_strbuf = 0;
uchar key;

extern uchar pgmain_handler(uchar);	//running menu
extern uchar pgrconfig_handler(uchar);	//main menu of R config
extern uchar pgtconfig_handler(uchar);	//main menu of T config
extern uchar pgchset_handler(uchar);	//menu of channel probe setup
extern uchar pgprblist_handler(uchar);	//list of probe selection
extern uchar pgprbtypelist_handler(uchar); //select probe type
extern uchar pgprbset_handler(uchar);	//menu of probe setup
extern uchar pgrprbconfig_handler(uchar);  //config of r probe
extern uchar pgtprbconfig_handler(uchar);	//config of t probe

#define SPIDLE	0
#define SPBUSY	1
uchar 	spSFlag=SPIDLE;
void DBG(uchar a)
{ 
	spSFlag = SPBUSY;
	SBUF=a;
	while(spSFlag == SPBUSY)
		;
}

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
extern uchar xdata ch1buf[8];
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
		if(rcv_pos < 8)
		{
			ch1buf[rcv_pos] = SBUF;
			rcv_pos++;
		}
		RI = 0;
	}
}


#define ONESEC	1282

sbit KTT=P3^7;
#define KTT_POSITIVE	KTT = 1
#define KTT_NEGATIVE	KTT = 0;
uchar ktt_pos = 0; //0 means positive,1 means negative
int 	curr_ch = 1;	//index of current channel
int   curr_prb = 1;	//index of current probe selection


LABEL code bootup = {LBL_HZ16,30,30,1,"正在启动..."};
LABEL code calibrate = {LBL_HZ16,30,30,1,"正在校准..."};
LABEL code lbldbg = {LBL_HZ16,30,50,8,strbuf};
uchar nextwin = 0;
void calculate_temp(ch)
{
	rdata.temperature[ch-1] = 20.1;
	return;
	//todo get temperature;
/*	if(prbdata.type[ch-1] && PRBTYPE_BIT)
		rdata.temperature[ch-1] = GetThmoVolt(rdata.reading[ch-1],prbdata.type[ch-1]);
	else
		rdata.temperature[ch-1] = MValueToTValue(rdata.reading[ch-1],prbdata.type[ch-1]);
*/
}
double get_reading(uchar ch)
{
	double ret;

	rdata.reading[ch-1] = 1.23;
	return 0;
	//todo get reading
	//sm_write();
	//sm_read();
	rdata.reading[ch-1] = ret;
	return 0;
}
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
void main()
{
	 int ch_to_search = 1;
	 int delay_cnt_p = 0;
	 int delay_cnt_n = 0;
	 double valuep;
	 nextwin = 0;
	 KTT_POSITIVE;

	 LCD_Init();

	 wnd_msgbox(&bootup);
	 
	 Key_Init();
	 Init_18b20();

	 IE = 0;//close int
	 /* Initiate timer */
	 initiate_timer();

	 sm_Init();
	 IE = 0x92;//enable serial int and timer0 interrupt
//	 State_Init();	
	 
	 //todo collect first batch data (based on scanmode)
	 rdata.has_scanner = 0; //debug use
	 sdata.ktime = 30;
	 rdata.stdV = 1.0;
	 sdata.Rs1 = 1;
	 sdata.V0 = 0;
	 sdata.mode = KTT_BIT;
	sdata.scanmode = 12;
	 for(curr_ch = 0; curr_ch <12;curr_ch++)
	 {
		 sjprintf(prbdata.name[curr_ch],"sr%i",curr_ch);
		 rdata.reading[curr_ch] = curr_ch+1.234;
 		 rdata.temperature[curr_ch] = curr_ch+20.5678;
	 }	 
	 if(rdata.has_scanner){
		scanner_set_channel(ch_to_search);
		delay_cnt_p = ONESEC * sdata.ktime;
	 }
	 nextwin = PG_MAIN;
	 
	 key = KEY_INVALID;
	 curr_ch = 1;
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
				if(key == KEY_BTN1) //mode switch
				{
					TOGGLE_MODE_BORE;
				}
				if(key == KEY_BTN2) //auto ktt or not
				{
					TOGGLE_MODE_KTT;
					if(!(IS_MODE_KTT)){
						KTT_POSITIVE;
					}
				}
				if(key == KEY_BTN3) //thermal probe type
				{
					//todo
				}
				if(key == KEY_BTN4) //remove zero
				{
					//todo
				}
			}else{
				(*curr_window)(key);
			}
			key = KEY_INVALID;
	  	}else{
			//todo get readings on current channel
			if(curr_window != pgmain_handler)
				continue;
			if(delay_cnt_p > 1){
				delay_cnt_p--;
				continue;
			}
			if(delay_cnt_p == 1)
			{
				valuep = get_reading(ch_to_search);
				if(IS_MODE_KTT)
				{
					delay_cnt_n = ONESEC * sdata.ktime;
					KTT_NEGATIVE;					
				}else{
					delay_cnt_n = 0;
				}
				if(IS_THERM_MODE)
					delay_cnt_n = 0;	//no ktt for therm mode
				delay_cnt_p = 0;
			}
			if(delay_cnt_n > 0){
				delay_cnt_n--;
				continue;
			}
/*
			if(IS_BORE_MODE)
			{

				if(IS_MODE_KTT)
				{
					valuep = abs(valuep) + abs(get_reading(ch_to_search));
					valuep = valuep / 2;
				}
				rdata.reading[ch_to_search-1] = valuep*sdata.Rs1/rdata.stdV;
			}
			if(IS_THERM_MODE)
			{
				rdata.reading[ch_to_search-1] = valuep - sdata.V0;	
			}
			calculate_temp(ch_to_search);
*/
			ch_to_search++;
			if(ch_to_search > sdata.scanmode)
				ch_to_search = 1;
			scanner_set_channel(ch_to_search);
			pgmain_handler(MSG_REFRESH);
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
	{PG_RPRBCONFIG,	pgrprbconfig_handler},  //config of r probe
	{PG_TPRBCONFIG,	pgtprbconfig_handler},	//config of t probe
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
