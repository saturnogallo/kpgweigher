#if !defined(AFX_RTESTCHARTCTL_H__ADD15D05_4A80_417F_99CC_6F39F3F3A54D__INCLUDED_)
#define AFX_RTESTCHARTCTL_H__ADD15D05_4A80_417F_99CC_6F39F3F3A54D__INCLUDED_
#include "Graph.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RTestChartCtl.h : Declaration of the CRTestChartCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CRTestChartCtrl : See RTestChartCtl.cpp for implementation.

class CRTestChartCtrl : public COleControl
{
	DECLARE_DYNCREATE(CRTestChartCtrl)

// Constructor
public:
	CRTestChartCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTestChartCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CRTestChartCtrl();

	DECLARE_OLECREATE_EX(CRTestChartCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CRTestChartCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CRTestChartCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CRTestChartCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CRTestChartCtrl)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CRTestChartCtrl)
	afx_msg void SetAxis(double upper, double lower, double base, double criteria);
	afx_msg void AddData(double value);
	afx_msg void ClearAll();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CRTestChartCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	CGraph *testGraph;
	enum {
	//{{AFX_DISP_ID(CRTestChartCtrl)
	dispidSetAxis = 1L,
	dispidAddData = 2L,
	dispidClearAll = 3L,
	//}}AFX_DISP_ID
	};

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTESTCHARTCTL_H__ADD15D05_4A80_417F_99CC_6F39F3F3A54D__INCLUDED)
