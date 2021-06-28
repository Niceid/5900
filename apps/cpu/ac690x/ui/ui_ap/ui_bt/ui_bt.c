#include "sdk_cfg.h"
#include "ui_bt/ui_bt.h"
#include "bluetooth/avctp_user.h"
#include "timer.h"

#ifdef TD_ADD_SPP
extern u8 gRefresh_BT_Disp;
extern void Spp_Tx_bt_Info(uint8 playFlag);
#endif

static bt_disp_ctrl_t *bt_disp_ctrl = NULL;
#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_BT_Tel_Time(uint32 tel_time);
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

static void ui_show_bt_clean(void)
{
	show_clear_icon(DISP_LEVEL_NORMAL);
	show_clear_num(DISP_LEVEL_NORMAL);
#if defined(_XM003_ANSWER_ICON_)
	ui_drv_func.flash_disable(UI_ICON_ANSWER);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ANSWER);
#endif
#if defined(_LCD_3_DIAPLAY_)
   ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
#endif
#if defined(_ENABLE_PAUSE_LIGHT_)
	global.pause_flg = FALSE;
#endif

}

#if defined(_KLD3300_SPECIAL_DISPLAY_)
static uint8 bt_connect_state4(void)
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

void ui_show_bt_init(void)
{
	bt_disp_ctrl = (bt_disp_ctrl_t *)malloc_fun(bt_disp_ctrl ,sizeof(bt_disp_ctrl_t), 0);
	if(bt_disp_ctrl == NULL)
	{
		ui_puts("ui_show_bt_init bt_disp_ctrl->>>err\n");
        while(1);
	}

	ui_puts("ui_show_bt_init\n");

	ui_show_bt_clean();
#if defined(_XM003_BT_DISP_)
	if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
	||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
	||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
	||(get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK))
	{
		ui_show_setting(LANG_ID_BT_OUTGOING, 0);
		ui_disp.timer = 1000;
		ui_show_normal(LANG_ID_BT_OUTGOING);
	}
	else
	{
		ui_show_setting(LANG_ID_BLUETOOTH, 0);
		ui_disp.timer = 1000;
		ui_show_normal(LANG_ID_BLUETOOTH);
	}
#elif defined(_SUPPORT_IPHONE_SIR_)
	if(global.sir_mode)
	{
		ui_show_setting(LANG_ID_SIR, 0);
		ui_disp.timer = 1000;
		ui_show_normal(LANG_ID_SIR);
	}
	else
	{
		ui_show_setting(LANG_ID_BLUETOOTH, 0);
		ui_disp.timer = 1000;
		ui_show_normal(LANG_ID_BLUETOOTH);
	}
#elif defined(_KLD3300_SPECIAL_DISPLAY_)
	if(bt_connect_state4())
	{
		ui_show_setting(LANG_ID_BT_CONNECTD, 0);
		ui_disp.timer = 1000;
		ui_show_normal(LANG_ID_BT_CONNECTD);

	}
	else
	{
		ui_show_setting(LANG_ID_BT_CONNECT, 0);
		ui_disp.timer = 1000;
		ui_show_normal(LANG_ID_BT_CONNECT);
	}
#else
#if !defined(HuiNuo_6651D_Copy)
#if !defined(_SUPPORT_LCM_1618_DRIVE)
	ui_show_setting(LANG_ID_BLUETOOTH, 0);
#endif
	ui_disp.timer = 1000;
	ui_show_normal(LANG_ID_BLUETOOTH);
#endif
#endif

#if defined(_FY7555_LCD_DISPLAY_)
	ui_show_setting1(LANG_ID_BLUETOOTH, 0);
	ui_show_normal1(LANG_ID_BLUETOOTH);
#endif
}

void ui_show_bt_exit(void)
{
	ui_puts("ui_show_bt_exit\n");

	ui_show_bt_clean();

	if(bt_disp_ctrl != NULL)
	{
		free_fun((void**)&bt_disp_ctrl);
	}
}

void ui_show_bt_play(void *info)
{
	if(bt_disp_ctrl == NULL)return;
	ui_puts("ui_show_bt_play\n");

	bt_disp_info_t *disp_info = (bt_disp_info_t *)info;

	bt_disp_ctrl->work_state	= disp_info->work_state;
	bt_disp_ctrl->hfp_state		= disp_info->hfp_state;
	bt_disp_ctrl->aud_state		= disp_info->aud_state;
	bt_disp_ctrl->a2dp_state	= disp_info->a2dp_state;
	bt_disp_ctrl->dial_flag		= disp_info->dial_flag;

	if(disp_info->num_buffer != NULL)
	{//来电号码 执行这里
		uint8 length = disp_info->num_length;

		if(length > (sizeof(bt_disp_ctrl->phone_buffer) - 1))
		{
			length = sizeof(bt_disp_ctrl->phone_buffer) - 1;
		}

		memcpy(bt_disp_ctrl->phone_buffer, disp_info->num_buffer, length);
		bt_disp_ctrl->phone_buffer[length] = 0;
		bt_disp_ctrl->phone_num = TRUE;
	}
}

#if defined(TD_ADD_NEW_APP)
u8 a2dp_staust=2;
extern void Spp_Tx_Bt_Staust(void);
#endif
void ui_show_bt_polling(void)
{
	if(bt_disp_ctrl == NULL)return;

#if defined(_ACC_OFF_POWER_LOGO_)
	if(global.bt_off)return;
#endif

#if defined(_ENABLE_A2DP_TIME_)
	if((a2dp_staust != global.a2dp_flag)||(global.a2dp_sync))
	{
		a2dp_staust = global.a2dp_flag;
		global.a2dp_sync = FALSE;
		Spp_Tx_Bt_Staust();
	}
#endif

#if defined(_SPECIAL_PAUSE_DISPLAY_)
	if((get_bt_connect_status() == BT_STATUS_CONNECTING)
	||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
	||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
	{
		if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
		{
			global.bt_first_flag = TRUE;
		}
	}
	else
	{
		global.bt_first_flag = FALSE;
	}
#endif

#if defined(_ENABLE_BT_AUDIO_DISP_)
	//ew_printf("bt_disp_ctrl->a2dp_state:%d\n",bt_disp_ctrl->a2dp_state);

	if((get_bt_connect_status() == BT_STATUS_CONNECTING)
	||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
	||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
	{
    	if(global.a2dp_time)
    	{
    		global.a2dp_time--;
    	}
    	
    	if(global.next_flg)
    	{
    		ui_show_setting(LANG_ID_BT_NEXT, 0);
    		ui_disp.timer = 1500;
    		global.next_flg = FALSE;
    		global.prev_flg = FALSE;
    	}
    	else if(global.prev_flg)
    	{
    		ui_show_setting(LANG_ID_BT_PREV, 0);
    		ui_disp.timer = 1500;
    		global.prev_flg = FALSE;
    		global.next_flg = FALSE;
    	}
	}
#endif

#if defined(_ENABLE_CLOCK_DISPLAY_)
    if(bt_disp_ctrl->now_state == BT_STATE_TALKING)
    {
		ui_disp.talkflg = TRUE;
	}
	else
	{
		ui_disp.talkflg = FALSE;
	}
#endif

#if defined(_ENABLE_BT_ON_OFF_)
	if(global.bt_on_off != setup.bt_on)
	{
		global.bt_on_off = setup.bt_on;
		global.bt_on_off1 = TRUE;
	}
#endif

#if defined(_XM003_ANSWER_ICON_)
	if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
	||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
	||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
	||((get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK)&&(get_call_status() == BT_CALL_OUTGOING))
	||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
	||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
	{
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_ANSWER);
	}
	else if(get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)
	{
		ui_drv_func.flash_disable(UI_ICON_ANSWER);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_ANSWER);
	}
	else
	{
		ui_drv_func.flash_disable(UI_ICON_ANSWER);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ANSWER);
	}
