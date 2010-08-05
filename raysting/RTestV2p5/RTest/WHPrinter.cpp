#include "WHPrinter.h"
#include "stdafx.h"

const MHCMDTYPE   cmdList[]={                              
	{ID_CHARSET1_SET,"%n1B6"},
	{ID_CHARSET2_SET,"%n1B7"},
	{ID_CHINASET_SET,"%n1B8"},
	{ID_PRINT_RET,"%n0A"},   
	{ID_RETLINE_SET,"%n1BJ%i"},
	{ID_DISLINE_SET,"%n1B1%i"},
	{ID_DISCHAR_SET,"%n1B%n20%i"},
	{ID_RETPAPER_RUN,"%n0C"},
	{ID_LENPARER_SET,"%n1BC%i"},
	{ID_LENDING_SET,"%n1BN%i"},
	{ID_LENDING_CAN,"%n1BO"},
	{ID_VTABLE_SET,"%n1BB%s%n00"},
	{ID_VTABLE_RUN,"%n0B"},  
	{ID_HTABLE_SET,"%n1BD%s%n00"},
	{ID_HTABLE_RUN,"%n09"},  
	{ID_SPACE_PRT,"%n1Bf%i%i"},
	{ID_RLIMIT_SET,"%n1BQ%i"},
	{ID_LLIMIT_SET,"%n1Bl%i"},
	{ID_HMAGNIFY_SET,"%n1BU%i"},
	{ID_VMAGNIFY_SET,"%n1BV%i"},
	{ID_CHARMAGNIFY_SET,"%n1CW%i"},
	{ID_VHMAGNIFY_SET,"%n1BW%i"},
	{ID_VHMAGNIFY2_SET,"%n1BX%i%i"},
	{ID_HMAGNIFYTWICE_SET,"%n0E"},
	{ID_HMAGNIFYNONE_SET,"%n14"},
	{ID_DLINEPERMIT_SET,"%n1B-%i"},
	{ID_ULINEPERMIT_SET,"%n1B+%i"},
	{ID_SLINEPERMIT_SET,"%n1B/%i"},
	{ID_UDNOTESEL_SET,"%n1Cr%i"},
	{ID_ZIGPRINT_SET,"%n1CG%i"},
	{ID_ZIGPRINT_CAN,"%n1CH"},
	{ID_ABGPRINT_PERMIT,"%n1Bi%i"},
	{ID_APOSPRINT_PERMIT,"%n1Bc%i"},
	{ID_VERTPRINT_SET,"%n1CJ"},
	{ID_HORIPRINT_SET,"%n1CK"},
	{ID_ROTATEPRINT_SET,"%n1CI%i"},
	{ID_DIRECTPRINT_SET,"%n1Be%i"},
	{ID_CHARDEFINE_SET,"%n1B&%s"},
	{ID_CHAREPLACE_SET,"%n1B%n25%s%n00"},
	{ID_CHARRESTORE_SET,"%n1B:"},
	{ID_CHARREPLACE2_SET,"%n1B;"},
	{ID_POINTPRINT_SET,"%n1BK%s"},
	{ID_CURVEPRINT_SET,"%n1B'%s"},
	{ID_TIAOPRINT_SET,"%n1BE%s%n00"},
	{ID_INITPRINT_SET,"%n1B@"},
	{ID_CRPRINT_SET,"%n0D"},
	{ID_LINEDELETE_SET,"%n18"},
	{ID_HEXPRINTPERMIT_SET,"%n1B%n22%i"},
	{ID_NORMALDATA_SEND,"%s"}
};                              

/**
 *	Char2Byte(BYTE highb,BYTE lowb)
 *	Combine the highb(low 4bit is valuable) 
 *	and lowb(low 4bit is valuable) to be a byte
 */
BYTE Char2Byte(BYTE highb,BYTE lowb)
{
	BYTE b,c;
	if((highb >= 'a')&&(highb <= 'f'))
		b = (highb-'a'+0x0a);
	else
		if((highb >= 'A')&&(highb <= 'F'))
			b = (highb-'A'+0x0a);
		else
			if((highb >= '0')&&(highb <= '9'))
				b = highb-'0';

	if((lowb >= 'a')&&(lowb <= 'f'))
		c = (lowb-'a'+0x0a);
	else
		if((lowb >= 'A')&&(lowb <= 'F'))
			c = (lowb-'A'+0x0a);
		else
			if((lowb >= '0')&&(lowb <= '9'))
				c = lowb-'0';
	return (b<<4)+c;
}
CWHPrinter::CWHPrinter()
{
	bOpen = false;
}
/**
 *	Open(int portnum)
 *
 *	Open the device and  do init process here
 *	Input Value : 
 *		portnum   //the Pararrel port we use
 */
 
