#include "sdk_cfg.h"
#include "linein.h"
#include "dac/dac_api.h"
#include "key_drv.h"
#include "dac/dac_api.h"
#include "dac/ladc.h"
#include "encode/encode.h"
#include "dac/dac.h"
#include "dac/dac_api.h"
#include "rcsp/rcsp_interface.h"
#include "sys_detect.h"
#include "disp.h"
#include "timer.h"
#include "clock.h"
#include "audio_drv.h"
#include "dac/eq.h"
#include "music.h"
#include "setup.h"
#include "audio_drv.h"
#include "play_sel/play_sel.h"

#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime;
extern void Spp_Tx_Aux(void);
#endif
#if USE_SOFTWARE_EQ 
static void aux_eq_task_init();
static void aux_eq_task_stop();
static void aux_eq_run(void *p);
#endif
#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_System_Status(void);
#endif

RECORD_OP_API * rec_aux_api = NULL;

extern void *malloc_fun(void *ptr, u32 len, char *info);
extern void free_fun(void **ptr);
#define get_mute_status   is_dac_mute


static void aux_dac_channel_off(void)
{
#if !defined(_ENABLE_SPECIAL_MUTE_)
#if defined(_DISABLE_POWER_OFF_MUTE_)
	if(global.power_mute)
	{
		global.power_mute = FALSE;
	}
	else
	{
		sys_mute_ctrl(ENABLE, 0);
	}
#else
#if !defined(_AUX_MODE_MUTE_)
	sys_mute_ctrl(ENABLE, 0);
#endif
#endif
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
#if !defined(_ENABLE_MODE_CHANNEL_)
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);
#endif

#if defined(_SUPPORT_TWO_PT2313_)
	audio_drv_ctrl1(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);
#endif
}

static void aux_info_init(void)
{
#if defined(_RADIO_RESET_HIGH_)
	RADIO_RESET_INIT();
	RADIO_RESET_OFF();
#endif

#if defined(_AUX_TONE_PLAY_)
#if defined(USB_SD_DIS_TONE_PLAY)
	extern u8 USB_SD_DIS_TONE_PLAY_FLAG;
	USB_SD_DIS_TONE_PLAY_FLAG=0;
#endif
	//dac_mute(0,1);//解码有声
	//AUDIO_MUTE_OFF();//功放有声
	sys_mute_ctrl(DISABLE, 0);
	tone_play_by_name(LINEIN_TASK_NAME, 1, BPF_LINEIN_MP3); 
	os_time_dly(150);	 //
#endif

#if defined(_SPECIAL_RDS_DELAY_)
	OSTimeDly(10);
#endif

#if defined(_ENABLE_SPECIAL_MUTE_)
	sys_mute_ctrl(DISABLE, 1);
#else
	sys_mute_ctrl(ENABLE, 500);
#endif

#if defined(_DISABLE_AMP_OFF_)
	if(global.amp_flag)
	{
		AMP_CONTROL_ON();
		global.amp_flag = FALSE;
	}
#endif

#if defined(_ENABLE_MODE_CHANNEL_)
	if(setup.mode_start == 0)
	{
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_ON, TRUE);
	}
	else if(setup.mode_start == 1)
	{
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);//OPTICAL
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_OPTICAL_ON, TRUE);
	}
	else if(setup.mode_start == 2)
	{
    #if defined(_DISBLE_MODE_OPTICAL_)
       audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);
    #endif
    	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_TV_ON, TRUE);//TV
	}
	else if(setup.mode_start == 3)
	{
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DVD_ON, TRUE);//DVD
	}
#else
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_ON, TRUE);
#endif

#if defined(_AUX_REDUCE_AUD_GATE_)||defined(_W116_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#elif defined(_ZZ158_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#elif defined(_REDUCE_AUX_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#elif defined(SH1308F_REDUCE_AUX_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_0, TRUE);
#elif defined(JYL3861E_CHANNEL_PLAY_GATE)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_0, TRUE);
#elif defined(_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_0, TRUE);
#else
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
#endif

#if defined(_DECODER_MUSIC_GAIN_)
	audio_drv_ctrl(AUD_CTRL_LR, setup.aud_bal, TRUE);
	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
#endif

#if defined(_SUPPORT_TWO_PT2313_)
    audio_drv_ctrl1(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_ON, TRUE);
    audio_drv_ctrl1(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#endif

#if defined(_ENABLE_POWER_LOGO_)
	global.power_up_flag = FALSE;
#endif
}

static void linein_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{

	}
}

