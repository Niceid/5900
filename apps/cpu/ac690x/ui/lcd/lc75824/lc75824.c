#include "sdk_cfg.h"
#include "lcd/lc75824/lc75824.h"

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

void  LcdWriteByte(uint8 Dat)
{
	uint8 i = 8;
	do{
		LCD_CLK_LOW();
		if(Dat&0x80) 
			LCD_DATA_HIGH();
		else
			LCD_DATA_LOW();
		LCD_DELAY();
		LCD_CLK_HIGH();
		LCD_DELAY();
   		Dat<<=1;		
	}while(--i);        
}

void LcdWriteHalfByte(uint8 Dat)
{
	uint8 i = 4;
	do{
		LCD_CLK_LOW();
		if(Dat&0x08) 
			LCD_DATA_HIGH();
		else
			LCD_DATA_LOW();
		LCD_DELAY();
		LCD_CLK_HIGH();
		LCD_DELAY();
   		Dat<<=1;		
	}while(--i);
}

void LcdWriteQuarterByte(uint8 Dat)
{
	uint8 i = 2;
	do{
		LCD_CLK_LOW();
		if(Dat&0x02) 
			LCD_DATA_HIGH();
		else
			LCD_DATA_LOW();
		LCD_DELAY();
		LCD_CLK_HIGH();
		LCD_DELAY();
   		Dat<<=1;		
	}while(--i);
}

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

/*----------------------------------------------------------------------------*/
void lcd_polling(void)
{
	uint8 Byte;
	Byte = 0;
	
	uint8 i;
	uint8 *disp_buf;

	lcd_hard_port_init(TRUE);
	disp_buf = lcd_info.buf[lcd_info.disp];

/*---------全部点亮------------*/
	/*uint8 Index;
	for(Index=0;Index<26;Index++)
	{
		disp_buf[Index] = 0xFF;
	}*/
	
/*---------第一页------------*/
	LCD_CE_LOW();
	LCD_DELAY();
	LcdWriteByte(LCD_CCB);
	LCD_DELAY();
	LCD_CE_HIGH();
	i = 6;
	do{ 
		LcdWriteByte(disp_buf[Byte++]);
	}while(--i);
	LcdWriteHalfByte(disp_buf[Byte]>>4);
	LcdWriteQuarterByte(0x00);
	
	LcdWriteByte(lcd_info.ctrl.All);
	
	LcdWriteQuarterByte(0x00);
	LCD_CE_LOW();
/*---------第二页------------*/
	LCD_CE_LOW();
	LCD_DELAY();
	LcdWriteByte(LCD_CCB);
	LCD_DELAY();
	LCD_CE_HIGH();
	LcdWriteHalfByte(disp_buf[Byte++] & 0x0F);
	i = 6;
	do{ 
		LcdWriteByte(disp_buf[Byte++]);
	}while(--i);
	LcdWriteByte(0x00);
	LcdWriteHalfByte(0x01);
	LCD_CE_LOW();
/*---------第三页------------*/
	LCD_CE_LOW();
	LCD_DELAY();
	LcdWriteByte(LCD_CCB);
	LCD_DELAY();
	LCD_CE_HIGH();
	i = 6;
	do{ 
		LcdWriteByte(disp_buf[Byte++]);
	}while(--i);
	LcdWriteHalfByte(disp_buf[Byte]>>4);
	LcdWriteByte(0x00);
	LcdWriteHalfByte(0x02);
	LCD_CE_LOW();
/*---------第四页------------*/
	LCD_CE_LOW();
	LCD_DELAY();
	LcdWriteByte(LCD_CCB);
	LCD_DELAY();
	LCD_CE_HIGH();
	LcdWriteHalfByte(disp_buf[Byte++] & 0x0F);
	i = 5;
	do{ 
	    LcdWriteByte(disp_buf[Byte++]);
	}while(--i);
	LcdWriteHalfByte(disp_buf[Byte]>>4);
	LcdWriteByte(0x00);
	LcdWriteByte(0x03);
	LCD_CE_LOW();
//-----------------------------
}

/*----------------------------------------------------------------------------*/

void lcd_hard_init(uint8 open_close)
{	
	memset(&lcd_info, 0, sizeof(lcd_info));
	
	/*lcd_info.ctrl.Bit.Cu = 0;
	lcd_info.ctrl.Bit.P  = 0;
	lcd_info.ctrl.Bit.Dr = 0;
	lcd_info.ctrl.Bit.Bu = 0;

	if(open_close)
	{
		lcd_info.ctrl.Bit.Sc = 0;
	}
	else
	{
		lcd_info.ctrl.Bit.Sc = 1;
	}*/
	
	lcd_polling();
	
	if(!open_close)
	{
		lcd_hard_port_init(FALSE);
	}
}

