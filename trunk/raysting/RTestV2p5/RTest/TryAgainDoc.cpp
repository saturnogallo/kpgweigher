// TryAgainDoc.cpp : implementation of the CTryAgainDoc class
//

#include "stdafx.h"
#include "TryAgain.h"

#include "TryAgainDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTryAgainDoc

IMPLEMENT_DYNCREATE(CTryAgainDoc, CDocument)

BEGIN_MESSAGE_MAP(CTryAgainDoc, CDocument)
	//{{AFX_MSG_MAP(CTryAgainDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTryAgainDoc construction/destruction

CTryAgainDoc::CTryAgainDoc()
{
	// TODO: add one-time construction code here

}

CTryAgainDoc::~CTryAgainDoc()
{
}

BOOL CTryAgainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTryAgainDoc serialization

void CTryAgainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTryAgainDoc diagnostics

#ifdef _DEBUG
void CTryAgainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTryAgainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTryAgainDoc commands
