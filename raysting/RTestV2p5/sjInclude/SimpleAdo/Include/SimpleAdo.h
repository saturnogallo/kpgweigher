#ifndef _SJ_SIMPLEADO_H_
#define _SJ_SIMPLEADO_H_

class CSimpleAdo
{
public:
	void CloseAdo();
	CSimpleAdo(CString name);
	~CSimpleAdo();
	void SetMdbname(CString name) {m_Mdbname = name;};
	BOOL QueryField(LPCTSTR sql,LPCTSTR fmt,CStringArray& retdata);
	BOOL AddField(LPCTSTR sql,LPCTSTR fmt,const CStringArray& adddata);
	BOOL DeleteField(LPCTSTR sql);
private:
	BOOL OpenAdo();
	CString m_Mdbname;
	BOOL m_bOpen;
	// 定义ADO连接、命令、记录集变量指针
	_ConnectionPtr	m_pConnection;
};


#endif
