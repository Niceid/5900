#include "sdk_cfg.h"
#include "bluetooth/avctp_user.h"
#include "key_drv.h"

#include "timer.h"
#include "dac/dac_api.h"
#include "setup.h"
#include "sys_detect.h"
#include "audio_drv/audio_drv.h"
#include "disp.h"

#include "bt.h"

#include "play_sel/play_sel.h"


u8 bt_init_ok=FALSE;
#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime;
extern void Spp_Tx_BT(void);
#endif

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);
extern u8 a2dp_get_status(void);

static bt_ctrl_t *bt_ctrl = NULL;
uint8 bt_task_work = FALSE;//进入蓝牙模式置TRUE  退出清为FLASE
#if defined(vbar_disp_bt_music)
u8 vbar_bt_music = 0;
#endif
#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_BT_Status(uint8 connect_status,uint8 call_status,uint8 music_status,uint8 sco_status);
extern void TFT_Tx_System_Status(void);
extern void TFT_Tx_BT_number_Display(uint8 *num,uint8 len,uint8 dial_flag);
#endif
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

static void bt_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{
		if((msg >= BT1_DISPLAY_BEGIN)&&(msg <= BT1_DISPLAY_END))
		{
			disp_send_msg(DISP_EVENT_RESET);
		}
		else if((msg >= BT2_DISPLAY_BEGIN)&&(msg <= BT2_DISPLAY_END))
		{
			disp_send_msg(DISP_EVENT_RESET);
		}
	}
}

static void bt_engine_msg(int msg, void *param)
{
	if(param != NULL)
	{
		os_taskq_post_msg(BTMSG_TASK_NAME, 2, msg, param);
	}
	else
	{
		os_taskq_post_msg(BTMSG_TASK_NAME, 1, msg);
	}
}

static bt_eng_state_e bt_eng_state(uint8 now_status, uint8 connect_status, uint8 call_status, uint8 music_status, uint8 sco_status)
{
	bt_printf("%s--->now_status:%d, connect_status:%d, call_status:%d, music_status:%d, sco_status:%d\n", __FUNCTION__, now_status, connect_status, call_status, music_status, sco_status);
	bt_eng_state_e eng_state = BT_ENG_STATUS_MAX;

#if defined(_ENABLE_A2DP_PLAY_)
	ew_printf("global.bt_start:%d\n",global.bt_start);
	ew_printf("global.a2dp_start:%d\n",global.a2dp_start);
	if(!global.bt_start)
	{
		puts("-------bt_eng_state---------\n");
		if((music_status == 2)&&(connect_status == 25)&&(global.a2dp_start))
		{
			setup.a2dp_flag = TRUE;
		}
	}
#endif

	switch(connect_status)
	{
		case BT_STATUS_INITING:				/*正在初始化*/
			switch(now_status)
			{
				case BT_STATUS_POWER_ON:	/*上电*/
    			case BT_STATUS_POWER_OFF:
					eng_state = BT_ENG_STATUS_POWER;
					break;
    			case BT_STATUS_INIT_OK:		/*初始化完成*/
				default:
					eng_state = BT_ENG_STATUS_INIT;
					break;
			}
			break;

		case BT_STATUS_WAITINT_CONN:		/*等待连接*/
			eng_state = BT_ENG_STATUS_DISCONNECT;
			break;

		case BT_STATUS_AUTO_CONNECTINT:		/*正在回连*/
			eng_state = BT_ENG_STATUS_AUTO_CONNECT;
			break;

		case BT_STATUS_CONNECTING:			/*已连接，没有电话和音乐在活动*/
			eng_state = BT_ENG_STATUS_IDLE;
			break;

	   	case BT_STATUS_TAKEING_PHONE:		/*正在电话*/
			switch(call_status)
			{
				case BT_CALL_INCOMING:	//电话打入
				puts("-------BT_CALL_INCOMING---------\n");
			    #if defined(_DISABLE_CALL_RING_)||defined(_ENABLE_PHONE_OUTGOING_)
			    	global.call_flag = FALSE;
			    #endif
					eng_state = BT_ENG_STATUS_RINGING;
	            	break;

	            case BT_CALL_OUTGOING:	//电话打出
					puts("-------BT_CALL_OUTGOING---------\n");
					eng_state = BT_ENG_STATUS_OUTGOING;

				#if defined(_DISABLE_CALL_RING_)||defined(_ENABLE_PHONE_OUTGOING_)
					global.call_flag = TRUE;
				#endif
					break;

				case BT_CALL_ACTIVE:	//接通电话
					if(sco_status)
					{
						eng_state = BT_ENG_STATUS_TALKING_HFP;
					}
					else
					{
						eng_state = BT_ENG_STATUS_TALKING_PHONE;
					}
					break;

				case BT_CALL_HANGUP:	//电话挂断
				     puts("-------BT_CALL_HANGUP---------\n");
			    #if defined(_DISABLE_CALL_RING_)||defined(_ENABLE_PHONE_OUTGOING_)
			    	global.call_flag = FALSE;
			    #endif
					eng_state = BT_ENG_STATUS_IDLE;
					break;
			}
			break;

	    case BT_STATUS_PLAYING_MUSIC:		/*正在音乐*/
			if(music_status == BT_MUSIC_STATUS_STARTING)
			{/*播歌状态*/

				eng_state = BT_ENG_STATUS_A2DP_PLAY;
				#if defined(vbar_disp_bt_music)
				vbar_bt_music = DISABLE;
				#endif
			#if defined(_USER_TFT_SEND_)
				TFT_Tx_System_Status();
				OSTimeDly(10);
			#endif
			}
			else
			{/*暂停状态*/
				eng_state = BT_ENG_STATUS_A2DP_PAUSE;
				#if defined(vbar_disp_bt_music)
				vbar_bt_music = ENABLE;
				#endif
			}
			break;
	}

#if 1
	bt_red_output("%s--->eng_status:%d, ", __FUNCTION__, eng_state);
		 if(eng_state == BT_ENG_STATUS_POWER)			bt_red_output("BT_ENG_STATUS_POWER\n");			/*上电等待状态*/
    else if(eng_state == BT_ENG_STATUS_INIT)			bt_red_output("BT_ENG_STATUS_INIT\n");			/*初始化状态*/
	else if(eng_state == BT_ENG_STATUS_DISCONNECT)		bt_red_output("BT_ENG_STATUS_DISCONNECT\n");	/*等待连接状态*/
    else if(eng_state == BT_ENG_STATUS_AUTO_CONNECT)	bt_red_output("BT_ENG_STATUS_AUTO_CONNECT\n");	/*自动回连状态*/
    else if(eng_state == BT_ENG_STATUS_LINKED)			bt_red_output("BT_ENG_STATUS_LINKED\n");		/*连接成功状态*/
    else if(eng_state == BT_ENG_STATUS_IDLE)			bt_red_output("BT_ENG_STATUS_IDLE\n");			/*空闲状态*/
    else if(eng_state == BT_ENG_STATUS_RINGING)			bt_red_output("BT_ENG_STATUS_RINGING\n");		/*来电状态*/
	else if(eng_state == BT_ENG_STATUS_OUTGOING)		bt_red_output("BT_ENG_STATUS_OUTGOING\n");		/*拨出电话状态*/
	else if(eng_state == BT_ENG_STATUS_TALKING_HFP)		bt_red_output("BT_ENG_STATUS_TALKING_HFP\n");	/*通话状态(声音在主机)*/
	else if(eng_state == BT_ENG_STATUS_TALKING_PHONE)	bt_red_output("BT_ENG_STATUS_TALKING_PHONE\n");	/*通话状态(声音在手机)*/
	else if(eng_state == BT_ENG_STATUS_A2DP_PAUSE)		bt_red_output("BT_ENG_STATUS_A2DP_PAUSE\n");	/*音乐播放暂停*/
	else if(eng_state == BT_ENG_STATUS_A2DP_PLAY)		bt_red_output("BT_ENG_STATUS_A2DP_PLAY\n");		/*音乐播放*/
	else bt_red_output("no default\n");
#endif

	return eng_state;
}

