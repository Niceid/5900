#include "sdk_cfg.h"
#include "setup.h"
#include "audio_drv.h"
#include "ui_res.h"
#include "ui_api.h"
#include "disp.h"
#include "key_drv.h"
#include "rtc.h"
#include "timer.h"
#include "dev_ctl.h"
#include "dac/dac_api.h"
#include "rds/rds_ctrl.h"
#include "ui_radio/ui_radio.h"
#include "ui_drv.h"
#include "fm_band.h"
#include "file_operate/file_op.h"

#if defined(SUPPORT_ALARM_FUNC_)
#include "alarm/alarm.h"
#endif

#if defined(_ENABLE_MEMORY_PASSWORD_)
#include "vm/vm_api.h"
#endif

#if defined(_SUPPORT_CAN_UART_)
#include "ew3048\ew3048.h"
#endif

#if defined(_SUPPORT_DISC_)
#include "disc/disc.h"
#endif
#include "key_drv_ad.h"
#include "bluetooth/avctp_user.h"
#if defined(_SUPPORT_LCM_DRIVE)
#include "Lcm_drive.h"
#endif
#if defined(_SUPPORT_LCM_1618_DRIVE)
#include "Lcm_1618_drive.h"
#include "lcd/pt1621/pt1621.h"
lcd_info_t lcd_info;
#endif
#if defined(_SUPPORT_PANEL_74HC595)
extern void latches_send_data(uint8 Data);
#endif

#if defined(_ENABLE_BT_ON_OFF_)
extern void recon_standby_bt(void);
#endif

#if defined(_SUPPORT_PLAYER_NAMES_)
#include "play_sel/play_sel.h"
#endif
#if defined(_SUPPORT_30P60_LIGHT_)
#if defined(_ENABLE_LIGHT_SHADE_)
extern void LightDriverIrCmd(uint8 cmd,uint8 red,uint8 green,uint8 blue);
#else
extern void LightDriverIrCmd(uint8 cmd);
#endif
#endif

#ifdef	TD_ADD_LCD_TEST
uint8 gDispPos=0;
#endif

#if defined(_ENABLE_ALARM_CLOCK_)
uint8 flag=0;
#endif

#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_System_Status(void);
extern void TFT_Tx_Sel(uint8 SelType);
extern void TFT_Tx_Audio(void);
#endif

#if defined(_ENABLE_RDS_ON_OFF_)
extern void cmd_radio_rds_onoff(uint8 onoff);
#endif

#define DISP_DEBUG
#ifdef DISP_DEBUG
#define disp_printf 		ew_printf
#define disp_puts 			ew_puts
#else
#define disp_printf(...)
#define disp_puts(...)
#endif

static const uint8 disp_func_sel_tab[]=
{
	COMMON_SETTING_FUNC_LIST//显示SEL里面的VOL BAS ....
};
#if defined(_ENABLE_SET_FUNTION_)
static const uint8 disp_func_sel_tab1[]=
{
	COMMON_SETTING_FUNC_LIST1//显示SEL里面的VOL BAS ....
};
#endif
#if defined(COMMON_FUNC_FUNC_LIST)
static const uint8 disp_func_func_tab[]=
{
	COMMON_FUNC_FUNC_LIST//显示多功能 RDS SUB AREA...
};
#endif

static const uint8 disp_eq_sel_tab[]=
{
	COMMON_SETTING_EQ_LIST//显示EQ信息
};
#if defined(COMMON_SETTING_AREA_LIST)
static const uint8 disp_area_sel_tab[]=
{
	COMMON_SETTING_AREA_LIST//显示欧美
};
#endif
#if defined(_SEL_ADD_TIME_SELECT_)
static const uint8 disp_clock_sel_tab[]=
{
	COMMON_SETTING_CLOCK_LIST
};
#endif
#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
static const uint8 disp_color_sel_tab[]=
{
	COMMON_SETTING_COLOR
};
#endif
#if defined(SUPPORT_LED_ADJUST)
static const uint8 disp_led_sel_tab[]=
{
	COMMON_SETTING_LED
};
#endif

#if defined(_SUPPORT_CAN_UART_)
static const uint8 disp_func_can_tab[]=
{
	COMMON_SETTING_CAN_LIST
};

static const uint8 disp_can_sel_tab[]=
{
	COMMON_SETTING_CAN_DISP_LIST
};

static const uint8 disp_can_speed_sel_tab[]=
{
	COMMON_SETTING_CAN_SPEED_DISP_LIST
};
#endif

#if defined(_ENABLE_TEMP_PWM_)
static const uint8 disp_func_temp_tab[]=
{
	COMMON_SETTING_TEMP
};
static const uint8 disp_func_warm_tab[]=
{
	COMMON_SETTING_WARM
};
#endif

#if defined(_ENABLE_RPT_RDM_FUNTION_)
static const uint8 disp_rpt_sel_tab[]=
{
	COMMON_SETTING_RPT_LIST
};
#endif

func_sel_check_f func_sel_check = NULL;
static uint8 mute_enable = FALSE;
static uint16 mute_time = 0;
static uint8 mute_state = FALSE;
static uint8 inside_mute_state = FALSE;
static uint8 aud_mute_state = FALSE;
uint8 setflag;
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
static void disp_func_tab_next(uint8 *now,const uint8 *tab, uint8 length, uint8 add_sub)
{
	uint8 index;

	for(index = 0; index < length; index++)
	{
		if((*now) == tab[index])
		{
			break;
		}
	}

	if(index == length)
	{
		index = 0;
	}
	else
	{
		if(add_sub)
		{
			if((++index) == length)
			{
				index = 0;
			}
		}
		else
		{
			if(index)index--;
			else
			{
				index = length - 1;
			}
		}
	}
	*now = tab[index];
}

static void disp_func_sel_control(void)
{
	switch(ui_disp.func_sel)
	{
		case FUNC_SEL_VOLUME:
		#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
		#endif
			audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
			break;

		case FUNC_SEL_BASS:
		#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
		#endif
			audio_drv_ctrl(AUD_CTRL_BASS, setup.aud_bass, TRUE);
			break;
			
    #if defined(_ENABLE_7419_MIDDLE_)
    	case FUNC_SEL_MIDDLE:
		#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
		#endif
    		audio_drv_ctrl(AUD_CTRL_MIDDLE, setup.aud_middle, TRUE);
    		break;
    #endif

		case FUNC_SEL_TREBLE:
		#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
		#endif
			audio_drv_ctrl(AUD_CTRL_TREBLE, setup.aud_treble, TRUE);
			break;

	#if !defined(_FAD_BAL_FUNTION_)
		case FUNC_SEL_BAL:
		#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
		#endif
			audio_drv_ctrl(AUD_CTRL_LR, setup.aud_bal, TRUE);
			break;

		case FUNC_SEL_FAD:
		#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
		#endif
			audio_drv_ctrl(AUD_CTRL_FR, setup.aud_fad, TRUE);
			break;
	#endif

		case FUNC_SEL_LOUD:
		#if defined(TD_ADD_NEW_APP)
	    	global.audio_flag = TRUE;
	    #endif
			audio_drv_ctrl(AUD_CTRL_LOUD, setup.aud_loud, TRUE);
			break;

		case FUNC_SEL_EQ:
		#if defined(TD_ADD_NEW_APP)
	    	global.audio_flag = TRUE;
	    #endif
			audio_drv_ctrl(AUD_CTRL_EQ, setup.aud_eq, TRUE);
			break;

	#if defined(_SPECIAL_TL2000_SET_)
		case FUNC_SEL_A_VOLUME:
			audio_drv_ctrl(AUD_CTRL_A_VOLUME, setup.aud_A_volume, TRUE);
			break;

		case FUNC_SEL_B_VOLUME:
			audio_drv_ctrl(AUD_CTRL_B_VOLUME, setup.aud_B_volume, TRUE);
			break;
	#endif
	
	#if defined(_ENABLE_AP3768_SUBWOOFER_)
		case FUNC_SEL_SUB:
			audio_drv_ctrl(AUD_CTRL_SUBWOOFER1, setup.aud_sub, TRUE);
			break;

		case FUNC_SEL_SUB1:
			audio_drv_ctrl(AUD_CTRL_SUBWOOFER2, setup.aud_sub1, TRUE);
			break;
	#endif

	#if defined(_SUPPORT_SUBWOOFER_)||defined(_ENABLE_SUB_OUTPUT_)
		case FUNC_SEL_SUBWOOFER:
			audio_drv_ctrl(AUD_CTRL_SUBWOOFER, setup.aud_subwoofer, TRUE);
			break;
	#endif

    #if defined(_SUPPORT_SUBWOOFER_MODE2_)
    	case FUNC_SEL_SUBWOOFER:
    		audio_drv_ctrl(AUD_CTRL_SUBWOOFER, setup.aud_subwoofer, TRUE);
    		break;
    #endif

    #if defined(AUDIO_SUB_LEVEL_SET)
    	case FUNC_SEL_SUB_CUT_CTRL:
    		audio_drv_ctrl(AUD_CTRL_SUB_CUT, setup.aud_sub_ctrl, TRUE);
    		break;
    #endif
	
    #if defined(_FAD_BAL_FUNTION_)
		case FUNC_SEL_FL:
    	#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
			global.app_data = FALSE;;
    	#endif
			audio_drv_ctrl(AUD_CTRL_FL, setup.aud_fl, TRUE);
			break;
		case FUNC_SEL_FR:
    	#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
			global.app_data = FALSE;;
    	#endif
			audio_drv_ctrl(AUD_CTRL_FR, setup.aud_fr, TRUE);
			break;
		case FUNC_SEL_RL:
    	#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
			global.app_data = FALSE;;
    	#endif
			audio_drv_ctrl(AUD_CTRL_RL, setup.aud_rl, TRUE);
			break;
		case FUNC_SEL_RR:
    	#if defined(TD_ADD_NEW_APP)
			global.audio_flag = TRUE;
			global.app_data = FALSE;;
    	#endif
			audio_drv_ctrl(AUD_CTRL_RR, setup.aud_rr, TRUE);
			break;
    #endif

	#if defined(_SUPPORT_10_EQ_FUNTION_)
    	case FUNC_SEL_60HZ:
    		audio_drv_ctrl(AUD_CTRL_60HZ, setup.aud_60hz, TRUE);
    		break;
		case FUNC_SEL_150HZ:
			audio_drv_ctrl(AUD_CTRL_150HZ, setup.aud_150hz, TRUE);
			break;
		case FUNC_SEL_400HZ:
			audio_drv_ctrl(AUD_CTRL_400HZ, setup.aud_400hz, TRUE);
			break;
		case FUNC_SEL_1KHZ:
			audio_drv_ctrl(AUD_CTRL_1KHZ, setup.aud_1khz, TRUE);
			break;
		case FUNC_SEL_2KHZ:
			audio_drv_ctrl(AUD_CTRL_2KHZ, setup.aud_2khz, TRUE);
			break;
		case FUNC_SEL_3KHZ:
			audio_drv_ctrl(AUD_CTRL_3KHZ, setup.aud_3khz, TRUE);
			break;
		case FUNC_SEL_7KHZ:
			audio_drv_ctrl(AUD_CTRL_7KHZ, setup.aud_7khz, TRUE);
			break;
		case FUNC_SEL_10KHZ:
			audio_drv_ctrl(AUD_CTRL_10KHZ, setup.aud_10khz, TRUE);
			break;
		case FUNC_SEL_15KHZ:
			audio_drv_ctrl(AUD_CTRL_15KHZ, setup.aud_15khz, TRUE);
			break;
		case FUNC_SEL_20KHZ:
			audio_drv_ctrl(AUD_CTRL_20KHZ, setup.aud_20khz, TRUE);
			break;
	#endif

	#if defined(_ADJUST_A_B_SPK_)
		case FUNC_SEL_A_SPK:
			if(global.sys_power)
	    	{
				if(setup.A_SPK)
				{
					AUDIO_A_SPK_LOW();
				}
				else
				{
					AUDIO_A_SPK_HIGH();
				}
	    	}
			break;
		case FUNC_SEL_B_SPK:	        
	        if(global.sys_power)
	    	{
				if(setup.B_SPK)
				{
					AUDIO_B_SPK_LOW();
				}
				else
				{
					AUDIO_B_SPK_HIGH();
				}
	    	}
			break;
    #endif
	
	#if defined(_MR889_SPECAIL_KEY_)
		case FUNC_SEL_CONTROL_SPK:
			if(global.sys_power)
	    	{
				if(setup.CONTROL_SPK)
				{
					AUDIO_CONTROL_SPK_LOW();
				}
				else
				{
					AUDIO_CONTROL_SPK_HIGH();
				}
	    	}
			break;
    #endif

	#if defined(_SUPPORT_AREA_)
		case FUNC_SEL_AREA:
			{
				typedef void (*radio_area_f)(setup_area_e);
				radio_area_f radio_area;

				if((func_sel_check != NULL)&&((radio_area = (radio_area_f)func_sel_check(FUNC_SEL_AREA)) != NULL))
				{
					radio_area(setup.area);
				}
			}
			break;
	#endif
	
		default:
			break;
	}
}

#if defined(_SUPPORT_VIDEO_)
static void video_show_sel(func_sel_e func_sel)
{
	int msg = 0;
	int value;

	switch(func_sel)
	{
		case FUNC_SEL_VOLUME:
			msg = MSG_VIDEO_VOLUME;
        	value = setup.aud_vol;
			break;
		case FUNC_SEL_BASS:
			msg = MSG_VIDEO_BASS;
			value = setup.aud_bass;
			break;
		case FUNC_SEL_TREBLE:
			msg = MSG_VIDEO_TREBLE;
			value = setup.aud_treble;
			break;
		case FUNC_SEL_BAL:
			msg = MSG_VIDEO_BAL;
	    	value = setup.aud_bal;
			break;
		case FUNC_SEL_FAD:
			msg = MSG_VIDEO_FAD;
	    	value = setup.aud_fad;
			break;
		case FUNC_SEL_LOUD:
			msg = MSG_VIDEO_LOUD;
        	value = setup.aud_loud;
			break;
		case FUNC_SEL_EQ:
			msg = MSG_VIDEO_EQ;
        	value = setup.aud_eq;
			break;
		case FUNC_SEL_MUTE:
			msg = MSG_VIDEO_MUTE;
			value = global.aud_mute;
			break;
	}

	if(msg != 0)
	{
		if(global.disc_task_name != NULL)
		{
			os_taskq_post_msg(global.disc_task_name, 2, msg, value);
		}
	}
}
#endif

static void disp_show_sel(func_sel_e func_sel)
{
	ui_show_sel(func_sel);
#if defined(_SUPPORT_VIDEO_)
	video_show_sel(func_sel);
#endif
#if defined(_USER_TFT_SEND_)
	//TFT_Tx_System_Status();
	//OSTimeDly(10);
	TFT_Tx_Sel(func_sel);
	OSTimeDly(10);
    TFT_Tx_Audio();
	OSTimeDly(10);
#endif
}


static void disp_func_mute_control(void)
{
	uint8 mute;

	if(global.aud_mute || mute_state || inside_mute_state)
	{
		mute = ON;
	}
	else
	{
		mute = OFF;
	}

	if(aud_mute_state != mute)
	{
    	aud_mute_state = mute;
    	audio_drv_ctrl(AUD_CTRL_MUTE, mute, TRUE);
	#if defined(_SUPPORT_TWO_PT2313_)
		audio_drv_ctrl1(AUD_CTRL_MUTE, mute, TRUE);
	#endif
	}
}

#if defined(_XM003_SPECIAL_MUTE_)||defined(_YH1701_SPECIAL_MUTE_)||defined(_XM007_SPECIAL_MUTE_)
void func_mute_control(void)
{
	uint8 mute;

	if(global.aud_mute || mute_state || inside_mute_state)
	{
		mute = ON;
	}
	else
	{
		mute = OFF;
	}

	if(aud_mute_state != mute)
	{
    	aud_mute_state = mute;
    	audio_drv_ctrl(AUD_CTRL_MUTE, mute, TRUE);
	}
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MUTE);
}
#endif

static uint8 disp_func_sel_others(uint8 up_down)
{
#if defined(_ENABLE_RADIO_RDS_)
	if((ui_disp.func_sel > FUNC_RDS_BEGIN)&&(ui_disp.func_sel < FUNC_RDS_END))
	{
		if(radio_rds_ctrl(ui_disp.func_sel, up_down ? RADIO_RDS_CTRL_ADD : RADIO_RDS_CTRL_SUB))return TRUE;
	}
#endif

	if(ui_disp.func_type == FUNC_TYPE_SEL)
	{
	#if defined(ENABLE_COLOP_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[1];
	#elif defined(ENABLE_VOL_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[4];
	#elif defined(TS5258_ENABLE_VOL_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[8];
	#elif defined(ENABLE_NO_FAD_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[7];
	#elif defined(_XZC1668_SEL_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[6];
	#elif defined(ENABLE_SEL_SETTING)
    	ui_disp.func_sel = disp_func_sel_tab[8];
	#elif defined(ENABLE_SEL_SETTING1)||defined(_JYL3435_SEL_SETTING_)
    	ui_disp.func_sel = disp_func_sel_tab[9];
	#elif defined(ENABLE_SEL_SETTING2)
    	ui_disp.func_sel = disp_func_sel_tab[5];
	#elif defined(ENABLE_JD_SEL_SETTING)||defined(_JYL3435_SEL_CLOUR_SETTING_)
    	ui_disp.func_sel = disp_func_sel_tab[10];
	#elif defined(HY601_SEL_SETTING)||defined(_YS_SEL_SETTING_)
    	ui_disp.func_sel = disp_func_sel_tab[3];
	#elif defined(_XM007_MENU_SET_)
    	#if defined(_XM407_MENU_SET_)
    		ui_disp.func_sel = disp_func_sel_tab[2];
    	#else
    		ui_disp.func_sel = disp_func_sel_tab[6];
    	#endif
	#else
    	ui_disp.func_sel = disp_func_sel_tab[0];
	#endif
    	return FALSE;//解决第一次转编码器没作用的问题
	}
	else if(ui_disp.func_type == FUNC_TYPE_FUNC)
	{
	#if defined(COMMON_FUNC_FUNC_LIST)
		if(!sizeof(disp_func_func_tab))
		{
			return FALSE;
		}
		ui_disp.func_sel = disp_func_func_tab[0];
	#else
		return FALSE;
	#endif
	}
	else return FALSE;

	return TRUE;
}

#if defined(_ENABLE_SET_FUNTION_)
static uint8 disp_func_sel_others1(uint8 up_down)
{
#if defined(_ENABLE_RADIO_RDS_)
	if((ui_disp.func_sel > FUNC_RDS_BEGIN)&&(ui_disp.func_sel < FUNC_RDS_END))
	{
		if(radio_rds_ctrl(ui_disp.func_sel, up_down ? RADIO_RDS_CTRL_ADD : RADIO_RDS_CTRL_SUB))return TRUE;
	}
#endif

	if(ui_disp.func_type == FUNC_TYPE_SEL)
	{
	#if defined(ENABLE_COLOP_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[1];
	#elif defined(ENABLE_VOL_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[4];
	#elif defined(TS5258_ENABLE_VOL_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[8];
	#elif defined(ENABLE_NO_FAD_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[7];
	#elif defined(_XZC1668_SEL_SETTING)
		ui_disp.func_sel = disp_func_sel_tab[6];
	#elif defined(ENABLE_SEL_SETTING)
    	ui_disp.func_sel = disp_func_sel_tab[8];
	#elif defined(ENABLE_SEL_SETTING1)
    	ui_disp.func_sel = disp_func_sel_tab[9];
	#elif defined(ENABLE_SEL_SETTING2)
    	ui_disp.func_sel = disp_func_sel_tab[5];
	#elif defined(ENABLE_JD_SEL_SETTING)
    	ui_disp.func_sel = disp_func_sel_tab[10];
	#elif defined(HY601_SEL_SETTING)||defined(_YS_SEL_SETTING_)
    	ui_disp.func_sel = disp_func_sel_tab[3];
	#else
    	ui_disp.func_sel = disp_func_sel_tab[0];
	#endif
    	return FALSE;//解决第一次转编码器没作用的问题
	}
	else if(ui_disp.func_type == FUNC_TYPE_FUNC)
	{
	#if defined(COMMON_FUNC_FUNC_LIST1)
		if(!sizeof(disp_func_func_tab1))
		{
			return FALSE;
		}
		ui_disp.func_sel = disp_func_func_tab1[0];
	#else
		return FALSE;
	#endif
	}
	else return FALSE;

	return TRUE;
}

#endif

static uint8 disp_func_sel_enter(func_sel_e func_sel)
{
#if defined(_ENABLE_7419_MIDDLE_)
	if((func_sel == FUNC_SEL_BASS)||(func_sel == FUNC_SEL_TREBLE)||(func_sel == FUNC_SEL_MIDDLE))
#else
	if((func_sel == FUNC_SEL_BASS)||(func_sel == FUNC_SEL_TREBLE))
#endif
	{
	#if !defined(_YS_SEL_SETTING_)
		if(setup.aud_eq != AUDIO_EQ_OFF)
		{
			return FALSE;
		}
	#endif
	}

#if defined(_CLOSE_EQ_ICON_)
    else if(func_sel == FUNC_SEL_EQ)
    {
    	if((!compare_task_name("MusicTaskUsb"))&&(!compare_task_name("MusicTaskCard"))&&(!compare_task_name("BtUserTask")))
    	{
    		return FALSE;
    	}
    }
#endif

#if defined(_ENABLE_RDS_ON_OFF_)
	else if((func_sel == FUNC_RDS_AF)||(func_sel == FUNC_RDS_TA))
	{
		if((!setup.rds_onoff)||(!compare_task_name("FMRadioTask")))
		{
			return FALSE;
		}
	}
	else if(func_sel == FUNC_RDS_REG)
	{
		if((!setup.rds_onoff)||(!compare_task_name("FMRadioTask"))||(!setup.rds.af))
		{
			return FALSE;
		}
	}
	else if(func_sel == FUNC_RDS_ONOFF)
	{
		if(!compare_task_name("FMRadioTask"))
		{
			return FALSE;
		}
	}
#endif

#if defined(_ENABLE_RPT_RDM_FUNTION_)
	else if((func_sel == FUNC_SEL_RPT)||(func_sel == FUNC_SEL_RDM))
	{
		if((!compare_task_name("MusicTaskUsb"))&&(!compare_task_name("MusicTaskCard")))
		{
			return FALSE;
		}
	}
#endif

#if defined(_ENABLE_RADIO_RDS_)
#if defined(DAB_MODLE)
#if defined(_DAB_MODLE_RDS_FIRST_)
	else if((func_sel == FUNC_RDS_AF)||(func_sel == FUNC_RDS_TA))
	{
		if(compare_task_name(DAB_TASK_NAME))
		{
			return FALSE;
		}
	}
#endif
#endif
#endif

#if defined(_ENABLE_RADIO_RDS_)
	else if((func_sel > FUNC_RDS_BEGIN)&&(func_sel < FUNC_RDS_END))
	{
    #if defined(_DISABLE_RDS_FUNTION_)
    	if(setup.area == SETUP_AREA_USA)
    	{
    		return FALSE;
    	}
    	else
    	{
    		if(!radio_rds_sel(func_sel))return FALSE;
    	}
    #else
    	if(!radio_rds_sel(func_sel))return FALSE;
    #endif
	}
#endif

#if defined(_SUPPORT_AREA_)
	else if(func_sel == FUNC_SEL_AREA)
	{
		if(!compare_task_name(FM_TASK_NAME))
		{
			return FALSE;
		}
	}
#endif

#if defined(_SUPPORT_SUBWOOFER2_)&&0
	else if((func_sel == FUNC_SEL_SUBWOOFER)||(func_sel == FUNC_SEL_SUB_CUT_CTRL))
	{
		if(setup.aud_sub == 0)
		{
			return FALSE;
		}
	}
#endif

#if defined(_ENABLE_ID3_ON_OFF_)||defined(_ENABLE_ID3_SET_)
	else if(func_sel == FUNC_SEL_ID3)
	{
		if((!compare_task_name("MusicTaskUsb"))&&(!compare_task_name("MusicTaskCard")))
		{
			return FALSE;
		}
	}
#endif

	else if((func_sel > FUNC_PRIVATELY_BEGIN)&&(func_sel < FUNC_PRIVATELY_END))
	{
		if((func_sel_check == NULL)||(func_sel_check(func_sel) == NULL))return FALSE;
	}

	ui_disp.func_sel = func_sel;
#if defined(SEL_SET_CLOCK_DISP)||defined(_ENABLE_SEL_MENU_TIME_)||defined(_MR9002_MENU_SET_)
	if(ui_disp.func_sel == FUNC_SEL_CLOCK_DISP)
	{
	#if defined(_FEIYIN_MENU_TIME_)
		ui_show_setting(LANG_ID_CLK_SET, 0);
		ui_disp.now_level = DISP_LEVEL_SETTING;
		ui_disp.func_sel = FUNC_SEL_TIME;
	#if defined(_FEIYIN_RDS_SET_)
		global.sel_rds = TRUE;
	#endif
	#else
		msg_realtime_show();
	#if defined(_ENABLE_SEL_FUNTION_)
		global.sel_mode = TRUE;
	#endif
	#endif
	}
	else
#endif
	disp_show_sel(func_sel);
	return TRUE;
}

#if defined(_ENABLE_SET_FUNTION_)
static uint8 disp_func_sel_next1(void)
{
	uint8 now_func_sel;
	uint8 const *func_tab;
	uint8 length;
	uint8 loop = 0;

	if(ui_disp.func_type == FUNC_TYPE_SEL)
	{
		func_tab = disp_func_sel_tab1;
		length = sizeof(disp_func_sel_tab1);
	}
	else if(ui_disp.func_type == FUNC_TYPE_FUNC)
	{
	#if defined(COMMON_FUNC_FUNC_LIST1)
		func_tab = disp_func_func_tab1;
		length  = sizeof(disp_func_func_tab1);
		if(!length)
		{
			return FALSE;
		}
	#else
		return FALSE;
	#endif
	}
	else return FALSE;

	now_func_sel = ui_disp.func_sel;

FUNC_SEL_AGAIN:

	disp_func_tab_next(&now_func_sel, func_tab, length, 1);

	if(!disp_func_sel_enter(now_func_sel))
	{
		if((++loop) == length)
		{
			return FALSE;
		}
		else
		{
			goto FUNC_SEL_AGAIN;
		}
	}

	return TRUE;
}
#endif

