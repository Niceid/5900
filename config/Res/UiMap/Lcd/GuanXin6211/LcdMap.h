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
#define SF6						(LI_I)|(LI_K)
#define SF7						LI_G
#define SF8						LI_M
#define SF9						LI_L
#define SFA						LI_E
#define SFB						LI_C
#define SFC						LI_D


/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
enum{
	_LCD_COM1_,
	_LCD_COM2_,
	_LCD_COM3_,
	_LCD_COM4_,
	_END_COM_,
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

/*---------------------------------------------
VFD??????Segӳ????
---------------------------------------------*/

#define V_P1					1
#define V_P2					2
#define V_P3					3
#define V_P4					4
#define V_P5					5
#define V_P6					6
#define V_P7					7
#define V_P8					8

#define V_P9					32
#define V_P10					31
#define V_P11					30
#define V_P12					29
#define V_P13					28
#define V_P14					27
#define V_P15					26
#define V_P16					25
#define V_P17					24
#define V_P18					23
#define V_P19					22
#define V_P20					21
#define V_P21					20
#define V_P22					19
#define V_P23					18
#define V_P24					17
#define V_P25					16

#define V_P26					9
#define V_P27					10
#define V_P28					11
#define V_P29					12
#define V_P30					13
#define V_P31					14
#define V_P32					15

#define L2I2(x,y)				L2I(x,V_P##y)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LI_R1					L2I2(1,2)
#define LI_R2					L2I2(2,2)
#define LI_R3					L2I2(3,2)
/*-------------------------------------*/
//#define LI_MUTE				LI_NULL
#define LI_LOUD				    L2I2(3,13)
/*-------------------------------------*/
#define LI_INT					L2I2(2,20)
#define LI_RDM					L2I2(1,29)
#define LI_RPT					L2I2(1,20)
/*-------------------------------------*/
//#define LI_FILE					LI_NULL
//#define LI_FOLDER				LI_NULL
//#define LI_MUSIC				LI_NULL
//#define LI_ALBUM				LI_NULL
//#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					L2I2(1,13)
#define LI_AF					L2I2(3,6)
#define LI_EON					LI_NULL
//#define LI_REG					LI_NULL
#define LI_TA					L2I2(1,6)
#define LI_TP					L2I2(2,6)
/*-------------------------------------*/	
#define LI_EQ					LI_NULL
#define LI_POP					LI_NULL
#define LI_FLAT					LI_NULL
#define LI_ROCK					LI_NULL
#define LI_CLASS				LI_NULL
#define LI_JAZZ					LI_NULL
/*-------------------------------------*/
#define LI_VBAR_1				L2I2(3,1)
#define LI_VBAR_2				L2I2(2,1)
#define LI_VBAR_3				L2I2(1,1)
#define LI_VBAR_4				L2I2(1,32)
/*-------------------------------------*/
//#define LI_SUBWOOFER			LI_NULL
//#define LI_ESP45				LI_NULL
#define LI_BLUETOOTH			LI_NULL
#define LI_POWER				L2I2(4,1)
/*-------------------------------------*/
//#define LI_RADIO				LI_NULL
//#define LI_TRACK				LI_NULL
//#define LI_DISC					LI_NULL
#define LI_USB					L2I2(2,32)
#define LI_SD					L2I2(1,31)
//#define LI_AUX					LI_NULL
//#define LI_FLASH				LI_NULL

#define LI_MP3					LI_NULL
//#define LI_MP4					LI_NULL
//#define LI_MP5					LI_NULL
#define LI_WMA					LI_NULL
//#define LI_WAV					LI_NULL
//#define LI_ASF					LI_NULL
//#define LI_CD					LI_NULL
//#define LI_CDC					LI_NULL
//#define LI_VCD					LI_NULL
//#define LI_DVD					LI_NULL
//#define LI_JPG					LI_NULL
/*-------------------------------------*/
#define LI_CLOCK				L2I2(1,24)
#define LI_POINT				L2I2(4,24)

//#define LI_T1					LI_NULL
#define LI_CH					L2I2(3,32)
/*-------------------------------------*/
//#define LI_ST					LI_NULL
#define LI_STO					L2I2(2,13)
//#define LI_LOC					LI_NULL

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//??ʾLCD????
#define DISP_VBAR_SIZE			(4)


#endif /* _LCDMAP_H_ */

