#ifndef _TRYAGAIN_TESTER_H_
#define _TRYAGAIN_TESTER_H_

#include "stdafx.h"



/* Printer Switcher Navmeter Configuration */
#define NOPRINTER
#define NOSWITCH
#define NONAVMETER

#include "..\\CommonH&Libs\\SwiNav.h"
#include "TesterData.h"
#include "TesterProgram.h"
#include "TryDataDefine.h"
class CTester : public CDialog
{
public:
	CTester(CWnd*);
	~CTester();
	void Begin();
	void End(UINT flag);
	void Pause();
	void Continue();
	void ReplaceResult(int newpos);
	void Retest(int pos);
	bool Quit();
	
	static void PushCommand(char type, CString Parm);
public:
	bool m_bTestMode;
protected:
	// Generated message map functions
	//{{AFX_MSG(CTryData3Tester)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DoEvents();
	void OverLoadHandle();
	
	bool HandleCommand(char type, CString Parm);
	bool InternalRoutines(char index);
	bool HandleDataShow(CString line);
	bool ReplaceReportDefine();

	void SetHtmlText(CString title,CString content);

	int m_CheckTicks;

	CSwitch m_Switch;
	CNavmeter m_Navmeter;

	CWnd *m_pwnd;
	int m_iTotalLines;

private:

	bool ListenClipboard();
	void RstdSettingCombination(const RSTDTYPE ds,RSTDTYPE &rs);

	static char pushtype;
	static CString pushparam;
	CTesterProgram m_Program;
public:
	// //do single command depend on cmd string
	void SingleAction(CString cmd);
};

#endif




















