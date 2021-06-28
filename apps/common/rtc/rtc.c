#include "sdk_cfg.h"
#include "dac/dac.h"
#include "setup.h"
#include "sys_detect.h"
#include "ui_res.h"
#include "ui_api.h"
#include "fm_engine.h"
#include "rtc.h"
#include "timer.h"
#include "disp/disp.h"
#include "ui_drv.h"
#include "vm/vm_api.h"

#ifdef SUPPORT_CALL_NAMES
extern uint8 acc_up_flag;
#endif

static rtc_t rtc;
#if defined(_ENABLE_RADIO_RDS_)
static rds_info_t rds_info;
#endif
#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_Clock_Display(uint8 hour,uint8 minute,uint8 state,uint8 select);
#endif
static const uint8 rtc_sel_tab[]=
{
	RTC_DISP_TIME,
	//RTC_DISP_DATE,
#if defined(_ENABLE_RADIO_RDS_)
	RTC_DISP_PSN,
	RTC_DISP_FREQ,
	RTC_DISP_PTY,
#endif
};

static const uint8 rtc_sel_tab1[]=
{
	RTC_DISP_TIME,
	//RTC_DISP_DATE,
#if defined(_ENABLE_RADIO_RDS_)
	//RTC_DISP_PSN,
	//RTC_DISP_FREQ,
	//RTC_DISP_PTY,
#endif
};

/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_RADIO_RDS_)
static uint8 rtc_rds_read(void)
{
	if(setup.rds.enable)
	{
		memset(&rds_info, 0, sizeof(rds_info));

		if(fm_engine_msg(MSG_FMENGINE_RDS_INFO, NULL, &rds_info))
	    {
			return TRUE;
	    }
		rtc_puts("radio_rds_read---->err\n");
	}
	return FALSE;
}

static uint16 rtc_rds_freq(void)
{
	if(rtc_rds_read())
	{
		if((rds_info.freq >= 8750) && (rds_info.freq <= 10800))
		{
			return rds_info.freq;
		}
	}
	return 0;
}

static const uint8 *rtc_rds_psn(void)
{
	if(rtc_rds_read())
	{
		if(rds_info.psn[0])
		{
			return rds_info.psn;
		}
	}
	return NULL;
}

static uint8 rtc_rds_pty(void)
{
	if(rtc_rds_read())
	{
		if(rds_info.pty < 32)
		{
			return rds_info.pty;
		}
	}
	return 0xFF;
}

static void rtc_psn_format(uint8 *disp_buf)
{
	const uint8 *psn = rtc_rds_psn();
#if defined(_FY7000_SPECIAL_DISPLAY_)
    if(psn != NULL)
    {
    	memcpy(disp_buf, psn, 13);
    }
    else
    {
    	memcpy(disp_buf, "   UNKNOWN   ", 13);
    }
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    if(psn != NULL)
    {
    	memcpy(disp_buf, psn, 1);
    }
    else
    {
    	memcpy(disp_buf, "  UNKNOWN  ", 11);
    }
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(psn != NULL)
    {
    	memcpy(disp_buf, psn, 1);
    }
    else
    {
    	memcpy(disp_buf, " UNKNOWN  ", 10);
    }
#elif defined(_XZC1668_SPECIAL_DISPLAY_)
    if(psn != NULL)
    {
    	memcpy(disp_buf, psn, 1);
    }
    else
    {
    	memcpy(disp_buf, "    UNKNOW", 10);
    }
#elif defined(_SH1330_SPECIAL_DISPLAY_)
    if(psn != NULL)
    {
    	memcpy(disp_buf, psn, 1);
    }
    else
    {
    	memcpy(disp_buf, " UNKNOWN  ", 10);
    }	
#elif defined(_FIVE_CHAR_DISPLAY_)
    if(psn != NULL)
    {
    #if defined(_EVB_RDS_PSN_DISPLAY_)
	    memcpy(disp_buf, psn, 5);
	#else
    	memcpy(disp_buf, psn, 8);
	#endif
    }
    else
    {   
    #if defined(_EVB_RDS_PSN_DISPLAY_)
	    memcpy(disp_buf, "UNKNO   ", 8);
	#else
    	memcpy(disp_buf, "UNKNOWN ", 8);
	#endif
    }
#elif defined(_SEVEN_CHAR_DISPLAY_)
    if(psn != NULL)
    {
    	memcpy(disp_buf, psn, 8);
    }
    else
    {
    	memcpy(disp_buf, "UNKNOWN ", 8);
    }
#else
	if(psn != NULL)
	{
		memcpy(disp_buf, psn, 5);
	}
	else
	{
		memcpy(disp_buf, "UNKNO   ", 8);
	}
#endif
}

#if defined(_FIVE_CHAR_DISPLAY_)
static void rtc_freq_format(uint8 *disp_buf)
{
	uint16 freq = rtc_rds_freq();

	if(freq)
	{
		memset(disp_buf, ' ', 8);

		disp_buf[0] = 'F';
		disp_buf[1] = 'M';
		uint16 freq_h = freq/100;
		uint16 freq_l = freq%100;

		disp_buf[2] = freq_h/100 + '0'; freq_h %= 100;
		disp_buf[3] = freq_h/10 + '0';
		disp_buf[4] = freq_h%10 + '0';
		disp_buf[5] = freq_l/10 + '0';
		disp_buf[6] = freq_l%10 + '0';

		if(disp_buf[2] == '0')
		{
			disp_buf[2] = ' ';
		}

		if(disp_buf[6] == '0')
		{
			disp_buf[6] = ' ';
		}
		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
	}
	else
	{
		memcpy(disp_buf, "UNKNO   ", 8);
	}
}

#else
static void rtc_freq_format(uint8 *disp_buf)
{
	uint16 freq = rtc_rds_freq();

	if(freq)
	{
	#if defined(_FY7000_SPECIAL_DISPLAY_)
    	memset(disp_buf, ' ', 13);

    	disp_buf[0] = ' ';
    	disp_buf[1] = 'F';
    	disp_buf[2] = 'M';
    	disp_buf[3] = ' ';
    	disp_buf[4] = ' ';
    	disp_buf[11] = ' ';
    	disp_buf[12] = ' ';

    	uint16 freq_h = freq/100;
    	uint16 freq_l = freq%100;

    	disp_buf[5] = freq_h/100 + '0'; freq_h %= 100;
    	disp_buf[6] = freq_h/10 + '0';
    	disp_buf[7] = freq_h%10 + '0';
    	disp_buf[8] = '.';
    	disp_buf[9] = freq_l/10 + '0';
    	disp_buf[10] = freq_l%10 + '0';

    	if(disp_buf[5] == '0')
    	{
    		disp_buf[5] = ' ';
    	}

    	if(disp_buf[10] == '0')
    	{
    		disp_buf[10] = ' ';
    	}
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
    	memset(disp_buf, ' ', 11);

    	disp_buf[0] = ' ';
    	disp_buf[1] = 'F';
    	disp_buf[2] = 'M';
    	disp_buf[3] = ' ';
    	disp_buf[10] = ' ';

    	uint16 freq_h = freq/100;
    	uint16 freq_l = freq%100;

    	disp_buf[4] = freq_h/100 + '0'; freq_h %= 100;
    	disp_buf[5] = freq_h/10 + '0';
    	disp_buf[6] = freq_h%10 + '0';
    	disp_buf[7] = '.';
    	disp_buf[8] = freq_l/10 + '0';
    	disp_buf[9] = freq_l%10 + '0';

    	if(disp_buf[4] == '0')
    	{
    		disp_buf[4] = ' ';
    	}

    	if(disp_buf[9] == '0')
    	{
    		disp_buf[9] = ' ';
    	}
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SL6812A_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    	memset(disp_buf, ' ', 10);

    	disp_buf[0] = 'F';
    	disp_buf[1] = 'M';
    	disp_buf[2] = ' ';
    	disp_buf[3] = ' ';

    	uint16 freq_h = freq/100;
    	uint16 freq_l = freq%100;

    	disp_buf[4] = freq_h/100 + '0'; freq_h %= 100;
    	disp_buf[5] = freq_h/10 + '0';
    	disp_buf[6] = freq_h%10 + '0';
    	disp_buf[7] = '.';
    	disp_buf[8] = freq_l/10 + '0';
    	disp_buf[9] = freq_l%10 + '0';

    	if(disp_buf[4] == '0')
    	{
    		disp_buf[4] = ' ';
    	}

    	if(disp_buf[9] == '0')
    	{
    		disp_buf[9] = ' ';
    	}
	#else
		memset(disp_buf, ' ', 8);

		disp_buf[0] = 'F';
		disp_buf[1] = 'M';

		uint16 freq_h = freq/100;
		uint16 freq_l = freq%100;

		disp_buf[2] = freq_h/100 + '0'; freq_h %= 100;
		disp_buf[3] = freq_h/10 + '0';
		disp_buf[4] = freq_h%10 + '0';
		disp_buf[5] = '.';
		disp_buf[6] = freq_l/10 + '0';
		disp_buf[7] = freq_l%10 + '0';

		if(disp_buf[2] == '0')
		{
			disp_buf[2] = ' ';
		}

		if(disp_buf[7] == '0')
		{
			disp_buf[7] = ' ';
		}
	#endif
	}
	else
	{
	#if defined(_FY7000_SPECIAL_DISPLAY_)
		memcpy(disp_buf, "   UNKNOWN   ", 13);
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
		memcpy(disp_buf, "  UNKNOWN  ", 11);
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)
		memcpy(disp_buf, "    UNKNOW", 10);
	#elif defined(_SL6812A_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
		memcpy(disp_buf, " UNKNOWN  ", 10);
	#else
		memcpy(disp_buf, "UNKNOWN ", 8);
	#endif
	}
}
#endif

static void rtc_pty_format(uint8 *disp_buf)
{
	uint8 pty = rtc_rds_pty();

	if(setup.rds.type == RDS_DEC_RDS)
	{
		ui_show_copy(LANG_ID_RDS_PTY_0 + pty, disp_buf);

	}
	else
	{
		ui_show_copy(LANG_ID_RBDS_PTY_0 + pty, disp_buf);
	}
}
#endif

static void rtc_time_save(void)
{
	sys_set_rtc(&rtc.set_time);
	rtc.cur_time = rtc.set_time;
}

static void rtc_time_load(void)
{
	sys_get_rtc(&rtc.cur_time);
	rtc.set_time = rtc.cur_time;
}

#if defined(SUPPORT_ALARM_FUNC_)
void get_rtc_time(void)
{
	sys_get_rtc(&rtc.cur_time);

	setup.alarm_second1	= rtc.cur_time.bSec;
	setup.alarm_minute1	= rtc.cur_time.bMin;
	setup.alarm_hour1   = rtc.cur_time.bHour;
}
#endif

static uint8 rtc_month_total_day(void)
{
	switch(rtc.set_time.bMonth)
	{
		case 2:
			if((rtc.set_time.dYear == 1900)||(rtc.set_time.dYear == 2100))
			{
				return 28;
			}
			else
			{
				if(rtc.set_time.dYear%4)
				{
					return 28;
				}
				else
				{
					return 29;
				}
			}
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
	   	case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
	}
	return 0;
}

#if defined(_SUPPORT_LCM_1618_DRIVE)
void lcm_rtc_time_format(uint8 *disp_buf)
{//时间正常显示
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}
	disp_buf[0] = ' ';
	disp_buf[1] = ' ';

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[0] = 'A';
			disp_buf[1] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[0] = 'P';
			disp_buf[1] = 'M';
		}
	}
	disp_buf[2] = hour/10 + '0';
	disp_buf[3] = hour%10 + '0';
	disp_buf[4] =  ':';
	disp_buf[5] = minute/10  + '0';
	disp_buf[6] = minute%10  + '0';
}
#endif


#if defined(_KLD3300_SPECIAL_DISPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

#if defined(_SUPPORT_AREA_)
#if !defined(_KLD3300_SPECIAL_DISPLAY_)
	if(setup.area == SETUP_AREA_USA)
#endif
#endif
	{
	#if !defined(_KLD3000_TIME_24_)
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
		}
	#endif
	}
	disp_buf[0] = hour/10 + '0';
	disp_buf[1] = hour%10 + '0';
	disp_buf[2] = minute/10  + '0';
	disp_buf[3] = minute%10  + '0';
#if defined(SPECIAL_icon_DISP)
	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER);
#endif
}

#elif defined(_FY7000_SPECIAL_DISPLAY_)
#if defined(_FY8567_TIME_DISPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	if(hour <12)
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_AM);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
	}
	else
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_AM);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PM);
	}

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
		}
	}

	disp_buf[0] = hour/10 + '0';
	disp_buf[1] = hour%10 + '0';
	disp_buf[2] = minute/10  + '0';
	disp_buf[3] = minute%10  + '0';
}

#else
static void rtc_time_format(uint8 *disp_buf)
{//时间正常显示
	uint8 hour;
	uint8 minute;

#if defined(_SUPPORT_LCM_1618_DRIVE)
	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}
	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[3] = ' ';
	disp_buf[4] = ' ';
	disp_buf[5] = ' ';
	disp_buf[6] = ' ';
	disp_buf[7] = ' ';
	disp_buf[8] = ' ';
	disp_buf[9] = ' ';
	disp_buf[10] = ' ';
	disp_buf[11] = ' ';
	disp_buf[12] = ' ';
#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[1] = 'A';
			disp_buf[2] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[1] = 'P';
			disp_buf[2] = 'M';
		}
	}
	disp_buf[4] = hour/10 + '0';
	disp_buf[5] = hour%10 + '0';
	disp_buf[6] =  ':';
	disp_buf[7] = minute/10  + '0';
	disp_buf[8] = minute%10  + '0';

    //ew_printf("hour:%d\n",hour);
	//ew_printf("minute:%d\n",minute);
	//ew_printf("sec:%d\n",rtc.cur_time.bSec);
#else

if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

#if defined(_FY7555_CLOCK_ICON_DISPLAY_)
	if(hour <12)
	{
		setup.time_flag = TRUE;
	}
	else
	{
		setup.time_flag = FALSE;
	}
#endif

