// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "trydata3.h"
#include "WaitDlg.h"
#include "math.h"
#include "TesterCaculate.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog


CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitDlg)
	m_sMsg = _T("");
	m_dStatus = 0.0;
	//}}AFX_DATA_INIT
	m_iPos = 0;
	m_iStable = false;
}


void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitDlg)
	DDX_Text(pDX, IDC_MSG, m_sMsg);
	DDX_Text(pDX, IDC_STATUS, m_dStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
	//{{AFX_MSG_MAP(CWaitDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg message handlers

void CWaitDlg::PushData(double data)
{
	m_dStatus = data;
	m_iData[m_iPos++] = data;
	m_iPos = m_iPos % BULKSIZE;
	if(CTesterCaculate::CheckStable(m_iData,BULKSIZE,WAITSCALE)){
		switch(m_iStable) {
		case STAB_BEGIN:
			m_iStable = STAB_STAB1;
			break;
		case STAB_CHAOS:
			m_iStable = STAB_STAB2;
		default:
			break;
		}
	}else{
		switch(m_iStable) {
		case STAB_STAB1:
			m_iStable = STAB_CHAOS;
			break;
		case STAB_STAB2:
			m_iStable = STAB_QUIT;
		default:
			break;
		}
	}
	if(m_iStable == STAB_QUIT){
		this->EndDialog(0);
	}else{
		UpdateData(FALSE);
	}
}

void CWaitDlg::PushMsg(CString msg)
{
	m_sMsg = msg;
	UpdateData(FALSE);
}

