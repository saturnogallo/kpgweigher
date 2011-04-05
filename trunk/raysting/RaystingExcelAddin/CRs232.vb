Imports System.Runtime.InteropServices
Imports System.Text
Imports System.Threading
Imports System.ComponentModel
Imports System.IO



#Region "RS232"
Public Class Rs232 : Implements IDisposable
   '===================================================
   '
   '	Module				:	Rs232
   '	Description		:	Class for handling RS232 comunication with VB.Net
   '	Created				:	10/08/2001 - 8:45:25
   '	Author				:	Corrado Cavalli (corrado@mvps.org)
    'WebSite				:	www.codeworks.it/net/index.htm
   '
   '	Notes					:	
   '-----------------------------------------------------------------------------------------------
	'																	*	Revisions *
	'
	'	02/12/2000		First internal alpha version built on framework beta1
	'
   '	1st Public release Beta2 (10/08/2001)
   '
   '	Rev.1	(28.02.2002)
   '	1.	Added ResetDev, SetBreak and ClearBreak to the EscapeCommFunction constants
   '	2.	Added the overloaded Open routine.
   '	3.	Added the modem status routines, properties and enum.
   '	4.	If a read times out, it now returns a EndOfStreamException (instead of a simple Exception).
   '	5.Compiled with VS.Net final

   '	Rev.2	(01.03.2002)
   '	Added Async support
   '
   '  Rev.3	(07.04.2002)
   '	Minor bugs fixed
   '
   '  Rev.3 (05/05/2002)
   '  Fixed BuildCommmDCB problem
   '
   '  Rev.4 (24/05/2002)
   '  Fixed problem with ASCII Encoding truncating 8th bit
   '
   '  Rev.5 (27/05/2002)
   '  Added IDisposable / Finalize implementation
   '
   '  Rev.6 (14/03/2003)
   '  Fixed problem on DCB fields Initialization
   '
   '  Rev.7 (26/03/2003)
   '  Added XON/XOFF support
   '
   '  Rev.8 (12/07/2003)
	'  Added support to COM port number greater than 4
	'
	'	Rev.9 (15/07/2003)
	'	Added CommEvent to detect incoming chars/events
	'	Updated both Tx/Rx method from Non-Ovelapped to Overlapped mode
	'	Removed unused Async methods and other stuff.
	'
	'	Rev.10 (21/07/2003)
    '    Fixed incorrect character handling when using EnableEvents()
   '
    '   Rev.11 (12/08/2003)
    '   Fixed some bugs signaled by users
   ' 
	'	Rev.12 (01/09/2003)
	'	Removed AutoReset of internal buffers and added PurgeBuffer() method
	'	
	'	Rev.13 (02/09/2003)
	'	Removed GetLastErrorUse in favour of Win32Exception()
	'
	'	Rev.14 (14/09/2003)
	'	Added IsPortAvailable() function
	'  Revised some API declaration
    '  Fixed problem with Win98/Me OS
   '
    '   Rev.15 (24/09/2003)
    '  	Fixed bug introduced on Rev.14
   '
    '   Rev.16 (12/10/2003)
    '	Added SetBreak/ClearBreak() methods
   '
    '   Rev.17 (02/11/2003)
    '   Fixed field on COMMCONFIG
	'
	'   Rev.18 (03/03/2004)
	'   Fixed bug: Testing mhRS for <>0 is not correct
	'
	'   Rev.19 (08/04/2004)
	'   Fixed bug: Fixed bug on DTR property
	'
	'   Rev.20 (12/07/2004)
	'   CommEvent is no more raised on a secondary thread
    '	pEventsWatcher now uses a background thread
	'
	'   Rev.21 (24/10/2004)
	'   EscapeCommFunction declaration fixed
	'	Pariti enum fixed to Parity
	'
	'	Rev. 22 (05/03/2005)
    '	Fixed memory leak problem causing program closing
    '	without any message on some systems.
    '	Thanks to Ralf Gedrat for testing this scenario
    '
    '   Rev.23 (05/04/2005)
    '   Fixed bug DisableEvents not working bug 
    '
    '   Rev.24 (20/04/2005) 
    '   Fixed memory leak on Read method
    '   Added InBufferCount property
    '   IsPortAvailable method is now shared
    '   Thanks to Jean-Pierre ZANIER for the feedback

    '===================================================  
   '// Class Members		
	Private mhRS As IntPtr = New IntPtr(0)	  '// Handle to Com Port									
   Private miPort As Integer = 1   '//  Default is COM1	
   Private miTimeout As Int32 = 70   '// Timeout in ms
   Private miBaudRate As Int32 = 9600
   Private meParity As DataParity = 0
   Private meStopBit As DataStopBit = 0
   Private miDataBit As Int32 = 8
   Private miBufferSize As Int32 = 512   '// Buffers size default to 512 bytes
   Private mabtRxBuf As Byte()   '//  Receive buffer	
   Private meMode As Mode  '//  Class working mode	
	Private moThreadTx As Thread
	Private moThreadRx As Thread
	Private moEvents As Thread
   Private miTmpBytes2Read As Int32
   Private meMask As EventMasks
   Private mbDisposed As Boolean
	Private mbUseXonXoff As Boolean
	Private mbEnableEvents As Boolean
	Private miBufThreshold As Int32 = 1
	Private muOvlE As OVERLAPPED
	Private muOvlW As OVERLAPPED
	Private muOvlR As OVERLAPPED
	Private mHE As GCHandle
	Private mHR As GCHandle
	Private mHW As GCHandle
	'----------------------------------------------------------------------------------------

