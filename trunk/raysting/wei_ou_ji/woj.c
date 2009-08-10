    #include <src51rd.h>
	#include <stdio.h>
	#include <intrins.h>
	#define uchar unsigned char
	#define uint unsigned int
	#define ulong unsigned long
	
	sbit dat= P1^7;   //定义数据线164
    sbit clk= P1^6;   //定义时钟端164
    sbit sclk= P1^0;  //串行时钟端
    sbit sdata= P1^1;   //串行数据线
    sbit drdy= P1^2;    //逻辑输出端
	sbit a0= P1^3;      //地址输入端
    sbit rfs= P1^4;    //同步接受端
    sbit tfs= P1^5;    //同步传输端
  

    sbit SER= P3^7;  //595数据端
    sbit SRCLK=P3^5; //串行数据端 
	sbit RCK= P3^6;  //数据输出端 
	sbit KG= P3^4;    //输出使能 

    sbit E_CLK =P2^3;//clock input                    同步时钟输入端
    sbit RW_SID=P2^2;//data input/output            串行数据输入、输出端//
    sbit RS_CS =P2^1;//chip select                    片选端//
    sbit PSB   =P2^4;//serial mode select            串口模式
    sbit RST   =P2^0;     
    

    uchar cr1,cr2,cr3,cr4;


    uchar keykeep;  //按键保持记忆
    uchar dw;       //档位寄存
    bit   Autod;     //自动档标志，1自动，0手动
    bit   career;   //速度标记，1快速，0慢速
    bit   reverse;  //电流换向标志，1正向，0反向 
   	ulong de;       //测量数据全局变量传递
 //   ulong jl;       //扣零量值 
    bit  ct;    //效准状态判断，1为当前效准、0为当前测量
    bit  cr;    //清零状态判断，1为清零状态、0为当前测量

    uchar quantifier;   //量程选择 0、2V ;1、200mV;2、20mv;3、单极2V;4、单极200mv;5、单极20mv
    uchar CH;           //通道选择
    uchar code rrc[]={0xdd,0x50,0x9b,0xd3,0x56,0xc7,0xcf,0xd0,0xdf,0xd6}; //LED显示
  //  float code dz[]={0.01,0.1,1,10,100,1000,10000};//标准电阻值表格
    uchar code tune[6]={0,0,0,0,0,0};     //调整数据
   // float code diagraph[]={0.671184,0.537451,0.537671,0.429600};//分度量化0.537037,0.268512
//	ulong code control[3]={0x03e304,0x03e334,0x03e31c}; //控制寄存器设定0x03e314
    uchar code jdq1[]={0xff,0x80,0x80,0x40,0x20,0x10,0x08,0x04,0x02};//档位控制
    uchar code jdq2[]={0x80,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40};
    ulong xdata DA[5];
 //   ulong DT[5]; 
    long  xdata ZL[6];       //增量计算，0为低;5为高   
    uint   LZ[6];
    cy();
    quantifier_with(float decimal );
    with();
  
	ulong code Sector[6][4]={
	                          {0x8000,0x8001,0x8002,0x8003},
							  {0x8200,0x8201,0x8202,0x8203},
							  {0x8400,0x8401,0x8402,0x8403},
							  {0x8600,0x8601,0x8602,0x8603},
							  {0x8800,0x8801,0x8802,0x8803},
							  {0x8a00,0x8a01,0x8a02,0x8a03},
							  };

extern void SectorErase(uint sector_addr);	// 扇区擦除
extern uchar byte_read(uint byte_addr);		// byte读
extern void byte_write(uint byte_addr, uchar original_data);	// byte写
extern uchar byte_write_verify(uint byte_addr, uchar original_data);// byte写并校验
extern uchar ArrayWrite(uint begin_addr, uint len, uchar code *array);	// byte数组写并校验
extern void ArrayRead(uint begin_addr, uchar len);		// 读出, 保存在Ttotal[]中


