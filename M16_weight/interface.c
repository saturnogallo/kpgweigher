/******************************************************************************************************/
//  Interface with packing machines
//  OR1(PORTB.4): OUT_READY, output to indicate at least one combination is available.
//  OF1(PORTB.5): OUT_Feed_Done, output to request packing machine to pack
//  OE1(PORTB.6): force release output to indicate current release is force release. 
//  IF1(PORTD.2): IN_Feed_Request. Request weigher to feed 
//  Note: PORTD.2 is external interrupt input 0               
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
// 1 11 10 0 1 0 
// 
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

#include <mega16.h>
#include "define.h"

#define MAX_SVS_NUM 8 
#define PACKER_OF_MASK      0x0018
#define PACKER_IF_MASK      0x0060  
#define NEED_HANDSHAKE      (packer_config & 0x0080)
#define ASYN_IF_MASK        0x0002
#define INIT_MASK           0x0001 
#define SIGNAL_DELAY_MASK   0x0004
#define INTF_MODE_SHAKEHANDS  (CONFIG_REG & 0x80)
#define CONFIG_REG      packer_config

u16 packer_config; 
u16 interface_pulse_width; 
extern void kick_off_timer(u8 task_id, u16 timer_len); 

typedef struct {
   volatile u8 pack_reqs_pending;              /* flag to indicate whether there is any 
                                                   pending reuqests from packer */                              
   volatile u8 weigher_is_ready;               /* weigher is available */
   volatile u8 multi_feed_inprogress; 
   volatile u8 max_feed_counter;               /* recorde how many times feed has been done */
   volatile u8 feed_counter;                   /* recorde how many times feed has been done */
   
}INTERFACE;

INTERFACE Intf;     

#define PIN_PACKER_REQ PIND.2
#define PENDING_REQ 0x3
#define NO_PENDING_REQ 0x0 
#define PACKER_BUSY 0xFF
#define PACKER_READY 0x0

/******************************************************************************************************/
// External Interrupt 0 service routine: feed request from packer
// Interrupt trigger type: Edge trigger. 
// Mark "Intf.pack_reqs_pending" to indicate whether packer is requesting feeding.
//    bit 1 = 1: rising edge detected; bit 0 = 1 falling edge detected; 
/******************************************************************************************************/
interrupt [EXT_INT0] void ext_int0_isr(void)
{                     
   if(hw_id != HW_ID_INTERFACE)                    /* board is not interface board */
   {
     GICR &= 0xBF;                                 /* clear bit 6 to disable int 0 */
     GIFR |= 0x40;                                 /* clear interrupt flag */
     return; 
   }
   /*debug info: packer request counter, byte 1 */
   /*if((RS485._global.cs_sys_offset_cal_data & 0x0000FF00) == 0x0000FF00)
        RS485._global.cs_sys_offset_cal_data &= 0xFFFF00FF; 
   else
        RS485._global.cs_sys_offset_cal_data += 0x100;*/    
   RS485._global.packer_intr_cnt++;
           
   /* clear interrupt flag,write "1" to bit 6 of GIFR. */
   GIFR |= 0x40;
   
   // read out remaining timer
   //RS485._global.cs_mtrl = os_sched.timerlen[0]; 
   // kill timer 0. 
   //os_sched.status &= 0xFE;
   //os_sched.timerlen[0] = 0;
   
   /* Set flag to indicate a request from packer is pending. */
   if(CONFIG_REG & ASYN_IF_MASK)                  /* Asyn setting, we should save pack request */
   {          
       if(PIN_PACKER_REQ)                         /* Current input level is high */
         Intf.pack_reqs_pending |= 0x02;          /* Set bit 1 to indicate rising edge */ 
       else
         Intf.pack_reqs_pending |= 0x01;          /* Set bit 0 to indicate falling edge */ 
   }
   else
   {                                              /* feed is set synchronized with IF singal*/
      if(!Intf.weigher_is_ready)
         Intf.pack_reqs_pending = 0;              /* ignore feed request */ 
      else                                        /* cnt edge cnt accordingly */
      {  if(PIN_PACKER_REQ)                     
            Intf.pack_reqs_pending |= 0x02;       /* Set bit 1 to indicate rising edge */ 
         else
            Intf.pack_reqs_pending |= 0x01;       /* Set bit 0 to indicate falling edge */ 
      }
   }
   
   Intf.feed_counter = 0;                         /* reset feed counter */
}

