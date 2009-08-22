#ifndef SERIAL_H
#define SERIAL_H

#include <windows.h>
#include <Winbase.h>

#define TIMEOUT_SECS			20
#define DEFAULT_BAUD_RATE		38400;

#define CR	0x0D
#define LF	0x0A

class Serial
{
protected:
	static DWORD m_dwTimeOut;
	HANDLE m_hComm;
	BOOL m_bOK;
	BOOL m_bBacked;
	char m_chLastChar;

protected:
	Serial(TCHAR *strPort,int nRate=57600,BOOL bSoftHandshake=FALSE);

public:
	static Serial *OpenSerial(TCHAR *strPort,int nRate=57600,BOOL bSoftHandshake=FALSE);
    virtual ~Serial();

public:
    BOOL ReadByte(char *c);
	BOOL PutBack(char c);
    BOOL ReadLine(char * buf,DWORD len);
    BOOL WriteLine(char * buf,DWORD len,BOOL bCR=TRUE);
    BOOL Wait(DWORD timeout);
	BOOL ClearBuffers();
    void SetTimeOut(DWORD timeout);

	BOOL IsSerialOK()
	{
		return m_bOK;
	}
};

#endif // #ifndef SERIAL_H

