/*****************************************************
This program was produced by the
CodeWizardAVR V1.24.6 Professional
Automatic Program Generator
?Copyright 1998-2005 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
e-mail:office@hpinfotech.com

Project :
Version :
Date    : 2007-1-15
Author  : F4CG
Company : F4CG
Comments:


Chip type           : ATmega128
Program type        : Application
Clock frequency     : 14.745600 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 1024
*****************************************************/

#include <mega128.h>
#include "uart.h"
#include "init.h"
#include "16c554.h"
#include "global.h"
#include "utili.h"
#include "window.h"
#include "lcd.h"
#include "key.h"
#include "wendu.h"
#include "scanner.h"
#include "uart.h"
#include "stdlib.h"
//#include "math.h"
extern void pgmain_handler(uchar);	//running menu
extern void pgrconfig_handler(uchar);	//main menu of R config
extern void pgtconfig_handler(uchar);	//main menu of T config
extern void pgchset_handler(uchar);	//menu of channel probe setup
extern void pgprblist_handler(uchar);	//list of probe selection
extern void pgprbtypelist_handler(uchar); //select probe type
extern void pgprbset_handler(uchar);	//menu of probe setup
extern void pgprbconfig_handler(uchar);  //config of probe
extern void pgboottype_handler(uchar msg) ; //boot up config
extern void com1_putstr(char *s);
extern unsigned int strlen(char *s);
RUNDATA rundata;
SYSDATA eeprom sysdata = {
	0,0,100,3,
	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
	1,1,1000};
PRBDATA	eeprom tprbdata = {
       {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	{25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0},
	{"11","","","","","","","",
	"","","","","","","","",
	"","","","","","","",""},
       {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
};
PRBDATA	eeprom rprbdata = {
       {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	{25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0},
	{"11","","","","","","","",
	"","","","","","","","",
	"","","","","","","",""},
       {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
};

char  strbuf[50];        //buffer for menu display
uchar key;



LABEL flash bootup = {LBL_HZ16,30,30,7,"正在启动..."};
LABEL flash modify = {LBL_HZ16,30,30,8,"正在更新数据..."};

LABEL flash lbldbg = {LBL_HZ6X8,10,30,8,strbuf};

uchar nextwin = 0;

int   curr_ch = 1;	//index of current channel in config menu window, 1 based
int   curr_prb = 1;	//index of current probe selection in menu window 1 based
int   curr_dispch = 1;  //index of start channel in main running window, 1 based
int ch_to_search = 0;   //current channel , 0 based

unsigned long dlg_cnt = 0;        //delay count time display
unsigned long onesec_cnt = 0;
uchar phase = 0;       //state machine index
double valuep;

/**************************************************************************************/
//                      Timer 1 overflow interrupt service routine
/**************************************************************************************/
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Place your flash here
}
void dbgout(double val)
{
//        sprintf(strbuf,"%f\r\n",val);
//        prints(strbuf,strlen(strbuf),PORT_PC);
}
void sdbgout(flash char *l)
{
        sprintf(strbuf,"%s",l);
prints(strbuf,strlen(strbuf),PORT_PC);
}
void InitLED()
{
    PORTB = 0xFF;
/*
    PORTB.7=1;
    PORTB.6=1;
    PORTB.5=1;
    PORTB.4=1;
    PORTD.7=1;
    PORTD.6=0;
*/
}
uchar nav1v = 1;
void navto120mv()
{
   nav1v = 0;
   nav_command(NAV_120MV);
   sleepms(200*ONEMS);
}

void navto1v()
{
    nav1v = 1;
    nav_command(NAV_1V);
    sleepms(200*ONEMS);
}
double mabs(double val)
{
        if(val < 0)
                return -val;
        else
                return val;
}
u8 ToByte(char l, char h)
{
	h = (h > 'A')?(h-'A'+0x0A):(h-'0');
	l = (l > 'A')?(l-'A'+0x0A):(l-'0');
	return (h<<4)|l;
}

/**************************************************************************************/
//                              Global Variable
/**************************************************************************************/

/**************************************************************************************/
//                               Watchdog
// Enable watchdog timer.
// Timeout limit WDTCR[2:0]:
// 000 - 16.3ms    001 - 32.5ms    010 - 65ms    011 - 0.13s
// 100 - 0.26ms    101 - 0.52s     110 - 1.0s    111 - 2.1s
/**************************************************************************************/
#define WDCE 4
#define WDE  3



void Turn_on_Watchdog()
{
  #asm("cli")
  #asm("wdr")                    // reset watchdog timer to avoid timeout reset
  WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
  WDTCR = 0x08;                  // turn on watch dog timer: WDE = 1, WDP = 000, 16.3ms.
  #asm("sei")                    // Enable global interrupt.
}

char namebuf[8];
char* tname2b(u8 i)
{
        u8 j=0;
        for(j=0;j<8;j++)
        {
            namebuf[j] = tprbdata.name[i][j];
            if(namebuf[j] == '\0')
                break;

            if((namebuf[j] >= '0' && namebuf[j] <= '9') || (namebuf[j] >= 'A' && namebuf[j] <= 'Z'))
                continue;
            namebuf[0] = '\0';
            break;
        }
        return namebuf;
}

char* rname2b(u8 i)
{
        u8 j=0;
        for(j=0;j<8;j++)
        {
            namebuf[j] = rprbdata.name[i][j];
            if(namebuf[j] == '\0')
                break;

            if((namebuf[j] >= '0' && namebuf[j] <= '9') || (namebuf[j] >= 'A' && namebuf[j] <= 'Z'))
                continue;
            namebuf[0] = '\0';
            break;
        }
        return namebuf;
}
void sleepms(u16 ms)
{
    uint p;
    while(ms-- > 0)
    {
        p = 24;//80;
        while(p-- > 0)
                ;
    }
}


/**************************************************************************************/
//      board init related function.
/**************************************************************************************/
void init_var()
{
        if((sysdata.Rs1 > 101) || (sysdata.Rs1 < 99))
                sysdata.Rs1 = 100;
        if((sysdata.Rs2 > 1010) || (sysdata.Rs2 < 990))
                sysdata.Rs2 = 1000;
}
//state machine of therm type
//phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
//phase 1:get the reading and update the display
//phase 2: move to next channel
uchar therm_state()
{
        uchar i;
	i = sysdata.tid[ch_to_search];
	if(phase > 2)
	        phase = 0;
	if(i == INVALID_PROBE)
	{
        	rundata.temperature[ch_to_search] = -9999;
        	phase = 2;
	}
	if(phase == 0)
	{
		if( (tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
		{
			scanner_set_channel(ch_to_search+1);
			dlg_cnt = 6*ONESEC;
			onesec_cnt = 0;
			phase = 1;
			return 0;
		}else{
			rundata.temperature[ch_to_search] = -9999; //invalid data
			phase = 2;
		}
	}
	if(phase == 1)
	{
		rundata.reading[ch_to_search] = nav_read();//-sysdata.V0;
                sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.reading[ch_to_search]);
                prints(strbuf,strlen(strbuf),PORT_PC);

		rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], tprbdata.type[i]);
//		if(ch_to_search==1) 		     rundata.temperature[ch_to_search] = 10;
                sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
                prints(strbuf,strlen(strbuf),PORT_PC);
		phase = 2;
	}
	if(phase == 2)
	{
		pgmain_handler(MSG_REFRESH);
		phase = 0;
	}
	return 1;       //one reading is updated
}
//state machine of bore measure
//phase0 the search the current channel and switch to it if apply, switch to rs, pktt, add delay, if not ktt to phase 2 else to phase 1, otherwise to phase n
//phase1 get reading of rs+, set to nktt, add delay
//phase2 get reading of rs-, switch to rx, add delay, if  ktt to phase 3 else to phase 4
//phase3 get reading of rx-, set to pktt, add delay
//phase4 get reading of rx+, switch to rx, caculate
//phase5 update display and move to next channel
uchar bore_state()
{
        uchar i;
	i = sysdata.rid[ch_to_search];
	if(phase > 5)
	        phase = 0;
	if(i == INVALID_PROBE)
	{
        	rundata.temperature[ch_to_search] = -9999;
        	phase = 5;
	}
	if(phase == 0)
	{
		if((rprbdata.type[i] <= PRBTYPE_MAX) && (rprbdata.type[i] >= PRBTYPE_MIN))
		{
			scanner_set_channel(ch_to_search+1);
			SET_PKTT;
			SET_TORS;

			if(rprbdata.type[i] == PRBTYPE_PT100)
			{
			        SET_TOPT100;
			        SET_TO1MA;
			        PORTF = 0x0f;
			}
			if(rprbdata.type[i] == PRBTYPE_PT25)
			{
			        SET_TOPT100;
			        SET_TO1MA;
			        PORTF = 0x0f;
			}

			if(rprbdata.type[i] == PRBTYPE_PT1000)
			{
			        SET_TOP1MA;
       			        SET_TOPT1000;
       			        PORTF = 0x00;
			}
			dlg_cnt =  ONESEC * sysdata.ktime;
			onesec_cnt = 0;
			if(IS_MODE_KTT)
				phase = 1;
			else
				phase = 2;
       			return 0;
		}else{
			rundata.temperature[ch_to_search] = -9999;
			phase = 5;
		}

	}
	if(phase == 1)  //-stdV in ktt mode
	{
		rundata.stdV = mabs(nav_read());
		SET_NKTT;
		dlg_cnt =  ONESEC * sysdata.ktime;
		onesec_cnt = 0;
		phase = 2;
		return 0;
	}
	if(phase == 2) //final stdV
	{
		if(IS_MODE_KTT)
		{
			rundata.stdV = (rundata.stdV + mabs(nav_read()));
			phase = 3;
		}else{
			rundata.stdV = mabs(nav_read());
			phase = 4;
		}
		SET_TORX;

		dlg_cnt = ONESEC * sysdata.ktime;
		onesec_cnt = 0;
		return 0;
	}
	if(phase == 3)  //-V on rx in ktt mode
	{
		valuep = mabs(nav_read());
		SET_PKTT;
		dlg_cnt = ONESEC * sysdata.ktime;
		onesec_cnt = 0;
		phase = 4;
		return 0;
	}
	if(phase == 4) // final V on rx
	{
		if(IS_MODE_KTT){
			valuep = (valuep + mabs(nav_read()));
			dbgout(valuep);
/*
			if((valuep > 0.21) && (valuep < 2) && (nav1v == 0))
			{
        		        navto1v();
			}
			if((valuep < 0.21) && (valuep > 0.0002) && (nav1v == 1))
			{
			        navto120mv();
			}
*/
		}else{
			valuep = mabs(nav_read());
/*
			if((valuep > 0.105) && (valuep < 1) && (nav1v == 0))
			{
        		        navto1v();
			}
			if((valuep < 0.105) && (valuep > 0.0001) && (nav1v == 1))
			{
			        navto120mv();
			}
*/
		}
		if(rundata.stdV != 0)
		{
                        if(rprbdata.type[sysdata.rid[ch_to_search]] == PRBTYPE_PT1000)
              			rundata.reading[ch_to_search] = valuep*sysdata.Rs2/rundata.stdV - sysdata.R0;
                        else
        			rundata.reading[ch_to_search] = valuep*sysdata.Rs1/rundata.stdV - sysdata.R0;
			if(rundata.reading[ch_to_search] > 0)
			{
                                sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.Rx);
                                prints(strbuf,strlen(strbuf),PORT_PC);

			        rundata.temperature[ch_to_search] = RValueToTValue(rundata.reading[ch_to_search],i);
                                rundata.Rx = rundata.reading[ch_to_search];
                                sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
                                prints(strbuf,strlen(strbuf),PORT_PC);
			}
		}else{
		        rundata.temperature[ch_to_search] = -9999;
		}
		phase = 5;
	}
	if(phase == 5) //got one reading
	{
		pgmain_handler(MSG_REFRESH);
		phase = 0;
	}
	return 1;
}
LABEL flash statelbl = {LBL_HZ6X8,50,55,26,strbuf};

void updatestate()
{
        char star[6];
        sprintf(star,"    ");
        if(phase == 0)                sprintf(star,"*   ");
        if(phase == 1)                sprintf(star,"**  ");
        if(phase == 2)                sprintf(star,"*** ");
        if(phase == 3)                sprintf(star,"****");
        if(IS_BORE_MODE){
                sprintf(strbuf,"(%s:ch%2i,%d,%s)",rname2b(sysdata.rid[ch_to_search]),ch_to_search+1, dlg_cnt/ONESEC,star);
        }else{
                sprintf(strbuf,"(%s:ch%2i,%d,%s)",tname2b(sysdata.tid[ch_to_search]),ch_to_search+1, dlg_cnt/ONESEC,star);
        }

/*
        if(IS_BORE_MODE){
                if(IS_MODE_KTT)
                        sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
                else
                        sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
        }else{
                sprintf(strbuf,"(ch:%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
        }
*/
        draw_label(&statelbl,SW_NORMAL);
}
static uchar tA = 0xff;
static uchar tB = 0xff;
void testA(uchar data)
{
        tA = data;
}
void testB(uchar data)
{
        tB = data;
}
/**************************************************************************************/
//                              Main Function Loop
/**************************************************************************************/
extern double GetThmoVolt(double t,char type);
extern u8 databuf[12];
extern u8 pos_databuf; //position in data buffer
void main(void)
{

    u16 i;
    u8 shortcut = KEY_INVALID;
    // RS485 Node
    init_var();	//init data structure
    // System Initialization
    Init_Port();
//    Init_Timers();
//    Init_Ex_Interrupt();
    Init_UART();
    Enable_XMEM();
    Init_554();
    InitLED();
    Key_Init();
    // Global enable interrupts
    WDTCR = 0x00; //disable dog watch
    #asm("sei")
    /*********************************************************************/
    // System hardware dection
    /*********************************************************************/
    // intialize LED.
    nextwin = 0;

    sleepms(20*ONEMS);
    LCD_Init();
    wnd_msgbox(&bootup);
    //init the DMM
    nav_command(NAV_INIT);
    sleepms(200*ONEMS);
    navto1v();
    nav_command(NAV_SLOWMODE);
    sleepms(200*ONEMS);
    nav_command(NAV_AFLTON);
    sleepms(200*ONEMS);

    sleepms(2*ONEMS); //wait until all the node is ready after power up
    State_Init();

    SET_BORE_MODE;

	 nextwin = PG_BOOTTYPE;
	 key = KEY_INVALID;

	 curr_ch = 1; //channel for display
	 curr_dispch = 1;
	 sysdata.R0 = 0;
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
			        shortcut = key;
                                //processing shortcut key
				if(curr_window == pgmain_handler)
				{
					LCD_Cls();
					wnd_msgbox(&modify);
				}
				if(shortcut == KEY_BTN1) //mode switch
				{
               			        SET_TOP1MA;
	                	        SET_TOPT1000;
					if(IS_BORE_MODE){
						SET_THERM_MODE;
					}else{
						SET_BORE_MODE;
					}
					dlg_cnt = 0;
					onesec_cnt = 0;
					phase = 0;      //reset the state machine
				}
				if(shortcut == KEY_BTN2) //auto ktt or not
				{
					if(IS_BORE_MODE)
					{
                      			        SET_TOP1MA;
	                        	        SET_TOPT1000;
						if((IS_MODE_KTT)){
							CLR_MODE_KTT;
							SET_PKTT;
						}else{
							SET_MODE_KTT;
							SET_PKTT;
						}
        					dlg_cnt = 0;
	        				onesec_cnt = 0;
		        			phase = 0;      //reset the state machine
					}
				}
				if(shortcut == KEY_BTN3) //thermal probe type
				{
					display_buttons(KEY_BTN3,1);
					if(IS_THERM_MODE)
					{
					        i = sysdata.tid[curr_dispch-1];
					        if(i != INVALID_PROBE)
					        {
                					if((tprbdata.type[i] >= PRBTYPE_K) &&\
	                				   (tprbdata.type[i] <= PRBTYPE_R))
		                			{
			                			if(tprbdata.type[i] == PRBTYPE_R)
				                			tprbdata.type[i] = PRBTYPE_K;
					                	else
						                	tprbdata.type[i] +=1;
        					        }
        					        if(rundata.reading[curr_dispch-1] > -9000)
                                                       		rundata.temperature[curr_dispch-1] = MValueToTValue(rundata.reading[curr_dispch-1], tprbdata.type[i]);
        					}
                                        }
					display_buttons(KEY_BTN3,0);

				}
				if(shortcut == KEY_BTN4) //remove zero
				{
					display_buttons(KEY_BTN4,1);
					if(IS_BORE_MODE){
					        sysdata.R0 = 0;// rundata.Rx;
					}else{
					        //sysdata.V0 = nav_read();
					        nav_command(NAV_ZEROON);
					        sleepms(1000*ONEMS);
					}
					display_buttons(KEY_BTN4,0);
				}
				if(curr_window == pgmain_handler)       //redraw the running window
				{
					pgmain_handler(MSG_INIT);
				}
			        shortcut = KEY_INVALID;

			}else{
				(*curr_window)(key);
			}
			key = KEY_INVALID;
	  	}else{
			if(curr_window != pgmain_handler)
				continue;
			if(dlg_cnt > 1)
			{
			        onesec_cnt++;
			        if(onesec_cnt == (ONESEC-10))
			        {
        			        updatestate();
        			}
        			if(onesec_cnt == ONESEC)
        			        onesec_cnt = 0 ;
				dlg_cnt--;
				continue;
			}
			updatestate();
			if((IS_THERM_MODE))
			{
				if(therm_state() == 0)
				        continue;
			}else{
				if(bore_state() == 0)
				        continue;
			}

                        //shift to next channel

                        while(true)
                        {
                                ch_to_search += 1;
        			if(ch_to_search >= MAX_CH_NUM)
        			{
	        			ch_to_search = 0;
	        			break;
	        		}
	        		if(IS_THERM_MODE)
	        		{
	        		        i = sysdata.tid[ch_to_search];
	        		}else{
	        		        i = sysdata.rid[ch_to_search];
	        		}
	        		if(i == INVALID_PROBE)
	        		        continue;
	        		if(IS_THERM_MODE)
	        		{
       					if((tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
       					        break;
	        		}else{
       		                        if((rprbdata.type[i] <= PRBTYPE_MAX) && (rprbdata.type[i] >= PRBTYPE_MIN))
       		        		        break;
	        		}
	                }
		}
	}
}

