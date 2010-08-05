#ifndef _NBZHANDLER_H_
#define _NBZHANDLER_H_

#include "stdafx.h"
#include "HtmlHandler.h"

class CNbzHandler : public CHtmlHandler
{
public:
	bool Handler(CString spage,CString op,CString scmd);
	const char* PathQuery(int clsid,int pathid);
protected:
	bool RegHandler(CString op,CString scmd);
	bool ReportHandler(CString op,CString scmd);
	bool DataHandler(CString op,CString scmd);
	bool JhHandler(CString op,CString scmd);
	bool TestHandler(CString op,CString scmd);
	bool Configure1Handler(CString op,CString scmd);
	bool Configure2Handler(CString op,CString scmd);
	
	void DataIntegrateForCheck(CMapStringToString& mstr,int indexmax,bool colormark);
	void PrepareArrayForReport(CMapStringToString& mstr);
	void DataIntegrateForCompensate(CMapStringToString& mstr,int indexmax);
private:
	
};
#endif






