#include "sdk_cfg.h"
#include "setup.h"
#include "disp.h"
#include "lcd/led_7pin/led_7pin.h"
#include "includes.h"

#if defined(DEBGE_LED_AND_LCD_TOGETHER)		
lcd_info_t1 lcd_info1;
#else
lcd_info_t lcd_info;
#endif
/*----------------------------------------------------------------------------*/
uint8 disp_buf[7];

#if !defined(DEBGE_LED_AND_LCD_TOGETHER)		

void lcd_polling(void)
{
	;
}
void lcd_hard_init(uint8 open_close)
{
	;
}
#endif
void led_hard_init(void)
{
#if !defined(_SUPPORT_DEBUG_)
#if defined (DRIVE_LED_7pin_001)||defined(DEBGE_LED_AND_LCD_TOGETHER)

	 /**********PA3  4  9  10   11  14  15**************/
    SEG0_OFF();
    SEG1_OFF();
    SEG2_OFF();
    SEG3_OFF();
    SEG4_OFF();
    SEG5_OFF();
    SEG6_OFF();
  
#else
    /**********PC1-PC5**************/
    JL_PORTC->OUT &= ~(BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));
    JL_PORTC->DIR |=  (BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));
    JL_PORTC->PU  &= ~(BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));
    JL_PORTC->PD  &= ~(BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));
    JL_PORTC->HD  &= ~(BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));

    /**********PA10-PA11**************/
    JL_PORTA->OUT &= ~(BIT(10)|BIT(11));
    JL_PORTA->DIR |=  (BIT(10)|BIT(11));
    JL_PORTA->PU  &= ~(BIT(10)|BIT(11));
    JL_PORTA->PD  &= ~(BIT(10)|BIT(11));
    JL_PORTA->HD  &= ~(BIT(10)|BIT(11));
#endif

#endif
}

void led_polling(void *param)//Ö±Çý7pin  led
{
	SEG0_DISABLE();
	SEG1_DISABLE();
	SEG2_DISABLE();
	SEG3_DISABLE();
	SEG4_DISABLE();
	SEG5_DISABLE();
	SEG6_DISABLE();

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
    memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[0],7);
	SYS_POW_ON();
#elif defined(DEBGE_LED_AND_LCD_TOGETHER)	
	memcpy((u8*)disp_buf,(uint8*)lcd_info1.buf[lcd_info.disp],7);
#else
	memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[lcd_info.disp],7);
#endif	
	//memset(&disp_buf, 0Xff, sizeof(disp_buf));
    switch(cnt)
    {
		case 0:
			SEG0_LOW();
            if(disp_buf[0]&0x01)
            {//0
                SEG1_HIGH();
            }
            if(disp_buf[0]&0x02)
            {// 1
                SEG2_HIGH();
            }
            if(disp_buf[0]&0x04)
            {// 2
                SEG3_HIGH();
            }
            if(disp_buf[0]&0x08)
            {// 3
                SEG4_HIGH();
            }
            if(disp_buf[0]&0x10)
            {// 4
               SEG5_HIGH();
            }
            if(disp_buf[0]&0x20)
            {// 5
                SEG6_HIGH();
            }
			break;
		case 1:
			SEG1_LOW();
            if(disp_buf[1]&0x01)
            {// 8
                SEG0_HIGH();
            }
            if(disp_buf[1]&0x02)
            {// 9
                SEG2_HIGH();
            }
            if(disp_buf[1]&0x04)
            {// 10
                SEG3_HIGH();
            }
            if(disp_buf[1]&0x08)
            {// 11
                SEG4_HIGH();
            }
            if(disp_buf[1]&0x10)
            {// 12
                SEG5_HIGH();
            }
            if(disp_buf[1]&0x20)
            {// 13
                SEG6_HIGH();
            }
        	break;

    	case 2:
			SEG2_LOW();
            if(disp_buf[2]&0x01)
            {//18
                SEG0_HIGH();
            }
            if(disp_buf[2]&0x02)
            {
                SEG1_HIGH();
            }
            if(disp_buf[2]&0x04)
            {
               SEG3_HIGH();
            }
            if(disp_buf[2]&0x08)
            {
                SEG4_HIGH();
            }
            if(disp_buf[2]&0x10)
            {
                SEG5_HIGH();
            }
            if(disp_buf[2]&0x20)
            {
               SEG6_HIGH();
            }
        	break;
    	case 3:
			SEG3_LOW();
            if(disp_buf[3]&0x01)
            {//24
                SEG0_HIGH();
            }
            if(disp_buf[3]&0x02)
            {
               SEG1_HIGH();
            }
            if(disp_buf[3]&0x04)
            {
                SEG2_HIGH();
            }
            if(disp_buf[3]&0x08)
            {
                SEG4_HIGH();
            }
            if(disp_buf[3]&0x10)
            {
               SEG5_HIGH();
            }
            if(disp_buf[3]&0x20)
            {
               SEG6_HIGH();
            }
        	break;
    	case 4:
			SEG4_LOW();
            if(disp_buf[4]&0x01)
            {//32
               SEG0_HIGH();
            }
            if(disp_buf[4]&0x02)
            {
                SEG1_HIGH();
            }
            if(disp_buf[4]&0x04)
            {
                SEG2_HIGH();
            }
            if(disp_buf[4]&0x08)
            {
                SEG3_HIGH();
            }
            if(disp_buf[4]&0x10)
            {
                SEG5_HIGH();
            }
            if(disp_buf[4]&0x20)
            {
                SEG6_HIGH();
            }
        	break;
    	case 5:
			SEG5_LOW();
            if(disp_buf[5]&0x01)
            {//40
               SEG0_HIGH();
            }
            if(disp_buf[5]&0x02)
            {
                SEG1_HIGH();
            }
            if(disp_buf[5]&0x04)
            {
                SEG2_HIGH();
            }
            if(disp_buf[5]&0x08)
            {
               SEG3_HIGH();
            }
            if(disp_buf[5]&0x10)
            {
               SEG4_HIGH();
            }
            if(disp_buf[5]&0x20)
            {
               SEG6_HIGH();
            }
        	break;
    	case 6:
			SEG6_LOW();
            if(disp_buf[6]&0x01)
            {//48
              SEG0_HIGH();
            }
            if(disp_buf[6]&0x02)
            {
                SEG1_HIGH();
            }
            if(disp_buf[6]&0x04)
            {
               SEG2_HIGH();
            }
            if(disp_buf[6]&0x08)
            {
                SEG3_HIGH();
            }
            if(disp_buf[6]&0x10)
            {
                SEG4_HIGH();
            }
            if(disp_buf[6]&0x20)
            {
               SEG5_HIGH();
            }
        	break;
			
    	default :
        	break;
	}
    cnt = (cnt >= 7) ? 0 : cnt+1;
#endif
}


