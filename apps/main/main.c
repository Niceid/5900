#include "sdk_cfg.h"
#include "dev_manage/dev_ctl.h"
#include "file_operate/file_op_err.h"
#include "disp.h"
#include "setup.h"
#include "key_drv.h"
#include "update.h"
#include "rtc.h"
#include "dac/dac.h"
#include "dac/dac_api.h"
#include "sys_detect.h"
#include "ui_res.h"
#include "ui_api.h"
#include "key_drv/key_drv_ad.h"
#include "bluetooth/avctp_user.h"
#include "cpu/uart_api.h"
#include "bluetooth/bt.h"
#include "file_operate/file_op.h"
#include "fm_band.h"
#include "timer.h"
#include "pt2313/pt2313.h"
#include "clock.h"
#include "audio_drv.h"
#include "vm/vm_api.h"


#if defined(_AUDIO_DRIVER_AP3768_)
#include "ap3768/ap3768.h"
#endif
#if defined(_SUPPORT_EXT_MEMORY_)
#include "ext_memory/ext_memory.h"
#endif

#if defined(_ENABLE_TIME_DISP_)
#include "ui_drv.h"
#endif

#if defined(_SUPPORT_TAPE_)
#include "tape/tape.h"
#endif

#if defined(_SUPPORT_MIC_OK_)
#include "mic/mic.h"
#endif

#ifdef TD_ADD_NEW_APP
#include "timer.h"
extern u8 spec_mode;
u8 power_up_flag=TRUE;

extern void app_link_ok_reply(void);
extern void app_link_ok_reply_init_data(void);
extern void new_app_radio_init(void);
extern void new_app_send_bandinfo(void);
extern void new_app_send_vol(void);
extern void clr_play_mode_list(u8 playmode);
#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
extern void new_app_send_light(void);
#endif

#if defined(_ENABLE_LIGHT_SHADE_)
extern void	new_app_shade_light(void);
#endif

#if !defined(_USB_AND_SD_FUNTION_)
static uint8 idle_usb_state(void)
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

static uint8 idle_card_state(void)
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
#endif
#endif

#if defined(_SUPPORT_DISC_)
#include "disc/disc.h"
#include "disc_drv/disc_drv.h"
#endif
#if defined(_SUPPORT_LCM_1618_DRIVE)
#include "Lcm_1618_drive.h"
#if defined(DAB_MODLE)
extern u8 dab_select_flag;
extern u16 radio_total;
extern int RADIO_CURRENT_STATION;
extern u8 DAB_DISP_TEPY;
extern u16 RADIO_AUTO_PLAY_TIME;//
extern u8 DAB_DISP_DLS_SIZE;//清除dls缓存信息长度
extern u8 DAB_DISP_PLAYPAUSE;//清除播放标志
extern u16  dab_select_TIME;
#endif
#endif

#if defined(ENABLE_MIN_HOUR_ONCE)
u8 msg_realtime_select_count=0;
#endif

extern u32 server_send_handle;

#ifdef SUPPORT_CALL_NAMES
extern uint8 powerup_flag;
extern uint8 play_song_flag;
#endif
#if defined(TD_ADD_ACCOFF_TIMEOUT_RESET)
extern u8 gAccOffFlag;
#endif

#if defined(UART_Servo_Pop)
extern void Yhx_Dab_Pop_Work(void);
#endif

#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime;
extern void Spp_Tx_Ack(void);
extern void Spp_Tx_Audio_ble(void);
extern void Spp_Tx_Audio(void);
void Spp_Tx_Radio_Info(uint16 freq);
extern void Spp_Tx_WorkMode(void);
extern void Spp_Tx_dab(void);
#if defined(_SUPPORT_TAPE_)
extern void Spp_Tx_Tape(void);
#endif
#if defined(_SUPPORT_MIC_OK_)
extern void Spp_Tx_Mic(void);
#endif
#endif
#if defined(DAB_ACC_ON_ERROR_)
extern u8 DAB_ACC;
#endif
#define MAIN_DEBUG
#ifdef MAIN_DEBUG
#define main_printf 	ew_printf
#define main_puts 		ew_puts
#else
#define main_printf(...)
#define main_puts(...)
#endif

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
extern void resourse_manage_init(void);
extern u32 lg_dev_status_chg(u32 status,void *parm,u32 lgdevnum);
extern u32 lg_dev_mount(void *parm,u8 first_let,u32 total_lgdev);
extern int msg_mask_off_in_bt(void);

extern volatile u8 new_lg_dev_let;
static uint8 main_sem_flag = FALSE;
static OS_SEM main_eng_sem;
extern void music_enter_updata(void);
u8 Interrupt=FALSE;
#ifdef	TD_ADD_IPONE_CHARGE
u8 usb_insert=false;
u8 usb_ERR=false;
#endif

#if defined(_SUPPORT_30P60_LIGHT_)
#if defined(_ENABLE_LIGHT_SHADE_)
extern void LightDriverIrCmd(uint8 cmd,uint8 red,uint8 green,uint8 blue);
#else
extern void LightDriverIrCmd(uint8 cmd);
#endif
#endif

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

#if REC_EN
extern uint16 rec_time_count,rec_status;
#endif

#if defined(_SL6812_POWER_FUNTION_)||defined(_AUX_MODE_BT_)
static uint8 bt_connect_state(void)
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
#endif

#if defined(_USB_AND_SD_FUNTION_)||defined(_ENABLE_ACC_MODE_)||defined(_ENABLE_MODE_FUNTION_)||defined(_DISABLE_AUX_)||defined(_CHANGE_USB_SD_FUNTION_)||defined(_SL6812_SPECIAL_MODE_)
#if !defined(DISABLE_TURN_BT_MODE)
static uint8 idle_usb_state(void)
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

static uint8 idle_card_state(void)
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
#endif
#endif

#if defined(DISABLE_TURN_BT_MODE)
static uint8 idle_bt_state(void)
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

static uint8 idle_usb_state(void)
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

static uint8 idle_card_state(void)
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

#if defined(_ENABLE_IPHONE_CHARGE_)
#if !defined(DISABLE_TURN_BT_MODE)
#if !defined(_DISABLE_RADIO_)
#if !defined(_ENABLE_ACC_MODE_)
static uint8 idle_card_state(void)
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
#endif
#endif
#endif

#if REC_EN
static uint8 detect_usb_card_state(void)
{
	if(file_operate_get_total_phydev())
	{
		if((dev_get_online_status(usb))||(dev_get_online_status(sd1)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}
#endif

static uint8 post_sync_event(char *task_name, int param1, int param2)
{
	uint8 flag = FALSE;

	if(main_sem_flag)
	{//发送同步消息
		os_taskq_post_event(task_name, 3, param1, param2, (void *)&main_eng_sem);
		if(os_sem_pend(&main_eng_sem, 0) == OS_NO_ERR)
	    {
	    	flag = TRUE;
	    }
	}
	return flag;
}

static void check_task_switch(const char *task_name)
{
	const char *last_task_name = get_last_task_name();
	main_printf("check_task_switch --- task_name:%s, last_task_name ---:%s\n", task_name, last_task_name);

	if((last_task_name == NULL) || (strlen(last_task_name) == 0))
	{
		task_switch(task_name, 0,SWITCH_SPEC_TASK);
		set_last_task_name(setup.task_name);
	}
	else
	{
		task_switch(task_name, 0,SWITCH_SPEC_TASK);
	}
}

static void switch_activation_task(char *name)
{
	if(!compare_task_name(name))
	{
		if(global.aud_mute)
		{
			disp_send_msg(DISP_EVENT_MUTE);
		}

		if(global.sys_power)
		{
		#if defined(_DISABLE_SYSTEM_MODE_)
			if(!global.rds_ta)
			{
				task_switch(name, 0,SWITCH_SPEC_TASK);
			}
		#else
			task_switch(name, 0,SWITCH_SPEC_TASK);
		#endif
		}
		else
		{
        #if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
        	if(setup.first_power_flag)
        	{
        		os_taskq_post_msg("IdleTask", 1, DISP_EVENT_PASSWORD);
        	}
        	else
        	{
        		os_taskq_post_msg("IdleTask", 1, MSG_IDLE_EXIT);
        	}
        #else
        	#if !defined(_DISABLE_POWER_ON_)
        		os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_EXIT);
			    main_printf("IDLE_TASK_NAME111112222222222\n");
        	#endif
        #endif
			setup.task_name = name;
		}
	}
}

#if defined(_ENABLE_ALARM_CLOCK_)
void PWM0_init(void) //time3  pwm0      PA1
{
	JL_IOMAP ->CON1 |= BIT(11)|BIT(12);         // 使用PWM3 输出
	JL_PORTA ->DIR	&= ~(BIT(1));               //输出PWM 的IO
	JL_PORTA ->PU	|=  BIT(1);
	JL_PORTA ->PD	|=  BIT(1);
	JL_PORTA ->DIE  |=	BIT(1);

	JL_TIMER3->CNT = 0;// 不需用到的寄存器,初始化为  0
	JL_TIMER3->PWM = 15000;//占空比计算      75/7500      1%
	JL_TIMER3->PRD = 30000;//周期频率  7500*4/60M == 500us
	//JL_TIMER3->CON = BIT(8) | BIT(0);
	JL_TIMER3->CON &= ~(BIT(8)| BIT(0));
	// 选择系统时钟(LSB),1 分频,使能 PWM, 配置为 PWM 模式
}
#endif

#if defined(_ENABLE_TEMP_PWM_)
void PWM0_init(uint8 duty) //time3  pwm0      PA1
{
	u32 sys_clk = OSC_Hz;//clock_get_lsb_freq();

	JL_IOMAP ->CON1 |= BIT(11)|BIT(12);
	JL_PORTA ->DIR	&= ~(BIT(1));
	JL_PORTA ->PU	|=	BIT(1);
	JL_PORTA ->PD	|=	BIT(1);
	JL_PORTA ->DIE	|=	BIT(1);

	JL_TIMER3->CON = 0x0000;
	JL_TIMER3->PRD = sys_clk/(6*670L);//1KHZ  频率
	JL_TIMER3->PWM = (JL_TIMER3->PRD*duty)/10;//占空比(0-10;0是低电平，10是高电平)
	JL_TIMER3->CON = (1<<4) | (1<<8)| (1<<0) |(2<<2); //

	/*JL_TIMER3->CNT = 0;// 不需用到的寄存器,初始化为  0
	JL_TIMER3->PWM = 15000;//占空比计算 	 75/7500	  1%
	JL_TIMER3->PRD = 30000;//周期频率  7500*4/60M == 500us
	JL_TIMER3->CON &= ~(BIT(8)| BIT(0));*/
	// 选择系统时钟(LSB),1 分频,使能 PWM, 配置为 PWM 模式
}
#endif

#if defined(_ENABLE_BEEP_)
#if defined(_ENABLE_BEEP_FREQ)
void set_beep_pwm(void)
{
	uint8 pwm = 0;

	if(setup.aud_beep)
	{
		pwm = 1;
		global.beep_time = 2;
    #if defined(_XM007_SPECIAL_MUTE_)
		printf("global.aud_mute:%d\n",global.aud_mute);
    	if(global.aud_mute)
    	{
    		if(!global.xm_mute_flag)
    		{
    			ui_disp.timer = 0;
    			global.aud_mute = !global.aud_mute;
    			func_mute_control();
    			global.xm_mute_flag = FALSE;
    		}
    	}
    #endif
		set_panel_pwm(pwm, pwm, pwm);
	}
	else
	{
		pwm = 0;
		set_panel_pwm(pwm, pwm, pwm);
	}
}

#else

extern void set_panel_pwm(uint8 r, uint8 g, uint8 b);
void set_beep_pwm(void)
{
	uint8 pwm = 0;

	if(setup.aud_beep)
	{
		ew_puts("set_beep_pwm\n");

		pwm = 1;
		global.beep_time = 3;
	}
	else
	{
		ew_puts("set_beep_pwm111\n");
		pwm = 0;
	}
#if defined(SET_BEEP_FUNCV)
	set_beep_ctrl(pwm);
#else
	set_panel_pwm(pwm, pwm, pwm);
#endif
}
#endif
#endif

#if defined(_ENABLE_RCA_MUTE_)//修改转模式RCA有杂音
void set_sub_mute(void)
{
#if !defined(_DISABLE_SUB_OFF_)

	global.sub_flag = TRUE;
	global.sub_time = 0;
#if defined(_SUPPORT_SUBWOOFER2_)
	if(setup.aud_sub)
	{
	#if !defined(_DISABLE_SUB_OFF_)
		SYS_SUB_OFF();
	#endif
	}
#endif
	
#if defined(_SUPPORT_SUBWOOFER3_)
	if(setup.aud_sub1)
	{
	#if !defined(_DISABLE_SUB_OFF_)
		SYS_SUB2_OFF();
	#endif
	}
#endif

#endif
}
#endif

#if defined(_CHANGE_ACC_POWER_ON_FLOWT)
typedef struct{
	uint8 test;
	uint8 acc_det_lock;
	uint8 acc_stable_cnt;
	uint8 acc_unstable_cnt;
	uint8 acc_stable_ok;
	uint8 lcd_open_dly;
}IdlePower_t;
IdlePower_t IdlePower;

uint8 GetAccStable(void)
{
	return IdlePower.acc_stable_ok;
}

void AccOnStableDet(void)
{
	if(IdlePower.lcd_open_dly)
	{
		IdlePower.lcd_open_dly--;
		if(IdlePower.lcd_open_dly==0)
		{
			power_up_task();
			os_taskq_post_event("CheckTask", 1, SYS_EVENT_LGDEV_ONLINE);
		}
	}
	
	if(IdlePower.acc_det_lock)return;
	
	if(!ACC_CHECK())
	{
		IdlePower.acc_unstable_cnt++;
		if(IdlePower.acc_unstable_cnt>3) 
		{
			IdlePower.acc_det_lock = TRUE;
			IdlePower.acc_unstable_cnt = 0;
			IdlePower.lcd_open_dly = 1;
			printf("Acc Detect unstable\n");
			//os_taskq_post_msg(IDLE_TASK_NAME, 1, SYS_EVENT_ACC_OFF);
		}
		IdlePower.acc_stable_cnt=0;
	}
	else
	{
		IdlePower.acc_unstable_cnt = 0;
		IdlePower.acc_stable_cnt++;
		if(IdlePower.acc_stable_cnt>5)
		{
			IdlePower.acc_det_lock = TRUE;
			IdlePower.acc_stable_cnt = 0;
			IdlePower.acc_stable_ok = TRUE;
			IdlePower.lcd_open_dly = 2;
			SYS_5VPOW_INIT();
			SYS_5VPOW_ON();
			printf("Acc Detect stable\n");
		}
	}
}

static void Main_timer(void *param)
{
	os_taskq_post_msg(MAIN_TASK_NAME, 1, SYS_EVENT_SYS_TIMER);
}
#endif

#if defined(_ENABLE_MODE_CHANNEL_)
void set_mode_funtion(void)
{
	if(compare_task_name("FMRadioTask"))
	{
		if(idle_usb_state())
		{
			setup.mode_start = 0;
			task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
		}
		else
		{
			setup.mode_start = 0;
			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
			audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_ON, TRUE);
		}
	}
	else if(compare_task_name("MusicTaskUsb"))
	{
		ew_puts("AUX\n");
		setup.mode_start = 0;
		task_switch("LineinTask",0,SWITCH_SPEC_TASK);//AUX
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_ON, TRUE);
	}
#if defined(_DISBLE_MODE_OPTICAL_)
	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 0))
	{
		ew_puts("TV\n");

		sys_mute_ctrl(ENABLE, 1000);
		setup.mode_start = 2;
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_TV_ON, TRUE);//TV

		ui_show_setting(LANG_ID_TV, 0);
		ui_show_normal(LANG_ID_TV);
	}
	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 2))
	{
		ew_puts("DVD\n");
		sys_mute_ctrl(ENABLE, 1000);
		setup.mode_start = 3;
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DVD_ON, TRUE);//DVD

		ui_show_setting(LANG_ID_DVD, 0);
		ui_show_normal(LANG_ID_DVD);
	}

#else
	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 0))
	{
		ew_puts("OPTICAL\n");

		sys_mute_ctrl(ENABLE, 1000);
		setup.mode_start = 1;
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);//OPTICAL
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_OPTICAL_ON, TRUE);

		ui_show_setting(LANG_ID_OPTICAL, 0);
		ui_show_normal(LANG_ID_OPTICAL);
	}
	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 1))
	{
		ew_puts("TV\n");
		sys_mute_ctrl(ENABLE, 1000);
		setup.mode_start = 2;
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_TV_ON, TRUE);//TV
		ui_show_setting(LANG_ID_TV, 0);
		ui_show_normal(LANG_ID_TV);
	}
	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 2))
	{
		ew_puts("DVD\n");
		sys_mute_ctrl(ENABLE, 1000);
		setup.mode_start = 3;
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DVD_ON, TRUE);//DVD

		ui_show_setting(LANG_ID_DVD, 0);
		ui_show_normal(LANG_ID_DVD);
	}
#endif
	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 3))
	{
		ew_puts("BT\n");
		setup.mode_start = 0;
		task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
	}
	else if(compare_task_name("BtUserTask"))
	{
		ew_puts("RADIO\n");
		setup.mode_start = 0;
		task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
	}
}

void set_mode_control(void)
{
#if defined(_ENABLE_IPHONE_CHARGE_)
    if(global.charge_flg)
    {
    	set_mode_funtion();
    }
    else
    {
    	if(compare_task_name("FMRadioTask"))
    	{
			setup.mode_start = 0;
			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
			audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_ON, TRUE);
    	}
    #if defined(_DISBLE_MODE_OPTICAL_)
    	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 0))
    	{
    		ew_puts("TV\n");

    		sys_mute_ctrl(ENABLE, 1000);
    		setup.mode_start = 2;
    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);
    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_TV_ON, TRUE);//TV
    		ui_show_setting(LANG_ID_TV, 0);
    		ui_show_normal(LANG_ID_TV);
    	}
    	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 2))
    	{
    		ew_puts("DVD\n");
    		sys_mute_ctrl(ENABLE, 1000);
    		setup.mode_start = 3;
    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DVD_ON, TRUE);//DVD
    		ui_show_setting(LANG_ID_DVD, 0);
    		ui_show_normal(LANG_ID_DVD);
    	}

    #else
    	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 0))
    	{
    		ew_puts("OPTICAL\n");

    		sys_mute_ctrl(ENABLE, 1000);
    		setup.mode_start = 1;
    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);//OPTICAL
    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_OPTICAL_ON, TRUE);

    		ui_show_setting(LANG_ID_OPTICAL, 0);
    		ui_show_normal(LANG_ID_OPTICAL);
    	}
    	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 1))
    	{
    		ew_puts("TV\n");
    		sys_mute_ctrl(ENABLE, 1000);
    		setup.mode_start = 2;
    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_TV_ON, TRUE);
//TV

    		ui_show_setting(LANG_ID_TV, 0);
    		ui_show_normal(LANG_ID_TV);
    	}
    	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 2))
    	{
    		ew_puts("DVD\n");
    		sys_mute_ctrl(ENABLE, 1000);
    		setup.mode_start = 3;
    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DVD_ON, TRUE);//DVD

    		ui_show_setting(LANG_ID_DVD, 0);
    		ui_show_normal(LANG_ID_DVD);
    	}
    #endif
    	else if((compare_task_name("LineinTask"))&&(setup.mode_start == 3))
    	{
    		ew_puts("BT\n");
    		setup.mode_start = 0;
    		task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
    	}
    	else if(compare_task_name("BtUserTask"))
    	{
    		ew_puts("RADIO\n");
    		setup.mode_start = 0;
    		task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
    	}
    }
#else
	set_mode_funtion();
#endif
}
#endif

#if defined(_SPECIAL_MODE_FUNTION_)
static void SystemMusicMode(u8 keymode)
{
	if((compare_task_name("FMRadioTask"))||(compare_task_name("BtUserTask"))||(compare_task_name("LineinTask")))
	{
		if(idle_usb_state())
		{
			task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
		}
		else if(idle_card_state())
		{
			task_switch("MusicTaskCard",0,SWITCH_SPEC_TASK);
		}
	}
}
static void SystemRadioMode(u8 keymode)
{
	if(keymode == MSG_MODE_UP_LESS_THAN)
	{
		if(!compare_task_name("FMRadioTask"))
		{
			task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
		}
	}
}
#endif

extern u8 le_hw_waiting_idle(void);
extern u8 is_busy_in_update_conn_param(void);

#if defined(_ENABLE_MEMORY_PASSWORD_)
extern uint8 pwd_set_save[4];
#endif

#if defined(_ENABLE_RADIO_RDS_)
extern void Spp_Tx_Radio_Rds(uint8 ta,uint8 af,uint8 pty);
#endif

#if defined(_YH1701_USB_STANBY_)
uint8 detect_usb_state1(void)
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
#endif

#if defined(_SUPPORT_PASSWORD_DISPLAY_)
extern uint8 password_number1;
extern uint8 password_number2;
extern uint8 password_number3;
extern uint8 password_number4;
#endif

#if defined(_SUPPORT_TOUCH_KEY_)
extern void touch_key_init(void);
#endif

#if defined(_UPDATA_CLEAN_MEMORY_)
u8 usb_updata = 0;
#endif

#if defined(_ENABLE_AUX_RESET_)
u8 soft_reset = 0;
#endif

