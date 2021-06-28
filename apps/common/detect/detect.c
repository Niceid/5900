#include "sdk_cfg.h"
#include "device_driver.h"
#include "detect.h"
#include "setup.h"
#include "rtc.h"
#include "key_drv_ad.h"

#include "dev_manage/drv_manage.h"

#if defined(_SUPPORT_ACC_)
static detect_var_t acc_var;
#endif
#if defined(_SUPPORT_AUX_DET)||defined(_SL6812_AUX_DETECT_)
static detect_var_t aux_var;
#endif
#if defined(_SUPPORT_DTH_)
static detect_var_t dth_var;
#endif
#if defined(_SUPPORT_USB_DET)
static detect_var_t usb_var;
#endif
#if defined(SUPPORT_LOW_PWR_DET)
static detect_var_t pwr_var;
#endif
#if defined(PLANE_LIGHT_FUNC)||defined(_SUPPORT_LIGHT_DETECT1_)||defined(ENABLE_PLANE_LIGHT_CONTROL_)
static detect_var_t ill_light_var;
#endif
#if defined(_SUPPORT_TAPE_)
static detect_var_t tape_var;
#endif

#if defined(_SUPPORT_MIC_OK_)
static detect_var_t mic_var;
#endif

#if defined(_SUPPORT_DISC_)
static detect_var_t player_disc_var;
static detect_var_t player_usb_var;
static detect_var_t player_card_var;
#endif

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

static uint8 detect_check(detect_var_t *var)
{
	//采样
	uint8 check = var->func();

    if(check != var->sample_state)
    {
        var->sample_state = check;
        var->sample_cnt = 0;
    }
    else
    {
    	check = var->sample_state;
        var->sample_cnt++;
    }

	//消抖
	if(check)
	{
		if(var->sample_cnt <= var->sample_on)
	    {
	        return DETECT_NULL;
	    }
	}
	else
	{
		if(var->sample_cnt <= var->sample_off)
	    {
	        return DETECT_NULL;
	    }
	}
    var->sample_cnt = 0;

    ///检测
    if(check)
    {
		if(DETECT_OFF == var->check_state)
		{
			var->check_state = DETECT_ON;
        	return DETECT_ON;
		}
	}
	else
	{
		if(DETECT_ON == var->check_state)
		{
			var->check_state = DETECT_OFF;
        	return DETECT_OFF;
		}
	}

    return DETECT_NULL;
}

static int dev_detect(detect_var_t *var)
{
    int dev_state = DEV_HOLD;
    uint8 res;

    res = detect_check(var);

    if(DETECT_OFF == res)
    {
        dev_state = DEV_OFFLINE;
    }
    else if(DETECT_ON == res)
    {
        dev_state = DEV_ONLINE;
    }

    if(var->dev_state == dev_state)
    {
        return DEV_HOLD;//设备状态保持不变
    }

    var->dev_state = dev_state;

    return dev_state;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_SUPPORT_ACC_)

static uint8 acc_check_func(void)
{
	return ACC_CHECK();
}

static void acc_check_api(uint32 info)
{
    int dev_state = dev_detect(&acc_var);

	if(DEV_OFFLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_ACC_OFF\n");
    #if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
	#if !defined(_IR_WITH_ACC_DELAY_)
    	AMP_CONTROL_OFF();
    #endif
	#endif
	
	#if defined(_ACC_OFF_SUB_)
		AMP_CONTROL_OFF();
	#endif
	
    #if defined(_ENABLE_MUTE_FUNTION_)
    	//FEIYIN_MUTE_ON();
    #endif
    #if defined(_FY7000_SUB_MUTE_)
    	if(setup.aud_sub)
    	{
    		SYS_SUB_OFF();
    	}
    	if(setup.aud_sub1)
    	{
    		SYS_SUB2_OFF();
    	}
    #endif
	#if !defined(_IR_WITH_ACC_DELAY_)
	#if !defined(_ACC_OFF_DELAY_TIME_)
    	os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_ACC_OFF, info);
    #endif
	#endif
	}
	else if(DEV_ONLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_ACC_ON\n");
	#if defined(_AUDIO_DRIVER_AP3768_)
    #if defined(_DISABLE_ACC_OFF_DISPLAY_)||defined(_ACC_OFF_DISP_LOGO_)||defined(_ACC_OFF_POWER_LOGO_)
    	global.bt_off = FALSE;
    #endif
	#else
	#if defined(_DISABLE_ACC_OFF_DISPLAY_CONNECT_)
	    global.bt_off = FALSE;
	#endif
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_ACC_ON, info);
	#endif
	}
}
#endif

