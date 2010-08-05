// ResiDefaultSet.h : main header file for the RESIDEFAULTSET application
//

#if !defined(AFX_RESIDEFAULTSET_H__B52D1A82_9255_49A9_96DB_17BAC4B9C970__INCLUDED_)
#define AFX_RESIDEFAULTSET_H__B52D1A82_9255_49A9_96DB_17BAC4B9C970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CResiDefaultSetApp:
// See ResiDefaultSet.cpp for the implementation of this class
//

class CResiDefaultSetApp : public CWinApp
{
public:
	CResiDefaultSetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResiDefaultSetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CResiDefaultSetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIDEFAULTSET_H__B52D1A82_9255_49A9_96DB_17BAC4B9C970__INCLUDED_)
