// TryData3Sql.cpp : sql interface implementation
//
#include "stdafx.h"
#include "trydata4dlg.h"
#include "TryDataSql.h"
#include "..\\sjInclude\\SimpleAdo\\Include\\SimpleAdo.h"
	
CSimpleAdo g_sado("");
const LPCTSTR fmt = "position=%s,value=%s,group=%s,name=%s,prg=%s";
extern CMapStringToString g_msTemp;
extern CString g_idvals;
extern  const char* PathQuery(int pathid,int clsid);
static CString sProgram;
void SqlQuit(){
	g_sado.CloseAdo();
}
bool SqlInit(CString mdbname)
{
	//check the available and whether appendable 
	CFile file;
	CFileException e;
	if(!file.Open(mdbname,CFile::modeWrite,&e))
		return false;

	file.Close();
	g_sado.SetMdbname(mdbname);
	sProgram.Format(" where [prg]='%s'",PathQuery(CLSGROUP,IDREG));
	return true;
}
/*
 *	return the namelists of records specified by condition
 */
CString SqlIndex(CString stable,CString sname,CString scon)
{
	CStringArray sarray;

	g_sado.QueryField("Select DISTINCT ["+sname+"] From "+stable+" "+scon,sname+"=%s",sarray);

	CString namelist;
	int spos=sarray.GetSize();
	while(spos-- > 0){
		sarray[spos].Remove(',');
		namelist += sarray[spos].Right(sarray[spos].GetLength()-(sname.GetLength()+1))+",";
	}
	return namelist;
}

/*
 *	return the namelists of records specified by sgroup
 */
CString SqlNames(CString stable,CString sname,CString sgroup)
{
	CStringArray sarray;
	CString ssql= "Select * From "+stable;
	CString scon= "";
	
	scon = sProgram;
	if(stable == CString("conf"))
		scon = CString(" WHERE [prg]='*'");
	if(sgroup.Compare("*") != 0)
		scon += " AND [group]='"+sgroup+"'";

	g_sado.QueryField("Select DISTINCT [name] From "+stable+scon,"name=%s",sarray);

	CString namelist;
	int spos=sarray.GetSize();
	bool clje = false; //found 测量结果
	while(spos-- > 0){
		if(sarray[spos] == _T("name=测量结果"))
		{
			clje = true;
			continue;
		}
		sarray[spos].Remove(',');
		namelist += sarray[spos].Right(sarray[spos].GetLength()-5)+",";
	}
	if(clje)
		return _T("测量结果,")+namelist;
	else
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
	CString scon= sProgram;

	if(stable == CString("conf"))
		scon = CString(" WHERE [prg]='*'");

	if(sname.Compare("*") != 0)		scon += " AND [name]='"+sname+"'";
	if(sgroup.Compare("*") != 0)
	{
		scon += " AND [group]='"+sgroup+"'";
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
		if(g_idvals.Find(","+record.Mid(i+1,in-(i+1))+",") >=0)
		{
			g_msTemp.SetAt(record.Mid(i+1,in-(i+1)),record.Mid(inn+1,innn-(inn+1)));
			if(((record.GetAt(i+1) == 'z') || (record.GetAt(i+1) == 'Z')) && (record.GetAt(i+2) >= '0') && (record.GetAt(i+2) <= '9'))
			{
				g_msTemp.SetAt(record.Mid(i+1,in-(i+1))+CString("YS"),record.Mid(inn+1,innn-(inn+1)));
			}
		}
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
	if(!g_msTemp.IsEmpty()){
		CString tempids = g_idvals;
		CString record,skey,sval,sreg;
		if(stable == CString("conf"))
			sreg = CString("*");
		else
			sreg = CString(PathQuery(CLSGROUP,IDREG));

		while(!tempids.IsEmpty() && tempids.Find(',') >= 0)
		{
			skey = tempids.Left(tempids.Find(','));
			tempids = tempids.Right(tempids.GetLength() - (skey.GetLength()+1));//remove the ','
			if(skey.IsEmpty()) continue;
			sval.Empty();
			g_msTemp.Lookup(skey,sval);	
			
			record.Format("position=%s,value=%s,group=%s,name=%s,prg=%s",skey,sval,sgroup,sname,sreg);

			sarray.Add(record);
		}
		g_sado.AddField("Select * From "+stable,fmt,sarray);
	}	
	return SqlSelect(stable,sname,sgroup);
}
/*
 *	Delete and select the next record specified by sname sgroup
 */
CString SqlDelete(CString stable,CString sname,CString sgroup)
{
	CString ssql= "Delete * From "+stable;
	CString scon=sProgram;
	if(stable == CString("conf"))
		scon = CString(" WHERE [prg]='*'");

	if(sname.Compare(_T("*")) != 0)		
		scon += " AND [name]='"+sname+"'";
	if(sgroup.Compare("*") != 0)
		scon += "AND [group]='"+sgroup+"'";

	g_sado.DeleteField(ssql+scon);
	return SqlSelect(stable,SqlSelect(stable,"*",sgroup),sgroup);
}

/*
 *	Delete the next record specified by sname sgroup
 */
void SqlDeleteOnly(CString stable,CString sname,CString sgroup)
{
	CString ssql= "Delete * From "+stable;
	CString scon=sProgram;
	if(stable == CString("conf"))
		scon = CString(" WHERE [prg]='*'");

	if(sname.Compare(_T("*")) != 0)		
		scon += " AND [name]='"+sname+"'";
	if(sgroup.Compare("*") != 0)
		scon += "AND [group]='"+sgroup+"'";
	
	g_sado.DeleteField(ssql+scon);
}
