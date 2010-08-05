#if !defined(__daogroup_h)
#define __daogroup_h
#include "DaoControl.h"

class CDaoGroup : public CButton, public DaoControl {
	void Transfer(BOOL bSaveAndValidate);
public:
	CDaoGroup() {};
	CDaoGroup& operator=(Variant const & v);
	void Enable(bool e = true);
	friend void AFXAPI DDX_Control(CDataExchange* pDX, int nIDC, CDaoGroup& rControl);
};


#endif
