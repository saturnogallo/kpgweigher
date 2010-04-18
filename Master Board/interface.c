/******************************************************************************************************/
//  Interface with packing machines
//  OR1(PORTF.0): OUT_READY, output to indicate at least one combination is available.
//  OF1(PORTF.1): OUT_Feed_Done, output to request packing machine to pack
//  FR1(PORTF.2): force release output to indicate current release is force release. 
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
// 1 10 10 0 1 0
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
//    DLY:      BIT[2]   1:  add delay for material to drop into packer 
//                           before sending release_done signal to packer.
//                       0:  no delay
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
//    OFDLY:    BIT[15:11]:  delay, unity 200ms
//  
//    
/******************************************************************************************************/

#include <mega64.h>
#include "global.h"

u16 packer_config=0;                           

extern SYSTEM system;

typedef struct {
   volatile u8 pack_reqs_pending;             /* flag to indicate whether there is any 
                                                 pending reuqests from packer */                              
   volatile u16 Tmr0_Svs[MAX_SVS_NUM];         /* Service 0 provided by Timer 0 */
   volatile u8 Tmr0_Svs_Status;               /* Service completion status*/ 
   u8 weigher_is_ready;                        /* weigher is available */
   u8 max_feed_counter;                        /* recorde how many times feed has been done */
   u8 feed_counter;                            /* recorde how many times feed has been done */
}INTERFACE;

INTERFACE Intf; 
u8 new_request;
u8 multi_feed_inprogress;
/******************************************************************************************************/
// External Interrupt 0 service routine: feed request from packer 
// return status whether packer is requesting feeding.
/******************************************************************************************************/
interrupt [EXT_INT0] void ext_int0_isr(void)
{
   /* clear interrupt flag,write "1" to bit 0. */
   EIFR |= 0x1; 
   //filter

   /* Set flag to indicate a request from packer is pending. */
   if(CONFIG_REG & ASYN_IF_MASK){       
      if(Intf.pack_reqs_pending == FALSE)
         Intf.pack_reqs_pending = TRUE;           /* non-zero: pack request pending*/
   }else{                                        /* feed is set synchronized with IF singal*/
      if(!Intf.weigher_is_ready)
         Intf.pack_reqs_pending = FALSE;          /* ignore feed request */ 
      else
         Intf.pack_reqs_pending = TRUE;           /* pack request pending */
   }
   
   /* On receive response from packer, test if we need to toggle "OF" signal */
 /*  switch(CONFIG_REG & PACKER_OF_MASK)
   {
      case 0x0:                                   /* low to enable */      
          PORTF |= (1<<PORT_OF1);                 /* assert low until packer response */              
      //    break;
     // case 0x8:                                   /* negative pulse to enable */ 
     //     break;                                  /* no need. handled by timer0_ovf_isr */
     // case 0x10:                                  /* positive pulse to enable */
     //     break;                                  /* no need. handled by timer0_ovf_isr */
     // case 0x18:                                  /* high to enable */
     //     PORTF &= (~(1<<PORT_OF1));              /* assert high until packer response */
      //    break;
     // default: 
     //     break;         
  // } 
   /* if current input is set to level, the input signal has been deasserted */
   switch(CONFIG_REG & PACKER_IF_MASK)
   {
      case 0x0: /* low level */
          PORTF |= (1<<PORT_OF1);                 /* assert low until packer response */ 
          new_request = TRUE;
          break;
      case 0x60: /* high level */
          PORTF &= (~(1<<PORT_OF1));              /* assert high until packer response */
          new_request = TRUE;
          break;
      default:
         break;
   }
   
   Intf.feed_counter = 0;                         /* reset feed counter */
         
   LED_FLASH(LED_RUN);//debug
}

/******************************************************************************************************/
// External Interrupt 1 service routine: request from packer, reserved for future use
/******************************************************************************************************/
interrupt [EXT_INT1] void ext_int1_isr(void)
{
   /* mask interrupt 1, subjected to change */
   TIMSK &= 0xFD; 
}

