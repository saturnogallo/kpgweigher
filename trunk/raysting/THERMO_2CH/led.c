#include "define.h"

#define DIGI_NUMBER     8   

/*
*       Every digital structure looks like this
*       has_point(1bit) flash(1bit) reserver(1bit)   data(5 bits)
*
*/
#define CHAR_CODE_MASK  0x70
#define POINT_ON        0x80
#define FLASH_ON        0x40        
#define CHAR_0          0x00     //char 0 to char 9 is 0-9
#define CHAR_A          0x0a     //A
#define CHAR_b          0x0b     //b
#define CHAR_c          0x0c     //c
#define CHAR_d          0x0d     //d
#define CHAR_E          0x0e     //E
#define CHAR_F          0x0f     //F
#define CHAR_r          0x10     //r
#define CHAR_o          0x11     //o
#define CHAR_t          0x12     //t
#define CHAR_n          0x13     //n
#define CHAR_L		0x14	 //L
#define CHAR_Y		0x15	 //Y
#define CHAR_h		0x16	 //h
#define CHAR_U		0x17	 //U
#define CHAR_N		0x18	 //N
#define CHAR_SPACE      0x19     // 
#define CHAR_MINUS      0x1A     //- 
#define CHAR_MAX_NUM	0x1B

// *******************
// CH453的定义
// *******************
#define CH453_DIG0      0x6000                      //数码管位0显示
#define CH453_DIG1      0x6200		            //数码管位1显示 
#define CH453_DIG2      0x6400		            //数码管位2显示
#define CH453_DIG3      0x6600		            //数码管位3显示
#define CH453_DIG4      0x6800		            //数码管位4显示
#define CH453_DIG5      0x6A00	                    //数码管位5显示	
#define CH453_DIG6      0x6C00	                    //数码管位6显示	
#define CH453_DIG7      0x6E00		            //数码管位7显示

#define CH453_LEDA		0x7800		    //数码管位12 for led
#define CH453_LEDB		0x7A00		    //数码管位13 for led


static u8 digi_data[DIGI_NUMBER] = {0,0,0,0,0,0,0,0}; //8 digital LEDs
static flash u8 digi_code[CHAR_MAX_NUM] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\ 
                                                0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                                0x00,0x00,0x00,0x00,0x00};
static u16 reg_led;             //16 leds on off status
static u16 reg_led_flash;       //16 leds flash status
static u8 flash_cnt = 0;        //counter of timer for flash

