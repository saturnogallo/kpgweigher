#include "window.h"
#include "utili.h"
#include "lcd.h"

MSG_HANDLER curr_window = 0;             

extern uchar key;

u8 databuf[12];
u8 pos_databuf; //position in data buffer
u8 max_databuf;
u8 data_sign;   // sign of the data
LABEL flash datalbl = {LBL_HZ16,10,10,8,strbuf};
LABEL flash datalbl2 = {LBL_HZ6X8,140,54,8,"UP:+/-,DN:'E'"};
LABEL flash datalbl3 = {LBL_HZ6X8,140,54,8,"UP/DN:'A'-'Z'"};
LABEL flash databox = {LBL_HZ16,20,30,9,databuf};


void prbsninput()
{
	uchar msg,len; 
	databuf[0] = '\0';
	pos_databuf = 0;
	key = MSG_INIT;
	while(1)
	{
		if(key == KEY_INVALID)
		        continue;

		msg = key;


		if(msg == MSG_INIT) {
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_label(&datalbl3,SW_NORMAL);
			sprintf(databuf,"");
			draw_inputbox(&databox);
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
                        key = KEY_INVALID;
                        return;
		}
		if(msg == KEY_CE) {	                  
		        key = KEY_INVALID;
			return;
		}

		if(msg == KEY_DN) {
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '0')
				databuf[pos_databuf-1] = 'Z';
			else if (databuf[pos_databuf-1] == 'A')
				databuf[pos_databuf-1] = '9';
			else
				databuf[pos_databuf-1] -= 1;
			msg = MSG_REFRESH;
		}
		if(msg == KEY_UP) {
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '9')
				databuf[pos_databuf-1] = 'A';
			else if (databuf[pos_databuf-1] == 'Z')
				databuf[pos_databuf-1] = '0';
			else
				databuf[pos_databuf-1] +=1;
			msg = MSG_REFRESH;
		}
		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
        		if(pos_databuf < max_databuf)
        		{
			        databuf[pos_databuf++] = msg;
        			databuf[pos_databuf] = '\0';
        		}
			msg = MSG_REFRESH;
		}
		if(msg == KEY_OK) {
		        if(IS_THERM_MODE)
		        {
			        for(len = 0; len < pos_databuf; len++)
        		                tprbdata.name[curr_prb-1][len] = databuf[len];
        			tprbdata.name[curr_prb-1][pos_databuf] = '\0';         
        		}else{
			        for(len = 0; len < pos_databuf; len++)
        		                rprbdata.name[curr_prb-1][len] = databuf[len];
        			rprbdata.name[curr_prb-1][pos_databuf] = '\0';         
        		}
			key = KEY_INVALID;
			break;
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}
		key = KEY_INVALID;
	}
//	LCD_HideCursor();
}

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
			draw_inputbox(&databox);
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
double wnd_floatinput(double lastval)
{
	uchar msg;

	databuf[0] = '+';
        databuf[1] = '\0';
        pos_databuf = 1;
        data_sign = 0;	
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
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_label(&datalbl2,SW_NORMAL);			
			draw_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}

		if(msg == KEY_TAB)
		{                                          
		        msg = MSG_REFRESH;
		}
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
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}                                 
				key = KEY_INVALID;
	}
}
//display a message box
void wnd_msgbox(flash LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
	draw_label(lbl, SW_NORMAL);	
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);

}

void draw_inputbox(flash LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	draw_label(lbl, SW_NORMAL);	
}

void draw_label(flash LABEL *lbl,uchar reverse) {
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
	/*
	if(lbl->type == LBL_HZ24X32) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 3, 32);
		return;
	}
	*/
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


