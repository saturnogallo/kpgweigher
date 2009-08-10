//端口说明：
//*****
//P1口为键盘口
//P1.0为自动键
//P1.1为手动键
//P1.2为档位一
//P1.3为档位二
//P1.4为档位三
//P1.5为档位四
//P1.6为档位五
//P1.6为档位六
//******
//P2.1为检测溢出信号
//P2.0为检测欠量程信号
//*****
//P0.2为164清零信号
//P0.1为164脉冲信号
//P0.0为164数据信号

    #include <reg52.h>
	#include <stdio.h>
	#include <intrins.h>
	#define uchar unsigned char
	#define uint unsigned int
	#define ulong unsigned long
	sbit dat= P0^0;   //定义数据线
    sbit clk= P0^1;   //定义时钟端
	sbit clr= P0^2;   //清零端
    sbit J0= P1^0;   
    sbit J1= P1^1;   
	sbit J3= P1^2;   
    sbit J4= P1^3;   
    sbit J5= P1^4;   
	sbit J6= P1^5;   
    sbit J7= P1^6;   
    sbit J8= P1^7;   
	sbit YC= P2^1;  
    sbit QL= P2^0;

    uchar  anjianbaochi;//按键保持记忆  
    uchar  daout;//数据输出
    uchar  outjiance;//输出检测
    uchar  zdbz;//自动档标志位
    uchar  zdbaochi;
    uchar  anjianbz;
    uchar  aj;
    uint  ys;
    uchar code jdq[]={0x60,0x50,0x48,0x44,0x42,0x41,0x01,0x00}; // 0x20,0x10,0x08,0x04,0x02,0x01,0x41,0x00
    uchar code xsdw[]={0x48,0x88,0x24,0x44,0x84,0x22,0x42,0x00};// 0xb8,0x78,0xd4,0xb4,0x74,0xd2,0xb2,0x00
void wait(uint j)
  {
   uint i;
   for(i=0;i<j;i++)
      ;
   }
void d164()   //74164串行移位一个数
   {
         uchar i=0,ajbc,c;
         ajbc=anjianbaochi;
         c=jdq[ajbc];
         
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
               i=0;
               c=xsdw[ajbc];
           if(zdbz==0xff)
             c=c|0x01; 
             
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
        }
   
void keydsp()
  {
   uchar aj;
   anjianbaochi=~anjianbaochi;
   aj=anjianbaochi;
   if(anjianbaochi==1)
     { zdbz=0xff;
       anjianbaochi=0;
       d164();}
    else
     { 
     zdbz=0;
      if(anjianbaochi==2)
        {  anjianbaochi=0;
           d164();}
      else if(anjianbaochi==4)
        {  anjianbaochi=1;
           d164();} 
      else if(anjianbaochi==8)
        {  anjianbaochi=2;
           d164(); }
      else if(anjianbaochi==16)
        {  anjianbaochi=3;
           d164(); }
      else if(anjianbaochi==32)
        {  anjianbaochi=4;
           d164(); }
      else if(anjianbaochi==64)
        {  anjianbaochi=5;
           d164(); }
      else if(anjianbaochi==128)
        {  anjianbaochi=6;
           d164(); }
       else
         ;
      
   
      }
   } 
void key(void)
   {
 
    if(anjianbz!=0)
      {
       if(P1==0xff)
         keydsp();
         anjianbz=0;
       }
     if(P1!=0xff)
       {
       wait(2);
       if(P1!=0xff)
          anjianbaochi=P1;
          anjianbz=0xff;
       }
     
   }
void jiance(void)
   {
    ys=0;
    if(zdbz==0xff)
     {
    
      if(YC==1)
        {if(anjianbaochi!=0)
         { anjianbaochi=anjianbaochi-1; 
          d164();}
          }
      if(QL==1)
         {if(anjianbaochi!=6)
          {anjianbaochi=anjianbaochi+1;  
          d164();} 
          }
       
      }
    }
void main()
   {
    zdbz=0xff;
    while(1)
    {
    key();
    if(ys>0x3ff0)
      {jiance();}
     ys++;
    }
   }
    
    
