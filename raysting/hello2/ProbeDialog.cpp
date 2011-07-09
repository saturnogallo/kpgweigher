// ProbeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "hello2Dlg.h"
#include "ProbeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeDialog dialog
extern CString SjGetAppDirectory();

CProbeDialog::CProbeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProbeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProbeDialog)
	m_coef = _T("");
	m_3wire = FALSE;
	m_name = _T("");
	m_serial = _T("");
	m_sqrtcurr = FALSE;
	//}}AFX_DATA_INIT
}


void CProbeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbeDialog)
	DDX_Text(pDX, IDC_PRB_COEF, m_coef);
	DDX_Check(pDX, IDC_PRB_3WIRE, m_3wire);
	DDX_LBString(pDX, IDC_PRB_LIST, m_name);
	DDX_Text(pDX, IDC_PRB_SERIAL, m_serial);
	DDV_MaxChars(pDX, m_serial, MAX_NAME_CHARS);
	DDX_Check(pDX, IDC_PRB_SQRTCURR, m_sqrtcurr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProbeDialog, CDialog)
	//{{AFX_MSG_MAP(CProbeDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnModify)
	ON_BN_CLICKED(IDC_BUTTON3, OnAddScheme)
	ON_LBN_SELCHANGE(IDC_PRB_LIST, OnSelchangePrbList)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SETCOEF, OnSetcoef)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#define DEFINFO		PRB_DEFINFO
#define IDC_CTRL	IDC_PRB_LIST
/////////////////////////////////////////////////////////////////////////////
// CProbeDialog message handlers
void CProbeDialog::OnModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SettingToCFG();
	m_setting.SaveParm(m_cfg);
	MyWriteProfileString(_T("cfg"),_T("cur_probe"),m_name);
	m_setting.StoreToFile(DEFINFO);	
}

void CProbeDialog::OnAddScheme() 
{
	// TODO: Add your control notification handler code here
	CString newname;
	CListBox *list = ((CListBox*)GetDlgItem(IDC_CTRL));
	UpdateData();
	newname.Format(_T("̽ͷ%d"),list->GetCount()+1);
	SettingToCFG();
	swscanf(newname,_T("%s"),m_cfg.sTitle);
	m_setting.SaveParm(m_cfg);
	m_setting.StoreToFile(DEFINFO);
	MyWriteProfileString(_T("cfg"),_T("cur_probe"),newname);	
	list->AddString(newname);
	UpdateData(FALSE);
}

void CProbeDialog::OnSelchangePrbList() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cfg = m_setting.SelectParm((LPCTSTR)m_name);
//	msg.Format(_T("%s,%i,%i"),m_name,m_cfg.iCurrent,m_cfg.iReference);
	UpdateData(FALSE);
	CFGToSetting();
}

int CProbeDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CProbeDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_setting.InstallFromFile(DEFINFO);			
	CString namelist = m_setting.GetTitleList('|');	
	if(namelist.IsEmpty()){							
		SettingToCFG();
		UpdateData();
	}else{											
		CString lastcfg = GetProfileString(_T("cfg"),_T("cur_probe"),_T(""));
		m_cfg = m_setting.SelectParm(namelist.Left(namelist.Find('|',0)));
		m_cfg = m_setting.SelectParm(lastcfg);

		CFGToSetting();

		while(namelist.Find('|',0) >= 0){
			CString item = namelist.Left(namelist.Find('|',0));
			((CListBox*)GetDlgItem(IDC_CTRL))->AddString(item);
			namelist.Delete(0,item.GetLength() + 1);
		}
		if(namelist.GetLength() >=10) //no more config when reach 10
			((CButton*)GetDlgItem(IDC_BUTTON3))->ShowWindow(SW_HIDE);
		UpdateData(FALSE);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProbeDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData(FALSE);
	this->ShowWindow(SW_HIDE);
	CHello2Dlg *rd = (CHello2Dlg*)this->GetParent();
	rd->QuitCfgDialog();
}

void CProbeDialog::OnSetcoef() 
{
	// TODO: Add your control notification handler code here
	CHello2Dlg *rd = (CHello2Dlg*)this->GetParent();
	rd->SwitchToCcfg(m_cfg.sCoef);
}
