#include "stdio.h"
#include "serial1.h"

DWORD Serial::m_dwTimeOut=TIMEOUT_SECS;

Serial *Serial::OpenSerial(TCHAR *strPort,int nRate,BOOL bSoftHandshake)
{
	Serial *pSerial=new Serial(strPort,nRate,bSoftHandshake);
	if(pSerial != NULL && !pSerial->IsSerialOK())
	{
		delete pSerial;
		pSerial=NULL;
	}
	return pSerial;		
}

Serial::Serial(TCHAR *strPort,int nRate,BOOL bSoftHandshake)
{
	m_bBacked=FALSE;
	m_bOK=TRUE;
	int nDelay[] = {2000, 1000, 400};

	m_hComm = CreateFile(strPort,
						 GENERIC_READ|GENERIC_WRITE,
						 0,
						 NULL,
						 OPEN_EXISTING,
						 FILE_ATTRIBUTE_NORMAL,
						 NULL);

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		m_bOK=FALSE;
		return;
	}
	
	int nTry = 3;
	BOOL bPass;
	while (nTry-- > 0)
	{
//		bPass = FlushFileBuffers(m_hComm);
//		if(!bPass)	break;

		bPass = EscapeCommFunction(m_hComm,CLRDTR);
		if(!bPass)	break;

		Sleep(nDelay[nTry]);
		bPass = EscapeCommFunction(m_hComm,SETDTR);
		if(!bPass)	break;
		
		DCB dcb;
		bPass = GetCommState(m_hComm,&dcb);
		if(!bPass)	break;
		
		dcb.fBinary = TRUE;
		dcb.BaudRate = nRate;
		dcb.EvtChar = 0x0D;
		
		dcb.fParity = FALSE;
		dcb.Parity = 0;
		dcb.ByteSize = 8;
		dcb.StopBits = 0;

		if (!bSoftHandshake)
		{
			dcb.fInX = FALSE;
			dcb.fOutX = FALSE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fOutxCtsFlow = FALSE;
		}
		else
		{
			dcb.fInX  = TRUE;
			dcb.fOutX = TRUE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fOutxCtsFlow = FALSE;
		}
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
		
		bPass = SetCommState(m_hComm,&dcb);
		if(!bPass)	break;

		Sleep(nDelay[nTry]);
		bPass = SetCommMask(m_hComm,EV_RXCHAR|EV_RXFLAG);
		if(!bPass)	break;
		
		bPass = SetupComm(m_hComm,1024,1024);
		if(!bPass)	break;
		
		bPass = PurgeComm(m_hComm,PURGE_RXABORT|PURGE_RXCLEAR);
		if(!bPass)	break;

		COMMTIMEOUTS timeouts;
		memset(&timeouts,0,sizeof(COMMTIMEOUTS));	//缺省超时设置
		timeouts.ReadIntervalTimeout=MAXDWORD;			//读时如果缓冲区有数据，直接返回，否则等待
		timeouts.ReadTotalTimeoutConstant=10000;		//最长10秒
		timeouts.ReadTotalTimeoutMultiplier=MAXDWORD;	//
		timeouts.WriteTotalTimeoutConstant=0;			//写时不计算超时
		timeouts.WriteTotalTimeoutMultiplier=0;			//
		bPass = SetCommTimeouts(m_hComm,&timeouts);
		if(!bPass)	break;

		if(bPass)
			break;
	}

	m_bOK=bPass;
}

Serial::~Serial()
{
	if(m_hComm != INVALID_HANDLE_VALUE)
		CloseHandle(m_hComm);
}

BOOL Serial::ReadByte(char *c)
{
	if(m_bBacked)
	{
		m_bBacked=FALSE;
		*c=m_chLastChar;
		return TRUE;
	}

	DWORD dwReaded;

	BOOL bDone=ReadFile(m_hComm,c,1,&dwReaded,NULL);
	if(bDone && dwReaded == 0)
		bDone=FALSE;

	if(bDone)
		return bDone;

	return bDone;
}

BOOL Serial::PutBack(char c)
{
	if(m_bBacked)
		return FALSE;

	m_bBacked=TRUE;
	m_chLastChar=c;
	return TRUE;
}

BOOL Serial::ReadLine(char *buf,DWORD len)
{
	DWORD i=0;
	char c;
	BOOL bRet;

	while ((bRet=ReadByte(&c)) == TRUE)
	{
		buf[i++]=c;
		if (c == LF)	break;
		if(i == (len-1))	break;
	}
	buf[i++]=0;
	
	return bRet;
}

BOOL Serial::WriteLine(char * buf,DWORD len,BOOL bCR)
{
	DWORD dwTimeElapsed=0,dwWritten=0,dwdwInitTime=GetTickCount();

	if (bCR)	{ buf[len++]=CR; }
	
	FlushFileBuffers(m_hComm);
	if (!WriteFile(m_hComm,buf,len,&dwWritten,NULL) || dwWritten != len)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL Serial::Wait(DWORD timeout)
{
//	SetCommMask(m_hComm,EV_RXCHAR);
	DWORD dwEvent;
	if(!WaitCommEvent(m_hComm,&dwEvent,NULL))
	{
	}

	return TRUE;
}

BOOL Serial::ClearBuffers()
{
	return FlushFileBuffers(m_hComm);
}

void Serial::SetTimeOut(DWORD timeout)
{
	m_dwTimeOut = timeout;
}