#endif

	if(bt_disp_ctrl->work_state < BT_WORK_STATE_LINKED)
	{//蓝牙工作在未连接状态
		bt_disp_ctrl->now_state = BT_STATE_CONNECT;
	}
	else
	{//蓝牙工作在已连接状态
		switch(bt_disp_ctrl->hfp_state)
		{
			case BT_HFP_STATE_RINGING:
				bt_disp_ctrl->now_state = BT_STATE_RINGING;
				break;
			case BT_HFP_STATE_OUTGOING:
				bt_disp_ctrl->now_state = BT_STATE_OUTGOING;
				break;
			case BT_HFP_STATE_TALKING:
				bt_disp_ctrl->now_state = BT_STATE_TALKING;
				break;
			default:
				bt_disp_ctrl->now_state = BT_STATE_IDLE;
				break;
		}
	}

	if(bt_disp_ctrl->now_state == BT_STATE_IDLE)
	{
		if(bt_disp_ctrl->phone_num)
		{
			bt_disp_ctrl->call_num = TRUE;
			bt_disp_ctrl->change = TRUE;
		}

		if(is_auto_mute())
		{
			bt_disp_ctrl->a2dp_state =BT_A2DP_STATE_PAUSE;
		}
		else
		{
			bt_disp_ctrl->a2dp_state =BT_A2DP_STATE_PLAY;
		}

		if(bt_disp_ctrl->music_play != bt_disp_ctrl->a2dp_state)
		{
			bt_disp_ctrl->music_play = bt_disp_ctrl->a2dp_state;
			bt_disp_ctrl->change = TRUE;
		}

		if(bt_disp_ctrl->dial_enter != bt_disp_ctrl->dial_flag)
		{
			bt_disp_ctrl->dial_enter = bt_disp_ctrl->dial_flag;
			bt_disp_ctrl->change = TRUE;
		}
	}
	else if(bt_disp_ctrl->prev_state == BT_STATE_IDLE)
	{
		bt_disp_ctrl->call_num = FALSE;
		bt_disp_ctrl->dial_enter = FALSE;
	}

	if(bt_disp_ctrl->now_state == BT_STATE_RINGING)
	{
		if(bt_disp_ctrl->phone_num)
		{
			bt_disp_ctrl->ring_num = TRUE;
			bt_disp_ctrl->change = TRUE;
		}
	}
	else if(bt_disp_ctrl->prev_state == BT_STATE_RINGING)
	{
		bt_disp_ctrl->ring_num = FALSE;
	}

	if((bt_disp_ctrl->now_state == BT_STATE_OUTGOING)||(bt_disp_ctrl->now_state == BT_STATE_TALKING))
	{
		if(bt_disp_ctrl->phone_num)
		{
			bt_disp_ctrl->dtmf_num = TRUE;
			bt_disp_ctrl->change = TRUE;
		}
	}
	else if((bt_disp_ctrl->prev_state == BT_STATE_OUTGOING)||(bt_disp_ctrl->prev_state == BT_STATE_TALKING))
	{
		bt_disp_ctrl->dtmf_num = FALSE;
	}

	//bt_disp_ctrl->phone_num = FALSE;

