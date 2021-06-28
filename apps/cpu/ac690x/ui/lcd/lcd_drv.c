#include "sdk_cfg.h"
#include "timer.h"



#if 1
#include "pt1621\pt1621.c"
#else
#if defined(DRIVE_LED_COM4_SEG8)
	#include "led4_8\led4_8.c"      		//直驱4*8   led
#elif defined(DRIVE_LED_COM5_SEG7)
	#include "led5_7\led5_7.c"      		//直驱5*7   led
#elif defined(DRIVE_LED_7pin_001)
	#include "led_7pin\led_7pin.c"     		//直驱7pin  led
#elif defined(_LCD_DRIVER_A4900B_)
	#include "wp001\wp001.c"      			//直驱6*16  lcd
#elif defined(_LCD_DRIVER_A4901_)
	#include "wp002\wp002.c"      			//直驱6*14  lcd
#elif defined(_LCD_COM5_SEG8)
	#include "wp003\wp003.c"      			//直驱5*8 lcd	4900B
#elif defined(_LCD_LCD_COM5_SEG8)
	#include "lcd5_8\lcd5_8.c"  			//直驱5*8   lcd
#elif defined(_LCD_LCD_COM5_SEG9)
	#include "lcd5_9\lcd5_9.c"  			//直驱5*9   lcd
#elif defined(_VFD_DRIVER_IC_PT1621_)
	#include "pt1621\pt1621.c"
#elif defined(_VFD_DRIVER_IC_AIP31623_)
	#include "aip31623\aip31623.c"
#elif defined(_VFD_DRIVER_IC_YL1622_)
	#include "yl1622\yl1622.c"
#elif defined(_VFD_DRIVER_IC_LC75823_)
	#include "lc75823\lc75823.c"
#elif defined(_VFD_DRIVER_IC_LC75824_)
	#include "lc75824\lc75824.c"
#elif defined(_VFD_DRIVER_IC_TM1628_)
	#include "tm1628\tm1628.c"
#elif defined(_VFD_DRIVER_IC_TM1729_)
	#include "TM1729\TM1729.c"
#endif

#endif

#if defined(_SUPPORT_LCM_1618_DRIVE)
#include "Lcm_1618_drive.h"
#endif
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
#include "led_7pin\led_7pin.c"			//直驱7pin	led
#endif


/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

#define LCD_POLLING_TIME					(50)//	50ms once
#define LCD_FLASH_TIME						(500/LCD_POLLING_TIME)

#if (DISP_VBAR_SIZE != 0)
static uint8 lcd_last_volume = 0;
#endif

#if defined(SUPPORT_TWO_VBVR_DISPLAY_)
#if (DISP_VBAR1_SIZE != 0)
static uint8 lcd_last1_volume = 0;
#endif
#endif

static void lcd_icon_disp_open(uint16);
static void lcd_icon_disp_close(uint16);

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
static void led_put_id_map1(uint8 status, uint8 level, uint16 id_map)
{
	u8 c_byte,c_bit;
	c_byte=id_map/8;
	c_bit=id_map%8;
	if(id_map != LI_NULL)
	{
		if(status)
		{
			lcd_info1.buf[level][c_byte]=lcd_info1.buf[level][c_byte]|(1<<c_bit);
		}
		else
		{
			lcd_info1.buf[level][c_byte]=lcd_info1.buf[level][c_byte]&(~(1<<c_bit));
		}
	}
}
#endif
static void lcd_put_id_map(uint8 status, uint8 level, uint16 id_map)
{
#ifdef	TD_ADD_USER_LCD
	u8 c_byte,c_bit;
	c_byte=id_map/8;
	c_bit=id_map%8;
	if(id_map != LI_NULL)
	{
		if(status)
		{
			lcd_info.buf[level][c_byte]=lcd_info.buf[level][c_byte]|(1<<c_bit);
		}
		else
		{
			lcd_info.buf[level][c_byte]=lcd_info.buf[level][c_byte]&(~(1<<c_bit));
		}
	}
#else
	if(id_map != LI_NULL)
	{
		if(status)
		{
			lcd_info.buf[level][_u16to8h_(id_map)] |=  LCD_ICON_LIST[_u16to8l_(id_map)];
		}
		else
		{
			lcd_info.buf[level][_u16to8h_(id_map)] &= ~LCD_ICON_LIST[_u16to8l_(id_map)];
		}
	}
#endif
}

