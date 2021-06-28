#include "sdk_cfg.h"
#include "app_cfg.h"
#include "error.h"
#include "stdarg.h"
#include "msg.h"
#include "timer.h"
#include "disp.h"
#include "setup.h"
#include "key_drv.h"
#include "sys_detect.h"
#include "disc/disc.h"
#include "disc/disc_state.h"
#include "disc/disc_order.h"
#include "disc_drv/disc_drv.h"


#if defined(_SUPPORT_DISC_)

#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime;
extern void Spp_Tx_disc(void);
extern void Spp_Tx_Usb(void);
extern void Spp_Tx_Sd(void);
#endif

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);


static void disc_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{
		if((msg >= MUSIC1_DISPLAY_BEGIN)&&(msg <= MUSIC1_DISPLAY_END))
		{
			disp_send_msg(DISP_EVENT_RESET);
		}
		else if((msg >= MUSIC2_DISPLAY_BEGIN)&&(msg <= MUSIC2_DISPLAY_END))
		{
			disp_send_msg(DISP_EVENT_RESET);
		}
	}
}

static uint8 disc_eject_check(uint8 run)
{
	if(run)
	{
		if(global.disc_task_name != NULL)
		{
			os_taskq_post_msg(global.disc_task_name, 1, MSG_MUSIC_EJECT);
		}
	}
	return TRUE;
}

static void disc_timer(void *param)
{
	os_taskq_post_msg(global.disc_task_name, 1, MSG_MUSIC_TIMER);
}

