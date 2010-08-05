#if !defined(__daoradio_h)
#define __daoradio_h
#include "DaoControl.h"

#define DAORADIO(name) m_##name = #name

class CDaoRadio : public CButton, public DaoControl {
	void Transfer(BOOL bSaveAndValidate);
public:
	CDaoRadio() {};
	CDaoRadio& operator=(Variant const & v);
	void Enable(bool e = true);
	friend void AFXAPI DDX_Control(CDataExchange* pDX, int nIDC, CDaoRadio& rControl);
};


#endif
