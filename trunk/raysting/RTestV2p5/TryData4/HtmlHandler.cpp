#include "stdafx.h"
#include "TryData4.h"
#include "trydata4dlg.h"
#include "TesterData.h"
#include "TryDataSql.h"
#include "TesterCaculate.h"
#include "InputDlg.h"
#include "..\sjInclude\Report\Include\Replace.h"
#include "WaitDialog.h"


extern CString g_Path;
extern CTesterData g_Data;
TESTERCONF g_TesterConf;
#define ZINDEX_MAX	400
#define MDB_PATTERN	"@@*@@"


CMapStringToString g_msTemp;
extern  void DataIntegrateForCheck(CMapStringToString &mstr, int indexmax, bool colormark);                                             
extern  void PrepareArrayForReport(CMapStringToString &mstr);                                                                           
extern  const char* PathQuery(int pathid,int clsid);										
extern  void  DataIntegrateForCompensate(CMapStringToString& mstr,int indexmax);     
extern  bool CheckPassFail(CMapStringToString& mstr,int position);
extern  CString SSearchDir(LPCTSTR rpath, LPCTSTR filetype);
CString g_idvals;
CString CTryData4Dlg::PathQuery2(int pathid,int clsid)
{
	if(pathid == CLSMDB){
		if(clsid == IDREG) g_idvals = m_sRegvals;
		if(clsid == IDJH) g_idvals = m_sJhvals;
		if(clsid == IDDATA) g_idvals = m_sDatavals;
		if(clsid == IDCONF) g_idvals = m_sConfigvals;
	}
	return CString(PathQuery(pathid,clsid));
}

