//#define NULL 0x0
// global.h    
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
MSG_HANDLER curr_window = 0;             
extern unsigned char key;
unsigned char databuf[12];
unsigned char pos_databuf; //position in data buffer
unsigned char max_databuf;
unsigned char data_sign;   // sign of the data
LABEL flash datalbl = {5,30,10,8,strbuf};
LABEL flash databox = {5,30,30,8,databuf};
void prbsninput()
{
	unsigned char msg,len; 
	databuf[0] = '\0';
	pos_databuf = 0;
	key = 0xff;
	while(1)
	{
		if(key == '-'													  )
		        continue;
		msg = key;
		if(msg == 0xff) {
			LCD_Cls();
			draw_label(&datalbl,1);
			sprintf(databuf,"");
			draw_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}
		if(msg == 'T')
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
		if(msg == 'C') {	                  
				key = '-'													  ;
			return;
		}
		if(msg == 'D') {
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
			msg = 0xfe;
		}
		if(msg == 'U') {
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
			msg = 0xfe;
		}
		if(msg >= '0' && msg <= '9') {
        		if(pos_databuf < max_databuf)
        		{
			        databuf[pos_databuf++] = msg;
        			databuf[pos_databuf] = '\0';
        		}
			msg = 0xfe;
		}
		if(msg == 'O') {
		        if(sysdata.prbmode == 0)
		        {
			        for(len = 0; len < pos_databuf; len++)
        		                tprbdata.name[curr_prb-1][len] = databuf[len];
        			tprbdata.name[curr_prb-1][pos_databuf] = '\0';         
        		}else{
			        for(len = 0; len < pos_databuf; len++)
        		                rprbdata.name[curr_prb-1][len] = databuf[len];
        			rprbdata.name[curr_prb-1][pos_databuf] = '\0';         
        		}
			key = '-'													  ;
			break;
		}
		if(msg == 0xfe) {
			draw_label(&databox,1);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}
				key = '-'													  ;
	}
//	LCD_HideCursor();
}
unsigned char wnd_intinput(unsigned char lastval)
{
	unsigned char msg;
		key = 0xff;
	databuf[0] = '\0';
	pos_databuf = 0;
        data_sign = 0;	
	while(1)
	{
		if(key != '-'													  )
		{
			msg = key;
					}else{
			continue;
		}
		if(msg == 0xff)
		{
			LCD_Cls();
			draw_label(&datalbl,1);
			draw_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}
		if(msg == 'T')
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
		if(msg == 'D') {
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}
			if(databuf[pos_databuf-1] == '0')
				databuf[pos_databuf-1] = '9';
			else
				databuf[pos_databuf-1] -= 1;
			msg = 0xfe;
		}
		if(msg == 'U') {
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}
			if(databuf[pos_databuf-1] == '9')
				databuf[pos_databuf-1] = '0';
			else
				databuf[pos_databuf-1] += 1;
			msg = 0xfe;
		}
		if(msg >= '0' && msg <= '9') {
        		if(pos_databuf < max_databuf)
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';
		        	msg = 0xfe;
		        }
		}
		if(msg == 'C') {
//			LCD_HideCursor();
                        key = '-'													  ;
			return lastval;
		}
		if(msg == 'O'){
//			LCD_HideCursor();
                        key = '-'													  ;
			return buf2byte();
		}
		if(msg == 0xfe) {
			draw_label(&databox,1);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}       
		key = '-'													  ;
	}
}
double wnd_floatinput(double lastval)
{
	unsigned char msg;
	databuf[0] = ' ';
        databuf[1] = '\0';
        pos_databuf = 1;
        data_sign = 0;	
	key = 0xff;
	while(1)
	{
		if(key != '-'													  )
		{
			msg = key;
			}else{
			continue;
		}
		if(msg == 0xff)
		{
			LCD_Cls();
			draw_label(&datalbl,1);
			draw_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}
		if(msg == 'T')
		{                                          
		        if(data_sign == 0)     
		        {
		                data_sign  = 1;
        			databuf[0] = '-';
		        }else{
		                data_sign  = 0;
        			databuf[0] = '+';
		        }
                        if(pos_databuf == 1){
	        	        databuf[pos_databuf] = '\0';
	        	}
		        msg = 0xfe;
		}
		if(msg == 'D') {
			if(pos_databuf == 1)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}
			if(databuf[pos_databuf-1] == '0')
				databuf[pos_databuf-1] = '9';
			else
				databuf[pos_databuf-1] -= 1;
			msg = 0xfe;
		}
		if(msg == 'U') {
			if(pos_databuf == 1)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}
			if(databuf[pos_databuf-1] == '9')
				databuf[pos_databuf-1] = '0';
			else
				databuf[pos_databuf-1] += 1;
			msg = 0xfe;
		}
		if((msg >= '0' && msg <= '9') || msg == '.') {
               		if(pos_databuf < max_databuf)
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';
		        	msg = 0xfe;
		        }
		}
		if(msg == 'C') {
//			LCD_HideCursor();
		key = '-'													  ;
			return lastval;
		}
		if(msg == 'O'){
				key = '-'													  ;
//			LCD_HideCursor();
			return buf2double();
		}
		if(msg == 0xfe) {
			draw_label(&databox,1);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}                                 
				key = '-'													  ;
	}
}
//display a message box
void wnd_msgbox(flash LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
	draw_label(lbl, 1);	
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);
}
void draw_inputbox(flash LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	draw_label(lbl, 1);	
}
void draw_label(flash LABEL *lbl,unsigned char reverse) {
	unsigned char len = lbl->width;
	if(lbl->type == 1) {
		if(reverse & 1) {
			if((reverse & 4) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 8);
			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
			return;			
		}
		if(reverse & 2)
			LCD_ReverseRect(lbl->x, lbl->y, len , 8);
		return;
	}
	if(lbl->type == 2) {
		if(reverse & 1) {
			if((reverse & 4) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 16);
			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
			return;
		}
		if(reverse & 2)
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
	if(lbl->type == 4) {
		if(reverse & 1) {
			if((reverse & 4) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & 2)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
		return;
	}
	if(lbl->type == 5) {
		if(reverse & 1) {
			if((reverse & 4) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 16);
			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & 2)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
		return;
	}
}
