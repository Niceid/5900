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

#if defined(_FY7555_LCD_DISPLAY_)
#define VFD_BUFF_MAP_SEG		13
#endif

#define SF0						LI_A
#define SF1						LI_F
#define SF2						LI_B
#define SF3						LI_I
#define SF4						LI_M
#define SF5						LI_G
#define SF6						LI_H
#define SF7						LI_K
#define SF8						LI_L
#define SF9						LI_J
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
//#define LI_MUTE				LI_NULL
#define LI_LOUD				    LI_NULL
/*-------------------------------------*/
#define LI_INT					L2I(7,45)
#define LI_RDM					L2I(8,46)
#define LI_RPT					L2I(7,47)
/*-------------------------------------*/
//#define LI_FILE					LI_NULL
//#define LI_FOLDER				LI_NULL
//#define LI_MUSIC				LI_NULL
//#define LI_ALBUM				LI_NULL
//#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					LI_NULL
#define LI_AF					L2I(8,43)
#define LI_EON					L2I(8,45)
#define LI_REG					LI_NULL
#define LI_TA					L2I(8,42)
#define LI_TP					L2I(8,44)

/*-------------------------------------*/	
#define LI_EQ					LI_NULL
#define LI_POP					LI_NULL
//#define LI_FLAT					LI_NULL
#define LI_ROCK					LI_NULL
#define LI_CLASS				LI_NULL
//#define LI_JAZZ					LI_NULL
/*-------------------------------------*/
#define LI_SUBWOOFER			LI_NULL//L2I(8,47)
//#define LI_ESP45				LI_NULL
#define LI_BLUETOOTH			L2I(7,41)
#define LI_BLUETOOTH2			L2I(8,41)//暂时没用
#define LI_POWER				LI_NULL
/*-------------------------------------*/
//#define LI_RADIO				LI_NULL
//#define LI_TRACK				LI_NULL
//#define LI_DISC					LI_NULL
#define LI_USB					L2I(7,43)
#define LI_SD					L2I(4,7)
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
#define LI_CLOCK				L2I(4,1) 
#define LI_POINT				LI_NULL

//#define LI_T1					LI_NULL
//#define LI_CH					LI_NULL
/*-------------------------------------*/
//#define LI_ST					LI_NULL
#define LI_STO					L2I(8,48)
//#define LI_LOC					LI_NULL

#define LI_AM					L2I(7,39)
#define LI_PM					L2I(8,39)
#define LI_TIME					LI_NULL

/*-------------------------------------*/
#define LI_L1P					L2I(8,13)
#define LI_L1Q					L2I(8,12)
#define LI_L1R					L2I(8,11)
#define LI_L1S					L2I(8,10)
#define LI_L1T					L2I(8,9)
#define LI_L1U					L2I(8,8)
#define LI_L1V					L2I(8,7)

#define LI_L2P					L2I(7,13)
#define LI_L2Q					L2I(7,15)
#define LI_L2R					L2I(7,11)
#define LI_L2S					L2I(7,17)
#define LI_L2T					L2I(7,9)
#define LI_L2U					L2I(7,19)
#define LI_L2V					L2I(7,7)

#define LI_L3P					L2I(8,14)
#define LI_L3Q					L2I(8,15)
#define LI_L3R					L2I(8,16)
#define LI_L3S					L2I(8,17)
#define LI_L3T					L2I(8,18)
#define LI_L3U					L2I(8,19)
#define LI_L3V					L2I(8,20)

#define LI_L4P					L2I(8,27)
#define LI_L4Q					L2I(8,26)
#define LI_L4R					L2I(8,25)
#define LI_L4S					L2I(8,24)
#define LI_L4T					L2I(8,23)
#define LI_L4U					L2I(8,22)
#define LI_L4V					L2I(8,21)

#define LI_L5P					L2I(7,27)
#define LI_L5Q					L2I(7,29)
#define LI_L5R					L2I(7,25)
#define LI_L5S					L2I(7,31)
#define LI_L5T					L2I(7,23)
#define LI_L5U					L2I(7,33)
#define LI_L5V					L2I(7,21)

#define LI_L6P					L2I(8,28)
#define LI_L6Q					L2I(8,29)
#define LI_L6R					L2I(8,30)
#define LI_L6S					L2I(8,31)
#define LI_L6T					L2I(8,32)
#define LI_L6U					L2I(8,33)
#define LI_L6V					L2I(8,34)

