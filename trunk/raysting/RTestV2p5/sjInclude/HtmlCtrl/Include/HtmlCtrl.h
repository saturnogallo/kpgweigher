#ifndef _HTML_CTRL_H_
#define _HTML_CTRL_H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealHtmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlCtrl html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CHtmlCtrl : public CHtmlView
{
 public:
 	CMapStringToString m_sMap;
 public:
 
    CHtmlCtrl() { }
    ~CHtmlCtrl() { }

    void SetMessage(UINT msg){m_wMsg = msg;};    
    void SetURL(CString url){m_sResultURL = url;};
    BOOL CreateFromStatic(UINT nID, CWnd* pParent);
 
    // Normally, CHtmlView destroys itself in PostNcDestroy,
    // but we don't want to do that for a control since a control
    // is usually implemented as a stack object in a dialog.
    //
    virtual void PostNcDestroy() {  }
 
    // overrides to bypass MFC doc/view frame dependencies
    afx_msg void OnDestroy();
    afx_msg int  OnMouseActivate(CWnd* pDesktopWnd,UINT nHitTest,UINT message);
 
    // override to trap "app:" pseudo protocol
    virtual void OnBeforeNavigate2( LPCTSTR lpszURL,
       DWORD nFlags,
       LPCTSTR lpszTargetFrameName,
       CByteArray& baPostedData,
       LPCTSTR lpszHeaders,
       BOOL* pbCancel );
 
    DECLARE_MESSAGE_MAP();
    DECLARE_DYNAMIC(CHtmlCtrl)

private:
 	UINT m_wMsg;
 	CString m_sResultURL;
};
#endif

 
