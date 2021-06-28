#include "sdk_cfg.h"
#include "setup.h"
#include "audio_drv.h"
#include "rtc.h"
#include "vm/vm_api.h"
#include "timer.h"
#if defined(_SUPPORT_EXT_MEMORY_)
#include "ext_memory/ext_memory.h"
#endif

#if defined( _SUPPORT_LCM_1618_DRIVE )
#include "Lcm_1618_drive.h"
#endif

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

setup_variable_t setup;
global_variable_t global;

typedef struct{
	uint32 title;
	setup_variable_t setup;
	uint32 check;
}wm_setup_t;

#define VM_SETUP_TITLE				0x83576205

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

static uint32 setup_vm_sum(wm_setup_t *vm_setup)
{
	uint32 back_sum = vm_setup->check;
	vm_setup->check = 0;

	uint32 sum = 0;
	int i;

	for(i = 0; i < sizeof(wm_setup_t); i++)
	{
		sum += ((uint8 *)vm_setup)[i];
	}

	vm_setup->check = back_sum;
	return (~sum)+1;
}

static uint8 setup_vm_read(setup_variable_t *addr)
{
	wm_setup_t *wm_setup = (wm_setup_t *)malloc(VM_SETUP_LEN);
	if(wm_setup == NULL)
	{
		setup_puts("setup_vm_read_error_malloc\n");
		return FALSE;
	}

	if(vm_read_api(VM_SETUP, wm_setup) != VM_SETUP_LEN)
	{
		free(wm_setup);
		setup_puts("setup_vm_read_error_read\n");
		return FALSE;
	}

	if(wm_setup->title != VM_SETUP_TITLE)
	{
		free(wm_setup);
		setup_puts("setup_vm_read_error_title\n");
		return FALSE;
	}

	if(setup_vm_sum(wm_setup) != wm_setup->check)
	{
		free(wm_setup);
		setup_puts("setup_vm_read_error_sum\n");
		return FALSE;
	}

	memcpy(addr, &wm_setup->setup, sizeof(setup_variable_t));
	free(wm_setup);

	return TRUE;
}

static void setup_wm_save(const setup_variable_t *addr)
{
	wm_setup_t *wm_setup = (wm_setup_t *)malloc(VM_FM_INFO_LEN);
	if(wm_setup == NULL)
	{
		setup_puts("setup_wm_save_error_malloc\n");
	}

	wm_setup->title = VM_SETUP_TITLE;
	wm_setup->setup = *addr;
	wm_setup->check = setup_vm_sum(wm_setup);

	vm_write_api(VM_SETUP, wm_setup);
	free(wm_setup);

	setup_puts("setup_wm_save\n");
}
#if defined(TD_ADD_W2DAB_UART)
extern u8 DAB_VDD;
#endif

#if defined(_ENABLE_MEMORY_PASSWORD_)
extern uint8 pwd_set_save[4];
extern uint8 power_password[4];
#endif

#if defined(_ENABLE_MEMORY_POWER_)
extern u8 power_set;
#endif

