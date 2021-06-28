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

#include "reverse\reverse.h"


#define REVE_DEBUG
#ifdef REVE_DEBUG
    #define reve_puts     ew_puts
    #define reve_printf   ew_printf
#else
    #define reve_puts(...)
    #define reve_printf(...)
#endif


#if defined(_SUPPORT_REVERSE_MODEL_)

static void reverse_channel_off(void)
{
	sys_mute_ctrl(ENABLE, 0);
	//audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_REV_OFF, TRUE);
}

static void reverse_info_init(void)
{
    sys_mute_ctrl(ENABLE, 1000);
	//audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_REV_ON, TRUE);
	//audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
}

static void reverse_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{

	}
}

static void reverse_timer(void *param)
{
	os_taskq_post_msg(REVERSE_TASK_NAME, 1, MSG_REVE_TIMER);
}

static void reverse_task(void *p)
{
    int msg[6];

    reve_puts("\n************************REVERSE TASK********************\n");

#if EQ_EN
	eq_enable();
	eq_mode = get_eq_default_mode();
#endif // EQ_EN
    malloc_stats();

	disp_send_msg(DISP_EVENT_REVE);
	os_taskq_post_msg(REVERSE_TASK_NAME, 1, MSG_REVE_INIT);

	timer_reg_func(reverse_timer, NULL, 1000);

    while(1)
    {
        memset(msg, 0x00, sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
		reverse_reset_common(msg[0], msg[1]);

		switch(msg[0])
        {
			case MSG_REVE_TIMER://Çå¹·ÏûÏ¢
				//printf("rec_task time\n");
				//AUDIO_MUTE_OFF();
				break;

            case SYS_EVENT_DEL_TASK:
			    if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
			    {
			    #if EQ_EN
			    	eq_disable();
			    #endif/*EQ_EN*/
			    	timer_unreg_func(reverse_timer);
			    	reverse_channel_off();
			    	os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case MSG_REVE_INIT:
	            reverse_info_init();
	            break;

            default:
	            task_msg_default(msg[0], msg[1]);
	            break;
        }
    }
}

static void reverse_task_init(void *priv)
{
	reve_puts("can_task_init\n");

    u32 err;
    err = os_task_create_ext(reverse_task,
						(void*) 0,
						TaskReversePrio,
						10
					#if OS_TIME_SLICE_EN > 0
			            ,1
					#endif
            			,REVERSE_TASK_NAME
            			,LINEIN_TASK_STACK_SIZE);

    if (OS_NO_ERR == err)
    {
        key_msg_register(REVERSE_TASK_NAME, KEY_PROJECT_REVE);
    }
}

static void reverse_task_exit(void)
{
	reve_puts("can_task_exit\n");
	key_msg_register(NULL, KEY_PROJECT_MAX);
    if (os_task_del_req(REVERSE_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(REVERSE_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        } while (os_task_del_req(REVERSE_TASK_NAME) != OS_TASK_NOT_EXIST);
    }
}

TASK_REGISTER(reverse_task_info) =
{
    .name = REVERSE_TASK_NAME,
    .init = reverse_task_init,
    .exit = reverse_task_exit,
};

#endif