#if defined(_FY7555_CLOCK_ICON_DISPLAY_)
	if(global.sys_power)
	{
		disp_buf[1] = 'C';
		disp_buf[2] = 'L';
		disp_buf[3] = 'O';
		disp_buf[4] = 'C';
		disp_buf[5] = 'K';
	}
	else
	{
		disp_buf[1] = ' ';
		disp_buf[2] = ' ';
		disp_buf[3] = ' ';
		disp_buf[4] = ' ';
		disp_buf[5] = ' ';
	}
    disp_buf[0] = ' ';
    disp_buf[6] = ' ';
    disp_buf[7] = ' ';
    disp_buf[8] = ' ';
#else
	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[3] = ' ';
	disp_buf[4] = ' ';
	disp_buf[5] = ' ';
	disp_buf[6] = ' ';
	disp_buf[7] = ' ';
	disp_buf[8] = ' ';
#endif

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
		#if !defined(_FY7555_CLOCK_ICON_DISPLAY_)
			disp_buf[5] = 'A';
			disp_buf[6] = 'M';
		#endif
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
		#if !defined(_FY7555_CLOCK_ICON_DISPLAY_)
			disp_buf[5] = 'P';
			disp_buf[6] = 'M';
		#endif
		}
	}
	disp_buf[9] = hour/10 + '0';
	disp_buf[10] = hour%10 + '0';
	disp_buf[11] = minute/10  + '0';
	disp_buf[12] = minute%10  + '0';
#endif
}
#endif

#elif defined(_FIVE_CHAR_DISPLAY_)||defined(DEBGE_LED_AND_LCD_TOGETHER)||defined(_FY5517_SPECIAL_DISPLAY_)||defined(_FY2010_LCD_DISPLAY_)
#if defined(_ENABLE_REALTIME_DIAPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	if(hour <12)
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_AM);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
	}
	else
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_AM);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PM);
	}

#if defined(_SUPPORT_AREA_)
#if !defined(_FY2010_LCD_DISPLAY_)
	if(setup.area == SETUP_AREA_USA)
#endif
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
		}
	}

#if defined(_FY5517_POWER_OFF_TIME_)
	if(global.sys_power)
	{
		disp_buf[0] = hour/10 + '0';
		disp_buf[1] = hour%10 + '0';
		disp_buf[2] = minute/10  + '0';
		disp_buf[3] = minute%10  + '0';
	}
	else
	{
		disp_buf[0] = ' ';
		disp_buf[1] = ' ';
		disp_buf[2] = ' ';
		disp_buf[3] = ' ';
		disp_buf[4] = ' ';
		disp_buf[5] = hour/10 + '0';
		disp_buf[6] = hour%10 + '0';
		disp_buf[7] = minute/10  + '0';
		disp_buf[8] = minute%10  + '0';
		disp_buf[9] = ' ';
		disp_buf[10] = ' ';
	}
#else
	disp_buf[0] = hour/10 + '0';
	disp_buf[1] = hour%10 + '0';
	disp_buf[2] = minute/10  + '0';
	disp_buf[3] = minute%10  + '0';
#endif
}

#elif defined(_MR889_SPECAIL_DISP_)
static void rtc_time_format(uint8 *disp_buf)
{
#if defined(_SEL_ADD_TIME_SELECT_)
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[6] = ' ';

    if(setup.rtc_disp == RTC_DISP_12H)
    {
    	if(hour <12)
    	{
    		if(hour == 0)
    		{
    			hour = 12;
    		}
    		disp_buf[0] = 'A';
    		disp_buf[1] = 'M';
    	}
    	else
    	{
    		if(hour != 12)
    		{
    			hour-=12;
    		}
    		disp_buf[0] = 'P';
    		disp_buf[1] = 'M';
    	}
    }

	disp_buf[2] = hour/10 + '0';
	disp_buf[3] = hour%10 + '0';
	disp_buf[4] = minute/10  + '0';
	disp_buf[5] = minute%10  + '0';
#else
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[0] = 'A';
			disp_buf[1] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[0] = 'P';
			disp_buf[1] = 'M';
		}
	}
	disp_buf[3] = hour/10 + '0';
	disp_buf[4] = hour%10 + '0';
	disp_buf[5] = minute/10  + '0';
	disp_buf[6] = minute%10  + '0';
#endif
}

#else
static void rtc_time_format(uint8 *disp_buf)
{
#if defined(_SEL_ADD_TIME_SELECT_)
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[6] = ' ';

    if(setup.rtc_disp == RTC_DISP_12H)
    {
    	if(hour <12)
    	{
    		if(hour == 0)
    		{
    			hour = 12;
    		}
    		disp_buf[0] = 'A';
    		disp_buf[1] = 'M';
    	}
    	else
    	{
    		if(hour != 12)
    		{
    			hour-=12;
    		}
    		disp_buf[0] = 'P';
    		disp_buf[1] = 'M';
    	}
    }

	disp_buf[2] = hour/10 + '0';
	disp_buf[3] = hour%10 + '0';
	disp_buf[4] = minute/10  + '0';
	disp_buf[5] = minute%10  + '0';
#else
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[6] = ' ';

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[0] = 'A';
			disp_buf[1] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[0] = 'P';
			disp_buf[1] = 'M';
		}
	}
	disp_buf[2] = hour/10 + '0';
	disp_buf[3] = hour%10 + '0';
	disp_buf[4] = minute/10  + '0';
	disp_buf[5] = minute%10  + '0';
#endif
}
#endif

#elif defined(_SUPPORT_TIME_DISPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{
	uint8 hour;
	uint8 minute;
	uint16 year;
	uint8 month;
	uint8 day;
	uint16 read_year;
	uint8 read_month;
    uint8 week;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
		year	= rtc.set_time.dYear;
		month	= rtc.set_time.bMonth;
		day		= rtc.set_time.bDay;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
		year	= rtc.cur_time.dYear;
		month	= rtc.cur_time.bMonth;
		day		= rtc.cur_time.bDay;
	}

	if((month == 1)||(month == 2))
	{
		read_year = year-1;
		read_month = 2*(month+12)+3*(month+12+1)/5;
		week = (day+read_month+read_year+read_year/4-read_year/100+read_year/400)%7;
	}
	else
	{
		week = (day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7;
	}

	ew_printf("year:%d\n",year);
	ew_printf("week:%d\n",week);

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
		}
	}

	disp_buf[0] = hour/10 + '0';//时间
	disp_buf[1] = hour%10 + '0';
	disp_buf[2] = minute/10  + '0';
	disp_buf[3] = minute%10  + '0';

	disp_buf[4] = day/10  + '0';//日期
	disp_buf[5] = day%10  + '0';

	disp_buf[6] = month/10  + '0';//月
	disp_buf[7] = month%10  + '0';

	//disp_buf[8] = (year/100)/10 + '0';//年
	//disp_buf[9] = (year/100)%10 + '0';
	disp_buf[8] = (year%100)/10 + '0';
	disp_buf[9] = (year%100)%10 + '0';

	ew_printf("disp_buf[8]:0x%x\n",disp_buf[8]);
	ew_printf("disp_buf[9]:0x%x\n",disp_buf[9]);

	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);

	switch(week)
	{
		case 0:
			disp_buf[10] = ']';
			disp_buf[11] = 'O';
			disp_buf[12] = '/';
			break;
		case 1:
			disp_buf[10] = 'T';
			disp_buf[11] = 'U';
			disp_buf[12] = 'E';
			break;
		case 2:
			disp_buf[10] = '[';
			disp_buf[11] = 'E';
			disp_buf[12] = 'D';
			break;
		case 3:
			disp_buf[10] = 'T';
			disp_buf[11] = 'H';
			disp_buf[12] = 'U';
			break;
		case 4:
			disp_buf[10] = 'F';
			disp_buf[11] = 'R';
			disp_buf[12] = 'I';
			break;
		case 5:
			disp_buf[10] = 'S';
			disp_buf[11] = 'A';
			disp_buf[12] = 'T';
			break;
		case 6:
			disp_buf[10] = 'S';
			disp_buf[11] = 'U';
			disp_buf[12] = '/';
			break;
	}
}

#elif defined(_SL6812A_SPECIAL_DISPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{//时间正常显示
#if defined(_SEL_ADD_TIME_SELECT_)
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[3] = ' ';
	disp_buf[4] = ' ';
	disp_buf[5] = ' ';
	
	if(setup.rtc_disp == RTC_DISP_12H)
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[2] = 'A';
			disp_buf[3] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[2] = 'P';
			disp_buf[3] = 'M';
		}
	}
	
	disp_buf[6] = hour/10 + '0';
	disp_buf[7] = hour%10 + '0';
	disp_buf[8] = minute/10  + '0';
	disp_buf[9] = minute%10  + '0';

#else

	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[3] = ' ';
	disp_buf[4] = ' ';
	disp_buf[5] = ' ';

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[2] = 'A';
			disp_buf[3] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[2] = 'P';
			disp_buf[3] = 'M';
		}
	}
	disp_buf[6] = hour/10 + '0';
	disp_buf[7] = hour%10 + '0';
	disp_buf[8] = minute/10  + '0';
	disp_buf[9] = minute%10  + '0';
#endif
}

#elif defined(_XZC1668_SPECIAL_DISPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{//时间正常显示
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[3] = ' ';
	disp_buf[4] = ' ';
	disp_buf[9] = ' ';

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[0] = 'A';
			disp_buf[1] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[0] = 'P';
			disp_buf[1] = 'M';
		}
	}
	disp_buf[5] = hour/10 + '0';
	disp_buf[6] = hour%10 + '0';
	disp_buf[7] = minute/10  + '0';
	disp_buf[8] = minute%10  + '0';
}

#elif defined(_SH1330_SPECIAL_DISPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{//时间正常显示
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[3] = ' ';
	disp_buf[4] = ' ';
	disp_buf[5] = ' ';

#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[0] = 'A';
			disp_buf[1] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[0] = 'P';
			disp_buf[1] = 'M';
		}
	}
	disp_buf[6] = hour/10 + '0';
	disp_buf[7] = hour%10 + '0';
	disp_buf[8] = minute/10  + '0';
	disp_buf[9] = minute%10  + '0';
}

#elif defined(_XM003_SPECIAL_DISPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	//ew_printf("hour:%d\n",hour);
	//ew_printf("minute:%d\n",minute);
	//ew_printf("sec:%d\n",rtc.cur_time.bSec);

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
#if defined(_SUPPORT_AREA_)
	if(setup.area == SETUP_AREA_USA)
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
			disp_buf[0] = 'A';
			disp_buf[1] = 'M';
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
			disp_buf[0] = 'P';
			disp_buf[1] = 'M';
		}
	}
	disp_buf[3] = hour/10 + '0';
	disp_buf[4] = hour%10 + '0';
	disp_buf[5] = minute/10  + '0';
	disp_buf[6] = minute%10  + '0';
}

#elif defined(_SEVEN_CHAR_DISPLAY_)
#if !defined(_ENABLE_REALTIME_DIAPLAY_)
static void rtc_time_format(uint8 *disp_buf)
{//时间正常显示
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}
#if !defined(TD_ADD_LCD_NUM_4)
#if defined(_JL8253_TIME_DISPLAY_)
	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[6] = ' ';
	disp_buf[7] = ' ';
#else
	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
#endif
#endif

#if defined(_SUPPORT_AREA_)
#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
	if((setup.area == SETUP_AREA_USA)||(setup.area == SETUP_AREA_LATIN))
#else
	if(setup.area == SETUP_AREA_USA)
#endif
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
	#if !defined(_USA_DISP_24HOUR_)
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
		#if !defined(TD_ADD_LCD_NUM_4)
			disp_buf[1] = 'A';
			disp_buf[2] = 'M';
		#endif
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
		#if !defined(TD_ADD_LCD_NUM_4)
			disp_buf[1] = 'P';
			disp_buf[2] = 'M';
		#endif
		}
	#endif
	}
#if defined(TD_ADD_LCD_NUM_4)
	disp_buf[0] = hour/10 + '0';
	disp_buf[1] = hour%10 + '0';
	disp_buf[2] = minute/10  + '0';
	disp_buf[3] = minute%10  + '0';
#elif defined(_JL8253_TIME_DISPLAY_)
	disp_buf[2] = hour/10 + '0';
	disp_buf[3] = hour%10 + '0';
	disp_buf[4] = minute/10  + '0';
	disp_buf[5] = minute%10  + '0';
#else
	disp_buf[3] = hour/10 + '0';
	disp_buf[4] = hour%10 + '0';
	disp_buf[5] = minute/10  + '0';
	disp_buf[6] = minute%10  + '0';
#endif
}
#endif

#elif defined(_TEMP_WARM_DISPLAY_)
uint8 warm_set = 10;
static void rtc_time_format(uint8 *disp_buf)
{
#if defined(_ENABLE_TEMP_PWM_)
	if(setup.warm_power)
	{
		if(setup.temp_set >= FUNC_SEL_TEMP_10)
		{
			disp_buf[0] = '1';
			disp_buf[1] = '0';
		}
		else
		{
			disp_buf[0] = '0';
			disp_buf[1] = (setup.temp_set+1) + '0';
		}
		
		if(warm_set != setup.warm_set)
		{
			warm_set = setup.warm_set;
			if(setup.warm_set == FUNC_SEL_WARM_30)
			{
				disp_buf[2] = '3';
				disp_buf[3] = '0';
			}
			else if(setup.warm_set == FUNC_SEL_WARM_60)
			{
				disp_buf[2] = '6';
				disp_buf[3] = '0';
			
			}
			else if(setup.warm_set == FUNC_SEL_WARM_ON)
			{
				disp_buf[2] = 'O';
				disp_buf[3] = 'N';
			}
		}
		else
		{
			if((setup.warm_time >= 36000)&&(setup.warm_time <= 43200))
			{
				disp_buf[2] = 'O';
				disp_buf[3] = 'N';
			}
			else if((setup.warm_time >= 600)&&(setup.warm_time <= 3660))
			{
				disp_buf[2] = setup.warm_time/600 + '0';
				disp_buf[3] = (setup.warm_time%600)/60 + '0';
			}
			else if(setup.warm_time < 600)
			{
				disp_buf[2] = '0';
				disp_buf[3] = setup.warm_time/60 + '0';
			}
			else
			{
				disp_buf[2] = setup.warm_time/3600 + '0';
				disp_buf[3] = 'H';
			}
		}
	}
	else
	{
		disp_buf[0] = ' ';
		disp_buf[1] = 'O';
		disp_buf[2] = 'F';
		disp_buf[3] = 'F';
	}
#else
	disp_buf[0] = '0';
	disp_buf[1] = '5';
	disp_buf[2] = '3';
	disp_buf[3] = '0';
#endif
}

