#include "uart.h"
#include "mega8.h"
#include "command.h"

extern u8 myaddr;              //RS485 address of this node.

/****************************************************************************/
// Global buffer to save data received or to be sent out.
// UART Frame Format: 
// Head1(0xfe) Head2(0x68), addr_from, addr_to, cmd, data_len
// data_to_send_or_receive, check_sum.
// Initialize frame buffer.
// The sender had better send an additional byte "Head1" prior to frame
// in case that the first byte is not correctly sent out by UART.
// For example, 0xfe, 0xfe, 0x68, addr_from ....
/****************************************************************************/
CM_INFRAME  infrm[STATE_MACHINE] =  {
        {0xfe,0x68,0,0,0,0, NULL_INBUF , 0}
};

CM_OUTFRAME outfrm[STATE_MACHINE] = {
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0}
};

u8 port_eeprom;
  
static u8 buf_cnt[STATE_MACHINE];   //STATE_MACHINE = 1, only 1 UART.
/****************************************************************************/
// Flag to indicate the process of receiving a frame. 
// Used by function "cm_pushc"
//#define RF_IDLE         0     // idle, no frame being received
//#define RF_HEAD1        1     // got frame head1 byte
//#define RF_HEAD2        2     // got frame head2 byte
//#define RF_ADDRFROM     3     // got addr_from byte
//#define RF_ADDRTO       4     // got addr_to byte
//#define RF_CMD          5     // got command byte
//#define RF_DATALEN      6     // got data length byte
//#define RF_DATABUF      7     // got data
//#define RF_CKSUM        8     // got check sum byte 
//Initialize status to "idle".   
/****************************************************************************/
u8 RFlag[STATE_MACHINE] = {RF_IDLE};

/****************************************************************************/
//                         Time Delay Subroutines
/****************************************************************************/
void sleepus(u16 us)
{               
        u8 j;
       	while(us-- > 0)
        	for (j=10; j>0; j--);
}

void sleepms(u16 ms)
{
  	while(ms-- > 0)
        	sleepus(100);	
}
 
/****************************************************************************/
//              Calculate check_sum data for a group of data
/****************************************************************************/                         
u8 checksum(u8 *buf, u8 size)
{
        u8 sum = 0;
        while(size-- > 0)
                sum = sum + *buf++;
                
        sum = (~sum)+1;
        return sum;
}   
        
/****************************************************************************/
//                         reply to a query 
// prints is an alias of function "m_puts"
//
/****************************************************************************/
void cm_ack(u8 port)
{                
   outfrm[port].cksum = checksum((u8*)(&outfrm[port]),6)+checksum( outfrm[port].databuf, outfrm[port].datalen);
   //print out the first 6 bytes of a frame, head1, ... cmd, data_len
   prints((u8*)(&outfrm[port]),6,port);
   //print out data if there is any to be sent out
   if(outfrm[port].datalen > 0)
        prints(outfrm[port].databuf, outfrm[port].datalen, port);
   //print out check_sum byte
   prints(&outfrm[port].cksum,1,port);
   // LED to indicate command ack.
   LED_FLASH(LED_D8);
}
/****************************************************************************/
//                          Command Receiver
// receive byte by byte from UART and save data into buffer until a completed
// frame is received. 
// Call "cm_process()" to analysis frame information
/****************************************************************************/

void cm_pushc(u8 c,u8 port)
{
      if(RFlag[port] == RF_CKSUM)                       //wait until it is process
                return;
      if(RFlag[port] == RF_DATABUF)               {     //checksum found
	      infrm[port].cksum = c;
    	      if(((checksum((u8*)(&infrm[port]),6)+checksum(infrm[port].databuf,infrm[port].datalen)) & 0xff) == c)
    	      	{
                        RFlag[port] = RF_CKSUM; 
                        cm_process(port);
                        RFlag[port] = RF_IDLE;
                        return;
                }                                                         
      }
      
      if(RFlag[port] == RF_DATALEN)               {     //databuf found
                infrm[port].databuf[buf_cnt[port]] = c;
                buf_cnt[port] += 1;
                if(buf_cnt[port] == infrm[port].datalen)
                        RFlag[port] = RF_DATABUF;
                return;
      }
      if(RFlag[port] == RF_CMD)                 {     //datalen found
                if(c <= CM_INBUF_MAX){
                        infrm[port].datalen = c;
                        buf_cnt[port] = 0;
                        if(c == 0) 
                                RFlag[port] = RF_DATABUF;
                        else
                                RFlag[port] = RF_DATALEN;
                        return;
                }                                
      }
      if(RFlag[port] == RF_ADDRTO)                 {     //cmd found
                infrm[port].cmd = c;
                RFlag[port] = RF_CMD; 
                return;
      }      
      if(RFlag[port] == RF_ADDRFROM)                 {     //addr_to found
                infrm[port].addr_to = c;
                RFlag[port] = RF_ADDRTO; 
                return;
      }

      if(RFlag[port] == RF_HEAD2)                 {     //addr_from found
                infrm[port].addr_from = c;
                RFlag[port] = RF_ADDRFROM;
                return;
      }
           
      if((RFlag[port] == RF_HEAD1) && (c == 0x68)){     //head2 found
                RFlag[port] = RF_HEAD2;  
                return;
      }
      if((RFlag[port] == RF_HEAD1) && (c == 0xfe)){     //double head1
                RFlag[port] = RF_HEAD1;
                return;
      }
      if((RFlag[port] == RF_IDLE) && (c == 0xfe)){      //first byte
                RFlag[port] = RF_HEAD1;
                return;
      }                
      RFlag[port] = RF_IDLE;
}

