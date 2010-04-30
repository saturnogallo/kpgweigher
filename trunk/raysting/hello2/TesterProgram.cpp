#include "stdafx.h"
#include "TesterProgram.h"
#include "resource.h"
#include "afx.h"
#include "TCHAR.h"
#include "math.h"
#include "stdio.h"

CSwiNav CTesterProgram::m_swinav;
bool CTesterProgram::swifree = true;
CMap<long,long,CString,CString> CTesterProgram::env_var;
extern CString SjGetAppDirectory();

#define WAITSWI 	www()
#define RETURNSWI	swifree = true;
static void www()
{
	int j = 0;
	while(!CTesterProgram::swifree){ 
		if(j++ > 1000)
		{
			TRACE(_T("!")); 
			j=0;
		}
	} 
	CTesterProgram::swifree = false;
}
char szTarget[256];

static void wchar2szTarget(CString str)
{
	sprintf(szTarget,"");
 	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    memset(szTarget, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0,str, -1, szTarget, len, NULL,NULL);

}
//hash table key generator
static long string2long(const TCHAR* str,int size)
{
	int len = size;
	long x;
	x = *str << 7;
	while(--len >= 0){
		x = (100003*x)^*str++;
	}
	x^=size;
	return x;
}
extern CString SjGetAppDirectory();
 /*
 *		here we use following structrue to add functionality
 *		m_fun(0)="funname1@funpos1,funname2@funpos2..."
 *		m_fun(1-FUN_MAX)="parameterstack(1-FUN_MAX)"
 */
CString ExtractPair(CString line){
	if((line.GetAt(0) == '[') && (line.Find(']',0) > 0))//function declaration
		return line.Mid(1,line.Find(']',0)-1);
	if((line.GetAt(0) == '<') && (line.Find('>',0) > 0))//function declaration
		return line.Mid(1,line.Find('>',0)-1);
	if((line.GetAt(0) == '{') && (line.Find('}',0) > 0))//function call
		return line.Mid(1,line.Find('}',0)-1);
	if((line.Find('(') > 0) && (line.Find(')',0) > 0))//function declaration
		return line.Mid(line.Find('('),line.Find(')',0)-line.Find('(',0)-1);
	return _T("");
}
CString ExtractString2(CString &sParam,CString sp)
{
	int pos1;
	CString value;
	pos1 = sParam.Find(sp,0);
	if(pos1 != -1)
	{
		value = sParam.Left(pos1);
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
	}else{
		value = sParam;
		sParam.Empty();
	}
	return value;	
};

CString ExtractString(CString &sParam,char sp)
{
	int pos1;
	CString value;
	pos1 = sParam.Find(sp,0);
	if(pos1 != -1)
	{
		value = sParam.Left(pos1);
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
	}else{
		value = sParam;
		sParam.Empty();
	}
	return value;	
};
CTesterProgram::CTesterProgram() : pause_cnt(0)
{
	FILE *csf;
	CString fname = SjGetAppDirectory()+_T("\\all.prg");
	wchar2szTarget(fname);
	csf = fopen(szTarget,"r");


	if(csf == NULL)
	{
		AfxMessageBox(_T("ERROR 102: 加载程序失败"));
		return;
	}

	fseek(csf,0,SEEK_SET);
	/*
	 *	Reset the environment
	 */
	prg_var.RemoveAll();
	CString line;
	CString funbody;
	CString funname;

	TCHAR lbuf[300];
	while(fgetws(lbuf,300,csf) != NULL)
	{
		line = CString(lbuf);
		if((line.GetAt(0) == '<') && (line.Find('>',0) > 0))//function declaration
		{
			line = ExtractPair(line);
			funname = ExtractString(line,'(');

			funbody.Empty();
			while(fgetws(lbuf,300,csf) != NULL)
			{
				line = CString(lbuf);
				if(line.Find(_T("><")) == 0)
					break;
				if(line.GetLength() < 1)
					continue;
				if((line.GetAt(0) == '{') && (line.Find('}',0) > 0))//function call
				{
					line = ExtractPair(line);
					CString callname = ExtractString(line,'(');
					CString callparm = ExtractString(line,')');
					CString callbody;
					prg_var.Lookup(string2long((LPCTSTR)callname,callname.GetLength()),callbody);

					CString parm;
					char id = '1';
					while(1){
						parm = ExtractString(callparm,',');
						if(parm.IsEmpty())
							break;
						parm.Replace('^','`');
						callbody.Replace(_T("^")+CString(id)+_T("^"),parm);
						id = id+1;
					}
					callbody.Replace('`','^');
					line = callbody;
				}
				funbody = funbody + line;// + "\n";
			}
			prg_var.SetAt(string2long((LPCTSTR)funname,funname.GetLength()),funbody);
		}
	}
	fclose(csf);
}

