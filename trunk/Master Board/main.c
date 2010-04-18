/*****************************************************
This program was produced by the
CodeWizardAVR V1.24.6 Professional
Automatic Program Generator
?Copyright 1998-2005 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com
e-mail:office@hpinfotech.com

Project : 
Version : 
Date    : 2007-1-15
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega64
Program type        : Application
Clock frequency     : 14.745600 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 1024
*****************************************************/

#include <mega64.h>
#include "uart.h"
#include "init.h"
#include "16c554.h"
#include "global.h"
#include "command.h"
static u16 rst554count = 0;
//master board reg_addr = n2m_map[ node_reg_addr]
flash u8 n2m_map[NREG_SIZE_TOTAL] = {0           ,1           ,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,
                                     MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,
                                     MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,
                                     MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,
                                     MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,2           ,3           ,4           ,
                                     5           ,6           ,7           ,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,
                                     MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,
                                     MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID,MREG_INVALID}; 

//regid to indicate port is waiting for data
extern u8 RWait[]; 

u8 true_val = 1;
u8 false_val = 0;

//current search node address on each port
static u8 port_node[SPORT_TOTAL];	
// Current timeout limit for each port
static u16 port_timeout[SPORT_TOTAL];	
//"has_node" indicates which port/group this node belongs to   
static u8 has_node = 0; //indicate of distribution of node on each group and port
static u16 loopcnt = 0; //tick of main loop
//handle forward of command and some reg read command

//examine the flag A is a cmd flag or state flag
#define is_cmd_flag(A)  (A != STATE_BEIDLE) && (A != STATE_DONE_FAIL) && (A != STATE_DONE_OK)
/**************************************************************************************/
//                      Timer 1 overflow interrupt service routine
/**************************************************************************************/
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Place your code here
}

/**************************************************************************************/
//                              Global Variable
/**************************************************************************************/
NODE RS485_Node[MAX_NODE_NUM];                       // RS485节点信息及参数设置 
NODE vibrator[MAX_VIBR_NUM];                         // 主振动机
SYSTEM system;                                       // 系统参数设置

 
#define SUCCEED	1
#define FAILED  0

/**************************************************************************************/
//                               Watchdog
// Enable watchdog timer. 
// Timeout limit WDTCR[2:0]: 
// 000 - 16.3ms    001 - 32.5ms    010 - 65ms    011 - 0.13s
// 100 - 0.26ms    101 - 0.52s     110 - 1.0s    111 - 2.1s  
/**************************************************************************************/
#define WDCE 4
#define WDE 3
void Turn_on_Watchdog()
{ 
  #asm("cli")
  #asm("wdr") // reset watchdog timer to avoid timeout reset
  WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
  WDTCR = 0x08; // turn on watch dog timer: WDE = 1, WDP = 000, 16.3ms.
  #asm("sei") // Enable global interrupt.
}

/**************************************************************************************/
// Pretending to be pc and send query of byte(regid) to nodeid until it is sent out or 
// timeout
/**************************************************************************************/
u8 pc_readb_until_return(u8 regid, u8 nodeid)
{
    u16 timeout;
    pc_query(RS485_Node[nodeid].addr, regid, 1, RS485_Node[nodeid].uart_port);
    nodeid = RS485_Node[nodeid].uart_port;
    //wait until time out or acknowledgement is received.
    timeout = 0x400; //1024
    
    RWait[nodeid] = regid;
    while((RWait[nodeid] != R_NOWAIT) && (timeout != 0)){
        cm_process();
        timeout--;     //wait for reply or time out   
    }                   
    RWait[nodeid] = R_NOWAIT;
    return (timeout > 0)? SUCCEED : FAILED;
}         
u8 pc_readb_until_return2(u8 regid, u8 nodeid)
{
    u16 timeout;
    pc_query(vibrator[nodeid].addr, regid, 1, vibrator[nodeid].uart_port);
    nodeid = vibrator[nodeid].uart_port;
    RWait[nodeid] = regid;
    //wait until time out or acknowledgement is received.
    timeout = 0x400; //1024

    while((RWait[nodeid] != R_NOWAIT) && (timeout != 0)){
        cm_process();
        timeout--;     //wait for reply or time out   
    }            
    RWait[nodeid] = R_NOWAIT;
    return (timeout > 0)? SUCCEED : FAILED;
}
/**************************************************************************************/
//query byte (regid) on port until it returns or timeout                                
/**************************************************************************************/
u8 readb_until_return(u8 port, u8 regid, u8 nodeid){
        u16 timeout;
        RWait[port] = regid;
        cm_query(RS485_Node[nodeid].addr,regid,1,RS485_Node[nodeid].uart_port);//old one is send_query_b(regid,nodeid);
        timeout = 0x400; //1024
        while((RWait[port] == regid) && (timeout != 0)){
                cm_process();
                timeout--;     //wait for reply or time out 
        }       
        RWait[port] = R_NOWAIT;                   
        return (timeout > 0)? SUCCEED : FAILED;
}


