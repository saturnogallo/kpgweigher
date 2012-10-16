/*****************************************************
This program was produced by the
CodeWizardAVR V1.24.8b Professional
Automatic Program Generator
© Copyright 1998-2006 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project :
Version :
Date    : 2010-5-2
Author  : F4CG
Company : F4CG
Comments:


Chip type           : ATmega16L
Program type        : Application
Clock frequency     : 7.372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
*****************************************************/

/*************************************************************************************************************/

/*************************************************************************************************************/
#include <mega16.h>
#include "define.h"
#include "spi.h"
#include "drveeprom.h"
#include "uart.h"
#include "utili.h"


/*********************************************************************************************************************/
// Boot vetctors: This structure is used to share boot configuration info between primary firmware and boot firmware.
/*********************************************************************************************************************/
#define BOOT_VECT_ADDR 0x60
//BOOT_CONFIG boot_vector @ BOOT_VECT_ADDR;  /* store boot vector at a specified address in SRAM for sharing with primary */
                                           /* firmware. The first 96 (0x0~0x5F) addresses are reserved for I/O and registers */

SYSDATA eeprom sysdata = {
   0,0,100,3,
   {0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
   0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
   0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
   {0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
   0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
   0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
   1,1,1000};

PRBDATA   eeprom rprbdata = {
   {0.0,0.0},
   {0.0,0.0},
   {0.0,0.0},
   {0.0,0.0},
   {3,3}
 };

/*****************************************************************************/
//                     Timer 2 interrupt service routine
// Timer 2 overflow interrupt service routine.
// Generate 10ms interrupt.
// Support up to 3 timer services. timerlen[3] store counters for different
// timers.
/*****************************************************************************/
//bit timeoutflag;
#define DELAY1_END timerlen[0]==0
#define DELAY2_END timerlen[1]==0
#define DELAY3_END timerlen[2]==0
u16 timerlen[4] = {0,0,0,0};

interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
  TCNT2=0xB8;            // 7.2KHZ input,interrupt frequency is set to 100HZ (10ms timer)
  // if timerlen != 0, at least 1 timer service is ongoing.
  if(timerlen[0] || timerlen[1] || timerlen[2] || timerlen[3]){
    if(timerlen[0]>0)   timerlen[0]--;
    if(timerlen[1]>0)   timerlen[1]--;
    if(timerlen[2]>0)   timerlen[2]--;
    if(timerlen[3]>0)   timerlen[3]--;
  }else{
    TIMSK &= 0xbf;       // mask Timer2 overflow interrupt
  }
}

/*****************************************************************************/
//                      Initialize Timers
// Timer 0 is used to generate pulses to drive electrical magnet
// Timer 1 is used to generate pulses to drive motors
// Timer 2 is used to measure RS485 frame intervals.
//
/*****************************************************************************/
void Init_Timers()
{
   // For Stepping Motor
   // Timer/Counter 0 initialization
   // Clock source: System Clock
   // Clock value: 7.200 kHz. Configured in TCCR0
   TCCR0=0x05;
   // define Timer0 interrupt frequency: 7.2K/(FF-TCNT) = 514HZ
   // 200 pulses to run one cycle (1.8/step)
   // Motor Speed: 514/200 ~= 2.5 cycles/sec
   TCNT0=0xF2;

   // Timer/Counter 1 initialization
   // Used for delay from 220V AC cross zero point to Triac ON.
   // Clock source: System Clock
   // Clock value: 115.200 kHz
   // Mode: Normal top=FFFFh
   // OC1A output: Discon.
   // OC1B output: Discon.
   // Noise Canceler: Off
   // Input Capture on Falling Edge
   // Timer 1 Overflow Interrupt: On
   // Input Capture Interrupt: Off
   // Compare A Match Interrupt: Off
   // Compare B Match Interrupt: Off
   TCCR1A=0x00;
   TCCR1B=0x03;
   TCNT1H=0x00;
   TCNT1L=0x00;
   ICR1H=0x00;
   ICR1L=0x00;
   OCR1AH=0x00;
   OCR1AL=0x00;
   OCR1BH=0x00;
   OCR1BL=0x00;

   // For Time Delay
   // Timer/Counter 2 initialization
   // Clock source: System Clock
   // Clock value: 7.200 kHz
   // Mode: Normal top=FFh
   // OC2 output: Disconnected
   ASSR=0x00;
   TCCR2=0x07;
   TCNT2=0x00;
   OCR2=0x00;

   // Timer(s)/Counter(s) Interrupt(s) initialization
   // disable Timer 0, 1 and 2 interrupt
   TIMSK=0x00;
}

/*******************************************************************************/
//                          MEGA8 PORT initialization
/*******************************************************************************/
void Init_PORT(void)
{

   // Input/Output Ports initialization
   // Port A initialization
   // Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
   // State7=T State6=T State5=1 State4=1 State3=1 State2=1 State1=1 State0=1
   // PA0: MT1_PS, PA1: MT2_PS, PA2: MT_MD0, PA3: MT_MD1, PA4: MT_CLK, PA5: MT_DIR
   // PA6: PWM_FB, PA7: Test1
   PORTA=0x3F;
   DDRA=0x3F;

   // Port B initialization
   // Func7=Out Func6=In Func5=Out Func4=Out Func3=In Func2=In Func1=Out Func0=Out
   // State7=0 State6=T State5=0 State4=1 State3=T State2=T State1=1 State0=0
   PORTB=0x12;
   DDRB=0xB3;

   // Port C initialization
   // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
   // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
   PORTC=0x00;
   DDRC=0x00;

   // Port D initialization
   // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=Out Func0=In
   // State7=1 State6=1 State5=1 State4=1 State3=T State2=T State1=1 State0=T
   PORTD=0xF2;
   DDRD=0xF2;

   // External Interrupt(s) initialization
   // INT0: Off
   // INT1: Off
   // INT2: Off
   MCUCR=0x00;
   MCUCSR=0x00;

   // Timer(s)/Counter(s) Interrupt(s) initialization
   TIMSK=0x00;

   // Analog Comparator initialization
   // Analog Comparator: Off
   // Analog Comparator Input Capture by Timer/Counter 1: Off
   ACSR=0x03;   // interrupt on rising edge, disable AC interrupt when intialized. To be enabled later.
   SFIOR=0x00;

}

/*******************************************************************************/
// Enable watchdog timer.
// Timeout limit WDTCR[2:0]:
// 000 - 16.3ms    001 - 32.5ms    010 - 65ms    011 - 0.13s
// 100 - 0.26ms    101 - 0.52s     110 - 1.0s    111 - 2.1s
/*******************************************************************************/
#ifdef _DISABLE_WATCHDOG_
//dummy subroutine
void Feed_Watchdog(){;;}
void Turn_Off_Watchdog(){;;}
#else
void Feed_Watchdog()
{
  #asm("cli")
  #asm("wdr") // reset watchdog timer to avoid timeout reset
  WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
  if(flag_turn_off_watchdog)
     WDTCR = 0x00; // turn off watch dog timer.
  else
     WDTCR = 0x0F; // turn on watch dog timer: WDE = 1, WDP = 110, 1 sec.
  #asm("sei") // Enable global interrupt.
}
void Turn_Off_Watchdog()
{
  #asm("cli")
  #asm("wdr") // reset watchdog timer to avoid timeout reset
  WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
  WDTCR = 0x00; // turn off watch dog timer
  #asm("sei") // Enable global interrupt.
}
#endif

/*******************************************************************************/
//                    Initialize System Global Variable
// Read data from EEPROM and set vaule for system global variables
/*******************************************************************************/
void Init_Vars(void)
{
}

void Init_EEPROM_Para()
 {
 }
/*******************************************************************************/
//
/*******************************************************************************/
void Validate_EEPROM_Data()
{
}

/*****************************************************************************/
//                      Main function entry here
/*****************************************************************************/
void main(void)
{
   /********************************************************************/
   //               System Initialization
   /********************************************************************/
   // Initialize MEGA8 PORT
   Init_PORT();
   // Timer (Interrupt) initialization
   Init_Timers();
   // Initialize EEPROM
   vInitEeprom();

   // Initialize Global Variable:
   // "Init_Vars()" has to be placed after "bReadDataFromEeprom()" because
   // RS485._flash.cs_zero is involved.
   Init_Vars();

   // Initialize UART, 8 data, 1 stop, even parity. Boardrate is set based on
   // EEPROM parameters.
   UART_Init();
   // Initialize SPI to communicate with CS5532/CS5530
   SPI_MasterInit();

   // initialize golbal interrupt.
   #asm("sei")



}
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
#include <mega16.h>
#include "uart.h"
#include "define.h"
#include "drveeprom.h"

/****************************************************************************/
//                 UART Receiver/ Transmiter Variable Definition
/****************************************************************************/
// USART Transmitter buffer
u8 volatile tx_buffer[TX_BUFFER_SIZE];

// define index for transmitter
u8 volatile tx_wr_index;
u8 volatile tx_rd_index;
u8 volatile tx_counter;

//#ifndef _OLD_FASHION_CMD_PROCESS_
//u8 volatile rx_buffer[RX_BUFFER_SIZE];
// define index for Receiver
u8 volatile rx_wr_index;
u8 volatile rx_rd_index;
u8 volatile rx_counter;
//#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

extern u8 debug_mode;

/****************************************************************************/
//               UART Receiver Interrupt service routine
// UART communication frame structure:
// Head1(0xfe) Head2(0x68), addr_from, addr_to, cmd, data_len
// data_to_send_or_receive, check_sum.
// in normal mode, once a byte is received, function cm_pushc() will be called
// to check if a frame is received completed or not. cm_pushc() will analysize
// the frame and respond to the frame.
/****************************************************************************/
//#define HW_STATUS_UART_ERROR 0b00010000
interrupt [USART_RXC] void usart_rx_isr(void)
{
   u8 status,data;

   //read status and data
   status=UCSRA;
   data=UDR;
   // check if error happened.
   if((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
//#ifdef _OLD_FASHION_CMD_PROCESS_
       //hw_status &=0xEF;
//#else
      // rx_buffer[rx_wr_index++]= data;
      // if(rx_wr_index >= RX_BUFFER_SIZE)
      //    rx_wr_index=0;
      // rx_counter++;
//#endif
   }
   //else
      //hw_status |= HW_STATUS_UART_ERROR;
}

/****************************************************************************/
//               UART Transmitter Interrupt service routine
/****************************************************************************/
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter)
   {
      PORTB.0 = 1;         // set RS485 node to transmiter mode
      --tx_counter;
      UDR=tx_buffer[tx_rd_index];
      if (++tx_rd_index == TX_BUFFER_SIZE)
         tx_rd_index=0;
   }
else
   PORTB.0 = 0;         // set RS485 node to receiver mode.
}