#Region "Enums"
	'// Parity Data
	Public Enum DataParity
		Parity_None = 0
        Parity_Odd
		Parity_Even
		Parity_Mark
	End Enum
	'// StopBit Data
	Public Enum DataStopBit
		StopBit_1 = 1
		StopBit_2
	End Enum
	<Flags()> Public Enum PurgeBuffers
		RXAbort = &H2
		RXClear = &H8
		TxAbort = &H1
		TxClear = &H4
	End Enum
	Private Enum Lines
		SetRts = 3
		ClearRts = 4
		SetDtr = 5
		ClearDtr = 6
		ResetDev = 7		 '	// Reset device if possible
		SetBreak = 8		 '	// Set the device break line.
		ClearBreak = 9		 '	// Clear the device break line.
	End Enum
	'// Modem Status
	<Flags()> Public Enum ModemStatusBits
		ClearToSendOn = &H10
		DataSetReadyOn = &H20
		RingIndicatorOn = &H40
		CarrierDetect = &H80
	End Enum
	'// Working mode
	Public Enum Mode
		NonOverlapped
		Overlapped
	End Enum
	'// Comm Masks
	<Flags()> Public Enum EventMasks
		RxChar = &H1
		RXFlag = &H2
		TxBufferEmpty = &H4
		ClearToSend = &H8
		DataSetReady = &H10
		CarrierDetect = &H20
		Break = &H40
		StatusError = &H80
		Ring = &H100
	End Enum

#End Region
#Region "Structures"
	<StructLayout(LayoutKind.Sequential, Pack:=1)> Private Structure DCB
		Public DCBlength As Int32
		Public BaudRate As Int32
		Public Bits1 As Int32
		Public wReserved As Int16
		Public XonLim As Int16
		Public XoffLim As Int16
		Public ByteSize As Byte
		Public Parity As Byte
		Public StopBits As Byte
		Public XonChar As Char
		Public XoffChar As Char
		Public ErrorChar As Char
		Public EofChar As Char
		Public EvtChar As Char
		Public wReserved2 As Int16
	End Structure
	<StructLayout(LayoutKind.Sequential, Pack:=1)> Private Structure COMMTIMEOUTS
		Public ReadIntervalTimeout As Int32
		Public ReadTotalTimeoutMultiplier As Int32
		Public ReadTotalTimeoutConstant As Int32
		Public WriteTotalTimeoutMultiplier As Int32
		Public WriteTotalTimeoutConstant As Int32
	End Structure
    <StructLayout(LayoutKind.Sequential, Pack:=8)> Private Structure COMMCONFIG
        Public dwSize As Int32
        Public wVersion As Int16
        Public wReserved As Int16
        Public dcbx As DCB
        Public dwProviderSubType As Int32
        Public dwProviderOffset As Int32
        Public dwProviderSize As Int32
        Public wcProviderData As Int16
    End Structure
    <StructLayout(LayoutKind.Sequential, Pack:=1)> Public Structure OVERLAPPED
        Public Internal As Int32
        Public InternalHigh As Int32
        Public Offset As Int32
        Public OffsetHigh As Int32
        Public hEvent As IntPtr
    End Structure
    <StructLayout(LayoutKind.Sequential, Pack:=1)> Private Structure COMSTAT
        Dim fBitFields As Int32
        Dim cbInQue As Int32
        Dim cbOutQue As Int32
    End Structure

#End Region
#Region "Constants"
	Private Const PURGE_RXABORT As Integer = &H2
	Private Const PURGE_RXCLEAR As Integer = &H8
	Private Const PURGE_TXABORT As Integer = &H1
	Private Const PURGE_TXCLEAR As Integer = &H4
	Private Const GENERIC_READ As Integer = &H80000000
	Private Const GENERIC_WRITE As Integer = &H40000000
	Private Const OPEN_EXISTING As Integer = 3
	Private Const INVALID_HANDLE_VALUE As Integer = -1
	Private Const IO_BUFFER_SIZE As Integer = 1024
	Private Const FILE_FLAG_OVERLAPPED As Int32 = &H40000000
	Private Const ERROR_IO_PENDING As Int32 = 997
	Private Const WAIT_OBJECT_0 As Int32 = 0
	Private Const ERROR_IO_INCOMPLETE As Int32 = 996
	Private Const WAIT_TIMEOUT As Int32 = &H102&
	Private Const INFINITE As Int32 = &HFFFFFFFF