/******************************************************************************************************/
// Timer 0 overflow interrupt service routine.
// Interrupt every 10 ms to generate timer services. Max 8 services are supported. 
/******************************************************************************************************/
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
   /*Timer0 is initialized in "init.c" */
   /*clear interrupt flag*/
   CLR_TOV0(); 
   
   Set_10ms_Tick();                            /* 10ms interrupt */
   
   /* Timer 0, service 0 */   
   if (Intf.Tmr0_Svs[0])                       /* service 0: pulse width not reached */
       Intf.Tmr0_Svs[0]--; 
   else if (Intf.Tmr0_Svs_Status & 0x1)        /* 1: pulse not toggled yet */
   {     TOGGLE_OR1;                           /* complete output ready pulse */
         Intf.Tmr0_Svs_Status &= 0xFE;         /* 0: completion flag */
   }

   /* Timer 0, service 1 */   
   if (Intf.Tmr0_Svs[1])                       /* service 1: pulse width not reached */
       Intf.Tmr0_Svs[1]--; 
   else if (Intf.Tmr0_Svs_Status & 0x2)        /* 1: pulse not toggled yet */
   {     TOGGLE_OF1;                           /* complete output feed pulse */
         Intf.Tmr0_Svs_Status &= 0xFD;         /* 0: completion flag */
   }

   /* Timer 0, service 2 */   
   if (Intf.Tmr0_Svs[2])                       /* service 2: pulse width not reached */
       Intf.Tmr0_Svs[2]--; 
   else if (Intf.Tmr0_Svs_Status & 0x4)        /* 1: pulse not toggled yet */
   {     TOGGLE_OFR1;                          /* complete output force release pulse */
         Intf.Tmr0_Svs_Status &= 0xFB;         /* 0: completion flag */
   }
   /* add service 3 - 7 below*/
   /* Timer 0, service 7 */   
   if (Intf.Tmr0_Svs[7])                       /* service 2: pulse width not reached */
       Intf.Tmr0_Svs[7]--; 
   else if (Intf.Tmr0_Svs_Status & 0x80)       /* 1: signal not sent yet */
   {     
       Intf.Tmr0_Svs_Status &= 0x7F;           /* 0: completion flag */       
   }

   /* Disable timer 0 interrupt on service completion*/
   if (!(Intf.Tmr0_Svs_Status & ((1<<MAX_SVS_NUM)-1)))
      MASK_TMR0_INT();

   /* IMPORTANT NOTE: user need to clear interrupt flag first before re-enable TMR again 
      to prevent residual interrupt flag. */
}

/******************************************************************************************************/
// Kick off Timer 0 services
/******************************************************************************************************/
void Kickoff_Timer0(u8 svs_num, u16 svs_time)
{    
   switch (svs_num)
   { case 0:                                      /* service 0: OR1 pulse width */
        Intf.Tmr0_Svs[0] = svs_time;
        Intf.Tmr0_Svs_Status |= 0x1;              /* set service completion status, 1: not completed */
        break;
     case 1:                                      /* service 1: OF1 pulse width */
        Intf.Tmr0_Svs[1] = svs_time;
        Intf.Tmr0_Svs_Status |= 0x2; 
        break;
     case 2:                                      /* service 2: FR1 pulse width */
        Intf.Tmr0_Svs[2] = svs_time;
        Intf.Tmr0_Svs_Status |= 0x4; 
        break;
     case 3:                                      /* add other services */
        Intf.Tmr0_Svs[3] = svs_time;
        Intf.Tmr0_Svs_Status |= 0x8; 
        break;
     case 4:
        Intf.Tmr0_Svs[4] = svs_time;
        Intf.Tmr0_Svs_Status |= 0x10; 
        break;
     case 5:
        Intf.Tmr0_Svs[5] = svs_time;
        Intf.Tmr0_Svs_Status |= 0x20; 
        break;
     case 6:
        Intf.Tmr0_Svs[6] = svs_time;
        Intf.Tmr0_Svs_Status |= 0x40; 
        break;
     case 7:
        Intf.Tmr0_Svs[7] = svs_time;              
        Intf.Tmr0_Svs_Status |= 0x80; 
        break;
     default:
        break;   
   }

   /* test if timer0 has already been enabled */
   if(!TMR0_Is_Enabled())  
   { 
     #asm("cli");
     CLR_TOV0();                                  /* clear residual flag before enabling interrupt*/
     Set_10ms_Tick();                             /* 10ms interrupt */
     ENABLE_TMR0_INT();                           /* enable timer 0 overflow interrupt */
     #asm("sei");
   }
   /* If there is already a service in progress, 
   service time for succeedors will be shorter than
   specified, by one interrupt period (ie 10ms) max */
}

/******************************************************************************************************/
// Initialize interface pins
// This procedure should be called after PC download interface settings to masterboard 
// Hardware Abstract
// INTF1_MTRL_RDY PORTF.0|INTF1_FEED_DONE PORTF.1|INTF1_FORCE_RELEASE PORTF.2|INTF1_FEED_REQ PORTD.0 

