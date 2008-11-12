#include "global.h"  
#include "uart.h"
#include "mega64.h"
#include "command.h"
                           

CM_INFRAME  infrm[STATE_MACHINE] =  {
        {0xfe,0x68,0,0,0,0, NULL_INBUF , 0},
        {0xfe,0x68,0,0,0,0, NULL_INBUF , 0}, 
        {0xfe,0x68,0,0,0,0, NULL_INBUF , 0}, 
        {0xfe,0x68,0,0,0,0, NULL_INBUF , 0}, 
        {0xfe,0x68,0,0,0,0, NULL_INBUF , 0} 
};

CM_OUTFRAME outfrm[STATE_MACHINE] = {
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0},
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0},
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0},
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0},
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0}
};
  
static u8 buf_cnt[STATE_MACHINE];       //counter used to count the incoming databuf position now.
u8 RFlag[STATE_MACHINE] = {RF_IDLE,RF_IDLE,RF_IDLE,RF_IDLE,RF_IDLE};           //state of status machine
u8 RWait[STATE_MACHINE] = {R_NOWAIT,R_NOWAIT,R_NOWAIT,R_NOWAIT,R_NOWAIT};      //whether address has been hit by nodes
                                                         
#define  RCMD_BUF_SIZE 16                                                      
static u8 rcmd_waiting[RCMD_BUF_SIZE];  //loop buffer to store the reading command, node board should not send read cmd here. 
//the highest 1 bit indicate whether it byte or word reading, another 7 bits indicate position                                       
static u8 *prcmd_head= rcmd_waiting;   //loop buffer start buffer
static u8 *prcmd_tail = rcmd_waiting;    //loop buffer end buffer

u8 FB_in_up  = 0; //forward in port to pc
u8 FB_out_up = 0; //forward out port to pc
u8 FB_in_dn  = 0; //forward in port to node
u8 FB_out_dn = 0; //forward out port to node

