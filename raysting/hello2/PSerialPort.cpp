// PSerialPort.cpp: implementation of the CPSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PSerialPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSerialPort::CPSerialPort()
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_hReadThread = NULL;
	m_bReceiving = FALSE;
	m_nBufferSize = 256; //缓冲大小
}

CPSerialPort::~CPSerialPort()
{
	ClosePort();
}

DWORD WINAPI CPSerialPort::ReadPortThread(LPVOID lpParameter)
{
	CPSerialPort* m_pSerial;

	m_pSerial=(CPSerialPort*)lpParameter;

	BOOL fReadState;
	DWORD dwLength;
	
	char* buf=new char[m_pSerial->m_nBufferSize];

	while((m_pSerial->m_hComm!=INVALID_HANDLE_VALUE)&&(m_pSerial->m_bReceiving))
	{		
		fReadState=ReadFile(m_pSerial->m_hComm,buf,m_pSerial->m_nBufferSize,&dwLength,NULL);
		if(!fReadState)
		{
			AfxMessageBox(_T("无法从串口读取数据！"));
		}
		else
		{
			if(dwLength!=0)
			{
				//AfxMessageBox(_T("回送数据"));
				if(m_pSerial->m_lpDataArriveProc!=NULL)
				{
					m_pSerial->m_lpDataArriveProc(buf,dwLength,m_pSerial->m_dwUserData);
				}
			}else{
				Sleep(1);
			}
		}		
	}

	delete[] buf;

	return 0;
}

BOOL CPSerialPort::OpenPort(LPCTSTR Port,int BaudRate,int StopBits,int Parity,int DataBits,LPDataArriveProc proc,DWORD userdata)
{
	m_lpDataArriveProc=proc;
	m_dwUserData=userdata;

	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		m_hComm=CreateFile(Port,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
		if(m_hComm==INVALID_HANDLE_VALUE )
		{
//			AfxMessageBox(_T("ERROR 104:无法打开端口！请检查是否已被占用。"));
			return FALSE;
		}
		GetCommState(m_hComm,&dcb);
		dcb.BaudRate=BaudRate;
		dcb.ByteSize=DataBits;
		dcb.Parity=Parity;
		dcb.StopBits=StopBits;
		dcb.fParity=FALSE;
		dcb.fBinary=TRUE;
		dcb.fDtrControl=0;
		dcb.fRtsControl=0;
		dcb.fOutX=dcb.fInX=dcb.fTXContinueOnXoff=0;
		
		//设置状态参数
		SetCommMask(m_hComm,EV_RXCHAR);		
		SetupComm(m_hComm,1024,1024);		
		if(!SetCommState(m_hComm,&dcb))
		{
			AfxMessageBox(_T("ERROR 105:无法按当前参数配置端口，请检查参数！"));
			PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}
		
		//设置超时参数
		GetCommTimeouts(m_hComm,&CommTimeOuts);		
		CommTimeOuts.ReadIntervalTimeout=100;
		CommTimeOuts.ReadTotalTimeoutMultiplier=1;
		CommTimeOuts.ReadTotalTimeoutConstant=100;
		CommTimeOuts.WriteTotalTimeoutMultiplier=1;
		CommTimeOuts.WriteTotalTimeoutConstant=100;		
		if(!SetCommTimeouts(m_hComm,&CommTimeOuts))
		{
			AfxMessageBox(_T("ERROR 106:无法设置超时参数！"));
			PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}
		
		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);		
		Activate();
		return TRUE;		
	}
	
	return TRUE;
}

BOOL CPSerialPort::ClosePort()
{
	Deactivate();
	if(m_hComm!=INVALID_HANDLE_VALUE)
	{
		if(IsActive())
			Deactivate();
		SetCommMask(m_hComm,0);		
		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
		CloseHandle(m_hComm);
		m_hComm=INVALID_HANDLE_VALUE;
		return TRUE;
	}
	
	return TRUE;	
}

BOOL CPSerialPort::Activate()
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if(!m_bReceiving)
	{
		//开始接收线程
		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
		m_bReceiving=TRUE;
		m_hReadThread=CreateThread(NULL,0,ReadPortThread,this,0,NULL);
	}
	if(m_hReadThread!=NULL)
	{		
		return TRUE;
	}
	else
	{
		m_bReceiving=FALSE;
		return FALSE;
	}

	return FALSE;
}

BOOL CPSerialPort::Deactivate()
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	//停止接收线程
	if(m_bReceiving)
	{
		m_bReceiving=FALSE;
		WaitForSingleObject(m_hReadThread,500);
		CloseHandle(m_hReadThread);
		m_hReadThread=NULL;
		return TRUE;
	}
	return FALSE;
}

BOOL CPSerialPort::IsActive()
{
	return m_bReceiving;
}

DWORD CPSerialPort::WritePort(unsigned char *data,int length)
{
	
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	BOOL bFileWriteState;
	DWORD dwBytesWritten=0;
	COMSTAT ComStat; //串口状态的详细情况表, 
    DWORD dwErrorFlags; //读串口状态的标志
	ClearCommError(m_hComm,&dwErrorFlags,&ComStat); 
	bFileWriteState = WriteFile(m_hComm,data,length*sizeof(char),&dwBytesWritten,NULL);
//	AfxMessageBox(L"正在写串口数据。。。");
	if(dwBytesWritten != length)
		TRACE(_T("missing data"));
	return dwBytesWritten;
}

DWORD CPSerialPort::ReadPort(char *data,int length)
{
	BOOL bFileReadState;
	DWORD dwLength,dwByteRead;
	int TimeOutCount;

	dwByteRead=0;
	TimeOutCount=0;
	char* buf=new char[length];

	while(m_hComm!=INVALID_HANDLE_VALUE)
	{
		bFileReadState = ReadFile(m_hComm,data,length,&dwLength,NULL);
		if(!bFileReadState)
		{
			break;
		}
		else
		{
			dwByteRead+=dwLength;
			data+=dwLength;			
		}
		if(dwByteRead == (unsigned long)length)
		{
			break;
		}
		if(dwLength!=0)
		{
			TimeOutCount=0;
		}
		else
		{
			TimeOutCount++;
			Sleep(5);
		}
		if(TimeOutCount==5)
		{
			break;
		}
	}

	delete[] buf;
	return dwByteRead;
}

