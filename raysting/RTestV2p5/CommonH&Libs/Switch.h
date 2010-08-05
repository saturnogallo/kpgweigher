//ATTENTION! no changes to this file could be made anyway
//           since it will affect all the other dll projects
#include "Serial.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SWITCH_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SWITCH_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SWITCH_EXPORTS
#define SWITCH_API __declspec(dllexport)
#else
#define SWITCH_API __declspec(dllimport)
#endif

#define NO_ERR		0
#define OVER_ERR 	1
#define WAIT_ERR	2
#define UNKN_ERR	3
#define HMARK		0x55
#define EMARK		0x56
#define QMARK	    0x57
#define RMARK	    0x58

typedef struct _SWISTATUS
{
	bool bKN[6];
	bool bOl1;
	bool bOl2;
	bool bRn;
}SWISTATUS;

/**	
*      CSwitch class is used to control and watch the
*	switch Control borad.
*/

// This class is exported from the Switch.dll
class SWITCH_API CSwitch {
public:
	CSwitch(void);
	// TODO: add your methods here.
	bool Open(int portnum);
	void Close();
	bool DoSwi(char *sParam);
	int CheckOverLoad();
	bool DoAction(unsigned char uMark);
	SWISTATUS GetSwitchStatus();
private:
	SWISTATUS SwiStatusConvert();
	
	bool bOpen;
	UINT nTimer;
	UINT ErrFlag;
	unsigned char P2;
	unsigned char P3;
	
};

extern SWITCH_API int nSwitch;

SWITCH_API int fnSwitch(void);