/****************************************************************************/
//    Analysis a incoming frame/command after it is completed received
/****************************************************************************/
void cm_process(u8 port)   
{       
   if((infrm[port].addr_to == myaddr) || (infrm[port].addr_to == BROADCAST_ADDR))
   {    parse_node_frm((u8*)&RS485,port); //modify the content 
        LED_FLASH(LED_D7);
   }                    
}

/****************************************************************************/
//              Analysis and process incoming frame  
// Frame format: header1, header2, addr_from, addr_to, cmd, data_len, 
// dat_buf[MAX], check_sum.  MAX = 16 here.
// infrm is the frame received. outfrm is the frame to be sent out
// infrm/output.databuf[] format: {reg_id,data, reg_id, data ...}
/****************************************************************************/
void parse_node_frm(u8* ptr, u8 port)
{
        u8 j;                                            
        /*if(infrm[port].cmd == 'C')
        {   // configure, save data to EEPROM
            // I add this command so that I can easily configure my raw boards.
            // infrm.databuf format: rom_start_addr,byte1, byte2, byte3, byte4.... 
            // add 0x80 to distiguish from 'F' command
            RS485._global.NumOfDataToBePgmed = infrm[port].datalen - 1 + 0x80;           
            port_eeprom = port;
        }//*/
        if(infrm[port].cmd == 'F'){   //write to flash
                RS485._global.NumOfDataToBePgmed = sizeof(S_FLASH);
        }        
        if(infrm[port].cmd == 'W'){   //write bytes to environment
                j = 0;
                while(j < infrm[port].datalen){  
                        *(ptr+infrm[port].databuf[j]) = infrm[port].databuf[j+1];
                        j = j+2;
                }                              
        }             
        if(infrm[port].cmd == 'X'){   //write words to environment
                j = 0;
                while(j < infrm[port].datalen){
                        *(ptr+infrm[port].databuf[j]) = infrm[port].databuf[j+1];
                        *(ptr+1+infrm[port].databuf[j]) = infrm[port].databuf[j+2];
                        j = j+3;
                }                              
        }             
        
        if(infrm[port].cmd == 'R'){   //read bytes and feedback to caller
                outfrm[port].addr_from = infrm[port].addr_to;
                outfrm[port].addr_to = infrm[port].addr_from;                
                outfrm[port].cmd = 'W';
                outfrm[port].datalen = infrm[port].datalen << 1;
                j = 0;                       
                while(j < infrm[port].datalen){
                        outfrm[port].databuf[2*j] = infrm[port].databuf[j];
                        outfrm[port].databuf[2*j+1] = *(ptr+infrm[port].databuf[j]);
                        j++;
                }
                cm_ack(port);  
        }
       if(infrm[port].cmd == 'S'){   //read words and feedback to caller           
                outfrm[port].addr_from = infrm[port].addr_to;
                outfrm[port].addr_to = infrm[port].addr_from;                
                outfrm[port].cmd = 'X';
                outfrm[port].datalen = infrm[port].datalen * 3;
                j = 0;                       
                while(j < infrm[port].datalen){
                        outfrm[port].databuf[3*j] = infrm[port].databuf[j];
                        outfrm[port].databuf[3*j+1] = *(ptr+infrm[port].databuf[j]);
                        outfrm[port].databuf[3*j+2] = *(ptr+1+infrm[port].databuf[j]);                        
                        j++;
                }
                cm_ack(port);  
        }        
}                                                                             

