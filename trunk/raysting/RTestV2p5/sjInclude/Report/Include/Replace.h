#ifndef _SJ_REPORT_H_
#define _SJ_REPORT_H_

BOOL SJReplaceWordByBookmark(LPCTSTR tfname,const CMapStringToString& smap);
BOOL SJReplaceExcelByMark(LPCTSTR tfname,const CMapStringToString& smap);
BOOL SJExtractTxtByMark(LPCTSTR tfname,LPCTSTR dfname,CString smark,void (*func)(LPCTSTR,LPCTSTR));
BOOL SJReplaceTxtByMark(LPCTSTR tfname,LPCTSTR dfname,CString smark,LPCTSTR (*func)(LPCTSTR));
BOOL SJFindFirstMatchByTxt(CString fname,CString& smatch);
BOOL SJPrintWord(LPCTSTR tfname);
#endif