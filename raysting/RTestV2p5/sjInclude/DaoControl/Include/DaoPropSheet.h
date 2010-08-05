#if !defined(AFX_DAOPROPSHEET_H__5826F27A_7422_11D2_916E_F46A46000000__INCLUDED_)
#define AFX_DAOPROPSHEET_H__5826F27A_7422_11D2_916E_F46A46000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DaoPropSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDaoPropSheet


class CDaoPropSheet : public CPropertySheet, public DaoWindow
{
	DECLARE_DYNAMIC(CDaoPropSheet)
	CButton button;
	bool isdirty;

// Construction
public:
	CDaoPropSheet(LPCTSTR pszCaption = 0, LPCSTR s = 0, bool a = false, CWnd* pParentWnd = NULL);

// Attributes
public:
	bool modif;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoPropSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	bool Execute();
	virtual ~CDaoPropSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDaoPropSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAOPROPSHEET_H__5826F27A_7422_11D2_916E_F46A46000000__INCLUDED_)
