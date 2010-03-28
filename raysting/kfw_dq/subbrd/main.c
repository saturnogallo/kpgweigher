    #include <stc51.h>
	#include <stdio.h>
	#include <intrins.h>
	#include <ad7710.h>

	#define uchar unsigned char
	#define uint unsigned int
	#define ulong unsigned long

	#define BUS_IDLE	0	//searching for sync
#define BUS_SENDING	2	//is sending out data
#define HEAD_MARK	0xcc00	//header mark
#define HEAD_MASK	0xff00	//header mark
#define CMD_READ	0x01	//read data
uint busbuf = 0;
uchar busstate = BUS_IDLE;
uchar obit_cnt; //输出bit 计数
ulong dT;



       sbit busIbit = P2^7;	//INPUT
       sbit busObit = P2^4;	//OUTPUT
       sbit busCbit = P2^5;	//CLOCK


       uchar mlzj;
	   uchar uart_sbuf;
																	              
  /*595*/
        sbit SER   = P2^0;   //595数据端
        sbit SRCLK = P2^1;   //串行数据端
		sbit DSRCLK= P2^2; 
    	sbit RCK   = P2^3;   //数据输出端 
    	sbit KG    = P2^4;   //输出使能 

    uchar code jdq1[]={0x80,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40};
	uchar code jdq2[]={0x00,0xF9,0xF9,0XF9,0xF9,0xF9,0x6F,0xAF,0xC7,0x77,0xB7};
	uchar code jdq3[]={0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	uchar code jdq4[]={0x00,0XF7,0xEF,0XDF,0xBF,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF};
    uchar code jdq5[]={0x08,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04};
	uchar code jdq6[]={0xa7,0xff,0xff,0Xff,0xff,0xff,0xEF,0xEF,0xEF,0xef,0xEF};
    uchar code jdq7[]={0x08,0x04,0x08,0x04,0x04,0x04};
	uchar code jdq8[]={0xf7,0xF7,0x1f,0x7f,0xbf,0Xdf};
                    // 正向,反向,初态，大流,中流,小流,
        			// 正向：电流方向正
					// 反向：电流方向反
					// 初态：无电流
					// 大流：倍功率
					// 中流：等功率
					// 小流：半功率
                    
void with_jdq(uchar dw) ;
extern void uart_5bit();

/***************************595继电器输出*************************/
void DA5951(uchar n)   //74595串行移位一个数
        {
		 
         uchar i=0;
         uchar val;
		
		 val=jdq1[n] ;
         for (i=0;i<8;i++)
          {
          SRCLK=0;																
          SER=(bit)(val&0x01);
          val=val>>1;
          SRCLK=1;
           } 
        val=jdq2[n];
        for (i=0;i<8;i++)
          {
         SRCLK=0;
         SER=(bit)(val&0x01);
         val=val>>1;
         SRCLK=1;
           } 
        RCK =0;
        _nop_();
        _nop_();
        RCK =1;
        KG=0;
        delay1(10);
        KG=1; 
		}
	
// ***************************595继电器输出*************************/
void DA5952(uchar n)   //74595串行移位一个数
        {
		
         uchar i=0;
         uchar val;
		 
		 val=jdq3[n] ;
         for (i=0;i<8;i++)
          {
          SRCLK=0;
          SER=(bit)(val&0x01);
          val=val>>1;
          SRCLK=1;
           } 
        val=jdq4[n];
        for (i=0;i<8;i++)
          {
         SRCLK=0;
         SER=(bit)(val&0x01);
         val=val>>1;
         SRCLK=1;
           } 
        RCK =0;
        _nop_();
        _nop_();
        RCK =1;
        KG=0;
        delay1(10);
        KG=1;  
 }
// ***************************595继电器输出*************************/
void DA5953(uchar n)   //74595串行移位一个数
        {
		
         uchar i=0;
         uchar val;
		 
		 val=jdq5[n] ;
         for (i=0;i<8;i++)
          {
          SRCLK=0;
          SER=(bit)(val&0x01);
          val=val>>1;
          SRCLK=1;
           } 
        val=jdq6[n];
        for (i=0;i<8;i++)
          {
         SRCLK=0;
         SER=(bit)(val&0x01);
         val=val>>1;
         SRCLK=1;
           } 
        RCK =0;
        _nop_();
        _nop_();
        RCK =1;
        KG=0;
        delay1(10);
        KG=1;  
 }

/***************************595继电器输出*************************/
void DA595_4(uchar n)   //74595串行移位一个数
        {
		
         uchar i=0;
         uchar val;
		 
		 val=jdq7[n] ;
         for (i=0;i<8;i++)
          {
          SRCLK=0;
          SER=(bit)(val&0x01);
          val=val>>1;
          SRCLK=1;
           } 
        val=jdq8[n];
        for (i=0;i<8;i++)
          {
         SRCLK=0;
         SER=(bit)(val&0x01);
         val=val>>1;
         SRCLK=1;
           } 
        RCK =0;
        _nop_();
        _nop_();
        RCK =1;
        KG=0;
        delay1(10);
        KG=1;  
 }

/**********串口通讯*****************/
/***********串口初始化***************/   
void uart_232(void)  
{
  //set serial port parameter (clock 11.0592M)
   //9600 baut rate 8 data non parity and 1 stop.
   SCON = 0x70;
   PCON = 0x00;

   //use timer 1 to be serial
   //timer count
   TH1 = 0xfd;

   //use timer 0 to be heart beat
   TH0 = -4608/256;   // 4608 , 4608/(11.0592M/12) = 0.005s = 5ms
   TL0 = -4608%256;

   TMOD = 0x21;
   TR1 = 1;

   ET0=1;
   EA=1;
   TR0 = 1;
 }  
/***********发送一个字节*********/
void  UART_SENDBIT(uchar UART_data)
{
  TI = 0;
  SBUF =UART_data; //输出字符
  while(!TI);      //空语句判断字符是否发完
  TI = 0;          //清TI
}
#define CMD_INVALID	0xff
#define CMD_QUERY	0xA0
uchar last_cmd = CMD_INVALID;
/**********串口中断接收*********/
void m1serial(void) interrupt 1
{
} 

void m3serial(void) interrupt 3
{
} 

void m4serial(void) interrupt 4
{
} 

/*******************************/
void with_jdq(uchar dw)
{
    uchar dw1;
    DA5951(0);
    DA5952(0);
    DA5953(0);
    dw1=dw&0x0f;
	g_iCurr = dw1;
	switch (dw1)
	{
		case 1:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(2);
			}
			break;
		case 2:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(1);
			}
			break;
	   
		case 3:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(1);
			}
			break;
		case 4:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(1);
			}
			break;
	   case 5:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(1);
			}
			break;
		case 6:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(0);
			}
			break;
	   
		case 7:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(0);
			}
			break;
		case 8:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(0);
			}
			break;
	   case 9:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(0);
			}
			break;
		case 10:
		   {
		    DA5951(dw1);DA5952(dw1);DA5953(dw1);
		    ad_write(0);
			}
			break;
		default:
			{
		    DA5951(0);DA5952(0);DA5953(0);
		    ad_write(0);
			}
			break;
			
	}
   


}
void with_md(uchar mdd )
{
   uchar mmd1; 
   mmd1=mdd&0x0f;
   g_iPwr = mmd1;
switch (mmd1)
	{
		case 0:
		   {

			}
			break;
		case 1:
		   {
		   DA595_4(0); //电流正向
           }
           break;
	   case 2:
		   {
		   DA595_4(1); //电流反向
           }
           break;
	   case 3:
		   {
			DA595_4(2);   DA595_4(5);	// 小流：半功率

           }
           break;
	   case 4:
		   {
		  	DA595_4(2);  DA595_4(4);// 中流：等功率
	        }
           break;
	   case 5:
		   {
		  	DA595_4(2);  DA595_4(3);// 大流：倍功率		 
           }
           break;
		default:
			{
		   
			}
			break;
	}    
}

/***************************************/
void int_0()      //外部中断0
{
	EX0=1;              // INT0中断开
	IT0=0;              // INT0低电平触发(为1则为下降沿触发)
}

void timer0(void) interrupt 2
{
}

/**********************主程序**********************/
void main()
{
	IE=0;
	uart_232();
//    int_0()   ; //外中断0初始化     
	IE=92;
    DSRCLK = 1;  
    ad_write(0);
	with_jdq(10);
	with_md(1);
	with_md(5);
	
  while(1)
  {
  	if(RI)          //RI==开始接收
	{
		last_cmd = SBUF;
		RI = 0 ;       //软件RI=0 //
	}

//	if(last_cmd != this_cmd)
//		last_cmd = this_cmd;

  	if(last_cmd!=CMD_INVALID)
    {
		mlzj=last_cmd&0xf0;
		if(mlzj==0)
        { 
			with_jdq(last_cmd);
		}else if(mlzj==0xc0)
        {
	    	with_md(last_cmd);
		}else if(mlzj==CMD_QUERY)
		{
			uart_5bit();
		}
		last_cmd = CMD_INVALID;
    }
    ad_main();
 }
}








