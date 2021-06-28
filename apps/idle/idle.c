#include "sdk_cfg.h"
#include "rtc.h"
#include "idle.h"
#include "key_drv.h"
#include "disp.h"
#include "setup.h"
#include "audio_drv.h"
#include "timer.h"
#include "clock.h"
#include "sys_detect.h"
#include "fm_engine.h"
#include "sleep.h"
#include "dac/dac.h"
#include "cpu/uart_api.h"
#include "bluetooth/avctp_user.h"
#include "file_operate/file_op.h"
#include "vm/vm_api.h"

#if defined(_SUPPORT_DISC_)
#include "disc/disc.h"
#endif
#if defined(_SUPPORT_CAN_UART_)
#include "ew3048\ew3048.h"
#endif

#include "ui_drv.h"

#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_System_Status(void);
#endif
#if defined(POWER_DISCONNECT_BT_)||defined(_ENABLE_POWER_BT_OFF_)
extern void recon_standby_bt(void);
#endif

#if defined(_ENABLE_TEMP_PWM_)
extern void PWM0_init(uint8 duty);
#endif

#include "dev_ctl.h"

#ifdef SUPPORT_CALL_NAMES
#include "play_sel/play_sel.h"
uint8 powerup_flag=FALSE;
uint8 play_song_flag=FALSE;//TRUE:正在播放语音
uint8 play_once_flag = FALSE;

uint8 play_end_flag=false;
uint8 acc_up_flag=FALSE;
#endif
#if defined(TD_ADD_ACCOFF_TIMEOUT_RESET)
u8 gAccOffFlag=false;
#endif

#if defined( _SUPPORT_LCM_1618_DRIVE )
#include "Lcm_1618_drive.h"
#endif

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);

#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime;
extern u8 AccDetTime;
extern void Spp_Tx_Idle(void);
extern void Spp_Tx_AccOff(void);
#endif
#if defined(TD_ADD_NEW_APP)
extern u8 power_up_flag;
#endif




enum{
	DISP_TYPE_CLEAN = 0,
	DISP_TYPE_RTC,
	DISP_TYPE_WELCOME,
	DISP_TYPE_GOODBYE,
	DISP_TYPE_ICON_ENABLE,
	DISP_TYPE_ICON_DISABLE,
#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
	DISP_TYPE_PASSWORD,
#endif
};

static void idle_disp_info(uint8 disp_type)
{
	idle_puts("idle_disp_info\n");

	typedef struct{
		uint8 disp_type;
	}idle_info_t;

	idle_info_t *idle_info = NULL;
	idle_info = (idle_info_t *)malloc_fun(idle_info ,sizeof(idle_info_t),0);

	idle_info->disp_type  = disp_type;

	idle_printf("disp_type:%d\n",disp_type);

	disp_send_param(DISP_EVENT_IDLE_PLAY, idle_info);

	free_fun((void**)&idle_info);
}

#if defined(_ENABLE_POWER_PIN_)
void poweron_io_output(uint8 flag)
{
	if(flag)
	{//打开3.3V
		dac_ldo_set(LDO_1,1);
		dac_ldo_set(LDO_2,1);
	}
	else//关闭3.3V
	{
		dac_ldo_set(LDO_1,0);
		dac_ldo_set(LDO_2,0);
	}
}
#endif

static void idle_power_on(void)
{
	SYS_POW_ON();
#if defined(_ENABLE_RADIO_LIGHT_)
	AMP_CONTROL_ON();
#endif
#if defined(_SUPPORT_BACKLIGHT_CONTROL_)
	SYS_BACKLIGHT_ON();
#endif

#if defined(_SUPPORT_PANEL_PWM_)
	//set_panel_enable(ENABLE);
#endif
#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
	global.amp_on = TRUE;
	global.amp_time = 0;
#endif
}

static void idle_power_off(void)
{
	SYS_POW_OFF();
#if defined(_VFD_DRIVER_IC_TM1729_)
	LCD_DATA_INIT();LCD_DATA_OUT(); LCD_DATA_LOW();
	LCD_CLK_INIT(); LCD_CLK_OUT();	LCD_CLK_LOW();
#endif

#if defined(_ENABLE_RADIO_LIGHT_)
	AMP_CONTROL_OFF();
#endif
#if defined(_SUPPORT_BACKLIGHT_CONTROL_)
	SYS_BACKLIGHT_OFF();
#endif
#if defined(_SH1302_AKIN_OUTPUT_PIN)
    AKIN_OUTPUT_OFF();
#endif
#if defined(_ENABLE_OPTICAL_CONTROL_)
	SYS_OPTICAL_OFF();
#endif

#if defined(_ENABLE_BEEP_)
#if defined(SET_BEEP_FUNCV)
	set_beep_ctrl(0);
#else
	set_panel_pwm(0, 0, 0);
#endif
#endif

#if defined(_ENABLE_LIGHT_LOW_)
    SYS_RED_OFF();
    SYS_GREEN_OFF();
    SYS_BLUE_OFF();
#endif

#if defined(_ENABLE_ALARM_CLOCK_)
	set_panel_pwm(0, 0, 0);
#endif
}

#if defined(_SUPPORT_DTH_)
#include "file_operate/file_op.h"
#include "dev_manage/dev_ctl.h"

#if defined(_SUPPORT_DISC_)
static uint8 idle_dth_card_state(void)
{
	static u8 count = 0;
	static u8 state = 0;
	if(count == 0)
		state = DETECT_SD_CHECK();
	if(DETECT_SD_CHECK())
	{
		if(++count > 6)
		{
			state = TRUE;
		}
	}
	else
	{
		count = 1;
		state = FALSE;
	}
	return state;
}
#else
static uint8 idle_dth_card_state(void)
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

#if defined(_SUPPORT_DISC_)
static void idle_dth_polling(void)
{
	extern const struct task_info fm_radio_task_info;
	uint8 count = 0;
	uint8 memory_flag = FALSE;
	uint8 card_now = idle_dth_card_state();
	uint8 card_state;

	//DISABLE_INT();
	global.main_msg_disable = TRUE;

	while(TRUE)
	{
		if(!ACC_CHECK())
		{
			break;
		}

		if(DTH_CHECK())
		{
			if(++count > 10)
			{
				break;
			}
		}
		else
		{
			count = 0;
		}

		card_state = idle_dth_card_state();
		if(card_now != card_state)
		{
			idle_printf("card_state:%d\n", card_state);

			card_now = card_state;
			if(card_now)
			{
			#if !defined(ENBLE_SD_MEMORY)
				setup.sd_break =0;//解决SD时关机 拔SD 再插SD有记忆的问题
			#endif

				if(setup.task_name)
				{
					if(strstr(setup.task_name, "Task") != NULL)
					{
						if(strcmp(setup.task_name, PLAYER_CARD_TASK_NAME))
						{
							setup.task_name = PLAYER_CARD_TASK_NAME;
							memory_flag = TRUE;
						}
					}
					else
					{
						setup.task_name = PLAYER_CARD_TASK_NAME;
						memory_flag = TRUE;
					}
				}
				else
				{
					setup.task_name = PLAYER_CARD_TASK_NAME;
					memory_flag = TRUE;
				}
			}
			else
			{
				if(setup.task_name)
				{
					if(strstr(setup.task_name, "Task") != NULL)
					{
						if(!strcmp(setup.task_name, PLAYER_CARD_TASK_NAME))
						{
							setup.task_name = fm_radio_task_info.name;
							memory_flag = TRUE;
						}
					}
				}
			}
		}
		delay(2000);
	}

	if(memory_flag)
	{
		setup_exit();
	}

	if(!ACC_CHECK())
	{//	使能唤醒
		system_lowpower_api();
	}
	else
	{//	软件复位
		system_reset_api();
	}
}
#else
static void idle_dth_polling(void)
{
extern const struct task_info fm_radio_task_info;
extern const struct task_info music_card_task_info;

	uint8 count = 0;
	uint8 memory_flag = FALSE;
	uint8 card_now = idle_dth_card_state();
	uint8 card_state;

	//DISABLE_INT();
	global.main_msg_disable = TRUE;

	while(TRUE)
	{
		if(DTH_CHECK())
		{
			if(++count > 10)
			{
				break;
			}
		}
		else
		{
			count = 0;
		}

		if(!ACC_CHECK())
		{
			break;
		}

		card_state = idle_dth_card_state();

		if(card_now != card_state)
		{
			idle_printf("card_state:%d\n", card_state);

			card_now = card_state;
			if(card_now)
			{
			#if !defined(ENBLE_SD_MEMORY)
				setup.sd_break =0;//解决SD时关机 拔SD 再插SD有记忆的问题
			#endif
			
		    #if defined(_ENABLE_ACC_OFF_LOGO_)
		    	setup.acc_on_flag = TRUE;
		    #endif

		    #if defined(_ENABLE_BT_MODE_MEMORY_)
		    	if(card_state)//蓝牙模式取面板再插SD卡，合上面板，不转SD卡
		    	{
		    		idle_puts("card_state11111\n");
		    		setup.acc_bt = FALSE;
		    		setup.sd_start = TRUE;
		    	}
		    	else
		    	{
		    		setup.sd_start = FALSE;
		    	}
		    #endif

				if(setup.task_name)
				{
					if(strstr(setup.task_name, "Task") != NULL)
					{
						if(strcmp(setup.task_name, MUSIC_CARD_NAME))
						{
							setup.task_name = music_card_task_info.name;
							memory_flag = TRUE;
						}
					}
					else
					{
						setup.task_name = music_card_task_info.name;
						memory_flag = TRUE;
					}
				}
				else
				{
					setup.task_name = music_card_task_info.name;
					memory_flag = TRUE;
				}
			}
			else
			{
				if(setup.task_name)
				{
					if(strstr(setup.task_name, "Task") != NULL)
					{
						if(!strcmp(setup.task_name, MUSIC_CARD_NAME))
						{
							setup.task_name = fm_radio_task_info.name;
							memory_flag = TRUE;
						}
					}
				}
			}
		}

		delay(2000);
	}

	if(memory_flag)
	{
		setup_exit();
	}

	if(!ACC_CHECK())
	{//	使能唤醒
	#if defined(_FY700B_PARTY_CONTROL_)||defined(_XZC1668_ADKEY_CONTROL_)||defined(_ENABLE_ACC_IR_)
		system_reset_api();
	#else
		system_lowpower_api();
	#endif
	}
	else
	{//	软件复位
		system_reset_api();
	}
}
#endif
#endif


