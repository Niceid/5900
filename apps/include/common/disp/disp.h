#ifndef __DISP_H__
#define __DISP_H__
#ifdef	TD_ADD_LCD_TEST
extern uint8 gDispPos;
#endif
#ifdef	TD_ADD_NEW_APP
extern u8 read_list_ready_flag;
#endif
#if defined(_SUPPORT_LCM_DRIVE)
extern u8	lLcdStr[16];
extern void Lcm_Start(void);
extern void Lcm_Stop(void);
extern void Lcm_Write (u8 data);
extern void Lcm_Gpio_Init(void);
extern void Lcm_Drive_Init(void);
extern void Clr_Lcm_Page(u8 page);
extern void Clr_Lcm_Disp(void);
extern void lcm_disp_clock(void);
extern void lcm_disp_radio(void);
extern void lcm_disp_music(void);
extern void lcm_disp_aux(void);
extern void lcm_disp_bt(void);
extern void Lcm_Disp(u8 x,u8 y,u8 *data,u8 data_size);
extern void Lcm_Disp_8_16(u8 x_start,u8 y_start,u8 *data,u8 data_size);
extern void Lcm_Disp_8_16_jump(u8 x_start,u8 y_start,u8 *data,u8 data_size);
extern u8 lcm_disp_audio(void);
extern void Lcm_disp_updata(u8 val);
#endif

#if defined(lcm_haotuo_dab)
extern void lcm_gpio_init(void);		//引脚初始化
extern void wcomspi(int cmd);
extern void wdataspi(int dat);
extern void ini_s6b0724(void);
extern void test(int ph,pl);
extern void Displine(int ph,pl,pw,pd);
extern void delay_ms(unsigned int y);	 //  延时ms

void test_picture (void);
void test_no1(void);


#endif

/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/

enum{
	DISP_TYPE_IDLE = 0,
	DISP_TYPE_RADIO,
#if defined(DAB_MODLE)
	DISP_TYPE_DAB,
#endif
#if defined(_SUPPORT_TAPE_)
	DISP_TYPE_TAPE,
#endif
#if defined(_SUPPORT_MIC_OK_)
	DISP_TYPE_MIC,
#endif
#if defined(_SUPPORT_CAN_MODEL_)
	DISP_TYPE_CAN,
#endif
#if defined(_SUPPORT_REVERSE_MODEL_)
	DISP_TYPE_REVE,
#endif

	DISP_TYPE_USB,
	DISP_TYPE_SD,
#if defined(_SUPPORT_DISC_)
	DISP_TYPE_PLAYER_DISC,
	DISP_TYPE_PLAYER_USB,
	DISP_TYPE_PLAYER_CARD,
#endif
	DISP_TYPE_BT,
	DISP_TYPE_AUX,
#if defined(_SUPPORT_AUX1_)
	DISP_TYPE_AUX1,
#endif
	DISP_TYPE_REC,
	DISP_TYPE_MAX,
};


#define disp_send_msg(x)				os_taskq_post_msg(DISP_TASK_NAME, 1, (int)(x))
#define disp_send_param(x,y)			os_taskq_post_msg(DISP_TASK_NAME, 2, (int)(x), (int)(y))
#define disp_send_param2(x,y,z)			os_taskq_post_msg(DISP_TASK_NAME, 3, (int)(x), (int)(y), (int)(z))


extern void disp_task_init(void);
extern void disp_task_exit(void);

extern void sys_mute_ctrl(uint8, uint16);
extern void disp_func_reset(void);

#if defined(_ENABLE_PASSWORD_INPUT_)||defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
extern void password_disp_polling(void);
#endif

#if defined(_DISABLE_TALKING_KEY_)
extern uint8 bt_call_state(void);
#endif

#if defined(_XM003_SPECIAL_MUTE_)||defined(_YH1701_SPECIAL_MUTE_)||defined(_XM007_SPECIAL_MUTE_)
extern void func_mute_control(void);
#endif

#endif /* __DISP_H__ */


