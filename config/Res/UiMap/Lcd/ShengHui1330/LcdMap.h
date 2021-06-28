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

#define LCD_MODE				1

#if LCD_MODE == 1
	#define L2I(COM,SEG)		((uint16)(((((SEG-1)*3+(COM-1))/8)<<8)|(((SEG-1)*3+(COM-1))%8)))
#elif LCD_MODE == 2
	#define L2I(COM,SEG)		((uint16)(((((SEG-1)*3+(3-COM))/8)<<8)|(((SEG-1)*3+(3-COM))%8)))
#endif


#define LI_NULL 				0xFFFF

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

#define LI_R1					LI_NULL
#define LI_R4					LI_NULL
#define LI_R2					LI_NULL
#define LI_R3					LI_NULL
/*-------------------------------------*/
#define LI_TRACK				LI_NULL
#define LI_DISC					LI_NULL
#define LI_MUTE					L2I(3,10)
#define LI_LOUD					L2I(3,9)//
/*-------------------------------------*/
#define LI_INT					L2I(3,23)
#define LI_RDM					L2I(3,32)
#define LI_RPT					L2I(2,23)
/*-------------------------------------*/
#define LI_DIR					LI_NULL
#define LI_FILE					LI_NULL
#define LI_TITLE				LI_NULL
#define LI_ALBUM				LI_NULL
#define LI_ARTIST				LI_NULL
/*-------------------------------------*/
#define LI_PTY					LI_NULL//
#define LI_AF					L2I(2,32)//
#define LI_EON					LI_NULL
#define LI_REG					LI_NULL
#define LI_TA					L2I(1,23)//
#define LI_TP					LI_NULL//
/*-------------------------------------*/
#define LI_EQ					LI_NULL//
#define LI_POP					LI_NULL
#define LI_FLAT					LI_NULL
#define LI_ROCK					LI_NULL
#define LI_CLASS				LI_NULL
#define LI_JAZZ					LI_NULL
/*-------------------------------------*/
#define LI_VBAR_6				LI_NULL
#define LI_VBAR_5				LI_NULL//
#define LI_VBAR_4				LI_NULL//
#define LI_VBAR_3				LI_NULL//
#define LI_VBAR_2				LI_NULL//
#define LI_VBAR_1				LI_NULL//

/*-------------------------------------*/
#define LI_SUBWOOFER			LI_NULL
#define LI_BLUETOOTH			L2I(1,9)//	
#define LI_ESP45				LI_NULL
#define LI_SCDP					L2I(1,48)//
/*-------------------------------------*/
#define LI_RADIO				L2I(2,10)
#define LI_MHZ				    L2I(2,42)
#define LI_KHZ				    L2I(1,42)
#define LI_AUX					LI_NULL
#define LI_FLASH				LI_NULL
#define LI_USB					L2I(1,10)
#define LI_SD					L2I(2,9)
#define LI_MP3					L2I(3,42)//
#define LI_WAV					LI_NULL
#define LI_ASF					LI_NULL
#define LI_WMA					LI_NULL
#define LI_CD					LI_NULL
#define LI_VCD					LI_NULL
#define LI_DVD					LI_NULL
#define LI_MP4					LI_NULL
/*-------------------------------------*/
#define LI_CLOCK				L2I(3,37)
#define LI_POINT				L2I(1,37)
#define LI_T1					LI_NULL
/*-------------------------------------*/
#define LI_POWER				LI_NULL
#define LI_CH					L2I(3,49)
#define LI_ST					LI_NULL
#define LI_LOC					LI_NULL//
#define LI_STO					L2I(1,32)//
#define LI_MONO					LI_NULL

/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

//ÏÔÊ¾LCDÅäÖÆ
#define DISP_VBAR_SIZE			(0)



#endif /* _LCDMAP_H_ */