//collect all the values on current page
void CTryData4Dlg::CollectValues()
{
		
		CString key,value,type;
		CString idvals = g_idvals;
		IHTMLElement* newElement;
		while(!idvals.IsEmpty() && idvals.Find(',')>= 0)
		{
			key = idvals.Left(idvals.Find(','));
			idvals = idvals.Right(idvals.GetLength()-(key.GetLength()+1));
			
			if(key.IsEmpty()) continue;

			if(S_OK != GetElement(key,&newElement))
				continue;
			type = GetAttribute(newElement,m_elemTypes[5]);
			if(type.Find("EDIT") == 0)
			{
				value = GetAttribute(newElement,m_elemTypes[1]);
				g_msTemp.SetAt(key,value);
			}
			if(type.Find("CMD") == 0)
			{
				value = GetAttribute(newElement,m_elemTypes[4]);
				g_msTemp.SetAt(key,value);
			}
			if(type.Find("ONCE") == 0)
			{
				value = GetAttribute(newElement,m_elemTypes[4]);
				g_msTemp.SetAt(key,value);
			}
			if(type.Find("CACU") == 0)
			{
				value = GetAttribute(newElement,m_elemTypes[4]);
				g_msTemp.SetAt(key,value);
			}
		}
}
CString CTryData4Dlg::GenerateOptionHtml(CString list,CString def)
{
	//generate the inner html based on list (seperated by ,)
	CString selelst("");
	CString perop=list;
	perop.Replace(", ,",",");
	while(perop.Find(",") > 0)
	{
		if(def.Compare(perop.Left(perop.Find(","))) == 0)
			selelst += _T("<option selected value=")+perop.Left(perop.Find(","))+_T(">")+perop.Left(perop.Find(","))+_T("</option>");
		else
			selelst += _T("<option value=")+perop.Left(perop.Find(","))+_T(">")+perop.Left(perop.Find(","))+_T("</option>");
		perop = perop.Right(perop.GetLength() - perop.Find(",") -1);
	}
	return selelst;
}
void CTryData4Dlg::ConvertNYR()
{
		  //data convert      
	  //convert jdrq to jdrqn,y,r and yxqz to yxqzn,y,r
	  CString sdate;
	  int pos1,pos2;
	  g_msTemp.Lookup("jdrq",sdate);
	  pos1 = sdate.Find("-")+1;
	  pos2 = sdate.Find("-",pos1);
	  g_msTemp.SetAt("jdrqn",sdate.Left(pos1-1));
	  g_msTemp.SetAt("jdrqy",sdate.Mid(pos1,pos2-pos1));
	  g_msTemp.SetAt("jdrqr",sdate.Right(sdate.GetLength()-(pos2+1)));
		
	  g_msTemp.Lookup("yxqz",sdate);
	  pos1 = sdate.Find("-")+1;
	  pos2 = sdate.Find("-",pos1);
	  g_msTemp.SetAt("yxqzn",sdate.Left(pos1-1));
	  g_msTemp.SetAt("yxqzy",sdate.Mid(pos1,pos2-pos1));
	  g_msTemp.SetAt("yxqzr",sdate.Right(sdate.GetLength()-(pos2+1)));

}
void CTryData4Dlg::ReportHandler(CString op)
{
	if(op.Compare("init") == 0)
    {
      //set the corresponding report-setting
      if(m_sReportLst.IsEmpty())
      {
         CString tmp = PathQuery2(CLSJS,IDTYPE);
		 CString doclst,xlslst;
         doclst = SSearchDir(g_Path+"\\Doc",tmp+CString("*.doc"));
		 xlslst = SSearchDir(g_Path+"\\Doc",tmp+CString("*.xls"));
		 m_sReportLst = doclst + CString("\n") + xlslst;
		 m_sReportLst.Replace("\n\n","\n");
         m_sReportLst.Replace("\n",",");
      }
	  if(m_sReportSel.IsEmpty())
		 m_sReportSel = m_sReportLst.Left(m_sReportLst.Find(',',0));
	  m_explorer.ShowWindow(SW_NORMAL);
	  op = CString("select");
	}
	CString ftype("doc");
	CString ftypel("Doc File(*.doc)|*.doc||");
	m_sReportUrl = g_Path+"\\Data\\tmp"+m_sReportSel+".doc";
	if(m_sReportSel.Find("(xls)") > 0)
	{
		ftype = _T("xls");
		ftypel = _T("Excel File(*.xls)|*.xls||");
		m_sReportUrl = g_Path+"\\Data\\tmp"+m_sReportSel+".xls";
	}
    if(op.Compare("print") == 0)
	{
      ShellExecute(0,"print",m_sReportUrl,"","",SW_NORMAL);
	}
    if(op.Compare("saveas") == 0)
    {
      CString tfile;
      char DirUse[300];
      ::GetCurrentDirectory(300,DirUse);
      
      CFileDialog cfd(FALSE,ftype,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,ftypel,NULL);
      if(cfd.DoModal() == IDOK){
         tfile=cfd.GetPathName();
         if(CopyFile(m_sReportUrl,tfile,TRUE))
			 ::AfxMessageBox("报告已保存到:"+tfile);
		 else
			 ::AfxMessageBox("保存报告失败");
      }
      ::SetCurrentDirectory(DirUse);
   }
   if(op.Compare("openall") == 0)
   {
	   //create folder and generate all the data
	   CString curr_ReportUrl;
	   CString full_ReportUrl;
	   CString full_ReportPath;
			SYSTEMTIME syt;
			CString s_bianhao;




	   CString perop=m_sReportLst;
	   perop.Replace(", ,",",");

	   SqlSelect(PathQuery2(CLSMDB,IDREG),m_sRegSel,"*");   //append the reg info
	   SqlSelect(PathQuery2(CLSMDB,IDDATA),m_sDataSel,m_sRegSel);//append the data info
	   SqlSelect(PathQuery2(CLSMDB,IDJH),m_sJhSel,PathQuery2(CLSGROUP,IDJH));//append the jh info
	   ConvertNYR();
	   //prepare data for report
	   g_msTemp.SetAt("jdjg","");
	   g_msTemp.SetAt("mvalues","");
	   DataIntegrateForCheck(g_msTemp,ZINDEX_MAX,false);
	   PrepareArrayForReport(g_msTemp);

		g_msTemp.Lookup("zsbh",s_bianhao);
			::GetLocalTime(&syt);
			full_ReportPath.Format("%s(%i%02i%02i_%02i%02i%02i)",s_bianhao,syt.wYear,syt.wMonth,syt.wDay,syt.wHour,syt.wMinute,syt.wSecond);
			full_ReportPath.Remove('\\');
			full_ReportPath.Remove('/');
			full_ReportPath.Replace('.','_');
			full_ReportPath.Replace('*','_');
			full_ReportPath.Replace(':','_');
			full_ReportPath.Replace('"','_');
			full_ReportPath.Replace('<','(');
			full_ReportPath.Replace('>','(');
			full_ReportPath.Replace('|','_');


//get target folder to save result.
CString m_Dir;  
BROWSEINFO bi;  
bi.hwndOwner = this->m_hWnd;  
bi.pidlRoot = NULL;  
bi.pszDisplayName = m_Dir.GetBuffer(512);  
bi.lpszTitle = _T("选择保存的文件夹");  
bi.ulFlags = BIF_STATUSTEXT  |BIF_RETURNONLYFSDIRS | BIF_BROWSEINCLUDEURLS | BIF_USENEWUI;  
bi.lpfn = NULL;  
bi.lParam = 0;  
bi.iImage = 0;  
ITEMIDLIST* pidl = ::SHBrowseForFolder(&bi);  
  
if (::SHGetPathFromIDList(pidl, m_Dir.GetBuffer(512)))  
{
		   m_Dir.ReleaseBuffer();  

			full_ReportPath = m_Dir;

   
			SECURITY_ATTRIBUTES attribute;
			attribute.nLength = sizeof(attribute);
			attribute.lpSecurityDescriptor = NULL;
			attribute.bInheritHandle = FALSE;
			

			if(0 == ::CreateDirectory(full_ReportPath + "\\"+s_bianhao,&attribute))
			{
				AfxMessageBox("创建目录失败:"+full_ReportPath + "\\"+s_bianhao);
				return;
			}
   
	   		CWaitDialog *pDlg = new CWaitDialog(this);
			pDlg->Create(IDD_DIALOG3,this);
			pDlg->ShowWindow(SW_NORMAL);
			pDlg->ShowOkButton(false);
	   while(perop.Find(",") > 0)
	   {
		    curr_ReportUrl = perop.Left(perop.Find(","));
			if(curr_ReportUrl.Find("(xls)") > 0)
				ftype = "xls";
			else
				ftype = "doc";

			//generate full report url
			full_ReportUrl = full_ReportPath + "\\"+s_bianhao+"\\"+s_bianhao+curr_ReportUrl+"."+ftype;
			
			DeleteFile(full_ReportUrl);
			if(CopyFile(g_Path+"\\Doc\\"+curr_ReportUrl+"."+ftype,full_ReportUrl,FALSE))
			{
				pDlg->SetWaitingText(CString("正在导出数据到:")+full_ReportUrl);		
				if(ftype == CString("doc"))
					SJReplaceWordByBookmark(full_ReportUrl,g_msTemp);
				else
					SJReplaceExcelByMark(full_ReportUrl,g_msTemp);
			}else{
				::AfxMessageBox("请关闭报告结果后重试");
				break;
			}
			perop = perop.Right(perop.GetLength() - perop.Find(",") -1);
	   }
	   pDlg->bQuit = true;
	   pDlg->DestroyWindow();
	   delete pDlg;
	  if(perop.Find(",") < 0)
			ShellExecute(0,"open",full_ReportPath,"","",SW_NORMAL);
}else{
	m_Dir.ReleaseBuffer();  
}
	  //generate report list
   	  CString schsel = _T("<select id=schsel>")+GenerateOptionHtml(m_sReportLst,m_sReportSel)+_T("</select>");
	  this->SetElementHtml("schemelist",schsel.AllocSysString());

	  POSITION pos;
	  CString value,key;
	  IHTMLElement* newElement;
	  for( pos = g_msTemp.GetStartPosition(); pos != NULL; )
	  {
		g_msTemp.GetNextAssoc( pos, key, value );
		if(S_OK != GetElement(key,&newElement))
				continue;
		CString sel = this->GetAttribute(newElement,m_elemTypes[5]); //class
		if((key.Find("select") > 0) || (key.Find("jhsel") > 0)){
			VARIANT vl;
			
			BSTR bstr = value.AllocSysString();
			vl.vt = VT_BSTR;
			vl.bstrVal = bstr;
			newElement->setAttribute(m_elemTypes[3],vl);
		}else
		{
			if(sel.Find("HIDE") != 0)//display all none hidden data
			{
				BSTR bstr = value.AllocSysString();
				this->SetElementText(key,bstr);
			}
		}
      }
	  op = m_sReportUrl;
	  m_explorer.Invalidate();
//	  m_explorer.Navigate(op,NULL,NULL,NULL,NULL);
   }
   if(op.Compare("select") == 0)
   {
	  
      SqlSelect(PathQuery2(CLSMDB,IDREG),m_sRegSel,"*");   //append the reg info
      SqlSelect(PathQuery2(CLSMDB,IDDATA),m_sDataSel,m_sRegSel);//append the data info
      SqlSelect(PathQuery2(CLSMDB,IDJH),m_sJhSel,PathQuery2(CLSGROUP,IDJH));//append the jh info

	  ConvertNYR();
	  //prepare data for report
	  g_msTemp.SetAt("jdjg","");
      g_msTemp.SetAt("mvalues","");
      DataIntegrateForCheck(g_msTemp,ZINDEX_MAX,false);
      PrepareArrayForReport(g_msTemp);
	  m_explorer.Navigate("about:blank",NULL,NULL,NULL,NULL);
	  Sleep(2000);
	  DeleteFile(m_sReportUrl);
	  if(CopyFile(g_Path+"\\Doc\\"+m_sReportSel+"."+ftype,m_sReportUrl,FALSE))
	  {
		  if(ftype == CString("doc"))
			  SJReplaceWordByBookmark(m_sReportUrl,g_msTemp);
		  else
			  SJReplaceExcelByMark(m_sReportUrl,g_msTemp);
	  }else
	  {
	     ::AfxMessageBox("请关闭报告结果后重试");
		return;
	  }
	  //generate report list
   	  CString schsel = _T("<select id=schsel>")+GenerateOptionHtml(m_sReportLst,m_sReportSel)+_T("</select>");
	  this->SetElementHtml("schemelist",schsel.AllocSysString());

	  POSITION pos;
	  CString value,key;
	  IHTMLElement* newElement;
	  for( pos = g_msTemp.GetStartPosition(); pos != NULL; )
	  {
		g_msTemp.GetNextAssoc( pos, key, value );
		if(S_OK != GetElement(key,&newElement))
				continue;
		CString sel = this->GetAttribute(newElement,m_elemTypes[5]); //class
		if((key.Find("select") > 0) || (key.Find("jhsel") > 0)){
			VARIANT vl;
			
			BSTR bstr = value.AllocSysString();
			vl.vt = VT_BSTR;
			vl.bstrVal = bstr;
			newElement->setAttribute(m_elemTypes[3],vl);
		}else
		{
			if(sel.Find("HIDE") != 0)//display all none hidden data
			{
				BSTR bstr = value.AllocSysString();
				this->SetElementText(key,bstr);
			}
		}
      }
	  op = m_sReportUrl;
	  m_explorer.Navigate(op,NULL,NULL,NULL,NULL);
   }


}
void CTryData4Dlg::Handler(CString op,CString &sRegSel,CString &sRegLst,bool silent)
{
	UINT pageid;
	CString group;
	bool bFirst = ((m_currPage == DATA) && sRegLst.IsEmpty()); //enter the function for the first time

	switch(m_currPage)
	{
	case INDEX:		pageid = IDREG;break;
	case CONFIG:	pageid = IDCONF;break;
	case JIAOHE:	pageid = IDJH;break;
	case DATA:		pageid = IDDATA;break;
	case REPORT:	ReportHandler(op);return;
	default:		return;
	}
	group = PathQuery2(CLSMDB,pageid); //update the g_idvals
	group = PathQuery2(CLSGROUP,pageid);
	if(pageid == IDDATA)
		group=m_sRegSel;
	
	
	if(op.Compare("init") == 0)
	{
		if((pageid != IDREG) || (m_sFltval.IsEmpty())){
			m_explorer.Navigate("about:blank",NULL,NULL,NULL,NULL);
			Sleep(1000);
			m_explorer.ShowWindow(SW_HIDE);
			sRegLst = SqlNames(PathQuery2(CLSMDB,pageid),"*",group);
		}
		else
		{
			sRegLst = SqlNames(PathQuery2(CLSMDB,pageid),"*",group+"' AND [position]='"+m_sFltid+"' AND [value] LIKE '"+m_sFltval) + ",";
		}
		
		if(	sRegSel.IsEmpty() || (sRegLst.Find(sRegSel,0) < 0))
			sRegSel = sRegLst.Left(sRegLst.Find(',',0));
		op = "select";
	}

	if(op.Compare("select") == 0)
	{
		sRegSel = SqlSelect(PathQuery2(CLSMDB,pageid),sRegSel,group);
			
	}
	if(op.Compare("add") == 0)
	{
		CInputDlg dlg;
		dlg.m_Message.Format("请将新设置命名为:");
		if(pageid == IDDATA)  dlg.m_Message.Format("将当前测量结果另存为:");
		while(1)
		{
			if(IDOK == dlg.DoModal())
			{
				int ipos = m_sRegLst.Find(dlg.m_Value+_T(","));
				if((ipos > 0)&&(m_sRegLst[ipos-1] != ','))
				{
					//MessageBox(_T("名字")+dlg.m_Value+_T("已被使用"),"电阻校验",MB_OK);
					//continue;
					sRegSel = dlg.m_Value;
					SqlDelete(PathQuery2(CLSMDB,pageid),sRegSel,group);
				}
				break;
			}else
				return;
		}
		sRegSel = dlg.m_Value;
		CollectValues();
		UpdateData(TRUE);
		UpdateConfSetting(FALSE);
		sRegSel = SqlAdd(PathQuery2(CLSMDB,pageid),sRegSel,group);
		sRegLst = SqlNames(PathQuery2(CLSMDB,pageid),"*",group);
	}
	if(op.Compare("delete") == 0)
	{
		if(sRegLst.Find(',') == (sRegLst.GetLength()-1))  //can't delete the last setting
		{
			::AfxMessageBox("这是最后一个设置，不能删除");
			return;
		}

		if(IDYES != MessageBox(_T("你是否确定删除\"")+sRegSel+_T("\""),"电阻校验",MB_YESNO|MB_ICONQUESTION))
			return;

		sRegSel = SqlDelete(PathQuery2(CLSMDB,pageid),sRegSel,group);
		sRegLst = SqlNames(PathQuery2(CLSMDB,pageid),"*",group);
	}
	if(op.Compare("update") == 0)
	{
		CollectValues();
		UpdateData(TRUE);
		UpdateConfSetting(FALSE);
		SqlDeleteOnly(PathQuery2(CLSMDB,pageid),sRegSel,group);
		sRegSel = SqlAdd(PathQuery2(CLSMDB,pageid),sRegSel,group);
		sRegLst = SqlNames(PathQuery2(CLSMDB,pageid),"*",group);
	}
	UpdateConfSetting(TRUE);
	if(silent)
		return;

	if(m_currPage == DATA)
	{
		SqlSelect(PathQuery2(CLSMDB,pageid),sRegSel,group);
		SqlSelect(PathQuery2(CLSMDB,IDREG),m_sRegSel,PathQuery2(CLSGROUP,IDREG));
		SqlSelect(PathQuery2(CLSMDB,IDCONF),m_sConfSel,PathQuery2(CLSGROUP,IDCONF));
		SqlSelect(PathQuery2(CLSMDB,IDJH),m_sJhSel,PathQuery2(CLSGROUP,IDJH));
		if(!m_sJhSel.IsEmpty())
			SetElementHtml("jhname",m_sJhSel.AllocSysString());
	}
	if(m_currPage == CONFIG)
	{
		DisableConfigDefault();
	}
	if(op.Compare("check") == 0)
	{
		g_msTemp.SetAt("jdjg","");
		g_msTemp.SetAt("mvalues","");
		
		DataIntegrateForCheck(g_msTemp,ZINDEX_MAX,true);
	}
	if(op.Compare("compensate") == 0)
	{
		g_msTemp.SetAt("jdjg","");
		g_msTemp.SetAt("mvalues","");
		
		DataIntegrateForCompensate(g_msTemp,ZINDEX_MAX);
	}
	UpdateData(FALSE);
	
	CString schsel = _T("<select id=schsel>")+GenerateOptionHtml(sRegLst,sRegSel)+_T("</select>");
	this->SetElementHtml("schemelist",schsel.AllocSysString());



	POSITION pos;
	CString value,key;
	IHTMLElement* newElement;

	if(bFirst && (sRegSel.Compare("测量结果") == 0))
	{
		SqlDeleteOnly(PathQuery2(CLSMDB,pageid),sRegSel,group);
		sRegSel = SqlAdd(PathQuery2(CLSMDB,pageid),sRegSel,group);
		for( pos = g_msTemp.GetStartPosition(); pos != NULL; )
		{
			g_msTemp.GetNextAssoc( pos, key, value );
			if(S_OK != GetElement(key,&newElement))
				continue;
			g_msTemp.SetAt(key,"");
		}
	}

	for( pos = g_msTemp.GetStartPosition(); pos != NULL; )
	{
		bool bNeedUpdate = true;
		g_msTemp.GetNextAssoc( pos, key, value );
		
		if(S_OK != GetElement(key,&newElement))
				continue;
		if((key.Find("select") >= 0) || (key.Find("jhsel") >= 0)){
			VARIANT vl;
			
			BSTR bstr = value.AllocSysString();
			vl.vt = VT_BSTR;
			vl.bstrVal = bstr;
			newElement->setAttribute(m_elemTypes[3],vl);
		}else
		{

/*			if(bFirst)
			{
				g_msTemp.SetAt(key,_T(""));
				value.Empty();
			}
*/
			IHTMLStyle *phtmlStyle;
			newElement->get_style(&phtmlStyle);
			VARIANT varColor;
			varColor.vt = VT_I4;

			CString id=this->GetAttribute(newElement,m_elemTypes[5]);//get className
			if(id.Find("HIDE") != 0)
			{
				
				if(value.Find("<font color=") != 0)
				{
					if(phtmlStyle)
						varColor.lVal = 0x000000;
					if((id.Find("ONCE") == 0) && (value.FindOneOf(_T("0123456789")) < 0))
					{
						bNeedUpdate = false;
						this->SetElementHtml(key,L"单击测量");
					}
				}else{
					if(phtmlStyle)
						varColor.lVal = 0xff0000; //red color

					value.Delete(0,value.Find('>',0)+1);
					value.Replace(_T("</font>"),_T(""));
				}
				
				if((op.Compare("compensate") != 0) && (op.Compare("check") != 0))
				{
					if(key[0] == 'Z')
					{	//check the calibration result pass/fail 
						int pos = atoi(key.Right(key.GetLength()-1));
						if(pos < ZINDEX_MAX){
							if(!CheckPassFail(g_msTemp,pos))
								varColor.llVal = 0xff0000;
						}
					}
				}
				if(phtmlStyle)
				{
					phtmlStyle->put_color(varColor);
					phtmlStyle->Release();
				}
				if(bNeedUpdate)
				{
					this->SetElementText(key,value.AllocSysString());
				}
			}
		}
    }

	//update the g_TestType
	g_msTemp.Lookup("xhgg",g_TesterConf.m_Xhgg);
}