static void bt_sys_status(bt_eng_state_e eng_state)
{
	if(eng_state < BT_ENG_STATUS_MAX)
	{
		switch(eng_state)
		{
			case BT_ENG_STATUS_POWER:
			case BT_ENG_STATUS_INIT:
				bt_ctrl->work_state 	= BT_WORK_STATE_INIT;
				bt_ctrl->hfp_state 		= BT_HFP_STATE_NULL;
				bt_ctrl->aud_state 		= BT_HFP_AUD_NULL;
				bt_ctrl->a2dp_state 	= BT_A2DP_STATE_NULL;
				break;

			case BT_ENG_STATUS_DISCONNECT:
			case BT_ENG_STATUS_AUTO_CONNECT:
				bt_ctrl->work_state 	= BT_WORK_STATE_CONNECT;
				bt_ctrl->hfp_state 		= BT_HFP_STATE_NULL;
				bt_ctrl->aud_state 		= BT_HFP_AUD_NULL;
				bt_ctrl->a2dp_state 	= BT_A2DP_STATE_NULL;
				break;

			case BT_ENG_STATUS_LINKED:
				bt_ctrl->work_state 	= BT_WORK_STATE_LINKED;
				bt_ctrl->hfp_state 		= BT_HFP_STATE_IDLE;
				bt_ctrl->aud_state 		= BT_HFP_AUD_HOST;
				bt_ctrl->a2dp_state 	= BT_A2DP_STATE_NULL;
				break;

			case BT_ENG_STATUS_IDLE:
				bt_ctrl->work_state 	= BT_WORK_STATE_LINKED;
				bt_ctrl->hfp_state 		= BT_HFP_STATE_IDLE;
				bt_ctrl->aud_state 		= BT_HFP_AUD_HOST;
				bt_ctrl->a2dp_state 	= BT_A2DP_STATE_NULL;
				break;
			case BT_ENG_STATUS_RINGING:
				bt_ctrl->work_state 	= BT_WORK_STATE_LINKED;
				bt_ctrl->hfp_state 		= BT_HFP_STATE_RINGING;
				bt_ctrl->aud_state 		= BT_HFP_AUD_HOST;
				break;
			case BT_ENG_STATUS_OUTGOING:
				bt_ctrl->work_state 	= BT_WORK_STATE_LINKED;
				bt_ctrl->hfp_state 		= BT_HFP_STATE_OUTGOING;
				bt_ctrl->aud_state 		= BT_HFP_AUD_HOST;
				break;
			case BT_ENG_STATUS_TALKING_HFP:
				bt_ctrl->work_state 	= BT_WORK_STATE_LINKED;
				bt_ctrl->hfp_state 		= BT_HFP_STATE_TALKING;
				bt_ctrl->aud_state 		= BT_HFP_AUD_HOST;
				break;
			case BT_ENG_STATUS_TALKING_PHONE:
				bt_ctrl->work_state 	= BT_WORK_STATE_LINKED;
				bt_ctrl->hfp_state 		= BT_HFP_STATE_TALKING;
				bt_ctrl->aud_state 		= BT_HFP_AUD_PHONE;
				break;

			case BT_ENG_STATUS_A2DP_PAUSE:
				bt_ctrl->work_state 	= BT_WORK_STATE_LINKED;
				bt_ctrl->a2dp_state 	= BT_A2DP_STATE_PAUSE;
				break;
			case BT_ENG_STATUS_A2DP_PLAY:
				bt_ctrl->work_state 	= BT_WORK_STATE_LINKED;
				bt_ctrl->a2dp_state 	= BT_A2DP_STATE_PLAY;
				break;

			default:
				break;
		}

#if defined(_USER_TFT_SEND_)
		OSTimeDly(10);
		TFT_Tx_BT_Status(bt_ctrl->work_state,bt_ctrl->hfp_state,bt_ctrl->aud_state,bt_ctrl->a2dp_state);
		OSTimeDly(10);
#endif
	}
}

static void bt_disp_info(uint8 num_enable)
{
	bt_ctrl->info.work_state 	= bt_ctrl->work_state;
	bt_ctrl->info.hfp_state 	= bt_ctrl->hfp_state;
	bt_ctrl->info.aud_state 	= bt_ctrl->aud_state;
	bt_ctrl->info.a2dp_state 	= bt_ctrl->a2dp_state;

	if(bt_ctrl->hfp_state != BT_HFP_STATE_IDLE)
	{
		bt_ctrl->dial_flag = FALSE;
	}

	if(bt_ctrl->info.dial_flag != bt_ctrl->dial_flag)
	{
		bt_printf("bt_dial_flag:%d\n", bt_ctrl->dial_flag);
		bt_ctrl->info.dial_flag = bt_ctrl->dial_flag;
		if(bt_ctrl->dial_flag)
		{
			key_msg_select(KEY_SELECT_2NDF);
		}
		else
		{
			key_msg_select(KEY_SELECT_DEFAULT);
		}
	}

	if(num_enable)
	{
		if(bt_ctrl->tel_num_input)
		{
			bt_ctrl->tel_num_input = FALSE;
			bt_ctrl->info.num_length = bt_ctrl->tel_num_length;
			bt_ctrl->info.num_buffer = bt_ctrl->tel_num_buffer;
		}
		else
		{
			bt_ctrl->info.num_length = bt_ctrl->num_length;
			bt_ctrl->info.num_buffer = bt_ctrl->num_buffer;
		}
		#if defined(_USER_TFT_SEND_)
			OSTimeDly(10);
			TFT_Tx_BT_number_Display(bt_ctrl->info.num_buffer,bt_ctrl->info.num_length,bt_ctrl->dial_flag);
			OSTimeDly(10);
		#endif
	}
	else
	{
		bt_ctrl->info.num_length = 0;
		bt_ctrl->info.num_buffer = NULL;
	}
	disp_send_param(DISP_EVENT_BT_PLAY, &bt_ctrl->info);
}

#if defined(_BT_PAUSE_MUTE_)
static uint8 bt_connect1(void)
{
	switch(get_bt_connect_status())
	{
		case BT_STATUS_CONNECTING:
		case BT_STATUS_TAKEING_PHONE:
		case BT_STATUS_PLAYING_MUSIC:
			return 1;
	}
	return 0;
}
#endif

