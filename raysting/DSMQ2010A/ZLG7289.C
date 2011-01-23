#include "ZLG7289.h"

//   #include <STC89C51RC.h>
#include <stc51.h>
//定义I/O 接口
sbit ZLG7289_pinCS = P3^2; //片选信号，低电平有效
sbit ZLG7289_pinCLK = P3^3; //时钟信号，上升沿有效
sbit ZLG7289_pinDIO = P3^4; //数据信号，双向

//定义一个全局变量，用于延时
//该变量在调用函数ZLG7289_Init()时被初始化
unsigned char ZLG7289_Delay_t;
/*
宏定义：ZLG7289_ShortDelay()
功能：短延时
说明：延时(ZLG7289_Delay_t*2+2)个机器周期
*/
#define ZLG7289_ShortDelay() \
{\
unsigned char t = ZLG7289_Delay_t;\
while ( --t != 0 );\
}
/*
宏定义：ZLG7289_LongDelay()
功能：长延时
说明：延时(ZLG7289_Delay_t*12+8)个机器周期
*/
#define ZLG7289_LongDelay() \
{\
unsigned char t = ZLG7289_Delay_t * 6;\
while ( --t != 0 );\
}
/*
函数：ZLG7289_SPI_Write()
功能：向SPI 总线写入1 个字节的数据
参数：
dat：要写入的数据
*/
void ZLG7289_SPI_Write(char dat) reentrant
{
unsigned char t = 8;
do
{
ZLG7289_pinDIO = (bit)(dat & 0x80);
dat <<= 1;
ZLG7289_pinCLK = 1;
ZLG7289_ShortDelay();
ZLG7289_pinCLK = 0;
ZLG7289_ShortDelay();
} while ( --t != 0 );
}
/*
函数：ZLG7289_SPI_Read()
功能：从SPI 总线读取1 个字节的数据
返回：
读取到的数据
*/
char ZLG7289_SPI_Read() reentrant
{
char dat;
unsigned char t = 8;
ZLG7289_pinDIO = 1; //读取数据之前DIO 引脚要置1 以切换到输入状态
do
{
ZLG7289_pinCLK = 1;
ZLG7289_ShortDelay();
dat <<= 1;
if ( ZLG7289_pinDIO ) dat++;
ZLG7289_pinCLK = 0;
ZLG7289_ShortDelay();
} while ( --t != 0 );
return dat;
}
/*
函数：ZLG7289_cmd()
功能：执行ZLG7289 纯指令
参数：
cmd：命令字
*/
void ZLG7289_cmd(char cmd)
{
char sav = IE;
IE &= 0xFA; //关闭外部中断
ZLG7289_pinCS = 0;
ZLG7289_LongDelay();
ZLG7289_SPI_Write(cmd);
ZLG7289_pinCS = 1;
ZLG7289_LongDelay();
IE = sav; //恢复IE
}
/*
函数：ZLG7289_cmd_dat()
功能：执行ZLG7289 带数据指令
参数：
cmd：命令字
dat：数据
*/
void ZLG7289_cmd_dat(char cmd, char dat)
{
char sav = IE;
IE &= 0xFA; //关闭外部中断
ZLG7289_pinCS = 0;
ZLG7289_LongDelay();
ZLG7289_SPI_Write(cmd);
ZLG7289_LongDelay();
ZLG7289_SPI_Write(dat);
ZLG7289_pinCS = 1;
ZLG7289_LongDelay();
IE = sav; //恢复IE
}
/*
函数：ZLG7289_Download()
功能：下载数据
参数：
mod=0：下载数据且按方式0 译码
mod=1：下载数据且按方式1 译码
mod=2：下载数据但不译码
x：数码管编号（横坐标），取值0～7
dp=0：小数点不亮
dp=1：小数点亮
dat：要显示的数据
说明：
译码方式的具体含义请参考数据手册
*/
void ZLG7289_Download(unsigned char mod, char x, bit dp, char dat)
{
code char ModDat[3] = {0x80,0xC8,0x90};
char d1;
char d2;
if ( mod > 2 ) mod = 2;
d1 = ModDat[mod];
x &= 0x07;
d1 |= x;
d2=dat;
dp=dp;
//d2 = dat & 0x7F;
//if ( dp ) d2 |= 0x80;
ZLG7289_cmd_dat(d1,d2);
}
/*
功能：执行ZLG7289 键盘命令
返回：
返回读到的按键值：0～63
如果返回 0xFF 则表示没有键按下
*/
char ZLG7289_Key() reentrant
{
char key;
ZLG7289_pinCS = 0;
ZLG7289_LongDelay();
ZLG7289_SPI_Write(0x15);
ZLG7289_LongDelay();
key = ZLG7289_SPI_Read();
ZLG7289_pinCS = 1;
ZLG7289_LongDelay();
return key;
}
/*
函数：ZLG7289_Init()
功能：ZLG7289 初始化
参数：
州周立功单片机发展有限公司 Tel：(020)38730976 38730977 Fax：38730925 http://www.zlgmcu.com
23
t：SPI 总线的延时值设置，取值1-40（超出范围可能导致错误）
说明：
t的取值可以参照以下公式：t >= 5 * f1 / f2，其中f1 表示80C51 的
晶振频率，f2 表示ZLG7289 的晶振频率
*/
void ZLG7289_Init(unsigned char t)
{
unsigned char x;
//I/O 口初始化
ZLG7289_pinCS = 1;
ZLG7289_pinCLK = 0;
ZLG7289_pinDIO = 1;
//ZLG7289_pinINT = 1;
//延时初始化
ZLG7289_Delay_t = t;
//点亮所有数码管
for ( x=0; x<8; x++ )
{
ZLG7289_Download(1,x,1,8);
}
}