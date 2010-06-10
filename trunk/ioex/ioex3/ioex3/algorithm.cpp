/*************************************************************************************************************/
//										    Combinational Algorithm            
/*************************************************************************************************************/

#include "stdafx.h" 
#include "ioex.h" 
#include "command.h"     
#include "action.h"     

/*************************************************************************************************************/
// Check if node belongs to this group, check if board is a weight board, check if board is waiting for 
// valid weight, skip current node if fails any of these 3 tests.
/*************************************************************************************************************/
#define chk_cond(A)      if((RS485_Node[A].board & BOARD_GROUP_MASK) != grp)    continue;\
						 if((RS485_Node[A].board & BOARD_TYPE_MASK) != BOARD_TYPE_WEIGHT)    continue;\
                         if(RS485_Node[A].Mtrl_Weight_gram > 0xfff0)              continue;    


/*************************************************************************************************************/
// if we haven't reached "force-clear-time" limit, fail counter increases by 1, otherwise, force release and 
// reset fail counter.
/*************************************************************************************************************/
#define MAX_FAIL_TIMES_ALLOWED 20
#define go_on(A)         if(App_Node[A].fail_counter < MAX_FAIL_TIMES_ALLOWED) {\
								App_Node[A].fail_counter++;\
						 }else{ \
								hitme(A);\
								RS485_Node[A].Mtrl_Weight_gram = ~0;\
								RS485_Node[A].flag_release = 1;	\
								App_Node[A].fail_counter = 0; \
								write_nodereg_b(A,flag_release);\
								printf("******Force release node %d******\n",A);\
						}

void hitme(int i)
{
	
	i = i+1;

}

/*************************************************************************************************************/
// send commands to force all nodes to go on (actually do nothing but increasing fail counter by 1)
/*************************************************************************************************************/
void goonall(u8 grp)
{
	u8 a;
	for(a=0; a<MAX_NODE_NUM-0; a++)
	{   chk_cond(a);
		go_on(a);
	}

}

/*************************************************************************************************************/
//
/*************************************************************************************************************/
#define get_weight(A)	 RS485_Node[A].Mtrl_Weight_gram
#define WEIGHT_OVER      1
#define WEIGHT_LOWER     0
#define WEIGHT_MATCH     2

#define MAX_COMBINATION  5
/*****************************************************************************************/
// Array to save addresss of node to be release. This serves as data returned by 
// calculation1(), calculation2() ... calculation5().
/*****************************************************************************************/
u8 node2release[MAX_COMBINATION]={1,2,3,4,5};
u8 pids[MAX_COMBINATION];


#define sys_target_weight(grp)		((double)Sysboard.gw_target[grp]/10.0)
#define sys_offset_up_limit(grp)	((double)Sysboard.gw_uvar[grp]/10.0)
#define sys_offset_lo_limit(grp)	((double)Sysboard.gw_dvar[grp]/10.0)

/*************************************************************************************************************/
// Write "release" command to "flag_release" register of node and reset its fail counter.
//
// Associated info in node system firmware:
// 1. Material will be released only when BOTH node register "flag_release" is written with "1" AND "flag_enable" 
//    is set to "ENABLE_ON". (See main.c of node firmware source code).
// 2. When "flag_enable" is set to "ENABLE_ON" and before "flag_release" is written with "1", motor_magnet_action()
//    is called to feed lower bucket. If no go on command received, code won't break out of motor_magnet_action().
//    motor_magnet_action() will continuously output weight values. Ioex needs to ensure nodes getting go_on
//    signal to perform release action.
//
/*************************************************************************************************************/
/*#define do_release(A)    RS485_Node[A].flag_release = 1;\
                         App_Node[A].fail_counter = 0;\
						 write_nodereg_b(A,flag_release);   */

