//ATTENTION! no changes to this file could be made anyway
//           since it will affect all the other dll projects
#include "Serial.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NAVMETER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NAVMETER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef NAVMETER_EXPORTS
#define NAVMETER_API __declspec(dllexport)
#else
#define NAVMETER_API __declspec(dllimport)
#endif

/**
*	this is used to store the common command of 
*	the NavMeter Keithley 2000
*/
#define NAV_READ		0//:GetValue
#define NAV_REMOTE		1//:Setup Remote Control
#define NAV_INIT		2//:Init Device
#define NAV_CLOSE		3//:Exit Command
#define NAV_ZEROON     	4//:Zero Check On
#define NAV_RANGE1		5//:Config 1 --auto
#define NAV_RANGE2   	6//:Config 2
#define NAV_RANGE3   	7//:Config 3
#define NAV_RANGE4   	8//:Config 4
#define NAV_RANGE5   	9//:Config 5
#define NAV_RANGE6   	10//:Config 6
#define NAV_ZEROOFF		11//:ZeroCheck Off
#define NAV_LPASON	    12//:ANOLOG FILTER ON
#define NAV_LPASOFF     13//:ANOLOG FILTER OFF
#define NAV_DFILON		14//:DIGITAL FILTER ON
#define NAV_DFILOFF     15//:DIGITAL FILTER OFF
#define NAV_SLOWMODE	16//:SLOWMODE
typedef struct _NMCMDTYPE
{
	UINT CmdID;	//>Comand ID
	char *Data;	//>the String we would send and use '%' add meaningful data
}NMCMDTYPE;

// This class is exported from the Navmeter.dll
class NAVMETER_API CNavmeter {
public:
	CNavmeter(void);
	bool Open(int portnum);     
	bool SendCommand(UINT nCmdID);
	void Close();
public:
	double value;
private:
//	CSerial m_Serial;
	bool bOpen;
private:
	bool HandleReply(int index);
	
};

extern NAVMETER_API int nNavmeter;

NAVMETER_API int fnNavmeter(void);

