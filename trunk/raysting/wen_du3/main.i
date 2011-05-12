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
// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega64
#pragma used+
sfrb PINF=0;
sfrb PINE=1;
sfrb DDRE=2;
sfrb PORTE=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      // 16 bit access
sfrb ADCSRA=6;
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRR0L=9;
sfrb UCSR0B=0xa;
sfrb UCSR0A=0xb;
sfrb UDR0=0xc;
sfrb SPCR=0xd;
sfrb SPSR=0xe;
sfrb SPDR=0xf;
sfrb PIND=0x10;
sfrb DDRD=0x11;
sfrb PORTD=0x12;
sfrb PINC=0x13;
sfrb DDRC=0x14;
sfrb PORTC=0x15;
sfrb PINB=0x16;
sfrb DDRB=0x17;
sfrb PORTB=0x18;
sfrb PINA=0x19;
sfrb DDRA=0x1a;
sfrb PORTA=0x1b;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   // 16 bit access
sfrb SFIOR=0x20;
sfrb WDTCR=0x21;
sfrb OCDR=0x22;
sfrb OCR2=0x23;
sfrb TCNT2=0x24;
sfrb TCCR2=0x25;
sfrb ICR1L=0x26;
sfrb ICR1H=0x27;
sfrw ICR1=0x26;   // 16 bit access
sfrb OCR1BL=0x28;
sfrb OCR1BH=0x29;
sfrw OCR1B=0x28;  // 16 bit access
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  // 16 bit access
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  // 16 bit access
sfrb TCCR1B=0x2e;
sfrb TCCR1A=0x2f;
sfrb ASSR=0x30;
sfrb OCR0=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TIFR=0x36;
sfrb TIMSK=0x37;
sfrb EIFR=0x38;
sfrb EIMSK=0x39;
sfrb EICRB=0x3a;
sfrb XDIV=0x3c;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
#pragma used-
// Interrupt vectors definitions
// uart.h
// global.h    
// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega64
//#define NULL 0x0
                                        /*
        Port related definition
*/ 
void kbd_uart_push(unsigned char);
void scanner_uart_push(unsigned char);
void pc_uart_push(unsigned char);
void nav_uart_push(unsigned char);
void testA(unsigned char);
//#define PORT_3          SPORTA
//#define PORT_B          SPORTB
// Hardware related
                            void sleepms(unsigned int ms);
                              double nav_read();
void scanner_set_mode();
void d_mputs(unsigned char *buf, unsigned char size);
void d_putchar2(char);    
void d_putchar(char);
void d_putstr(flash unsigned char *ptr);
void Init_UART(void);
void com0_putc(unsigned char);      
void com0_putstr(unsigned char *ptr);
void com1_putc(unsigned char);
void putchar(char);  
void putchar2(char);
void putstr(flash unsigned char *ptr);    
void mputs(unsigned char *buf, unsigned char size);
unsigned char ishexchar(unsigned char dat);
// Initialization header files
extern void Init_Port(void);
extern void Init_Port(void);
extern void Init_Timers(void);
extern void Enable_XMEM(void);
extern void Init_Ex_Interrupt(void);
extern void Init_ADC(void);
/*********************************************************************************/
//                              16C554 Header file
/*********************************************************************************/
// global.h    
                                                                                                          // Registers of 16C554
// Registers for UART A
// Registers for UART B
// Registers for UART C
// Registers for UART D
extern void Reset_554(void);
extern void Init_554(void);
void prints(unsigned char*, unsigned char, char);
                                // global.h    
                                                                                                          // global.h    
                                                                                                          void delay (unsigned int us) ;
void delay (unsigned int us) ;
void delay (unsigned int us) ;
void delay1 (unsigned int ms);
char lowc(unsigned char x);
char highc(unsigned char x);
//'k'
//'l'
//#define PG_TPRBCONFIG	12
/*
 *	Probe data structure definition
 */
