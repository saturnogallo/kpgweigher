// TryData4Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TryData4.h"
#include "TryData4Dlg.h"
#include "TryDataSql.h"
#include "trydatadefine.h"
#include "InputDlg.h"
#include "TesterData.h"
#include "..\SjInclude\Double2String\sjdouble2string.h"
#include "..\SjInclude\Maths\sjCalib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_NAME "Default"
extern CString SjGetAppDirectory();
CString g_Path;
extern CMapStringToString g_msTemp;
extern TESTERCONF g_TesterConf;
extern CTesterData g_Data;

extern  bool CheckPassFail(CMapStringToString& mstr,int position);
// CTryData4Dlg 对话框

BEGIN_DHTML_EVENT_MAP(CTryData4Dlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
	DHTML_EVENT_ONCLICK(_T("schsel"),OnSchSelect)
	DHTML_EVENT_ONCLICK(_T("schadd"),OnSchAdd)
	DHTML_EVENT_ONCLICK(_T("schdel"),OnSchDelete)
	DHTML_EVENT_ONCLICK(_T("schupd"),OnSchUpdate)
	DHTML_EVENT_ONCLICK(_T("fltidselect"),OnFltidSelect)
	DHTML_EVENT_ONCLICK(_T("fltvalselect"),OnFltvalSelect)
	DHTML_EVENT_ONCLICK(_T("report_saveas"),OnReportSaveAs)
	DHTML_EVENT_ONCLICK(_T("report_print"),OnReportPrint)
	DHTML_EVENT_ONCLICK(_T("report_openall"),OnReportOpenAll)
	DHTML_EVENT_ONCLICK(_T("data_export"),OnDataExport)
	DHTML_EVENT_ONCLICK(_T("data_xiuzheng"),OnDataXiuzheng)
	DHTML_EVENT_ONCLICK(_T("data_jiaohe"),OnDataJiaohe)
	
    DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, _T("CMD"),  OnMouseOverCmd)
    DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOUT, _T("CMD"),  OnMouseOutCmd)
    DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, _T("ONCE"),  OnMouseOverOnce)
    DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOUT, _T("ONCE"),  OnMouseOutOnce)
	DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, _T("URL"),  OnMouseOverUrl)
    DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOUT, _T("URL"),  OnMouseOutUrl)
    DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONCLICK, _T("CMD"), OnClickCmd)
	DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONCLICK, _T("ONCE"), OnClickOnce)
    DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONCLICK, _T("URL"), OnClickUrl)
	DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONFOCUSOUT, _T("EDIT"), OnFinishEdit)
END_DHTML_EVENT_MAP()


CTryData4Dlg::CTryData4Dlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CTryData4Dlg::IDD, CTryData4Dlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_CheckTicks = 0;
	m_currPage = IDR_HTML1;
	m_cmdStep = 1;
	m_State = IDLE;
	m_sDataUrl.Empty();
	m_sJhvals.Format(",jhsel19,jhsel18,jhsel17,jhsel16,jhsel15,jhsel14,jhsel13,jhsel12,jhsel11,jhsel10,jhsel9,jhsel8,jhsel7,jhsel6,jhsel5,jhsel4,jhsel3,jhsel2,jhsel1,jhsel0,");
	m_bAddData = false;
	m_pChartDlg = NULL;
	m_pWaitDlg = NULL;
}

void CTryData4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_DHtml_SelectString(pDX, "jhsel0", m_jhsel0);
	DDX_DHtml_SelectString(pDX, "jhsel1", m_jhsel1);
	DDX_DHtml_SelectString(pDX, "jhsel2", m_jhsel2);
	DDX_DHtml_SelectString(pDX, "jhsel3", m_jhsel3);
	DDX_DHtml_SelectString(pDX, "jhsel4", m_jhsel4);
	DDX_DHtml_SelectString(pDX, "jhsel5", m_jhsel5);
	DDX_DHtml_SelectString(pDX, "jhsel6", m_jhsel6);
	DDX_DHtml_SelectString(pDX, "jhsel7", m_jhsel7);
	DDX_DHtml_SelectString(pDX, "jhsel8", m_jhsel8);
	DDX_DHtml_SelectString(pDX, "jhsel9", m_jhsel9);
	DDX_DHtml_SelectString(pDX, "jhsel10", m_jhsel10);
	DDX_DHtml_SelectString(pDX, "jhsel11", m_jhsel11);
	DDX_DHtml_SelectString(pDX, "jhsel12", m_jhsel12);
	DDX_DHtml_SelectString(pDX, "jhsel13", m_jhsel13);
	DDX_DHtml_SelectString(pDX, "jhsel14", m_jhsel14);
	DDX_DHtml_SelectString(pDX, "jhsel15", m_jhsel15);
	DDX_DHtml_SelectString(pDX, "jhsel16", m_jhsel16);
	DDX_DHtml_SelectString(pDX, "jhsel17", m_jhsel17);
	DDX_DHtml_SelectString(pDX, "jhsel18", m_jhsel18);
	DDX_DHtml_SelectString(pDX, "jhsel19", m_jhsel19);
	DDX_DHtml_CheckBox( pDX, "cklt1",g_TesterConf.m_Rs.bUse[0]);
	DDX_DHtml_CheckBox( pDX, "cklt2",g_TesterConf.m_Rs.bUse[1]);
	DDX_DHtml_CheckBox( pDX, "cklt3",g_TesterConf.m_Rs.bUse[2]);
	DDX_DHtml_CheckBox( pDX, "cklt4",g_TesterConf.m_Rs.bUse[3]);
	DDX_DHtml_CheckBox( pDX, "cklt5",g_TesterConf.m_Rs.bUse[4]);
	DDX_DHtml_CheckBox( pDX, "cklt6",g_TesterConf.m_Rs.bUse[5]);
	DDX_DHtml_CheckBox( pDX, "cklt7",g_TesterConf.m_Rs.bUse[6]);
	DDX_DHtml_CheckBox( pDX, "cklt8",g_TesterConf.m_Rs.bUse[7]);
	DDX_DHtml_CheckBox( pDX, "cklt9",g_TesterConf.m_Rs.bUse[8]);
	DDX_DHtml_CheckBox( pDX, "confdxcl",g_TesterConf.m_Ps.bSingle);
	DDX_DHtml_SelectString(pDX,"zdpselect",g_TesterConf.m_Ps.sLowest);
	DDX_DHtml_SelectString(pDX,"lcselect",g_TesterConf.m_Ps.sRange);
	DDX_Control(pDX, IDC_EXPLORER1, m_explorer);
}