/******************************************************************************************************/
// Initialize interface pins
// This procedure should be called after PC download interface settings to masterboard 
// If level trigger rather than edge trigger, use polling instead of interrupt.
/******************************************************************************************************/
void Init_interface()
{      
   static u8 not_poweron_initialization;
       
   packer_config = RS485._flash.target_weight; 
   interface_pulse_width = (u16)RS485._flash.cs_Filter_option ; /* max pulse width 256*10 ms = 2.56s */    
   if(interface_pulse_width < 5)   /* minimum pulse width 50ms */
      interface_pulse_width = 5;
   
   /* We need first to check if we are in handshake mode, if not, we will disable interrupt */
   if(!INTF_MODE_SHAKEHANDS)
   {
      GICR &= 0xBF;                              /* clear bit 6 to disable int 0 */
      DDRD.2 = 0;                                /* PORTD.2 as input (int0)*/
      
      DDRB |= 0xF0;                              /* set PORTB[7:4] as output */ 
      PORTB |= 0xF0;                             /* output high to shut down interface */          
      return;
   }
   
   /* set initial states of interface signal */
   DDRB |= 0xF0;                                  /* set PORTB[7:4] as output */    
   switch(CONFIG_REG & PACKER_OF_MASK)            
   {
      case 0x0:                                   /* low or negative pulse to enable */
      case 0x8:
          PORTB &= 0x0F;                          /* output low to close relay */
          break;
      case 0x10:
      case 0x18:                                  /* high or positive pulse to enable */
          PORTB |= 0xF0;                          /* output high to open relay */
          break;
      default: 
          break;                                                            

   }
     
   if(!not_poweron_initialization)
   {  /* Settings for IF1, use External interrupt 0*/
      /* PORTD.2 has been set as input in Init_Port() */  
      DDRD.2=0;          /* set PORTD.2 (INT0)  as input */
      /* set to edge (rising/falling) trigger */
      MCUCR &=0xFB;      /* Interrupt triggered when PORTD.2 toggles */
      MCUCR |=0x1; 
      GIFR &= 0xBF;      /* Clear interupt flag (INTF0: bit 6) */
      GICR |= 0x40;      /* Enable INT0 interrupt */ 
      
      /* Initialize packer flag for the first time, for non-powerup initializations, use last state */
      Intf.pack_reqs_pending = NO_PENDING_REQ;   
      
      not_poweron_initialization = TRUE;                            
   }

   /*Initialize interface variables*/
   Intf.max_feed_counter = 1 + ((CONFIG_REG & 0x0700) >> 8); 
   Intf.feed_counter = 0;

   if(CONFIG_REG & INIT_MASK)
      Intf.pack_reqs_pending = PENDING_REQ;      /* set pending flag so that weigher won't wait */
   
   Intf.weigher_is_ready = 0;                    /* initialize weigher status */      
   Intf.multi_feed_inprogress = FALSE; 
}

