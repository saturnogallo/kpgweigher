// SystemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "hello2Dlg.h"
#include "SystemDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemDialog dialog
extern CString SjGetAppDirectory();

CSystemDialog::CSystemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemDialog)
	m_100ohm = 0.0;
	m_10kohm = 0.0;
	m_10ohm = 0.0;
	m_1kohm = 0.0;
	m_1ohm = 0.0;
	m_name = _T("");
	m_alpha1ohm = 0.0;
	m_alpha10ohm = 0.0;
	m_alpha100ohm = 0.0;
	m_alpha10kohm = 0.0;
	m_alpha1kohm = 0.0;
	m_beta1ohm = 0.0;
	m_beta10ohm = 0.0;
	m_beta100ohm = 0.0;
	m_beta10kohm = 0.0;
	m_beta1kohm = 0.0;
	m_tempnow = 0.0f;
	//}}AFX_DATA_INIT
}


void CSystemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemDialog)
	DDX_Text(pDX, IDC_100OHM, m_100ohm);
	DDV_MinMaxDouble(pDX, m_100ohm, 0., 1000.);
	DDX_Text(pDX, IDC_10kohm, m_10kohm);
	DDV_MinMaxDouble(pDX, m_10kohm, 0., 100000.);
	DDX_Text(pDX, IDC_10OHM, m_10ohm);
	DDV_MinMaxDouble(pDX, m_10ohm, 0., 100.);
	DDX_Text(pDX, IDC_1KOHM, m_1kohm);
	DDV_MinMaxDouble(pDX, m_1kohm, 0., 10000.);
	DDX_Text(pDX, IDC_1ohm, m_1ohm);
	DDV_MinMaxDouble(pDX, m_1ohm, 0., 10.);
	DDX_LBString(pDX, IDC_SYS_LIST, m_name);
	DDX_Text(pDX, IDC_ALPHA1, m_alpha1ohm);
	DDX_Text(pDX, IDC_ALPHA10, m_alpha10ohm);
	DDX_Text(pDX, IDC_ALPHA100, m_alpha100ohm);
	DDX_Text(pDX, IDC_ALPHA10K, m_alpha10kohm);
	DDX_Text(pDX, IDC_ALPHA1K, m_alpha1kohm);
	DDX_Text(pDX, IDC_BETA1, m_beta1ohm);
	DDX_Text(pDX, IDC_BETA10, m_beta10ohm);
	DDX_Text(pDX, IDC_BETA100, m_beta100ohm);
	DDX_Text(pDX, IDC_BETA10K, m_beta10kohm);
	DDX_Text(pDX, IDC_BETA1K, m_beta1kohm);
	DDX_Text(pDX, IDC_TEMPNOW, m_tempnow);
	DDV_MinMaxFloat(pDX, m_tempnow, -273.f, 10000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemDialog, CDialog)
	//{{AFX_MSG_MAP(CSystemDialog)
	ON_BN_CLICKED(IDC_BUTTON3, OnAddScheme)
	ON_BN_CLICKED(IDC_BUTTON1, OnModify)
	ON_LBN_SELCHANGE(IDC_SYS_LIST, OnSelchangeSysList)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SYS_CALIB_1ohm, OnSYSCALIB1ohm)
	ON_BN_CLICKED(IDC_SYS_CALIB_10ohm, OnSYSCALIB10ohm)
	ON_BN_CLICKED(IDC_SYS_CALIB_100ohm, OnSYSCALIB100ohm)
	ON_BN_CLICKED(IDC_SYS_CALIB_1k, OnSYSCALIB1k)
	ON_BN_CLICKED(IDC_SYS_CALIB_10kohm, OnSYSCALIB10kohm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemDialog message handlers
#define DEFINFO		SYS_DEFINFO
#define IDC_CTRL	IDC_SYS_LIST

void CSystemDialog::OnAddScheme() 
{
	// TODO: Add your control notification handler code here
	CString newname;
	CListBox *list = ((CListBox*)GetDlgItem(IDC_CTRL));
	UpdateData();
	newname.Format(_T("外标准设置%d"),list->GetCount()+1);
	SettingToCFG();
	swscanf(newname,_T("%s"),m_cfg.sTitle);
	m_setting.SaveParm(m_cfg);
	m_setting.StoreToFile(DEFINFO);
	list->AddString(newname);
	MyWriteProfileString(_T("cfg"),_T("cur_system"),newname);	
	UpdateData(FALSE);
}

void CSystemDialog::OnModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	SettingToCFG();
	m_setting.SaveParm(m_cfg);
	m_setting.StoreToFile(DEFINFO);	
	MyWriteProfileString(_T("cfg"),_T("cur_system"),m_name);
}

void CSystemDialog::OnSelchangeSysList() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cfg = m_setting.SelectParm((LPCTSTR)m_name);
	MyWriteProfileString(_T("cfg"),_T("cur_system"),m_name);
	CFGToSetting();
	UpdateData(FALSE);	
	UpdateCalibButton();
}

int CSystemDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CSystemDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_setting.InstallFromFile(DEFINFO);			
	CString namelist = m_setting.GetTitleList('|');	
	if(namelist.IsEmpty()){							
		SettingToCFG();
		UpdateData();
	}else{			
		if(namelist.Find(DEFAULT_RES) < 0)
		{
			SYSTEMCFG tp;
			tp.d_1ohm = 1;
			tp.d_10ohm = 10;
			tp.d_100ohm = 100;
			tp.d_1kohm = 1000;
			tp.d_10kohm = 10000;
			tp.d_alpha1 = 0;
			tp.d_alpha10 = 0;
			tp.d_alpha100 = 0;
			tp.d_alpha1k = 0;
			tp.d_alpha10k = 0;
			tp.d_tempnow = 25;
			tp.d_beta1 = 0;
			tp.d_beta10 = 0;
			tp.d_beta100 = 0;
			tp.d_beta1k = 0;
			tp.d_beta10k = 0;
			
			swprintf(tp.sTitle,DEFAULT_RES);
			m_setting.SaveParm(tp);
			m_setting.StoreToFile(DEFAULT_RES);
			namelist = m_setting.GetTitleList('|');
		}
		CString lastcfg = GetProfileString(_T("cfg"),_T("cur_system"),_T(""));
		m_cfg = m_setting.SelectParm(namelist.Left(namelist.Find('|',0)));
		m_cfg = m_setting.SelectParm(lastcfg);
		
		CFGToSetting();
		while(namelist.Find('|',0) >= 0){
			if(namelist.Find('|',0) > 0){
				CString item = namelist.Left(namelist.Find('|',0));
				((CListBox*)GetDlgItem(IDC_CTRL))->AddString(item);
				namelist.Delete(0,item.GetLength() + 1);
			}else{
				namelist.Delete(0,1);
			}
			
		}
		if(namelist.GetLength() >=10) //no more config when reach 10
			((CButton*)GetDlgItem(IDC_BUTTON3))->ShowWindow(SW_HIDE);
		UpdateData(FALSE);
		UpdateCalibButton();

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSystemDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData(FALSE);
	this->ShowWindow(SW_HIDE);
	CHello2Dlg *rd = (CHello2Dlg*)this->GetParent();
	rd->QuitCfgDialog();
}
void CSystemDialog::UpdateCalibButton()
{
	CString name;
	name.Format(_T("%s"),m_cfg.sTitle);
	UINT show;
	show = (name.Find(DEFAULT_RES) >= 0) ? SW_SHOWNORMAL : SW_HIDE;
	
	GetDlgItem(IDC_SYS_CALIB_1ohm)->ShowWindow(show);
	GetDlgItem(IDC_SYS_CALIB_10ohm)->ShowWindow(show);
	GetDlgItem(IDC_SYS_CALIB_100ohm)->ShowWindow(show);
	GetDlgItem(IDC_SYS_CALIB_1k)->ShowWindow(show);
	GetDlgItem(IDC_SYS_CALIB_10kohm)->ShowWindow(show);
}
void CSystemDialog::OnSYSCALIB1ohm() 
{
	// TODO: Add your control notification handler code here
	DoSysCalib(_T("syscalib1"),IDC_SYS_CALIB_1ohm);
}

void CSystemDialog::OnSYSCALIB10ohm() 
{
	// TODO: Add your control notification handler code here
	DoSysCalib(_T("syscalib10"),IDC_SYS_CALIB_10ohm);
}
void CSystemDialog::OnSYSCALIB100ohm() 
{
	// TODO: Add your control notification handler code here
	DoSysCalib(_T("syscalib100"),IDC_SYS_CALIB_100ohm);
}

void CSystemDialog::OnSYSCALIB1k() 
{
	// TODO: Add your control notification handler code here
	DoSysCalib(_T("syscalib1k"),IDC_SYS_CALIB_1k);
}

void CSystemDialog::OnSYSCALIB10kohm() 
{
	// TODO: Add your control notification handler code here
	DoSysCalib(_T("syscalib10k"),IDC_SYS_CALIB_10kohm);
}
void CSystemDialog::DoSysCalib(CString func,UINT BTNID)
{
	CButton *bt = (CButton*)GetDlgItem(BTNID);

	bt->EnableWindow(FALSE);


	CString	syscfg;
	SYSTEMCFG def=m_setting.SelectParm(DEFAULT_RES);

	UpdateData();
	syscfg.Format(_T("d_10k=%f|d_1k=%f|d_100=%f|d_10=%f|d_1=%f|u_10k=%f|u_1k=%f|u_100=%f|u_10=%f|u_1=%f|"),\
		def.d_10kohm,def.d_1kohm,def.d_100ohm,def.d_10ohm,def.d_1ohm,\
		m_10kohm,m_1kohm,m_100ohm,m_10ohm,m_1ohm);

	m_prg.InstallScheme(func);	
	m_prg.SetEnv(syscfg);

	while(m_prg.StepProgram())
	{
		bt->SetWindowText(m_prg.GetEnv(_T("status")));
		::Sleep(1000);
	}

	bt->SetWindowText(_T("校准"));
	bt->EnableWindow(TRUE);

	SYSTEMCFG tp;
	tp = m_setting.SelectParm(DEFAULT_RES);
	if(func == _T("syscalib1"))
		tp.d_1ohm = wcstod(m_prg.GetEnv(_T("SYSRES1")),NULL);
	if(func == _T("syscalib10"))
		tp.d_10ohm = wcstod(m_prg.GetEnv(_T("SYSRES10")),NULL);
	if(func == _T("syscalib100"))
		tp.d_100ohm = wcstod(m_prg.GetEnv(_T("SYSRES100")),NULL);
	if(func == _T("syscalib1k"))
		tp.d_1kohm = wcstod(m_prg.GetEnv(_T("SYSRES1K")),NULL);
	if(func == _T("syscalib10k"))
		tp.d_10kohm = wcstod(m_prg.GetEnv(_T("SYSRES10K")),NULL);



	m_setting.SaveParm(tp);
	m_setting.StoreToFile(DEFINFO);	
	m_cfg = m_setting.SelectParm(DEFAULT_RES);
	MyWriteProfileString(_T("cfg"),_T("cur_system"),DEFAULT_RES);
	CFGToSetting();
	UpdateData(FALSE);
}
