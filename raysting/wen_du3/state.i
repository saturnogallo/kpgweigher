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
//#define NULL 0x0
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
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega64
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
void SectorErase(unsigned int sector_addr);
void SectorErase(unsigned int sector_addr);
unsigned char byte_read(unsigned int byte_addr);
void byte_write(unsigned int byte_addr, unsigned char original_data);
void LoadFromEEPROM();
void SaveToEEPROM();
void LoadProbeData();
void SaveProbeData();
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
  																	  																	void scanner_set_channel(unsigned char ch);
void scanner_uart_push(unsigned char data);
void pc_uart_push(unsigned char data);
void nav_uart_push(unsigned char data);     
void nav_command(unsigned char cmd);
extern unsigned char nextwin;  
				                									extern int   curr_ch;	//index of current channel
extern int   curr_prb;	//index of current probe selection
void State_Init() {
	display_buttons('a',sysdata.prbmode);
	display_buttons('b',sysdata.kttmode);
	display_buttons('c',0); //probe type
	display_buttons('d',0); //zero
}
char* getprbtype(unsigned char);
LABEL flash pgmain_chs[] = {
		{4, 4, 3,3,strbuf},
		{1,2,30,3,strbuf},
		{1,2,40,3,strbuf},
		{1,2,50,3,strbuf}
	};
LABEL flash pgmain_values[] = {
		{4, 40,3,  7,strbuf},
		{1,40,30,10,strbuf},
		{1,40,40,10,strbuf},
		{1,40,50,10,strbuf}
	};
