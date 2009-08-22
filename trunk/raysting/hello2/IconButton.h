#pragma once


// CIconButton

class CIconButton : public CButton
{
	DECLARE_DYNAMIC(CIconButton)

public:
	CIconButton();
	virtual ~CIconButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	HICON m_hIcon;
protected:
	DECLARE_MESSAGE_MAP()
};