#End Region
#Region "Win32API"
	'// Win32 API
   <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function SetCommState(ByVal hCommDev As IntPtr, ByRef lpDCB As DCB) As Int32
   End Function
   <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function GetCommState(ByVal hCommDev As IntPtr, ByRef lpDCB As DCB) As Int32
   End Function
   <DllImport("kernel32.dll", SetlastError:=True, CharSet:=CharSet.Auto)> Private Shared Function BuildCommDCB(ByVal lpDef As String, ByRef lpDCB As DCB) As Int32
   End Function
   <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function SetupComm(ByVal hFile As IntPtr, ByVal dwInQueue As Int32, ByVal dwOutQueue As Int32) As Int32
   End Function
   <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function SetCommTimeouts(ByVal hFile As IntPtr, ByRef lpCommTimeouts As COMMTIMEOUTS) As Int32
   End Function
   <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function GetCommTimeouts(ByVal hFile As IntPtr, ByRef lpCommTimeouts As COMMTIMEOUTS) As Int32
   End Function
   <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function ClearCommError(ByVal hFile As IntPtr, ByRef lpErrors As Int32, ByRef lpComStat As COMSTAT) As Int32
   End Function
   <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function PurgeComm(ByVal hFile As IntPtr, ByVal dwFlags As Int32) As Int32
   End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function EscapeCommFunction(ByVal hFile As IntPtr, ByVal ifunc As Int32) As Boolean
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function WaitCommEvent(ByVal hFile As IntPtr, ByRef Mask As EventMasks, ByRef lpOverlap As OVERLAPPED) As Int32
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function WriteFile(ByVal hFile As IntPtr, ByVal Buffer As Byte(), ByVal nNumberOfBytesToWrite As Integer, ByRef lpNumberOfBytesWritten As Integer, ByRef lpOverlapped As OVERLAPPED) As Integer
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function ReadFile(ByVal hFile As IntPtr, <Out()> ByVal Buffer As Byte(), ByVal nNumberOfBytesToRead As Integer, ByRef lpNumberOfBytesRead As Integer, ByRef lpOverlapped As OVERLAPPED) As Integer
    End Function
    <DllImport("kernel32.dll", SetlastError:=True, CharSet:=CharSet.Auto)> Private Shared Function CreateFile(ByVal lpFileName As String, ByVal dwDesiredAccess As Integer, ByVal dwShareMode As Integer, ByVal lpSecurityAttributes As Integer, ByVal dwCreationDisposition As Integer, ByVal dwFlagsAndAttributes As Integer, ByVal hTemplateFile As Integer) As IntPtr
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function CloseHandle(ByVal hObject As IntPtr) As Boolean
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Public Shared Function GetCommModemStatus(ByVal hFile As IntPtr, ByRef lpModemStatus As Int32) As Boolean
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function SetEvent(ByVal hEvent As IntPtr) As Boolean
    End Function
    <DllImport("kernel32.dll", SetlastError:=True, CharSet:=CharSet.Auto)> Private Shared Function CreateEvent(ByVal lpEventAttributes As IntPtr, ByVal bManualReset As Int32, ByVal bInitialState As Int32, ByVal lpName As String) As IntPtr
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function WaitForSingleObject(ByVal hHandle As IntPtr, ByVal dwMilliseconds As Int32) As Int32
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function GetOverlappedResult(ByVal hFile As IntPtr, ByRef lpOverlapped As OVERLAPPED, ByRef lpNumberOfBytesTransferred As Int32, ByVal bWait As Int32) As Int32
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function SetCommMask(ByVal hFile As IntPtr, ByVal lpEvtMask As Int32) As Int32
    End Function
    <DllImport("kernel32.dll", SetlastError:=True, CharSet:=CharSet.Auto)> Private Shared Function GetDefaultCommConfig(ByVal lpszName As String, ByRef lpCC As COMMCONFIG, ByRef lpdwSize As Integer) As Boolean
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function SetCommBreak(ByVal hFile As IntPtr) As Boolean
    End Function
    <DllImport("kernel32.dll", SetlastError:=True)> Private Shared Function ClearCommBreak(ByVal hFile As IntPtr) As Boolean
    End Function


#End Region
#Region "Events"
	Public Event CommEvent As CommEventHandler
#End Region
#Region "Delegates"
	Public Delegate Sub CommEventHandler(ByVal source As Rs232, ByVal Mask As EventMasks)
