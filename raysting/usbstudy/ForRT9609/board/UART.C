#include "sjDefine.h"
// UART.C
//
// Generic software uart written in C, requiring a timer set to 3 times
// the baud rate, and two software read/write pins for the receive and
// transmit functions.
//
// * Received characters are buffered
// * putchar(), getchar(), kbhit() and flush_input_buffer() are available
// * There is a facility for background processing while waiting for input
//
// Colin Gittins, Software Engineer, Halliburton Energy Services
//
// The baud rate can be configured by changing the BAUD_RATE macro as
// follows:
//
//
// The function init_uart() must be called before any comms can take place
//
// Interface routines required:
// 1. get_rx_pin_status()
//    Returns 0 or 1 dependent on whether the receive pin is high or low.
// 2. set_tx_pin_high()
//    Sets the transmit pin to the high state.
// 3. set_tx_pin_low()
//    Sets the transmit pin to the low state.
// 4. idle()
//    Background functions to execute while waiting for input.
// 5. timer_set( BAUD_RATE )
//    Sets the timer to 3 times the baud rate.
// 6. set_timer_interrupt( timer_isr )
//    Enables the timer interrupt.
//
// Functions provided:
// 1. void flush_input_buffer( void )
//    Clears the contents of the input buffer.
// 2. char io_hasc( void )
//    Tests whether an input character has been received.
// 3. char getchar( void )
//    Reads a character from the input buffer, waiting if necessary.
// 4. void turn_rx_on( void )
//    Turns on the receive function.
// 5. void turn_rx_off( void )
//    Turns off the receive function.
// 6. void putchar( char )
//    Writes a character to the serial port.

#include <stdio.h>

sbit asp_rx	= P3^6; //rx of simulated serial port1
sbit asp_tx = P3^7; //tx of simulated serial port1


sbit asp_rx2 = P3^4; //rx of simulated serial port2
sbit asp_tx2 = P3^5; //tx of simulated serial port2



#define IN_BUF_SIZE 32


static pdata BYTE inbuf[IN_BUF_SIZE];
static unsigned char qin = 0;
static unsigned char qout = 0;
/*****************************************************
Interface Routine for I/O analog serial port (UART.C)
(added by songj)
// 1. get_rx_pin_status()
//    Returns 0 or 1 dependent on whether the receive pin is high or low.
// 2. set_tx_pin_high()
//    Sets the transmit pin to the high state.
// 3. set_tx_pin_low()
//    Sets the transmit pin to the low state.
// 4. idle()
//    Background functions to execute while waiting for input.
// 5. timer_set( BAUD_RATE )
//    Sets the timer to 3 times the baud rate.
// 6. set_timer_interrupt( timer_isr )
//    Enables the timer interrupt.

******************************************************/

void asp_set_timer()
{
	AUXR |= 0x80;		//1T mod
	TMOD &= 0xF0;		//timer mode
	TL0 = (65536-(FOSC/3/BAUD_RATE));
    TH0 = (65536-(FOSC/3/BAUD_RATE))>>8;                      //initial timer0 and set reload value
	
	TF0 = 0;		//clear TF0
    TR0 = 1;                            //tiemr0 start running
}
#define get_rx_pin_status()		asp_rx

#define set_tx_pin_high()	asp_tx = 1

#define set_tx_pin_low()	asp_tx = 0;

#define turn_rx_on()	flag_rx_off = FALSE

#define turn_rx_off()	flag_rx_off = TRUE


void idle()
{

}




static char  flag_rx_waiting_for_stop_bit;
static char  flag_rx_off;
static BYTE  rx_mask;
static char  flag_rx_ready;
static char  flag_tx_ready;
static char  timer_rx_ctr;
static char  timer_tx_ctr;
static char  bits_left_in_rx;
static char  bits_left_in_tx;
static char  rx_num_of_bits;
static char  tx_num_of_bits;
static BYTE  internal_rx_buffer;
//static char  internal_tx_buffer;
static WORD  internal_tx_buffer;
static BYTE user_tx_buffer;

static	BYTE mask;
static 	char start_bit, flag_in;
void asp_handler()
{
	// Transmitter Section
	if ( flag_tx_ready )
	{
		if ( --timer_tx_ctr<=0 )
		{
			mask = (BYTE)internal_tx_buffer ;
			mask = mask & 0x01;
			internal_tx_buffer >>= 1;
			if ( mask )
			{
				set_tx_pin_high();
			}
			else
			{
				set_tx_pin_low();
			}
			timer_tx_ctr = 3;
			if ( --bits_left_in_tx<=0 )
			{
				flag_tx_ready = FALSE;
			}
		}
	}
	// Receiver Section
	if ( flag_rx_off==FALSE )
	{
		if ( flag_rx_waiting_for_stop_bit )
		{
			if ( --timer_rx_ctr<=0 )
			{
				flag_rx_waiting_for_stop_bit = FALSE;
				flag_rx_ready = FALSE;
				internal_rx_buffer &= 0xFF;
				if ( internal_rx_buffer!=0xC2 )
				{	
					inbuf[qin] = internal_rx_buffer;
					if ( ++qin>=IN_BUF_SIZE )
					{
						qin = 0;
					}
				}	
			}
		}
		else // rx_test_busy
		{
			if ( flag_rx_ready==FALSE )
			{
				start_bit = get_rx_pin_status();
				// Test for Start Bit
				if ( start_bit==0 )
				{
					flag_rx_ready = TRUE;
					internal_rx_buffer = 0;
					timer_rx_ctr = 4;
					bits_left_in_rx = rx_num_of_bits;
					rx_mask = 1;
				}
			}
			else // rx_busy
			{
				if ( --timer_rx_ctr<=0 )
				{ 
					// rcv
					timer_rx_ctr = 3;
					flag_in = get_rx_pin_status();
					if ( flag_in )
					{
						internal_rx_buffer |= rx_mask;
					}
					rx_mask <<= 1;
					if ( --bits_left_in_rx<=0 )
					{
						flag_rx_waiting_for_stop_bit = TRUE;
					}
				}
			}
		}
	}
}

void init_uart( void )
{
	asp_tx = 1;//set the asp_tx to be output pin
	if(asp_rx);//set the asp_rx to be input pin

	flag_tx_ready = FALSE;
	flag_rx_ready = FALSE;
	flag_rx_waiting_for_stop_bit = FALSE;
	flag_rx_off = FALSE;
	rx_num_of_bits = 8;//8 data ,one stop ,no check
	tx_num_of_bits = 10;//8 data,one stop ,no check

	set_tx_pin_low();

	turn_rx_on();//enable I/O serial
	qin = 0;
	qout = 0;


}

BYTE io_getc( void )
{
	BYTE ch;

	do
	{
		while ( qout==qin )
		{
			idle();	
		}
		ch = inbuf[qout] & 0xFF;
		if ( ++qout>=IN_BUF_SIZE )
		{
			qout = 0;
		}
	}
	while (ch==0xC2);
	return( ch );
}

void io_putc( BYTE ch )
{
	while ( flag_tx_ready );
	user_tx_buffer = ch;

	// invoke_UART_transmit
	timer_tx_ctr = 3;
	bits_left_in_tx = tx_num_of_bits;
	internal_tx_buffer = (user_tx_buffer<<1) | 0x200;
	flag_tx_ready = TRUE;
}


BYTE io_hasc( void )
{
	if(qin ==qout)
		return 0;
	if(qin > qout)
		return qin-qout;
	return qin+IN_BUF_SIZE -qout;
}