#if defined(_SUPPORT_AUX_DET)||defined(_SL6812_AUX_DETECT_)
static uint8 aux_check_func(void)
{
	return AUX_DET_CHECK();
}
static void aux_check_api(uint32 info)
{
    int dev_state = dev_detect(&aux_var);

	if(DEV_OFFLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_AUX_OFF\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_AUX_OUT, info);
	}
	else if(DEV_ONLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_AUX_ON\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_AUX_IN, info);
	}
}
#endif

#if defined(SUPPORT_LOW_PWR_DET)
static uint8 pwr_check_func(void)
{
	return BAT_PWR_DET_CHECK();
}
static void pwr_check_api(uint32 info)
{
    int dev_state = dev_detect(&pwr_var);

	if(DEV_OFFLINE == dev_state)
	{
		if(global.sys_power)
		{
			os_taskq_post_event("MainTask", 1, MSG_SYS_POWER);
		}
	}
	else if(DEV_ONLINE == dev_state)
	{
		if(!global.sys_power)
		{
			os_taskq_post_event("MainTask", 1, MSG_SYS_POWER);
		}
	}
}
#endif

#if defined(_SUPPORT_USB_DET)
static uint8 usb_check_func(void)
{
	return USB_SWITCH_CHECK();
}
static void usb_check_api(uint32 info)
{
    int dev_state = dev_detect(&usb_var);

	if(DEV_OFFLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_USB_DET_OFF\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_USB_DET_OFF, info);
	}
	else if(DEV_ONLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_USB_DET_ON\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_USB_DET_ON, info);
	}
}
#endif

#if defined(PLANE_LIGHT_FUNC)
static uint8 ill_light_check_func(void)
{
	return ILL_LIGHT_DET_CHECK();
}
static void ill_light_check_api(uint32 info)
{
    int dev_state = dev_detect(&ill_light_var);

	if(DEV_OFFLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_ILL_LIGHT_OFF\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_ILL_LIGHT_OFF, info);
	}
	else if(DEV_ONLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_ILL_LIGHT_ON\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_ILL_LIGHT_ON, info);
	}
}
#endif

#if defined(ENABLE_PLANE_LIGHT_CONTROL_)
static uint8 ill_light_check_func(void)
{
	return ILL_LIGHT_DET_CHECK();
}
static void ill_light_check_api(uint32 info)
{
    int dev_state = dev_detect(&ill_light_var);

	if(DEV_OFFLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_ILL_LIGHT_OFF\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_ILL_LIGHT_OFF, info);
	}
	else if(DEV_ONLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_ILL_LIGHT_ON\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_ILL_LIGHT_ON, info);
	}
}
#endif

#if defined(_SUPPORT_LIGHT_DETECT1_)
static uint8 ill_light_check_func(void)
{
	return ILL_LIGHT_DET_CHECK();
}
#endif
#if defined(_SUPPORT_TAPE_)
static uint8 tape_check_func(void)
{
	return TAPE_DETCET_CHECK();
}
static void tape_check_api(uint32 info)
{
    int dev_state = dev_detect(&tape_var);

	if(DEV_OFFLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_TAPE_OFF\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_TAPE_OUT, info);
	}
	else if(DEV_ONLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_TAPE_ON\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_TAPE_IN, info);
	}
}
#endif

#if defined(_SUPPORT_MIC_OK_)
static uint8 mic_check_func(void)
{
	return MIC_CHECK();
}
static void mic_check_api(uint32 info)
{
    int dev_state = dev_detect(&mic_var);

	if(DEV_OFFLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_MIC_OFF\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_MIC_OUT, info);
	}
	else if(DEV_ONLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_MIC_IN\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_MIC_IN, info);
	}
}
#endif