bool CWHPrinter::Open(int portnum)
{
	unsigned char state;
#ifdef NOPRINTER
	return true;
#endif

	if(bOpen)
		return true;
		
	m_Port.Open(portnum);
	state = m_Port.ReadStatus();
	if(!((state&FLAG_SELE)&&(state&FLAG_ERROR)))
		return false;
	
	if(!PrinterWrite(ID_INITPRINT_SET,_T("")))
		return false;
	if(!PrinterWrite(ID_CHARSET1_SET,_T("")))
		return false;

	bOpen = true;
	return true;
}

bool CWHPrinter::PrinterWrite(unsigned char nCmdID,CString sParam)
{
	int cmdpos,sparpos, cmdlen,sparlen;
	int totalCmd;
	char *param;
	BYTE tmpbyte;
	MHCMDTYPE cmd;
	unsigned char PState;
#ifdef NOPRINTER
	return true;
#endif

	if(!bOpen)
		return true;

	PState = m_Port.ReadStatus();
	if(!((PState&FLAG_SELE)&&(PState&FLAG_ERROR)))
		return false;

	totalCmd = sizeof(cmdList)/sizeof(MHCMDTYPE);
	
	if(nCmdID > totalCmd)	//is not the command available
		return false;
		
	cmd = cmdList[nCmdID];
	param = cmd.Data;
	cmdlen = strlen(param);
	sparlen = sParam.GetLength();
	
	cmdpos = 0;
	sparpos = 0;
	while(cmdpos < cmdlen)
	{
		if(param[cmdpos] != '%')
		{
			if(!SendDataByByte(param[cmdpos]))
				return false;
		}
		else
		{
			//character with the meaning
			cmdpos++;
			switch(param[cmdpos])
			{
			case 'n':
				//one byte combined from the following 2 char
				tmpbyte = Char2Byte(param[cmdpos+1],param[cmdpos+2]);
				cmdpos += 2;
				if(!SendDataByByte(tmpbyte))
					return false;
				break;
			case 'i':
				//one byte extracted from sParam;
				tmpbyte = Char2Byte(sParam.GetAt(sparpos),sParam.GetAt(sparpos+1));
				sparpos += 2;
				if(!SendDataByByte(tmpbyte))
					return false;
				break;
			case 's':
				while(sparpos < sparlen)
				{
					if(!SendDataByByte(sParam.GetAt(sparpos++)))
						return false;
				}
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		cmdpos++;
	}
	return true;
}
/**
 *	Close() :close the device
 */
void CWHPrinter::Close()
{
#ifdef NOPRINTER
	return;
#endif
	m_Port.Close();
	bOpen = false;
}
/**
 *	Send one byte to the device
 */
bool CWHPrinter::SendDataByByte(unsigned char uData)
{
	unsigned char PState;
	int count;
	PState = 0xff;
	
	//wait for the device for ready-state
	count = 0;


	while(((PState&FLAG_BUSY) == FLAG_BUSY)||((PState&FLAG_ACK) == 0))
	{
		Sleep(1);
		if(count++ > MAXWAITCOUNT)
			return false;
		PState = m_Port.ReadStatus();
	}
	
	m_Port.WriteData(uData);
	m_Port.WriteControl(!FLAG_STB);
	this->SleepByMicroSecond(20);
	m_Port.WriteControl(FLAG_STB);
	this->SleepByMicroSecond(30);
	m_Port.WriteData(0x00);
	return true;
}
/**
 *	SleepByMicroSecond(1) will sleep 1us
 *	since is do by software so is not accurate
 */
void CWHPrinter::SleepByMicroSecond(int MicroSeconds)
{
	int mcount,idlecount;
	mcount = 0;
	while(mcount++ < MicroSeconds)
	{
		idlecount = 0;
		while(idlecount++ < WAITMSCOUNT);
	}
}