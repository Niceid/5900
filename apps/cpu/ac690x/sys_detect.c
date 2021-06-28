#include "sdk_cfg.h"
#include "sys_detect.h"
#include "detect.h"
#include "key_drv.h"
#include "file_operate/file_op.h"
#include "dac/dac.h"
#include "rtc.h"
#include "disp.h"
#include "bt.h"
#include "dev_manage/drv_manage.h"
#include "common/common.h"
#if defined(_SUPPORT_LCM_1618_DRIVE)
#include "Lcm_1618_drive.h"
#endif
#if defined(_SUPPORT_DISC_)
#include "disc_drv/disc_drv.h"
#endif
#if defined(_USER_TFT_DISP_)
#include "AMT630A\AMT630A.h"
#endif
#define SYS_DETECT_DBG
#ifdef SYS_DETECT_DBG
    #define sys_det_puts           ew_puts
    #define sys_det_printf         ew_printf
#else
    #define sys_det_puts(...)
    #define sys_det_printf(...)
#endif


u8 soft_poweroff_cnt = 0;
u8 going_to_pwr_off = 0;
u8 bd_work_mode;

#if defined(_ENABLE_IPHONE_CHARGE_)
static void charge_init(void)
{
	SYS_CHARGE_INIT();
	SYS_CHARGE_OFF();
}
#endif

#if defined(_SUPPORT_BACKLIGHT_CONTROL_)
static void backlight_init(void)
{
	SYS_BACKLIGHT_INIT();
	SYS_BACKLIGHT_OFF();
}
#endif

#if defined(_YH1701_SPECIAL_FUNTION_)
static void mcu_init(void)
{
	SYS_MCU_INIT();
	SYS_MCU_ON();
}
#endif

#if defined(_ENABLE_USB_CONTROL_)
static void usb_init(void)
{
	SYS_USB_INIT();
	SYS_USB_OFF();
}
#endif

#if defined(_SH1302_AKIN_OUTPUT_PIN)
static void akinoutput_control_init(void)
{
    AKIN_OUTPUT_INIT();
    AKIN_OUTPUT_OFF();
}	
#endif

#if defined(_ENABLE_OPTICAL_CONTROL_)
static void optical_control_init(void)
{
	SYS_OPTICAL_INIT();
	SYS_OPTICAL_OFF();
}
#endif

#if defined(_ENABLE_VOLUME_CONTROL_)
static void zone_control_init(void)
{
	SYS_ZONE1_INIT();
	SYS_ZONE1_ON();
	SYS_ZONE2_INIT();
	SYS_ZONE2_ON();
	SYS_ZONE3_INIT();
	SYS_ZONE3_ON();
	SYS_ZONE4_INIT();
	SYS_ZONE4_ON();
}
#endif

#if defined(_SUPPORT_SUB_)
static void sub_control_init(void)
{
	SYS_SUB_INIT();
	SYS_SUB_OFF();
}
#endif

#if defined(_VR_SCAN_IO_)
static void vol_init(void)
{
	VOL_POSITIVE_INIT();
	VOL_POSITIVE_IN();
	VOL_NEGATIVE_INIT();
	VOL_NEGATIVE_IN();
}
#endif

#if defined(_ENABLE_AMPLIFIER_CONTROL_)
static void amplifier_control_init(void)
{
	AMPLIFIER_INIT();
	AMPLIFIER_OUT();
	AMPLIFIER_LOW();
}
#endif

#if defined(_KEY_LIGHT_CONTROL_)
static void light_init(void)
{
	SYS_LIGHT_INIT();
	SYS_LIGHT_OFF();
}
#endif

#if defined(_ENABLE_SUB_DETECT_)
static void sub_init(void)
{
	SYS_SUB_INIT();
	SYS_SUB_OFF();
}
#endif

