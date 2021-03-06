// Switch.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "..\\CommonH&Libs\\Switch.h"



#define MAXSWISTORE     10   //only 10 switch will be sent at one time
//we use swilist and command with the same length to search, it is a stupid way
#define SWILENGTH	56  //the available switch number
#define SWISTNUM	28  //the status of the SWITCH
extern	CSerial m_Serial;
int swi_port;
const char  *swilist = "FHP1,FHP2,FHP3,FHP4,FHP5,FHP6,FHP7,FHP8,FHP9,FHP0,OHP1,OHP2,OHP3,OHP4,OHP5,OHP6,OHP7,OHP8,OHP9,OHP0,FHN1,FHN2,FHN3,FHN4,FHN5,FHN6,OHN1,OHN2,OHN3,OHN4,OHN5,OHN6,FKN1,FKN2,FKN3,FKN4,FKN5,FKN6,OKN1,OKN2,OKN3,OKN4,OKN5,OKN6,FKI1,FKI2,FKI3,FKI4,OKI1,OKI2,OKI3,OKI4,FHX1,FHX2,FFX1,PKTT,FST1,FST2,OHX1,OHX2,OFX1,NKTT,OST1,OST2,FHPO,OHPO,FHNO,OHNO";
const int  index[] = {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,
1,2,3,4,5,6,7,8,9,10,
-11,-12,-13,-14,-15,-16,
11,12,13,14,15,16,
-17,-18,-19,-20,-21,-22,
17,18,19,20,21,22,
-23,-24,-25,-26,
23,24,25,26,
-27,-28,-29,-30,-31,-32,
27,28,29,30,31,32,-33,33,-34,34};
const unsigned char table[] ={
		0x16,0x14,0x26,0x24,0x01,0x00,0x31,0x11,0x32,0x17,//FHP 0-9
		0x1f,0x1d,0x2f,0x2d,0x08,0x09,0x38,0x18,0x3b,0x1e,//OHP 10-19
		0x15,0x13,0x27,0x25,0x12,0x10,//FHN 20-25
		0x1c,0x1a,0x2e,0x2c,0x1b,0x19,//OHN 26-31
		0x06,0x07,0x04,0x05,0x02,0x03,//FKN 32-37
		0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,//OKN 38-43
		0x37,0x36,0x35,0x34,//FKI 44-47
		0x3e,0x3f,0x3c,0x3d,//OKI 48-51
		0x23,0x20,0x21,0x33,0x22,0x30,//FHX1,FHX2,FFX1,PKTT,FST1,FST2 52-57
		0x2a,0x29,0x28,0x3a,0x2b,0x39,//OHX1,OHX2,OFX1,NKTT,OST1,OST2, 58-63
		0x41,0x40,0x43,0x42};//FHPO,OHP0,FHNO,OHNO
static bool status[]={
		false,false,false,false,false,false,false,false,false,false,//HP1-HP10(1-10)
			false,false,false,false,false,false,//HN1-HN6 (11-16)
			false,false,false,false,false,true,//KN1-KN6 (17-22)
			false,false,false,true,//KI1-KI4 (23-26)
			false,false,false,false,false,false,false,false};//HX1,HX2,FX1,KTT,ST1,ST2 (27-32),HPO,HNO

		

// This is the constructor of a class that has been exported.
// see Switch.h for the class definition
CSwitch::CSwitch()
{ 
	ErrFlag = NO_ERR;
	bOpen = false;
	return; 
}

static void SwiReplace(char *sParam)
{
	char *psrc;
	const char *pdst;
	int i;
	const char* srcstr[]={"OHP1","OHN1","OHP2","OHN2","OHP3","OHN3",
						  "OHP4","OHN4","OHP5","OHN5","OHP6","OHN6",
						  "FHP1","FHN1","FHP2","FHN2","FHP3","FHN3",
						  "FHP4","FHN4","FHP5","FHN5","FHP6","FHN6"};
	const char* dststr[]={"OHPO","OHNO","OHPO","OHNO","OHPO","OHNO",
						  "OHPO","OHNO","OHPO","OHNO","OHPO","OHNO",
						  "FHPO","FHNO","FHPO","FHNO","FHPO","FHNO",
						  "FHPO","FHNO","FHPO","FHNO","FHPO","FHNO"};

	for(i=0;i<sizeof(srcstr)/sizeof(char*);i++)
	{
		psrc = strstr(sParam,srcstr[i]);
		if(psrc != NULL)
		{
			pdst= dststr[i];
			while(*pdst != '\0')
				*psrc++ = *pdst++;
		}
	}
}


