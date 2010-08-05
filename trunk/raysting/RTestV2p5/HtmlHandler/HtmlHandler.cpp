// HtmlHandler.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "..\CommonH&Libs\HtmlHandler.h"
#include "..\SjInclude\Double2String\sjdouble2string.h"

extern HTMLHANDLER_API CMapStringToString g_msTemp;
extern HTMLHANDLER_API TESTERCONF g_TesterConf;
extern HTMLHANDLER_API RTESTGROUP g_RGroup;

static const CString g_Lclst("自动 0.1 ");
static const CString g_Panlst("10");
static int Searchstring(LPCTSTR strid,const CString stable)
{
	int rpos = 0; //result position
	int spos = 0; //search position;
	while(spos >= 0)
	{
		if(spos == stable.Find(strid,spos))
			return rpos;
		spos = stable.Find(' ',spos+1);
		rpos++;
	}
	return spos;
}
static int searchpanlst(LPCTSTR strid)
{
	return Searchstring(strid,g_Panlst);
}
static int searchlclst(LPCTSTR strid)
{
	return Searchstring(strid,g_Lclst);
}



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
HTMLHANDLER_API int nHtmlHandler=0;

// This is an example of an exported function.
HTMLHANDLER_API int fnHtmlHandler(void)
{
	return 42;
}

extern CString SqlDelete(CString stable,CString sname,CString sgroup);
extern CString SqlAdd(CString stable,CString sname,CString sgroup);
extern CString SqlSelect(CString stable,CString sname,CString sgroup);
extern CString SqlNames(CString stable,CString sname,CString sgroup);
extern BOOL SJExtractTxtByMark (LPCTSTR tfname,LPCTSTR dfname,CString smark,void (*func)(LPCTSTR,LPCTSTR));
extern BOOL SJReplaceTxtByMark (LPCTSTR tfname,LPCTSTR dfname,CString smark,LPCTSTR (*func)(LPCTSTR));
extern BOOL SJReplaceWordByBookmark (LPCTSTR tfname,const CMapStringToString& smap);
extern BOOL SJFindFirstMatchByTxt (CString fname,CString& smatch);
extern CString SSearchDir(LPCTSTR ,LPCTSTR );
extern void CopySTSMap(const CMapStringToString&,CMapStringToString&);

// This is the constructor of a class that has been exported.
// see HtmlHandler.h for the class definition
CHtmlHandler::CHtmlHandler()
{ 
	return; 
}

