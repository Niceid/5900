/***********************************Jieli tech************************************************
  File : timer.c
  By   : Juntham
  date : 2014-07-04 11:21
********************************************************************************************/
#include "sdk_cfg.h"
#include "timer.h"
#include "common/jiffies.h"
#include "key_drv.h"
#include "clock_api.h"
#include "rtc/rtc.h"
#include "sys_detect.h"
#include "key_drv.h"
#include "irq_api.h"
#include "clock.h"
#include "setup.h"
#include "dac/dac.h"
#include "bluetooth/avctp_user.h"
#if defined(_SUPPORT_DISC_)
#include "disc_drv/disc_drv.h"
#endif


#define TIMER2_FOR_TIME_DATA	//TIME2 用来走时


#if defined(TD_ADD_ACCOFF_TIMEOUT_RESET)
extern u8 gAccOffFlag;
u8 accoffcnt=0;
#endif

#define TIMER_DEBUG
#ifdef TIMER_DEBUG
#define timer_printf 	ew_printf
#define timer_puts 		ew_puts
#else
#define timer_printf(...)
#define timer_puts(...)
#endif

u8 ghalf_sec=false;
extern void sys_timer_schedule();

__timer_handle *timer0_hl = NULL;
__timer_handle  *timer2_hl= NULL;

__timer_handle  *timer3_hl= NULL;

u16 t0_cnt1;

volatile u32 g_rand_seed = 0;


typedef void (*timer_func)(void *);

int32 timer_reg_func(timer_callback func, void *param, uint32 time)
{
	return timer_reg_isr_fun(timer0_hl, time/2, func, param);
}

int32 timer_unreg_func(timer_callback func)
{
	return timer_unreg_isr_fun(timer0_hl, func);
}

#ifdef	TD_ADD_SPP
extern u8 gRefresh_Audio;
extern u8  Audio_info_compare(void);
extern u8 a2dp_get_status(void);
u16 t3_cnt1=0;
u8 send_flag=0;
u8 send_count=0;
extern u8 gAppRefreshTime;
#endif
#ifdef	TD_ADD_NEW_APP
extern u8 audio_vol_compare(void);
#endif
#ifdef SUPPORT_CALL_NAMES
u8 playsong_cnt=0;
extern uint8 powerup_flag;
extern uint8 play_song_flag;
#endif
#ifdef	UART_Servo_Pop
u16 t2_cnt1=0;
extern void DABDriverServer(void);
#endif

#if defined(_USER_TFT_DISP_)
u16 t3_cnt3=0;
extern void TFTDriverServer(void);
#endif

#if defined(_SUPPORT_CAN_UART_)
u16 t4_cnt4=0;
extern void McuDriverServer(void);
#endif

#if defined(_AUDIO_DRIVER_AP3768_)
u16 t5_cnt5=0;
extern void AudioDriverServer(void);
#endif

#if defined(SYS_MODE_CHANG_NOPP)
u8 app_mode_enable_COUNT=0;
#endif

#if defined(_ENABLE_BT_AUTO_CONNECT_)
u8 bt_auto_connect=0;
u8 bt_auto_time=0;
static uint8 bt_connect2(void)
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

#if defined(_ENABLE_A2DP_TIME_)
static uint8 bt_connect3(void)
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

