#ifndef LCDCTRL_H_
#define LCDCTRL_H_
#include "key_drv.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

#define Order_Mode 0x8 //命令模式
#define Write_Mode 0xa //写模式
#define Null       10

uint Basic_Num = 0x02;
uint Basic_Num_2 = 0x10;
uint Basic_Num_3 = 0x17;

static float num = 87.5;


/*  命令的定义  */
#define BiasACom_Cfg   0x29 //1/3,4Com
#define BiasACom_Cfg_2 0x28 //1/2,4Com
#define TurnOn_Bias    0x03
#define TurnOff_Bias   0x02
#define Normal_Mode    0xe3
#define Text_Mode      0xe0
#define SYS_EN         0x01
#define AddNum         0x30
#define AddNum2		   0x20

void Lcd_Delay(int time)
{
	uchar i;
	for(i = 0; i < time; i++)
        key_printf("sdds");
}
 //写入一定位数的数据
void Write_Bytes_Data(uint Data,uint Number)
{
	uchar i;
	uint Temp = 0x80;
	for(i = 0; i<Number; i++)
	{
		LCD_CLK_LOW();
		Lcd_Delay(1);
		if(Data & Temp)//取出第一位数据
			LCD_DATA_HIGH();
		else
			LCD_DATA_LOW();
		Lcd_Delay(1);
		LCD_CLK_HIGH();
		Lcd_Delay(1);
		Temp  >>=  1;
	}
}

void Write_Bytes_Data_2(uchar Data,uint Number)
{
	uchar i;
	uchar Temp = 0x8;
	for(i = 0; i<Number; i++)
	{
		LCD_CLK_LOW();
		Lcd_Delay(1);
		if(Data & Temp)//取出第一位数据
			LCD_DATA_HIGH();
		else
			LCD_DATA_LOW();
		Lcd_Delay(1);
		LCD_CLK_HIGH();
		Lcd_Delay(1);
		Temp  >>=  1;
	}
}
void Write_Data(uchar ,uint ,uchar ); //写入一帧完整地数据
void LCD_Display_Num(uchar ,uchar );//Lcd显示函数
void Clear_Screen(void);
void Rand_Clear_Screen(void);
uchar Random_number(uchar ,uchar );
void LCD_FM_Num_Deal(float );
void LCD_FM_Display(void);

#endif