#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_TEMP_WARM_DISPLAY_)
static void lcd_put_icon(uint8 index,uint8 c)
{
	uint16	seg;
	uint16 	screen;
	uint8 	i;

	if(index > 4)return;

	if(index == 4)
	{
		if(c < 20)
		{
			c %= 10;
			c += '0';
		}
		else
		{
			c = ' ';
		}
	}

	c = get_text_index(c);
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
	seg = SEG13_FONT1[c];
	screen = _B0_;

	for(i = 0; i < 7; i++)
	{
		led_put_id_map1(((seg & screen)?1:0), lcd_info1.level, fp_char_map1[index*7 + i]);

		screen<<=1;
	}
#else
	seg = SEG13_FONT[c];

	screen = _B0_;

	for(i = 0; i < VFD_ICON_MAP_SEG; i++)
	{
		lcd_put_id_map(((seg & screen)?1:0), lcd_info.level, fp_icon_map[index*VFD_ICON_MAP_SEG + i]);

		screen<<=1;
	}
#endif
}
#endif

#if defined(_SUPPORT_TIME_DISPLAY_)
static void lcd_put_icon(uint8 index,uint8 c)
{
	uint16	seg;
	uint16 	screen;
	uint8 	i;

	if(index > 13)return;

	if(index == 13)
	{
		if(c < 20)
		{
			c %= 10;
			c += '0';
		}
		else
		{
			c = ' ';
		}
	}

	c = get_text_index(c);
	seg = SEG13_FONT[c];

	screen = _B0_;

	for(i = 0; i < VFD_ICON_MAP_SEG; i++)
	{
		lcd_put_id_map(((seg & screen)?1:0), lcd_info.level, fp_icon_map[index*VFD_ICON_MAP_SEG + i]);

		screen<<=1;
	}
}
#endif

static void lcd_put_char(uint8 index,uint8 c)
{
	uint16	seg;
	uint16 	screen;

	uint8 	i;

#if defined(_FY7000_SPECIAL_DISPLAY_)
	if(index > 13)return;

	if(index == 13)
	{
		if(c < 10)
		{
			lcd_icon_disp_close(UI_ICON_T1);
		}
		else if(c < 20)
		{
			lcd_icon_disp_open(UI_ICON_T1);
		}

		if(c < 20)
		{
			c %= 10;
			c += '0';
		}
		else
		{
			c = ' ';
		}
	}
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    if(index > 11)return;

    if(index == 11)
    {
    	if(c < 10)
    	{
    		lcd_icon_disp_close(UI_ICON_T1);
    	}
    	else if(c < 20)
    	{
    		lcd_icon_disp_open(UI_ICON_T1);
    	}

    	if(c < 20)
    	{
    		c %= 10;
    		c += '0';
    	}
    	else
    	{
    		c = ' ';
    	}
    }
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SL6812A_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(index > 10)return;

    if(index == 10)
    {
    	if(c < 10)
    	{
    		lcd_icon_disp_close(UI_ICON_T1);
    	}
    	else if(c < 20)
    	{
    		lcd_icon_disp_open(UI_ICON_T1);
    	}

    	if(c < 20)
    	{
    		c %= 10;
    		c += '0';
    	}
    	else
    	{
    		c = ' ';
    	}
    }
#else
	if(index > 8)return;

	if(index == 8)
	{
		if(c < 10)
		{
			lcd_icon_disp_close(UI_ICON_T1);
		}
		else if(c < 20)
		{
			lcd_icon_disp_open(UI_ICON_T1);
		}

		if(c < 20)
		{
			c %= 10;
			c += '0';
		}
		else
		{
			c = ' ';
		}
	}
#endif

#if defined ( _SUPPORT_LCM_1618_DRIVE )
	lcd_info.lcm_disp_now[lcd_info.level][index]= c;
#endif

	c = get_text_index(c);

	seg = SEG13_FONT[c];

	screen = _B0_;

#if defined(_HKY1032_SPECIAL_LOGO_)
	if(global.temp_power_flag)
	{
		for(i = 0; i < VFD_CHAR_MAP_SEG; i++)
		{
			lcd_put_id_map(((seg & screen)?1:0), lcd_info.level, fp_char_map[index*VFD_CHAR_MAP_SEG + i]);
			screen<<=1;
		}
	}
	else
	{
		for(i = 0; i < VFD_CHAR_MAP_SEG; i++)
		{
			lcd_put_id_map(((seg & screen)?1:0), lcd_info.level, fp_char_map1[index*VFD_CHAR_MAP_SEG + i]);
			screen<<=1;
		}
	}
#else
	for(i = 0; i < VFD_CHAR_MAP_SEG; i++)
	{
		lcd_put_id_map(((seg & screen)?1:0), lcd_info.level, fp_char_map[index*VFD_CHAR_MAP_SEG + i]);
		screen<<=1;
	}
#endif
}