LABEL flash pgmain_temps[] = {
		{4, 140,3,  7,strbuf},
		{1,140,30,10,strbuf},
		{1,140,40,10,strbuf},
		{1,140,50,10,strbuf}
};
/*      Main display      */
void pgmain_handler(unsigned char msg) {
        unsigned char i,old_ch;	
                unsigned char *cptr;
	//LABEL flash usage = {LBL_HZ6X8,210,60,5,"usage"};
        //Menu
	if(msg == 'T') {
		if(sysdata.prbmode == 0)
			nextwin = 4;
		else
			nextwin = 3;
		return;
	}
	if(msg == 0xff) {
		LCD_Cls();
		old_ch = curr_ch;
		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
		{
			sprintf(strbuf,"CH%02i:",curr_ch);
			draw_label(&pgmain_chs[i],1);
			curr_ch += 1 ;  if(curr_ch > 24){ curr_ch = 1;};
		}
		curr_ch = old_ch;
		msg = 0xfe;
	}
	if(msg == 'U') {
		curr_ch -= 1 ;  if(curr_ch == 0){  curr_ch = 24;  };	
		old_ch = curr_ch;
		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
		{
			sprintf(strbuf,"CH%02i:",curr_ch);
			draw_label(&pgmain_chs[i],1);
			curr_ch += 1 ;  if(curr_ch > 24){ curr_ch = 1;};
		}
		curr_ch = old_ch;
		msg = 0xfe;
	}
	if(msg == 'D') {
		curr_ch += 1 ;  if(curr_ch > 24){ curr_ch = 1;};
		old_ch = curr_ch;
		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
		{
			sprintf(strbuf,"CH%02i:",curr_ch);
			draw_label(&pgmain_chs[i],1);
			curr_ch += 1 ;  if(curr_ch > 24){ curr_ch = 1;};
		}
		curr_ch = old_ch;
		msg = 0xfe;
	}
	if(msg == 0xfe) {
		old_ch = curr_ch;
		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
		{                         
		        if(sysdata.prbmode == 0)
		        {
			        cptr = getprbtype(tprbdata.type[sysdata.tid[curr_ch-1]]);
        			if((sysdata.tid[curr_ch - 1] == 0xff) ||        				(rundata.temperature[curr_ch - 1] < -9000) ||	        			(cptr[0] == 'P')){
        				sprintf(strbuf," -------");
	        			draw_label(&pgmain_values[i],1|4);
		        		draw_label(&pgmain_temps[i],1|4);
			        }else{
					sprintf(strbuf,"%8f",rundata.reading[curr_ch - 1]);
					sprintf(strbuf+8," mV");
					draw_label(&pgmain_values[i],1|4);
				        sprintf(strbuf," %8f",rundata.temperature[curr_ch - 1]);
        				sprintf(strbuf+8," C %c",cptr[0]);
	        			draw_label(&pgmain_temps[i],1|4); 
				        if(pgmain_temps[i].type == 1)
        				        LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12, pgmain_temps[i].y -2 +10);
        				else                                     //Hz12                            
                				LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12,pgmain_temps[i].y -2 + 16);	        			
	                        }
			}else{
       			        cptr = getprbtype(rprbdata.type[sysdata.rid[curr_ch-1]]);
        			if((sysdata.rid[curr_ch - 1] == 0xff) ||        				(rundata.temperature[curr_ch - 1] < -9000) ||	        			(cptr[0] != 'P'))
	        		{
        				sprintf(strbuf," -------");
	        			draw_label(&pgmain_values[i],1|4);
		        		draw_label(&pgmain_temps[i],1|4);
			        }else{
					sprintf(strbuf,"%8f",rundata.reading[curr_ch - 1]);
					sprintf(strbuf+8," ohm");
	        			draw_label(&pgmain_values[i],1|4);
		        		sprintf(strbuf," %8f",rundata.temperature[curr_ch - 1]);
			        	sprintf(strbuf+8," C %c",cptr[0]);
				        draw_label(&pgmain_temps[i],1|4);
				        if(pgmain_temps[i].type == 1)
        				        LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 +10);
        				else                                     //Hz12                            
                				LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 + 16);
        			}       			        
	                }
			curr_ch += 1 ;  if(curr_ch > 24){ curr_ch = 1;};
		}
		curr_ch = old_ch;
	}
}
//main configuration window of R
LABEL flash pgr_banner = {5,3,3,7,"铂电阻参数配置"};
LABEL flash pgr_calibrate = {5,30,30,8,"内标准校准中..."};
LABEL flash pgr_klbl = {1, 88,28, 3,strbuf}; //ktime label
LABEL flash pgr_options[] = {
	{5, 10,23,6,"1.电流换向"},
	{5,130,23,7,"2.内标准校准"},
	{5, 10,45,8,"3.通道探头选择"},
	{5,130,45,7,"4.设置探头参数"}
};
//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
void pgcalibrate()
{                             
        double oldvalue = 0;  
        unsigned int cnt;
        max_databuf = 10;
	sprintf(strbuf,"请输入外标准(CH1)阻值");
	oldvalue = wnd_floatinput(oldvalue);
	if(oldvalue == 0)
                return;	
        wnd_msgbox(&pgr_calibrate);                        
        sysdata.Rs1 = (oldvalue + sysdata.R0)*sysdata.Rs1/(rundata.Rx+sysdata.R0);
}       
//main menu of bore config
void pgrconfig_handler(unsigned char msg) {
	unsigned char i;
		static unsigned char curr_sel = 1;
	static unsigned char last_sel = 1;
	unsigned char min_option = 1;
	unsigned char max_option = sizeof(pgr_options)/sizeof(LABEL);
	if(msg == 'T' || msg == 'C' || msg == 'O') {
		nextwin = 2;
		return;
	}
	if(msg == 0xff) {
		LCD_Cls();
		draw_label(&pgr_banner, 1);
		for(i = min_option;i <= max_option; i++) {                                        draw_label(&pgr_options[i-1], 1);			        }	;
		sprintf(strbuf,"(%i)",sysdata.ktime);
		draw_label(&pgr_klbl, 1);
		msg = 0xfe;
	}
	if(msg == '1') {
		max_databuf = 4; //4 char at max
		sprintf(strbuf,"电流换向时间设置");
		sysdata.ktime = wnd_intinput(sysdata.ktime);
		nextwin = 3;
		return;
	}
	if(msg == '2')
	{
                pgcalibrate();
		nextwin = 3;
		return;
	}
	if(msg == '3') {
		nextwin = 7;
		return;
	}
	if(msg == '4') {
		nextwin = 10;
		return;
	}
	if(msg == 0xfe) {
		if(last_sel == curr_sel)                                        return;        			for(i = min_option;i <= max_option; i++){	        			if(last_sel == i)       draw_label(&pgr_options[i-1], 2);        				if(curr_sel == i)	draw_label(&pgr_options[i-1], 2);        			};
	}
}
//main configuration window of T
LABEL flash pgt_banner = {5,3,3,7,"热电偶参数配置"};
LABEL flash pgt_options[] = {
	{5,40,25,8,"1.选择各通道探头"},
	{5,40,45,8,"2.设置各探头参数"}
};
                          //main menu of thermo config
