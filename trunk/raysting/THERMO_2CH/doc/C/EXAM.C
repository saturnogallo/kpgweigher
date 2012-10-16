// *************************************
//Website:  http://wch.cn
//Email:    tech@wch.cn
//Author:   W.ch 2005.4
// *************************************

#include <reg52.h>
#include <intrins.h>

#include "CH452CMD.H"	// 定义常用命令码及外部子程序

sbit	LED=P1^0;

void delayms(unsigned char i)
{	unsigned int	j;
	do{	for(j=0;j!=1000;j++)
		{;}
	}while(--i);
}

void init()
{	SCON=0x50;
	PCON=0x80;
	TMOD=0x21;
	TH1=0xF3;//        ;24MHz晶振, 9600bps,通过串口发送命令，或接收返回的按键值。
	TR1=1;
	RI=0;
	TI=0;
// 设置CH452按键中断
//	CH452_DOUT=1;	// 设置该引脚为输入方向
	IE1=0;
	EX1=1;
	EA=1;
}

// INT1中断服务程序
void int1() interrupt 2 //using 1
{
	TI=0;
	SBUF=CH452_Read();  //将按键值通过串口发给PC机检查
	while(!TI);
	TI=0;
	LED=!LED;
}

main()
{	unsigned char cmd,dat;
	unsigned short	command;
	delayms(50);	
	init();
	SBUF=CH452_Read();		//读取CH452的版本号，正式应用时不需要。
	while(!TI);
	TI=0;
	CH452_Write(CH452_SYSON2);	//两线制方式，如果SDA用做按键中断输出，那么命令应该为CH452_SYSON2W(0x04,0x23)
	CH452_Write(CH452_BCD);   // BCD译码,8个数码管
	CH452_Write(CH452_DIG7 | 1);
	CH452_Write(CH452_DIG6 | 2);
	CH452_Write(CH452_DIG5 | 3);
	CH452_Write(CH452_DIG4 | 4);
	CH452_Write(CH452_DIG3 | 5);
	CH452_Write(CH452_DIG2 | 6);
	CH452_Write(CH452_DIG1 | 7);
	CH452_Write(CH452_DIG0 | 8);  // 显示字符8
	delayms(50);
	LED=!LED;
	while ( 1 ){  // PC机串口通讯单片机，再控制CH452演示
		while(!RI);			//等待接收命令数据
		cmd=SBUF;
		RI=0;
		if ( cmd & 0xE0 ) continue;  // 无效命令码
		while(!RI);	//接收第二个命令数据,未考虑超时
		dat=SBUF;
		RI=0;
		command=cmd;
		command= command<<8 | dat;
		CH452_Write(command);
	}
}
