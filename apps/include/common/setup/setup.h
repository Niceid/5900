#ifndef __SETUP_H__
#define __SETUP_H__


#define SETUP_DEBUG
#ifdef SETUP_DEBUG
#define setup_printf 			ew_printf
#define setup_puts 				ew_puts
#else
#define setup_printf(...)
#define setup_puts(...)
#endif


#if defined(_SUPPORT_THREE_AREA_)
typedef enum{
	SETUP_AREA_EUR = 0,
	SETUP_AREA_USA,
	SETUP_AREA_LATIN,
	SETUP_AREA_MAX,
}setup_area_e;
#elif defined(_SUPPORT_FOUR_AREA_)
typedef enum{
	SETUP_AREA_EUR = 0,
	SETUP_AREA_USA,
	SETUP_AREA_LATIN,
	SETUP_AREA_ASIA,
	SETUP_AREA_MAX,
}setup_area_e;
#else
typedef enum{
	SETUP_AREA_EUR = 0,
	SETUP_AREA_USA,
	SETUP_AREA_MAX,
}setup_area_e;
#endif

typedef enum{
	RTC_DISP_24H = 0,
	RTC_DISP_12H,
}rtc_disp_e;

typedef enum{
	DATE_DISP_MDY = 0,
	DATE_DISP_DMY,
	DATE_DISP_YMD,
}date_disp_e;

#if defined(_ENABLE_PASSWORD_INPUT_)
typedef enum{
	PWD_NUMBER_1 =1,
	PWD_NUMBER_2,
	PWD_NUMBER_3,
	PWD_NUMBER_4,
	PWD_NUMBER_END,
}pwd_number;
#endif

#if defined(_ENABLE_RADIO_RDS_)

typedef enum{
	RDS_DEC_RDS = 0,
	RDS_DEC_RBDS,
}rds_dec_type_e;

typedef struct{
	uint8 enable;
	uint8 type;
	uint8 af;
	uint8 ta;
	uint8 reg;
	uint8 eon;
	uint8 long_short;
	uint8 mask;
	uint8 alarm;
	uint8 time;
	uint8 volume;
	uint16 cur_freq;
}rds_set_t;
#endif

#if defined(_ACC_OFF_POWER_LOGO_)
typedef struct{
	uint8	status;
	uint8	point;
	uint8	step;
	uint8	timer;
}disp_ctrl_t;
#endif