// use offset_up_limit register as variable interface with PC
// If level trigger rather than edge trigger, use polling instead of interrupt.
/******************************************************************************************************/
void Init_interface()
{   
   u8 i;
   /* We need first to check if we are in handshake mode, if not, we will disable interrupt */
   if(!INTF_MODE_SHAKEHANDS)
   {
      EICRA=0x00; 
      EICRB=0xFF;
      EIMSK=0xF0;
      EIFR=0xF0;
      return;
   }
   /* set initial states for outputs: OR1/OF1/FR1 */
   DDRF |= 0xF;                                   /* set PORTF[4:0] as output */      
   switch(CONFIG_REG & PACKER_OF_MASK)            
   {
      case 0x0:                                   /* low or negative pulse to enable */
      case 0x8:
          PORTF |= 0x0F;                          /* so,initialize to high */
          break;
      case 0x10:
      case 0x18:                                  /* high or positive pulse to enable */
          PORTF &= 0xF0;                          /* so, initialize to low */
          break;
      default: 
          break;                                                            

   }
   
   Intf.max_feed_counter = 1 + ((CONFIG_REG & 0x0700) >> 8); 
   //Intf.max_feed_counter = 2; 
   Intf.feed_counter = 0;
   /* Settings for IF1/IF2, use External interrupt 0*/
   /* PORTD.0 has been set as input in Init_Port() */
   /* Make sure Ex_Int4~7 (used by 16C554) is included */   
   switch(CONFIG_REG & PACKER_IF_MASK) /* IF1 */
   {        
      case 0x00:      /* low level valid, use rising edge to detect end of request */
          PORTD.0=0;  /* set as input */
          PORTD.1=0;  /* set as input */
          new_request = TRUE;
      case 0x20:
          EICRA=0x0F; /*interrupt triggered by rising edge*/
          EICRB=0xFF;
          EIMSK=0xF3;
          EIFR=0xF3;         
          break;
      case 0x60:      /* high level valid, use falling edge to detect end of request */
          PORTD.0=0;  /* set as input */
          PORTD.1=0;  /* set as input */
          new_request = TRUE;
      case 0x40:
          EICRA=0x0A; /*interrupt triggered by falling edge*/
          EICRB=0xFF;
          EIMSK=0xF3;
          EIFR=0xF3;          
          break;
      default:
          EICRA=0x00; 
          EICRB=0xFF;
          EIMSK=0xF0;
          EIFR=0xF0;
          break;   
   }         
   
   /*Initialize interface variables*/
   if(CONFIG_REG & INIT_MASK)
      Intf.pack_reqs_pending = TRUE;             /* Weigher to send first */
   else
      Intf.pack_reqs_pending = FALSE;            /* packer to send first, wait for request */ 
      
   Intf.Tmr0_Svs_Status = 0;                     /* all services completed */
   Intf.weigher_is_ready = 0;                    /* initialize weigher status */
   for (i=0; i <MAX_SVS_NUM; i++)
      Intf.Tmr0_Svs[i] = 0;                      /* Reset timer services (pulse width) */   
   
}

/******************************************************************************************************/
// return 0x0 if packing machine is ready to receive another feeding. return 0xff if busy
/******************************************************************************************************/
u8 Packer_Is_Busy()
{  
    /* if in shakehands mode and no requests from packer is pending.*/
    /* variable "pack_reqs_pending" is set by ISR, it is declared as volatile type.*/
    if(INTF_MODE_SHAKEHANDS) {
       if(multi_feed_inprogress)   /**/
          return 0x0;
          
       if (((CONFIG_REG & PACKER_IF_MASK) == 0x00) && (PIND.0 = 0 ))   /* low valid */
         if(new_request)    /* last flag has been cleared. detected by ISR */
         {  new_request = FALSE;
            return 0x0;
         }
         else
            return 0xff;
            
       if (((CONFIG_REG & PACKER_IF_MASK) == 0x60) && (PIND.0 = 1 ))   /* high valid */
         if(new_request){
           new_request = FALSE;
           return 0x0;  
         }
         else
           return 0xff;

       /* request detected by interrupt */           
       if(!Intf.pack_reqs_pending)
         return 0xff; 
    }
    else
       return 0x0;
}
 
/******************************************************************************************************/
// When combination is available, call this function to inform packer that weigher is ready to release
// material 
// Output: negative or positive pulse, 
// this signal is to connect collector bucket
/******************************************************************************************************/
void Tell_Packer_Weigher_Rdy()
{  
   switch(CONFIG_REG & PACKER_OF_MASK)
   {
      case 0x0:                                   /* low to enable */
      case 0x8:
          PORTF &= (~(1<<PORT_OR1));              /* OR1(PORTF.0) = 0 */
          break;
      case 0x10:
      case 0x18:                                  /* high to enable */
          PORTF |= (1<<PORT_OR1);                 /* OR1 (PORTF.0) = 1 */
          break;
      default: 
          break;         
   }
   Intf.weigher_is_ready = TRUE;                  /* flag to inform interrupt 0 */
   Kickoff_Timer0(0, INTF_PLS_WIDTH);             /* start timer to generate pulse OR1. */   
}

