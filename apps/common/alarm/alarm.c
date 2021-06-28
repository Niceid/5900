#include "sdk_cfg.h"
#include "dac/dac.h"
#include "setup.h"
#include "sys_detect.h"
#include "ui_res.h"
#include "ui_api.h"
#include "fm_engine.h"
#include "timer.h"
#include "disp/disp.h"
#include "alarm/alarm.h"

/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
#if defined(SUPPORT_ALARM_FUNC_)

static alarm_t alarm_rtc;

static const uint8 alarm_sel_tab[]=
{
	ALARM_DISP_TIME,
};

static void rtc_time_save1(void)
{
	//alarm_rtc.cur_time = alarm_rtc.set_time;
}

static void rtc_time_load1(void)
{
	//alarm_rtc.set_time = alarm_rtc.cur_time;
}

static void rtc_time_format1(uint8 *disp_buf)
{
	uint8 hour;
	uint8 minute;
	
	hour   = setup.alarm_hour;
	minute = setup.alarm_minute;

	disp_buf[0] = ' ';
	disp_buf[1] = ' ';
	disp_buf[2] = ' ';
	disp_buf[3] = ' ';
	if(setup.area == SETUP_AREA_USA)
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
	disp_buf[6] = minute/10  + '0';
	disp_buf[7] = minute%10  + '0';
}

static void rtc_format1(uint8 *disp_buf)
{
	rtc_time_format1(disp_buf);
}

