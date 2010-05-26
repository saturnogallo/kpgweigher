#include "define.h"

/*
	LED definition
*/
#define SEG_A 0x01
#define SEG_B 0x02
#define SEG_C 0x04
#define SEG_D 0x08
#define SEG_E 0x10
#define SEG_F 0x20
#define SEG_G 0x40
#define SEG_H 0x80

#define NUM_0 SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F
#define NUM_1 SEG_B|SEG_C
#define NUM_2 SEG_A|SEG_B|SEG_D|SEG_E|SEG_G
#define NUM_3 SEG_A|SEG_B|SEG_C|SEG_D|SEG_G
#define NUM_4 SEG_B|SEG_C|SEG_F|SEG_G
#define NUM_5 SEG_A|SEG_C|SEG_D|SEG_F|SEG_G
#define NUM_6 SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G
#define NUM_7 SEG_A|SEG_B|SEG_C
#define NUM_8 SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G
#define NUM_9 SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G

#define CHAR_a CHAR_A 
#define CHAR_b CHAR_B
#define CHAR_c SEG_D|SEG_E|SEG_G
#define CHAR_d SEG_B|SEG_C|SEG_D|SEG_E|SEG_G
#define CHAR_e CHAR_E
#define CHAR_f CHAR_F 
#define CHAR_g NUM_9
#define CHAR_h CHAR_H
#define CHAR_i NUM_1
#define CHAR_j CHAR_BLANK
#define CHAR_k CHAR_BLANK
#define CHAR_l CHAR_L 
#define CHAR_m CHAR_BLANK
#define CHAR_n SEG_B|SEG_C|SEG_E|SEG_F|SEG_H
#define CHAR_o SEG_C|SEG_D|SEG_E|SEG_G
#define CHAR_p CHAR_P
#define CHAR_q CHAR_BLANK
#define CHAR_r SEG_E|SEG_G
#define CHAR_s CHAR_BLANK
#define CHAR_t SEG_D|SEG_E|SEG_F|SEG_G
#define CHAR_u CHAR_BLANK
#define CHAR_v CHAR_BLANK
#define CHAR_w CHAR_BLANK
#define CHAR_x CHAR_BLANK
#define CHAR_y CHAR_BLANK
#define CHAR_z CHAR_BLANK

#define CHAR_A SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G
#define CHAR_B SEG_C|SEG_D|SEG_E|SEG_F|SEG_G
#define CHAR_C SEG_A|SEG_D|SEG_E|SEG_F
#define CHAR_D CHAR_d 
#define CHAR_E SEG_A|SEG_D|SEG_E|SEG_F|SEG_G
#define CHAR_F SEG_A|SEG_E|SEG_F|SEG_G 
#define CHAR_G NUM_9
#define CHAR_H SEG_B|SEG_C|SEG_E|SEG_F|SEG_G
#define CHAR_J CHAR_BLANK
#define CHAR_K CHAR_BLANK
#define CHAR_I NUM_1
#define CHAR_L SEG_D|SEG_E|SEG_F
#define CHAR_M CHAR_BLANK
#define CHAR_N SEG_A|SEG_B|SEG_C|SEG_E|SEG_F
#define CHAR_O NUM_0
#define CHAR_P SEG_A|SEG_B|SEG_E|SEG_F|SEG_G
#define CHAR_Q CHAR_BLANK
#define CHAR_R CHAR_r
#define CHAR_S SEG_A|SEG_C|SEG_D|SEG_F|SEG_G
#define CHAR_T CHAR_t
#define CHAR_U SEG_B|SEG_C|SEG_D|SEG_E|SEG_F
#define CHAR_V CHAR_BLANK
#define CHAR_W CHAR_BLANK
#define CHAR_X CHAR_BLANK
#define CHAR_Y SEG_B|SEG_C|SEG_D|SEG_F|SEG_G
#define CHAR_Z CHAR_BLANK

#define CHAR_BLANK 0x00 

typedef union _PTSEGS
{
	u16	dat;
	struct {
		u8 lseg; //seg1 to seg8
		u8 hseg; //seg9 to seg16
	}C0;
}PTSEGS;

