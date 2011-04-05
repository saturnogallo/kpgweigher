/********************************************************************************/
//                             TL16C554 Driver
// Controller:  MEGA64
// 16C554 clk:  7.3728MHZ
//
// Schematic Description: 
// 16C554 <---> MEGA64
// INTA   <---> INT4
// INTB   <---> INT5
// INTC   <---> INT6
// INTD   <---> INT7
// CSA#   <---> A15
// CSB#   <---> A14
// CSC#   <---> A13
// CSD#   <---> A12
//   A0   <---> A8
//   A1   <---> A9
//   A2   <---> A10
// RESET  <---> PB0
// INTN#  <---> PE2

// Coded by: fd_lxh@yahoo.com.cn
/********************************************************************************/
#include "mega64.h"
#include "global.h"
#include "16c554.h"
#include "uart.h" 

#define FCR_DATA 0b00000111   // FIFO trigger level: 1 byte
#define IER_DATA 0b00000001
// LCR.7 must be cleared to 0 when accessing RBR/THR/IER
// LCR.7 must be set to 1 when accessing divisor latch
#define LCR_DATA 0b00011011   // Even Parity 8 data bit, 1 stop bit
#define DLL_DATA 0x30          // 115200bps: DLL = 0x4, 57600bps: DLL= 0x8, 38400 0xC: 9600 : 0x30  
#define DLM_DATA 0x0
#define MCR_DATA 0b00001000

#define min(A,B)        ((A<B)?(A):(B))
/********************************************************************************/
//               TLC16C554 UART PORT A Interrupt Subroutine
/********************************************************************************/
interrupt [EXT_INT4] void ext_int4_isr(void)
{
   u8 rundata;     
   switch(UC_554A_IIR & 0xF)
   { 
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int          
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = UC_554A_RBR;        // read data from receiver FIFO. RBR read to clear int          
          cm_pushA(rundata);
          return;
      default:
          break;
   }           
   
   rundata = UC_554A_LSR;        // read LSR to clear interrupt. dummy read     
}
/********************************************************************************/
//               TLC16C554 UART PORT B Interrupt Subroutine
/********************************************************************************/
// External Interrupt 5 service routine
interrupt [EXT_INT5] void ext_int5_isr(void)
{
   u8 rundata;           
   switch(UC_554B_IIR & 0xF)
   { 
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int          
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = UC_554B_RBR;        // read data from receiver FIFO. RBR read to clear int          
          cm_pushB(rundata);
          return;
      default:
          break;
   }                   
   rundata = UC_554B_LSR;        // read LSR to clear interrupt. dummy read     
}
/********************************************************************************/
//               TLC16C554 UART PORT C Interrupt Subroutine
/********************************************************************************/
// External Interrupt 6 service routine
interrupt [EXT_INT6] void ext_int6_isr(void)
{
   u8 rundata;
   switch(UC_554C_IIR & 0xF)
   { 
      case 0xC:                         // character time out. RBR read to clear interrupt
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int          
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = UC_554C_RBR;        // read data from receiver FIFO. RBR read to clear int          
          cm_pushC(rundata);
          return;
      default:
          break;
   }                   

   rundata = UC_554C_LSR;        // read LSR to clear interrupt. dummy read     
}

/********************************************************************************/
//               TLC16C554 UART PORT D Interrupt Subroutine
/********************************************************************************/
// External Interrupt 7 service routine
interrupt [EXT_INT7] void ext_int7_isr(void)
{
   u8 rundata;
   switch(UC_554D_IIR & 0xF)
   { 
      case 0xC:                         // character time out. RBR read to clear interrupt            
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int          
          break;
      case 0x4:                         // Receiver data available or trigger level reached
          rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int          
          cm_pushD(rundata);
          return;
      default:
          break;
   }                   
   rundata = UC_554D_LSR;        // read LSR to clear interrupt. dummy read     
}

