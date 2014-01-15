/********************************************************************************
 Author : CAC (China Applications Support Team) 

 Date :   January, 2012

 File name :   ADuC7026Driver.c

 Description :	 Use the GPIO to simulate the SPI communication of AD5791

 Hardware plateform : 	ADuC7026 and AD5791
********************************************************************************/

#include "ADuC7026.h"
#include "ADuC7026Driver.h"
#include "stdio.h"

#pragma import(__use_no_semihosting_swi)

/*    Function Pointers for Interrupts  */
// Copied from irq_arm.c in Keil uV4, required 
tyVctHndlr    IRQ     = (tyVctHndlr)0x0;
tyVctHndlr    SWI     = (tyVctHndlr)0x0;
tyVctHndlr    FIQ     = (tyVctHndlr)0x0;
tyVctHndlr    UNDEF   = (tyVctHndlr)0x0;
tyVctHndlr    PABORT  = (tyVctHndlr)0x0;
tyVctHndlr    DABORT  = (tyVctHndlr)0x0;

void	IRQ_Handler   (void) __irq;
void	SWI_Handler   (void) __irq;
void	FIQ_Handler   (void) __irq;
void	Undef_Handler (void) __irq;
void	PAbt_Handler  (void) __irq;
void	DAbt_Handler  (void) __irq;

void	IRQ_Handler(void) __irq
{
	if ( *IRQ !=0x00)
	{
		IRQ();
	}
}

void	FIQ_Handler(void) __irq
{
	if ( *FIQ !=0x00)
	{
		FIQ();
	}
}

void	SWI_Handler(void) __irq
{
	if ( *SWI !=0x00)
	{
		SWI();
	}
}

void	Undef_Handler(void)__irq 
{
	if ( *UNDEF !=0x00)
	{
		UNDEF();
	}
}

void	PAbt_Handler(void) __irq
{
	if ( *PABORT !=0x00)
	{
		PABORT();
	}
}

void	DAbt_Handler(void) __irq
{
	if ( *DABORT !=0x00)
	{
		DABORT();
	}
}
/*    Function Pointers for Interrupts  */

/* GPIO Control */
unsigned char ADuC7026InputBit(unsigned char GPIONum)
{
	unsigned long int Temp;
	unsigned char Data;

	Temp=0xFFFFFFFF-(1<<((GPIONum&0x0F)+24));

	switch(GPIONum>>4)
	{
		case	0:
			GP0DAT&=Temp;
			Data=GP0DAT;		
			break;
		case	1:
			GP1DAT&=Temp;
			Data=GP1DAT;
			break;
		case	2:
			GP2DAT&=Temp;
			Data=GP2DAT;
			break;
		case	3:
			GP3DAT&=Temp;
			Data=GP3DAT;
			break;
		case	4:
			GP4DAT&=Temp;
			Data=GP4DAT;
			break;
	}
	if((Data&(1<<(GPIONum&0x0F)))==(1<<(GPIONum&0x0F)))
	{
		Data=1;
	}
	else
	{
		Data=0;
	}

	return Data;
}

void ADuC7026OutputBit(unsigned char GPIONum, unsigned char Data)
{
	unsigned long int Temp;

	Temp=1<<(GPIONum&0x0F);



	switch(GPIONum>>4)
	{
		case	0:
			GP0DAT|=(Temp<<24);
			if(Data==0)
			{
				GP0CLR=(Temp<<16);
			}
			else
			{
				GP0SET=(Temp<<16);	
			}
			break;
		case	1:
			GP1DAT|=(Temp<<24);
			if(Data==0)
			{
				GP1CLR=(Temp<<16);
			}
			else
			{
				GP1SET=(Temp<<16);	
			}
			break;
		case	2:
			GP2DAT|=(Temp<<24);
			if(Data==0)
			{
				GP2CLR=(Temp<<16);
			}
			else
			{
				GP2SET=(Temp<<16);	
			}
			break;
		case	3:
			GP3DAT|=(Temp<<24);
			if(Data==0)
			{
				GP3CLR=(Temp<<16);
			}
			else
			{
				GP3SET=(Temp<<16);	
			}
			break;
		case	4:
			GP4DAT|=(Temp<<24);
			if(Data==0)
			{
				GP4CLR=(Temp<<16);
			}
			else
			{
				GP4SET=(Temp<<16);	
			}
			break;
	}
}
/* GPIO Control */

// add the c file here
void ADuC7026InitializePll(void)
{	
	//41.78MHz
	POWKEY1 = 0x01;				//Start PLL setting,changeless
	POWCON=0x00;
	POWKEY2 = 0xF4;				//Finish PLL setting,changeless
}