void do_release(u8 i, u8 grp)
{
	App_Node[i].fail_counter = 0;
	// send release command to node
	RS485_Node[i].flag_release = 1;
	write_nodereg_b(i,flag_release);
	
/*	// if current node is overweight( >3/8 of target, same as master board), ioex has to send goon command to
	// node instead to permit release,because master board won't send goon command to an overweight node. 
	// Goon command is to get node firmware out of a infinite loop. The command should not be sent prior to 
	// release command.
	// This will be unnecessary since node firmware will be updated so that release command can also break out
	// infinite loop. 11-13-2009
	//if(get_weight(i) >= Sysboard.gw_target[grp]/16)
	{	RS485_Node[i].flag_goon = 1;
		write_nodereg_b(i,flag_goon);
	}//*/
		

}

/*************************************************************************************************************/
//  Check if current weight meet release criteria: |target - sum| =< offset
/*************************************************************************************************************/
inline u8  check_weight(double sum, u16 grp)
{
	  sum = sum;
      if(sum >= sys_target_weight(grp)){
              return ((sum - sys_target_weight(grp)) < sys_offset_up_limit(grp)) ? WEIGHT_MATCH : WEIGHT_OVER;
      }else{
              return ((sys_target_weight(grp) - sum) < sys_offset_lo_limit(grp)) ? WEIGHT_MATCH : WEIGHT_LOWER;
      }
}

/*************************************************************************************************************/
// Test if we can find 1 node whose weight meets release criteria: |target - sum| =< offset
/*************************************************************************************************************/
u8 Calculation1(u8 grp)
{                      
      u8 a, oflag; 
      u32 weight_sum;
      if(Sysboard.node_num < 1)
            return 1;

      for(a=0; a<MAX_NODE_NUM-0; a++){     chk_cond(a);
              weight_sum = get_weight(a);
                        
              oflag = check_weight(weight_sum,grp);
              if(oflag == WEIGHT_MATCH){ //组合成功                                   
				           if(check_packer(grp) == STATE_BUSY)
							   return 0;  /* packer is busy, return, try next time. */
                           do_release(a,grp);
						   
						   pids[0] = RS485_Node[a].addr;	//pids used by report_pack()					   
						   
						   //node2release to inform collect_reading() of nodes to be released
						   node2release[0] = a;
						   node2release[1] = 0xff;  // not a valid node address
						   node2release[2] = 0xff;
						   node2release[3] = 0xff;
						   node2release[4] = 0xff;

						   drop_packer(grp);
                           report_pack(1); //report to flash
                           return 0;
              }
      }        
      return 1;
}

/*************************************************************************************************************/
// Test if we can find 2 nodes whose weight sum meets release criteria: |target - sum| =< offset
/*************************************************************************************************************/
u8 Calculation2(u8 grp)
{                      
      u8 a, b, oflag;
      u32 weight_sum;
      if(Sysboard.node_num < 2)
            return 1;

      for(b=0;   b<MAX_NODE_NUM-1; b++){     chk_cond(b);
      for(a=b+1; a<MAX_NODE_NUM-0; a++){     chk_cond(a);
              weight_sum = get_weight(a) + \
                           get_weight(b);
                        
              oflag = check_weight(weight_sum,grp);
              if(oflag == WEIGHT_MATCH){ //组合成功 
				           if(check_packer(grp) == STATE_BUSY)
							   return 0;  /* packer is busy, return, try next time. */
                           do_release(a,grp);
                           do_release(b,grp);                           
                           
						   pids[0] = RS485_Node[a].addr; //pids used by report_pack()
                           pids[1] = RS485_Node[b].addr;
						   
						   //node2release to inform collect_reading() of nodes to be released
						   node2release[0] = a;
						   node2release[1] = b;  
						   node2release[2] = 0xff; // not a valid node address
						   node2release[3] = 0xff;
						   node2release[4] = 0xff;
						   
						   drop_packer(grp);
                           report_pack(2);
						   printf("release %d and %d\n", a, b);
                           return 0;
              }
      }}
      return 1;      
}

