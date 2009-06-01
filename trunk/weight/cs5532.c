/******************************************************************/
// CS5532 driver           
#include "define.h"
#include "spi.h"
#include "cs5532.h"
#include "MEGA8.h" 
#include "uart.h"
#include "drveeprom.h" 
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
  SPI_MasterTransmit_c(CMD_WRITE_CFG_REG);     
  
  // write data to configuration register, 32bits (4 bytes)
  SPI_MasterTransmit_l(0x20000000); 
  
  sleepms(1);
  
  //change RS in configuration register back to "0"
  // First to write configuration register command
  SPI_MasterTransmit_c(CMD_WRITE_CFG_REG);
  // write data to configuration register, 32bits (4 bytes)
  SPI_MasterTransmit_l(0x00000000);  // Set RS to 0
  
  //check if reset is successful(RV "1") or not.
  //read configuration register, send command first
  SPI_MasterTransmit_c(CMD_READ_CFG_REG);
  // read 4-bytes from configuration register. MSB first.
  return SPI_MasterReceive_l();
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
        SPI_MasterTransmit_c(SYNC1);
  SPI_MasterTransmit_c(SYNC0);
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
void CS5532_Config(u16 cfg_hw, u16 cfg_lw)
{ 
  // First to write configuration register command
  SPI_MasterTransmit_c(CMD_WRITE_CFG_REG);
  SPI_MasterTransmit_w(cfg_hw); 
  SPI_MasterTransmit_w(cfg_lw);
}

/******************************************************************/
//              CS5532/CS5530 channel setup 
// CS5532 totally has 8 16-bit setup registers
// Decoder: 
// CS[1:0]/Bit[15:14], forced to 00 to select channel 1
// CG[2:0]/Bit[13:11], Gain x1(set to 0)/x2/x4/x8/x16/x32/x64(set to 6)
// WR[3:0]/Bit[10:7], 0000/100sps 0001/50sps 0010/25sps 0011/12.5sps
// 0100/6.25sps 1000/3200sps 1001/1600sps 1010/800sps 1011/400sps
// 1110/200sps
// other bits (Bit[6:0] set to 0x40, see datasheet for details.
// Function introduction:
// This function sets setup register 1 based on word rate and gain settings
// input by user. 
// valid word rate range is "0~4" and "8~12"
// valid gain setting is "0~6". See datasheet for details
// return 0x0 if setup is successful. return 0xff if setup failed. 
// Note: each CSR are 32 bits long and contains 2 setups. 2 setups
// need to be accessed in pairs, even only 1 setup is needed.
/******************************************************************/
/*
u8 CS5532_Setup_CH1(u8 word_rate, u8 gain)
{
   u8 cFlag;
   u8 setup_data[4],i;   

   if(word_rate>12)
        return(0xff);                   //invalid word rate settings
   if((word_rate>4) && (word_rate<8))                               
        return(0xff);                   //invalid word rate settings
   if (gain>6)                          // invalid gain settings
        return (0xff);
   
   setup_data[0] = (gain<<3) + (word_rate/2);
   setup_data[1] = 0x40 + ((word_rate%2)<<7);
   //setup registers needed to be be accessed x32 bits together
   setup_data[2] = setup_data[0];  
   setup_data[3] = setup_data[1];   
   
   // Initalize setup success flag. 0 represents successful setup.
   cFlag = 0x0;           
 
   //write settings into CS5530/CS5532 setup register 1
   SPI_MasterTransmit_c(CMD_WRITE_SETUP_REG1);
   for(i=0;i<4;i++)
        SPI_MasterTransmit_c(setup_data[i]);
 
   //read out all setup registers to check if setup is successful or not.
   SPI_MasterTransmit_c(CMD_READ_SETUP_REG1);
   for(i=0;i<4;i++)
   { //set error flag if data read out doesn't match previous settings
     if(SPI_MasterReceive_c() != setup_data[i])
         cFlag = 0xFF;
   } 
   return(cFlag);      
}
//*/
 
