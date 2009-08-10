/******************************************************************************************************/
//  Interface with packing machines
//  OR1(PORTF.0): OUT_READY, output to indicate at least one combination is available.
//  OF1(PORTF.1): OUT_Feed_Done, output to request packing machine to pack
//  IF1(PORTD.0): IN_Feed_Request. Request weigher to feed 
//  Note: PORTD.0 is external interrupt input 0

// 一，和外界是以下3个信号
// 1）OR1，就绪信号，多头称组合完成， 通知包装机，
// 2）OF1，灌装完成信号，多头称将相应的已组合的斗打开完成灌装，并给出这个信号通知包装机
// 3）IF1，要求灌装信号，包装机给出一个要求灌装的信号给多头称，多头称如果已组合完成， 就马上灌装，同时输出OF1
// 对于24头的组合称， 最多可以是4个独立的4台，因此是4组这样的信号， OR1-OR4，OF1-OF4， IF1-IF4

// 二，强制灌装和应答灌装功能
// 有功能键选择强制灌装和应答灌装，
// 强制灌装：不管包装机是否就绪完毕，多头称组合完成就马上灌装，OR 和OF是有效的，IF无效
// 应答灌装：需要和包装机应答，OR ，OF，IF均有效.

// 三，延时灌装能
// 可以先发出OF1信号0-x毫秒后，再打开斗灌装

// 四，信号形式选择
// IF， 可以选择低电平，高电平，上升沿，下降沿有效
// OR，可以选者低电平，高电平脉冲有效

// 五，信号电气规格
// IF：推挽
// OR，OC  24VDC                    
//  

// ***************************** USAGE ******************************
// Init_interface() should be re-called everytime when interface settings are changed in GUI. 

// *********Shakehands mode:*************
// step 1: On finding available combinations, master board output OR1 pulse 
// step 2: Packer request weigher to feed
// step 3: master board releases material and output OF1 pulse. 
                                                                
// IF (combinations available)
// {    Tell_Packer_Weigher_Rdy();
//   While(Packer_Is_Busy()); 
//   Release_Material();
//   Tell_Packer_Release_Done();
// }
// CHECK NEW COMBINATIONS
//
                           
// ********Non shakehands mode:**********
// step 1: On finding available combinations, master board output OR1 pulse 
// step 2: master board releases material and output OF1 pulse. 
// IF (combinations available)
// {    Tell_Packer_Weigher_Rdy();
//   Release_Material();
//   Tell_Packer_Release_Done();
// }
// CHECK NEW COMBINATIONS.

   
// *************************************************************************

// PC interface (use command start reg and offset_low_limit only): 
//    Mode:     BIT[7] 1: with shakehands    0: No shakehands 
//    IF1:      BIT[6:5] 00: fixed low 
//                       01: rising edge  
//                       10: falling edge
//                       11: fixed high
//    OR1/OF1:  BIT[4:3] 00: fixed low 
//                       01: rising edge  
//                       10: falling edge
//                       11: fixed high
//    Reserved: BIT[2:0]
//    
//    OFDLY:    BIT[15:8] delay, unity 10ms     
//  
//    
/******************************************************************************************************/

#include <mega64.h>
#include "global.h"

u16 packer_config=0;                           
extern SYSTEM system;     // 系统参数设置
typedef struct {
   volatile u8 pack_reqs_pending; /*flag to indicate whether there is any pending reuqests from packer*/                              
   volatile u8 Tmr0_Svs[MAX_SVS_NUM];         /*Service 0 provided by Timer 0 */
   volatile u8 Tmr0_Svs_cmplt[MAX_SVS_NUM]; /*Service completion status*/
}INTERFACE;

INTERFACE Intf; 

/******************************************************************************************************/
// External Interrupt 0 service routine
/******************************************************************************************************/
interrupt [EXT_INT0] void ext_int0_isr(void)
{
   //clear interrupt flag
   //是否需要防抖动处理？
   
   // Set flag to indicate a request from packer is pending. 
   //Intf.pack_reqs_pending = 0xff;   
   if(Intf.pack_reqs_pending < 0xff)
        Intf.pack_reqs_pending++;     /*non-zero means pack request pending*/
      
}

/******************************************************************************************************/
// Timer 0 overflow interrupt service routine.
/******************************************************************************************************/
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
   /*Timer0 is initialized in "init.c" */
   /*clear interrupt flag*/
   CLR_TOV0(); 
   
   Set_10ms_Tick(); /*10ms interrupt*/
   
   if (Intf.Tmr0_Svs[0])    // pulse width not reached
       Intf.Tmr0_Svs[0]--; 
   else if (!Intf.Tmr0_Svs_cmplt[0]) //pulse width reached, not toggled yet.
   {     TOGGLE_OR1;     // pluse width timeout
         Intf.Tmr0_Svs_cmplt[0] = SVS_CMPLT;
   }
   
   if (Intf.Tmr0_Svs[1])   // pulse width not reached 
       Intf.Tmr0_Svs[1]--; 
   else if (!Intf.Tmr0_Svs_cmplt[1])  //pulse width reached, not toggled yet.
   {     TOGGLE_OF1;     // pluse width timeout
         Intf.Tmr0_Svs_cmplt[1] = SVS_CMPLT;
   }
   
   /*Disable timer 0 interrupt on service completion*/
   if(Intf.Tmr0_Svs_cmplt[0] && Intf.Tmr0_Svs_cmplt[1])
        MASK_TMR0(); 
}

