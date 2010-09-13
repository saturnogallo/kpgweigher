/*********************************************************************************/
//                         UART0 and UART1 Subroutines
/*********************************************************************************/
#include <mega64.h>
#include "uart.h" 
#include "16c554.h" 
#include "global.h"                 

extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
extern u8 x_modem_databuf[];                       /* store data sent from PC */
extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
extern u8 x_modem_file_being_transferred;  
extern u8 x_modem_file_transfer_complete;

/*********************************************************************************/
//                     UART0  Variables & Buffer Definition
/*********************************************************************************/
u8 volatile tx_buffer0[TX_BUFFER_SIZE0];
u8 volatile tx_wr_index0,tx_rd_index0,tx_counter0;

/*********************************************************************************/
//                     UART1  Variables & Buffer Definition
/*********************************************************************************/
u8 volatile tx_buffer1[TX_BUFFER_SIZE1];
u8 volatile tx_wr_index1,tx_rd_index1,tx_counter1;

u8 ishexchar(u8 dat){
        if( (dat >= '0') && (dat <= '9'))
                return dat-'0';
        if( (dat >= 'a') && (dat <= 'f'))
                return dat-'a'+0x0a;
        if( (dat >= 'A') && (dat <= 'F'))
                return dat-'A'+0x0a;
        return 0xff;                
}                                     

/*********************************************************************************/
//              USART0 (Debug Port) Receiver interrupt service routine                
/*********************************************************************************/
void debug_state_machine(u8); 
void x_modem_protocol_rsm(u8);

interrupt [USART0_RXC] void usart0_rx_isr(void)
{  
   u8 status,data;
   
   //read status and data
   status=UCSR0A;
   data=UDR0; 
#ifdef USE_COM0  
   LED_FLASH(LED_PC); //LED to indicate communication status
   if(RFlagPC == RF_CKSUM)
        return;    
   cm_pushPC(data);
#endif
#ifdef USE_COM1
   //d_putchar(data);
   if(!dsm_cmd_received)
      debug_state_machine(data); 
   x_modem_protocol_rsm(data);
#endif
}

/*********************************************************************************/
//                 USART0 Transmitter interrupt service routine
/*********************************************************************************/

interrupt [USART0_TXC] void usart0_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0++];
   tx_rd_index0 =tx_rd_index0 & (TX_BUFFER_SIZE0-1);
   };
}


/*********************************************************************************/
//                      UART1 Receiver interrupt service routine
/*********************************************************************************/
interrupt [USART1_RXC] void usart1_rx_isr(void)
{
   u8 status,data;
   
   //read status and data
   status=UCSR1A;
   data=UDR1; 
#ifdef USE_COM0
   //d_putchar(data);
   if(!dsm_cmd_received)
      debug_state_machine(data);
   x_modem_protocol_rsm(data);
#endif
#ifdef USE_COM1
   LED_FLASH(LED_PC); //LED to indicate communication status
   if(RFlagPC == RF_CKSUM)
        return;    
   cm_pushPC(data);
#endif
}

/*********************************************************************************/
//                   USART1 Transmitter interrupt service routine
/*********************************************************************************/

interrupt [USART1_TXC] void usart1_tx_isr(void)
{
if (tx_counter1)
   {
   --tx_counter1;
   UDR1=tx_buffer1[tx_rd_index1++];
   tx_rd_index1=tx_rd_index1 & (TX_BUFFER_SIZE1-1);
   };
}

void d_putchar(char c)
{
while (tx_counter0 == TX_BUFFER_SIZE0);
#asm("cli")
if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0++]=c;
   tx_wr_index0 = tx_wr_index0 & (TX_BUFFER_SIZE0-1);
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}                      

void putchar(char c)
{
while (tx_counter1 == TX_BUFFER_SIZE1);
#asm("cli")
if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer1[tx_wr_index1++]=c;
   tx_wr_index1 = tx_wr_index1 & (TX_BUFFER_SIZE1-1);
   ++tx_counter1;
   }
else
   UDR1=c;
#asm("sei")
}