WINDOW flash wins[]={
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
/*********************************************************************************/
//                         UART0 and UART1 Subroutines
/*********************************************************************************/
#include <mega128.h>
#include "uart.h"
#include "16c554.h"
#include "global.h"

//extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
//extern u8 x_modem_databuf[];                       /* store data sent from PC */
//extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
//extern u8 x_modem_file_being_transferred;
//extern u8 x_modem_file_transfer_complete;

/*********************************************************************************/
//                     UART0  Variables & Buffer Definition
/*********************************************************************************/
u8 volatile tx_buffer0[TX_BUFFER_SIZE0];
u8 volatile tx_wr_index0,tx_rd_index0,tx_counter0;

/*********************************************************************************/
//                     UART1  Variables & Buffer Definition
/*********************************************************************************/
u8 volatile tx_buffer1[TX_BUFFER_SIZE1];
u8 volatile tx_wr_index1,tx_rd_index1,tx_counter1;

/*
 * 0-9,a-f,A-F char to hex conversion
 * otherwise 0xff will be returned.
 */
u8 ishexchar(u8 dat){
        if( (dat >= '0') && (dat <= '9'))
                return dat-'0';
        if( (dat >= 'a') && (dat <= 'f'))
                return dat-'a'+0x0a;
        if( (dat >= 'A') && (dat <= 'F'))
                return dat-'A'+0x0a;
        return 0xff;
}



interrupt [USART0_RXC] void usart0_rx_isr(void)
{
   u8 status,data;
   //read status and data
   status=UCSR0A;
   data=UDR0;
   cm0_push(data);
}

/*********************************************************************************/
//                 USART0 Transmitter interrupt service routine
/*********************************************************************************/
interrupt [USART0_TXC] void usart0_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0++];
   tx_rd_index0 =tx_rd_index0 & (TX_BUFFER_SIZE0-1);
   };
}

/*********************************************************************************/
//                      UART1 Receiver interrupt service routine
/*********************************************************************************/
interrupt [USART1_RXC] void usart1_rx_isr(void)
{
   u8 status,data;

   //read status and data
   status=UCSR1A;
   data=UDR1;
   cm1_push(data);
}

/*********************************************************************************/
//                   USART1 Transmitter interrupt service routine
/*********************************************************************************/
interrupt [USART1_TXC] void usart1_tx_isr(void)
{
if (tx_counter1)
   {
   --tx_counter1;
   UDR1=tx_buffer1[tx_rd_index1++];
   tx_rd_index1=tx_rd_index1 & (TX_BUFFER_SIZE1-1);
   };
}

void com0_putc(char c)
{
if(DEBUG == 1)
        return;
while (tx_counter0 == TX_BUFFER_SIZE0);
#asm("cli")
if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0++]=c;
   tx_wr_index0 = tx_wr_index0 & (TX_BUFFER_SIZE0-1);
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}


void com1_putc(char c)
{
while (tx_counter1 == TX_BUFFER_SIZE1);
#asm("cli")
if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer1[tx_wr_index1++]=c;
   tx_wr_index1 = tx_wr_index1 & (TX_BUFFER_SIZE1-1);
   ++tx_counter1;
   }
else
   UDR1=c;
#asm("sei")
}

/*
void com0_puthex(u8 a)
{
	unsigned char h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)
		com0_putc(h+'0');
	else
		com0_putc(h+'A'-0x0a);
	if(l <= 9)
		com0_putc(l+'0');
	else
		com0_putc(l+'A'-0x0a);
}
*/

void com0_putstr(u8 *ptr){
        while(*ptr != 0x00){
                com0_putc(*ptr++);
        }
}

void com1_puthex(u8 a)
{
	unsigned char h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)
		com1_putc(h+'0');
	else
		com1_putc(h+'A'-0x0a);
	if(l <= 9)
		com1_putc(l+'0');
	else
		com1_putc(l+'A'-0x0a);
}
void com1_putstr(u8 *ptr){
        while(*ptr != 0x00){
                com1_putc(*ptr++);
        }
}
/*
void com0_mputs(u8 *buf, u8 size){
        while(size-- > 0){
                com0_puthex(*buf++);
        }
}
*/
void com1_mputs(u8 *buf, u8 size){
        while(size-- > 0){
                com1_puthex(*buf++);
        }
}

/*********************************************************************************/
//                              Intialize UART
/*********************************************************************************/
void Init_UART()
{
   // USART0 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART0 Receiver: On
   // USART0 Transmitter: On
   // USART0 Mode: Asynchronous
   // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
      UCSR0A=0x00;
      UCSR0B=0xD8;
      UCSR0C=0x06;
      UBRR0H=0x00;
      UBRR0L=0x0F;

   // USART1 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART1 Receiver: On
   // USART1 Transmitter: On
   // USART1 Mode: Asynchronous
   // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
      UCSR1A=0x00;
      UCSR1B=0xD8;
      UCSR1C=0x06;
      UBRR1H=0x00;
      UBRR1L=0x0F;

   // Initialize buffer variables
      tx_wr_index0 = 0;
      tx_rd_index0 = 0;
      tx_counter0  = 0;

      tx_wr_index1 = 0;
      tx_rd_index1 = 0;
      tx_counter1  = 0;
}
/********************************************************************************/
//                             TL16C554 Driver
// Controller:  MEGA64
// 16C554 clk:  7.3728MHZ
//
// Schematic Description:
// 16C554 <---> MEGA64
// INTA   <---> INT4
// INTB   <---> INT5
// INTC   <---> INT6
// INTD   <---> INT7
// CSA#   <---> A15
// CSB#   <---> A14
// CSC#   <---> A13
// CSD#   <---> A12
//   A0   <---> A8
//   A1   <---> A9
//   A2   <---> A10
// RESET  <---> PB0
// INTN#  <---> PE2

// Coded by: fd_lxh@yahoo.com.cn
/********************************************************************************/
#include "mega128.h"
#include "global.h"
#include "16c554.h"
#include "uart.h"
#include "utili.h"
#define FCR_DATA 0b00000111   // FIFO trigger level: 1 byte
#define IER_DATA 0b00000001
// LCR.7 must be cleared to 0 when accessing RBR/THR/IER
// LCR.7 must be set to 1 when accessing divisor latch
#define LCR_DATA 0b00011011    // Even Parity 8 data bit, 1 stop bit
#define DLL_DATA 0x30          // 115200bps: DLL = 0x4, 57600bps: DLL= 0x8, 38400 0xC: 9600 : 0x30
#define DLM_DATA 0x0
#define MCR_DATA 0b00001000

