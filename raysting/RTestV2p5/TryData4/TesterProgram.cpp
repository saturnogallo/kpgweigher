#include "stdafx.h"
#include "TesterProgram.h"
#include "resource.h"
#include "InputDlg.h"
extern TESTERCONF g_TesterConf;		//tester configure
extern CString g_Path;				//app path
extern CTesterData g_Data;			//tester result

CTesterProgram::CTesterProgram()
{
	bLogOn = false;
	m_iStackDepth = 0;
}
 /*
 *		here we use following structrue to add functionality
 *		m_fun(0)="funname1@funpos1,funname2@funpos2..."
 *		m_fun(1-FUN_MAX)="parameterstack(1-FUN_MAX)"
 */
bool CTesterProgram::InstallScheme(CString schemename)
{
//	if(m_iStackDepth > 0)		return false;
	CStdioFile csf;

	if(0 == csf.Open(g_Path+"\\Data\\"+schemename+".prg",CFile::modeRead))
		return false;
	csf.SeekToBegin();
	 /*
	 *	Reset the component
	 */
	if(g_TesterConf.m_iRetestID == -1)
		g_Data.ResetRecord();

	m_fun.RemoveAll();
	for(int i=0;i<(FUN_MAX+1);i++)	//add function declaration and function stack
	{
		m_fun.Add(_T(""));
	}
	
	g_TesterConf.m_iRetestID = -1;
	m_iStackDepth = 0;
	
	CString line;
	while(csf.ReadString(line))
	{
		if(line.GetAt(0) == '<')//function declaration
		{
			CString str1;
			int pos=line.Find('>',0);
			str1.Format("%d",m_fun.GetSize());
			m_fun.SetAt(0,m_fun.GetAt(0)+line.Mid(1,pos-1)+'@'+str1+',');
			while(csf.ReadString(line))
			{
				if(line.GetLength() < 1)
					continue;
				if(line.GetAt(0) == '>')
					break;
				m_fun.Add(line);
			}
			m_fun.Add(_T("><"));
		}
	}

	csf.Close();

	if(bLogOn)
		logout.Close();

	bLogOn = (0 != logout.Open(g_Path+"\\Data\\record.txt",CFile::modeCreate|CFile::modeWrite));

	
	return true;

}
void CTesterProgram::FunctionInternal(CString funname)
{
		if(SetFunctionPointer(funname))
		{
			char type;
			CString cmd;
			while(m_iStackDepth > 0)
				NextLine(type,cmd);
		}

}
bool CTesterProgram::NextLine(char &type, CString &cmd)
{
	if(m_iStackDepth <= 0) //not running in any function
		return false;

	CString line;
	CString para;
	
	//stay in some function ,retrieve the parameter first
	para = m_fun.GetAt(m_iStackDepth);
	line = m_fun.GetAt(m_iFunpos[m_iStackDepth]);
	m_iFunpos[m_iStackDepth]+=1;
	
	//replace the ^i^ with the corresponding parameter
	if(line.Find('^',0) > 0)
	{
		int count = 1;
		while(1)
		{
			CString str2;
			int pos;
			pos=para.Find(',',0);
			if(pos < 0)
				break;
			str2.Format("^%d^",count++);
			line.Replace(str2,para.Left(pos));
			para = para.Right(para.GetLength()-pos-1);
		}
	}
	
	//skip empty lines
	if(line.GetLength() < 1)
		line = _T("//");

	if(line.GetAt(0) == '{')//function calls
	{
		while(FunEnter(line));
	}
	else
	{
		if(line.GetAt(0) == '>')
		{
			m_iStackDepth--;
		}
	}

	if(CheckCondition(line,type))
	{
		if(!ProgramFunction(type,line))
		{
			cmd = line;
			return true;
		}
	}
	return false;
}
bool CTesterProgram::SetFunctionPointer(CString funname)
{
	if(m_iStackDepth > 0)
		return false;
	for(int i=0;i<(FUN_MAX+1);i++)
		m_iFunpos[i] = 0;

	CString funlist=m_fun.GetAt(0);
	int pos = funlist.Find(CString(",")+funname);
	if(pos < 0)
		 pos = funlist.Find(funname);

	pos = funlist.Find('@',pos);
	if(pos < 0) return false;
	m_iStackDepth = 1;
	CString str = funlist.Mid(pos+1,funlist.Find(',',pos)-(pos+1));
	m_iFunpos[1] = atoi(str);
	return true;
}
bool CTesterProgram::SetFunctionParameter(CString parm)
{
	if(m_iStackDepth <= 0)
		return false;
	m_fun.SetAt(1,parm);
	return true;
}
/*
 *	FunEnter is called when entering a function
 */

