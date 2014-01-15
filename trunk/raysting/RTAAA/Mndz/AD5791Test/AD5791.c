/********************************************************************************
 Author : CAC (China Applications Support Team) 

 Date :   January, 2012

 File name :   AD5791.c

 Description :	 Use the GPIO to simulate the SPI communication of AD5791

 Hardware plateform : 	ADuC7026 and AD5791
********************************************************************************/

#include "ADuC7026.h"
#include "ADuC7026Driver.h"
#include "AD5791.h"
#include "stdio.h"

unsigned long int AD5791Registers[5]={0,0,0,0,0};		// only low 24 bits are used for each register


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

unsigned char WriteToAD5791ViaSPI(const unsigned char RegisterStartAddress, const unsigned char NumberOfRegistersToWrite, unsigned long int *DataBuffer, const unsigned char OffsetInBuffer)
{
	unsigned char WriteBuf[3];
	unsigned char ReadBuf[3];
	
	unsigned char i;

	for(i=0; i<NumberOfRegistersToWrite; i++)
	{
		WriteBuf[0] = ((RegisterStartAddress + i)<<4) | (DataBuffer[RegisterStartAddress + i]>>16);
		WriteBuf[1] = DataBuffer[RegisterStartAddress + i]>>8;
		WriteBuf[2]	= DataBuffer[RegisterStartAddress + i];
		ADuC7026SpiOperation(WriteBuf, ReadBuf, 3);
	}

	return 0;
}

unsigned char ReadFromAD5791ViaSPI(const unsigned char RegisterStartAddress, const unsigned char NumberOfRegistersToRead, unsigned long int *DataBuffer, const unsigned char OffsetInBuffer)
{
	unsigned char WriteBuf[3];
	unsigned char ReadBuf[3];
	
	unsigned char i;

	for(i=0; i< (NumberOfRegistersToRead+1); i++)
	{
		if (i < NumberOfRegistersToRead)
		{
			WriteBuf[0] = 0x80 | ((RegisterStartAddress+i)<<4) ;	
		}
		else
		{		
			WriteBuf[0] = NOP;	
		}
		WriteBuf[1] = NOP;
		WriteBuf[2]	= NOP;

		ADuC7026SpiOperation(WriteBuf, ReadBuf, 3);

		if (i>0)
		{
			DataBuffer[OffsetInBuffer + i -1] = ReadBuf[0];
			DataBuffer[OffsetInBuffer + i -1] = (DataBuffer[OffsetInBuffer + i -1]<<8) + ReadBuf[1];
			DataBuffer[OffsetInBuffer + i -1] = (DataBuffer[OffsetInBuffer + i -1]<<8) + ReadBuf[2];
		}
	}

	return 0;
}