#define min(A,B)        ((A<B)?(A):(B))
/********************************************************************************/
//               TLC16C554 UART PORT A Interrupt Subroutine
/********************************************************************************/
interrupt [EXT_INT4] void ext_int4_isr(void)
{
   u8 rundata;
   switch(UC_554A_IIR & 0xF)
   {
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = UC_554A_RBR;        // read data from receiver FIFO. RBR read to clear int
          cm_pushA(rundata);
          return;
      default:
          break;
   }

   rundata = UC_554A_LSR;        // read LSR to clear interrupt. dummy read
}
/********************************************************************************/
//               TLC16C554 UART PORT B Interrupt Subroutine
/********************************************************************************/
// External Interrupt 5 service routine
interrupt [EXT_INT5] void ext_int5_isr(void)
{
   u8 rundata;
   switch(UC_554B_IIR & 0xF)
   {
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = UC_554B_RBR;        // read data from receiver FIFO. RBR read to clear int
          cm_pushB(rundata);
          return;
      default:
          break;
   }
   rundata = UC_554B_LSR;        // read LSR to clear interrupt. dummy read
}
/********************************************************************************/
//               TLC16C554 UART PORT C Interrupt Subroutine
/********************************************************************************/
// External Interrupt 6 service routine
interrupt [EXT_INT6] void ext_int6_isr(void)
{
   u8 rundata;
   switch(UC_554C_IIR & 0xF)
   {
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = UC_554C_RBR;        // read data from receiver FIFO. RBR read to clear int
          cm_pushC(rundata);
          return;
      default:
          break;
   }

   rundata = UC_554C_LSR;        // read LSR to clear interrupt. dummy read
}

/********************************************************************************/
//               TLC16C554 UART PORT D Interrupt Subroutine
/********************************************************************************/
// External Interrupt 7 service routine
interrupt [EXT_INT7] void ext_int7_isr(void)
{
   u8 rundata;
   switch(UC_554D_IIR & 0xF)
   {
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
          cm_pushD(rundata);
          return;
      default:
          break;
   }
   rundata = UC_554D_LSR;        // read LSR to clear interrupt. dummy read
}

/********************************************************************************/
//                             Reset 16C554
/********************************************************************************/
void Reset_554(void)
{
   // reset 16C554. Reset pin of 16C554 is connected to PB0 of MEGA64
      PORTE.3 = 1;
      sleepms(5*ONEMS);    // Delay
      PORTE.3 = 0;
      sleepms(5*ONEMS);    // Delay
}
/********************************************************************************/
//                             Initialize 16C554
/********************************************************************************/
void Init_554(void)
{
      Reset_554();  // Reset 554

   /********************************************************************/
   //                      Initialize UART A
   /********************************************************************/
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      UC_554A_LCR = 0x80;
      UC_554A_DLL = DLL_DATA;
      UC_554A_DLM = DLM_DATA;

   // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      UC_554A_LCR = LCR_DATA;

   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      UC_554A_FCR = FCR_DATA;

   // Set Modem Control Register:
   // Enable enternal interrupt
      UC_554A_MCR = MCR_DATA;

   // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      UC_554A_IER = IER_DATA;

   /********************************************************************/
   //                      Initialize UART B
   /********************************************************************/
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
   // LCR.7 must be set to "1" before setting baud rate
      UC_554B_LCR = 0x80;
      UC_554B_DLL = DLL_DATA;
      UC_554B_DLM = DLM_DATA;

   // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      UC_554B_LCR = LCR_DATA;

   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      UC_554B_FCR = FCR_DATA;

   // Set Modem Control Register:
   // Enable enternal interrupt
      UC_554B_MCR = MCR_DATA;

   // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      UC_554B_IER = IER_DATA;

   /********************************************************************/
   //                      Initialize UART C
   /********************************************************************/
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      UC_554C_LCR = 0x80;
      UC_554C_DLL = DLL_DATA;
      UC_554C_DLM = DLM_DATA;

   // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      UC_554C_LCR = LCR_DATA;

   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      UC_554C_FCR = FCR_DATA;

   // Set Modem Control Register:
   // Enable enternal interrupt
      UC_554C_MCR = MCR_DATA;

   // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      UC_554C_IER = IER_DATA;

   /********************************************************************/
   //                      Initialize UART D
   /********************************************************************/
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      UC_554D_LCR = 0x80;
      UC_554D_DLL = DLL_DATA;
      UC_554D_DLM = DLM_DATA;

   // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      UC_554D_LCR = LCR_DATA;

   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      UC_554D_FCR = FCR_DATA;

   // Set Modem Control Register:
   // Enable enternal interrupt
      UC_554D_MCR = MCR_DATA;

   // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      UC_554D_IER = IER_DATA;
}


/********************************************************************************/
//                 Send multi-bytes (less than 16 bytes) via UART
// Function:
// Send multi-bytes ( < 16 Bytes) through the port specified by "uart_port"
// format:
// print_554(u8 *str, char uart_port)
// arg1: string to be sent. arg2: port (A B,C,D)
/********************************************************************************/
void prints(u8 *str, u8 length, char uart_port)
{
    u8 len, i;
    //len = length & 0x0F;        //15bytes at most
    len = length;
    switch(uart_port)
    {
       case SPORTA:
           // wait if data still being transmitted in UART
           // LSR5: Transmitter Holding Register Empty, 1 Empty, 0: Not Empty
           // LSR6: Transmitter Register Empty. 1 Empty, 0: Not Empty.
           // check if FIFO is empty by LSR5.
           // Note: (UC_554A_LSR) & 0x20 bracket here is necessary because
           // UC_554A_LSR is not a register name,
           // it is replaced by "*(volatile u8 *)(UARTA_BASE_ADDR+0x300)" during compilation
           // thus UC_554A_LSR & 0x20 actually means *(volatile u8 *) ((UARTA_BASE_ADDR+0x300) & 0x20)
           // here in 16c554.h, a bracket is added
           while(len)
           {
              while( !(UC_554A_LSR & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
                  UC_554A_THR = *str++;
              len = len - min(len,16);
           }
           break;
       case SPORTB:
           while(len)
           {
              while( !(UC_554B_LSR & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
                  UC_554B_THR = *str++;
              len = len - min(len,16);
           }
           break;
       case SPORTC:
           while(len)
           {
              while( !(UC_554C_LSR & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
                  UC_554C_THR = *str++;
              len = len - min(len,16);
           }

           break;
       case SPORTD:
           while(len)
           {
              while( !(UC_554D_LSR & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
                  UC_554D_THR = *str++;
              len = len - min(len,16);
           }
           break;
       case SPORT0:
           while(len-- > 0){
                com0_putc(*str++);
           }
           break;
       case SPORT1:
           while(len-- > 0){
                com1_putc(*str++);
           }
           break;
       default:
           PORTC = 0xF0;
           break;
    }
}


// System Initialization file

#include <mega128.h>
#include "global.h"
/**************************************************************************************/
//                              Initialize Port A-G
/**************************************************************************************/
void Init_Port()
{
 // Input/Output Ports initialization
 // Port A initialization
 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
    PORTA=0x00;
    DDRA=0x00;

 // Port B initialization
 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=Out
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=1
    PORTB=0xFF;
    DDRB=0xFF;

 // Port C initialization
 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
 // State7=1 State6=1 State5=1 State4=1 State3=0 State2=0 State1=0 State0=0
 // PORTC[7:4]: chip select of 16C554 channels.
    PORTC=0xF0;
    DDRC=0xFF;

 // Port D initialization
 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
    PORTD=0x00;
    DDRD=0x00;

 // Port E initialization
 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=In Func0=In
 // State7=T State6=T State5=T State4=T State3=T State2=0 State1=T State0=T
 // PORTE.0: Input (M_RXD0)
 // PORTE.1: output (M_TXD0)
 // PORTE.2: output (16C554 INTC#)
 // PORTE.3: output (16C554 reset)
    PORTE=0x06;
    DDRE=0x0E;

 // Port F initialization
 // Input
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=Out State0=Out
 // PORTF[3:0]: LED[4:1]
    PORTF=0x0F;
    DDRF=0x0F;

 // Port G initialization
 // Input
    PORTG=0x00;
    DDRG=0x00;
}

/**************************************************************************************/
//                              Initialize Timers
/**************************************************************************************/
void Init_Timers()
{
 // Timer/Counter 0 initialization
 // Clock source: System Clock
 // Clock value: 14.400 kHz
 // Mode: Normal top=FFh
 // OC0 output: Disconnected
    ASSR=0x00;
    TCCR0=0x07;
    TCNT0=0x00;
    OCR0=0x00;

 // Timer/Counter 1 initialization
 // Clock source: System Clock
 // Clock value: 14.400 kHz
 // Mode: Normal top=FFFFh
 // OC1A output: Discon.
 // OC1B output: Discon.
 // OC1C output: Discon.
 // Noise Canceler: Off
 // Input Capture on Falling Edge
 // Timer 1 Overflow Interrupt: On
 // Input Capture Interrupt: Off
 // Compare A Match Interrupt: Off
 // Compare B Match Interrupt: Off
 // Compare C Match Interrupt: Off
    TCCR1A=0x00;
    TCCR1B=0x05;
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;
    OCR1CH=0x00;
    OCR1CL=0x00;

 // Timer/Counter 2 initialization
 // Clock source: System Clock
 // Clock value: Timer 2 Stopped
 // Mode: Normal top=FFh
 // OC2 output: Disconnected
    TCCR2=0x00;
    TCNT2=0x00;
    OCR2=0x00;

 // Timer/Counter 3 initialization
 // Clock source: System Clock
 // Clock value: Timer 3 Stopped
 // Mode: Normal top=FFFFh
 // Noise Canceler: Off
 // Input Capture on Falling Edge
 // OC3A output: Discon.
 // OC3B output: Discon.
 // OC3C output: Discon.
 // Timer 3 Overflow Interrupt: Off
 // Input Capture Interrupt: Off
 // Compare A Match Interrupt: Off
 // Compare B Match Interrupt: Off
 // Compare C Match Interrupt: Off
    TCCR3A=0x00;
    TCCR3B=0x00;
    TCNT3H=0x00;
    TCNT3L=0x00;
    ICR3H=0x00;
    ICR3L=0x00;
    OCR3AH=0x00;
    OCR3AL=0x00;
    OCR3BH=0x00;
    OCR3BL=0x00;
    OCR3CH=0x00;
    OCR3CL=0x00;

 // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=0x05;
    ETIMSK=0x00;
}

/**************************************************************************************/
//                              Enable External Memory
/**************************************************************************************/
void Enable_XMEM()
{
 // External SRAM page configuration:
 // 1100h - 7FFFh / 8000h - FFFFh
 // Lower page wait state(s): 2r/w +1 addr
 // Upper page wait state(s): 2r/w + 1 addr
    MCUCR=0xC0;
    //MCUCR = 0x80;

    XMCRA=0x4E;
    //XMCRA=0x0A;
 // PC7:0 can not be released
    XMCRB=0x80;
    // XMCRB = 0x07;
}

/**************************************************************************************/
//                              Initialize External Interrupt
/**************************************************************************************/
void Init_Ex_Interrupt()
{
 // External Interrupt(s) initialization
 // INT0: Off
 // INT1: Off
 // INT2: Off
 // INT3: Off
 // INT4: On
 // INT4 Mode: Rising Edge
 // INT5: On
 // INT5 Mode: Rising Edge
 // INT6: On
 // INT6 Mode: Rising Edge
 // INT7: On
 // INT7 Mode: Rising Edge
    EICRA=0x00;
    EICRB=0xFF;
    EIMSK=0xF0;
    EIFR=0xF0;
}

/**************************************************************************************/
//                              Analog Comparator initialization
/**************************************************************************************/
void Init_ADC()
{
 // Analog Comparator initialization
 // Analog Comparator: Off
 // Analog Comparator Input Capture by Timer/Counter 1: Off
    ACSR=0x80;
    SFIOR=0x00;
}
#include "global.h"
#include "wendu.h"
#include "utili.h"
#include "math.h"
#define logl log
#define powl pow
double PT100RToTValue(double r,double r0)
{
        double ac,bc,cc,tlow,tup,rnew,tnew;
        int count;
        ac = 3.908e-3;
        bc = -5.775E-7;
        cc = -4.183E-12;
        tlow = -200;
        tup = 850;
	count = 0;

	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = r0 + r0*ac*tnew + r0*bc*tnew*tnew;
		if(tnew < 0)
		         rnew = rnew + r0*cc*(tnew-100)*tnew*tnew*tnew;

		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;

}
double RValueToTValue(double r, u8 prbid)
{
	double ac,bc,cc,tlow,tup,rnew,tnew;
	int count;

	ac = rprbdata.param1[prbid];
	bc = rprbdata.param2[prbid];
	cc = rprbdata.param3[prbid];

	if(rprbdata.type[prbid] == PRBTYPE_PT100)
		return PT100RToTValue(r, cc);
	if(rprbdata.type[prbid] == PRBTYPE_PT1000)
		return PT100RToTValue(r, cc);
	if(rprbdata.type[prbid] != PRBTYPE_PT25)
		return -9999.999;
        if(rprbdata.rtp[prbid] > 0.1)
                r = r/rprbdata.rtp[prbid];
        else
		r = r/25.0;





	//set the search range of T between GetT(r) +/- 1 degree
	tlow = GetT(r) - 1;
	tup = tlow + 2;

	count = 0;

	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetWr(tnew);
		if((tnew >= (83.8058-273.15)) && (tnew <= (273.16-273.15)))
        		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*logl(rnew);
                else
        		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;


//	return r+1;
}

flash double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};
flash u8 sizeAi = sizeof(Ai)/sizeof(double);
flash u8 sizeCi = sizeof(Ci)/sizeof(double);

double GetWr(double t)
{
	double result;
	double pert;
	int i;
	if(t < 0)
	{       //equals to Wr189(t) in mfile
		result = Ai[0];
		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
		t = pert;
		for(i=1;i<sizeAi;i++)
		{
			result = result + Ai[i] * t;
			t = t*pert;
		}
		result = exp(result);
	}else{
			 result = Ci[0];
			 pert = t/481.0 - 1;
			 t = pert;
			 for(i=1;i<sizeCi;i++)
			 {
				 result = result + Ci[i] * t;
				 t = t*pert;
			 }
	}
	return result;
}
flash double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
flash double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
flash u8 sizeDi = sizeof(Di)/sizeof(double);
flash u8 sizeBi = sizeof(Bi)/sizeof(double);

double GetT(double w)
{
	double perw;
	double result;
	int i;

	if(w >= 0)
	{       //t660r
		perw = (w-2.64)/1.64;
		w = perw;
		result = Di[0];
		for(i=1;i<sizeDi;i++)
		{
			result = result + Di[i] * w;
			w = w*perw;
		}
	}else{   //t189(r)
		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
		w = perw;
		result = Bi[0];
		for(i=1;i<sizeBi;i++)
		{
			result = result + Bi[i] * w;
			w = w*perw;
		}
		result = 273.15*result - 273.15;
	}
	return result;
}

flash double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};

flash double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};

flash double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};

flash double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};

flash double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};

flash double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};

flash double BLow[]={0.000000000000E+00,-0.24650818346E-03,0.59040421171E-05,-0.13257931636E-08,0.15668291901E-11,-0.16944529240E-14,0.62990347094E-18};
flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};

