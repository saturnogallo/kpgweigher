#if !defined(__daocheck_h)
#define __daocheck_h
#include "DaoControl.h"

#define DAOCHECK(name) m_##name = #name

class CDaoCheck : public CButton, public DaoControl {
	void Transfer(BOOL bSaveAndValidate);
public:
	CDaoCheck() {};
	CDaoCheck& operator=(Variant const & v);
	void Enable(bool e = true);
	friend void AFXAPI DDX_Control(CDataExchange* pDX, int nIDC, CDaoCheck& rControl);
};


#endif