static void lcd_irq_polling(void *param)
{
	if((++lcd_info.flash_count) >= LCD_FLASH_TIME)
	{
		uint8 i;
		uint8 level;

		lcd_info.flash_count = 0;
		lcd_info.flash_state = !lcd_info.flash_state;

		for(i = 0; i < _LCD_FLASH_LENGTH_; i++)
		{
			if(lcd_info.flash[i].enable)
			{
				if(lcd_info.flash[i].level == UI_LEVEL_ALL)
				{
					for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
					{
						lcd_put_id_map(lcd_info.flash_state, level, lcd_info.flash[i].id_map);
					}
				}
				else if(lcd_info.flash[i].level < _LCD_SUPPORT_LEVEL_)
				{
					lcd_put_id_map(lcd_info.flash_state, lcd_info.flash[i].level, lcd_info.flash[i].id_map);
				}
			}
		}
	}

	lcd_polling();
}

/*---------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------*/
static uint8 lcd_icon_id_state(uint8 level, uint16 id_map)
{
	if(id_map != LI_NULL)
	{
		if((lcd_info.buf[level][_u16to8h_(id_map)])&(LCD_ICON_LIST[_u16to8l_(id_map)]))
		{
			return 1;
		}
	}
	return 0;
}
static void lcd_icon_id_map(uint8 status,const uint16 *id_map_tab)
{
	uint16 id_map;
	uint8 index;
	uint8 flag;

	while((id_map = *id_map_tab++) != LI_NULL)
	{
		flag = TRUE;

		for(index = 0; index < _LCD_FLASH_LENGTH_; index++)
		{
			if(lcd_info.flash[index].enable)
			{
				if(id_map == lcd_info.flash[index].id_map)
				{
					if(status)
					{
						lcd_info.flash[index].save |= (1<<lcd_info.level);
					}
					else
					{
						lcd_info.flash[index].save &= ~(1<<lcd_info.level);
					}
					flag = FALSE;
					break;
				}
			}
		}

		if(flag)
		{
			lcd_put_id_map(status, lcd_info.level, id_map);
		}
	}
}
static void lcd_icon_disp_open(uint16 id)
{
	if(id < UI_ICON_MAX)
	{
	#if defined(_SUPPORT_LCM_1618_DRIVE)
		for(u8 i=0;i<LCM_ICOM_MAX;i++)
		{
			if(id==LCM_ICOM[i][0])
			{
				LCM_ICOM[i][1]=1;
				break;
			}
		}
	#endif

	#if defined(TD_ADD_USER_LCD)
    	{
    		lcd_icon_id_map(1,LCD_ICON_MAP[id]);
    	}
    #else
        {
        	const uint16 *id_map_tab = LCD_ICON_MAP[id];
        	if(id_map_tab != NULL)
        	{
        		lcd_icon_id_map(1, id_map_tab);
        	}
        }
	#endif
	}
}

