/* CH374芯片 应用层 V1.0 */
/* USB设备,模拟CH372或CH375的TEST程序与计算机通讯 */

#include	"HAL.H"			// 以MCS51为例，其它单片机需修改HAL*硬件抽象层的几个文件
//#include	"HAL_BASE.C"	// 基本子程序及中断查询子程序

/* 硬件接口层,以下连接方式任选一种 */
//#include "..\PARA_HW.C"	/* 硬件标准8位并口 */
//#include "..\PARA_SW.C"	/* 软件I/O模拟8位并口 */
//#include "..\SPI_HW.C"	/* 硬件标准4线SPI串口 */
//#include "SPI_SW.C"	/* 软件I/O模拟4线SPI串口 */
//#include "..\SPI3_SW.C"	/* 软件I/O模拟3线SPI串口,SDO和SDI合用一个引脚 */

// 设备描述符
const	UINT8C	MyDevDescr[] = {0x12,  //length of desc, 18 bytes
								0x01,  //type: dev desc is 01
								0x10,0x01,  //USB version 1.1 , low byte first
								0x00,0x00,0x00,//old one, 0xFF, 0x80, 0x37, //dev class (0xFF is owner defined, subclass, protocal, 
								0x08, //max pack size (8byte) 
								0x48, 0x43, 0x01, 0x05,  // 厂商ID和产品ID
								0x01, 0x00, //bcd Device version
								0x01, 0x02, 0x00,	//index to manufacutre string , index to product string, index to serial string (0 means no serial),
								0x01 };			 // number of configuration. 
// 配置描述符
//
// old
/*   0x09, 0x02, 0x27, 0x00, 0x01, 0x01, 0x00, 0x80, 0x32,
								 0x09, 0x04, 0x00, 0x00, 0x03, 0xFF, 0x80, 0x37, 0x00,
								 0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00,
								 0x07, 0x05, 0x02, 0x02, 0x40, 0x00, 0x00,
								 0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0x00 */
const	UINT8C	MyCfgDescr[] = { 0x09, //len of desc
				 0x02, //type: cfg desc is 0x02
				 0x37,0x00,//total len
				 0x01, //num of interface
				 0x01, //config value;
				 0x00, //string index to this configuration, 00 means no string
				 0x80, //attribute, 1 0(1,selfpower) 0(1,remote wakeup) 0 0 0 0 0
				 0x64, //max power, 100*2 = 200mA
								 0x09, //len of interface
								 0x04, //type:  intf desc is 0x04
								 0x00, //index of this intf
								 0x00, //alternate setting
								 0x04, //number of endp
								 0x03, //old is 0xFF, //interface class
								 0x00, //old is 0x80, //subclass
								 0x00, //old is 0x37, //protocol
								 0x00, //string index to interface, 0 means no serial
								 //HID desc should be put here (after each interface)
								 	0x09, //len of HID
									0x21, //type: HID desc is 0x21
									0x10,0x01,// hid1.1
									0x21, //country(US)
									0x01, //desc number2s
									0x22, //desc type (22 is report, 23 is physic)
									0x23,0x00, //(35byte of report desc

				 	 0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00,  //endp , len 0x07, type 0x05, address 0x82 (D7 means dir), attrib 0x02 (0,ctrl, 1,ISO, 2.batch, 3. interrupt), max packsize 0x0040,   interval 0
					 0x07, 0x05, 0x02, 0x02, 0x40, 0x00, 0x00,  //batch, 64 byte at max, interval 20ms
					 0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0x20, 
					 0x07, 0x05, 0x01, 0x03, 0x08, 0x00, 0x20,};//interrupt, 8 byte at max, interval 20ms , the chip has only 8 bytes buffer for endp0 and endp1, 64 bytes for endp2
					 