void pgtconfig_handler(unsigned char msg) {
	unsigned char i;
	static unsigned char curr_sel = 1;
	static unsigned char last_sel = 1;
	unsigned char min_option = 1;
	unsigned char max_option = sizeof(pgt_options)/sizeof(LABEL);
	if(msg == 'T' || msg == 'C' || msg == 'O') {
		nextwin = 2;
		return;
	}
	if(msg == 0xff) {
		LCD_Cls();
		draw_label(&pgt_banner, 1);
		for(i = min_option;i <= max_option; i++) {                                        draw_label(&pgt_options[i-1], 1);			        }	;
		msg = 0xfe;
	}
	if(msg == '1') {
		nextwin = 7;
		return;
	}
	if(msg == '2') {
		nextwin = 10;
		return;
	}
	if(msg == 0xfe) {
		if(last_sel == curr_sel)                                        return;        			for(i = min_option;i <= max_option; i++){	        			if(last_sel == i)       draw_label(&pgt_options[i-1], 2);        				if(curr_sel == i)	draw_label(&pgt_options[i-1], 2);        			};
	}
	return;
}
//channel probe setup
LABEL flash pgch_banner =    {5,3,3,4,"通道设置"};
LABEL flash pgch_chvalue = {1,130,3,2,strbuf};
LABEL flash pgch_items[] = {
	{1, 10, 30, 10, strbuf},
	{1, 130,30, 10, strbuf},
	{1, 10, 40, 10, strbuf},
	{1, 130,40, 10, strbuf},
	{1, 10, 50, 10, strbuf},
	{1, 130,50, 10, strbuf}
};
//select the channel to config
void pgchset_handler(unsigned char msg) {
	static unsigned int curr_pos = 0;
	static unsigned int last_pos = 0;
	unsigned char min_option = 1;
	unsigned char max_option = sizeof(pgch_items)/sizeof(LABEL);
	unsigned char min_index = 1;
	unsigned char max_index = 24;//rundata.scanmode;
	unsigned char new_page = 0;
	int i,j;
	if(msg == 'T' || msg == 'C') {
		if(sysdata.prbmode == 1)
			nextwin = 3;
		else
			nextwin = 4;
		return;
	}
	if(msg == 'U') {
		last_pos = curr_pos;   curr_pos--;			if(curr_pos < min_index) curr_pos = max_index;			if(last_pos == curr_pos)  return;			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;};
		msg = 0xfe;
	}
	if(msg == 'D') {
		last_pos = curr_pos;	curr_pos++;			if(curr_pos > max_index) curr_pos = min_index;			if(last_pos == curr_pos)	return;			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;};
		msg = 0xfe;
	}
	if(msg == 'O') {	//show window to select the probe
		curr_ch = curr_pos;
		nextwin = 8;
		return;
	}
	if(msg >= '0' && msg <= '9') {
		if((msg > '0') && (unsigned char)((msg-'0')*max_option) <= max_index)					{new_page = 1; curr_pos = (unsigned char)((msg-'1')*max_option)+1;};
		msg = 0xfe;
	}
	if(msg == '.')
	{                                    
	        if(sysdata.prbmode == 0)
	        {
		        sysdata.tid[curr_pos - 1] = 0xff;
	        }else{
       		        sysdata.rid[curr_pos - 1] = 0xff;
	        }
		msg = 0xfe;
	}
	if(msg == 0xff) {
		LCD_Cls();
		draw_label(&pgch_banner, 1);
		curr_pos = 1;
		new_page = 1;
		msg = 0xfe;
	}
	if(msg == 0xfe) {      
	        		sprintf(strbuf,"%i",curr_pos);draw_label(&pgch_chvalue,1);                		if(new_page == 1){		                	for(i = min_option; i <= max_option; i++){				                j = ((curr_pos-1)/max_option)*max_option + i;;
		        if(sysdata.prbmode == 0)
        	        {
				if(sysdata.tid[j-1] == 0xff){
					sprintf(strbuf,"CH%i:*",j);
				}else{
			        	//draw line item j
				        sprintf(strbuf,"CH%i:%s",j,tname2b((sysdata.tid[j-1])));
				}           
		        }else{
				if(sysdata.rid[j-1] == 0xff){
					sprintf(strbuf,"CH%i:*",j);
				}else{
			        	//draw line item j
				        sprintf(strbuf,"CH%i:%s",j,rname2b((sysdata.rid[j-1])));
				}           		        
		        }
		draw_label(&(pgch_items[i-1]),1);				        if((curr_pos-1) % max_option == (i-1))					        draw_label(&(pgch_items[i-1]),2);        			        }                                }else{	                		for(i = min_option; i <= max_option; i++) {        		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(pgch_items[i-1]),2);				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(pgch_items[i-1]),2);			                }		                }	;
	}
	return;
}
//select probe to edit.
LABEL flash pgprbset_banner = {5,3,3,8,"请选择要配置的探头"};
LABEL flash pgprbset_chvalue = {1,180,3,2,strbuf};
LABEL flash pgprbset_items[] = {
	{1, 10, 30, 10, strbuf},
	{1, 130,30, 10, strbuf},
	{1, 10, 40, 10, strbuf},
	{1, 130,40, 10, strbuf},
	{1, 10, 50, 10, strbuf},
	{1, 130,50, 10, strbuf}
};
//select probe to config
void pgprbset_handler(unsigned char msg) {
	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
	static unsigned int curr_pos = 1;
	static unsigned int last_pos = 1;
	unsigned char min_option = 1;
	unsigned char max_option = sizeof(pgprbset_items)/sizeof(LABEL);
	unsigned char min_index = 1;
	unsigned char max_index = 24;
	int i,j;
	unsigned char new_page = 1;
	if(msg >= '0' && msg <= '9') {
		if((msg > '0') && (unsigned char)((msg-'0')*max_option) <= max_index)					{new_page = 1; curr_pos = (unsigned char)((msg-'1')*max_option)+1;};
		msg = 0xfe;
	}
	if(msg == 'C' || msg == 'T')
	{
		if(sysdata.prbmode == 0)
			nextwin = 4;
		else
			nextwin = 3;
		return;
	}
	if(msg == 'O')
	{
		curr_prb = curr_pos;
		nextwin = 11;
		return;
	}
	if(msg == 0xff)
	{
		LCD_Cls();
		draw_label(&pgprbset_banner, 1);
		curr_pos = 1;
		new_page = 1;
		msg = 0xfe;
	}
	if(msg == 'U') {
		last_pos = curr_pos;   curr_pos--;			if(curr_pos < min_index) curr_pos = max_index;			if(last_pos == curr_pos)  return;			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;};
		msg = 0xfe;
	}
	if(msg == 'D') {
		last_pos = curr_pos;	curr_pos++;			if(curr_pos > max_index) curr_pos = min_index;			if(last_pos == curr_pos)	return;			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;};
		msg = 0xfe;
	}
	if(msg == 0xfe) {
		sprintf(strbuf,"%i",curr_pos);draw_label(&pgprbset_chvalue,1);                		if(new_page == 1){		                	for(i = min_option; i <= max_option; i++){				                j = ((curr_pos-1)/max_option)*max_option + i;;	
		        if(sysdata.prbmode == 0)
		        {
			        sprintf(strbuf,"%i.%s",i,tname2b(j-1));
		        }else{                                        
        		        sprintf(strbuf,"%i.%s",i,rname2b(j-1));
		        }
        		draw_label(&(pgprbset_items[i-1]),1);
		draw_label(&(pgprbset_items[i-1]),1);				        if((curr_pos-1) % max_option == (i-1))					        draw_label(&(pgprbset_items[i-1]),2);        			        }                                }else{	                		for(i = min_option; i <= max_option; i++) {        		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(pgprbset_items[i-1]),2);				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(pgprbset_items[i-1]),2);			                }		                }	;
	}
	return;
}
//select probe for channel setup
LABEL flash prblist_banner = {5,3,3,8,strbuf};
LABEL flash prblist_chvalue = {1,180,3,2,strbuf};
LABEL flash prblist_items[] = {
	{1, 10, 30, 10, strbuf},
	{1, 130,30, 10, strbuf},
	{1, 10, 40, 10, strbuf},
	{1, 130,40, 10, strbuf},
	{1, 10, 50, 10, strbuf},
	{1, 130,50, 10, strbuf}
};
//select channel's probe
void pgprblist_handler(unsigned char msg) {
	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
	static unsigned int curr_pos = 1;
	static unsigned int last_pos = 1;
	unsigned char min_option = 1;
	unsigned char max_option = sizeof(prblist_items)/sizeof(LABEL);
	unsigned char min_index = 1;
	unsigned char max_index = 24;
	unsigned char i,j;
	unsigned char new_page = 1;
	if(msg >= '0' && msg <= '9') {
		if((msg > '0') && (unsigned char)((msg-'0')*max_option) <= max_index)					{new_page = 1; curr_pos = (unsigned char)((msg-'1')*max_option)+1;};
		msg = 0xfe;
	}
	if(msg == 'C')
	{
		nextwin = 7;
		return;
	}
	if(msg == 'T' || msg == 'O')
	{                                      
	        if(sysdata.prbmode == 0)
	        {
		        sysdata.tid[curr_ch-1] = curr_pos - 1;
	        }else{
		        sysdata.rid[curr_ch-1] = curr_pos - 1;	        
	        }
		nextwin = 7;
		return;
	}
	if(msg == 0xff)
	{
		LCD_Cls();
		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
		draw_label(&prblist_banner, 1);
		curr_pos = 1;
		new_page = 1;
		msg = 0xfe;
	}
	if(msg == 'U') {
		last_pos = curr_pos;   curr_pos--;			if(curr_pos < min_index) curr_pos = max_index;			if(last_pos == curr_pos)  return;			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;};
		msg = 0xfe;
	}
	if(msg == 'D') {
		last_pos = curr_pos;	curr_pos++;			if(curr_pos > max_index) curr_pos = min_index;			if(last_pos == curr_pos)	return;			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;};
		msg = 0xfe;
	}              
	if(msg == '.'){
	        if(sysdata.prbmode == 0)
	        {
		        sysdata.tid[curr_ch-1] = 0xff;
	        }else{
		        sysdata.rid[curr_ch-1] = 0xff;	        
	        }	        
       		msg = 0xfe;
	}
	if(msg == 0xfe) {
		sprintf(strbuf,"%i",curr_pos);draw_label(&prblist_chvalue,1);                		if(new_page == 1){		                	for(i = min_option; i <= max_option; i++){				                j = ((curr_pos-1)/max_option)*max_option + i;;  
		        if(sysdata.prbmode == 0)
		        {	
		                sprintf(strbuf,"%s",tname2b(j-1));
		        }else{
        		        sprintf(strbuf,"%s",rname2b(j-1));
        		}
			draw_label(&(prblist_items[i-1]),1);
		draw_label(&(prblist_items[i-1]),1);				        if((curr_pos-1) % max_option == (i-1))					        draw_label(&(prblist_items[i-1]),2);        			        }                                }else{	                		for(i = min_option; i <= max_option; i++) {        		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(prblist_items[i-1]),2);				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(prblist_items[i-1]),2);			                }		                }	;
	}
	return;
}
//select probe type list
LABEL flash tplist_banner = {5,3,3,8,strbuf};
LABEL flash tplist_options[] = {
	{1,10,20,8,"1.PT100"},
	{1,80,20,8,"2.PT25"},
	{1,10,30,8,"3.K-TYPE"},
	{1,80,30,8,"4.N-TYPE"},
	{1,150,30,8,"5.E-TYPE"},
	{1,10,40,8,"6.B-TYPE"},
	{1,80,40,8,"7.J-TYPE"},
	{1,150,40,8,"8.S-TYPE"},
	{1,10,50,8,"9.R-TYPE"}
};                            
//select probe type thermo
void pgprbtypelist_handler(unsigned char msg) {
	static int curr_sel = 1;
	static int last_sel = 1;
	unsigned char min_option = 1;
	unsigned char max_option = sizeof(tplist_options)/sizeof(LABEL);
	unsigned char i;
	if(msg >= '1' && msg <= '9') {
	        if(sysdata.prbmode == 0)
	        {
        		switch(msg)
	        	{
		        	case '1':
        			       return;
	        		case '2':
			               return;
        			default:
	        		       tprbdata.type[curr_prb-1] = msg-'3'+0x03;
		        	       break;
        		}            
        	}else{
        		switch(msg)
	        	{
		        	case '1':
			               rprbdata.type[curr_prb-1] = 0xf1;
        			       break;
	        		case '2':
		        	       rprbdata.type[curr_prb-1] = 0xf2;
			               break;
        			default:
                                       return; 
        		}            
        	}
	        msg = 'O';
	}
	if(msg == 'T' || msg == 'C' || msg == 'O') {
		nextwin = 11;
		return;
	}
	if(msg == 0xff)
	{
		LCD_Cls();
		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
		draw_label(&tplist_banner, 1);
		for(i = min_option;i <= max_option; i++) {                                        draw_label(&tplist_options[i-1], 1);			        }	;
		msg = 0xfe;
	}
	if(msg == 'U') {
		last_sel = curr_sel;			curr_sel--;			if(curr_sel < min_option)				curr_sel = max_option;;
		msg = 0xfe;
	}
	if(msg == 'D') {
		last_sel = curr_sel;			curr_sel++;			if(curr_sel > max_option)				curr_sel = min_option;;
		msg = 0xfe;
	}
	if(msg == 0xfe) {
		if(last_sel == curr_sel)                                        return;        			for(i = min_option;i <= max_option; i++){	        			if(last_sel == i)       draw_label(&tplist_options[i-1], 2);        				if(curr_sel == i)	draw_label(&tplist_options[i-1], 2);        			};
	}
	return;
}
/*
 *       Convert the probe type const to string
 */
