/** \file
*	@brief Common Const and Command Routine Defines.
*/
#ifndef _SJ_DEFINE_H_
#define _SJ_DEFINE_H_
//#include "reg52.h"

/////////////////////////////////////////////////

//???????,???????"REG51.H"

//?????????        // ???   ??
sfr ACC         =   0xE0;   //0000,0000 ???Accumulator
sfr B           =   0xF0;   //0000,0000 B???
sfr PSW         =   0xD0;   //0000,0000 ?????
sbit CY         =   PSW^7;
sbit AC         =   PSW^6;
sbit F0         =   PSW^5;
sbit RS1        =   PSW^4;
sbit RS0        =   PSW^3;
sbit OV         =   PSW^2;
sbit P          =   PSW^0;
sfr SP          =   0x81;   //0000,0111 ????
sfr DPL         =   0x82;   //0000,0000 ???????
sfr DPH         =   0x83;   //0000,0000 ???????

//I/O ????????
sfr P0          =   0x80;   //1111,1111 ??0
sbit P00        =   P0^0;
sbit P01        =   P0^1;
sbit P02        =   P0^2;
sbit P03        =   P0^3;
sbit P04        =   P0^4;
sbit P05        =   P0^5;
sbit P06        =   P0^6;
sbit P07        =   P0^7;
sfr P1          =   0x90;   //1111,1111 ??1
sbit P10        =   P1^0;
sbit P11        =   P1^1;
sbit P12        =   P1^2;
sbit P13        =   P1^3;
sbit P14        =   P1^4;
sbit P15        =   P1^5;
sbit P16        =   P1^6;
sbit P17        =   P1^7;
sfr P2          =   0xA0;   //1111,1111 ??2
sbit P20        =   P2^0;
sbit P21        =   P2^1;
sbit P22        =   P2^2;
sbit P23        =   P2^3;
sbit P24        =   P2^4;
sbit P25        =   P2^5;
sbit P26        =   P2^6;
sbit P27        =   P2^7;
sfr P3          =   0xB0;   //1111,1111 ??3
sbit P30        =   P3^0;
sbit P31        =   P3^1;
sbit P32        =   P3^2;
sbit P33        =   P3^3;
sbit P34        =   P3^4;
sbit P35        =   P3^5;
sbit P36        =   P3^6;
sbit P37        =   P3^7;
sfr P4          =   0xC0;   //1111,1111 ??4
sbit P40        =   P4^0;
sbit P41        =   P4^1;
sbit P42        =   P4^2;
sbit P43        =   P4^3;
sbit P44        =   P4^4;
sbit P45        =   P4^5;
sbit P46        =   P4^6;
sbit P47        =   P4^7;
sfr P5          =   0xC8;   //xxxx,1111 ??5
sbit P50        =   P5^0;
sbit P51        =   P5^1;
sbit P52        =   P5^2;
sbit P53        =   P5^3;
sbit P54        =   P5^4;
sbit P55        =   P5^5;
sbit P56        =   P5^6;
sbit P57        =   P5^7;
sfr P6          =   0xE8;   //0000,0000 ??6
sbit P60        =   P6^0;
sbit P61        =   P6^1;
sbit P62        =   P6^2;
sbit P63        =   P6^3;
sbit P64        =   P6^4;
sbit P65        =   P6^5;
sbit P66        =   P6^6;
sbit P67        =   P6^7;
sfr P7          =   0xF8;   //0000,0000 ??7
sbit P70        =   P7^0;
sbit P71        =   P7^1;
sbit P72        =   P7^2;
sbit P73        =   P7^3;
sbit P74        =   P7^4;
sbit P75        =   P7^5;
sbit P76        =   P7^6;
sbit P77        =   P7^7;
sfr P0M0        =   0x94;   //0000,0000 ??0?????0
sfr P0M1        =   0x93;   //0000,0000 ??0?????1
sfr P1M0        =   0x92;   //0000,0000 ??1?????0
sfr P1M1        =   0x91;   //0000,0000 ??1?????1
sfr P2M0        =   0x96;   //0000,0000 ??2?????0
sfr P2M1        =   0x95;   //0000,0000 ??2?????1
sfr P3M0        =   0xB2;   //0000,0000 ??3?????0
sfr P3M1        =   0xB1;   //0000,0000 ??3?????1
sfr P4M0        =   0xB4;   //0000,0000 ??4?????0
sfr P4M1        =   0xB3;   //0000,0000 ??4?????1
sfr P5M0        =   0xCA;   //0000,0000 ??5?????0
sfr P5M1        =   0xC9;   //0000,0000 ??5?????1
sfr P6M0        =   0xCC;   //0000,0000 ??6?????0
sfr P6M1        =   0xCB;   //0000,0000 ??6?????1
sfr P7M0        =   0xE2;   //0000,0000 ??7?????0
sfr P7M1        =   0xE1;   //0000,0000 ??7?????1

