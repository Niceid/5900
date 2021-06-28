#include "sdk_cfg.h"
#include "fm_radio.h"
#include "fm_api.h"
#include "key_drv.h"
#include "disp.h"
#include "timer.h"
#include "sys_detect.h"
#include "dac/eq.h"
#include "music.h"
#include "fm_band.h"
#include "play_sel/play_sel.h"
#include "audio_drv/audio_drv.h"

#include "setup.h"
#include "audio_drv.h"


#if defined(_SUPPORT_RADIO_)
u8 radio_err=FALSE;
extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);
#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime;
extern void Spp_Tx_FM(void);
extern void TFT_Tx_System_Status(void);
#endif
#ifdef TD_ADD_NEW_APP
extern void new_app_radio_init(void);
extern void new_app_send_bandinfo(void);
extern void recv_app_freq_pro(u8 freq_h,u8 freq_l);
#endif

#ifdef	TD_ADD_LCD_TEST
extern uint8 gDispPos;
#endif
#if REC_EN
RECORD_OP_API * rec_fm_api = NULL;
#endif // REC_EN

#ifdef	TD_ADD_DISP_CHECKSUM
u8 checksum_step=0;
static const int Checksum_key[]=
{
    MSG_FM_M1,
    MSG_FM_M1,
    MSG_FM_M2,
    MSG_FM_M2,
    MSG_FM_M1,
    MSG_FM_M2,
};

void check_key(int msg)
{
    if(msg ==Checksum_key[checksum_step])
    {
		checksum_step++;
		if(checksum_step >=6)
		{
			checksum_step =0;
        	disp_send_msg(DISP_EVENT_CHECKSUM);
		}
    }
	else
	{
        checksum_step =0;
	}
}
#endif
#if defined(_SUPPORT_C912DAB_)
#if defined(TD_ADD_DAB_POWER)
void DAB_Power_On(void)
{
	DAB_POW_INIT();
	DAB_POW_OUT();
	DAB_POW_ON();
}
void DAB_Power_Off(void)
{
	DAB_POW_INIT();
	DAB_POW_OUT();
	DAB_POW_OFF();
}
#endif
#endif
#if defined(SD_FM_I2C_PIN_COMMON_USE)
extern void release_sd1_controller(void);
extern void close_sd1_controller(void);
#endif

static void radio_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{
		if((msg >= FM1_DISPLAY_BEGIN)&&(msg <= FM1_DISPLAY_END))
		{
			disp_send_msg(DISP_EVENT_RESET);
		}
		else if((msg >= FM2_DISPLAY_BEGIN)&&(msg <= FM2_DISPLAY_END))
		{
			disp_send_msg(DISP_EVENT_RESET);
		}
	}
}

static void fm_timer(void *param)
{
	os_taskq_post_msg(FM_TASK_NAME, 1, MSG_FM_TIMER);
}

#if FM_REC_EN
static bool fm_rec_msg_filter(RECORD_OP_API * rec_fm_api, int msg)
{
    if(rec_fm_api)
    {
        if(rec_fm_api->rec_ctl->enable != ENC_STOP)//正在录音，不响应以下消息
        {
            if((msg == MSG_FM_NEXT_SEEK)||(msg == MSG_FM_PREV_SEEK)||(msg == MSG_FM_SCAN)||(msg == MSG_FM_AUTO_SAVE))
            {
                return 0;
            }
        }
    }
    return 1;
}
static bool fm_rec_msg_stop(RECORD_OP_API * rec_fm_api, int msg)
{
    if(rec_fm_api)
    {
        if(rec_fm_api->rec_ctl->enable != ENC_STOP)//
        {
            return 1;
        }
    }
    return 0;
}

#endif


