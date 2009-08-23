// ioex.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Serial.h"
#include "ioex.h"
#include "command.h"
#include "action.h"
#include "conio.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object
CWinApp theApp;

CSerial serial;

using namespace std;

NODE_CONFIG RS485_Node[MAX_NODE_NUM];
APP_NODE_CONFIG App_Node[MAX_NODE_NUM];
SYSTEM_BOARD Sysboard;

CString sAppPath;
CString sLogPath;
CStringArray sLog;

CString s_starttime,s_endtime,s_type("Default");
double g_wtotal;
int g_wcount;
int glb_debug = 1;

SOCKET sockConn;
DWORD  dwThreadId;  
HANDLE  hThread;  
CString flash_cmd("");

CString SjGetAppDirectory()
{
	CString sAppFolder;
	char szPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	sAppFolder=szPath;
	int id=sAppFolder.ReverseFind('\\');
	ASSERT(id!=-1);
	return sAppFolder.Left(id);
}

//loop of serial parser , just call cm_pushc to update the register
static char sendBuf[1500];
void check_serial()
{
	static char h;
	static char l;
	int pos = 0;
 	  //send the recieved bytes on socket
	  if(serial.ReadDataWaiting() > 0){
		  
		  int cnt = serial.ReadData(sendBuf,1500);
		  ASSERT(cnt != 1500);
		  if(cnt <= 0)	  
			  return;
		  if(sockConn == SOCKET_ERROR)	//flash connection is closed
			  return;

		  for(int i=0;i<cnt;i++){
			  cm_pushc(sendBuf[i]); //parse command
		  }
	  }else{
		  Sleep(1);
	  }
	}
