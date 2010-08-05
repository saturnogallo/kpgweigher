#include "stdafx.h"
#include "TesterCaculate.h"


static const CString g_Lclst("×Ô¶¯ 10mV 100mV 1V 10V 100V ");
static const CString g_Panlst("0.01 0.1 1 10 100 1k 10k 100k 1M 10M");

extern CString g_Path;
double CTesterCaculate::GetAverage(double *data, int count)
{
	double sum,vara;
	int length;
	int index,i;
	if(count <= 1)
		return data[0];
	length = count;
	while(true)
	{
		sum = 0;
		for(i=0;i<count;i++)
		{
			if(data[i] != -1000)
				sum += data[i];
		}
		sum = sum/length;
		vara = 0;
		for(i=0;i<count;i++)
		{
			if(data[i] != -1000)
			{
				if(data[i] > sum)
					vara += (data[i]-sum);
				else
					vara += (sum-data[i]);
			}	
		}
		vara = vara/length;
		
		if((sum > 0)&&(vara < (0.001*sum)))
			return sum;
		if((sum < 0)&&(vara < (-0.001*sum)))
			return sum;
		
		vara = 0;
		index = 0;
		for(int i=0;i<count;i++)
		{
			if(data[i] != -1000)
			{
				if(((data[i]-sum) > vara)||((sum-data[i]) > vara))
				{
					if(data[i] > sum)
						vara = data[i]-sum;
					else
						vara = sum-data[i];
					index = i;
				}
			}
		}
		data[index]=-1000;
		length--;
	}
}


static const char *mealist="YSIN NSIN YRAN NRAN YRLS NRLS YBRN NBRN YVAL NVAL YRST NRST EMPT YSCA NSCA YRES NRES";


//extract datas from the sParam and the sParam will be cut
int CTesterCaculate::RetrieveIndex(LPCSTR search)
{
	char *pdest;
	pdest = strstr(mealist,search);
	if(pdest == NULL)
		return -1;//not found in swilist
	return (int)(pdest-mealist)/(strlen(search)+1);
};
bool CTesterCaculate::ExtractInt(CString &sParam,int &value)
{
	int pos1,pos2;
	pos1 = sParam.Find(',',0);
	pos2 = sParam.Find(']',0);
	if(pos1 == -1) pos1 = pos2;
	if(pos1 != -1)
	{
		value = atoi(sParam.Left(pos1));
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
		return true;
	}
	return false;	
};
bool CTesterCaculate::ExtractDouble(CString &sParam,double &value)
{
	int pos1,pos2;
	pos1 = sParam.Find(',',0);
	pos2 = sParam.Find(']',0);
	if(pos1 == -1) pos1 = pos2;
	if(pos1 != -1)
	{
		value = atof(sParam.Left(pos1));
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
		return true;
	}
	return false;	
};
bool CTesterCaculate::ExtractString(CString &sParam,CString &value)
{
	int pos1,pos2;
	pos1 = sParam.Find(',',0);
	pos2 = sParam.Find(']',0);
	if(pos1 == -1) pos1 = pos2;
	if(pos1 != -1)
	{
		value = sParam.Left(pos1);
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
		return true;
	}
	return false;	
};
bool CTesterCaculate::ExtractChar(CString &sParam,char &value)
{
	int pos1,pos2;
	pos1 = sParam.Find(',',0);
	pos2 = sParam.Find(']',0);
	if(pos1 == -1) pos1 = pos2;
	if(pos1 != -1)
	{
		value = sParam.GetAt(0);
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
		return true;
	}
	return false;	
};


double CTesterCaculate::ConvertDataExp(double input)
{
	int exp;
	double data;
	if(input == 0)
		return 0;
	
	exp = 0;
	while(true)
	{
		if((input >= 10)||(input <= -10))
		{
			input = input/10;
			exp++;
			continue;
		}
		if((input > -1)&&(input < 1))
		{
			input = input*10;
			exp--;
			continue;
		}
		break;
	}
	
	if((input > 5)||(input < -5))     //get the round value
	{
		exp++;
	}
	
	data = (input>0)?1:-1;
	while(true)
	{
		if(exp > 0)
		{
			data = data*10;
			exp--;
			continue;
		}
		if(exp < 0)
		{
			data = data/10;
			exp++;
			continue;
		}
		break;
	}
	return data;
}
void CTesterCaculate::InsertColon(CString &val)
{

	CString value(val);
	if((val.FindOneOf(_T("0123456789")) < 0) || (val.Find('.') > 0))
		return;
	
	if(val.Find("<font color=") >= 0)
	{
		value.Delete(0,value.Find('>',0)+1);
		value.Replace(_T("</font>"),_T(""));
	}
	if(value.GetLength() <= 6)
		return;
	CString newval;
	while(value.GetLength() > 3)
	{
		newval =_T(",")+value.Right(3)+newval;
		value.Delete(value.GetLength() - 3,3);
	}
	val = value + newval;
}

