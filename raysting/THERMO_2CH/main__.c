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
//#include "spi.h"
#include "drveeprom.h"
#include "uart.h"
#include "utili.h"
#include "scanner.h"
#include "wendu.h"
//#include "stdlib.h"
/*********************************************************************************************************************/
// Boot vetctors: This structure is used to share boot configuration info between primary firmware and boot firmware.
/*********************************************************************************************************************/
#define BOOT_VECT_ADDR 0x60
//BOOT_CONFIG boot_vector @ BOOT_VECT_ADDR;  /* store boot vector at a specified address in SRAM for sharing with primary */
                                           /* firmware. The first 96 (0x0~0x5F) addresses are reserved for I/O and registers */
/*
	double          R0;  	 //zero offset
	double          V0;  	 //zero offset
	double          Rs1; 	 //jiao-zheng zhi for PT100
	double          Rs2; 	 //for PT1000
	u8             	ktime;	 //time for switch
	u8		type_bore;  //probe type of channel 1 and channel 2 for bore case
	u8              type_therm; //probe type of channel 1 and channel 2 for therm case
	u8		mode;	 //mode
	u8		kttmode; //ktt mode
	u8		fcmode;	 //F or C
	u8		chan;	 //channel mode
*/
SYSDATA eeprom sysdata = { 0,0,100,1000,4,1,1,1,1,0,0};

RUNDATA rundata;
int ch_to_search = 0;
extern u8 databuf[12];

extern double GetThmoVolt(double t,char type);
extern void HandleKey(u8);
extern void CH453_I2C_Init();
extern void init_digi_Led();
extern void digi_cprint(u8 flash *str,u8 startpos, u8 size);
extern void timer_tick();
extern u8 CH453_ReadKey();

/*
typedef eeprom struct _PRBDATA
{
	char param1[8]; //+0.000E+0
	char param2[8]; //+0.000E+0
	char param3[8]; //+0.000E+0
	char rtp[8];	//0000.0000
}PRBDATA;
*/
PRBDATA   eeprom rprbdata = {
        {"", ""},
        {"", ""},
        {"", ""},
        {"", ""}
 };

u8 start_of_buf()
{
        u8 i = 0;
        while(i< 16)
        {
                if(databuf[i] == 0x00)
                        return i;
                i++;
        }
        return i;
}
void myftoa(double v)
{
        u8 pos;
        int dat;
        sprintf(databuf,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
        dat = (int)v;
        sprintf(databuf,"%d",dat);
        pos = start_of_buf();
        databuf[pos] = '.';
        v = v - dat;
        if(v < 0)
                v = -v;
        pos = pos+1;
        while(1)
        {
                v = v * 10000;
                dat = (int)v;
                sprintf(databuf+pos,"%d",dat);
                v = v - dat;
                pos = start_of_buf();
                if(pos > 11)
                        return;
        }
}
void sleepms(unsigned int i)
{
   while(i-- > 0)
      ;
}
u8 keycode = KEY_INVALID;
u8 refresh_cnt = 0;
u8 busy = 0;
extern u8 flash_cnt;
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
        if(busy == 1)
                return;
        busy = 1;
	flash_cnt++;
	if(flash_cnt > 10)
	        flash_cnt = 0;

        if(keycode == KEY_INVALID)
        {
                keycode = CH453_ReadKey();
        }
        busy = 0;
}
/*****************************************************************************/
//                      Initialize Timers
// Timer 0 is used to generate pulses to drive electrical magnet
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
   TCNT0=0x92;            //60hz

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
   TCCR1B=0x00;
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
   TCCR2=0x00;
   TCNT2=0x00;
   OCR2=0x00;

   // Timer(s)/Counter(s) Interrupt(s) initialization
   // disable Timer 0, 1 and 2 interrupt
   TIMSK=0x01;
}

