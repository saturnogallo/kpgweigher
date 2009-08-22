// RICacuDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "RICacuDialog.h"
#include "RunitemDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRICacuDialog dialog


CRICacuDialog::CRICacuDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRICacuDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRICacuDialog)
	m_op = _T("");
	m_op1 = _T("");
	m_op2 = _T("");
	//}}AFX_DATA_INIT
}


void CRICacuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRICacuDialog)
	DDX_LBString(pDX, IDC_OP, m_op);
	DDX_LBString(pDX, IDC_OP1, m_op1);
	DDX_LBString(pDX, IDC_OP2, m_op2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRICacuDialog, CDialog)
	//{{AFX_MSG_MAP(CRICacuDialog)
	ON_BN_CLICKED(IDC_BUTTON5, OnExit)
	ON_BN_CLICKED(IDC_TEMPBTN, OnToNextCfg)
	ON_LBN_SELCHANGE(IDC_OP1, OnSelchangeOp1)
	ON_CBN_EDITCHANGE(IDC_OP, OnEditchangeOp)
	ON_LBN_SELCHANGE(IDC_OP2, OnSelchangeOp2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRICacuDialog message handlers

void CRICacuDialog::OnExit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->ShowWindow(SW_HIDE);
}

void CRICacuDialog::OnToNextCfg() 
{
	// TODO: Add extra cleanup here
}

BOOL CRICacuDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddCBItem(IDC_OP,_T("+|-|*|/|>|<|"));
	// TODO: Add mem list
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CRICacuDialog::FillOperator(CString memlist)
{	//fill the operator
	AddCBItem(IDC_OP1,_T("T_CH1|R_CH1|T_CH2|R_CH2|") + memlist);
	AddCBItem(IDC_OP2,_T("T_CH1|R_CH1|T_CH2|R_CH2|") + memlist);
}

void CRICacuDialog::OnSelchangeOp1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();	
}

void CRICacuDialog::OnEditchangeOp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CRICacuDialog::OnSelchangeOp2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