CString CTesterCaculate::DoublePrecision(double value,int prec)
{
	if(value < 0) 
		value -= 1e-14;
	else
		value += 1e-14;
	int exp;
	double tmp;
	CString str;
	exp = 0;
	tmp = value;
	if((value < 1e-10)&&(value > -1e-10))
		return _T("0.0000000");
	if((tmp > 1)||(tmp < -1))
	{
		while((tmp > 1)||(tmp < -1))
		{
			exp++;
			tmp = tmp/10.0;
		}
	}
	else 
		if((tmp < 0.1)&&(tmp > -0.1))
		{
			while((tmp < 0.1)&&(tmp > -0.1))
			{
				exp--;
				tmp = tmp*10.0;
			}
		}		
		str.Format("%f",value);
		exp = prec-exp;
		if(exp <= 0)
			return (value >= 0)?str.Left(prec-exp):str.Left(prec+1-exp);
		if(exp == 1)
		{
			if(prec == 5) 
				str.Format("%.5f",value);
			if(prec == 6) 
				str.Format("%.6f",value);
			if(prec == 7) 
				str.Format("%.7f",value);
			if(prec == 8) 
				str.Format("%.8f",value);
			if(prec == 9) 
				str.Format("%.9f",value);
		}
		if(exp == 1)
			str.Format("%.1f",value);
		if(exp == 2)
			str.Format("%.2f",value);
		if(exp == 3)
			str.Format("%.3f",value);
		if(exp == 4)
			str.Format("%.4f",value);
		if(exp == 5)
			str.Format("%.5f",value);
		if(exp == 6)
			str.Format("%.6f",value);
		if(exp == 7)
			str.Format("%.7f",value);
		if(exp == 8)
			str.Format("%.8f",value);
		if(exp == 9)
			str.Format("%.9f",value);
		if(exp == 10)
			str.Format("%.10f",value);
		if(exp == 11)
			str.Format("%.11f",value);
		if(exp == 12)
			str.Format("%.12f",value);
		if(exp == 13)
			str.Format("%.13f",value);
		
		if(str.GetLength() == (prec+1)) //normal case "1.1234"
			return str;
		if(str.GetAt(0) == '0')    //value less than 1 "0.0001"
			return str;
		return str.Left(prec);    //value with no xiao-shu part "10000"
}
int CTesterCaculate::searchlclst(LPCTSTR strid)
{
	return Searchstring(strid,g_Lclst);
}
int CTesterCaculate::searchpanlst(LPCTSTR strid)
{
	return Searchstring(strid,g_Panlst);
}

int CTesterCaculate::Searchstring(LPCTSTR strid,const CString stable)
{
	int rpos = 0; //result position
	int spos = 0; //search position;
	while(spos >= 0)
	{
		if(spos == stable.Find(strid+CString(" "),spos))
			return rpos;
		spos = stable.Find(' ',spos+1)+1;
		if(spos == 0)
			return 0;
		rpos++;
	}
	return spos;
}
char kbuffer[100];

extern void RetrieveDriverID();

bool CTesterCaculate::CheckDriveID()
{
	CStdioFile csf;
	CString str,str1;
	char data[100];
	int i,j;
	unsigned char count;
	int length;
	
	if(!csf.Open(g_Path+_T("\\Data\\Default\\license.txt"),CFile::modeRead))
		return false;
	if(!csf.ReadString(str))
		return false;

	csf.Close();

	RetrieveDriverID();
	char *kb2;
	kb2 = kbuffer;
	for(j=0;j<99;j++){
		if( ' ' != *kb2)
			break;
		kb2++;
	}

	length = strlen(kb2);
	while(length < 8)
	{
		kb2[length++]='1';
		kb2[length]='\0';
	}
	length = 8;
	for(j=0;j<8;j++)
	{
		count = 0;
		for(i=0;i<length;i++)
		{
			if(kb2[i]&(0x01<<j))
				count++;
		}
		count +=  kb2[j];
		if((count>>4) <= 9)
			data[2*j]='0'+(count>>4);
		else
			data[2*j]='A'+((count>>4)-0x0a);
		
		if((count&0x0f) <= 9)
			data[2*j+1]='0'+(count&0x0f);
		else
			data[2*j+1]='A'+((count&0x0f)-0x0a);
		
	}
	data[16]='\0';
	str1.Format("%s",data);
	return (str.Compare(str1) == 0);
}



bool CTesterCaculate::CheckStable(double data[], int length, double scale)
{
	//check whether the data is stable;
	ASSERT(length > 0);

	double avg=0;
	int i=length;
	while (--i >= 0) {
		avg += data[i];
	}
	avg /= length;

	//if(avg > 0.1) scale*=avg;

	//compare the difference
	while(++i < length){
		if((data[i] >= avg) && ((data[i] - avg)> scale))
			return false;
		if((data[i] <  avg) && ((avg - data[i])> scale))
			return false;
		
	}
	return true;
}

CString CTesterCaculate::retrievelclst(int nIndex)
{
	CString value = g_Lclst;
	while(nIndex-- > 0){
		value = value.Right(value.GetLength()-value.Find(' ',0)-1);
	}
	return value.Left(value.Find(' ',0));
}
CString CTesterCaculate::retrievepanlst(int nIndex)
{
	CString value = g_Panlst;
	while(nIndex-- > 0){
		value = value.Right(value.GetLength()-value.Find(' ',0)-1);
	}
	return value.Left(value.Find(' ',0));
}
