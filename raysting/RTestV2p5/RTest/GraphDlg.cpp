// GraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TryAgain.h"
#include "GraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphDlg dialog


CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGraphDlg)
	m_dbase = 0.0;
	m_dAverage = 0.0;
	//}}AFX_DATA_INIT
	validcount = 0;
}


void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphDlg)
	DDX_Text(pDX, IDC_DBASE, m_dbase);
	DDX_Text(pDX, IDC_AVERAGE, m_dAverage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialog)
	//{{AFX_MSG_MAP(CGraphDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphDlg message handlers

BOOL CGraphDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_dbase = DataExp(data[0]);
	m_dAverage = DataAverage(data,validcount);

	testGraph = new CGraph();
	SetTimer(1,200,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CGraphDlg::OnDrawGraph()
{
	
	CRect rt;

    CWnd* graphFrame = (CWnd*)GetDlgItem(IDC_GRAPH_FRAME);
	
    CDC* pDC = graphFrame->GetDC();
	graphFrame->GetClientRect(&rt);

	testGraph->validcount = validcount;
    for(int i=0;i<validcount;i++)
		 testGraph->data[i]=data[i];
	testGraph->dbase = m_dbase;
    testGraph->DrawGraph(pDC,rt);
    ReleaseDC(pDC);
}

void CGraphDlg::OnButton1() 
{
	UpdateData();
	SetTimer(1,200,NULL);
//	this->PostMessage(WM_DRAWGRAPH,0,0);
	// TODO: Add your control notification handler code here
/*
	CRect rt;
	 CWnd* graphFrame = (CWnd*)GetDlgItem(IDC_GRAPH_FRAME);
     CDC* pDC = graphFrame->GetDC();
	 //UpdateData();
	graphFrame->GetClientRect(&rt);
	 testGraph->validcount = 10;
	 for(int i=0;i<10;i++)
		 testGraph->data[i]=1+(0.1*i/100000);
//	 this->Invalidate();

     testGraph->DrawGraph(pDC,rt);
     ReleaseDC(pDC);	
*/
}

void CGraphDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_dAverage = DataAverage(data,validcount);
	UpdateData(FALSE);
	KillTimer(1);
	if(validcount <= 0)
		this->PostMessage(WM_CLOSE,0,0);
	else
		this->OnDrawGraph();
	CDialog::OnTimer(nIDEvent);
}

double CGraphDlg::DataExp(double input)
{
	int exp;
	double data;
	if(input == 0)
		return 0;

	exp = 0;
	while(true)
	{
		if((input >= 10)||(input <= -10))
		{
			input = input/10;
			exp++;
			continue;
		}
		if((input > -1)&&(input < 1))
		{
			input = input*10;
			exp--;
			continue;
		}
		break;
	}

	if((input > 5)||(input < -5))     //get the round value
	{
		exp++;
	}

	data = (input>0)?1:-1;
	while(true)
	{
		if(exp > 0)
		{
			data = data*10;
			exp--;
			continue;
		}
		if(exp < 0)
		{
			data = data/10;
			exp++;
			continue;
		}
		break;
	}
	return data;
}

double CGraphDlg::DataAverage(double input[], int length)
{
	double sum,upbase,lobase;
	int index,i;

	if(length <= 1)
		return data[0];

	upbase = (m_dbase >= 0)?1.00002:0.99998;
	lobase = (m_dbase >= 0)?0.99998:1.00002;
	
	upbase *= m_dbase;
	lobase *= m_dbase;

	sum=0;
	index = 0; //count valid data number
	for(i=0;i<length;i++)
	{
		if((data[i] < lobase)||(data[i] > upbase))
			continue;
		sum += data[i];
		index++;
	}
	if(index == 0)
		return 0;
	else
		return (sum/index);
}





void CGraphDlg::OnDestroy() 
{
	delete testGraph;
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	
}

void CGraphDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	this->OnDrawGraph();
	// TODO: Add your message handler code here
	
}