/****************************************************************************/
//                     UART Set Baud rate.
// This subroutine sets UART baud rate based on user settings
// Users are allowed to change baudrate of RS485 nodes.
/****************************************************************************/
void SetBaudrate()
{
u8 baud = 0x0;
   switch(baud)
   {  case 0x0:                 // 115200bps
        UBRRH=0x00;
        UBRRL=0x03;
        break;
      case 0x1:                 // 57600bps
        UBRRH=0x00;
        UBRRL=0x07;
        break;
      case 0x2:                 // 38400bps
        UBRRH=0x00;
        UBRRL=0x0B;
        break;
      case 0x3:                 // 19200bps
        UBRRH=0x00;
        UBRRL=0x17;
        break;
      case 0x4:                 // 9600bps
        UBRRH=0x00;
        UBRRL=0x2F;
        break;
      default:                  // 115200bps
        UBRRH=0x00;
        UBRRL=0x03;
        break;
   }
}
/****************************************************************************/
//                     UART Initialization
// need to enable global interrupt in main function.
// This module only set UART interrupt bits.
/****************************************************************************/
void UART_Init(void)
{
 // Set RS485 node to receiver mode
    PORTB.0 = 0;

 // Reset UART buffer and flag;
    rx_buffer_overflow = 0;
    tx_wr_index=0;
    tx_rd_index=0;
    tx_counter=0;
//#ifndef _OLD_FASHION_CMD_PROCESS_
    rx_wr_index=0;
    rx_rd_index=0;
    rx_counter=0;
//#endif

 // USART initialization
 // Communication Parameters: 8 Data, 1 Stop, No Parity
 // USART Receiver: On
 // USART Transmitter: On
 // USART Mode: Asynchronous
    #asm("cli")
    UCSRA=0x00;

 // Enable TXC interrupt, RX interrupt, disable UDRE interrupt
    UCSRB=0xD8;

 // UCSRC share IO address with reg UBRR, MSB bit for UCSRC must be 1.
 // 8 data, 1 stop, even parity
    UCSRC=0xA6;    // EVEN parity bit
 // UCSRC=0x86;    // No parity bit

 // Set Baud rate based on System setting
 // RS485.BaudRate_Index can be read out from EEPROM
 // or set by RS485 master via RS485 communication
 // UCSRC share IO address with reg UBRR, MSB bit for UBRR must be 0.
 // don't enable interrupt here.
 // enable global interrupt in main function after all modules are initialized
    //RS485._flash.baudrate = 0;
    SetBaudrate();
}