static uint8 disp_func_sel_next(void)
{
	uint8 now_func_sel;
	uint8 const *func_tab;
	uint8 length;
	uint8 loop = 0;

	if(ui_disp.func_type == FUNC_TYPE_SEL)
	{
		puts("disp_func_sel_sel1111\n");
    #if defined(_ENABLE_MENU_SET_)
    	global.menu_flag = TRUE;
    #endif
	#if defined(_SUPPORT_CAN_UART_)
		if(compare_task_name("CanTask"))
		{
			func_tab = disp_func_can_tab;
			length = sizeof(disp_func_can_tab);
		}
		else
		{
			func_tab = disp_func_sel_tab;
			length = sizeof(disp_func_sel_tab);
		}
	#else
		func_tab = disp_func_sel_tab;
		length = sizeof(disp_func_sel_tab);
	#endif
	}
	else if(ui_disp.func_type == FUNC_TYPE_FUNC)
	{
		puts("disp_func_sel_func111\n");
    #if defined(_ENABLE_MENU_SET_)
    	global.menu_flag = TRUE;
    #endif
	#if defined(COMMON_FUNC_FUNC_LIST)
		func_tab = disp_func_func_tab;
		length  = sizeof(disp_func_func_tab);
		if(!length)
		{
			return FALSE;
		}
	#else
		return FALSE;
	#endif
	}
	else return FALSE;
#if defined(SEL_SET_CLOCK_DISP)
	if(ui_disp.func_sel == FUNC_SEL_TIME)
	{
		ui_disp.func_sel = FUNC_SEL_NULL;
	}
#endif
	now_func_sel = ui_disp.func_sel;

FUNC_SEL_AGAIN:

	disp_func_tab_next(&now_func_sel, func_tab, length, 1);

	if(!disp_func_sel_enter(now_func_sel))
	{
		if((++loop) == length)
		{
			return FALSE;
		}
		else
		{
			goto FUNC_SEL_AGAIN;
		}
	}

	return TRUE;
}
static void disp_func_sel_sub(void)
{
	uint8 control = FALSE;

func_sel_sub_again:
	switch(ui_disp.func_sel)
	{
	#if !defined(_ENABLE_MENU_SET_)
	#if !defined(_DISABLE_SEL_VOLUME_)
		case FUNC_SEL_VOLUME:
			if(get_cur_vol() > 0)
	        {
	        	set_cur_vol(get_cur_vol() - 1);
				control = TRUE;
	        }
			break;
	#endif
	#endif
		case FUNC_SEL_BASS:
			if(setup.aud_bass > AUDIO_BASS_MIN)
	        {
	            setup.aud_bass--;
				control = TRUE;
	        }
			break;
    #if defined(_ENABLE_7419_MIDDLE_)
    	case FUNC_SEL_MIDDLE:
    		if(setup.aud_middle > AUDIO_MIDDLE_MIN)
    		{
    			setup.aud_middle--;
    			control = TRUE;
    		}
    		break;
    
    #endif
		case FUNC_SEL_TREBLE:
			if(setup.aud_treble > AUDIO_TREBLE_MIN)
	        {
	            setup.aud_treble--;
				control = TRUE;
	        }
			break;
	#if !defined(_FAD_BAL_FUNTION_)
		case FUNC_SEL_BAL:
			if(setup.aud_bal > AUDIO_LR_MIN)
	        {
	            setup.aud_bal--;
				control = TRUE;
	        }
			break;
		case FUNC_SEL_FAD:
			if(setup.aud_fad > AUDIO_FR_MIN)
	        {
	            setup.aud_fad--;
				control = TRUE;
	        }
			break;
	#endif
		case FUNC_SEL_LOUD:
		#if defined(_SL6812A_MENU_SET_)
    		if(setup.aud_loud == AUDIO_LOUDNESS_ON)
    		{
    			setup.aud_loud = AUDIO_LOUDNESS_OFF;
    		}
		#else
			if(setup.aud_loud == AUDIO_LOUDNESS_OFF)
	        {
	        	setup.aud_loud = AUDIO_LOUDNESS_ON;
	        }
			else
			{
				setup.aud_loud = AUDIO_LOUDNESS_OFF;
			}
		#endif
			control = TRUE;
			break;
		case FUNC_SEL_EQ:
		#if defined(_SL6812A_MENU_SET_)
			if(setup.aud_eq == AUDIO_EQ_OFF)break;
		#endif
			disp_func_tab_next(&setup.aud_eq, disp_eq_sel_tab, sizeof(disp_eq_sel_tab), 0);
			control = TRUE;
			break;
    #if defined(_ENABLE_RPT_RDM_FUNTION_)
    	case FUNC_SEL_RPT:
    		disp_func_tab_next(&setup.rpt_start, disp_rpt_sel_tab, sizeof(disp_rpt_sel_tab), 0);
    		if(setup.rpt_start)
    		{
    			setup.rdm_start = FALSE;
    			setup.rdm_start1 = FALSE;
    		}
    		break;
    
    	case FUNC_SEL_RDM:
    		if(setup.rdm_start)
    		{
    			setup.rdm_start = FALSE;
    		}
    		else
    		{
    			setup.rdm_start = TRUE;
    			setup.rpt_start = FALSE;
    		}
    		break;
    #endif
	#if defined(_SPECIAL_TL2000_SET_)
		case FUNC_SEL_A_VOLUME:
			if(setup.aud_A_volume > AUDIO_A_VOLUME_MIN)
	        {
	            setup.aud_A_volume--;
				control = TRUE;
	        }
			break;
		case FUNC_SEL_B_VOLUME:
			if(setup.aud_B_volume > AUDIO_B_VOLUME_MIN)
	        {
	            setup.aud_B_volume--;
				control = TRUE;
	        }
			break;
	#endif
	
    #if defined(_SUPPORT_SUB_)
    	case FUNC_SEL_SUB:
    		if(setup.sub_start)
            {
            	setup.sub_start = OFF;
            }
    		else
    		{
    			setup.sub_start = ON;
    		}
    		break;
    #endif

	#if defined(_CHANGE_DEF_VOL_)
		case FUNC_SEL_DEFVOLUME:
			if(setup.DefVol > AUDIO_VOLUME_MIN)
	        {
	            setup.DefVol--;
				control = TRUE;
	        }
			break;
	#endif
	
    #if defined(_ENABLE_TALK_VOL_)
    	case FUNC_SEL_TALK_VOL:
    		if(setup.talk_volume > AUDIO_VOLUME_MIN)
    		{
    			setup.talk_volume--;
    		}
    		break;
    #endif
	
    #if defined(_ENABLE_BT_ON_OFF_)
    	case FUNC_SEL_BT_ON:
    		if(setup.bt_on)
    		{
    			setup.bt_on = FALSE;
				user_send_cmd_prepare(USER_CTRL_DISCONNECTION_HCI,0,NULL);
    			os_time_dly(10);
    			user_send_cmd_prepare(USER_CTRL_DEL_ALL_REMOTE_INFO,0,NULL);
				os_time_dly(10);
				user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
				user_send_cmd_prepare(USER_CTRL_WRITE_CONN_DISABLE,0,NULL);
				setup.power_off_flag = TRUE;
    		}
			else
			{
    			setup.bt_on = TRUE;
				user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
				user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE,0,NULL);
				recon_standby_bt();
				setup.power_off_flag = FALSE;
				setup.acc_off_flag = FALSE;
			}
    		break;
    #endif
	
	#if defined(_ENABLE_AP3768_SUBWOOFER_)
		case FUNC_SEL_SUB:
        	if(setup.aud_sub == 0)
        	{
        		setup.aud_sub = 1;
        	}
        	else
        	{
        		setup.aud_sub = 0;
        	}
			control = TRUE;
    		break;
	
		case FUNC_SEL_SUB1:
        	if(setup.aud_sub1 == 0)
        	{
        		setup.aud_sub1 = 1;
        	}
        	else
        	{
        		setup.aud_sub1 = 0;
        	}
			control = TRUE;
    		break;
    #endif

	#if defined(_SUPPORT_SUBWOOFER_)||defined(_ENABLE_SUB_OUTPUT_)
		case FUNC_SEL_SUBWOOFER:
			if(setup.aud_subwoofer == AUDIO_SUBWOOFER_OFF)
	        {
	        	setup.aud_subwoofer = AUDIO_SUBWOOFER_ON;
	        }
			else
			{
				setup.aud_subwoofer = AUDIO_SUBWOOFER_OFF;
			}
			control = TRUE;
			break;
	#endif
    #if defined(_SUPPORT_SUBWOOFER_MODE2_)
    	case FUNC_SEL_SUBWOOFER:
    		if(setup.aud_subwoofer > 0)
            {
                setup.aud_subwoofer--;
    			control = TRUE;
            }
    		break;
    #endif
    #if defined(AUDIO_SUB_LEVEL_SET)
    	case FUNC_SEL_SUB_CUT_CTRL:
    		if( setup.aud_sub_ctrl > AUDIO_SUBWOOFER_CUT_80)
            {
                setup.aud_sub_ctrl--;
    			control = TRUE;
            }
    		break;
    #endif
    #if defined(_SUPPORT_SUBWOOFER2_)
    	case FUNC_SEL_SUB:
        	if(setup.aud_sub == 0)
        	{
        		setup.aud_sub = 1;
			#if defined(_SUPPORT_AMP_CONTROL_)
				SYS_SUB_ON();
			#endif
        	}
        	else
        	{
        		setup.aud_sub = 0;
			#if defined(_SUPPORT_AMP_CONTROL_)
				SYS_SUB_OFF();
			#endif
        	}
        	break;
    #endif
	#if defined(_SUPPORT_SUBWOOFER3_)
    	case FUNC_SEL_SUB1:
        	if(setup.aud_sub1 == 0)
        	{
        		setup.aud_sub1 = 1;
		    #if defined(_SUPPORT_AMP_CONTROL_)
		    	SYS_SUB2_ON();
		    #endif
        	}
        	else
        	{
        		setup.aud_sub1 = 0;
			#if defined(_SUPPORT_AMP_CONTROL_)
				SYS_SUB2_OFF();
			#endif
        	}
        	break;
	#endif
	#if defined(_ENABLE_ID3_ON_OFF_)||defined(_ENABLE_ID3_SET_)
    	case FUNC_SEL_ID3:
    		if(setup.id3_on_flag)
            {
            	setup.id3_on_flag = OFF;
            }
    		else
    		{
    			setup.id3_on_flag = ON;
    		}
    		break;
	#endif
    #if defined(_FAD_BAL_FUNTION_)
    	case FUNC_SEL_FL:
    		if(setup.aud_fl > AUDIO_LR_MIN)
    		{
    			setup.aud_fl--;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_FR:
    		if(setup.aud_fr > AUDIO_LR_MIN)
    		{
    			setup.aud_fr--;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_RL:
    		if(setup.aud_rl > AUDIO_LR_MIN)
    		{
    			setup.aud_rl--;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_RR:
    		if(setup.aud_rr > AUDIO_LR_MIN)
    		{
    			setup.aud_rr--;
    			control = TRUE;
    		}
    		break;
    #endif

	#if defined(_SUPPORT_10_EQ_FUNTION_)
    	case FUNC_SEL_60HZ:
    		if(setup.aud_60hz > AUDIO_60HZ_MIN)
    		{
    			setup.aud_60hz--;
    			control = TRUE;
    		}
    		break;
		case FUNC_SEL_150HZ:
			if(setup.aud_150hz > AUDIO_150HZ_MIN)
			{
				setup.aud_150hz--;
				control = TRUE;
			}
			break;
		case FUNC_SEL_400HZ:
			if(setup.aud_400hz > AUDIO_400HZ_MIN)
			{
				setup.aud_400hz--;
				control = TRUE;
			}
			break;
		case FUNC_SEL_1KHZ:
			if(setup.aud_1khz > AUDIO_1KHZ_MIN)
			{
				setup.aud_1khz--;
				control = TRUE;
			}
			break;
		case FUNC_SEL_2KHZ:
			if(setup.aud_2khz > AUDIO_2KHZ_MIN)
			{
				setup.aud_2khz--;
				control = TRUE;
			}
			break;
		case FUNC_SEL_3KHZ:
			if(setup.aud_3khz > AUDIO_3KHZ_MIN)
			{
				setup.aud_3khz--;
				control = TRUE;
			}
			break;
		case FUNC_SEL_7KHZ:
			if(setup.aud_7khz > AUDIO_7KHZ_MIN)
			{
				setup.aud_7khz--;
				control = TRUE;
			}
			break;
		case FUNC_SEL_10KHZ:
			if(setup.aud_10khz > AUDIO_10KHZ_MIN)
			{
				setup.aud_10khz--;
				control = TRUE;
			}
			break;
		case FUNC_SEL_15KHZ:
			if(setup.aud_15khz > AUDIO_15KHZ_MIN)
			{
				setup.aud_15khz--;
				control = TRUE;
			}
			break;
		case FUNC_SEL_20KHZ:
			if(setup.aud_20khz > AUDIO_20KHZ_MIN)
			{
				setup.aud_20khz--;
				control = TRUE;
			}
			break;
	#endif
	#if defined(_ENABLE_BEEP_)
		case FUNC_SEL_BEEP:
			if(setup.aud_beep)
	        {
	        	setup.aud_beep = OFF;
	        }
			else
			{
				setup.aud_beep = ON;
			}
			break;
	#endif
	#if defined(_SUPPORT_AREA_)
    	case FUNC_SEL_AREA:
			disp_func_tab_next(&setup.area, disp_area_sel_tab, sizeof(disp_area_sel_tab), 0);
			control = TRUE;
			break;
    #endif
	#if defined(_SEL_ADD_TIME_SELECT_)
    	case FUNC_SEL_CLOCK:
    		disp_func_tab_next(&setup.rtc_disp, disp_clock_sel_tab, sizeof(disp_clock_sel_tab), 0);
    		control = TRUE;
    		break;
	#endif
    #if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    	case FUNC_SEL_7COLOR:
    		disp_func_tab_next(&setup.State, disp_color_sel_tab, sizeof(disp_color_sel_tab), 0);
        #if defined(_ENABLE_CHRAMATIC_CAMP_SHADE_)
        	if(GET_PANEL_LIGHT_STATE == PANEL_LIGHT_STATE_AUTO)
        	{
        		SET_PANEL_AUTO_STATE(0);
        		SET_PANEL_LIGHT_STATE(PANEL_LIGHT_STATE_AUTO);
        		setup.Time = 0;
			#if defined(_WLS316_PANEL_LIGHT_)
     			GET_LED_COLOR_LEVER_R = 0;
     			GET_LED_COLOR_LEVER_G = 0;
     			GET_LED_COLOR_LEVER_B = 0;
			#else
        		GET_LED_COLOR_LEVER_R = 0;
        		GET_LED_COLOR_LEVER_G = LED_COLOR_G_LEVER_MAX;
        		GET_LED_COLOR_LEVER_B = LED_COLOR_B_LEVER_MAX;
			#endif
        	}
        #endif
    		control = TRUE;
    		break;
    #endif
	#if defined(SUPPORT_LED_ADJUST)
    	case FUNC_SEL_LED:
    		disp_func_tab_next(&setup.LedState, disp_led_sel_tab, sizeof(disp_led_sel_tab), 0);
			break;
	#endif
	
    #if defined(_ENABLE_RDS_ON_OFF_)
		case FUNC_RDS_ONOFF:
		#if defined(_SL6812A_MENU_SET_)
			if(setup.rds_onoff)
			{
				setup.rds_onoff = 0;
			}
		#else
			if(setup.rds_onoff)
			{
				setup.rds_onoff = 0;
			}
			else
			{
				setup.rds_onoff = 1;
			}
		#endif
			if(compare_task_name("FMRadioTask"))
			{
				cmd_radio_rds_onoff(setup.rds_onoff);
			}
			else
			{
				global.rds_flag = TRUE;
			}
			break;
    #endif
	
    #if defined(_SL6812A_MENU_SET_)
    	case FUNC_RDS_AF:
    		if(setup.rds.af)
    		{
    			setup.rds.af = 0;
    			if(radio_rds_check())
    			{
    				ui_disp.func_sel = FUNC_RDS_AF;
    				disp_func_sel_others(TRUE);
    			}
    		}
    		break;
    		
    	case FUNC_RDS_TA:
    		if(setup.rds.ta)
    		{
    			setup.rds.ta = 0;
    			if(radio_rds_check())
    			{
    				ui_disp.func_sel = FUNC_RDS_TA;
    				disp_func_sel_others(TRUE);
    			}
    		}
    		break;
			
		case FUNC_RDS_REG:
			puts("FUNC_RDS_REG\n");
			if(setup.rds.reg)
			{
				setup.rds.reg = 0;
				if(radio_rds_check())
				{
					ui_disp.func_sel = FUNC_RDS_REG;
					disp_func_sel_others(TRUE);
				}
			}
			break;
    #endif

	#if defined(_MR889_SPECAIL_KEY_)
		case FUNC_SEL_CONTROL_SPK:
			if(setup.CONTROL_SPK)
	        {
	        	setup.CONTROL_SPK = OFF;
	        }
			else
			{
				setup.CONTROL_SPK = ON;
			}
			control = TRUE;
			break;
	#endif
	
	#if defined(SUPPORT_ALARM_FUNC_)
		case FUNC_SEL_ALARM:
			if(setup.alarm)
	        {
	        	setup.alarm = OFF;
	        }
			else
			{
				setup.alarm = ON;
			}
			break;
	#endif

	#if defined(_ENABLE_TIME_DISP_)
    	case FUNC_SEL_TIME_DISP:
    		if(setup.time_disp)
    		{
    			setup.time_disp = OFF;
    			global.work_flag = TRUE;
    		}
    		else
    		{
    			setup.time_disp = ON;
    		}
    		break;
	#endif
	
	#if defined(_SUPPORT_CAN_UART_)
    	case FUNC_SEL_CAN_DISP:
			if(global.can.disp == 0)
			{
				global.can.disp = AUDIO_CAN_MAX;
			}
			if(global.can.disp > 0)
	        {
	            global.can.disp--;
	        }
    		break;
			
		case FUNC_SEL_MODE:
			if(setup.can_mode == 1)
			{
				setup.can_mode = 2;
			}
			else if(setup.can_mode == 2)
			{
				setup.can_mode = 1;
			}
			u8 mcu_send_buf1[8]={0x01,setup.can_mode,0x00,0x00,0x00,0x00,0x00,0x00};
			Mcu_Send_Data(mcu_send_buf1,8);
			break;
			
		case FUNC_SEL_SPEED:
			disp_func_tab_next(&setup.can_speed, disp_can_speed_sel_tab, sizeof(disp_can_speed_sel_tab), 0);
			u8 mcu_send_buf2[8]={0x01,setup.can_speed,0x00,0x00,0x00,0x00,0x00,0x00};
			Mcu_Send_Data(mcu_send_buf2,8);
			break;
	#endif
	
	#if defined(_ENABLE_MENU_FUNTION_)
		case FUNC_SEL_SETUP:
			if(setup.setup)
	        {
	        	setup.setup = OFF;
	        }
			else
			{
				setup.setup = ON;
			}
			break;
			
		case FUNC_SEL_DEFAULT_VOL:
			if(setup.default_vol > 5)
	        {
	            setup.default_vol--;
	        }
			break;
			
		case FUNC_SEL_LOG:
			if(setup.log)
	        {
	        	setup.log = OFF;
	        }
			else
			{
				setup.log = ON;
			}
			break;
	#endif
	
	#if defined(_ENABLE_DEFAULT_CONTROL_)
    	case FUNC_SEL_DEFAULT_VOL:
    		if(setup.default_vol > 5)
    		{
    			setup.default_vol--;
    		}
    		break;
	#endif
	
    #if defined(ADD_ADJUST_DEFAULT_VOL)
    	case FUNC_SEL_DEFAULT_VOL:
    		if(setup.default_vol > 0)
    		{
    		   setup.default_vol--;
    		}
    		break;
    #endif

		case FUNC_SEL_LOC:
		case FUNC_SEL_ST:
			{
				uint8 *com_ap;
				if((func_sel_check != NULL)&&((com_ap = (uint8 *)func_sel_check(ui_disp.func_sel)) != NULL))
				{
			    #if defined(_SL6812A_MENU_SET_)
			    	if(*com_ap)
			    	{
			    		*com_ap = FALSE;
			    	}
			    #else
			    	if(*com_ap)
			    	{
			    		*com_ap = FALSE;
			    	}
			    	else
			    	{
			    		*com_ap = TRUE;
			    	}
			    #endif
				}
			}
			break;
    #if defined(ENABLE_TA_VOL_SET)
		case FUNC_RDS_TA_VOL:
		#if defined(_FY7000G_SPECIAL_DISPLAY_)
			if(setup.rds.volume>0)setup.rds.volume--;
		#else
			if(setup.rds.volume>10)setup.rds.volume--;
		#endif
			break;
    #endif
		default:
		#if defined(_ENABLE_SET_FUNTION_)
    		if((!disp_func_sel_others(FALSE))||(!disp_func_sel_others1(FALSE)))
    		{
    			goto func_sel_sub_again;
    		}
		#else
			if(!disp_func_sel_others(FALSE))
			{
				goto func_sel_sub_again;
			}
		#endif	
			break;
	}

	if(control)
	{
		disp_func_sel_control();
	}

	disp_show_sel(ui_disp.func_sel);
}

#if defined(_YS_SEL_SETTING_)

#if 0
static void EqSyncSet(uint8 iSelMode)
{
	switch(iSelMode)
	{
		case AUDIO_EQ_FLAT:		//-2 0 0 2 2 0 0 0 6 4
	 		 setup.aud_bass = 0;
			 setup.aud_treble = 4;
			 break;
			 
		case AUDIO_EQ_CLASSIC:	//7 8 8 4 2 1 1 1 1 1 
	 		 setup.aud_bass = 5;
			 setup.aud_treble = 1;
			 break;
			 
		case AUDIO_EQ_POP:		// 1 1 1 4 2 2 0 1 1 1 
	 		 setup.aud_bass = 1;
			 setup.aud_treble = 1;
			 break;
			 
		case AUDIO_EQ_ROCK:		// 7 8 8 4 1 1 8 8 8 8
	 		 setup.aud_bass = 5;
			 setup.aud_treble = 5;
			 break;
			 
		case AUDIO_EQ_JAZZ:		// 0 0 0 4 4 4 0 2 3 4
	 		 setup.aud_bass = 0;
			 setup.aud_treble = 2;
			 break;	
		case AUDIO_EQ_OFF:		// 0 -3 0 0 0 0 0 0 -5 0 
	 		 setup.aud_bass =0;
			 setup.aud_treble = 0;
			 break;
	}		
}
#endif

static void EqSyncSet(uint8 iSelMode)
{
	switch(iSelMode)
	{
		case AUDIO_EQ_FLAT:		//-2 0 0 2 2 0 0 0 6 4
	 		 setup.aud_bass = 0;	//	0
			 setup.aud_treble = 0;	//	4
			 break;
			 
		case AUDIO_EQ_CLASSIC:	//7 8 8 4 2 1 1 1 1 1 
	 		 setup.aud_bass = 3;	//	5
			 setup.aud_treble = 3;	//	1
			 break;
			 
		case AUDIO_EQ_POP:		// 1 1 1 4 2 2 0 1 1 1 
	 		 setup.aud_bass = 3;	//	1
			 setup.aud_treble = 3;	//	1
			 break;
			 
		case AUDIO_EQ_ROCK:		// 7 8 8 4 1 1 8 8 8 8
	 		 setup.aud_bass = 5;	//	5
			 setup.aud_treble = 4;	//	5
			 break;
			 
		case AUDIO_EQ_JAZZ:		// 0 0 0 4 4 4 0 2 3 4
	 		 setup.aud_bass = 2;	//	0
			 setup.aud_treble = 3;	//	2
			 break;	
		case AUDIO_EQ_OFF:		// 0 -3 0 0 0 0 0 0 -5 0 
	 		 setup.aud_bass = 0;	// 0
			 setup.aud_treble = 0;	// 0
			 break;
	}		
}
#endif

static void disp_func_sel_add(void)
{
	uint8 control = FALSE;

func_sel_add_again:
	switch(ui_disp.func_sel)
	{
	#if !defined(_ENABLE_MENU_SET_)
	#if !defined(_DISABLE_SEL_VOLUME_)
		case FUNC_SEL_VOLUME:
			puts("FUNC_SEL_VOLUME\n");
			if(get_cur_vol() < AUDIO_VOLUME_MAX)
	        {
	        	set_cur_vol(get_cur_vol() + 1);
				control = TRUE;
	        }
			break;
	#endif
	#endif
		case FUNC_SEL_BASS:
			if(setup.aud_bass < AUDIO_BASS_MAX)
	        {
	            setup.aud_bass++;
				control = TRUE;
	        }
			break;
    #if defined(_ENABLE_7419_MIDDLE_)
    	case FUNC_SEL_MIDDLE:
    		if(setup.aud_middle < AUDIO_MIDDLE_MAX)
    		{
    			setup.aud_middle++;
    			control = TRUE;
    		}
    		break;
    #endif
		case FUNC_SEL_TREBLE:
			if(setup.aud_treble < AUDIO_TREBLE_MAX)
	        {
	            setup.aud_treble++;
				control = TRUE;
	        }
			break;
	#if !defined(_FAD_BAL_FUNTION_)
		case FUNC_SEL_BAL:
			if(setup.aud_bal < AUDIO_LR_MAX)
	        {
	            setup.aud_bal++;
				control = TRUE;
	        }
			break;
		case FUNC_SEL_FAD:
			if(setup.aud_fad < AUDIO_FR_MAX)
	        {
	            setup.aud_fad++;
				control = TRUE;
	        }
			break;
	#endif
		case FUNC_SEL_LOUD:
		#if defined(_SL6812A_MENU_SET_)
    		if(setup.aud_loud == AUDIO_LOUDNESS_OFF)
    		{
    			setup.aud_loud = AUDIO_LOUDNESS_ON;
    		}
		#else
			if(setup.aud_loud == AUDIO_LOUDNESS_OFF)
	        {
	        	setup.aud_loud = AUDIO_LOUDNESS_ON;
	        }
			else
			{
				setup.aud_loud = AUDIO_LOUDNESS_OFF;
			}
		#endif
			control = TRUE;
			break;
		case FUNC_SEL_EQ:
		#if defined(_SL6812A_MENU_SET_)
			if(setup.aud_eq == AUDIO_EQ_JAZZ)break;
		#endif
			disp_func_tab_next(&setup.aud_eq, disp_eq_sel_tab, sizeof(disp_eq_sel_tab), 1);
		#if defined(_YS_SEL_SETTING_)
			EqSyncSet(setup.aud_eq);
			ew_printf("setup.aud_eq:%d\n",setup.aud_eq);
		#endif			
			control = TRUE;
			break;
    #if defined(_ENABLE_RPT_RDM_FUNTION_)
    	case FUNC_SEL_RPT:
    		disp_func_tab_next(&setup.rpt_start, disp_rpt_sel_tab, sizeof(disp_rpt_sel_tab), 1);
    		if(setup.rpt_start)
    		{
    			setup.rdm_start = FALSE;
    			setup.rdm_start1 = FALSE;
    		}
    		break;
    
    	case FUNC_SEL_RDM:
    		if(setup.rdm_start)
    		{
    			setup.rdm_start = FALSE;
    		}
    		else
    		{
    			setup.rdm_start = TRUE;
    			setup.rpt_start = FALSE;
    		}
    		break;
    #endif
	#if defined(_SPECIAL_TL2000_SET_)
		case FUNC_SEL_A_VOLUME:
			if(setup.aud_A_volume < AUDIO_A_VOLUME_MAX)
	        {
	            setup.aud_A_volume++;
				control = TRUE;
	        }
			break;
		case FUNC_SEL_B_VOLUME:
			if(setup.aud_B_volume < AUDIO_B_VOLUME_MAX)
	        {
	            setup.aud_B_volume++;
				control = TRUE;
	        }
			break;
	#endif
	
	#if defined(_ENABLE_TALK_VOL_)
    	case FUNC_SEL_TALK_VOL:
    		if(setup.talk_volume < AUDIO_VOLUME_MAX)
    		{
    			setup.talk_volume++;
    		}
    		break;
	#endif
	
    #if defined(_ENABLE_BT_ON_OFF_)
    	case FUNC_SEL_BT_ON:
    		if(setup.bt_on)
    		{
    			setup.bt_on = FALSE;
    			user_send_cmd_prepare(USER_CTRL_DISCONNECTION_HCI,0,NULL);
    			os_time_dly(10);
    			user_send_cmd_prepare(USER_CTRL_DEL_ALL_REMOTE_INFO,0,NULL);
    			os_time_dly(10);
    			user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
    			user_send_cmd_prepare(USER_CTRL_WRITE_CONN_DISABLE,0,NULL);
    			setup.power_off_flag = TRUE;
    		}
    		else
    		{
    			setup.bt_on = TRUE;
    			user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
    			user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE,0,NULL);
    			recon_standby_bt();
    			setup.power_off_flag = FALSE;
    			setup.acc_off_flag = FALSE;
    		}
    		break;
    #endif

	#if defined(_CHANGE_DEF_VOL_)
		case FUNC_SEL_DEFVOLUME:
			if(setup.DefVol < AUDIO_VOLUME_MAX)
	        {
	            setup.DefVol++;
				control = TRUE;
	        }
			break;
	#endif
	
    #if defined(_SUPPORT_SUB_)
    	case FUNC_SEL_SUB:
    		if(setup.sub_start)
            {
            	setup.sub_start = OFF;
            }
    		else
    		{
    			setup.sub_start = ON;
    		}
    		break;
    #endif

    #if defined(_ENABLE_AP3768_SUBWOOFER_)
    	case FUNC_SEL_SUB:
        	if(setup.aud_sub == 0)
        	{
        		setup.aud_sub = 1;
        	}
        	else
        	{
        		setup.aud_sub = 0;
        	}
    		control = TRUE;
    		break;
    
    	case FUNC_SEL_SUB1:
        	if(setup.aud_sub1 == 0)
        	{
        		setup.aud_sub1 = 1;
        	}
        	else
        	{
        		setup.aud_sub1 = 0;
        	}
    		control = TRUE;
    		break;
    #endif

	#if defined(_SUPPORT_SUBWOOFER_)
		case FUNC_SEL_SUBWOOFER:
			if(setup.aud_subwoofer == AUDIO_SUBWOOFER_OFF)
	        {
	        	setup.aud_subwoofer = AUDIO_SUBWOOFER_ON;
	        }
			else
			{
				setup.aud_subwoofer = AUDIO_SUBWOOFER_OFF;
			}
			control = TRUE;
			break;
	#endif
    #if defined(_SUPPORT_SUBWOOFER_MODE2_)
    	case FUNC_SEL_SUBWOOFER:
    		if(setup.aud_subwoofer < AUDIO_SUB_VOL_MAX)
            {
                setup.aud_subwoofer++;
    			control = TRUE;
            }
    		break;
    #endif

    #if defined(AUDIO_SUB_LEVEL_SET)
    	case FUNC_SEL_SUB_CUT_CTRL:
    		if( setup.aud_sub_ctrl < AUDIO_SUBWOOFER_CUT_160)
            {
                setup.aud_sub_ctrl++;
    			control = TRUE;
            }
    		break;
    #endif
    #if defined(_SUPPORT_SUBWOOFER2_)
    	case FUNC_SEL_SUB:
        	if(setup.aud_sub == 0)
        	{
        		setup.aud_sub = 1;
			#if defined(_SUPPORT_AMP_CONTROL_)
				SYS_SUB_ON();
			#endif
        	}
        	else
        	{
        		setup.aud_sub = 0;
			#if defined(_SUPPORT_AMP_CONTROL_)
				SYS_SUB_OFF();
			#endif
        	}
        	break;
    #endif
    #if defined(_SUPPORT_SUBWOOFER3_)
    	case FUNC_SEL_SUB1:
        	if(setup.aud_sub1 == 0)
        	{
        		setup.aud_sub1 = 1;
			#if defined(_SUPPORT_AMP_CONTROL_)
				SYS_SUB2_ON();
			#endif
        	}
        	else
        	{
        		setup.aud_sub1 = 0;
		    #if defined(_SUPPORT_AMP_CONTROL_)
		    	SYS_SUB2_OFF();
		    #endif
        	}
        	break;
    #endif
	#if defined(_ENABLE_ID3_ON_OFF_)||defined(_ENABLE_ID3_SET_)
    	case FUNC_SEL_ID3:
    		if(setup.id3_on_flag)
            {
            	setup.id3_on_flag = OFF;
            }
    		else
    		{
    			setup.id3_on_flag = ON;
    		}
    		break;
	#endif
	
	#if defined(_FAD_BAL_FUNTION_)
    	case FUNC_SEL_FL:
    		if(setup.aud_fl< AUDIO_LR_MAX)
    		{
    			setup.aud_fl++;
    			control = TRUE;
    		}
    		break;
		case FUNC_SEL_FR:
			if(setup.aud_fr< AUDIO_LR_MAX)
			{
				setup.aud_fr++;
				control = TRUE;
			}
			break;
		case FUNC_SEL_RL:
			if(setup.aud_rl< AUDIO_LR_MAX)
			{
				setup.aud_rl++;
				control = TRUE;
			}
			break;
		case FUNC_SEL_RR:
			if(setup.aud_rr< AUDIO_LR_MAX)
			{
				setup.aud_rr++;
				control = TRUE;
			}
			break;
	#endif
	
    #if defined(_SUPPORT_10_EQ_FUNTION_)
    	case FUNC_SEL_60HZ:
    		if(setup.aud_60hz< AUDIO_60HZ_MAX)
    		{
    			setup.aud_60hz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_150HZ:
    		if(setup.aud_150hz < AUDIO_150HZ_MAX)
    		{
    			setup.aud_150hz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_400HZ:
    		if(setup.aud_400hz < AUDIO_400HZ_MAX)
    		{
    			setup.aud_400hz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_1KHZ:
    		if(setup.aud_1khz < AUDIO_1KHZ_MAX)
    		{
    			setup.aud_1khz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_2KHZ:
    		if(setup.aud_2khz < AUDIO_2KHZ_MAX)
    		{
    			setup.aud_2khz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_3KHZ:
    		if(setup.aud_3khz < AUDIO_3KHZ_MAX)
    		{
    			setup.aud_3khz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_7KHZ:
    		if(setup.aud_7khz < AUDIO_7KHZ_MAX)
    		{
    			setup.aud_7khz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_10KHZ:
    		if(setup.aud_10khz < AUDIO_10KHZ_MAX)
    		{
    			setup.aud_10khz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_15KHZ:
    		if(setup.aud_15khz < AUDIO_15KHZ_MAX)
    		{
    			setup.aud_15khz++;
    			control = TRUE;
    		}
    		break;
    	case FUNC_SEL_20KHZ:
    		if(setup.aud_20khz < AUDIO_20KHZ_MAX)
    		{
    			setup.aud_20khz++;
    			control = TRUE;
    		}
    		break;
    #endif
	#if defined(_ENABLE_BEEP_)
		case FUNC_SEL_BEEP:
			if(setup.aud_beep)
	        {
	        	setup.aud_beep = OFF;
	        }
			else
			{
				setup.aud_beep = ON;
			}
			control = TRUE;
			break;
	#endif
	#if defined(_SUPPORT_AREA_)
    	case FUNC_SEL_AREA:
			disp_func_tab_next(&setup.area, disp_area_sel_tab, sizeof(disp_area_sel_tab), 1);
			control = TRUE;
			break;
    #endif
    #if defined(_SEL_ADD_TIME_SELECT_)
    	case FUNC_SEL_CLOCK:
    		disp_func_tab_next(&setup.rtc_disp, disp_clock_sel_tab, sizeof(disp_clock_sel_tab), 1);
    		control = TRUE;
    		break;
    #endif
    #if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    	case FUNC_SEL_7COLOR:
    		disp_func_tab_next(&setup.State, disp_color_sel_tab, sizeof(disp_color_sel_tab), 1);
        #if defined(_ENABLE_CHRAMATIC_CAMP_SHADE_)
        	if(GET_PANEL_LIGHT_STATE == PANEL_LIGHT_STATE_AUTO)
        	{
        		SET_PANEL_AUTO_STATE(0);
        		SET_PANEL_LIGHT_STATE(PANEL_LIGHT_STATE_AUTO);
        		setup.Time = 0;
		    #if defined(_WLS316_PANEL_LIGHT_)
		    	GET_LED_COLOR_LEVER_R = 0;
		    	GET_LED_COLOR_LEVER_G = 0;
		    	GET_LED_COLOR_LEVER_B = 0;
		    #else
        		GET_LED_COLOR_LEVER_R = 0;
        		GET_LED_COLOR_LEVER_G = LED_COLOR_G_LEVER_MAX;
        		GET_LED_COLOR_LEVER_B = LED_COLOR_B_LEVER_MAX;
			#endif
        	}
        #endif
    		control = TRUE;
    		break;
    #endif
	
	#if defined(SUPPORT_LED_ADJUST)
    	case FUNC_SEL_LED:
    		disp_func_tab_next(&setup.LedState, disp_led_sel_tab, sizeof(disp_led_sel_tab), 1);
			break;
	#endif
	#if defined(_ADJUST_A_B_SPK_)
		case FUNC_SEL_A_SPK:
			if(setup.A_SPK)
	        {
	        	setup.A_SPK = OFF;
	        }
			else
			{
				setup.A_SPK = ON;
			}
			control = TRUE;
			break;

		case FUNC_SEL_B_SPK:
			if(setup.B_SPK)
	        {
	        	setup.B_SPK = OFF;
	        }
			else
			{
				setup.B_SPK = ON;
			}
			control = TRUE;
			break;
	#endif

	#if defined(_MR889_SPECAIL_KEY_)
		case FUNC_SEL_CONTROL_SPK:
			if(setup.CONTROL_SPK)
	        {
	        	setup.CONTROL_SPK = OFF;
	        }
			else
			{
				setup.CONTROL_SPK = ON;
			}
			control = TRUE;
			break;
	#endif
	
    #if defined(_ENABLE_RDS_ON_OFF_)
    	case FUNC_RDS_ONOFF:
		#if defined(_SL6812A_MENU_SET_)
    		if(!setup.rds_onoff)
    		{
    			setup.rds_onoff = 1;
    		}
		#else
    		if(setup.rds_onoff)
    		{
    			setup.rds_onoff = 0;
    		}
    		else
    		{
    			setup.rds_onoff = 1;
    		}
		#endif
			if(compare_task_name("FMRadioTask"))
			{
				cmd_radio_rds_onoff(setup.rds_onoff);
			}
			else
			{
				global.rds_flag = TRUE;
			}
    		break;
    #endif
	
	#if defined(_SL6812A_MENU_SET_)
		case FUNC_RDS_AF:
			puts("FUNC_RDS_AF\n");
			if(!setup.rds.af)
			{
				setup.rds.af = 1;
				if(radio_rds_check())
				{
					ui_disp.func_sel = FUNC_RDS_AF;
					disp_func_sel_others(TRUE);
				}
			}
    		break;
			
		case FUNC_RDS_TA:
			puts("FUNC_RDS_TA\n");
			if(!setup.rds.ta)
			{
				setup.rds.ta = 1;
				if(radio_rds_check())
				{
					ui_disp.func_sel = FUNC_RDS_TA;
					disp_func_sel_others(TRUE);
				}
			}
			break;
			
		case FUNC_RDS_REG:
			puts("FUNC_RDS_REG\n");
			if(!setup.rds.reg)
			{
				setup.rds.reg = 1;
				if(radio_rds_check())
				{
					ui_disp.func_sel = FUNC_RDS_REG;
					disp_func_sel_others(TRUE);
				}
			}
			break;
	#endif

	#if defined(SUPPORT_ALARM_FUNC_)
		case FUNC_SEL_ALARM:
			if(setup.alarm)
	        {
	        	setup.alarm = OFF;
	        }
			else
			{
				setup.alarm = ON;
			}
			break;
	#endif
	
    #if defined(_ENABLE_TIME_DISP_)
    	case FUNC_SEL_TIME_DISP:
    		if(setup.time_disp)
    		{
    			setup.time_disp = OFF;
    			global.work_flag = TRUE;
			}
    		else
    		{
    			setup.time_disp = ON;
    		}
    		break;
    #endif
	
    #if defined(_SUPPORT_CAN_UART_)
    	case FUNC_SEL_CAN_DISP:
			if(global.can.disp < AUDIO_CAN_MAX)
	        {
	            global.can.disp++;
	        }
			if(global.can.disp == AUDIO_CAN_MAX)
			{
				global.can.disp = AUDIO_CAN_ERR;
			}
    		break;
			
		case FUNC_SEL_MODE:
			if(setup.can_mode == 1)
			{
				setup.can_mode = 2;
			}
			else if(setup.can_mode == 2)
			{
				setup.can_mode = 1;
			}
			u8 mcu_send_buf1[8]={0x01,setup.can_mode,0x00,0x00,0x00,0x00,0x00,0x00};
			Mcu_Send_Data(mcu_send_buf1,8);
			break;
			
		case FUNC_SEL_SPEED:
			disp_func_tab_next(&setup.can_speed, disp_can_speed_sel_tab, sizeof(disp_can_speed_sel_tab), 1);
			u8 mcu_send_buf2[8]={0x01,setup.can_speed,0x00,0x00,0x00,0x00,0x00,0x00};
			Mcu_Send_Data(mcu_send_buf2,8);
			break;
    #endif
	
    #if defined(_ENABLE_MENU_FUNTION_)
    	case FUNC_SEL_SETUP:
    		if(setup.setup)
            {
            	setup.setup = OFF;
            }
    		else
    		{
    			setup.setup = ON;
    		}
    		break;
    		
    	case FUNC_SEL_DEFAULT_VOL:
    		if(setup.default_vol < 25)
            {
                setup.default_vol++;
            }
    		break;
    		
    	case FUNC_SEL_LOG:
    		if(setup.log)
            {
            	setup.log = OFF;
            }
    		else
    		{
    			setup.log = ON;
    		}
    		break;
    #endif

	#if defined(_ENABLE_DEFAULT_CONTROL_)
    	case FUNC_SEL_DEFAULT_VOL:
    		if(setup.default_vol < 25)
    		{
    			setup.default_vol++;
    		}
    		break;
	#endif
	
    #if defined(ADD_ADJUST_DEFAULT_VOL)
    	case FUNC_SEL_DEFAULT_VOL:
    		if(setup.default_vol < AUDIO_VOLUME_MAX)
    		{
    		   setup.default_vol++;
    		}
    		break;
    #endif

		case FUNC_SEL_LOC:
		case FUNC_SEL_ST:
			{
				uint8 *com_ap;
				if((func_sel_check != NULL)&&((com_ap = (uint8 *)func_sel_check(ui_disp.func_sel)) != NULL))
				{
				#if defined(_SL6812A_MENU_SET_)
    				if(!*com_ap)
    				{
    					*com_ap = TRUE;
    				}
				#else
					if(*com_ap)
					{
						*com_ap = FALSE;
					}
					else
					{
						*com_ap = TRUE;
					}
				#endif
				}
			}
			break;
    #if defined(ENABLE_TA_VOL_SET)
    	case FUNC_RDS_TA_VOL:
    		if(setup.rds.volume<40)setup.rds.volume++;
    		break;
    #endif
		default:
		#if defined(_ENABLE_SET_FUNTION_)
	    	if((!disp_func_sel_others(TRUE))||(!disp_func_sel_others1(TRUE)))
	    	{
	    		goto func_sel_add_again;
	    	}
	    #else	
			if(!disp_func_sel_others(TRUE))
			{
				goto func_sel_add_again;
			}
		#endif	
			break;
	}

	if(control)
	{
		disp_func_sel_control();
	}

	disp_show_sel(ui_disp.func_sel);
}

#if defined(_ENABLE_MENU_SET_)||defined(_DISABLE_SEL_VOLUME_)
static void disp_func_sub_vol(void)
{
#if defined(_ENABLE_MENU_SET_)
    global.menu_flag = FALSE;
#endif
	if(get_cur_vol() > 0)
	{
		set_cur_vol(get_cur_vol() - 1);
	}
	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
#if defined(_ENABLE_TALK_VOL_)
	if(bt_call_state())
	{
		disp_show_sel(FUNC_SEL_TALK_VOL);
	}
	else
	{
		disp_show_sel(FUNC_SEL_VOLUME);
	}
#else
	disp_show_sel(FUNC_SEL_VOLUME);
#endif
}
static void disp_func_add_vol(void)
{
#if defined(_ENABLE_MENU_SET_)
    global.menu_flag = FALSE;
#endif
	if(get_cur_vol() < AUDIO_VOLUME_MAX)
	{
		set_cur_vol(get_cur_vol() + 1);
	}
	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
#if defined(_ENABLE_TALK_VOL_)
	if(bt_call_state())
	{
		disp_show_sel(FUNC_SEL_TALK_VOL);
	}
	else
	{
		disp_show_sel(FUNC_SEL_VOLUME);
	}
#else
	disp_show_sel(FUNC_SEL_VOLUME);
#endif
}
#endif

#if defined(_SUPPORT_TWO_PT2313_)
static void disp_func_sel_sub1(void)
{
	if(get_cur_vol2() > 0)
	{
		set_cur_vol2(get_cur_vol2() - 1);
	}
	audio_drv_ctrl1(AUD_CTRL_VOLUME, get_cur_vol2(), TRUE);
	ui_show_sel1(FUNC_SEL_VOLUME);
}
static void disp_func_sel_add1(void)
{
	if(get_cur_vol2() < AUDIO_VOLUME_MAX)
	{
		set_cur_vol2(get_cur_vol2() + 1);
	}
	audio_drv_ctrl1(AUD_CTRL_VOLUME, get_cur_vol2(), TRUE);
	ui_show_sel1(FUNC_SEL_VOLUME);
}
#endif

#if defined(_SINGLE_MENU_FUNCTION_)
static void disp_func_sel_sel(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	disp_func_sel_next();
}

static void disp_func_sel_func(void)
{
	ui_disp.func_type = FUNC_TYPE_FUNC;
	disp_func_sel_next();
}
#else
static void disp_func_sel_sel(void)
{
	puts("disp_func_sel_sel\n");
	disp_func_sel_next();
#if defined(_DISABLE_SEL_VOLUME_)
	global.sel_flag = TRUE;
#endif
}

#if defined(SINGLE_MENU_FUNCTION)
static void disp_func_sel_func(void)
{
	ui_disp.func_type = FUNC_TYPE_FUNC;
	disp_func_sel_next();
}
#else
static void disp_func_sel_func(void)
{
	func_type_e func_type;

	func_type = ui_disp.func_type;
	
	puts("disp_func_sel_func\n");

#if defined(_DISABLE_SEL_VOLUME_)
	global.sel_flag = TRUE;
#endif
	if(ui_disp.func_type != FUNC_TYPE_FUNC)
	{
		ui_disp.func_type = FUNC_TYPE_FUNC;
		if(!disp_func_sel_next())
		{
			ui_disp.func_type = func_type;
		}
	}
}
#endif
#endif

#if defined(_ENABLE_SET_FUNTION_)
static void disp_func_sel_sel1(void)
{
	disp_func_sel_next1();
}

static void disp_func_sel_func1(void)
{
	func_type_e func_type;

	func_type = ui_disp.func_type;

	if(ui_disp.func_type != FUNC_TYPE_FUNC)
	{
		ui_disp.func_type = FUNC_TYPE_FUNC;
		if(!disp_func_sel_next1())
		{
			ui_disp.func_type = func_type;
		}
	}
}
#endif

void disp_func_reset(void)
{
	ui_disp.now_level 	= ui_disp.front_level;
	ui_disp.timer 		= 0;
	ui_disp.func_type 	= FUNC_TYPE_SEL;
	ui_disp.func_sel 	= FUNC_SEL_NULL;
}

static void disp_func_volume(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_VOLUME;
	disp_show_sel(ui_disp.func_sel);
	ui_disp.func_sel = FUNC_SEL_NULL;
#if defined(_USER_TFT_SEND_)
	//TFT_Tx_System_Status();
	//OSTimeDly(10);
	TFT_Tx_Sel(ui_disp.func_sel);
	OSTimeDly(10);
	TFT_Tx_Audio();
	OSTimeDly(10);
#endif
}

static void disp_func_sel_eq(void)
{
    ui_disp.func_type = FUNC_TYPE_SEL;
    ui_disp.func_sel = FUNC_SEL_EQ;
    disp_func_sel_add();
    ui_disp.func_sel = FUNC_SEL_NULL;
#if defined(_USER_TFT_SEND_)
	//TFT_Tx_System_Status();
	//OSTimeDly(10);
	TFT_Tx_Sel(ui_disp.func_sel);
	OSTimeDly(10);
	TFT_Tx_Audio();
	OSTimeDly(10);
#endif
}

#if defined(_SPECIAL_TL2000_SET_)		
static void disp_func_a_add_volume(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_A_VOLUME;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
static void disp_func_a_sub_volume(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_A_VOLUME;
	disp_func_sel_sub();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
static void disp_func_b_add_volume(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_B_VOLUME;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
static void disp_func_b_sub_volume(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_B_VOLUME;
	disp_func_sel_sub();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
uint8 msg_vol_check(void)
{
	if((ui_disp.func_sel == FUNC_SEL_BASS)||(ui_disp.func_sel == FUNC_SEL_TREBLE)||(ui_disp.func_sel == FUNC_SEL_BAL)
	||(ui_disp.func_sel == FUNC_SEL_LOUD)||(ui_disp.func_sel == FUNC_SEL_EQ)||(ui_disp.func_sel == FUNC_SEL_AREA)
	||(ui_disp.func_sel == FUNC_SEL_ST)||(ui_disp.func_sel == FUNC_SEL_ALARM)||(ui_disp.func_sel == FUNC_SEL_TIME_DISP))
	{
		return FALSE;
	}

	return TRUE;
}
#endif

static void disp_func_sel_loud(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_LOUD;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
#if defined(_USER_TFT_SEND_)
	//TFT_Tx_System_Status();
	//OSTimeDly(10);
	TFT_Tx_Sel(ui_disp.func_sel);
	OSTimeDly(10);
	TFT_Tx_Audio();
	OSTimeDly(10);
#endif
}

#if defined(_ENABLE_TEMP_PWM_)
extern void PWM0_init(uint8 duty);
static void disp_func_sel_temp(void)
{
	disp_func_tab_next(&setup.temp_set, disp_func_temp_tab, sizeof(disp_func_temp_tab), 1);
	PWM0_init((setup.temp_set+1));
}
static void disp_func_sel_warm(void)
{
	disp_func_tab_next(&setup.warm_set, disp_func_warm_tab, sizeof(disp_func_warm_tab), 1);
	
	switch(setup.warm_set)
	{
		case FUNC_SEL_WARM_30:
			setup.warm_time = 1859;//30X60半小时
			break;
		case FUNC_SEL_WARM_60:
			setup.warm_time = 3659;//60X60一小时
			break;
		case FUNC_SEL_WARM_ON:
			setup.warm_time = 43200;//3600X12//12小时
			break;
		default:
			break;
	}
}
#endif

static void disp_func_sel_mute(void)
{
#if defined(_CHANGE_MUTE_DISP_) 
    if(global.mute_disp)
    {
       ui_show_mute(global.aud_mute);
	   global.mute_disp =0;
	}
	else
	{
	  global.aud_mute = !global.aud_mute;
	}
#else	
	global.aud_mute = !global.aud_mute;
#endif

#if defined (Tuobu_B324)|| defined(_SUPPORT_LCM_1618_DRIVE)
#else
	ui_show_mute(global.aud_mute);
#endif
#if defined(_USER_TFT_SEND_)
	TFT_Tx_Audio();
	OSTimeDly(10);
#endif
#if defined(TD_ADD_NEW_APP)
    extern void new_app_send_vol(void);
	new_app_send_vol();
#endif
	disp_func_mute_control();
#if defined(_SUPPORT_VIDEO_)
	video_show_sel(FUNC_SEL_MUTE);
#endif

	ew_puts("disp_func_sel_mute.....\n");
}

static void disp_func_inside_mute(uint8 mute)
{
	inside_mute_state = mute;
	disp_func_mute_control();
}

#if defined(_POWER_SET_COLOR_)||defined(_ENABLE_SET_COLOR_)
static void disp_func_sel_light(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_7COLOR;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
#endif

#if defined(_ENABLE_ID3_SET_)
static void disp_func_sel_id3(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_ID3;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
#endif

#if defined(SUPPORT_LED_ADJUST)
static void disp_func_sel_led(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_LED;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
#endif

#if defined(_ADJUST_A_B_SPK_)
static void disp_func_sel_a_spk(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_A_SPK;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
static void disp_func_sel_b_spk(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_B_SPK;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
#endif

#if defined(_MR889_SPECAIL_KEY_)
static void disp_func_sel_control_spk(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_CONTROL_SPK;
	disp_func_sel_add();
	ui_disp.func_sel = FUNC_SEL_NULL;
}
#endif

#if defined(SUPPORT_ALARM_FUNC_)
static void disp_func_sel_alarm(void)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	ui_disp.func_sel = FUNC_SEL_ALARM;
	//disp_func_sel_add();
	disp_show_sel(ui_disp.func_sel);
	ui_disp.func_sel = FUNC_SEL_NULL;
	
}
#endif

#if defined(_ENABLE_DLASHING_LIGHT_)
#if defined(_ENABLE_PAUSE_LIGHT_)
void dlashine_liaht_polling(void)
{
	if(global.sys_power)
	{
		if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard"))
		||(compare_task_name("BtUserTask")))
		{
			if(global.pause_flg)
			{
				if(++global.light_time <= (1000/DISP_POLLING_TIME))
				{
					SYS_DLASHING_ON();
				}
				else if((++global.light_time <= (2500/DISP_POLLING_TIME))&&(++global.light_time > (1000/DISP_POLLING_TIME)))
				{
					SYS_DLASHING_OFF();
				}
				else
				{
					global.light_time = 0;
				}
			}
			else
			{
				SYS_DLASHING_ON();
				global.light_time = 0;
			}
		}
		else
		{
			SYS_DLASHING_ON();
			global.light_time = 0;
		}
	}
	else
	{
		SYS_DLASHING_OFF();
		global.light_time = 0;
	}
}
#else
void dlashine_liaht_polling(void)
{
	if(global.sys_power)
	{
		if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
		{
			if(++global.light_time <= (1000/DISP_POLLING_TIME))
			{
				SYS_DLASHING_ON();
			}
			else if((++global.light_time <= (2500/DISP_POLLING_TIME))&&(++global.light_time > (1000/DISP_POLLING_TIME)))
			{
				SYS_DLASHING_OFF();
			}
			else
			{
				global.light_time = 0;
			}
		}
		else
		{
			SYS_DLASHING_ON();
			global.light_time = 0;
		}
	}
	else
	{
		SYS_DLASHING_OFF();
		global.light_time = 0;
	}
}
#endif
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_PASSWORD_INPUT_)

#define PASSWORD_SIZE			4
uint8 power_password[PASSWORD_SIZE];
static uint8 user_password[PASSWORD_SIZE] = "1234";

void password_disp_polling(void)
{
	uint8 dispchar[8];
	
	if(setup.password)
	{
		ew_puts("password_disp_polling\n");
		
		memset(dispchar, ' ', sizeof(dispchar));
		dispchar[0] = 'P';
		dispchar[1] = 'W';
		dispchar[2] = 'D';
		dispchar[3] = ' ';
		
		setup.password_input = (setup.password_input*10)%10000;
		setup.password_input = setup.password_input + setup.password_number;
		
		ew_printf("setup.password_number:%d\n",setup.password_number);
		ew_printf("setup.password_input:%d\n",setup.password_input);
				
		if(setup.password_input == 0)
		{
			dispchar[4] = '-';
			dispchar[5] = '-';
			dispchar[6] = '-';
			dispchar[7] = '-';
		}
		else if((setup.password_input < 10)&&(setup.password_input > 0))
		{
			dispchar[4] = setup.password_input + '0';
			dispchar[5] = '-';
			dispchar[6] = '-';
			dispchar[7] = '-';
		}
		else if((setup.password_input < 100)&&(setup.password_input > 10))
		{
			dispchar[4] = setup.password_input/10 + '0';
			dispchar[5] = (setup.password_input%10)%10 + '0';
			dispchar[6] = '-';
			dispchar[7] = '-';
		}
		else if((setup.password_input < 1000)&&(setup.password_input > 100))
		{
			dispchar[4] = setup.password_input/100 + '0';
			dispchar[5] = (setup.password_input%100)/10 + '0';
			dispchar[6] = (setup.password_input%100)%10 + '0';
			dispchar[7] = '-';
		}
		else
		{				
			dispchar[4] = (setup.password_input/100)/10 + '0';
			dispchar[5] = (setup.password_input/100)%10 + '0';
			dispchar[6] = (setup.password_input%100)/10 + '0';
			dispchar[7] = (setup.password_input%100)%10 + '0';
		}
		ui_show_num(DISP_LEVEL_SETTING, 0xFF);
    	show_clear_icon(DISP_LEVEL_SETTING);
		ui_show_string(DISP_LEVEL_SETTING, dispchar);
		ui_disp.now_level = DISP_LEVEL_SETTING;
		ui_disp.timer = 4000;
	}
}

void password_input(pwd_number number)
{
	uint8 index;
	uint8 password_number;
	
	ew_puts("password_input\n");
	password_number = number + '0';
	setup.password_number = number;
	
	for(index = 0; index < (PASSWORD_SIZE - 1); index++)
	{
		power_password[index] = power_password[index + 1];
	}
	
	power_password[index] = password_number;
	password_disp_polling();
}

void password_control(void)
{
	ew_puts("password_control\n");
	ui_show_setting(LANG_ID_CAN, 0);
	ui_show_normal(LANG_ID_CAN);
}

void password_control_number1(void)
{
	ew_puts("password_control_number1\n");
	password_input(PWD_NUMBER_1);
}

void password_control_number2(void)
{
	ew_puts("password_control_number2\n");
	password_input(PWD_NUMBER_2);
}

void password_control_number3(void)
{
	ew_puts("password_control_number3\n");
	password_input(PWD_NUMBER_3);
}

void password_control_number4(void)
{
	ew_puts("password_control_number4\n");
 	password_input(PWD_NUMBER_4);
}

void password_control_enter(void)
{
	setup.password_input = 0;
	setup.password_number = 0;
	if(!memcmp(power_password, user_password, PASSWORD_SIZE))
	{
		memset(power_password, 0, sizeof(power_password));
		disp_send_msg(DISP_EVENT_PASSWORD);
		setup.password = FALSE;
	}
	else
	{
		ui_show_setting(LANG_ID_ERROR, 0);
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_RADIO_RDS_)
static void msg_rds_af(void)
{
#if !defined(_SL6812A_MENU_SET_)
	if(radio_rds_check())
	{
		ui_disp.func_type = FUNC_TYPE_SEL;
		ui_disp.func_sel = FUNC_RDS_AF;
		disp_func_sel_add();
		ui_disp.func_sel = FUNC_SEL_NULL;
	}
#endif
}

static void msg_rds_ta(void)
{
#if !defined(_SL6812A_MENU_SET_)
	if(radio_rds_check())
	{
		ui_disp.func_type = FUNC_TYPE_SEL;
		ui_disp.func_sel = FUNC_RDS_TA;
		disp_func_sel_add();
		ui_disp.func_sel = FUNC_SEL_NULL;
	}
#endif
}

static void msg_rds_eon(void)
{
	if(radio_rds_check())
	{
		ui_disp.func_type = FUNC_TYPE_SEL;
		ui_disp.func_sel = FUNC_RDS_EON;
		disp_func_sel_add();
		ui_disp.func_sel = FUNC_SEL_NULL;
	}
}

static void msg_rds_reg(void)
{
#if !defined(_SL6812A_MENU_SET_)
	if(radio_rds_check())
	{
		ui_disp.func_type = FUNC_TYPE_SEL;
		ui_disp.func_sel = FUNC_RDS_REG;
		disp_func_sel_add();
		ui_disp.func_sel = FUNC_SEL_NULL;
	}
#endif
}
#endif

#if defined(_ENABLE_VBAR_)
#if defined(vbar_disp_bt_music)
extern u8 vbar_bt_music ;
#endif

#if defined(_SUPPORT_REAL_VBAR_)
uint8 GetAverageVolume(void)
{
	uint8  Vol;

	//Vol = ad_vbar_scan();
	Vol = GET_VBAR_DATA;
	//ew_printf("Vol:%d\n",Vol);

	if(Vol >= VBAR_DC)
	{
		Vol -= VBAR_DC;
	#if	  AUDIO_BAR_VALUE_TYPE == AUDIO_AMP_ATTENUATION_OCTUPLE
		Vol>>=3;
	#elif AUDIO_BAR_VALUE_TYPE == AUDIO_AMP_ATTENUATION_FOURFOLD
		Vol>>=2;
	#elif AUDIO_BAR_VALUE_TYPE == AUDIO_AMP_ATTENUATION_DOUBLENESS
		Vol>>=1;
	#elif AUDIO_BAR_VALUE_TYPE == AUDIO_AMP_ZERO

	#elif AUDIO_BAR_VALUE_TYPE == AUDIO_PLUS_DOUBLENESS
		Vol<<=1;
	#elif AUDIO_BAR_VALUE_TYPE == AUDIO_PLUS_FOURFOLD
		Vol<<=2;
	#elif AUDIO_BAR_VALUE_TYPE == AUDIO_PLUS_OCTUPLE
		Vol<<=3;
	#endif
	}
	else Vol = 0;

	return Vol;
}

static void sys_vbar_polling(uint8 disp_type)
{
	if((disp_type != DISP_TYPE_IDLE) && (disp_type < DISP_TYPE_MAX))
	{
		uint8 vol_val = 0;

		if(mute_state || global.aud_mute || (setup.aud_vol == 0))
		{
			vol_val = 0;
		}
		else
		{
		 	vol_val = GetAverageVolume();
		#if defined(VBAR_REDUCE)
			vol_val /= VBAR_REDUCE;
		#else
			vol_val /= 5;
		#endif
		}
		//ew_printf("vol_val:%d\n",vol_val);
		ui_show_vbar(ENABLE, vol_val);
	}
}

#elif defined(_SPECIAL_VBAR_)

static const int8 VolTab[][2]=
{
	{1,2},
	{2,2},
	{4,2},
	{6,2},
	{2,5},
	{5,2},
	{4,2},
	{7,2},
	{8,2},
	{7,2},
	{9,2},
	{4,5},
	{1,5},
};

uint8 GetAverageVolume(void)
{
	static uint8 Vol;
	static uint8 Count;

	if(Count)
	{
		Count --;
	}
	else
	{
		if(++Vol >= (sizeof(VolTab)/2))
		{
			Vol = 0;
		}
		Count = VolTab[Vol][1];
	}
	return VolTab[Vol][0];
}

static void sys_vbar_polling(uint8 disp_type)
{
	static uint8 SimuVbarStep;
	static uint8 VbarVolume;

	if((disp_type != DISP_TYPE_IDLE) && (disp_type < DISP_TYPE_MAX))
	{
		uint8 vol_val = 0;

		if(mute_state || global.aud_mute || (setup.aud_vol == 0))
		{
			vol_val = 0;
		}
		else
		{
			SimuVbarStep++;
			vol_val=GetAverageVolume();
			if(vol_val > 7)vol_val = 7;//DISP_VBAR_SIZE
		}

		if(vol_val<VbarVolume)
		{
			vol_val=VbarVolume-1;
		}
		VbarVolume = vol_val;

	#if defined(_ENABLE_VBAR_BLINK_)
		global.vbar_time++;
		if(global.vbar_time <= (1550/DISP_POLLING_TIME))
		{
			ui_show_vbar(ENABLE, 0);
		}
		else
		{
			if(global.vbar_time == 100)
			{
				global.vbar_time = 50;
			}
			ui_show_vbar(ENABLE, vol_val);
		}
	#else
		ui_show_vbar(ENABLE, vol_val);
	#endif
	}
}

#else
static void sys_vbar_polling(uint8 disp_type)
{
	if((disp_type != DISP_TYPE_IDLE) && (disp_type < DISP_TYPE_MAX))
	{
		uint8 vol_val = 0;
		if(mute_state || global.aud_mute || (setup.aud_vol == 0)
	#if defined(vbar_disp_bt_music)
		||vbar_bt_music
	#endif
		)
		{
        #if defined(_ENABLE_POWER_ICON_)
        	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POWER);
        #endif
			vol_val = 0;
		}
		//else if((disp_type == DISP_TYPE_RADIO)
		//	  ||(disp_type == DISP_TYPE_AUX))
		else
		{
			static uint8 SimuVbarStep;
			static const uint8 SumiVbarTab[] =
			{
				0,0,3,2,4,5,6,7,8,9,3,2,1,4,2,3,1,0,0,9,5,6,2,3,4,7,
				5,3,1,0,2,0,2,0,8,9,4,2,1,7,2,3,1,5,0,9,0,0,2,3,4,7,
				0,0,0,2,1,0,3,1,8,9,0,0,0,2,1,0,0,1,0,9,3,4,6,3,4,7,
			};
			if(++SimuVbarStep > sizeof(SumiVbarTab)-1)
			{
				SimuVbarStep = 0;
			}
			vol_val = SumiVbarTab[SimuVbarStep];
	    #if defined(_ENABLE_POWER_ICON_)
	    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER);
	    #endif
		}
		/*else if((disp_type == DISP_TYPE_USB)
			  ||(disp_type == DISP_TYPE_SD)
			  ||(disp_type == DISP_TYPE_BT))
		{
			uint32 adc_val = get_dac_energy_value();
			vol_val = _u32to8hl_(adc_val);
			vol_val /= 5;
		}*/

		ui_show_vbar(ENABLE, vol_val);
	}
}
#endif
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

static void sys_mute_polling(void)
{
	if(mute_enable)
	{
		if(mute_time)
		{
			if(mute_time > DISP_POLLING_TIME)
			{
				mute_time -= DISP_POLLING_TIME;
			}
			else
			{
				mute_time = 0;
				goto mute_ctrl_disable;
			}
		}
		else
		{
		mute_ctrl_disable:
			mute_state  = DISABLE;
			mute_enable = DISABLE;
			disp_func_mute_control();
		}
	}
}

void sys_mute_ctrl(uint8 mute, uint16 timer)
{
	//ew_printf("sys_mute_ctrl  :%d  %d\n",mute,timer);
	if(mute)
	{
		mute_state = ENABLE;
		if(timer)
		{
			mute_enable = ENABLE;
			mute_time = timer;

		#if defined(_SUPPORT_AMP_CONTROL_)
			if(global.number_flag)
	    #endif
    		{
    		#if !defined(_MUTE_ON_CLOSE_AMP_)
    			AUDIO_MUTE_ON();
		    #if defined(_ENABLE_RCA_MUTE_)
		    	RCA_MUTE_ON();
		    #endif
			#endif
    		}		
		}
		else
		{
			mute_enable = DISABLE;
		}

		disp_func_mute_control();
	}
	else
	{
		if(timer)
		{
			mute_enable = ENABLE;
			mute_time = timer;
		}
		else
		{
			mute_state = DISABLE;
			mute_enable = FALSE;
			disp_func_mute_control();
		}
	}
}
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_SUPPORT_PANEL_LIGHT_)
#if defined(_SUPPORT_PANEL_PWM_)
static void panel_light_polling(void)
{
	switch(GET_PANEL_LIGHT_STATE)
	{
		case PANEL_LIGHT_STATE_RED:
			set_panel_pwm(LED_COLOR_LEVER_MAX, 0, 0);
			break;

		case PANEL_LIGHT_STATE_GREEN:
			set_panel_pwm(0, LED_COLOR_LEVER_MAX, 0);
			break;

		case PANEL_LIGHT_STATE_BLUE:
			set_panel_pwm(0, 0, LED_COLOR_LEVER_MAX);
			break;

		case PANEL_LIGHT_STATE_YELLOW:
			set_panel_pwm(LED_COLOR_LEVER_MAX, LED_COLOR_LEVER_MAX, 0);
			break;

		case PANEL_LIGHT_STATE_FUCHSINE:
			set_panel_pwm(LED_COLOR_LEVER_MAX, 0, LED_COLOR_LEVER_MAX);
			break;

		case PANEL_LIGHT_STATE_CYAN:
			set_panel_pwm(0, LED_COLOR_LEVER_MAX, LED_COLOR_LEVER_MAX);
			break;

		case PANEL_LIGHT_STATE_WHITE:
			set_panel_pwm(LED_COLOR_LEVER_MAX, LED_COLOR_LEVER_MAX, LED_COLOR_LEVER_MAX);
			break;
		case PANEL_LIGHT_STATE_AUTO:
			if(setup.pwm_speed < LED_COLOR_SPEED/PANEL_LIGHT_FLASH_POLLIGN_TIME)
			{
				setup.pwm_speed++;
			}
			else
			{
				setup.pwm_speed = 0;
				switch(GET_PANEL_AUTO_STATE)
				{
    				case 0:
						if(GET_LED_COLOR_TIME)
						{
							GET_LED_COLOR_TIME--;
						}
						else
						{
							GET_LED_COLOR_TIME = 0;
							setup.pwm_g++;
						}
						if(setup.pwm_g >= LED_COLOR_LEVER_MAX)
						{
							GET_PANEL_AUTO_STATE++;
						}
						break;
    				case 1:
						setup.pwm_r--;
						if(!setup.pwm_r)
						{
							GET_PANEL_AUTO_STATE++;
						}
    					break;
					case 2:
						if(++setup.pwm_b >= LED_COLOR_LEVER_MAX)
						{
							GET_PANEL_AUTO_STATE++;
						}
						break;
					case 3:
						setup.pwm_g--;
						if(!setup.pwm_g)
						{
							GET_PANEL_AUTO_STATE++;
						}
						break;
					case 4:
						if(++setup.pwm_r >= LED_COLOR_LEVER_MAX)
						{
							GET_PANEL_AUTO_STATE++;
						}
						break;
					case 5:
						if(++setup.pwm_g >= LED_COLOR_LEVER_MAX)
						{
							GET_PANEL_AUTO_STATE++;
						}
						break;
					case 6:
						setup.pwm_g--;
						if(!setup.pwm_g)
						{
							GET_PANEL_AUTO_STATE++;
						}
						break;
					case 7:
						setup.pwm_b--;
						if(!setup.pwm_b)
						{
							SET_LED_COLOR_TIME(17);
							GET_PANEL_AUTO_STATE = 0;
						}
						break;
					default:
						GET_PANEL_AUTO_STATE = 0;
						break;
				}
				set_panel_pwm(setup.pwm_r, setup.pwm_g, setup.pwm_b);
			}
			break;
	}
}

#elif defined(_ENABLE_CHRAMATIC_CAMP_SHADE_)
extern void Color6803Refresh(uint16 RData, uint16 GData, uint16 BData);

static void panel_light_polling(void)
{
	switch(GET_PANEL_LIGHT_STATE)
	{
		case PANEL_LIGHT_STATE_GREEN:
			SET_LED_COLOR_LEVER_R(LED_COLOR_R_LEVER_MAX);
			SET_LED_COLOR_LEVER_G(0);
			SET_LED_COLOR_LEVER_B(LED_COLOR_B_LEVER_MAX);
			break;
		case PANEL_LIGHT_STATE_BLUE:
			SET_LED_COLOR_LEVER_R(LED_COLOR_R_LEVER_MAX);
			SET_LED_COLOR_LEVER_G(LED_COLOR_G_LEVER_MAX);
			SET_LED_COLOR_LEVER_B(0);
			break;
		case PANEL_LIGHT_STATE_RED:
			SET_LED_COLOR_LEVER_R(0);
			SET_LED_COLOR_LEVER_G(LED_COLOR_G_LEVER_MAX);
			SET_LED_COLOR_LEVER_B(LED_COLOR_B_LEVER_MAX);
			break;
		case PANEL_LIGHT_STATE_YELLOW:
			SET_LED_COLOR_LEVER_R(0);
			SET_LED_COLOR_LEVER_G(0);
			SET_LED_COLOR_LEVER_B(LED_COLOR_B_LEVER_MAX);
			break;
		case PANEL_LIGHT_STATE_CYAN:
			SET_LED_COLOR_LEVER_R(LED_COLOR_R_LEVER_MAX);
			SET_LED_COLOR_LEVER_G(0);
			SET_LED_COLOR_LEVER_B(0);
			break;
		case PANEL_LIGHT_STATE_FUCHSINE:
			SET_LED_COLOR_LEVER_R(0);
			SET_LED_COLOR_LEVER_G(LED_COLOR_G_LEVER_MAX);
			SET_LED_COLOR_LEVER_B(0);
			break;
		case PANEL_LIGHT_STATE_WHITE:
			SET_LED_COLOR_LEVER_R(0);
			SET_LED_COLOR_LEVER_G(0);
			SET_LED_COLOR_LEVER_B(0);
			break;
		case PANEL_LIGHT_STATE_AUTO:

			 if (GET_LED_COLOR_SPEED < LED_COLOR_SPEED/PANEL_LIGHT_FLASH_POLLIGN_TIME)
			 {
				 GET_LED_COLOR_SPEED++;
			 }
			 else
			 {
				 GET_LED_COLOR_SPEED = 0;
			#if defined(_WLS316_PANEL_LIGHT_)
    			switch (GET_PANEL_AUTO_STATE)
    			{
    				case 0:
    					if(GET_LED_COLOR_TIME)
    					{
    						GET_LED_COLOR_TIME --;
    					}
    					else
    					{
    						GET_LED_COLOR_TIME = 0;
    						GET_LED_COLOR_LEVER_R++;
    					}
    			
    					if(GET_LED_COLOR_LEVER_R >= LED_COLOR_R_LEVER_MAX)
    					{
    						GET_PANEL_AUTO_STATE++;
    					}
    					break;
    				case 1:
    					GET_LED_COLOR_LEVER_G++;
    					if(GET_LED_COLOR_LEVER_G >= LED_COLOR_G_LEVER_MAX)
    					{
    						GET_PANEL_AUTO_STATE++;
    					}
    					break;
    				case 2:
    					GET_LED_COLOR_LEVER_R--;
    					if(!GET_LED_COLOR_LEVER_R)
    					{
    						GET_PANEL_AUTO_STATE++;
    					}
    					break;
    				case 3:
    					GET_LED_COLOR_LEVER_B++;
    					if(GET_LED_COLOR_LEVER_B >= LED_COLOR_B_LEVER_MAX)
    					{
    						GET_PANEL_AUTO_STATE++;
    					}
    					break;
    				case 4:
    					GET_LED_COLOR_LEVER_G--;
    					if(!GET_LED_COLOR_LEVER_G)
    					{
    						GET_PANEL_AUTO_STATE++;
    					}
    					break;
    				case 5:
    					GET_LED_COLOR_LEVER_R++;
    					if(GET_LED_COLOR_LEVER_R >= LED_COLOR_R_LEVER_MAX)
    					{
    						GET_PANEL_AUTO_STATE++;
    					}
    					break;
    				case 6:
    					GET_LED_COLOR_LEVER_B--;
    					if(!GET_LED_COLOR_LEVER_B)
    					{
    						GET_PANEL_AUTO_STATE = 1;
    					}
    					break;
    				default :
    					GET_PANEL_AUTO_STATE = 0;
    					break;
    			}

			#else
			
				 switch (GET_PANEL_AUTO_STATE)
				 {
					 case 0:
						 if(GET_LED_COLOR_TIME)
						 {
							 GET_LED_COLOR_TIME --;
						 }
						 else
						 {
							 GET_LED_COLOR_TIME = 0;
							 GET_LED_COLOR_LEVER_G--;
						 }

						 if(!GET_LED_COLOR_LEVER_G)
						 {
							 GET_PANEL_AUTO_STATE++;
						 }
						 break;
					 case 1:
						 GET_LED_COLOR_LEVER_R++;
						 if(GET_LED_COLOR_LEVER_R >= LED_COLOR_R_LEVER_MAX)
						 {
							 GET_PANEL_AUTO_STATE++;
						 }
						 break;
					 case 2:
						 GET_LED_COLOR_LEVER_B--;
						 if(!GET_LED_COLOR_LEVER_B)
						 {
							 GET_PANEL_AUTO_STATE++;
						 }
						 break;
					 case 3:
						 GET_LED_COLOR_LEVER_G++;
						 if(GET_LED_COLOR_LEVER_G >= LED_COLOR_G_LEVER_MAX)
						 {
							 GET_PANEL_AUTO_STATE++;
						 }
						 break;
					 case 4:
						 GET_LED_COLOR_LEVER_R--;
						 if(!GET_LED_COLOR_LEVER_R)
						 {
							 GET_PANEL_AUTO_STATE++;
						 }
						 break;
					 case 5:
						 GET_LED_COLOR_LEVER_G--;
						 if(!GET_LED_COLOR_LEVER_G)
						 {
							 GET_PANEL_AUTO_STATE++;
						 }
						 break;
					 case 6:
						 GET_LED_COLOR_LEVER_G++;
						 if(GET_LED_COLOR_LEVER_G >= LED_COLOR_G_LEVER_MAX)
						 {
							 GET_PANEL_AUTO_STATE++;
						 }
						 break;
					 case 7:
						 GET_LED_COLOR_LEVER_B++;
						 if(GET_LED_COLOR_LEVER_B >= LED_COLOR_B_LEVER_MAX)
						 {
							 SET_LED_COLOR_TIME(10);
							 //SET_LED_COLOR_TIME(17);
							 GET_PANEL_AUTO_STATE = 0;
						 }
						 break;
					 default :
						 GET_PANEL_AUTO_STATE = 0;
						 break;
				 }
			 #endif
				 break;
			 }
		case PANEL_LIGHT_STATE_END:
			break;
	}
	Color6803Refresh(GET_LED_COLOR_LEVER_R, GET_LED_COLOR_LEVER_G, GET_LED_COLOR_LEVER_B);
}

#elif defined(_SUPPORT_THREE_LIGHT_)
static void panel_light_polling(void)
{
	switch(GET_PANEL_LIGHT_STATE)
	{
		case PANEL_LIGHT_STATE_BLUE:
			SYS_RED_OFF();
			SYS_BLUE_ON();
			break;
		case PANEL_LIGHT_STATE_RED:
			SYS_RED_ON();
			SYS_BLUE_OFF();
			break;
		case PANEL_LIGHT_STATE_FUCHSINE:
			SYS_RED_ON();
			SYS_BLUE_ON();
			break;
		case PANEL_LIGHT_STATE_AUTO:
			if(GET_PANEL_LIGHT_TIME)GET_PANEL_LIGHT_TIME--;
			else
			{
				SET_PANEL_LIGHT_TIME(PANEL_LIGHT_AUTO_TIME);

			#if defined(_SPECIAL_LIGHT_DISPLAY_)
    			if(GET_PANEL_AUTO_STATE < PANEL_LIGHT_STATE_RED)GET_PANEL_AUTO_STATE++;
    			else SET_PANEL_AUTO_STATE(PANEL_LIGHT_STATE_BLUE);
			#else
				if(GET_PANEL_AUTO_STATE < PANEL_LIGHT_STATE_FUCHSINE)GET_PANEL_AUTO_STATE++;
				else SET_PANEL_AUTO_STATE(PANEL_LIGHT_STATE_RED);
			#endif
			}
			switch(GET_PANEL_AUTO_STATE)
			{
				case PANEL_LIGHT_STATE_BLUE:
					SYS_RED_OFF();
					SYS_BLUE_ON();
					break;
				case PANEL_LIGHT_STATE_RED:
					SYS_RED_ON();
					SYS_BLUE_OFF();
					break;
				case PANEL_LIGHT_STATE_FUCHSINE:
					SYS_RED_ON();
					SYS_BLUE_ON();
					break;
			}
			break;
	}
}
#elif defined(_SUPPORT_PANEL_74HC595)
static void panel_light_polling(void)
{
	switch(GET_PANEL_LIGHT_STATE)
	{
		case PANEL_LIGHT_STATE_BLUE:
				latches_send_data(0x20);//输出数据由高至低
			break;
		case PANEL_LIGHT_STATE_GREEN:
				latches_send_data(0x40);
			break;
		case PANEL_LIGHT_STATE_CYAN:
				latches_send_data(0x60);
			break;
		case PANEL_LIGHT_STATE_RED:
				latches_send_data(0x80);
			break;
		case PANEL_LIGHT_STATE_FUCHSINE:
				latches_send_data(0xA0);
			break;
		case PANEL_LIGHT_STATE_YELLOW:
				latches_send_data(0xC0);
			break;
		case PANEL_LIGHT_STATE_WHITE:
				latches_send_data(0xE0);
			break;
		case PANEL_LIGHT_STATE_AUTO:
			if(GET_PANEL_LIGHT_TIME)GET_PANEL_LIGHT_TIME--;
			else
			{
				SET_PANEL_LIGHT_TIME(PANEL_LIGHT_AUTO_TIME);
				if(GET_PANEL_AUTO_STATE < PANEL_LIGHT_STATE_WHITE)GET_PANEL_AUTO_STATE++;
				else SET_PANEL_AUTO_STATE(PANEL_LIGHT_STATE_BLUE);
			}
			switch(GET_PANEL_AUTO_STATE)
			{
				case PANEL_LIGHT_STATE_BLUE:
					latches_send_data(0x20);
					break;
				case PANEL_LIGHT_STATE_GREEN:
					latches_send_data(0x40);
					break;
				case PANEL_LIGHT_STATE_CYAN:
					latches_send_data(0x60);
					break;
				case PANEL_LIGHT_STATE_RED:
					latches_send_data(0x80);
					break;
				case PANEL_LIGHT_STATE_FUCHSINE:
					latches_send_data(0xA0);
					break;
				case PANEL_LIGHT_STATE_YELLOW:
					latches_send_data(0xC0);
					break;
				case PANEL_LIGHT_STATE_WHITE:
					latches_send_data(0xE0);
					break;
			}
			break;
	}
}

#else

#if defined(TD_ADD_NEW_APP)
uint8 panel_light=10;
uint8 auto_panel_light=10;
extern void new_app_send_light(void);
#endif
static void panel_light_polling(void)
{
	switch(GET_PANEL_LIGHT_STATE)
	{
		case PANEL_LIGHT_STATE_BLUE:
	    #if defined(TD_ADD_NEW_APP)
	    	setup.Auto_On_Off = 0;
	    	setup.clour = 0x02;
		#endif
			SYS_RED_OFF();
			SYS_GREEN_OFF();
			SYS_BLUE_ON();
			break;
		case PANEL_LIGHT_STATE_GREEN:
	    #if defined(TD_ADD_NEW_APP)
	    	setup.Auto_On_Off = 0;
			setup.clour = 0x01;
	    #endif
			SYS_RED_OFF();
			SYS_GREEN_ON();
			SYS_BLUE_OFF();
			break;
		case PANEL_LIGHT_STATE_CYAN:
	    #if defined(TD_ADD_NEW_APP)
	    	setup.Auto_On_Off = 0;
			setup.clour = 0x05;
	    #endif
			SYS_RED_OFF();
			SYS_GREEN_ON();
			SYS_BLUE_ON();
			break;
		case PANEL_LIGHT_STATE_RED:
        #if defined(TD_ADD_NEW_APP)
        	setup.Auto_On_Off = 0;
			setup.clour = 0x00;
        #endif
			SYS_RED_ON();
			SYS_GREEN_OFF();
			SYS_BLUE_OFF();
			break;
		case PANEL_LIGHT_STATE_FUCHSINE:
	    #if defined(TD_ADD_NEW_APP)
	    	setup.Auto_On_Off = 0;
			setup.clour = 0x04;
	    #endif
			SYS_RED_ON();
			SYS_GREEN_OFF();
			SYS_BLUE_ON();
			break;
		case PANEL_LIGHT_STATE_YELLOW:
	    #if defined(TD_ADD_NEW_APP)
	    	setup.Auto_On_Off = 0;
			setup.clour = 0x03;
	    #endif
			SYS_RED_ON();
			SYS_GREEN_ON();
			SYS_BLUE_OFF();
			break;
		case PANEL_LIGHT_STATE_WHITE:
	    #if defined(TD_ADD_NEW_APP)
	    	setup.Auto_On_Off = 0;
			setup.clour = 0x06;
	    #endif
			SYS_RED_ON();
			SYS_GREEN_ON();
			SYS_BLUE_ON();
			break;
		case PANEL_LIGHT_STATE_AUTO:
		#if defined(TD_ADD_NEW_APP)
			setup.Auto_On_Off = 1;
		#endif
			if(GET_PANEL_LIGHT_TIME)GET_PANEL_LIGHT_TIME--;
			else
			{
				SET_PANEL_LIGHT_TIME(PANEL_LIGHT_AUTO_TIME);
				if(GET_PANEL_AUTO_STATE < PANEL_LIGHT_STATE_WHITE)GET_PANEL_AUTO_STATE++;
				else SET_PANEL_AUTO_STATE(PANEL_LIGHT_STATE_BLUE);
			}
			switch(GET_PANEL_AUTO_STATE)
			{
				case PANEL_LIGHT_STATE_BLUE:
			    #if defined(TD_ADD_NEW_APP)
			    	setup.clour = 0x02;
			    #endif
					SYS_RED_OFF();
					SYS_GREEN_OFF();
					SYS_BLUE_ON();
					break;
				case PANEL_LIGHT_STATE_GREEN:
				#if defined(TD_ADD_NEW_APP)
					setup.clour = 0x01;
				#endif
					SYS_RED_OFF();
					SYS_GREEN_ON();
					SYS_BLUE_OFF();
					break;
				case PANEL_LIGHT_STATE_CYAN:
				#if defined(TD_ADD_NEW_APP)
					setup.clour = 0x05;
				#endif
					SYS_RED_OFF();
					SYS_GREEN_ON();
					SYS_BLUE_ON();
					break;
				case PANEL_LIGHT_STATE_RED:
				#if defined(TD_ADD_NEW_APP)
					setup.clour = 0x00;
				#endif
					SYS_RED_ON();
					SYS_GREEN_OFF();
					SYS_BLUE_OFF();
					break;
				case PANEL_LIGHT_STATE_FUCHSINE:
				#if defined(TD_ADD_NEW_APP)
					setup.clour = 0x04;
				#endif
					SYS_RED_ON();
					SYS_GREEN_OFF();
					SYS_BLUE_ON();
					break;
				case PANEL_LIGHT_STATE_YELLOW:
				#if defined(TD_ADD_NEW_APP)
					setup.clour = 0x03;
				#endif
					SYS_RED_ON();
					SYS_GREEN_ON();
					SYS_BLUE_OFF();
					break;
				case PANEL_LIGHT_STATE_WHITE:
				#if defined(TD_ADD_NEW_APP)
					setup.clour = 0x06;
				#endif
					SYS_RED_ON();
					SYS_GREEN_ON();
					SYS_BLUE_ON();
					break;
			}
		#if defined(TD_ADD_NEW_APP)
			if(auto_panel_light != GET_PANEL_AUTO_STATE)
			{
				new_app_send_light();
				auto_panel_light = GET_PANEL_AUTO_STATE;
			}
		#endif
			break;
	}
#if defined(TD_ADD_NEW_APP)
	if(panel_light != GET_PANEL_LIGHT_STATE)
	{
		new_app_send_light();
		panel_light = GET_PANEL_LIGHT_STATE;
		if(GET_PANEL_LIGHT_STATE != PANEL_LIGHT_STATE_AUTO)
		{
			setup.save_clour = GET_PANEL_LIGHT_STATE;
		}
	}
#endif
}
#endif
#endif

#if defined(_SUPPORT_30P60_LIGHT_)
uint8 light_state = 10;
extern void new_app_send_light(void);
#if defined(_ENABLE_LIGHT_SHADE_)
extern void new_app_shade_light(void);
#endif
void panel_light_polling(void)
{
	if(light_state != GET_PANEL_LIGHT_STATE)
	{
		light_state = GET_PANEL_LIGHT_STATE;
		ew_printf("GET_PANEL_LIGHT_STATE:%d\n",GET_PANEL_LIGHT_STATE);
		if(GET_PANEL_LIGHT_STATE != PANEL_LIGHT_STATE_AUTO)
		{
			setup.save_clour = GET_PANEL_LIGHT_STATE;
		}
		
	#if defined(_ENABLE_LIGHT_SHADE_)
		if((GET_PANEL_LIGHT_STATE != PANEL_LIGHT_STATE_AUTO)&&(GET_PANEL_LIGHT_STATE != 8))
		{
			setup.save_light2 = GET_PANEL_LIGHT_STATE;
		}
	#endif
		
		ew_printf("setup.save_clour:%d\n",setup.save_clour);
		
    #if defined(_ENABLE_LIGHT_SHADE_)//APP调了渐变之后,开关机和开关ACC要保存渐变状态
    	if((setup.save_light)&&((setup.red > 0)&&(setup.red < 0xFF))&&((setup.green > 0)&&(setup.green < 0xFF))&&((setup.blue > 0)&&(setup.blue < 0xFF)))
    	{
    		setup.save_light = 0;
    		LightDriverIrCmd(0x0A,setup.red,setup.green,setup.blue);
    	}
		else
    #endif
    	{
			switch(GET_PANEL_LIGHT_STATE)
    		{
    			case PANEL_LIGHT_STATE_BLUE:
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				LightDriverIrCmd(0x03,0,0,0);
    			#else
    				LightDriverIrCmd(0x03);
    			#endif
    			
    			#if defined(TD_ADD_NEW_APP)
    		    	setup.Auto_On_Off = 0;
    		    	setup.clour = 0x02;
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				setup.red = 0;
        			setup.green = 0;
        			setup.blue = 0xFF;
    				new_app_shade_light();
    			#else
    				new_app_send_light();
    			#endif
    			#endif
    				break;
    			case PANEL_LIGHT_STATE_GREEN:
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				LightDriverIrCmd(0x02,0,0,0);
    			#else
    				LightDriverIrCmd(0x02);
    			#endif
    			#if defined(TD_ADD_NEW_APP)
    		    	setup.Auto_On_Off = 0;
    		    	setup.clour = 0x01;
    			#if defined(_ENABLE_LIGHT_SHADE_)
    		    	setup.red = 0;
    		    	setup.green = 0xFF;
    		    	setup.blue = 0;
    		    	new_app_shade_light();
    			#else
    		    	new_app_send_light();
    			#endif
    			#endif
    				break;
    			case PANEL_LIGHT_STATE_CYAN:
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				LightDriverIrCmd(0x05,0,0,0);
    			#else
    				LightDriverIrCmd(0x05);
    			#endif
    			
    			#if defined(TD_ADD_NEW_APP)
    				setup.Auto_On_Off = 0;
    				setup.clour = 0x05;
    			#if defined(_ENABLE_LIGHT_SHADE_)
    		    	setup.red = 0;
    		    	setup.green = 0xFF;
    		    	setup.blue = 0xFF;
    		    	new_app_shade_light();
    			#else
    		    	new_app_send_light();
    			#endif
    			#endif
    				break;
    			case PANEL_LIGHT_STATE_RED:
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				LightDriverIrCmd(0x04,0,0,0);
    			#else
    				LightDriverIrCmd(0x04);
    			#endif
    			
    			#if defined(TD_ADD_NEW_APP)
    				setup.Auto_On_Off = 0;
    				setup.clour = 0x00;
    			#if defined(_ENABLE_LIGHT_SHADE_)
    		    	setup.red = 0xFF;
    		    	setup.green = 0;
    		    	setup.blue = 0;
    		    	new_app_shade_light();
    			#else
    		    	new_app_send_light();
    			#endif
    			#endif
    				break;
    			case PANEL_LIGHT_STATE_FUCHSINE:
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				LightDriverIrCmd(0x07,0,0,0);
    			#else
    				LightDriverIrCmd(0x07);
    			#endif
    			
    			#if defined(TD_ADD_NEW_APP)
    				setup.Auto_On_Off = 0;
    				setup.clour = 0x04;
    			#if defined(_ENABLE_LIGHT_SHADE_)
    		    	setup.red = 0xFF;
    		    	setup.green = 0;
    		    	setup.blue = 0xFF;
    		    	new_app_shade_light();
    			#else
    		    	new_app_send_light();
    			#endif
    			#endif
    				break;
    			case PANEL_LIGHT_STATE_YELLOW:
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				LightDriverIrCmd(0x06,0,0,0);
    			#else
    				LightDriverIrCmd(0x06);
    			#endif
    			
    			#if defined(TD_ADD_NEW_APP)
    				setup.Auto_On_Off = 0;
    				setup.clour = 0x03;
    			#if defined(_ENABLE_LIGHT_SHADE_)
    		    	setup.red = 0xFF;
    		    	setup.green = 0xFF;
    		    	setup.blue = 0;
    		    	new_app_shade_light();
    			#else
    		    	new_app_send_light();
    			#endif
    			#endif
    				break;
    			case PANEL_LIGHT_STATE_WHITE:
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				LightDriverIrCmd(0x08,0,0,0);
    			#else
    				LightDriverIrCmd(0x08);
    			#endif
    			
    			#if defined(TD_ADD_NEW_APP)
    				setup.Auto_On_Off = 0;
    				setup.clour = 0x06;
                	new_app_send_light();
    			#endif
    				break;
    			case PANEL_LIGHT_STATE_AUTO:
					puts("PANEL_LIGHT_STATE_AUTO111\n");
    			#if defined(_ENABLE_LIGHT_SHADE_)
    				LightDriverIrCmd(0x09,0,0,0);
    			#elif defined(_ENABLE_LIGHT_SHADE1_)
    				LightDriverIrCmd(0x09);
    			#else
    				LightDriverIrCmd(0x01);
    			#endif
    			#if defined(TD_ADD_NEW_APP)
    				setup.Auto_On_Off = 1;
    				new_app_send_light();
    			#endif
    				break;
			#if defined(_ENABLE_LIGHT_SHADE_)
				case 8:
					ew_printf("setup.save_light1:%d\n",setup.save_light1);
			    	switch(setup.save_light1)
			    	{
			    		case PANEL_LIGHT_STATE_BLUE:
			    			LightDriverIrCmd(0x03,0,0,0);
							setup.Auto_On_Off = 0;
							setup.clour = 0x02;
							setup.red = 0;
							setup.green = 0;
							setup.blue = 0xFF;
							new_app_shade_light();
			    			break;
			    		case PANEL_LIGHT_STATE_GREEN:
			    			LightDriverIrCmd(0x02,0,0,0);
							setup.Auto_On_Off = 0;
							setup.clour = 0x01;
							setup.red = 0;
							setup.green = 0xFF;
							setup.blue = 0;
							new_app_shade_light();
			    			break;
			    		case PANEL_LIGHT_STATE_CYAN:
			    			LightDriverIrCmd(0x05,0,0,0);
							setup.Auto_On_Off = 0;
							setup.clour = 0x05;
							setup.red = 0;
							setup.green = 0xFF;
							setup.blue = 0xFF;
							new_app_shade_light();
			    			break;
			    		case PANEL_LIGHT_STATE_RED:
			    			LightDriverIrCmd(0x04,0,0,0);
							setup.Auto_On_Off = 0;
							setup.clour = 0x00;
							setup.red = 0xFF;
							setup.green = 0;
							setup.blue = 0;
							new_app_shade_light();
			    			break;
			    		case PANEL_LIGHT_STATE_FUCHSINE:
			    			LightDriverIrCmd(0x07,0,0,0);
							setup.Auto_On_Off = 0;
							setup.clour = 0x04;
							setup.red = 0xFF;
							setup.green = 0;
							setup.blue = 0xFF;
							new_app_shade_light();
			    			break;
			    		case PANEL_LIGHT_STATE_YELLOW:
			    			LightDriverIrCmd(0x06,0,0,0);
							setup.Auto_On_Off = 0;
							setup.clour = 0x03;
							setup.red = 0xFF;
							setup.green = 0xFF;
							setup.blue = 0;
							new_app_shade_light();
			    			break;
			    		case PANEL_LIGHT_STATE_WHITE:
			    			LightDriverIrCmd(0x08,0,0,0);
							setup.Auto_On_Off = 0;
							setup.clour = 0x06;
							setup.red = 0xFF;
							setup.green = 0xFF;
							setup.blue = 0xFF;
							new_app_shade_light();
			    			break;
			    		//case PANEL_LIGHT_STATE_AUTO:
			    		//	LightDriverIrCmd(0x09,0,0,0);
			    		//	break;
			    	}
					break;
				#endif
				
				default:
					break;
    		}
		}
	}
}
#endif

#if defined(SUPPORT_LED_ADJUST)
static void back_led_polling(void)
{
	switch(setup.LedState)
	{
		case BACK_LED_LOW:
			BACK_LED_LOW_ON();
			BACK_LED_MIN_OFF();
			break;
		case BACK_LED_MID:
			BACK_LED_LOW_OFF();
			BACK_LED_MIN_ON();
			break;
		case BACK_LED_HIGH:
			BACK_LED_LOW_ON();
			BACK_LED_MIN_ON();
			break;
		default:
			break;
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_BT_ICON_)
void open_bt_icon_polling(void)
{
	//ew_printf("get_bt_connect_status:%d\n",get_bt_connect_status());

	if((get_bt_connect_status() == BT_STATUS_CONNECTING)
	||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
	||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
    #if defined(BT_ICON_BLINK)
    	ui_drv_func.flash_disable(UI_ICON_BLUETOOTH);
    #endif
	}
	else
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
	}
}
#endif

#if defined(_ENABLE_TEL_MUTE_DETECT_)
void sys_tel_mute_polling(void)
{
	if(BT_TEL_MUTE_CHECK())
	{
		global.tel_mute_flag = TRUE;
		if(global.aud_mute)
		{
			disp_send_msg(DISP_EVENT_MUTE);
			global.tel_mute_flag1 = TRUE;
		}
		AUDIO_MUTE_ON();
		audio_drv_ctrl(AUD_CTRL_VOLUME, 0, TRUE);
	#if defined(_ENABLE_TEL_MUTE_FLASH_)
    	if(++global.tel_mute_time <= (800/DISP_POLLING_TIME))
    	{
    		ui_show_setting(LANG_ID_OTHERS_MUTE_ON,0);
    	}
    	else if((++global.tel_mute_time <= (2000/DISP_POLLING_TIME))&&(++global.tel_mute_time > (800/DISP_POLLING_TIME)))
    	{
    		ui_show_setting(LANG_ID_NULL,0);
    	}
    	else
    	{
    		global.tel_mute_time = 0;
    	}
	#else
		ui_show_setting(LANG_ID_TEL_MUTE,0);
		ui_disp.timer = -1;
	#endif
	}
	else
	{
		if(global.tel_mute_flag)
		{
			AUDIO_MUTE_OFF();
			audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
			if(global.tel_mute_flag1)
			{
				disp_send_msg(DISP_EVENT_MUTE);
				global.tel_mute_flag1 = FALSE;
			}
			else
			{
    			ui_disp.timer = 0;
			}
			global.tel_mute_flag = FALSE;
		#if defined(_ENABLE_TEL_MUTE_FLASH_)
			global.tel_mute_time = 0;
		#endif
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*------50MS进一次---------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_BEEP_)
void sys_beep_polling(void)
{
	if(global.sys_power)
	{
		if(global.beep_time)
		{
			global.beep_time--;
			ew_printf("global.beep_time:%d\n",global.beep_time);
			if(!global.beep_time)
			{
			#if defined(SET_BEEP_FUNCV)
				set_beep_ctrl(0);
				ew_printf("sys_beep_polling--flag:%d--mute:%d\n",global.beep_mute_flag,global.aud_mute);
				if(global.beep_mute_flag)
				{
					global.beep_mute_flag = FALSE;
					if(global.aud_mute || mute_state || inside_mute_state)
					{
						ew_printf("AUDIO_MUTE_ON\n");
    					AUDIO_MUTE_ON();
					}
				}
			#else
			#if !defined(_XM007_SPECIAL_MUTE_)
				if(global.aud_mute)//按MUTE键
				{
					AUDIO_MUTE_ON();
				}
			#endif
			#if defined(_SUPPORT_PANEL_PWM_)
				set_panel_pwm(0, 0, 0);
			#endif
			#if defined(_XM007_SPECIAL_MUTE_)
				global.xm_mute_flag = FALSE;
			#endif
			#endif
			}
		}
	}
	else
	{
		AUDIO_MUTE_ON();
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_DISK_DISC_DISPLAY_)
void sys_det_disc_polling(void)
{
    if((!compare_task_name("MusicTaskUsb"))&&(!compare_task_name("MusicTaskCard")))
    {
		if(file_operate_get_total_phydev())
		{
			if((dev_get_online_status(usb))
		#if defined(_SUPPORT_SD_)
			||(dev_get_online_status(sd1))
		#endif
			)
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
			}
			else
			{
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
			}
		}
		else
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SPECIAL_MUTE_FLASH_)
void mute_display_polling(void)
{
	if(global.sys_power)
	{
		if(global.aud_mute)
		{
			if(++global.mute_time <= (800/DISP_POLLING_TIME))
			{
				ui_show_setting(LANG_ID_OTHERS_MUTE_ON,0);
			}
			else if((++global.mute_time <= (2000/DISP_POLLING_TIME))&&(++global.mute_time > (800/DISP_POLLING_TIME)))
			{
				ui_show_setting(LANG_ID_NULL,0);
			}
			else
			{
				global.mute_time = 0;
			}
		}
		else
		{
			global.mute_time = 0;
		}
	}
	else
	{
		global.mute_time = 0;
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_VOLUME_CONTROL_)
void show_icon_polling(void)
{
	if(global.sys_power)
	{
		if(setup.zone1_start)
		{
			if(global.aud_mute)
			{
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE1);
			}
			else
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_ZONE1);
			}
		}
		else
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE1);
		}

    	if(setup.zone2_start)
    	{
			if(global.aud_mute)
			{
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE2);
			}
			else
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_ZONE2);
			}
    	}
    	else
    	{
    		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE2);
    	}

    	if(setup.zone3_start)
    	{
			if(global.aud_mute)
			{
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE3);
			}
			else
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_ZONE3);
			}
    	}
    	else
    	{
    		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE3);
    	}

    	if(setup.zone4_start)
    	{
			if(global.aud_mute)
			{
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE4);
			}
			else
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_ZONE4);
			}
    	}
    	else
    	{
    		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE4);
    	}
	}
	else
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE1);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE2);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE3);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ZONE4);
	}
}
#endif