//double test3;
static void timer0_isr_callback(void)
{
	g_rand_seed++; //随机数种子

	t0_cnt1++;
	
#if defined(_ENABLE_POWER_PIN_)
	if(!global.sys_power)
	{
	#if defined(_SPECIAL_POWER_LOGO_)
		if(!global.off_flag)
		{
			dac_ldo_set(LDO_1,0);
			dac_ldo_set(LDO_2,0);
			global.off_flag = FALSE;
		}
	#elif defined(_ENABLE_DELAY_POWER_)
		if(!ACC_CHECK())
		{
			dac_ldo_set(LDO_1,0);
			dac_ldo_set(LDO_2,0);
		}
	#elif defined(_XM007_SPECIAL_BACKLIGHT_)
		if(!global.off_flag)
		{
			dac_ldo_set(LDO_1,0);
			dac_ldo_set(LDO_2,0);
		}
	#else
		dac_ldo_set(LDO_1,0);
		dac_ldo_set(LDO_2,0);
	#endif
	
    #if defined(_SUPPORT_AMP_CONTROL_)
    	AMP_CONTROL_INIT();
    	AMP_CONTROL_OFF();
    #endif
	}
#endif
    
#if defined(_ENABLE_RADIO_TIME_)
	if((t0_cnt1 % 5) == 0)
	{
		OSTimeTick();
		sys_timer_schedule();
	}
#endif

#if defined(_ENABLE_AMPLIFIER_CONTROL_)
	if((t0_cnt1 % 2) == 0)//4MS
	{
		if(!ACC_CHECK())
		{
			AMPLIFIER_LOW();
		}
		else
		{
			if(global.sys_power)
			{
				AMPLIFIER_HIGH();
			}
		}
	}
#endif

#if defined(_SPECIAL_VR_)||defined(_ENABLE_RADIO_TIME_)
	if((t0_cnt1 % 10) == 0)
#else
	if((t0_cnt1 % 5) == 0)
#endif
	{
	#if !defined(_ENABLE_RADIO_TIME_)
		OSTimeTick();
		sys_timer_schedule();
	#endif

	#ifdef	TD_ADD_USER_UART
    	if(gRecvTime)
    	{
			gRecvTime --;
    	}
    	else
    	{
            gRecvCheckSum =0;
            gRecvStep =0;
    	}
	#endif

    #if defined(SYS_MODE_CHANG_NOPP)
    	if(global.app_mode_enable==0)
    	{
    		if(++app_mode_enable_COUNT>=110)//1100ms
    		{
    			app_mode_enable_COUNT=0;
    		 	global.app_mode_enable=1;
    		}
    	}
    #endif
	
    #if defined(_SUPPORT_DISC_)
    	disc_drv_process();
    #endif
	}

#if defined(_SPECIAL_VR_)||defined(_ENABLE_RADIO_TIME_)
	if(t0_cnt1 == 500)
#else
	if(t0_cnt1 == 250)
#endif
	{
		t0_cnt1 = 0;
        ghalf_sec =!ghalf_sec;
		
	#ifdef	TD_ADD_SPP
		if(gAppRefreshTime)
		{
			gAppRefreshTime --;
		}
	#endif
	#ifdef SUPPORT_CALL_NAMES
        if(play_song_flag==TRUE)
        {
			playsong_cnt++;
            if(playsong_cnt >40)
            {
	            playsong_cnt =0;
	            play_song_flag =FALSE;
				powerup_flag=TRUE;
			}
        }
        else
        {
            playsong_cnt =0;
        }
	#endif
	#if defined(TD_ADD_ACCOFF_TIMEOUT_RESET)
        if(gAccOffFlag ==TRUE)
        {
            accoffcnt++;
            if(accoffcnt>20)
            {
                accoffcnt =0;
	            gAccOffFlag =FALSE;
	            JL_POWER->CON |= BIT(4);
            }
        }
        else
        {
            accoffcnt =0;
        }
	#endif
	
    #if defined(_ENABLE_BT_AUTO_CONNECT_)
    	if(bt_auto_connect)
    	{
    		if(!bt_connect2())
    		{
    			bt_auto_time++;
    			printf("bt_auto_time:%d\n",bt_auto_time);
    			if(bt_auto_time == 40)
    			{
    				puts("bt_auto_time11111\n");
    				bt_auto_time = 0;
    				user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
    				user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE,0,NULL);
    				user_send_cmd_prepare(USER_CTRL_START_CONNECTION,0,NULL);
    				os_taskq_post_msg("btmsg", 1, MSG_BT_CONNECT_CTL);
    			}
    		}
    		else
    		{
    			bt_auto_connect = FALSE;
    			bt_auto_time = 0;
    		}
    	}
    #endif

	#if defined(_ENABLE_A2DP_TIME_)
		if(bt_connect3())
		{
			//ew_printf("a2dp_get_status():%d\n",a2dp_get_status());
			global.a2dp_count++;
			if(a2dp_get_status() == BT_MUSIC_STATUS_STARTING)
			{
				user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_GET_PLAY_TIME,0,NULL);
			}

			if(global.a2dp_count == 100)
			{
				global.a2dp_count = 40;
			}
		}
		else
		{
			global.a2dp_count = 0;
		}
	#endif

	#if defined(_DISABLE_BT_MODE_)
    	user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
    	user_send_cmd_prepare(USER_CTRL_WRITE_CONN_DISABLE,0,NULL);
		user_send_cmd_prepare(USER_CTRL_POWER_OFF,0,NULL);
	#endif
	
		/* puts("."); */
	
		//ew_printf("test3:%d\n",sizeof(test3));
	
		//ew_printf("get_bt_newest_status:%d\n",get_bt_newest_status());
		//ew_printf("get_bt_connect_status:%d\n",get_bt_connect_status());
		
		//ew_printf("setup.aud_vol:%d\n",setup.aud_vol);
		//ew_printf("sys_vol_l:%d\n",dac_ctl.sys_vol_l);
		//ew_printf("sys_vol_r:%d\n",dac_ctl.sys_vol_r);
	
		//ew_printf("AUDIO_VOLUME_MAX:%d\n",AUDIO_VOLUME_MAX);
	}

	struct list_head *pos;
    __timer_handle  *timer;
	list_for_each(pos,&(timer0_hl->list))
	{
	    timer = list_entry(pos,__timer_handle,list);

   		if(++timer->cnt >= timer->time)
   		{
   			timer->cnt = 0;
   			if(timer->handler)
   			{
   		        timer->handler(timer->param);
   			}
   		}
	}
}