/****************************************************************************/
//
//                 Higher Level UART print Subroutines
//
/****************************************************************************/
#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
/****************************************************************************/
// Send a char-Type data via uart.
/****************************************************************************/
void putchar(char c)
{
   // need to be changed to avoid waiting
   while (tx_counter >= TX_BUFFER_SIZE);

   // set RS85-node to transmiter mode
   PORTB.0 = 1;

   #asm("cli")
   // if there is data in TX buffer or data being transmitted.
   // then write data to buffer.
   if (tx_counter || ((UCSRA & DATA_REGISTER_EMPTY)==0))
      {
      tx_buffer[tx_wr_index]=c;
      if (++tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
      ++tx_counter;
      }
   // if no data in buffer and no data being transitted,
   // write data to UDR to start transimision immediately
   else
      UDR=c;
   #asm("sei")
}
/****************************************************************************/
// mputs(): UART prints a data array
/****************************************************************************/
void mputs(u8 *buf, u8 size, u8 port)
{
    while(size-- > 0)
      putchar(*buf++);
}

/****************************************************************************/
// Read a byte from UART buffer
/****************************************************************************/
/*#ifndef _OLD_FASHION_CMD_PROCESS_
u8 read_uart_db()
{
  u8 uart_rdata;
  uart_rdata = rx_buffer[rx_rd_index++];
  rx_rd_index %= RX_BUFFER_SIZE;
  rx_counter--;
  return uart_rdata;
}
u8 data_available_in_rxbuf()
{
  if (rx_counter > RX_BUFFER_SIZE)
  {
     rx_rd_index = 0;
     #asm("cli")
     rx_wr_index = 0;
     rx_counter = 0;
     #asm("sei")
  }
  return rx_counter;
}
#endif//*/

/****************************************************************************/
// d_putchar(): output a character in ASCII code mode.
// For example: 0x9b is converted to 2 characters '9'/'B' before output
/****************************************************************************/
/*void d_putchar(u8 a)
{
    unsigned char h,l;
    h = (a & 0xf0) >> 4;
    l = (a & 0x0f);
    if(h <= 9)
       putchar(h+'0');
    else
        putchar(h+'A'-0x0a);

    if(l <= 9)
	putchar(l+'0');
    else
	putchar(l+'A'-0x0a);
} //*/

/****************************************************************************/
// d_mputs() convert an ASCII code array to HEX and print them via UART
/****************************************************************************/
/*void d_mputs(u8 *buf, u8 size, u8 port)
{   while(size-- > 0)
      d_putchar(*buf++);
} //*/

/****************************************************************************/
// UART print a string
/****************************************************************************/
/*void putstr(flash u8 *ptr)
{
   while(*ptr != 0x00)
      putchar(*ptr++);
} //*/

#pragma used-
#endif

/************************************************************************/
// MEGA8 EEPROM Read and Write subroutines
// Interrupt mode is used to avoid wait during EEPROM writing
// Header files needed:
// drveeprom.h:
// mega8.h: MEGA8 related constant
/************************************************************************/
#define _DRVEEPROM_C_

#include "drveeprom.h"
#include <mega16.h>
#include "define.h"

/************************************************************************/

// define data buffer queue for EEPROM read/write
static S_EEPROM_POSITION volatile _sEepromBuf[EEPROM_WRITE_BUF_SIZE];

// EEPROM buffer write pointer/index(user writes to RAM buffer)
static u8 volatile _bEepromBufWrPtr;

// EEPROM buffer read pointer(read data from buffer and write to EEPROM)
static u8 volatile _bEepromBufRdPtr;

// number of data in EEPROM writing buffer
static u8 volatile _bEepromBufNs;

#define fgEepromBufEmpty()      (_bEepromBufNs == 0)
#define fgEepromBufFull()       (_bEepromBufNs == EEPROM_WRITE_BUF_SIZE)


/************************************************************************/
//               Write EEPROM using EEPROM Interrupt
// if buffer is empty, stop EEPROM interrupt
// if buffer is not empty(still some data to be written into EEPROM),
// write EEPROM.  The buffer here is a queue.
/************************************************************************/

//#pragma interrupt_handler vIvEeReady:iv_EE_READY
interrupt [EE_RDY] void vIvEeReady(void)
{
    if(!fgEepromBufEmpty()){
        while(EECR & (1<<EEWE));  /* wait for last write to complete */
        EEAR = _sEepromBuf[_bEepromBufRdPtr].wAddress;
        EEDR = _sEepromBuf[_bEepromBufRdPtr].bVal;
        EECR |= (1<<EEMWE);
        EECR |= (1<<EEWE);
        _bEepromBufRdPtr++;
        if(_bEepromBufRdPtr >= EEPROM_WRITE_BUF_SIZE)
            _bEepromBufRdPtr = 0;

        _bEepromBufNs--;
    }else{   // disable EEPROM interrupt if no more data to be written into EEPROM
        EECR &= ~(1<<EERIE);
    }
}

/************************************************************************/
//                     Write data into EEPROM buffer
// If buffer is not full, write data into buffer, return RET_SUCCESS(0x0)
// flag. If buffer is full, return RET_BUSY(0xff) flag
/************************************************************************/

u8 bWriteData2Eeprom_c(u16 wAddress, u8 bVal)
{
    if(fgEepromBufFull())
        return RET_BUSY;
    // add for EEPROM protection
    //if(!EN_EEPROM_WRITE)
    //   return RET_SUCCESS;

    // full file buffer if buffer is not full yet
    #asm("cli");     // disable all interrupt
    _sEepromBuf[_bEepromBufWrPtr].wAddress = wAddress;
    _sEepromBuf[_bEepromBufWrPtr].bVal = bVal;
    _bEepromBufWrPtr++;
    if(_bEepromBufWrPtr >= EEPROM_WRITE_BUF_SIZE)
        _bEepromBufWrPtr = 0;

    _bEepromBufNs++;

    EECR |= (1<<EERIE);
    #asm("sei"); // enable EEPROM ready interrupt and global interrupts

    return RET_SUCCESS;
}
u8 bWriteData2Eeprom(u16 wAddress, u8 *bVal, u8 totalsize){
  u8 uc_eeprom_data;
  while(totalsize > 0){
          if(bReadDataFromEeprom_c(wAddress,&uc_eeprom_data) != RET_SUCCESS){
                return totalsize;
          }
          // skip programming if data to be written is the same as EEPROM data
          if(uc_eeprom_data != *bVal) {
	         if(RET_SUCCESS != bWriteData2Eeprom_c(wAddress,*bVal))
	                return totalsize;       //wait until the write succeed
	  }
          totalsize--; // already programmed one, number of data left decreases by 1
          wAddress++;
          bVal++;
  }
  return totalsize;
}


/************************************************************************/
//                         Read data from EEPROM
// The programer must ensure EEPROM is NOT in writing state before reading
// EEPROM. This subroutine return RET_BUSY(0xff) flag when EEPROM is in
// writting state. Return RET_SUCCESS(0x0) flag if reading successfully
// Data readout is saved into read buffer.
/************************************************************************/
u8 bReadDataFromEeprom(u16 wAddress, u8 *pbVal,u8 size)
{
	while(size-- > 0){
		if(bReadDataFromEeprom_c(wAddress++,pbVal++) != RET_SUCCESS)
		        return RET_BUSY;
	}
	return RET_SUCCESS;
}
u8 bReadDataFromEeprom_c(u16 wAddress, u8 *pbVal)
{
    if(EECR & (1<<EEWE))
        return RET_BUSY;

    EEAR = wAddress;
    EECR |= (1<<EERE);
    *pbVal = EEDR;
    return RET_SUCCESS;
}

/************************************************************************/
//                        Initialize EEPROM
// Intialize buffer variables. Diable EEPROM interruption
// EEPROM interrupt is enabled after writing data to EEPROM buffer.
/************************************************************************/
void vInitEeprom(void)
{
    EECR = 0x00;
    _bEepromBufWrPtr = 0;
    _bEepromBufRdPtr = 0;
    _bEepromBufNs    = 0;
}

#include "define.h"
#include "wendu.h"
#include "utili.h"
#include "math.h"
#define logl log
#define powl pow
double PT100RToTValue(double r,double r0)
{
        double ac,bc,cc,tlow,tup,rnew,tnew;
        int count;
        ac = 3.908e-3;
        bc = -5.775E-7;
        cc = -4.183E-12;
        tlow = -200;
        tup = 850;
	count = 0;

	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = r0 + r0*ac*tnew + r0*bc*tnew*tnew;
		if(tnew < 0)
		         rnew = rnew + r0*cc*(tnew-100)*tnew*tnew*tnew;

		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;

}
double RValueToTValue(double r, u8 prbid)
{
	double ac,bc,cc,tlow,tup,rnew,tnew;
	int count;

	ac = rprbdata.param1[prbid];
	bc = rprbdata.param2[prbid];
	cc = rprbdata.param3[prbid];

	if(rprbdata.type[prbid] == PRBTYPE_PT100)
		return PT100RToTValue(r, cc);
	if(rprbdata.type[prbid] == PRBTYPE_PT1000)
		return PT100RToTValue(r, cc);
	if(rprbdata.type[prbid] != PRBTYPE_PT25)
		return -9999.999;
        if(rprbdata.rtp[prbid] > 0.1)
                r = r/rprbdata.rtp[prbid];
        else
		r = r/25.0;





	//set the search range of T between GetT(r) +/- 1 degree
	tlow = GetT(r) - 1;
	tup = tlow + 2;

	count = 0;

	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetWr(tnew);
		if((tnew >= (83.8058-273.15)) && (tnew <= (273.16-273.15)))
        		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*logl(rnew);
                else
        		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}
	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;


//	return r+1;
}