#if defined(_SUPPORT_SUB_)
void sys_sub_polling(void)
{
	if(setup.sub_start)
	{
		SYS_SUB_ON();
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
	}
	else
	{
		SYS_SUB_OFF();
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_SUBWOOFER2_)
void sys_sub_polling(void)
{
	if(setup.aud_sub)
	{
		if((global.aud_mute)||mute_state ||(setup.aud_vol == 0))
		{
		#if !defined(_FY7000_SUB_MUTE_)
	    #if defined(_ENABLE_RCA_MUTE_)
	    	if(setup.aud_vol != 0)
	    	{
			#if defined(_DISABLE_SUB_OFF_)
				if(global.sub_flag)
				{
					SYS_SUB_OFF();
				}
			#else
	    		SYS_SUB_OFF();
	    	#endif
	    	}
	    #else
	    	SYS_SUB_OFF();
	    #endif
		#endif
		}
		else
		{
		#if defined(_ENABLE_RCA_MUTE_)
			if(global.sub_flag)
			{
				global.sub_time++;
				if(global.sub_time >= (1000/50))
				{
					SYS_SUB_ON();
					global.sub_flag = FALSE;
					global.sub_time = 0;
				}
			}
			else
			{
				SYS_SUB_ON();
			}
		#else
			SYS_SUB_ON();
		#endif
		}
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
	}
	else
	{
		SYS_SUB_OFF();
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
	}
}
#endif

#if defined(_SUPPORT_SUBWOOFER3_)
void sys_sub2_polling(void)
{
	if(setup.aud_sub1)
	{	
		if((global.aud_mute)||mute_state ||(setup.aud_vol == 0))
		{
		#if !defined(_FY7000_SUB_MUTE_)
		#if defined(_ENABLE_RCA_MUTE_)
			if(setup.aud_vol != 0)
			{
			#if defined(_DISABLE_SUB_OFF_)
				if(global.sub_flag)
				{
					SYS_SUB2_OFF();
				}
			#else
				SYS_SUB2_OFF();
			#endif
			}
		#else
			SYS_SUB2_OFF();
		#endif
		#endif
		}
		else
		{
		#if defined(_ENABLE_RCA_MUTE_)
			if(global.sub_flag)
			{
				global.sub_time++;
				if(global.sub_time >= (1000/50))
				{
					SYS_SUB2_ON();
					global.sub_flag = FALSE;
					global.sub_time = 0;
				}
			}
			else
			{
				SYS_SUB2_ON();
			}
		#else
			SYS_SUB2_ON();
		#endif
		}
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
	}
	else
	{
		SYS_SUB2_OFF();
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
	}
}
#endif

#if defined(_ENABLE_RCA_MUTE_)&&0
void sys_rca_polling(void)
{
	if(setup.aud_vol == 0)
	{
	#if defined(_CLOSE_AMP_LIGHT_)
		AMP_CONTROL_OFF();
	#endif
		global.rca_time++;
		if(global.rca_time == (500/50))
		{
			RCA_MUTE_ON();
		#if defined(_SUPPORT_SUBWOOFER2_)
			if(setup.aud_sub)
			{
				SYS_SUB_OFF();
			}
		#endif
		#if defined(_SUPPORT_SUBWOOFER3_)
			if(setup.aud_sub1)
			{
				SYS_SUB2_OFF();
			}
		#endif
		}
		
		if(global.rca_time == 50)
		{
			global.rca_time = 100;
		}
	}
	else
	{
    #if defined(_CLOSE_AMP_LIGHT_)
		if(global.rca_power)
		{
			AMP_CONTROL_ON();
		}
    #endif
		global.rca_time = 0;
	}
}
#endif

#if defined(SET_MUTE_WHEN_VOL_)
void set_mute_polling(void)
{
	if(setup.aud_vol==0)
	{
		dac_mute(1,1);
		AUDIO_MUTE_ON();
	}
}
#endif

#if defined(_ENABLE_IPHONE_CHARGE_)
static uint8 idle_usb_detect(void)
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

static uint8 idle_card_state1(void)
{
	if(file_operate_get_total_phydev())
	{
		if(dev_get_online_status(sd1))
		{
			return TRUE;
		}
	}
	return FALSE;
}

#if defined(_ENABLE_FAST_CHARGING_)
void ui_show_fast_charge(void)
{
	ui_show_setting(LANG_ID_FAST_CHARGE, 0);
	setup.charge_flg = TRUE;
	setup.charge_time = (6000/DISP_POLLING_TIME);
}

void ui_show_slow_charge(void)
{
	setup.charge_flg = FALSE;
	ui_show_setting(LANG_ID_SLOW_CHARGE, 0);
}
#endif

void sys_charge_polling(void)
{
	uint8 card_now = idle_card_state1();
	uint8 card_state;

	if(idle_usb_detect())
	{
		//disp_puts("sys_charge_polling\n");
		global.charge_time = 0;

		card_state = idle_card_state1();
		if(card_now != card_state)
		{
			//disp_puts("sys_charge_polling888\n");
			card_now = card_state;
			if(idle_card_state1())
			{
				setup.usb_detect1 = TRUE;
				//disp_puts("sys_charge_polling777\n");
			}
		}

		if(setup.usb_detect1)
		{
			SYS_CHARGE_ON();
			global.charge_flg = FALSE;
			//disp_puts("sys_charge_polling444\n");
		}
		else
		{
			SYS_CHARGE_OFF();
			global.charge_flg = TRUE;
			global.charge_time = 80;
			//disp_puts("sys_charge_polling555\n");
		}
	}
	else
	{
		if(!setup.usb_detect)
		{
			//disp_printf("sys_charge_polling000\n");
			global.charge_time++;
			if(global.charge_time <= (3000/DISP_POLLING_TIME))
			{
				//disp_printf("sys_charge_polling111\n");
				SYS_CHARGE_OFF();
				setup.usb_detect = FALSE;
				setup.usb_detect1 = FALSE;

			}
			else
			{
				//disp_printf("sys_charge_polling222\n");
				setup.usb_detect = TRUE;
			}
		}

	#if defined(_ENABLE_FAST_CHARGING_)
		if(setup.charge_flg)
		{
			if(setup.charge_time)
			{
				setup.charge_time--;
			}
			else
			{
				setup.charge_time = 0;
				ui_show_slow_charge();
			}
		}
	#endif

		if(setup.usb_detect)
		{
			//disp_printf("sys_charge_polling999\n");
			if(!global.charge_time--)
			{
				//disp_printf("sys_charge_polling111222\n");
			#if defined(_ENABLE_FAST_CHARGING_)
				if(setup.charge_flg)
				{
					SYS_CHARGE_ON();
				}
				else
				{
					SYS_CHARGE_OFF();
				}
			#else
				SYS_CHARGE_ON();
			#endif
				global.charge_time = 0;
				setup.usb_detect1 = FALSE;
				global.charge_flg = FALSE;
			}
		}
		//disp_printf("sys_charge_polling333\n");
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_AUTO_POWER_)
void memory_mode_polling(void)
{
	if(compare_task_name("MusicTaskUsb"))
	{
		setup.save_mode_name = "MusicTaskUsb";
	}
	else if(compare_task_name("MusicTaskCard"))
	{
		setup.save_mode_name = "MusicTaskCard";
	}
	else if(compare_task_name("LineinTask"))
	{
		setup.save_mode_name = "LineinTask";
	}
	else if(compare_task_name("FMRadioTask"))
	{
		setup.save_mode_name = "FMRadioTask";
	}
	else if(compare_task_name("BtUserTask"))
	{
		setup.save_mode_name = "BtUserTask";
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_FY7555_CLOCK_ICON_DISPLAY_)
void time_icon_polling(void)
{
	if(setup.time_flag)
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_AM);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
	}
	else
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_AM);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PM);
	}
	
