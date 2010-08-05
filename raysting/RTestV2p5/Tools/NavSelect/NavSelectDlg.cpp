// NavSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NavSelect.h"
#include "NavSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
const char *dlldir = _T("\\Data\\Default\\Navdll\\");

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNavSelectDlg dialog

CNavSelectDlg::CNavSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNavSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNavSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNavSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNavSelectDlg)
	DDX_Control(pDX, IDC_LIST1, m_NavList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNavSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CNavSelectDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNavSelectDlg message handlers

BOOL CNavSelectDlg::OnInitDialog()
{
	char Dir[300];
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CString sAppFolder;
	char szPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	sAppFolder=szPath;
	int id=sAppFolder.ReverseFind('\\');
	sAppFolder = sAppFolder.Left(id) + CString("\\Data\\Default\\Navdll\\");
	SearchDir(sAppFolder.GetBuffer());
	if(m_NavList.GetCount() <= 0)
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	else
		m_NavList.SetCurSel(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNavSelectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNavSelectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNavSelectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNavSelectDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString fname;
	char curdir[300];
	char Dir[300];
	// TODO: Add extra validation here
	
	if(m_NavList.GetCurSel() >= 0)
	{
		CString sAppFolder;
		char szPath[MAX_PATH];
		::GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
		sAppFolder=szPath;
		int id=sAppFolder.ReverseFind('\\');
		sAppFolder = sAppFolder.Left(id);

		m_NavList.GetText(m_NavList.GetCurSel(),fname);

		DeleteFile(sAppFolder + CString("\\SwiNav.dll"));
		if(::CopyFile( sAppFolder + CString("\\Data\\Default\\Navdll\\")+fname,sAppFolder + CString("\\SwiNav.dll"),FALSE))
		{
			::AfxMessageBox("设定完成");
		}
		else
		{
			::AfxMessageBox("设定失败");
		}
	}
	
	CDialog::OnOK();
}

void CNavSelectDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CNavSelectDlg::SearchDir(LPSTR pszDir)
{
	WIN32_FIND_DATA ffd ;
	char szDirFile[1024];
	char total[1024];
	sprintf(szDirFile,"%s*",pszDir);
	HANDLE hFind = FindFirstFile(szDirFile,&ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		char szDir[1024];
		if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
			strcmp(ffd.cFileName,".") && 
			strcmp(ffd.cFileName,".."))
		{
			//确定找到一个目录并且不为 . 或 ..
			sprintf(szDir,"%s%s\\",pszDir,ffd.cFileName);
			SearchDir(szDir);
		}
		sprintf(total,"%s%s",pszDir,ffd.cFileName);
		while(FindNextFile(hFind,&ffd))
		{
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
				strcmp(ffd.cFileName,".") && 
				strcmp(ffd.cFileName,".."))
			{
				//确定找到一个目录并且不为 . 或 ..
				sprintf(szDir,"%s%s\\",pszDir,ffd.cFileName);
				//printf("%s\n",szDir);
				SearchDir(szDir);
			}
			sprintf(total,"%s%s",pszDir,ffd.cFileName);
			if(strcmp(ffd.cFileName,".."))
				m_NavList.AddString(ffd.cFileName);
		}
		FindClose(hFind);
	}
	
}

void CNavSelectDlg::OnDblclkList1() 
{
	// TODO: Add your control notification handler code here
	CString fname;
	char curdir[300];
	char Dir[300];
	// TODO: Add extra validation here
	
	if(m_NavList.GetCurSel() >= 0)
	{
		CString sAppFolder;
		char szPath[MAX_PATH];
		::GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
		sAppFolder=szPath;
		int id=sAppFolder.ReverseFind('\\');
		sAppFolder = sAppFolder.Left(id);

		m_NavList.GetText(m_NavList.GetCurSel(),fname);

		DeleteFile(sAppFolder + CString("\\SwiNav.dll"));
		if(::CopyFile( sAppFolder + CString("\\Data\\Default\\Navdll\\")+fname,sAppFolder + CString("\\SwiNav.dll"),FALSE))
		{
			::AfxMessageBox("设定完成");
		}
		else
		{
			::AfxMessageBox("设定失败");
		}
	}
	::PostQuitMessage(0);
}
