#ifndef _AFX_WHPRINTER_H_021106
#define _AFX_WHPRINTER_H_021106

#include "ParallelPort.h"
#include "stdafx.h"
/**
 *	MHCMDTYPE structure
 *
 *	the structure is used to store
 *	the command available of the Printer WH_1608A
 *    
 */

typedef struct _MHCMDTYPE
{
	UINT CmdID;     //>command ID
	char *Data;	//>the const string of command 
			    //>%nab : 0xab will be sent
			    //>%i   : one byte(combine two byte)extracted from sParam will be sent
			    //>%s	: String(org ones)from sParam will be sent
}MHCMDTYPE;

#define BYTE unsigned char
#define ID_CHARSET1_SET        0
#define ID_CHARSET2_SET        1
#define ID_CHINASET_SET        2
#define ID_PRINT_RET           3
#define ID_RETLINE_SET         4
#define ID_DISLINE_SET         5
#define ID_DISCHAR_SET         6
#define ID_RETPAPER_RUN        7
#define ID_LENPARER_SET        8
#define ID_LENDING_SET         9
#define ID_LENDING_CAN         10
#define ID_VTABLE_SET          11
#define ID_VTABLE_RUN          12
#define ID_HTABLE_SET          13
#define ID_HTABLE_RUN          14
#define ID_SPACE_PRT           15
#define ID_RLIMIT_SET          16
#define ID_LLIMIT_SET          17
#define ID_HMAGNIFY_SET        18
#define ID_VMAGNIFY_SET        19
#define ID_CHARMAGNIFY_SET     20
#define ID_VHMAGNIFY_SET       21
#define ID_VHMAGNIFY2_SET      22
#define ID_HMAGNIFYTWICE_SET   23
#define ID_HMAGNIFYNONE_SET    24
#define ID_DLINEPERMIT_SET     25
#define ID_ULINEPERMIT_SET     26
#define ID_SLINEPERMIT_SET     27
#define ID_UDNOTESEL_SET       28
#define ID_ZIGPRINT_SET        29
#define ID_ZIGPRINT_CAN        30
#define ID_ABGPRINT_PERMIT     31
#define ID_APOSPRINT_PERMIT    32
#define ID_VERTPRINT_SET       33
#define ID_HORIPRINT_SET       34
#define ID_ROTATEPRINT_SET     35
#define ID_DIRECTPRINT_SET     36
#define ID_CHARDEFINE_SET      37
#define ID_CHAREPLACE_SET      38
#define ID_CHARRESTORE_SET     39
#define ID_CHARREPLACE2_SET    40
#define ID_POINTPRINT_SET      41
#define ID_CURVEPRINT_SET      42
#define ID_TIAOPRINT_SET       43
#define ID_INITPRINT_SET       44
#define ID_CRPRINT_SET         45
#define ID_LINEDELETE_SET      46
#define ID_HEXPRINTPERMIT_SET  47
#define ID_NORMALDATA_SEND     48
#define ID_MAXCOMMAND	       49

#define MAXWAITCOUNT	 1000  //the max time wait for ready is Sleep(1)*MAXWAITCOUNT
#define WAITMSCOUNT 	 20    //make the SleepByMicroSecond(1) to be 1us accurate
#define FLAG_BUSY   	 0x80
#define FLAG_ACK    	 0x40
#define FLAG_PAPER       0x20
#define FLAG_SELE        0x10
#define FLAG_ERROR       0x08
#define FLAG_STB	 0x01                            


/**
 *	CWHPrinter : the device of WH_1608A
 *
 */		               	                       
class CWHPrinter
{
public:
	CWHPrinter();
	bool Open(int portnum);  //<open the device with the parrarel port number
	bool PrinterWrite(BYTE nCmdId,CString sParam); //<ID_NORMALDATA_SEND
	
	void Close();		 //<closethe device
private:
	bool bOpen;
	CParallelPort m_Port;
private:
	bool SendDataByByte(BYTE uData);  //<one byte send
	void SleepByMicroSecond(int MicroSeconds); //sleep MicroSecond using software
};
#endif
		               
		               
	                       
		               
	                       
		               
		               
	                       
                               
                               
	                       
	                       
                               