/*******************************************************************************/
//                          MEGA8 PORT initialization
/*******************************************************************************/
void Init_PORT(void)
{
   //PORT,DDR: 0,0, Hi-Z, 		1,0, Hi-Z pullup
   //          0,1, output low, 1,1, output high
   // Input/Output Ports initialization
   // Port A initialization
   // bit6,bit5,bit4: OUTPUT, others are No use , all set to Hi-Z
   PORTA=0x70;
   DDRA=0x70;

   // Port B initialization
   // bit7: OUT, bit6: OUT, bit 5: OUT bit4-bit1 no use bit0 : 485DIR, bit1
   // 11100000
   // 11100001
   PORTB=0xE0;
   DDRB=0xE1;

   // Port C initialization
   // bit7-bit2 no use bit1 = SDA, bit0,SCL
   PORTC=0x03;
   DDRC=0x03;

   // Port D initialization
   // bit7-bit2 out, bit1 :txd, bit0:rxd
   // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=Out Func0=In
   // State7=1 State6=1 State5=1 State4=1 State3=T State2=T State1=1 State0=T
   PORTD=0xFE;
   DDRD=0xFE;

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
extern void ValidatePrbType();
void Init_Vars(void)
{
        if((sysdata.Rs1 > 101) || (sysdata.Rs1 < 99))
                sysdata.Rs1 = 100;
        if((sysdata.Rs2 > 1010) || (sysdata.Rs2 < 990))
                sysdata.Rs2 = 1000;
    if(IS_BORE_MODE)
    {
        if(IS_THERM_MODE)
                sysdata.mode = MODE_TEMP | MODE_BORE;
    }
    ValidatePrbType();

}

uchar nav1v = 1;
void navto120mv()
{
   nav1v = 0;
   nav_command(NAV_120MV);
   sleepms(200*ONEMS);
}

void navto1v()
{
    nav1v = 1;
    nav_command(NAV_1V);
    sleepms(200*ONEMS);
}
double mabs(double val)
{
        if(val < 0)
                return -val;
        else
                return val;
}
u8 ToByte(char l, char h)
{
	h = (h > 'A')?(h-'A'+0x0A):(h-'0');
	l = (l > 'A')?(l-'A'+0x0A):(l-'0');
	return (h<<4)|l;
}
void update_display()
{
        double v;
        if(sysdata.mode & MODE_TEMP)
        {
                v = rundata.temperature[ch_to_search];
                if((sysdata.chan == CHAN_MINUS))
                {
                        if(ch_to_search==0)
                                return;
                        v = v - rundata.temperature[0];
                }
                if(sysdata.fcmode == 0) //F case
                        v = (v*9.0+32*5.0)/5.0;
                myftoa(v);
                digi_print(databuf,0,11);
                return;
        }else
        {
                v = rundata.reading[ch_to_search];
                if((sysdata.chan == CHAN_MINUS))
                {
                        if(ch_to_search==0)
                            return;

                        v = v - rundata.reading[0];
                }
                myftoa(v);
                digi_print(databuf,0,11);
                return;
        }
}


double pickfrom5()
{
        double v,sum,max,min; //use the average of middle 3
        uchar i;
        max = -9999999;
        min  = 9999999;
        sum = 0;
        for(i = 0; i < 5; i++)
        {
                v = nav_read();
                if(max < v)
                        max = v;
                if(min > v)
                        min = v;
                sum = sum + v;
        }
        sum = sum - max - min;
        return sum/3.0;

}
u8 phase=0;
u16 dlg_cnt = 0;
u16 onesec_cnt = 0;
//state machine of therm type
//phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
//phase 1:get the reading and update the display
//phase 2: move to next channel
uchar therm_state()
{
        uchar i;
	if(phase > 2)
	        phase = 0;

	if(phase == 0)
	{
	        update_led(1);
		if( (sysdata.type_therm >= PRBTYPE_K) && (sysdata.type_therm <= PRBTYPE_R))
		{
			scanner_set_channel(ch_to_search+1);
			dlg_cnt = 6*ONESEC;
			onesec_cnt = 0;
			phase = 1;
			return 0;
		}else{
			rundata.temperature[ch_to_search] = -9999; //invalid data
			phase = 2;
		}
	}
	if(phase == 1)
	{
		rundata.reading[ch_to_search] = pickfrom5();//nav_read();//-sysdata.V0;
                //sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.reading[ch_to_search]); //no work
                //prints(strbuf,strlen(strbuf),PORT_PC);

		rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], sysdata.type_therm);

                //sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
                //prints(strbuf,strlen(strbuf),PORT_PC);
		phase = 2;
	}
	if(phase == 2)
	{
		update_display();
		phase = 0;
	}
	return 1;       //one reading is updated
}
//state machine of bore measure
//phase0 the search the current channel and switch to it if apply, switch to rs, pktt, add delay, if not ktt to phase 2 else to phase 1, otherwise to phase n
//phase1 get reading of rs+, set to nktt, add delay
//phase2 get reading of rs-, switch to rx, add delay, if  ktt to phase 3 else to phase 4
//phase3 get reading of rx-, set to pktt, add delay
//phase4 get reading of rx+, switch to rx, caculate
//phase5 update display and move to next channel
void safesys()
{
SET_PKTT;
SET_TORX;
SET_TO1MA;
SET_TOCH1;
}
double valuep;
u8 databuf[12];
uchar bore_state()
{
        uchar i;
	if(phase > 5)
	        phase = 0;
        i = sysdata.type_bore;
	if(phase == 0)
	{
	        update_led(1);
		if((i <= PRBTYPE_PT1000) && (i >= PRBTYPE_PT100))
		{
			scanner_set_channel(ch_to_search+1);
			SET_PKTT;
			SET_TORS;

			if(i == PRBTYPE_PT100)
			{
			        SET_TOPT100;
			        SET_TO1MA;
			}
			if(i == PRBTYPE_PT25)
			{
			        SET_TOPT100;
			        SET_TO1MA;
			}
			if(i == PRBTYPE_PT1000)
			{
			        SET_TOP1MA;
       			        SET_TOPT1000;
			}
			dlg_cnt =  ONESEC * sysdata.ktime;
			onesec_cnt = 0;
			if(IS_MODE_KTT)
				phase = 1;
			else
				phase = 2;
       			return 0;
		}else{
			rundata.temperature[ch_to_search] = -9999;
			phase = 5;
		}

	}
	if(phase == 1)  //-stdV in ktt mode
	{
		rundata.stdV = mabs(pickfrom5());
		SET_NKTT;
		dlg_cnt =  ONESEC * sysdata.ktime;
		onesec_cnt = 0;
		phase = 2;
		return 0;
	}
	if(phase == 2) //final stdV
	{
		if(IS_MODE_KTT)
		{
			rundata.stdV = (rundata.stdV + mabs(pickfrom5()));
			phase = 3;
		}else{
			rundata.stdV = mabs(pickfrom5());
			phase = 4;
		}
		SET_TORX;

		dlg_cnt = ONESEC * sysdata.ktime;
		onesec_cnt = 0;
		return 0;
	}
	if(phase == 3)  //-V on rx in ktt mode
	{
		valuep = mabs(pickfrom5());
		SET_PKTT;
		dlg_cnt = ONESEC * sysdata.ktime;
		onesec_cnt = 0;
		phase = 4;
		return 0;
	}
	if(phase == 4) // final V on rx
	{
		if(IS_MODE_KTT){
			valuep = (valuep + mabs(pickfrom5()));
/* auto switch voltage range
			if((valuep > 0.21) && (valuep < 2) && (nav1v == 0))
			{
        		        navto1v();
			}
			if((valuep < 0.21) && (valuep > 0.0002) && (nav1v == 1))
			{
			        navto120mv();
			}
*/
		}else{
			valuep = mabs(nav_read());
/* auto switch voltage range
			if((valuep > 0.105) && (valuep < 1) && (nav1v == 0))
			{
        		        navto1v();
			}
			if((valuep < 0.105) && (valuep > 0.0001) && (nav1v == 1))
			{
			        navto120mv();
			}
*/
		}
		if(rundata.stdV != 0)
		{
                        if(i == PRBTYPE_PT1000)
              			rundata.reading[ch_to_search] = valuep*sysdata.Rs2/rundata.stdV - sysdata.R0;
                        else
        			rundata.reading[ch_to_search] = valuep*sysdata.Rs1/rundata.stdV - sysdata.R0;
			if(rundata.reading[ch_to_search] > 0)
			{
                                //sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.Rx);
                                //prints(strbuf,strlen(strbuf),PORT_PC);

			        rundata.temperature[ch_to_search] = RValueToTValue(rundata.reading[ch_to_search],i,sysdata.type_bore);
                                rundata.Rx = rundata.reading[ch_to_search];
                                //sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
                                //prints(strbuf,strlen(strbuf),PORT_PC);
			}
		}else{
		        rundata.temperature[ch_to_search] = -9999;
		}
		phase = 5;
	}
	if(phase == 5) //got one reading
	{
		update_display();//pgmain_handler(MSG_REFRESH);
		phase = 0;
	}
	return 1;
}

