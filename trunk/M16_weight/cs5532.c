/******************************************************************/
// CS5532 driver           
#include "define.h"
#include "spi.h"
#include "cs5532.h"
#include <MEGA16.h> 
#include "uart.h"
#include "drveeprom.h" 
/******************************************************************/

#ifdef _DEBUG_WEIGHT_ 
bit flag_to_print_ad_readings;
#endif

/* Calibration point */
flash u16 cal_gram[] = {20,50,100,200,300,400,500,700,900,1000};
#if 0
#define BASE_INDEX 2
#define CAL_BASE 200
#define CAL_BASE_X_64 (CAL_BASE*64)
#endif

bit flushDataBuffer;
void flush_data_buffer()
{
   flushDataBuffer = TRUE;
}
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
  if(PINB.P_MISO == 1)
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
void CS5532_Manual_Offset_Gain_Cal(u8 *AD_Output, u8 gain)
{
  u8 *ptr;
  u32 offset, temp1;   // temp2, temp3;
  
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

  //write offset data to offset register
  SPI_MasterTransmit_c(CMD_WRITE_OFFSET_REG1);
  SPI_MasterTransmit_l(offset); 
  
  //write gain settings to gain register
  SPI_MasterTransmit_c(CMD_WRITE_GAIN_REG1);
  SPI_MasterTransmit_c(1<<gain); 
  SPI_MasterTransmit_c(0); 
  SPI_MasterTransmit_c(0); 
  SPI_MasterTransmit_c(0); 
  
}

/******************************************************************/
// stop CS5532 continous conversion mode so that it can receive
// new command.
/******************************************************************/
u8 CS5532_Cont_Conv_Stop()
{ 
   if(PINB.P_MISO == 1)
      return(0xff);
   //stop continous converson mode
   SPI_MasterTransmit_c(CMD_STOP_CONT_CONV);
   //clock out the 32 bits before exit
   SPI_MasterReceive_l();      
   return(0x0);   
}

#ifdef _DEBUG_WEIGHT_ 
void enable_data_printing()
{
   flag_to_print_ad_readings = FALSE;
}

void disable_data_printing()
{
   flag_to_print_ad_readings = TRUE;
}
#endif

/********************************************************************************/ 
// fill data buffer and get average 
// return AD_BUSY if A/D conversion is not completed yet.
// return AD_OVER_FLOW if A/D conversion overflow happens.
// otherwise return average of multiple AD samples.
/********************************************************************************/ 
u16 read_ad_data()
{
   u16 raw_data, temp;
   u8 ConvTempbuf[4]; 

   if(CS5532_ReadADC(ConvTempbuf) != SUCCESSFUL)                      
      return(AD_BUSY);
   // LSB byte of the 32 bits is monitor byte, 
   if (ConvTempbuf[3] != 0) { 
       RS485._global.diag_status1 |= 0x20;         // set bit 5 error flag
       return(AD_OVER_FLOW);                       
   }
   else{  
       RS485._global.diag_status1 &= 0xDF;         // clear error flag
        
       temp = ConvTempbuf[0];                      // don't merge these 2 lines.                   
       raw_data = temp << 8; 
       
       raw_data += ConvTempbuf[1]; 

#ifdef _DEBUG_WEIGHT_        
       if(flag_to_print_ad_readings)
       {
          putchar(ConvTempbuf[0]);
          putchar(ConvTempbuf[1]);
       }
#endif                       

   }   
   return(raw_data);
    
} 

/********************************************************************************/     
// software filter on material weight  
/********************************************************************************/    
#define MAX_BUF_SIZE 8 //average buffer size, must be times of 8.  xianghua 64
#define SAMPLE_SIZE 4  
#define MAX_TOLERANCE 32  
#define MIN_TOLERANCE 4
#define FILTER_PHASE1 0
#define FILTER_PHASE2 1

