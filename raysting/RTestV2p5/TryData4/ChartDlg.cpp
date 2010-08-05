// ChartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TryData4.h"
#include "ChartDlg.h"
#include ".\chartdlg.h"
#include "math.h"

// CChartDlg dialog

CChartDlg::CChartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartDlg::IDD, pParent)
{
	m_dBase = 1.0;
	m_dVary = 0.0001;
	bQuit = false;		
}

CChartDlg::~CChartDlg()
{
}

void CChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Datasel);
	DDX_Control(pDX, IDC_RTESTCHARTCTRL1, m_Chart);
	DDX_Text(pDX, IDC_EDIT3, m_dVary);
	DDX_Text(pDX, IDC_EDIT2, m_dBase);
}

BOOL CChartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CChartDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedSelectAll)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_BN_CLICKED(ID_AUTOUPDATE, OnBnClickedAutoupdate)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT2, OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnEnChangeEdit3)
END_MESSAGE_MAP()


// CChartDlg message handlers

void CChartDlg::OnClose()
{
	if(bQuit)
		CDialog::OnClose();
	else
		this->ShowWindow(SW_HIDE);
}
void CChartDlg::OnCancel()
{
	if(bQuit)
		CDialog::OnCancel();
	else
		this->ShowWindow(SW_HIDE);
}
void CChartDlg::OnOK()
{
	if(bQuit) 
		CDialog::OnOK();
	else
		this->ShowWindow(SW_HIDE);
}
void CChartDlg::OnDestroy()
{
	if(bQuit) 
		CDialog::OnDestroy();
	else
		this->ShowWindow(SW_HIDE);
}
void CChartDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	this->DeleteAll();
	this->UpdateChart();
}

void CChartDlg::OnBnClickedSelectAll()
{
	// TODO: Add your control notification handler code here
	m_Datasel.SetSel(-1,1);
	this->UpdateChart();
}

void CChartDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
	UpdateChart();
}


void CChartDlg::UpdateChart()
{
	// Get the indexes of all the selected items.
	int nCount = m_Datasel.GetSelCount();
	if(nCount <= 0)
		return;
	UpdateData();
	m_Chart.ClearAll();
	m_Chart.SetAxis(m_dBase+5.0*m_dVary,m_dBase-5.0*m_dVary,m_dBase,m_dVary*4.0);
	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_Datasel.GetSelItems(nCount, aryListBoxSel.GetData()); 
	int nTotal = nCount;
	CString sCurr;
	while(nCount > 0)
	{
		m_Datasel.GetText(nTotal-nCount--,sCurr);
		m_Chart.AddData(atof(sCurr));

	}
}
void CChartDlg::AddData(CString newValue)
{
	m_Datasel.InsertString(0,newValue);
	m_Datasel.SetSel(0);
	UpdateChart();
}
void CChartDlg::DeleteAll()
{
	while(m_Datasel.GetCount() > 0)
	{
		m_Datasel.DeleteString(0);
	}
	UpdateChart();
}
void CChartDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateChart();
}

void CChartDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateChart();
}

void CChartDlg::OnBnClickedAutoupdate()
{
	// TODO: 在此添加控件通知处理程序代码
	//auto size the chart
	int nCount = m_Datasel.GetSelCount();
	if(nCount <= 0)
		return;
	CArray<int,int> aryListBoxSel;
	aryListBoxSel.SetSize(nCount);
	m_Datasel.GetSelItems(nCount, aryListBoxSel.GetData()); 

	int nTotal = nCount;
	CString sCurr;
	double new_base=0;
	
	while(nCount > 0)
	{
		m_Datasel.GetText(nTotal-nCount--,sCurr);
		new_base += atof(sCurr);
	}
	new_base = new_base/nTotal;

	nCount = nTotal;
	double new_vary=0;
	while(nCount > 0)
	{
		m_Datasel.GetText(nTotal-nCount--,sCurr);
		new_vary += (atof(sCurr)-new_base)*(atof(sCurr)-new_base);
	}
	new_vary = sqrt(new_vary/nTotal);

	m_dBase = new_base;
	m_dVary = new_vary;
	UpdateData(FALSE);
	UpdateChart();
}