/******************************************************************/
//            CS5532 continous conversion
// Initialize CS5532 to continous conversion mode. 
// SDO of CS5532 falls to indicate completion of a conversion cycle.
// To make code work well on CS5530 which only have setup1, we force
// setup setting to SETUP1.
/******************************************************************/
void CS5532_Cont_Conversion(void)
{ 
  SPI_MasterTransmit_c(CMD_CONTINUE_CONV_SETUP1);  
}

/******************************************************************/
//            CS5532 single conversion
// Initialize CS5532 to single conversion mode. 
// SDO of CS5532 falls to indicate completion of a conversion cycle.
/******************************************************************
void CS5532_Single_Conversion(void)
{
  SPI_MasterTransmit_c(CMD_SINGLE_CONV_SETUP1); 
}        

/******************************************************************/
//            CS5532 conversion result readout
// ADC conversion result readout
// SDO of CS5532 falls to indicate completion of a conversion cycle
// read conversion result when is low.
// return SUCCESSFUL(0x0) when data is read. return TIMEOUTERR(0xff)
// if timeout error occurs. 
// Conversion result is returned via pointer parameter.
/******************************************************************/
u8 CS5532_ReadADC(u8 *ConvDataBuf)
{ 
  //long int timeout = CS5532_READ_DATA_TIMEOUT;
  unsigned char i;
  unsigned char *pConvDataBuf;
  pConvDataBuf = ConvDataBuf; 
   
  // return busy flag: data not available
  if(PORTB.P_MISO == 1)
      return(0xff);
  
  // first 8 clock to clear SDO flag;
   SPI_MasterTransmit_c(NULL);

  // read conversion result
  for(i=4;i>0;i--)
    *pConvDataBuf++ = SPI_MasterReceive_c();
    
  return(0x0);
}

/******************************************************************/
//            CS5532 Manual Offset Calibration
// mannually write a data to offset register to do offset cal. 
// This function calculates data to be written into offset reg based
// on input arg (CS5532 output) so that CS5532 output can be initialized
// to zero. 
// Offset data ~= CS5532_Output * (1/4 + 1/64 + 1/128)
// 1 LSB represents 1.83007 of (-24) order of 2. 
// 1/(1.8300*2) ~ = 1/4 + 1/64 + 1/128
/******************************************************************/
/*void CS5532_Manual_Offset_Gain_Cal(u8 *AD_Output, u8 gain)
{
  u8 *ptr;
  u32 offset, temp1, temp2, temp3;
  
  //convert array data(CS5532 output) into a u32 type data
  ptr = AD_Output;
  offset = 0;
  temp1 = *ptr++;
  offset = temp1 << 24;
  temp1 = *ptr++;
  offset += temp1 << 16; 
  temp1 = *ptr++;
  offset += temp1 << 8; 
  offset += *ptr;
  
  //calculate data to be written into offset register
  temp1 = offset >> 2; 
  temp2 = offset >> 6;
  temp3 = offset >> 7;       
  offset = temp1 + temp2 + temp3;
  
  //the lsb byte should be zero when write to offset register
  offset = offset & 0xffffff00;

  //write offset data to offset register
  SPI_MasterTransmit_c(CMD_WRITE_OFFSET_REG1);
  SPI_MasterTransmit_l(offset); 
  
  //write gain settings to gain register
  SPI_MasterTransmit_c(CMD_WRITE_GAIN_REG1);
  SPI_MasterTransmit_c(1<<gain); 
  SPI_MasterTransmit_c(0); 
  SPI_MasterTransmit_c(0); 
  SPI_MasterTransmit_c(0); 
  
}    //*/

/******************************************************************/
// stop CS5532 continous conversion mode so that it can receive
// new command.
/******************************************************************/
/*u8 CS5532_Cont_Conv_Stop()
{ 
   if(PORTB.P_MISO == 1)
      return(0xff);
   //stop continous converson mode
   SPI_MasterTransmit_c(CMD_STOP_CONT_CONV);
   //clock out the 32 bits before exit
   SPI_MasterReceive_l();      
   return(0x0);   
} //*/

