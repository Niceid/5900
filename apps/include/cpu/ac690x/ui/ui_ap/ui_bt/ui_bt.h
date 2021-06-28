#ifndef __UI_BT_H__
#define __UI_BT_H__
//enter here

#define BT_POLLING_TIME								DISP_TYPE_POLLING//100
#define BT_PHONE_BUFFER_MAX							(30 + 1)
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
#define BT_DISP_BUF_SIZE							(4)
#elif defined(_FY7000_SPECIAL_DISPLAY_)
#define BT_DISP_BUF_SIZE							(13)
#elif defined(_FY5517_SPECIAL_DISPLAY_)
#define BT_DISP_BUF_SIZE							(11)
#elif defined(_SEVEN_CHAR_DISPLAY_)
#define BT_DISP_BUF_SIZE							(7)
#elif defined(_XM003_SPECIAL_DISPLAY_)
#define BT_DISP_BUF_SIZE							(7)
#elif defined(_FIVE_CHAR_DISPLAY_)
#define BT_DISP_BUF_SIZE							(6)
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
#define BT_DISP_BUF_SIZE							(10)
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
#define BT_DISP_BUF_SIZE							(10)
#else
#define BT_DISP_BUF_SIZE							(8)
#endif
#define BT_DISP_USER_MAX							(BT_PHONE_BUFFER_MAX + 10)//30+1+10


enum{
	BT_DISP_STATUS_STATIC = 0,//静态显示
	BT_DISP_STATUS_ROLL,//滚动显示
#if defined(_SUPPORT_CALL_RECORDS_)
	BT_DISP_STATUS_ROLL1,//滚动显示
#endif
	BT_DISP_STATUS_FLASH,//闪烁显示
	BT_DISP_STATUS_TIME,
};


enum{
	BT_WORK_STATE_INIT = 0,
	BT_WORK_STATE_CONNECT,
	BT_WORK_STATE_LINKED,//蓝牙工作在已连接状态
};

enum{
	BT_HFP_STATE_NULL = 0,
	BT_HFP_STATE_IDLE,
	BT_HFP_STATE_RINGING,
	BT_HFP_STATE_OUTGOING,
	BT_HFP_STATE_TALKING,
};

enum{
	BT_HFP_AUD_NULL = 0,
	BT_HFP_AUD_HOST,
	BT_HFP_AUD_PHONE,
};

enum{
	BT_A2DP_STATE_NULL = 0,
	BT_A2DP_STATE_PAUSE,
	BT_A2DP_STATE_PLAY,
};


typedef struct{
	uint8	work_state;
	uint8	hfp_state;
	uint8	aud_state;
	uint8	a2dp_state;
	uint8	dial_flag;
	uint8	num_length;
	uint8 	*num_buffer;
}bt_disp_info_t;


typedef enum{
	BT_STATE_NULL = 0,
	BT_STATE_CONNECT,//未连接
	BT_STATE_IDLE,//已连接
	BT_STATE_RINGING,//来电响铃
	BT_STATE_OUTGOING,//去电
	BT_STATE_TALKING,//通话中
}bt_state_e;

typedef struct{
	uint8	work_state;
	uint8	hfp_state;
	uint8	aud_state;
	uint8	a2dp_state;
	uint8	dial_flag;
	
	bt_state_e 	now_state;
	bt_state_e 	prev_state;
	
	uint8	phone_num;
	uint8	phone_buffer[BT_PHONE_BUFFER_MAX];//30+1BYTE

	uint8	dial_enter;
	uint8	call_num;
	uint8	music_play;
	uint8	ring_num;
	uint8	dtmf_num;
	
	uint8 	change;
	uint8 	info_buf[BT_DISP_BUF_SIZE];//8 BYTE
	uint8 	disp_buf[BT_DISP_BUF_SIZE];//8 BYTE
	uint8 	disp_timer;
	uint8	disp_status;
	
	uint8 	flash_status;
	uint8	roll_status;
	uint8	roll_point;
	
	uint8	disp_length;
	uint8	disp_user[BT_DISP_USER_MAX];//30+1+10
	
    uint32	disp_time;
}bt_disp_ctrl_t;

#endif  /*  __UI_BT_H__  */