void CTryData4Dlg::UpdateConfSetting(bool StringToStruct)
{
	if(m_currPage == CONFIG)
	{
		if(StringToStruct)
		{
			CString value;
			g_msTemp.Lookup("confdxcl",value);g_TesterConf.m_Ps.bSingle = (value.Compare("on") == 0)?1:0;
			g_msTemp.Lookup("confcycs",value);g_TesterConf.m_Ps.iTimes = atoi(value);
			g_msTemp.Lookup("confcysj",value);g_TesterConf.m_Ps.iDelay = atoi(value);
			g_msTemp.Lookup("confwd",value);g_TesterConf.m_Ps.dTemper = atof(value);
			g_msTemp.Lookup("lcselect",g_TesterConf.m_Ps.sRange);
			g_msTemp.Lookup("zdpselect",g_TesterConf.m_Ps.sLowest);

			g_TesterConf.m_Ps.iRange = CTesterCaculate::searchlclst(g_TesterConf.m_Ps.sRange);
		    g_TesterConf.m_Ps.iLowest = CTesterCaculate::searchpanlst(g_TesterConf.m_Ps.sLowest);

			for(char i='1';i <= '9';i++){
				g_msTemp.Lookup(CString("cklt")+i,value); 
				g_TesterConf.m_Rs.bUse[i-'1'] = (value.Compare("on") == 0)?1:0;
				g_msTemp.Lookup(CString("rstdlt")+i,value);
				g_TesterConf.m_Rs.dValue[i-'1'] = atof(value);
				g_msTemp.Lookup(CString("alphalt")+i,value);
				g_TesterConf.m_Rs.dAlpha[i-'1'] = atof(value);
				g_msTemp.Lookup(CString("betalt")+i,value);
				g_TesterConf.m_Rs.dBeta[i-'1'] = atof(value);
			}
		}else{
			g_msTemp.SetAt("confdxcl",(g_TesterConf.m_Ps.bSingle == 1)?"on":"");
			g_msTemp.SetAt("lcselect",g_TesterConf.m_Ps.sRange);
			g_msTemp.SetAt("zdpselect",g_TesterConf.m_Ps.sLowest);
			for(char i='1';i <= '9';i++){
				g_msTemp.SetAt(CString("cklt")+i,(g_TesterConf.m_Rs.bUse[i-'1'] == 1)?"on":""); 
			}
		}
	}
	if(m_currPage == JIAOHE)
	{
		if(StringToStruct)
		{
			g_msTemp.Lookup("jhsel0",m_jhsel0);
			g_msTemp.Lookup("jhsel1",m_jhsel1);
			g_msTemp.Lookup("jhsel2",m_jhsel2);
			g_msTemp.Lookup("jhsel3",m_jhsel3);
			g_msTemp.Lookup("jhsel4",m_jhsel4);
			g_msTemp.Lookup("jhsel5",m_jhsel5);
			g_msTemp.Lookup("jhsel6",m_jhsel6);
			g_msTemp.Lookup("jhsel7",m_jhsel7);
			g_msTemp.Lookup("jhsel8",m_jhsel8);
			g_msTemp.Lookup("jhsel9",m_jhsel9);
			g_msTemp.Lookup("jhsel10",m_jhsel10);
			g_msTemp.Lookup("jhsel11",m_jhsel11);
			g_msTemp.Lookup("jhsel12",m_jhsel12);
			g_msTemp.Lookup("jhsel13",m_jhsel13);
			g_msTemp.Lookup("jhsel14",m_jhsel14);
			g_msTemp.Lookup("jhsel15",m_jhsel15);
			g_msTemp.Lookup("jhsel16",m_jhsel16);
			g_msTemp.Lookup("jhsel17",m_jhsel17);
			g_msTemp.Lookup("jhsel18",m_jhsel18);
			g_msTemp.Lookup("jhsel19",m_jhsel19);
		}else
		{
			g_msTemp.SetAt("jhsel0",m_jhsel0);
			g_msTemp.SetAt("jhsel1",m_jhsel1);
			g_msTemp.SetAt("jhsel2",m_jhsel2);
			g_msTemp.SetAt("jhsel3",m_jhsel3);
			g_msTemp.SetAt("jhsel4",m_jhsel4);
			g_msTemp.SetAt("jhsel5",m_jhsel5);
			g_msTemp.SetAt("jhsel6",m_jhsel6);
			g_msTemp.SetAt("jhsel7",m_jhsel7);
			g_msTemp.SetAt("jhsel8",m_jhsel8);
			g_msTemp.SetAt("jhsel9",m_jhsel9);
			g_msTemp.SetAt("jhsel10",m_jhsel10);
			g_msTemp.SetAt("jhsel11",m_jhsel11);
			g_msTemp.SetAt("jhsel12",m_jhsel12);
			g_msTemp.SetAt("jhsel13",m_jhsel13);
			g_msTemp.SetAt("jhsel14",m_jhsel14);
			g_msTemp.SetAt("jhsel15",m_jhsel15);
			g_msTemp.SetAt("jhsel16",m_jhsel16);
			g_msTemp.SetAt("jhsel17",m_jhsel17);
			g_msTemp.SetAt("jhsel18",m_jhsel18);
			g_msTemp.SetAt("jhsel19",m_jhsel19);
		}
	}
}
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

			sValue = (g_TesterConf.m_RsDefault.bUse[i-'0']) ? "on":"";
			sKey.Format("cklt%d",i+1-'0');	g_msTemp.SetAt(sKey,sValue);
		}
}

