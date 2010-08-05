#include "stdafx.h"

#include "TesterProgram.h"
#include "TesterCaculate.h"
#include "HtmlHandler.h"
#include "Tester.h"
#include "TryData3Dlg.h"
extern CString g_Path;
extern CTesterData g_Data;
TESTERCONF g_TesterConf;
CTester *g_Tester;
extern CMapStringToString g_Result;
extern void CopySTSMap(CMapStringToString& src,CMapStringToString& dst);
extern int bExit;
char CTester::pushtype='1';
CString CTester::pushparam = CString("");
static CString single("");
#define SPORT_NAVMETER   'n'
#define SPORT_SWITCH	 's'
#define PPORT_PRINTER	 1
#define CHECKWAIT		110//30*100ms

#include "WaitDlg.h"
#include ".\tester.h"

/************************************************************************
 * RstdSettingCombination is used to merge the parameter 
 * when external standard is pluged in
************************************************************************/
void CTester::RstdSettingCombination(const RSTDTYPE ds,RSTDTYPE	&rs)
{
	SWISTATUS ss;
	ss = m_Switch.GetSwitchStatus();
	
	//the RN0 always use the external standard value
	for(int i=1;i<7;i++)
	{
		//if the outer standard not pluged, then use the internal ones
		if(!ss.bKN[i-1])
		{
			rs.dAlpha[i] = ds.dAlpha[i];
			rs.dBeta[i] = ds.dBeta[i];
			rs.dValue[i] = ds.dValue[i];
		}
	}
}


CTester::CTester(CWnd *pwnd)
{
	m_pwnd = pwnd;
	m_CheckTicks = 0;
	m_bTestMode = 0;
}
CTester::~CTester()
{
}
extern  const char* PathQuery(int pathid,int clsid);										
void CTester::Begin()
{
	CString title;
	title.Format("%s",PathQuery(CLSGROUP,IDREG));
	if(title.Compare("nbz") != 0)
		RstdSettingCombination(g_TesterConf.m_RsDefault,g_TesterConf.m_Rs);

	g_TesterConf.m_iRetestID = -1;
	if(m_Program.CommandBegin() == 0)
		return;
	m_Switch.DoAction(RMARK);
	return;
}

void CTester::End(UINT flag)
{
	RESDATA rd;
	m_Program.CommandFinish();
	if((flag == BEFINI)||(flag == BEIDLE))
	{
		if(g_TesterConf.m_iRetestID != -1){
			//override the result with the retest result 
			rd = g_Data.GetRecord(g_TesterConf.m_iRetestID+g_TesterConf.m_iBase);
			g_Data.SetRecord(g_TesterConf.m_iRetestID,rd);
		}
		if(flag == BEFINI)
			SetHtmlText("rtest:header",(ReplaceReportDefine())?_T("done"):_T("fail"));
	}else{
		SetHtmlText("rtest:header",_T("error"));
	}
}

void CTester::Pause()
{
	// TODO: Add your control notification handler code here
	m_Program.Pause();
}
void CTester::Continue()
{
	m_Program.Continue();
}

void CTester::Retest(int pos) 
{
	// TODO: Add your control notification handler code here
	g_TesterConf.m_iRetestID = pos;
	if(m_Program.CommandBegin() == 0)
		return;
	m_Switch.DoAction(RMARK);	
}

void CTester::OverLoadHandle()
{
	char sParam[300];
#ifndef NOOVERCHECK
	SWISTATUS ss;
//	m_Program.Pause();
	ss = m_Switch.GetSwitchStatus();
	if(ss.bOl1)
	{
//		sprintf(sParam,_T("OST1]"));
//		m_Switch.DoSwi(sParam);
		::AfxMessageBox("发生过载");
//		no reset and ERROR abort
//		m_Switch.DoAction(RMARK);
//		CTesterProgram::State = BEERRO;
	}
	/*
	if(ss.bOl2)
	{
		sprintf(sParam,_T("OST2]"));		
		m_Switch.DoSwi(sParam);
		
		::AfxMessageBox("扩展电流源发生过载");
		m_Switch.DoAction(RMARK);
		CTesterProgram::State = BEERRO;
	}
	*/
#endif
}