static void setup_data_init(void)
{
	memset(&setup, 0, sizeof(setup));

	setup.aud_vol		= AUDIO_VOLUME_DEFAULT;
	
#if defined(_SUPPORT_TWO_PT2313_)
	setup.aud_vol2		= AUDIO_VOLUME_DEFAULT;
#endif

#if defined(_ENABLE_DEFAULT_LOUD_ON_)	
	setup.aud_loud		= AUDIO_LOUDNESS_ON;
#else
    setup.aud_loud		= AUDIO_LOUDNESS_OFF;
#endif
	setup.aud_bass		= AUDIO_BASS_DEFAULT;
	setup.aud_treble	= AUDIO_TREBLE_DEFAULT;
#if defined(_FAD_BAL_FUNTION_)
    setup.aud_fl		= AUDIO_LR_DEFAULT;
    setup.aud_fr		= AUDIO_FR_DEFAULT;
    setup.aud_rl		= AUDIO_LR_DEFAULT;
    setup.aud_rr		= AUDIO_FR_DEFAULT;
#else
	setup.aud_bal		= AUDIO_LR_DEFAULT;
	setup.aud_fad		= AUDIO_FR_DEFAULT;
#endif
#if defined(_CUSTOM_SPECAIL_REQUEST_)
	setup.aud_eq		= AUDIO_EQ_ROCK;
#else
	setup.aud_eq		= AUDIO_EQ_OFF;
#endif
#if defined(_SPECIAL_TL2000_SET_)
	setup.aud_A_volume 	= AUDIO_A_VOLUME_DEFAULT;
	setup.aud_B_volume 	= AUDIO_B_VOLUME_DEFAULT;
#endif
#if defined(_CHANGE_DEF_VOL_)
	setup.DefVol = CHANGE_VOLUME_DEFAULT;
#endif
#if defined(_ENABLE_TALK_VOL_)
	#if defined(_CHANGE_MODE_1_)
	setup.talk_volume = BT_TALK_VOL;
	#else
	setup.talk_volume = AUDIO_VOLUME_DEFAULT;
	#endif
#endif
	setup.loc_flg = FALSE;

#if defined(_ENABLE_BEEP_)
	setup.aud_beep		= TRUE;
#endif
#if defined(_ENABLE_RADIO_RDS_)
	setup.rds.enable	= TRUE;
	setup.rds.type		= RDS_DEC_RDS;
	#if defined(RDS_AF_OFF_INIT)
	setup.rds.af		= FALSE;
	#else
	setup.rds.af		= TRUE;
	#endif
	#if defined(RDS_TA_ON_INIT)
	setup.rds.ta		= TRUE;
	#endif
	setup.rds.eon		= TRUE;
	setup.rds.long_short= TRUE;
	setup.rds.mask		= TRUE;
	setup.rds.time		= TRUE;
#if !defined(_ENABLE_RDS_VOLUME_)
#if defined(_DAOAN_RDS_DISPLAY_)
	setup.rds.volume	= AUDIO_VOLUME_DEFAULT;
#else
	setup.rds.volume	= AUDIO_VOLUME_DEFAULT + 4;
#endif
#endif
	setup.rds.cur_freq	= 8750;
#if defined(TD_ADD_NEW_APP)
	setup.pty_type = 0;
#endif
#endif

#if defined(_SUPPORT_AREA_)
	setup.area = SYS_DEFAULT_AREA;
#endif
#if defined(_SEL_ADD_TIME_SELECT_)
	setup.rtc_disp = SYS_DEFAULT_CLOCK;
#endif
#if defined(_SPECIAL_RADIO_INIT_)
	setup.radio	= TRUE;
#endif

#if defined(DISABLE_TURN_BT_MODE)
    setup.bt_flg = FALSE;
#endif

#if defined(_SUPPORT_SUBWOOFER_)
	setup.aud_subwoofer = 1;
#if defined(_CHANGE_SUB_CUT_OFF_160)
	setup.aud_sub_ctrl = AUDIO_SUBWOOFER_CUT_160;
#else
	setup.aud_sub_ctrl = AUDIO_SUBWOOFER_CUT_120;
#endif
#endif

#if defined(_SUPPORT_SUBWOOFER2_)
	setup.aud_sub = 1;
#if defined(_CHANGE_SUB_CUT_OFF_160)
#if defined(_CHANGE_SUB_CUT_OFF_160)
	setup.aud_sub_ctrl = AUDIO_SUBWOOFER_CUT_160;
#else
	setup.aud_sub_ctrl = AUDIO_SUBWOOFER_CUT_120;
#endif
#endif
#endif
#if defined(_SUPPORT_SUBWOOFER3_)
	setup.aud_sub1 = 1;
#endif

#if defined(_ENABLE_AP3768_SUBWOOFER_)
    setup.aud_sub = 1;
    setup.aud_sub1 = 1;
#endif

#if defined(_USB_DELAY_CHECK_)
	setup.usb_delay = 90;
#endif

    setup.usb_standby = FALSE;
    setup.sd_standby = FALSE;

#if defined(_SUPPORT_CALL_RECORDS_)
	global.info.sum = 0;
	global.info.call_num = 0;
	global.info.call_info_flag = TRUE;
#endif

#if defined(_ENABLE_ACC_OFF_LOGO_)
	setup.acc_on_flag = FALSE;
	global.first_power_on = TRUE;
#endif

#if defined(ACC_OFF_REMEBER_PLAY_STATE)
	setup.play_flag = 16;//REPEAT_ALL;
#endif

#if defined(_FY5517_POWER_OFF_TIME_)
	global.clock_flag = TRUE;
#endif

#if defined(_ENABLE_DAB_SEEK_)
	global.dab_seek_flag = TRUE;
#endif

#if defined(_POWER_OFF_BT_DEBUG_)
    global.bt_flag = FALSE;
#endif

#if defined(_FY7555_CLOCK_ICON_DISPLAY_)
	setup.time_flag = TRUE;
#endif

#if defined(AUDIO_SUB_VOL_DEFAULT)
	setup.aud_subwoofer = AUDIO_SUB_VOL_DEFAULT;
#endif
#if defined(TD_ADD_W2DAB_UART)
	DAB_VDD=1;
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
#if defined(_LIGHT_DEFAULT_WHITE_)||defined(_DEFAULT_COLOP_WHITE_)
	setup.State = PANEL_LIGHT_STATE_WHITE;
#elif defined(_LIGHT_DEFAULT_BLUE_)
	setup.State = PANEL_LIGHT_STATE_BLUE;
#elif defined(_DEFAULT_COLOP_RED_)
	setup.State = PANEL_LIGHT_STATE_RED;
#elif defined(_DEFAULT_COLOP_GREEN_)
	setup.State = PANEL_LIGHT_STATE_GREEN;
#else
	setup.State = PANEL_LIGHT_STATE_AUTO;
#if defined(TD_ADD_NEW_APP)
	setup.Auto_On_Off = 1;
	setup.save_clour = PANEL_LIGHT_STATE_RED;
#endif
#endif
	setup.Time = 0;

#if defined(_ENABLE_CHRAMATIC_CAMP_SHADE_)
#if defined(_WLS316_PANEL_LIGHT_)
	GET_LED_COLOR_LEVER_R = 0;
	GET_LED_COLOR_LEVER_G = 0;
	GET_LED_COLOR_LEVER_B = 0;
#else
	GET_LED_COLOR_LEVER_R = 0;
	GET_LED_COLOR_LEVER_G = LED_COLOR_G_LEVER_MAX;
	GET_LED_COLOR_LEVER_B = LED_COLOR_B_LEVER_MAX;
#endif
#elif defined(_SUPPORT_PANEL_PWM_)
	SET_LED_COLOR_TIME(17);
	setup.pwm_r = LED_COLOR_LEVER_MAX;
	setup.pwm_g = 0;
	setup.pwm_b = 0;
#endif
#endif

#if defined(_ENABLE_ALARM_CLOCK_)
	setup.alarm_beep	= FALSE;
	global.beep_str_flag = FALSE;
#endif

#if defined(_ENABLE_TEMP_PWM_)
	setup.temp_set = FUNC_SEL_TEMP_05;//µÚ5µµ
	setup.warm_set = FUNC_SEL_WARM_30;//30·ÖÖÓ
	setup.warm_time = 1859;
	setup.warm_power = TRUE;
#endif

#if defined(_KEY_LIGHT_CONTROL_)
	setup.light_flg = 0;
#endif

#if defined(SUPPORT_ALARM_FUNC_)
    setup.alarm_hour = 0;
    setup.alarm_minute = 0;
    setup.alarm_second = 0;
#endif

#if defined(_ENABLE_ID3_ON_OFF_)
	setup.id3_on_flag = TRUE;
#endif

#if defined(SUPPORT_LED_ADJUST)
	setup.LedState = BACK_LED_LOW;
#endif

#if defined(_ADJUST_A_B_SPK_)
	setup.A_SPK= 1;
	setup.B_SPK= 1;
#endif

#if defined(_MR889_SPECAIL_KEY_)
	setup.CONTROL_SPK= 1;
#endif

#if defined(_SUPPORT_CAN_UART_)
	setup.can_mode = 2;
	setup.can_speed = 2;
#endif

#if defined(BT_TALK_VOL)
#if !defined(_AUDIO_USE_DECODER_)
	setup.bt_vol = BT_TALK_VOL;
#endif
#endif

#if defined(_LIANKE_6803_POWER_)
	global.lp6803_time = 4;
#endif

#if defined(_ENABLE_BT_CONNECT_)
	global.bt_connect_time = 0;
	global.bt_count = 0;
#endif

#if defined(_ENABLE_RPT_RDM_FUNTION_)
	setup.rpt_start = FALSE;
	setup.rdm_start = FALSE;
#endif

#if defined(_SUPPORT_EXT_MEMORY_)
#if defined(_YH1701_BT_NAME_)
	setup.acc_off = FALSE;
#endif
#endif

#if defined(_ENABLE_VOLUME_CONTROL_)
	setup.zone1_start = 1;
    setup.zone2_start = 0;
    setup.zone3_start = 0;
    setup.zone4_start = 0;
#endif

#if defined(_SUPPORT_SUB_)
	setup.sub_start = 0;
#endif

#if defined(ADD_ADJUST_DEFAULT_VOL)
	setup.default_vol = AUDIO_VOLUME_DEFAULT;
#endif

#if defined(_DISABLE_BT_AUTO_CONNECT_)
	setup.acc_on_flag = FALSE;
#endif

#if defined(_ENABLE_OPTICAL_CONTROL_)
	setup.optical = 0;
#endif

#if defined(_ENABLE_MODE_CHANNEL_)
	setup.mode_start = 0;
#endif

#if defined( _SUPPORT_LCM_1618_DRIVE )
	Clear_1_2_page_disp_flag=0;
	Clear_page_disp_flag=1;
#endif

#if defined(_ENABLE_AUX_DETECT_)||defined(_SUPPORT_MIC_OK_)
	setup.save_name = "FMRadioTask";
#endif

#if defined(_ENABLE_USB_DELAY_)
	setup.name = "FMRadioTask";
#endif

#if defined(_ENABLE_POWER_LOGO_)
	setup.save_task_name = "FMRadioTask";
	setup.save_task_name1 = "FMRadioTask";
	setup.save_flag = FALSE;
#endif

#if defined(POWER_DISCONNECT_BT_)
	setup.power_off_flag = TRUE;
	setup.acc_off_flag = TRUE;
#endif

#if defined(_CHANGE_DISP_MODE_)
    setup.ad_keyflag = TRUE;
#endif

#if defined(_ENABLE_BT_ON_OFF_)
	setup.bt_on = TRUE;
    setup.power_off_flag = FALSE;
    setup.acc_off_flag = FALSE;
#endif

#if defined(_ENABLE_FIRST_POWER_MUTE_)&&0
	global.mute_flag = TRUE;
	global.mute_time = 90;
#endif
#if defined(_CHANGE_MODE_1_)
	global.dab_detect_flag = TRUE;
	global.dab_init_flag = TRUE;
#endif
#if defined(_XZC1668_RADIO_FUNTION_)
	setup.fm_flag = FALSE;
	setup.fm_band = 2;
	setup.am_band = 4;
#endif

#if defined(_SUPPORT_MIC_AUX_)
	//setup.save_name = "LineinTask";
#endif

#if defined(_ENABLE_MENU_FUNTION_)||defined(_ENABLE_DEFAULT_CONTROL_)
	setup.default_vol = AUDIO_VOLUME_DEFAULT;
#endif

#if defined(_JTI_POWER_LOGO_)
	setup.ch_index = 1;
	setup.band_index = 0;
#endif

#if defined(_ENABLE_BT_OFF_)
	setup.bt_scan_flag = 0;
#endif

#if defined(_DISABLE_RADIO_LOGO_)
#if defined(JTI_FREQ_LA_BRAZIL_)
	setup.radio_freq = 7610;
#else
	setup.radio_freq = 8750;
#endif
    setup.radio_band= 1;
    setup.radio_num = 1;
#endif

#if defined(_SUPPORT_POWER_ON_PASSWORD_)
	setup.first_power_flag = TRUE;
	setup.password_input = 0;
#endif

#if defined(_SUPPORT_PASSWORD_DISPLAY_)
	setup.first_power_flag = TRUE;
#endif

#if defined(_ENABLE_PASSWORD_SET_)
	setup.pwd_set = 0;
	global.pwd_flag = 0;
#endif

#if defined(_ENABLE_POWER_INPUT_)
	setup.first_input = 1;
#endif

#if defined(_ENABLE_RDS_ON_OFF_)
	setup.rds_onoff = 0;
#endif
#if defined(_ENABLE_MEMORY_PASSWORD_)
	vm_read_api(VM_PWD_SET, &pwd_set_save);
	printf("save[0]:0x%x\n",pwd_set_save[0]);
    printf("save[1]:0x%x\n",pwd_set_save[1]);
    printf("save[2]:0x%x\n",pwd_set_save[2]);
    printf("save[3]:0x%x\n",pwd_set_save[3]);
    if(!memcmp(power_password, pwd_set_save, 4))
    {
    	puts("_ENABLE_MEMORY_PASSWORD_\n");
		pwd_set_save[0] = 0x31;
		pwd_set_save[1] = 0x31;
		pwd_set_save[2] = 0x31;
		pwd_set_save[3] = 0x31;
		vm_write_api(VM_PWD_SET, &pwd_set_save);
	}
#endif

#if defined(_ENABLE_MEMORY_POWER_)
	//puts("------------setup_data_init_ok--------------\n");
	power_set = 0;
	vm_write_api(VM_POWER_SET, &power_set);
#endif
}