BEGIN_MESSAGE_MAP(CTryData4Dlg, CDHtmlDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CTryData4Dlg 消息处理程序
HRESULT CTryData4Dlg::OnClickCmd(IHTMLElement* pElement)
{
	if(m_State == RUNNING) return S_OK;

	if(m_sDataUrl.IsEmpty()) //no result has been generated yet, so can't do re-test,
		return S_OK;

	CString sel = this->GetAttribute(pElement,m_elemTypes[5]);
	if(sel.IsEmpty() || (sel.Find("CMD") != 0) || (m_sDataSel.Find("测量结果") != 0))  //only running test scheme can be re-test
		return S_OK;

	sel = this->GetAttribute(pElement,m_elemTypes[2]);
	if(sel.IsEmpty())
		return this->GetEventError(pElement,m_elemTypes[2]);


	int pos = atoi(sel.Right(sel.GetLength()-1));
	if(pos <= 0)
		return S_OK;
	if(IDYES == ::AfxMessageBox("确定要重测吗？",MB_YESNO))
	{
		if(m_Program.SetFunctionPointer("MAIN"))
		{
			CString str("暂停测量");
			g_TesterConf.m_iRetestID = pos;
			m_State = RUNNING;
			SetElementText("pause",str.AllocSysString());
			m_bAddData = true;
		}
	}
	return S_OK;
}
HRESULT CTryData4Dlg::OnClickOnce(IHTMLElement* pElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_sDataUrl.IsEmpty()) //no result has been generated yet, so can't do re-test,
		return S_OK;

	CString sel = this->GetAttribute(pElement,m_elemTypes[5]);

	if(sel.IsEmpty() || (sel.Find("ONCE") != 0) || (m_sDataSel.Find("测量结果") != 0)) //only running test scheme can be re-test
		return S_OK;

    //title will contain two part, one is function name, another is parameter
	sel = this->GetAttribute(pElement,m_elemTypes[3]); 
	if(sel.IsEmpty())
		return this->GetEventError(pElement,m_elemTypes[3]);

	CString func=sel.Left(sel.Find(':'));
	sel = sel.Right(sel.GetLength()-(func.GetLength()+1));
	m_Program.m_iStackDepth = 0;
	if(m_Program.SetFunctionPointer(func) && m_Program.SetFunctionParameter(sel))
	{
		OnMouseOutOnce(pElement);
		CString str("暂停测量");
		g_TesterConf.m_iRetestID = -1;
		m_State = RUNNING;
		SetElementText("pause",str.AllocSysString());
		m_bAddData = true;
	}
	return S_OK;
}

HRESULT CTryData4Dlg::OnDataJiaohe(IHTMLElement *pElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
		
	Handler("check",m_sDataSel,m_sDataLst);
	return S_OK;
}
HRESULT CTryData4Dlg::OnDataXiuzheng(IHTMLElement *pElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
	Handler("compensate",m_sDataSel,m_sDataLst);
	return S_OK;
}

HRESULT CTryData4Dlg::OnFltidSelect(IHTMLElement *pElement)
{
	m_sFltid=this->GetAttribute(pElement,m_elemTypes[1]);

	//search for fltval and fill into the fltvalselect
	CString sFltLst("任意值,");
	if(!m_sFltid.IsEmpty())
	{
		CString sflt;
		sflt.Format("where [position]='%s' AND [group]='%s'",m_sFltid,PathQuery2(CLSGROUP,IDREG));
		sFltLst += SqlIndex(PathQuery2(CLSMDB,IDREG),"value",sflt);
	}
	m_sFltval.Empty();
	CString schsel = _T("<select id=fltvalselect>")+GenerateOptionHtml(sFltLst,"任意值")+_T("</select>");
	this->SetElementHtml("filtervalue",schsel.AllocSysString());
	return S_OK;
}
HRESULT CTryData4Dlg::OnFltvalSelect(IHTMLElement *pElement)
{
	m_sFltval=this->GetAttribute(pElement,m_elemTypes[1]);
	m_sFltval.Replace("任意值",""); 
	Handler("init",m_sRegSel,m_sRegLst);
	return S_OK;
}