/******************************************************************/
//            CS5532 system offset calibration
// Offset calibration on CS5532.
// return TIMEOUTERR (0xff) if timeout error occurs.
// return SUCCESSFUL (0x0) if operation is successful.
/******************************************************************/ 
/*u8 CS5532_SysOffsetCal(void)
{
  u16 timeout;

#ifdef _CS5532_DISPLAY_OFFSET_
  u8 data[4];
#endif  
  
 // perform offset calibration
 SPI_MasterTransmit_c(CMD_SYSTEM_OFFSET_CAL_SETUP1);
 // wait till calibration completes or timeout
 timeout = CAL_TIMEOUT_LIMIT;
 while(PORTB.P_MISO == 1)
 {
    if (timeout-- == 0)
      return(TIMEOUTERR);
 }
 
#ifdef _CS5532_DISPLAY_OFFSET_
  //read back offset register 
  SPI_MasterTransmit_c(CMD_READ_OFFSET_REG1);
  data[0] = SPI_MasterReceive_c(); 
  data[1] = SPI_MasterReceive_c(); 
  data[2] = SPI_MasterReceive_c(); 
  data[3] = SPI_MasterReceive_c();   
  
  //display through RS485 port
  sleepms(UART_DELAY);
  putchar(data[0]);   
  sleepms(UART_DELAY);
  putchar(data[1]);   
  sleepms(UART_DELAY);
  putchar(data[2]);   
  sleepms(UART_DELAY);
  putchar(data[3]);
  sleepms(UART_DELAY);  
#endif   
 
  return(SUCCESSFUL);   
} //*/

/******************************************************************/
//            CS5532 system Gain calibration
// Gain calibration on CS5532 
// return TIMEOUTERR (0xff) if timeout error occurs.
// return SUCCESSFUL (0x0) if operation is successful.
/******************************************************************/
/*u8 CS5532_SysGainCal(void)
{
 u8 timeout;

#ifdef _CS5532_DISPLAY_GAIN_
 u8 data[4];
#endif   

 // gain calibration
 SPI_MasterTransmit_c(CMD_SYSTEM_GAIN_CAL_SETUP1);
 // wait till calibration completes or timeout
 timeout = CAL_TIMEOUT_LIMIT;
 while(PORTB.P_MISO == 1)
  { 
    if (timeout-- == 0)
      return(TIMEOUTERR);
  } 
 
#ifdef _CS5532_DISPLAY_GAIN_
  //read back offset register 
  SPI_MasterTransmit_c(CMD_READ_GAIN_REG1);
  data[0] = SPI_MasterReceive_c(); 
  data[1] = SPI_MasterReceive_c(); 
  data[2] = SPI_MasterReceive_c(); 
  data[3] = SPI_MasterReceive_c(); 
  
  sleepms(UART_DELAY);
  putchar(data[0]);   
  sleepms(UART_DELAY);
  putchar(data[1]);   
  sleepms(UART_DELAY);
  putchar(data[2]);   
  sleepms(UART_DELAY);
  putchar(data[3]);
  sleepms(UART_DELAY);  
#endif 
  
 return(SUCCESSFUL);  
}  //*/

