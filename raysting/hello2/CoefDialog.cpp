// CoefDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "CoefDialog.h"
#include "Hello2Dlg.h"
#include "math.h"
#include "Wendu.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const char *atitle[] = {"a7","a8","a9","a10","a11","a5","a4"};
const char *btitle[] = {"b7","b8","b9",""   ,""   ,"b5","b4"};
const char *ctitle[] = {"c7",""  ,""  ,""   ,""   ,""  ,""  };

/////////////////////////////////////////////////////////////////////////////
// CCoefDialog dialog


CCoefDialog::CCoefDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCoefDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoefDialog)
	m_tp = 0.0;
	m_zn = 0.0;
	m_al = 0.0;
	m_ar = 0.0;
	m_ga = 0.0;
	m_hg = 0.0;
	m_in = 0.0;
	m_sn = 0.0;
	m_acoef = 0.0;
	m_bcoef = 0.0;
	m_ccoef = 0.0;
	m_atitle = _T("");
	m_btitle = _T("");
	m_ctitle = _T("");
	//}}AFX_DATA_INIT
	for(int i=0;i<16;i++)
		abc_coef[i] = 0;
	
}

void CCoefDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoefDialog)
	DDX_Text(pDX, IDC_TPEDIT, m_tp);
	DDX_Text(pDX, IDC_ZNEDIT, m_zn);
	DDX_Text(pDX, IDC_ALEDIT, m_al);
	DDX_Text(pDX, IDC_AREDIT, m_ar);
	DDX_Text(pDX, IDC_GAEDIT, m_ga);
	DDX_Text(pDX, IDC_HGEDIT, m_hg);
	DDX_Text(pDX, IDC_INEDIT, m_in);
	DDX_Text(pDX, IDC_SNEDIT, m_sn);
	DDX_Text(pDX, IDC_ACOEF, m_acoef);
	DDX_Text(pDX, IDC_BCOEF, m_bcoef);
	DDX_Text(pDX, IDC_CCOEF, m_ccoef);
	DDX_Text(pDX, IDC_STATIC_COEFA, m_atitle);
	DDX_Text(pDX, IDC_STATIC_COEFB, m_btitle);
	DDX_Text(pDX, IDC_STATIC_COEFC, m_ctitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCoefDialog, CDialog)
	//{{AFX_MSG_MAP(CCoefDialog)
	ON_BN_CLICKED(IDC_WTOCOEF, OnWtocoef)
	ON_BN_CLICKED(IDC_COEFMODIFY, OnCoefmodify)
	ON_BN_CLICKED(IDC_COEFQUIT, OnCoefquit)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoefDialog message handlers


void CCoefDialog::OnWtocoef() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	WrtoCoef(m_sn,m_zn,m_al,m_in,m_ga,m_hg,m_ar,m_tp,m_trange,m_acoef,m_bcoef,m_ccoef,abc_coef);
	UpdateControlStatus();
}

void CCoefDialog::OnCoefmodify() 
{
	// TODO: Add your control notification handler code here
	CHello2Dlg *rd = (CHello2Dlg*)this->GetParent();
	UpdateData();
	SaveABCcoef(m_trange,m_tp,m_acoef,m_bcoef,m_ccoef,abc_coef);
	AfxMessageBox(_T("系数修改成功"));
}

void CCoefDialog::OnCoefquit() 
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_HIDE);
	CHello2Dlg *rd = (CHello2Dlg*)this->GetParent();
	rd->SwitchFromCcfg(abc_coef);
}

BOOL CCoefDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_trange = 0;
	UpdateControlStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

