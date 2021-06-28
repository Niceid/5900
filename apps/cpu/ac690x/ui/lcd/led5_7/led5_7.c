#include "sdk_cfg.h"
#include "setup.h"
#include "disp.h"
#include "lcd/led5_7/led5_7.h"
#include "includes.h"

lcd_info_t lcd_info;

#if 0
void lcd_polling(void)
{
	;
}
void lcd_hard_init(uint8 open_close)
{
	;
}


void led_hard_init(void)
{

	LED_P0_INIT();	LED_P0_OUT();	LED_P0_LOW();
	LED_P1_INIT();	LED_P1_OUT();	LED_P1_LOW();
	LED_P2_INIT();	LED_P2_OUT();	LED_P2_LOW();
	LED_P3_INIT();	LED_P3_OUT();	LED_P3_LOW();
	LED_P4_INIT();	LED_P4_OUT();	LED_P4_LOW();

	LED_P5_INIT();	LED_P5_OUT();	LED_P5_HIGH();
	LED_P6_INIT();	LED_P6_OUT();	LED_P6_HIGH();
	LED_P7_INIT();	LED_P7_OUT();	LED_P7_HIGH();
	LED_P8_INIT();	LED_P8_OUT();	LED_P8_HIGH();
	LED_P9_INIT();	LED_P9_OUT();	LED_P9_HIGH();
	LED_P10_INIT();	LED_P10_OUT();	LED_P10_HIGH();
	LED_P11_INIT();	LED_P11_OUT();	LED_P11_HIGH();
}

void led_polling(void *param)
{
	led_hard_init();

	uint8 disp_data = lcd_info.buf[lcd_info.disp][lcd_info.pos];

    if(disp_data & _B0_)
    {
    	LED_P5_OUT();	LED_P5_LOW();
    }
    
    if(disp_data & _B1_)
    {
    	LED_P6_OUT();	LED_P6_LOW();
    }
    
    if(disp_data & _B2_)
    {
    	LED_P7_OUT();	LED_P7_LOW();
    }
    
    if(disp_data & _B3_)
    {
    	LED_P8_OUT();	LED_P8_LOW();
    }
    
    if(disp_data & _B4_)
    {
    	LED_P9_OUT();	LED_P9_LOW();
    }
    
    if(disp_data & _B5_)
    {
    	LED_P10_OUT();	LED_P10_LOW();
    }
    
    if(disp_data & _B6_)
    {
    	LED_P11_OUT();	LED_P11_LOW();
    }
    
    switch(lcd_info.pos)
    {
    	case 0: LED_P0_ENABLE();	LED_P0_HIGH();break;
    	case 1: LED_P1_ENABLE();	LED_P1_HIGH();break;
    	case 2: LED_P2_ENABLE();	LED_P2_HIGH();break;
    	case 3: LED_P3_ENABLE();	LED_P3_HIGH();break;
    	case 4: LED_P4_ENABLE();	LED_P4_HIGH();break;
    }
    
    if((++lcd_info.pos) >= _LCD_LEVEL_LENGTH_)
    {
    	lcd_info.pos = 0;
    }
}

#else
/*----------------------------------------------------------------------------*/
uint8 disp_buf[7];

void lcd_polling(void)
{
	;
}
void lcd_hard_init(uint8 open_close)
{
	;
}


#if defined(DRIVE_LED_COM5_SEG7)//--------------------------------5¸öcom£¬7¸öseg---------------------------------------
void led_hard_init(void)
{
    LED_COM0_OFF();
    LED_COM1_OFF();
    LED_COM2_OFF();
    LED_COM3_OFF();
    LED_COM4_OFF();

    SEG0_OFF();
    SEG1_OFF();
    SEG2_OFF();
    SEG3_OFF();
    SEG4_OFF();
    SEG5_OFF();
    SEG6_OFF();
}
void led_polling(void *param)
{
#if !defined(_SUPPORT_DEBUG_)
    static u8 cnt;
    led_hard_init();
#ifdef    TD_ADD_LCD_TEST
	u8 c_byte,c_bit;
	c_byte=gDispPos/8;
	c_bit=gDispPos%8;
	memset(&lcd_info, 0XFF, sizeof(lcd_info));
	if(ghalf_sec)
    {
		lcd_info.buf[0][c_byte]&=(~(1<<c_bit));;
	}
    memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[0],5);
	SYS_POW_ON();
	