/******************************************************************/
//            Read CS5532 Offset/Gain calibration result
// Read CS5532 offset/gain calibration result and generate checksum.
// Result is read back into a buffer pointed by a pinter parametric.
// Buffer size is at least 11 bytes long. Result will be stored into
// EEPROM later.
/******************************************************************
 //void CS5532_ReadCal(unsigned char *CalBuf) 
 void CS5532_ReadCal(unsigned int CalBuf)
 {
   unsigned char i;
   unsigned char checksum;
   unsigned char *pCalBuf;
   pCalBuf = CalBuf;
   checksum = 0;
   
   //read gain calibration result and save data into buffer
   SPI_MasterTransmit(CMD_READ_GAIN_REG1);
   *pCalBuf++ = 'G';
   for(i=4;i>0;i--)
     *pCalBuf++=SPI_MasterTransmit(NULL);  

   //read offset calibration result and save data into buffer
   SPI_MasterTransmit(CMD_READ_OFFSET_REG1);
   *pCalBuf++= 'O';
   for(i=4;i>0;i--)
      *pCalBuf++=SPI_MasterTransmit(NULL);
  
   //generate checksum byte
   pCalBuf=pCalBuf-10;
   for(i=10;i>0;i--)
       checksum=checksum+*pCalBuf++;
   // checksum byte
   *pCalBuf = ~checksum + 1;      
  } //*/

/******************************************************************/
//             Set CS5532 OFFSET/GAIN Registers 
// This function allows users to set offset cal value manually
// Arg 1: offset_data,  points to a 4-byte data array
// Arg 2: 'O' to set OFFSET register, 'G' to set Gain register.
// return 0x0 if set offset register successfully
// return 0xff if fails to set offset register.
/******************************************************************/
/*u8 CS5532_Set_Offset_Gain(u8 *setting, char reg)
{
  u8 *ptr;
  u8 i, cFlag;

  //Write offset data to OFFSET register
  ptr = setting;
  if ((reg != 'O') && (reg != 'G'))
      return(0xff);  
  if(reg =='O')  
    SPI_MasterTransmit_c(CMD_WRITE_OFFSET_REG1);
  else 
    SPI_MasterTransmit_c(CMD_WRITE_GAIN_REG1); 
  for(i=4;i>0;i--)
     SPI_MasterTransmit_c(*ptr++);
      
  // Verify whether setting is successfully done.
  cFlag = 0x0;
  ptr = setting;
  if(reg =='O')
    SPI_MasterTransmit_c(CMD_READ_OFFSET_REG1);
  else
    SPI_MasterTransmit_c(CMD_READ_GAIN_REG1);
  for(i=4;i>0;i--)
  { if(SPI_MasterReceive_c() != *ptr++)
       cFlag = 0xFF;
  } 
  return(cFlag);           
} //*/

         
/******************************************************************/
//  Get CS5532 output and perform average filter  
//  To simplify calculation, only value the 16 bits of CS5532 output
//  re-edited by Xianghua - Apr 10 2008
/******************************************************************/  
#define DATA_NUM 2
#define DIVIDED_BY_DATA_NUM 1                   
//fill data buffer and return average.
u16 DataBuf[DATA_NUM];
u8 Buf_Is_Empty = 0;

/********************************************************************/
// fill data buffer and get average 
// return AD_BUSY if A/D conversion is not completed yet.
// return AD_OVER_FLOW if A/D conversion overflow happens.
// otherwise return average of multiple AD samples.
/********************************************************************/
u16 FillDatBuf()
{
   u32 sum;
   u16 average;
   u8 ConvTempbuf[4], k; 
   k = 0;
   
    //get multiple (DATA_NUM) samples.
   while(k < DATA_NUM) {      
      if(CS5532_ReadADC(ConvTempbuf) != SUCCESSFUL)                      
          return(AD_BUSY);
      //LSB byte of the 32 bits is monitor byte, 
      if (ConvTempbuf[3] != 0) { 
          RS485._global.cs_status |= 0x8; 
          return(AD_OVER_FLOW);
      }
      else {  
         RS485._global.cs_status &= 0xF7; 
         sum = ConvTempbuf[0];            
         DataBuf[k++] = (sum << 8) + ConvTempbuf[1];
      }
   }
   
   // average of all samples.          
   sum = 0;
   for(k=0; k< DATA_NUM; k++)
      sum += DataBuf[k];
   // long int to int type
   average = (sum >> DIVIDED_BY_DATA_NUM) & 0xFFFF;              
   return(average);
} 

