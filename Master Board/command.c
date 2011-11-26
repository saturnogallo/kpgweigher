#include "global.h"  
#include "uart.h"
#include "mega64.h"
#include "command.h"
                           

CM_INFRAME  ginfrm[STATE_MACHINE] =  {
        {0,0,0,0,0,0, NULL_INBUF , 0},
        {0,0,0,0,0,0, NULL_INBUF , 0},
        {0,0,0,0,0,0, NULL_INBUF , 0},
        {0,0,0,0,0,0, NULL_INBUF , 0},
        {0,0,0,0,0,0, NULL_INBUF , 0}
};
volatile u8  *infrmA;
volatile u8  *infrmB;
volatile u8  *infrmC;
volatile u8  *infrmD;
volatile u8  *infrmPC;

CM_INFRAME  ginfrm2[STATE_MACHINE] =  {
        {0,0,0,0,0,0, NULL_INBUF , 0},
        {0,0,0,0,0,0, NULL_INBUF , 0},
        {0,0,0,0,0,0, NULL_INBUF , 0},
        {0,0,0,0,0,0, NULL_INBUF , 0},
        {0,0,0,0,0,0, NULL_INBUF , 0}
};

CM_OUTFRAME outfrm[STATE_MACHINE] = {
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0},
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0},
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0},
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0},
        {0xfe,0x68,0,0,0,0,NULL_OUTBUF ,0}
};
  

u8 RFlagA = RF_IDLE;    //status of state machine
u8 buf_cntA = 0;        //counter used to count the incoming databuf position now.
u8 RFlagB = RF_IDLE;
u8 buf_cntB = 0;
u8 RFlagC = RF_IDLE;
u8 buf_cntC = 0;
u8 RFlagD = RF_IDLE;
u8 buf_cntD = 0;
u8 RFlagPC = RF_IDLE;
u8 buf_cntPC = 0;        


u8 RWait[STATE_MACHINE] = {R_NOWAIT,R_NOWAIT,R_NOWAIT,R_NOWAIT,R_NOWAIT};      //whether address has been hit by nodes
                                                         
#define  RCMD_BUF_SIZE 16 
                                                     
void sleepms(u16 ms)
{                     
        u8 k;
        while(ms-- >0){
                k = 100;
                while(k-- > 0)
                        ;
        }
}            
/*******************************************************************************/
// PC read data from a node specified by arg1 addr. 
// Arg2: starting register address of data to be read out. 
// Arg3: How many bytes to be read starting from reg address defined by arg2.
// arg4: define message will be send out through which port of 16C554.
// function cm_ack() calculates checksum and sends out a complete message frame.
/*******************************************************************************/
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
/*******************************************************************************/
// Master board reads data from a specified node.
// Arg1: node adress
// Arg2: starting address of data to be read from said node.
// Arg3: how many data to be read
// Arg4: from which port of 16C554 message will be sent out.
/*******************************************************************************/
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
/****************************************************************************/
// Block data transfer
// output data buf: 1-byte addr, "size"-byte data, 2-byte CRC
// max data size: 128 bytes.
/****************************************************************************/
extern int calcrc(char *ptr, int count);