bool CTester::InternalRoutines(char index)
{
	//double value;
	int i;
	int count=0;
	RSTDTYPE rt;
	RESDATA rd;
	double sum,min,max;
	
	switch(index)
	{
	case '1':
		//check process used here is for standard resistor test
		//requiration: should and only one outer resistor used
		//			 : and resistor value should greater than 0


		for(i = 0;i < 9;i++)
			count += (g_TesterConf.m_Rs.bUse[i])?1:0;

		if(count > 1)	::AfxMessageBox("注意：只能使用一个外标准电阻");
		if(count == 0)	::AfxMessageBox("注意：必须使用一个外标准电阻");
		
		if(g_TesterConf.m_Rs.bUse[0])
		{
			g_TesterConf.m_Rs.iScaledR0 = 0;
			if(g_TesterConf.m_Rs.dValue[0] < 0.005)
				g_TesterConf.m_Rs.iScaledR0 = 1;
			else if(g_TesterConf.m_Rs.dValue[0] < 0.05)
				g_TesterConf.m_Rs.iScaledR0 = 2;
			else if(g_TesterConf.m_Rs.dValue[0] < 0.5)
				g_TesterConf.m_Rs.iScaledR0 = 3;
		}
		return (count == 1);
	case '2':
		//replace the checked value to the default one
		//checked one are stored in Z11-Z16
		rt = g_TesterConf.m_RsDefault;
		for(i = 1;i < 7;i++)
		{
			rd = g_Data.GetRecord(10 + i);
			if(rd.rbResult)
			{
				rt.dAlpha[i]=0;
				rt.dBeta[i]=0;
				rt.dValue[i]=rd.rResult;
			}	
		}
		g_TesterConf.m_RsDefault = rt;//CHtmlHandler::ModifyDefaultRstdSetting(rt);
		return true;
	case '3':
		//check does the outer standard exist
		count = 0;
		for(i=0;i< 9 ;i++)
			index += (g_TesterConf.m_Rs.bUse[i])?1:0;

		if(index == 0)
			::AfxMessageBox(_T("请检查并选择至少一个外标准电阻"));

//		g_TesterConf.m_RsDefault = g_TesterConf.m_Rs;
		return (index > 0);
	case '4':
		//caculate the average and the standard different between r0

		sum = 0;
		rd = g_Data.GetRecord(10);	
		sum += rd.rResult; 
		min = sum; 
		max = sum;
		rd = g_Data.GetRecord(11);  
		sum += rd.rResult; 
		min = (rd.rResult < min)? rd.rResult:min; 
		max = (rd.rResult > max)? rd.rResult:max; 
		rd = g_Data.GetRecord(9);  
		sum += rd.rResult; 
		min = (rd.rResult < min)? rd.rResult:min; 
		max = (rd.rResult > max)? rd.rResult:max; 
		
		rd.rResult = sum/3.0;
		g_Data.SetRecord(9,rd);

		rd = g_Data.GetRecord(10);
		rd.rResult = 300*(max-min)/sum;
		g_Data.SetRecord(10,rd);

		return true;
	case '5':
		//copy the user set to the current setting
		//which is useful in check test case begin;
//?		g_TesterConf.m_Rs = CTryData3Dlg::RetrieveRstdSetting();
		return true;
	case '6':
		//scale the Z8 to power of ten
		rd = g_Data.GetRecord(8);
		if(rd.rbResult)
		{
			rd.rResult=CTesterCaculate::ConvertDataExp(rd.rResult)/100;
			g_Data.SetRecord(8,rd);
		}
		return true;
	case '7':
		//pause the status 
		g_Tester->Pause();
	default:
		break;
	}
	return true;

}