typedef struct{
	/*U 盘记忆标志*/
	uint8 usb_break;
	/*SD 卡盘记忆标志*/
	uint8 sd_break;
#if defined(_SUPPORT_DISC_)
	/*DISC 碟片记忆标志*/
	uint8 disc_break;
#endif
#if defined(_USB_DELAY_CHECK_)
	uint8 usb_delay;
#endif

	/*音量*/
	uint8 aud_vol;
	/*响度开关*/
	uint8 aud_loud;
	/*低音*/
	int8  aud_bass;
	/*高音*/
	int8  aud_treble;
#if defined(_FAD_BAL_FUNTION_)
	int8  aud_fl;
	int8  aud_fr;
	int8  aud_rl;
	int8  aud_rr;
#else
	/*左右平衡*/
	int8  aud_bal;
	/*前后平衡*/
	int8  aud_fad;
#endif
	/*音效*/
	uint8  aud_eq;

#if defined(_SUPPORT_TWO_PT2313_)
	uint8 aud_vol2;
#endif

#if defined(_ENABLE_7419_MIDDLE_)
	/*中音*/
	int8  aud_middle;
#endif
#if defined(_CHANGE_DEF_VOL_)
	uint8 DefVol;
#endif
#if defined(_SPECIAL_TL2000_SET_)
	uint8 aud_A_volume;
	uint8 aud_B_volume;
#endif
#if defined(_SUPPORT_SUBWOOFER_)||defined(_SUPPORT_SUBWOOFER_MODE2_)
	/*!重低音*/
	uint8 aud_subwoofer;
#endif
#if defined(SUPPORT_ANTENNA_DISPLAY_)||defined(_SPECIAL_AUTOSEEK_MEMORY_)||defined(_YH1701_AUTOSEEK_MEMORY_)
	uint8 rssi;
#endif
#if defined(_SUPPORT_10_EQ_FUNTION_)
	int8  aud_60hz;
	int8  aud_150hz;
	int8  aud_400hz;
	int8  aud_1khz;
	int8  aud_2khz;
	int8  aud_3khz;
	int8  aud_7khz;
	int8  aud_10khz;
	int8  aud_15khz;
	int8  aud_20khz;
#endif

#if defined(_ENABLE_BEEP_)
	/*!BEEP 开关*/
	uint8 aud_beep;
#endif
#if defined(_SUPPORT_AREA_)
	/*区域设置*/
	uint8 area;//0 :欧洲 1:美洲
#endif
#if defined(_SEL_ADD_TIME_SELECT_)
	/*时钟显示格式*/
	uint8 rtc_disp;// /*12小时和24设置*/
#endif

	/*日期显示格式*/
	uint8 date_disp;
	/*开机运行TASK_NAME*/
	const void *task_name;
	const void *app_name;
#if defined(_ENABLE_RADIO_RDS_)
	/*!RDS设置状态*/
	rds_set_t rds;
#endif
	uint8 mono_flg;
	uint8 loc_flg;
	/*蓝牙音量*/
	uint8 bt_vol;
	uint8 a2dp_flag;
#if defined(_ENABLE_USB_DELAY_)
	uint8 name_flag;
	const void *name;
#endif

#if defined(_ENABLE_BT_POWER_)
	uint8 bt_talk;
	uint8 acc_off;
	const void *mode_name;
#endif

#if defined(_ENABLE_TALKING_VOL_)
	uint8 first_power;
	uint8 bt_vol_flg;
#endif
#if defined(_SUPPORT_SUBWOOFER2_)||defined(_ENABLE_AP3768_SUBWOOFER_)
	uint8 aud_sub;
#endif
#if defined(_SUPPORT_SUBWOOFER3_)||defined(_ENABLE_AP3768_SUBWOOFER_)
	uint8 aud_sub1;
#endif
#if defined(AUDIO_SUB_LEVEL_SET)
	uint8 aud_sub_ctrl;
#endif
#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
#if defined(_SUPPORT_PANEL_PWM_)
	uint8 pwm_r;
	uint8 pwm_g;
	uint8 pwm_b;
	uint8 pwm_speed;
#elif defined(_ENABLE_CHRAMATIC_CAMP_SHADE_)
	uint16 PanelControlR;
	uint16 PanelControlG;
	uint16 PanelControlB;
	uint8  Speed;
#endif
	uint8 Time;
	uint8 State;
	uint8 AutoState;
	uint8 save_clour;
#if defined(TD_ADD_NEW_APP)
	uint8 Auto_On_Off;
	uint8 clour;
	//uint8 save_clour;
#if defined(_ENABLE_RADIO_RDS_)
	uint8 pty_type;
#endif
#endif

#if defined(_ENABLE_LIGHT_SHADE_)
	uint8 red;
    uint8 green;
    uint8 blue;
	uint8 save_light;
	uint8 save_light1;
	uint8 save_light2;
#endif

#endif
#if defined(SUPPORT_LED_ADJUST)
	uint8 LedState;
#endif
#if defined(_ADJUST_A_B_SPK_)
	uint8 A_SPK;
	uint8 B_SPK;
#endif
#if defined(_MR889_SPECAIL_KEY_)
	uint8 CONTROL_SPK;
#endif
#if defined(_DISABLE_MEMORY_POWER_)||defined(_JTI_CHANGE_MODE_)
	uint8 acc_flag;
#endif
    uint8 usb_standby;
    uint8 sd_standby;
#if defined(DAB_CURRENT_STATION_MEMORY)
	uint8 DAB_CURRENT_STATION;
#endif
#if defined(DAB_NUM_SAVE_STATION)
	uint8 DAB_NUM_STATION[6];
#endif
#if defined(_SPECIAL_RADIO_INIT_)
    uint8 radio;
#endif
#if defined(DISABLE_TURN_BT_MODE)
	uint8 bt_flg;
	uint8 connect_time;
#endif
#if defined(_ENABLE_BT_MODE_MEMORY_)
	uint8 acc_bt;
	uint8 bt_mode;
	uint8 sd_start;
#endif
#if defined(_SPECIAL_CHANGE_)
	uint8 usb_out_delay;
#endif
#if defined(_DISC_OUT_DELAY_CHECK_)
	uint8 disc_out_delay;
#endif
#if defined(_ENABLE_ST_MONO_)
	uint8 st_flag;
#endif
#if defined(SUPPORT_ALARM_FUNC_)
	uint8 alarm;
    uint8 alarm_minute;
    uint8 alarm_hour;
    uint8 alarm_second;
    uint8 alarm_minute1;//实时时间
    uint8 alarm_hour1;
    uint8 alarm_second1;

    uint16 alarm_time;
    uint16 alarm_count;
    uint8 alarm_power_flag;
    uint8 alarm_power_flag1;

    uint8 alarm_power;
#endif
#if defined(_ENABLE_ALARM_CLOCK_)
    uint8 alarm_beep;
	uint8 alarm_flag;
#endif
#if defined(_ENABLE_TIME_DISP_)
	uint8 time_disp;
#endif
#if defined(_ENABLE_AUX_DETECT_)||defined(_SUPPORT_MIC_AUX_)
    uint8 aux1;
    uint8 aux2;
    uint8 aux3;
    uint8 aux4;
    const char  *save_name;
#endif
#if defined(_SUPPORT_MIC_OK_)
	const char	*save_name;
#endif
#if defined(_ENABLE_PASSWORD_INPUT_)
	uint8 password;
	uint16 password_input;
	uint8 password_number;
#endif
#if defined(_SUPPORT_CAN_UART_)
	uint8 can_mode;
	uint8 can_speed;
#endif
#if defined(POWER_DISCONNECT_BT_)||defined(_ENABLE_BT_ON_OFF_)
	uint8 acc_off_flag;
	uint8 power_off_flag;
#endif
#if defined(_ENABLE_AUTO_POWER_)
	uint8 power_flag;
	const char	 *save_mode_name;
#endif
#if defined(_AUDIO_USE_DECODER_)||defined(_AUDIO_DRIVER_AP3768_)
	uint8 eq_mode;
#endif
#if defined(_AUDIO_DRIVER_AP3768_)
	//uint8 acc_ap3768;
#endif
#if defined(TD_ADD_SPP)
	uint32 usb_total_time;
	uint32 sd_total_time;
#endif
#if defined(_ENABLE_ACC_MODE_)
	uint8 playusb_flg;
	uint8 playsd_flg;
#endif
#if defined(_XM003_BT_NAME_)||defined(_YH1701_BT_NAME_)||defined(_JTI_BT_NAME_)
	uint8 acc_off;
	uint16 save_number;
    uint16 rand_number;
#endif

#if defined(DTH_USE_KEY1_DETECT)
	uint8 dth_detect;
#endif

#if defined(_CHANGE_DISP_MODE_)
	uint8 ad_keyflag;
#endif

#if defined(_SUPPORT_PLAYER_NAMES_)
    uint8 back_flag;
	const void *back_name;
#endif

#if defined(_ENABLE_IPHONE_CHARGE_)
	uint8 usb_detect;
	uint8 usb_detect1;
#endif
#if defined(_ENABLE_ACC_OFF_LOGO_)||defined(_DISABLE_BT_AUTO_CONNECT_)
	uint8 acc_on_flag;
#endif
#if defined(_ENABLE_TALK_VOL_)
	uint8 talk_volume;
#endif
#if defined(_ENABLE_BT_ON_OFF_)
	uint8 bt_on;
#endif
#if defined(_ACC_OFF_POWER_LOGO_)
	uint8 acc_logo;
#endif
#if defined(_ENABLE_ID3_ON_OFF_)||defined(_ENABLE_ID3_SET_)
	uint8 id3_on_flag;
#endif
#if defined(_ENABLE_UART_LOW_)
	uint8 ap3768_power_flag;
	uint8 ap3768_power_flag1;
#endif
#if defined(SD_FM_I2C_PIN_COMMON_USE)
	uint8 acc_off_flag;
#endif
#if defined(_XZC1668_RADIO_FUNTION_)
	uint8 fm_flag;
	uint8 fm_band;
	uint8 am_band;
#endif
#if defined(_ENABLE_MENU_FUNTION_)
    uint8 setup;
    uint8 default_vol;
    uint8 log;
#endif
#if defined(_ENABLE_DEFAULT_CONTROL_)||defined(ADD_ADJUST_DEFAULT_VOL)
	uint8 default_vol;
#endif
#if defined(_SUPPORT_POWER_ON_PASSWORD_)
	uint8 first_power_flag;
	uint8 password_flag;
	uint8 password_number;
	uint16 password_input;
#endif
#if defined(_SUPPORT_PASSWORD_DISPLAY_)
	uint8 first_power_flag;
	uint8 password_flag;
#if defined(_ENABLE_PASSWORD_SET_)
	uint8 pwd_set;
#endif
#endif
#if defined(_FY7555_CLOCK_ICON_DISPLAY_)
	uint8 time_flag;
#endif
#if defined(_ENABLE_BT_CONNECT_)
	uint8 acc_bt_flag;
	uint8 bt_connect_flag;
#endif
#if defined(ACC_OFF_REMEBER_PLAY_STATE)
	uint8 play_flag;
#endif
#if defined(_ENABLE_RDS_ON_OFF_)
	uint8 rds_onoff;
#endif
#if defined(_REMEBER_FLAG_STATE_)
    uint8 memory_play;
    uint8 rpt_flag;
    uint8 rdm_flag;
#endif
#if defined(_ACC_OFF_MEMORY_INT_)
    uint8 int_flag;
#endif
#if defined(_DISABLE_RADIO_LOGO_)
    uint16 radio_freq;//频率
    uint8 radio_band;//波段号
    uint8 radio_num;//台位号
#endif
#if defined(_ENABLE_TEMP_PWM_)
	uint8 temp_set;//温度档次设置
	uint8 warm_set;//加热设置
	uint32 warm_time;//解热时间倒计时
	uint8 warm_power;//加热开关
#endif
#if defined(_ENABLE_RPT_RDM_FUNTION_)
	uint8 rpt_flag1;
	uint8 rpt_start;
	uint8 rpt_start1;
	uint8 rdm_start;
	uint8 rdm_start1;
#endif
#if defined(_ENABLE_RPT_DIR_BUG_)
    uint8 rpt_flag;
	uint32 first_dir;
	uint32 last_dir;
#endif
#if defined(_YH1701_SPECIAL_IR_)
	uint8 ir_power;
	uint8 ir_power_time;
#endif
#if defined(_ENABLE_VOLUME_CONTROL_)
	uint8 zone1_start;
    uint8 zone2_start;
    uint8 zone3_start;
    uint8 zone4_start;
#endif
#if defined(_SUPPORT_SUB_)
	uint8 sub_start;
#endif
#if defined(_ENABLE_OPTICAL_CONTROL_)
	uint8 optical;
#endif
#if defined(_ENABLE_MODE_CHANNEL_)
	uint8 mode_start;
#endif
#if defined(_ENABLE_ST_MEMORY_)
	uint8 first_st_flag;
#endif
#if defined(_ENABLE_POWER_LOGO_)
	uint8 save_flag;
	const void *save_task_name;
	const void *save_task_name1;
#endif
#if defined(_ENABLE_POWER_INPUT_)
	uint8 first_input;
#endif
#if defined(ENABLE_SPECIAL_BT_)
    const void *last_name;
#endif
#if defined(_SUPPORT_TOUCH_KEY_)
	uint8 touch_init;
#endif
#if defined(_KEY_LIGHT_CONTROL_)
	uint8 light_flg;
#endif
#if defined(_JTI_POWER_LOGO_)
	uint8 band_index;
	uint8 ch_index;
#endif
#if defined(_JTI_CHANGE_MODE_)
    uint8 change_power_flag;
#endif
#if defined(_ACC_ON_LCD_BUG_)
	uint8 pt1622_flag;
#endif
#if defined(_ENABLE_BT_OFF_)
	uint8 bt_scan_flag;
	uint8 bt_first_scan;
#endif
#if defined(_ENABLE_CHARGE_MUTE_)
	uint8 accoff_flag;
#endif

}setup_variable_t;

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)