flash double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};

flash double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};

flash int TLowLen = sizeof(TLow)/sizeof(double);
flash int THighLen = sizeof(THigh)/sizeof(double);

flash int SLowLen = sizeof(SLow)/sizeof(double);
flash int SMedLen = sizeof(SMed)/sizeof(double);
flash int SHighLen = sizeof(SHigh)/sizeof(double);

flash int RLowLen = sizeof(RLow)/sizeof(double);
flash int RMedLen = sizeof(RMed)/sizeof(double);
flash int RHighLen = sizeof(RHigh)/sizeof(double);

flash int NLowLen = sizeof(NLow)/sizeof(double);
flash int NHighLen = sizeof(NHigh)/sizeof(double);

flash int JLowLen = sizeof(JLow)/sizeof(double);
flash int JHighLen = sizeof(JHigh)/sizeof(double);

flash int BLowLen = sizeof(BLow)/sizeof(double);
flash int BHighLen = sizeof(BHigh)/sizeof(double);

flash int ELowLen = sizeof(ELow)/sizeof(double);
flash int EHighLen = sizeof(EHigh)/sizeof(double);

flash int KLowLen = sizeof(KLow)/sizeof(double);
flash int KHighLen = sizeof(KHigh)/sizeof(double);

flash double *coef;

double GetThmoVolt(double t,char type)
{
	double result,pert;
	int i;
	int len = 0;
	switch(type)
	{
		case PRBTYPE_T:
			coef = (t < 0) ? TLow : THigh;
			len = (t < 0) ? TLowLen : THighLen;
			break;
		case PRBTYPE_K:
			coef = (t < 0) ? KLow : KHigh;
			len = (t < 0) ? KLowLen : KHighLen;
			break;
		case PRBTYPE_N:
			coef = (t < 0) ? NLow : NHigh;
			len = (t < 0) ? NLowLen : NHighLen;
			break;
		case PRBTYPE_E:
			coef = (t < 0) ? ELow : EHigh;
			len = (t < 0) ? ELowLen : EHighLen;
			break;
		case PRBTYPE_B:
			coef = (t < 630.615) ? BLow : BHigh;
			len = (t < 630.615) ? BLowLen : BHighLen;
			break;
		case PRBTYPE_J:
			coef = (t < 760) ? JLow : JHigh;
			len = (t < 760) ? JLowLen : JHighLen;
			break;

		case PRBTYPE_S:
			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
			break;
		case PRBTYPE_R:
			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
			break;
		default:
			return 0.0;
	}
	if (len == 0)
		return 0.0;

	result = coef[0];
	if(type == PRBTYPE_K)
	{
		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	}

	pert = t;
	for(i=1;i< len ;i++)
	{
		result = result + coef[i] * t;
		t = t*pert;
	}
	return result;
}

double MValueToTValue(double r,char type)
{
	double tlow;
	double tup;
	double rnew;
	double tnew;
	int count = 0;
	switch(type)
	{
		case PRBTYPE_T:
			tlow =  -270;	tup = 400;
			break;
		case PRBTYPE_K:
			tlow =  -270;	tup = 1372;
			break;
		case PRBTYPE_N:
			tlow =  -270;	tup = 1300;
			break;
		case PRBTYPE_E:
			tlow =  -270;	tup = 1000;
			break;
		case PRBTYPE_B:
			tlow =  0;	tup = 1800;
			break;
		case PRBTYPE_J:
			tlow =  -210;	tup = 1200;
			break;
		case PRBTYPE_S:
			tlow =  -50;	tup = 1768;
			break;
		case PRBTYPE_R:
			tlow =  -50;	tup = 1768;
			break;
		default:
			return -9999.9999;
	}

	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetThmoVolt(tnew,type);

		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}

	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
}

#include "font.h"

#include <MATH.H>
#include "typedef.h"
#include "global.h"
#include "utili.h"
#include "key.h"
extern uchar key;
extern u8 lcd_buffer;
/*
        Communication with LCD/KBD board,
        Incoming command format:
                CMDI_CLEARBUF   reset the LCD buffer count. (like the ack of the command)
                key:            key pressed
        Out command format:
                CMDO_DISPLAYBTN,LED_STATUS      update the led status
*/
/**************************主程序********************************/
void Key_Init()
{
	key = KEY_INVALID;
}

#include <string.h>
#include "lcd.h"
#include "font.h"
#include "global.h"
#include "utili.h"



extern uchar key;
static uchar kbd_state;
void kbd_uart_push(uchar data)
{
        if(data == STATUS_IDLE||
           data == STATUS_ERR||
           data == STATUS_DRAW||
           data == STATUS_DONE) //lcd acknowledge
        {
                kbd_state = data;
                return;
        }
        if(key == KEY_INVALID)
                key = data;
}
static u8 lcdcmd[40];

static u8 head[3] = {MARK_HEAD,MARK_HEAD,MARK_HEAD};
static u8 tail[3] = {MARK_TAIL,MARK_TAIL,MARK_TAIL};

static u8 hex2ch[2];
void hex2char(uchar a)
{
        uchar h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)
		hex2ch[0] = (h+'0');
	else
		hex2ch[0] = (h+'A'-0x0a);
	if(l <= 9)
		hex2ch[1] = (l+'0');
	else
		hex2ch[1] = (l+'A'-0x0a);
}
void onecmd(uchar len)
{
     uchar i = 0;
     uchar sum;
     sum = 0;
     prints(head,3,PORT_KBD); //send head
     for(i = 0;i<len;i++)     //caculate cksum
     {
        hex2char(lcdcmd[i]);
	prints(hex2ch,2,PORT_KBD);
        sum = sum + lcdcmd[i];
     }
     lcdcmd[i] = 0xff - sum;
     hex2char(lcdcmd[i]);
     prints(hex2ch,2,PORT_KBD);
     prints(tail,3,PORT_KBD); //send tail
}
static u8 idle[10] = {MARK_HEAD,MARK_HEAD,MARK_HEAD,'1','2','E','D',MARK_TAIL,MARK_TAIL,MARK_TAIL};
void sendcmd(uchar len)
{
     ulong timeout;
     if(DEBUG == 1)
        return;
     while(kbd_state != STATUS_IDLE)
     {
        if(kbd_state == STATUS_DRAW)
                continue;
        if(kbd_state == STATUS_ERR)
        {
                prints(idle,10,PORT_KBD);
                kbd_state = STATUS_DRAW;
        }
     }
     onecmd(len);
     timeout = 0;
     kbd_state = STATUS_DONE;
     while(kbd_state != STATUS_IDLE)
     {
         if(kbd_state == STATUS_ERR)
         {
                onecmd(len);
                timeout = 0;
         }
         if(timeout < 655350)
         {
                sleepms(1);
                timeout++;
         }else{
                onecmd(len);
                timeout = 0;
         }
     }
}
/*------------------初始化-----------------*/
void LCD_Init (void)
{
        kbd_state = STATUS_IDLE;
        lcdcmd[0] = CMDO_LCD_INIT;
        sendcmd(1);
        //do it twice avoid the missing of first command on boot up
        lcdcmd[0] = CMDO_LCD_INIT;
        sendcmd(1);

}

/********************************************************/
/* 清屏							*/
/********************************************************/
void LCD_Cls(void)
{
        lcdcmd[0] = CMDO_LCD_CLS;
        sendcmd(1);
}
void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2)
{
        lcdcmd[0] = CMDO_LCD_CLSBLK;
        lcdcmd[1] = x1;
        lcdcmd[2] = y1;
        lcdcmd[3] = x2;
        lcdcmd[4] = y2;
        sendcmd(5);
}
void display_buttons(uchar btn_pos,uchar btn_val)   //send led status to sub board
{
        lcdcmd[0] = CMDO_DISPLAYBTN;
        if(btn_val == 0)
	      btn_pos += 4; //change from 'a'-'d' to 'e'-'h'   //add 4
        lcdcmd[1] = btn_pos;
        sendcmd(2);
}


/*	设定图形x,y值*/
/*
void LCD_GraphSetAddr(uchar x,uchar y)
{
	uint xy;
	xy=y;
	xy=xy*16+x+256;
  	//LCD_CE(0);
	LCD_Write2(xy&0xff,xy/256,0x24);
}
*/
/*	设定文本x,y值	*/
/*
void LCD_TextSetAddr(uchar x,uchar y)
{
  	//LCD_CE(0);
	LCD_Write2(y*16+x,0,0x24);
}
*/
/*	清除一点*/
/*
void LCD_ClrPixel(uchar x,uchar y)
{
	uchar b;
	uint addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;

	b = 7 - (x % 8);

	LCD_WriteInt(addr,0x24);
	LCD_Comm(0xf0|b);
}
*/
/*	点亮一点	*/
/*
void LCD_Putpixel(uchar x,uchar y)
{
	uchar b;
	uint addr = LCD_GRAPH_HOME_ADDR + ((uint)y * LCD_WIDTH) + x>>3;

	b = 7 - (x % 8);

	LCD_WriteInt(addr,0x24);
	LCD_Comm(0xf8|b);
}
*/
/*	x,y处显示光标	*/
/*
void LCD_ShowCursor(uchar x,uchar y)
{
	return;
  	//LCD_CE(0);
	LCD_Comm(0x97);	//光标开
	LCD_Write2(x,y,0x21);
}

//	取消光标

void LCD_HideCursor(void)
{
	return;
  	//LCD_CE(0);
	LCD_Comm(0x9c);
}
*/
/*
void LCD_PrintNumStr(uchar x,uchar y,uchar *s)
{
	x =(x / 8) * 8;

	while(*s)
	{
		LCD_PutImg(x,y,1,11,Num_Tab + (*s - '0') * 11);

		x = x + 8;

		s++;
	}
}


void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty)
{
	x =(x / 8) * 8;
	LCD_PutImg(x,y,1,11,BlockTab + (not_empty?0:1) * 11);
}
*/