/* --------------- 命令定义 --------------- */
#define RdCommand		0x01	/* 字节读 	*/
#define PrgCommand		0x02	/* 字节写 	*/
#define EraseCommand	0x03	/* 扇区擦除 */

/* 定义常量 */
#define Error   1
#define Ok      0

/* 定义Flash对应于20MHz晶振系统的操作等待时间 */
/* 时钟倍频时WaitTime用 0x00*/

#define WaitTime	0x01

/* ------------- 定义扇区大小 ------------- */
#define PerSector		512

extern uchar Ttotal[];



/* ================ 打开 ISP,IAP 功能 ================= */
void ISP_IAP_enable(void)
{
	EA	=	0;							/* 关中断 		*/
	ISP_CONTR =	ISP_CONTR & 0x18;       /* 0001,1000	*/
	ISP_CONTR =	ISP_CONTR | WaitTime;	/* 写入硬件延时	*/
	ISP_CONTR =	ISP_CONTR | 0x80;       /* ISPEN=1		*/
}

/* =============== 关闭 ISP,IAP 功能 ================== */
void ISP_IAP_disable(void)
{
	ISP_CONTR	=	ISP_CONTR & 0x7f;	/* ISPEN = 0 */
	ISP_TRIG	=	0x00;
	EA			=   1;			/* 开中断 */
}
/* ================ 公用的触发代码 ==================== */
void ISPgoon(void)
{
	ISP_IAP_enable();			/* 打开 ISP,IAP 功能	*/
	ISP_TRIG	=	0x46;		/* 触发ISP_IAP命令字节1	*/
	ISP_TRIG	=	0xb9;		/* 触发ISP_IAP命令字节2	*/
	_nop_();
}
/* ==================== 字节读 ======================== */
uchar byte_read(uint byte_addr)
{   
	ISP_ADDRH = (uchar)(byte_addr >> 8);	/* 地址赋值	*/
	ISP_ADDRL = (uchar)(byte_addr & 0x00ff);

	ISP_CMD   = ISP_CMD	& 0xf8;			/* 清除低3位 	*/
	ISP_CMD   = ISP_CMD	| RdCommand;	/* 写入读命令	*/

	ISPgoon();							/* 触发执行		*/
	ISP_IAP_disable();				/* 关闭ISP,IAP功能	*/

	return (ISP_DATA);				/* 返回读到的数据	*/
}

/* ================== 扇区擦除 ======================== */
void SectorErase(uint sector_addr)
{
	uint iSectorAddr;
	iSectorAddr = (sector_addr & 0xfe00); /* 取扇区地址 */
	ISP_ADDRH = (uchar)(iSectorAddr >> 8);
	ISP_ADDRL = 0x00;

	ISP_CMD	= ISP_CMD & 0xf8;			/* 清空低3位 	*/
	ISP_CMD	= ISP_CMD | EraseCommand;	/* 擦除命令3 	*/

	ISPgoon();							/* 触发执行		*/
	ISP_IAP_disable();				/* 关闭ISP,IAP功能	*/

}


/* ==================== 字节写 ======================== */
void byte_write(uint byte_addr, uchar original_data)
{
    
	ISP_ADDRH =	(uchar)(byte_addr >> 8); 	/* 取地址 	*/
	ISP_ADDRL =	(uchar)(byte_addr & 0x00ff);

	ISP_CMD	 = ISP_CMD & 0xf8;				/* 清低3位	*/
	ISP_CMD  = ISP_CMD | PrgCommand;		/* 写命令2	*/
	ISP_DATA = original_data;			/* 写入数据准备	*/

	ISPgoon();							/* 触发执行		*/
	ISP_IAP_disable();					/* 关闭IAP功能	*/
}



    void delay(unsigned int n) 
{
  unsigned int i;
  for(i=0; i<n; i++) {;}
}
 //串行发送一字节数据
