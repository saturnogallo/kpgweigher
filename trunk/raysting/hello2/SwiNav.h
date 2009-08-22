#ifndef _SWINAV_H_
#define _SWINAV_H_

#include "stdafx.h"
#include "PSerialPort.h"
#include "afxtempl.h"
#include "TCHAR.h"
#include "math.h"

static const char* swi_reset = "OH3,FH1,FH2,FH4,FH5,FH6,OKI3,FKI1,FKI2,FKI4,FKI5,FKI6,FKI7,OHX1,FSQT,FEXT,FKTT,FCH1,OCH2";
typedef struct _SWI_VAR{
	bool rx;
	bool rs;
	bool ov1;
	bool ov2;
}SWI_VAR;

typedef struct _RELAY{
	char* swiname;
	char type;
	char* grpname;
	int status;
	unsigned char faddr;
	unsigned char oaddr;
}RELAY;

static RELAY switable[] = {
{"KI2",'s',"ki",0,0x32,0x3b},
{"KI1",'s',"ki",0,0x21,0x28},
{"KI3",'s',"ki",1,0x3a,0x33},
{"KI4",'s',"ki",0,0x02,0x0b},
{"KI6",'s',"ki",0,0x25,0x2c},
{"KI5",'s',"ki",0,0x24,0x2d},
{"KI7",'s',"ki",0,0x2e,0x27},


{"H1",'s',"h",0, 0x20,0x29},
{"H2",'s',"h",0, 0x42,0x43},
{"H3",'s',"h",1, 0x16,0x1f},
{"H4",'s',"h",0, 0x40,0x41},
{"H5",'s',"h",0, 0x00,0x09},
//{"H6",'n',"h",0, 0xff,0xff},

{"CH1",'n',"ch",1, 0x35,0x3c},
//{"CH2",'n',"ch",0, 0xff,0xff},

{"EXT",'n',"u",0, 0x2b,0x22},
{"HX1",'n',"u",1, 0x08,0x01},
{"KTT",'r',"u",0, 0x23,0x2a},
{"SQT",'n',"u",0, 0x2f,0x26}
};

static const char* navc[] = {
"Keithley2000:range6::SENS:VOLT:DC:RANG 100\n",
"Keithley2000:range4::SENS:VOLT:DC:RANG 1\n",
"Keithley2000:init:*RST\n*CLS\n:INIT:CONT ON\n:ABORT\n:SENS:FUNC 'VOLT:DC'\n:SENS:VOLT:DC:RANG:AUTO ON\n:SENS:VOLT:DC:AVER:STAT OFF\n:SENS:VOLT:DC:DIG 7\n:SYST:AZER:STAT ON\n:SENS:VOLT:DC:NPLC 1\n",
"Keithley2000:afltoff::SENS:VOLT:DC:AVER:STAT OFF\n",
"Keithley2000:remote:\n",
"Keithley2000:close:\n",
"Keithley2000:readonly:true",
"Keithley2000:aflton::SENS:VOLT:DC:AVER:COUN 10\n:SENS:VOLT:DC:AVER:TCON MOV\n:SENS:VOLT:DC:AVER:STAT ON\n",
"Keithley2000:slowmode::SENS:VOLT:DC:NPLC 10\n",
"Keithley2000:read::FETC?\n",
"Keithley2000:range5::SENS:VOLT:DC:RANG 10\n",
"Keithley2000:zeroon::SENS:VOLT:DC:REF:ACQ\n:SENS:VOLT:DC:REF:STAT ON\n",
"Keithley2000:dflton::SENS:VOLT:DC:AVER:COUN 10\n:SENS:VOLT:DC:AVER:TCON MOV\n:SENS:VOLT:DC:AVER:STAT ON\n",
"Keithley2000:zerooff::SENS:VOLT:DC:REF:STAT OFF\n",
"Keithley2000:range1::SENS:VOLT:DC:RANG:AUTO ON\n",
"Keithley2000:range3::SENS:VOLT:DC:RANG .1\n",
"Keithley2000:range2::SENS:VOLT:DC:RANG .01\n",
"Keithley2000:dfltoff::SENS:VOLT:DC:AVER:STAT OFF\n"
};

extern void l_update_buffer(char *data,int length,DWORD userdata);
class CSwiNav
{
public:
	CSwiNav() { open(); };
	~CSwiNav() { close(); };
	BOOL open();
	
	void close();
	void turn_nav(char *cmd);
	void reset_swi();
	void turn_swi(const char *cmd);
	bool read_nav(double &value);
	SWI_VAR read_swi();
	void update_buffer(char *data,int length);
	double reading;
	void parse_buffer();
private:
	bool isnumber(unsigned char c);
	void swi_force_write(const char *cmd);
	void swi_convert(unsigned char p2,unsigned char p3);

	CPSerialPort m_serial;
	SWI_VAR swivar;
	CArray<unsigned char,unsigned char> m_list;
};


#endif;