bool CHtmlHandler::Handler(CString spage,CString op,CString scmd)
{
	bool brefresh=false;

	if(spage.Compare("reg")    == 0)		brefresh= RegHandler(op,scmd);
	if(spage.Compare("data")   == 0)		brefresh= DataHandler(op,scmd);
	if(spage.Compare("jh")	   == 0)		brefresh= JhHandler(op,scmd);
	if(spage.Compare("report") == 0)		brefresh= ReportHandler(op,scmd);
	if(spage.Compare("test")   == 0)		brefresh= TestHandler(op,scmd);
	if(spage.Compare("conf1")  == 0)		brefresh= Configure1Handler(op,scmd);
	if(spage.Compare("conf2")  == 0)		brefresh= Configure2Handler(op,scmd);
	if(spage.Compare("tbr")  == 0)			brefresh= TbrHandler(op,scmd);
	if(brefresh){
		g_msTemp.SetAt("testprglst",g_RGroup.sTestLst);
		g_msTemp.SetAt("testprgsz",g_TesterConf.m_RetestType);
		SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDTBR),m_gPath+TBR_FORM,MDB_PATTERN,CallBack_Replace);
	}
	return brefresh;
}
bool CHtmlHandler::RegHandler(CString op,CString scmd)
{
	bool brefresh = true;
	if(op.Compare("init") == 0)
	{
		TbrHandler("init","");
		g_RGroup.sRegLst = SqlNames(PathQuery(CLSMDB,IDREG),"*",PathQuery(CLSGROUP,IDREG));
		if(	g_RGroup.sRegSel.IsEmpty() || (g_RGroup.sRegLst.Find(g_RGroup.sRegSel,0) < 0))
			scmd = g_RGroup.sRegLst.Left(g_RGroup.sRegLst.Find(',',0));
		else
			scmd = g_RGroup.sRegSel;//use the value of last time
		
		op = _T("select");
	}
	if(op.Compare("select") == 0)
	{
		g_RGroup.sRegSel = SqlSelect(PathQuery(CLSMDB,IDREG),scmd,PathQuery(CLSGROUP,IDREG));
	}
	if(op.Compare("add") == 0)
	{
		g_RGroup.sRegSel = SqlAdd(PathQuery(CLSMDB,IDREG),scmd,PathQuery(CLSGROUP,IDREG));
		g_RGroup.sRegLst = SqlNames(PathQuery(CLSMDB,IDREG),"*",PathQuery(CLSGROUP,IDREG));
	}
	if(op.Compare("delete") == 0)
	{
		g_RGroup.sRegSel = SqlDelete(PathQuery(CLSMDB,IDREG),scmd,PathQuery(CLSGROUP,IDREG));
		g_RGroup.sRegLst = SqlNames(PathQuery(CLSMDB,IDREG),"*",PathQuery(CLSGROUP,IDREG));
	}
	//update the g_TestType
	g_msTemp.Lookup("xhgg",g_TesterConf.m_Xhgg);
	g_msTemp.SetAt("reglst",g_RGroup.sRegLst);
	g_msTemp.SetAt("regsz",g_RGroup.sRegSel);
	g_msTemp.SetAt("curview","reghtm");
	g_msTemp.SetAt("curhtm","reghtm");
	if(brefresh)
		SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDREG),m_gPath+REG_FORM,MDB_PATTERN,CallBack_Replace);
	
	return brefresh;
}
bool CHtmlHandler::ReportHandler(CString op,CString scmd)
{
	bool brefresh=false;
	if(op.Compare("init") == 0)
	{
		//set the corresponding report-setting
		if(g_RGroup.sReportLst.IsEmpty())
		{
			g_RGroup.sReportUrl = m_gPath+"\\doccopy.doc";
			g_RGroup.sReportLst = SSearchDir(m_gPath+"\\Doc","*.doc");
			g_RGroup.sReportLst.Replace("\n",",");
		}
		
		if(	g_RGroup.sReportSel.IsEmpty() || (g_RGroup.sReportLst.Find(g_RGroup.sReportSel,0) < 0))
			scmd = g_RGroup.sReportLst.Left(g_RGroup.sReportLst.Find(',',0));
		else
			scmd = g_RGroup.sReportSel;//use the value of last time
		
		op = _T("select");
	}
	
	if(op.Compare("print") == 0)
	{
		ShellExecute(0,"print",g_RGroup.sReportUrl,"","",SW_HIDE);
	}
	if(op.Compare("savenew") == 0)
	{
		CString tfile;
		char DirUse[300];
		::GetCurrentDirectory(300,DirUse);
		
		CFileDialog cfd(FALSE,_T("doc"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Doc File(*.doc)|*.doc||"),NULL);
		if(cfd.DoModal() == IDOK){
			tfile=cfd.GetPathName();
			CopyFile(g_RGroup.sReportUrl,tfile,TRUE);
		}
		::SetCurrentDirectory(DirUse);
	}
	if(op.Compare("select") == 0)
	{
		g_RGroup.sReportSel = scmd;
		SqlSelect(PathQuery(CLSMDB,IDREG),g_RGroup.sRegSel,"*");	//append the reg info
		SqlSelect(PathQuery(CLSMDB,IDDATA),g_RGroup.sDataSel,g_RGroup.sRegSel);//append the data info
		SqlSelect(PathQuery(CLSMDB,IDJH),g_RGroup.sJhSel1,PathQuery(CLSGROUP,IDJH));//append the jh info
		this->DataIntegrateForCheck(g_msTemp,ZINDEX_MAX,false);
		PrepareArrayForReport(g_msTemp);
		DeleteFile(g_RGroup.sReportUrl);
		if(CopyFile(m_gPath+"\\Doc\\"+g_RGroup.sReportSel+".doc",g_RGroup.sReportUrl,FALSE))
		{
			SJReplaceWordByBookmark(g_RGroup.sReportUrl,g_msTemp);
			brefresh = true;
		}
		else
			::AfxMessageBox("请关闭报告结果后重试");
	}
	
	CString stmp = g_RGroup.sReportUrl;
	stmp.Replace("\\","\\\\");
	g_msTemp.SetAt("reporturl",stmp);
	
	stmp = m_gPath+PathQuery(CLSJS,IDWAIT);
	stmp.Replace("\\","\\\\");
	g_msTemp.SetAt("waiturl",stmp);
	
	g_msTemp.SetAt("reportlst",g_RGroup.sReportLst);
	g_msTemp.SetAt("reportsz",g_RGroup.sReportSel);
	g_msTemp.SetAt("curview","reporthtm");
	g_msTemp.SetAt("curhtm","reporthtm");
	if(brefresh)			
		SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDREPORT),m_gPath+REPORT_FORM,MDB_PATTERN,CallBack_Replace);
	return brefresh;
}
bool CHtmlHandler::DataHandler(CString op,CString scmd)
{
	bool brefresh=true;
	if (op.Compare("init") == 0)
	{
		if(g_RGroup.sDataLst.IsEmpty())
		{
			//try to import the test result first
			CString Rtype(SCHEME_LIST);//find the result type
			if(SJFindFirstMatchByTxt(m_gPath+"\\Data\\temp.htm",Rtype))	//extract the result
				if(SJExtractTxtByMark(m_gPath+"\\Data\\"+Rtype+".htm",m_gPath+"\\Data\\temp.htm",MDB_PATTERN,DataCallBack_Extract)){
					SqlDelete(PathQuery(CLSMDB,IDDATA),"测试结果",g_RGroup.sRegSel);
					SqlAdd(PathQuery(CLSMDB,IDDATA),"测试结果",g_RGroup.sRegSel);
				}
				//set the corresponding Data Setting
				g_RGroup.sDataLst = SqlNames(PathQuery(CLSMDB,IDDATA),"*",g_RGroup.sRegSel);
		}
		scmd = (g_RGroup.sDataSel.IsEmpty())?TEXT("测试结果"):g_RGroup.sDataSel;//set the initial dataset
		
		if (g_RGroup.sDataLst.Find(scmd,0) < 0)
			scmd = g_RGroup.sDataLst.Left(g_RGroup.sDataLst.Find(',',0));
		
		if(g_RGroup.sJhSel1.IsEmpty())
			JhHandler("init","");//initial setting of jh
		
		op = _T("select");
	}
	
	if(op.Compare("add") == 0)
	{
		SqlSelect(PathQuery(CLSMDB,IDDATA),g_RGroup.sDataSel,g_RGroup.sRegSel);
		g_RGroup.sDataSel = SqlAdd(PathQuery(CLSMDB,IDDATA),scmd,g_RGroup.sRegSel);
		g_RGroup.sDataLst = SqlNames(PathQuery(CLSMDB,IDDATA),"*",g_RGroup.sRegSel);
	}
	if(op.Compare("delete") == 0)
	{
		g_RGroup.sDataSel = SqlDelete(PathQuery(CLSMDB,IDDATA),scmd,g_RGroup.sRegSel);
		g_RGroup.sDataLst = SqlNames(PathQuery(CLSMDB,IDDATA),"*",g_RGroup.sRegSel);
	}
	
	if(op.Compare("select") == 0)
	{
		g_RGroup.sDataSel = SqlSelect(PathQuery(CLSMDB,IDDATA),scmd,g_RGroup.sRegSel);
	}
	if(op.Compare("check") == 0)
	{
		SqlSelect(PathQuery(CLSMDB,IDDATA),g_RGroup.sDataSel,g_RGroup.sRegSel);
		SqlSelect(PathQuery(CLSMDB,IDJH),g_RGroup.sJhSel1,PathQuery(CLSGROUP,IDJH));
		this->DataIntegrateForCheck(g_msTemp,ZINDEX_MAX,true);
	}
	g_msTemp.SetAt("datalst",g_RGroup.sDataLst);
	g_msTemp.SetAt("datasz",g_RGroup.sDataSel);
	g_msTemp.SetAt("jhsz",g_RGroup.sJhSel1);
	g_msTemp.SetAt("curview","datahtm");
	g_msTemp.SetAt("curhtm","datahtm");
	if(brefresh)	
		SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDDATA),m_gPath+DATA_FORM,MDB_PATTERN,CallBack_Replace);
	return brefresh;
}

