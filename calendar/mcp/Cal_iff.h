/*
**
** Calendar - Calendar MUI custom class
**
** Copyright 2004-2005 by Alfonso [alfie] Ranieri <alforan@tin.it>
** All Rights Are Reserved.
**
** Destributed Under The Terms Of The LGPL II
**
**/

#ifdef USE_CAL_COLORS
const ULONG Cal_colors[24] =
{
	0x95959595,0x95959595,0x95959595,
	0x00000000,0x00000000,0x00000000,
	0xffffffff,0xffffffff,0xffffffff,
	0x3b3b3b3b,0x67676767,0xa2a2a2a2,
	0x7b7b7b7b,0x7b7b7b7b,0x7b7b7b7b,
	0xafafafaf,0xafafafaf,0xafafafaf,
	0xffffffff,0xffffffff,0x00000000,
	0xffffffff,0xa9a9a9a9,0x97979797,
};
#endif

#define CAL_WIDTH        24
#define CAL_HEIGHT       16
#define CAL_DEPTH         3
#define CAL_COMPRESSION   0
#define CAL_MASKING       2

#ifdef USE_CAL_HEADER
const struct BitMapHeader Cal_header =
{ 24,16,19,127,3,2,0,0,0,22,22,24,16 };
#endif

#ifdef USE_CAL_BODY
const UBYTE Cal_body[192] = {
0x00,0x41,0xf0,0x00,0x00,0x81,0xf0,0x00,0x00,0x40,0x00,0x00,0x02,0xe7,0xfc,
0x00,0x01,0x47,0xfc,0x00,0x02,0x81,0xf0,0x00,0x0b,0xee,0x0e,0x00,0x05,0xef,
0xfe,0x00,0x0a,0x06,0x0c,0x00,0x2f,0xfc,0xd6,0x00,0x16,0x6f,0xac,0x00,0x29,
0x8c,0x14,0x00,0xbf,0xf8,0xeb,0x00,0x54,0xbf,0x97,0x00,0xab,0x48,0x2a,0x00,
0xff,0xd9,0xf7,0x00,0xe6,0xc6,0x8a,0x00,0x19,0x31,0x06,0x00,0xff,0x7e,0xff,
0x00,0x6b,0x05,0xf2,0x00,0x14,0xfa,0x0e,0x00,0xfd,0xff,0x5e,0x00,0xbd,0x82,
0xa6,0x00,0x02,0x79,0x5d,0x00,0xf7,0xfe,0xbe,0x00,0x51,0x41,0x4c,0x00,0x0e,
0x3c,0xbd,0x00,0xff,0xdf,0xfc,0x00,0x00,0x90,0xf4,0x00,0x1e,0x2e,0xf2,0x00,
0xff,0x7f,0xf0,0x00,0x41,0x41,0x10,0x00,0x0e,0x8c,0x0c,0x00,0xff,0x7c,0x00,
0x00,0x20,0x28,0x00,0x00,0x0f,0x93,0xf0,0x00,0xff,0xf0,0x00,0x00,0x50,0x20,
0x00,0x00,0x07,0xcc,0x00,0x00,0xbf,0xc0,0x00,0x00,0x10,0x80,0x00,0x00,0x47,
0x30,0x00,0x00,0xaf,0x00,0x00,0x00,0x88,0x00,0x00,0x00,0x50,0xc0,0x00,0x00,
0x78,0x00,0x00,0x00,0x48,0x00,0x00,0x00,0x07,0x00,0x00,0x00, };
#endif