/******************************************************************************************************/
// return 0x0 if packing machine is ready to receive another feeding. return 0xff if busy    
// "new_request"
//    TRUE:  request from packer is level valid and weigher has not responded yet
//    FALSE: request from packer is level valid and weigher has responded already
/******************************************************************************************************/
u8 Packer_Is_Busy()
{  
    /* if in shakehands mode and no requests from packer is pending.*/
    /* variable "pack_reqs_pending" is set by ISR, it is declared as volatile type.*/
    if(INTF_MODE_SHAKEHANDS) 
    {
       if(Intf.multi_feed_inprogress)            /* packer needs multiple feeds, no need to wait */
          return PACKER_READY;
              
       /* Packer request is configured as low valid */           
       if ((CONFIG_REG & PACKER_IF_MASK) == 0x00)
       {  /* Release material only when falling edge detected (new reqeust). */
          /* edge flag is cleared when responding to packer (Tell_Packer_Release_Done) */
          //if((!PIN_PACKER_REQ) && (Intf.pack_reqs_pending & 0x1))    /* bit 0: falling edge flag */
          if(!PIN_PACKER_REQ)
          {  if((Intf.pack_reqs_pending & 0x1))
                 return PACKER_READY;
             else
             {
                sleepms(150);
                if(!PIN_PACKER_REQ)
                   return PACKER_READY;
                else
                   return PACKER_BUSY;
             }
          }
          else
             return PACKER_BUSY;
       }          
       /* Packer request is configured as high valid */             
       else if ((CONFIG_REG & PACKER_IF_MASK) == 0x60)
       {  /* Release material only when input valid and rising edge detected (new reqeust). */
          /* edge flag is cleared when responding to packer (Tell_Packer_Release_Done) */
          //if((PIN_PACKER_REQ) && (Intf.pack_reqs_pending & 0x2))     /* bit 1: rising edge flag */
          if(PIN_PACKER_REQ)
          {  if((Intf.pack_reqs_pending & 0x2))
                return PACKER_READY;
             else
             {  sleepms(150);
                if(PIN_PACKER_REQ)
                   return PACKER_READY;
                else
                   return PACKER_BUSY;             
             }
          }
          else 
             return PACKER_BUSY;
       }       
       /* packer request is configured as rising edge valid */           
       else if ((CONFIG_REG & PACKER_IF_MASK) == 0x20)
       {  if(Intf.pack_reqs_pending & 0x2)
             return PACKER_READY; 
          else
             return PACKER_BUSY; 
       }
       /* packer request is configured as falling edge valid */           
       else 
       {
          if(Intf.pack_reqs_pending & 0x1)
             return PACKER_READY; 
          else
             return PACKER_BUSY;        
       } 
    }
    else  /* no shakehands, don't care about packer */
       return PACKER_READY;
}
 
/******************************************************************************************************/
// When combination is available, call this function to inform packer that weigher is ready to release
// material 
// Output: negative or positive pulse, 
// this signal is to connect collector bucket
/******************************************************************************************************/
#define PIN_WEIGHER_READY PORTB.4
#define TASK_OR 3
void Tell_Packer_Weigher_Rdy()
{  
   switch(CONFIG_REG & PACKER_OF_MASK)
   {
      case 0x0:                                   /* low to enable */
      case 0x8:
          PIN_WEIGHER_READY = 0;
          break;
      case 0x10:
      case 0x18:                                  /* high to enable */
          PIN_WEIGHER_READY = 1;
          break;
      default: 
          break;         
   }
   Intf.weigher_is_ready = TRUE;                    /* flag to inform interrupt 0 */
   kick_off_timer(TASK_OR, interface_pulse_width);  /* start timer to generate pulse OR1. */   
}

