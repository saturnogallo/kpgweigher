#include "ParamSetting.h"
#include "NameSelDlg1.h"


	
CParamSetting::CParamSetting()
{
	CurIndex = -1;
}
/**
 *	Load list from file "Info.def"
 *	here we use the format as following
 *	<title>
 *	iLowest
 *	iRange
 *	iDelay
 *  dTemper
 *  iTimes
 *  bSingle
 *  sOther
 *  <title2>
 *  ......
 */
bool CParamSetting::InstallFromFile()
{
	char DirUse[300];
	int epos;
	
	CString line;
	PARAMTYPE pt;
	
	::GetCurrentDirectory(300,DirUse);
	strcat(DirUse,"\\Data\\Default\\Info.def");
	if(!csf.Open(DirUse,CFile::modeRead))
		return false;

	while(csf.ReadString(line))
	{
		if(line.IsEmpty())
			continue;
		if(line.GetAt(0) == '<')
		{
			epos=line.Find('>',0);
			if(epos > 1)
			{
				pt.sTitle = line.Mid(1,epos-1);
				csf.ReadString(line);
				pt.iLowest = atoi(line);
				csf.ReadString(line);
				pt.iRange = atoi(line);
				csf.ReadString(line);
				pt.iDelay = atoi(line);
				csf.ReadString(line);
				pt.dTemper = atof(line);
				csf.ReadString(line);
				pt.iTimes = atoi(line);
				csf.ReadString(line);
				pt.bSingle = (line.GetAt(0)=='Y');
				csf.ReadString(pt.sOther);
				m_list.Add(pt);
				CurIndex++;
			}
			continue;
		}
	}
	csf.Close();
	return true;
}

/**
 *	Store the ParamSetting in the list to the file "Info.def"
 *	with the same sequence as in Installfromfile()
 */
 
bool CParamSetting::StoreToFile()
{
	char DirUse[300];
	PARAMTYPE pt;
	int index,listsize;
	CString line;
	::GetCurrentDirectory(300,DirUse);
	strcat(DirUse,"\\Data\\Default\\Info.def");
	if(!csf.Open(DirUse,CFile::modeCreate|CFile::modeWrite))
		return false;
		
	
	index = 0;
	listsize = m_list.GetSize();
	while(index < listsize)
	{
		pt = m_list.GetAt(index++);
		csf.WriteString('<'+pt.sTitle+'>'+"\n");
		line.Format("%i\n",pt.iLowest);
		csf.WriteString(line);
		line.Format("%i\n",pt.iRange);
		csf.WriteString(line);
		line.Format("%i\n",pt.iDelay);
		csf.WriteString(line);
		line.Format("%f\n",pt.dTemper);
		csf.WriteString(line);
		line.Format("%i\n",pt.iTimes);
		csf.WriteString(line);
		line.Format("%c\n",pt.bSingle?'Y':'N');
		csf.WriteString(line);
		csf.WriteString(pt.sOther+"\n");
	}
	csf.Close();	
	return true;
}
/**
 *	Get the Current ParamSetting if available
 *
 */
 
PARAMTYPE CParamSetting::GetCurParam()
{
	PARAMTYPE pt;
	if(CurIndex < 0)
	{
		pt.sTitle = _T("");
		pt.iLowest = 0;
		pt.iRange = 0;
		pt.iDelay = 3;
		pt.dTemper = 0;
		pt.iTimes = 0;
		pt.bSingle = true;
		pt.sOther = _T("");
		return pt;
	}
	return m_list.GetAt(CurIndex);
}
/**
 *	Delete the Current paramsetting if available 
 *	and set the CurIndex
 */
void CParamSetting::DeleteCurParm()   
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
	{
		//the current selection is the last one
		CurIndex--;
	}
}
/**
 *	Add the PARAMTYPE to the list with a new name
 */
void CParamSetting::AddParam(PARAMTYPE pt) 
{
	int index;
	CNameSelDlg nsd;
	CString sTitle;
	index = 0;
	while(index < m_list.GetSize())
	{
		sTitle += m_list.GetAt(index++).sTitle+"\n";
	}
	nsd.nametype = DT_NAMEDLG;
	nsd.namelist = sTitle;
	if(nsd.DoModal() == IDCANCEL)
		return;
	pt.sTitle = nsd.m_sInput;
	m_list.Add(pt);
	CurIndex = m_list.GetSize()-1;//set the current one to the newest one
}
/**
 *	let the user select another PARAMTYPE
 *	Note: the function should be followed with 
 *         GetCurParam() to get the latest setting
 */
void CParamSetting::SelectParam()
{
	int index;
	CNameSelDlg nsd;
	CString sTitle;
	index = 0;
	while(index < m_list.GetSize())
	{
		sTitle += m_list.GetAt(index++).sTitle+"\n";
	}
	nsd.nametype = DT_SELDLG;
	nsd.namelist = sTitle;
	if(nsd.DoModal() == IDCANCEL)
		return;
	CurIndex = nsd.nameid;
}
