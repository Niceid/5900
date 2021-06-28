#include "sdk_cfg.h"
#include "linein2\linein2.h"
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


#if defined(_SUPPORT_AUX1_)	

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

extern void *malloc_fun(void *ptr, u32 len, char *info);
extern void free_fun(void **ptr);
#define get_mute_status   is_dac_mute

static void aux1_dac_channel_off(void)
{

	sys_mute_ctrl(ENABLE, 0);
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX1_OFF, TRUE);
}

static void aux1_info_init(void)
{

	sys_mute_ctrl(ENABLE, 500);
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX1_ON, TRUE);
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
	//audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
}

static void linein1_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{

	}
}

static void linein1_timer(void *param)
{
	os_taskq_post_msg(LINEIN1_TASK_NAME, 1, MSG_AUX1_TIMER);
}

static void linein2_task(void *p)
{
    int msg[6];

    aux1_puts("\n************************LINEIN2 TASK********************\n");
#if EQ_EN
	eq_enable();
	eq_mode = get_eq_default_mode();
#endif // EQ_EN
    malloc_stats();

	disp_send_msg(DISP_EVENT_AUX1);
	os_taskq_post_msg(LINEIN1_TASK_NAME, 1, MSG_AUX1_INIT); //触发AUX模块初始化
	timer_reg_func(linein1_timer, NULL, LINEIN1_POLLING_TIME);
	
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
		linein1_reset_common(msg[0], msg[1]);

		switch(msg[0])
        {
            case SYS_EVENT_DEL_TASK:
	            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
			    #if defined(_SPECIAL_RDS_DELAY_)
			    	OSTimeDly(15);
			    #endif
				#if EQ_EN
					eq_disable();
				#endif/*EQ_EN*/
					timer_unreg_func(linein1_timer);
	                aux1_dac_channel_off();
					os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case MSG_AUX1_INIT:
	            aux1_info_init();
	            break;

            case MSG_AUX1_MUTE:
	            if(get_mute_status())
                {
                    dac_mute(0,1);
                }
                else
                {
                    dac_mute(1,1);
                }
            	break;

			case MSG_LOW_POWER:
                break;

			case MSG_AUX1_TIMER:
				break;

            default:
	            task_msg_default(msg[0], msg[1]);
	            break;
        }
    }
}

static void linein2_task_init(void *priv)
{
    u32 err;
	
    err = os_task_create_ext(linein2_task,
						(void*) 0,
						TaskLinein2Prio,
						10
					#if OS_TIME_SLICE_EN > 0
			            ,1
					#endif
            			,LINEIN1_TASK_NAME
            			,LINEIN_TASK_STACK_SIZE);

    if (OS_NO_ERR == err)
    {
        key_msg_register(LINEIN1_TASK_NAME, KEY_PROJECT_AUX);
    }
}

static void linein2_task_exit(void)
{

    if (os_task_del_req(LINEIN1_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(LINEIN1_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        } while (os_task_del_req(LINEIN1_TASK_NAME) != OS_TASK_NOT_EXIST);
    }
}

TASK_REGISTER(linein1_task_info) =
{
    .name = LINEIN1_TASK_NAME,
    .init = linein2_task_init,
    .exit = linein2_task_exit,
};

#endif

