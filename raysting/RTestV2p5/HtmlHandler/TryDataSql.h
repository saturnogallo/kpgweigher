#ifndef _TRYDATASQL_H_
#define _TRYDATASQL_H_

// TryDataSql.h : sql interface implementation
//
#include "stdafx.h"

//wrapped sql operation
bool SqlInit(CString mdbname);
CString SqlSelect(CString stable,CString sname,CString sgroup);
CString SqlAdd(CString stable,CString sname,CString sgroup);
CString SqlDelete(CString stable,CString sname,CString sgroup);
CString SqlNames(CString stable,CString sname,CString sgroup);


#endif