flash double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};
flash u8 sizeAi = sizeof(Ai)/sizeof(double);
flash u8 sizeCi = sizeof(Ci)/sizeof(double);

double GetWr(double t)
{
	double result;
	double pert;
	int i;
	if(t < 0)
	{       //equals to Wr189(t) in mfile
		result = Ai[0];
		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
		t = pert;
		for(i=1;i<sizeAi;i++)
		{
			result = result + Ai[i] * t;
			t = t*pert;
		}
		result = exp(result);
	}else{
			 result = Ci[0];
			 pert = t/481.0 - 1;
			 t = pert;
			 for(i=1;i<sizeCi;i++)
			 {
				 result = result + Ci[i] * t;
				 t = t*pert;
			 }
	}
	return result;
}
flash double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
flash double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
flash u8 sizeDi = sizeof(Di)/sizeof(double);
flash u8 sizeBi = sizeof(Bi)/sizeof(double);

double GetT(double w)
{
	double perw;
	double result;
	int i;

	if(w >= 0)
	{       //t660r
		perw = (w-2.64)/1.64;
		w = perw;
		result = Di[0];
		for(i=1;i<sizeDi;i++)
		{
			result = result + Di[i] * w;
			w = w*perw;
		}
	}else{   //t189(r)
		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
		w = perw;
		result = Bi[0];
		for(i=1;i<sizeBi;i++)
		{
			result = result + Bi[i] * w;
			w = w*perw;
		}
		result = 273.15*result - 273.15;
	}
	return result;
}