void reset_counter()
{
	dlg_cnt = 0;
       	onesec_cnt = 0;
        phase = 0;      //reset the state machine
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
      // Initialize SPI to communicate with CH453
   CH453_I2C_Init();

   // Initialize Global Variable:
   // "Init_Vars()" has to be placed after "bReadDataFromEeprom()" because
   Init_Vars();

   // Initialize UART, 8 data, 1 stop, no parity. Boardrate is 2400.
   UART_Init();
      // Timer (Interrupt) initialization
   safesys();
   digi_cprint("88888888",0,8);


   init_digi_Led();
   Init_Timers();
   // initialize golbal interrupt.
   #asm("sei")

    //init the DMM
    nav_command(NAV_INIT);
    sleepms(200*ONEMS);
    navto1v();
    nav_command(NAV_SLOWMODE);
    sleepms(200*ONEMS);
    nav_command(NAV_AFLTON);
    sleepms(200*ONEMS);

    update_led(0);


    sysdata.R0 = 0;
	 while(1)
	 {
	        if(keycode != KEY_INVALID)
	        {
                        HandleKey(keycode);
                        keycode = KEY_INVALID;
                        continue;
                }

		if(dlg_cnt > 1)
		{
       			dlg_cnt--;
			continue;
		}
                timer_tick();
		if(IS_THERM_MODE)
		{
        	        if(therm_state() == 0)
		                continue;
                }
		if(IS_BORE_MODE)
		{
	        	if(bore_state() == 0)
		                continue;
		}

                //shift to next channel

                if(sysdata.chan == CHAN_1)
                       ch_to_search = 0;
                else if(sysdata.chan == CHAN_2)
                       ch_to_search = 1;
                else
                     ch_to_search = 1 - ch_to_search&0x01;
        }
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

// #ifndef _OLD_FASHION_CMD_PROCESS_
// u8 volatile rx_buffer[RX_BUFFER_SIZE];
// define index for Receiver
u8 volatile rx_wr_index;
u8 volatile rx_rd_index;
u8 volatile rx_counter;
// #endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;


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
        //todo process bytes
        nav_uart_push(data);
   }
}

/****************************************************************************/
//               UART Transmitter Interrupt service routine
/****************************************************************************/
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter)
   {
//      PORTB.0 = 1;         // set RS485 node to transmiter mode
      --tx_counter;
      UDR=tx_buffer[tx_rd_index];
      if (++tx_rd_index == TX_BUFFER_SIZE)
         tx_rd_index=0;
   }
//else
//   PORTB.0 = 0;         // set RS485 node to receiver mode.
}