static void gpio_init(void)
{
	SYS_POW_INIT();
#if defined(_ENABLE_POWER_SYNCHRO_)
	SYS_POW_ON();
#else
	SYS_POW_OFF();
#endif

#if defined(_ENABLE_USB_CONTROL_)
	usb_init();
#endif

#if defined(_KLD_SYS5VPWR_SOFT_CTRL)
	SYS_5VPOW_INIT();
	SYS_5VPOW_OFF();
#endif
#if defined(_SUPPORT_USB_DET)
	USB_PWR_INIT();
	USB_PWR_H();
#endif

	AUDIO_MUTE_INIT();
	AUDIO_MUTE_ON();
#if defined(_ENABLE_MUTE_FUNTION_)
	FEIYIN_MUTE_ON();
#endif

#if defined(_ENABLE_TDA7265_MUTE_)
	TDA7265_MUTE_INIT();
	TDA7265_MUTE_OFF();
#endif

#if defined(_ENABLE_RCA_MUTE_)
    RCA_MUTE_INIT();
    RCA_MUTE_ON();
#if defined(_DSP_RCA_MUTE_)
	DSP_RCA_MUTE_ON();
#endif
#endif

#if defined(_ENABLE_AMPLIFIER_CONTROL_)
	amplifier_control_init();
#endif

#if defined(_WLS_MUTE_FUNTION_)
    AUDIO_MUTE1_INIT();
    AUDIO_MUTE1_ON();
#endif

#if defined(_SPECIAL_2313_CONTROL_)
	I2C_SCL_OUT1();
	I2C_SDA_OUT1();
#endif

#if defined(_SUPPORT_SUBWOOFER2_)
	SYS_SUB_INIT();
	SYS_SUB_OFF();
#endif

#if defined(_ENABLE_SUB_DETECT_)
	sub_init();
#endif

#if defined(_KEY_LIGHT_CONTROL_)
	light_init();
#endif

#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_RADIO_LIGHT_)||defined(_ENABLE_AMP_LIGHT_)||defined(_KLD3000_RADIO_LIGHT_)
   	AMP_CONTROL_INIT();
    AMP_CONTROL_OFF();
#endif

#if defined(_SUPPORT_SUBWOOFER3_)
	SYS_SUB2_INIT();
	SYS_SUB2_OFF();
#endif

#if defined(_ENABLE_INH_CONTROL_)
    LCD_INH_INIT();
    LCD_INH_OFF();
#endif

#if defined(_ENABLE_USB_BATT_CONTROL_)
    USB_CONTROL_INIT();
    USB_CONTROL_OFF();
#endif

#if defined(_SUPPORT_ACC_)
	ACC_INIT();
#endif

#if defined(_SL6812_ILL_LIGHT_)
	ILL_DETECT_INIT();
	ILL_DETECT_IN();
	ILL_LIGHT_INIT();
#if defined(_SL6812A_LIGHT_DETECT_)
	if(ILL_DETECT_CHECK())
	{
		ILL_LIGHT_ON();
	}
	else
	{
		ILL_LIGHT_OFF();
	}
#else
	ILL_LIGHT_OFF();
#endif
#endif

#if defined(_ENABLE_TEL_MUTE_DETECT_)
	BT_TEL_MUTE_INIT();
	BT_TEL_MUTE_IN();
#endif

#if defined(_SUPPORT_AUX_DET)||defined(_SL6812_AUX_DETECT_)
	AUX_DET_INIT();
	AUX_DET_IN();
#endif

#if defined(_SUPPORT_TAPE_)
	TAPE_DETCET_INIT();
	TAPE_DETCET_IN();

	TAPE_POWER_INIT();
	TAPE_POWER_OFF();

	TAPE_AB_INIT();
	TAPE_AB_IN();
#endif
#if defined(_SUPPORT_DTH_)
	DTH_INIT();
#endif

#if defined(_SUPPORT_MIC_OK_)||defined(_SUPPORT_MIC_AUX_)
	MIC_INIT();
	MIC_IN();
#endif

#if defined(_SUPPORT_LCM_DRIVE)||defined(_SUPPORT_LCM_1618_DRIVE)
	Lcm_Gpio_Init();
#endif

#if defined(_ACC_POWER_CONTROL_)
	ACC_POWER_INIT();
	ACC_POWER_ON();
#endif

#if defined(_ENABLE_PWR_PIN_)
	ACC_POW_INIT();
	ACC_POW_ON();

	ACC_POW1_INIT();
	ACC_POW1_ON();
#endif

#if defined(_SUPPORT_BACKLIGHT_CONTROL_)
	SYS_BACKLIGHT_INIT();
	SYS_BACKLIGHT_ON();
#endif

#if defined(_SUPPORT_DISC_)
#if defined(ACC_POW_GPIO)
	ACC_POW_INIT(); ACC_POW_ON();
#endif

	DISC_POWER_INIT();	DISC_POWER_OFF();
	DISC_RESET_INIT();	DISC_RESET_ON();

	DISC_SWA_INIT();	DISC_SWA_IN();
	DISC_SWB_INIT();	DISC_SWB_IN();
	DISC_SWC_INIT();	DISC_SWC_IN();
	DISC_OPEN_INIT();	DISC_OPEN_OFF();
	DISC_CLOSE_INIT();	DISC_CLOSE_OFF();

	DETECT_USB_INIT();	DETECT_USB_IN();
	DETECT_SD_INIT();	DETECT_SD_IN();
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_ENABLE_ALARM_CLOCK_)||defined(_ENABLE_BEEP_)
#if defined(_SUPPORT_PANEL_74HC595)
	LATCH_CLK_INIT();	LATCH_CLK_OUT();  	  LATCH_CLK_HIGH();
	LATCH_DATA_INIT();	LATCH_DATA_OUT();	  LATCH_DATA_HIGH();
	LATCH_SCLK_INIT();	LATCH_SCLK_OUT();	  LATCH_SCLK_HIGH();
