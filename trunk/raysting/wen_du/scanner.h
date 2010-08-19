#ifndef	_SCANNER_H_
#define _SCANNER_H_
#include "utili.h"

#define CMD_RESERVE	0x00
#define CMD_ACTION	0x40
#define	CMD_QUERY	0x80
#define RET_ERROR	0xC0
#define STATE_MASK	0xC0
#define	WIRE_2		0x00
#define WIRE_4		0x20
#define	WIRE_MASK	0x20
#define POS_MASK	0x1f

void scanner_set_channel(uchar ch);
#endif
