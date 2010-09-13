/*********************************************************************************
This file is MEGA16 SPI driver

Chip type           : ATmega16
Program type        : Application
Clock frequency     : 7.372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256

Code by             : fd_lxh@yahoo.com.cn
*********************************************************************************/

#include <mega16.h>
#include "define.h"   

/********************************************************************************/
//   PCB Board configuration:
/********************************************************************************/
sfrb DDR_SPI=0x17;                   /* SPI pins shared with PORT B[7:4] */
                                     /* 0x17 is the addr of DDRB */

/********************************************************************************/
//            MEAG16 SPI Master Initialization
// This subroutine initialize MEGA8 SPI as master.
// SPI interrupt disabled. MSB sent first.
/********************************************************************************/


void SPI_MasterInit(void)
{

/* Set MOSI SCK and SS to outputm Set MISO to input mode */ 
DDR_SPI.P_MOSI = 1;
DDR_SPI.P_SCK  = 1;
DDR_SPI.P_SS   = 1;
DDR_SPI.P_MISO = 0;

//SS# high to disable slave
//PORTB = PORTB | (1<<P_SS);  
SPI_SS = 1;

//clear SPIF flag by reading SPSR first and followed by SPDR later.
#asm
    in   r30,spsr
    in   r30,spdr
#endasm

//Set SPI to master mode, clock rate to fck/8
//SPCR: SPI Control Register
//Bit 7 - SPIE : "1" to enable SPI interrupt.
//Bit 6 - SPE  : "1" to enable.
//Bit 5 - DORD : "1" stands for LSB first to send
//Bit 4 - MSTR : "1" to set SPI as master
//Bit 3 - CPOL : When set to "1", SCK keep high in idle state
//Bit 2 - CPHA : when set to "1", setting data at the rising edge of clock, while sample data at clock falling edge.
//Bit 1,0 - SPR1,SPR0 :  SRR1, SPR0 and SPI2X together define clock freq of SPI master.
//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
SPSR = 0x1;
SPCR = 0b01010001;    
}

/********************************************************************************/
//             MEAG8 SPI Write/Read subroutine
// Function: char SPI_MasterTransmit(char cData)
// when used as writing fuction, cData is the data to be write to 
// SPI slave. When used as read function, set cData equal to 0x0, 
// this subroutine returns data sent from SPI slave.
/********************************************************************************/
void SPI_MasterTransmit_c(u8 cData)
{                            
        u8 dat;
        /* Set SS# pin to low */
        SPI_SS = 0;   
        /* start data transition*/
        SPDR = cData;
        /* waiting for data transition end */
        while(!(SPSR & (1<<SPIF)));
        /* return data and clear SPIF */
        dat = (SPDR); //just discard it
}

/* send a word data MSB first*/
void SPI_MasterTransmit_w(u16 cData)
{
	SPI_MasterTransmit_c((cData>>8) & 0xFF);
	SPI_MasterTransmit_c(cData & 0xFF);	
} 

/* send a long data MSB first*/
void SPI_MasterTransmit_l(u32 cData)
{
	SPI_MasterTransmit_c((cData>>24) & 0xFF);
	SPI_MasterTransmit_c((cData>>16) & 0xFF);
	SPI_MasterTransmit_c((cData>>8) & 0xFF);
	SPI_MasterTransmit_c(cData & 0xFF);	
}

/********************************************************************************/
//             MEAG8 SPI Write/Read subroutine
// Function: char SPI_MasterTransmit(char cData)
// when used as writing fuction, cData is the data to be write to 
// SPI slave. When used as read function, set cData equal to 0x0, 
// this subroutine returns data sent from SPI slave.
/********************************************************************************/
/* receive a char data */
u8 SPI_MasterReceive_c()
{
        /* Set SS# pin to low */
        SPI_SS = 0;   
        /* start data transition*/
        SPDR = 0;
        /* waiting for data transition end */
        while(!(SPSR & (1<<SPIF)));
        /* return data and clear SPIF */
        return(SPDR);
}

/* recieve a long data MSR first*/
u32 SPI_MasterReceive_l()
{
        u32 ret;
	ret = SPI_MasterReceive_c();
	ret = (ret<<8) + SPI_MasterReceive_c();
	ret = (ret<<8) + SPI_MasterReceive_c();
	ret = (ret<<8) + SPI_MasterReceive_c();
	return ret;
}