// TryAgainView.cpp : implementation of the CTryAgainView class
//

#include "stdafx.h"
#include "TryAgain.h"
#include "GraphDlg.h"
#include "TryAgainDoc.h"
#include "TryAgainView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SPORT_NAVMETER   'n'
#define SPORT_SWITCH	 's'
#define PPORT_PRINTER	 1
#define CHECKWAIT		110//30*100ms

const char *mealist="YSIN NSIN YRAN NRAN YRLS NRLS YBRN NBRN YVAL NVAL YRST NRST EMPT YSCA NSCA YRES NRES";


//extract datas from the sParam and the sParam will be cut
int RetrieveIndex(LPCSTR search)
{
	char *pdest;
	pdest = strstr(mealist,search);
	if(pdest == NULL)
		return -1;//not found in swilist
	return (int)(pdest-mealist)/(strlen(search)+1);
};
bool ExtractInt(CString &sParam,int &value)
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
bool ExtractDouble(CString &sParam,double &value)
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
bool ExtractString(CString &sParam,CString &value)
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
bool ExtractChar(CString &sParam,char &value)
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

/////////////////////////////////////////////////////////////////////////////
// CTryAgainView

IMPLEMENT_DYNCREATE(CTryAgainView, CFormView)

BEGIN_MESSAGE_MAP(CTryAgainView, CFormView)
	//{{AFX_MSG_MAP(CTryAgainView)
	ON_BN_CLICKED(IDC_RSTADD, OnRstAdd)
	ON_BN_CLICKED(IDC_RSTDEL, OnRstDelete)
	ON_BN_CLICKED(IDC_RSTSEL, OnRstSelect)
	ON_BN_CLICKED(IDC_PARMADD, OnParmAdd)
	ON_BN_CLICKED(IDC_PARMDEL, OnParmDelete)
	ON_BN_CLICKED(IDC_PARMSEL, OnParmSelect)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BEGINTEST, OnBeginTest)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_COMMAND(ID_EXITPROGRAM, OnExitprogram)
	ON_UPDATE_COMMAND_UI(ID_EXITPROGRAM, OnUpdateExitprogram)
	ON_BN_CLICKED(IDC_QUITPROGRAM, OnQuitProgram)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTabSelchange)
	ON_NOTIFY(NM_CLICK, IDC_TAB1, OnClickTab1)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_BN_CLICKED(IDC_SAVEREPORT, OnSaveReport)
	ON_BN_CLICKED(IDC_DRAWDATA, OnDrawData)
	ON_MESSAGE(WM_TESTOVER,OnEndTest)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_PROGRESS1, OnOutofmemoryProgress1)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTryAgainView construction/destruction
	//ON_BN_CLICKED(IDC_VIEWREPORT, OnViewReport)
CTryAgainView::CTryAgainView()
	: CFormView(CTryAgainView::IDD)
{
	//{{AFX_DATA_INIT(CTryAgainView)
	m_bUse1 = FALSE;
	m_bUse2 = FALSE;
	m_bUse3 = FALSE;
	m_bUse4 = FALSE;
	m_bUse5 = FALSE;
	m_bUse6 = FALSE;
	m_bUse7 = FALSE;
	m_dValue1 = 0.0;
	m_dValue2 = 0.0;
	m_dValue3 = 0.0;
	m_dValue4 = 0.0;
	m_dValue5 = 0.0;
	m_dValue6 = 0.0;
	m_dValue7 = 0.0;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dAlpha3 = 0.0;
	m_dAlpha4 = 0.0;
	m_dAlpha5 = 0.0;
	m_dAlpha6 = 0.0;
	m_dAlpha7 = 0.0;
	m_dBeta1 = 0.0;
	m_dBeta7 = 0.0;
	m_dBeta2 = 0.0;
	m_dBeta3 = 0.0;
	m_dBeta6 = 0.0;
	m_dBeta5 = 0.0;
	m_dBeta4 = 0.0;
	m_bSingle = FALSE;
	m_dTemper = 0.0;
	m_iTimes = 0;
	m_sRunOn = _T("");
	m_sDisplay = _T("");
	m_iDelay = 0.0;
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	bOpen = false;
	bReport = false;
	m_State = BEIDLE;
	m_RetestType = 0;
	m_iGridLine = -1;
	m_CheckCount = 0;
	m_iReplace = -1;
}

CTryAgainView::~CTryAgainView()
{
}

void CTryAgainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTryAgainView)
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_SCHEME, m_Scheme);
	DDX_Control(pDX, IDC_RANGE, m_iRange);
	DDX_Control(pDX, IDC_LOWEST, m_iLowest);
	DDX_Check(pDX, IDC_CHECK1, m_bUse1);
	DDX_Check(pDX, IDC_CHECK2, m_bUse2);
	DDX_Check(pDX, IDC_CHECK3, m_bUse3);
	DDX_Check(pDX, IDC_CHECK4, m_bUse4);
	DDX_Check(pDX, IDC_CHECK5, m_bUse5);
	DDX_Check(pDX, IDC_CHECK6, m_bUse6);
	DDX_Check(pDX, IDC_CHECK7, m_bUse7);
	DDX_Text(pDX, IDC_EDIT1, m_dValue1);
	DDX_Text(pDX, IDC_EDIT4, m_dValue2);
	DDX_Text(pDX, IDC_EDIT7, m_dValue3);
	DDX_Text(pDX, IDC_EDIT10, m_dValue4);
	DDX_Text(pDX, IDC_EDIT13, m_dValue5);
	DDX_Text(pDX, IDC_EDIT16, m_dValue6);
	DDX_Text(pDX, IDC_EDIT19, m_dValue7);
	DDX_Text(pDX, IDC_EDIT2, m_dAlpha1);
	DDX_Text(pDX, IDC_EDIT5, m_dAlpha2);
	DDX_Text(pDX, IDC_EDIT8, m_dAlpha3);
	DDX_Text(pDX, IDC_EDIT11, m_dAlpha4);
	DDX_Text(pDX, IDC_EDIT14, m_dAlpha5);
	DDX_Text(pDX, IDC_EDIT17, m_dAlpha6);
	DDX_Text(pDX, IDC_EDIT20, m_dAlpha7);
	DDX_Text(pDX, IDC_EDIT3, m_dBeta1);
	DDX_Text(pDX, IDC_EDIT21, m_dBeta7);
	DDX_Text(pDX, IDC_EDIT6, m_dBeta2);
	DDX_Text(pDX, IDC_EDIT9, m_dBeta3);
	DDX_Text(pDX, IDC_EDIT18, m_dBeta6);
	DDX_Text(pDX, IDC_EDIT15, m_dBeta5);
	DDX_Text(pDX, IDC_EDIT12, m_dBeta4);
	DDX_Check(pDX, IDC_SINGLE, m_bSingle);
	DDX_Text(pDX, IDC_TEMPERATURE, m_dTemper);
	DDX_Text(pDX, IDC_TIMES, m_iTimes);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_Grid);
	DDX_Control(pDX, IDC_EXPLORER1, m_Web);
	DDX_Text(pDX, IDC_DISPLAY, m_sRunOn);
	DDX_Text(pDX, IDC_HEADER, m_sDisplay);
	DDX_Text(pDX, IDC_DELAY, m_iDelay);
	//}}AFX_DATA_MAP
}

BOOL CTryAgainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CFormView::PreCreateWindow(cs);
}