static void TaskMain(void *p)
{
    int msg[6];
    u32 status;
    u32 bt_status = 0;
	u8 test_box_vol_up=0x41;
	u8 test_box_vol_down=0x42;
	const char *next_name = NULL;
	const char *task_name = NULL;
	uint8 power_msg = FALSE;
	uint8 bt_work_flag = FALSE;

	main_puts("****TaskMain********\n");
	update_deal(UPDATE_OK_WHILE);
    JL_SYSTEM->LVD_CON |= BIT(2);
#if defined(_SUPPORT_EXT_MEMORY_)
	ext_memory_init();
#endif

#if DAC_AUTO_MUTE_EN
	digit_auto_mute_set(AUTO_MUTE_CFG, 4, 1200, 200);
#endif
    resourse_manage_init();
	setup_init();
	
#if defined(TD_ADD_NEW_APP)&&defined(_AUDIO_DRIVER_AP3768_)
	extern void ap3768_memory_init(void);
	ap3768_memory_init();
#endif

#if defined(USB_MODE_ACC_ON_OK)
	if(strcmp(setup.task_name, MUSIC_USB_NAME)==0)
	{
		SYS_POW_ON();
    #if defined(_ENABLE_POWER_PIN_)
    	dac_ldo_set(LDO_1,1);
    	dac_ldo_set(LDO_2,1);
    #endif
	}
#endif

    audio_init();
    dev_ctrl_sys_init(0);
	task_manager_init();
	task_name = setup.app_name;

#if defined(_CHANGE_ACC_POWER_ON_FLOWT)	
	memset(&IdlePower, 0, sizeof(IdlePower_t));
	timer_reg_func(Main_timer, NULL, 100);
#endif

#if defined(_SUPPORT_LCM_DRIVE)||defined(_SUPPORT_LCM_1618_DRIVE)
	Lcm_Drive_Init();
#endif
#if defined(_VFD_DRIVER_IC_TM1729_)&&0
	extern void Tm1729LcdOnPwrInit(void);
	Tm1729LcdOnPwrInit();
#endif	
#if defined(_ENABLE_ALARM_CLOCK_)
	PWM0_init();
#endif

#if defined(_ENABLE_TEMP_PWM_)
	PWM0_init(0);
#endif

#if defined(_ENABLE_UART_LOW_)
	if(setup.ap3768_power_flag1)
	{
		extern s32 audio_uart_init(u32 baud);
		audio_uart_init(115200);
	}
#endif

#if defined(_ENABLE_PIN_CONTROL_)
    SYS_POW_ON();
#endif

#if defined(_ENABLE_ACC_OFF_LOGO_)
	global.first_power_on = TRUE;
#endif
#if defined(_ENABLE_USB_DELAY_)||defined(_SLEEP_MODE_USB_DONT_DETCT)||defined(_SLEEP_MODE_USB_DONT_DETCT)
	global.pwrstarttime = FALSE;
	global.usb_det_delay = 50;
#endif

#if defined(_SUPPORT_30P60_LIGHT_)
#if defined(_ENABLE_LIGHT_SHADE_)
	LightDriverIrCmd(0x00,0,0,0);
#else
	LightDriverIrCmd(0x00);
#endif
#endif

#if defined(_POWER_OFF_FUNTION_)
	SYS_POW_ON();
#endif

#if defined(_YH1701_SPECIAL_FUNTION_)
	SYS_MCU_INIT();
	SYS_MCU_ON();
#endif

#if defined(_ENABLE_POWER_SYNCHRO_)
	SYS_POW_INIT();
	SYS_POW_ON();
#endif

#if defined(_ENABLE_USB_CONTROL_)
	SYS_USB_ON();
#endif

#if defined(_SUPPORT_TOUCH_KEY_)
	//os_time_dly(10);
	touch_key_init();
#endif

#if defined(_ENABLE_AUX_RESET_)
	soft_reset = 0;
	vm_write_api(VM_SOFT_RESET, &soft_reset);
#endif

    while(1)
    {
        memset(msg,0x00,sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
	#if defined(_SUPPORT_DTH_)
		if(global.main_msg_disable)
		{
			continue;
		}
	#endif

		//蓝牙初始化和搜索需要屏蔽一些切换模式的消息
		bt_status = msg_mask_off_in_bt();
		//ew_green_printf("%s, msg0:%d, msg1:%d, bt_status:%d\n", __FUNCTION__, msg[0], msg[1], bt_status);

		if((bt_status)
	#ifdef SUPPORT_CALL_NAMES
		||(play_song_flag)
	#endif
        )
		{
			main_printf("bt_status_waring_msg:%d\n", msg[0]);

			switch(msg[0])
        	{
				case SYS_EVENT_LGDEV_ONLINE:
				case SYS_EVENT_LGDEV_OFFLINE:
				case SYS_EVENT_TASK_RUN_REQ:
				case SYS_EVENT_POWER:
				case SYS_EVENT_ACC_OFF:
			#if defined(_SUPPORT_DTH_)
				case SYS_EVENT_DTH_OFF:
			#endif
			#if defined(PLANE_LIGHT_FUNC)||defined(ENABLE_PLANE_LIGHT_CONTROL_)
				case SYS_EVENT_ILL_LIGHT_OFF:
			#endif
			#if defined(_SUPPORT_TAPE_)
				case SYS_EVENT_TAPE_OUT:
				case SYS_EVENT_TAPE_IN:
			#endif
		    #if defined(_SUPPORT_MIC_OK_)
		    	case SYS_EVENT_MIC_OUT:
		    	case SYS_EVENT_MIC_IN:
		    #endif
			#if defined(_SUPPORT_DISC_)
				case SYS_EVENT_PLAYER_DISC_IN:
				case SYS_EVENT_PLAYER_DISC_OUT:
				case SYS_EVENT_PLAYER_USB_IN:
				case SYS_EVENT_PLAYER_USB_OUT:
				case SYS_EVENT_PLAYER_CARD_IN:
				case SYS_EVENT_PLAYER_CARD_OUT:
			#endif

					break;

				case 0 ... MSG_MAIN_MAX:
				case MAIN_DISPLAY_BEGIN ... MAIN_DISPLAY_END:
					break;

				default:
                    msg[0] = MSG_NULL;
					break;
			}
		}

		switch(msg[0])
		{
        	case SYS_EVENT_LGDEV_ONLINE:
            	main_printf("main--->SYS_EVENT_LGDEV_ONLINE--%08x-%08x\n",msg[1],msg[2]);

            #if defined(_ENABLE_TEA685x_INIT_)||defined(_ENABLE_TDA770x_INIT_)
            	global.init = TRUE;
            #endif

		    #if defined(_ENABLE_TIME_DISP_)
		    	global.timer_count = 4000;
		    #endif
			
				status = lg_dev_status_chg(SYS_EVENT_LGDEV_ONLINE,(void *)msg[1],msg[2]);
				if((int)usb == msg[1])
				{
					main_printf("main--->usb__msg[1]=%08x-%08x\n",msg[1],msg[2]);
				#if defined(_ENABLE_USB_SAVE_)||defined(_ENABLE_USB_DELAY_)||defined(_SLEEP_MODE_USB_DONT_DETCT)
					if(!global.power_time)
				#endif
					{
						main_printf("main--->DISP_EVENT_ICON_USB\n");
					#if !defined(USB_SD_MODE_WORK_ONOFF)
					#if defined(_EVB_USB_CHANGE_MODE_)
						if(global.charge_flag)
						{
							disp_send_param(DISP_EVENT_ICON_USB, OFF);
						}
						else
						{
							disp_send_param(DISP_EVENT_ICON_USB, ON);
						}
					#else
						disp_send_param(DISP_EVENT_ICON_USB, ON);
					#endif
					#endif
					}
				}
			#if defined(_SUPPORT_SD_)
				else if((int)sd1 == msg[1])
				{
				#if defined(_ENABLE_USB_SAVE_)
				#if !defined(_EVB_USB_DELAY_)
					if(!global.power_time)
				#endif
				#endif
					{
    					main_printf("main--->DISP_EVENT_ICON_SD\n");
    				#if !defined(USB_SD_MODE_WORK_ONOFF)
    					disp_send_param(DISP_EVENT_ICON_SD, ON);
    				#endif
					}
				}
			#endif

            	if((status!=FILE_OP_ERR_LGDEV_FULL)/*&&(status!='A')*/)
	            {
					main_printf("main--->status!=FILE_OP_ERR_LGDEV_FULL\n");
					main_printf("main--->1111status=%c\n",status);
	                if(((char)status == 'A'))       ///< SPIFLASH璁惧mount
	                {//第一次上电自动执行这里
						main_printf("main--->2222status=%c\n",status);
						lg_dev_mount((void *)msg[1],status,msg[2]);
	                    main_puts("power up\n");
					#if defined(_XM003_USB_DELAY_)
						os_time_dly(200);	 //延时1s，防止开关acc优盘播歌无记忆
					#else
						os_time_dly(100);    //延时1s，防止开关acc优盘播歌无记忆
					#endif				
					#if !defined(_CHANGE_ACC_POWER_ON_FLOWT)
						power_up_task();
						os_taskq_post_event("CheckTask", 1, SYS_EVENT_LGDEV_ONLINE);
					#endif
	                    break;
	                }
	                else
	                {//'B'
	               		main_printf("main--->3333status=%c\n",status);
	                    new_lg_dev_let = status;
	                }
				#ifdef POWERUP_DEV_IGNORE
	                if(!msg[3])
	                {
						main_printf("main--->444msg[3]=%08x\n",msg[3]);
						break;
					}
				#endif
	                if(((os_time_get() > 200) && (!bt_status))
				#ifdef SUPPORT_CALL_NAMES
					&&(!play_song_flag)
				#endif
                    )
	                {
	                    if(compare_task_name(UDISK_TASK_NAME)) ///<PC Audio
	                        break;
						
				    #if defined(_SUPPORT_PASSWORD_DISPLAY_)
				    	if(setup.password_flag)break;
				    #endif

						if((int)usb == msg[1])
						{//	usb
							main_puts("main--->music usb11111111111\n");
					    #if defined(_ENABLE_IPHONE_CHARGE_)
					    	setup.usb_detect = FALSE;
					    #endif
						#ifdef	TD_ADD_ALEN
						#if !defined(ENBLE_USB_MEMORY)
						#if !defined(_EVB_USB_DELAY_)
							setup.usb_break =0;//解决USB时关机 拔USB 再插USB有记忆的问题
    					#endif
						#endif
    					#endif

						#if defined(_ENABLE_USB_SAVE_)||defined(_ENABLE_USB_DELAY_)||defined(_SLEEP_MODE_USB_DONT_DETCT)
							if(!global.power_time)
						#endif
							{
							main_puts("main--->music usb2\n");
							#if defined(_SLEEP_MODE_USB_DONT_DETCT)
								if(setup.task_name == "MusicTaskUsb");
								else switch_activation_task(MUSIC_USB_NAME);
							#else
								switch_activation_task(MUSIC_USB_NAME);
							#endif
							}
						}
					#if defined(_SUPPORT_SD_)
						else if((int)sd1 == msg[1])
						{//	card
							main_puts("main--->music card\n");
						#ifdef	TD_ADD_ALEN
						#if !defined(ENBLE_SD_MEMORY)
							setup.sd_break =0;//解决SD时关机 拔SD 再插SD有记忆的问题
						#endif
						#endif
						#if defined(_ENABLE_USB_SAVE_)
						#if !defined(_EVB_USB_DELAY_)
							if(!global.power_time)
						#endif
						#endif
							{
								switch_activation_task(MUSIC_CARD_NAME);
							}
						}
					#endif
	                }
	            }
            break;
			
	#if defined(_CHANGE_ACC_POWER_ON_FLOWT)
		case SYS_EVENT_SYS_TIMER:
			AccOnStableDet();
			break;
	#endif
	
        case SYS_EVENT_LGDEV_OFFLINE:
            main_puts("main--->SYS_EVENT_LGDEV_OFFLINE\n");
			
	    #if defined(_ENABLE_RCA_MUTE_)//修改放音RCA有杂音
	    	set_sub_mute();
	    #endif
		
		#if defined(_DISABLE_AMP_OFF_)
		#if !defined(_DISABLE_USB_OFF_AMP_)
			global.amp_flag = TRUE;
			AMP_CONTROL_OFF();
		#endif
		#endif

			if((int)usb == msg[1])
			{
				main_puts("main--->SYS_EVENT_LGDEV_OFFLINE1111\n");
			#ifdef	TD_ADD_ALEN
		    #ifdef	TD_ADD_IPONE_CHARGE
		        usb_insert =FALSE;
		    	usb_ERR =FALSE;
		    #endif
			#if !defined(ENBLE_USB_MEMORY)
			    if(ACC_CHECK())
			    {
					main_puts("main--->SYS_EVENT_LGDEV_OFFLINE9999999\n");
				#if defined(_EVB_USB_DELAY_)
					if(global.sys_power)
					{
						setup.usb_break =0;//解决USB时关机 拔USB 再插USB有记忆的问题
					}
				#else
				    setup.usb_break =0;//解决USB时关机 拔USB 再插USB有记忆的问题
				#endif
			    }
			#endif
			#endif

		    #if defined(_ENABLE_ACC_MODE_)
		    	if(global.sys_power)
		    	{
		    		setup.playusb_flg = FALSE;
		    	}
		    #endif
			
		    #if defined(_USB_OUT_MEMORY_)
		    	setup.usb_break =0;
		    #endif
			    disp_send_param(DISP_EVENT_ICON_USB, OFF);
			
            #if defined(_CHANGE_MUTE_DISP_)
			    if(global.aud_mute)
			    {
                   global.mute_disp = TRUE;
				}
			#endif
			
			#if defined(_NO_REMEMBER_PLAY_STATUS_)
			    global.play_status_flag = TRUE;
            #if defined(_ACC_OFF_MEMORY_INT_)
            	setup.int_flag = FALSE;
            #endif	
			#endif
			
			#if defined(_SUPPORT_RECORD_)
				if(compare_task_name("RECTask"))
				{
					task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
				}
			#if defined(_SUPPORT_TAPE_)
				else if(compare_task_name("TapeTask"))
				{
					rec_status = rec_stop;
				}
			#endif
			#endif
			}
		#if defined(_SUPPORT_SD_)
			else if((int)sd1 == msg[1])
			{
			#ifdef	TD_ADD_ALEN
			#if !defined(ENBLE_SD_MEMORY)
				setup.sd_break =0;//解决SD时关机 拔SD 再插SD有记忆的问题
			#endif
			#endif
			main_puts("main--->SYS_EVENT_LGDEV_OFFLINE3333333333\n");
			
		    #if defined(_ENABLE_ACC_MODE_)
		    	if(global.sys_power)
		    	{
		    		setup.playsd_flg = FALSE;
		    	}
		    #endif
			
		    #if defined(_USB_OUT_MEMORY_)
		    	setup.sd_break =0;
		    #endif
				disp_send_param(DISP_EVENT_ICON_SD, OFF);

            #if defined(_CHANGE_MUTE_DISP_)
				if(global.aud_mute)
				{
				   global.mute_disp = TRUE;
				}
            #endif
            
            #if defined(_NO_REMEMBER_PLAY_STATUS_)
            	global.play_status_flag = TRUE;
            #if defined(_ACC_OFF_MEMORY_INT_)
                setup.int_flag = FALSE;
            #endif
            #endif
						
			#if defined(_SUPPORT_RECORD_)
				if(compare_task_name("RECTask"))
				{
					task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
				}
			#if defined(_SUPPORT_TAPE_)
				else if(compare_task_name("TapeTask"))
				{
					rec_status = rec_stop;
				}
			#endif
			#endif
			}
		#endif

			main_puts("main--->SYS_EVENT_LGDEV_OFFLINE2222\n");
	    #if 0//defined(ENBLE_SD_MEMORY)||defined(ENBLE_USB_MEMORY)//没有识别到U盘自己切模式，这样可以有记忆
           if(compare_task_name(MUSIC_USB_NAME))
           {
		     	if((int)usb == msg[1])
		     	{
					main_puts("main--->SYS_EVENT_LGDEV_OFFLINE555\n");
		     	#if defined(_USB_OUT_MODE_RADIO_)
				#if defined(_ENABLE_PLAY_ERROR_)
					global.play_err= TRUE;
    				ui_show_setting(LANG_ID_USB_ERR, 0);
					ui_disp.timer = 1000;
					ui_show_normal(LANG_ID_USB_ERR);
					OSTimeDly(200);
				#endif
					task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
				#else
					os_taskq_post(MAIN_TASK_NAME, 1, MSG_CHANGE_WORKMODE);
				#endif
				#if defined(_USB_OUT_MEMORY_)
				    setup.usb_break =0;
				#endif
				}
		   }
		   else if(compare_task_name(MUSIC_CARD_NAME))
		   {
		   		if((int)sd1 == msg[1])
		     	{
					main_puts("main--->SYS_EVENT_LGDEV_OFFLINE666\n");
				#if defined(_USB_OUT_MODE_RADIO_)
			    #if defined(_ENABLE_PLAY_ERROR_)
					global.play_err= TRUE;
			    	ui_show_setting(LANG_ID_SD_ERR, 0);
					ui_disp.timer = 1000;
					ui_show_normal(LANG_ID_SD_ERR);
					OSTimeDly(200);
			    #endif
					task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
				#else
					os_taskq_post(MAIN_TASK_NAME, 1, MSG_CHANGE_WORKMODE);
				#endif
				#if defined(_USB_OUT_MEMORY_)
				    setup.sd_break =0;
				#endif
				}
		   }
		#else
		
        #if defined(_ENABLE_PLAY_ERROR_)
			if(compare_task_name(MUSIC_USB_NAME))
			{
				global.play_err= TRUE;
				ui_show_setting(LANG_ID_USB_ERR, 0);
				ui_disp.timer = 1000;
				ui_show_normal(LANG_ID_USB_ERR);
				OSTimeDly(200);
			}
			else if(compare_task_name(MUSIC_CARD_NAME))
			{
				global.play_err= TRUE;
				ui_show_setting(LANG_ID_SD_ERR, 0);
				ui_disp.timer = 1000;
				ui_show_normal(LANG_ID_SD_ERR);
				OSTimeDly(200);
			}
        #endif
			//ew_printf("keymsg_task_name:%s\n",keymsg_task_name);
		    if(keymsg_task_name)
			{
				//main_puts("main--->keymsg_task_name111111\n");	
				os_taskq_post_event((char *)keymsg_task_name, 2, SYS_EVENT_DEV_OFFLINE, msg[1]);
			#if defined(_CHANGE_MUTE_DISP_)	
				if(global.aud_mute)
    			{
   				    disp_send_msg(DISP_EVENT_MUTE);
    			}
    		#endif	
			}
            lg_dev_status_chg(SYS_EVENT_LGDEV_OFFLINE,(void *)msg[1],1);
		#endif
		
		#ifdef  TD_ADD_NEW_APP
			if(ACC_CHECK())
			{
				if(compare_task_name(MUSIC_USB_NAME))
				{
					 if((int)usb == msg[1])
					 {
						os_taskq_post_event(MAIN_TASK_NAME,2,SYS_CLR_LIST,0x2);
					 }
				}
				else if(compare_task_name(MUSIC_CARD_NAME))
				{
					 if((int)sd1 == msg[1])
					 {
						os_taskq_post_event(MAIN_TASK_NAME,2,SYS_CLR_LIST,0x1);
					 }
				}
			}
       #endif
			main_puts("main--->keymsg_task_name222222\n");
            break;

		case SYS_EVENT_AUX_IN:
			main_puts("main--->SYS_EVENT_AUX_IN\n");
		#ifdef POWERUP_DEV_IGNORE
			if(!msg[1])break;
		#endif
			if(os_time_get() > 200)
			{
				task_switch(LINEIN_TASK_NAME,0,SWITCH_SPEC_TASK);
			}
            break;

		case SYS_EVENT_AUX_OUT:
			main_puts("main--->SYS_EVENT_AUX_OUT\n");
            if(compare_task_name(LINEIN_TASK_NAME))
            {
            #if defined(_SL6812_AUX_DETECT_)
				task_switch(FM_TASK_NAME,0,SWITCH_SPEC_TASK);
			#else
                task_switch(0,0,SWITCH_NEXT_TASK);
			#endif
            }
			break;

        case SYS_EVENT_PC_IN:
            main_puts("main--->SYS_EVENT_PC_IN\n");
		#ifdef POWERUP_DEV_IGNORE
			if(!msg[1])break;
		#endif
			if(os_time_get() > 200)
			{
            	task_switch(PC_TASK_NAME, 0, SWITCH_SPEC_TASK);
			}
            break;

        case SYS_EVENT_PC_OUT:
            main_puts("main--->SYS_EVENT_PC_OUT\n");
            if(compare_task_name(UDISK_TASK_NAME))
            {
                task_switch(0, 0, SWITCH_NEXT_TASK);
            }
            break;


		case SYS_EVENT_POWER:
			main_puts("main--->SYS_EVENT_POWER\n");
			if(power_msg)
			{
				power_msg = FALSE;
		    #if defined(_DISABLE_MEMORY_POWER_)
		    	if(!setup.acc_flag)
		    	{
		    		main_puts("main--->SYS_EVENT_POWER000\n");
		    		task_name = setup.app_name;
		    	}
		    #endif
				if(task_name == NULL)
				{
					main_puts("main--->task_switch11111111111111\n");
				#if defined(_POWER_ON_AUX_)||defined(_DISABLE_RADIO_)
				#if defined(_POWER_ON_BT_)
		            task_switch(BT_TASK_NAME, 0, SWITCH_SPEC_TASK);
		        #else
					task_switch(LINEIN_TASK_NAME, 0, SWITCH_SPEC_TASK);
				#endif
				#else
					task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
				#endif
				}
				else
				{
					main_puts("main--->SYS_EVENT_POWER111\n");
				
				#if defined(_ENABLE_ACC_MODE_)
					if((!idle_usb_state()&&(setup.playusb_flg))||(!idle_card_state()&&(setup.playsd_flg)))
					{
						task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
						setup.playusb_flg = FALSE;
						setup.playsd_flg = FALSE;
					}
					else
					{
						task_switch(task_name,0,SWITCH_SPEC_TASK);
					}
				#else
					task_switch(task_name,0,SWITCH_SPEC_TASK);
				#endif
				}
			}
			else
			{
				main_printf("SYS_EVENT_POWER ---> setup.task_name:%s, next_name:%s\n",setup.task_name, next_name);
				if(next_name)
				{
					Interrupt =TRUE;
					main_puts("main--->task_switch2222222222222\n");
					check_task_switch(next_name);
					next_name = NULL;
				}
				else if(setup.task_name)
				{
					if((strstr(setup.task_name, "Task") != NULL) && strcmp(setup.task_name, "IdleTask"))
					{
					#if defined(_ENABLE_ACC_MODE_)
						if((!idle_usb_state()&&(setup.playusb_flg))||(!idle_card_state()&&(setup.playsd_flg)))
						{
							ew_puts("main--->SYS_EVENT_POWER555\n");
							task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
							setup.playusb_flg = FALSE;
							setup.playsd_flg = FALSE;
						}
						else
						{
							ew_puts("main--->SYS_EVENT_POWER666\n");
							check_task_switch(setup.task_name);
						}
					#else
						check_task_switch(setup.task_name);
					#endif
					}
					else
					{
						main_puts("main--->task_switch33333333333333333\n");
						task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
					}
					setup.task_name = NULL;
				}
				else
				{
					main_puts("main--->task_switch44444444444444\n");
					task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
				}
			}
			break;
		#if defined(UART_Servo_Pop)
		case MSG_uart_reveice://串口接收数据处理
			if(msg[1]==0x88)//串口接收到数据
			{
				Yhx_Dab_Pop_Work();
			}
			break;
  		#endif
		#if defined(TD_ADD_SPP)
		case MSG_spp_ble://串口接收数据处理
			printf("=TD_ADD_SPP=\n");
			Spp_Tx_Audio_ble();
			break;
  		#endif

		case SYS_EVENT_ACC_ON:
			main_puts("main--->SYS_EVENT_ACC_ON\n");
			main_printf("main1111--->SYS_EVENT_ACC_ON=%d\n",gEnterAccMode);

		#if !defined(_AUDIO_DRIVER_AP3768_)
        #if defined(_DISABLE_ACC_OFF_DISPLAY_)||defined(_ACC_OFF_DISP_LOGO_)||defined(_ACC_OFF_POWER_LOGO_)||defined(_DISABLE_ACC_OFF_DISPLAY_CONNECT_)
        	global.bt_off = FALSE;
        #endif
		#endif
				
		#if !defined(_ENABLE_RTC_CLK_)
            if(gEnterAccMode)
            {
                gEnterAccMode =FALSE;
	            JL_POWER->CON |= BIT(4);
            }
		#endif
		#if defined(DAB_ACC_ON_ERROR_)
			if(strcmp(setup.task_name,DAB_TASK_NAME))
			{
				DAB_ACC = 0;
			}
		#endif
			break;
	#if REC_EN
		case MSG_SYS_REC:
			if(detect_usb_card_state())//有U盘或者sd卡
			{
				main_puts("main--->MSG_SYS_REC\n");
		        if(rec_status == rec_start)
		        {
					os_taskq_post((char *)curr_task->name,1,MSG_REC_STOP);
		        }
				//else if(rec_status == rec_pp)
				//{
				//	os_taskq_post((char *)curr_task->name,1,MSG_REC_PP);
				//}
				else if(rec_status == rec_stop)
				{
					os_taskq_post((char *)curr_task->name,1,MSG_REC_START);
				}
			}
			break;
	#endif

		case SYS_EVENT_ACC_OFF:
			main_puts("main--->SYS_EVENT_ACC_OFF\n");
			
	    #if defined(_ENABLE_BT_MODE_MEMORY_)
	    	setup.acc_bt = TRUE;
	    #endif
		
	    #if defined(_ENABLE_BT_CONNECT_)
	    	if(setup.bt_connect_flag)
	    	{
	    		setup.acc_bt_flag = TRUE;
	    		setup.bt_connect_flag = FALSE;
	    	}
	    #endif

		#if defined(_ACC_OFF_DISP_LOGO_)
			ui_show_setting(LANG_ID_SYSTEM_GOODBYE, 0);
			ui_disp.timer = 2000;
			ui_show_normal(LANG_ID_SYSTEM_GOODBYE);
		#endif
		
		#if defined(_ENABLE_TEA685x_INIT_)
			global.init = TRUE;
		#endif
		
	    #if defined(_ENABLE_TDA770x_INIT_)
	    	global.init = FALSE;
	    #endif

		#if defined(_DISABLE_ACC_OFF_DISPLAY_)||defined(_ACC_OFF_DISP_LOGO_)||defined(_DISABLE_ACC_OFF_DISPLAY_CONNECT_)
			global.bt_off = TRUE;
		#endif
		
        #if defined(_REMEBER_FLAG_STATE_)
		    setup.memory_play = TRUE;
	    #endif
				
        #if defined(_ENABLE_A2DP_PLAY_)
        	global.bt_start = TRUE;
        #endif

        #if defined(_ENABLE_BT_POWER_)
        	if(setup.bt_talk)
        	{
        		setup.acc_off = TRUE;
        		ew_printf("setup.acc_off:%d\n",setup.acc_off);
        	}
        #endif
		
		#if defined(TD_ADD_ACCOFF_TIMEOUT_RESET)
			gAccOffFlag=TRUE;
		#endif
	    #if defined(_SUPPORT_DISC_)
	    	disc_drv_disable();
	    #endif

			if(global.sys_normal)
			{
				if(global.sys_power)
				{
					if(strcmp(curr_task->name,IDLE_TASK_NAME))
					{
						if(bt_work_flag)
						{
							main_puts("main--->SYS_EVENT_ACC_OFF777\n");
							bt_work_flag = FALSE;
							setup.task_name = get_last_task_name();
						}
						else
						{
							main_puts("main--->SYS_EVENT_ACC_OFF888\n");
							setup.task_name = curr_task->name;
						}
					}
				}
				else
				{
					main_puts("main--->SYS_EVENT_ACC_OFF999\n");
					setup.task_name = curr_task->name;
				}
			}
			
			printf("setup.task_name:%s\n", setup.task_name);
			
			if(strcmp(curr_task->name,IDLE_TASK_NAME))
			{
				main_puts("main--->SYS_EVENT_ACC_OFF111\n");
				if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
				{
					main_puts("main--->SYS_EVENT_ACC_OFF555\n");
					setup.a2dp_flag = TRUE;
				}
				/*if(bt_status && (!strcmp(curr_task->name, BT_TASK_NAME)))
				{
					if(post_sync_event(BT_TASK_NAME, MSG_BT_DISCONNECT, 0))
					{//发送同步消息
						os_time_dly(2);
					}
					else
					{
						os_taskq_post_event(BT_TASK_NAME, 1, MSG_BT_DISCONNECT);
						os_time_dly(20);
					}
				}*/
				if((!strcmp(curr_task->name, BT_TASK_NAME)))
				{
					main_puts("main--->SYS_EVENT_ACC_OFF222\n");
					if(get_bt_connect_status()>BT_STATUS_AUTO_CONNECTINT)
					{
						main_puts("main--->SYS_EVENT_ACC_OFF333\n");
						if(post_sync_event(BT_TASK_NAME, MSG_BT_DISCONNECT, 0))
						{//发送同步消息
							os_time_dly(60);
						}
						else
						{
							main_puts("main--->SYS_EVENT_ACC_OFF444\n");
							os_taskq_post_event(BT_TASK_NAME, 1, MSG_BT_DISCONNECT);
							os_time_dly(60);
						}
					}
				}
				task_switch(IDLE_TASK_NAME,0,SWITCH_SPEC_TASK);
			}
            AUDIO_MUTE_ON();
	    #if defined(_ENABLE_RCA_MUTE_)
	    	RCA_MUTE_ON();
	    #endif
            sys_mute_ctrl(ENABLE, 0);
			os_taskq_post_event(IDLE_TASK_NAME, 1, SYS_EVENT_ACC_OFF);
			break;

	#if defined(_SUPPORT_DTH_)
		case SYS_EVENT_DTH_ON:
			main_puts("main--->SYS_EVENT_DTH_ON\n");
			break;

		case SYS_EVENT_DTH_OFF:
			main_puts("main--->SYS_EVENT_DTH_OFF\n");
			
	    #if defined(_ENABLE_BT_CONNECT_)
	    	if(setup.bt_connect_flag)
	    	{
				main_puts("-----main--->SYS_EVENT_DTH_OFF1111-----\n");
	    		setup.acc_bt_flag = TRUE;
	    		setup.bt_connect_flag = FALSE;
	    	}
	    #endif

			if(global.sys_normal)
			{
				if(global.sys_power)
				{
					if(strcmp(curr_task->name,IDLE_TASK_NAME))
					{
						if(bt_work_flag)
						{
							bt_work_flag = FALSE;
							setup.task_name = get_last_task_name();
						}
						else
						{
							setup.task_name = curr_task->name;
						}
					}
				}
				else
				{
					setup.task_name = curr_task->name;
				}
			}
			if(strcmp(curr_task->name,IDLE_TASK_NAME))
			{
				if(bt_status && (!strcmp(curr_task->name, BT_TASK_NAME)))
				{
					if(post_sync_event(BT_TASK_NAME, MSG_BT_DISCONNECT, 0))
					{//发送同步消息
						os_time_dly(2);
					}
					else
					{
						os_taskq_post_event(BT_TASK_NAME, 1, MSG_BT_DISCONNECT);
						os_time_dly(20);
					}
				}
				task_switch(IDLE_TASK_NAME,0,SWITCH_SPEC_TASK);
			}
			os_taskq_post_event(IDLE_TASK_NAME, 1, SYS_EVENT_DTH_OFF);
			break;
	#endif

	#if defined(PLANE_LIGHT_FUNC)||defined(ENABLE_PLANE_LIGHT_CONTROL_)
	#if !defined(_ENABLE_PIN_CONTROL_)
		case SYS_EVENT_ILL_LIGHT_ON:
			main_puts("main--->SYS_EVENT_ILL_LIGHT_ON\n");
			KB_LIGHT_ON();
        #if defined(_ENABLE_CONTROL_POWER_)
    		POWOR_LED_OFF();
        #endif
			break;

		case SYS_EVENT_ILL_LIGHT_OFF:
			main_puts("main--->SYS_EVENT_ILL_LIGHT_OFF\n");
			KB_LIGHT_OFF();
        #if defined(_ENABLE_CONTROL_POWER_)
    		POWOR_LED_ON();
        #endif
			break;
	#endif		
	#endif

	#if defined(_SUPPORT_TAPE_)
    	case SYS_EVENT_TAPE_IN:
    		main_puts("main--->SYS_EVENT_TAPE_IN\n");
    	#ifdef POWERUP_DEV_IGNORE
    		if(!msg[1])break;
    	#endif
			switch_activation_task(TAPE_TASK_NAME);
    		break;

    	case SYS_EVENT_TAPE_OUT:
    		main_puts("main--->SYS_EVENT_TAPE_OUT\n");
    		if(compare_task_name(TAPE_TASK_NAME))
    		{
				task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
    		}
    		break;
	#endif

    #if defined(_SUPPORT_MIC_OK_)
    	case SYS_EVENT_MIC_IN:
    		main_puts("main--->SYS_EVENT_MIC_IN\n");
			setup.save_name = curr_task->name;
		#ifdef POWERUP_DEV_IGNORE
    		if(!msg[1])break;
		#endif
			switch_activation_task(MIC_TASK_NAME);
    		break;

    	case SYS_EVENT_MIC_OUT:
    		main_puts("main--->SYS_EVENT_MIC_OUT\n");
    		if(compare_task_name(MIC_TASK_NAME))
    		{
				task_switch(setup.save_name, 0, SWITCH_SPEC_TASK);
    		}
    		break;
    #endif

	#if defined(_ENABLE_RADIO_RDS_)
		case SYS_EVENT_TA_ENTER:
			//main_puts("main--->SYS_EVENT_TA_ENTER\n");
			if(keymsg_task_name)
			{
				os_taskq_post_event((char *)keymsg_task_name, 1, SYS_EVENT_TA_ENTER);
			}
			break;

		case SYS_EVENT_TA_EXIT:
			//main_puts("main--->SYS_EVENT_TA_EXIT\n");
			if(keymsg_task_name)
			{
				os_taskq_post_event((char *)keymsg_task_name, 1, SYS_EVENT_TA_EXIT);
			}
			break;
	#endif

		#if defined(_SUPPORT_USB_DET)
		case SYS_EVENT_USB_DET_OFF:
             main_puts("main--->SYS_EVENT_USB_DET_OFF\n");
			 USB_PWR_H();
			 OSTimeDly(20);
			 break;

		case SYS_EVENT_USB_DET_ON:
             main_puts("main--->SYS_EVENT_USB_DET_ON\n");
			 USB_PWR_L();
			 os_time_dly(500);
			 break;
		#endif

		case SYS_EVENT_BT_BACKCON:
            main_puts("main--->SYS_EVENT_BT_BACKCON\n");
            break;

		case SYS_EVENT_BT_BACKDISCON:
            main_puts("main--->SYS_EVENT_BT_BACKDISCON\n");
            break;

		case SYS_EVENT_TASK_RUN_REQ:
			main_puts("main--->SYS_EVENT_TASK_RUN_REQ\n");
			{
			#if defined(_BT_SPECIAL_MUTE_)
				global.bt_mute_flag = TRUE;
			#endif

			#if defined(_ENABLE_RDS_FIRST_)
				if(!global.rds_ta)
				{
				#if defined(_EVB_CALLING_MUTE_)
					AUDIO_MUTE_ON();
				#if defined(_ENABLE_RCA_MUTE_)
					RCA_MUTE_ON();
				#endif
					OSTimeDly(50);
				#endif
				}
			#else
			#if defined(_EVB_CALLING_MUTE_)
    			AUDIO_MUTE_ON();
		    #if defined(_ENABLE_RCA_MUTE_)
		    	RCA_MUTE_ON();
		    #endif
    			OSTimeDly(50);
			#endif
			#endif

				if(global.aud_mute)
				{
					disp_send_msg(DISP_EVENT_MUTE);
				}

				const char *task_name = (char *)msg[1];
				if(!strcmp(task_name, BTSTACK_TASK_NAME))
				{
					main_puts("main--->SYS_EVENT_TASK_RUN_REQ1111\n");
				#if defined(_ENABLE_BT_POWER_)
	                setup.bt_talk = TRUE;
				#endif
				#if defined(BT_PHONE_NUMBER)
					global.ring_flag = TRUE;
				#endif
					if(compare_task_name(IDLE_TASK_NAME))
	                {
	                main_puts("main--->SYS_EVENT_TASK_RUN_REQ222222222222\n");
					#if defined(_ENABLE_POWER_LOGO_)
						setup.save_task_name = "BtUserTask";
						setup.save_flag = TRUE;
					#endif
					
					#if defined(_BT_SPECIAL_MUTE_)
						AUDIO_MUTE_OFF();
				    #if defined(_ENABLE_RCA_MUTE_)
				    	RCA_MUTE_OFF();
				    #endif
					#endif
					#if defined(_BT_PAUSE_MUTE_)
						global.bt_ring_flag = TRUE;
					#endif
	                    os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_EXIT);
						next_name = BT_TASK_NAME;
	                }
					else if(compare_task_name(BT_TASK_NAME))
					{
						main_puts("SYS_EVENT_TASK_RUN_REQ ---333--- clean_suspend_flag\n");
					}
					else
					{
					main_puts("main--->SYS_EVENT_TASK_RUN_REQ444444444444444\n");
					#if defined(_BT_CALL_MEMORY_RPT_)
						global.memory_rpt = 0;
					#endif
					
					#if defined(_ENABLE_TEA685x_INIT_)||defined(_ENABLE_TDA770x_INIT_)
						global.init = TRUE;
					#endif

					#if defined(_ENABLE_RDS_FIRST_)
						if(!global.rds_ta)
						{
							check_task_switch(BT_TASK_NAME);
						}
						else
						{
							global.rds_mode = TRUE;
						}
					#else
						check_task_switch(BT_TASK_NAME);
					#endif
					}
					bt_work_flag = TRUE;
				}
				else
				{
					check_task_switch(task_name);
					bt_work_flag = FALSE;
				}
			}
			break;

		case MSG_POWER:
		case MSG_SYS_POWER:
			puts("main--->MSG_POWER\n");
	    #if defined(_DISABLE_TALKING_KEY_)
	    	if(bt_call_state())break;
	    #endif
		
		#if defined(_SUPPORT_PASSWORD_DISPLAY_)
			if(global.err_flag)break;
		#endif

        #if defined(_ENABLE_AMP_LIGHT_)
        	global.amp_power = TRUE;
        #endif

		#if defined(_XM003_SPECIAL_MUTE_)||defined(_YH1701_SPECIAL_MUTE_)||defined(_XM007_SPECIAL_MUTE_)
			if(global.aud_mute)
			{
				global.aud_mute = !global.aud_mute;
			}
		#endif

	    #if defined(_ENABLE_ACC_MODE_)
	    	if(curr_task->name == music_usb_task_info.name)
	    	{
	    		setup.playusb_flg = TRUE;
	    	}
	    	if(curr_task->name == music_card_task_info.name)
	    	{
	    		setup.playsd_flg = TRUE;
	    	}
	    #endif

		#if defined(_DISABLE_POWER_OFF_MUTE_)
			if(global.sys_power)
			{
				global.power_mute = TRUE;
			}
		#endif
		
		
	    #if defined(_YH1701_SPECIAL_IR_)
	    	if(get_msg_type(msg[1]) == MSG_TYPE_IR)
	    	{
	    		setup.ir_power = TRUE;
	    		setup.ir_power_time = 22;
	    		ew_puts("MSG_POWER1111\n");
	    	}
	    #endif

		#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
			if(setup.first_power_flag)
			{
				power_msg = TRUE;
				os_taskq_post_msg("IdleTask", 1, DISP_EVENT_PASSWORD);
			}
			else
		#endif
			{
    			if(bt_status && compare_task_name(BT_TASK_NAME))
    			{
    			#ifndef	TD_ADD_ALEN//POWER键不要求有挂机作用
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
    			#endif
    			#if defined(_XDXF_SPECIAL_ANSWER_REJECT_)
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
    			#endif
                #if defined(_HMF_SPECIAL_ANSWER_)
                    if(get_msg_type(msg[1]) == MSG_TYPE_IR)
        			{
        				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
        			}
                #endif
                #if defined(_FAS_SPECIAL_REJECT_)||defined(_LK019_POWER_FUNTION)
                    if(get_msg_type(msg[1]) == MSG_TYPE_KEY)
        			{
        				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
        			}
                #endif
    			}
    			else
    			{
    				if(compare_task_name(IDLE_TASK_NAME))
    				{
    				#ifdef SUPPORT_CALL_NAMES
    					if(play_song_flag==TRUE)
    					{
    						break;
    					}
    					else
    				#endif
    					if(global.sys_power)
    					{
    				    #if defined(_ENABLE_TEA685x_INIT_)||defined(_ENABLE_TDA770x_INIT_)
    				    #if defined(_ENABLE_TEA685x_POWER_)
    				    	global.init = FALSE;
    				    #else
    				    	global.init = TRUE;
    				    #endif
    					#endif
    					#if defined(_CHANGE_MODE_1_)||defined(_FY_CHANGE_POWER_KEY_)
    						if(get_msg_mode(msg[1]) != MSG_MODE_DOWN_DELEY)break;
    					#endif
    					#if defined(_EVB_POWER_MUTE_)
    						AUDIO_MUTE_ON();
    				    #if defined(_ENABLE_RCA_MUTE_)
    				    	RCA_MUTE_ON();
    				    #endif
    						OSTimeDly(20);
    					#elif defined(_SPECIAL_POWER_MUTE_)
    						AUDIO_MUTE_ON();
    				    #if defined(_ENABLE_RCA_MUTE_)
    				    	RCA_MUTE_ON();
    				    #endif
    						OSTimeDly(50);
    					#elif defined(_ENABLE_POWER_OFF_MUTE_)
        					AUDIO_MUTE_ON();
    				    #if defined(_ENABLE_RCA_MUTE_)
    				    	RCA_MUTE_ON();
    				    #endif
        					OSTimeDly(50);
    					#endif
    					#if defined(_PS_CHANGE_BT_CALL_DISP_)
    					    power_msg = FALSE;
    					#endif
							puts("main--->MSG_POWER222\n");
    						os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_ENTER);
    					}
    					else
    					{
    						power_msg = TRUE;
							puts("main--->MSG_POWER333\n");
                    		os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_EXIT);
    					}
    				}
    				else
    				{
    				#ifdef SUPPORT_CALL_NAMES
    					if(powerup_flag==TRUE)
    				#endif
                        {
    					#if defined(_CHANGE_MODE_1_)||defined(_FY_CHANGE_POWER_KEY_)
    						if(get_msg_mode(msg[1]) != MSG_MODE_DOWN_DELEY)break;
    					#endif
                        #if defined(_EVB_POWER_MUTE_)
					    #if !defined(_ENABLE_POWER_BEEP_)
    						AUDIO_MUTE_ON();
					    #endif
    				    #if defined(_ENABLE_RCA_MUTE_)
    				        RCA_MUTE_ON();
    				    #endif
						#if !defined(_ENABLE_POWER_BEEP_)
    						OSTimeDly(20);
						#endif
    					#elif defined(_SPECIAL_POWER_MUTE_)
    						AUDIO_MUTE_ON();
    				    #if defined(_ENABLE_RCA_MUTE_)
    				    	RCA_MUTE_ON();
    				    #endif
    						OSTimeDly(50);
    					#elif defined(_ENABLE_POWER_OFF_MUTE_)
    						AUDIO_MUTE_ON();
    				    #if defined(_ENABLE_RCA_MUTE_)
    				    	RCA_MUTE_ON();
    				    #endif
    						OSTimeDly(50);
    					#endif
    				    #if defined(_ENABLE_TEA685x_INIT_)||defined(_ENABLE_TDA770x_INIT_)
    					#if defined(_ENABLE_TEA685x_POWER_)
    						global.init = FALSE;
    					#else
    				    	global.init = TRUE;
    				    #endif
    					#endif
    					#if defined(_FY7000_A2DP_POWER_)
    						AMP_CONTROL_OFF();
        					OSTimeDly(50);
    					#endif
							puts("main--->MSG_POWER444\n");
    						setup.app_name = task_name = curr_task->name;
    	                    task_switch(IDLE_TASK_NAME,0,SWITCH_SPEC_TASK);
    						os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_ENTER);
    					}
    				}
    			}
			}
			break;

		case MSG_LOW_POWER:
            main_puts("main--->MSG_LOW_POWER\n");
            break;

        case MSG_CHANGE_WORKMODE:
         	main_puts("main--->MSG_CHANGE_WORKMODE\n");
			if(global.aud_mute)
			{
				disp_send_msg(DISP_EVENT_MUTE);
				main_puts("main--->DISP_EVENT_MUTE111111111111111111111111\n");
			}
			printf("global.charge_flag:%d\n",global.charge_flag);
		#if defined(USB_SD_SWITCH_TOGETHER)
			if(dev_get_online_status(sd1))
			{
				task_switch(MUSIC_CARD_NAME, 0, SWITCH_SPEC_TASK);
			}
			else if(dev_get_online_status(usb))
			{
				task_switch(MUSIC_USB_NAME, 0, SWITCH_SPEC_TASK);
			}
			else
			{
				task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
			}
		#else
		#if defined(_ENABLE_USB_MODE_)
		#if defined(_EVB_USB_CHANGE_MODE_)
    		if(global.charge_flag)
    		{
				main_puts("main--->MSG_CHANGE_WORKMODE111\n");
    			task_switch(0, 0, SWITCH_NEXT_TASK);
    			global.charge_flag = FALSE;
    		}
    		else
    		{
				main_puts("main--->MSG_CHANGE_WORKMODE222\n");
           #if defined(_SD_NO_FILE_DISPLAY_ERR_)
   				if(global.music_error)
   				{
   					ui_show_setting(LANG_ID_MUSIC_ERR, 0);
   					ui_disp.timer = 1000;
   					ui_show_normal(LANG_ID_MUSIC_ERR);
   					OSTimeDly(200);
   					global.music_error = FALSE;
   				}
           #endif
    			task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
    			global.charge_flag = FALSE;
    		}
		#elif defined(_SUPPORT_DISC_)
			main_puts("main--->MSG_CHANGE_WORKMODE333\n");
			task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
		#else
	    	if(global.charge_flag)
	    	{
			#if defined(_DISABLE_RADIO_)||defined(_POWER_ON_AUX_)
			#if defined(_POWER_ON_BT_)
		        task_switch(BT_TASK_NAME, 0, SWITCH_SPEC_TASK);
		    #else
				task_switch(LINEIN_TASK_NAME, 0, SWITCH_SPEC_TASK);
			#endif
			#else
				task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
			#endif
				global.charge_flag = FALSE;
			}
			else
			{
                task_switch(0, 0, SWITCH_NEXT_TASK);
			}
		#endif
		#else
	    #if defined(_DISABLE_RADIO_)||defined(_POWER_ON_AUX_)
		#if defined(_POWER_ON_BT_)
		    task_switch(BT_TASK_NAME, 0, SWITCH_SPEC_TASK);
		#else
	    	task_switch(LINEIN_TASK_NAME, 0, SWITCH_SPEC_TASK);
		#endif
	    #else
	    	task_switch(FM_TASK_NAME, 0, SWITCH_SPEC_TASK);
	    #endif
		#endif
		#endif
            break;

		case MSG_NEXT_WORKMODE:
		case MSG_SYS_MODE:
			main_puts("main--->MSG_SYS_MODE\n");
	    #if defined(_ENABLE_TEA685x_INIT_)||defined(_ENABLE_TDA770x_INIT_)
	    	global.init = TRUE;
	    #endif

		#if defined(_SUPPORT_PASSWORD_DISPLAY_)
			if(setup.password_flag)break;
		#endif

		#if defined(_ZY32_MODE_FUNTION_)
			if(!idle_usb_state())
			{
				break;
			}
		#endif
		
		#if defined(_AUX_MODE_BT_)
			if(compare_task_name("LineinTask"))
			{
				DSP_RCA_MUTE_ON();
				global.mode_rca = TRUE;
				//global.mode_rca_time = 0;
			}
		#endif
		
		#if defined(_ENABLE_RCA_MUTE_)//修改转模式RCA有杂音
			set_sub_mute();
		#endif

		#if defined(_DISABLE_SUB_OFF_)&&0
    		global.sub_flag = TRUE;
    		global.sub_time = 0;
		#endif
		
			if(bt_status && compare_task_name(BT_TASK_NAME))
			{
				main_puts("main--->MSG_SYS_MODE111\n");
			#if defined(MODE_REJECT_FUNCTION)
				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
			#elif defined(_MODE_KEY_REJECT_FUNCTION)
			    if(get_msg_type(msg[1]) == MSG_TYPE_KEY)
			    {
			      os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
			    }
			#elif defined(_LK019_MODE_FUNCTION)
    			if(get_msg_type(msg[1]) == MSG_TYPE_KEY)
    			{
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
    			}
    			else if(get_msg_type(msg[1]) == MSG_TYPE_IR)
    			{
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
    			}
			#elif defined(KEY_MODE_ANSWER_FUNCTION)
			    if(get_msg_type(msg[1]) == MSG_TYPE_KEY)
			    {
			      os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
			    }
			#elif defined(SPECIAL_MODE_ANSWER_FUNCTION)
			    if(get_msg_type(msg[1]) == MSG_TYPE_IR)
			    {
			      os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
			    }	
			#elif defined(_KW6295_KEY_MODE_BT_FUNCTION)
			    if(get_msg_type(msg[1]) == MSG_TYPE_IR)
    			{
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
    			}
    			else if(get_msg_type(msg[1]) == MSG_TYPE_KEY)
    			{
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
    			}
			#elif defined(_GX9006_MODE_BT_FUNCTION)
			    if(get_msg_type(msg[1]) == MSG_TYPE_IR)
    			{
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
    			}
    			else if(get_msg_type(msg[1]) == MSG_TYPE_KEY)
    			{
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
    			}
			#else
			#if !defined(DISABLE_MODE_FUNTION)
				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
			#endif
			#endif
			}
			else
			{
			#ifdef SUPPORT_CALL_NAMES
				if(powerup_flag==FALSE)
					break;//语音没有播报完成 不能转模式
			#endif
			
		    #if defined(_FY700B_ICON_CLOSE_)
		    	if(!global.power_close)break;
		    #endif

			#if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
					sys_mute_ctrl(ENABLE, 1000);
					global.aud_mute = !global.aud_mute;
					func_mute_control();
		    	}
			#endif
			
		    #if defined(_YH1701_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			
			#if defined(_XM007_SPECIAL_MUTE_)
    			if(global.aud_mute)
    			{
					sys_mute_ctrl(ENABLE, 1000);
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
			#endif

            #if defined(_ENABLE_MODE_DEL_)
            	if(!strcmp(curr_task->name, "BtUserTask"))
            	{
            		if(bt_dial_flag())
            		{
            			os_taskq_post_msg("BtUserTask", 1, MSG_BT_DEL);
            			break;
            		}
            	}
				else
			#elif defined(_ENABLE_MODE_WITH_ZERO_)
				if(!strcmp(curr_task->name, "BtUserTask"))
				{
					if(get_msg_type(msg[1]) == MSG_TYPE_IR)
					{
						if(bt_dial_flag())
						{
							os_taskq_post_msg("BtUserTask", 1, MSG_BT_0);
							break;
						}
					}
				}
				else
            #endif
            	{
				#ifdef	TD_ADD_ALEN
				#if !defined(_TL_SPCIAL_MODE_SWITCH_)
				#if !defined(_DISABLE_SYSTEM_MODE_)
				#if !defined(_SUPPORT_AMP_CONTROL_)
				#if !defined(_XM003_SPECIAL_MUTE_)
				#if !defined(_ENABLE_MODE_FUNTION_)
				#if !defined(_AUX_MODE_MUTE_)
				#if !defined(_DISABLE_MODE_MUTE_)
					AUDIO_MUTE_ON();
			    #if defined(_ENABLE_RCA_MUTE_)
			    	RCA_MUTE_ON();
			    #endif
				#endif
				#endif
				#endif
				#endif
				#endif
				#endif
				#endif
				#endif
				}

			#if defined(_DISABLE_SYSTEM_MODE_)
				if(!global.rds_ta)
			#endif
				{
				#if defined(FM_INIT_OK_FLAG)
					if(global.fm_init_ok_flag)
					{
						break;
					}
				#endif

			    #if defined(_AUDIO_DRIVER_AP3768_)//开机LoGo显示完成才能转模式
			    	if(global.power_mode_flag)
			    	{
			    		break;
			    	}
			    #endif

				#if defined(_ENABLE_MODE_PLAY_)
					if(global.ndf2_flg)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_0);
						break;
					}
				#endif

				#if defined(_SPECIAL_RDS_DELAY_)
					OSTimeDly(20);
				#endif

			    #if defined(DISABLE_TURN_BT_MODE)
			    	if(idle_bt_state())
			    	{
			    		task_switch(0,0,SWITCH_NEXT_TASK);
			    	}
			    	else
			    	{
			    	#if defined(_TL_SPCIAL_MODE_SWITCH_)
			    		if(compare_task_name("FMRadioTask"))
			    		{
			    			if(idle_usb_state())
			    			{
			    				task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
			    			}
			    			else if(idle_card_state())
			    			{
			    				task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
			    			}
						#if defined(_XM003_MODE_DISPLAY_)
							else
							{
								ui_show_setting(LANG_ID_NO_SRC, 0);
								ui_disp.timer = 1000;
							}
						#endif
			    		}
			    		else if(compare_task_name("MusicTaskUsb"))
			    		{
			    			if(idle_card_state())
			    			{
			    				task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
			    			}
			    			else
			    			{
			    				task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
			    			}
			    		}
			    		else if(compare_task_name("MusicTaskCard"))
			    		{
			    			task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
						}
					#elif defined(_DISABLE_RADIO_)
					#if defined(_POWER_ON_BT_)
					    if(compare_task_name("MusicTaskUsb"))
    					{
    						if(idle_card_state())
    						{
    							task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
    						}
    						else
    						{
    							task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
    						}
    					}
    					else if(compare_task_name("MusicTaskCard"))
    					{
    						task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
    					}
						else if(compare_task_name("LineinTask"))
			    		{
			    			task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
			    		}
    					else if(compare_task_name("BtUserTask"))
    					{
			    			if(idle_usb_state())
			    			{
			    				task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
			    			}
			    			else if(idle_card_state())
			    			{
			    				task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
			    			}
    					}
		            #else
    					if(compare_task_name("MusicTaskUsb"))
    					{
    						if(idle_card_state())
    						{
    							task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
    						}
    						else
    						{
    							task_switch("LineinTask",0,SWITCH_SPEC_TASK);
    						}
    					}
    					else if(compare_task_name("MusicTaskCard"))
    					{
    						task_switch("LineinTask",0,SWITCH_SPEC_TASK);
    					}
    					else if(compare_task_name("LineinTask"))
    					{
			    			if(idle_usb_state())
			    			{
			    				task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
			    			}
			    			else if(idle_card_state())
			    			{
			    				task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
			    			}
    					}
			    		else if(compare_task_name("BtUserTask"))
			    		{
			    			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
			    		}
					#endif	
					#elif defined(_DISABLE_AUX_)
    					if(compare_task_name("FMRadioTask"))
						{
							ew_puts("MSG_SYS_MODE111\n");
							if(idle_usb_state())
							{
								ew_puts("MSG_SYS_MODE222\n");
								task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
							}
							else if(idle_card_state())
							{
								ew_puts("MSG_SYS_MODE333\n");
								task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
							}
						}
						else if(compare_task_name("MusicTaskUsb"))
						{
							if(idle_card_state())
							{
								task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
							}
							else
							{
								task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
							}
						}
						else if(compare_task_name("MusicTaskCard"))
						{
							task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
						}
    					else if(compare_task_name("BtUserTask"))
						{
							task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
						}
					#else
			    		if(compare_task_name("FMRadioTask"))
			    		{
			    			if(idle_usb_state())
			    			{
			    				task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
			    			}
			    			else if(idle_card_state())
			    			{
			    				task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
			    			}
			    			else
			    			{
			    				task_switch("LineinTask",0,SWITCH_SPEC_TASK);
			    			}
			    		}
			    		else if(compare_task_name("MusicTaskUsb"))
			    		{
			    			if(idle_card_state())
			    			{
			    				task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
			    			}
			    			else
			    			{
			    				task_switch("LineinTask",0,SWITCH_SPEC_TASK);
			    			}
			    		}
			    		else if(compare_task_name("MusicTaskCard"))
			    		{
			    			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
			    		}
			    		else if(compare_task_name("LineinTask"))
			    		{
			    			task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
			    		}
			    		else if(compare_task_name("BtUserTask"))
			    		{
			    			task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
			    		}
					#endif
			    	}
				#elif defined(_ENABLE_IPHONE_CHARGE_)
				    #if defined(_SPECIAL_POWER_MUTE_)
						AUDIO_MUTE_ON();
				    #if defined(_ENABLE_RCA_MUTE_)
				    	RCA_MUTE_ON();
				    #endif
						OSTimeDly(50);
				    #endif
					if(global.charge_flg)
					{
						task_switch(0,0,SWITCH_NEXT_TASK);
						if(!strcmp(curr_task->name,"IdleTask"))
						{
							os_taskq_post_msg("IdleTask", 1, MSG_IDLE_ENTER);
						}
					}
					else
					{
						if(compare_task_name("FMRadioTask"))
						{
							if(idle_card_state())
							{
								task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
							}
							else
							{
								task_switch("LineinTask",0,SWITCH_SPEC_TASK);
							}
						}
						else if(compare_task_name("MusicTaskCard"))
						{
							task_switch("LineinTask",0,SWITCH_SPEC_TASK);
						}
					#if defined(_DISABLE_BT_MODE_)
    					else if(compare_task_name("LineinTask"))
    					{
    						task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
    					}
					#else
						else if(compare_task_name("LineinTask"))
						{
							task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
						}
						else if(compare_task_name("BtUserTask"))
						{
							task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
						}
					#endif
					}
				#elif defined(_ENABLE_MODE_FUNTION_)
    				if(compare_task_name("FMRadioTask"))
    				{
    					if(idle_usb_state())
    					{
    						task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
    					}
    					else if(idle_card_state())
    					{
    						task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
    					}
    				}
    				else if(compare_task_name("MusicTaskUsb"))
    				{
    					if(idle_card_state())
    					{
    						task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
    					}
						else
						{
							task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
						}
    				}
    				else if(compare_task_name("MusicTaskCard"))
    				{
    					task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
    				}
				#elif defined(_DISABLE_REVERSE_MODE_)
				    if(compare_task_name("MusicTaskUsb"))
					{
						if(idle_card_state())
						{
							task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
						}
						else
						{
							task_switch("LineinTask",0,SWITCH_SPEC_TASK);
						}
					}
					else if(compare_task_name("MusicTaskCard"))
					{
						task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
					}
					else if(compare_task_name("LineinTask"))
					{
		    			if(idle_usb_state())
		    			{
		    				task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
		    			}
		    			else if(idle_card_state())
		    			{
		    				task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
		    			}
						else
						{
                             task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
						}
					}
		    		else if(compare_task_name("BtUserTask"))
		    		{
		    			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
		    		}
		    	#else
    		    #if defined(_EVB_POWER_MUTE_)
    		    	AUDIO_MUTE_ON();
			    #if defined(_ENABLE_RCA_MUTE_)
			    	RCA_MUTE_ON();
			    #endif
    		    	OSTimeDly(20);
    			#elif defined(_SPECIAL_POWER_MUTE_)
    				AUDIO_MUTE_ON();
			    #if defined(_ENABLE_RCA_MUTE_)
			    	RCA_MUTE_ON();
			    #endif
    				OSTimeDly(50);
    		    #endif
    		    	task_switch(0,0,SWITCH_NEXT_TASK);//切换模式
    		    #endif
				if(compare_task_name(IDLE_TASK_NAME))
				{
					os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_ENTER);
				}
			}
			}
			break;

	#if defined(DISABLE_MODE_REJECT_FUNCTION_)||defined(_SL6812_SPECIAL_MODE_)
		case MSG_SYS_MODE1:
		if(bt_status && compare_task_name(BT_TASK_NAME))
		{
		}
		else
		{
		#ifdef SUPPORT_CALL_NAMES
		if(powerup_flag==FALSE)
	    break;//语音没有播报完成 不能转模式
		#endif
    		{
			#ifdef	TD_ADD_ALEN
			#if !defined(_TL_SPCIAL_MODE_SWITCH_)
			#if !defined(_DISABLE_SYSTEM_MODE_)
			#if !defined(_SUPPORT_AMP_CONTROL_)
			#if !defined(_XM003_SPECIAL_MUTE_)
				AUDIO_MUTE_ON();
			#endif
			#endif
			#endif
			#endif
			#endif
    		}
			#if defined(_EVB_POWER_MUTE_)
		      AUDIO_MUTE_ON();
			  OSTimeDly(20);
			#elif defined(_SPECIAL_POWER_MUTE_)
			  AUDIO_MUTE_ON();
			  OSTimeDly(50);
			#endif
			
		#if defined(_SL6812_SPECIAL_MODE_)
		#if defined(_SL6812A_MODE_FUNTION_)
    		if(idle_bt_state())
    		{
				if((compare_task_name("LineinTask"))||(compare_task_name("FMRadioTask")))
				{
					if(idle_usb_state())
					{
						task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
					}
					else if(idle_card_state())
					{
						task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
					}
					else
					{
						 task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
					}
				}
				else if(compare_task_name("BtUserTask"))
				{
					if(AUX_DET_CHECK())
					{
						task_switch("LineinTask",0,SWITCH_SPEC_TASK);
					}
					else if(idle_usb_state())
					{
						task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
					}
					else if(idle_card_state())
					{
						task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
					}
				}
				else if(compare_task_name("MusicTaskUsb"))
				{
					if(idle_card_state())
					{
						task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
					}
					else
					{
						task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
					}
				}
				else if(compare_task_name("MusicTaskCard"))
				{
					task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
				}
    		}
    		else
    		{
				if(compare_task_name("MusicTaskUsb"))
				{
					if(idle_card_state())
					{
						task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
					}
					else if(AUX_DET_CHECK())
					{
						task_switch("LineinTask",0,SWITCH_SPEC_TASK);
					}
				}
				else if(compare_task_name("MusicTaskCard"))
				{
					if(AUX_DET_CHECK())
					{
						task_switch("LineinTask",0,SWITCH_SPEC_TASK);
					}
					else if(idle_usb_state())
					{
						task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
					}
				}
				else if(compare_task_name("LineinTask"))
				{
					if(idle_usb_state())
					{
						task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
					}
					else if(idle_card_state())
					{
						task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
					}
				}
				else if(compare_task_name("FMRadioTask"))
				{
					if(idle_usb_state())
					{
						task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
					}
					else if(idle_card_state())
					{
						task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
					}
					else if(AUX_DET_CHECK())
					{
						task_switch("LineinTask",0,SWITCH_SPEC_TASK);
					}
					else
					{
						ui_show_setting(LANG_ID_NO_MEDIA, 0);
						ui_disp.timer = 1000;
					}
				}
			}
		#else
			if(compare_task_name("LineinTask"))
			{
				if(idle_usb_state())
				{
					task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
				}
				else if(idle_card_state())
				{
					task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
				}
				else
				{
					 task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
				}
			}
			else
			{
				task_switch(0,0,SWITCH_NEXT_TASK);//切换模式
			}
		#endif
		#else
			task_switch(0,0,SWITCH_NEXT_TASK);//切换模式
		#endif
			
			if(compare_task_name(IDLE_TASK_NAME))
			{
			   os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_ENTER);
			}
		}
	    break;
	#endif
	
	#if defined(_XZC1668_MODE_FUNTION_)
		case MSG_SYS_MODE2:
			puts("MSG_SYS_MODE2\n");
			if((compare_task_name("FMRadioTask"))&&(global.fm_am_status))//在FM模式
			{
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_AM_BAND);
			}
			else if((compare_task_name("FMRadioTask"))&&(!global.fm_am_status))//在AM模式
			{
				if(idle_usb_state())
				{
					task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
				}
				else
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_FM_BAND);
				}
			}
			else if(compare_task_name("MusicTaskUsb"))
			{
				task_switch("FMRadioTask", 0,SWITCH_SPEC_TASK);
				OSTimeDly(1);
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_FM_BAND);
			}
			break;
	#endif

	#if defined(ENABLE_BT_MODE_)
		case MSG_SYS_BT:
	    	if(idle_bt_state())
	    	{
    		    if(!compare_task_name("BtUserTask"))
    		    {
					task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
    		    }
	    	}
			break;
	#endif
	
    #if defined(BAND_ACTION_ANSWER_REJECT)
    	case MSG_BAND_ANSWER:
    		ew_puts("MSG_BAND_ANSWER\n");
    		if(compare_task_name("BtUserTask"))
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				if(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
    				{
    					os_taskq_post_msg("BtUserTask", 1, MSG_BT_ANSWER_CALL);
    				}
    				else if((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
    				{
    					os_taskq_post_msg("BtUserTask", 1, MSG_BT_ANSWER_CALL);
    				}
    				else if((get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)||(get_bt_newest_status() == BT_STATUS_PHONE_OUT))
    				{
    					os_taskq_post_msg("BtUserTask", 1, MSG_BT_CALL_REJECT);
    				}
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				if((get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)||(get_bt_newest_status() == BT_STATUS_PHONE_OUT))
    				{
    					os_taskq_post_msg("BtUserTask", 1, MSG_BT_CALL_REJECT);
    				}
    				else if(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
    				{
    					os_taskq_post_msg("BtUserTask", 1, MSG_BT_CALL_REJECT);
    				}
    				else if((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
    				{
    					os_taskq_post_msg("BtUserTask", 1, MSG_BT_CALL_REJECT);
    				}
    			}
    		}
    		else if(compare_task_name("FMRadioTask"))
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_BAND);
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				disp_send_msg(DISP_EVENT_BT_NAME);
    			}
    		}
    	break;
    #endif

    #if defined(_ENABLE_DISP_BT_NAME_)
    	case MSG_SYS_BT_NAME:
    		ew_puts("MSG_SYS_BT_NAME\n");
    		if(compare_task_name("LineinTask"))
    		{
    			disp_send_msg(DISP_EVENT_BT_NAME);
    		}
    		break;
    #endif

	#if defined(_ADJUST_A_B_SPK_)
		case MSG_ADJ_A_SPK:
			disp_send_msg(DISP_EVENT_ADJ_A_SPK);
			break;
		case MSG_ADJ_B_SPK:
			disp_send_msg(DISP_EVENT_ADJ_B_SPK);
			break;
	#endif

	#if defined(_MR889_SPECAIL_KEY_)
		case MSG_ADJ_CONTROL_SPK:
			if(global.sys_power)
			{
				disp_send_msg(DISP_EVENT_ADJ_CONTROL_SPK);
			}
			break;
	#endif

	#if defined(_SPECIAL_TL2000_SET_)
		case MSG_VOL_A_SUB:
			disp_send_msg(DISP_EVENT_A_SUB_VOLUME);
			break;
		case MSG_VOL_A_ADD:
			disp_send_msg(DISP_EVENT_A_ADD_VOLUME);
			break;
		case MSG_VOL_B_SUB:
			disp_send_msg(DISP_EVENT_B_SUB_VOLUME);
			break;
		case MSG_VOL_B_ADD:
			disp_send_msg(DISP_EVENT_B_ADD_VOLUME);
			break;
	#endif
	
	#if defined(_JL8258_KEY_FUNTION_)
    	case MSG_SYS_FM_TIME:
			main_puts("main--->MSG_SYS_FM_TIME\n");
			if(msg_realtime_check())
			{
				if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_hour_check()))
				{
					msg_realtime_select();
				}
				else if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_min_check()))
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if((ui_disp.now_level == DISP_LEVEL_SETTING)
					 &&(ui_disp.func_sel == FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
					{
						if(compare_task_name("FMRadioTask"))
						{
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M5);
						}
						else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
						{
							os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_SUB_10);
						}
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
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
			}
			break;
	#endif

    #if defined(_ENABLE_BAND_TIME_)
		case MSG_SYS_BAND_CLOCK:
			ew_puts("MSG_SYS_BAND_CLOCK\n");
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if((ui_disp.now_level == DISP_LEVEL_SETTING)
				 &&(ui_disp.func_sel == FUNC_SEL_TIME))
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
				else
				{
					if(compare_task_name("FMRadioTask"))
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_BAND);
					}
					else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_ID3);
					}
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
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
			break;
    #endif

	#if defined(_SUPPORT_EXT_MEMORY_)
    	case MSG_CLEAR_MEMORY:
    		ew_puts("MSG_CLEAR_MEMORY\n");
    		if(compare_task_name("LineinTask"))
    		{
			#if defined(YH1701_SOFT_RESET)
				if(setup.mode_start == 0)
			#endif
				{
    				ext_memory_clear();
    				OSTimeDly(20);
    				JL_POWER->CON |= BIT(4);
				}
    		}
		#if !defined(YH1701_SOFT_RESET)
    		else if(compare_task_name("FMRadioTask"))
    		{
    			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_AUTO_SAVE);
    		}
		#endif
    		break;
	#endif
	
	#if defined(_ENABLE_PASSWORD_INPUT_)
		case MSG_SYS_PASSWORD:
			main_puts("main--->MSG_SYS_PASSWORD\n");
			//setup.password = TRUE;
			//disp_send_msg(DISP_EVENT_PASSWORD);
			break;
		case MSG_SYS_PASSWORD1:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if(setup.password)
				{
					disp_send_msg(DISP_EVENT_PASSWORD1);
				}
				else
				{
					if(compare_task_name("FMRadioTask"))
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M1);
					}
					else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PP);
					}
					else if(compare_task_name("BtUserTask"))
					{
						os_taskq_post_msg("BtUserTask", 1, MSG_BT_PP);
					}
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
				if(compare_task_name("FMRadioTask"))
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_S1);
				}
			}
			break;
		case MSG_SYS_PASSWORD2:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if(setup.password)
				{
					disp_send_msg(DISP_EVENT_PASSWORD2);
				}
				else
				{
					if(compare_task_name("FMRadioTask"))
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M2);
					}
					else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_INT);
					}
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
				if(compare_task_name("FMRadioTask"))
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_S2);
				}
			}
			break;
		case MSG_SYS_PASSWORD3:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if(setup.password)
				{
					disp_send_msg(DISP_EVENT_PASSWORD3);
				}
				else
				{
					if(compare_task_name("FMRadioTask"))
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M3);
					}
					else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_RPT);
					}
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
				if(compare_task_name("FMRadioTask"))
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_S3);
				}
			}
			break;
		case MSG_SYS_PASSWORD4:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if(setup.password)
				{
					disp_send_msg(DISP_EVENT_PASSWORD4);
				}
				else
				{
					if(compare_task_name("FMRadioTask"))
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M4);
					}
					else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_RDM);
					}
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
				if(compare_task_name("FMRadioTask"))
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_S4);
				}
			}
			break;
		case MSG_SYS_ENTER:
			if(setup.password)
			{
				disp_send_msg(DISP_EVENT_ENTER);
			}
			else
			{
				disp_send_msg(DISP_EVENT_SEL);
			}
			break;
	#endif

		case MSG_PRE_WORKMOD:
			main_puts("main--->MSG_PRE_WORKMOD\n");
			if(bt_status && compare_task_name(BT_TASK_NAME))
			{
				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
			}
			else
			{
				task_switch(0,0,SWITCH_PRE_TASK);
				if(compare_task_name(IDLE_TASK_NAME))
				{
					os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_ENTER);
				}
			}
			break;

		case MSG_LAST_WORKMOD:
			main_puts("main--->MSG_LAST_WORKMOD\n");
			bt_work_flag = FALSE;
			Interrupt =FALSE;
	    #if defined(_SUPPORT_AMP_CONTROL_)//非蓝牙模式挂断电话有杂音
	    	global.number_flag = TRUE;
	    #endif
			if(bt_status && compare_task_name(BT_TASK_NAME))
			{
				main_puts("main--->MSG_LAST_WORKMOD1111\n");
				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
			}
			else
			{
				main_puts("main--->MSG_LAST_WORKMOD2222\n");
			#if defined(_ENABLE_BT_POWER_)
				if((setup.bt_talk)&&(setup.acc_off))
				{
					main_puts("main--->MSG_LAST_WORKMOD333\n");
					task_switch(setup.mode_name,0,SWITCH_SPEC_TASK);
					setup.acc_off = FALSE;
					setup.bt_talk = FALSE;
				}
				else
				{
					main_puts("main--->MSG_LAST_WORKMOD444\n");
					if(setup.bt_talk)
					{
						main_puts("main--->MSG_LAST_WORKMOD555\n");
						setup.bt_talk = FALSE;
						task_switch(0,0,SWITCH_LAST_TASK);
						if(compare_task_name(IDLE_TASK_NAME))
						{
							main_puts("main--->MSG_LAST_WORKMOD666\n");
							os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_ENTER);
						}
					}
				}
			#else
    			task_switch(0,0,SWITCH_LAST_TASK);
    			if(compare_task_name(IDLE_TASK_NAME))
    			{
    				os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_ENTER);
    			}
			#endif
			}
			break;
			
	    #if defined(_ENABLE_MODE_CHANNEL_)
	    	case MSG_SYS_MODE3:
	    		set_mode_control();
	    		break;
	    #endif

		case MSG_VOL_UP:
		case MSG_VOL_ADD:
			main_puts("main--->MSG_VOL_ADD\n");
			
	    #if defined(_ENABLE_TEL_MUTE_DETECT_)
	    	if(global.tel_mute_flag)
	    	{
	    		break;
	    	}
	    #endif

        #if defined(_XM003_SPECIAL_MUTE_)
        	if(global.aud_mute)
        	{
        		global.aud_mute = !global.aud_mute;
        		func_mute_control();
        	}
        #endif

	    #if defined(_ENABLE_BEEP_)
	    	if(setup.aud_beep)
	    	{
	        #if !defined(_VOL_BEEP_CANCEL)
	        	set_beep_pwm();
	        #endif
	    	}
	    #endif

		#if defined(_SUPPORT_LCM_1618_DRIVE)
		#if defined(DAB_MODLE)
		#if defined(dab_select_station)
            if((dab_select_flag==1) &&( radio_total!=0))
            {
				RADIO_CURRENT_STATION++;
				if(RADIO_CURRENT_STATION>(radio_total-1))
				{
					RADIO_CURRENT_STATION=0;
				}
				#if defined(DAB_CURRENT_STATION_MEMORY)
					setup.DAB_CURRENT_STATION=RADIO_CURRENT_STATION;
				#endif
				dab_select_flag=1;
				DAB_DISP_TEPY=2;//显示节目信息
				RADIO_AUTO_PLAY_TIME=15;//
				DAB_DISP_DLS_SIZE=0;//清除dls缓存信息长度
				DAB_DISP_PLAYPAUSE=0;//清除播放标志
				break;
			}
		#endif
		#endif
		#endif

			if(global.aud_mute)
			{
				disp_send_msg(DISP_EVENT_MUTE);
			}
			else
			{
		    #if defined(_DISABLE_VOL_SEEK_)
		    	if(global.seek_up_flag)break;
		    #endif
			
			#if defined(_SUPPORT_LCM_1618_DRIVE)
				if(vol_set_disp_det(1));
				else
			#endif
			#if defined(_ENABLE_MENU_SET_)
				disp_send_msg(DISP_EVENT_ADD_VOL);
			#elif defined(_DISABLE_SEL_VOLUME_)
				if(global.sel_flag)
				{
					disp_send_msg(DISP_EVENT_ADD);
				}
				else
				{
					disp_send_msg(DISP_EVENT_ADD_VOL);
				}
			#else
				disp_send_msg(DISP_EVENT_ADD);
			#endif
			}
			user_send_cmd_prepare(USER_CTRL_TEST_KEY,1,&test_box_vol_up);//test_box test
		#if BT_TWS
			if(is_check_stereo_slave())
			{
				os_taskq_post("btmsg", 1, msg[0]);
			}
			else
			{
				user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_SEND_VOL,0,NULL);
				os_taskq_post("btmsg", 2,MSG_VOL_STEREO,dac_ctl.sys_vol_l );
			}
		#else
			user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_SEND_VOL,0,NULL);
		#endif
            break;


        case MSG_VOL_DOWN:
		case MSG_VOL_SUB:
			main_puts("main--->MSG_VOL_SUB\n");
			
	    #if defined(_ENABLE_TEL_MUTE_DETECT_)
	    	if(global.tel_mute_flag)
	    	{
	    		break;
	    	}
	    #endif

        #if defined(_XM003_SPECIAL_MUTE_)
        	if(global.aud_mute)
        	{
        		global.aud_mute = !global.aud_mute;
        		func_mute_control();
        	}
        #endif

	    #if defined(_ENABLE_BEEP_)
	    	if(setup.aud_beep)
	    	{
	        #if !defined(_VOL_BEEP_CANCEL)
	        	set_beep_pwm();
	        #endif
	        }
	    #endif

		#if defined(_SUPPORT_LCM_1618_DRIVE)
		#if defined(DAB_MODLE)
		#if defined(dab_select_station)
            if((dab_select_flag==1) &&( radio_total!=0))
            {
				RADIO_CURRENT_STATION--;
				if(RADIO_CURRENT_STATION<0)
				{
					RADIO_CURRENT_STATION=(radio_total-1);
				}

			#if defined(DAB_CURRENT_STATION_MEMORY)
				setup.DAB_CURRENT_STATION=RADIO_CURRENT_STATION;
			#endif
				DAB_DISP_TEPY=2;//显示节目信息

				RADIO_AUTO_PLAY_TIME=15;// 15后电台自动播放
				dab_select_flag=1;
				DAB_DISP_DLS_SIZE=0;//清除dls缓存信息长度
				DAB_DISP_PLAYPAUSE=0;//清除播放标志
				break;
			}
		#endif
		#endif
		#endif

			if(global.aud_mute)
			{
				disp_send_msg(DISP_EVENT_MUTE);
			}
			else
			{
		    #if defined(_DISABLE_VOL_SEEK_)
		    	if(global.seek_up_flag)break;
		    #endif
			
			#if defined(_SUPPORT_LCM_1618_DRIVE)
				if(vol_set_disp_det(0));

				else
			#endif
		    #if defined(_ENABLE_MENU_SET_)
		    	disp_send_msg(DISP_EVENT_SUB_VOL);
			#elif defined(_DISABLE_SEL_VOLUME_)
				if(global.sel_flag)
				{
					disp_send_msg(DISP_EVENT_SUB);
				}
				else
				{
					disp_send_msg(DISP_EVENT_SUB_VOL);
				}
		    #else
		    	disp_send_msg(DISP_EVENT_SUB);
		    #endif
			}
            user_send_cmd_prepare(USER_CTRL_TEST_KEY,1,&test_box_vol_down);//test_box test
		#if BT_TWS
			if(is_check_stereo_slave())
			{
				os_taskq_post("btmsg", 1, msg[0]);
			}
			else
			{
				user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_SEND_VOL,0,NULL);
				os_taskq_post("btmsg", 2,MSG_VOL_STEREO,dac_ctl.sys_vol_l);
			}
		#else
			user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_SEND_VOL,0,NULL);
		#endif
            break;

    #if BT_TWS
    	case MSG_VOL_STEREO:
    		dac_ctl.sys_vol_l = msg[1] ;
    		printf("V = %d\n",dac_ctl.sys_vol_l);
    		if(dac_ctl.sys_vol_l >= MAX_SYS_VOL_L)
    			dac_ctl.sys_vol_l = MAX_SYS_VOL_L ;
    	#if KEY_TONE_EN
    		if(dac_ctl.sys_vol_l == MAX_SYS_VOL_L)
    			send_key_voice(500);
    	#endif
    		printf("V+ = %d\n",dac_ctl.sys_vol_l);
    		dac_ctl.sys_vol_r = dac_ctl.sys_vol_l;
    		if(is_dac_mute()) {
    			dac_mute(0,1);
    		}
    		set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
    	#if (SYS_DEFAULT_VOL==0)
    		vm_cache_write(VM_SYS_VOL,&dac_ctl.sys_vol_l,2);
    	#endif
    		break;
    #endif

        case MSG_MUTE:
		case MSG_SYS_MUTE:
			main_puts("main--->MSG_SYS_MUTE\n");
			
	    #if defined(_XM007_SPECIAL_MUTE_)
	    	global.xm_mute_flag = TRUE;
	    #endif

		#if defined(MUTE_KEY_ACTION_REJECT)
    		if(bt_status && compare_task_name(BT_TASK_NAME))
    		{
    			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
    		}
    		else
        #elif defined(_MUTE_KEY_ACTION_ANSWER_)
			if(bt_status && compare_task_name(BT_TASK_NAME))
    		{
    			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
    		}
    		else
		#elif defined(MUTE_IR_ACTION_REJECT)
			if(bt_status && compare_task_name(BT_TASK_NAME))
    		{
    			if(get_msg_type(msg[1]) == MSG_TYPE_IR)
    			{
    				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
    			}
    		}
    		else
		#endif
			{
				disp_send_msg(DISP_EVENT_MUTE);
			}
            break;

		case MSG_AUD_EQ:
			main_puts("main--->MSG_SYS_EQ\n");
		#if defined(_LOUD_EQ_ANSWER_REJECT)
			if(bt_status && compare_task_name(BT_TASK_NAME))
			{
			   if(get_msg_type(msg[1]) == MSG_TYPE_IR)
			   {
				  os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
			   }
			}
			else
		#endif

			disp_send_msg(DISP_EVENT_EQ);
            break;

		case MSG_AUD_LOUD:
			main_puts("main--->MSG_SYS_LOUD\n");
		#if defined(_LOUD_EQ_ANSWER_REJECT)
			if(bt_status && compare_task_name(BT_TASK_NAME))
    		{
    		   if(get_msg_type(msg[1]) == MSG_TYPE_IR)
    		   {
				  os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
    		   }
    		}
    	    else
		#endif
			disp_send_msg(DISP_EVENT_LOUD);
            break;

		case MSG_USER_SEL:
			main_puts("main--->MSG_SYS_SEL\n");
		#if defined (sel_use_be_call_reject)
			if(bt_status && compare_task_name(BT_TASK_NAME))
			{
				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
			}
			else
		#elif defined(_ENABLE_SEL_WITH_WELL_)
			if(!strcmp(curr_task->name, "BtUserTask"))
			{
				if(get_msg_type(msg[1]) == MSG_TYPE_IR)
				{
					if(bt_dial_flag())
					{
						os_taskq_post_msg("BtUserTask", 1, MSG_BT_WELL);
						break;
					}
				}
			}
		#endif
		#if defined(_DISABLE_TALKING_SEL_)
			if(!bt_call_state())
		#endif
			{
		    #if defined(_SUPPORT_PASSWORD_DISPLAY_)
		    	if(compare_task_name("IdleTask"))
		    	{
		    		if(setup.password_flag)
		    		{
						if((password_number1 != 10)&&(password_number2 != 10)&&(password_number3 != 10)&&(password_number4 != 10))
						{
							disp_send_msg(DISP_EVENT_PASSWORD_ENTER);
						}
		    		}
		    	}
		    	else
		    	{
		    		disp_send_msg(DISP_EVENT_SEL);
		    	}
		    #else
		    	disp_send_msg(DISP_EVENT_SEL);
		    #endif
			}
			break;
			
	#if defined(SUPPORT_ALARM_FUNC_)
		case MSG_ALARM_MODE:
			main_puts("main--->MSG_ALARM_MODE\n");
			if(setup.alarm)
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if((ui_disp.now_level == DISP_LEVEL_SETTING)
					 &&(ui_disp.func_sel == FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK1);
					}
					else
					{
						os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_MODE);
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					if((ui_disp.now_level != DISP_LEVEL_SETTING)
					 ||(ui_disp.func_sel != FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK1);
					}
					else
					{
						disp_send_msg(DISP_EVENT_CLOCK_ADJ1);
					}
				}
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_MODE);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_ALARM);
				}
			}
			break;
	#endif

		case MSG_USER_FUNC://长按SEL键
			main_puts("main--->MSG_SYS_FUNC\n");
	    #if defined(_DISABLE_TALKING_SEL_)
	    	if(!bt_call_state())
	    #endif
	    	{
				disp_send_msg(DISP_EVENT_FUNC);
			}
			break;
		
    #if defined(_ENABLE_SET_FUNTION_)
		case MSG_USER_SEL1:
			main_puts("main--->MSG_SYS_SEL\n");
    	#if defined (sel_use_be_call_reject)
			if(bt_status && compare_task_name(BT_TASK_NAME))
			{
				os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
			}
			else
    	#endif
			{
				disp_send_msg(DISP_EVENT_SEL1);
			}
			break;
			
		case MSG_USER_FUNC1://长按SEL1键
			main_puts("main--->MSG_SYS_FUNC1\n");
			disp_send_msg(DISP_EVENT_FUNC1);
			break;
    #endif

	#if defined(_XM007_CLK_SET_)
		case MSG_CLOCK_SET3:
			ew_puts("MSG_CLOCK_SET3\n");
		
		#if defined(_XIMA_SPECIAL_SEEK_)
			if(global.radio_seek)break;
		#endif
		
		#if defined(_DISABLE_TALKING_KEY_)
			if(!bt_call_state())
		#endif
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if(msg_realtime_check())
					{
						msg_realtime_select();
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
							if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
							{
								os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_8);
							}
							else if(compare_task_name("BtUserTask"))
							{
								if(bt_dial_flag())
								{
									os_taskq_post_msg("BtUserTask", 1, MSG_BT_8);
								}
							}
						}
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
				#if defined(_XM007_MENU_SET_)
					ui_disp.func_type = FUNC_TYPE_SEL;
				#endif
					if(msg_realtime_check())
					{
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
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
				}
			}
			break;
	#endif
	
		case MSG_CLOCK_DISP:
			main_puts("main--->MSG_CLOCK_DISP\n");
		#if defined(_SUPPORT_LCM_1618_DRIVE)
		#if defined(DAB_MODLE)
		#if defined(dab_select_station)
			if(msg_realtime_check())
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					msg_realtime_select();
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			}
			else if((ui_disp.now_level == DISP_LEVEL_SETTING)
				 &&(ui_disp.func_sel == FUNC_SEL_TIME))
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			}
			else if(compare_task_name(DAB_TASK_NAME))
			{
				if((DAB_DISP_TEPY!=1)&&(DAB_DISP_TEPY!=14)&&( radio_total!=0))//搜台和下载信息时不能选台
				{
					if(dab_select_flag==0)
		            {
						dab_select_flag=1;
						dab_select_TIME=30;//
						DAB_DISP_TEPY=2;
					}
					else
					{
						dab_select_flag=0;
					}
				}
			}
			else
			{
				disp_send_msg(DISP_EVENT_CLOCK);
				dab_select_flag=0;
			}
			break;
		#endif
		#endif
		#endif
			{
			#if defined(TIME_KEY_ACTION_REJECT)
        		if(bt_status && compare_task_name(BT_TASK_NAME))
        		{
        			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
        		}
            	else
			#endif
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
			}
			break;

		case MSG_CLOCK_ADJ:
			main_puts("main--->MSG_CLOCK_ADJ\n");
			disp_send_msg(DISP_EVENT_CLOCK_ADJ);
			break;

		case MSG_CLOCK_MUTE:
			main_puts("main--->MSG_CLOCK_MUTE\n");
        #if defined(MUTE_TIME_ACTION_REJECT)
    		if(bt_status && compare_task_name(BT_TASK_NAME))
    		{
    			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
    		}
        	else
        #endif
        	{
			#if defined(_FY5517_TIME_SET_)
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
				#if defined(_FY5517_TIME_DISPLAY_)
				printf("global.time_set = %d\n",global.time_set);
    				if(global.time_set)
    				{
    					disp_send_msg(DISP_EVENT_CLOCK);
						main_puts("main--->DISP_EVENT_CLOCK11111\n");
    				}
    				else
    				{
    					disp_send_msg(DISP_EVENT_MUTE);
    				}
				#else
					if((ui_disp.now_level == 0xFF)
					&&(ui_disp.func_sel == FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
					{
						disp_send_msg(DISP_EVENT_MUTE);
					}
				#endif
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			#elif defined(_GX_SPECIAL_MUTE_CLK_)
			    if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				disp_send_param(DISP_EVENT_CLOCK_MUTE, 0);
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				disp_send_param(DISP_EVENT_CLOCK_MUTE, 1);
    			}
			#else
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				disp_send_param(DISP_EVENT_CLOCK_MUTE, 0);
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				disp_send_param(DISP_EVENT_CLOCK_MUTE, 1);
    			}
			#endif
        	}
			break;

		case MSG_SYS_MODE_CLOCK:
			main_puts("main--->MSG_SYS_MODE_CLOCK\n");
		#if defined(_SPECIAL_MODE_TIME_)
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if((ui_disp.now_level == 0xFF)&&(ui_disp.func_sel == FUNC_SEL_TIME))
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
				else
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_MODE);
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
			}
		#else
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if((ui_disp.now_level == DISP_LEVEL_SETTING)
				 &&(ui_disp.func_sel == FUNC_SEL_TIME))
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
				else
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_MODE);
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
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
		#endif
			break;
		
    #if defined(_SPECIAL_MODE_FUNTION_)
    	case MSG_SYS_AUX_OPTICAL:
    		ew_puts("MSG_SYS_AUX_OPTICAL\n");
    		if((compare_task_name("LineinTask"))&&(setup.mode_start == 0))//AUX
    		{
    			setup.mode_start = 1;
    			audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);
    			audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_OPTICAL_ON, TRUE);
    
    			ui_show_setting(LANG_ID_OPTICAL, 0);
    			ui_show_normal(LANG_ID_OPTICAL);
    		}
    		else if((compare_task_name("LineinTask"))&&(setup.mode_start == 1))//OPTICAL
    		{
    			setup.mode_start = 0;
    			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
    			audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_ON, TRUE);
    
    			ui_show_setting(LANG_ID_AUX_IN, 0);
    			ui_show_normal(LANG_ID_AUX_IN);
    		}
    		break;
    
    	case MSG_SYS_TV_DVD:
    		ew_puts("MSG_SYS_TV_DVD\n");
    		if((compare_task_name("LineinTask"))&&(setup.mode_start == 2))//TV
    		{
    			setup.mode_start = 3;
    			audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DVD_ON, TRUE);
    
    			ui_show_setting(LANG_ID_DVD, 0);
    			ui_show_normal(LANG_ID_DVD);
    		}
    		else if((compare_task_name("LineinTask"))&&(setup.mode_start == 3))//DVD
    		{
    			setup.mode_start = 2;
    			audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_TV_ON, TRUE);
    
    
    			ui_show_setting(LANG_ID_TV, 0);
    			ui_show_normal(LANG_ID_TV);
    		}
    		break;
    #endif
		
    #if defined(_SPECIAL_MODE_FUNTION_)
    	case MSG_SYS_RADIO:
    		ew_puts("MSG_SYS_RADIO\n");
    		SystemRadioMode(get_msg_mode(msg[1]));
    		break;
    	case MSG_SYS_MUSIC:
    		ew_puts("MSG_SYS_MUSIC");
    		SystemMusicMode(get_msg_mode(msg[1]));
    		break;
    #endif

	#if defined(_KEY_SWITCH_TO_RADIO_)
		case MSG_SYS_RADIO:
			ew_puts("MSG_SYS_RADIO1\n");
			if(compare_task_name("FMRadioTask"))
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_BAND);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_AUTO_SAVE);
				}
			}
			else
			{
		    #if defined(_DISABLE_TALKING_KEY_)
		    	if(!bt_call_state())
		    #endif
		    	{
			    #if defined(_XM003_SPECIAL_MUTE_)
			    	if(global.aud_mute)
			    	{
			    		global.aud_mute = !global.aud_mute;
			    		func_mute_control();
			    	}
			    #endif
					task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
				}
			}
			break;
	#endif

    #if defined(_SH_SWITCH_TO_RADIO_)
    	case MSG_SYS_FM:
    		ew_puts("MSG_SYS_RADIO1\n");
			if(compare_task_name("FMRadioTask"))
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_BAND);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_AUTO_SAVE);
				}
			}
			else
			{
			#if defined(_DISABLE_TALKING_KEY_)
		    	if(!bt_call_state())
		    #endif
		    	{
					task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
				}
			}
			break;
    #endif
	
    #if defined(_KEY_SWITCH_TO_BT_)
		case MSG_SYS_BT:
			ew_puts("MSG_SYS_BT\n");
			if(compare_task_name("BtUserTask"))
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					os_taskq_post_msg("BtUserTask", 1, MSG_SYS_BT);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					os_taskq_post_msg("BtUserTask", 1, MSG_SYS_BT);
				}
			}
			else
			{
					task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
			}
			break;
    #endif
	
	#if defined(ENABLE_SPECIAL_BT_)
	    case MSG_SYS_BT_MODE:
			ew_puts("MSG_SYS_BT\n");
			if(compare_task_name("BtUserTask"))
			{
				task_switch(setup.last_name,0,SWITCH_SPEC_TASK);
			}
			else
			{
			    setup.last_name = curr_task->name;
			    task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
			}
			break;
	#endif
	
    #if defined(_ENABLE_VOLUME_CONTROL_)
    	case MSG_SYS_ZONE1:
    		ew_puts("MSG_SYS_ZONE1\n");
    		if(setup.zone1_start)
    		{
    			SYS_ZONE1_ON();
    			setup.zone1_start = FALSE;
    		}
    		else
    		{
    			SYS_ZONE1_OFF();
    			setup.zone1_start = TRUE;
    		}
    		break;
    	case MSG_SYS_ZONE2:
    		ew_puts("MSG_SYS_ZONE2\n");
    		if(setup.zone2_start)
    		{
    			SYS_ZONE2_ON();
    			setup.zone2_start = FALSE;
    		}
    		else
    		{
    			SYS_ZONE2_OFF();
    			setup.zone2_start = TRUE;
    		}
    		break;
    	case MSG_SYS_ZONE3:
    		ew_puts("MSG_SYS_ZONE3\n");
    		if(setup.zone3_start)
    		{
    			SYS_ZONE3_ON();
    			setup.zone3_start = FALSE;
    		}
    		else
    		{
    			SYS_ZONE3_OFF();
    			setup.zone3_start = TRUE;
    		}
    		break;
    	case MSG_SYS_ZONE4:
    		ew_puts("MSG_SYS_ZONE4\n");
    		if(setup.zone4_start)
    		{
    			SYS_ZONE4_ON();
    			setup.zone4_start = FALSE;
    		}
    		else
    		{
    			SYS_ZONE4_OFF();
    			setup.zone4_start = TRUE;
    		}
    		break;
    #endif

	#if defined(_ENABLE_SWITCH_TO_BT_)
    	case MSG_SYS_BT_CALL:
    		ew_puts("MSG_SYS_BT_CALL\n");
    		if(compare_task_name("BtUserTask"))
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				os_taskq_post_msg("BtUserTask", 1, MSG_BT_ANSWER_CALL);
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				os_taskq_post_msg("BtUserTask", 1, MSG_BT_REDIAL);
    			}
    		}
    		else
    		{
    			task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
    		}
    		break;
	#endif

	#if defined(_TL_KEY_TO_RADIO_)
		case MSG_SYS_RADIO:
			ew_puts("MSG_SYS_RADIO2\n");
			if(compare_task_name("FMRadioTask"))
			{
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_BAND);
			}
			else
			{
				task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
			}
			break;
	#endif

	#if defined(_EP_SWITCH_TO_RADIO_)
		case MSG_SYS_RADIO:
			ew_puts("MSG_SYS_RADIO2\n");
			if(compare_task_name("FMRadioTask"))
			{
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_BAND);
			}
			else
			{
				task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
			}
			break;
	#endif
	
	#if defined(_USB_AND_SD_FUNTION_)
		case MSG_SYS_MODE_SUB_SD:
			ew_puts("MSG_SYS_MODE_SUB_SD\n");
			#if defined(_ZW_SCN_FUNC_)
			if(compare_task_name("FMRadioTask"))
			{
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_SCAN);
			}
			else
			#endif
			{
				if(compare_task_name("MusicTaskUsb"))
				{
					if(idle_card_state())
					{
						task_switch("MusicTaskCard",0,SWITCH_SPEC_TASK);
					}
				}
				else if(compare_task_name("MusicTaskCard"))
				{
					if(idle_usb_state())
					{
						task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
					}
				}
			}
			break;
	#endif
	
    #if defined(_CHANGE_USB_SD_FUNTION_)
		case MSG_SYS_SUB_SD:
			ew_puts("MSG_SYS_MODE_SUB_SD\n");
			if(compare_task_name("FMRadioTask"))
			{
				if(idle_usb_state())
				{
					task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
				}
				else if(idle_card_state())
				{
					task_switch("MusicTaskCard",0,SWITCH_SPEC_TASK);
				}
			}
			else if(compare_task_name("LineinTask"))
			{
                if(idle_usb_state())
				{
					task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
				}
				else if(idle_card_state())
				{
					task_switch("MusicTaskCard",0,SWITCH_SPEC_TASK);
				}
			}
			else if(compare_task_name("BtUserTask"))
			{
                if(idle_usb_state())
				{
					task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
				}
				else if(idle_card_state())
				{
					task_switch("MusicTaskCard",0,SWITCH_SPEC_TASK);
				}
			}
			else if(compare_task_name("MusicTaskUsb"))
			{
				if(idle_card_state())
				{
					task_switch("MusicTaskCard",0,SWITCH_SPEC_TASK);
				}
			}
			else if(compare_task_name("MusicTaskCard"))
			{
				if(idle_usb_state())
				{
					task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
				}
			}
			break;
	#endif

	#if defined(_SH1330_KEY_SET_)
	    case MSG_SPECIAL_MUTE:
			ew_puts("MSG_SPECIAL_MUTE\n");
			if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
			{
				os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PP);
			}
			else
			{
				disp_send_msg(DISP_EVENT_MUTE);
			}
			break;
	#endif
			
	#if defined(_KEY_LIGHT_CONTROL_)
		case MSG_SYS_KEY_LIGHT:
	
    	#if defined(_DISABLE_TALKING_KEY_)
    		if(!bt_call_state())
    	#endif
    		{
    			if(setup.light_flg)
    			{
    				setup.light_flg = FALSE;
    			}
    			else
    			{
    				setup.light_flg = TRUE;
    			}
    		}
			break;
	#endif
	
	#if defined(_ENABLE_SUB_DETECT_)
		case MSG_SYS_SUB:
			if(global.sub_flg)
			{
				global.sub_flg = FALSE;
				SYS_SUB_ON();
			}
			else
			{
				global.sub_flg = TRUE;
				SYS_SUB_OFF();
			}
			disp_send_msg(DISP_EVENT_SYS_SUB);
			break;
	#endif

	#if defined(_SPECIAL_KEY_FUNTION_)
		case MSG_PLAY_TIME:
			ew_puts("main--->MSG_PLAY_TIME\n");
		#if defined(_SUPPORT_POWER_ON_PASSWORD_)
    		if(compare_task_name("IdleTask"))
    		{
    			if(setup.password_flag)
    			{
					disp_send_msg(DISP_EVENT_PASSWORD_NO_1);
				}
    		}
    		else
		#endif
			{
    			if(msg_realtime_check())
    			{
    				if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_hour_check()))
    				{
    					msg_realtime_select();
    				}
    				else if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_min_check()))
    				{
    					disp_send_msg(DISP_EVENT_CLOCK);
    				}
    			}
    			else
    			{
    				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    				{
    					if((ui_disp.now_level == DISP_LEVEL_SETTING)
    					 &&(ui_disp.func_sel == FUNC_SEL_TIME))
    					{
    						disp_send_msg(DISP_EVENT_CLOCK);
    					}
    					else
    					{
    						if(compare_task_name("FMRadioTask"))
    						{
    							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M1);
    						}
    						else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
    						{
    							os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PP);
    						}
    						else if(compare_task_name("BtUserTask"))
    						{
    							os_taskq_post_msg("BtUserTask", 1, MSG_BT_PP);
    						}
    					}
    				}
    				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
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
    			}
			}
			break;
	#endif
    #if defined(DA_SPECIAL_KEY_FUNTION_)
		case MSG_PLAY_TIME:
			ew_puts("main--->MSG_PLAY_TIME\n");
			if(msg_realtime_check())
			{
				if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_hour_check()))
				{
					msg_realtime_select();
				}
				else if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_min_check()))
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if((ui_disp.now_level == DISP_LEVEL_SETTING)
					 &&(ui_disp.func_sel == FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
					{
						if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
						{
							os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PP);
						}
						else if(compare_task_name("BtUserTask"))
						{
							os_taskq_post_msg("BtUserTask", 1, MSG_BT_PP);
						}
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
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
			}
			break;
    #endif
    #if defined(_FYDVD_SPECIAL_KEY_)
    	case MSG_ENTER_CLOCK:
    		ew_puts("main--->MSG_ENTER_CLOCK\n");
    		if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    		{
    			if((ui_disp.now_level == DISP_LEVEL_SETTING)
    			 &&(ui_disp.func_sel == FUNC_SEL_TIME))
    			{
    				disp_send_msg(DISP_EVENT_SEL);
    			}
    			else
    			{
    				 if(compare_task_name(PLAYER_DISC_TASK_NAME))
    				 {
    					 os_taskq_post_msg(PLAYER_DISC_TASK_NAME, 1, MSG_MUSIC_ENTER);
    				 }
    				 else if(compare_task_name(PLAYER_USB_TASK_NAME))
    				 {
    					 os_taskq_post_msg(PLAYER_USB_TASK_NAME, 1, MSG_MUSIC_ENTER);
    				 }
    				 else if(compare_task_name(PLAYER_CARD_TASK_NAME))
    				 {
    					os_taskq_post_msg(PLAYER_CARD_TASK_NAME, 1, MSG_MUSIC_ENTER);
    				 }
    			}
    		}
    		else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
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
    		break;
    #endif
	
	#if defined(_MR9982_SPECIAL_KEY_)||defined(_GDH5001_SPECIAL_IR_)
    	case MSG_PTY_CLOCK:
    		ew_puts("main--->MSG_ENTER_CLOCK\n");
			if(msg_realtime_check())
			{
				if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_hour_check()))
				{
					msg_realtime_select();
				}
				else if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_min_check()))
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if((ui_disp.now_level == DISP_LEVEL_SETTING)
					 &&(ui_disp.func_sel == FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
					{
						if(compare_task_name("FMRadioTask"))
						{
							os_taskq_post_msg("FMRadioTask", 1, MSG_RDS_PTY);
						}
					#if defined(_ADD_REJECT_FUNCTION_)
						else if(bt_status && compare_task_name(BT_TASK_NAME))
                		{
                			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
                		}
					#endif
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
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
			}
    		break;
	#endif
    #if defined(_MR9926_SPECIAL_KEY_)
        case MSG_AF_TA:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
			    if(bt_status && compare_task_name(BT_TASK_NAME))
        		{
        			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_ANSWER_CALL);
        		}
				else
				{
                    os_taskq_post_msg("MainTask", 1, MSG_RDS_TA);
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
			    if(bt_status && compare_task_name(BT_TASK_NAME))
        		{
        			os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_REDIAL);
        		}
				else
				{
                    os_taskq_post_msg("MainTask", 1, MSG_RDS_AF);
				}
			}
			break;
	#endif
	
    #if defined(USE_FY_ADD_3KEY)
    	case MSG_USER_FUNC2:
    		ew_puts("MSG_SYS_FUNC2\n");
    		if(compare_task_name(PLAYER_DISC_TASK_NAME))
    		{
    			os_taskq_post_msg(PLAYER_DISC_TASK_NAME, 1, MSG_MUSIC_GOTO);
    		}
    		else if(compare_task_name(PLAYER_USB_TASK_NAME))
    		{
    			os_taskq_post_msg(PLAYER_USB_TASK_NAME, 1, MSG_MUSIC_GOTO);
    		}
    		else if(compare_task_name(PLAYER_CARD_TASK_NAME))
    		{
    			os_taskq_post_msg(PLAYER_CARD_TASK_NAME, 1, MSG_MUSIC_GOTO);
    		}
    		else
    		{
    			disp_send_msg(DISP_EVENT_FUNC);
    		}
    	break;

		case MSG_CLOCK_AF:
    		ew_puts("MSG_CLOCK_AF\n");
        #if defined(_ENABLE_RADIO_RDS_)
        	if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
        	{
        		disp_send_param(DISP_EVENT_CLOCK_AF, 0);
        	}
        	else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
        	{
        		if(compare_task_name(PLAYER_DISC_TASK_NAME))
        		{
        			os_taskq_post_msg(PLAYER_DISC_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else if(compare_task_name(PLAYER_USB_TASK_NAME))
        		{
        			os_taskq_post_msg(PLAYER_USB_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else if(compare_task_name(PLAYER_CARD_TASK_NAME))
        		{
        			os_taskq_post_msg(PLAYER_CARD_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else if(compare_task_name(MUSIC_USB_NAME))
        		{
        			os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else if(compare_task_name(MUSIC_CARD_NAME))
        		{

        			os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else
        		{
        			disp_send_param(DISP_EVENT_CLOCK_AF, 1);
        		}
        	}
        #else
        	if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
        	{
        		if(compare_task_name(PLAYER_DISC_TASK_NAME))
        		{
        			os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else if(compare_task_name(PLAYER_USB_TASK_NAME))
        		{
        			os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else if(compare_task_name(PLAYER_CARD_TASK_NAME))
        		{
        			os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else if(compare_task_name(MUSIC_USB_NAME))
        		{
        			os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else if(compare_task_name(MUSIC_CARD_NAME))
        		{
        			os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_SUB_10);
        		}
        		else
        		{
        			disp_send_param(DISP_EVENT_CLOCK_AF, 1);
        		}
        	}
        	else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
        	{
        		disp_send_param(DISP_EVENT_CLOCK_AF, 0);
        	}
        #endif
			break;
    #endif

	#if defined(_ENABLE_RECORD_RADIO_)
    	case MSG_RECORD_RADIO:
    		ew_puts("main--->MSG_RECORD_RADIO\n");
    		if(compare_task_name("FMRadioTask"))
    		{
    			if(detect_usb_card_state())//有U盘或者sd卡
    			{
    				if(rec_status == rec_start)
    				{
    					os_taskq_post_msg("FMRadioTask", 1, MSG_REC_STOP);
    				}
    				else if(rec_status == rec_stop)
    				{
    					os_taskq_post_msg("FMRadioTask", 1, MSG_REC_START);
    				}
    			}
    		}
			else if(compare_task_name("BtUserTask"))
			{
				os_taskq_post_msg("BtUserTask", 1, MSG_BT_REDIAL);
			}
    		break;
	#endif
    
    #if defined(_ENABLE_MIC_REC_)
    	case MSG_RECORD_MIC:
    		ew_puts("main--->MSG_RECORD_MIC\n");
    		if((compare_task_name("LineinTask"))&&(MIC_CHECK()))
    		{
    			if(detect_usb_card_state())//有U盘或者sd卡
    			{
    				if(rec_status == rec_start)
    				{
    					os_taskq_post_msg("LineinTask", 1, MSG_REC_STOP);
    				}
    				else if(rec_status == rec_stop)
    				{
    					os_taskq_post_msg("LineinTask", 1, MSG_REC_START);
    				}
    			}
    		}
    		break;
    #endif
	
	#if defined(_ENABLE_RECORD_KEY_)
    	case MSG_RECORD_INT:
			ew_puts("main--->MSG_RECORD_INT\n");
			if(compare_task_name("FMRadioTask"))
			{
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M2);
			}
			else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
			{
				os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_INT);
			}
			else if(compare_task_name("TapeTask"))
			{
				if(detect_usb_card_state())//有U盘或者sd卡
				{
					if(rec_status == rec_start)
					{
						rec_status = rec_stop;
						os_taskq_post_msg("TapeTask", 1, MSG_REC_STOP);
					}
					else if(rec_status == rec_stop)
					{
						rec_status = rec_start;
						os_taskq_post_msg("TapeTask", 1, MSG_REC_START);
					}
				}
			}
			else if(compare_task_name("RECTask"))
			{
				if(detect_usb_card_state())//有U盘或者sd卡
				{
					if(rec_status == rec_start)
					{
						os_taskq_post_msg("RECTask", 1, MSG_REC_STOP);
					}
					else if(rec_status == rec_stop)
					{
						os_taskq_post_msg("RECTask", 1, MSG_REC_START);
					}
				}
			}
    		break;
	#endif

    #if defined(_ZST2020_MENU_SET_)
    	case MSG_MODE_SEL:
    		ew_puts("main--->MSG_MODE_SEL\n");
    		if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    		{
			#if defined(_SUPPORT_POWER_ON_PASSWORD_)
    			if(compare_task_name("IdleTask"))
    			{
					if(setup.password_flag)
					{
						if((password_number1 != 10)&&(password_number2 != 10)&&(password_number3 != 10)&&(password_number4 != 10))
						{
							disp_send_msg(DISP_EVENT_PASSWORD_ENTER);
						}
					}
    			}
				else
			#endif
				{
        			if(ui_disp.func_sel != FUNC_SEL_NULL)
        			{
        				disp_send_msg(DISP_EVENT_SEL);
        			}
        			else
        			{
    					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_MODE);
        			}
				}
    		}
    		else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    		{
    			disp_send_msg(DISP_EVENT_FUNC);
    		}
    		break;
    #endif

	#if defined(_SH1330_MENU_SET_)
    	case MSG_MODE_SEL_CLK:
    		ew_puts("main--->MSG_MODE_SEL\n");
    		if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    		{
    			if(ui_disp.func_sel != FUNC_SEL_NULL)
    			{
    				disp_send_msg(DISP_EVENT_SEL);
    			}
    			else
    			{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_MODE);
    			}
    		}
    		else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    		{
    		    if((ui_disp.now_level == DISP_LEVEL_SETTING)
				 &&(ui_disp.func_sel == FUNC_SEL_TIME))
    		     {
                    if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
        				disp_send_msg(DISP_EVENT_CLOCK);
					}
        			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
        			{
        				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
    				}
    		     }
				 else
				 {
                       disp_send_msg(DISP_EVENT_SEL);
				 }
    		}
    		break;
     #endif
	 
     #if defined(_MR9002_MENU_SET_)
 		 case MSG_SEL_CLK:
 			ew_puts("main--->MSG_SEL_CLK\n");
			if(msg_realtime_check())
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				msg_realtime_select();
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)//确认并退出
    			{
    				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
					ui_disp.func_type = FUNC_TYPE_SEL;
    			}
    		}
 			else if((ui_disp.now_level == DISP_LEVEL_SETTING)
    			&&(ui_disp.func_sel == FUNC_SEL_TIME))
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				disp_send_msg(DISP_EVENT_CLOCK);
					ui_disp.func_type = FUNC_TYPE_SEL;
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
    			}
    		}
			else
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_SIR);
    			}
    		}
 			 break;
     #endif
	 
	 #if defined(_SPECIAL_SEL_TIME_FUNCTION_)
	    case MSG_CLOCK_SEL:
			main_puts("main--->MSG_CLOCK_SEL\n");
			if(msg_realtime_check())
			{
				if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_hour_check()))
    			{
    				msg_realtime_select();
    			}
    			else if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_min_check()))
    			{
    				disp_send_msg(DISP_EVENT_CLOCK);
    			}
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if((ui_disp.now_level == DISP_LEVEL_SETTING)
					 &&(ui_disp.func_sel == FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK_ADJ);
					}
					else
					{
						os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					if((ui_disp.now_level != DISP_LEVEL_SETTING)
					 ||(ui_disp.func_sel != FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK);
					}
				}
			}
			break;
	 #else
		case MSG_CLOCK_SEL:
			main_puts("main--->MSG_CLOCK_SEL\n");
			if(msg_realtime_check())
			{
			#if defined(ENABLE_MIN_HOUR_ONCE)
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if(msg_realtime_select_count>=1)
					{
						msg_realtime_select_count=0;
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
					{
						msg_realtime_select_count++;
						msg_realtime_select();
					}

				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					msg_realtime_select_count=0;
					disp_send_msg(DISP_EVENT_CLOCK);
				}
			#else
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					msg_realtime_select();
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			#endif
			}
			else if((ui_disp.now_level == DISP_LEVEL_SETTING)
				 &&(ui_disp.func_sel == FUNC_SEL_TIME))
			{
			#if defined(_SUPPORT_SEL_SETTING_)
			    if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
				#if defined(ENABLE_MIN_HOUR_ONCE)
					msg_realtime_select_count=0;
				#endif
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			#else
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
				#if defined(ENABLE_MIN_HOUR_ONCE)
					msg_realtime_select_count=0;
				#endif
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			#endif	
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
			}
			break;
	#endif
	
	#if defined(_ENABLE_CLK_TIME_SET)
	     case MSG_CLK_TIME:
			main_puts("main--->MSG_CLK_TIME\n");
			if(msg_realtime_check())
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					msg_realtime_select();
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			}
			else if((ui_disp.now_level == DISP_LEVEL_SETTING)
				 &&(ui_disp.func_sel == FUNC_SEL_TIME))
			{
			    if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					disp_send_msg(DISP_EVENT_CLOCK);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			}
			break;
	#endif
	#if defined(_ENABLE_SEL_MENU_TIME_)
    	case MSG_SEL_TIME:
    		puts("main--->MSG_SEL_TIME\n");
		#if defined(_FY8567_SET_TIME_)
			if(msg_realtime_check())
			{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				msg_realtime_select();
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)//确认并退出
    			{
    				disp_send_msg(DISP_EVENT_CLOCK);
    			}
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
				}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)//确认并退出
    			{
    				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
    			}
			}

		#else
    		if(msg_realtime_check())
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
				#if defined(_FEIYIN_MENU_TIME_)
					ui_disp.timer = 3000;
				#endif
    				msg_realtime_select();
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)//确认并退出
    			{
    				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
					ui_disp.func_type = FUNC_TYPE_SEL;
				#if defined(_FEIYIN_MENU_TIME_)
					ui_disp.timer = 0;
				#endif
    			}
    		}
    		else if((ui_disp.now_level == DISP_LEVEL_SETTING)
    			 &&(ui_disp.func_sel == FUNC_SEL_TIME))
    		{
		    #if defined(_FEIYIN_MENU_TIME_)
		    	ui_disp.timer = 3000;
		    #endif
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
				#if defined(_ENABLE_SEL_FUNTION_)
					if(global.sel_mode)
					{
						global.sel_mode = FALSE;
						os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
					}
					else
				#endif
				#if defined(_FEIYIN_RDS_SET_)
					if(global.sel_rds)
					{
						ui_disp.func_type = FUNC_TYPE_FUNC;
					#if defined(_FEIYIN_MENU_TIME_)
						ui_disp.timer = 0;
					#endif
						global.sel_rds = FALSE;
					}
					else
				#endif
					{
        				disp_send_msg(DISP_EVENT_CLOCK);
    					ui_disp.func_type = FUNC_TYPE_SEL;
					}
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
				#if defined(_ENABLE_SEL_FUNTION_)
    				global.sel_mode = FALSE;
				#endif
    				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
    			}
    		}
    		else
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
				#if defined(_SUPPORT_PASSWORD_DISPLAY_)
        			if(compare_task_name("IdleTask"))
        			{
    					if(setup.password_flag)
    					{
    						if((password_number1 != 10)&&(password_number2 != 10)&&(password_number3 != 10)&&(password_number4 != 10))
    						{
								disp_send_msg(DISP_EVENT_PASSWORD_ENTER);
							}
    					}
        			}
					else
					{
					#if defined(_SPECIAL_MENU_FUNTION_)
						if(!global.set_flag)
						{
							global.set_flag = TRUE;
							os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
						}
						else
						{
							disp_func_reset();
						}
					#else
						os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
					#endif
					}
				#else
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
				#endif
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
				#if defined(_SPECIAL_MENU_FUNTION_)
    				if(!global.set_flag)
    				{
    					global.set_flag = TRUE;
    					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_FUNC);
    				}
    				else
    				{
    					disp_func_reset();
    				}
				#else
    				os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_FUNC);
				#endif
    			}
    		}
		#endif
    		break;
	#endif

	#if defined(_SWL_ENABLE_SEL_TIME_)
    	case MSG_CLOCK_SET2:
    		main_puts("main--->MSG_CLOCK_SET2\n");
    		if(msg_realtime_check())
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				msg_realtime_select();
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				disp_send_msg(DISP_EVENT_CLOCK);
    			}
    		}
    		else
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
    			}
    		}
    		break;
	#endif

	#if defined(_SUPPORT_VR2_)
		case MSG_VOL_ADD2:
		#if defined(_SUPPORT_TWO_PT2313_)
    		if(global.aud_mute)
    		{
    			disp_send_msg(DISP_EVENT_MUTE);
    		}
    		else
    		{
    			disp_send_msg(DISP_EVENT_ADD1);
    		}
		#else
    		if(compare_task_name("FMRadioTask"))
    		{
    			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_STATION);
    		}
		#if !defined(_DISABLE_VR2_FUNTION_)
    		else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
    		{
    			os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_NEXT_FILE);
    		}
    		else if(compare_task_name("BtUserTask"))
    		{
    			os_taskq_post_msg("BtUserTask", 1, MSG_BT_NEXT_FILE);
    		}
		#endif
		#endif
    		break;

    	case MSG_VOL_SUB2:
		#if defined(_SUPPORT_TWO_PT2313_)
    		if(global.aud_mute)
    		{
    			disp_send_msg(DISP_EVENT_MUTE);
    		}
    		else
    		{
    			disp_send_msg(DISP_EVENT_SUB1);
    		}
		#else
    		if(compare_task_name("FMRadioTask"))
    		{
    			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_STATION);
    		}
		#if !defined(_DISABLE_VR2_FUNTION_)
    		else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
    		{
    			os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PREV_FILE);
    		}
    		else if(compare_task_name("BtUserTask"))
    		{
    			os_taskq_post_msg("BtUserTask", 1, MSG_BT_PREV_FILE);
    		}
		#endif
		#endif
    		break;
	#endif

	#if defined(_SUPPORT_VOL_ADD)
		case MSG_VOL_ADD1:
    		if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    		{
        		if(compare_task_name("FMRadioTask"))
        		{
        			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_SEEK);
        		}
        		else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
        		{
        			os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_NEXT_FILE);
        		}
        		else if(compare_task_name("BtUserTask"))
        		{
        			os_taskq_post_msg("BtUserTask", 1, MSG_BT_NEXT_FILE);
        		}
    		}
    		else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
    		{
    			 disp_send_msg(DISP_EVENT_ADD);
    		}
    		break;

    	case MSG_VOL_SUB1:
        	if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    		{
        		if(compare_task_name("FMRadioTask"))
        		{
        			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_SEEK);
        		}
        		else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
        		{
        			os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PREV_FILE);
        		}
        		else if(compare_task_name("BtUserTask"))
        		{
        			os_taskq_post_msg("BtUserTask", 1, MSG_BT_PREV_FILE);
        		}
    		}
    		else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
    		{
    			 disp_send_msg(DISP_EVENT_SUB);
    		}
    		break;
	#endif
	
	#if defined(_ENABLE_AUX_VERSION_)
    	case MSG_SYS_M1:
        	if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
        	{
        		if(compare_task_name("FMRadioTask"))
        		{
        			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M1);
        		}
        		else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
        		{
        			os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PP);
        		}
        		else if(compare_task_name("BtUserTask"))
        		{
        			os_taskq_post_msg("BtUserTask", 1, MSG_BT_PP);
        		}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
        		if(compare_task_name("FMRadioTask"))
        		{
        			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_S1);
        		}
				else if(compare_task_name("LineinTask"))
				{
					disp_send_msg(DISP_EVENT_AUX_VERSION);
				}
			}
    		break;
	#endif

	#if defined(_ENABLE_MODE_VERSION_)
    	case MSG_SYS_VERSION:
        	if(compare_task_name("LineinTask"))
        	{
        		disp_send_msg(DISP_EVENT_AUX_VERSION);
        	}
    		break;
	#endif
	
	#if defined(_WLGK3001_IR_FUNTION_)
		case MSG_SYS_BT_ANSWER:
			if(!compare_task_name("BtUserTask"))
			{
				task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
			}
			else
			{
				if(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
				{
					os_taskq_post_msg("BtUserTask", 1, MSG_BT_ANSWER_CALL);
				}
				else if((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
				{
					os_taskq_post_msg("BtUserTask", 1, MSG_BT_ANSWER_CALL);
				}
				else if((get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)||(get_bt_newest_status() == BT_STATUS_PHONE_OUT))
				{
					os_taskq_post_msg("BtUserTask", 1, MSG_BT_CALL_REJECT);
				}
			}
    		break;
			
		case MSG_SYS_RADIO_BAND:
			if(!compare_task_name("FMRadioTask"))
			{
				task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
			}
			else
			{
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_BAND);
			}
			break;
	#endif
	
	#if defined(_WLGK3001_KEY_FUNTION_)
		case MSG_SYS_ANSWER_MODE:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
        		if(compare_task_name("BtUserTask"))
        		{
        			
					if(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
					{
						os_taskq_post_msg("BtUserTask", 1, MSG_BT_ANSWER_CALL);
					}
					else if((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
					{
						os_taskq_post_msg("BtUserTask", 1, MSG_BT_ANSWER_CALL);
					}
					else
					{
						os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_MODE);
					}
        		}
				else
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_MODE);
				}

			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
			#if defined(_ENABLE_BT_OFF_)
				if(!setup.bt_scan_flag)
				{
					setup.bt_scan_flag = 1;
					setup.bt_first_scan = 0;
				}
				else
				{
					os_taskq_post_msg("btmsg", 1, MSG_BT_CONNECT_CTL);
				}
			#else
				os_taskq_post_msg("btmsg", 1, MSG_BT_CONNECT_CTL);
			#endif
			}
			break;
			
		case MSG_SYS_BAND_BT:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if(compare_task_name("FMRadioTask"))
				{
        			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_BAND);
				}
        		else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
        		{
        			os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_ID3);
        		}
        		else if(compare_task_name("BtUserTask"))
        		{
        			os_taskq_post_msg("BtUserTask", 1, MSG_BT_CALL_REJECT);
        		}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
				os_taskq_post_msg("btmsg", 1, MSG_BT_DISCONNECT);
			}
			break;
	#endif
	
	#if defined(_ZSBY5209_VOLUME_FUNTION_)
		case MSG_VOL_ADD_CH:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if(compare_task_name("FMRadioTask"))
				{
        			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_SLECT_UP);
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_OTHERS)
			{
				os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_VOL_ADD);
			}
			break;
			
		case MSG_VOL_SUB_CH:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				if(compare_task_name("FMRadioTask"))
				{
        			os_taskq_post_msg("FMRadioTask", 1, MSG_FM_SLECT_DOWN);
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_OTHERS)
			{
				os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_VOL_SUB);
			}
			break;
	#endif

	#if defined(_ENABLE_TEMP_PWM_)
		case MSG_SYS_TEMP:
			if(setup.warm_power)
			{
				disp_send_msg(DISP_EVENT_TEMP);
			}
			break;
			
		case MSG_SYS_WARM:
			if(setup.warm_power)
			{
				disp_send_msg(DISP_EVENT_WARM);
			}
			break;
			
    	case MSG_SYS_WARM_POWER:
    		if(setup.warm_power)
    		{
				PWM0_init(0);
				setup.warm_power = 0;
			}
			else
			{
				setup.warm_power = 1;
				PWM0_init(5);
				setup.temp_set = FUNC_SEL_TEMP_05;//第5档
				setup.warm_set = FUNC_SEL_WARM_30;//30分钟
				setup.warm_time = 1859;
			}
    		break;
	#endif

    #if defined(_ENABLE_EQ_TIME_)
    	case MSG_CLOCK_EQ:
    	#if defined(_DISABLE_TALKING_KEY_)
    		if(!bt_call_state())
    	#endif
    		{
    		#if defined(_XM003_SPECIAL_MUTE_)
    			if(global.aud_mute)
    			{
    				if((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)&&(compare_task_name("FMRadioTask")))
    				{
    					ui_disp.timer = 0;
    				}
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
    		#endif
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				if(msg_realtime_check())
    				{
    					msg_realtime_select();
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
        					if(compare_task_name("MusicTaskUsb")||compare_task_name("MusicTaskCard")||compare_task_name("BtUserTask"))
        					{
        						disp_send_msg(DISP_EVENT_EQ);
    						}
    					}
    				}
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				if(msg_realtime_check())
    				{
    					disp_send_msg(DISP_EVENT_CLOCK);
    				}
    				else
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
    			}
    		}
    		break;
    #endif

	#if defined(_XZC1668_RADIO_FUNTION_)
		case MSG_SYS_FM_BAND:
			ew_puts("MSG_SYS_FM_BAND\n");
			if(compare_task_name("MusicTaskUsb"))
			{
				task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
				OSTimeDly(1);
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_FM_BAND);
			}
			else if(compare_task_name("FMRadioTask"))
			{
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_FM_BAND);
			}
    		break;

		case MSG_SYS_AM_BAND:
			ew_puts("MSG_SYS_FM_BAND\n");
			if(compare_task_name("MusicTaskUsb"))
			{
				task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
				OSTimeDly(1);
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_AM_BAND);
			}
			else if(compare_task_name("FMRadioTask"))
			{
				os_taskq_post_msg("FMRadioTask", 1, MSG_FM_AM_BAND);
			}
			break;
	#endif

	#if defined(_ENABLE_MENU_SET_)
    	case MSG_SYS_NEXT:
			ew_puts("MSG_SYS_NEXT\n");
			if((ui_disp.timer > 0)&&(global.menu_flag))
			{
				disp_send_msg(DISP_EVENT_ADD);
			}
			else if((ui_disp.timer == 0)||(ui_disp.timer == -1)
			||((ui_disp.timer > 0)&&(!global.menu_flag)))
			{
				if(compare_task_name("FMRadioTask"))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_SEEK);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
					{
    					if(global.mute_key_flag)
    					{
							global.mute_key_flag = FALSE;
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_SEEK);
    					}
						else
						{
							global.mute_key_flag = FALSE;
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_STEP);
						}
					}
				}
				else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_NEXT_FILE);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FF);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_UP_MORE_THAN)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FFR_DONE);
					}
				}
				else if(compare_task_name("BtUserTask"))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("BtUserTask", 1, MSG_BT_NEXT_FILE);
					}
				}
			}
    		break;
			
		case MSG_SYS_PREV:
			ew_puts("MSG_SYS_PREV\n");
			if((ui_disp.timer > 0)&&(global.menu_flag))
			{
				disp_send_msg(DISP_EVENT_SUB);
			}
			else if((ui_disp.timer == 0)||(ui_disp.timer == -1)
			||((ui_disp.timer > 0)&&(!global.menu_flag)))
			{
				if(compare_task_name("FMRadioTask"))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_SEEK);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
					{
    					if(global.mute_key_flag)
    					{
							global.mute_key_flag = FALSE;
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_SEEK);
    					}
						else
						{
							global.mute_key_flag = FALSE;
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_STEP);
						}
					}
				}
				else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PREV_FILE);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FR);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_UP_MORE_THAN)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FFR_DONE);
					}
				}
				else if(compare_task_name("BtUserTask"))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("BtUserTask", 1, MSG_BT_PREV_FILE);
					}
				}
			}
			break;
	#endif

	#if defined(_ENABLE_NEXT_PRVE_SET_)
    	case MSG_SYS_NEXT1:
			ew_puts("MSG_SYS_NEXT1\n");
			if(msg_realtime_check())
			{
				disp_send_msg(DISP_EVENT_ADD);
			}
			else
			{
				if(compare_task_name("FMRadioTask"))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_SEEK);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
					{
    					if(global.mute_key_flag)
    					{
							global.mute_key_flag = FALSE;
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_SEEK);
    					}
						else
						{
							global.mute_key_flag = FALSE;
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_NEXT_STEP);
						}
					}
				}
				else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_NEXT_FILE);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FF);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_UP_MORE_THAN)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FFR_DONE);
					}
				}
				else if(compare_task_name("BtUserTask"))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("BtUserTask", 1, MSG_BT_NEXT_FILE);
					}
				}
			}
    		break;
			
		case MSG_SYS_PREV1:
			ew_puts("MSG_SYS_PREV1\n");
			if(msg_realtime_check())
			{
				disp_send_msg(DISP_EVENT_SUB);
			}
			else
			{
				if(compare_task_name("FMRadioTask"))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_SEEK);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
					{
    					if(global.mute_key_flag)
    					{
							global.mute_key_flag = FALSE;
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_SEEK);
    					}
						else
						{
							global.mute_key_flag = FALSE;
							os_taskq_post_msg("FMRadioTask", 1, MSG_FM_PREV_STEP);
						}
					}
				}
				else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PREV_FILE);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_DELEY_LOOP)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FR);
					}
					else if(get_msg_mode(msg[1]) == MSG_MODE_UP_MORE_THAN)
					{
						os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FFR_DONE);
					}
				}
				else if(compare_task_name("BtUserTask"))
				{
					if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
					{
						os_taskq_post_msg("BtUserTask", 1, MSG_BT_PREV_FILE);
					}
				}
			}
			break;
	#endif
	
    #if defined(_XM003_SPECIAL_MUTE_)
    	case MSG_USER_M1:
    		if(compare_task_name("BtUserTask"))
    		{
    			if(global.aud_mute)
    			{
    				ui_disp.timer = 0;
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
    		}
    		break;
    	case MSG_USER_M2:
    		if(compare_task_name("BtUserTask"))
    		{
    			if(global.aud_mute)
    			{
    				ui_disp.timer = 0;
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
    		}
    		break;
    	case MSG_USER_M3:
    		if(compare_task_name("BtUserTask"))
    		{
    			if(global.aud_mute)
    			{
    				ui_disp.timer = 0;
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
    		}
    		break;
    	case MSG_USER_M4:
    		if(compare_task_name("BtUserTask"))
    		{
    			if(global.aud_mute)
    			{
    				ui_disp.timer = 0;
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
    		}
    		break;
    	case MSG_USER_M5:
    		if(compare_task_name("BtUserTask"))
    		{
    			if(global.aud_mute)
    			{
    				ui_disp.timer = 0;
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
    		}
    		break;
    	case MSG_USER_M6:
    		if((compare_task_name("BtUserTask"))||(compare_task_name("MusicTaskCard"))||(compare_task_name("MusicTaskUsb")))
    		{
    			if(global.aud_mute)
    			{
    				ui_disp.timer = 0;
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
    		}
    		break;
    #endif

	#if defined(_XM007_BEEP_CONTROL)
		case MSG_USER_M1:
			if(compare_task_name("BtUserTask"))
			{
				set_beep_pwm();
			}
			break;
		case MSG_USER_M2:
			if(compare_task_name("BtUserTask"))
			{
				set_beep_pwm();
			}
			break;
		case MSG_USER_M3:
			if(compare_task_name("BtUserTask"))
			{
				set_beep_pwm();
			}
			else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					set_beep_pwm();
				}
			}
			break;
		case MSG_USER_M4:
			if(compare_task_name("BtUserTask"))
			{
				set_beep_pwm();
			}
			break;
		case MSG_USER_M5:
			if(compare_task_name("BtUserTask"))
			{
				set_beep_pwm();
			}
			break;
		case MSG_USER_M6:
			if(compare_task_name("BtUserTask"))
			{
				set_beep_pwm();
			}
			else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					set_beep_pwm();
				}
			}
			break;
		case MSG_USER_M9:
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
				set_beep_pwm();
			}
			break;
		case MSG_USER_M0:
			set_beep_pwm();
			break;
		case MSG_USER_REJECT:
			set_beep_pwm();
			break;
	#endif

    #if defined(_EVB_DISP_TIME_SET_)
	#if defined(_SUPPORT_TIME_DISPLAY_)
    	case MSG_CLOCK_SET1:
    		ew_puts("MSG_CLOCK_SET1\n");
    		if(msg_realtime_check())
    		{
    			if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(!(msg_realtime_day_check())))
    			{
    				msg_realtime_select();
    			}
    			else if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_day_check()))
    			{
    				disp_send_msg(DISP_EVENT_CLOCK);
    			}
    		}
    		else
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
    				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
    			}
    			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    			{
    				//disp_send_msg(DISP_EVENT_CLOCK_ADJ);
    			}
    		}
    		break;
	#else
    	case MSG_CLOCK_SET1:
    		ew_puts("MSG_CLOCK_SET1\n");
	#if defined(TIME_ACTION_REJECT)
			if(bt_status && compare_task_name(BT_TASK_NAME))
			{
    		    if(get_msg_type(msg[1]) == MSG_TYPE_KEY)
    		    {
    			   os_taskq_post_msg(BT_TASK_NAME, 1, MSG_BT_CALL_REJECT);
    		    }
			}
			else
	#endif
		    {
        		if(msg_realtime_check())
        		{
        			if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_hour_check()))
        			{
        				msg_realtime_select();
        			}
        			else if(((get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))&&(msg_realtime_min_check()))
        			{
        				disp_send_msg(DISP_EVENT_CLOCK);
        			}
        		}
        		else
        		{
        			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
        			{
        				disp_send_msg(DISP_EVENT_CLOCK);
        			}
        			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
        			{
        				disp_send_msg(DISP_EVENT_CLOCK_ADJ);
        			}
        		}
		    }
    		break;
    #endif
	#endif
	
	#if defined(_XM007_MENU_SET_)
		case MSG_USER_MENU:
			ew_puts("MSG_USER_MENU\n");
        #if defined(_XIMA_SPECIAL_SEEK_)
        	if(global.radio_seek)break;
        #endif

		#if defined(_DISABLE_TALKING_KEY_)
			if(!bt_call_state())
		#endif
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if(ui_disp.func_sel != FUNC_SEL_NULL)
					{
						disp_send_msg(DISP_EVENT_SEL);
					}
					else
					{
						if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
						{
							os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_7);
						}
						else if(compare_task_name("BtUserTask"))
						{
							if(bt_dial_flag())
							{
								os_taskq_post_msg("BtUserTask", 1, MSG_BT_7);
							}
						}
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_FUNC);
				}
			}
			break;
	#elif defined(_SPECIAL_MENU_SET_)
    	case MSG_USER_MENU:
    		ew_puts("main--->MSG_USER_MENU\n");
    		if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    		{
    			if(ui_disp.func_sel != FUNC_SEL_NULL)
    			{
    				disp_send_msg(DISP_EVENT_SEL);
    			}
    			else
    			{
    				if(compare_task_name("FMRadioTask"))
    				{
    					os_taskq_post_msg("FMRadioTask", 1, MSG_FM_M4);
    				}
    				else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
    				{
    					os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_RDM);
    				}
    			}
    		}
    		else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    		{
    			disp_send_msg(DISP_EVENT_FUNC);
    		}
    		break;
	#endif

    #if defined(_CONNECT_BT_PHONE_)||defined(_ENABLE_REJECT_CONNECT_)
    	case MSG_SYS_CONNECT:
    		ew_puts("MSG_SYS_CONNECT\n");
    		os_taskq_post_msg("btmsg", 1, MSG_BT_CONNECT_CTL);
    		break;
    #endif
		
    #if defined(_IR_SWITCH_TO_BT_)
    	case MSG_SYS_PHONE:
        	ew_puts("MSG_SYS_PHONE");
		#if defined(POWER_BT_AS_CALL_ANSWER)
			if(bt_status && (!strcmp(curr_task->name, "BtUserTask")))
			{
				os_taskq_post_msg("BtUserTask", 1, MSG_BT_ANSWER_CALL);
			}
			else
		#endif
    		if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard"))||(compare_task_name("LineinTask"))||(compare_task_name("FMRadioTask")))
    		{
    			task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
    		}
        	break;
    #endif

	#if defined(_POWER_SET_COLOR_)||defined(_ENABLE_SET_COLOR_)
		case MSG_SYS_LIGHT:
			ew_puts("MSG_SYS_LIGHT\n");
			disp_send_msg(DISP_EVENT_LIGHT);
			break;
	#endif

    #if defined(_SL6812_TEST_LCD_)
    	case MSG_SYS_LCD:
			ew_puts("MSG_SYS_LCD\n");
    		disp_send_msg(DISP_EVENT_SYS_LCD);
    		break;
    #endif

	#if defined(_SL6812_KEY_FUNTION_)
    	case MSG_SYS_SPM:
    		ew_puts("MSG_SYS_SPM\n");
    		disp_send_msg(DISP_EVENT_SYS_SPM);
    		break;
	#endif
	
    #if defined(_BT_DISCONNECT_DISPLAY_)
		case MSG_SYS_DISPLAY:
			ew_puts("MSG_SYS_DISPLAY\n");
			if((get_bt_connect_status() == BT_STATUS_CONNECTING)
    	    ||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
    	    ||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
    		{
    			if(compare_task_name("BtUserTask"))
    			{
    				os_taskq_post_msg("BtUserTask", 1, MSG_BT_SIR);
    			}
    			else
    			{
    				global.sir_mode = TRUE;
    				global.name = curr_task->name;
    				//ew_printf("global.name:%s\n",global.name);
    				task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
    				OSTimeDly(90);
    				os_taskq_post_msg("BtUserTask", 1, MSG_BT_SIR);
    			}
    		}
			else
			{
				disp_send_msg(DISP_EVENT_SYS_DISP);
			}
			break;
    #endif
	
	#if defined(_ENABLE_PASSWORD_SET_)
    	case MSG_SYS_POWER_OFF:
    		ew_puts("MSG_SYS_POWER_OFF\n");
	    #if defined(_DISABLE_TALKING_KEY_)
	    	if(bt_call_state())break;
	    #endif
    		if(!compare_task_name(IDLE_TASK_NAME))
    		{
				setup.app_name = task_name = curr_task->name;
				task_switch(IDLE_TASK_NAME,0,SWITCH_SPEC_TASK);
				os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_ENTER);
			}
    		break;
			
    	case MSG_SYS_SET_PWD:
    		ew_puts("MSG_SYS_SET_PWD\n");
			os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_POWER_OFF);
			setup.first_power_flag = TRUE;
			setup.pwd_set = TRUE;
    		break;
	#endif

	#if defined(_ENABLE_ID3_SET_)
		case MSG_SYS_ID3:
			ew_puts("MSG_SYS_ID3\n");
			if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
			{
				disp_send_msg(DISP_EVENT_ID3);
			}
			break;
	#endif
	
	#if defined(SUPPORT_LED_ADJUST)
		case MSG_CLOCK_LED:
			ew_puts("MSG_CLOCK_LED\n");
			if(setup.time_disp)
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if((ui_disp.now_level == UI_LEVEL_ALL)
					 &&(ui_disp.func_sel == FUNC_SEL_TIME)
					 &&(global.rtc_time))
					{
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
					{
						disp_send_msg(DISP_EVENT_LED);
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					disp_send_msg(DISP_EVENT_CLOCK_ADJ);
				}
			}
			else
			{
				if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
				{
					if((ui_disp.now_level == DISP_LEVEL_SETTING)
					 &&(ui_disp.func_sel == FUNC_SEL_TIME))
					{
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
					{
						disp_send_msg(DISP_EVENT_LED);
					}
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					ew_puts("MSG_CLOCK_LED111\n");
					if((ui_disp.now_level != DISP_LEVEL_SETTING)
					 ||(ui_disp.func_sel != FUNC_SEL_TIME))
					{
						ew_puts("MSG_CLOCK_LED222\n");
						disp_send_msg(DISP_EVENT_CLOCK);
					}
					else
					{
						ew_puts("MSG_CLOCK_LED333\n");
						disp_send_msg(DISP_EVENT_CLOCK_ADJ);
					}
				}
			}
			break;
	#endif

    #if defined(_SUPPORT_DISC_)
    	case MSG_SYS_EJECT:
			ew_puts("MSG_SYS_EJECT\n");
        #if defined(_SUPPORT_CALL_RECORDS_)
			global.info.call_info = FALSE;
        	global.info.call_roll = FALSE;
			global.info.call_info_exit = TRUE;
			global.info.call_info_flag = TRUE;
        	global.info.roll_time = 0;
        #endif
    		disc_drv_key();
    		break;

	#if defined(_ENABLE_REJECT_)
		case MSG_SYS_EJECT1:
			ew_puts("MSG_SYS_EJECT1\n");
	    #if defined(_SUPPORT_CALL_RECORDS_)
			global.info.call_info = FALSE;
			global.info.call_roll = FALSE;
			global.info.call_info_exit = TRUE;
			global.info.call_info_flag = TRUE;
	    	global.info.roll_time = 0;
	    #endif
			disc_drv_key1();
			break;
	#endif

    	case SYS_EVENT_PLAYER_DISC_IN:
    		ew_puts("main ---> SYS_EVENT_PLAYER_DISC_IN\n");
    		if((!disc_drv_auto(TRUE))
    			&&(!bt_status))
    		{
				ew_puts("main ---> SYS_EVENT_PLAYER_DISC_IN1111\n");
    			switch_activation_task(PLAYER_DISC_TASK_NAME);
    		}
    		disp_send_param(DISP_EVENT_ICON_DISC, ON);
    		break;

    	case SYS_EVENT_PLAYER_USB_IN:
    		ew_puts("main ---> SYS_EVENT_PLAYER_USB_IN\n");
    		if(!bt_status)
    		{
    			ew_puts("main ---> SYS_EVENT_PLAYER_USB_IN111\n");
    			switch_activation_task(PLAYER_USB_TASK_NAME);
    		}
    		disp_send_param(DISP_EVENT_ICON_USB, ON);
    		break;
    	case SYS_EVENT_PLAYER_CARD_IN:
    		ew_puts("main ---> SYS_EVENT_PLAYER_CARD_IN\n");
    		if(!bt_status)
    		{
    			switch_activation_task(PLAYER_CARD_TASK_NAME);
    		}
    		disp_send_param(DISP_EVENT_ICON_SD, ON);
    		break;

    	case SYS_EVENT_PLAYER_DISC_OUT:
    		ew_puts("main ---> SYS_EVENT_PLAYER_DISC_OUT\n");
    		setup.disc_break &= (~_B0_);
    		if(compare_task_name(PLAYER_DISC_TASK_NAME))
    		{
				ew_puts("main ---> SYS_EVENT_PLAYER_DISC_OUT111\n");
    			os_taskq_post("MainTask", 1, MSG_CHANGE_WORKMODE);
    		}
    		disp_send_param(DISP_EVENT_ICON_DISC, OFF);
    		break;

    	case SYS_EVENT_PLAYER_USB_OUT:
    		ew_puts("main ---> SYS_EVENT_PLAYER_USB_OUT\n");
    		setup.disc_break &= (~_B1_);
    		if(compare_task_name(PLAYER_USB_TASK_NAME))
    		{
    			os_taskq_post("MainTask", 1, MSG_CHANGE_WORKMODE);
    		}
    		disp_send_param(DISP_EVENT_ICON_USB, OFF);
    		break;

    	case SYS_EVENT_PLAYER_CARD_OUT:
    		ew_puts("main ---> SYS_EVENT_PLAYER_CARD_OUT\n");
    		setup.disc_break &= (~_B2_);
    		if(compare_task_name(PLAYER_CARD_TASK_NAME))
    		{
    			os_taskq_post("MainTask", 1, MSG_CHANGE_WORKMODE);
    		}
    		disp_send_param(DISP_EVENT_ICON_SD, OFF);
    		break;
    #endif
	
    #if defined(_SUPPORT_IPHONE_SIR_)
      	case MSG_SYS_SIR:
      		ew_puts("MSG_SYS_SIR\n");
    		if((get_bt_connect_status() == BT_STATUS_CONNECTING)
    	    ||(get_bt_connect_status() == BT_STATUS_TAKEING_PHONE)
    	    ||(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC))
    		{
    			if(compare_task_name("BtUserTask"))
    			{
    				os_taskq_post_msg("BtUserTask", 1, MSG_BT_SIR);
    			}
    			else
    			{
    				global.sir_mode = TRUE;
    				global.name = curr_task->name;
    				ew_printf("global.name:%s\n",global.name);
    				task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
    				OSTimeDly(90);
    				os_taskq_post_msg("BtUserTask", 1, MSG_BT_SIR);
    			}
    		}
      		break;
    #endif

    #if defined(_ENABLE_AUX_VER_VIEW_)
    	case MSG_VER_VIEW:
    		ew_puts("MSG_VER_VIEW\n");
    		if(compare_task_name("LineinTask"))
    		{
    			disp_send_msg(DISP_EVENT_VER_VIEW);
    		}
    		break;
    #endif
	
	#if defined(_ENABLE_AUX_RESET_)
	#if defined(_ENABLE_AUX_SPECIAL_RESET_)
	    case MSG_SYS_RESET_VIEW:
    		ew_puts("MSG_SYS_RESET_VIEW\n");
    		if(compare_task_name("LineinTask"))
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
					soft_reset = 1;
					vm_write_api(VM_SOFT_RESET, &soft_reset);
				#if defined(_SUPPORT_EXT_MEMORY_)
					ext_memory_clear();
				#endif
					os_time_dly(20);
					JL_POWER->CON |= BIT(4);
				}
    		}
    		break;
	#else
    	case MSG_SYS_RESET_VIEW:
    		ew_puts("MSG_SYS_RESET_VIEW\n");
    		if(compare_task_name("LineinTask"))
    		{
    			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    			{
					disp_send_msg(DISP_EVENT_VER_VIEW);
				}
				else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
				{
				#if defined(_ENABLE_RESET_DISPLAY_)
    				ui_show_setting(LANG_ID_RESET, 0);
    				ui_disp.timer = 2000;
    				ui_show_normal(LANG_ID_RESET);
					os_time_dly(100);
				#endif
					soft_reset = 1;
					vm_write_api(VM_SOFT_RESET, &soft_reset);
				#if defined(_SUPPORT_EXT_MEMORY_)
					ext_memory_clear();
				#endif
					os_time_dly(20);
					JL_POWER->CON |= BIT(4);
				}
    		}
    		break;
	#endif
    #endif
	
    #if defined(_ENABLE_FAD_FUNTION_)
		case MSG_FAD_SINGLE_F:
			ew_puts("MSG_FAD_SINGLE_F\n");
			disp_send_msg(DISP_EVENT_FAD_SINGLE_F);
			break;
		case MSG_FAD_SINGLE_R:
			ew_puts("MSG_FAD_SINGLE_R\n");
			disp_send_msg(DISP_EVENT_FAD_SINGLE_R);
			break;
		case MSG_FAD_ALL:
			ew_puts("MSG_FAD_ALL\n");
			disp_send_msg(DISP_EVENT_FAD_ALL);
			break;
    #endif
	
	#if defined(_ENABLE_APP_FREQ_)
    	case MSG_RADIO_FREQ:
    		ew_puts("MSG_RADIO_FREQ\n");
			os_taskq_post_msg(FM_TASK_NAME,3,MSG_FM_SELECT_FREQ,msg[1],msg[2]);
    		break;
	#endif
	
	#if defined(_SL6812_POWER_FUNTION_)
    	case MSG_POWER_FUNTION:
    		ew_puts("MSG_POWER_FUNTION\n");
			if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
			{
			#if defined(_SPECIAL_MENU_FUNTION_)
				if(global.set_flag)
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_USER_SEL);
				}
				else
			#endif
				{
    				if((compare_task_name("FMRadioTask"))||(compare_task_name("LineinTask")))
    				{
    					disp_send_msg(DISP_EVENT_MUTE);
    				}
    				else if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
    				{
    					os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PP);
    				}
    				else if(compare_task_name("BtUserTask"))
    				{
    					if((bt_connect_state())&&((get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
    					||(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_COME)
    					||(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO)))
    					{
    						os_taskq_post_msg("BtUserTask", 1, MSG_BT_PP);
    					}
    					else
    					{
    						disp_send_msg(DISP_EVENT_MUTE);
    					}
    				}
				}
			}
			else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
			{
				ew_puts("MSG_POWER_FUNTION11\n");
				os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_POWER);
			}
    		break;
	#endif

	#if defined(_CHANGE_DISP_MODE_)
	    case MSG_SYS_CHANGE1:
		   if(!setup.ad_keyflag)
		   {
               if(((get_msg_mode(msg[1]) == MSG_MODE_DOWN)||(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN))||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))
     		   {
                   setup.ad_keyflag =1;
     		   }
		   }
		   else
		   {
               if(((get_msg_mode(msg[1]) == MSG_MODE_DOWN)||(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN))||(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY))
     		   {
                   setup.ad_keyflag =0;
     		   }
		   }
		   break;
	#endif

	#if defined(_SUPPORT_PLAYER_NAMES_)
	    case MSG_SYS_PLAYER_NAMES:
			ew_puts("main--->MSG_SYS_PLAYER_NAMES\n");
		    if(setup.back_flag)
		    {
                setup.back_flag =0;
				OSTimeDly(13);
				task_switch(setup.back_name,0,SWITCH_SPEC_TASK);
		    }
		    else
		    {
                setup.back_flag =1;
				setup.back_name = curr_task->name;
				task_switch(REVERSE_TASK_NAME,0,SWITCH_SPEC_TASK);
		    }		
		   break;	
    #endif
	
	#if defined(_ENABLE_RADIO_RDS_)	
		case MSG_RDS_AF:
			main_puts("main--->MSG_RDS_AF\n");
			disp_send_msg(DISP_EVENT_AF);
			break;

		case MSG_RDS_TA:
			main_puts("main--->MSG_RDS_TA\n");
			disp_send_msg(DISP_EVENT_TA);
			break;

		case MSG_RDS_EON:
			main_puts("main--->MSG_RDS_EON\n");
			disp_send_msg(DISP_EVENT_EON);
			break;

		case MSG_RDS_REG:
			main_puts("main--->MSG_RDS_REG\n");
			disp_send_msg(DISP_EVENT_REG);
			break;
			
    #if defined(_RDS_KEY_FUNTION_)
    	case MSG_RDS_AF_REG:
    		main_puts("main--->MSG_RDS_AF_REG\n");
    		if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    		{
    			if(global.reg_flag)
    			{
    				if(setup.rds.reg)
    				{
    					setup.rds.reg = FALSE;
    				}
    				else
    				{
    					setup.rds.reg = TRUE;
    				}
    				disp_send_msg(DISP_EVENT_REG);
    			}
    			else
    			{
    				disp_send_msg(DISP_EVENT_AF);
					global.reg_flag = FALSE;
    			}
    		}
    		else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    		{
    			os_taskq_post_msg("MainTask", 1, MSG_RDS_REG);
				global.reg_flag = TRUE;
    		}
    		break;
    		
    	case MSG_RDS_TA_EON:
    		main_puts("main--->MSG_RDS_TA_EON\n");
    		if(get_msg_mode(msg[1]) == MSG_MODE_UP_LESS_THAN)
    		{
    			if(global.eon_flag)
    			{
    				if(setup.rds.eon)
    				{
    					setup.rds.eon = FALSE;
    				}
    				else
    				{
    					setup.rds.eon = TRUE;
    				}
    				disp_send_msg(DISP_EVENT_EON);
    			}
    			else
    			{
    				disp_send_msg(DISP_EVENT_TA);
					global.eon_flag = FALSE;
    			}
    		}
    		else if(get_msg_mode(msg[1]) == MSG_MODE_DOWN_DELEY)
    		{
    			os_taskq_post_msg("MainTask", 1, MSG_RDS_EON);
				global.eon_flag = TRUE;
    		}
    		break;
    #endif
	
	#endif

        case MSG_UPDATA:
			printf("MSG_UPDATA\n\n");
			if(msg[1] == DEV_UPDATA)
			{
		    #if defined(_UPDATA_CLEAN_MEMORY_)
		    	usb_updata = 1;
		    	vm_write_api(VM_USB_UPDATA, &usb_updata);
		    	os_time_dly(5);
		    #endif
		    #if defined(_ENABLE_MEMORY_PASSWORD_)
		    	memset(pwd_set_save, 0x31, sizeof(pwd_set_save));
		    	vm_write_api(VM_PWD_SET, &pwd_set_save);
		    	os_time_dly(10);
		    #endif
				music_enter_updata();
			}
	    #if defined(TD_ADD_UPDATA)
	    	else if(msg[1] == UART_UPDATA)
	    	{
				printf("UART_UPDATA\n\n");
	    		uart_updata_receive_cmd();

	    		os_time_dly(50);//500ms 此时解码那边要切换波特率460800，延时最少要500ms
	    	}
	    #endif
	    #if BLE_APP_UPDATE_SUPPORT_EN 
	    	else if(msg[1] == BLE_UPDATE)
	    	{
	    		while(is_busy_in_update_conn_param())
	    		{
	    			putchar('W');
	    			os_time_dly(1);
	    		}
	    		while(!le_hw_waiting_idle())
	    		{
	    			os_time_dly(1);
	    		}
	    	}
	    #endif
			dev_updata_mode((void *)msg[2],msg[1]);
			break;

		case SYS_EVENT_DEV_BEGIN_MOUNT:
			main_puts("main--->SYS_EVENT_DEV_BEGIN_MOUNT\n");

		#if defined(_YH1701_USB_STANBY_)
			if(!global.sys_power)
			{
				if(detect_usb_state1())
				{
					setup.usb_standby = TRUE;
					os_time_dly(10);
					usb->dev_io->power(DEV_POWER_STANDBY);
				}
			}
		#endif
			
			global.usb_flg = TRUE;

		#if defined(_ENABLE_RCA_MUTE_)
	    	set_sub_mute();
	    #endif

		#if defined(_DISABLE_SYSTEM_MODE_)
			if(!global.rds_ta)
		#endif
			{
				sys_mute_ctrl(ENABLE, 0);
			}

		#ifdef	TD_ADD_IPONE_CHARGE
			if((int)usb == msg[1])
			{
            	usb_insert =TRUE;
			}
		#endif

            break;

		case SYS_EVENT_DEV_MOUNT_ERR:
			main_puts("main--->SYS_EVENT_DEV_MOUNT_ERR\n");
		#ifdef	TD_ADD_IPONE_CHARGE
            if(usb_insert ==TRUE)
            {
				usb_ERR =TRUE;
            }
		#endif
		#if defined(_JTI_CHANGE_MODE_)
    		if(global.sys_power)
    		{
    			if(!setup.acc_flag)
    			{
    				if(!setup.change_power_flag)
    				{
    					sys_mute_ctrl(DISABLE, 0);//如果屏蔽，读SD时，插手机充电，SD卡会无声
    				}
    				else if((setup.change_power_flag)&&(compare_task_name("MusicTaskCard")))
    				{
    					sys_mute_ctrl(DISABLE, 0);
    				}
    				setup.change_power_flag = FALSE;
    			}
    			else
    			{
					setup.acc_flag = FALSE;
    			}
    		}
		#elif defined(_ENABLE_CHARGE_MUTE_)
		    if(global.sys_power)
		    {
		        //printf("setup.accoff_flagaaaaaaaaaaa:%d\r\n",setup.accoff_flag);
                if(!setup.accoff_flag)
                {
                    sys_mute_ctrl(DISABLE, 0);//如果屏蔽，读SD时，插手机充电，SD卡会无声
				}
				else
				{
                    setup.accoff_flag = FALSE;
				}
			}
		#else
    		if(global.sys_power)
    		{
    			sys_mute_ctrl(DISABLE, 0);//如果屏蔽，读SD时，插手机充电，SD卡会无声
    		}
		#endif

			#if defined(_ENABLE_IPHONE_CHARGE_)
				setup.usb_detect1 = TRUE;
			#endif

            if(compare_task_name(MUSIC_TASK_NAME))
            {
				main_puts("main--->SYS_EVENT_DEV_MOUNT_ERR111\n");
                os_taskq_post_event(MAIN_TASK_NAME, 1, MSG_CHANGE_WORKMODE);
            }
			break;

		case SYS_EVENT_SYS_TIMER_DELAY:
			main_puts("main--->SYS_EVENT_SYS_TIMER_DELAY\n");
			break;
	#ifdef	TD_ADD_SPP
		case SYS_EVENT_SPP_ACK:
		#ifdef TD_ADD_NEW_APP
			OSTimeDly(15);
			app_link_ok_reply();
			OSTimeDly(15);
			app_link_ok_reply_init_data();
			OSTimeDly(15);
	    #if defined(_AUDIO_DRIVER_AP3768_)
	    	Spp_Tx_Audio_ble();
	    #else
	    	Spp_Tx_Audio();
	    #endif
	    	OSTimeDly(15);
		#if defined(_AUDIO_DRIVER_AP3768_)
            ap3768_info_to_app();
            OSTimeDly(15);
		#else
    		Spp_Tx_Audio_ble();
    		OSTimeDly(15);
		#endif
			Spp_Tx_WorkMode();
			OSTimeDly(15);
			new_app_send_vol();
			OSTimeDly(15);
		#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
		#if defined(_ENABLE_LIGHT_SHADE_)
			new_app_shade_light();
		#else
			new_app_send_light();
		#endif
			OSTimeDly(15);
		#endif
			if(compare_task_name(FM_TASK_NAME))
			{
				new_app_radio_init();
				OSTimeDly(15);
				new_app_send_bandinfo();
				OSTimeDly(15);
				Spp_Tx_Radio_Info(GET_BAND_TEMP_FREQ);
		    #if defined(_ENABLE_RADIO_RDS_)
				OSTimeDly(10);
		    	Spp_Tx_Radio_Rds(setup.rds.ta,setup.rds.af,setup.pty_type);
		    #endif
			}
		#else
			OSTimeDly(15);
			Spp_Tx_Ack();
            OSTimeDly(15);
            Spp_Tx_WorkMode();
            OSTimeDly(15);
            Spp_Tx_Audio();
			OSTimeDly(15);
            Spp_Tx_WorkMode();
            OSTimeDly(15);
            Spp_Tx_Audio();
		#endif
			break;
	#endif
        case MSG_UART_ERR:
            uart_module_off();
            uart_init(UART_BAUD_RAE);
		#if defined(TD_ADD_W2DAB_UART)
            extern s32 W2dab_uart_init(u32 baud);
			W2dab_uart_init(DAB_UART_BAUD_RAE);
		#elif defined( _SUPPORT_C912DAB_)
            uart_interrupt_receive_init(1200);
		#endif
        	break;
	#ifdef	TD_ADD_USER_UART
		case MSG_SWITCH_BAUD:
            Close_Uart2();
            uart_interrupt_receive_init(19200);
			break;
	#endif
	#ifdef	TD_ADD_NEW_APP
        case SYS_EVENT_CHANGE_MODE:
            if(power_up_flag ==FALSE)
            {
				spec_mode =0;
            }
			if(spec_mode != 0)
			{
				printf("app_change_mode:%d\r\n",spec_mode);
	            if(compare_task_name(IDLE_TASK_NAME))
	            {//非开机状态下 点APPmode键 执行开机任务  要不然会死机
                    power_up_flag =FALSE;//解决待机状态下 一直操作手机APP模式会一直开关机的问题
	                os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_SYS_POWER);
	            }
	            else
	            {
					os_taskq_post_event(MAIN_TASK_NAME, 1,((spec_mode-1)+SYS_MODE_CHANGE_IDLE));
				}
				spec_mode = 0;
			}
		break;
		
	#ifdef	TD_ADD_NEW_APP
        case SYS_EVENT_EQ_RESET:
            printf("eq_reset\r\n");
		#if defined(_AUDIO_DRIVER_AP3768_)
            ap3768_reset();
		#else
			pt2313_reset();
		#endif
        	break;
	#endif
	
		case SYS_VOL_CHANGE:
			new_app_send_vol();
			break;
		case SYS_CLR_LIST:
		#if !defined(_DISABLE_MUSIC_LIST_)
			clr_play_mode_list(msg[1]);
		#endif
			break;
		case SYS_MODE_CHANGE_IDLE:
			printf("app_change_idle\r\n");
			task_switch(IDLE_TASK_NAME,0,SWITCH_SPEC_TASK);
			break;
		case SYS_MODE_CHANGE_RADIO:
			printf("app_change_radio\r\n");
			task_switch("FMRadioTask",0,SWITCH_SPEC_TASK);
			break;
		case SYS_MODE_CHANGE_AUX:
			printf("app_change_aux\r\n");
			task_switch("LineinTask",0,SWITCH_SPEC_TASK);
			break;
		case SYS_MODE_CHANGE_BT:
			printf("app_change_bt\r\n");
			task_switch("BtUserTask",0,SWITCH_SPEC_TASK);
			break;
		case SYS_MODE_CHANGE_USB:
			printf("app_change_usb\r\n");
			if(idle_usb_state())
			{
				task_switch("MusicTaskUsb", 0,SWITCH_SPEC_TASK);
			}
			break;
		case SYS_MODE_CHANGE_SD:
			printf("app_change_sd\r\n");
			if(idle_card_state())
			{
				task_switch("MusicTaskCard", 0,SWITCH_SPEC_TASK);
			}
			break;
	#endif
    #if defined(SRVR_ERROR_TURN_MODE)
    	case SYS_EVENT_SERVO_ERROR_MODE:
    		ew_puts("main --- > SYS_EVENT_SERVO_ERROR_MODE\n");
    		switch_activation_task(FM_TASK_NAME);
    		break;
    #endif
		default:
			break;
        }
	}
}

/*----------------------------------------------------------------------------*/
/**@brief  创建main task
   @param
   @return
   @note
*/
/*----------------------------------------------------------------------------*/
int main_task_init(void)
{
    u32 err;
    main_puts("create main task\n");

	if(!main_sem_flag)
	{
		err = os_sem_create(&main_eng_sem, 0);
		if(err == OS_NO_ERR)
		{
			main_sem_flag = TRUE;
		}
		else
		{
			main_printf("main sem err = %08x \n", err);
		}
	}

    err = os_task_create_ext(TaskMain,
                             (void*)0,
                             TaskMainPrio,
                             10
                             ,MAIN_TASK_NAME,
                             MAIN_TASK_STACK_SIZE
                            );

    if(OS_NO_ERR == err)
    {
        key_msg_register(MAIN_TASK_NAME, KEY_PROJECT_MAX);
    }
    else
    {
        main_printf("main task err = %08x \n", err);
    }
    return 0;
}

