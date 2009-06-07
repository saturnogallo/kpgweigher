#include "stdafx.h" 
#include "ioex.h" 
#include "command.h"     
#include "action.h"     

/**************************************************************************************/
//                          组合称重算法（部分组合）            
/**************************************************************************************/
#define chk_cond(A)      if((RS485_Node[A].board & BOARD_GROUP_MASK) != grp)    continue;\
						 if((RS485_Node[A].board & BOARD_TYPE_MASK) != BOARD_TYPE_WEIGHT)    continue;\
                         if(RS485_Node[A].Mtrl_Weight_gram > 0xfff0)              continue;            
#define do_release(A)    RS485_Node[A].flag_release = 1;\
                         App_Node[A].fail_counter = 0;\
						 write_nodereg_b(A,flag_release);       
#define go_on(A)         if(App_Node[A].fail_counter < 200) {\
								App_Node[A].fail_counter++;\
						 }else{ \
								RS485_Node[A].Mtrl_Weight_gram = ~0;\
								RS485_Node[A].flag_release = 1;	\
								App_Node[A].fail_counter = 0; \
								write_nodereg_b(A,flag_release);\
						}

#define get_weight(A)	 RS485_Node[A].Mtrl_Weight_gram
#define WEIGHT_OVER      1
#define WEIGHT_LOWER     0
#define WEIGHT_MATCH     2

#define MAX_COMBINATION  5
u8 pids[MAX_COMBINATION];                       

#define sys_target_weight(grp)		(double)Sysboard.gw_target[grp]			//(*(grp+(u16*)&Sysboard.gw_target1))
#define sys_offset_up_limit(grp)	(double)Sysboard.gw_uvar[grp]			//(*(grp+(u16*)&Sysboard.gw_uvar1))
#define sys_offset_lo_limit(grp)	(double)Sysboard.gw_dvar[grp]			//(*(grp+(u16*)&Sysboard.gw_dvar1))
inline u8  check_weight(double sum, u16 grp)
{
	  sum = sum;
      if(sum >= sys_target_weight(grp)){
              return ((sum - sys_target_weight(grp)) < sys_offset_up_limit(grp)) ? WEIGHT_MATCH : WEIGHT_OVER;
      }else{
              return ((sys_target_weight(grp) - sum) < sys_offset_lo_limit(grp)) ? WEIGHT_MATCH : WEIGHT_LOWER;
      }
}
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
                           do_release(a);

                           pids[0] = RS485_Node[a].addr;
                           report_pack(1);
                           return 0;
              }
			  go_on(a);    
      }        
      return 1;
}

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
                           do_release(a);
                           do_release(b);                           
                           pids[0] = RS485_Node[a].addr;
                           pids[1] = RS485_Node[b].addr;
                           report_pack(2);

                           return 0;
              }
			  go_on(a);                           
			  go_on(b);                           

      }}
      return 1;      
}

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
                           do_release(a);
                           do_release(b);                           
                           do_release(c);                           

                           pids[0] = RS485_Node[a].addr;
                           pids[1] = RS485_Node[b].addr;
                           pids[2] = RS485_Node[c].addr;
                           report_pack(3);

                           return 0;
              }
			  go_on(a);                           
			  go_on(b);                           
			  go_on(c);                           

      }}}      
      return 1;
}

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
                           do_release(a);
                           do_release(b);
                           do_release(c);
                           do_release(d);
                           pids[0] = RS485_Node[a].addr;
                           pids[1] = RS485_Node[b].addr; 
                           pids[2] = RS485_Node[c].addr;
                           pids[3] = RS485_Node[d].addr;
                           report_pack(4);

                           return 0;
              }
			  go_on(a);                           
			  go_on(b);                           
			  go_on(c);                           
			  go_on(d);                                                                                  

      }}}} 
      return 1;
}
 
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
                           do_release(a);
                           do_release(b);
                           do_release(c);
                           do_release(d);
                           do_release(e);
                           pids[0] = RS485_Node[a].addr;
                           pids[1] = RS485_Node[b].addr; 
                           pids[2] = RS485_Node[c].addr;
                           pids[3] = RS485_Node[d].addr;
                           pids[4] = RS485_Node[e].addr;
                           report_pack(5);
                           return 0;
              }
			  go_on(a);                           
			  go_on(b);                           
			  go_on(c);                           
			  go_on(d);                                                                                  
			  go_on(e);        

      }}}}} 
      return 1;
}
 