/**************************************************************************************/
////////////////////////////////////////////
//      board init related function.
////////////////////////////////////////////
//regroup the identified nodes	, 
//all the nodes will stay in RS485_Node[i] after search
//we need to move them to vibrator or put into the RS485_Node[i] , by index (fill the first n position) while not by address
/**************************************************************************************/
void regroup_node()
{          
    u8 i;      
    system.vibrator_num = 0; 
    system.node_num = 0;

    for(i=0;i < MAX_NODE_NUM; i++){                                               
        if((RS485_Node[i].board_property & BOARD_TYPE_MASK) == BOARD_TYPE_INVALID)
                continue;

        if((RS485_Node[i].board_property & BOARD_GROUP_MASK) == BOARD_GROUP_NONE){  //uninitialized board, identified as weight board and in group A  
                //todo: handle initial board. (should set the address and write to eeprom automatically.
                RS485_Node[i].board_property = BOARD_TYPE_WEIGHT | BOARD_GROUP_A;
        }
        if((RS485_Node[i].board_property & BOARD_TYPE_MASK ) == BOARD_TYPE_VIBRATE){//如果是主振机节点：移动到主振机参数区
                vibrator[system.vibrator_num].status = RS485_Node[i].status;
	        vibrator[system.vibrator_num].addr = RS485_Node[i].addr;
                vibrator[system.vibrator_num].board_property = RS485_Node[i].board_property;
		vibrator[system.vibrator_num].uart_port = RS485_Node[i].uart_port;
		RS485_Node[i].board_property = BOARD_TYPE_INVALID|BOARD_GROUP_NONE;
	   	RS485_Node[i].addr = 0x79;
		system.vibrator_num++;
		continue;            
        }
        if((RS485_Node[i].board_property & BOARD_TYPE_MASK) == BOARD_TYPE_WEIGHT){  //如果是称重板  
                if(i != system.node_num){     //no more action for just in place case. otherwise exchange it with the item in place                        
       		   		RS485_Node[system.node_num].status = RS485_Node[i].status;
			   	RS485_Node[system.node_num].addr = RS485_Node[i].addr;
	   			RS485_Node[system.node_num].board_property = RS485_Node[i].board_property;
	   			RS485_Node[system.node_num].uart_port = RS485_Node[i].uart_port;
	   			RS485_Node[i].board_property = BOARD_TYPE_INVALID|BOARD_GROUP_NONE;
		   		RS485_Node[i].addr = 0x79;
                }
		system.node_num++;  
		continue;
        }                
    }                                                                  
}                  