#define LED_NUMBER	6

PTSEGS LED_DIGITALS[LED_NUMBER];
//DP: digital point
//CM: comma point
//ID: indicate point
#define LED_DP_ON(x)        ((LED_DIGITALS[x].C0.lseg) | 0x80)
#define LED_CM_ON(x)        ((LED_DIGITALS[x].C0.lseg) | 0x80)
#define LED_ID_ON(x)        ((LED_DIGITALS[x].C0.lseg) | 0x80)

const u16 code LED_DIGI_CODE[] = {NUM_0,NUM_1,NUM_2,NUM_3,NUM_4,NUM_5,NUM_6,NUM_7,NUM_8,NUM_9};
const u16 code LED_LCHAR_CODE[] = {CHAR_A,CHAR_B,CHAR_C,CHAR_D,CHAR_E,CHAR_F,CHAR_G,CHAR_H,CHAR_I,\
								   CHAR_J,CHAR_K,CHAR_L,CHAR_M,CHAR_N,CHAR_O,CHAR_P,CHAR_Q,CHAR_R,\
								   CHAR_S,CHAR_T,CHAR_U,CHAR_V,CHAR_W,CHAR_X,CHAR_Y,CHAR_Z};
const u16 code LED_UCHAR_CODE[] = {CHAR_a,CHAR_b,CHAR_c,CHAR_d,CHAR_e,CHAR_f,CHAR_g,CHAR_h,CHAR_i,\
								   CHAR_j,CHAR_k,CHAR_l,CHAR_m,CHAR_n,CHAR_o,CHAR_p,CHAR_q,CHAR_r,\
								   CHAR_s,CHAR_t,CHAR_u,CHAR_v,CHAR_w,CHAR_x,CHAR_y,CHAR_z};
void	Led_print(u8 *str,u8 size)
{
	u8 i;
	if(size > LED_NUMBER)
		return;
	if((*str) == '.')
		return;
	for(i=(LED_NUMBER-size);i<LED_NUMBER;i++){
		if(((*str) <= '9') && ((*str) >= '0'))
		{
			LED_DIGITALS[i] = LED_DIGI_CODE[(*str -'0')];
			continue;
		}
		if(((*str) <= 'Z') && ((*str) >= 'A'))
		{
			LED_DIGITALS[i] = LED_UCHAR_CODE[(*str -'0')];
			continue;
		}
		if(((*str) <= 'z') && ((*str) >= 'a'))
		{
			LED_DIGITALS[i] = LED_UCHAR_CODE[(*str -'0')];
			continue;
		}

		switch(*str){

			case '.':  
					LED_DP_ON(i); 
					i--;
					break;
			case ',':  
					LED_CM_ON(i); 
					i--;
					break;
			case '^':
					LED_ID_ON(i);
					i--;
					break;
			case ' ':
					LED_DIGITALS[i] = CHAR_BLANK;
					break;
			
			default: 
					break;
			}
		str++;
	}
}


// VFD管脚定义
sbit VFDDOUT=P2^6;
sbit VFDDIN=P2^5;
sbit VFDCLK=P2^0;
sbit VFDSTB=P2^4;
#define PT6312_DOUT()         VFDDOUT//GET_PORT( PINC,PINC3 )
#define PT6312_DIN_H()        VFDDIN = 1//SET_PORT( PORTC,PC4 )
#define PT6312_DIN_L()        VFDDIN = 0//CLR_PORT( PORTC,PC4 )
#define PT6312_CLK_H()        VFDCLK = 1//SET_PORT( PORTC,PC5 )
#define PT6312_CLK_L()        VFDCLK = 0//CLR_PORT( PORTC,PC5 )
#define PT6312_STB_H()        VFDSTB = 1//SET_PORT( PORTC,PC6 )
#define PT6312_STB_L()        VFDSTB = 0//CLR_PORT( PORTC,PC6 )
#define PT6312_DDRC_INIT()    //for avr only: DDRC |= _BV(PC4) | _BV(PC5) | _BV(PC6); DDRC &= ~_BV(PC3)

// 向指定的LED(从0开始编号）写入数据(假设只用前8段）
#define PT6312_WriteN( n,dat )  PT6312_WriteData( ((n) << 1), (dat) )