#else
static void rtc_time_format(uint8 *disp_buf)
{//时间正常显示
#if defined(_SEL_ADD_TIME_SELECT_)
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
#if defined(_SL6812_TIME_DISPLAY_)
	disp_buf[7] = ' ';
#else
    disp_buf[3] = ' ';
#endif

    if(setup.rtc_disp == RTC_DISP_12H)
    {
    	if(hour <12)
    	{
    		if(hour == 0)
    		{
    			hour = 12;
    		}
    		disp_buf[0] = 'A';
    		disp_buf[1] = 'M';
    	}
    	else
    	{
    		if(hour != 12)
    		{
    			hour-=12;
    		}
    		disp_buf[0] = 'P';
    		disp_buf[1] = 'M';
    	}
    }

#if defined(_SL6812_TIME_DISPLAY_)
	disp_buf[3] = hour/10 + '0';
	disp_buf[4] = hour%10 + '0';
	disp_buf[5] = minute/10  + '0';
	disp_buf[6] = minute%10  + '0';
#else
	disp_buf[4] = hour/10 + '0';
	disp_buf[5] = hour%10 + '0';
	disp_buf[6] = minute/10  + '0';
	disp_buf[7] = minute%10  + '0';
#endif

#else
	uint8 hour;
	uint8 minute;

	if(rtc.state == RTC_STATE_SETTING)
	{
		hour	= rtc.set_time.bHour;
		minute	= rtc.set_time.bMin;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		hour	= rtc.cur_time.bHour;
		minute	= rtc.cur_time.bMin;
	}

	//ew_printf("hour:%d\n",hour);
	//ew_printf("minute:%d\n",minute);
	//ew_printf("sec:%d\n",rtc.cur_time.bSec);

#if !defined(TD_ADD_LCD_NUM_4)
#if defined(_JL8253_TIME_DISPLAY_)
    disp_buf[0] = ' ';
    disp_buf[1] = ' ';
    disp_buf[6] = ' ';
    disp_buf[7] = ' ';
#elif defined(_JL8258_TIME_DISPLAY_)
	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[7] = ' ';
#elif defined(_SL6812_TIME_DISPLAY_)
    disp_buf[0] = ' ';
    disp_buf[1] = ' ';
    disp_buf[2] = ' ';
    disp_buf[7] = ' ';
#elif defined(_HH5259_TIME_DISPLAY_)
	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[6] = ' ';
	disp_buf[7] = ' ';
	disp_buf[8] = ' ';
#else
#if defined(_LCD_3_DIAPLAY_)
#else
	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[3] = ' ';
#endif
#endif
#endif
#if defined(_SUPPORT_AREA_)
#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
	if((setup.area == SETUP_AREA_USA)||(setup.area == SETUP_AREA_LATIN))
#else
#if defined(_LCD_3_DIAPLAY_)||defined(_ENABLE_TIME12_DISPLAY_)
    if(setup.area == SETUP_AREA_EUR)
#else
	if(setup.area == SETUP_AREA_USA)
#endif
#endif
#else
	if(setup.rtc_disp == RTC_DISP_12H)
#endif
	{
	#if !defined(_USA_DISP_24HOUR_)
		if(hour <12)
		{
			if(hour == 0)
			{
				hour = 12;
			}
		#if !defined(TD_ADD_LCD_NUM_4)
		#if !defined(_DISP_AM_PM_DISP_)
		#if !defined(_LCD_3_DIAPLAY_)
			disp_buf[1] = 'A';
			disp_buf[2] = 'M';
		#endif
		#endif
		#endif
		#if defined(_LCD_3_DIAPLAY_)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
		#endif
		}
		else
		{
			if(hour != 12)
			{
				hour-=12;
			}
		#if !defined(TD_ADD_LCD_NUM_4)
		#if !defined(_DISP_AM_PM_DISP_)
		#if !defined(_LCD_3_DIAPLAY_)
			disp_buf[1] = 'P';
			disp_buf[2] = 'M';
		#endif
		#endif
		#endif
		#if defined(_LCD_3_DIAPLAY_)
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PM);
		#endif
		}
	#endif
	}
#if defined(TD_ADD_LCD_NUM_4)
	disp_buf[0] = hour/10 + '0';
	disp_buf[1] = hour%10 + '0';
	disp_buf[2] = minute/10  + '0';
	disp_buf[3] = minute%10  + '0';
#elif defined(_JL8253_TIME_DISPLAY_)
    disp_buf[2] = hour/10 + '0';
    disp_buf[3] = hour%10 + '0';
    disp_buf[4] = minute/10  + '0';
    disp_buf[5] = minute%10  + '0';
#elif defined(_HH5259_TIME_DISPLAY_)
    disp_buf[2] = hour/10 + '0';
    disp_buf[3] = hour%10 + '0';
    disp_buf[4] = minute/10  + '0';
    disp_buf[5] = minute%10  + '0';
#elif defined(_PS329a_SPECIAL_DISPLAY_)
    disp_buf[3] = hour/10 + '0';
    disp_buf[4] = hour%10 + '0';
    disp_buf[5] = minute/10  + '0';
    disp_buf[6] = minute%10  + '0';
#elif defined(_JL8258_TIME_DISPLAY_)
	disp_buf[3] = hour/10 + '0';
	disp_buf[4] = hour%10 + '0';
	disp_buf[5] = minute/10  + '0';
	disp_buf[6] = minute%10  + '0';
#elif defined(_LCD_3_DIAPLAY_)
    disp_buf[0] = hour/10 + '0';
	disp_buf[1] = hour%10 + '0';
	disp_buf[2] = minute/10  + '0';
	disp_buf[3] = minute%10  + '0';
#elif defined(_SL6812_TIME_DISPLAY_)
    disp_buf[3] = hour/10 + '0';
    disp_buf[4] = hour%10 + '0';
    disp_buf[5] = minute/10  + '0';
    disp_buf[6] = minute%10  + '0';
#else
	disp_buf[4] = hour/10 + '0';
	disp_buf[5] = hour%10 + '0';
	disp_buf[6] = minute/10  + '0';
	disp_buf[7] = minute%10  + '0';

#if defined(AP_ENABLE_REALTIME_DIAPLAY_)
	TFT_Tx_Clock_Display(hour,minute,rtc.state,rtc.select);
#endif
#endif
#if defined(_LCD_3_DIAPLAY_)
	if(disp_buf[0] == '0')
	{
		disp_buf[0] = ' ';
	}
#endif
#endif
}
#endif

static void rtc_date_format(uint8 *disp_buf)
{
	uint16 read_year;
	uint8 year;
	uint8 month;
	uint8 day;
	uint8 x_data,y_data,z_data;

	if(rtc.state == RTC_STATE_SETTING)
	{
		read_year	= rtc.set_time.dYear;
		month		= rtc.set_time.bMonth;
		day			= rtc.set_time.bDay;
	}
	else
	{
		sys_get_rtc(&rtc.cur_time);
		read_year	= rtc.cur_time.dYear;
		month		= rtc.cur_time.bMonth;
		day			= rtc.cur_time.bDay;
	}

#if defined(_FY7000_SPECIAL_DISPLAY_)
	year = read_year;
#else
	year  = (uint8)(read_year - 2000);
	year %= 100;
#endif

	if(setup.date_disp == DATE_DISP_MDY)
	{
		x_data = month;
		y_data = day;
		z_data = year;
	}
	else if(setup.date_disp == DATE_DISP_DMY)
	{
		x_data = day;
		y_data = month;
		z_data = year;
	}
	else
	{
		x_data = year;
		y_data = month;
		z_data = day;
	}

#if defined(_FY7000_SPECIAL_DISPLAY_)
    disp_buf[0] = ' ';
	disp_buf[1] = (x_data/100)/10 + '0';
	disp_buf[2] = (x_data/100)%10 + '0';
	disp_buf[3] = (x_data%100)/10 + '0';
	disp_buf[4] = (x_data%100)%10 + '0';
    disp_buf[5] = '-';
    disp_buf[6] = y_data/10 + '0';
    disp_buf[7] = y_data%10 + '0';
    disp_buf[8] = '-';
    disp_buf[9] = z_data/10 + '0';
    disp_buf[10] = z_data%10 + '0';
    disp_buf[11] = ' ';
    disp_buf[12] = ' ';
#else
	disp_buf[0] = x_data/10 + '0';
	disp_buf[1] = x_data%10 + '0';
	disp_buf[2] = '-';
	disp_buf[3] = y_data/10 + '0';
	disp_buf[4] = y_data%10 + '0';
	disp_buf[5] = '-';
	disp_buf[6] = z_data/10 + '0';
	disp_buf[7] = z_data%10 + '0';
#endif
}

static void rtc_format(uint8 *disp_buf)
{
	switch(rtc.sub_state.disp_state)
	{
		case RTC_DISP_TIME:
			rtc_time_format(disp_buf);
			break;
		case RTC_DISP_DATE:
			rtc_date_format(disp_buf);
			break;

	#if defined(_ENABLE_RADIO_RDS_)
		case RTC_DISP_PSN:
			rtc_psn_format(disp_buf);
			break;

		case RTC_DISP_FREQ:
			rtc_freq_format(disp_buf);
			break;

		case RTC_DISP_PTY:
			rtc_pty_format(disp_buf);
			break;
	#endif
	}
}

static void rtc_disp_normal(uint8 *disp_buf, uint8 *disp_dot)
{
	rtc_format(disp_buf);

	if(rtc.sub_state.disp_state == RTC_DISP_TIME)
	{
		if(rtc.flash)
		{
			*disp_dot = FALSE;
		}
		else
		{
			*disp_dot = TRUE;
		}
	}
	else
	{
		*disp_dot = FALSE;
	}
}

