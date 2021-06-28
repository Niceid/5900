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


#define L2I(COM,SEG)         	(((COM)<<5)|(SEG))

#define LI_NULL 				0xFF

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/


//System
//#define LI_POWER				LI_NULL
#define LI_RADIO				LI_NULL
#define LI_BLUETOOTH			LI_NULL

//Audio
//#define LI_MUTE				LI_NULL

//Radio
#define LI_ST					LI_NULL
#define LI_STO					LI_NULL
//#define LI_LOC					LI_NULL
#define LI_MHZ					LI_NULL
#define LI_KHZ					LI_NULL

//Player
#define LI_USB					LI_NULL
#define LI_SD					LI_NULL
#define LI_AUX					LI_NULL
//#define LI_INT					LI_NULL
//#define LI_RDM					LI_NULL
//#define LI_RPT					LI_NULL

//Common
#define LI_CLOCK				LI_NULL//L2I(1,7)
#define LI_POINT				LI_NULL//L2I(1,7)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//ÏÔÊ¾LEDÅäÖÆ
#define DISP_VBAR_SIZE			(3)


#endif /* _LEDMAP_H_ */