// 前置声明
static void _SendData( u8 dat );
static void _SendCommand( u8 dat );
static u8 _ReadSWData();
// 向指定地址写入数据
static void _WriteData( u8 addr,u8 dat );

/*
00 : Write data to display memory
01 : Write data to LED port
10 : Read key data
11 : Read SW data
*/
#define WR_MEM	0
#define	WR_LED	1
#define RD_KEY	2
#define RD_SW	3

#define ADDR_INC	0
#define ADDR_FIX	1

#define MODE_WORK	0
#define MODE_TEST	1

/*	
	000 : Set pulse width to 1/16.
	001 : 2/16
	010 : 4/16
	011 : 10/16
	100 : 11/16
	101 : 12/16
	110 : 13/16
	111 : 14/16
*/
#define DISP_DIM_BASE	0
#define DISP_OFF		0
#define	DISP_ON			1

// 设置显示方式
#define SET_MODE_COMMAND( cmd,dispmode )    cmd.C0.Mode = dispmode; cmd.C0.Null = 0; cmd.C0.Cmd = 0;

// 数据IO
#define DATA_IO_COMMAND( cmd, iomode,addrmode,testmode) cmd.C1.IoMode=iomode;cmd.C1.AddrMode=addrmode;cmd.C1.TestMode=testmode;cmd.C1.Null= 0;cmd.C1.Cmd = 1;

// 地址设定
#define SET_ADDR_COMMAND( cmd,addr ) cmd.C3.Addr = addr; cmd.C3.Null = 0; cmd.C3.Cmd = 3;

// 显示控制
#define DISP_CONTROL_COMMAND( cmd,light,control) cmd.C2.Light = light;cmd.C2.Control = control;cmd.C2.Null = 0;cmd.C2.Cmd = 2;


// VFD模式
#define PT6312_MODE            0x02 // 6 digits * 16 segment 

// PT6312命令
typedef union uPT6312Command
{
    u8 Data;
    
    // 设置显示方式
/*	
	Mode:
	000 : 4 digits 16 segments
	001 : 5 digits 16 segments
	010 : 6 digits 16 segments
	011 : 7 digits 15 segments
	101 : 9 digits 13 segments
	110 : 10 digits 12 segments
	111 : 11 digits 11 segments
*/
    struct
    {
        u8 Mode    : 3;    // 显示屏硬件的栅和段的位数
        u8 Null    : 3;    // 数据空位
        u8 Cmd    : 2;     // 固定为00
    }C0;

    // 数据IO
	/*
	00 : Write data to display memory
	01 : Write data to LED port
	10 : Read key data
	11 : Read SW data
	*/
    struct
    {
        u8 IoMode    : 2;    // IO模式
        u8 AddrMode    : 1;  // 地址模式 （0：递增 1：固定地址）
        u8 TestMode    : 1;  // 测试模式（0：正常操作 1： 测试模式）
        u8 Null        : 2;  // 无用
        u8 Cmd        : 2;   // 固定为01
    }C1;

    // 地址设定
    struct
    {
        u8 Addr    : 5;    // 地址（00-15H）
        u8 Null    : 1;    // 无用
        u8 Cmd    : 2;     // 固定为11
    }C3;

    // 显示控制
	/*
	000 : Set pulse width to 1/16.
	001 : 2/16
	010 : 4/16
	011 : 10/16
	100 : 11/16
	101 : 12/16
	110 : 13/16
	111 : 14/16
	*/
    struct
    {    
        u8 Light        : 3;    // 调光等级
        u8 Control    : 1;      // 显示控制（ 0 关闭 1 显示）
        u8 Null        : 2;     // 无用
        u8 Cmd        : 2;      // 固定为10
    }C2;

}PT6312Command;



// 向PT6312写数据(跟在命令后面，所需不需设置STB）
static void _SendData( u8 dat )
{
    u8 i = 0;
    for( i = 0; i < 8; i++)
    {
        PT6312_CLK_H();
        PT6312_CLK_H();
        PT6312_CLK_L();
        PT6312_CLK_L();

        if( dat & 0x01 )
            PT6312_DIN_H();
        else
            PT6312_DIN_L();

        dat = dat >> 1;
        PT6312_CLK_H();
        PT6312_CLK_H();
        PT6312_CLK_H();
        PT6312_CLK_H();
    }
}

