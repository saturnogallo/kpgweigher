#include <stc51.h>
#include <stdio.h>
#include <intrins.h>
#include "math.h"

#define logl log
#define powl pow

#define F_TM F0

#define TIMER0_ENABLE  TL0=TH0; TR0=1;
#define TIMER0_DISABLE TR0=0;

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define BOOL	unsigned char
#define BYTE 	unsigned char
#define FALSE	0
#define TRUE	1

//#define MODE_SCANNER	0
//#define MODE_DMM		1

sbit  LED=P0^0;

uchar CH;           //通道选择


/*
void delay(unsigned int n) 
{
  unsigned int i;
  for(i=0; i<n; i++) {;}
}
*/
void delay(int abc)
{
	uchar i = 0;
	while(abc > 0)
	{
		abc--;
		for(i=0;i<50;i++)
			;
	}
}
/* Wendu convert code*/
const double code TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
const double code THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};

const double code SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
const double code SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
const double code SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};

const double code RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};

const double code RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
const double code RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};

const double code NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
const double code NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};

const double code JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
const double code JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};

const double code BLow[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
const double code BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};

const double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};	
const double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};	

const double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
const double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};

double GetThmoVolt(double t,char type)
{
	double result,pert;
	int i;
	const int code TLowLen = sizeof(TLow)/sizeof(double);
	int code THighLen = sizeof(THigh)/sizeof(double);

	int code SLowLen = sizeof(SLow)/sizeof(double);
	int code SMedLen = sizeof(SMed)/sizeof(double);
	int code SHighLen = sizeof(SHigh)/sizeof(double);

	int code RLowLen = sizeof(RLow)/sizeof(double);
	int code RMedLen = sizeof(RMed)/sizeof(double);
	int code RHighLen = sizeof(RHigh)/sizeof(double);

	int code NLowLen = sizeof(NLow)/sizeof(double);
	int code NHighLen = sizeof(NHigh)/sizeof(double);

	int code JLowLen = sizeof(JLow)/sizeof(double);
	int code JHighLen = sizeof(JHigh)/sizeof(double);

	int code BLowLen = sizeof(BLow)/sizeof(double);
	int code BHighLen = sizeof(BHigh)/sizeof(double);

	int code ELowLen = sizeof(ELow)/sizeof(double);
	int code EHighLen = sizeof(EHigh)/sizeof(double);

	int code KLowLen = sizeof(KLow)/sizeof(double);
	int code KHighLen = sizeof(KHigh)/sizeof(double);

	const double *coef;
	int len = 0;
	switch(type)
	{
		case 'T': 	
			coef = (t < 0) ? TLow : THigh;
			len = (t < 0) ? TLowLen : THighLen;
			break;
		case 'K': 	
			coef = (t < 0) ? KLow : KHigh;
			len = (t < 0) ? KLowLen : KHighLen;
			break;
		case 'N': 	
			coef = (t < 0) ? NLow : NHigh;
			len = (t < 0) ? NLowLen : NHighLen;
			break;
		case 'E': 	
			coef = (t < 0) ? ELow : EHigh;
			len = (t < 0) ? ELowLen : EHighLen;
			break;
		case 'B': 	
			coef = (t < 630.615) ? BLow : BHigh;
			len = (t < 630.615) ? BLowLen : BHighLen;
			break;
		case 'J': 	
			coef = (t < 760) ? JLow : JHigh;
			len = (t < 760) ? JLowLen : JHighLen;
			break;

		case 'S': 	
			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
			break;
		case 'R': 	
			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
			break;
		default:
			return -9999.9;
	}
	if (len == 0)
		return -9999.9;

	result = coef[0];
	if(type == 'K')
	{
		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	}

	pert = t;
	for(i=1;i< len ;i++)
	{
		result = result + coef[i] * t;
		t = t*pert;
	}
	return result;
}