//???????????
sfr PCON        =   0x87;   //0001,0000 ???????
sfr AUXR        =   0x8E;   //0000,0000 ?????
sfr AUXR1       =   0xA2;   //0000,0000 ?????1
sfr P_SW1       =   0xA2;   //0000,0000 ?????????1
sfr CLK_DIV     =   0x97;   //0000,0000 ?????????
sfr BUS_SPEED   =   0xA1;   //xx10,x011 ?????????
sfr P1ASF       =   0x9D;   //0000,0000 ??1?????????
sfr P_SW2       =   0xBA;   //xxxx,x000 ?????????

//?????????
sfr IE          =   0xA8;   //0000,0000 ???????
sbit EA         =   IE^7;
sbit ELVD       =   IE^6;
sbit EADC       =   IE^5;
sbit ES         =   IE^4;
sbit ET1        =   IE^3;
sbit EX1        =   IE^2;
sbit ET0        =   IE^1;
sbit EX0        =   IE^0;
sfr IP          =   0xB8;   //0000,0000 ????????
sbit PPCA       =   IP^7;
sbit PLVD       =   IP^6;
sbit PADC       =   IP^5;
sbit PS         =   IP^4;
sbit PT1        =   IP^3;
sbit PX1        =   IP^2;
sbit PT0        =   IP^1;
sbit PX0        =   IP^0;
sfr IE2         =   0xAF;   //0000,0000 ???????2
sfr IP2         =   0xB5;   //xxxx,xx00 ????????2
sfr INT_CLKO    =   0x8F;   //0000,0000 ??????????????

//??????????
sfr TCON        =   0x88;   //0000,0000 T0/T1?????
sbit TF1        =   TCON^7;
sbit TR1        =   TCON^6;
sbit TF0        =   TCON^5;
sbit TR0        =   TCON^4;
sbit IE1        =   TCON^3;
sbit IT1        =   TCON^2;
sbit IE0        =   TCON^1;
sbit IT0        =   TCON^0;
sfr TMOD        =   0x89;   //0000,0000 T0/T1?????
sfr TL0         =   0x8A;   //0000,0000 T0???
sfr TL1         =   0x8B;   //0000,0000 T1???
sfr TH0         =   0x8C;   //0000,0000 T0???
sfr TH1         =   0x8D;   //0000,0000 T1???
sfr T4T3M       =   0xD1;   //0000,0000 T3/T4?????
sfr T3T4M       =   0xD1;   //0000,0000 T3/T4?????
sfr T4H         =   0xD2;   //0000,0000 T4???
sfr T4L         =   0xD3;   //0000,0000 T4???
sfr T3H         =   0xD4;   //0000,0000 T3???
sfr T3L         =   0xD5;   //0000,0000 T3???
sfr T2H         =   0xD6;   //0000,0000 T2???
sfr T2L         =   0xD7;   //0000,0000 T2???
sfr WKTCL       =   0xAA;   //0000,0000 ??????????
sfr WKTCH       =   0xAB;   //0000,0000 ??????????
sfr WDT_CONTR   =   0xC1;   //0000,0000 ????????

//??????????
sfr SCON        =   0x98;   //0000,0000 ??1?????
sbit SM0        =   SCON^7;
sbit SM1        =   SCON^6;
sbit SM2        =   SCON^5;
sbit REN        =   SCON^4;
sbit TB8        =   SCON^3;
sbit RB8        =   SCON^2;
sbit TI         =   SCON^1;
sbit RI         =   SCON^0;
sfr SBUF        =   0x99;   //xxxx,xxxx ??1?????
sfr S2CON       =   0x9A;   //0000,0000 ??2?????
sfr S2BUF       =   0x9B;   //xxxx,xxxx ??2?????
sfr S3CON       =   0xAC;   //0000,0000 ??3?????
sfr S3BUF       =   0xAD;   //xxxx,xxxx ??3?????
sfr S4CON       =   0x84;   //0000,0000 ??4?????
sfr S4BUF       =   0x85;   //xxxx,xxxx ??4?????
sfr SADDR       =   0xA9;   //0000,0000 ???????
sfr SADEN       =   0xB9;   //0000,0000 ?????????

