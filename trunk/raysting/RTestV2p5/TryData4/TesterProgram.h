#ifndef TESTER_PROGRAM_H_
#define TESTER_PROGRAM_H_

#include "stdafx.h"
#include "TesterData.h"
#include "TesterCaculate.h"
#include "Trydatadefine.h"

#define BEIDLE		0
#define BEPAUSE		1
#define BETEST		2
#define BEERRO		4
#define BEFINI		3


#define FUN_MAX		10
class CTesterProgram
{
public:
	CTesterProgram();
	void Abort(){m_iStackDepth = 0;};
	bool NextLine(char &type, CString &cmd); //next valid command
	void FunctionInternal(CString funname);  //execute function internally
	bool SetFunctionPointer(CString funname); //go to the specific function
	bool SetFunctionParameter(CString parm);  //set the parameter for current function
	bool InstallScheme(CString schemename);				//open and install the scheme file
	CStdioFile logout; //log file
	bool bLogOn;
	int m_iStackDepth;
protected:
	bool CheckCondition(CString &line,char &type);		//condition check
	bool FunEnter(CString &cmdLine);			//function control 
	bool ProgramFunction(char type,CString Param);		//internal functions
private:

	CStringArray m_fun;
	
	int m_iFunpos[FUN_MAX+1];

	int m_iCmdPos;			//current position in program
	
	int m_iTotalLines;		//total lines in program	

};


#endif




















