#include "utili.h"


/*---------------延时子程序----------------*/
void delay (uint us) 
{
  	while(us--);
}
void delay1 (uint ms) 
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;
}
//计算字符串长度
uchar strlen(uchar *s)
{
	uchar len = 0;
	while(*s++) len ++;
	
	return len;
}
double buf2double()		//convert rdata.tempbuf to double
{
	double tmp = 0.0;
	uchar i = 0;
	uchar pos = 0;
	for(i=0;i<rdata.pos_len;i++)
	{
		if(rdata.tempbuf[i] != KEY_DOT)
			tmp = tmp * 10.0+(rdata.tempbuf[i] - '0');
		else
			pos = rdata.pos_len - i - 1;
	}
	while(pos > 0)
	{
		tmp = tmp / 10.0;
		pos--;
	}
	return tmp;
}
int buf2byte()	    //convert rdata.tempbuf to byte (00-99)
{
	int tmp = 0;
	uchar i;
	for(i=0;i<rdata.pos_len;i++)
	{
		tmp = tmp * 10+(rdata.tempbuf[i] - '0');
	}
	return tmp;
}

