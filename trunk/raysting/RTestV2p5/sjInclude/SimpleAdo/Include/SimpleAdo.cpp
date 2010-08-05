#include "stdafx.h"
#include "SimpleAdo.h"

CSimpleAdo::CSimpleAdo(CString name)
{
	m_Mdbname=name;
	m_bOpen = false;
	m_pConnection = NULL;
}

CSimpleAdo::~CSimpleAdo()
{
	// 关闭ADO连接状态
	CloseAdo();
}
/*
	query the field  filtered by sql sentence 
	the format specify the field we select
	format : name1=%(i,s,b,f),name2=%(i,s,b,f),...
	and the result will be stored in a string format 
	and packed in retdata
*/

BOOL CSimpleAdo::QueryField(LPCTSTR sql,LPCTSTR fmt,CStringArray& retdata)
{
	if (!OpenAdo())
		return false;
	
	retdata.RemoveAll();
	_CommandPtr		m_pCommand;
	
	_RecordsetPtr	m_pRecordset;
	// 使用ADO创建数据库记录集
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	// 在ADO操作中建议语句中要常用try...catch()来捕获错误信息,因为它有时会经常出现一些想不到的错误。jingzhou xu
	try
	{
		m_pRecordset->Open(sql, // 查询DemoTable表中所有字段
			m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	
	try
	{
		_variant_t var;	
		int pos;
		CString sFmt(fmt);
		
		pos = sFmt.ReverseFind('=');
		if(pos > sFmt.GetLength()-3) //illegal format
			throw(0);
			
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		// 读入库中各字段并加入array中
		while(!m_pRecordset->adoEOF)
		{
			
			int opos=0;
			
			CString sRec,sLine;
			sLine.Empty();
			pos = sFmt.Find('=',opos);
			while (pos != -1)
			{
				_bstr_t bstr;
				var = m_pRecordset->GetCollect(sFmt.Mid(opos,pos-opos).AllocSysString());
				if(var.vt == VT_NULL)
					continue;
				switch(sFmt[pos+2])
				{
					
					case 'i'://int case
					case 'd':
						sRec.Format("%s=%i,",sFmt.Mid(opos,pos-opos),short(var));break;
					case 's'://string case
					case 'c':
						bstr = var;
						sRec.Format("%s=%s,",sFmt.Mid(opos,pos-opos),(const char*)(bstr));break;
					case 'f'://float case
						sRec.Format("%s=%f,",sFmt.Mid(opos,pos-opos),double(var));break;
					default :
						sRec.Format("%s= ,",sFmt.Mid(opos,pos-opos));break;;
				}
				opos = pos+4;//update old position
				pos = sFmt.Find('=',opos);
				sLine += sRec;
			}
			if(sLine.GetLength() > 0) //remove the final ','
				sLine = sLine.Left(sLine.GetAllocLength()-1);
			retdata.Add(sLine);
			m_pRecordset->MoveNext();
		}
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	
	if(m_pRecordset->State)
		m_pRecordset->Close();
	m_pRecordset = NULL;
	return true;
}

/*
	add the field  to the data set filtered by sql sentence 
	the format specify the field we select
	format : name1=%(i,s,b,f),name2==%(i,s,b,f),...
	and the data will be stored in a string format 
	and packed in adddata
*/
BOOL CSimpleAdo::AddField(LPCTSTR sql,LPCTSTR fmt,const CStringArray& adddata)
{
	if (!OpenAdo())
		return false;
	
	// 定义ADO连接、命令、记录集变量指针
	_CommandPtr		m_pCommand;
	
	_RecordsetPtr	m_pRecordset;
	// 使用ADO创建数据库记录集
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	// 在ADO操作中建议语句中要常用try...catch()来捕获错误信息,因为它有时会经常出现一些想不到的错误。jingzhou xu
	try
	{
		m_pRecordset->Open(sql, // 查询DemoTable表中所有字段
			m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	_variant_t var;	
	
	try
	{
		int pos;
		int index=0;
		CString sFmt(fmt);
				
		pos = sFmt.ReverseFind('=');
		if(pos > sFmt.GetLength()-3) //illegal format
			throw(0);
			
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveLast();
		// 读入库中各字段并加入列表框中
		while(index < adddata.GetSize())
		{
			TRACE1("%d,\n",index);
			CString sRec,sLine;
			sLine = adddata.GetAt(index++);
			m_pRecordset->AddNew();
			int opos=0,shift=0,npos;
			pos = sFmt.Find('=',opos);
			sLine += ',';
			while (pos != -1)
			{
				npos = sLine.Find(',',pos+shift);
				if (npos == -1) break;
				
				sRec = sLine.Mid(pos+shift+1,npos-(pos+shift+1));
				if(sRec.IsEmpty())
					sRec = " ";
				switch(sFmt[pos+2])
				{
					case 'i'://int case
					case 'd':
						m_pRecordset->PutCollect(sFmt.Mid(opos,pos-opos).AllocSysString(),(short)atoi(sRec));break;
					case 's'://string case
					case 'c':
						if(sRec.GetLength() > 50)
							sRec = sRec.Left(50);
							m_pRecordset->PutCollect(sFmt.Mid(opos,pos-opos).AllocSysString(),_bstr_t(sRec.AllocSysString(),false));break;
					case 'f'://float case
						m_pRecordset->PutCollect(sFmt.Mid(opos,pos-opos).AllocSysString(),atof(sRec));break;
					default :	break;
				}
				opos = sFmt.Find(',',pos)+1;
				if (opos == 0)
					break;
				pos = sFmt.Find('=',opos);	
				shift = npos-opos+1;
			}
			
		}
		if(index > 0)
			m_pRecordset->Update();		
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	
	if(m_pRecordset->State)
		m_pRecordset->Close();
	m_pRecordset = NULL;
	return true;
}

BOOL CSimpleAdo::DeleteField(LPCTSTR sql)
{
	if (!OpenAdo())
		return false;

	_CommandPtr		m_pCommand;
	
	_RecordsetPtr	m_pRecordset;
	// 使用ADO创建数据库记录集
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	// 在ADO操作中建议语句中要常用try...catch()来捕获错误信息,因为它有时会经常出现一些想不到的错误。jingzhou xu
	try
	{
		m_pRecordset->Open(sql, // 查询DemoTable表中所有字段
			m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	_variant_t var;	
	
	if(m_pRecordset->State)
		m_pRecordset->Close();
	m_pRecordset = NULL;	

	return true;
}

BOOL CSimpleAdo::OpenAdo()
{
	if(m_Mdbname.IsEmpty())
		return FALSE;

	if(m_pConnection != NULL)
		return TRUE;//already open

	m_pConnection.CreateInstance(__uuidof(Connection));	

	_bstr_t bstr(m_Mdbname.AllocSysString(),true);
	bstr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source="+bstr;
	
	try                 
	{	
		// 打开本地Access库 输出数据.mdb
		m_pConnection->Open(bstr,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		AfxMessageBox("数据库连接失败，确认路径是否正确!");
		return FALSE;
	} 
	return TRUE;	
}


void CSimpleAdo::CloseAdo()
{
	if(m_pConnection == NULL)//already closed
		return;
	// 关闭ADO连接状态
	if(m_pConnection->State)
		m_pConnection->Close();
	m_pConnection= NULL;
}
