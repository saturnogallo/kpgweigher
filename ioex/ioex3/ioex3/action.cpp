#include "stdafx.h"
#include "ioex.h"
#include "msxml2.h"
#include "command.h"
#include "action.h"
#include "time.h"
#include "afxtempl.h"
#include "algorithm.h"


#import "MSXML2.dll" 
using namespace MSXML2;

extern CString sAppPath;

/*************************************************************************************************************/
//size of each reg in node or master board 
/*************************************************************************************************************/
// addr(u8), board_id(u8), baudrate(u8), Poise_Weight_gram[5](u16)
// cs_poise[5](u16), cs_zero(u16),  target_weight(u16), offset_up(u16)
// cs_Filter_option(u8), cs_gain_wordrate(u8), motor_speed(u8), magnet_freq(u8),magnet_amp(u8), magnet_time(u16)
// delay_f(u8), delay_w(u8), delay_s(u8),open_s(u8), open_w(u8)
// check_sum(u8), rom_para_valid(u8),backup_addr(u8),backup_board(u8)
// cs_mtrl(u16), Mtrl_Weight_gram(u16), Mtrl_Weight_decimal(u8),status(u8), cs_status(u8), hw_status(u8)
// cs_sys_gain_cal_data(u32), s_sys_offset_cal_data(u32), old_cs_zero(u16),NumOfDataToBePgmed(u8),flag_reset(u8)
// flag_enable(u8),flag_disable(u8),flag_release(u8), flag_goon(u8),test_mode_reg1(u8), test_mode_reg2(u8)
// mode(u8),phase(u8),motor_pulse_num(u8),magnet_pulsenum(u32), magnet_halfcycle(u16)
const u8	node_reg_size[] = 
{8,8,8,16,16,16,16,16,\
16,16,16,16,16,16,16,16, \
8,8,8,8,8,16,\
8,8,8,8,8, \
8,8,8,8,\
16,16,8,8,8,8,\
32,32,16,8,8,\
8,8,8,8,8,8,\
8,8,8,32,16};

/********************************************************************************************************************/
// "sys_reg_size" should be consistent with structure "SYSTEM_BOARD"defined in "ioex.h"
/* typedef struct
{ //status from pc
	u16 gw_target[MAX_VIBR_NUM];//0
	u16 gw_uvar[MAX_VIBR_NUM];  //8
	u16 gw_dvar[MAX_VIBR_NUM];  //16
	u8  flag_goon[MAX_VIBR_NUM];//24
	u8  node_num;               //28  // how many weight node in this system
	u8  vibrator_num;           //29  // number of Vibrator detected in the system
	u8  running[MAX_VIBR_NUM];  //30
	u8  flag_start_machine[MAX_VIBR_NUM]; //34   Set when starting machine command is received.
	u8  flag_stop_machine[MAX_VIBR_NUM];  //38
	u8  flag_search[MAX_VIBR_NUM];        //42
	u8  flag_report;					  //46
	u8  reserved1;                        //47  -- aligned
	u16 packer_config1;                   //48  -- word aligned
    u16 packer_config2;                   //50      
    u16 reserved2;                        //52
    u16 reserved3;                        //54
}SYSTEM_BOARD;*/

const u8	sys_reg_size[] = {16,16,16,16,\
16,16,16,16,\
16,16,16,16,\
8,8,8,8,\
8,\
8,\
8,8,8,8,\
8,8,8,8,\
8,8,8,8,\
8,8,8,8,\
8,\
8,\
16,16,16,16};

/********************************************************************************************************************/

u8	l2n_map[sizeof(NODE_CONFIG)]; //local address to node address map
u8	n2l_map[sizeof(NODE_CONFIG)]; //node address to local address map
u8	l2s_map[sizeof(SYSTEM_BOARD)];//local address to system address map
u8	s2l_map[sizeof(SYSTEM_BOARD)];//system address to local address map

u8 *glb_address=0;
u8 glb_hit=0;                 ///indicator 1: get answer from master board 

/*************************************************************************************************************/
// The following 2 global variables are used for data sharing between get_node_reg() and parse_frame()
// which is called by check_serial() indirectly.
// "*node_reg" points to a register in a node. "node_reg_answered" indicates whether answer has been received
// by serial port or not, 0: still waiting, 1: received.
/*************************************************************************************************************/
u8 *node_reg=0;				// node register address
u8 node_reg_answered=0;		// indicator: 1: node reg data received.


extern SOCKET sockConn;
/*************************************************************************************************************/
//
// This subroutine read node registers and wait until data is returned or timeout.
// Subroutine called : read_nodereg_b(id,name)
// Data returned: data in register flag_goon or flag_release.
// These 2 registers are reset to "0" when goon or release command is served. 
/*************************************************************************************************************/
#define QUERY_TIMEOUT 0xff
#define QUERY_SOCKET_ERR 0xfe