void CCoefDialog::UpdateControlStatus()
{
	//update the display of dialog based on the m_trange selection
	const bool tpshow[] = {true,true,true,true,true,true,true};
	const bool alshow[] = {true,false,false,false,false,false,false};
	const bool arshow[] = {false,false,false,false,false,false,true};
	const bool gashow[] = {false,false,false,false,true,true,false};
	const bool hgshow[] = {false,false,false,false,false,true,true};
	const bool inshow[] = {false,true,false,true,false,false,false};
	const bool snshow[] = {true,true,true,false,false,false,false};
	const bool znshow[] = {true,false,true,false,false,false,false};

	
	if((m_trange < 0) || (m_trange >= (sizeof(atitle)/sizeof(char*))))
		m_trange = 0;
	//coef window
	m_atitle = CString(atitle[m_trange]);
	m_btitle = CString(btitle[m_trange]);
	m_ctitle = CString(ctitle[m_trange]);


	GetABCcoef(m_trange,m_tp,m_acoef,m_bcoef,m_ccoef,abc_coef);

	GetDlgItem(IDC_STATIC_COEFB)->ShowWindow((m_btitle.GetLength() == 0)?SW_HIDE:SW_SHOWNORMAL);
	GetDlgItem(IDC_STATIC_COEFC)->ShowWindow((m_ctitle.GetLength() == 0)?SW_HIDE:SW_SHOWNORMAL);

	GetDlgItem(IDC_BCOEF)->ShowWindow((m_btitle.GetLength() == 0)?SW_HIDE:SW_SHOWNORMAL);
	GetDlgItem(IDC_CCOEF)->ShowWindow((m_ctitle.GetLength() == 0)?SW_HIDE:SW_SHOWNORMAL);
	
	//wcoef window
	GetDlgItem(IDC_STATICTP)->EnableWindow(tpshow[m_trange]);
	GetDlgItem(IDC_STATICAL)->EnableWindow(alshow[m_trange]);
	GetDlgItem(IDC_STATICAR)->EnableWindow(arshow[m_trange]);
	GetDlgItem(IDC_STATICGA)->EnableWindow(gashow[m_trange]);
	GetDlgItem(IDC_STATICHG)->EnableWindow(hgshow[m_trange]);
	GetDlgItem(IDC_STATICIN)->EnableWindow(inshow[m_trange]);
	GetDlgItem(IDC_STATICSN)->EnableWindow(snshow[m_trange]);
	GetDlgItem(IDC_STATICZN)->EnableWindow(znshow[m_trange]);

	GetDlgItem(IDC_TPEDIT)->EnableWindow(tpshow[m_trange]);
	GetDlgItem(IDC_ALEDIT)->EnableWindow(alshow[m_trange]);
	GetDlgItem(IDC_AREDIT)->EnableWindow(arshow[m_trange]);
	GetDlgItem(IDC_GAEDIT)->EnableWindow(gashow[m_trange]);
	GetDlgItem(IDC_HGEDIT)->EnableWindow(hgshow[m_trange]);
	GetDlgItem(IDC_INEDIT)->EnableWindow(inshow[m_trange]);
	GetDlgItem(IDC_SNEDIT)->EnableWindow(snshow[m_trange]);
	GetDlgItem(IDC_ZNEDIT)->EnableWindow(znshow[m_trange]);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck((m_trange == 0)?TRUE:FALSE);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck((m_trange == 1)?TRUE:FALSE);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck((m_trange == 2)?TRUE:FALSE);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck((m_trange == 3)?TRUE:FALSE);
	((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck((m_trange == 4)?TRUE:FALSE);
	((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck((m_trange == 5)?TRUE:FALSE);
	((CButton*)GetDlgItem(IDC_RADIO7))->SetCheck((m_trange == 6)?TRUE:FALSE);
	UpdateData(FALSE);
}

void CCoefDialog::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_trange = 0;
	UpdateControlStatus();
}

void CCoefDialog::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_trange = 1;
	UpdateControlStatus();
}

void CCoefDialog::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_trange = 2;
	UpdateControlStatus();
}

void CCoefDialog::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_trange = 3;
	UpdateControlStatus();
}

void CCoefDialog::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_trange = 4;
	UpdateControlStatus();
}

void CCoefDialog::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	m_trange = 5;
	UpdateControlStatus();
}

void CCoefDialog::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	m_trange = 6;
	UpdateControlStatus();
}

void CCoefDialog::LoadABCCoef(double val[])
{
	for(int i=0;i<16;i++)
		abc_coef[i] = val[i];
	m_trange = (int)floor(abc_coef[COEF_RANGE_POS]);
	m_tp = abc_coef[COEF_VALUE_RTP];
	UpdateControlStatus();
}
