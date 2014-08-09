//#include <stc89c51rc.h>
#include <stc51.h>
#include <intrins.h>

#include "zlg7289.h"
unsigned char dlyde,dlying,UART_DAT,MAIN_DAT ;
unsigned char key_7289,keying,key_7289_ed ;
bit  lineing;//0:2线；  1：4线。

sbit ZLG7289_pinINT = P3^5; //键盘中断请求信号，低电平（负边沿）有效
/*595*/
sbit SER   = P2^4;  //595数据端
sbit SRCLK = P2^1;  //串行数据端 
sbit RCK   = P2^2;  //数据输出端 
sbit KG    = P2^3;  //输出使能 
sbit CLR   = P2^0;  //清零
void delay(unsigned int dy)
{
	unsigned int i;
	for(i=0;i<dy;i++  )
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();

	}
}
/***************************595继电器输出**************************/
void D595(unsigned char DA1,DA2,DA3,DA4)   //74595串行移位一个数
{
	unsigned char i=0;
	for (i=0;i<8;i++)
	{
		SRCLK=0;            _nop_();_nop_();
		SER=(bit)(DA1&0x01);_nop_(); _nop_();
		DA1=DA1>>1;         _nop_(); _nop_();
		SRCLK=1;            _nop_();_nop_();
	} 

	for (i=0;i<8;i++)
	{
		SRCLK=0;              _nop_();_nop_();
		SER=(bit)(DA2&0x01);	_nop_();_nop_();
		DA2=DA2>>1;			_nop_();_nop_();
		SRCLK=1;				_nop_();_nop_();
	} 

	for (i=0;i<8;i++)
	{
		SRCLK=0;			   _nop_();_nop_();
		SER=(bit)(DA3&0x01); _nop_();_nop_();
		DA3=DA3>>1;		   _nop_();_nop_();
		SRCLK=1;			   _nop_();_nop_();
	}

	for (i=0;i<8;i++)
	{
		SRCLK=0;			  _nop_();_nop_();
		SER=(bit)(DA4&0x01);_nop_();_nop_();
		DA4=DA4>>1;		  _nop_();_nop_();
		SRCLK=1;			  _nop_();_nop_();
	}
	RCK =0;
	_nop_();
	_nop_();
	RCK =1;
	KG=0;
	delay(2000);
	KG=1; 

}

void int_jdq (void)
{
	KG=1; 	 CLR=1;

	D595(0X55,0X2a,0X2a,0X55);

	_nop_();

	_nop_();
}




