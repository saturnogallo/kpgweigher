#include "StdAfx.h"
#include "HtmlCtrl.h"
 
 #ifdef _DEBUG
 #define new DEBUG_NEW
 #undef THIS_FILE
 static char THIS_FILE[] = __FILE__;
 #endif
 
 IMPLEMENT_DYNAMIC(CHtmlCtrl, CHtmlView)
 BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
    ON_WM_DESTROY()
    ON_WM_MOUSEACTIVATE()
 END_MESSAGE_MAP()
 
 //////////////////
 // Create control in same position as an existing static control with
 // the same ID (could be any kind of control, really)
 //
 BOOL CHtmlCtrl::CreateFromStatic(UINT nID, CWnd* pParent)
 {
    CStatic wndStatic;
    if (!wndStatic.SubclassDlgItem(nID, pParent))
       return FALSE;
 
    // Get static control rect, convert to parent's client coords.
    CRect rc;
    wndStatic.GetWindowRect(&rc);
    pParent->ScreenToClient(&rc);
    wndStatic.DestroyWindow();
 
    // create HTML control (CHtmlView)
    return Create(NULL,                  // class name
       NULL,                             // title
       (WS_CHILD | WS_VISIBLE ),         // style
       rc,                               // rectangle
       pParent,                          // parent
       nID,                              // control ID
       NULL);                            // frame/doc context not used
 }
 
 ////////////////
 // Override to avoid CView stuff that assumes a frame.
 //
 void CHtmlCtrl::OnDestroy()
 {
    // This is probably unecessary since ~CHtmlView does it, but
    // safer to mimic CHtmlView::OnDestroy.
    if (m_pBrowserApp) {
       m_pBrowserApp->Quit();
       m_pBrowserApp = NULL;
    }
    CWnd::OnDestroy(); // bypass CView doc/frame stuff
 }
 
 ////////////////
 // Override to avoid CView stuff that assumes a frame.
 //
 int CHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
 {
    // bypass CView doc/frame stuff
    return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
 }
 
static CString WebStr2Str(LPCSTR lpBuf)
{
	int nLen;
	if(!lpBuf)	nLen=0;
	else		nLen=::lstrlen(lpBuf);

	CString s;	int i=0;
	while(i<nLen)
	{
		if(lpBuf[i]=='%')
		{
			BYTE c1=lpBuf[i+1];
			BYTE c2=lpBuf[i+2];
			i+=2;
			if(c1>='0' && c1<='9')		c1=(c1-'0')*16;
			else if(c1>='A' && c1<='Z')	c1=(c1-'A'+10)*16;
			else if(c1>='a' && c1<='a') c1=(c1-'a'+10)*16;
			if(c2>='0' && c2<='9')		c2=c2-'0';
			else if(c2>='A' && c2<='Z')	c2=c2-'A'+10;
			else if(c2>='a' && c2<='z')	c2=c2-'a'+10;

			char szStr[2];	szStr[0]=c1+c2;	szStr[1]=0;
			s+=szStr;
		}
		else if(lpBuf[i]=='+')	s+=" ";
		else s+=CString(&lpBuf[i],1);
		i++;
	}
	return s;
}

 //////////////////
 // Override navigation handler to pass to "app:" links to virtual handler.
 // Cancels the navigation in the browser, since app: is a pseudo-protocol.
 //
 void CHtmlCtrl::OnBeforeNavigate2( 
    LPCTSTR lpszURL,
    DWORD nFlags,
    LPCTSTR lpszTargetFrameName,
    CByteArray& baPostedData,
    LPCTSTR lpszHeaders,
    BOOL* pbCancel )
{

	CString sURL(lpszURL);
	int id=sURL.ReverseFind('\\');		//反向查找"\和/"
	if(id==-1)	sURL.ReverseFind('/');

	try
	{
		if(sURL.Mid(id+1).CompareNoCase("ToHtmlCtrl"))	{
			*pbCancel = FALSE;
			return;
			//throw(0);	//不是预定义的URL
		}
/*
		if(((baPostedData.vt) & (VT_VARIANT | VT_BYREF))==0)
			throw(0);
		VARIANT * v=baPostedData.pvarVal;
		if(((v->vt) & (VT_UI1 | VT_ARRAY))==0)
			throw(0);
		SAFEARRAY *pArr=v->parray;

		CString sData=(LPCSTR)pArr->pvData;		//至此，得到传送的数据流
*/
		CString sData;
		sData.Format("%s",(char *)baPostedData.GetData());
		////////////// 数据流的结构 //////////////
		// 变量1=值1&变量2=值2&......变量n=值n  //
		//////////////////////////////////////////

		CStringArray arrPart;
		while(TRUE)		//按照 '&' 进行拆分
		{
			id=sData.Find('&');
			if(id==-1){	arrPart.Add(sData);	break;	}
			arrPart.Add(sData.Left(id));
			sData=sData.Mid(id+1);
		}
		
		m_sMap.RemoveAll();
		for(int nPart=0;nPart<arrPart.GetSize();nPart++)
		{	//循环处理每个“变量=值”的部分
			CString sPart=arrPart.GetAt(nPart);	//取出
			id=sPart.Find('=');
			ASSERT(id!=-1);
			CString sName = sPart.Left(id);		//变量名
			CString sValue= sPart.Mid(id+1);	//值

			sName = WebStr2Str(sName);		//转换Web字符串到标准字符串
			sValue= WebStr2Str(sValue);
			m_sMap.SetAt(sName,sValue);
		}
		m_sResultURL.Empty();
		this->GetParent()->SendMessage(m_wMsg,0,0);
//		BSTR bstr = m_sResultURL.AllocSysString();		
		
		//////////// 以下是演示使用DHTML //////////////
/*
		IHTMLDocument2 *pDoc=(IHTMLDocument2 *)m_hc.GetDocument();
		VARIANT *param;
		SAFEARRAY *sfArray;
		BSTR bstr = sResult.AllocSysString();
		sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
		if(sfArray && pDoc)
		{
			if(S_OK == SafeArrayAccessData(sfArray,(LPVOID*) & param))
			{
				param->vt = VT_BSTR;
				param->bstrVal = bstr;
				SafeArrayUnaccessData(sfArray);
				pDoc->write(sfArray);
			}
			SysFreeString(bstr);
			if (sfArray)	SafeArrayDestroy(sfArray);
		}
		pDoc->Release();
*/
		if(m_sResultURL.IsEmpty())
			*pbCancel = FALSE;
		else
		{
			lpszURL = m_sResultURL;
			*pbCancel=TRUE;
		}
	}
	catch(...)
	{
			lpszURL = m_sResultURL;
			*pbCancel=TRUE;
	}
 }
