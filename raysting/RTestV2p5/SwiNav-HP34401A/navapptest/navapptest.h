// navapptest.h : main header file for the NAVAPPTEST application
//

#if !defined(AFX_NAVAPPTEST_H__CAEEE00D_54AD_48D9_82BD_EEE47263ADA4__INCLUDED_)
#define AFX_NAVAPPTEST_H__CAEEE00D_54AD_48D9_82BD_EEE47263ADA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNavapptestApp:
// See navapptest.cpp for the implementation of this class
//

class CNavapptestApp : public CWinApp
{
public:
	CNavapptestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNavapptestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNavapptestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVAPPTEST_H__CAEEE00D_54AD_48D9_82BD_EEE47263ADA4__INCLUDED_)
