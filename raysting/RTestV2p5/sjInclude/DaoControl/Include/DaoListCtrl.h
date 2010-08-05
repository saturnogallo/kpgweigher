#if !defined(AFX_DAOLISTCTRL_H__A7DB9677_6820_11D2_9149_E655B8000000__INCLUDED_)
#define AFX_DAOLISTCTRL_H__A7DB9677_6820_11D2_9149_E655B8000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DaoListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDaoListCtrl window

class CDaoListCtrl : public CListCtrl, public Recordset
{
	CMenu ContextMenu;
	short count;
	LVITEM* item;
	short* type;
	int* lencol;
    int* totcol;
	int indcol;
// Construction
public:
	CDaoListCtrl();
	CDaoListCtrl(Recordset& r);

// Attributes
public:
	void Init(Recordset const & r);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDaoListCtrl();
	int Llenar();
	void Linea(int index);
	LPCSTR GetIndexText(int index);
	int Update(int index);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDaoListCtrl)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
	void DrawItemText(CDC* pDC, char* text, CRect rect, int nWidth, int nFormat);
	int GetSelIndexes(int* indexes, int maxCount) const;

	friend class CListDialog;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAOLISTCTRL_H__A7DB9677_6820_11D2_9149_E655B8000000__INCLUDED_)