#else
	memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[lcd_info.disp],7);
#endif	
    switch(cnt)
    {
		case 0:
            LED_COM0_ON();
            if(disp_buf[0]&0x01)
            {
                SEG0_ON();
            }
            if(disp_buf[0]&0x02)
            {
                SEG1_ON();
            }
            if(disp_buf[0]&0x04)
            {
                SEG2_ON();
            }
            if(disp_buf[0]&0x08)
            {
                SEG3_ON();
            }
            if(disp_buf[0]&0x10)
            {
                SEG4_ON();
            }
            if(disp_buf[0]&0x20)
            {
                SEG5_ON();
            }
            if(disp_buf[0]&0x40)
            {
                SEG6_ON();
            }
			break;
		case 1:
            LED_COM1_ON();
            if(disp_buf[1]&0x01)
            {
                SEG0_ON();
            }
            if(disp_buf[1]&0x02)
            {
                SEG1_ON();
            }
            if(disp_buf[1]&0x04)
            {
                SEG2_ON();
            }
            if(disp_buf[1]&0x08)
            {
                SEG3_ON();
            }
            if(disp_buf[1]&0x10)
            {
                SEG4_ON();
            }
            if(disp_buf[1]&0x20)
            {
                SEG5_ON();
            }
            if(disp_buf[1]&0x40)
            {
                SEG6_ON();
            }
        	break;

    	case 2:
            LED_COM2_ON();
            if(disp_buf[2]&0x01)
            {
                SEG0_ON();
            }
            if(disp_buf[2]&0x02)
            {
                SEG1_ON();
            }
            if(disp_buf[2]&0x04)
            {
                SEG2_ON();
            }
            if(disp_buf[2]&0x08)
            {
                SEG3_ON();
            }
            if(disp_buf[2]&0x10)
            {
                SEG4_ON();
            }
            if(disp_buf[2]&0x20)
            {
                SEG5_ON();
            }
            if(disp_buf[2]&0x40)
            {
                SEG6_ON();
            }
        	break;
    	case 3:
            LED_COM3_ON();
            if(disp_buf[3]&0x01)
            {
                SEG0_ON();
            }
            if(disp_buf[3]&0x02)
            {
                SEG1_ON();
            }
            if(disp_buf[3]&0x04)
            {
                SEG2_ON();
            }
            if(disp_buf[3]&0x08)
            {
                SEG3_ON();
            }
            if(disp_buf[3]&0x10)
            {
                SEG4_ON();
            }
            if(disp_buf[3]&0x20)
            {
                SEG5_ON();
            }
            if(disp_buf[3]&0x40)
            {
                SEG6_ON();
            }
        	break;
		case 4:
            LED_COM4_ON();
            if(disp_buf[4]&0x01)
            {
                SEG0_ON();
            }
            if(disp_buf[4]&0x02)
            {
                SEG1_ON();
            }
            if(disp_buf[4]&0x04)
            {
                SEG2_ON();
            }
            if(disp_buf[4]&0x08)
            {
                SEG3_ON();
            }
            if(disp_buf[4]&0x10)
            {
                SEG4_ON();
            }
            if(disp_buf[4]&0x20)
            {
                SEG5_ON();
            }
            if(disp_buf[4]&0x40)
            {
                SEG6_ON();
            }
			break;
    	default :
        	break;
	}
    cnt = (cnt >= 4) ? 0 : cnt+1;
#endif
}

#endif
#endif

