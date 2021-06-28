#ifndef __UI_API_H__
#define __UI_API_H__
#include "ui_res.h"


#define DISP_POLLING_TIME		(50) // 50ms
#define DISP_TYPE_POLLING		(100)

#define DISP_LEVEL_NORMAL		(0)
#define DISP_LEVEL_BACK			(1)
#define DISP_LEVEL_SETTING		(2)
#define DISP_LEVEL_MAX			(3)


#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
#if defined(_SUPPORT_THREE_LIGHT_)
enum{
	FUNC_SEL_RED = 0,
	FUNC_SEL_BLUE,
	FUNC_SEL_FUCHSINE,
	FUNC_SEL_AUTO,
};
#else
enum{
	FUNC_SEL_BLUE = 0,
	FUNC_SEL_GREEN,
	FUNC_SEL_CYAN,
	FUNC_SEL_RED,
	FUNC_SEL_FUCHSINE,
	FUNC_SEL_YELLOW,
	FUNC_SEL_WHITE,
	FUNC_SEL_AUTO,
};
#endif
#endif

#if defined(SUPPORT_LED_ADJUST)
enum{
	FUNC_SEL_LOW = 0,
	FUNC_SEL_MID,
	FUNC_SEL_HIGH,
};
#endif

typedef enum{
	FUNC_SEL_NULL = 0,
	
	FUNC_SEL_VOLUME,		//	音量设置
	FUNC_SEL_BASS,			//	低音设置
	FUNC_SEL_TREBLE,		//	高音设置
#if !defined(_FAD_BAL_FUNTION_)
	FUNC_SEL_BAL,			//	左右分离设置
	FUNC_SEL_FAD,			//	前后分离设置
#endif
	FUNC_SEL_LOUD,			//	响度开关设置
	FUNC_SEL_EQ,			//	音效设置
	FUNC_SEL_MUTE,
#if defined(_ENABLE_RPT_RDM_FUNTION_)
	FUNC_SEL_RPT,
	FUNC_SEL_RDM,
#endif
#if defined(_SPECIAL_TL2000_SET_)
	FUNC_SEL_A_VOLUME,
	FUNC_SEL_B_VOLUME,
#endif
#if defined(_ENABLE_TALK_VOL_)
	FUNC_SEL_TALK_VOL,
#endif
#if defined(_ENABLE_ID3_ON_OFF_)||defined(_ENABLE_ID3_SET_)
	FUNC_SEL_ID3,
#endif
#if defined(_ENABLE_BT_ON_OFF_)
	FUNC_SEL_BT_ON,
#endif
#if defined(_CHANGE_DEF_VOL_)
	FUNC_SEL_DEFVOLUME,
#endif

#if defined(_ENABLE_7419_MIDDLE_)
	FUNC_SEL_MIDDLE,
#endif

#if defined(_SUPPORT_SUBWOOFER_)||defined(_SUPPORT_SUBWOOFER_MODE2_)
	FUNC_SEL_SUBWOOFER,		//	重低音开关设置
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	FUNC_SEL_SUB_CUT_CTRL,
#endif

#if defined(_SUPPORT_SUBWOOFER2_)||defined(_ENABLE_AP3768_SUBWOOFER_)||defined(_SUPPORT_SUB_)
	FUNC_SEL_SUB,
#endif

#if defined(_SUPPORT_SUBWOOFER3_)||defined(_ENABLE_AP3768_SUBWOOFER_)
	FUNC_SEL_SUB1,
#endif

#if defined(_ENABLE_BEEP_)
	FUNC_SEL_BEEP,			//	峰鸣 开关设置
#endif

#if defined(_SUPPORT_AREA_)
    FUNC_SEL_AREA,			//	区域设置
#endif

#if defined(_SEL_ADD_TIME_SELECT_)
	FUNC_SEL_CLOCK,			//	12小时和24小时转换
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    FUNC_SEL_7COLOR,
#endif

#if defined(_ENABLE_RDS_ON_OFF_)
	FUNC_RDS_ONOFF,
#endif

#if defined(SUPPORT_LED_ADJUST)
    FUNC_SEL_LED,
#endif

#if defined(_ADJUST_A_B_SPK_)
    FUNC_SEL_A_SPK,
    FUNC_SEL_B_SPK,
#endif

#if defined(_MR889_SPECAIL_KEY_)
    FUNC_SEL_CONTROL_SPK,
#endif

#if defined(SUPPORT_ALARM_FUNC_)
	FUNC_SEL_ALARM,
#endif

#if defined(_ENABLE_TIME_DISP_)
	FUNC_SEL_TIME_DISP,
#endif

#if defined(_SUPPORT_CAN_UART_)
	FUNC_SEL_CAN_DISP,
	FUNC_SEL_MODE,
	FUNC_SEL_SPEED,
#endif

#if defined(_ENABLE_MENU_FUNTION_)
    FUNC_SEL_SETUP,
    FUNC_SEL_DEFAULT_VOL,
    FUNC_SEL_LOG,
#endif

#if defined(_ENABLE_DEFAULT_CONTROL_)||defined(ADD_ADJUST_DEFAULT_VOL)
	FUNC_SEL_DEFAULT_VOL,
#endif

#if defined(_FAD_BAL_FUNTION_)
	FUNC_SEL_FL,	//前左
	FUNC_SEL_FR,	//前右
	FUNC_SEL_RL,	//后左
	FUNC_SEL_RR,	//后右
#endif

#if defined(_SUPPORT_10_EQ_FUNTION_)
	FUNC_SEL_60HZ,
	FUNC_SEL_150HZ,
	FUNC_SEL_400HZ,
	FUNC_SEL_1KHZ,
	FUNC_SEL_2KHZ,
	FUNC_SEL_3KHZ,
	FUNC_SEL_7KHZ,
	FUNC_SEL_10KHZ,
	FUNC_SEL_15KHZ,
	FUNC_SEL_20KHZ,
#endif

/***************************************************/
	
#if defined(_ENABLE_RADIO_RDS_)
	FUNC_RDS_BEGIN,
	FUNC_RDS_AF,			//	AF 开关设置
	FUNC_RDS_TA,			//	TA 开关设置
	FUNC_RDS_REG,			//	AF 列表区域判断设置(可区别远近程电台列表)
	FUNC_RDS_EON,			//	EON 列表区域判断设置(可区别远近程区域列表)
	FUNC_RDS_L_S,			//	PI 搜索时间设置S: 30S，L: 90S
	FUNC_RDS_MASK,			//   屏蔽PI 判断设置
	FUNC_RDS_ALARM,			//	RDS 报警设置(当TA 信号来时，是报警还是转过去收听)
	FUNC_RDS_TIME,			//	RDS 时间同步设置
	FUNC_RDS_PTY,			//	RDS  搜索
#if defined(ENABLE_TA_VOL_SET)
	FUNC_RDS_TA_VOL,
#endif
	FUNC_RDS_END,
#endif
	
/***************************************************/
	
	FUNC_PRIVATELY_BEGIN,
	FUNC_SEL_LOC,			//	收音机搜台远近程开关设置
	FUNC_SEL_ST,			//	收音机调频立体声开关设置
	FUNC_PRIVATELY_END,
	
/***************************************************/
	FUNC_SEL_TIME,			//	时间设置
#if defined(SEL_SET_CLOCK_DISP)||defined(_ENABLE_SEL_MENU_TIME_)||defined(_MR9002_MENU_SET_)
	FUNC_SEL_CLOCK_DISP,
#endif	
	FUNC_SEL_MAX,
}func_sel_e;