/**
 *	we analyses the sParam and extract switch command, 
 *	then store them to a buffer and send together
 *	we use 0xff as the head and the tail
 */
bool CSwitch::DoSwi(char *sParam)
{
	unsigned char data[MAXSWISTORE+2];
	char *pdest;
	int cpos,opos; //current and old position in sParam
	int count;	//position in switch buffer
	bool bneg;
	int ipos,pos;	
	SWISTATUS ss;

	if(!bOpen)
		return false;
	ss = SwiStatusConvert();
	if(ss.bKN[0])//we have outer standard input
		SwiReplace(sParam);
	
	opos = 0;
	while(sParam[opos] != '\0')
	{
		if(sParam[opos] == ']')
			sParam[opos] = ',';
		opos++;
	}

	opos = 0;
	cpos = 0;
	data[0]=HMARK;	//add head
	count = 0;
	while(cpos!= -1)
	{
		cpos = 0;
		while((sParam[opos+cpos] != ',')&&(sParam[opos+cpos] != '\0'))
				cpos++;
		if(sParam[opos+cpos] == '\0')
			break;
		sParam[opos+cpos] = '\0';
		pdest = strstr(swilist,sParam+opos);
		if(pdest == NULL)
			break;
		pos = (int)((pdest-swilist)/5);
		opos = opos+cpos+1;
		//the check of the PKTT&NKTT
		//we just skip the PKTT and take NKTT as the change of direction
		if(pos == 55)
			continue;
		if(pos == 61)
		{
			pos = status[29]?55:61;
		}

		//the check of KI's choice only
		if((pos >= 44)&&(pos <= 47))
		{
			//cant close the final one
			//no close ki manually permitted
			continue;
		}

		if((pos >= 48)&&(pos <= 51))
		{
			if(!status[pos-26])
			{
				for(int i=0;i<4;i++)
				{
					if(status[22+i])
					{
						data[1+count++]=table[44+i];
						status[22+i] = false;
						break;
					}
				}
			}
			else
				continue;
		}
		//check the onlyness of kn's choice
		if((pos >= 32)&&(pos <= 37))
		{
			//cant close the final one
			//no close kn manually permitted
			continue;
		}

		if((pos >= 38)&&(pos <= 43))
		{
			if(!status[pos-22])
			{
				for(int i=0;i<6;i++)
				{
					if(status[16+i])
					{
						data[1+count++]=table[32+i];
						status[16+i] = false;
						break;
					}
				}
			}
			else
				continue;
		}

		ipos = index[pos];
		bneg = (ipos < 0);
		ipos = bneg?(-ipos):(ipos);
		--ipos;
		if(bneg&&(!status[ipos]))
			continue;
		if((!bneg)&&(status[ipos]))
			continue;
		status[ipos]=!status[ipos];
		data[1+count++] = table[pos];
		if(count == MAXSWISTORE)
			break;		//exceed the max limitation
	}
	//add tail
	if(count < 1)   //no switch is needed
		return true;
	//check status of hp1-hp6 ,hn1-hn6
	if(ss.bKN[0])
	{
		//add for disconnect
		if(status[0]) data[1+count++]=0x0;
		if(status[1]) data[1+count++]=0x1;
		if(status[2]) data[1+count++]=0x2;
		if(status[3]) data[1+count++]=0x3;
		if(status[4]) data[1+count++]=0x4;
		if(status[5]) data[1+count++]=0x5;
		if(status[10]) data[1+count++]=0x20;
		if(status[11]) data[1+count++]=0x21;
		if(status[12]) data[1+count++]=0x22;
		if(status[13]) data[1+count++]=0x23;
		if(status[14]) data[1+count++]=0x24;
		if(status[15]) data[1+count++]=0x25;
	}

	data[1+count++]=EMARK;
	
	m_Serial.SendData(swi_port,data,count+1);
	
		for(int i=0;i<3000;i++)
	{
		Sleep(1);
		if(m_Serial.ReadDataWaiting() > 0)
		{
			Sleep(1);
			break;
		}
	}


	if(m_Serial.ReadDataWaiting() <= 0)
	{
	   //wait too long-in DoSwi
 		ErrFlag = WAIT_ERR;
		return false;
	}
	cpos = m_Serial.ReadData(data,MAXSWISTORE);
	
	if((cpos == 4)&&(data[0] == HMARK)&&(data[3] == EMARK)) //normal return;
	{
		P2 = data[1];
		P3 = data[2];
		ss = SwiStatusConvert();
		if(ss.bOl1||ss.bOl2)
		{
			Sleep(500);
			ss = GetSwitchStatus();
			if(ss.bOl1||ss.bOl2)
			{
				Sleep(2000);
				ss = GetSwitchStatus();
				if(ss.bOl1||ss.bOl2)
					return false;
			}
		}
		return true;
	}
	//unexpected reply - in DoSwi;
	return false;
}
/**
 *	Open():we just open the serial and send 5 ff to make normal 
 *	Besides ,we do nothing check at all
 */