void CTryAgainView::OnInitialUpdate()
{
	RSTDTYPE m_rs;
	PARAMTYPE m_pt;
	char DirUse[300];
	bool bOk;
	CRect rc;
	TCITEM tcItem;
	CHARFORMAT cf;
	CRichEditCtrl *re;

	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	bOk = true;
	m_iBase = 0;	

	if(bOk)
	{
	//	bOk = CheckDriveID();
		if(!bOk)
			m_iBase = 7;
	}


	if(bOk)
	{
			bOk =m_Switch.Open(SPORT_SWITCH);
			if(!bOk)
				m_iBase = 2;
	}

	if(bOk)
	{
		bOk = m_Navmeter.Open(SPORT_NAVMETER);
		if(!bOk)
			m_iBase = 1;
	}

#ifndef  NOPRINTER
	m_WHPrinter.Open(PPORT_PRINTER);
#endif

		this->IntallScheme();


	if(bOk)
	{
		bOk = m_rset.InstallFromFile();
		if(!bOk)
			m_iBase = 4;
	}

	if(bOk)
	{
		bOk = m_pset.InstallFromFile();
		if(!bOk)
			m_iBase = 5;
	}

	if(bOk)
	{
		bOk = (m_Scheme.GetCount() > 0);
		if(!bOk)
			m_iBase = 6;
	}
	if(!bOk)
		m_State = BEFINI;

	delete splash;

	if(!bOk)
		return;

	m_Scheme.SetCurSel(0);
	m_Progress.ShowWindow(SW_HIDE);


	m_rs = m_rset.GetCurRstd();
	m_pt = m_pset.GetCurParam();
	UpdateRstSetting(m_rs);
	UpdateParamSetting(m_pt);

	m_Grid.SetCols(3);
	m_Grid.SetRow(0);
	m_Grid.SetCol(0);
	
	m_Grid.GetClientRect(&rc);

	m_Grid.SetColWidth(2,2*m_Grid.GetColWidth(1));
	m_Grid.SetRowHeightMin(500);

//	m_Grid.SetFontWidth(10);

	::GetCurrentDirectory(300,DirUse);
	strcat(DirUse,"\\Data\\temp.htm");
	if(csf.Open(DirUse,CFile::modeRead))
	{
		csf.Close();
		m_Web.Navigate(DirUse,NULL,NULL,NULL,NULL);
	}
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T(" 测试 ");
    m_Tab.InsertItem(0, &tcItem);

	tcItem.pszText = _T(" 报告 ");
	m_Tab.InsertItem(1,&tcItem);
	m_Tab.SetCurSel(0);
	m_Web.SetVisible(FALSE);
	m_WebWidth = m_Web.GetWidth();
	m_Web.SetWidth(1);
	re = (CRichEditCtrl*)GetDlgItem(IDC_HEADER);
	re->GetDefaultCharFormat(cf);
	cf.dwMask |= CFM_SIZE;
	cf.yHeight = 500;
	re->SetDefaultCharFormat(cf);
}

/////////////////////////////////////////////////////////////////////////////
// CTryAgainView printing

BOOL CTryAgainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTryAgainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTryAgainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTryAgainView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTryAgainView diagnostics

#ifdef _DEBUG
void CTryAgainView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTryAgainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTryAgainDoc* CTryAgainView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTryAgainDoc)));
	return (CTryAgainDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTryAgainView message handlers

void CTryAgainView::UpdateRstSetting(RSTDTYPE m_rs)
{
	//convert m_rs for display
	UpdateData();
	m_bUse1=m_rs.bUse[0];
	m_bUse2=m_rs.bUse[1];
	m_bUse3=m_rs.bUse[2];
	m_bUse4=m_rs.bUse[3];
	m_bUse5=m_rs.bUse[4];
	m_bUse6=m_rs.bUse[5];
	m_bUse7=m_rs.bUse[6];

	m_dAlpha1=m_rs.dAlpha[0];
	m_dAlpha2=m_rs.dAlpha[1];
	m_dAlpha3=m_rs.dAlpha[2];
	m_dAlpha4=m_rs.dAlpha[3];
	m_dAlpha5=m_rs.dAlpha[4];
	m_dAlpha6=m_rs.dAlpha[5];
	m_dAlpha7=m_rs.dAlpha[6];

	m_dBeta1 = m_rs.dBeta[0];
	m_dBeta2 = m_rs.dBeta[1];
	m_dBeta3 = m_rs.dBeta[2];
	m_dBeta4 = m_rs.dBeta[3];
	m_dBeta5 = m_rs.dBeta[4];
	m_dBeta6 = m_rs.dBeta[5];
	m_dBeta7 = m_rs.dBeta[6];

	m_dValue1 = m_rs.dValue[0];
	m_dValue2 = m_rs.dValue[1];
	m_dValue3 = m_rs.dValue[2];
	m_dValue4 = m_rs.dValue[3];
	m_dValue5 = m_rs.dValue[4];
	m_dValue6 = m_rs.dValue[5];
	m_dValue7 = m_rs.dValue[6];

	GetDlgItem(IDC_STATICTITLE)->SetWindowText(_T("电阻方案-")+m_rs.sTitle);
	UpdateData(FALSE);
	this->Invalidate();
}

RSTDTYPE CTryAgainView::RetrieveRstSetting()
{
	RSTDTYPE m_rs;
	UpdateData();
	m_rs = m_rset.GetCurRstd();
	m_rs.bUse[0]=m_bUse1;
	m_rs.bUse[1]=m_bUse2;
	m_rs.bUse[2]=m_bUse3;
	m_rs.bUse[3]=m_bUse4;
	m_rs.bUse[4]=m_bUse5;
	m_rs.bUse[5]=m_bUse6;
	m_rs.bUse[6]=m_bUse7;

	m_rs.dAlpha[0]=m_dAlpha1;
	m_rs.dAlpha[1]=m_dAlpha2;
	m_rs.dAlpha[2]=m_dAlpha3;
	m_rs.dAlpha[3]=m_dAlpha4;
	m_rs.dAlpha[4]=m_dAlpha5;
	m_rs.dAlpha[5]=m_dAlpha6;
	m_rs.dAlpha[6]=m_dAlpha7;

	m_rs.dBeta[0]=m_dBeta1;
	m_rs.dBeta[1]=m_dBeta2;
	m_rs.dBeta[2]=m_dBeta3;
	m_rs.dBeta[3]=m_dBeta4;
	m_rs.dBeta[4]=m_dBeta5;
	m_rs.dBeta[5]=m_dBeta6;
	m_rs.dBeta[6]=m_dBeta7;

	m_rs.dValue[0]=m_dValue1;
	m_rs.dValue[1]=m_dValue2;
	m_rs.dValue[2]=m_dValue3;
	m_rs.dValue[3]=m_dValue4;
	m_rs.dValue[4]=m_dValue5;
	m_rs.dValue[5]=m_dValue6;
	m_rs.dValue[6]=m_dValue7;
	return m_rs;
}


void CTryAgainView::OnRstAdd() 
{
	// TODO: Add your control notification handler code here
	RSTDTYPE rt;
	rt = this->RetrieveRstSetting();
	m_rset.AddRstd(rt);	
	rt = m_rset.GetCurRstd();
	this->UpdateRstSetting(rt);
}

void CTryAgainView::OnRstDelete() 
{
	// TODO: Add your control notification handler code here
	RSTDTYPE rt;
	if(::AfxMessageBox("确定要删除当前设置吗?",IDOK) != IDOK)
		return;

	m_rset.DeleteCurRstd();
	rt = m_rset.GetCurRstd();
	this->UpdateRstSetting(rt);
}

void CTryAgainView::OnRstSelect() 
{
	// TODO: Add your control notification handler code here
	RSTDTYPE rt;
	m_rset.SelectRstd();
	rt = m_rset.GetCurRstd();
	this->UpdateRstSetting(rt);
}

void CTryAgainView::OnParmAdd() 
{
	// TODO: Add your control notification handler code here
	PARAMTYPE pt;
	pt = this->RetrieveParmSetting();
	m_pset.AddParam(pt);
	pt = m_pset.GetCurParam();
	this->UpdateParamSetting(pt);
}

void CTryAgainView::OnParmDelete() 
{
	// TODO: Add your control notification handler code here
	PARAMTYPE pt;
	if(::AfxMessageBox("确定要删除当前设置吗?",IDOK) != IDOK)
		return;
	m_pset.DeleteCurParm();
	pt = m_pset.GetCurParam();
	this->UpdateParamSetting(pt);
}

void CTryAgainView::OnParmSelect() 
{
	// TODO: Add your control notification handler code here
	PARAMTYPE pt;
	m_pset.SelectParam();
	pt = m_pset.GetCurParam();
	this->UpdateParamSetting(pt);
}

PARAMTYPE CTryAgainView::RetrieveParmSetting()
{
	PARAMTYPE pt;
	pt = m_pset.GetCurParam();
	UpdateData();
	pt.bSingle = m_bSingle;
	pt.dTemper = m_dTemper;
	pt.iDelay = m_iDelay;
	pt.iTimes = m_iTimes;
	pt.iLowest = m_iLowest.GetCurSel();
	pt.iRange = m_iRange.GetCurSel();
	return pt;
}

void CTryAgainView::UpdateParamSetting(PARAMTYPE pt)
{
	UpdateData();
	m_bSingle = pt.bSingle;
	m_dTemper = pt.dTemper;
	m_iDelay = pt.iDelay;
	m_iTimes = pt.iTimes;
	if((pt.iLowest < m_iLowest.GetCount())&&(pt.iLowest >= 0))
		m_iLowest.SetCurSel(pt.iLowest);
	if((pt.iRange < m_iRange.GetCount())&&(pt.iRange >= 0))
		m_iRange.SetCurSel(pt.iRange);
	GetDlgItem(IDC_STATICPARM)->SetWindowText(_T("参数方案-")+pt.sTitle);
	UpdateData(FALSE);
	Invalidate();
}

void CTryAgainView::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CFormView::OnClose();
}

