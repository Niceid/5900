#ifndef _LCDMAP_H_
#define _LCDMAP_H_


#if defined (TD_ADD_USER_LCD)
//=========================================================================
//         a
//     |-------|
//     |\  |  /|
//    f| h i j |b
//     |  \|/  |
//     |-g---k-|
//     |  /|\  |
//    e| n m l |c
//     |/  |  \|
//     |-------|
//         d
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
#define VFD_DIGITAL_B		0x154f//0x047C
#define VFD_DIGITAL_C		0x039
#define VFD_DIGITAL_D		0x110F//0x045e
#define VFD_DIGITAL_E		0x0479
#define VFD_DIGITAL_F		0x0471
#define VFD_DIGITAL_G		0x047d
#define VFD_DIGITAL_H		0x0476
#define VFD_DIGITAL_I		0x1109
#define VFD_DIGITAL_J		0x001e
#define VFD_DIGITAL_K		0x0A30
#define VFD_DIGITAL_L		0x038
#define VFD_DIGITAL_M		0x02B6
#define VFD_DIGITAL_N		0x08b6
#define VFD_DIGITAL_O		0x03F
#define VFD_DIGITAL_P		0x0473
#define VFD_DIGITAL_Q		0x083F
#define VFD_DIGITAL_R		0x0C73
#define VFD_DIGITAL_S		0x046D
#define VFD_DIGITAL_T		0x1101
#define VFD_DIGITAL_U		0x003E
#define VFD_DIGITAL_V		0x2230
#define VFD_DIGITAL_W		0x2836//当M使用0x1137
#define VFD_DIGITAL_X		0x2A80
#define VFD_DIGITAL_Y		0x046e
#define VFD_DIGITAL_Z		0x2209
#define VFD_DIGITAL_01		0x800	/*.*/
#define VFD_DIGITAL_02		0x1100/*|*/
#define VFD_DIGITAL_03		0x440/*-*/
#define VFD_DIGITAL_04		0x1540/*+*/
#define VFD_DIGITAL_05		0x448/*=*/
#define VFD_DIGITAL_06		0x200/*'*/
#define VFD_DIGITAL_07		0x08/*_*/
#define VFD_DIGITAL_08		0x2080/*>*/
#define VFD_DIGITAL_09		0xa00/*<*/
#define VFD_DIGITAL_10		0x39/*[*/
#define VFD_DIGITAL_11		0x0f/*]*/
#define VFD_DIGITAL_12		0x2ec0/***/
#define VFD_DIGITAL_13		0x2200/*/*/
#define VFD_DIGITAL_14		0x880/*\*/
#define VFD_DIGITAL_15		0x154f/*#*/

#define VFD_CHAR_MAP_SEG		14  //米8点数
#define DISP_VBAR_SIZE			(4) //音效波浪条数
#define LI_NULL 				156
#define LI_R1					125
#define LI_R2					126
#define LI_R3					127
#define LI_R4					156
#define LI_MUTE					156
#define LI_LOUD				    33
#define LI_EQ					67
#define LI_POP					69
#define LI_FLAT					156
#define LI_ROCK					70
#define LI_CLASS				68
#define LI_JAZZ					156
#if defined(disenable_int_rpt_rdm)
#define LI_INT					156
#define LI_RDM					156
#define LI_RPT					156
#else
#define LI_INT					64
#define LI_RDM					65
#define LI_RPT					66
#endif
#define LI_AF					26
#define LI_TA					25
#define LI_TP					32
#define LI_PTY					24
#define LI_EON					27
#define LI_REG					156
#define LI_FILE					156
#define LI_FOLDER				156
#define LI_MUSIC				156
#define LI_ALBUM				156
#define LI_ARTIST				156
#define LI_VBAR_1				121
#define LI_VBAR_2				122
#define LI_VBAR_3				123
#define LI_VBAR_4				124
#define LI_VBAR_5				156
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
#define LI_USB					34
#define LI_SD					28
#define LI_PLAY					156
#define LI_PAUSE				156
#define LI_AUX					156
#define LI_FLASH				156
#define LI_MP3					156
#define LI_WMA					156
#define LI_WAV					156
#define LI_FLAC					156
#define LI_APE					156
#define LI_CLOCK				156
#define LI_POINT				35
#define LI_T1					156
#define LI_CH					156
#define LI_ST					156
#define LI_MONO					156
#define LI_STO					71
#define LI_LOC					156
#define LI_MHZ					156
#define LI_KHZ					156
#define LI_FM					156
#define LI_POWER				120
#define LI_DASH					156
#define LI_SCH					156
#define LI_AM					156
#define LI_PM					156