bool CSwitch::Open(int portnum)
{
	unsigned char data;

	if(bOpen)
		return true;
	swi_port = 's';
	int nPort = -1;
	while(nPort++ < 8){
		ErrFlag = NO_ERR;
		if(nPort > 0)
			m_Serial.SetPort(nPort);
		if(!m_Serial.Open(portnum,9600,0,0,8))
			continue;

		//	swi_port = portnum;
		bOpen = true;
		Sleep(10);

		while(m_Serial.ReadDataWaiting() > 0)
			m_Serial.ReadData(&data,1);
		if(!DoAction(QMARK))
		{
			m_Serial.Close();
			bOpen = false;
			continue;
		}
		if(!DoAction(RMARK))
		{
			m_Serial.Close();
			bOpen = false;
			continue;
		}
		return true;
	}
	return false;
}
/**
 *	We just close the serial port and do nothing else
 */
void CSwitch::Close()
{
	m_Serial.Close();
	bOpen = false;
}

/**
 *	here is function which the main function should 
 *	check frequently to see if there is some overload occurred
 *	
 */
SWISTATUS CSwitch::GetSwitchStatus()
{
	
//	SWISTATUS ss;
	if(!DoAction(QMARK))
	{
		P2 = 0xff;
		P3 = 0xff;
	}
	//TODO :convert the P2,P3 to the status value
	return SwiStatusConvert();
}

int CSwitch::CheckOverLoad()
{
	SWISTATUS ss;
	//CString str;
	int ov;
	ss = GetSwitchStatus();
	//str.Format("%x,%x",P2,P3);
	//::AfxMessageBox(str);
	ov = 0;
	if(ss.bOl1)
		ov++;
	if(ss.bOl2)
		ov++;

	return ov;

}


bool CSwitch::DoAction(unsigned char uMark)
{
	int cpos,i;
	unsigned char data[4];
	if(!bOpen)
		return false;

	data[0]=HMARK;
	data[1]=uMark;
	data[2]=EMARK;

	m_Serial.SendData(swi_port,data,3);
	
	switch(uMark)
	{
	case RMARK:
			for(i=0;i<5000;i++)
			{
				Sleep(1);
				if(m_Serial.ReadDataWaiting() > 0)
				{
					Sleep(1);
					break;
				}
			}
			cpos = sizeof(status)/sizeof(bool);
			for(i=0;i<cpos;i++)
				status[i]=false;
			status[25]=true;
			status[21]=true;
			break;
	case QMARK:
			for(i=0;i<1000;i++)
			{
				Sleep(1);
				if(m_Serial.ReadDataWaiting() > 0)
				{
					Sleep(1);
					break;
				}
			}
			break;
	default:
		return false;
	}

	if(m_Serial.ReadDataWaiting() <= 0)
	{
		//("wait too long in do action\n");
 		ErrFlag = WAIT_ERR;
		return false;
	}

	cpos = m_Serial.ReadData(data,4);


	if((cpos == 4)&&(data[0] == HMARK)&&(data[3] == EMARK)) //normal return;
	{
		P2 = data[1];
		P3 = data[2];
	}
	else
	{
		//unexpected reply -DoAction
		ErrFlag = UNKN_ERR;
//		return false;
	}
	while(m_Serial.ReadDataWaiting() > 0)
	{
		cpos = m_Serial.ReadData(data,4);
	}

	return (ErrFlag != UNKN_ERR);	
}
#define CHECKBIT(a,b)   (a&(0x01<<b))?false:true
SWISTATUS CSwitch::SwiStatusConvert()
{
	SWISTATUS ss;

	ss.bRn=CHECKBIT(P2,0);
	ss.bKN[0]=CHECKBIT(P2,1);
	ss.bKN[1]=CHECKBIT(P2,1);
	ss.bKN[2]=CHECKBIT(P2,1);
	ss.bKN[3]=CHECKBIT(P2,1);
	ss.bKN[4]=CHECKBIT(P2,1);
	ss.bKN[5]=CHECKBIT(P2,1);
	ss.bOl1=CHECKBIT(P2,2);
	ss.bOl2=CHECKBIT(P2,3);

	return ss;
}
