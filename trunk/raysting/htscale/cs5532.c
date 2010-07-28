#include "define.h"
#include <intrins.h>

#define CS5532_READ_DATA_TIMEOUT 2000000

//根据实际情况定义
sbit SDI5532 = P2^2;
sbit SDO5532 = P1^5;
sbit CLK5532 = P1^4;
//sbit CS5532 = P2^0;

#define CS_SET	//CS5532=0
#define CS_CLR	//CS5532=1
#define CS5532_BUSY 0xFF  
#define SYS_GAIN_CAL_PREWORK_NOT_READY 1
#define CS5532_SETUP_ERR 0x1
#define CS5532_OFFSET_CAL_ERR 0x2
#define CS5532_GAIN_CAL_ERR 0x3

#define FLAG_OVER	0x04	//over range flag
//CS5532 command
#define SYNC1 0xFF
#define SYNC0 0xFE






void sleepms(u8 t)
{
	u16 i;
	while(t-- > 0)
	{
		i = 10000;
		while(i-- > 0)
			;
	}
}
u32 CS5532_Init(void);
u32 CS5532_Reset(void);
void CS5532_Config(u32);
void CS5532_Cont_Conversion(void);
u8 CS5532_ReadADC(u8*);

//The ADC results varibles define
union{
	struct{
          u8 top;
          u8 high;
          u8 mid;
          u8 low;
	}c;
	u32 l;
}RegDat;

//The data(8bit) form MCU to CS5532
static void _SendByte(u8 Dat)
{
	u8 i;
    CLK5532 = 0;
    for(i=8;i>0;i--)
    {
        SDI5532=(bit)(Dat & 0x80);
	    _nop_();_nop_();
        CLK5532=1;
        _nop_();_nop_();
        CLK5532=0;
        Dat = Dat<<1;
    }
    SDI5532 = 1;
}
//The data(8bit) form CS5532 to MCU
static u8 _ReceiveByte(void)
{
    u8 i;
	u8 dat;
    dat=0;
	CLK5532=0;
    for(i=8;i>0;i--)
    {
       dat=dat<<1;
       if(SDO5532)
	   		dat |= 0x01;
       CLK5532=1;
       _nop_();_nop_();
       CLK5532=0;
       _nop_();_nop_();
    }
    return(dat);
}

//The Setup CS5532's register
static void _WriteReg(u8 command,u32 reg)
{
    CS_SET;
	RegDat.l = reg;
    _SendByte(command);
    _SendByte(RegDat.c.top);
    _SendByte(RegDat.c.high);
    _SendByte(RegDat.c.mid);
    _SendByte(RegDat.c.low);
    CS_CLR;
}
//Receive CS5532's Register from CS5532 to MCU
static void _ReadReg(u8 command)
{
    CS_SET;
    _SendByte(command);
    RegDat.c.top = _ReceiveByte();
    RegDat.c.high = _ReceiveByte();
    RegDat.c.mid = _ReceiveByte();
    RegDat.c.low = _ReceiveByte();
    CS_CLR;
}

//Receive ADC signal data from CS5532 to MCU
//single convert
//Data conversion cycle
static void CS5532_ReadSADC(u8 command)
{
    CS_SET;
    _SendByte(command);
	CLK5532=0;
	SDI5532=0;
    do{
		_nop_();
	}while(SDO5532!=0);			  //delay until SDO drops
    _SendByte(0x00);              //generate 8 bit SCLK only with SDI = 0;
    RegDat.c.top = _ReceiveByte();
    RegDat.c.high = _ReceiveByte();
    RegDat.c.mid = _ReceiveByte();
    RegDat.c.low = _ReceiveByte();
    CS_CLR;
}
static void CS5532_ReadMADC(u8 command)
{
    CS_SET;
    do{
		_nop_();
	}while(SDO5532!=0);
    //SDO5532 = 1;
    _SendByte(command);            //8bit SCLK and SDI=command;
    RegDat.c.top = _ReceiveByte();
    RegDat.c.high = _ReceiveByte();
    RegDat.c.mid = _ReceiveByte();
    RegDat.c.low = _ReceiveByte();
    CS_CLR;
}



/******************************************************************/

