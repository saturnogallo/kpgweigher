#include "window.h"
#include "lcd.h"
#include "stdio.h"
#include "stdlib.h"
//#include "sjDefine.h"
//MSG_HANDLER curr_window = 0;             


extern uchar pos_databuf;     
extern uchar data_sign;
extern u8 databuf[12];

/*¼ÆËã×Ö·û´®³¤¶È*/
uchar strlen(uchar *s)
{
	uchar len = 0;
	while(*s++) len ++;
	
	return len;
}

double buf2double()      
{                  
        return atof(databuf);
        /*
   double tmp = 0.0;
   uchar i = 0;
   uchar pos = 0;
   for(i=1;i<pos_databuf;i++) {
      if(databuf[i] != KEY_DOT)
         tmp = tmp * 10.0+(databuf[i] - '0');
      else
         pos = pos_databuf - i - 2;
   }
   while(pos > 0) {
      tmp = tmp / 10.0;
      pos--;
   }                                        
   if(data_sign == 1)
           tmp = -tmp;
          return tmp;
          */
}
int buf2byte()       //convert rundata.tempbuf to byte (00-99)
{       
        return atoi(databuf);
        /*
   int tmp = 0;
   uchar i;
   for(i=0;i<pos_databuf;i++) {
      tmp = tmp * 10+(databuf[i] - '0');
   }
   return tmp;
   */
}



extern uchar key;
u8 strbuf[12];
u8 databuf[12];
u8 pos_databuf; //position in data buffer
u8 max_databuf;
u8 data_sign;   // sign of the data
LABEL code datalbl = {LBL_HZ16,10,10,8,strbuf};
LABEL code datalbl2 = {LBL_HZ6X8,140,54,8,"DN:'E'"};
LABEL code datalbl3 = {LBL_HZ6X8,140,54,8,"UP/DN:'A'-'Z'"};
LABEL code databox = {LBL_HZ16,20,30,9,databuf};


uchar wnd_intinput(uchar lastval)
{
	uchar msg;
	
	key = MSG_INIT;
	databuf[0] = '\0';
	pos_databuf = 0;
        data_sign = 0;	
	while(1)
	{
		if(key != KEY_INVALID)
		{
			msg = key;
			
		}else{
			continue;
		}
		if(msg == MSG_INIT)
		{
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_inputbox(&databox, TRUE);
//			LCD_ShowCursor(databox.x,databox.y);

		}

		if(msg == KEY_TAB)
		{
		/*
        		if(pos_databuf < max_databuf)
        		{
			        databuf[pos_databuf++] = '0';
			        databuf[pos_databuf] = '\0';
        			msg = MSG_REFRESH;
                        }
                */
		}
		
		if(msg == KEY_DN) {
		/*
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '0')
				databuf[pos_databuf-1] = '9';
			else
				databuf[pos_databuf-1] -= 1;
			msg = MSG_REFRESH;
                */
		}
		if(msg == KEY_UP) {
		/*
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '9')
				databuf[pos_databuf-1] = '0';
			else
				databuf[pos_databuf-1] += 1;
			msg = MSG_REFRESH;
                */
		}
		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
        		if(pos_databuf < max_databuf)
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';
		        	msg = MSG_REFRESH;
		        }
		}
		if(msg == KEY_CE) {
//			LCD_HideCursor();
                        key = KEY_INVALID;
			return lastval;
		}
		if(msg == KEY_OK){
//			LCD_HideCursor();
                        key = KEY_INVALID;
			return buf2byte();
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}       
		key = KEY_INVALID;
	}
}

