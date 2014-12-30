#ifndef	_WINDOW_H_
#define _WINDOW_H_
#include "typedef.h"

//typedef void (*MSG_HANDLER)(uchar key);
#define SW_NORMAL	1
#define SW_REVERSE	2
#define SW_OVERLAP	4

#define WINTYPE_LIST	1
#define WINTYPE_INPUT	2
#define WINTYPE_MSG	3
#define WINTYPE_USER	4
#define MSG_INIT	0xff
#define MSG_REFRESH	0xfe

/*
typedef code struct typWINDOW
{
	uchar page_id;	      //id of the page
	MSG_HANDLER handler;  //message handler of current window, a static variable can be declared in the function
}WINDOW;											
*/
#define LBL_HZ6X8	1
#define LBL_HZ8X16	2
#define LBL_HZ24X32	3
#define LBL_HZ12	4
#define LBL_HZ16	5
typedef code struct typCHECKBOX
{
	uint type;	//type of the label 
	int   x;
	int   y; 
	char  *param;
}CHECKBOX;
typedef code struct  typLABEL
{
	uint type;	//type of the label 
	int   x;
	int   y; 
	int   width; //width in characters
	char  *param;
}LABEL;


void draw_label(LABEL *lbl, uchar reverse);
void wnd_msgbox(LABEL *lbl);
void wnd_msgbox2(LABEL *lbl);
void draw_inputbox(LABEL *lbl,BOOL redraw);
double wnd_floatinput(double oldval);
uchar wnd_intinput(uchar oldval);

//extern MSG_HANDLER curr_window;
//extern MSG_HANDLER caller;
extern uchar max_databuf;
#define window_setup(A)	max_databuf = A
#endif