void sleepms(u16 ms)
{                     
        u8 k;
        while(ms-- >0){
                k = 100;
                while(k-- > 0)
                        ;
        }
}            
void pc_query(u8 addr, u8 regid, u8 size, u8 port)
{                          
        u8 j;
        outfrm[port].addr_from = PC_ADDR;
        outfrm[port].addr_to = addr;       
        outfrm[port].cmd = 'R';
        outfrm[port].datalen = size;
        j = 0;
        while(j < size){
                outfrm[port].databuf[j++] = regid++;
        }                           
        cm_ack(port);
}                    
//query several bytes('size' bytes starting from 'regid') from addr, 
void cm_query(u8 addr, u8 regid, u8 size, u8 port)
{                          
        u8 j;
        outfrm[port].addr_from = MY_ADDR;
        outfrm[port].addr_to = addr;       
        outfrm[port].cmd = 'R';
        outfrm[port].datalen = size+1;
        j = 0;
        while(j < size){
                outfrm[port].databuf[j++] = regid++;
        }                           
        outfrm[port].databuf[j] = NREG_STATUS;   //always query status
        cm_ack(port);
}                    
//report several bytes('size' bytes starting from 'regid') to PC
void cm_report_b(u8 addr, u8 regid, u8 size, u8 *dat, u8 port)
{
        u8 j;
        outfrm[port].addr_from = addr;
        outfrm[port].addr_to = PC_ADDR;       
        outfrm[port].cmd = 'W';
        outfrm[port].datalen = 2*size;
        j = 0;
        while(j < 2*size){
        	outfrm[port].databuf[j++] = regid++;
                outfrm[port].databuf[j++] = *dat++;
        }
        cm_ack(port);
}
//report several words('size' words starting from 'regid') to PC
void cm_report_w(u8 addr, u8 regid, u8 size, u8 *dat, u8 port)
{
        u8 j;
        outfrm[port].addr_from = addr;
        outfrm[port].addr_to = PC_ADDR;       
        outfrm[port].cmd = 'X';
        outfrm[port].datalen = 3*size;
        j = 0;
        while(j < 3*size){
        	outfrm[port].databuf[j++] = regid++;
                outfrm[port].databuf[j++] = *dat++;
                outfrm[port].databuf[j++] = *dat++;
		regid = regid + 2;
        }                       
        cm_ack(port);
}
void cm_write(u8 addr, u8 regid, u8 size, u8 *dat, u8 port)
{
        u8 j;
        outfrm[port].addr_from = MY_ADDR;
        outfrm[port].addr_to = addr;       
        outfrm[port].cmd = 'W';
        outfrm[port].datalen = 2*size;
        j = 0;
        while(j < 2*size){   
                outfrm[port].databuf[j++] = regid++;
                outfrm[port].databuf[j++] = *dat++;
        }
        cm_ack(port);
}                              
u8 checksum(u8 *buf, u8 size){
        u8 sum = 0;
        while(size-- > 0){
                sum = sum + *buf++;
        }                          
        sum = (~sum)+1;
        return sum;
}             
/****************************************************************************/
//                            Frame cm_ack and forward
/****************************************************************************/
void cm_forward(u8 iport, u8 oport)
{                     
   prints((u8*)(&infrm[iport]),6,oport);
   if(infrm[iport].datalen > 0)
        prints(infrm[iport].databuf, infrm[iport].datalen, oport);
   prints((u8*)&(infrm[iport].cksum),1,oport);

}
void cm_ack(u8 port)
{                      
   checkforward_dn();
   checkforward_up();
   outfrm[port].cksum = checksum((u8*)(&outfrm[port]),6)+checksum( outfrm[port].databuf, outfrm[port].datalen);

   prints((u8*)(&outfrm[port]),6,port);
   if(outfrm[port].datalen > 0)
        prints(outfrm[port].databuf, outfrm[port].datalen, port);
   prints((u8*)&(outfrm[port].cksum),1,port);
}
/****************************************************************************/
//                     RS485 Frame Analysis 
// 程序功能： RS485通讯协议分析,分析接收到的帧，并作出相应的回应。
// 格    式： void cm_pushc(char,port) port: A,B,C,D,E
// 
/****************************************************************************/
extern u8 debug_mode;
void cm_pushc(u8 c,u8 port)
{
      if(RFlag[port] == RF_CKSUM)                       //wait until it is processed
                return;
      if(RFlag[port] == RF_DATABUF)               {     //checksum found
                infrm[port].cksum = c;
                if(((checksum((u8*)(&infrm[port]),6)+checksum(infrm[port].databuf,infrm[port].datalen)) & 0xff) == c){
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
void parse_node_frm(u8* ptr, u8 port)
{
        u8 j;
        if(infrm[port].cmd == 'W'){   //write environment by bytes
                j = 0;
                while(j < infrm[port].datalen){
                        if(infrm[port].databuf[j] == RWait[port]){
                              RWait[port] = R_NOWAIT;  
                        }
                        if( infrm[port].databuf[j] < NREG_SIZE_TOTAL ){
                                if( n2m_map[infrm[port].databuf[j]] != MREG_INVALID) 
                                {
                                        infrm[port].databuf[j] = n2m_map[infrm[port].databuf[j]];
                                        *(ptr+infrm[port].databuf[j]) = infrm[port].databuf[j+1];  
                                }		       
                        }
                        j = j+2;
                }                              
        }
        if(infrm[port].cmd == 'X'){   //write environment by words
                j = 0;
                while(j < infrm[port].datalen){                   
                        if(infrm[port].databuf[j] == RWait[port]){
                              RWait[port] = R_NOWAIT;  
                        }                        
                        if( infrm[port].databuf[j] < NREG_SIZE_TOTAL ){
                                if( n2m_map[infrm[port].databuf[j]] != MREG_INVALID)
                                {
                                        infrm[port].databuf[j] = n2m_map[infrm[port].databuf[j]];
                                        *(ptr+infrm[port].databuf[j]) = infrm[port].databuf[j+1];
                                        *(ptr+1+infrm[port].databuf[j]) = infrm[port].databuf[j+2];                        
                                }
                        }
                        j = j+3;
                }                              
        }
}
void parse_sys_frm(u8* ptr, u8 port)
{
        u8 j;
	u8 *tptr;
        if(infrm[port].cmd == 'W'){   //write environment by bytes
                j = 0;
                while(j < infrm[port].datalen){
                        if(infrm[port].databuf[j] == RWait[port]){
                              RWait[port] = R_NOWAIT;  
                        }
                        *(ptr+infrm[port].databuf[j]) = infrm[port].databuf[j+1];
                        j = j+2;
                }                              
        }
        if(infrm[port].cmd == 'X'){   //write environment by words
                j = 0;
                while(j < infrm[port].datalen){                   
                        if(infrm[port].databuf[j] == RWait[port]){
                              RWait[port] = R_NOWAIT;  
                        }
                        *(ptr+infrm[port].databuf[j]) = infrm[port].databuf[j+1];
                        *(ptr+1+infrm[port].databuf[j]) = infrm[port].databuf[j+2];                        
                        j = j+3;
                }                              
        }
        if(infrm[port].cmd == 'R'){   //read environment by bytes
                j = 0;                       
                while(j < infrm[port].datalen){                
                        if(((prcmd_tail+1) == prcmd_head) || 
                           (prcmd_tail == (prcmd_head+ RCMD_BUF_SIZE - 1)))       //buffer is full
                                return;
			//search for duplication
			tptr = prcmd_head;
			while(tptr != prcmd_tail){
				if(*tptr == infrm[port].databuf[j]){
					j++;
					break;	//duplication request found
				}
				tptr++;
                        	if(tptr == rcmd_waiting + RCMD_BUF_SIZE)
	                        	tptr = rcmd_waiting;
			}
			if(tptr != prcmd_tail)
			       continue;	
                        *prcmd_tail++ = infrm[port].databuf[j++]; 
                        if(prcmd_tail == (rcmd_waiting + RCMD_BUF_SIZE))            //come to border
                                prcmd_tail = rcmd_waiting;
                }
        }
        if(infrm[port].cmd == 'S'){   //read environment by words
               j = 0;                       
                while(j < infrm[port].datalen){                
                        if(((prcmd_tail+1) == prcmd_head) || 
                           (prcmd_tail == (prcmd_head + RCMD_BUF_SIZE - 1)))       //buffer is full
                                return;
       			//search for duplication
			tptr = prcmd_head;
			while(tptr != prcmd_tail){
				if(*tptr == (0x80 | infrm[port].databuf[j])){
					j++;	//duplication request found
					break;
				}
				tptr++;
                        	if(tptr == (rcmd_waiting + RCMD_BUF_SIZE))
	                        	tptr = rcmd_waiting;
			}
			if(tptr != prcmd_tail)
			       continue;	
       		        *prcmd_tail++ = 0x80 | infrm[port].databuf[j++]; 
                        if(prcmd_tail == (rcmd_waiting + RCMD_BUF_SIZE))            //come to border
                                prcmd_tail = rcmd_waiting;
                }
        }
}

void cm_process(u8 port)   //incoming command from node
{       
        u8 i;
        u8 addr;                                             
        if(port == SPORTPC){
                if(infrm[port].addr_to == MY_ADDR){ 
                        parse_sys_frm((u8*)&system,port);   
                        return;
                } 
//                if(infrm[port].addr_to > 0x80){        //no use now
                  if(0){
                      addr = infrm[port].addr_to-0x80;
                      for(i=0;i<system.node_num;i++){ //search the node
                         if(addr == RS485_Node[i].addr){
                                parse_node_frm((u8*)&RS485_Node[i],port);
                                return;
                         }
                       }           
                      for(i=0;i<system.vibrator_num;i++){ //search the vibrator
                         if(addr == vibrator[i].addr){
                                parse_node_frm((u8*)&vibrator[i],port);
                                return;
                         }
                       }                       
                }else{//forward command 
                      for(i=0;i<system.node_num;i++){ //search the node
                         if(infrm[port].addr_to == RS485_Node[i].addr){   
                                FB_in_dn = port;
                                FB_out_dn = RS485_Node[i].uart_port;
                                return;
                         }
                       }           
                      for(i=0;i<system.vibrator_num;i++){ //search the vibrator
                         if(infrm[port].addr_to == vibrator[i].addr){    
                                FB_in_dn = port;
                                FB_out_dn = vibrator[i].uart_port;
                                return;
                         }
                       }                       
                }
		return;
        }
        if(infrm[port].addr_to == PC_ADDR){ //forward command  to pc    
                FB_in_up = port;
                FB_out_up = SPORTPC;
                return;
        }
        if(infrm[port].addr_to == MY_ADDR){ //content sent from node to system board                                    
              for(i=0;i<system.node_num;i++){ //search the node         
                      if(infrm[port].addr_from == RS485_Node[i].addr){         
                                parse_node_frm((u8*)&RS485_Node[i],port); //modify the content
                                return;
                      }
              }
              for(i=0;i<system.vibrator_num;i++){ //search the vibrator
                      if(infrm[port].addr_from == vibrator[i].addr){
                                parse_node_frm((u8*)&vibrator[i],port); //modify the content
                                return;
                      }
              }
              
        }
}
void handle_pending_read(){         
        u8 type,value,cnt;
        if(prcmd_head == prcmd_tail)
                return;   
        type = *prcmd_head & 0x80;
        cnt = 0;     
        
        if(type){       //read by word
                outfrm[SPORTPC].cmd = 'X';         
                outfrm[SPORTPC].addr_from = MY_ADDR;
                outfrm[SPORTPC].addr_to = PC_ADDR;                
                outfrm[SPORTPC].datalen = 0;
                while((prcmd_head != prcmd_tail) && (cnt++ < 5)){ // 3x5 = 15 < 16
                     value = 0x7f & *prcmd_head++;        
                     outfrm[SPORTPC].databuf[outfrm[SPORTPC].datalen] = value;
                     outfrm[SPORTPC].datalen++;
                     outfrm[SPORTPC].databuf[outfrm[SPORTPC].datalen] = *((u8*)&system+value); 
                     outfrm[SPORTPC].datalen++;
                     outfrm[SPORTPC].databuf[outfrm[SPORTPC].datalen] = *((u8*)&system+1+value);
                     outfrm[SPORTPC].datalen++;
                     if(prcmd_head == rcmd_waiting + RCMD_BUF_SIZE)
                        prcmd_head = rcmd_waiting;
                }
                cm_ack(SPORTPC);                              
        }else{          //read by byte
                outfrm[SPORTPC].cmd = 'W';         
                outfrm[SPORTPC].addr_from = MY_ADDR;
                outfrm[SPORTPC].addr_to = PC_ADDR;                
                outfrm[SPORTPC].datalen = 0;
                while((prcmd_head != prcmd_tail) && (cnt++ < 7)){ // 2x7 = 14 < 16
                     value = *prcmd_head++;        
                     outfrm[SPORTPC].databuf[outfrm[SPORTPC].datalen] = value;
                     outfrm[SPORTPC].datalen++;
                     outfrm[SPORTPC].databuf[outfrm[SPORTPC].datalen] = *((u8*)&system+value);
                     outfrm[SPORTPC].datalen++;
                     if(prcmd_head == rcmd_waiting + RCMD_BUF_SIZE)
                        prcmd_head = rcmd_waiting;
                }
                cm_ack(SPORTPC);         
        }
}
