#include <string.h>

#include <mega64.h>
#include "lcd.h"
#include "font.h"
#include "global.h"
#include "utili.h"       

#define LCD_DATA_BUS_OUT        PORTB     
#define LCD_DATA_SET_IN         DDRB=0x00;
#define LCD_DATA_SET_OUT        DDRB=0xff;
#define LCD_DATA_BUS_IN         PINB
#define LCD_WR			PORTD.0
#define LCD_RD			PORTD.1
#define LCD_CE			PORTD.4
#define LCD_CD			PORTD.5
#define LCD_RESET		PORTD.6
#define LCD_FS			PORTD.7
#define LCD_DELAY               3                      
uchar LCD_BusyStatus (uchar mask)    //测状态
{
	uchar dat= 0;
	

	LCD_DATA_SET_OUT;
	LCD_CE = 0;
	LCD_CD = 1;
	LCD_RD = 1;
	LCD_WR = 1;
        LCD_DATA_BUS_OUT = 0xff;	
       	LCD_DATA_SET_IN;
 	LCD_RD = 0;
        sleepms(LCD_DELAY);
       	dat=LCD_DATA_BUS_IN;
       	LCD_RD = 1;
  	return dat;
}

/* 数据,指令读写判别 until STA0 = 1 AND STA1 =1 for normal command*/
void LCD_CheckBusy1(void)  
{                                 
        while((3 & LCD_BusyStatus(3)) != 3) {};
}
/* 数据,指令读写判别 until STA3 =1 for auto command*/
void LCD_CheckBusy3(void)  
{                         
        while((8 & LCD_BusyStatus(8)) != 8) {};
}

/*------------写命令或数据到LCD--------------*/
void LCD_Write1 (uchar dat,uchar comm)       //写一个数据和一个命令
{
  	LCD_Data(dat);
  	LCD_Comm(comm);
}
void LCD_Write2 (uchar datl,uchar dath,uchar comm)  //写两个数据和一个命令
{
  	LCD_Data(datl);
  	LCD_Data(dath);
  	LCD_Comm(comm);
}
void LCD_WriteInt (uint dat,uchar comm)       //写一个16进制数据和一个命令
{
  	uchar datl,dath;
  	datl=(dat&0xff);
  	dath=dat>>8;
  	LCD_Data(datl);
  	LCD_Data(dath);
  	LCD_Comm(comm);
}
void LCD_AutoWrite (uchar dat)               //自动写数据
{
       	LCD_CE = 0;
  	LCD_CheckBusy3();
  	LCD_DATA_SET_OUT;
  	LCD_CD = 0;
  	LCD_RD = 1;
       	LCD_DATA_BUS_OUT=dat;  	
  	LCD_WR = 0;
	sleepms(LCD_DELAY);
	LCD_WR = 1;

}
/*	读数据	*/
uchar LCD_Read(void) 
{
	uchar dat;
	LCD_CheckBusy1();
	LCD_DATA_SET_IN;
	LCD_CE = 0;
	LCD_WR = 1;
	LCD_RD = 0;
	LCD_CD = 0;
	sleepms(LCD_DELAY);
	dat = LCD_DATA_BUS_IN;
	LCD_RD = 1;
	LCD_CD = 1;
	return dat;
}
/*	自动读数据	

uchar LCD_AutoRead(void) 
{
	uchar dat;
  	LCD_CE = 0;
	LCD_CheckBusy2();
	LCD_CD=0;
	LCD_RD=0;          
	LCD_DATE_SET_IN;
	dat=LCD_DATA_BUS_IN;
	LCD_RD=1;
	LCD_CD=1;
	return dat;
}
*/
void LCD_Comm (uchar comm)       //写命令
{       
        LCD_CE = 0;
  	LCD_CheckBusy1();
  	LCD_DATA_SET_OUT;                        
  	LCD_CD = 1;
        LCD_RD = 1;
        
       	LCD_DATA_BUS_OUT=comm;                       
       	LCD_WR = 0;
	sleepms(LCD_DELAY);
	LCD_WR = 1;
}
void LCD_Data (uchar dat)       //写数据
{
  	LCD_CE = 0;
  	LCD_CheckBusy1();
  	LCD_DATA_SET_OUT;  

  	LCD_CD = 0;
        LCD_RD = 1;
       	LCD_DATA_BUS_OUT=dat;  	
       	LCD_WR = 0;
	sleepms(LCD_DELAY);
	LCD_WR = 1;
  	
}

