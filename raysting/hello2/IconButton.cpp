// IconButton.cpp : 实现文件
//

#include "stdafx.h"
#include "Hello2.h"
#include "IconButton.h"


// CIconButton

IMPLEMENT_DYNAMIC(CIconButton, CButton)
CIconButton::CIconButton()
{
}

CIconButton::~CIconButton()
{
}


BEGIN_MESSAGE_MAP(CIconButton, CButton)
END_MESSAGE_MAP()



// CIconButton 消息处理程序

void CIconButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   UINT uStyle = DFCS_BUTTONPUSH;

   // This code only works with buttons.
/*
   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
   CDC dc;
   dc.Attach(lpDrawItemStruct->hDC);
   dc.DrawIcon(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top,m_hIcon);
   if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
		dc.Draw3dRect(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, 
					(lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left),
					(lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top),RGB(255,255,10),RGB(0,0,10));
   dc.Detach();
   */
	
   // If drawing selected, add the pushed style to DrawFrameControl.
   if (lpDrawItemStruct->itemState & ODS_SELECTED)
      uStyle |= DFCS_PUSHED;

   // Draw the button frame.
   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, 
      DFC_BUTTON, uStyle);

   // Get the button's text.
   CString strText;
   GetWindowText(strText);

   // Draw the button text using the text color red.
   COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255,0,0));
   ::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
      &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   ::SetTextColor(lpDrawItemStruct->hDC, crOldColor);

}