#if defined(_SUPPORT_PANEL_PWM_)

enum{
	PANEL_LIGHT_STATE_AUTO,
	PANEL_LIGHT_STATE_RED,
	PANEL_LIGHT_STATE_GREEN,
	PANEL_LIGHT_STATE_BLUE,
	PANEL_LIGHT_STATE_YELLOW,
	PANEL_LIGHT_STATE_FUCHSINE,
	PANEL_LIGHT_STATE_CYAN,
	PANEL_LIGHT_STATE_WHITE,
};

#define PANEL_LIGHT_FLASH_POLLIGN_TIME			(50)
#define LED_COLOR_LEVER_MAX						(20)

#define LED_COLOR_LEVER_MIN						(0)

#define LED_COLOR_SPEED 						(100)

#define GET_LED_COLOR_TIME						(setup.Time)
#define SET_LED_COLOR_TIME(x)					(GET_LED_COLOR_TIME = (x))

#define GET_PANEL_LIGHT_STATE					(setup.State)
#define SET_PANEL_LIGHT_STATE(x)				(GET_PANEL_LIGHT_STATE = (x))

#define GET_PANEL_AUTO_STATE					(setup.AutoState)
#define SET_PANEL_AUTO_STATE(x)					(GET_PANEL_AUTO_STATE = (x))