bool CTesterProgram::FunEnter(CString &cmdLine)
{
	//push the function
	int pos1,pos2,count,pos;
	CString str,str1,line,str2;
	
	pos1=cmdLine.Find('(',0);
	pos2=cmdLine.Find(')',0);
	
	str=cmdLine.Mid(pos1+1,pos2-pos1-1)+',';
	m_fun.SetAt(++m_iStackDepth,str);//add parameter
	
	str=cmdLine.Mid(1,pos1-1);
	str1=m_fun.GetAt(0);
	pos1=str1.Find(str,0);
	pos2=str1.Find('@',pos1);
	pos1=str1.Find(',',pos2);
	m_iFunpos[m_iStackDepth]=atoi(str1.Mid(pos2+1,pos1-pos2-1));
	
	str = m_fun.GetAt(m_iStackDepth);
	line = m_fun.GetAt(m_iFunpos[m_iStackDepth]);
	m_iFunpos[m_iStackDepth] += 1;

	if(line.Find('^',0) > 0)
	{
		count = 1;
		while(1)
		{
			pos=str.Find(',',0);
			if(pos < 0)
				break;
			str1 = str.Left(pos);
			str = str.Right(str.GetLength()-pos-1);
			str2.Format("^%d^",count++);
			line.Replace(str2,str1);
		}
	}
	cmdLine = line;
	return (line.GetAt(0) == '{');
}

/**
 *      the CheckCondition funtion is used to 
 *		check whether the condition is fit
 *  	format :[type,if,ifpos,ifvalue,parm]
 */
bool CTesterProgram::CheckCondition(CString &line,char &type)
{
	int	ipos,vpos;
	double  value;
	bool	bfit=false;
	CString Istr;
	RESDATA	rd;
	PARAMTYPE ps=g_TesterConf.m_Ps;
	RSTDTYPE rs=g_TesterConf.m_Rs;

	if(line.IsEmpty())		return true;

	if(line.GetAt(0) != '[')   //illegal program line , just notation
		return false;
	line.TrimLeft('[');
	CTesterCaculate::ExtractChar(line,type);

	if((type >= 'A')&&(type <= 'Z'))
		return false;
	CTesterCaculate::ExtractString(line,Istr);

	ipos = CTesterCaculate::RetrieveIndex(Istr)+1;

	CTesterCaculate::ExtractInt(line,vpos);
	CTesterCaculate::ExtractDouble(line,value);
	
	switch(ipos)
	{
	case 1:		//"YSIN"
		bfit = (ps.bSingle == TRUE);	break;
	case 2:		//"NSIN"
		bfit = (ps.bSingle == FALSE);	break;
	case 3:		//"YRAN"
		bfit = (ps.iRange == vpos);		break;
	case 4:		//"NRAN"
		bfit = (ps.iRange != vpos);		break;
	case 5:		//"YRLS"
		bfit = (ps.iLowest == vpos);	break;
	case 6:		//"NRLS"
		bfit = (ps.iLowest != vpos);	break;
	case 7:		//"YBRN"
		bfit = (rs.bUse[vpos] == TRUE);	break;
	case 8:		//"NBRN"
		bfit = (rs.bUse[vpos] == FALSE);break;
	case 9:		//"YVAL"
		rd = g_Data.GetRecord(vpos+g_TesterConf.m_iBase);
		bfit = rd.rbValue;				break;
	case 10:	//"NVAL"
		rd = g_Data.GetRecord(vpos+g_TesterConf.m_iBase);
		bfit = !rd.rbValue;				break;
	case 11:	//"YRST"
		rd = g_Data.GetRecord(vpos+g_TesterConf.m_iBase);
		bfit = rd.rbResult;				break;
	case 12:	//"NRST"
		rd = g_Data.GetRecord(vpos+g_TesterConf.m_iBase);
		bfit = !rd.rbResult;			break;
	case 13:    //"EMPT"
		bfit = true;					break;
	case 14:	//"YSCA"
		bfit = (g_TesterConf.m_Rs.iScaledR0 == vpos);	break;
	case 15:	//"NSCA"
		bfit = (g_TesterConf.m_Rs.iScaledR0 != vpos);	break;
	case 16:	//"YRES"
		bfit = (g_TesterConf.m_iRetestID != -1);		break;
	case 17:	//"NRES"
		bfit = (g_TesterConf.m_iRetestID == -1);		break;
	default:
		break;	
	}
	return bfit;
}