s32 timer0_init(void)
{
	s32 ret;
    __timer_param   timer_parm;
	timer_module_on();
	timer0_hl = timer_open(TIMER0,TIMER_MAX_ISR_FUN);
	if(NULL == timer0_hl)
	{
		printf("timer_open err");
		ret = TIMER_DRV_OPEN_ERR;
		return ret;
	}
    timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
#if defined(_SPECIAL_VR_)||defined(_ENABLE_RADIO_TIME_)
    timer_parm.tick_time  = 1000;//(1ms)
#else
	timer_parm.tick_time  = 2000;//(2ms)
#endif
	ret = timer_init_api(timer0_hl,&timer_parm);

	if(ret != TIMER_NO_ERR)
	{
		printf("timer_init err = %x\n",ret);
		return ret;
	}

	ret = timer_start(timer0_hl);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_start err = %x\n",ret);
		return ret;
	}
	ret = timer_reg_isr_callback_fun(timer0_hl,1,timer0_isr_callback);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_reg_isr_callback_fun err = %x\n",ret);
		return ret;
	}
	return ret;
}

static s32 timer0_clk_reset(void)
{
	s32 ret;
    __timer_param   timer_parm;
	puts("timer_clk_reset\n");

    timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
#if defined(_SPECIAL_VR_)||defined(_ENABLE_RADIO_TIME_)
	timer_parm.tick_time  = 1000;//(1ms)
#else
	timer_parm.tick_time  = 2000;//(2ms)
#endif

	ret = timer_init_api(timer0_hl,&timer_parm);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_init err = %x\n",ret);
		return ret;
	}

	ret = timer_start(timer0_hl);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_start err = %x\n",ret);
		return ret;
	}

	puts("timer_init_OK\n");
	return ret;
}

static struct clock_switch clock_switch_timer;

CLOCK_SWITCH_USER_REGISTER(CLOCK_SWITCH_EXIT, &clock_switch_timer, timer0_clk_reset, "TIMER0");

#if defined( TIMER2_FOR_TIME_DATA)