#if defined(DRIVE_LCD_LED)
extern void led_polling(void *param);
extern void led_hard_init(uint8 open_close);
#endif
u8 gEnterAccMode=FALSE;
void setup_init(void)
{
	setup_printf("setup_init:size:%d, max;%d\n", sizeof(setup_variable_t), VM_SETUP_LEN);
	if(sizeof(setup_variable_t) > VM_SETUP_LEN)
	{
		setup_puts("setup_variable_t full over!!!!!\n");
		while(TRUE);
	}
#ifdef	TD_ADD_ALEN
    gLast_bt_status=0;
	gDir_disp_flag=FALSE;
    gEnterAccMode=FALSE;
#endif
	memset(&global, 0, sizeof(global));

	if(vm_check_reset(VM_RESET_SETUP) || (!setup_vm_read(&setup)))
	{
	#if defined(_SUPPORT_EXT_MEMORY_)
    #if defined(_ENABLE_MEMORY_POWER_)
    	vm_read_api(VM_POWER_SET, &power_set);
    	//printf("--------power_set111--------:%d\n",power_set);
    #endif
		static uint8 check_flag;
		puts("-----------_DISABLE_MEMORY_BAL_--------------\n");
		if(ext_memory_msg(MSG_EXT_MEMORY_SETUP, &setup, &check_flag))
		{
			setup_printf("%s, check_flag:%d\n", __FUNCTION__, check_flag);
			if(!check_flag)
			{
				setup_data_init();
			}
		#if defined(_ENABLE_ST_MEMORY_)
			else
			{
				setup.first_st_flag = TRUE;
			}
		#endif
		#if defined(_DISABLE_MEMORY_BAL_)
    		else
    		{
    			puts("-----------_DISABLE_MEMORY_BAL111_--------------\n");
    			setup.aud_bal = 0;
			#if defined(_JTI_CHANGE_MODE_)
        		setup.change_power_flag = TRUE;
			#endif
    		}
		#endif
		}
		else
	#endif
		{
			setup_data_init();
		}
		setup_wm_save(&setup);
		vm_clean_reset(VM_RESET_SETUP);
	}

	if(vm_check_reset(VM_RESET_FM))
	{
		uint8 *wm_radio = (uint8 *)malloc(VM_FM_INFO_LEN);
		if(wm_radio != NULL)//here
		{
			memset(wm_radio, 0, VM_FM_INFO_LEN);
			vm_write_api(VM_FM_INFO, wm_radio);
			free(wm_radio);

		#if defined(_ENABLE_RADIO_RDS_)
			uint8 *wm_rds = (uint8 *)malloc(VM_FM_RDS_LEN);
			if(wm_rds != NULL)
			{
				memset(wm_rds, 0, VM_FM_RDS_LEN);
				vm_write_api(VM_FM_RDS1, wm_rds);
				vm_write_api(VM_FM_RDS2, wm_rds);
				vm_write_api(VM_FM_RDS3, wm_rds);
				vm_write_api(VM_FM_RDS4, wm_rds);
//				memset(wm_radio, 0, VM_FM_RDS_LEN);
//				vm_write_api(VM_FM_RDS1, wm_radio);
//				vm_write_api(VM_FM_RDS2, wm_radio);
//				vm_write_api(VM_FM_RDS3, wm_radio);
//				vm_write_api(VM_FM_RDS4, wm_radio);
				free(wm_rds);
			}
		#endif
		#if defined(TD_ADD_NEW_APP)&&defined(_AUDIO_DRIVER_AP3768_)
            {
                uint8 *vm_ap3768 =(uint8 *)malloc(VM_AP3768_LEN);
                if(vm_ap3768 !=NULL)
                {
                    printf("clr ap3768_info\r\n");
                    memset(vm_ap3768, 0, VM_AP3768_LEN);
                    vm_write_api(VM_AP3768_INFO, vm_ap3768);
                    free(vm_ap3768);
                }
            }
		#endif
			vm_clean_reset(VM_RESET_FM);
		}
	}

#if defined(DRIVE_LED_7pin_001)||defined(DRIVE_LED_COM4_SEG8)||defined(DRIVE_LED_COM5_SEG7)
	led_hard_init(TRUE);
	timer_reg_isr_fun(timer0_hl,1,led_polling,NULL);
#endif
}

