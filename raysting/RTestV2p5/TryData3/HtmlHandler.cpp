#include "stdafx.h"
#include "TryData4.h"
#include "TryData4Dlg.h"
#include ".\trydata4dlg.h"

extern CString g_Path;
extern CTesterData g_Data;
extern TESTERCONF g_TesterConf;

#define ZINDEX_MAX	400
#define MDB_PATTERN	"@@*@@"
#define DEFAULT_NAME "Default"		//Default Scheme and Conf name

CMapStringToString g_msTemp;
extern  void DataIntegrateForCheck(CMapStringToString &mstr, int indexmax, bool colormark);                                             
extern  void PrepareArrayForReport(CMapStringToString &mstr);                                                                           
extern  const char* PathQuery(int pathid,int clsid);										
extern  void  DataIntegrateForCompensate(CMapStringToString& mstr,int indexmax);     

#define PathQuery2 PathQuery

void CTryData4Dlg::Handler(UINT uPage,CString op)
{
	//restore the reportform.htm for performance consideration
	g_Result.SetAt("reporturl","about::blank");
	if(uPage  == INDEX)		brefresh= RegHandler(op);
	/*
	if(uPage  == DATA)		brefresh= DataHandler(op);
	if(uPage  == JIAOHE)		brefresh= JhHandler(op);
	if(uPage  == REPORT)		brefresh= ReportHandler(op);
	if(uPage  == TEST)		brefresh= TestHandler(op);
	if(uPage  == CONFIGURE)			brefresh= ConfigureHandler(op);
	*/
	return brefresh;
}
bool CTryData4Dlg::RegHandler(CString op)
{
	if(op.Compare("init") == 0)
	{
		m_RGroup.sRegLst = SqlNames(PathQuery2(CLSMDB,IDREG),"*",PathQuery2(CLSGROUP,IDREG));
		if(	m_RGroup.sRegSel.IsEmpty() || (m_RGroup.sRegLst.Find(m_RGroup.sRegSel,0) < 0))
			m_RGroup.sRegSel = m_RGroup.sRegLst.Left(m_RGroup.sRegLst.Find(',',0));
		op = _T("select");
	}
	if(op.Compare("select") == 0)
	{
		m_RGroup.sRegSel = SqlSelect(PathQuery2(CLSMDB,IDREG),m_RGroup.sRegSel,PathQuery2(CLSGROUP,IDREG));
	}
	if(op.Compare("add") == 0)
	{
		m_RGroup.sRegSel = SqlAdd(PathQuery2(CLSMDB,IDREG),m_RGroup.sRegSel,PathQuery2(CLSGROUP,IDREG));
		m_RGroup.sRegLst = SqlNames(PathQuery2(CLSMDB,IDREG),"*",PathQuery2(CLSGROUP,IDREG));
	}
	if(op.Compare("delete") == 0)
	{
		m_RGroup.sRegSel = SqlDelete(PathQuery2(CLSMDB,IDREG),m_RGroup.sRegSel,PathQuery2(CLSGROUP,IDREG));
		m_RGroup.sRegLst = SqlNames(PathQuery2(CLSMDB,IDREG),"*",PathQuery2(CLSGROUP,IDREG));
	}
	//update the g_TestType
	g_msTemp.Lookup("xhgg",g_TesterConf.m_Xhgg);
	g_msTemp.SetAt("reglst",m_RGroup.sRegLst);
	g_msTemp.SetAt("regsz",m_RGroup.sRegSel);

	return brefresh;
}
/*
bool CTryData4Dlg::ReportHandler(CString op,CString scmd)
{
	bool brefresh=false;
	
	if(op.Compare("init") == 0)
	{
		//set the corresponding report-setting
		if(m_RGroup.sReportLst.IsEmpty())
		{
			CString tmp = PathQuery2(CLSJS,IDTYPE);
			m_RGroup.sReportLst = SSearchDir(g_Path+"\\Doc",tmp+CString("*.doc"));
			m_RGroup.sReportLst.Replace("\n",",");
		}
		//m_RGroup.sReportSel.Format("%s",PathQuery2(CLSJS,IDTYPE));
		if(	m_RGroup.sReportSel.IsEmpty() || (m_RGroup.sReportLst.Find(m_RGroup.sReportSel,0) < 0))
			scmd = m_RGroup.sReportLst.Left(m_RGroup.sReportLst.Find(',',0));
		else
			scmd = m_RGroup.sReportSel;//use the value of last time
		op = _T("select");
	}

	m_RGroup.sReportUrl = g_Path+"\\tmp"+scmd+".doc";
	if(op.Compare("print") == 0)
	{
		ShellExecute(0,"print",m_RGroup.sReportUrl,"","",SW_HIDE);
	}
	if(op.Compare("savenew") == 0)
	{
		CString tfile;
		char DirUse[300];
		::GetCurrentDirectory(300,DirUse);
		
		CFileDialog cfd(FALSE,_T("doc"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Doc File(*.doc)|*.doc||"),NULL);
		if(cfd.DoModal() == IDOK){
			tfile=cfd.GetPathName();
			CopyFile(m_RGroup.sReportUrl,tfile,TRUE);
		}
		::SetCurrentDirectory(DirUse);
	}
	if(op.Compare("select") == 0)
	{
		m_RGroup.sReportSel = scmd;
		SqlSelect(PathQuery2(CLSMDB,IDREG),m_RGroup.sRegSel,"*");	//append the reg info
		SqlSelect(PathQuery2(CLSMDB,IDDATA),m_RGroup.sDataSel,m_RGroup.sRegSel);//append the data info
		SqlSelect(PathQuery2(CLSMDB,IDJH),m_RGroup.sJhSel1,PathQuery2(CLSGROUP,IDJH));//append the jh info
		g_msTemp.SetAt("jdjg","");
		g_msTemp.SetAt("mvalues","");
		DataIntegrateForCheck(g_msTemp,ZINDEX_MAX,false);

		PrepareArrayForReport(g_msTemp);
		DeleteFile(m_RGroup.sReportUrl);

		if(CopyFile(g_Path+"\\Doc\\"+m_RGroup.sReportSel+".doc",m_RGroup.sReportUrl,FALSE))
		{
			SJReplaceWordByBookmark(m_RGroup.sReportUrl,g_msTemp);
			brefresh = true;
		}
		else
			::AfxMessageBox("请关闭报告结果后重试");
	}
	
	CString stmp = m_RGroup.sReportUrl;
	stmp.Replace("\\","\\\\");
	g_msTemp.SetAt("reporturl",stmp);
	
	g_msTemp.SetAt("reportlst",m_RGroup.sReportLst);
	g_msTemp.SetAt("reportsz",m_RGroup.sReportSel);
	g_msTemp.SetAt("curview","reporthtm");
	g_msTemp.SetAt("curhtm","reporthtm");
	if(brefresh)			
	{
		CStdioFile jsfile;
		if(jsfile.Open(g_Path+REPORT_FORM,CFile::modeCreate|CFile::modeWrite))
			ReplaceReport(jsfile,g_msTemp,IDR_REPORTJS);
		jsfile.Close();
	}

	return brefresh;
}
bool CTryData4Dlg::DataHandler(CString op,CString scmd)
{
	bool brefresh=true;
	BOOL bCheck = FALSE;
	if (op.Compare("init") == 0)
	{
//		if((scmd.Compare("测试结果") == 0))
//		{
			m_RGroup.sDataLst.Empty();
			m_RGroup.sDataSel.Empty();
//		}

		if(m_RGroup.sDataLst.IsEmpty()||scmd.IsEmpty())
		{
//			g_Tester->End(CTesterProgram::State);
			//try to import the test result first
			CString Rtype = PathQuery2(CLSGROUP,IDTYPE);//find the result type
			SqlDelete(PathQuery2(CLSMDB,IDDATA),"测试结果",m_RGroup.sRegSel);
			CopySTSMap(g_Result,g_msTemp);
			SqlAdd(PathQuery2(CLSMDB,IDDATA),"测试结果",m_RGroup.sRegSel);
			//set the corresponding Data Setting
			m_RGroup.sDataLst = SqlNames(PathQuery2(CLSMDB,IDDATA),"*",m_RGroup.sRegSel);
		}
		scmd = (m_RGroup.sDataSel.IsEmpty())?TEXT("测试结果"):m_RGroup.sDataSel;//set the initial dataset
		
		if (m_RGroup.sDataLst.Find(scmd,0) < 0)
			scmd = m_RGroup.sDataLst.Left(m_RGroup.sDataLst.Find(',',0));
		
		if(m_RGroup.sJhSel1.IsEmpty())
			JhHandler("init","");//initial setting of jh
		
		op = _T("select");
	}
	
	if(op.Compare("add") == 0)
	{
		SqlSelect(PathQuery2(CLSMDB,IDDATA),m_RGroup.sDataSel,m_RGroup.sRegSel);
		m_RGroup.sDataSel = SqlAdd(PathQuery2(CLSMDB,IDDATA),scmd,m_RGroup.sRegSel);
		m_RGroup.sDataLst = SqlNames(PathQuery2(CLSMDB,IDDATA),"*",m_RGroup.sRegSel);
	}
	if(op.Compare("delete") == 0)
	{
		m_RGroup.sDataSel = SqlDelete(PathQuery2(CLSMDB,IDDATA),scmd,m_RGroup.sRegSel);
		m_RGroup.sDataLst = SqlNames(PathQuery2(CLSMDB,IDDATA),"*",m_RGroup.sRegSel);
	}
	
	if(op.Compare("select") == 0)
	{
		m_RGroup.sDataSel = SqlSelect(PathQuery2(CLSMDB,IDDATA),scmd,m_RGroup.sRegSel);
	}
	if(op.Compare("check") == 0)
	{
		bCheck = TRUE;
		SqlSelect(PathQuery2(CLSMDB,IDDATA),m_RGroup.sDataSel,m_RGroup.sRegSel);
		SqlSelect(PathQuery2(CLSMDB,IDJH),m_RGroup.sJhSel1,PathQuery2(CLSGROUP,IDJH));
		g_msTemp.SetAt("jdjg","");
		g_msTemp.SetAt("mvalues","");
		DataIntegrateForCheck(g_msTemp,ZINDEX_MAX,true);
	}
	if(op.Compare("compensate") == 0)
	{
		bCheck = TRUE;
		SqlSelect(PathQuery2(CLSMDB,IDDATA),m_RGroup.sDataSel,m_RGroup.sRegSel);
		SqlSelect(PathQuery2(CLSMDB,IDJH),m_RGroup.sJhSel1,PathQuery2(CLSGROUP,IDJH));
		g_msTemp.SetAt("jdjg","");
		g_msTemp.SetAt("mvalues","");
		DataIntegrateForCompensate(g_msTemp,ZINDEX_MAX);
	}
	if(op.Compare("modnbz") == 0)
	{
		CString sname = m_RGroup.sDataSel;
		SqlDelete(PathQuery2(CLSMDB,IDCONF),DEFAULT_NAME,PathQuery2(CLSGROUP,IDCONF));
		m_RGroup.sDataSel = SqlSelect(PathQuery2(CLSMDB,IDDATA),sname,m_RGroup.sRegSel);
		ModifyDefaultRstdSetting();
//		ModifyDefaultParmSetting();
		SqlAdd(PathQuery2(CLSMDB,IDCONF),DEFAULT_NAME,PathQuery2(CLSGROUP,IDCONF));
	}
	if(op.Compare("save") == 0)
	{
		CString tfile,result;
		char DirUse[300];
		::GetCurrentDirectory(300,DirUse);
		
		CFileDialog cfd(FALSE,_T("htm"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Html File(*.htm)|*.htm||"),NULL);
		if(cfd.DoModal() == IDOK){
			CStdioFile jsfile,htmfile;
			tfile=cfd.GetPathName();
			if(jsfile.Open(tfile,CFile::modeCreate|CFile::modeWrite)){
				if(htmfile.Open(g_Path+PathQuery2(CLSJS,IDTEST),CFile::modeRead)){
					SqlSelect(PathQuery2(CLSMDB,IDDATA),m_RGroup.sDataSel,m_RGroup.sRegSel);
					if(ReplaceReport(jsfile,g_msTemp,htmfile,FALSE,FALSE,FALSE))
						result = CString("原始数据已保存成功,是否需要立即打印原始数据报告?");
				}
				jsfile.Close();
			}
		}
		brefresh = false;
		if(result.IsEmpty())
			::AfxMessageBox("原始数据保存失败");
		else
		{
			if(::AfxMessageBox(result,MB_YESNO) == IDYES)
			{
				ShellExecute(0,"print",cfd.GetPathName(),"","",SW_HIDE);
			}
		}
		::SetCurrentDirectory(DirUse);
	}
	SqlSelect(PathQuery2(CLSMDB,IDREG),m_RGroup.sRegSel,"*");	//append the reg info
	g_msTemp.SetAt("datalst",m_RGroup.sDataLst);
	g_msTemp.SetAt("datasz",m_RGroup.sDataSel);
	g_msTemp.SetAt("jhsz",m_RGroup.sJhSel1);
	g_msTemp.SetAt("curview","datahtm");
	g_msTemp.SetAt("curhtm","datahtm");
	if(brefresh)	
	{
		CStdioFile jsfile,htmfile;
		if(jsfile.Open(g_Path+DATA_FORM,CFile::modeCreate|CFile::modeWrite)){
			ReplaceReport(jsfile,g_msTemp,IDR_DATAHEAD);
			if(htmfile.Open(g_Path+PathQuery2(CLSJS,IDTEST),CFile::modeRead)){
				ReplaceReport(jsfile,g_msTemp,htmfile,FALSE,bCheck);
			}
			ReplaceReport(jsfile,g_msTemp,IDR_DATAFOOT);
			jsfile.Close();
		}
	}
	return brefresh;
}


void CTryData4Dlg::UpdateRstdSetting()
{
	CString value,title;
	for(char i='1';i <= '9';i++){
		g_msTemp.Lookup(CString("cklt")+i,value); 
		g_TesterConf.m_Rs.bUse[i-'1'] = (value.Compare("on") == 0);
		g_msTemp.Lookup(CString("rstdlt")+i,value);
		g_TesterConf.m_Rs.dValue[i-'1'] = atof(value);
		g_msTemp.Lookup(CString("alphalt")+i,value);
		g_TesterConf.m_Rs.dAlpha[i-'1'] = atof(value);
		g_msTemp.Lookup(CString("betalt")+i,value);
		g_TesterConf.m_Rs.dBeta[i-'1'] = atof(value);
	}
}*/
bool CTryData4Dlg::HandleDataShow(CString line)
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
/*
void CTryData4Dlg::ModifyDefaultRstdSetting()
{
	CString sKey,sValue;
	for(char i='1';i <= '9';i++){
		sKey.Format("Z1%d",i-'0');
		if(!g_msTemp.Lookup(sKey,sValue)||(sValue.Find('.') <= 0))
			sValue.Format("%f",g_TesterConf.m_RsDefault.dValue[i-'0']);

		sKey.Format("rstdlt%d",i+1-'0');	g_msTemp.SetAt(sKey,sValue);

		sValue.Format("%f",g_TesterConf.m_RsDefault.dAlpha[i-'0']);
		sKey.Format("alphalt%d",i+1-'0');	g_msTemp.SetAt(sKey,sValue);

		sValue.Format("%f",g_TesterConf.m_RsDefault.dBeta[i-'0']);
		sKey.Format("betalt%d",i+1-'0');	g_msTemp.SetAt(sKey,sValue);

		sValue = g_TesterConf.m_RsDefault.bUse[i-'0'] ? "on":"";
		sKey.Format("cklt%d",i+1-'0');	g_msTemp.SetAt(sKey,sValue);
	}
}

void CTryData4Dlg::ModifyDefaultParmSetting(void)
{
	CString value;
	value = g_TesterConf.m_Ps.bSingle ? "on":"";
	g_msTemp.SetAt("confdxcl",value);
	value.Format("%i",g_TesterConf.m_Ps.iTimes);
	g_msTemp.SetAt("confcycs",value);
	value.Format("%i",g_TesterConf.m_Ps.iDelay);
	g_msTemp.SetAt("confcysj",value);
	value.Format("%f",g_TesterConf.m_Ps.dTemper);
	g_msTemp.SetAt("confwd",value);
	g_msTemp.SetAt("lcselect",CTesterCaculate::retrievelclst(g_TesterConf.m_Ps.iRange));
	g_msTemp.SetAt("zdpselect",CTesterCaculate::retrievepanlst(g_TesterConf.m_Ps.iLowest));
	g_msTemp.SetAt("conf2sz",g_TesterConf.m_Ps.sTitle);
}
*/
