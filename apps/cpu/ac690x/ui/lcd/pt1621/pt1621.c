#include "LcdCtrl.h"
#include "sdk_cfg.h"
#include "setup.h"
#include "lcd/pt1621/pt1621.h"
#include "disp.h"

#include "key_drv.h"
#if defined(_SUPPORT_LCM_1618_DRIVE)
#include "Lcm_1618_drive.h"
#include "lcd/pt1621/pt1621.h"
#endif

lcd_info_t lcd_info;

/*定义LCD*/
enum LCD_NUMBER{LCD1 = 0,LCD2,LCD3,LCD4,LCD5,LCD6,LCD7,LCD8};

/*数字0~9码型*/
uchar Led_Num[][3] = {
{0xa,0x9,0xa},//0
{0x0,0x0,0xa},//1
{0x2,0xb,0x8},//2
{0x0,0xb,0xa},//3 Led_Num
{0x8,0x2,0xa},//4
{0x8,0xb,0x2},//5
{0xa,0xb,0x2},//6
{0x0,0x8,0xa},//7
{0xa,0xb,0xa},//8
{0x8,0xa,0xa},//9
};

/*LCD 0~9地址*/
uint Clear_Add[][3] = {
{0x08,0x0c,0x10},
{0x14,0x18,0x1c},
{0x20,0x24,0x28},
{0x2c,0x30,0x34},

{0x40,0x44,0x48,},
{0x4c,0x50,0x54},

{0x5c,0x60,0x64},
{0x68,0x6c,0x70},
};

/*硬件接口定义*/
static void lcd_hard_port_init(uint8 enable)
{
	key_printf("lcd_hard_port_init\n");
	if(1)
	{
		LCD_CE_INIT();		LCD_CE_OUT();	LCD_CE_HIGH();
		LCD_DATA_INIT();	LCD_DATA_OUT();	LCD_DATA_HIGH();
		LCD_CLK_INIT();		LCD_CLK_OUT();	LCD_CLK_HIGH();
	}
	else
	{
		Ht1621_Close();
		LCD_CE_INIT();		LCD_CE_IN();
		LCD_CE_INIT();		LCD_DATA_IN();
		LCD_CE_INIT();		LCD_CLK_IN();
	}
}

/*任务实现接口*/
void lcd_polling(void)
{
	LCD_Display_Num(LCD1,2);
	LCD_Display_Num(LCD2,3);
	LCD_Display_Num(LCD3,2);
	LCD_Display_Num(LCD4,3);
	LCD_Display_Num(LCD5,4);
	LCD_Display_Num(LCD6,5);
	LCD_Display_Num(LCD7,6);
	LCD_Display_Num(LCD8,7);
	Rand_Clear_Screen();
}

void lcd_hard_init(uint8 open_close)
{
	lcd_hard_port_init(true);//初始化硬件接口
	/*  驱动    */
	LCD_CE_LOW();
	Lcd_Delay(10);
	Write_Bytes_Data_2(Order_Mode,3);
	Write_Bytes_Data(SYS_EN,8);
	Write_Bytes_Data_2(0,1);
	Write_Bytes_Data(TurnOn_Bias,8);
	Write_Bytes_Data_2(0,1);
	Write_Bytes_Data(BiasACom_Cfg_2,8);
	Write_Bytes_Data_2(0,1);
	Clear_Screen();
	LCD_CE_HIGH();
	memset(&lcd_info, 0, sizeof(lcd_info));
}

void Write_Data(uchar mode,uint address,uchar data)
{
	LCD_CE_LOW();
	Lcd_Delay(10);
	Write_Bytes_Data_2(mode,3);
	Write_Bytes_Data(address,6);
	Write_Bytes_Data_2(data,4);
	LCD_CLK_HIGH();
	LCD_DATA_HIGH();
	LCD_CE_HIGH();
	Lcd_Delay(20);
}

void LCD_Display_Num(uchar Lcd_Num ,uchar Number)
{
	uchar i;
	for(i = 0; i < 3; i++)
		Write_Data(Write_Mode,Clear_Add[Lcd_Num][i], Led_Num[Number][i]);
}

void Clear_Screen(void)
{
	uchar i;
	uchar j;
	for(i = 0; i<10; i++)
		for(j = 0; j < 3; j++)
			Write_Data(Write_Mode,Clear_Add[i][j],0);
}

void Rand_Clear_Screen(void)
{
	uchar rand_x[10] = {5,4,1,2,3,6,8,9,0,7};
	uchar rand_y[3] = {2,1,3};
	uchar i;
	uchar j;
	for(i = 0; i < 10; i++)
		for(j = 0; j < 3; j++)
			Write_Data(Write_Mode,Clear_Add[rand_x[i]][rand_y[j]],0);
}