#if defined(_ENABLE_TIME_CLEAN_)
	if(global.clean_time)
	{
		if((++global.logo_time) >= (200/DISP_POLLING_TIME))
		{
			extern void poweron_io_output(uint8 flag);
			poweron_io_output(1);
			global.clean_time = FALSE;
			global.logo_time = 0;
		}
	}
#endif
}
#endif
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_WLS_MUTE_FUNTION_)
void mute_funtion_polling(void)
{
	if(global.sys_power)
	{
		if(compare_task_name("FMRadioTask"))
		{
			AUDIO_MUTE1_OFF();
		}
		else
		{
			AUDIO_MUTE1_ON();
		}
	}
	else
	{
		AUDIO_MUTE1_ON();
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_BAND_MUTE_)
void band_mute_polling(void)
{
	if(compare_task_name("FMRadioTask"))
	{
		if(global.mute_flag)
		{
			if(global.mute_time)
			{
				AUDIO_MUTE_ON();
				global.mute_time--;
				//ew_puts("band_mute_polling\n");
			}
			else
			{
				AUDIO_MUTE_OFF();
				global.mute_time = 0;
				global.mute_flag = FALSE;
				//ew_puts("band_mute_polling11111\n");
			}
		}
		else
		{
			global.mute_time = 0;
		}
	}
	else
	{
		global.mute_flag = FALSE;
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_POWER_MUTE_)
void power_mute_polling(void)
{
	if(global.mute_flag)
	{
		if(global.mute_time)
		{
			AUDIO_MUTE_ON();
			global.mute_time--;
		}
		else
		{
			AUDIO_MUTE_OFF();
			global.mute_time = 0;
			global.mute_flag = FALSE;
		}
	}
	else
	{
		global.mute_time = 0;
	}
}
#elif defined(_ENABLE_FIRST_POWER_MUTE_)
void power_mute_polling(void)
{
	if(global.mute_flag)
	{
		if(global.mute_time)
		{
			AUDIO_MUTE_ON();
			audio_drv_ctrl(AUD_CTRL_VOLUME, 0, TRUE);
			global.mute_time--;
		}
		else
		{
			AUDIO_MUTE_OFF();
			audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
			global.mute_time = 0;
			global.mute_flag = FALSE;
		}
	}
	else
	{
		global.mute_time = 0;
	}
}
#endif

#if defined(_ENABLE_MUTE_OFF_)
void power_off_polling(void)
{
	if(global.mute_power_flag)
	{
		global.mute_power_time--;
		if(!global.mute_power_time)
		{
			AUDIO_MUTE_OFF();
			global.mute_power_flag = FALSE;
			global.mute_power_time = 0;
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_BT_POWER_)
void mode_start_polling(void)
{
	if(compare_task_name("MusicTaskUsb"))
	{
		setup.mode_name = "MusicTaskUsb";
	}
	else if(compare_task_name("MusicTaskCard"))
	{
		setup.mode_name = "MusicTaskCard";
	}
	else if(compare_task_name("LineinTask"))
	{
		setup.mode_name = "LineinTask";
	}
	else if(compare_task_name("FMRadioTask"))
	{
		setup.mode_name = "FMRadioTask";
	}
	else if(compare_task_name("DABTask"))
	{
		setup.mode_name = "DABTask";
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_EVB_BT_CONNECT_BT_MODE_)
static uint8 bt_connect4(void)
{
	switch(get_bt_connect_status())
	{
		case BT_STATUS_CONNECTING:
		case BT_STATUS_TAKEING_PHONE:
		case BT_STATUS_PLAYING_MUSIC:
			return 1;
	}
	return 0;
}

extern u8 get_first_time_conn_flag(void);//判断是回连还是第一次连接
void connect_bt_mode_polling(void)
{
	if(bt_connect4())
	{
		if(!global.bt_connect)
		{
			global.bt_connect = TRUE;
			
			if(get_first_time_conn_flag())
			{
				if(global.sys_power)
				{
					if(!compare_task_name("BtUserTask"))
					{
						task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
					}
				}
				else
				{
					os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_EXIT);
					OSTimeDly(5);
					task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
				}
			}
		}
	}
	else
	{
		global.bt_connect = FALSE;
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(DISABLE_TURN_BT_MODE)
static uint8 bt_connect(void)
{
	switch(get_bt_connect_status())
	{
		case BT_STATUS_CONNECTING:
		case BT_STATUS_TAKEING_PHONE:
		case BT_STATUS_PLAYING_MUSIC:
			return 1;
	}
	return 0;
}

void bt_detect_polling(void)
{
	if(bt_connect())
	{
	 	if(compare_task_name("BtUserTask"))
		{
    		if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
    		||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
    		||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
    		||(get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)
    		||(get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK)
    		||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
    		||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
			{
				setup.bt_flg = FALSE;
			}
			else
			{
				setup.bt_flg = TRUE;
			}
		#if defined(_ENABLE_BT_MODE_MEMORY_)
			setup.sd_start = FALSE;
		#endif
		}
		else if(compare_task_name("MusicTaskUsb"))
		{
			setup.bt_flg = FALSE;
		}
		else if(compare_task_name("MusicTaskCard"))
		{
			setup.bt_flg = FALSE;
		}
		else if(compare_task_name("LineinTask"))
		{
			setup.bt_flg = FALSE;
		}
		else if(compare_task_name("FMRadioTask"))
		{
			setup.bt_flg = FALSE;
		}
	}

	//printf("setup.bt_flg:%d\n",setup.bt_flg);
	//printf("bt_connect:%d\n",bt_connect());
	if((!bt_connect())&&(setup.bt_flg))
	{
		setup.connect_time++;
		//printf("setup.connect_time:%d\n",setup.connect_time);
	#if defined(_DISABLE_BT_MEMORY_)
		if(setup.connect_time >= (1500/DISP_POLLING_TIME))//1300
	#else
		if(setup.connect_time >= (1300/DISP_POLLING_TIME))//1300
	#endif
		{
	    #if defined(_ENABLE_BT_MODE_MEMORY_)
	    	setup.bt_mode = TRUE;
	    #endif
			setup.connect_time = 0;
			setup.bt_flg = FALSE;
		#if defined(_DISABLE_RADIO_)
		#if defined(_POWER_ON_BT_)
		    task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
		#else
			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
		#endif
		#elif defined(_ENABLE_BT_MODE_MEMORY_)
			if(!setup.sd_start)
			{
				task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
				setup.sd_start = FALSE;
			}
		#else
			task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
		#endif
		}
	}
	else
	{
		setup.connect_time = 0;
	}

#if defined(_ENABLE_BT_MODE_MEMORY_)
	//ew_printf("setup.bt_mode:%d\n",setup.bt_mode);
	//ew_printf("bt_connect:%d\n",bt_connect());
	//ew_printf("setup.acc_bt:%d\n",setup.acc_bt);
	if((setup.bt_mode)&&(bt_connect())&&(setup.acc_bt))
	{
		task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
		setup.bt_mode = FALSE;
		setup.acc_bt = FALSE;
	}
#endif
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_USB_DELAY_)
void mode_name_polling(void)
{
 	if(compare_task_name("BtUserTask"))
	{
		 setup.name = "BtUserTask";
	}
	else if(compare_task_name("MusicTaskUsb"))
	{
		setup.name = "MusicTaskUsb";
	}
	else if(compare_task_name("MusicTaskCard"))
	{
		setup.name = "MusicTaskCard";
	}
	else if(compare_task_name("LineinTask"))
	{
		setup.name = "LineinTask";
	}
	else if(compare_task_name("FMRadioTask"))
	{
		setup.name = "FMRadioTask";
	}
}
#endif

#if defined(_ENABLE_POWER_LOGO_)
void save_name_polling(void)
{
	if(global.sys_power)
	{
		if(!setup.save_flag)
		{
			if(compare_task_name("FMRadioTask"))
			{
				setup.save_task_name = "FMRadioTask";
				setup.save_task_name1 = "FMRadioTask";
			}
			else if(compare_task_name("BtUserTask"))
			{
				setup.save_task_name = "BtUserTask";
				setup.save_task_name1 = "BtUserTask";
			}
			else if(compare_task_name("MusicTaskUsb"))
			{
				setup.save_task_name = "MusicTaskUsb";
				setup.save_task_name1 = "MusicTaskUsb";
			}
			else if(compare_task_name("MusicTaskCard"))
			{
				setup.save_task_name = "MusicTaskCard";
				setup.save_task_name1 = "MusicTaskCard";
			}
		}
	}
	else
	{
		if(setup.save_flag)
		{
			setup.save_task_name = setup.save_task_name1;
			setup.save_flag = FALSE;
		}
	}
}
#endif
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_BT_CONNECT_)
void sys_btconnect_polling(void)
{
	if((get_bt_connect_status() == BT_STATUS_CONNECTING)
	||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
	||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
	{
		global.bt_connect_time = 0;
		global.bt_count = 0;
		setup.bt_connect_flag = TRUE;
		setup.acc_bt_flag = FALSE;
	}
	else
	{
		//printf("setup.acc_bt_flag:%d\n",setup.acc_bt_flag);
	#if defined(_ENABLE_POWER_BT_OFF_)
		if((setup.acc_bt_flag)||(global.connect_flag))
	#else
		if(setup.acc_bt_flag)
	#endif
		{
			//puts("sys_btconnect_polling\n");
			global.bt_connect_time++;
			if(global.bt_connect_time >= (8000/DISP_POLLING_TIME))
			{
				puts("sys_btconnect_polling111\n");
				global.bt_connect_time = 0;
				global.bt_count++;
				if(global.bt_count > 240)
				{
					global.bt_count = 10;
				}
				if(global.bt_count < 5)
				{
					puts("sys_btconnect_polling222\n");
					os_taskq_post_msg("btmsg", 1, MSG_BT_CONNECT_CTL);
				}
			}
		}
	}
}
#endif
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_USB_SD_ICON_)
extern uint8 detect_usb_state(void);
extern uint8 detect_sd_state(void);
void icon_detect_polling(void)
{
	if(compare_task_name("FMRadioTask"))
	{
		if((detect_usb_state())&&(detect_sd_state()))
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SD);
		}
	}
}
#endif
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_VBAR_DISPLAY_)
void vbar_display_polling(void)
{
	if(global.sys_power)
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_VBAR_1);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_VBAR_2);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_VBAR_3);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_VBAR_4);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_VBAR_5);
	}
	else
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_1);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_2);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_3);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_4);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_5);
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_DISABLE_TALKING_KEY_)
uint8 bt_call_state(void)
{
	if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
	||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
	||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
	||(get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)
	||(get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK)
	||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
	||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
	{
		return TRUE;
	}
	return FALSE;
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_ALARM_CLOCK_)
void set_beep_pwm(void)
{
	uint8 pwm = 0;

	if(setup.alarm_beep)
	{
		global.beep_count++;
		if(global.beep_count == 2)			// 3
		{
			pwm = 0;
			global.beep_count = 0;
		}
		else
		{
			pwm = 1;
		}
	}
	else
	{
		pwm = 0;
		global.beep_count = 0;
	}
#if defined(SET_BEEP_FUNCV)
	set_beep_ctrl(pwm);
#else
	set_panel_pwm(pwm, pwm, pwm);
#endif
}

