#ifndef TESTER_PROGRAM_H_
#define TESTER_PROGRAM_H_

#include "stdafx.h"
#include "afxtempl.h"
#include "math.h"
#include "SwiNav.h"

class CTesterProgram
{
public:
	CTesterProgram();

	~CTesterProgram()
	{
		Abort();
	}
	void Abort();
	void InstallScheme(CString schemename);				//open and install the scheme file to prg_var
	bool StepProgram();
	inline BOOL IsDone() { return m_fun.IsEmpty(); };
	bool DuringPause();
	void SetEnv(CString env);
	CString GetEnv(CString key);
	static void SetGlobalEnv(CString env);
	static CString GetGlobalEnv(CString key);


	CString mystate;	//last state string
	static CSwiNav m_swinav; //shared switch port
	static bool swifree;
private:
	CString lastnav; //last navmeter command;
	int pause_cnt;	//count of pause
	double CacuExpr(CString expr);
	bool ProgramFunction(TCHAR type,CString Param);		//internal functions
	void ApplyEnv(CString &line);

	CString	m_fun;	//current function string (ex: line1\nline2\nline3... all the parameter and sub-calls are replaced.

	CMap<long,long,CString,CString> prg_var;	//hash table to lookup program and different parameter
	static CMap<long,long,CString,CString> env_var;
};


#endif




















