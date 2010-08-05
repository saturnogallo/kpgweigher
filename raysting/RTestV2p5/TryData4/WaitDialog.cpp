// WaitDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TryData4.h"
#include "WaitDialog.h"
#include ".\waitdialog.h"


// CWaitDialog 对话框

IMPLEMENT_DYNAMIC(CWaitDialog, CDialog)
CWaitDialog::CWaitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDialog::IDD, pParent)
	, m_sWaiting(_T(""))
{
	bQuit = false;
}

CWaitDialog::~CWaitDialog()
{
}

void CWaitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WAITTEXT, m_sWaiting);
}


BEGIN_MESSAGE_MAP(CWaitDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWaitDialog 消息处理程序
void CWaitDialog::OnClose()
{
	if(bQuit)
		CDialog::OnClose();
	else
		this->ShowWindow(SW_HIDE);
}
void CWaitDialog::OnCancel()
{
	if(bQuit)
		CDialog::OnCancel();
	else
		this->ShowWindow(SW_HIDE);
}

void CWaitDialog::OnDestroy()
{
	if(bQuit) 
		CDialog::OnDestroy();
	else
		this->ShowWindow(SW_HIDE);
}
void CWaitDialog::OnBnClickedOk()
{
	//TODO: 在此添加控件通知处理程序代码
	if(bQuit) 
		CDialog::OnOK();
	else
		this->ShowWindow(SW_HIDE);
}