/*************************************************************************************************************/
// Test if we can find 3 nodes whose weight sum meets release criteria: |target - sum| =< offset
/*************************************************************************************************************/
u8 Calculation3(u8 grp)
{                      
      u8 a, b, c,oflag;
      u32 weight_sum;      
      if(Sysboard.node_num < 3)
            return 1;
      for(c=0;   c<MAX_NODE_NUM-2; c++){    
		  chk_cond(c);
      for(b=c+1; b<MAX_NODE_NUM-1; b++){
		  chk_cond(b);
      for(a=b+1; a<MAX_NODE_NUM-0; a++){
		  chk_cond(a);
              weight_sum = get_weight(a) + \
                           get_weight(b) + \
						   get_weight(c);
                      
              oflag = check_weight(weight_sum,grp);
              if(oflag == WEIGHT_MATCH){ //组合成功  
				           if(check_packer(grp) == STATE_BUSY)
							   return 0;  /* packer is busy, return, try next time. */
                           do_release(a,grp);
                           do_release(b,grp);                           
                           do_release(c,grp);     

                           pids[0] = RS485_Node[a].addr;
                           pids[1] = RS485_Node[b].addr;
                           pids[2] = RS485_Node[c].addr;

						   //node2release to inform collect_reading() of nodes to be released
						   node2release[0] = a;
						   node2release[1] = b;  
						   node2release[2] = c; 
						   node2release[3] = 0xff; // not a valid node address
						   node2release[4] = 0xff;

						   drop_packer(grp);
                           report_pack(3);
						   printf("release %d, %d and %d\n", a,b,c);
                           return 0;
              }
      }}}      
      return 1;
}

/*************************************************************************************************************/
// Test if we can find 4 nodes whose weight sum meets release criteria: |target - sum| =< offset
/*************************************************************************************************************/
u8 Calculation4(u8 grp)
{                      
      u8 a, b, c,d,oflag;
      u32 weight_sum;      
      if(Sysboard.node_num < 4)
            return 1;
      for(d=0;   d<MAX_NODE_NUM-3; d++){     chk_cond(d);            
      for(c=d+1; c<MAX_NODE_NUM-2; c++){     chk_cond(c);
      for(b=c+1; b<MAX_NODE_NUM-1; b++){     chk_cond(b);
      for(a=b+1; a<MAX_NODE_NUM-0; a++){     chk_cond(a);
              weight_sum = get_weight(a) + \
                           get_weight(b) + \
						   get_weight(c) + \
						   get_weight(d);

              oflag = check_weight(weight_sum,grp);
              if(oflag == WEIGHT_MATCH){ //组合成功  
				           if(check_packer(grp) == STATE_BUSY)
							   return 0;  /* packer is busy, return, try next time. */
                           do_release(a,grp);
                           do_release(b,grp);
                           do_release(c,grp);
                           do_release(d,grp);

                           pids[0] = RS485_Node[a].addr;
                           pids[1] = RS485_Node[b].addr; 
                           pids[2] = RS485_Node[c].addr;
                           pids[3] = RS485_Node[d].addr;
						   
						   //node2release to inform collect_reading() of nodes to be released
						   node2release[0] = a;
						   node2release[1] = b;  
						   node2release[2] = c; 
						   node2release[3] = d; 
						   node2release[4] = 0xff;  // not a valid node address

						   drop_packer(grp);
                           report_pack(4);
						   printf("release %d, %d, %d and %d\n", a,b,c,d);
                           return 0;
              }
      }}}} 
      return 1;
}
 