HRESULT CTryData4Dlg::OnReportSaveAs(IHTMLElement* pElement)
{
	ReportHandler("saveas");
    return S_OK;
}
HRESULT CTryData4Dlg::OnReportPrint(IHTMLElement* pElement)
{
	ReportHandler("print");
    return S_OK;
}
HRESULT CTryData4Dlg::OnReportOpenAll(IHTMLElement* pElement)
{
	ReportHandler("openall");
	return S_OK;
}
HRESULT CTryData4Dlg::OnDataExport(IHTMLElement* pElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
	CString content,result;
	char DirUse[300];
	::GetCurrentDirectory(300,DirUse);
	content=CString(GetElementHtml("cont"));
	content.Replace("单击测量","&nbsp;&nbsp;&nbsp;&nbsp;");
	CFileDialog cfd(FALSE,_T("htm"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Html File(*.htm)|*.htm||"),NULL);
	if(cfd.DoModal() == IDOK){
		CStdioFile jsfile;
		if(jsfile.Open(cfd.GetPathName(),CFile::modeCreate|CFile::modeWrite)){
			jsfile.WriteString(content);
			jsfile.Close();
			if(::AfxMessageBox("原始数据已导出成功\n是否需要立即打印原始数据报告?",MB_YESNO) == IDYES)
			{
				ShellExecute(0,"print",cfd.GetPathName(),"","",SW_HIDE);
			}
		}
		else
		{
			::AfxMessageBox("原始数据保存失败");
		}
		::SetCurrentDirectory(DirUse);
	}
    return S_OK;
}

HRESULT CTryData4Dlg::OnSchSelect(IHTMLElement* pElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
	CString sel = this->GetAttribute(pElement,m_elemTypes[1]);
	if(sel.IsEmpty())
		return this->GetEventError(pElement,m_elemTypes[1]);

	if((m_currPage == INDEX)&&(m_sRegSel.Compare(sel) != 0))
	{
		m_sRegSel = sel;
		Handler("select",m_sRegSel,m_sRegLst);
    }
	if((m_currPage == CONFIG)&&(m_sConfSel.Compare(sel) != 0))
	{
		m_sConfSel = sel;
		Handler("select",m_sConfSel,m_sConfLst);
    }
	if((m_currPage == DATA)&&(m_sDataSel.Compare(sel) != 0))
	{
		m_sDataSel = sel;
		Handler("select",m_sDataSel,m_sDataLst);
    }
	if((m_currPage == JIAOHE)&&(m_sJhSel.Compare(sel) != 0))
	{
		m_sJhSel = sel;
		Handler("select",m_sJhSel,m_sJhLst);
    }
	if((m_currPage == REPORT)&&(m_sReportSel.Compare(sel) != 0))
	{
		m_sReportSel = sel;
		Handler("select",m_sReportSel,m_sReportLst);
    }
	return S_OK;
}
HRESULT CTryData4Dlg::OnSchAdd(IHTMLElement* pElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
	if(m_currPage == INDEX)		Handler("add",m_sRegSel,m_sRegLst);
	if(m_currPage == CONFIG)	Handler("add",m_sConfSel,m_sConfLst);
	if(m_currPage == JIAOHE)	Handler("add",m_sJhSel,m_sJhLst);
	if(m_currPage == DATA)		Handler("add",m_sDataSel,m_sDataLst);
//	if(m_currPage == REPORT)	Handler("add",m_sDataSel,m_sReportLst); //no add for report
	return S_OK;
}
HRESULT CTryData4Dlg::OnSchDelete(IHTMLElement* pElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
	if(m_currPage == INDEX)		Handler("delete",m_sRegSel,m_sRegLst);
	if(m_currPage == CONFIG)	Handler("delete",m_sConfSel,m_sConfLst);
	if(m_currPage == JIAOHE)	Handler("delete",m_sJhSel,m_sJhLst);
	if(m_currPage == DATA)		Handler("delete",m_sDataSel,m_sDataLst);
//	if(m_currPage == REPORT)	Handler("delete",m_sDataSel,m_sReportLst); //no delete for report
    return S_OK;
}
HRESULT CTryData4Dlg::OnSchUpdate(IHTMLElement* pElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
	if(m_currPage == INDEX)		Handler("update",m_sRegSel,m_sRegLst);
	if(m_currPage == CONFIG)	Handler("update",m_sConfSel,m_sConfLst);
	if(m_currPage == JIAOHE)	Handler("update",m_sJhSel,m_sJhLst);
	if(m_currPage == DATA)		Handler("update",m_sDataSel,m_sDataLst);
//	if(m_currPage == REPORT)	Handler("update",m_sDataSel,m_sReportLst); //no update for report
    return S_OK;
}

HRESULT CTryData4Dlg::OnClickUrl(IHTMLElement* pElement)
{
	CString sel = this->GetAttribute(pElement,m_elemTypes[0]);
	UINT newPage=0xffff;
	while(m_cmdStep != CMD_IDLEHTML)
		DoEvents();
	if(sel.Find("#page_") > 0)
	{
		if(m_State == RUNNING) return S_OK;
		if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
		if(sel.Find("#page_reg") > 0)		newPage = INDEX;
		if(sel.Find("#page_data") > 0)		newPage = DATA;
		if(sel.Find("#page_config")>0)		newPage = CONFIG;
		if(sel.Find("#page_jh")>0)			newPage = JIAOHE;
		if(sel.Find("#page_report")>0)		newPage = REPORT;
		if(sel.Find("#page_quit") > 0 ) 
		{
			if(IDYES != MessageBox("是否确定退出?","电阻校验",MB_YESNO))
				return S_OK;
			m_currPage = 0;
			PostMessage(WM_CLOSE,1,0);
			return S_OK;
		}
		if((newPage != 0xffff)&&(newPage != m_currPage))
		{
			m_cmdStep = CMD_LOADHTML;
			m_currPage = newPage;
		}
	}
	if(sel.Find("prg_") > 0)
	{
		if(sel.Find("#prg_start") > 0)
		{
			if(m_State == RUNNING)
			{
				CString str("暂停测量");
				SetElementText("pause",str.AllocSysString());
				if(IDNO == MessageBox("是否继续测量?","电阻校验",MB_YESNO))
				{
					m_State = BEERROR;
					Abort();
				}
				return S_OK;
			}
			if(m_bAddData)		InternalRoutines('9'); //update the 测量结果
			if(!m_Program.InstallScheme(PathQuery2(CLSGROUP,IDTYPE)))
			{
				MessageBox("程序装载失败","电阻校验",MB_OK);
				return S_OK;
			}
			if(m_Program.SetFunctionPointer("MAIN"))
			{
				CString str("暂停测量");
				m_pChartDlg->DeleteAll();
				g_TesterConf.m_iRetestID = -1;
				m_State = RUNNING;
				SetElementText("pause",str.AllocSysString());
				m_bAddData = true;
			};
		}
	}
	if(sel.Find("data_") > 0)
	{
		if(sel.Find("data_chart") > 0)
		{
			m_pChartDlg->ShowWindow(SW_NORMAL);
		}
		if(sel.Find("data_modnbz") > 0)
		{
			if(m_State == RUNNING) return S_OK;
			InternalRoutines('a');
			AfxMessageBox("内标准设置修改成功");
		}
	}
    return S_OK;
}
//store the value of edit
HRESULT CTryData4Dlg::OnFinishEdit(IHTMLElement* pElement)
{
	if(m_State == RUNNING) return S_OK;

	CString sel = this->GetAttribute(pElement,m_elemTypes[5]);
	if(sel.IsEmpty() || (sel.Find("EDIT") != 0))
		return S_OK;

	CString selid = this->GetAttribute(pElement,m_elemTypes[2]);
	if(selid.IsEmpty())
		return S_OK;

	sel = this->GetAttribute(pElement,m_elemTypes[1]); 
	g_msTemp.SetAt(selid,sel);
	while(sel.GetLength() > 0){
		if(sel[0] != ' ')
			break;
		sel.Delete(0);
	}
	if(selid[0] == 'Z')
	{
		int uid = atoi(selid.Right(selid.GetLength()-1));
		
		RESDATA m;
		m.rResult = atof(sel);
		m.rbResult = true;
		g_Data.SetRecord(uid,m);
	}
    return S_OK;
}
void CTryData4Dlg::DisableConfigDefault()
{
	char i;
	IHTMLElement *newElement;
	for(i='1';i<='9';i++)
	{
		VARIANT varEnable;
		varEnable.vt = VT_BOOL;
		varEnable.boolVal = (m_sConfSel.Compare(DEFAULT_NAME) == 0);;

		if(S_OK == this->GetElement(CString("alphalt")+i,&newElement))
			newElement->setAttribute(L"disabled",varEnable);
		if(S_OK == this->GetElement(CString("betalt")+i,&newElement))
			newElement->setAttribute(L"disabled",varEnable);
		if(S_OK == this->GetElement(CString("rstdlt")+i,&newElement))
			newElement->setAttribute(L"disabled",varEnable);

	}
}
HRESULT CTryData4Dlg::OnMouseOverCmd(IHTMLElement *phtmlElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_sDataSel.Find("测量结果") < 0)
		return S_OK;
	CString sel = this->GetAttribute(phtmlElement,m_elemTypes[2]);
	if(sel.IsEmpty() || sel[0] != 'Z')
		return S_OK;
	//inner text
	m_sDataUrl = this->GetAttribute(phtmlElement,m_elemTypes[4]);
	if(m_sDataUrl.FindOneOf("0123456789") < 0)
		return S_OK;
	IHTMLStyle *phtmlStyle;
	phtmlElement->get_style(&phtmlStyle);
	if (phtmlStyle)
	{
		CString retest("重  测");
		phtmlElement->put_innerText(retest.AllocSysString());

		VARIANT varColor;
		varColor.vt = VT_I4;
		varColor.lVal = 0x990033;
		phtmlStyle->put_backgroundColor(varColor);
		phtmlStyle->put_textDecorationUnderline(VARIANT_TRUE);
//		phtmlStyle->put_color(varColor);
		phtmlStyle->put_cursor(L"hand");
		phtmlStyle->Release();
	}
	m_spCurrElement = phtmlElement;
	return S_OK;
}

HRESULT CTryData4Dlg::OnMouseOutCmd(IHTMLElement *phtmlElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_sDataSel.Find("测量结果") < 0)
		return S_OK;
	CString sel = this->GetAttribute(phtmlElement,m_elemTypes[2]);
	if(sel.IsEmpty() || sel[0] != 'Z')
		return S_OK;

	IHTMLStyle *phtmlStyle;
	phtmlElement->get_style(&phtmlStyle);
	if (phtmlStyle)
	{
		if(m_spCurrElement != NULL){
			phtmlElement->put_innerText(m_sDataUrl.AllocSysString());
			m_sDataUrl.Empty();
		}

		VARIANT varColor;
		varColor.vt = VT_BSTR;
		varColor.bstrVal = L"transparent";
		phtmlStyle->put_backgroundColor(varColor);

		varColor.vt = VT_I4;
		varColor.lVal = 0x3366CC;
		phtmlStyle->put_textDecorationUnderline(VARIANT_FALSE);
//		phtmlStyle->put_color(varColor);
		phtmlStyle->Release();
	}
	m_spCurrElement = NULL;
	return S_OK;
}
HRESULT CTryData4Dlg::OnMouseOverOnce(IHTMLElement *phtmlElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_sDataSel.Find("测量结果") < 0)
		return S_OK;
	CString sel = this->GetAttribute(phtmlElement,m_elemTypes[2]);
	if(sel.IsEmpty() || sel[0] != 'Z')
		return S_OK;
	m_sDataUrl = this->GetAttribute(phtmlElement,m_elemTypes[4]);
