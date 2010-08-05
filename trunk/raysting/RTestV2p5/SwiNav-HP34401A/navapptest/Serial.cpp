// Serial.cpp


#include "stdafx.h"
#include "Serial.h"

/*
HANDLE CSerial::m_hIDComDev=NULL;
OVERLAPPED CSerial::m_OverlappedRead, CSerial::m_OverlappedWrite;

BOOL CSerial::m_bOpened = FALSE;
*/
HANDLE m_hIDComDev=NULL;
OVERLAPPED m_OverlappedRead,m_OverlappedWrite;
BOOL m_bOpened = FALSE;

CSerial m_Serial;

//unsigned char CSerial::m_spcount = 1;
CSerial::CSerial()
{
//	m_spid=m_spcount++;
	if(!m_bOpened)
	{
		memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
		
		memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
		
		m_hIDComDev = NULL;
		
		m_bOpened = FALSE;
		
	}
}



CSerial::~CSerial()
{
	if(m_bOpened)
		Close();
}



BOOL CSerial::Open( int nPort, int nBaud,int nStopBits,int nParity,int nDataBits)
{
	m_spid= nPort;
	nPort= 1;
	if( m_bOpened ) return( TRUE );

	char szPort[15];

	char szComParams[50];

	DCB dcb;


TRACE("HERE\n");
	wsprintf( szPort, "COM%d", nPort );

	m_hIDComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );

	if( m_hIDComDev == NULL ) return( FALSE );



	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );

 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );



	COMMTIMEOUTS CommTimeOuts;

	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;

	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;

	CommTimeOuts.ReadTotalTimeoutConstant = 0;

	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;

	CommTimeOuts.WriteTotalTimeoutConstant = 5000;

	SetCommTimeouts( m_hIDComDev, &CommTimeOuts );



	wsprintf( szComParams, "COM%d:%d,n,8,1", nPort, nBaud );



	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );



	dcb.DCBlength = sizeof( DCB );

	GetCommState( m_hIDComDev, &dcb );

	dcb.BaudRate = nBaud;

	dcb.ByteSize = 8;

	unsigned char ucSet;

	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_DTRDSR ) != 0 );

	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_RTSCTS ) != 0 );

	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_XONXOFF ) != 0 );

	if( !SetCommState( m_hIDComDev, &dcb ) ||
		!SetupComm( m_hIDComDev, 10000, 10000 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL ){

		DWORD dwError = GetLastError();

		if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );

		if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );

		CloseHandle( m_hIDComDev );

		return( FALSE );

		}



	m_bOpened = TRUE;



	return( m_bOpened );



}



BOOL CSerial::Close( void )
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( TRUE );

	if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );

	if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );

	CloseHandle( m_hIDComDev );

	m_bOpened = FALSE;

	m_hIDComDev = NULL;

	return( TRUE );

}



BOOL CSerial::WriteCommByte( unsigned char ucByte )
{

	BOOL bWriteStat;

	DWORD dwBytesWritten;



	bWriteStat = WriteFile( m_hIDComDev, (LPSTR) &ucByte, 1, &dwBytesWritten, &m_OverlappedWrite );

	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) ){
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) dwBytesWritten = 0;
		else{

			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE );

			m_OverlappedWrite.Offset += dwBytesWritten;

			}

		}



	return( TRUE );

}

#define HMARK	0x55

int CSerial::SendData( int portnum,unsigned char *buffer, int size)
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwBytesWritten = 0;

	int i;

	WriteCommByte( HMARK );
	WriteCommByte( portnum );
	for( i=0; i<size; i++ ){

		WriteCommByte( buffer[i] );
		if(buffer[i] == HMARK)
			WriteCommByte( HMARK );

		dwBytesWritten++;

		}

	return( (int) dwBytesWritten );

}



int CSerial::ReadDataWaiting( void )
{



	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );



	DWORD dwErrorFlags;

	COMSTAT ComStat;



	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );



	return( (int) ComStat.cbInQue );



}



int CSerial::ReadData( void *buffer, int limit )
{



	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );



	BOOL bReadStatus;

	DWORD dwBytesRead, dwErrorFlags;

	COMSTAT ComStat;



	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );

	if( !ComStat.cbInQue ) return( 0 );



	dwBytesRead = (DWORD) ComStat.cbInQue;

	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;



	bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead );

	if( !bReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return( (int) dwBytesRead );
			}

		return( 0 );

		}



	return( (int) dwBytesRead );



}




int CSerial::SendData(int portnum,char *buffer, int size)
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );



	DWORD dwBytesWritten = 0;

	WriteCommByte( HMARK );
	WriteCommByte( portnum );
	
	int i;

	for( i=0; i<size; i++ ){

		WriteCommByte( (unsigned char)buffer[i] );
		if(buffer[i] == HMARK)
			WriteCommByte( HMARK );
		
		dwBytesWritten++;

		}
	return( (int) dwBytesWritten );
}

BOOL CSerial::IsOpened()
{
	return m_bOpened?TRUE:FALSE;
}