#End Region

	Public Property Port() As Integer
		'===================================================
		'
		'		Description	:		Comunication Port
		'		Created			:		21/09/2001 - 11:25:49
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Get
			Return miPort
		End Get
		Set(ByVal Value As Integer)
			miPort = Value
		End Set
	End Property
	Public Sub PurgeBuffer(ByVal Mode As PurgeBuffers)
		'===================================================
		'												©2003 ALSTOM FIR S.p.A All rights reserved
		'
		'	Description	:	Purge Communication Buffer
		'	Created			:	01/09/03 - 10:37:39
		'	Author			:	Corrado Cavalli
		'
		'						*Parameters Info*
		'
		'	Notes				:	This method will clear any character into buffer, use TxAbort/RxAbort
		'								to terminate any pending overlapped Tx/Rx operation.
		'===================================================
		If (mhRS.ToInt32 > 0) Then PurgeComm(mhRS, Mode)
	End Sub
	Public Overridable Property Timeout() As Integer
		'===================================================
		'
		'		Description:		Comunication timeout in seconds
		'		Created			:		21/09/2001 - 11:26:50
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Get
			Return miTimeout
		End Get
		Set(ByVal Value As Integer)
			miTimeout = CInt(IIf(Value = 0, 500, Value))
			'// If Port is open updates it on the fly
			pSetTimeout()
		End Set
	End Property
	Public Property Parity() As DataParity
		'===================================================
		'
		'		Description	:		Comunication parity
		'		Created			:		21/09/2001 - 11:27:15
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Get
			Return meParity
		End Get
		Set(ByVal Value As DataParity)
			meParity = Value
		End Set
	End Property
	Public Property StopBit() As DataStopBit
		'===================================================
		'
		'		Description:		Comunication StopBit
		'		Created			:		21/09/2001 - 11:27:37
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Get
			Return meStopBit
		End Get
		Set(ByVal Value As DataStopBit)
			meStopBit = Value
		End Set
	End Property
	Public Property BaudRate() As Integer
		'===================================================
		'
		'		Description:		Comunication BaudRate
		'		Created			:		21/09/2001 - 11:28:00
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Get
			Return miBaudRate
		End Get
		Set(ByVal Value As Integer)
			miBaudRate = Value
		End Set
	End Property
	Public Property DataBit() As Integer
		'===================================================
		'
		'		Description	:		Comunication DataBit
		'		Created			:		21/09/2001 - 11:28:20
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Get
			Return miDataBit
		End Get
		Set(ByVal Value As Integer)
			miDataBit = Value
		End Set
	End Property
	Public Property BufferSize() As Integer
		'===================================================
		'
		'		Description	:		Receive Buffer size		
		'		Created			:		21/09/2001 - 11:33:05
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Get
			Return miBufferSize
		End Get
		Set(ByVal Value As Integer)
			miBufferSize = Value
		End Set
	End Property
	Public Overloads Sub Open()
		'===================================================
		'
		'		Description	:		Initializes and Opens comunication port
		'		Created			:		21/09/2001 - 11:33:40
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		'// Get Dcb block,Update with current data
		Dim uDcb As DCB, iRc As Int32
		'// Set working mode
		meMode = Mode.Overlapped
		Dim iMode As Int32 = Convert.ToInt32(IIf(meMode = Mode.Overlapped, FILE_FLAG_OVERLAPPED, 0))
		'// Initializes Com Port
		If miPort > 0 Then
			Try
				'// Creates a COM Port stream handle 
				mhRS = CreateFile("\\.\COM" & miPort.ToString, GENERIC_READ Or GENERIC_WRITE, 0, 0, OPEN_EXISTING, iMode, 0)
				If (mhRS.ToInt32 > 0) Then
					'// Clear all comunication errors
					Dim lpErrCode As Int32
					iRc = ClearCommError(mhRS, lpErrCode, New COMSTAT)
					'// Clears I/O buffers
					iRc = PurgeComm(mhRS, PurgeBuffers.RXClear Or PurgeBuffers.TxClear)
					'// Gets COM Settings
					iRc = GetCommState(mhRS, uDcb)
					'// Updates COM Settings
					Dim sParity As String = "NOEM"
					sParity = sParity.Substring(meParity, 1)
					'// Set DCB State
					Dim sDCBState As String = String.Format("baud={0} parity={1} data={2} stop={3}", miBaudRate, sParity, miDataBit, CInt(meStopBit))
					iRc = BuildCommDCB(sDCBState, uDcb)
					uDcb.Parity = CByte(meParity)
					'// Set Xon/Xoff State
					If mbUseXonXoff Then
						uDcb.Bits1 = 768
					Else
						uDcb.Bits1 = 0
					End If
					iRc = SetCommState(mhRS, uDcb)
					If iRc = 0 Then
						Dim sErrTxt As String = New Win32Exception().Message
						Throw New CIOChannelException("Unable to set COM state " & sErrTxt)
					End If
					'// Setup Buffers (Rx,Tx)
					iRc = SetupComm(mhRS, miBufferSize, miBufferSize)
					'// Set Timeouts
					pSetTimeout()
					'//Enables events if required
					If mbEnableEvents Then Me.EnableEvents()
				Else
					'// Raise Initialization problems
					Dim sErrTxt As String = New Win32Exception().Message
					Throw New CIOChannelException("Unable to open COM" + miPort.ToString + ControlChars.CrLf + sErrTxt)
				End If
			Catch Ex As Exception
				'// Generica error
				Throw New CIOChannelException(Ex.Message, Ex)
			End Try
		Else
			'// Port not defined, cannot open
			Throw New ApplicationException("COM Port not defined,use Port property to set it before invoking InitPort")
		End If
	End Sub
	Public Overloads Sub Open(ByVal Port As Integer, ByVal BaudRate As Integer, ByVal DataBit As Integer, ByVal Parity As DataParity, ByVal StopBit As DataStopBit, ByVal BufferSize As Integer)
		'===================================================
		'
		'		Description:		Opens comunication port (Overloaded method)
		'		Created		:		21/09/2001 - 11:33:40
		'
		'												*Parameters Info*
		'
		'		Notes			:
		'===================================================
		Me.Port = Port
		Me.BaudRate = BaudRate
		Me.DataBit = DataBit
		Me.Parity = Parity
		Me.StopBit = StopBit
		Me.BufferSize = BufferSize
		Open()
	End Sub
	Public Sub Close()
		'===================================================
		'
		'		Description:		Close comunication channel
		'		Created			:		21/09/2001 - 11:38:00
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		If mhRS.ToInt32 > 0 Then
			If mbEnableEvents = True Then
				Me.DisableEvents()
			End If
			Dim ret As Boolean = CloseHandle(mhRS)
			If Not ret Then Throw New Win32Exception
			mhRS = New IntPtr(0)
		End If
	End Sub
	ReadOnly Property IsOpen() As Boolean
		'===================================================
		'
		'		Description:		Returns Port Status		
		'		Created			:		21/09/2001 - 11:38:51
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Get
			Return CBool(mhRS.ToInt32 > 0)
		End Get
	End Property
	Public Overloads Sub Write(ByVal Buffer As Byte())
		'===================================================
		'
		'		Description:		Transmit a stream
		'		Created			:		21/09/2001 - 11:39:51
		'
		'												*Parameters Info*
		'		Buffer				:   Array of Byte() to write
		'		Notes				:
		'===================================================
		Dim iRc, iBytesWritten As Integer, hOvl As GCHandle
		'-----------------------------------------------------------------
		muOvlW = New Overlapped
		If mhRS.ToInt32 <= 0 Then
			Throw New ApplicationException("Please initialize and open port before using this method")
		Else
			'// Creates Event
			Try
                hOvl = GCHandle.Alloc(muOvlW, GCHandleType.Pinned)
				muOvlW.hEvent = CreateEvent(Nothing, 1, 0, Nothing)
				If muOvlW.hEvent.ToInt32 = 0 Then Throw New ApplicationException("Error creating event for overlapped writing")
				'// Clears IO buffers and sends data
				iRc = WriteFile(mhRS, Buffer, Buffer.Length, 0, muOvlW)
				If iRc = 0 Then
					If Marshal.GetLastWin32Error <> ERROR_IO_PENDING Then
						Throw New ApplicationException("Write command error")
					Else
						'// Check Tx results
						If GetOverlappedResult(mhRS, muOvlW, iBytesWritten, 1) = 0 Then
							Throw New ApplicationException("Write pending error")
						Else
							'// All bytes sent?
							If iBytesWritten <> Buffer.Length Then Throw New ApplicationException("Write Error - Bytes Written " & iBytesWritten.ToString & " of " & Buffer.Length.ToString)
						End If
					End If
				End If
			Finally
				'//Closes handle
				CloseHandle(muOvlW.hEvent)
                If (hOvl.IsAllocated = True) Then hOvl.Free()
			End Try
		End If
	End Sub
	Public Overloads Sub Write(ByVal Buffer As String)
		'===================================================
		'
		'	Description	:	Writes a string to RS232
		'	Created			:	04/02/2002 - 8:46:42
		'
		'						*Parameters Info*
		'
		'	Notes				:  24/05/2002 Fixed problem with ASCII Encoding
		'===================================================
		Dim oEncoder As New System.Text.ASCIIEncoding
		Dim oEnc As Encoding = oEncoder.GetEncoding(1252)
		'-------------------------------------------------------------
		Dim aByte() As Byte = oEnc.GetBytes(Buffer)
		Me.Write(aByte)
	End Sub
	Public Function Read(ByVal Bytes2Read As Integer) As Integer
		'===================================================
		'
		'		Description:		Read Bytes from Port
		'		Created			:		21/09/2001 - 11:41:17
		'
		'												*Parameters Info*
		'		Bytes2Read		:		Bytes to read from port
		'		Returns					:		Number of readed chars
		'
		'		Notes				:
		'===================================================
		Dim iReadChars, iRc As Integer, bReading As Boolean, hOvl As GCHandle
		'--------------------------------------------------------------
		'// If Bytes2Read not specified uses Buffersize
		If Bytes2Read = 0 Then Bytes2Read = miBufferSize
		muOvlR = New Overlapped
		If mhRS.ToInt32 <= 0 Then
			Throw New ApplicationException("Please initialize and open port before using this method")
		Else
			'// Get bytes from port
			Try
                hOvl = GCHandle.Alloc(muOvlR, GCHandleType.Pinned)
				muOvlR.hEvent = CreateEvent(Nothing, 1, 0, Nothing)
				If muOvlR.hEvent.ToInt32 = 0 Then Throw New ApplicationException("Error creating event for overlapped reading")
				'// Clears IO buffers and reads data
				ReDim mabtRxBuf(Bytes2Read - 1)
				iRc = ReadFile(mhRS, mabtRxBuf, Bytes2Read, iReadChars, muOvlR)
				If iRc = 0 Then
					If Marshal.GetLastWin32Error() <> ERROR_IO_PENDING Then
						Throw New ApplicationException("Read pending error")
					Else
						'// Wait for characters
						iRc = WaitForSingleObject(muOvlR.hEvent, miTimeout)
						Select Case iRc
							Case WAIT_OBJECT_0
								'// Some data received...
								If GetOverlappedResult(mhRS, muOvlR, iReadChars, 0) = 0 Then
									Throw New ApplicationException("Read pending error.")
								Else
									Return iReadChars
								End If
							Case WAIT_TIMEOUT
								Throw New IOTimeoutException("Read Timeout.")
							Case Else
								Throw New ApplicationException("General read error.")
						End Select
					End If
				Else
					Return (iReadChars)
				End If
			Finally
				'//Closes handle
				CloseHandle(muOvlR.hEvent)
				If (hOvl.IsAllocated) Then hOvl.Free()
			End Try
		End If
	End Function
	Overridable ReadOnly Property InputStream() As Byte()
		'===================================================
		'
		'		Description:		Returns received data	as Byte()	
		'		Created			:		21/09/2001 - 11:45:06
		'
		'												*Parameters Info*
		'		
		'		Notes				:
		'===================================================
      Get
         Return mabtRxBuf
      End Get
   End Property
	Overridable ReadOnly Property InputStreamString() As String
		'===================================================
		'
		'	Description	:	Return a string containing received data
		'	Created			:	04/02/2002 - 8:49:55
		'
		'						*Parameters Info*
		'
		'	Notes				:
		'===================================================
		Get
         Dim oEncoder As New System.Text.ASCIIEncoding
         Dim oEnc As Encoding = oEncoder.GetEncoding(1252)
			'-------------------------------------------------------------
			If Not Me.InputStream Is Nothing Then Return oEnc.GetString(Me.InputStream)
		End Get
	End Property
	Public Sub ClearInputBuffer()
		'===================================================
		'
		'		Description:		Clears Input buffer
		'		Created			:		21/09/2001 - 11:45:34
		'
		'												*Parameters Info*
		'
		'		Notes				:  Gets all character until end of buffer
		'===================================================
		If mhRS.ToInt32 > 0 Then
			PurgeComm(mhRS, PURGE_RXCLEAR)
		End If
	End Sub
	Public WriteOnly Property Rts() As Boolean
		'===================================================
		'
		'		Description:		Set/Resets RTS Line		
		'		Created			:		21/09/2001 - 11:45:34
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Set(ByVal Value As Boolean)
			If mhRS.ToInt32 > 0 Then
				If Value Then
					EscapeCommFunction(mhRS, Lines.SetRts)
				Else
					EscapeCommFunction(mhRS, Lines.ClearRts)
				End If
			End If
		End Set
	End Property
	Public WriteOnly Property Dtr() As Boolean
		'===================================================
		'
		'		Description:		Set/Resets DTR Line		
		'		Created			:		21/09/2001 - 11:45:34
		'
		'												*Parameters Info*
		'
		'		Notes				:
		'===================================================
		Set(ByVal Value As Boolean)
			If mhRS.ToInt32 > 0 Then
				If Value Then
					EscapeCommFunction(mhRS, Lines.SetDtr)
				Else
					EscapeCommFunction(mhRS, Lines.ClearDtr)
				End If
			End If
		End Set
	End Property
	Public ReadOnly Property ModemStatus() As ModemStatusBits
		'===================================================
		'
		'	Description	:	Gets Modem status
		'	Created			:	28/02/2002 - 8:58:04
		'
		'						*Parameters Info*
		'
		'	Notes				:
		'===================================================
		Get
			If mhRS.ToInt32 <= 0 Then
				Throw New ApplicationException("Please initialize and open port before using this method")
			Else
				'// Retrieve modem status
				Dim lpModemStatus As Int32
				If Not GetCommModemStatus(mhRS, lpModemStatus) Then
					Throw New ApplicationException("Unable to get modem status")
				Else
					Return CType(lpModemStatus, ModemStatusBits)
				End If
			End If
		End Get
	End Property
	Public Function CheckLineStatus(ByVal Line As ModemStatusBits) As Boolean
		'===================================================
		'
		'	Description	:	Check status of a Modem Line
		'	Created			:	28/02/2002 - 10:25:17
		'
		'						*Parameters Info*
		'
		'	Notes				:
		'===================================================
		Return Convert.ToBoolean(ModemStatus And Line)
	End Function
	Public Property UseXonXoff() As Boolean
		'===================================================
		'
		'	Description	:	Set XON/XOFF mode 
		'	Created			:	26/05/2003 - 21:16:18
		'
		'						*Parameters Info*
		'
		'	Notes				:
		'===================================================
		Get
			Return mbUseXonXoff
		End Get
		Set(ByVal Value As Boolean)
			mbUseXonXoff = Value
		End Set
	End Property
	Public Sub EnableEvents()
		'===================================================
		'
		'	Description	:	Enables monitoring of incoming events
		'	Created			:	15/07/2003 - 12:00:56
		'
		'						*Parameters Info*
		'
		'	Notes				:
		'===================================================
		If mhRS.ToInt32 <= 0 Then
			Throw New ApplicationException("Please initialize and open port before using this method")
		Else
			If moEvents Is Nothing Then
				mbEnableEvents = True
				moEvents = New Thread(AddressOf pEventsWatcher)
				moEvents.IsBackground = True
				moEvents.Start()
			End If
		End If
	End Sub
	Public Sub DisableEvents()
		'===================================================
		'
		'	Description	:	Disables monitoring of incoming events
		'	Created			:	15/07/2003 - 12:00:56
		'
		'						*Parameters Info*
		'
		'	Notes				:
		'===================================================
		If mbEnableEvents = True Then
			SyncLock Me
				mbEnableEvents = False				 '// This should kill the thread
			End SyncLock
			'// Let WaitCommEvent exit...
			If muOvlE.hEvent.ToInt32 <> 0 Then SetEvent(muOvlE.hEvent)
			moEvents = Nothing
		End If
	End Sub
	Public Property RxBufferThreshold() As Int32
		'===================================================
		'												©2003 www.codeworks.it All rights reserved
		'
		'	Description	:	Numer of characters into input buffer
		'	Created			:	16/07/03 - 9:00:57
		'	Author			:	Corrado Cavalli
		'
		'						*Parameters Info*
		'
		'	Notes				:
		'===================================================
		Get
			Return miBufThreshold
		End Get
		Set(ByVal Value As Int32)
			miBufThreshold = Value
		End Set
	End Property
    Public Shared Function IsPortAvailable(ByVal portNumber As Int32) As Boolean
        '===================================================
        '												©2003 www.codeworks.it All rights reserved
        '
        '	Description	:	Returns true if a specific port number is supported by the system
        '	Created			:	14/09/03 - 17:00:57
        '	Author			:	Corrado Cavalli
        '
        '						*Parameters Info*
        '	portNumber	:	port number to check
        '
        '	Notes				:
        '===================================================
        If portNumber <= 0 Then
            Return False
        Else
            Dim cfg As COMMCONFIG
            Dim cfgsize As Int32 = Marshal.SizeOf(cfg)
            cfg.dwSize = cfgsize
            Dim ret As Boolean = GetDefaultCommConfig("COM" + portNumber.ToString, cfg, cfgsize)
            Return ret
        End If
    End Function
    Public Sub SetBreak()
        '===================================================
        '												©2003 www.codeworks.it All rights reserved
        '
        '	Description	:	Set COM in break modem
        '	Created			:	12/10/03 - 10:00:57
        '	Author			:	Corrado Cavalli
        '
        '						*Parameters Info*
        '	
        '
        '	Notes				:
        '===================================================
        If mhRS.ToInt32 > 0 Then
            If SetCommBreak(mhRS) = False Then Throw New Win32Exception
        End If
    End Sub
    Public Sub ClearBreak()
        '===================================================
        '												©2003 www.codeworks.it All rights reserved
        '
        '	Description	:	Clear COM break mode
        '	Created			:	12/10/03 - 10:02:57
        '	Author			:	Corrado Cavalli
        '
        '						*Parameters Info*
        '	
        '
        '	Notes				:
        '===================================================
        If mhRS.ToInt32 > 0 Then
            If ClearCommBreak(mhRS) = False Then Throw New Win32Exception
        End If

    End Sub
    Public ReadOnly Property InBufferCount() As Int32
        '===================================================
        '												©2003 www.codeworks.it All rights reserved
        '
        '	Description	:	Returns the number of bytes inside Rx buffer
        '	Created			:	20/04/05 - 10:02:57
        '	Author			:	Corrado Cavalli/Jean-Pierre ZANIER
        '
        '
        '===================================================
        Get
            Dim comStat As COMSTAT
            Dim lpErrCode As Int32
            Dim iRc As Int32
            comStat.cbInQue = 0
            If mhRS.ToInt32 > 0 Then
                iRc = ClearCommError(mhRS, lpErrCode, comStat)
                Return comStat.cbInQue
            End If
            Return 0
        End Get
    End Property