/****************************************/     
// software filter on material weight   
// 

/****************************************/
#define SKIP_N_SAMPLES 3
#define AVERAGE_NUM 8  //average buffer size

void CS5532_PoiseWeight()
{       
   static u16 AverageBuf[AVERAGE_NUM]; 
   static u8 wait_to_be_stablized;     
   u16 up,dn;
   u8 flag,j,range;    
   range = RS485._flash.cs_Filter_option & 0xF;
                                                  
   //check whether delay (skip samples) is needed before A/D output is stablized.
   if(wait_to_be_stablized > 0){   
        RS485._global.cs_mtrl = FILTER_ONGOING; // invalid data 
        wait_to_be_stablized--; // delay counter decreases
        return;
   }         
   //sample more data at the first time.
   if(Buf_Is_Empty == 0){ 
      for(j=0; j< AVERAGE_NUM; j++){
         AverageBuf[j] = FillDatBuf();
         if((AverageBuf[j] == AD_BUSY) || (AverageBuf[j] == AD_OVER_FLOW)) {
           RS485._global.cs_mtrl = AverageBuf[j]; // invalid data 
           return;
         }
      }
      Buf_Is_Empty = 1;  
   }     
   //shift in the buffer to only save latest AVERAGE_NUM average data.
   for(j=0;j<AVERAGE_NUM-1;j++)
      AverageBuf[j] = AverageBuf[j+1];
   AverageBuf[AVERAGE_NUM-1] = FillDatBuf();
   if((AverageBuf[AVERAGE_NUM-1] == AD_BUSY) || (AverageBuf[AVERAGE_NUM-1] == AD_OVER_FLOW)) {
      RS485._global.cs_mtrl = AverageBuf[AVERAGE_NUM-1]; // invalidate "cs_mtrl"
      return;
   }
      
   // start to filter
   flag = PASS;  
   up = AverageBuf[AVERAGE_NUM-1] + (AverageBuf[AVERAGE_NUM-1] >> 6);
   dn = AverageBuf[AVERAGE_NUM-1] - (AverageBuf[AVERAGE_NUM-1] >> 6);         

   //Filter option #1: Perform "average+Forcast" filter
   if ((RS485._flash.cs_Filter_option & 0xF0) == 0x10){
      for(j=0; j<AVERAGE_NUM-1; j++)     
         if((AverageBuf[j] > up) || (AverageBuf[j] < dn)){   
            flag = FAIL;
            // weight is not stable, skip next n samples.
            wait_to_be_stablized = ((RS485._flash.cs_Filter_option & 0xF) << SKIP_N_SAMPLES) + 1;
            break; 
         }          
   }
   // add other options:
     
   if(flag == PASS)                 
      RS485._global.cs_mtrl = AverageBuf[AVERAGE_NUM-1]; 
   else
      RS485._global.cs_mtrl = FILTER_ONGOING; // invalidate "cs_mtrl"
                    
}                                                                   

