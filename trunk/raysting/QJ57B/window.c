#include "window.h"
#include "utili.h"
#include "lcd.h"

MSG_HANDLER curr_window = 0;             
MSG_HANDLER lp_ownerdraw = 0; //owner draw for label            
MSG_HANDLER lp_listvalue = 0; //prepare the list for value
MSG_HANDLER lp_listclear = 0; //clear the value in list
extern uchar key;

u8 databuf[12]; //store char of input box
u8 pos_databuf; //position in data buffer
u8 max_databuf; //limits of data input length


LABEL flash datalbl = {LBL_HZ16,10,10,8,strbuf};             //prompt label
LABEL flash datalbl2 = {LBL_HZ6X8,140,54,8,"UP:+/-,DN:'E'"}; //for data input
LABEL flash datalbl3 = {LBL_HZ6X8,140,54,8,"UP/DN:'A'-'Z'"}; //for string input
LABEL flash databox = {LBL_HZ16,20,30,9,databuf};            //box of inputs

u8* wnd_strinput()
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
	        key = KEY_INVALID;        	
		if(msg == MSG_INIT) {
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_label(&datalbl3,SW_NORMAL);
			sprintf(databuf,"");
			draw_inputbox(&databox);
		}
		if(msg == KEY_TAB)
		{ 
                        return NULL;
		}
		if(msg == KEY_CE) {	                  
			return NULL;
		}
		if(msg == KEY_DN) {
			if(pos_databuf == 0) //fill in the first char
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
			if(pos_databuf == 0) //fill in the first char
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
			        databuf[pos_databuf++] = (u8)(msg - KEY_NUM0 + '0');
        			databuf[pos_databuf] = '\0';
        		}
			msg = MSG_REFRESH;
		}
		if(msg == KEY_OK) {
		        return databuf;
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
		}
	}
}

