#ifndef _ALARM_H_
#define _ALARM_H_


#if defined(SUPPORT_ALARM_FUNC_)

typedef enum{
	ALARM_STATE_DISP = 0,
	ALARM_STATE_SETTING,	
}alarm_state_e;

typedef enum{
	ALARM_DISP_TIME = 0,
	ALARM_DISP_DATE,
}alarm_disp_state_e;

typedef enum{
	ALARM_SETTING_TIME = 0,
	ALARM_SETTING_DATE,
}alarm_setting_state_e;

typedef enum{
	ALARM_SELECT_MIN = 0,
	ALARM_SELECT_HOUR,
}alarm_select_e;

typedef union{
	alarm_disp_state_e 		disp_state;
	alarm_setting_state_e 	setting_state;
}alarm_sub_state_e;

typedef struct
{	uint8 	hour;
	uint8 	minute;
}alarm_time_t;

typedef struct{
	uint8				disp_buffer[8];
	uint8				disp_dot;
	alarm_state_e 		state;			//	实时时钟状态
	alarm_sub_state_e	sub_state;		//	子状态
	alarm_select_e 		select;			//	临时设置类型选择
	uint16				flash_count;	//	延时闪烁计数器
	uint8 				flash;			//	闪烁计状态	
	alarm_time_t		set_time;		//	设置时间
	alarm_time_t		cur_time;		//	当前时间
}alarm_t;

extern void rtc_init1(void);
extern void rtc_disp1(void);
extern void rtc_disp_time1(void);

extern uint8 msg_realtime_check1(void);
extern void msg_realtime_show1(void);
extern void msg_realtime_setting1(void);
extern void msg_realtime_select1(void);
extern void msg_realtime_add1(void);
extern void msg_realtime_sub1(void);

#endif
#endif