//	if(m_sDataUrl.FindOneOf("0123456789") < 0)		return S_OK;
	IHTMLStyle *phtmlStyle;
	phtmlElement->get_style(&phtmlStyle);
	if (phtmlStyle)
	{
		CString retest("单击测量");
		phtmlElement->put_innerText(retest.AllocSysString());

		VARIANT varColor;
		varColor.vt = VT_I4;
		varColor.lVal = 0x990033;
		phtmlStyle->put_backgroundColor(varColor);
		phtmlStyle->put_textDecorationUnderline(VARIANT_TRUE);
//		phtmlStyle->put_color(varColor);
		phtmlStyle->put_cursor(L"hand");
		phtmlStyle->Release();
	}
	m_spCurrElement = phtmlElement;
	return S_OK;
}

HRESULT CTryData4Dlg::OnMouseOutOnce(IHTMLElement *phtmlElement)
{
	if(m_State == RUNNING) return S_OK;
	if(m_sDataSel.Find("测量结果") < 0)
		return S_OK;
	CString sel = this->GetAttribute(phtmlElement,m_elemTypes[2]);
	if(sel.IsEmpty() || sel[0] != 'Z')
		return S_OK;

	IHTMLStyle *phtmlStyle;
	phtmlElement->get_style(&phtmlStyle);
	if (phtmlStyle)
	{
		if(m_spCurrElement != NULL){
			phtmlElement->put_innerText(m_sDataUrl.AllocSysString());
			m_sDataUrl.Empty();
		}

		VARIANT varColor;
		varColor.vt = VT_BSTR;
		varColor.bstrVal = L"transparent";
		phtmlStyle->put_backgroundColor(varColor);

		varColor.vt = VT_I4;
		varColor.lVal = 0x3366CC;
		phtmlStyle->put_textDecorationUnderline(VARIANT_FALSE);
//		phtmlStyle->put_color(varColor);
		phtmlStyle->Release();
	}
	m_spCurrElement = NULL;
	return S_OK;
}

HRESULT CTryData4Dlg::OnMouseOverUrl(IHTMLElement *phtmlElement)
{
	CString id=GetAttribute(phtmlElement,m_elemTypes[2]);

	IHTMLStyle *phtmlStyle;
	phtmlElement->get_style(&phtmlStyle);
	if (phtmlStyle)
	{
		VARIANT varColor;
		varColor.vt = VT_I4;
		if(id.Find("page_") >= 0)
			varColor.lVal = 0x333333;
		else
			varColor.lVal = 0x330033;
		phtmlStyle->put_textDecorationUnderline(VARIANT_TRUE);
		phtmlStyle->put_color(varColor);
		phtmlStyle->put_cursor(L"hand");
		phtmlStyle->Release();
	}
	m_spCurrElement = phtmlElement;
	return S_OK;
}

HRESULT CTryData4Dlg::OnMouseOutUrl(IHTMLElement *phtmlElement)
{
	CString id=GetAttribute(phtmlElement,m_elemTypes[2]);
	IHTMLStyle *phtmlStyle;
	phtmlElement->get_style(&phtmlStyle);
	if (phtmlStyle)
	{
		VARIANT varColor;
		varColor.vt = VT_I4;
		varColor.lVal = 0x3366CC;
		phtmlStyle->put_textDecorationUnderline(VARIANT_FALSE);
		phtmlStyle->put_color(varColor);
		phtmlStyle->Release();
	}
	m_spCurrElement = NULL;
	return S_OK;
}