void LCD_ReverseRect(uchar x,uchar y,uchar w,uchar h)
{
        lcdcmd[0] = CMDO_LCD_REVERSE;
        lcdcmd[1] = x;
        lcdcmd[2] = y;
        lcdcmd[3] = w;
        lcdcmd[4] = h;
        sendcmd(5);
}
/*--------------显示字符------------------*/
/*
void LCD_TextPutchar(uchar x,uchar y,uchar c)
{
  	//LCD_CE(0);
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
  	LCD_Comm(0xb0);
	LCD_AutoWrite(c - 0x20);
  	LCD_Comm(0xb2);
}

void LCD_TextPrint(uchar x,uchar y,char *s)
{
  	//LCD_CE(0);
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
  	LCD_Comm(0xb0);
  	while(*s)
  	{
		LCD_AutoWrite(*s - 0x20);
		s++;
	}
  	LCD_Comm(0xb2);
}
void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start)
{
	uchar buf[4];
	int i;

	if(start > 3) start = 0;

	for(i = 0; i < 4; i++)
	{
		buf[i] = n % 10;
		n /= 10;
	}

	for(i = 3-start; i >= 0; i--)
	{
		LCD_TextPutchar(x,y,'0' + buf[i]);
		x ++;
	}
}

void LCD_TextPrintHex(uchar x,uchar y,uchar hex)
{
	uchar c1,c2;
	//低4位
	c1 = hex & 0x0f;	//lo
	//高4位
	c2 = (hex >> 4) & 0x0f; //hi

	LCD_TextPutchar(x,y,HexTable[c2]);
	LCD_TextPutchar(x+1,y,HexTable[c1]);
}
*/
/************************************************/
/*画线。任意方向的斜线,直线数学方程 aX+bY=1	*/
/************************************************/
/*
void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt)
{
	register uchar t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	delta_x = xt-x0;				//计算坐标增量
	delta_y = yt-y0;
	uRow = x0;
	uCol = y0;
	if(delta_x>0) incx=1;				//设置单步方向
	else if( delta_x==0 ) incx=0;			//垂直线
	else {incx=-1;delta_x=-delta_x;}

	if(delta_y>0) incy=1;
	else if( delta_y==0 ) incy=0;			//水平线
	else {incy=-1;delta_y=-delta_y;}

	if( delta_x > delta_y )	distance=delta_x;	//选取基本增量坐标轴
	else distance=delta_y;

  	//LCD_CE(0);
	for( t=0;t <= distance+1; t++ )
        {					//画线输出
		LCD_Putpixel(uRow,uCol);			//画点
		xerr +=	delta_x	;
		yerr +=	delta_y	;

		if( xerr > distance )
               	{
			xerr-=distance;
			uRow+=incx;
		}
		if( yerr > distance )
               	{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
*/
/*
void LCD_LineH(uchar y)
{
	char i;

  	//LCD_CE(0);
  	LCD_WriteInt(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y ,0x24);
	LCD_Comm(0xb0);
	for(i=0;i<LCD_WIDTH;i++)
 		LCD_AutoWrite(0xff);
	LCD_Comm(0xb2);
}
*/
/*
void LCD_LineV(uchar x,uchar y1,uchar y2)
{
	int i;
  	//LCD_CE(0);
	for(i = y1; i < y2; i++)
	{
		LCD_Putpixel(x,i);
	}
}
*/

void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2)
{
        lcdcmd[0] = CMDO_LCD_RECT;
        lcdcmd[1] = x1;
        lcdcmd[2] = y1;
        lcdcmd[3] = x2;
        lcdcmd[4] = y2;
        sendcmd(5);
}
void LCD_PrintChar(uchar cmd, uchar x,uchar y,uchar *s)
{
        uchar pos = 0;
        lcdcmd[0] = cmd;
        lcdcmd[1] = x;
        lcdcmd[2] = y;
        pos = 1;
        while(*s)
        {
                lcdcmd[(uchar)(pos + 3)] = *s++;
                pos = pos + 1;
        }
        lcdcmd[(uchar)(pos+3)] = 0x00;
        lcdcmd[3] = pos;
        pos = pos + 4;
        sendcmd(pos);
}
void LCD_PrintHz12(uchar x,uchar y,uchar *s)
{
        LCD_PrintChar(CMDO_LCD_HZ12,x,y,s);
}
void LCD_PrintHz16(uchar x,uchar y,uchar *s)
{
        LCD_PrintChar(CMDO_LCD_HZ16,x,y,s);
}
//显示6x8的数字
void LCD_Print6X8(uchar x, uchar y,uchar *s)
{
        LCD_PrintChar(CMDO_LCD_68,x,y,s);
}

//显示6x8的数字
void LCD_Print8X16(uchar x, uchar y,uchar *s)
{
        LCD_PrintChar(CMDO_LCD_816,x,y,s);
}

//显示24x32的数字
/*
void LCD_Print24X32(uchar x, uchar y,uchar *s)
{
	//x坐标必需是8位对齐
	x =(x / 8) * 8;
	while(*s)
	{
		if( *s >= '0' && *s <= '9')	//显示
		{
			LCD_PutImg(x,y,3,32,Font24X32[*s-'0']);
			x += 24;
		}
		else if( *s == ' ')
		{
			x += 8;
		}
		else if( *s == '-')
		{
			LCD_PutImg(x,y,3,32,Font24X32[12]);
			x += 24;
		}
		else
		{
			LCD_PutImg(x,y+16,1,16,ASC8x16[*s]);	//
			x += 8;
		}
		s ++;
	}
}*/
#include "utili.h"
#include "scanner.h"
#include "stdlib.h"

static uchar navlen = 0;
static double reading = -1000;
static char navread[20];
static char navread2[20];
u8 scancmd[5];
u8 eeprom scanner_type = 1; //1: MI, 2: GUIDLINE
void scanner_set_mode()
{
        if (IS_BORE_MODE)
        {
            scancmd[0] = '#';
        }else{
            scancmd[0]= '!';
        }
        scancmd[1] = 0x0D;
        scancmd[2] = 0x0A;
        prints(scancmd,3,PORT_SCANNER);
}
void scanner_set_channel(uchar ch)
{
        if(ch == 1)
        {
                SET_TOCH1;
        }
        if(ch == 2)
        {
                SET_TOCH2;
                return;
        }
        if(scanner_type == 1) //MI
        {
                if(ch < 10)
                {
                        scancmd[0] = (ch + '0');
                        scancmd[1] = 'A';
                        scancmd[2] = 0x0D;
                        scancmd[3] = 0x0A;
                        prints(scancmd,4,PORT_SCANNER);
                        return;
                }else{
                        scancmd[0] = (u8)(ch / 10) + '0';
                        ch = ch % 10;
                        scancmd[1] = ch + '0';
                        scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
                        prints(scancmd,5,PORT_SCANNER);
                }
        }
        if(scanner_type == 2) //guidline
        {
                        scancmd[0] = 'A';
                        scancmd[1] = (u8)(ch / 10) + '0';
                        ch = ch % 10;
                        scancmd[2] = ch + '0';
                        scancmd[3] = 0x0D; scancmd[4] = 0x0A;
                        prints(scancmd,5,PORT_SCANNER);
        }
}


//incoming data handler of scanner
void scanner_uart_push(uchar data)
{
}
//incoming data hander of PC
void pc_uart_push(uchar data)
{
}
static uchar navcmd[12];
#define NAV_INVALID     -1000
#define NAV_VALID       -2000
#define RESETNAV       navlen = 0;   reading = NAV_INVALID;

void nav_command(uchar cmd)
{
        if(cmd == NAV_30V)
               sprintf(navcmd,"%%01;12;02\r");
        if(cmd == NAV_1V)
               sprintf(navcmd,"%%01;12;01\r");
        if(cmd == NAV_120MV)
               sprintf(navcmd,"%%01;12;00\r");
        if(cmd == NAV_AFLTOFF)
               sprintf(navcmd,"%%01;26\r");
        if(cmd == NAV_AFLTON)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == NAV_SLOWMODE)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == NAV_ZEROON)
        {
               sprintf(navcmd,"%%01;06\r");
        }
        if(cmd == NAV_INIT)
        {
                RESETNAV;
                sprintf(navcmd,"%%01;00\r");
        }
        if(cmd == NAV_READ)
        {
                RESETNAV;
                sprintf(navcmd,"%%01;01\r");
        }
//                com1_putc('?');
        prints(navcmd,strlen(navcmd),PORT_NAV);
}


unsigned long navtime;
double nav_read()
{
        if(DEBUG == 1)
                return 1.0;
        nav_command(NAV_READ);
        navtime = 0;
        while(1)
        {
                if(reading < NAV_INVALID) //make sure it is a valid reading
                {
                     reading = atof(navread2);
                     return reading;
                }
                if(navtime++ > 655350)
                {
                     nav_command(NAV_READ);
                     navtime = 0;
                }
                sleepms(1);
        }
        return 1.0;
}
//incoming data hander of navameter
void nav_uart_push(uchar data)
{
        if(navlen >= 19)
        {
                RESETNAV;
        }
        if(((data >= '0') && (data <= '9')) ||
                (data == '.') ||
                (data == '-') ||
                (data == '+') ||
                (data == 'e') ||
                (data == 'E') )
        {
                navread[navlen++] = data;
                reading = NAV_INVALID;
                return;
        }

        if(navlen < 4) //not enough digits
        {
                RESETNAV;
                return;
        }
        navread[navlen] = '\0';
        do
        {
                navread2[navlen] = navread[navlen];
        }while(navlen-- > 0);
        navlen = 0;
        reading =  NAV_VALID; //valid data
}
#include <mega128.h>
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "window.h"
#include "utili.h"
#include "scanner.h"