/******************************************************************************************************/
// Inform packer that weigher has completed release of material       
// In some applications, material is packed after multiple feeds. Therefore packer will acknowledge
// to weigher only when weigher has sent release_done signals for n times. 
// Variable "feed_counter" is used to track whether weigher needs to await acknowledge signal from 
// packer.
/******************************************************************************************************/
//  send out release_done signal to packer
/**************************************************/
void send_packer_release_signal()
{  
   switch(CONFIG_REG & PACKER_OF_MASK)
   {
      case 0x0:                                   /* low to enable */      
          PORTF &= (~(1<<PORT_OF1));              /* assert low until packer response */              
          break;
      case 0x8:                                   /* negative pulse to enable */
          PORTF &= (~(1<<PORT_OF1));              /* assert low first */
          Kickoff_Timer0(1, INTF_PLS_WIDTH);      /* back to high when timeout*/
          break;
      case 0x10:
          PORTF |= (1<<PORT_OF1);                 /* assert high first */
          Kickoff_Timer0(1, INTF_PLS_WIDTH);      /* back to low when timeout*/
          break;
      case 0x18:                                  /* high to enable */
          PORTF |= (1<<PORT_OF1);                 /* assert high until packer response */
          break;
      default: 
          break;         
   } 
}
/***************************************************************/
// Tell packer material is release, packer can start to pack now.
// There is a time between multi-head-weigher release material and
// material drops into packer. So we allow user to set a delay here.
// In the case of single-release-single-pack and multi-release-single-pack
// mode, it is good enough to add the delay after last release and
// before sending out release_done signal to packer.
// To avoid waiting in this subroutine,  TMR0_SVS7 status bit is
// checked to decide whether we should go into this subroutine. 
// Accordingly cmd_loop() in main.c periodically call this subroutine
// and complete remaining actions in it if delay expires.
/***************************************************************/
u8 Tell_Packer_Release_Done()
{
   u16 signal_delay;
   static u8 signal_not_sent_yet;
      
   if(Intf.Tmr0_Svs_Status & 0x80)                 /* Tmr0_svs7(delay) is ongoing */
      return 0xff;                                 /* return and check again later*/           
   else if(signal_not_sent_yet)                    /* delay expires, and we do have signal to sent*/
   {
       send_packer_release_signal();               /* send packer release done signal*/   
       signal_not_sent_yet = 0;                    /* clear flag */
       return 0;                                   /* signal sent to packer, return (task done!) */
   }
   
   /* send out release_done signal or kick off timer to send signal later */  
   Intf.weigher_is_ready = FALSE;                  /* we are responding and completing this cycle, so clear flag for next cycle */      
   Intf.feed_counter++;                            /* how many times weigher has feed packer in this cycle */
   if(Intf.feed_counter < Intf.max_feed_counter)   /* packer needs more before it starts to pack */
   {  Intf.pack_reqs_pending = TRUE;               /* so we don't have to wait for acknowledge signal from packer to proceed */
      new_request = TRUE;                          /* workaround for level input */
      multi_feed_inprogress = TRUE;              
      send_packer_release_signal();                /* send packer release done signal*/
      return 0;                                    /* task done, return */
   }
   else                                            /* last feed, already gave packer what it claims */
   {  Intf.pack_reqs_pending = FALSE;              /* Now need to await packer's feedback to continue. */   
      new_request = FALSE;
      multi_feed_inprogress = FALSE;
      Intf.feed_counter = 0;                       /* reset counter */

      if((CONFIG_REG & SIGNAL_DELAY_MASK)&&(CONFIG_REG & 0xF800)) /* add delay between releaseing material and sending out signal?  */
      {   
         signal_delay = (CONFIG_REG & 0xF800)>>11;  /* 5 most significant bits:  get delay time */         
         signal_delay = signal_delay * 20;          /* X 20 */
         Kickoff_Timer0(7,signal_delay);            /* kickoff timer service 7 to start a delay, unit: 20*10ms */ 
         signal_not_sent_yet = 0xff;                /* we haven't sent release_done signal to packer yet */
         return(0xff);                              /* return 0xff to tell caller we have something to do in the future.*/
      }
      else
      {   send_packer_release_signal();             /* send packer release done signal*/         
      }
   }
   return 0;
}
                       
