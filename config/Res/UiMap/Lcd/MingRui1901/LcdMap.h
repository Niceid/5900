#ifndef _LCDMAP_H_
#define _LCDMAP_H_

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

#if defined(_SUPPORT_TIME_DISPLAY_)
#define VFD_ICON_MAP_SEG		13
#endif

#define SF0					LI_A
#define SF1					LI_F
#define SF2					LI_B
#define SF3					LI_I
#define SF4					LI_M
#define SF5					LI_G
#define SF6					LI_H
#define SF7					LI_G
#define SF8					LI_L
#define SF9					LI_J
#define SFA					LI_E
#define SFB					LI_C
#define SFC					LI_D

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LCD_MODE				1

#if LCD_MODE == 1
	#define L2I(COM,SEG)		((uint16)(((((SEG-1)*4+(COM-1))/8)<<8) | (((SEG-1)*4+(COM-1))%8)))
#elif LCD_MODE == 2
	#define L2I(COM,SEG)		((uint16)(((((SEG-1)*4+(4-COM))/8)<<8) | (((SEG-1)*4+(4-COM))%8)))
#endif

#define LI_NULL 				0xFFFF

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LI_R1					LI_NULL
#define LI_R2					LI_NULL
#define LI_R3					LI_NULL
/*-------------------------------------*/
//#define LI_MUTE				LI_NULL
#define LI_LOUD				    LI_NULL
/*-------------------------------------*/
#define LI_INT					LI_NULL
#define LI_RDM					LI_NULL
#define LI_RPT					LI_NULL
/*-------------------------------------*/
//#define LI_FILE					LI_NULL
//#define LI_FOLDER				LI_NULL
//#define LI_MUSIC				LI_NULL
//#define LI_ALBUM				LI_NULL
//#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					L2I(3,49)
#define LI_AF					L2I(2,49)
#define LI_EON					LI_NULL
#define LI_REG					LI_NULL
#define LI_TA					L2I(1,49)
#define LI_TP					L2I(1,47)
/*-------------------------------------*/	
#define LI_EQ					L2I(1,50)
#define LI_POP					LI_NULL
//#define LI_FLAT				LI_NULL
#define LI_ROCK					LI_NULL
#define LI_CLASS				LI_NULL
//#define LI_JAZZ				LI_NULL
/*-------------------------------------*/
#define LI_VBAR_1				L2I(1,7)
#define LI_VBAR_2				L2I(1,3)
#define LI_VBAR_3				L2I(1,1)
#define LI_VBAR_4				L2I(1,9)
/*-------------------------------------*/
#define LI_SUBWOOFER			L2I(1,15)//P5
//#define LI_ESP45				LI_NULL
#define LI_BLUETOOTH			L2I(4,50)
#define LI_POWER				L2I(1,9)
/*-------------------------------------*/
//#define LI_RADIO				LI_NULL
//#define LI_TRACK				LI_NULL
//#define LI_DISC					LI_NULL
#define LI_USB					L2I(2,50)
#define LI_SD					L2I(1,51)
//#define LI_AUX					LI_NULL
//#define LI_FLASH				LI_NULL

#define LI_MP3					L2I(3,50)
//#define LI_MP4					LI_NULL
//#define LI_MP5					LI_NULL
#define LI_WMA					L2I(4,51)
//#define LI_WAV					LI_NULL
//#define LI_ASF					LI_NULL
//#define LI_CD					LI_NULL
//#define LI_CDC					LI_NULL
//#define LI_VCD					LI_NULL
//#define LI_DVD					LI_NULL
//#define LI_JPG					LI_NULL
/*-------------------------------------*/
#define LI_CLOCK				L2I(1,43)
#define LI_POINT				L2I(1,43)

#define LI_TIME					L2I(1,5)
#define LI_AM					LI_NULL
#define LI_PM					LI_NULL

#define LI_DASH					L2I(1,45)

//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
/*-------------------------------------*/
//#define LI_ST					LI_NULL
#define LI_STO					L2I(4,49) 
//#define LI_LOC					LI_NULL

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//ÏÔÊ¾LCDÅäÖÆ
#define DISP_VBAR_SIZE			(3)


#endif /* _LCDMAP_H_ */