/********************************************************************************/
//                             Reset 16C554
/********************************************************************************/
void Reset_554(void)
{
   // reset 16C554. Reset pin of 16C554 is connected to PB0 of MEGA64
      PORTB.0 = 1;                                                         
      sleepms(500);    // Delay
      PORTB.0 = 0;     // Recover from reset
      sleepms(500);    // Delay
}
/********************************************************************************/
//                             Initialize 16C554 
/********************************************************************************/
void Init_554(void)
{
      Reset_554();  // Reset 554
   
   /********************************************************************/      
   //                      Initialize UART A
   /********************************************************************/         
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      UC_554A_LCR = 0x80;
      UC_554A_DLL = DLL_DATA;
      UC_554A_DLM = DLM_DATA;     
   
   // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      UC_554A_LCR = LCR_DATA;

   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      UC_554A_FCR = FCR_DATA;
            
   // Set Modem Control Register:
   // Enable enternal interrupt
      UC_554A_MCR = MCR_DATA;
      
   // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      UC_554A_IER = IER_DATA;                                                  

   /********************************************************************/      
   //                      Initialize UART B
   /********************************************************************/         
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
   // LCR.7 must be set to "1" before setting baud rate
      UC_554B_LCR = 0x80;
      UC_554B_DLL = DLL_DATA;
      UC_554B_DLM = DLM_DATA;     
   
   // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      UC_554B_LCR = LCR_DATA;

   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      UC_554B_FCR = FCR_DATA;
            
   // Set Modem Control Register:
   // Enable enternal interrupt
      UC_554B_MCR = MCR_DATA;
      
   // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      UC_554B_IER = IER_DATA;

   /********************************************************************/      
   //                      Initialize UART C
   /********************************************************************/         
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      UC_554C_LCR = 0x80;
      UC_554C_DLL = DLL_DATA;
      UC_554C_DLM = DLM_DATA;     
   
   // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      UC_554C_LCR = LCR_DATA;

   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      UC_554C_FCR = FCR_DATA;
            
   // Set Modem Control Register:
   // Enable enternal interrupt
      UC_554C_MCR = MCR_DATA;
      
   // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      UC_554C_IER = IER_DATA;      
   
   /********************************************************************/      
   //                      Initialize UART D
   /********************************************************************/         
   // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
      UC_554D_LCR = 0x80;
      UC_554D_DLL = DLL_DATA;
      UC_554D_DLM = DLM_DATA;     
   
   // Set Line Control Register:
   // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
      UC_554D_LCR = LCR_DATA;

   // Set FIFO Control Register:
   // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
      UC_554D_FCR = FCR_DATA;
            
   // Set Modem Control Register:
   // Enable enternal interrupt
      UC_554D_MCR = MCR_DATA;
      
   // Set Interrupt Enable Register:
   // Enable Received data available / timeout / transmitter holding register empty &
   // Receiver line status interrupt
      UC_554D_IER = IER_DATA; 
}                                            


/********************************************************************************/
//                 Send multi-bytes (less than 16 bytes) via UART
// Function: 
// Send multi-bytes ( < 16 Bytes) through the port specified by "uart_port"
// format:
// print_554(u8 *str, char uart_port)
// arg1: string to be sent. arg2: port (A B,C,D)
/********************************************************************************/
void prints(u8 *str, u8 length, char uart_port)
{           
    u8 len, i;
    len = length & 0x0F;        //15bytes at most
    switch(uart_port)
    {
       case SPORTA:
           // wait if data still being transmitted in UART
           // LSR5: Transmitter Holding Register Empty, 1 Empty, 0: Not Empty
           // LSR6: Transmitter Register Empty. 1 Empty, 0: Not Empty.
           // check if FIFO is empty by LSR5. 
           // Note: (UC_554A_LSR) & 0x20 bracket here is necessary because 
           // UC_554A_LSR is not a register name, 
           // it is replaced by "*(volatile u8 *)(UARTA_BASE_ADDR+0x300)" during compilation
           // thus UC_554A_LSR & 0x20 actually means *(volatile u8 *) ((UARTA_BASE_ADDR+0x300) & 0x20)
           // here in 16c554.h, a bracket is added
           while(len)
           {
              while( !(UC_554A_LSR & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */              
                  UC_554A_THR = *str++;           
              len = len - min(len,16);
           }               
           break;
       case SPORTB:
           while(len)
           {
              while( !(UC_554B_LSR & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */              
                  UC_554B_THR = *str++;           
              len = len - min(len,16);
           }               
           break;
       case SPORTC:
           while(len)
           {
              while( !(UC_554C_LSR & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */              
                  UC_554C_THR = *str++;           
              len = len - min(len,16);
           }               
              
           break;
       case SPORTD:
           while(len)
           {
              while( !(UC_554D_LSR & 0x20) );  /* wait for FIFO to be empty */
              for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */              
                  UC_554D_THR = *str++;           
              len = len - min(len,16);
           }               
           break;
       case SPORT0:
           while(len-- > 0){    
                com0_putc(*str++);            
           }
           break;
       case SPORT1:
           while(len-- > 0){
                com1_putc(*str++);            
           }
           break;
       default:
           PORTC = 0xF0;
           break;
    }
}

           