/*-------------------------warning!!!!------------------------*/
/*-------------timer2 ?D??·t??oˉêy???ü×??òμ￥????--------------*/
/*-------------------------warning!!!!------------------------*/
static u32  * Time_cnt_p = (u32 *)0x19FE8;
static u32  * tmp_cnt_p = (u32 *)(0x19FE8 + 4);
u32  * power_up_flag_2 = (u32 *)(0x19FE8 + 8);
extern u8 sleep_flag;
u16 time_4ms_cnt=0;
void accoff_auto_inc_onesecond(void)
{//每次开关ACC,时钟自动补偿一秒
    (* Time_cnt_p) ++;
}
void accoff_auto_inc_halfsecond(void)
{//+125 8小时快1S +100 14小时慢1S
	time_4ms_cnt += 118;
	if(time_4ms_cnt >250)
	{
	    (* Time_cnt_p) ++;
		time_4ms_cnt -= 250;
	}
}

void clr_time_cnt(void)
{
	RTC_TIME rtc_time;
	(* Time_cnt_p)=0;
	(* tmp_cnt_p)=0;
    rtc_time.dYear  = 2019;
    rtc_time.bMonth = 1;
    rtc_time.bDay   = 1;
    rtc_time.bHour  = 0;
    rtc_time.bMin   = 0;
    rtc_time.bSec   = 0;
    sys_set_rtc(&rtc_time);
}

uint32 get_time_tick(void)
{
	return (*Time_cnt_p);
}

void set_time_tick(uint32 tick)
{
	(*Time_cnt_p) = tick;
}
static void timer2_isr_callback(void)  //  4ms
{
#if defined(_ENABLE_TEMP_PWM_)
	static u32 cnt1 = 0;
#endif

	time_4ms_cnt++;
	if(time_4ms_cnt >=250)
	{
		time_4ms_cnt =0;
	    (* Time_cnt_p) ++;
	}
//	printf("my_test_cnt:%d\n",(* Time_cnt_p));
	if(sleep_flag &&ACC_CHECK())
	{
        sleep_flag =0;
        gEnterAccMode=FALSE;
		(*power_up_flag_2) = 0x00;
		(*tmp_cnt_p) = JL_PWM->TMR2_CNT;
		JL_POWER->CON |= BIT(4);
	}
	
#ifdef	UART_Servo_Pop
	t2_cnt1++;
	if((t2_cnt1 % 3) == 0)//12ms
	{
		DABDriverServer();
	}
#endif

#if defined(_USER_TFT_DISP_)
    t3_cnt3++;
	if((t3_cnt3 % 3) == 0)//12ms
	{
		 TFTDriverServer();
	}
#endif

#if defined(_SUPPORT_CAN_UART_)
    t4_cnt4++;
	if((t4_cnt4 % 3) == 0)
//12ms
	{
		 McuDriverServer();
	}
#endif

#if defined(_AUDIO_DRIVER_AP3768_)
    t5_cnt5++;
	if((t5_cnt5 % 3) == 0)
//12ms
	{
		 AudioDriverServer();
	}
#endif

#ifdef	TD_ADD_SPP
	t3_cnt1++;
	if(t3_cnt1  == 25)//100
	{
		t3_cnt1=0;
 
		if(Audio_info_compare())
		{   //gRefresh_Audio=0;
		    send_flag=1;
			send_count=0;
		}
    #ifdef	TD_ADD_NEW_APP
    	if((audio_vol_compare())&&(global.audio_flag)) //APP发过来的数据只做处理，不回传给APP
    	{
    		os_taskq_post(MAIN_TASK_NAME,1,SYS_VOL_CHANGE);
    		global.audio_flag = FALSE;
    	}
    #endif
		if(send_flag==1)
		{
		#ifndef TD_ADD_NEW_APP
			if(gRefresh_Audio==1)
		#endif
			{
			#if defined(TD_ADD_NEW_APP)//APP发过来的数据只做处理，不回传给APP
			    if(global.audio_flag)
			    {
					global.audio_flag = FALSE;
					send_flag = 0;
					gRefresh_Audio=0;
					os_taskq_post(MAIN_TASK_NAME,1,MSG_spp_ble);//spp发送数据最好不要往主函数main里发消息，
				}
			#else
				send_flag = 0;
				gRefresh_Audio=0;
				os_taskq_post(MAIN_TASK_NAME,1,MSG_spp_ble);//spp发送数据最好不要往主函数main里发消息，
			#endif
			}
		}
	}
#endif

#if defined(_ENABLE_TEMP_PWM_)
	cnt1++;
	if(cnt1  == 250)//1S
	{
		cnt1 = 0;
		if((global.sys_power)&&(setup.warm_power))
		{
			if(setup.warm_time)
			{
				setup.warm_time--;
			}
		}
	}
#endif

/*	static u32 cnt = 0;

	JL_PORTB->DIR &=~(BIT(0)|BIT(1)|BIT(2));

	cnt++;

	JL_PORTB->OUT ^= BIT(2);

    if((cnt % 10) == 0)
	{
		JL_PORTB->OUT ^= BIT(1);
	}

	if(cnt >= 500)//500ms
	{
		JL_PORTB->OUT ^= BIT(0);
        cnt = 0;
	}*/
}