void draw_box(LABEL *lbl);
double wnd_floatinput(double lastval)
{
	uchar msg;

	databuf[0] = '+';
    databuf[1] = '\0';
    pos_databuf = 1;
    data_sign = 0;	
	max_databuf = 8;
	key = MSG_INIT;
	while(1)
	{
		if(key != KEY_INVALID)
		{
			msg = key;
	
		}else{
			continue;
		}
		if(msg == MSG_INIT)
		{
//			LCD_Cls();
//			draw_label(&datalbl,SW_NORMAL);
//			draw_label(&datalbl2,SW_NORMAL);
			draw_box(&databox);
			sprintf(databuf,"        ");
			msg = MSG_REFRESH;
//			draw_inputbox(&databox,TRUE);
//			LCD_ShowCursor(databox.x,databox.y);
		}

		if(msg == KEY_TAB)
		{                                          
		        msg = MSG_REFRESH;
		}
		/*
		if(msg == KEY_UP) {                                
		        if(pos_databuf == 1)
		        {              
		                if((data_sign & 0x01) == 0)      //no sign
		                {
		                        databuf[0] = '-';
		                        data_sign |= 0x01;
		                }else{
		                        databuf[0] = '+';       //
		                        data_sign ^= 0x01;
		                }
		        }else{
                       		if((pos_databuf < max_databuf) && (databuf[pos_databuf-2] == 'E'))
                		{                          
			                if((data_sign & 0x08) == 0)         
			                {
               		        	        databuf[pos_databuf-1] = '-';
               		        	        data_sign |= 0x08;
               		                }else{
               		        	        databuf[pos_databuf-1] = '+';
               		        	        data_sign ^= 0x08;
               		                }
	        		}        
		        	msg = MSG_REFRESH;
		        }
		        msg = MSG_REFRESH;
		}
		
		if(msg == KEY_DN) {
               		if((pos_databuf < max_databuf) && ((data_sign & 0x04) == 0))    //no E in string
        		{
        			databuf[pos_databuf++] = 'E';
        			databuf[pos_databuf++] = '+';
	        		databuf[pos_databuf] = '\0';                      
	        		data_sign |= 0x04;
		        	msg = MSG_REFRESH;
		        }
		
			msg = MSG_REFRESH;
		}
        */                              
		if( msg == KEY_DOT)
		{
               		if((pos_databuf < max_databuf) && ((data_sign & 0x02) == 0))      //no dot in string
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';                      
	        		data_sign |= 0x02;
		        	msg = MSG_REFRESH;
		        }

		}
		if((msg >= KEY_NUM0 && msg <= KEY_NUM9)) {
               		if(pos_databuf < max_databuf)
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';
		        	msg = MSG_REFRESH;
		        }
		}
		if(msg == KEY_CE) {
//			LCD_HideCursor();
			key = KEY_INVALID;
			return lastval;
		}
		if(msg == KEY_OK){
				key = KEY_INVALID;
//			LCD_HideCursor();
			return buf2double();
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL|SW_OVERLAP);
			msg = KEY_INVALID;
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}                                 
		key = KEY_INVALID;
	}
}
//display a message box
void wnd_msgbox(LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
	draw_label(lbl, SW_NORMAL);	
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);

}

//display a message box
void wnd_msgbox2(LABEL *lbl)
{
	draw_label(lbl, SW_NORMAL | SW_OVERLAP);	
	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);
}

void draw_box(LABEL *lbl)
{
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
//	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);
}
void draw_inputbox(LABEL *lbl,BOOL redraw)
{
	if(redraw)
	{
		LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
		LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	}
	draw_label(lbl, SW_NORMAL);	
}

void draw_label(LABEL *lbl,uchar reverse) {
	uchar len = lbl->width;

	if(lbl->type == LBL_HZ6X8) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 8);

			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
			return;			
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len , 8);
		return;
	}
	if(lbl->type == LBL_HZ8X16) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 16);
			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
			return;
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 1, 16);
		return;
	}
	
	if(lbl->type == LBL_HZ24X32) {
		if(lbl->param[0] == '-') //using small -
			lbl->param[0] = '~';
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 3, 32);
		return;
	}
	
	if(lbl->type == LBL_HZ12) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
		return;
	}
	if(lbl->type == LBL_HZ16) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 16);
			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
		return;
	}
}