u8 sleep_flag=0;
extern void set_pwrmd(u8 mode);
extern void set_sys_ldo_level(u8 level);
extern void adc_init_api(u32 channel,u32 lsb_clk,u8 lvd_en);
void set_io_to_low_power_mode()
{
#if !defined(_FY700B_PARTY_CONTROL_)
#if !defined(_XZC1668_ADKEY_CONTROL_)
#if !defined(_ENABLE_ACC_IR_)
	dac_off_control(); //close dac mudule
#endif
#endif
#endif

    SFR(WLA_CON17, 10, 4, 0x0);   //osc HCS
    SFR(WLA_CON17, 0, 5, 0x7);    //osc CLS
    SFR(WLA_CON17, 5, 5, 0x7);    //osc CRS
    SFR(WLA_CON14, 13, 1, 0x0);   //osc bt oe
    SFR(WLA_CON14, 14, 1, 0x1);   //osc fm oe
    SFR(WLA_CON17, 14, 2, 0x0);   //osc LDO level
    SFR(WLA_CON14, 11, 1, 0x0);   //osc ldo en
    SFR(WLA_CON14, 12, 1, 0x0);   //osc test
    SFR(WLA_CON18, 2, 2, 0x0);    //osc xhd current

    uart_module_off();

    JL_AUDIO->LADC_CON = 0;
    JL_AUDIO->DAC_CON = 0;
    JL_AUDIO->ADA_CON0 = 0;
    JL_AUDIO->ADA_CON1 = 0;
    JL_AUDIO->ADA_CON2 = 0;

    JL_AUDIO->DAA_CON0 = 0;
    JL_AUDIO->DAA_CON1 = 0;
    JL_AUDIO->DAA_CON2 = 0;
    JL_AUDIO->DAA_CON3 = 0;
    JL_AUDIO->DAA_CON4 = 0;
    JL_AUDIO->DAA_CON5 = 0;

    SFR(JL_FMA->CON1, 12, 1,  0x0);

#if defined(_LCD_DRIVER_A4900B_)
	JL_PORTA->DIR = 0x0000;
#elif defined(_LCD_DRIVER_A4901_)
	JL_PORTA->DIR = 0x0006;
#else
	JL_PORTA->DIR = 0xffff;
#endif
    JL_PORTA->PU  = 0;
    JL_PORTA->PD  = 0;
    JL_PORTA->DIE = 0;

#if !defined(_FY700B_PARTY_CONTROL_)
#if !defined(_XZC1668_ADKEY_CONTROL_)
#if !defined(_ENABLE_ACC_IR_)
    JL_PORTB->DIR = 0xffff;
    JL_PORTB->PU  = 0;
    JL_PORTB->PD  = 0;
    JL_PORTB->DIE = 0;
#endif
#endif
#endif

    JL_PORTC->DIR = 0xffff;
    JL_PORTC->PU  = 0;
    JL_PORTC->PD  = 0;
    JL_PORTC->DIE = 0;

    JL_PORTD->DIR |= 0xfff0;
    JL_PORTD->PU  &= ~0xfff0;
    JL_PORTD->PD  &= ~0xfff0;
    JL_PORTD->DIE &= ~0xfff0;

    //PORTR_PU(PORTR0, 0);
    //PORTR_PD(PORTR0, 0);
    //PORTR_DIR(PORTR0, 1);
    //PORTR_DIE(PORTR0, 0);

#if !defined(_ENABLE_AMPLIFIER_CONTROL_)
    PORTR_PU(PORTR2, 0);
    PORTR_PD(PORTR2, 0);
    PORTR_DIR(PORTR2, 1);
    PORTR_DIE(PORTR2, 0);
#endif

    //PORTR_PU(PORTR3, 0);
    //PORTR_PD(PORTR3, 0);
    //PORTR_DIR(PORTR3, 1);
    //PORTR_DIE(PORTR3, 0);

#ifndef USE_USB_DM_PRINTF
    //USB_DP_DIR(1); //DP设置为输入
    //USB_DM_DIR(1); //DP设置为输入
    //USB_DP_PD(0);
    //USB_DM_PD(0);
    //USB_DM_PU(0);
    //USB_DP_PU(0);

#endif  //USE_USB_DM_PRINTF
	JL_PWM->TMR0_CON = 0;
	JL_PWM->TMR1_CON = 0;
	JL_PWM->TMR3_CON = 0;

    JL_AUDIO->LADC_CON = 0;
    JL_AUDIO->ADA_CON0 = 0;
    JL_AUDIO->ADA_CON1 = 0;
    JL_AUDIO->ADA_CON2 = 0;

	JL_AUDIO->DAA_CON0 = 0;
	JL_AUDIO->DAA_CON1 = 0;
	JL_AUDIO->DAA_CON2 = 0;
	JL_AUDIO->DAA_CON3 = 0;
	JL_AUDIO->DAA_CON4 = 0;
	JL_AUDIO->DAA_CON5 = 0;

	//JL_SD0->CON1 = 0;
	//JL_SD1->CON1 = 0;

	JL_USB->CON0 = 0;
	JL_USB->IO_CON |= BIT(4);
	JL_NFC->CON0 = 0;

	JL_FMA->CON0 = 0;
	JL_FMA->CON1 = 0;
	JL_FMA->CON2 = 0;
	JL_FMA->CON3 = 0;
	JL_FMA->CON4 = 0;

    //SFR(JL_SYSTEM->LDO_CON, 15, 3, 0b110);     // 3.3 -> 1.2 DVDD set to 1.0v
}
void idle_io_sleep(void)
{
#if !defined(_ENABLE_AMPLIFIER_CONTROL_)
    PORTR_PU(PORTR2,0);
	PORTR_PD(PORTR2,0);
	PORTR_DIR(PORTR2,1);
	PORTR_DIE(PORTR2,1);
	PORTR_HD(PORTR2,0);
#endif
	JL_PORTA->PU &= ~(BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(9)|BIT(10)|BIT(11)|BIT(14)|BIT(15));
	JL_PORTA->PD &= ~(BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(9)|BIT(10)|BIT(11)|BIT(14)|BIT(15));
	JL_PORTA->HD &= ~(BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(9)|BIT(10)|BIT(11)|BIT(14)|BIT(15));
    JL_PORTA->DIR |= (BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(9)|BIT(10)|BIT(11)|BIT(14)|BIT(15));

	JL_PORTB->PU &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(9)|BIT(10)|BIT(11)|BIT(12)|BIT(13));
	JL_PORTB->PD &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(9)|BIT(10)|BIT(11)|BIT(12)|BIT(13));
	JL_PORTB->HD &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(9)|BIT(10)|BIT(11)|BIT(12)|BIT(13));
    JL_PORTB->DIR |= (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(9)|BIT(10)|BIT(11)|BIT(12)|BIT(13));

	JL_PORTC->PU &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));
	JL_PORTC->PD &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));
	JL_PORTC->HD &= ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));
    JL_PORTC->DIR |= (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5));
}
void sleep_bt_exit(void)
{
    u8 cnt=50;
	idle_puts("sleep_bt_exit\n");
	key_msg_register(NULL, KEY_PROJECT_MAX);
	/*if(os_task_del_req(BTSTACK_TASK_NAME) != OS_TASK_NOT_EXIST)
	{
		os_taskq_post_event(BTSTACK_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
		do{
			OSTimeDly(1);
		} while((os_task_del_req(BTSTACK_TASK_NAME) != OS_TASK_NOT_EXIST)&&(--cnt));
		idle_puts("del_BTSTACK_TASK: succeed\n");
	}
    cnt=50;*/
	if(os_task_del_req(BTMSG_TASK_NAME) != OS_TASK_NOT_EXIST)
	{
		os_taskq_post_event(BTMSG_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
		do{
			OSTimeDly(4);
		} while((os_task_del_req(BTMSG_TASK_NAME) != OS_TASK_NOT_EXIST)&&(--cnt));
		idle_puts("del_BTMSG_TASK: succeed\n");
	}
    cnt=50;
	if(os_task_del_req(BT_TASK_NAME) != OS_TASK_NOT_EXIST)
	{
		os_taskq_post_event(BT_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
		do{
	    	OSTimeDly(4);
		} while((os_task_del_req(BT_TASK_NAME) != OS_TASK_NOT_EXIST)&&(--cnt));
		idle_puts("del_BT_TASK: succeed1111\n");
	}
}
u32 enter_sleep_last_time=0;
void idle_fun(void )
{
	extern void accoff_auto_inc_halfsecond(void);
	extern u32 get_time_tick(void);
	
#if defined(_EVB_ACC_OFF_CURRENT_)
	OSTimeDly(100);
	AUDIO_MUTE_OFF();
#endif

#if defined(_ENABLE_RCA_MUTE_)
	OSTimeDly(100);
	RCA_MUTE_OFF();
#endif

    sleep_bt_exit();
    //idle_io_sleep();
    //set_pwrmd(1);
    set_io_to_low_power_mode();

	clock_init(SYS_CLOCK_INPUT_BT_OSC, OSC_Hz, 24000000L);
	clock_set_sys_div(12);	//sys run 2M
//    clock_set_sys_div(60);  //sys run 2M
   // clock_set_sys_div(1);  //sys run 2M
    //uart_set_baud(9600);
    set_sys_ldo_level(3+1);
    //timer2_init();

	SFR(JL_SYSTEM->LDO_CON, 15, 3, 0b110);     // 3.3 -> 1.2 DVDD set to 1.0v
//    adc_init_api(AD_KEY_LDOIN, clock_get_lsb_freq(), SYS_LVD_EN);

    //charge_timer0_init();
    timer_module_on();
#if !defined(_ENABLE_ACC_MEMORY_)
    timer2_init();
#endif
    sleep_flag =1;
	enter_sleep_last_time = get_time_tick();

#if defined(_KLD_SYS5VPWR_SOFT_CTRL)
	SYS_5VPOW_INIT();
	SYS_5VPOW_OFF();
#endif

#if defined(DRIVE_lCD_CLEAR)||defined(_LCD_LCD_COM5_SEG8)
	extern void lcd_hard_init(uint8 enable);
	lcd_hard_init(FALSE);
	LCD_COM_INIT();
	LCD_COM_OFF();
	LCD_SEG_INIT();
	LCD_SEG_OFF();
#endif
#if defined(_ENABLE_USB_DELAY_)||defined(_SLEEP_MODE_USB_DONT_DETCT)||defined(_SLEEP_MODE_USB_DONT_DETCT)
	global.pwrstarttime = FALSE;
	global.usb_det_delay = 50;
#endif

	while (1) {
		clear_wdt();
		if((get_time_tick()-enter_sleep_last_time)>=3600)
		{
			enter_sleep_last_time = get_time_tick();
			accoff_auto_inc_halfsecond();
		}
		__asm__ volatile("idle");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
	}
}

uint8 detect_usb_state(void)
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

uint8 detect_sd_state(void)
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

static void idle_timer(void *param)
{
	os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_TIMER);
}

#if defined(_ENABLE_BT_ONOFF_DISPLAY_)||defined(_LIANKE_6803_POWER_)
extern void Color6803Refresh(uint16 RData, uint16 GData, uint16 BData);
#endif

#if defined(_AUDIO_DRIVER_AP3768_)
extern void audio_drv_init(void);
#endif

#if defined(_SUPPORT_30P60_LIGHT_)
#if defined(_ENABLE_LIGHT_SHADE_)
extern void LightDriverIrCmd(uint8 cmd,uint8 red,uint8 green,uint8 blue);
#else
extern void LightDriverIrCmd(uint8 cmd);
#endif
#endif

#if defined(_ENABLE_POWER_INPUT_)
extern uint8 pwd_set_save[4];
extern uint8 power_password[4];
#endif

#if defined(DRIVE_LED_COM4_SEG8)||defined(DRIVE_LED_COM5_SEG7)
extern void led_hard_init(void);
#endif

#if defined(_UPDATA_CLEAN_MEMORY_)
extern u8 usb_updata;
#endif

#if defined(_ENABLE_MEMORY_POWER_)
u8 power_set=0;
#endif

static void idle_task(void *p)
{
	int msg[3];
	int timer_cnt = 0;
	uint8 power_on_flag = FALSE;
	uint8 power_off_flag = FALSE;
#if !defined(_ENABLE_RTC_CLK_)
    u8 cnt =20;
#endif

	idle_puts("\n************************Idle TASK********************\n");

	sys_mute_ctrl(ENABLE, 0);

	disp_send_msg(DISP_EVENT_IDLE);

#if defined(_SUPPORT_ACC_)
	idle_printf("ACC_CHECK():%d\n", ACC_CHECK());
#if !defined(_ENABLE_RTC_CLK_)
	if(!ACC_CHECK())
	{
    #if defined(_ENABLE_ACC_OFF_MUTE_)
    	sys_mute_ctrl(ENABLE, 0);
    #endif

	#if defined(_SPECIAL_DELAY_)
	    OSTimeDly(200);// 2s
	#elif defined(_SPECIAL_DELAY_TIME_)
	    OSTimeDly(500);// 5s
	#else
		OSTimeDly(100);// 1s
	#endif
		os_taskq_post_msg(IDLE_TASK_NAME, 1, SYS_EVENT_ACC_OFF);
	}
#endif
#endif

#if defined(_EVB_ACC_DELAY_TIME_)
    if(!ACC_CHECK())
    {
	#if defined(_KEY_LIGHT_CONTROL_)
		poweron_io_output(0);
	#endif
    	OSTimeDly(120);// 1s
    	os_taskq_post_msg(IDLE_TASK_NAME, 1, SYS_EVENT_ACC_OFF);
    }
#endif

#if defined(_ACC_ON_DELAY_)
    if(ACC_CHECK())
	{
		OSTimeDly(120);// 1s
	}
#endif

#if defined(_ACC_ON_LCD_BUG_)
    if(ACC_CHECK())
    {
    	OSTimeDly(100);// 1s
    }
#endif

#if defined(_ENABLE_TDA7265_MUTE_)
	if(!ACC_CHECK())
	{
		OSTimeDly(50);// 1s
		TDA7265_MUTE_ON();
	}
#endif

#if defined(_SUPPORT_DTH_)
    idle_printf("DTH_CHECK():%d\n", DTH_CHECK());
    if(!DTH_CHECK())
    {	OSTimeDly(100);// 1s
    	os_taskq_post_msg(IDLE_TASK_NAME, 1, SYS_EVENT_DTH_OFF);
    }
#endif

#if defined(PLANE_LIGHT_FUNC)||defined(ENABLE_PLANE_LIGHT_CONTROL_)
    idle_printf("ILL_LIGHT_DET_CHECK():%d\n", ILL_LIGHT_DET_CHECK());
    if(!ILL_LIGHT_DET_CHECK())
    {	OSTimeDly(100);// 1s
    	os_taskq_post_msg(IDLE_TASK_NAME, 1, SYS_EVENT_ILL_LIGHT_OFF);
    }
#endif
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(idle_timer, NULL, 200);
#else
	timer_reg_func(idle_timer, NULL, IDLE_POLLING_TIME);
#endif

#if !defined(_FY700B_ICON_CLOSE_)
#if !defined(_XM007_POWER_DISPLAY_)
#if !defined(_WLGK_POWER_OFF_DELAY_)
	global.sys_power = FALSE;
#endif
#endif
#endif

#if defined(_SPECIAL_LOGO_BUG_)
	global.logo_flag = FALSE;
#endif

	Interrupt =FALSE;
#ifdef	TD_ADD_IPONE_CHARGE
	usb_ERR =FALSE;
#endif
	I2C_SCL_L();
	I2C_SDA_L();
#ifdef	TD_ADD_SPP
	Spp_Tx_Idle();
#endif

	printf("setup.task_name222:%s\n", setup.task_name);

	while(1)
	{
		os_taskq_pend(0, ARRAY_SIZE(msg), msg);
		clear_wdt();
		
    #if defined(_ENABLE_POWER_OFF_I2C_LOW_)
    	I2C_SCL_OUT();
    	I2C_SDA_OUT();
    	I2C_SCL_LOW();
    	I2C_SDA_LOW();
    #endif

		switch(msg[0])
		{
			case SYS_EVENT_DEL_TASK:
				idle_puts("idle--->SYS_EVENT_DEL_TASK\n");
				if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
				{
					if(!global.sys_power)
					{
						global.sys_power = TRUE;
						idle_power_on();
				    #if defined(_ENABLE_POWER_PIN_)
				    	poweron_io_output(1);
				    #endif
						OSTimeDly(10);
					}
					idle_disp_info(DISP_TYPE_ICON_ENABLE);
				#if defined(_ENABLE_RADIO_RDS_)
					radio_rds_init();
				#endif
					timer_unreg_func(idle_timer);
					os_task_del_res_name(OS_TASK_SELF);
				}
				break;

			case SYS_EVENT_ACC_OFF:
				idle_puts("idle--->SYS_EVENT_ACC_OFF\n");
			#if defined(TD_ADD_NEW_APP)&&defined(_AUDIO_DRIVER_AP3768_)
				extern void exit_ap3768(void);
				exit_ap3768();//释放ap3768记忆内存
			#endif
			
			#if defined(_ACC_ON_LCD_BUG_)	
    			LCD_CE_INIT();
    			LCD_DATA_INIT();
    			LCD_CLK_INIT();
    			LCD_CE_OUT();
    			LCD_CE_LOW();
    			LCD_DATA_OUT(); 
    			LCD_DATA_LOW();
    			LCD_CLK_OUT();
    			LCD_CLK_LOW();
				setup.pt1622_flag = TRUE;
			#endif
						
		    #if defined(_ENABLE_BT_MODE_MEMORY_)
		    	setup.acc_bt = TRUE;
		    #endif

			#if defined(_ENABLE_LIGHT_SHADE_)
				setup.save_light = 1;
				LightDriverIrCmd(0x00,0,0,0);
			#endif

			#if defined(_AUDIO_DRIVER_AP3768_)
				//setup.acc_ap3768 = 1;
			#endif
			
		    #if defined(_SUPPORT_PASSWORD_DISPLAY_)
		    	if(global.err_flag)
		    	{
					setup.first_power_flag = TRUE;
		    	}
		    #endif

			#if defined(_ENABLE_ACC_OFF_LOGO_)
				if((detect_usb_state())&&(detect_sd_state()))
				{
					idle_puts("SYS_EVENT_ACC_OFF111\n");
					setup.acc_on_flag = TRUE;
				}
			#endif

            #if defined(SD_FM_I2C_PIN_COMMON_USE)
				setup.acc_off_flag = TRUE;
			#endif

			#if defined(_ENABLE_UART_LOW_)
				if(setup.ap3768_power_flag)
				{
					setup.ap3768_power_flag1 = TRUE;
					extern void Close_Audio_Uart2(void);
					Close_Audio_Uart2();
				}
			#endif

			#if defined(_CLEAN_LCD_DISPLAY_)
				 idle_disp_info(DISP_TYPE_ICON_DISABLE);
			#endif

			#if defined(_XM003_BT_NAME_)||defined(_YH1701_BT_NAME_)||defined(_JTI_BT_NAME_)
				setup.acc_off = TRUE;
			#endif

			#if defined(_ENABLE_USB_DELAY_)
				setup.name_flag = TRUE;
			#endif

		    #if defined(_WLS_MUTE_FUNTION_)
		    	AUDIO_MUTE1_ON();
		    #endif

		    #if defined(_ACCOFF_LOW_MUTE_)
		    	AUDIO_MUTE_ON();
		    #endif

			#if defined(TD_ADD_ACCOFF_TIMEOUT_RESET)
                gAccOffFlag =FALSE;
			#endif
			#if defined( _SUPPORT_LCM_1618_DRIVE )
				Clear_1_2_page_disp_flag=0;
			    Clear_page_disp_flag=1;
			#endif
			#if defined(ACC_OFF_PANEL_LIGHT_LOW)
			    SYS_RED_OFF();
			 	SYS_GREEN_OFF();
		  		SYS_BLUE_OFF();
			#endif
			
			#if defined(_ENABLE_PANEL_LIGHT_LOW_)
			    set_panel_pwm(0, 0, 0);
			#endif
			
			#if defined(_ENABLE_PIN_CONTROL_)
			    SYS_POW_OFF();
			#endif
			#if defined(_SUPPORT_TAPE_)
				TAPE_POWER_OFF();
			#endif
			
            #if defined(_ENABLE_CONTROL_POWER_)
            	POWOR_LED_OFF();
            #endif
						
		    #if !defined(_SUPPORT_DISC_)
			#if !defined(_DISABLE_USB_STANBY_)
    			if(detect_usb_state())
    			{
    				setup.usb_standby = TRUE;
    				usb->dev_io->power(DEV_POWER_STANDBY);
    			}
    			if(detect_sd_state())
    			{
    				setup.sd_standby = TRUE;
    				sd1->dev_io->power(DEV_POWER_STANDBY);
    			}
			#endif
			#endif

			#if defined(_DISABLE_MEMORY_POWER_)||defined(_JTI_CHANGE_MODE_)
				setup.acc_flag = TRUE;
			#endif

            #if defined(_ENABLE_CHARGE_MUTE_)
			    setup.accoff_flag = TRUE;
			#endif
			
		    #if defined(POWER_DISCONNECT_BT_)||defined(_ENABLE_BT_ON_OFF_)
		    	if(setup.power_off_flag)
		    	{
					setup.acc_off_flag = TRUE;
				}
		    #endif

			#if defined(_ENABLE_RADIO_RDS_)
				radio_rds_exit();
			#endif

				close_wdt();
			#if !defined(DRIVE_lCD_CLEAR)
				idle_power_off();				// PWR OFF
            #endif
			
		    #if defined(_SL6812_ILL_LIGHT_)
		    	ILL_LIGHT_OFF();
		    #endif
			
			#if defined(_ENABLE_POWER_PIN_)
			#if !defined(_KEY_LIGHT_CONTROL_)
				poweron_io_output(0);
			#endif
			#endif
			
			#if defined(_KLD3000_RADIO_LIGHT_)
				AMP_CONTROL_ON();
			#endif

		    #if defined(_ENABLE_INH_CONTROL_)
		    	LCD_INH_OFF();
		    #endif

            #if defined(_SUPPORT_BACKLIGHT_CONTROL_)
		    	SYS_BACKLIGHT_OFF();
			#endif

		    #if defined(_SUPPORT_DISC_)
		    #if defined(ACC_POW_GPIO)
		    	ACC_POW_OFF();
		    #endif
		    #endif

			#if defined(_ENABLE_PWR_PIN_)
    	    	ACC_POW_OFF();
    			ACC_POW1_OFF();
			#endif
			
			#if defined(_ENABLE_SUB_DETECT_)
				SYS_SUB_OFF();
			#endif
			
			#if defined(_ENABLE_USB_CONTROL_)
				SYS_USB_OFF();
			#endif

			#ifdef	TD_ADD_SPP
				Spp_Tx_AccOff();
			#endif
			    idle_puts("-----idle_puts-22222\n");
			#if !defined(_ENABLE_RTC_CLK_)
                while((get_bt_connect_status()>BT_STATUS_AUTO_CONNECTINT)&&(cnt--))
                {
				#if defined(ENABLE_TWO_PHONE_CONNECT)
					os_taskq_post_event(BT_TASK_NAME, 1, MSG_BT_DISCONNECT);
					os_time_dly(5);
				#else
					os_taskq_post_event(BTMSG_TASK_NAME, 1, MSG_BT_DISCONNECT);
					OSTimeDly(5);
				#endif
				}
			#endif

		    #if defined (DRIVE_LED_7pin_001)||defined(DEBGE_LED_AND_LCD_TOGETHER)
		    	SEG0_OFF();
		    	SEG1_OFF();
		    	SEG2_OFF();
		    	SEG3_OFF();
		    	SEG4_OFF();
		    	SEG5_OFF();
		    	SEG6_OFF();
		    #endif
			
			#if defined(DRIVE_LED_COM4_SEG8)||defined(DRIVE_LED_COM5_SEG7)
				led_hard_init();
			#endif
			
				idle_printf("setup.task_name111:%s\n", setup.task_name);

				disp_task_exit();

				setup_exit();
            #if defined(_ENABLE_MEMORY_POWER_)&&0
				OSTimeDly(5);
            	vm_read_api(VM_POWER_SET, &power_set);
            #endif

		    #if defined(_ENABLE_USB_BATT_CONTROL_)
				OSTimeDly(5);
		    	USB_CONTROL_OFF();
		    #endif
			
			#if defined(_ACC_POWER_CONTROL_)
				OSTimeDly(150);
				ACC_POWER_OFF();
			#endif

			#if defined(_ENABLE_ACC_DIRECTION_)
    			PORTR_DIR(PORTR1,1);
    			PORTR_PD(PORTR1,0);
    			PORTR_PU(PORTR1,1);
			#endif

			#if defined(_ENABLE_ACC_PIN_)
    			PORTR_DIR(PORTR1,1);
    			PORTR_PD(PORTR1,0);
    			PORTR_PU(PORTR1,1);
    			idle_fun();
			#else
    			if(!ACC_CHECK())
    			{// 使能唤醒
    			#if defined(_ENABLE_RTC_CLK_)
					idle_puts("-----33333333333\n");
    				system_lowpower_api();
    			#endif
				#if defined(_ENABLE_ACC_MEMORY_)
					sleep_flag =1;//开关ACC死机
				#endif
    				gEnterAccMode=TRUE;

    				idle_fun();
    			}
    			else
    			{// 软件复位
					idle_puts("-----44444444444\n");
    				system_reset_api();
    			}
			#endif
				break;

        #if defined(_SUPPORT_DTH_)
        	case SYS_EVENT_DTH_OFF:
				idle_puts("idle--->SYS_EVENT_DTH_OFF\n");
		    #if defined(TD_ADD_NEW_APP)&&defined(_AUDIO_DRIVER_AP3768_)
		    	extern void exit_ap3768(void);
		    	exit_ap3768();//释放ap3768记忆内存
		    #endif
			
		    #if defined(_ENABLE_ACC_OFF_LOGO_)
		    	if(detect_sd_state())
		    	{
		    		idle_puts("SYS_EVENT_ACC_OFF111\n");
		    		setup.acc_on_flag = TRUE;
		    	}
		    #endif

		    #if defined(_ENABLE_BT_MODE_MEMORY_)
		    	setup.acc_bt = TRUE;
		    #endif
			
			#if defined(_ENABLE_RADIO_RDS_)
				radio_rds_exit();
			#endif
		    #if defined(_ENABLE_UART_LOW_)
		    	if(setup.ap3768_power_flag)
		    	{
					setup.ap3768_power_flag1 = TRUE;
		    		extern void Close_Audio_Uart2(void);
		    		Close_Audio_Uart2();
		    	}
		    #endif
				close_wdt();

    			//JL_PORTB->PU |= (BIT(3));
    			//JL_PORTB->PD &=~(BIT(3));
    			//JL_PORTB->DIR &=~(BIT(3));
    			//JL_PORTB->OUT |= (BIT(3));

				//set_sys_ldo_level(3+1);
				//SFR(JL_SYSTEM->LDO_CON, 15, 3, 0b110);	   // 3.3 -> 1.2 DVDD set to 1.0v

				idle_power_off();

		    #if defined(_ENABLE_POWER_PIN_)
		    	poweron_io_output(0);
		    #endif

			#if defined(_DISABLE_MEMORY_POWER_)
				setup.acc_flag = TRUE;
			#endif

		    #if defined(_ENABLE_USB_BATT_CONTROL_)
		    	USB_CONTROL_OFF();
		    #endif

		    #if defined(_ENABLE_INH_CONTROL_)
		    	LCD_INH_OFF();
		    #endif
			#if defined(_SUPPORT_DISC_)
		    #if defined(ACC_POW_GPIO)
		    	ACC_POW_OFF();
		    #endif
			#endif
				os_taskq_post_event(BTMSG_TASK_NAME, 1, MSG_BT_DISCONNECT);
				OSTimeDly(5);
				disp_task_exit();
				setup_exit();
				idle_dth_polling();
				break;
        #endif

        #if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
        	case DISP_EVENT_PASSWORD_NO_1:
        		ew_puts("DISP_EVENT_PASSWORD_NO_1\n");
				if(setup.password_flag)
				{
					disp_send_msg(DISP_EVENT_PASSWORD_NO_1);
				}
        		break;

        	case DISP_EVENT_PASSWORD_NO_2:
        		ew_puts("DISP_EVENT_PASSWORD_NO_2\n");
				if(setup.password_flag)
				{
					disp_send_msg(DISP_EVENT_PASSWORD_NO_2);
				}
        		break;

        	case DISP_EVENT_PASSWORD_NO_3:
        		ew_puts("DISP_EVENT_PASSWORD_NO_3\n");
				if(setup.password_flag)
				{
					disp_send_msg(DISP_EVENT_PASSWORD_NO_3);
				}
        		break;

        	case DISP_EVENT_PASSWORD_NO_4:
        		ew_puts("DISP_EVENT_PASSWORD_NO_4\n");
				if(setup.password_flag)
				{
					disp_send_msg(DISP_EVENT_PASSWORD_NO_4);
				}
        		break;
				
			case DISP_EVENT_PASSWORD_NO_5:
				ew_puts("DISP_EVENT_PASSWORD_NO_5\n");
				if(setup.password_flag)
				{
				#if !defined(_SUPPORT_PASSWORD_DISPLAY_)
					disp_send_msg(DISP_EVENT_PASSWORD_NO_5);
				#endif
				}
				break;

			case DISP_EVENT_PASSWORD_NO_6:
				ew_puts("DISP_EVENT_PASSWORD_NO_6\n");
				if(setup.password_flag)
				{
				#if !defined(_SUPPORT_PASSWORD_DISPLAY_)
					disp_send_msg(DISP_EVENT_PASSWORD_NO_6);
				#endif
				}
				break;

        	case DISP_EVENT_PASSWORD_ENTER:
        		ew_puts("DISP_EVENT_PASSWORD_ENTER\n");
				if(setup.password_flag)
				{
					disp_send_msg(DISP_EVENT_PASSWORD_ENTER);
				}
        		break;

        	case DISP_EVENT_PASSWORD:
        		global.sys_power = TRUE;
        		setup.password_flag = TRUE;
			#if defined(_ENABLE_PASSWORD_SET_)
				if(setup.pwd_set)
				{
					global.pwd_set_flag = TRUE;
				}
			#endif
        		idle_disp_info(DISP_TYPE_PASSWORD);
        		break;
        #endif
		
			case MSG_IDLE_DETECT:
				idle_puts("MSG_IDLE_DETECT\n");
			#if defined(USB_SD_DIS_TONE_PLAY)
				extern u8 USB_SD_DIS_TONE_PLAY_FLAG;
				USB_SD_DIS_TONE_PLAY_FLAG=0;
			#endif

		    #if defined(_ENABLE_ACC_MODE_)
		    	if(setup.task_name == music_usb_task_info.name)
		    	{
		    		setup.playusb_flg = TRUE;
		    	}
		    	if(setup.task_name == music_card_task_info.name)
		    	{
		    		setup.playsd_flg = TRUE;
		    	}
		    #endif

			#if defined(_SUPPORT_CALL_RECORDS_)
				global.info.call_info_flag = TRUE;
			#endif

			#if defined(_ENABLE_USB_SAVE_)||defined(_ENABLE_USB_DELAY_)||defined(_SLEEP_MODE_USB_DONT_DETCT)//修改开关ACC 后不能记忆到模式
			#if defined(_JTI_USB_DELAY_)
				global.power_time = 80;
			#else
				global.power_time = 50;
			#endif
			#endif

			#if defined(_USB_DELAY_CHECK_)
				setup.usb_delay = 90;
			#endif

				if(setup.task_name)
				{
					idle_printf("--->name:%s\n", setup.task_name);

			    #if defined(_ENABLE_INH_CONTROL_)
			    	LCD_INH_ON();
			    #endif

					if((strstr(setup.task_name, "Task") != NULL) && strcmp(setup.task_name, IDLE_TASK_NAME))
					{
						idle_puts("MSG_IDLE_DETECT111\n");
						
					#if defined(_DISABLE_MEMORY_POWER_)
						if((setup.task_name != "IdleTask")&&(!setup.acc_flag))
						{
							idle_disp_info(DISP_TYPE_RTC);
							setup.app_name = setup.task_name;
						}
						else
						{
							global.sys_power = TRUE;
							os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_EXIT);
						}
					#else
						global.sys_power = TRUE;
					#if defined(_SPECIAL_LOGO_BUG_)
						global.logo_flag = TRUE;
					#endif
					
						os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_EXIT);
					#endif

					#if defined(AUDIO_MUTE_SPECIAL_SET)
						//dac_mute(1,1);
						//HAUDIO_MUTE_ON();
					#endif
					}
					else
					{
						idle_puts("MSG_IDLE_DETECT222\n");
				    #if defined(_ENABLE_UART_LOW_)//关机,关ACC,再上ACC
				    	AUDIO_RX_INIT();
				    	AUDIO_RX_OUT();
				    	AUDIO_RX_LOW();
				    	AUDIO_TX_INIT();
				    	AUDIO_TX_OUT();
				    	AUDIO_TX_LOW();
				    #endif
				    #if defined(_ENABLE_AUTO_POWER_)
					    printf("setup.power_flag222222222222222=%d\n",setup.power_flag);
				    	if(setup.power_flag)
				    	{
				    		setup.power_flag = FALSE;
				    		setup.task_name = setup.save_mode_name;
				    		os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_EXIT);
				    	}
				    #endif
					
                    #if defined(_ENABLE_MEMORY_POWER_)
					    vm_read_api(VM_POWER_SET, &power_set);
						//printf("power_set=%d\n",power_set);
						if((power_set)&&(ACC_CHECK()))
						{
							power_set = 0;
					        vm_write_api(VM_POWER_SET, &power_set);
				    		os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_EXIT);
						}
					#endif
					
					#if defined(PLANE_LIGHT_FUNC)
						SYS_POWOR_LED_ON();
					#endif
					
				    #if defined(_POWER_OFF_FUNTION_)
				    	idle_power_on();
				    #endif

				    #if !defined(_SUPPORT_DISC_)
					#if defined(_FEIYIN_SPECIAL_DELAY_)
						if((detect_usb_state())&&(detect_sd_state()))
						{
				    		setup.sd_standby = TRUE;
				    		setup.usb_standby = TRUE;
				    		os_time_dly(300);
				    		usb->dev_io->power(DEV_POWER_STANDBY);
				    		sd1->dev_io->power(DEV_POWER_STANDBY);
						}
						else
						{
							if(detect_usb_state())
							{
								setup.usb_standby = TRUE;
								os_time_dly(50);
								usb->dev_io->power(DEV_POWER_STANDBY);
							}
							if(detect_sd_state())
							{
								setup.sd_standby = TRUE;
								os_time_dly(50);
								sd1->dev_io->power(DEV_POWER_STANDBY);
							}
						}
					#else
				    #if !defined(_DISABLE_USB_STANBY_)
				    	if(detect_usb_state())
				    	{
				    		setup.usb_standby = TRUE;
						#if defined(_RX_SPECIAL_DELAY_)
				    		os_time_dly(200);
						#else
				    		os_time_dly(50);
						#endif
				    		usb->dev_io->power(DEV_POWER_STANDBY);
				    	}
				    	if(detect_sd_state())
				    	{
				    		setup.sd_standby = TRUE;
                        #if defined(_RX_SPECIAL_DELAY_)
							os_time_dly(200);
                        #else
							os_time_dly(50);
                        #endif
				    		sd1->dev_io->power(DEV_POWER_STANDBY);
				    	}
					#endif
					#endif
				    #endif
					

					#ifdef SUPPORT_CALL_NAMES
                        os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_PLAY);
					#endif
				    #if defined(_POWER_OFF_DISABLE_TIME_)
				        idle_disp_info(DISP_TYPE_CLEAN);
				    #else
				    	idle_disp_info(DISP_TYPE_RTC);
				    #endif
					
					#if defined(_LIANKE_6803_POWER_)
						Color6803Refresh(31, 31, 31);
					#endif
					
				    #if defined(_ENABLE_POWER_OFF_CURRENT_)
				    	set_sys_freq(BT_RCCL_SYS_Hz);
				    #endif

					#if defined(_ENABLE_POWER_INPUT_)
						if(setup.first_input)
						{
						#if defined(_ENABLE_MEMORY_PASSWORD_)
							vm_read_api(VM_PWD_SET, &pwd_set_save);
							printf("save[0]:0x%x\n",pwd_set_save[0]);
							printf("save[1]:0x%x\n",pwd_set_save[1]);
							printf("save[2]:0x%x\n",pwd_set_save[2]);
							printf("save[3]:0x%x\n",pwd_set_save[3]);
							if(!memcmp(power_password, pwd_set_save, 4))
							{
								puts("_ENABLE_MEMORY_PASSWORD11111_\n");
								pwd_set_save[0] = 0x31;
								pwd_set_save[1] = 0x31;
								pwd_set_save[2] = 0x31;
								pwd_set_save[3] = 0x31;
								vm_write_api(VM_PWD_SET, &pwd_set_save);
							}
						#endif
							os_taskq_post_msg("IdleTask", 1, DISP_EVENT_PASSWORD);
						}
					#endif
					}
				}
				else
				{
					idle_puts("MSG_IDLE_DETECT333\n");
				#if defined(_ENABLE_MEMORY_PASSWORD_)
					vm_read_api(VM_PWD_SET, &pwd_set_save);
					printf("save1[0]:0x%x\n",pwd_set_save[0]);
					printf("save1[1]:0x%x\n",pwd_set_save[1]);
					printf("save1[2]:0x%x\n",pwd_set_save[2]);
					printf("save1[3]:0x%x\n",pwd_set_save[3]);
				#endif
					
			    #if defined(_ENABLE_UART_LOW_)//第一次上电的时候也要拉低
					AUDIO_RX_INIT();
					AUDIO_RX_OUT();
					AUDIO_RX_LOW();
					AUDIO_TX_INIT();
					AUDIO_TX_OUT();
					AUDIO_TX_LOW();
			    #endif
				
				#if defined(_UPDATA_CLEAN_MEMORY_)
			     	usb_updata = 0;
			     	vm_write_api(VM_USB_UPDATA, &usb_updata);
				#endif

			    #if defined(_ENABLE_MUTE_OFF_)
			    	global.mute_power_flag = TRUE;
			    	global.mute_power_time = 20;
			    #endif

				#if defined(_SUPPORT_CAN_UART_)
					u8 mcu_send_buf1[8]={0x01,setup.can_mode,0x00,0x00,0x00,0x00,0x00,0x00};
					Mcu_Send_Data(mcu_send_buf1,8);

					u8 mcu_send_buf2[8]={0x01,setup.can_speed,0x00,0x00,0x00,0x00,0x00,0x00};
					Mcu_Send_Data(mcu_send_buf2,8);
				#endif

				#if defined(PLANE_LIGHT_FUNC)
					SYS_POWOR_LED_ON();
				#endif

			    #if defined( _SUPPORT_LCM_1618_DRIVE )
			    	lcm_clear();//清屏
			    #endif

				#ifdef SUPPORT_CALL_NAMES
					if(acc_up_flag ==TRUE)
					{
						acc_up_flag=FALSE;
                        os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_PLAY);
					}
				#endif

				#if defined(_ENABLE_AUTO_POWER_ON_)
					os_taskq_post_event("MainTask", 1, MSG_SYS_POWER);
				#endif

			    #if defined(_ENABLE_INH_CONTROL_)
			    	LCD_INH_ON();
			    #endif

				#if defined(_POWER_OFF_FUNTION_)
					idle_power_on();
				#endif

			    #if defined(_POWER_OFF_DISABLE_TIME_)
			    	idle_disp_info(DISP_TYPE_CLEAN);
			    #else
			    	idle_disp_info(DISP_TYPE_RTC);
			    #endif
				
				#if defined(_LIANKE_6803_POWER_)
					//Color6803Refresh(31, 31, 31);
				#endif

			    #if defined(_USB_DELAY_CHECK_)
			    	setup.usb_delay = 0;
			    #endif

				#if defined(_SUPPORT_BACKLIGHT_CONTROL_)
					SYS_BACKLIGHT_OFF();
				#endif
				
                #if !defined(_SUPPORT_DISC_)
				#if !defined(_DISABLE_USB_SD_STANBY_)
                #if !defined(_DISABLE_USB_STANBY_)
                	if(detect_usb_state())
                	{
                		setup.usb_standby = TRUE;

                    #if defined(_RX_SPECIAL_DELAY_)
						os_time_dly(200);
                    #else
						os_time_dly(50);
                    #endif
                		usb->dev_io->power(DEV_POWER_STANDBY);
                	}
                	if(detect_sd_state())
                	{
                		setup.sd_standby = TRUE;
				    #if defined(_RX_SPECIAL_DELAY_)
						os_time_dly(200);
                    #else
						os_time_dly(50);
                    #endif
                		sd1->dev_io->power(DEV_POWER_STANDBY);
                	}
                #endif
				#endif
				#endif
				
			    #if defined(_ENABLE_POWER_OFF_CURRENT_)
			    	set_sys_freq(BT_RCCL_SYS_Hz);
			    #endif

			    #if defined(_ENABLE_POWER_INPUT_)
			    	os_taskq_post_msg("IdleTask", 1, DISP_EVENT_PASSWORD);
			    #endif
				}

				global.sys_normal = TRUE;
				break;

			case MSG_IDLE_ENTER://	power down
				idle_puts("------------MSG_IDLE_ENTER--------------\n");

			#if defined(_ENABLE_MUTE_FUNTION_)
				FEIYIN_MUTE_ON();
			#endif
			
		    #if defined(_ENABLE_AMPLIFIER_CONTROL_)
		    	AMPLIFIER_LOW();
		    #endif
			
		    #if defined(_YH1701_BT_NAME_)
		    	setup.acc_off = TRUE;
		    #endif

			#if defined(_TEMP_WARM_DISPLAY_)
				global.temp_power_flag = FALSE;
			#endif
			
		    #if defined(_ENABLE_TEMP_PWM_)
		    	PWM0_init(0);
		    #endif

		    #if defined(_ENABLE_TEMP_ICON_DISPLAY_)
		    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_TEMP);
		    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_WARM);
		    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SET);
		    #endif

		    #if defined(_DSP_RCA_MUTE_)//关机不要操作RCA MUTE
		    	//DSP_RCA_MUTE_ON();
		    #endif

			#if defined(_ENABLE_MUTE_OFF_)
				global.mute_power_flag = TRUE;
				global.mute_power_time = 40;
			#endif

			#if defined(_POWER_OFF_SUB_)
				global.sub_power_flag = TRUE;
				global.sub_power_time = 0;
				AMP_CONTROL_OFF();
			#endif

			#if defined(_CLOSE_AMP_LIGHT_)
				global.rca_power = FALSE;
			#endif

			#if defined(_ENABLE_SUB_MUTE_)||defined(_FY7000_SUB_MUTE_)
				if(setup.aud_sub)
				{
					SYS_SUB_OFF();
				}
				if(setup.aud_sub1)
				{
					SYS_SUB2_OFF();
				}
			#endif
			
		    #if defined(_ENABLE_AUTO_POWER_)
		    	setup.power_flag = FALSE;
		    #endif
			
            #if defined(_ENABLE_MEMORY_POWER_)
				power_set = 0;
				vm_write_api(VM_POWER_SET, &power_set);
			#endif
			
			#if defined(ACC_OFF_PANEL_LIGHT_LOW)
				SYS_RED_OFF();
				SYS_GREEN_OFF();
				SYS_BLUE_OFF();
			#endif
			
            #if defined(_ENABLE_PANEL_LIGHT_LOW_)
            	set_panel_pwm(0, 0, 0);
            #endif
			
			#if defined(PLANE_LIGHT_FUNC)
				SYS_POWOR_LED_ON();
			#endif
			
            #if defined(_ENABLE_CONTROL_POWER_)
			    POWOR_LED_OFF();
			#endif
			
			#if defined(USB_SD_DIS_TONE_PLAY)
				extern u8 USB_SD_DIS_TONE_PLAY_FLAG;
				USB_SD_DIS_TONE_PLAY_FLAG=0;

			#endif
			#if defined(DAB_UART_ONOFF)
				extern void Close_DAB_Uart2(void);
				Close_DAB_Uart2();
			#endif

		    #if defined(_ENABLE_UART_LOW_)
		    	extern void Close_Audio_Uart2(void);
		    	Close_Audio_Uart2();
		    	setup.ap3768_power_flag = FALSE;
		    #endif

				if(global.sys_power || !power_off_flag)
				{
					idle_puts("MSG_IDLE_ENTER111\n");
			    #if !defined(_FY700B_ICON_CLOSE_)
				#if !defined(_ENABLE_DELAY_POWER_)
				#if !defined(_XM007_POWER_DISPLAY_)
				#if !defined(_WLGK_POWER_OFF_DELAY_)
			    	global.sys_power = FALSE;
			    #endif
				#endif
				#endif
				#endif
				
    			#if defined(_XM007_POWER_DISPLAY_)
    				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POWER);
    				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POWER2);
    			#endif
			
				#if defined(_SPECIAL_LOGO_BUG_)
					global.logo_flag = FALSE;
				#endif

			    #if defined(_FY700B_ICON_CLOSE_)
			    	global.power_close = FALSE;
			    #endif

				#if defined(POWER_DISCONNECT_BT_)
					user_send_cmd_prepare(USER_CTRL_DISCONNECTION_HCI,0,NULL);
					os_time_dly(10);
					user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
					user_send_cmd_prepare(USER_CTRL_WRITE_CONN_DISABLE,0,NULL);
					setup.power_off_flag = TRUE;
				#endif
				#if defined(_ENABLE_RADIO_RDS_)
					radio_rds_exit();
				#endif

				#if defined(AUDIO_MUTE_SPECIAL_SET)
					dac_mute(1,1);
					HAUDIO_MUTE_ON();
				#endif

				#if !defined(_SUPPORT_DISC_)
				#if !defined(_DISABLE_USB_STANBY_)
    				if(detect_usb_state())
    				{
    					setup.usb_standby = TRUE;
    					usb->dev_io->power(DEV_POWER_STANDBY);
    				}
    				if(detect_sd_state())
    				{
    					setup.sd_standby = TRUE;
    					sd1->dev_io->power(DEV_POWER_STANDBY);
    				}
				#endif
				#endif

				#if defined(_USER_TFT_SEND_)
					TFT_Tx_System_Status();
				#endif
					idle_disp_info(DISP_TYPE_ICON_DISABLE);
					idle_puts("MSG_IDLE_ENTER222\n");
				#ifdef  Tuobu_B324
                #else
				#if defined( _SUPPORT_LCM_1618_DRIVE )
					enable_disp_set_icom=0;
				#endif
				#if !defined(_DISABLE_POWER_OFF_LOGO_)
					idle_disp_info(DISP_TYPE_GOODBYE);
				#endif
				#endif
				
				#if defined(_XM007_POWER_DISPLAY_)
					idle_disp_info(DISP_TYPE_RTC);
				#endif

				#if defined(_ENABLE_USB_SAVE_)
				#if defined(_WJ361_USB_DELAY_)||defined(_EVB_USB_DELAY_)
					global.power_time = 80;
				#else
					global.power_time = 40;  //30,延时1.5秒
				#endif
				#endif

					power_on_flag = FALSE;
					power_off_flag = TRUE;
				#if defined(_DISABLE_POWER_OFF_LOGO_)
				#if defined(_XM007_POWER_DISPLAY_)
					timer_cnt = 0;
				#else
					timer_cnt = 1000;
				#endif
				#else
					timer_cnt = 0;
				#endif

				#if defined(_XM007_POWER_MUTE_)
					AUDIO_MUTE_ON();
				#endif
				}
				break;

			case MSG_IDLE_EXIT://	power up
				idle_puts("MSG_IDLE_EXIT\n");
				ew_printf("global.sys_power:%d\n",global.sys_power);
				ew_printf("power_off_flag:%d\n",power_off_flag);
				if(global.sys_power || !power_off_flag)
				{					
				#if defined(_ENABLE_BT_ONOFF_DISPLAY_)
    				global.bt_disp_time = 0;
    				global.bt_display = FALSE;
				#endif
				
			    #if defined(_EVB_POWER_ICON_)
			    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER);
			    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POWER2);
			    #endif

				#if defined(_ENABLE_POWER_OFF_CURRENT_)
					set_sys_freq(SYS_Hz);
				#endif

				#if defined(_XM007_SPECIAL_MUTE_)
					global.aud_mute = FALSE;
				#endif
								
			    #if defined(_BT_CALL_MEMORY_RPT_)
			    	global.memory_rpt = 1;
			    #endif
				
				#if defined(_ENABLE_POWER_LOGO_)
					global.power_up_flag = TRUE;
				#endif
				
				#if defined(_ENABLE_POWER_INPUT_)
					setup.first_input = 0;
				#endif

			    #if defined(_ENABLE_AMPLIFIER_CONTROL_)
			    	AMPLIFIER_HIGH();
			    #endif

                #if defined(_ENABLE_CONTROL_POWER_)
                	POWOR_LED_ON();
                #endif
				
				#if defined(_ENABLE_DSP_RESET_)
					global.reset_first = TRUE;
				#endif
				
			    #if defined(_DISABLE_BT_AUTO_CONNECT_)
			    #if !defined(YH1701A_SOFT_RESET)
			    	setup.acc_on_flag = TRUE;
			    #endif
			    #endif

			    #if defined(_ENABLE_MUTE_FUNTION_)
			    	FEIYIN_MUTE_ON();
			    #endif
				
			    #if defined(_DSP_RCA_MUTE_)
			    	DSP_RCA_MUTE_ON();
			    #endif

				#if defined(_AUDIO_DRIVER_AP3768_)
					global.power_mode_flag = TRUE;
				#endif

			    #if defined(_CLOSE_AMP_LIGHT_)
			    	global.rca_power = TRUE;
			    #endif

			    #if defined(_FY700B_ICON_CLOSE_)
			    	global.power_close = TRUE;
			    #endif
				
			    #if defined(_ENABLE_VOLUME_CONTROL_)
			    	if(!setup.zone1_start)
			    	{
			    		SYS_ZONE1_INIT();
			    		SYS_ZONE1_ON();
			    	}
			    #endif

				#if defined(POWER_DISCONNECT_BT_)
					user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
					user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE,0,NULL);
					recon_standby_bt();
					setup.power_off_flag = FALSE;
					setup.acc_off_flag = FALSE;
				#endif
				
			    #if defined(_ENABLE_POWER_BT_OFF_)
			    	user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
			    	user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE,0,NULL);
			    	if(global.bt_off_flag)
			    	{
			    		puts("------------_ENABLE_POWER_BT_OFF_-------------\n");
			    		recon_standby_bt();
			    		global.bt_off_flag = FALSE;
			    	}
			    #endif

				#if defined(_ENABLE_ACC_OFF_LOGO_)
					global.first_power_on = FALSE;
				#endif

			    #if defined(_ENABLE_AUTO_POWER_)
			    	setup.power_flag = TRUE;
			    #endif

				#if defined(_ENABLE_MEMORY_POWER_)
				    power_set = 1;
					vm_write_api(VM_POWER_SET, &power_set);
				#endif
				
				#if defined(_SPECIAL_LOGO_BUG_)
					global.logo_flag = TRUE;
				#endif

				#if defined(_ENABLE_USB_BATT_CONTROL_)
					USB_CONTROL_ON();
				#endif

			    #if defined(_ENABLE_TIME_DISP_)
			    	global.timer_count = 4000;
			    #endif

			    #if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
			    	setup.first_power_flag = FALSE;
			    	setup.password_flag = FALSE;
			    #endif

			    #if defined( _SUPPORT_LCM_1618_DRIVE )
			    	lcm_clear();//清屏
			    	lcm_disp_line ();
			    	lcm_disp_line ();
			    	Clear_1_2_page_disp_flag=1;
			    	Clear_page_disp_flag=0;
			    #endif

			    #if defined(_ENABLE_POWER_MUTE_)||defined(_ENABLE_FIRST_POWER_MUTE_)
			    	global.mute_flag = TRUE;
			    	global.mute_time = 90;
			    #endif

				#if defined(DAB_UART_ONOFF)
					extern void OPEN_DAB_Uart2(void);
					OPEN_DAB_Uart2();
				#endif

				#if defined(_ENABLE_UART_LOW_)
					if(!setup.ap3768_power_flag1)
					{
						extern void Open_Audio_Uart2(void);
						Open_Audio_Uart2();
					}
			    	setup.ap3768_power_flag = TRUE;
					setup.ap3768_power_flag1 = FALSE;
				#endif

				#if defined(_SUPPORT_PANEL_PWM_)
					set_panel_pwm(0, 0, 0);
				#endif

				#if defined(_USB_DELAY_CHECK_)
					setup.usb_delay = 0;
				#endif

			    #if defined(HuiNuo_6651D_Copy)
			    	if(setup.aud_vol==0)
			    	{
			    		setup.aud_vol=16;
			    	}
			    	else if(setup.aud_vol>=21)
			    	{
			    		setup.aud_vol=20;
			    	}
				#elif defined(_XM003_DEFAULT_VOL_)
					if(get_cur_vol() > 20)
					{
						set_cur_vol(20);
					}
					else if(get_cur_vol() == 0)
					{
						set_cur_vol(AUDIO_VOLUME_DEFAULT);
					}
				#elif defined(_POWER_ON_VOLUME_MEMORY_)
					if(get_cur_vol() > AUDIO_VOLUME_DEFAULT)
					{
						set_cur_vol(AUDIO_VOLUME_DEFAULT);
					}
				#elif defined(BL_POWER_ON_VOLUME_MEMORY_)
					if(get_cur_vol() > AUDIO_VOLUME_DEFAULT)
					{
						set_cur_vol(AUDIO_VOLUME_DEFAULT);
					}
					else
					{
                        set_cur_vol(AUDIO_VOLUME_DEFAULT);
					}
				#elif defined(_ENABLE_MENU_FUNTION_)
					if(get_cur_vol() > setup.default_vol)
					{
						set_cur_vol(setup.default_vol);
					}
					else if(get_cur_vol() < 5)
					{
						set_cur_vol(5);
					}
				#elif defined(ADD_ADJUST_DEFAULT_VOL)
					set_cur_vol(setup.default_vol);
				#elif defined(_ENABLE_DEFAULT_CONTROL_)
    				if(get_cur_vol() > setup.default_vol)
    				{
    					set_cur_vol(setup.default_vol);
    				}
    				else if(get_cur_vol() < 5)
    				{
    					set_cur_vol(5);
    				}
			    #endif
				#if defined(_CHANGE_DEF_VOL_)
    				if(get_cur_vol() > setup.DefVol)
    				{
    					set_cur_vol(setup.DefVol);
    				}
				#endif

				#if defined(_SPECIAL_TL2000_SET_)
					if(setup.aud_A_volume > 15)
					{
						setup.aud_A_volume = 15;
					}
					if(setup.aud_B_volume > 15)
					{
						setup.aud_B_volume = 15;
					}
			    #endif
				#if defined(_USER_TFT_SEND_)
					TFT_Tx_System_Status();
				#endif
					global.sys_power = TRUE;

				#if defined(AUDIO_MUTE_SPECIAL_SET)
					dac_mute(0,1);
					HAUDIO_MUTE_OFF();
				#endif
				#if defined(Tuobu_B324)
					idle_disp_info(DISP_TYPE_RTC);
	            #else
				#if !defined(_DISABLE_POWER_LOGO_)
					idle_disp_info(DISP_TYPE_WELCOME);
			    #endif
				#endif

					power_on_flag = TRUE;
					power_off_flag = FALSE;
				#if defined(_DISABLE_POWER_LOGO_)
					timer_cnt = 1500;
				#else
					timer_cnt = 0;
				#endif

				#if !defined(_LIANKE_6803_POWER_)
					idle_power_on();
				#endif
				
			    #if defined(_ENABLE_SUB_DETECT_)
			    	SYS_SUB_ON();
			    #endif

				#if defined(_ENABLE_POWER_PIN_)
				#if !defined(_ENABLE_TIME_CLEAN_)
				#if !defined(_XM007_TIME_CLEAN_)
				#if !defined(_WLGK_POWER_FLASH_)
					poweron_io_output(1);
				#endif
				#endif
				#endif
				#endif

				#if defined(_ENABLE_TIME_CLEAN_)||defined(_XM007_TIME_CLEAN_)
					global.clean_time = TRUE;
					global.logo_time = 0;
				#endif

				#if defined(PLANE_LIGHT_FUNC)
                	SYS_POWOR_LED_OFF();
                #endif

				#if defined(_ENABLE_IPHONE_CHARGE_)
					OSTimeDly(100);
				#endif

				#ifdef SUPPORT_CALL_NAMES
					if(play_once_flag == TRUE)
					{
    					play_once_flag = FALSE;
                        play_song_flag =TRUE;
    					OSTimeDly(100);//延时1S 要不然2313初始化不成功
    					setup.aud_vol =(AUDIO_VOLUME_MAX-15);
    					set_cur_vol(get_cur_vol());
    					audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(),FALSE);
    					audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_ON, TRUE);
    					audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
                        OSTimeDly(20);
    					sys_mute_ctrl(ENABLE,1);
                        OSTimeDly(20);
                        powerup_flag=FALSE;
    					tone_play_by_name(IDLE_TASK_NAME,1,BPF_TUOBU_MP3);
    					setup.aud_vol =AUDIO_VOLUME_DEFAULT;
    					set_cur_vol(get_cur_vol());
    					audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(),FALSE);
					}
				#endif
				}
				break;
		#ifdef SUPPORT_CALL_NAMES
			case MSG_IDLE_PLAY:

                idle_power_on();
			#if defined(Tuobu_B324)
				idle_disp_info(DISP_TYPE_RTC);
	        #else
				idle_disp_info(DISP_TYPE_WELCOME);
			#endif
                if(play_once_flag == TRUE)
                {
    				play_once_flag = FALSE;
                    play_song_flag =TRUE;
                    OSTimeDly(100);//延时1S 要不然2313初始化不成功
                    setup.aud_vol =(AUDIO_VOLUME_MAX-15);
                    set_cur_vol(get_cur_vol());
                    audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(),FALSE);
                    audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_ON, TRUE);
                    audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
                    OSTimeDly(20);
                    sys_mute_ctrl(ENABLE,1);
                    OSTimeDly(20);
                    powerup_flag=FALSE;
                    tone_play_by_name(IDLE_TASK_NAME,1,BPF_TUOBU_MP3);
                    play_end_flag =TRUE;
                    setup.aud_vol =AUDIO_VOLUME_DEFAULT;
                    set_cur_vol(get_cur_vol());
                    audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(),FALSE);
                }
				break;
			#endif

			case MSG_IDLE_TIMER:
				if((power_on_flag)
			#ifdef SUPPORT_CALL_NAMES
				&&(powerup_flag==TRUE)
			#endif
                )
				{
				#if defined(_SPECIAL_LOGO_TIMER)
					if((++timer_cnt) >= (_SPECIAL_LOGO_TIMER/IDLE_POLLING_TIME))
				#elif defined(_EVB_USB_DELAY_)
				#if defined(_JTI_USB_DELAY_)
					if((++timer_cnt) >= (2300/IDLE_POLLING_TIME))
				#else
					if((++timer_cnt) >= (2500/IDLE_POLLING_TIME))
				#endif
				#elif defined(_ENABLE_RADIO_LOGO_TIME_)
					if((++timer_cnt) >= (1200/IDLE_POLLING_TIME))
				#else
					if((++timer_cnt) >= (1500/IDLE_POLLING_TIME))
				#endif
					{
						idle_puts("MSG_IDLE_TIMER----111----\n");
					#if defined(_TEMP_WARM_DISPLAY_)
						global.temp_power_flag = TRUE;
					#endif
					
					#if defined(_POWER_OFF_BT_DEBUG_)
					    global.bt_flag = TRUE;
						global.phone_time = 0;
					#endif
					
					#if defined(_ENABLE_TEMP_ICON_DISPLAY_)
						ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_TEMP);
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_WARM);
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SET);
					#endif
					
				    #if defined(_ENABLE_TEMP_PWM_)
						if(setup.warm_power)
						{
							PWM0_init(setup.temp_set+1);
						}
				    #endif

				    #if defined(_AUDIO_DRIVER_AP3768_)
						audio_drv_init();
						global.power_mode_flag = FALSE;
				    #endif
						os_taskq_post_event(MAIN_TASK_NAME, 1, SYS_EVENT_POWER);
						power_on_flag = FALSE;
					}
				
				}
			#ifdef SUPPORT_CALL_NAMES
				else if((powerup_flag==TRUE)&&(play_end_flag))
				{
                    play_end_flag =FALSE;
                    AUDIO_MUTE_ON();
                    idle_power_off();
                	idle_disp_info(DISP_TYPE_RTC);
				}
			#endif
				if(power_off_flag)
				{
				#if defined(_XC3318_POWER_FUNTION_)
    				idle_power_off();
				#endif

				#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
				#if !defined(_DISABLE_SUB_OFF_)
					AMP_CONTROL_OFF();
				#endif
				#endif
				#if defined(_ENABLE_USB_SAVE_)
				#if defined(_WJ361_USB_DELAY_)||defined(_EVB_USB_DELAY_)
					if((++timer_cnt) >= (1500/IDLE_POLLING_TIME))
				#else
					if((++timer_cnt) >= (2500/IDLE_POLLING_TIME))
				#endif
				#elif defined(_SPECIAL_POWER_OFF_TIMER)
					if((++timer_cnt) >= (_SPECIAL_POWER_OFF_TIMER/IDLE_POLLING_TIME))
				#elif defined(_POWER_OFF_SUB_)
					if((++timer_cnt) >= (2000/IDLE_POLLING_TIME))
				#elif defined(_XM007_POWER_DISPLAY_)
					if((++timer_cnt) >= (1500/IDLE_POLLING_TIME))
				#else
					if((++timer_cnt) >= (1000/IDLE_POLLING_TIME))
				#endif
					{
						idle_puts("MSG_IDLE_TIMER----222----\n");
						
    			    #if defined(_LIANKE_6803_POWER_)
    			    	Color6803Refresh(31, 31, 31);
    			    #endif
					
					#if defined(_ENABLE_SUB_DETECT_)
						SYS_SUB_OFF();
						global.sub_flg = FALSE;
					#endif

				    #if defined(_TEMP_WARM_DISPLAY_)
				    	global.temp_power_off = TRUE;
				    #endif

					#if !defined(_ENABLE_PIN_CONTROL_)
					#if defined(_POWER_OFF_FUNTION_)
				    #if defined(_ENABLE_BEEP_)
				    	set_panel_pwm(0, 0, 0);
				    #endif
					#else
					#if !defined(_XC3318_POWER_FUNTION_)
						idle_power_off();
					#endif
					#endif
					#endif
					
					#if defined(_ENABLE_POWER_PIN_)
					#if !defined(_XM007_POWER_DISPLAY_)
						poweron_io_output(0);
					#endif
					#endif

					#if defined(_ENABLE_USB_BATT_CONTROL_)
						USB_CONTROL_OFF();
					#endif
					
					#if defined(_SL6812_ILL_LIGHT_)
						//ILL_LIGHT_OFF();
					#endif

					#if defined(_FY700B_ICON_CLOSE_)||defined(_ENABLE_DELAY_POWER_)||defined(_XM007_POWER_DISPLAY_)||defined(_WLGK_POWER_OFF_DELAY_)
     					global.sys_power = FALSE;
					#endif

					#if defined(_POWER_OFF_DISABLE_TIME_)||defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
					#if defined(_FY5517_POWER_OFF_TIME_)
						global.clock_flag = TRUE;
						idle_disp_info(DISP_TYPE_RTC);
					#else
						idle_disp_info(DISP_TYPE_CLEAN);
					#endif
					#elif defined(_ENABLE_PASSWORD_SET_)
						if(setup.pwd_set)
						{
							idle_disp_info(DISP_TYPE_CLEAN);
						}
						else
						{
							idle_disp_info(DISP_TYPE_RTC);
						}
					#else
						idle_disp_info(DISP_TYPE_RTC);
					#endif
					
				    #if defined(_SUPPORT_30P60_LIGHT_)
						extern uint8 light_state;
				    	light_state = 10;
					#if defined(_ENABLE_LIGHT_SHADE_)
						setup.save_light = 1;
						LightDriverIrCmd(0x00,0,0,0);
					#else
				    	LightDriverIrCmd(0x00);
					#endif
				    #endif

						power_off_flag = FALSE;
					#if defined( _SUPPORT_LCM_1618_DRIVE )
						Clear_1_2_page_disp_flag=0;
						Clear_page_disp_flag=1;
					#endif
					
				    #if defined(_YH1701_SPECIAL_FUNTION_)
				    	OSTimeDly(5);
				    	SYS_MCU_OFF();
				    #endif
					
					#if defined(_ENABLE_POWER_OFF_CURRENT_)
				    	set_sys_freq(BT_RCCL_SYS_Hz);
					#endif
					}
				}

		    #if defined(_ENABLE_BT_ONOFF_DISPLAY_)
		    	if(global.bt_display)
		    	{
		    		global.bt_disp_time++;
					if(global.bt_disp_time >= (3000/IDLE_POLLING_TIME))
					{
						SYS_POW_OFF();
						global.bt_disp_time = 0;
						global.bt_display = FALSE;
						Color6803Refresh(31, 31, 31);
						idle_disp_info(DISP_TYPE_RTC);
					}
		    	}
		    #endif

				break;

			default:
				break;
		}
	}
}