u8 CS5532_PoiseWeight()
{       
   static u16 Raw_DataBuf[MAX_BUF_SIZE];       // data buffer contains raw data from A/D 
   static u8 cur_data_index, data_counts;      // variables associated to Raw_DataBuf
   static u8 filter_phase;
   u16 SortBuf[MAX_BUF_SIZE];
   
   u16 latest_n_average, temp; 
   u32 sum;
   u8 i, j, tolerance, setting;  

   /*************************************************************/   
   // Test if we need to flush raw data buffer
   // send from 
   /*************************************************************/
  if(flushDataBuffer)
   {
      cur_data_index = 0;
      data_counts = 0;
      filter_phase = 0;
      flushDataBuffer = FALSE;
   }
            
   /*************************************************************/
   // read a AD conversion, return error if result is invalid
   /*************************************************************/    
   temp = read_ad_data();
   if(temp == AD_BUSY)               // Conversion not completed yet 
      return ERR_AD_BUSY;
   else if(temp == AD_OVER_FLOW)                   
   {                                           
      RS485._global.cs_mtrl = temp;  // Invalidate "cs_mtrl" and return      
      return ERR_AD_OVER_FLOW;
   }
           
   /*************************************************************/
   // Queue data. Report FILTER_ONGOING if queue is not full yet. 
   /*************************************************************/  
   Raw_DataBuf[cur_data_index++] = temp;              
   if(cur_data_index >= MAX_BUF_SIZE)  
       cur_data_index = 0; 
        
   if(data_counts < SAMPLE_SIZE) /* insufficent data samples */            
   {   
       data_counts++;        
       RS485._global.cs_mtrl = FILTER_ONGOING;
       return ERR_FILTER_ONGOING;                                          
   }    
   /*************************************************************/
   // Calculate deviation to test if readings are stable
   /*************************************************************/      
   switch(filter_phase)
   {
      /*************************************************************/   
      // PHASE1: Pass when 4 consequent samples are within tolerance
      /*************************************************************/
      case FILTER_PHASE1:
          /*************************************************************/   
          // Get user tolerance setting for filering
          /*************************************************************/
          if(RS485._flash.cs_gain_wordrate < MIN_TOLERANCE)
              setting = MIN_TOLERANCE; 
          else if(RS485._flash.cs_gain_wordrate > MAX_TOLERANCE)
              setting = MAX_TOLERANCE; 
          else
              setting = RS485._flash.cs_gain_wordrate;    
          tolerance = setting * SAMPLE_SIZE; 
          /*************************************************************/   
          // Calculate deviatioin of weight samples
          /*************************************************************/   
          // get average first
          for(i=0, sum = 0; i<SAMPLE_SIZE; i++)
          {  
             if(cur_data_index>i)                      
                 j = cur_data_index-1- i;
             else
                 j = MAX_BUF_SIZE - 1 - i + cur_data_index;
             sum+= Raw_DataBuf[j];   
          }
          latest_n_average = (sum/SAMPLE_SIZE) & 0xFFFF;
          // calculate deviation
          for(i=0,sum = 0; i<SAMPLE_SIZE; i++)
          {  
              if(cur_data_index>i)                      
                  j = cur_data_index-1- i;
              else
                  j = MAX_BUF_SIZE - 1 - i + cur_data_index;      
      
              if(latest_n_average > Raw_DataBuf[j])
                  sum += latest_n_average - Raw_DataBuf[j];
              else
                  sum += Raw_DataBuf[j] - latest_n_average;
          }          
          /*************************************************************/     
          // if weight is not stable, return to get new sample
          /*************************************************************/   
          if(sum > tolerance)
          {
              RS485._global.cs_mtrl = FILTER_ONGOING;  // data is not stable
              return ERR_FILTER_ONGOING;      
          }   
          else
          {   
              filter_phase = FILTER_PHASE2; 
              RS485._global.cs_mtrl = latest_n_average; 
              return DATA_VALID;
          }   
          break;
      /*************************************************************/   
      // PHASE2: Pass when 4 consequent samples are within tolerance
      /*************************************************************/
      case FILTER_PHASE2:
          if(data_counts < MAX_BUF_SIZE)
          {  data_counts++;        
             //RS485._global.cs_mtrl = temp; //use last weight
             break;
          }          
          /*************************************************************/
          // Medium Value Filtering
          // First copy data to sort buffer and sort data (Min in Buf[0])
          /*************************************************************/
          for(j=0; j<MAX_BUF_SIZE; j++)
              SortBuf[j] = Raw_DataBuf[j];             
          for(i=0; i<(MAX_BUF_SIZE>>1); i++)          // sort data buffer. min at buf[0].
          {  for(j=0; j < MAX_BUF_SIZE-1-i; j++)
             {  
                if(SortBuf[j] > SortBuf[j+1])         // swap data, max to higher address
                {  temp = SortBuf[j+1];
                   SortBuf[j+1] = SortBuf[j];
                   SortBuf[j] = temp;            
                }                                       
              }      
           }       
           /*********************************************************/
           // Medium Filtering, excludes high end (1/8 of queue size)
           // and low end data (1/8 of queue size)
           // Calculate average over medium parts (3/4 of queue size).
           /*********************************************************/   
           sum = 0;
           i = MAX_BUF_SIZE >> 3; 
           for(j=i;j<MAX_BUF_SIZE-i; j++)
               sum+= SortBuf[j];
           i = MAX_BUF_SIZE- (MAX_BUF_SIZE >>2);
           RS485._global.cs_mtrl = (sum/i) & 0xFFFF; 
           break;
      default:
          flushDataBuffer = TRUE;          
          break;
   }
   return DATA_VALID;
}                                                                   

