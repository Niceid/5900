#ifndef _LCDMAP_H_
#define _LCDMAP_H_
//enter here

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
//*******************************************************************************************************
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
enum{
	_LCD_COM1_,// 0
	_LCD_COM2_,// 1
	_LCD_COM3_,// 2
	_LCD_COM4_,// 3
	_END_COM_,// 4
};

#define L0_COM1					_LCD_COM1_
#define L0_COM2					_LCD_COM2_
#define L0_COM3					_LCD_COM3_
#define L0_COM4					_LCD_COM4_ 

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
VFD引脚与Seg映射表
---------------------------------------------*/
#define V_P1					32
#define V_P2					31
#define V_P3					30
#define V_P4					29
#define V_P5					28
#define V_P6					27
#define V_P7					26
#define V_P8					25
#define V_P9					24
#define V_P10					23
#define V_P11					22
#define V_P12					21
#define V_P13					20
#define V_P14					19
#define V_P15					18
#define V_P16					17
#define V_P17					16
#define V_P18					15
#define V_P19					14
#define V_P20					13
#define V_P21					12
#define V_P22					11
#define V_P23					10
#define V_P24					9

#define V_P25					1
#define V_P26					2
#define V_P27					3
#define V_P28					4
#define V_P29					5
#define V_P30					6
#define V_P31					7
#define V_P32					8

#define L2I2(x,y)				L2I(x,V_P##y)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LI_R1					L2I2(3,2)
#define LI_R2					L2I2(2,2)
#define LI_R3					L2I2(1,2)
/*-------------------------------------*/
//#define LI_MUTE				LI_NULL
#define LI_LOUD				    LI_NULL 
/*-------------------------------------*/
#define LI_INT					L2I2(4,15)
#define LI_RDM					L2I2(3,15)
#define LI_RPT					L2I2(2,15)
/*-------------------------------------*/
//#define LI_FILE					LI_NULL
//#define LI_FOLDER				LI_NULL
//#define LI_MUSIC				LI_NULL
//#define LI_ALBUM				LI_NULL
//#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					L2I2(4,32)
#define LI_AF					L2I2(2,32)
#define LI_EON					LI_NULL
//#define LI_REG					LI_NULL
#define LI_TA					L2I2(3,32)
#define LI_TP					LI_NULL
/*-------------------------------------*/	
#define LI_EQ					LI_NULL
#define LI_POP					L2I2(3,16)
//#define LI_FLAT					LI_NULL
#define LI_ROCK					L2I2(2,16)
#define LI_CLASS				L2I2(4,16)
//#define LI_JAZZ					LI_NULL
/*-------------------------------------*/
#define LI_VBAR_1				L2I2(4,1)
#define LI_VBAR_2				L2I2(3,1)
#define LI_VBAR_3				L2I2(2,1)
#define LI_VBAR_4				L2I2(1,1)
#define LI_VBAR_5				L2I2(4,2)
/*-------------------------------------*/
//#define LI_SUBWOOFER			LI_NULL
//#define LI_ESP45				LI_NULL
#define LI_BLUETOOTH			L2I2(1,32)
#define LI_POWER				LI_NULL
/*-------------------------------------*/
//#define LI_RADIO				LI_NULL
//#define LI_TRACK				LI_NULL
//#define LI_DISC					LI_NULL
#define LI_USB					L2I2(2,23)
#define LI_SD					L2I2(4,31)
//#define LI_AUX					LI_NULL
//#define LI_FLASH				LI_NULL

#define LI_MP3					LI_NULL
#define LI_MP4					LI_NULL
#define LI_MP5					LI_NULL
#define LI_WMA					LI_NULL
//#define LI_WAV					LI_NULL
//#define LI_ASF					LI_NULL
//#define LI_CD					LI_NULL
//#define LI_CDC					LI_NULL
//#define LI_VCD					LI_NULL
//#define LI_DVD					LI_NULL
//#define LI_JPG					LI_NULL
/*-------------------------------------*/
#define LI_CLOCK				L2I2(4,23)
#define LI_POINT				L2I2(1,23)

//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
/*-------------------------------------*/
//#define LI_ST					LI_NULL
#define LI_STO					L2I2(1,16) 
//#define LI_LOC					LI_NULL

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//显示LCD配制
#define DISP_VBAR_SIZE			(5)

#endif /* _LCDMAP_H_ */

