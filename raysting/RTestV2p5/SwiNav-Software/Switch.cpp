// Switch.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "stdio.h"
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
const char* swistr[64]={
		"FHP1","FHP2","FHP3","FHP4","FHP5","FHP6","FHP7","FHP8","FHP9","FHP0",
		"OHP1","OHP2","OHP3","OHP4","OHP5","OHP6","OHP7","OHP8","OHP9","OHP0",
		"FHN1","FHN2","FHN3","FHN4","FHN5","FHN6",
		"OHN1","OHN2","OHN3","OHN4","OHN5","OHN6",
		"FKN1","FKN2","FKN3","FKN4","FKN5","FKN6",
		"OKN1","OKN2","OKN3","OKN4","OKN5","OKN6",
		"FKI1","FKI2","FKI3","FKI4",
		"OKI1","OKI2","OKI3","OKI4",
		"FHX1","FHX2","FFX1","PKTT","FST1","FST2",
		"OHX1","OHX2","OOX1","NKTT","OST1","FST2"};
		

// This is the constructor of a class that has been exported.
// see Switch.h for the class definition
CSwitch::CSwitch()
{ 
	ErrFlag = NO_ERR;
	bOpen = false;
	return; 
}



/**
 *	we analyses the sParam and extract switch command, 
 *	then store them to a buffer and send together
 *	we use 0xff as the head and the tail
 */
static void showmsg(const char *data)
{
	MessageBox(GetDesktopWindow(),data,NULL,MB_OK);

}
bool CSwitch::DoSwi(char *sParam)
{
	unsigned char data[MAXSWISTORE+2];
	char buffer[200];
	char *pdest;
	int cpos,opos; //current and old position in sParam
	int count;	//position in switch buffer
	bool bneg;
	int ipos,pos;	
	SWISTATUS ss;
	if(!bOpen)
		return false;
//	showmsg((LPCSTR)sParam);
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
		//showmsg((LPCSTR)pdest);
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
	data[1+count++]=EMARK;
//	for(pos = 0;pos<64;pos++)
//	{
//		sprintf(buffer,"swistr[%d]=%s",pos,swistr[pos]);
//		MessageBox(GetDesktopWindow(),(LPCSTR)buffer,NULL,MB_OK);
//	}
		for(ipos=1;ipos < count;ipos++)
	{
		for(pos = 0;pos<64;pos++)
			if(data[ipos]==table[pos])
			{
//				MessageBox(GetDesktopWindow(),swistr[pos],NULL,MB_OK);
//				sprintf(buffer,"data[%d]=%d,swistr[%d]=%s,table[%d]=%d",ipos,data[ipos],pos,swistr[pos],pos,table[pos]);
//				MessageBox(GetDesktopWindow(),(LPCSTR)buffer,NULL,MB_OK);
			}

	}
	P2=0;P3=0;
	return true;
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
	bOpen = true;
	return true;
}
/**
 *	We just close the serial port and do nothing else
 */
void CSwitch::Close()
{
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
	//TODO :convert the P2,P3 to the status value
	return SwiStatusConvert();
}

int CSwitch::CheckOverLoad()
{
	return 0;

}


bool CSwitch::DoAction(unsigned char uMark)
{
	int cpos,i;
	if(uMark == RMARK)
		if(bOpen)
		{
			cpos = sizeof(status)/sizeof(bool);
			for(i=0;i<cpos;i++)
				status[i]=false;
			status[25]=true;
			status[21]=true;
	
//			MessageBox(GetDesktopWindow(),"reset all swi",NULL,MB_OK);
		}
	return true;
}
#define CHECKBIT(a,b)   (a&(0x01<<b))?false:true
SWISTATUS CSwitch::SwiStatusConvert()
{
	SWISTATUS ss;
	P2=0;P3=0;
	ss.bRn=CHECKBIT(P2,0);
	ss.bKN[0]=CHECKBIT(P2,1);
	ss.bKN[1]=CHECKBIT(P2,1);
	ss.bKN[2]=CHECKBIT(P2,1);
	ss.bKN[3]=CHECKBIT(P2,1);
	ss.bKN[4]=CHECKBIT(P2,1);
	ss.bKN[5]=CHECKBIT(P2,1);
	ss.bOl1= false; //CHECKBIT(P2,2);
	ss.bOl2=CHECKBIT(P2,3);

	return ss;
}
