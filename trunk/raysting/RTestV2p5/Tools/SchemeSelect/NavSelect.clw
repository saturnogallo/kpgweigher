; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNavSelectDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NavSelect.h"

ClassCount=3
Class1=CNavSelectApp
Class2=CNavSelectDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_NAVSELECT_DIALOG

[CLS:CNavSelectApp]
Type=0
HeaderFile=NavSelect.h
ImplementationFile=NavSelect.cpp
Filter=N

[CLS:CNavSelectDlg]
Type=0
HeaderFile=NavSelectDlg.h
ImplementationFile=NavSelectDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CAboutDlg]
Type=0
HeaderFile=NavSelectDlg.h
ImplementationFile=NavSelectDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_NAVSELECT_DIALOG]
Type=1
Class=CNavSelectDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,listbox,1352728835