#Region "Finalize"
    Protected Overrides Sub Finalize()
        '===================================================
        '
        '	Description	:	Closes COM port if object is garbage collected and still owns
        '                       COM port reosurces
        '
        '	Created			:	27/05/2002 - 19:05:56
        '
        '						*Parameters Info*
        '
        '	Notes				:
        '===================================================
        Try
            If Not mbDisposed Then
                If mbEnableEvents Then Me.DisableEvents()
                Close()
            End If
        Finally
            MyBase.Finalize()
        End Try
    End Sub
#End Region

#Region "Private Routines"
    Private Sub pSetTimeout()
        '===================================================
        '
        '		Description:		Set comunication timeouts
        '		Created			:		21/09/2001 - 11:46:40
        '
        '												*Parameters Info*
        '
        '		Notes				:
        '===================================================
        Dim uCtm As COMMTIMEOUTS
        '// Set ComTimeout
        If mhRS.ToInt32 <= 0 Then
            Exit Sub
        Else
            '// Changes setup on the fly
            With uCtm
                .ReadIntervalTimeout = 0
                .ReadTotalTimeoutMultiplier = 0
                .ReadTotalTimeoutConstant = miTimeout
                .WriteTotalTimeoutMultiplier = 10
                .WriteTotalTimeoutConstant = 100
            End With
            SetCommTimeouts(mhRS, uCtm)
        End If
    End Sub
    Private Sub pDispose() Implements IDisposable.Dispose
        '===================================================
        '
        '	Description	:	Handles correct class disposing Write
        '	Created			:	27/05/2002 - 19:03:06
        '
        '						*Parameters Info*
        '
        '	Notes				:
        '===================================================
        If (Not mbDisposed AndAlso (mhRS.ToInt32 > 0)) Then
            '// Closes Com Port releasing resources
            Try
                Me.Close()
            Finally
                mbDisposed = True
                '// Suppress unnecessary Finalize overhead
                GC.SuppressFinalize(Me)
            End Try
        End If


    End Sub
    Private Sub pEventsWatcher()
        '===================================================
        '												©2003 www.codeworks.it All rights reserved
        '
        '	Description	:	Watches for all events raising events when they arrive to the port
        '	Created			:	15/07/03 - 11:45:13
        '	Author			:	Corrado Cavalli
        '
        '						*Parameters Info*
        '
        '	Notes				:
        '===================================================
        '// Events to watch
        Dim lMask As EventMasks = EventMasks.Break Or EventMasks.CarrierDetect Or EventMasks.ClearToSend Or _
        EventMasks.DataSetReady Or EventMasks.Ring Or EventMasks.RxChar Or EventMasks.RXFlag Or _
        EventMasks.StatusError
        Dim lRetMask As EventMasks, iBytesRead, iTotBytes, iErrMask As Int32, iRc As Int32, aBuf As New ArrayList
        Dim uComStat As COMSTAT
        '-----------------------------------
        '// Creates Event
        muOvlE = New Overlapped
        Dim hOvlE As GCHandle = GCHandle.Alloc(muOvlE, GCHandleType.Pinned)
        muOvlE.hEvent = CreateEvent(Nothing, 1, 0, Nothing)
        If muOvlE.hEvent.ToInt32 = 0 Then Throw New ApplicationException("Error creating event for overlapped reading")
        '// Set mask
        SetCommMask(mhRS, lMask)
        '// Looks for RxChar
        While mbEnableEvents = True
            WaitCommEvent(mhRS, lMask, muOvlE)
            Select Case WaitForSingleObject(muOvlE.hEvent, INFINITE)
                Case WAIT_OBJECT_0
                    '// Event (or abort) detected
                    If mbEnableEvents = False Then Exit While
                    If (lMask And EventMasks.RxChar) > 0 Then
                        '// Read incoming data
                        ClearCommError(mhRS, iErrMask, uComStat)
                        If iErrMask = 0 Then
                            Dim ovl As New Overlapped
                            Dim hOvl As GCHandle = GCHandle.Alloc(ovl, GCHandleType.Pinned)
                            ReDim mabtRxBuf(uComStat.cbInQue - 1)
                            If ReadFile(mhRS, mabtRxBuf, uComStat.cbInQue, iBytesRead, ovl) > 0 Then
                                If iBytesRead > 0 Then
                                    '// Some bytes read, fills temporary buffer
                                    If iTotBytes < miBufThreshold Then
                                        aBuf.AddRange(mabtRxBuf)
                                        iTotBytes += iBytesRead
                                    End If
                                    '// Threshold reached?, raises event
                                    If iTotBytes >= miBufThreshold Then
                                        '//Copies temp buffer into Rx buffer
                                        ReDim mabtRxBuf(iTotBytes - 1)
                                        aBuf.CopyTo(mabtRxBuf)
                                        '// Raises event
                                        Try
                                            Me.OnCommEventReceived(Me, lMask)
                                        Finally
                                            iTotBytes = 0
                                            aBuf.Clear()
                                        End Try
                                    End If
                                End If
                            End If
                            If (hOvl.IsAllocated) Then hOvl.Free()
                        End If
                    Else
                        '// Simply raises OnCommEventHandler event
                        Me.OnCommEventReceived(Me, lMask)
                    End If
                Case Else
                    Dim sErr As String = New Win32Exception().Message
                    Throw New ApplicationException(sErr)
            End Select
        End While
        '// Release Event Handle
        CloseHandle(muOvlE.hEvent)
        muOvlE.hEvent = IntPtr.Zero
        If (hOvlE.IsAllocated) Then hOvlE.Free()
        muOvlE = Nothing
    End Sub