/***************************************************************************/
// Validate user calibration data, which is used by default. 
//   cs_zero < cs_poise[0] < cs_poise[1] < ... < cs_poise[4]
//   Poise_Weight_gram[0]< ... < Poise_Weight_gram[4] 
/***************************************************************************/
#define MIN_CAL_DELTA 200   //  20g/0.1g
u8 valid_user_calibration()
{
   u8 i, status;                 
   
   status = PASS;

   for(i=0; i<(sizeof(cal_gram)/sizeof(u16))-1; i++)
   { 
      if((RS485._flash.cs_poise[i] <= RS485._flash.cs_zero) || 
         (RS485._flash.cs_poise[i]+ MIN_CAL_DELTA > RS485._flash.cs_poise[i+1]))
      {   
         status = FAIL;
         break; 
      }
   }
   return status;

}

/***************************************************************************/
// Copy factory calibration data to user calibration field.
/***************************************************************************/
u8 load_mfg_calibration()
{
   u8 i, status;
   u16 *src,*des, checksum;
   
   status = PASS;
   src = &(RS485._rom.mfg_cs[0]);
   // mfg_cs[10], mfg_cs_zero; mfg_checksum
   for (checksum=0,i=0; i<NUM_OF_CAL_POINT+2; i++)
      checksum += *src++ ;
   // If factory calibration data is valid, copy it 
   // user calibration data field.
   if(!checksum)
   {
      src = &(RS485._rom.mfg_cs[0]);
      des = &(RS485._flash.cs_poise[0]); 
      #asm("cli");
      // copy mfg_cs[10], mfg_cs_zero;
      for(i=0; i<NUM_OF_CAL_POINT+1; i++)
         *des++ = *src++;
      #asm("sei");
   }
   else  
      status = FAIL;
   
   return status;
}

/***************************************************************************/
// Save factory calibration data
/***************************************************************************/
u8 save_mfg_calibration()
{
   u8 i, status;
   u16 *src,*des, checksum;
   
   status = valid_user_calibration(); 
  
   if(status == PASS)
   {    
      src = &(RS485._flash.cs_poise[0]);
      des = &(RS485._rom.mfg_cs[0]);
      for (checksum=0,i=0; i< NUM_OF_CAL_POINT+1; i++)
      {
          checksum += *src;
          *des++ = *src++;       
      }
      RS485._rom.mfg_checksum = (~checksum)+1;      
   }
   return status;
}