BOOL CTryData4Dlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_Path=SjGetAppDirectory();
	m_explorer.ShowWindow(SW_HIDE);
	InitElementTypes();
	BringWindowToTop();
	CenterWindow();
	SetTimer(1,100,NULL);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTryData4Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}
HRESULT STDMETHODCALLTYPE CTryData4Dlg::ShowContextMenu(DWORD /*dwID*/, POINT *ppt, IUnknown* /*pcmdtReserved*/, IDispatch* /*pdispReserved*/)
{
    // no context menu
    return S_OK;
}
//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CTryData4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
HRESULT CTryData4Dlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CTryData4Dlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
HRESULT CTryData4Dlg::GetEventError(IHTMLElement* pElement, BSTR attr)
{
	CComVariant varCmd;
    return pElement->getAttribute(attr, 0, &varCmd);
}
CString CTryData4Dlg::GetAttribute(IHTMLElement* pElement, BSTR attr)
{
	CComVariant varCmd;
    HRESULT hr = pElement->getAttribute(attr, 0, &varCmd);
    if(hr!=S_OK) return CString("");
	if(varCmd.vt != VT_NULL && varCmd.bstrVal)
		return CString(varCmd.bstrVal);
	else
		return CString("");
}

void CTryData4Dlg::OnTimer(UINT nIDEvent) 
{
	static bool bTwice = false;
	if(m_currPage == 0)	//is quiting
		return;
	if(bTwice)
		return;
	bTwice = true;
	SWISTATUS ss;
	CDHtmlDialog::OnTimer(nIDEvent);
	if (nIDEvent == 1)
	{
		CString status;
		m_CheckTicks++;
		if(m_CheckTicks == 1){
			status.Format("正在进行License检查");
			SetElementText(_T("checking"),status.AllocSysString());
			}
		if(m_CheckTicks == 2){	
			if(!CheckLicense())
			{
				MessageBox("本软件无法在本机上使用,请联系供应商");
				m_currPage = 0;
				::AfxGetMainWnd()->PostMessage(WM_CLOSE,1,0);
			}
			status.Format("正在连接控制板");
			SetElementText(_T("checking"),status.AllocSysString());
			}
		if(m_CheckTicks == 3){
			if(!m_Switch.Open(SPORT_SWITCH))
			{
				MessageBox("连接控制板失败","电阻校验",MB_OK);
				m_currPage = 0;
				::AfxGetMainWnd()->PostMessage(WM_CLOSE,1,0);
			}
		
			status.Format("正在连接纳伏计");
			SetElementText(_T("checking"),status.AllocSysString());
			}
		if(m_CheckTicks == 4){
			if(!m_Navmeter.Open(SPORT_NAVMETER))
			{
				MessageBox("连接纳伏计失败","电阻校验",MB_OK);
				m_currPage = 0;
				::AfxGetMainWnd()->PostMessage(WM_CLOSE,1,0);
			}

			status.Format("正在连接数据库");
			SetElementText(_T("checking"),status.AllocSysString());
		}
		if(m_CheckTicks == 5){
			if(!SqlInit(g_Path+_T("\\Data\\test.mdb")))
			{
				MessageBox("连接数据库失败","电阻校验",MB_OK);
				m_currPage = 0;
				::AfxGetMainWnd()->PostMessage(WM_CLOSE,1,0);
			}
			status.Format("正在装载程序");
			SetElementText(_T("checking"),status.AllocSysString());
		}
		if(m_CheckTicks == 6){
			if(!m_Program.InstallScheme(PathQuery2(CLSGROUP,IDTYPE)))
			{
				MessageBox("程序装载失败","电阻校验",MB_OK);
				m_currPage = 0;
				::AfxGetMainWnd()->PostMessage(WM_CLOSE,1,0);
			}
		}
		if(m_CheckTicks == 7){
			m_currPage = INDEX;		Handler("init",m_sRegSel,m_sRegLst,true);
//			m_currPage = CONFIG;	Handler("init",m_sConfSel,m_sConfLst,true);
//			m_currPage = DATA;		Handler("init",m_sDataSel,m_sDataLst,true);
//			m_currPage = JIAOHE;	Handler("init",m_sJhSel,m_sJhLst,true);
			KillTimer(1);
			m_currPage = INDEX;
			m_cmdStep = CMD_LOADHTML;
			//create the chart dialog
			m_pChartDlg = new CChartDlg(this);
			m_pChartDlg->Create(IDD_DIALOG2,this);
			m_pChartDlg->ShowWindow(SW_HIDE);
			//create the wait dialog
			m_pWaitDlg = new CWaitDialog(this);
			m_pWaitDlg->Create(IDD_DIALOG3,this);
			m_pWaitDlg->ShowWindow(SW_HIDE);

			SetTimer(2,30,NULL);
		}
	}	
	
	if(nIDEvent == 2)
	{
		if(m_cmdStep == CMD_INITHTML){
			IHTMLElement* newElement;
			if((m_currPage == INDEX) && (S_OK == GetElement("idvals",&newElement)))
			{
				m_sRegvals = this->GetAttribute(newElement,m_elemTypes[1]);
				Handler("init",m_sRegSel,m_sRegLst);
			}
			if((m_currPage == CONFIG) && (S_OK == GetElement("idvals",&newElement)))
			{
				m_sConfigvals = this->GetAttribute(newElement,m_elemTypes[1]);
				Handler("init",m_sConfSel,m_sConfLst);
			}
			if((m_currPage == JIAOHE) && (S_OK == GetElement("idvals",&newElement)))
			{

				m_sJhvals = this->GetAttribute(newElement,m_elemTypes[1]);
				Handler("init",m_sJhSel,m_sJhLst);
			}
			if((m_currPage == DATA) && (S_OK == GetElement("idvals",&newElement)))
			{
				if(g_TesterConf.m_Ps.iTimes == 0) //come to the data page directly from reg page 
				{
					m_currPage = CONFIG;
					m_sConfigvals = CString(",cklt9,rstdlt9,alphalt9,betalt9,cklt8,rstdlt8,alphalt8,betalt8,cklt1,rstdlt1,alphalt1,betalt1,cklt2,rstdlt2,alphalt2,betalt2,cklt3,rstdlt3,alphalt3,betalt3,cklt4,rstdlt4,alphalt4,betalt4,cklt5,rstdlt5,alphalt5,betalt5,cklt6,rstdlt6,alphalt6,betalt6,cklt7,rstdlt7,alphalt7,betalt7,confdxcl,zdpselect,lcselect,confcysj,confcycs,confwd,");
					Handler("init",m_sConfSel,m_sConfLst,true);


					m_currPage = JIAOHE;
					m_sJhvals = CString(",jhsel19,jhsel18,jhsel17,jhsel16,jhsel15,jhsel14,jhsel13,jhsel12,jhsel11,jhsel10,jhsel9,jhsel8,jhsel7,jhsel6,jhsel5,jhsel4,jhsel3,jhsel2,jhsel1,jhsel0,");
					Handler("init",m_sJhSel,m_sJhLst,true);
					m_sJhvals.Empty();

					m_currPage = DATA;
				}

				m_sDatavals = this->GetAttribute(newElement,m_elemTypes[1]);
				Handler("init",m_sDataSel,m_sDataLst);
				if(m_sDataLst.IsEmpty() || (m_sDataLst == " ,"))
				{
						InternalRoutines('9');//update value to page
				}
				Handler("update",m_sDataSel,m_sDataLst);
				
			}
			if(m_currPage == REPORT)
			{
				Handler("init",m_sReportSel,m_sReportLst);
			}
			m_cmdStep = CMD_IDLEHTML;
		}
		if((m_cmdStep == CMD_CONTHTML)||(m_cmdStep == CMD_CONTHTML2)){
			int group = -1;
			switch(m_currPage)
			{
			case JIAOHE:	group=IDJH;break;
			case DATA:		group=IDTEST;break;
			default: break;
			}
			if(group >= 0)
			{
				CStdioFile csf;
				if(csf.Open(g_Path+CString(PathQuery2(CLSJS,group)),CFile::modeRead))
				{
					CString content(""),line;
					while(csf.ReadString(line))
					{
						content += line;
					}
					csf.Close();
					SetElementHtml("cont",content.AllocSysString());
				}
			}
			if(m_cmdStep == CMD_CONTHTML2)
				m_cmdStep = CMD_INITHTML;
			else
				m_cmdStep = CMD_CONTHTML2;
		}
		if(m_cmdStep == CMD_LOADHTML && m_currPage > 0){
			LoadFromResource(m_currPage);
			m_cmdStep = CMD_CONTHTML;
		}
				CString str;
				CTime t = CTime::GetCurrentTime();
				str = t.Format("%H:%M:%S");
				SetElementText("curtime",str.AllocSysString());
		
		if(m_State == SUSPEND)			m_State = IDLE;
		if(m_State == BEERROR)			m_State = SUSPEND;

		if((m_State == IDLE)||(m_State == BEERROR))
		{
			if(m_CheckTicks++ > CHECKWAIT)
			{
				m_CheckTicks = 0;
#if 1
				SWISTATUS ss = m_Switch.GetSwitchStatus();
				while(ss.bOl1)
				{
					MessageBox("发生过载","电阻校验",MB_OK);
					ss = m_Switch.GetSwitchStatus();
				}

				CString str;
				str.Format("外标准[%s],被测[%s]",ss.bKN[0]?"接通":"未接",ss.bRn?"接通":"未接");
				SetElementText("swistatus",str.AllocSysString());
#endif;
			}
		}
		if(m_State == RUNNING)
		{
			char type;
			CString line;
//			m_State = SUSPEND;
				while((m_Program.m_iStackDepth > 0) )
				{
					if(m_Program.NextLine(type,line))//loop until some pausable command
					{
						if((type == 'z') || (type == 'd') || (type == 'w'))
						{
							HandleCommand(type,line);
							continue;
						}
						break;
					}
					
					 
				}
			    if(m_Program.m_iStackDepth <= 0)
				{
					Abort();
				}else{
					if(!HandleCommand(type,line))
					{
						m_State = BEERRO;
						Abort();
					}
				}
		}
	}
	bTwice = false;
	return;

}