bool CHtmlHandler::JhHandler(CString op,CString scmd)
{
	bool brefresh=true;
	if(op.Compare("init") == 0)
	{
		//set the corresponding jhsetting
		g_RGroup.sJhLst1 = SqlNames(PathQuery(CLSMDB,IDJH),"*",PathQuery(CLSGROUP,IDJH));
		
		scmd = g_TesterConf.m_Xhgg;	//initial value
		
		if (scmd.IsEmpty()||(g_RGroup.sJhLst1.Find(scmd,0) < 0))
			scmd = g_RGroup.sJhLst1.Left(g_RGroup.sJhLst1.Find(',',0));
		
		op = _T("select");
	}
	if(op.Compare("add") == 0)
	{
		g_RGroup.sJhSel1 = SqlAdd(PathQuery(CLSMDB,IDJH),scmd,PathQuery(CLSGROUP,IDJH));
		g_RGroup.sJhLst1 = SqlNames(PathQuery(CLSMDB,IDJH),"*",PathQuery(CLSGROUP,IDJH));
	}
	if(op.Compare("delete") == 0)
	{
		g_RGroup.sJhSel1 = SqlDelete(PathQuery(CLSMDB,IDJH),scmd,PathQuery(CLSGROUP,IDJH));
		g_RGroup.sJhLst1 = SqlNames(PathQuery(CLSMDB,IDJH),"*",PathQuery(CLSGROUP,IDJH));
	}
	if(op.Compare("select") == 0)
	{
		g_RGroup.sJhSel1 = SqlSelect(PathQuery(CLSMDB,IDJH),scmd,PathQuery(CLSGROUP,IDJH));
	}
	g_msTemp.SetAt("jhlst",g_RGroup.sJhLst1);
	g_msTemp.SetAt("jhsz",g_RGroup.sJhSel1);
	g_msTemp.SetAt("curview","datahtm");
	g_msTemp.SetAt("curhtm","jhhtm");
	if(brefresh)	
		SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDJH),m_gPath+"\\Html\\jhform.js","@@*@@",CallBack_Replace);
	return brefresh;
}
bool CHtmlHandler::TestHandler(CString op,CString scmd)
{
	bool brefresh = true;
	if(op.Compare("init") == 0){
		//selection for g_TesterConf ,RTestType
		if(g_RGroup.sTestUrl.IsEmpty())
			Configure1Handler("init","");
	}
	g_msTemp.SetAt("testprglst",g_RGroup.sTestLst);
	g_msTemp.SetAt("testprgsz",g_TesterConf.m_RetestType);
	g_msTemp.SetAt("curview","testhtm");
	g_msTemp.SetAt("curhtm","testhtm");
	if(brefresh)
		SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDTEST),m_gPath+TEST_FORM,MDB_PATTERN,CallBack_Replace);
	return brefresh;
}
bool CHtmlHandler::Configure1Handler(CString op,CString scmd)
{
	bool brefresh=true;
	if(op.Compare("init") == 0)
	{
		//set the corresponding jhsetting
		g_RGroup.sConfLst1 = SqlNames(PathQuery(CLSMDB,IDCONF1),"*",PathQuery(CLSGROUP,IDCONF1));
		
		//determine the initial value
		scmd = g_RGroup.sConfSel1;
		if (scmd.IsEmpty()){
			Configure2Handler("init","");
			scmd = DEFAULT_NAME;
			//select the default corresponding value
			g_msTemp.RemoveAll();
			g_RGroup.sConfSel1 = SqlSelect(PathQuery(CLSMDB,IDCONF1),scmd,PathQuery(CLSGROUP,IDCONF1));
			//intial part of the g_TesterConf
			UpdateRstdSetting();
			g_TesterConf.m_RsDefault = g_TesterConf.m_Rs;
		}
		if(g_RGroup.sConfLst1.Find(scmd,0) < 0)
			scmd = DEFAULT_NAME;
		
		op = "select";
	}
	if(op.Compare("add") == 0)
	{
		if(scmd != DEFAULT_NAME ){
			g_RGroup.sConfSel1 = SqlAdd(PathQuery(CLSMDB,IDCONF1),scmd,PathQuery(CLSGROUP,IDCONF1));
			g_RGroup.sConfLst1 = SqlNames(PathQuery(CLSMDB,IDCONF1),"*",PathQuery(CLSGROUP,IDCONF1));
		}
	}
	if(op.Compare("delete") == 0)
	{
		if(scmd != DEFAULT_NAME ){
			g_RGroup.sConfSel1 = SqlDelete(PathQuery(CLSMDB,IDCONF1),scmd,PathQuery(CLSGROUP,IDCONF1));
			g_RGroup.sConfLst1 = SqlNames(PathQuery(CLSMDB,IDCONF1),"*",PathQuery(CLSGROUP,IDCONF1));
		}
	}
	if(op.Compare("select") == 0)
	{
		g_msTemp.RemoveAll();
		g_RGroup.sConfSel1 = SqlSelect(PathQuery(CLSMDB,IDCONF1),scmd,PathQuery(CLSGROUP,IDCONF1));
	}
	g_msTemp.SetAt("conf1lst",g_RGroup.sConfLst1);
	g_msTemp.SetAt("conf1sz",g_RGroup.sConfSel1);
	g_msTemp.SetAt("curview","testhtm");
	g_msTemp.SetAt("curhtm","confhtm");
	if(brefresh)
		SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDCONF1),m_gPath+CONF1_FORM,MDB_PATTERN,CallBack_Replace);
	return brefresh;
}

