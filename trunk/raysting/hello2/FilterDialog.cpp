// FilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "hello2Dlg.h"
#include "FilterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString SjGetAppDirectory();
/////////////////////////////////////////////////////////////////////////////
// CFilterDialog dialog


CFilterDialog::CFilterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterDialog)
	m_integral = 0;
	m_interval = 0;
	m_name = _T("");
	m_response = 0;
	m_thresh = 0.0;
	//}}AFX_DATA_INIT
}


void CFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterDialog)
	DDX_Text(pDX, IDC_FLT_INTEGRAL, m_integral);
	DDV_MinMaxInt(pDX, m_integral, 0, 100);
	DDX_Text(pDX, IDC_FLT_INTERVAL, m_interval);
	DDV_MinMaxInt(pDX, m_interval, 0, 100);
	DDX_LBString(pDX, IDC_FLT_LIST, m_name);
	DDX_Text(pDX, IDC_FLT_RESPONSE, m_response);
	DDV_MinMaxInt(pDX, m_response, 0, 100);
	DDX_Text(pDX, IDC_FLT_THRESH, m_thresh);
	DDV_MinMaxDouble(pDX, m_thresh, 0., 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilterDialog, CDialog)
	//{{AFX_MSG_MAP(CFilterDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnModify)
	ON_BN_CLICKED(IDC_BUTTON3, OnAddScheme)
	ON_LBN_SELCHANGE(IDC_FLT_LIST, OnSelchangeFltList)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterDialog message handlers
#define DEFINFO		FLT_DEFINFO
#define IDC_CTRL	IDC_FLT_LIST
void CFilterDialog::OnModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SettingToCFG();
	m_setting.SaveParm(m_cfg);
	m_setting.StoreToFile(DEFINFO);		
	MyWriteProfileString(_T("cfg"),_T("cur_filter"),m_name);
	::AfxMessageBox(_T("ÐÞ¸Ä³É¹¦"));
}

void CFilterDialog::OnAddScheme() 
{
	// TODO: Add your control notification handler code here
	CString newname;
	CListBox *list = ((CListBox*)GetDlgItem(IDC_CTRL));
	UpdateData();
	newname.Format(_T("ÂË²¨Æ÷%d"),list->GetCount()+1);
	SettingToCFG();
	swscanf(newname,_T("%s"),m_cfg.sTitle);
	m_setting.SaveParm(m_cfg);
	m_setting.StoreToFile(DEFINFO);
	MyWriteProfileString(_T("cfg"),_T("cur_filter"),newname);	
	list->AddString(newname);
	UpdateData(FALSE);
}

void CFilterDialog::OnSelchangeFltList() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cfg = m_setting.SelectParm((LPCTSTR)m_name);
	MyWriteProfileString(_T("cfg"),_T("cur_filter"),m_name);
	CFGToSetting();
	UpdateData(FALSE);	
}

int CFilterDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CFilterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// TODO: Add extra initialization here
	m_setting.InstallFromFile(DEFINFO);			
	CString namelist = m_setting.GetTitleList('|');	
	if(namelist.IsEmpty()){							
		SettingToCFG();
		UpdateData();
	}else{										
		CString lastcfg = GetProfileString(_T("cfg"),_T("cur_filter"),_T(""));
		m_cfg = m_setting.SelectParm(namelist.Left(namelist.Find('|',0)));
		m_cfg = m_setting.SelectParm(lastcfg);
		CFGToSetting();
		while(namelist.Find('|',0) >= 0){

			CString item = namelist.Left(namelist.Find('|',0));
			((CListBox*)GetDlgItem(IDC_CTRL))->AddString(item);
			namelist.Delete(0,item.GetLength() + 1);

		}
		if(namelist.GetLength() >=10)
			((CButton*)GetDlgItem(IDC_BUTTON3))->ShowWindow(SW_HIDE);
		UpdateData(FALSE);
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData(FALSE);
	this->ShowWindow(SW_HIDE);
	CHello2Dlg *rd = (CHello2Dlg*)this->GetParent();
	rd->QuitCfgDialog();

}