s32 timer2_init(void)
{
	s32 ret;
    u32 t2_cnt = (*tmp_cnt_p);
    __timer_param   timer_parm;

	timer2_hl = timer_open(TIMER2,TIMER_MAX_ISR_FUN);
	if(NULL == timer2_hl)
	{
		printf("timer_open err");
		ret = TIMER_DRV_OPEN_ERR;
		return ret;
	}

	timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
	timer_parm.tick_time  = 4000;//
	ret = timer_init_api(timer2_hl,&timer_parm);
    JL_PWM->TMR2_CNT = t2_cnt+4687; //200MS

	if(ret != TIMER_NO_ERR)
	{
		printf("timer_init err = %x\n",ret);
		return ret;
	}

	ret = timer_start(timer2_hl);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_start err = %x\n",ret);
		return ret;
	}
	ret = timer_reg_isr_callback_fun(timer2_hl,1,timer2_isr_callback);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_reg_isr_callback_fun err = %x\n",ret);
		return ret;
	}
	return ret;
}

static s32 timer2_clk_reset(void)
{
	s32 ret;
    __timer_param   timer_parm;
	puts("timer_clk_reset\n");

    timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
    timer_parm.tick_time  = 4000;//

	ret = timer_init_api(timer2_hl,&timer_parm);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_init err = %x\n",ret);
		return ret;
	}

	ret = timer_start(timer2_hl);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer_start err = %x\n",ret);
		return ret;
	}

	puts("timer_init_OK\n");
	return ret;
}

static struct clock_switch clock_switch_timer2;

CLOCK_SWITCH_USER_REGISTER(CLOCK_SWITCH_EXIT, &clock_switch_timer2, timer2_clk_reset, "TIMER2");

#else
s32 timer2_init(void)
{
	return 0;
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

#if defined TD_ADD_RCLK_32_768K
__timer_handle  *timer3_hl= NULL;
static void timer3_isr_callback(void)//100us
{
    static u8 flag=0;
    JL_PORTC->DIR &= ~ BIT(0);
    if(flag)
    {
        JL_PORTC->OUT |= BIT(0);
    	flag=0;
    }
    else
    {
        JL_PORTC->OUT &= ~ BIT(0);
    	flag=1;
    }
}
s32 timer3_init(void)
{
	s32 ret;
    __timer_param   timer_parm;
	timer3_hl = timer_open(TIMER3,TIMER_MAX_ISR_FUN);
	if(NULL == timer3_hl)
	{
		printf("timer3_open err");
		ret = TIMER_DRV_OPEN_ERR;
		return ret;
	}
    timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
    timer_parm.tick_time  = 10;
	ret = timer_init_api(timer3_hl,&timer_parm);

	if(ret != TIMER_NO_ERR)
	{
		printf("timer3_init err = %x\n",ret);
		return ret;
	}

	ret = timer_start(timer3_hl);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer3_start err = %x\n",ret);
		return ret;
	}
	ret = timer_reg_isr_callback_fun(timer3_hl,1,timer3_isr_callback);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer3_reg_isr_callback_fun err = %x\n",ret);
		return ret;
	}
    JL_TIMER3->PRD =227;//0xEA60/40;//60000/15M/40=100US
    JL_TIMER3->CON =0x11;//60M/4=15M(4分频)
	return ret;
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_PANEL_PWM_)
#if defined(_ENABLE_ALARM_CLOCK_)

