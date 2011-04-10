/**********************************************/
// Header file for CS5532
// CS5532 commands and settings
// CS5532 functions
/**********************************************/
#ifndef _CS5532_H_ 
#define _CS5532_H_ 

#include "define.h"
#define CS5532_READ_DATA_TIMEOUT 2000000

//CS5532 command

#define SYNC1 0xFF
#define SYNC0 0xFE


#define CMD_WRITE_CFG_REG 0x3
#define CMD_READ_CFG_REG 0xB

#define CMD_READ_ALL_OFFSET_CAL 0x49
#define CMD_WRITE_ALL_OFFSET_CAL 0x41

#define CMD_READ_ALL_GAIN_CAL 0x4A
#define CMD_WRITE_ALL_GAIN_CAL 0x42

#define CMD_READ_ALL_SETUP_REG 0x4D
#define CMD_WRITE_ALL_SETUP_REG 0x45

#define CMD_READ_OFFSET_REG1 0x9
#define CMD_WRITE_OFFSET_REG1 0x1
#define CMD_READ_OFFSET_REG2 0x19
#define CMD_WRITE_OFFSET_REG2 0x11

#define CMD_READ_GAIN_REG1 0xA
#define CMD_WRITE_GAIN_REG1 0x2
#define CMD_READ_GAIN_REG2 0x1A
#define CMD_WRITE_GAIN_REG2 0x12

#define CMD_READ_SETUP_REG1 0xD
#define CMD_WRITE_SETUP_REG1 0x5
#define CMD_READ_SETUP_REG2 0x1D
#define CMD_WRITE_SETUP_REG2 0x15
#define CMD_READ_SETUP_REG3 0x2D
#define CMD_WRITE_SETUP_REG3 0x25
#define CMD_READ_SETUP_REG4 0x3D
#define CMD_WRITE_SETUP_REG4 0x35



#define CMD_SINGLE_CONV_SETUP1 0x80
#define CMD_SINGLE_CONV_SETUP2 0x88
#define CMD_SINGLE_CONV_SETUP3 0x90
#define CMD_SINGLE_CONV_SETUP4 0x98
#define CMD_SINGLE_CONV_SETUP5 0xA0
#define CMD_SINGLE_CONV_SETUP6 0xA8
#define CMD_SINGLE_CONV_SETUP7 0xB0
#define CMD_SINGLE_CONV_SETUP8 0xB8


                                        
#define CMD_CONTINUE_CONV_SETUP1 0xC0
#define CMD_CONTINUE_CONV_SETUP2 0xC8
#define CMD_CONTINUE_CONV_SETUP3 0xD0
#define CMD_CONTINUE_CONV_SETUP4 0xD8
#define CMD_CONTINUE_CONV_SETUP5 0xE0
#define CMD_CONTINUE_CONV_SETUP6 0xE8
#define CMD_CONTINUE_CONV_SETUP7 0xF0
#define CMD_CONTINUE_CONV_SETUP8 0xF8


#define CMD_SELF_OFFSET_CAL_SETUP1 0x81
#define CMD_SELF_OFFSET_CAL_SETUP2 0x89
#define CMD_SELF_OFFSET_CAL_SETUP3 0x91
#define CMD_SELF_OFFSET_CAL_SETUP4 0x99
#define CMD_SELF_OFFSET_CAL_SETUP5 0xA1
#define CMD_SELF_OFFSET_CAL_SETUP6 0xA9
#define CMD_SELF_OFFSET_CAL_SETUP7 0xB1
#define CMD_SELF_OFFSET_CAL_SETUP8 0xB9

#define CMD_SELF_GAIN_CAL_SETUP1 0x82
#define CMD_SELF_GAIN_CAL_SETUP2 0x8A
#define CMD_SELF_GAIN_CAL_SETUP3 0x92
#define CMD_SELF_GAIN_CAL_SETUP4 0x9A
#define CMD_SELF_GAIN_CAL_SETUP5 0xA2
#define CMD_SELF_GAIN_CAL_SETUP6 0xAA
#define CMD_SELF_GAIN_CAL_SETUP7 0xB2
#define CMD_SELF_GAIN_CAL_SETUP8 0xBA


#define CMD_SYSTEM_OFFSET_CAL_SETUP1 0x85
#define CMD_SYSTEM_OFFSET_CAL_SETUP2 0x8D
#define CMD_SYSTEM_OFFSET_CAL_SETUP3 0x95
#define CMD_SYSTEM_OFFSET_CAL_SETUP4 0x9D
#define CMD_SYSTEM_OFFSET_CAL_SETUP5 0xA5
#define CMD_SYSTEM_OFFSET_CAL_SETUP6 0xAD
#define CMD_SYSTEM_OFFSET_CAL_SETUP7 0xB5
#define CMD_SYSTEM_OFFSET_CAL_SETUP8 0xBD


#define CMD_SYSTEM_GAIN_CAL_SETUP1 0x86
#define CMD_SYSTEM_GAIN_CAL_SETUP2 0x8E
#define CMD_SYSTEM_GAIN_CAL_SETUP3 0x96
#define CMD_SYSTEM_GAIN_CAL_SETUP4 0x9E
#define CMD_SYSTEM_GAIN_CAL_SETUP5 0xA6
#define CMD_SYSTEM_GAIN_CAL_SETUP6 0xAE
#define CMD_SYSTEM_GAIN_CAL_SETUP7 0xB6
#define CMD_SYSTEM_GAIN_CAL_SETUP8 0xBE

#define CMD_STOP_CONT_CONV 0xFF

#define TIMEOUTERR 0xff
#define SUCCESSFUL 0x0  
#define CAL_TIMEOUT_LIMIT 4000

//CS5532 Gain settings
#define GAINX1 0
#define GAINX2 1
#define GAINX4 2
#define GAINX8 3
#define GAINX16 4
#define GAINX32 5
#define GAINX64 6

//CS5532 Word rate settings
#define WORD_RATE_100SPS 0x0
#define WORD_RATE_50SPS 0x1
#define WORD_RATE_25SPS 0x2
#define WORD_RATE_12P5SPS 0x3
#define WORD_RATE_6P25SPS 0x4
#define WORD_RATE_3200SPS 0x8
#define WORD_RATE_1600SPS 0x9
#define WORD_RATE_800SPS 0xA
#define WORD_RATE_400SPS 0xB
#define WORD_RATE_200SPS 0xC


//CS5532 subroutines
u32 CS5532_Init(void);
u32 CS5532_Reset(void);
void CS5532_Config(u16, u16);
//extern u8 CS5532_Setup(void);  
//u8 CS5532_Setup_CH1(u8, u8);
void CS5532_Cont_Conversion(void);
//extern void CS5532_Single_Conversion(void);
//extern u8 CS5532_Set_Offset_Gain(u8 *, char);
u8 CS5532_ReadADC(u8 *);
//u8 CS5532_SysGainCal(void); 
void CS5532_Manual_Offset_Gain_Cal(u8 *, u8);
u8 CS5532_SysOffsetCal(void);
//extern u8 CS5532_SelfOffsetCal(void);
//extern void CS5532_ReadCal(unsigned int);
//void CS5532_Test(void);
void CS5532_Poise2Result(void);
void CS5532_PoiseWeight(void); 
u8 CS5532_Cont_Conv_Stop();

#endif
