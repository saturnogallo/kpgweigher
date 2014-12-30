#include "sjDefine.h"

#define uchar	unsigned char

//0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6,0xff
unsigned char code led_code[]={0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,0x7F,0x6F,0xFF};//LED显示
                //0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0xFF,0x7F
/*
                 0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                 0x03,0x62,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
                 0xfd,0x61,0xdB,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xe6,
*/
#define LED_HF      0x08
#define LED_SPACE   0x00
#define LED_PT      0x80
#define LEDPT_OP    |

#define _nop_()	
//bits for LED display
sbit dat1 = P2^2;
sbit clk1 = P2^1;
sbit rst1 = P2^0;

void d164(uchar c)   //74164串行移位一个数
{
    uchar i=0;
     do{  
        if(c&0x01)     
              dat1=1;
        else
         dat1=0;
      c>>=1;        //数组tabel中的第N个数向右移动一位
      _nop_(); _nop_();
        clk1=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
        _nop_(); _nop_();
      clk1=1;
      _nop_(); _nop_();
        clk1=0;
      _nop_(); _nop_();
        i++;
    }
    while(i<8);
}

//bits for row output
sbit dat2 = P2^5;
sbit clk2 = P2^4;
sbit rst2 = P2^3;

void c164(uchar c)   //74164串行移位一个bit
{
        if(c)     
              dat2=1;
        else
         dat2=0;
      _nop_(); _nop_();
        clk2=0;            //下降沿将数据写入164，拉高时钟端停止，再拉低等待下一次写入
        _nop_(); _nop_();
      clk2=1;
      _nop_(); _nop_();
        clk2=0;
      _nop_(); _nop_();
}
void pause()
{
   while(1)
   {
      if(sjSerialWaitForOneByte() == '!')   
         return;
   }
}

#define POINTPOS   7   //position of point
#define MAXROWS      16   //total rows for one pan
#define FULLCOLS    11    //total pans
#define SCALECOLS   10   //total scale pan number
#define TYPECOL      10   //index of type pan 11th
#define TOTALLEDS   12   //total leds
#define MAXRETRY   10  //max retry times
xdata uchar keypos[FULLCOLS];
uchar wait;   //wait times count
extern void sleepms(unsigned int i);
/*
{
   while(i-- > 0)
      ;
}*/
uchar bUpdate = 1;



xdata char leds[TOTALLEDS]; //led code, total 12 LEDS,
xdata char out_12knob[TOTALLEDS]; //data buffer for serial output
xdata char cmp_leds[TOTALLEDS]; //buffer to compare the new data to old data

                     //   N001  N01  N1    P1   P10    P100   P1K   P10K    P100K   P1M  Z_SCA   Z_10   Z_1      Z_P1
//uchar code knobcol[MAXCOLS] = {0,    1,   2,   3,    4,     5,    6,     7,      8,     9,   10,      11,      12,      13};