/*****************************************************************************/
//              Convert CS5532 output to gram  
// Formula to transfer CS5532 measurement to actual weight£º
//   Mtrl_Weight_gram      (cs_mtrl  - cs_zero)        temp1
// ------------------  = --------------------------- = ------
//   Poise_Weight_gram     (cs_poise - cs_zero)        temp2
// note: to minimize non-lineary of A/D, 5 poises can be used for calibriation
// The one which is most close to cs_Mtrl will be found and used. 
// If Bit 7 of test_mode_reg1 is cleared, multi poises are used.
// "old_cs_zero" is used to monitor change of caliration data.
// if "old_cs_zero" is not equal to "cs_zero", "cs_poise" will be adjusted
// accordingly. 
// There could be such a confliction as below:
// "cs_zero" is changed by user, then node board software detect the change
// and start to adjust "cs_poise" automatically while at the same time
// user starts to change "cs_poise" (calibration) at the same time. Confliction!!!
// However because MCU speed to update cs_poise is much faster than user's 
// operations on PC, this case will not happen.
/***************************************************************************/
void CS5532_Poise2Result()
{  
   u8 min_delta_index;     // index of poise with min delta from material weight.
   u16 temp2;
   u32 temp1,temp3, temp4;
   u16 delta, min_delta;
   u8 i;             
#ifdef _FORCE_CONSTANT_WEIGHT_ 
   RS485._global.Mtrl_Weight_gram = 0xfff1;
   return;   
#endif

   // check if error happened in CS5532_PoiseWeight()
   if(RS485._global.cs_mtrl > MAX_VALID_DATA)
   { // result is invalid. 
     // pass fail code to variable Weight_gram, which will be read by master board.
     RS485._global.Mtrl_Weight_gram =  RS485._global.cs_mtrl;                
     return;      
   }       
  
   if(RS485._global.cs_mtrl < RS485._flash.cs_zero)
      temp1 = 0;
   else
      temp1 = RS485._global.cs_mtrl - RS485._flash.cs_zero;                       

   /***************************************************************/
   //adjust cs_poise based on comparison between cs_zero and old_cs_zero
   if(RS485._flash.cs_zero > RS485._global.old_cs_zero)
   {
      delta = RS485._flash.cs_zero - RS485._global.old_cs_zero; 
      for(i=0;i<5;i++)
         RS485._flash.cs_poise[i] += delta;
   }
   if(RS485._flash.cs_zero < RS485._global.old_cs_zero)
   {
      delta = RS485._global.old_cs_zero - RS485._flash.cs_zero; 
      for(i=0;i<5;i++)
         RS485._flash.cs_poise[i] -= delta;   
   }   
   RS485._global.old_cs_zero = RS485._flash.cs_zero;   
   
   /***************************************************************/
   // Search the poise which is most close to cs_mtrl
   if(!ENABLE_MULTI_POISES) 
      min_delta_index = 0;
   else
   {  min_delta = 0xffff;
      for(i=0;i<5;i++)
      { 
         if(RS485._global.cs_mtrl > RS485._flash.cs_poise[i])
            delta =  RS485._global.cs_mtrl -  RS485._flash.cs_poise[i];
         else
            delta =  RS485._flash.cs_poise[i] - RS485._global.cs_mtrl;      
         if (delta < min_delta)
         {   min_delta = delta ; min_delta_index = i; }      
      }
   }          

   /***************************************************************/        
   if(RS485._flash.cs_poise[min_delta_index] < RS485._flash.cs_zero)
      temp2 = 0;
   else
      temp2 = RS485._flash.cs_poise[min_delta_index] - RS485._flash.cs_zero;  
        
   // compiler default: u16*u16 = u16       
   // so we use u16 * u32 = u32 here
   temp3 = RS485._flash.Poise_Weight_gram[min_delta_index] * temp1;                                                         
   temp3 = temp3 << 6;   // X64. so unity is 1/64g      	
        
   if(temp2 != 0)
   {
      temp4 =  temp3/temp2;
      RS485._global.Mtrl_Weight_gram = (temp4 >> 6);
      if(RS485._global.Mtrl_Weight_gram==0xffff)
          RS485._global.Mtrl_Weight_gram= OVERWEIGHT;
      RS485._global.Mtrl_Weight_decimal = temp4 & 0x3F;
   }   
   else
      RS485._global.Mtrl_Weight_gram =  DIV_ERROR; // indicate bad calibration                     
   // master board won't find this node if Mtrl_Weight_gram is always 0xffff.
   // Bad calibartion data will make Mtrl_Weight_gram always bad.
   // so we set Mtrl_Weight_gram to a different data to indicate calibration error.
   /***************************************************************/                      
   // if bit2 of test_mode_reg1 is set, AD output is sent to master board / PC.
   if(DISPLAY_AD_RAW_DATA)
        RS485._global.Mtrl_Weight_gram =  RS485._global.cs_mtrl;
   return;
}
