#ifndef __LC75824_H__
#define __LC75824_H__


#define _LCD_SUPPORT_LEVEL_						UI_LEVEL_MAX
#define _LCD_LEVEL_LENGTH_						(26)
#define _LCD_FLASH_LENGTH_						(10)

typedef struct{
	uint8	enable;
	uint8 	level;
	uint8	save;
	uint16 	id_map;
}lcd_flash_t;

typedef union{
	struct{
		uint8	Cu:1;
		uint8	P:4;//U8	P:3;
		uint8	Dr:1;
		uint8	Sc:1;
		uint8	Bu:1;
	}Bit;
	uint8	All;
}lcd_ctrl;

typedef struct{
	uint8 		disp;
	uint8 		level;
	uint8 		buf[_LCD_SUPPORT_LEVEL_][_LCD_LEVEL_LENGTH_];
	uint8 		flash_count;
	uint8 		flash_state;
	lcd_flash_t	flash[_LCD_FLASH_LENGTH_];
	lcd_ctrl 	ctrl;
}lcd_info_t;

extern lcd_info_t lcd_info;

extern void lcd_hard_init(uint8);
extern void lcd_polling(void);


#endif //__LC75824_H__