bool CTester::Quit() 
{
	// TODO: Add your control notification handler code here
	if((CTesterProgram::State == BETEST)||(CTesterProgram::State == BEPAUSE))
		return false;

	m_Switch.DoAction(RMARK);
	m_Switch.Close();
	m_Navmeter.Close();
#ifdef USEPRINTER
	m_WHPrinter.Close();
#endif
	return true;
}
BEGIN_MESSAGE_MAP(CTester, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTester::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	UINT flag;
	SWISTATUS ss;
	CString str;
	bool bOk = true;
	
	if (nIDEvent == 1)
	{
		int reason;
		KillTimer(1);
		reason = 0;	
		reason = (reason != 0) ? reason: (CTesterCaculate::CheckDriveID() ? 0 : 7);
		reason = (reason != 0) ? reason: (m_Switch.Open(SPORT_SWITCH) ? 0 : 2);
		reason = (reason != 0) ? reason: (m_Navmeter.Open(SPORT_NAVMETER) ? 0 : 1);
#ifndef  NOPRINTER
		m_WHPrinter.Open(PPORT_PRINTER);
#endif
		delete ((CTryData3Dlg*)this->GetParent())->splash;
		if(reason != 0){
			if(reason == 1)
				::AfxMessageBox("连接纳伏计失败");
			if(reason == 2)
				::AfxMessageBox("连接控制板失败");
			if(reason == 3)
				::AfxMessageBox("连接打印机失败");
			if(reason == 6)
				::AfxMessageBox("未发现测试程序");
			if(reason == 7)
				::AfxMessageBox("本软件无法在本机上使用,请联系供应商");
			bExit = 1;
			::AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
		}
		::AfxGetMainWnd()->SetActiveWindow();
		::AfxGetMainWnd()->ShowWindow(SW_NORMAL);
		SetTimer(2,30,NULL);
		return;
	}	
	
	if(nIDEvent == 2)
	{
		if(!ListenClipboard())
			return;
		if(!single.IsEmpty()){
			if(CTesterProgram::State == BEIDLE)
			{
				CTesterProgram::State = BETEST;

				g_Tester->SingleAction(single);
				if(CTesterProgram::State == BETEST)
					CTesterProgram::State = BEIDLE;
				return;
			}
		}
//		if(CTesterProgram::State == BEPAUSE)
//			return;
		
		if((CTesterProgram::State == BEIDLE) || (CTesterProgram::State == BEPAUSE))
		{
			if(m_CheckTicks++ < CHECKWAIT)
				return;
			m_CheckTicks = 0;

#ifndef	NOOVERCHECK
			KillTimer(2);
			UINT st= CTesterProgram::State;
			//m_Program.Pause();
			CTesterProgram::State = BEPAUSE;
			SWISTATUS ss = m_Switch.GetSwitchStatus();
			while(ss.bOl1)
			{
				::AfxMessageBox("发生过载");
				ss = m_Switch.GetSwitchStatus();
			}
			str.Format("外标准[%s],被测[%s]",ss.bKN[0]?"接通":"未接",ss.bRn?"接通":"未接");
			SetHtmlText("rtest:switch",str);
			//m_Program.Continue();
			m_Program.State = st;
			SetTimer(2,30,NULL);
#endif			
			return;
		}
		if(CTesterProgram::State != BETEST)
		{
			KillTimer(2);
			this->End(CTesterProgram::State);
			SetTimer(2,30,NULL);
			m_CheckTicks = 0;
			return;
		}
		if(!pushparam.IsEmpty())
		{
			if(!HandleCommand(pushtype,pushparam))
				m_Program.Abort();
			pushparam.Empty();
			return;
		}
		if(!m_Program.CommandStep())
		{
#ifndef	NOOVERCHECK
			if(m_Switch.CheckOverLoad() != 0)
			{
				OverLoadHandle();
			}
#endif
			m_Program.Abort();
		}
	}
	if(nIDEvent == 3)
	{
		int i=0;
		while(i++<100)
			DoEvents();
		SetHtmlText("rtest:display",_T("测量完毕"));
	}
	//CFormView::OnTimer(nIDEvent);
}
bool CTester::HandleCommand(char type, CString Parm)
{
	CString str,str1;
	char  sParm[300];
	double data[20];
	int count;
	int max,min,lines,left;
	char oper;
	bool bRet;
	RESDATA rd;
	CWaitDlg *wd;
			CString szlog;
		char tmpbuf[128];

	switch(type)
	{
	case 's':	//turn the switch
		
		SetHtmlText("rtest:display","开关切换");
		strcpy(sParm,(LPCSTR)Parm);
		return m_Switch.DoSwi(sParm);
	case 't':	//measure and retrieve the value 
		SetHtmlText("rtest:display",_T("取得读数"));
		CTesterCaculate::ExtractInt(Parm,lines);
		count = 0;
		while(count < g_TesterConf.m_Ps.iTimes)
		{
			max = 0;
#ifndef NOSWITCH
			while(g_TesterConf.m_Ps.iTimes*max++ < (g_TesterConf.m_Ps.iDelay*20))
			{
				Sleep(20);
				DoEvents();
				if((CTesterProgram::State != BEPAUSE) && (CTesterProgram::State != BETEST))
					return false;

				if(CTesterProgram::State == BEPAUSE)//BEIDLE 
					max = 0;					
			}
#endif
			if(m_Navmeter.SendCommand(NAV_READ))
			{
				data[count++]=m_Navmeter.value;
				_strtime(tmpbuf);
				szlog.Format("[%s]:%.12f\n",tmpbuf,m_Navmeter.value);
				//m_Program.logout.WriteString(szlog);
				continue;
			}
			if(!m_Navmeter.SendCommand(NAV_INIT))
				return false;
			continue;	//try again
		}
		rd = g_Data.GetRecord(g_TesterConf.m_iBase+lines);
		rd.rbValue = true;	
		rd.rValue = CTesterCaculate::GetAverage(data,g_TesterConf.m_Ps.iTimes);
		g_Data.SetRecord(g_TesterConf.m_iBase+lines,rd);
		break;
	case 'm'://[m,if,ifvalue,msg,option]
		//show the message
		max = CTesterProgram::State; 
		CTesterProgram::State = BEPAUSE;
		CTesterCaculate::ExtractString(Parm,str1);
		ASSERT(str1.GetLength() > 2);

		min = -1;
		CTesterCaculate::ExtractInt(Parm,min);
		if(min != -1){
			wd = new CWaitDlg();
			if(wd != NULL)
			{
				BOOL ret = wd->Create(IDD_WAITDLG,this);
				if(ret){
					wd->ShowWindow(SW_HIDE);
					wd->PushMsg(str1);
					//check status first
					int i=5;
					while(i-- > 0){
						if(m_Navmeter.SendCommand(NAV_READ))
							wd->PushData(m_Navmeter.value);
					}
					wd->ShowWindow(SW_NORMAL);
					while(!wd->IsClosed()){
						if(m_Navmeter.SendCommand(NAV_READ))
							wd->PushData(m_Navmeter.value);
					}
					return true;
				}
				delete wd;
				wd = NULL;
			}
		}
		
		AfxMessageBox(str1);
		CTesterProgram::State = max; 
		return true;		
	case 'n':		//[ID]Send command to navmeter	using ID 	
		CTesterCaculate::ExtractInt(Parm,left);
		CTesterCaculate::ExtractString(Parm,str);
		return m_Navmeter.SendCommand(left);
	case 'i':	    //[ID]Run the InternalRoutines (ID)
		CTesterCaculate::ExtractChar(Parm,oper);
		m_Program.Pause();
		bRet = InternalRoutines(oper);
		m_Program.Continue();
		return bRet;
	case 'd': //control of the FlexGrid
		return HandleDataShow(Parm);
	case 'p':
		CTesterCaculate::ExtractInt(Parm,min);
		if(min == -1)
			min = g_TesterConf.m_iDelayKT;
		if(min == -2)
			min = g_TesterConf.m_iDelayKI;
		if(0){
			CTesterProgram::State = BEPAUSE;
			str.Format("等待 %is...",min);
			::AfxMessageBox(str);
			CTesterProgram::State = BETEST;
			return true;
		}
		max = 0;

		while(max++ < min*100)
		{
			Sleep(8);
			if(CTesterProgram::State == BETEST){
				CTesterProgram::State = BEPAUSE;
				DoEvents();
				if((CTesterProgram::State != BEPAUSE)&&(CTesterProgram::State != BETEST))
					return false;
				CTesterProgram::State = BETEST;
			}
			if((max%100) == 0)
			{
				str.Format("等待 %is...",min-(max/100));
				SetHtmlText("rtest:display",str);
			}
			if(CTesterProgram::State == BEERRO)
				return false;
		}
		break;
#ifndef NOPRINTER
	case 'w':
		CTesterCaculate::ExtractChar(Parm,oper);
		SYSTEMTIME syt;
		switch(oper)
		{
		case 'd':
			::GetLocalTime(&syt);
			str.Format("%i-%i-%i %i:%i",syt.wYear,syt.wMonth,syt.wDay,syt.wHour,syt.wMinute);
			break;
		case 't':
			CTesterCaculate::ExtractString(Parm,str);
			str.Replace("\\n","\n");
			break;
		case 'v':
			CTesterCaculate::ExtractInt(Parm,min);
			CTesterCaculate::ExtractInt(Parm,max);
			rd = g_Data.GetRecord(min);
			if(rd.rbValue)
				str = CTesterCaculate::DoublePrecision(rd.rValue,max);
			else
				str.Format("数据错误");

			break;
		case 'z':
			CTesterCaculate::ExtractInt(Parm,min);
			CTesterCaculate::ExtractInt(Parm,max);
			rd = g_Data.GetRecord(min+g_TesterConf.m_iBase);
			if(rd.rbResult)
				str = CTesterCaculate::DoublePrecision(rd.rResult,max);
			else
				str.Format("数据错误");
			break;
		default:
			return false;
		}
		str += _T("\n\n");
		m_WHPrinter.PrinterWrite(ID_NORMALDATA_SEND,str);
#endif
		break;
	//added by songj 04-04-22 :标准变差显示
	case 'z':
		if((CTesterProgram::State == BETEST))
			CTesterProgram::State = BEPAUSE;
		CTesterCaculate::ExtractChar(Parm,oper);
		CTesterCaculate::ExtractInt(Parm,min);
		CTesterCaculate::ExtractInt(Parm,max);
		CTesterCaculate::ExtractString(Parm,str1);
		str.Format("数据错误");
		if(oper == 'v')
		{
			str.Format("Z%d:",min);
			rd = g_Data.GetRecord(min);
			if(rd.rbValue)
				str += CTesterCaculate::DoublePrecision(rd.rValue,max);
		}
		if(oper == 'z')
		{
			str.Format("Z%d:",min+g_TesterConf.m_iBase);
			rd = g_Data.GetRecord(min+g_TesterConf.m_iBase);
			if(rd.rbResult)
				str += CTesterCaculate::DoublePrecision(rd.rResult,max);
			else
				str += CString("N/A");
		}
		SetHtmlText("rtest",str);
		if(CTesterProgram::State == BEPAUSE)
			CTesterProgram::State = BETEST;
		break;
	case 'u':
		CTesterCaculate::ExtractChar(Parm,oper);
		if(oper == 'r')
		{
			//since we 
		}
		if(oper == 't')
		{
			CTesterCaculate::ExtractString(Parm,str);
			CTester::SetHtmlText("rtest:header",str);
		}
		break;		
		
	default:
		break;
	}
	return true; //undefined command
}

