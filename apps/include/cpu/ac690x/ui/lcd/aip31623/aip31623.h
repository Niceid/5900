#ifndef __AIP31623_H__
#define __AIP31623_H__


#define _LCD_SUPPORT_LEVEL_						UI_LEVEL_MAX// 3
#define _LCD_LEVEL_LENGTH_						(48)
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

extern void lcd_hard_init(uint8);
extern void lcd_polling(void);

#endif //__AIP31623_H__