extern char kbuffer[];
extern void RetrieveDriverID();
// Check the valid of license
bool CTryData4Dlg::CheckLicense(void)
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
void CTryData4Dlg::OnDestroy()
{
	SqlQuit();
	/*
	POSITION pos = g_msTemp.GetStartPosition();
	while(pos != NULL)
	{
		CString skey,svalue;
		g_msTemp.GetNextAssoc(pos,skey,svalue);
		g_msTemp.RemoveKey(skey);
	}*/
	
	m_explorer.Navigate("about:blank",NULL,NULL,NULL,NULL);
//	g_msTemp.RemoveAll();
	if(m_pChartDlg != NULL)
	{
		m_pChartDlg->bQuit = true;
		m_pChartDlg->DestroyWindow();
		delete m_pChartDlg;
	}
	if(m_pWaitDlg != NULL)
	{
		m_pWaitDlg->bQuit = true;
		m_pWaitDlg->DestroyWindow();
		delete m_pWaitDlg;
	}

}
void CTryData4Dlg::DoEvents()
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

bool CTryData4Dlg::HandleCommand(char type, CString Parm)
{
	CString str,str1;
	char  sParm[300];
	double data[200];
	int count;
	int max,min,lines,left;
	char oper;
	bool bRet;
	RESDATA rd;
	CString szlog;
	char tmpbuf[128];

	switch(type)
	{
	case 's':	//turn the switch
		str.Format("开关切换");
		SetElementText("action",str.AllocSysString());
		strcpy(sParm,(LPCSTR)Parm);
		if(false == m_Switch.DoSwi(sParm))
		{
			SWISTATUS ss = m_Switch.GetSwitchStatus();
			while(ss.bOl1)
			{
				MessageBox("发生过载","电阻校验",MB_OK);
				ss = m_Switch.GetSwitchStatus();
			}
		}
		return true;
	case 't':	//measure and retrieve the value 
		str.Format("取得读数");
		SetElementText("action",str.AllocSysString());
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
				if(m_State == BEERROR)		return false;
			}
#endif
			if(m_Navmeter.SendCommand(NAV_READ))
			{
				data[count++]=m_Navmeter.value;
				_strtime(tmpbuf);
				szlog.Format("[%s]:%.12f\n",tmpbuf,m_Navmeter.value);
				m_Program.logout.WriteString(szlog);
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
		CTesterCaculate::ExtractString(Parm,str1);
		ASSERT(str1.GetLength() > 2);
		if(Parm.GetLength() == 0){
			MessageBox(str1);
		}else{
			CTesterCaculate::ExtractInt(Parm,min);
			CTesterCaculate::ExtractInt(Parm,max);
			if(!m_Navmeter.SendCommand(NAV_READ))
				return false;

			double lastval = m_Navmeter.value;
			if(min <= max){
				m_pWaitDlg->SetWaitingText(str1);
				m_pWaitDlg->ShowWindow(SW_NORMAL);
				count = 300;
				while(count-- > 0){
					DoEvents();
					Sleep(1);
				}
				::Beep(1000,200);
				count = 0;
				while(m_pWaitDlg->IsWindowVisible()){
					DoEvents();
					if(m_State == BEERROR){		
						m_pWaitDlg->ShowWindow(SW_HIDE);					
						return false;
					}
		
					if(m_Navmeter.SendCommand(NAV_READ))
					{
						if(min == max){
                            if(1000000.0*abs(m_Navmeter.value - lastval) > min)
								count++;
							else
								count = 0;
						}else{
                            if((1000000.0*m_Navmeter.value > min) && (1000000.0*m_Navmeter.value < max))
								count++;
							else
								count = 0;
						}
									
						if(count >= 5)
							break;
						else
							continue;
					}
/*
					if(!m_Navmeter.SendCommand(NAV_INIT)){
						m_pWaitDlg->ShowWindow(SW_HIDE);
						return false;
					}
*/
				}
			}
		}

		m_pWaitDlg->ShowWindow(SW_HIDE);
		return true;		
	case 'n':		//[ID]Send command to navmeter	using ID 	
		CTesterCaculate::ExtractInt(Parm,left);
		CTesterCaculate::ExtractString(Parm,str);
		return m_Navmeter.SendCommand(left);
	case 'i':	    //[ID]Run the InternalRoutines (ID)
		CTesterCaculate::ExtractChar(Parm,oper);
		bRet = InternalRoutines(oper);
		return bRet;
	case 'f':	    //Run single action function
		CTesterCaculate::ExtractChar(Parm,oper);
		CTesterCaculate::ExtractString(Parm,str);
		bRet = ExecuteClickAction(oper,str);
		return bRet;
	case 'd': //control of the FlexGrid
		return HandleDataShow(Parm);
	case 'p':
		//break;
		CTesterCaculate::ExtractInt(Parm,min);
		if(min == -1)
			min = g_TesterConf.m_iDelayKT;
		if(min == -2)
			min = g_TesterConf.m_iDelayKI;

		max = 0;
		while(max++ < min*100)
		{
			Sleep(8);
			DoEvents();
			if(m_State == BEERROR)
					return false;

			if((max%100) == 0)
			{
				str.Format("等待 %is...",min-(max/100));
				SetElementText("action",str.AllocSysString());
			}
		}
		break;
#define NOPRINTER
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
		CTesterCaculate::ExtractChar(Parm,oper);
		CTesterCaculate::ExtractInt(Parm,min);
		CTesterCaculate::ExtractInt(Parm,max);
		CTesterCaculate::ExtractString(Parm,str1);
		str.Format("数据错误");
		if(oper == 'v')
		{
			str1.Format("Z%d",min);
			rd = g_Data.GetRecord(min);
			if(rd.rbValue)
				SetHtmlText(str1,rd.rValue);
		}
		if(oper == 'z')
		{
			str1.Format("Z%d",min+g_TesterConf.m_iBase);
			rd = g_Data.GetRecord(min+g_TesterConf.m_iBase);
			if(rd.rbResult)
				SetHtmlText(str1,rd.rResult);
		}
		break;
	case 'u':
		CTesterCaculate::ExtractChar(Parm,oper);
		if(oper == 'r')
		{
		}
		if(oper == 't')
		{
			CTesterCaculate::ExtractString(Parm,str);
			SetElementText("action",str.AllocSysString());
		}
		break;		
		
	default:
		break;
	}
	return true; //undefined command
}
bool CTryData4Dlg::HandleDataShow(CString line)
{
	//we post data to clipboard  here :)
	//type = 'r','c','t','v','z','p'
	//={rowset(if(-1)added),columnset,textset,valueuse,resultuse,store positioninFlexGrid}
	char type;
	int  num;
	CString str,str1;
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
		
		str1.Format("V%d",num);
		rd = g_Data.GetRecord(num);
		if(rd.rbValue)
			SetHtmlText(str1,rd.rValue);
		break;
	case 'z':
		CTesterCaculate::ExtractInt(line,num);
		str1.Format("Z%d",num);
		rd = g_Data.GetRecord(num+g_TesterConf.m_iBase);
		if(rd.rbResult)
			SetHtmlText(str1,rd.rResult);		
		break;
	case 'p':
		break;
	default:
		return false;
	}
	return true;
}
bool CTryData4Dlg::InternalRoutines(char index)
{
	//double value;
	int i;
	int count=0;
	RSTDTYPE rt;
	RESDATA rd;
	double sum,min,max;
	CString skey,svalue;
	POSITION pos;
	switch(index)
	{
	case '1':
		//check process used here is for standard resistor test
		//requiration: should and only one outer resistor used
		//			 : and resistor value should greater than 0


		for(i = 0;i < 9;i++)
			count += (g_TesterConf.m_Rs.bUse[i])?1:0;

		if(count > 1)	MessageBox("注意：只能使用一个外标准电阻","电阻校验",MB_OK);
		if(count == 0)	MessageBox("注意：必须使用一个外标准电阻","电阻校验",MB_OK);
		
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
			MessageBox(_T("请检查并选择至少一个外标准电阻"),"电阻校验",MB_OK);

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
		MessageBox("why pause here?","电阻校验",MB_OK);
		//g_Tester->Pause();
	case '8':
		//清除原有数据
		pos = g_msTemp.GetStartPosition();
		while(pos != NULL)
		{
			CString skey,svalue;
			g_msTemp.GetNextAssoc(pos,skey,svalue);
			if(skey.Find('Z') == 0){
				svalue.Empty();
				g_msTemp.SetAt(skey,svalue);
				this->SetElementText(skey,svalue.AllocSysString());
			}
		}
		break;
	case '9':
		//保存到测量结果
		skey = m_sDataSel;
		m_sDataSel = "测量结果";
		Handler("update",m_sDataSel,m_sDataLst);
		m_sDataSel = skey;
		m_bAddData = false;
		break;
	case 'a'://修改内标准
		skey = m_sConfSel;
		m_sConfSel = DEFAULT_NAME;
		m_currPage = CONFIG;
		ModifyDefaultRstdSetting();
		Handler("update",m_sConfSel,m_sConfSel,true);
		m_currPage = DATA;
		m_sConfSel = skey;
		break;
	default:
		break;
	}
	return true;

}
void CTryData4Dlg::Abort()
{
	CString str;
	if(m_State == BEERROR)
		str.Format("测试出现错误，已结束");
	else
		str.Format("测试完毕");
	m_State = BEERROR;
	SetElementText("action",str.AllocSysString());
	str.Format("开始测量");
	SetElementText("pause",str.AllocSysString());
}