extern uchar nextwin;                                         //draw_label(&PRE##chvalue,SW_NORMAL);
#define REFRESH_TABLES1(PRE)	sprintf(strbuf,"%i",curr_pos);\
                		if(new_page == 1){\
		                	for(i = min_option; i <= max_option; i++){\
				                j = ((curr_pos-1)/max_option)*max_option + i;

#define REFRESH_TABLES2(PRE)	        draw_label(&(PRE##items[i-1]),SW_NORMAL);\
				        if((curr_pos-1) % max_option == (i-1))\
					        draw_label(&(PRE##items[i-1]),SW_REVERSE);\
        			        }\
                                }else{\
	                		for(i = min_option; i <= max_option; i++) {\
        		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
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

#define KEY_TABLE	if((msg > KEY_NUM0) && (uchar)((msg-KEY_NUM0)*max_option) <= max_index)\
					{new_page = 1; curr_pos = (uchar)((msg-KEY_NUM1)*max_option)+1;}

#define INIT_OPTIONS(PRE)	for(i = min_option;i <= max_option; i++) {\
                                        draw_label(&PRE##options[i-1], SW_NORMAL);\
			        }
#define REFRESH_OPTIONS(PRE)	if(last_sel == curr_sel)\
                                        return;\
        			for(i = min_option;i <= max_option; i++){\
	        			if(last_sel == i)       draw_label(&PRE##options[i-1], SW_REVERSE);\
        				if(curr_sel == i)	draw_label(&PRE##options[i-1], SW_REVERSE);\
        			}

#define INC_OPTION	last_sel = curr_sel;\
			curr_sel++;\
			if(curr_sel > max_option)\
				curr_sel = min_option;

#define DEC_OPTION	last_sel = curr_sel;\
			curr_sel--;\
			if(curr_sel < min_option)\
				curr_sel = max_option;

extern int   curr_ch;	//index of current channel in configuation   menu
extern int   curr_prb;	//index of current probe selection
extern int   curr_dispch; //start of current channel for display in main menu

void State_Init() {
	display_buttons(KEY_BTN1,sysdata.prbmode);
	display_buttons(KEY_BTN2,sysdata.kttmode);
	display_buttons(KEY_BTN3,0); //probe type
	display_buttons(KEY_BTN4,0); //zero
}
char* getprbtype(uchar);
LABEL flash pgmain_chs[] = {
		{LBL_HZ6X8,3, 3,5,strbuf},
		{LBL_HZ6X8,3,16,5,strbuf},
		{LBL_HZ6X8,3,29,5,strbuf},
		{LBL_HZ6X8,3,42,5,strbuf}
	};
LABEL flash pgmain_values[] = {
		{LBL_HZ6X8,40,3,10,strbuf},
		{LBL_HZ6X8,40,16,10,strbuf},
		{LBL_HZ6X8,40,29,10,strbuf},
		{LBL_HZ6X8,40,42,10,strbuf}
	};
LABEL flash pgmain_temps[] = {
		{LBL_HZ6X8,140,3, 10,strbuf},
		{LBL_HZ6X8,140,16,10,strbuf},
		{LBL_HZ6X8,140,29,10,strbuf},
		{LBL_HZ6X8,140,42,10,strbuf}
};
/*      Main display      */

void pgmain_handler(uchar msg) {
        int max_option = 4;
        uchar i,old_ch,isinit;
                uchar *cptr;
	//LABEL flash usage = {LBL_HZ6X8,210,60,5,"usage"};

	isinit = 0;
        //Menu
        if(msg == KEY_CE) {
                nextwin = PG_BOOTTYPE;
                return;
        }
	if(msg == KEY_TAB) {
		if(IS_THERM_MODE)
			nextwin = PG_TCONFIG;
		else
			nextwin = PG_RCONFIG;
		return;
	}
        if(key == KEY_NUM1) //R0
        {
/*
                window_setup(10);
                if(IS_BORE_MODE)
                {
                        sprintf(strbuf,"请输入铂电阻R0阻值");
	        	sysdata.R0 = wnd_floatinput(sysdata.R0);
		        msg = MSG_INIT;
		}else{
		        return;
                        sprintf(strbuf,"请输入V0值");
	        	sysdata.V0 = wnd_floatinput(sysdata.V0);
                        msg = MSG_INIT;
		}
*/
        }
        if(key == KEY_NUM2) //Rs1
        {
/*
                if(IS_BORE_MODE)
                {
                        window_setup(10);
                        sprintf(strbuf,"请输入PT100内标准阻值");
		        sysdata.Rs1 = wnd_floatinput(sysdata.Rs1);
        		msg = MSG_INIT;
                }
*/
        }
        if(key == KEY_NUM3) //Rs2
        {
/*
                if(IS_BORE_MODE)
                {
                        window_setup(10);
                        sprintf(strbuf,"请输入PT1000内标准阻值");
		        sysdata.Rs2 = wnd_floatinput(sysdata.Rs2);
        		msg = MSG_INIT;
                }
*/
        }

	if(msg == KEY_UP) {
		DEC_DISPCH;
		msg = MSG_INIT;
	}
	if(msg == KEY_DN) {
		INC_DISPCH;
		msg = MSG_INIT;
	}

	if(msg == MSG_INIT) {
	        isinit = 1;
		LCD_Cls();
		old_ch = curr_dispch;

	        curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
		{
		        if(curr_dispch == old_ch)
		        {
        			sprintf(strbuf,"CH%02i:",curr_dispch);
                                draw_label(&pgmain_chs[i],SW_NORMAL);
                                draw_label(&pgmain_chs[i],SW_REVERSE);
        		}
        		else{
        			sprintf(strbuf,"CH%02i:",curr_dispch);
        			draw_label(&pgmain_chs[i],SW_NORMAL);
        		}
			INC_DISPCH;
		}
		curr_dispch = old_ch;
		msg = MSG_REFRESH;
	}

	if(msg == MSG_REFRESH) {
		old_ch = curr_dispch;
		curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
		{
		        if(IS_THERM_MODE)
		        {
			        cptr = getprbtype(tprbdata.type[sysdata.tid[curr_dispch-1]]);
        			if((sysdata.tid[curr_dispch - 1] == INVALID_PROBE) ||\
	        			(cptr[0] == 'P')){
               				sprintf(strbuf," -------  ");
                                        if((curr_dispch-1 == ch_to_search) || (isinit == 1))
                                        {
                        			draw_label(&pgmain_values[i],SW_NORMAL);
		                		draw_label(&pgmain_temps[i],SW_NORMAL);
		                        }else{
                        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
		                        }
			        }else{
					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
					if(curr_dispch-1 == ch_to_search)
        					sprintf(strbuf+8,"*mV");
                                        else
                                                sprintf(strbuf+8," mV");
                                        if((curr_dispch-1 == ch_to_search) || (isinit == 1))
                                        {
        					draw_label(&pgmain_values[i],SW_NORMAL);
        					if(rundata.temperature[curr_dispch - 1] < -9000)
        					        sprintf(strbuf," OVER    ");
        					else
                				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);

                				sprintf(strbuf+8," ! %c",cptr[0]);
                			        draw_label(&pgmain_temps[i],SW_NORMAL);
                                        }else{
                                                draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
                				if(rundata.temperature[curr_dispch - 1] < -9000)
                				{
                				        sprintf(strbuf," OVER    ");
                				}else{
                				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
                				}
        				        sprintf(strbuf+8," ! %c",cptr[0]);
	        			        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
                                        }
				        if(pgmain_temps[i].type == LBL_HZ6X8)
        				        LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12, pgmain_temps[i].y -2 +10);
        				else                                     //Hz12
                				LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12,pgmain_temps[i].y -2 + 16);
	                        }
			}else{
       			        cptr = getprbtype(rprbdata.type[sysdata.rid[curr_dispch-1]]);
        			if((sysdata.rid[curr_dispch - 1] == INVALID_PROBE) ||\
        				(rundata.temperature[curr_dispch - 1] < -9000) ||\
	        			(cptr[0] != 'P'))
	        		{
	        		        if(sysdata.rid[curr_dispch - 1] == INVALID_PROBE)
                				sprintf(strbuf," ------- ohm");
                			if(rundata.temperature[curr_dispch - 1] < -9000)
                        			sprintf(strbuf," NO READ    ");
                                        else
                                        	sprintf(strbuf," -------    ");
                                        if((curr_dispch-1 == ch_to_search) || (isinit == 1))
                                        {
        	        			draw_label(&pgmain_values[i],SW_NORMAL);
                				sprintf(strbuf," -------  ");
		                		draw_label(&pgmain_temps[i],SW_NORMAL);
		                	}else{
        	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
                				sprintf(strbuf," -------  ");
		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
		                	}
			        }else{
					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
					if(curr_dispch-1 == ch_to_search)
        					sprintf(strbuf+8,"*ohm");
                                        else
        					sprintf(strbuf+8," ohm");
                                        if((curr_dispch-1 == ch_to_search) || (isinit == 1))
                                        {
	        			        draw_label(&pgmain_values[i],SW_NORMAL);
        		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
		        		        draw_label(&pgmain_temps[i],SW_NORMAL);
		        		 }else{
	        			        draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
        		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
		        		        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
		        		 }
				        if(pgmain_temps[i].type == LBL_HZ6X8)
        				        LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 +10);
        				else                                     //Hz12
                				LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 + 16);
        			}
	                }
			INC_DISPCH;
		}
		curr_dispch = old_ch;
	}
}

//main configuration window of R
LABEL flash pgr_banner = {LBL_HZ16,3,3,7,"铂电阻参数配置"};
LABEL flash pgr_calibrate = {LBL_HZ16,30,30,8,"内标准校准中..."};
LABEL flash pgr_klbl = {LBL_HZ6X8, 88,28, 3,strbuf}; //ktime label
LABEL flash pgr_options[] = {
	{LBL_HZ16, 10,23,7,"1.电流换向"},
	{LBL_HZ16,130,23,7,"2.内标准校准"},
	{LBL_HZ16, 10,45,7,"3.通道探头选择"},
	{LBL_HZ16,130,45,7,"4.设置探头参数"}
};

//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
#define SLEEPKTIME      cnt = sysdata.ktime * ONESEC; while(cnt > 0) cnt--;
void pgcalibrate()
{
        double oldvalue = 0;
        window_setup(10);
	sprintf(strbuf,"请输入外标准(CH1)阻值");
	oldvalue = wnd_floatinput(oldvalue);
	if(oldvalue == 0)
                return;
        wnd_msgbox(&pgr_calibrate);
        if(rprbdata.type[sysdata.rid[curr_dispch-1]] == PRBTYPE_PT1000)
                sysdata.Rs2 = (oldvalue + sysdata.R0)*sysdata.Rs2/(rundata.reading[curr_dispch-1]+sysdata.R0);
        else
                sysdata.Rs1 = (oldvalue + sysdata.R0)*sysdata.Rs1/(rundata.reading[curr_dispch-1]+sysdata.R0);

        if((sysdata.Rs1 > 101) || (sysdata.Rs1 < 99))
        {
                sysdata.Rs1 = 100;
        }
        if((sysdata.Rs2 > 1010) || (sysdata.Rs2 < 990))
        {
                sysdata.Rs2 = 1000;
        }
}
//main menu of bore config
void pgrconfig_handler(uchar msg) {
	uchar i;

	static uchar curr_sel = 1;
	static uchar last_sel = 0xff;
	uchar min_option = 1;
	uchar max_option = sizeof(pgr_options)/sizeof(LABEL);

	if(msg == KEY_TAB) {
	        SET_BORE_MODE;
	        SET_TORS;
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_CE)
	{
	        nextwin = PG_BOOTTYPE;
	        return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		last_sel = 0xff;
		draw_label(&pgr_banner, SW_NORMAL);
		INIT_OPTIONS(pgr_);
		sprintf(strbuf,"(%i)",sysdata.ktime);
		draw_label(&pgr_klbl, SW_NORMAL);
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN)
	{
	        last_sel = curr_sel;
	        curr_sel++;
	        if(curr_sel > max_option)
	                curr_sel = 1;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_UP)
	{
	        last_sel = curr_sel;
	        curr_sel--;
	        if(curr_sel == 0)
	                curr_sel = max_option;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_OK)
	{
	        msg = curr_sel + KEY_NUM0;
	}
	if(msg == KEY_NUM1) {
		window_setup(4); //4 char at max
		sprintf(strbuf,"电流换向时间设置");
		sysdata.ktime = wnd_intinput(sysdata.ktime);
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM2)
	{
                pgcalibrate();
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
		REFRESH_OPTIONS(pgr_);
	}
}
//main configuration window of T
LABEL flash pgt_banner = {LBL_HZ16,3,3,7,"热电偶参数配置"};
LABEL flash pgt_options[] = {
	{LBL_HZ16,40,25,8,"1.选择各通道探头"},
	{LBL_HZ16,40,45,8,"2.设置各探头参数"}
};

//main menu of thermo config
void pgtconfig_handler(uchar msg) {
	uchar i;
	static uchar curr_sel = 1;
	static uchar last_sel = 0xff;
	uchar min_option = 1;
	uchar max_option = sizeof(pgt_options)/sizeof(LABEL);
	if(msg == KEY_TAB) {
	        SET_THERM_MODE;
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_CE)
	{
	        nextwin = PG_BOOTTYPE;
	        return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		last_sel = 0xff;
		draw_label(&pgt_banner, SW_NORMAL);
		INIT_OPTIONS(pgt_);
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN)
	{
	        last_sel = curr_sel;
	        curr_sel++;
	        if(curr_sel > max_option)
	                curr_sel = 1;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_UP)
	{
	        last_sel = curr_sel;
	        curr_sel--;
	        if(curr_sel == 0)
	                curr_sel = max_option;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_OK)
	{
	        msg = curr_sel + KEY_NUM0;
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
		REFRESH_OPTIONS(pgt_);
	}
	return;
}
//channel probe setup
LABEL flash pgch_banner =    {LBL_HZ16,3,3,6,"选择要配置的通道"};
LABEL flash pgch_chvalue = {LBL_HZ6X8,130,3,2,strbuf};
LABEL flash pgch_items[] = {
	{LBL_HZ6X8, 10, 30, 10, strbuf},
	{LBL_HZ6X8, 130,30, 10, strbuf},
	{LBL_HZ6X8, 10, 40, 10, strbuf},
	{LBL_HZ6X8, 130,40, 10, strbuf},
	{LBL_HZ6X8, 10, 50, 10, strbuf},
	{LBL_HZ6X8, 130,50, 10, strbuf}
};

//select the channel to config
void pgchset_handler(uchar msg) {
	static uint curr_pos = 1; //absolute postion in options 1-24
	static uint last_pos = 1;
	uchar min_option = 1;
	uchar max_option = sizeof(pgch_items)/sizeof(LABEL);
	uchar min_index = 1;
	uchar max_index = MAX_CH_NUM;//rundata.scanmode;
	uchar new_page = 0;
	int i,j;
	if(msg == KEY_CE) {
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
	/*
		KEY_TABLE;
		msg = MSG_REFRESH;
	*/
	}
	if(msg == KEY_DOT)
	{
	        if(IS_THERM_MODE)
	        {
		        sysdata.tid[curr_pos - 1] = INVALID_PROBE;
	        }else{
       		        sysdata.rid[curr_pos - 1] = INVALID_PROBE;
	        }
		msg = MSG_REFRESH;
		new_page = 1; //refresh the whole page
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		draw_label(&pgch_banner, SW_NORMAL);
		curr_pos = 1;
		new_page = 1;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {

		REFRESH_TABLES1(pgch_);
		        if(IS_THERM_MODE)
        	        {
				if(sysdata.tid[j-1] == INVALID_PROBE){
					sprintf(strbuf,"CH%i:*",j);
				}else{
			        	//draw line item j
				        sprintf(strbuf,"CH%i:%s",j,tname2b((sysdata.tid[j-1])));
				}
		        }else{
				if(sysdata.rid[j-1] == INVALID_PROBE){
					sprintf(strbuf,"CH%i:*",j);
				}else{
			        	//draw line item j
				        sprintf(strbuf,"CH%i:%s",j,rname2b((sysdata.rid[j-1])));
				}
		        }
		REFRESH_TABLES2(pgch_);
	}
	return;
}
//select probe to edit.
LABEL flash pgprbset_banner = {LBL_HZ16,3,3,8,"请选择要配置的探头"};
LABEL flash pgprbset_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
LABEL flash pgprbset_items[] = {
	{LBL_HZ6X8, 10, 30, 10, strbuf},
	{LBL_HZ6X8, 130,30, 10, strbuf},
	{LBL_HZ6X8, 10, 40, 10, strbuf},
	{LBL_HZ6X8, 130,40, 10, strbuf},
	{LBL_HZ6X8, 10, 50, 10, strbuf},
	{LBL_HZ6X8, 130,50, 10, strbuf}
};
//select probe to config
void pgprbset_handler(uchar msg) {
	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
	static uint curr_pos = 1;
	static uint last_pos = 1;
	uchar min_option = 1;
	uchar max_option = sizeof(pgprbset_items)/sizeof(LABEL);
	uchar min_index = 1;
	uchar max_index = MAX_PROBE_NUM;
	int i,j;
	uchar new_page = 1;
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	/*
		KEY_TABLE;
		msg = MSG_REFRESH;
	*/
	}

	if(msg == KEY_CE)
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
		draw_label(&pgprbset_banner, SW_NORMAL);
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
		REFRESH_TABLES1(pgprbset_);
		        if(IS_THERM_MODE)
		        {
			        sprintf(strbuf,"%i.%s",j,tname2b(j-1));
		        }else{
        		        sprintf(strbuf,"%i.%s",j,rname2b(j-1));
		        }
        		draw_label(&(pgprbset_items[i-1]),SW_NORMAL);
		REFRESH_TABLES2(pgprbset_);
	}
	return;
}
//select probe for channel setup
LABEL flash prblist_banner = {LBL_HZ16,3,3,8,strbuf};
LABEL flash prblist_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
LABEL flash prblist_items[] = {
	{LBL_HZ6X8, 10, 30, 10, strbuf},
	{LBL_HZ6X8, 130,30, 10, strbuf},
	{LBL_HZ6X8, 10, 40, 10, strbuf},
	{LBL_HZ6X8, 130,40, 10, strbuf},
	{LBL_HZ6X8, 10, 50, 10, strbuf},
	{LBL_HZ6X8, 130,50, 10, strbuf}
};
//select channel's probe
void pgprblist_handler(uchar msg) {
	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
	static uint curr_pos = 1;
	static uint last_pos = 1;
	uchar min_option = 1;
	uchar max_option = sizeof(prblist_items)/sizeof(LABEL);
	uchar min_index = 1;
	uchar max_index = MAX_PROBE_NUM;
	uchar i,j;
	uchar new_page = 1;
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	/*
		KEY_TABLE;
		msg = MSG_REFRESH;
	*/
	}
	if(msg == KEY_CE)
	{
		nextwin = PG_CHSET;
		return;
	}
	if(msg == KEY_OK)
	{
	        if(IS_THERM_MODE)
	        {
       	                if(tprbdata.name[curr_pos-1][0] != '\0')
        		        sysdata.tid[curr_ch-1] = curr_pos - 1;
	        }else{
                        if(rprbdata.name[curr_pos-1][0] != '\0')
		                sysdata.rid[curr_ch-1] = curr_pos - 1;
	        }
		nextwin = PG_CHSET;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
		draw_label(&prblist_banner, SW_NORMAL);
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
	if(msg == KEY_DOT){
	        if(IS_THERM_MODE)
	        {
		        sysdata.tid[curr_ch-1] = INVALID_PROBE;
	        }else{
		        sysdata.rid[curr_ch-1] = INVALID_PROBE;
	        }
       		msg = MSG_REFRESH;
       		new_page = 1;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_TABLES1(prblist_);
		        if(IS_THERM_MODE)
		        {
		                sprintf(strbuf,"%s",tname2b(j-1));
		        }else{
        		        sprintf(strbuf,"%s",rname2b(j-1));
        		}
			draw_label(&(prblist_items[i-1]),SW_NORMAL);
		REFRESH_TABLES2(prblist_);
	}
	return;
}
//select probe type list
LABEL flash tplist_banner = {LBL_HZ16,3,3,8,strbuf};
LABEL flash tplist_options[] = {
	{LBL_HZ6X8,10,20,8,"0.PT1000"},
	{LBL_HZ6X8,80,20,8,"1.PT100"},
	{LBL_HZ6X8,150,20,8,"2.PT25"},
	{LBL_HZ6X8,10,30,8,"3.K-TYPE"},
	{LBL_HZ6X8,80,30,8,"4.N-TYPE"},
	{LBL_HZ6X8,150,30,8,"5.E-TYPE"},
	{LBL_HZ6X8,10,40,8,"6.B-TYPE"},
	{LBL_HZ6X8,80,40,8,"7.J-TYPE"},
	{LBL_HZ6X8,150,40,8,"8.S-TYPE"},
        {LBL_HZ6X8,10,50,8,"9.T-TYPE"},
	{LBL_HZ6X8,10,50,8,"*.R-TYPE"}

};
//select probe type thermo
void pgprbtypelist_handler(uchar msg) {
	static int curr_sel = 1;
	static int last_sel = 0xff;
	uchar min_option = 1;
	uchar max_option = sizeof(tplist_options)/sizeof(LABEL);
	uchar i;
	if(msg == KEY_DOT)
	{
	        curr_sel = 10;
	        msg = KEY_OK;
	}
	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
	        curr_sel = (u8)(msg - KEY_NUM1 + 1) ;
	        msg = KEY_OK;
	}
        if (msg == KEY_NUM0)
        {
                curr_sel = 0;
                msg = KEY_OK;
        }
	if(msg == KEY_CE ) {
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == KEY_OK)
	{
	        if(IS_THERM_MODE)
	        {
        		switch(curr_sel)
	        	{
	        	        case 0:
	        	               return;
		        	case 1:
        			       return;
	        		case 2:
			               return;
        			default:
	        		       tprbdata.type[curr_prb-1] = curr_sel - 3+PRBTYPE_K;
		        	       break;
        		}
        	}else{
        		switch(curr_sel)
	        	{
	        	        case 0:
	        	               rprbdata.type[curr_prb-1] = PRBTYPE_PT1000;
	        	               break;
		        	case 1:
			               rprbdata.type[curr_prb-1] = PRBTYPE_PT100;
        			       break;
	        		case 2:
                       	               sprintf(strbuf,"输入Rtp:");
        	                       rprbdata.rtp[curr_prb-1] = wnd_floatinput(rprbdata.rtp[curr_prb-1]);
		        	       rprbdata.type[curr_prb-1] = PRBTYPE_PT25;
			               break;
        			default:
                                       return;
        		}
        	}
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
		draw_label(&tplist_banner, SW_NORMAL);
		INIT_OPTIONS(tplist_);
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
		REFRESH_OPTIONS(tplist_);
	}
	return;
}
/*
 *       Convert the probe type const to string
 */
