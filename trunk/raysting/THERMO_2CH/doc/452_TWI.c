//本例子程序提供操作方法，仅供参考
// *******************************************************************
// Website:  http://wch.cn
// Email:    tech@wch.cn
// Author:   TECH  2008.11.25
// *******************************************************************

//此程序仅可以在202714085批号的CH452上面使用
//ATMEGA16 PC0   -------    CH452 SCL
//ATMEGA16 PC2   -------    CH452 INT#
//ATMEGA16 PC1   -------    CH452 SDA
//这里CH452接地址1

//ICCAVR下编译

// **************************************
// 加载头文件
// **************************************
#include <iom16v.h>


// *******************
// CH452的定义
// *******************
#define CH452_DIG0      0x0800                  //数码管位0显示
#define CH452_DIG1      0x0900		            //数码管位1显示 
#define CH452_DIG2      0x0a00		            //数码管位2显示
#define CH452_DIG3      0x0b00		            //数码管位3显示
#define CH452_DIG4      0x0c00		            //数码管位4显示
#define CH452_DIG5      0x0d00	                //数码管位5显示	
#define CH452_DIG6      0x0e00	                //数码管位6显示	
#define CH452_DIG7      0x0f00		            //数码管位7显示

#define	CH452_INT_IN		{ PORTC |= (1<<PC2); DDRC &= ~(1<<PC2); }
#define mINT				( PINC&(1<<PC2) )		//中断脚接INT#

#define	CH452_I2C_ADDR0		0x40			// CH452的时序地址位，与I2C地址无关
#define	CH452_I2C_ADDR1		0x60			// CH452的时序地址位，与I2C地址无关
#define	CH452_I2C_MASK		0x3E			// CH452的2线接口高字节命令掩码



// **************************************
// 类型定义
// **************************************
typedef unsigned char UINT8;
typedef unsigned short UINT16;

// **************************************
// CH452的全局变量
// **************************************
UINT8 mADDR;//I2C地址



// **************************************
// 延时毫秒,不准
// **************************************
void mDelaymS( unsigned char ms )   
{
    unsigned short i;
    while ( ms -- )
    {
        for( i = 0; i != 1000; i++ );
    }
}


// **************************************
// CH452初始化
// **************************************
void CH452_I2C_Init( void )
{
 	 PORTC |= (1<<PC1)|(1<<PC0);
	 DDRC |= (1<<PC1)|(1<<PC0);
   	 TWBR = 0x64; 
	 TWSR = (1<<TWPS1)|(1<<TWPS0);
   	 TWCR = ( 1<<TWEN ); //使能TWI
}


// **************************************
// I2C START
// **************************************
void CH452_START( void )
{
    TWCR = ( 1<<TWEN )|(1<<TWSTA )|(1<<TWINT);//发 START
	
	while( !(TWCR & (1<<TWINT)) );
}

// **************************************
// I2C STOP
// **************************************
void CH452_STOP( void ) 
{ 
    TWCR = ( 1<<TWEN )|(1<<TWSTO)|(1<<TWINT); //STOP 
}

// **************************************
// I2C 发命令地址
// **************************************
void CH452_Wr_Cmd( UINT8 cmd )//发CH452命令
{
 	TWDR = cmd;

	TWCR = ( 1<<TWEN )|(1<<TWINT);
	
	while( !(TWCR & (1<<TWINT)) );
}

// **************************************
// I2C 发数据
// **************************************
void CH452_Wr_Data( UINT8 bData )//发CH452数据
{
	TWDR = bData;
	
	TWCR = ( 1<<TWEN )|(1<<TWINT);
	
	while( !(TWCR & (1<<TWINT)) );
}

// **************************************
// I2C写CH452命令
// **************************************
void CH452_Write( UINT16 cmd )
{
     UINT8 try;//重试次数
	 
	 try = 50;
	 do
	 {
 	   	 CH452_START();
	 
	 	 if( mADDR == 1 )
		 {
   	         CH452_Wr_Cmd((UINT8)(cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1);    
		 }
		 else
		 {
	         CH452_Wr_Cmd((UINT8)(cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0);
		 }
		
	 	 if( ( TWSR & 0xF8 ) == 0x18 )//收到ACK
	 	 {
	  	  	 CH452_Wr_Data( (UINT8)cmd );
			 CH452_STOP();
			 break;
	 	 }
	 	 
		 CH452_STOP();
		 
		 if( try ) try--;
	 }
	 while( try );
	 
}

// **************************************
// I2C读CH452命令
// **************************************
UINT8 CH452_Read( UINT16 read_cmd )		
{
    UINT8 try;//重试次数
	UINT8 val;
	
	try = 50;
	do
	{
	  	val = 0xFF;
		
   	  	CH452_START();

		if( mADDR == 1 )
		{
   	         CH452_Wr_Cmd((UINT8)(read_cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1|0x01);    
		}
		else
		{
	         CH452_Wr_Cmd((UINT8)(read_cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0|0x01);
		}
		
		if( ( TWSR & 0xF8 ) == 0x40 )//收到ACK
   		{
	 	 	TWCR = ( 1<<TWEN )|(1<<TWINT);
			while( !(TWCR & (1<<TWINT)) );
	    	val = TWDR;
			CH452_STOP();
			break;
    	}
		
		CH452_STOP();
		
		if( try ) try--;
 	}
	while( try );
	
	return val;
}

// **************************************
// 主程序
// **************************************
void main( void )
{
     UINT8 CH452_Key;
	 
 	 mDelaymS(20);
	 CH452_I2C_Init();
	 
	 //选择被操作的CH452地址  0和1可选
   	 mADDR = 1;//操作1地址CH452
   	 
	 CH452_Write( 0x0700 );//开ACK功能
   	 CH452_Write( 0x403 );//开显示键盘
   	 CH452_Write( 0x500 );//直接译码    
   	 CH452_Write( 0x580 );//BCD译码
  
   	 //显示 1,2,3,4,5,6,7,8
   	 CH452_Write( CH452_DIG0 | 1 ); 
   	 CH452_Write( CH452_DIG1 | 2 ); 
   	 CH452_Write( CH452_DIG2 | 3 ); 
   	 CH452_Write( CH452_DIG3 | 4 );
   	 CH452_Write( CH452_DIG4 | 5 );
   	 CH452_Write( CH452_DIG5 | 6 );
   	 CH452_Write( CH452_DIG6 | 7 ); 
   	 CH452_Write( CH452_DIG7 | 8 );

   	 //读按键
   	 while(1)
   	 {
        	 while( mINT );//等待中断

        	 CH452_Key = CH452_Read(0x0700);
			 if( CH452_Key != 0xFF )//读正确
			 {
			  	 CH452_Key &= 0x3F;

        	 	 CH452_Write(0x300);//左移两位
        	 	 CH452_Write(0x300);
   
        	 	 CH452_Write(CH452_DIG1|(CH452_Key/10));
        	 	 CH452_Write(CH452_DIG0|(CH452_Key%10));
			 
			 	 while(1)//判断按键是否释放
			 	 {
			  	  	   CH452_Key = CH452_Read(0x0700);
				  	   if( CH452_Key & 0x40 )
				  	   {
				   	   	   CH452_Write( 0x0603 );
				       }
				  	   else
				  	   {
				   	       CH452_Write( 0x0600 );
					  	   break;
				       }
				 }
			}
   	 }
}







