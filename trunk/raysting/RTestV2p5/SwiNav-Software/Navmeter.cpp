// Navmeter.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "..\\CommonH&Libs\\Navmeter.h"
#include "math.h"

#define MAXWAITCOUNT	50	//longest time for read reply = MAXWAITCOUNT*50ms
extern	CSerial m_Serial;
const NMCMDTYPE cmdList[]={
	{NAV_INIT,"*RST\n*CLS\n:INIT:CONT ON;:ABORT\n:SENS:FUNC 'VOLT:DC'\n:SENSE:CHAN 1\n:sens:volt:rang:auto on\n:sens:volt:chan1:lpas off\nVOLT:DIG 8\n:syst:azer on\n" },
	{NAV_RANGE1,":sens:volt:rang:auto on\n"},
	{NAV_RANGE2,":sens:volt:chan1:rang 0.01\n"},
	{NAV_RANGE3,":sens:volt:chan1:rang 0.1\n"},
	{NAV_RANGE4,":sens:volt:chan1:rang 1\n"},
	{NAV_RANGE5,":sens:volt:chan1:rang 10\n"},
	{NAV_RANGE6,":sens:volt:chan1:rang 100\n"},
	{NAV_READ,":fetc?\n"},
	{NAV_ZEROON,":sens:volt:ref:acq\n:sens:volt:ref:stat on\n"},
	{NAV_ZEROOFF,":sens:volt:ref:stat off\n"},
	{NAV_LPASON,":sens:volt:chan1:lpas on\n"},
	{NAV_LPASOFF,":sens:volt:chan1:lpas off\n"},
	{NAV_DFILON,":sens:volt:chan1:dfil:wind 5\n:sens:volt:chan1:dfil:coun 10\n:sens:volt:chan1:dfil:tcon mov\n:sens:volt:chan1:dfil:stat on\n"},
	{NAV_DFILOFF,":sens:volt:chan1:dfil:stat off\n"}
};
int nav_port;

// This is an example of an exported variable
// This is the constructor of a class that has been exported.
// see Navmeter.h for the class definition
CNavmeter::CNavmeter()
{
	bOpen = false;
	value = -10000;
}
/**
 * 	Open the device and use data read to check if it works
 */
bool CNavmeter::Open(int portnum)
{

	unsigned char data;
	char DirUse[300];

	bool	bok=true;
	bOpen = true;
	return bok;
}

/**
 *	Send A command specified with nCmdID seperated with each "\n"
 */
bool CNavmeter::SendCommand(UINT nCmdID)
{
	char *pdest,*cmdstr;
	int pos;
	int totalCmd,count;
	int cmdlen;
	if(nCmdID == NAV_READ)
		value = cos(value)*10;
	return true;
};

/**
 *	except the case NAV_READ command we wait 
 *	50ms if no reply ,that's ok.else
 *	we wait MAXWAITCOUNT*50ms to accept the reply and
 *	and check if it is reasonable
 */
 
bool CNavmeter::HandleReply(int index)
{

	char buffer[50];
	char *wp;
	int pos;	
//	CString str;
	value=3;	return true;
	
//	value = atof(buffer);
//	return (!((value < -1000)||(value > 1000)));
}
void CNavmeter::Close()
{
//	m_Serial.Close();
	bOpen = false;
}
