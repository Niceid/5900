#ifndef __BT_H__
#define __BT_H__


#define BT_POLLING_TIME								(100)
extern u8 bt_init_ok;

typedef enum
{
	BT_ENG_STATUS_POWER = 0,		//初始化状态
	BT_ENG_STATUS_INIT,

	BT_ENG_STATUS_DISCONNECT,		//等待连接状态
	BT_ENG_STATUS_AUTO_CONNECT,		//自动回连

	BT_ENG_STATUS_LINKED,			//连接成功状态

	BT_ENG_STATUS_IDLE,				//空闲状态
	BT_ENG_STATUS_RINGING,			//来电状态
	BT_ENG_STATUS_OUTGOING,			//拨出电话状态
	BT_ENG_STATUS_TALKING_HFP,		//通话状态(声音在主机)
	BT_ENG_STATUS_TALKING_PHONE,	//通话状态(声音在手机)

	BT_ENG_STATUS_A2DP_PAUSE,		//音乐播放暂停//10
	BT_ENG_STATUS_A2DP_PLAY,		//音乐播放

	BT_ENG_STATUS_MAX,
}bt_eng_state_e;

typedef enum{
	BT_WORK_STATE_INIT = 0,
	BT_WORK_STATE_CONNECT,
	BT_WORK_STATE_LINKED,
}bt_work_state_e;

typedef enum{
	BT_HFP_STATE_NULL = 0,
	BT_HFP_STATE_IDLE,
	BT_HFP_STATE_RINGING,
	BT_HFP_STATE_OUTGOING,
	BT_HFP_STATE_TALKING,
}bt_hfp_state_e;

typedef enum{
	BT_HFP_AUD_NULL = 0,
	BT_HFP_AUD_HOST,
	BT_HFP_AUD_PHONE,
}bt_hfp_aud_e;

typedef enum{
	BT_A2DP_STATE_NULL = 0,
	BT_A2DP_STATE_PAUSE,
	BT_A2DP_STATE_PLAY,
}bt_a2dp_state_e;

typedef enum{
	TEL_KEY_0 = 0,
	TEL_KEY_1,
	TEL_KEY_2,
	TEL_KEY_3,
	TEL_KEY_4,
	TEL_KEY_5,
	TEL_KEY_6,
	TEL_KEY_7,
	TEL_KEY_8,
	TEL_KEY_9,
	TEL_KEY_STAR,
	TEL_KEY_WELL,
	TEL_KEY_ADD,
	TEL_KEY_DEL,
}tel_key_e;

typedef struct{
	uint8	work_state;
	uint8	hfp_state;
	uint8	aud_state;
	uint8	a2dp_state;
	uint8	dial_flag;
	uint8	num_length;
	uint8 	*num_buffer;
}bt_info_t;

typedef struct{
	bt_info_t 		info;

	bt_eng_state_e  eng_now_state;
	bt_eng_state_e  eng_prev_state;
	bt_work_state_e work_state;
	bt_hfp_state_e	hfp_state;
	bt_hfp_aud_e	aud_state;
	bt_a2dp_state_e	a2dp_state;

	uint8			dial_flag;
	uint16			dial_timer;

	uint8			num_length;
	uint8 			num_buffer[30];

	uint8			init_flag;
	uint16			idle_timer;

	uint8			tel_prev_state;
	uint8			tel_num_input;
	uint8			tel_num_length;
	uint8			tel_num_buffer[30];

	uint16			a2dp_timer;
	
#if defined(_ENABLE_RADIO_RDS_)&&defined(_ENABLE_BT_FIRST_)
	uint8			ta_disable;
#endif
#if defined(_ALARM_CLOCK_RING_)
	uint8			play_state;
#endif
}bt_ctrl_t;
extern uint8 bt_task_work;
extern void bt_task_background_init(void);
extern uint8 bt_dial_flag(void);

#endif

