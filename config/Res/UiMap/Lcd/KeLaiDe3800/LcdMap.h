#ifndef _LCDMAP_H_
#define _LCDMAP_H_
//enter here

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

#define VFD_ICON_MAP_SEG		7

/*

                    A ----->0		
			B ----->2		
     ---A---	C ----->B    ---0---		
    |\   |  / |	D ----->C   |\   |  /|		
    F H I  J B	E ----->A   1 3  6 4 2		
    |  \ |/   |	F ----->1    |  \ |/  |		
     -G-|-K-	G ----->5    -5-|-7-		
    |  / |\   |	H ----->3   |  /|\    |		
    E M I  L C	I ----->6    A 8  6 9 B		
    |/   |  \ |	J ----->4    |/   |  \ |		
     ---D---	K ----->7    ---C---		
                    L ----->9		
                    M ----->8		

*/

#define SF0						LI_A
#define SF1						LI_F
#define SF2						LI_B
#define SF3						LI_H
#define SF4						LI_J
#define SF5						LI_G
#define SF6					    LI_I
#define SF7						LI_G
#define SF8						LI_L
#define SF9						LI_K
#define SFA						LI_E
#define SFB						LI_C
#define SFC						LI_D

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
enum{
	_LCD_COM1_ = 0,
	_LCD_COM2_,
	_LCD_COM3_,
	_LCD_COM4_,
	_END_COM_
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
#define V_P1					41
#define V_P2					42
#define V_P3					43
#define V_P4					44

#define V_P5					1
#define V_P6					2
#define V_P7					3
#define V_P8					4
#define V_P9					5
#define V_P10					6
#define V_P11					7
#define V_P12					8
#define V_P13					9
#define V_P14					10
#define V_P15					11
#define V_P16					12

#define V_P17					45
#define V_P18					46

#define V_P19					23
#define V_P20					24

#define V_P21					49
#define V_P22					50
#define V_P23					51
#define V_P24					52

#define V_P25					25
#define V_P26					26
#define V_P27					27
#define V_P28					28
#define V_P29					29
#define V_P30					30

#define V_P31					31
#define V_P32					32
#define V_P33					33
#define V_P34					34
#define V_P35					35
#define V_P36					36
#define V_P37					37
#define V_P38					38
#define V_P39					39
#define V_P40					40

#define L2I2(x,y)				L2I(x,V_P##y)

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LI_R1					LI_NULL
#define LI_R2					LI_NULL
#define LI_R3					LI_NULL
#define LI_R4					LI_NULL
/*-------------------------------------*/
#define LI_LOUD					LI_NULL
#define LI_MUTE					LI_NULL
/*-------------------------------------*/
#define LI_INT					LI_NULL
#define LI_RDM					LI_NULL
#define LI_RPT					LI_NULL
/*-------------------------------------*/
#define LI_FILE					LI_NULL
#define LI_FOLDER				LI_NULL
#define LI_MUSIC				LI_NULL
#define LI_ALBUM				LI_NULL
#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					LI_NULL
#define LI_AF					LI_NULL
#define LI_EON					LI_NULL
#define LI_REG					LI_NULL
#define LI_TA					LI_NULL
#define LI_TP					LI_NULL
/*-------------------------------------*/	
#define LI_EQ					LI_NULL
#define LI_POP					LI_NULL
#define LI_ROCK					LI_NULL
#define LI_CLASS				LI_NULL
#define LI_FLAT					LI_NULL
#define LI_JAZZ					LI_NULL
/*-------------------------------------*/
#define LI_VBAR_1				LI_NULL
#define LI_VBAR_2				LI_NULL
#define LI_VBAR_3				LI_NULL
#define LI_VBAR_4				LI_NULL

/*-------------------------------------*/
#define LI_SUBWOOFER			LI_NULL
#define LI_BLUETOOTH			LI_NULL
#define LI_POWER				L2I2(2,40)
/*-------------------------------------*/
#define LI_RADIO				LI_NULL
#define LI_TRACK				LI_NULL
#define LI_DISC					LI_NULL
#define LI_USB					L2I2(3,19)
#define LI_WMA					L2I2(4,40)
#define LI_MP3					L2I2(3,40)

#define LI_SD					L2I2(4,19)
#define LI_AUX					LI_NULL
#define LI_FLASH				LI_NULL

#define LI_PLAYER				LI_NULL

//#define LI_WAV					LI_NULL
//#define LI_ASF					LI_NULL
//#define LI_CD					LI_NULL
//#define LI_CDC					LI_NULL
//#define LI_VCD					LI_NULL
//#define LI_DVD					LI_NULL
//#define LI_JPG					LI_NULL

/*-------------------------------------*/
#define LI_CLOCK				L2I2(1,21)
#define LI_POINT				L2I2(1,21)

#define LI_CLOCK2				LI_NULL

#define LI_TIME				    L2I2(1,19)//时间显示
#define LI_AM					LI_NULL
#define LI_PM					LI_NULL

#define LI_T1					LI_NULL
#define LI_CH					LI_NULL
/*-------------------------------------*/
#define LI_ST					LI_NULL
#define LI_STO					L2I2(2,19)
#define LI_LOC					LI_NULL

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//显示LCD配制
#define DISP_VBAR_SIZE			(0)

#endif /* _LCDMAP_H_ */

