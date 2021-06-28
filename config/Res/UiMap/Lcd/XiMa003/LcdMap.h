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

#define L1I_COM(COM)			((COM - 1) * 16)
#define L1I_SEG(SEG)			(SEG - 1)

#define L1I(COM,SEG)			(L1I_SEG(SEG) + L1I_COM(COM))
#define L2I(COM,SEG)			((uint16)(((L1I(COM,SEG)/8)<<8)|(L1I(COM,SEG)%8)))

#define LI_NULL 				0xFFFF


/*---------------------------------------------
VFD引脚与Seg映射表
---------------------------------------------*/
#define V_P1					16
#define V_P2					15
#define V_P3					14
#define V_P4					13
#define V_P5					12
#define V_P6					11
#define V_P7					10
#define V_P8					9
#define V_P9					8
#define V_P10					7
#define V_P11					6
#define V_P12					5
#define V_P13					4
#define V_P14					3
#define V_P15					2
#define V_P16					1

/*--------------------------------------------*/
/*--------------------------------------------*/
#define L2I2(x,y)				L2I(x,V_P##y)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LI_R1					LI_NULL
#define LI_R2					LI_NULL
#define LI_R3					LI_NULL
/*-------------------------------------*/
//#define LI_MUTE				LI_NULL
#define LI_LOUD					LI_NULL
/*-------------------------------------*/
#define LI_INT					L2I2(2,8)
#define LI_RDM					LI_NULL
#define LI_RPT					L2I2(2,7)
/*-------------------------------------*/
//#define LI_FILE					LI_NULL
//#define LI_FOLDER				LI_NULL
//#define LI_MUSIC				LI_NULL
//#define LI_ALBUM				LI_NULL
//#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					LI_NULL
#define LI_AF					LI_NULL
//#define LI_EON					LI_NULL
//#define LI_REG					LI_NULL
#define LI_TA					LI_NULL
#define LI_TP					LI_NULL
/*-------------------------------------*/	
#define LI_EQ					LI_NULL

#define LI_POP					L2I2(1,9)
#define LI_FLAT					L2I2(1,7)
#define LI_ROCK					L2I2(1,8)
#define LI_CLASS				L2I2(1,11)
//#define LI_JAZZ					LI_NULL
/*-------------------------------------*/
#define LI_VBAR_1				L2I2(2,5)
#define LI_VBAR_2				LI_NULL
#define LI_VBAR_3				L2I2(1,5)
#define LI_VBAR_4				L2I2(1,4)
#define LI_VBAR_5				L2I2(2,4)
/*-------------------------------------*/
//#define LI_SUBWOOFER			LI_NULL
//#define LI_ESP45				LI_NULL
#define LI_BLUETOOTH			L2I2(2,9)
#define LI_POWER				L2I2(1,10)
#define LI_POWER2				L2I2(2,11)
#define LI_ANSWER				L2I2(2,10)

/*-------------------------------------*/
#define LI_RADIO				L2I2(3,16)
//#define LI_TRACK				LI_NULL
//#define LI_DISC					LI_NULL
#define LI_USB					L2I2(2,6)
#define LI_SD					L2I2(1,6)
//#define LI_AUX					LI_NULL
//#define LI_FLASH				LI_NULL

#define LI_MP3					L2I2(6,15)
//#define LI_MP4					LI_NULL
//#define LI_MP5					LI_NULL
#define LI_WMA					L2I2(6,16)
#define LI_WAV					L2I2(5,15)
#define LI_APE					L2I2(3,15)
#define LI_FLAC					L2I2(4,15)
//#define LI_CD					LI_NULL
//#define LI_CDC					LI_NULL
//#define LI_VCD					LI_NULL
//#define LI_DVD					LI_NULL
/*-------------------------------------*/
#define LI_CLOCK				L2I2(1,15)
#define LI_POINT				L2I2(2,16)
#define LI_DASH					LI_NULL

//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
/*-------------------------------------*/
//#define LI_ST					LI_NULL
#define LI_STO					LI_NULL
//#define LI_LOC					LI_NULL


/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//显示LCD配制
#define DISP_VBAR_SIZE			(5)



#endif /* _LCDMAP_H_ */

