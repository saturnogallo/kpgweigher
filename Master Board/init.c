// System Initialization file

#include <mega64.h>

/**************************************************************************************/
//                              Initialize Port A-G
/**************************************************************************************/
void Init_Port()
{
 // Input/Output Ports initialization
 // Port A initialization
 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
    PORTA=0x00;
    DDRA=0x00;

 // Port B initialization
 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=Out 
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=1 
 // Use PORTB[7:4] as LED output, 0 -> LED ON
    PORTB=0x01;
    DDRB=0xF1;

 // Port C initialization
 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
 // State7=1 State6=1 State5=1 State4=1 State3=0 State2=0 State1=0 State0=0 
    PORTC=0xF0;
    DDRC=0xFF;

 // Port D initialization
 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
  // Use PORTD[7:6] as LED output, 0 -> LED ON
    PORTD=0x00;
    DDRD=0xF0;

 // Port E initialization
 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=In Func0=In 
 // State7=T State6=T State5=T State4=T State3=T State2=0 State1=T State0=T 
    PORTE=0x00;
    DDRE=0x04;

 // Port F initialization
 // Input 
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
    PORTF=0x00;
    DDRF=0x00;

 // Port G initialization
 // Func4=out Func3=out Func2=out Func1=out Func0=out 
 // State4=0 State3=0 State2=0 State1=0 State0=0 
    PORTG=0xFF;
    DDRG=0x1F;
}

/**************************************************************************************/
//                              Initialize Timers
/**************************************************************************************/
void Init_Timers()
{
 // Timer/Counter 0 initialization
 // Clock source: System Clock
 // Clock value: Timer 0 Stopped
 // Mode: Normal top=FFh
 // OC0 output: Disconnected
    ASSR=0x00;
    TCCR0=0x00;
    TCNT0=0x00;
    OCR0=0x00;

 // Timer/Counter 1 initialization
 // Clock source: System Clock
 // Clock value: 14.400 kHz
 // Mode: Normal top=FFFFh
 // OC1A output: Discon.
 // OC1B output: Discon.
 // OC1C output: Discon.
 // Noise Canceler: Off
 // Input Capture on Falling Edge
 // Timer 1 Overflow Interrupt: On
 // Input Capture Interrupt: Off
 // Compare A Match Interrupt: Off
 // Compare B Match Interrupt: Off
 // Compare C Match Interrupt: Off
    TCCR1A=0x00;
    TCCR1B=0x05;
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;
    OCR1CH=0x00;
    OCR1CL=0x00;

 // Timer/Counter 2 initialization
 // Clock source: System Clock
 // Clock value: Timer 2 Stopped
 // Mode: Normal top=FFh
 // OC2 output: Disconnected
    TCCR2=0x00;
    TCNT2=0x00;
    OCR2=0x00;

 // Timer/Counter 3 initialization
 // Clock source: System Clock
 // Clock value: Timer 3 Stopped
 // Mode: Normal top=FFFFh
 // Noise Canceler: Off
 // Input Capture on Falling Edge
 // OC3A output: Discon.
 // OC3B output: Discon.
 // OC3C output: Discon.
 // Timer 3 Overflow Interrupt: Off
 // Input Capture Interrupt: Off 
 // Compare A Match Interrupt: Off
 // Compare B Match Interrupt: Off
 // Compare C Match Interrupt: Off
    TCCR3A=0x00;
    TCCR3B=0x00;
    TCNT3H=0x00;
    TCNT3L=0x00;
    ICR3H=0x00;
    ICR3L=0x00;
    OCR3AH=0x00;
    OCR3AL=0x00;
    OCR3BH=0x00;
    OCR3BL=0x00;
    OCR3CH=0x00;
    OCR3CL=0x00; 

 // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=0x04;
    ETIMSK=0x00;     
}               

/**************************************************************************************/
//                              Enable External Memory
/**************************************************************************************/
void Enable_XMEM()
{
 // External SRAM page configuration: 
 // 1100h - 7FFFh / 8000h - FFFFh
 // Lower page wait state(s): 2r/w +1 addr
 // Upper page wait state(s): 2r/w + 1 addr
    MCUCR=0xC0;    
    //MCUCR = 0x80;
    
    XMCRA=0x4E;
    //XMCRA=0x0A;
 // PC7:0 can not be released 
    XMCRB=0x80;  
    // XMCRB = 0x07;
}

/**************************************************************************************/
//                              Initialize External Interrupt
/**************************************************************************************/
void Init_Ex_Interrupt()
{
 // External Interrupt(s) initialization
 // INT0: Off
 // INT1: Off
 // INT2: Off
 // INT3: Off
 // INT4: On
 // INT4 Mode: Rising Edge
 // INT5: On
 // INT5 Mode: Rising Edge
 // INT6: On
 // INT6 Mode: Rising Edge
 // INT7: On
 // INT7 Mode: Rising Edge
    EICRA=0x00;
    EICRB=0xFF;
    EIMSK=0xF0;
    EIFR=0xF0;   
}

/**************************************************************************************/
//                              Analog Comparator initialization
/**************************************************************************************/
void Init_ADC()
{
 // Analog Comparator initialization
 // Analog Comparator: Off
 // Analog Comparator Input Capture by Timer/Counter 1: Off
    ACSR=0x80;
    SFIOR=0x00;
}