#else
#if !defined(_ENABLE_CHRAMATIC_CAMP_SHADE_)
	SYS_RED_INIT();   SYS_RED_OUT();   SYS_RED_OFF();
#if !defined(_SUPPORT_THREE_LIGHT_)
	SYS_GREEN_INIT(); SYS_GREEN_OUT(); SYS_GREEN_OFF();
#endif
	SYS_BLUE_INIT();  SYS_BLUE_OUT();  SYS_BLUE_OFF();
#endif
#endif
#endif

#if defined(SUPPORT_LED_ADJUST)
	BACK_LED_LOW_INIT(); BACK_LED_LOW_OUT(); BACK_LED_LOW_HIGH();
	BACK_LED_MIN_INIT(); BACK_LED_MIN_OUT(); BACK_LED_MIN_HIGH();
#endif

#if defined(_ADJUST_A_B_SPK_)
	AUDIO_A_SPK_INIT(); AUDIO_A_SPK_OUT(); AUDIO_A_SPK_LOW();
	AUDIO_B_SPK_INIT(); AUDIO_B_SPK_OUT(); AUDIO_B_SPK_LOW();
#endif

#if defined(_MR889_SPECAIL_KEY_)
	AUDIO_CONTROL_SPK_INIT(); AUDIO_CONTROL_SPK_OUT(); AUDIO_CONTROL_SPK_HIGH();
#endif

#if defined(SET_BEEP_FUNCV)
	SYS_BEEP_INIT();SYS_BEEP_OUT();	SYS_BEEP_LOW();
#endif
#if defined(PLANE_LIGHT_FUNC)
    ILL_LIGHT_DET_INIT();ILL_LIGHT_DET_IN();
    KB_LIGHT_INIT();KB_LIGHT_OUT();KB_LIGHT_LOW();
    SYS_POWOR_LED_INIT();SYS_POWOR_LED_OUT();SYS_POWOR_LED_LOW();
#endif
#if defined(ENABLE_PLANE_LIGHT_CONTROL_)
    ILL_LIGHT_DET_INIT();ILL_LIGHT_DET_IN();
    KB_LIGHT_INIT();KB_LIGHT_OUT();KB_LIGHT_LOW();
#endif
#if defined(_SUPPORT_LIGHT_DETECT1_)
    ILL_LIGHT_DET_INIT();ILL_LIGHT_DET_IN();
#endif

#if defined(_ENABLE_DLASHING_LIGHT_)
	SYS_DLASHING_INIT();
	SYS_DLASHING_OFF();
#endif

#if defined(_SUPPORT_30P60_LIGHT_)
	SYS_30P60_LIGHT_INIT();
	SYS_30P60_LIGHT_OUT();
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief
   @param  void
   @return void
   @note   void sys_init(void)
