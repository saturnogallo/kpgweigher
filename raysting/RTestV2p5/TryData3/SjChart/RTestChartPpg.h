#if !defined(AFX_RTESTCHARTPPG_H__43546A97_8977_4D7F_9B98_1A1D5F53A623__INCLUDED_)
#define AFX_RTESTCHARTPPG_H__43546A97_8977_4D7F_9B98_1A1D5F53A623__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RTestChartPpg.h : Declaration of the CRTestChartPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CRTestChartPropPage : See RTestChartPpg.cpp.cpp for implementation.

class CRTestChartPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRTestChartPropPage)
	DECLARE_OLECREATE_EX(CRTestChartPropPage)

// Constructor
public:
	CRTestChartPropPage();

// Dialog Data
	//{{AFX_DATA(CRTestChartPropPage)
	enum { IDD = IDD_PROPPAGE_RTESTCHART };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CRTestChartPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTESTCHARTPPG_H__43546A97_8977_4D7F_9B98_1A1D5F53A623__INCLUDED)