void setup_exit(void)
{
	setup_puts("setup_date_save\n");
	setup_wm_save(&setup);
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

uint8 get_cur_vol(void)
{
#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_ENABLE_RDS_VOLUME_)
	if(global.rds_ta)
	{
		ew_printf("setup.rds.volume:%d\n",setup.rds.volume);
		return setup.rds.volume;
	}
	else
#endif
#endif
#if !defined(_AUDIO_USE_DECODER_)
	if(global.bt_call)
	{
	#if defined(_ENABLE_TALK_VOL_)
		return setup.talk_volume;
	#else
		return setup.bt_vol;
	#endif
	}
	else
#endif
	{
		return setup.aud_vol;
	}
}

void set_cur_vol(uint8 vol)
{
#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_ENABLE_RDS_VOLUME_)
	if(global.rds_ta)
	{
		setup.rds.volume = vol;
	}
	else
#endif
#endif
#if !defined(_AUDIO_USE_DECODER_)
	if(global.bt_call)
	{
    #if defined(_ENABLE_TALK_VOL_)
    	setup.talk_volume = vol;
    #else
    	setup.bt_vol = vol;
    #endif
	}
	else
#endif
	{
		setup.aud_vol = vol;
	}
}

#if defined(_SUPPORT_TWO_PT2313_)
uint8 get_cur_vol2(void)
{
	return setup.aud_vol2;
}

void set_cur_vol2(uint8 vol)
{
	setup.aud_vol2 = vol;
}
#endif

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/