void d_putchar2(u8 a)
{
	unsigned char h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)		
		d_putchar(h+'0');
	else
		d_putchar(h+'A'-0x0a);

	if(l <= 9)
		d_putchar(l+'0');
	else
		d_putchar(l+'A'-0x0a);
}
void d_putstr(flash u8 *ptr){
        while(*ptr != 0x00){
                d_putchar(*ptr++);
        } 
}
void putchar2(u8 a)
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
} 
void putstr(flash u8 *ptr){
        while(*ptr != 0x00){
                putchar(*ptr++);
        } 
}                             

void d_mputs(u8 *buf, u8 size){
        while(size-- > 0){
                d_putchar2(*buf++);
        }
}
void mputs(u8 *buf, u8 size){
        while(size-- > 0){
                putchar(*buf++);
        }
}

/*********************************************************************************/
//                              Intialize UART
/*********************************************************************************/
void Init_UART()
{
   // USART0 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART0 Receiver: On
   // USART0 Transmitter: On
   // USART0 Mode: Asynchronous
   // USART0 Baud rate: 115200 (0x7), 57600 (0xF) 38400 (0x17)
   // USART0 Baud rate: 19200 (2F) 9600(0x5F)
      UCSR0A=0x00;
      UCSR0B=0xD8;
      UCSR0C=0x06;
      UBRR0H=0x00;      
      UBRR0L=0x5F;

   // USART1 initialization
   // Communication Parameters: 8 Data, 1 Stop, No Parity
   // USART1 Receiver: On
   // USART1 Transmitter: On
   // USART1 Mode: Asynchronous
   // USART1 Baud rate: 115200
      UCSR1A=0x00;
      UCSR1B=0xD8;
      UCSR1C=0x06;
      UBRR1H=0x00;
      UBRR1L=BAUD_TO_USE;
      
   // Initialize buffer variables
      tx_wr_index0 = 0;
      tx_rd_index0 = 0;
      tx_counter0  = 0;
      
      tx_wr_index1 = 0;
      tx_rd_index1 = 0;
      tx_counter1  = 0; 
     
} 

/**************************************************************************************/
// debug receiver state machine
// format: "CMDn nn<CR>" 
// This subroutine receives and extract input parameters from user command.
// input: data received from UART
// output: global variables: dsm_rcmd, dsm_rpara, dsm_cmd_received
/**************************************************************************************/ 
#define INIT_STATE 0
#define CMD_CHAR1_RECEIVED 1
#define CMD_CHAR2_RECEIVED 2
#define CMD_CHAR3_RECEIVED 3
#define CMD_CHAR4_RECEIVED 4
#define CMD_SPACE_RECEIVED 5
#define CMD_MSB_RECEIVED   6
#define CMD_LSB_RECEIVED   7

void debug_state_machine(u8 data)
{
   static u8 dsm_state;
   d_putchar(data);
   switch(dsm_state)
   {
     case INIT_STATE:
        if(data == 'C') 
			dsm_state = CMD_CHAR1_RECEIVED;	
            dsm_rcmd=0;
			dsm_rpara=0;
	    break;

	 case CMD_CHAR1_RECEIVED:
		if(data == 'M') 
			dsm_state = CMD_CHAR2_RECEIVED;
		else if(data == 'C') 
			dsm_state = CMD_CHAR1_RECEIVED;
		else
			dsm_state = INIT_STATE;
		break;

	 case CMD_CHAR2_RECEIVED: 
        if(data == 'D')
			dsm_state = CMD_CHAR3_RECEIVED;
		else if(data == 'C')
		    dsm_state = CMD_CHAR1_RECEIVED;
		else
			dsm_state = INIT_STATE;
		break;
	 case CMD_CHAR3_RECEIVED:
	    if((data>0x29) &&(data <0x3a))
		{	dsm_state = CMD_CHAR4_RECEIVED;
		    dsm_rcmd = data - 0x30;
		}
		else if(data == 'C')
		    dsm_state = CMD_CHAR1_RECEIVED;
		else
			dsm_state = INIT_STATE;
		break;

	 case CMD_CHAR4_RECEIVED:
	    if(data == ' ')
			dsm_state = CMD_SPACE_RECEIVED;
		else if(data == 'C')
		    dsm_state = CMD_CHAR1_RECEIVED;
		else
			dsm_state = INIT_STATE;
		break;

	 case CMD_SPACE_RECEIVED:
		 if((data>0x29) && (data<0x3a))
		 {	 dsm_state = CMD_MSB_RECEIVED;
		     dsm_rpara = (data-0x30)*10;            /* get the most significant 4 bits of parameter byte */
		 }
		 else if(data == 'C')
		     dsm_state = CMD_CHAR1_RECEIVED;
		 else
			 dsm_state = INIT_STATE;
		 break;

	 case CMD_MSB_RECEIVED:
		 if((data>0x29) && (data<0x3a))
		 {	 dsm_state = CMD_LSB_RECEIVED;
             dsm_rpara += (data - 0x30);              /* get parameter byte */
		 }
		 else if(data == 'C')
		    dsm_state = CMD_CHAR1_RECEIVED;
		 else
			 dsm_state = INIT_STATE;
		 break;

	 case CMD_LSB_RECEIVED:
		 dsm_cmd_received = 1;                      /* inform main loop a command is available */
		 dsm_state = INIT_STATE;		 
		 break;

	 default:
		 dsm_state = INIT_STATE;
		 break;

   }
 
}

