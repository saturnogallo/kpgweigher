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


Chip type           : ATmega64
Program type        : Application
Clock frequency     : 14.745600 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 1024
*****************************************************/

#include <mega64.h>
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
	1,1};
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

int   curr_ch = 1;	//index of current channel in menu window
int   curr_prb = 1;	//index of current probe selection in menu window

int ch_to_search = 0;

long dlg_cnt = 0;        //delay count time display
uchar phase = 0;       //state machine index
double valuep;

/**************************************************************************************/
//                      Timer 1 overflow interrupt service routine
/**************************************************************************************/
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Place your flash here
}

void InitLED()
{
/*
    PORTB.7=1;
    PORTB.6=1;
    PORTB.5=1;
    PORTB.4=1;
    PORTD.7=1;
    PORTD.6=0; 
*/
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

            if(namebuf[j] < '0' || namebuf[j] > '9')
            {
                namebuf[0] = '\0';
                break;
            }

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

            if(namebuf[j] < '0' || namebuf[j] > '9')
            {
                namebuf[0] = '\0';
                break;
            }

        }
        return namebuf;
}                                        
void sleepms(u16 ms)
{
    while(ms-- > 0)
        ;
}


/**************************************************************************************/
//      board init related function.
/**************************************************************************************/
void init_var()
{                          
}
//state machine of therm type
//phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
//phase 1:get the reading and update the display 
//phase 2: move to next channel
uchar therm_state()
{                  
        uchar i;
	i = sysdata.tid[ch_to_search];   
	if(i == INVALID_PROBE)
	{
        	rundata.temperature[ch_to_search] = -9999;
        	phase = 2;
	}
	if(phase == 0)
	{
		if((tprbdata.type[i] <= PRBTYPE_R) && (tprbdata.type[i] >= PRBTYPE_K))
		{
			scanner_set_channel(ch_to_search+1);	
			dlg_cnt = ONESEC;
			phase = 1;
			return 0;
		}else{
			rundata.temperature[ch_to_search] = -9999; //invalid data
			phase = 2;
		}
	}
	if(phase == 1)
	{
		rundata.reading[ch_to_search] = nav_read();
                sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.reading[ch_to_search]);
                prints(strbuf,strlen(strbuf),PORT_PC);
		
		rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], tprbdata.type[i]);
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
	if(i == INVALID_PROBE)
	{
        	rundata.temperature[ch_to_search] = -9999;
        	phase = 5;
	}
	if(phase == 0)
	{
		if((rprbdata.type[i] <= PRBTYPE_PT25) && (rprbdata.type[i] >= PRBTYPE_PT100))
		{
			scanner_set_channel(ch_to_search+1);	
			SET_PKTT;
			SET_TORX;
			dlg_cnt =  ONESEC * sysdata.ktime;
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
		rundata.stdV = nav_read();
		SET_NKTT;
		dlg_cnt =  ONESEC * sysdata.ktime;
		phase = 2;
		return 0;
	}
	if(phase == 2) //final stdV 
	{
		if(IS_MODE_KTT)
		{
			rundata.stdV = (rundata.stdV + nav_read());
			phase = 3;
		}else{
			rundata.stdV = nav_read();
			phase = 4;
		}                              
		SET_TORX;
		dlg_cnt = ONESEC * sysdata.ktime;
		return 0;	
	}
	if(phase == 3)  //-V on rx in ktt mode
	{
		valuep = nav_read();
		SET_PKTT;
		dlg_cnt = ONESEC * sysdata.ktime;
		phase = 4;
		return 0;	
	}
	if(phase == 4) // final V on rx
	{
		if(IS_MODE_KTT){
			valuep = (valuep + nav_read());
		}else{
			valuep = nav_read();
		}
		if(rundata.stdV != 0)
		{       
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

void main(void)
{
    u16 i;  
    double dt;
          
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
    //init the DMM
    nav_command(NAV_INIT);              
    sleepms(2000);
    nav_command(NAV_1V);
    sleepms(2000);
    nav_command(NAV_SLOWMODE);
    sleepms(2000);
    nav_command(NAV_AFLTON);
    
  
    if(sysdata.Rs1 < 0.01)
    {
        sysdata.Rs1 = 100;
    }
    sleepms(2000);
    LCD_Init();
    wnd_msgbox(&bootup);

    sleepms(2*ONESEC); //wait until all the node is ready after power up        
    if(key != KEY_INVALID)
    {
        if(key == KEY_NUM1) //R0
        {       
                window_setup(10);                
                sprintf(strbuf,"请输入铂电阻R0阻值");
		sysdata.R0 = wnd_floatinput(sysdata.R0);
        }
        if(key == KEY_NUM2) //Rs1
        {       
                window_setup(10);                
                sprintf(strbuf,"请输入内标准阻值");
		sysdata.Rs1 = wnd_floatinput(sysdata.Rs1);
        }
    }
    
    State_Init();	
    

            
    SET_BORE_MODE;
    
    SET_PKTT;                               
    SET_TORS;
    
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
                                                SET_TORS;
                                                nav_command(NAV_120MV);
					}else{
						SET_BORE_MODE;
                                                SET_TORX;              
                                                nav_command(NAV_INIT);
                                                sleepms(2000);
                                                nav_command(NAV_1V);
                                                sleepms(2000);
                                                nav_command(NAV_SLOWMODE);
                                                sleepms(2000);
                                                nav_command(NAV_AFLTON);
                                                sleepms(2000);
					}
					dlg_cnt = 0;					
					phase = 0;      //reset the state machine

					SET_PKTT;
				}
				if(key == KEY_BTN2) //auto ktt or not
				{
					if(IS_BORE_MODE)
					{
						if((IS_MODE_KTT)){
							CLR_MODE_KTT;
						}else{
							SET_MODE_KTT;
							SET_PKTT;
						}
					}
				}
				if(key == KEY_BTN3) //thermal probe type
				{                            
				        
					display_buttons(KEY_BTN3,1);
					if(IS_THERM_MODE)
					{                        
					        i = sysdata.tid[ch_to_search];
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
        					}
                                        }
					display_buttons(KEY_BTN3,0);
					
				}
				if(key == KEY_BTN4) //remove zero
				{
					display_buttons(KEY_BTN4,1);
					if(IS_BORE_MODE){
					        sysdata.R0 = rundata.Rx;
					}else{
					        nav_command(NAV_ZEROON);
					        sleepms(1000);
					}
					display_buttons(KEY_BTN4,0);
				}
				if(curr_window == pgmain_handler)       //redraw the running window
				{
					pgmain_handler(MSG_INIT);
				}
			}else{
				(*curr_window)(key);
			}
			key = KEY_INVALID;
	  	}else{
			if(curr_window != pgmain_handler)
				continue;                               
			if(dlg_cnt > 1)
			{
				dlg_cnt--;
				continue;
			}
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
       		        		if((rprbdata.type[i] <= PRBTYPE_PT25) && (rprbdata.type[i] >= PRBTYPE_PT100))
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