void set_panel_pwm(uint8 r, uint8 g, uint8 b)
{
	if(r)
	{
		JL_TIMER3->CON = BIT(8) | BIT(0);
	}
	else
	{
		JL_TIMER3->CON &= ~(BIT(8)| BIT(0));
	}
}

void set_panel_enable(uint8 enable)
{
	SYS_RED_OFF();
	SYS_GREEN_OFF();
	SYS_BLUE_OFF();
#if defined(SET_BEEP_FUNCV)
	SYS_BEEP_OFF();
#endif
}

#else
#if defined(_ENABLE_ALARM_CLOCK_)// 2K
#define PWM_STEP_MAX			2  // 亮度等级
#elif defined(_ENABLE_BEEP_)
#define PWM_STEP_MAX			2  // 亮度等级
#else
#define PWM_STEP_MAX			20  // 亮度等级
#endif

static uint8 pwm_r = 0;
static uint8 pwm_g = 0;
static uint8 pwm_b = 0;
static uint8 pwm_cnt = 0;

#if defined(SET_BEEP_FUNCV)
static uint8 pwm_beep = 0;
#endif

#if defined(SET_BEEP_FUNCV)
void set_beep_ctrl(uint8 beep)
{
	if(beep > PWM_STEP_MAX)
		beep = PWM_STEP_MAX;
	pwm_beep = beep;
	printf("pwm_beep:%d\r\n",pwm_beep);
}
#endif

void set_panel_pwm(uint8 r, uint8 g, uint8 b)
{
	if(r > PWM_STEP_MAX)r = PWM_STEP_MAX;
	if(g > PWM_STEP_MAX)g = PWM_STEP_MAX;
	if(b > PWM_STEP_MAX)b = PWM_STEP_MAX;

	pwm_r = r;
	pwm_g = g;
	pwm_b = b;
}

void set_panel_enable(uint8 enable)
{
	SYS_RED_OFF();
	SYS_GREEN_OFF();
	SYS_BLUE_OFF();
#if defined(SET_BEEP_FUNCV)
	SYS_BEEP_OFF();
#endif
}

//static uint8 pwm_flag = 0;
static void timer3_isr_callback(void)
{
	/*if(pwm_flag)
	{
		SYS_RED_ON();
		SYS_GREEN_ON();
		SYS_BLUE_ON();
		pwm_flag = FALSE;
	}
	else
	{
		SYS_RED_OFF();
		SYS_GREEN_OFF();
		SYS_BLUE_OFF();
		pwm_flag = TRUE;
	}*/

	if(pwm_cnt < pwm_r)
	{
		SYS_RED_ON();
	}
	else
	{
		SYS_RED_OFF();
	}

	if(pwm_cnt < pwm_g)
	{
		SYS_GREEN_ON();
	}
	else
	{
		SYS_GREEN_OFF();
	}

	if(pwm_cnt < pwm_b)
	{
		SYS_BLUE_ON();
	}
	else
	{
		SYS_BLUE_OFF();
	}
	
#if defined(SET_BEEP_FUNCV)
	if(pwm_cnt < pwm_beep)
	{
		SYS_BEEP_ON();
	}
	else
	{
		SYS_BEEP_OFF();
	}
#endif

	if(++pwm_cnt >= PWM_STEP_MAX)
	{
		pwm_cnt = 0;
	}
}

//__timer_handle  *timer3_hl= NULL;
s32 timer3_init(void)
{
	s32 ret;
    __timer_param   timer_parm;
	timer3_hl = timer_open(TIMER3,TIMER_MAX_ISR_FUN);
	if(NULL == timer3_hl)
	{
		printf("timer3_open err");
		ret = TIMER_DRV_OPEN_ERR;
		return ret;
	}
    timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
    timer_parm.tick_time  = 10;
	ret = timer_init_api(timer3_hl,&timer_parm);

	if(ret != TIMER_NO_ERR)
	{
		printf("timer3_init err = %x\n",ret);
		return ret;
	}

	ret = timer_start(timer3_hl);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer3_start err = %x\n",ret);
		return ret;
	}
	ret = timer_reg_isr_callback_fun(timer3_hl,1,timer3_isr_callback);
	if(ret != TIMER_NO_ERR)
	{
		printf("timer3_reg_isr_callback_fun err = %x\n",ret);
		return ret;
	}