#if defined(SUPPORT_ALARM_FUNC_)
void disp_alarm(void)
{
	if(global.beep_str_flag != setup.alarm_beep)
	{
		//puts("beep\n");
		global.beep_str_flag = setup.alarm_beep;
		ui_disp.func_type = FUNC_TYPE_SEL;
		if(global.beep_str_flag)
		{
			ui_show_setting(LANG_ID_OTHERS_ALARM_RINGING,0);
			ui_disp.timer = -1;
		}
		else
		{
			ui_disp.timer = 0;
		}
		ui_disp.func_sel = FUNC_SEL_NULL;
	}
}
#endif

void sys_beep_polling(void)
{
	//if(global.sys_power)
	{
		if(global.beep_time)
		{
			if(compare_task_name("FMRadioTask"))
			{
				dac_channel_off(FM_INSI_CHANNEL, FADE_OFF);
			}
			global.beep_time--;
			if(!global.beep_time)
			{
			#if defined(SET_BEEP_FUNCV)
				set_beep_ctrl(0); 
			#else
				set_panel_pwm(0, 0, 0); 
			#endif
 				setup.alarm_beep = FALSE;
				if(setup.alarm_power_flag1)
				{
					setup.alarm_power_flag1 = FALSE;
					setup.alarm_power_flag = FALSE;
				}
				
				if(setup.alarm_power)
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_POWER);
					setup.alarm_power = FALSE;
				}
				/*if(!global.sys_power)
				{
					AUDIO_MUTE_ON();
				}*/
			}
		}
	}
}
#endif

