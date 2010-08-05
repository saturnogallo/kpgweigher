// ModifySerialDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ModifySerial.h"
#include "ModifySerialDlg.h"
#include ".\modifyserialdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModifySerialDlg 对话框



CModifySerialDlg::CModifySerialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifySerialDlg::IDD, pParent)
	, m_strKey(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModifySerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strKey);
}

BEGIN_MESSAGE_MAP(CModifySerialDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CModifySerialDlg 消息处理程序

BOOL CModifySerialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CModifySerialDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CModifySerialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CModifySerialDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CModifySerialDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString sAppFolder;
	char szPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	sAppFolder=szPath;
	int id=sAppFolder.ReverseFind('\\');
	CString oldpath = sAppFolder.Left(id);

	::WinExec(oldpath+"\\postrun.bat",SW_NORMAL);	
	::Sleep(3000);
	UpdateData();
	sAppFolder = sAppFolder.Left(id) + CString("\\Default\\License.txt");
	
	CStdioFile csf;
	if(csf.Open(sAppFolder,CFile::modeCreate|CFile::modeWrite))
	{
		csf.WriteString(m_strKey);
		csf.Close();
		OnOK();
	}else{
		::AfxMessageBox("填写产品序列号失败");
		OnBnClickedCancel();
	}

}

void CModifySerialDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
