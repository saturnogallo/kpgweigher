// CodeVisionAVR C Compiler
// (C) 1998-2005 Pavel Haiduc, HP InfoTech S.R.L.
// Prototypes for string functions
#pragma used+
char *strcat(char *str1,char *str2);
char *strcatf(char *str1,char flash *str2);
char *strchr(char *str,char c);
signed char strcmp(char *str1,char *str2);
signed char strcmpf(char *str1,char flash *str2);
char *strcpy(char *dest,char *src);
char *strcpyf(char *dest,char flash *src);
unsigned char strcspn(char *str,char *set);
unsigned char strcspnf(char *str,char flash *set);
unsigned int strlenf(char flash *str);
char *strncat(char *str1,char *str2,unsigned char n);
char *strncatf(char *str1,char flash *str2,unsigned char n);
signed char strncmp(char *str1,char *str2,unsigned char n);
signed char strncmpf(char *str1,char flash *str2,unsigned char n);
char *strncpy(char *dest,char *src,unsigned char n);
char *strncpyf(char *dest,char flash *src,unsigned char n);
char *strpbrk(char *str,char *set);
char *strpbrkf(char *str,char flash *set);
signed char strpos(char *str,char c);
char *strrchr(char *str,char c);
char *strrpbrk(char *str,char *set);
char *strrpbrkf(char *str,char flash *set);
signed char strrpos(char *str,char c);
char *strstr(char *str1,char *str2);
char *strstrf(char *str1,char flash *str2);
unsigned char strspn(char *str,char *set);
unsigned char strspnf(char *str,char flash *set);
char *strtok(char *str1,char flash *str2);
 unsigned int strlen(char *str);
void *memccpy(void *dest,void *src,char c,unsigned n);
void *memchr(void *buf,unsigned char c,unsigned n);
signed char memcmp(void *buf1,void *buf2,unsigned n);
signed char memcmpf(void *buf1,void flash *buf2,unsigned n);
void *memcpy(void *dest,void *src,unsigned n);
void *memcpyf(void *dest,void flash *src,unsigned n);
void *memmove(void *dest,void *src,unsigned n);
void *memset(void *buf,unsigned char c,unsigned n);
#pragma used-
#pragma library string.lib
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
typedef flash struct typFNT_GB24		// 汉字字模数据结构
typedef flash struct typFNT_GB24		{
	unsigned char Index[4];
	unsigned char Msk[72];
}FNT_GB24;
typedef flash struct typFNT_GB16		// 汉字字模数据结构
{
	unsigned char Index[4];
	unsigned char Msk[32];
}FNT_GB16;
typedef flash struct typFNT_GB12		// 汉字字模数据结构
{
	unsigned char Index[4];
	unsigned char Msk[24];
}FNT_GB12;
//字库
extern FNT_GB12 flash HZK12[];
extern FNT_GB16 flash HZK16[];
extern unsigned char flash ASC8x16[256][16];
extern unsigned char flash Font24X32[13][96];
extern unsigned char flash font6x8[256][8];
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
	double rtp[24];
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
extern unsigned char key;             
static unsigned char kbd_state;
void kbd_uart_push(unsigned char data)
{           
        if(data == 0xF0||
           data == 0xF1||
           data == 0xF2||
           data == 0xF3) //lcd acknowledge
        {
                kbd_state = data;
                return;
        }  
        if(key == '-'													  )                              
                key = data;
}  
static unsigned char lcdcmd[40];
static unsigned char head[3] = {0xAA,0xAA,0xAA};
static unsigned char tail[3] = {0x55,0x55,0x55};
static unsigned char hex2ch[2];  
void hex2char(unsigned char a)
{
        unsigned char h,l;
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
void onecmd(unsigned char len)
{    
     unsigned char i = 0;
     unsigned char sum;
     sum = 0;
     prints(head,3,4); //send head
     for(i = 0;i<len;i++)     //caculate cksum     
     {              
        hex2char(lcdcmd[i]);
	prints(hex2ch,2,4);
        sum = sum + lcdcmd[i];
     }                        
     lcdcmd[i] = 0xff - sum;
     hex2char(lcdcmd[i]);
     prints(hex2ch,2,4);
     prints(tail,3,4); //send tail      
}
static unsigned char idle[10] = {0xAA,0xAA,0xAA,'1','2','E','D',0x55,0x55,0x55};
void sendcmd(unsigned char len)
{
     unsigned long timeout;  
     if(0 == 1)
        return;
     while(kbd_state != 0xF0)
     {
        if(kbd_state == 0xF2)
                continue;
        if(kbd_state == 0xF1)
        {
                prints(idle,10,4);
                kbd_state = 0xF2;     
        }   
     }                                  
     onecmd(len);  
     timeout = 0;
     kbd_state = 0xF3;
     while(kbd_state != 0xF0)
     {
         if(kbd_state == 0xF1)
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
        kbd_state = 0xF0;
        lcdcmd[0] = 0x03;
        sendcmd(1);
        //do it twice avoid the missing of first command on boot up
        lcdcmd[0] = 0x03;
        sendcmd(1);
        }
/********************************************************/
/* 清屏							*/
/********************************************************/
void LCD_Cls(void) 
{           
        lcdcmd[0] = 0x04;
        sendcmd(1);
}
void LCD_ClsBlock(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
        lcdcmd[0] = 0x06;
        lcdcmd[1] = x1;
        lcdcmd[2] = y1;
        lcdcmd[3] = x2;
        lcdcmd[4] = y2;                
        sendcmd(5);
}
void display_buttons(unsigned char btn_pos,unsigned char btn_val)   //send led status to sub board
{              
        lcdcmd[0] = 0x02;
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
void LCD_ReverseRect(unsigned char x,unsigned char y,unsigned char w,unsigned char h)
{
        lcdcmd[0] = 0x05;
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
void LCD_Rectange(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
        lcdcmd[0] = 0x07;
        lcdcmd[1] = x1;
        lcdcmd[2] = y1;
        lcdcmd[3] = x2;
        lcdcmd[4] = y2;                
        sendcmd(5);
}                        
void LCD_PrintChar(unsigned char cmd, unsigned char x,unsigned char y,unsigned char *s)
{                
        unsigned char pos = 0;        
        lcdcmd[0] = cmd;
        lcdcmd[1] = x;
        lcdcmd[2] = y;
        pos = 1;
        while(*s)
        {      
                lcdcmd[(unsigned char)(pos + 3)] = *s++;
                pos = pos + 1;
        }      
        lcdcmd[(unsigned char)(pos+3)] = 0x00;
        lcdcmd[3] = pos;
        pos = pos + 4;
        sendcmd(pos);
}
void LCD_PrintHz12(unsigned char x,unsigned char y,unsigned char *s)
{                     
        LCD_PrintChar(0x08,x,y,s);
}
void LCD_PrintHz16(unsigned char x,unsigned char y,unsigned char *s)
{
        LCD_PrintChar(0x09,x,y,s);
}
//显示6x8的数字
void LCD_Print6X8(unsigned char x, unsigned char y,unsigned char *s)
{                                  
        LCD_PrintChar(0x10,x,y,s);
}
//显示6x8的数字
void LCD_Print8X16(unsigned char x, unsigned char y,unsigned char *s)
{                                
        LCD_PrintChar(0x11,x,y,s);
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
