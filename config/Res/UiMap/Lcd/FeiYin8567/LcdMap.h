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
#define LI_N					(0x0001<<13)
#define VFD_CHAR_MAP_SEG		14

#define VFD_ICON_MAP_SEG		7

#define SF0						LI_A
#define SF1						LI_F
#define SF2						LI_B
#define SF3						LI_I
#define SF4						LI_J
#define SF5						LI_G
#define SF6						(LI_H|LI_K)
#define SF7						LI_N
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
	_LCD_COM5_,
	_LCD_COM6_,
	_LCD_COM7_,
	_LCD_COM8_,
	_END_COM_,
};

#define L0_COM1					_LCD_COM1_
#define L0_COM2					_LCD_COM2_
#define L0_COM3					_LCD_COM3_
#define L0_COM4					_LCD_COM4_ 
#define L0_COM5					_LCD_COM5_ 
#define L0_COM6					_LCD_COM6_ 
#define L0_COM7					_LCD_COM7_ 
#define L0_COM8					_LCD_COM8_ 


#define L1_COM1(COM)			((COM == 1)? L0_COM1 : 0 )			
#define L1_COM2(COM)			((COM == 2)? L0_COM2 : L1_COM1(COM)) 
#define L1_COM3(COM)			((COM == 3)? L0_COM3 : L1_COM2(COM)) 
#define L1_COM4(COM)			((COM == 4)? L0_COM4 : L1_COM3(COM)) 
#define L1_COM5(COM)			((COM == 5)? L0_COM5 : L1_COM4(COM)) 
#define L1_COM6(COM)			((COM == 6)? L0_COM6 : L1_COM5(COM)) 
#define L1_COM7(COM)			((COM == 7)? L0_COM7 : L1_COM6(COM)) 
#define L1_COM8(COM)			((COM == 8)? L0_COM8 : L1_COM7(COM)) 


#define L1I_COM(COM)			(L1_COM8(COM))
#define L1I_SEG(SEG)			((SEG - 1) * _END_COM_)


#define L1I(COM,SEG)			(L1I_SEG(SEG) + L1I_COM(COM))
#define L2I(COM,SEG)			((uint16)(((L1I(COM,SEG)/8)<<8)|(L1I(COM,SEG)%8)))

#define LI_NULL 				0xFFFF


/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LI_R1					LI_NULL
#define LI_R2					LI_NULL
#define LI_R3					LI_NULL
/*-------------------------------------*/
#define LI_MUTE					L2I(8,19)
#define LI_LOUD				    L2I(8,30)
/*-------------------------------------*/
#define LI_INT					L2I(8,23)
#define LI_RDM					L2I(8,28)
#define LI_RPT					L2I(8,26)
/*-------------------------------------*/
//#define LI_FILE					LI_NULL
//#define LI_FOLDER				LI_NULL
//#define LI_MUSIC				LI_NULL
//#define LI_ALBUM				LI_NULL
//#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					L2I(8,16)
#define LI_AF					L2I(8,18)
#define LI_EON					LI_NULL
#define LI_REG					LI_NULL
#define LI_TA					L2I(8,20)
#define LI_TP					LI_NULL
/*-------------------------------------*/	
#define LI_EQ					LI_NULL
#define LI_POP					L2I(8,24)
//#define LI_FLAT					LI_NULL
#define LI_ROCK					L2I(8,25)
#define LI_CLASS				L2I(8,27)
//#define LI_JAZZ					LI_NULL
#define LI_FLAT					L2I(8,22)
/*-------------------------------------*/
#define LI_SUBWOOFER			LI_NULL
//#define LI_ESP45				LI_NULL
#define LI_BLUETOOTH			L2I(8,29)
#define LI_POWER				LI_NULL
/*-------------------------------------*/
//#define LI_RADIO				LI_NULL
//#define LI_TRACK				LI_NULL
//#define LI_DISC					LI_NULL
#define LI_USB					L2I(8,17)
#define LI_SD					L2I(8,15)
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
#define LI_CLOCK				L2I(2,32) 
#define LI_POINT				L2I(1,32) 
#define LI_TIME					L2I(8,2)

#define LI_PM					LI_NULL
#define LI_AM					LI_NULL
//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
/*-------------------------------------*/
//#define LI_ST					LI_NULL
#define LI_STO					L2I(8,21)
//#define LI_LOC					LI_NULL

/*-------------------------------------*/
#define LI_VBAR_1				L2I(8,8)
#define LI_VBAR_2				L2I(8,9)
#define LI_VBAR_3				L2I(8,10)
#define LI_VBAR_4				L2I(8,11)
#define LI_VBAR_5				L2I(8,12)
#define LI_VBAR_6				L2I(8,13)
#define LI_VBAR_7				L2I(8,14)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//????LCD????
#define DISP_VBAR_SIZE			(7)

#endif /* _LCDMAP_H_ */

