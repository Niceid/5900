#ifndef __UI_RADIO_H__
#define __UI_RADIO_H__
//没有进入这里

#define RADIO_DISP_ST				_B0_
#define RADIO_DISP_STO				_B1_
#define RADIO_DISP_LOC				_B2_
#if defined(TD_ADD_BANDCH)
#define RADIO_DISP_CH				_B3_
#endif
#if defined(_WK5951CH_SPECIAL_DISPLAY_)
#define RADIO_DISP_CH1				_B4_
#endif

typedef struct{
	uint8	state;
	uint16 	freq;
	uint8 	band;
	uint8 	num;//收音频率当前的通道 CH
	uint8 	flag;//BIT0:st  BIT1:sto  BIT2:loc
	uint8	*psn;
}radio_disp_info_t;

typedef struct{
	radio_disp_info_t info;
	uint8 state_flag;
	uint8 psn_flag;
	uint8 psn[8];
#if defined(TD_ADD_BANDCH)
	uint8 ch_flag;
	uint8 ch_timer;
#endif
#if defined(_WK5951CH_SPECIAL_DISPLAY_)
	uint8 ch_flag1;
	uint8 ch_timer1;
#endif
}radio_disp_t;


#define RADIO_POLLING_TIME							DISP_TYPE_POLLING

#if defined(TD_ADD_BANDCH)
extern void ui_show_bandch(void);
#endif
#if defined(TD_ADD_CHADD_SUB)
extern void disp_add_sub(void);
#endif

#if defined(_ENABLE_VERSION_VIEW_)
#define VERSION_PASSWORD_SIZE			6
extern uint8 version_password[VERSION_PASSWORD_SIZE];
extern void ui_show_radio_version(uint8 sel);
#endif

#if defined(ENABLE_PASSWORD_RESET)
#define VERSION_PASSWORD_SIZE1			6
extern uint8 version_password1[VERSION_PASSWORD_SIZE1];
extern void ui_show_soft_reset(uint8 sel);
#endif

#endif  /*  __UI_RADIO_H__  */

