#include "sdk_cfg.h"
#include "setup.h"
#include "disp.h"
#include "lcd/lcd5_8/lcd5_8.h"
#include "includes.h"

lcd_info_t lcd_info;

/*----------------------------------------------------------------------------*/
uint8 disp_buf[7];
uint16 disp_data=0;
uint16 disp_temp=0;


void lcd_polling(void)
{
#if !defined(_SUPPORT_DEBUG_)
#ifdef	TD_ADD_LCD_TEST
	u8 c_byte,c_bit;
	c_byte=gDispPos/8;
	c_bit=gDispPos%8;
	memset(&lcd_info, 0XFF, sizeof(lcd_info));
	if(ghalf_sec)
    {
		lcd_info.buf[0][c_byte]&=(~(1<<c_bit));
	}
    memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[0],5);
	SYS_POW_ON();

#else
	memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[lcd_info.disp],5);
#endif	
	COM0_SEG(disp_buf[0]);
	COM1_SEG(disp_buf[1]);
    COM2_SEG(disp_buf[2]);
    COM3_SEG(disp_buf[3]);
    COM4_SEG(disp_buf[4]);
#endif
}

/*----------------------------------------------------------------------------*/

void lcd_hard_init(uint8 open_close)
{
	memset(&lcd_info, 0, sizeof(lcd_info));
#if !defined(_SUPPORT_DEBUG_)   
	if(open_close)
	{
        JL_CLOCK->CLK_CON1 &= ~(BIT(5)|BIT(4));    //[5:4]:OSC
    #if defined(_HY_LCD_VLCD_)
		JL_LCDC->CON0 = 0x0005<<4;  //[6:4]:VLCDS 3.1 V
	#elif defined(_ENABLE_LCD_VOLTAGE_)
		JL_LCDC->CON0 = 0x0004<<4;	//[6:4]:VLCDS 3.0V
	#elif defined(_DA_LCD_VLCD_)
		JL_LCDC->CON0 = 0x0001<<4;	//[6:4]:VLCDS 2.7V	
	#else
        JL_LCDC->CON0 = 0x0007<<4;  //[6:4]:VLCDS 3.3V
    #endif
        JL_LCDC->CON0 |= BIT(7);    //[7]:32KHZ/64
        JL_LCDC->CON0 |= 0x0001<<12;  //[13:12]:CHGMOD 一直强充电
    #if defined(_CHANGE_LCD_BIAS_)
        JL_LCDC->CON0 |= 0x0002<<2;  //[3:2]:0x01:1/2 Bias。0x02:1/3。0x03:1/4
	#else
        JL_LCDC->CON0 |= 0x0001<<2;  //[3:2]:0x01:1/2 Bias。0x02:1/3。0x03:1/4
    #endif
        //JL_LCDC->CON0 &= ~(BIT(14)|BIT(15));  //6个com  PC3-PC5
        //JL_LCDC->CON0 |=BIT(15);
		JL_LCDC->CON0 |= 0x0002<<14;// 设置COM //5个COM
		
        JL_LCDC->SEG_IOEN0 = 0xCE18;
        JL_LCDC->SEG_IOEN1 = 0x01;
        JL_LCDC->CON0 |= 0x01 ;  //[0] LCDCEN 使能这个模块
	}
	else
	{
		JL_LCDC->CON0 |= 0x00 ; // 关掉内部模块
	}
#endif
}

