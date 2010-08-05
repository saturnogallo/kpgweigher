// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TryData4.h"
#include "InputDlg.h"


// CInputDlg dialog

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)
CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
	, m_Message(_T(""))
	, m_Value(_T(""))
{
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_Message);
	DDX_Text(pDX, IDC_EDIT1, m_Value);
}

BOOL CInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_EDIT1)->SetFocus();
	return TRUE;
}
BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
END_MESSAGE_MAP()


// CInputDlg message handlers