bool CHtmlHandler::Configure2Handler(CString op,CString scmd)
{
	bool brefresh=true;
	if(op.Compare("init") == 0)
	{
		//set the corresponding jhsetting
		g_RGroup.sConfLst2 = SqlNames(PathQuery(CLSMDB,IDCONF2),"*",PathQuery(CLSGROUP,IDCONF2));
		
		//initial value
		scmd = g_RGroup.sConfSel2;
		if (scmd.IsEmpty() ){
			scmd = DEFAULT_NAME;
			g_msTemp.RemoveAll();
			g_RGroup.sConfSel2 = SqlSelect(PathQuery(CLSMDB,IDCONF2),scmd,PathQuery(CLSGROUP,IDCONF2));

			UpdateRstdSetting();
		}
		if (g_RGroup.sConfLst2.Find(scmd,0) < 0)
			scmd = DEFAULT_NAME;
		op = _T("select");
	}
	if(op.Compare("add") == 0)
	{
		g_RGroup.sConfSel2 = SqlAdd(PathQuery(CLSMDB,IDCONF2),scmd,PathQuery(CLSGROUP,IDCONF2));
		g_RGroup.sConfLst2 = SqlNames(PathQuery(CLSMDB,IDCONF2),"*",PathQuery(CLSGROUP,IDCONF2));
	}
	if(op.Compare("delete") == 0)
	{
		g_RGroup.sConfSel2 = SqlDelete(PathQuery(CLSMDB,IDCONF2),scmd,PathQuery(CLSGROUP,IDCONF2));
		g_RGroup.sConfLst2 = SqlNames(PathQuery(CLSMDB,IDCONF2),"*",PathQuery(CLSGROUP,IDCONF2));
	}
	if(op.Compare("select") == 0)
	{
		g_msTemp.RemoveAll();
		g_RGroup.sConfSel2 = SqlSelect(PathQuery(CLSMDB,IDCONF2),scmd,PathQuery(CLSGROUP,IDCONF2));
	}
	g_msTemp.SetAt("conf2lst",g_RGroup.sConfLst2);
	g_msTemp.SetAt("conf2sz",g_RGroup.sConfSel2);
	g_msTemp.SetAt("curview","testhtm");
	g_msTemp.SetAt("curhtm","confhtm");
	if(brefresh)
		SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDCONF2),m_gPath+CONF2_FORM,MDB_PATTERN,CallBack_Replace);	
	return brefresh;
}

