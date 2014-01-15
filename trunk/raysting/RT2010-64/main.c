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
					        sysdata.R0 = rundata.Rx;
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
