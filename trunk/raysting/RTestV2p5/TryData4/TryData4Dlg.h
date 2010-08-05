// TryData4Dlg.h : 头文件
//
#pragma once
#include "stdafx.h"
#include "TryData4.h"
#include "..\\CommonH&Libs\\SwiNav.h"
#include "trydatadefine.h"
#include "TesterProgram.h"
#include "explorer1.h"
#include "ChartDlg.h"
#include "WaitDialog.h"
#define SPORT_NAVMETER   'n'
#define SPORT_SWITCH	 's'
#define CHECKWAIT		110//30*100ms

#define ELEM_TYPE	9

#define CLSMDB		1
#define CLSJS		2
#define CLSGROUP	3

#define IDREG		1
#define IDDATA		2 
#define IDJH		3 
#define IDREPORT	4 
#define IDTEST		5 
#define IDCONF		6 
#define IDTBR		7
#define IDWAIT		8
#define IDTYPE      9

#define CMD_IDLEHTML	0
#define CMD_CONTHTML	1
#define CMD_CONTHTML2	2
#define CMD_LOADHTML	3
#define CMD_INITHTML	4

#define IDLE	0
#define RUNNING	1
#define BEERROR 2
#define SUSPEND 3
#define QUITING 4
// CTryData4Dlg 对话框
class CTryData4Dlg : public CDHtmlDialog
{
// 构造
public:
	CTryData4Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRYDATA4_DIALOG, IDH = IDR_HTML1 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	HRESULT OnSchSelect(IHTMLElement *pElement);
	HRESULT OnSchAdd(IHTMLElement *pElement);
	HRESULT OnSchDelete(IHTMLElement *pElement);
	HRESULT OnSchUpdate(IHTMLElement *pElement);
	HRESULT OnFltidSelect(IHTMLElement *pElement);
	HRESULT OnFltvalSelect(IHTMLElement *pElement);
    HRESULT OnClickCmd(IHTMLElement *pElement);
	HRESULT OnClickOnce(IHTMLElement *pElement);
    HRESULT OnClickUrl(IHTMLElement *pElement);
	HRESULT OnFinishEdit(IHTMLElement *pElement);
	HRESULT OnReportSaveAs(IHTMLElement *pElement);
	HRESULT OnReportPrint(IHTMLElement *pElement);
	HRESULT OnReportOpenAll(IHTMLElement *pElement);
	HRESULT OnDataExport(IHTMLElement *pElement);
	HRESULT OnDataXiuzheng(IHTMLElement *pElement);
	HRESULT OnDataJiaohe(IHTMLElement *pElement);
    HRESULT OnMouseOverCmd(IHTMLElement *phtmlElement);
    HRESULT OnMouseOutCmd(IHTMLElement *phtmlElement);
    HRESULT OnMouseOverOnce(IHTMLElement *phtmlElement);
    HRESULT OnMouseOutOnce(IHTMLElement *phtmlElement);
    HRESULT OnMouseOverUrl(IHTMLElement *phtmlElement);
    HRESULT OnMouseOutUrl(IHTMLElement *phtmlElement);
	HRESULT STDMETHODCALLTYPE ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

	CComPtr<IHTMLElement> m_spCurrElement;
// 实现
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTryData3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	DECLARE_DHTML_EVENT_MAP()
public:	

	BSTR m_elemTypes[ELEM_TYPE]; // 0-edit 1-display 2-test 3-select

	inline void InitElementTypes(void)
	{
		m_elemTypes[0] = ::SysAllocString(L"href");
		m_elemTypes[1] = ::SysAllocString(L"value");
		m_elemTypes[2] = ::SysAllocString(L"id");
		m_elemTypes[3] = ::SysAllocString(L"title");
		m_elemTypes[4] = ::SysAllocString(L"innerText");
		m_elemTypes[5] = ::SysAllocString(L"className");
		m_elemTypes[6] = ::SysAllocString(L"innerHTML");
		m_elemTypes[7] = ::SysAllocString(L"prec");
		m_elemTypes[8] = ::SysAllocString(L"style");
	}

private:
	int m_CheckTicks; //ticks for checkoverload counting
	UINT m_State;
	CSwitch m_Switch;
	CNavmeter m_Navmeter;
	CTesterProgram m_Program;
	UINT m_currPage;
	UINT m_cmdStep;
	CChartDlg *m_pChartDlg;
	CWaitDialog *m_pWaitDlg;
	bool m_bAddData;
	// check the connection to switch board and navmeter
	bool CheckConnection(void);

	void Handler(CString op,CString &sRegSel,CString &sRegLst,bool silent=false);//default handler for pages
	void ReportHandler(CString op);

	void CollectValues();
	CString GenerateOptionHtml(CString list,CString def);
	CString GetAttribute(IHTMLElement* pElement, BSTR attr);
	HRESULT GetEventError(IHTMLElement* pElement, BSTR attr);
	void SetHtmlText(CString id,double dvalue);

	void UpdateConfSetting(bool StringToStruct);
	void DoEvents();
	bool HandleCommand(char type, CString Parm);
	bool InternalRoutines(char index);
	bool ExecuteClickAction(char oper, CString parm);
	bool HandleDataShow(CString line);
	void ModifyDefaultRstdSetting();
	CString PathQuery2(int pathid,int clsid);
	void DisableConfigDefault(); //check whether we need to disable edit for default nbz
	void ConvertNYR(); //convert jdrq, yxqz to n,y,r
	void Abort();
	CString m_sFltid;
	CString m_sFltval;
	CString m_sRegvals;
	CString m_sJhvals;
	CString m_sDatavals;
	CString m_sConfigvals;

	CString m_jhsel0;
	CString m_jhsel1;
	CString m_jhsel2;
	CString m_jhsel3;
	CString m_jhsel4;
	CString m_jhsel5;
	CString m_jhsel6;
	CString m_jhsel7;
	CString m_jhsel8;
	CString m_jhsel9;
	CString m_jhsel10;
	CString m_jhsel11;
	CString m_jhsel12;
	CString m_jhsel13;
	CString m_jhsel14;
	CString m_jhsel15;
	CString m_jhsel16;
	CString m_jhsel17;
	CString m_jhsel18;
	CString m_jhsel19;

	CString m_sRegSel;
	CString m_sRegLst;
	CString m_sRegUrl;
	
	CString m_sConfSel;
	CString m_sConfLst;

	CString m_sTestUrl;
	CString m_sTestLst;
	
	CString m_sJhSel;
	CString m_sJhLst;
	CString m_sJhUrl;
	
	CString m_sDataSel;
	CString m_sDataLst;

	CString m_sDataUrl;
	
	CString m_sReportSel;
	CString m_sReportLst;
	CString m_sReportUrl;
public:
	// Check the valid of license
	bool CheckLicense(void);
	// //report viewer
	CExplorer1 m_explorer;
};