// 向PT6312写命令
static void _SendCommand( u8 dat )
{
    PT6312_STB_H();
    PT6312_STB_H();
    PT6312_STB_L();
    _SendData( dat );
}

// 向指定地址写入数据
static void _WriteData( u8 addr,u8 dat )
{
    PT6312Command command;

    // 数据IO
    DATA_IO_COMMAND( command,WR_MEM,ADDR_FIX,MODE_WORK);
    _SendCommand( command.Data );
    PT6312_STB_H();

    // 指定地址
    SET_ADDR_COMMAND( command,addr );
    _SendCommand( command.Data );
    _SendData( dat );
    PT6312_STB_H();

    // 显示控制
    DISP_CONTROL_COMMAND( command,DISP_DIM_BASE+7,DISP_ON ); //light = 7, control = 1
    _SendCommand( command.Data );
    PT6312_STB_H();
}

// 读取SW数据
static u8 _ReadSWData()
{
    u8 i = 0,dat = 0;
    PT6312Command command;
    DATA_IO_COMMAND( command,RD_SW,ADDR_FIX,MODE_WORK);
    _SendCommand( command.Data );
    for( i = 0; i < 4; i++ )
    {
        PT6312_CLK_H();
        PT6312_CLK_H();
        PT6312_CLK_L();
        PT6312_CLK_L();
        PT6312_CLK_L();
        PT6312_CLK_H();
        if( PT6312_DOUT() )
            dat = ( dat << 1 ) +1;
        else
            dat = dat << 1;
    }
    
    return dat;
}

// 初始化PT6312
void PT6312_Init()
{
    PT6312Command command;

    // 设定端口输出方向
    PT6312_DDRC_INIT();

    // 设置显示模式
    SET_MODE_COMMAND( command,PT6312_MODE );
    _SendCommand( command.Data );
    PT6312_STB_H();
    
    // 显示控制
    DISP_CONTROL_COMMAND( command,DISP_DIM_BASE+7,DISP_ON );
    _SendCommand( command.Data );
    PT6312_STB_H();
}

// 写入LED数据
void PT6312_WriteLED( u8 dat )
{
    PT6312Command command;
    DATA_IO_COMMAND( command,WR_LED,ADDR_FIX,MODE_WORK);
    _SendCommand( command.Data );
    _SendData( dat );
    PT6312_STB_H();
}


// 读取键盘
u32 PT6312_ReadKey()
{
    u8 i      = 0;
    u32 key    = 0;
    PT6312Command command;
    DATA_IO_COMMAND( command,RD_KEY,ADDR_FIX,MODE_WORK);
    _SendCommand( command.Data );
    for( i = 0; i < 24; i++ )
    {
        PT6312_CLK_H();
        PT6312_CLK_H();
        PT6312_CLK_L();
        PT6312_CLK_L();
        PT6312_CLK_L();
        PT6312_CLK_H();
        if( PT6312_DOUT() )
            key = ( key << 1 ) +1;
        else
            key = key << 1;
    }
    return key;
}


// 清空缓冲区
void PT6312_Refresh()
{
    u8 i = 0;
	u8 *pt = (u8*)LED_DIGITALS;
    for( i = 0; i <= 0x15; i++ )
    {
        // SEG1-SEG16 全为0
        _WriteData( i, *pt++ ); 
    }
}

// 测试PT6312的程序
void PT6312_Test()
{
    u8 i     = 0;
    u8 j        = 0;
    u32 key    = 0;
    
    // 初始化
    PT6312_Init();    

        
    
    while(1)
    {

        // 读取键盘
        key = PT6312_ReadKey();
        
        // 显示键盘值
        j = 0;
        for( i = 0; i < 8; i++,j+=2 )
        {
            _WriteData( j, LED_DP_OFF( key%10) );
            key = key /10;
            if( key == 0 )
                break;
        }    
    }
    
}


