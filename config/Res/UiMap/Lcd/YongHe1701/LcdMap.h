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
#define SF7						LI_K
#define SF8						LI_M
#define SF9						LI_L
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

#define SEG_25					3
#define SEG_26					4
#define SEG_27					5
#define SEG_28					6
#define SEG_29					7

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
#define LI_POWER				L2I(COM_1,SEG_4)
//#define LI_RADIO				LI_NULL
//#define LI_PLAYER				LI_NULL
//#define LI_AUX					LI_NULL
#define LI_BLUETOOTH			L2I(COM_2,SEG_29)
//#define LI_MODE_USB			LI_NULL
//#define LI_MODE_SD				LI_NULL

//Audio
#define LI_MUTE					LI_NULL
#define LI_LOUD					LI_NULL
#define LI_SUBWOOFER			L2I(COM_3,SEG_29)
//#define LI_EQ					LI_NULL
#define LI_POP					LI_NULL
#define LI_FLAT					LI_NULL
#define LI_ROCK					LI_NULL
#define LI_CLASS				LI_NULL
//#define LI_JAZZ					LI_NULL


#define LI_VBAR_1				LI_NULL
#define LI_VBAR_2				LI_NULL
#define LI_VBAR_3				LI_NULL
#define LI_VBAR_4				LI_NULL
#define LI_VBAR_5               LI_NULL
#define LI_VBAR_6               LI_NULL
#define LI_VBAR_7               LI_NULL

//Radio
//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
//#define LI_ST					LI_NULL
#define LI_STO					LI_NULL
#define LI_LOC					LI_NULL
#define LI_PTY					LI_NULL
#define LI_AF					LI_NULL
#define LI_EON					LI_NULL
//#define LI_REG					LI_NULL
#define LI_TA					LI_NULL
#define LI_TP					LI_NULL

//Player
#define LI_USB					LI_NULL
#define LI_SD					LI_NULL
#define LI_INT					LI_NULL
#define LI_RDM					LI_NULL
#define LI_RPT					LI_NULL

#define LI_R1					LI_NULL
#define LI_R2					LI_NULL
#define LI_R3					LI_NULL

//Common
#define LI_CLOCK				L2I(COM_2,SEG_25)
#define LI_POINT				L2I(COM_1,SEG_29)

#define LI_ZONE					LI_NULL//L2I(COM_1,SEG_4)
#define LI_ZONE1				L2I(COM_1,SEG_11)
#define LI_ZONE2				L2I(COM_1,SEG_18)
#define LI_ZONE3				L2I(COM_1,SEG_25)
#define LI_ZONE4				L2I(COM_4,SEG_29)
//#define LI_Q5					L2I(COM_3,SEG_29)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//????LCD????
#define DISP_VBAR_SIZE			(4)

#endif /* _LCDMAP_H_ */