#if defined(SUPPORT_ALARM_FUNC_)
void alarm_time_polling(void)
{
	uint8 key1,key2;
	key1 = GET_KEY1_DATA;
	key2 = GET_KEY2_DATA;

	//ew_printf("setup.alarm_hour:%d\n",setup.alarm_hour);
	//ew_printf("setup.alarm_minute:%d\n",setup.alarm_minute);
	//ew_printf("setup.alarm_second:%d\n",setup.alarm_second);
	//ew_printf("setup.alarm_hour1:%d\n",setup.alarm_hour1);
	//ew_printf("setup.alarm_minute1:%d\n",setup.alarm_minute1);
	//ew_printf("setup.alarm_second1:%d\n",setup.alarm_second1);
	
	if(setup.alarm)
	{
		if((setup.alarm_hour == setup.alarm_hour1)
		&&(setup.alarm_minute == setup.alarm_minute1)
		&&(setup.alarm_second == setup.alarm_second1))
		{
			if(!global.sys_power)// 关机状态闹钟响要打开功放
			{
				os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_POWER);
				setup.alarm_power = TRUE;
				//AUDIO_MUTE_OFF();
			}
			setup.alarm_beep = TRUE;
			global.beep_time = 60;			//400
		}
		
		if(setup.alarm_beep)
		{
			if((key1 < 62)||(key2 < 62))
			{
				if((key1 < 23)&&(key1 > 18))//MODE键
				{
					setup.alarm_power_flag = TRUE;
					setup.alarm_time = 0;
					setup.alarm_count++;
					if(setup.alarm_count >= 3)
					{
						setup.alarm_count = 0;
						setup.alarm_power_flag1 = TRUE;
					}
				}
				else
				{
					setup.alarm_power_flag = FALSE;
					setup.alarm_time = 0;
					setup.alarm_count = 0;
				}
				
				setup.alarm_beep = FALSE;
			#if defined(SET_BEEP_FUNCV)
				set_beep_ctrl(0); 
			#else
				set_panel_pwm(0, 0, 0);
			#endif

				if(setup.alarm_power)
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_POWER);
					setup.alarm_power = FALSE;
				}
				/*if(!global.sys_power)
				{
					AUDIO_MUTE_ON();
				}*/
			}
		}
		
		if(setup.alarm_power_flag)
		{
			setup.alarm_time++;
			if(setup.alarm_time >= (600000/DISP_POLLING_TIME))// 10分钟
			//if(setup.alarm_time >= (30000/DISP_POLLING_TIME))// 10分钟
			{
				if(!global.sys_power)// 关机状态闹钟响要打开功放
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_POWER);
					setup.alarm_power = TRUE;
					//AUDIO_MUTE_OFF();
				}
				setup.alarm_time = 0;
				setup.alarm_beep = TRUE;
				global.beep_time = 60;			//400
				setup.alarm_power_flag = FALSE;
			}
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_CHANGE_DISP_MODE_)
void key_time_polling(void)
{
	uint8 key1,key2;
	key1 = GET_KEY1_DATA;
	key2 = GET_KEY2_DATA;
	//ew_printf("key1:%d\n",key1);
	//ew_printf("key2:%d\n",key2);
	//ew_printf("setup.ad_keyflag55555555555:%d\n",setup.ad_keyflag);
	//ew_printf("global.ad_keytime2222222:%d\n",global.ad_keytime);
	if((key2 < 64)&&(!setup.ad_keyflag))
	{
	    if((key2 == 0)||(key2 == 3)||(key2 == 7)||(key2 == 12)||(key2 == 16)||(key2 == 22)||(key2 == 27)||(key2 == 31)||(key1 == 16))
	  	{
             global.ad_keytime = 200; // 10s
		}
		else
		{
             global.ad_keytime--;
     		 if(global.ad_keytime == 0)
       		 {
                 setup.ad_keyflag = 1;
       			 global.ad_keytime = 200; // 10s
       			 //ew_printf("global.ad_keytime11111111:%d\n",global.ad_keytime);
       		 }
		}
	}
	else
	{
         global.ad_keytime = 200; // 10s
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_TIME_DISP_)
void time_disp_polling(void)
{
	uint8 key1,key2;
	key1 = GET_KEY1_DATA;
	key2 = GET_KEY2_DATA;

	//ew_printf("key1:%d\n",key1);
	//ew_printf("key2:%d\n",key2);
	if(((key1 < 63)||(key2 < 63))&&(!global.timer_flag))
	{
		global.timer_count = 4000;
		global.timer_flag = TRUE;
	}
	else
	{
		global.timer_flag = FALSE;
	}

	if(global.aud_mute)//MUTE键
	{
		global.timer_count = 4000;
	}
	
	if(setup.time_disp)
	{
		//ew_puts("time_disp\n");
		if(!global.timer_count)
		{
			//ew_puts("time_disp111\n");
			msg_realtime_show();
		}
	}
	else
	{
		if((global.work_flag1)&&(compare_task_name("BtUserTask")))//关掉时间长显清除时间显示
		{
			ui_show_setting(LANG_ID_BT_PLAY, 0);
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_SUPPORT_PLAYER_NAMES_)
void back_play_polling(void)
{
	if(setup.back_flag)
	{
		if(compare_task_name("ReverseTask"))
		{
			global.back_time++;
			if(global.back_time == 1800/50)
			{
				//tone_play_by_name(MAIN_TASK_NAME, 1, BPF_SGW05_MP3);
				tone_play_by_name(REVERSE_TASK_NAME, 1, BPF_SGW05_MP3);
				global.back_time = 0;
			}
		}
		else
		{
			global.back_time = 0;
		}
	}
	else
	{
		global.back_time = 0;
	}
}
#endif

#if defined(_POWER_OFF_BT_DEBUG_)
void talking_phone_polling(void)
{
	if(global.bt_flag)
	{
	   if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
	   ||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
	   ||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
	   ||(get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)
	   ||((get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK)&&(get_call_status() != BT_CALL_HANGUP))
	   ||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
	   ||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
	   {
	      global.phone_time++;
	      if(global.phone_time > 35)
	      {
    	       //ew_puts("takeing_phone_polling\n");
               task_switch(BT_TASK_NAME, 0, SWITCH_SPEC_TASK);
    		   global.phone_time = 0;
    		   global.bt_flag = FALSE;
	       } 
	   }
	   else
	   {
            global.phone_time = 0;
    		global.bt_flag = FALSE;
	   }
	}
	else
	{
        global.phone_time = 0;
	}
}	
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_XM007_SPECIAL_BACKLIGHT_)
void backlight_control_polling(void)
{
	if(ACC_CHECK())
	{
		if(!global.sys_power)
		{
			if(global.off_flag)
			{
				if(global.off_time--)
				{
					dac_ldo_set(LDO_1,1);
					dac_ldo_set(LDO_2,1);
				}
				else
				{
					dac_ldo_set(LDO_1,0);
					dac_ldo_set(LDO_2,0);
					
					global.off_flag = FALSE;
					global.off_time = 0;
				}
			}
		}
	}
	
#if defined(_XM007_TIME_CLEAN_)
	if(global.sys_power)
	{
		if(global.clean_time)
		{
			if((++global.logo_time) >= (200/DISP_POLLING_TIME))
			{
				dac_ldo_set(LDO_1,1);
				dac_ldo_set(LDO_2,1);
				global.clean_time = FALSE;
				global.logo_time = 0;
			}
		}
	}
#endif
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_AUX_DETECT_)
void sys_aux_polling(void)
{
	if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)&&
	(get_bt_newest_status() != BT_STATUS_PHONE_OUT)&&
	(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE))
	{
		setup.aux2 = setup.aux1;
		setup.aux1 = !(AUX_CHECK());
		if(setup.aux1 != setup.aux2)return;
		setup.aux4 = setup.aux3;
		setup.aux3 = setup.aux2;
		if(setup.aux3 == setup.aux4)return;

		if(!setup.aux3)//检测到有麦克风
		{
			ew_puts("sys_aux_polling\n");
			setup.save_name = curr_task->name;
			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
		}
		else if(!setup.aux4)//检测到麦克风
		{
			ew_puts("sys_aux_polling1111\n");
			task_switch(setup.save_name,0,SWITCH_SPEC_TASK);
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_MIC_AUX_)
uint8 mode_change=0;
void sys_mic_polling(void)
{ 
	if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)&&
	(get_bt_newest_status() != BT_STATUS_PHONE_OUT)&&
	(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE))
	{
		setup.aux2 = setup.aux1;
		setup.aux1 = !(MIC_CHECK());
		if(setup.aux1 != setup.aux2)return;
		setup.aux4 = setup.aux3;
		setup.aux3 = setup.aux2;
		if(setup.aux3 == setup.aux4)return;
		
		if(!setup.aux3)//检测到有麦克风
		{
			ew_puts("sys_mic_polling\n");
            if(!compare_task_name("LineinTask"))
    		{
    			setup.save_name = curr_task->name;
    			mode_change = 1;
    			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
    		}
		}
		else if(!setup.aux4)//检测到没有麦克风
		{ 
			ew_puts("sys_mic_polling1111\n");
			ew_printf("mode_change:%d\n",mode_change);
			if(mode_change)
			{
				mode_change = 0;
				AUDIO_MUTE_ON();
				task_switch(setup.save_name,0,SWITCH_SPEC_TASK);
			}
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
void amp_control_polling(void)
{
#if defined(_DISABLE_USB_OFF_AMP_)&&0
	if(global.sys_power)
	{
		if(global.amp_on)
		{
			AMP_CONTROL_ON();
		}
	}
#else
	if(global.amp_on)
	{
		global.amp_time++;
		if(global.amp_time == 1000/50)// 1秒
		{
			AMP_CONTROL_ON();
			global.amp_on = FALSE;
			global.amp_time = 0;
		}
	}
#endif
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_IPHONE_SIR_)
void sir_mode_polling(void)
{
	if(global.sir_mode1)
	{
		if(!global.sir_flag)
		{
			task_switch(global.name,0,SWITCH_SPEC_TASK);
			global.sir_mode1 = FALSE;
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SL6812_ILL_LIGHT_)
void ill_light_polling(void)
{
	if(ACC_CHECK())
	{
		if(ILL_DETECT_CHECK())
		{
			ILL_LIGHT_ON();
		}
		else
		{
			ILL_LIGHT_OFF();
		}
	}

	/*if(global.sys_power)
	{
		if(ILL_DETECT_CHECK())
		{
			ILL_LIGHT_ON();
		}
		else
		{
			ILL_LIGHT_OFF();
		}
	}*/
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_KEY_LIGHT_CONTROL_)
void backlight_polling(void)
{
	//if((global.sys_power)&&(ACC_CHECK()))
	//if(ACC_CHECK())
	{
		if(setup.light_flg)
		{
			SYS_LIGHT_ON();
		}
		else
		{
			SYS_LIGHT_OFF();
		}
	}
}
#endif

#if defined(_SH1302_AKIN_OUTPUT_PIN)
void sys_akinoutput_polling(void)
{
    if(compare_task_name("LineinTask"))
    {
        AKIN_OUTPUT_ON();
	}
	else
	{
        AKIN_OUTPUT_OFF();
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_OPTICAL_CONTROL_)
void sys_optical_polling(void)
{
	if((compare_task_name("MusicTaskUsb"))
	||(compare_task_name("FMRadioTask"))
	||(compare_task_name("BtUserTask")))
	{
		setup.optical = FALSE;
		setup.mode_start = 0;
	}

	if(setup.mode_start == 1)
	{
		setup.optical = TRUE;
	}
	else
	{
		setup.optical = FALSE;
	}

	if(setup.optical)
	{
		SYS_OPTICAL_ON();
	}
	else
	{
		SYS_OPTICAL_OFF();
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_SWL_ENABLE_DISK_DISC_TO_FLASH_)
uint8 DiscTime =0;
uint8 DiscState=0;

void DiscPolling(void)
{
    if(DiscTime)DiscTime--;
    else
	{
		DiscTime = 333/DISP_POLLING_TIME;
		switch(DiscState)
		{
			case 0:
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
				DiscState = 1;
				break;
			case 1:
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
				DiscState = 2;
				break;
			case 2:
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
				DiscState = 0;
				break;
		}
	}
}
#endif
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_BT_VOL_)
void bt_volume_polling(void)
{
	if((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
	||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
	||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
	||(get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)
	||((get_bt_newest_status() == BT_STATUS_RESUME_BTSTACK)&&(get_call_status() != BT_CALL_HANGUP))
	||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
	||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
    {
		//ew_puts("setup.first_power\n");

	#if defined(_ENABLE_TALKING_VOL_)
		global.bt_call = TRUE;
		if(!setup.first_power)
		{
			setup.first_power = TRUE;
			audio_drv_ctrl(AUD_CTRL_VOLUME, setup.bt_vol, TRUE);
		}
		else
		{
			if(!setup.bt_vol_flg)
			{
				setup.bt_vol_flg = TRUE;
				audio_drv_ctrl(AUD_CTRL_VOLUME, setup.bt_vol, TRUE);
			}
		}
	#else
		global.bt_call = TRUE;

		//ew_printf("setup.aud_vol:%d\n",setup.aud_vol);
		//ew_printf("setup.bt_vol:%d\n",setup.bt_vol);

    #if defined(_AUDIO_USE_DECODER_)
    	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
    #else
    	audio_drv_ctrl(AUD_CTRL_VOLUME, setup.bt_vol, TRUE);
    #endif
	#endif
	}
	else
	{
    #if defined(_ENABLE_TALKING_VOL_)
    	global.bt_call = FALSE;
    	if(setup.bt_vol_flg)
    	{
    		setup.bt_vol_flg = FALSE;
    	}
    #else
    	global.bt_call = FALSE;

	#if defined(_AUDIO_USE_DECODER_)
		setup.bt_vol = get_cur_vol();
	#else
		setup.bt_vol = BT_TALK_VOL;
	#endif
    #endif

	//ew_puts("setup.first_power1111\n");

    #if defined(_ENABLE_RADIO_RDS_)
    #if defined(_ENABLE_RDS_VOLUME_)
    	audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
    #else
    	if(global.rds_ta)
    	{
    		audio_drv_ctrl(AUD_CTRL_VOLUME, setup.rds.volume, TRUE);
    	}
    	else
    	{
    		audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
    	}
    #endif
    #else
    	//ew_printf("setup.aud_vol1111:%d\n",setup.aud_vol);
    	audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
    #endif
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_DISP_BT_NAME_)
void disp_bt_name(void)
{
	uint8 dispchar[4];

    memset(dispchar, ' ', sizeof(dispchar));
    if(setup.save_number < 10)
    {
    	dispchar[1] = setup.save_number + '0';
    }
    else if((setup.save_number > 9)&&(setup.save_number < 100))
    {
    	dispchar[1] = setup.save_number/10 + '0';
    	dispchar[2] = (setup.save_number/10)%10 + '0';
    }
    else if((setup.save_number > 99)&&(setup.save_number < 1000))
    {
    	dispchar[0] = setup.save_number/100 + '0';
    	dispchar[1] = (setup.save_number%100)/10 + '0';
    	dispchar[2] = (setup.save_number%100)%10 + '0';
    }
    else if((setup.save_number > 999)&&(setup.save_number < 10000))
    {
    	dispchar[0] = (setup.save_number/100)/10 + '0';
    	dispchar[1] = (setup.save_number/100)%10 + '0';
    	dispchar[2] = (setup.save_number%100)/10 + '0';
    	dispchar[3] = (setup.save_number%100)%10 + '0';
    }

    //show_clear_icon(DISP_LEVEL_SETTING);
	ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_CLOCK);
    ui_show_string(DISP_LEVEL_SETTING, dispchar);
    ui_disp.now_level = DISP_LEVEL_SETTING;
    ui_disp.timer = 3000;
}
#endif
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_CHANGE_VBRE_DISP_)
void radio_display_polling(void)
{
	if(compare_task_name("FMRadioTask"))
    {
	   ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_VBAR_1);
	   ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_VBAR_2);
	   ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_VBAR_3);
	}
	else if((compare_task_name("LineinTask"))||(compare_task_name("BtUserTask")))
    {
	   ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_1);
	   ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_2);
	   ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_3);
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------+------*/
#if defined(_ENABLE_BUG_DETECT_)
#if 1//任何时候，故障检测脚为低时就MUTE功放
void sys_bug_polling(void)
{
	global.bug_time++;
	if(global.bug_time > 5000/DISP_POLLING_TIME)//开机前5秒 不要做音量减1的操作
	{
		if(BUG_CHECK())
		{
			ew_puts("BUG_CHECK111\n");
			AUDIO_MUTE_ON();
			global.bug_flag = TRUE;
			if(global.bug_flag1)
			{
				ew_puts("BUG_CHECK333\n");
				if(setup.aud_vol > 30)
				{
					ew_puts("BUG_CHECK444\n");
					setup.aud_vol = (setup.aud_vol-1);
					audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
				}
				global.bug_flag1 = FALSE;
			}
		}
		else
		{
			//ew_puts("BUG_CHECK222\n");
			global.bug_flag1 = TRUE;
			if(global.bug_flag)
			{
				ew_puts("BUG_CHECK555\n");
				AUDIO_MUTE_OFF();
				global.bug_flag = FALSE;
			}
		}
		
		if(global.bug_time == 1000)
		{
			global.bug_time = 150;
		}
	}
}
#else
void sys_bug_polling(void)
{
	global.bug_time++;
	if(global.bug_time > 30000/DISP_POLLING_TIME)//开机前30秒 不要做音量减1的操作
	{
		if(BUG_CHECK())
		{
			//if((global.bug_flag)&&(!global.bug_flag))
			if(global.bug_flag)
			{
				if(setup.aud_vol > 30)
				{
					setup.aud_vol = (setup.aud_vol-1);
					audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
				}
				global.bug_flag = FALSE;
			}
		}
		else
		{
			global.bug_flag = TRUE;
		}

		if(global.bug_time < 9000)
		{
			global.bug_time = 610;
		}
	}
}
#endif
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------+------*/
#if defined(_ENABLE_POWER_BT_OFF_)
static uint8 bt_connect_state1(void)
{
	switch(get_bt_connect_status())
	{
		case BT_STATUS_CONNECTING:
		case BT_STATUS_TAKEING_PHONE:
		case BT_STATUS_PLAYING_MUSIC:
			return 1;
	}
	return 0;
}

void bt_scan_polling(void)
{
	if(global.sys_power)
	{
		if(bt_connect_state1())
		{
			global.bt_off_flag = TRUE;
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
			global.connect_flag = FALSE;
		}
	}
	else
	{
		if(bt_connect_state1())
		{
			user_send_cmd_prepare(USER_CTRL_DISCONNECTION_HCI,0,NULL);
			global.connect_flag = TRUE;
			global.bt_connect_time = 0;
			//os_time_dly(10);
		}
		user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
		user_send_cmd_prepare(USER_CTRL_WRITE_CONN_DISABLE,0,NULL);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------+------*/
#if defined(_ENABLE_BT_OFF_)
void bt_scan_polling(void)
{
	if(setup.bt_scan_flag)
	{
		if(!setup.bt_first_scan)
		{
			setup.bt_first_scan = 1;
			user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
			user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE,0,NULL);
		}
	}
	else
	{
		user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
		user_send_cmd_prepare(USER_CTRL_WRITE_CONN_DISABLE,0,NULL);
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------+------*/
#if defined(_ENABLE_SUB_DETECT_)
void ui_show_sub(void)
{
	if(global.sub_flg)
	{
		ui_show_setting(LANG_ID_OFF, 0);
	}
	else
	{
		ui_show_setting(LANG_ID_ON, 0);
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ACC_OFF_POWER_LOGO_)
void acc_off_logo_polling(void)
{
	ew_puts("ui_show_power_off_polling---1111----\n");
	switch(global.disp_ctr1.status)
	{
		case 1:
			{
		    	const uint8 disp_length = 8;
				uint8 dips_buffer[disp_length + 1];
				uint8 str_length;
				uint8 index;
				const char *disp_char = _SPECIAL_POWER_OFF_DISPLAY_;
				
				str_length = strlen(disp_char);
				ew_puts("ui_show_power_off_polling---222----\n");
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
				
				ew_printf("str_length:%d, disp_length:%d\n", str_length, disp_length);
				
				if(str_length > disp_length)
				{
					global.disp_ctr1.timer = 0;
					global.disp_ctr1.point = 0;
					global.disp_ctr1.step = str_length - disp_length + 0;
					global.disp_ctr1.status ++;
				}
				else
				{
					global.disp_ctr1.status = 0;
				}
			}
			break;
			
		case 2:
			{
				ew_puts("ui_show_power_off_polling---333----\n");
				if((++global.disp_ctr1.timer) >= (100/100))// 800
				{
					global.disp_ctr1.timer = 0;
					global.disp_ctr1.status++;
				}
			}
			break;
			
		case 3:
			if((++global.disp_ctr1.timer) >= (500/100))
			{
				global.disp_ctr1.timer = 0;
				if((++global.disp_ctr1.point) <= global.disp_ctr1.step)
				{
					const uint8 disp_length = 8;
					uint8 dips_buffer[disp_length + 1];
					uint8 str_length;
					uint8 index;
					const char *disp_char = _SPECIAL_POWER_OFF_DISPLAY_;
					
					str_length = strlen(disp_char) - global.disp_ctr1.point;
					for(index = 0; index < disp_length; index++)
					{
						if(index < str_length)
						{
							dips_buffer[index] = disp_char[global.disp_ctr1.point + index];
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
					global.disp_ctr1.status++;
				}
			}
			break;
			
		case 4:
			ew_puts("ui_show_power_off_polling---4444----\n");
			global.disp_ctr1.status = 0;
			
    	#if defined(_ACC_OFF_POWER_LOGO_)
    		if(setup.acc_logo)
    		{
    			ew_puts("ui_show_power_off_polling---5555----\n");
                AUDIO_MUTE_ON();
		    #if defined(_ENABLE_RCA_MUTE_)
		    	RCA_MUTE_ON();
		    #endif
                sys_mute_ctrl(ENABLE, 0);
    			setup.acc_logo = FALSE;
		    #if defined(_ENABLE_AMP_LIGHT_)
		    	AMP_CONTROL_OFF();
		    #endif
    			os_taskq_post_msg(MAIN_TASK_NAME, 1, SYS_EVENT_ACC_OFF);
    		}
    	#endif
			break;
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_IR_WITH_ACC_DELAY_)
void acc_off_polling(void)
{
	if(!ACC_CHECK())
	{
		global.acc_off_delay++;
		if((global.acc_off_delay == 30)&&(!global.acc_ir_flag))
		{
			global.acc_off_delay = 0;
			global.acc_ir_flag = TRUE;
        #if defined(_ACC_OFF_POWER_LOGO_)
        	setup.acc_logo = TRUE;
        	global.disp_ctr1.status = 1;
        	global.bt_off = TRUE;
        #else
        #if defined(_ENABLE_MUTE_FUNTION_)
        	FEIYIN_MUTE_ON();
        #endif
        #if defined(_ENABLE_AMP_LIGHT_)
		#if defined(_ACC_OFF_SUB_)
    		if(setup.aud_sub)
    		{
    			SYS_SUB_OFF();
    		}
    		if(setup.aud_sub1)
    		{
    			SYS_SUB2_OFF();
    		}
		#else
        	AMP_CONTROL_OFF();
        #endif
		#endif
		
	    #if defined(_DSP_RCA_MUTE_)//关ACC 不要操作RCA MUTE
	    	//DSP_RCA_MUTE_ON();
	    #endif
        	os_taskq_post_msg(MAIN_TASK_NAME, 1, SYS_EVENT_ACC_OFF);
        #endif
		}
		
		if(global.acc_off_delay == 50)
		{
			global.acc_off_delay = 30;
		}
	}
	else
	{
		global.acc_off_delay = 0;
		global.acc_ir_flag = FALSE;
	}
}
#endif

#if defined(_ACC_OFF_DELAY_TIME_)
void acc_off_polling(void)
{
	if(!ACC_CHECK())
	{
		global.acc_off_delay++;
		if(global.acc_off_delay == 50)//2.5S
		{
			global.acc_off_delay = 70;
        	os_taskq_post_msg(MAIN_TASK_NAME, 1, SYS_EVENT_ACC_OFF);
		}
		
		if(global.acc_off_delay == 100)
		{
			global.acc_off_delay = 60;
		}
	}
	else
	{
		global.acc_off_delay = 0;
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_RDS_FIRST_)
void rds_mode_polling(void)
{
	if((global.rds_mode)&&(!global.rds_ta)&&((get_bt_newest_status() == BT_STATUS_PHONE_INCOME)
	||(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
	||(get_bt_newest_status() == BT_STATUS_PHONE_OUT)
	||(get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)
	||((get_call_status() == BT_CALL_OUTGOING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
	||((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))))
	{
		global.rds_mode = FALSE;
		task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_POWER_ON_PASSWORD_)

#define PASSWORD_SIZE			4
uint8 power_password[PASSWORD_SIZE];
static uint8 user_password[PASSWORD_SIZE] = "1111";

void password_disp_polling(void)
{
	uint8 dispchar[8];

	if(setup.password_flag)
	{
		ew_puts("password_disp_polling\n");

		memset(dispchar, ' ', sizeof(dispchar));
		dispchar[0] = 'P';
		dispchar[1] = 'W';
		dispchar[2] = 'D';
		dispchar[3] = ' ';

		setup.password_input = (setup.password_input*10)%10000;
		setup.password_input = setup.password_input + setup.password_number;

		ew_printf("setup.password_number:%d\n",setup.password_number);
		ew_printf("setup.password_input:%d\n",setup.password_input);

		if(setup.password_input == 0)
		{
			dispchar[4] = '-';
			dispchar[5] = '-';
			dispchar[6] = '-';
			dispchar[7] = '-';
		}
		else if((setup.password_input < 10)&&(setup.password_input > 0))
		{
			dispchar[4] = setup.password_input + '0';
			dispchar[5] = '-';
			dispchar[6] = '-';
			dispchar[7] = '-';
		}
		else if((setup.password_input < 100)&&(setup.password_input > 10))
		{
			dispchar[4] = setup.password_input/10 + '0';
			dispchar[5] = (setup.password_input%10)%10 + '0';
			dispchar[6] = '-';
			dispchar[7] = '-';
		}
		else if((setup.password_input < 1000)&&(setup.password_input > 100))
		{
			dispchar[4] = setup.password_input/100 + '0';
			dispchar[5] = (setup.password_input%100)/10 + '0';
			dispchar[6] = (setup.password_input%100)%10 + '0';
			dispchar[7] = '-';
		}
		else
		{
			dispchar[4] = (setup.password_input/100)/10 + '0';
			dispchar[5] = (setup.password_input/100)%10 + '0';
			dispchar[6] = (setup.password_input%100)/10 + '0';
			dispchar[7] = (setup.password_input%100)%10 + '0';
		}
		ui_show_string(DISP_LEVEL_NORMAL, dispchar);
	}
}

void password_input(pwd_number number)
{
	uint8 index;
	uint8 password_number;

	ew_puts("password_input\n");
	password_number = number + '0';
	setup.password_number = number;

	for(index = 0; index < (PASSWORD_SIZE - 1); index++)
	{
		power_password[index] = power_password[index + 1];
	}

	power_password[index] = password_number;
	password_disp_polling();
}

void password_control_number1(void)
{
	ew_puts("password_control_number1\n");
	if(setup.first_power_flag)
	{
		return password_input(PWD_NUMBER_1);
	}
}

void password_control_number2(void)
{
	ew_puts("password_control_number2\n");
	if(setup.first_power_flag)
	{
		return password_input(PWD_NUMBER_2);
	}
}

void password_control_number3(void)
{
	ew_puts("password_control_number3\n");
	if(setup.first_power_flag)
	{
		return password_input(PWD_NUMBER_3);
	}
}

void password_control_number4(void)
{
	ew_puts("password_control_number4\n");
	if(setup.first_power_flag)
	{
		return password_input(PWD_NUMBER_4);
	}
}

void password_control_number5(void)
{
	ew_puts("password_control_number5\n");
	if(setup.first_power_flag)
	{
		return password_input(PWD_NUMBER_5);
	}
}

void password_control_number6(void)
{
	ew_puts("password_control_number6\n");
	if(setup.first_power_flag)
	{
		return password_input(PWD_NUMBER_6);
	}
}

void password_control_enter(void)
{
	if(setup.first_power_flag)
	{
		if(!memcmp(power_password, user_password, PASSWORD_SIZE))
		{
			memset(power_password, 0, sizeof(power_password));
			os_taskq_post_msg("IdleTask", 1, MSG_IDLE_EXIT);
		}
		else
		{
			setup.password_input = 0;
			setup.password_number = 0;
			ui_show_setting(LANG_ID_ERROR, 0);
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_PASSWORD_DISPLAY_)
#define PASSWORD_SIZE			4
uint8 power_password[PASSWORD_SIZE];
static uint8 user_password[PASSWORD_SIZE] = "1111";

#if defined(_ENABLE_PASSWORD_SET_)
uint8 pwd_set1[PASSWORD_SIZE];
uint8 pwd_set2[PASSWORD_SIZE];
#endif

uint8 password_number1=10;
uint8 password_number2=10;
uint8 password_number3=10;
uint8 password_number4=10;

#if defined(_PASSWORD_INPUT_BLINK_)
uint8 blink_time=0;
uint8 input_number=1;
#endif


#if defined(_SL6812A_SPECIAL_DISPLAY_)
void password_disp_polling(void)
{
	uint8 dispchar[10];

	if(setup.password_flag)
	{
		//ew_puts("password_disp_polling\n");

		memset(dispchar, ' ', sizeof(dispchar));
		dispchar[0] = ' ';
		dispchar[1] = 'P';
		dispchar[2] = 'W';
		dispchar[3] = 'D';
		dispchar[4] = ' ';
		dispchar[9] = ' ';

    #if defined(_PASSWORD_INPUT_BLINK_)
		if(++blink_time <= (700/DISP_POLLING_TIME))
		{
			dispchar[5] = password_number1 + '0';
			dispchar[6] = password_number2 + '0';
			dispchar[7] = password_number3 + '0';
			dispchar[8] = password_number4 + '0';
			
			if(password_number1 == 10)
			{
				if(setup.pwd_set)
				{
					dispchar[5] = '-';
				}
				else
				{
					dispchar[5] = '*';
				}
			}
		}
		else if((++blink_time <= (1800/DISP_POLLING_TIME))&&(++blink_time > (700/DISP_POLLING_TIME)))
		{
			if(setup.pwd_set)
			{
				dispchar[5] = password_number1 + '0';
				dispchar[6] = password_number2 + '0';
				dispchar[7] = password_number3 + '0';
				dispchar[8] = password_number4 + '0';
			}
			else
			{
				if(input_number == 1)
				{
					dispchar[5] = ' ';
					dispchar[6] = password_number2 + '0';
					dispchar[7] = password_number3 + '0';
					dispchar[8] = password_number4 + '0';
				}
				else if(input_number == 2)
				{
					dispchar[5] = password_number1 + '0';
					dispchar[6] = ' ';
					dispchar[7] = password_number3 + '0';
					dispchar[8] = password_number4 + '0';

				}
				else if(input_number == 3)
				{
					dispchar[5] = password_number1 + '0';
					dispchar[6] = password_number2 + '0';
					dispchar[7] = ' ';
					dispchar[8] = password_number4 + '0';

				}
				else if(input_number == 4)
				{
					dispchar[5] = password_number1 + '0';
					dispchar[6] = password_number2 + '0';
					dispchar[7] = password_number3 + '0';
					dispchar[8] = ' ';
				}
			}
			
			if(password_number1 == 10)
			{
				if(setup.pwd_set)
				{
					dispchar[5] = '-';
				}
				else
				{
					if(input_number == 1)
					{
						dispchar[5] = ' ';
					}
					else
					{
						dispchar[5] = '*';
					}
				}
			}
		}
		else
		{
			blink_time = 0;
			dispchar[5] = password_number1 + '0';
			dispchar[6] = password_number2 + '0';
			dispchar[7] = password_number3 + '0';
			dispchar[8] = password_number4 + '0';
			if(password_number1 == 10)
			{
				if(setup.pwd_set)
				{
					dispchar[5] = '-';
				}
				else
				{
					dispchar[5] = '*';
				}
			}
		}
    #else
    	dispchar[5] = password_number1 + '0';
    	dispchar[6] = password_number2 + '0';
    	dispchar[7] = password_number3 + '0';
    	dispchar[8] = password_number4 + '0';
    
    	if(password_number1 == 10)
    	{
    	    if(setup.pwd_set)
    	    {
    			dispchar[5] = '-';
    		}
    		else
    		{
    			dispchar[5] = '*';
    		}
    	}
    #endif

		if(password_number2 == 10)
		{
		    if(setup.pwd_set)
		    {
				dispchar[6] = '-';
			}
			else
			{
				dispchar[6] = '*';
			}
		}
		if(password_number3 == 10)
		{
		    if(setup.pwd_set)
		    {
				dispchar[7] = '-';
			}
			else
			{
				dispchar[7] = '*';
			}
		}
		if(password_number4 == 10)
		{
		    if(setup.pwd_set)
		    {
				dispchar[8] = '-';
			}
			else
			{
				dispchar[8] = '*';
			}
		}
		ui_show_string(DISP_LEVEL_NORMAL, dispchar);
	}
}

#else
void password_disp_polling(void)
{
	uint8 dispchar[8];

	if(setup.password_flag)
	{
		//ew_puts("password_disp_polling\n");

		memset(dispchar, ' ', sizeof(dispchar));
		dispchar[0] = 'P';
		dispchar[1] = 'W';
		dispchar[2] = 'D';
		dispchar[3] = ' ';

		dispchar[4] = password_number1 + '0';
		dispchar[5] = password_number2 + '0';
		dispchar[6] = password_number3 + '0';
		dispchar[7] = password_number4 + '0';

		if(password_number1 == 10)
		{
		    if(setup.pwd_set)
		    {
				dispchar[4] = '-';
			}
			else
			{
				dispchar[4] = '*';
			}
		}
		if(password_number2 == 10)
		{
		    if(setup.pwd_set)
		    {
				dispchar[5] = '-';
			}
			else
			{
				dispchar[5] = '*';
			}
		}
		if(password_number3 == 10)
		{
		    if(setup.pwd_set)
		    {
				dispchar[6] = '-';
			}
			else
			{
				dispchar[6] = '*';
			}
		}
		if(password_number4 == 10)
		{
		    if(setup.pwd_set)
		    {
				dispchar[7] = '-';
			}
			else
			{
				dispchar[7] = '*';
			}
		}
		ui_show_string(DISP_LEVEL_NORMAL, dispchar);
	}
}
#endif

void password_input(pwd_number number,uint8 num)
{
	ew_puts("password_input\n");
	switch(number)
	{
		case PWD_NUMBER_1:
			password_number1 = num;
			power_password[0] = num + '0';
			break;
			
		case PWD_NUMBER_2:
			password_number2 = num;
			power_password[1] = num + '0';
			break;
			
		case PWD_NUMBER_3:
    		password_number3 = num;
    		power_password[2] = num + '0';
			break;
			
		case PWD_NUMBER_4:
    		password_number4 = num;
    		power_password[3] = num + '0';
			break;
	}
	
	if(global.pwd_flag)
	{
		pwd_set2[0] = power_password[0];
		pwd_set2[1] = power_password[1];
		pwd_set2[2] = power_password[2];
		pwd_set2[3] = power_password[3];
	}
	else
	{
		pwd_set1[0] = power_password[0];
		pwd_set1[1] = power_password[1];
		pwd_set1[2] = power_password[2];
		pwd_set1[3] = power_password[3];
	}
	password_disp_polling();
}

uint8 input_num1=0;
uint8 input_num2=0;
uint8 input_num3=0;
uint8 input_num4=0;
void password_control_number1(void)
{
	ew_puts("password_control_number1\n");
	if(setup.first_power_flag)
	{
	#if defined(_PASSWORD_INPUT_BLINK_)
		input_number = 1;
	#endif
		input_num1++;
		if(input_num1 == 10)
		{
			input_num1 = 0;
		}
		return password_input(PWD_NUMBER_1,input_num1);
	}
}

void password_control_number2(void)
{
	ew_puts("password_control_number2\n");
	if(setup.first_power_flag)
	{
    #if defined(_PASSWORD_INPUT_BLINK_)
    	input_number = 2;
    #endif
		input_num2++;
		if(input_num2 == 9)
		{
			input_num2 = 0;
		}
		return password_input(PWD_NUMBER_2,input_num2);
	}
}

void password_control_number3(void)
{
	ew_puts("password_control_number3\n");
	if(setup.first_power_flag)
	{
    #if defined(_PASSWORD_INPUT_BLINK_)
    	input_number = 3;
    #endif
		input_num3++;
		if(input_num3 == 9)
		{
			input_num3 = 0;
		}
		return password_input(PWD_NUMBER_3,input_num3);
	}
}

void password_control_number4(void)
{
	ew_puts("password_control_number4\n");
	if(setup.first_power_flag)
	{
    #if defined(_PASSWORD_INPUT_BLINK_)
    	input_number = 4;
    #endif
		input_num4++;
		if(input_num4 == 9)
		{
			input_num4 = 0;
		}
		return password_input(PWD_NUMBER_4,input_num4);
	}
}

void password_control_number5(void)
{
	ew_puts("password_control_number5\n");
	if(setup.first_power_flag)
	{
	#if !defined(_SUPPORT_PASSWORD_DISPLAY_)
		return password_input(PWD_NUMBER_5);
	#endif
	}
}

void password_control_number6(void)
{
	ew_puts("password_control_number6\n");
	if(setup.first_power_flag)
	{
	#if !defined(_SUPPORT_PASSWORD_DISPLAY_)
		return password_input(PWD_NUMBER_6);
	#endif
	}
}

uint8 err_count=0;
#if defined(_ENABLE_MEMORY_PASSWORD_)
uint8 pwd_set_save[PASSWORD_SIZE];
#endif
void password_control_enter(void)
{
	if(setup.first_power_flag)
	{
		if(setup.pwd_set)
		{
			if((pwd_set1[0] == pwd_set2[0])&&(pwd_set1[1] == pwd_set2[1])&&(pwd_set1[2] == pwd_set2[2])&&(pwd_set1[3] == pwd_set2[3]))
			{
				puts("password_control_enter\n");
			#if defined(_ENABLE_MEMORY_PASSWORD_)
				pwd_set_save[0] = pwd_set1[0];
				pwd_set_save[1] = pwd_set1[1];
				pwd_set_save[2] = pwd_set1[2];
				pwd_set_save[3] = pwd_set1[3];
				vm_write_api(VM_PWD_SET, &pwd_set_save);
			#endif
				ui_show_setting(LANG_ID_PWD_SET, 0);
				global.pwd_flag = FALSE;
				memset(power_password, 0, sizeof(power_password));
				memset(pwd_set1, 0, sizeof(pwd_set1));
				memset(pwd_set2, 0, sizeof(pwd_set2));
				//os_taskq_post_msg("IdleTask", 1, MSG_IDLE_EXIT);
		    #if defined(_ENABLE_PASSWORD_SET_)
				global.pwd_set_ok = TRUE;
		    	setup.pwd_set = FALSE;
				global.pwd_set_flag = FALSE;
		    	password_number1 = 10;
		    	password_number2 = 10;
		    	password_number3 = 10;
		    	password_number4 = 10;
		    	input_num1 = 0;
		    	input_num2 = 0;
		    	input_num3 = 0;
		    	input_num4 = 0;
		    #endif
			}
			else
			{
				puts("password_control_enter111\n");
				global.pwd_flag = !global.pwd_flag;
				memset(power_password, 0, sizeof(power_password));
		    	password_number1 = 10;
		    	password_number2 = 10;
		    	password_number3 = 10;
		    	password_number4 = 10;
		    	input_num1 = 0;
		    	input_num2 = 0;
		    	input_num3 = 0;
		    	input_num4 = 0;
				password_disp_polling();
			}
		}
		else
		{
		#if defined(_ENABLE_MEMORY_PASSWORD_)
			if(!memcmp(power_password, pwd_set_save, PASSWORD_SIZE))
		#else
			if(!memcmp(power_password, user_password, PASSWORD_SIZE))
		#endif
			{
				memset(power_password, 0, sizeof(power_password));
			#if defined(_PASSWORD_TRUE_DISPLAY_)
				global.pwd_set_true = TRUE;
				ui_show_setting(LANG_ID_PWD_CORRECT, 0);
			#else
				os_taskq_post_msg("IdleTask", 1, MSG_IDLE_EXIT);
			#endif
			
			#if defined(_ENABLE_PASSWORD_SET_)
				global.pwd_set_flag = FALSE;
				setup.pwd_set = FALSE;
				password_number1 = 10;
				password_number2 = 10;
				password_number3 = 10;
				password_number4 = 10;
				input_num1 = 0;
				input_num2 = 0;
				input_num3 = 0;
				input_num4 = 0;
			#endif
			}
			else
			{
				password_number1 = 10;
				password_number2 = 10;
				password_number3 = 10;
				password_number4 = 10;
				input_num1 = 0;
				input_num2 = 0;
				input_num3 = 0;
				input_num4 = 0;
				err_count++;
				setup.first_power_flag = FALSE;
				global.err_flag = TRUE;
    	    #if defined(_PASSWORD_INPUT_BLINK_)
    	    	input_number = 1;
    	    #endif
				if(err_count == 1)
				{
					ui_show_setting(LANG_ID_ERROR1, 0);
					ui_disp.timer = 3000;//3秒
				}
				else if(err_count == 2)
				{
					ui_show_setting(LANG_ID_ERROR2, 0);
					ui_disp.timer = 30000;//30秒
				}
				else if(err_count == 3)
				{
					ui_show_setting(LANG_ID_ERROR3, 0);
					ui_disp.timer = 1000000;//10分钟
				}
				else if(err_count == 4)
				{
					ui_show_setting(LANG_ID_ERROR4, 0);
					ui_disp.timer = 6000000;//1小时
				}
				else if(err_count == 5)
				{
					ui_show_setting(LANG_ID_ERROR5, 0);
					ui_disp.timer = 60000000;//10小时
				}
			}
		}
	}
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_AUX_VER_VIEW_)
static uint8 version_mouth_to_number1[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static uint8 version_info_date1[] = __DATE__;
static uint8 version_info_time1[] = __TIME__;

void disp_ver_view(void)
{
	uint8 dispchar[8];
	uint8 index;

	memset(dispchar, ' ', sizeof(dispchar));

	for(index = 0; index < 12; index++)
	{
		if(!memcmp(version_info_date1, version_mouth_to_number1[index], 3))
		{
			break;
		}
	}

	if(index < 12)
	{
		index++;
	}
	else
	{
		index = 0;
	}

	dispchar[0] = index/10 + '0';
	dispchar[1] = index%10 + '0';
	dispchar[2] = (version_info_date1[4]== ' ')? '0' : version_info_date1[4];
	dispchar[3] = (version_info_date1[5]== ' ')? '0' : version_info_date1[5];
	dispchar[4] = version_info_time1[0];
	dispchar[5] = version_info_time1[1];
	dispchar[6] = version_info_time1[3];
	dispchar[7] = version_info_time1[4];
	ui_show_string(DISP_LEVEL_SETTING, dispchar);
	ui_disp.now_level = DISP_LEVEL_SETTING;
	ui_disp.timer = 5000;
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

static void disp_type_exit(uint8 type)
{
	disp_printf("%s, disp_type:%d\n", __FUNCTION__, type);

#if defined(_ENABLE_VBAR_)
	ui_show_vbar(DISABLE, 0);
#endif

#if defined(_SUPPORT_LCM_DRIVE)
    Clr_Lcm_Disp();
#endif
	switch(type)
	{
		case DISP_TYPE_IDLE:
			ui_show_idle_exit();
			break;

		case DISP_TYPE_RADIO:
			ui_show_radio_exit();
			break;
    #if defined(DAB_MODLE)
		case DISP_TYPE_DAB:
			ui_show_dab_exit();
			break;
    #endif
	#if defined(_SUPPORT_TAPE_)
    	case DISP_TYPE_TAPE:
    		ui_show_tape_exit();
    		break;
	#endif
    #if defined(_SUPPORT_MIC_OK_)
    	case DISP_TYPE_MIC:
    		ui_show_mic_exit();
    		break;
    #endif
    #if defined(_SUPPORT_CAN_MODEL_)
    	case DISP_TYPE_CAN:
    		ui_show_can_exit();
    		break;
    #endif
	
    #if defined(_SUPPORT_REVERSE_MODEL_)
    	case DISP_TYPE_REVE:
    		ui_show_reverse_exit();
    		break;
    #endif
		case DISP_TYPE_USB:
		case DISP_TYPE_SD:
			ui_show_play_exit();
			break;

    #if defined(_SUPPORT_DISC_)
    	case DISP_TYPE_PLAYER_DISC:
    	case DISP_TYPE_PLAYER_USB:
    	case DISP_TYPE_PLAYER_CARD:
    		ui_show_disc_exit();
    		break;
    #endif

		case DISP_TYPE_BT:
			ui_show_bt_exit();
			break;

		case DISP_TYPE_AUX:
			ui_show_aux_exit();
			break;
        #if defined(_SUPPORT_AUX1_)
		case DISP_TYPE_AUX1:
			ui_show_aux1_exit();
			break;
        #endif
			#if REC_EN
		case DISP_TYPE_REC:
			ui_show_rec_exit();
			break;
			#endif

		default:
			break;
	}
}

static void disp_type_init(uint8 type)
{
	disp_printf("%s, disp_type:%d\n", __FUNCTION__, type);

	disp_func_reset();
#if defined( _SUPPORT_LCM_1618_DRIVE )
	if(type!=DISP_TYPE_IDLE)
	{
		enable_disp_set_icom=1;
	}
else
	{
		enable_disp_set_icom=0;
	}
#endif

	switch(type)
	{
		case DISP_TYPE_IDLE:
			ui_show_idle_init();
			disp_send_msg(DISP_EVENT_REFUR);
			break;

		case DISP_TYPE_RADIO:
			ui_show_radio_init();
			disp_send_msg(DISP_EVENT_REFUR);
			break;
	#if defined(DAB_MODLE)
		case DISP_TYPE_DAB:
			ui_show_dab_init();
			disp_send_msg(DISP_EVENT_REFUR);
			break;
    #endif
	#if defined(_SUPPORT_TAPE_)
		case DISP_TYPE_TAPE:
			ui_show_tape_init();
			disp_send_msg(DISP_EVENT_REFUR);
			break;
	#endif
    #if defined(_SUPPORT_MIC_OK_)
    	case DISP_TYPE_MIC:
    		ui_show_mic_init();
    		disp_send_msg(DISP_EVENT_REFUR);
    		break;
    #endif
    #if defined(_SUPPORT_CAN_MODEL_)
    	case DISP_TYPE_CAN:
    		ui_show_can_init();
    		disp_send_msg(DISP_EVENT_REFUR);
    		break;
    #endif
	
    #if defined(_SUPPORT_REVERSE_MODEL_)
    	case DISP_TYPE_REVE:
    		ui_show_reverse_init();
    		disp_send_msg(DISP_EVENT_REFUR);
    		break;
    #endif
		case DISP_TYPE_USB:
			ui_show_play_init(1);
			disp_send_msg(DISP_EVENT_REFUR);
			break;

		case DISP_TYPE_SD:
			ui_show_play_init(2);
			disp_send_msg(DISP_EVENT_REFUR);
			break;

    #if defined(_SUPPORT_DISC_)
    	case DISP_TYPE_PLAYER_DISC:
    		ui_show_disc_init(1);
    		disp_send_msg(DISP_EVENT_REFUR);
    		break;

    	case DISP_TYPE_PLAYER_USB:
    		ui_show_disc_init(2);
    		disp_send_msg(DISP_EVENT_REFUR);
    		break;

    	case DISP_TYPE_PLAYER_CARD:
    		ui_show_disc_init(3);
    		disp_send_msg(DISP_EVENT_REFUR);
    		break;
    #endif

		case DISP_TYPE_BT:
			ui_show_bt_init();
			disp_send_msg(DISP_EVENT_REFUR);
			break;

		case DISP_TYPE_AUX:
			ui_show_aux_init();
			disp_send_msg(DISP_EVENT_REFUR);
			break;
    #if defined(_SUPPORT_AUX1_)
		case DISP_TYPE_AUX1:
			ui_show_aux1_init();
			disp_send_msg(DISP_EVENT_REFUR);
			break;
    #endif		
			#if REC_EN
		case DISP_TYPE_REC:
			ui_show_rec_init();
			disp_send_msg(DISP_EVENT_REFUR);
			break;
			#endif
		default:
			break;
	}
}

static void disp_type_polling(uint8 type)
{
#if defined(_FY7555_VBAR_DISPLAY_)&&0
	if(type != DISP_TYPE_IDLE)
	{
		for(uint8 i = 0; i < 91; i++)
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_L1P+i);
		}
	}
#endif

	switch(type)
	{
		case DISP_TYPE_IDLE:
		#if defined(_SPECIAL_LOGO_BUG_)
			if(global.logo_flag)
			{
			#if defined(_SPECIAL_LOGO_DISPLAY_)||defined(_FY7555_LOGO_DISPLAY_)
				ui_show_idle_polling();
			#endif
			}
        #if defined(_SPECIAL_POWER_OFF_DISPLAY_)
        	else
        	{
        		ui_show_idle_polling1();
        	}
        #endif

		#else
		
			if(global.sys_power)
			{
			#if defined(_SPECIAL_LOGO_DISPLAY_)||defined(_FY7555_LOGO_DISPLAY_)
				ui_show_idle_polling();
			#endif
			#if defined(_SUPPORT_LCM_DRIVE)
				lcm_disp_clock();
			#endif
			}
	    #if defined(_SPECIAL_POWER_OFF_DISPLAY_)
	    	else
	    	{
	    		ui_show_idle_polling1();
	    	}
	    #endif
		#endif
			break;

		case DISP_TYPE_RADIO:
			ui_show_radio_polling();
		#if defined(_SUPPORT_LCM_DRIVE)
            if(!lcm_disp_audio())
            {
				lcm_disp_radio();
			}
		#endif
			break;
    #if defined(DAB_MODLE)
		case DISP_TYPE_DAB:
			ui_show_dab_polling();
			break;
    #endif

	#if defined(_SUPPORT_TAPE_)
		case DISP_TYPE_TAPE:
			ui_show_tape_polling();
			break;
	#endif
	
    #if defined(_SUPPORT_MIC_OK_)
    	case DISP_TYPE_MIC:
    		ui_show_mic_polling();
    		break;
    #endif
	
    #if defined(_SUPPORT_CAN_MODEL_)
    	case DISP_TYPE_CAN:
    		ui_show_can_polling();
    		break;
    #endif
    #if defined(_SUPPORT_REVERSE_MODEL_)
    	case DISP_TYPE_REVE:
    		ui_show_reverse_polling();
    		break;
    #endif
		case DISP_TYPE_USB:
		case DISP_TYPE_SD:
			ui_show_play_polling();
		#if defined(_SUPPORT_LCM_DRIVE)
            if(!lcm_disp_audio())
            {
            	lcm_disp_music();
            }
		#endif
			break;

    #if defined(_SUPPORT_DISC_)
    	case DISP_TYPE_PLAYER_DISC:
    	case DISP_TYPE_PLAYER_USB:
    	case DISP_TYPE_PLAYER_CARD:
    		ui_show_disc_polling();
    		break;
    #endif

		case DISP_TYPE_BT:
			ui_show_bt_polling();
		#if defined(_SUPPORT_LCM_DRIVE)
            if(!lcm_disp_audio())
            {
            	lcm_disp_bt();
            }
		#endif
			break;

		case DISP_TYPE_AUX:
			ui_show_aux_polling();
		#if defined(_SUPPORT_LCM_DRIVE)
            if(!lcm_disp_audio())
            {
            	lcm_disp_aux();
            }
		#endif
            break;
    #if defined(_SUPPORT_AUX1_)
		case DISP_TYPE_AUX1:
			ui_show_aux1_polling();
			break;
    #endif
	#if REC_EN
		case DISP_TYPE_REC:
			ui_show_rec_polling();
			break;
	#endif
		default:
			break;
	}
}

static void disp_timer(void *param)
{
	os_taskq_post_msg(DISP_TASK_NAME, 1, DISP_EVENT_REFUR);
}
#if defined(HuiNuo_6651D_Copy)
u8 disp_once=0;
u8 HuiNuo_disp_type=0;
void huinuo_disp_switch(void)
{
	ui_show_setting(LANG_ID_RADIO, 0);
	ui_disp.timer = 2000;
}
#endif

#ifdef	TD_ADD_NEW_APP
extern void MusicListPackSend(void);
extern void BtComSendPoll(void);
extern void compare_play_mode(void);
#endif

static void disp_task(void *p_arg)
{
    int msg[3];
	uint8 disp_type = DISP_TYPE_MAX;
	uint8 ap_refur_timer = 0;
#if defined(AP_ENABLE_REALTIME_DIAPLAY_)
	uint8 ap_refur_timer2 = 0;
#endif
    ui_show_enable(ENABLE);
	ui_show_init();
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(disp_timer, NULL, 100);
#else
	timer_reg_func(disp_timer, NULL, DISP_POLLING_TIME);
#endif

#if defined(HuiNuo_6651D_Copy)
	disp_once=1;
	HuiNuo_disp_type=disp_type;
#endif

    while(1)
    {
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
		//disp_printf("disp_start_msg:%x\n",msg[0]);
		#if defined(HuiNuo_6651D_Copy)//惠诺抄机切换模式会显示aead
		if((disp_type!=DISP_TYPE_MAX)&&(disp_type!=DISP_TYPE_IDLE))
		{
			if(disp_once)
			{
				disp_once=0;
				HuiNuo_disp_type=disp_type;
			}
			if(HuiNuo_disp_type !=disp_type)
			{
				HuiNuo_disp_type=disp_type;
				//huinuo_disp_switch();
			}

		}
		#endif
        switch(msg[0])
        {
        	case SYS_EVENT_DEL_TASK:
				{
		            disp_puts("disp ---> get_SYS_EVENT_DEL_TASK\n");
				#if defined(_SUPPORT_LCM_DRIVE)
					Clr_Lcm_Disp();
					LCM_PWR_OFF();
                    LCM_RESET_LOW();
                    LCM_SDA_LOW();
                    LCM_SCL_LOW();
				#endif
		            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
		            {
		            	timer_unreg_func(disp_timer);
		            	if(disp_type != DISP_TYPE_IDLE)
						{
							disp_type_exit(disp_type);
							disp_type = DISP_TYPE_IDLE;
						}
		                ui_show_enable(DISABLE);
		                os_task_del_res_name(OS_TASK_SELF);
		            }
        		}
	            break;

			//-------disp msg--------------
			case DISP_EVENT_REFUR://实时刷新50ms
				{
					sys_mute_polling();

				#if defined(_ENABLE_VBAR_)
					sys_vbar_polling(disp_type);
				#endif
				
				#if defined(_ENABLE_VBAR_BLINK_)
					if(!global.sys_power)
					{
						global.vbar_time = 0;
					}
				#endif
				
                #if defined(_CHANGE_VBRE_DISP_)
				    if(global.sys_power)
				   	{
						radio_display_polling();
				   	}
				#endif
				
					if((++ap_refur_timer) >= (DISP_TYPE_POLLING/DISP_POLLING_TIME))
					{
						ap_refur_timer = 0;

						disp_type_polling(disp_type);
						
				    #if defined(_ENABLE_BT_VOL_)
				    	bt_volume_polling();
				    #endif

						//ew_printf("GET_BATT_DET_DATA:%d\n",GET_BATT_DET_DATA);
					}

				#ifdef	TD_ADD_NEW_APP
				#if !defined(_DISABLE_MUSIC_LIST_)
    				compare_play_mode();
    				MusicListPackSend();
    				BtComSendPoll();
				#endif
				#endif

					ui_show_polling();
				
			    #if defined(_POWER_OFF_SUB_)
			    	if(global.sub_power_flag)
			    	{
			    		if((++global.sub_power_time) >= (1500/DISP_POLLING_TIME))
			    		{
			    			global.sub_power_flag = FALSE;
			    			global.sub_power_time = 0;
							if(setup.aud_sub)
							{
								SYS_SUB_OFF();
							}
							if(setup.aud_sub1)
							{
								SYS_SUB2_OFF();
							}
			    		}
			    	}
			    #endif

				#if defined(_EVB_BT_CONNECT_BT_MODE_)
					connect_bt_mode_polling();
				#endif
				
				#if defined(_ENABLE_BT_POWER_)
    				if(global.sys_power)
    				{
    					mode_start_polling();
    				}
				#endif
				
				#if defined(_ENABLE_ALARM_CLOCK_)
    				if(flag == 9)
    				{
    					sys_beep_polling();
    					set_beep_pwm();
    					flag = 0;
    				}
    				else
    				{
    					flag++;
    				}
				#endif
				
				#if defined(_CHANGE_lCD_DISPLAY_)
					if(!ACC_CHECK())
					{
						ui_drv_func.flash_disable(UI_ICON_USB);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_USB);
						ui_drv_func.flash_disable(UI_ICON_SD);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SD);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EQ_POP);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EQ_ROCK);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EQ_CLASS);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EQ_JAZZ);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_INT);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RPT);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RDM);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_1);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_2);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_3);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_VBAR_4);
						ui_show_setting(LANG_ID_NULL, 0);
						ui_show_normal(LANG_ID_NULL);
					}
				#endif
				
				#if defined(SUPPORT_ALARM_FUNC_)
					alarm_time_polling();
					get_rtc_time();
					disp_alarm();
				#endif
				
				#if defined(_ENABLE_TIME_DISP_)
					if(global.sys_power)
					{
						time_disp_polling();
					}
				#endif

				#if defined(_CHANGE_DISP_MODE_)
				    if(global.sys_power)
					{
						key_time_polling();
					}
				#endif
				
			    #if defined(_ENABLE_BUG_DETECT_)
			    	if(ACC_CHECK())
			    	{
			    		if(global.sys_power)
			    		{
			    			sys_bug_polling();
			    		}
			    		else
			    		{
			    			global.bug_time = 0;
			    		}
			    	}
			    	else
			    	{
			    		global.bug_time = 0;
			    	}
			    #endif
				
				#if defined(_ENABLE_POWER_BT_OFF_)
					bt_scan_polling();
				#endif

				#if defined(_SUPPORT_PLAYER_NAMES_)
				    if(global.sys_power)
					{
						back_play_polling();
					}
				#endif
				
				#if defined(_POWER_OFF_BT_DEBUG_)
				    talking_phone_polling();
				#endif
				
			    #if defined(_ACC_OFF_POWER_LOGO_)
			    	if(setup.acc_logo)
			    	{
			    		show_clear_icon(DISP_LEVEL_NORMAL);
			    		show_clear_num(DISP_LEVEL_NORMAL);
			    		acc_off_logo_polling();
			    	}
			    #endif
				
				#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
					amp_control_polling();
				#endif
					
			    #if defined(_KEY_LIGHT_CONTROL_)
			    	backlight_polling();
			    #endif
				
				#if defined(_SUPPORT_IPHONE_SIR_)
    				if(global.sys_power)
    				{
    					sir_mode_polling();
    				}
				#endif

				#if defined(_SL6812_ILL_LIGHT_)
					ill_light_polling();
				#endif

				#if defined(_SH1302_AKIN_OUTPUT_PIN)
				    if(global.sys_power)
			        {
			        	sys_akinoutput_polling();
			        }
				#endif
				
			    #if defined(_ENABLE_OPTICAL_CONTROL_)
			        if(global.sys_power)
			        {
			        	sys_optical_polling();
			        }
			    #endif

				#if defined(_CLEAN_LCD_DISPLAY_)
					if(!ACC_CHECK())
					{
						ui_show_setting(LANG_ID_NULL, 0);
						ui_show_normal(LANG_ID_NULL);
					}
				#endif
				
				#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
					if(global.sys_power)
					{
					#if defined(_LIANKE_6803_POWER_)
						if(global.lp6803_time)
						{
							global.lp6803_time--;
						}
						else
						{
							SYS_POW_ON();
							panel_light_polling();
							global.lp6803_time = 0;
						}
					#else
						panel_light_polling();
					#endif
					}
				#if defined(_SUPPORT_LIGHT_DETECT1_)
					else
					{
						if(ILL_LIGHT_DET_CHECK())
						{
							panel_light_polling();
						}
						else
						{
							SYS_RED_OFF();
							SYS_GREEN_OFF();
							SYS_BLUE_OFF();
						}
					}
				#endif
				#if defined(_CHANGE_IDLE_LIGHT_ON_)
				    else
					{
						panel_light_polling();
					}
				#endif
				#endif
				
				#if defined(_ENABLE_PIN_CONTROL_)
				    if(global.sys_power)
    				{
                        KB_LIGHT_ON();
    				}
					else
					{
                        if(ILL_LIGHT_DET_CHECK())
    					{
    					   KB_LIGHT_ON();
    					}
					    else
    					{
    					  KB_LIGHT_OFF();
    					}
					}
				#endif
				#if defined(ENABLE_PLANE_LIGHT_CONTROL_)
				#if defined(_ADD_LIGHT_DISP_)
				    if(global.sys_power)
				    {
                        if(ILL_LIGHT_DET_CHECK())
    					{
    					   KB_LIGHT_ON();
    					}
				        else
    					{
    					  KB_LIGHT_OFF();
    					}
					}
					else
					{
                        if(ILL_LIGHT_DET_CHECK())
						{
							panel_light_polling();
						}
						else
						{
							SYS_RED_OFF();
							SYS_GREEN_OFF();
							SYS_BLUE_OFF();
						}
					}
				#else
                    if(ILL_LIGHT_DET_CHECK())
					{
					   KB_LIGHT_ON();
					}
				    else
					{
					  KB_LIGHT_OFF();
					}
				#endif
				#endif
				#if defined(SUPPORT_LED_ADJUST)
					if(global.sys_power)
					{
						back_led_polling();
					}
				#endif

				#if defined(_ENABLE_PASSWORD_SET_)
				#if defined(_PASSWORD_INPUT_BLINK_)
    				if(((setup.pwd_set)&&(global.pwd_set_flag))||(setup.password_flag))
    				{
    					password_disp_polling();
    				}
				#else
					if((setup.pwd_set)&&(global.pwd_set_flag))
					{
						password_disp_polling();
					}
				#endif
				#endif
				
			    #if defined(_ENABLE_BT_ICON_)
			    	if(global.sys_power)
			    	{
			    		open_bt_icon_polling();
			    	}
			    #endif
				
			    #if defined(_ENABLE_TEL_MUTE_DETECT_)
			    	if(global.sys_power)
			    	{
			    		sys_tel_mute_polling();
			    	}
				#if defined(_ENABLE_TEL_MUTE_DISPLAY_)
					else
					{
						global.tel_mute_time = 0;
					}
			    #endif
				#endif

			    #if defined(_ENABLE_DISK_DISC_DISPLAY_)
			    	if(global.sys_power)
			    	{
						sys_det_disc_polling();
					}
			    #endif
				
			    #if defined(_ENABLE_BEEP_)
			    	sys_beep_polling();
			    #endif
				
				#if defined(_SPECIAL_MUTE_FLASH_)
					mute_display_polling();
				#endif
				
			#if !defined(_SUPPORT_AMP_CONTROL_)
			    #if defined(_SUPPORT_SUBWOOFER2_)
					if((global.sys_power)&&(ACC_CHECK()))
			    	{
			    		sys_sub_polling();
			    	}
			    #endif
			    #if defined(_SUPPORT_SUBWOOFER3_)
			    	if((global.sys_power)&&(ACC_CHECK()))
			    	{
			    		sys_sub2_polling();
			    	}
			    #endif
			#endif

		    #if defined(_SUPPORT_SUB_)
		    	if(global.sys_power)
		    	{
		    		sys_sub_polling();
		    	}
		    #endif
			
		    #if defined(_ENABLE_BT_OFF_)
		    	bt_scan_polling();
		    #endif

			#if defined(_ENABLE_RCA_MUTE_)&&0
    			if((global.sys_power)&&(ACC_CHECK()))
    			{
    				sys_rca_polling();
    			}
			#endif
			
				#if defined(_ENABLE_RDS_FIRST_)
    				if(global.sys_power)
    				{
    					rds_mode_polling();
    				}
				#endif

				#if defined(_IR_WITH_ACC_DELAY_)||defined(_ACC_OFF_DELAY_TIME_)
					acc_off_polling();
				#endif

				#if defined(_ENABLE_DLASHING_LIGHT_)
					dlashine_liaht_polling();
				#endif
				
				#if defined(SET_MUTE_WHEN_VOL_)
			    	set_mute_polling();
			    #endif
				
				#if defined(_ENABLE_IPHONE_CHARGE_)
			    	if(ACC_CHECK())
			    	{
			    		sys_charge_polling();
			    	}
				#endif
				
				#if defined(_WLS_MUTE_FUNTION_)
					mute_funtion_polling();
				#endif
				
			    #if defined(_ENABLE_AUTO_POWER_)
			    	if(global.sys_power)
			    	{
			    		memory_mode_polling();
			    	}
			    #endif
				
				#if defined(_FY7555_CLOCK_ICON_DISPLAY_)
    				if(global.sys_power)
    				{
    					time_icon_polling();
    				}
				#endif
				
				#if defined(_ENABLE_REALTIME_DIAPLAY_)
				#if defined(_FY5517_POWER_OFF_TIME_)
					if(global.sys_power)
					{
						rtc_disp();
						global.clock_flag = FALSE;
					}
					else
					{
						if(global.clock_flag)
						{
							rtc_disp();
						}
					}
				#else
					rtc_disp();
				#endif
				#endif
				
				#if defined(_TEMP_WARM_DISPLAY_)
    				if(global.temp_power_flag)
    				{
    					rtc_disp();
    				}
					else
					{
					#if defined(_HKY1032_SPECIAL_LOGO_)
						if(global.temp_power_off)
						{
							clean_rtc_disp();
							global.temp_power_off = FALSE;
						}
					#endif
					}
				#endif
				
				#if defined(_SUPPORT_TIME_DISPLAY_)
					if(global.sys_power)
					{
						rtc_disp();
					}
					else
					{
						clean_rtc_disp();
					}
				#endif
				
				#if defined(AP_ENABLE_REALTIME_DIAPLAY_)
    				if(global.sys_power)
    			    {
    					if((++ap_refur_timer2) >= 20)
    					{
    						ap_refur_timer2 = 0;
    						rtc_disp();
    					}
    				}
				#endif
				
			    #if defined(_ENABLE_BAND_MUTE_)
			    	if(global.sys_power)
			    	{
			    		band_mute_polling();
			    	}
				#if defined(_ENABLE_MUTE_OFF_)
    				else
    				{
						power_off_polling();
    				}
				#endif
			    #endif

				#if defined(_ENABLE_POWER_MUTE_)||defined(_ENABLE_FIRST_POWER_MUTE_)
			    	if(global.sys_power)
			    	{
			    		power_mute_polling();
			    	}
				#endif

			    #if defined(DISABLE_TURN_BT_MODE)
			    	if((global.sys_power)&&(ACC_CHECK()))
			    	{
			    		bt_detect_polling();
			    	}
			    #endif
				
				#if defined(_ENABLE_USB_SD_ICON_)
					if((global.sys_power)&&(ACC_CHECK()))
					{
			    		icon_detect_polling();
					}
				#endif
				
			    #if defined(_ENABLE_VOLUME_CONTROL_)
			    	show_icon_polling();
			    #endif

				#if defined(_ENABLE_USB_DELAY_)
    				if(global.sys_power)
    				{
    					mode_name_polling();
    				}
				#endif
				
				#if defined(_ENABLE_POWER_LOGO_)
					if(ACC_CHECK())
					{
						save_name_polling();
					}
				#endif
				
			    #if defined(_ENABLE_BT_CONNECT_)
				#if defined(_SUPPORT_DTH_)
					if((ACC_CHECK())&&(DTH_CHECK()))
				#else
					if(ACC_CHECK())
				#endif
					{
					#if defined(_ENABLE_POWER_BT_OFF_)
						if(global.sys_power)
					#endif
						{
							sys_btconnect_polling();
						}
					}
			    #endif
				
				#if defined(_ENABLE_VBAR_DISPLAY_)
					vbar_display_polling();
				#endif

			    #if defined(_ENABLE_AUX_DETECT_)
					if(global.sys_power)
					{
						sys_aux_polling();
					}
			    #endif
				
				#if defined(_XM007_SPECIAL_BACKLIGHT_)
					backlight_control_polling();
				#endif

				#if defined(_SUPPORT_MIC_AUX_)
				    if(global.sys_power)
				    {
						sys_mic_polling();
					}
				#endif
				
				#if defined(_EVB_POWER_ICON_)&&0
					if(global.sys_power)
					{
						ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER);
						ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER2);
					}
				#endif
				
				#if defined(_ENABLE_USB_SAVE_)||defined(_ENABLE_USB_DELAY_)||defined(_SLEEP_MODE_USB_DONT_DETCT)
    				if(global.sys_power||global.pwrstarttime)
    				{
    					if(global.power_time)
    					{
    						global.power_time--;
    					}
    				}
				#if defined(_SLEEP_MODE_USB_DONT_DETCT)	
					if(global.usb_det_delay)
					{
						//printf("usb_det_delay %d\n",global.usb_det_delay);
						global.usb_det_delay--;
						if(global.usb_det_delay==0)global.pwrstarttime = TRUE;
					}
				#endif
				#endif
				}
			#if defined(_SWL_ENABLE_DISK_DISC_TO_FLASH_)
				if(global.sys_power && disp_type != DISP_TYPE_IDLE)
    			{
					DiscPolling();
				}
			#endif
				break;

			case DISP_EVENT_RESET:
				{
					disp_func_reset();
				}
				break;

			case DISP_EVENT_CALLBACK:
				{
    				func_sel_check = (func_sel_check_f)msg[1];
				}
				break;

			case DISP_EVENT_VOLUME:
				{
					disp_func_volume();
				}
				break;

			case DISP_EVENT_MUTE:
				{
					disp_func_sel_mute();
				}
				break;
			case DISP_EVENT_INSIDE_MUTE:
				{
					disp_func_inside_mute(msg[1]);
				}
				break;

			case DISP_EVENT_EQ:
				{
					disp_func_sel_eq();
				}
				break;

			case DISP_EVENT_LOUD:
				{
					disp_func_sel_loud();
				}
				break;
			
	    #if defined(_ENABLE_DISP_BT_NAME_)
	    	case DISP_EVENT_BT_NAME:
	    		{
	    			disp_bt_name();
	    		}
	    		break;
	    #endif

		#if defined(_ENABLE_TEMP_PWM_)
    		case DISP_EVENT_TEMP:
    			{
    				disp_func_sel_temp();
    			}
    			break;
				
			case DISP_EVENT_WARM:
				{
					disp_func_sel_warm();
				}
				break;
		#endif
		
		#if defined(_SPECIAL_TL2000_SET_)		
			case DISP_EVENT_A_ADD_VOLUME:
				if(msg_realtime_check())
				{
					msg_realtime_add();
				}
				else if(msg_realtime_check1())
				{
					msg_realtime_add1();
				}
				else if(msg_vol_check())
				{
					disp_func_a_add_volume();
				}
				else
				{
					disp_func_sel_add();
				}
				break;
			case DISP_EVENT_A_SUB_VOLUME:
				if(msg_realtime_check())
				{
					msg_realtime_sub();
				}
				else if(msg_realtime_check1())
				{
					msg_realtime_sub1();
				}
				else if(msg_vol_check())
				{
					disp_func_a_sub_volume();
				}
				else
				{
					disp_func_sel_sub();
				}
				break;
			case DISP_EVENT_B_ADD_VOLUME:
				if(msg_realtime_check())
				{
					msg_realtime_add();
				}
				else if(msg_realtime_check1())
				{
					msg_realtime_add1();
				}
				else if(msg_vol_check())
				{
					disp_func_b_add_volume();
				}
				else
				{
					disp_func_sel_add();
				}
				break;
			case DISP_EVENT_B_SUB_VOLUME:
				if(msg_realtime_check())
				{
					msg_realtime_sub();
				}
				else if(msg_realtime_check1())
				{
					msg_realtime_sub1();
				}
				else if(msg_vol_check())
				{
					disp_func_b_sub_volume();
				}
				else
				{
					disp_func_sel_sub();
				}
				break;
	#endif
			case DISP_EVENT_ADD:
				{
				#if defined(_FY5517_TIME_DISPLAY_)
					if(global.time_set)
					{
						msg_realtime_add();
					}
					else
					{
						disp_func_sel_add();
					}
				#else
					if(msg_realtime_check())
					{
				    #if defined(_FEIYIN_MENU_TIME_)
				    	ui_disp.timer = 3000;
				    #endif
						msg_realtime_add();
					}
				#if defined(SUPPORT_ALARM_FUNC_)
    				else if(msg_realtime_check1())
    				{
    					msg_realtime_add1();
    				}
				#endif
					else
					{
						disp_func_sel_add();
					}
				#endif
				}
				break;

			case DISP_EVENT_SUB:
				{
				#if defined(_FY5517_TIME_DISPLAY_)
					if(global.time_set)
					{
						msg_realtime_sub();
					}
					else
					{
						disp_func_sel_sub();
					}
				#else
					if(msg_realtime_check())
					{
				    #if defined(_FEIYIN_MENU_TIME_)
				    	ui_disp.timer = 3000;
				    #endif
						msg_realtime_sub();
					}
			    #if defined(SUPPORT_ALARM_FUNC_)
			    	else if(msg_realtime_check1())
			    	{
			    		msg_realtime_sub1();
			    	}
			    #endif
					else
					{
						disp_func_sel_sub();
					}
				#endif
				}
				break;
				
		#if defined(_SUPPORT_TWO_PT2313_)
    		case DISP_EVENT_ADD1:
				disp_func_sel_add1();
    			break;
    			
    		case DISP_EVENT_SUB1:
				disp_func_sel_sub1();
    			break;
		#endif
		
		#if defined(_ENABLE_MENU_SET_)||defined(_DISABLE_SEL_VOLUME_)
    		case DISP_EVENT_ADD_VOL:
				disp_func_add_vol();
    			break;
				
			case DISP_EVENT_SUB_VOL:
				disp_func_sub_vol();
				break;
		#endif

			case DISP_EVENT_SEL:
				{
				#if defined(_FY5517_TIME_DISPLAY_)
					if(global.time_set)
					{
						msg_realtime_select();
					}
					else
					{
						disp_func_sel_sel();
					}
				#else
					if(msg_realtime_check())
					{
						msg_realtime_select();
					}
					else
					{
						disp_func_sel_sel();
					}
				#endif
				}
				break;

			case DISP_EVENT_FUNC:
				{
					disp_func_sel_func();
				}
				break;

	  #if defined(_ENABLE_SET_FUNTION_)
			case DISP_EVENT_SEL1:
				{
					if(msg_realtime_check())
					{
						msg_realtime_select();
					}
					else
					{
						disp_func_sel_sel1();
					}
				}
				break;
		
			case DISP_EVENT_FUNC1:
				{
					disp_func_sel_func1();
				}
				break;
	  #endif
			case DISP_EVENT_CLOCK:
				{
					msg_realtime_show();
				}
				break;

			case DISP_EVENT_CLOCK_ADJ:
				{
					msg_realtime_setting();
				}
				break;
				
		#if defined(SUPPORT_ALARM_FUNC_)
    		case DISP_EVENT_CLOCK1:
    			{
    				msg_realtime_show1();
    			}
    			break;
    		
    		case DISP_EVENT_CLOCK_ADJ1:
    			{
    				msg_realtime_setting1();
    			}
    			break;
		#endif
				
	    #if defined(_SL6812_TEST_LCD_)
	    	case DISP_EVENT_SYS_LCD:
	    		{
	    			ui_drv_func.open_screen(DISP_LEVEL_SETTING);
	    			ui_disp.now_level = DISP_LEVEL_SETTING;
	    			ui_disp.timer = 3000;
					global.lcd_flag = TRUE;
	    		}
	    		break;
	    #endif
		
		#if defined(_SL6812_KEY_FUNTION_)
			case DISP_EVENT_SYS_SPM:
				ui_show_setting(LANG_ID_SPM, 0);
	    		break;
		#endif
		
        #if defined(_BT_DISCONNECT_DISPLAY_)
	    	case DISP_EVENT_SYS_DISP:
	    		ui_show_setting(LANG_ID_CONNECTD, 0);
	    		break;
        #endif
		
		#if defined(_ENABLE_PASSWORD_INPUT_)
    		case DISP_EVENT_PASSWORD:
				password_control();
    			break;
			case DISP_EVENT_PASSWORD1:
				password_control_number1();
				break;
    		case DISP_EVENT_PASSWORD2:
    			password_control_number2();
    			break;
			case DISP_EVENT_PASSWORD3:
				password_control_number3();
				break;
			case DISP_EVENT_PASSWORD4:
				password_control_number4();
				break;
			case DISP_EVENT_ENTER:
				password_control_enter();
				break;
		#endif
		
			case DISP_EVENT_CLOCK_MUTE:
				{
					if(msg[1])
					{
						if((ui_disp.now_level != DISP_LEVEL_SETTING)
						 ||(ui_disp.func_sel != FUNC_SEL_TIME))
						{
							disp_send_msg(DISP_EVENT_CLOCK);
						}
						else
						{
							disp_send_msg(DISP_EVENT_CLOCK_ADJ);
						}
					}
					else
					{
						if((ui_disp.now_level == DISP_LEVEL_SETTING)
		                 &&(ui_disp.func_sel == FUNC_SEL_TIME))
						{
							disp_send_msg(DISP_EVENT_CLOCK);
						}
						else
						{
							disp_send_msg(DISP_EVENT_MUTE);
						}
					}
				}
				break;
				
	    #if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
	    	case DISP_EVENT_PASSWORD_NO_1:
	    		{
	    			password_control_number1();
	    		}
	    		break;
	    	case DISP_EVENT_PASSWORD_NO_2:
	    		{
	    			password_control_number2();
	    		}
	    		break;
	    	case DISP_EVENT_PASSWORD_NO_3:
	    		{
	    			password_control_number3();
	    		}
	    		break;
	    	case DISP_EVENT_PASSWORD_NO_4:
	    		{
	    			password_control_number4();
	    		}
	    		break;
				
			case DISP_EVENT_PASSWORD_NO_5:
				{
					password_control_number5();
				}
				break;
			case DISP_EVENT_PASSWORD_NO_6:
				{
					password_control_number6();
				}
				break;
			
	    	case DISP_EVENT_PASSWORD_ENTER:
	    		{
	    			password_control_enter();
	    		}
	    		break;
	    #endif

        #if defined(_ENABLE_FAD_FUNTION_)
    		case DISP_EVENT_FAD_SINGLE_F:
				ui_show_setting(LANG_ID_FAD_SINGLE_F, 0);
    			setup.aud_fad = 7;
				audio_drv_ctrl(AUD_CTRL_FR,	setup.aud_fad, TRUE);
				disp_func_sel_control();
				break;
			case DISP_EVENT_FAD_SINGLE_R:
				ui_show_setting(LANG_ID_FAD_SINGLE_R, 0);
    			setup.aud_fad = -7;
				audio_drv_ctrl(AUD_CTRL_FR,	setup.aud_fad, TRUE);
				disp_func_sel_control();
				break;
			case DISP_EVENT_FAD_ALL:
				ui_show_setting(LANG_ID_FAD_ALL, 0);
    			setup.aud_bal = 0;
    			setup.aud_fad = 0;
				audio_drv_ctrl(AUD_CTRL_LR,	setup.aud_bal, TRUE);
				audio_drv_ctrl(AUD_CTRL_FR,	setup.aud_fad, TRUE);
				disp_func_sel_control();
				break;
        #endif
		
        #if defined(_ENABLE_AUX_VER_VIEW_)
        	case DISP_EVENT_VER_VIEW:
        		{
        			disp_ver_view();
        		}
        		break;
        #endif

	    #if defined(_POWER_SET_COLOR_)||defined(_ENABLE_SET_COLOR_)
	        case DISP_EVENT_LIGHT:
	        	{
	    			disp_func_sel_light();
	        	}
	        	break;
	    #endif
		
		#if defined(_ENABLE_ID3_SET_)
	        case DISP_EVENT_ID3:
	        	{
	    			disp_func_sel_id3();
	        	}
	        	break;
		#endif
		
		#if defined(SUPPORT_LED_ADJUST)
	        case DISP_EVENT_LED:
	        	{
	    			disp_func_sel_led();
	        	}
	        	break;
	    #endif

		#if defined(_ADJUST_A_B_SPK_)
	        case DISP_EVENT_ADJ_A_SPK:
	        	{
	    			disp_func_sel_a_spk();
	        	}
	        	break;
	        case DISP_EVENT_ADJ_B_SPK:
	        	{
	    			disp_func_sel_b_spk();
	        	}
	        	break;
	    #endif

		#if defined(_MR889_SPECAIL_KEY_)
			case DISP_EVENT_ADJ_CONTROL_SPK:	
				{
					disp_func_sel_control_spk();
				}
				break;
		#endif

		#if defined(SUPPORT_ALARM_FUNC_)
			case DISP_EVENT_ALARM:	
				{
					disp_func_sel_alarm();
				}
				break;
		#endif
		
		#if defined(_ENABLE_RADIO_RDS_)
			case DISP_EVENT_AF:
				{
					msg_rds_af();
				}
				break;

			case DISP_EVENT_TA:
				{
					msg_rds_ta();
				}
				break;

			case DISP_EVENT_PTY:
				{
					if(radio_rds_ctrl(FUNC_RDS_PTY, msg[1]))
					{
						disp_show_sel(FUNC_RDS_PTY);
					}
				}
				break;

			case DISP_EVENT_EON:
				{
					msg_rds_eon();
				}
				break;

			case DISP_EVENT_REG:
				{
					msg_rds_reg();
				}
				break;

			case DISP_EVENT_RDS_ICON:
				{
					ui_show_rds_icon(msg[1]);
				}
				break;

			case DISP_EVENT_RDS_TXT:
				{
					ui_show_rds_txt(msg[1]);
				}
				break;

			case DISP_EVENT_RDS_DATA:
				{
					typedef struct{
						uint16	year;
						uint8	month;
						uint8	day;
						uint8	hour;
						uint8	min;
					}rds_date_t;

					RTC_TIME rtc_time;
					rds_date_t *rds_time = (rds_date_t *)msg[1];

					rtc_time.dYear 	= rds_time->year;
					rtc_time.bMonth	= rds_time->month;
					rtc_time.bDay	= rds_time->day;
					rtc_time.bHour	= rds_time->hour;
					rtc_time.bMin	= rds_time->min;
					rtc_time.bSec	= 0;

					sys_set_rtc(&rtc_time);
				}
				break;
		#endif

	    #if defined(_SUPPORT_DISC_)
	    	case DISP_EVENT_ICON_DISC:
	    		disp_puts("DISP_EVENT_ICON_DISC\n");
	    		if(disp_type != DISP_TYPE_IDLE)
	    		{
	    			ui_show_disc_icon(msg[1]);
	    		}
	    		break;
	    	case DISP_EVENT_EJECT:
	    		disp_puts("DISP_EVENT_EJECT\n");
	    		if(disp_type != DISP_TYPE_IDLE)
	    		{
	    			ui_show_disc_eject(msg[1]);
	    		}
	    		break;
	    #if defined(NEED_NO_DISC_DISP)
	    	case DISP_EVENT_NO_DISC:
	    		disp_puts("DISP_EVENT_NO_DISC\n");
	    		if(disp_type != DISP_TYPE_IDLE)
	    		{
	    			ui_show_no_disc(msg[1]);
	    		}
	    		break;
	    #endif
	    #endif

			case DISP_EVENT_ICON_USB:
				disp_puts("DISP_EVENT_ICON_USB\n");
				if(disp_type != DISP_TYPE_IDLE)
				{
					ui_show_usb_icon(msg[1]);
				}
				break;

			case DISP_EVENT_ICON_SD:
				disp_puts("DISP_EVENT_ICON_SD\n");
				if(disp_type != DISP_TYPE_IDLE)
				{
					ui_show_sd_icon(msg[1]);
				}
				break;

			case DISP_EVENT_ICON_BT:
				disp_puts("DISP_EVENT_ICON_BT\n");
				if(disp_type != DISP_TYPE_IDLE)
				{
					ui_show_bt_icon(msg[1]);
				}
		    #if defined(_ENABLE_BT_ONOFF_DISPLAY_)
				else
				{
					if(msg[1])
					{
						SYS_POW_ON();
						Color6803Refresh(0, 0, 0);
						ui_show_bt_icon(msg[1]);
						global.bt_display = TRUE;
						global.bt_disp_time = 0;
					}
				}
			#endif
				break;
			
	    #if defined(HANGUP_DISP_HANGUP)
	    	case DISP_EVENT_BT_END:
	    		if(disp_type != DISP_TYPE_IDLE)
	    		{
					ui_show_setting(LANG_ID_BT_END, 0);
					ui_disp.timer = 500;
					ui_show_normal(LANG_ID_BT_END);
	    		}
	    		break;
	    #endif

			//-------disp idle msg--------------
			case DISP_EVENT_IDLE:
				disp_puts("DISP_EVENT_IDLE\n");
				if(disp_type != DISP_TYPE_IDLE)
				{
					disp_type_exit(disp_type);
					disp_type = DISP_TYPE_IDLE;
					disp_type_init(disp_type);
				}
				break;
			case DISP_EVENT_IDLE_PLAY:
				disp_puts("DISP_EVENT_IDLE_PLAY\n");
				if(disp_type == DISP_TYPE_IDLE)
				{
					ui_show_idle_play((void *)msg[1]);
			 	}
				break;

			//-------disp radio msg--------------
			case DISP_EVENT_RADIO:
				disp_puts("DISP_EVENT_RADIO\n");
				if(disp_type != DISP_TYPE_RADIO)
				{
					disp_type_exit(disp_type);
					disp_type = DISP_TYPE_RADIO;
					disp_type_init(disp_type);
				}
				break;
			case DISP_EVENT_FREQ:
				if(disp_type == DISP_TYPE_RADIO)
				{
				#if defined(FM_INIT_OK_FLAG)
					global.fm_init_ok_flag=0;
				#endif
					//printf("11111=FREQ\n");
					ui_show_radio_play((void *)msg[1]);
				}
				break;
		#if defined(TD_ADD_CHADD_SUB)
            case DISP_EVENT_BANDCH:
                disp_add_sub();
            	break;
		#endif
			case DISP_EVENT_ST:
				if(disp_type == DISP_TYPE_RADIO)
				{
					ui_show_radio_st((uint8)msg[1]);
				}
				break;
			case DISP_EVENT_LOC:
				if(disp_type == DISP_TYPE_RADIO)
				{
					ui_show_radio_loc((uint8)msg[1]);
				}
				break;
        #if !defined(_TY2200_RADIO_SEEK_)
		#if !defined(_DISABLE_RADIO_MANUAL_AUTO_DISP_)	
            case DISP_EVENT_MANUAL:
            	if(disp_type == DISP_TYPE_RADIO)
            	{
            		ui_show_setting(LANG_ID_RADIO_MANUAL, 0);
            	}
            	break;
            case DISP_EVENT_AUTO:
            	if(disp_type == DISP_TYPE_RADIO)
            	{
            		ui_show_setting(LANG_ID_RADIO_AUTO, 0);
            	}
            	break;
		#endif		
        #endif		
		#ifdef	TD_ADD_DISP_CHECKSUM
			case DISP_EVENT_CHECKSUM:
                disp_func_reset();
                ui_show_checksum();
				break;
		#endif

	    #if defined(_ENABLE_VERSION_VIEW_)
	    	case DISP_EVENT_VERSION:
	    		if(disp_type == DISP_TYPE_RADIO)
	    		{
	    			disp_puts("DISP_EVENT_VERSION\n");
	    			ui_show_radio_version((uint8)msg[1]);
			    #if defined(ENABLE_PASSWORD_RESET)
			    	ui_show_soft_reset((uint8)msg[1]);
			    #endif
	    		}
	    		break;
	    #endif
		
        #if defined(_ENABLE_AUX_VERSION_)||defined(_ENABLE_MODE_VERSION_)
    		case DISP_EVENT_AUX_VERSION:
    			if(disp_type == DISP_TYPE_AUX)
    			{
    				ui_show_aux_version();
    			}
    			break;
        #endif
		
	    #if defined(_ENABLE_SUB_DETECT_)
	        case DISP_EVENT_SYS_SUB:
	        	{
	    			ui_show_sub();
	        	}
	        	break;
	    #endif

		//-------disp usb msg--------------
			case DISP_EVENT_USB:
				disp_puts("DISP_EVENT_USB\n");
				if(disp_type != DISP_TYPE_USB)
				{
				#if defined(TD_ADD_NEW_APP)
					read_list_ready_flag=FALSE;
				#endif
					disp_type_exit(disp_type);
					disp_type = DISP_TYPE_USB;
					disp_type_init(disp_type);
				}
				break;
			case DISP_EVENT_SD:
				disp_puts("DISP_EVENT_SD\n");
				if(disp_type != DISP_TYPE_SD)
				{
				#if defined(TD_ADD_NEW_APP)
					read_list_ready_flag=FALSE;
				#endif
					disp_type_exit(disp_type);
					disp_type = DISP_TYPE_SD;
					disp_type_init(disp_type);
				}
				break;

	    #if defined(_SUPPORT_DISC_)
	    	case DISP_EVENT_PLAYER_DISC:
	    		disp_puts("DISP_EVENT_PLAYER_DISC\n");
	    		if(disp_type != DISP_TYPE_PLAYER_DISC)
	    		{
	    			disp_type_exit(disp_type);
	    			disp_type = DISP_TYPE_PLAYER_DISC;
	    			disp_type_init(disp_type);
	    		}
	    		break;
	    	case DISP_EVENT_PLAYER_USB:
	    		disp_puts("DISP_EVENT_PLAYER_USB\n");
	    		if(disp_type != DISP_TYPE_PLAYER_USB)
	    		{
				#if defined(TD_ADD_NEW_APP)
                    read_list_ready_flag=FALSE;
				#endif
	    			disp_type_exit(disp_type);
	    			disp_type = DISP_TYPE_PLAYER_USB;
	    			disp_type_init(disp_type);
	    		}
	    		break;
	    	case DISP_EVENT_PLAYER_CARD:
	    		disp_puts("DISP_EVENT_PLAYER_CARD\n");
	    		if(disp_type != DISP_TYPE_PLAYER_CARD)
	    		{
				#if defined(TD_ADD_NEW_APP)
                    read_list_ready_flag=FALSE;
				#endif
	    			disp_type_exit(disp_type);
	    			disp_type = DISP_TYPE_PLAYER_CARD;
	    			disp_type_init(disp_type);
	    		}
	    		break;
	    #endif

			case DISP_EVENT_PLAY:// 1S执行一次
				if((disp_type == DISP_TYPE_USB)||(disp_type == DISP_TYPE_SD))
				{
					//disp_puts("DISP_EVENT_PLAY-----111111\n");
					ui_show_play_play((void *)msg[1]);
				}
		    #if defined(_SUPPORT_DISC_)
		    	else if((disp_type == DISP_TYPE_PLAYER_DISC)
		    		  ||(disp_type == DISP_TYPE_PLAYER_USB)
		    		  ||(disp_type == DISP_TYPE_PLAYER_CARD))
		    	{
		    		ui_show_disc_play((void *)msg[1]);
		    	}
		    #endif
				break;
			case DISP_EVENT_PLAY_CMD:
				disp_puts("DISP_id3_11111\n");
				if((disp_type == DISP_TYPE_USB)||(disp_type == DISP_TYPE_SD))
				{
					disp_puts("DISP_id3_22222\n");
					ui_show_play_cmd(msg[1], (void *)msg[2]);
				}
				break;

			//-------disp bt msg--------------
			case DISP_EVENT_BT:
				disp_puts("DISP_EVENT_BT\n");
				if(disp_type != DISP_TYPE_BT)
				{
					disp_type_exit(disp_type);
					disp_type = DISP_TYPE_BT;
					disp_type_init(disp_type);
				}
				break;
			case DISP_EVENT_BT_PLAY:
				if(disp_type == DISP_TYPE_BT)
				{
					ui_show_bt_play((void *)msg[1]);
				}
				break;

			//-------disp aux msg--------------
			case DISP_EVENT_AUX:
				disp_puts("DISP_EVENT_AUX\n");
				if(disp_type != DISP_TYPE_AUX)
				{
					disp_type_exit(disp_type);
					disp_type = DISP_TYPE_AUX;
					disp_type_init(disp_type);
				}
				break;
        #if defined(_SUPPORT_AUX1_)
			 case DISP_EVENT_AUX1:
				 disp_puts("DISP_EVENT_AUX1\n");
				 if(disp_type != DISP_TYPE_AUX1)
				 {
					 disp_type_exit(disp_type);
					 disp_type = DISP_TYPE_AUX1;
					 disp_type_init(disp_type);
				 }
				 break;
        #endif
		#if defined(DAB_MODLE)
			case DISP_EVENT_DAB:
				disp_puts("DISP_EVENT_DAB\n");
				if(disp_type != DISP_TYPE_DAB)
				{
					disp_type_exit(disp_type);
					disp_type = DISP_TYPE_DAB;
					disp_type_init(disp_type);
				}
				break;
		#endif
		#if defined(_SUPPORT_TAPE_)
    		case DISP_EVENT_TAPE:
    			disp_puts("DISP_EVENT_TAPE\n");
    			if(disp_type != DISP_TYPE_TAPE)
    			{
    				disp_type_exit(disp_type);
    				disp_type = DISP_TYPE_TAPE;
    				disp_type_init(disp_type);
    			}
    			break;
		#endif
		#if defined(_SUPPORT_MIC_OK_)
    		case DISP_EVENT_MIC:
    			disp_puts("DISP_EVENT_MIC\n");
    			if(disp_type != DISP_TYPE_MIC)
    			{
    				disp_type_exit(disp_type);
    				disp_type = DISP_TYPE_MIC;
    				disp_type_init(disp_type);
    			}
    			break;
		#endif
	    #if defined(_SUPPORT_CAN_MODEL_)
	    	case DISP_EVENT_CAN:
	    		disp_puts("DISP_EVENT_CAN\n");
	    		if(disp_type != DISP_TYPE_CAN)
	    		{
	    			disp_type_exit(disp_type);
	    			disp_type = DISP_TYPE_CAN;
	    			disp_type_init(disp_type);
	    		}
	    		break;
	    #endif
		
        #if defined(_SUPPORT_REVERSE_MODEL_)
	    	case DISP_EVENT_REVE:
	    		disp_puts("DISP_EVENT_REVE\n");
	    		if(disp_type != DISP_TYPE_REVE)
	    		{
	    			disp_type_exit(disp_type);
	    			disp_type = DISP_TYPE_REVE;
	    			disp_type_init(disp_type);
	    		}
	    		break;
        #endif
		
		#if defined(EASYESY_APP_ONOFF)
			case DISP_APP_ONOFF:
				{
					ui_APP_ONOFF(msg[1]);
					break;
				}
		#endif
			case DISP_EVENT_REC:
				disp_puts("DISP_EVENT_REC\n");
				if(disp_type != DISP_TYPE_REC)
				{
					disp_type_exit(disp_type);
					disp_type = DISP_TYPE_REC;
					disp_type_init(disp_type);
				}
				break;

	        default:
	            break;
        }
		//disp_printf("disp_end_msg:%x\n",msg[0]);
    }
}

void disp_task_init(void)  //
{
    disp_puts("disp task init\n");
	u32 err;
    err = os_task_create( disp_task,
					(void *)0,
				#if defined(_ENABLE_USB_BUG_)
					14,
				#else
					TaskDispPrio,
				#endif
					60,
					DISP_TASK_NAME);

	if(OS_NO_ERR != err)
    {
        disp_puts("create disp msg fail\n");
        while(1);
    }
}

void disp_task_exit(void)
{
	disp_puts("disp task exit\n");

    if(os_task_del_req(DISP_TASK_NAME) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(DISP_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        }
        while(os_task_del_req(DISP_TASK_NAME) != OS_TASK_NOT_EXIST);
		#if defined( _SUPPORT_LCM_1618_DRIVE )
			lcm_clear();//清屏
		#endif
    }
}

