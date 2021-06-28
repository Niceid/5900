#include "lcd/lc75823/lc75823.h"

lcd_info_t lcd_info;

void Lcd_Delay(void)
{
	uint8 i;
	
	for(i = 0; i < 5; i++)
	{
		;
	}
}

#define LCD_CCB					0x82
#define LCD_DELAY()				Lcd_Delay()


#define LCD_DR_1_2				0
#define LCD_DR_1_3				0x08
#define LCD_SCR_ON				0
#define LCD_SCR_OFF				0x04
#define LCD_BU_NORMAL			0
#define LCD_BU_SAVING			0x02

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

static void lcd_hard_port_init(uint8 enable)
{
	if(enable)
	{
		LCD_CE_INIT();	LCD_CE_OUT();	LCD_CE_LOW();
		LCD_DATA_INIT();LCD_DATA_OUT();	LCD_DATA_HIGH();
		LCD_CLK_INIT();	LCD_CLK_OUT();	LCD_CLK_HIGH();
	}
	else
	{
		LCD_CE_INIT();	LCD_CE_IN();
		LCD_DATA_INIT();LCD_DATA_IN();
		LCD_CLK_INIT();	LCD_CLK_IN();
	}
}

void LcdWriteByte(uint8 Data)
{
	uint8 Screen;
	for(Screen = _B7_; Screen; Screen >>= 1)
	{
		LCD_CLK_LOW();
		if(Data & Screen) 
			LCD_DATA_HIGH();
		else
			LCD_DATA_LOW();
		LCD_DELAY();
		LCD_CLK_HIGH();
		LCD_DELAY();	
	}
}

/*----------------------------------------------------------------------------*/
void lcd_polling(void)
{
	uint8 i;
	uint8 *disp_buf;
	uint8 ctrl;

	lcd_hard_port_init(TRUE);
	disp_buf = lcd_info.buf[lcd_info.disp];
	
/*---------全部点亮------------*/
	/*uint8 Index;
	for(Index=0;Index<25;Index++)
	{
		disp_buf[Index] = 0xFF;
	}*/
/*---------全部点亮------------*/
		
	LCD_CE_LOW();
	LCD_DELAY();
	LcdWriteByte(LCD_CCB);
	LCD_DELAY();
	LCD_CE_HIGH();

	ctrl = LCD_SCR_ON|LCD_BU_NORMAL|LCD_DR_1_3;
	lcd_info.buf[lcd_info.disp][_LCD_LEVEL_LENGTH_ - 1] &= 0xF0;
	lcd_info.buf[lcd_info.disp][_LCD_LEVEL_LENGTH_ - 1] |= ctrl;

	for(i = 0; i < _LCD_LEVEL_LENGTH_; i++)
	{
		LcdWriteByte(disp_buf[i]);
	}
	LCD_CE_LOW();
}

/*----------------------------------------------------------------------------*/

void lcd_hard_init(uint8 open_close)
{
	uint8 i;
	uint8 ctrl;
	
	memset(&lcd_info, 0, sizeof(lcd_info));
	
	if(open_close)
	{
		ctrl = LCD_SCR_ON|LCD_BU_NORMAL|LCD_DR_1_3;
	}
	else
	{
		ctrl = LCD_SCR_OFF|LCD_BU_SAVING;
	}
	
	for(i = 0; i < _LCD_SUPPORT_LEVEL_; i++)
	{
		lcd_info.buf[i][_LCD_LEVEL_LENGTH_ - 1] &= 0xF0;
		lcd_info.buf[i][_LCD_LEVEL_LENGTH_ - 1] |= ctrl;
	}
	
	lcd_polling();
	
	if(!open_close)
	{
		lcd_hard_port_init(FALSE);
	}
}


