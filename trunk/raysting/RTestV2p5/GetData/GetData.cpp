// GetData.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GetData.h"
#include "GetDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetDataApp

BEGIN_MESSAGE_MAP(CGetDataApp, CWinApp)
	//{{AFX_MSG_MAP(CGetDataApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetDataApp construction

CGetDataApp::CGetDataApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGetDataApp object

CGetDataApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGetDataApp initialization

BOOL CGetDataApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	AfxOleInit();
	
	CGetDataDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_sDataPath = this->m_lpCmdLine;
	
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在API中定义了吧，好象是128 
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	CString fullpath(exeFullPath);
	fullpath = fullpath.Left(fullpath.GetLength()-12);
//	::AfxMessageBox(fullpath);
	SetCurrentDirectory(fullpath);
	if(!dlg.m_sDataPath.IsEmpty())
		dlg.m_sDataPath += "\\";
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