/***************************************************************************/
// Save factory calibration data
/***************************************************************************/
u32 computeWeight(u16 cs_mtrl)
{
   u8 i, num_of_cal, min_delta_index;
   u16 temp2, temp4;
   u32 temp1,temp3, weight;
   u16 delta, min_delta;
   /*************************************************************/
   // Search the poise array to find best calibration range  
   /*************************************************************/ 
   num_of_cal = sizeof(cal_gram)/sizeof(u16);
   for(min_delta=0xffff,i=0; i<num_of_cal; i++)
   { 
      if(cs_mtrl > RS485._flash.cs_poise[i])
          delta =  cs_mtrl -  RS485._flash.cs_poise[i];
      else
          delta =  RS485._flash.cs_poise[i] - cs_mtrl;     
      if (delta < min_delta)                         
      { min_delta = delta; min_delta_index = i;}
   } 

   /*************************************************************/
   // if cal_gram[i] < 200(cal_gram[2])     
   // temp1     (cs_matrl -(2*cs_zero - cs[2]))*64    (gram +200)*64      temp3
   // ------ = ------------------------------------ = ----------------  = --------
   // temp2     cs[i]   -  (2*cs_zero - cs[2])        cal_gram[i]+200     temp4
   /*************************************************************/    
#if 0
   if(min_delta_index < BASE_INDEX)
   {
     temp1 = (u32)cs_mtrl + RS485._flash.cs_poise[BASE_INDEX] - (u32)RS485._flash.cs_zero*2;
     temp1 = temp1<<6;
     
     temp2 = RS485._flash.cs_poise[min_delta_index] - RS485._flash.cs_zero;
     temp4 = RS485._flash.cs_poise[BASE_INDEX] - RS485._flash.cs_zero;
     temp2 += temp4;
     
     temp4 = cal_gram[min_delta_index] + CAL_BASE;
     
     temp3 = temp1*temp4;
     weight = temp3/temp2;
     weight -= CAL_BASE_X_64; 
   }
   /*************************************************************/
   // if cal_gram[i] >= 200     
   // temp1 =  (cs_matrl - cs_zero)*64    (gram - 0)*64      temp3
   // ------  ------------------------ = ---------------- = --------
   // temp2    cs[i]   - cs_zero          cal_gram[i] - 0     temp4
   /*************************************************************/   
   else
#endif
   {
       if(cs_mtrl < RS485._flash.cs_zero)
           temp1 = RS485._flash.cs_zero - cs_mtrl;
       else
           temp1 = cs_mtrl - RS485._flash.cs_zero;    //positive
       temp1 = temp1<<6;
       temp2 = RS485._flash.cs_poise[min_delta_index] - RS485._flash.cs_zero;
       temp4 = cal_gram[min_delta_index];
       temp3 = temp1*temp4;
       weight = temp3/temp2; 
       
       /* set weight to negative */
       if(cs_mtrl<RS485._flash.cs_zero)
          weight = ~weight + 1;      
   }   
   return weight;
}

void CS5532_Poise2Result()
{  
   u32 weight_with_decimal, zero_offset;

   /*************************************************************/
   // return if weight is invalid
   /*************************************************************/      
   if(RS485._global.cs_mtrl > MAX_VALID_DATA)
   {  
      RS485._global.Mtrl_Weight_gram =  RS485._global.cs_mtrl;                
      return;      
   }
   if(valid_user_calibration() == FAIL)
   {   
      RS485._global.Mtrl_Weight_gram =  BAD_CALIBRATION;
      RS485._global.diag_status1 |= 0x40;
      return;
   }                  
   
   // calculate integral
   weight_with_decimal = computeWeight(RS485._global.cs_mtrl);
   
   //compute weight offset   
   if(RS485._rom.new_cs_zero < MAX_VALID_DATA)
   {   
       zero_offset = computeWeight(RS485._rom.new_cs_zero);
       //compute final weight
       weight_with_decimal -= zero_offset;
       //if result is negative, set weight to zero.  
       if(weight_with_decimal>>31)
          weight_with_decimal = 0;
   }
 
   // Get integral part of weight 
   RS485._global.Mtrl_Weight_gram = (u16)(weight_with_decimal>>6);
   // Get decimal part of weight
   RS485._global.Mtrl_Weight_decimal = (u16)weight_with_decimal & 0x3F; 
   RS485._global.diag_status1 &= 0xBF;    
   
   return;     
}