void CTester::DoEvents()
{
	MSG msg;
	if (::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) 
	{
		if(!AfxGetApp()->PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);	
		}
	}
	AfxGetApp()->OnIdle(0);AfxGetApp()->OnIdle(1);
}
bool CTester::ReplaceReportDefine()
{
//	g_Result.RemoveAll();
	double value;
	for(int i=1;i < ZINDEX_MAX;i++){
		CString zid,zvalue;
		zid.Format("Z%d",i);
		g_Result.RemoveKey(zid);
		RESDATA rd = g_Data.GetRecord(i);
		if(rd.rbResult){
			zvalue.Format("%f",rd.rResult);
			g_Result.SetAt(zid,zvalue);
		}
	}
	return true;
}

bool CTester::HandleDataShow(CString line)
{
	//we post data to clipboard  here :)
	//type = 'r','c','t','v','z','p'
	//={rowset(if(-1)added),columnset,textset,valueuse,resultuse,store positioninFlexGrid}
	char type;
	int  num;
	CString str;
	RESDATA rd;
	CTesterCaculate::ExtractChar(line,type);
	switch(type)
	{
	case 'r':
	case 'c':
	case 'd':
	case 't':
		break;
	case 'v':
		CTesterCaculate::ExtractInt(line,num);
		if(num < 0)	break;
		
		str.Format("V%d:",num);
		rd = g_Data.GetRecord(num);
		if(rd.rbValue)
			str += CTesterCaculate::DoublePrecision(rd.rValue,9);
		//str.Format("%9f",rd.rValue);
		else
			str += TEXT("NA");
		SetHtmlText("rtest",str);
		break;
	case 'z':
		CTesterCaculate::ExtractInt(line,num);
		str.Format("Z%d:",num);
		rd = g_Data.GetRecord(num+g_TesterConf.m_iBase);
		if(rd.rbResult)
		{
			str += CTesterCaculate::DoublePrecision(rd.rResult,9);
		}
		else
			str += TEXT("NA");
		SetHtmlText("rtest",str);
		break;
	case 'p':
		break;
	default:
		return false;
	}
	return true;
}
void CTester::PushCommand(char type,CString Parm)
{
	pushtype = type;
	pushparam = Parm;
}
void CTester::SetHtmlText(CString title,CString content)
{
	CString source;
	source = title+':'+content;
	while (1) {
		if(OpenClipboard())
		{
			HGLOBAL clipbuffer;
			TCHAR *buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, 2*source.GetLength()+2);
			buffer = (TCHAR*)GlobalLock(clipbuffer);
			strcpy(buffer, LPCSTR(source));
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT,clipbuffer);
			CloseClipboard();
			break;
		}
	}
	Sleep(200);
	return;
}

