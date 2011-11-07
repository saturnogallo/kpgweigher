#ifndef	_WINDOW_H_
#define _WINDOW_H_
#include "typedef.h"
#include "global.h"
typedef void (*MSG_HANDLER)(uchar key);
#define SW_NORMAL	1
#define SW_REVERSE	2
#define SW_OVERLAP	4

#define WINTYPE_LIST	1
#define WINTYPE_INPUT	2
#define WINTYPE_MSG	3
#define WINTYPE_USER	4
#define MSG_INIT	0xff
#define MSG_REFRESH	0xfe


typedef flash struct typWINDOW
{
	uchar page_id;	      //id of the page
	MSG_HANDLER handler;  //message handler of current window, a static variable can be declared in the function
}WINDOW;											

#define LBL_HZ6X8	1
#define LBL_HZ8X16	2
#define LBL_HZ24X32	3
#define LBL_HZ12	4
#define LBL_HZ16	5

typedef flash struct  typLABEL
{
	u8 id_type;	//id of the label (high byte) and type of the label, font type (low byte)
	int   x;
	int   y; 
	int   width;    //width in characters
	char  *param;
}LABEL;

void draw_label(flash LABEL *lbl, uchar reverse);
void draw_inputbox(flash LABEL *lbl);
void wnd_msgbox(flash LABEL *lbl); //display a message box
double wnd_floatinput(double oldval); //input a float
u8 wnd_listbox(flash LABEL *title, uchar max_index,uchar max_option,uchar curr_pos);//two column list box type
u8 wnd_uintinput(u8 oldval); //input a unsigned int
u8* wnd_strinput(); //input a string
uchar wnd_menu(flash LABEL *title,flash LABEL *labels,uchar max_option,uchar page_option); //select value from a menu
extern MSG_HANDLER curr_window;
extern MSG_HANDLER lp_ownerdraw;    
extern MSG_HANDLER lp_listvalue;
extern uchar max_databuf;
#define window_setup(A)	max_databuf = A
#endif