uchar wnd_uintinput(uchar lastval)
{
	u8 msg;
        u8 data_sign = 0;   // sign of the data	
        
	key = MSG_INIT;
	databuf[0] = '\0';
	pos_databuf = 0;
	while(1)
	{
		if(key == KEY_INVALID)
		        continue;
		msg = key;
		key = KEY_INVALID;
		if(msg == MSG_INIT)
		{
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_inputbox(&databox);
		}

		if(msg == KEY_TAB)
		{
			return lastval;
		}
		if(msg == KEY_CE) {
			return lastval;
		}
		if(msg == KEY_DN) {
		}
		if(msg == KEY_UP) {
		}
		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
        		if(pos_databuf < max_databuf)
        		{
        			databuf[pos_databuf++] = msg;
	        		databuf[pos_databuf] = '\0';
		        	msg = MSG_REFRESH;
		        }
		}
		if(msg == KEY_OK){
			return buf2byte();
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
		}       
	}
}
double wnd_floatinput(double lastval)
{
	uchar msg;
        u8 data_sign = 0;   // sign of the data	
	databuf[0] = '+';
        databuf[1] = '\0';
        pos_databuf = 1;
	key = MSG_INIT;
	while(1)
	{
		if(key == KEY_INVALID)
		        continue;
		msg = key;
	        key = KEY_INVALID;		
		if(msg == MSG_INIT)
		{
			LCD_Cls();
			draw_label(&datalbl,SW_NORMAL);
			draw_label(&datalbl2,SW_NORMAL);			
			draw_inputbox(&databox);
		}

		if(msg == KEY_TAB)
		{                                          
			return lastval;
		}
		if(msg == KEY_CE) {
			return lastval;
		}
		
		if(msg == KEY_UP) {                                
		        if(pos_databuf == 1)
		        {              
		                if((data_sign & 0x01) == 0)      // sign '+'
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
		        }
		        msg = MSG_REFRESH;
		}
		if(msg == KEY_DN) {
               		if((pos_databuf < max_databuf) && (pos_databuf > 1) && ((data_sign & 0x04) == 0))    //no E in string
        		{
        			databuf[pos_databuf++] = 'E';
        			databuf[pos_databuf++] = '+';
	        		databuf[pos_databuf] = '\0';                      
	        		data_sign |= 0x04;
		        }
			msg = MSG_REFRESH;
		}                                      
		if( msg == KEY_DOT)
		{
               	if((pos_databuf < max_databuf) && (pos_databuf > 1) && ((data_sign & (0x02|0x04)) == 0))      //no dot in string and no E in string
        		{
        			databuf[pos_databuf++] = '.';
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
		if(msg == KEY_OK){
		        if((pos_databuf > 1) && (databuf[pos_databuf-1] >= '0') && (databuf[pos_databuf-1] <= '9'))
		        {
        			return buf2double();
        		}       
                	msg = MSG_REFRESH;        		
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
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
//select value from a menu
LABEL flash menupage = {LBL_HZ6X8,130,3,2,strbuf}; //page index
uchar wnd_menu(flash LABEL *banner,flash LABEL *labels,uchar max_option, uchar page_option) 
{
        uchar i;                        
        uchar msg;
 	static uchar curr_sel = 1;
	static uchar last_sel = 1;
	uchar min_option = 1;   
	key = MSG_INIT;  
	while(1)
	{
	        if(key == KEY_INVALID) // '-'
	                continue; 
	        msg = key;
	        key = KEY_INVALID;	        
        	if((msg == KEY_CE) || (msg == KEY_TAB))
        	{
        	        return KEY_CE;
        	}
        	if(msg == MSG_INIT)
        	{
       			LCD_Cls();
			if(banner != 0)
				draw_label(banner,SW_NORMAL);
       			msg = MSG_REFRESH;
        	}           
        	if(msg == KEY_DN)
        	{
        	        if((curr_sel+page_option) > max_option)
        	                continue;
        	        curr_sel  = curr_sel+page_option;
        	        msg = MSG_REFRESH;
        	}                         

        	if(msg == KEY_UP)
        	{
        	        if(curr_sel < page_option)
        	                continue;
        	        curr_sel  = curr_sel-page_option;
        	        msg = MSG_REFRESH;
        	}        	
        	if(msg == MSG_REFRESH)
        	{
			for(i = curr_sel; i < (curr_sel+page_option); i++)
       			    draw_label(&(labels[i-1]),SW_NORMAL);    
                        if(max_option > page_option)
                        {
               		        sprintf(strbuf,"********");
        		        strbuf[(u8)((max_option-1)/page_option)+1] = '\0';
		                strbuf[(u8)((curr_sel-1)/max_option)] = '0' + (u8)((curr_sel-1)/page_option);
        		        draw_label(&menupage,SW_NORMAL);
                        }
        	}                         
        	if((msg >= (u8)('0'+curr_sel)) && (msg < (u8)('0'+curr_sel+page_option)))
        	{
        	        return (u8)(msg-'0');
        	}
	}
       
}                        
extern u8 strbuf[];                                     
LABEL flash listpage = {LBL_HZ6X8,130,3,2,strbuf}; //page index
LABEL flash listitems[] = {
	{LBL_HZ6X8, 10, 30, 10, strbuf},
	{LBL_HZ6X8, 130,30, 10, strbuf},
	{LBL_HZ6X8, 10, 40, 10, strbuf},
	{LBL_HZ6X8, 130,40, 10, strbuf},
	{LBL_HZ6X8, 10, 50, 10, strbuf},
	{LBL_HZ6X8, 130,50, 10, strbuf}
};

//list box type : two column, six item one page             
#define MAX_LISTBOX_OPT         6
uchar wnd_listbox(flash LABEL *title, uchar max_index,uchar curr_pos)
{
        uchar i,j;
	uchar last_pos = curr_pos;
	uchar new_page = 0;\
	uchar msg;
	key = MSG_INIT;

	while(1)
	{
	        if(key == KEY_INVALID) // '-'
	                continue; 
	        msg = key;
	        key = KEY_INVALID;	        
        	if((msg == KEY_CE) || (msg == KEY_TAB))
        	{
        	        return 0xFF;
        	}
        	if(msg == KEY_NUM0)
        	        msg = MSG_INIT;
        	if(msg == KEY_DOT)
        	{                    
        	        curr_pos = curr_pos+MAX_LISTBOX_OPT;
        	        if(curr_pos > max_index)
        	                curr_pos = curr_pos - max_index;
        	        new_page = 1;                           
        	        msg = MSG_REFRESH;
        	}
        	if(msg == KEY_UP)
        	{
                        last_pos = curr_pos;   curr_pos--;
			if(curr_pos < 1) 
			        curr_pos = max_index;
			if(last_pos == curr_pos)  
			        continue;
			if((curr_pos % MAX_LISTBOX_OPT) == 0)  
			        new_page = 1;
			else
			        new_page = 0;
			        
       	      		msg = MSG_REFRESH;  
        	}
        	if(msg == KEY_DN)
        	{
                        last_pos = curr_pos;	curr_pos++;
			if(curr_pos > max_index) 
			        curr_pos = 1;
			if(last_pos == curr_pos)	
			        return;
			if((curr_pos % MAX_LISTBOX_OPT) == 1)  
			        new_page = 1;
			else
			        new_page = 0;        	
			        
       	      		msg = MSG_REFRESH;  
        	}
        	if(msg == KEY_OK)
        	{
        	        return (u8)curr_pos;
        	}
        	if((msg >= KEY_NUM1) && (msg <= KEY_NUM9) && (curr_pos <= 12)) //within 2 pages
        	{
        	        return (u8)(msg-KEY_NUM0);
        	}
			if(msg == KEY_DOT)
			{
				if(lp_listclear != 0)
				{
					lp_listclear(curr_pos);
					msg = MSG_REFRESH;
					new_page = 1;
				}
			}
        	if(msg == MSG_INIT)
        	{
       			LCD_Cls();
        		draw_label(title, SW_NORMAL);
		        new_page = 1;
       			msg = MSG_REFRESH;
        	}           
        	if(msg == MSG_REFRESH)
        	{
               		if(new_page == 1){
		               for(i = 1; i <= MAX_LISTBOX_OPT; i++){
			             j = ((curr_pos-1)/MAX_LISTBOX_OPT)*MAX_LISTBOX_OPT + i;
			             lp_listvalue(curr_pos);
			             draw_label(&listitems[i-1],SW_NORMAL);
        	                     if((curr_pos-1) % MAX_LISTBOX_OPT == (i-1))
					        draw_label(&(listitems[i-1]),SW_REVERSE);
        	                }
                         }else{
	                	for(i = 1; i <= MAX_LISTBOX_OPT; i++) {
        		              if((last_pos-1) % MAX_LISTBOX_OPT == (i-1)) 	
        		                        draw_label(&(listitems[i-1]),SW_REVERSE);
			              if((curr_pos-1) % MAX_LISTBOX_OPT == (i-1))	
				                draw_label(&listitems[i-1],SW_REVERSE);
			        }
		         }         
		         //add page index display .2..
		        sprintf(strbuf,"********");
		        strbuf[(u8)((max_index-1)/MAX_LISTBOX_OPT)+1] = '\0';
		        strbuf[(u8)((curr_pos-1)/MAX_LISTBOX_OPT)] = '0' + (u8)((curr_pos-1)/MAX_LISTBOX_OPT);
		        draw_label(&listpage,SW_NORMAL);
        	}                         
	}

} 

void draw_inputbox(flash LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	draw_label(lbl, SW_NORMAL);	
}

void draw_label(flash LABEL *lbl,uchar reverse) {
	uchar len = lbl->width;
	uchar type = (lbl->id_type & 0x07);
	uchar id = (lbl->id_type & 0xf8) >> 3;
	if(id != 0)
                lp_ownerdraw(id); //dynamic static label
	
	if(type == LBL_HZ6X8) {
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
	if(type == LBL_HZ8X16) {
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
	if(type == LBL_HZ24X32) {
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
	if(type == LBL_HZ12) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
		return;
	}
	if(type == LBL_HZ16) {
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


