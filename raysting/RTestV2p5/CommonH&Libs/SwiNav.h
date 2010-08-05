
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SWINAV_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SWINAV_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef SWINAV_EXPORTS
#define SWINAV_API __declspec(dllexport)
#else
#define SWINAV_API __declspec(dllimport)
#endif

#include "Switch.h"
#include "Navmeter.h"
// This class is exported from the SwiNav.dll
class SWINAV_API CSwiNav {
public:
	CSwiNav(void);
	// TODO: add your methods here.
};
/**
*	this is used to store the common command of 
*	the NavMeter Keithley 2182
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

extern SWINAV_API int nSwiNav;

SWINAV_API int fnSwiNav(void);

