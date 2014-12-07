#include <string.h>
#include "font.h"
#include "lcd.h"
//#include "sjDefine.h"
#include "sjSerial.h"

void sleepms(unsigned int i)
{
   while(i-- > 0)
      ;
      
}
#define MARK_HEAD        0xAA
#define MARK_TAIL        0x55


extern void sjSerialSendByte2(u8);
extern void io_putc(unsigned char);
void prints(u8 *str, u8 length, char uart_port)
{
	if(uart_port == PORT_KBD)
	{
		while(length-- > 0){    
        	sjSerialSendByte2(*str++);            
    	}
	}
	if(uart_port == PORT_NAV)
	{
		while(length-- > 0){    
        	io_putc(*str++);            
    	}
	}
}


uchar key;             
static uchar kbd_state;
void kbd_uart_push(uchar dat)
{           
        if(dat == STATUS_IDLE||
           dat == STATUS_ERR||
           dat == STATUS_DRAW||
           dat == STATUS_DONE) //lcd acknowledge
        {
                kbd_state = dat;
                return;
        }  
        if(key == KEY_INVALID)                              
                key = dat;
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
//     if(DEBUG == 1)        return;
     while(kbd_state != STATUS_IDLE)
     {
	 	if(sjSerialIsDataWaiting2())
		{
			kbd_uart_push(sjSerialWaitForOneByte2());
		}
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
 	 	if(sjSerialIsDataWaiting2())
		{
			kbd_uart_push(sjSerialWaitForOneByte2());
		}
         if(kbd_state == STATUS_ERR)
         {
                onecmd(len);     
                timeout = 0;
         }
         if(timeout < 655350)
         {
                sleepms(2);
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
