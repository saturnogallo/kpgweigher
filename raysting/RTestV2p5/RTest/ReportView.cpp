// ReportView.cpp : implementation file
//

#include "stdafx.h"
#include "TryAgain.h"
#include "ReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportView

IMPLEMENT_DYNCREATE(CReportView, CHtmlView)

CReportView::CReportView()
{
	//{{AFX_DATA_INIT(CReportView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CReportView::~CReportView()
{
}

void CReportView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportView, CHtmlView)
	//{{AFX_MSG_MAP(CReportView)
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportView diagnostics

#ifdef _DEBUG
void CReportView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CReportView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReportView message handlers

void CReportView::OnInitialUpdate() 
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	char DirUse[300];
	CStdioFile csf;
	::GetCurrentDirectory(300,DirUse);
	strcat(DirUse,"\\Data\\temp.htm");
	if(csf.Open(DirUse,CFile::modeRead))
	{
		csf.Close();
		Navigate2(DirUse,NULL,NULL);
		return;
	}
	::GetCurrentDirectory(300,DirUse);
	strcat(DirUse,"\\Data\\Default\\Default.htm");
	Navigate2(DirUse,NULL,NULL);
}

void CReportView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;
	CHtmlView::OnRButtonUp(nFlags, point);
}