flash double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};

flash double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};

flash double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};

flash double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};

flash double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};

flash double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};

flash double BLow[]={0.000000000000E+00,-0.24650818346E-03,0.59040421171E-05,-0.13257931636E-08,0.15668291901E-11,-0.16944529240E-14,0.62990347094E-18};
flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};

flash double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};

flash double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};

flash int TLowLen = sizeof(TLow)/sizeof(double);
flash int THighLen = sizeof(THigh)/sizeof(double);

flash int SLowLen = sizeof(SLow)/sizeof(double);
flash int SMedLen = sizeof(SMed)/sizeof(double);
flash int SHighLen = sizeof(SHigh)/sizeof(double);

flash int RLowLen = sizeof(RLow)/sizeof(double);
flash int RMedLen = sizeof(RMed)/sizeof(double);
flash int RHighLen = sizeof(RHigh)/sizeof(double);

flash int NLowLen = sizeof(NLow)/sizeof(double);
flash int NHighLen = sizeof(NHigh)/sizeof(double);

flash int JLowLen = sizeof(JLow)/sizeof(double);
flash int JHighLen = sizeof(JHigh)/sizeof(double);

flash int BLowLen = sizeof(BLow)/sizeof(double);
flash int BHighLen = sizeof(BHigh)/sizeof(double);

