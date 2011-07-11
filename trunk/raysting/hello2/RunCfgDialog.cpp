// RunCfgDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "RunCfgDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunCfgDialog dialog


CRunCfgDialog::CRunCfgDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRunCfgDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunCfgDialog)
	m_times = 1;
	//}}AFX_DATA_INIT
}


void CRunCfgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunCfgDialog)
	DDX_Control(pDX, IDC_TIMESTAB, m_timestab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunCfgDialog, CDialog)
	//{{AFX_MSG_MAP(CRunCfgDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TIMESTAB, OnSelchangeTimeTab)
	ON_BN_CLICKED(IDC_BUTTON5, OnExit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunCfgDialog message handlers
#define UPDATE_TIME		CString time;	time.Format(_T("%i"),m_times);\
						GetDlgItem(IDC_TIMES)->SetWindowText(time)

const int const_time[] = {1,10,20,50,100};
void CRunCfgDialog::OnSelchangeTimeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_timestab.GetCurSel() < (sizeof(const_time)/sizeof(int)))
	{
		m_times = const_time[m_timestab.GetCurSel()];
		UpdateData(FALSE);
		UPDATE_TIME;
	}else{
		m_times = 604800;
		UpdateData(FALSE);
		GetDlgItem(IDC_TIMES)->SetWindowText(_T("连续"));
	}

	*pResult = 0;
}

void CRunCfgDialog::OnExit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	this->ShowWindow(SW_HIDE);
}

CString CRunCfgDialog::GetCFGSetting()
{
	CString ret;
	ret.Format(_T("times=%i|"),m_times);
	return ret;
}

BOOL CRunCfgDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i;
	// TODO: Add extra initialization here
	for(i=0;i<sizeof(const_time)/sizeof(int);i++)
	{
		CString time;
		time.Format(_T("%i"),const_time[i]);
		m_timestab.InsertItem(i,time);
	}
	m_timestab.InsertItem(i,_T("连续"));
	CSpinButtonCtrl *spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	spin->SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunCfgDialog::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_times -= pNMUpDown->iDelta;
	if(m_times < 0) m_times = 0;
	if(m_times > 604800) m_times = 604800;

	UpdateData(FALSE);
	UPDATE_TIME;	
	*pResult = 0;
}
