// System Initialization file
// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega64
#pragma used+
sfrb PINF=0;
sfrb PINE=1;
sfrb DDRE=2;
sfrb PORTE=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      // 16 bit access
sfrb ADCSRA=6;
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRR0L=9;
sfrb UCSR0B=0xa;
sfrb UCSR0A=0xb;
sfrb UDR0=0xc;
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
sfrb PORTB=0x18;
sfrb PINA=0x19;
sfrb DDRA=0x1a;
sfrb PORTA=0x1b;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   // 16 bit access
sfrb SFIOR=0x20;
sfrb WDTCR=0x21;
sfrb OCDR=0x22;
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
sfrb ASSR=0x30;
sfrb OCR0=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TIFR=0x36;
sfrb TIMSK=0x37;
sfrb EIFR=0x38;
sfrb EIMSK=0x39;
sfrb EICRB=0x3a;
sfrb XDIV=0x3c;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
#pragma used-
// Interrupt vectors definitions
// global.h    
// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega64
//#define NULL 0x0
                                        /*
        Port related definition
*/ 
void kbd_uart_push(unsigned char);
void scanner_uart_push(unsigned char);
void pc_uart_push(unsigned char);
void nav_uart_push(unsigned char);
void testA(unsigned char);
//#define PORT_3          SPORTA
//#define PORT_B          SPORTB
// Hardware related
                            void sleepms(unsigned int ms);
                              double nav_read();
void scanner_set_mode();
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
    PORTB=0x00;
    DDRB=0x00;
 // Port C initialization
 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
 // State7=1 State6=1 State5=1 State4=1 State3=0 State2=0 State1=0 State0=0 
 // PORTC[7:4]: chip select of 16C554 channels.
    PORTC=0xF0;
    DDRC=0xFF;
 // Port D initialization
 // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
    PORTD=0x00;
    DDRD=0x00;
 // Port E initialization
 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=In Func0=In 
 // State7=T State6=T State5=T State4=T State3=T State2=0 State1=T State0=T 
 // PORTE.0: Input (M_RXD0)
 // PORTE.1: output (M_TXD0)
 // PORTE.2: output (16C554 INTC#)
 // PORTE.3: output (16C554 reset)
    PORTE=0x06;
    DDRE=0x0E;
 // Port F initialization
 // Input 
 // State7=T State6=T State5=T State4=T State3=T State2=T State1=Out State0=Out
 // PORTF[3:0]: LED[4:1]
    (*(unsigned char *) 0x62)=0x0F;
    (*(unsigned char *) 0x61)=0x0F;
 // Port G initialization
 // Input 
    (*(unsigned char *) 0x65)=0x00;
    (*(unsigned char *) 0x64)=0x00;
}
/**************************************************************************************/
//                              Initialize Timers
/**************************************************************************************/
void Init_Timers()
{ 
 // Timer/Counter 0 initialization
 // Clock source: System Clock
 // Clock value: 14.400 kHz
 // Mode: Normal top=FFh
 // OC0 output: Disconnected
    ASSR=0x00;
    TCCR0=0x07;
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
    (*(unsigned char *) 0x79)=0x00;
    (*(unsigned char *) 0x78)=0x00;
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
    (*(unsigned char *) 0x8b)=0x00;
    (*(unsigned char *) 0x8a)=0x00;
    (*(unsigned char *) 0x89)=0x00;
    (*(unsigned char *) 0x88)=0x00;
    (*(unsigned char *) 0x81)=0x00;
    (*(unsigned char *) 0x80)=0x00;
    (*(unsigned char *) 0x87)=0x00;
    (*(unsigned char *) 0x86)=0x00;
    (*(unsigned char *) 0x85)=0x00;
    (*(unsigned char *) 0x84)=0x00;
    (*(unsigned char *) 0x83)=0x00;
    (*(unsigned char *) 0x82)=0x00; 
 // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=0x05;
    (*(unsigned char *) 0x7d)=0x00;     
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
        (*(unsigned char *) 0x6d)=0x4E;
    //XMCRA=0x0A;
 // PC7:0 can not be released 
    (*(unsigned char *) 0x6c)=0x80;  
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
    (*(unsigned char *) 0x6a)=0x00;
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