/*------------------初始化-----------------*/
void LCD_Init (void) 
{
	LCD_DATA_SET_OUT;
	LCD_CE = 1;
	LCD_WR = 1;
	LCD_RD = 1;
	//LCD_RESET = 0;
	//sleepms(2000);
        //LCD_RESET = 1;        
	//sleepms(2000);
	LCD_FS = 0;
	        
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR,LC_TXT_STP); 	//文本显示区首地址
  	LCD_WriteInt(LCD_GRAPH_HOME_ADDR,LC_GRH_STP); //图形显示区首地址
  	LCD_Write2(LCD_WIDTH,0x00,LC_TXT_WID);        //文本显示区宽度
  	LCD_Write2(LCD_WIDTH,0x00,LC_GRH_WID);        //图形显示区宽度
  	LCD_Comm(LC_MOD_XOR);                        	//逻辑"异或"
  	LCD_Write2(0x02,0x00,LC_CGR_POS);             //CGRAM偏置地址设置
  	LCD_Comm(0x9c);                        	//启用文本显示,启用图形显示 /光标禁用
  	//LCD_Comm(0x98);                        	//禁用文本显示,启用图形显示
  	
  	LCD_Cls();
}

/********************************************************/
/* 清屏							*/
/********************************************************/
void LCD_Cls(void) 
{      

	u16  i;
	//LCD_Write2(0x00,0x00,LC_ADD_POS);// 置地址指针为从零开始
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR,LC_ADD_POS);
	LCD_Comm(LC_AUT_WR);		// 自动写
//	LCD_CheckBusy1();
	for(i=0;i< (LCD_HEIGHT+8)*LCD_WIDTH;i++)	// 清一屏
	{
		LCD_AutoWrite(0x00);		// 写数据0x00
	}
//	LCD_CheckBusy3();	
	LCD_Comm(LC_AUT_OVR);		// 自动写结束
	LCD_Write2(0x00,0x00,LC_ADD_POS);// 重置地址指针
}

void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2)
{
	u32 addr;
	uchar h,w;
  	uchar i,j;
	
	//x坐标要8字节对齐
	x1 =(x1 / 8) * 8;
	x2 =(x2 / 8) * 8;
	
	h = y2 - y1;
	w = (x2 - x1)/8;
	
	addr = LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y1 + x1/8;
	
  	for(j=0;j<h;j++)
  	{
  		LCD_WriteInt(addr,LC_ADD_POS);
		LCD_Comm(LC_AUT_WR);
  		LCD_CheckBusy1();
    		for(i=0;i<w;i++)
    		{
	 		LCD_AutoWrite(0x00);
    		}                
    		LCD_CheckBusy3();
		LCD_Comm(LC_AUT_OVR);
		addr += LCD_WIDTH;
  	}
}


/*	设定图形x,y值*/
/*
void LCD_GraphSetAddr(uchar x,uchar y) 
{
	u32 xy;
	xy=y;
	xy=xy*16+x+256;
  	//LCD_CE = 0;
	LCD_Write2(xy&0xff,xy/256,0x24);
}
*/
/*	设定文本x,y值	*/
/*
void LCD_TextSetAddr(uchar x,uchar y) 
{
  	//LCD_CE = 0;
	LCD_Write2(y*16+x,0,0x24);
}
*/
/*	清除一点*/
/*
void LCD_ClrPixel(uchar x,uchar y) 
{
	uchar b;
	u32 addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;

	b = 7 - (x % 8);
	
	LCD_WriteInt(addr,0x24);
	LCD_Comm(0xf0|b);
}
*/
/*	点亮一点	*/

