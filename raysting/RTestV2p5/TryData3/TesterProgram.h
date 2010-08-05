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
	void Pause();			//pause the program
	void Continue();		//continue the program 
	void Abort();
	static int State;
	bool NextLine(char &type, CString &cmd); //next valid command
	bool SetFunctionPointer(CString funname); //go to the specific function
	bool InstallScheme(CString schemename);				//open and install the scheme file
	CStdioFile logout; //log file
	bool bLogOn;
protected:
	bool CheckCondition(CString &line,char &type);		//condition check
	bool FunEnter(CString &cmdLine);			//function control 
	bool ProgramFunction(char type,CString Param);		//internal functions
private:

	CStringArray m_fun;
	int m_iStackDepth;
	int m_iFunpos[FUN_MAX+1];

	int m_iCmdPos;			//current position in program
	
	int m_iTotalLines;		//total lines in program	

};


#endif




