double MValueToTValue(double r,char type)
{
	double tlow;
	double tup;
	double rnew;
	double tnew;
	int count = 0;


	switch(type)
	{
		case 'T': 	
			tlow =  -270;	tup = 400;
			break;
		case 'K': 	
			tlow =  -270;	tup = 1372;
			break;
		case 'N': 	
			tlow =  -270;	tup = 1300;
			break;
		case 'E': 	
			tlow =  -270;	tup = 1000;
			break;
		case 'B': 	
			tlow =  0;	tup = 1280;
			break;
		case 'J': 	
			tlow =  -210;	tup = 1200;
			break;
		case 'S': 	
			tlow =  -50;	tup = 1768;
			break;
		case 'R': 	
			tlow =  -50;	tup = 1768;
			break;
		default:
			return -9999.9999;
	}	

	while((tup - tlow > 0.00005) || (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetThmoVolt(tnew,type);
		
		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
				
	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
}

/* 164 related*/
sbit dat= P0^1;   //定义数据线
sbit clk= P0^0;   //定义时钟端

uchar code rrc[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                  0x03,0x62,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                  0xfd,0x61,0xdB,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,}; //LED显示


void d164(uchar n)   //74164串行移位一个数
{
         uchar i=0;
         uchar c=rrc[n];
         do{  
             if(c&0x01){
               	dat=1;
             }else{
		dat=0;
	     }
	     c>>=1;        //数组tabel中的第N个数向右移动一位
	      _nop_(); _nop_();
             clk=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
             _nop_(); _nop_();
	      clk=1;
	     _nop_(); _nop_();
             clk=0;
	     _nop_(); _nop_();
             i++;
         }while(i<8);
}

void display164(ulong de1)
{
	uchar d1,d2,d3,d4,d5,d6,d7;
	d1=de1/0x989680;
	d2=(de1%0x989680)/0xf4240;
	d3=((de1%0xf989680)%0xf4240)/0x186a0;
	d4=(((de1%0xf989680)%0xf4240)%0x186a0)/0x2710;
	d5=((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)/0x3e8;
	d6=(((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)%0x3e8)/0x64;
	d7=((((((de1%0x989680)%0xf4240)%0x186a0)%0x2710)%0x3e8)%0x64)/10;
	d164(d1+20);
	d164(d2);
	d164(d3);
	d164(d4);
	d164(d5);
	d164(d6);
	d164(d7);
	d164(0);
}


    ulong code Sector[2][4]={
	                       {0x8000,0x8001,0x8002,0x8003},
			               {0x8200,0x8201,0x8202,0x8203},
						   }   ;




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

/*interface to main board*/

#define BUS_IDLE	0	//searching for sync
#define BUS_SYNCED	1	//waiting for data filling
#define BUS_SENDING	2	//is sending out data

#define HEAD_MARK	0xcc00	//header mark
#define HEAD_MASK	0xff00	//header mark

/*Serial related code*/
#define SP_BUFFER_MAX	18
void sjSerialInit();///< init the serial setting  on PortID
BOOL sjSerialIsDataWaiting();///<check if there data waiting on port
void sjSerialSendByte(BYTE udata);///<byte send on PortID
BYTE sjSerialWaitForOneByte();///<wait until one BYTE appear on port
BYTE spbuf[SP_BUFFER_MAX];	///< serial port fifo buffer
static BYTE *spHead,*spTail;	///< buffer head and tailst
static BYTE spSFlag;///<serial port flag for send use

//const for SerialPort Status
#define SPBUSY 	0xd6	
#define SPIDLE 	0xd8


/**
*	@brief Set Initially for all serial port(under construction)
*	
*	Serial port format will be set identically
*/
void sjSerialInit()
{
	spHead=spbuf;
	spTail=spbuf;
	spSFlag=SPIDLE;
}

/********************************
*	Add the byte recieved to the 
*	Tail of the buffer
********************************/
void sjSerialHandler(void) interrupt 4 using 2
{
	//just handle serial interrupt 1
	if(TI)
	{
		TI = 0;
		spSFlag = SPIDLE;	
	}
	if(RI)
	{
		*spTail = SBUF;
		spTail = spTail+1;
		if(spTail == (spbuf+SP_BUFFER_MAX))
			spTail = spbuf;
		RI = 0;
	}
}

/**
*	@return TRUE when data availabel ,FALSE while not
*/
BOOL sjSerialIsDataWaiting()
{
	if(spHead == spTail)
		return FALSE;
	else 
		return TRUE;
}
/**
*	Send One Byte on Serial port , (under construction)
*	and return with no wait for send finished
*	@param udata the Byte to send
*	@param PortID serial port to use
*/
void sjSerialSendByte(BYTE udata)
{
	spSFlag=SPBUSY;
	//Send the udata
	SBUF = udata;
	while(spSFlag == SPBUSY)	
		;
}
/**
*	Wait One Byte until that appeared,
*	it perform read byte function at the same time
*	@param PortID serial port to use
*	@return Byte we received
*/
BYTE sjSerialWaitForOneByte()
{
	BYTE udata;
	while(spHead == spTail)	
		;
	udata = *spHead;
	spHead = spHead+1;
	if(spHead == (spbuf+SP_BUFFER_MAX))
		spHead = spbuf;
	return udata;
}

void _uart_str(char *str)
{
	while(*str != '\0') {
		sjSerialSendByte(*str++);
	}
}


uint busbuf = 0; //buffer to store incoming bits
uchar busstate = BUS_IDLE;
uchar obit_cnt; //输出bit 计数
uchar obit_numbits; //要输出bit数
sbit busIbit = P2^7;	//INPUT
sbit busObit = P2^4;	//OUTPUT
sbit busCbit = P2^5;	//CLOCK

uchar *da;
#define CMD_INVALID		0xfe
#define CMD_ACCEPT		0xa5
uchar ack_ok = CMD_ACCEPT;   //ack of command received.
#ifdef MODE_SCANNER
//all scanner command <  0x80
#define CMD_SCAN_SETCH_MAX	24	//set channel command 1 to 24
#define CMD_SCAN_UPDATE		0x70    //update the status of scanner
#define CMD_SCAN_PKTT		0x71	//switch to pktt
#define CMD_SCAN_NKTT		0x73	//switch to nktt
#define CMDR_SCAN_CHTYPE	0x74	//scanner channel type 2 wire or 4 wire
#define CMDR_SCAN_CHCURR	0x76	//current channel index
#define CMDR_SCAN_CHNUM		0x78	//current channel number
#define CMDR_SCAN_STATE		0x72    //read ch_action

#define SCAN_RESERVE	0x00
#define SCAN_ACTION	0x40
#define	SCAN_QUERY	0x80
#define RET_ERROR	0xC0
#define STATE_MASK	0xC0
#define	WIRE_2		0x00
#define WIRE_4		0x20
#define	WIRE_MASK	0x20
#define POS_MASK	0x1f

uchar ch_type = 0;
uchar ch_curr = 0;
uchar ch_num = 1;
uchar ch_action = 0xff; //action buffer for main loop

void query_scanner()
{
	int i;
	uchar j;

	ch_type = 2;
	ch_num = 24;
	ch_curr = 2;
	return;
	sjSerialSendByte(SCAN_QUERY);
	i = 10;
	while(i-- > 0)
	{
		if(sjSerialIsDataWaiting() == FALSE)
		{
			delay(255);
			continue;
		}
		j = sjSerialWaitForOneByte();	
		ch_type = j & WIRE_MASK;
		ch_curr = j & POS_MASK;
		if(ch_type == WIRE_2)
		{
			ch_type = 2;
			ch_num = 24;
		}else{
			ch_type = 4;
			ch_num = 12;
		}
		break;
	}

}

#else //dmm and relay switch
//all dmm command > 0x80
//single command
#define CMD_DMM_RX		0x85	//switch rx,rs
#define CMD_DMM_RS		0x87	//switch rx,rs
#define CMD_DMM_ZERO		0x89	//set the current voltage to zero
#define CMD_DMM_VUPDATE		0x8b	//update the volt reading
#define CMD_DMM_TUPDATE		0x8d	//update the temp reading
#define CMD_DMM_TYPE_K		0x93		
#define CMD_DMM_TYPE_N		0x94		
#define CMD_DMM_TYPE_E		0x95		
#define CMD_DMM_TYPE_B		0x96		
#define CMD_DMM_TYPE_J		0x97		
#define CMD_DMM_TYPE_S		0x98		
#define CMD_DMM_TYPE_R		0x99		
//data read
#define CMDR_DMM_VOLT		0x84	//read dmm voltage
#define CMDR_DMM_TEMP		0x86	//read converted result
#define CMDR_DMM_STATE		0x88	//read state of dmm_action

uchar dmm_volt_last[5]; //5 bytes to store the last dmm reading
uchar dmm_temp_last[5]; //5 bytes to store the last mtemp reading
uchar dmm_type = 0; //K-S, thermocouple type
ulong dmm_zero = 0; //store the dmm zero value
uchar dmm_action = CMD_INVALID; //command buffer for main loop
void dmm_1v()
{
	_uart_str("%01;12;01");
	sjSerialSendByte(0x0d);
}
void dmm_120mv()
{
	_uart_str("%01;12;00");
	sjSerialSendByte(0x0d);
}
void dmm_init()
{
	 dmm_type = CMD_DMM_TYPE_K;
     dmm_1v();
}

void dmmwaithead()
{
	ulong tout = 0;
	while(1)
	{
		_uart_str("%01;01");
		sjSerialSendByte(0x0d);
		tout = 1 *1832;
		while(tout-- > 1)
		{
			if(spHead != spTail)
			{
				tout = 1 *1832;
				if(';' == sjSerialWaitForOneByte())
					return;
			}
		}
	}
}
uchar dmmwaitend()
{
	ulong tout = 0;
	tout = 3 *1832;
	while(tout-- > 1)
	{
		if(spHead != spTail)
		{
			tout = 3 *1832;
			if(0x0d == sjSerialWaitForOneByte())
				return 1;
		}
	}
	return 0;
}
double onereading()
{
	uchar sign = 1;
	uchar b;
	uchar decimal = 0;
	double val = 0;
	ulong tout;	
	while(1)
	{
		while(sjSerialIsDataWaiting())
		{
			b = sjSerialWaitForOneByte();
		}
		sign = 1;
		decimal = 0;
		val = 0;
		dmmwaithead();
		tout = 10*1836;
		while(tout > 1 )
		{
			if(sjSerialIsDataWaiting() != 1)
			{
				tout--;
				continue;
			}
			tout = 10*1836;
			b = sjSerialWaitForOneByte();
			if(b == '+')
				sign = 1;
			if(b == '-')
				sign = 0;
			if((b <= '9') && (b >= '0'))
			{
				val = val * 10.0;
				val = val + (double)(b - '0');
				if(decimal > 0) //decimal part
					decimal = decimal + 1;
			}
			if(b == '.')
				decimal = 1;
			if(b == ';')
				break;
		}
		if(tout <= 1) //timeout
			continue;
		if(0 == dmmwaitend()) //no line end found
			continue;

		if(sign == 0)
			val = 0 - val ;
		
		if(b == ';')
		{
			while(decimal > 3)
			{
				val = val / 10.0;
				decimal = decimal - 1;
			}
			return val;
		}
		
	}
}
void LoadFromEEPROM()
{
     dmm_zero = 0;
     dmm_zero = dmm_zero | byte_read(Sector[0][3]);
     dmm_zero = dmm_zero << 8;
     dmm_zero = dmm_zero | byte_read(Sector[0][2]);
     dmm_zero = dmm_zero << 8;
     dmm_zero = dmm_zero | byte_read(Sector[0][1]);
     dmm_zero = dmm_zero << 8;
     dmm_zero = dmm_zero | byte_read(Sector[0][0]);
}
void SaveToEEPROM()
{
     SectorErase(Sector[0][0]); 
     byte_write(Sector[0][0],dmm_zero&0xff); 
     byte_write(Sector[0][1],dmm_zero>>8&0xff);
     byte_write(Sector[0][2],dmm_zero>>16&0xff);
     byte_write(Sector[0][3],dmm_zero>>24&0xff);
}
#endif
void int_0() interrupt 0 using 3
{
	if(busstate == BUS_IDLE)
	{
		busbuf = (busbuf<<1);
		if(busIbit)
		   busbuf |= 0x0001;
		else
		   busbuf &= 0xfffe;

		if((busbuf & HEAD_MASK) == HEAD_MARK) //valid frame is got
		{
#ifdef MODE_SCANNER
			if((uchar)busbuf <= ch_num ||\			
			   (uchar)busbuf == CMD_SCAN_PKTT ||\
			   (uchar)busbuf == CMD_SCAN_NKTT ||\
			   (uchar)busbuf == CMD_SCAN_UPDATE)
			{
				ch_action = (uchar)busbuf;
				da = &ack_ok;
				obit_cnt = 0;
				obit_numbits = 1*8;
				busstate = BUS_SENDING;
			}else{
				if( (uchar)busbuf == CMDR_SCAN_STATE ||\
				    (uchar)busbuf == CMDR_SCAN_CHNUM ||\
  				    (uchar)busbuf == CMDR_SCAN_CHTYPE ||\
				    (uchar)busbuf == CMDR_SCAN_CHCURR)
				{
					if((uchar)busbuf == CMDR_SCAN_STATE)
						da = &ch_action;
					else if((uchar)busbuf == CMDR_SCAN_CHNUM)
						da = &ch_num;
					else if((uchar)busbuf == CMDR_SCAN_CHTYPE)
						da = &ch_type;
					else if((uchar)busbuf == CMDR_SCAN_CHCURR)
						da = &ch_curr;
				}
				obit_cnt = 0;
				obit_numbits = 1*8;
				busstate = BUS_SENDING;
			}
#else 
			
			if( (uchar)busbuf >= CMD_DMM_TYPE_K &&\
			    (uchar)busbuf <= CMD_DMM_TYPE_R)
			{
	
				dmm_action = (uchar)busbuf;
				da = &ack_ok;
				obit_cnt = 0;
				obit_numbits = 1*8;
				busstate = BUS_SENDING;
			}
			if( (uchar)busbuf == CMD_DMM_VUPDATE ||\
  			    (uchar)busbuf == CMD_DMM_TUPDATE ||\
  			    (uchar)busbuf == CMD_DMM_RX ||\
			    (uchar)busbuf == CMD_DMM_RS)
			{
				dmm_action = (uchar)busbuf;
				da = &ack_ok;
				obit_cnt = 0;
				obit_numbits = 1*8;
				busstate = BUS_SENDING;
			}else{
				if((uchar)busbuf == CMDR_DMM_STATE)
				{
					da = &dmm_action;
					obit_cnt = 0;
					obit_numbits = 1*8;
					busstate = BUS_SENDING;
				}
				if((uchar)busbuf == CMDR_DMM_VOLT)
				{
					da = &dmm_volt_last;
					obit_cnt = 0;
					obit_numbits = 5*8;
					busstate = BUS_SENDING;
				}	
				if((uchar)busbuf == CMDR_DMM_TEMP)
				{
					da = &dmm_temp_last;
					obit_numbits = 5*8;
					obit_cnt = 0;
					busstate = BUS_SENDING;
				}

			}
#endif
		}
		return;
	}
	if(busstate == BUS_SENDING)
	{
		if(obit_cnt < obit_numbits)
		{
			if( (*da) & (0x01 << (7 - (obit_cnt & 0x7)))){
				busObit = 1;
			}else{
				busObit = 0;
			}
			
			obit_cnt = obit_cnt+1;
			if((obit_cnt & 0x07) == 0)
				da++;
		}else{
			busObit = 1;
			obit_cnt = busObit; //set Obit to input
			obit_cnt = 0;
			obit_numbits = 0;
			busstate = BUS_IDLE;
		}
	}
}





void INIT_W1(void)
{
    //set serial port parameter (clock 11.0592M)
    //9600 baut rate 8 data non parity and 1 stop.
    SCON = 0x70;
    PCON = 0x00;
    TH1 = 0xf4; //F4: 2400 baudrate, FD: 9600 baudrate
    TMOD = 0x20;
    TR1 = 1;
    EX1 = 1;

    /***********************中断零初始化****************************/    
    EX0=1;              // INT0中断开
    IT0=1;              // INT0低电平触发(为1则为下降沿触发)
    EA=1; 
}



sbit jdq1= P1^7;   //继电器XI
sbit jdq2= P1^6;   //继电器X2

#define SET_JDQ		jdq1=1;jdq2=0;
#define CLR_JDQ		jdq1=0;jdq2=1;
#define copymem(dst,src) dst[3] =  src&0xff;\
			 dst[2] =  (src>>8)&0xff;\
			 dst[1] =  (src>>16)&0xff;\
			 dst[0] =  (src>>24)&0xff;\
			 dst[4] =  ~(dst[0]+dst[1]+dst[2]+dst[3])
void main()
{

	int i = 0;
	ulong count = 0;
	ulong ledout = 0;
	uchar j;
	double reading;


	j = busObit; //set Obit to input
	sjSerialInit();
	INIT_W1();
	IE = 0x91;

#ifdef MODE_SCANNER	
	query_scanner();
	if(ch_num > 1)
	{
		sjSerialSendByte(SCAN_ACTION | 1); //set to position 1
		ch_curr = 1;
	}

	while(1)
	{
		if(ch_action <= ch_num)
		{
			sjSerialSendByte(ch_action | SCAN_ACTION);
		}
		if(ch_action == CMD_SCAN_UPDATE)
		{
			query_scanner();
			count = 0;
		}
		if(ch_action == CMD_SCAN_PKTT)
		{
			SET_JDQ;
		}
		if(ch_action == CMD_SCAN_NKTT)
		{
			CLR_JDQ;
		}

		ch_action = CMD_INVALID;
/*
		if(count++ == 99999)
		{
			count = 0;
			query_scanner();
			ledout = ch_type * 10000 + ch_num * 100 + ch_curr;
			display164(ledout);
		}
*/
	}
#else
	dmm_init();
	//LoadFromEEPROM(); //load last dmm_zero
	dmm_volt_last[1] = 	dmm_volt_last[2] = 	dmm_volt_last[3] = 	dmm_volt_last[4] = 0x11;
	dmm_temp_last[1] = 	dmm_temp_last[2] = 	dmm_temp_last[3] = 	dmm_temp_last[4] = 0x22;
	while(1)
	{
/*
		if(testout != 0xff)
		{
			sjSerialSendByte(testout);
			testout = 0xff;
		}
*/
		if(dmm_action == CMD_INVALID)
			continue;
		if(dmm_action == CMD_DMM_RX)
		{
			SET_JDQ;
			dmm_action = CMD_INVALID;
		}
		if(dmm_action == CMD_DMM_RS)
		{
			CLR_JDQ;
			dmm_action = CMD_INVALID;
		}
		if(dmm_action == CMD_DMM_ZERO)
		{
			dmm_zero = onereading();
			SaveToEEPROM();
			dmm_action = CMD_INVALID;
		}
		if(dmm_action >= CMD_DMM_TYPE_K &&\
			   dmm_action <= CMD_DMM_TYPE_R)
		{
			dmm_type = dmm_action;
			dmm_action = CMD_INVALID;
		}
		if(dmm_action == CMD_DMM_VUPDATE)
		{
			reading = onereading();
			//display164(reading);
			copymem(dmm_volt_last,(*((ulong*)&reading)));
			dmm_action = CMD_INVALID;
		}
		if(dmm_action == CMD_DMM_TUPDATE)
		{
			reading = onereading();
			copymem(dmm_volt_last,(*((ulong*)&reading)));
			reading = MValueToTValue(reading,dmm_type);
			copymem(dmm_temp_last,(*((ulong*)&reading)));
			dmm_action = CMD_INVALID;
		}

	}
#endif	
}