//send str to flash
void answer_flash(CString str){
	if(sockConn == SOCKET_ERROR)
		return;
	const char a='\0';
	send(sockConn,(LPCTSTR)str,str.GetLength(),0);
	send(sockConn,&a,1,0);
}
CString ExtractString(CString &parm)
{
	CString ret;
	if(parm.Find(":") < 0){
		ret = parm;
		return ret;
	}
	ret = parm.Left(parm.Find(":"));
	parm.Delete(0,ret.GetLength() + 1);
	return ret;
}
//big handler here:)
//command format	cmdid:parm1:parm2...
//return command will be in the same format;
int flash_cmd_handler(){
	CString stemp;
	CString cmdid,cmdparm;

	static u32 cali_zero, cali_poise;

	if(flash_cmd.IsEmpty())
		return 0;

	if(flash_cmd.Find("\n") < 0)
		return 0;
	stemp = flash_cmd.Left(flash_cmd.Find("\n"));
	flash_cmd.Delete(0,stemp.GetLength()+1);

	cmdid = ExtractString(stemp);
	cmdparm = stemp;

	if(cmdid == "p1_chkpwd"){	//check password on page one :  p1_chkpwd:user:pwd	return p1_chkpwd_pass or p1_chkpwd_fail
		CString suser,spwd;
		suser=ExtractString(cmdparm);
		spwd =ExtractString(cmdparm); 
		/*
		if(spwd != ReadProfile("PASSWORD",suser)){
			printf("invalid user account\n");
			answer_flash("status:_root.p1_status:str_login_fail");
			return 1;
		}
		*/
		//init_vars();
		//init_sys_regs();

		if(!search_node(0xff)){
			answer_flash("status:_root.p1_status:findsys_fail");
			return 1;
		}
		
		answer_flash("p1_chkpwd_pass:0:0"); 
#if 1
		//u8 oldnum = Sysboard.node_num;
		answer_flash(_T("status:_root.glb_status:searchnode"));
		//research all the nodes
		setnodexml(0xff,"<opt flag_start_machine[0]='2'"); 
		Sleep(2000);
		for(int addr=1;addr<MAX_NODE_NUM;addr++){
			CString saddr;
			saddr.Format("%i",addr);
			Sysboard.node_num = MAX_NODE_NUM;

			if(search_node(addr) && 
				((RS485_Node[addr].board & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID) &&
					(((RS485_Node[addr].board & BOARD_TYPE_MASK) == BOARD_TYPE_WEIGHT)  || 
					 ((RS485_Node[addr].board & BOARD_TYPE_MASK) == BOARD_TYPE_VIBRATE) || 
					 ((RS485_Node[addr].board & BOARD_TYPE_MASK) == BOARD_TYPE_COLLECT)))
			{
						answer_flash(_T("display:")+ saddr +_T(":") + getnodexml(addr));
						answer_flash(_T("log:")+ saddr +_T(":") + getnodexml(addr));
			}else{
					answer_flash(CString("nofound:")+saddr+_T(":"));
			}
		}
		setnodexml(0xff,ReadProfile("LASTCONFIG","lastweight0"));
		setnodexml(0xff,ReadProfile("LASTCONFIG","lastweight1"));
		setnodexml(0xff,ReadProfile("LASTCONFIG","lastweight2"));
		setnodexml(0xff,ReadProfile("LASTCONFIG","lastweight3"));
		CString toxml;
		CString packercfg;
		
		packercfg = ReadProfile("LASTCONFIG","lastpacker0");
		toxml.Format("<opt offset_up_limit[0]='%i' flag_start_machine[0]='%i'/>",atoi(packercfg),CMD_PACKER_INIT);
		setnodexml(0xff,toxml);		
		Sleep(toxml.GetLength()*10);
		toxml.Format(_T("%i:%i"),256*int(atoi(packercfg)/256),atoi(packercfg) % 256);
		answer_flash(_T("updatepacker:0:")+toxml);

		packercfg = ReadProfile("LASTCONFIG","lastpacker1");
		toxml.Format("<opt offset_up_limit[1]='%i' flag_start_machine[1]='%i'/>",atoi(packercfg),CMD_PACKER_INIT);
		setnodexml(0xff,toxml);		
		Sleep(toxml.GetLength()*10);
		toxml.Format(_T("%i:%i"),256*int(atoi(packercfg)/256),atoi(packercfg) % 256);
		answer_flash(_T("updatepacker:1:")+toxml);


		packercfg = ReadProfile("LASTCONFIG","lastpacker2");
		toxml.Format("<opt offset_up_limit[2]='%i' flag_start_machine[2]='%i'/>",atoi(packercfg),CMD_PACKER_INIT);
		setnodexml(0xff,toxml);		
		Sleep(toxml.GetLength()*10);
		toxml.Format(_T("%i:%i"),256*int(atoi(packercfg)/256),atoi(packercfg) % 256);
		answer_flash(_T("updatepacker:2:")+toxml);

		packercfg = ReadProfile("LASTCONFIG","lastpacker3");
		toxml.Format("<opt offset_up_limit[3]='%i' flag_start_machine[3]='%i'/>",atoi(packercfg),CMD_PACKER_INIT);
		setnodexml(0xff,toxml);		
		Sleep(toxml.GetLength()*10);
		toxml.Format(_T("%i:%i"),256*int(atoi(packercfg)/256),atoi(packercfg) % 256);
		answer_flash(_T("updatepacker:3:")+toxml);

		for(int addr=1;addr<MAX_NODE_NUM;addr++){
			if((RS485_Node[addr].board & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID){
				if((RS485_Node[addr].board & BOARD_TYPE_MASK) == BOARD_TYPE_WEIGHT) 
				{
					CString item;
					item.Format("lastnode%i",RS485_Node[addr].addr);
					setnodexml(RS485_Node[addr].addr,ReadProfile("LASTCONFIG",item));
				}
				if((RS485_Node[addr].board & BOARD_TYPE_MASK) == BOARD_TYPE_VIBRATE) 
				{
					CString item;
					item.Format("lastvib%i",RS485_Node[addr].board & BOARD_GROUP_MASK);
					setnodexml(RS485_Node[addr].addr,ReadProfile("LASTCONFIG",item));
				}

			}
		}
		answer_flash(_T("status:_root.glb_status:searchnode_done"));
		answer_flash(_T("productsel:")+ReadProfile("LASTCONFIG","lastindex1")+_T(":")+ReadProfile("LASTCONFIG","lastindex2"));
		//Sysboard.node_num = oldnum;
#endif
	}
	//just update the content of the node
	if(cmdid == "search"){	//search node:	search:addr			return  display:saddr:nodexml
		CString saddr;
		saddr = ExtractString(cmdparm);
		int addr = atoi(saddr);
		
		if(search_node(addr) && 
			((RS485_Node[addr].board & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID) &&
				(((RS485_Node[addr].board & BOARD_TYPE_MASK) == BOARD_TYPE_WEIGHT)  || 
				 ((RS485_Node[addr].board & BOARD_TYPE_MASK) == BOARD_TYPE_VIBRATE) || 
				 ((RS485_Node[addr].board & BOARD_TYPE_MASK) == BOARD_TYPE_COLLECT))){
					answer_flash(_T("display:")+ saddr +_T(":") + getnodexml(addr));
		}else{
					answer_flash(CString("nofound:")+saddr+_T(":"));
		}
	}
	if(cmdid == "wrflash") //write flash downcfg:saddr:nodexml	no return;
	{
		answer_flash(_T("status:_root.glb_status:flashnode"));
		int addr=atoi(ExtractString(cmdparm));
		if(addr != 0xff){
			if((Sysboard.running[(RS485_Node[addr].board & BOARD_GROUP_MASK)])	||
				(addr >= MAX_NODE_NUM) ||
				(RS485_Node[addr].board == (BOARD_TYPE_INVALID|BOARD_GROUP_NONE))){
				answer_flash(_T("status:_root.glb_status:node_invalid"));
				return 1;
			}
		}
		setnodexml(addr,ExtractString(cmdparm));
		while(RS485_Node[addr].NumOfDataToBePgmed > 0){
			Sleep(500);
			search_node(addr);
		}
		answer_flash(_T("status:_root.glb_status:flashnode_done"));
		return 1;
	}
	if(cmdid == "downcfg"){	//download config , downcfg:saddr:nodexml	no return;
		answer_flash(_T("status:_root.glb_status:downnode"));		
		int addr=atoi(ExtractString(cmdparm));
		if(addr != 0xff){
			if((Sysboard.running[(RS485_Node[addr].board & BOARD_GROUP_MASK)] > BOARD_GROUP_D)	||
				(addr >= MAX_NODE_NUM) ||
				(RS485_Node[addr].board == (BOARD_TYPE_INVALID|BOARD_GROUP_NONE))){
				answer_flash(_T("status:_root.glb_status:node_invalid"));
				return 1;
			}

		}
		setnodexml(addr,ExtractString(cmdparm));
		Sleep(cmdparm.GetLength()*1);
		if(addr != 0xff){
			search_node(addr);
		}
		answer_flash(_T("status:_root.glb_status:downnode_done"));
		return 1;
	}
	if(cmdid == "startrun")
	{
		time_t t;
		time(&t);
		struct tm t_now;
		if(0 == localtime_s(&t_now,&t))
		{
			CString ymd;
			
			ymd.Format("%i%02i%02i_%02i%02i%02i.txt",1900+t_now.tm_year,t_now.tm_mon,t_now.tm_mday,t_now.tm_hour,t_now.tm_min,t_now.tm_sec);
			s_starttime.Format("%i-%02i-%02i %02i:%02i:%02i",1900+t_now.tm_year,t_now.tm_mon,t_now.tm_mday,t_now.tm_hour,t_now.tm_min,t_now.tm_sec);
			sLogPath = sAppPath + _T("\\log\\") + ymd;
			CStdioFile csf;
			if(csf.Open(sLogPath,CFile::modeCreate|CFile::modeWrite))
			{
				csf.Close();
				if(sLog.GetCount() > 0)
					sLog.RemoveAll();
				g_wcount = 0;
				g_wtotal = 0;
			}else{
				printf("failed to open log file\n");
			}
		}
	}
	if(cmdid == "stoprun")
	{
		if(sLog.GetCount() > 0)
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
		}else{
			return 1;
		}
		time_t t;
		time(&t);
		struct tm t_now;
		if(0 == localtime_s(&t_now,&t))
		{

			s_endtime.Format(_T("%i-%02i-%02i %02i:%02i:%02i"),1900+t_now.tm_year,t_now.tm_mon,t_now.tm_mday,t_now.tm_hour,t_now.tm_min,t_now.tm_sec);
			CString srecord;
			if (g_wcount > 0)
			{
				srecord.Format(_T("<node type='%s' start='%s' end='%s' number='%i' weight='%.2d'/>"),s_type,s_starttime,s_endtime,g_wcount,g_wtotal/g_wcount);
				XmlAppend(srecord);
			}
			//append a record to history.log starttime,endtime,count, weight average,
		}
	}
	if (cmdid == "productsel") //node info of level1
	{
		CString id = ExtractString(cmdparm);
		WriteProfile("LASTCONFIG","lastindex1",id);
		WriteProfile("LASTCONFIG","lastindex2",cmdparm);
	}

	if(cmdid == "productxml")	//save the current all product setting. format is productxml:detail.txt:xmlstring no return;
	{
		CStdioFile cf;
		//update the product.xml file
		CString detail = ExtractString(cmdparm);

		if(cf.Open(PRODUCT_NAME,CFile::modeCreate|CFile::modeWrite)){
			cf.WriteString(cmdparm);
			cf.Close();
		}else
			return 1;
		
		
		if(detail.IsEmpty())
			return 1;
		//update all data setting file
		for(int i=1;i<MAX_NODE_NUM;i++)
		{
			if((RS485_Node[i].board & BOARD_TYPE_MASK) != BOARD_TYPE_INVALID)
			{
				CString id;
				id.Format("n%i",RS485_Node[i].addr);
				WriteProfile(detail,id,getnodexml(i));

			}
		}
	}
	if(cmdid == "downvib")	//download the config of group setting. format is downvib:group(0-3):nodexml
	{
		int group=atoi(ExtractString(cmdparm));
		
		if(group < 0 || group > 3)
		{
			answer_flash(_T("status:_root.glb_status:downgrp_fail"));
			return 1;
		}

		s_type = getxmlstring("name",cmdparm);

		CString item;
		
		//set group property and 
		CString nxml;
		nxml.Format("<node gw_target[%i]='%i' gw_uvar[%i]='%i' gw_dvar[%i]='%i' />",group,getxmlint("gw_target",cmdparm),\
																					  group,getxmlint("gw_uvar",cmdparm),\
																					  group,getxmlint("gw_dvar",cmdparm));
		item.Format("lastweight%i",group);
		WriteProfile("LASTCONFIG",item,nxml);

		setnodexml(0xff,nxml);
		nxml.Format("<node magnet_time='%i' magnet_amp='%i' />",getxmlint("main_freq",cmdparm),getxmlint("main_amp",cmdparm));

		item.Format("lastvib%i",group);
		WriteProfile("LASTCONFIG",item,nxml);

		//set the data of vibrate
		for(int i=1;i<MAX_NODE_NUM;i++)
		{
			if((group == (RS485_Node[i].board & BOARD_GROUP_MASK)) && (BOARD_TYPE_VIBRATE == (RS485_Node[i].board & BOARD_TYPE_MASK)))
			{
				setnodexml(RS485_Node[i].addr ,nxml);
				break;
			}
		}
		CString detail = getxmlstring("detail",cmdparm);
		WriteProfile("LASTCONFIG","lastdetail",detail);
		//set the data of all the related node
		for(int i=1;i<MAX_NODE_NUM;i++)
		{
			if((group == (RS485_Node[i].board & BOARD_GROUP_MASK)) && (BOARD_TYPE_WEIGHT == (RS485_Node[i].board & BOARD_TYPE_MASK)))
			{
				CString id;
				id.Format("n%i",RS485_Node[i].addr);
				setnodexml(RS485_Node[i].addr ,xmlfilter(ReadProfile(detail,id)));
				item.Format("lastnode%i",RS485_Node[i].addr);
				WriteProfile("LASTCONFIG",item,xmlfilter(ReadProfile(getxmlstring("detail",cmdparm),id)));
			}
		}
		answer_flash(_T("status:_root.glb_status:refreshnode"));
		search_node(0xff);
		for(int i=1;i<MAX_NODE_NUM;i++)
		{
			if(group == (RS485_Node[i].board & BOARD_GROUP_MASK))
			{
				search_node(i);
			}
		}
		answer_flash(_T("status:_root.glb_status:downgrp_done"));
	}
	
	if(cmdid == "copycfg"){	//copy config to all the nodes in same group and download config , copycfg:saddr:nodexml	no return;
		int addr=atoi(ExtractString(cmdparm));
		CString parmstr;
		if(addr != 0xff){
			if(Sysboard.running[(RS485_Node[addr].board & BOARD_GROUP_MASK)])
			{
				answer_flash(_T("status:_root.glb_status:failofrunning"));
				return 1;
			}
			if(addr >= MAX_NODE_NUM)
				return 1;
			if(RS485_Node[addr].board == (BOARD_TYPE_INVALID|BOARD_GROUP_NONE))
			{
				answer_flash(_T("status:_root.glb_status:node_invalid"));
				return 1;	
			}
			parmstr = ExtractString(cmdparm);
			for(int i=0;i < MAX_NODE_NUM;i++){
				if(((RS485_Node[i].board & BOARD_TYPE_MASK) != BOARD_TYPE_WEIGHT))
					continue;
				if(((RS485_Node[i].board & BOARD_GROUP_MASK) == BOARD_GROUP_NONE))
					continue;
				if((RS485_Node[i].board & BOARD_GROUP_MASK) == (RS485_Node[addr].board & BOARD_GROUP_MASK))
				{
					answer_flash(_T("status:_root.glb_status:confignode"));
					setnodexml(i,xmlfilter(parmstr));
					Sleep(parmstr.GetLength()*1);
					search_node(addr);
				}
			}
			answer_flash(_T("status:_root.glb_status:confignode_done"));
		}
		return 1;
	}
	
	if(cmdid == "docali"){	// do calibration docali:addr:pos:std
		answer_flash(_T("status:_root.glb_status:calinode"));
		CString saddr = ExtractString(cmdparm);
		int ipos = atoi(ExtractString(cmdparm));
		int istd = atoi(ExtractString(cmdparm));
		int addr=atoi(saddr);
		CString toxml;
		if(search_node(addr)){
			if(ipos == 0){
				toxml.Format("<opt cs_zero='%i' />",RS485_Node[addr].cs_mtrl);
				setnodexml(addr,toxml);
				RS485_Node[addr].cs_zero = RS485_Node[addr].cs_mtrl;
			}else{
				if(ipos <=5 ){
					--ipos;
					toxml.Format("<opt Poise_Weight_gram[%i]='%i' cs_poise[%i]='%i' />",ipos,istd,ipos,RS485_Node[addr].cs_mtrl);
					setnodexml(addr,toxml);
					RS485_Node[addr].Poise_Weight_gram[ipos] = istd; 
					RS485_Node[addr].cs_poise[ipos] = RS485_Node[addr].cs_mtrl;
				}
			}
			answer_flash(_T("status:_root.glb_status:calinode_done"));
		}else{
			answer_flash(_T("status:_root.glb_status:calinode_fail"));
		}
	}
/*
	if(cmdid == "changegroup"){	//changegroup:addr:newgroup
		CString saddr = ExtractString(cmdparm);
		CString temp;
		int addr = atoi(saddr);
		RS485_Node[addr].board = (RS485_Node[addr].board & BOARD_TYPE_MASK) + atoi(cmdparm);
		temp.Format("<opt board='%i'/>",RS485_Node[addr].board);
		setnodexml(addr,temp);
		Sleep(500);
		search_node(addr);
	}
*/
	if(cmdid == "clickgrp")
	{
		CString saddr = ExtractString(cmdparm);
		saddr.Trim("abcdefghijklmnopqrstuvwxyz_");
		int grp = atoi(saddr)-1;
		for(int i=0;i<MAX_NODE_NUM;i++)
		{
			
			if((grp == (RS485_Node[i].board & BOARD_GROUP_MASK)) && (BOARD_TYPE_VIBRATE == (RS485_Node[i].board & BOARD_TYPE_MASK)))
			{
				answer_flash(_T("showvib:")+saddr+_T(":") + getnodexml(i));
				CString sys;
				sys.Format(_T("<node gw_target='%i' gw_uvar='%i' gw_dvar='%i' />"),Sysboard.gw_target[grp],Sysboard.gw_uvar[grp],Sysboard.gw_dvar[grp]);
				answer_flash(_T("showvib2:")+saddr+_T(":") +sys);
				
				break;
			}
		}

	}
	if(cmdid == "clickset"){
		CString saddr = ExtractString(cmdparm);
		saddr.Trim("abcdefghijklmnopqrstuvwxyz_");
		int addr = atoi(saddr);
		answer_flash(_T("showwei:")+saddr+_T(":") + getnodexml(addr));

		switch(RS485_Node[addr].board & BOARD_TYPE_MASK)
		{
		case BOARD_TYPE_WEIGHT:
			answer_flash(_T("showwei:")+saddr+_T(":") + getnodexml(addr));//here saddr means the address of the node
			break;
		case BOARD_TYPE_COLLECT:
			//no handler yet
			break;
		default:
			break;
		}
	}
	if(cmdid == "initpacker") //format initpacker:grp:configint:delayint
	{
		CString sgrp = ExtractString(cmdparm);
		CString trigtype = ExtractString(cmdparm);
		CString toxml;
		sgrp.Trim("abcdefghijklmnopqrstuvwxyz_");

		toxml.Format("<opt offset_up_limit[%i]='%i' flag_start_machine[%i]='%i'/>",atoi(sgrp),atoi(trigtype)+atoi(cmdparm),atoi(sgrp),CMD_PACKER_INIT);
		setnodexml(0xff,toxml);
		Sleep(toxml.GetLength()*10);
		answer_flash(_T("updatepacker:")+sgrp+_T(":")+trigtype+_T(":")+cmdparm);
		toxml.Format(_T("%i"),atoi(trigtype)+atoi(cmdparm));
		WriteProfile("LASTCONFIG","lastpacker"+sgrp,toxml);
	}
	if(cmdid == "his_query") //format his_query:start day:end day
	{
		CString sstart = ExtractString(cmdparm);
		XmlQuery(sstart,cmdparm);
		answer_flash(_T("loadqry:0:result.xml"));
	}
	if (cmdid == "his_delete") //format his_delete:start day
	{
		
	}
	return 1;
}
//loop of handler with flash UI,  just call cmd_handler
DWORD  WINAPI  AnswerThread(LPVOID  lparam){
	  static char recvBuf[1000];
	  CString str("");
	  while(1){
		  int cnt=recv( sockConn , recvBuf , 999 , 0 );
		  if(cnt == SOCKET_ERROR){
			  printf("socket error");
			  break;
		  }
		  if(cnt == 0){	
			  printf("socket closed");
			  break;
		  }
		  //handle recieved command;
		  recvBuf[cnt] = '\0';
		  
		  flash_cmd = flash_cmd+CString(recvBuf);
		  flash_cmd.Replace("\\n","\n");
	  }
	  closesocket( sockConn ) ;
	  sockConn = SOCKET_ERROR;
	  return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	xmlfilter(_T("<node  addr='9' board='16' baudrate='0' Poise_Weight_gram0='50' Poise_Weight_gram1='100' Poise_Weight_gram2='200' Poise_Weight_gram3='300' Poise_Weight_gram4='500' cs_poise0='36227' cs_poise1='37571' cs_poise2='40262' cs_poise3='42948' cs_poise4='48330' cs_zero='34882' target_weight='100' offset_up='25' cs_Filter_option='18' cs_gain_wordrate='3' motor_speed='1' magnet_freq='9' magnet_amp='60' magnet_time='2' delay_f='20' delay_w='20' delay_s='20' open_s='10' open_w='10' revision='0' reserved1='170' reserved2='255' cs_mtrl='35136' Mtrl_Weight_gram='9' Mtrl_Weight_decimal='23' status='0' cs_status='0' hw_status='0' cs_sys_gain_cal_data='0' cs_sys_offset_cal_data='0' reserved3='34882' NumOfDataToBePgmed='0' flag_reset='0' flag_enable='0' flag_disable='0' flag_release='0' flag_goon='0' test_mode_reg1='0' test_mode_reg2='0' mode='0' phase='0' motor_pulse_num='0' mag_pulse_num='0' half_period='0' />"));
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
	AfxOleInit(); 
	sAppPath=SjGetAppDirectory();
	if((argc != 6) && (argc != 4)){
		printf("normal usage: ioex port baud stopbit parity databit \ndebug usage: ioex port baud nodeaddr");
		return 0;
	}
	if(argc == 6){
	
	//打开串口
	if(!serial.Open(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]))){
  	    printf("open serial failed");
		return 0;
	}
	init_vars();
	init_sys_regs();

	

	//初始化socket编程环境
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ){
		printf("WSAStartup failed\n");
		return 0;
	}
	if ( LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1 ) {
		printf("WSA Version is not 1.1\n");
		WSACleanup( );
		return 0;
	}

	//创建Tcp服务器socket
	SOCKET sockSrv = socket( AF_INET , SOCK_STREAM , 0 );
 
	//服务器地址
	SOCKADDR_IN addrSrv ;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY) ;
	addrSrv.sin_family = AF_INET ;
	addrSrv.sin_port = htons(10255) ;
 
	//将socket与地址绑定在一起
	bind( sockSrv ,(SOCKADDR*)&addrSrv , sizeof(SOCKADDR) ) ;

	//开始监听客户端请求,最大连接数为5
	listen( sockSrv , 1) ;

	sockConn = SOCKET_ERROR;
	
	while(1){
		if(sockConn == SOCKET_ERROR){
			printf("waiting for flash connect...\n");
			while(sockConn == SOCKET_ERROR){
				check_serial();
				sockConn = accept( sockSrv , NULL, NULL ) ;
			}
			printf("flash connected\n");
			hThread = CreateThread(NULL,NULL,AnswerThread,NULL ,0,&dwThreadId);  
			if(hThread==NULL)  {
					printf("CreatThread  AnswerThread failed.\n");
					closesocket(sockConn);
					sockConn = SOCKET_ERROR;
			}else{
				printf("CreatThread  AnswerThread OK.\n");
			}
		}
		check_serial();
		while(1 == flash_cmd_handler())
		{
			Sleep(1);	//handle flash command
		}

		collect_reading(); //collect the reading of the bucket periodically
  }
  //关闭串口
  serial.Close();
 
  //关闭socket
  if(sockConn != SOCKET_ERROR)
		closesocket(sockConn);

  //清理socket编程环境
  WSACleanup() ;

  }
	if(argc == 4){
		glb_debug = 1;
		int nodeaddr =0;
	sAppPath=SjGetAppDirectory();
	//打开串口
	
	if(!serial.Open(atoi(argv[1]),atoi(argv[2]),0,0,8)){
  	    printf("open serial failed");
		return 0;
	}
	printf("debug mode\n");
	nodeaddr = atoi(argv[3]);
	init_vars();
	init_sys_regs();

	if(!search_node(nodeaddr)){
		printf("failed to find node %i",nodeaddr);
		serial.Close();
		return 0;
	}

	printf("please input (r[1,2,4] reg,w[1,2,4] reg value):");
	char buf[20];
	char c;
	int pos=0;
	char cmd = '0';
	int reg=-1;
	int size = 0;
	int value = 0;
	Sysboard.node_num = 30;
	while(1){
		check_serial();
		if(_kbhit()){
			c = _getch();
			_putch(c);
			if(c == '\n' || c== '\r'){
				printf("\n");
				buf[pos]='\0';
				if((cmd == 'r')&&(pos == 0)){ //repeat last read
					sprintf_s(buf,"%c%d %d",cmd,size,reg);
				}
				if((cmd == 'w')&&(pos == 0)){ //repeat last write
					sprintf_s(buf,"%c%d %d %i",cmd,size,reg,value);
				}

				cmd = '0';
				if(sscanf_s(buf,"r%d %i",&size,&reg) >= 2){
					if((size == 1) || (size == 2) || (size == 4)){
						cmd = 'r';
						//printf("query %d %d\n",reg,size);
						cm_query(nodeaddr,reg,size);
					}
				}
				if(sscanf_s(buf,"w%d %i %i",&size,&reg,&value) >= 3){
					if((size == 1) || (size == 2) || (size == 4)){
						cmd = 'w';
						//printf("set %d %d %i\n",reg,size,value);
						cm_write(nodeaddr,reg,size,(unsigned char*)&value);
					}
				}
				if(sscanf_s(buf,"qui%c",&c) > 0){
					break;	
				}
				if(pos !=0)
					printf("please input (r[1,2,4] reg,w[1,2,4] reg value):");
				pos = 0;
				
				
			}else{
				if(pos >= 19)
					pos = 0;
				buf[pos++] = c;
			}
		}
	}
  //关闭串口
  serial.Close();

	}

	}
	
  return nRetCode;
}
