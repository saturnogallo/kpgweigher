// RTestChartCtl.cpp : Implementation of the CRTestChartCtrl ActiveX Control class.

#include "stdafx.h"
#include "SjChart.h"
#include "RTestChartCtl.h"
#include "RTestChartPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRTestChartCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRTestChartCtrl, COleControl)
	//{{AFX_MSG_MAP(CRTestChartCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CRTestChartCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CRTestChartCtrl)
	DISP_FUNCTION(CRTestChartCtrl, "SetAxis", SetAxis, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION(CRTestChartCtrl, "AddData", AddData, VT_EMPTY, VTS_R8)
	DISP_FUNCTION(CRTestChartCtrl, "ClearAll", ClearAll, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CRTestChartCtrl, COleControl)
	//{{AFX_EVENT_MAP(CRTestChartCtrl)
	EVENT_STOCK_MOUSEUP()
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CRTestChartCtrl, 1)
	PROPPAGEID(CRTestChartPropPage::guid)
END_PROPPAGEIDS(CRTestChartCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRTestChartCtrl, "SJCHART.RTestChartCtrl.1",
	0xf537bb40, 0xfe2d, 0x4c97, 0x84, 0xb1, 0x53, 0x9c, 0x75, 0xd8, 0xcb, 0xf9)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CRTestChartCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DRTestChart =
		{ 0xd84d0a57, 0xc9f, 0x4d1c, { 0xbb, 0xff, 0x5b, 0xa7, 0xf8, 0x29, 0x37, 0x35 } };
const IID BASED_CODE IID_DRTestChartEvents =
		{ 0x9783833c, 0x4ed6, 0x4c0e, { 0xa8, 0x82, 0x11, 0xe6, 0xa3, 0x9b, 0xc1, 0x23 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwRTestChartOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CRTestChartCtrl, IDS_RTESTCHART, _dwRTestChartOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CRTestChartCtrl::CRTestChartCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CRTestChartCtrl

BOOL CRTestChartCtrl::CRTestChartCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_RTESTCHART,
			IDB_RTESTCHART,
			afxRegApartmentThreading,
			_dwRTestChartOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartCtrl::CRTestChartCtrl - Constructor

CRTestChartCtrl::CRTestChartCtrl()
{
	InitializeIIDs(&IID_DRTestChart, &IID_DRTestChartEvents);
	testGraph = new CGraph();
	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartCtrl::~CRTestChartCtrl - Destructor

CRTestChartCtrl::~CRTestChartCtrl()
{
	// TODO: Cleanup your control's instance data here.
	delete testGraph;
}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartCtrl::OnDraw - Drawing function

void CRTestChartCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);

    testGraph->DrawGraph(pdc,rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartCtrl::DoPropExchange - Persistence support

void CRTestChartCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartCtrl::OnResetState - Reset control to default state

void CRTestChartCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CRTestChartCtrl message handlers

void CRTestChartCtrl::SetAxis(double upper, double lower, double base, double criteria) 
{
	// TODO: Add your dispatch handler code here
	testGraph->upper = upper;
	testGraph->lower = lower;
	testGraph->dbase = base;
	testGraph->criteria = criteria;
	Invalidate();
}

void CRTestChartCtrl::AddData(double value) 
{
	// TODO: Add your dispatch handler code here
	testGraph->data.Add(value);
	Invalidate();
}

void CRTestChartCtrl::ClearAll() 
{
	// TODO: Add your dispatch handler code here
	testGraph->data.RemoveAll();
	Invalidate();
}


void CRTestChartCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	testGraph->MouseUp(LEFT_BUTTON,SHIFT_MASK,point.x,point.y);
	COleControl::OnLButtonUp(nFlags, point);
	Invalidate();
}

void CRTestChartCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	testGraph->MouseUp(RIGHT_BUTTON,SHIFT_MASK,point.x,point.y);	
	COleControl::OnRButtonUp(nFlags, point);
	Invalidate();
}
