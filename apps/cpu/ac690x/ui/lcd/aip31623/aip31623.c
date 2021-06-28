#include "sdk_cfg.h"
#include "setup.h"
#include "lcd/aip31623/aip31623.h"
#include "disp.h"

lcd_info_t lcd_info;

void Lcd_Delay(int time)
{
	int i;
	for(i = 0; i < time; i++)
	{
		asm volatile ("nop");
	}
}

#define LCD_DELAY()				Lcd_Delay(5)
#define LCD_DELAY2()			Lcd_Delay(20)

#define LCD_READ_ID				(_B2_ | _B1_)
#define LCD_WRITE_DI			(_B2_ | _B0_)
#define LCD_MODIFY_DI			(_B2_ |	_B0_)
#define LCD_COMAMND_ID			(_B2_)

#define LCD_SYSTEM_DISABLE		(0x00)
#define LCD_SYSTEM_ENABLE		(_B0_)

#define LCD_OFF					(_B1_)
#define LCD_ON					(_B1_ | _B0_)

#define LCD_TIMER_DISABLE		(_B2_)
#define LCD_TIMER_ENABLE		(_B2_ | _B1_)

#define LCD_WDT_DISABLE			(_B2_ | _B0_)
#define LCD_WDT_ENABLE			(_B2_ | _B1_ | _B0_)

#define LCD_TONE_OFF			(_B3_)

#define LCD_CLEAR_TIMER			(_B3_ | _B2_ | _B0_)
#define LCD_CLEAR_WDT			(_B3_ | _B2_ | _B1_ | _B0_)

#define LCD_RC_32K				(_B4_ | _B3_)
#define LCD_XTAL_32K			(_B4_ | _B3_ | _B2_)

#define LCD_TONE_4K				(_B6_)
#define LCD_TONE_2K				(_B6_ | _B5_)

#define LCD_IRQ_DISABLE			(_B7_)
#define LCD_IRQ_ENABLE			(_B7_ | _B3_)

#define LCD_F1 					(_B7_ | _B5_)
#define LCD_F2					(_B7_ | _B5_ | _B0_)
#define LCD_F4					(_B7_ | _B5_ | _B1_)
#define LCD_F8					(_B7_ | _B5_ | _B1_ | _B0_)
#define LCD_F16					(_B7_ | _B5_ | _B2_)
#define LCD_F32					(_B7_ | _B5_ | _B2_ | _B0_)
#define LCD_F64					(_B7_ | _B5_ | _B2_ | _B1_)
#define LCD_F128				(_B7_ | _B5_ | _B2_ | _B1_ | _B0_)

#define LCD_TEST				(_B7_ | _B6_ | _B5_)
#define LCD_NORMAL				(_B7_ | _B6_ | _B5_ | _B1_ | _B0_)


static const uint8 LcdCommandTab[]=
{
	LCD_SYSTEM_ENABLE,
	LCD_ON,
};

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static void lcd_hard_port_init(uint8 enable)
{
	if(enable)
	{
		LCD_CE_INIT();		LCD_CE_OUT();	LCD_CE_HIGH();
		LCD_DATA_INIT();	LCD_DATA_OUT();	LCD_DATA_HIGH();
		LCD_CLK_INIT();		LCD_CLK_OUT();	LCD_CLK_HIGH();
	}
	else
	{
		LCD_CE_INIT();		LCD_CE_IN();
		LCD_CE_INIT();		LCD_DATA_IN();
		LCD_CE_INIT();		LCD_CLK_IN();
	}
}

static void LcdWriteBit(uint8 Data,uint8 Wide)
{
	uint8 Screen;

	for(Screen = (1<<(Wide -1)); Screen; Screen >>= 1)
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

static void LcdWriteByte(uint8 Data)
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
	const uint8 *disp_buf;

	lcd_hard_port_init(TRUE);
	disp_buf = lcd_info.buf[lcd_info.disp];

	LCD_CE_LOW();
	LCD_DELAY();
	LcdWriteBit(LCD_WRITE_DI,3);
	LcdWriteBit(0x00,7);
	LCD_DELAY();
	for(i = 0; i < _LCD_LEVEL_LENGTH_; i++)
	{
		LcdWriteByte(disp_buf[i]);
		//LcdWriteByte(0xFF) ;// È«²¿µãÁÁ
	}
	LCD_CE_HIGH();
	LCD_DELAY();
	LCD_DATA_HIGH();
	LCD_DELAY();

	LCD_DELAY2();

	LCD_CE_LOW();
	LCD_DELAY();
	LcdWriteBit(LCD_COMAMND_ID,3);
	LCD_DELAY();
	for(i = 0; i < sizeof(LcdCommandTab); i++)
	{
		LcdWriteByte(LcdCommandTab[i]);
		LcdWriteBit(0x00,1);
	}
	LCD_CE_HIGH();
	LCD_DELAY();
	LCD_DATA_HIGH();
}

/*----------------------------------------------------------------------------*/

void lcd_hard_init(uint8 open_close)
{
	memset(&lcd_info, 0, sizeof(lcd_info));

	lcd_polling();
	if(!open_close)
	{
		lcd_hard_port_init(FALSE);
	}
}
