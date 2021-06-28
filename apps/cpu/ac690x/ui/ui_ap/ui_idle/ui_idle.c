#include "sdk_cfg.h"
#include "ui_idle/ui_idle.h"
#include "file_operate/file_op.h"
#include "dev_manage/dev_ctl.h"
#include "bluetooth/avctp_user.h"
#include "key_drv_ad.h"

static idle_disp_t *idle_disp = NULL;

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
static void ui_show_idle_clean(void)
{
	show_clear_icon(DISP_LEVEL_NORMAL);
	show_clear_num(DISP_LEVEL_NORMAL);
}
static void ui_idle_icon_enable(void)
{
#if !defined(_ENABLE_POWER_ICON_)
	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER);
#endif

#if defined(_ENABLE_SUB_DISPLAY_)
	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
#endif

#if defined(_SWL8809_DISPLAY_)||defined(_JL8224B_ICON_DISPLAY_)||defined(_XM003_SPECIAL_DISPLAY_)||defined(_ENABLE_L1_DISP_)
	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER2);
#endif
#if defined(_SUPPORT_DISC_)
	if(DETECT_DISC_CHECK())
	{
		ui_show_disc_icon(ON);
	}
	if(DETECT_USB_CHECK())
	{
		ui_show_usb_icon(ON);
	}
	if(DETECT_SD_CHECK())
	{
		ui_show_sd_icon(ON);
	}
#endif

#if !defined(USB_SD_MODE_WORK_ONOFF)
	if(file_operate_get_total_phydev())
	{
		if(dev_get_online_status(usb))
		{
			ew_puts("ui_show_usb_icon_on\n");
			ui_show_usb_icon(ON);
		}
	#if defined(_SUPPORT_SD_)
		if(dev_get_online_status(sd1))
		{
			ew_puts("ui_show_sd_icon_on\n");
	    	ui_show_sd_icon(ON);
		}
	#endif
	}
#endif

	if((get_bt_connect_status() == BT_STATUS_CONNECTING)
	 ||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
	 ||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
	}
#if defined(BT_ICON_BLINK)
	else
	{
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
	}
#endif
	if(setup.aud_loud == AUDIO_LOUDNESS_ON)
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_LOUD);
	}
	
	show_eq_icon(setup.aud_eq);
	if(setup.aud_eq == AUDIO_EQ_OFF)
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EQ);
	}
}

static void ui_idle_icon_disable(void)
{
#if !defined(SPECIAL_JD_DISP)
#if !defined(SPECIAL_icon_DISP)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POWER);
#endif
#endif

#if defined(_SWL8809_DISPLAY_)||defined(_JL8224B_ICON_DISPLAY_)||defined(_XM003_SPECIAL_DISPLAY_)||defined(_ENABLE_L1_DISP_)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POWER2);
#endif

#if defined(_SUPPORT_DISC_)
	ui_show_disc_icon(OFF);
#endif
	ui_show_usb_icon(OFF);
	ui_show_sd_icon(OFF);
	ew_puts("ui_idle_icon_disable\n");
	
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
#if defined(BT_ICON_BLINK)
	ui_drv_func.flash_disable(UI_ICON_BLUETOOTH);
#endif
    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_LOUD);
	
	show_eq_icon(AUDIO_EQ_MAX);
    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EQ);
#if defined(_SWL_ENABLE_DISK_DISC_TO_FLASH_)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
#endif
#if defined(_FY5517_POWER_OFF_TIME_)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_TIME);
#endif
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
}

void ui_show_idle_init(void)
{
	ui_puts("ui_show_idle_init\n");
	
	ui_show_idle_clean();
	
	audio_drv_exit();
}

void ui_show_idle_exit(void)
{
	ui_puts("ui_show_idle_exit\n");
	
	ui_disp.func_sel = FUNC_SEL_NULL;
	
	ui_show_idle_clean();
	
#if !defined(_AUDIO_DRIVER_AP3768_)
	audio_drv_init();
#endif
}