#if defined(_SUPPORT_DTH_)
static uint8 dth_check_func(void)
{
	return DTH_CHECK();
}

static void dth_check_api(uint32 info)
{
    int dev_state = dev_detect(&dth_var);

	if(DEV_OFFLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_DTH_OFF\n");
    #if defined(_ENABLE_MUTE_FUNTION_)
    	FEIYIN_MUTE_ON();
    #endif
	#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
		AMP_CONTROL_OFF();
	#endif
    #if defined(_DSP_RCA_MUTE_)
    	DSP_RCA_MUTE_ON();
    #endif
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_DTH_OFF, info);
	}
	else if(DEV_ONLINE == dev_state)
	{
		detect_puts("detect--->SYS_EVENT_DTH_ON\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_DTH_ON, info);
	}
}
#endif

#if defined(_SUPPORT_DISC_)

#if defined(USB_DELAY_CHECK)
static void usb_delay_check_api(uint32 info)
{
    int dev_state = dev_detect(&player_usb_var);

	//ew_printf("usb_detect--->dev_state == %d\n",dev_state);
	if(player_usb_var.usbdelayoff)
	{
		//ew_printf("detect--->usbdelayoff == %d\n",player_usb_var.usbdelayoff);
		player_usb_var.usbdelayoff--;
		if (!player_usb_var.usbdelayoff)
		{
		#if defined(_SPECIAL_CHANGE_)
			setup.usb_out_delay = FALSE;
		#endif
			os_taskq_post_event("MainTask", 2, SYS_EVENT_PLAYER_USB_OUT, info);
		}
	}

	if(DEV_OFFLINE == dev_state)
	{
		ew_puts("detect--->SYS_EVENT_USB_OFF\n");
		player_usb_var.usbdelayoff = USB_DELAY_TIME/20;
	}
	else if(DEV_ONLINE == dev_state)
	{
		ew_puts("detect--->SYS_EVENT_USB_ON\n");
		player_usb_var.usbdelayoff = 0;
		os_taskq_post_event("MainTask", 2, SYS_EVENT_PLAYER_USB_IN, info);
	#if defined(_SPECIAL_CHANGE_)
		setup.usb_out_delay = TRUE;
	#endif
	}
}
#endif

#if defined(_DISC_OUT_DELAY_CHECK_)
static void disc_delay_check_api(uint32 info)
{
    int dev_state = dev_detect(&player_disc_var);

	//ew_printf("usb_detect--->dev_state == %d\n",dev_state);
	if(player_disc_var.discdelayoff)
	{
		//ew_printf("detect--->usbdelayoff == %d\n",player_usb_var.usbdelayoff);
		player_disc_var.discdelayoff--;
		if (!player_disc_var.discdelayoff)
		{
			os_taskq_post_event("MainTask", 2, SYS_EVENT_PLAYER_DISC_OUT, info);
		}
	}

	if(DEV_OFFLINE == dev_state)
	{
		ew_puts("detect--->SYS_EVENT_DISC_OUT\n");
		player_disc_var.discdelayoff = 1000/20;
	}
	else if(DEV_ONLINE == dev_state)
	{
		ew_puts("detect--->SYS_EVENT_DISC_IN\n");
		player_disc_var.discdelayoff = 0;
		global.disc_detect = 1;
		os_taskq_post_event("MainTask", 2, SYS_EVENT_PLAYER_DISC_IN, info);
	}
}
#endif

static uint8 player_disc_check_func(void)
{
	return DETECT_DISC_CHECK();
}
static uint8 player_usb_check_func(void)
{
	return DETECT_USB_CHECK();
}
static uint8 player_card_check_func(void)
{
	return DETECT_SD_CHECK();
}