void dis_play( void )
{
	ZLG7289_Reset();
	int_jdq(); 
	if(lineing==1)
	{
		ZLG7289_Download(2,0,0,0x10); 
		D595(0X55,0X2a,0X2a,0X55); 
	}else{
		ZLG7289_Download(2,1,0,0x10); 
		D595(0X55,0X2a,0X2a,0X55); 
	}

	switch (keying )
	{
	case 1:
		{
			D595(0X55,0X2a,0X2a,0X35); 
			if(lineing==1)
			{
				ZLG7289_Download(2,0,0,0x11); 
			}else{
				ZLG7289_Download(2,0,0,0x01);
				ZLG7289_Download(2,1,0,0x10); 
			}
		}

		break;
	case 2:
		{
			D595(0X55,0X2a,0X2a,0X4D); 
			if(lineing==1)
			{
				ZLG7289_Download(2,1,0,0x01); 
				ZLG7289_Download(2,0,0,0x10);
			}else{
				ZLG7289_Download(2,1,0,0x11); 
			}
		}
		break;
	case 3:
		D595(0X55,0X2a,0X2a,0X53);
		ZLG7289_Download(2,2,0,0x01);
		break;
	case 4:
		D595(0X55,0X2a,0X2B,0X54);
		ZLG7289_Download(2,3,0,0x01);
		break;
	case 5:
		D595(0X35,0X2a,0X2a,0X55);
		ZLG7289_Download(2,4,0,0x01);
		break;
	case 6:
		D595(0X4d,0X2a,0X2a,0X55);
		ZLG7289_Download(2,5,0,0x01);
		break;
	case 7:
		D595(0X53,0X2a,0X4a,0X55);
		{ 
			if(lineing==1)
			{
				ZLG7289_Download(2,0,0,0x12); 
			}else{
				ZLG7289_Download(2,0,0,0x02);
				ZLG7289_Download(2,1,0,0x10);
			}
		}
		break;
	case 8:
		D595(0X53,0X2a,0X32,0X55);
		{ 
			if(lineing==1)
			{
				ZLG7289_Download(2,1,0,0x02); 
				ZLG7289_Download(2,0,0,0x10);
			}else{
				ZLG7289_Download(2,1,0,0x12); 
			}
		}
		break;
	case 9:
		D595(0X53,0X2a,0X2C,0X55);
		ZLG7289_Download(2,2,0,0x02);
		break;
	case 10:
		D595(0X53,0X29,0X2A,0X55);
		ZLG7289_Download(2,3,0,0x02);
		break;
	case 11:
		D595(0X53,0X26,0X2A,0X55);
		ZLG7289_Download(2,4,0,0x02);
		break;
	case 12:
		D595(0X53,0X1A,0X6A,0X55);
		ZLG7289_Download(2,5,0,0x02);
		break;
	case 13:
		D595(0X54,0X6a,0X2a,0X35); 
		{ 
			if(lineing==1)
			{
				ZLG7289_Download(2,0,0,0x14); 
			}else{
				ZLG7289_Download(2,0,0,0x04);
				ZLG7289_Download(2,1,0,0x10);
			}
		}
		break;
	case 14:
		D595(0X54,0X6a,0X2a,0X4D); 
		{ 
			if(lineing==1)
			{
				ZLG7289_Download(2,1,0,0x04); 
				ZLG7289_Download(2,0,0,0x10);
			}else{
				ZLG7289_Download(2,1,0,0x14); 
			}
		}
		break;
	case 15:
		D595(0X54,0X6a,0X2a,0X53);
		ZLG7289_Download(2,2,0,0x04);
		break;
	case 16:
		D595(0X54,0X6a,0X2B,0X54); 
		ZLG7289_Download(2,3,0,0x04);
		break;
	case 17:
		D595(0X34,0X6a,0X2a,0X55);
		ZLG7289_Download(2,4,0,0x04);
		break;
	case 18:
		D595(0X4c,0X6a,0X2a,0X55);
		ZLG7289_Download(2,5,0,0x04);
		break;
	case 19:
		D595(0X42,0X6a,0X4a,0X55);
		{ 
			if(lineing==1)
			{
				ZLG7289_Download(2,0,0,0x18); 
			}else{
				ZLG7289_Download(2,0,0,0x08);
				ZLG7289_Download(2,1,0,0x10);
			}
		}
		break;
	case 20:
		D595(0X52,0X6a,0X32,0X55);
		{ 
			if(lineing==1)
			{
				ZLG7289_Download(2,1,0,0x08); 
				ZLG7289_Download(2,0,0,0x10);
			}else{
				ZLG7289_Download(2,1,0,0x18); 
			}
		}
		break;
	case 21:
		D595(0X52,0X6a,0X2C,0X55);
		ZLG7289_Download(2,2,0,0x08);
		break;
	case 22:
		D595(0X52,0X69,0X2A,0X55);
		ZLG7289_Download(2,3,0,0x08);
		break;
	case 23:
		D595(0X52,0X66,0X2A,0X55);
		ZLG7289_Download(2,4,0,0x08);
		break;
	case 24:
		D595(0X52,0X5A,0X2A,0X55);
		ZLG7289_Download(2,5,0,0x08);
		break;
	case 25:
		D595(0X55,0X2A,0X2A,0X55);
		break;
	default:
		ZLG7289_Test();
		break;
	}
}

void with_key( void )
{
	switch (key_7289 )
	{
	case 0:
		keying=1;
		break;
	case 8:
		keying=2;
		break;
	case 16:
		keying=3;
		break;
	case 24:
		keying=4;
		break;
	case 32:
		keying=5;
		break;
	case 40:
		keying=6;
		break;
	case 1:
		keying=7;
		break;
	case 9:
		keying=8;
		break;
	case 17:
		keying=9;
		break;
	case 25:
		keying=10;
		break;
	case 33:
		keying=11;
		break;
	case 41:
		keying=12;
		break;
	case 2:
		keying=13;
		break;
	case 10:
		keying=14;
		break;
	case 18:
		keying=15;
		break;
	case 26:
		keying=16;
		break;
	case 34:
		keying=17;
		break;
	case 42:
		keying=18;
		break;
	case 3:
		keying=19;
		break;
	case 11:
		keying=20;
		break;
	case 19:
		keying=21;
		break;
	case 27:
		keying=22;
		break;
	case 35:
		keying=23;
		break;
	case 43:
		keying=24;
		break;
	case 4:
		{
			lineing=1;keying=25;dis_play( );
		}
		break;
	case 12:
		{
			lineing=0;keying=25;dis_play( );
		}
		break;
	default:

		break;
	}
}