char PRBSTR[7];
char* getprbtype(uchar prbtype)
{
	switch(prbtype)
	{
	        case PRBTYPE_PT1000:
		        sprintf(PRBSTR,"PT1000");
		        return PRBSTR;
		case PRBTYPE_PT25:
		        sprintf(PRBSTR,"PT  25");
		        return PRBSTR;
		case PRBTYPE_PT100:
		        sprintf(PRBSTR,"PT 100");
		        return PRBSTR;
		case PRBTYPE_T:
		        sprintf(PRBSTR,"T TYPE");
		        return PRBSTR;

		case PRBTYPE_K:
		        sprintf(PRBSTR,"K TYPE");
		        return PRBSTR;
		case PRBTYPE_N:
		        sprintf(PRBSTR,"N TYPE");
		        return PRBSTR;
		case PRBTYPE_E:
		        sprintf(PRBSTR,"E TYPE");
		        return PRBSTR;
		case PRBTYPE_B:
		        sprintf(PRBSTR,"B TYPE");
		        return PRBSTR;
		case PRBTYPE_J:
		        sprintf(PRBSTR,"J TYPE");
		        return PRBSTR;
		case PRBTYPE_S:
		        sprintf(PRBSTR,"S TYPE");
		        return PRBSTR;
		case PRBTYPE_R:
		        sprintf(PRBSTR,"R TYPE");
		        return PRBSTR;

	}

		        sprintf(PRBSTR,"? TYPE");
		        return PRBSTR;

}
//config r probe parameter
LABEL flash pgprbcfg_banner = {LBL_HZ12,3,3,8,strbuf};
LABEL flash snlbl = {LBL_HZ12,10,17,4,"1.序列号"};
LABEL flash snval = {LBL_HZ6X8,75,22,10,strbuf};
LABEL flash typelbl = {LBL_HZ12,10,31,3,"2.类型"};
LABEL flash typeval = {LBL_HZ6X8,59,36,10,strbuf};
LABEL flash paramlbl1 = {LBL_HZ12,130,31,3,"3."};
LABEL flash paramval1 = {LBL_HZ6X8,147,36,10,strbuf};
LABEL flash paramlbl2 = {LBL_HZ12,10,45,3,"4."};
LABEL flash paramval2 = {LBL_HZ6X8,27,50,10,strbuf};
LABEL flash paramlbl3 = {LBL_HZ12,130,45,3,"5."};
LABEL flash paramval3 = {LBL_HZ6X8,147,50,10,strbuf};
LABEL flash paramlbl3b = {LBL_HZ12,10,45,7,"3."};
LABEL flash paramval3b = {LBL_HZ6X8,27,50,10,strbuf};

//configuration of BORE probe parameter
void pgprbconfig_handler(uchar msg) {
	if( msg == KEY_CE) {
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		if(IS_THERM_MODE)
		{
		        sprintf(strbuf,"热电偶探头%i参数",curr_prb);
        		draw_label(&pgprbcfg_banner,SW_NORMAL);
	        	//name
		        strbuf[0]=':';
        		sprintf(strbuf,":%s",tname2b(curr_prb-1));
	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
        		//type
	        	sprintf(strbuf,":%s",getprbtype(tprbdata.type[curr_prb-1]));
		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
		}else{
		        sprintf(strbuf,"铂电阻探头%i参数",curr_prb);
        		draw_label(&pgprbcfg_banner,SW_NORMAL);
	        	//name
		        strbuf[0]=':';
		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
		        {
        		        sprintf(strbuf,":%s",rname2b(curr_prb-1));
        	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
        	        }else{  //PT1000 or PT100
               		        sprintf(strbuf,":%s Rtp:%7f",rname2b(curr_prb-1),rprbdata.rtp[curr_prb-1]);
        	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
        	        }
               		//type
                       	sprintf(strbuf,":%s",getprbtype(rprbdata.type[curr_prb-1]));
	                draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);

		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
		        {
		                sprintf(strbuf,"R(0!):%7f",rprbdata.param3[curr_prb-1]);
        		        draw_label(&paramlbl3b,SW_NORMAL);draw_label(&paramval3b,SW_NORMAL);
        		}else{
        		        //param1
                		sprintf(strbuf,"a:%9.3E",rprbdata.param1[curr_prb-1]);
	        	        draw_label(&paramlbl1,SW_NORMAL);draw_label(&paramval1,SW_NORMAL);
        	        	//param2
	        	        sprintf(strbuf,"b:%9.3E",rprbdata.param2[curr_prb-1]);
                		draw_label(&paramlbl2,SW_NORMAL);draw_label(&paramval2,SW_NORMAL);
	                	//param3
		                sprintf(strbuf,"c:%9.3E",rprbdata.param3[curr_prb-1]);
        		        draw_label(&paramlbl3,SW_NORMAL);draw_label(&paramval3,SW_NORMAL);
        		}
                }
		return;
	}
	if(msg == KEY_NUM1) {
		window_setup(7);
		sprintf(strbuf,"请输入探头序列号");
		prbsninput();
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == KEY_NUM2){
		nextwin = PG_PRBTYPELIST;
		return;
	}
	if(msg == KEY_NUM3 ){

	        if(IS_BORE_MODE)
	        {
               		window_setup(10);
	                if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	                {
        	                sprintf(strbuf,"输入R(0!)");
        	                rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
                        }else{
        	        	sprintf(strbuf,"输入系数a");
        	        	rprbdata.param1[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
        	        }
        		nextwin = PG_PRBCONFIG;
        	}
		return;
	}
	if(msg == KEY_NUM4) {
        	if(IS_BORE_MODE)
        	{
	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	                        return;
	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT1000)
	                        return;
        		window_setup(10);
	        	sprintf(strbuf,"输入系数b");
		        rprbdata.param2[curr_prb-1] = wnd_floatinput(rprbdata.param2[curr_prb-1]);
        		nextwin = PG_PRBCONFIG;
                }
		return;
	}
	if(msg == KEY_NUM5) {
	        if(IS_BORE_MODE)
	        {
       	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
       	                        return;
       	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT1000)
       	                        return;
	                window_setup(10);
       		        sprintf(strbuf,"输入系数c");
        		rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param3[curr_prb-1]);
		        nextwin = PG_PRBCONFIG;
	        }
		return;
	}
	return;
}

