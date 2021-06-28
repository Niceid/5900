#include "includes.h"
#include "lp6803/lp6803.h"
#include "setup.h"

#if defined(_COLOR_DRIVER_6803_)
//------------------------------------------------------------------------------------------

static void i2c_6803_delay(void)
{
	uint16 i = 100;
	while(i)
	{
		asm volatile ("nop");
		i--;
	}
}

//------------------------------------------------------------------------------------------
static void LedColorWriteData(uint8 Data)
{
	uint8 Screen;
	
	for (Screen = 0x10; Screen; Screen >>= 1) 
	{ 
		if(Screen & Data)
		{
			LCD_DATA_HIGH(); 
		}
		else
		{
			LCD_DATA_LOW();
		}
		i2c_6803_delay();
		LCD_CE_HIGH();
		i2c_6803_delay();
		LCD_CE_LOW(); 
	} 
}

void Color6803Refresh(uint16 RData, uint16 GData, uint16 BData)
{
	uint8 i;
	
	LCD_CE_LOW();
	LCD_DATA_LOW(); 
	for(i=0;i<32;i++)
	{
		i2c_6803_delay();
		LCD_CE_HIGH();
		i2c_6803_delay();
		LCD_CE_LOW();
	}
	LCD_DATA_HIGH();
	i2c_6803_delay();
	LCD_CE_HIGH();
	i2c_6803_delay();
	LCD_CE_LOW();  

	LedColorWriteData(GData);
	LedColorWriteData(BData);
	LedColorWriteData(RData);

	LCD_DATA_LOW(); 
	i2c_6803_delay();
	LCD_CE_HIGH();
	LCD_DATA_HIGH();
}
#endif

/*----------------------------------------------------------------------------*/