static void bt_time_polling(void)
{
	if(!bt_ctrl->init_flag)
	{
		if((++bt_ctrl->idle_timer) >= (50/50))
		{
			bt_ctrl->idle_timer = 0;
			bt_ctrl->init_flag = TRUE;
			bt_engine_msg(MSG_BT_STATE, NULL);
		}
		return;
	}

#if defined(_ENABLE_BT_RDS_)
	if(global.bt_rds_flag)
	{
		if((++global.bt_rds_time) == (1000/50))
		{
			audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RDS_ON, TRUE);
			global.bt_rds_flag = FALSE;
			global.bt_rds_time = 0;
		}
	}
	else
	{
		global.bt_rds_time = 0;
	}
#endif

#if defined(_BT_PAUSE_MUTE_)
#if defined(_ENABLE_RADIO_RDS_)
	if(!global.rds_ta)
#endif
	{
    	if(!bt_connect1())
    	{
    		sys_mute_ctrl(ENABLE, 0);
    	}
    	else
    	{
    		//ew_printf("get_bt_connect_status:%d\n",get_bt_connect_status());
    		//ew_printf("get_bt_newest_status:%d\n",get_bt_newest_status());
    		if(!global.aud_mute)
    		{
    			if((get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
    			||(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_COME))
    			{
    				//ew_puts("111111111111111\n");
    				sys_mute_ctrl(DISABLE, 50);
    			}
    			else if((get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)||(get_bt_connect_status() == BT_STATUS_CONNECTING))
    			{
    				//ew_puts("2222222222222222\n");
    				sys_mute_ctrl(ENABLE, 0);
    			}
    			else if((get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE))
    			{
    				//ew_puts("333333333333333\n");
    				if(!global.bt_ring_flag)
    				{
    					//ew_puts("4444444\n");
    					sys_mute_ctrl(DISABLE, 0);
    					global.bt_ring_time = 0;
    				}
    				else
    				{
    					//ew_puts("5555555\n");
    					global.bt_ring_time++;
    					if(global.bt_ring_time == 20)
    					{
    						global.bt_ring_flag = FALSE;
    						global.bt_ring_time = 0;
    					}
    				}
    			}
    		}
    	}
	}
#endif

	if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER))
	{
		bt_ctrl->hfp_state = BT_HFP_STATE_RINGING;
	}

	if(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
	{
		bt_ctrl->hfp_state = BT_HFP_STATE_OUTGOING;
	}

#if defined(_SUPPORT_CALL_RECORDS_)
	if(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE)
	{
    	if(global.info.call_roll)
    	{
    		key_msg_select(KEY_SELECT_2NDF);
    	}
    	else
    	{
    		if(bt_ctrl->dial_flag)
    		{
				key_msg_select(KEY_SELECT_2NDF);
			}
			else
			{
				key_msg_select(KEY_SELECT_DEFAULT);
			}
    	}
	}
#endif

	if(bt_ctrl->eng_now_state == BT_ENG_STATUS_LINKED)
	{
		if((++bt_ctrl->idle_timer) >= (3000/50))
		{
			bt_ctrl->idle_timer 	= 0;
			bt_ctrl->eng_prev_state = bt_ctrl->eng_now_state;
			bt_ctrl->eng_now_state	= BT_ENG_STATUS_IDLE;
			bt_sys_status(bt_ctrl->eng_now_state);
			bt_disp_info(FALSE);
		}
	}
	else
	{
		bt_ctrl->idle_timer = 0;
	}

	if(bt_ctrl->tel_prev_state != bt_ctrl->hfp_state)
	{
		bt_ctrl->tel_prev_state = bt_ctrl->hfp_state;
		bt_ctrl->tel_num_length = 0;
		if(bt_ctrl->dial_flag)
		{
			bt_ctrl->dial_flag = FALSE;
			bt_disp_info(FALSE);
		}
	#if defined(_SUPPORT_CALL_RECORDS_)
		if(global.info.call_info)
		{
			global.info.call_info = FALSE;
			bt_disp_info(FALSE);
		}
	#endif
	}
	if(bt_ctrl->dial_flag)
	{
		if(bt_ctrl->dial_timer)
		{
			bt_ctrl->dial_timer--;
		}
		else
		{
			bt_ctrl->dial_flag = FALSE;

			if(bt_ctrl->tel_num_length)
			{
				bt_ctrl->tel_num_length = 0;
				bt_ctrl->tel_num_input = TRUE;
				bt_disp_info(TRUE);
			}
			else
			{
				bt_disp_info(FALSE);
			}
		}
	}

#if defined(_ALARM_CLOCK_RING_)
	if(setup.alarm_beep)
	{
		ew_puts("setup.alarm_beep\n");
		if((bt_ctrl->a2dp_state == BT_A2DP_STATE_PLAY)&&(!bt_ctrl->play_state))
		{
			ew_puts("setup.alarm_beep111\n");
			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_PP, 1);
			setup.alarm_flag = TRUE;
			bt_ctrl->play_state = TRUE;
		}
	}
	else
	{
		if(setup.alarm_flag)
		{
			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_PP, 1);
			setup.alarm_flag = FALSE;
			bt_ctrl->play_state = FALSE;
		}
	}
#endif

    if(setup.a2dp_flag)
    {
		ew_puts("a2dp_flag1111\n");
		if((bt_ctrl->work_state == BT_WORK_STATE_LINKED)
	 	 &&(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE))
		{
			ew_puts("a2dp_flag2222\n");

		#if defined(_ENABLE_A2DP_PLAY_)
			if((++bt_ctrl->a2dp_timer) > (1500/50))
		#else
			if((++bt_ctrl->a2dp_timer) > (2000/50))
		#endif
			{
				ew_puts("a2dp_flag3333\n");
				bt_ctrl->a2dp_timer = 0;

				//if(a2dp_get_status() != BT_MUSIC_STATUS_IDLE)
				{
				    if(bt_ctrl->a2dp_state != BT_A2DP_STATE_PLAY)
					{
						ew_puts("a2dp_flag4444\n");
						os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_PP, 1);
					}
					else
					{
						ew_puts("a2dp_flag5555\n");
						setup.a2dp_flag = FALSE;
					}
				}
			}
		}
	}

#if defined(_ENABLE_AUTO_A2DP_)
	if(global.auto_a2dp)
	{
		if((++global.auto_a2dp_time) > (2000/50))
		{
			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_PP, 1);
			global.auto_a2dp = FALSE;
			global.auto_a2dp_time = 0;
		}
	}
#endif

#if defined(_XM003_BT_VOL_)||defined(_HYM_BT_VOL_)
    if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
    ||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
    ||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
    ||(get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)
    ||((get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK)&&(get_call_status() != BT_CALL_HANGUP))
    ||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
    ||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
	{
		if(!global.aud_mute)
		{
			global.bt_time++;
			//ew_printf("global.bt_time:%d\n",global.bt_time);
			if(global.bt_time <= 150)
			{
				audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
			}
			if(global.bt_time == 250)
			{
				global.bt_time = 200;
			}
		}
		else
		{
			dac_mute(1,1);
		}
	}
	else
	{
		global.bt_time = 0;
	}
#endif

