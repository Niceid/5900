#ifndef _LEDMAP_H_
#define _LEDMAP_H_

//=========================================================================
//               a
//     |---------|
//     |\      |   / |
//    f| h    i   j   |b
//     |    \  |/    |
//     |-g----k--|
//     |    /|\	     |
//   e|  n m  l	     |c
//     |/    |   \   |
//     |---------|
//              d
//ponm lkji    hgfe   dcba
//0000 0000 	0000  0000
#define VFD_DIGITAL_NULL	0x0
#define VFD_DIGITAL_0		0x3f
#define VFD_DIGITAL_1		0x06
#define VFD_DIGITAL_2		0x45b
#define VFD_DIGITAL_3		0x44f
#define VFD_DIGITAL_4		0x466
#define VFD_DIGITAL_5		0x46d
#define VFD_DIGITAL_6		0x47d
#define VFD_DIGITAL_7		0x07
#define VFD_DIGITAL_8		0x47f
#define VFD_DIGITAL_9		0x46f
#define VFD_DIGITAL_A		0x477
#define VFD_DIGITAL_B		0x047C
#define VFD_DIGITAL_C		0x039
#define VFD_DIGITAL_D		0x045e
#define VFD_DIGITAL_E		0x0479
#define VFD_DIGITAL_F		0x0471
#define VFD_DIGITAL_G		0x46F
#define VFD_DIGITAL_H		0x0476
#define VFD_DIGITAL_I		0x06
#define VFD_DIGITAL_J		0x001e
#define VFD_DIGITAL_K		0x470
#define VFD_DIGITAL_L		0x038
#define VFD_DIGITAL_M		0x37
#define VFD_DIGITAL_N		0x37
#define VFD_DIGITAL_O		0x03F
#define VFD_DIGITAL_P		0x0473
#define VFD_DIGITAL_Q		0x467
#define VFD_DIGITAL_R		0x0450
#define VFD_DIGITAL_S		0x046D
#define VFD_DIGITAL_T		0x478
#define VFD_DIGITAL_U		0x003E
#define VFD_DIGITAL_V		0x3E
#define VFD_DIGITAL_W		0x1C
#define VFD_DIGITAL_X		0x0476
#define VFD_DIGITAL_Y		0x046e
#define VFD_DIGITAL_Z		0x45b
#define VFD_DIGITAL_01		0x800	/*.*/
#define VFD_DIGITAL_02		0x06/*|*/
#define VFD_DIGITAL_03		0x440/*-*/
#define VFD_DIGITAL_04		0x446/*+*/
#define VFD_DIGITAL_05		0x448/*=*/
#define VFD_DIGITAL_06		0x02/*'*/
#define VFD_DIGITAL_07		0x08/*_*/
#define VFD_DIGITAL_08		0x0/*>*/
#define VFD_DIGITAL_09		0x0/*<*/
#define VFD_DIGITAL_10		0x39/*[*/
#define VFD_DIGITAL_11		0x0f/*]*/
#define VFD_DIGITAL_12		0x0/***/
#define VFD_DIGITAL_13		0x0/*/*/
#define VFD_DIGITAL_14		0x0/*\*/
#define VFD_DIGITAL_15		0x0/*#*/


#define LI_TIME1				33
#define LI_TIME2				41
//#define LI_PION1				12

#define LI_TIME					0xffff
#define LI_AM					0xffff
#define LI_PM					0xffff


#endif

