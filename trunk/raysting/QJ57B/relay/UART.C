#include "reg52.h"
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
// #define BAUD_RATE 19200.0
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
// 2. char kbhit( void )
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

extern unsigned char get_rx_pin_status();
extern void set_tx_pin_low();
extern void set_tx_pin_high();
extern void timer_set(int baud_rate);
extern void idle();

#define BAUD_RATE 9600//19200.0

#define IN_BUF_SIZE 32

#define TRUE 1
#define FALSE 0

static unsigned char inbuf[IN_BUF_SIZE];
static unsigned char qin = 0;
static unsigned char qout = 0;

static char  flag_rx_waiting_for_stop_bit;
static char  flag_rx_off;
static char  rx_mask;
static char  flag_rx_ready;
static char  flag_tx_ready;
static char  timer_rx_ctr;
static char  timer_tx_ctr;
static char  bits_left_in_rx;
static char  bits_left_in_tx;
static char  rx_num_of_bits;
static char  tx_num_of_bits;
static char  internal_rx_buffer;
//static char  internal_tx_buffer;
static int  internal_tx_buffer;
static char user_tx_buffer;

void timer_isr(void) interrupt 1 using 3
{
	char mask, start_bit, flag_in;

	// Transmitter Section
	if ( flag_tx_ready )
	{
		if ( --timer_tx_ctr<=0 )
		{
			mask = internal_tx_buffer&1;
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
	TF0 = 0;
}

void init_uart( void )
{
	flag_tx_ready = FALSE;
	flag_rx_ready = FALSE;
	flag_rx_waiting_for_stop_bit = FALSE;
	flag_rx_off = FALSE;
	rx_num_of_bits = 8;//8 data ,one stop ,no check
	tx_num_of_bits = 10;//8 data,one stop ,no check

	set_tx_pin_low();

//  Sets the timer 1 to 3 times the baud rate(9600).(clock 11.0592M)
	//TH0 = 0xE0;//9600
	//TH0 = 0xC0;//4800
	TH0 = 0x80;//2400
	//TH0 = 0;//1200
	TR0= 1;
//sojo commented timer_set( BAUD_RATE );

//sojo commented set_timer_interrupt( timer_isr );  // Enable timer interrupt
}

char iogetchar( void )
{
	char ch;

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

void ioputchar( char ch )
{
	while ( flag_tx_ready );
	user_tx_buffer = ch;

	// invoke_UART_transmit
	timer_tx_ctr = 3;
	bits_left_in_tx = tx_num_of_bits;
	internal_tx_buffer = (user_tx_buffer<<1) | 0x200;
	flag_tx_ready = TRUE;
}

void flush_input_buffer( void )
{
	qin = 0;
	qout = 0;
}

char kbhit( void )
{
	return( qin!=qout );
}

void turn_rx_on( void )
{
	flag_rx_off = FALSE;
}
/*
void turn_rx_off( void )
{
	flag_rx_off = TRUE;
}
*/