bool CTester::ListenClipboard()
{
	char * buffer = NULL;
	//open the clipboard
	CString fromClipboard;
	if(!g_Tester->m_bTestMode)
		return true;
	if ( OpenClipboard() ) 
	{
		HANDLE hData = GetClipboardData( CF_TEXT );
		char * buffer = (char*)GlobalLock( hData );
		fromClipboard = buffer;
		GlobalUnlock( hData );
		EmptyClipboard();
		CloseClipboard();
	}else
		return true;
	if(fromClipboard.Find("test-cmd",0) != 0)
		return true;
	//handle test-cmd and so on:)
	if(fromClipboard.Find(":abort") > 0){
		CTesterProgram::State = BEERRO;
		return false;
	}
	if(fromClipboard.Find(":begin") > 0){
		g_Tester->Begin();
		return false;
	}
	if(fromClipboard.Find(":pause") > 0){
		g_Tester->Pause();
		return false;
	}
	if(fromClipboard.Find(":retest:Z") > 0){
		int pos = fromClipboard.Find(":retest:Z");
		fromClipboard.Delete(0,pos+9);
		pos = atoi(fromClipboard);
		if(pos > 0)
			g_Tester->Retest(pos);
		return false;
	}
	if(fromClipboard.Find(":testonce:Z") > 0){
		int pos = fromClipboard.Find(":testonce:Z");
		fromClipboard.Delete(0,pos+11);
		if(single.IsEmpty())
			single = fromClipboard;
		//g_Tester->SingleAction(fromClipboard);
		return false;
	}
	if(fromClipboard.Find(":fillcommit:") > 0){
		int pos = fromClipboard.Find(":fillcommit:");
		fromClipboard.Delete(0,pos+12);
		pos = fromClipboard.Find('=',0);
		g_Result.SetAt(fromClipboard.Left(pos),fromClipboard.Right(fromClipboard.GetLength()-pos-1));
		return false;
	}
	if(fromClipboard.Find(":continue") > 0){
		g_Tester->Continue();
		return false;
	}
	if(fromClipboard.Find(":draw") > 0){
		if(CTesterProgram::State == BEIDLE){
			//show the dialog
		}
		return false;
	}
	
	return true;
}
extern CString SJD2SByPosition(double,int );
extern int SJD2SGetcarryC(char,char,int);
extern int SJD2SGetcarryI(int,int,int);
extern char SJD2SGetchar(double,int);
// do single command depend on cmd string
void CTester::SingleAction(CString cmd)
{
	single.Empty();
	//parse the cmd structure first as /ZPos=prec:type:parameter,
	CString sPos =  cmd.Left(cmd.Find(':'));
	CString sPrec=  sPos.Right(sPos.GetLength()-sPos.Find('=')-1);
	sPos = sPos.Left(sPos.Find('='));
	cmd = cmd.Right(cmd.GetLength()-cmd.Find(':')-1);
	int nType = atoi(cmd.Left(cmd.Find(':')));
	cmd = cmd.Right(cmd.GetLength()-cmd.Find(':')-1);
	//parse the configuration
	TESTERCONF tempconf = g_TesterConf;
/*	
	while(cmd.Find(':') > 0){
		char cParamType = cmd[0];
		int iValue = atoi(cmd.Mid(2,cmd.Find(':')-2));
		switch(cParamType){
			case 'I':	g_TesterConf.m_iDelayKI = iValue;break;
			case 'T':	g_TesterConf.m_iDelayKT = iValue;break;
		}
	}
*/
	if(nType == 1){//just read a voltage value only for dwcj test
			//turn on the filters
	
		HandleCommand('n',CString("12]"));
		HandleCommand('n',CString("14]"));
		if(cmd.GetLength() >= 1)
		{
			//speed option exists
			HandleCommand('n',cmd+CString("]"));
		}
		if( g_TesterConf.m_Ps.iTimes == 0)
			g_TesterConf.m_Ps.iTimes = 1;

		int iPos=g_TesterConf.m_iBase+atoi(sPos);

		if((iPos == 170)||(iPos == 190)||(iPos == 130)||(iPos == 150)){
			HandleCommand('m',_T("请将数字表短路清零后按'确定']"));
			HandleCommand('n',_T("4]"));
			HandleCommand('m',_T("调整电位差计工作电流（对标准）后开始测量]"));
			
		}
		


		HandleCommand('t',_T("1]"));
		g_Data.OperateValue('Z','Z','-',iPos,iPos,'0',iPos);
		g_Data.OperateValue('Z','V','+',iPos,1,'0',iPos);
		
		RESDATA rd;
		double dZero1 = 0;
		double dZero2 = 0;
		double dZero3 = 0;
		double dZero4 = 0;
		rd = g_Data.GetRecord(190);
		if(rd.rbResult) {
			dZero1 = rd.rResult;
			if((iPos >= 190) && (iPos < 210)) dZero1 = 0;
			if((iPos == 170)||(iPos == 150)||(iPos == 130))
				return;
		}
		else
		{
			rd = g_Data.GetRecord(170);
			if(rd.rbResult) {
				dZero1 = rd.rResult;
				if((iPos >= 170) && (iPos < 190)) dZero2 = 0;
				if((iPos == 190)||(iPos == 150)||(iPos == 130))
					return;

			}
			else
			{
				rd = g_Data.GetRecord(150);
				if(rd.rbResult) {
					dZero1 = rd.rResult;
					if((iPos >= 150) && (iPos < 170)) dZero3 = 0;
					if((iPos == 170)||(iPos == 190)||(iPos == 130))
						return;

				}
				else
				{
					rd = g_Data.GetRecord(130);
					if(rd.rbResult) {
						dZero2 = rd.rResult;
						if((iPos >= 130) && (iPos < 150)) dZero4 = 0;
						if((iPos == 170)||(iPos == 150)||(iPos == 190))
							return;
					}
				}
			}
		}
		double dZero = (dZero1 + dZero2 + dZero3 + dZero4)*1e-6;

		double dFull = 0;
		rd = g_Data.GetRecord(200);
		if(rd.rbResult) dFull = 0.0000001;
		rd = g_Data.GetRecord(180);
		if(rd.rbResult) dFull = 0.000001;
		rd = g_Data.GetRecord(160);
		if(rd.rbResult) dFull = 0.00001;
		rd = g_Data.GetRecord(140);
		if(rd.rbResult) dFull = 0.0001;
		rd = g_Data.GetRecord(110);
		if(rd.rbResult) dFull = 0.001;
		rd = g_Data.GetRecord(80);
		if(rd.rbResult) dFull = 0.01;
		rd = g_Data.GetRecord(50);
		if(rd.rbResult) dFull = 0.1;

		rd = g_Data.GetRecord(iPos);
		
		//caculate the diff value;
		double dstd = 0;
		if((iPos >= 40) && (iPos < 70) )  dstd = ((iPos-40))*0.1;
		if((iPos >= 70) && (iPos < 100) )  dstd = ((iPos-70))*0.01;
		if((iPos >= 100) && (iPos < 130))  dstd = ((iPos-100))*0.001;
		if((iPos > 130) && (iPos < 150))  dstd = ((iPos-130))*0.0001;
		if((iPos > 150) && (iPos < 170))  dstd = ((iPos-150))*0.00001;
		if((iPos > 170) && (iPos < 190))  dstd = ((iPos-170))*0.000001;
		if((iPos > 190) && (iPos < 210))  dstd = ((iPos-190))*0.0000001;

		if((iPos >= 221) && (iPos < 224))  dstd = dFull*((iPos-221)*5+10)*0.1;
		if((iPos >= 224) && (iPos < 227))  dstd = dFull*((iPos-224)*5+10)*0.2;
		if((iPos >= 227) && (iPos < 230))  dstd = dFull*((iPos-227)*5+10)*5;

		rd.rResult = -(dstd-rd.rResult+dZero);
		rd.rResult *= 1e6;
		g_Data.SetRecord(iPos,rd);

//		HandleCommand('d',"z,"+sPos+","+sPrec+"]");
		char nchar = SJD2SGetchar(rd.rResult,-2);//last char
		char n1char = SJD2SGetchar(rd.rResult,-1);//next last char
		int  carry = SJD2SGetcarryI(n1char-'0',nchar-'0',1);//get carry char

		rd.rResult += carry*1e-1;//adjust
		sPos.Format("Z%d",iPos);
		if(rd.rbResult){
			sPrec = SJD2SByPosition(rd.rResult,atoi(sPrec)-8);
		}
			//sPrec = CTesterCaculate::DoublePrecision(rd.rResult,atoi(sPrec));
		else 
			sPrec.Empty();
		g_Result.SetAt(sPos,sPrec);
		SetHtmlText("rtest",sPos+":"+sPrec);
	}
	if((nType == 2)&&(cmd.GetLength() > 1))
	{
		//turn on the filters
		HandleCommand('n',CString("12]"));
		HandleCommand('n',CString("14]"));

		CString sSwiI,sSwiH;
		CString scmd;
		CString T1("1]"),T2("1]"),T3("1]"),T4("1]");
		CString speed;
		char scale = cmd[0];
		RESDATA rd;
		sPos = _T("Z")+sPos;
		//parameter = scale:t1:t2:t3:t4:
		T1 = cmd.Mid(2,cmd.Find(':',2)-2) + CString("]");
		cmd = cmd.Right(cmd.GetLength()-cmd.Find(':',2)+1);
		T2 = cmd.Mid(2,cmd.Find(':',2)-2) + CString("]");
		cmd = cmd.Right(cmd.GetLength()-cmd.Find(':',2)+1);
		T3 = cmd.Mid(2,cmd.Find(':',2)-2) + CString("]");
		cmd = cmd.Right(cmd.GetLength()-cmd.Find(':',2)+1);
		T4 = cmd.Mid(2,cmd.Find(':',2)-2) + CString("]");
		if(cmd.Find(':',2) >= 0)
		{
			//speed option exists
			cmd = cmd.Right(cmd.GetLength()-cmd.Find(':',2)+1);
			speed = cmd.Mid(2,cmd.Find(':',2)-2) + CString("]");
			if(speed.GetLength() >= 2)
				HandleCommand('n',speed);
		}
		//biao zhun rtest, parameter = scale (1-9)
		if((scale > '1')&&(scale < '8')){
			switch(scale){
				case '2'://1ohm
					sSwiI = "1"; sSwiH = "1";break;
				case '3'://10ohm
					sSwiI = "2"; sSwiH = "2";break;
				case '4'://100ohm
					sSwiI = "2"; sSwiH = "3";break;
				case '5'://1kohm
					sSwiI = "3"; sSwiH = "4";break;
				case '6'://10kohm
					sSwiI = "3"; sSwiH = "5";break;
				case '7'://100kohm
					sSwiI = "4"; sSwiH = "6";break;
			}
			scmd =_T("OKI")+sSwiI+_T(",OKN")+sSwiH+_T(",OHP")+sSwiH+_T(",OHN")+sSwiH+_T("]");
			HandleCommand('s',scmd);
			HandleCommand('p',_T("")+T1);
			HandleCommand('t',_T("1]"));
			HandleCommand('s',_T("NKTT]"));
			HandleCommand('p',_T("")+T2);
			HandleCommand('t',_T("2]"));
			g_Data.OperateValue('V','V','+',1,2,'1',1);
			g_Data.OperateValue('Z','Z','-',2,2,'0',2);
			g_Data.OperateValue('Z','Z','+',2,1,'0',2);
//			HandleCommand('p',_T("1]"));
			scmd= _T("FHP")+sSwiH+_T(",FHN")+sSwiH+_T(",OHX1,OHX2]");
			HandleCommand('s',scmd);
			HandleCommand('p',_T("")+T3);
			HandleCommand('t',_T("1]"));
			HandleCommand('s',_T("NKTT]"));
			HandleCommand('p',_T("")+T4);
			HandleCommand('t',_T("2]"));
			g_Data.OperateValue('V','V','+',1,2,'1',1);
			g_Data.OperateValue('Z','R','*',1,scale-'1','0',3);
			g_Data.OperateValue('Z','Z','/',3,2,'0',3);
			HandleCommand('s',_T("FHX1,FHX2]"));
			rd = g_Data.GetRecord(3);
			if(atoi(sPrec) < 0)
				sPrec = SJD2SByPosition(rd.rResult,atoi(sPrec));
			else
				sPrec = CTesterCaculate::DoublePrecision(rd.rResult,atoi(sPrec));
			
			g_Result.SetAt(sPos,sPrec);
			SetHtmlText("rtest",sPos+":"+sPrec);
		}else{
			CTesterProgram::State = BEPAUSE;
			HandleCommand('m',_T("请确认已接入扩展电流源和RX、RN电阻]"));
			CTesterProgram::State = BETEST;
			HandleCommand('s',_T("FHX1,FHX2,FFX1]"));
			switch(scale){
				case '1': //0.1ohm
					sSwiH = "FHP7,FHP8,FHP9,FHP0]";break;
				case '8':// 0.01ohm
					sSwiH = "FHP7,OHP8,FHP9,FHP0]";break;
				case '9'://0.001,0.0001
					sSwiH = "OHP7,OHP8,FHP9,FHP0]";break;
			}
			HandleCommand('s',_T("")+sSwiH);
			HandleCommand('p',_T("")+T1);
			HandleCommand('t',_T("1]"));
            HandleCommand('s',_T("OHP9]"));
            HandleCommand('p',_T("")+T2);
            HandleCommand('t',_T("2]"));
			g_Data.OperateValue('V','V','+',1,2,'1',1);
            HandleCommand('p',_T("1]"));
            HandleCommand('s',_T("OHP0]"));;
			g_Data.OperateValue('Z','Z','-',2,2,'0',2);
			g_Data.OperateValue('Z','Z','+',2,1,'0',2);
			HandleCommand('p',_T("")+T3);
			HandleCommand('t',_T("1]"));;
            HandleCommand('s',_T("FHP9]"));
            HandleCommand('p',_T("")+T4);
            HandleCommand('t',_T("2]"));
			g_Data.OperateValue('V','V','+',1,2,'1',1);
			g_Data.OperateValue('Z','R','*',1,scale-'1','0',3);
			g_Data.OperateValue('Z','Z','/',3,2,'0',3);
			rd = g_Data.GetRecord(3);
			if(atoi(sPrec) < 0)
				sPrec = SJD2SByPosition(rd.rResult,atoi(sPrec));
			else
				sPrec = CTesterCaculate::DoublePrecision(rd.rResult,atoi(sPrec));
			g_Result.SetAt(sPos,sPrec);
			SetHtmlText("rtest",sPos+":"+sPrec);	
		}
	}
	g_TesterConf = tempconf;
	single.Empty();
	this->PostMessage(WM_TIMER,3,0);
}