/*************************************************************************************************************/
// Test if we can find 5 nodes whose weight sum meets release criteria: |target - sum| =< offset
/*************************************************************************************************************/
u8 Calculation5(u8 grp)
{                      
      u8 a, b, c,d,e,oflag;
      u32 weight_sum;      
      if(Sysboard.node_num < 5)
            return 1;
      for(e=0  ; e<MAX_NODE_NUM-4; e++){     chk_cond(e);
      for(d=e+1; d<MAX_NODE_NUM-3; d++){     chk_cond(d);            
      for(c=d+1; c<MAX_NODE_NUM-2; c++){     chk_cond(c);
      for(b=c+1; b<MAX_NODE_NUM-1; b++){     chk_cond(b);
      for(a=b+1; a<MAX_NODE_NUM-0; a++){     chk_cond(a);
              weight_sum = get_weight(a) + \
                           get_weight(b) + \
						   get_weight(c) + \
						   get_weight(d) + \
						   get_weight(e);                          
              oflag = check_weight(weight_sum,grp);
              if(oflag == WEIGHT_MATCH){ //组合成功   
				           if(check_packer(grp) == STATE_BUSY)
							   return 0;  /* packer is busy, return, try next time. */
                           do_release(a,grp);
                           do_release(b,grp);
                           do_release(c,grp);
                           do_release(d,grp);
                           do_release(e,grp);
 
						   pids[0] = RS485_Node[a].addr;
                           pids[1] = RS485_Node[b].addr; 
                           pids[2] = RS485_Node[c].addr;
                           pids[3] = RS485_Node[d].addr;
                           pids[4] = RS485_Node[e].addr;

						   //node2release to inform collect_reading() of nodes to be released
						   node2release[0] = a;
						   node2release[1] = b;  
						   node2release[2] = c; 
						   node2release[3] = d; 
						   node2release[4] = e;  // not a valid node address

						   drop_packer(grp);
                           report_pack(5);
						   printf("release %d, %d, %d, %d and %d\n", a,b,c,d,e);
                           return 0;
              }
      }}}}} 
      return 1;
}
 
/*************************************************************************************************************/
//						Auto-Feeder-Adjustment Algorithm
//  Adjust Magnet's amplitude to control material feeding speed. 
//  1. Clean all upper/lower bucket and "zero" out current weight.
//  2. Get current magnet amplitude of node.
//  3. Get current weight.
//  4. Send goon command to feed node. 
//  5. Get new weight and calculate weight increment delta[0]
//  6. repeat step 3 - 4 to get weight increment delta[1], delta[2] ... delta[N]
//  7. Calculate average of delta. 
//  8. If |average - sys_target_weight/4| < MAX_OFFSET, magnet amplitude is good, jump to step 12.
//  9. If average > sys_target_weight/4 + MAR_OFFSET, manget amplitude is too high, minish amplitude.
// 10. If average < sys_target_weight/4 - MAR_OFFSET, manget amplitude is too low, enlarge amplitude.
// 11. If adjustment has been re-tried for more than 3 time, give up, otherwise go back to step 1.
// 12. Clean all upper/lower bucket and "zero" out current weight.
// 13. return status and quit. 
/*************************************************************************************************************/
#define AUTO_ADJUSTMENT_DONE_SUCCESSFULLY 0
#define AMPLITUDE_TOO_LOW 1
#define AMPLITUDE_TOO_HIGH 2

void turn_upper_bucket()
{	
	RS485_Node[1].magnet_amp = 40;
	write_nodereg_b(1,magnet_amp);
	Sleep(1);
	RS485_Node[1].flag_enable = ENABLE_VIBRATE;
	write_nodereg_b(1,flag_enable);

	Sleep(1000);
	
	RS485_Node[1].magnet_amp = 55;
	write_nodereg_b(1,magnet_amp);
	Sleep(1);
	RS485_Node[1].flag_enable = ENABLE_VIBRATE;
	write_nodereg_b(1,flag_enable);
	Sleep(1000);	
}

void master_vibrator()
{
	RS485_Node[MASTER_VIBR_ADDR].flag_enable = ENABLE_VIBRATE;
	write_nodereg_b(MASTER_VIBR_ADDR,flag_enable);	
	printf("master vibrator vibrating..\n");
}
/*
tested: ok
void turn_upper_bucket()
{	
	RS485_Node[1].flag_enable = ENABLE_TURNS;
	write_nodereg_b(1,flag_enable);
	
}

void turn_lower_bucket()
{	
	RS485_Node[1].flag_enable = ENABLE_TURNW;
	write_nodereg_b(1,flag_enable);
	
}

void start_vibrator()
{	
	RS485_Node[1].flag_enable = ENABLE_VIBRATE;
	write_nodereg_b(1,flag_enable);
	
}

void clear_weight_to_zero()
{
	RS485_Node[9].flag_enable = ENABLE_RESET_WEIGHT;
	write_nodereg_b(9,flag_enable);
}
*/

