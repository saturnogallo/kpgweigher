// NavSelect.h : main header file for the NAVSELECT application
//

#if !defined(AFX_NAVSELECT_H__AE11A44C_AFD4_40E2_997C_DFB6B03E6F6C__INCLUDED_)
#define AFX_NAVSELECT_H__AE11A44C_AFD4_40E2_997C_DFB6B03E6F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNavSelectApp:
// See NavSelect.cpp for the implementation of this class
//

class CNavSelectApp : public CWinApp
{
public:
	CNavSelectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNavSelectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNavSelectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVSELECT_H__AE11A44C_AFD4_40E2_997C_DFB6B03E6F6C__INCLUDED_)