void SendByte(unsigned char dat)
{
     unsigned char i;
     for(i=0;i<8;i++)
           {
                 E_CLK=0;
                 if(dat&0x80)RW_SID=1;else RW_SID=0;
                 E_CLK=1;
                 dat=dat<<1;
            }
}
//串行接收一字节数据
unsigned char ReceieveByte(void)
{
     unsigned char i,d1,d2;
     for(i=0;i<8;i++)
           {
                 E_CLK=0;delay(100);
                 E_CLK=1;
                 if(RW_SID)d1++;
                 d1=d1<<1;
            }
     for(i=0;i<8;i++)
           {
                 E_CLK=0;delay(100);
                 E_CLK=1;
                 if(RW_SID)d2++;
                 d2=d2<<1;
            }
     return (d1&0xF0+d2&0x0F);
}
//读取标志位BF
bit ReadBF(bit bf)
{
     unsigned char dat;
     SendByte(0xFA);//11111,01,0 RW=1,RS=0
     dat=ReceieveByte();
     if(dat>0x7F)bf=1;else bf=0;
     return bf;
     }      
//写控制命令
void SendCMD(unsigned char dat)
{
//      while(ReadBF){;}
//      RS_CS=1;
     SendByte(0xF8);//11111,00,0 RW=0,RS=0 同步标志
     SendByte(dat&0xF0);//高四位
     SendByte((dat&0x0F)<<4);//低四位
//      RS_CS=0;
}
//写显示数据或单字节字符
void SendDat(unsigned char dat)
{
//      while(ReadBF){;}
//      RS_CS=1;
     SendByte(0xFA);//11111,01,0 RW=0,RS=1
     SendByte(dat&0xF0);//高四位
     SendByte((dat&0x0F)<<4);//低四位
//      RS_CS=0;
}      
/*      写汉字到LCD 指定的位置
     x_add显示RAM的地址
     dat1/dat2显示汉字编码
*/
void display(unsigned char x_add,unsigned char dat1,unsigned char dat2){
     SendCMD(x_add);//1xxx,xxxx 设定DDRAM 7位地址xxx,xxxx到地址计数器AC
     SendDat(dat1);
     SendDat(dat2);
}
//初始化 LCM
void initlcm(void)
{
     RST=0;
//      RS_CS=0;
//      PSB=0;//serial mode
     delay(100);
     RST=1;
     SendCMD(0x30);//功能设置，一次送8位数据，基本指令集
    SendCMD(0x0C);//0000,1100  整体显示，游标off，游标位置off
     SendCMD(0x01);//0000,0001 清DDRAM
     SendCMD(0x02);//0000,0010 DDRAM地址归位
     SendCMD(0x80);//1000,0000 设定DDRAM 7位地址000，0000到地址计数器AC//    
    SendCMD(0x04);//点设定，显示字符/光标从左到右移位，DDRAM地址加 一//    
     SendCMD(0x0c);//显示设定，开显示，显示光标，当前显示位反白闪动
}
    /*------------------------*/
    void wait(uint j)
    {
     uint i,x;     for(i=0;i<j;i++)
        {
         for(x=0;x<200;x++)  
           ;
          } 
     }

    /*------------------------*/
   void d595(uchar n)   //74164串行移位一个数
        {
         uchar i=0;
         uchar val=jdq1[n];
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
        wait(15);
        KG=1; 
      
        }
//
	void write(ulong j)
	   {  
        uint i;
        sclk=0;
     	tfs=0;
		rfs=1;
	    a0=0;
		for(i=0;i<25;i++)
		 {
		  sclk=0;
		  _nop_();
	    
   	      sclk=1;
		    sdata=j&1;
		  _nop_();
		  
		  j=(j>>=1);
		  }
         tfs=1;       
	  }
/*-----------------------*/