typedef void *(*func_sel_check_f)(func_sel_e);

typedef enum{
	FUNC_TYPE_SEL = 0,
	FUNC_TYPE_FUNC,
}func_type_e;


typedef struct{
	int			timer;
#if defined(_FY5517_TIME_DISPLAY_)
	int			timer1;
#endif	
	uint8		front_level;
	uint8 		prev_level;
	uint8 		now_level;
	
	func_type_e	func_type;			//	设置选择类型
	func_sel_e	 func_sel;			//	设置选择功能
#if defined(_USER_TFT_SEND_)
	int			sel_timer;
#endif
#if defined(_ENABLE_CLOCK_DISPLAY_)||defined(_LCD_3_DIAPLAY_)
    uint8	playflg;
	uint8	talkflg;
#endif
}ui_disp_t;

extern ui_disp_t ui_disp;

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
#if defined(_SPECIAL_LOGO_DISPLAY_)||defined(_FY7555_LOGO_DISPLAY_)
extern void ui_show_idle_polling(void);
#endif

#if defined(_SPECIAL_POWER_OFF_DISPLAY_)
extern void ui_show_idle_polling1(void);
#endif

#if defined(_ACC_OFF_POWER_LOGO_)
extern void show_clear_num(uint8 level);
#endif

#if defined(_FY7555_LCD_DISPLAY_)
extern void ui_show_normal1(ui_lang_id_e);
extern void ui_show_string1(uint8, const uint8 *);
extern void ui_show_setting1(ui_lang_id_e, int32);
#endif

