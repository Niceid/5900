#include "sdk_cfg.h"
#include "dab/dab.h"
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
#include "ui_api.h"
#include "sdk_cfg.h"
#include "setup.h"
#include "sys_detect.h"

#include "tape\tape.h"


//#define TAPE_DEBUG
#ifdef TAPE_DEBUG
    #define tape_puts     ew_puts
    #define tape_printf   ew_printf
#else
    #define tape_puts(...)
    #define tape_printf(...)
#endif


#if defined(_SUPPORT_TAPE_)

#if defined(_SUPPORT_RECORD_)
RECORD_OP_API * rec_tape_api = NULL;
#endif

static void tape_dac_channel_off(void)
{
	sys_mute_ctrl(ENABLE, 0);
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_TAPE_OFF, TRUE);
}

static void tape_info_init(void)
{
    sys_mute_ctrl(ENABLE, 1000);
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_TAPE_ON, TRUE);
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
}

static void tape_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{

	}
}

#if defined(_SUPPORT_RECORD_)
static bool tape_rec_msg_stop(RECORD_OP_API * rec_tape_api, int msg)
{
    if(rec_tape_api)
    {
        if(rec_tape_api->rec_ctl->enable != ENC_STOP)//
        {  
            return 1;
        }
    }
    return 0;
}
extern uint16 rec_time_count,rec_status;
#endif

static void tape_timer(void *param)
{
	os_taskq_post_msg(TAPE_TASK_NAME, 1, MSG_TAPE_TIMER);
}

#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime ;
void Spp_Tx_Tape(void);
#endif

static void tape_task(void *p)
{
    int msg[6];

    tape_puts("\n************************TAPE TASK********************\n");
	
#if EQ_EN
	eq_enable();
	eq_mode = get_eq_default_mode();
#endif // EQ_EN
    malloc_stats();

	disp_send_msg(DISP_EVENT_TAPE);
	os_taskq_post_msg(TAPE_TASK_NAME, 1, MSG_TAPE_INIT);
	//timer_reg_func(tape_timer, NULL, DAB_POLLING_TIME);
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(tape_timer, NULL, 2000);
#else
	timer_reg_func(tape_timer, NULL, 1000);
#endif
	
#ifdef	TD_ADD_SPP
	Spp_Tx_Tape();
#endif

    while(1)
    {
        memset(msg, 0x00, sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
		tape_reset_common(msg[0], msg[1]);

		switch(msg[0])
        {
			case MSG_TAPE_TIMER://清狗消息
				//printf("rec_task time\n");
			#if defined(_SUPPORT_RECORD_)
				if(rec_status == rec_start)
				{
					rec_time_count++;
				}	
			#endif
				break;
				
            case SYS_EVENT_DEL_TASK:
				tape_puts("tape--->SYS_EVENT_DEL_TASK\n");
				TAPE_POWER_OFF();
		    #if defined(_SUPPORT_RECORD_)
		    	if(tape_rec_msg_stop(rec_tape_api, msg[0]))//转模式停止录音
		    	{
		    		msg[0]=MSG_REC_STOP;
		    		rec_msg_deal_api(&rec_tape_api, msg);
		    	}
		    #endif
			    if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
			    {
			    #if EQ_EN
			    	eq_disable();
			    #endif/*EQ_EN*/
			    	timer_unreg_func(tape_timer);
			    	tape_dac_channel_off();
			    	tape_puts("TAPE_SYS_EVENT_DEL_TASK\n");
			    	os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case MSG_TAPE_INIT:
	            tape_puts("MSG_TAPE_INIT\n");
				TAPE_POWER_ON();
	            tape_info_init();
	            break;

            default:
			#if defined(_SUPPORT_RECORD_)
			    rec_msg_deal_api(&rec_tape_api, msg);
			#else
	            task_msg_default(msg[0], msg[1]);
			#endif
	            break;
        }
    }
}

static void tape_task_init(void *priv)
{
	tape_puts("tape_task_init\n");

    u32 err;
    err = os_task_create_ext(tape_task,
						(void*) 0,
						TaskTapePrio,
						10
					#if OS_TIME_SLICE_EN > 0
			            ,1
					#endif
            			,TAPE_TASK_NAME
            			,LINEIN_TASK_STACK_SIZE);

    if (OS_NO_ERR == err)
    {
        key_msg_register(TAPE_TASK_NAME, KEY_PROJECT_TAPE);
    }
}

static void tape_task_exit(void)
{
	tape_puts("tape_task_exit\n");
	key_msg_register(NULL, KEY_PROJECT_MAX);
    if (os_task_del_req(TAPE_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(TAPE_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        } while (os_task_del_req(TAPE_TASK_NAME) != OS_TASK_NOT_EXIST);
    }
}

TASK_REGISTER(tape_task_info) =
{
    .name = TAPE_TASK_NAME,
    .init = tape_task_init,
    .exit = tape_task_exit,
};

#endif

