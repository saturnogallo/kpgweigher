#ifndef _AFX_Setting_H_021106
#define _AFX_Setting_H_021106

/**
 *	PARAMTYPE is a collection of the 
 *	parameters used in test and stored in file;
 *
 */
#include "Resource.h"
#include "stdafx.h"
#include "afxtempl.h"
#include "math.h"

/**
 *	CSetting class is used to manage the PARAMTYPE list
 *	for example,store or install from to file,and delete,add 
 *	operations
 */
#define PROFILE_NAME	SjGetAppDirectory()+_T("\\Profile.txt")



template <class PTYPE>
class CSetting
{
public:

	CSetting()
	{	
		CurIndex = -1;
		
	}
	//file operation
	bool InstallFromFile(CString cfgfile) //<Load list from file
	{
		CFile cf;
		PTYPE pt;
		CString line;
		
		if(!cf.Open(cfgfile,CFile::modeRead)){

			if(!cf.Open(cfgfile,CFile::modeCreate|CFile::modeWrite))
				return false;
			cf.Close();
			if(!cf.Open(cfgfile,CFile::modeRead)){
				::AfxMessageBox(_T("ERROR 101:打开配置文件失败"));
				return false;
			}
		}
		while(cf.Read(&pt,sizeof(pt)) >= sizeof(pt))
		{
			CString t(pt.sTitle);
			if(t.IsEmpty() || (t.Find(' ') >= 0))
				continue;
			m_list.Add(pt);
			CurIndex++;
		}
		cf.Close();
		return true;
	}
	bool StoreToFile(CString cfgfile)	//<Store the list to file
	{
		PTYPE pt;
		CFile cf;
		
		if(!cf.Open(cfgfile,CFile::modeCreate|CFile::modeWrite))
			return false;
		
		for(int index = 0; index < m_list.GetSize(); index++){
			pt = m_list[index];
			cf.Write(&pt, sizeof(pt));
		}
		
		cf.Close();	
		return true;
	}
	/**
	 *	Delete the Current Setting if available 
	 *	and set the CurIndex
     */
	void DeleteCurParm()   
	{
		if(CurIndex == -1)
			return;
		if(m_list.GetSize() == 1)
		{
			m_list.RemoveAt(0);
			CurIndex = -1;
			return;
		}
		m_list.RemoveAt(CurIndex);
		if(CurIndex == m_list.GetSize())
			CurIndex--;
	}
	CString GetTitleList(TCHAR sp)
	{
		CString names("");
		PTYPE pt;
		if (m_list.GetSize() <= 0) {
			return CString("");
		}
		for(int index = 0; index < m_list.GetSize(); index++){
			pt = m_list[index];
			names = names + CString(pt.sTitle)+ CString(sp);
		}
		return names;
	}
	CString GetNameAt(int index)
	{
		CString names("");
		PTYPE pt;
		if (m_list.GetSize() <= index) {
			return CString("");
		}else{
			pt = m_list[index];
			return CString(pt.sTitle);
		}
	}

	PTYPE SelectParm(const TCHAR *name) //set the cursor of current Parm 
	{
		CString names;
		PTYPE pt;
		::memset(&pt,0,sizeof(pt));
		for(int index = 0; index < m_list.GetSize(); index++){
			if(wcscmp(name,m_list[index].sTitle) == 0){
				return m_list[index];
			}
		}
		return pt;
	}
	void  SaveParm(PTYPE pt) //add a new Parm by copying the current one
	{
		for(int index = 0; index < m_list.GetSize(); index++){
			if(wcscmp(pt.sTitle,m_list[index].sTitle) == 0){
				m_list[index] = pt;
				return;
			}
		}
		m_list.Add(pt);
	}


private:
	CArray<PTYPE,PTYPE> m_list; //<the setting list for store
	int  CurIndex;		 //<store the index of the current selection	
};


#endif