bool CHtmlHandler::TbrHandler(CString op,CString scmd)
{
	bool brefresh = false;
	if(op.Compare("init") == 0){
		if(g_RGroup.sTestLst.IsEmpty()){
			g_RGroup.sTestLst = SSearchDir(m_gPath+"\\Data","*.prg");
			g_RGroup.sTestLst.Replace("\n",",");
			g_TesterConf.m_RetestType = g_RGroup.sTestLst.Left(g_RGroup.sTestLst.Find(',',0));
		}
	}
	if(op.Compare("select") == 0){
		g_TesterConf.m_RetestType = scmd;
		TestHandler("init","");
		brefresh = true;
	}
	SJReplaceTxtByMark(m_gPath+PathQuery(CLSJS,IDTBR),m_gPath+TBR_FORM,MDB_PATTERN,CallBack_Replace);	
	return brefresh;
}
void CHtmlHandler::UpdateParmSetting()
{
	CString value;
	g_msTemp.Lookup("confdxcl",value);g_TesterConf.m_Ps.bSingle = (value.Compare("on") == 0);
	g_msTemp.Lookup("confcycs",value);g_TesterConf.m_Ps.iTimes = atoi(value);
	g_msTemp.Lookup("confcysj",value);g_TesterConf.m_Ps.iDelay = atoi(value);
	g_msTemp.Lookup("confwd",value);g_TesterConf.m_Ps.dTemper = atof(value);
	g_msTemp.Lookup("conf2lcsz",value);g_TesterConf.m_Ps.iRange = searchlclst(value);
	g_msTemp.Lookup("zdpselect",value);g_TesterConf.m_Ps.iLowest = searchpanlst(value);
	g_msTemp.Lookup("conf2sz",value);g_TesterConf.m_Ps.sTitle = value;
}
void CHtmlHandler::UpdateRstdSetting()
{
	CString value;
	for(char i='1';i < '8';i++){
		g_msTemp.Lookup(CString("cklt")+i,value); 
		g_TesterConf.m_Rs.bUse[i-'1'] = (value.Compare("on") == 0);
		g_msTemp.Lookup(CString("rstdlt")+i,value);
		g_TesterConf.m_Rs.dValue[i-'1'] = atof(value);
		g_msTemp.Lookup(CString("alphalt")+i,value);
		g_TesterConf.m_Rs.dAlpha[i-'1'] = atof(value);
		g_msTemp.Lookup(CString("betalt")+i,value);
		g_TesterConf.m_Rs.dAlpha[i-'1'] = atof(value);
	}
	g_msTemp.Lookup("conf1sz",value);
	g_TesterConf.m_Rs.sTitle = value;
}
void CHtmlHandler::ModifyDefaultRstdSetting(RSTDTYPE ps)
{
	
}
void CHtmlHandler::UpdateScheme()
{
	CString value;
	g_msTemp.Lookup("schemesz",value);
	g_TesterConf.m_RetestType = value;
}

void CHtmlHandler::CallBack_Extract(LPCTSTR smark, LPCTSTR sdata)
{
	g_msTemp.SetAt(smark,sdata);
}
//the only difference between CallBack is to remove the mark '@' in string
void CHtmlHandler::DataCallBack_Extract(LPCTSTR smark, LPCTSTR sdata)
{
	CString temp=smark;
	temp.Remove('@');
	int pos=temp.ReverseFind('.');
	if(pos > 0)
		temp = temp.Left(pos);
	g_msTemp.SetAt(temp,sdata);
}
LPCTSTR CHtmlHandler::CallBack_Replace(LPCTSTR smark)
{
	CString data;
	CString temp=smark;
	temp.Remove('@');
	return g_msTemp.Lookup(temp,data)?data:"";
}























