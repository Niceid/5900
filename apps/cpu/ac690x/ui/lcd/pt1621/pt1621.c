#include "LcdCtrl.h"
#include "sdk_cfg.h"
#include "setup.h"
#include "lcd/pt1621/pt1621.h"
#include "disp.h"
#include "My_audio/My_audio.h"

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
{0x8,0xb,0xa},//9
{0,0,0}
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
	/*
	LCD_Display_Num(LCD1,2);
	LCD_Display_Num(LCD2,3);
	LCD_Display_Num(LCD3,2);
	LCD_Display_Num(LCD4,3);
	LCD_Display_Num(LCD5,4);
	LCD_Display_Num(LCD6,5);
	LCD_Display_Num(LCD7,6);
	LCD_Display_Num(LCD8,7);
	//Clear_Screen();
	Rand_Clear_Screen();
	*/
	//LCD_FM_Num_Deal(Valid_Fv_Tab[0] / 100);
	LCD_FM_Num_Deal(Valid_Fv_Tab[0]);
	//uchar i;
	//for(i = 0; i < 44; i++)
		//key_printf("%d",Valid_Fv_Tab[i]);
	//key_printf("___________");
	//LCD_FM_Num_Deal(99.9);
}

void lcd_hard_init(uint8 open_close)
{
	lcd_hard_port_init(true);//初始化硬件接口
	/*  驱动    */
	LCD_CE_LOW();
	Lcd_Delay(2);
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

/*写一帧数据*/
void Write_Data(uchar mode,uint address,uchar data)
{
	LCD_CE_LOW();
	Lcd_Delay(2);
	Write_Bytes_Data_2(mode,3);
	Write_Bytes_Data(address,6);
	Write_Bytes_Data_2(data,4);
	LCD_CLK_HIGH();
	LCD_DATA_HIGH();
	LCD_CE_HIGH();
	Lcd_Delay(2);
}

/*写Number个byte*/
void LCD_Display_Num(uchar Lcd_Num ,uchar Number)
{
	uchar i;
	for(i = 0; i < 3; i++)
		Write_Data(Write_Mode,Clear_Add[Lcd_Num][i], Led_Num[Number][i]);
}

/*LCD显示函数*/
void LCD_FM_Num_Deal(float number)
{
	uchar decimal;
	uchar piece;
	uchar ten;
	uchar hundred;

	uchar flag = 0;
	uchar LCD_X = LCD5;
	uint  Number;

	if(number >= 100)
	{
		number *= 10;
		LCD_X--;
		flag = 1;
	}
	else
		number *= 100;

	Number  = number;
	decimal = Number  % 10;
	piece   = (Number % 100) / 10;
	ten     = (Number % 1000) / 100;
	hundred = (Number % 10000) /1000;

	LCD_Display_Num(LCD_X++,hundred);
	LCD_Display_Num(LCD_X++,ten);
	Write_Data(Write_Mode,4*22,8);
	LCD_Display_Num(LCD_X++,piece);
	if(flag)
		LCD_Display_Num(LCD_X,decimal);
}

/*在LCD上显示 FM 87.5 ~ FM 108*/
void LCD_FM_Display(float max)
{
	while(num <= max)
	{
		LCD_FM_Num_Deal(num);
		num += 0.5;
		//Clear_Screen();
	}
	num = 87.5;
	LCD_Display_Num(LCD4,Null);
}

/*窗帘清屏*/
void Clear_Screen(void)
{
	uchar i;
	uchar j;
	for(i = 0; i<10; i++)
		for(j = 0; j < 3; j++)
			Write_Data(Write_Mode,Clear_Add[i][j],0);
	Write_Data(Write_Mode,4*22,0);
}

/*随机清屏*/
void Rand_Clear_Screen(void)
{
	uchar i;
	uchar j;
	uchar temp = 0;
	uchar Flag = 1;
	uchar  rand_x[10] = {0};
	uchar rand_y[3]  = {1,2,0};
	uchar index = 0;
	while(Flag)
	{
		uchar Rand_Num = Random_number(0,9);
		for(i = 0; i <= index; i++)
		{
			if(Rand_Num != rand_x[i])
			{
				temp++;
				if(temp > index)
				{
					rand_x[index] = Rand_Num;
					index++;
					temp = 0;
					if(index == 10)
						Flag = 0;
					break;
				}
			}
		}
	}

	for(i = 0; i < 10; i++)
        for(j = 0; j < 3; j++)
			Write_Data(Write_Mode,Clear_Add[*(rand_x + i)][*(rand_y + j)],0);
}

/*生成start~end随机数*/
uchar Random_number(uchar start, uchar end)
{
	if(end<=start)
	{
		return start;
	}

	//获得一个范围在start到end之间的随机数
	return JL_TIMER0->CNT % (end-start+1) + start;
}

