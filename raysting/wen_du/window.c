#include "window.h"
#include "utili.h"
#include "lcd.h"

MSG_HANDLER curr_window = 0;
MSG_HANDLER caller = 0;

char xdata databuf[12];
uchar pos_databuf;
uchar max_databuf;
const LABEL code datalbl = {LBL_HZ16,30,30,8,strbuf};
const LABEL code databox = {LBL_HZ16,30,30,8,databuf};

uchar wnd_sninput(uchar msg)
{
	if(msg == KEY_TAB)
		return KEY_TAB;
	if(msg == KEY_CE) {	
		return KEY_CE;
	}
	if(pos_databuf >= max_databuf)
		return 0;
	if(msg == KEY_DN) {
		if(databuf[pos_databuf] == '0')
			databuf[pos_databuf] = 'Z';
		else if (databuf[pos_databuf] == 'A')
			databuf[pos_databuf] = '9';
		else
			databuf[pos_databuf] -= 1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		if(databuf[pos_databuf] == '9')
			databuf[pos_databuf] = 'A';
		else if (databuf[pos_databuf] == 'Z')
			databuf[pos_databuf] = '0';
		else
			databuf[pos_databuf]++;
		msg = MSG_REFRESH;
	}
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
		databuf[pos_databuf++] = msg;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_OK) {
		return KEY_OK;
	}
	if(msg == MSG_INIT) {
		databuf[0] = '\0';
		pos_databuf = 0;
		draw_label(&datalbl,SW_NORMAL);
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		draw_label(&databox,SW_NORMAL);
	}
	return 0;
}

uchar wnd_intinput(uchar msg)
{
	if(msg == KEY_TAB)
		return KEY_TAB;
	if(msg == KEY_CE) {	
		databuf[0] = '0';
		pos_databuf = 0;
		msg = MSG_REFRESH;
	}
	if(pos_databuf >= max_databuf)
		return 0;
	if(msg == KEY_DN) {
		if(databuf[pos_databuf] == '0')
			databuf[pos_databuf] = '9';
		else
			databuf[pos_databuf] -= 1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		if(databuf[pos_databuf] == '9')
			databuf[pos_databuf] = '0';
		else
			databuf[pos_databuf]++;
		msg = MSG_REFRESH;
	}
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
		databuf[pos_databuf++] = msg;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_OK) {
		databuf[pos_databuf++] = '0';
		msg = MSG_REFRESH;
	}
	if(msg == MSG_INIT) {
		databuf[0] = '0';
		pos_databuf = 0;
		draw_label(&datalbl,SW_NORMAL);
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		draw_label(&databox,SW_NORMAL);
	}
	return 0;
}
uchar wnd_floatinput(uchar msg)
{
	if(msg == KEY_TAB)
		return KEY_TAB;
	if(msg == KEY_CE) {	
		databuf[0] = '0';
		pos_databuf = 0;
		msg = MSG_REFRESH;
	}
	if(pos_databuf >= max_databuf)
		return 0;
	if(msg == KEY_DN) {
		if(databuf[pos_databuf] == '0')
			databuf[pos_databuf] = '9';
		else
			databuf[pos_databuf] -=  1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		if(databuf[pos_databuf] == '9')
			databuf[pos_databuf] = '0';
		else
			databuf[pos_databuf]++;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DOT) {
		databuf[pos_databuf++] = msg;
		msg = MSG_REFRESH;
	}
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
		databuf[pos_databuf++] = msg;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_OK) {
		databuf[pos_databuf++] = '0';
		msg = MSG_REFRESH;
	}
	if(msg == MSG_INIT) {
		databuf[0] = '0';
		pos_databuf = 0;
		draw_label(&datalbl,SW_NORMAL);
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		draw_label(&databox,SW_NORMAL);
	}
	return 0;
}

//display a message box
void wnd_msgbox(LABEL *lbl)
{
	LCD_Cls();
	draw_label(lbl, SW_NORMAL);	
}

void draw_label(LABEL *lbl,uchar reverse) {
	uchar len = strlen(lbl->param);
	if(lbl->type == LBL_HZ6X8) {
		if(reverse & SW_NORMAL) {
			LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 6, lbl->y + 8);
			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
			return;			
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 6, 8);
		return;
	}
	if(lbl->type == LBL_HZ8X16) {
		if(reverse & SW_NORMAL) {
			LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 8, lbl->y + 16);
			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
			return;
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 8, 16);
		return;
	}
	if(lbl->type == LBL_HZ24X32) {
		if(reverse & SW_NORMAL) {
			LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 24, 32);
		return;
	}
	if(lbl->type == LBL_HZ12) {
		if(reverse & SW_NORMAL) {
			LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 16, lbl->y + 12);
			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 16, 12);
		return;
	}
	if(lbl->type == LBL_HZ16) {
		if(reverse & SW_NORMAL) {
			LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 16, lbl->y + 16);
			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 16, 16);
		return;
	}
}


