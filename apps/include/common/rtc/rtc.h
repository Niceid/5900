#ifndef __RTC_H__
#define __RTC_H__

#include "rtc/rtc_api.h"

#define RTC_DEBUG
#ifdef RTC_DEBUG
#define rtc_puts         	ew_puts
#define rtc_printf       	ew_printf
#else
#define rtc_puts(...)
#define rtc_printf(...)
#endif

#define YEAR 				2000
#define MONTH 				1
#define DAY 				1
#define HOUR 				0
#define MINUTES 			0
#define SECOND 				0


typedef struct
{
    uint16 	day;		///<天
    uint8 	hour;		///<时
    uint8 	minute;		///<分
    uint8 	second;		///<秒
}rtc_reg_t;

typedef enum{
	RTC_STATE_DISP = 0,
	RTC_STATE_SETTING,	
}rtc_state_e;

typedef enum{
	RTC_DISP_TIME = 0,
	RTC_DISP_DATE,
#if defined(_ENABLE_RADIO_RDS_)
	RTC_DISP_PSN,
	RTC_DISP_FREQ,
	RTC_DISP_PTY,
#endif
}rtc_disp_state_e;

typedef enum{
	RTC_SETTING_TIME = 0,
	RTC_SETTING_DATE,
}rtc_setting_state_e;

typedef enum{
	RTC_SELECT_MIN = 0,
	RTC_SELECT_HOUR,
	RTC_SELECT_DAY,
	RTC_SELECT_MONTH,
	RTC_SELECT_YEAR,
}rtc_select_e;

typedef union{
	rtc_disp_state_e 	disp_state;//0:显示时间  1:显示日期
	rtc_setting_state_e setting_state;//0:设置时间 1:设置日期
}rtc_sub_state_e;

#if defined(_ENABLE_RADIO_RDS_)

typedef struct{
	uint16 	freq;
	uint8 	psn[8];
	uint8 	pty;
}rds_info_t;

#endif

typedef struct{
#if defined(_FY7000_SPECIAL_DISPLAY_)
#if defined(_FY8567_TIME_DISPLAY_)
	uint8			disp_buffer[4];
#else
	uint8			disp_buffer[13];
#endif
#elif defined(_ENABLE_REALTIME_DIAPLAY_)
	uint8 			disp_buffer[4];
#elif defined(_TEMP_WARM_DISPLAY_)
	uint8			disp_buffer[4];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8			disp_buffer[10];
#elif defined(_SUPPORT_TIME_DISPLAY_)
	uint8			disp_buffer[13];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8			disp_buffer[10];
#else
	uint8			disp_buffer[8];
#endif
	uint8			disp_dot;
	rtc_state_e 	state;			//	实时时钟状态0:显示    1:设置
	rtc_sub_state_e	sub_state;		//	子状态
	rtc_select_e 	select;			//	临时设置类型选择
	uint16			flash_count;	//	延时闪烁计数器
	uint8 			flash;			//	闪烁计状态	
	RTC_TIME		set_time;		//	设置时间
	RTC_TIME		cur_time;		//	当前时间
}rtc_t;


extern void rtc_disp(void);
extern void rtc_disp_time(void);

extern uint8 msg_realtime_check(void);
extern void msg_realtime_show(void);
extern void msg_realtime_setting(void);
extern void msg_realtime_select(void);
extern void msg_realtime_add(void);
extern void msg_realtime_sub(void);
#if defined(_SPECIAL_LCD_4_DIAPLAY_)
extern uint8 disp_realtime_check(void);
#endif

#if defined(_EVB_DISP_TIME_SET_)
extern uint8 msg_realtime_min_check(void);
extern uint8 msg_realtime_hour_check(void);
#endif

#if defined(_SUPPORT_TIME_DISPLAY_)
extern uint8 msg_realtime_day_check(void);
extern void clean_rtc_disp(void);
#endif

#if defined(_TEMP_WARM_DISPLAY_)
extern void clean_rtc_disp(void);
#endif

#if defined(SUPPORT_ALARM_FUNC_)
extern void get_rtc_time(void);
#endif

extern void irtc_hw_init(void);
extern void sys_set_rtc(RTC_TIME *);
extern void sys_get_rtc(RTC_TIME *);


#define VM_RESET_FM				_B0_
#define VM_RESET_SETUP			_B1_
extern uint8 vm_check_reset(uint32);
extern void vm_clean_reset(uint32);

#endif /* __RTC_H__ */

