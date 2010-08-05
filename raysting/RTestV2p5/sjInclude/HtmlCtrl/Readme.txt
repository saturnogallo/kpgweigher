Usage:
1: Declare a CHtmlCtrl m_hc in your class ( dialog or anything else)
2: Put(or create) a CStatic on your dialog (or view)
3: call CreateFromStatic in your init code 
   and denote a message code for call back
   and a message handler is necessary
   and right click menu should be prevented (you may use javascript to manage it)
4: Navigate anywhere you like
5: <name,value> pair will be stored in CMapStringToOb when data is submitted
	you can specify some special name and value for your own use such as "command type"
	javascript is prefered
6: a message denoted will be posted when submit button clicked
7: in htmls method = "post" action  should be "ToHtmlCtrl"

for more info ,see the example