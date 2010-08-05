#if !defined(AFX_WAITDLG_H__BCCCB66D_0472_42BD_B18A_6BBFA45FF8A3__INCLUDED_)
#define AFX_WAITDLG_H__BCCCB66D_0472_42BD_B18A_6BBFA45FF8A3__INCLUDED_
#include "resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog
#define  BULKSIZE	5
#define  WAITSCALE  0.001
//waveform
//234240000000433452311111111xxxxxx
//BEGIN  STAB1  CHAOS  STAB2 QUIT

#define  STAB_BEGIN	0
#define  STAB_STAB1	1
#define  STAB_CHAOS 2
#define  STAB_STAB2	3
#define  STAB_QUIT	4

class CWaitDlg : public CDialog
{
// Construction
public:
	bool IsClosed()	{return (m_iStable==STAB_QUIT);};
	void PushMsg(CString msg);
	void PushData(double data);
	CWaitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaitDlg)
	enum { IDD = IDD_WAITDLG };
	CString	m_sMsg;
	double	m_dStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaitDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	double m_iData[BULKSIZE];
	int m_iPos;//count for data pushed
	int m_iStable;//flag for data stablility
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITDLG_H__BCCCB66D_0472_42BD_B18A_6BBFA45FF8A3__INCLUDED_)
