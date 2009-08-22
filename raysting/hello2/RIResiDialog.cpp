// RIResiDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "RIResiDialog.h"
#include "RunitemDialog.h"
#include "ProbeDialog.h"
#include "FilterDialog.h"
#include "SystemDialog.h"
#include "Setting.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString SjGetAppDirectory();
/////////////////////////////////////////////////////////////////////////////
// CRIResiDialog dialog


CRIResiDialog::CRIResiDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRIResiDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRIResiDialog)
	m_filterid = _T("");
	m_probeid = _T("");
	m_systemid = _T("");
	//}}AFX_DATA_INIT
}


void CRIResiDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRIResiDialog)
	DDX_CBString(pDX, IDC_FILTER_ID, m_filterid);
	DDX_CBString(pDX, IDC_PROBE_ID, m_probeid);
	DDX_CBString(pDX, IDC_SYSTEM_ID, m_systemid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRIResiDialog, CDialog)
	//{{AFX_MSG_MAP(CRIResiDialog)
	ON_BN_CLICKED(IDC_BUTTON5, OnExit)
	ON_BN_CLICKED(IDC_TEMPBTN, OnToNextCfg)
	ON_CBN_EDITCHANGE(IDC_PROBE_ID, OnEditchangeProbeId)
	ON_CBN_EDITCHANGE(IDC_FILTER_ID, OnEditchangeFilterId)
	ON_CBN_EDITCHANGE(IDC_SYSTEM_ID, OnEditchangeSystemId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRIResiDialog message handlers

void CRIResiDialog::OnExit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->ShowWindow(SW_HIDE);
}

BOOL CRIResiDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CSetting<PROBECFG> prb;	
	CSetting<FILTERCFG> flt;
	CSetting<SYSTEMCFG> sys;
	prb.InstallFromFile(PRB_DEFINFO);
	flt.InstallFromFile(FLT_DEFINFO);
	sys.InstallFromFile(SYS_DEFINFO);

	AddCBItem(IDC_PROBE_ID,prb.GetTitleList('|'));	
	AddCBItem(IDC_SYSTEM_ID,sys.GetTitleList('|'));	
	AddCBItem(IDC_FILTER_ID,flt.GetTitleList('|'));	
	// TODO: Add extra initialization here
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CRIResiDialog::GetCFGSetting()
{
	UpdateData();
	CString def = CRunitemDialog::GetSingleSetting(SYS_DEFINFO,DEFAULT_RES);
	def.Replace(_T("u_"),_T("d_"));

	return def+_T("|")+\
		CRunitemDialog::GetSingleSetting(SYS_DEFINFO,m_systemid)+\
		CRunitemDialog::GetSingleSetting(FLT_DEFINFO,m_filterid)+\
		CRunitemDialog::GetSingleSetting(PRB_DEFINFO,m_probeid);
}

void CRIResiDialog::OnToNextCfg() 
{
	// TODO: Add your control notification handler code here
}

void CRIResiDialog::OnEditchangeProbeId() 
{
	// TODO: Add your control notification handler code here
	
}

void CRIResiDialog::OnEditchangeFilterId() 
{
	// TODO: Add your control notification handler code here
	
}

void CRIResiDialog::OnEditchangeSystemId() 
{
	// TODO: Add your control notification handler code here
	
}
