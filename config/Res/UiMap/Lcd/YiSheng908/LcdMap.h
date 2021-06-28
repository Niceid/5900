#ifndef _LCDMAP_H_
#define _LCDMAP_H_
#if 1

/*

	
  ---A----		----0----		
|\   |   /|		|\   |   / |		
F H  I  J B		1 3  6  4  2		
|  \ | /  |		|  \ | /   |		
-G-- |--K-		-5- | -7-		
|  / | \  |		|  / | \   |		
E M  I  L C		A 8  6  9  B		
|/   |   \|		|/   |   \ |		
 ----D----		----C----			

*/


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
#define SF7						(LI_G|LI_K)
#define SF8						LI_M
#define SF9						LI_L
#define SFA						LI_E
#define SFB						LI_C
#define SFC						LI_D


enum{
	_LCD_COM1_,
	_LCD_COM2_,
	_LCD_COM3_,
	_LCD_COM4_,
	_END_COM_,
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
#define L1I_SEG(SEG)            ((SEG < 28)?(35-SEG)* _END_COM_:(SEG-25)* _END_COM_)


#define L1I(COM,SEG)			(L1I_SEG(SEG) + L1I_COM(COM))
#define L2I(COM,SEG)			((uint16)(((L1I(COM,SEG)/8)<<8)|(L1I(COM,SEG)%8)))

#define LI_NULL 				0xFFFF
/*--------------------------------------------*/

#define LI_R1					LI_NULL
#define LI_R2					LI_NULL
#define LI_R3					LI_NULL
/*-------------------------------------*/
#define LI_MUTE					LI_NULL
#define LI_LOUD					LI_NULL
/*-------------------------------------*/
#define LI_INT					LI_NULL
#define LI_RDM					LI_NULL
#define LI_RPT					LI_NULL
/*-------------------------------------*/
#define LI_DIR					LI_NULL
#define LI_MUSIC				LI_NULL
#define LI_FILE					LI_NULL
#define LI_TITLE				LI_NULL
#define LI_ALBUM				LI_NULL
#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					LI_NULL
#define LI_AF					LI_NULL
#define LI_TA					LI_NULL
#define LI_TP					LI_NULL
#define LI_EON					LI_NULL	
/*-------------------------------------*/	
#define LI_EQ					LI_NULL
#define LI_POP					LI_NULL
#define LI_FLAT					LI_NULL
#define LI_ROCK					LI_NULL
#define LI_CLASS				LI_NULL
#define LI_JAZZ					LI_NULL
/*-------------------------------------*/
#define LI_L5					LI_NULL
#define LI_L4					LI_NULL
#define LI_L3					LI_NULL
#define LI_L2					LI_NULL
#define LI_L1					LI_NULL
/*-------------------------------------*/
#define LI_SUBWOOFER			LI_NULL
#define LI_ESP45				LI_NULL
#define LI_BLUETOOTH			LI_NULL
/*-------------------------------------*/
#define LI_TRACK				LI_NULL
#define LI_DISC					LI_NULL
#define LI_USB					L2I(3,32)
#define LI_SD					L2I(4,32)
#define LI_KHZ					LI_NULL
#define LI_MHZ					LI_NULL

#define LI_PAUSE				LI_NULL
#define LI_PLAY					LI_NULL

#define LI_MP3					L2I(1,5)
#define LI_MP4					LI_NULL
#define LI_MP5					LI_NULL
#define LI_WMA					L2I(3,5)

#define LI_CD					L2I(2,5)
/*-------------------------------------*/
#define LI_P3					LI_NULL
#define LI_P2					L2I(1,32)
#define LI_P1					L2I(1,30)

#define LI_T1					LI_NULL
/*-------------------------------------*/
#define LI_STO					L2I(2,32) 
#define LI_POWER				LI_NULL




#define LI_CLOCK					LI_P1
//#define LI_CLOCK1					LI_P2
#define LI_POINT					LI_P2
#define LI_ALL_POLINT				LI_P1,LI_P2
#endif
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//ÏÔÊ¾LCDÅäÖÆ
#define DISP_VBAR_SIZE			(4)

#endif /* _LCDMAP_H_ */

