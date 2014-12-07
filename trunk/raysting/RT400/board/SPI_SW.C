/* CH374芯片 软件模拟SPI串行连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序,提供寄存器级读写子程序 */

#include	"HAL.H"
/* 本例中的硬件连接方式如下(实际应用电路可以参照修改下述定义及子程序) */
/* 单片机的引脚    CH374芯片的引脚
      P1.4                 SCS#
      P1.5                 SDI
      P1.6                 SDO
      P1.7                 SCK      */
sbit    CH374_SPI_SCS					=   P5^5;
sbit    CH374_SPI_SDO					=   P4^1;
sbit	CH374_SPI_SDI					=	P4^0;
sbit	CH374_SPI_SCK					=	P4^3;

#define MYSLOW()	//mDelayuS(1)

//#define	CH374_SPI_SCS			P55		/* 假定CH374的SCS引脚 */
//#define	CH374_SPI_SDI			P41		/* 假定CH374的SDI引脚 */
//#define	CH374_SPI_SDO			P40		/* 假定CH374的SDO引脚 */
//#define	CH374_SPI_SCK			P43		/* 假定CH374的SCK引脚 */

void CH374_PORT_INIT( )  /* 由于使用通用I/O模拟并口读写时序,所以进行初始化 */
{
	UINT8 i;
/* 如果是硬件SPI接口,那么可使用mode3(CPOL=1&CPHA=1)或mode0(CPOL=0&CPHA=0),CH374在时钟上升沿采样输入,下降沿输出,数据位是高位在前 */
	CH374_SPI_SCS = 1;  /* 禁止SPI片选 */
	CH374_SPI_SCK = 1;  /* 默认为高电平,SPI模式3,也可以用SPI模式0,但模拟程序可能需稍做修改 */
/* 对于双向I/O引脚模拟SPI接口,那么必须在此设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,SPI_SDO为输入方向 */
	//Added by song,jie
	CH374_SPI_SDI = 1;
	i = CH374_SPI_SDO;
}

void	Spi374OutByte( UINT8 d )  /* SPI输出8个位数据 */
{  /* 如果是硬件SPI接口,应该是先将数据写入SPI数据寄存器,然后查询SPI状态寄存器以等待SPI字节传输完成 */
	UINT8	i;
	for ( i = 0; i < 8; i ++ ) {
		CH374_SPI_SCK = 0;MYSLOW();

		if ( d & 0x80 ) CH374_SPI_SDI = 1;
		else CH374_SPI_SDI = 0;

		MYSLOW();
		d <<= 1;  /* 数据位是高位在前 */
		CH374_SPI_SCK = 1;  /* CH374在时钟上升沿采样输入 */
		MYSLOW();
	}
}

UINT8	Spi374InByte( void )  /* SPI输入8个位数据 */
{  /* 如果是硬件SPI接口,应该是先查询SPI状态寄存器以等待SPI字节传输完成,然后从SPI数据寄存器读出数据 */
	UINT8	i, d;
	d = 0;
/* 如果SPI的SDO和SDI合用一个I/O引脚，那么必须在此设置该I/O方向为输入 */
	for ( i = 0; i < 8; i ++ ) {
		CH374_SPI_SCK = 0;  /* CH374在时钟下降沿输出 */
		MYSLOW();
		d <<= 1;  /* 数据位是高位在前 */
		if ( CH374_SPI_SDO ) d ++;
		CH374_SPI_SCK = 1;
		MYSLOW();
	}
	return( d );
}

void	Spi374Start( UINT8 addr, UINT8 cmd )  /* SPI开始 */
{
//	CH374_SPI_SCS = 1;
/* 对于双向I/O引脚模拟SPI接口,那么必须确保已经设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,SPI_SDO为输入方向 */
	CH374_SPI_SCS = 0;  /* SPI片选有效 */
	MYSLOW();
	Spi374OutByte( addr );
	Spi374OutByte( cmd );
}
void  MySpi374Stop()	/* SPI结束 */
{ 
	CH374_SPI_SCS = 1; 
}  
#define	Spi374Stop( )	{ CH374_SPI_SCS = 1; }  /* SPI结束 */
//void	Spi374Stop( void )  /* SPI结束 */
//{
//	CH374_SPI_SCS = 1;	/* SPI片选无效 */
//}

UINT8	Read374Byte( UINT8 mAddr )  /* 从指定寄存器读取数据 */
{
	UINT8	d;
	Spi374Start( mAddr, CMD_SPI_374READ );
	d = Spi374InByte( );
	Spi374Stop( );
	return( d );
}

void	Write374Byte( UINT8 mAddr, UINT8 mData )  /* 向指定寄存器写入数据 */
{
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	Spi374OutByte( mData );
	Spi374Stop( );
}

void	Read374Block( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf )  /* 从指定起始地址读出数据块 */
{
	Spi374Start( mAddr, CMD_SPI_374READ );
	while ( mLen -- ) *mBuf++ = Spi374InByte( );
	Spi374Stop( );
}

void	Write374Block( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf )  /* 向指定起始地址写入数据块 */
{
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	while ( mLen -- ) Spi374OutByte( *mBuf++ );
	Spi374Stop( );
}



