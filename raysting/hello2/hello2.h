// hello2.h : main header file for the HELLO2 application
//

#if !defined(AFX_HELLO2_H__F3A75458_2437_43AF_B5F6_D8687E0CBA78__INCLUDED_)
#define AFX_HELLO2_H__F3A75458_2437_43AF_B5F6_D8687E0CBA78__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHello2App:
// See hello2.cpp for the implementation of this class
//
#define MAX_NAME_CHARS	10
#define MAX_VALUE_CHARS	20
class RUNITEM
{
public:
	TCHAR sTitle[MAX_NAME_CHARS];
	double dReading;
	int iType;
	int iAlternate;
	int iTimes;
	TCHAR sChannel_r[MAX_NAME_CHARS];
	TCHAR sProbe_r[MAX_NAME_CHARS];
	TCHAR sSystem_r[MAX_NAME_CHARS];

	TCHAR sChannel_t[MAX_NAME_CHARS];
	TCHAR sProbe_t[MAX_NAME_CHARS];
	TCHAR sSystem_t[MAX_NAME_CHARS];
	TCHAR sFilter_t[MAX_NAME_CHARS];
	
	TCHAR sFmt_op1[MAX_NAME_CHARS];
	TCHAR sFmt_op[MAX_NAME_CHARS];
	TCHAR sFmt_op2[MAX_NAME_CHARS];
};

class SYSTEMCFG
{
public:
	TCHAR sTitle[MAX_NAME_CHARS];
	double d_1ohm;
	double d_10ohm;
	double d_100ohm;
	double d_1kohm;
	double d_10kohm;
	double d_alpha1;
	double d_alpha10;
	double d_alpha100;
	double d_alpha1k;
	double d_alpha10k;
	double d_beta1;
	double d_beta10;
	double d_beta100;
	double d_beta1k;
	double d_beta10k;
	float  d_tempnow;
};
class PROBECFG
{
public:
	TCHAR sTitle[MAX_NAME_CHARS];
	int  iReference;
	bool bSqtcurrent;
	TCHAR sSerial[MAX_NAME_CHARS];
	int  iConvert;
	bool b3woffset;
	double  dExtref;
	int  iCurrent;
	double sCoef[16];
};

class FILTERCFG
{
public:
	TCHAR sTitle[MAX_NAME_CHARS];
	double dThresh;
	int iInterval;
	int iIntegral;
	int iResponse;
	int iAverage;
	int iSpeed;
};

class MEMITEM
{
public:
	TCHAR sTitle[MAX_NAME_CHARS];
	TCHAR sValue[MAX_VALUE_CHARS];
};
class CHello2App : public CWinApp
{
public:
	CHello2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHello2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHello2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLO2_H__F3A75458_2437_43AF_B5F6_D8687E0CBA78__INCLUDED_)