void CTryAgainView::OnDestroy() 
{

	CFormView::OnDestroy();	
	// TODO: Add your message handler code here
}

void CTryAgainView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	UINT flag;
	SWISTATUS ss;
	CString str;
	bool bOk;
	bOk = true;
	if(nIDEvent == 4)
	{
		//refresh the webviewer
		KillTimer(4);
		keybd_event(VK_F5,0x3f,0,0);
		Sleep(10);
		keybd_event(VK_F5,0x3f,KEYEVENTF_KEYUP,0);
		return;
	}

	if (nIDEvent == 1)
	{
		KillTimer(1);

		GetDlgItem(IDC_PAUSE)->ShowWindow(SW_HIDE);
		if(m_State != BEIDLE)
		{
			if(m_iBase == 1)
				::AfxMessageBox("连接纳伏计失败");
			if(m_iBase == 2)
				::AfxMessageBox("连接控制板失败");
			if(m_iBase == 3)
				::AfxMessageBox("连接打印机失败");
			if(m_iBase == 4)
				::AfxMessageBox("未发现标准电阻设置");
			if(m_iBase == 5)
				::AfxMessageBox("未发现测量参数设置");
			if(m_iBase == 6)
				::AfxMessageBox("未发现测试程序");
			if(m_iBase == 7)
				::AfxMessageBox("本软件无法在本机上使用,请联系供应商");
				
			::AfxGetMainWnd()->PostMessage(WM_CLOSE);
		}
		::AfxGetMainWnd()->SetActiveWindow();
		SetTimer(2,30,NULL);
		return;
	}	

	if(nIDEvent == 2)
	{
		if(m_State == BEPAUSE)
			return;

		if(m_State == BEIDLE)
		{

			if(m_CheckCount++ < CHECKWAIT)
				return;
			m_CheckCount = 0;
#ifndef	NOOVERCHECK
			KillTimer(2);
			m_State = BEPAUSE;
			ss = m_Switch.GetSwitchStatus();
			if(ss.bOl1||ss.bOl2)
			{
				Sleep(500);
				if(m_Switch.CheckOverLoad() != 0)
				{
					Sleep(3000);
					if(m_Switch.CheckOverLoad() != 0)
						OverLoadHandle();
				}
			}
			else
			{
				str.Format("   RN:%c  RX:%c",ss.bKN[0]?'Y':'N',ss.bRn?'Y':'N');
				GetDlgItem(IDC_DISPLAY2)->SetWindowText(str);
			}
			m_State = BEIDLE;
			SetTimer(2,30,NULL);
#endif			
			return;
		}
		if(m_State != BETEST)
		{
			KillTimer(2);
			flag = m_State;
			m_State = BEPAUSE;
			this->OnEndTest(flag);
			SetTimer(2,30,NULL);
			m_CheckCount = 0;
			return;
		}
		if(CommandStep())
		{
			m_iCmdPos++;
			m_Progress.SetPos(m_iCmdPos);
			if(m_iCmdPos >= m_TotalLines)
				m_State = BEFINI;
		}
		else
		{
#ifndef	NOOVERCHECK
			if(m_Switch.CheckOverLoad() != 0)
			{
				OverLoadHandle();
			}
#endif
			m_State = BEERRO;
			if(m_iCmdPos >= m_TotalLines)
				m_State = BEFINI;
		}
	}
	//CFormView::OnTimer(nIDEvent);
}

int CTryAgainView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//Show the Startup Flash Window

	SetTimer(1,5,NULL);
	splash = new CSplashDlg();
	splash->Create(IDD_SPLASH);
	splash->ShowWindow(SW_SHOW);
	splash->UpdateWindow();
	splash->CenterWindow();
	return 0;
}

bool CTryAgainView::RuleCheck(CString sType)
{
	char DirUse[300];
	CString str,line;

	if(m_ps.iDelay < 0)
	{
		::AfxMessageBox("采样时间必须大于0秒");
		return false;
	}
	if((m_ps.iTimes < 1)||(m_ps.iTimes > 20))
	{
		::AfxMessageBox("采样次数必须在1，20之间");
		return false;
	}
	::GetCurrentDirectory(300,DirUse);
	str.Format("%s",DirUse);
	str+=_T("\\Data\\")+sType+_T(".prg");
	if(!csf.Open(LPCSTR(str),CFile::modeRead))
	{
		::AfxMessageBox("程序文件打开失败");
		return false;
	}
	int index=-1;
	while(csf.ReadString(line))
	{
		if(line.IsEmpty())
			continue;
		if(line.Compare("[l,check]") == 0)
		{
			index++;
			break;
		}
		index++;
	}
	if(index == -1)
	{
		::AfxMessageBox("程序文件已损坏");
		csf.Close();
		return false;
	}
	while(csf.ReadString(line))
	{
		if(line.IsEmpty())
			continue;
		if(line.Compare("[e,check]") == 0)
		{
			csf.Close();
			return true;
		}
		if(!HandleProgram(line))
			break;
	}
	csf.Close();
	return false;
}