char PRBSTR[7];
char* getprbtype(unsigned char prbtype)
{
	switch(prbtype)
	{
		case 0xf2: 	
		        sprintf(PRBSTR,"PT  25");
		        return PRBSTR;
		case 0xf1:
		        sprintf(PRBSTR,"PT 100");
		        return PRBSTR;
		case 0x03:
		        sprintf(PRBSTR,"K TYPE");
		        return PRBSTR;
		case 0x04:
		        sprintf(PRBSTR,"N TYPE");
		        return PRBSTR;
		case 0x05:
		        sprintf(PRBSTR,"E TYPE");
		        return PRBSTR;
		case 0x06:
		        sprintf(PRBSTR,"B TYPE");
		        return PRBSTR;
		case 0x07:
		        sprintf(PRBSTR,"J TYPE");
		        return PRBSTR;
		case 0x08:
		        sprintf(PRBSTR,"S TYPE");
		        return PRBSTR;
		case 0x09:
		        sprintf(PRBSTR,"R TYPE");
		        return PRBSTR;
	}
		        sprintf(PRBSTR,"? TYPE");
		        return PRBSTR;
}
//config r probe parameter
LABEL flash pgprbcfg_banner = {4,3,3,8,strbuf};
LABEL flash snlbl = {4,10,17,4,"1.序列号"};
LABEL flash snval = {1,75,22,10,strbuf};
LABEL flash typelbl = {4,10,31,3,"2.类型"};
LABEL flash typeval = {1,59,36,10,strbuf};
LABEL flash paramlbl1 = {4,130,31,3,"3.a"};
LABEL flash paramval1 = {1,165,36,10,strbuf};
LABEL flash paramlbl2 = {4,10,45,3,"4.b"};
LABEL flash paramval2 = {1,45,50,10,strbuf};
LABEL flash paramlbl3 = {4,130,45,3,"5.c"};
LABEL flash paramval3 = {1,165,50,10,strbuf};
//configuration of BORE probe parameter
void pgprbconfig_handler(unsigned char msg) {
	if(msg == 'T' || msg == 'O') {
		nextwin = 10;
		return;
	}
	if(msg == 'C') {
		nextwin = 10;
		return;
	}
	if(msg == 0xff)
	{
		LCD_Cls();                       
		if(sysdata.prbmode == 0)
		{
		        sprintf(strbuf,"热电偶探头%i参数",curr_prb);
        		draw_label(&pgprbcfg_banner,1);
	        	//name            
		        strbuf[0]=':';
        		sprintf(strbuf,":%s",tname2b(curr_prb-1));
	        	draw_label(&snlbl,1);draw_label(&snval,1);
        		//type
	        	sprintf(strbuf,":%s",getprbtype(tprbdata.type[curr_prb-1]));
		        draw_label(&typelbl,1);draw_label(&typeval,1);
		}else{
		        sprintf(strbuf,"铂电阻探头%i参数",curr_prb);
        		draw_label(&pgprbcfg_banner,1);
	        	//name            
		        strbuf[0]=':';
        		sprintf(strbuf,":%s",rname2b(curr_prb-1));
	        	draw_label(&snlbl,1);draw_label(&snval,1);
        		//type
	        	sprintf(strbuf,":%s",getprbtype(rprbdata.type[curr_prb-1]));
		        draw_label(&typelbl,1);draw_label(&typeval,1);
        		//param1
        		sprintf(strbuf,":%7f",rprbdata.param1[curr_prb-1]);
		        draw_label(&paramlbl1,1);draw_label(&paramval1,1);
        		//param2
	        	sprintf(strbuf,":%7f",rprbdata.param2[curr_prb-1]);
        		draw_label(&paramlbl2,1);draw_label(&paramval2,1);
	        	//param3
		        sprintf(strbuf,":%7f",rprbdata.param3[curr_prb-1]);
        		draw_label(&paramlbl3,1);draw_label(&paramval3,1);
                }
		return;
	}
	if(msg == '1') {
		max_databuf = 7;
		sprintf(strbuf,"请输入探头序列号");          
		prbsninput();
		nextwin = 11;
		return;
	}
	if(msg == '2'){
		nextwin = 9;
		return;
	}
	if(msg == '3' ){ 
	        if(sysdata.prbmode == 1)
	        {
        		max_databuf = 8;
	        	sprintf(strbuf,"输入系数a");
		        rprbdata.param1[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
        		nextwin = 11;
        	}
		return;
	}
	if(msg == '4') {   
        	if(sysdata.prbmode == 1) 
        	{
        		max_databuf = 8;
	        	sprintf(strbuf,"输入系数b");
		        rprbdata.param2[curr_prb-1] = wnd_floatinput(rprbdata.param2[curr_prb-1]);
        		nextwin = 11;
                }
		return;
	}
	if(msg == '5') {     
	        if(sysdata.prbmode == 1)
	        {
        		max_databuf = 8;
		        sprintf(strbuf,"输入系数c");
        		rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param3[curr_prb-1]);
		        nextwin = 11;
	        }
		return;
	}
	return;
}
LABEL flash boot_banner = {5,3,3,7,"请选择工作模式"};
LABEL flash boot_options[] = {
 	{5, 10,23,7,"1.铂电阻测量"},
 	{5, 130,23,7,"2.铂电阻配置"},
 	{5, 10,45,7,"3.热电偶测量"},
 	{5, 130,45,7,"4.热电偶配置"}
};       
//boot up menu
void pgboottype_handler(unsigned char msg) {
	unsigned char i;
	static unsigned char curr_sel = 1;
	static unsigned char last_sel = 1;
	unsigned char min_option = 1;
	unsigned char max_option = sizeof(boot_options)/sizeof(LABEL);
	if(msg == 0xff) {
		LCD_Cls();
		draw_label(&boot_banner, 1);
		for(i = min_option;i <= max_option; i++) {                                        draw_label(&boot_options[i-1], 1);			        }	;
		msg = 0xfe;
	}
	if(msg == '1') {
		sysdata.prbmode = 1; scanner_set_mode(); display_buttons('a',0);
		display_buttons('j',0);
		nextwin = 2;
		return;
	}
	if(msg == '2') {
		sysdata.prbmode = 1; scanner_set_mode(); display_buttons('a',0);
		display_buttons('j',0);
		nextwin = 3;
		return;
	}
	if(msg == '3') {
		sysdata.prbmode = 0; scanner_set_mode(); display_buttons('a',1);
		display_buttons('j',1);
		nextwin = 2;
		return;
	}
	if(msg == '4') {
		sysdata.prbmode = 0; scanner_set_mode(); display_buttons('a',1);
		display_buttons('j',1);
		nextwin = 4;
		return;
	}
	if(msg == 0xfe) {
		if(last_sel == curr_sel)                                        return;        			for(i = min_option;i <= max_option; i++){	        			if(last_sel == i)       draw_label(&boot_options[i-1], 2);        				if(curr_sel == i)	draw_label(&boot_options[i-1], 2);        			};
	}
}
