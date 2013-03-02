#include "define.h"
#include "mega16.h"
//ATMEGA16 PC0   -------    CH452 SCL
//ATMEGA16 PC1   -------    CH452 SDA

#define TWINT	7
#define TWEA	6
#define TWSTA 	5
#define TWSTO	4
#define TWWC	3
#define TWEN	2
#define TWIE	0

#define TWPS0	0
#define TWPS1	1

#define PC0     0
#define PC1     1

#define  SLA_W    			0x32
#define  SLA_R    			0x33
#define  SLAVER_ADDRESS     0x32
#define  START              0x08
#define  RE_START           0x10
#define  MT_SLA_WRITE_ACK   0x18
#define  MT_SLA_READ_ACK    0x40
#define  MT_DATA_ACK        0x28
#define  MT_READ_ACK        0x58 
   


#define CMD_CFG_SYSTEM		0x4843					//0x4800| 01000011[SLEEP][INTENS]0[X_INT]0[KEYB][DISP]B
#define CMD_READ_KEY		0x4F00

#define CH453_START()  			TWCR = ( 1<<TWEN )|(1<<TWSTA )|(1<<TWINT); while( !(TWCR & (1<<TWINT)) )//发 START
#define CH453_WAIT()	 		while( !(TWCR & (1<<TWINT)) )
#define CH453_STOP()			TWCR = ( 1<<TWEN )|(1<<TWSTO)|(1<<TWINT)
#define CH453_Wr_Byte(twi_d)	{TWDR=(twi_d);TWCR=(1<<TWINT)|(1<<TWEN); while( !(TWCR & (1<<TWINT)) );}
#define CH453_Rd_Byte(twi_d)	{TWCR=(1<<TWINT)|(1<<TWEN); while( !(TWCR & (1<<TWINT)) ); twi_d = TWDR;}
#define TestACK()		 		(TWSR&0xF8)    		//取出状态码
#define SetACK() 				(TWCR|=(1<<TWEA))   //产生ACK
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
// I2C写CH452命令
// **************************************
void CH453_Write( u16 cmd )
{
     u8 try;//重试次数
	 
	 try = 50;
	 do
	 {
		 CH453_START();
	 
   	     CH453_Wr_Byte((u8)(cmd>>8));    //command
		
	 	 if( TestACK() == MT_SLA_WRITE_ACK )//收到ACK
	 	 {
	  	  	 CH453_Wr_Byte( (u8)cmd );
			 CH453_STOP();
			 break;
	 	 }
	 	 
		 CH453_STOP();
		 
		 if( try ) try--;
	 } while( try );
	 
}

// **************************************
// I2C读CH452命令
// **************************************
u8 CH453_Read( u16 read_cmd )		
{
    u8 try;//重试次数
	u8 val;
	
	try = 5;
	do
	{
	  	val = 0xFF;
		
   	  	CH453_START();

   	    CH453_Wr_Byte((u8)(read_cmd>>8));    
   	    
//                sleepms(10);
		if( TestACK() == MT_SLA_READ_ACK )//收到ACK
   		{
			CH453_Rd_Byte(val);

			CH453_STOP();
			break;
    	        }
		
		CH453_STOP();
		
		if( try ) try--;
 	}while( try );
	return val;
}
// **************************************
// CH453初始化
// **************************************
void CH453_I2C_Init( void )
{
 	 PORTC |= (1<<PC1)|(1<<PC0);
	 DDRC |= (1<<PC1)|(1<<PC0);
   	 TWBR = 0x64; 
	 TWSR = (1<<TWPS1)|(1<<TWPS0);
   	 TWCR = ( 1<<TWEN ); //使能TWI
	 mDelaymS(100);
	 CH453_Write(CMD_CFG_SYSTEM);
}

static u8 lastkey = 0xff;
u8 CH453_ReadKey()
{
	u8 key;
	key = CH453_Read(CMD_READ_KEY);
	if(key == 0xff)
		return 0xff;
	if((key & 0x40)) //key down
	{
		lastkey = key & 0x0f;
		return 0xff;
	}else{
	        if(lastkey != 0xff)
        	{	//key up
	        	key = lastkey;
        		lastkey = 0xff;
		        return key;
	        }     
	}                  
	return 0xff;
}