u8 get_node_reg(u8 node_id, u8 *reg)
{   
	u16 timeout=2048;
	u16 count = 0 ;
	node_reg = reg;
	node_reg_answered = 0;
	//send query command to node:
	cm_query(RS485_Node[node_id].addr, (u8)(reg-(u8*)&RS485_Node[node_id]), sizeof(u8));
	Sleep(TWOFRM_DURATION);
	while(node_reg_answered == 0){
		if((count++ % 211) == 210){
			//send the query for every 211*10 ms
			cm_query(RS485_Node[node_id].addr, (u8)(reg-(u8*)&RS485_Node[node_id]), sizeof(u8)); 
			printf("query RS485_Node[%d] reg again\n",node_id);
		}
		check_serial();
		if(timeout == count){
			printf("get RS485_Node[%d] reg timeout\n",node_id);
			count = 0;
			return QUERY_TIMEOUT;  
		}
		if(sockConn == SOCKET_ERROR)	//flash connection is closed
		{
			printf("socket error(location: get_node_reg())\n");
			node_reg_answered = 1;
			return QUERY_SOCKET_ERR;
		}
		Sleep(TICK_DURATION);
	}
	return *reg;
}
/*************************************************************************************************************/
//
// query the sys flag and wait until we got result
//
/*************************************************************************************************************/
u8 get_sysflag(u8 *address,u16 timeout=1024)
{
	u16 count = 0 ;
	glb_address = address;
	glb_hit = 0;
	cm_query(0xff,(u8)(address-(u8*)&Sysboard),sizeof(u8));
	Sleep(TWOFRM_DURATION);
	while(glb_hit == 0){
		if((count++ % 211) == 210){
			cm_query(0xff,(u8)(address-(u8*)&Sysboard),sizeof(u8));		//send the query for every 211*10 ms
			printf("query again\n");
		}
		check_serial();
		if(timeout == count){
			printf("get sysflag timeout\n");
			count = 0;
		}
		if(sockConn == SOCKET_ERROR)	//flash connection is closed
		{
			printf("socket error\n");
			glb_hit = 1;
			break;
		}
		Sleep(TICK_DURATION);
	}
	return *address;
}
//set the sys flag and no wait the acknowledge of master board comes
bool set_sysflagb(u8 *address, u8 target, u16 timeout=1024)
{
	u8 oldval = *address;
	*address = target;
	cm_write(0xff,(u8)(address-(u8*)&Sysboard),sizeof(u8),address);
	*address = oldval;
	return true;
}
//query and wait until the sysflag is state_idle
u8 wait_until_idle_sysflagb(u8 *reg, int timeout)
{
	u8 ret = get_sysflag(reg);
	while(1){
		if(ret == STATE_DONE_FAIL){
			set_sysflagb(reg,STATE_BEIDLE);
			Sleep(TICK_DURATION);
		}
		if(ret == STATE_DONE_OK){
			set_sysflagb(reg,STATE_BEIDLE);
			Sleep(TICK_DURATION);
		}
		if(ret == STATE_BEIDLE){
			break;
		}
		if(ret == STATE_BUSY){
			break;
		}
		if(sockConn == SOCKET_ERROR)	//flash connection is closed
		{
			printf("socket error\n");
			break;
		}
		ret = get_sysflag(reg);
	}
	return(ret);
}
//set the flag_report to addr, so that main board will report the node
bool search_node(u8 addr)
{
	u8 *reg_report = (u8*)&Sysboard.flag_report;
	wait_until_idle_sysflagb(reg_report,TWOFRM_DURATION);
	set_sysflagb(reg_report,addr);
	Sleep(TWOFRM_DURATION);
	while(1){
		if(check_sysflag(reg_report,STATE_DONE_OK)){
			set_sysflagb(reg_report,STATE_BEIDLE);
			return true;
		}
		if(check_sysflag(reg_report,STATE_DONE_FAIL)){
			set_sysflagb(reg_report,STATE_BEIDLE);
			return false;
		}
		if(sockConn == SOCKET_ERROR)	//flash connection is closed
		{
			printf("socket error\n");
			break;
		}
		Sleep(50*TWOFRM_DURATION);
		get_sysflag(reg_report);
	}; //wait for done
	return true;
}
//form an xml string based on sys board information
CString getsysxml(){
	CString str(_T("<sys "));
	int j=0; //position in size array
	for(int i=0;i<sizeof(SYSTEM_BOARD);i++)
	{
		CString name = CString(addr2name(0xff,i));
		if(name == _T("N/A"))
			continue;
		if(sys_reg_size[j] == 32)
			name.Format(_T(" %s='%i'"),name,*((u32*)((u8*)&Sysboard+i)));
		if(sys_reg_size[j] == 16)
			name.Format(_T(" %s='%i'"),name,*((u16*)((u8*)&Sysboard+i)));
		if(sys_reg_size[j] == 8)
			name.Format(_T(" %s='%i'"),name,*((u8*)&Sysboard+i));
		str = str+name;
		j++;
	}
	str.Remove('[');	str.Remove(']');
	return str+_T(" />");
}
CString xmlfilter(CString org)
{
	const TCHAR* filter[] = {_T("motor_speed"),_T("cs_Filter_option"),_T("cs_gain_wordrate"),\
		_T("test_mode_reg1"),_T("test_mode_reg2"),_T("reserved1"),_T("reserved2"),\
		_T("magnet_freq"),_T("magnet_amp"),_T("magnet_time"),_T("delay_f"),_T("delay_w"),\
		_T("delay_s"),_T("open_s"),_T("open_w")};
	CString ret(_T("<node "));
	for(int i=0;i<sizeof(filter)/sizeof(char*);i++)
	{
		CString pattern = CString(filter[i]) + _T("='");
		int pos = org.Find(pattern,0);
		if(pos<=0)
			continue;

		ret = ret + org.Mid(pos,1+org.Find(_T("'"),pos+pattern.GetLength())-pos) + _T(" ");
	}
	ret = ret + _T(" />");
	return ret;
}
int getxmlint(CString name,CString xml)
{
	return atoi(getxmlstring(name,xml));
}
CString getxmlstring(CString name,CString xml)
{
	CString pattern = name + _T("='");
	int pos = xml.Find(pattern);
	if(pos<=0)
		return _T("");
	return xml.Mid(pos+name.GetLength()+2,xml.Find(_T("'"),pos+pattern.GetLength())-(pos+name.GetLength()+2));
}
//form an xml string based on node board information
CString getnodexml(u8 addr){
	CString str(_T("<node "));
	CString name;
	int j=0; //position in size array
	for(int i=0;i<sizeof(NODE_CONFIG);i++)
	{
		 name = CString(addr2name(0,i));
		if(name == "N/A")
			continue;
		str = str+_T(" ") + name+_T("=");
		if(node_reg_size[j] == 32){
			name.Format(_T("'%i'"),*((u32*)((u8*)&RS485_Node[addr]+i)));
		}
		if(node_reg_size[j] == 16){
			name.Format(_T("'%i'"),*((u16*)((u8*)&RS485_Node[addr]+i)));
		}
		if(node_reg_size[j] == 8){
			name.Format(_T("'%i'"),*((u8*)&RS485_Node[addr]+i));
		}
		str = str + name;
		j++;
	}
	str.Remove('[');	str.Remove(']');
	return str+" />";
}
/***************************************************************************************************************/
//does the group has at least one node? 
/***************************************************************************************************************/
bool group_hasnode(u8 grp)
{
		for(int j=0;j<MAX_NODE_NUM;j++){
			//if((RS485_Node[j].board & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID){
            if(((RS485_Node[j].board & BOARD_TYPE_MASK) == BOARD_TYPE_WEIGHT) ||((RS485_Node[j].board & BOARD_TYPE_MASK) == BOARD_TYPE_VIBRATE)){
				if((RS485_Node[j].board & BOARD_GROUP_MASK) == grp){
					return true;
				}
			}
		}
		return false;
}
//send the nodeupd_id(xml) command of all the nodes in the group to display
void update_node(u8 grp)
{
		for(int j=0;j<MAX_NODE_NUM;j++){
			if((RS485_Node[j].board & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID){
				if((RS485_Node[j].board & BOARD_GROUP_MASK) == grp){
					CString id;
					id.Format(_T("%i"),RS485_Node[j].addr);
					answer_flash(_T("display:")+id+_T(":") + getnodexml(RS485_Node[j].addr));
				}
			}
		}
}
//check whether 1 sec has passed since last call
bool second_pass()
{
	static time_t t(0);
	time_t now;
	time(&now);
	
	if((now-t) > 1){
		t = now;
		return true;
	}
	return  false;

}
//check whether 6 sec has passed since last call
bool second6_pass()
{
	static time_t t6(0);
	time_t now;
	time(&now);
	if((now-t6) > 6){
		t6 = now;
		return true;
	}
	return  false;
}
/*************************************************************************************************************/
//
// Algorithm to do combinations and release
//
/*************************************************************************************************************/

#define RELEASE_DONE 0
extern u8 node2release[]; // addr of node to be released

void collect_reading(){
	u8 prev_release_done;
	u8 j;
	static bool weight_bucket_cleaned[MAX_NODE_NUM];
	
	for(int i=0;i<MAX_VIBR_NUM;i++)
	{
		u8 *reg_search = (u8*)&Sysboard.flag_search[i];
		u8 *reg_goon = (u8*)&Sysboard.flag_goon[i];	
		u8 node_to_query =MAX_NODE_NUM+1;
		
		if(!group_hasnode(i))	//without node in this group
			continue;

		u8 ret_search;
		ret_search = get_sysflag(reg_search);

		
		if(STATE_DONE_FAIL == ret_search)	//search failed
		{
			set_sysflagb(reg_search,STATE_BEIDLE);
			printf("gather weight failed\n");
			continue;
		}
		if(STATE_DONE_OK != ret_search)	//still searching
		{
			Sleep(60);
			continue;
		}
			
		update_node(i); //update the display of flash

		//clear_weight_to_zero();
		//turn_upper_bucket();
		//run combinational algorithm

		if(Sysboard.running[i]) 
		{			
			/***************************************************************************************************************/
			// First to send release command to clean lower bucket. Node firmware will adjust weight to zero on completion 
			// of release. 
			/*for(j=0;j<10;j++)
			{
			    // Check node propery
			    if((RS485_Node[j].board & BOARD_GROUP_MASK) != i) continue;
			    if((RS485_Node[j].board & BOARD_TYPE_MASK) != BOARD_TYPE_WEIGHT) continue;
                //if(RS485_Node[j].Mtrl_Weight_gram > 0xfff0) continue; 
			    if(weight_bucket_cleaned[j]) continue;
			    // weight bucket has not been cleaned. send release command.
			    printf("sending release command to node %d, grp =%d\n", j, i);
			    RS485_Node[j].flag_release = 1;
			    write_nodereg_b(j,flag_release);
			    weight_bucket_cleaned[j] = 1; // set "cleaned" flag.
			    Sleep(500);
				return;
				//if(get_node_reg(j, &(RS485_Node[j].flag_release))) return;
			    
			}		
			printf("going to algorithm, i= %d\n", i);	*/	
			/***************************************************************************************************************/
			// poll flag_release reg: 0: release action is done. We need to poll release flag from 5 nodes 
			// max, but here only sample the first node to be released.
			if(get_node_reg(node2release[0], &(RS485_Node[node2release[0]].flag_release))) 
				prev_release_done = 0; 
			else
				prev_release_done = 1;

			// if previous release request has been served by nodes, start next combination
			// Need to make sure IOEX gets updated weight value after previous release or set flag to prevent 
			// out-of-dated weight from participating new calculations. This is done by subroutine report_pack() in 
			// CalculationX(), which invalidates weight after release. 
			if(prev_release_done)
				if(Calculation5(i) == 0) { prev_release_done = 0; Sleep(10);}
			if(prev_release_done)
				if(Calculation4(i) == 0) { prev_release_done = 0; Sleep(10);}
			if(prev_release_done)
				if(Calculation3(i) == 0) { prev_release_done = 0; Sleep(10);}
			if(prev_release_done)
				if(Calculation2(i) == 0) { prev_release_done = 0; Sleep(10);}
			if(prev_release_done)
				if(Calculation1(i) == 0) { prev_release_done = 0; Sleep(10);}
			/***************************************************************************************************************/ 
			if(prev_release_done) 
			{    
				/*****************************************************************************************/
				printf("No combination this cycle, feed bucket again...\n");
				// if no successful combination, feed lower bucket to generate new weight for next cycle 
				for(j=0;j<MAX_NODE_NUM;j++)
				{	 // Check node propery
					 if((RS485_Node[j].board & BOARD_GROUP_MASK) != i) continue;
					 if((RS485_Node[j].board & BOARD_TYPE_MASK) != BOARD_TYPE_WEIGHT) continue;
                     if(RS485_Node[j].Mtrl_Weight_gram > 0xfff0) continue; 
					// Don't feed is current weight is > 1/3 of target ( unity of system.gw_target is 0.1g)
					if(RS485_Node[j].Mtrl_Weight_gram < Sysboard.gw_target[i]/32)
					{   RS485_Node[j].flag_goon = 1;
				        write_nodereg_b(j,flag_goon);
						node_to_query = j;
				    }
					Sleep(TWOFRM_DURATION);
				}
				/*****************************************************************************************/
				// If no node gets goon command, try to find nodes with low weight. Then issue goon command
				// to add material to those nodes. 
				if(node_to_query > MAX_NODE_NUM)
				{  
					for(j=0;j<MAX_NODE_NUM;j++)
					{	 // Check node propery
					     if((RS485_Node[j].board & BOARD_GROUP_MASK) != i) continue;
					     if((RS485_Node[j].board & BOARD_TYPE_MASK) != BOARD_TYPE_WEIGHT) continue;
                         if(RS485_Node[j].Mtrl_Weight_gram > 0xfff0) continue; 
					     // Don't feed is current weight is > 10/16 of target ( unity of system.gw_target is 0.1g)
					     if(RS485_Node[j].Mtrl_Weight_gram < Sysboard.gw_target[i]/16)
					     {   RS485_Node[j].flag_goon = 1;
				             write_nodereg_b(j,flag_goon);
						     node_to_query = j;
				         }
					     Sleep(TWOFRM_DURATION);
				    }
				}
			    /*****************************************************************************************/
				//set reg_goon register, master board trigger new round of weight search.
				set_sysflagb(reg_goon,1,1);	//set goon flag
				while(1){
					u8 ret = get_sysflag(reg_goon);
					if(ret == STATE_DONE_OK || ret == STATE_DONE_FAIL)
						break;
					Sleep(TWOFRM_DURATION);
				}
                /*****************************************************************************************/
				// Wait for the last issued go_on command to be completed.
				// node will reset flag_goon once command is serveed
				if(node_to_query < MAX_NODE_NUM)
				{  // start master vibrator
				   master_vibrator();				 
				   for(j=0;j<3;j++) /* try max 3 times, the delay should be enough for a node to complete goon command*/
				   {   Sleep(100);
				       printf("waiting for node (addr = %d) to complete goon command...\n", RS485_Node[node_to_query].addr );
					   if(!get_node_reg(node_to_query, &(RS485_Node[node_to_query].flag_goon)))
						   break; /* break if node has already complete goon command*/
				   }
				}
				// update fail counter for each node
				goonall(i);
			} /* end of if(prev_release_done) */
            /***************************************************************************************************************/ 
		} /* end of if(Sysboard.running[i]) */
		else{
			set_sysflagb(reg_search,STATE_BEIDLE);
		} 
		
		Sleep(TWOFRM_DURATION);
		//wait_until_idle_sysflagb(reg_search);
	}
}
/******************************************************************************************************/
//wait until packer is available
/******************************************************************************************************/
u8 check_packer(u8 grp)
{
	u8 *reg_start = (u8*)&Sysboard.flag_start_machine[grp];
	set_sysflagb(reg_start,CMD_PACKER_AVAILABLE);
	return(wait_until_idle_sysflagb(reg_start,100));
}

/******************************************************************************************************/
/* tell packer release is done*/
/******************************************************************************************************/
void drop_packer(u8 grp)
{
	u8 *reg_start = (u8*)&Sysboard.flag_start_machine[grp];
	set_sysflagb(reg_start,CMD_PACKER_DONE);
	wait_until_idle_sysflagb(reg_start,100);
}
/******************************************************************************************************/
// 1 10 01 1 1 1
// PC interface (use command start reg and offset_low_limit only): 
//    Mode:     BIT[7] 1: with shakehands    0: No shakehands 
//
//    IF1/IF2:  BIT[6:5] 00: low level trigger
//                       01: rising edge trigger 
//                       10: falling edge trigger
//                       11: high level trigger
//
//    OR1/OF1   BIT[4:3] 00: low level true
//                       01: negative pulse true
//                       10: positvie pulse true
//    FR/OF2             11: high level true
//
//    Mem:      BIT[2]   1:  pending
//                       0:  no pending
//
//    Asyn_IF:  BIT[1]   1:  Save feed request if weigher is not ready. 
//                           Packer doesn't need to send IF request again.
//                       0:  Ingore feed request if weigher is not ready.
//                           Packer has to request again later.
//
//    Init:     BIT[0]   0:  wait for packer to intiate handshake. 
//                       1:  weigher intiates handshake (first send)
//
//    MULTI_FD: BIT[10:8]:   continuously feed multi (1~8) times every request from packer.
//    OFDLY:    BIT[15:11] delay, unity 200 ms     
//  
/******************************************************************************************************/
/* initialize interface with packer*/
void init_packer_interface(u8 grp, u16 config)
{  
   /* packer interface settings 0xc2: output low, 0xCA: negative pulse, 0xCB: packer to initiate */
   //Sysboard.gw_uvar[grp] = config; 
   //write_sysreg_w(gw_uvar);
   Sysboard.packer_config1 = config; 
   write_sysreg_w(packer_config1);

   u8 *reg_start = (u8*)&Sysboard.flag_start_machine[grp];
   set_sysflagb(reg_start,CMD_PACKER_INIT);
   wait_until_idle_sysflagb(reg_start,100);   

}
/*********************************************************************************************************************/
//get node property setting from flash UI then send it to the board here
//this function just set one node property
//str : <node name1='value1|' name2='value2' name3='value3&'>
/*********************************************************************************************************************/
void setnodeproperty(int id, CString str, CString sfind, u8 regid,u8* dat, u8 size)
{
	sfind = sfind+CString("='");
	int pos = str.Find(sfind);
	if(pos < 0)
		return;
	pos = pos + sfind.GetLength();
	
	str = str.Mid(pos ,str.Find("'",pos+1)-pos);

	bool isor = (str.Find("|") >= 0);
	bool isand =(str.Find("&") >= 0);
	str.Remove('|');
	str.Remove('&');
	unsigned long val = atoi(str);
	if(size == 1){ //byte case
		if(isor)	val = (*dat) | val;
		if(isand)	val = (*dat) & val;
		if((*dat) == val)
			return; //just the same
		u8 nval = (u8)val;
		cm_write(id,regid,size,&nval); //write the data
		return;
	}
	if(size == 2){
		if(isor)	val = (*((u16*)dat)) | val;
		if(isand)	val = (*((u16*)dat)) & val;
		if((*((u16*)dat)) == val)
			return;
		u16 nval = (u16)val;
		cm_write(id,regid,size,(u8*)&nval); //write the data
		return;
	}
	if(size == 4){
		if(isor)	val = (*((u32*)dat)) | val;
		if(isand)	val = (*((u32*)dat)) & val;
		if((*((u32*)dat)) == val)
			return;
		u32 nval = (u32)val;
		cm_write(id,regid,size,(u8*)&nval); //write the data
		return;
	}

}

/*********************************************************************************************************************/
//get the node or sys setting from flash UI and send them all to node or sys board
/*********************************************************************************************************************/
void setnodexml(int id, CString str){
	int j;
	if(id == 0xff){ //system board node
			j = 0;
			for(int i=0;i<sizeof(SYSTEM_BOARD);i++)
			{
				CString name = CString(addr2name(id,i));
				if(name == "N/A")
					continue;
				if(sys_reg_size[j] == 32)
					setnodeproperty(id,str,name,i,(u8*)&Sysboard+i,sizeof(u32));
				if(sys_reg_size[j] == 16)
					setnodeproperty(id,str,name,i,(u8*)&Sysboard+i,sizeof(u16));
				if(sys_reg_size[j] == 8)
					setnodeproperty(id,str,name,i,(u8*)&Sysboard+i,sizeof(u8));
				j++;
			}
			return;
	}
	j=0;
	for(int i=0;i<sizeof(NODE_CONFIG);i++)
	{
		CString name = CString(addr2name(0,i));
		if(name == "N/A")
			continue;
		if(node_reg_size[j] == 16)
			setnodeproperty(id,str,name,i,(u8*)&RS485_Node[id]+i,sizeof(u16));
		if(node_reg_size[j] == 32)
			setnodeproperty(id,str,name,i,(u8*)&RS485_Node[id]+i,sizeof(u32));
		if(node_reg_size[j] == 8)
			setnodeproperty(id,str,name,i,(u8*)&RS485_Node[id]+i,sizeof(u8));
		j++;
	}
}
/*********************************************************************************************************************/
//caculate the average packs in past 60 seconds and send it to flash UI
//with command release_0<...
/*********************************************************************************************************************/
extern u8 pids[];
static CArray<time_t, time_t> timelist;
extern CStringArray sLog;
extern CString sLogPath;
extern double g_wavg;
extern int g_wcount;
void report_pack(u8 num){
	
	time_t t;
	time(&t);
	timelist.Add(t);
	t = t - 60;
	while(timelist.GetSize() > 0){
		if(timelist.GetAt(0) < t){
			timelist.RemoveAt(0);
		}else{
			break;
		}
	}
	double sum=0;
	CString pack;
	int group = 0;
	for(int i=0;i<num;i++)
	{
		for(int j=0;j<MAX_NODE_NUM;j++){
			if(RS485_Node[j].addr == pids[i]){
				CString a;
				a.Format(_T("bkset%i='%i' "),RS485_Node[j].addr,RS485_Node[j].Mtrl_Weight_gram);
				pack = pack + a;
				sum += (RS485_Node[j].Mtrl_Weight_gram+RS485_Node[j].Mtrl_Weight_decimal/64.0);
				RS485_Node[j].Mtrl_Weight_gram = ~0;
				group = RS485_Node[j].board & BOARD_GROUP_MASK;
			}
		}
	}
	CString ret;
	
	g_wavg += (g_wavg*g_wcount+sum)/(g_wcount + 1);
	g_wcount++;
	ret.Format(_T("release:0:<comb group='%i' weight='%.1f' speed='%i' %s/>\n"),group, sum,timelist.GetSize(),pack);
	//it should be recorded
	if(sLog.GetCount() > 500)
	{
		CStdioFile csf;
		if(csf.Open(sLogPath,CFile::modeWrite|CFile::modeNoTruncate))
		{
			for(int i=0;i<sLog.GetCount();i++)
			{
				csf.WriteString(sLog.GetAt(i));
			}
			csf.Close();
			sLog.RemoveAll();
		}
	}
	sLog.Add(ret);
	answer_flash(ret);
}
/*************************************************************************************************************/
//
/*************************************************************************************************************/
void init_vars()
{
	memset(&Sysboard,0,sizeof(SYSTEM_BOARD));
	int i,j,pos;
	//initialize RS485_Node[]: clear all field, set addr, reset fail_counter, set board property as invalid
	//and not grouped.
	for(i=0;i<MAX_NODE_NUM;i++){
		memset(&RS485_Node[i],0,sizeof(NODE_CONFIG));
		RS485_Node[i].addr = i;
		App_Node[i].fail_counter = 0;
		RS485_Node[i].board = (BOARD_TYPE_INVALID|BOARD_GROUP_NONE);
	}
	
	//generating a map from local to node and node to local
	j=0;
	pos = 0;
	// first initialze all bytes in "RS485_Node" to 0xff.
	// 0xff means: no such a register
	for(i=0;i<sizeof(NODE_CONFIG);i++)
	{
		l2n_map[i] = 0xff;
		n2l_map[i] = 0xff;
	}
	// "i" : local byte address offset.
	for(i=0;i<sizeof(NODE_CONFIG);i++)
	{
		// if one register is found with local reg address equal to i.
		if(_T("N/A") != CString(addr2name(0,i)))
		{
			// register address in node firmware is byte.
			l2n_map[i] = j/8; //u8 case
			// 
			if(node_reg_size[pos] == 8){
				n2l_map[j/8] = i;
			}
			if(node_reg_size[pos] == 16){
				n2l_map[j/8] = i;
				n2l_map[1+j/8] = i+1;
			}
			if(node_reg_size[pos] == 32){
				n2l_map[j/8] = i;
				n2l_map[1+j/8] = i+1;
				n2l_map[2+j/8] = i+2;
				n2l_map[3+j/8] = i+3;
			}
			j = j + node_reg_size[pos++];
		}
	}
	//init sys reg size array
	j=0;
	pos = 0;
	for(i=0;i<sizeof(SYSTEM_BOARD);i++)
	{
		l2s_map[i] = 0xff;
		s2l_map[i] = 0xff;
	}
	for(i=0;i<sizeof(SYSTEM_BOARD);i++)
	{
		if(_T("N/A") != CString(addr2name(0xff,i)))
		{
			l2s_map[i] = j/8;
			if(sys_reg_size[pos] == 8){
				s2l_map[j/8] = i;
			}
			if(sys_reg_size[pos] == 16){
					s2l_map[j/8] = i;
				s2l_map[1+j/8] = i+1;
			}
			if(sys_reg_size[pos] == 32){
				s2l_map[j/8] = i;
				s2l_map[1+j/8] = i+1;
				s2l_map[2+j/8] = i+2;
				s2l_map[3+j/8] = i+3;
			}

			j = j + sys_reg_size[pos++];
		}
	}
}
/*************************************************************************************************************/
//
/*************************************************************************************************************/
void init_sys_regs()
{
	for(int i=0;i<MAX_VIBR_NUM;i++){
		Sysboard.flag_stop_machine[i] = STATE_DONE_OK;
		Sysboard.flag_goon[i] = STATE_DONE_OK;
		Sysboard.flag_search[i] = STATE_DONE_OK;

		//write_sysreg_b(flag_stop_machine[i]);
		//write_sysreg_b(flag_goon[i]);//=0
		//write_sysreg_b(flag_search[i]);//=0
	}
}
//profile related function
BOOL WriteProfile(CString sect,CString name,CString value){
	return	::WritePrivateProfileString(sect,name,value,PROFILE_NAME);
}
CString ReadProfile(CString sect,CString name){
	char buffer[1000];
	::GetPrivateProfileString(sect,name,_T(""),buffer,1000,PROFILE_NAME);
	return CString(buffer);
}

inline void TESTHR( HRESULT _hr ) 
{ if FAILED(_hr) throw(_hr); }

void dump_com_error(_com_error &e)
{
	printf("Error\n");
	printf("\a\tCode = %08lx\n", e.Error());
	printf("\a\tCode meaning = %s", e.ErrorMessage());
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	printf("\a\tSource = %s\n", (LPCSTR) bstrSource);
	printf("\a\tDescription = %s\n", (LPCSTR) bstrDescription);
}

BOOL XmlAppend(CString record)
{
	bool bret = false;
	try
	{
		MSXML2::IXMLDOMDocumentPtr docPtr;
		MSXML2::IXMLDOMNodePtr DOMNodePtr;
		//init
		TESTHR(CoInitialize(NULL)); 
		TESTHR(docPtr.CreateInstance(_T("Msxml2.DOMDocument.3.0")));

		// Load a document.
		CString docname = HISTORY_NAME;
		_variant_t varXml((const char*)docname);
		_variant_t varOut((bool)TRUE);
		varOut = docPtr->load(varXml);
		if ((bool)varOut == FALSE)
		{// Show error description - IXMLDOMParseError sample.
			MSXML2::IXMLDOMParseErrorPtr errPtr = docPtr->GetparseError();
			_tprintf(_T("Format of xml broken"));
		}
		else
		{
			VARIANT vtTemp;
			MSXML2::IXMLDOMElementPtr pNode;
			vtTemp.vt=VT_I2;
			vtTemp.iVal = 1; //NODE_ELEMENT
			DOMNodePtr = docPtr->createNode(vtTemp, "item", "");
			docPtr->documentElement->appendChild(DOMNodePtr);

			pNode = docPtr->createElement((_bstr_t) "type" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("type",record));
			DOMNodePtr->appendChild(pNode);

			pNode = docPtr->createElement((_bstr_t) "start" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("start",record));
			DOMNodePtr->appendChild(pNode);

			pNode = docPtr->createElement((_bstr_t) "end" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("end",record));
			DOMNodePtr->appendChild(pNode);

			pNode = docPtr->createElement((_bstr_t) "weight" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("weight",record));
			DOMNodePtr->appendChild(pNode);

			pNode = docPtr->createElement((_bstr_t) "number" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("number",record));
			DOMNodePtr->appendChild(pNode);

			docPtr->save(varXml);
			bret = true;
		}
		
		
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	catch(...)
	{
		_tprintf(_T("Unknown error!"));
	}
	CoUninitialize();
	return bret;

}

BOOL XmlDelete(CString start,CString send)
{
	
	bool bret = false;
	try
	{
		MSXML2::IXMLDOMDocumentPtr docPtr;
		MSXML2::IXMLDOMNodePtr DOMNodePtr;
		//init
		TESTHR(CoInitialize(NULL)); 
		TESTHR(docPtr.CreateInstance(_T("Msxml2.DOMDocument.3.0")));

		// Load a document.
		CString docname = HISTORY_NAME;
		_variant_t varXml((const char*)docname);
		_variant_t varOut((bool)TRUE);
		varOut = docPtr->load(varXml);
		if ((bool)varOut == FALSE)
		{// Show error description - IXMLDOMParseError sample.
			MSXML2::IXMLDOMParseErrorPtr errPtr = docPtr->GetparseError();
			_tprintf(_T("Format of xml broken"));
		}
		else
		{
			VARIANT vtTemp;
			MSXML2::IXMLDOMElementPtr pNode;
			vtTemp.vt=VT_I2;
			vtTemp.iVal = 1; //NODE_ELEMENT
//			docPtr->setProperty("SelectionLanguage","XPath");
			CString sfind = _T("//history/item[start='")+start+_T("' and end='"+send+"']");
			MSXML2::IXMLDOMNodeListPtr pXMLDomNodeList = NULL;
			pXMLDomNodeList = docPtr->selectNodes((const char *)(sfind));
			long size;
			pXMLDomNodeList->get_length(&size);
			if (size > 0)
			{
				MSXML2::IXMLDOMNode *pN;
				MSXML2::IXMLDOMNode *pParentN;
				for (int i=0;i<size;i++)
				{
					pXMLDomNodeList->get_item(i,&pN);
					pN->get_parentNode(&pParentN);
					pParentN->removeChild(pN);
				}
				

				docPtr->save(varXml);

			}
			bret = true;
		}


	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	catch(...)
	{
		_tprintf(_T("Unknown error!"));
	}
	CoUninitialize();
	return bret;
	
}

BOOL XmlQuery(CString start,CString send)
{
	bool bret = false;
	try
	{
		MSXML2::IXMLDOMDocumentPtr docPtr;
		MSXML2::IXMLDOMNodePtr DOMNodePtr;
		//init
		TESTHR(CoInitialize(NULL)); 
		TESTHR(docPtr.CreateInstance(_T("Msxml2.DOMDocument.3.0")));

		// Load a document.
		CString docname = HISTORY_NAME;
		_variant_t varXml((const char*)docname);
		_variant_t varOut((bool)TRUE);
		varOut = docPtr->load(varXml);
		if ((bool)varOut == FALSE)
		{// Show error description - IXMLDOMParseError sample.
			MSXML2::IXMLDOMParseErrorPtr errPtr = docPtr->GetparseError();
			_tprintf(_T("Format of xml broken"));
		}
		else
		{
			VARIANT vtTemp;
			MSXML2::IXMLDOMElementPtr pNode;
			vtTemp.vt=VT_I2;
			vtTemp.iVal = 1; //NODE_ELEMENT
			//			docPtr->setProperty("SelectionLanguage","XPath");
			CString sfind = _T("//history/item[start>='")+start+_T("' and end<='"+send+"']");
			MSXML2::IXMLDOMNodeListPtr pXMLDomNodeList = NULL;
			pXMLDomNodeList = docPtr->selectNodes((const char *)(sfind));
			long size;
			pXMLDomNodeList->get_length(&size);
			CStdioFile csf;
			if(csf.Open(RESULT_NAME,CFile::modeCreate|CFile::modeWrite))
			{
				csf.WriteString(_T("<?xml version=\"1.0\" encoding=\"utf-8\"?><history>"));
				if (size > 0)
				{

					BSTR xmlstring;
					for (int i=0;i<size;i++)
					{
						pXMLDomNodeList->get_item(i,&DOMNodePtr);
						DOMNodePtr->get_xml(&xmlstring);
						csf.WriteString(CW2CT(xmlstring));
					}

				}
				csf.WriteString("</history>");
				csf.Close();
			}
			bret = true;
		}


	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	catch(...)
	{
		_tprintf(_T("Unknown error!"));
	}
	CoUninitialize();
	return bret;}