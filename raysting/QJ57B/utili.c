#include "utili.h"
#include "window.h"  
#include "stdlib.h"
/*---------------延时子程序----------------*/
/*
void delay1 (uint ms) 
{
  	int i,j;
  	for(i=0;i<ms;i++)
  	for(j=0;j<1000;j++)
  		;
		
}
*/
/*
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
*/
/*计算字符串长度*/

extern uchar pos_databuf;     
extern uchar data_sign;
extern u8 databuf[12];
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
int buf2byte()	    //convert rundata.tempbuf to byte (00-99)
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