void uart_232(void)  
{
	SCON = 0x50; //串口方式1,允许接收
	TMOD = 0x21; //定时器1定时方式2
	TH1 = 0xfa;TL1 = 0xfa;	  //9600比特
	PCON = 0x80; //波特率加倍控制,SMOD位
	//  TI = 1;		 //开启定时器1
	ET1 =0;
	TR1 = 1; //启动定时器
	EA=1;  	 //打开总中断
	ES=1;	 //打开串行中断
}
/***********发送一个字节*********/
void  UART_SENDBIT(unsigned char UART_data)
{
	TI = 0; 
	SBUF =UART_data; //输出字符
	while(!TI);      //空语句判断字符是否发完
	TI = 0;          //清TI
}

/**********串口中断接收*********/
void serial () interrupt 4 using 3
{

	if (RI)          //RI==开始接收
	{
		UART_DAT=SBUF;
		//UART_SENDBIT(UART_DAT);
		RI = 0 ;       //软件RI=0 //

	}
}

#define BUFFER_MAX	15

void main( void )
{
	unsigned char uart_buffer[BUFFER_MAX];
	unsigned char uart_ptr;

	ZLG7289_pinINT = 1;

	ZLG7289_Init(40)	  ;
	ZLG7289_Reset();
	int_jdq()  ;
	lineing=1; //4 wire model
	keying=25;
	dis_play();
	uart_232();
	uart_ptr = 0;
	key_7289_ed = 0xfe; //invalid last value;
	while(1){
		if(ZLG7289_pinINT ==0)
		{
			key_7289=ZLG7289_Key();
			if(key_7289!=key_7289_ed)
			{
				with_key();
				key_7289_ed=key_7289;
				if(lineing==0|keying<13)
				{
					dis_play( );
				}
			}
		}
		if(UART_DAT!=0)
		{
			
			uart_buffer[uart_ptr] = UART_DAT;
			UART_DAT = 0;
			uart_ptr++;
			if(uart_ptr >= BUFFER_MAX)
			{
				uart_ptr = 0;
				continue;
			}
			if(uart_buffer[uart_ptr-1] != 0x0A) //end of command
				continue;
			if((uart_ptr <= 2) || (uart_buffer[uart_ptr-2] != 0x0D)) //not ended with 0x0D,0x0A
			{
				uart_ptr = 0;
				continue;
			}
			if((uart_ptr == 3) && (uart_buffer[uart_ptr-3] == '!')) //2wire
			{
				lineing = 0;
				uart_ptr = 0;
				dis_play();
				continue;
			}
			if((uart_ptr == 3) && (uart_buffer[uart_ptr-3] == '#')) //4wire
			{
				lineing = 1;
				uart_ptr = 0;
				dis_play();
				continue;
			}
			if((uart_ptr == 3) && (uart_buffer[uart_ptr-3] == '*')) //reset command
			{
				keying = 25;
				dis_play();
				uart_ptr = 0;
				continue;
			}
			if((uart_ptr == 4) && (uart_buffer[0] > '0') && (uart_buffer[0] <= '9')) // 3A case
			{
				keying = uart_buffer[0] - '0';
				dis_play( );
				uart_ptr = 0;
				continue;
			}
			if((uart_ptr == 5) && 
				(uart_buffer[1] >= '0') && (uart_buffer[1] <= '9') &&
				(uart_buffer[0] > '0') && (uart_buffer[0] <= '2')) //12A case
			{
				keying = (uart_buffer[0] - '0')*10+(uart_buffer[1] - '0');
				if(lineing==1 && keying < 13) //4 wire
					dis_play( );
				else if (keying < 25) //2 wire
					dis_play( );
				uart_ptr = 0;
				continue;
			}
			uart_ptr = 0;
			continue;

			/*
			MAIN_DAT=UART_DAT&0XC0; 
			if(MAIN_DAT==0X40)
			{
				lineing=UART_DAT&0X20;
				keying =UART_DAT&0X1F;
				if(lineing)
				{
					if(keying>0x0c)
					{
						UART_SENDBIT(0xc0); 
						keying=25;
					}
				}else{
					if(keying>24)  
					{ 
						UART_SENDBIT(0xc0); 
						keying=25;
					} 
				} 
				dis_play( );
				UART_DAT=0;
			}
			MAIN_DAT=UART_DAT&0XC0;
			if(MAIN_DAT==0X80)
			{
				UART_DAT=0;
				UART_DAT=0X80|keying;
				if(lineing)
				{
					UART_DAT=UART_DAT|0X20;
				}else
					;
				UART_SENDBIT(UART_DAT);
				UART_DAT=0;
			}
			*/
		}	
		_nop_();
	}		 
}


