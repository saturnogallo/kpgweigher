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
   0,0,100,1,1,1000};

PRBDATA   eeprom rprbdata = {
   {"0000E+00",   "0000E+00",   "0000E+00",   "0000E+00"},
   {"0000E+00",   "0000E+00",   "0000E+00",   "0000E+00"},
   {"0000E+00",   "0000E+00",   "0000E+00",   "0000E+00"},
   {"0000E+00",   "0000E+00",   "0000E+00",   "0000E+00"}                     
 };
                                            
/*****************************************************************************/
//                     Timer 2 interrupt service routine
// Timer 2 overflow interrupt service routine.
// Generate 10ms interrupt.
// Support up to 3 timer services. 
// timerlen[3] store counters for different 
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
   // Initialize SPI to communicate with CH453
   SPI_MasterInit();                           

   // initialize golbal interrupt.
   #asm("sei")                        
}
