#include "sjDefine.h"
// UART.C
//
// Generic software uart written in C, requiring a timer set to 3 times
// the baud rate, and two software read/write pins for the receive and
// transmit functions.
//
// * Received characters are buffered
// * putchar(), getchar(), io_hasc() and flush_input_buffer() are available
// * There is a facility for background processing while waiting for input
//
// Colin Gittins, Software Engineer, Halliburton Energy Services
//
// The baud rate can be configured by changing the BAUD_RATE2 macro as
// follows:
//
//
// The function init_uart() must be called before any comms can take place
//
// Interface routines required:
// 1. get_rx2_pin_status()
//    Returns 0 or 1 dependent on whether the receive pin is high or low.
// 2. set_tx2_pin_high()
//    Sets the transmit pin to the high state.
// 3. set_tx2_pin_low()
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
// 4. void turn_rx2_on( void )
//    Turns on the receive function.
// 5. void turn_rx2_off( void )
//    Turns off the receive function.
// 6. void putchar( char )
//    Writes a character to the serial port.

#include <stdio.h>


sbit asp_rx2 = P3^4; //rx of simulated serial port2
sbit asp_tx2 = P3^5; //tx of simulated serial port2



#define IN_BUF_SIZE 32


static pdata unsigned char inbuf2[IN_BUF_SIZE];
static unsigned char qin2 = 0;
static unsigned char qout2 = 0;
/*****************************************************
Interface Routine for I/O analog serial port (UART.C)
(added by songj)
// 1. get_rx2_pin_status()
//    Returns 0 or 1 dependent on whether the receive pin is high or low.
// 2. set_tx2_pin_high()
//    Sets the transmit pin to the high state.
// 3. set_tx2_pin_low()
//    Sets the transmit pin to the low state.
// 4. idle()
//    Background functions to execute while waiting for input.
// 5. timer_set( BAUD_RATE2 )
//    Sets the timer to 3 times the baud rate.
// 6. set_timer_interrupt( timer_isr )
//    Enables the timer interrupt.

******************************************************/

#define get_rx2_pin_status()		asp_rx2

#define set_tx2_pin_high()	asp_tx2 = 1

#define set_tx2_pin_low()	asp_tx2 = 0

#define turn_rx2_on()	flag_rx2_off = FALSE

#define turn_rx2_off()	flag_rx2_off = TRUE


void idle2()
{

}




static char  flag_rx2_waiting_for_stop_bit;
static char  flag_rx2_off;
static BYTE  rx2_mask;
static char  flag_rx2_ready;
static char  flag_tx2_ready;
static char  timer_rx2_ctr;
static char  timer_tx2_ctr;
static char  bits_left_in_rx2;
static char  bits_left_in_tx2;
static char  rx2_num_of_bits;
static char  tx2_num_of_bits;
static BYTE  internal_rx2_buffer;
//static char  internal_tx2_buffer;
static WORD  internal_tx2_buffer;
static BYTE user_tx2_buffer;

static BYTE mask;
static char start_bit, flag_in;


void asp_handler2()
{
	// Transmitter Section
	if ( flag_tx2_ready )
	{
		if ( --timer_tx2_ctr<=0 )
		{
			mask = internal_tx2_buffer& 0x0001;
			internal_tx2_buffer >>= 1;
			if ( mask )
			{
				set_tx2_pin_high();
			}
			else
			{
				set_tx2_pin_low();
			}
			timer_tx2_ctr = 3;
			if ( --bits_left_in_tx2<=0 )
			{
				flag_tx2_ready = FALSE;
			}
		}
	}
	// Receiver Section
	if ( flag_rx2_off==FALSE )
	{
		if ( flag_rx2_waiting_for_stop_bit )
		{
			if ( --timer_rx2_ctr<=0 )
			{
				flag_rx2_waiting_for_stop_bit = FALSE;
				flag_rx2_ready = FALSE;
				internal_rx2_buffer &= 0xFF;
				if ( internal_rx2_buffer!=0xC2 )
				{	
					inbuf2[qin2] = internal_rx2_buffer;
					if ( ++qin2>=IN_BUF_SIZE )
					{
						qin2 = 0;
					}
				}	
			}
		}
		else // rx2_test_busy
		{
			if ( flag_rx2_ready==FALSE )
			{
				start_bit = get_rx2_pin_status();
				// Test for Start Bit
				if ( start_bit==0 )
				{
					flag_rx2_ready = TRUE;
					internal_rx2_buffer = 0;
					timer_rx2_ctr = 4;
					bits_left_in_rx2 = rx2_num_of_bits;
					rx2_mask = 1;
				}
			}
			else // rx2_busy
			{
				if ( --timer_rx2_ctr<=0 )
				{ 
					// rcv
					timer_rx2_ctr = 3;
					flag_in = get_rx2_pin_status();
					if ( flag_in )
					{
						internal_rx2_buffer |= rx2_mask;
					}
					rx2_mask <<= 1;
					if ( --bits_left_in_rx2<=0 )
					{
						flag_rx2_waiting_for_stop_bit = TRUE;
					}
				}
			}
		}
	}
}

void init_uart2( void )
{
	asp_tx2 = 1;//set the asp_tx2 to be output pin
	if(asp_rx2);//set the asp_rx2 to be input pin

	flag_tx2_ready = FALSE;
	flag_rx2_ready = FALSE;
	flag_rx2_waiting_for_stop_bit = FALSE;
	flag_rx2_off = FALSE;
	rx2_num_of_bits = 8;//8 data ,one stop ,no check
	tx2_num_of_bits = 10;//8 data,one stop ,no check

	set_tx2_pin_low();

	turn_rx2_on();//enable I/O serial

	qin2 = 0;
	qout2 = 0;

}

BYTE io_getc2( void )
{
	BYTE ch;

	do
	{
		while ( qout2==qin2 )
		{
			idle2();	
		}
		ch = inbuf2[qout2] & 0xFF;
		if ( ++qout2>=IN_BUF_SIZE )
		{
			qout2 = 0;
		}
	}
	while (ch==0xC2);
	return( ch );
}

void io_putc2( char ch )
{
	while ( flag_tx2_ready );
	user_tx2_buffer = ch;

	// invoke_UART_transmit
	timer_tx2_ctr = 3;
	bits_left_in_tx2 = tx2_num_of_bits;
	internal_tx2_buffer = (user_tx2_buffer<<1) | 0x200;
	flag_tx2_ready = TRUE;
}


unsigned char io_hasc2( void )
{
	if(qin2 ==qout2)
		return 0;
	if(qin2 > qout2)
		return qin2-qout2;
	return qin2+IN_BUF_SIZE -qout2;
}


