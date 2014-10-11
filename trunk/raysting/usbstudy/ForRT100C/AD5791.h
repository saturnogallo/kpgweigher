/********************************************************************************
 Author : CAC (China Applications Support Team) 

 Date :   January, 2012

 File name :   AD5791.h

 Description :	 Use the GPIO to simulate the SPI communication of AD5791

 Hardware plateform : 	ADuC7026 and AD5791
********************************************************************************/

#ifndef AD5791_H
#define AD5791_H
#include "sjDefine.h"

extern xdata unsigned long int AD5791Registers[5];

//Register Map and bit definition
//DAC Register	 
#define	DAC_VALUE			0x01

//Control Register
#define	CONTROL				0x02
	#define	RBUF_OFF			0x02   
	#define	RBUF_ON				0x00
	#define	OPGND_DIS			0x00
	#define	OPGND_EN			0x04
	#define DACTRI_DIS			0x00
	#define DACTRI_EN			0x08
	#define	BIN_2SC_2SC			0x00
	#define	BIN_2SC_BIN			0x10
	#define	SDO_EN				0x00
	#define	SDO_DIS				0x20
	#define	LIN_COMP_10			0x00
	#define	LIN_COMP_12			0x240
	#define	LIN_COMP_16			0x280
	#define	LIN_COMP_19			0x2C0
	#define	LIN_COMP_20			0x300	


//Clearcode Register
#define	CLEARCODE			0x03

//Software Control Register
#define	SOFTWARE_CONTROL	0x04
	#define LDAC_EN				0x01
	#define LDAC_DIS			0x00
	#define	CLR_EN				0x02
	#define	CLR_DIS				0x00
	#define	RESET_EN			0x04
	#define	RESET_DIS			0x00	

//No Operation			
#define NOP					0x00
 
void ADuC7026Init();
//void AD5791Initiate(void);
void WriteToAD5791ViaSPI(const unsigned char RegisterStartAddress, unsigned long int *DataBuffer);
void ReadFromAD5791ViaSPI(const unsigned char RegisterStartAddress, unsigned char *DataBuffer);
void AD5791Initialization(void);
void AD5791HardwareClear(void);
void AD5791HardwareReset(void);
void AD5791HardwareLoad(void);
#endif

