// GetDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetData.h"
#include "GetDataDlg.h"
#include "Smart\\Quickmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
BOOL SjOutputData();

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
// CGetDataDlg dialog

CGetDataDlg::CGetDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetDataDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetDataDlg, CDialog)
	//{{AFX_MSG_MAP(CGetDataDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnGetAllData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetDataDlg message handlers

BOOL CGetDataDlg::OnInitDialog()
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
	OnGetAllData();
	PostMessage(WM_CLOSE,0,0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGetDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGetDataDlg::OnPaint() 
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
HCURSOR CGetDataDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/*******************************
*	We extract all the data from datafile(dfname).
*	which is accomplished by comparing  with 
*	templatefile (tfname)
*   We store the result into a Quickmap array
*
*	_SPECIAL_BASE_ID(int);
*	CQuickMap<double,int,300,false> zarray;
********************************/
_SPECIAL_BASE_ID(int);
CQuickMap<double,int,300,false> zarray;

bool SjGetAllData(LPCTSTR tfname,LPCTSTR dfname)
{
	CStdioFile	tfile,dfile;//template file and data file
	CString		tline,dline;
	
	if(!dfile.Open(dfname,CFile::modeRead))
		return false;
	if(!tfile.Open(tfname,CFile::modeRead)) 
	{
		dfile.Close();
		return false;
	}

	while(dfile.ReadString(dline)&&tfile.ReadString(tline))
	{
		int nStart = tline.Find("@@",0);
		int nEnd;
		while(nStart != -1)
		{
			int zindex;
			double zvalue;
			CString stmp;

			nEnd = tline.Find('.',nStart+2);
			if(nEnd == -1)	break;//illegal data format

			zindex = atoi(tline.Mid(nStart+3,nEnd-(nStart+3)));
			if(zindex == -1) break;//illegal data format
			
			stmp.Empty();
			nEnd = nStart;
			do
			{
				char ctmp= dline.GetAt(nEnd++);
				if(((ctmp > '9') || (ctmp < '0'))&&(ctmp != '.'))
					break;
				stmp += ctmp;
			}while(1);

			zvalue = (stmp.IsEmpty())?-1000:atof(stmp);

			zarray.Add(zvalue,zindex);

			nStart = tline.Find("@@",nStart+2);//find the matching one
			nStart = tline.Find("@@",nStart+2);//find the next one
		}
	}
	tfile.Close();
	dfile.Close();
	return true;
}
/*****************************
*	We check the type of Data\\temp.htm and
*	return the template file name such as
*	Data\\电阻箱**.htm if available
*******************************/
bool SjCheckType(CString &stype,CString spath)
{
	CStdioFile tfile;
	CString tline;

	if(!tfile.Open(spath + _T("Data\\temp.htm"),CFile::modeRead))
	{
		AfxMessageBox("未找到数据文件");
		return false;
	}
	while(tfile.ReadString(tline))
	{
		stype.Format("电阻箱快速测量");
		if(tline.Find(stype,0) != -1)
			break;
		stype.Format("电阻箱精密测量");
		if(tline.Find(stype,0) != -1)
			break;
		stype.Format("电阻箱一般测量");
		if(tline.Find(stype,0) != -1)
			break;
		stype.Format("标准电阻测量2");
		if(tline.Find(stype,0) != -1)
			break;
		stype.Format("标准电阻测量3");
		if(tline.Find(stype,0) != -1)
			break;
		stype.Format("标准电阻测量");
		if(tline.Find(stype,0) != -1)
			break;
		stype.Format("电桥测量");
		if(tline.Find(stype,0) != -1)
			break;
		
	}
	stype = spath + _T("Data\\")+stype+_T(".htm");
	return (tfile.ReadString(tline) != 0);
}

void CGetDataDlg::OnGetAllData() 
{
	// TODO: Add your control notification handler code here
	CString tfname("");
	if(SjCheckType(tfname,m_sDataPath))
		if(SjGetAllData(tfname,m_sDataPath + _T("Data\\temp.htm")))
		{	//add all the data to the access database
			if(zarray._ar.Items())
			{
				SjOutputData();	
			}
			zarray.DelAll();
			AfxMessageBox("数据导出成功");
		}
	    else
			AfxMessageBox("数据导出失败");
	else
		AfxMessageBox("数据模版失败");
}

/***************************************************
*
****************************************************/
BOOL SjOutputData()
{
	// 定义ADO连接、命令、记录集变量指针
	_ConnectionPtr	m_pConnection;
	m_pConnection.CreateInstance(__uuidof(Connection));	
	try                 
	{	
		// 打开本地Access库 输出数据.mdb
		m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=输出数据.mdb","","",adModeUnknown);
	}
	catch(_com_error e)
	{
		AfxMessageBox("数据库连接失败，确认<输出数据.mdb>是否在当前路径下!");
		return FALSE;
	} 

	_CommandPtr		m_pCommand;
	
	_RecordsetPtr	m_pRecordset;
	// 使用ADO创建数据库记录集
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	// 在ADO操作中建议语句中要常用try...catch()来捕获错误信息,因为它有时会经常出现一些想不到的错误。jingzhou xu
	try
	{
		m_pRecordset->Open("SELECT * FROM Table1", 	 // 查询DemoTable表中所有字段
			m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}
	_variant_t var;	
	
	try
	{
		int pos;
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		// 读入库中各字段并加入列表框中
		while(!m_pRecordset->adoEOF)
		{
			double value;
			
			var = m_pRecordset->GetCollect("位置");
			if(var.vt != VT_NULL)
				pos = short(var);
			pos = zarray.FindByID(pos);
			if(pos != -1)
				value = zarray[pos];
			else
				value = -1000.0;

			m_pRecordset->PutCollect("值",value);
			m_pRecordset->MoveNext();
		}
//		m_pRecordset->Update();
	}
	catch(_com_error *e)
	{
		TRACE(e->ErrorMessage());
	}
	
	if(m_pRecordset->State)
		m_pRecordset->Close();
	m_pRecordset = NULL;	
	// 关闭ADO连接状态
	if(m_pConnection->State)
		m_pConnection->Close();
	m_pConnection= NULL;
	return true;
}