/*----------------------------------------------------------------------------*/
/**@brief  IDLE 任务创建
   @param  priv：任务间参数传递指针
   @return 无
   @note   static void idle_task_init(void *priv)
*/
/*----------------------------------------------------------------------------*/
static void idle_task_init(void *priv)
{
    u32 err;
	ew_puts("idle_task_init\n");
	err = os_task_create(idle_task,
			(void*)0,
			TaskIdlePrio,
			10
		#if OS_TIME_SLICE_EN > 0
			,1
		#endif
			,IDLE_TASK_NAME);

    if(OS_NO_ERR == err)
    {
        key_msg_register(IDLE_TASK_NAME, KEY_PROJECT_MAX);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  IDLE 任务删除退出
   @param  无
   @return 无
   @note   static void idle_task_exit(void)
*/
/*----------------------------------------------------------------------------*/
static void idle_task_exit(void)
{
	key_msg_register(NULL, KEY_PROJECT_MAX);

	if(os_task_del_req(IDLE_TASK_NAME) != OS_TASK_NOT_EXIST)
	{
		os_taskq_post_event(IDLE_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
		do{
			OSTimeDly(1);
		} while(os_task_del_req(IDLE_TASK_NAME) != OS_TASK_NOT_EXIST);
		idle_puts("del_idle_task: succeed\n");
	#if defined(TD_ADD_NEW_APP)
		power_up_flag = TRUE;
	#endif
	}
}

/*----------------------------------------------------------------------------*/
/**@brief  IDLE 任务信息
   @note   const struct task_info idle_task_info
*/
/*----------------------------------------------------------------------------*/
TASK_REGISTER(idle_task_info) = {
	.name = IDLE_TASK_NAME,
	.init = idle_task_init,
	.exit = idle_task_exit,
};

