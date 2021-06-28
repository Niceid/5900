#ifndef _LCDMAP_H_
#define _LCDMAP_H_
//enter here
#ifdef	TD_ADD_USER_LCD
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
#define DISP_VBAR_SIZE			(3) //音效波浪条数
#define LI_NULL 				156
#define LI_R1					156
#define LI_R2					156
#define LI_R3					156
#define LI_R4					156
#define LI_MUTE					156
#define LI_LOUD				    112
#define LI_EQ					156
#define LI_POP					126
#define LI_FLAT					121
#define LI_ROCK					125
#define LI_CLASS				124
#define LI_JAZZ					120
#define LI_INT					106
#define LI_RDM					123
#define LI_RPT					122
#define LI_AF					105
#define LI_TA					104
#define LI_TP					119
#define LI_PTY					118
#define LI_EON					113
#define LI_REG					156
#define LI_FILE					156
#define LI_FOLDER				156
#define LI_MUSIC				156
#define LI_ALBUM				156
#define LI_ARTIST				156
#define LI_VBAR_1				109
#define LI_VBAR_2				108
#define LI_VBAR_3				107
#define LI_VBAR_4				156
#define LI_VBAR_5				156
#define LI_VBAR_6				156
#define LI_VBAR_7				156
#define LI_VBAR_8				156
#define LI_VBAR_9				156
#define LI_VBAR_10				156
#define LI_SUBWOOFER			156
#define LI_BLUETOOTH			114
#define LI_RADIO				156
#define LI_TRACK				156
#define LI_DISC					156
#define LI_USB					127
#define LI_SD					115
#define LI_PLAY					156
#define LI_PAUSE				156
#define LI_AUX					156
#define LI_FLASH				156
#define LI_MP3					117
#define LI_MP5                  156
#define LI_WMA					116
#define LI_WAV					156
#define LI_FLAC					156
#define LI_APE					156
#define LI_CLOCK				99
#define LI_POINT				111
#define LI_T1					156
#define LI_CH					156
#define LI_ST					156
#define LI_MONO					156
#define LI_STO					156
#define LI_LOC					156
#define LI_MHZ					156
#define LI_KHZ					156
#define LI_FM					156
#define LI_POWER				110
#define LI_DASH					156
#define LI_SCH					156
#define LI_AM					156
#define LI_PM					156



#else//*******************************************************************************************************
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
#define SF6						LI_K
#define SF7						LI_H
#define SF8						LI_L
#define SF9						LI_J
#define SFA						LI_E
#define SFB						LI_C
#define SFC						LI_D


/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
enum{
	_LCD_COM1_,// 0
	_LCD_COM2_,// 1
	_LCD_COM3_,// 2
	_LCD_COM4_,// 3
	_END_COM_,// 4
};

#define L0_COM1					_LCD_COM4_
#define L0_COM2					_LCD_COM3_
#define L0_COM3					_LCD_COM2_
#define L0_COM4					_LCD_COM1_ 


#define L1_COM1(COM)			((COM == 1)? L0_COM1 : 0 )			
#define L1_COM2(COM)			((COM == 2)? L0_COM2 : L1_COM1(COM)) 
#define L1_COM3(COM)			((COM == 3)? L0_COM3 : L1_COM2(COM)) 
#define L1_COM4(COM)			((COM == 4)? L0_COM4 : L1_COM3(COM)) 


#define L1I_COM(COM)			(L1_COM4(COM))
#define L1I_SEG(SEG)			((SEG - 1) * _END_COM_)


#define L1I(COM,SEG)			(L1I_SEG(SEG) + L1I_COM(COM))
#define L2I(COM,SEG)			((uint16)(((L1I(COM,SEG)/8)<<8)|(L1I(COM,SEG)%8)))

#define LI_NULL 				0xFFFF


/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LI_R1					L2I(3,2)
#define LI_R2					L2I(2,2)
#define LI_R3					L2I(1,2)
/*-------------------------------------*/
//#define LI_MUTE				LI_NULL
#define LI_LOUD				    L2I(3,23)
/*-------------------------------------*/
#define LI_INT					L2I(4,15)
#define LI_RDM					L2I(3,15)
#define LI_RPT					L2I(2,15)
/*-------------------------------------*/
//#define LI_FILE					LI_NULL
//#define LI_FOLDER				LI_NULL
//#define LI_MUSIC				LI_NULL
//#define LI_ALBUM				LI_NULL
//#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					L2I(4,32)
#define LI_AF					L2I(2,32)
#define LI_EON					L2I(1,32)
//#define LI_REG					LI_NULL
#define LI_TA					L2I(3,32)
#define LI_TP					L2I(4,23)
/*-------------------------------------*/	
#define LI_EQ					L2I(1,15)
#define LI_POP					L2I(3,16)
//#define LI_FLAT					LI_NULL
#define LI_ROCK					L2I(2,16)
#define LI_CLASS				L2I(4,16)
//#define LI_JAZZ					LI_NULL
/*-------------------------------------*/
#define LI_VBAR_1				L2I(3,1)
#define LI_VBAR_2				L2I(2,1)
#define LI_VBAR_3				L2I(1,1)
#define LI_VBAR_4				L2I(4,2)
/*-------------------------------------*/
//#define LI_SUBWOOFER			LI_NULL
//#define LI_ESP45				LI_NULL
#define LI_BLUETOOTH			LI_NULL
#define LI_POWER				L2I(4,1)
/*-------------------------------------*/
//#define LI_RADIO				LI_NULL
//#define LI_TRACK				LI_NULL
//#define LI_DISC					LI_NULL
#define LI_USB					L2I(2,23)
#define LI_SD					L2I(4,31)
//#define LI_AUX					LI_NULL
//#define LI_FLASH				LI_NULL

//#define LI_MP3					LI_NULL
//#define LI_MP4					LI_NULL
//#define LI_MP5					LI_NULL
//#define LI_WMA					LI_NULL
//#define LI_WAV					LI_NULL
//#define LI_ASF					LI_NULL
//#define LI_CD					LI_NULL
//#define LI_CDC					LI_NULL
//#define LI_VCD					LI_NULL
//#define LI_DVD					LI_NULL
//#define LI_JPG					LI_NULL
/*-------------------------------------*/
#define LI_CLOCK				L2I(1,23)
#define LI_POINT				L2I(1,23)

//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
/*-------------------------------------*/
//#define LI_ST					LI_NULL
#define LI_STO					L2I(1,16) 
//#define LI_LOC					LI_NULL

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//显示LCD配制
#define DISP_VBAR_SIZE			(4)

#endif
#endif /* _LCDMAP_H_ */