#if defined(_ENABLE_RECORD_RADIO_)
extern uint16 rec_time_count,rec_status;
#endif
static void fm_radio_task(void *p)
{
    int msg[3];
	//int MM[1];
    fm_puts("\n***********************RADIO TASK********************\n");
#if EQ_EN
	eq_enable();
	eq_mode = get_eq_default_mode();
#endif // EQ_EN

#if defined(_AUDIO_USE_DECODER_)
	if(setup.aud_eq == 0)
	{
		audio_drv_ctrl(AUD_CTRL_BASS,	setup.aud_bass, 	FALSE);
		audio_drv_ctrl(AUD_CTRL_TREBLE, setup.aud_treble,	FALSE);
	}
	else
	{
		audio_drv_ctrl(AUD_CTRL_EQ, 	setup.aud_eq,		FALSE);
	}
	//audio_drv_ctrl(AUD_CTRL_LOUD,	setup.aud_loud, 	FALSE);
#endif

#if defined(_SUPPORT_C912DAB_)
#if defined(TD_ADD_DAB_POWER)
	DAB_Power_On();
#endif
	memset(Disp_dab_buf,' ',32);
#endif
	disp_send_msg(DISP_EVENT_RADIO);
#ifdef	TD_ADD_SPP
	Spp_Tx_FM();
#endif
#if defined(_USER_TFT_SEND_)
	TFT_Tx_System_Status();
#endif

#if defined(_ENABLE_TONE_PLAY_)
#if defined(USB_SD_DIS_TONE_PLAY)
	extern u8 USB_SD_DIS_TONE_PLAY_FLAG;
	USB_SD_DIS_TONE_PLAY_FLAG=0;
#endif
	//dac_mute(0,1);//解码有声
	//AUDIO_MUTE_OFF();//功放有声
	sys_mute_ctrl(DISABLE, 0);
	tone_play_by_name(FM_TASK_NAME, 1, BPF_RADIO_MP3);	
	os_time_dly(100);	 //
#endif
	
#if defined(FM_INIT_MUTE_HJ)
	//sys_mute_ctrl(ENABLE, 0);
	AUDIO_MUTE_ON();
#endif

	Interrupt =FALSE;
    fm_init();

#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(fm_timer, NULL, 500);
#else
	timer_reg_func(fm_timer, NULL, RADIO_POLLING_TIME);
#endif

    while(1)
    {
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
		clear_wdt();
		//fm_printf("fm msg[0]:0x%x\n", msg[0]);
		radio_reset_common(msg[0], msg[1]);

		#if FM_REC_EN
        if(!fm_rec_msg_filter(rec_fm_api, msg[0]))//根据FM录音状态，过滤消息
            continue;
		#endif

        switch(msg[0])
		{
			case SYS_EVENT_DEL_TASK:
	            fm_puts("fm_radio ---> get_SYS_EVENT_DEL_TASK\n");
			#if FM_REC_EN
		        if(fm_rec_msg_stop(rec_fm_api, msg[0]))//转模式停止录音
		        {
					msg[0]=MSG_REC_STOP;
		            rec_msg_deal_api(&rec_fm_api, msg);
		        }
			#endif
				
	            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
				#ifdef	TD_ADD_DISP_CHECKSUM
					checksum_step=0;
				#endif
				#if EQ_EN
					eq_disable();
				#endif/*EQ_EN*/
	            	timer_unreg_func(fm_timer);
	                fm_exit();
				#if defined(TD_ADD_DAB_POWER)
					DAB_Power_Off();
				#endif
	                fm_puts("fm_rev_powerdown\n");
	                os_task_del_res_name(OS_TASK_SELF);
	            }
				
				#if defined(SD_FM_I2C_PIN_COMMON_USE)
					release_sd1_controller();
				#endif
	            break;

			case MSG_FM_PREV_STATION:
				fm_puts("MSG_FM_PREV_STATION\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
				checksum_step=0;
			#endif
			#if defined(TD_ADD_CHADD_SUB)
                if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
                {
                	Radio_CH_Sub();
                }
                else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
                {
                	Radio_Ch_Save();
                }
			#else
        		cmd_radio_load_prev();
			#endif
				break;

			case MSG_FM_NEXT_STATION:
				fm_puts("MSG_FM_NEXT_STATION\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
				checksum_step=0;
			#endif
			#if defined(TD_ADD_CHADD_SUB)
                if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
                {
                	Radio_CH_Add();
                }
                else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
                {
                	Radio_Ch_Save();
                }
			#else
        		cmd_radio_load_next();
			#endif
				break;
			
		#if defined(_ENABLE_SAVE_FREQ_)
			case MSG_FM_SLECT_UP:
				fm_puts("MSG_FM_SLECT_UP\n");
			#if defined(_ZSBY5209_VOLUME_FUNTION_)
				cmd_radio_slect_up();
			#else
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					fm_puts("MSG_FM_SLECT_UP111\n");
					cmd_radio_slect_up();
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					fm_puts("MSG_FM_SLECT_UP2222\n");
					cmd_radio_save_up();
				}
			#endif
				break;
			
			case MSG_FM_SLECT_DOWN:
				fm_puts("MSG_FM_SLECT_DOWN\n");
			#if defined(_ZSBY5209_VOLUME_FUNTION_)
				cmd_radio_slect_down();
			#else
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					cmd_radio_slect_down();
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					cmd_radio_save_down();
				}
			#endif
				break;
		#endif
		
		#if defined(_MR889_SPECAIL_KEY_)
			case MSG_FM_SLECT_UP1:
				fm_puts("MSG_FM_SLECT_UP\n");
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					fm_puts("MSG_FM_SLECT_UP111\n");
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_SEEK);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					fm_puts("MSG_FM_SLECT_UP2222\n");
					cmd_radio_slect_up();
				}
				break;
			
			case MSG_FM_SLECT_DOWN1:
				fm_puts("MSG_FM_SLECT_DOWN\n");
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_SEEK);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					cmd_radio_slect_down();
				}
				break;
		#endif
		
	#if defined(_EVB_SEEK_FUNTION_)
    	case MSG_FM_PREV_STEP1:
    		fm_puts("MSG_FM_PREV_STEP1\n");
    		cmd_radio_freq_prev1();
    		break;
    	
    	case MSG_FM_NEXT_STEP1:
    		fm_puts("MSG_FM_NEXT_STEP1\n");
    		cmd_radio_freq_next1();
    		break;
	#endif
	
	#if defined(_XZC1668_RADIO_FUNTION_)
    	case MSG_FM_FM_BAND:
    		fm_puts("MSG_FM_FM_BAND\n");
    		cmd_radio_fm();
    		break;
    	
    	case MSG_FM_AM_BAND:
    		fm_puts("MSG_FM_AM_BAND\n");
    		cmd_radio_am();
    		break;
	#endif
	
		 case SYS_EVENT_PLAY_SEL_END://提示音播完会到这里
	            music_puts("FM----SYS_EVENT_PLAY_SEL_END---\n");
				
				break;
			case MSG_FM_PREV_SEEK:
		    #if defined(_ENABLE_WB_FUNTION_)
		    	if(RADIO_STATE_WB)break;
		    #endif

			#ifdef	TD_ADD_LCD_TEST
				gDispPos --;
				printf("-----------------------------------------\n");
				printf("gDispPos=%d\n",gDispPos);

			#else
				fm_puts("MSG_FM_PREV_SEEK\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
			
			#if defined(_SUPPORT_C912DAB_)
				if(gC912detOk)
				{
                    uart_interrupt_receive_write(0x42);
                    uart_interrupt_receive_write(0x02);
				}
			#else
                disp_func_reset();
				cmd_radio_seek_prev();
			#endif
		#endif
				break;

			case MSG_FM_NEXT_SEEK:
		    #if defined(_ENABLE_WB_FUNTION_)
		    	if(RADIO_STATE_WB)break;
		    #endif

			#ifdef	TD_ADD_LCD_TEST
				gDispPos ++;
				printf("++++++++++++++++++++++++++++++++++++++\n");
				printf("gDispPos=%d\n",gDispPos);

			#else
				fm_puts("MSG_FM_NEXT_SEEK\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
			#if defined(_SUPPORT_C912DAB_)
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x41);
					uart_interrupt_receive_write(0x01);
				}
			#else
                disp_func_reset();
				cmd_radio_seek_next();
			#endif
		#endif
				break;

			case MSG_FM_PREV_STEP:
				fm_puts("MSG_FM_PREV_STEP\n");
		    #if defined(_ENABLE_WB_FUNTION_)
		    	if(RADIO_STATE_WB)
		    	{
		    		cmd_radio_slect_down();
		    	}
		    	else
		    #endif
		    	{
			    #ifdef	TD_ADD_DISP_CHECKSUM
			    	checksum_step=0;
			    #endif
			    #if defined(_SUPPORT_C912DAB_)
			    	if(gC912detOk)
			    	{
			    		uart_interrupt_receive_write(0x41);
			    		uart_interrupt_receive_write(0x01);
			    		uart_interrupt_receive_write(0x42);
			    		uart_interrupt_receive_write(0x02);
			    		uart_interrupt_receive_write(0x50);
			    		uart_interrupt_receive_write(0x10);
			    	}
			    #else
			    	cmd_radio_freq_prev();
			    #endif
				}
				break;

			case MSG_FM_NEXT_STEP:
				fm_puts("MSG_FM_NEXT_STEP\n");
		    #if defined(_ENABLE_WB_FUNTION_)
		    	if(RADIO_STATE_WB)
		    	{
		    		cmd_radio_slect_up();
		    	}
		    	else
		    #endif
				{
			    #ifdef	TD_ADD_DISP_CHECKSUM
			    	checksum_step=0;
			    #endif
			    #if defined(_SUPPORT_C912DAB_)
			    	if(gC912detOk)
			    	{
			    		uart_interrupt_receive_write(0x42);
			    		uart_interrupt_receive_write(0x02);
			    		uart_interrupt_receive_write(0x41);
			    		uart_interrupt_receive_write(0x01);
			    		uart_interrupt_receive_write(0x50);
			    		uart_interrupt_receive_write(0x10);
			    	}
			    #else
			    	cmd_radio_freq_next();
			    #endif
				}
				break;

			case MSG_FM_AUTO_SAVE:
				fm_puts("MSG_FM_AUTO_SAVE\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
			
			#if defined(_SUPPORT_C912DAB_)
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x48);
					uart_interrupt_receive_write(0x08);//全自动搜台
				}
				else
			#endif
                {
					cmd_radio_auto_save();
				}
				break;

			case MSG_FM_BROWSE:
				fm_puts("MSG_FM_BROWSE\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
				cmd_radio_browse();
				break;

			case MSG_FM_SCAN:
				fm_puts("MSG_FM_SCAN\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
				cmd_radio_scan();
				break;

			case MSG_FM_BAND:
				//MM[0]=MSG_REC_START;
				//rec_msg_deal_api(&rec_fm_api, MM);
				fm_puts("MSG_FM_BAND\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
				
			#if defined(_SUPPORT_C912DAB_)
				if(gC912detOk)
				{
                    if(GET_RADIO_BAND <=2)
                    {
                        SET_RADIO_BAND(3);
                    }
                    else
                    {
                        SET_RADIO_BAND(0);
                    }
	                radio_engine_mute(ON);
                    AUDIO_MUTE_ON();
	                radio_engine_init();
                    AUDIO_MUTE_OFF();
                }
			#else
				cmd_radio_band();
			#endif
				break;

			case MSG_FM_LOC:
				fm_puts("MSG_FM_LOC\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
				cmd_radio_loc();
				break;

			case MSG_FM_ST:
				fm_puts("MSG_FM_ST\n");
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
				cmd_radio_stereo();
				break;

        #if defined(_ENABLE_RADIO_RDS_)
			enum{
				RADIO_RDS_CTRL_NULL = 0,
				RADIO_RDS_CTRL_ADD,
				RADIO_RDS_CTRL_SUB,
				RADIO_RDS_CTRL_NEXT,
				RADIO_RDS_CTRL_PREV,
				RADIO_RDS_CTRL_M1,
				RADIO_RDS_CTRL_M2,
				RADIO_RDS_CTRL_M3,
				RADIO_RDS_CTRL_M4,
				RADIO_RDS_CTRL_M5,
				RADIO_RDS_CTRL_M6,
			#if defined(_ENABLE_NEW_APP_RDS_)
				RADIO_RDS_CTRL_PTY,
			#endif
			};

    		case MSG_RDS_PTY:
    			fm_puts("MSG_RDS_PTY\n");
				disp_send_param(DISP_EVENT_PTY, RADIO_RDS_CTRL_ADD);
    			break;

			case MSG_RDS_NEXT:
				fm_puts("MSG_RDS_NEXT\n");
				disp_send_param(DISP_EVENT_PTY, RADIO_RDS_CTRL_NEXT);
				break;

			case MSG_RDS_BACK:
				fm_puts("MSG_RDS_BACK\n");
				disp_send_param(DISP_EVENT_PTY, RADIO_RDS_CTRL_PREV);
				break;

            case MSG_RDS_M1 ... MSG_RDS_M6:
            	fm_printf("MSG_RDS_M%d\n", msg[0] - MSG_RDS_M1 + 1);
				disp_send_param(DISP_EVENT_PTY, msg[0] - MSG_RDS_M1 + RADIO_RDS_CTRL_M1);
            	break;
				
		#if defined(_ENABLE_NEW_APP_RDS_)
    		case MSG_RDS_PTY_TYPE:
    			fm_puts("MSG_RDS_PTY_TYPE\n");
    			disp_send_param(DISP_EVENT_PTY, RADIO_RDS_CTRL_PTY);
    			break;
		#endif

			case MSG_FM_RDS_PSN:
				/*fm_printf("MSG_FM_RDS_PSN:%c%c%c%c%c%c%c%c\n",  ((uint8 *)msg[1])[0],
																((uint8 *)msg[1])[1],
																((uint8 *)msg[1])[2],
																((uint8 *)msg[1])[3],
																((uint8 *)msg[1])[4],
																((uint8 *)msg[1])[5],
																((uint8 *)msg[1])[6],
																((uint8 *)msg[1])[7]);*/
				cmd_radio_psn((uint8 *)msg[1]);
				break;

			case MSG_FM_RDS_PTY:
		    	fm_printf("MSG_FM_RDS_PTY:%d\n", msg[1]);
		    	if(msg[1])
		    	{
		    		key_msg_select(KEY_SELECT_2NDF);
		    	}
		    	else
		    	{
		    		key_msg_select(KEY_SELECT_DEFAULT);
		    	}
            	break;
        #endif


		#if defined(_SUPPORT_C912DAB_)
			case MSG_FM_M1:
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x47);
					uart_interrupt_receive_write(0x07);//查看电台信息INFO
				}
				break;
			case MSG_FM_M2:
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x49);
					uart_interrupt_receive_write(0x09);//进入或退出菜单MENU
				}
				break;
			case MSG_FM_M3:
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x4b);
					uart_interrupt_receive_write(0x0b);//返回上一层菜单或退出
				}
				break;
			case MSG_FM_M4:
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x45);
					uart_interrupt_receive_write(0x05);//取台列表清单
				}
				break;
			case MSG_FM_M5:
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x46);
					uart_interrupt_receive_write(0x06);//存台列表清单
				}
				break;
		#else
		
			case MSG_FM_M1 ... MSG_FM_M6:
				fm_puts("MSG_FM_M\n");
				cmd_radio_load_number((radio_sel_e)(msg[0] - MSG_FM_M1 + RADIO_SEL_M1));
			#ifdef	TD_ADD_DISP_CHECKSUM
                check_key(msg[0]);
			#endif
				break;
		#endif
			case MSG_FM_S1 ... MSG_FM_S6:
				fm_puts("MSG_FM_S\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
			#endif
				cmd_radio_save_number((radio_sel_e)(msg[0] - MSG_FM_S1 + RADIO_SEL_M1));
			#ifdef TD_ADD_NEW_APP
				new_app_send_bandinfo();//每次存台需要重置6个电台
			#endif
				break;
			
	    #if defined(_SPECIAL_NUMBER_SAVE_)
	    	case MSG_FM_M7:
	    		fm_puts("MSG_FM_M7\n");
	    		cmd_radio_load_number((radio_sel_e)(7));
	    		break;
	    	case MSG_FM_M8:
	    		fm_puts("MSG_FM_M8\n");
	    		cmd_radio_load_number((radio_sel_e)(8));
	    		break;
	    	case MSG_FM_M9:
	    		fm_puts("MSG_FM_M9\n");
	    		cmd_radio_load_number((radio_sel_e)(9));
	    		break;
	    	case MSG_FM_M10:
	    		fm_puts("MSG_FM_M10\n");
	    		cmd_radio_load_number((radio_sel_e)(10));
	    		break;
	    
	    	case MSG_FM_S7:
	    		fm_puts("MSG_FM_S7\n");
	    		cmd_radio_save_number((radio_sel_e)(7));
	    		break;
	    	case MSG_FM_S8:
	    		fm_puts("MSG_FM_S8\n");
	    		cmd_radio_save_number((radio_sel_e)(8));
	    		break;
	    	case MSG_FM_S9:
	    		fm_puts("MSG_FM_S9\n");
	    		cmd_radio_save_number((radio_sel_e)(9));
	    		break;
	    	case MSG_FM_S10:
	    		fm_puts("MSG_FM_S10\n");
	    		cmd_radio_save_number((radio_sel_e)(10));
	    		break;
	    #endif

	        case MSG_FM_TIMER:
		    #if defined(_ENABLE_RECORD_RADIO_)
		    	if(rec_status == rec_start)
		    	{
		    		rec_time_count++;
		    	} 
		    #endif

			#if defined(_SUPPORT_C912DAB_)
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x71);
					uart_interrupt_receive_write(0x31);
				}
			#else
				radio_polling_time();
			#endif
	            break;

			case MSG_FM_ERR:
				printf("radio err\n");
		#ifdef	TD_ADD_DISP_CHECKSUM
                checksum_step=0;
		#endif
				fm_exit();
				fm_init();
				break;
		#if defined(_SUPPORT_C912DAB_)
			case MSG_FM_DAB_ENTER:
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x4a);
					uart_interrupt_receive_write(0x0a);
				}
				break;
		#endif
		#ifdef  TD_ADD_NEW_APP
            case MSG_FM_SELECT_FREQ:
                recv_app_freq_pro(msg[1],msg[2]);
            	break;
		#endif
	        default:
	        #if FM_REC_EN
				if(GET_RADIO_STATE == RADIO_STATE_IDLE)
				{
					rec_msg_deal_api(&rec_fm_api, msg);
				}
			#else
				task_msg_default(msg[0], msg[1]);
			#endif
	        break;
		}
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  FM模式主线程初始化
   @param  priv：NULL
   @return NULL
   @note   static void fm_radio_task_init(void *priv)
