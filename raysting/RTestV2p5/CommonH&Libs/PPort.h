#ifndef _PPORT_H_
#define _PPORT_H_

#include "stdafx.h"

class CPPort{
public:
	CPPort();
	~CPPort();
	
	//Methods
	void                               Open(int nPort=1);
	inline BOOL		                   IsOpen();
	void                               Close();
	void                               WriteControl(int nData);
	int                                ReadControl();
	void                               WriteData(int nData);
	int                                ReadData();
	int                                ReadStatus();
private:
	HANDLE         m_hPort;         //SDK handle to lock Win32 access to the port
	unsigned short m_nBaseAddress;  //The base address of the port
};
#endif