//ADC ???????
sfr ADC_CONTR   =   0xBC;   //0000,0000 A/D???????
sfr ADC_RES     =   0xBD;   //0000,0000 A/D?????8?
sfr ADC_RESL    =   0xBE;   //0000,0000 A/D?????2?

//SPI ???????
sfr SPSTAT      =   0xCD;   //00xx,xxxx SPI?????
sfr SPCTL       =   0xCE;   //0000,0100 SPI?????
sfr SPDAT       =   0xCF;   //0000,0000 SPI?????

//IAP/ISP ???????
sfr IAP_DATA    =   0xC2;   //0000,0000 EEPROM?????
sfr IAP_ADDRH   =   0xC3;   //0000,0000 EEPROM?????
sfr IAP_ADDRL   =   0xC4;   //0000,0000 EEPROM?????
sfr IAP_CMD     =   0xC5;   //xxxx,xx00 EEPROM?????
sfr IAP_TRIG    =   0xC6;   //0000,0000 EEPRPM???????
sfr IAP_CONTR   =   0xC7;   //0000,x000 EEPROM?????

//PCA/PWM ???????
sfr CCON        =   0xD8;   //00xx,xx00 PCA?????
sbit CF         =   CCON^7;
sbit CR         =   CCON^6;
sbit CCF2       =   CCON^2;
sbit CCF1       =   CCON^1;
sbit CCF0       =   CCON^0;
sfr CMOD        =   0xD9;   //0xxx,x000 PCA ???????
sfr CL          =   0xE9;   //0000,0000 PCA??????
sfr CH          =   0xF9;   //0000,0000 PCA??????
sfr CCAPM0      =   0xDA;   //0000,0000 PCA??0?PWM???
sfr CCAPM1      =   0xDB;   //0000,0000 PCA??1?PWM???
sfr CCAPM2      =   0xDC;   //0000,0000 PCA??2?PWM ???
sfr CCAP0L      =   0xEA;   //0000,0000 PCA??0???/????????
sfr CCAP1L      =   0xEB;   //0000,0000 PCA??1???/????????
sfr CCAP2L      =   0xEC;   //0000,0000 PCA??2???/????????
sfr PCA_PWM0    =   0xF2;   //xxxx,xx00 PCA??0?PWM???
sfr PCA_PWM1    =   0xF3;   //xxxx,xx00 PCA??1?PWM???
sfr PCA_PWM2    =   0xF4;   //xxxx,xx00 PCA??1?PWM???
sfr CCAP0H      =   0xFA;   //0000,0000 PCA??0???/????????
sfr CCAP1H      =   0xFB;   //0000,0000 PCA??1???/????????
sfr CCAP2H      =   0xFC;   //0000,0000 PCA??2???/????????

/////////////////////////////////////////////////






#define CHAR_CMDBEGIN	':'
#define CHAR_CMDEND		';'
#define CHAR_LINEEND	0x0A

typedef unsigned int WORD;

#ifndef BYTE
#define BYTE unsigned char
#endif
#ifndef BOOL
#define BOOL BYTE
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#define GET_BYTE(pbuf)	(pbuf[0]-'0')*10+(pbuf[1]-'0')///<combine 2char to BYTE 

//BYTE length;		///<indicat the buffer length (no use?)

//sbit tbit_1 = P3^4;
//sbit tbit_2 = P3^5;
//sbit tbit_3 = P3^6;
//DA


/*
PIN ON DA BOARD
1	SDO  = P2^0
2	RESET = P2^1
3	DGND
4	CLR = P2^2
5	IOVCC
6	LDAC = P2^3
7	SDIN = P2^4
8	DGND
9	SCLK = P2^5
10	SYNC = P2^6
*/
sbit SDO = P2^0;
sbit RESET = P2^1;
sbit CLR = P2^2;
sbit LDAC = P2^3;
sbit SDI = P2^4;
sbit SCLK  = P2^5;
sbit SYNC  = P2^6;