#else
#define LI_A					(0x0001<<0)
#define LI_B					(0x0001<<1)
#define LI_C					(0x0001<<2)
#define LI_D					(0x0001<<3)
#define LI_E					(0x0001<<4)
#define LI_F					(0x0001<<5)
#define LI_G					(0x0001<<6)
#define LI_H					(0x0001<<7)
#define LI_I					(0x0001<<8)
#define LI_J					(0x0001<<9)
#define LI_K					(0x0001<<10)
#define LI_L					(0x0001<<11)
#define LI_M					(0x0001<<12)
#define VFD_CHAR_MAP_SEG		13


#define SF0						LI_A
#define SF1						LI_F
#define SF2						LI_B
#define SF3						LI_I
#define SF4						LI_M
#define SF5						LI_G
#define SF6						(LI_H|LI_K)
#define SF7						LI_G
#define SF8						LI_L
#define SF9						LI_J
#define SFA						LI_E
#define SFB						LI_C
#define SFC						LI_D

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define SEG_1					31
#define SEG_2					30
#define SEG_3					29
#define SEG_4					28
#define SEG_5					27
#define SEG_6					26
#define SEG_7					25
#define SEG_8					24
#define SEG_9					23
#define SEG_10					22
#define SEG_11					21
#define SEG_12					20
#define SEG_13					19
#define SEG_14					18
#define SEG_15					17
#define SEG_16					16
#define SEG_17					15
#define SEG_18					14
#define SEG_19					13
#define SEG_20					12
#define SEG_21					11
#define SEG_22					10
#define SEG_23					9
#define SEG_24					8

#define SEG_25					0
#define SEG_26					1
#define SEG_27					2
#define SEG_28					3
#define SEG_29					4
#define SEG_30					5
#define SEG_31					6
#define SEG_32			    	7

#define COM_1					0
#define COM_2					1
#define COM_3					2
#define COM_4					3

#define L1I(COM,SEG)			((SEG)*4 + (COM))
#define L2I(COM,SEG)			((uint16)(((L1I(COM,SEG)/8)<<8)|(L1I(COM,SEG)%8)))

#define LI_NULL 				0xFFFF

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/


//System
#define LI_POWER				LI_NULL
//#define LI_RADIO				LI_NULL
//#define LI_PLAYER				LI_NULL
//#define LI_AUX					LI_NULL
#define LI_BLUETOOTH			L2I(COM_3,SEG_23)
//#define LI_MODE_USB			LI_NULL
//#define LI_MODE_SD				LI_NULL

//Audio
#define LI_MUTE					LI_NULL
#define LI_LOUD					L2I(COM_2,SEG_2)
//#define LI_SUBWOOFER			LI_NULL
//#define LI_EQ					LI_NULL
#define LI_POP					L2I(COM_3,SEG_16)
#define LI_FLAT					L2I(COM_1,SEG_32)
#define LI_ROCK					L2I(COM_2,SEG_16)
#define LI_CLASS				L2I(COM_4,SEG_16)
//#define LI_JAZZ					LI_NULL

#define LI_VBAR_1				L2I(COM_4,SEG_1)
#define LI_VBAR_2				L2I(COM_3,SEG_1)
#define LI_VBAR_3				L2I(COM_2,SEG_1)
#define LI_VBAR_4				L2I(COM_1,SEG_1)
#define LI_VBAR_5               L2I(COM_4,SEG_2)
#define LI_VBAR_6               LI_NULL
#define LI_VBAR_7               LI_NULL

//Radio
//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
//#define LI_ST					LI_NULL
#define LI_STO					L2I(COM_1,SEG_16)
//#define LI_LOC					LI_NULL
#define LI_PTY					L2I(COM_4,SEG_32)
#define LI_AF					L2I(COM_2,SEG_32)
#define LI_EON					L2I(COM_1,SEG_30)
//#define LI_REG					LI_NULL
#define LI_TA					L2I(COM_3,SEG_32)
#define LI_TP					L2I(COM_4,SEG_23)

//Player
#define LI_USB					L2I(COM_2,SEG_23)
#define LI_SD					L2I(COM_4,SEG_31)
#define LI_INT					LI_NULL
#define LI_RDM					LI_NULL
#define LI_RPT					LI_NULL

#define LI_MP3					L2I(COM_3,SEG_2)
#define LI_WMA					L2I(COM_1,SEG_2)

#define LI_R1					LI_NULL
#define LI_R2					LI_NULL
#define LI_R3					LI_NULL

//Common
#define LI_CLOCK				L2I(COM_1,SEG_23)
#define LI_POINT				L2I(COM_1,SEG_23)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//显示LCD配制
#define DISP_VBAR_SIZE			(7)
#endif
#endif /* _LCDMAP_H_ */