static void disc_check_api(uint32 info)
{
#if defined(_SUPPORT_ACC_)
	if(acc_var.check_state == DETECT_OFF)
	{
		//ew_puts("detect--->acc_var.check_state_OFF\n");
		return;
	}
#endif

#if defined(OPEN_KEY_INSIDE_)
	if(dth_var.check_state == DETECT_OFF)
	{
		ew_puts("detect--->acc_var.check_state_OFF\n");
		return;
	}
#endif

#if defined(_ACC_ON_DELAY_DETECT_)
	{
		static u16 cnt = 0;
		if(cnt>150)//250
		{
			//ew_puts("detect--->delay\n");
	 	}
		else
		{
			cnt++;
			return;
		}
	}
#endif
#if !defined(_DISC_OUT_DELAY_CHECK_)
	int player_disc_state = dev_detect(&player_disc_var);
#endif
#if !defined(USB_DELAY_CHECK)
	int player_usb_state = dev_detect(&player_usb_var);
#endif
	int player_card_state = dev_detect(&player_card_var);

#if defined(_DISC_OUT_DELAY_CHECK_)
	disc_delay_check_api(info);
#else
	if(DEV_OFFLINE == player_disc_state)
	{
		ew_puts("detect--->SYS_EVENT_PLAYER_DISC_OUT\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_PLAYER_DISC_OUT, info);
	}
	else if(DEV_ONLINE == player_disc_state)
	{
		ew_puts("detect--->SYS_EVENT_PLAYER_DISC_IN\n");
		global.disc_detect = 1;
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_PLAYER_DISC_IN, info);
	}
#endif

#if defined(USB_DELAY_CHECK)
	usb_delay_check_api(info);
#else
	if(DEV_OFFLINE == player_usb_state)
	{
		ew_puts("detect--->SYS_EVENT_PLAYER_USB_OUT\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_PLAYER_USB_OUT, info);
	#if defined(_USB_DELAY_CHECK_)
		setup.usb_delay = 0;
	#endif
	}
	else if(DEV_ONLINE == player_usb_state)
	{
		ew_puts("detect--->SYS_EVENT_PLAYER_USB_IN\n");
	#if defined(_USB_DELAY_CHECK_)
		if(setup.usb_delay)
		{
			setup.usb_delay--;
		}
		else
		{
			os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_PLAYER_USB_IN, info);
		}
	#else
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_PLAYER_USB_IN, info);
	#endif
	}
#endif

	if(DEV_OFFLINE == player_card_state)
	{
		ew_puts("detect--->SYS_EVENT_PLAYER_CARD_OUT\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_PLAYER_CARD_OUT, info);
	}
	else if(DEV_ONLINE == player_card_state)
	{
		ew_puts("detect--->SYS_EVENT_PLAYER_CARD_IN\n");
		os_taskq_post_event(MAIN_TASK_NAME, 2, SYS_EVENT_PLAYER_CARD_IN, info);
	}
}
#endif


/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