#if defined(_FY7555_LOGO_DISPLAY_)
void ui_show_idle_polling(void)
{
	switch(idle_disp->disp_ctrl.status)
	{
		case 1:
			{
		    	const uint8 disp_length = 13;
				uint8 dips_buffer[disp_length + 1];
				uint8 str_length;
				uint8 index;
				const char *disp_char = _FY7555_LOGO_;
				
				str_length = strlen(disp_char);
				for(index = 0; index < disp_length; index++)
				{
					if(index < str_length)
					{
						dips_buffer[index] = disp_char[index];
					}
					else
					{
						dips_buffer[index] = ' ';
					}
				}
				dips_buffer[index] = '\0';
							
				ui_show_string(DISP_LEVEL_NORMAL,  dips_buffer);
				ui_show_string(DISP_LEVEL_SETTING, dips_buffer);
				
				//idle_disp_printf("str_length:%d, disp_length:%d\n", str_length, disp_length);
				
				if(str_length > disp_length)
				{
					idle_disp->disp_ctrl.timer = 0;
					idle_disp->disp_ctrl.point = 0;
					idle_disp->disp_ctrl.step = str_length - disp_length + 0;
					idle_disp->disp_ctrl.status ++;
				}
				else
				{
					idle_disp->disp_ctrl.status = 0;
				}
			}
			break;
			
		case 2:
			{
				//idle_disp_printf("ui_show_idle_polling---2222----timer:%d\n", idle_disp->disp_ctrl.timer);
				if((++idle_disp->disp_ctrl.timer) >= (100/100))
				{
					idle_disp->disp_ctrl.timer = 0;
					idle_disp->disp_ctrl.status++;
				}
			}
			break;
			
		case 3:
			if((++idle_disp->disp_ctrl.timer) >= (200/100))
			{
				//idle_disp_puts("ui_show_idle_polling---3333----\n");
				
				idle_disp->disp_ctrl.timer = 0;
				if((++idle_disp->disp_ctrl.point) <= idle_disp->disp_ctrl.step)
				{
					const uint8 disp_length = 13;
					uint8 dips_buffer[disp_length + 1];
					uint8 str_length;
					uint8 index;
					const char *disp_char = _FY7555_LOGO_;
					
					str_length = strlen(disp_char) - idle_disp->disp_ctrl.point;
					for(index = 0; index < disp_length; index++)
					{
						if(index < str_length)
						{
							dips_buffer[index] = disp_char[idle_disp->disp_ctrl.point + index];
						}
						else
						{
							dips_buffer[index] = ' ';
						}
					}
					dips_buffer[index] = '\0';
					
					ui_show_string(DISP_LEVEL_NORMAL,  dips_buffer);
					ui_show_string(DISP_LEVEL_SETTING, dips_buffer);
				}
				else
				{
					idle_disp->disp_ctrl.status++;
				}
			}
			break;
			
		case 4:
			//idle_disp_puts("ui_show_idle_polling---4444----\n");
			idle_disp->disp_ctrl.status = 0;
			break;
	}

#if defined(_FY7555_VBAR_DISPLAY_)&&0
	uint8 i;
	if(++global.vbar_time <= (200/100))
	{
		for(i = 0; i < 91; i++)
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_L1P+i);
		}
	}
	else if((++global.vbar_time <= (400/100))&&(++global.vbar_time > (200/100)))
	{
		for(i = 0; i < 91; i++)
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_L1P+i);
		}
	}
	else
	{
		global.vbar_time = 0;
	}
