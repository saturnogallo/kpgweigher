// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__EF3F4E80_D819_4AD3_B4FE_4A9EAB6BC3E5__INCLUDED_)
#define AFX_MAINFRM_H__EF3F4E80_D819_4AD3_B4FE_4A9EAB6BC3E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ToggleTestView();
	void ToggleReportView();
	void EndFullScreen();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
//	CToolBar	m_wndFull;
//	CToolBar* m_pwndFullScreenBar;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnToggleReportView();
	afx_msg void OnToggleTestView();
	afx_msg void OnUpdateToggleleft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToggleright(CCmdUI* pCmdUI);
	afx_msg void OnExitProgram();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnFullScreen();
	afx_msg void OnToggleTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	

//	CSplitterWnd m_Split;
//	bool bReport;
	WINDOWPLACEMENT m_OldWndPlacement;
	BOOL m_bFullScreen;
	CRect m_FullScreenRect;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__EF3F4E80_D819_4AD3_B4FE_4A9EAB6BC3E5__INCLUDED_)
