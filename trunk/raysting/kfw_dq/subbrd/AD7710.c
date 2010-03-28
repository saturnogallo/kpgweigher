#include <stc51.h>
#include <stdio.h>
#include <intrins.h>
#include <ad7710.h>

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

uchar g_iAdc;
uchar g_iCurr;
uchar g_iPwr;

sbit sclk= P1^0;  //串行时钟端
sbit sdata= P1^1;   //串行数据线
sbit drdy= P1^2;    //逻辑输出端
sbit a0= P1^3;      //地址输入端
sbit rfs= P1^4;    //同步接受端
    sbit tfs= P1^5;    //同步传输端

	sbit dat= P0^1;   //定义数据线
    sbit clk= P0^0;   //定义时钟端

    sbit jdq1= P1^7;   //继电器XI
	sbit jdq2= P1^6;   //继电器X2

	sbit key_tl=  P0^2; //调零
    sbit key_xz=  P0^3; //校准

#define MD_NORMAL	0L<<21  //标准模式
#define MD_SELF_CAL	1L<<21  //自校准模式
#define MD_SYS_CAL_ZERO	2L<<21  //系统校准模式，零量程
#define MD_SYS_CAL_FULL 3L<<21  //系统校准模式，满量程
#define MD_SYS_OFFSET   4L<<21  //系统补偿模式
#define MD_BG_CAL	5L<<21  //背景校准模式
#define MD_RW_ZEROSCALE	6L<<21  //读写零量程差分模式
#define MD_RW_FULLSCALE 7L<<21  //读写满量程差分模式

#define PGA_1		0L<<18
#define PGA_2		1L<<18
#define PGA_4		2L<<18
#define PGA_8		3L<<18
#define PGA_16		4L<<18
#define PGA_32		5L<<18
#define PGA_64		6L<<18
#define PGA_128		7L<<18

#define CH_0		0L<<17  //通道1
#define CH_1		1L<<17  //通道2

#define PD_NORMAL	0L<<16  //标准模式
#define PD_DOWN		1L<<16  //节能模式

#define WL_16		0L<<15  //16位模式
#define WL_24		1L<<15  //24帷模式

#define IO_OFF		0L<<14  //补偿电流关
#define IO_ON		1L<<14  //补偿电流开

#define BO_OFF		0L<<13  //熔断电流关
#define BO_ON		1L<<13  //熔断电流开


#define BU_BIPOLAR	0L<<12  //双极测量
#define BU_UNIPOLAR	1L<<12  //单极测量



   	ulong de;           //测量数据全局变量传递


   	   uchar  dejs;
	   uchar  dedjs;
       unsigned long  dd   ;
       uchar  xdata da[5];

  
ulong code syscfg[] = {	(MD_SELF_CAL)+(PGA_1) +(CH_0)+(PD_NORMAL)+(WL_24)+(IO_OFF)+(BO_OFF)+(BU_BIPOLAR)+1984,
						(MD_SELF_CAL)+(PGA_8) +(CH_0)+(PD_NORMAL)+(WL_24)+(IO_OFF)+(BO_OFF)+(BU_BIPOLAR)+1984,
						(MD_SELF_CAL)+(PGA_64)+(CH_0)+(PD_NORMAL)+(WL_24)+(IO_OFF)+(BO_OFF)+(BU_BIPOLAR)+1984,
                        (MD_SYS_OFFSET)+(PGA_1)+(CH_0)+(PD_NORMAL)+(WL_24)+(IO_OFF)+(BO_OFF)+(BU_BIPOLAR)+1984,
                        (MD_SYS_OFFSET)+(PGA_8) +(CH_0)+(PD_NORMAL)+(WL_24)+(IO_OFF)+(BO_OFF)+(BU_BIPOLAR)+1984,
						(MD_SYS_OFFSET)+(PGA_64)+(CH_0)+(PD_NORMAL)+(WL_24)+(IO_OFF)+(BO_OFF)+(BU_BIPOLAR)+1984,
                         };//0x03e304,0x03e334,0x03e31c};

 #define N 17//N值可根据实际情况调整

uchar code rrc[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,};

/************延时程序**************/
void delay (uint us) 
{
  	while(us--);
}
void delay1 (uint ms) 
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;		
}

