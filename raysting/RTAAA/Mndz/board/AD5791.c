/********************************************************************************
 Author : CAC (China Applications Support Team) 

 Date :   January, 2012

 File name :   AD5791.c

 Description :	 Use the GPIO to simulate the SPI communication of AD5791

 Hardware plateform : 	ADuC7026 and AD5791
********************************************************************************/

//#include "ADuC7026.h"
//#include "ADuC7026Driver.h"
#include "AD5791.h"
#include "stdio.h"
extern void swiDelay(unsigned char dl1,unsigned char dl2);

xdata unsigned long int AD5791Registers[5]={0,0,0,0,0};		// only low 24 bits are used for each register

unsigned char ch_dummy;

#define ADuC7026OutputBit(A,B)	A=B;

#define ADuC7026DelayUs(i)	swiDelay(1,i);

#define ADuC7026InputBit(A)  (A) 



unsigned char ADuC7026SpiRead(unsigned char WriteData)
{
	 unsigned   char   ReadData;
     unsigned   char  j;

  

      ReadData = 0;

      for(j=0; j<8; j++)
      {
         ADuC7026OutputBit(SCLK,1);

         if(0x80 == (WriteData & 0x80))
         {
            ADuC7026OutputBit(SDI,1);     //Send one to SDI pin
         }
         else
         {
            ADuC7026OutputBit(SDI,0);     //Send zero to SDI pin
         }
         WriteData = WriteData << 1;
         ReadData = (ReadData<<1) | ADuC7026InputBit(SDO); 

         ADuC7026OutputBit(SCLK,0);   /////

      }
      return ReadData;
}
void ADuC7026SpiWrite(unsigned char WriteData)
{
     unsigned   char   j;


      for(j=0; j<8; j++)
      {
         ADuC7026OutputBit(SCLK,1);

         if(0x80 == (WriteData & 0x80))
         {
            ADuC7026OutputBit(SDI,1);     //Send one to SDI pin
         }
         else
         {
            ADuC7026OutputBit(SDI,0);     //Send zero to SDI pin
         }
         WriteData = WriteData << 1;

         ADuC7026OutputBit(SCLK,0);   /////
      }
}

void ADuC7026Init()
{
   /* initialize the GPIOs for controlling the AD5791*/
   ADuC7026OutputBit(LDAC, 1);
   ADuC7026OutputBit(SYNC, 1);
   ADuC7026OutputBit(CLR, 1);
   ADuC7026OutputBit(SCLK, 1);
   ADuC7026OutputBit(SDI, 1);
   ch_dummy = ADuC7026InputBit(SDO);
}

void AD5791HardwareReset()
{
	ADuC7026OutputBit(RESET,0);
	ADuC7026OutputBit(RESET,1);
	ADuC7026DelayUs(10);
}

void AD5791HardwareClear()
{
	ADuC7026OutputBit(CLR,0);
	ADuC7026OutputBit(CLR,1);
}

void AD5791HardwareLoad()
{
	ADuC7026OutputBit(LDAC,0);
	ADuC7026OutputBit(LDAC,1);
}

void AD5791Initialization()
{
	AD5791HardwareReset();
	AD5791HardwareClear();
}

void WriteToAD5791ViaSPI(unsigned char RegisterStartAddress, unsigned long int *DataBuffer)
{
	unsigned char  WriteData;
	

   ADuC7026OutputBit(SYNC,0);
   ADuC7026OutputBit(SCLK,1); 

		WriteData = ((RegisterStartAddress)<<4) | (DataBuffer[RegisterStartAddress]>>16);
		ADuC7026SpiWrite(WriteData);
		WriteData = DataBuffer[RegisterStartAddress]>>8;
		ADuC7026SpiWrite(WriteData);
		WriteData = DataBuffer[RegisterStartAddress];
		ADuC7026SpiWrite(WriteData);

   ADuC7026OutputBit(SCLK,1);
   ADuC7026OutputBit(SYNC,1);
}

void ReadFromAD5791ViaSPI(const unsigned char RegisterStartAddress,unsigned char *DataBuffer)
{
	unsigned char  WriteData;

   	ADuC7026OutputBit(SYNC,0);
	ADuC7026OutputBit(SCLK,1); 

	WriteData = 0x80 | ((RegisterStartAddress)<<4) ;	
	ADuC7026SpiWrite(WriteData);
	ADuC7026SpiWrite(NOP);
	ADuC7026SpiWrite(NOP);
	ADuC7026OutputBit(SCLK,1);
   	ADuC7026OutputBit(SYNC,1);

	ADuC7026OutputBit(SCLK,0);
   	ADuC7026OutputBit(SYNC,0);

	DataBuffer[0] = 0x00;
	DataBuffer[1] = 0x0f & ADuC7026SpiRead(NOP);
	DataBuffer[2] = ADuC7026SpiRead(NOP);
	DataBuffer[3] = ADuC7026SpiRead(NOP);

   	ADuC7026OutputBit(SYNC,1);
	ADuC7026OutputBit(SCLK,1);

}

