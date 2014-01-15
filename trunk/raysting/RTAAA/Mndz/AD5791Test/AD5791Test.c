/********************************************************************************
 Author : CAC (China Applications Support Team) 

 Date :   January, 2012

 File name :   AD5791Test.c

 Description :	 Use the GPIO to simulate the SPI communication of AD5791

 Hardware plateform : 	ADuC7026 and AD5791
********************************************************************************/

#include "ADuC7026.h"
#include "ADuC7026Driver.h"
#include "AD5791.h"
#include "stdio.h"

int main(void)
{
	// Add the test functions here

    ADuC7026Initialization();   

	printf("Hardware reset AD5791\r\n");
	AD5791HardwareReset();

	ReadFromAD5791ViaSPI(CONTROL, 1, AD5791Registers, CONTROL);
	printf("Read Control Register=0x%06X \r\n", AD5791Registers[CONTROL]);

	printf("Write 0x000302 and 0x040000 to Control Register and Clearcode Register.\r\n");
	AD5791Registers[CONTROL] = RBUF_OFF|OPGND_DIS|DACTRI_DIS|BIN_2SC_2SC|SDO_EN|LIN_COMP_20;
	AD5791Registers[CLEARCODE] = 0x00040000;	// 3/4 full scale
	WriteToAD5791ViaSPI(CONTROL, 2, AD5791Registers, CONTROL);

	printf("Write 0x0C0000 to DAC Register and hardware load.\r\n");
	AD5791Registers[DAC_VALUE] = 0x000C0000;	// 1/4 full scale
	WriteToAD5791ViaSPI(DAC_VALUE, 1, AD5791Registers, DAC_VALUE);
	AD5791HardwareLoad();

	AD5791Registers[DAC_VALUE] = 0;
	AD5791Registers[CONTROL] = 0;
	AD5791Registers[CLEARCODE] = 0;
	ReadFromAD5791ViaSPI(DAC_VALUE, 1, AD5791Registers, DAC_VALUE);
	ReadFromAD5791ViaSPI(CONTROL, 2, AD5791Registers, CONTROL);

	printf("Read DAC Register=0x%06X \r\n", AD5791Registers[DAC_VALUE]);
	printf("Read Control Register=0x%06X	\r\nRead Clearcode Register=0x%06X \r\n", AD5791Registers[CONTROL], AD5791Registers[CLEARCODE]);

	AD5791HardwareClear();

	while(1) {;}

}