collate()
{

  
  if(ct==1)
   { 
     ct=0;
	 if(dw==2||dw==3||dw==4||dw==5||dw==6)
	   write(0x03e334);
	  else
	   write(0x03e31c);
     with();
//	 de=de-LZ[dw-1];
     quantifier_with(1);
     ZL[dw-1]=de;
     SectorErase(Sector[dw-1][2]); 
	 byte_write(Sector[dw-1][0],ZL[dw-1]&0xff);
	 byte_write(Sector[dw-1][1],ZL[dw-1]>>8&0xff);
	 byte_write(Sector[dw-1][2],ZL[dw-1]>>16&0xff);
	 byte_write(Sector[dw-1][3],ZL[dw-1]>>24&0xff);

     display(0x9c,0xd0,0xa3);
     wait(1000);
     wait(1000);
    }
}
clear()
  {
  // float ctd;
   if(cr==1)
   { 
   cr=0;
   with();
  /* ZL[dw-1]=byte_read(Sector[dw-1][3])*16777216+byte_read(Sector[dw-1][2])*65535+byte_read(Sector[dw-1][1])*256+byte_read(Sector[dw-1][0]);
   ctd=0.0000001*ZL[dw-1];
   */
   quantifier_with(1);
   LZ[dw-1]=de;
   display(0x9c,0xc7,0xe5); 
   wait(1000);
   wait(1000);
   }
   
   }

/*-----------------------*/  
keysetdisposal()
{
  
  
 if(keykeep==1)
  {
   Autod=~Autod; 
   keykeep=0;
   display(0x82,0xd7,0xd4);
     display(0x83,0xb6,0xaf);
   }
  else if(keykeep==2)
    { 
      
      career=~career;
      keykeep=0;}
   else if(keykeep==6)
    { 
       Autod=0;
       cr=~cr;
       clear();
       keykeep=0; }
   else if(keykeep==7)
    {
     Autod=0;
     ct=~ct; 
     collate(); 
     keykeep=0;
      }
 else if(keykeep==3||keykeep==4||keykeep==5||keykeep==8||keykeep==9||keykeep==10)
   {
     Autod=0;
     switch(keykeep)
      {
       case 3:
          dw=1; 
          display(0x82,0xb5,0xb5);
          display(0x83,0xa2,0xf1);    
          break;
       case 4:
          dw=2;
          write(0x03e334);
          display(0x82,0xb5,0xb5);
          display(0x83,0xa2,0xf2);
          break;
       case 5:
          dw=3; 
          write(0x03e334);
          display(0x82,0xb5,0xb5);
          display(0x83,0xa2,0xf3);
          break;
       case 8:
          dw=4;
          write(0x03e334);
          display(0x82,0xb5,0xb5);
          display(0x83,0xa2,0xf4);
          break;
       case 9:
          dw=5;
          write(0x03e334);
          display(0x82,0xb5,0xb5);
          display(0x83,0xa2,0xf5);
          break;
       case 10:
          dw=6;
          write(0x03e334);
          display(0x82,0xb5,0xb5);
          display(0x83,0xa2,0xf6);
          break;
       default:
          break;
        }
      keykeep=0;
      d595(0);
      wait(15);
      dw=dw;
      d595(dw);  
    }

   
     
 else
   keykeep=0;

}
/*------------------------*/
keyset()
 {
  uchar keykp;
  if(keykeep!=0)
   {
     if((0x0f&P0)==0x0f)
       keysetdisposal();
    }
  if((0x0f&P0)!=0x0f)
    {         keykp=0x0f&~P0; 
     //  wait(10);
       if(keykp==(0x0f&~P0))
         {keykeep=keykp;
          keykp=0;}
      }
  }
