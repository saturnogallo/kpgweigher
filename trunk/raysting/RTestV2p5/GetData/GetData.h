// GetData.h : main header file for the GETDATA application
//

#if !defined(AFX_GETDATA_H__06FE0DC9_BD70_48D0_B6AF_2DC258F71810__INCLUDED_)
#define AFX_GETDATA_H__06FE0DC9_BD70_48D0_B6AF_2DC258F71810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGetDataApp:
// See GetData.cpp for the implementation of this class
//

class CGetDataApp : public CWinApp
{
public:
	CGetDataApp();
	// 定义ADO连接变量指针
	_ConnectionPtr	m_pConnection;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetDataApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGetDataApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETDATA_H__06FE0DC9_BD70_48D0_B6AF_2DC258F71810__INCLUDED_)