static void lcd_icon_disp_close(uint16 id)
{
	if(id < UI_ICON_MAX)
	{
	#if defined(_SUPPORT_LCM_1618_DRIVE)
		for(u8 i=0;i<LCM_ICOM_MAX;i++)
		{
			if(id==LCM_ICOM[i][0])
			{
				LCM_ICOM[i][1]=0;
				break;

			}
		}
	#endif

	#if defined(TD_ADD_USER_LCD)
		{
			lcd_icon_id_map(0,LCD_ICON_MAP[id]);
		}
	#else
        {
        	const uint16 *id_map_tab = LCD_ICON_MAP[id];
        	if(id_map_tab != NULL)
        	{
        		lcd_icon_id_map(0, id_map_tab);
        	}
        }
    #endif
	}
}

static void lcd_icon_disp_ctrl(uint16 *buf, uint8 *ctrl)
{
	uint16 disp_icon;
	uint16 index;

	index = 0;
	while(TRUE)
	{
		disp_icon = *buf++;
		if(disp_icon == UI_ICON_NULL)break;
		if(ctrl[index++])
		{
			lcd_icon_disp_open(disp_icon);
		}
		else
		{
			lcd_icon_disp_close(disp_icon);
		}
	}
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static void drv_lcd_open(void)
{
	lcd_hard_init(TRUE);
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(lcd_irq_polling, NULL, 100);
#else
	timer_reg_func(lcd_irq_polling, NULL, 50);
#endif
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
	led_hard_init();
	timer_reg_isr_fun(timer0_hl,1,led_polling,NULL);
#endif
}

static void drv_lcd_close(void)
{
#if defined(DRIVE_LED_7pin_001)||defined(DRIVE_LED_COM4_SEG8)||defined(DRIVE_LED_COM5_SEG7)||defined(DEBGE_LED_AND_LCD_TOGETHER)
	timer_unreg_func(led_polling);
#endif
	timer_unreg_func(lcd_irq_polling);
	lcd_hard_init(FALSE);
}

static void drv_lcd_refur(uint8 level)
{
	if(level < _LCD_SUPPORT_LEVEL_)
	{
		timer_unreg_func(lcd_irq_polling);
		uint8 cur_level = lcd_info.disp;
		lcd_info.disp = level;
		lcd_polling();
		lcd_info.disp = cur_level;
    #if defined(_ENABLE_RADIO_TIME_)
    	timer_reg_func(lcd_irq_polling, NULL, 100);
    #else
		timer_reg_func(lcd_irq_polling, NULL, 50);
    #endif
	}
}


static void drv_lcd_clear(uint8 level)
{
	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			memset(lcd_info.buf[level], 0, _LCD_LEVEL_LENGTH_);
		#if defined(_SUPPORT_LCM_1618_DRIVE)
			memset(lcd_info.lcm_disp_now[level], 0, _SUPPORT_LCM_1618_DRIVE_char_size);
		#endif
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		memset(lcd_info.buf[level], 0, _LCD_LEVEL_LENGTH_);
	#if defined(_SUPPORT_LCM_1618_DRIVE)
		memset(lcd_info.lcm_disp_now[level], 0, _SUPPORT_LCM_1618_DRIVE_char_size);
	#endif
	}
}

static void drv_lcd_icon_open(uint8 level, uint16 icon)
{
	uint8 modifiy = lcd_info.level;

	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			lcd_info.level = level;
			lcd_icon_disp_open(icon);
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info.level = level;
		lcd_icon_disp_open(icon);
	}

	lcd_info.level = modifiy;
}

static void drv_lcd_icon_close(uint8 level, uint16 icon)
{
	uint8 modifiy = lcd_info.level;

	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			lcd_info.level = level;
			lcd_icon_disp_close(icon);
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info.level = level;
		lcd_icon_disp_close(icon);
	}

	lcd_info.level = modifiy;
}

