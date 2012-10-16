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
extern void pg_main_handler(uchar);	
extern void pg_ch_sel_handler(uchar);	
extern void pg_prb_sel_handler(uchar);	
extern void pg_prb_param_handler(uchar);	
extern void pg_system_handler(uchar);	
extern void pg_boot_handler(uchar); 
extern void com1_putstr(char *s);
extern unsigned int strlen(char *s);
RUNDATA rundata;
SYSDATA eeprom sysdata = {
	0,0,100,0,0,25,3,
	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},//24IDs
	1,1,3};
PRBDATA	eeprom prbdata = {
       {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, //24paramA
       {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, //24paramB
       {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, //24paramC
       {25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0}, //24rtp
       {"1","","","","","","","",
	"","","","","","","","",
	"","","","","","","",""},//24name
       {4,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 }, //24 current
       {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff} //24type
};

char  strbuf[50];        //buffer for menu display
uchar key;

LABEL flash bootup = {LBL_HZ16,0,10,7,"ÕýÔÚÆô¶¯..."};

LABEL flash lbldbg = {LBL_HZ6X8,10,30,8,strbuf};

void testA(uchar i)
{
}

/**************************************************************************************/
//                      Timer 1 overflow interrupt service routine
/**************************************************************************************/
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Place your flash here
}
void dbgout(double val)
{
        sprintf(strbuf,"%f\r\n",val);
        prints(strbuf,strlen(strbuf),PORT_PC);        
}
#define SDBG         prints(strbuf,strlen(strbuf),PORT_PC)

double mabs(double val)
{
        return (val<0)? -val:val;
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
LABEL flash statelbl = {LBL_HZ6X8,100,55,16,strbuf};
int ch_to_search=1; //1 based index
int last_ch_to_search = 0;
int nextwin=0; //ID to next window
uint dlg_cnt,onesec_cnt;

uchar phase = 0;                
double vrs,vrx,vcross_p,vcross_n;
double isrc = 0.001;             //current of i src based on initial vrs reading
double rs = 100;                 //rs value
double rx = 100;                 //calculated rx value
uchar irx=1;                     //index of irx
unsigned int k_pos = 800;        //default k value

//several phase define
//0: switch to new channel 
//10: capture prepare , and get vrs
//20: get vrx,
//30: prepare track
//40: TRACK P irx
//50: TRACK N irx
//60: update reading
uchar track_state()
{
                  
    uchar i;
    unsigned int newN; 
return;	
	i = sysdata.id[ch_to_search-1];   
	
	if(i == INVALID_PROBE)
	{
        	rundata.temperature[ch_to_search-1] = -9999;
        	rundata.reading[ch_to_search-1] = -9999;
        	phase = 60;
	}
	
	if(phase == 0)
	{
	        irx = prbdata.current[ch_to_search-1];
		if(ch_to_search == last_ch_to_search) //not new channel
		{
			phase = 40;
			return 0;
		}
		scanner_set_channel(ch_to_search);
		phase = 10;
		dlg_cnt = 0;
		return 0;
	}
	if(phase == 10)
	{
                navto1v();                                     
                turn_k(800); //1:1 mode                       
                relaystate(RLYMODE_VRS + irx);  
		dlg_cnt = 1;
		phase = 20;
		return 0;
	
	}
	if(phase == 20)
	{
                vrs = nav_read_stable();
                if((vrs > 10.0)||(vrs < -10.0))
		{
			phase = 59;
                        return 0;
		}
                //update vrx
                relaystate(RLYMODE_VRX + irx);  		
		dlg_cnt = 1;
		phase = 30;
		return 0;
	}
	if(phase == 30)
	{
                vrx = nav_read_stable();
                if((vrx > 10.0)||(vrx < -10.0))
		{
	                phase = 59;
                        return 0;
		}
                //update k
                turn_k(k_pos + calc_capture_nextk());		
                navto120mv();                                     
		dlg_cnt = 0;
		phase = 40;
		return 0;
	}
	if(phase == 40)
	{
                relaystate(RLYMODE_TRACKP + irx);       
		dlg_cnt = 5;
		if(sysdata.kttmode)
			phase = 50;
		else
			phase = 55;
		return 0;
	}
	if(phase == 50)
	{
		vcross_p = nav_read_stable();
                if((vcross_p > 10.0)||(vcross_p < -10.0))
        	{
	        	phase = 59;
                        return 0;
        	}
                relaystate(RLYMODE_TRACKN + irx); 
		dlg_cnt = 5;
		phase = 55;
		return 0;
	}
	if(phase == 55)
	{
		vcross_n = nav_read_stable();
                if((vcross_n > 10.0)||(vcross_n < -10.0))
		{
			phase = 59;
			return 0;
		}
		if(!sysdata.kttmode)
		{
			vcross_p = vcross_n;
			vcross_n = -vcross_n;
		}
		newN = calc_track_nextk();   //rx is updated here
                if(newN > 0)                 
                {
                        if(newN >= 64)   //too big changes
			{
				phase = 10; //capture again
                                return 0;
			}
                        turn_k(k_pos + newN);  
			phase = 40;
			return 0;
                }        
		rundata.reading[ch_to_search-1] = rx;
		rundata.temperature[ch_to_search-1] = RValueToTValue(rx,sysdata.id[ch_to_search-1]);
                phase = 60;
		return 0;
	}
	if(phase == 59)
	{
		rundata.reading[ch_to_search-1] = -9999;
		rundata.temperature[ch_to_search-1] = -9999;
		phase = 60;
	}
	if(phase == 60)
	{
		pg_main_handler(MSG_REFRESH);
		phase = 0;
	}
	return 1;
}

void updatestate()
{
        char star[6];
        sprintf(star,"    ");
        sprintf(strbuf,"(ch%2i,%2d,%2i)",ch_to_search+1,dlg_cnt/ONESEC,phase);                        
        draw_label(&statelbl,SW_NORMAL);
}

/**************************************************************************************/
//                              Main Function Loop
/**************************************************************************************/
extern u8 databuf[12];
extern u8 pos_databuf; //position in data buffer
extern void update_shortcuts();
void main(void)
{
    u16 i;     
// RS485 Node    
    init_var();	//init data structure 
//  System Initialization
    Init_Port();
//  Init_Timers();
    Init_Ex_Interrupt();
    Init_UART();  
    Enable_XMEM();
    Init_554();    
    LCD_Init();            
    Key_Init();    
    // Global enable interrupts
    WDTCR = 0x00; //disable dog watch
    #asm("sei")                 
    /*********************************************************************/
    // System hardware detection
    /*********************************************************************/
    rundata.runstop &= 0xFE; //set to stop state 
    ch_to_search = 1;        
    update_shortcuts();
    nextwin = PG_BOOT;
	 scanner_set_mode();
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
			(*curr_window)(key);
			key = KEY_INVALID;
			continue;
		}		
		if(curr_window != pg_main_handler) //update data only in main screen
			continue;   		
		if(dlg_cnt > 1) //still in delay part
		{         
		        onesec_cnt++;
		        if(onesec_cnt == (ONESEC-10))
       			        updatestate(); //display status string every 1 second

       			if(onesec_cnt == ONESEC)
       			        onesec_cnt = 0 ;
			dlg_cnt--;
			continue;
		}    
		updatestate();          //display status string
		if(track_state() == 0)  //update the tracking state
			continue;            
		last_ch_to_search = ch_to_search;
                //shift to next valid channel
                while(true)
                {
                    ch_to_search += 1;
                    if(ch_to_search > MAX_CH_NUM)
                    {
	        	ch_to_search = 1;
	        	break;
                    }           
	            i = sysdata.id[ch_to_search-1];
       		    if(i == INVALID_PROBE)
   		        continue;
       		    if((prbdata.type[i] == PRBTYPE_PT25) || (prbdata.type[i] == PRBTYPE_PT100))
       		        break;
		}	
	 }
}      

WINDOW flash wins[]={
	{PG_MAIN,		pg_main_handler},	
	{PG_CH_SEL,		pg_ch_sel_handler},	
	{PG_PRB_SEL,	        pg_prb_sel_handler},
	{PG_PRB_PARAM,	        pg_prb_param_handler},
	{PG_SYSTEM,		pg_system_handler},	
	{PG_BOOT,	        pg_boot_handler}
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