void CTryData4Dlg::OnCancel()
{
	if(m_currPage > 0)
		return;
	CDHtmlDialog::OnCancel();
}

void CTryData4Dlg::OnOK()
{
	if(m_currPage > 0)
		return;
	CDHtmlDialog::OnOK();
}
void CTryData4Dlg::OnClose()
{
	if(m_currPage > 0)
		return;
	CDHtmlDialog::OnClose();
}
extern CString DataIntergrate(double value,int digipos,int mantissa);
void CTryData4Dlg::SetHtmlText(CString id,double dvalue)
{
	IHTMLElement *newElement;
	if(S_OK != GetElement(id,&newElement))
		return;
	
	CString value = GetAttribute(newElement,m_elemTypes[7]);//prec property
	int mod=1;
	if(value.Find('m') > 0) //module to 1,2,5
	{
		mod = atoi(value.Right(value.GetLength()-value.Find('m')-1));
		
		value = value.Left(value.Find('m'));
	}
	if(value.Find('s') > 0) //scale to another level in format -7s-1 mean value *= pow(10,-1) before going on
	{
		double mul = pow(10.0,(double)atoi(value.Right(value.GetLength()-value.Find('s')-1)));
		dvalue = dvalue * mul;
		value = value.Left(value.Find('s'));
	}
	int iprec = atoi(value);
	if(iprec < -12 || iprec > 12 || iprec == 0)
		iprec = 8;

	if(iprec > 0)
		value = SJD2SByLength(dvalue,iprec);
	else
		value = SJD2SByPosition(dvalue,iprec);

	if(mod != 1)
	{
		value = DataIntergrate(dvalue,iprec,mod);
	}
	g_msTemp.SetAt(id,value);

	CString sel = this->GetAttribute(newElement,m_elemTypes[5]);
	if(sel.IsEmpty() || (sel.Find("HIDE") == 0))
		return;
	
	if(value.GetLength() > 14)//too many digitals
	{
		value = CString("出错");
	}else
	{
		if(m_currPage == DATA)
		{
			m_pChartDlg->AddData(value);
		}
	}
	
	
	if(id[0] == 'Z')
	{	//check the calibration result pass/fail 
		IHTMLStyle *phtmlStyle;
		newElement->get_style(&phtmlStyle);
		VARIANT varColor;
		varColor.vt = VT_I4;
		int pos = atoi(id.Right(id.GetLength()-1));
		if(!CheckPassFail(g_msTemp,pos))
			varColor.llVal = 0xff0000;
		else
			varColor.llVal = 0x000000;

		if(phtmlStyle)
		{
			phtmlStyle->put_color(varColor);
			phtmlStyle->Release();
		}
		this->SetElementText(id,value.AllocSysString());
		return;
	}
	
	this->SetElementText(id,value.AllocSysString());
}