flash int ELowLen = sizeof(ELow)/sizeof(double);
flash int EHighLen = sizeof(EHigh)/sizeof(double);

flash int KLowLen = sizeof(KLow)/sizeof(double);
flash int KHighLen = sizeof(KHigh)/sizeof(double);

flash double *coef;

double GetThmoVolt(double t,char type)
{
	double result,pert;
	int i;
	int len = 0;
	switch(type)
	{
		case PRBTYPE_T:
			coef = (t < 0) ? TLow : THigh;
			len = (t < 0) ? TLowLen : THighLen;
			break;
		case PRBTYPE_K:
			coef = (t < 0) ? KLow : KHigh;
			len = (t < 0) ? KLowLen : KHighLen;
			break;
		case PRBTYPE_N:
			coef = (t < 0) ? NLow : NHigh;
			len = (t < 0) ? NLowLen : NHighLen;
			break;
		case PRBTYPE_E:
			coef = (t < 0) ? ELow : EHigh;
			len = (t < 0) ? ELowLen : EHighLen;
			break;
		case PRBTYPE_B:
			coef = (t < 630.615) ? BLow : BHigh;
			len = (t < 630.615) ? BLowLen : BHighLen;
			break;
		case PRBTYPE_J:
			coef = (t < 760) ? JLow : JHigh;
			len = (t < 760) ? JLowLen : JHighLen;
			break;

		case PRBTYPE_S:
			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
			break;
		case PRBTYPE_R:
			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
			break;
		default:
			return 0.0;
	}
	if (len == 0)
		return 0.0;

	result = coef[0];
	if(type == PRBTYPE_K)
	{
		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	}

	pert = t;
	for(i=1;i< len ;i++)
	{
		result = result + coef[i] * t;
		t = t*pert;
	}
	return result;
}