/******************************************************************/
//                      CS5532 Reset      
// Set RS bit in configuration reg to "1" to trigger reset, then 
// change it back to "0" 
// Return 0x10 if reset is successful. Return 0x0 if reset
// failed.
/******************************************************************/ 
u32 CS5532_Reset(void)
{ 
  // Set RS in configuration register to "1" to reset CS5532. 
  // First to write configuration register command
  // write data to configuration register, 32bits (4 bytes)
  _WriteReg(CMD_WRITE_CFG_REG,0x20000000);     
  
  
  sleepms(20);
  
  //change RS in configuration register back to "0"
  // First to write configuration register command
  // write data to configuration register, 32bits (4 bytes)
  _WriteReg(CMD_WRITE_CFG_REG,0x00000000);

  
  //check if reset is successful(RV "1") or not.
  //read configuration register, send command first
  // read 4-bytes from configuration register. MSB first.
  _ReadReg(CMD_READ_CFG_REG);

  return RegDat.l;
}                        

/******************************************************************/
//                      CS5532 Initilization      
// At least 15 SYNC1 followed by SYNC0
// Call subroutine CS5532_Reset() to reset CS5532
/******************************************************************/
u32 CS5532_Init(void)
{
  u8 i;
  for (i=18; i>0; i--)
        _SendByte(SYNC1);
  _SendByte(SYNC0);
  // reset CS5532
  return CS5532_Reset();
}   

/******************************************************************/
//                      CS5532 Configuration      
// Configuration register is 32-bit long and only D31~D19 are used
// RS(D29): 0, normal operation; 1, reset cycle
// RV(D28): 0, normal operation; 1, reset valid
// IS(D27): 0, normal input; 1, input pairs are short internally
// OGS(D20): 0, cal Regs are used based on CS1-CS0; 1, OG1-OG0
// Filter Rate Select(D19): 0, 60Hz filter; 1, 50Hz filter
// Keep all other bits as default "0"
// Argument examples:
// cfgh: 0x20  --- Reset
// cfgl: 0x8   --- 50 HZ filter
// cfgl: 0x0   --- 60 HZ filter
// cfgh: 0x8,  --- Input short
// For CS5530, Bit D14~D11 are word rate config bits. These bits are
// reserved bits and should be set to "0" if AD is CS5532.
// D10 is the unipolar or bipolar input option bit, only for CS5530
// should be clear to zero if CS5532 is used.
/******************************************************************/
void CS5532_Config(u32 cfg)
{ 
  // First to write configuration register command
  _WriteReg(CMD_WRITE_CFG_REG, cfg);
}

    
// 		PSS 	PDW				RS             RV			IS          GB
// Power Save  Power down mode  Reset System Reset Valid  Input short  guard bit
//	    VRS           A0  A1	OLS					0	OGS					FRS        000
//     Voltage Ref   latch      output latch select		offset gain select  filter rate

#define AD_CONFIG	(u32)(0x80) << 16
u8 init_cs5532(u16 ch_set)
{            
   u8 i; 
   
   // To begin, a 4.9152 MHz or 4.096 MHz crystal takes approximately 20 ms 
   // to start. To accommodate for this, it is recommended that a software delay 
   // of approximately 20 ms be inserted before the start of the processor’s ADC 
   // initialization code. Actually this is not necessary in this system because
   // MEGA8 is configured to stay in reset state for 64ms (fuse bit) after power up.
   // sleepms(20);  
   //Initialize CS5532. Try 3 times max. 
   //LED_OFF(LED_D8);
   for(i=3;i>0;i--)
   {
      if(CS5532_Init() == RESET_VALID)
      {  //Bit[3:0] are word rate.         
         CS5532_Config(AD_CONFIG);       
         //CS5530 has no setup registers.                                
		 _WriteReg(B_GRP2|B_CH|B_WRITE|B_SETUP,((u32)ch_set)<<16|ch_set);
         //set to single conversion mode
         _SendByte(B_GRP2|B_SADC|B_SETUP1|B_NORMAL_CONV);
         return 0;
      }
   }
   return 0xff;
}

u32 read_cs5532(void)
{
 	CS5532_ReadSADC(B_GRP2|B_SADC|B_SETUP1);
	return RegDat.l;
}
