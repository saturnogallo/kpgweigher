#include "stdafx.h"
#include "command.h"
#include "action.h"

/****************************************************************************/
// UART Frame Format: 
// Head1(0xfe) Head2(0x68), addr_from, addr_to, cmd, data_len
// data_to_send_or_receive, check_sum.
// Initialize frame buffer.
// The sender had better send an additional byte "Head1" prior to frame
// in case that the first byte is not correctly sent out by UART.
// For example, 0xfe, 0xfe, 0x68, addr_from ....
/****************************************************************************/
#define NULL_INBUF   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define NULL_OUTBUF  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#define RF_IDLE         0
#define RF_HEAD1        1
#define RF_HEAD2        2
#define RF_ADDRFROM     3
#define RF_ADDRTO       4
#define RF_CMD          5
#define RF_DATALEN      6
#define RF_DATABUF      7
#define RF_CKSUM        8

static u8 RFlag = RF_IDLE;

CM_INFRAME  infrm = {0xfe,0x68,0,0,0,0,NULL_INBUF,0};
CM_OUTFRAME  outfrm = {0xfe,0x68,0,0,0,0,NULL_OUTBUF,0};

extern u8 *glb_address;
extern u8 glb_hit;

u16 checksum(u8 *buf, u8 size){
        u8 sum = 0;
        while(size-- > 0){
                sum = sum + *buf++;
        }                          
        sum = 256-sum;
        return sum;
}        

const TCHAR* addr2name(u8 addr,u8 id)
{
	if(addr == 0xff){ //from sysboard
		SYS_ADDR_TO_NAME;
	}else{
		NODE_ADDR_TO_NAME;
	}
	return _T("N/A");
}
extern int glb_debug;
//handle infrm.cmd (only W and X is required)
void parse_frame(u8* ptr)
{
        u8 j,offset;
        if(infrm.cmd == 'W'){   //write environment by bytes
                j = 0;
                while(j < infrm.datalen){
					if(infrm.addr_from == 0xff)
						offset = s2l_map[infrm.databuf[j]];
					else
						offset = n2l_map[infrm.databuf[j]];
					if(offset == 0xff)
					{
						printf("unknow W pos\n");
						return;
					}
					
                    *(ptr+offset) = infrm.databuf[j+1];
					if(((ptr+offset)	== glb_address) && (glb_hit == 0) && (infrm.addr_from == 0xff))
						glb_hit = 1;
					if(glb_debug == 1)
						printf("(0x%02x)get 0x%02x as 0x%02x (%s)\n",infrm.addr_from,infrm.databuf[j],infrm.databuf[j+1],addr2name(infrm.addr_from,offset));
	                j = j+2;
                
				}                              
        }
        if(infrm.cmd == 'X'){   //write environment by words
                j = 0;
                while(j < infrm.datalen){                   
					if(infrm.addr_from == 0xff)
						offset = s2l_map[infrm.databuf[j]];
					else
						offset = n2l_map[infrm.databuf[j]];
					if(offset == 0xff)
					{
						printf("unknown X pos\n");
						return;
					}

                    *(ptr+offset) = infrm.databuf[j+1];
                    *(ptr+1+offset) = infrm.databuf[j+2];   
					if(((ptr+offset) == glb_address) && (glb_hit == 0) && (infrm.addr_from == 0xff))
						glb_hit = 1;
					if(((ptr+offset+1) == glb_address) && (glb_hit == 0) && (infrm.addr_from == 0xff))
						glb_hit = 1;
					if(glb_debug == 1)
						printf("(0x%02x)get 0x%02x as 0x%02x%02x (%s)\n",infrm.addr_from,infrm.databuf[j],infrm.databuf[j+1],infrm.databuf[j+2],addr2name(infrm.addr_from,offset));
                    j = j+3;
                }                              
        }
}

void cm_process(){
	int i;
    if(infrm.addr_to == 0x00)
	{
			if(infrm.addr_from == 0xff){//the frame comes from mainboard
				parse_frame((u8*)&Sysboard);
			}else{//from node
       		     for(i=0;i<MAX_NODE_NUM;i++){ //search the node
					 if(infrm.addr_from == RS485_Node[i].addr){
                       	 	parse_frame((u8*)&RS485_Node[i]);
                       	 	break;
                   	}
				 }   
				 if(i >= MAX_NODE_NUM)
					 dbgprint("unmatched command\n");
			}
	}else{
		printf("addr error\n");
	}
}

