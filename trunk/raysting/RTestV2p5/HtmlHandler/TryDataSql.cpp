// TryData3Sql.cpp : sql interface implementation
//
#include "stdafx.h"
#include "TryDataSql.h"
#include "..\\sjInclude\\SimpleAdo\\Include\\SimpleAdo.h"
	
CSimpleAdo g_sado("");
const LPCTSTR fmt = "position=%s,value=%s,group=%s,name=%s";

extern CMapStringToString g_msTemp;
bool SqlInit(CString mdbname)
{
	//check the available and whether appendable 
	CFile file;
	if(!file.Open(mdbname,CFile::modeWrite))
		return false;

	file.Close();
	g_sado.SetMdbname(mdbname);
	return true;
}
/*
 *	return the namelists of records specified by sgroup
 */
CString SqlNames(CString stable,CString sname,CString sgroup)
{
	CStringArray sarray;
	CString ssql= "Select * From "+stable;
	CString scon= "";
	
	if(sgroup.Compare("*") != 0)
		scon += " where [group]='"+sgroup+"'";
	
	g_sado.QueryField("Select DISTINCT [name] From "+stable+scon,"name=%s",sarray);

	CString namelist;
	int spos=sarray.GetSize();
	while(spos-- > 0){
		sarray[spos].Remove(',');
		namelist += sarray[spos].Right(sarray[spos].GetLength()-5)+",";
	}
	return namelist;
}
/*
 *	select the record specified by sname and sgroup
 *	and set the (stable+"sz" ,stable+"lst") schemes
 */
CString SqlSelect(CString stable,CString sname,CString sgroup)
{
	CStringArray sarray;
	CString ssql= "Select * From "+stable;
	CString scon= "";
	if(sname.Compare("*") != 0)		scon += " where [name]='"+sname+"'";
	if(sgroup.Compare("*") != 0)
	{
		if(scon.IsEmpty())
			scon += " where [group]='"+sgroup+"'";
		else
			scon += "AND [group]='"+sgroup+"'";
	}
		
	g_sado.QueryField(ssql+scon,fmt,sarray);
	int pos=sarray.GetSize();
	while(pos-- > 0)
	{
		int i,in,inn,innn;
		CString record;
		record = sarray[pos];
		i=record.Find('=',0);
		in=record.Find(',',i);
		inn=record.Find('=',in);
		innn=record.Find(',',inn);
		g_msTemp.SetAt(record.Mid(i+1,in-(i+1)),record.Mid(inn+1,innn-(inn+1)));
	}
	CString namelist = SqlNames(stable,sname,sgroup);
	return (namelist.Find(sname,0) >= 0) ? sname: namelist.Left(namelist.Find(','));
}
/*
 *	add and select the record specified by sname sgroup
 */
CString SqlAdd(CString stable,CString sname,CString sgroup)
{
	CStringArray sarray;
	
	POSITION pos=g_msTemp.GetStartPosition();
	while(pos != NULL)
	{
		CString record,skey,sval;
		g_msTemp.GetNextAssoc(pos,skey,sval);
		record.Format("position=%s,value=%s,group=%s,name=%s",skey,sval,sgroup,sname);
		sarray.Add(record);
	}
	g_sado.AddField("Select * From "+stable,fmt,sarray);
	
	return SqlSelect(stable,sname,sgroup);
}
/*
 *	Delete and select the next record specified by sname sgroup
 */
CString SqlDelete(CString stable,CString sname,CString sgroup)
{
	CString ssql= "Delete * From "+stable;
	CString scon= "";
	if(sname.Compare("*") != 0)		scon += " where [name]='"+sname+"'";
	if(sgroup.Compare("*") != 0){
		if(scon.IsEmpty())
			scon += " where [group]='"+sgroup+"'";
		else
			scon += "AND [group]='"+sgroup+"'";
	}
	g_sado.DeleteField(ssql+scon);
	
	return SqlSelect(stable,SqlSelect(stable,"*",sgroup),sgroup);
}