//where is  Interface Descriptor and Endpoint desc and string desc
//USB2.0 has Device Qualifier Desc and Other speed Configuration desc
// 语言描述符
const	UINT8C	MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 }; //len of lang desc, type is 0x03, lang id 0 is 0409, //us english
// 厂家信息
const	UINT8C	MyManuInfo[] = { 18, 0x03, 'R', 0, 'a', 0, 'y', 0, 's', 0, 't', 0, 'i', 0, 'n', 0,'g', 0}; //string desc len, type 0x03 is string, string in unicode
// 产品信息
const	UINT8C	MyProdInfo[] = { 16, 0x03, 'R', 0, 'T', 0, 'B', 0, 'o', 0, 'a', 0, 'r', 0, 'd', 0 }; //product string
// 报告描述符
const   UINT8C MyReportDescr[] = {
0x06, 0x00 , 0xff,  //USAGE_PAGE (Vendor Defined page1)
0x09 ,0x01, //USAGE (Vendor Usage 1)
0xa1 ,0x01,  //COLLECTION (Application)
0x19 ,0x01, //USAGE_MINIMUM (Vendor Usage 1)
0x29 ,0x07, //USAGE_MAXIMUM (keypad)
0x15 ,0x00, // LOGICAL_MINIMUM is 0
0x26 ,0xff, 0x00,  //LOGICAL_MAXIMUM is 255
0x75 ,0x08, //REPORT_SIZE(8)
0x95 ,0x07, //REPORT_COUNT(7)
0x81 ,0x02, //INPUT data, var, abs, no wrap, linear, preferred state, no null, non volatile, bit field
0x19 ,0x01, //USAGE_MINIMUM (Vendor Usage 1)
0x29 ,0x07, //USAGE_MAXIMUM (keypad)
0x91 ,0x02, //output const, array, abs, no wrap, linear, preferred state, no null, non volatile, bit field
0x19 ,0x01, //USAGE_MINIMUM (Vendor Usage 1)
0x29 ,0x07, //USAGE_MAXIMUM (keypad)
0xb1 ,0x02, //FEATURE(data, var, abs, no wrap, linear, preferred state, no null, non volatile, bit field)
0xc0 //END COLLECTION
};

UINT8	UsbConfig = 255;	// USB配置标志

void	USB_DeviceInterrupt( void );  // USB设备中断服务程序

void	Init374Device( void );  // 初始化USB设备

extern void sjSerialSendByte(UINT8C d);