#elif defined(_ENABLE_CHRAMATIC_CAMP_SHADE_)

#if defined(WLS316_COLOP_REGSTER)
enum{
	PANEL_LIGHT_STATE_RED,
	PANEL_LIGHT_STATE_GREEN,
	PANEL_LIGHT_STATE_BLUE,
	PANEL_LIGHT_STATE_YELLOW,
	PANEL_LIGHT_STATE_CYAN,
	PANEL_LIGHT_STATE_FUCHSINE,
	PANEL_LIGHT_STATE_WHITE,
	PANEL_LIGHT_STATE_AUTO,
	PANEL_LIGHT_STATE_END
};
#else
enum{
	PANEL_LIGHT_STATE_GREEN,
	PANEL_LIGHT_STATE_BLUE,
	PANEL_LIGHT_STATE_RED,
	PANEL_LIGHT_STATE_YELLOW,
	PANEL_LIGHT_STATE_CYAN,
	PANEL_LIGHT_STATE_FUCHSINE,
	PANEL_LIGHT_STATE_WHITE,
	PANEL_LIGHT_STATE_AUTO,
	PANEL_LIGHT_STATE_END
};
#endif

#define PANEL_LIGHT_FLASH_POLLIGN_TIME			(50)

#define GET_LED_COLOR_TIME						(setup.Time)
#define SET_LED_COLOR_TIME(x)					(GET_LED_COLOR_TIME = (x))

#define GET_PANEL_LIGHT_STATE					(setup.State)
#define SET_PANEL_LIGHT_STATE(x)				(GET_PANEL_LIGHT_STATE = (x))

#define GET_PANEL_AUTO_STATE					(setup.AutoState)
#define SET_PANEL_AUTO_STATE(x)					(GET_PANEL_AUTO_STATE = (x))

#define GET_LED_COLOR_LEVER_R				    (setup.PanelControlR)
#define SET_LED_COLOR_LEVER_R(x)				(GET_LED_COLOR_LEVER_R = (x))

#define GET_LED_COLOR_LEVER_G					(setup.PanelControlG)
#define SET_LED_COLOR_LEVER_G(x)				(GET_LED_COLOR_LEVER_G = (x))

#define GET_LED_COLOR_LEVER_B					(setup.PanelControlB)
#define SET_LED_COLOR_LEVER_B(x)				(GET_LED_COLOR_LEVER_B = (x))

#define GET_LED_COLOR_SPEED						(setup.Speed)
#define SET_LED_COLOR_SPEED(x)					(GET_LED_COLOR_SPEED = (x)/PANEL_LIGHT_FLASH_POLLIGN_TIME)

#define LED_COLOR_R_LEVER_MAX					(31)
#define LED_COLOR_G_LEVER_MAX					(31)
#define LED_COLOR_B_LEVER_MAX					(31)

#if defined(_WLS316_PANEL_LIGHT_)
#define LED_COLOR_SPEED 						(250)
#else
#define LED_COLOR_SPEED 						(170)
#endif

#else

#define PANEL_LIGHT_FLASH_POLLIGN_TIME			(50)

#define SET_PANEL_LIGHT_TIME(x)					(setup.Time = x/PANEL_LIGHT_FLASH_POLLIGN_TIME)
#define GET_PANEL_LIGHT_TIME					(setup.Time)

#define PANEL_LIGHT_AUTO_TIME					3000

#define SET_PANEL_LIGHT_STATE(x)				(setup.State = x)
#define GET_PANEL_LIGHT_STATE					(setup.State)

