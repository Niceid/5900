#include "lcd/TM1729/TM1729.h"
#include "setup.h"
#include "disp.h"
#include "sdk_cfg.h"

lcd_info_t lcd_info;

#define slave_addr 0x7C //从机地址

unsigned char ICSET = 0xEA; //执行芯片复位命令，内部时钟
unsigned char BLKCTL = 0xF0; //不闪烁
unsigned char DISCTL = 0xA2; //Power save mode FR=Normal mode
//Power save mode SR=Normal mode
//Line翻转模式
unsigned char MODESET = 0xC8; // 显示打开，1/3Bias
unsigned char APCTL8 = 0xFE; //All pixels ON
unsigned char APCTL = 0xFC; //Normal
unsigned char ADSET = 0X00; //起始地址为0x00
unsigned char ack = 0x00; // ack
unsigned char pwron_dly;

void Lcd_Delay(int time)
{
	int i;
	for(i = 0; i < time; i++)
	{
		asm volatile ("nop");
	}
}

#define LCD_DELAY()			Lcd_Delay(20)//30
	
void I2cStart()
{
	LCD_DATA_OUT();
	LCD_CLK_OUT();
	LCD_DELAY();
	LCD_DATA_LOW();		//SDA=0
	LCD_DELAY();
	LCD_CLK_LOW();		//SCL=0
	LCD_DELAY();
}
void I2cStop()
{
	LCD_CLK_LOW();		//SCL=0
	LCD_DELAY();
	LCD_DATA_LOW();		//SDA=0
	LCD_DATA_OUT();
	LCD_DELAY();
	LCD_CLK_HIGH();		//SCL=1
	LCD_DELAY();
	LCD_DATA_HIGH();	//SDA=1
	LCD_DELAY();
}
u8 I2cWrite(uint8 Data)
{
	uint8 Screen;
	
	LCD_CLK_LOW();
	LCD_DELAY();
	LCD_DATA_OUT();
	for(Screen = _B7_; Screen ; Screen >>= 1)
	{
		LCD_CLK_LOW();
		LCD_DELAY();
		if(Data & Screen)
			LCD_DATA_HIGH();
		else
			LCD_DATA_LOW();
		LCD_DELAY();
		LCD_CLK_HIGH();
		LCD_DELAY();
 	}
	LCD_CLK_LOW();
	LCD_DELAY();
	LCD_DATA_IN();
	LCD_DELAY();
	LCD_CLK_HIGH();
	LCD_DELAY();
	if(LCD_DATA_GET())
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

u8 InitTM1729(void)
{
	u8 i=0;

	for(i=0;i<5;i++)
	{
		I2cStop();
		I2cStart();
		if (I2cWrite(slave_addr))return FALSE;
		if (I2cWrite(ICSET))return FALSE; //将TM1729复位
		
		if (I2cWrite(DISCTL))return FALSE; //显示控制的设置，这里的设置都为初始值
		if (I2cWrite(BLKCTL))return FALSE; //不闪烁 
		if (I2cWrite(APCTL))return FALSE; //不启用全部亮或全部暗的功能
		if (I2cWrite(MODESET))return FALSE;
		I2cStop();
		LCD_DELAY();
		LCD_DELAY();
		LCD_DELAY();
		LCD_DELAY();
	}
	return TRUE;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
static void lcd_hard_port_init(uint8 enable)
{
	u8 i=0;
	
	if(enable)
	{
		LCD_DATA_INIT();LCD_DATA_OUT();	LCD_DATA_LOW();
		LCD_CLK_INIT();	LCD_CLK_OUT();	LCD_CLK_LOW();

		for(i=0;i<10;i++) LCD_DELAY();	// 参考规格书 上电注意事项第二条
		LCD_CLK_HIGH();					// 增加STOP 条件
		for(i=0;i<5;i++) LCD_DELAY();
		LCD_DATA_HIGH();
//		LCD_DATA_INIT();LCD_DATA_OUT();	LCD_DATA_HIGH();
//		LCD_CLK_INIT();	LCD_CLK_OUT();	LCD_CLK_HIGH();
	}
	else
	{
		LCD_DATA_INIT();LCD_DATA_OUT();	LCD_DATA_LOW();
		LCD_CLK_INIT();	LCD_CLK_OUT();	LCD_CLK_LOW();
	}
}

void Tm1729LcdOnPwrInit(void)
{
	pwron_dly = 10;
	LCD_DATA_INIT();LCD_DATA_OUT(); LCD_DATA_LOW();
	LCD_CLK_INIT(); LCD_CLK_OUT();	LCD_CLK_LOW();
}

uint8 LcdInitFlag = 0;
void lcd_hard_init(uint8 open_close)
{	
	memset(&lcd_info, 0, sizeof(lcd_info));
	
	if(!open_close)
	{
		lcd_hard_port_init(FALSE);
		lcd_polling();
		LcdInitFlag = 0;
		Tm1729LcdOnPwrInit();
	}
	else
	{
		lcd_hard_port_init(TRUE);
		if (!LcdInitFlag)
		{
			if(InitTM1729())
			{
				lcd_polling();
				LcdInitFlag = 1;
			}
		}
	}
}

void WriteDDRAM(void)
{
	uint8 i;
	uint8 *disp_buf;
	
	lcd_hard_port_init(TRUE);
	disp_buf = lcd_info.buf[lcd_info.disp];

	I2cStart();
	I2cWrite(slave_addr);
	I2cWrite(ADSET);
	for(i=0;i<26;i++)
	{
		I2cWrite(disp_buf[i]);
	}
	I2cStop();
}

/*----------------------------------------------------------------------------*/
void lcd_polling(void)
{
	/*if(pwron_dly)
	{
		pwron_dly--;
		LCD_DATA_INIT();LCD_DATA_OUT(); LCD_DATA_LOW();
		LCD_CLK_INIT(); LCD_CLK_OUT();	LCD_CLK_LOW();
		return;		
	}*/
	
	if (!LcdInitFlag)return;
	WriteDDRAM();
}