void detect_init_api(void)
{
#if defined(_SUPPORT_ACC_)
	acc_var.sample_state = 1;
    acc_var.sample_on = 0;
#if defined(_ENABLE_RTC_CLK_)
#if defined(_EVB_ACC_DELAY_TIME_)
	acc_var.sample_off = 14;// 10
#else
	acc_var.sample_off = 70;// 10
#endif
#else
	acc_var.sample_off = 10;// 10
#endif
	acc_var.sample_cnt = 0;
	acc_var.check_state = DETECT_ON;
    acc_var.dev_state = DEV_ONLINE;
	acc_var.func = acc_check_func;
#endif
#if defined(_SUPPORT_AUX_DET)||defined(_SL6812_AUX_DETECT_)
	aux_var.sample_state = 1;
    aux_var.sample_on = 0;
	aux_var.sample_off = 10;
	aux_var.sample_cnt = 0;
	aux_var.check_state = DETECT_ON;
    aux_var.dev_state = DEV_ONLINE;
	aux_var.func = aux_check_func;
#endif

#if defined(SUPPORT_LOW_PWR_DET)
	pwr_var.sample_state = 1;
    pwr_var.sample_on = 0;
	pwr_var.sample_off = 10;
	pwr_var.sample_cnt = 0;
	pwr_var.check_state = DETECT_ON;
    pwr_var.dev_state = DEV_ONLINE;
	pwr_var.func = pwr_check_func;
#endif

#if defined(_SUPPORT_USB_DET)
	usb_var.sample_state = 1;
    usb_var.sample_on = 0;
	usb_var.sample_off = 10;
	usb_var.sample_cnt = 0;
	usb_var.check_state = DETECT_ON;
    usb_var.dev_state = DEV_ONLINE;
	usb_var.func = usb_check_func;
#endif

#if defined(PLANE_LIGHT_FUNC)||defined(_SUPPORT_LIGHT_DETECT1_)||defined(ENABLE_PLANE_LIGHT_CONTROL_)
	ill_light_var.sample_state = 1;
    ill_light_var.sample_on = 0;
	ill_light_var.sample_off = 10;
	ill_light_var.sample_cnt = 0;
	ill_light_var.check_state = DETECT_ON;
    ill_light_var.dev_state = DEV_ONLINE;
	ill_light_var.func = ill_light_check_func;
#endif

#if defined(_SUPPORT_DTH_)
	dth_var.sample_state = 1;
    dth_var.sample_on = 0;
	dth_var.sample_off = 10;
	dth_var.sample_cnt = 0;
	dth_var.check_state = DETECT_ON;
    dth_var.dev_state = DEV_ONLINE;
	dth_var.func = dth_check_func;

#if defined(DTH_USE_KEY1_DETECT)
	setup.dth_detect = 0;
#endif
#endif

#if defined(_SUPPORT_TAPE_)
    tape_var.sample_state = 1;
    tape_var.sample_on = 0;
    tape_var.sample_off = 10;
    tape_var.sample_cnt = 0;
    tape_var.check_state = DETECT_ON;
    tape_var.dev_state = DEV_ONLINE;
    tape_var.func = tape_check_func;
#endif

#if defined(_SUPPORT_MIC_OK_)
    mic_var.sample_state = 1;
    mic_var.sample_on = 0;
    mic_var.sample_off = 10;
    mic_var.sample_cnt = 0;
    mic_var.check_state = DETECT_ON;
    mic_var.dev_state = DEV_ONLINE;
    mic_var.func = mic_check_func;
#endif

#if defined(_SUPPORT_DISC_)
	player_disc_var.sample_state = 1;
    player_disc_var.sample_on = 0;
	player_disc_var.sample_off = 10;
	player_disc_var.sample_cnt = 0;
	player_disc_var.check_state = DETECT_ON;
    player_disc_var.dev_state = DEV_ONLINE;
	player_disc_var.func = player_disc_check_func;

	player_usb_var.sample_state = 1;
    player_usb_var.sample_on = 0;
	player_usb_var.sample_off = 10;
	player_usb_var.sample_cnt = 0;
	player_usb_var.check_state = DETECT_ON;
    player_usb_var.dev_state = DEV_ONLINE;
	player_usb_var.func = player_usb_check_func;

	player_card_var.sample_state = 1;
    player_card_var.sample_on = 0;
	player_card_var.sample_off = 10;
	player_card_var.sample_cnt = 0;
	player_card_var.check_state = DETECT_ON;
    player_card_var.dev_state = DEV_ONLINE;
	player_card_var.func = player_card_check_func;
#endif

#if defined(_ENABLE_AUX_DETECT_)
	AUX_INIT();
#endif
}

void detect_check_api(uint32 info)
{
#if defined(_SUPPORT_ACC_)
	acc_check_api(info);
#endif
#if defined(_SUPPORT_AUX_DET)||defined(_SL6812_AUX_DETECT_)
	aux_check_api(info);
#endif
#if defined(_SUPPORT_DTH_)
    dth_check_api(info);
#endif
#if defined(SUPPORT_LOW_PWR_DET)
    pwr_check_api(info);
#endif
#if defined(_SUPPORT_USB_DET)
    usb_check_api(info);
#endif
#if defined(PLANE_LIGHT_FUNC)||defined(ENABLE_PLANE_LIGHT_CONTROL_)
    ill_light_check_api(info);
#endif
#if defined(_SUPPORT_TAPE_)
	tape_check_api(info);
#endif
#if defined(_SUPPORT_MIC_OK_)
	mic_check_api(info);
#endif
#if defined(_SUPPORT_DISC_)
	disc_check_api(info);
#endif
}