#if !defined(_ENABLE_BT_VOL_)
#if defined(BT_TALK_VOL)
	//ew_printf("get_bt_newest_status:%d\n",get_bt_newest_status());
	//ew_printf("get_call_status:%d\n",get_call_status());

	if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
	||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
	||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
	||(get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)
	||((get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK)&&(get_call_status() != BT_CALL_HANGUP))
	||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
	||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
	{
		//ew_puts("setup.first_power\n");

	#if defined(_ENABLE_TALKING_VOL_)
		global.bt_call = TRUE;
		if(!setup.first_power)
		{
			setup.first_power = TRUE;
			audio_drv_ctrl(AUD_CTRL_VOLUME, setup.bt_vol, TRUE);
		}
		else
		{
			if(!setup.bt_vol_flg)
			{
				setup.bt_vol_flg = TRUE;
				audio_drv_ctrl(AUD_CTRL_VOLUME, setup.bt_vol, TRUE);
			}
		}
	#else
		global.bt_call = TRUE;

		//ew_printf("setup.aud_vol:%d\n",setup.aud_vol);
		//ew_printf("setup.bt_vol:%d\n",setup.bt_vol);

    #if defined(_AUDIO_USE_DECODER_)
    	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
	#elif defined(_ENABLE_TALK_VOL_)
		audio_drv_ctrl(AUD_CTRL_VOLUME, setup.talk_volume, TRUE);
    #else
    	audio_drv_ctrl(AUD_CTRL_VOLUME, setup.bt_vol, TRUE);
    #endif
	#endif
	}
	else
	{
    #if defined(_ENABLE_TALKING_VOL_)
    	global.bt_call = FALSE;
    	if(setup.bt_vol_flg)
    	{
    		setup.bt_vol_flg = FALSE;
    	}
    #else
    	global.bt_call = FALSE;

	#if defined(_AUDIO_USE_DECODER_)
		setup.bt_vol = get_cur_vol();
	#elif defined(_ENABLE_TALK_VOL_)
		setup.bt_vol = setup.talk_volume;
	#else
		setup.bt_vol = BT_TALK_VOL;
	#endif
    #endif

	//ew_puts("setup.first_power1111\n");

#if defined(_ENABLE_RADIO_RDS_)
#if defined(_ENABLE_RDS_VOLUME_)
	audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
#else
	if(global.rds_ta)
	{
		audio_drv_ctrl(AUD_CTRL_VOLUME, setup.rds.volume, TRUE);
	}
	else
	{
		audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
	}
#endif
#else
	//ew_printf("setup.aud_vol1111:%d\n",setup.aud_vol);
	audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
#endif
	}
#endif
#endif

#if defined(_ENABLE_RADIO_RDS_) && defined(_ENABLE_BT_FIRST_)
	if(bt_ctrl->work_state == BT_WORK_STATE_LINKED)
	{
		if((bt_ctrl->hfp_state == BT_HFP_STATE_RINGING)
		 ||(bt_ctrl->hfp_state == BT_HFP_STATE_OUTGOING)
		 ||(bt_ctrl->hfp_state == BT_HFP_STATE_TALKING))
		{
			if(!bt_ctrl->ta_disable)
			{
				bt_puts("bt_time_polling --- MSG_FMENGINE_RDS_TA_DSABLE --- TRUE\n");
				os_taskq_post_event("FMEngTask", 2, 0x3100, TRUE);
				bt_ctrl->ta_disable = TRUE;
			}
		}
		else
		{
			if(bt_ctrl->ta_disable)
			{
				bt_puts("bt_time_polling --- MSG_FMENGINE_RDS_TA_DSABLE --- FALSE\n");
				os_taskq_post_event("FMEngTask", 2, 0x3100, FALSE);
				bt_ctrl->ta_disable = FALSE;
			}
		}
	}
#endif
}

static void tel_num_ctrl(uint8 num)
{
	if(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE)
	{
		if(num == '-')
		{
			if(bt_ctrl->tel_num_length)
			{
				bt_ctrl->tel_num_length--;
				bt_ctrl->tel_num_input = TRUE;
			}
		}
		else
		{
			if(bt_ctrl->tel_num_length < sizeof(bt_ctrl->tel_num_buffer))
			{
				bt_ctrl->tel_num_buffer[bt_ctrl->tel_num_length++] = num;
				bt_ctrl->tel_num_input = TRUE;
			}
		}
	}
	else if((bt_ctrl->hfp_state == BT_HFP_STATE_OUTGOING)||	(bt_ctrl->hfp_state == BT_HFP_STATE_TALKING))
	{
		if((num == '*')||(num == '#')||((num >= '0')&&(num <= '9')))
		{
			bt_engine_msg(MSG_BT_DTMF, (void *)(int)num);

			if(bt_ctrl->tel_num_length < sizeof(bt_ctrl->tel_num_buffer))
			{
				bt_ctrl->tel_num_buffer[bt_ctrl->tel_num_length++] = num;
			}
			else
			{
				memcpy(bt_ctrl->tel_num_buffer, bt_ctrl->tel_num_buffer + 1, sizeof(bt_ctrl->tel_num_buffer) - 1);
				bt_ctrl->tel_num_buffer[bt_ctrl->tel_num_length] = num;
			}
			bt_ctrl->tel_num_input = TRUE;
		}
	}
}

static void tel_num_input(tel_key_e tel_key)
{
	uint8 num = 0;

	if(tel_key <= TEL_KEY_9)
	{
		num = (uint8)(tel_key + '0');
	}
	else if(tel_key == TEL_KEY_WELL)
	{
		num = '#';
	}
	else if(tel_key == TEL_KEY_STAR)
	{
		num = '*';
	}
	else if(tel_key == TEL_KEY_ADD)
	{
		num = '+';
	}
	else if(tel_key == TEL_KEY_DEL)
	{
		num = '-';
	}
	else return;

	tel_num_ctrl(num);

	if(bt_ctrl->tel_num_input)
	{
		bt_disp_info(TRUE);
	}
}

#if defined(_SUPPORT_CALL_RECORDS_)
static uint8 bt_state(void)
{
	switch(get_bt_connect_status())
	{
		case BT_STATUS_CONNECTING:
		case BT_STATUS_TAKEING_PHONE:
		case BT_STATUS_PLAYING_MUSIC:
			return 1;
	}
	return 0;
}
#endif

static void bt_timer(void *param)
{
	os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_TIMER);
}

#ifdef	TD_ADD_ALEN
u8 gLast_bt_status=BT_STATUS_INIT_OK;
#endif

#if defined(_ENABLE_A2DP_TIME_)
extern void send_bt_id3(u8 type);
#endif

static void bt_user_task(void *p)
{
	int msg[3];
	OS_SEM *sem;

	bt_ctrl = (bt_ctrl_t *)malloc_fun(bt_ctrl ,sizeof(bt_ctrl_t), 0);
	if(bt_ctrl == NULL)
	{
		bt_puts("bt_ctrl err\n");
        while(1);
	}

	bt_ctrl->eng_prev_state = bt_ctrl->eng_now_state = BT_ENG_STATUS_MAX;

    bt_puts("\n***********************BT TASK********************\n");

	disp_send_msg(DISP_EVENT_BT);

#if defined(_ENABLE_SPECIAL_MUTE_)
	sys_mute_ctrl(DISABLE, 1);
#elif defined(_LK8005_BT_MUTE_)
	sys_mute_ctrl(DISABLE, 2500);
#elif defined(_EVB_BT_MUTE_)
#if defined(BT_PHONE_NUMBER)
	if(global.ring_flag)
	{
		sys_mute_ctrl(DISABLE, 50);
		global.ring_flag = FALSE;
	}
	else
	{
		sys_mute_ctrl(DISABLE, 500);
	}
#else
	sys_mute_ctrl(DISABLE, 500);
#endif
#elif defined(_BT_SPECIAL_MUTE_)
	sys_mute_ctrl(DISABLE, 1);
#elif defined(_SUPPORT_AMP_CONTROL_)
	sys_mute_ctrl(DISABLE, 300);
#else
	sys_mute_ctrl(DISABLE, 50);
#endif

#if defined(_ENABLE_A2DP_TIME_)
	global.a2dp_sync = TRUE;
	global.a2dp_count = 0;
#endif

	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_ON, TRUE);