//translate real col value to ideal col value //real col index to pan index
uchar code coltbls[16] = {1,3,5,7,9,0xff,0xff,0xff,0,0xff,0xff,10,8,6,4,2};
//uchar code coltbls[16] = {1,    3,   5,   7,    9,   0xff,  13,    11,   12,    10, 0xff,    8,   6,    4,   2,  0};
//translate real row value to ideal row value //real row index to num index
//uchar code rowtbls[16] = {1,0,5,6,7,2,3,4,9,8,0xff,0xff,0xff,10,11,2};
//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
uchar code rowtbls[16] = {9,10,5,4,3,8,7,6,1,2,0xff,0xff,0xff,0,11};
//parse the column position of 1 in P0 and P1
xdata uchar hitrow[FULLCOLS];
void getcols(uchar row)
{
   uchar pv0,pv1,pos;
   pv0 = P0;
   pv1 = P1;

   pos = 8;
   while(pos < 16)
   {
      if((pv1 & 0x01) == 0)
      {
         if((rowtbls[row] != 0xff) && (coltbls[pos] != 0xff)) 
         {
            hitrow[coltbls[pos]] = 1;
            if((keypos[coltbls[pos]] != rowtbls[row]) || (wait < MAXRETRY))
            {
/*debug only
               sjSerialSendByte(pos);
               sjSerialSendByte(keypos[coltbls[pos]]);
               sjSerialSendByte(row);
               sjSerialSendByte(rowtbls[row]);
               sjSerialSendByte('.');
               pause();
*/               
               bUpdate = 1;
            }
            keypos[coltbls[pos]] = rowtbls[row];
         }
      }
      pos++;
      pv1 = pv1 >> 1;
   }
   pos = 0;
   while(pos < 8)
   {
      if((pv0 & 0x01) == 0)
      {
         if((rowtbls[row] != 0xff) && (coltbls[pos] != 0xff)) 
         {
            hitrow[coltbls[pos]] = 1;
            if((keypos[coltbls[pos]] != rowtbls[row]) || (wait < MAXRETRY))
            {
/*debug only
               sjSerialSendByte(pos);
               sjSerialSendByte(keypos[coltbls[pos]]);
               sjSerialSendByte(row);
               sjSerialSendByte(rowtbls[row]);
               sjSerialSendByte('.');
               pause();
*/
               bUpdate = 1;
            }
            keypos[coltbls[pos]] = rowtbls[row];
         }
      }
      pos++;
      pv0 = pv0 >> 1;
   }
}
void clearrows()
{
   uchar i;
   for(i = 0; i < MAXROWS; i++) 
   {
      c164(1); //clear out all the output to 1   
   }
}

void scanrows()
{
   uchar i = 0;

   for(i = 0; i < FULLCOLS; i++)
      hitrow[i] = 0xff;
//      sjSerialSendByte(0xaa);
   for(i = 0; i < MAXROWS; i++)
   {
      c164(i);    //only 1 '0' will be shift out
      sleepms(100);

      getcols(i); //fill the column with current col value
//      sjSerialSendByte(P0);
//      sjSerialSendByte(P1);
//      if((P0 != 0xff) && (P1 != 0xff))
//         pause();

   }
//         sjSerialSendByte(0x55);
//   pause();
   clearrows();
   
   for(i = 0; i < FULLCOLS; i++)
   {
      if(hitrow[i] == 0xff) //no number found for one col
      {
//         if(keypos[i] != 0xff)
//            bUpdate = 1;   
         keypos[i] = 0xff;
      }
   }
}
//clear leds and out
void clear_12knob()
{
   uchar p;
   for(p = 0;p < TOTALLEDS;p++)
   {
      leds[p] = LED_SPACE; //delight all the lines
      out_12knob[p] = '0';
   }
}
//set le
void update_12knob()
{
   uchar pre = 0; //进位
   uchar p,i;

    leds[11] = led_code[0];
   out_12knob[11] = '0';
   for(p = 0;p < SCALECOLS;p++)
   {
      i = keypos[p] + pre;
      if(i == 0xff)
         break;
      if(i >= 10)
      {
         pre = 1;
         i = i-10;
      }else{
         pre = 0;
      }
      out_12knob[SCALECOLS-p] = i+'0';
      leds[SCALECOLS-p] = led_code[i];
   }
   leds[POINTPOS] = leds[POINTPOS] LEDPT_OP LED_PT;   //set the point
   if(pre == 1) //has pre
   {
      out_12knob[0] = '1';
      leds[0] = led_code[1];
   }else{
      out_12knob[0] = '0';
      leds[0] = led_code[0];
   }
   if(p < SCALECOLS)   //invalid value
   {
      clear_12knob();
      return;
   }
}
void output_12knob()
{
   uchar p;
   uchar head = 0; //whether leading non-zero value is found
   
   for(p = 0;p < TOTALLEDS;p++)
   {
      if(out_12knob[p] !='0')
      {
         head = 1;
      }
      if((p < POINTPOS) && (head == 0)) //remove leading zero
         continue;
      sjSerialSendByte(out_12knob[p]);
      if(p == POINTPOS)
         sjSerialSendByte('.');
   }
}


