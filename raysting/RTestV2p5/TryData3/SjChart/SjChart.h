#if !defined(AFX_SJCHART_H__76B13EED_DB15_40F0_AF19_8241FEC0D578__INCLUDED_)
#define AFX_SJCHART_H__76B13EED_DB15_40F0_AF19_8241FEC0D578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SjChart.h : main header file for SJCHART.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSjChartApp : See SjChart.cpp for implementation.

class CSjChartApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SJCHART_H__76B13EED_DB15_40F0_AF19_8241FEC0D578__INCLUDED)
