# Microsoft Developer Studio Project File - Name="TryData3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TryData3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TryData3.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TryData3.mak" CFG="TryData3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TryData3 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TryData3 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TryData3 - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 nafxcw.lib Libcmt.lib ..\CommonH&Libs\PrgInterface3.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"Libcmt.lib nafxcw.lib" /out:"../../Release/TestBed/TryData3.exe"

!ELSEIF  "$(CFG)" == "TryData3 - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /profile /debug /machine:I386 /out:"../../Release/TestBed/TryData3.exe"

!ENDIF 

# Begin Target

# Name "TryData3 - Win32 Release"
# Name "TryData3 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Diskid32.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\SjInclude\HtmlCtrl\Include\HtmlCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\SjInclude\Report\Include\MSWORD8.CPP
# End Source File
# Begin Source File

SOURCE=.\PrgInterface.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\SjInclude\Report\Include\Replace.cpp
# End Source File
# Begin Source File

SOURCE=..\SjInclude\SimpleAdo\Include\SimpleAdo.cpp
# End Source File
# Begin Source File

SOURCE=.\SjSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Tester.cpp
# End Source File
# Begin Source File

SOURCE=.\TesterCaculate.cpp
# End Source File
# Begin Source File

SOURCE=.\TesterData.cpp
# End Source File
# Begin Source File

SOURCE=.\TesterProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\TryData3.cpp
# End Source File
# Begin Source File

SOURCE=.\TryData3.rc
# End Source File
# Begin Source File

SOURCE=.\TryData3Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TryDataSql.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\HtmlHandler.h
# End Source File
# Begin Source File

SOURCE=.\SjInclude\Report\Include\MSWORD8.H
# End Source File
# Begin Source File

SOURCE=.\PORT32.H
# End Source File
# Begin Source File

SOURCE=.\SjInclude\Report\Include\Replace.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\SjInclude\SimpleAdo\Include\SimpleAdo.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE="..\CommonH&Libs\SwiNav.h"
# End Source File
# Begin Source File

SOURCE=.\Tester.h
# End Source File
# Begin Source File

SOURCE=.\TesterCaculate.h
# End Source File
# Begin Source File

SOURCE=.\TesterData.h
# End Source File
# Begin Source File

SOURCE=.\TesterProgram.h
# End Source File
# Begin Source File

SOURCE=.\TryData3.h
# End Source File
# Begin Source File

SOURCE=.\TryData3Dlg.h
# End Source File
# Begin Source File

SOURCE=.\TryDataSql.h
# End Source File
# Begin Source File

SOURCE=.\WaitDlg.h
# End Source File
# Begin Source File

SOURCE=.\WINIO.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\startup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TryData3.ico
# End Source File
# Begin Source File

SOURCE=.\res\TryData3.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\conf1.js
# End Source File
# Begin Source File

SOURCE=.\res\conf2.js
# End Source File
# Begin Source File

SOURCE=.\conf2js.htm
# End Source File
# Begin Source File

SOURCE=.\res\datafoot.js
# End Source File
# Begin Source File

SOURCE=.\res\datahead.js
# End Source File
# Begin Source File

SOURCE=.\res\jhfoot.js
# End Source File
# Begin Source File

SOURCE=.\res\jhhead.js
# End Source File
# Begin Source File

SOURCE="..\CommonH&Libs\PrgInterface3.dll"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\reg.js
# End Source File
# Begin Source File

SOURCE=.\res\regjs.htm
# End Source File
# Begin Source File

SOURCE=.\res\regjs.js
# End Source File
# Begin Source File

SOURCE=.\res\report.js
# End Source File
# Begin Source File

SOURCE="..\CommonH&Libs\SwiNav.dll"
# End Source File
# Begin Source File

SOURCE=.\res\tbr.js
# End Source File
# Begin Source File

SOURCE=.\res\testfoot.js
# End Source File
# Begin Source File

SOURCE=.\res\testhead.js
# End Source File
# Begin Source File

SOURCE="..\CommonH&Libs\SwiNav.lib"
# End Source File
# Begin Source File

SOURCE="..\CommonH&Libs\PrgInterface3.lib"
# End Source File
# End Target
# End Project