#endif
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_SPECIAL_LOGO_DISPLAY_)
void ui_show_idle_polling(void)
{
	switch(idle_disp->disp_ctrl.status)
	{
		case 1:
			{
		    #if defined(_FY7000_SPECIAL_DISPLAY_)
		    	const uint8 disp_length = 13;
			#elif defined(_FIVE_CHAR_DISPLAY_)
			    const uint8 disp_length = 5;
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				const uint8 disp_length = 10;
		    #else
		    	const uint8 disp_length = 8;
		    #endif
				uint8 dips_buffer[disp_length + 1];
				uint8 str_length;
				uint8 index;
				const char *disp_char = _SPECIAL_LOGO_DISPLAY_;
				
				str_length = strlen(disp_char);
				for(index = 0; index < disp_length; index++)
				{
					if(index < str_length)
					{
						dips_buffer[index] = disp_char[index];
					}
					else
					{
						dips_buffer[index] = ' ';
					}
				}
				dips_buffer[index] = '\0';
							
				ui_show_string(DISP_LEVEL_NORMAL,  dips_buffer);
				ui_show_string(DISP_LEVEL_SETTING, dips_buffer);
				
				//idle_disp_printf("str_length:%d, disp_length:%d\n", str_length, disp_length);
				
				if(str_length > disp_length)
				{
					idle_disp->disp_ctrl.timer = 0;
					idle_disp->disp_ctrl.point = 0;
					idle_disp->disp_ctrl.step = str_length - disp_length + 0;
					idle_disp->disp_ctrl.status ++;
				}
				else
				{
					idle_disp->disp_ctrl.status = 0;
				}
			}
			break;
			
		case 2:
			{
				//idle_disp_printf("ui_show_idle_polling---2222----timer:%d\n", idle_disp->disp_ctrl.timer);
				if((++idle_disp->disp_ctrl.timer) >= (100/100))
				{
					idle_disp->disp_ctrl.timer = 0;
					idle_disp->disp_ctrl.status++;
				}
			}
			break;
			
		case 3:
		#if defined(_REDUCE_LOGO_DISP_TIME)
			if((++idle_disp->disp_ctrl.timer) >= (200/100))
		#else
			if((++idle_disp->disp_ctrl.timer) >= (300/100))
		#endif
			{
				//idle_disp_puts("ui_show_idle_polling---3333----\n");
				
				idle_disp->disp_ctrl.timer = 0;
				if((++idle_disp->disp_ctrl.point) <= idle_disp->disp_ctrl.step)
				{
				#if defined(_FY7000_SPECIAL_DISPLAY_)
					const uint8 disp_length = 13;
				#elif defined(_FIVE_CHAR_DISPLAY_)
				    const uint8 disp_length = 5;
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					const uint8 disp_length = 10;
				#else
					const uint8 disp_length = 8;
				#endif
					uint8 dips_buffer[disp_length + 1];
					uint8 str_length;
					uint8 index;
					const char *disp_char = _SPECIAL_LOGO_DISPLAY_;
					
					str_length = strlen(disp_char) - idle_disp->disp_ctrl.point;
					for(index = 0; index < disp_length; index++)
					{
						if(index < str_length)
						{
							dips_buffer[index] = disp_char[idle_disp->disp_ctrl.point + index];
						}
						else
						{
							dips_buffer[index] = ' ';
						}
					}
					dips_buffer[index] = '\0';
					
				#if defined(_SL6812A_SPECIAL_DISPLAY_)
					ui_disp.timer = 100;
				#endif
					ui_show_string(DISP_LEVEL_NORMAL,  dips_buffer);
					ui_show_string(DISP_LEVEL_SETTING, dips_buffer);
				}
				else
				{
					idle_disp->disp_ctrl.status++;
				}
			}
			break;
			
		case 4:
			//idle_disp_puts("ui_show_idle_polling---4444----\n");
			idle_disp->disp_ctrl.status = 0;
			break;
	}
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_SPECIAL_POWER_OFF_DISPLAY_)
void ui_show_idle_polling1(void)
{
	switch(idle_disp->disp_ctrl1.status)
	{
		case 1:
			{
		    	const uint8 disp_length = 8;
				uint8 dips_buffer[disp_length + 1];
				uint8 str_length;
				uint8 index;
				const char *disp_char = _SPECIAL_POWER_OFF_DISPLAY_;
				
				str_length = strlen(disp_char);
				for(index = 0; index < disp_length; index++)
				{
					if(index < str_length)
					{
						dips_buffer[index] = disp_char[index];
					}
					else
					{
						dips_buffer[index] = ' ';
					}
				}
				dips_buffer[index] = '\0';
				
				ui_show_string(DISP_LEVEL_NORMAL,  dips_buffer);
				ui_show_string(DISP_LEVEL_SETTING, dips_buffer);
				
				//idle_disp_printf("str_length:%d, disp_length:%d\n", str_length, disp_length);
				
				if(str_length > disp_length)
				{
					idle_disp->disp_ctrl1.timer = 0;
					idle_disp->disp_ctrl1.point = 0;
					idle_disp->disp_ctrl1.step = str_length - disp_length + 0;
					idle_disp->disp_ctrl1.status ++;
				}
				else
				{
					idle_disp->disp_ctrl1.status = 0;
				}
			}
			break;
			
		case 2:
			{
				if((++idle_disp->disp_ctrl1.timer) >= (100/100))// 800
				{
					idle_disp->disp_ctrl1.timer = 0;
					idle_disp->disp_ctrl1.status++;
				}
			}
			break;
			
		case 3:
		#if defined(_EVB_LOGO_DISP_TIME_)
			if((++idle_disp->disp_ctrl1.timer) >= (200/100))
		#else
			if((++idle_disp->disp_ctrl1.timer) >= (300/100))
		#endif
			{
				idle_disp->disp_ctrl1.timer = 0;
				if((++idle_disp->disp_ctrl1.point) <= idle_disp->disp_ctrl1.step)
				{
					const uint8 disp_length = 8;
					uint8 dips_buffer[disp_length + 1];
					uint8 str_length;
					uint8 index;
					const char *disp_char = _SPECIAL_POWER_OFF_DISPLAY_;
					
					str_length = strlen(disp_char) - idle_disp->disp_ctrl1.point;
					for(index = 0; index < disp_length; index++)
					{
						if(index < str_length)
						{
							dips_buffer[index] = disp_char[idle_disp->disp_ctrl1.point + index];
						}
						else
						{
							dips_buffer[index] = ' ';
						}
					}
					dips_buffer[index] = '\0';
					
					ui_show_string(DISP_LEVEL_NORMAL,  dips_buffer);
					ui_show_string(DISP_LEVEL_SETTING, dips_buffer);
				}
				else
				{
					idle_disp->disp_ctrl1.status++;
				}
			}
			break;
			
		case 4:
			//idle_disp_puts("ui_show_power_off_polling---4444----\n");
			idle_disp->disp_ctrl1.status = 0;
			break;
	}
}
#endif

