// ThermoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Hello2.h"
#include "ThermoDlg.h"
#include "RunitemDialog.h"
#include "filterDialog.h"
#include "Setting.h"

// CThermoDlg 对话框
extern CString SjGetAppDirectory();
IMPLEMENT_DYNAMIC(CThermoDlg, CDialog)
CThermoDlg::CThermoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThermoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThermoDlg)
	//}}AFX_DATA_INIT

}

CThermoDlg::~CThermoDlg()
{
}

void CThermoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThermoDlg)
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CThermoDlg, CDialog)
	ON_BN_CLICKED(IDC_THMONOW, OnToNextCfg)
	ON_BN_CLICKED(IDC_BUTTON5, OnExit)
END_MESSAGE_MAP()


// CThermoDlg 消息处理程序
BOOL CThermoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CThermoDlg::GetCFGSetting()
{
	return _T("");
}

void CThermoDlg::OnToNextCfg() 
{
}


void CThermoDlg::OnExit()
{
}
