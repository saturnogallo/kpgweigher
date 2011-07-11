// hello2Dlg.h : header file
//

#if !defined(AFX_HELLO2DLG_H__145FE46F_6A44_4871_9193_E83949C99E97__INCLUDED_)
#define AFX_HELLO2DLG_H__145FE46F_6A44_4871_9193_E83949C99E97__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "Setting.h"
#include "ProbeDialog.h"
#include "SystemDialog.h"
#include "FilterDialog.h"
#include "RunitemDialog.h"
#include "KeyDialog.h"
#include "CoefDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CHello2Dlg 对话框
#define RWIN_MAX	2
#define CFGTYPE_NONE	0
#define CFGTYPE_BORE	1
#define CFGTYPE_THMO	2
#define CFGTYPE_RESI	3
#define CFGTYPE_CACU	4
class CHello2Dlg : public CDialog
{
// 构造
public:
	CHello2Dlg(CWnd* pParent = NULL);	// 标准构造函数
	CRunitemDialog *m_ridlgs[RWIN_MAX];
	bool m_pause;
	bool m_running;
	void QuitCfgDialog();
	void SwitchToCcfg(double *cc);
	void SwitchFromCcfg(double *abc_coef);
	void ShowCFG(CWnd* target);
	void MoveWindowTo(CWnd* wnd, int xoffset, int yoffset, UINT style);
	void SendCmd(char cmd);
	CTesterProgram *cur_prg;
	char thread_cmd;
// 对话框数据
	enum { IDD = IDD_HELLO2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:

	HICON m_hIcon;
	int curview;

	CProbeDialog *m_prbdlg;
	CKeyDialog	*m_keydlg;
	CCoefDialog *m_ccfg;

	CSystemDialog *m_sysdlg;
	CFilterDialog *m_fltdlg;
	
	
	void LoadCurrentView();
	void Refresh();

	// Generated message map functions
	//{{AFX_MSG(CHello2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowSysCFG();
	afx_msg void OnShowProbeCFG();
	afx_msg void OnShowFilterCFG();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnExit();
	afx_msg void OnSelchangeRuntab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnRun();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnLbnDblclkList3();
	afx_msg void OnProcessSel1();
	afx_msg void OnProcessSel2();
	afx_msg void OnProcessSel3();
	afx_msg void OnProcessSel4();
	afx_msg void OnProcessSel5();
	afx_msg void OnProcessSel6();
	afx_msg void OnProcessSel7();
	afx_msg void OnProcessSel8();
	afx_msg void OnProcessSel9();
	afx_msg void OnProcessSel10();
	afx_msg void OnProcessSel11();
	afx_msg void OnEnSetfocusRuncount();
	afx_msg void OnEnKillfocusRuncount();
	afx_msg void OnLbnDblclkList4();
	afx_msg void OnStnClickedRunGraph();
	afx_msg void OnStnDblclickRunGraph();
	afx_msg void OnBtnDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_cfgtab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	// //run times
	CListBox m_chlist;
private:
	int m_cfgtype;
	void OnProcessSel(int id);
	int m_cmdbase;
	int lastid;
public:
	CGraph m_graph;

	CListBox m_history;
	
	int m_runcount;

};
#endif
