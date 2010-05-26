#include "define.h"
/*
	write 1 byte to ds1302
	_w1Byte(u8 c)
*/

static void _w1Byte(u8 c)
{
	u8 i;
	for(i=8;i>0;i--)
	{
		DS1302_IO = (bit)(c & 0x01);
		DS1302_SCLK = 1;
		_nop_();
		DS1302_SCLK = 0;
		c >>= 1;
	}
	return;
}
/*
	read 1 byte from ds1302
	u8 _r1Byte()
*/
static u8 _r1Byte()
{
	u8 i;
	u8 u = 0;
	for(i=8;i>0;i--)
	{
		DS1302_IO = 1;
		u >>= 1;
		if(DS1302_IO)
			u |= 0x80;
		_nop_();
		DS1302_SCLK = 0;
	}
	return u;
}

/*
	write 1 byte to ds1302's address
	void _ds_write(u8 addr,u8 dat)
*/
static void _ds_write(u8 addr,u8 dat)
{
	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	_w1Byte(addr);
	_w1Byte(dat);
	DS1302_SCLK = 1;
	DS1302_RST = 0;
	return;
}

/*
	read 1 byte from ds1302's address
	u8 _ds_read(u8 addr)
*/
static u8 _ds_read(u8 addr)
{
	u8 dat;
	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	_w1Byte(addr);
	dat = _r1Byte();
	DS1302_SCLK = 1;
	DS1302_RST = 0;
	return dat;
}
/*
	write 8 byte to ds1302's address : 7 byte data + 1byte control
	void _ds_write8(u8 *p)
*/
static void _ds_write8(u8 *p)
{
	u8 i;
	_ds_write(0x8e,0x00); //control: WP=0
	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	_w1Byte(0xbe); 
	for(i=8;i>0;i--)
	{
		_w1Byte(*p++);
	}
	DS1302_SCLK = 1;
	DS1302_RST = 0;
	return;
}

/*
	read 8 byte from ds1302's address
	void _ds_read8(u8 addr,u8 *p)
*/
static void _ds_read8(u8 *p)
{
	u8 i;
	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	_w1Byte(0xfe); 
	for(i=8;i>0;i--)
	{
		*p++ = _r1Byte();
	}
	DS1302_SCLK = 1;
	DS1302_RST = 0;
	return;
}
/*
	write 31 byte to ds1302's address
	void _ds_write31(u8 addr,u8 *p)
*/
static void _ds_write31(u8 *p)
{
	u8 i;
	_ds_write(0x8e,0x00); //control: WP=0
	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	_w1Byte(0xfe); //0xbf: multiple bytes read
	for(i=31;i>0;i--)
	{
		_w1Byte(*p++);
	}
	DS1302_SCLK = 1;
	DS1302_RST = 0;
	return;
}

/*
	read 31 byte from ds1302's address
	void _ds_read31(u8 addr,u8 *p)
*/
static void _ds_read31(u8 *p)
{
	u8 i;
	DS1302_RST = 0;
	DS1302_SCLK = 0;
	DS1302_RST = 1;
	_w1Byte(0xff); //0xbf: multiple bytes read
	for(i=31;i>0;i--)
	{
		*p++ = _r1Byte();
	}
	DS1302_SCLK = 1;
	DS1302_RST = 0;
	return;
}
/*
	set initial time to ds1302
	input format:  
	p: 7 bytes (BCD code)	sec min hr day mon week year
	n: byte counts
	void ds1302_set(u8 addr,u8 idata *p, u8 n)
*/
void ds1302_set(u8 addr,u8 *p, u8 n)
{
	_ds_write(0x8e,0x00); //control: WP=0 start to write
	while(n-- > 0)
	{
		_ds_write(addr,*p++);
		addr += 2;
	}
	_ds_write(0x8e,0x80); //control: WP=1 start to protect
}

/*
	read current time from ds1302
	output format:
	p: 7 bytes (BCD code)	sec min hr day mon week year
	n: byte counts
	void ds1302_get(u8 addr,u8 idata *p, u8 n)
*/
void ds1302_get(u8 addr,u8 *p, u8 n)
{
	while(n-- > 0)
	{
		*p++ = _ds_read(addr);
		addr += 2;
	}
	return;
}
/*
	init the ds1302
	void ds1302_init()
*/
void ds1302_init()
{
	DS1302_SCLK = 0;
	DS1302_RST = 0;
}