#if defined(_ENABLE_ACC_OFF_LOGO_)
extern uint8 detect_usb_state(void);
extern uint8 detect_sd_state(void);
#endif

#if defined(_HKY1032_SPECIAL_LOGO_)
void ui_show_goodbye(void)
{
	uint8 dispchar[8];

	memset(dispchar, ' ', sizeof(dispchar));

	dispchar[0] = 'G';
	dispchar[1] = 'O';
	dispchar[2] = 'O';
	dispchar[3] = 'D';
	dispchar[4] = 'B';
	dispchar[5] = 'Y';
	dispchar[6] = 'E';
	dispchar[7] = ' ';
	show_clear_icon(DISP_LEVEL_SETTING);
	ui_show_string(DISP_LEVEL_SETTING, dispchar);
	ui_disp.now_level = DISP_LEVEL_SETTING;
	ui_disp.timer = 1000;
}

void ui_show_welcome(void)
{
	uint8 dispchar[8];

	memset(dispchar, ' ', sizeof(dispchar));

	dispchar[0] = 'W';
	dispchar[1] = 'E';
	dispchar[2] = 'L';
	dispchar[3] = 'C';
	dispchar[4] = 'O';
	dispchar[5] = 'M';
	dispchar[6] = 'E';
	dispchar[7] = ' ';
	show_clear_icon(DISP_LEVEL_SETTING);
	ui_show_string(DISP_LEVEL_SETTING, dispchar);
	ui_disp.now_level = DISP_LEVEL_SETTING;
	ui_disp.timer = 1500;
}
#endif

#if defined(_ENABLE_POWER_LOGO_)
static uint8 idle_usb_state3(void)
{
	if(file_operate_get_total_phydev())
	{
		if(dev_get_online_status(usb))
		{
			return TRUE;
		}
	}
	return FALSE;
}

static uint8 idle_card_state3(void)
{
#if defined(_SUPPORT_SD_)
	if(file_operate_get_total_phydev())
	{
		if(dev_get_online_status(sd1))
		{
			return TRUE;
		}
	}
#endif
	return FALSE;
}
#endif

