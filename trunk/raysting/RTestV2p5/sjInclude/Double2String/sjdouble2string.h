/*
 *	File Name : SJDouble2String.h
 *	Author	  : sojo at sunext
 *	Details	  : omnifarious double-to-string conversions
 *	Notes	  : 数字的位下标为 ...3210.-1-2-3..
 *			Date		Description		
 *	History   : 
 * 			2004.11.4	created ,used in trydata3(RtestV1.4)
 */
 
#ifndef _SJ_DOUBLE2STRING_H_
#define _SJ_DOUBLE2STRING_H_
#include "stdafx.h"
#include "math.h"

void SJTryRound(double &value)
{
	if(value < 0) 
		value -= 1e-14;
	else
		value += 1e-14;
}
/*
 *	得到posn位置的字符
 */
char SJD2SGetchar(double value,int posn)
{
	SJTryRound(value);
	return (char)('0'+(int(floor(value*pow(10.0,-posn)))-10*int(floor(floor(value*pow(10.0,-posn))/10.0))));
}

/*
 * 打印数字，直到lastpos位置
 *
 * 若第一位有效数字在lastpos后，则全为0
 * lastpos必须小于0,否则打印所有整数部分
 * 注：可考虑用printf("% .*f",-lastpos,value)实现
*/
CString SJD2SByPosition(double value,int lastpos)
{
	SJTryRound(value);
	bool vsign = (value >= 0);
	value = vsign?value: -value;
	CString sv;
	
	sv.Format("%d",int(floor(value)));
	
	if (lastpos < 0)
		sv += '.';
			
	while(lastpos++ < 0)
	{
		value = (value-int(value))*10.0;
		sv +=  char(int(value)+'0');
	}
	return vsign?sv:("-"+sv);
}
/*
 *	打印数字，直到满n位显示数字
 *
 *  	若整数部分位数大于n则打印所有的整数部分
 */
CString SJD2SByLength(double value,int n)
{
	SJTryRound(value);
	CString sv;
	sv.Format("% d",int(value));
	n -= (sv.GetLength()-1);
	
	if (n > 0)
		sv = SJD2SByPosition(value,-n);
	return sv;
}
/*
 *	得到第1个非0数字后的n位的数字（非第n个有效数字)
 */
char SJD2SGetcharV(double value,int n)
{
	SJTryRound(value);
	if (n <= 0) return '0';
	if (value < 0) value = -value;
	
	if(value < pow(10,-n))
		return '0';

	while(value >= 1.0)
		value /= 10.0;
	while(value < 0.1)
		value *= 10.0;

	return SJD2SGetchar(value,-n);
}


/*
 *	由参考字符ochar(omittable char)的值和
 *	最后一个有用位上字符vchar(valid char)的值得到进位值（0，1）
 *
 *	sel为选择末尾值的最大公因数（1，2，5）,
 * 	计算使用了4舍5入+偶数法则
 */

int SJD2SGetcarryI(int vchar,int ochar,int sel)
{
	int tmp = ((ochar > 5) || ((ochar == 5)&&((vchar%2) == 1)))?1:0; //4舍5入+偶数法则
	
	if (sel == 5)
	{	//5 case
		return (vchar+tmp) - ((vchar+tmp)%5) - vchar;
	}
	if (sel == 1)
	{	//1 case
		return (ochar >= 5)? 1:0;//4舍5入
	}
	if (sel == 2)
	{	//2 case
		tmp = vchar+tmp;
		if (tmp%2 == 0)	return tmp-vchar;
		
		switch(tmp) {
		case 1:
			return 2-vchar;
		case 3:
		case 5:
			return 4-vchar;
		case 7:
		case 9:
			return 8-vchar;
		default:
			return tmp-vchar;
		}
	}
	return 0;	
}
int SJD2SGetcarryC(char vchar,char ochar,int sel)
{
	return SJD2SGetcarryI(vchar-'0',ochar-'0',sel);
}
#endif