double MValueToTValue(double r,char type)
{
	double tlow;
	double tup;
	double rnew;
	double tnew;
	int count = 0;
	switch(type)
	{
		case PRBTYPE_T:
			tlow =  -270;	tup = 400;
			break;
		case PRBTYPE_K:
			tlow =  -270;	tup = 1372;
			break;
		case PRBTYPE_N:
			tlow =  -270;	tup = 1300;
			break;
		case PRBTYPE_E:
			tlow =  -270;	tup = 1000;
			break;
		case PRBTYPE_B:
			tlow =  0;	tup = 1800;
			break;
		case PRBTYPE_J:
			tlow =  -210;	tup = 1200;
			break;
		case PRBTYPE_S:
			tlow =  -50;	tup = 1768;
			break;
		case PRBTYPE_R:
			tlow =  -50;	tup = 1768;
			break;
		default:
			return -9999.9999;
	}

	while((tup - tlow > 0.00005) && (count++ < 100))
	{
		tnew = (tlow+tup)/2.0;
		rnew = GetThmoVolt(tnew,type);

		if(r < rnew)
			tup = tnew;
		else
			tlow = tnew;
	}

	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
}

#include "utili.h"
#include "stdlib.h"
/*---------------ÑÓÊ±×Ó³ÌÐò----------------*/
/*
void delay1 (uint ms)
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;

}
*/
/*
char lowc(uchar x)
{
	x = x&0x0f;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}
char highc(uchar x)
{

	x = x>>4;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}
*/
