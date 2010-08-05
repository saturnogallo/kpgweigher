#if !defined(AFX_LISTDIALOG_H__A7DB9676_6820_11D2_9149_E655B8000000__INCLUDED_)
#define AFX_LISTDIALOG_H__A7DB9676_6820_11D2_9149_E655B8000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPrintingDialog : public CDialog
{
	CString strTemp;
public:
	//{{AFX_DATA(CPrintingDialog)
	enum { IDD = AFX_IDD_PRINTDLG };
	//}}AFX_DATA
	CPrintingDialog::CPrintingDialog(CWnd* pParent, CPrintDialog& Pr, LPCTSTR Title);

	virtual ~CPrintingDialog() { }

	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	void PrintPageNum(int page);
};


// ListDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListDialog dialog

#define IDD_DIALOG2	2
#define IDC_SYSLISTVIEW1 102
#define IDC_IMPRIMIR 101

#include "DaoListCtrl.h"

class Recordset;

typedef bool (CWnd::* ConsFunc)(const char*);
typedef bool (CWnd::* ConsMFunc)(CDaoListCtrl*,int*,int);
typedef bool (CWnd::* AltaFunc)();
typedef bool (CWnd::* BajaFunc)(const char*);
typedef bool (CWnd::* BajaMFunc)(CDaoListCtrl*,int*,int);

class CListDialog : public CDialog
{
    ConsFunc Cons;
    ConsMFunc ConsM;
    AltaFunc Alta;
    BajaFunc Baja;
    BajaMFunc BajaM;
	CString caption;
	DaoWindow* Daw;
	void Insert();
	void Delete();

// Construction
public:
	CListDialog(Recordset& r, DaoWindow* d = 0, CWnd* pParent = NULL);   // standard constructor
	~CListDialog();
	void SetCaption(LPCSTR str);
	int* lencol;
	int indcol;
	int* totcol;
// Dialog Data
	//{{AFX_DATA(CListDialog)
	enum { IDD = IDD_DIALOG2 };
	CDaoListCtrl	m_Lista;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CListDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKeydownSyslistview1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnImprimir();
	afx_msg void OnDblclkSyslistview1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTDIALOG_H__A7DB9676_6820_11D2_9149_E655B8000000__INCLUDED_)