#define LI_L7P					L2I(7,35)
#define LI_L7Q					L2I(8,35)
#define LI_L7R					L2I(8,36)
#define LI_L7S					L2I(8,37)
#define LI_L7T					L2I(7,37)
#define LI_L7U					L2I(8,38)
#define LI_L7V					L2I(8,6)

#define LI_L8P					L2I(7,6)
#define LI_L8Q					L2I(6,6)
#define LI_L8R					L2I(5,6)
#define LI_L8S					L2I(4,6)
#define LI_L8T					L2I(3,6)
#define LI_L8U					L2I(2,6)
#define LI_L8V					L2I(1,6)

#define LI_L9P					L2I(7,5)
#define LI_L9Q					L2I(6,5)
#define LI_L9R					L2I(5,5)
#define LI_L9S					L2I(4,5)
#define LI_L9T					L2I(3,5)
#define LI_L9U					L2I(2,5)
#define LI_L9V					L2I(1,5)

#define LI_L10P					L2I(7,4)
#define LI_L10Q					L2I(6,4)
#define LI_L10R					L2I(5,4)
#define LI_L10S					L2I(4,4)
#define LI_L10T					L2I(3,4)
#define LI_L10U					L2I(2,4)
#define LI_L10V					L2I(1,4)

#define LI_L11P					L2I(7,3)
#define LI_L11Q					L2I(6,3)
#define LI_L11R					L2I(5,3)
#define LI_L11S					L2I(4,3)
#define LI_L11T					L2I(3,3)
#define LI_L11U					L2I(2,3)
#define LI_L11V					L2I(1,3)

#define LI_L12P					L2I(7,2)
#define LI_L12Q					L2I(6,2)
#define LI_L12R					L2I(5,2)
#define LI_L12S					L2I(4,2)
#define LI_L12T					L2I(3,2)
#define LI_L12U					L2I(2,2)
#define LI_L12V					L2I(1,2)

#define LI_L13P					L2I(8,2)
#define LI_L13Q					L2I(8,3)
#define LI_L13R					L2I(8,4)
#define LI_L13S					L2I(8,5)
#define LI_L13T					L2I(8,1)
#define LI_L13U					L2I(7,1)
#define LI_L13V					L2I(6,1)

/*-------------------------------------*/
#if defined(_FY7555_VBAR_DISPLAY_)
#define LI_VBAR_1				LI_L1V,LI_L2V,LI_L3V,LI_L4V,LI_L5V,LI_L6V,LI_L7V,LI_L8V,LI_L9V,LI_L10V,LI_L11V,LI_L12V,LI_L13V
#define LI_VBAR_2				LI_L1U,LI_L2U,LI_L3U,LI_L4U,LI_L5U,LI_L6U,LI_L7U,LI_L8U,LI_L9U,LI_L10U,LI_L11U,LI_L12U,LI_L13U
#define LI_VBAR_3				LI_L1T,LI_L2T,LI_L3T,LI_L4T,LI_L5T,LI_L6T,LI_L7T,LI_L8T,LI_L9T,LI_L10T,LI_L11T,LI_L12T,LI_L13T
#define LI_VBAR_4				LI_L1S,LI_L2S,LI_L3S,LI_L4S,LI_L5S,LI_L6S,LI_L7S,LI_L8S,LI_L9S,LI_L10S,LI_L11S,LI_L12S,LI_L13S
#define LI_VBAR_5				LI_L1R,LI_L2R,LI_L3R,LI_L4R,LI_L5R,LI_L6R,LI_L7R,LI_L8R,LI_L9R,LI_L10R,LI_L11R,LI_L12R,LI_L13R
#define LI_VBAR_6				LI_L1Q,LI_L2Q,LI_L3Q,LI_L4Q,LI_L5Q,LI_L6Q,LI_L7Q,LI_L8Q,LI_L9Q,LI_L10Q,LI_L11Q,LI_L12Q,LI_L13Q
#define LI_VBAR_7				LI_L1P,LI_L2P,LI_L3P,LI_L4P,LI_L5P,LI_L6P,LI_L7P,LI_L8P,LI_L9P,LI_L10P,LI_L11P,LI_L12P,LI_L13P
#else
#define LI_VBAR_1				LI_NULL
#define LI_VBAR_2				LI_NULL
#define LI_VBAR_3				LI_NULL
#define LI_VBAR_4				LI_NULL
#define LI_VBAR_5				LI_NULL
#define LI_VBAR_6				LI_NULL
#define LI_VBAR_7				LI_NULL
#endif

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//显示LCD配制
#define DISP_VBAR_SIZE			(7)

#define DISP_VBAR1_SIZE			(91)

#endif /* _LCDMAP_H_ */