bool CTryAgainView::CommandStep()
{
	if(!bOpen)
		return false;
	CString line;

//	if(!csf.ReadString(line))
//		return true;
	if(!ProgramAnalyse(line))
	{
		m_iCmdPos = m_TotalLines+1;
		return true;
	}

	m_State = BEPAUSE;

	if(!HandleProgram(line))
	{
//		::AfxMessageBox(line);
		m_State  = BETEST;
		return false;
	}
	m_iCmdPos = 0;
	if(m_State == BEPAUSE)
		m_State = BETEST;
	return true;
}

int CTryAgainView::CommandBegin(CString sType)
{
	char DirUse[300];
	CString str,line;
	int count,cmdc;
	if(bOpen)
		return true;
	::GetCurrentDirectory(300,DirUse);
	str.Format("%s",DirUse);
	str+=_T("\\Data\\")+sType+_T(".prg");
	if(!csf.Open(LPCSTR(str),CFile::modeRead))
	{
 TRACE0("failed to open file\n");
		return -1;
	}
	count=1;
	while(csf.ReadString(str))
	{
		if(str.Compare("[l,test]")==0)
		{
			count = 0;
			break;
		}
	}
	if(count != 0)
	{
		csf.Close();
		return 0;
	}
	count = 0;
	while(csf.ReadString(str))
	{
		if(str.Compare("[e,test]")==0)
		{
			cmdc = count;
			count = -1;
			break;
		}
		count++;
	}
	csf.Close();
	if(count != -1)	return 0;

	str.Format("%s",DirUse);
	str+=_T("\\Data\\")+sType+_T(".prg");

	csf.Open(LPCSTR(str),CFile::modeRead);
	while(csf.ReadString(str))
	{
		if(str.Compare("[l,test]")==0) break;
	}
	bOpen = true;

	if(m_iRetestID == -1)
	{
		m_Data.ResetRecord();
	}

	m_fun.RemoveAll();
	for(count =0;count<11;count++)
		m_fun.Add(_T(""));

	m_iStackDepth = 0;
	
	GetDlgItem(IDC_BEGINTEST)->ShowWindow(SW_HIDE);
	return cmdc;
}

void CTryAgainView::CommandFinish()
{
	csf.Close();
	bOpen = false;
	GetDlgItem(IDC_BEGINTEST)->ShowWindow(SW_NORMAL);
}

void CTryAgainView::OverLoadHandle()
{
#ifndef NOOVERCHECK
	SWISTATUS ss;
	m_State = BEPAUSE;
	ss = m_Switch.GetSwitchStatus();
	if(ss.bOl1)
	{
		m_Switch.DoSwi(_T("OST1]"));
		::AfxMessageBox("发生过载");
		m_Switch.DoAction(RMARK);
		m_State = BEERRO;
	}
	if(ss.bOl2)
	{
//		m_Switch.DoSwi(_T("OST1]"));
		::AfxMessageBox("扩展电流源发生过载");
		m_Switch.DoAction(RMARK);
		m_State = BEERRO;
	}
#endif
}
bool CTryAgainView::HandleProgram(CString line)
{
 /**
 *         the HandleProgram funtion is used to 
 *		the condition fittness .when fit send to HandleCommand
 *	        else just return true;
 *  	   format :[type,if,ifpos,ifvalue,parm]
 */
	char type;
	int	ipos,vpos;
	double  value;
	bool	bfit;
	CString Istr;
	RESDATA	rd;
	
	if(line.IsEmpty())
		return true;
	if(line.GetAt(0) != '[')   //illegal program line , just notation
		return true;
	line = line.Right(line.GetLength()-1);
	ExtractChar(line,type);
	if((type >= 'A')&&(type <= 'Z'))
		return true;
	ExtractString(line,Istr);
	ipos = RetrieveIndex(Istr)+1;
	ExtractInt(line,vpos);
	ExtractDouble(line,value);
	bfit = false;
	switch(ipos)
	{
	case 1:		//"YSIN"
		if(m_ps.bSingle) bfit = true;
		break;
	case 2:		//"NSIN"
		if(!m_ps.bSingle) bfit = true;
		break;
	case 3:		//"YRAN"
		if(m_ps.iRange == vpos) bfit = true;
		break;
	case 4:		//"NRAN"
		if(m_ps.iRange != vpos) bfit = true;
		break;
	case 5:		//"YRLS"
		if(m_ps.iLowest == vpos) bfit = true;
		break;
	case 6:		//"NRLS"
		if(m_ps.iLowest != vpos) bfit = true;
		break;
	case 7:		//"YBRN"
		if(m_rs.bUse[vpos]) bfit = true;
		break;
	case 8:		//"NBRN"
		if(!m_rs.bUse[vpos]) bfit = true;
		break;
	case 9:		//"YVAL"
		rd = m_Data.GetRecord(vpos+m_iBase);
		if(rd.rbValue)	bfit = true;
		break;
	case 10:	//"NVAL"
		rd = m_Data.GetRecord(vpos+m_iBase);
		if(!rd.rbValue)	bfit = true;
		break;
	case 11:	//"YRST"
		rd = m_Data.GetRecord(vpos+m_iBase);
		if(rd.rbResult)	bfit = true;
		break;
	case 12:	//"NRST"
		rd = m_Data.GetRecord(vpos+m_iBase);
		if(!rd.rbResult)	bfit = true;
		break;
	case 13:    //"EMPT"
		bfit = true;
		break;
	case 14:	//"YSCA"
		if(m_iScaledR0 == vpos)	bfit = true;
		break;
	case 15:	//"NSCA"
		if(m_iScaledR0 != vpos) bfit = true;
		break;
	case 16:	//"YRES"
		if(m_iRetestID != -1) bfit = true;
		break;
	case 17:	//"NRES"
		if(m_iRetestID == -1) bfit = true;
		break;
	default:
		bfit = false;
		break;	
	}
	if(!bfit)
		return true;
	return HandleCommand(type,line);
}