static void drv_lcd_icon_ctrl(uint8 level, uint16 *icon, uint8 *ctrl)
{
	uint8 modifiy = lcd_info.level;

	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			lcd_info.level = level;
			lcd_icon_disp_ctrl(icon, ctrl);
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info.level = level;
		lcd_icon_disp_ctrl(icon, ctrl);
	}

	lcd_info.level = modifiy;
}
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
static void led_icon_id_map(uint8 status,const uint16 *id_map_tab)
{
	uint16 id_map;
	if((id_map = *id_map_tab++) != LI_NULL)
	led_put_id_map1(status, lcd_info1.level, id_map);
}
static void led_icon_disp_open(uint16 id)
{
	if(id < UI_ICON_MAX1)
	{

    	led_icon_id_map(1,LCD_ICON_MAP1[id]);
	}
}
static void led_icon_disp_close(uint16 id)
{
	if(id < UI_ICON_MAX1)
	{

    	led_icon_id_map(0,LCD_ICON_MAP1[id]);
	}
}
static void led_icon_disp_ctrl(uint16 *icon, uint8 *ctrl)
{
	uint16 disp_icon;
	uint16 index;
	index = 0;
	while(TRUE)
	{
		disp_icon = *icon++;
		if(disp_icon == UI_ICON_NULL1)break;
		if(ctrl[index++])
		{
			led_icon_disp_open(disp_icon);
		}
		else
		{
			led_icon_disp_close(disp_icon);
		}
	}
}
static void drv_led_icon_ctrl(uint8 level, uint16 *icon, uint8 *ctrl)
{
	uint8 modifiy = lcd_info1.level;

	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			lcd_info1.level = level;
			led_icon_disp_ctrl(icon, ctrl);
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info1.level = level;
		led_icon_disp_ctrl(icon, ctrl);
	}
	lcd_info1.level = modifiy;
}

#endif
static void drv_lcd_display(uint8 level, uint8 addr, uint8 *display, uint8 length)
{
	uint8 i;
	uint8 modifiy = lcd_info.level;

	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			lcd_info.level = level;
			for(i = 0; i < length; i++)
			{
				lcd_put_char(addr + i, display[i]);
			}
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info.level = level;
		for(i = 0; i < length; i++)
		{
			lcd_put_char(addr + i, display[i]);
		}
	}
	lcd_info.level = modifiy;

}

#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
static void drv_lcd_icon_display(uint8 level, uint8 addr, uint8 *display, uint8 length)
{
	uint8 i;
	uint8 modifiy = lcd_info.level;

	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			lcd_info.level = level;
			for(i = 0; i < length; i++)
			{
				lcd_put_icon(addr + i, display[i]);
			}
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info.level = level;
		for(i = 0; i < length; i++)
		{
			lcd_put_icon(addr + i, display[i]);
		}
	}
	lcd_info.level = modifiy;
}
#endif

#if defined(DEBGE_LED_AND_LCD_TOGETHER)
static void drv_led_icon_display(uint8 level, uint8 addr, uint8 *display, uint8 length)
{
	uint8 i;
	uint8 modifiy = lcd_info1.level;

	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			lcd_info1.level = level;
			for(i = 0; i < length; i++)
			{
				lcd_put_icon(addr + i, display[i]);
			}
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info1.level = level;
		for(i = 0; i < length; i++)
		{
			lcd_put_icon(addr + i, display[i]);
		}
	}
	lcd_info1.level = modifiy;
}
#endif

#if defined(_FY7555_LCD_DISPLAY_)
static void lcd_put_buff(uint8 index,uint8 c)
{
	uint16	seg;
	uint16 	screen;
	uint8 	i;

	if(index > 8)return;

	if(index == 8)
	{
		if(c < 20)
		{
			c %= 10;
			c += '0';
		}
		else
		{
			c = ' ';
		}
	}

	c = get_text_index(c);
	seg = SEG13_FONT[c];

	screen = _B0_;

	for(i = 0; i < VFD_BUFF_MAP_SEG; i++)
	{
		lcd_put_id_map(((seg & screen)?1:0), lcd_info.level, fp_buff_map[index*VFD_BUFF_MAP_SEG + i]);

		screen<<=1;
	}
}
static void drv_lcd_buff_display(uint8 level, uint8 addr, uint8 *display, uint8 length)
{
	uint8 i;
	uint8 modifiy = lcd_info.level;

	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			lcd_info.level = level;
			for(i = 0; i < length; i++)
			{
				lcd_put_buff(addr + i, display[i]);
			}
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info.level = level;
		for(i = 0; i < length; i++)
		{
			lcd_put_buff(addr + i, display[i]);
		}
	}
	lcd_info.level = modifiy;
}
#endif

