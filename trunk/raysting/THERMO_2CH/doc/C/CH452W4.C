/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2005.4
 CH452的4线接口子程序
****************************
 CH45X的4线接口，不含按键中断为3个I/O引脚，含按键中断为4个I/O，类似SPI时序，
 四线制方式也可以参照CH451，其中对CH451的选择4线串行接口初始化子函数可以省去
*************************************************************************** */
#include	"PIN.H"			// 修改该文件以适应硬件环境/单片机型号等
#include	"CH452CMD.H"	// CH452常量定义

 //输出操作命令子程序
 //定义一无符号整型变量存储12字节的命令字。
void CH452_Write(unsigned short cmd)
{
	unsigned char i;
	DISABLE_KEY_INTERRUPT;		  //禁止键盘中断,防止传输过程中被CH452中断而进入中断服务程序中再次传输
	CH452_LOAD_CLR;                 //命令开始,LOAD=0
	CH452_LOAD_D_OUT;		// 设置LOAD为输出方向
	CH452_DOUT_D_IN;		// 设置DOUT为输入方向
	CH452_DIN_D_OUT;		// 设置DIN为输出方向
	CH452_DCLK_D_OUT;		// 设置DCLK为输出方向
	for(i=0;i!=12;i++)				//送入12位数据，低位在前
	{
		if (cmd&1) {CH452_DIN_SET;}
		else {CH452_DIN_CLR;}  // 输出位数据
//		CH452_DIN=cmd&1;
    	CH452_DCLK_CLR;
    	cmd>>=1;
    	CH452_DCLK_SET;             //上升沿有效
	}
  	CH452_LOAD_SET;         //加载数据,LOAD上升沿
	DELAY_1US;				//命令数据加载周期不小于6US,CH451不需要该周期
	DELAY_1US;
	DELAY_1US;
	DELAY_1US;
	DELAY_1US;
	DELAY_1US;
  	ENABLE_KEY_INTERRUPT;
}

//*************************************************
//输入按键代码子程序，从CH452读取
unsigned char CH452_Read( void )
{
	unsigned char i;
  	unsigned char cmd,keycode;		//定义命令字，和数据存储器
  	DISABLE_KEY_INTERRUPT;			               //禁止键盘中断,防止传输过程中被CH452中断而进入中断服务程序中再次传输	
  	cmd=0x07;			               //读按键的命令字
  	CH452_LOAD_CLR;
  	for(i=0;i!=4;i++)  // 只需要发出高4位,多发也可以,但应该确保最后留下的4位是该命令码
	{
		if (cmd&1) {CH452_DIN_SET;}
		else {CH452_DIN_CLR;}  // 输出位数据
//		CH452_DIN=cmd&1;		      //送入最低位
    	CH452_DCLK_CLR;			 
    	cmd>>=1;			      //往右移一位
    	CH452_DCLK_SET;			      //产生时钟上升沿锁通知CH451输入位数据
 	}
  	CH452_LOAD_SET;				      //产生加载上升沿通知CH451处理命令数据
  	keycode=0;				      //清除keycode
  	for(i=0;i!=7;i++)
	{
		keycode<<=1;			      //数据移入keycode,高位在前,低位在后
		if (CH452_DOUT) keycode++;      //从高到低读入451的数据
//    	keycode|=CH452_DOUT;
    	CH452_DCLK_CLR;			      //产生时钟下升沿通知CH451输出下一位
    	CH452_DCLK_SET;
 	}
  	CLEAR_KEY_INTER_FLAG;	     //清中断标志
  	ENABLE_KEY_INTERRUPT;
  	return(keycode);			     //反回键值
}
