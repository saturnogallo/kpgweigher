Usage:
1: add the following line to your stdafx.h
	#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename ("EOF", "adoEOF")

2: add the following line to your YourApp::InitInstance()
	AfxOleInit();

3: now just call simple functions
	AddField
	QueryField
	DeleteField 
(需在sql语句中使用delete!!,ex DELETE [FROM] tablename [WHERE where-expression])
 
4: we use format string and a stringarray to add and queryfield
   see the following example:
	fmt = "field1=%(i,d,s,f,c),field2=%(i,d,s,f,c)..."
	adddata,retdata = "field1=123,field2=abc,field3=T"
5: Note
	so no ',' should appear in your records!