static void drv_lcd_vbar_disp(uint8 level, uint8 vol)
{
#if (DISP_VBAR_SIZE != 0)
	uint8	i;
	uint16	vbar_tab[DISP_VBAR_SIZE+1];
	uint8	vbar_ctrl[DISP_VBAR_SIZE];

	if(vol > DISP_VBAR_SIZE)
	{
		vol = DISP_VBAR_SIZE;
	}

	if(vol != lcd_last_volume)
	{
		if(vol < lcd_last_volume)
		{
			vol = lcd_last_volume - 1;
		}
		lcd_last_volume = vol;

		for(i = 0; i < DISP_VBAR_SIZE; i++)
		{
			vbar_tab[i]= UI_ICON_VBAR_1+i;
			if(i < vol)
			{
				vbar_ctrl[i] = TRUE;
			}
			else
			{
				vbar_ctrl[i] = FALSE;
			}
		}
    	vbar_tab[i] = UI_ICON_NULL;
    	drv_lcd_icon_ctrl(level, vbar_tab, vbar_ctrl);
	}
#endif

#if defined(SUPPORT_TWO_VBVR_DISPLAY_)
#if (DISP_VBAR1_SIZE != 0)
	uint8	j;
	uint16	vbar_tab1[DISP_VBAR1_SIZE+1];
	uint8	vbar_ctrl1[DISP_VBAR1_SIZE];

	if(vol > DISP_VBAR1_SIZE)
	{
		vol = DISP_VBAR1_SIZE;
	}

	if(vol != lcd_last1_volume)
	{
		if(vol < lcd_last1_volume)
		{
			vol = lcd_last1_volume - 1;
		}
		lcd_last1_volume = vol;

		for(j = 0; j < DISP_VBAR1_SIZE; j++)
		{
			vbar_tab1[j]= UI_ICON_VBAR1_1+j;
			if(j < vol)
			{
				vbar_ctrl1[j] = TRUE;
			}
			else
			{
				vbar_ctrl1[j] = FALSE;
			}
		}
		vbar_tab1[j] = UI_ICON_NULL;
		drv_lcd_icon_ctrl(level, vbar_tab1, vbar_ctrl1);
	}
#endif
#endif
}

static void drv_lcd_vbar_clean(uint8 level)
{
#if (DISP_VBAR_SIZE != 0)
    uint16	vbar_tab[DISP_VBAR_SIZE+1];
	uint8	vbar_ctrl[DISP_VBAR_SIZE];
	uint8	i;

	lcd_last_volume = 0;

	for(i = 0; i < DISP_VBAR_SIZE; i++)
	{
		vbar_tab[i] = UI_ICON_VBAR_1+i;
		vbar_ctrl[i]= FALSE;
	}
	vbar_tab[i] = UI_ICON_NULL;
	drv_lcd_icon_ctrl(level, vbar_tab, vbar_ctrl);
#endif

#if defined(SUPPORT_TWO_VBVR_DISPLAY_)
#if (DISP_VBAR1_SIZE != 0)
    uint16	vbar_tab1[DISP_VBAR1_SIZE+1];
	uint8	vbar_ctrl1[DISP_VBAR1_SIZE];
	uint8	j;

	lcd_last1_volume = 0;

	for(j = 0; j < DISP_VBAR1_SIZE; j++)
	{
		vbar_tab1[j] = UI_ICON_VBAR1_1+j;
		vbar_ctrl1[j]= FALSE;
	}
	vbar_tab1[j] = UI_ICON_NULL;
	drv_lcd_icon_ctrl(level, vbar_tab1, vbar_ctrl1);
#endif
#endif
}