/**************************************************************************************/
// just send query command to all the node from 1 to MAX_NODE_NUM during identification
// process
/**************************************************************************************/
void query_port(u8 port)
{          
    u8 i;                                               
    for(i=1;i <= MAX_NODE_NUM; i++)//send command to RS485 node one by one
    {  
        if((RS485_Node[i-1].board_property & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID)     //acquire node property      
                continue;       //it has been identified
        RS485_Node[i-1].uart_port = port;
        if (SUCCEED == readb_until_return(port,NREG_BOARD,i-1)){
               readb_until_return(port,NREG_STATUS,i-1);
        }
    }
} 
/**************************************************************************************/
//just set the target and offset and running variables
/**************************************************************************************/
void init_var()
{                          
    u8 i;
    // Initialize system configuration 
    for(i=0; i< MAX_VIBR_NUM;i++){
        system.target_weight[i] = 200;                	// system target weight of goods  
        system.offset_up_limit[i] = 20;               	// offset upper limit
        system.offset_lo_limit[i] = 20;               	// offset lower limit
        system.running[i] = 0;
    }
    for(i=SPORTA; i <= SPORTD;i++){
        port_node[i] = 0xff;								//port node id that is waiting
        port_timeout[i] = 0;        					//port timeout count
        system.flag_goon[i] = STATE_DONE_OK;  			//flag to sent go on command
        system.flag_start_machine[i] = STATE_DONE_OK;   //Flag to start machine.
        system.flag_stop_machine[i] = STATE_DONE_OK;    //Flag to stop machine. 
        system.flag_search[i] = STATE_DONE_OK;          //Flag to gather weight.
    }
	// Initialize node configuration
    for(i=0; i<MAX_NODE_NUM; i++)
    {
    	system.vibrator_num = 0;
	system.node_num = 0;
	RS485_Node[i].addr=0x79;                     		// invalid RS485 address
	RS485_Node[i].board_property = BOARD_TYPE_INVALID | BOARD_GROUP_NONE;       // invalid Board
	RS485_Node[i].fail_counter = 0;             	// how many time the node failed to be selected
    }	
    for(i=0; i<MAX_VIBR_NUM; i++)
    {
	vibrator[i].addr=0x80;                     // RS485 address beyond the max_node_num
	vibrator[i].board_property = BOARD_TYPE_INVALID | BOARD_GROUP_NONE; // General Purpose Board
	vibrator[i].fail_counter = 0;             // how many time the node failed to be selected
    }
}
/**************************************************************************************/
void find_nodes(void)
/**************************************************************************************/
{
    u8 i;
    system.vibrator_num = MAX_VIBR_NUM;                  // how many vibrator node (head) in this system
    system.node_num = MAX_NODE_NUM;                      // how many weight node (head) in this system

  // Initialize node parameter buffer
    for(i=0; i<MAX_NODE_NUM; i++)
    {   
        RS485_Node[i].Mtrl_Weight_gram = 0xffff;
        RS485_Node[i].Mtrl_Weight_decimal = 0;
        RS485_Node[i].addr=i+1;                     	// RS485 address starting from 1
	RS485_Node[i].board_property = BOARD_TYPE_INVALID | BOARD_GROUP_NONE;       // invalid Board
	RS485_Node[i].fail_counter = 0;             	// how many time the node failed to be selected
    }
    for(i=0; i<MAX_VIBR_NUM; i++)
    {
	vibrator[i].addr=0x80;                     
	vibrator[i].board_property = BOARD_TYPE_INVALID | BOARD_GROUP_NONE; // General Purpose Board
	vibrator[i].fail_counter = 0;             
    }
    
    // start to inqury RS485 nodes. Initilize node number first    
    query_port(SPORTA);        
    query_port(SPORTB);        
    query_port(SPORTC);        
    query_port(SPORTD);

    regroup_node();

    //has_node = bit 7-4 : whether there is node in grp 3-0 : 0/1
    //         = bit 3-0 : whether there is node in SPORTD-SPORTA : 0/1
    for(i = 0;i < MAX_NODE_NUM;i++){
               if((RS485_Node[i].board_property & BOARD_TYPE_MASK) !=  BOARD_TYPE_INVALID){
                    has_node = has_node |  (1 << (RS485_Node[i].board_property & BOARD_GROUP_MASK_SHORT)); //including missing board
                    has_node = has_node |  (0x10 << (RS485_Node[i].uart_port));                    
               }                                                                                    
    }
    for(i = 0;i < MAX_VIBR_NUM;i++){
               if((vibrator[i].board_property & BOARD_TYPE_MASK) !=  BOARD_TYPE_INVALID){
                    has_node = has_node |  (1 << (vibrator[i].board_property & BOARD_GROUP_MASK_SHORT)); //including missing board
                    has_node = has_node |  (0x10 << (vibrator[i].uart_port));                                        
               }                                                                                    
    }
}
/**************************************************************************************/
//                              Main Function Loop
/**************************************************************************************/
extern u16 packer_config;
void cmd_loop(u8 grp){     
       u8 i; 
       static u8 signal_to_packer_pending;                 
       cm_process();                                   
       /**************************************************************************************/
       // Command to stop nodes.
       // if we receive a stop_machine command, we need to make sure the following is well handled
       // before we stop the machine:
       // (1) In the case of multiple-release-single-pack mode, if we haven't complete packer feeding
       //     tasks, we need to complete all the release actions (one cycle) and send out release-done
       //     signal before stopping the machine.        
       /**************************************************************************************/
       if(is_cmd_flag(system.flag_stop_machine[grp])){   
           for(i=0;i<system.node_num;i++){  
              if((RS485_Node[i].board_property & BOARD_GROUP_MASK) == grp)	//not including missing board
                unset_cmd(NREG_ENABLE,i);    //enable = 0
                set_cmd(NREG_RELEASE,i);     //release = 1
           }           
           for(i=0;i<system.vibrator_num;i++){                                         
              if((vibrator[i].board_property  & BOARD_GROUP_MASK) == grp)	//not including missing board
                vunset_cmd(NREG_ENABLE,i);   //enable = 0          
                vset_cmd(NREG_RELEASE,i);   //release = 1
           }       
           system.running[grp] = 0;
           cm_report_b(MY_ADDR, (u8)((u8*)&system.running[grp]-(u8*)&system), 1, (u8*)&system.running[grp], SPORTPC);                      
           sleepms(400); //wait until all the release is done
           system.flag_stop_machine[grp] = STATE_DONE_OK;    //set the flag that the cmd has been done           
           return;   
       }
       
       /**************************************************************************************/
       // command to go on nodes (feed or release)
       // if single bucket weight is greater than 1/16 of target weight(unit: 0.1 gram), don't feed (go on) 
       // anymore. If an overweight bucket receiving a combination release command, it will also
       // need "go-on" command to release, ioex will send goon command directly to that node.
       /**************************************************************************************/       
       if(is_cmd_flag(system.flag_goon[grp])){   // go on command   
           for(i=0;i<system.node_num;i++){  
              if((RS485_Node[i].board_property & BOARD_GROUP_MASK) == grp)
              {     
                  //if(system.running[grp] && (RS485_Node[i].Mtrl_Weight_gram < system.target_weight[grp]>>4) ) { 
                  //if(system.running[grp] && (RS485_Node[i].Mtrl_Weight_gram < (system.target_weight[grp]>>2+system.target_weight[grp]>>3))){	//running 0.25+0.125 = 0.375 near 30%
                  //    set_cmd(NREG_GOON,i);   //overweight case is handled by pc side
                  //}                                                              
                  RS485_Node[i].Mtrl_Weight_gram = 0xffff; //clear the weight for next search.                                
                  cm_process();
              }          
           }                                         
           system.flag_goon[grp] = STATE_DONE_OK; //set the flag that the cmd has been done                      
           system.flag_search[grp] = STATE_BEIDLE; //trigger another round of weight gathering
           return;
       }     
       /**************************************************************************************/
       // command to start machine 
       /**************************************************************************************/        
       if(is_cmd_flag(system.flag_start_machine[grp]))
       {                         
            /******************************************************/
            // CMD_PACKER_INIT
            /******************************************************/
            if(system.flag_start_machine[grp] == CMD_PACKER_INIT)	//download setting of packer
            {    //parameter is stored in system.packer_config1
                packer_config = system.packer_config1; 
                Init_interface();
                system.flag_start_machine[grp] = STATE_BEIDLE;
                return;
            }                                                    

            /******************************************************/
            //
            /******************************************************/
            if(system.flag_start_machine[grp] == CMD_PACKER_BUSY)	//query status of packer busy
            {    
                system.flag_start_machine[grp] = (Packer_Is_Busy() == 0x0) ? STATE_DONE_OK : STATE_BEIDLE;
                return;
            }                                                    
            /******************************************************/
            // CMD_PACKER_AVAILABLE
            /******************************************************/
            if(system.flag_start_machine[grp] == CMD_PACKER_AVAILABLE)	//query status of packer busy
            {                                    
                Tell_Packer_Weigher_Rdy();
                if((NEED_HANDSHAKE) && Packer_Is_Busy())
                {   system.flag_start_machine[grp] = STATE_BUSY;
                }
                else                       
                       system.flag_start_machine[grp] = STATE_BEIDLE;
                return;
            }                   

            /******************************************************/
            // CMD_PACKER_DONE
            // sometimes we need to delay for a while before sending out
            // release_done signal to packer. We kickoff timer 0 service 7 in 
            // Tell_Packer_Release_Done() for the delay to expire. 
            // IOEX (PC side) only send CMD_PACKER_DONE once and then wait until 
            // system.flag_start_machine[grp] = STATE_BEIDLE.
            // So we add a control switch "signal_to_packer_pending" to 
            // force Tell_Packer_Release_Done() to run until delay expires AND
            // release_done signal is sent. By doing this way, the advantage is we
            // don't need to stay in Tell_Packer_Release_Done().
            /******************************************************/
            if((system.flag_start_machine[grp] == CMD_PACKER_DONE)|| signal_to_packer_pending)	//material release is done
            {   
               if(Tell_Packer_Release_Done())  /* need delay*/
               {   system.flag_start_machine[grp] = STATE_BUSY; 
                   signal_to_packer_pending = 0xff;
               }
               else
               {   system.flag_start_machine[grp] = STATE_BEIDLE;  
                   signal_to_packer_pending = 0;
               }                             
               return;
            } //*/                   

            /******************************************************/
            // CMD_START_SEARCH
            /******************************************************/		  
            if(system.flag_start_machine[grp] == CMD_START_SEARCH)	//research all the nodes
            {	
			   init_var();
			   find_nodes();
			   system.flag_start_machine[grp] = STATE_BEIDLE;
			   return;
            }

            /******************************************************/
            // CMD_RESET_16C554
            /******************************************************/
            if(system.flag_start_machine[grp] == CMD_RESET_16C554)
            {
	                Init_554();                                          
		        system.flag_start_machine[grp] = STATE_BEIDLE;
       	                cm_report_b(MY_ADDR, (u8)((u8*)&system.flag_start_machine[grp]-(u8*)&system), 1, (u8*)&system.flag_start_machine[grp], SPORTPC); 
		        return;
            }           

            /******************************************************/
            // CMD_REBOOT_ME
            /******************************************************/
            if(system.flag_start_machine[grp] == CMD_REBOOT_ME)
            {             
               Turn_on_Watchdog();
               while(1);   /* wait for watchdog reset */                 		        
            }                                          

            /******************************************************/
            // CCMD_START_MACHINE
            /******************************************************/
            if(system.flag_start_machine[grp] == CMD_START_MACHINE)
            {		   
                        for(i=0;i<system.node_num;i++){  
                                if((RS485_Node[i].board_property & BOARD_GROUP_MASK) == grp){ //not including missing board
                                      set_cmd(NREG_ENABLE,i);     //enable = 1                      
                                }
                        }             
                        for(i=0;i<system.vibrator_num;i++){  
                                if((vibrator[i].board_property & BOARD_GROUP_MASK) == grp)	//not including missing board
                                      vset_cmd(NREG_ENABLE,i);    //enable = 1
                        }             
                        system.running[grp] = 1;
                        cm_report_b(MY_ADDR, (u8)((u8*)&system.running[grp]-(u8*)&system), 1, (u8*)&system.running[grp], SPORTPC);                                 
                        system.flag_start_machine[grp] = STATE_DONE_OK; //set the flag that the cmd has been done
                        return;
            }
       } 
}
/**************************************************************************************/
//one round of gathering weight                                                         
/**************************************************************************************/
void search_loop(int grp)
{
       u8 i,port;
       cm_process();
       /***********************************************************************/
       //
       /***********************************************************************/
       if(system.flag_search[grp] == STATE_BEIDLE){   //is idle , means trigger another round of weight gathering   
           if(loopcnt%2 == 1){	//take half chance to collect the weight
                return;
           }
           for(i=0;i<system.node_num;i++){  
                if((RS485_Node[i].board_property & BOARD_GROUP_MASK_SHORT) == grp){   //including missing board
                     RS485_Node[i].Mtrl_Weight_gram = 0xffff;     //clear the weight to unavailable 
                }
           }
           system.flag_search[grp] = 1; //start to gather weight from node 1 (position in array, not address). flag_search will store the board_id we are searching from now on
       }
       /***********************************************************************/
       // Multi entries here: query one node every entry. 
       // Eventually all nodes will be queried.
       /***********************************************************************/
       if((system.flag_search[grp] >=1) && (system.flag_search[grp] < STATE_DONE_OK)){ //during search of some board
           for(i=0;i<system.node_num;i++){             //search whether all the data has been collected.    
		if((RS485_Node[i].board_property & BOARD_TYPE_MASK) != BOARD_TYPE_WEIGHT)		
		     continue;
           	if((RS485_Node[i].board_property & BOARD_GROUP_MASK_SHORT) != grp) //not in this group //check the missing board too
	      	     continue;
		port = RS485_Node[i].uart_port;
		if(RWait[port] != R_NOWAIT) //still waiting
		     return;
                if(RS485_Node[i].Mtrl_Weight_gram != 0xffff)	//weight has been got
		     continue;
	        port_node[port] = i;
                RWait[port] = NREG_STATUS;
                port_timeout[port] = 0x512;    
	        cm_query(RS485_Node[i].addr,NREG_WEIGHT,4,port); //old command is send_query_l(REG_WEIGHT,i);                
	        system.flag_search[grp] = RS485_Node[i].addr; 
	        cm_process();
	        return;
           }                        
	   /***********************************************************************/
	   // The following will be executed only when all weight has been queried   
	   /***********************************************************************/
           port = 0;
           for(i=0;i<system.node_num;i++){  
              if((RS485_Node[i].board_property & BOARD_GROUP_MASK_SHORT) == grp){ //including the missing board
                   if((RS485_Node[i].Mtrl_Weight_gram == MY_INVALID_DATA) || 
                        (RS485_Node[i].Mtrl_Weight_gram == AD_INVALID_DATA) || 
                        (RS485_Node[i].Mtrl_Weight_gram == AD_BUSY) || 
                        (RS485_Node[i].Mtrl_Weight_gram == AD_FILTER))
                   {                                   
                        RS485_Node[i].Mtrl_Weight_gram = 0xffff;
                        port = port | 0x01;
                   } else                   {
                        port = port | 0x10; 
                   }
              }
           }
           /***********************************************************************/
           // check data validation flag, 0x01: all data are invalid
           /***********************************************************************/                     
           if(port == 0x01)          
           {     
                if(rst554count++ > 3)
                {
                        rst554count = 0;
                        LED_FLASH(LED_RST554);   
//                      Init_554();          
                        port = UC_554D_LSR;
                        port = UC_554D_MSR;
                        port = 0x01;
                }
                
                 for(i=0;i<system.node_num;i++){  
                        if((RS485_Node[i].board_property & BOARD_GROUP_MASK_SHORT) == grp){   //including missing board
                           RS485_Node[i].Mtrl_Weight_gram = 0xffff;     //clear the weight to unavailable 
                        }
                }
                system.flag_search[grp] = 1;                          
                return;
           }
           /***********************************************************************/
           // check data validation flag. 0x11: got something and failed something
           /***********************************************************************/
           if(port == 0x11)     
           {
                 for(i=0;i<system.node_num;i++){  
                        if((RS485_Node[i].board_property & BOARD_GROUP_MASK_SHORT) == grp){   //including missing board
                           RS485_Node[i].Mtrl_Weight_gram = 0xffff;     //clear the weight to unavailable 
                        }
                }
                system.flag_search[grp] = 1;                          
                return;
           }           
           
           /***********************************************************************/
           // The following will be executed only when all data are valid                                                     
           /***********************************************************************/
           for(i=0;i<system.node_num;i++){  
              if((RS485_Node[i].board_property & BOARD_GROUP_MASK_SHORT) == grp){ //including the missing board
                   if((loopcnt%10 == 0)){     //weigh_gram, weigh_decimal, status, cs_status, hw_status will be reported.     
                        cm_report_b(RS485_Node[i].addr, NREG_WEIGHT, 6, (u8*)&RS485_Node[i].Mtrl_Weight_gram, SPORTPC);                                   
                   }else{ //just weight and status is reported.
			cm_report_b(RS485_Node[i].addr, NREG_WEIGHT, 4, (u8*)&RS485_Node[i].Mtrl_Weight_gram, SPORTPC);                                   
                   }
                   cm_process();
              }
           }                      
           system.flag_search[grp] = STATE_DONE_OK;                          
       }
}
/**************************************************************************************/
//just hand the timeout case on port waiting
/**************************************************************************************/
void port_loop(u8 port)
{                                
       u8 i;          
                       
       i=port_node[port]; //current querying node id;          
       if(i == 0xff)
                return;
       if(RS485_Node[i].uart_port != port) //node not on this port
                return;                                    
       if(RWait[port] == R_NOWAIT){ //not waiting any command (ex: weight)
               if(port_timeout[port] == 0)
                        return;
                //get response in time    
                /*
				   if(RS485_Node[i].fail_counter >= 10) //missing node
				   {
				   	//missing board founded again
				   	if(system.running[RS485_Node[i].board_property & BOARD_GROUP_MASK_SHORT])
			  	        {
		                		set_cmd(NREG_ENABLE,i);    //enable = 1
					}else{
                				unset_cmd(NREG_ENABLE,i);    //enable = 0
				   	}
				   }
                */
		RS485_Node[i].fail_counter = 0;
		port_timeout[port] = 0; 
       }else{    
               if(port_timeout[port] != 0){  //tick the timeout count
                       port_timeout[port]--;
               	       return;
               }                            
	       //time out, so increment the fail_count and move to the next node;
//               if(RS485_Node[i].fail_counter++ <){ 
//			++RS485_Node[i].fail_counter;         
//			sleepms(200);
//               }else{
                        RS485_Node[i].Mtrl_Weight_gram = MY_INVALID_DATA; //mark with an invalid weight
//                        RS485_Node[i].fail_counter = 0;
//               }
               RWait[port] = R_NOWAIT;     
       }                                  
       port_node[port] = 0xff;
}                 
/**************************************************************************************/
//
/**************************************************************************************/
void report_loop(){  
       u8 i,j;
       
       cm_process();
       if(system.flag_report == MY_ADDR){ //report system board information
                for(i=0;i<sizeof(system);i++){ 
                        cm_report_b(MY_ADDR, i, 1, ((u8*)&system) + i, SPORTPC);           
                }                           
                system.flag_report = STATE_DONE_OK;
       }else{                      
                for(i=0;i<system.vibrator_num;i++){  //search through vibrator board
                         if(vibrator[i].addr == system.flag_report){  
                                for(j=0; j< (NREG_SIZE_TOTAL);j++){
                                        if(pc_readb_until_return2(j,i)){
                                                cm_process();                                                       
                                        };
                                        sleepms(100); //why we need this delay?
                                }
                                system.flag_report = STATE_DONE_OK;
                                return;
                         }
                }               
                for(i=0;i<system.node_num;i++){  
                         if(RS485_Node[i].addr == system.flag_report){  
                                for(j=0; j< (NREG_SIZE_TOTAL);j++){
                                        if(pc_readb_until_return(j,i)){
                                                cm_process();                                                       
                                        };
                                        sleepms(100); //why we need this delay?
                                }
                                break;
                         }
                }                
                if(i >= system.node_num)
                        system.flag_report = STATE_DONE_FAIL;	//the node is not existed 
                else
                        system.flag_report = STATE_DONE_OK;                
       }
}

/**************************************************************************************/
//
/**************************************************************************************/
void main(void)
{
    
    u16 tick = 0;
    system.flag_report = MY_ADDR;                //will be set to STATE_DONE_OK once node search is done.
    // RS485 Node    
    init_var();	//init data structure 
    //init IO bin
    use_infrmBin(A);    
    use_infrmBin(B);    
    use_infrmBin(C);    
    use_infrmBin(D);    
    use_infrmBin(PC);
    
    // System Initialization
    Init_Port();
    Init_Timers();
    Init_Ex_Interrupt();
    Init_ADC();
    Init_UART();  
    Enable_XMEM();
    Init_554();
    // Global enable interrupts
    #asm("sei")                 
    /*********************************************************************/
    // System hardware dection
    // Search for available nodes and save node info into node buffer
    /*********************************************************************/
    // not sure if RS485 nodes are ready to accept cmd, 
    // Master board delays for some time here. otherwise first several cmds
    // may be lost.    
    // intialize LED. 
    PORTB.7=1;
    PORTB.6=1;
    PORTB.5=1;
    PORTB.4=1;
    PORTD.7=1;
    PORTD.6=0; 
    sleepms(800); //wait until all the node is ready after power up    
    find_nodes(); //query regroup all nodes;
    system.flag_report = STATE_DONE_OK;	//search action is done
    
    //intialize packer interface
    packer_config = 0x00; // 0xa8 IR OR/OF falling edge, with handshake, mem
    Init_interface(); 
         
    //loop of each group;
    while(1)
    {
 //       if(system.flag_search[0] == STATE_DONE_OK)
 //	                system.flag_goon[0] = 1;
      
	if(loopcnt & 0x8000)	    
	{
		loopcnt = 0;
	}
       loopcnt++;
       if(is_cmd_flag(system.flag_report))//report command = board id (1..36 or 0xff will be valid)
           report_loop(); //doing report
                                           

       if(has_node & 0x01)                    {search_loop(BOARD_GROUP_A);cmd_loop(BOARD_GROUP_A);};
       if(has_node & 0x10)                    port_loop(SPORTA);

       if(has_node & 0x02)                    {search_loop(BOARD_GROUP_B);cmd_loop(BOARD_GROUP_B);};
       if(has_node & 0x20)                    port_loop(SPORTB);       

       if(has_node & 0x04)                    {search_loop(BOARD_GROUP_C);cmd_loop(BOARD_GROUP_C);};             
       if(has_node & 0x40)                    port_loop(SPORTC);       

       if(has_node & 0x08)                    {search_loop(BOARD_GROUP_D);cmd_loop(BOARD_GROUP_D);};
       if(has_node & 0x80)                    port_loop(SPORTD);


       cm_process(); 
       
       // LED to indicate code is running. 
       if(++tick>2000)
       {
         //LED_FLASH(LED_RUN);       
         tick=0;
#ifdef TEST_SERIAL_PORTS
         putchar('C');
         putchar('O');
         putchar('M');
         putchar('1');
         d_putchar('C');
         d_putchar('O');
         d_putchar('M');
         d_putchar('0');
#endif         
       }
       // LED logic ends.
    }
}      
/*******************************************/
/*      debug related function             */
/*******************************************/
void push_debug(u8 data)
{               
        u8 i;
        if(data == 'L'){                      
            d_putstr("system :");
            d_mputs((u8*)&system,sizeof(SYSTEM));
            d_putstr("\r\n");
            sleepms(5000);
            for(i = 0;i < MAX_NODE_NUM;i++){      
               if((RS485_Node[i].board_property & BOARD_TYPE_MASK) ==  BOARD_TYPE_INVALID)        continue;
                d_putstr("node ");  d_putchar2(i);  d_putstr(":");
                 d_mputs((u8*)&RS485_Node[i],sizeof(NODE));
                d_putstr("\r\n");   sleepms(5000);
            }
            for(i = 0;i < MAX_VIBR_NUM;i++){        
                if((vibrator[i].board_property & BOARD_TYPE_MASK) ==  BOARD_TYPE_INVALID)        continue;
                d_putstr("vibr ");  d_putchar2(i);   d_putstr(":");
                d_mputs((u8*)&vibrator[i],sizeof(NODE));
                d_putstr("\r\n");    sleepms(5000);                
            }                   
            for(i = 0; i < (SPORTD+1);i++){
                d_putstr("port_node "); d_putchar2(i);d_putstr(":");
                d_mputs((u8*)&port_node[i],sizeof(u8));
                d_putstr("\r\n");    sleepms(5000);                

                d_putstr("port_tout "); d_putchar2(i);d_putstr(":");
                d_mputs((u8*)&port_timeout[i],sizeof(u16));
                d_putstr("\r\n");    sleepms(5000);                

                d_putstr("port_wait "); d_putchar2(i);d_putstr(":");
                d_mputs((u8*)&RWait[i],sizeof(u8));
                d_putstr("\r\n");    sleepms(5000);                

            }                   
            d_putstr("hasnode : ");
            d_mputs((u8*)&has_node,sizeof(u8));
            d_putstr("\r\n");    sleepms(5000);  
            return;       
        }       
                
        if(data == 'R'){
                d_putstr("\r\nrelease node 0 (index not addr)");
                set_cmd(NREG_RELEASE,0);                
                return;                
        }

        if(data == 'G'){
                d_putstr("\r\ngo on node 0 (index not addr)");
                set_cmd(NREG_GOON,0);                
                return;                
        }
        if(data == 'M'){
                d_putstr("\r\ndisable node 0 (index not addr)");
                unset_cmd(NREG_ENABLE,0);
                return;
        }
        if(data == 'N'){
                d_putstr("\r\nenable node 0 (index not addr)");
                set_cmd(NREG_ENABLE,0);                      
                return;        
        }                                                                              
        if(data == 'Q'){    
                init_var(); 
                find_nodes(); 
                d_putstr("\rquery nodes done");
                return;
        }                      
        if(data == 'S'){                        
                d_putstr("\rstart machine\r\n");
                system.flag_start_machine[0] = 0x10;
                system.flag_start_machine[1] = 0x10;
                system.flag_start_machine[2] = 0x10;                
                system.flag_start_machine[3] = 0x10;        
                return;
        }   
        if(data == 'P'){        
                d_putstr("\rreport node 24(0x18)\r\n");
                system.flag_report = 24;
                return;
        }               
        if(data == 'O'){
                d_putstr("\rgoon group 0\r\n");
                system.flag_goon[0] = 1;
                return;
        }              
        if(data == 'H'){
                d_putstr("\rsearch group 0\r\n");
                system.flag_search[0] = 1;
                return;
        }                                            
                    

        d_putstr("L:list all the register\r\n");
        d_putstr("M:disable node\r\nN:enable node\r\nQ:query nodes\r\nG:go on\r\nP:report node 24\r\nO:goon group 0\r\nH:search group0");

}
/**************************************************************************************/
//
/**************************************************************************************/
void debug_getc(u8 data){
        static u8 dtpos = 0;
        static u8 dtval = 0;
        static u8 dtvah = 0;   
        if(dtpos == 0)  { // high byte recieved. 
                dtvah = ishexchar(data);
                if(dtvah != 0xff)
                        dtpos = 1;
        }else{
                dtval = ishexchar(data);
                if(dtval != 0xff){
                        d_putchar((dtvah << 4)|dtval);
                        cm_pushPC((dtvah << 4)|dtval);
                }
                dtpos = 0;
        }        
} 

/**************************************************************************************/
//
/**************************************************************************************/                                   
void debug(u8 data){               
                if((data>= 'G') && (data <='Z')){    
                        push_debug(data);       
                        return;
                }
                if((data >= 'a') && (data <= 'z')){                                             
                        return;
                }
                if(0xff == ishexchar(data))
                        return;
                debug_getc(data);
}



