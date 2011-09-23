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

void testA(uchar data)
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
int ch_to_search=0;
int curr_prb=0;
int curr_dispch=0;
int curr_ch = 0;
int nextwin=0;
/**************************************************************************************/
//                              Main Function Loop
/**************************************************************************************/
extern u8 databuf[12];
extern u8 pos_databuf; //position in data buffer
void main(void)
{
    u16 i;  
// RS485 Node    
    init_var();	//init data structure 
//  System Initialization
    Init_Port();
//  Init_Timers();
//  Init_Ex_Interrupt();
    Init_UART();  
    Enable_XMEM();
    Init_554();                
    Key_Init();    
    // Global enable interrupts
    WDTCR = 0x00; //disable dog watch
    #asm("sei")                 
    /*********************************************************************/
    // System hardware dection
    /*********************************************************************/
    while(1)
    {
        sleepms(10000);
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