#if defined(JYL3861E_CHANNEL_PLAY_GATE)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#elif defined(_FY5517_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#elif defined(_HKY1032_REDUCE_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#elif defined(_XM007_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#elif defined(_ENABLE_CHANNEL_GAIN_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#else
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
#endif

#if defined(_SUPPORT_TWO_PT2313_)
    audio_drv_ctrl1(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_ON, TRUE);
    audio_drv_ctrl1(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#endif

#if defined(_DECODER_MUSIC_GAIN_)
	audio_drv_ctrl(AUD_CTRL_LR, setup.aud_bal, TRUE);
	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
#endif

#if defined(_XM003_BT_VOL_)||defined(_HYM_BT_VOL_)
	global.bt_time = 0;
#endif

#if defined(_ENABLE_POWER_LOGO_)
	global.power_up_flag = FALSE;
#endif

#if defined(_DISABLE_BT_MEMORY_)
	global.radio_flag = FALSE;
#endif

#if defined(_AUX_MODE_BT_)
	if(global.mode_rca)
	{
		global.mode_rca = FALSE;
		DSP_RCA_MUTE_OFF();
	}
#endif

#if defined(_ENABLE_TONE_PLAY_)
	#if defined(USB_SD_DIS_TONE_PLAY)
		extern u8 USB_SD_DIS_TONE_PLAY_FLAG;
		USB_SD_DIS_TONE_PLAY_FLAG=0;
	#endif

    if(get_bt_connect_status()!=BT_STATUS_TAKEING_PHONE)
    {
		sys_mute_ctrl(DISABLE, 0);
	    tone_play_by_name(BT_TASK_NAME, 1, BPF_BT_MP3);
	}
	os_time_dly(150);	 //此处的延时是防止报提示音时又播蓝牙音乐，这样会内存不够死机复位!!!!!!!
#endif


#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(bt_timer, NULL, 100);//BT_POLLING_TIME
#else
	timer_reg_func(bt_timer, NULL, 50);//BT_POLLING_TIME
#endif
	os_taskq_post_event(BTMSG_TASK_NAME, 2, MSG_BT_ENGINE, ENABLE);

	bt_task_work = TRUE;
#ifdef	TD_ADD_ALEN
	if((bt_ctrl->a2dp_state != BT_A2DP_STATE_PLAY)
	&&(gLast_bt_status ==BT_STATUS_PLAYING_MUSIC))
	{
		gLast_bt_status =BT_STATUS_INIT_OK;
	#if !defined(DISENABLE_BT_PLAY_LASTA2DP_STATUS)
		bt_engine_msg(MSG_BT_PP, NULL);
	#endif
	}
#endif
#if defined(_ENTER_BT_PLAY_)
	if(bt_ctrl->a2dp_state != BT_A2DP_STATE_PLAY)
	{
		bt_engine_msg(MSG_BT_PP, NULL);
	}
#endif
#ifdef	TD_ADD_SPP
	Spp_Tx_BT();
#endif
#if defined(_USER_TFT_SEND_)
	OSTimeDly(10);//转换太快蓝牙模式有时候转换不过来
	TFT_Tx_System_Status();
	OSTimeDly(10);
#endif

#if defined(_ENABLE_TIME_DISP_)
	global.timer_count = 4000;
#endif

    while(1)
    {
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
		clear_wdt();
		bt_reset_common(msg[0], msg[1]);

		sem = NULL;
        switch(msg[0])
		{
			case SYS_EVENT_DEL_TASK:
				bt_puts("bt--->SYS_EVENT_DEL_TASK\n");
				if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
				{
					bt_task_work = FALSE;
			    #if defined(_ENABLE_RADIO_RDS_) && defined(_ENABLE_BT_FIRST_)
			    	if(bt_ctrl->ta_disable)
			    	{
			    		os_taskq_post_event("FMEngTask", 2, 0x3100, FALSE);
			    		bt_ctrl->ta_disable = FALSE;
			    	}
			    #endif

				#if defined(_SUPPORT_CALL_RECORDS_)
					global.info.call_roll = FALSE;
					global.info.roll_time = 0;
					global.info.call_info_flag = TRUE;
				#endif

				#if defined(_XM003_BT_VOL_)||defined(_HYM_BT_VOL_)
					global.bt_time = 0;
				#endif

					ew_puts("a2dp_flag666\n");
					if((bt_ctrl->work_state == BT_WORK_STATE_LINKED)
					 &&(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE))
					{
						ew_puts("a2dp_flag777\n");
						if(a2dp_get_status() != BT_MUSIC_STATUS_IDLE)
						{
							ew_puts("a2dp_flag888\n");
							if(bt_ctrl->a2dp_state == BT_A2DP_STATE_PLAY)
							{
								ew_puts("a2dp_flag999\n");
								if(is_auto_mute())
								{
									ew_puts("a2dp_flag000\n");
									setup.a2dp_flag = FALSE;
								}
								else
								{
									ew_puts("a2dp_flag000111\n");
									setup.a2dp_flag = TRUE;
								}
							}
							else
							{
								ew_puts("a2dp_flag000222\n");
								setup.a2dp_flag = FALSE;
							}
						}
					}

					os_taskq_post_event(BTMSG_TASK_NAME, 2, MSG_BT_ENGINE, DISABLE);
					timer_unreg_func(bt_timer);

				#if !defined(_ENABLE_SPECIAL_MUTE_)
					sys_mute_ctrl(ENABLE, 0);
				#endif
				
			    #if defined(_ENABLE_BT_FIRST_)
			    #if defined(_AUDIO_DRIVER_AP3768_)
			    	if(global.rds_ta)
			    	{
			    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RDS_OFF, TRUE);
			    		OSTimeDly(10);
			    	}
			    #endif
			    #endif
				
					audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_OFF, TRUE);
				#if defined(_SUPPORT_TWO_PT2313_)
					audio_drv_ctrl1(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_OFF, TRUE);
				#endif

					free_fun((void**)&bt_ctrl);
					os_task_del_res_name(OS_TASK_SELF);
				}
				break;

			case MSG_BT_PP:
				bt_puts("bt--->MSG_BT_PP\n");

		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif

				if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)
					&&(get_bt_newest_status() != BT_STATUS_PHONE_OUT)
					&&(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE)
					&&(get_bt_connect_status() != BT_STATUS_TAKEING_PHONE))
				{
    			#if !defined(_WLS_SPECIAL_MUTE_)
				#if !defined(_PLAY_PAUSE_MUTE_)
    				sys_mute_ctrl(ENABLE, 0);//解决小米手机播放暂停杂音问题
    			#endif
				#endif
				#if defined(_ENABLE_TL_MUTE_)
					AUDIO_MUTE_ON();
				#endif
				
			    #if defined(_ENABLE_BT_AUDIO_DISP_)
			    	global.a2dp_time = 0;
			    #endif
					bt_engine_msg(MSG_BT_PP, NULL);
				}
				break;

			case MSG_BT_NEXT_FILE:
				bt_puts("bt--->MSG_BT_NEXT_FILE\n");
		    #if defined(_ENABLE_BT_AUDIO_DISP_)
		    	if((get_bt_connect_status() == BT_STATUS_CONNECTING)
		    	||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
		    	||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
		    	{
		    		global.next_flg = TRUE;
		    		global.a2dp_time = 15;
		    	}
		    #endif

				if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)
					&&(get_bt_newest_status() != BT_STATUS_PHONE_OUT)
					&&(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE)
					&&(get_bt_connect_status() != BT_STATUS_TAKEING_PHONE))
				{
				#if !defined(_NEXT_PREV_MUTE_)
    				sys_mute_ctrl(ENABLE, 0);
    			#endif
				#if defined(_ENABLE_TL_MUTE_)
					AUDIO_MUTE_ON();
    		    	bt_engine_msg(MSG_BT_NEXT_FILE, NULL);
    			#else
    		    	bt_engine_msg(MSG_BT_NEXT_FILE, NULL);
    			#if defined(_EVB_SPECIAL_MUTE_)
    				sys_mute_ctrl(DISABLE, 3000);
    			#else
				#if !defined(_NEXT_PREV_MUTE_)
    				sys_mute_ctrl(DISABLE, 2000);//解决小米手机上下曲杂音问题
    			#endif
				#endif
    			#endif
				}
				break;

			case MSG_BT_PREV_FILE:
				bt_puts("bt--->MSG_BT_PREV_FILE\n");
		    #if defined(_ENABLE_BT_AUDIO_DISP_)
		    	if((get_bt_connect_status() == BT_STATUS_CONNECTING)
		    	||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
		    	||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
		    	{
		    		global.prev_flg = TRUE;
		    		global.a2dp_time = 15;
		    	}
		    #endif

				if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)
					&&(get_bt_newest_status() != BT_STATUS_PHONE_OUT)
					&&(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE)
					&&(get_bt_connect_status() != BT_STATUS_TAKEING_PHONE))
				{
				#if !defined(_NEXT_PREV_MUTE_)
    				sys_mute_ctrl(ENABLE, 0);
    			#endif
				#if defined(_ENABLE_TL_MUTE_)
					AUDIO_MUTE_ON();
                	bt_engine_msg(MSG_BT_PREV_FILE, NULL);
				#else
                	bt_engine_msg(MSG_BT_PREV_FILE, NULL);
    			#if defined(_EVB_SPECIAL_MUTE_)
    				sys_mute_ctrl(DISABLE, 3000);
    			#else
				#if !defined(_NEXT_PREV_MUTE_)
    				sys_mute_ctrl(DISABLE, 2000);//解决小米手机上下曲杂音问题
    			#endif
				#endif
    			#endif
				}
				break;

			case MSG_BT_ANSWER_CALL:
				bt_puts("bt--->MSG_BT_ANSWER_CALL\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			#if defined(_SUPPORT_IR_DIAL_)
				if(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE)
				{
				#if defined(_SUPPORT_CALL_RECORDS_)&&0
    				if(global.info.call_roll)
    				{
    					bt_engine_msg(MSG_BT_CALL_ONE_NUMBER, &global.info.disp_length);
    					break;
    				}
    				else
				#endif
					{
    					if(bt_ctrl->tel_num_length)
    					{
    						bt_engine_msg(MSG_BT_CALL_NUMBER, &bt_ctrl->tel_num_length);
    						break;
    					}
    					else
    					{
    						bt_ctrl->dial_flag = !bt_ctrl->dial_flag;
    						bt_ctrl->dial_timer = (5000/50);
    						bt_disp_info(FALSE);
    						break;
    					}
					}
				}
			#endif
			
		    #if defined(_SPECIAL_BT_FUNTION_)
		    	if(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE)
		    	{
		    		if(bt_ctrl->tel_num_length)
		    		{
		    			bt_engine_msg(MSG_BT_CALL_NUMBER, &bt_ctrl->tel_num_length);
		    			break;
		    		}
		    	}
		    #endif

		    #if defined(_SUPPORT_CALL_RECORDS_)
			#if !defined(_SUPPORT_IR_DIAL_)
		    	if(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE)
		    	{
		    		if(global.info.call_roll)
		    		{
		    			bt_engine_msg(MSG_BT_CALL_ONE_NUMBER, &global.info.disp_length);
		    			break;
		    		}
		    	}
		    #endif
			#endif
            	bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
				break;

		#if defined(_SPECIAL_ANSWER_KEY_)
    		case MSG_BT_ANSWER:
    			bt_puts("bt--->MSG_BT_ANSWER\n");
				if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
            	{
    				if(get_call_status() == BT_CALL_INCOMING)
    				{
						bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
    				}
    				else
    				{
            			bt_engine_msg(MSG_BT_PREV_FILE, NULL);
    				}
        		}
				else
				{
					bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
				}
    			break;
		#endif
        
        #if defined(_SY5258_SPECIAL_ANSWER_REJECT_)
		case MSG_BT_ANSWER_CALL3:
			bt_puts("bt--->MSG_BT_ANSWER_CALL3\n");
			if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
			{
				if(get_call_status() == BT_CALL_INCOMING)
				{
					bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
				}
				else
				{
					bt_engine_msg(MSG_BT_NEXT_FILE, NULL);
				}
			}
			else
			{
				bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
			}
			break;
		case MSG_BT_CALL_REJECT3:
			bt_puts("bt--->MSG_BT_CALL_REJECT3\n");
			if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
			{
				if(get_call_status() == BT_CALL_INCOMING)
				{
					bt_engine_msg(MSG_BT_CALL_REJECT, NULL);
				}
				else
				{
					bt_engine_msg(MSG_BT_PREV_FILE, NULL);
				}
			}
			else
			{
				bt_engine_msg(MSG_BT_CALL_REJECT, NULL);
			}
			break;
        #endif
		
		#if defined(_SPECIAL_ANSWER_REJECT_)
    		case MSG_BT_ANSWER_CALL1:
    			bt_puts("bt--->MSG_BT_ANSWER_CALL1\n");
    			//bt_printf("get_bt_connect_status:%d\n",get_bt_connect_status());
    			//bt_printf("get_bt_newest_status:%d\n",get_bt_newest_status());
    			//bt_printf("get_call_status:%d\n",get_call_status());
    			if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
    			{
    				if(get_call_status() == BT_CALL_INCOMING)
    				{
    					bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
    				}
    				else
    				{
    					bt_engine_msg(MSG_BT_NEXT_FILE, NULL);
    				}
    			}
    			else
    			{
    				bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
    			}
    			break;
				
			case MSG_BT_CALL_REJECT1:
				bt_puts("bt--->MSG_BT_CALL_REJECT1\n");
    			//bt_printf("get_bt_connect_status11:%d\n",get_bt_connect_status());
    			//bt_printf("get_bt_newest_status11:%d\n",get_bt_newest_status());
    			//bt_printf("get_call_status11:%d\n",get_call_status());
				if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
				{
					if(get_call_status() == BT_CALL_INCOMING)
					{
						bt_engine_msg(MSG_BT_CALL_REJECT, NULL);
					}
					else
					{
						bt_engine_msg(MSG_BT_PREV_FILE, NULL);
					}
				}
				else
				{
					bt_engine_msg(MSG_BT_CALL_REJECT, NULL);
				}
				break;
		#endif

        #if defined(_EVB_ANSWER_REJECT_)||defined(_CD8800_EVB_ANSWER_REJECT_)
        	case MSG_BT_ANSWER_CALL2:
        		bt_puts("MSG_BT_ANSWER_CALL2\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
        		if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
        		{
        			if(get_call_status() == BT_CALL_INCOMING)
        			{
        				bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
        			}
        			else
        			{
        				bt_engine_msg(MSG_BT_PREV_FILE, NULL);
        			}
        		}
        		else
        		{
        			bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
        		}
        		break;
        	case MSG_BT_CALL_REJECT2:
        		bt_puts("MSG_BT_CALL_REJECT2\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
        		if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
        		{
        			if(get_call_status() == BT_CALL_INCOMING)
        			{
        				bt_engine_msg(MSG_BT_CALL_REJECT, NULL);
        			}
        			else
        			{
        				bt_engine_msg(MSG_BT_NEXT_FILE, NULL);
        			}
        		}
        		else
        		{
        			bt_engine_msg(MSG_BT_CALL_REJECT, NULL);
        		}
        		break;
        #endif

		#if defined(_ENABLE_KEY_REDIAL_)
            case MSG_BT_ANSWER_REDIAL:
            	bt_puts("bt--->MSG_BT_ANSWER_REDIAL\n");
			#if defined(_SUPPORT_CALL_RECORDS_)
    			if(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE)
    			{
					if(global.info.call_roll)
    				{
    					bt_engine_msg(MSG_BT_CALL_ONE_NUMBER, &global.info.disp_length);
    					break;
    				}
    			}
			#endif
            	bt_engine_msg(MSG_BT_ANSWER_REDIAL, NULL);
            	break;
		#endif
		
		#if defined(_YJ3993_ADKEY_CONTROL_)
    		case MSG_BT_ANSWER_REJECT:
    			bt_puts("bt--->MSG_BT_ANSWER_REJECT\n");
    			bt_engine_msg(MSG_BT_ANSWER_REJECT, NULL);
    			break;
		#endif

		#if defined(_SUPPORT_IPHONE_SIR_)
			case MSG_BT_SIR:
            	bt_puts("bt--->MSG_BT_SIR\n");
            	bt_engine_msg(MSG_BT_SIR, NULL);
            	break;
		#endif
		
	    #if defined(_SPECIAL_BT_FUNTION_)
	    	case MSG_BT_ANSWER1:
        		if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
        		{
        			if(get_call_status() == BT_CALL_INCOMING)
        			{
        				bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
        			}
        			else
        			{
        				bt_engine_msg(MSG_BT_PREV_FILE, NULL);
        			}
        		}
        		else
        		{
        			bt_engine_msg(MSG_BT_ANSWER_CALL, NULL);
        		}
	    		break;
	    	case MSG_BT_REJECT1:
	    		if(bt_ctrl->hfp_state == BT_HFP_STATE_IDLE)
	    		{
	    			bt_ctrl->dial_flag = !bt_ctrl->dial_flag;
	    			bt_ctrl->dial_timer = (5000/BT_POLLING_TIME);
	    			bt_disp_info(FALSE);
	    		}
	    		else
	    		{
	    			sys_mute_ctrl(ENABLE, 200);
	    			bt_engine_msg(MSG_BT_CALL_REJECT, NULL);
	    		}
	    		break;
	    #endif

			case MSG_BT_CALL_LAST_NO://回拨上一个电话
				bt_puts("bt--->MSG_BT_CALL_LAST_NO\n");
				bt_engine_msg(MSG_BT_CALL_LAST_NO, NULL);
				break;

			case MSG_BT_CALL_REJECT:
				bt_puts("bt--->MSG_BT_CALL_REJECT\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			#if defined(_DISABLE_MUTE_REJECT_)
				if(get_bt_connect_status() != BT_STATUS_PLAYING_MUSIC)
			#endif
				{
				#if !defined(_EVB_BT_REJECT_MUTE_)
					sys_mute_ctrl(ENABLE, 500);
				#endif
		    		bt_engine_msg(MSG_BT_CALL_REJECT, NULL);
				}
				break;

			case MSG_BT_CONNECT_CTL:
				bt_puts("bt--->MSG_BT_CONNECT_CTL\n");
				bt_engine_msg(MSG_BT_CONNECT_CTL, NULL);
				break;

		#if defined(_SUPPORT_CALL_RECORDS_)
    		case MSG_BT_INFO:
    			bt_puts("bt--->MSG_BT_INFO\n");
				if(bt_state())
				{
					global.info.call_info_flag = !global.info.call_info_flag;

					global.info.call_info = !global.info.call_info;

					if(global.info.call_info_flag)
					{
						global.info.call_info = FALSE;
						global.info.call_info_exit = TRUE;
					}
					bt_disp_info(FALSE);
				}
				break;

			case MSG_BT_INFO_ADD:
				bt_puts("bt--->MSG_BT_INFO_ADD\n");
				if(bt_state())
				{
					global.info.call_info = TRUE;

					if(global.info.flag)
					{
						global.info.sum++;
						if(global.info.sum> 10)
						{
						  	global.info.sum = 1;
						}
					}
					else
					{
						if(global.info.call_num > 1)
						{
							global.info.sum++;
							if(global.info.sum > global.info.call_num)
							{
								global.info.sum = 1;
							}
						}
						else
						{
							global.info.sum = 1;
						}
					}
					bt_disp_info(FALSE);
				}
				break;

			case MSG_BT_INFO_SUB:
				bt_puts("bt--->MSG_BT_INFO_SUB\n");
				if(bt_state())
				{
					global.info.call_info = TRUE;

					if(global.info.flag)
					{
						global.info.sum--;
						if(global.info.sum < 1)
						{
							global.info.sum = 10;
						}
					}
					else
					{
    					if(global.info.call_num > 1)
    					{
    						global.info.sum--;
    						if(global.info.sum < 1)
    						{
    							global.info.sum = global.info.call_num;
    						}
    					}
    					else
    					{
    						global.info.sum = 1;
    					}
					}
					bt_disp_info(FALSE);
				}
				break;
		#endif

	    #if defined(_TALKING_VOL_CHANGE_)
	    	case MSG_BT_VOL_CHANGE:
	    		bt_puts("bt--->MSG_BT_VOL_CHANGE\n");
	    		bt_engine_msg(MSG_BT_VOL_CHANGE, NULL);
	    		break;
	    #endif

            case MSG_BT_REDIAL:
            	bt_puts("bt--->MSG_BT_REDIAL\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
            	sys_mute_ctrl(ENABLE, 500);
				bt_engine_msg(MSG_BT_REDIAL, NULL);
            	break;

			case MSG_BT_0 ... MSG_BT_9:
			    {
			    	bt_printf("bt--->MSG_BT_%d\n", msg[0] - MSG_BT_0);
			    	tel_key_e tel_key = ((msg[0] - MSG_BT_0) + TEL_KEY_0);
			    	tel_num_input(tel_key);

					if(bt_ctrl->dial_flag)
					{
						bt_ctrl->dial_timer = (5000/50);
					}
			    }
				break;

			case MSG_BT_ADD:
				bt_puts("bt--->MSG_BT_ADD\n");
				tel_num_input(TEL_KEY_ADD);
				if(bt_ctrl->dial_flag)
				{
					bt_ctrl->dial_timer = (5000/50);
				}
				break;

			case MSG_BT_STAR:
				bt_puts("bt--->MSG_BT_STAR\n");
				tel_num_input(TEL_KEY_STAR);
				if(bt_ctrl->dial_flag)
				{
					bt_ctrl->dial_timer = (5000/50);
				}
				break;

			case MSG_BT_WELL:
				bt_puts("bt--->MSG_BT_WELL\n");
				tel_num_input(TEL_KEY_WELL);
				if(bt_ctrl->dial_flag)
				{
					bt_ctrl->dial_timer = (5000/50);
				}
				break;

			case MSG_BT_DEL:
				bt_puts("bt--->MSG_BT_DEL\n");
				tel_num_input(TEL_KEY_DEL);
				if(bt_ctrl->dial_flag)
				{
					bt_ctrl->dial_timer = (5000/50);
				}
				break;
			case MSG_BT_AUDIO_PHONE://音源自动切换到手机端 断开SCO
				bt_puts("bt--->MSG_BT_AUDIO_PHONE\n");
				bt_engine_msg(MSG_BT_AUDIO_PHONE, NULL);
				break;

			case MSG_BT_AUDIO_HOST://音源自动切换到机器端  连接SCO
				bt_puts("bt--->MSG_BT_AUDIO_HOST\n");
				bt_engine_msg(MSG_BT_AUDIO_HOST, NULL);
				break;

			case MSG_BT_AUDIO_CHANGE://在手机和机器间切换音源
				bt_puts("bt--->MSG_BT_AUDIO_CHANGE\n");
				bt_engine_msg(MSG_BT_AUDIO_CHANGE, NULL);
				break;

			case MSG_BT_DISCONNECT:
				bt_puts("bt--->MSG_BT_DISCONNECT\n");
				bt_engine_msg(MSG_BT_DISCONNECT, NULL);
				sem = (OS_SEM *)msg[2];//同步命令
				break;

			case MSG_BT_STATE:
				{
					bt_puts("bt--->MSG_BT_STATE\n");

					typedef struct{
						uint8 now_status;
						uint8 connect_status;
						uint8 call_status;
						uint8 music_status;
						uint8 sco_status;
					}bt_status_t;

					const bt_status_t *status = (bt_status_t *)msg[1];
					uint8 now_state = bt_eng_state(status->now_status, status->connect_status, status->call_status, status->music_status, status->sco_status);

					if(bt_ctrl->eng_now_state != now_state)
					{
						bt_ctrl->eng_prev_state = bt_ctrl->eng_now_state;
						bt_ctrl->eng_now_state 	= now_state;
						bt_sys_status(bt_ctrl->eng_now_state);
						bt_disp_info(FALSE);
					}
					bt_ctrl->init_flag = TRUE;
				}
				break;

			case MSG_BT_INCOM_NUM://来电号码
				{
					bt_puts("bt--->MSG_BT_INCOM_NUM\n");

					typedef struct{
						uint8 length;
						uint8 *buffer;
					}bt_number_t;

					bt_number_t *number = (bt_number_t *)msg[1];

					uint8 length  = number->length;
					if(length > sizeof(bt_ctrl->num_buffer))
					{
						length = sizeof(bt_ctrl->num_buffer);
					}
					memcpy(bt_ctrl->num_buffer, number->buffer, length);
					bt_ctrl->num_length = length;

					if(length)
					{
						bt_disp_info(TRUE);
					}
				}
				break;

			case MSG_BT_TIMER:
				bt_time_polling();
				break;

        #if defined(_ENABLE_A2DP_TIME_)
        	case MSG_BT_ID3:
        		puts("MSG_BT_ID3\n");
        		send_bt_id3(msg[1]);
        		break;
        #endif
			default:
				task_msg_default(msg[0], msg[1]);
				break;
		}

		if(sem != NULL)
		{//	同步消息
			os_sem_set(sem,0);
			os_sem_post(sem);
		}
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static uint8 btmsg_task = FALSE;
extern TASK_REGISTER(btstack_task_info);

static void bt_msg_task_init(void)
{
	bt_printf("bt_msg_task_init ---> btmsg_task:%d, name:%s\n", btmsg_task, btstack_task_info.name);
	if(!btmsg_task)
	{
		btstack_task_info.init((void *)btstack_task_info.name);
		btmsg_task = TRUE;
	}
}

static void bt_msg_task_exit(void)
{
	bt_puts("bt_msg_task_exit\n");
	if(btmsg_task)
	{
		btstack_task_info.exit();
		btmsg_task = FALSE;
	}
}

static void bt_task_init(void *priv)
{
    u32 err;

	bt_puts("bt_task_init\n");
	#if defined(vbar_disp_bt_music)
	vbar_bt_music = ENABLE;
	#endif


	//user_ctrl_prompt_tone_play(BT_STATUS_FIRST_CONNECTED, NULL);

    err = os_task_create(bt_user_task,
                         (void*)0,
                         TaskBtPrio,
                         50
					#if OS_TIME_SLICE_EN > 0
                         ,1
					#endif
                         ,BT_TASK_NAME);

    if(OS_NO_ERR == err)
    {
    	while(!bt_task_work)
		{
			OSTimeDly(1);
		}

    	bt_msg_task_init();
        key_msg_register(BT_TASK_NAME, KEY_PROJECT_TEL);
    }

	bt_puts("bt_task_init_finish\n");
}

static void bt_task_exit(void)
{
	bt_puts("bt_task_exit\n");

#if defined(vbar_disp_bt_music)
	vbar_bt_music = DISABLE;
#endif
#ifdef	TD_ADD_ALEN
	if(get_bt_connect_status() 	== BT_STATUS_PLAYING_MUSIC)
	{
		gLast_bt_status =BT_STATUS_PLAYING_MUSIC;
	}
#endif
    if(os_task_del_req(BT_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
    	key_msg_register(NULL, KEY_PROJECT_MAX);
		bt_msg_task_exit();

        os_taskq_post_event(BT_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        }
        while(os_task_del_req(BT_TASK_NAME) != OS_TASK_NOT_EXIST);
    }

	bt_puts("bt_task_exit_finish\n");
}

TASK_REGISTER(bluetooth_task_info) =
{
    .name = BT_TASK_NAME,
    .init = bt_task_init,
    .exit = bt_task_exit,
};

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

void bt_task_background_init(void)
{
	bt_puts("bt_task_background_init_start\n");
	u8 cnt=20;
	bt_init_ok =FALSE;
	bt_msg_task_init();
	while((!bt_init_ok)&&(cnt--))
	{
		clear_wdt();
		os_time_dly(10);
	}
	bt_msg_task_exit();
	bt_puts("bt_task_background_init_succeed\n");
}

uint8 bt_dial_flag(void)
{
	if(bt_ctrl == NULL)return FALSE;
	return bt_ctrl->dial_flag;
}