void ADuC7026InitializeUart(void)
{
	// Setup tx & rx pins on SPM 0 and SPM 1
	GP1CON |= 0x11;
 
	//Initiate the UART Port to 115200bps
	// CD = 0 !		 
	COMCON0 = 0x80;					// Setting DLAB
   	COMDIV0 = 0x0B;					// Setting DIV0 and DIV1 to DL calculated
	COMDIV1 = 0x00;

   	COMCON0 = 0x07;					// Clearing DLAB
	// fractional divider
  	COMDIV2 = 0x883E;			  	// M=1
									// N=01101010101  =853
									// M+N/2048	 =1.4165
									//41.78MHz/(16*2*2^CD*DL*(M+N/2048))	 //CD=0  DL=0B=11
									//115.2Kbps  M+N/2048 =1.0303	M=1, N=	 62=0x3EH=000 0011 1110
									//comdiv2=0x883E
		
}

void ADuC7026InitializeInterrupt(void)
{
	// disable all FIQs and IRQs
	FIQEN=0x00;
	IRQEN=0x00;
}

void ADuC7026Initialization(void)
{
	ADuC7026InitializePll();
	ADuC7026InitializeInterrupt();
	ADuC7026InitializeUart();

	/* initialize the GPIOs for controlling the AD5791*/
	ADuC7026OutputBit(LDAC, 1);
	ADuC7026OutputBit(SYNC, 1);
	ADuC7026OutputBit(CLR, 1);
	ADuC7026OutputBit(SCLK, 1);
	ADuC7026OutputBit(SDI, 1);
	ADuC7026InputBit(SDO);
}

// delay for Time*1ms, at 41.78MHz CLK.		41.78M / 256 / 163 = 1K
void ADuC7026DelayMs( unsigned int TimeMs)		
{								 
	unsigned int i;				 

	for(i=0; i<TimeMs; i++)
	{	
		T1LD = 163;			// 163 Counter Value,1ms
		T1CON = 0xC8;		// Enabled, Non-periodic,Binary and CLK/256
		while(T1VAL>1) {}
		T1CON = 0x48;		// Disabled, Non-periodic,Binary and CLK/256
	}	
}

// delay for Time*1ms, at 41.78MHz CLK.		41.78M  / 42 = 1M
void ADuC7026DelayUs( unsigned int TimeUs)		
{								 
	unsigned int i;				 

	for(i=0; i<TimeUs; i++)
	{	
		T1LD = 42;			// 163 Counter Value,1ms
		T1CON = 0xC0;		// Enabled, Non-periodic,Binary and CLK / 1
		while(T1VAL>1) {}
		T1CON = 0x40;		// Disabled, Non-periodic,Binary and CLK / 1
	}	
}

void ADuC7026SpiOperation(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)
{
	unsigned	char	WriteData, ReadData;
  	unsigned	char	i, j;

	ADuC7026OutputBit(SYNC,0);

	ADuC7026OutputBit(SCLK,1);	

	for(i=0; i<NumberOfByte; i++)
 	{
	 	WriteData = *(WriteBuffer + i);
		ReadData = 0;

		for(j=0; j<8; j++)
		{
			ADuC7026OutputBit(SCLK,1);

			if(0x80 == (WriteData & 0x80))
			{
				ADuC7026OutputBit(SDI,1);	  //Send one to SDI pin
			}
			else
			{
				ADuC7026OutputBit(SDI,0);	  //Send zero to SDI pin
			}
			WriteData = WriteData << 1;
			ReadData = (ReadData<<1) | ADuC7026InputBit(SDO); 

			ADuC7026OutputBit(SCLK,0);	/////

		}
		*(ReadBuffer + i)= ReadData;
	}  

	ADuC7026OutputBit(SCLK,1);

	ADuC7026OutputBit(SYNC,1);
}

/*    Rewrite the serial port function  */
/* 	  To use the pfrintf() in ADuC702x in Keil UV4, the sendchar() must be rewrite */
int sendchar (int ch)  {                 
      while(!(0x020==(COMSTA0 & 0x020)))	{}
 
 	COMTX = ch;

	while(!(0x020==(COMSTA0 & 0x020)))	{}

	return ch;
}

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
FILE __stderr;

int fputc (int ch, FILE *f) {
  return (sendchar(ch));
}

int fgetc (FILE *fp)  {
  return (0);
}


int fclose (FILE* f) {
  return 0;
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}

int fseek (FILE *fp, long nPos, int nMode)  {
  return (0);
}

int fflush (FILE *pStream)  {
  return (0);
}

void _ttywrch (int ch) {
  sendchar(ch);
}

void _sys_exit (int return_code) {
label:  goto label;  /* endless loop */
}
/*    Rewrite the serial port function  */
