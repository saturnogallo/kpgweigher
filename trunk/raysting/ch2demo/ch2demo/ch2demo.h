// ch2demo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cch2demoApp:
// See ch2demo.cpp for the implementation of this class
//

class Cch2demoApp : public CWinApp
{
public:
	Cch2demoApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cch2demoApp theApp;