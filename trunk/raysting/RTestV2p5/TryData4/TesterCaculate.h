#ifndef _TESTER_CACULATER_H_
#define _TESTER_CACULATER_H_
#include "stdafx.h"

class CTesterCaculate
{
public:
	static bool CheckStable(double data[], int length, double scale);
	static double GetAverage(double*,int);
	static int RetrieveIndex(LPCSTR);
	static bool ExtractInt(CString &,int &);
	static bool ExtractDouble(CString &,double &);
	static bool ExtractString(CString &,CString &);
	static bool ExtractChar(CString&,char&);
	static double ConvertDataExp(double);
	static CString DoublePrecision(double ,int );
	static bool CheckDriveID();
	static int searchlclst(LPCTSTR strid);
	static int searchpanlst(LPCTSTR strid);
	static CString retrievelclst(int nIndex);
	static CString retrievepanlst(int nIndex);
	static void InsertColon(CString &val);
protected:
private:
	static int Searchstring(LPCTSTR strid,const CString stable);
public:
	
};
#endif




