void CTesterProgram::InstallScheme(CString schemename)
{
	WAITSWI;
	if(!m_swinav.open()){
		AfxMessageBox(_T("ERROR 103:open serial failed"));
	}
	RETURNSWI;
	prg_var.Lookup(string2long((LPCTSTR)schemename,schemename.GetLength()),m_fun);	
}
CString CTesterProgram::GetEnv(CString key)
{
	CString ret;
	prg_var.Lookup(string2long((LPCTSTR)key,key.GetLength()),ret);	
	return ret;
}
CString CTesterProgram::GetGlobalEnv(CString key)
{
	CString ret;
	env_var.Lookup(string2long((LPCTSTR)key,key.GetLength()),ret);	
	return ret;
}

bool CTesterProgram::StepProgram()
{
	static bool lock = false;
	if(lock)
		return false;
	lock = true;
	mystate = _T("");
	do{
		CString line = ExtractString2(m_fun,_T("\n"));
		if((line.GetLength() > 1) && (line[0] == '[') && (line.Find(',') > 0) )
		{
			
			line = ExtractPair(line);
			if(!line.IsEmpty()){
				ApplyEnv(line);
				if(!line.IsEmpty()){
					TCHAR type = ExtractString(line,',').GetAt(0);
					if(ExtractString(line,',') == ExtractString(line,',')){
						
						if(!ProgramFunction(type, line)){
							if(pause_cnt > 0){
								mystate.Format(_T("等待 %i"),pause_cnt);
							}else{
								mystate = _T("");
							}
							
							break;
						}else{
//							mystate = _T("NEXT");							
						}
						
					}
				}
			}

		}
	}while((pause_cnt == 0) && (!IsDone()));
	lock = false;
	return !IsDone();
}
bool CTesterProgram::DuringPause()
 { 
		if(pause_cnt > 0){ 
			mystate.Format(_T("正在等待%i秒"),--pause_cnt);
		}
		return (pause_cnt > 0);
};
void CTesterProgram::Abort()
{
	pause_cnt = 0;
	m_fun.Empty(); 
}
bool CTesterProgram::ProgramFunction(TCHAR type,CString Param)
{
	CString org = Param;
	CString str,str1,str2;
	int len;
	
	SWI_VAR sn;
	double v;
	int mtry;
	
	switch(type)
	{
	case 'v'://[v,target,value];
		str =  ExtractString(Param,',');
		str1 = ExtractString(Param,',');
		prg_var.SetAt(string2long((LPCTSTR)str,str.GetLength()),str1);
		return true;

	case 'c'://[c,target,expression];
		str2 = ExtractString(Param,',');
		str = ExtractString(Param,',');
		str1 = ExtractString(Param,',');
		TRACE(str1+_T("\n"));
		if(str2 == "f")
			str1.Format(_T("%.8f"),CacuExpr(str1));
		if(str2 == "i")
			str1.Format(_T("%i"),int(CacuExpr(str1)));
				TRACE(str+_T("=")+str1+_T("\n"));
		prg_var.SetAt(string2long((LPCTSTR)str,str.GetLength()),str1);		
		return true;
	case 'j':	//jump to mark lines [j,MARK];如果条件满足，跳到MARK行
		str1 = CString('[')+ExtractString(Param,',')+CString(']');
		while(str1 != ExtractString2(m_fun,_T("\n")))	; //search the line
		return true;
	case 'i':	//push the next lines into the stack; [i,line_id]
		str = ExtractString(Param,',');
		prg_var.SetAt(string2long((LPCTSTR)str,str.GetLength()),m_fun.Left(m_fun.Find(CString("[o,0,0,")+str+CString("]"))+8+str.GetLength()));
		return true;
	case 'o':	//push the next lines into the stack; [o,line_id]
		m_fun = GetEnv(ExtractString(Param,','))+_T("\n")+m_fun;
		return true;
	case 'a'://[a,msg,MARK];ask the message and if no,jump to the mark position
		str   = ExtractString(Param,',');
		str1  = ExtractString(Param,',');
		if (AfxMessageBox(str,MB_YESNO) == IDYES) 
			return true;
				
		while(str1 != ExtractString2(m_fun,_T("\n")))	; //search the line
					
		return	true;
	case 'm'://[m,msg];ask the message and if no,jump to the mark position
		str   = ExtractString(Param,',');
		AfxMessageBox(str,MB_OK);
		return	true;
	case 'r': //reset navameter
		TRACE(_T("reset\n"));
		WAITSWI;
		m_swinav.reset_swi();
		mystate = _T("正在复位");
		Sleep(2000);
		RETURNSWI;
		return false;
	case 'n': //[n,command]
		lastnav = Param;
		wchar2szTarget(Param);
		WAITSWI;
		m_swinav.turn_nav(szTarget);
		RETURNSWI;
		m_swinav.reading = -2000;
		return false;
	case 's': //[s,command]
		TRACE(Param+_T("\n"));
		wchar2szTarget(Param);
		WAITSWI;
		
		mystate = _T("开关切换");
		if(!Param.IsEmpty())
		{
			m_swinav.turn_swi(szTarget);
		}
		mystate = _T("退出切换");
		RETURNSWI;
		
		return false;
	case 't': //[t,target]
		mystate = _T("正在读数");
		mtry = 32;
		v = -9999;
		WAITSWI;
		Sleep(300);
		while(mtry-- >= 0)
		{
			if(m_swinav.read_nav(v)){
				break;
			}

			v = -9999;
			m_swinav.parse_buffer();
			if((mtry % 3) == 0){
				TRACE(_T("Hout"));
				wchar2szTarget(lastnav);
				m_swinav.turn_nav(szTarget);
			}else{
				Sleep(1000);
			}
		};

		RETURNSWI;
		if(v < -1000)
		{
			AfxMessageBox(_T("failed at t ")+lastnav);
		}

		str = ExtractString(Param,',');
		str1.Format(_T("%.7f"),v);
		TRACE(str+_T("=")+str1+_T("\n"));
		prg_var.SetAt(string2long((LPCTSTR)str,str.GetLength()),str1);
		
		return false;
	case 'w': //[w,target]
		str = ExtractString(Param,',');
		if(str == _T("rx"))
		{
			WAITSWI;
			sn = m_swinav.read_swi();
			RETURNSWI;
			str1.Format(_T("%i"),(sn.rx?1:0));
			prg_var.SetAt(string2long((LPCTSTR)str,str.GetLength()),str1);
		}
		if(str == _T("rs"))
		{
			WAITSWI;
			sn = m_swinav.read_swi();
			RETURNSWI;
			str1.Format(_T("%i"),(sn.rs?1:0));
			prg_var.SetAt(string2long((LPCTSTR)str,str.GetLength()),str1);
		}
		break;
	case 'p': //[p,time]
		pause_cnt = PAUSE_DIVIDER*wcstol(ExtractString(Param,','),NULL,10);
		return false;
	default:
		break;
	}
	return true;
}
void CTesterProgram::SetEnv(CString env)
{
	while(1){
		CString item = ExtractString(env,'|');
		if(item.IsEmpty())
			return;
		CString name = ExtractString(item,'=');
		prg_var.SetAt(string2long((LPCTSTR)name,name.GetLength()),item);
	}

}