/******************************************************************************************************/
// Kick off Timer 0 
/******************************************************************************************************/
void Kickoff_Timer0(u8 svs_num, u8 svs_time)
{    
   switch (svs_num)
   { case 0:
        Intf.Tmr0_Svs[0] = svs_time;
        Intf.Tmr0_Svs_cmplt[0] = 0;  // set timer service completion status to false
        break;
     case 1:
        Intf.Tmr0_Svs[1] = svs_time;
        Intf.Tmr0_Svs_cmplt[1] = 0; 
        break;
     default:
        break;   
   }
   // test if timer0 has already been enabled
   if(!TMR0_Is_Enabled())  
   { 
     CLR_TOV0();      /*clear interrupt flag*/
     Set_10ms_Tick(); /*10ms interrupt*/
     START_TMR0();
   }
}

/******************************************************************************************************/
// Initialize interface pins
// This procedure should be called after PC download interface settings to masterboard 
// Hardware Abstract
// INTF1_MTRL_RDY PORTF.0  INTF1_FEED_DONE PORTF.1  INTF1_FEED_REQ PORTD.0 

// use target_weight register as variable interface with PC
/******************************************************************************************************/
void Init_interface()
{   
   u8 i;
   DDRF |= 0x3 ;  /* set PORTF[1:0] as output */      
   switch(CONFIG_REG & PACKER_OF_MASK) /*set initial states for OR1/OF1*/
   {
      case 0x0:    // low to enable  
      case 0x8:
          PORTF |= 0x3;   //so,initialize to high 
          break;
      case 0x10:
      case 0x18:   // high to enable
          PORTF &= 0xFC; // so, initialize to low
          break;
      default: 
          break;         
   }
   
   /* Settings for IF1, use External interrupt 0*/
   /* PORTD.0 has been set as input in Init_Port() */
   /* Make sure Ex_Int4~7 (used by 16C554) is included */   
   switch(CONFIG_REG & PACKER_IF_MASK) /* IF1 */
   {        
      case 0x00:
          EICRA=0x00; /*interrupt triggered by low level*/
          EICRB=0xFF;
          EIMSK=0xF1;
          EIFR=0xF1;
          break;
      case 0x20:
          EICRA=0x03; /*interrupt triggered by rising edge*/
          EICRB=0xFF;
          EIMSK=0xF1;
          EIFR=0xF1;         
          break;
      case 0x40:
          EICRA=0x02; /*interrupt triggered by falling edge*/
          EICRB=0xFF;
          EIMSK=0xF1;
          EIFR=0xF1;          
          break;
      default:
          EICRA=0x00; 
          EICRB=0xFF;
          EIMSK=0xF0;
          EIFR=0xF0;
          break;   
   }         
   
   /*Initialize interface variables*/
   Intf.pack_reqs_pending = 0;  /* no pending requests from packing machine */
   for (i=0; i <MAX_SVS_NUM; i++)
   {     Intf.Tmr0_Svs[i] = 0;   /* Reset services provided by Timer 0 */   
         Intf.Tmr0_Svs_cmplt[i] = SVS_CMPLT; /*all services completed*/
   }
}

/******************************************************************************************************/
// return 0x0 if packing machine is ready to receive another feeding. return 0xff if busy
/******************************************************************************************************/
#define INTF_MODE_SHAKEHANDS  (CONFIG_REG & 0xff7f)

u8 Packer_Is_Busy()
{  
    /* if in shakehands mode and no requests from packer is pending.*/
    /* variable "pack_reqs_pending" is set by ISR, it is declared as volatile type.*/
    if((INTF_MODE_SHAKEHANDS) && (!Intf.pack_reqs_pending))
        return 0xff;             
    else
       return 0x0; 
}
 

/******************************************************************************************************/
// When combination is available, call this function to inform packer that weigher is ready to release
// material 
// Output: Pulse, 
/******************************************************************************************************/
void Tell_Packer_Weigher_Rdy()
{  
   switch(CONFIG_REG & PACKER_OF_MASK)
   {
      case 0x0:    // low to enable  
      case 0x8:
          PORTF &= 0xFE; // OR1(PORTF.0) = 0
          break;
      case 0x10:
      case 0x18:   // high to enable
          PORTF |= 0x1; //OR1 (PORTF.0) = 1;
          break;
      default: 
          break;         
   }
   //start timer to generate output pulse.
   Kickoff_Timer0(0, INTF_PLS_WIDTH);   
}

/******************************************************************************************************/
// Inform packer that weigher has completed release of material       
// pulse
/******************************************************************************************************/
void Tell_Packer_Release_Done()
{
   switch(CONFIG_REG & PACKER_OF_MASK)
   {
      case 0x0:    // low to enable  
      case 0x8:
          PORTF &=0xFD; //OF1(PORTF.1) = 0
          break;
      case 0x10:
      case 0x18:   // high to enable
          PORTF |=0x2; //OF1(PORTF.1) = 1
          break;
      default: 
          break;         
   } 
   
   //start timer to generate output pulse.
   Kickoff_Timer0(1, INTF_PLS_WIDTH);      
   Intf.pack_reqs_pending = 0; /* clear pending request status, no requests pending */
}
