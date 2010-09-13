#include "uart.h"
#include <mega16.h>
#include "command.h"

extern u8 myaddr;                     /* RS485 address of this node.*/
extern BOOT_COMM_INTERFACE boot_comm; /* communications interface between bootloader with master boards */ 

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
CM_INFRAME  infrm; // = {0xfe,0x68,0,0,0,0, NULL_INBUF , 0};

CM_OUTFRAME outfrm; // = {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0};
  
u8 buf_cnt; 
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
u8 RFlag = RF_IDLE;
 
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
/****************************************************************************/
void cm_ack(void)
{                
   LED_FLASH(PIN_TxOK);
   outfrm.cksum = checksum( (u8*)(&outfrm), 6+outfrm.datalen);

   //print out the first 6 bytes of a frame, head1, ... cmd, data_len
   prints((u8*)(&outfrm),6);

   //print out data if there is any to be sent out
   if(outfrm.datalen > 0)
        prints(outfrm.databuf, outfrm.datalen);

   //print out check_sum byte
   putchar(outfrm.cksum);

}
/****************************************************************************/
//                          Command Receiver
// receive byte by byte from UART and save data into buffer until a completed
// frame is received. 
// Call "cm_process()" to analysis frame information
/****************************************************************************/

void cm_pushc(u8 c)
{  
  /* re-coded this subroutine to reduce code size */
  
  switch(RFlag)
  {  
     /*************************************************************/
     // idle state, awaiting head 1 byte
     /*************************************************************/     
     case RF_IDLE:                
        if(c == 0xfe)
        { 
          RFlag = RF_HEAD1;
          infrm.head_1 = c; 
        }
        break; 
     /*************************************************************/
     // Head 1 received, awaiting HEAD2
     /*************************************************************/                          
     case RF_HEAD1:
        if(c == 0x68 ) 
        {
          RFlag = RF_HEAD2;
          infrm.head_2 = c;
        }
        else if (c != 0xfe)
        {
          RFlag = RF_IDLE;
        }       
        break;   
     /*************************************************************/
     // Head 2 received, awaiting ADDR_FROM
     /*************************************************************/           
     case RF_HEAD2:
        infrm.addr_from = c;
        RFlag = RF_ADDRFROM;
        break; 
     /*************************************************************/
     // ADDR_FROM received, awaiting ADDR_TO byte
     /*************************************************************/     
     case RF_ADDRFROM:
        infrm.addr_to = c;
        RFlag = RF_ADDRTO; 
        break; 
     /*************************************************************/
     // ADDR_TO byte received, awaiting command byte
     /*************************************************************/        
     case RF_ADDRTO:
        infrm.cmd = c;
        RFlag = RF_CMD;
        break;
     /*************************************************************/
     // Command byte received, awaiting data
     /*************************************************************/        
     case RF_CMD:
        if(c <= CM_INBUF_MAX)
        {
           infrm.datalen = c;
           buf_cnt = 0;
           if(!c) 
               RFlag = RF_DATABUF;
           else
               RFlag = RF_DATALEN;
        }
        else
          RFlag = RF_IDLE;          
        break;     
     /*************************************************************/
     // Length byte received, awaiting data
     /*************************************************************/         
     case RF_DATALEN:
        infrm.databuf[buf_cnt++] = c;
        if(buf_cnt == infrm.datalen)
           RFlag = RF_DATABUF;
             
        break;                  
     /*************************************************************/
     // all data received, awaiting checksum bytes
     /*************************************************************/ 
     case RF_DATABUF: 
        infrm.cksum = c;     	    
        if( checksum(&infrm.head_1, 6+infrm.datalen) == c)
    	{       
           cm_process();
           RFlag = RF_IDLE; 
        }
        else
        {
           RFlag = RF_IDLE; 
/*         if(infrm.datalen == 131)
              LED_FLASH(PIN_RUN);*/
        }
        break;               
     /*************************************************************/
     // other cases: 
     /*************************************************************/         
     default:
        RFlag = RF_IDLE;
        break;
  }  
}

/****************************************************************************/
//    Analysis a incoming frame/command after it is completed received
/****************************************************************************/
void cm_process(void)   
{       
   if((infrm.addr_to == myaddr) || (infrm.addr_to == BROADCAST_ADDR))
   {
       LED_FLASH(PIN_RxOK);
       parse_node_frm((u8*)&boot_comm); //modify the content 
   }                    
}

/****************************************************************************/
//              Analysis and process incoming frame  
// Frame format: header1, header2, addr_from, addr_to, cmd, data_len, 
// dat_buf[MAX], check_sum.  MAX = 16 here.
// infrm is the frame received. outfrm is the frame to be sent out
// infrm/output.databuf[] format: {reg_id,data, reg_id, data ...}
/****************************************************************************/
void parse_node_frm(u8* ptr,)
{
        u8 j;                                            
        
        if(infrm.cmd == 'W'){   //write bytes, regid, data, regid, data, regid, data ...
                j = 0;
                while(j < infrm.datalen){  
                        *(ptr+infrm.databuf[j]) = infrm.databuf[j+1];
                        j = j+2;
                }                              
        }
        if(infrm.cmd == 'S'){   //Stream write: addr, data, data, data ....
            LED_FLASH(PIN_RUN);
            for (j =0; j < infrm.datalen-1; j++)
            {
                *(ptr+infrm.databuf[0] + j) = infrm.databuf[j+1];  
            }                             
        }                                   
        if(infrm.cmd == 'R'){   //read bytes and feedback to caller
                outfrm.head_1 = 0xfe;
                outfrm.head_2 = 0x68;
                outfrm.addr_from = infrm.addr_to;
                outfrm.addr_to = infrm.addr_from;                
                outfrm.cmd = 'W';
                outfrm.datalen = infrm.datalen << 1;
                for(j=0; j < infrm.datalen; j++){
                        outfrm.databuf[j+j] = infrm.databuf[j];
                        outfrm.databuf[j+j+1] = *(ptr+infrm.databuf[j]);
                }
                cm_ack();  
        }   
}                                                                             