typedef eeprom struct _PRBDATA
{
	double param1[24];
	double param2[24];
	double param3[24];
	char  name[24][8];	        //probe serials
	unsigned char type[24];		//probe type
}PRBDATA;
typedef eeprom struct _SYSDATA
{
	double          R0;  //zero offset
	double          V0;  //zero offset
	double          Rs1; //jiao-zheng zhi
	int             ktime;//time for switch
	unsigned char 	        tid[24];	//probe index of each channel for T mode
	unsigned char           rid[24];        //probe index of each channel for R mode
	unsigned char           prbmode;
	unsigned char           kttmode;                    
}SYSDATA;               
typedef struct _RUNDATA
{
	double  reading[24];		  //reading on each channel
	double  temperature[24];	  //temperature result on each channel
	double 	Rx;		  //final result
	double  stdV;		  //voltage on stdV;
}RUNDATA;
extern RUNDATA rdata;
extern SYSDATA eeprom sysdata;
extern PRBDATA eeprom tprbdata;	//probe data for T mode
extern PRBDATA eeprom rprbdata;	//probe data for R mode
void State_Init();
void display_buttons(unsigned char pos,unsigned char val);           
double buf2double();
int buf2byte();
//#define ONESECBIT       14
extern void DBG(unsigned char);
void SwitchWindow(unsigned char page);
char* rname2b(unsigned char i);
char* tname2b(unsigned char i);
// CodeVisionAVR C Compiler
// (C) 1998-2006 Pavel Haiduc, HP InfoTech S.R.L.
// Prototypes for standard I/O functions
// CodeVisionAVR C Compiler
// (C) 1998-2002 Pavel Haiduc, HP InfoTech S.R.L.
// Variable length argument list macros
typedef char *va_list;
#pragma used+
char getchar(void);
void putchar(char c);
void puts(char *str);
void putsf(char flash *str);
char *gets(char *str,unsigned int len);
void printf(char flash *fmtstr,...);
void sprintf(char *str, char flash *fmtstr,...);
void snprintf(char *str, unsigned int size, char flash *fmtstr,...);
void vprintf (char flash * fmtstr, va_list argptr);
void vsprintf (char *str, char flash * fmtstr, va_list argptr);
void vsnprintf (char *str, unsigned int size, char flash * fmtstr, va_list argptr);
signed char scanf(char flash *fmtstr,...);
signed char sscanf(char *str, char flash *fmtstr,...);
                                               #pragma used-
#pragma library stdio.lib
// global.h    
                                                                                                          typedef void (*MSG_HANDLER)(unsigned char key);
typedef void (*MSG_HANDLER)(unsigned char key);
typedef void (*MSG_HANDLER)(unsigned char key);
typedef flash struct typWINDOW
{
	unsigned char page_id;	      //id of the page
	MSG_HANDLER handler;  //message handler of current window, a static variable can be declared in the function
}WINDOW;											
typedef flash struct typCHECKBOX
{
	unsigned int type;	//type of the label 
	int   x;
	int   y; 
	char  *param;
}CHECKBOX;
typedef flash struct  typLABEL
{
	unsigned int type;	//type of the label 
	int   x;
	int   y; 
	int   width; //width in characters
	char  *param;
}LABEL;
void draw_label(flash LABEL *lbl, unsigned char reverse);
void wnd_msgbox(flash LABEL *lbl);
void draw_inputbox(flash LABEL *lbl);
double wnd_floatinput(double oldval);
unsigned char wnd_intinput(unsigned char oldval);
void prbsninput();
extern MSG_HANDLER curr_window;
extern MSG_HANDLER caller;
extern unsigned char max_databuf;
//#define LCD_DATA_BUS   		P2      //MCU P1<------> LCM
// ASCII字符控制代码解释定义
// T6963C 命令定义
					// D1=1/0:光标显示启用/禁用；
					// D2=1/0:文本显示启用/禁用；
					// D3=1/0:图形显示启用/禁用；
					// D0-D2：定义D0-D7位；D3：1置位；0：清除