static void rtc_disp_setting(uint8 *disp_buf, uint8 *disp_dot)
{//时间设置显示
	rtc_format(disp_buf);
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	rtc.sub_state.setting_state = RTC_SETTING_TIME;
	*disp_dot = TRUE;
	if(rtc.flash)
	{
	#if defined(_EVB_SETTING_CLOCK_)
		disp_buf[0]=' ';
		disp_buf[1]=' ';
		disp_buf[2]=' ';
		disp_buf[3]=' ';
	#else
		if(rtc.select == RTC_SELECT_MIN)
		{
			disp_buf[2]=' ';
			disp_buf[3]=' ';
		}
		else
		{
			disp_buf[0]=' ';
			disp_buf[1]=' ';
		}
	#endif
	}
#elif defined(_MR889_SPECAIL_DISP_)
	if(rtc.sub_state.setting_state == RTC_SETTING_TIME)
	{
		*disp_dot = TRUE;
		if(rtc.flash)
		{
		#if defined(_VOL_SETTING_CLOCK_)
			disp_buf[4]=' ';
			disp_buf[5]=' ';
			disp_buf[6]=' ';
			disp_buf[7]=' ';
		#else

        	if(rtc.select == RTC_SELECT_MIN)
        	{
    			disp_buf[5]=' ';
    			disp_buf[6]=' ';
        	}
        	else
        	{
				disp_buf[3]=' ';
				disp_buf[4]=' ';
        	}
		#endif
		}
	}
	else if(rtc.sub_state.setting_state == RTC_SETTING_DATE)
	{
		*disp_dot = FALSE;
		if(rtc.flash)
		{
			if(rtc.select == RTC_SELECT_DAY)
			{
				if(setup.date_disp == DATE_DISP_MDY)
				{
					disp_buf[3]=' ';
					disp_buf[4]=' ';
				}
				else if(setup.date_disp == DATE_DISP_DMY)
				{
					disp_buf[0]=' ';
					disp_buf[1]=' ';
				}
				else if(setup.date_disp == DATE_DISP_YMD)
				{
					disp_buf[6]=' ';
					disp_buf[7]=' ';
				}
			}
			else if(rtc.select == RTC_SELECT_MONTH)
			{
				if(setup.date_disp == DATE_DISP_MDY)
				{
					disp_buf[0]=' ';
					disp_buf[1]=' ';
				}
				else if(setup.date_disp == DATE_DISP_DMY)
				{
					disp_buf[3]=' ';
					disp_buf[4]=' ';
				}
				else if(setup.date_disp == DATE_DISP_YMD)
				{
					disp_buf[3]=' ';
					disp_buf[4]=' ';
				}
			}
			else if(rtc.select == RTC_SELECT_YEAR)
			{
				if(setup.date_disp == DATE_DISP_MDY)
				{
					disp_buf[6]=' ';
					disp_buf[7]=' ';
				}
				else if(setup.date_disp == DATE_DISP_DMY)
				{
					disp_buf[6]=' ';
					disp_buf[7]=' ';
				}
				else if(setup.date_disp == DATE_DISP_YMD)
				{
					disp_buf[0]=' ';
					disp_buf[1]=' ';
				}
			}
		}
	}
#elif defined(_SUPPORT_TIME_DISPLAY_)
	if(rtc.sub_state.setting_state == RTC_SETTING_TIME)
	{
		if(rtc.flash)
		{
        	if(rtc.select == RTC_SELECT_MIN)
        	{
				*disp_dot = TRUE;
        		disp_buf[2]=' ';
        		disp_buf[3]=' ';
        	}
        	else if(rtc.select == RTC_SELECT_HOUR)
        	{
				*disp_dot = TRUE;
			    disp_buf[0]=' ';
        		disp_buf[1]=' ';
        	}
			else if(rtc.select == RTC_SELECT_YEAR)
			{
				*disp_dot = FALSE;
			    disp_buf[8]=' ';
        		disp_buf[9]=' ';

			    //disp_buf[10]=' ';
        		//disp_buf[11]=' ';
			}
			else if(rtc.select == RTC_SELECT_MONTH)
			{
				*disp_dot = FALSE;
			    disp_buf[6]=' ';
        		disp_buf[7]=' ';
			}
			else if(rtc.select == RTC_SELECT_DAY)
			{
				*disp_dot = FALSE;
			    disp_buf[4]=' ';
        		disp_buf[5]=' ';
			}
		}
	}
#else
	if(rtc.sub_state.setting_state == RTC_SETTING_TIME)
	{
		*disp_dot = TRUE;
		if(rtc.flash)
		{
		#if defined(_VOL_SETTING_CLOCK_)
			disp_buf[4]=' ';
			disp_buf[5]=' ';
			disp_buf[6]=' ';
			disp_buf[7]=' ';
		#else
        	if(rtc.select == RTC_SELECT_MIN)
        	{
			#if defined(_FY7000_SPECIAL_DISPLAY_)
			#if defined(_SUPPORT_LCM_1618_DRIVE)
			    disp_buf[7]=' ';
        		disp_buf[8]=' ';
			#elif defined(_FY8567_TIME_DISPLAY_)
    			disp_buf[2]=' ';
    			disp_buf[3]=' ';
			#else
    			disp_buf[11]=' ';
    			disp_buf[12]=' ';
			#endif
			#elif defined(_FIVE_CHAR_DISPLAY_)||defined(DEBGE_LED_AND_LCD_TOGETHER)||defined(_FY5517_SPECIAL_DISPLAY_)||defined(_FY2010_LCD_DISPLAY_)||defined(_KLD3300_SPECIAL_DISPLAY_)
			#if defined(_ENABLE_REALTIME_DIAPLAY_)
    			disp_buf[2]=' ';
    			disp_buf[3]=' ';
			#else
    			disp_buf[4]=' ';
    			disp_buf[5]=' ';
			#endif
			#elif defined(_JL8253_TIME_DISPLAY_)
    			disp_buf[4]=' ';
    			disp_buf[5]=' ';
			#elif defined(_HH5259_TIME_DISPLAY_)
    			disp_buf[4]=' ';
    			disp_buf[5]=' ';
			#elif defined(_JL8258_TIME_DISPLAY_)
    			disp_buf[5]=' ';
    			disp_buf[6]=' ';
			#elif defined(_SEVEN_CHAR_DISPLAY_)
			#if !defined(_FY2010_LCD_DISPLAY_)
    			disp_buf[5]=' ';
    			disp_buf[6]=' ';
			#endif
			#elif defined(_XM003_SPECIAL_DISPLAY_)
				disp_buf[5]=' ';
				disp_buf[6]=' ';
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)
    			disp_buf[7]=' ';
    			disp_buf[8]=' ';
			#elif defined(_SH1330_SPECIAL_DISPLAY_)
    			disp_buf[8]=' ';
    			disp_buf[9]=' ';	
			#elif defined(_SL6812_TIME_DISPLAY_)
			#if defined(_SL6812A_SPECIAL_DISPLAY_)
    			disp_buf[8]=' ';
    			disp_buf[9]=' ';
			#else
    			disp_buf[5]=' ';
    			disp_buf[6]=' ';
			#endif
			#else
        		disp_buf[6]=' ';
        		disp_buf[7]=' ';
			#endif
        	}
        	else
        	{
			#if defined(_FY7000_SPECIAL_DISPLAY_)
			#if defined(_SUPPORT_LCM_1618_DRIVE)
			    disp_buf[4]=' ';
        		disp_buf[5]=' ';
			#elif defined(_FY8567_TIME_DISPLAY_)
				disp_buf[0]=' ';
				disp_buf[1]=' ';
			#else
    			disp_buf[9]=' ';
    			disp_buf[10]=' ';
			#endif
			#elif defined(_FIVE_CHAR_DISPLAY_)||defined(DEBGE_LED_AND_LCD_TOGETHER)||defined(_FY5517_SPECIAL_DISPLAY_)||defined(_FY2010_LCD_DISPLAY_)||defined(_KLD3300_SPECIAL_DISPLAY_)
			#if defined(_ENABLE_REALTIME_DIAPLAY_)
    			disp_buf[0]=' ';
    			disp_buf[1]=' ';
			#else
				disp_buf[2]=' ';
				disp_buf[3]=' ';
			#endif
			#elif defined(_JL8253_TIME_DISPLAY_)
    			disp_buf[2]=' ';
    			disp_buf[3]=' ';
			#elif defined(_HH5259_TIME_DISPLAY_)
    			disp_buf[2]=' ';
    			disp_buf[3]=' ';
			#elif defined(_JL8258_TIME_DISPLAY_)
				disp_buf[3]=' ';
				disp_buf[4]=' ';
			#elif defined(_SEVEN_CHAR_DISPLAY_)
			#if !defined(_FY2010_LCD_DISPLAY_)
				disp_buf[3]=' ';
				disp_buf[4]=' ';
			#endif
			#elif defined(_XM003_SPECIAL_DISPLAY_)
				disp_buf[3]=' ';
				disp_buf[4]=' ';
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)
				disp_buf[5]=' ';
				disp_buf[6]=' ';
            #elif defined(_SH1330_SPECIAL_DISPLAY_)
				disp_buf[6]=' ';
    			disp_buf[7]=' ';
			#elif defined(_SL6812_TIME_DISPLAY_)
			#if defined(_SL6812A_SPECIAL_DISPLAY_)
    			disp_buf[6]=' ';
    			disp_buf[7]=' ';
			#else
				disp_buf[3]=' ';
				disp_buf[4]=' ';
			#endif
			#else
        		disp_buf[4]=' ';
        		disp_buf[5]=' ';
			#endif
        	}
		#endif
		}
	}
	else if(rtc.sub_state.setting_state == RTC_SETTING_DATE)
	{
		*disp_dot = FALSE;
		if(rtc.flash)
		{
			if(rtc.select == RTC_SELECT_DAY)
			{
				if(setup.date_disp == DATE_DISP_MDY)
				{
					disp_buf[3]=' ';
					disp_buf[4]=' ';
				}
				else if(setup.date_disp == DATE_DISP_DMY)
				{
					disp_buf[0]=' ';
					disp_buf[1]=' ';
				}
				else if(setup.date_disp == DATE_DISP_YMD)
				{
					disp_buf[6]=' ';
					disp_buf[7]=' ';
				}
			}
			else if(rtc.select == RTC_SELECT_MONTH)
			{
				if(setup.date_disp == DATE_DISP_MDY)
				{
					disp_buf[0]=' ';
					disp_buf[1]=' ';
				}
				else if(setup.date_disp == DATE_DISP_DMY)
				{
					disp_buf[3]=' ';
					disp_buf[4]=' ';
				}
				else if(setup.date_disp == DATE_DISP_YMD)
				{
					disp_buf[3]=' ';
					disp_buf[4]=' ';
				}
			}
			else if(rtc.select == RTC_SELECT_YEAR)
			{
				if(setup.date_disp == DATE_DISP_MDY)
				{
					disp_buf[6]=' ';
					disp_buf[7]=' ';
				}
				else if(setup.date_disp == DATE_DISP_DMY)
				{
					disp_buf[6]=' ';
					disp_buf[7]=' ';
				}
				else if(setup.date_disp == DATE_DISP_YMD)
				{
					disp_buf[0]=' ';
					disp_buf[1]=' ';
				}
			}
		}
	}
#endif
}

static void rtc_disp_init(void)
{
	memset(&rtc, 0, sizeof(rtc));
	sys_get_rtc(&rtc.cur_time);

#if defined(_SUPPORT_TIME_DISPLAY_)
	ui_disp.func_sel  = FUNC_SEL_TIME;
	rtc.state = RTC_STATE_SETTING;
#endif
}

#if defined(_SUPPORT_TIME_DISPLAY_)
void clean_rtc_disp(void)
{
	memset(rtc.disp_buffer, ' ', sizeof(rtc.disp_buffer));
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_TIME);
	ui_show_rtc(rtc.disp_buffer, 0xFF);
}
#elif defined(_TEMP_WARM_DISPLAY_)
void clean_rtc_disp(void)
{
	memset(rtc.disp_buffer, ' ', sizeof(rtc.disp_buffer));
	ui_show_rtc(rtc.disp_buffer, 0xFF);
}
#endif

void rtc_disp(void)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
#if defined(_FY8567_TIME_DISPLAY_)
	uint8 disp_buffer[4];
#else
	uint8 disp_buffer[13];
#endif
#elif defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_TEMP_WARM_DISPLAY_)
#if defined(_FY5517_POWER_OFF_TIME_)
	uint8 disp_buffer[11];
#else
	uint8 disp_buffer[4];
#endif
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 disp_buffer[10];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 disp_buffer[10];
#elif defined(_SUPPORT_TIME_DISPLAY_)
	uint8 disp_buffer[13];
#else
	uint8 disp_buffer[8];
#endif
	uint8 disp_dot;

	if(rtc.flash_count > DISP_POLLING_TIME)
	{
		rtc.flash_count -= DISP_POLLING_TIME;
	}
	else
	{
    	rtc.flash_count = 500;
		rtc.flash = !rtc.flash;
	}
	if(rtc.state == RTC_STATE_SETTING)
	{
	#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
    #if defined(_FY5517_TIME_DISPLAY_)
		if(ui_disp.timer1)
		{
			ui_disp.timer1--;
		}
		else
		{
			rtc.flash = FALSE;
			rtc.state = RTC_STATE_DISP;
			ui_disp.func_sel = FUNC_SEL_TIME;
		}
	#else
    	if(ui_disp.timer)
    	{
    		ui_disp.timer--;
        #if defined(_ENABLE_SET_TIME_)
        	global.sel_flag = TRUE;
        #endif
    	}
    	else
    	{
    		rtc.flash = FALSE;
    		rtc.state = RTC_STATE_DISP;
    		ui_disp.func_sel = FUNC_SEL_TIME;
    	}
	#endif
	#elif defined(_ENABLE_TIME_DISP_)
		global.rtc_time = TRUE;
    	if(setup.time_disp)
    	{
    		if(ui_disp.timer)
    		{
    			ui_disp.timer--;
    		}
    		else
    		{
    			rtc.flash = FALSE;
    			rtc.state = RTC_STATE_DISP;
    			ui_disp.func_sel = FUNC_SEL_TIME;
    		}
    	}
    #endif

	#if defined(ENABLE_TIME_SET)
    	rtc_time_save();
	#endif

		rtc_disp_setting(disp_buffer, &disp_dot);
	}
	else
	{
	#if defined(_ENABLE_TIME_DISP_)
		global.rtc_time = FALSE;
	#endif
		rtc_disp_normal(disp_buffer, &disp_dot);
	}

#if defined(_FY7000_SPECIAL_DISPLAY_)
#if defined(_FY8567_TIME_DISPLAY_)
    if(memcmp(rtc.disp_buffer, disp_buffer, 4))
    {
    	memcpy(rtc.disp_buffer, disp_buffer, 4);
    	ui_show_rtc(rtc.disp_buffer, 0xFF);
    }
#else
    if(memcmp(rtc.disp_buffer, disp_buffer, 13))
    {
    	memcpy(rtc.disp_buffer, disp_buffer, 13);
    	ui_show_rtc(rtc.disp_buffer, 0xFF);
    }
#endif
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SL6812A_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(memcmp(rtc.disp_buffer, disp_buffer, 10))
    {
    	memcpy(rtc.disp_buffer, disp_buffer, 10);
    	ui_show_rtc(rtc.disp_buffer, 0xFF);
    }
#elif defined(_SUPPORT_TIME_DISPLAY_)
    if(memcmp(rtc.disp_buffer, disp_buffer, 13))
    {
    	memcpy(rtc.disp_buffer, disp_buffer, 13);
    	ui_show_rtc(rtc.disp_buffer, 0xFF);
    }
#elif defined(_ENABLE_REALTIME_DIAPLAY_)
#if defined(_FY5517_POWER_OFF_TIME_)
	if(global.sys_power)
	{
		if(memcmp(rtc.disp_buffer, disp_buffer, 4))
		{
			memcpy(rtc.disp_buffer, disp_buffer, 4);
			ui_show_rtc(rtc.disp_buffer, 0xFF);
		}
	}
	else
	{
		if(memcmp(rtc.disp_buffer, disp_buffer, 11))
		{
			memcpy(rtc.disp_buffer, disp_buffer, 11);
			ui_show_rtc(rtc.disp_buffer, 0xFF);
		}
	}
#else
	if(memcmp(rtc.disp_buffer, disp_buffer, 4))
	{
		memcpy(rtc.disp_buffer, disp_buffer, 4);
		ui_show_rtc(rtc.disp_buffer, 0xFF);
	}
#endif
#elif defined(_TEMP_WARM_DISPLAY_)
    if(memcmp(rtc.disp_buffer, disp_buffer, 4))
    {
    	memcpy(rtc.disp_buffer, disp_buffer, 4);
    	ui_show_rtc(rtc.disp_buffer, 0xFF);
    }
#else
	if(memcmp(rtc.disp_buffer, disp_buffer, 8))
	{
		memcpy(rtc.disp_buffer, disp_buffer, 8);
		ui_show_rtc(rtc.disp_buffer, 0xFF);
	}
#endif

	if(rtc.disp_dot != disp_dot)
	{
		rtc.disp_dot = disp_dot;
		ui_show_rtc(NULL, rtc.disp_dot);
	}
}

void rtc_disp_time(void)
{
#if !defined(_ENABLE_REALTIME_DIAPLAY_)||!defined(AP_ENABLE_REALTIME_DIAPLAY_)||!defined(_TEMP_WARM_DISPLAY_)
#if defined(_ENABLE_TIME_DISP_)
	if(!setup.time_disp)
	{
		memset(rtc.disp_buffer, 0, sizeof(rtc.disp_buffer));
	}
#else
#if !defined(_SUPPORT_TIME_DISPLAY_)
	memset(rtc.disp_buffer, 0, sizeof(rtc.disp_buffer));
#endif
#endif
#endif

	rtc.disp_dot = FALSE;

	ui_disp.timer = -1;
	ui_disp.func_sel = FUNC_SEL_TIME;
#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
#if defined(_FY5517_POWER_OFF_TIME_)
	if(global.sys_power)
	{
		ui_disp.now_level = UI_LEVEL_ALL;
	}
	else
	{
		ui_disp.now_level = DISP_LEVEL_SETTING;
	}
#else
	ui_disp.now_level = UI_LEVEL_ALL;
#endif
#elif defined(_ENABLE_TIME_DISP_)
	if(setup.time_disp)
	{
		ui_disp.now_level = UI_LEVEL_ALL;
	}
	else
	{
		ui_disp.now_level = DISP_LEVEL_SETTING;
	}
#else
	ui_disp.now_level = DISP_LEVEL_SETTING;
#endif

	rtc.state 					= RTC_STATE_DISP;
	rtc.sub_state.disp_state	= RTC_DISP_TIME;
	rtc.flash_count 			= 500;
	rtc.flash 					= FALSE;
	rtc_disp();
}