static void disc_task(void *name)
{
    int msg[3];
	SERVO_SOURCE_e cur_source = SERVO_SOURCE_NULL;
	SERVO_SOURCE_e servo_source = SERVO_SOURCE_NULL;
	disc_ctrl_t *disp_ctrl = NULL;
	uint16 disc_check_time = 0;
	uint16 source_check_time = 0;
#if defined(_ENABLE_RADIO_RDS_)
	uint8 rds_pause = FALSE;
#endif
	//uint8 ap_refur_timer = 0;

    disc_puts("\n***********************DISC TASK********************\n");

	disp_ctrl = (disc_ctrl_t *)malloc_fun(disp_ctrl ,sizeof(disc_ctrl_t), 0);
	if(disp_ctrl == NULL)
	{
		disc_puts("disc_task err\n");
        while(1);
	}
	
#ifdef	TD_ADD_SPP
	gAppRefreshTime =0;
#endif

	if(!strcmp(name, PLAYER_DISC_TASK_NAME))
	{
		cur_source = SERVO_SOURCE_DISC;
		disp_send_msg(DISP_EVENT_PLAYER_DISC);
		disc_check_time = 2000/DISC_POLLING_DISP_TIME;
	}
	else if(!strcmp(name, PLAYER_USB_TASK_NAME))
	{
		cur_source = SERVO_SOURCE_USB;
		disp_send_msg(DISP_EVENT_PLAYER_USB);
	}
	else if(!strcmp(name, PLAYER_CARD_TASK_NAME))
	{
		cur_source = SERVO_SOURCE_CARD;
		disp_send_msg(DISP_EVENT_PLAYER_CARD);
	}
	global.disc_task_name = name;
	disc_state_init((char *)name);
#if defined(_SERVR_USE_8202RQ_D_)
	OSTimeDly(20);	// 上电和复位同步时解码才工作，所以disc与usb和sd模式间转换需延时
#endif
	disc_servo_enable((char *)name);
	disc_audio_init();
	
	ew_printf("disc_task_name:%s\n",global.disc_task_name);
	
	timer_reg_func(disc_timer, NULL, DISC_POLLING_DISP_TIME);

#ifdef	TD_ADD_SPP
	if(compare_task_name(PLAYER_DISC_TASK_NAME))
	{
		Spp_Tx_disc();
	}
	else if(compare_task_name(PLAYER_USB_TASK_NAME))
	{
		Spp_Tx_Usb();
	}
	else if(compare_task_name(PLAYER_CARD_TASK_NAME))
	{
		Spp_Tx_Sd();
	}
#endif

    while(1)
    {
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
		clear_wdt();
		disc_reset_common(msg[0], msg[1]);

	#if defined(BACKWORD_OR_FORWARD_CANCEL_ANY_KEY)
		if((disp_ctrl->info.status == SERVO_STATUS_FORWARD)
			||(disp_ctrl->info.status == SERVO_STATUS_BACKWARD))
		{
			if((((msg[0]>=MSG_MUSIC_NEXT_FILE )&&(msg[0] <= MSG_MUSIC_EQ))
				||((msg[0]>=MSG_MUSIC_NEXT_FOLDER )&&(msg[0] <= MSG_MUSIC_ENTER)))
				&&((msg[0]!=MSG_MUSIC_FF )&&(msg[0] != MSG_MUSIC_FR)))
			{
				disc_order_send(SERVO_ORDER_PLAY);
				continue;
			}
		}
	#endif
		
        switch(msg[0])
		{
			case SYS_EVENT_DEL_TASK:
	            disc_printf("%s ---> DISC_SYS_EVENT_DEL_TASK\n", name);
							
	            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
	            	disc_audio_exit();
					timer_unreg_func(disc_timer);
					disc_drv_eject(NULL);
					global.disc_task_name = NULL;
					global.disc_time = 0;
					global.disc_count = 0;
					
	            	if(cur_source == servo_source)
	            	{
	            		uint8 flag = FALSE;

	            		if(cur_source == SERVO_SOURCE_DISC)
						{
							if(setup.disc_break & _B0_)
							{
								flag = TRUE;
							}
						}
						else if(cur_source == SERVO_SOURCE_USB)
						{
							if(setup.disc_break & _B1_)
							{
								flag = TRUE;
							}
						}
						else if(cur_source == SERVO_SOURCE_CARD)
						{
							if(setup.disc_break & _B2_)
							{
								flag = TRUE;
							}
						}

						if(flag)
						{
							disc_puts("DISC_SYS_EVENT_DEL_TASK1111\n");
							disc_order_send(SERVO_ORDER_POWER);
							OSTimeDly(30);
							disc_order_send(SERVO_ORDER_POWER);
							OSTimeDly(50);
						}
	            	}

	            	free_fun((void**)&disp_ctrl);
				#if defined(_ENABLE_ACC_OFF_RESET_)
					OSTimeDly(10);
				#endif
	            	disc_state_exit();
	            	disc_servo_disable();
	                os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case SYS_EVENT_SERVO_SEND:
				//disc_printf("%s ---> SYS_EVENT_SERVO_SEND\n", name);
				break;

			case SYS_EVENT_SERVO_RECEIVE:
				//disc_printf("%s ---> SYS_EVENT_SERVO_RECEIVE\n", name);
				disc_state_receive();
				break;

			case SYS_EVENT_SERVO_NORMAL:
				//disc_printf("%s ---> SYS_EVENT_SERVO_NORMAL\n", name);
				disc_printf("cur_source:%d\n", cur_source);
				switch(cur_source)
				{
					case SERVO_SOURCE_NULL:
						break;
					case SERVO_SOURCE_DISC:
						disc_printf("setup.disc_break:%d\n", setup.disc_break & _B0_);
						disc_order_send((setup.disc_break & _B0_) ? SERVO_ORDER_DISC_RESUME : SERVO_ORDER_DISC);
						break;
					case SERVO_SOURCE_USB:
						disc_printf("setup.disc_usb_break:%d\n", setup.disc_break & _B1_);
						disc_order_send((setup.disc_break & _B1_) ? SERVO_ORDER_USB_RESUME : SERVO_ORDER_USB);
						break;
					case SERVO_SOURCE_CARD:
						disc_printf("setup.disc_sd_break:%d\n", setup.disc_break & _B2_);
						disc_order_send((setup.disc_break & _B2_) ? SERVO_ORDER_SD_RESUME : SERVO_ORDER_SD);
						break;
				}
				source_check_time = 500/DISC_POLLING_DISP_TIME;
				break;

			case SYS_EVENT_SERVO_ESP:
				disc_printf("%s ---> SYS_EVENT_SERVO_ESP:%d\n", name, msg[1]);
				break;

			case SYS_EVENT_SERVO_SOURCE:
				disc_printf("%s ---> SYS_EVENT_SERVO_SOURCE:%d\n", name, msg[1]);
				servo_source = (SERVO_SOURCE_e)msg[1];
				if(cur_source == servo_source)
				{
					source_check_time = 0;
					if(cur_source == SERVO_SOURCE_DISC)
					{
						setup.disc_break |= _B0_;
					}
					else if(cur_source == SERVO_SOURCE_USB)
					{
						setup.disc_break |= _B1_;
					}
					else if(cur_source == SERVO_SOURCE_CARD)
					{
						setup.disc_break |= _B2_;
					}
				}
				disc_printf("setup.disc_break:%d\n",setup.disc_break);
				break;

			case SYS_EVENT_SERVO_STATUS:
				disc_printf("%s ---> SYS_EVENT_SERVO_STATUS:%d\n", name, msg[1]);
				disp_ctrl->info.status = (uint8)msg[1];
				disp_send_param(DISP_EVENT_PLAY, &disp_ctrl->info);
				break;

			case SYS_EVENT_SERVO_MODE:
				//disc_printf("%s ---> SYS_EVENT_SERVO_MODE:%d\n", name, msg[1]);
				disp_ctrl->info.mode = (uint8)msg[1];
				disp_send_param(DISP_EVENT_PLAY, &disp_ctrl->info);
				break;

			case SYS_EVENT_SERVO_SEARCH_MODE:
				//disc_printf("%s ---> SYS_EVENT_SERVO_SEARCH_MODE:%d\n", name, msg[1]);
				disp_ctrl->info.search_mode = (uint8)msg[1];

				if((disp_ctrl->info.search_mode == SERVO_SEARCH_MODE_TRK_SCH)
				 ||(disp_ctrl->info.search_mode == SERVO_SEARCH_MODE_FILE_SCH)
				 ||(disp_ctrl->info.search_mode == SERVO_SEARCH_MODE_DIR_SCH))
				{
					key_msg_select(KEY_SELECT_2NDF);
				}
				else
				{
					key_msg_select(KEY_SELECT_DEFAULT);
				}
				disp_send_param(DISP_EVENT_PLAY, &disp_ctrl->info);
				break;

			case SYS_EVENT_SERVO_SEARCH_STATUS:
				//disc_printf("%s ---> SYS_EVENT_SERVO_SEARCH_STATUS:%d\n", name, msg[1]);
				disp_ctrl->info.search_status = (uint8)msg[1];
				disp_send_param(DISP_EVENT_PLAY, &disp_ctrl->info);
				break;

			case SYS_EVENT_SERVO_MEDIA:
				disc_printf("%s ---> SYS_EVENT_SERVO_MEDIA:%d\n", name, msg[1]);
				disp_ctrl->info.media = (uint8)msg[1];
				disp_send_param(DISP_EVENT_PLAY, &disp_ctrl->info);
				break;

			case SYS_EVENT_SERVO_ERROR:
				disc_printf("%s ---> SYS_EVENT_SERVO_ERROR:%d\n", name, msg[1]);
				disp_ctrl->info.error = (uint8)msg[1];
				disp_send_param(DISP_EVENT_PLAY, &disp_ctrl->info);
			#if defined(SRVR_ERROR_TURN_MODE)
				{
					static u8 err_cnt =0 ;
					if(disp_ctrl->info.error != SERVO_ERROR_NULL)
					{
						err_cnt++;
						if(err_cnt>3)
						{
							os_taskq_post_msg("MainTask", 1, SYS_EVENT_SERVO_ERROR_MODE);
						#if defined(ERR_DISC_AUTO_EJECT)
							if(cur_source == SERVO_SOURCE_DISC)
							{
								os_taskq_post_msg("MainTask", 1, MSG_SYS_EJECT);
							}
						#endif
						}
					}
					else
					{
						err_cnt = 0;
					}
				}

			#endif
				break;

			case SYS_EVENT_SERVO_VFD:
				{
					const uint8 *buffer = (const uint8 *)msg[1];

					memcpy(disp_ctrl->info.disp_buffer, buffer, sizeof(disp_ctrl->info.disp_buffer));
					disp_ctrl->info.disp_num = 0;
				#if defined(CHANGE_DISP_BUFFER_SIZE)
					disp_ctrl->info.dot_flag = buffer[6];
				#elif defined(_SEVEN_CHAR_DISPLAY_)
					disp_ctrl->info.dot_flag = buffer[7];
				#else
					disp_ctrl->info.dot_flag = buffer[sizeof(disp_ctrl->info.disp_buffer)];
				#endif
					disp_send_param(DISP_EVENT_PLAY, &disp_ctrl->info);
				}
				break;

		#if defined(_ENABLE_RADIO_RDS_)
			case SYS_EVENT_TA_ENTER:
				disc_printf("%s ---> SYS_EVENT_TA_ENTER\n", name);
				if(disp_ctrl->info.status == SERVO_STATUS_PLAYE)
				{
					rds_pause = TRUE;
					disc_order_send(SERVO_ORDER_PAUSE);
				}
				break;

			case SYS_EVENT_TA_EXIT:
				disc_printf("%s ---> SYS_EVENT_TA_EXIT\n", name);
				if(rds_pause)
				{
					rds_pause = FALSE;
					if(disp_ctrl->info.status == SERVO_STATUS_PAUSE)
					{
						disc_order_send(SERVO_ORDER_PLAY);
					}
				}
				break;
		#endif

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

		   case MSG_MUSIC_NEXT_FILE:
		   		disc_printf("%s ---> MSG_MUSIC_NEXT_FILE\n", name);
			#if defined(RPT_ON_CHANGE_KEY_FUNCV)
				if(!global.rpt_flg)
			#endif
				disc_order_send(SERVO_ORDER_NEXT);

			#if defined(_MODIFY_MUTE_FUNTION_)
				sys_mute_ctrl(ENABLE, 1000);
			#endif
				break;

			case MSG_MUSIC_PREV_FILE:
				disc_printf("%s ---> MSG_MUSIC_PREV_FILE\n", name);
			#if defined(RPT_ON_CHANGE_KEY_FUNCV)
				if(!global.rpt_flg)
			#endif
				disc_order_send(SERVO_ORDER_PRVE);
			#if defined(_MODIFY_MUTE_FUNTION_)
				sys_mute_ctrl(ENABLE, 1000);
			#endif
				break;

			case MSG_MUSIC_PP:
			case MSG_MUSIC_PLAY:
			case MSG_MUSIC_PAUSE:
				disc_printf("%s ---> MSG_MUSIC_PLAY_PAUSE\n", name);
				disc_order_send(SERVO_ORDER_PLAY_PAUSE);
				break;

			case MSG_MUSIC_STOP:
				disc_printf("%s ---> MSG_MUSIC_STOP\n", name);
				disc_order_send(SERVO_ORDER_STOP);
				break;

			case MSG_MUSIC_FF:
				disc_printf("%s ---> MSG_MUSIC_FF\n", name);
		    #if defined(_ENABLE_A5900_USB_)
		    	if(disp_ctrl->info.status == SERVO_STATUS_FORWARD)
		    	{
		    
		    	}
		    	else
		    	{
		    		disc_order_send(SERVO_ORDER_FORWARD);
		    	}
		    #else
		    	disc_order_send(SERVO_ORDER_FORWARD);
		    #endif
				break;

			case MSG_MUSIC_FR:
				disc_printf("%s ---> MSG_MUSIC_FR\n", name);
		    #if defined(_ENABLE_A5900_USB_)
		    	if(disp_ctrl->info.status == SERVO_ORDER_BACKWARD)
		    	{
		    
		    	}
		    	else
		    	{
		    		disc_order_send(SERVO_ORDER_BACKWARD);
		    	}
		    #else
		    	disc_order_send(SERVO_ORDER_BACKWARD);
		    #endif
				break;
				
			case MSG_MUSIC_FFR_DONE:
				disc_printf("%s ---> MSG_MUSIC_FFR_DONE\n", name);
				disc_order_send(SERVO_ORDER_PLAY);
				break;

			case MSG_MUSIC_NEXT_FOLDER:
				disc_printf("%s ---> MSG_MUSIC_NEXT_FOLDER\n", name);
				disc_order_send(SERVO_ORDER_ADD_DIR);
				break;

			case MSG_MUSIC_PREV_FOLDER:
				disc_printf("%s ---> MSG_MUSIC_PREV_FOLDER\n", name);
				disc_order_send(SERVO_ORDER_SUB_DIR);
				break;

			case MSG_MUSIC_ADD_10:
				disc_printf("%s ---> MSG_MUSIC_ADD_10\n", name);
				disc_order_send(SERVO_ORDER_ADD_10);
			#if defined(_MODIFY_MUTE_FUNTION_)
				sys_mute_ctrl(ENABLE, 1000);
			#endif
				break;

			case MSG_MUSIC_SUB_10:
				disc_printf("%s ---> MSG_MUSIC_SUB_10\n", name);
				disc_order_send(SERVO_ORDER_SUB_10);
			#if defined(_MODIFY_MUTE_FUNTION_)
				sys_mute_ctrl(ENABLE, 1000);
			#endif
				break;

			case MSG_MUSIC_ID3:
				disc_printf("%s ---> MSG_MUSIC_ID3\n", name);
				disc_order_send(SERVO_ORDER_SHOW);
				break;

			case MSG_MUSIC_SEARCH:
				disc_printf("%s ---> MSG_MUSIC_SEARCH\n", name);
				disc_order_send(SERVO_ORDER_SCH);
				break;

			case MSG_MUSIC_RPT:
				disc_printf("%s ---> MSG_MUSIC_RPT\n", name);
				disc_order_send(SERVO_ORDER_PRT);
			#if defined(_MODIFY_MUTE_FUNTION_)
				sys_mute_ctrl(ENABLE, 1000);
			#endif
				break;

			case MSG_MUSIC_RDM:
				disc_printf("%s ---> MSG_MUSIC_RDM\n", name);
				disc_order_send(SERVO_ORDER_RDM);
			#if defined(_MODIFY_MUTE_FUNTION_)
				sys_mute_ctrl(ENABLE, 1000);
			#endif
				break;

			case MSG_MUSIC_INT:
				disc_printf("%s ---> MSG_MUSIC_INT\n", name);
				disc_order_send(SERVO_ORDER_INT);
				break;

			case MSG_MUSIC_PLAY_MODE:
				disc_printf("%s ---> MSG_MUSIC_PLAY_MODE\n", name);
				break;

			case MSG_MUSIC_0 ... MSG_MUSIC_9:
				disc_printf("%s ---> MSG_MUSIC_NUMBER:%d\n", name, msg[0] - MSG_MUSIC_0);
				disc_order_send(msg[0] - MSG_MUSIC_0 + SERVO_ORDER_0);
				break;

			case MSG_MUSIC_ADD:
				disc_printf("%s ---> MSG_MUSIC_ADD\n", name);
				disc_order_send(SERVO_ORDER_ADD);
				break;

			case MSG_MUSIC_SUB:
				disc_printf("%s ---> MSG_MUSIC_SUB\n", name);
				disc_order_send(SERVO_ORDER_SUB);
				break;

			case MSG_MUSIC_DELETE:
				disc_printf("%s ---> MSG_MUSIC_DELETE\n", name);
				disc_order_send(SERVO_ORDER_DELETE);
				break;

			case MSG_MUSIC_UP:
				disc_printf("%s ---> MSG_MUSIC_UP\n", name);
				disc_order_send(SERVO_ORDER_UP);
				break;

			case MSG_MUSIC_DOWN:
				disc_printf("%s ---> MSG_MUSIC_DOWN\n", name);
				disc_order_send(SERVO_ORDER_DOWN);
				break;

			case MSG_MUSIC_LEFT:
				disc_printf("%s ---> MSG_MUSIC_LEFT\n", name);
				disc_order_send(SERVO_ORDER_LEFT);
				break;

			case MSG_MUSIC_RIGHT:
				disc_printf("%s ---> MSG_MUSIC_RIGHT\n", name);
				disc_order_send(SERVO_ORDER_RIGHT);
				break;

			case MSG_MUSIC_SEL:
				disc_printf("%s ---> MSG_MUSIC_SEL\n", name);
				disc_order_send(SERVO_ORDER_SEL);
				break;

			case MSG_MUSIC_CHANNEL:
				disc_printf("%s ---> MSG_MUSIC_CHANNEL\n", name);
				disc_order_send(SERVO_ORDER_CHANNEL);
				break;

			case MSG_MUSIC_OSD:
				disc_printf("%s ---> MSG_MUSIC_OSD\n", name);
				disc_order_send(SERVO_ORDER_OSD);
				break;

			case MSG_MUSIC_PBC:
				disc_printf("%s ---> MSG_MUSIC_PBC\n", name);
				disc_order_send(SERVO_ORDER_PBC);
				break;

			case MSG_MUSIC_GOTO:
				disc_printf("%s ---> MSG_MUSIC_GOTO\n", name);
				disc_order_send(SERVO_ORDER_GOTO);
				break;

			case MSG_MUSIC_ZOOM:
				disc_printf("%s ---> MSG_MUSIC_ZOOM\n", name);
				disc_order_send(SERVO_ORDER_ZOOM);
				break;

			case MSG_MUSIC_FORMAT:
				disc_printf("%s ---> MSG_MUSIC_FORMAT\n", name);
				disc_order_send(SERVO_ORDER_FORMAT);
				break;

			case MSG_MUSIC_SETUP:
				disc_printf("%s ---> MSG_MUSIC_SETUP\n", name);
				disc_order_send(SERVO_ORDER_SETUP);
				break;

			case MSG_MUSIC_SLOW:
				disc_printf("%s ---> MSG_MUSIC_SLOW\n", name);
				disc_order_send(SERVO_ORDER_SLOW);
				break;

			case MSG_MUSIC_PROGRAM:
				disc_printf("%s ---> MSG_MUSIC_PROGRAM\n", name);
				disc_order_send(SERVO_ORDER_PROGRAM);
				break;

			case MSG_MUSIC_ANGLE:
				disc_printf("%s ---> MSG_MUSIC_ANGLE\n", name);
				disc_order_send(SERVO_ORDER_ANGLE);
				break;

			case MSG_MUSIC_TITLE:
				disc_printf("%s ---> MSG_MUSIC_TITLE\n", name);
				disc_order_send(SERVO_ORDER_TITLE);
				break;

			case MSG_MUSIC_SUB_T:
				disc_printf("%s ---> MSG_MUSIC_SUB_T\n", name);
				disc_order_send(SERVO_ORDER_SUBP_COPY);
				break;

		#if defined(PLAY_TOP_FUNCTION)
			case MSG_MUSIC_TOP:
				disc_printf("%s ---> MSG_MUSIC_TOP\n", name);
				disc_order_send(SERVO_ORDER_TOP);
				break;
		#endif
		#if defined(_SUPPORT_PLAY_MP3JPGMP4)
			case MSG_MUSIC_MP3_JPG_MP4:
				disc_printf("%s ---> MSG_MUSIC_MP3_JPG_MP4\n", name);
				disc_order_send(SERVO_ORDER_MP3JPEGMP4);
				break;
		#endif
		#if defined(_SUPPORT_PLAY_RETURN)
			case MSG_MUSIC_RETURN:
				disc_printf("%s ---> MSG_MUSIC_RETURN\n", name);
				disc_order_send(SERVO_ORDER_RETURN);
				break;
		#endif
		#if defined(_SUPPORT_PLAY_10_)
    		case MSG_MUSIC_10:
    			disc_printf("%s ---> MSG_MUSIC_10\n", name);
    			disc_order_send(SERVO_ORDER_10);
    			break;
		#endif
			case MSG_MUSIC_ENTER:
				disc_printf("%s ---> MSG_MUSIC_ENTER\n", name);
				disc_order_send(SERVO_ORDER_ENTER);
				break;

			case MSG_MUSIC_TIMER:
				//ew_puts("MSG_MUSIC_TIMER\n");

				if(global.disc_open)
				{
					if(disp_ctrl->info.status == 6)
					{
						global.disc_open_flag = TRUE;
						global.disc_count = 0;
					}
					else
					{
						global.disc_count++;
					}
				}
				else
				{
					global.disc_count = 0;
				}

				if(disc_check_time)
				{
					if(!(--disc_check_time))
					{
						disc_drv_eject(disc_eject_check);
					}
				}

				if(source_check_time)
				{
					if(!(--source_check_time))
					{
						os_taskq_post_msg(name, 1, SYS_EVENT_SERVO_NORMAL);
					}
				}
				break;

			case MSG_MUSIC_EJECT:
				disc_printf("%s ---> MSG_MUSIC_EJECT\n", name);
				//disc_order_send(SERVO_ORDER_EJECT);
				break;

		#if defined(_SUPPORT_VIDEO_)
			case MSG_VIDEO_VOLUME:
				{
					uint8 volume = (uint8)msg[1];
					disc_order_send_data(SERVO_DISP_VOL, &volume, 1);
				}
				break;

			case MSG_VIDEO_BASS:
				{
					uint8 bass = (uint8)msg[1];
					disc_order_send_data(SERVO_DISP_BASS, &bass, 1);
				}
				break;

			case MSG_VIDEO_TREBLE:
				{
					uint8 treble = (uint8)msg[1];
					disc_order_send_data(SERVO_DISP_TREBLE, &treble, 1);
				}
				break;
			case MSG_VIDEO_BAL:
				{
					uint8 bal = (uint8)msg[1];
					disc_order_send_data(SERVO_DISP_BAL, &bal, 1);
				}
				break;
			case MSG_VIDEO_FAD:
				{
					uint8 fad = (uint8)msg[1];
					disc_order_send_data(SERVO_DISP_FAD, &fad, 1);
				}
				break;
			case MSG_VIDEO_LOUD:
				{
					uint8 loud = (uint8)msg[1];
					disc_order_send_data(SERVO_DISP_LOUD, &loud, 1);
				}
				break;

			case MSG_VIDEO_EQ:
				{
					uint8 eq = (uint8)msg[1];
					disc_order_send_data(SERVO_DISP_EQ, &eq, 1);
				}
				break;
			case MSG_VIDEO_MUTE:
				{
					uint8 mute = (uint8)msg[1];
					disc_order_send_data(SERVO_DISP_MUTE, &mute, 1);
				}
				break;
		#endif
	        default:
				task_msg_default(msg[0], msg[1]);
	            break;
		}
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  DISC模式主线程初始化
   @param  priv：NULL
   @return NULL
   @note   static void disc_task_init(void *priv)
*/
/*----------------------------------------------------------------------------*/
static void disc_task_init(char *name)
{
    u32 err;

	disc_printf("disc_task_init:%s\n", name);

#ifdef DISC_DEBUG
    GET_SYS_MEM_STAT();
#endif

    err = os_task_create(disc_task,
                         name,
                         TaskDiscPrio,
                         50
					#if OS_TIME_SLICE_EN > 0
                         ,1
					#endif
                         ,name);

    if(OS_NO_ERR == err)
    {
        key_msg_register(name, KEY_PROJECT_PLAYER);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  DISC模式退出
   @param  NULL
   @return NULL
   @note   static void disc_task_exit(void)
*/
/*----------------------------------------------------------------------------*/
static void disc_task_exit(char *name)
{
	disc_puts("disc_task_exit\n");

	key_msg_register(NULL, KEY_PROJECT_MAX);

    if (os_task_del_req(name) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(name, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        }
        while(os_task_del_req(name) != OS_TASK_NOT_EXIST);
        disc_puts("close disc task succeed!\n");
    }

#ifdef DISC_DEBUG
    GET_SYS_MEM_STAT();
#endif
}

static void player_disc_task_init(void *priv)
{
	disc_task_init(PLAYER_DISC_TASK_NAME);
}
static void player_disc_task_exit(void)
{
	disc_task_exit(PLAYER_DISC_TASK_NAME);
}
TASK_REGISTER(player_disc_task_info) =
{
    .name = PLAYER_DISC_TASK_NAME,
    .init = player_disc_task_init,
    .exit = player_disc_task_exit,
};

static void player_usb_task_init(void *priv)
{
	disc_task_init(PLAYER_USB_TASK_NAME);
}
static void player_usb_task_exit(void)
{
	disc_task_exit(PLAYER_USB_TASK_NAME);
}
TASK_REGISTER(player_usb_task_info) =
{
    .name = PLAYER_USB_TASK_NAME,
    .init = player_usb_task_init,
    .exit = player_usb_task_exit,
};

static void player_card_task_init(void *priv)
{
	disc_task_init(PLAYER_CARD_TASK_NAME);
}
static void player_card_task_exit(void)
{
	disc_task_exit(PLAYER_CARD_TASK_NAME);
}
TASK_REGISTER(player_card_task_info) =
{
    .name = PLAYER_CARD_TASK_NAME,
    .init = player_card_task_init,
    .exit = player_card_task_exit,
};

#endif