static void drv_lcd_level(uint8 level)
{
	if(level < _LCD_SUPPORT_LEVEL_)
	{
		lcd_info.disp = level;
	}
}

static void drv_lcd_flash_enable(uint8 level, uint16 id)
{
	if(id < UI_ICON_MAX)
	{
	#if defined(_SUPPORT_LCM_1618_DRIVE)
		for(u8 i=0;i<LCM_ICOM_MAX;i++)
		{
			if(id==LCM_ICOM[i][0])
			{
				LCM_ICOM[i][1]=2;
				break;
			}
		}
	#endif
		if((level != UI_LEVEL_ALL)&&(level >= _LCD_SUPPORT_LEVEL_))return;


		const uint16 *id_map_tab = LCD_ICON_MAP[id];
		if(id_map_tab == NULL)return;
		uint16 id_map;
		uint8 flag;
		uint8 index;

		while((id_map = *id_map_tab++) != LI_NULL)
		{
			flag = TRUE;

			for(index = 0; index < _LCD_FLASH_LENGTH_; index++)
			{
				if(lcd_info.flash[index].enable)
				{
					if(id_map == lcd_info.flash[index].id_map)
					{
						lcd_info.flash[index].level = level;
						flag = FALSE;
						break;
					}
				}
			}

				if(!flag)continue;

			for(index = 0; index < _LCD_FLASH_LENGTH_; index++)
			{
				if(lcd_info.flash[index].enable == FALSE)
				{
					break;
				}
			}

			if(index == _LCD_FLASH_LENGTH_)continue;

			lcd_info.flash[index].id_map = id_map;
			lcd_info.flash[index].level  = level;
			lcd_info.flash[index].save	 = 0;

			if(level == UI_LEVEL_ALL)
			{
				for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
				{
					if(lcd_icon_id_state(level, id_map))
					{
						lcd_info.flash[index].save |= (1<<level);
					}
				}
			}
			else
			{
				if(lcd_icon_id_state(level, id_map))
				{
					lcd_info.flash[index].save |= (1<<level);
				}
			}

			lcd_info.flash[index].enable = TRUE;
		}
	}
}

static void drv_lcd_flash_disable(uint16 id)
{
	if(id < UI_ICON_MAX)
	{
	#if defined(_SUPPORT_LCM_1618_DRIVE)
		for(u8 i=0;i<LCM_ICOM_MAX;i++)
		{
			if(id==LCM_ICOM[i][0])
			{
				LCM_ICOM[i][1]=0;
				break;
			}
		}
	#endif
		const uint16 *id_map_tab = LCD_ICON_MAP[id];
		if(id_map_tab == NULL)return;
		uint16 id_map;
		uint8 index;
		uint8 level;

		while((id_map = *id_map_tab++) != LI_NULL)
		{
			for(index = 0; index < _LCD_FLASH_LENGTH_; index++)
			{
				if(lcd_info.flash[index].enable)
				{
					if(id_map == lcd_info.flash[index].id_map)
					{
						lcd_info.flash[index].enable = FALSE;

						if(lcd_info.flash[index].level == UI_LEVEL_ALL)
						{
							for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
							{
								lcd_put_id_map(lcd_info.flash[index].save&(1<<level) , level, lcd_info.flash[index].id_map);
							}
						}
						else
						{
							level = lcd_info.flash[index].level;
							lcd_put_id_map(lcd_info.flash[level].save&(1<<level) , level, lcd_info.flash[index].id_map);
						}
						break;
					}
				}
			}
		}
	}
}

#if defined(_SL6812_TEST_LCD_)
static void drv_lcd_open_screen(uint8 level)
{
	if(level == UI_LEVEL_ALL)
	{
		for(level = 0; level < _LCD_SUPPORT_LEVEL_; level++)
		{
			memset(lcd_info.buf[level], 0xFF, _LCD_LEVEL_LENGTH_);
		}
	}
	else if(level < _LCD_SUPPORT_LEVEL_)
	{
		memset(lcd_info.buf[level], 0xFF, _LCD_LEVEL_LENGTH_);
	}
}
#endif

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