extern void Led_Output(u16);
extern void Digi_Output(u8,u8);          
extern void CH453_Write( u16 );
void digi_print(u8 *str,uchar startpos, uchar size);
void timer_tick()
{
	u8 i = 0;
	flash_cnt = 1 - flash_cnt;
	if(flash_cnt)
	{	
		Led_Output(reg_led | reg_led_flash);
	}else{
		Led_Output(reg_led ^ reg_led_flash);
	}
	for(i=0;i<DIGI_NUMBER;i++)
	{
		if(digi_data[i] & FLASH_ON)
		{
			if(flash_cnt)
			{
				Digi_Output(i,digi_code[CHAR_CODE_MASK & digi_data[i]] | ((digi_data[i] & POINT_ON) >> 4));
			}else{
				Digi_Output(i,digi_code[CHAR_SPACE] | ((digi_data[i] & POINT_ON) >> 4));
			}
		}else{
		        Digi_Output(i,digi_code[CHAR_CODE_MASK & digi_data[i]] | ((digi_data[i] & POINT_ON) >> 4));
		}
	}
}
void Digi_Output(u8 pos, u8 val)
{                                      
        switch(pos)
        {
                case 0: CH453_Write(CH453_DIG0 | val); break;
                case 1: CH453_Write(CH453_DIG1 | val); break;
                case 2: CH453_Write(CH453_DIG2 | val); break;
                case 3: CH453_Write(CH453_DIG3 | val); break;
                case 4: CH453_Write(CH453_DIG4 | val); break;
                case 5: CH453_Write(CH453_DIG5 | val); break;
                case 6: CH453_Write(CH453_DIG6 | val); break;
                case 7: CH453_Write(CH453_DIG7 | val); break;
                default: break;                
        }
}
void init_digi_Led()
{             
    reg_led = 0;
    reg_led_flash = 0;
} 
void led_on(u16 key, bool on)
{
	if(on){
		reg_led |= key;
	}else{
		reg_led = reg_led & (key ^ 0xffff);
	};
}
void led_flash(u16 key, bool on)
{
	if(on){
		reg_led_flash |= key;
	}else{                         
		reg_led_flash = reg_led_flash & (key ^ 0xffff);
	}
}
void digi_flash(uchar startpos, uchar endpos, bool on)
{
	while(startpos <= endpos)
	{
		if(on)
			digi_data[startpos] |= FLASH_ON;
		else
			digi_data[startpos] = digi_data[startpos] & (0xff^FLASH_ON);
		startpos++;
	}
}
void digi_print(u8 *str,uchar startpos, uchar size)
{     
   u8 i;               
   u8 endpos = min(DIGI_NUMBER,startpos+size);
   for(i=startpos; i < endpos; i++){
      switch(*str){
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9': digi_data[i] = (digi_data[i] & CHAR_CODE_MASK) | (*str -'0'); break;
         case 'A': digi_data[i] = CHAR_A; break;
         case 'b': digi_data[i] = CHAR_b; break;
         case 'c': digi_data[i] = CHAR_c; break;
         case 'd': digi_data[i] = CHAR_d; break;
         case 'E': digi_data[i] = CHAR_E; break;
         case 'F': digi_data[i] = CHAR_F; break;
         case 'r': digi_data[i] = CHAR_r; break;
         case 'o': digi_data[i] = CHAR_o; break;
         case 't': digi_data[i] = CHAR_t; break;
         case 'n': digi_data[i] = CHAR_n; break;
         case 'U': digi_data[i] = CHAR_U; break;
         case 'N': digi_data[i] = CHAR_N; break;
         case '-': digi_data[i] = CHAR_MINUS; break;
         case '.':  digi_data[i-1] |= POINT_ON; 
               i--;
               break;
         default: digi_data[i] = CHAR_SPACE; //no show
               break;
         }
      str++;
   }
}
//const string print
void digi_cprint(u8 flash *str,uchar startpos, uchar size)
{     
   u8 i;               
   u8 endpos = min(DIGI_NUMBER,startpos+size);
   for(i=startpos; i < endpos; i++){
      switch(*str){
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9': digi_data[i] = (digi_data[i] & CHAR_CODE_MASK) | (*str -'0'); break;
         case 'A': digi_data[i] = CHAR_A; break;
         case 'b': digi_data[i] = CHAR_b; break;
         case 'c': digi_data[i] = CHAR_c; break;
         case 'd': digi_data[i] = CHAR_d; break;
         case 'E': digi_data[i] = CHAR_E; break;
         case 'F': digi_data[i] = CHAR_F; break;
         case 'r': digi_data[i] = CHAR_r; break;
         case 'o': digi_data[i] = CHAR_o; break;
         case 't': digi_data[i] = CHAR_t; break;
         case 'n': digi_data[i] = CHAR_n; break;
         case 'U': digi_data[i] = CHAR_U; break;
         case 'N': digi_data[i] = CHAR_N; break;
         case '-': digi_data[i] = CHAR_MINUS; break;
         case '.':  digi_data[i-1] |= POINT_ON; 
               i--;
               break;
         default: digi_data[i] = CHAR_SPACE; //no show
               break;
         }
      str++;
   }
}

void Led_Output(u16 data)
{
        u8 key;
        key = (u8)(data & 0x00ff);
        CH453_Write(CH453_LEDA | key);
        key = (u8)(data >> 8);
        CH453_Write(CH453_LEDB | key);
}