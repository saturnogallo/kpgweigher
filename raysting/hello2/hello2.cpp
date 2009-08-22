// hello2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "hello2.h"
#include "hello2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHello2App

BEGIN_MESSAGE_MAP(CHello2App, CWinApp)
	//{{AFX_MSG_MAP(CHello2App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHello2App construction

CHello2App::CHello2App()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHello2App object

CHello2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CHello2App initialization

BOOL CHello2App::InitInstance()
{
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	free((void*)this->m_pszProfileName);  
	//   Change   the   name   of   the   .ini   file.  
	//   The   CWinApp   destructor   will   free   the   memory.  
	this->m_pszProfileName =_tcsdup(_T("\\ÄÚ²¿´æ´¢\\Profile.ini"));   
	
	CHello2Dlg dlg;
	m_pMainWnd = &dlg;
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