bt_ui_again:
	if(bt_disp_ctrl->change || (bt_disp_ctrl->prev_state != bt_disp_ctrl->now_state)
#ifdef TD_ADD_SPP
    ||(gRefresh_BT_Disp==TRUE)//修改蓝牙模式下，连上APP没有发状态给APP显示
#endif
#if defined(_SUPPORT_CALL_RECORDS_)
	||(global.info.call_info)
	||(global.info.call_info_exit)
#endif
#if defined(_ENABLE_TIME_DISP_)
	||(global.work_flag1)
#endif
#if defined(_ENABLE_BT_ON_OFF_)
	||(global.bt_on_off1)
#endif
	)
    {
    	//ui_printf("%s--->now_state:%d\n", __FUNCTION__, bt_disp_ctrl->now_state);

	#if defined(_ENABLE_TIME_DISP_)
		global.work_flag1 = FALSE;
		show_clear_icon(DISP_LEVEL_NORMAL);
	#endif

	#if defined(_ENABLE_BT_ON_OFF_)
		global.bt_on_off1 = FALSE;
	#endif

	#ifdef TD_ADD_SPP
		gRefresh_BT_Disp =FALSE;
	#endif

    	bt_disp_ctrl->disp_timer 	= 0;
		bt_disp_ctrl->disp_status 	= BT_DISP_STATUS_STATIC;
		bt_disp_ctrl->flash_status 	= TRUE;

		switch(bt_disp_ctrl->now_state)
		{
			case BT_STATE_CONNECT:
				
			#if defined(_ENABLE_PAUSE_LIGHT_)
				global.pause_flg = FALSE;
			#endif
							
			#if defined(_ACC_OFF_DISP_LOGO_)
				if(global.bt_off)
				{
					ui_lang_res_search(LANG_ID_SYSTEM_GOODBYE, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
					global.bt_off = FALSE;
				}
				else
				{
					ui_lang_res_search(LANG_ID_BT_CONNECT, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
					bt_disp_ctrl->disp_status = BT_DISP_STATUS_FLASH;
				}
			#elif defined(_ENABLE_BT_ON_OFF_)
				if(setup.bt_on)
				{
					ui_lang_res_search(LANG_ID_BT_CONNECT, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
					bt_disp_ctrl->disp_status = BT_DISP_STATUS_FLASH;
				}
				else
				{
					ui_lang_res_search(LANG_ID_OTHERS_BT_OFF, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
				}
			#elif defined(_DISABLE_ACC_OFF_DISPLAY_CONNECT_)
			    if(global.bt_off)
				{
					ui_lang_res_search(LANG_ID_NULL, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
					global.bt_off = FALSE;
				}
				else
				{
					ui_lang_res_search(LANG_ID_BT_CONNECT, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
				#if !defined(_WLS316_SPECIAL_DISPLAY_)
					bt_disp_ctrl->disp_status = BT_DISP_STATUS_FLASH;
				#endif
				}
			#else
				ui_lang_res_search(LANG_ID_BT_CONNECT, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
			#if !defined(_XM003_SPECIAL_DISPLAY_)
			#if !defined(_WLS316_SPECIAL_DISPLAY_)
			#if !defined(_KLD3300_SPECIAL_DISPLAY_)
				bt_disp_ctrl->disp_status = BT_DISP_STATUS_FLASH;
			#endif
			#endif
			#endif
			#endif
				break;

			case BT_STATE_IDLE:
				#if defined(_LCD_3_DIAPLAY_)
					ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
				#endif
				if(bt_disp_ctrl->dial_enter && bt_disp_ctrl->call_num)
				{
					ew_puts("BT_STATE_IDLE111\n");
					strcpy((char *)bt_disp_ctrl->disp_user, (char *)bt_disp_ctrl->phone_buffer);
					bt_disp_ctrl->disp_length = strlen((char *)bt_disp_ctrl->disp_user);

					if(bt_disp_ctrl->disp_length)
					{
						if(bt_disp_ctrl->disp_length > BT_DISP_BUF_SIZE)
						{
							strcpy((char *)bt_disp_ctrl->info_buf, (char *)&(bt_disp_ctrl->disp_user[bt_disp_ctrl->disp_length - BT_DISP_BUF_SIZE]));
						}
						else
						{
							strcpy((char *)bt_disp_ctrl->info_buf, (char *)bt_disp_ctrl->disp_user);
						}
					}
					else
					{
						goto idle_disp_null;
					}
				}
				else
				{
					ew_puts("BT_STATE_IDLE777\n");
				idle_disp_null:
					if(bt_disp_ctrl->prev_state == BT_STATE_CONNECT)
					{//刚连上蓝牙时
					#ifdef TD_ADD_SPP
                        Spp_Tx_bt_Info(0);
					#endif
						ew_puts("BT_STATE_IDLE000\n");
					#if defined(_ENABLE_A2DP_PLAY_)
						global.a2dp_start = FALSE;
					#endif
						ui_lang_res_search(LANG_ID_BT_CONNECTD, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
						bt_disp_ctrl->disp_timer = (3000/BT_POLLING_TIME);
						bt_disp_ctrl->disp_status = BT_DISP_STATUS_TIME;
					#if defined(_ENABLE_PAUSE_LIGHT_)
						global.pause_flg = FALSE;
					#endif
					#if defined(_ENABLE_A2DP_TIME_)
						global.a2dp_flag = FALSE;
					#endif
					}
					else
					{
						ew_puts("BT_STATE_IDLE222\n");
						if(bt_disp_ctrl->dial_enter)
						{//刚连上蓝牙后 数字拨号
							ui_lang_res_search(LANG_ID_BT_DIAL, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
							ew_puts("BT_STATE_IDLE333\n");
						}
						else
						{
							ew_puts("BT_STATE_IDLE444\n");
						#if defined(_SUPPORT_CALL_RECORDS_)
							if(global.info.call_info)
							{
								if(global.info.call_ring)
								{
									ew_printf("setup.call_num:%d\n",global.info.call_num);

									ew_printf("setup.sum:%d\n",global.info.sum);
									ew_printf("setup.num_buffer:%s\n",global.info.num_buffer[global.info.sum]);

									global.info.disp_length = strlen((char *)global.info.num_buffer[global.info.sum]);

									memcpy(bt_disp_ctrl->disp_buf, global.info.num_buffer[global.info.sum], BT_DISP_BUF_SIZE);

									//ew_printf("bt_disp_ctrl->disp_length:%d\n",bt_disp_ctrl->disp_length);
									//if(bt_disp_ctrl->disp_length)
									{
										global.info.call_roll = TRUE;
										bt_disp_ctrl->roll_status = 0;
										bt_disp_ctrl->disp_status = BT_DISP_STATUS_ROLL1;
										global.info.roll_time = 0;
									}
								}
								else
								{
									ew_puts("BT_STATE_IDLE555\n");
									ui_lang_res_search(LANG_ID_BT_NO_CALL, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
									bt_disp_ctrl->disp_timer = (3000/BT_POLLING_TIME);
									bt_disp_ctrl->disp_status = BT_DISP_STATUS_TIME;
								}
								global.info.call_info = FALSE;
							}
							else
						#endif
							{
								ew_puts("BT_STATE_IDLE666\n");
							#if defined(_SUPPORT_CALL_RECORDS_)
								global.info.call_info_exit = FALSE;
							#endif

    							if(bt_disp_ctrl->a2dp_state == BT_A2DP_STATE_PLAY)
    							{//刚连上蓝牙后 播放状态
    							#ifdef TD_ADD_SPP
    								Spp_Tx_bt_Info(2);
    							#endif

									ew_puts("BT_STATE_IDLE999\n");
								#if defined(_ENABLE_A2DP_PLAY_)
    								if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
    								||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
    								||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
    								||((get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK)&&(get_call_status() == BT_CALL_OUTGOING))
    								||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
    								||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
									{
										global.a2dp_start = FALSE;
									}
									else
									{
										global.a2dp_start = TRUE;
									}
								#endif
								
                                #if defined(_ENABLE_PAUSE_LIGHT_)
									global.pause_flg = TRUE;
								#endif
																
								#if defined(_ENABLE_ALARM_CLOCK_)
    								if(setup.alarm_beep == TRUE)
    								{
        								ui_lang_res_search(LANG_ID_OTHERS_ALARM_RINGING, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
    								}
    								else
    								{
        								ui_lang_res_search(LANG_ID_BT_PLAY, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
    								}
								#else
    								ui_lang_res_search(LANG_ID_BT_PLAY, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
								#endif
								#if defined(_SUPPORT_CALL_RECORDS_)
									global.info.call_roll = FALSE;
									global.info.roll_time = 0;
								#endif
							    #if defined(_ENABLE_A2DP_TIME_)
							    	global.a2dp_flag = TRUE;
							    #endif
    							}
    							else
    							{//刚连上蓝牙后 空闲状态
    							#ifdef TD_ADD_SPP
    								Spp_Tx_bt_Info(1);
    							#endif

									ew_puts("BT_STATE_IDLE888\n");
								#if defined(_ENABLE_A2DP_PLAY_)
									global.a2dp_start = FALSE;
								#endif

								#if defined(_ENABLE_ALARM_CLOCK_)
    								if(setup.alarm_beep == TRUE)
    								{
        								ui_lang_res_search(LANG_ID_OTHERS_ALARM_RINGING, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
    								}
    								else
    								{
        								ui_lang_res_search(LANG_ID_BT_PAUSE, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
    								}
								#elif defined(_ENABLE_BT_PAUSE_DISPLAY_)
									ui_lang_res_search(LANG_ID_BT_PLAY, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
								#elif defined(_ENABLE_BT_AUDIO_DISP_)
									if(!global.a2dp_time)
									{
										ui_lang_res_search(LANG_ID_BT_PAUSE, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
									}
								#elif defined(_SPECIAL_PAUSE_DISPLAY_)
									if(global.bt_first_flag)
									{
										ui_lang_res_search(LANG_ID_BT_PAUSE, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
									}
									else
									{
										ui_lang_res_search(LANG_ID_BT_CONNECTD, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
									}
								#else
    								ui_lang_res_search(LANG_ID_BT_PAUSE, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
								#endif
								#if defined(_SUPPORT_CALL_RECORDS_)
									global.info.call_roll = FALSE;
									global.info.roll_time = 0;
								#endif
								
								#if defined(_ENABLE_PAUSE_LIGHT_)
									global.pause_flg = FALSE;
								#endif
								
							    #if defined(_ENABLE_A2DP_TIME_)
							    	global.a2dp_flag = FALSE;
							    #endif
    							}
							}
						}
					}
				}
				break;

			case BT_STATE_RINGING:
				ew_puts("BT_STATE_RINGING\n");
            #if defined(_ENABLE_PAUSE_LIGHT_)
				global.pause_flg = FALSE;
			#endif
			
				bt_disp_ctrl->phone_num = FALSE;
			
			#if defined(_SUPPORT_CALL_RECORDS_)
				global.info.call_ring = TRUE;
				global.info.call_roll = FALSE;
				global.info.call_info_flag = TRUE;
			#endif

		    	bt_disp_ctrl->disp_time = 0;
    			if(global.aud_mute)
    			{
			    #if defined(_ENABLE_BT_MUTE_)
				#if !defined(_RX_ENABLE_BT_MUTE_)
			    	OSTimeDly(10);
				#endif
				#elif defined(_SWL1787_ENABLE_BT_MUTE_)
			    	OSTimeDly(5);
				#elif defined(_HYM_ENABLE_BT_MUTE_)
				    //OSTimeDly(5);
			    #else
				    OSTimeDly(50);
				#endif
    				disp_send_msg(DISP_EVENT_MUTE);
    			}

				if(bt_disp_ctrl->ring_num)
				{
				#if defined(_SUPPORT_CALL_RECORDS_)
    				global.info.call_num++;
    				if(global.info.call_num> 10)
    				{
    					global.info.call_num=1;
    					global.info.flag = TRUE;
    				}
    				global.info.sum=global.info.call_num ;
				#endif

					memset(bt_disp_ctrl->disp_user, 0, sizeof(bt_disp_ctrl->disp_user));
					bt_disp_ctrl->disp_length = strlen((char *)bt_disp_ctrl->phone_buffer);

				#if defined(_SUPPORT_CALL_RECORDS_)
					global.info.disp_length = bt_disp_ctrl->disp_length;
					memset(global.info.num_buffer[global.info.call_num], 0, 30);
					memcpy(global.info.num_buffer[global.info.call_num], bt_disp_ctrl->phone_buffer, global.info.disp_length);
				#endif

					if(bt_disp_ctrl->disp_length)
					{
				    #if defined(_YONGHE_SPECIAL_DISPLAY_)
				    	strcpy((char *)bt_disp_ctrl->disp_user, "CALL IN ");
				    #endif
						strcat((char *)bt_disp_ctrl->disp_user, (char *)bt_disp_ctrl->phone_buffer);
						bt_disp_ctrl->disp_length = strlen((char *)bt_disp_ctrl->disp_user);
					#if defined(_LCD_3_DIAPLAY_)
					    if(bt_disp_ctrl->disp_length > BT_DISP_BUF_SIZE-1)
    					{
    						bt_disp_ctrl->roll_status = 0;
    						bt_disp_ctrl->disp_status = BT_DISP_STATUS_ROLL;
    					}
    					else
    					{
    						memcpy(&bt_disp_ctrl->info_buf[1], bt_disp_ctrl->disp_user, BT_DISP_BUF_SIZE-1);
    					}
					#else
    					if(bt_disp_ctrl->disp_length > BT_DISP_BUF_SIZE)
    					{
    						bt_disp_ctrl->roll_status = 0;
    						bt_disp_ctrl->disp_status = BT_DISP_STATUS_ROLL;
    					}
    					else
    					{
    						memcpy(bt_disp_ctrl->info_buf, bt_disp_ctrl->disp_user, BT_DISP_BUF_SIZE);
    					}
					#endif
					}
					else
					{
					#if !defined(_DISABLE_RINGING_)
						goto ringing_disp_null;
					#endif
					}
				}
				else
				{
				#if !defined(_DISABLE_RINGING_)
				ringing_disp_null:
					ui_lang_res_search(LANG_ID_BT_INCOMING, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
				#endif	
				}
				break;

			case BT_STATE_OUTGOING:
				ew_puts("BT_STATE_OUTGOING\n");
			#if defined(_ENABLE_PAUSE_LIGHT_)
				global.pause_flg = FALSE;
			#endif
			
				bt_disp_ctrl->phone_num = FALSE;
			
			#if defined(_SUPPORT_CALL_RECORDS_)
				global.info.call_ring = TRUE;
				global.info.call_roll = FALSE;
				global.info.call_info_flag = TRUE;
			#endif

		    	bt_disp_ctrl->disp_time = 0;
				if(global.aud_mute)
				{
				#if defined(_ENABLE_BT_MUTE_)
				#if !defined(_RX_ENABLE_BT_MUTE_)
					OSTimeDly(10);
				#endif
				#elif defined(_SWL1787_ENABLE_BT_MUTE_)
					OSTimeDly(5);
				#elif defined(_HYM_ENABLE_BT_MUTE_)
				    //OSTimeDly(5);
				#else
					OSTimeDly(50);
				#endif
					disp_send_msg(DISP_EVENT_MUTE);
				}
				
			#if !defined(_DISABLE_CALL_NUMBER_)
				if(bt_disp_ctrl->dtmf_num)
				{
				#if defined(_SUPPORT_CALL_RECORDS_)
    				global.info.call_num++;
    				if(global.info.call_num> 10)
    				{
    					global.info.call_num = 1;
    					global.info.flag = TRUE;
    				}
    				global.info.sum = global.info.call_num;
				#endif

					strcpy((char *)bt_disp_ctrl->disp_user, (char *)bt_disp_ctrl->phone_buffer);
					bt_disp_ctrl->disp_length = strlen((char *)bt_disp_ctrl->disp_user);

					ew_printf("bt_disp_ctrl->disp_user:%s\n",bt_disp_ctrl->disp_user);
					ew_printf("bt_disp_ctrl->phone_buffer:%s\n",bt_disp_ctrl->phone_buffer);
					//ew_printf("global.info.call_num:%d\n",global.info.call_num);

				#if defined(_SUPPORT_CALL_RECORDS_)
					global.info.disp_length = bt_disp_ctrl->disp_length;
					memset(global.info.num_buffer[global.info.call_num], 0, 30);
					memcpy(global.info.num_buffer[global.info.call_num], bt_disp_ctrl->phone_buffer, global.info.disp_length);
				#endif

					if(bt_disp_ctrl->disp_length)
					{
    					if(bt_disp_ctrl->disp_length > BT_DISP_BUF_SIZE)
    					{
    						bt_disp_ctrl->roll_status = 0;
    						bt_disp_ctrl->disp_status = BT_DISP_STATUS_ROLL;
    					}
    					else
    					{
    						memcpy(bt_disp_ctrl->info_buf, bt_disp_ctrl->disp_user, BT_DISP_BUF_SIZE);
    					}
					}
					else
					{
						goto outgoing_disp_null;
					}
				}
				else
			#endif
				{
			#if !defined(_DISABLE_CALL_NUMBER_)
				outgoing_disp_null:
			#endif
					ui_lang_res_search(LANG_ID_BT_OUTGOING, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
				}
				break;
		#if 0
		#ifndef	TD_ADD_ALEN
			case BT_STATE_TALKING:
				if(bt_disp_ctrl->dtmf_num)
				{
					strcpy((char *)bt_disp_ctrl->disp_user, (char *)bt_disp_ctrl->phone_buffer);
					bt_disp_ctrl->disp_length = strlen((char *)bt_disp_ctrl->disp_user);

					if(bt_disp_ctrl->disp_length)
					{
						if(bt_disp_ctrl->disp_length > BT_DISP_BUF_SIZE)
						{
							strcpy((char *)bt_disp_ctrl->info_buf, (char *)&(bt_disp_ctrl->disp_user[bt_disp_ctrl->disp_length - BT_DISP_BUF_SIZE]));
						}
						else
						{
							strcpy((char *)bt_disp_ctrl->info_buf, (char *)bt_disp_ctrl->disp_user);
						}
					}
					else
					{
						goto talking_disp_null;
					}
				}
				else
				{
				talking_disp_null:
					if(bt_disp_ctrl->aud_state == BT_HFP_AUD_HOST)
					{
						ui_lang_res_search(LANG_ID_BT_HFP, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
					}
					else
					{
				    	ui_lang_res_search(LANG_ID_BT_PHONE, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
					}
				}
				break;
		#endif
		#endif
			default:
				break;
		}
		bt_disp_ctrl->prev_state = bt_disp_ctrl->now_state;
		memcpy(bt_disp_ctrl->disp_buf, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
		bt_disp_ctrl->change = TRUE;
    }

	switch(bt_disp_ctrl->disp_status)
	{
		case BT_DISP_STATUS_ROLL:
			if(bt_disp_ctrl->roll_status == 0)
			{
			#if defined(_LCD_3_DIAPLAY_)
			    memcpy(&bt_disp_ctrl->disp_buf[1], bt_disp_ctrl->disp_user, BT_DISP_BUF_SIZE-1);
			#else
				memcpy(bt_disp_ctrl->disp_buf, bt_disp_ctrl->disp_user, BT_DISP_BUF_SIZE);
			#endif
				bt_disp_ctrl->roll_status = 1;
				bt_disp_ctrl->change = TRUE;
			}
		    else if(bt_disp_ctrl->roll_status == 1)
			{
				if((++bt_disp_ctrl->disp_timer)>(800/BT_POLLING_TIME))
				{
					bt_disp_ctrl->disp_timer = 0;
					bt_disp_ctrl->roll_point = 0;
					bt_disp_ctrl->roll_status = 2;
				}
			}
			else if(bt_disp_ctrl->roll_status == 2)
			{
				if((++bt_disp_ctrl->disp_timer)>(300/BT_POLLING_TIME))
				{
					bt_disp_ctrl->disp_timer = 0;
					if(bt_disp_ctrl->roll_point <= bt_disp_ctrl->disp_length)
					{
					#if defined(_LCD_3_DIAPLAY_)
					   	memcpy(&bt_disp_ctrl->disp_buf[1], &bt_disp_ctrl->disp_user[bt_disp_ctrl->roll_point], BT_DISP_BUF_SIZE-1);
					#else
						memcpy(bt_disp_ctrl->disp_buf, &bt_disp_ctrl->disp_user[bt_disp_ctrl->roll_point], BT_DISP_BUF_SIZE);
					#endif
						bt_disp_ctrl->roll_point++;
						bt_disp_ctrl->change = TRUE;
					}
					else
					{
						bt_disp_ctrl->roll_status = 3;
					}
				}
			}
			else if(bt_disp_ctrl->roll_status == 3)
			{
				if((++bt_disp_ctrl->disp_timer)>(500/BT_POLLING_TIME))
				{
					bt_disp_ctrl->disp_timer = 0;
					bt_disp_ctrl->roll_status = 0;
				}
			}
			break;

	#if defined(_SUPPORT_CALL_RECORDS_)
    	case BT_DISP_STATUS_ROLL1:
    		if(bt_disp_ctrl->roll_status == 0)
    		{
    			memcpy(bt_disp_ctrl->disp_buf, global.info.num_buffer[global.info.sum], BT_DISP_BUF_SIZE);
    			bt_disp_ctrl->roll_status = 1;
    			bt_disp_ctrl->change = TRUE;
    		}
    		else if(bt_disp_ctrl->roll_status == 1)
    		{
    			if((++bt_disp_ctrl->disp_timer)>(800/BT_POLLING_TIME))
    			{
    				bt_disp_ctrl->disp_timer = 0;
    				bt_disp_ctrl->roll_point = 0;
    				bt_disp_ctrl->roll_status = 2;
    			}
    		}
    		else if(bt_disp_ctrl->roll_status == 2)
    		{
    			if((++bt_disp_ctrl->disp_timer)>(300/BT_POLLING_TIME))
    			{
    				bt_disp_ctrl->disp_timer = 0;
    				if(bt_disp_ctrl->roll_point <= global.info.disp_length)
    				{
    					memcpy(bt_disp_ctrl->disp_buf, &global.info.num_buffer[global.info.sum][bt_disp_ctrl->roll_point], BT_DISP_BUF_SIZE);
    					bt_disp_ctrl->roll_point++;
    					bt_disp_ctrl->change = TRUE;
    				}
    				else
    				{
    					bt_disp_ctrl->roll_status = 3;
    				}
    			}
    		}
    		else if(bt_disp_ctrl->roll_status == 3)
    		{
    			if((++bt_disp_ctrl->disp_timer)>(500/BT_POLLING_TIME))
    			{
    				bt_disp_ctrl->disp_timer = 0;
    				bt_disp_ctrl->roll_status = 0;
    			#if defined(_SUPPORT_CALL_RECORDS_)
    				if(global.info.call_roll)
    				{
    					global.info.roll_time++;
    					//ew_printf("global.info.roll_time:%d\n",global.info.roll_time);
    					if(global.info.roll_time == 3)
    					{
    						global.info.roll_time = 0;
    						global.info.call_info = FALSE;
    						global.info.call_roll = FALSE;

							global.info.call_info_flag = TRUE;

    						bt_disp_ctrl->change = TRUE;
    						goto bt_ui_again;
    					}
    				}
    			#endif
    			}
    		}
    		break;
	#endif

		case BT_DISP_STATUS_FLASH:
			if((++bt_disp_ctrl->disp_timer)>(500/BT_POLLING_TIME))
			{
				bt_disp_ctrl->disp_timer = 0;
				bt_disp_ctrl->flash_status = !bt_disp_ctrl->flash_status;
				if(bt_disp_ctrl->flash_status)
				{
					memcpy(bt_disp_ctrl->disp_buf, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);
				}
				else
				{
					memset(bt_disp_ctrl->disp_buf, ' ', BT_DISP_BUF_SIZE);
				}
				bt_disp_ctrl->change = TRUE;
			}
			break;

		case BT_DISP_STATUS_TIME:
			if(bt_disp_ctrl->disp_timer)
			{
				if(!(--bt_disp_ctrl->disp_timer))
				{
					bt_disp_ctrl->change = TRUE;
					goto bt_ui_again;
				}
			}
			break;

		default:
			break;
	}
#ifdef	TD_ADD_ALEN//增加通话时间显示
	if(bt_disp_ctrl->now_state == BT_STATE_TALKING)
	{
		bt_disp_ctrl->disp_time++;
	#if defined(TD_ADD_LCD_NUM_4)
		bt_disp_ctrl->info_buf[0]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
		bt_disp_ctrl->info_buf[1]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
		bt_disp_ctrl->info_buf[2]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
		bt_disp_ctrl->info_buf[3]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
	#elif defined(_LCD_3_DIAPLAY_)
		bt_disp_ctrl->info_buf[0] = ((((bt_disp_ctrl->disp_time/10)%3600)/60)/10) + '0';
		bt_disp_ctrl->info_buf[1] = ((((bt_disp_ctrl->disp_time/10)%3600)/60)%10) + '0';
		bt_disp_ctrl->info_buf[2] = ((((bt_disp_ctrl->disp_time/10)%3600)%60)/10) + '0';
		bt_disp_ctrl->info_buf[3] = ((((bt_disp_ctrl->disp_time/10)%3600)%60)%10) + '0';
		if(bt_disp_ctrl->info_buf[0] == '0')
		{
			bt_disp_ctrl->info_buf[0] = ' ';
		}
	#elif defined(_XM003_SPECIAL_DISPLAY_)
		bt_disp_ctrl->info_buf[0] = ' ';
		bt_disp_ctrl->info_buf[1] = ' ';
		bt_disp_ctrl->info_buf[2] = ' ';
		bt_disp_ctrl->info_buf[3] = ((((bt_disp_ctrl->disp_time/10)%3600)/60)/10) + '0';
		bt_disp_ctrl->info_buf[4] = ((((bt_disp_ctrl->disp_time/10)%3600)/60)%10) + '0';
		bt_disp_ctrl->info_buf[5] = ((((bt_disp_ctrl->disp_time/10)%3600)%60)/10) + '0';
		bt_disp_ctrl->info_buf[6] = ((((bt_disp_ctrl->disp_time/10)%3600)%60)%10) + '0';
		//bt_disp_ctrl->info_buf[7] = ' ';
	#elif defined(_FY7000_SPECIAL_DISPLAY_)
    	bt_disp_ctrl->info_buf[0] =' ';
    	bt_disp_ctrl->info_buf[1] =' ';
    	bt_disp_ctrl->info_buf[2] =' ';
    	bt_disp_ctrl->info_buf[3] =((bt_disp_ctrl->disp_time/10)/3600)/10+'0';
    	bt_disp_ctrl->info_buf[4] =((bt_disp_ctrl->disp_time/10)/3600)%10+'0';
    	bt_disp_ctrl->info_buf[5] ='-';
    	bt_disp_ctrl->info_buf[6] =((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[7] =((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[8] ='-';
    	bt_disp_ctrl->info_buf[9] =((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[10] =((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
    	bt_disp_ctrl->info_buf[11] =' ';
    	bt_disp_ctrl->info_buf[12] =' ';
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)
    	bt_disp_ctrl->info_buf[0] =' ';
    	bt_disp_ctrl->info_buf[1] =' ';
    	bt_disp_ctrl->info_buf[2] =' ';
    	bt_disp_ctrl->info_buf[3] =' ';
    	bt_disp_ctrl->info_buf[4]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[5]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[6] ='-';
    	bt_disp_ctrl->info_buf[7]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[8]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
    	bt_disp_ctrl->info_buf[9] =' ';
	#elif defined(_SH1330_SPECIAL_DISPLAY_)
    	bt_disp_ctrl->info_buf[0] =' ';
    	bt_disp_ctrl->info_buf[1] =' ';
    	bt_disp_ctrl->info_buf[2] =' ';
    	bt_disp_ctrl->info_buf[3]=((bt_disp_ctrl->disp_time/10)/3600)/10+'0';
		bt_disp_ctrl->info_buf[4]=((bt_disp_ctrl->disp_time/10)/3600)%10+'0';
    	bt_disp_ctrl->info_buf[5] ='-';
    	bt_disp_ctrl->info_buf[6] =((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[7] =((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[8] =((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[9] =((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
	#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	#if defined(_SL6812A_BT_DISPLAY_)
    	bt_disp_ctrl->info_buf[0] =' ';
    	bt_disp_ctrl->info_buf[1] ='T';
    	bt_disp_ctrl->info_buf[2] ='A';
    	bt_disp_ctrl->info_buf[3] ='L';
    	bt_disp_ctrl->info_buf[4] ='K';
    	bt_disp_ctrl->info_buf[5] =' ';
    	bt_disp_ctrl->info_buf[6] =((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[7] =((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[8] =((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[9] =((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
	#else
    	bt_disp_ctrl->info_buf[0] =' ';
    	bt_disp_ctrl->info_buf[1] =((bt_disp_ctrl->disp_time/10)/3600)/10+'0';
    	bt_disp_ctrl->info_buf[2] =((bt_disp_ctrl->disp_time/10)/3600)%10+'0';
    	bt_disp_ctrl->info_buf[3] ='-';
    	bt_disp_ctrl->info_buf[4] =((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[5] =((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[6] ='-';
    	bt_disp_ctrl->info_buf[7] =((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[8] =((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
    	bt_disp_ctrl->info_buf[9] =' ';
	#endif
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
    	bt_disp_ctrl->info_buf[0] =' ';
    	bt_disp_ctrl->info_buf[1] =((bt_disp_ctrl->disp_time/10)/3600)/10+'0';
    	bt_disp_ctrl->info_buf[2] =((bt_disp_ctrl->disp_time/10)/3600)%10+'0';
    	bt_disp_ctrl->info_buf[3] ='-';
    	bt_disp_ctrl->info_buf[4] =((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[5] =((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[6] ='-';
    	bt_disp_ctrl->info_buf[7] =((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[8] =((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
    	bt_disp_ctrl->info_buf[9] =' ';
    	bt_disp_ctrl->info_buf[10] =' ';
	#elif defined(_FIVE_CHAR_DISPLAY_)
		bt_disp_ctrl->info_buf[0] =' ';
    	bt_disp_ctrl->info_buf[1]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[2]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
		bt_disp_ctrl->info_buf[3]='-';
    	bt_disp_ctrl->info_buf[4]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[5]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
		bt_disp_ctrl->info_buf[6] =' ';
	#elif defined(_SEVEN_CHAR_DISPLAY_)
	#if defined(_FY2010_LCD_DISPLAY_)
		bt_disp_ctrl->info_buf[0] =' ';
    	bt_disp_ctrl->info_buf[1]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[2]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[3]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[4]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
		bt_disp_ctrl->info_buf[5] =' ';
		bt_disp_ctrl->info_buf[6] =' ';
	#else
		bt_disp_ctrl->info_buf[0]=((bt_disp_ctrl->disp_time/10)/3600)/10+'0';
		bt_disp_ctrl->info_buf[1]=((bt_disp_ctrl->disp_time/10)/3600)%10+'0';
		bt_disp_ctrl->info_buf[2]='-';
		bt_disp_ctrl->info_buf[3]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
		bt_disp_ctrl->info_buf[4]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
		bt_disp_ctrl->info_buf[5]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
		bt_disp_ctrl->info_buf[6]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
	#endif
	#else
	#if defined(SPECIAL_JD_DISP)
		bt_disp_ctrl->info_buf[0]=' ';
		bt_disp_ctrl->info_buf[1]=' ';
		bt_disp_ctrl->info_buf[2]=' ';
		bt_disp_ctrl->info_buf[3]=' ';
		bt_disp_ctrl->info_buf[4]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
		bt_disp_ctrl->info_buf[5]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
		bt_disp_ctrl->info_buf[6]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
		bt_disp_ctrl->info_buf[7]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
	#elif defined(_SL6812_TIME_DISPLAY_)
    	bt_disp_ctrl->info_buf[0]=((bt_disp_ctrl->disp_time/10)/3600)/10+'0';
    	bt_disp_ctrl->info_buf[1]=((bt_disp_ctrl->disp_time/10)/3600)%10+'0';
    	bt_disp_ctrl->info_buf[2]='-';
    	bt_disp_ctrl->info_buf[3]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[4]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[5]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[6]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
    	bt_disp_ctrl->info_buf[7] =' ';
	#elif defined(_WLS316_SPECIAL_DISPLAY_)||defined(_LIANKE_TALKING_DISPLAY_)
    	bt_disp_ctrl->info_buf[0]=((bt_disp_ctrl->disp_time/10)/3600)/10+'0';
    	bt_disp_ctrl->info_buf[1]=((bt_disp_ctrl->disp_time/10)/3600)%10+'0';
    	bt_disp_ctrl->info_buf[2]='-';
    	bt_disp_ctrl->info_buf[3]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
    	bt_disp_ctrl->info_buf[4]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
    	bt_disp_ctrl->info_buf[5] ='-';
    	bt_disp_ctrl->info_buf[6]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
    	bt_disp_ctrl->info_buf[7]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
	#elif defined(_KLD3300_SPECIAL_DISPLAY_)
    	bt_disp_ctrl->info_buf[0] ='T';
    	bt_disp_ctrl->info_buf[1] ='A';
    	bt_disp_ctrl->info_buf[2] ='L';
    	bt_disp_ctrl->info_buf[3] ='K';
    	bt_disp_ctrl->info_buf[4] ='I';
    	bt_disp_ctrl->info_buf[5] ='N';
    	bt_disp_ctrl->info_buf[6] ='G';
    	bt_disp_ctrl->info_buf[7] =' ';
	#else
		bt_disp_ctrl->info_buf[0] =' ';
		bt_disp_ctrl->info_buf[1]=((bt_disp_ctrl->disp_time/10)/3600)/10+'0';
		bt_disp_ctrl->info_buf[2]=((bt_disp_ctrl->disp_time/10)/3600)%10+'0';
		bt_disp_ctrl->info_buf[3]='-';
		bt_disp_ctrl->info_buf[4]=((((bt_disp_ctrl->disp_time/10)%3600)/60)/10)+'0';
		bt_disp_ctrl->info_buf[5]=((((bt_disp_ctrl->disp_time/10)%3600)/60)%10)+'0';
		bt_disp_ctrl->info_buf[6]=((((bt_disp_ctrl->disp_time/10)%3600)%60)/10)+'0';
		bt_disp_ctrl->info_buf[7]=((((bt_disp_ctrl->disp_time/10)%3600)%60)%10)+'0';
	#endif
	#endif
	#if defined(_USER_TFT_SEND_)
		TFT_Tx_BT_Tel_Time(bt_disp_ctrl->disp_time);
	#endif
		memcpy(bt_disp_ctrl->disp_buf, bt_disp_ctrl->info_buf, BT_DISP_BUF_SIZE);

		global.clk_flag = TRUE;
	#if defined(TD_ADD_LCD_NUM_4)||defined(_SEVEN_CHAR_DISPLAY_)||defined(_XM003_SPECIAL_DISPLAY_)||defined(_LCD_3_DIAPLAY_)||defined(_SL6812_TIME_DISPLAY_)
		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
	#else
	#if !defined(_FIVE_CHAR_DISPLAY_)
	#if !defined(_FY5517_SPECIAL_DISPLAY_)
	#if !defined(_FY7000_SPECIAL_DISPLAY_)
	#if !defined(_WLS316_SPECIAL_DISPLAY_)
	#if !defined(_KLD3300_SPECIAL_DISPLAY_)
	#if !defined(_LIANKE_TALKING_DISPLAY_)
		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
	#if defined(_ENABLE_TIME_DISPLAY_)
    	ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
	#endif
		ui_show_string(DISP_LEVEL_NORMAL, bt_disp_ctrl->disp_buf);
		bt_disp_ctrl->change = FALSE;
	}
	else
	{
		if(global.clk_flag)
		{
			global.clk_flag = FALSE;
		#if defined(TD_ADD_LCD_NUM_4)||defined(_PS329a_SPECIAL_DISPLAY_)||defined(_XM003_SPECIAL_DISPLAY_)||defined(_LCD_3_DIAPLAY_)||defined(_SL6812_TIME_DISPLAY_)
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
		#elif defined(_ENABLE_TIME_DISPLAY_)
    		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
    		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);
		#else
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);
		#endif
		}
	}
#endif
	if(bt_disp_ctrl->change)
	{
		ui_show_string(DISP_LEVEL_NORMAL, bt_disp_ctrl->disp_buf);
		bt_disp_ctrl->change = FALSE;
	}
}

#if defined(_SUPPORT_LCM_DRIVE)
extern void LCM_Disp_CallNum(void);
u8 btlink_16_16buf[][32]=
{
	{0x00,0x20,0x20,0x27,0x20,0xF0,0x2F,0x20,0x21,0x2E,0xF4,0x26,0x25,0x24,0x24,0x20,
/*0*/0x00,0x02,0x02,0x82,0x3E,0x22,0xE2,0xBE,0x22,0x3E,0x22,0x22,0xBE,0x02,0x02,0x02},/*"蓝",0*/

	{0x00,0x00,0x84,0x8E,0xB4,0x84,0x84,0x85,0x86,0xFF,0x84,0x84,0x84,0x84,0x04,0x00,
/*1*/0x00,0x04,0x08,0x10,0x20,0x40,0x80,0x04,0x02,0xFC,0x00,0x00,0x00,0x00,0x00,0x00},/*"牙",0*/

	{0x00,0x04,0x04,0x24,0x24,0x24,0x24,0x27,0xFF,0x25,0x24,0x24,0x24,0x24,0x04,0x04,
/*2*/0x00,0x08,0x08,0x10,0x10,0x20,0xC0,0x00,0xFE,0x00,0x80,0x40,0x20,0x30,0x18,0x10},/*"未",0*/

	{0x00,0x02,0x82,0x72,0x23,0x00,0x22,0x27,0x3A,0xE2,0x2F,0x22,0x22,0x22,0x20,0x00,
/*3*/0x00,0x00,0x02,0x04,0xF8,0x44,0x42,0x42,0x42,0x42,0xFA,0x42,0x42,0x42,0x42,0x42},/*"连",0*/

	{0x00,0x20,0x20,0x21,0xFF,0x22,0x2C,0x48,0x68,0x5B,0x48,0xC8,0x58,0x68,0x48,0x08,
/*4*/0x00,0x80,0x84,0x02,0xFC,0x80,0x82,0xA2,0xE4,0x94,0x98,0x90,0xE8,0x84,0x86,0x80},/*"接",0*/

    {0x00,0x00,0x23,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x3F,0x00,0x00,0x00,0x00,
/*5*/0x00,0x00,0xFE,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x1E,0x04,0x00},/*"已",0*/

	{0x00,0x00,0x01,0x21,0x21,0x29,0x25,0x23,0xFF,0x21,0x23,0x2D,0x29,0x21,0x21,0x01,
/*6*/0x00,0x04,0x04,0x08,0x08,0x10,0x20,0x40,0xFF,0x80,0x40,0x20,0x10,0x18,0x0C,0x08},/*"来",0*/

    {0x00,0x00,0x1F,0x12,0x12,0x12,0x12,0xFF,0x12,0x12,0x12,0x12,0x1F,0x00,0x00,0x00,
/*7*/0x00,0x00,0xF0,0x20,0x20,0x20,0x20,0xFC,0x22,0x22,0x22,0x22,0xF2,0x02,0x0E,0x00},/*"电",0*/

	{0x00,0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
/*8*/0x00,0x80,0x84,0x8E,0x94,0xA4,0xE4,0x84,0x84,0x84,0x94,0x8E,0x84,0x80,0x80,0x00},/*"去",0*/

	{0x00,0x01,0x49,0x59,0x69,0xDF,0x4A,0x4A,0x01,0x7E,0x48,0x48,0x48,0x4F,0x48,0x08,
/*9*/0x00,0x00,0x00,0x00,0x7E,0xD2,0x52,0xD2,0x52,0x52,0x52,0x52,0x7E,0x80,0x00,0x00},/*"暂",0*/

	{0x00,0x02,0x04,0x08,0x3F,0xC0,0x81,0x41,0x5D,0x55,0x55,0xD5,0x55,0x5D,0x41,0x41,
/*10*/0x00,0x00,0x00,0x00,0xFE,0x00,0x80,0x00,0x40,0x44,0x42,0x7C,0x40,0x40,0x40,0x80},/*"停",0*/

	{0x00,0x20,0x21,0xFF,0x22,0x25,0x91,0xD3,0xB5,0x99,0xFF,0x99,0xb5,0xd3,0x13,0x12,
/*11*/0x00,0x84,0x02,0xFC,0x00,0x00,0x00,0xFE,0x24,0x24,0xFC,0x24,0x24,0xFE,0x00,0x00},/*"播",0*/

    {0x00,0x20,0x20,0x3F,0x24,0xE4,0x27,0x22,0x04,0x38,0xE6,0xA1,0x20,0x23,0x3C,0x20,
/*12*/0x00,0x04,0x18,0xE0,0x04,0x02,0xFC,0x00,0x04,0x08,0x10,0xA0,0xE0,0x18,0x0C,0x08},/*"放",0*/

    {0x00,0x00,0x3F,0x77,0xF3,0xF9,0xFC,0x80,0xCE,0xE4,0xF1,0xFB,0x7F,0x3F,0x00,0x00,
/*13*/0x00,0x00,0xFC,0xEE,0xCF,0x9F,0x3F,0x01,0x73,0x27,0x8F,0xDF,0xFE,0xFC,0x00,0x00},//蓝牙图标

	{0x00,0x02,0x82,0x63,0x00,0x00,0x4F,0x4A,0x4A,0x6A,0x5F,0x5A,0x6A,0x4F,0x00,0x00,
/*14*/0x00,0x02,0x04,0xF8,0x04,0x02,0xFA,0x42,0x42,0x42,0xFA,0x52,0x4A,0xF2,0x02,0x02},/*"通",0*/

	{0x00,0x02,0x82,0x72,0x23,0x00,0x04,0x24,0x24,0x24,0x24,0x3F,0x44,0x44,0x44,0x04,
/*15*/0x00,0x00,0x00,0x00,0xFE,0x04,0x08,0x00,0x7E,0x44,0x44,0xC4,0x44,0x44,0x7E,0x00},/*"话",0*/

	{0x00,0x00,0x00,0x7F,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0x7F,0x20,0x00,
/*16*/0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x80,0xFE,0x80,0x80,0x80,0x80,0xC0,0x00,0x00},/*"中",0*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*17*/0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/

};
void Lcm_Disp_Bt16_16(u8 x_start,u8 y_start,u8 *data,u8 data_size)
{
	u8 *p_disp_buf;
	u8 curr_disp_page=0;
	for(u8 col=x_start;col<(x_start+2);col++)
	{//页数
		curr_disp_page=(col-x_start);
		Lcm_Start();
		Lcm_Write(0x70); // set slave address
		Lcm_Write(0xb3-col);  //set page address
		Lcm_Write(0x10);//set column address
		Lcm_Write(0x00);//
		Lcm_Stop();
		Lcm_Start();
		Lcm_Write(0x72); // set slave address
        for(u8 i=0;i<y_start;i++)
        {
            Lcm_Write(0x00);
        }
		for(u8 y=0;y<data_size;y++)
		{
			p_disp_buf=btlink_16_16buf[data[y]];
            for(u8 y_8bit=0;y_8bit<16;y_8bit++)
            {
                switch(curr_disp_page)
                {
					case 0:
                        Lcm_Write(p_disp_buf[y_8bit]);
						break;
					case 1:
                        Lcm_Write(p_disp_buf[y_8bit+16]);
						break;
                }
			}
		}
        for(u8 i=(y_start+data_size*16);i<132;i++)
        {
            Lcm_Write(0x00);
        }
		Lcm_Stop();
	}
}
u16 BT_Phone_time=0;
u8 call_hous=0;
void clr_bt_phone_time(void)
{
    BT_Phone_time =0;
    call_hous =0;
}
void BT_Phone_disp(void)
{
	static u8 gHalfcoun =0;
	lLcdStr[0]=call_hous/10;
	lLcdStr[1]=call_hous%10;
	lLcdStr[2]='-';
	lLcdStr[3]=BT_Phone_time/2/60/10;
	lLcdStr[4]=BT_Phone_time/2/60%10;
	lLcdStr[5]='-';
	lLcdStr[6]=BT_Phone_time/2%60/10;
	lLcdStr[7]=BT_Phone_time/2%60%10;
	if(ghalf_sec!=gHalfcoun)
	{
		gHalfcoun =ghalf_sec;
		if(BT_Phone_time <3599*2)
			BT_Phone_time++;
		else
		{
			BT_Phone_time =0;
			call_hous ++;
			if(call_hous ==24)
				call_hous =0;
		}
	}
    Lcm_Disp_8_16(2,32,lLcdStr,8);
}
void lcm_disp_bt(void)
{
    switch(get_bt_connect_status())
	{
		case BT_STATUS_INITING://正在初始化
		case BT_STATUS_WAITINT_CONN://等待连接
		case BT_STATUS_AUTO_CONNECTINT://正在回连
            lLcdStr[0]=0;
            lLcdStr[1]=1;
            lLcdStr[2]=2;
            lLcdStr[3]=3;
            lLcdStr[4]=4;
            Lcm_Disp_Bt16_16(0,24,lLcdStr,5);
            if(ghalf_sec)
            {
                lLcdStr[0]=13;
            }
            else
            {
                lLcdStr[0]=17;
            }
            Lcm_Disp_Bt16_16(2,56,lLcdStr,1);
            clr_bt_phone_time();
			break;
		case BT_STATUS_CONNECTING://已经连接
            lLcdStr[0]=0;
            lLcdStr[1]=1;
            lLcdStr[2]=5;
            lLcdStr[3]=3;
            lLcdStr[4]=4;
            Lcm_Disp_Bt16_16(0,24,lLcdStr,5);
			lLcdStr[0]=13;
            Lcm_Disp_Bt16_16(2,56,lLcdStr,1);
            clr_bt_phone_time();
			break;
		case BT_STATUS_TAKEING_PHONE://正在电话
			switch(get_call_status())
			{
				case BT_CALL_INCOMING:
                    lLcdStr[0]=0;
                    lLcdStr[1]=1;
                    lLcdStr[2]=6;
                    lLcdStr[3]=7;
                    Lcm_Disp_Bt16_16(0,32,lLcdStr,4);
                    LCM_Disp_CallNum();
                    clr_bt_phone_time();
					break;
				case BT_CALL_OUTGOING:
                    lLcdStr[0]=0;
                    lLcdStr[1]=1;
                    lLcdStr[2]=8;
                    lLcdStr[3]=7;
                    Lcm_Disp_Bt16_16(0,32,lLcdStr,4);
                    LCM_Disp_CallNum();
                    clr_bt_phone_time();
					break;
				case BT_CALL_ACTIVE:
                    lLcdStr[0]=0;
                    lLcdStr[1]=1;
                    lLcdStr[2]=14;
                    lLcdStr[3]=15;
                    lLcdStr[4]=16;
                    Lcm_Disp_Bt16_16(0,24,lLcdStr,5);
                    BT_Phone_disp();
					break;
				default:
					break;
			}
			break;
		case BT_STATUS_PLAYING_MUSIC://正在音乐
            clr_bt_phone_time();
			if(is_auto_mute())
			{
                lLcdStr[0]=0;
                lLcdStr[1]=1;
                lLcdStr[2]=9;
                lLcdStr[3]=10;
                Lcm_Disp_Bt16_16(0,32,lLcdStr,4);
			}
			else
			{
                lLcdStr[0]=0;
                lLcdStr[1]=1;
                lLcdStr[2]=11;
                lLcdStr[3]=12;
                Lcm_Disp_Bt16_16(0,32,lLcdStr,4);
			}
			lLcdStr[0]=13;
            Lcm_Disp_Bt16_16(2,56,lLcdStr,1);
			break;
	}
}
#endif

