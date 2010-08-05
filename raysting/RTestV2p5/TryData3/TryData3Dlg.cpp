// TryData3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "TryData3.h"
#include "TryData3Dlg.h"
#include "TryDataSql.h"
#include "Tester.h"
#include ".\trydata3dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DBG(x)		TRACE(x)	
extern CString SjGetAppDirectory();
extern CString SSearchDir(LPCTSTR rpath, LPCTSTR filetype);
extern const char* PathQuery(int pathid, int clsid);
CMapStringToString g_msTemp;
CString g_Path;
int bExit = 0;
extern CTester *g_Tester;
extern TESTERCONF	g_TesterConf;
static const char *mdbname = TEXT("test.mdb");
#define MAIN_URL	TEXT("\\Html\\main.htm")


void CopySTSMap(CMapStringToString& src,CMapStringToString& dst)
{
	if(!dst.IsEmpty())
		dst.RemoveAll();
	POSITION pos = src.GetStartPosition();
	while(pos != NULL)
	{
		CString skey,svalue;
		src.GetNextAssoc(pos,skey,svalue);
		dst.SetAt(skey,svalue);
	}
}
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
// CTryData3Dlg dialog
CTryData3Dlg::CTryData3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTryData3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTryData3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTryData3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTryData3Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_DHtml_CheckBox( pDX, L"cklt1", );
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTryData3Dlg, CDialog)
	//{{AFX_MSG_MAP(CTryData3Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_HTMLCTRL,OnHtmlCtrlNotify)
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTryData3Dlg message handlers

BOOL CTryData3Dlg::OnInitDialog()
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
	::AfxGetMainWnd()->ShowWindow(SW_HIDE);
	DBG("Before create htmlctrl");
	m_hc.CreateFromStatic(IDC_STATIC1,this);
	DBG("After create htmlctrl");
	m_hc.SetMessage(WM_HTMLCTRL);
	DBG("messaged");
	g_msTemp.RemoveAll();//clear the all the setting 
	DBG("removed");
	g_Path = SjGetAppDirectory();	
	if(!SqlInit(g_Path+TEXT("\\Data\\test.mdb")))
	{
		::AfxMessageBox("数据库文件错误");
		bExit = 1;
		this->PostMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	g_TesterConf.m_RetestType=PathQuery(CLSGROUP,IDTYPE);
	
	g_Tester = new CTester(this);
	g_Tester->Create(IDD_ABOUTBOX, this);

	//initialize the regform data
	m_Handler.Handler("reg","init","");
	m_Handler.Handler("test","init","");
	m_Handler.Handler("data","init","");
	m_Handler.Handler("reg","init","");

	g_Tester->ShowWindow(SW_HIDE);
	g_Tester->SendMessage(WM_TIMER,1,0);
	m_hc.Navigate(g_Path+MAIN_URL);
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CTryData3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTryData3Dlg::OnPaint() 
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
HCURSOR CTryData3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


LRESULT CTryData3Dlg::OnHtmlCtrlNotify(WPARAM wParam,LPARAM lParam)
{
	if(wParam  == 1){	//refresh case
		m_hc.Refresh2(0);
		return 0;
	}
	m_hc.SetURL("http:\\\\"+g_Path+MAIN_URL);

	//event case
	CString op;
	if(!m_hc.m_sMap.Lookup("cmdtype",op))
		return 0;
	
	//global commands dont have the format of normal commands
	if(op.Find('_',0) <= 0){	
		if(op.Compare("exit") == 0) {
			bExit = 1;
			CTesterProgram::State = BEERRO;
			this->PostMessage(WM_CLOSE,99,0);
			CTesterProgram::State = BEERRO;
			return 0;
		}
	}

	//cmdtype format = page_action
	CString spage,scmd;
	spage = op.Left(op.Find('_',0));
	op.Delete(0,spage.GetLength()+1);
	
	CopySTSMap(m_hc.m_sMap,g_msTemp);

	//scan all and retrieve the parameter if available,a stupid way?
	m_hc.m_sMap.Lookup(spage+_T("sz"),scmd);

	if(op == "update"){
			//decomposition the update case
			m_Handler.Handler(spage,"delete",scmd);
			CopySTSMap(m_hc.m_sMap,g_msTemp);
			op = "add";
	}
	if(!m_Handler.Handler(spage,op,scmd))
			return 0;//no refresh
	this->PostMessage(WM_HTMLCTRL,1,0);
	return 0;
}

// TODO: Add your control notification handler code here

void CTryData3Dlg::OnDestroy() 
{
	SqlQuit();
	POSITION pos = g_msTemp.GetStartPosition();
	while(pos != NULL)
	{
		CString skey,svalue;
		g_msTemp.GetNextAssoc(pos,skey,svalue);
		g_msTemp.RemoveKey(skey);
	}
	m_hc.DestroyWindow();
	g_Tester->DestroyWindow();
	delete g_Tester;
	g_Tester = NULL;
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}






















int CTryData3Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CTryData3Dlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

// //overwrite the onOK function
void CTryData3Dlg::OnOK(void)
{
	return;
}
void CTryData3Dlg::OnCancel()
{
	if (bExit)
		CDialog::OnCancel();
	return;
}
