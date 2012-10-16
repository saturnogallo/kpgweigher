//本例子程序提供一种操作方法，仅供参考
// *******************************************************************
// Website:  http://wch.cn
// Email:    tech@wch.cn
// Author:   TECH  2008.10.14
// *******************************************************************

//ATMEGA16 PB7   -------    CH452 SCL
//ATMEGA16 PB6   -------    CH452 INT#
//ATMEGA16 PB5   -------    CH452 SDA
//CH452这里接地址1

//ICCAVR下编译

// *******************
// 加载头文件
// *******************
#include <iom16v.h>
#include <macros.h>


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

#define	CH452_SDA_OUT   	{ DDRB |= (1<<PB5); }
#define	CH452_SDA_IN		{ PORTB |= (1<<PB5); DDRB &= ~(1<<PB5); }
#define	CH452_SDA_SET		PORTB |= (1<<PB5)
#define	CH452_SDA_CLR		PORTB &= ~(1<<PB5)
#define	CH452_SDA			( PINB & (1<<PB5) )
#define	CH452_SCL_OUT		{ PORTB |= (1<<PB7); DDRB |= (1<<PB7); }
#define	CH452_SCL_SET		PORTB |= (1<<PB7)
#define	CH452_SCL_CLR		PORTB &= ~(1<<PB7)
#define	CH452_INT_IN		{ PORTB |= (1<<PB6); DDRB &= ~(1<<PB6); }
#define mINT				( PINB&(1<<PB6) )		//中断脚接DOUT
#define DELAY				mDelayuS(1)

#define	CH452_I2C_ADDR0		0x40			// CH452的ADDR=0时的地址
#define	CH452_I2C_ADDR1		0x60			// CH452的ADDR=1时的地址,默认值
#define	CH452_I2C_MASK		0x3E			// CH452的2线接口高字节命令掩码

// *******************
// 类型定义
// *******************
typedef unsigned char UINT8;
typedef unsigned short UINT16;

// **************************************
// CH452的全局变量
// **************************************
UINT8 mADDR;//I2C地址


// *******************
// 延时uS
// *******************
void mDelayuS( UINT8 n ) 
{	
   while( --n ) 
   {
       NOP();
	   NOP();
   }
}

// *******************
// 延时mS
// *******************
void mDelaymS( UINT8 delay )
{ 	
   UINT8 ms;
   UINT8 sm;
   UINT8 c;
	
   for( ms = delay; ms != 0; ms-- )
   { 	
	   for( sm = 200; sm != 0; sm-- ) c += 3;  
	   for( sm = 200; sm != 0; sm-- ) c += 3;  
	   for( sm = 200; sm != 0; sm-- ) c += 3;  
	   for( sm = 200; sm != 0; sm-- ) c += 3;  
	   for( sm = 240; sm != 0; sm-- ) c += 3;  							 
   }	
}

// **************************************
// I2C START
// **************************************									   
void CH452_I2c_Start( void )
{
	CH452_SDA_SET; /*发送起始条件的数据信号*/
	CH452_SDA_OUT;  /* 设置SDA为输出方向 */
 	DELAY;
	
	CH452_SCL_SET;/* 设置SCL为输出方向 */
	DELAY;
	
	CH452_SDA_CLR;   /*发送起始信号*/
	DELAY;    
	  
	CH452_SCL_CLR;  /*钳住I2C总线，准备发送或接收数据 */
	DELAY;
}

// **************************************
// I2C STOP
// **************************************	
void CH452_I2c_Stop( void )
{
	CH452_SDA_CLR; 
	CH452_SDA_OUT; /* 设置SDA为输出方向 */
	DELAY;
	
	CH452_SCL_SET; 
	DELAY;
	
	CH452_SDA_SET; /*发送I2C总线结束信号*/
	DELAY;   
	
	CH452_SDA_IN; /* 设置SDA为输入方向 */
	DELAY; 
}

// **************************************
// I2C 写一个字节
// **************************************
void CH452_I2c_WrByte( UINT8 c )	//写一个字节数据
{
	UINT8 i;
	
	CH452_SDA_SET;
	CH452_SDA_OUT; /* 设置SDA为输出方向 */
	DELAY;
	
	for( i = 0; i != 8; i++ ) 
	{
		if( c & 0x80 ) { CH452_SDA_SET; }
		else { CH452_SDA_CLR; }
		DELAY;
		
		CH452_SCL_SET;
		
		c <<= 1;
		DELAY;
		
		CH452_SCL_CLR;
	}
	
	CH452_SDA_IN;   /* 设置SDA为输入方向 */
	DELAY;
	
	CH452_SCL_SET;  
	DELAY;
	
	CH452_SCL_CLR;
	DELAY;
}

// **************************************
// I2C 读一个字节
// **************************************
UINT8  CH452_I2c_RdByte( void )		//读一个字节数据
{
	UINT8 c, i;
	
	CH452_SDA_IN;   /* 设置SDA为输入方向 */
	
	c = 0;
	for( i = 0; i != 8; i++ )  // 输入8位数据
	{
		CH452_SCL_SET;
		DELAY;
		
		c <<= 1;
		if( CH452_SDA ) c++;  // 输入1位
		
		CH452_SCL_CLR;
		DELAY;
	}
	
	CH452_SDA_SET;
	CH452_SDA_OUT;
	
	CH452_SCL_SET;  // 发出无效应答
	DELAY;
	
	CH452_SCL_CLR;
	DELAY;
	
	return(c);
}

// **************************************
// CH452写命令
// **************************************
void CH452_Write( UINT16 command )	//写命令
{	
     CH452_I2c_Start();               /*启动总线*/
	 
	 if( mADDR == 0 )
	 {
   	     CH452_I2c_WrByte((UINT8)(command>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0);  // CH452的ADDR=0时
	 }
     else
     {
   	     CH452_I2c_WrByte((UINT8)(command>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1);  // CH452的ADDR=1时(默认)
	 }
	 
   	 CH452_I2c_WrByte( (UINT8)command );               /*发送数据*/
	 
  	 CH452_I2c_Stop();                 /*结束总线*/
}

// **************************************
// CH452读命令
// **************************************
UINT8 CH452_Read( UINT16 read_cmd )		
{
	UINT8 val;

   	CH452_I2c_Start();/*启动总线*/

	if( mADDR == 1 )
	{
   	     CH452_I2c_WrByte((UINT8)(read_cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR1|0x01);    // 若有两个CH452并连,当ADDR=0时,需修改为CH452_I2C_ADDR0
	}
	else
	{
	     CH452_I2c_WrByte((UINT8)(read_cmd>>7)&CH452_I2C_MASK|CH452_I2C_ADDR0|0x01);
	}

   	val = CH452_I2c_RdByte();      /*读取数据*/

	CH452_I2c_Stop();                /*结束总线*/
 
	return val;
}

// *******************
// CH452的初始化函数
// *******************
void CH452_Init( void )
{
	 CH452_SCL_OUT;
	 CH452_SDA_SET;
	 CH452_SDA_OUT;
	 CH452_INT_IN;
}



// *******************
// 主函数
// *******************
void main( void )
{
 	 UINT8 CH452_Key;
	 
 	 mDelaymS(20);
	 CH452_Init();
	 
	 //选择被操作的CH452地址  0和1可选
   	 mADDR = 1;//操作1地址CH452
   	 
   	 CH452_Write(0x403);//开显示键盘
   	 CH452_Write(0x500);//直接译码    
   	 CH452_Write(0x580);//BCD译码
  
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

        	 CH452_Key = CH452_Read(0x0700)&0x3F;

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