unsigned char code HexTable[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void DBG(UINT8C d)
{
	unsigned char b;
	b = HexTable[d & 0x0f];
	sjSerialSendByte(HexTable[d >> 4]);
	sjSerialSendByte(b);
}

void DBGS(const char* s)
{
	while(*s != 0x00)
	{
		sjSerialSendByte(*s++);
	}
}


void DBGA(unsigned char s[],int l)
{
	int cnt = 0;
	unsigned char b;
	while(cnt < l)
	{
		b = HexTable[s[cnt] & 0x0f];
		sjSerialSendByte(HexTable[s[cnt++] >> 4]);
		sjSerialSendByte(b);
	}
}

extern void CommandProcessor();



#define USBOUT_BUF_MAX	12
UINT8   uo_buf[USBOUT_BUF_MAX];
UINT8   uo_ptr_out = 0; //1st valid data to read
UINT8   uo_ptr_in = 0; //next empty space

void UsbSendByte(UINT8 c)
{
	uo_buf[uo_ptr_in++] = c;
	if(uo_ptr_in >= USBOUT_BUF_MAX)
		uo_ptr_in = 0;
}

UINT8 UsbGetByte()
{
	UINT8 c;
	while(uo_ptr_out == uo_ptr_in)
		;
	c = uo_buf[uo_ptr_out++];
	if(uo_ptr_out >= USBOUT_BUF_MAX)
		uo_ptr_out = 0;
	return c;
}
UINT8 UsbHasByteToSend()	
{
	if(uo_ptr_in==uo_ptr_out)
		return 0;
	if(uo_ptr_in > uo_ptr_out)
		return uo_ptr_in - uo_ptr_out;
	else
		return uo_ptr_in + USBOUT_BUF_MAX - uo_ptr_out;
}
UINT8    FlushToEndp1() //interrupt endp;
{

	UINT8 len = UsbHasByteToSend();
	UINT8 cnt;
		return 0;
	if(len == 0)
	{
		return 0;
	}
	
	if(len > 8)		len = 8; //8 byte at max
	cnt = len;
	
	Spi374Start( RAM_ENDP1_TRAN, CMD_SPI_374WRITE );
	while ( len-- ) 
	{
		Spi374OutByte( UsbGetByte() );
	}
	MySpi374Stop();
	Write374Byte( REG_USB_LENGTH, cnt );
	return cnt;


}


UINT8 ep2busy = 0;

UINT8    FlushToEndp2(UINT8 toggle) //batch endp
{
	UINT8 cnt = 0;
	UINT8 len = 0;
	
	if(UsbConfig == 255) //not setup yet
		return 0;

	if(ep2busy > 0)
		return 0;

	len = sjSerialIsDataWaiting()+io_hasc();
	if(len == 0)
	{
			Write374Byte( REG_USB_ENDP2, M_SET_EP2_TRAN_NAK( Read374Byte( REG_USB_ENDP2 ) ) ^ toggle );
			ep2busy = 0;
			return 0;
	}
	ep2busy = 1;

	Spi374Start( RAM_ENDP2_TRAN, CMD_SPI_374WRITE );
	//head
	Spi374OutByte(0xAA); 
	Spi374OutByte(0x55);
	cnt = 2;
	//usb buffer
	len = UsbHasByteToSend();
	Spi374OutByte(len);
	cnt++;
	while ( len-- > 0 && cnt < 64 ) //64 bytes at most
	{
	 	Spi374OutByte( UsbGetByte());
		cnt++;
	}
	//serial port 1
	len = sjSerialIsDataWaiting();
	Spi374OutByte(len);
	cnt++;
	while ( len-- > 0 && cnt < 64 ) //64 bytes at most
	{
	 	Spi374OutByte( sjSerialWaitForOneByte());
		cnt++;
	}
	//analog serial port2
	len = io_hasc();
	Spi374OutByte(len);
	cnt++;
	while ( len-- > 0 && cnt < 64 ) //64 bytes at most
	{
	 	Spi374OutByte( io_getc());
		cnt++;
	}


	MySpi374Stop( );
	
	Write374Byte( REG_USB_LENGTH, cnt );
	Write374Byte( REG_USB_ENDP2, M_SET_EP2_TRAN_ACK( Read374Byte( REG_USB_ENDP2 )  ) ^ toggle );
		

	return cnt;
}
UINT8	buf[64];
UINT8	len;
UINT8	ptr;
void	USB_DeviceInterrupt( void )  // USB设备中断服务程序
{
	UINT8	s, l;
	static	UINT8	SetupReq, SetupLen;
	static	PUINT8	pDescr;
	s = Read374Byte( REG_INTER_FLAG );  // 获取中断状态

	if ( s & BIT_IF_BUS_RESET ) {  // USB总线复位

		Write374Byte( REG_USB_ADDR, 0x00 );  // 清USB设备地址, 3个端点
		Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_NAK( 0 ) ); //控制传输
		Write374Byte( REG_USB_ENDP1, M_SET_EP1_TRAN_NAK( 0 ) ); //中断端点
		Write374Byte( REG_USB_ENDP2, M_SET_EP2_TRAN_NAK( 0 ) ); //批量端点
		Write374Byte( REG_INTER_FLAG, BIT_IF_USB_PAUSE | BIT_IF_BUS_RESET );  // 清中断标志
		return;
	}
	if ( s & BIT_IF_TRANSFER ) {  // USB传输完成
//		DBGS("*");
		s = Read374Byte( REG_USB_STATUS );
//		DBG(s);
		switch( s & BIT_STAT_PID_ENDP ) {  // USB设备中断状态
			//批量端点
			case USB_INT_EP2_OUT: {  // 批量端点下传成功 
				
				if ( s & BIT_STAT_TOG_MATCH ) {  // 仅同步包
					l = Read374Byte( REG_USB_LENGTH );
					Read374Block( RAM_ENDP2_RECV, l, buf );
					
					DBGA(buf,l);
					len = l;
					ptr = 0;
//					CommandProcessor();
//				if(FlushToEndp2() > 0)
//				{
//					Write374Byte( REG_USB_ENDP2, M_SET_EP2_TRAN_ACK( Read374Byte( REG_USB_ENDP2 )  ) ^ BIT_EP2_RECV_TOG );  // DATA1
//					ep2busy = 1;
//				}
//				else
//				{
					Write374Byte( REG_USB_ENDP2, M_SET_EP2_TRAN_NAK( Read374Byte( REG_USB_ENDP2 ) ) ^ BIT_EP2_RECV_TOG );
//				}
				DBGS("EP2_OUT\r\n");
				UsbSendByte(0x01);
				UsbSendByte(0x07);
				UsbSendByte(0x32);
				UsbSendByte(0x33);
				UsbSendByte(0x34);
				UsbSendByte(0x35);
				UsbSendByte(0x36);

//					Write374Index( REG_USB_ENDP2 );  // 对于并口连接可以用本行及下面一行代替上一行的程序,减少写一次index的时间,提高效率
//					Write374Data( M_SET_EP2_TRAN_ACK( Read374Data0( ) ) ^ BIT_EP2_RECV_TOG );
				}
				break;
			}
			case USB_INT_EP2_IN: {  // 批量端点上传成功
				ep2busy = 0;
				//FlushToEndp2(BIT_EP2_TRAN_TOG);
				Write374Byte( REG_USB_ENDP2, M_SET_EP2_TRAN_NAK( Read374Byte( REG_USB_ENDP2 ) ) );
				DBGS("EP2_IN\r\n");

					



//				Write374Index( REG_USB_ENDP2 );  // 对于并口连接可以用本行及下面一行代替上一行的程序,减少写一次index的时间,提高效率
//				Write374Data( M_SET_EP2_TRAN_NAK( Read374Data0( ) ) ^ BIT_EP2_TRAN_TOG );
				break;
			}
			//中断端点
			case USB_INT_EP1_OUT:{ // download done
				UINT8 len;
									DBGS("EP1_OUT\r\n");
//				if ( s & BIT_STAT_TOG_MATCH ) {  // 仅同步包
					l = Read374Byte( REG_USB_LENGTH );
					DBG(l);
					Read374Block( RAM_ENDP1_RECV, l, buf );
					DBGA(buf,l);
				len = FlushToEndp1();
				DBGS("EP1_IN\r\n");
				if(0 == len)
					Write374Byte( REG_USB_ENDP1, M_SET_EP1_TRAN_NAK( Read374Byte( REG_USB_ENDP1 ) )^ BIT_EP1_RECV_TOG );
				else
					Write374Byte( REG_USB_ENDP1, M_SET_EP1_TRAN_ACK( Read374Byte( REG_USB_ENDP1 ), len ) ^ BIT_EP1_RECV_TOG );  // DATA1					


//				}
				break;
			}
			case USB_INT_EP1_IN: {  // 中断端点上传成功,未处理
				UINT8 len = FlushToEndp1();
				DBGS("EP1_IN\r\n");
				if(0 == len)
					Write374Byte( REG_USB_ENDP1, M_SET_EP1_TRAN_NAK( Read374Byte( REG_USB_ENDP1 ) )^ BIT_EP1_TRAN_TOG );
				else
					Write374Byte( REG_USB_ENDP1, M_SET_EP1_TRAN_ACK( Read374Byte( REG_USB_ENDP1 ), len ) ^ BIT_EP1_TRAN_TOG );  // DATA1					
				break;
			}
			//控制端点
			case USB_INT_EP0_SETUP: {  // 控制传输
				USB_SETUP_REQ	SetupReqBuf;
				DBGS("SETUP_:");
				l = Read374Byte( REG_USB_LENGTH );
				if ( l == sizeof( USB_SETUP_REQ ) ) {
					Read374Block( RAM_ENDP0_RECV, l, (PUINT8)&SetupReqBuf );
					SetupLen = SetupReqBuf.wLengthL;
					if ( SetupReqBuf.wLengthH || SetupLen > 0x7F ) SetupLen = 0x7F;  // 限制总长度
					l = 0;  // 默认为成功并且上传0长度
					if ( ( SetupReqBuf.bType & DEF_USB_REQ_TYPE ) == DEF_USB_REQ_CLASS ) {  /* 只支持标准请求 和 DEF_USB_REQ_CLASS, DEF_USB_REQ_VENDOR 不处理*/
						DBGS("REQ_CLASS_");
						SetupReq = SetupReqBuf.bReq;
						switch(SetupReq) {
							case 0x0A : //SET_IDLE //TODO
								DBGS("SET_IDLE\r\n");
								break;
							default: //TODO
								DBG(SetupReqBuf.bReq);
								DBGS("??1\r\n");
								break;
						}
					}
					else if ( ( SetupReqBuf.bType & DEF_USB_REQ_TYPE ) == DEF_USB_REQ_STAND ) 
					{  // 标准请求
						DBGS("REQ_STAND_");
						SetupReq = SetupReqBuf.bReq;  // 请求码
						switch( SetupReq ) {
							case DEF_USB_GET_DESCR: //0x60
								DBGS("GET_DESC_");
								switch( SetupReqBuf.wValueH ) {
									case 1: //dev string
										DBGS("dev_desc\r\n");
										pDescr = (PUINT8)( &MyDevDescr[0] );
										l = sizeof( MyDevDescr );
										break;
									case 2: //cfg string
										DBGS("cfg_desc\r\n");
										pDescr = (PUINT8)( &MyCfgDescr[0] );
										l = sizeof( MyCfgDescr );
										break;
									case 0x22: //report desc
										DBGS("rpt_desc\r\n");
										pDescr = (PUINT8)( &MyReportDescr[0] );
										l = sizeof( MyReportDescr );
										break;
									case 3: //string index
										DBGS("str_desc\r\n");
										switch( SetupReqBuf.wValueL ) {
											case 1: //id = 1
												pDescr = (PUINT8)( &MyManuInfo[0] );
												l = sizeof( MyManuInfo );
												break;
											case 2: //id=2
												pDescr = (PUINT8)( &MyProdInfo[0] );
												l = sizeof( MyProdInfo );
												break;
											case 0: //id=0
												pDescr = (PUINT8)( &MyLangDescr[0] );
												l = sizeof( MyLangDescr );
												break;
											default:
												l = 0xFF;  // 操作失败
												break;
										}
										break;
									default:
										DBG(SetupReqBuf.wValueH);
										DBGS("??2\r\n");
										l = 0xFF;  // 操作失败
										break;
								}
								if ( SetupLen > l ) SetupLen = l;  // 限制总长度
								l = SetupLen >= RAM_ENDP0_SIZE ? RAM_ENDP0_SIZE : SetupLen;  // 本次传输长度
								Write374Block( RAM_ENDP0_TRAN, l, pDescr );  /* 加载上传数据 */
								SetupLen -= l;
								pDescr += l;
								break;
							case DEF_USB_SET_ADDRESS:
								DBGS("SET_ADDR\r\n");
								SetupLen = SetupReqBuf.wValueL;  // 暂存USB设备地址
								break;
							case DEF_USB_GET_CONFIG:
								DBGS("GET_CFG\r\n");
								Write374Byte( RAM_ENDP0_TRAN, UsbConfig );
								if ( SetupLen >= 1 ) l = 1; //already have a address
								break;
							case DEF_USB_SET_CONFIG:
								DBGS("SET_CFG\r\n");
								UsbConfig = SetupReqBuf.wValueL;
								break;
							case DEF_USB_CLR_FEATURE:
								DBGS("CLR_FEATURE\r\n");
								if ( ( SetupReqBuf.bType & 0x1F ) == 0x02 ) {  // 不是端点不支持
									DBG(SetupReqBuf.wIndexL);
									switch( SetupReqBuf.wIndexL ) {
										case 0x82: //batch endp
											Write374Byte( REG_USB_ENDP2, M_SET_EP2_TRAN_NAK( Read374Byte( REG_USB_ENDP2 ) ) );
											break;
										case 0x02:
											Write374Byte( REG_USB_ENDP2, M_SET_EP2_RECV_NAK( Read374Byte( REG_USB_ENDP2 ) ) );
											break;
										case 0x81://interrupt endp
											Write374Byte( REG_USB_ENDP1, M_SET_EP1_TRAN_NAK( Read374Byte( REG_USB_ENDP1 ) ) );
											break;
										case 0x01:
											Write374Byte( REG_USB_ENDP1, M_SET_EP1_RECV_NAK( Read374Byte( REG_USB_ENDP1 ) ) );
											break;
										default:
											l = 0xFF;  // 操作失败
											break;
									}
								}
								else l = 0xFF;  // 操作失败
								break;
							case DEF_USB_GET_INTERF:
								DBGS("GET_INTERF\r\n");
								Write374Byte( RAM_ENDP0_TRAN, 0 );
								if ( SetupLen >= 1 ) l = 1;
								break;
							case DEF_USB_GET_STATUS:
								DBGS("GET_STATUS\r\n");
								Write374Byte( RAM_ENDP0_TRAN, 0 );
								Write374Byte( RAM_ENDP0_TRAN + 1, 0 );
								if ( SetupLen >= 2 ) l = 2;
								else l = SetupLen;
								break;
							default:
								DBG(SetupReq);
								DBGS("??3\r\n");
								l = 0xFF;  // 操作失败
								break;
						}
					}
					else
					{
						l = 0xFF;
					}
				}
				else l = 0xFF;  // 操作失败
				if ( l == 0xFF ) {  // 操作失败
					Write374Byte( REG_USB_ENDP0, M_SET_EP0_RECV_STA( M_SET_EP0_TRAN_STA( 0 ) ) );  // STALL
				}
				else if ( l <= RAM_ENDP0_SIZE ) {  // 上传数据
					Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_ACK( M_SET_EP0_RECV_ACK( Read374Byte( REG_USB_ENDP0 ) ), l ) | BIT_EP0_TRAN_TOG );  // DATA1
				}
				else {  // 下传数据或其它
					Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_NAK( M_SET_EP0_RECV_ACK( Read374Byte( REG_USB_ENDP0 ) ) ) | BIT_EP0_RECV_TOG );  // DATA1
				}
				break;
			}
			case USB_INT_EP0_IN: {
				DBGS("IN_");
				switch( SetupReq ) { //SetupReq here is the one in last USB_INT_EP0_SETUP command
					case DEF_USB_GET_DESCR:
						l = SetupLen >= RAM_ENDP0_SIZE ? RAM_ENDP0_SIZE : SetupLen;  // 本次传输长度
						Write374Block( RAM_ENDP0_TRAN, l, pDescr );  /* 加载上传数据 */
						SetupLen -= l;
						pDescr += l;
						Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_ACK( Read374Byte( REG_USB_ENDP0 ), l ) ^ BIT_EP0_TRAN_TOG );
						DBGS("DESC\r\n");
						break;
					case DEF_USB_SET_ADDRESS:
						Write374Byte( REG_USB_ADDR, SetupLen );
						Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_NAK( 0 ) );  // 结束
						DBGS("SETADDR\r\n");
						break;
					case DEF_USB_SET_CONFIG: //no action for configuration setting
						Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_NAK( 0 ) );  // 结束
						DBGS("SETCFG\r\n");
						break;
					case DEF_USB_GET_CONFIG:
						Write374Byte( RAM_ENDP0_TRAN, UsbConfig );
						Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_ACK( Read374Byte( REG_USB_ENDP0 ), 1 ) ^ BIT_EP0_TRAN_TOG );
						DBGS("GETCFG\r\n");
						break;
					default:
						Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_NAK( 0 ) );  // 结束
						DBG(SetupReq);
						DBGS("??4\r\n");
						break;
				}
				break;
			}
			case USB_INT_EP0_OUT: {//SetupReq here is the one in last USB_INT_EP0_SETUP command
				DBGS("OUT_");
				switch( SetupReq ) {
//					case download:
//						get_data;
//						break;
					case DEF_USB_GET_DESCR:
						Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_NAK( 0 ) );  // 结束
						DBGS("DESC\r\n");
						break;

					default:
						Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_NAK( 0 ) );  // 结束
						DBG(SetupReq);
						DBGS("??5\r\n");

						break;
				}
				break;
			}
			default: {
				DBG(s);
				DBGS("\r\n");
				break;
			}
		}
		Write374Byte( REG_INTER_FLAG, BIT_IF_USB_PAUSE | BIT_IF_TRANSFER );  // 清中断标志
		return;
	}
	if ( s & BIT_IF_USB_SUSPEND ) {  // USB总线挂起
		Write374Byte( REG_INTER_FLAG, BIT_IF_USB_PAUSE | BIT_IF_USB_SUSPEND );  // 清中断标志
		Write374Byte( REG_SYS_CTRL, Read374Byte( REG_SYS_CTRL ) | BIT_CTRL_OSCIL_OFF );  // 时钟振荡器停止振荡,进入睡眠状态
		return;
	}
	if ( s & BIT_IF_WAKE_UP ) {  // 芯片唤醒完成
		Write374Byte( REG_INTER_FLAG, BIT_IF_USB_PAUSE | BIT_IF_WAKE_UP );  // 清中断标志
		return;
	}
	DBGS("WRONG");
	// 意外的中断,不可能发生的情况,除了硬件损坏
	Write374Byte( REG_INTER_FLAG, BIT_IF_USB_PAUSE | BIT_IF_INTER_FLAG );  // 清中断标志
}