LABEL flash boot_banner = {LBL_HZ16,3,3,7,"请选择工作模式"};
LABEL flash boot_options[] = {
 	{LBL_HZ16, 10,23,7,"1.铂电阻测量"},
 	{LBL_HZ16, 130,23,7,"2.铂电阻配置"},
 	{LBL_HZ16, 10,45,7,"3.热电偶测量"},
 	{LBL_HZ16, 130,45,7,"4.热电偶配置"}
};
//boot up menu
extern u8 eeprom scanner_type;
void pgboottype_handler(uchar msg) {
	uchar i;
	static uchar curr_sel = 1;
	static uchar last_sel = 0xff;

	uchar min_option = 1;
	uchar max_option = sizeof(boot_options)/sizeof(LABEL);
	if(msg == KEY_NUM5) { //select scanner type
       		window_setup(4); //2 char at max
		sprintf(strbuf,"Scanner(1:MI,2:GUIDLINE)");
		i = scanner_type;
		i = wnd_intinput(i);
		if(i == 1 || i == 2)
		        scanner_type = i;
		else
		        scanner_type = 1;
	        msg = MSG_INIT;
	}

	if(msg == MSG_INIT) {
		LCD_Cls();
		last_sel = 0xff;
		draw_label(&boot_banner, SW_NORMAL);
		INIT_OPTIONS(boot_);
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN)
	{
	        last_sel = curr_sel;
	        curr_sel++;
	        if(curr_sel > max_option)
	                curr_sel = 1;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_UP)
	{
	        last_sel = curr_sel;
	        curr_sel--;
	        if(curr_sel == 0)
	                curr_sel = max_option;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_OK)
	{
	        msg = curr_sel + KEY_NUM0;
	}
	if(msg == KEY_NUM1) {
		SET_BORE_MODE;
		SET_TORS;
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_NUM2) {
		SET_BORE_MODE;
		SET_TORS;
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM3) {
		SET_THERM_MODE;
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_NUM4) {
		SET_THERM_MODE;
		nextwin = PG_TCONFIG;
		return;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS(boot_);
	}
}
#include "utili.h"
#include "window.h"
#include "stdlib.h"
/*---------------延时子程序----------------*/
/*
void delay1 (uint ms)
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;

}
*/
/*
char lowc(uchar x)
{
	x = x&0x0f;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}
char highc(uchar x)
{

	x = x>>4;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}
*/
/*计算字符串长度*/

extern uchar pos_databuf;
extern uchar data_sign;
extern u8 databuf[12];
double buf2double()
{
        return atof(databuf);
        /*
	double tmp = 0.0;
	uchar i = 0;
	uchar pos = 0;
	for(i=1;i<pos_databuf;i++) {
		if(databuf[i] != KEY_DOT)
			tmp = tmp * 10.0+(databuf[i] - '0');
		else
			pos = pos_databuf - i - 2;
	}
	while(pos > 0) {
		tmp = tmp / 10.0;
		pos--;
	}
	if(data_sign == 1)
	        tmp = -tmp;
       	return tmp;
       	*/
}
int buf2byte()	    //convert rundata.tempbuf to byte (00-99)
{
        return atoi(databuf);
        /*
	int tmp = 0;
	uchar i;
	for(i=0;i<pos_databuf;i++) {
		tmp = tmp * 10+(databuf[i] - '0');
	}
	return tmp;
	*/
}


#include "window.h"
#include "utili.h"
#include "lcd.h"

MSG_HANDLER curr_window = 0;

extern uchar key;

u8 databuf[12];
u8 pos_databuf; //position in data buffer
u8 max_databuf;
u8 data_sign;   // sign of the data
LABEL flash datalbl = {LBL_HZ16,10,10,8,strbuf};
LABEL flash datalbl2 = {LBL_HZ6X8,140,54,8,"UP:+/-,DN:'E'"};
LABEL flash datalbl3 = {LBL_HZ6X8,140,54,8,"UP/DN:'A'-'Z'"};
LABEL flash databox = {LBL_HZ16,20,30,9,databuf};


void prbsninput()
{
	uchar msg,len;
	databuf[0] = '\0';
	pos_databuf = 0;
	key = MSG_INIT;
	while(1)
	{
		if(key == KEY_INVALID)
		        continue;

		msg = key;


		if(msg == MSG_INIT) {
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_label(&datalbl3,SW_NORMAL);
			sprintf(databuf,"");
			draw_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}
		if(msg == KEY_TAB)
		{
		/*
        		if(pos_databuf < max_databuf)
        		{
			        databuf[pos_databuf++] = '0';
			        databuf[pos_databuf] = '\0';
        			msg = MSG_REFRESH;
                        }
                */
                        key = KEY_INVALID;
                        return;
		}
		if(msg == KEY_CE) {
		        key = KEY_INVALID;
			return;
		}

		if(msg == KEY_DN) {
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '0')
				databuf[pos_databuf-1] = 'Z';
			else if (databuf[pos_databuf-1] == 'A')
				databuf[pos_databuf-1] = '9';
			else
				databuf[pos_databuf-1] -= 1;
			msg = MSG_REFRESH;
		}
		if(msg == KEY_UP) {
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '9')
				databuf[pos_databuf-1] = 'A';
			else if (databuf[pos_databuf-1] == 'Z')
				databuf[pos_databuf-1] = '0';
			else
				databuf[pos_databuf-1] +=1;
			msg = MSG_REFRESH;
		}
		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
        		if(pos_databuf < max_databuf)
        		{
			        databuf[pos_databuf++] = msg;
        			databuf[pos_databuf] = '\0';
        		}
			msg = MSG_REFRESH;
		}
		if(msg == KEY_OK) {
		        if(IS_THERM_MODE)
		        {
			        for(len = 0; len < pos_databuf; len++)
        		                tprbdata.name[curr_prb-1][len] = databuf[len];
        			tprbdata.name[curr_prb-1][pos_databuf] = '\0';
        		}else{
			        for(len = 0; len < pos_databuf; len++)
        		                rprbdata.name[curr_prb-1][len] = databuf[len];
        			rprbdata.name[curr_prb-1][pos_databuf] = '\0';
        		}
			key = KEY_INVALID;
			break;
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}
		key = KEY_INVALID;
	}
//	LCD_HideCursor();
}

uchar wnd_intinput(uchar lastval)
{
	uchar msg;

	key = MSG_INIT;
	databuf[0] = '\0';
	pos_databuf = 0;
        data_sign = 0;
	while(1)
	{
		if(key != KEY_INVALID)
		{
			msg = key;

		}else{
			continue;
		}
		if(msg == MSG_INIT)
		{
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);

		}

		if(msg == KEY_TAB)
		{
		/*
        		if(pos_databuf < max_databuf)
        		{
			        databuf[pos_databuf++] = '0';
			        databuf[pos_databuf] = '\0';
        			msg = MSG_REFRESH;
                        }
                */
		}

		if(msg == KEY_DN) {
		/*
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '0')
				databuf[pos_databuf-1] = '9';
			else
				databuf[pos_databuf-1] -= 1;
			msg = MSG_REFRESH;
                */
		}
		if(msg == KEY_UP) {
		/*
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '9')
				databuf[pos_databuf-1] = '0';
			else
				databuf[pos_databuf-1] += 1;
			msg = MSG_REFRESH;
                */
		}
		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
        		if(pos_databuf < max_databuf)
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';
		        	msg = MSG_REFRESH;
		        }
		}
		if(msg == KEY_CE) {
//			LCD_HideCursor();
                        key = KEY_INVALID;
			return lastval;
		}
		if(msg == KEY_OK){
//			LCD_HideCursor();
                        key = KEY_INVALID;
			return buf2byte();
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}
		key = KEY_INVALID;
	}
}
double wnd_floatinput(double lastval)
{
	uchar msg;

	databuf[0] = '+';
        databuf[1] = '\0';
        pos_databuf = 1;
        data_sign = 0;
	key = MSG_INIT;
	while(1)
	{
		if(key != KEY_INVALID)
		{
			msg = key;

		}else{
			continue;
		}
		if(msg == MSG_INIT)
		{
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_label(&datalbl2,SW_NORMAL);
			draw_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}

		if(msg == KEY_TAB)
		{
		        msg = MSG_REFRESH;
		}
		if(msg == KEY_UP) {
		        if(pos_databuf == 1)
		        {
		                if((data_sign & 0x01) == 0)      //no sign
		                {
		                        databuf[0] = '-';
		                        data_sign |= 0x01;
		                }else{
		                        databuf[0] = '+';       //
		                        data_sign ^= 0x01;
		                }
		        }else{
                       		if((pos_databuf < max_databuf) && (databuf[pos_databuf-2] == 'E'))
                		{
			                if((data_sign & 0x08) == 0)
			                {
               		        	        databuf[pos_databuf-1] = '-';
               		        	        data_sign |= 0x08;
               		                }else{
               		        	        databuf[pos_databuf-1] = '+';
               		        	        data_sign ^= 0x08;
               		                }
	        		}
		        	msg = MSG_REFRESH;
		        }
		        msg = MSG_REFRESH;
		}
		if(msg == KEY_DN) {
               		if((pos_databuf < max_databuf) && ((data_sign & 0x04) == 0))    //no E in string
        		{
        			databuf[pos_databuf++] = 'E';
        			databuf[pos_databuf++] = '+';
	        		databuf[pos_databuf] = '\0';
	        		data_sign |= 0x04;
		        	msg = MSG_REFRESH;
		        }

			msg = MSG_REFRESH;
		}
		if( msg == KEY_DOT)
		{
               		if((pos_databuf < max_databuf) && ((data_sign & 0x02) == 0))      //no dot in string
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';
	        		data_sign |= 0x02;
		        	msg = MSG_REFRESH;
		        }

		}
		if((msg >= KEY_NUM0 && msg <= KEY_NUM9)) {
               		if(pos_databuf < max_databuf)
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';
		        	msg = MSG_REFRESH;
		        }
		}
		if(msg == KEY_CE) {
//			LCD_HideCursor();
		key = KEY_INVALID;
			return lastval;
		}
		if(msg == KEY_OK){
				key = KEY_INVALID;
//			LCD_HideCursor();
			return buf2double();
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}
				key = KEY_INVALID;
	}
}
//display a message box
void wnd_msgbox(flash LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
	draw_label(lbl, SW_NORMAL);
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);

}

void draw_inputbox(flash LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	draw_label(lbl, SW_NORMAL);
}

void draw_label(flash LABEL *lbl,uchar reverse) {
	uchar len = lbl->width;
	if(lbl->type == LBL_HZ6X8) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 8);
			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
			return;
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len , 8);
		return;
	}
	if(lbl->type == LBL_HZ8X16) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 16);
			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
			return;
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 1, 16);
		return;
	}
	/*
	if(lbl->type == LBL_HZ24X32) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 3, 32);
		return;
	}
	*/
	if(lbl->type == LBL_HZ12) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
		return;
	}
	if(lbl->type == LBL_HZ16) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 16);
			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
		return;
	}
}


