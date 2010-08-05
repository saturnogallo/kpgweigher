#if !defined(__daocontrol_h)
#define __daocontrol_h

//This is the base class for control objects. They must inherit from this
//in addition to their MFC hierarchy.
//It manages database data retrieving and storing.

#define DAOCONTROL(name) m_##name = #name
					//Macro for initializing control objects

class DaoControl : public Field {
	virtual void Transfer(BOOL bSaveAndValidate) = 0;
		//This forces derived classes to implement the transfer mechanism.
protected:
	Variant index;
		//Attribute for the field class. It can be a field name or a field index.
	void DDX_Control(CDataExchange *pDX);
		//Binding method. It manages collaboration between the GUI 
		//and database components.
public:
	DaoControl();
	DaoControl& operator=(Variant const &);
		//Assignment operator. Each derived class must override this.
	virtual void Enable(bool e = true) = 0;
		//User interface enabling/disabling mandatory method.
};


#endif