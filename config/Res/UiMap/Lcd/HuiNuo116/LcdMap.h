#ifndef _LCDMAP_H_
#define _LCDMAP_H_

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
#define VFD_DIGITAL_I		0x30  //0x06
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

#define VFD_CHAR_MAP_SEG		14
#define DISP_VBAR_SIZE			(5)




#define LED_NULL 				156
#define LI_NULL 				156
#define LI_R1					156
#define LI_R2					156
#define LI_R3					156
#define LI_R4					156
#define LI_MUTE					71
#define LI_LOUD				    156
#define LI_EQ					156
#define LI_POP					56
#define LI_FLAT					156
#define LI_ROCK					72
#define LI_CLASS				70
#define LI_JAZZ					67
#define LI_INT					156
#define LI_RDM					156
#define LI_RPT					156
#define LI_AF					156
#define LI_TA					156
#define LI_TP					156
#define LI_PTY					156
#define LI_EON					156
#define LI_REG					156
#define LI_FILE					156
#define LI_FOLDER				156
#define LI_MUSIC				156
#define LI_ALBUM				156
#define LI_ARTIST				156
#define LI_VBAR_1				74
#define LI_VBAR_2				75
#define LI_VBAR_3				76
#define LI_VBAR_4				65
#define LI_VBAR_5				66
#define LI_VBAR_6				156
#define LI_VBAR_7				156
#define LI_VBAR_8				156
#define LI_VBAR_9				156
#define LI_VBAR_10				156
#define LI_SUBWOOFER			156
#define LI_BLUETOOTH			156
#define LI_RADIO				156
#define LI_TRACK				156
#define LI_DISC					156
#define LI_USB					39
#define LI_SD					23
#define LI_PLAY					156
#define LI_PAUSE				156
#define LI_AUX					156
#define LI_FLASH				156
#define LI_MP3					156
#define LI_WMA					156
#define LI_WAV					156
#define LI_FLAC					156
#define LI_APE					156
#define LI_CLOCK				7
#define LI_POINT				55
#define LI_T1					156
#define LI_CH					156
#define LI_ST					156
#define LI_MONO					156
#define LI_STO					156
#define LI_LOC					156
#define LI_MHZ					156
#define LI_KHZ					156
#define LI_FM					156
#define LI_POWER				156
#define LI_DASH					156
#define LI_SCH					156
#if defined(SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)||defined(SUPPORT_ANTENNA_DISPLAY_)
#define LI_L21				   8//
#define LI_L22				   24//
#define LI_L23				   40//
#define LI_L24				   156//
#endif





#endif