//display 17 led code in leds based on hardware setting
void led_show()
{
   uchar cnt;
   for(cnt = 0;cnt < TOTALLEDS;cnt++)
   {
      d164(LED_SPACE);
   }
   for(cnt = 0;cnt < TOTALLEDS;cnt++)
   {
      d164(leds[cnt]);
   }
}
void update_fixknob(char type)
{
   uchar cnt;
   for(cnt = 0;cnt < POINTPOS;cnt++)
   {
      leds[cnt] = LED_SPACE;
      out_12knob[cnt] = '0';
   }
   for(cnt = POINTPOS;cnt < TOTALLEDS;cnt++)
   {
      leds[cnt] = led_code[0];
      out_12knob[cnt] = '0';
   }
   leds[type+2] = led_code[1];
   out_12knob[type+2] = 1+'0';
   leds[POINTPOS] = leds[POINTPOS] LEDPT_OP LED_PT;
}
void update_scaleknob(int type)
{
   uchar pre = 0; //进位
   uchar d   = 0;
   uchar cnt;
   int kcnt;
   d = keypos[type];
   if(d >= 10)
   {
      pre = 1;
      d = d - 10;
   }
   for(cnt = 0;cnt < TOTALLEDS; cnt++)
   {
      kcnt = SCALECOLS-cnt;
      if(cnt < POINTPOS)
         leds[cnt] = LED_SPACE;
      else
         leds[cnt] = led_code[0];
      out_12knob[cnt] = '0';
      if((type+1) == kcnt)
      {
         if(pre > 0)
         {
            leds[cnt] = led_code[pre];
            out_12knob[cnt] = pre+'0';
         }
      }
      if((type) == (kcnt))
      {
         leds[cnt] = led_code[d];
         out_12knob[cnt] = d+'0';
      }
      if(type > kcnt)
      {
         leds[cnt] = led_code[0];
         out_12knob[cnt] = '0';
      }
   }
   for(cnt = 1;cnt  < POINTPOS;cnt++)
   {
      if((leds[cnt-1] == LED_SPACE) && ((leds[cnt] == led_code[0]) || (leds[cnt] == LED_SPACE)))
         leds[cnt] = LED_SPACE;
      else
         break;
   }

   leds[POINTPOS] = leds[POINTPOS] LEDPT_OP LED_PT;
}

void btnencode_loop()
{
	uchar type;
 while(1)
 {
      scanrows();
      type = keypos[TYPECOL];
//      type = 6; //debug only
      if(type < 12)   
      {
         if(type < 6)
            update_scaleknob(type);
         if(type == 6)
            update_12knob();
         if(type > 6)
            update_fixknob(type-2);
/*
         if((type <=4) || (type > 9))
         {
            if(type == 4)
               update_12knob();
            if(type > 9)
               update_scaleknob(type - 10);
            if(type < 4)
               update_scaleknob(type + 2);
         }else{
            update_fixknob(type);
         }
*/
      }else{
         clear_12knob();
      }
      if(bUpdate == 1)
      {
      /*
         if(wait == 0) 
         {   //copy leds to cmp_leds
            for(cnt = 0;cnt < TOTALLEDS; cnt++)
               cmp_leds[cnt] = leds[cnt];
            wait = 1;
            continue;
         }else{
            for(cnt = 0;cnt < TOTALLEDS; cnt++)
            {
               if(cmp_leds[cnt] != leds[cnt])
                  break;
            }            
            if(cnt < TOTALLEDS)
            {
               wait = 0;
               continue;
            }
            if(wait == MAXRETRY)
            {
            }
            if(wait < MAXRETRY)   //retry 10 time before update
            {
               wait++;
               continue;
            }
         }*/
         wait = MAXRETRY;
         led_show();

         bUpdate = 0;
      }
	}
}