bool CTryData4Dlg::ExecuteClickAction(char oper, CString parm)
{
	if(oper == 'd')
	{
		int iPos = -1;
		iPos = atoi(parm);
		if(iPos < 0) return false;
		RESDATA rd,rd2;
		double dZero1 = 0;
		double dZero2 = 0;
		double dZero3 = 0;
		double dZero4 = 0;
		rd = g_Data.GetRecord(190);
		if(rd.rbResult) {
			dZero1 = rd.rResult;
			if((iPos >= 190) && (iPos < 210)) dZero1 = 0;
			if((iPos == 170)||(iPos == 150)||(iPos == 130))
				return true;
		}
		else
		{
			rd = g_Data.GetRecord(170);
			if(rd.rbResult) {
				dZero1 = rd.rResult;
				if((iPos >= 170) && (iPos < 190)) dZero2 = 0;
				if((iPos == 190)||(iPos == 150)||(iPos == 130))
					return true;
			}
			else
			{
				rd = g_Data.GetRecord(150);
				if(rd.rbResult) {
					dZero1 = rd.rResult;
					if((iPos >= 150) && (iPos < 170)) dZero3 = 0;
					if((iPos == 170)||(iPos == 190)||(iPos == 130))
						return true;

				}
				else
				{
					rd = g_Data.GetRecord(130);
					if(rd.rbResult) {
						dZero1 = rd.rResult;
						if((iPos >= 130) && (iPos < 150)) dZero4 = 0;
						if((iPos == 170)||(iPos == 150)||(iPos == 190))
							return true;
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

		
		if((iPos >= 222) && (iPos <= 224))
		{
			dstd = dFull*((iPos-222)*5+10)*0.1;
			rd2 = g_Data.GetRecord(221);
			dZero = 0;
			if(rd.rbResult)
				dZero = rd2.rResult/1e6;
		}
		if((iPos >= 226) && (iPos <= 228))  
		{
			dstd = dFull*((iPos-226)*5+10)*0.2;
			rd2 = g_Data.GetRecord(225);
			dZero = 0;
			if(rd.rbResult)
				dZero = rd2.rResult/1e6;
		}
		if((iPos >= 230) && (iPos <= 232))  
		{
			dstd = dFull*((iPos-230)*5+10)*5;
			rd2 = g_Data.GetRecord(229);
			dZero = 0;
			if(rd.rbResult)
				dZero = rd2.rResult/1e6;
		}
		if( (iPos == 221) || (iPos == 225) || (iPos == 229))
		{
			dstd = 0;
			dZero = 0;
		}
		rd.rResult = -(dstd-rd.rResult+dZero);
		rd.rResult *= 1e6;
		rd.rbResult = true;
		g_Data.SetRecord(iPos,rd);
	}
	if(oper == 'e')
	{
		int iPos = -1;
		iPos = atoi(parm);
		if(iPos < 0) return false;
		RESDATA rd;
		rd = g_Data.GetRecord(iPos);		
		if(rd.rbResult)
			rd.rResult *= 1e6;
		g_Data.SetRecord(iPos,rd);
	}
	return true;
}