void ui_show_idle_play(void *info)
{
	idle_disp_info_t *disp_info = (idle_disp_info_t *)info;
	
	ew_printf("disp_type:%d\n",disp_info->disp_type);
	
	switch(disp_info->disp_type)
	{
		case DISP_TYPE_CLEAN:
			ui_show_setting(LANG_ID_NULL, 0);
			ui_show_normal(LANG_ID_NULL);
			ui_disp.func_sel = FUNC_SEL_NULL;
			ui_disp.timer = -1;
			break;
			
		case DISP_TYPE_RTC:
			rtc_disp_time();
			break;
			
		case DISP_TYPE_WELCOME:
			ew_puts("DISP_TYPE_WELCOME\n");
		#if defined(_WLGK_POWER_FLASH_)
			dac_ldo_set(LDO_1,1);
			dac_ldo_set(LDO_2,1);
		#endif
			
		#if defined(_DISABLE_MEMORY_POWER_)
			setup.acc_flag = FALSE;
		#endif
		
	    #if defined(_FY5517_POWER_OFF_TIME_)
	    	ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_CLOCK);
	    #endif
		#if defined(HuiNuo_6651D_Copy)
			ui_drv_func.flash_disable(UI_ICON_MUTE);
		#endif
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MUTE);
		#if defined(SPECIAL_JD_DISP)||defined(SPECIAL_icon_DISP)
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER);
		#endif

		#if defined(_ENABLE_USB_DELAY_)
			if(setup.name == fm_radio_task_info.name)
			{
				ui_show_setting(LANG_ID_RADIO, 0);
				ui_show_normal(LANG_ID_RADIO);
				setup.name_flag = FALSE;
			}
			else if(setup.name == music_usb_task_info.name)
			{
				ui_show_setting(LANG_ID_MUSIC_USB, 0);
				ui_show_normal(LANG_ID_MUSIC_USB);
				setup.name_flag = FALSE;
			}
			else if(setup.name == music_card_task_info.name)
			{
				ui_show_setting(LANG_ID_MUSIC_SD, 0);
				ui_show_normal(LANG_ID_MUSIC_SD);
				setup.name_flag = FALSE;
			}
			else if(setup.name == bluetooth_task_info.name)
			{
				if(setup.name_flag)
				{
					ui_show_setting(LANG_ID_RADIO, 0);
					ui_show_normal(LANG_ID_RADIO);
					setup.name_flag = FALSE;
				}
				else
				{
					ui_show_setting(LANG_ID_BLUETOOTH, 0);
					ui_show_normal(LANG_ID_BLUETOOTH);
				}
			}
			else if(setup.name == linein_task_info.name)
			{
				ui_show_setting(LANG_ID_AUX_IN, 0);
				ui_show_normal(LANG_ID_AUX_IN);
				setup.name_flag = FALSE;
			}
		#elif defined(_ENABLE_POWER_LOGO_)
			if(setup.save_task_name == music_usb_task_info.name)
			{
			#if defined(_JTI_POWER_LOGO_)
    			ui_show_setting(LANG_ID_MUSIC_USB, 0);
    			ui_show_normal(LANG_ID_MUSIC_USB);
			#else
				if(idle_usb_state3())
				{
					puts("-----idle_usb_state3-----\n");
					ui_show_setting(LANG_ID_MUSIC_USB, 0);
					ui_show_normal(LANG_ID_MUSIC_USB);
				}
				else
				{
					puts("-----idle_usb_state4-----\n");
					ui_show_setting(LANG_ID_RADIO, 0);
					ui_show_normal(LANG_ID_RADIO);
				}
			#endif
			}
			else if(setup.save_task_name == music_card_task_info.name)
			{
				if(idle_card_state3())
				{
					ui_show_setting(LANG_ID_MUSIC_SD, 0);
					ui_show_normal(LANG_ID_MUSIC_SD);
				}
				else
				{
					ui_show_setting(LANG_ID_RADIO, 0);
					ui_show_normal(LANG_ID_RADIO);
				}
			}
			else if(setup.save_task_name == bluetooth_task_info.name)
			{
			#if defined(_DISABLE_BT_MEMORY_)
				global.radio_flag = TRUE;
			#endif
				ui_show_setting(LANG_ID_BLUETOOTH, 0);
				ui_show_normal(LANG_ID_BLUETOOTH);
			}
			else if(setup.save_task_name == fm_radio_task_info.name)
			{
				ui_show_setting(LANG_ID_RADIO, 0);
				ui_show_normal(LANG_ID_RADIO);
			}
		#elif defined(_SPECIAL_LOGO_DISPLAY_)
			ui_show_setting(LANG_ID_NULL, 0);
			ui_show_normal(LANG_ID_NULL);
			idle_disp->disp_ctrl.status = 1;
    		ui_show_idle_polling();
		#elif defined(_FY7555_LOGO_DISPLAY_)
    		ui_show_setting(LANG_ID_NULL, 0);
    		ui_show_normal(LANG_ID_NULL);
			idle_disp->disp_ctrl.status = 1;
    		ui_show_idle_polling();
		#elif defined(_HKY1032_SPECIAL_LOGO_)
			ui_show_welcome();
		#else
    		ui_show_setting(LANG_ID_SYSTEM_WELCOME, 0);
    		ui_show_normal(LANG_ID_SYSTEM_WELCOME);
		#endif
		#if defined(_ENABLE_ACC_OFF_LOGO_)
			setup.acc_on_flag = FALSE;
		#endif
			ui_disp.func_sel = FUNC_SEL_NULL;
			ui_disp.timer = -1;
			break;
			
		case DISP_TYPE_GOODBYE:
			ew_puts("DISP_TYPE_GOODBYE\n");
	    #if defined(_FY7555_LCD_DISPLAY_)
	    	ui_show_setting1(LANG_ID_NULL, 0);
	    	ui_show_normal1(LANG_ID_NULL);
	    #endif

	    #if defined(_FY700B_ICON_CLOSE_)||defined(_FY700E_ICON_CLOSE_)
	    	ui_idle_icon_disable();
	    #endif
		#if !defined(_YS_SPECIAL_DISP_)
		#if defined(_ENABLE_ACC_OFF_LOGO_)
			if(setup.acc_on_flag)
			{
				setup.acc_on_flag = FALSE;
			#if defined(_SPECIAL_POWER_OFF_DISPLAY_)
    			ui_show_setting(LANG_ID_NULL, 0);
    			ui_show_normal(LANG_ID_NULL);
    			idle_disp->disp_ctrl.status = 1;
        		ui_show_idle_polling();
			#else
				ui_show_setting(LANG_ID_SYSTEM_WELCOME, 0);
				ui_show_normal(LANG_ID_SYSTEM_WELCOME);
			#endif
    			ui_disp.func_sel = FUNC_SEL_NULL;
    			ui_disp.timer = -1;
			}
			else
			{
				if((global.first_power_on)&&(detect_usb_state())&&(detect_sd_state()))
				{
					global.first_power_on = FALSE;
					rtc_disp_time();
				}
				else
				{
				#if defined(_SPECIAL_POWER_OFF_DISPLAY_)
    				ui_show_setting(LANG_ID_NULL, 0);
    				ui_show_normal(LANG_ID_NULL);
    				idle_disp->disp_ctrl1.status = 1;
    				ui_show_idle_polling1();
				#else
					ui_show_setting(LANG_ID_SYSTEM_GOODBYE, 0);
					ui_show_normal(LANG_ID_SYSTEM_GOODBYE);
				#endif
					ui_disp.func_sel = FUNC_SEL_NULL;
					ui_disp.timer = -1;
				}
			}
		#elif defined(_SPECIAL_POWER_OFF_DISPLAY_)
			ui_show_setting(LANG_ID_NULL, 0);
			ui_show_normal(LANG_ID_NULL);
			idle_disp->disp_ctrl1.status = 1;
			ui_show_idle_polling1();
			ui_disp.func_sel = FUNC_SEL_NULL;
			ui_disp.timer = -1;
		#elif defined(_HKY1032_SPECIAL_LOGO_)
			ui_show_goodbye();
    		ui_disp.func_sel = FUNC_SEL_NULL;
    		ui_disp.timer = -1;
		#else
			ui_show_setting(LANG_ID_SYSTEM_GOODBYE, 0);
			ui_show_normal(LANG_ID_SYSTEM_GOODBYE);
			ui_disp.func_sel = FUNC_SEL_NULL;
			ui_disp.timer = -1;
		#endif
		#endif
			break;
			
		case DISP_TYPE_ICON_ENABLE:
			ui_idle_icon_enable();
			break;
			
		case DISP_TYPE_ICON_DISABLE:
			ui_idle_icon_disable();
			break;
			
    #if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
    	case DISP_TYPE_PASSWORD:
    		SYS_POW_ON();
		#if defined(_ENABLE_PASSWORD_SET_)
			if(setup.pwd_set)
			{
				ui_show_setting(LANG_ID_PWD_SET_DISPLAY, 0);
			}
			else
			{
				ui_show_setting(LANG_ID_PASSWORD, 0);
			}
		#else
    		ui_show_setting(LANG_ID_PASSWORD, 0);
		#endif
    		ui_disp.func_sel = FUNC_SEL_NULL;
    		ui_disp.timer = 100;
    		break;
    #endif
	
		default:
			break;
	}
}