*/
/*----------------------------------------------------------------------------*/
static void fm_radio_task_init(void *priv)
{
    u32 err;

	fm_puts("fm_radio_task_init\n");
	
#if defined(SYS_IO_OUT_FM_CLK)
	SYS_IO_OUT_FM_CLK_ENABLE();
#endif
	
#if defined(FM_INIT_OK_FLAG)
	global.fm_init_ok_flag=1;
#endif

#ifdef FM_DEBUG
    GET_SYS_MEM_STAT();
#endif
#if defined(SD_FM_I2C_PIN_COMMON_USE)
	close_sd1_controller();
#endif
    err = os_task_create(fm_radio_task,
                         (void*)0,
                         TaskFMradioPrio,
                         50
					#if OS_TIME_SLICE_EN > 0
                         ,1
					#endif
                         ,FM_TASK_NAME);

    if(OS_NO_ERR == err)
    {
        key_msg_register(FM_TASK_NAME, KEY_PROJECT_RADIO);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  FM模式退出
   @param  NULL
   @return NULL
   @note   static void fm_radio_task_exit(void)
*/
/*----------------------------------------------------------------------------*/
static void fm_radio_task_exit(void)
{
    /*先关闭FM模块线程，再关闭FM变采样线程*/
	fm_puts("fm_radio_task_exit\n");

	key_msg_register(NULL, KEY_PROJECT_MAX);
#if defined(SD_FM_I2C_PIN_COMMON_USE)
	//close_sd1_controller();
#endif
#if defined(SYS_IO_OUT_FM_CLK)
	SYS_IO_OUT_FM_CLK_DISENABLE();
#endif
	
#if defined(AUDIO_MUTE_SPECIAL_SET)
	HAUDIO_MUTE_OFF();
#endif
#if defined(FM_INIT_OK_FLAG)
	global.fm_init_ok_flag=0;
#endif
    if (os_task_del_req(FM_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(FM_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        }
        while(os_task_del_req(FM_TASK_NAME) != OS_TASK_NOT_EXIST);
        fm_puts("clos_fm_task_succ-01\n");
    }

#ifdef FM_DEBUG
    GET_SYS_MEM_STAT();
#endif
}


TASK_REGISTER(fm_radio_task_info) =
{
    .name = FM_TASK_NAME,
    .init = fm_radio_task_init,
    .exit = fm_radio_task_exit,
};

#endif

