// RITemp.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "RITempDialog.h"
#include "RunitemDialog.h"
#include "Setting.h"
#include "ProbeDialog.h"
#include "FilterDialog.h"
#include "SystemDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CString SjGetAppDirectory();
/////////////////////////////////////////////////////////////////////////////
// CRITempDialog dialog


CRITempDialog::CRITempDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRITempDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRITempDialog)
	m_filterid = _T("");
	m_probeid = _T("");
	m_systemid = _T("");
	//}}AFX_DATA_INIT
}


void CRITempDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRITempDialog)
	DDX_CBString(pDX, IDC_FILTER_ID, m_filterid);
	DDX_CBString(pDX, IDC_PROBE_ID, m_probeid);
	DDX_CBString(pDX, IDC_SYSTEM_ID, m_systemid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRITempDialog, CDialog)
	//{{AFX_MSG_MAP(CRITempDialog)
	ON_BN_CLICKED(IDC_BUTTON5, OnExit)
	ON_BN_CLICKED(IDC_TEMPBTN, OnToNextCfg)
	ON_CBN_EDITCHANGE(IDC_PROBE_ID, OnEditchangeProbeId)
	ON_CBN_EDITCHANGE(IDC_FILTER_ID, OnEditchangeFilterId)
	ON_CBN_EDITCHANGE(IDC_SYSTEM_ID, OnEditchangeSystemId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRITempDialog message handlers

void CRITempDialog::OnToNextCfg() 
{
	// TODO: Add your control notification handler code here
	//CRunitemDialog *rd = (CRunitemDialog*)this->GetParent();
	//rd->ToggleCFG(IDD_RIRESI_CFG);
}

void CRITempDialog::OnExit() 
{
	// TODO: Add extra cleanup here
}

BOOL CRITempDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CSetting<PROBECFG> prb;	
	CSetting<FILTERCFG> flt;
	CSetting<SYSTEMCFG> sys;
	prb.InstallFromFile(PRB_DEFINFO);
	flt.InstallFromFile(FLT_DEFINFO);
	sys.InstallFromFile(SYS_DEFINFO);

	AddCBItem(IDC_PROBE_ID,prb.GetTitleList('|'));	
	AddCBItem(IDC_SYSTEM_ID,sys.GetTitleList('|'));	
	AddCBItem(IDC_FILTER_ID,flt.GetTitleList('|'));	
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CRITempDialog::GetCFGSetting()
{
	UpdateData();
	CString def = CRunitemDialog::GetSingleSetting(SYS_DEFINFO,DEFAULT_RES);
	def.Replace(_T("u_"),_T("d_"));

	return def+_T("|")+\
		CRunitemDialog::GetSingleSetting(SYS_DEFINFO,m_systemid)+\
		CRunitemDialog::GetSingleSetting(FLT_DEFINFO,m_filterid)+\
		CRunitemDialog::GetSingleSetting(PRB_DEFINFO,m_probeid);

}

void CRITempDialog::OnEditchangeProbeId() 
{
	// TODO: Add your control notification handler code here
}

void CRITempDialog::OnEditchangeFilterId() 
{
	// TODO: Add your control notification handler code here
	
}

void CRITempDialog::OnEditchangeSystemId() 
{
	// TODO: Add your control notification handler code here
	
}