/*---------------------*/

     void d164(uchar n)   //74164串行移位一个数
        {
         uchar i=0;
         uchar c=rrc[n];
         do{  
             if(c&0x01)     
                   	dat=1;
                    else
					dat=0;
		     c>>=1;        //数组tabel中的第N个数向右移动一位
             clk=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
             clk=1;
             clk=0;
             i++;
            }
        while(i<8);
             c=0;
        }

     display1()
        {
		  uchar d1,d2,d3,d4,d5,d6,d7;
	      signed long de1;
          
          de1=de;
          if(de1>0)
            {
          if(de1>29000000)
           {
             display(0x80,0xb3,0xac);
              
             display(0x90,0xa1,0xa0);
             display(0x91,0xa1,0xa0);
             display(0x92,0xa1,0xa0);
             display(0x93,0xa1,0xa0);
             display(0x94,0xa1,0xa0);
             display(0x95,0xa1,0xa0);
             display(0x96,0xa1,0xa0);
             display(0x97,0xa1,0xa0);
             
            
            }
          else if(de1<=29000000)
          {
          
          display(0x80,0xa1,0xa0); 
		  if(de1<0)
		  de1=10;
		  else
		    ;
          d1=de1/0x989680;
  		  d2=(de1%0x989680)/0xf4240;
		  d3=((de1%0xf989680)%0xf4240)/0x186a0;
		  d4=(((de1%0xf989680)%0xf4240)%0x186a0)/0x2710;
		  d5=((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)/0x3e8;
		  d6=(((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)%0x3e8)/0x64;
          d7=((((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)%0x3e8)%0x64)/10;
		  
          if(dw==1||dw==4)
           {
            display(0x90,0xa3,0xb0+d1);
            display(0x91,0xa3,0xb0+d2);
            display(0x92,0xa3,0xae);
            display(0x93,0xa3,0xb0+d3);
            display(0x94,0xa3,0xb0+d4);
            display(0x95,0xa3,0xb0+d5);
            display(0x96,0xa3,0xb0+d6);
            display(0x97,0xa3,0xb0+d7);
            
            }
           else if(dw==2||dw==5)
            {
            display(0x90,0xa3,0xb0+d1);
            display(0x91,0xa3,0xb0+d2);
            display(0x92,0xa3,0xb0+d3);
            display(0x93,0xa3,0xae);
            display(0x94,0xa3,0xb0+d4);
            display(0x95,0xa3,0xb0+d5);
            display(0x96,0xa3,0xb0+d6);
            display(0x97,0xa3,0xb0+d7);
            
             }
           else if(dw==3||dw==6)
            {
            display(0x90,0xa3,0xb0+d1);
            display(0x91,0xa3,0xae);
            display(0x92,0xa3,0xb0+d2);
            display(0x93,0xa3,0xb0+d3);
            display(0x94,0xa3,0xb0+d4);
            display(0x95,0xa3,0xb0+d5);
            display(0x96,0xa3,0xb0+d6);
            display(0x97,0xa3,0xb0+d7);
             }
           else 
             ;
           if(dw==1||dw==2)
             display(0x85,0xa3,0xed);
             else if(dw==3||dw==3||dw==5)
              display(0x85,0xa1,0xa0);
               else if(dw==6)
                display(0x85,0xa3,0xcb);   
            }
          
           
           else  
             ;
            }
    /*       else if(de1<0) 
           { display(0x90,0xa1,0xa0);
             display(0x91,0xa1,0xa0);
             display(0x92,0xa1,0xa0);
             display(0x93,0xa1,0xa0);
             display(0x94,0xa1,0xa0);
             display(0x95,0xa1,0xa0);
             display(0x96,0xa1,0xa0);
             display(0x97,0xa1,0xa0); }
           else  
             ;  */
           if(career==1)
             {display(0x99,0xa2,0xf1);
              }
           else
             {display(0x99,0xa2,0xf2);}
            
           }
	 read7710()
	   {
	    uint j;
        sclk=0;
		a0=1;
		tfs=1;
		rfs=1;
		sdata=1;
   	RD:	
		    de=0;
 		    if(drdy==0)
		       { rfs=0 ;
                 for(j=0;j<0x18;j++)
			     {sclk=0;
				  _nop_();
				  sclk=1;
				  _nop_();
			      de=de|sdata;
				  sclk=0;
			      de<<=1;
				  
				    if (drdy==1)
					 j=0x18;
			     }
			 
               }
			else 
              goto RD;
	 }
   void yanshi(void)
	   {
	    ulong i;
		for(i=0;i<1;i++)
		  
		  ;
        }

     with()
	     {
		  uchar i;
		  ulong dx1,d0,d1,d2,d3,d4,d5;
		  dx1=0;
		  for(i=0;i<2;i++)
		  {
           read7710();
           d0=de;
		   read7710();
		   d1=de;
           read7710();
           d2=de;
		   read7710();
		   d3=de;
		   read7710();
           d4=de;
		   read7710();
		   d5=de;
    	   if(d0<d1)
	 		  d0=d1;
	   	  if(d0<d2)
			  d0=d2;
 		  if(d0<d3)
			  d0=d3;
		  if(d0<d4)
			  d0=d4;
		  if(d0<d5)
		      d0=d5;
		  if(d1>d2)
			  d1=d2;
		  if(d1>d3)
			  d1=d3;
 		  if(d1>d4)
			  d1=d4;
		  if(d1<d5)
			  d1=d5;
		   de=d2+d3+d4+d5;
		   dx1=dx1+de;
		   }
		 de=dx1/8;
	   }
     quantifier_with(float decimal )     
      {
	 
        if(de>16777216)
	     de=de-16777216;
	   	else
         de=16777216-de;          
		  de=(de-LZ[dw-1])/decimal;
		  
        display1(); 
	   }
   qh()
    {
    
    if(de<1000000)
         { 
           if(dw!=1)
           dw=dw-1;
           d595(0);
           wait(15);
           
           d595(dw);
          }
    else if(de>20000000)
         { 
           if(dw!=6)
           dw=dw+1;
           d595(0);
           wait(15);
           
           d595(dw);
           }
      
    else
      ;
   }
/*------------------------------*/



/*------------------------------*/
cy()
  {
   float ctd;//,cd1;
  // uchar cr11,cr22,cr33,cr44;
  // ulong f1,f2,f3,f4;
  // cr11=cr1;cr22=cr2;cr33=cr3;cr44=cr4;

   if(ct==0)
   {
   if(career==1)
     { read7710(); 
      }
     
    else
     with(); 
   if(dw==2||dw==3||dw==4||dw==5||dw==6)
    {
    ZL[dw-1]=byte_read(Sector[dw-1][3])*16777216+byte_read(Sector[dw-1][2])*65535+byte_read(Sector[dw-1][1])*256+byte_read(Sector[dw-1][0]);
    write(0x03e334);
    ctd=0.0000001*ZL[dw-1];
    quantifier_with(ctd);
     }
      else if(dw==1)
      { write(0x03e31c);
	    ZL[dw-1]=byte_read(Sector[dw-1][3])*16777216+byte_read(Sector[dw-1][2])*65535+byte_read(Sector[dw-1][1])*256+byte_read(Sector[dw-1][0]);
        ctd=0.0000001*ZL[dw-1];
	    quantifier_with(ctd);
       }
       else
         ;
   
   }
  display(0x9c,0xb2,0xe2);
  }

   main( )
     {
	
     RST=1;
     PSB=0; 
     RS_CS=1; 
     initlcm();
     SendCMD(0x81);//1000,0001 设定DDRAM 7位地址000，0001到地址计数器AC
     SendDat(0x33);
     SendDat(0x42);
     SendDat(0x43);
     SendDat(0x44);
     SendCMD(0x00);
     d595(0); 
    
     write(0x03e334);
     display(0x86,0xa6,0xb8);
     display(0x98,0xcb,0xd9);
	      
   do{
      
       cy();       //测量状态调用
       keyset(); 
	  
           }
 	  while(1);
      }
     
