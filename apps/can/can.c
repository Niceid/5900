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

#include "can\can.h"


#define CAN_DEBUG
#ifdef CAN_DEBUG
    #define can_puts     ew_puts
    #define can_printf   ew_printf
#else
    #define can_puts(...)
    #define can_printf(...)
#endif


#if defined(_SUPPORT_CAN_MODEL_)

static void can_dac_channel_off(void)
{
	sys_mute_ctrl(ENABLE, 0);
	//audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MIC_OFF, TRUE);
}

static void can_info_init(void)
{
    sys_mute_ctrl(ENABLE, 0);
	//audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MIC_ON, TRUE);
	//audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
}

static void can_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{

	}
}

static void can_timer(void *param)
{
	os_taskq_post_msg(CAN_TASK_NAME, 1, MSG_CAN_TIMER);
}

static void can_task(void *p)
{
    int msg[6];

    can_puts("\n************************CAN TASK********************\n");
	
#if EQ_EN
	eq_enable();
	eq_mode = get_eq_default_mode();
#endif // EQ_EN
    malloc_stats();

#if defined(_SUPPORT_CAN_UART_)
	global.can.disp = 0;
#endif

	disp_send_msg(DISP_EVENT_CAN);
	setup.password = TRUE;
	os_taskq_post_msg(CAN_TASK_NAME, 1, MSG_CAN_INIT);
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(can_timer, NULL, 2000);
#else
	timer_reg_func(can_timer, NULL, 1000);
#endif
	
    while(1)
    {
        memset(msg, 0x00, sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
		can_reset_common(msg[0], msg[1]);

		switch(msg[0])
        {
			case MSG_CAN_TIMER://Çå¹·ÏûÏ¢
				//printf("rec_task time\n");
				break;
				
            case SYS_EVENT_DEL_TASK:
				can_puts("MIC--->SYS_EVENT_DEL_TASK\n");
				setup.password = FALSE;
				setup.password_input = 0;
				setup.password_number = 0;
			    if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
			    {
			    #if EQ_EN
			    	eq_disable();
			    #endif/*EQ_EN*/
			    	timer_unreg_func(can_timer);
			    	can_dac_channel_off();
			    	can_puts("MIC_SYS_EVENT_DEL_TASK\n");
			    	os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case MSG_CAN_INIT:
	            can_puts("MSG_CAN_INIT\n");
	            can_info_init();
	            break;

            default:
	            task_msg_default(msg[0], msg[1]);
	            break;
        }
    }
}

static void can_task_init(void *priv)
{
	can_puts("can_task_init\n");

    u32 err;
    err = os_task_create_ext(can_task,
						(void*) 0,
						TaskCanPrio,
						10
					#if OS_TIME_SLICE_EN > 0
			            ,1
					#endif
            			,CAN_TASK_NAME
            			,LINEIN_TASK_STACK_SIZE);

    if (OS_NO_ERR == err)
    {
        key_msg_register(CAN_TASK_NAME, KEY_PROJECT_CAN);
    }
}

static void can_task_exit(void)
{
	can_puts("can_task_exit\n");
	key_msg_register(NULL, KEY_PROJECT_MAX);
    if (os_task_del_req(CAN_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(CAN_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        } while (os_task_del_req(CAN_TASK_NAME) != OS_TASK_NOT_EXIST);
    }
}

TASK_REGISTER(can_task_info) =
{
    .name = CAN_TASK_NAME,
    .init = can_task_init,
    .exit = can_task_exit,
};

#endif