/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_TIME_DISP_)
uint8 msg_realtime_check(void)
{
	if(setup.time_disp)
	{
		if((ui_disp.now_level == UI_LEVEL_ALL)
		&& (ui_disp.func_sel  == FUNC_SEL_TIME)
		&& (rtc.state == RTC_STATE_SETTING))
		{
			return TRUE;
		}
	}
	else
	{
		if((ui_disp.now_level == DISP_LEVEL_SETTING)
		&& (ui_disp.func_sel  == FUNC_SEL_TIME)
		&& (rtc.state == RTC_STATE_SETTING))
		{
			return TRUE;
		}
	}
	return FALSE;
}

#else
uint8 msg_realtime_check(void)
{
#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)
	if((ui_disp.now_level == UI_LEVEL_ALL)
#else
	if((ui_disp.now_level == DISP_LEVEL_SETTING)
#endif
	&& (ui_disp.func_sel  == FUNC_SEL_TIME)
	&& (rtc.state == RTC_STATE_SETTING))
	{
		return TRUE;
	}

	return FALSE;
}
#endif

#if defined(_SPECIAL_LCD_4_DIAPLAY_)
uint8 disp_realtime_check(void)
{
#if defined(_ENABLE_REALTIME_DIAPLAY_)
	if((ui_disp.now_level == UI_LEVEL_ALL)
#else
	if((ui_disp.now_level == DISP_LEVEL_SETTING)
#endif
	&& (ui_disp.func_sel  == FUNC_SEL_TIME))
	{
		return TRUE;
	}

	return FALSE;
}
#endif

#if defined(_EVB_DISP_TIME_SET_)
uint8 msg_realtime_hour_check(void)
{
#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)
	if((ui_disp.now_level == UI_LEVEL_ALL)
#else
	if((ui_disp.now_level == DISP_LEVEL_SETTING)
#endif
	&& (ui_disp.func_sel  == FUNC_SEL_TIME)
	&& (rtc.state == RTC_STATE_SETTING)
	&& (rtc.select == RTC_SELECT_HOUR))
	{
		return TRUE;
	}

	return FALSE;
}

uint8 msg_realtime_min_check(void)
{
#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)
	if((ui_disp.now_level == UI_LEVEL_ALL)
#else
	if((ui_disp.now_level == DISP_LEVEL_SETTING)
#endif
	&& (ui_disp.func_sel  == FUNC_SEL_TIME)
	&& (rtc.state == RTC_STATE_SETTING)
	&& (rtc.select == RTC_SELECT_MIN))
	{
		return TRUE;
	}

	return FALSE;
}
#endif

#if defined(_SUPPORT_TIME_DISPLAY_)
uint8 msg_realtime_day_check(void)
{
	if((ui_disp.now_level == UI_LEVEL_ALL)
	&& (ui_disp.func_sel  == FUNC_SEL_TIME)
	&& (rtc.state == RTC_STATE_SETTING)
	&& (rtc.select == RTC_SELECT_DAY))
	{
		return TRUE;
	}

	return FALSE;
}
#endif

void msg_realtime_show(void)
{
#if defined(SUPPORT_ALARM_FUNC_)
	global.time_flag = TRUE;
	global.alarm_flag = FALSE;
#endif
#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
#if defined(_FY5517_TIME_DISPLAY_)
	if(global.time_set)
#else
	if((ui_disp.now_level == UI_LEVEL_ALL)&&(ui_disp.func_sel == FUNC_SEL_TIME))
#endif
#elif defined(_ENABLE_TIME_DISP_)
	if(setup.time_disp)
	{
		if((ui_disp.now_level == UI_LEVEL_ALL)&&(ui_disp.func_sel == FUNC_SEL_TIME))
    	{
    		if(rtc.state == RTC_STATE_SETTING)
    		{
    			if(rtc.sub_state.setting_state == RTC_SETTING_TIME)
    			{
    				rtc_time_save();
    				rtc.sub_state.disp_state = RTC_DISP_TIME;
    			}
    			else if(rtc.sub_state.setting_state == RTC_SETTING_DATE)
    			{
    				rtc_time_save();
    				rtc.sub_state.disp_state = RTC_DISP_DATE;
    			}

    	    #if defined(ENABLE_TIME_SET)
    	    	ui_disp.timer	= 5000;
    	    #else
    	    	ui_disp.timer	= 10000;
    	    #endif
    			rtc.flash_count = 500;
    			rtc.state 		= RTC_STATE_DISP;
    			rtc.flash 		= FALSE;
    			rtc_disp();
    		}
		#if 0
    		else
    		{
    			rtc_disp_state_e next_state;
    			uint8 index;

    			next_state = rtc.sub_state.disp_state;

    		#if defined(_ENABLE_RADIO_RDS_)
    		realtime_sel_again:
    			if((global.rds_ta)||(global.rds_af))
    			{
    				for(index = 0; index < sizeof(rtc_sel_tab); index++)
    				{
    					if(next_state == rtc_sel_tab[index])
    					{
    						break;
    					}
    				}

    				if(index >= (sizeof(rtc_sel_tab)-1))
    				{
    					ui_disp.now_level = ui_disp.front_level;
    				}
    				else
    				{
    					next_state = rtc_sel_tab[index+1];

    					switch(next_state)
    					{
    						default:
    							break;

    						case RTC_DISP_PSN:
    							{
    								if(rtc_rds_psn() != NULL)
    									goto realtime_sel_again;
    							}
    							break;

    						case RTC_DISP_FREQ:
    							{
    								if(!rtc_rds_freq())
    									goto realtime_sel_again;
    							}
    							break;

    						case RTC_DISP_PTY:
    							{
    								if(rtc_rds_pty() == 0xFF)
    									goto realtime_sel_again;
    							}
    							break;
    					}

    			    #if defined(ENABLE_TIME_SET)
    			    	ui_disp.timer	= 5000;
    			    #else
    			    	ui_disp.timer	= 10000;
    			    #endif
    					rtc.sub_state.disp_state = next_state;
    					rtc.flash_count = 500;
    					rtc.flash = TRUE;
    					rtc_disp();
    				}
    			}
    			else
    			{
    				for(index = 0; index < sizeof(rtc_sel_tab1); index++)
    				{
    					if(next_state == rtc_sel_tab1[index])
    					{
    						break;
    					}
    				}

    				if(index >= (sizeof(rtc_sel_tab1)-1))
    				{
    					ui_disp.now_level = ui_disp.front_level;
    				}
    				else
    				{
    					next_state = rtc_sel_tab1[index+1];

    					switch(next_state)
    					{
    						default:
    							break;

    						case RTC_DISP_PSN:
    							{
    								if(rtc_rds_psn() != NULL)
    									goto realtime_sel_again;
    							}
    							break;

    						case RTC_DISP_FREQ:
    							{
    								if(!rtc_rds_freq())
    									goto realtime_sel_again;
    							}
    							break;

    						case RTC_DISP_PTY:
    							{
    								if(rtc_rds_pty() == 0xFF)
    									goto realtime_sel_again;
    							}
    							break;
    					}

    			    #if defined(ENABLE_TIME_SET)
    			    	ui_disp.timer	= 5000;
    			    #else
    			    	ui_disp.timer	= 10000;
    			    #endif
    					rtc.sub_state.disp_state = next_state;
    					rtc.flash_count = 500;
    					rtc.flash = TRUE;
    					rtc_disp();
    				}
    			}
    		#else
        		for(index = 0; index < sizeof(rtc_sel_tab1); index++)
        		{
        			if(next_state == rtc_sel_tab1[index])
        			{
        				break;
        			}
        		}
    		#ifdef	TD_ADD_ALEN
    			if(index ==0)//不需要显示日期
    			{
    				index =1;
    			}
    		#endif
        		if((index > ((sizeof(rtc_sel_tab1))-1))||(index == ((sizeof(rtc_sel_tab1))-1)))
        		{
                    ui_disp.func_sel = FUNC_SEL_NULL;
        			ui_disp.now_level = ui_disp.front_level;
    			}
        		else
        		{
        			next_state = rtc_sel_tab1[index+1];

        			switch(next_state)
        			{
        				default:
        					break;
        			}

    		    #if defined(ENABLE_TIME_SET)
    		    	ui_disp.timer	= 5000;
    		    #else
    		    	ui_disp.timer	= 10000;
    		    #endif
    				rtc.flash_count = 500;
        			rtc.sub_state.disp_state = next_state;
        			rtc.flash = TRUE;
        			rtc_disp();
        		}
    		#endif
    		}
		#endif
    	}
		else
		{
			memset(rtc.disp_buffer, 0, sizeof(rtc.disp_buffer));
			rtc.disp_dot = FALSE;
        #if defined(ENABLE_TIME_SET)
        	ui_disp.timer	= 5000;
        #else
        	ui_disp.timer	= 10000;
        #endif
			rtc.flash_count = 500;
			ui_disp.func_sel = FUNC_SEL_TIME;
	    #if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
	    	ui_disp.now_level = UI_LEVEL_ALL;
	    #elif defined(_ENABLE_TIME_DISP_)
	    	if(setup.time_disp)
	    	{
	    		ui_disp.now_level = UI_LEVEL_ALL;
	    	}
	    	else
	    	{
	    		ui_disp.now_level = DISP_LEVEL_SETTING;
	    	}
	    #else
	    	ui_disp.now_level = DISP_LEVEL_SETTING;
	    #endif
			rtc.state					= RTC_STATE_DISP;
			rtc.sub_state.disp_state	= RTC_DISP_TIME;
			rtc.flash					= FALSE;
			rtc_disp();
		}
	}
	else
	{
		if((ui_disp.now_level == DISP_LEVEL_SETTING)&&(ui_disp.func_sel == FUNC_SEL_TIME))
    	{
			ew_puts("rtc_disp777\n");
    		if(rtc.state == RTC_STATE_SETTING)
    		{
    			if(rtc.sub_state.setting_state == RTC_SETTING_TIME)
    			{
    				rtc_time_save();
    				rtc.sub_state.disp_state = RTC_DISP_TIME;
    			}
    			else if(rtc.sub_state.setting_state == RTC_SETTING_DATE)
    			{
    				rtc_time_save();
    				rtc.sub_state.disp_state = RTC_DISP_DATE;
    			}

    	    #if defined(ENABLE_TIME_SET)
    	    	ui_disp.timer	= 5000;
    	    #else
    	    	ui_disp.timer	= 10000;
    	    #endif
    			rtc.flash_count = 500;
    			rtc.state 		= RTC_STATE_DISP;
    			rtc.flash 		= FALSE;
    			rtc_disp();
    		}
    		else
    		{
    			rtc_disp_state_e next_state;
    			uint8 index;

    			next_state = rtc.sub_state.disp_state;

    		#if defined(_ENABLE_RADIO_RDS_)
    		realtime_sel_again:
    			if((global.rds_ta)||(global.rds_af))
    			{
    				for(index = 0; index < sizeof(rtc_sel_tab); index++)
    				{
    					if(next_state == rtc_sel_tab[index])
    					{
    						break;
    					}
    				}

    				if(index >= (sizeof(rtc_sel_tab)-1))
    				{
    					ui_disp.now_level = ui_disp.front_level;
    				}
    				else
    				{
    					next_state = rtc_sel_tab[index+1];

    					switch(next_state)
    					{
    						default:
    							break;

    						case RTC_DISP_PSN:
    							{
    								if(rtc_rds_psn() != NULL)
    									goto realtime_sel_again;
    							}
    							break;

    						case RTC_DISP_FREQ:
    							{
    								if(!rtc_rds_freq())
    									goto realtime_sel_again;
    							}
    							break;

    						case RTC_DISP_PTY:
    							{
    								if(rtc_rds_pty() == 0xFF)
    									goto realtime_sel_again;
    							}
    							break;
    					}

    			    #if defined(ENABLE_TIME_SET)
    			    	ui_disp.timer	= 5000;
    			    #else
    			    	ui_disp.timer	= 10000;
    			    #endif
    					rtc.sub_state.disp_state = next_state;
    					rtc.flash_count = 500;
    					rtc.flash = TRUE;
    					rtc_disp();
    				}
    			}
    			else
    			{
    				for(index = 0; index < sizeof(rtc_sel_tab1); index++)
    				{
    					if(next_state == rtc_sel_tab1[index])
    					{
    						break;
    					}
    				}

    				if(index >= (sizeof(rtc_sel_tab1)-1))
    				{
    					ui_disp.now_level = ui_disp.front_level;
    				}
    				else
    				{
    					next_state = rtc_sel_tab1[index+1];

    					switch(next_state)
    					{
    						default:
    							break;

    						case RTC_DISP_PSN:
    							{
    								if(rtc_rds_psn() != NULL)
    									goto realtime_sel_again;
    							}
    							break;

    						case RTC_DISP_FREQ:
    							{
    								if(!rtc_rds_freq())
    									goto realtime_sel_again;
    							}
    							break;

    						case RTC_DISP_PTY:
    							{
    								if(rtc_rds_pty() == 0xFF)
    									goto realtime_sel_again;
    							}
    							break;
    					}

    			    #if defined(ENABLE_TIME_SET)
    			    	ui_disp.timer	= 5000;
    			    #else
    			    	ui_disp.timer	= 10000;
    			    #endif
    					rtc.sub_state.disp_state = next_state;
    					rtc.flash_count = 500;
    					rtc.flash = TRUE;
    					rtc_disp();
    				}
    			}
    		#else
        		for(index = 0; index < sizeof(rtc_sel_tab1); index++)
        		{
        			if(next_state == rtc_sel_tab1[index])
        			{
        				break;
        			}
        		}
    		#ifdef	TD_ADD_ALEN
    			if(index ==0)//不需要显示日期
    			{
    				index =1;
    			}
    		#endif
        		if((index > ((sizeof(rtc_sel_tab1))-1))||(index == ((sizeof(rtc_sel_tab1))-1)))
        		{
                    ui_disp.func_sel = FUNC_SEL_NULL;
        			ui_disp.now_level = ui_disp.front_level;
    			}
        		else
        		{
        			next_state = rtc_sel_tab1[index+1];

        			switch(next_state)
        			{
        				default:
        					break;
        			}

    		    #if defined(ENABLE_TIME_SET)
    		    	ui_disp.timer	= 5000;
    		    #else
    		    	ui_disp.timer	= 10000;
    		    #endif
    				rtc.flash_count = 500;
        			rtc.sub_state.disp_state = next_state;
        			rtc.flash = TRUE;
        			rtc_disp();
        		}
    		#endif
    		}
    	}
		else
		{
			memset(rtc.disp_buffer, 0, sizeof(rtc.disp_buffer));
			rtc.disp_dot = FALSE;
	    #if defined(ENABLE_TIME_SET)
	    	ui_disp.timer	= 5000;
	    #else
	    	ui_disp.timer	= 10000;
	    #endif
			rtc.flash_count 			= 500;
			ui_disp.func_sel			= FUNC_SEL_TIME;
	    #if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
	    	ui_disp.now_level = UI_LEVEL_ALL;
	    #elif defined(_ENABLE_TIME_DISP_)
	    	if(setup.time_disp)
	    	{
	    		ui_disp.now_level = UI_LEVEL_ALL;
	    	}
	    	else
	    	{
	    		ui_disp.now_level = DISP_LEVEL_SETTING;
	    	}
	    #else
	    	ui_disp.now_level = DISP_LEVEL_SETTING;
	    #endif
			rtc.state					= RTC_STATE_DISP;
			rtc.sub_state.disp_state	= RTC_DISP_TIME;
			rtc.flash					= FALSE;
			rtc_disp();
		}
	}
#else
	if((ui_disp.now_level == DISP_LEVEL_SETTING)&&(ui_disp.func_sel == FUNC_SEL_TIME))
#endif
#if !defined(_ENABLE_TIME_DISP_)
	{
		if(rtc.state == RTC_STATE_SETTING)
		{
			if(rtc.sub_state.setting_state == RTC_SETTING_TIME)
			{
	        #if defined(_ENABLE_SET_TIME_)
	        	global.sel_flag = FALSE;
	        #endif
				rtc_time_save();
				rtc.sub_state.disp_state = RTC_DISP_TIME;
			#if defined(_SPECIAL_MODE_TIME_)
				ui_disp.now_level = ui_disp.front_level;
			#endif
			}
			else if(rtc.sub_state.setting_state == RTC_SETTING_DATE)
			{
				rtc_time_save();
				rtc.sub_state.disp_state = RTC_DISP_DATE;
			}

	    #if defined(ENABLE_TIME_SET)
	    	ui_disp.timer	= 5000;
	    #else
	    #if defined(_FY5517_TIME_DISPLAY_)
	    	ui_disp.timer1	= 10000;
	    #else
	    	ui_disp.timer	= 10000;
	    #endif
	    #endif
			rtc.flash_count = 500;
			rtc.state 		= RTC_STATE_DISP;
			rtc.flash 		= FALSE;
			rtc_disp();
		}
		else
		{
			rtc_disp_state_e next_state;
			uint8 index;

			next_state = rtc.sub_state.disp_state;

	#if defined(_ENABLE_RADIO_RDS_)
		realtime_sel_again:
			if((global.rds_ta)||(global.rds_af))
			{
				for(index = 0; index < sizeof(rtc_sel_tab); index++)
				{
					if(next_state == rtc_sel_tab[index])
					{
						break;
					}
				}

				if(index >= (sizeof(rtc_sel_tab)-1))
				{
					ui_disp.now_level = ui_disp.front_level;
				}
				else
				{
					next_state = rtc_sel_tab[index+1];

					switch(next_state)
					{
						default:
							break;

						case RTC_DISP_PSN:
							{
								if(rtc_rds_psn() != NULL)
									goto realtime_sel_again;
							}
							break;

						case RTC_DISP_FREQ:
							{
								if(!rtc_rds_freq())
									goto realtime_sel_again;
							}
							break;

						case RTC_DISP_PTY:
							{
								if(rtc_rds_pty() == 0xFF)
									goto realtime_sel_again;
							}
							break;
					}

			    #if defined(ENABLE_TIME_SET)
			    	ui_disp.timer	= 5000;
			    #else
			    #if defined(_FY5517_TIME_DISPLAY_)
			    	ui_disp.timer1	= 10000;
			    #else
			    	ui_disp.timer	= 10000;
			    #endif
			    #endif
					rtc.sub_state.disp_state = next_state;
					rtc.flash_count = 500;
					rtc.flash = TRUE;
					rtc_disp();
				}
			}
			else
			{
				for(index = 0; index < sizeof(rtc_sel_tab1); index++)
				{
					if(next_state == rtc_sel_tab1[index])
					{
						break;
					}
				}

				if(index >= (sizeof(rtc_sel_tab1)-1))
				{
					ui_disp.now_level = ui_disp.front_level;
				}
				else
				{
					next_state = rtc_sel_tab1[index+1];

					switch(next_state)
					{
						default:
							break;

						case RTC_DISP_PSN:
							{
								if(rtc_rds_psn() != NULL)
									goto realtime_sel_again;
							}
							break;

						case RTC_DISP_FREQ:
							{
								if(!rtc_rds_freq())
									goto realtime_sel_again;
							}
							break;

						case RTC_DISP_PTY:
							{
								if(rtc_rds_pty() == 0xFF)
									goto realtime_sel_again;
							}
							break;
					}

			    #if defined(ENABLE_TIME_SET)
			    	ui_disp.timer	= 5000;
			    #else
			    #if defined(_FY5517_TIME_DISPLAY_)
			    	ui_disp.timer1	= 10000;
			    #else
			    	ui_disp.timer	= 10000;
			    #endif
			    #endif
					rtc.sub_state.disp_state = next_state;
					rtc.flash_count = 500;
					rtc.flash = TRUE;
					rtc_disp();
				}
			}
		#else
    		for(index = 0; index < sizeof(rtc_sel_tab1); index++)
    		{
    			if(next_state == rtc_sel_tab1[index])
    			{
    				break;
    			}
    		}
		#ifdef	TD_ADD_ALEN
			if(index ==0)//不需要显示日期
			{
				index =1;
			}
		#endif
    		if((index > ((sizeof(rtc_sel_tab1))-1))||(index == ((sizeof(rtc_sel_tab1))-1)))
    		{
                ui_disp.func_sel = FUNC_SEL_NULL;
    			ui_disp.now_level = ui_disp.front_level;
			}
    		else
    		{
    			next_state = rtc_sel_tab1[index+1];

    			switch(next_state)
    			{
    				default:
    					break;
    			}

		    #if defined(ENABLE_TIME_SET)
		    	ui_disp.timer	= 5000;
		    #else
		    #if defined(_FY5517_TIME_DISPLAY_)
		    	ui_disp.timer1	= 10000;
		    #else
		    	ui_disp.timer	= 10000;
		    #endif
		    #endif
				rtc.flash_count = 500;
    			rtc.sub_state.disp_state = next_state;
    			rtc.flash = TRUE;
    			rtc_disp();
    		}
		#endif
		}
	}
	else
	{
		memset(rtc.disp_buffer, 0, sizeof(rtc.disp_buffer));
		rtc.disp_dot = FALSE;
    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer	= 5000;
    #else
    #if defined(_FY5517_TIME_DISPLAY_)
    	ui_disp.timer1	= 10000;
    #else
    	ui_disp.timer	= 10000;
    #endif
    #endif
		rtc.flash_count 			= 500;
		ui_disp.func_sel 			= FUNC_SEL_TIME;
    #if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
    	ui_disp.now_level = UI_LEVEL_ALL;
	#elif defined(_ENABLE_TIME_DISP_)
		if(setup.time_disp)
		{
			ui_disp.now_level = UI_LEVEL_ALL;
		}
		else
		{
			ui_disp.now_level = DISP_LEVEL_SETTING;
		}
    #else
    	ui_disp.now_level = DISP_LEVEL_SETTING;
    #endif
		rtc.state 					= RTC_STATE_DISP;
		rtc.sub_state.disp_state	= RTC_DISP_TIME;
		rtc.flash 					= FALSE;
		rtc_disp();
	}
#endif

#if defined(_FY5517_TIME_DISPLAY_)
	global.time_set = FALSE;
#endif

#if defined(_FY5517_TIME_SET_)
	ui_disp.func_sel = FUNC_SEL_NULL;
#endif
}

void msg_realtime_setting(void)
{
	ew_puts("msg_realtime_setting\n");

#if defined(SUPPORT_ALARM_FUNC_)
	global.time_flag = TRUE;
	global.alarm_flag = FALSE;
#endif

#if defined(_FY5517_TIME_DISPLAY_)
	global.time_set = TRUE;
#endif

#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
	if((ui_disp.now_level == UI_LEVEL_ALL)&&(ui_disp.func_sel == FUNC_SEL_TIME))
#elif defined(_ENABLE_TIME_DISP_)
	if(setup.time_disp)
	{
		if((ui_disp.now_level == UI_LEVEL_ALL)&&(ui_disp.func_sel == FUNC_SEL_TIME))
    	{
    		if(rtc.state == RTC_STATE_SETTING)
    		{
    			if((rtc.sub_state.setting_state == RTC_SETTING_TIME)||(rtc.sub_state.setting_state == RTC_SETTING_DATE))
    			{
    				rtc_time_save();
    			}
    			ui_disp.now_level = ui_disp.front_level;
    		}
    		else
    		{
    			if(rtc.sub_state.disp_state == RTC_DISP_TIME)
    			{
    				rtc_time_load();
    				rtc.select = RTC_SELECT_HOUR;
    			}
    			else if(rtc.sub_state.disp_state == RTC_DISP_DATE)
    			{
    				rtc_time_load();
    				rtc.select = RTC_SELECT_YEAR;
    			}
    			else return;

    	    #if defined(ENABLE_TIME_SET)
    	    	ui_disp.timer	= 5000;
    	    #else
    	    	ui_disp.timer	= 10000;
    	    #endif
    			rtc.state 		= RTC_STATE_SETTING;
    			rtc.flash_count = 500;
    			rtc.flash 		= TRUE;
    			rtc_disp();
    		}
		}
		else
		{
			memset(rtc.disp_buffer, 0, sizeof(rtc.disp_buffer));
			rtc.disp_dot = FALSE;

			rtc_time_load();

			ui_disp.timer				= 10000;
			ui_disp.func_sel			= FUNC_SEL_TIME;
	    #if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
	    	ui_disp.now_level = UI_LEVEL_ALL;
	    #elif defined(_ENABLE_TIME_DISP_)
	    	if(setup.time_disp)
	    	{
	    		ui_disp.now_level = UI_LEVEL_ALL;
	    	}
	    	else
	    	{
	    		ui_disp.now_level = DISP_LEVEL_SETTING;
	    	}
	    #else
	    	ui_disp.now_level = DISP_LEVEL_SETTING;
	    #endif
			rtc.state					= RTC_STATE_SETTING;
			rtc.sub_state.setting_state = RTC_SETTING_TIME;
			rtc.select					= RTC_SELECT_HOUR;
			rtc.flash_count 			= 500;
			rtc.flash					= TRUE;
			rtc_disp();
		}
	}
	else
	{
		if((ui_disp.now_level == DISP_LEVEL_SETTING)&&(ui_disp.func_sel == FUNC_SEL_TIME))
    	{
    		if(rtc.state == RTC_STATE_SETTING)
    		{
    			if((rtc.sub_state.setting_state == RTC_SETTING_TIME)||(rtc.sub_state.setting_state == RTC_SETTING_DATE))
    			{
    				rtc_time_save();
    			}
    			ui_disp.now_level = ui_disp.front_level;
    		}
    		else
    		{
    			if(rtc.sub_state.disp_state == RTC_DISP_TIME)
    			{
    				rtc_time_load();
    				rtc.select = RTC_SELECT_HOUR;
    			}
    			else if(rtc.sub_state.disp_state == RTC_DISP_DATE)
    			{
    				rtc_time_load();
    				rtc.select = RTC_SELECT_YEAR;
    			}
    			else return;

    	    #if defined(ENABLE_TIME_SET)
    	    	ui_disp.timer	= 5000;
    	    #else
    	    	ui_disp.timer	= 10000;
    	    #endif
    			rtc.state 		= RTC_STATE_SETTING;
    			rtc.flash_count = 500;
    			rtc.flash 		= TRUE;
    			rtc_disp();
    		}
		}
		else
		{
			memset(rtc.disp_buffer, 0, sizeof(rtc.disp_buffer));
			rtc.disp_dot = FALSE;

			rtc_time_load();

	    #if defined(ENABLE_TIME_SET)
	    	ui_disp.timer				= 5000;
	    #else
	    	ui_disp.timer				= 10000;
	    #endif
			ui_disp.func_sel			= FUNC_SEL_TIME;
        #if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
        	ui_disp.now_level = UI_LEVEL_ALL;
        #elif defined(_ENABLE_TIME_DISP_)
        	if(setup.time_disp)
        	{
        		ui_disp.now_level = UI_LEVEL_ALL;
        	}
        	else
        	{
        		ui_disp.now_level = DISP_LEVEL_SETTING;
        	}
        #else
        	ui_disp.now_level = DISP_LEVEL_SETTING;
        #endif
			rtc.state					= RTC_STATE_SETTING;
			rtc.sub_state.setting_state = RTC_SETTING_TIME;
			rtc.select					= RTC_SELECT_HOUR;
			rtc.flash_count 			= 500;
			rtc.flash					= TRUE;
			rtc_disp();
		}
	}
#else
	if((ui_disp.now_level == DISP_LEVEL_SETTING)&&(ui_disp.func_sel == FUNC_SEL_TIME))
#endif
#if !defined(_ENABLE_TIME_DISP_)
	{
		if(rtc.state == RTC_STATE_SETTING)
		{
			if((rtc.sub_state.setting_state == RTC_SETTING_TIME)||(rtc.sub_state.setting_state == RTC_SETTING_DATE))
			{
				rtc_time_save();
			}
			ui_disp.now_level = ui_disp.front_level;
		}
		else
		{
			if(rtc.sub_state.disp_state == RTC_DISP_TIME)
			{
				rtc_time_load();
				rtc.select = RTC_SELECT_HOUR;
			}
			else if(rtc.sub_state.disp_state == RTC_DISP_DATE)
			{
				rtc_time_load();
				rtc.select = RTC_SELECT_YEAR;
			}
			else return;

	    #if defined(ENABLE_TIME_SET)
	    	ui_disp.timer	= 5000;
	    #else
		#if defined(_FY5517_TIME_DISPLAY_)
			ui_disp.timer1	= 10000;
		#else
	    	ui_disp.timer	= 10000;
		#endif
	    #endif
			rtc.state 		= RTC_STATE_SETTING;
			rtc.flash_count = 500;
			rtc.flash 		= TRUE;
			rtc_disp();
		}
	}
	else
	{
		memset(rtc.disp_buffer, 0, sizeof(rtc.disp_buffer));
		rtc.disp_dot = FALSE;

		rtc_time_load();

    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer	= 5000;
    #else
    #if defined(_FY5517_TIME_DISPLAY_)
    	ui_disp.timer1	= 10000;
    #else
    	ui_disp.timer	= 10000;
    #endif
    #endif
		ui_disp.func_sel 	= FUNC_SEL_TIME;
    #if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(AP_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
    	ui_disp.now_level = UI_LEVEL_ALL;
	#elif defined(_ENABLE_TIME_DISP_)
		if(setup.time_disp)
		{
			ui_disp.now_level = UI_LEVEL_ALL;
		}
		else
		{
			ui_disp.now_level = DISP_LEVEL_SETTING;
		}
    #else
    	ui_disp.now_level = DISP_LEVEL_SETTING;
    #endif
		rtc.state 					= RTC_STATE_SETTING;
		rtc.sub_state.setting_state = RTC_SETTING_TIME;
		rtc.select 					= RTC_SELECT_HOUR;
    	rtc.flash_count 			= 500;
		rtc.flash 					= TRUE;
		rtc_disp();
	}
#endif
}

#if defined(_SUPPORT_TIME_DISPLAY_)
void msg_realtime_select(void)
{
	if(msg_realtime_check())
	{
		if(rtc.sub_state.setting_state == RTC_SETTING_TIME)
		{
			switch(rtc.select)
			{
				default:
				case RTC_SELECT_MIN:
					rtc.select = RTC_SELECT_YEAR;
					break;
				case RTC_SELECT_HOUR:
					rtc.select = RTC_SELECT_MIN;
					break;
				case RTC_SELECT_YEAR:
					rtc.select = RTC_SELECT_MONTH;
					break;
				case RTC_SELECT_MONTH:
					rtc.select = RTC_SELECT_DAY;
					break;
			}
		}

    	ui_disp.timer	= 10000;
		rtc.flash_count = 500;
		rtc.flash = TRUE;
		rtc_disp();
	}
}

#else
void msg_realtime_select(void)
{
#if defined(SUPPORT_ALARM_FUNC_)
	global.time_flag = TRUE;
	global.alarm_flag = FALSE;
#endif

#if defined(_FY5517_TIME_DISPLAY_)
	global.time_set = TRUE;
#endif

#if defined(_FY5517_TIME_DISPLAY_)
	if(global.time_set)
#else
	if(msg_realtime_check())
#endif
	{
		if(rtc.sub_state.setting_state == RTC_SETTING_TIME)
		{
			switch(rtc.select)
			{
				default:
				case RTC_SELECT_MIN :
					rtc.select = RTC_SELECT_HOUR;
					break;
				case RTC_SELECT_HOUR:
					rtc.select = RTC_SELECT_MIN;
					break;
			}
		}
		else if(rtc.sub_state.setting_state == RTC_SETTING_DATE)
		{
			switch(rtc.select)
			{
				default:
				case RTC_SELECT_DAY:
					rtc.select = RTC_SELECT_YEAR;
					break;
				case RTC_SELECT_MONTH:
					rtc.select = RTC_SELECT_DAY;
					break;
				case RTC_SELECT_YEAR:
					rtc.select = RTC_SELECT_MONTH;
					break;
			}
		}

    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer	= 5000;
    #else
    #if defined(_FY5517_TIME_DISPLAY_)
    	ui_disp.timer1	= 10000;
    #else
    	ui_disp.timer	= 10000;
    #endif
    #endif
		rtc.flash_count = 500;
		rtc.flash = TRUE;
		rtc_disp();
	}
}
#endif

void msg_realtime_add(void)
{
#if defined(SUPPORT_ALARM_FUNC_)
	global.time_flag = TRUE;
	global.alarm_flag = FALSE;
#endif

#if defined(_FY5517_TIME_DISPLAY_)
	global.time_set = TRUE;
#endif

#if defined(_FY5517_TIME_DISPLAY_)
	if(global.time_set)
#else
	if(msg_realtime_check())
#endif
	{
	#if defined(_VOL_SETTING_CLOCK_)
		if(rtc.set_time.bMin < 59)
		{
			rtc.set_time.bMin ++;
		}
		else
		{
			rtc.set_time.bMin = 0;
		}
	#elif defined(_EVB_SETTING_CLOCK_)
		if(rtc.set_time.bHour < 23)
		{
			rtc.set_time.bHour ++;
		}
		else
		{
			rtc.set_time.bHour = 0;
		}
	#else
		switch(rtc.select)
		{
			case RTC_SELECT_MIN:
				if(rtc.set_time.bMin < 59)
				{
					rtc.set_time.bMin ++;
				}
				else
				{
					rtc.set_time.bMin = 0;
				}
				break;
			case RTC_SELECT_HOUR:
				if(rtc.set_time.bHour < 23)
				{
					rtc.set_time.bHour ++;
				}
				else
				{
					rtc.set_time.bHour = 0;
				}
				break;
			case RTC_SELECT_DAY:
				if(rtc.set_time.bDay < rtc_month_total_day())
				{
					rtc.set_time.bDay ++;
				}
				else
				{
					rtc.set_time.bDay = 1;
				}
				break;
			case RTC_SELECT_MONTH:
				if(rtc.set_time.bMonth < 12)
				{
					rtc.set_time.bMonth ++;
				}
				else
				{
					rtc.set_time.bMonth = 1;
				}
				break;
			case RTC_SELECT_YEAR:
				if(rtc.set_time.dYear < 2099)
				{
					rtc.set_time.dYear ++;
				}
				else
				{
					rtc.set_time.dYear = 2019;
				}
				break;
		}
	#endif

    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer = 5000;
    	rtc.flash_count = 500;
    #else
    #if defined(_FY5517_TIME_DISPLAY_)
    	ui_disp.timer1	= 10000;
    #else
    	ui_disp.timer	= 10000;
    #endif
    	rtc.flash_count = 2000;
    #endif
		rtc.flash = FALSE;
		rtc_disp();
	}
}

void msg_realtime_sub(void)
{
#if defined(SUPPORT_ALARM_FUNC_)
	global.time_flag = TRUE;
	global.alarm_flag = FALSE;
#endif

#if defined(_FY5517_TIME_DISPLAY_)
	global.time_set = TRUE;
#endif

#if defined(_FY5517_TIME_DISPLAY_)
	if(global.time_set)
#else
	if(msg_realtime_check())
#endif
	{
	#if defined(_VOL_SETTING_CLOCK_)
    	if(rtc.set_time.bHour < 23)
    	{
    		rtc.set_time.bHour ++;
    	}
    	else
    	{
    		rtc.set_time.bHour = 0;
    	}
	#elif defined(_EVB_SETTING_CLOCK_)
		if(rtc.set_time.bMin < 59)
		{
			rtc.set_time.bMin ++;
		}
		else
		{
			rtc.set_time.bMin = 0;
		}
	#else
		switch(rtc.select)
		{
			case RTC_SELECT_MIN:
				if(rtc.set_time.bMin > 0)
				{
					rtc.set_time.bMin --;
				}
				else
				{
					rtc.set_time.bMin = 59;
				}
				break;
			case RTC_SELECT_HOUR:
				if(rtc.set_time.bHour > 0)
				{
					rtc.set_time.bHour --;
				}
				else
				{
					rtc.set_time.bHour = 23;
				}
				break;
			case RTC_SELECT_DAY:
				if(rtc.set_time.bDay > 1)
				{
					rtc.set_time.bDay --;
				}
				else
				{
					rtc.set_time.bDay = rtc_month_total_day();
				}
				break;
			case RTC_SELECT_MONTH:
				if(rtc.set_time.bMonth > 1)
				{
					rtc.set_time.bMonth --;
				}
				else
				{
					rtc.set_time.bMonth = 12;
				}
				break;
			case RTC_SELECT_YEAR:
				if(rtc.set_time.dYear > 2019)
				{
					rtc.set_time.dYear --;
				}
				else
				{
					rtc.set_time.dYear = 2099;
				}
				break;
		}
	#endif

    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer = 5000;
    	rtc.flash_count = 500;
    #else
    #if defined(_FY5517_TIME_DISPLAY_)
    	ui_disp.timer1	= 10000;
    #else
    	ui_disp.timer	= 10000;
    #endif
    	rtc.flash_count = 2000;
    #endif
		rtc.flash = FALSE;
		rtc_disp();
	}
}

/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_RTC_SLEEP_)
uint32 vm_reset_flag;
uint8 rtc_reset_flag;
#else
static uint32 vm_reset_flag;
#endif


static void rtc_isr_user_handler(u8 flag)
{
    if(INTERRUPT_TYPE_ALARM_COME == flag)
    {
        rtc_puts("--ALM-ON-ISR--\n");
    }
    else if(INTERRUPT_TYPE_PCNT_OVF == flag)
    {
        rtc_puts("--TYPE_PCNT_OVF--\n");
    }
    else if(INTERRUPT_TYPE_LDO5V_DET == flag)
    {
        rtc_puts("--LDO5V DET--\n");
    }
}

#if defined(_ENABLE_AUX_RESET_)
extern u8 soft_reset;
#endif

#if defined(_ENABLE_RTC_CLK_)
#if defined(_UPDATA_CLEAN_MEMORY_)
extern u8 usb_updata;
#endif
static void sys_reset_rtc(void)
{
	if(rtc_module_get_power_up_flag())
	{
		rtc_printf("rtc power off, PWR_CON:0x%x\n", JL_POWER->CON);
	#if defined(_ENABLE_ACC_MEMORY_)
		if(JL_POWER->CON == 0x60)//不会自动开机
		{
			vm_reset_flag = 0;
		}
		else
		{
			if(rtc_reset_flag!=0XAA)vm_reset_flag = -1;
		}
	#else
		if(rtc_reset_flag!=0XAA)vm_reset_flag = -1;
	#endif
	}
	else
	{
		rtc_printf("rtc reset, PWR_CON:0x%x\n", JL_POWER->CON);
		uint8 reset_state = JL_POWER->CON & (_B7_|_B6_|_B5_);
		/*
		b7...b5
		0:POWER_ON
		1:LVD 低电压复位
		2:WDT 益处复位
		3:SRST 软件复位
		4:POWEROFF
		*/

		if(reset_state == (3<<5))
		{//	软件复位
			rtc_puts("soft reset\n");
		//#if !defined(_ENABLE_RTC_SLEEP_)
		#if defined(_USB_UPG_CLEAN_MEMORY_)
			vm_reset_flag = -1;
		#else
			vm_reset_flag = 0;
		#endif
		//#endif
		}
		else if(reset_state == (4<<5))
		{
			//	ACC 上电唤醒
			rtc_puts("ACC walk up\n");
		//#if !defined(_ENABLE_RTC_SLEEP_)
			vm_reset_flag = 0;
		//#endif
		}
		else
		{
			rtc_puts("others reset\n");
		//#if !defined(_ENABLE_RTC_SLEEP_)
			vm_reset_flag = -1;
		//#endif
		}
	}

#if defined(_UPDATA_CLEAN_MEMORY_)
	vm_read_api(VM_USB_UPDATA, &usb_updata);
	printf("usb_updata:%d\n",usb_updata);
	if(usb_updata)
	{
		vm_reset_flag = -1;
	}
#endif

#if defined(_ENABLE_AUX_RESET_)
	vm_read_api(VM_SOFT_RESET, &soft_reset);
	printf("soft_reset:\n",soft_reset);
	if(soft_reset)
	{
		vm_reset_flag = -1;
	}
#endif

    sys_get_rtc(&rtc.cur_time);
	
	rtc_printf("time:y  %d\n",rtc.cur_time.dYear);
	rtc_printf("time:m  %d\n",rtc.cur_time.bMonth);
	rtc_printf("time:h  %d\n",rtc.cur_time.bHour);
	rtc_printf("time:Min  %d\n",rtc.cur_time.bMin);
	rtc_printf("time:bSec  %d\n",rtc.cur_time.bSec);
	rtc_printf("vm_reset_flag  %d\n",vm_reset_flag);

    if (((rtc.cur_time.dYear > 2100)
    || (rtc.cur_time.dYear < 1900))
    || (rtc.cur_time.bMonth >= 12)
    || (rtc.cur_time.bHour >= 24)
    || (rtc.cur_time.bMin >= 60)
    || (rtc.cur_time.bSec >= 60)
    || (vm_reset_flag == -1))
    {
        rtc_puts("\n--------RTC RESET--------\n");
        rtc.cur_time.dYear = 2020;
        rtc.cur_time.bMonth = 1;
        rtc.cur_time.bDay = 1;
        rtc.cur_time.bHour = 0;
        rtc.cur_time.bMin = 0;
        rtc.cur_time.bSec = 0;

        rtc_module_write_rtc_datetime(&rtc.cur_time);
    }

    rtc_module_reset_irtc();
}

#else

extern void accoff_auto_inc_onesecond(void);
static void sys_reset_rtc(void)
{
	uint8 reset_state;
	reset_state = JL_POWER->CON & (_B7_|_B6_|_B5_);
#ifdef	TD_ADD_IPONE_CHARGE
    usb_ERR =false;
#endif
	if(rtc_module_get_power_up_flag())
	{
		rtc_printf("rtc power off, PWR_CON:0x%x\n", JL_POWER->CON);
        {
			extern u8 read_reset_power();
	    	u8 pmu_rst_src = 0;
	    	pmu_rst_src = read_reset_power() >> 5;
	    	printf("pmu_rst_src : %d\n",pmu_rst_src); //3.3v reset source
			ew_printf("get_time_tick():%d\n", get_time_tick());
			if(get_time_tick() <= 2)
			{
				vm_reset_flag = -1;
				ew_puts("升级复位1111\n");
			}
			else
			{
				if(pmu_rst_src == 2)
				{
					printf("VCM RESET\n");
					AUDIO_MUTE_ON();
			    #if defined(_ENABLE_RCA_MUTE_)
			    	RCA_MUTE_ON();
			    #endif
					clr_time_cnt();
					vm_reset_flag = -1;
				}
				else
				{
					//1.2v reset source
					if(reset_state == (3<<5))
					{// 软件复位
						rtc_puts("soft reset\n");
					#if defined(_ENABLE_AUX_RESET_)
						vm_read_api(VM_SOFT_RESET, &soft_reset);
						printf("soft_reset:\n",soft_reset);
						if(soft_reset)
						{
							clr_time_cnt();
							vm_reset_flag = -1;
						}
						else
						{
							accoff_auto_inc_onesecond();
							vm_reset_flag = 0;
						}
					#else
						accoff_auto_inc_onesecond();
						vm_reset_flag = 0;
					#endif
					}
					else
					{
						clr_time_cnt();
						vm_reset_flag = -1;
					}
				}
			}
		}
	}
	else
	{
		rtc_printf("rtc reset, PWR_CON:0x%x\n", JL_POWER->CON);
		/*
		b7...b5
		0:POWER_ON
		1:LVD 低电压复位
		2:WDT 益处复位
		3:SRST 软件复位
		4:POWEROFF
		*/

		ew_printf("get_time_tick111():%d\n", get_time_tick());
		if(get_time_tick() <= 2)
		{
			vm_reset_flag = -1;
			ew_puts("升级复位1111\n");
		}
		else
		{
			if(reset_state == (3<<5))
			{// 软件复位
				rtc_puts("soft reset\n");
				accoff_auto_inc_onesecond();
				vm_reset_flag = 0;
			}
			else if(reset_state == (4<<5))
			{
				//	ACC 上电唤醒
				rtc_puts("ACC walk up\n");
				accoff_auto_inc_onesecond();
				vm_reset_flag = 0;
			}
			else
			{
				rtc_puts("others reset\n");
				clr_time_cnt();
			#if defined(_ENABLE_RTC_SLEEP_)
				vm_reset_flag = 0;
			#else
				vm_reset_flag = -1;
			#endif
			}
		}
	}
#ifdef SUPPORT_CALL_NAMES
    if(vm_reset_flag ==0)
    {
		acc_up_flag=TRUE;
	}
	else
	{
        acc_up_flag=FALSE;
	}
#endif

#if !defined(_ENABLE_RTC_CLK_)
    rtc_module_on(WAKE_UP_DISENABLE , 0, rtc_isr_user_handler);
#endif
    sys_get_rtc(&rtc.cur_time);
    if (((rtc.cur_time.dYear > 2100)
    || (rtc.cur_time.dYear < 1900))
    || (rtc.cur_time.bMonth >= 12)
    || (rtc.cur_time.bHour >= 24)
    || (rtc.cur_time.bMin >= 60)
    || (rtc.cur_time.bSec >= 60)
    || (vm_reset_flag == -1))
    {
        rtc_puts("\n--------RTC RESET--------\n");
        rtc.cur_time.dYear = 2019;
        rtc.cur_time.bMonth = 1;
        rtc.cur_time.bDay = 1;
        rtc.cur_time.bHour = 0;
        rtc.cur_time.bMin = 0;
        rtc.cur_time.bSec = 0;

        rtc_module_write_rtc_datetime(&rtc.cur_time);
    }

    rtc_module_reset_irtc();
}
#endif

void irtc_hw_init(void)
{
	rtc_puts("irtc_hw_init\n");
#if defined(_ENABLE_RTC_CLK_)
	rtc_module_on(WAKE_UP_DISENABLE , 0, rtc_isr_user_handler);
#endif
	sys_reset_rtc();
	rtc_disp_init();
}


static const u16 smonth_tab1[]= {0,31,59,90,120,151,181,212,243,273,304,334,365};	  ///<非闰年的当月累计天数
static const u16 smonth_tab2[]= {0,31,60,91,121,152,182,213,244,274,305,335,366};	  ///<	闰年的当月累计天数

static uint8 leapyear(u16 year)
{
    if ((year%4==0 && year%100!=0)||(year%400==0))
        return TRUE;
    else
        return FALSE;
}
static u16 year_to_day(u16 year)
{
    if (leapyear(year))
        return 366;
    else
        return 365;
}

u16 ymd_to_day(RTC_TIME *rtc_time)
{
    tu16 tmp_year;
    tu16 tmp_day = 0;

    for(tmp_year = YEAR; tmp_year < rtc_time->dYear; tmp_year++)		//年->日，当年不算在内，所以是<
    {
        tmp_day += year_to_day(tmp_year);
    }

    if (leapyear(rtc_time->dYear))									//月->日
        tmp_day += smonth_tab2[rtc_time->bMonth - MONTH];
    else
        tmp_day += smonth_tab1[rtc_time->bMonth - MONTH];

    tmp_day += (rtc_time->bDay - DAY);								//日->日,当日不算在内，所以日应该减1

    return tmp_day;
}

static u16 month_to_day(u16 year,u8 month)
{
    if (leapyear(year))
        return smonth_tab2[month];
    else
        return smonth_tab1[month];
}

void day_to_ymd(u16 day,RTC_TIME *rtc_time)
{
    tu8 tmp;

    tmp = 0;
    while (day >= year_to_day(YEAR + tmp))
    {
        day -= year_to_day(YEAR + tmp);
        tmp++;
    }
    rtc_time->dYear = YEAR+tmp;

    tmp = 0;
    while (day >= month_to_day(rtc_time->dYear, MONTH + tmp))
    {
        tmp++;
    }
    day -= month_to_day(rtc_time->dYear, MONTH + tmp - 1);
    rtc_time->bMonth = MONTH + tmp;

    rtc_time->bDay = DAY + day;
}
void sys_set_rtc(RTC_TIME *rtc_time)
{
#if defined(_ENABLE_RTC_CLK_)
	rtc_module_write_rtc_datetime(rtc_time);
#else
	rtc_reg_t reg_time;

	reg_time.day 	= ymd_to_day(rtc_time);
	reg_time.hour 	= rtc_time->bHour;
	reg_time.minute = rtc_time->bMin;
	reg_time.second = 0;//rtc_time->bSec;

	set_time_tick(reg_time.day * (3600 * 24) + reg_time.hour * 3600 + reg_time.minute * 60 + reg_time.second);
#endif
}

void sys_get_rtc(RTC_TIME *rtc_time)
{
#if defined(_ENABLE_RTC_CLK_)
	rtc_module_read_rtc_datetime(rtc_time);
#else
	uint32 titck = get_time_tick();
	uint32 day_tick = titck % (3600 * 24);

	day_to_ymd(titck / (3600 * 24), rtc_time);

	rtc_time->bHour		= day_tick / 3600;
	rtc_time->bMin		= (day_tick % 3600) / 60;
	rtc_time->bSec		= (day_tick % 3600) % 60;

	//ew_printf("titck:%x\n", titck);
	//ew_printf("day_tick:%x\n", day_tick);
	//ew_printf("rtc_time:%x\n", rtc_time);
#endif
}

/*----------------------------------------------------------------------------------------------------------------------------*/

uint8 vm_check_reset(uint32 flag)
{
	if(flag & VM_RESET_FM)
	{
		if(vm_reset_flag & VM_RESET_FM)return TRUE;
	}
	else if(flag & VM_RESET_SETUP)
	{
		if(vm_reset_flag & VM_RESET_SETUP)return TRUE;
	}

	return FALSE;
}

void vm_clean_reset(uint32 flag)
{
	vm_reset_flag &= (~flag);
}
#if defined(_SUPPORT_LCM_DRIVE)
u8 Digt_16_16buf[][60]=
{
    {0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x06,0x06,0x06,0x06,0x06,0x07,0x03,0x03,0x01,
	0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xC0,0xFF,0xFF,0x3F,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF0,0x70,0x38,0x18,
	0x18,0x18,0x18,0x18,0x38,0x30,0xF0,0xE0,0xC0,0x00,0x00,0x00},/*"0",0*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0xFF,0xFF,0xFF,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"1",0*/

	{0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x06,0x06,0x06,0x06,0x06,0x07,0x03,0x03,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0xE0,0xE0,0x81,0x01,0x03,0x03,0x07,0x06,
	0x0E,0x1C,0xF8,0xF8,0xE0,0x00,0x00,0x00,0x00,0x00,0x18,0x78,0x78,0xF8,0xD8,0xD8,
	0x98,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00},/*"2",0*/

    {0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x06,0x06,0x06,0x06,0x06,0x07,0x03,0x03,
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x18,0x18,0x18,
    0x18,0x3C,0xFC,0xE7,0xC7,0x03,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF0,0x30,0x38,0x18,
    0x18,0x18,0x18,0x18,0x18,0x38,0x30,0xF0,0xE0,0xC0,0x00,0x00},/*"3",0*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x07,0x07,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0D,0x19,0x31,0x71,0xE1,0xC1,0x81,0x01,
	0xFF,0xFF,0xFF,0x01,0x01,0x01,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0xF8,0xF8,0xF8,0x80,0x80,0x80,0x00,0x00},/*"4",0*/

    {0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
    0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xF8,0xF8,0x98,0x30,0x30,0x30,0x30,0x30,
    0x30,0x38,0x38,0x1F,0x0F,0x07,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF0,0x70,0x38,0x18,
    0x18,0x18,0x18,0x18,0x18,0x30,0x70,0xF0,0xE0,0x80,0x00,0x00},/*"5",0*/


    {0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x06,0x06,0x06,0x06,0x06,0x06,0x07,0x03,
    0x01,0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xCC,0x18,0x30,0x30,0x30,0x30,0x30,
    0x30,0x38,0x1C,0x9F,0x8F,0x87,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x30,0x18,
    0x18,0x18,0x18,0x18,0x18,0x38,0x70,0xF0,0xE0,0xC0,0x00,0x00},/*"6",0*/

	{0x00,0x00,0x00,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x07,0x07,
	0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,
	0x1F,0x7F,0xFC,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x18,0x78,0xF8,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"7",0*/

	{0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x06,0x06,0x06,0x06,0x06,0x06,0x03,0x03,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xE3,0xF3,0xFE,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,
	0x0C,0x1E,0xFE,0xF3,0xE3,0x01,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF0,0x30,0x38,0x18,
	0x18,0x18,0x18,0x18,0x18,0x38,0x30,0xF0,0xE0,0xC0,0x00,0x00},/*"8",0*/


    {0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x06,0x06,0x06,0x06,0x06,0x06,0x03,0x03,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFC,0xFE,0x8E,0x07,0x03,0x03,0x03,0x03,0x03,
    0x03,0x06,0x8E,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x40,0x60,0x70,0x38,0x18,0x18,
    0x18,0x18,0x18,0x18,0x38,0x30,0xF0,0xE0,0xC0,0x00,0x00,0x00},/*"9",0*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*":",0*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
};
void Lcm_Disp_20_32(u8 x_start,u8 y_start,u8 *data,u8 data_size)
{
    u8 *p_disp_buf;
    u8 curr_disp_page=0;
    y_start+=4;
    if(((16*data_size)+y_start) >131)
    {//y坐标越界 退出
        return;
    }
	for(u8 col=x_start;col<(x_start+3);col++)
	{//页数
        curr_disp_page=(col-x_start);
        Lcm_Start();
        Lcm_Write(0x70); // set slave address
        Lcm_Write(0xb3-col);  //set page address
        Lcm_Write(0x10+(4>>4));//set column address
        Lcm_Write(0x00+(4&0x0f));//
        Lcm_Stop();
        Lcm_Start();
        Lcm_Write(0x72); // set slave address
        for(u8 i=0;i<y_start;i++)
        {
            Lcm_Write(0x00);
        }
		for(u8 y=0;y<data_size;y++)
		{
			if(data[y]>=0&&data[y]<=9)
			{
				p_disp_buf=Digt_16_16buf[data[y]];
			}
			else if(data[y]>='0'&&data[y]<='9')
			{
				p_disp_buf=Digt_16_16buf[data[y]-'0'];
			}
            else if(data[y]==':')
            {
                p_disp_buf=Digt_16_16buf[10];
            }
            else
            {
                p_disp_buf=Digt_16_16buf[11];
            }
            for(u8 y_8bit=0;y_8bit<20;y_8bit++)
            {
                switch(curr_disp_page)
                {
                    case 0:
						Lcm_Write(p_disp_buf[y_8bit]);
						break;
					case 1:
						Lcm_Write(p_disp_buf[20+y_8bit]);
						break;
					case 2:
						Lcm_Write(p_disp_buf[40+y_8bit]|0x01);
						break;
                }
			}
		}
        for(u8 i=(y_start+data_size*8);i<132;i++)
        {
            Lcm_Write(0x00);
        }
        Lcm_Stop();
	}
}
void lcm_disp_clock(void)
{
    memset(lLcdStr,' ',16);
    if((rtc.state == RTC_STATE_SETTING)&&(rtc.select == RTC_SELECT_HOUR))
    {
        if(ghalf_sec)
        {
			lLcdStr[0] = rtc.set_time.bHour/10;
			lLcdStr[1] = rtc.set_time.bHour%10;
		}
    }
    else
    {
		lLcdStr[0] = rtc.cur_time.bHour/10;
		lLcdStr[1] = rtc.cur_time.bHour%10;
	}
    if(ghalf_sec)
    {
	    lLcdStr[2]=' ';
    }
    else
    {
	    lLcdStr[2]=':';
    }
    if((rtc.state == RTC_STATE_SETTING)&&(rtc.select == RTC_SELECT_MIN))
    {
        if(ghalf_sec)
        {
			lLcdStr[3] = rtc.set_time.bMin/10;
			lLcdStr[4] = rtc.set_time.bMin%10;
		}
    }
    else
    {
		lLcdStr[3] = rtc.cur_time.bMin/10;
		lLcdStr[4] = rtc.cur_time.bMin%10;
	}
    Lcm_Disp_20_32(0,14,lLcdStr,5);

    memset(lLcdStr,' ',16);
	lLcdStr[3] = rtc.cur_time.dYear%10000/1000;
	lLcdStr[4] = rtc.cur_time.dYear%1000/100;
	lLcdStr[5] = rtc.cur_time.dYear%100/10;
	lLcdStr[6] = rtc.cur_time.dYear%100%10;
    lLcdStr[7] ='-';
	lLcdStr[8] = rtc.cur_time.bMonth/10;
	lLcdStr[9] = rtc.cur_time.bMonth%10;
    lLcdStr[10] ='-';
	lLcdStr[11] = rtc.cur_time.bDay/10;
	lLcdStr[12] = rtc.cur_time.bDay%10;
    Lcm_Disp(3,0,lLcdStr,13);
}
#endif

