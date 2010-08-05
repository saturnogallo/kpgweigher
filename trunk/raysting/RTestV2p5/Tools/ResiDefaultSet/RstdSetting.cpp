#include "RstdSetting.h"
#include "stdafx.h"

#define DT_NAMEDLG  1
#define DT_SELDLG   2
	
CRstdSetting::CRstdSetting()
{
	CurIndex = -1;
}
/**
 *	Load list from file "Resi.def"
 *	and we assume the fisrt record as the default one.
 *	and we copy one for reservation
 *	here we use the format as following.
 *	so the current setting is pointing to m_list(CurIndex+1)
 *	<title>
 *	Param1
 *	Param2
 *	...
 *		
 */
bool CRstdSetting::InstallFromFile()
{
	char DirUse[300];
	int epos;
	int count;
	CString line;
	RSTDTYPE rt;
	
	::GetCurrentDirectory(300,DirUse);
	strcat(DirUse,"\\Data\\Default\\Resi.def");
	if(!csf.Open(DirUse,CFile::modeRead))
		return false;
	CurIndex = -1;
	while(csf.ReadString(line))
	{
		if(line.IsEmpty())
			continue;
		if(line.GetAt(0) == '<')
		{
			epos=line.Find('>',0);
			if(epos > 1)
			{
				rt.sTitle = line.Mid(1,epos-1);
				for(count = 0;count<7;count++)
				{
					csf.ReadString(line);
					rt.bUse[count] = (line.GetAt(0) == 'Y');
					csf.ReadString(line);
					rt.dAlpha[count] = atof(line);
					csf.ReadString(line);
					rt.dBeta[count] = atof(line);
					csf.ReadString(line);
					rt.dValue[count] = atof(line);
				}
				m_list.Add(rt);
				if(CurIndex == -1)
					m_list.Add(rt); //add a copy to m_list
				CurIndex++;
			}
			continue;
		}
	}
	csf.Close();
	return (CurIndex != -1);
}

/**
 *	Store the RstdSetting in the list to the file "Resi.def"
 *	with the same sequence as in Installfromfile()
 *      one thing should be paid attention to is that the second setting 
 *	will be omitted.
 */
 
bool CRstdSetting::StoreToFile()
{
	char DirUse[300];
	RSTDTYPE rt;
	int index,listsize;
	CString line;
	int count;
	::GetCurrentDirectory(300,DirUse);
	strcat(DirUse,"\\Data\\Default\\Resi.def");
	if(!csf.Open(DirUse,CFile::modeCreate|CFile::modeWrite))
		return false;
	
	index = 0;
	listsize = m_list.GetSize();
	while(index < listsize)
	{
		if(index == 1)
		{
			index++;
			continue;
		}
		rt = m_list.GetAt(index++);
		csf.WriteString('<'+rt.sTitle+'>'+'\n');
		for(count = 0;count<7;count++)
		{
			line.Format("%c\n",(rt.bUse[count])?'Y':'N');
			csf.WriteString(line);
			line.Format("%f\n",rt.dAlpha[count]);
			csf.WriteString(line);
			line.Format("%f\n",rt.dBeta[count]);
			csf.WriteString(line);
			line.Format("%f\n",rt.dValue[count]);
			csf.WriteString(line);
		}
	}
	csf.Close();	
	return true;
}
/**
 *	Get the Current RstdSetting if available
 *
 */
 
RSTDTYPE CRstdSetting::GetCurRstd()
{
	RSTDTYPE rt;
	if(CurIndex < 0)
	{
		for(int count =0 ;count <7;count++)
		{
			rt.sTitle = _T("");
			rt.bUse[count] = true;
			rt.dAlpha[count] = 0;
			rt.dBeta[count]  = 0;
			rt.dValue[count] = 0;
		}
		return rt;
	}
	return m_list.GetAt(CurIndex+1);
}
/**
 *	Delete the Current Rstdsetting if available 
 *	and set the CurIndex
 */
void CRstdSetting::DeleteCurRstd()   
{
	if(CurIndex <= 0)
		return;		//the default one cant be deleted
	m_list.RemoveAt(CurIndex+1);
	CurIndex--;
}
/**
 *	Add the RSTDTYPE to the list with a new name
 */
void CRstdSetting::AddRstd(RSTDTYPE rt)
{
/*
	int index;
	CNameSelDlg nsd;
	CString sTitle;
	index = 1;
	while(index < m_list.GetSize())
	{
		sTitle += m_list.GetAt(index++).sTitle+"\n";
	}
	nsd.nametype = DT_NAMEDLG;
	nsd.namelist = sTitle;
	if(nsd.DoModal() == IDCANCEL)
		return;
	rt.sTitle = nsd.m_sInput;
	m_list.Add(rt);
	CurIndex = m_list.GetSize()-2;//set the current one to the latest one
*/
}
/**
 *	let the user select another RSTDTYPE
 *	should be followed with GetCurRstd to get the updated setting
 */
void CRstdSetting::SelectRstd()
{
/*	int index;
	CNameSelDlg nsd;
	CString sTitle;
	index = 1;
	while(index < m_list.GetSize())
	{
		sTitle += m_list.GetAt(index++).sTitle+"\n";
	}
	nsd.nametype = DT_SELDLG;
	nsd.namelist = sTitle;
	if(nsd.DoModal() == IDCANCEL)
		return;
	CurIndex = nsd.nameid;
*/
}
void CRstdSetting::ModifyDefaultRst(RSTDTYPE rt)
{
	m_list.SetAt(1,rt);
}

RSTDTYPE CRstdSetting::GetDefault()
{
	return m_list.GetAt(1);	
}

bool CRstdSetting::SetCurIndex(int newindex)
{
	CurIndex = newindex;
	return true;
}

void CRstdSetting::ModifyOriDefaultRst(RSTDTYPE rt)
{
	m_list.SetAt(0,rt);
}

CString CRstdSetting::RetrieveTitleAll()
{
	int index;
	CString sTitle;
	index = 1;
	while(index < m_list.GetSize())
	{
		sTitle += m_list.GetAt(index++).sTitle+"\n";
	}
	return sTitle;
}

