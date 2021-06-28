#ifndef __DRIV_7PINLED_H__
#define __DRIV_7PINLED_H__
#include "includes.h"
extern u16 disp_data;
extern u16 disp_temp;

#define SET_COM_H(reg, val)			do{	disp_temp = (reg); 						\
								   		disp_temp &= ~0xCE18; 					\
										disp_temp |=(((val&(BIT(7)|BIT(6)))<<8)|((val&(BIT(5)|BIT(4)|BIT(3)))<<6)|((val&(BIT(2)|BIT(1)))<<2));\
										(reg) = disp_temp;						\
									}while(0)
									
#define SET_COM_L(reg, val)			do{	disp_temp = (reg); 						\
										disp_temp &= ~0x0001;					\
										disp_temp |= ((val) & BIT(0));	\
										(reg) = disp_temp;						\
									}while(0)

#define SET_COM(regl, regh, val)	do{ SET_COM_L(regl, val); SET_COM_H(regh, val);}while(0)


#define COM0_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->DIE,JL_PORTA->DIE,disp_data); }while(0)
#define COM1_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->HD,JL_PORTA->HD,disp_data); }while(0)
#define COM2_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->PD,JL_PORTA->PD,disp_data); }while(0)
#define COM3_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->PU,JL_PORTA->PU,disp_data); }while(0)
#define COM4_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->OUT,JL_PORTA->OUT,disp_data); }while(0)


#define _LCD_SUPPORT_LEVEL_						UI_LEVEL_MAX// 3
#define _LCD_LEVEL_LENGTH_						(16)
#define _LCD_FLASH_LENGTH_						(10)

typedef struct{
	uint8	enable;
	uint8 	level;
	uint8	save;
	uint16 	id_map;
}lcd_flash_t;


typedef struct{
	uint8 		disp;
	uint8 		level;
	uint8 		buf[_LCD_SUPPORT_LEVEL_][_LCD_LEVEL_LENGTH_+4];
	uint8 		flash_count;
	uint8 		flash_state;
	lcd_flash_t	flash[_LCD_FLASH_LENGTH_];
}lcd_info_t;

extern lcd_info_t lcd_info;
extern void lcd_hard_init(uint8 open_close);

extern void lcd_polling(void);
#endif

