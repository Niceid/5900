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
#define SF3						LI_H
#define SF4						LI_J
#define SF5						LI_G
#define SF6						LI_I
#define SF7						LI_G
#define SF8						LI_M
#define SF9						LI_L
#define SFA						LI_E
#define SFB						LI_C
#define SFC						LI_D

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define SEG_1					0
#define SEG_2					1
#define SEG_3					2
#define SEG_4					3
#define SEG_5					4
#define SEG_6					5
#define SEG_7					6
#define SEG_8					7
#define SEG_9					8
#define SEG_10					9
#define SEG_11					10
#define SEG_12					11
#define SEG_13					12
#define SEG_14					13
#define SEG_15					14
#define SEG_16					15
#define SEG_17					16
#define SEG_18					17
#define SEG_19					18
#define SEG_20					19
#define SEG_21					20
#define SEG_22					21
#define SEG_23					22
#define SEG_24					23
#define SEG_25					24
#define SEG_26					25
#define SEG_27					26
#define SEG_28					27
#define SEG_29					28
#define SEG_30					29
#define SEG_31					30
#define SEG_32			    	31

#define COM_1					3
#define COM_2					2
#define COM_3					1
#define COM_4					0

#define L1I(COM,SEG)			((SEG)*4 + (COM))
#define L2I(COM,SEG)			((uint16)(((L1I(COM,SEG)/8)<<8)|(L1I(COM,SEG)%8)))

#define LI_NULL 				0xFFFF

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/


//System
#define LI_POWER				L2I(COM_1,SEG_32)
//#define LI_RADIO				LI_NULL
//#define LI_PLAYER				LI_NULL
//#define LI_AUX					LI_NULL
#define LI_BLUETOOTH			L2I(COM_3,SEG_29)
//#define LI_MODE_USB			LI_NULL
//#define LI_MODE_SD				LI_NULL

//Audio
#define LI_MUTE					L2I(COM_1,SEG_29)
#define LI_LOUD					L2I(COM_2,SEG_30)
//#define LI_SUBWOOFER			LI_NULL
//#define LI_EQ					LI_NULL
#define LI_POP					L2I(COM_1,SEG_28)
#define LI_FLAT					L2I(COM_4,SEG_28)
#define LI_ROCK					L2I(COM_2,SEG_28)
#define LI_CLASS				L2I(COM_3,SEG_28)
//#define LI_JAZZ					LI_NULL


#define LI_VBAR_1				L2I(COM_2,SEG_32)
#define LI_VBAR_2				L2I(COM_3,SEG_32)
#define LI_VBAR_3				L2I(COM_4,SEG_32)
#define LI_VBAR_4				L2I(COM_4,SEG_31)
#define LI_VBAR_5               L2I(COM_3,SEG_31)
#define LI_VBAR_6               L2I(COM_2,SEG_31)
#define LI_VBAR_7               L2I(COM_1,SEG_31)

//Radio
//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
//#define LI_ST					LI_NULL
#define LI_STO					L2I(COM_2,SEG_29)
//#define LI_LOC					LI_NULL
#define LI_PTY					L2I(COM_3,SEG_1)
#define LI_AF					L2I(COM_1,SEG_1)
#define LI_EON					L2I(COM_1,SEG_30)
//#define LI_REG					LI_NULL
#define LI_TA					L2I(COM_2,SEG_1)
#define LI_TP					L2I(COM_4,SEG_1)

//Player
#define LI_USB					L2I(COM_4,SEG_2)
#define LI_SD					L2I(COM_4,SEG_29)
#define LI_INT					L2I(COM_2,SEG_2)
#define LI_RDM					L2I(COM_1,SEG_2)
#define LI_RPT					L2I(COM_3,SEG_2)

#define LI_R1					L2I(COM_2,SEG_15)
#define LI_R2					L2I(COM_3,SEG_15)
#define LI_R3					L2I(COM_4,SEG_15)

//Common
#define LI_CLOCK				L2I(COM_4,SEG_30)
#define LI_POINT				L2I(COM_3,SEG_30)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//ÏÔÊ¾LCDÅäÖÆ
#define DISP_VBAR_SIZE			(7)


#endif /* _LCDMAP_H_ */

