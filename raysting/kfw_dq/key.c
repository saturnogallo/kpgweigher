    #include <stc51.h>
	#include <stdio.h>
	#include <intrins.h>
	#include <MATH.H>
	#include "utili.h"
	#include "key.h"
	#define uchar unsigned char
	#define uint unsigned int
	#define ulong unsigned long
	#define KEYTIMEOUT	1

    sbit l9= P3^6;   //定义时钟端164

	bit  KEYBJ2	;	//KEY_DOWN event
    uchar keykeep;
	uint  keycount;	//delay counter

	sbit kbit = P3^2; //key int
extern uchar key;
void decode_key()
{
   uchar k;
   P1 = 0x0F;l9=1;//低位置○，准备查询按键
   k  = P1;       //取得当前P1口的状态
  
   if(k != 0x0F)
   {//如果有变化则表示有键按下
     k = 0xef;
   do{//循环扫描每一行
     P1 = k;
     if(k != P1)
     {
     switch(P1)
       {
      //第一行
      case 0x77:{keykeep=KEY_OK;break;}
      case 0xb7:{keykeep=KEY_CE;break;}
      case 0xd7:{keykeep=KEY_NUM0;break;}
      case 0xe7:{keykeep=KEY_DOT;break;}
      //第二行
      case 0x7b:{keykeep=KEY_DN;break;}
      case 0xbb:{keykeep=KEY_NUM3;break;}
      case 0xdb:{keykeep=KEY_NUM2;break;}
      case 0xeb:{keykeep=KEY_NUM1;break;}
      //第三行
      case 0x7d:{keykeep=KEY_UP;break;}
      case 0xbd:{keykeep=KEY_NUM6;break;}
      case 0xdd:{keykeep=KEY_NUM5;break;}
      case 0xed:{keykeep=KEY_NUM4;break;}
      //第四行
      case 0x7e:{keykeep=KEY_TAB;break;}
      case 0xbe:{keykeep=KEY_NUM9;break;}
      case 0xde:{keykeep=KEY_NUM8;break;}
      case 0xee:{keykeep=KEY_NUM7;break;}
      default:

	   break;
       }
      } 
     k = _crol_(k,1);//移位，进入下一行扫描
    }while(k !=0xef);//超过范围退出    
  }

 P1 = 0xff;l9=0;  //第五列如下
   k = P1;
  if(k != 0xff)
     {
     switch(P1)
        {          //第五行
        case 0xf7:{keykeep=KEY_BTN4; break;}
        case 0xfb:{keykeep=KEY_BTN3;break;}
        case 0xfd:{keykeep=KEY_BTN2; break;}
        case 0xfe:{keykeep=KEY_BTN1;break;}
	    default:
	    break;
	    }	
	  }   
    P1 = 0x0f;l9=0;	 
 }  
  
/**************************主程序********************************/
void Key_Init()
{
	KEYBJ2 = 0;			  //key down event
	keycount = 0;
	keykeep = KEY_INVALID;
    P1 = 0x0f;l9=0;	 
}
void key_debug(char* buf)
{
	if(KEYBJ2)
		sprintf(buf,"1,%i,%c",keycount,keykeep);
	else
		sprintf(buf,"0,%i,%c",keycount,keykeep);
}

void Key_Scan()
{
	uchar k;
	if(KEYBJ2 == 0){
		if(kbit == 0)
		{
			KEYBJ2 = 1;
			keycount = 0;
		}
		return;
	}

		k = keykeep;
		keykeep = KEY_INVALID;
		decode_key(); //decode the key
		if(keycount == 0) //initial decode
		{
			if(keykeep == KEY_INVALID) //invalid key
			{
				KEYBJ2 = 0;
				P1 = 0x0f;l9=0;	 
				return;
			}
		}
		if(keykeep == KEY_INVALID) //key up event found
		{
			KEYBJ2 = 0;
 			P1 = 0x0f;l9=0;

			if(keycount >= KEYTIMEOUT)
				key = k;
			return;
		}
		keycount++;
		if(keycount > 128)
			keycount = 1;
	return;

}
sbit sca_clk = P0^7;
sbit sca_dat = P0^6;
void display_buttons(uchar btn_pos,uchar btn_val)   //74164串行移位一个数
{
         uchar i=0;
		 uchar c= 0;
		 if(btn_val){
			 switch(btn_pos)
			 {
 				case KEY_BTN1:  c = c | 0x11;break;
 				case KEY_BTN2:  c = c | 0x22;break;
 				case KEY_BTN3:  c = c | 0x44;break;
	 			case KEY_BTN4:  c = c | 0x88;break;
				default:
					return;
		 	}
		}else{
			switch(btn_pos)
			{
 				case KEY_BTN1:  c = c & 0xEE;break;
 				case KEY_BTN2:  c = c & 0xDD;break;
 				case KEY_BTN3:  c = c & 0xBB;break;
	 			case KEY_BTN4:  c = c & 0x77;break;
				default:
					return;
		 	}
		}

         do{  
             if(c&0x01)     
                 sca_dat=1;
             else
				 sca_dat=0;
		     c>>=1;        //数组tabel中的第N个数向右移动一位
			 _nop_(); _nop_();
             sca_clk=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
             _nop_(); _nop_();
			 sca_clk=1;
			  _nop_(); _nop_();
             sca_clk=0;
			  _nop_(); _nop_();
             i++;
            }
        while(i<8);
}