void CTesterProgram::SetGlobalEnv(CString env)
{
	while(1){
		CString item = ExtractString(env,'|');
		if(item.IsEmpty())
			return;
		CString name = ExtractString(item,'=');
		env_var.SetAt(string2long((LPCTSTR)name,name.GetLength()),item);
	}

}
double CTesterProgram::CacuExpr(CString expr)
{
		int pos = 0;
		int pos_a,pos_s,pos_m,pos_d,pos_g,pos_l;
		CString left,right;
		pos_a = expr.ReverseFind('+');
		pos_s = expr.ReverseFind('-');
		pos_m = expr.ReverseFind('*');
		pos_d = expr.ReverseFind('/');
		pos_g = expr.ReverseFind('>');
		pos_l = expr.ReverseFind('<');
		
		
		pos = max(max(pos_g,pos_l),max(max(pos_a,pos_s),max(pos_m,pos_d)));

		if(pos < 0){
			return wcstod(expr,NULL);
		}
		left = expr.Left(pos);
		right = expr.Right(expr.GetLength()-(pos+1));
		double op1 = CacuExpr(left);
		double op2 = CacuExpr(right);

		if(expr[pos] == '*')
			return op1 * op2;
		
		if(expr[pos] == '-')
			return op1 - op2;

		if(expr[pos] == '+')
			return op1 + op2;

		if(expr[pos] == '/')
			return op1 / ((op2 == 0)? 1 : op2);
		
		if(expr[pos] == '>')
			return (op1 > op2) ? 1.0 : 0.0 ;
		
		if(expr[pos] == '<')
			return (op1 < op2) ? 1.0 : 0.0 ;
		
		return wcstod(expr,NULL);
}


void CTesterProgram::ApplyEnv(CString &line){
	int i=0;
	while(i++ < 1000)
	{
		int pos2 = line.Find(_T("@env\""));
		if( pos2 < 0)
			break;

		int pos1 = pos2;
		while(pos1-- > 0){
			if(line[pos1] == '"')
				break;
		}
		CString item = line.Mid(pos1+1,pos2-(pos1+1));
		CString value;
		env_var.Lookup(string2long((LPCTSTR)item,item.GetLength()),value);
		line.Replace(_T("\"")+item+_T("@env\""),value);
	}
	if(i>=1000)
	{
		AfxMessageBox(_T("weird thing"));
	}
	i=0;
	while(i++<1000)
	{
		int pos2 = line.Find(_T("@prg\""));
		if( pos2 < 0)
			return;

		int pos1 = pos2;
		while(pos1-- > 0){
			if(line[pos1] == '"')
				break;
		}
		CString item = line.Mid(pos1+1,pos2-(pos1+1));
		CString value;
		prg_var.Lookup(string2long((LPCTSTR)item,item.GetLength()),value);
		line.Replace(_T("\"")+item+_T("@prg\""),value);
	}
	if(i>=1000)
	{
		AfxMessageBox(_T("weird thing"));
	}
	
	return;
}