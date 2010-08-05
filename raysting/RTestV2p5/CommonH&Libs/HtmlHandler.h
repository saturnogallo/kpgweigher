
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HTMLHANDLER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HTMLHANDLER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef HTMLHANDLER_EXPORTS
#define HTMLHANDLER_API __declspec(dllexport)
#else
#define HTMLHANDLER_API __declspec(dllimport)
#endif
#include "stdafx.h"
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename ("EOF", "adoEOF")

#define CLSMDB		1
#define CLSJS		2
#define CLSGROUP	3

#define IDREG		1
#define IDDATA		2 
#define IDJH		3 
#define IDREPORT	4 
#define IDTEST		5 
#define IDCONF1		6 
#define IDCONF2		7 
#define IDTBR		8 
#define IDWAIT		9

#define ZINDEX_MAX	400
#define MDB_PATTERN	"@@*@@"
#define DEFAULT_NAME "默认"		//Default Scheme and Conf name
#define SCHEME_LIST "电阻箱快速测量\n电阻箱精密测量\n电阻箱一般测量\n电桥测量\n电压测量"

#define MAIN_FORM	"\\Html\\imgorg\\main.htm"
#define REG_FORM	"\\Html\\regform.js"
#define DATA_FORM	"\\Html\\dataform.js"
#define JH_FORM		"\\Html\\jhform.js"
#define REPORT_FORM	"\\Html\\reportform.js"
#define TEST_FORM	"\\Html\\testform.js"
#define CONF1_FORM	"\\Html\\conf1form.js"
#define CONF2_FORM	"\\Html\\conf2form.js"
#define TBR_FORM	"\\Html\\tbrform.js"

#include "TryDataDefine.h"
// This class is exported from the HtmlHandler.dll
class HTMLHANDLER_API CHtmlHandler {
public:
	CString m_gPath;
public:
	CHtmlHandler(void);
	bool Handler(CString spage,CString op,CString scmd);
	static void ModifyDefaultRstdSetting(RSTDTYPE ps);
protected:
	bool TbrHandler(CString op,CString scmd);
	bool RegHandler(CString op,CString scmd);
	bool ReportHandler(CString op,CString scmd);
	bool DataHandler(CString op,CString scmd);
	bool JhHandler(CString op,CString scmd);
	bool TestHandler(CString op,CString scmd);
	bool Configure1Handler(CString op,CString scmd);
	bool Configure2Handler(CString op,CString scmd);
	void DataIntegrateForCheck(CMapStringToString& mstr,int indexmax,bool colormark){return;};
	void PrepareArrayForReport(CMapStringToString& mstr){return;};
private:
	void UpdateParmSetting();
	void UpdateRstdSetting();
	void UpdateScheme();
};

extern HTMLHANDLER_API int nHtmlHandler;

HTMLHANDLER_API int fnHtmlHandler(void);

