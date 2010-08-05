// DaoWindow.h: interface for the DaoWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAOWINDOW_H__8DA15773_75B9_11D2_9172_8E5E24000000__INCLUDED_)
#define AFX_DAOWINDOW_H__8DA15773_75B9_11D2_9172_8E5E24000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bordao.h"

//This is the base class for dialog and property sheet objects. They must
//inherit from this in addition to their MFC hierarchy.
//It manages recordset mapping of the user interface object.

class DaoWindow : public Recordset  
{
protected:
	bool alta;
		//Attribute that determines if we are editing or adding a record.
public:
	virtual bool Execute() = 0;
		//Mandatory execution method.
	virtual bool Exec(char* sql, bool a = false);
		//This method launches a dialog object from an external user interface.
	virtual bool Baja(char* sql);
		//Method for managing record deletion
	DaoWindow(LPCSTR s = 0, bool a = false);
		//Regular constructor. It takes a SQL string and an editing/adding condition.
	DaoWindow(Recordset& r, bool a = false);
		//Constructor for child objects, taking an existing recordset.
	virtual ~DaoWindow() {};
	friend class CDaoPropPage;
		//Objects of this class must access protected attributes.
};

#endif // !defined(AFX_DAOWINDOW_H__8DA15773_75B9_11D2_9172_8E5E24000000__INCLUDED_)