static void linein_timer(void *param)
{
	os_taskq_post_msg(LINEIN_TASK_NAME, 1, MSG_AUX_TIMER);
}
#if AUX_REC_EN
static bool aux_rec_msg_stop(RECORD_OP_API * rec_aux_api, int msg)
{
    if(rec_aux_api)
    {
        if(rec_aux_api->rec_ctl->enable != ENC_STOP)//
        {  
            return 1;
        }
    }
    return 0;
}
#if defined(_ENABLE_MIC_REC_)
extern uint16 rec_time_count,rec_status;
#endif

#endif


static void linein_task(void *p)
{
    int msg[6];

    aux_puts("\n************************LINEIN TASK********************\n");
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
	//audio_drv_ctrl(AUD_CTRL_LOUD, setup.aud_loud, 	FALSE);
#endif
    malloc_stats();

	disp_send_msg(DISP_EVENT_AUX);
	os_taskq_post_msg(LINEIN_TASK_NAME, 1, MSG_AUX_INIT); //触发AUX模块初始化
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(linein_timer, NULL, 200);
#elif defined(_ENABLE_MIC_REC_)
    timer_reg_func(linein_timer, NULL, 1000);
#else
	timer_reg_func(linein_timer, NULL, LINEIN_POLLING_TIME);
#endif
	
#ifdef	TD_ADD_SPP
	Spp_Tx_Aux();
#endif
#if defined(_USER_TFT_SEND_)
	TFT_Tx_System_Status();
#endif

    while(1)
    {
        memset(msg, 0x00, sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
		linein_reset_common(msg[0], msg[1]);

		switch(msg[0])
        {
            case SYS_EVENT_DEL_TASK:
				aux_puts("linein--->SYS_EVENT_DEL_TASK\n");
			#if AUX_REC_EN
		        if(aux_rec_msg_stop(rec_aux_api, msg[0]))//转模式停止录音
		        {
					msg[0]=MSG_REC_STOP;
		            rec_msg_deal_api(&rec_aux_api, msg);
		        }
			#endif
			
		    #if defined(_ENABLE_AUX_MUTE_)
		    	sys_mute_ctrl(ENABLE, 0);
		    #endif
			
			#if defined(_ENABLE_MUTE_AUX_)
    			u8 mute_send_buf[11]={0x05,0x00,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x00,0x00};
				extern void Audio_Send_Data(uint8 *data,uint8 length);
    			Audio_Send_Data(mute_send_buf,11);
			#endif

	            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
			    #if defined(_SPECIAL_RDS_DELAY_)
			    	OSTimeDly(15);
			    #endif
				#if EQ_EN
					eq_disable();
				#endif/*EQ_EN*/
					timer_unreg_func(linein_timer);
	                aux_dac_channel_off();
					aux_puts("AUX_SYS_EVENT_DEL_TASK\n");
					os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case MSG_AUX_INIT:
	            aux_puts(">MSG_AUX_INIT\n");
	            aux_info_init();
	            break;

            case MSG_AUX_MUTE:
	            if(get_mute_status())
                {
                    aux_puts("MSG_AUX_UNMUTE\n");
                    dac_mute(0,1);
                }
                else
                {
                    aux_puts("MSG_AUX_MUTE\n");
                    dac_mute(1,1);
                }
            	break;

			case MSG_LOW_POWER:
                aux_puts("aux_low_power\n");
                break;

			case MSG_AUX_TIMER:
			#if defined(_ENABLE_MIC_REC_)
				if(rec_status == rec_start)
				{
					rec_time_count++;
				}	
			#endif
				break;

            default:
			#if AUX_REC_EN
				//echo_msg_deal_api((void **)&aux_reverb, msg);
			    rec_msg_deal_api(&rec_aux_api, msg);
			#else
	            task_msg_default(msg[0], msg[1]);
	        #endif			
	            break;
        }
    }
}

static void linein_task_init(void *priv)
{
	aux_puts("linein_task_init\n");

	//DSP_RCA_MUTE_ON();

    u32 err;
	
    err = os_task_create_ext(linein_task,
						(void*) 0,
						TaskLineinPrio,
						10
					#if OS_TIME_SLICE_EN > 0
			            ,1
					#endif
            			,LINEIN_TASK_NAME
            			,LINEIN_TASK_STACK_SIZE);

    if (OS_NO_ERR == err)
    {
        key_msg_register(LINEIN_TASK_NAME, KEY_PROJECT_AUX);
    }
}

static void linein_task_exit(void)
{
	aux_puts("linein_task_exit\n");

    if (os_task_del_req(LINEIN_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(LINEIN_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        } while (os_task_del_req(LINEIN_TASK_NAME) != OS_TASK_NOT_EXIST);
        aux_puts("del_linein_task: succ\n");
    }
}

TASK_REGISTER(linein_task_info) =
{
    .name = LINEIN_TASK_NAME,
    .init = linein_task_init,
    .exit = linein_task_exit,
};