void	Init374Device( void )  // 初始化USB设备
{
	mDelaymS(50);	
	Write374Byte( REG_USB_ADDR, 0x00 );
	Write374Byte( REG_USB_ENDP0, M_SET_EP0_TRAN_NAK( 0 ) );
	Write374Byte( REG_USB_ENDP1, M_SET_EP1_TRAN_NAK( 0 ) );
	Write374Byte( REG_USB_ENDP2, M_SET_EP2_TRAN_NAK( 0 ) );
	Write374Byte( REG_INTER_FLAG, BIT_IF_USB_PAUSE | BIT_IF_INTER_FLAG );  // 清所有中断标志
	Write374Byte( REG_INTER_EN, BIT_IE_TRANSFER | BIT_IE_BUS_RESET | BIT_IE_USB_SUSPEND );  // 允许传输完成中断和USB总线复位中断以及USB总线挂起中断,芯片唤醒完成中断
	Write374Byte( REG_SYS_CTRL, BIT_CTRL_OE_POLAR );  // 对于CH374T或者UEN引脚悬空的CH374S必须置BIT_CTRL_OE_POLAR为1
	Write374Byte( REG_USB_SETUP, BIT_SETP_TRANS_EN | BIT_SETP_PULLUP_EN);  // 启动USB设备, //DEVICE MODE AND PULLUPM 
}

int	test( void )  // USB device
{
	BYTE d;
//	P1&=0xF8; // 如果在U盘文件读写模块上试用本程序必须加上本行
	mDelaymS( 50 );  // 等待CH374复位完成

	d = Read374Byte(REG_SYS_INFO);
	DBG(d);
	return 0;
//	CH374_PORT_INIT( );  // CH374接口初始化
//	Init374Device( );  // 初始化USB设备
//	while ( 1 ) {
//		if ( Query374Interrupt( ) ) USB_DeviceInterrupt( );  // 等待USB设备中断，然后处理USB设备中断
//	}
}

BOOL USBHasChar()	{
	return ptr < len;
}
unsigned char USBGetChar()	{
	if(ptr < len)	
	{
		return buf[ptr++]; 
	}
}
