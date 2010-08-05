/*
Module : PARALLEL.CPP
Purpose: Implementation for an MFC class to encapsulate parallel ports
Created: PJN / 28-12-1999
History: PJN / 06-03-2000 Code now throws an exception if no Win32 parallel ports are available
         PJN / 04-04-2000 Code handles the case where it is run on NT / Windows 2000 by throwing
                          an exception                 
         PJN / 19-07-2000 1. Now works on NT / Windows 2000 thanks to DriverLINX Port I/O Driver
                          2. Made all typedefs and enums local to the appropiate class rather
                          than polluting the global namespace


Copyright (c) 1999 - 2000 by PJ Naughter.  
All rights reserved.

*/

/////////////////////////////////  Includes  //////////////////////////////////
#include "stdafx.h"
#include "ParallelPort.h"
#include <winerror.h>
#include <conio.h>
#include <afxpriv.h>




///////////////////////////////// Defines / Statics ///////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CParallelPort::sm_nRefCount = 0;
BOOL CParallelPort::sm_bRunningOnNT = FALSE;
HINSTANCE CParallelPort::sm_hDLINX = NULL;
CParallelPort::LPDLPORTREADPORTUCHAR CParallelPort::sm_lpfnDlPortReadUchar = NULL;
CParallelPort::LPDLPORTWRITEPORTUCHAR CParallelPort::sm_lpfnDlPortWriteUchar = NULL;
CArray<CParallelPortSettings, CParallelPortSettings&> CParallelPort::sm_Ports;




//////////////////////////////// Implementation ///////////////////////////////


////////// Exception handling code

void AfxThrowParallelException(DWORD dwError /* = 0 */)
{
	if (dwError == 0)
		dwError = ::GetLastError();

	CParallelException* pException = new CParallelException(dwError);

	TRACE(_T("Warning: throwing CParallelException for error %d\n"), dwError);
	THROW(pException);
}

BOOL CParallelException::GetErrorMessage(LPTSTR pstrError, UINT nMaxError, PUINT pnHelpContext)
{
	ASSERT(pstrError != NULL && AfxIsValidString(pstrError, nMaxError));

	if (pnHelpContext != NULL)
		*pnHelpContext = 0;

	LPTSTR lpBuffer;
	BOOL bRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			                      NULL,  m_dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			                      (LPTSTR) &lpBuffer, 0, NULL);

	if (bRet == FALSE)
		*pstrError = '\0';
	else
	{
		lstrcpyn(pstrError, lpBuffer, nMaxError);
		bRet = TRUE;

		LocalFree(lpBuffer);
	}

	return bRet;
}

CString CParallelException::GetErrorMessage()
{
  CString rVal;
  LPTSTR pstrError = rVal.GetBuffer(4096);
  GetErrorMessage(pstrError, 4096, NULL);
  rVal.ReleaseBuffer();
  return rVal;
}

CParallelException::CParallelException(DWORD dwError)
{
	m_dwError = dwError;
}

CParallelException::~CParallelException()
{
}

IMPLEMENT_DYNAMIC(CParallelException, CException)

#ifdef _DEBUG
void CParallelException::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "m_dwError = " << m_dwError;
}
#endif



////////// Settings class

CParallelPortSettings::CParallelPortSettings()
{
  m_nBaseAddress = 0;
  m_Type         = ParallelTypeUndefined;
  m_ECPMode      = ECPModeUndefined;
}

CParallelPortSettings::CParallelPortSettings(const CParallelPortSettings& state)
{
  *this = state;
}

CParallelPortSettings& CParallelPortSettings::operator=(const CParallelPortSettings& state)
{
  m_nBaseAddress = state.m_nBaseAddress;
  m_Type         = state.m_Type;   
  m_ECPMode      = state.m_ECPMode;

  return *this;
}



////////// The actual parallel port class

