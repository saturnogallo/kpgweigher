// ResiDefaultSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResiDefaultSet.h"
#include "ResiDefaultSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CResiDefaultSetDlg dialog

CResiDefaultSetDlg::CResiDefaultSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResiDefaultSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResiDefaultSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CResiDefaultSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResiDefaultSetDlg)
	DDX_Control(pDX, IDC_LIST1, m_Name);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResiDefaultSetDlg, CDialog)
	//{{AFX_MSG_MAP(CResiDefaultSetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SETDEFAULT, OnSetdefault)
	ON_BN_CLICKED(IDC_BUTTON2, OnDeleteCur)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResiDefaultSetDlg message handlers

BOOL CResiDefaultSetDlg::OnInitDialog()
{
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
	if(!m_rst.InstallFromFile())
	{
		::AfxMessageBox("电阻标准文件已损坏");
		this->PostMessage(WM_CLOSE,0,0);
	}
	UpdateList(m_rst.RetrieveTitleAll());
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CResiDefaultSetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CResiDefaultSetDlg::OnPaint() 
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
HCURSOR CResiDefaultSetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CResiDefaultSetDlg::OnSetdefault() 
{
	// TODO: Add your control notification handler code here
	if(m_Name.GetCurSel() >= 0)
	{
		m_rst.SetCurIndex(m_Name.GetCurSel());
		m_rs=m_rst.GetCurRstd();
		m_rst.ModifyDefaultRst(m_rs);
		m_rs.sTitle=_T("default");
		m_rst.ModifyOriDefaultRst(m_rs);
		UpdateList(m_rst.RetrieveTitleAll());
	}

}

void CResiDefaultSetDlg::OnDeleteCur() 
{
	// TODO: Add your control notification handler code here
	if(m_Name.GetCurSel() >= 0)
	{
		m_rst.SetCurIndex(m_Name.GetCurSel());
		m_rst.DeleteCurRstd();
		UpdateList(m_rst.RetrieveTitleAll());
	}
}

void CResiDefaultSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_rst.StoreToFile();
	CDialog::OnOK();
}

void CResiDefaultSetDlg::UpdateList(CString namelist)
{
	int pos,opos,count,spos;
	count = 0;
	pos = 0;
	opos = 0;
	while(m_Name.GetCount() > 0)
		m_Name.DeleteString(0);
	while(pos != -1)
	{
		pos = namelist.Find(_T("\n"),opos);
		if(pos == -1)
			break;
		if(count != 0)
			spos = m_Name.AddString(namelist.Mid(opos,pos-opos));
		else
			spos = m_Name.AddString(_T("default(")+namelist.Mid(opos,pos-opos)+_T(")"));
		m_Name.SetItemData(spos,count++);
		opos = pos+1;
	}
	UpdateData();
}