bool CTesterProgram::ProgramFunction(char type,CString Param)
{
	CString str,str1;
	int min,max,left,right,count;
	char oper,babs,vzl,vzr;
	CInputDlg dlg;
	switch(type)
	{
	case 'c'://[c,if,if_value,LEFT,lpos,OP,RIGHT,rpos,abs,ID];caculate(LEFT OP RIGHT and store to ID)
		CTesterCaculate::ExtractChar(Param,vzl);
		CTesterCaculate::ExtractInt(Param,left);
		CTesterCaculate::ExtractChar(Param,oper);
		CTesterCaculate::ExtractChar(Param,vzr);
		CTesterCaculate::ExtractInt(Param,right);
		CTesterCaculate::ExtractChar(Param,babs);
		CTesterCaculate::ExtractInt(Param,count);
		if(((vzl == 'Z')||(vzl == 'V')) && (left > 0))
			left += g_TesterConf.m_iBase;
		
		if(((vzr == 'Z')||(vzr == 'V')) && (right > 0))
			right += g_TesterConf.m_iBase;
		
		if(count >= 0)	
			count += g_TesterConf.m_iBase;
		
		g_Data.OperateValue(vzl,vzr,oper,abs(left),abs(right),babs,abs(count));
		return true;		
	
	case 'r'://[r,if,if_value,min,max,MARK];
		//jump to the MARK if retest value is out vb &ve
		if(g_TesterConf.m_iRetestID == -1)
			return true;
		CTesterCaculate::ExtractInt(Param,min);
		CTesterCaculate::ExtractInt(Param,max);
		
		if((g_TesterConf.m_iRetestID >= min)&&(g_TesterConf.m_iRetestID <= max))
			return true;
		
	case 'j':	//jump to mark lines [j,MARK];如果条件满足，跳到MARK行
		CTesterCaculate::ExtractString(Param,str1);
		if(str1.GetAt(0) == '-')
		{
			str1.Delete(0,1);
			str1 = '['+str1+']';
			while(m_iFunpos[m_iStackDepth] > 0)
			{
				str = m_fun.GetAt(--m_iFunpos[m_iStackDepth]);
				if(str.Compare(str1) == 0)
					return true;
			}
		}else{
			str1 = '['+str1+']';
			while(true)
			{
				if(m_iFunpos[m_iStackDepth] >= m_fun.GetSize())
					break;
				str = m_fun.GetAt(m_iFunpos[m_iStackDepth]++);
				if(str.Compare(str1) == 0)
					return true;
			}
		}
		return true;
	case 'a'://[a,msg,MARK];ask the message and if no,jump to the mark position
		CTesterCaculate::ExtractString(Param,str);
		str1 = str;
		if (AfxMessageBox(str,MB_YESNO) == IDYES) 
		{
			return true;
		}
		return	ProgramFunction('j',Param);
	case 'v'://[v,if,if_value,TYPE,value];set the type variable to the value
		CTesterCaculate::ExtractChar(Param,oper);
		CTesterCaculate::ExtractInt(Param,min);
		switch(oper)
		{
		case 'B':
			g_TesterConf.m_iBase = min;
			if(g_TesterConf.m_iBase != 0) //retest case
				g_Data.ResetRecord(g_TesterConf.m_iBase);
			break;
		case 'V':
			g_TesterConf.m_RangeV = min;
			break;
		case 'T':
			g_TesterConf.m_iDelayKT = min;
			break;
		case 'I':
			g_TesterConf.m_iDelayKI = min;
		default:
			break;
		}
		return true;
	default:
			break;
	}
	return false;
}
