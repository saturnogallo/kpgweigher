// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.

// I/O registers definitions for the ATmega8

#ifndef _MEGA8_INCLUDED_
#define _MEGA8_INCLUDED_

//#pragma used+
sfrb TWBR=0;
sfrb TWSR=1;
sfrb TWAR=2;
sfrb TWDR=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      // 16 bit access
sfrb ADCSRA=6;
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRRL=9;
sfrb UCSRB=0xa;
sfrb UCSRA=0xb;
sfrb UDR=0xc;
sfrb SPCR=0xd;
sfrb SPSR=0xe;
sfrb SPDR=0xf;
sfrb PIND=0x10;
sfrb DDRD=0x11;
sfrb PORTD=0x12;
sfrb PINC=0x13;
sfrb DDRC=0x14;
sfrb PORTC=0x15;
sfrb PINB=0x16;
sfrb DDRB=0x17;
sfrb DDR_SPI=0x17; // SPI share with PORT B
sfrb PORTB=0x18;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   // 16 bit access
sfrb UBRRH=0x20;
sfrb UCSRC=0X20;
sfrb WDTCR=0x21;
sfrb ASSR=0x22;
sfrb OCR2=0x23;
sfrb TCNT2=0x24;
sfrb TCCR2=0x25;
sfrb ICR1L=0x26;
sfrb ICR1H=0x27;
sfrw ICR1=0x26;   // 16 bit access
sfrb OCR1BL=0x28;
sfrb OCR1BH=0x29;
sfrw OCR1B=0x28;  // 16 bit access
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  // 16 bit access
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  // 16 bit access
sfrb TCCR1B=0x2e;
sfrb TCCR1A=0x2f;
sfrb SFIOR=0x30;
sfrb OSCCAL=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TWCR=0x36;
sfrb SPMCR=0x37;
sfrb TIFR=0x38;
sfrb TIMSK=0x39;
sfrb GIFR=0x3a;
sfrb GICR=0x3b;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
//#pragma used-

// Interrupt vectors definitions

#define EXT_INT0 2
#define EXT_INT1 3
#define TIM2_COMP 4
#define TIM2_OVF 5
#define TIM1_CAPT 6
#define TIM1_COMPA 7
#define TIM1_COMPB 8
#define TIM1_OVF 9
#define TIM0_OVF 10
#define SPI_STC 11
#define USART_RXC 12
#define USART_DRE 13
#define USART_TXC 14
#define ADC_INT 15
#define EE_RDY 16
#define ANA_COMP 17
#define TWI 18
#define SPM_READY 19

// define EEPROM Register bits
#define EERIE 3
#define EEMWE 2
#define EEWE 1
#define EERE 0

// define watch dog register bits
#define WDE 3
#define WDCE 4

// define MEGA8 SPI pins
#define P_SS 2
#define P_MOSI 3
#define P_MISO 4
#define P_SCK 5 

#define SPIE 7
#define SPE 6 
#define MSTR 4

#define SPIF 7

#endif

