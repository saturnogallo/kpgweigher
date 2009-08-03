#include "stdafx.h"
#include "ioex.h"
#include "msxml2.h"
#include "command.h"
#include "action.h"
#include "time.h"
#include "afxtempl.h"
#include "algorithm.h"


extern CString sAppPath;
#define STATE_BEIDLE		0
#define STATE_DONE_OK		0xfd
#define STATE_DONE_FAIL		0xfe
//size of each reg
const u8	node_reg_size[] = {8,8,8,16,16,16,16,16,16,16,16,16,16,16,16,16,8,8,8,8,8,16,8,8,8,8,8,8,8,16,16,16,8,8,8,8,32,32,16,8,8,8,8,8,8,8,8,8,8,8,32,16};
const u8	sys_reg_size[] = {16,16,16,16,16,16,16,16,16,16,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
u8	l2n_map[sizeof(NODE_CONFIG)]; //local address to node address map
u8	n2l_map[sizeof(NODE_CONFIG)]; //node address to local address map
u8	l2s_map[sizeof(SYSTEM_BOARD)];//local address to system address map
u8	s2l_map[sizeof(SYSTEM_BOARD)];//system address to local address map

u8 *glb_address=0;
u8 glb_hit=0;
extern SOCKET sockConn;
//query the sys flag and wait until we got result
u8 get_sysflag(u8 *address,u16 timeout=1024)
{
	u16 count = 0 ;
	glb_address = address;
	glb_hit = 0;
	cm_query(0xff,(u8)(address-(u8*)&Sysboard),sizeof(u8));
	Sleep(TWOFRM_DURATION);
	while(glb_hit == 0){
		if((count++ % 211) == 210){
			cm_query(0xff,(u8)(address-(u8*)&Sysboard),sizeof(u8));		//send the query for every 211*10 ms
			printf("query again\n");
		}
		check_serial();
		if(timeout == count){
			printf("get sysflag timeout\n");
			count = 0;
		}
		if(sockConn == SOCKET_ERROR)	//flash connection is closed
		{
			printf("socket error");
			glb_hit = 1;
			break;
		}
		Sleep(TICK_DURATION);
	}
	return *address;
}
//set the sys flag and no wait the acknowledge of master board comes
bool set_sysflagb(u8 *address, u8 target, u16 timeout=1024)
{
	u8 oldval = *address;
	*address = target;
	cm_write(0xff,(u8)(address-(u8*)&Sysboard),sizeof(u8),address);
	*address = oldval;
	return true;
}
//query and wait until the sysflag is state_idle
void wait_until_idle_sysflagb(u8 *reg, int timeout)
{
	u8 ret = get_sysflag(reg);
	while(1){
		if(ret == STATE_DONE_FAIL){
			set_sysflagb(reg,STATE_BEIDLE);
			Sleep(TICK_DURATION);
		}
		if(ret == STATE_DONE_OK){
			set_sysflagb(reg,STATE_BEIDLE);
			Sleep(TICK_DURATION);
		}
		if(ret == STATE_BEIDLE){
			break;
		}
		if(sockConn == SOCKET_ERROR)	//flash connection is closed
		{
			printf("socket error\n");
			break;
		}
		ret = get_sysflag(reg);
	}
}
//set the flag_report to addr, so that main board will report the node
bool search_node(u8 addr)
{
	u8 *reg_report = (u8*)&Sysboard.flag_report;
	wait_until_idle_sysflagb(reg_report,TWOFRM_DURATION);
	set_sysflagb(reg_report,addr);
	Sleep(TWOFRM_DURATION);
	while(1){
		if(check_sysflag(reg_report,STATE_DONE_OK)){
			set_sysflagb(reg_report,STATE_BEIDLE);
			return true;
		}
		if(check_sysflag(reg_report,STATE_DONE_FAIL)){
			set_sysflagb(reg_report,STATE_BEIDLE);
			return false;
		}
		if(sockConn == SOCKET_ERROR)	//flash connection is closed
		{
			printf("socket error\n");
			break;
		}
		Sleep(50*TWOFRM_DURATION);
		get_sysflag(reg_report);
	}; //wait for done
	return true;
}
//form an xml string based on sys board information
CString getsysxml(){
	CString str(_T("<sys "));
	int j=0; //position in size array
	for(int i=0;i<sizeof(SYSTEM_BOARD);i++)
	{
		CString name = CString(addr2name(0xff,i));
		if(name == _T("N/A"))
			continue;
		if(sys_reg_size[j] == 32)
			name.Format(_T(" %s='%i'"),name,*((u32*)((u8*)&Sysboard+i)));
		if(sys_reg_size[j] == 16)
			name.Format(_T(" %s='%i'"),name,*((u16*)((u8*)&Sysboard+i)));
		if(sys_reg_size[j] == 8)
			name.Format(_T(" %s='%i'"),name,*((u8*)&Sysboard+i));
		str = str+name;
		j++;
	}
	str.Remove('[');	str.Remove(']');
	return str+_T(" />");
}
CString xmlfilter(CString org)
{
	const TCHAR* filter[] = {_T("motor_speed"),_T("cs_Filter_option"),_T("cs_gain_wordrate"),\
		_T("test_mode_reg1"),_T("test_mode_reg2"),_T("reserved1"),_T("reserved2"),\
		_T("magnet_freq"),_T("magnet_amp"),_T("magnet_time"),_T("delay_f"),_T("delay_w"),\
		_T("delay_s"),_T("open_s"),_T("open_w")};
	CString ret(_T("<node "));
	for(int i=0;i<sizeof(filter)/sizeof(char*);i++)
	{
		CString pattern = CString(filter[i]) + _T("='");
		int pos = org.Find(pattern,0);
		if(pos<=0)
			continue;

		ret = ret + org.Mid(pos,1+org.Find(_T("'"),pos+pattern.GetLength())-pos) + _T(" ");
	}
	ret = ret + _T(" />");
	return ret;
}
int getxmlint(CString name,CString xml)
{
	return atoi(getxmlstring(name,xml));
}
CString getxmlstring(CString name,CString xml)
{
	CString pattern = name + _T("='");
	int pos = xml.Find(pattern);
	if(pos<=0)
		return _T("");
	return xml.Mid(pos+name.GetLength()+2,xml.Find(_T("'"),pos+pattern.GetLength())-(pos+name.GetLength()+2));
}
//form an xml string based on node board information
CString getnodexml(u8 addr){
	CString str(_T("<node "));
	CString name;
	int j=0; //position in size array
	for(int i=0;i<sizeof(NODE_CONFIG);i++)
	{
		 name = CString(addr2name(0,i));
		if(name == "N/A")
			continue;
		str = str+_T(" ") + name+_T("=");
		if(node_reg_size[j] == 32){
			name.Format(_T("'%i'"),*((u32*)((u8*)&RS485_Node[addr]+i)));
		}
		if(node_reg_size[j] == 16){
			name.Format(_T("'%i'"),*((u16*)((u8*)&RS485_Node[addr]+i)));
		}
		if(node_reg_size[j] == 8){
			name.Format(_T("'%i'"),*((u8*)&RS485_Node[addr]+i));
		}
		str = str + name;
		j++;
	}
	str.Remove('[');	str.Remove(']');
	return str+" />";
}
//does the group has at least one node?
bool group_hasnode(u8 grp)
{
		for(int j=0;j<MAX_NODE_NUM;j++){
			if((RS485_Node[j].board & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID){
				if((RS485_Node[j].board & BOARD_GROUP_MASK) == grp){
					return true;
				}
			}
		}
		return false;
}
//send the nodeupd_id(xml) command of all the nodes in the group to display
void update_node(u8 grp)
{
		for(int j=0;j<MAX_NODE_NUM;j++){
			if((RS485_Node[j].board & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID){
				if((RS485_Node[j].board & BOARD_GROUP_MASK) == grp){
					CString id;
					id.Format(_T("%i"),RS485_Node[j].addr);
					answer_flash(_T("display:")+id+_T(":") + getnodexml(RS485_Node[j].addr));
				}
			}
		}
}
//check whether 1 sec has passed since last call
bool second_pass()
{
	static time_t t(0);
	time_t now;
	time(&now);
	
	if((now-t) > 1){
		t = now;
		return true;
	}
	return  false;

}
//check whether 6 sec has passed since last call
bool second6_pass()
{
	static time_t t6(0);
	time_t now;
	time(&now);
	if((now-t6) > 6){
		t6 = now;
		return true;
	}
	return  false;
}
//todo
void collect_reading(){
	for(int i=0;i<4;i++){
		u8 *reg_search = (u8*)&Sysboard.flag_search[i];
		u8 *reg_goon = (u8*)&Sysboard.flag_goon[i];	

		if(!group_hasnode(i))	//without node in this group
			continue;

		u8 ret_search;
		ret_search = get_sysflag(reg_search);

		
		if(STATE_DONE_FAIL == ret_search)	//search failed
		{
			set_sysflagb(reg_search,STATE_BEIDLE);
			printf("gather weight failed\n");
			continue;
		}
		if(STATE_DONE_OK != ret_search)	//still searching
		{
			Sleep(60);
			continue;
		}
			
		update_node(i); //update the display of flash
		if(Sysboard.running[i]) {
			while(Calculation5(i) == 0){
				printf("there is 5 box  match\n");
				Sleep(10);
			}
			while(Calculation4(i) == 0){
				printf("there is 4 box  match\n");
				Sleep(10);
			}
			while(Calculation3(i) == 0){
				printf("there is 3 box  match\n");
				Sleep(10);
			}
			while(Calculation2(i) == 0){
				printf("there is 2 box  match\n");
				Sleep(10);
			}
			while(Calculation1(i) == 0){
				printf("there is 1 box  match\n");
				Sleep(10);
			}

			goonall(i);
			set_sysflagb(reg_goon,1,1);	//set goon flag
			while(1){
				u8 ret = get_sysflag(reg_goon);
				if(ret == STATE_DONE_OK || ret == STATE_DONE_FAIL)
					break;
				Sleep(TWOFRM_DURATION);
			}
		}else{
			set_sysflagb(reg_search,STATE_BEIDLE);
		}
		Sleep(TWOFRM_DURATION);
		//wait_until_idle_sysflagb(reg_search);
	}
}
//get node property setting from flash UI then send it to the board here
//this function just set one node property
//str : <node name1='value1|' name2='value2' name3='value3&'>
void setnodeproperty(int id, CString str, CString sfind, u8 regid,u8* dat, u8 size)
{
	sfind = sfind+CString("='");
	int pos = str.Find(sfind);
	if(pos < 0)
		return;
	pos = pos + sfind.GetLength();
	
	str = str.Mid(pos ,str.Find("'",pos+1)-pos);

	bool isor = (str.Find("|") >= 0);
	bool isand =(str.Find("&") >= 0);
	str.Remove('|');
	str.Remove('&');
	unsigned long val = atoi(str);
	if(size == 1){ //byte case
		if(isor)	val = (*dat) | val;
		if(isand)	val = (*dat) & val;
		if((*dat) == val)
			return; //just the same
		u8 nval = (u8)val;
		cm_write(id,regid,size,&nval); //write the data
		return;
	}
	if(size == 2){
		if(isor)	val = (*((u16*)dat)) | val;
		if(isand)	val = (*((u16*)dat)) & val;
		if((*((u16*)dat)) == val)
			return;
		u16 nval = (u16)val;
		cm_write(id,regid,size,(u8*)&nval); //write the data
		return;
	}
	if(size == 4){
		if(isor)	val = (*((u32*)dat)) | val;
		if(isand)	val = (*((u32*)dat)) & val;
		if((*((u32*)dat)) == val)
			return;
		u32 nval = (u32)val;
		cm_write(id,regid,size,(u8*)&nval); //write the data
		return;
	}

}

//get the node or sys setting from flash UI and send them all to node or sys board
void setnodexml(int id, CString str){
	int j;
	if(id == 0xff){ //system board node
			j = 0;
			for(int i=0;i<sizeof(SYSTEM_BOARD);i++)
			{
				CString name = CString(addr2name(id,i));
				if(name == "N/A")
					continue;
				if(sys_reg_size[j] == 32)
					setnodeproperty(id,str,name,i,(u8*)&Sysboard+i,sizeof(u32));
				if(sys_reg_size[j] == 16)
					setnodeproperty(id,str,name,i,(u8*)&Sysboard+i,sizeof(u16));
				if(sys_reg_size[j] == 8)
					setnodeproperty(id,str,name,i,(u8*)&Sysboard+i,sizeof(u8));
				j++;
			}
			return;
	}
	j=0;
	for(int i=0;i<sizeof(NODE_CONFIG);i++)
	{
		CString name = CString(addr2name(0,i));
		if(name == "N/A")
			continue;
		if(node_reg_size[j] == 16)
			setnodeproperty(id,str,name,i,(u8*)&RS485_Node[id]+i,sizeof(u16));
		if(node_reg_size[j] == 32)
			setnodeproperty(id,str,name,i,(u8*)&RS485_Node[id]+i,sizeof(u32));
		if(node_reg_size[j] == 8)
			setnodeproperty(id,str,name,i,(u8*)&RS485_Node[id]+i,sizeof(u8));
		j++;
	}
}
//caculate the average packs in past 60 seconds and send it to flash UI
//with command release_0<...
extern u8 pids[];
static CArray<time_t, time_t> timelist;
extern CStringArray sLog;
extern CString sLogPath;
extern double g_wtotal;
extern int g_wcount;
void report_pack(u8 num){
	
	time_t t;
	time(&t);
	timelist.Add(t);
	t = t - 60;
	while(timelist.GetSize() > 0){
		if(timelist.GetAt(0) < t){
			timelist.RemoveAt(0);
		}else{
			break;
		}
	}
	int sum=0;
	CString pack;
	int group = 0;
	for(int i=0;i<num;i++)
	{
		for(int j=0;j<MAX_NODE_NUM;j++){
			if(RS485_Node[j].addr == pids[i]){
				CString a;
				a.Format(_T("bkset%i='%i' "),RS485_Node[j].addr,RS485_Node[j].Mtrl_Weight_gram);
				pack = pack + a;
				sum += RS485_Node[j].Mtrl_Weight_gram;
				RS485_Node[j].Mtrl_Weight_gram = ~0;
				group = RS485_Node[j].board & BOARD_GROUP_MASK;
			}
		}
	}
	CString ret;
	g_wcount++;
	g_wtotal += sum;
	ret.Format(_T("release:0:<comb group='%i' weight='%i' speed='%i' %s/>\n"),group, sum,timelist.GetSize(),pack);
	//it should be recorded
	if(sLog.GetCount() > 500)
	{
		CStdioFile csf;
		if(csf.Open(sLogPath,CFile::modeWrite|CFile::modeNoTruncate))
		{
			for(int i=0;i<sLog.GetCount();i++)
			{
				csf.WriteString(sLog.GetAt(i));
			}
			csf.Close();
			sLog.RemoveAll();
		}
	}
	sLog.Add(ret);
	answer_flash(ret);
}
void init_vars()
{
	memset(&Sysboard,0,sizeof(SYSTEM_BOARD));
	int i,j,pos;
	for(i=0;i<MAX_NODE_NUM;i++){
		memset(&RS485_Node[i],0,sizeof(NODE_CONFIG));
		RS485_Node[i].addr = i;
		App_Node[i].fail_counter = 0;
		RS485_Node[i].board = (BOARD_TYPE_INVALID|BOARD_GROUP_NONE);
	}
	//generating a map from local to node and node to local
	j=0;
	pos = 0;
	for(i=0;i<sizeof(NODE_CONFIG);i++)
	{
		l2n_map[i] = 0xff;
		n2l_map[i] = 0xff;
	}
	for(i=0;i<sizeof(NODE_CONFIG);i++)
	{
		if(_T("N/A") != CString(addr2name(0,i)))
		{
			l2n_map[i] = j/8; //u8 case
			if(node_reg_size[pos] == 8){
				n2l_map[j/8] = i;
			}
			if(node_reg_size[pos] == 16){
				n2l_map[j/8] = i;
				n2l_map[1+j/8] = i+1;
			}
			if(node_reg_size[pos] == 32){
				n2l_map[j/8] = i;
				n2l_map[1+j/8] = i+1;
				n2l_map[2+j/8] = i+2;
				n2l_map[3+j/8] = i+3;
			}
			j = j + node_reg_size[pos++];
		}
	}
	//init sys reg size array
	j=0;
	pos = 0;
	for(i=0;i<sizeof(SYSTEM_BOARD);i++)
	{
		l2s_map[i] = 0xff;
		s2l_map[i] = 0xff;
	}
	for(i=0;i<sizeof(SYSTEM_BOARD);i++)
	{
		if(_T("N/A") != CString(addr2name(0xff,i)))
		{
			l2s_map[i] = j/8;
			if(sys_reg_size[pos] == 8){
				s2l_map[j/8] = i;
			}
			if(sys_reg_size[pos] == 16){
					s2l_map[j/8] = i;
				s2l_map[1+j/8] = i+1;
			}
			if(sys_reg_size[pos] == 32){
				s2l_map[j/8] = i;
				s2l_map[1+j/8] = i+1;
				s2l_map[2+j/8] = i+2;
				s2l_map[3+j/8] = i+3;
			}

			j = j + sys_reg_size[pos++];
		}
	}
}
//todo
void init_sys_regs()
{
	for(int i=0;i<MAX_VIBR_NUM;i++){
		Sysboard.flag_stop_machine[i] = STATE_DONE_OK;
		Sysboard.flag_goon[i] = STATE_DONE_OK;
		Sysboard.flag_search[i] = STATE_DONE_OK;

		//write_sysreg_b(flag_stop_machine[i]);
		//write_sysreg_b(flag_goon[i]);//=0
		//write_sysreg_b(flag_search[i]);//=0
	}
}
//profile related function
BOOL WriteProfile(CString sect,CString name,CString value){
	return	::WritePrivateProfileString(sect,name,value,PROFILE_NAME);
}
CString ReadProfile(CString sect,CString name){
	char buffer[1000];
	::GetPrivateProfileString(sect,name,_T(""),buffer,1000,PROFILE_NAME);
	return CString(buffer);
}

inline void TESTHR( HRESULT _hr ) 
{ if FAILED(_hr) throw(_hr); }

void dump_com_error(_com_error &e)
{
	printf("Error\n");
	printf("\a\tCode = %08lx\n", e.Error());
	printf("\a\tCode meaning = %s", e.ErrorMessage());
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	printf("\a\tSource = %s\n", (LPCSTR) bstrSource);
	printf("\a\tDescription = %s\n", (LPCSTR) bstrDescription);
}

BOOL XmlAppend(CString record)
{
	bool bret = false;
	try
	{
		MSXML2::IXMLDOMDocumentPtr docPtr;
		MSXML2::IXMLDOMNodePtr DOMNodePtr;
		//init
		TESTHR(CoInitialize(NULL)); 
		TESTHR(docPtr.CreateInstance(_T("Msxml2.DOMDocument.3.0")));

		// Load a document.
		CString docname = HISTORY_NAME;
		_variant_t varXml((const char*)docname);
		_variant_t varOut((bool)TRUE);
		varOut = docPtr->load(varXml);
		if ((bool)varOut == FALSE)
		{// Show error description - IXMLDOMParseError sample.
			MSXML2::IXMLDOMParseErrorPtr errPtr = docPtr->GetparseError();
			_tprintf(_T("Format of xml broken"));
		}
		else
		{
			VARIANT vtTemp;
			MSXML2::IXMLDOMElementPtr pNode;
			vtTemp.vt=VT_I2;
			vtTemp.iVal = 1; //NODE_ELEMENT
			DOMNodePtr = docPtr->createNode(vtTemp, "item", "");
			docPtr->documentElement->appendChild(DOMNodePtr);

			pNode = docPtr->createElement((_bstr_t) "type" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("type",record));
			DOMNodePtr->appendChild(pNode);

			pNode = docPtr->createElement((_bstr_t) "start" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("start",record));
			DOMNodePtr->appendChild(pNode);

			pNode = docPtr->createElement((_bstr_t) "end" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("end",record));
			DOMNodePtr->appendChild(pNode);

			pNode = docPtr->createElement((_bstr_t) "weight" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("weight",record));
			DOMNodePtr->appendChild(pNode);

			pNode = docPtr->createElement((_bstr_t) "number" );
			pNode->Puttext((_bstr_t)( const   char   *)getxmlstring("number",record));
			DOMNodePtr->appendChild(pNode);

			docPtr->save(varXml);
			bret = true;
		}
		
		
	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	catch(...)
	{
		_tprintf(_T("Unknown error!"));
	}
	CoUninitialize();
	return bret;

}

BOOL XmlDelete(CString start)
{
	return false; //failed test
	/*
	bool bret = false;
	try
	{
		MSXML2::IXMLDOMDocumentPtr docPtr;
		MSXML2::IXMLDOMNodePtr DOMNodePtr;
		//init
		TESTHR(CoInitialize(NULL)); 
		TESTHR(docPtr.CreateInstance(_T("Msxml2.DOMDocument.3.0")));

		// Load a document.
		CString docname = HISTORY_NAME;
		_variant_t varXml((const char*)docname);
		_variant_t varOut((bool)TRUE);
		varOut = docPtr->load(varXml);
		if ((bool)varOut == FALSE)
		{// Show error description - IXMLDOMParseError sample.
			MSXML2::IXMLDOMParseErrorPtr errPtr = docPtr->GetparseError();
			_tprintf(_T("Format of xml broken"));
		}
		else
		{
			VARIANT vtTemp;
			MSXML2::IXMLDOMElementPtr pNode;
			vtTemp.vt=VT_I2;
			vtTemp.iVal = 1; //NODE_ELEMENT
//			docPtr->setProperty("SelectionLanguage","XPath");
			CString sfind = _T("//history/item[start>'")+start+_T("']");
			MSXML2::IXMLDOMNodeListPtr pXMLDomNodeList = NULL;
			pXMLDomNodeList = docPtr->selectNodes((const char *)(sfind));
			long size;
			pXMLDomNodeList->get_length(&size);
			if (size > 0)
			{
				for (int i=0;i<size;i++)
				{
					pXMLDomNodeList->get_item(i,&DOMNodePtr);
					docPtr->removeChild(pNode);
				}
				

				docPtr->save(varXml);

			}
			bret = true;
		}


	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	catch(...)
	{
		_tprintf(_T("Unknown error!"));
	}
	CoUninitialize();
	return bret;
	*/
}

BOOL XmlQuery(CString start,CString send)
{
	bool bret = false;
	try
	{
		MSXML2::IXMLDOMDocumentPtr docPtr;
		MSXML2::IXMLDOMNodePtr DOMNodePtr;
		//init
		TESTHR(CoInitialize(NULL)); 
		TESTHR(docPtr.CreateInstance(_T("Msxml2.DOMDocument.3.0")));

		// Load a document.
		CString docname = HISTORY_NAME;
		_variant_t varXml((const char*)docname);
		_variant_t varOut((bool)TRUE);
		varOut = docPtr->load(varXml);
		if ((bool)varOut == FALSE)
		{// Show error description - IXMLDOMParseError sample.
			MSXML2::IXMLDOMParseErrorPtr errPtr = docPtr->GetparseError();
			_tprintf(_T("Format of xml broken"));
		}
		else
		{
			VARIANT vtTemp;
			MSXML2::IXMLDOMElementPtr pNode;
			vtTemp.vt=VT_I2;
			vtTemp.iVal = 1; //NODE_ELEMENT
			//			docPtr->setProperty("SelectionLanguage","XPath");
			CString sfind = _T("//history/item[start>='")+start+_T("' and start<='"+send+"']");
			MSXML2::IXMLDOMNodeListPtr pXMLDomNodeList = NULL;
			pXMLDomNodeList = docPtr->selectNodes((const char *)(sfind));
			long size;
			pXMLDomNodeList->get_length(&size);
			CStdioFile csf;
			if(csf.Open(RESULT_NAME,CFile::modeCreate|CFile::modeWrite))
			{
				csf.WriteString(_T("<?xml version=\"1.0\" encoding=\"utf-8\"?><history>"));
				if (size > 0)
				{

					BSTR xmlstring;
					for (int i=0;i<size;i++)
					{
						pXMLDomNodeList->get_item(i,&DOMNodePtr);
						DOMNodePtr->get_xml(&xmlstring);
						csf.WriteString(CW2CT(xmlstring));
					}

				}
				csf.WriteString("</history>");
				csf.Close();
			}
			bret = true;
		}


	}
	catch (_com_error &e)
	{
		dump_com_error(e);
	}
	catch(...)
	{
		_tprintf(_T("Unknown error!"));
	}
	CoUninitialize();
	return bret;}