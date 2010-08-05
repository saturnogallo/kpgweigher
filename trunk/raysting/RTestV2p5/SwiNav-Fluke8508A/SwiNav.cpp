// SwiNav.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "..\\CommonH&Libs\\SwiNav.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
SWINAV_API int nSwiNav=0;

// This is an example of an exported function.
SWINAV_API int fnSwiNav(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see SwiNav.h for the class definition
CSwiNav::CSwiNav()
{ 
	return; 
}