const ui_drv_fun_t ui_drv_func =
{
	drv_lcd_open,
	drv_lcd_close,
	drv_lcd_refur,
	drv_lcd_clear,
	drv_lcd_icon_open,
	drv_lcd_icon_close,
	drv_lcd_icon_ctrl,
	drv_lcd_display,
#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
	drv_lcd_icon_display,
#endif
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
	drv_led_icon_display,
	drv_led_icon_ctrl,
#endif
#if defined(_FY7555_LCD_DISPLAY_)
	drv_lcd_buff_display,
#endif
	drv_lcd_vbar_disp,
	drv_lcd_vbar_clean,
	drv_lcd_level,
	drv_lcd_flash_enable,
	drv_lcd_flash_disable,
#if defined(_SL6812_TEST_LCD_)
	drv_lcd_open_screen,
#endif
};

void ui_drv_updata(uint8 enable)
{
#if defined(_SUPPORT_LCM_DRIVE)
    Lcm_disp_updata(enable);
#endif
#if !defined(_VFD_DRIVER_IC_TM1729_)
	//drv_lcd_close();
	drv_lcd_open();
#endif
	//drv_lcd_open();
#if defined(DRIVE_LCD_LED)
	if(enable)
	{
		drv_lcd_display(0, 0, (uint8 *)" UPG",4);
	}
	else
	{
		drv_lcd_display(0, 0, (uint8 *)"UPOK",4);
	}
#else
	if(enable)
	{
	#if defined(_FY7000_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"   UPGRADE   ", 13);
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"  UPGRADE  ", 11);
	#elif defined(_SL6812A_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"  UPGRADE ", 10);
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"     UPG  ", 10);
	#elif defined(_FIVE_CHAR_DISPLAY_)
	    drv_lcd_display(0, 0, (uint8 *)" UPG    ", 8);
	#elif defined(_GX6211_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"UPGRADE ", 8);
	#elif defined(_XM003_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)" UPG    ", 8);
	#elif defined(_SEVEN_CHAR_DISPLAY_)
	#if defined(_FY2010_LCD_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"  UPG   ", 8);
	#else
		drv_lcd_display(0, 0, (uint8 *)"UPGREAD ", 8);
	#endif
	#elif defined(_MR1901_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"UPGRADE ", 8);
	#else
    	drv_lcd_display(0, 0, (uint8 *)" UPGRADE", 8);
	#endif
	}
	else
	{
	#if defined(_FY7000_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"   SUCCEED   ", 13);
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"  SUCCEED  ", 11);
	#elif defined(_SL6812A_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"  SUCCEED ", 10);
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"    SUCCED", 10);
	#elif defined(_FIVE_CHAR_DISPLAY_)
	#if defined(_SL_SPECIAL_DISPLAY_)
	    drv_lcd_display(0, 0, (uint8 *)"SUCCESS ", 8);
	#else
    	drv_lcd_display(0, 0, (uint8 *)"SUCCE   ", 8);
	#endif
	#elif defined(TD_ADD_LCD_NUM_4)
		drv_lcd_display(0, 0, (uint8 *)"SUCS    ", 8);
	#elif defined(_LCD_3_DIAPLAY_)
		drv_lcd_display(0, 0, (uint8 *)" SUC    ", 8);
	#elif defined(_GX6211_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"SUCCEED ", 8);
	#elif defined(_XM003_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"SUCCED	", 8);
	#elif defined(_SEVEN_CHAR_DISPLAY_)
	#if defined(_FY2010_LCD_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"SUCED   ", 8);
	#else
		drv_lcd_display(0, 0, (uint8 *)"SUCCEED ", 8);
	#endif
	#elif defined(_MR1901_SPECIAL_DISPLAY_)
		drv_lcd_display(0, 0, (uint8 *)"SUCCEE  ", 8);
	#else
     	drv_lcd_display(0, 0, (uint8 *)" SUCCEED", 8);
	#endif
	}
#endif
	drv_lcd_refur(0);
	drv_lcd_refur(0);
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