/****************************************************************************/
//                     UART Set Baud rate.
// This subroutine sets UART baud rate based on user settings
// Users are allowed to change baudrate of RS485 nodes.
/****************************************************************************/
void SetBaudrate()
{
u8 baud = 0x01;
        UBRRH=0x00;
        //UBRRL= 0x2F; //9600
        UBRRL=191; //2400
        /*

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
   */
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
 // UCSRC=0xA6;    // EVEN parity bit
    UCSRC=0x86;    // No parity bit

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
   //PORTB.0 = 1;

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
/*
interrupt [EE_RDY] void vIvEeReady(void)
{
    if(!fgEepromBufEmpty()){
        while(EECR & (1<<EEWE));  // wait for last write to complete
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
*/
/************************************************************************/
//                     Write data into EEPROM buffer
// If buffer is not full, write data into buffer, return RET_SUCCESS(0x0)
// flag. If buffer is full, return RET_BUSY(0xff) flag
/************************************************************************/
/*
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

*/
/************************************************************************/
//                         Read data from EEPROM
// The programer must ensure EEPROM is NOT in writing state before reading
// EEPROM. This subroutine return RET_BUSY(0xff) flag when EEPROM is in
// writting state. Return RET_SUCCESS(0x0) flag if reading successfully
// Data readout is saved into read buffer.
/************************************************************************/
/*
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
*/
/************************************************************************/
//                        Initialize EEPROM
// Intialize buffer variables. Diable EEPROM interruption
// EEPROM interrupt is enabled after writing data to EEPROM buffer.
/************************************************************************/
/*
void vInitEeprom(void)
{
    EECR = 0x00;
    _bEepromBufWrPtr = 0;
    _bEepromBufRdPtr = 0;
    _bEepromBufNs    = 0;
}
*/

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
double myatoi(eeprom u8* str)
{
        return 0.0;
}
double RValueToTValue(double r, u8 prbid,u8 type)
{
	double ac,bc,cc,tlow,tup,rnew,tnew;
	int count;

	ac = myatoi(rprbdata.param1[prbid]);
	bc = myatoi(rprbdata.param2[prbid]);
	cc = myatoi(rprbdata.param3[prbid]);

	if(type == PRBTYPE_PT100)
		return PT100RToTValue(r, cc);
	if(type == PRBTYPE_PT1000)
		return PT100RToTValue(r, cc);
	if(type != PRBTYPE_PT25)
		return -9999.999;
        if(myatoi(rprbdata.rtp[prbid]) > 0.1)
                r = r/myatoi(rprbdata.rtp[prbid]);
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
//#include "stdlib.h"
/*---------------—” ±◊”≥Ã–Ú----------------*/
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
#include "define.h"
#include "utili.h"

#define DIGI_NUMBER     8

/*
*       Every digital structure looks like this
*       has_point(1bit) flash(1bit) reserver(1bit)   data(5 bits)
*
*/
#define CHAR_CODE_MASK  0x1F//0x70
#define FUNC_CODE_MASK  0x40
#define POINT_ON        0x80
#define POINT_OFF       0x7F
#define FLASH_ON        0x40
#define FLASH_OFF       0xBF
#define CHAR_0          0x00     //char 0 to char 9 is 0-9
#define CHAR_A          0x0a     //A
#define CHAR_b          0x0b     //b
#define CHAR_c          0x0c     //c

#define CHAR_d          0x0d     //d
#define CHAR_E          0x0e     //E
#define CHAR_F          0x0f     //F
#define CHAR_r          0x10     //r
#define CHAR_o          0x11     //o
#define CHAR_t          0x12     //t
#define CHAR_n          0x13     //n
#define CHAR_L		0x14	 //L
#define CHAR_Y		0x15	 //Y
#define CHAR_h		0x16	 //h
#define CHAR_U		0x17	 //U
#define CHAR_N		0x18	 //N
#define CHAR_SPACE      0x19     //
#define CHAR_MINUS      0x1A     //-
#define CHAR_BC         0x1B     //C
#define CHAR_MAX_NUM	0x1C

// *******************
// CH453µƒ∂®“Â
// *******************
#define CH453_DIG0      0x6000                      // ˝¬Îπ‹Œª0œ‘ æ
#define CH453_DIG1      0x6200		            // ˝¬Îπ‹Œª1œ‘ æ
#define CH453_DIG2      0x6400		            // ˝¬Îπ‹Œª2œ‘ æ
#define CH453_DIG3      0x6600		            // ˝¬Îπ‹Œª3œ‘ æ
#define CH453_DIG4      0x6800		            // ˝¬Îπ‹Œª4œ‘ æ
#define CH453_DIG5      0x6A00	                    // ˝¬Îπ‹Œª5œ‘ æ
#define CH453_DIG6      0x6C00	                    // ˝¬Îπ‹Œª6œ‘ æ
#define CH453_DIG7      0x6E00		            // ˝¬Îπ‹Œª7œ‘ æ

#define CH453_LEDA		0x7800		    // ˝¬Îπ‹Œª12 for led
#define CH453_LEDB		0x7A00		    // ˝¬Îπ‹Œª13 for led


static u8 digi_data[DIGI_NUMBER] = {0,0,0,0,0,0,0,0}; //8 digital LEDs

static flash u8 digi_code[CHAR_MAX_NUM] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,\
                                                0x77,0x7C,0x58,0x5E,0x79,0x71,0x50,\
                                                0x5C,0x78,0x54,0x38,0x6E,0x74,0x3E,0x37,0x00,0x40,0x39};
u16 reg_led;             //16 leds on off status
static u16 reg_led_flash;       //16 leds flash status
u8 flash_cnt = 0;        //counter of timer for flash

extern void Led_Output(u16);
extern void Digi_Output(u8,u8);
extern void CH453_Write( u16 );
void digi_print(u8 *str,uchar startpos, uchar size);
extern u8 busy;
u8 needrefresh=0;
void add_refresh()
{
   needrefresh = 2;
   timer_tick();
}

u8 fl_onoff = 0; //current on off status;
void timer_tick()
{
	u8 i,j,fl_cnt;
	i = 0;
	fl_cnt = flash_cnt;

	if(needrefresh == 0)
        	return;

        if(needrefresh == 2)     //direct output
                fl_cnt = 1;
        else{
                if((fl_onoff == 0) && (fl_cnt <=5)) //need on
                {
                        fl_cnt = 1;
                        fl_onoff = 1;
                }else{
                        if((fl_onoff == 1) && (fl_cnt >5)) //need off
                        {
                                fl_cnt = 6;
                                fl_onoff = 0;
                        }else{
                                return; //no action required
                        }
                }
        }

        if(fl_cnt != 1 && fl_cnt != 6) //1: on 6: off
                return;

        needrefresh = 0;
        //only 1 and 6 can come here
	if(fl_cnt == 1)
	{
		Led_Output(reg_led | reg_led_flash);
	}else{
		Led_Output(reg_led ^ reg_led_flash);
	}
        if(reg_led_flash != 0)
                needrefresh = 1;

	for(i=0;i<DIGI_NUMBER;i++)
	{
	        j = digi_data[i];
		if(digi_data[i] & FLASH_ON)
		{
		        needrefresh = 1;
			if(fl_cnt == 1)
			{
				Digi_Output(i,digi_code[CHAR_CODE_MASK & digi_data[i]] | (digi_data[i] & POINT_ON));
			}else{
				Digi_Output(i,digi_code[CHAR_SPACE] | (digi_data[i] & POINT_ON));
			}
		}else{
		        j = digi_code[CHAR_CODE_MASK & digi_data[i]];
		        j = j | (digi_data[i] & POINT_ON);
		        Digi_Output(i,j);
		}
	}
}
void Digi_Output(u8 pos, u8 val)
{
        while(busy == 1)
                ;
        busy = 1;
        switch(pos)
        {
                case 4: CH453_Write(CH453_DIG0 | val); break;
                case 5: CH453_Write(CH453_DIG1 | val); break;
                case 6: CH453_Write(CH453_DIG2 | val); break;
                case 7: CH453_Write(CH453_DIG3 | val); break;
                case 0: CH453_Write(CH453_DIG4 | val); break;
                case 1: CH453_Write(CH453_DIG5 | val); break;
                case 2: CH453_Write(CH453_DIG6 | val); break;
                case 3: CH453_Write(CH453_DIG7 | val); break;
                default: break;
        }
        busy = 0;
}
void init_digi_Led()
{
    reg_led = 0;
    reg_led_flash = 0;
}
void led_on(u16 key, bool on)
{

	if(on){
		reg_led |= key;
	}else{
		reg_led = reg_led & (key ^ 0xffff);
	}
	add_refresh();
}
void led_flash(u16 key, bool on)
{

	if(on){
		reg_led_flash |= key;
	}else{
		reg_led_flash = reg_led_flash & (key ^ 0xffff);
	}
	add_refresh();
}
void digi_flash(uchar startpos, uchar endpos, bool on)
{
        u8 i;
        for(i=0;i<DIGI_NUMBER;i++)
        {
                digi_data[i] = digi_data[i] & FLASH_OFF;
        }
	while(startpos <= endpos)
	{
		if(on)
			digi_data[startpos] |= FLASH_ON;
		else
			digi_data[startpos] = digi_data[startpos] & FLASH_OFF;
		startpos++;
	}
	add_refresh();
}
void digi_print(u8 *str,uchar startpos, uchar size)
{
   u8 i;
   u8 endpos;
   endpos = startpos + size;
   if(endpos > DIGI_NUMBER)
        endpos = DIGI_NUMBER;

   for(i=startpos; i < endpos; i++){
      switch(*str){
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9': digi_data[i] = (digi_data[i] & (FUNC_CODE_MASK)) | (*str -'0'); break;
         case 'A': digi_data[i] = CHAR_A; break;
         case 'b': digi_data[i] = CHAR_b; break;
         case 'c': digi_data[i] = CHAR_c; break;
         case 'd': digi_data[i] = CHAR_d; break;
         case 'E': digi_data[i] = CHAR_E; break;
         case 'F': digi_data[i] = CHAR_F; break;
         case 'r': digi_data[i] = CHAR_r; break;
         case 'o': digi_data[i] = CHAR_o; break;
         case 't': digi_data[i] = CHAR_t; break;
         case 'n': digi_data[i] = CHAR_n; break;
         case 'L': digi_data[i] = CHAR_L; break;
         case 'Y': digi_data[i] = CHAR_Y; break;
         case 'U': digi_data[i] = CHAR_U; break;
         case 'h': digi_data[i] = CHAR_h; break;
         case 'N': digi_data[i] = CHAR_N; break;
         case 'C': digi_data[i] = CHAR_BC; break;
         case '-': digi_data[i] = CHAR_MINUS; break;
         case '.':  digi_data[i-1] |= POINT_ON;
               i--;
               break;
         default: digi_data[i] = CHAR_SPACE; //no show
               break;
         }
      str++;
   }
   add_refresh();
}
//const string print
void digi_cprint(u8 flash *str,uchar startpos, uchar size)
{
   u8 i;
   u8 endpos;
   endpos = startpos + size;
   if(endpos > DIGI_NUMBER)
        endpos = DIGI_NUMBER;
   for(i=startpos; i < endpos; i++){
      switch(*str){
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9': digi_data[i] = (digi_data[i] & (FUNC_CODE_MASK)) | (*str -'0'); break;
         case 'A': digi_data[i] = CHAR_A; break;
         case 'b': digi_data[i] = CHAR_b; break;
         case 'c': digi_data[i] = CHAR_c; break;
         case 'd': digi_data[i] = CHAR_d; break;
         case 'E': digi_data[i] = CHAR_E; break;
         case 'F': digi_data[i] = CHAR_F; break;
         case 'r': digi_data[i] = CHAR_r; break;
         case 'o': digi_data[i] = CHAR_o; break;
         case 't': digi_data[i] = CHAR_t; break;
         case 'n': digi_data[i] = CHAR_n; break;
         case 'L': digi_data[i] = CHAR_L; break;
         case 'Y': digi_data[i] = CHAR_Y; break;
         case 'U': digi_data[i] = CHAR_U; break;
         case 'h': digi_data[i] = CHAR_h; break;
         case 'N': digi_data[i] = CHAR_N; break;
         case 'C': digi_data[i] = CHAR_BC; break;
         case '-': digi_data[i] = CHAR_MINUS; break;
         case '.':  digi_data[i-1] |= POINT_ON;
               i--;
               break;
         default: digi_data[i] = CHAR_SPACE; //no show
               break;
         }
      str++;
   }
   add_refresh();
}


void update_led(u8 mode)
{
        //mode 0 is for fix   sysdata.chan
        //mode 1 is for flash ch_to_search
        //D1,D2 bore, therm, D3, temp/raw
        /*D4 -- D13 PRBTYPE_PT100	        0x00
                    PRBTYPE_PT25	0x01
                    PRBTYPE_PT1000  0x02
                PRBTYPE_K 	   	0x03
                PRBTYPE_N		0x04
                PRBTYPE_E		0x05
                PRBTYPE_B		0x06
                PRBTYPE_J		0x07
                PRBTYPE_S		0x08
                PRBTYPE_T		0x09
                PRBTYPE_R		0x0A
        */
        //D14,D15, ch1, ch2, minus
        u16 dat = 0;
        u8 ch = 0;
        ch = ch_to_search;
        if(sysdata.mode & MODE_TEMP)
                dat = dat + LED_TEMP;
        if(IS_BORE_MODE)
        {
                dat = dat + LED_BORE;
                if(sysdata.type_bore == PRBTYPE_PT100)
                        dat = dat + LED_PT100;
                if(sysdata.type_bore == PRBTYPE_PT1000)
                        dat = dat + LED_PT1000;
        }
        if(IS_THERM_MODE)
        {
                dat = dat + LED_THERM;
                if(sysdata.type_therm == PRBTYPE_K)
                        dat = dat + LED_K;
                if(sysdata.type_therm == PRBTYPE_N)
                        dat = dat + LED_N;
                if(sysdata.type_therm == PRBTYPE_E)
                        dat = dat + LED_E;
                if(sysdata.type_therm == PRBTYPE_B)
                        dat = dat + LED_B;
                if(sysdata.type_therm == PRBTYPE_J)
                        dat = dat + LED_J;
                if(sysdata.type_therm == PRBTYPE_S)
                        dat = dat + LED_S;
                if(sysdata.type_therm == PRBTYPE_T)
                        dat = dat + LED_T;
                if(sysdata.type_therm == PRBTYPE_R)
                        dat = dat + LED_R;
        }

        if(sysdata.chan == CHAN_1 || sysdata.chan == CHAN_ALL)
        {
                dat = dat + LED_CH1;
        }
        if(sysdata.chan == CHAN_2 || sysdata.chan == CHAN_ALL)
                dat = dat + LED_CH2;
        if(mode == 0)  //fix channel led
        {
                reg_led = dat;
                add_refresh();
                return;
        }
        //flash mode
        if(ch == 0)
        {
                dat = dat | LED_CH1;
        }
        if(ch == 1)
        {
                dat = dat | LED_CH2;
        }
        reg_led = dat;
        add_refresh();

}
void Led_Output(u16 data)
{
        u8 key;
        while(busy == 1)
                ;
        busy = 1;

        key = (u8)(data & 0x00ff);
        CH453_Write(CH453_LEDA | key);
        key = (u8)(data >> 8);
        CH453_Write(CH453_LEDB | key);
        busy = 0;
}
#include "define.h"
#include "mega16.h"
//ATMEGA16 PC0   -------    CH452 SCL
//ATMEGA16 PC1   -------    CH452 SDA

#define TWINT	7
#define TWEA	6
#define TWSTA 	5
#define TWSTO	4
#define TWWC	3
#define TWEN	2
#define TWIE	0

#define TWPS0	0
#define TWPS1	1

#define PC0     0
#define PC1     1

#define  SLA_W    			0x32
#define  SLA_R    			0x33
#define  SLAVER_ADDRESS     0x32
#define  START              0x08
#define  RE_START           0x10
#define  MT_SLA_WRITE_ACK   0x18
#define  MT_SLA_READ_ACK    0x40
#define  MT_DATA_ACK        0x28
#define  MT_READ_ACK        0x58



#define CMD_CFG_SYSTEM		0x4843					//0x4800| 01000011[SLEEP][INTENS]0[X_INT]0[KEYB][DISP]B
#define CMD_READ_KEY		0x4F00

#define CH453_START()  			TWCR = ( 1<<TWEN )|(1<<TWSTA )|(1<<TWINT); while( !(TWCR & (1<<TWINT)) )//∑¢ START
#define CH453_WAIT()	 		while( !(TWCR & (1<<TWINT)) )
#define CH453_STOP()			TWCR = ( 1<<TWEN )|(1<<TWSTO)|(1<<TWINT)
#define CH453_Wr_Byte(twi_d)	{TWDR=(twi_d);TWCR=(1<<TWINT)|(1<<TWEN); while( !(TWCR & (1<<TWINT)) );}
#define CH453_Rd_Byte(twi_d)	{TWCR=(1<<TWINT)|(1<<TWEN); while( !(TWCR & (1<<TWINT)) ); twi_d = TWDR;}
#define TestACK()		 		(TWSR&0xF8)    		//»°≥ˆ◊¥Ã¨¬Î
#define SetACK() 				(TWCR|=(1<<TWEA))   //≤˙…˙ACK
// **************************************
// —” ±∫¡√Î,≤ª◊º
// **************************************
void mDelaymS( unsigned char ms )
{
    unsigned short i;
    while ( ms -- )
    {
        for( i = 0; i != 1000; i++ );
    }
}


// **************************************
// I2C–¥CH452√¸¡Ó
// **************************************
void CH453_Write( u16 cmd )
{
     u8 try;//÷ÿ ‘¥Œ ˝

	 try = 50;
	 do
	 {
		 CH453_START();

   	     CH453_Wr_Byte((u8)(cmd>>8));    //command

	 	 if( TestACK() == MT_SLA_WRITE_ACK )// ’µΩACK
	 	 {
	  	  	 CH453_Wr_Byte( (u8)cmd );
			 CH453_STOP();
			 break;
	 	 }

		 CH453_STOP();

		 if( try ) try--;
	 } while( try );

}

// **************************************
// I2C∂¡CH452√¸¡Ó
// **************************************
u8 CH453_Read( u16 read_cmd )
{
    u8 try;//÷ÿ ‘¥Œ ˝
	u8 val;

	try = 5;
	do
	{
	  	val = 0xFF;

   	  	CH453_START();

   	    CH453_Wr_Byte((u8)(read_cmd>>8));

//                sleepms(10);
		if( TestACK() == MT_SLA_READ_ACK )// ’µΩACK
   		{
			CH453_Rd_Byte(val);

			CH453_STOP();
			break;
    	        }

		CH453_STOP();

		if( try ) try--;
 	}while( try );
	return val;
}
// **************************************
// CH453≥ı ºªØ
// **************************************
void CH453_I2C_Init( void )
{
 	 PORTC |= (1<<PC1)|(1<<PC0);
	 DDRC |= (1<<PC1)|(1<<PC0);
   	 TWBR = 0x64;
	 TWSR = (1<<TWPS1)|(1<<TWPS0);
   	 TWCR = ( 1<<TWEN ); // πƒ‹TWI
	 mDelaymS(100);
	 CH453_Write(CMD_CFG_SYSTEM);
}

static u8 lastkey = 0xff;
u8 CH453_ReadKey()
{
	u8 key;
	key = CH453_Read(CMD_READ_KEY);
	if(key == 0xff)
		return 0xff;
	if((key & 0x40)) //key down
	{
		lastkey = key & 0x0f;
		return 0xff;
	}else{
	        if(lastkey != 0xff)
        	{	//key up
	        	key = lastkey;
        		lastkey = 0xff;
		        return key;
	        }
	}
	return 0xff;
}
#include "define.h"
#include "scanner.h"
#include "utili.h"
#include "stdlib.h"
extern PRBDATA eeprom  rprbdata;
void memcpy1(eeprom u8 *src, u8 *dst, u8 size)
{
        while(size-- > 0)
        {
                *dst++ = *src++;
        }
}
void memcpy2( u8 *src, eeprom u8 *dst, u8 size)
{
        while(size-- > 0)
        {
                *dst++ = *src++;
        }
}
double buf2double(u8 *buf)
{
        return 0.1;
}
void ValidatePrbType()
{
	if((sysdata.type_bore > PRBTYPE_PT1000))
		sysdata.type_bore = PRBTYPE_PT100;
        if((sysdata.type_therm < PRBTYPE_K) || (sysdata.type_therm > PRBTYPE_R))
		sysdata.type_therm = PRBTYPE_K;


}
u8 menu_index = 0xff; //not in menu mode
extern void reset_counter();
extern void EnterMenu();

void HandleKey(u8 key)
{
	if(key == 0xff)
		return;
	if(key == KEY_MODE) //display temperature of direct reading
	{
	        if(IS_BORE_MODE)
	        {
        	        sysdata.mode = (sysdata.mode & MODE_TEMP) | MODE_THERM;
	        }else{
               	        sysdata.mode = (sysdata.mode & MODE_TEMP) | MODE_BORE;
	        }
	        digi_cprint("   . . . . ",0,8);
	        update_led(0);
	        phase = 0;
                safesys();
		return;
	}
	if(key == KEY_PROBE) //change to next probe type
	{
	        if(IS_BORE_MODE)
	        {
	                sysdata.type_bore += 1;
	        }
	        if(IS_THERM_MODE)
	        {
	                sysdata.type_therm += 1;
	        }
	        digi_cprint("   . . . . ",0,8);
	        ValidatePrbType();
	        update_led(0);
	        return;
	}

	if(key == KEY_ZERO)
	{
		if(IS_BORE_MODE){
		        sysdata.R0 = 0;// rundata.Rx;
		}else{
			sysdata.V0 = nav_read();
		        nav_command(NAV_ZEROON);
		        sleepms(1000*ONEMS);
		}
		digi_cprint("  doNE   ",0,8);
		update_display();
		return;
	}
	if(key == KEY_DIGI) //show temp or not
	{
	        sysdata.mode = sysdata.mode ^ MODE_TEMP;
		ValidatePrbType();
		update_led(0);
		update_display();
		return;
	}
	if(key == KEY_CHAN)
	{
		sysdata.chan++;
		if(sysdata.chan > CHAN_MINUS)
			sysdata.chan = CHAN_1;
		if(sysdata.chan == CHAN_1)
		        digi_cprint("  chAn 1",0,8);
		else if(sysdata.chan == CHAN_2)
		        digi_cprint("  chAn 2",0,8);
		else if(sysdata.chan == CHAN_MINUS)
		        digi_cprint("  chAn -",0,8);
		else
       		        digi_cprint(" chAn 1.2",0,8);
		update_led(0);
		return;
	}
	if(key == KEY_KTT)
	{
		if(IS_BORE_MODE)
		{
			sysdata.kttmode = sysdata.kttmode ^ 0x01;
                        SET_TOP1MA;
	                SET_TOPT1000;
			SET_PKTT;
			if(sysdata.kttmode & 0x01)
				digi_cprint("dUAL  0N",0,8);
			else
				digi_cprint("dUAL oFF",0,8);
                        reset_counter();
		}
		return;
	}
	if(key == KEY_OK)
	{
		EnterMenu();
		digi_flash(0,7,0);
	}
}
void ValidateDataBuf()
{//need format 1234.5678
        u8 j=0;
        for(j=0;j<9;j++)
        {
                if(j == 4)
                        continue;
                if(databuf[j] <= '9' && databuf[j] >= '0')
                        continue;
                databuf[j] = '0';
        }
	databuf[4] = '.';
	databuf[9] = 0x00;

}
void ValidateDataBuf2()
{//need format +1.234E-3
        u8 j=0;
        for(j=0;j<9;j++)
        {
                if((j==0) || (j==7))
                {
                     if((databuf[j] != '-') && (databuf[j] != ' '))
                        databuf[j] = ' ';
                     continue;
                }
                if(databuf[j] > '9' || databuf[j] < '0')
                        databuf[j] = '0';
        }
     databuf[2] = '.';
     databuf[6] = 'E';
     databuf[9] = 0x00;
}
#define MAX_MENU_INDEX	10	//total menu rows index     0:dual, 1:unit, 2:dly, 3,4,5,6:r1,a1,b1,c1, 7,8,9,10:r2,a2,b2,c2
extern u8 databuf[12];
extern u8 keycode;
void EnterMenu()
{	//mode, probe, zero(left), digi(right), chan(up), ktt(down), key_ok
	//up, 	 down is shift the menu index, and modify the digital
	//right, left is change the position
	//ok :	enter the menu, exit the menu, and start to modify
	u8 key;
	u8 inflash = 0xff; //flash position
	double v;
	if(keycode != KEY_OK)
		return;
	menu_index = 1;
	key = KEY_INVALID;
	keycode = KEY_INVALID;
	while(1)
	{
/*
		if(menu_index == 0)	//dUAL	ON|OFF
		{
		        if(key == KEY_OK)
		        {
		                if(inflash == 0xff)
		                        inflash = 1;
		                else
		                        inflash = 0xff;
			}
			if((inflash == 1) && ((key == KEY_UP) || (key == KEY_DN)))
					sysdata.kttmode	= 1 - sysdata.kttmode & 0x01;


			if(inflash == 1)
				digi_flash(5,7,1);
			else
				digi_flash(0,7,0);
			key = 0xff;
		}
*/
		if(menu_index == 1) //UN1t   F|C
		{
		        if(key == KEY_OK)
		        {
		                if(inflash == 0xff)
		                        inflash = 1;
		                else
		                        inflash = 0xff;
			}

			if((inflash == 1) && ((key == KEY_UP) || (key == KEY_DN)))
					sysdata.fcmode	= 1 - sysdata.fcmode&0x01;

			if(sysdata.fcmode == 0)
				digi_cprint("UN1t   F",0,8);
			else
				digi_cprint("UN1t   C",0,8);
			if(inflash == 1)
				digi_flash(7,7,1);
			else
				digi_flash(7,7,0);
			key = 0xff;
		}
		if(menu_index == 2) //dLY   00
		{
			if(key == KEY_OK)
			{
				if(inflash == 0xff)
					inflash = 6;
				else
					inflash = 0xff;
			}
			if((inflash == 6) && (key == KEY_RIGHT))
				inflash = 7;
			if((inflash == 7) && (key == KEY_LEFT))
				inflash = 6;
			if((inflash == 6) && (key == KEY_DN))
			{
				if(sysdata.ktime >= 10)
					sysdata.ktime -= 10;
				else
					sysdata.ktime += 90;
			}
			if((inflash == 6) && (key == KEY_UP))
			{
				if(sysdata.ktime < 90)
					sysdata.ktime += 10;
				else
					sysdata.ktime -= 90;
			}
			if((inflash == 7) && (key == KEY_DN))
			{
				if((sysdata.ktime % 10) != 9)
					sysdata.ktime += 1;
				else
					sysdata.ktime -= 9;
			}
			if((inflash == 7) && (key == KEY_UP))
			{
				if((sysdata.ktime % 10) != 0)
					sysdata.ktime -= 1;
				else
					sysdata.ktime += 9;
			}
			if(sysdata.ktime >= 100)
			        sysdata.ktime = 0;
			sprintf(databuf,"dLY   %d%d", (sysdata.ktime/10),(sysdata.ktime%10));
			digi_print(databuf,0,8);

			if(inflash != 0xff)
				digi_flash(inflash,inflash,1);
			else
			        digi_flash(0,7,0);
			key = 0xff;
		}
		if((menu_index == 3) || (menu_index == 7))//1r, 2r  ....   rvalue for channel 1 and channel 2
		{       //1234.5678
			if(key == KEY_OK)
			{
				if(inflash == 0xff)
				{
					inflash = (menu_index-3)/4;
					memcpy1(rprbdata.rtp[inflash],databuf,4);
					memcpy1(4+rprbdata.rtp[inflash],databuf+5,4);
					ValidateDataBuf();
					inflash = 0;
				}else{
					ValidateDataBuf();
					inflash = (menu_index-3)/4;
					memcpy2(databuf,rprbdata.rtp[inflash],4);
					memcpy2(databuf+5,rprbdata.rtp[inflash]+4,4);
					rundata.rtp[inflash] = atof(databuf);//buf2double(databuf); //
					inflash = 0xff;
				}
			}
			if((key == KEY_DN) && (inflash != 0xff))
			{
			        if(inflash >= 4)
			                inflash += 1;
				if(databuf[inflash] == '0')
					databuf[inflash] = '9';
				else
					databuf[inflash] = databuf[inflash]-1;
		                if(inflash >= 4)
			                inflash -= 1;

			}
			if((key == KEY_UP) && (inflash != 0xff))
			{
			        if(inflash >= 4)
			                inflash += 1;
				if(databuf[inflash] == '9')
					databuf[inflash] = '0';
				else
					databuf[inflash] = databuf[inflash]+1;
		                if(inflash >= 4)
			                inflash -= 1;
			}
			if((key == KEY_LEFT) && (inflash != 0xff))
			{
				if(inflash <= 0)
					inflash = 7;
				else
					inflash--;
				if(inflash == 4)
					inflash = 3;
			}
			if((key == KEY_RIGHT) && (inflash != 0xff))
			{
				if(inflash >= 7)
					inflash = 0;
				else
					inflash++;
			}

			if(inflash != 0xff)
			{
				digi_print(databuf,0,8);
				digi_flash(inflash,inflash,1);
			}else{
				sprintf(databuf, "%cr    . . .",'1' + (menu_index-3)/4);
				digi_print(databuf,0,8);
				digi_flash(0,7,0);
			}
			key = 0xff;
			timer_tick();
		}
		if((menu_index >= 4) && (menu_index  != 7) && (menu_index  < 11))//1AbC  ....     coeffient a,b,c for channel 1 and channel 2, row 4,5,6, 8, 9, 10
		{ //+1.234E-3
			if(key == KEY_OK)
			{
				if(inflash == 0xff)
				{
					inflash = (menu_index-3)/4;
					memcpy1(rprbdata.rtp[inflash],databuf,2);
					memcpy1(2+rprbdata.rtp[inflash],databuf+3,6);
					ValidateDataBuf2();
					inflash = 0;
				}else{
					inflash = (menu_index-3)/4;
					ValidateDataBuf2();
					memcpy2(databuf,rprbdata.rtp[inflash],2);
					memcpy2(databuf+3,rprbdata.rtp[inflash]+2,6);
					v = atof(databuf);//buf2double(databuf); //
					if((menu_index % 4) == 0)
						rundata.param1[inflash] = v;
					if((menu_index % 4) == 1)
						rundata.param2[inflash] = v;
					if((menu_index % 4) == 2)
						rundata.param3[inflash] = v;
					inflash = 0xff;
				}
			}
			if((key == KEY_DN) && (inflash != 0xff))
			{
			        if(inflash >= 2)
			                inflash += 1;
				if(databuf[inflash] == '0')
					databuf[inflash] = '9';
				else if(databuf[inflash] == '-')
					databuf[inflash] = ' ';
				else if(databuf[inflash] == ' ')
					databuf[inflash] = '-';
				else if((databuf[inflash] >='1') && (databuf[inflash] <='9'))
					databuf[inflash] = databuf[inflash]-1;
			        if(inflash >= 2)
			                inflash -= 1;

			}
			if((key == KEY_UP) && (inflash != 0xff))
			{
			        if(inflash >= 2)
			                inflash += 1;

				if(databuf[inflash] == '9')
					databuf[inflash] = '0';
				else if(databuf[inflash] == '-')
					databuf[inflash] = ' ';
				else if(databuf[inflash] == ' ')
					databuf[inflash] = '-';
				else if((databuf[inflash] >='0') && (databuf[inflash] <='8'))
					databuf[inflash] = databuf[inflash]+1;
			        if(inflash >= 2)
			                inflash -= 1;

			}
			if((key == KEY_LEFT) && (inflash != 0xff))
			{
				if(inflash == 0)
					inflash = 7;
				else
					inflash--;
				if(inflash == 5)
					inflash = 4;
			}
			if((key == KEY_RIGHT) && (inflash != 0xff))
			{       inflash++;
				if(inflash == 8)
					inflash = 0;

				if(inflash == 5)
					inflash = 6;
			}

			if(inflash != 0xff)
			{
				digi_print(databuf,0,8);
				digi_flash(inflash,inflash,1);
			}else{
			        key = '1' + (menu_index-3)/4;
				if((menu_index % 4) == 0)
					sprintf(databuf, "%cA    . . .",key);
				if((menu_index % 4) == 1)
					sprintf(databuf, "%cb    . . .",key);
				if((menu_index % 4) == 2)
					sprintf(databuf, "%cc    . . .",key);
				digi_print(databuf,0,8);
				digi_flash(0,7,0);
			}
			key = 0xff;
		}
		while(keycode == 0xff)
		{
		        timer_tick();
			sleepms(1);
		}
	        key = keycode;
	        keycode = KEY_INVALID;
		if((key == KEY_MODE) || (key == KEY_PROBE))
			break;
		if(key == KEY_OK || key == KEY_LEFT || key == KEY_RIGHT)
			continue; //let sub handle it
		if((key == KEY_UP))
		{
        		if(inflash == 0xff)
        		{
		        	if(menu_index == 1)
	        			menu_index = MAX_MENU_INDEX;
        			else
			        	menu_index--;
        		}else{
        		        continue; //let sub handle it
        		}
		}
		if((key == KEY_DN) )
		{
        		if(inflash == 0xff)
                	{
		        	if(menu_index == MAX_MENU_INDEX)
		        	{
		        	        digi_cprint("  doNE  ",0,8);
			        	break;//menu_index = 0;
        			}
        			menu_index++;
	        	}else{
        		        continue; //let sub handle it
        		}
		}
		key = 0xff;
		continue;
	}
	menu_index = 0xff;
}
#include "utili.h"
#include "scanner.h"
#include "stdlib.h"
#include "string.h"
static uchar navlen = 0;
static double reading = -1000;
static char navread[20];
static char navread2[20];
u8 scancmd[5];
u8 eeprom scanner_type = 1; //1: MI, 2: GUIDLINE
extern void prints(u8 *str, u8 length, char uart_port);
void scanner_set_mode()
{
/* select channel
        if (IS_BORE_MODE)
        {
            scancmd[0] = '#';
        }else{
            scancmd[0]= '!';
        }
        scancmd[1] = 0x0D;
        scancmd[2] = 0x0A;
        prints(scancmd,3,PORT_SCANNER);
*/
}
void scanner_set_channel(uchar ch)
{
        if(ch == 1)
        {
                SET_TOCH1;
        }
        if(ch == 2)
        {
                SET_TOCH2;
                return;
        }
/*external switch
        if(scanner_type == 1) //MI
        {
                if(ch < 10)
                {
                        scancmd[0] = (ch + '0');
                        scancmd[1] = 'A';
                        scancmd[2] = 0x0D;
                        scancmd[3] = 0x0A;
                        prints(scancmd,4,PORT_SCANNER);
                        return;
                }else{
                        scancmd[0] = (u8)(ch / 10) + '0';
                        ch = ch % 10;
                        scancmd[1] = ch + '0';
                        scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
                        prints(scancmd,5,PORT_SCANNER);
                }
        }
        if(scanner_type == 2) //guidline
        {
                        scancmd[0] = 'A';
                        scancmd[1] = (u8)(ch / 10) + '0';
                        ch = ch % 10;
                        scancmd[2] = ch + '0';
                        scancmd[3] = 0x0D; scancmd[4] = 0x0A;
                        prints(scancmd,5,PORT_SCANNER);
        }
        */
}


//incoming data handler of scanner
void scanner_uart_push(uchar data)
{
}
//incoming data hander of PC
void pc_uart_push(uchar data)
{
}
static uchar navcmd[12];
#define NAV_INVALID     -1000
#define NAV_VALID       -2000
#define RESETNAV       navlen = 0;   reading = NAV_INVALID;

void nav_command(uchar cmd)
{
        if(cmd == NAV_30V)
               sprintf(navcmd,"%%01;12;02\r");
        if(cmd == NAV_1V)
               sprintf(navcmd,"%%01;12;01\r");
        if(cmd == NAV_120MV)
               sprintf(navcmd,"%%01;12;00\r");
        if(cmd == NAV_AFLTOFF)
               sprintf(navcmd,"%%01;26\r");
        if(cmd == NAV_AFLTON)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == NAV_SLOWMODE)
               sprintf(navcmd,"%%01;27\r");
        if(cmd == NAV_ZEROON)
        {
               sprintf(navcmd,"%%01;06\r");
        }
        if(cmd == NAV_INIT)
        {
                RESETNAV;
                sprintf(navcmd,"%%01;00\r");
        }
        if(cmd == NAV_READ)
        {
                RESETNAV;
                sprintf(navcmd,"%%01;01\r");
        }
//                com1_putc('?');
        mputs(navcmd,strlen(navcmd),PORT_NAV);
}


unsigned long navtime;
double nav_read()
{
//        sleepms(10);
//                return 1.0;
        nav_command(NAV_READ);
        navtime = 0;
        while(1)
        {
                if(reading < NAV_INVALID) //make sure it is a valid reading
                {
                     reading = atof(navread2);//buf2double(navread2);//
                     return reading;
                }
                if(navtime++ > 655350)
                {
                     nav_command(NAV_READ);
                     navtime = 0;
                }
                sleepms(1);
        }
        return 1.0;
}
//incoming data hander of navameter
void nav_uart_push(uchar data)
{
        if(navlen >= 19)
        {
                RESETNAV;
        }
        if(((data >= '0') && (data <= '9')) ||
                (data == '.') ||
                (data == '-') ||
                (data == '+') ||
                (data == 'e') ||
                (data == 'E') )
        {
                navread[navlen++] = data;
                reading = NAV_INVALID;
                return;
        }

        if(navlen < 4) //not enough digits
        {
                RESETNAV;
                return;
        }
        navread[navlen] = '\0';
        do
        {
                navread2[navlen] = navread[navlen];
        }while(navlen-- > 0);
        navlen = 0;
        reading =  NAV_VALID; //valid data
}
