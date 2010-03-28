/************************************************************
18B20驱动程序，DQ为数据口，接于P3.5
11.0592M晶振，上拉4.7k电阻
*************************************************************/
#include<stc51.h>
#include<intrins.h>
#include<utili.h>


sbit dq = P3^5;
bit  flag;
uint Temperature;
uchar temp_buff[9]; //存储读取的字节，read scratchpad为9字节，read rom ID为8字节
uchar id_buff[8];
uchar *p;
uchar crc_data;
uchar code CrcTable [256]={
0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53};
//
/************************************************************
*Function:延时处理
*parameter:
*Return:
*Modify:
*************************************************************/
void TempDelay (uchar us)
{
  while(us--);
}
/************************************************************
*Function:18B20初始化
*parameter:
*Return:
*Modify:
*************************************************************/
void Start18b20 (void)
{
 dq=1;
 _nop_();
 dq=0;
 TempDelay(86);   //delay 530 uS//80
 _nop_();
 dq=1;
 TempDelay(14);   //delay 100 uS//14
 _nop_();
 _nop_();
 _nop_();
 
 if(dq==0)
  flag = 1;   //detect 1820 success!
 else
  flag = 0;    //detect 1820 fail!
 TempDelay(20);       //20
 _nop_();
 _nop_();
 dq = 1;
}
/************************************************************
*Function:向18B20写入一个字节
*parameter:
*Return:
*Modify:
*************************************************************/
void WriteByte (uchar wr)  //单字节写入
{
 uchar i;
 for (i=0;i<8;i++)
 {
  dq = 0;
  _nop_();
  dq=wr&0x01;
  TempDelay(5);   //delay 45 uS //5
  _nop_();
  _nop_();
  dq=1;
  wr >>= 1;
 }
}
/************************************************************
*Function:读18B20的一个字节
*parameter:
*Return:
*Modify:
*************************************************************/
uchar ReadByte (void)     //读取单字节
{
 uchar i,u=0;
 for(i=0;i<8;i++)
 {
  dq = 0;
  u >>= 1;
  dq = 1;
  if(dq==1)
  u |= 0x80;
  TempDelay (4);
  _nop_();
 }
 return(u);
}
/************************************************************
*Function:读18B20
*parameter:
*Return:
*Modify:
*************************************************************/
void read_bytes (uchar j)	 //读出J个数据并存放在p指向的缓冲区
{
  uchar i;
  for(i=0;i<j;i++)
  {
    *p = ReadByte();
    p++;
  }
}
/************************************************************
*Function:CRC校验
*parameter:
*Return:
*Modify:
*************************************************************/
uchar CRC(uchar j)
{
    uchar i,crc_data=0;
   for(i=0;i<j;i++)  //查表校验
     crc_data = CrcTable[crc_data^temp_buff[i]];
    return (crc_data);
}
/************************************************************
*Function:读取温度
*parameter:
*Return:
*Modify:
*************************************************************/
void GemTemp (void)	//把温度放进变量Temperature中
{  
   read_bytes (9);
   if (CRC(9)==0) //校验正确
   {
     Temperature = temp_buff[1]*0x100 + temp_buff[0];
//   Temperature *= 0.625;
//   Temperature /= 16;
     TempDelay(1);
    }
}
/************************************************************
*Function:内部配置
*parameter:
*Return:
*Modify:
*************************************************************/
void Config18b20 (void)  //重新配置报警限定值和分辨率
{
     Start18b20();
     WriteByte(0xcc);  //skip rom
     WriteByte(0x4e);  //write scratchpad
     WriteByte(0x0);   //上限
     WriteByte(0x64);  //下限
     WriteByte(0x7f);  //set 11 bit (0.125)
     Start18b20();
     WriteByte(0xcc);  //skip rom
     WriteByte(0x48);  //保存设定值
     Start18b20();
     WriteByte(0xcc);  //skip rom
     WriteByte(0xb8);  //回调设定值
}
/************************************************************
*Function:读18B20ID
*parameter:
*Return:
*Modify:
*************************************************************/
void ReadID (void)//读取器件 id
{
 Start18b20();
 WriteByte(0x33);  //read rom
 read_bytes(8);
}
/************************************************************
*Function:18B20ID全处理
*parameter:
*Return:
*Modify:
*************************************************************/
void Init_18b20(void)
{
   p = id_buff;
   ReadID();
   Config18b20();
   Start18b20 ();
   WriteByte(0xcc);   //skip rom
   WriteByte(0x44);   //Temperature convert
   Start18b20();
   WriteByte(0xcc);   //skip rom			      
   WriteByte(0xbe);   //read Temperature
   p = temp_buff;
   GemTemp();
   rdata.Temp = Temperature/16.0;
} 

void get_18b20()
{
	Start18b20 ();
	WriteByte(0xcc);   //skip rom
	WriteByte(0x44);   //Temperature convert
	Start18b20 ();
	WriteByte(0xcc);   //skip rom			      
	WriteByte(0xbe);   //read Temperature
	p = temp_buff;
	GemTemp();		//把温度放进变量Temperature中
   rdata.Temp = Temperature/16.0;
} 
