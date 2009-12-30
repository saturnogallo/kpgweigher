    #include <stc51.h>
	#include <stdio.h>
	#include <intrins.h>
	#include<MATH.H>
 	#define uchar unsigned char
	#define uint unsigned int
	#define ulong unsigned long
 ulong code Sector[10][4]={
	                       {0x8000,0x8001,0x8002,0x8003},
			               {0x8200,0x8201,0x8202,0x8203},
	           	           {0x8400,0x8401,0x8402,0x8403},
			               {0x8600,0x8601,0x8602,0x8603},
			               {0x8800,0x8801,0x8802,0x8803},
			               {0x8a00,0x8a01,0x8a02,0x8a03},
	        	           {0x8c00,0x8c01,0x8c02,0x8c03},
			               {0x8e00,0x8e01,0x8e02,0x8e03},
			               {0x9000,0x9001,0x9002,0x9003},
			               {0x9200,0x9201,0x9202,0x9203},						 
			                  };



extern void SectorErase(uint sector_addr);	// 扇区擦除
extern uchar byte_read(uint byte_addr);		// byte读
extern void byte_write(uint byte_addr, uchar original_data);	// byte写
extern uchar byte_write_verify(uint byte_addr, uchar original_data);// byte写并校验
extern uchar ArrayWrite(uint begin_addr, uint len, uchar code *array);	// byte数组写并校验
extern void ArrayRead(uint begin_addr, uchar len);		// 读出, 保存在Ttotal[]中

/*********************************************************************
                           EEPROM使用
*********************************************************************/
                /* - 命令定义 - */
#define RdCommand		0x01	/* 字节读 	*/
#define PrgCommand		0x02	/* 字节写 	*/
#define EraseCommand	0x03	/* 扇区擦除 */
                /* 定义常量 */
#define Error   1
#define Ok      0
/* 定义Flash对应于20MHz晶振系统的操作等待时间 */
/* 时钟倍频时WaitTime用 0x00*/

#define WaitTime	0x01
               /* - 定义扇区大小 - */
#define PerSector		512
extern uchar Ttotal[];
               /* = 打开 ISP,IAP 功能 = */
void ISP_IAP_enable(void)
{
	EA	=	0;			/* 关中断       */
	ISP_CONTR =	ISP_CONTR & 0x18;       /* 0001,1000	*/
	ISP_CONTR =	ISP_CONTR | WaitTime;	/* 写入硬件延时	*/
	ISP_CONTR =	ISP_CONTR | 0x80;       /* ISPEN=1	*/
}
                 /* = 关闭 ISP,IAP 功能 = */
void ISP_IAP_disable(void)
{
	ISP_CONTR	=	ISP_CONTR & 0x7f;	/* ISPEN = 0 */
	ISP_TRIG	=	0x00;
	EA			=   1;			/* 开中断    */
}
                /* = 公用的触发代码 = */
void ISPgoon(void)
{
	ISP_IAP_enable();			/* 打开 ISP,IAP 功能	*/
	ISP_TRIG	=	0x46;		/* 触发ISP_IAP命令字节1	*/
	ISP_TRIG	=	0xb9;		/* 触发ISP_IAP命令字节2	*/
	_nop_();
}
                   /* = 字节读 = */
uchar byte_read(uint byte_addr)
{   
	ISP_ADDRH = (uchar)(byte_addr >> 8);	        /* 地址赋值     */
	ISP_ADDRL = (uchar)(byte_addr & 0x00ff);
	ISP_CMD   = ISP_CMD	& 0xf8;			/* 清除低3位 	*/
	ISP_CMD   = ISP_CMD	| RdCommand;	       /* 写入读命令	*/
	ISPgoon();					/* 触发执行	*/
	ISP_IAP_disable();				/* 关闭ISP,IAP功能*/
	return (ISP_DATA);				/* 返回读到的数据*/
}
                  /* = 扇区擦除 = */
void SectorErase(uint sector_addr)
{
	uint iSectorAddr;
	iSectorAddr = (sector_addr & 0xfe00);         /* 取扇区地址 */
	ISP_ADDRH = (uchar)(iSectorAddr >> 8);
	ISP_ADDRL = 0x00;
	ISP_CMD	= ISP_CMD & 0xf8;		      /* 清空低3位 	*/
	ISP_CMD	= ISP_CMD | EraseCommand;	      /* 擦除命令3 	*/
	ISPgoon();					/* 触发执行	*/
	ISP_IAP_disable();				/* 关闭ISP,IAP功能*/

}
               /* = 字节写 = */
void byte_write(uint byte_addr, uchar original_data)
{
  	ISP_ADDRH =	(uchar)(byte_addr >> 8); 	/* 取地址 	*/
	ISP_ADDRL =	(uchar)(byte_addr & 0x00ff);
	ISP_CMD	 = ISP_CMD & 0xf8;			/* 清低3位	*/
	ISP_CMD  = ISP_CMD | PrgCommand;		/* 写命令2	*/
	ISP_DATA = original_data;			/* 写入数据准备	*/
	ISPgoon();				        /* 触发执行	*/
	ISP_IAP_disable();				/* 关闭IAP功能	*/
}


/********举例说明********************************************
  byte_read(Sector[1][3])        读一个字节
  byte_write(Sector[1][3],char)  把char写到这个字节中
   SectorErase(Sector[1][2])     扇区清除 
**/
