// RTestChartPpg.cpp : Implementation of the CRTestChartPropPage property page class.

#include "stdafx.h"
#include "SjChart.h"
#include "RTestChartPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRTestChartPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRTestChartPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CRTestChartPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRTestChartPropPage, "SJCHART.RTestChartPropPage.1",
	0xcee7a95c, 0x7e41, 0x422c, 0x98, 0xbc, 0xfd, 0xba, 0x76, 0xd8, 0xcf, 0x34)


/////////////////////////////////////////////////////////////////////////////
// CRTestChartPropPage::CRTestChartPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CRTestChartPropPage

BOOL CRTestChartPropPage::CRTestChartPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_RTESTCHART_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartPropPage::CRTestChartPropPage - Constructor

CRTestChartPropPage::CRTestChartPropPage() :
	COlePropertyPage(IDD, IDS_RTESTCHART_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CRTestChartPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartPropPage::DoDataExchange - Moves data between page and properties

void CRTestChartPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CRTestChartPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartPropPage message handlers