//serial port
#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2RB8 0x04              //S2CON.2
#define S2TB8 0x08              //S2CON.3

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7
#define S2_S0 0x01              //P_SW2.0

#define TRT3M 	T4T3M


#define FOSC  24000000L		//24000000L	//33177600L	//22118400L		//11059200L	//18432000L          
#define BAUD1 9600             

#define BAUD2 57600

BOOL sjSerialIsDataWaiting();
BYTE sjSerialWaitForOneByte();
void sjSerialSendByte(BYTE);

BOOL sjSerialIsDataWaiting2();
BYTE sjSerialWaitForOneByte2();
void sjSerialSendByte2(BYTE);

void io_putc(unsigned char);
BYTE io_getc();
BYTE io_hasc();
void init_uart( void );

//-----------------------------------------
//define baudrate const
//BAUD = 256 - FOSC/3/BAUDRATE/M (1T:M=1; 12T:M=12)
//#define BAUD	0xF400                  // 1200bps @ 11.0592MHz
//#define BAUD	0xFA00                  // 2400bps @ 11.0592MHz
//#define BAUD	0xFD00                  // 4800bps @ 11.0592MHz
//#define BAUD	0xFE80                  // 9600bps @ 11.0592MHz
//#define BAUD	0xFF40                  //19200bps @ 11.0592MHz
//#define BAUD	0xFFA0					//38400bps @ 11.0592MHz

//#define BAUD	0xEC00                  // 1200bps @ 18.432MHz
//#define BAUD	0xF600                  // 2400bps @ 18.432MHz
//#define BAUD	0xFB00                  // 4800bps @ 18.432MHz
//#define BAUD	0xFD80                  // 9600bps @ 18.432MHz
//#define BAUD	0xFEC0                  //19200bps @ 18.432MHz
//#define BAUD	0xFF60                  //38400bps @ 18.432MHz

//#define BAUD	0xE800                  // 1200bps @ 22.1184MHz
//#define BAUD	0xF400                  // 2400bps @ 22.1184MHz
//#define BAUD	0xFA00                  // 4800bps @ 22.1184MHz
//#define BAUD	0xFD00                  // 9600bps @ 22.1184MHz
//#define BAUD	0xFE80                  //19200bps @ 22.1184MHz
//#define BAUD	0xFF40                  //38400bps @ 22.1184MHz
//#define BAUD	0xFF80                  //57600bps @ 22.1184MHz

#define BAUD_RATE	9600

void io_putc2(unsigned char);
BYTE io_getc2();
BYTE io_hasc2();
void init_uart2( void );

void 	asp_set_timer();
void	asp_handler();
void 	asp_handler2();

//HW SPI function
//BYTE SpiShift(BYTE);
//#define	Spi374OutByte(A)  SpiShift(A)/* SPI输出8个位数据 */
//#define Spi374InByte()	SpiShift(0) /* SPI输入8个位数据 */

//SW_SPI
void Spi374OutByte(BYTE);
BYTE Spi374InByte(BYTE);



//USB function
#define SERIAL_VERSION_1_0

#define SP_BUFFER_MAX	32 ///<the fifo buffer size for all serial port

#define USB_OUT_BUF_MAX	32
#define	SWI_DELAY	10///<wait(SleepMs(1)*SWI_DELAY after switch data sent 

BOOL Query374Interrupt();
void USB_DeviceInterrupt(); 
void CH374_PORT_INIT();
void Init374Device();
BYTE USBGetChar();
BOOL USBHasChar();
//relay function
void cm_relay(unsigned char);
void sjRelayInit();
void swiAction(unsigned char);
//the MARK of header ender and error and quest and reset
#define  HMARK 0x55	//U
#define  EMARK 0x56 //V
#define  QMARK 0x57 //W
#define  RMARK 0x58 //X


#define NAV_INIT        1
#define NAV_30V         2
#define NAV_1V          3
#define NAV_120MV       4
#define NAV_AFLTOFF     5
#define NAV_AFLTON      6
#define NAV_SLOWMODE    7
#define NAV_ZEROON      8
#define NAV_READ        9

unsigned char strlen(unsigned char *s);
void sleepms(unsigned int i);
void nav_command(unsigned char);
double nav_read();

#endif