#define SET_PANEL_AUTO_STATE(x)					(setup.AutoState = x)
#define GET_PANEL_AUTO_STATE					(setup.AutoState)

#if defined(_SUPPORT_THREE_LIGHT_)
#if defined(_SPECIAL_LIGHT_DISPLAY_)
enum{
	PANEL_LIGHT_STATE_BLUE,
	PANEL_LIGHT_STATE_FUCHSINE,
	PANEL_LIGHT_STATE_RED,
	PANEL_LIGHT_STATE_AUTO
};

#else
enum{
	PANEL_LIGHT_STATE_RED,
	PANEL_LIGHT_STATE_BLUE,
	PANEL_LIGHT_STATE_FUCHSINE,
	PANEL_LIGHT_STATE_AUTO
};
#endif

#else
enum{
	PANEL_LIGHT_STATE_BLUE,
	PANEL_LIGHT_STATE_GREEN,
	PANEL_LIGHT_STATE_CYAN,
	PANEL_LIGHT_STATE_RED,
	PANEL_LIGHT_STATE_FUCHSINE,
	PANEL_LIGHT_STATE_YELLOW,
	PANEL_LIGHT_STATE_WHITE,
	PANEL_LIGHT_STATE_AUTO
};
#endif
#endif
#endif

#if defined(SUPPORT_LED_ADJUST)
enum{
	BACK_LED_LOW,
	BACK_LED_MID,
	BACK_LED_HIGH,
	BACK_LED_NULL
};
#endif

#if defined(_ENABLE_RPT_RDM_FUNTION_)
enum{
	MUSIC_RPT_ALL = 0,
	MUSIC_RPT_ONE,
	MUSIC_RPT_FOLDER,
	MUSIC_RPT_MAX,
};
#endif

#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
typedef enum{
	PWD_NUMBER_1 =1,
	PWD_NUMBER_2,
	PWD_NUMBER_3,
	PWD_NUMBER_4,
#if !defined(_SUPPORT_PASSWORD_DISPLAY_)
	PWD_NUMBER_5,
	PWD_NUMBER_6,
	PWD_NUMBER_END,
#endif
}pwd_number;
#endif

#if defined(_SUPPORT_CALL_RECORDS_)
typedef struct{
	uint8	call_info;
	uint8	call_info_flag;
	uint8	call_info_exit;
	uint8   roll_time;
	uint8   call_roll;
	uint8 	call_ring;
	uint8 	call_num;
	uint8 	disp_length;
	uint8 	flag;
	uint8 	sum;
	uint8 	num_buffer[11][30];
}call_info;
#endif

#if defined(_SUPPORT_CAN_UART_)
typedef struct{
	uint32	speed;//速度
	uint32	total_distance;//总里程
	uint32	electric;//电量
	uint32	gears;//档位方向
	uint32	err;//故障代码
	uint32	disp;
}can_info;

enum{
	AUDIO_CAN_ERR = 0,
	AUDIO_CAN_SPEED,
	AUDIO_CAN_DISTANCE,
	AUDIO_CAN_ELECTRIC,
	AUDIO_CAN_GEARS,
	AUDIO_CAN_MAX,
};

enum{
	AUDIO_CAN_HIGH_SPEED = 1,
	AUDIO_CAN_MIDDLE_SPEED,
	AUDIO_CAN_LOW_SPEED,
	AUDIO_CAN_SPEED_MAX,
};
#endif

#if defined(_ENABLE_TEMP_PWM_)

enum{
	FUNC_SEL_TEMP_01 = 0,
	FUNC_SEL_TEMP_02,
	FUNC_SEL_TEMP_03,
	FUNC_SEL_TEMP_04,
	FUNC_SEL_TEMP_05,
	FUNC_SEL_TEMP_06,
	FUNC_SEL_TEMP_07,
	FUNC_SEL_TEMP_08,
	FUNC_SEL_TEMP_09,
	FUNC_SEL_TEMP_10,
	FUNC_SEL_TEMP_MAX,
};

enum{
	FUNC_SEL_WARM_30 = 0,
	FUNC_SEL_WARM_60,
	FUNC_SEL_WARM_ON,
	FUNC_SEL_WARM_MAX,
};
#endif

