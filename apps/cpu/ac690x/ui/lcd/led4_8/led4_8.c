#include "sdk_cfg.h"
#include "setup.h"
#include "disp.h"
#include "lcd/led4_8/led4_8.h"
#include "includes.h"

lcd_info_t lcd_info;

/*----------------------------------------------------------------------------*/
uint8 disp_buf[8];

void lcd_polling(void)
{
	;
}
void lcd_hard_init(uint8 open_close)
{
	;
}


#if defined(DRIVE_LED_COM4_SEG8)//--------------------------------4¸öcom£¬8¸öseg---------------------------------------
void led_hard_init(void)
{
#if !defined(_SUPPORT_DEBUG_)
/**********COM:PC1-PC5**************/
    LED_COM0_OFF();
    LED_COM1_OFF();
    LED_COM2_OFF();
    LED_COM3_OFF();

/**********SEG:PA3-PA4***PA9-PA11***PA14-PA15***PC0***********/
    SEG0_OFF();
    SEG1_OFF();
    SEG2_OFF();
    SEG3_OFF();
    SEG4_OFF();
    SEG5_OFF();
    SEG6_OFF();
    SEG7_OFF();
#endif
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
    memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[0],4);
	SYS_POW_ON();

#else
	memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[lcd_info.disp],8);
#endif	
//	memset(&disp_buf, 0XFF, sizeof(disp_buf));
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
            if(disp_buf[0]&0x80)
            {
                SEG7_ON();
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
            if(disp_buf[1]&0x80)
            {
                SEG7_ON();
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
            if(disp_buf[2]&0x80)
            {
                SEG7_ON();
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
            if(disp_buf[3]&0x80)
            {
                SEG7_ON();
            }
        	break;
    	default :
        	break;
	}
    cnt = (cnt >= 3) ? 0 : cnt+1;
#endif
}

#endif

