// Navmeter.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "..\\CommonH&Libs\\Navmeter.h"
#include "math.h"

#define MAXWAITCOUNT	50	//longest time for read reply = MAXWAITCOUNT*50ms
extern	CSerial m_Serial;
const NMCMDTYPE cmdList[]={
	{NAV_INIT,"*RST\n*CLS\n:INIT:CONT ON;:ABORT\n:SENS:FUNC 'VOLT:DC'\n:SENSE:CHAN 1\n:sens:volt:rang:auto on\n:sens:volt:chan1:lpas off\n:SENS:VOLT:DC:NPLC 1\nVOLT:DIG 8\n:syst:azer on\n"},
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
	{NAV_DFILOFF,":sens:volt:chan1:dfil:stat off\n"},
	{NAV_SLOWMODE,":SENS:VOLT:DC:NPLC 10\n"}
};
int nav_port;

bool bLPOn,bDFOn,bSlow;
bool SkipCommand(UINT cmd){
	bool bRet=false;
	if(cmd == NAV_INIT){
		bLPOn=false;
		bDFOn=false;
		bSlow=false;
	}
	if(cmd == NAV_LPASON){
		bRet = bLPOn;
		bLPOn = true;
	}
	if(cmd == NAV_LPASOFF){
		bRet = !bLPOn;
		bLPOn = false;
	}
	if(cmd == NAV_DFILON){
		bRet = bDFOn;
		bDFOn = true;
	}
	if(cmd == NAV_DFILOFF){
		bRet = !bDFOn;
		bDFOn = false;
	}
	if(cmd == NAV_SLOWMODE){
		bRet = bSlow;
		bSlow = true;
	}
	return bRet;
}
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

	bool	bok;
//	CString str;
	nav_port = 'n';//	nav_port = portnum;
	if(bOpen)
		return true;

	::GetCurrentDirectory(300,DirUse);
	strcat(DirUse,"\\Data\\Default\\Navmeter.def");
	int nPort = 0;
	while(nPort++ < 1){
//		m_Serial.SetPort(nPort);
		if(!m_Serial.Open(portnum,9600,0,0,8))
			continue;
		bOpen = true;

		Sleep(50);
		while(m_Serial.ReadDataWaiting() > 0)
			m_Serial.ReadData(&data,1);

		bok = true;
		if(!SendCommand(NAV_INIT))
			bok=false;
	
		if(bok){
			::Sleep(2000);
			while(m_Serial.ReadDataWaiting() > 0)
				m_Serial.ReadData(&data,1);

			value = -10000;
			if(!SendCommand(NAV_READ))
				bok =false;
			if(value < -1000)	//failed to read a data from the device
				bok = false;
		}
		if(bok == false)
		{
			m_Serial.Close();
			bOpen = false;
			continue;
		}
		break;
	}
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
//	CString str;

	if(bOpen == false)
		return false;
	if(SkipCommand(nCmdID))		return true;
	totalCmd = sizeof(cmdList)/sizeof(NMCMDTYPE);
	for(count = 0;count < totalCmd;count++)
	{
		if(cmdList[count].CmdID == nCmdID)
			break;
	}
	if(count >= totalCmd)	//is not the command available
		return true;
	pos=0;

	cmdstr=cmdList[count].Data;

	cmdlen = strlen(cmdstr);
	while(true)
	{
		pdest = strstr(cmdstr+pos,"\n");
		if(pdest == NULL)	
			return true;
		m_Serial.SendData(nav_port,cmdstr+pos,2+pdest-cmdstr-pos);
		if(!HandleReply(count))
		{
			return false;
		}
		pos += 2+pdest-cmdstr-pos;
		if( (pos-cmdlen) >= -3)
			return true;
//		Sleep(200);		
	}
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
//	value=3;	return true;
	
	

	if(cmdList[index].CmdID != NAV_READ)
	{
		Sleep(100);
		if(cmdList[index].CmdID == NAV_SLOWMODE) Sleep(3000);
		return (m_Serial.ReadDataWaiting() <= 0);
	}
	pos = 0;
	wp = buffer;
	
	while(pos++ < MAXWAITCOUNT)
	{
		if(bSlow) Sleep(2);
		Sleep(1);
		if(m_Serial.ReadDataWaiting())
		{
			m_Serial.ReadData(wp++,1);
			if(((*(wp-1)) == '\r')||((*(wp-1)) == '\n'))
				break;
			pos = 0;
		}
		if(wp == buffer+49)
			return false;
	}
	*wp ='\0';
	if(wp == buffer)//no data recieved 
		return false;
	value = atof(buffer);
	return (!((value < -1000)||(value > 1000)));
}
void CNavmeter::Close()
{
	m_Serial.Close();
	bOpen = false;
}