bool CTryAgainView::HandleCommand(char type, CString Parm)
{
	CString str,str1;
	char  sParm[300];
	double data[20];
	int count;
	int max,min,lines;
	int left,right;
	char oper,babs,vzl,vzr;
	RESDATA rd;
	double sum;
	SYSTEMTIME syt;
	switch(type)
	{
	case 'm':	//show the message
		m_State = BEPAUSE;
		AfxMessageBox(Parm.Left(Parm.GetLength()-1));
		m_State = BETEST;
		break;
	case 's':	//turn the switch
		GetDlgItem(IDC_DISPLAY)->SetWindowText(_T("开关切换"));
		strcpy(sParm,(LPCSTR)Parm);
		return m_Switch.DoSwi(sParm);
	case 't':	//measure and retrieve the value 
		GetDlgItem(IDC_DISPLAY)->SetWindowText(_T("取得读数"));
		ExtractInt(Parm,lines);
		count = 0;
		sum = 0;
		while(count < m_ps.iTimes)
		{
			max = 0;
#ifndef NOSWITCH
			while(max++ < (m_ps.iDelay*100/m_ps.iTimes))
			{
				Sleep(10);
				DoEvents();
				while(m_State == BEIDLE)
				{
					DoEvents();
				};
				if(m_State == BEERRO)
					return false;
			}
#endif
			if(m_Navmeter.SendCommand(NAV_READ))
			{
				//sum += m_Navmeter.value;
				data[count++]=m_Navmeter.value;
			}
			else
			{
				if(!m_Navmeter.SendCommand(NAV_INIT))
				{
					TRACE0("Command -GET_VALUE FAILED\n");
					return false;
				}
				::Sleep(50);
				if(m_Navmeter.SendCommand(NAV_READ))
				{
					//sum += m_Navmeter.value;
					data[count++]=m_Navmeter.value;
				}
				else
				{
					TRACE0("Command -GET_VALUE FAILED\n");
					return false;
				}
			}
		}
		//sum = sum/m_ps.iTimes;
		sum=GetAverage(data,m_ps.iTimes);
		rd = m_Data.GetRecord(m_iBase+lines);
		rd.rbValue = true;
		rd.rValue = sum;
		m_Data.SetRecord(m_iBase+lines,rd);
		break;
	case 'j':	//jump to mark lines [j,MARK];如果条件满足，跳到MARK行
		ExtractString(Parm,str1);
		str1 = '['+str1+']';
		//while(csf.ReadString(str))
		while(ProgramAnalyse(str))
		{
			m_iCmdPos++;
			if(str == str1)
				return true;
		
		}
		return false;
	case 'r'://[r,if,if_value,min,max,MARK];
			 //jump to the MARK if retest value is out vb &ve
		if(m_iRetestID == -1)
			return true;
		ExtractInt(Parm,min);
		ExtractInt(Parm,max);

		if((m_iRetestID >= min)&&(m_iRetestID <= max))
			return true;
		ExtractString(Parm,str1);
		str1 = _T("[")+str1+_T("]");
		//while(csf.ReadString(str))
		while(ProgramAnalyse(str))
		{
			m_iCmdPos++;
			if(str == str1)
				return true;
		}
		return false;
	case 'c'://[c,if,if_value,LEFT,lpos,OP,RIGHT,rpos,abs,ID];caculate(LEFT OP RIGHT and store to ID)
		ExtractChar(Parm,vzl);
		ExtractInt(Parm,left);
		ExtractChar(Parm,oper);
		ExtractChar(Parm,vzr);
		ExtractInt(Parm,right);
		ExtractChar(Parm,babs);
		ExtractInt(Parm,count);
		if((vzl == 'Z')||(vzl == 'V'))
			if(left >= 0)	
				left += m_iBase;
			else
				left = -left;

		if((vzr == 'Z')||(vzr == 'V'))
			if(right >=0)	
				right += m_iBase;
			else
				right = -right;

		if(count >= 0)	
			count += m_iBase;
		else
			count = -count;
		m_Data.OperateValue(vzl,vzr,oper,left,right,babs,count,m_rs,m_ps);
		break;
	case 'a'://[a,msg,MARK];ask the message and if no,jump to the mark position
		m_State = BEPAUSE;
		ExtractString(Parm,str);
		if(::AfxMessageBox(str,MB_YESNO) == IDYES) 
		{
			m_State = BETEST;
			return true;
		}
		ExtractString(Parm,str1);
		str1 = "["+str1+"]";
		//while(csf.ReadString(str))
		while(ProgramAnalyse(str))
		{
			m_iCmdPos++;
			if(str == str1)
			{
				m_State = BETEST;
				return true;
			}
		}
		m_State = BETEST;
		return false;
	case 'v'://[v,if,if_value,TYPE,value];set the type variable to the value
		ExtractChar(Parm,oper);
		ExtractInt(Parm,min);
		switch(oper)
		{
		case 'B':
			m_iBase = min;
			if(m_iBase != 0) //retest case
				m_Data.ResetRecord(m_iBase);
			break;
		case 'V':
			m_RangeV = min;
			break;
		case 'T':
			m_iDelayKT = min;
			break;
		case 'I':
			m_iDelayKI = min;
		default:
			break;
		}
		break;
	case 'n':		//[ID]Send command to navmeter	using ID 	
		ExtractInt(Parm,left);
		ExtractString(Parm,str);
		return m_Navmeter.SendCommand(left);
	case 'i':	    //[ID]Run the InternalRoutines (ID)
		ExtractChar(Parm,oper);
		return InternalRoutines(oper);
	case 'd': //control of the FlexGrid
		return HandleFlexGridData(Parm);
	case 'u':
		ExtractChar(Parm,oper);
		if(oper == 'r')
		{
			str=_T("");
			m_Grid.SetCol(0);
			str += m_Grid.GetText()+_T(" ");
			m_Grid.SetCol(1);
			str += m_Grid.GetText()+_T(" ");
			m_Grid.SetCol(2);
			str += m_Grid.GetText();
			GetDlgItem(IDC_HEADER)->SetWindowText(str);
		}
		if(oper == 't')
		{
			ExtractString(Parm,str);
			GetDlgItem(IDC_HEADER)->SetWindowText(str);
		}
		break;
	case 'p':
#ifdef NOSWITCH
		break;
#endif
		ExtractInt(Parm,min);
		if(min == -1)
			min = m_iDelayKT;
		if(min == -2)
			min = m_iDelayKI;
		max = 0;
		while(max++ < min*100)
		{
			Sleep(8);
			DoEvents();
			if((max%100) == 0)
			{
				str.Format("等待%i秒...",min-(max/100));
				GetDlgItem(IDC_DISPLAY)->SetWindowText(str);
			}
			while(m_State == BEIDLE)
			{
				DoEvents();
			};
			if(m_State == BEERRO)
				return false;
		}
		break;
#ifndef NOPRINTER
	case 'w':
		ExtractChar(Parm,oper);
		switch(oper)
		{
		case 'd':
			::GetLocalTime(&syt);
			str.Format("%i-%i-%i %i:%i",syt.wYear,syt.wMonth,syt.wDay,syt.wHour,syt.wMinute);
			break;
		case 't':
			ExtractString(Parm,str);
			str.Replace("\\n","\n");
			break;
		case 'v':
			ExtractInt(Parm,min);
			ExtractInt(Parm,max);
			rd = m_Data.GetRecord(min);
			if(rd.rbValue)
			{
				str = DoublePrecision(rd.rValue,max);
			}
			else
				str.Format("数据错误");
			break;
		case 'z':
			ExtractInt(Parm,min);
			ExtractInt(Parm,max);
			rd = m_Data.GetRecord(min+m_iBase);
			if(rd.rbResult)
				str = DoublePrecision(rd.rResult,max);
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
		m_State = BEPAUSE;
		ExtractChar(Parm,oper);
		ExtractInt(Parm,min);
		ExtractInt(Parm,max);
		ExtractString(Parm,str1);
		str.Format("数据错误");
		if(oper == 'v')
		{
			rd = m_Data.GetRecord(min);
			if(rd.rbValue)
				str = DoublePrecision(rd.rValue,max);
		}
		if(oper == 'z')
		{
			rd = m_Data.GetRecord(min+m_iBase);
			if(rd.rbResult)
				str = DoublePrecision(rd.rResult,max);
		}
		str1.Replace("#1",str);
		AfxMessageBox(str1);
		m_State = BETEST;
	default:
		break;
	}
	return true; //undefined command
}

bool CTryAgainView::InternalRoutines(char type)
{
	//double value;
	int i;
	int index;
	RSTDTYPE rt;
	RESDATA rd;
	double da,db,dc,dd;
	switch(type)
	{
	case '1':
		//check process used here is for standard resistor test
		//requiration: should and only one outer resistor used
		//			 : and resistor value should greater than 0
		index=0;
		for(i=0;i<7;i++)
			index += (m_rs.bUse[i])?1:0;
		if(index > 1)	::AfxMessageBox("注意：只能使用一个外标准电阻");
		if(index == 0)	::AfxMessageBox("注意：必须使用一个外标准电阻");
		if(index != 1)
			return false;		
		
		if(m_rs.bUse[0])
		{
			m_iScaledR0 = 0;
			if(m_rs.dValue[0] < 0.005)
				m_iScaledR0 = 1;
			if((m_rs.dValue[0] >= 0.005)&&(m_rs.dValue[0] < 0.05))
				m_iScaledR0 = 2;
			if((m_rs.dValue[0] >= 0.05)&&(m_rs.dValue[0] < 0.5))
				m_iScaledR0 = 3;
		}
		return true;
	case '2':
		//replace the checked value to the default one
		//checked one are stored in Z11-Z16
		rt = m_rset.GetDefault();
		for(i=1;i<7;i++)
		{
			rd = m_Data.GetRecord(10+i);
			if(rd.rbResult)
			{
				rt.dAlpha[i]=0;
				rt.dBeta[i]=0;
				rt.dValue[i]=rd.rResult;
			}	
		}
		m_rset.ModifyDefaultRst(rt);
		break;
	case '3':
		//check does the outer standard exist
		index=0;
		for(i=0;i<7;i++)
			index += (m_rs.bUse[i])?1:0;
		if(index == 0)
		{
			::AfxMessageBox(_T("请检查并选择至少一个外标准电阻"));
			return false;
		}
		break;
	case '4':
		//caculate the average and the standard different between r0
		rd = m_Data.GetRecord(10);
		db = rd.rResult;
		rd = m_Data.GetRecord(11);
		dc = rd.rResult;
		rd = m_Data.GetRecord(9);
		da = rd.rResult;
		rd.rResult = (da+db+dc)/3.0;
		m_Data.SetRecord(9,rd);

		rd = m_Data.GetRecord(10);
		
		if(da > db) 
		{
			dd =db;
			db =da;
			da =dd;
		}
		if(db > dc)
		{
			dd = dc;
			dc = db;
			db = dd;
		}
		rd.rResult = 300*(dc-da)/(da+db+dc);
		m_Data.SetRecord(10,rd);
		break;
	case '5':
		//copy the user set to the current setting
		//which is useful in check test case begin;
		m_rs = this->RetrieveRstSetting();
		break;
	case '6':
		//scale the Z8 to power of ten
		rd = m_Data.GetRecord(8);
		if(rd.rbResult)
		{
			rd.rResult=ConvertDataExp(rd.rResult)/100;
			m_Data.SetRecord(8,rd);
		}
		break;
	default:
		break;
	}
	return true;

}

bool CTryAgainView::ReplaceReportDefine(CString sType)
{
	CStdioFile Hcsf;
	char DirUse[300];
	CString line,str,sdata;
	int pos1,pos2,pos3,dindex,prec;
//	double value;
	RESDATA rd;
	::GetCurrentDirectory(300,DirUse);
	str.Format("%s",DirUse);
	str+=_T("\\Data\\")+sType+_T(".htm");
	if(!csf.Open(LPCSTR(str),CFile::modeRead))
		return false;

	strcat(DirUse,"\\Data\\temp.htm");
	if(!Hcsf.Open(DirUse,CFile::modeCreate|CFile::modeRead|CFile::modeWrite))
	{
		csf.Close();
		TRACE0("temp.htm create failed\n");
		return false;
	}

	while(csf.ReadString(line))
	{
		pos1=1;
		while(pos1 != -1)
		{
			pos1=line.Find("@@");
			if(pos1 == -1)
				continue;
			pos2 = line.Find("@@",pos1+2);
			if(pos2 == -1)
				continue;
			str=line.Mid(pos1+2,pos2-pos1-2);
			pos3 = str.Find('.',0);
			if(pos3 == -1)
				continue;
			dindex=atoi(str.Mid(1,pos3-1));
			prec = atoi(str.Right(str.GetLength()-pos3-1));
			switch(str.GetAt(0))
			{
			case 'V':
				rd = m_Data.GetRecord(dindex);
				if(rd.rbValue)
					sdata = DoublePrecision(rd.rValue,prec);
				else
					sdata.Format(" ----- ");
				break;
			case 'Z':
				rd = m_Data.GetRecord(dindex);
				if(rd.rbResult)
					sdata = DoublePrecision(rd.rResult,prec);
				else
					sdata.Format(" ----- ");
				break;
			default:
				sdata.Format("      ");
				break;
			}
			line.Replace("@@"+str+"@@",sdata);
		}
		Hcsf.WriteString(line+'\n');
	}
	csf.Close();
	Hcsf.Close();
	return true;
}

void CTryAgainView::IntallScheme()
{
	HANDLE hFinder; 
	LPWIN32_FIND_DATA lpFindFileData; 
	
	char DirName[300];
	char DirTemp[300];
	lpFindFileData  = new WIN32_FIND_DATA; 
	::GetCurrentDirectory(300,DirTemp);
	::GetCurrentDirectory(300,DirName);

	strcat(DirName,"\\Data");
	
	if(::SetCurrentDirectory(DirName) == FALSE)  // if can't find the dir 
	{
		  TRACE0("Failed to Locate the dir"); 
		  delete lpFindFileData;
		  return;
	} 
	CFile cf;
	hFinder = ::FindFirstFile("*.prg",  // pointer to name of file to search for 
					lpFindFileData );  // pointer to returned information 
	if(hFinder != INVALID_HANDLE_VALUE)
	{
		if( strcmp(lpFindFileData->cFileName,".") && strcmp(lpFindFileData->cFileName,"..") ) 
		{ 
			CString csTempDir; 
			csTempDir = lpFindFileData->cFileName; 
			if((lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) 
			{ 
				m_Scheme.AddString(csTempDir.Left(csTempDir.GetLength()-4));
			} 
		}
	}
	
	while(::FindNextFile(hFinder,lpFindFileData))  // process the catalog dir; 
	{ 
		if( !strcmp(lpFindFileData->cFileName,".") || !strcmp(lpFindFileData->cFileName,"..") ) 
			continue; 
		CString csTempDir; 
		csTempDir = lpFindFileData->cFileName; 
		if((lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) 
		{ 
			m_Scheme.AddString(csTempDir.Left(csTempDir.GetLength()-4));
		} 
	}	
	::SetCurrentDirectory(DirTemp);
	delete lpFindFileData;
}

void CTryAgainView::OnBeginTest() 
{
	// TODO: Add your control notification handler code here
	CString type;
	RSTDTYPE rt;
	if(m_State != BEIDLE)
		return;
	if(m_Scheme.GetCount() <= 0)
		return;
	m_Scheme.GetLBText(m_Scheme.GetCurSel(),type);
	m_RetestType = m_Scheme.GetCurSel();
	m_ps = RetrieveParmSetting();
	m_rs = RetrieveRstSetting();
	RstdSettingCombination();

	while(m_iGridLine > 1)
	{
		m_Grid.SetRow(m_iGridLine-1);
		m_Grid.SetCol(0);
		m_Grid.SetText(_T(""));
		m_Grid.SetCol(1);
		m_Grid.SetText(_T(""));
		m_Grid.SetCol(2);
		m_Grid.SetText(_T(""));
		m_iGridLine--;
	}
	m_Data.ResetRecord(0);
	if(!RuleCheck(type))
		return;
	m_TotalLines = CommandBegin(type);
	if(m_TotalLines==0)
	{
		::AfxMessageBox("程序文件已损坏");
		CommandFinish();
		return;
	}
	m_Progress.SetRange(1,m_TotalLines);
	m_Progress.SetStep(1);
	m_Progress.SetPos(1);
	m_Progress.ShowWindow(SW_NORMAL);
	GetDlgItem(IDC_PAUSE)->ShowWindow(SW_NORMAL);
	m_Grid.SetRows(m_RangeV);
	m_iRetestID = -1;
	m_iCmdPos = 1;
	m_iGridLine = 1;
	m_Switch.DoAction(RMARK);
	m_State = BETEST;
	return;
}

void CTryAgainView::OnEndTest(UINT flag)
{
	CString type;
	if(m_State == BEIDLE)
		return;

	m_State = BEPAUSE;

	m_Scheme.GetLBText(m_RetestType,type);

	CommandFinish();

	if(m_iRetestID != -1)
	{
		m_State = BEIDLE;
		return;
	}
	m_Progress.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PAUSE)->ShowWindow(SW_HIDE);
	if(flag == BEFINI)
	{
		if(!ReplaceReportDefine(type))
		{
			::AfxMessageBox("报告创建失败");
			GetDlgItem(IDC_DISPLAY)->SetWindowText(_T("报告创建失败"));
		}
		else
		{
			::AfxMessageBox("报告创建成功");
			GetDlgItem(IDC_DISPLAY)->SetWindowText(_T("报告创建成功"));
		}
	}
	else
	{
		::AfxMessageBox("测试出现错误");
		GetDlgItem(IDC_DISPLAY)->SetWindowText(_T("测试出现错误"));
	}
	m_State = BEIDLE;
}

void CTryAgainView::OnPause() 
{
	// TODO: Add your control notification handler code here
	if(m_State == BEPAUSE)
	{
		m_State = BEIDLE;
		if(::AfxMessageBox("按确定后继续测量。。。",MB_YESNO) == IDYES)
			m_State = BEPAUSE;
		else
			m_State = BEERRO;
	}
	if(m_State == BEIDLE)
	{
		m_State = BEPAUSE;
		if(::AfxMessageBox("按确定后继续测量。。。",MB_YESNO) == IDYES)
			m_State = BETEST;
		else
			m_State = BEERRO;
	}
}

BEGIN_EVENTSINK_MAP(CTryAgainView, CFormView)
    //{{AFX_EVENTSINK_MAP(CTryAgainView)
	ON_EVENT(CTryAgainView, IDC_MSFLEXGRID1, -601 /* DblClick */, OnRetestQuest, VTS_NONE)
	ON_EVENT(CTryAgainView, IDC_MSFLEXGRID1, 69 /* SelChange */, OnSelChangeMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTryAgainView::OnRetestQuest() 
{
	// TODO: Add your control notification handler code here
	UINT line;
	RESDATA rd;
	CString str,type;

	if(m_State != BEIDLE)
		return;

	if(m_iGridLine >= (m_RangeV-1))
		return;
	m_Scheme.GetLBText(m_RetestType,type);

	line = m_Grid.GetRowSel();
	
	if(m_iGridLine < 0)
		return;
	if(line >= (UINT)m_iGridLine)
		return;
	rd = m_Data.GetRecord(line);
	if(rd.rPos < 0)
		return;

	//m_Grid.SetRow(line);
	m_Grid.SetCol(0);
	m_Grid.SetRow(0);
	str = _T("对")+m_Grid.GetText()+_T(": ");
	m_Grid.SetRow(line);
	str = str+m_Grid.GetText()+_T("\n");
	m_Grid.SetCol(1);
	m_Grid.SetRow(0);
	str = str+m_Grid.GetText()+_T(": ");
	m_Grid.SetRow(line);
	str = str+m_Grid.GetText()+_T("\n的值进行重测？");

	if(::AfxMessageBox(str,MB_YESNO) == IDNO) 
		return;

	if(!RuleCheck(type))
		return;
	rd = m_Data.GetRecord(line);
	m_iRetestID = rd.rPos;
	CommandBegin(type);
	m_iCmdPos = 1;
	m_State = BETEST;
}

void CTryAgainView::OnSelChangeMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
}

void CTryAgainView::OnExitprogram() 
{
	// TODO: Add your command handler code here
}

void CTryAgainView::OnUpdateExitprogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

bool CTryAgainView::HandleFlexGridData(CString &line)
{
	//type = 'r','c','t','v','z','p'
	//={rowset(if(-1)added),columnset,textset,valueuse,resultuse,positioninFlexGrid}
	char type;
	int  num;
	CString str;
	RESDATA rd;
	ExtractChar(line,type);
	switch(type)
	{
	case 'r':
		ExtractInt(line,num);
		if(num > -1)
			m_Grid.SetRow(num);
		else
		{
			m_Grid.SetRow(m_iGridLine++);
		}
		break;
	case 'c':
		ExtractInt(line,num);
		m_Grid.SetCol(num-1);
		break;
	case 'd':
		ExtractInt(line,num);
		str.Format("%i",num);
		m_Grid.SetText(str);
		break;
	case 't':
		ExtractString(line,str);
		m_Grid.SetText(str);
		break;
	case 'v':
		ExtractInt(line,num);
		rd = m_Data.GetRecord(num);
		if(rd.rbValue)
			str = DoublePrecision(rd.rValue,9);
			//str.Format("%9f",rd.rValue);
		else
			str.Format("NA");
		m_Grid.SetText(str);
		break;
	case 'z':
		ExtractInt(line,num);
		rd = m_Data.GetRecord(num+m_iBase);
		if(rd.rbResult)
		{
			/*
			ExtractChar(line,type);
			switch(type)
			{
			case '1':
				str.Format("%.1f",rd.rResult);
				break;
			case '2':
				str.Format("%.2f",rd.rResult);
				break;
			case '3':
				str.Format("%.3f",rd.rResult);
				break;
			case '4':
				str.Format("%.4f",rd.rResult);
				break;
			case '5':
				str.Format("%.5f",rd.rResult);
				break;
			default:
				str.Format("%f",rd.rResult);
				break;
			}
			*/
			str = DoublePrecision(rd.rResult,9);
		}
		else
			str.Format("NA");
		m_Grid.SetText(str);
		break;
	case 'p':
		ExtractInt(line,num);
		rd = m_Data.GetRecord(m_iGridLine);
		rd.rPos = num+m_iBase;
		m_Data.SetRecord(m_iGridLine,rd);
		break;
	default:
		return false;
	}
	return true;
}

void CTryAgainView::RstdSettingCombination()
{
	RSTDTYPE rt;
	SWISTATUS ss;
	rt = m_rset.GetDefault();
	ss = m_Switch.GetSwitchStatus();
	//the RN0 always use the external standard value
	for(int i=1;i<7;i++)
	{
		//if the outer standard not pluged, then use the internal ones
		if(!ss.bKN[i-1])
		{
			m_rs.dAlpha[i] = rt.dAlpha[i];
			m_rs.dBeta[i] = rt.dBeta[i];
			m_rs.dValue[i] = rt.dValue[i];
		}
	}
}

void CTryAgainView::OnQuitProgram() 
{
	// TODO: Add your control notification handler code here
	if((m_State == BETEST)||(m_State == BEPAUSE))
		return;
	m_rset.StoreToFile();
	m_pset.StoreToFile();
	KillTimer(2);
	m_Switch.DoAction(RMARK);
	m_Switch.Close();
	m_Navmeter.Close();
#ifdef USEPRINTER
	m_WHPrinter.Close();
#endif
	::AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
}

void CTryAgainView::OnViewReport() 
{
	// TODO: Add your control notification handler code here
}

void CTryAgainView::OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	char DirUse[300];
	// TODO: Add your control notification handler code here
	if((m_Tab.GetCurSel() == 0)&&(bReport))
	{
		bReport = false;
		m_Web.SetVisible(FALSE);
		m_Web.SetWidth(1);
		GetDlgItem(IDC_STATICTITLE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_DISPLAY2)->ShowWindow(SW_NORMAL);
		this->Invalidate();
	}
	if((m_Tab.GetCurSel() == 1)&&(!bReport))
	{
		bReport = true;
		m_Web.SetVisible(TRUE);
		m_Web.SetWidth(m_WebWidth);
		GetDlgItem(IDC_STATICTITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DISPLAY2)->ShowWindow(SW_HIDE);
		this->Invalidate();
		GetDlgItem(IDC_EXPLORER1)->SetFocus();
		SetTimer(4,30,NULL);
	}
}

void CTryAgainView::OnClickTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTryAgainView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if(bReport)
		SetTimer(4,30,NULL);
}

