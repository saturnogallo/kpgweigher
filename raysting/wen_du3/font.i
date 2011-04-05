//#define NULL 0x0
typedef flash struct typFNT_GB24		// 汉字字模数据结构
{
	unsigned char Index[4];
	unsigned char Msk[72];
}FNT_GB24;
typedef flash struct typFNT_GB16		// 汉字字模数据结构
{
	unsigned char Index[4];
	unsigned char Msk[32];
}FNT_GB16;
typedef flash struct typFNT_GB12		// 汉字字模数据结构
{
	unsigned char Index[4];
	unsigned char Msk[24];
}FNT_GB12;
//字库
extern FNT_GB12 flash HZK12[];
extern FNT_GB16 flash HZK16[];
extern unsigned char flash ASC8x16[256][16];
extern unsigned char flash Font24X32[13][96];
extern unsigned char flash font6x8[256][8];
