#if !defined(AFX_REPORTVIEW_H__9F0A4BF1_B6BF_4263_84FA_142C9DAA226B__INCLUDED_)
#define AFX_REPORTVIEW_H__9F0A4BF1_B6BF_4263_84FA_142C9DAA226B__INCLUDED_
#include "TryAgainDoc.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportView html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CReportView : public CHtmlView
{
protected:
	CReportView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CReportView)

// html Data
public:
	//{{AFX_DATA(CReportView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CReportView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CReportView)
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTVIEW_H__9F0A4BF1_B6BF_4263_84FA_142C9DAA226B__INCLUDED_)