#End Region

#Region "Protected Routines"
    Protected Sub OnCommEventReceived(ByVal source As Rs232, ByVal mask As EventMasks)
        '===================================================
        '												©2003 www.codeworks.it All rights reserved
        '
        '	Description	:	Raises CommEvent
        '	Created			:	15/07/03 - 15:09:50
        '	Author			:	Corrado Cavalli
        '
        '						*Parameters Info*
        '
        '	Notes				:
        '===================================================
        Dim del As CommEventHandler = Me.CommEventEvent
        If (Not del Is Nothing) Then
            Dim SafeInvoker As ISynchronizeInvoke
            Try
                SafeInvoker = DirectCast(del.Target, ISynchronizeInvoke)
            Catch
            End Try
            If (Not SafeInvoker Is Nothing) Then
                SafeInvoker.Invoke(del, New Object() {source, mask})
            Else
                del.Invoke(source, mask)
            End If
        End If
    End Sub
#End Region

End Class
#End Region

#Region "Exceptions"
Public Class CIOChannelException : Inherits ApplicationException
	'===================================================
	'
	'		Module			:		CChannellException
	'		Description:		Customized Channell Exception
	'		Created			:		17/10/2001 - 10:32:37
	'
	'		Notes				:		This exception is raised when NACK error found
	'===================================================
	Sub New(ByVal Message As String)
		MyBase.New(Message)
	End Sub
	Sub New(ByVal Message As String, ByVal InnerException As Exception)
		MyBase.New(Message, InnerException)
	End Sub
End Class
Public Class IOTimeoutException : Inherits CIOChannelException
	'===================================================
	'
	'	Description	:	Timeout customized exception
	'	Created			:	28/02/2002 - 10:43:43
	'
	'						*Parameters Info*
	'
	'	Notes				:
	'===================================================
	Sub New(ByVal Message As String)
		MyBase.New(Message)
	End Sub
	Sub New(ByVal Message As String, ByVal InnerException As Exception)
		MyBase.New(Message, InnerException)
	End Sub
End Class

#End Region

