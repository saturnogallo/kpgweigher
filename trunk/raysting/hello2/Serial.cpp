// Serial.cpp


#include "stdafx.h"
#include "Serial.h"

CSerial::CSerial()
{
	m_dwTimeOut=TIMEOUT_SECS;
	m_hIDComDev = NULL;
	m_bOpened = FALSE;
}



CSerial::~CSerial()
{
	if(m_bOpened)
		Close();

}
void dbg(CString a){	AfxMessageBox(a); }
void dbg(int i){
	CString a; a.Format(_T("%i"),i);
	AfxMessageBox(a);
}
BOOL CSerial::Open( BYTE nPort, DWORD nBaud,BYTE nStopBits,BYTE nParity,BYTE nDataBits)
{
	if( m_bOpened ) return( TRUE );

	CString szPort;

	szPort.Format(_T("COM%d:"), nPort );

	m_hIDComDev = CreateFile( szPort, 
										GENERIC_READ | GENERIC_WRITE, 
										0, 
										NULL, 
										OPEN_EXISTING, 
										FILE_ATTRIBUTE_NORMAL, 
										NULL );

	if( m_hIDComDev == NULL ){
		return( FALSE );
	}


	int nTry = 0;
	int nDelay[] = {500, 1000, 400};
	while(1){
		if(!EscapeCommFunction(m_hIDComDev,CLRDTR))
			break;
		Sleep(nDelay[nTry]);
		if(!EscapeCommFunction(m_hIDComDev,SETDTR))
			break;
		DCB dcb;
		dcb.DCBlength = sizeof( DCB );
		if(!GetCommState(m_hIDComDev,&dcb))
			break;
		dcb.BaudRate = nBaud;
		dcb.ByteSize = nDataBits;//8
		dcb.StopBits = 0;//nStopBits;
		dcb.Parity = nParity;
		
		dcb.fBinary = TRUE;
		dcb.EvtChar = 0x0D;
		
		dcb.fParity = FALSE;

			dcb.fInX = FALSE;
			dcb.fOutX = FALSE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fOutxCtsFlow = FALSE;

		dcb.fDtrControl = DTR_CONTROL_DISABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;

		if(!SetCommState(m_hIDComDev,&dcb) ||
			!SetupComm(m_hIDComDev,1024,1024))
			break;

		Sleep(nDelay[nTry]);
		if(!SetCommMask(m_hIDComDev,EV_RXCHAR|EV_RXFLAG) || 
			!PurgeComm(m_hIDComDev,PURGE_RXABORT|PURGE_RXCLEAR))
			break;
		COMMTIMEOUTS timeouts;
		memset(&timeouts,0,sizeof(COMMTIMEOUTS));	
		timeouts.ReadIntervalTimeout=0xFFFFFFFF;	
		timeouts.ReadTotalTimeoutConstant=0;	
		timeouts.ReadTotalTimeoutMultiplier=0xFFFFFFFF;	
		timeouts.WriteTotalTimeoutConstant=0;		
		timeouts.WriteTotalTimeoutMultiplier=0;		
		if(!SetCommTimeouts(m_hIDComDev,&timeouts))
			break;
		m_bOpened = TRUE;
		return m_bOpened;
	}
	m_bOpened = FALSE;
	return m_bOpened;

}



BOOL CSerial::Close( void )
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( TRUE );

	CloseHandle( m_hIDComDev );

	m_bOpened = FALSE;

	m_hIDComDev = NULL;

	return( TRUE );

}



BOOL CSerial::WriteCommByte( unsigned char ucByte )
{

	DWORD dwBytesWritten = 1;


	WriteFile( m_hIDComDev, (LPSTR) &ucByte, 1, &dwBytesWritten, NULL );
	return  ( dwBytesWritten > 0) ? TRUE:FALSE;

}

int CSerial::SendData( unsigned char *buffer, int size)
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwBytesWritten = 0;
	FlushFileBuffers(m_hIDComDev);
	for(int i=0; i<size; i++ ){
		WriteCommByte( buffer[i] );
			
		dwBytesWritten++;
	}
	return( (int) dwBytesWritten );

}





int CSerial::ReadData( void *buffer, int limit )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bReadStatus;

	DWORD dwBytesRead;

	bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, NULL );

	if( !bReadStatus ){
		return( 0 );
	}
	return( (int) dwBytesRead );
}

int CSerial::SendData(char *buffer, int size)
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwBytesWritten = 0;
	
	int i;

	for( i=0; i<size; i++ ){

		WriteCommByte( (unsigned char)buffer[i] );
		dwBytesWritten++;
	}
	return( (int) dwBytesWritten );
}