/**************************************************************************************/
// x modem protocol receiver state machine
// X_MODEM protocol
// format <SOH>,<pack_index>, <255-pack_index> ,<128 bytes>, <crc_check_sum_word>
/**************************************************************************************/
#define X_MODEM_AWAIT_SOH          0
#define X_MODEM_SOH_RECEIVED       1
#define X_MODEM_PACKNUM1_RECEIVED  2
#define X_MODEM_PACKNUM2_RECEIVED  3
#define X_MODEM_DATA_RECEIVING     4
#define X_MODEM_ALL_DATA_RECEIVED  5
#define X_MODEM_CRC1_RECEIVED      6
#define X_MODEM_CRC2_RECEIVED      7

void x_modem_protocol_rsm(u8 data)
{
   static u8 x_modem_state;
   static u8 num_of_data_received;
   switch(x_modem_state)
   { 
      case X_MODEM_AWAIT_SOH:
		num_of_data_received = 0;
		if(data == XMODEM_SOH )
		{   x_modem_state = X_MODEM_SOH_RECEIVED;                    
                    x_modem_file_being_transferred = 1;
                    x_modem_file_transfer_complete = 0; 
		}
		if(data == XMODEM_EOT)                             /* End of transfer */
		{
		    x_modem_file_transfer_complete = 1; 
		    PORTD.7 = 0;
		}
		break;

	  case X_MODEM_SOH_RECEIVED: 
	    num_of_data_received = data;                                    /* temporarily save received pack_num into variable "num_of_data_received"*/
		x_modem_state = X_MODEM_PACKNUM1_RECEIVED;
	    break;

	  case X_MODEM_PACKNUM1_RECEIVED:
		if (data + num_of_data_received !=255)                          /* double check transfer complies with x_modem protocol */
            x_modem_state = X_MODEM_AWAIT_SOH;
		else
		    x_modem_state = X_MODEM_PACKNUM2_RECEIVED;
	    break;

	  case X_MODEM_PACKNUM2_RECEIVED:  
	       num_of_data_received = 1;                                     /* record num of data that has been received */
		   x_modem_databuf[0] = data;                                    /* save data into receiver buffer*/
		   x_modem_state = X_MODEM_DATA_RECEIVING;
		break;

	  case X_MODEM_DATA_RECEIVING: 
	    num_of_data_received++;
        x_modem_databuf[num_of_data_received-1] = data;
		if(num_of_data_received >= 128)                                  /* all data in the pack has been successfully received */
           x_modem_state = X_MODEM_ALL_DATA_RECEIVED;
	    break;

	  case X_MODEM_ALL_DATA_RECEIVED:  
	    num_of_data_received++;										    /* save first CRC byte */
        x_modem_databuf[num_of_data_received-1] = data;	    
        x_modem_state = X_MODEM_CRC1_RECEIVED;
        break;

	  case X_MODEM_CRC1_RECEIVED:
	    num_of_data_received++;										    /* save second CRC byte */
        x_modem_databuf[num_of_data_received-1] = data;	  
		x_modem_one_pack_received = 1;                                  /* flag to inform main loop a pack has been received */
		x_modem_state = X_MODEM_AWAIT_SOH;
        break;

	  default: 
        x_modem_state = X_MODEM_AWAIT_SOH;
		break;         
   }

}