*/
/*----------------------------------------------------------------------------*/
void sys_init(void)
{
	puts("-------sys_init_start-------\n");

#if defined(_YH1701_SPECIAL_FUNTION_)
	mcu_init();
#endif

#if defined(_SUPPORT_TOUCH_KEY_)
	TOUCH_CLK_INIT();
	TOUCH_DATA_INIT();
#endif

	gpio_init();

#if defined(_ENABLE_BUG_DETECT_)
	BUG_INIT();
	BUG_IN();
#endif

#if defined(_VR_SCAN_IO_)
	vol_init();
#endif

#if defined(_USER_TFT_DISP_)
    tft_disp_init();
#endif

#if RTC_CLK_EN
    irtc_hw_init();
#endif
#if UART_UPDATA_EN
   	uart_update_init();
#endif
#if !defined(use_usb_as_uart)
#if !defined(_SUPPORT_DISC_)
	usb_2_io();
#endif
#endif
	key_init();

#if defined(_ENABLE_IPHONE_CHARGE_)
	charge_init();
#endif

#if defined(_SUPPORT_BACKLIGHT_CONTROL_)
	backlight_init();
#endif

#if defined(_SUPPORT_DISC_)
	disc_drv_init();
#endif

#if defined(_ENABLE_VOLUME_CONTROL_)
	zone_control_init();
#endif

#if defined(_SUPPORT_SUB_)
	sub_control_init();
#endif

#if defined(_SH1302_AKIN_OUTPUT_PIN)
    akinoutput_control_init();
#endif

#if defined(_ENABLE_OPTICAL_CONTROL_)
	optical_control_init();
#endif
	detect_init_api();

#if defined(_SUPPORT_RADIO_)
	extern void tuner_drv_init(void);
	tuner_drv_init();
#endif

	soft_poweroff_cnt = 0;
    going_to_pwr_off = 0;
#if(BT_MODE!=NORMAL_MODE)
    bd_work_mode = BD_PAGE_MODE;
#else
    bd_work_mode = BD_CONN_MODE;
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief  软开关控制
   @param  ctl :power_on or power_off
   @return null
   @note   void soft_power_ctl(u8 ctl)
*/
/*----------------------------------------------------------------------------*/
void soft_power_ctl(u8 ctl)
{
    if(PWR_ON == ctl)
    {

    }
    else
    {
		sys_det_printf("soft off\n");
        soft_poweroff();
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   power off 检测延时关机
   @param   null
   @return  null
   @note    void check_power_off(void)
*/
/*----------------------------------------------------------------------------*/
void check_power_off(void)
{
    if(soft_poweroff_cnt)
    {
        if(--soft_poweroff_cnt == 0)
        {
            soft_power_ctl(PWR_OFF);
        }
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   关闭IO长按复位功能
   @param   null
   @return  null
   @note    void clr_PINR_ctl(void)
*/
/*----------------------------------------------------------------------------*/
void clr_PINR_ctl(void)
{
	rtc_module_port_4s_reset(PORTR2 , 0 , 0 );
}

/*----------------------------------------------------------------------------*/
/**@brief  恢复USB口为普通IO口
   @param  USB_IO_CON
   BIT(15)-BIT(8):  --   --   --   --  DMIN DPIN  --    --
   BIT(7) -BIT(0): DMPU DPPU DMPD DPPD DMIE DPIE DMOUT DPOUT
   @return void
   @note   void usb_2_io(void)

*/
/*----------------------------------------------------------------------------*/
void usb_2_io(void)
{
//    sys_det_puts("USB_2_IO\n");
    JL_USB->CON0 |= BIT(0)|BIT(1);//PHY_ON(enable)  IO_MODE(enable)
    JL_USB->IO_CON = (BIT(3)|BIT(2));//IO输入
}

/*----------------------------------------------------------------------------*/
/**@brief   watch dog timer config
   @param   void
   @return  void
   @note
 */
/*----------------------------------------------------------------------------*/
/*
cfg:1010:1s  1011:2s    1100:4s
    1101:8s  1110:16s   1111:32s
*/
void open_wdt(u8 cfg)//开看门狗
{
    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
    sys_det_puts("open_wdt\n");
    JL_CRC->REG  = 0X6EA5;
    JL_SYSTEM->WDT_CON  = cfg&0x0f;
    JL_SYSTEM->WDT_CON &= ~BIT(5);
    JL_SYSTEM->WDT_CON |=  BIT(4);
    JL_CRC->REG  = 0XAAAA;
    OS_EXIT_CRITICAL();
}

void close_wdt(void)//关看门狗
{
    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
	sys_det_puts("close_wdt\n");
    JL_CRC->REG = 0x6EA5;
    JL_SYSTEM->WDT_CON &= ~BIT(4);
    JL_CRC->REG = 0;
    OS_EXIT_CRITICAL();
}

void clear_wdt(void)//清看门狗
{
    //sys_det_puts(" clr_wdt ");
    JL_SYSTEM->WDT_CON |= BIT(6);
}

/*----------------------------------------------------------------------------*/
/**@brief  	sd data multiplex
   @param   void
   @return  void
   @note
 */
/*----------------------------------------------------------------------------*/
#if SD_DADA_MULT
u16 adc_value_PA1;
u8 g_aux_online_flag;
extern u32 get_sd0_dev_busy_status_api(void);
extern void close_sd0_controller(void);
extern void release_sd0_controller(void);
void sd_data_multiplex(void)
{
    u16 res = 0;
    static u8 dev_online_cnt,dev_offline_cnt,adc_timeout_cnt,adc_err_cnt;

    //if(task_start_complete == 0)
        //return;

    if(!get_sd0_dev_busy_status_api())
    {
		adc_busy_flag = 1;

		//CPU_SR_ALLOC();
        //OS_ENTER_CRITICAL();

        //关闭 sd controller
        close_sd0_controller();

        //设置PA1 IO
        JL_PORTA->DIR |= BIT(1);
        JL_PORTA->HD &= ~BIT(1);
        JL_PORTA->PU |= BIT(1);
        JL_PORTA->PD &= ~BIT(1);
        JL_PORTA->DIE &= ~BIT(1);

        //启动ADC
		//adc_ch_sel(AD_KEY_PA1);
		res = adc_res_api(AD_KEY_PA1);
		res = adc_res_api(get_next_adc_ch());

        //恢复 sd io
        JL_PORTA->DIR &= ~BIT(1);
        JL_PORTA->HD |= BIT(1);
        JL_PORTA->PU |= BIT(1);
        JL_PORTA->DIE |= BIT(1);

        //恢复 sd controller
        release_sd0_controller();
        //OS_EXIT_CRITICAL();
		adc_busy_flag = 0;
    }
    else {
        putchar('B');
        return;
    }

    adc_value_PA1 = (res*33)/0x3ffL;
    if(++adc_timeout_cnt > 20)
    {
        //sys_det_printf("online_cnt:%d     offline_cnt:%d       err_cnt:%d\n"   ,dev_online_cnt,dev_offline_cnt,adc_err_cnt);
        if(dev_online_cnt > 13) {
            //ew_puts("aux_online\n");
            g_aux_online_flag = 1;
        }
        else if(dev_offline_cnt > 13) {
            //ew_puts("*************aux_offline*************\n");
            g_aux_online_flag = 0;
        }
        dev_online_cnt = 0;
        adc_timeout_cnt = 0;
        dev_offline_cnt = 0;
        adc_err_cnt = 0;
    }

    if((adc_value_PA1>27)&&(adc_value_PA1<31)) {
        dev_online_cnt++;
    }
    else if(adc_value_PA1 >= 31) {
        dev_offline_cnt++;
    }
    else {
        adc_err_cnt++;
        sys_det_printf("\n========err======= %d   %d  \n",res ,adc_value_PA1);
    }
    //sys_det_printf("adc_val:%d,online_cnt:%d,offline_cnt:%d\n",adc_value_PA1,dev_online_cnt,dev_offline_cnt);
    sys_det_printf("[%d]",adc_value_PA1);
}
#endif //SD_DADA_MULT

/*----------------------------------------------------------------------------*/
/**@brief  	pwm4_cfg
   @param	toggle:switch
  			pre_div:0-2-4-8-16-32-64-128(div = 0~7)
			duty:duty/16(dety = 0~15)
   @return  void
   @note    pwm4 clk = lsb_clk/16div(default:48M/16 = 3M)
 */
/*----------------------------------------------------------------------------*/
void pwm4_cfg(u8 toggle,u8 pre_div,u8 duty)
{
	u8 pwm4_scaler;
	u8 pwm4_duty;
	if(toggle) {
		pwm4_scaler = pre_div & 0x7;
		pwm4_duty = duty & 0xF;
    	JL_PWM4->CON = (pwm4_scaler << 4) | pwm4_duty;
		JL_IOMAP->CON1 |= BIT(11)|BIT(12)|BIT(13);
    	//JL_PORTA->DIR &= ~BIT(2);JL_PORTA->PU |= BIT(2);JL_PORTA->PD |= BIT(2);
	} else {
    	JL_PWM4->CON = 0;
		JL_IOMAP->CON1 &= ~(BIT(11)|BIT(12)|BIT(13));
	}
}
extern u32 get_sd0_dev_busy_status_api(void);
extern u32 get_sd1_dev_busy_status_api(void);
extern void close_sd0_controller(void);
extern void close_sd1_controller(void);
extern void release_sd0_controller(void);
extern void release_sd1_controller(void);
extern s32 sd0_mult_det_api(void);
extern s32 sd1_mult_det_api(void);
extern void usb_host_mult_det_api(void);

/* extern void resume_sd0_io(void); */
extern u8 get_io_status(void);


static void suspend_sd_io(void)
{
#if USB_SD0_MULT_EN	//sd0a
	JL_PORTA->DIR|= (BIT(11) | BIT(12));
	JL_PORTA->PU &=~(BIT(11) | BIT(12));
	JL_PORTA->PD &=~(BIT(11) | BIT(12));
#else				//sd1b
	JL_PORTC->DIR|= (BIT(3) | BIT(4));
	JL_PORTC->PU &=~(BIT(3) | BIT(4));
	JL_PORTC->PD &=~(BIT(3) | BIT(4));
#endif
}

void usb_sd_detect_mult_api(void)
{
#if USB_SD1_MULT_EN ||USB_SD1_MULT_EN
	u32 sd_online;
	u32 usb_online;
#endif
#if USB_SD0_MULT_EN
	sd0_mult_det_api();
	sd0->dev_io->ioctrl(&sd_online, DEV_GET_STATUS);
	if(sd_online == 0)//sd_offline
	{
		suspend_sd_io();
		usb_host_mult_det_api();
	}
	else//sd_online
	{
		if(get_io_status() == IO2SD0)//sd_is_using
		{
			if(!get_sd0_dev_busy_status_api())//free
			{
				//1?±? sd controller
				close_sd0_controller();

				delay(100);//wait io stably
				usb_host_mult_det_api();

				//???′ sd controller
				release_sd0_controller();
			}
		}
		else
		{
			usb_host_mult_det_api();
		}
	}

#endif

#if USB_SD1_MULT_EN
	sd1_mult_det_api();
	sd1->dev_io->ioctrl(&sd_online, DEV_GET_STATUS);
	if(sd_online == 0)//sd_offline
	{
		suspend_sd_io();
		usb_host_mult_det_api();
	}
	else//sd_online
	{
		if(get_io_status() == IO2SD0)//sd_is_using
		{
			if(!get_sd1_dev_busy_status_api())//free
			{
				//1?±? sd controller
				close_sd1_controller();

				delay(100);//wait io stably
				usb_host_mult_det_api();

				//???′ sd controller
				release_sd1_controller();
			}
		}
		else
		{
			usb_host_mult_det_api();
		}
	}
#endif
}

uint8 task_msg_default(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{
		if(get_msg_others(type))
		{
			os_taskq_post_msg(MAIN_TASK_NAME, 2, msg, type);
			return TRUE;
		}
	}
	return FALSE;
}
#if defined(SUPPORT_CALL_NAMES)
extern uint8 play_once_flag;
#endif

void power_up_task(void)
{
	sys_det_puts("power_up_task enter\n");

#if BT_BACKMODE
    bt_task_background_init();
#endif
	disp_task_init();
    task_switch(IDLE_TASK_NAME, 0,SWITCH_SPEC_TASK);
#if defined(SUPPORT_CALL_NAMES)
	play_once_flag = TRUE;
#endif
#if defined(_CHANGE_ACC_POWER_ON_FLOWT)
	extern uint8 GetAccStable(void);
	if(!GetAccStable())
	{
		sys_det_puts("power_up_task acc ready off\n");
		os_taskq_post_msg(IDLE_TASK_NAME, 1, SYS_EVENT_ACC_OFF);
		os_time_dly(5);
	}
	else
	{
		os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_DETECT);
		os_time_dly(5);
	}
#else
	os_taskq_post_msg(IDLE_TASK_NAME, 1, MSG_IDLE_DETECT);
	os_time_dly(5);
#endif

	sys_det_puts("power_up_task exit\n");
}