/******************************************************************************************************/
// When material in a bucket is force released due to some reasons (for example overweight), master 
// issues command to call this subroutine to send out signal indicating current activity is a force 
// release. 
/******************************************************************************************************/
#define PIN_FORCE_RELEASE PORTB.6
#define TASK_OE 5     
void Tell_Packer_Force_Release()
{  
   switch(CONFIG_REG & PACKER_OF_MASK)
   {
      case 0x0:                                   /* low to enable */
      case 0x8:
          PIN_FORCE_RELEASE = 0;
          break;
      case 0x10:
      case 0x18:                                  /* high to enable */
          PIN_FORCE_RELEASE = 1;
          break;
      default: 
          break;         
   }

   kick_off_timer(TASK_OE, interface_pulse_width);  /* start timer to generate pulse OR1. */   
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
#define PIN_FEED_PACKER PORTB.5
#define TASK_OF 4
void send_packer_release_signal()
{  
   
   switch(CONFIG_REG & PACKER_OF_MASK)
   {
      case 0x0:                                   /* low to enable */      
          PIN_FEED_PACKER = 0;                    /* assert low until packer response */              
          //break;
      case 0x8:                                   /* negative pulse to enable */
          PIN_FEED_PACKER = 0;                    /* assert low first */
          kick_off_timer(TASK_OF, interface_pulse_width);/* back to high when timeout*/
          break;
      case 0x18:                                  /* high to enable */
          PIN_FEED_PACKER = 1;                    /* assert high until packer response */
          //break;
      case 0x10:
          PIN_FEED_PACKER = 1;                    /* assert high first */
          kick_off_timer(TASK_OF, interface_pulse_width);/* back to low when timeout*/
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
// To avoid waiting in this subroutine,  TMR0_SVS2 status bit is
// checked to decide whether we should go into this subroutine. 
// Accordingly main.c periodically call this subroutine
// and complete remaining actions in it if delay expires.
/***************************************************************/
#define TASK_DELAY 2

u8 Tell_Packer_Release_Done()
{
   u16 signal_delay;
   static u8 release_signal_not_sent_yet; 
   static u8 wait_for_signal_complete; 
      
   /* Check if delay (releasing material <-> sending signal) is ongoing */   
   if(os_sched.status & 0x4)                       /* Task 2(feed delay) is ongoing */
       return 0xff;                                /* return and check again later*/           
   else if(release_signal_not_sent_yet)            /* delay expires, and we do have signal to sent*/
   {
       send_packer_release_signal();               /* send packer release done signal*/   
       release_signal_not_sent_yet = FALSE;        /* clear flag */
       wait_for_signal_complete = TRUE; 
       return 0xff;                                   /* signal sent to packer, return (task done!) */
   }
   
   /* wait for release signal to be sent out complete */   
   if (wait_for_signal_complete)                        
   {
      if (os_sched.status & 0x10)      /* wait for pulse width to complete */
          return 0xff;
      else
      {
          wait_for_signal_complete = FALSE;
          /*debug info: counter of signal, byte 4*/
          /*if((RS485._global.cs_sys_offset_cal_data & 0xFF000000) == 0xFF000000)
              RS485._global.cs_sys_offset_cal_data &= 0x00FFFFFF;
          else
              RS485._global.cs_sys_offset_cal_data += 0x1000000; */
          RS485._global.packer_release_cnt++;   
              
          return 0x0;     
      }
   }
     
   
   /* send out release_done signal or kick off timer to send signal later */  
   Intf.weigher_is_ready = FALSE;                  /* we are responding and completing this cycle, so clear flag for next cycle */      
   Intf.feed_counter++;                            /* how many times weigher has feed packer in this cycle */
   if(Intf.feed_counter < Intf.max_feed_counter)   /* packer needs more before it starts to pack */
   {  
      Intf.pack_reqs_pending = PENDING_REQ;        /* so we don't have to wait for acknowledge signal from packer to proceed */
      Intf.multi_feed_inprogress = TRUE;              
      //send_packer_release_signal();              /* send packer release done signal*/
      return 0;                                    /* task done, return */
   }
   else                                            /* last feed, already gave packer what it claims */
   {  Intf.pack_reqs_pending = NO_PENDING_REQ;     /* Now need to await packer's feedback to continue. */   
      Intf.multi_feed_inprogress = FALSE;
      Intf.feed_counter = 0;                       /* reset counter */

      //if((CONFIG_REG & SIGNAL_DELAY_MASK)&&(CONFIG_REG & 0xF800)) /* add delay between releaseing material and sending out signal?  */
      if(CONFIG_REG & 0xF800)    /* need delay before sending out signal */
      {   
         signal_delay = (CONFIG_REG & 0xF800)>>11;  /* 5 most significant bits:  get delay time */         
         signal_delay = signal_delay * 20;          /* X 20 */
         kick_off_timer(TASK_DELAY,signal_delay);   /* kickoff timer service 2 to start a delay, unit: 20*10ms */ 
         release_signal_not_sent_yet = 0xff;        /* we haven't sent release_done signal to packer yet */         
         return(0xff);                              /* return 0xff to tell caller we have something to do in the future.*/
      }
      else                     /* send out signal immediately */
      {   send_packer_release_signal();             /* send packer release done signal*/
          wait_for_signal_complete = TRUE; 
          return 0xff;         
      }
   }
   
   return 0;
}
                       