static u8 buf_cnt = 0;

//just form the frame and send it out
void cm_ack()
{                
   outfrm.cksum = checksum((u8*)(&outfrm),6)+checksum( outfrm.databuf, outfrm.datalen);
   serial.SendData((u8*)(&outfrm),6);
   if(outfrm.datalen > 0)
        serial.SendData(outfrm.databuf, outfrm.datalen);
   serial.SendData((u8*)&(outfrm.cksum),1);
}

void cm_write(u8 addr, u8 regid, u8 size, u8 *dat)
{
        u8 j,offset;
	if(addr == 0xff){
		offset = l2s_map[regid];
	}else{
		offset = l2n_map[regid];
	}
	if(offset == 0xff)
		return;

        outfrm.addr_from = 0x00;
		outfrm.addr_to = addr;
		outfrm.cmd = 'W';
        outfrm.datalen = 2*size;
        j = 0;
	while(j < 2*size){
		outfrm.databuf[j++] = offset++;
       		outfrm.databuf[j++] = *dat;
		CString name = addr2name(addr,regid);
		if((name.Find("flag_report") < 0))//(name.Find("flag_search") < 0) && (name.Find("flag_goon") < 0))
			printf("(0x%02x)set (%s) to 0x%02x\n",addr,addr2name(addr,regid++),*dat++);
		else{
			regid++;
			dat++;
		}
	}
        cm_ack();
}
void cm_query(u8 addr, u8 regid, u8 size)
{                          
    u8 j,offset;
    outfrm.addr_from = 0x00;
    outfrm.addr_to = addr;       
		
    outfrm.cmd = 'R';
    outfrm.datalen = size;
	if(addr == 0xff){
		offset = l2s_map[regid];
	}else{
		offset = l2n_map[regid];
	}
	if(offset == 0xff)
		return;

    j = 0;
    while(j < size){
        outfrm.databuf[j++] = offset++;
    }                           
    cm_ack();
}                    

void cm_pushc(u8 c)
{
      if(RFlag == RF_CKSUM)                       //wait until it is processed
                return;
      if(RFlag == RF_DATABUF)               {     //checksum found
                infrm.cksum = c;
                if(((checksum((u8*)(&infrm),6)+checksum(infrm.databuf,infrm.datalen)+256-c) & 0xff) == 0){
                        RFlag = RF_CKSUM;
                        cm_process();
                        RFlag = RF_IDLE;
                        return;
				}						
				dbgprint("ck error\n");
      }
      if(RFlag == RF_DATALEN)               {     //databuf found
                infrm.databuf[buf_cnt] = c;
                buf_cnt += 1;
                if(buf_cnt == infrm.datalen)
                        RFlag = RF_DATABUF;
                return;
      }
      if(RFlag == RF_CMD)                 {     //datalen found
                infrm.datalen = c;
                buf_cnt = 0;
                if(c == 0) 
                     RFlag = RF_DATABUF;
                else
                    RFlag = RF_DATALEN;
				if(c <= CM_INBUF_MAX)                
					return;
                                             
      }
      if(RFlag == RF_ADDRTO)                 {     //cmd found
                infrm.cmd = c;
                RFlag = RF_CMD; 
                return;
      }      
      if(RFlag == RF_ADDRFROM)                 {     //addr_to found
                infrm.addr_to = c;
                RFlag = RF_ADDRTO; 
                return;
      }

      if(RFlag == RF_HEAD2)                 {     //addr_from found
                infrm.addr_from = c;
                RFlag = RF_ADDRFROM;
                return;
      }
      if((RFlag == RF_HEAD1) && (c == 0x68)){     //head2 found
                RFlag = RF_HEAD2;
                return;
      }
      if((RFlag == RF_HEAD1) && (c == 0xfe)){     //double head1
                RFlag = RF_HEAD1;
                return;
      }
      if((RFlag == RF_IDLE) && (c == 0xfe)){      //first byte
                RFlag = RF_HEAD1;
                return;
      }                
      RFlag = RF_IDLE;
}