#if defined(_ENABLE_ALARM_CLOCK_)||defined(_ENABLE_BEEP_)
#if defined(_ENABLE_BEEP_FREQ)
	JL_TIMER3->PRD =5200;//60000/15M/40=100US
#else
	JL_TIMER3->PRD =3760;//60000/15M/40=100US
#endif
#else
    JL_TIMER3->PRD =0xEA60/20;//60000/15M/40=100US
#endif
    JL_TIMER3->CON =0x11;//60M/4=15M(4分频)
	return ret;
}
#endif
#endif

#if defined(_ENABLE_TIME3_COUNT_)
u8 P60_data = 0;
#if defined(_ENABLE_LIGHT_SHADE_)
u8 red_data = 0;
u8 green_data = 0;
u8 blue_data = 0;
#endif
volatile u8 P60_busy = 0;
u32 delay_count = 0;
u16 step = 0;

#if defined(_ENABLE_LIGHT_SHADE_)
u8 LightDriverIrCmd(uint8 cmd,uint8 red,uint8 green,uint8 blue)
{
    if(P60_busy){
        return 0;
    }
    P60_data = cmd;
    red_data = red;
    green_data = green;
    blue_data = blue;
    delay_count = 0;
    step = 0;
    P60_busy = 1;
    return 1;
}
#else
u8 LightDriverIrCmd(uint8 cmd)
{
    if(P60_busy){
        return 0;
    }
    P60_data = cmd;
    delay_count = 0;
    step = 0;
    P60_busy = 1;
    return 1;
}
#endif

#if defined(_ENABLE_LIGHT_SHADE_)
static void timer3_isr_callback(void)
{
    static u8 i = 0;
    static u8 j = 0;
    static u8 i1 = 0;
    static u8 j1 = 0;
    static u8 i2 = 0;
    static u8 j2 = 0;
    static u8 i3 = 0;
    static u8 j3 = 0;
	
    if(delay_count)
	{
        delay_count--;
        if(delay_count)return;
    }
    if(P60_busy == 0)
	{
        return;
    }
    switch(step)
    {
    case 0:
        SYS_30P60_LIGHT_H();
        delay_count = 1;
        step++;
        break;
    case 1:
        SYS_30P60_LIGHT_L();
        delay_count = 10;
        step++;
        i = 0;
        j = 0;
        break;
    case 2:
        if(j == 0)
        {
            SYS_30P60_LIGHT_L();
            delay_count = 2;
        }
		else
        {
            SYS_30P60_LIGHT_H();
            if (P60_data &0x01)
			{
                delay_count = 6;
            }
			else
            {
                delay_count = 2;
            }
            P60_data >>= 1;
            i++;
        }
        j = !j;
        if(i >= 8)
		{
            step++;
			i1 = 0;
			j1 = 0;
        }
        break;
	case 3:
		if(j1 == 0)
		{
			SYS_30P60_LIGHT_L();
			delay_count = 2;
		}
		else
		{
			SYS_30P60_LIGHT_H();
			if (red_data &0x01)
			{
				delay_count = 6;
			}
			else
			{
				delay_count = 2;
			}
			red_data >>= 1;
			i1++;
		}
		j1 = !j1;
		if(i1 >= 8)
		{
			step++;
			i2 = 0;
			j2 = 0;
		}
		break;
	case 4:
		if(j2 == 0)
		{
			SYS_30P60_LIGHT_L();
			delay_count = 2;
		}
		else
		{
			SYS_30P60_LIGHT_H();
			if (green_data &0x01)
			{
				delay_count = 6;
			}
			else
			{
				delay_count = 2;
			}
			green_data >>= 1;
			i2++;
		}
		j2 = !j2;
		if(i2 >= 8)
		{
			step++;
			i3 = 0;
			j3 = 0;
		}
		break;
	case 5:
		if(j3 == 0)
		{
			SYS_30P60_LIGHT_L();
			delay_count = 2;
		}
		else
		{
			SYS_30P60_LIGHT_H();
			if (blue_data &0x01)
			{
				delay_count = 6;
			}
			else
			{
				delay_count = 2;
			}
			blue_data >>= 1;
			i3++;
		}
		j3 = !j3;
		if(i3 >= 8)
		{
			step++;
		}
		break;
    case 6:
        SYS_30P60_LIGHT_L();
        delay_count = 1;
        step++;
        i = 0;
        j = 0;
        i1 = 0;
        j1 = 0;
        i2 = 0;
        j2 = 0;
        i3 = 0;
        j3 = 0;
        break;
    case 7:
        SYS_30P60_LIGHT_H();
        step = 0xff;
        P60_busy = 0;
        break;
    default:
        break;
    }
}
#else
static void timer3_isr_callback(void)
{
    static u8 i = 0;
    static u8 j = 0;
    if(delay_count)
	{
        delay_count--;
        if(delay_count)return;
    }
    if(P60_busy == 0)
	{
        return;
    }
    switch(step)
    {
    case 0:
        SYS_30P60_LIGHT_H();
        delay_count = 1;
        step++;
        break;
    case 1:
        SYS_30P60_LIGHT_L();
        delay_count = 10;
        step++;
        i = 0;
        j = 0;
        break;
    case 2:
        if(j == 0)
        {
            SYS_30P60_LIGHT_L();
            delay_count = 2;
        }
		else
        {
            SYS_30P60_LIGHT_H();
            if (P60_data &0x01)
			{
                delay_count = 6;
            }
			else
            {
                delay_count = 2;
            }
            P60_data >>= 1;
            i++;
        }
        j = !j;
        if(i >= 8)
		{
            step++;
        }
        break;
    case 3:
        SYS_30P60_LIGHT_L();
        delay_count = 1;
        step++;
        i = 0;
        j = 0;
        break;
    case 4:
        SYS_30P60_LIGHT_H();
        step = 0xff;
        P60_busy = 0;
        break;
    default:
        break;
    }
}
#endif

