




/* CH374芯片 软件模拟SPI串行连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序,提供寄存器级读写子程序 */

#include	"HAL.H"

#define SPI_S0          0x04
#define SPI_S1          0x08

#define SPIF            0x80                    //SPSTAT.7
#define WCOL            0x40                    //SPSTAT.6

#define SSIG            0x80                    //SPCTL.7
#define SPEN            0x40                    //SPCTL.6
#define DORD            0x20                    //SPCTL.5
#define MSTR            0x10                    //SPCTL.4
#define CPOL            0x08                    //SPCTL.3
#define CPHA            0x04                    //SPCTL.2
#define SPDHH           0x00                    //CPU_CLK/4
#define SPDH            0x01                    //CPU_CLK/16
#define SPDL            0x02                    //CPU_CLK/64
#define SPDLL           0x03                    //CPU_CLK/128

sbit SS             =   P5^5;                   //chip selection



//#define	CH374_SPI_SCS			P55		/* 假定CH374的SCS引脚 */
//#define	CH374_SPI_SDI			P41		/* 假定CH374的SDI引脚 */
//#define	CH374_SPI_SDO			P40		/* 假定CH374的SDO引脚 */
//#define	CH374_SPI_SCK			P43		/* 假定CH374的SCK引脚 */

void CH374_PORT_INIT( )  /* 由于使用通用I/O模拟并口读写时序,所以进行初始化 */
{
  	SPSTAT = SPIF | WCOL;                       //??SPI??
    SS = 1;
    SPCTL = SSIG | SPEN | MSTR;                 //??SPI????

}

/************************************************
SPI data exchange

    dat : byte to send
return :
    data from unit
************************************************/
BYTE SpiShift(BYTE dat)
{
    SPDAT = dat;                                //??SPI??
    while (!(SPSTAT & SPIF));                   //??SPI??????
    SPSTAT = SPIF | WCOL;                       //??SPI??
    
    return SPDAT;
}


void	Spi374Start( UINT8 addr, UINT8 cmd )  /* SPI开始 */
{
//	CH374_SPI_SCS = 1;
/* 对于双向I/O引脚模拟SPI接口,那么必须确保已经设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,SPI_SDO为输入方向 */
	SS = 0;  /* SPI片选有效 */
	Spi374OutByte( addr );
	Spi374OutByte( cmd );
}
void  MySpi374Stop()	/* SPI结束 */
{ 
	SS = 1; 
}  
#define	Spi374Stop( )	{ SS = 1; }  /* SPI结束 */
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


