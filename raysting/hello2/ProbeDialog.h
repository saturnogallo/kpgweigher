#if !defined(AFX_PROBEDIALOG_H__7845DAEF_C173_4FE2_9F10_666AC84C2200__INCLUDED_)
#define AFX_PROBEDIALOG_H__7845DAEF_C173_4FE2_9F10_666AC84C2200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProbeDialog dialog
#include "Setting.h"
#define PRB_DEFINFO	SjGetAppDirectory()+_T("\\ProbeInfo.def")


class CProbeDialog : public CDialog
{
// Construction
public:
	CProbeDialog(CWnd* pParent = NULL);   // standard constructor
	PROBECFG	m_cfg;
	static void GetCoef(CString prbid,double *out)
	{
		CSetting<PROBECFG> prbf;
		prbf.InstallFromFile(PRB_DEFINFO);
		PROBECFG prb=prbf.SelectParm(prbid);
		memcpy(out,prb.sCoef,sizeof(prb.sCoef));
		return;

	}
	void CFGToSetting(){
		m_name.Format(_T("%s"),m_cfg.sTitle);
		m_serial.Format(_T("%s"),m_cfg.sSerial);
		((CComboBox*)GetDlgItem(IDC_PRB_REFERENCE))->SetCurSel(m_cfg.iReference);
		m_sqrtcurr = m_cfg.bSqtcurrent;
		((CComboBox*)GetDlgItem(IDC_PRB_CONVERT))->SetCurSel(m_cfg.iConvert);
		m_3wire = m_cfg.b3woffset;
		((CComboBox*)GetDlgItem(IDC_PRB_RCURR))->SetCurSel(m_cfg.iCurrent);
		m_coef.Format(_T("%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f"),
			m_cfg.sCoef[0],m_cfg.sCoef[1],m_cfg.sCoef[2],m_cfg.sCoef[3],m_cfg.sCoef[4],\
			m_cfg.sCoef[5],m_cfg.sCoef[6],m_cfg.sCoef[7],m_cfg.sCoef[8],m_cfg.sCoef[9],\
			m_cfg.sCoef[10],m_cfg.sCoef[11],m_cfg.sCoef[12],m_cfg.sCoef[13],m_cfg.sCoef[14],\
			m_cfg.sCoef[15]);
		m_setting.SaveParm(m_cfg);
		m_setting.StoreToFile(PRB_DEFINFO);
		UpdateData(FALSE);
	};
// Dialog Data
	//{{AFX_DATA(CProbeDialog)
	enum { IDD = IDD_PROBE2_CFG };
	CString	m_coef;
	CString	m_convert;
	BOOL	m_3wire;
	CString	m_name;
	CString	m_current;
	CString	m_serial;
	CString	m_ref;
	BOOL	m_sqrtcurr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProbeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSetting<PROBECFG> m_setting;

	void SettingToCFG(){
		swprintf(m_cfg.sTitle,_T("%s"),m_name);
		swprintf(m_cfg.sSerial,_T("%s"),m_serial);
		m_cfg.iReference = ((CComboBox*)GetDlgItem(IDC_PRB_REFERENCE))->GetCurSel();
		m_cfg.bSqtcurrent = (m_sqrtcurr == 1);
		m_cfg.iConvert = ((CComboBox*)GetDlgItem(IDC_PRB_CONVERT))->GetCurSel();
		m_cfg.b3woffset = (m_3wire == 1);
		m_cfg.iCurrent = ((CComboBox*)GetDlgItem(IDC_PRB_RCURR))->GetCurSel();
		swscanf(m_coef,_T("%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f"),
			   &m_cfg.sCoef[0],&m_cfg.sCoef[1],&m_cfg.sCoef[2],&m_cfg.sCoef[3],&m_cfg.sCoef[4],\
			   &m_cfg.sCoef[5],&m_cfg.sCoef[6],&m_cfg.sCoef[7],&m_cfg.sCoef[8],&m_cfg.sCoef[9],\
			   &m_cfg.sCoef[10],&m_cfg.sCoef[11],&m_cfg.sCoef[12],&m_cfg.sCoef[13],&m_cfg.sCoef[14],\
			   &m_cfg.sCoef[15]);
	};

	// Generated message map functions
	//{{AFX_MSG(CProbeDialog)
	afx_msg void OnModify();
	afx_msg void OnAddScheme();
	afx_msg void OnSelchangePrbList();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSetcoef();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBEDIALOG_H__7845DAEF_C173_4FE2_9F10_666AC84C2200__INCLUDED_)