void CTryAgainView::DoEvents()
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

double CTryAgainView::GetAverage(double *data, int count)
{
	double sum,vara;
	int length;
	int index,i;
	if(count == 1)
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

void CTryAgainView::OnReplace() 
{
	// TODO: Add your control notification handler code here
	int line;
	RESDATA rs;
	CString str;
	double value;
	if(m_iReplace == -1)
	{
		line = m_Grid.GetRowSel();
		if((line < 0)||(m_iGridLine < 0)||(line >= m_iGridLine))
			return;			
		rs = m_Data.GetRecord(line);
		if(rs.rPos >= 0)
			return;
		m_iReplace = line;
		GetDlgItem(IDC_REPLACE)->SetWindowText(_T("替换"));
		return;
	}
	if(m_iReplace > 0)
	{
		line = m_Grid.GetRowSel();
		if((line < 0)||(m_iGridLine < 0)||(line >= m_iGridLine))
			return;
		rs = m_Data.GetRecord(line);
		if(rs.rPos < 0)
			return;
		m_Grid.SetRow(m_iReplace);
		m_Grid.SetCol(2);
		str = m_Grid.GetText();
		value = atof(str);
		m_Grid.SetRow(line);
		m_Grid.SetCol(2);
		m_Grid.SetText(str);
		line = rs.rPos;
		rs = m_Data.GetRecord(rs.rPos);
		rs.rResult = value;
		m_Data.SetRecord(line,rs);

		m_Scheme.GetLBText(m_RetestType,str);
		if(!ReplaceReportDefine(str))
			::AfxMessageBox("报告创建失败");

		GetDlgItem(IDC_REPLACE)->SetWindowText(_T("选择/替换"));
		m_iReplace = -1;
		return;
	}
}

CString CTryAgainView::DoublePrecision(double value,int prec)
{
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

void CTryAgainView::OnSaveReport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog cfd(FALSE,_T("htm"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Html File(*.htm)|*.htm||"),NULL);
	CString str;
	char DirUse[300];

	::GetCurrentDirectory(300,DirUse);
	str.Format("%s",DirUse);
	str+=_T("\\Data\\temp.htm");

	if(cfd.DoModal() != IDOK)
	{
		::SetCurrentDirectory(DirUse);
		return;
	}

	if(!CopyFile(LPCSTR(str),LPCSTR(cfd.GetPathName()),TRUE))	
	{
		::AfxMessageBox("保存报告文件失败");
	}
	::SetCurrentDirectory(DirUse);
}

char kbuffer[100];
extern void RetrieveDriverID();

bool CTryAgainView::CheckDriveID()
{
	CString str,str1;
	char data[100];
	int i,j;
	unsigned char count;
	int length;

	//retrieve the license string
	char DirUse[300];
	::GetCurrentDirectory(300,DirUse);
	str.Format("%s",DirUse);
	str+=_T("\\Data\\Default\\license.txt");
	if(!csf.Open(LPCSTR(str),CFile::modeRead))
		return false;
	if(!csf.ReadString(str))
		return false;
	csf.Close();
	::SetCurrentDirectory(DirUse);

	RetrieveDriverID();
	length = strlen(kbuffer);
	while(length < 8)
	{
		kbuffer[length++]='1';
		kbuffer[length]='\0';
	}
	length = 8;
	for(j=0;j<8;j++)
	{
		count = 0;
		for(i=0;i<length;i++)
		{
			if(kbuffer[i]&(0x01<<j))
				count++;
		}
		count +=  kbuffer[j];
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

void CTryAgainView::OnDrawData() 
{
	// TODO: Add your control notification handler code here

	CGraphDlg cgd;
	CString str;

	int i;
	if(m_iGridLine < 2)
	{
		::AfxMessageBox("没有足够数据");
		return;
	}
	i= 1;
	m_Grid.SetCol(2);
	cgd.validcount = m_iGridLine-1;
	while(i<m_iGridLine)
	{
		if(i > 100) break;
		m_Grid.SetRow(i);
		cgd.data[i-1]=atof(m_Grid.GetText());
		i++;
	}
	cgd.DoModal();
}

bool CTryAgainView::ProgramAnalyse(CString &cmdLine,bool bUse)
{
	//here we use following structrue to add functionality
	// m_fun(0)="funname1@funpos1,funname2@funpos2..."
	// m_fun(1-10)="parameterstack(1-10)"
	CString line;
	CString str,str1,str2;
	int pos;
	int count;

	if(m_iStackDepth <= 0)
	{
		//not in any function
		if(!csf.ReadString(line))
			return false;
		if(line.GetLength() < 1)
		{
			cmdLine = _T("//");
			return true;
		}
		if(line.GetAt(0) == '<')//function declaration
		{
			pos=line.Find('>',0);
			str1.Format("%d",m_fun.GetSize());
			str = m_fun.GetAt(0);
			str += line.Mid(1,pos-1)+'@'+str1+',';
			m_fun.SetAt(0,str);
			while(csf.ReadString(line))
			{
				if(line.GetLength() < 1)
					continue;
				if(line.GetAt(0) == '>')
					break;
				m_fun.Add(line);
			}
			m_fun.Add(_T("><"));
			cmdLine = _T("//");
			return true;
		}
	}
	else
	{
		//stay in some function ,copy the parameter first
		str = m_fun.GetAt(m_iStackDepth);
		line = m_fun.GetAt(m_iFunpos[m_iStackDepth]);
		m_iFunpos[m_iStackDepth]+=1;
	
		if(line.Find('^',0) > 0)
		{
			count = 1;
			while(bUse)
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

	}
	
	if(line.GetLength() < 1)
	{
		cmdLine = _T("//");
		return true;
	}
	if(line.GetAt(0) == '{')//function calls
	{
		while(FunEnter(line,bUse));
	}
	else
	{
		if(line.GetAt(0) == '>')
			m_iStackDepth--;
	}
	cmdLine = line;		
	return true;
}

bool CTryAgainView::FunEnter(CString &cmdLine,bool bUse)
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

//	cmdLine=m_fun.GetAt(m_iFunpos[m_iStackDepth]);

	str = m_fun.GetAt(m_iStackDepth);
	line = m_fun.GetAt(m_iFunpos[m_iStackDepth]);
	m_iFunpos[m_iStackDepth] += 1;
	if(line.Find('^',0) > 0)
	{
		count = 1;
		while(bUse)
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





void CTryAgainView::OnOutofmemoryProgress1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

double CTryAgainView::ConvertDataExp(double input)
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
