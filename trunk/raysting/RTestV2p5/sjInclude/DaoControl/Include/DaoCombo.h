#if !defined(__daocombo_h)
#define __daocombo_h
#include "DaoControl.h"

#define DAOCOMBO(name, table) m_##name.Init(#name, table)
#define DAOCOMBOD(name, table, dis)\
	m_##name.Init(#name, table);\
	m_##name.Disabled = dis

class CDaoCombo: public CComboBox, public DaoControl, public Recordset {
	void Transfer(BOOL bSaveAndValidate);
	bool Dis;
public:
	CDaoCombo() {Dis = false;}
	void Init(Variant v, LPCSTR s);
	void Enable(bool e = true);
	void SetDis(bool d) {Dis = d;}
	bool __declspec(property(put = SetDis)) Disabled;
	friend void AFXAPI DDX_Control(CDataExchange* pDX, int nIDC, CDaoCombo& rControl);
};


#endif