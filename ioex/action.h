#include "stdafx.h"
#include "ioex.h"

void init_vars();			//init variables, should be put at first line
void init_sys_regs();		//download initial variables to board, 
void setnodexml(int id, CString str);
void collect_reading();		//collect_reading periodically
CString getnodexml(u8 addr);//convert the node structure to xml
CString getsysxml();		//convert the sys structure to xml
bool search_node(u8 addr);	//get information of the node
void report_pack(u8);		//report pack information to display
void wait_packer(u8 grp);
void drop_packer(u8 grp);
CString xmlfilter(CString org);//filter the content based on special register allowed only
int getxmlint(CString name,CString xml);
CString getxmlstring(CString name,CString xml);
BOOL XmlAppend(CString record);
BOOL XmlDelete(CString start);
BOOL XmlQuery(CString start,CString send);
extern u8 n2l_map[];		//node to local
extern u8 l2n_map[];		//local to node
extern u8 s2l_map[];		//sys to local
extern u8 l2s_map[];		//local to sys

#define CMD_PACKER_BUSY         0x05    //check until the packer is busy or not, FAIL means busy, PASS means free
#define CMD_PACKER_INIT         0x06    //init the packer
#define CMD_PACKER_AVAILABLE    0x07    //combination available
#define CMD_PACKER_DONE         0x08    //release is donE