static void rtc_disp_normal1(uint8 *disp_buf, uint8 *disp_dot)
{
	rtc_format1(disp_buf);

	if(alarm_rtc.sub_state.disp_state == ALARM_DISP_TIME)
	{
		if(alarm_rtc.flash)
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

static void rtc_disp_setting1(uint8 *disp_buf, uint8 *disp_dot)
{
	rtc_format1(disp_buf);

	if(alarm_rtc.sub_state.setting_state == ALARM_SETTING_TIME)
	{
		*disp_dot = TRUE;
		if(alarm_rtc.flash)
		{
	    #if defined(_VOL_SETTING_CLOCK_)
	    	disp_buf[4]=' ';
	    	disp_buf[5]=' ';
	    	disp_buf[6]=' ';
	    	disp_buf[7]=' ';
	    #else
        	if(alarm_rtc.select == ALARM_SELECT_MIN)
        	{
        		disp_buf[6]=' ';
        		disp_buf[7]=' ';
        	}
        	else
        	{
        		disp_buf[4]=' ';
        		disp_buf[5]=' ';
        	}
		#endif
		}
	}
}

void rtc_init1(void)
{
	memset(&alarm_rtc, 0, sizeof(alarm_rtc));
}


void rtc_disp1(void)
{
	uint8 disp_buffer[8];
	uint8 disp_dot;

	if(alarm_rtc.flash_count > DISP_POLLING_TIME)
	{
		alarm_rtc.flash_count -= DISP_POLLING_TIME;
	}
	else
	{
    	alarm_rtc.flash_count = 500;
		alarm_rtc.flash = !alarm_rtc.flash;
	}


	if(alarm_rtc.state == ALARM_STATE_SETTING)
	{
		rtc_disp_setting1(disp_buffer, &disp_dot);
	}
	else
	{
		rtc_disp_normal1(disp_buffer, &disp_dot);
	}

	if(memcmp(alarm_rtc.disp_buffer, disp_buffer, 8))
	{
		memcpy(alarm_rtc.disp_buffer, disp_buffer, 8);
		ui_show_rtc1(alarm_rtc.disp_buffer, 0xFF);
	}

    if(alarm_rtc.disp_dot != disp_dot)
    {
    	alarm_rtc.disp_dot = disp_dot;
    	ui_show_rtc1(NULL, alarm_rtc.disp_dot);
    }
}

void rtc_disp_time1(void)
{
	memset(alarm_rtc.disp_buffer, 0, sizeof(alarm_rtc.disp_buffer));
	alarm_rtc.disp_dot = FALSE;

	ui_disp.timer = -1;
	ui_disp.func_sel = FUNC_SEL_TIME;

	ui_disp.now_level = DISP_LEVEL_SETTING;

	alarm_rtc.state 					= ALARM_STATE_DISP;
	alarm_rtc.sub_state.disp_state		= ALARM_DISP_TIME;
	alarm_rtc.flash_count 				= 500;
	alarm_rtc.flash 					= FALSE;
	rtc_disp1();
}

/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
uint8 msg_realtime_check1(void)
{
	if((ui_disp.now_level == DISP_LEVEL_SETTING)
	&& (ui_disp.func_sel  == FUNC_SEL_TIME)
	&& (alarm_rtc.state == ALARM_STATE_SETTING))
	{
		return TRUE;
	}

	return FALSE;
}

void msg_realtime_show1(void)
{
#if defined(SUPPORT_ALARM_FUNC_)
	global.alarm_flag = TRUE;
	global.time_flag = FALSE;
#endif
	if((ui_disp.now_level == DISP_LEVEL_SETTING)&&(ui_disp.func_sel == FUNC_SEL_TIME))
	{
		if(alarm_rtc.state == ALARM_STATE_SETTING)
		{
			ew_puts("msg_realtime_setting_alarm333\n");
			if(alarm_rtc.sub_state.setting_state == ALARM_SETTING_TIME)
			{
				rtc_time_save1();
				alarm_rtc.sub_state.disp_state = ALARM_DISP_TIME;
			}

	    #if defined(ENABLE_TIME_SET)
	    	ui_disp.timer	= 5000;
	    #else
	    	ui_disp.timer	= 10000;
	    #endif
			alarm_rtc.flash_count = 500;
			alarm_rtc.state 		= ALARM_STATE_DISP;
			alarm_rtc.flash 		= FALSE;
			rtc_disp1();
		}
		else
		{
			alarm_disp_state_e next_state;
			uint8 index;

			next_state = alarm_rtc.sub_state.disp_state;

    		for(index = 0; index < sizeof(alarm_sel_tab); index++)
    		{
    			if(next_state == alarm_sel_tab[index])
    			{
    				break;
    			}
    		}

    		if(index >= (sizeof(alarm_sel_tab)-1))
    		{
    			ui_disp.now_level = ui_disp.front_level;
    		}
    		else
    		{
    			next_state = alarm_sel_tab[index+1];

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
				alarm_rtc.flash_count = 500;
    			alarm_rtc.sub_state.disp_state = next_state;
    			alarm_rtc.flash = TRUE;
    			rtc_disp1();
    		}
		}
	}
	else
	{
		memset(alarm_rtc.disp_buffer, 0, sizeof(alarm_rtc.disp_buffer));
		alarm_rtc.disp_dot = FALSE;
    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer	= 5000;
    #else
    	ui_disp.timer	= 10000;
    #endif
		alarm_rtc.flash_count 			= 500;

		ui_disp.func_sel			= FUNC_SEL_TIME;
    	ui_disp.now_level 			= DISP_LEVEL_SETTING;
		alarm_rtc.state 				= ALARM_STATE_DISP;
		alarm_rtc.sub_state.disp_state	= ALARM_DISP_TIME;
		alarm_rtc.flash 				= FALSE;
		rtc_disp1();
	}
}

void msg_realtime_setting1(void)
{
#if defined(SUPPORT_ALARM_FUNC_)
	global.alarm_flag = TRUE;
	global.time_flag = FALSE;
#endif
	if((ui_disp.now_level == DISP_LEVEL_SETTING)&&(ui_disp.func_sel == FUNC_SEL_TIME))
	{
		if(alarm_rtc.state == ALARM_STATE_SETTING)
		{
			if(alarm_rtc.sub_state.setting_state == ALARM_SETTING_TIME)
			{
				rtc_time_save1();
			}
			ui_disp.now_level = ui_disp.front_level;
		}
		else
		{
			if(alarm_rtc.sub_state.disp_state == ALARM_DISP_TIME)
			{
				rtc_time_load1();
				alarm_rtc.select = ALARM_SELECT_HOUR;
			}
			else return;

	    #if defined(ENABLE_TIME_SET)
	    	ui_disp.timer	= 5000;
	    #else
	    	ui_disp.timer	= 10000;
	    #endif
			alarm_rtc.state 		= ALARM_STATE_SETTING;
			alarm_rtc.flash_count   = 500;
			alarm_rtc.flash 		= TRUE;
			rtc_disp1();
		}
	}
	else
	{
		memset(alarm_rtc.disp_buffer, 0, sizeof(alarm_rtc.disp_buffer));
		alarm_rtc.disp_dot = FALSE;

		rtc_time_load1();

    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer	= 5000;
    #else
    	ui_disp.timer	= 10000;
    #endif
		ui_disp.func_sel 			= FUNC_SEL_TIME;
    	ui_disp.now_level = DISP_LEVEL_SETTING;
		alarm_rtc.state 					= ALARM_STATE_SETTING;
		alarm_rtc.sub_state.setting_state   = ALARM_SETTING_TIME;
		alarm_rtc.select 					= ALARM_SELECT_HOUR;
    	alarm_rtc.flash_count 				= 500;
		alarm_rtc.flash 					= TRUE;
		rtc_disp1();
	}
}
void msg_realtime_select1(void)
{
	if(msg_realtime_check1())
	{
		if(alarm_rtc.sub_state.setting_state == ALARM_SETTING_TIME)
		{
			switch(alarm_rtc.select)
			{
				default:
				case ALARM_SELECT_MIN :
					alarm_rtc.select = ALARM_SELECT_HOUR;
					break;
				case ALARM_SELECT_HOUR:
					alarm_rtc.select = ALARM_SELECT_MIN;
					break;
			}
		}

    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer	= 5000;
    #else
    	ui_disp.timer	= 10000;
    #endif
		alarm_rtc.flash_count = 500;
		alarm_rtc.flash = TRUE;
		rtc_disp1();
	}
}
void msg_realtime_add1(void)
{
#if defined(SUPPORT_ALARM_FUNC_)
	global.alarm_flag = TRUE;
	global.time_flag = FALSE;
#endif
	if(msg_realtime_check1())
	{
    #if defined(_VOL_SETTING_CLOCK_)
    	if(alarm_rtc.set_time.minute < 59)
    	{
    		alarm_rtc.set_time.minute ++;
    	}
    	else
    	{
    		alarm_rtc.set_time.minute = 0;
    	}
    #else
		switch(alarm_rtc.select)
		{
			case ALARM_SELECT_MIN:
				if(alarm_rtc.set_time.minute < 59)
				{
					alarm_rtc.set_time.minute ++;
				}
				else
				{
					alarm_rtc.set_time.minute = 0;
				}
				break;
			case ALARM_SELECT_HOUR:
				if(alarm_rtc.set_time.hour < 23)
				{
					alarm_rtc.set_time.hour ++;
				}
				else
				{
					alarm_rtc.set_time.hour = 0;
				}
				break;
		}
	#endif
	
		setup.alarm_minute = alarm_rtc.set_time.minute;
		setup.alarm_hour = alarm_rtc.set_time.hour;

    #if defined(ENABLE_TIME_SET)
    	ui_disp.timer = 5000;
    	alarm_rtc.flash_count = 500;
    #else
        ui_disp.timer = 10000;
        alarm_rtc.flash_count = 2000;
	#endif
		alarm_rtc.flash = FALSE;
		rtc_disp1();
	}
}

void msg_realtime_sub1(void)
{
#if defined(SUPPORT_ALARM_FUNC_)
	global.alarm_flag = TRUE;
	global.time_flag = FALSE;
#endif
	if(msg_realtime_check1())
	{
    #if defined(_VOL_SETTING_CLOCK_)
    	if(alarm_rtc.set_time.hour < 23)
    	{
    		alarm_rtc.set_time.hour ++;
    	}
    	else
    	{
    		alarm_rtc.set_time.hour = 0;
    	}
    #else
		switch(alarm_rtc.select)
		{
			case ALARM_SELECT_MIN:
				if(alarm_rtc.set_time.minute > 0)
				{
					alarm_rtc.set_time.minute --;
				}
				else
				{
					alarm_rtc.set_time.minute = 59;
				}
				break;
			case ALARM_SELECT_HOUR:
				if(alarm_rtc.set_time.hour > 0)
				{
					alarm_rtc.set_time.hour --;
				}
				else
				{
					alarm_rtc.set_time.hour = 23;
				}
				break;
		}
	#endif
	
		setup.alarm_minute = alarm_rtc.set_time.minute;
		setup.alarm_hour = alarm_rtc.set_time.hour;
		
	#if defined(ENABLE_TIME_SET)
    	ui_disp.timer = 5000;
    	alarm_rtc.flash_count = 500;
	#else
    	ui_disp.timer = 10000;
    	alarm_rtc.flash_count = 2000;
	#endif
	
		alarm_rtc.flash = FALSE;
		rtc_disp1();
	}
}
#endif

