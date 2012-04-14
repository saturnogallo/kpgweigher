#include <MATH.H>
#include "typedef.h"       
#include "global.h"
#include "utili.h"
#include "key.h"
extern uchar key;         
extern u8 lcd_buffer;    
static u8 lcdcmd[40];
static uchar kbd_state;
static u8 head[3] = {MARK_HEAD,MARK_HEAD,MARK_HEAD};
static u8 tail[3] = {MARK_TAIL,MARK_TAIL,MARK_TAIL};

static u8 idle[10] = {MARK_HEAD,MARK_HEAD,MARK_HEAD,'1','2','E','D',MARK_TAIL,MARK_TAIL,MARK_TAIL};

static u8 hex2ch[2];  
void hex2char(uchar a)
{
        uchar h,l;
	h = (a & 0xf0) >> 4;
	l = (a & 0x0f);
	if(h <= 9)		
		hex2ch[0] = (h+'0');
	else
		hex2ch[0] = (h+'A'-0x0a);
	if(l <= 9)
		hex2ch[1] = (l+'0');
	else
		hex2ch[1] = (l+'A'-0x0a);               
}
void onecmd(uchar len)
{    
     uchar i = 0;
     uchar sum;
     sum = 0;
     prints(head,3,PORT_KBD); //send head
     for(i = 0;i<len;i++)     //caculate cksum     
     {              
        hex2char(lcdcmd[i]);
	prints(hex2ch,2,PORT_KBD);
        sum = sum + lcdcmd[i];
     }                        
     lcdcmd[i] = 0xff - sum;
     hex2char(lcdcmd[i]);
     prints(hex2ch,2,PORT_KBD);
     prints(tail,3,PORT_KBD); //send tail      
}
void sendcmd(uchar len)
{
     ulong timeout;  
     if(DEBUG == 1)
        return;
     while(kbd_state != STATUS_IDLE)
     {
        if(kbd_state == STATUS_DRAW)
                continue;
        if(kbd_state == STATUS_ERR)
        {
                prints(idle,10,PORT_KBD);
                kbd_state = STATUS_DRAW;     
        }   
     }                                  
     onecmd(len);  
     timeout = 0;
     kbd_state = STATUS_DONE;
     while(kbd_state != STATUS_IDLE)
     {
         if(kbd_state == STATUS_ERR)
         {
                onecmd(len);     
                timeout = 0;
         }
         if(timeout < 655350)
         {
                sleepms(1);
                timeout++;    
         }else{                    
                onecmd(len);
                timeout = 0;
         }
     }
}

/*
        Communication with LCD/KBD board,
        Incoming command format:
                CMDI_CLEARBUF   reset the LCD buffer count. (like the ack of the command)
                key:            key pressed
        Out command format:
                CMDO_DISPLAYBTN,LED_STATUS      update the led status                
*/
/**************************Ö÷³ÌÐò********************************/
void Key_Init()
{
	key = KEY_INVALID;
}

         

void kbd_uart_push(uchar data)
{           
        if(data == STATUS_IDLE||
           data == STATUS_ERR||
           data == STATUS_DRAW||
           data == STATUS_DONE) //lcd acknowledge
        {
                kbd_state = data;
                return;
        }  
        if(key == KEY_INVALID)                              
                key = data;
}  

void display_buttons(uchar btn_pos,uchar btn_val)   //send led status to sub board
{              
        lcdcmd[0] = CMDO_DISPLAYBTN;
        if(btn_val == 0)
	      btn_pos += 4; //change from 'a'-'d' to 'e'-'h'   //add 4
        lcdcmd[1] = btn_pos;      
        sendcmd(2);
}          

