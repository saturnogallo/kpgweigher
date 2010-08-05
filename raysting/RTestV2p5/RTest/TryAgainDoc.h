// TryAgainDoc.h : interface of the CTryAgainDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRYAGAINDOC_H__04C96C0C_E83B_41E6_9EB6_55E1F1EEAC83__INCLUDED_)
#define AFX_TRYAGAINDOC_H__04C96C0C_E83B_41E6_9EB6_55E1F1EEAC83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTryAgainDoc : public CDocument
{
protected: // create from serialization only
	CTryAgainDoc();
	DECLARE_DYNCREATE(CTryAgainDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTryAgainDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTryAgainDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTryAgainDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRYAGAINDOC_H__04C96C0C_E83B_41E6_9EB6_55E1F1EEAC83__INCLUDED_)