void LCD_Putpixel(uchar x,uchar y) 
{
	uchar b;
	u32 addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;

	b = 7 - (x % 8);
	
	LCD_WriteInt(addr,LC_ADD_POS);
	LCD_Comm(0xf8|b);  //set pixel
}

/*	x,y处显示光标	*/

void LCD_ShowCursor(uchar x,uchar y) 
{
  	//LCD_CE = 0;
	LCD_Comm(0x97);	//光标启用
	LCD_Write2(x,y,LC_CUR_POS);
}

/*	取消光标	*/

void LCD_HideCursor(void)  
{	
  	//LCD_CE = 0;
	LCD_Comm(0x9c);	
}

void LCD_PutImg(uchar x,uchar y,uchar w,uchar h,flash uchar *img) 
{
	u32 addr;
  	uchar i,j;
  	uchar c;
	      
	addr = LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y + (x / 8);
  	//LCD_CE = 0;
  	for(j=0;j<h;j++)
  	{
  		LCD_WriteInt(addr,LC_ADD_POS);
		LCD_Comm(LC_AUT_WR);
		LCD_CheckBusy1();
    		for(i=0;i<w;i++)
    		{
    			c = img[j*w+i] ;
	 		LCD_AutoWrite(c);
   				
    		}
		LCD_CheckBusy3();
		LCD_Comm(LC_AUT_OVR);
		addr += LCD_WIDTH;
  	}
}
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
	u32 addr;
  	uchar i,j;
  	uchar c;
	
	x =(x / 8) * 8;
	addr = LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y + x/8;
	
  	for(j=0;j<h;j++)
  	{
    		for(i=0;i<w;i++)
    		{
	  		LCD_WriteInt(addr +i,LC_ADD_POS);	
	  		LCD_Comm(LC_NOC_RD);
	  		c = LCD_Read();
	  		c = ~c;

      			LCD_Write1(c,LC_NOC_WR);
    		}
		addr += LCD_WIDTH;
  	}
}
/*
void LCD_PrintWord(uchar x,uchar y,uint16 n,uchar start) 
{
	uchar buf[4];
	int i;
	
	if(start > 3) start = 0;
	
	for(i = 0; i < 4; i++)
	{
		buf[i] = n % 10;   
		n /= 10;
	}
	x =(x / 8) * 8;
	
	for(i = 3-start; i >= 0; i--)
	{
		LCD_PutImg(x,y,1,11,Num_Tab + buf[i] * 11);
		
		x = x + 8;
	}
}
void LCD_PrintHex(uchar x,uchar y,uchar hex) 
{
	uchar c1,c2;
	//低4位
	c1 = hex & 0x0f;	//lo
	//高4位
	c2 = (hex >> 4) & 0x0f; //hi

	LCD_PutImg(x,y,1,11,Num_Tab + c2 * 11);
	LCD_PutImg(x+8,y,1,11,Num_Tab + c1 * 11);
}

void LCD_GrapPutchar(uchar x,uchar y,uchar num) 
{
	uchar i,j;
	uchar dot;
	num = (num- '0')*9;
  	//LCD_CE = 0;
	for(i=0; i< 12;i++)
	{
		dot = Num_Tab[num + i];
		for(j = 0; j < 7; j++)
		{
			if( dot & 0x80)
				LCD_Putpixel(x+j,y);
			else
				LCD_ClrPixel(x+j,y);
				
			dot <<= 1;
		}
		y++;
	}
}

void LCD_GrapPrint(uchar x,uchar y,uchar code *s) 
{
	while(*s)
	{
		LCD_GrapPutchar(x,y,*s);
		x += 8;
		s++;
	}
}
*/
/*--------------显示字符------------------*/
/*
void LCD_TextPutchar(uchar x,uchar y,uchar c) 
{
  	//LCD_CE = 0;
  	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
  	LCD_Comm(0xb0);
	LCD_AutoWrite(c - 0x20);
  	LCD_Comm(0xb2);
}

void LCD_TextPrint(uchar x,uchar y,char *s) 
{
  	//LCD_CE = 0;
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

  	//LCD_CE = 0;
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

void LCD_LineH(uchar y) 
{
	char i;
	
  	//LCD_CE = 0;
  	LCD_WriteInt(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y ,0x24);
	LCD_Comm(LC_AUT_WR);
	for(i=0;i<LCD_WIDTH;i++)
 		LCD_AutoWrite(0xff);
	LCD_Comm(LC_AUT_OVR);
}
void LCD_LineV(uchar x,uchar y1,uchar y2) 
{
	int i;
  	//LCD_CE = 0;
	for(i = y1; i < y2; i++)
	{
		LCD_Putpixel(x,i);		
	}
	
}


void LCD_LineXX(uchar x1,uchar x2,uchar y)
{
	int i;
	for(i = x1; i < x2; i++)
	{
		LCD_Putpixel(i,y);		
	}
}

void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2)
{
	LCD_LineXX(x1,x2,y1);
	LCD_LineXX(x1,x2,y2);
	LCD_LineV(x1,y1,y2);
	LCD_LineV(x2,y1,y2);
}



FNT_GB12 flash *GetHzk12(uchar c1,uchar c2)
{
	int i = 0;

	while(HZK12[i].Index[0] != 0)
	{
		if(c1 == HZK12[i].Index[0] && c2 == HZK12[i].Index[1])
			return &HZK12[i];
		i ++;
	}
	return NULL;
}


FNT_GB16 flash *GetHzk16(uchar c1,uchar c2)
{
	int i = 0;

	while(HZK16[i].Index[0] != 0)
	{
		if(c1 == HZK16[i].Index[0] && c2 == HZK16[i].Index[1])
			return &HZK16[i];
		i ++;
	}
	return NULL;
}

void LCD_PrintHz12(uchar x,uchar y,uchar *s)
{
	uchar c1,c2;
	//x坐标必需是8位对齐
	x =(x / 8) * 8;
	while(*s)
	{
		c1 = *s++;
		if(c1 == ' ')
			x += 8;
		else if( c1 > 0x80)
		{
			//汉字
			c2 = *s++;
			LCD_PutImg(x,y+2,2,12,GetHzk12(c1,c2)->Msk);
			x += 16;	
		}
		else	//ASCII
		{
			LCD_PutImg(x,y,1,16,ASC8x16[c1]);	//-
			x += 8;
		}
	}
}


void LCD_PrintHz16(uchar x,uchar y,uchar *s)
{
	uchar c1,c2;
	//x坐标必需是8位对齐
	x =(x / 8) * 8;
	while(*s)
	{
		c1 = *s++;
		if(c1 == ' ')
		{
			LCD_PutImg(x,y,1,16,ASC8x16[' ']);
			LCD_PutImg(x+8,y,1,16,ASC8x16[' ']);
			x += 16;
		}
		else if( c1 > 0x80)
		{
			//汉字
			c2=*s++;
			LCD_PutImg(x,y,2,16,GetHzk16(c1,c2)->Msk);
			x += 16;	
		}
		else	//ASCII
		{
			LCD_PutImg(x,y+2,1,16,ASC8x16[c1]);	//-
			x += 8;
		}
	}
}

//显示6x8的数字
void LCD_Print6X8(uchar x, uchar y,uchar *s)
{
	//x坐标必需是8位对齐
	x =(x / 8) * 8;
	while(*s)
	{
		LCD_PutImg(x,y,1,8,font6x8[*s - 32]);	//-
		x += 8;

		s ++;
	}
}

//显示6x8的数字
void LCD_Print8X16(uchar x, uchar y,uchar *s)
{
	//x坐标必需是8位对齐
	x =(x / 8) * 8;
	while(*s)
	{
		LCD_PutImg(x,y,1,16,ASC8x16[*s]);	//-
		x += 8;

		s ++;
	}
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