void write(ulong j)
	   {  
        uint i;
        sclk=0;
     	tfs=0;
		rfs=1;
	    a0=0;
		for(i=0;i<25;i++)
		 {
		  sclk=0;
		  _nop_();
	     _nop_();
   	      sclk=1;
		    sdata=j&1;
		  _nop_();
		  _nop_();
		  j=(j>>=1);
		  }
         tfs=1; 
		 a0=1; 
		  while(drdy == 1);      
	  }

/********************************/
WLCL(ulong cfg)
{
jdq2=0;
jdq1=1;
delay1(2);
write(cfg);
delay1(4);
jdq1=0;
jdq2=1;
delay1(2);
jdq2=0;
}
/**************************/
 reversecfg(ulong ul)
{
    ulong j;
	ulong cfg = 0;
	for(j = 0;j<32;j++)
	{
		if(ul & ((ulong)0x01<<j))
			cfg = cfg | ((ulong)0x01 << (23-j));
	}
   WLCL(cfg);
}
/***************写164******************/
	
 void d164(uchar n)   //74164串行移位一个数
        {
         uchar i=0;
         uchar c=rrc[n];
         do{  
             if(c&0x01)     
                   	dat=1;
                    else
					dat=0;
		     c>>=1;        //数组tabel中的第N个数向右移动一位
			 _nop_(); _nop_();
             clk=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
             _nop_(); _nop_();
			 clk=1;
			  _nop_(); _nop_();
             clk=0;
			  _nop_(); _nop_();
             i++;
            }
        while(i<8);
        }

 display()
        {
		  uchar d1,d2,d3,d4,d5,d6,d7;
	      ulong de1;
		  de1=de;
          d1=de1/0x989680;
  		  d2=(de1%0x989680)/0xf4240;
		  d3=((de1%0xf989680)%0xf4240)/0x186a0;
		  d4=(((de1%0xf989680)%0xf4240)%0x186a0)/0x2710;
		  d5=((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)/0x3e8;
		  d6=(((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)%0x3e8)/0x64;
          d7=((((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)%0x3e8)%0x64)/10;
        d164(d1);
        d164(d2);
        d164(d3);
        d164(d4);
        d164(d5);
        d164(d6);
        d164(d7);
		  d164(0);
     
}

 /********************************/
 read7710()
	   {
	    uint j;
        sclk=0;
		a0=1;
		tfs=1;
		rfs=1;
		sdata=1;		 
	 RD:	
		de=0;
 		if(drdy==0)
		   { rfs=0 ;
             for(j=0;j<0x18;j++)
			     {sclk=0;
                 _nop_(); _nop_();_nop_();_nop_();
				  sclk=1;
				  _nop_();_nop_();_nop_();_nop_();
			      de=de|sdata;
				  sclk=0;
			      de<<=1; 
				 if (drdy==1)
					 j=0x18;
			     }	 
               }
		      	else 
            goto RD;
    
 
	 }

 
/*****************数据处理***********************/
 with()

{

   char count,i,j;
   ulong value_buf[N];
   ulong  sum=0,temp;
   for  (count=0;count<N;count++)
   {
	   read7710();
      value_buf[count] = de;
      
   }
   for (j=0;j<N-1;j++)
   {
      for (i=0;i<N-j;i++)
      {
         if ( value_buf[i]>value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
             value_buf[i+1] = temp;
         }
      }
   }
   for(count=2;count<N-2;count++)
     {  sum += value_buf[count];	 }
  de=sum/(N-4);
  dd=de;
  dejs=dejs+1;
}

void uart_5bit()
{
 	da[3] =  dd&0xff; 
	da[2] =  dd>>8&0xff;
	da[1] =  dd>>16&0xff;
	da[0] =  dd>>24&0xff; 
	if(dedjs!=dejs)
     {
	 dedjs=dejs;
	 da[4] = da[0]+da[1]+da[2]+da[3]+g_iAdc+g_iCurr+g_iPwr;
	 da[4] = ~da[4];
	 
	  }
	else 
	 da[4]=0;
	 
	 UART_SENDBIT(da[0]);
	 UART_SENDBIT(da[1]);
	 UART_SENDBIT(da[2]);
	 UART_SENDBIT(da[3]);
	 UART_SENDBIT(g_iAdc);
	 UART_SENDBIT(g_iCurr);
	 UART_SENDBIT(g_iPwr);
	 UART_SENDBIT(da[4]);

}

void ad_write( uchar lc )
{
	g_iAdc = lc;
    reversecfg(syscfg[lc+3]); //lc=2:20mV; lc=1:200mV; lc=0:2V;
 
}
void ad_main( void )
{     
	  with()   ;
	  display();
	  
}

