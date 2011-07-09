#include "stdafx.h"
#include "SwiNav.h"
void l_update_buffer(char *data,int length,DWORD userdata)
{
	CSwiNav *swinav = (CSwiNav*) userdata;
	swinav->update_buffer(data,length);
}
	BOOL CSwiNav::open(){
		BOOL ret = m_serial.OpenPort(_T("COM3:"),9600,0,0,8,l_update_buffer,(DWORD)this);
		return ret;
	};
	
	void CSwiNav::close(){
		m_serial.ClosePort();
	};
	void CSwiNav::turn_nav(char *cmd){
		CString scmd(cmd);
		scmd.Replace(_T("U"),_T("UU"));
		scmd.Replace(_T("\\n"),_T("\n"));
		scmd.Replace(_T("\\r"),_T("\r"));
		scmd = _T("Un") + scmd;
		for(int i=0;i<scmd.GetLength();i++){
			unsigned char c=(unsigned char)scmd[i];
			if(c == '^'){
				::Sleep(10);
			}else{
				if((i % 16) == 15)
					Sleep(1);
				m_serial.SendData(&c,1);
			}
		}
		Sleep(100);
	};
	void CSwiNav::reset_swi(){
		swi_force_write(swi_reset);
	};
	void CSwiNav::turn_swi(const char *cmd){
		CString scmd(cmd);
		scmd += ",";

		while(scmd.Find(_T(",")) > 0){
			CString a = scmd.Left(scmd.Find(_T(",")));
			scmd.Delete(0,a.GetLength()+1);
			RELAY rl;
			for(int i=0;i<sizeof(switable)/sizeof(RELAY);i++){
				rl = switable[i];
				if(a.Right(a.GetLength()-1).Compare(CString(rl.swiname)) == 0){
					if(rl.type == 'n'){
/*
						if((rl.status == 1) && (a[0] == 'O')){
							m_serial.SendData("UsUU",4);
							m_serial.SendData(&rl.oaddr,1);
							m_serial.SendData("V",1);
							Sleep(30);
						}
						if((rl.status == 0) && (a[0] == 'F')){
							m_serial.SendData("UsUU",4);
							m_serial.SendData(&rl.faddr,1);
							m_serial.SendData("V",1);
							Sleep(30);
						}
*/
						if((a[0] == 'O') && (rl.oaddr != 0xff)){
							m_serial.SendData("UsUU",4);
							m_serial.SendData(&rl.oaddr,1);
							m_serial.SendData("V",1);
							switable[i].status = 1;
							Sleep(30);
						}

						if((a[0] == 'F') && (rl.faddr != 0xff)){
							m_serial.SendData("UsUU",4);
							m_serial.SendData(&rl.faddr,1);
							m_serial.SendData("V",1);
							switable[i].status = 0;
							Sleep(30);
						}
						break;
					}

					if(rl.type == 'r'){
						if((a[0] == 'O') && (rl.oaddr != 0xff)){
							if(rl.status == 0){
								m_serial.SendData("UsUU",4);
								m_serial.SendData(&rl.oaddr,1);
								m_serial.SendData("V",1);
								Sleep(30);
								switable[i].status = 1;
							}else{
								m_serial.SendData("UsUU",4);
								m_serial.SendData(&rl.faddr,1);
								m_serial.SendData("V",1);
								Sleep(30);
								switable[i].status = 0;
							}
						}
						break;
					}
					if(rl.type == 's'){
						if(a[0] == 'F') break;
						if((rl.status == 1) && (a[0] == 'O')) break;
						for(int j=0;j<sizeof(switable)/sizeof(RELAY);j++){
							RELAY rln = switable[j];
							if((strcmp(rln.grpname,rl.grpname) == 0) && (rln.status == 1)){
								if(rln.faddr != 0xff){
									m_serial.SendData("UsUU",4);
									m_serial.SendData(&rln.faddr,1); //close the old one
									m_serial.SendData("V",1);
									Sleep(30);
								}
								
								switable[j].status = 0;
							}
						}
						if(rl.oaddr != 0xff){
							m_serial.SendData("UsUU",4);
							m_serial.SendData(&rl.oaddr,1); //open the new one
							m_serial.SendData("V",1);
							Sleep(30);
						}
						switable[i].status  = 1;
					}
				}
			}
		}
		
	}
	bool CSwiNav::read_nav(double &value){
		value = reading;
		reading = -9999;
		CString tt;
		return (value > -1000);
	};
	SWI_VAR CSwiNav::read_swi(){
		return swivar;
	};
	void CSwiNav::update_buffer(char *data,int length)
	{
		for(int i=0;i<length;i++)
		{
			m_list.Add((unsigned char)data[i]);
		}
		parse_buffer();
	}

	bool CSwiNav::isnumber(unsigned char c){
		switch(c)
		{
		case	'-':
		case	'+':
		case	'e':
		case	'E':
		case	'.': return true;
		default:
			return ((c >= '0') && (c <= '9'));
		}
	}
	void CSwiNav::swi_force_write(const char *cmd){
		CString scmd(cmd);
		scmd += ",";
		
		while(scmd.Find(_T(",")) > 0){
			CString a = scmd.Left(scmd.Find(_T(",")));
			scmd.Delete(0,a.GetLength()+1);
			RELAY rl;
			for(int i=0;i<sizeof(switable)/sizeof(RELAY);i++){
				rl = switable[i];
				if(a.Right(a.GetLength()-1).Compare(CString(rl.swiname)) == 0){
					if((a[0] == 'O')){
						if(rl.oaddr != 0xff){
							m_serial.SendData("UsUU",4);
							m_serial.SendData(&rl.oaddr,1);
							m_serial.SendData("V",1);
							Sleep(30);
						}
						switable[i].status = 1;
					}
					if((a[0] == 'F')){
						if(rl.faddr != 0xff){
							m_serial.SendData("UsUU",4);
							m_serial.SendData(&rl.faddr,1);
							m_serial.SendData("V",1);
							Sleep(30);
						}
						switable[i].status = 0;
					}
					break;
				}
			}
		}
		
	}
	void CSwiNav::swi_convert(unsigned char p2,unsigned char p3){
		swivar.rx  = ((p2 & 0x01) != 0);
		swivar.rs  = ((p2 & 0x02) != 0);
		swivar.ov1 = ((p3 & 0x04) != 0);
		swivar.ov2 = ((p3 & 0x08) != 0);
	}
	void CSwiNav::parse_buffer(){

		bool found = false;
		if(m_list.GetSize() < 4) 
			return;
		int i;
		for(i=0;i<=m_list.GetSize() - 4;i++){
			if((m_list.GetAt(i) == 'U') && (m_list.GetAt(i+3) == 'V')){
				swi_convert(m_list.GetAt(i+1),m_list.GetAt(i+2));
				m_list.RemoveAt(i,4);
			}
		}
		CString s;
		for(i=0;i<m_list.GetSize();i++){
			s.Empty();
			if((m_list.GetAt(i) == '\r')||(m_list.GetAt(i) == ';')){
				
				if((i > 0) && isnumber(m_list.GetAt(i-1))){
					int j=i-1;
					while(j > 0){
						if(!isnumber(m_list.GetAt(j)))
							break;
						s = CString((char)m_list.GetAt(j)) + s;
						j--;
					}
					if(!s.IsEmpty() && (s.GetLength() > 6)){
						TCHAR *stopstring;
						
						reading = fabs(wcstod((LPCTSTR)s,&stopstring));
						found = true;
						m_list.RemoveAt(0,m_list.GetSize());
						break;
					}
					
				}
			}
		}
	}