//void LCD_Busy (uchar autowr) ;    //测状态
//uchar LCD_BusyStatus () ;    //测状态
//void LCD_CheckBusy1(void) ;
//void LCD_CheckBusy2(void) ;
//void LCD_CheckBusy3(void) ;
//void LCD_Write1 (uchar dat,uchar comm) ;       //写一个数据和一个命令
//void LCD_Write2 (uchar datl,uchar dath,uchar comm) ;  //写两个数据和一个命令
//void LCD_WriteInt (uint dat,uchar comm) ;       //写一个16进制数据和一个命令
//void LCD_AutoWrite (uchar dat) ;               //自动写数据
//uchar LCD_Read(void) ;
//uchar LCD_AutoRead(void) ;
//void LCD_Comm (uchar comm) ;       //写命令
//void LCD_Data (uchar dat) ;       //写数据
void LCD_Init (void) ;
void LCD_Cls (void) ;
void LCD_ClsBlock(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
//void LCD_GraphSetAddr(uchar x,uchar y) ; /*地址*/
//void LCD_TextSetAddr(uchar x,uchar y) ; /*地址*/
//void LCD_ClrPixel(uchar x,uchar y) ; 
//void LCD_Putpixel(uchar x,uchar y) ;
void LCD_ShowCursor(unsigned char x,unsigned char y) ;/*光标*/
void LCD_HideCursor(void) ;
//void LCD_LineH(uchar y) ;
//void LCD_LineV(uchar x,uchar y1,uchar y2) ;
//void LCD_LineXX(uchar x1,uchar x2,uchar y);
void LCD_Rectange(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
//void LCD_Line(uchar x0,uchar y0,uchar x1,uchar y1) ;
//void LCD_PutImg(uchar x,uchar y,uchar w,uchar h,flash uchar *img) ;
//void LCD_PrintNumStr(uchar x,uchar y,uchar *s) ;
//void LCD_TextPutchar(uchar x,uchar y,uchar c) ;
//void LCD_TextPrint(uchar x,uchar y,char *s) ;
//void LCD_TextPrintHex(uchar x,uchar y,uchar hex) ;
//void LCD_GrapPutchar(uchar x,uchar y,uchar num) ;
//void LCD_GrapPrint(uchar x,uchar y,uchar code *s) ;
//void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt) ;
//void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty);
//void LCD_PrintWord(uchar x,uchar y,uint16 n,uchar start) ;
//void LCD_PrintHex(uchar x,uchar y,uchar hex) ;
//void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start) ;
void LCD_ReverseRect(unsigned char x,unsigned char y,unsigned char w,unsigned char h);
void LCD_PrintHz12(unsigned char x,unsigned char y,unsigned char *s);
void LCD_PrintHz16(unsigned char x,unsigned char y,unsigned char *s);
//void LCD_Print24X32(uchar x, uchar y,uchar *s);
void LCD_Print6X8(unsigned char x, unsigned char y,unsigned char *s);
void Key_Init();
void Key_Init();
void Key_Scan();
// global.h    
                                                                                                          // CodeVisionAVR C Compiler
// (C) 1998-2002 Pavel Haiduc, HP InfoTech S.R.L.
// Variable length argument list macros
double RValueToTValue(double r, unsigned char prbid);
double RValueToTValue(double r, unsigned char prbid);
double RValueToTValue(double r, unsigned char prbid);
double MValueToTValue(double r,char type);
double GetWr(double t);
double GetT(double w);
  																	void scanner_set_channel(unsigned char ch);
void scanner_uart_push(unsigned char data);
void pc_uart_push(unsigned char data);
void nav_uart_push(unsigned char data);     
void nav_command(unsigned char cmd);
// uart.h
/* CodeVisionAVR C Compiler
   Prototypes for standard library functions

   (C) 1998-2003 Pavel Haiduc, HP InfoTech S.R.L.
*/
#pragma used+
int atoi(char *str);
long int atol(char *str);
float atof(char *str);
void itoa(int n,char *str);
void ltoa(long int n,char *str);
void ftoa(float n,unsigned char decimals,char *str);
void ftoe(float n,unsigned char decimals,char *str);
void srand(int seed);
int rand(void);
void *malloc(unsigned int size);
void *calloc(unsigned int num, unsigned int size);
void *realloc(void *ptr, unsigned int size); 
void free(void *ptr);
#pragma used-
#pragma library stdlib.lib
//#include "math.h"
extern void pgmain_handler(unsigned char);	//running menu
extern void pgrconfig_handler(unsigned char);	//main menu of R config
extern void pgtconfig_handler(unsigned char);	//main menu of T config
extern void pgchset_handler(unsigned char);	//menu of channel probe setup
extern void pgprblist_handler(unsigned char);	//list of probe selection
extern void pgprbtypelist_handler(unsigned char); //select probe type
extern void pgprbset_handler(unsigned char);	//menu of probe setup
extern void pgprbconfig_handler(unsigned char);  //config of probe
extern void pgboottype_handler(unsigned char msg) ; //boot up config
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
unsigned char key;
LABEL flash bootup = {5,30,30,7,"正在启动..."};
LABEL flash modify = {5,30,30,8,"正在更新数据..."};
LABEL flash lbldbg = {1,10,30,8,strbuf};
unsigned char nextwin = 0;
int   curr_ch = 1;	//index of current channel in config menu window, 1 based
int   curr_prb = 1;	//index of current probe selection in menu window 1 based
int   curr_dispch = 1;  //index of start channel in main running window, 1 based
int ch_to_search = 0;   //current channel , 0 based
unsigned long dlg_cnt = 0;        //delay count time display
unsigned long onesec_cnt = 0;
unsigned char phase = 0;       //state machine index
double valuep;
/**************************************************************************************/
//                      Timer 1 overflow interrupt service routine
/**************************************************************************************/
interrupt [15] void timer1_ovf_isr(void)
{
// Place your flash here
}
void dbgout(double val)
{
        sprintf(strbuf,"%f\r\n",val);
        prints(strbuf,strlen(strbuf),3              );        
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
unsigned char nav1v = 1;
void navto120mv()
{               
   nav1v = 0;  
   dbgout(-0.12);      
   nav_command(4);
   sleepms(20000);            
}           
void navto1v()
{            
    nav1v = 1;
    dbgout(-1.0);
    nav_command(1);
    sleepms(20000);
    nav_command(3);
    sleepms(20000);
    nav_command(7);
    sleepms(20000);
    nav_command(6);
    sleepms(20000);
} 
double mabs(double val)
{
        if(val < 0)
                return -val;
        else
                return val;
}
unsigned char ToByte(char l, char h)
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
void Turn_on_Watchdog()
{ 
  #asm("cli")
  #asm("wdr")                    // reset watchdog timer to avoid timeout reset
  WDTCR |= (1<<4) | (1<<3); // set WDCE and WDE bit
  WDTCR = 0x08;                  // turn on watch dog timer: WDE = 1, WDP = 000, 16.3ms.
  #asm("sei")                    // Enable global interrupt.
}
char namebuf[8];
char* tname2b(unsigned char i)
{                       
        unsigned char j=0;
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
char* rname2b(unsigned char i)
{                       
        unsigned char j=0;
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
void sleepms(unsigned int ms)
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
unsigned char therm_state()
{                  
        unsigned char i;
	i = sysdata.tid[ch_to_search];   
	if(i == 0xff)
	{
        	rundata.temperature[ch_to_search] = -9999;
        	phase = 2;
	}
	if(phase == 0)
	{
		if( (tprbdata.type[i] >= 0x03) && (tprbdata.type[i] <= 0x09))
		{              
			scanner_set_channel(ch_to_search+1);	
			dlg_cnt = 100000;
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
                prints(strbuf,strlen(strbuf),3              );
				rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], tprbdata.type[i]);
//		if(ch_to_search==1) 		     rundata.temperature[ch_to_search] = 10;
                sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
                prints(strbuf,strlen(strbuf),3              );
		phase = 2;
	}
	if(phase == 2)
	{
		pgmain_handler(0xfe);
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
unsigned char bore_state()
{
        unsigned char i;
	i = sysdata.rid[ch_to_search];   
	if(i == 0xff)
	{
        	rundata.temperature[ch_to_search] = -9999;
        	phase = 5;
	}
	if(phase == 0)
	{
		if((rprbdata.type[i] <= 0xf2) && (rprbdata.type[i] >= 0xf1))
		{
			scanner_set_channel(ch_to_search+1);	
			display_buttons('i',1);
			display_buttons('j',0);
			dlg_cnt =  100000 * sysdata.ktime;
			onesec_cnt = 0;
			if(sysdata.kttmode == 1)
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
		display_buttons('i',0);
		dlg_cnt =  100000 * sysdata.ktime;
		onesec_cnt = 0;
		phase = 2;
		return 0;
	}
	if(phase == 2) //final stdV 
	{
		if(sysdata.kttmode == 1)
		{          
			rundata.stdV = (rundata.stdV + mabs(nav_read()));
			phase = 3;
		}else{                       
			rundata.stdV = mabs(nav_read());
			phase = 4;
		}                              
		display_buttons('j',1);
		dlg_cnt = 100000 * sysdata.ktime;      
		onesec_cnt = 0;
		return 0;	
	}
	if(phase == 3)  //-V on rx in ktt mode
	{                             
		valuep = mabs(nav_read());
		display_buttons('i',1);
		dlg_cnt = 100000 * sysdata.ktime;
		onesec_cnt = 0;
		phase = 4;
		return 0;	
	}
	if(phase == 4) // final V on rx
	{
		if(sysdata.kttmode == 1){       
			valuep = (valuep + mabs(nav_read()));
			dbgout(valuep);
			if((valuep > 0.21) && (valuep < 2) && (nav1v == 0))
			{
        		        navto1v();     
			}            
			if((valuep < 0.21) && (valuep > 0.0002) && (nav1v == 1))
			{
			        navto120mv();
			}
		}else{               
			valuep = mabs(nav_read());                      
			if((valuep > 0.105) && (valuep < 1) && (nav1v == 0))
			{
        		        navto1v();     
			}            
			if((valuep < 0.105) && (valuep > 0.0001) && (nav1v == 1))
			{
			        navto120mv();
			}
		}
		if(rundata.stdV != 0)
		{            
			rundata.reading[ch_to_search] = valuep*sysdata.Rs1/rundata.stdV - sysdata.R0;
			if(rundata.reading[ch_to_search] > 0)
			{
                                sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.Rx);
                                prints(strbuf,strlen(strbuf),3              );
			        rundata.temperature[ch_to_search] = RValueToTValue(rundata.reading[ch_to_search],i);
                                rundata.Rx = rundata.reading[ch_to_search];
                                sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
                                prints(strbuf,strlen(strbuf),3              );
			}
		}else{  
		        rundata.temperature[ch_to_search] = -9999;
		}
		phase = 5;
	}
	if(phase == 5) //got one reading
	{
		pgmain_handler(0xfe);
		phase = 0;
	}                 
	return 1;
}         
LABEL flash statelbl = {1,100,55,16,strbuf};
void updatestate()
{
        char star[6];
        sprintf(star,"    ");
        if(phase == 0)                sprintf(star,"*   ");
        if(phase == 1)                sprintf(star,"**  ");
        if(phase == 2)                sprintf(star,"*** ");        
        if(phase == 3)                sprintf(star,"****");        
                        if(sysdata.prbmode == 1){         
                if(sysdata.kttmode == 1)                                                   
                        sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/100000,star);
                else
                        sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/100000,star);                        
        }else{                                                                      
                sprintf(strbuf,"(ch:%2i,%2d,%s)",ch_to_search+1,dlg_cnt/100000,star);
        }      
        draw_label(&statelbl,1);
}
static unsigned char tA = 0xff;
static unsigned char tB = 0xff;
void testA(unsigned char data)
{
        tA = data;        
}                       
void testB(unsigned char data)
{
        tB = data;
}
/**************************************************************************************/
//                              Main Function Loop
/**************************************************************************************/
extern double GetThmoVolt(double t,char type);      
extern unsigned char databuf[12];
extern unsigned char pos_databuf; //position in data buffer
void main(void)
{
    unsigned int i;  
    double dt;
    /*  just test algrithom 
    sprintf(databuf,"9.99");    
    pos_databuf = 4;
    dt = buf2double();
    rprbdata.type[0] = PRBTYPE_PT100;
    rprbdata.param1[0] = 3.9083e-3;
    rprbdata.param2[0] = -5.775e-7;
    rprbdata.param3[0] = 100;//-4.183e-12;
    dt = RValueToTValue(139.26, 0);//102
    */
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
        sleepms(2000);
    LCD_Init();
    wnd_msgbox(&bootup);
    //init the DMM
    nav_command(1);              
    sleepms(20000);
    nav_command(3);
    sleepms(20000);
    nav_command(7);
    sleepms(20000);
    nav_command(6);
    sleepms(20000);
                         sleepms(2*100000); //wait until all the node is ready after power up        
    State_Init();	
        sysdata.prbmode = 1; scanner_set_mode(); display_buttons('a',0);
        display_buttons('i',1);                               
    display_buttons('j',0);
    	 nextwin = 13;
	 key = '-'													  ;
	 curr_ch = 1; //channel for display
	 curr_dispch = 1;
	 while(1)
	 {
 	 	if(nextwin != 0)
		{
			SwitchWindow(nextwin);
			(*curr_window)(0xff);
			nextwin = 0;
		}
		if(key != '-'													  )
		{
			if((key == 'a')||(key == 'b')||(key == 'c')||(key == 'd'))
			{
				if(curr_window == pgmain_handler)
				{
					LCD_Cls();
					wnd_msgbox(&modify);
				}
				if(key == 'a') //mode switch
				{
					if(sysdata.prbmode == 1){
						sysdata.prbmode = 0; scanner_set_mode(); display_buttons('a',1);
                                                display_buttons('j',0);      
                				display_buttons('i',1);
                                                navto120mv();
					}else{
						sysdata.prbmode = 1; scanner_set_mode(); display_buttons('a',0);
                                                display_buttons('j',1);
                				display_buttons('i',1);                                                              
                                                navto1v();
					}
					dlg_cnt = 0;					
					onesec_cnt = 0;
					phase = 0;      //reset the state machine
					display_buttons('i',1);
				}
				if(key == 'b') //auto ktt or not
				{
					if(sysdata.prbmode == 1)
					{
						if((sysdata.kttmode == 1)){
							sysdata.kttmode = 0; display_buttons('b',1);
							display_buttons('i',1);
						}else{
							sysdata.kttmode = 1; display_buttons('b',0);
							display_buttons('i',1);
						}
					}
				}
				if(key == 'c') //thermal probe type
				{                            
				        					display_buttons('c',1);
					if(sysdata.prbmode == 0)
					{                        
					        i = sysdata.tid[curr_dispch-1];
					        if(i != 0xff)
					        {
                					if((tprbdata.type[i] >= 0x03) &&	                				   (tprbdata.type[i] <= 0x09))
		                			{
			                			if(tprbdata.type[i] == 0x09)
				                			tprbdata.type[i] = 0x03;
					                	else
						                	tprbdata.type[i] +=1;
        					        }
        					}
                                        }
					display_buttons('c',0);
									}
				if(key == 'd') //remove zero
				{
					display_buttons('d',1);
					if(sysdata.prbmode == 1){
					        sysdata.R0 = rundata.Rx;
					}else{             
					        //sysdata.V0 = nav_read();
					        nav_command(8);
					        sleepms(100000);
					}
					display_buttons('d',0);
				}
				if(curr_window == pgmain_handler)       //redraw the running window
				{
					pgmain_handler(0xff);      
				}
			}else{
				(*curr_window)(key);
			}
			key = '-'													  ;
	  	}else{
			if(curr_window != pgmain_handler)
				continue;                               
			                        			if(dlg_cnt > 1)
			{         
			        onesec_cnt++;
			        if(onesec_cnt == (100000-10))
			        {       
        			        updatestate();
        			}
        			if(onesec_cnt == 100000)
        			        onesec_cnt = 0 ;
				dlg_cnt--;
				continue;
			}    
			updatestate();
			if((sysdata.prbmode == 0))			
			{
				if(therm_state() == 0)
				        continue;
			}else{
				if(bore_state() == 0)
				        continue;
			}
                        //shift to next channel
                        while(1)
                        {
                                ch_to_search += 1;
        			if(ch_to_search >= 24)
        			{
	        			ch_to_search = 0;
	        			break;
	        		}           
	        		if(sysdata.prbmode == 0)
	        		{
	        		        i = sysdata.tid[ch_to_search];
	        		}else{
	        		        i = sysdata.rid[ch_to_search];
	        		}
	        		if(i == 0xff)
	        		        continue;
	        		if(sysdata.prbmode == 0)
	        		{   
       					if((tprbdata.type[i] >= 0x03) && (tprbdata.type[i] <= 0x09))
       					        break;
	        		}else{
       		        		if((rprbdata.type[i] <= 0xf2) && (rprbdata.type[i] >= 0xf1))
       		        		        break;
	        		}
	                }
		}
	}	
}      
WINDOW flash wins[]={
	{2,	pgmain_handler},	//running menu
	{3,	pgrconfig_handler},	//main menu of R config
	{4,	pgtconfig_handler},	//main menu of T config
	{7,	pgchset_handler},	//menu of channel probe setup
	{8,	pgprblist_handler},	//list of probe selection
	{9,pgprbtypelist_handler}, //select probe type
	{10,	pgprbset_handler},	//menu of probe setup
	{11,	pgprbconfig_handler},  //config of r probe
	{13,	pgboottype_handler}	//config of t probe
};
void SwitchWindow(unsigned char page)
{
	unsigned char i;
	for(i = 0; i<sizeof(wins)/sizeof(WINDOW);i++)
	{
		if(wins[i].page_id == page)
		{
			curr_window = wins[i].handler;
			return;
		}
	}
}
