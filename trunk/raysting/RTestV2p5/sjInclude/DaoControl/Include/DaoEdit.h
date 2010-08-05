#if !defined(__daoedit_h)
#define __daoedit_h
#include "DaoControl.h"

#define DAOEDIT(name, ronly) m_##name = #name; m_##name.ReadOnly = ronly

class CDaoEdit : public CEdit, public DaoControl {
	bool ROnly;
	void Transfer(BOOL bSaveAndValidate);
public:
	CDaoEdit() {ReadOnly = false;};
	CDaoEdit& operator=(Variant const &);
	void Enable(bool e = true);
	bool GetROnly() {return ROnly;}
	void SetROnly(bool r) {ROnly = r;}
	bool __declspec(property(get = GetROnly, put = SetROnly)) ReadOnly;
	friend void AFXAPI DDX_Control(CDataExchange* pDX, int nIDC, CDaoEdit& rControl);
};


#endif 