extern void show_clear_icon(uint8 level);
extern void ui_show_init(void);
extern void ui_show_copy(ui_lang_id_e, uint8 *);
extern void ui_show_string(uint8, const uint8 *);
extern void ui_show_num(uint8, uint8);
extern void ui_show_normal(ui_lang_id_e);
#if defined(_SUPPORT_CAN_UART_)
extern void ui_show_setting1(ui_lang_id_e, int32);
#endif
extern void ui_show_setting(ui_lang_id_e, int8);
extern void ui_show_mute(uint8);
#if defined(_SUPPORT_DISC_)
extern void ui_show_disc_eject(uint8);
extern void ui_show_no_disc(uint8);
#endif
extern void ui_show_vbar(uint8, uint8);
extern void ui_show_sel(func_sel_e);

#if defined(_SUPPORT_TWO_PT2313_)
extern void ui_show_sel1(func_sel_e);
#endif

extern void ui_show_rtc(const uint8 *, uint8);
extern void ui_show_com(const uint8 *, uint8);
#if defined(SUPPORT_ALARM_FUNC_)
extern void ui_show_rtc1(const uint8 *, uint8);
#endif

#if defined(_ENABLE_RADIO_RDS_)
extern void ui_show_rds_icon(int);
extern void ui_show_rds_txt(uint8);
#endif
#if defined(_SUPPORT_DISC_)
extern void ui_show_disc_icon(uint8);
#endif
extern void ui_show_usb_icon(uint8);
extern void ui_show_sd_icon(uint8);
extern void ui_show_bt_icon(uint8);
extern void ui_show_enable(uint8);
extern void ui_show_polling(void);

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
//	idle display
extern void ui_show_idle_init(void);
extern void ui_show_idle_polling(void);
extern void ui_show_idle_exit(void);
extern void ui_show_idle_play(void *);

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
//	play disc
#if defined(_SUPPORT_DISC_)
extern void ui_show_disc_init(uint8);
extern void ui_show_disc_exit(void);
extern void ui_show_disc_play(void *);
extern void ui_show_disc_polling(void);
#endif
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
//	radio display
extern void ui_show_radio_init(void);
extern void ui_show_radio_exit(void);
extern void ui_show_radio_polling(void);
extern void ui_show_radio_play(void *);
extern void ui_show_radio_st(uint8);
extern void ui_show_radio_loc(uint8);

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
//	play display
extern void ui_show_play_init(uint8);
extern void ui_show_play_exit(void);
extern void ui_show_play_polling(void);
extern void ui_show_play_play(void *);
extern void ui_show_play_cmd(int, void *);

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
//	bt display
extern void ui_show_bt_init(void);
extern void ui_show_bt_exit(void);
extern void ui_show_bt_play(void *);
extern void ui_show_bt_polling(void);

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
//	aux display
extern void ui_show_aux_init(void);
extern void ui_show_aux_exit(void);
extern void ui_show_aux_polling(void);
#if defined(_SUPPORT_AUX1_)
extern void ui_show_aux1_init(void);
extern void ui_show_aux1_exit(void);
extern void ui_show_aux1_polling(void);
#endif
extern void ui_show_rec_init(void);
extern void ui_show_rec_exit(void);
extern void ui_show_rec_polling(void);

#ifdef	TD_ADD_DISP_CHECKSUM
extern void ui_show_checksum(void);
#endif

#if defined(_ENABLE_AUX_VERSION_)||defined(_ENABLE_MODE_VERSION_)
extern void ui_show_aux_version(void);
#endif

#if defined(DAB_MODLE)
extern void ui_show_dab_init(void);
extern void ui_show_dab_exit(void);
extern void ui_show_dab_polling(void);
#endif

#if defined(_SUPPORT_TAPE_)
extern void ui_show_tape_init(void);
extern void ui_show_tape_exit(void);
extern void ui_show_tape_polling(void);
#endif

#if defined(_SUPPORT_MIC_OK_)
extern void ui_show_mic_init(void);
extern void ui_show_mic_exit(void);
extern void ui_show_mic_polling(void);
#endif

#if defined(_SUPPORT_CAN_MODEL_)
extern void ui_show_can_init(void);
extern void ui_show_can_exit(void);
extern void ui_show_can_polling(void);
#endif

#if defined(_SUPPORT_REVERSE_MODEL_)
extern void ui_show_reverse_init(void);
extern void ui_show_reverse_exit(void);
extern void ui_show_reverse_polling(void);
#endif

#if defined(EASYESY_APP_ONOFF)
extern void ui_APP_ONOFF(u8 onoff);
#endif

#ifdef	TD_ADD_NEW_APP
extern void ui_show_clean(void);
#endif

#endif  /*  _UI_API_H_  */