typedef struct{
	uint8 main_msg_disable;
	uint8 bt_call;
#if defined(_ENABLE_RADIO_RDS_)
	uint8 rds_ta;
	uint8 rds_af;
#endif
#if defined(_ENABLE_RDS_FIRST_)
	uint8 rds_mode;
#endif
	uint8 sys_normal;//系统工作OK标志 IDLE状态置起
	uint8 sys_power;
	/*静音*/
	uint8 aud_mute;
    uint8 usb_flg;
    uint8 scan_flg;
	uint8 set_freq_flag;
	uint8 ff_fb_flag;
	uint8 disc_detect;
#if defined(_FF_FB_SPECIAL_FUNCTION_)
    uint8 ff_name;
    uint8 fb_name;
#endif
	uint8 clk_flag;
#if defined(_SUPPORT_DISC_)
	void *disc_task_name;
	uint8 disc_open;
	uint8 disc_open_flag;
	uint8 disc_count;
	uint8 disc_time;
#endif
#if defined(TD_ADD_CHADD_SUB)
	uint8 save_radio_ch;
	uint8 SaveRadio;
    uint8 SaveRadioTime;
#endif
#if defined(_ENABLE_PAUSE_LIGHT_)
    uint8 pause_flg;
#endif

#if defined(_ENABLE_USB_SAVE_)||defined(_ENABLE_USB_DELAY_)||defined(_SLEEP_MODE_USB_DONT_DETCT)
	uint16 power_time;
	uint8 pwrstarttime;		//增加此标志 防止第一次上电关机状态插USB不检测
	uint16 usb_det_delay;
#endif
#if defined(SYS_MODE_CHANG_NOPP)
	uint8 app_mode_enable;
	uint8 music_pp_enable;
#endif
#if defined(FM_INIT_OK_FLAG)
	uint8 fm_init_ok_flag;
#endif
#if defined(_SUPPORT_TAPE_)
	uint8 tape_a_time;
	uint8 tape_b_time;
#endif
#if defined(_ENABLE_BAND_MUTE_)||defined(_ENABLE_POWER_MUTE_)||defined(_ENABLE_FIRST_POWER_MUTE_)
	uint8 mute_flag;
	uint8 mute_time;
#endif
#if defined(_CHANGE_MODE_1_)
	uint8 dab_detect_flag;
	uint8 dab_init_flag;
#endif
#if defined(_SUPPORT_CALL_RECORDS_)
	call_info info;
#endif

#if defined(_ENABLE_MODE_PLAY_)
    uint8 ndf2_flg;
#endif
#if defined(_BT_SPECIAL_MUTE_)
    uint8 bt_mute_flag;
#endif
#if defined(_ENABLE_ST_MONO_)||defined(_RADIO_FM_GATE_FYDVD_)||defined(_SI474X_REDUCE_AUD_GATE_)||defined(_ENABLE_RADIO_OUTPUT_)
	uint8 tuner_drive;
#endif
#if defined(_DISABLE_POWER_OFF_MUTE_)
	uint8 power_mute;
#endif
#if defined(_ENABLE_ALARM_CLOCK_)
	uint16 beep_time;
	uint16 beep_count;
#endif
#if defined(_CHANGE_DISP_MODE_)
    uint16 ad_keytime;
#endif

#if defined(SUPPORT_ALARM_FUNC_)
	uint8 time_flag;
	uint8 alarm_flag;
	uint8 beep_str_flag;
#endif
#if defined(_ENABLE_TIME_DISP_)
	int   timer_count;
	uint8 timer_flag;
	uint8 work_flag;
	uint8 work_flag1;
	uint8 rtc_time;
#endif
#if defined(_SUPPORT_CAN_UART_)
	can_info can;
#endif
#if defined(ST_READ_DELAY_)
	uint32 st_read_time;
	uint8 st_read_flag;
#endif
#if defined(_DISABLE_CALL_RING_)||defined(_ENABLE_PHONE_OUTGOING_)
	uint8 call_flag;
#endif
#if defined(_ENABLE_USB_MODE_)
	uint8 charge_flag;
#endif
#if defined(BT_PHONE_NUMBER)
	uint8 ring_flag;
#endif
#if defined(_ENABLE_RECORD_RADIO_)
	uint8 record_flag;
#endif
#if defined(_ENABLE_BT_NAME_)
	uint8 mac_name[6];
#endif
#if defined(_SPECIAL_MUTE_FLASH_)
	uint8 mute_time;
#endif
#if defined(_XM003_SPECIAL_SEEK_)
	uint8 seek_flag;
#endif
#if defined(_FY5517_POWER_OFF_TIME_)
	uint8 clock_flag;
#endif
#if defined(_ENABLE_IPHONE_CHARGE_)
	uint8 charge_time;
	uint8 charge_flg;
#endif
#if defined(_FY5517_TIME_DISPLAY_)
	uint8 time_set;
#endif

#if defined(_SUPPORT_AMP_CONTROL_)
	uint8 number_flag;
#endif
#if defined(_ENABLE_ACC_OFF_LOGO_)
	uint8 first_power_on;
#endif
#if defined(_SPECIAL_POWER_LOGO_)
	uint8 off_flag;
#endif
#if defined(_XM003_MUTE_FUNTION_)
	uint8 mute_seek_flag;
#endif
#if defined(_XM003_BT_VOL_)||defined(_HYM_BT_VOL_)
	uint8 bt_time;
#endif
#if defined(_SUPPORT_SPECIAL_KEY_FUNCTION_)
    uint8 AdKeyFlag;
#endif
#if defined(_ENABLE_RADIO_STO_)
	uint8 sto_flag;
#endif
#if defined(_ENABLE_TEL_MUTE_DETECT_)
	uint8 tel_mute_flag;
	uint8 tel_mute_flag1;
#if defined(_ENABLE_TEL_MUTE_FLASH_)
	uint8 tel_mute_time;
#endif
#endif

#if defined(_WJ361_BROWSE_MUTE_)
	uint8 browse;
	uint8 browse_flag;
#endif
#if defined(_ENABLE_TEA685x_INIT_)||defined(_ENABLE_TDA770x_INIT_)
	uint8 init;
#endif
#if defined(_DISABLE_ACC_OFF_DISPLAY_)||defined(_ACC_OFF_DISP_LOGO_)||defined(_ACC_OFF_POWER_LOGO_)||defined(_DISABLE_ACC_OFF_DISPLAY_CONNECT_)
	uint8 bt_off;
#endif
#if defined(_ENABLE_AMP_LIGHT_)
	uint8 amp_power;
#endif
#if defined(_ENABLE_A2DP_PLAY_)
	uint8 bt_start;
	uint8 a2dp_start;
#endif
#if defined(_ENABLE_EW730_AMS_)
	uint8 ams_flag;
#endif
#if defined(_SUPPORT_IPHONE_SIR_)
	uint8 sir_flag;
	uint8 sir_mode;
	uint8 sir_mode1;
	const void *name;
#endif
#if defined(_SPECIAL_PREV_FILE_)
	uint8 prev_flag;
#endif
#if defined(_ENABLE_DAB_SEEK_)
	uint8 dab_seek_flag;
#endif
#if defined(_ENABLE_BT_ON_OFF_)
	uint8 bt_on_off;//在蓝牙模式关掉蓝牙，显示不刷新
	uint8 bt_on_off1;
#endif
#if defined(_SPECIAL_LOGO_BUG_)
	uint8 logo_flag;
#endif
#if defined(_ENABLE_READ_ST_TIME_)
	uint8 st_time;
#endif
#if defined(_ENABLE_BEEP_)
	uint8 beep_time;
#if defined(SET_BEEP_FUNCV)
	uint8 beep_mute_flag;
#endif
#endif
#if defined(_ACC_OFF_POWER_LOGO_)
	disp_ctrl_t disp_ctr1;
#endif
#if defined(_IR_WITH_ACC_DELAY_)||defined(_ACC_OFF_DELAY_TIME_)
	uint8 acc_off_delay;
	uint8 acc_ir_flag;
#endif
#if defined(_AP3768_DETECT_VOL_)
	uint8 send_time;
	uint8 send_time1;
#endif
#if defined(_ENABLE_DSP_RESET_)
	uint8 reset_flag;
	uint8 reset_time;
	uint8 reset_first;
#endif
#if defined(_SEEK_REFRESH_FREQ_)
	uint8 pty_flag;
#endif
#if defined(TD_ADD_LCD_NUM_4)
	uint8 version;
	uint8 version1;
	uint8 version_time;
#endif
#if defined(_FY700B_ICON_CLOSE_)
	uint8 power_close;
#endif
#if defined(_ENABLE_RCA_MUTE_)
	uint8 sub_flag;
	uint8 sub_time;
	uint8 rca_time;
#if defined(_CLOSE_AMP_LIGHT_)
	uint8 rca_power;
#endif
#if defined(_DISABLE_AMP_OFF_)
	uint8 amp_flag;
#endif
#endif
#if defined(_ENABLE_DLASHING_LIGHT_)
	uint8 light_time;
#endif
#if defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_WLS316_SPECIAL_DISPLAY_)||defined(_SL6812A_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)||defined(SPECIAL_SLA6799_DISP)||defined(_ENABLE_PLAY_DISPLAY_)
	uint8 song_state;
#endif
#if defined(_ENABLE_MENU_SET_)
	uint8 mute_key_flag;
	uint8 menu_flag;
#endif
#if defined(_ENABLE_NEXT_PRVE_SET_)
	uint8 mute_key_flag;
#endif
#if defined(_AUDIO_DRIVER_AP3768_)
	uint8 power_mode_flag;
#endif
#if defined(_BT_PAUSE_MUTE_)
	uint8 bt_ring_flag;//关机状态来电无声音
	uint8 bt_ring_time;
#endif
#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
	uint8 amp_time;
	uint8 amp_on;
#endif
#if defined(_FY7555_VBAR_DISPLAY_)
	uint8 vbar_time;
#endif
#if defined(_ENABLE_TIME_CLEAN_)||defined(_XM007_TIME_CLEAN_)
	uint8 clean_time;
	uint8 logo_time;
#endif
#if defined(_POWER_OFF_SUB_)
	uint8 sub_power_flag;
	uint8 sub_power_time;
#endif
#if defined(_XZC1668_MODE_FUNTION_)
	uint8 fm_am_status;
#endif
#if defined(TD_ADD_NEW_APP)
	uint8 audio_flag;
#if defined(_ENABLE_A2DP_TIME_)
	uint8 a2dp_flag;
#endif
#if defined(_ENABLE_RADIO_RDS_)
	uint8 rds_info;//APP发过来的数据不做回传处理
	uint8 save_rds_af;
	uint8 save_rds_ta;
	uint8 save_rds_pty;
	uint8 save_rds_pty1;
	uint8 pty_type;
#endif
#endif
#if defined(_FAD_BAL_FUNTION_)
	uint8 app_data;
	uint8 app_reset;//复位的时候
#endif
#if defined(_ENABLE_MIC_REC_)
	uint8 mic_record;
#endif
#if defined(_SUPPORT_PLAYER_NAMES_)
    uint8 back_time;
#endif
#if defined(_ENABLE_MUTE_OFF_)
	uint8 mute_power_flag;
	uint8 mute_power_time;
#endif
#if defined(_ENABLE_BT_CONNECT_)
    uint8 bt_connect_time;
    uint8 bt_count;// 连接次数
#endif
#if defined(_ENABLE_BT_RDS_)
	uint8 bt_rds_flag;
	uint8 bt_rds_time;
#endif
#if defined(_DISABLE_SEL_VOLUME_)
	uint8 sel_flag;
#endif
#if defined(_ENABLE_RDS_ON_OFF_)
	uint8 rds_flag;
#endif
#if defined(_CHANGE_MUTE_DISP_)
	uint8 mute_disp;
#endif

#if defined(_SUPPORT_PASSWORD_DISPLAY_)
	uint8 err_flag;
#endif
#if defined(_SL6812_TEST_LCD_)
	uint8 lcd_flag;
	uint8 lcd_mode_flag;
#endif
#if defined(_ENABLE_PASSWORD_SET_)
	uint8 pwd_flag;
	uint8 pwd_set_flag;
	uint8 pwd_set_ok;
#if defined(_PASSWORD_TRUE_DISPLAY_)
	uint8 pwd_set_true;
#endif
#endif
#if defined(_NO_REMEMBER_PLAY_STATUS_)
    uint8 play_status_flag;
#endif
#if defined(_MODE_MEMORY_INT_)
    uint8 int_memory;
#endif
#if defined(_ENABLE_SEL_FUNTION_)
	uint8 sel_mode;
#endif
#if defined(_FEIYIN_RDS_SET_)
	uint8 sel_rds;
#endif
#if defined(_ENABLE_A2DP_TIME_)
	uint8 a2dp_sync;
	uint8 a2dp_count;
#endif
#if defined(_ENABLE_BT_ONOFF_DISPLAY_)
	uint8 bt_display;
	uint8 bt_disp_time;
#endif
#if defined(_AUX_MODE_BT_)
	uint8 mode_rca;
	//uint8 mode_rca_time;
#endif
#if defined(_ENABLE_PLAY_ERROR_)
	uint8 play_err;
#endif
#if defined(_ENABLE_AUTO_A2DP_)
	uint8 auto_a2dp;
	uint8 auto_a2dp_time;
#endif
#if defined(_ADD_SPECIAL_CH_DISP_)
    uint8 fm_disp;
    uint8 fm_time;
#endif
#if defined(_TEMP_WARM_DISPLAY_)
	uint8 temp_power_flag;
#endif
#if defined(_HKY1032_SPECIAL_LOGO_)
	uint8 temp_power_off;
#endif
#if defined(_ENABLE_VBAR_BLINK_)
	uint8 vbar_time;
#endif
#if defined(_POWER_OFF_BT_DEBUG_)
    uint8 bt_flag;
    uint8 phone_time;
#endif
#if defined(_BT_CALL_MEMORY_RPT_)
	uint8 memory_rpt;
#endif
#if defined(SUPPORT_ANTENNA_DISPLAY_)
	uint8 radio_type;
#endif
#if defined(_ENABLE_FOLD_TOTAL_)
	uint8 total_flag;
#endif
#if defined(_SPECIAL_MENU_FUNTION_)
	uint8 set_flag;
#endif
#if defined(_LIANKE_6803_POWER_)
	uint8 lp6803_time;
#endif
#if defined(_ENABLE_POWER_LOGO_)
	uint8 power_up_flag;
#endif
#if defined(_DISABLE_FM_ICON_)
	uint8 radio_stast;
#endif
#if defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 play_trk10;
	uint8 trk10_up;
#endif
#if defined(_ENABLE_BT_AUDIO_DISP_)
	uint8 next_flg;
	uint8 prev_flg;
	uint8 a2dp_time;
#endif
#if defined(_EVB_BT_CONNECT_BT_MODE_)
	uint8 bt_connect;
#endif
#if defined(_XM007_SPECIAL_BACKLIGHT_)
	uint8 off_flag;
	uint8 off_time;
#endif
#if defined(_XM007_SPECIAL_MUTE_)
	uint8 xm_mute_flag;
#endif
#if defined(_ENABLE_SUB_DETECT_)
	uint8 sub_flg;
#endif
#if defined(_ENABLE_BUG_DETECT_)
	uint8 bug_flag;
	uint8 bug_flag1;
	uint16 bug_time;
#endif
#if defined(_EVB_PAUSE_DISPLAY_)
	uint8 pause_flag;
#endif
#if defined(_ENABLE_POWER_BT_OFF_)
	uint8 bt_off_flag;
	uint8 connect_flag;
#endif
#if defined(_XIMA_SPECIAL_SEEK_)
	uint8 radio_seek;
#endif
#if defined(_SD_NO_FILE_DISPLAY_ERR_)
	uint8 music_error;
#endif
#if defined(_DISABLE_BT_MEMORY_)
	uint8 radio_flag;
#endif
#if defined(_DISABLE_VOL_SEEK_)
	uint8 seek_up_flag;
#endif
#if defined(_KLD3300_VERSION_POINT_)
	uint8 view_flag;
#endif
#if defined(_RDS_KEY_FUNTION_)
	uint8 reg_flag;
	uint8 eon_flag;
#endif
#if defined(_DIR_NAME_BUG_)
	uint8 first_flag;
#endif
#if defined(_SPECIAL_PAUSE_DISPLAY_)
	uint8 bt_first_flag;
#endif
#if defined(_DISABLE_AUTO_POWER_)
	uint8 power_flag;
#endif
}global_variable_t;

extern setup_variable_t setup;
extern global_variable_t global;
extern u8 gLast_bt_status;
extern u8 gDir_disp_flag;
extern u8 gEnterAccMode;


extern void setup_init(void);
extern void setup_exit(void);

extern uint8 get_cur_vol(void);
extern void set_cur_vol(uint8);

#if defined(_SUPPORT_TWO_PT2313_)
extern uint8 get_cur_vol2(void);
extern void set_cur_vol2(uint8);
#endif

#endif /* __SETUP_H__ */