CParallelPort::CParallelPort()
{
  m_hPort = INVALID_HANDLE_VALUE;
  m_nPortIndex = -1;
  m_nBaseAddress = 0;
  m_dwTimeout = 1000;  //Default timeout is 1 second

  //Test for presence of parallel ports at the standard addresses of 0x3BC, 0x378 and 0x278
  if (sm_nRefCount == 0)
  {
    ++sm_nRefCount;

    //Initialize the DriverLINX driver if on NT / Windows 2000
    sm_bRunningOnNT = RunningOnNT();
    if (sm_bRunningOnNT)
    {
      if (!InitializeDriverLINX())
      {
        TRACE(_T("Running on Windows NT / Windows 2000 and the DriverLINX PORTIO driver is not installed, aborting !!\n"));
        AfxThrowParallelException();
      }
    }

    //Open LPT1 - LPT3 as a precaution against other processes trying to write 
    //to the port while we are detecting them
    HANDLE hPort1 = CreateFile(_T("\\\\.\\LPT1"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    HANDLE hPort2 = CreateFile(_T("\\\\.\\LPT2"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    HANDLE hPort3 = CreateFile(_T("\\\\.\\LPT3"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    //Must have at least one port available in Win32 to continue to attempt
    //detection of the ports
    int nWin32Ports = 0;
    if (hPort1 != INVALID_HANDLE_VALUE)
      ++nWin32Ports;
    if (hPort2 != INVALID_HANDLE_VALUE)
      ++nWin32Ports;
    if (hPort3 != INVALID_HANDLE_VALUE)
      ++nWin32Ports;
    if (nWin32Ports == 0)
    {
      TRACE(_T("No parallel ports are available to Win32, aborting !!\n"));
      AfxThrowParallelException(ERROR_DEV_NOT_EXIST);
    }

    //Try to detect the details of the 3 standard ports
    CParallelPortSettings settings;
    if (GetPort(0x3BC, settings))
      sm_Ports.Add(settings);

    if (sm_Ports.GetSize() < nWin32Ports) 
      if (GetPort(0x378, settings))
        sm_Ports.Add(settings);

    if (sm_Ports.GetSize() < nWin32Ports) 
      if (GetPort(0x278, settings))
        sm_Ports.Add(settings);

    if (sm_Ports.GetSize() == 0)
      TRACE(_T("Could not detect any parallel ports on this machine\n"));

    //Don't forget to close the 3 SDK handles we had open
    CloseHandle(hPort3);
    CloseHandle(hPort2);
    CloseHandle(hPort1);
  }
}

CParallelPort::~CParallelPort()
{
  //decrement the reference count and 
  //free the DriverLINX pointers if necessary
  --sm_nRefCount;
  if (sm_nRefCount == 0)
    DeInitializeDriverLINX();

  Close();
}

IMPLEMENT_DYNAMIC(CParallelPort, CObject)

#ifdef _DEBUG
void CParallelPort::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << _T("m_hPort = ") << m_hPort << _T("\n");
}
#endif

void CParallelPort::Open(int nPort)
{
  //Call Close just in case we already have the port open
  Close();

  m_nPortIndex = nPort - 1;
  if (m_nPortIndex < sm_Ports.GetSize())
  {
    //Cache the base address of the port for performance reasons
    m_nBaseAddress = sm_Ports.ElementAt(m_nPortIndex).m_nBaseAddress;

    //Call CreateFile to open up the parallel port. This prevents other apps 
    //causing problems when we do the Port IO directly.
    CString sPort;
    sPort.Format(_T("\\\\.\\LPT%d"), nPort);
    m_hPort = CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (m_hPort == INVALID_HANDLE_VALUE)
    {
      TRACE(_T("Failed to open parallel port, LPT%d\n"), nPort);
      Close();
      AfxThrowParallelException();
    }
  }
  else
  {
    TRACE(_T("Could not find the parallel port, LPT%d\n"), nPort);
    Close();
    AfxThrowParallelException(ERROR_FILE_NOT_FOUND);
  }
}

BOOL CParallelPort::IsOpen() const
{
  return (m_hPort != INVALID_HANDLE_VALUE);
}

void CParallelPort::Close()
{
  if (IsOpen())
  {
    BOOL bSuccess = CloseHandle(m_hPort);
    m_hPort = INVALID_HANDLE_VALUE;
    m_nPortIndex = -1;
    m_nBaseAddress = 0;
    if (!bSuccess)
      TRACE(_T("Failed to close the parallel port, GetLastError:%d\n"), GetLastError());
  }
}

void CParallelPort::SetECPMode(CParallelPortSettings::ECPPortMode mode)
{
  ASSERT(IsOpen()); //Port must be open 
  CParallelPortSettings& settings = sm_Ports.ElementAt(m_nPortIndex);
  ASSERT(settings.m_Type == CParallelPortSettings::ParallelTypeECP); //Must be an ECP port
  ASSERT(mode != CParallelPortSettings::ECPModeUndefined);

  unsigned short nEcrAddress = (unsigned short)(m_nBaseAddress + 0x402);

  //Read the ECR & clear bits 5, 6 & 7
  int nEcrData = _inp(nEcrAddress) & 0x1F;
  
  //Write the selected value to bits 5, 6 & 7
  switch (mode)
  {
    case CParallelPortSettings::ECPModeSPP:            nEcrData |= (0 << 5); break;
    case CParallelPortSettings::ECPModePS2:            nEcrData |= (1 << 5); break;
    case CParallelPortSettings::ECPModeFastCentronics: nEcrData |= (2 << 5); break;
    case CParallelPortSettings::ECPModeECP:            nEcrData |= (3 << 5); break;
    case CParallelPortSettings::ECPModeEPP:            nEcrData |= (4 << 5); break;
    case CParallelPortSettings::ECPModeTest:           nEcrData |= (6 << 5); break;
    case CParallelPortSettings::ECPModeConfiguration:  nEcrData |= (7 << 5); break;
    default: ASSERT(FALSE);                           break;
  }
  _outp(nEcrAddress, nEcrData);

  //Update the value in our cached array
  settings.m_ECPMode = mode;
}

CParallelPortSettings::ECPPortMode CParallelPort::GetECPMode()
{
  ASSERT(IsOpen()); //Port must be open 
  CParallelPortSettings& settings = sm_Ports.ElementAt(m_nPortIndex);
  ASSERT(settings.m_Type == CParallelPortSettings::ParallelTypeECP); //Must be an ECP port

  CParallelPortSettings::ECPPortMode t = ReadECPMode(settings.m_nBaseAddress);
  ASSERT(t == settings.m_ECPMode);

  return settings.m_ECPMode;
}

CParallelPortSettings::ECPPortMode CParallelPort::ReadECPMode(unsigned short nBaseAddress)
{
  CParallelPortSettings::ECPPortMode mode = CParallelPortSettings::ECPModeUndefined;
  int nEcrData = _inp((unsigned short)(nBaseAddress+0x402));
  nEcrData = (nEcrData & 0xE0) >> 5;
  switch (nEcrData)
  { 
    case 0: mode = CParallelPortSettings::ECPModeSPP;            break;
    case 1: mode = CParallelPortSettings::ECPModePS2;            break;
    case 2: mode = CParallelPortSettings::ECPModeFastCentronics; break;
    case 3: mode = CParallelPortSettings::ECPModeECP;            break;
    case 4: mode = CParallelPortSettings::ECPModeEPP;            break;
    case 6: mode = CParallelPortSettings::ECPModeTest;           break;
    case 7: mode = CParallelPortSettings::ECPModeConfiguration;  break;
    default: break;
  }
  return mode;
}

BOOL CParallelPort::GetPort(unsigned short nBaseAddress, CParallelPortSettings& settings)
{
  BOOL bSuccess = FALSE;

  //First try to detect an ECP port
  if (GetECPPort(nBaseAddress))
  {
    settings.m_nBaseAddress = nBaseAddress;
    settings.m_Type = CParallelPortSettings::ParallelTypeECP;
    settings.m_ECPMode = ReadECPMode(nBaseAddress);
    bSuccess = TRUE;
  }
  else
  {
    //If its not an ECP, look for an EPP.
    //If the baseaddress is 3BCh, skip the EPP test.
    //EPPs aren't allowed at 3BCh due to possible conflicts
    //with video memory
    BOOL bFoundEPP = FALSE;

    if (nBaseAddress != 0x3BC)
    {
      bFoundEPP = GetEPPPort(nBaseAddress);
      if (bFoundEPP)
      {
        settings.m_nBaseAddress = nBaseAddress;
        settings.m_Type = CParallelPortSettings::ParallelTypeEPP;
        settings.m_ECPMode = CParallelPortSettings::ECPModeUndefined;
        bSuccess = TRUE;
      }
    }

    if (!bFoundEPP)
    {
      //If its not an EPP, look for an SPP.
      if (GetSPPPort(nBaseAddress))
      {
        //Test for a PS/2 port only if the SPP exists.
        //because if the port doesn't exist, it will 
        //pass the PS/2 test!
        if (GetPS2Port(nBaseAddress))
          settings.m_Type = CParallelPortSettings::ParallelTypePS2;
        else
          settings.m_Type = CParallelPortSettings::ParallelTypeSPP;

        settings.m_nBaseAddress = nBaseAddress;
        settings.m_ECPMode = CParallelPortSettings::ECPModeUndefined;
        bSuccess = TRUE;
      }
    }
  }

  if (!bSuccess)
  {
    settings.m_nBaseAddress = 0;
    settings.m_Type = CParallelPortSettings::ParallelTypeUndefined;
    settings.m_ECPMode = CParallelPortSettings::ECPModeUndefined;
  }  

  return bSuccess;
}

BOOL CParallelPort::GetECPPort(unsigned short nBaseAddress)
{
  //If the ECP is idle and the FIFO empty,
  //in the ECP's Ecp (at base address+402h),
  //bit 1 (Fifo full)=0, and bit 0 (Fifo empty)=1.
  //The first test is to see if these bits differ from the
  //corresponding bits in the control port (at base address+2).
  //If so a further test is to write 34h to the Ecr, 
  //then read it back. Bit 1 is read/write and bit 0 is read-only.
  //If the value read is 35h, the port is an ECP.
  BOOL bSuccess = FALSE;
 
  unsigned short nEcrAddress = (unsigned short)(nBaseAddress+0x402);
  int nEcrData = _inp(nEcrAddress);
  
  //Read bits 0 and 1 and control port bit 1
  int nEcrBit0 = nEcrData & 0x1;
  int nEcrBit1 = (nEcrData & 0x2) >> 1;
  int nControlBit1 = (ReadControl(nBaseAddress) & 0x2) >> 1;

  if (nEcrBit0 == 1 && nEcrBit1 == 0)
  {
    //Compare control bit 1 to ECR bit 1
    //Toggle the control bit if necessary
    //to be sure the two registers are different.
    if (nControlBit1 == 0)
    {
      WriteControl(nBaseAddress, 0xF);
      nControlBit1 = (ReadControl(nBaseAddress) & 0x2) >> 1;
    }

    if (nEcrBit1 != nControlBit1)
    {
      int nOriginalEcrData = nEcrData;
      _outp(nEcrAddress, 0x34);
      if (_inp(nEcrAddress) == 0x35)
        bSuccess = TRUE;

      //Restore the ECR to its original value
      _outp(nEcrAddress, nOriginalEcrData);
    }
  }

  return bSuccess;
}

BOOL CParallelPort::GetEPPPort(unsigned short nBaseAddress)
{
  //Write to an EPP register, then read it back.
  //If the read matches the write, it's probably an EPP
  BOOL bSuccess = FALSE;

  //Use nEppAddressPort for testing, SPP's, ECP's and PS/2 ports
  //do not have this register
  unsigned short nEppAddressPort = (unsigned short) (nBaseAddress+3);
  _outp(nEppAddressPort, 0x55);

  //Clear the timeout bit after each EPP operation
  int nTimeoutBit = GetEPPTimeoutBit(nBaseAddress);
  int nRead = _inp(nEppAddressPort);
  nTimeoutBit = GetEPPTimeoutBit(nBaseAddress);
  if (nRead == 0x55)
  {
    _outp(nEppAddressPort, 0xAA);
    nTimeoutBit = GetEPPTimeoutBit(nBaseAddress);
    nRead = _inp(nEppAddressPort);
    nTimeoutBit = GetEPPTimeoutBit(nBaseAddress);
    if (nRead == 0xAA)
      bSuccess = TRUE;
  }  

  return bSuccess;
}

int CParallelPort::GetEPPTimeoutBit(unsigned short nBaseAddress)
{
  //Reads and clears the EPP timeout bit (Statis port bit 0)
  //Should be done after each EPP operation.
  //The method for clearing the bit varies, so try 3 ways
  //1. Write 1 to status port bit 0
  //2. Write 0 to status port bit 1
  //3. Read the status port again
  int nReturn = (ReadStatus(nBaseAddress) & 0x1);
  unsigned short nStatusPortAddress = (unsigned short)(nBaseAddress+1);
  _outp(nStatusPortAddress, 1);
  _outp(nStatusPortAddress, 0);
  nReturn = (ReadStatus(nBaseAddress) & 0x1);
  return nReturn;
}

BOOL CParallelPort::GetPS2Port(unsigned short nBaseAddress)
{
  //First try to tristate (disable) the data outputs by
  //setting bit 5 of the control port. Then write 2 values
  //to the data port and read each one back. If the values
  //match, the data outputs are not disabled and the port
  //is not bidirectional. If the values don't match, the data 
  //outputs are disabled and the port is didirectional
  BOOL bSuccess = FALSE;

  //Set control port bit 5
  WriteControl(nBaseAddress, 0x2F);

  //Write the first byte and read it back
  WriteData(nBaseAddress, 0x55);

  //If it doesn't match, the port is bidirectional
  if (ReadData(nBaseAddress) == 0x55)
  {      \
    WriteData(nBaseAddress, 0xAA);

    //If it doesn't match, the port is bidirectional
    if (ReadData(nBaseAddress) != 0xAA)
      bSuccess = TRUE;
  }
  else
    bSuccess = TRUE;

  //Reset control port bit 5
  WriteControl(nBaseAddress, 0xF);

  return bSuccess;
}

BOOL CParallelPort::GetSPPPort(unsigned short nBaseAddress)
{
  //Write two bytes and read them back. If the
  //reads matches the writes, the port exists
  BOOL bSuccess = FALSE;
  
  //Be sure that control port bit 5 = 0 (Data outputs enabled)
  WriteControl(nBaseAddress, 0xF);

  //Perform the first write  
  WriteData(nBaseAddress, 0x55);
  if (ReadData(nBaseAddress) == 0x55)
  {
    WriteData(nBaseAddress, 0xAA);
    bSuccess = (ReadData(nBaseAddress) == 0xAA);
  }

  return bSuccess;
}

void CParallelPort::WriteControl(unsigned short nBaseAddress, int nData)
{
  //The control port is at offset nBaseAddress + 2.
  //Bits 0, 1 & 3 need to be inverted  
  _outp((unsigned short)(nBaseAddress+2), nData ^ 0xB);
}

int CParallelPort::ReadControl(unsigned short nBaseAddress)
{
  //The control port is at offset nBaseAddress + 2.
  //Bits 0, 1 & 3 need to be inverted  
  return (_inp((unsigned short)(nBaseAddress+2)) ^ 0xB);
}

void CParallelPort::WriteData(unsigned short nBaseAddress, int nData)
{
  //The data port is at offset nBaseAddress.
  _outp(nBaseAddress, nData);
}

int CParallelPort::ReadData(unsigned short nBaseAddress)
{
  //The data port is at offset nBaseAddress.
  return _inp(nBaseAddress);
}

int CParallelPort::ReadStatus(unsigned short nBaseAddress)
{
  //The status port is at offset nBaseAddress + 1.
  //Bit 7 need to be inverted  
  return (_inp((unsigned short)(nBaseAddress+1)) ^ 0x80);
}

void CParallelPort::WriteControl(int nData)
{
  ASSERT(IsOpen()); //Port must be open
  WriteControl(m_nBaseAddress, nData);  
}

int CParallelPort::ReadControl()
{
  ASSERT(IsOpen()); //Port must be open
  return ReadControl(m_nBaseAddress);  
}

void CParallelPort::WriteData(int nData)
{
  ASSERT(IsOpen()); //Port must be open
  WriteData(m_nBaseAddress, nData);  
}

int CParallelPort::ReadData()
{
  ASSERT(IsOpen()); //Port must be open
  return ReadData(m_nBaseAddress);  
}

int CParallelPort::ReadStatus()
{
  ASSERT(IsOpen()); //Port must be open
  return ReadStatus(m_nBaseAddress);  
}

BOOL CParallelPort::ReadByteUsingNibbleMode(BYTE& byData)
{
  ASSERT(IsOpen()); //Port must be open

  //Read a byte of data at the status port, in 2 nibbles

  //When S6 = 0, set D3 to 0
  int S6;
  DWORD dwStartTicks = GetTickCount();
  do
  {
    S6 = (ReadStatus(m_nBaseAddress) & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > m_dwTimeout)
      return FALSE;
  }
  while (S6 == 1);
  WriteData(m_nBaseAddress, 0);

  //When the peripheral responds by setting S6=1, Set D3=1
  //nLowNibble then holds 4 bits of data
  int nLowNibble;
  do
  {
    nLowNibble = ReadStatus(m_nBaseAddress);
    S6 = (nLowNibble & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > m_dwTimeout)
      return FALSE;
  }
  while (S6 == 0);
  WriteData(m_nBaseAddress, 8);

  //When S6 = 0, set D3 to 0
  do
  {
    S6 = (ReadStatus(m_nBaseAddress) & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > m_dwTimeout)
      return FALSE;
  }
  while (S6 == 1);
  WriteData(m_nBaseAddress, 0);

  //When the peripheral responds by setting S6=1, Set D3=1
  //nHighNibble then holds 4 bits of data
  int nHighNibble;
  do
  {
    nHighNibble = ReadStatus(m_nBaseAddress);
    S6 = (nHighNibble & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > m_dwTimeout)
      return FALSE;
  }
  while (S6 == 0);
  WriteData(m_nBaseAddress, 8);

  //Recombine the two nibbles back into the byte
  byData = (BYTE) (((nLowNibble & 0x8) >> 3) +
                   ((nLowNibble & 0x10) >> 3) +
                   ((nLowNibble & 0x20) >> 3) +
                   ((nLowNibble & 0x80) >> 4) +
                   ((nHighNibble & 0x8) << 1) +
                   ((nHighNibble & 0x10) << 1) +
                   ((nHighNibble & 0x20) << 1) +
                   ((nHighNibble & 0x80))); 

  return TRUE;
}

BOOL CParallelPort::WriteByteUsingNibbleMode(BYTE byData)
{
  ASSERT(IsOpen()); //Port must be open

  //Write a byte to the data port, in 2 nibbles.
  //The remote system reads the data at its status port.
  //The data bits are D0, D1, D2 and D4.
  //D3 is the strobe
  int nLowNibble = byData & 0x07;
  nLowNibble |= (byData & 0x08) << 1;
  int nHighNibble = (byData & 0x70) >> 4;
  nHighNibble |= (byData & 0x80) >> 3;

  //When S6=1 (not busy), write the low nibble and set D3=0.
  int S6;
  DWORD dwStartTicks = GetTickCount();  
  do
  {
    S6 = (ReadStatus(m_nBaseAddress) & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > m_dwTimeout)
      return FALSE;
  }
  while (S6 == 0);
  WriteData(m_nBaseAddress, nLowNibble);

  //When the peripheral responds by setting S6=0, Set D3=1.
  do
  {
    S6 = (ReadStatus(m_nBaseAddress) & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > m_dwTimeout)
      return FALSE;
  }
  while (S6 == 1);
  WriteData(m_nBaseAddress, nLowNibble | 0x8);

  //When S6=1, write the high nibble and set D3=0
  do
  {
    S6 = (ReadStatus(m_nBaseAddress) & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > m_dwTimeout)
      return FALSE;
  }
  while (S6 == 0);
  WriteData(m_nBaseAddress, nHighNibble);

  //When the peripheral responds by setting S6=0, Set D3=1.
  do
  {
    S6 = (ReadStatus(m_nBaseAddress) & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > m_dwTimeout)
      return FALSE;
  }
  while (S6 == 1);
  WriteData(m_nBaseAddress, nHighNibble | 0x8);

  return TRUE;
}

BOOL CParallelPort::ReadUsingNibbleMode(void* lpBuf, DWORD dwBytes)
{
  //Validate the parameters
  ASSERT(lpBuf);

  //Read each byte into lpBuf  
  BYTE* lpByteBuf = (BYTE*) lpBuf;
  for (DWORD i=0; i<dwBytes; i++)
  {
    if (!ReadByteUsingNibbleMode(lpByteBuf[i]))
      return FALSE;
  }
  return TRUE;
}

BOOL CParallelPort::WriteUsingNibbleMode(const void* lpBuf, DWORD dwBytes)
{
  //Validate the parameters
  ASSERT(lpBuf);

  //Write each byte from lpBuf  
  const BYTE* lpByteBuf = (const BYTE*) lpBuf;
  for (DWORD i=0; i<dwBytes; i++)
  {
    if (!WriteByteUsingNibbleMode(lpByteBuf[i]))
      return FALSE;
  }
  return TRUE;
}

int CParallelPort::_inp(unsigned short port)
{
  if (sm_bRunningOnNT)
  {
    ASSERT(sm_lpfnDlPortReadUchar);
    return sm_lpfnDlPortReadUchar(port);
  }
  else
    return ::_inp(port);
}

int CParallelPort::_outp(unsigned short port, int databyte)
{
  if (sm_bRunningOnNT)
  {
    ASSERT(sm_lpfnDlPortWriteUchar);
    sm_lpfnDlPortWriteUchar(port, (UCHAR) databyte);
    return databyte;
  }
  else
    return ::_outp(port, databyte);
}

BOOL CParallelPort::InitializeDriverLINX()
{
  //Load up the DriverLINX dll and get the function pointers we are interested in
  ASSERT(sm_hDLINX == NULL);
  BOOL bSuccess = FALSE;
  sm_hDLINX = LoadLibrary(_T("DLPORTIO.DLL"));
  if (sm_hDLINX)
  {
    //Get the function pointers
    sm_lpfnDlPortReadUchar = (LPDLPORTREADPORTUCHAR) GetProcAddress(sm_hDLINX, "DlPortReadPortUchar");
    sm_lpfnDlPortWriteUchar = (LPDLPORTWRITEPORTUCHAR) GetProcAddress(sm_hDLINX, "DlPortWritePortUchar");

    //If any of the functions are not installed then fail the load
    if (sm_lpfnDlPortReadUchar == NULL ||
        sm_lpfnDlPortWriteUchar == NULL)
    {
      TRACE(_T("Failed to get one of the function pointer`s in the DriverLINX dll\n"));
      DeInitializeDriverLINX();
    }
    else
      bSuccess = TRUE;
  }
  else
    TRACE(_T("Could not find the load the DriverLINX dll, please ensure DriverLINX driver is installed\n"));

  return bSuccess;
}

void CParallelPort::DeInitializeDriverLINX()
{
  if (sm_hDLINX)
  {
    //Unload the dll and reset the function pointers to NULL    
    FreeLibrary(sm_hDLINX);
    sm_hDLINX = NULL;
    sm_lpfnDlPortReadUchar = NULL;
    sm_lpfnDlPortWriteUchar = NULL;
  }
}

BOOL CParallelPort::PortPresent(int nPort)
{
  BOOL bSuccess = FALSE;

  //Try to open the port 
  CString sPort;
  sPort.Format(_T("\\\\.\\LPT%d"), nPort);
  HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
  if (hPort != INVALID_HANDLE_VALUE)
  {
    bSuccess = TRUE;

    //Close the port, now that we don't need it anymore
    ::CloseHandle(hPort);
  }
  else
  {
    //Determine if the port exists based on the error code.
    DWORD dwError = GetLastError();

    //Check to see if the error was because some other app had the port open or a general failure
    if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE)
      bSuccess = TRUE;
  }

  return bSuccess;
}

BOOL CParallelPort::RunningOnNT()
{
  OSVERSIONINFO osvi;
  memset(&osvi, 0, sizeof(OSVERSIONINFO));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  VERIFY(GetVersionEx(&osvi));
  return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
}

CParallelPortSettings CParallelPort::GetSettings() const
{
  ASSERT(IsOpen()); //Port must be open 
  return sm_Ports.GetAt(m_nPortIndex);
}



////////// The file transfer class

CParallelPortFileTransfer::CParallelPortFileTransfer(CParallelPort* pPort)
{
  m_pPort = NULL;
  ASSERT(pPort != NULL);
  ASSERT(pPort->IsOpen());

  //Ensure the Port is set to SPP mode if it is an ECP port
  CParallelPortSettings settings = pPort->GetSettings();
  if (settings.m_Type == CParallelPortSettings::ParallelTypeECP)
    pPort->SetECPMode(CParallelPortSettings::ECPModeSPP);

  //Initialize D3 (strobe) to 1
  pPort->WriteData(0x8);

  //Wait for the opposite end to set D3=1 (not busy)
  int S6;
  DWORD dwStartTicks = GetTickCount();
  do
  {
    S6 = (pPort->ReadStatus() & 0x40) >> 6;

    //Check the timeout has not elapsed
    if ((GetTickCount() - dwStartTicks) > pPort->GetTimeout())
    {
      TRACE(_T("Could not setup file transfer, other end busy\n"));
      AfxThrowParallelException(ERROR_TIMEOUT);
    }
  }
  while (S6 == 0);

  //Store away the port pointer for use in other class methods
  m_pPort = pPort;
}

CParallelPortFileTransfer::~CParallelPortFileTransfer()
{
  m_pPort = NULL;
}

void CParallelPortFileTransfer::SendFile(const CString& sLocalFile)
{
  ASSERT(m_pPort); //Initialize failed

	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

  //First try to open the file to send
  CFile file;
  if (file.Open(sLocalFile, CFile::modeRead | CFile::shareDenyWrite))
  {
    //first send the filename (excluding the path) (size if _MAX_PATH)
    TCHAR pszPath[_MAX_PATH];
    TCHAR pszFname[_MAX_FNAME];
    TCHAR pszExt[_MAX_EXT];
    _tsplitpath(sLocalFile, NULL, NULL, pszFname, pszExt);
    _tmakepath(pszPath, NULL, NULL, pszFname, pszExt);

    //Now need to convert the filename to ASCII so that a UNICODE
    //client can take to an ASCII server
    char pszAsciiPath[_MAX_PATH];
    char* pszTemp = T2A(pszPath);
    ZeroMemory(pszAsciiPath, _MAX_PATH);
    strcpy(pszAsciiPath, pszTemp);

    //Do the actual send of the filename
    if (!m_pPort->WriteUsingNibbleMode(pszAsciiPath, _MAX_PATH))
    {
      TRACE(_T("Failed to send the filename\n"));
      AfxThrowParallelException(ERROR_TIMEOUT);
    }

    try
    {
      //Then send the size of the file (as 4 bytes).
      DWORD dwLength = file.GetLength();
      if (!m_pPort->WriteUsingNibbleMode(&dwLength, sizeof(DWORD)))
      {
        TRACE(_T("Failed to send the file length\n"));
        AfxThrowParallelException(ERROR_TIMEOUT);
      }

      //Then the actual file contents ifself (4k at a time)
      BYTE byData[4096];
      int nBytesRead;
      do
      {
        nBytesRead = file.Read(byData, 4096);
        if (!m_pPort->WriteUsingNibbleMode(byData, nBytesRead))
        {
          TRACE(_T("Failed to send the file contents\n"));
          AfxThrowParallelException(ERROR_TIMEOUT);
        }
      }
      while (nBytesRead);
    }
    catch(CFileException* pEx)
    {
      TRACE(_T("An error occurred reading from the file to send\n"));
      delete pEx;
      AfxThrowParallelException();
    }
  }
  else
  {
    TRACE(_T("Could not open the file to send, %s\n"), sLocalFile);
    AfxThrowParallelException();
  }
}

void CParallelPortFileTransfer::ReceiveFile(const CString& sLocalFile)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

  //First try to open the file to send
  CFile file;
  if (file.Open(sLocalFile, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
  {
    //Receive the filename
    char pszAsciiPath[_MAX_PATH];
    if (!m_pPort->ReadUsingNibbleMode(pszAsciiPath, _MAX_PATH))
    {
      TRACE(_T("Failed to receive the filename\n"));

      //Delete the local file
      file.Close();
      DeleteFile(sLocalFile);

      //Throw the exception
      AfxThrowParallelException(ERROR_TIMEOUT);
    }

    try
    {
      //Receive the size of the file (as 4 bytes).
      DWORD dwLength;
      if (!m_pPort->ReadUsingNibbleMode(&dwLength, sizeof(DWORD)))
      {
        TRACE(_T("Failed to receive the file length\n"));
  
        //Delete the local file
        file.Close();
        DeleteFile(sLocalFile);

        //Throw the exception
        AfxThrowParallelException(ERROR_TIMEOUT);
      }

      //Then the actual file contents ifself (4k at a time)
      BYTE byData[4096];
      DWORD dwBytesLeft = dwLength;
      do
      {
        DWORD dwBytesToRead;
        if (dwBytesLeft > 4096)
          dwBytesToRead = 4096;
        else
          dwBytesToRead = dwBytesLeft;
          
        if (!m_pPort->ReadUsingNibbleMode(byData, dwBytesToRead))
        {
          TRACE(_T("Failed to receive the file contents\n"));

          //Delete the local file
          file.Close();
          DeleteFile(sLocalFile);

          //Throw the exception
          AfxThrowParallelException(ERROR_TIMEOUT);
        }

        file.Write(byData, dwBytesToRead);
        dwBytesLeft -= dwBytesToRead;
      }
      while (dwBytesLeft);
    }
    catch(CFileException* pEx)
    {
      TRACE(_T("An error occurred writing the file which was being received\n"));
      delete pEx;
      AfxThrowParallelException();
    }
  }
  else
  {
    TRACE(_T("Could not open the file to receive into, %s\n"), sLocalFile);
    AfxThrowParallelException();
  }
}