s32 timer3_init(void)
{
    s32 ret;
    __timer_param	timer_parm;
    timer3_hl = timer_open(TIMER3,TIMER_MAX_ISR_FUN);
    if(NULL == timer3_hl)
    {
    	printf("timer3_open err");
    	ret = TIMER_DRV_OPEN_ERR;
    	return ret;
    }
    timer_parm.work_mode  = TIMER_WORK_MODE_COUNTER;
    timer_parm.tick_time  = 10;
    ret = timer_init_api(timer3_hl,&timer_parm);
    
    if(ret != TIMER_NO_ERR)
    {
    	printf("timer3_init err = %x\n",ret);
    	return ret;
    }
    
    ret = timer_start(timer3_hl);
    if(ret != TIMER_NO_ERR)
    {
    	printf("timer3_start err = %x\n",ret);
    	return ret;
    }
    ret = timer_reg_isr_callback_fun(timer3_hl,1,timer3_isr_callback);
    if(ret != TIMER_NO_ERR)
    {
    	printf("timer3_reg_isr_callback_fun err = %x\n",ret);
    	return ret;
    }
    JL_TIMER3->PRD =16000;//1MS
    JL_TIMER3->CON =0x11;//60M/4=15M(4分频)
    return ret;
}
#endif

#if defined(_ENABLE_TIME3_DELAY_)
void delay_1ms()
{
    JL_TIMER3->CON = BIT(14);
    JL_TIMER3->PRD = 375;
    JL_TIMER3->CNT = 0;
    SFR(JL_TIMER3->CON, 2, 2, 2); //use osc
    SFR(JL_TIMER3->CON, 4, 4, 3); //div64
    SFR(JL_TIMER3->CON, 14, 1, 1); //clr pending
    SFR(JL_TIMER3->CON, 0, 2, 1); //
    while (!(JL_TIMER3->CON & BIT(15)));
    JL_TIMER3->CON = BIT(14);
}
#endif

