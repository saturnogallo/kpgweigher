#ifndef _HTMLHANDLER_H_
#define _HTMLHANDLER_H_

#include "stdafx.h"
#include "Tester.h"

extern CString SqlDelete(CString stable,CString sname,CString sgroup);
extern CString SqlAdd(CString stable,CString sname,CString sgroup);
extern CString SqlSelect(CString stable,CString sname,CString sgroup);
extern CString SqlNames(CString stable,CString sname,CString sgroup);
extern BOOL SJExtractTxtByMark (LPCTSTR tfname,LPCTSTR dfname,CString smark,void (*func)(LPCTSTR,LPCTSTR));
extern BOOL SJReplaceTxtByMark (LPCTSTR tfname,LPCTSTR dfname,CString smark,LPCTSTR (*func)(LPCTSTR));
extern BOOL SJReplaceWordByBookmark (LPCTSTR tfname,const CMapStringToString& smap);
extern BOOL SJFindFirstMatchByTxt (CString fname,CString& smatch);
extern CString SSearchDir(LPCTSTR ,LPCTSTR );
extern void CopySTSMap(const CMapStringToString&,CMapStringToString&);




#define ZINDEX_MAX	400
#define MDB_PATTERN	"@@*@@"
#define DEFAULT_NAME "Default"		//Default Scheme and Conf name
#define SCHEME_LIST "电阻箱快速测量\n电阻箱精密测量\n电阻箱一般测量\n电桥测量\n电压测量"

#define MAIN_FORM	"\\Html\\imgorg\\main.htm"
#define REG_FORM	"\\Html\\regform.js"
#define DATA_FORM	"\\Html\\dataform.js"
#define JH_FORM		"\\Html\\jhform.js"
#define REPORT_FORM	"\\Html\\reportform.js"
#define TEST_FORM	"\\Html\\testform.js"
#define CONF_FORM	"\\Html\\confform.js"
#define TBR_FORM	"\\Html\\tbrform.js"



class CHtmlHandler
{
public:
	virtual bool Handler(CString spage,CString op,CString scmd);
	
	
protected:
	bool TbrHandler(CString op,CString scmd);
	bool RegHandler(CString op,CString scmd);
	bool ReportHandler(CString op,CString scmd);
	bool DataHandler(CString op,CString scmd);
	bool JhHandler(CString op,CString scmd);
	bool TestHandler(CString op,CString scmd);
	bool ConfigureHandler(CString op,CString scmd);


	static RTESTGROUP	m_RGroup;
private:
	void UpdateParmSetting();
	void UpdateRstdSetting();
	
	void UpdateScheme();
	static void ModifyDefaultRstdSetting(void);
	void ModifyDefaultParmSetting(void);
};

#endif