void cm_block_write(u8 addr, u8 start_addr, u8 size, u8 *dat, u8 port)
{

        u8 j;
        u16 crc_data;
        outfrm[port].addr_from = MY_ADDR;
        outfrm[port].addr_to = addr;       
        outfrm[port].cmd = 'S';
        outfrm[port].datalen = size + 1; // start address (1 byte) + data size; 
        j = 0;
        /* start address byte */
        outfrm[port].databuf[j++] = start_addr;
        /* fill in data bytes */
        while(j < size+1 ){                   
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
void cm_forward(u8 *infrm, u8 oport)
{                     
   prints(infrm,6,oport);
   if(*(infrm+FRM_POS_DLEN) > 0)
        prints(infrm+FRM_POS_DBUF, *(infrm+FRM_POS_DLEN), oport);
   prints((infrm+FRM_POS_CKSUM),1,oport);

}
// #define _BL_DEBUG_
void cm_ack(u8 port)
{                      
#ifdef _BL_DEBUG_
   u8 i;         
#endif
   
   outfrm[port].cksum = checksum((u8*)(&outfrm[port]),6)+checksum( outfrm[port].databuf, outfrm[port].datalen);
   /* output header */
   prints((u8*)(&outfrm[port]),6,port);
   
#ifdef _BL_DEBUG_
   /*debug*/
   putchar(outfrm[port].head_1);
   putchar(outfrm[port].head_2);
   putchar(outfrm[port].addr_from);
   putchar(outfrm[port].addr_to);
   putchar(outfrm[port].cmd);
   putchar(outfrm[port].datalen);
#endif
   
   /* output data buf */
   if(outfrm[port].datalen > 0)
   {
        prints(outfrm[port].databuf, outfrm[port].datalen, port);
   } 
   
   /*debug*/
#ifdef _BL_DEBUG_
   for(i=0; i<outfrm[port].datalen; i++)
    putchar(outfrm[port].databuf[i]);
#endif
   
   /* output checksum */
   prints((u8*)&(outfrm[port].cksum),1,port); 
   
   /*debug */  
#ifdef _BL_DEBUG_
   putchar(outfrm[port].cksum); 
#endif
   
}
/****************************************************************************/
//                     RS485 Frame Analysis 
// 程序功能： RS485通讯协议分析,分析接收到的帧，并作出相应的回应。
// 格    式： void cm_pushc(char,port) port: A,B,C,D,E
// 
/****************************************************************************/
extern u8 debug_mode;

void parse_node_frm(u8* ptr, u8 *infrm,u8 port)
{
        u8 j;                                      
        u8 temp;
        if(infrm[FRM_POS_CMD] == 'W'){   //write environment by bytes
                j = 0;
                while(j < infrm[FRM_POS_DLEN]){             
                        temp = infrm[FRM_POS_DBUF+j];
                        if( temp == RWait[port]){
                              RWait[port] = R_NOWAIT;  
                        }
                        if( temp < NREG_SIZE_TOTAL ){
                                temp = n2m_map[temp];
                                if(temp != MREG_INVALID) {
                                        *(ptr+temp) = infrm[FRM_POS_DBUF+j+1];  
                                }		       
                        }
                        j = j+2;
                }      
                return;                        
        }
        if(infrm[FRM_POS_CMD] == 'X'){   //write environment by words
                j = 0;
                while(j < infrm[FRM_POS_DLEN]){   
                        temp = infrm[FRM_POS_DBUF+j];                
                        if(temp == RWait[port]){
                              RWait[port] = R_NOWAIT;  
                        }                        
                        if( temp < NREG_SIZE_TOTAL ){
                                temp = n2m_map[temp];
                                if( temp != MREG_INVALID) {
                                        *(ptr+temp) = infrm[FRM_POS_DBUF+j+1];
                                        *(ptr+1+temp) = infrm[FRM_POS_DBUF+j+2];                        
                                }
                        }
                        j = j+3;
                }                    
                return;          
        }
}

void nfu_parse_node_frm(u8* ptr, u8 *infrm,u8 port)
{
        u8 j;                                      
        u8 temp;
        if(infrm[FRM_POS_CMD] == 'W'){   //write environment by bytes
                j = 0;
                while(j < infrm[FRM_POS_DLEN]){             
                        temp = infrm[FRM_POS_DBUF+j];
                        if( temp == RWait[port]){
                              RWait[port] = R_NOWAIT;  
                        }                        
                        if( temp < 5 ){  // totally 5 bytes in boot_comm
                             *(ptr+temp) = infrm[FRM_POS_DBUF+j+1];  		       
                        }
                        j = j+2;
                }                        
        }
}

void parse_sys_frm(u8* ptr, u8 *infrm)
{
        u8 j;     
        u8 temp;
        if(infrm[FRM_POS_CMD] == 'W'){   //write environment by bytes
                j = 0;
                while(j < infrm[FRM_POS_DLEN]){  
                        *(ptr+infrm[FRM_POS_DBUF+j]) = infrm[FRM_POS_DBUF+j+1];
                        j = j+2;
                }      
                return;                        
        }
        if(infrm[FRM_POS_CMD] == 'X'){   //write environment by words
                j = 0;
                while(j < infrm[FRM_POS_DLEN]){  
                        *(ptr+infrm[FRM_POS_DBUF+j]) = infrm[FRM_POS_DBUF+j+1];
                        *(ptr+1+infrm[FRM_POS_DBUF+j]) = infrm[FRM_POS_DBUF+j+2];                        
                        j = j+3;
                }      
                return;                        
        }

        if(infrm[FRM_POS_CMD] == 'R'){   //read environment by bytes
                j = 0;                       
               outfrm[SPORTPC].cmd = 'W';         
               outfrm[SPORTPC].addr_from = MY_ADDR;
               outfrm[SPORTPC].addr_to = PC_ADDR;                
               temp = 0;

               while(j < infrm[FRM_POS_DLEN]){ 
                        outfrm[SPORTPC].databuf[temp] = infrm[FRM_POS_DBUF+j];
                        temp++;
                        outfrm[SPORTPC].databuf[temp] = ptr[infrm[FRM_POS_DBUF+j]]; 
                        temp++;                                               
                        j++;
                }       
                outfrm[SPORTPC].datalen = temp;
                cm_ack(SPORTPC);
                return;
        }
        if(infrm[FRM_POS_CMD] == 'S'){   //read environment by words
               j = 0;                       
               outfrm[SPORTPC].cmd = 'X';         
               outfrm[SPORTPC].addr_from = MY_ADDR;
               outfrm[SPORTPC].addr_to = PC_ADDR;                
               temp = 0;

               while(j < infrm[FRM_POS_DLEN]){ 
                        outfrm[SPORTPC].databuf[temp] = infrm[FRM_POS_DBUF+j];
                        temp++;
                        outfrm[SPORTPC].databuf[temp] = ptr[infrm[FRM_POS_DBUF+j]]; 
                        temp++;
                        outfrm[SPORTPC].databuf[temp] = ptr[1 + (u8)infrm[FRM_POS_DBUF+j]];
                        temp++;                                               
                        j++;
                }       
                outfrm[SPORTPC].datalen = temp;
                cm_ack(SPORTPC);                                               
                return;
        }
}

/**********************************************************************************************************/
//
//  Process command when a complete frame has been received.
//  RFlagPC/RFlagA/RFlagB/RFlagC/RFlagD are status flags of port PC,A,B,C and D respectively.
//  Only to process when checksum byte (last byte) received.  
//  For example, (RFlagPC == RF_CKSUM) means a complete frame has been received at PORT PC (RS232).
//  Subroutines called: 
//      parse_sys_frm(), read or write local register on master board.
//      cm_forward(), command is to access remote register on node board, forward it to node board.
//
/**********************************************************************************************************/
void cm_process()   //process command from nodes
{       
        u8 i;
        u8 *infrm;
        /****************************************************************/
        // A complete frame has been received at PORT PC
        /****************************************************************/
        if(RFlagPC == RF_CKSUM)
        {             
                infrm = infrmPC;
                if(infrm == (u8*)&ginfrm[SPORTPC]){
                        use_infrm2Bin(PC);
                }else{
                        use_infrmBin(PC);
                }        
                //reset port status flag to idle after receiving a complete frame
                RFlagPC = RF_IDLE; 

                //If checksum is correct, no error occured. Start to parse frame
                if(((checksum(infrm,6)+checksum(infrm+FRM_POS_DBUF,*(infrm+FRM_POS_DLEN))) & 0xff) == *(infrm+FRM_POS_CKSUM))
                {  
                        // command is to me (accesssing local register)
                        if(*(infrm+FRM_POS_TO) == MY_ADDR){ 
                                parse_sys_frm((u8*)&system,infrm);   
                                return;
                        }        
                        
                        //otherwise, forward command to nodes or vibrators.
                        for(i=0;i<system.node_num;i++){ //search the node
                                if(*(infrm+FRM_POS_TO) == RS485_Node[i].addr){   
                                        cm_forward(infrm,RS485_Node[i].uart_port);
                                        return;
                                }
                         }
                        for(i=0;i<system.vibrator_num;i++){ //search the vibrator
                                if(*(infrm+FRM_POS_TO) == vibrator[i].addr){   
                                        cm_forward(infrm,vibrator[i].uart_port);
                                        return;
                                }
                         }
                }           
                return;
        }             
         /****************************************************************/
        // A complete frame has been received at PORT A
        /****************************************************************/       
        if(RFlagA == RF_CKSUM)
        {             
                infrm = infrmA;
                if(infrm == (u8*)&ginfrm[SPORTA]){
                        use_infrm2Bin(A);
                }else{
                        use_infrmBin(A);
                }        
                RFlagA = RF_IDLE;
                if(((checksum(infrm,6)+checksum(infrm+FRM_POS_DBUF,*(infrm+FRM_POS_DLEN))) & 0xff) == *(infrm+FRM_POS_CKSUM)){   //cksum ok
                        if(*(infrm+FRM_POS_TO) == MY_ADDR){ 
                                for(i=0;i<system.node_num;i++){ //search the node         
                                      if(*(infrm+FRM_POS_FROM) == RS485_Node[i].addr){         
                                           parse_node_frm((u8*)&RS485_Node[i],infrm,SPORTA); //modify the content
                                           return;
                                      }
                                }
                                for(i=0;i<system.vibrator_num;i++){ //search the vibrator
                                      if(*(infrm+FRM_POS_FROM) == vibrator[i].addr){
                                           parse_node_frm((u8*)&vibrator[i],infrm,SPORTA); //modify the content
                                           return;
                                      }
                                }
                                return;
                        }        
                        //forward command        
                        if(*(infrm+FRM_POS_TO) == PC_ADDR){                         
                                cm_forward(infrm,SPORTPC);
                                return;
                        }
                }           
                return;
        }       
        /****************************************************************/
        // A complete frame has been received at PORT B
        /****************************************************************/
        if(RFlagB == RF_CKSUM)
        {             
                infrm = infrmB;
                if(infrm == (u8*)&ginfrm[SPORTB]){
                        use_infrm2Bin(B);
                }else{
                        use_infrmBin(B);
                }        
                RFlagB = RF_IDLE;
                if(((checksum(infrm,6)+checksum(infrm+FRM_POS_DBUF,*(infrm+FRM_POS_DLEN))) & 0xff) == *(infrm+FRM_POS_CKSUM)){   //cksum ok
                        if(*(infrm+FRM_POS_TO) == MY_ADDR){ 
                                for(i=0;i<system.node_num;i++){ //search the node         
                                      if(*(infrm+FRM_POS_FROM) == RS485_Node[i].addr){         
                                           parse_node_frm((u8*)&RS485_Node[i],infrm,SPORTB); //modify the content
                                           return;
                                      }
                                }
                                for(i=0;i<system.vibrator_num;i++){ //search the vibrator
                                      if(*(infrm+FRM_POS_FROM) == vibrator[i].addr){
                                           parse_node_frm((u8*)&vibrator[i],infrm,SPORTB); //modify the content
                                           return;
                                      }
                                }
                                return;
                        }        
                        //forward command        
                        if(*(infrm+FRM_POS_TO) == PC_ADDR){                         
                                cm_forward(infrm,SPORTPC);
                                return;
                        }
                }           
                return;
        }        
        /****************************************************************/
        // A complete frame has been received at PORT C
        /****************************************************************/
        if(RFlagC == RF_CKSUM)
        {             
                infrm = infrmC;
                if(infrm == (u8*)&ginfrm[SPORTC]){
                        use_infrm2Bin(C);
                }else{
                        use_infrmBin(C);
                }        
                RFlagC = RF_IDLE;
                if(((checksum(infrm,6)+checksum(infrm+FRM_POS_DBUF,*(infrm+FRM_POS_DLEN))) & 0xff) == *(infrm+FRM_POS_CKSUM)){   //cksum ok
                        if(*(infrm+FRM_POS_TO) == MY_ADDR){ 
                                for(i=0;i<system.node_num;i++){ //search the node         
                                      if(*(infrm+FRM_POS_FROM) == RS485_Node[i].addr){         
                                           parse_node_frm((u8*)&RS485_Node[i],infrm,SPORTC); //modify the content
                                           return;
                                      }
                                }
                                for(i=0;i<system.vibrator_num;i++){ //search the vibrator
                                      if(*(infrm+FRM_POS_FROM) == vibrator[i].addr){
                                           parse_node_frm((u8*)&vibrator[i],infrm,SPORTC); //modify the content
                                           return;
                                      }
                                }
                                return;
                        }        
                        //forward command        
                        if(*(infrm+FRM_POS_TO) == PC_ADDR){                         
                                cm_forward(infrm,SPORTPC);
                                return;
                        }
                }           
                return;
        }        
        /****************************************************************/
        // A complete frame has been received at PORT D
        /****************************************************************/
        if(RFlagD == RF_CKSUM)
        {             
                infrm = infrmD;
                if(infrm == (u8*)&ginfrm[SPORTD]){
                        use_infrm2Bin(D);
                }else{
                        use_infrmBin(D);
                }        
                RFlagD = RF_IDLE;
                if(((checksum(infrm,6)+checksum(infrm+FRM_POS_DBUF,*(infrm+FRM_POS_DLEN))) & 0xff) == *(infrm+FRM_POS_CKSUM)){   //cksum ok
                        if(*(infrm+FRM_POS_TO) == MY_ADDR){ 
                                for(i=0;i<system.node_num;i++){ //search the node         
                                      if(*(infrm+FRM_POS_FROM) == RS485_Node[i].addr){         
                                           parse_node_frm((u8*)&RS485_Node[i],infrm,SPORTD); //modify the content
                                           return;
                                      }
                                }
                                for(i=0;i<system.vibrator_num;i++){ //search the vibrator
                                      if(*(infrm+FRM_POS_FROM) == vibrator[i].addr){
                                           parse_node_frm((u8*)&vibrator[i],infrm,SPORTD); //modify the content
                                           return;
                                      }
                                }
                                return;
                        }        
                        //forward command        
                        if(*(infrm+FRM_POS_TO) == PC_ADDR){                         
                                cm_forward(infrm,SPORTPC);
                                return;
                        }
                }           
                return;
        }        


         
}

/**********************************************************************************************************/
// subroutine for node firmware upgrade 
/**********************************************************************************************************/
void nfu_process_node_feedback()
{
    u8 *infrm;
    if(RFlagD == RF_CKSUM)
    {             
       infrm = infrmD;
       if(infrm == (u8*)&ginfrm[SPORTD]){
          use_infrm2Bin(D);
       }else{
          use_infrmBin(D);
       }        
       RFlagD = RF_IDLE;
       if(((checksum(infrm,6)+checksum(infrm+FRM_POS_DBUF,*(infrm+FRM_POS_DLEN))) & 0xff) == *(infrm+FRM_POS_CKSUM)){   //cksum ok
          if(*(infrm+FRM_POS_TO) == MY_ADDR){ 
              nfu_parse_node_frm((u8*)&boot_comm,infrm,SPORTD); //modify the content
          }
       }           
    }        
}






/**********************************************************************************************************/
//
//  Frame receiving state machine (FRSM) called by UART interrupt. 
//  This subroutine is called every time when a byte has been received by UART.
//
/**********************************************************************************************************/
void cm_pushPC(u8 c)
{
#ifdef _TEST_UARTS_
     if(c=='0')
        led_off(0);
     if(c=='1')
        led_on(0);
     if(c=='F')
        led_flash(0);
#endif                      
      if(RFlagPC == RF_DATABUF)               {     //checksum found
                infrmPC[FRM_POS_CKSUM] = c;
                infrmPC[FRM_POS_HEAD2] = 0x68;                
                infrmPC[FRM_POS_HEAD1] = 0xfe;                
                RFlagPC = RF_CKSUM;                 //wait for the main program to handle it and clear the flag to IDLE                       
                return;
      }
      if(RFlagPC == RF_DATALEN)               {     //databuf found
                infrmPC[FRM_POS_DBUF+buf_cntPC] = c;
                buf_cntPC += 1;
                if(buf_cntPC >= infrmPC[FRM_POS_DLEN])
                        RFlagPC = RF_DATABUF;
                return;
      }
      if(RFlagPC == RF_CMD)                 {     //datalen found
                if(c <= CM_INBUF_MAX){
                        infrmPC[FRM_POS_DLEN] = c;
                        buf_cntPC = 0;
                        RFlagPC = RF_DATALEN;
                }       
                return;                         
      }
      if(RFlagPC == RF_ADDRTO)                 {     //cmd found
                infrmPC[FRM_POS_CMD] = c;
                RFlagPC = RF_CMD; 
                return;
      }      
      if(RFlagPC == RF_ADDRFROM)                 {     //addr_to found
                infrmPC[FRM_POS_TO] = c;
                RFlagPC = RF_ADDRTO; 
                return;
      }
      if(RFlagPC == RF_HEAD2)                 {     //addr_from found
                infrmPC[FRM_POS_FROM] = c;
                RFlagPC = RF_ADDRFROM;
                return;
      }
      if((RFlagPC == RF_HEAD1) && (c == 0x68)){     //head2 found
                RFlagPC = RF_HEAD2;
                return;
      }
      if((RFlagPC == RF_HEAD1) && (c == 0xfe)){     //double head1
                RFlagPC = RF_HEAD1;
                return;
      }
      if((RFlagPC == RF_IDLE) && (c == 0xfe)){      //first byte
                RFlagPC = RF_HEAD1;
                return;
      }                
      RFlagPC = RF_IDLE;
}

/**********************************************************************************************************/
//
//  Frame receiving state machine (FRSM) called by PORT A UART interrupt. 
//  This subroutine is called every time when a byte has been received by UART.
//
/**********************************************************************************************************/
void cm_pushA(u8 c)
{
#ifdef _TEST_UARTS_
     if(c=='0')
        led_off(0);
     if(c=='1')
        led_on(0);
     if(c=='F')
        led_flash(0);
#endif               

      if(RFlagA == RF_DATABUF)               {     //checksum found
                infrmA[FRM_POS_CKSUM] = c;
                infrmA[FRM_POS_HEAD2] = 0x68;                
                infrmA[FRM_POS_HEAD1] = 0xfe;                
                RFlagA = RF_CKSUM;                 //wait for the main program to handle it and clear the flag to IDLE                       
                return;
      }
      if(RFlagA == RF_DATALEN)               {     //databuf found
                infrmA[FRM_POS_DBUF+buf_cntA] = c;
                buf_cntA += 1;
                if(buf_cntA >= infrmA[FRM_POS_DLEN])
                        RFlagA = RF_DATABUF;
                return;
      }
      if(RFlagA == RF_CMD)                 {     //datalen found
                if(c <= CM_INBUF_MAX){
                        infrmA[FRM_POS_DLEN] = c;
                        buf_cntA = 0;
                        RFlagA = RF_DATALEN;
                }       
                return;                         
      }
      if(RFlagA == RF_ADDRTO)                 {     //cmd found
                infrmA[FRM_POS_CMD] = c;
                RFlagA = RF_CMD; 
                return;
      }      
      if(RFlagA == RF_ADDRFROM)                 {     //addr_to found
                infrmA[FRM_POS_TO] = c;
                RFlagA = RF_ADDRTO; 
                return;
      }
      if(RFlagA == RF_HEAD2)                 {     //addr_from found
                infrmA[FRM_POS_FROM] = c;
                RFlagA = RF_ADDRFROM;
                return;
      }
      if((RFlagA == RF_HEAD1) && (c == 0x68)){     //head2 found
                RFlagA = RF_HEAD2;
                return;
      }
      if((RFlagA == RF_HEAD1) && (c == 0xfe)){     //double head1
                RFlagA = RF_HEAD1;
                return;
      }
      if((RFlagA == RF_IDLE) && (c == 0xfe)){      //first byte
                RFlagA = RF_HEAD1;
                return;
      }                
      RFlagA = RF_IDLE;
}

/**********************************************************************************************************/
//
//  Frame receiving state machine (FRSM) called by PORT B UART interrupt. 
//  This subroutine is called every time when a byte has been received by UART.
//
/**********************************************************************************************************/
void cm_pushB(u8 c)
{
#ifdef _TEST_UARTS_
     if(c=='0')
        led_off(1);
     if(c=='1')
        led_on(1);
     if(c=='F')
        led_flash(1);
#endif               

      if(RFlagB == RF_DATABUF)               {     //checksum found
                infrmB[FRM_POS_CKSUM] = c;
                infrmB[FRM_POS_HEAD2] = 0x68;                
                infrmB[FRM_POS_HEAD1] = 0xfe;                
                RFlagB = RF_CKSUM;                 //wait for the main program to handle it and clear the flag to IDLE                       
                return;
      }
      if(RFlagB == RF_DATALEN)               {     //databuf found
                infrmB[FRM_POS_DBUF+buf_cntB] = c;
                buf_cntB += 1;
                if(buf_cntB >= infrmB[FRM_POS_DLEN])
                        RFlagB = RF_DATABUF;
                return;
      }
      if(RFlagB == RF_CMD)                 {     //datalen found
                if(c <= CM_INBUF_MAX){
                        infrmB[FRM_POS_DLEN] = c;
                        buf_cntB = 0;
                        RFlagB = RF_DATALEN;
                }       
                return;                         
      }
      if(RFlagB == RF_ADDRTO)                 {     //cmd found
                infrmB[FRM_POS_CMD] = c;
                RFlagB = RF_CMD; 
                return;
      }      
      if(RFlagB == RF_ADDRFROM)                 {     //addr_to found
                infrmB[FRM_POS_TO] = c;
                RFlagB = RF_ADDRTO; 
                return;
      }
      if(RFlagB == RF_HEAD2)                 {     //addr_from found
                infrmB[FRM_POS_FROM] = c;
                RFlagB = RF_ADDRFROM;
                return;
      }
      if((RFlagB == RF_HEAD1) && (c == 0x68)){     //head2 found
                RFlagB = RF_HEAD2;
                return;
      }
      if((RFlagB == RF_HEAD1) && (c == 0xfe)){     //double head1
                RFlagB = RF_HEAD1;
                return;
      }
      if((RFlagB == RF_IDLE) && (c == 0xfe)){      //first byte
                RFlagB = RF_HEAD1;
                return;
      }                
      RFlagB = RF_IDLE;
}

/**********************************************************************************************************/
//
//  Frame receiving state machine (FRSM) called by PORT C UART interrupt. 
//  This subroutine is called every time when a byte has been received by UART.
//
/**********************************************************************************************************/
void cm_pushC(u8 c)
{
#ifdef _TEST_UARTS_
     if(c=='0')
        led_off(2);
     if(c=='1')
        led_on(2);
     if(c=='F')
        led_flash(2);
#endif               

      if(RFlagC == RF_DATABUF)               {     //checksum found
                infrmC[FRM_POS_CKSUM] = c;
                infrmC[FRM_POS_HEAD2] = 0x68;                
                infrmC[FRM_POS_HEAD1] = 0xfe;                
                RFlagC = RF_CKSUM;                 //wait for the main program to handle it and clear the flag to IDLE                       
                return;
      }
      if(RFlagC == RF_DATALEN)               {     //databuf found
                infrmC[FRM_POS_DBUF+buf_cntC] = c;
                buf_cntC += 1;
                if(buf_cntC >= infrmC[FRM_POS_DLEN])
                        RFlagC = RF_DATABUF;
                return;
      }
      if(RFlagC == RF_CMD)                 {     //datalen found
                if(c <= CM_INBUF_MAX){
                        infrmC[FRM_POS_DLEN] = c;
                        buf_cntC = 0;
                        RFlagC = RF_DATALEN;
                }       
                return;                         
      }
      if(RFlagC == RF_ADDRTO)                 {     //cmd found
                infrmC[FRM_POS_CMD] = c;
                RFlagC = RF_CMD; 
                return;
      }      
      if(RFlagC == RF_ADDRFROM)                 {     //addr_to found
                infrmC[FRM_POS_TO] = c;
                RFlagC = RF_ADDRTO; 
                return;
      }
      if(RFlagC == RF_HEAD2)                 {     //addr_from found
                infrmC[FRM_POS_FROM] = c;
                RFlagC = RF_ADDRFROM;
                return;
      }
      if((RFlagC == RF_HEAD1) && (c == 0x68)){     //head2 found
                RFlagC = RF_HEAD2;
                return;
      }
      if((RFlagC == RF_HEAD1) && (c == 0xfe)){     //double head1
                RFlagC = RF_HEAD1;
                return;
      }
      if((RFlagC == RF_IDLE) && (c == 0xfe)){      //first byte
                RFlagC = RF_HEAD1;
                return;
      }                
      RFlagC = RF_IDLE;
}

/**********************************************************************************************************/
//
//  Frame receiving state machine (FRSM) called by PORT D UART interrupt. 
//  This subroutine is called every time when a byte has been received by UART.
//
/**********************************************************************************************************/
void cm_pushD(u8 c)
{
 #ifdef _TEST_UARTS_
     if(c=='0')
        led_off(3);
     if(c=='1')
        led_on(3);
     if(c=='F')
        led_flash(3);
#endif               

      if(RFlagD == RF_DATABUF)               {     //checksum found
                infrmD[FRM_POS_CKSUM] = c;
                infrmD[FRM_POS_HEAD2] = 0x68;                
                infrmD[FRM_POS_HEAD1] = 0xfe;                
                RFlagD = RF_CKSUM;                 //wait for the main program to handle it and clear the flag to IDLE                       
                return;
      }
      if(RFlagD == RF_DATALEN)               {     //databuf found
                infrmD[FRM_POS_DBUF+buf_cntD] = c;
                buf_cntD += 1;
                if(buf_cntD >= infrmD[FRM_POS_DLEN])
                        RFlagD = RF_DATABUF;
                return;
      }
      if(RFlagD == RF_CMD)                 {     //datalen found
                if(c <= CM_INBUF_MAX){
                        infrmD[FRM_POS_DLEN] = c;
                        buf_cntD = 0;
                        RFlagD = RF_DATALEN;
                }       
                return;                         
      }
      if(RFlagD == RF_ADDRTO)                 {     //cmd found
                infrmD[FRM_POS_CMD] = c;
                RFlagD = RF_CMD; 
                return;
      }      
      if(RFlagD == RF_ADDRFROM)                 {     //addr_to found
                infrmD[FRM_POS_TO] = c;
                RFlagD = RF_ADDRTO; 
                return;
      }
      if(RFlagD == RF_HEAD2)                 {     //addr_from found
                infrmD[FRM_POS_FROM] = c;
                RFlagD = RF_ADDRFROM;
                return;
      }
      if((RFlagD == RF_HEAD1) && (c == 0x68)){     //head2 found
                RFlagD = RF_HEAD2;
                return;
      }
      if((RFlagD == RF_HEAD1) && (c == 0xfe)){     //double head1
                RFlagD = RF_HEAD1;
                return;
      }
      if((RFlagD == RF_IDLE) && (c == 0xfe)){      //first byte
                RFlagD = RF_HEAD1;
                return;
      }                
      RFlagD = RF_IDLE;
}

