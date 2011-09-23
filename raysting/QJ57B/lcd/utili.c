#include "utili.h"
#include "stc51.h"
#include "window.h"

/*---------------延时子程序----------------*/
void delay (uint us) 
{
  	while(us--);
}
/*
void delay1 (uint ms) 
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;
		
}
*/
char lowc(uchar x)
{
	x = x&0x0f;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}
char highc(uchar x)
{

	x = x>>4;
	if(x > 0x09)
		return 'A'+x-0x0a;
	else
		return '0'+x;
}

/*计算字符串长度*/
uchar strlen(uchar *s)
{
	uchar len = 0;
	while(*s++) len ++;
	
	return len;
}

double buf2double()		
{
	double tmp = 0.0;
	uchar i = 0;
	uchar pos = 0;
	for(i=0;i<pos_databuf;i++) {
		if(databuf[i] != KEY_DOT)
			tmp = tmp * 10.0+(databuf[i] - '0');
		else
			pos = pos_databuf - i - 1;
	}
	while(pos > 0) {
		tmp = tmp / 10.0;
		pos--;
	}
	return tmp;
}

int buf2byte()	    //convert rdata.tempbuf to byte (00-99)
{
	int tmp = 0;
	uchar i;
	for(i=0;i<pos_databuf;i++) {
		tmp = tmp * 10+(databuf[i] - '0');
	}
	return tmp;
}


/*********************************************************************/
/*                                                                   */
/*   Copyright Agilent Technology  1996-2000                         */
/*   all rights reserved                                             */
/*                                                                   */
/*********************************************************************/

#include	"stdarg.h"

#define   fSP  0x01
#define   fPL  0x02
#define   fMI  0x04
#define   fNO  0x08
#define   fZE  0x10
#define   WMAX   99

