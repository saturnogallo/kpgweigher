/*
Module : PARALLEL.H
Purpose: Declaration for an MFC class to encapsulate parallel ports
Created: PJN / 28-12-1999
History: None

Copyright (c) 1999 - 2000 by PJ Naughter.  
All rights reserved.

*/



///////////////////// Macros / Structs etc //////////////////////////

#ifndef __PARALLEL_H__
#define __PARALLEL_H__


#ifndef __AFXTEMPL_H__
//#pragma message("To avoid this message, put afxtempl.h in your pre compiled header (normally stdafx.h)")
#include <afxtempl.h> 
#endif




/////////////////////////// Classes ///////////////////////////////////////////


////// Parallel port exception class ////////////////////////////////////////////

void AfxThrowParallelException(DWORD dwError = 0);

class CParallelException : public CException
{
public:
//Constructors / Destructors
	CParallelException(DWORD dwError);
	~CParallelException();

//Methods
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL GetErrorMessage(LPTSTR lpstrError, UINT nMaxError,	PUINT pnHelpContext = NULL);
	CString GetErrorMessage();

//Data members
	DWORD m_dwError;

protected:
	DECLARE_DYNAMIC(CParallelException)
};


////// The class used to hold a parallel ports settings /////////////////////////

class CParallelPortSettings
{
public:
//Typedefs / Defines
  enum ParallelPortType
  {
    ParallelTypeUndefined,
    ParallelTypeSPP,
    ParallelTypePS2,
    ParallelTypeEPP,
    ParallelTypeECP,
  };

  enum ECPPortMode
  {
    ECPModeUndefined,
    ECPModeSPP,
    ECPModePS2,
    ECPModeFastCentronics,
    ECPModeECP,
    ECPModeEPP,
    ECPModeTest,
    ECPModeConfiguration
  };

//Constructors / Destructors
  CParallelPortSettings();
  CParallelPortSettings(const CParallelPortSettings& state);

//Standard methods
  CParallelPortSettings& operator=(const CParallelPortSettings& state);

//Member variables
  unsigned short   m_nBaseAddress; //The base address of the parallel port e.g. 0x278
  ParallelPortType m_Type;         //The port type
  ECPPortMode      m_ECPMode;      //The ECP mode if the type is ECP 
};

                                  
//// The actual parallel port class /////////////////////////////////////////////

class CParallelPort : public CObject
{
public:  
//Constructors / Destructors
  CParallelPort();
  ~CParallelPort();

//Methods
  void                               Open(int nPort=1);
  inline BOOL                        IsOpen() const;
  void                               Close();
  void                               SetECPMode(CParallelPortSettings::ECPPortMode mode);
  CParallelPortSettings::ECPPortMode GetECPMode();
  void                               WriteControl(int nData);
  int                                ReadControl();
  void                               WriteData(int nData);
  int                                ReadData();
  int                                ReadStatus();
  BOOL                               ReadByteUsingNibbleMode(BYTE& byData);
  BOOL                               WriteByteUsingNibbleMode(BYTE byData);
  inline BOOL                        ReadUsingNibbleMode(void* lpBuf, DWORD dwBytes);
  inline BOOL                        WriteUsingNibbleMode(const void* lpBuf, DWORD dwBytes);
  void                               SetTimeout(DWORD dwTimeout) { m_dwTimeout = dwTimeout; };
  DWORD                              GetTimeout() const { return m_dwTimeout; };
  CParallelPortSettings              GetSettings() const;

//Static methods
  static BOOL PortPresent(int nPort);

//MFC Diagnostics
#ifdef _DEBUG
  void Dump(CDumpContext& dc) const;
#endif

protected:
//typedefs
  typedef UCHAR (_stdcall DLPORTREADPORTUCHAR)(ULONG Port);
  typedef DLPORTREADPORTUCHAR* LPDLPORTREADPORTUCHAR;
  typedef VOID (_stdcall DLPORTWRITEPORTUCHAR)(ULONG Port, UCHAR Value);
  typedef DLPORTWRITEPORTUCHAR* LPDLPORTWRITEPORTUCHAR;

//Methods
  static BOOL        InitializeDriverLINX();
  static void        DeInitializeDriverLINX();
  static BOOL        RunningOnNT();
  static BOOL        GetPort(unsigned short nBaseAddress, CParallelPortSettings& settings);
  static BOOL        GetECPPort(unsigned short nBaseAddress);
  static BOOL        GetEPPPort(unsigned short nBaseAddrsss);
  static BOOL        GetPS2Port(unsigned short nBaseAddress);
  static BOOL        GetSPPPort(unsigned short nBaseAddress);
  static int         GetEPPTimeoutBit(unsigned short nBaseAddress);
  static CParallelPortSettings::ECPPortMode ReadECPMode(unsigned short nBaseAddress);
  inline static int  _inp(unsigned short port);
  inline static int  _outp(unsigned short port, int databyte);
  inline static void WriteControl(unsigned short nBaseAddress, int nData);
  inline static int  ReadControl(unsigned short nBaseAddress);
  inline static void WriteData(unsigned short nBaseAddress, int nData);
  inline static int  ReadData(unsigned short nBaseAddress);
  inline static int  ReadStatus(unsigned short nBaseAddress);

//Member variables
  HANDLE         m_hPort;         //SDK handle to lock Win32 access to the port
  int            m_nPortIndex;    //index of the current port into m_Ports
  DWORD          m_dwTimeout;     //Timeout in Milliseconds for reads and writes
  unsigned short m_nBaseAddress;  //The base address of the port

//Static memmber variables
  static BOOL                                                  sm_bRunningOnNT;         //Are we running on NT / Windows 2000
  static int                                                   sm_nRefCount;            //Reference count for the class
  static HINSTANCE                                             sm_hDLINX;               //handle to the DriverLINX dll.
  static LPDLPORTREADPORTUCHAR                                 sm_lpfnDlPortReadUchar;  //Function pointer for writing via DriverLINX
  static LPDLPORTWRITEPORTUCHAR                                sm_lpfnDlPortWriteUchar; //Function pointer for rading via DriverLINX
  static CArray<CParallelPortSettings, CParallelPortSettings&> sm_Ports;                //All the parallel ports available on this machine

  DECLARE_DYNAMIC(CParallelPort)
};



//// A Simple class to transfer and receive files using nibble mode

class CParallelPortFileTransfer
{
public:
//Constructors / Destructors
  CParallelPortFileTransfer(CParallelPort* pPort);
  ~CParallelPortFileTransfer();

//Methods
  void SendFile(const CString& sLocalFile);
  void ReceiveFile(const CString& sLocalFile);

protected:
  CParallelPort* m_pPort;
};


#endif //__PARALLEL_H__