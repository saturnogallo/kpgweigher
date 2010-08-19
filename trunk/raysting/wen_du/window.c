#include "window.h"
#include "utili.h"
#include "lcd.h"

MSG_HANDLER curr_window = 0;
extern uchar key;

char  data databuf[12];
uchar pos_databuf;
uchar max_databuf;
const LABEL code datalbl = {LBL_HZ16,30,10,8,strbuf};
const LABEL code databox = {LBL_HZ16,30,30,8,databuf};

void mystrcpy(char* dst, char* src, uchar len)
{
	while(len-- != 0)
	{
		*dst++ = *src++;
	}

}

void wnd_sninput(char* oldbuf)
{
	uchar msg;
	key = MSG_INIT;
	while(1)
	{
		if(key != KEY_INVALID)
		{
			msg = key;
			key = KEY_INVALID;
		}else{
			continue;
		}
		if(msg == MSG_INIT) {
			LCD_Cls();
			databuf[0] = '\0';
			pos_databuf = 0;
			draw_label(&datalbl,SW_NORMAL);
			wnd_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}
		if(msg == KEY_TAB)
		{
			databuf[pos_databuf++] = '0';
			databuf[pos_databuf] = '\0';
			msg = MSG_REFRESH;
		}
		if(msg == KEY_CE) {	
			break;
		}
		if(pos_databuf >= max_databuf)
			continue;

		if(msg == KEY_DN) {
			if(pos_databuf == 0)
			{
				databuf[pos_databuf++] = '0';
				databuf[pos_databuf] = '\0';
			}

			if(databuf[pos_databuf-1] == '0')
				databuf[pos_databuf-1] = 'Z';
			else if (databuf[pos_databuf] == 'A')
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
			databuf[pos_databuf++] = msg;
			databuf[pos_databuf] = '\0';
			msg = MSG_REFRESH;
		}
		if(msg == KEY_OK) {
			mystrcpy(oldbuf,databuf,pos_databuf);
			oldbuf[pos_databuf] = '\0';
			break;
		}

		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}
	}
//	LCD_HideCursor();
}

uchar wnd_intinput(uchar lastval)
{
	uchar msg;
	
	key = MSG_INIT;
	while(1)
	{
		if(key != KEY_INVALID)
		{
			msg = key;
			key = KEY_INVALID;
		}else{
			continue;
		}
		if(msg == MSG_INIT)
		{
			LCD_Cls();
			databuf[0] = '0';
			databuf[1] = '\0';
			pos_databuf = 0;
			draw_label(&datalbl,SW_NORMAL);
			wnd_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}

		if(msg == KEY_TAB)
		{
			databuf[pos_databuf++] = '0';
			databuf[pos_databuf] = '\0';
			msg = MSG_REFRESH;
		}
		if(pos_databuf >= max_databuf)
			continue;
		if(msg == KEY_DN) {
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
		}
		if(msg == KEY_UP) {
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
		}
		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
			databuf[pos_databuf++] = msg;
			databuf[pos_databuf] = '\0';
			msg = MSG_REFRESH;
		}
		if(msg == KEY_CE) {
//			LCD_HideCursor();
			return lastval;
		}
		if(msg == KEY_OK){
//			LCD_HideCursor();
			return buf2byte();
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}
	}
}
double wnd_floatinput(double lastval)
{
	uchar msg;
	
	key = MSG_INIT;
	while(1)
	{
		if(key != KEY_INVALID)
		{
			msg = key;
			key = KEY_INVALID;
		}else{
			continue;
		}
		if(msg == MSG_INIT)
		{
			LCD_Cls();
			databuf[0] = '0';
			databuf[1] = '\0';
			pos_databuf = 0;
			draw_label(&datalbl,SW_NORMAL);
			wnd_inputbox(&databox);
//			LCD_ShowCursor(databox.x,databox.y);
		}

		if(msg == KEY_TAB)
		{
			databuf[pos_databuf++] = '0';
			databuf[pos_databuf] = '\0';
			msg = MSG_REFRESH;
		}
		if(pos_databuf >= max_databuf)
			continue;
		if(msg == KEY_DN) {
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
		}
		if(msg == KEY_UP) {
			if(databuf[pos_databuf-1] == '9')
				databuf[pos_databuf-1] = '0';
			else
				databuf[pos_databuf-1] += 1;
			msg = MSG_REFRESH;
		}
		if((msg >= KEY_NUM0 && msg <= KEY_NUM9) || msg == KEY_DOT) {
			databuf[pos_databuf++] = msg;
			databuf[pos_databuf] = '\0';
			msg = MSG_REFRESH;
		}
		if(msg == KEY_CE) {
//			LCD_HideCursor();
			return lastval;
		}
		if(msg == KEY_OK){
//			LCD_HideCursor();
			return buf2double();
		}
		if(msg == MSG_REFRESH) {
			draw_label(&databox,SW_NORMAL);
//			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
		}
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

void wnd_inputbox(LABEL *lbl)
{
	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	draw_label(lbl, SW_NORMAL);	
}

void draw_label(LABEL *lbl,uchar reverse) {
	uchar len = lbl->width;
	if(lbl->type == LBL_HZ6X8) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 8, lbl->y + 8);
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
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 8, lbl->y + 16);
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
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 16, lbl->y + 12);
			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
		return;
	}
	if(lbl->type == LBL_HZ16) {
		if(reverse & SW_NORMAL) {
			if((reverse & SW_OVERLAP) == 0)
				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 16, lbl->y + 16);
			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
		}
		if(reverse & SW_REVERSE)
			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
		return;
	}
}


