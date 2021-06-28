#include "sdk_cfg.h"
#include "order.h"
#include "msg.h"
#include "key_drv.h"
#include "setup.h"
//#ifdef SUPPORT_CALL_NAMES
//extern uint8 play_song_flag;
//#endif

#include "easyway_config.h"

#include ORDER_KEY_MAP_DIR_H
#include ORDER_KEY_MAP_DIR_C

#include ORDER_IR_MAP_DIR_H
#include ORDER_IR_MAP_DIR_C

#ifdef KEY_ORDER_SYSTEM_2NDF
#define KEY_MAP_SYSTEM_2NDF_			KEY_ORDER_SYSTEM_2NDF
#else
#define KEY_MAP_SYSTEM_2NDF_			ORDER_SYSTEM_KEY
#endif


#ifdef IR_ORDER_SYSTEM_2NDF
#define IR_MAP_SYSTEM_2NDF_				IR_ORDER_OTHERS_2NDF
#else
#define IR_MAP_SYSTEM_2NDF_				ORDER_SYSTEM_IR
#endif

#ifdef KEY_ORDER_OTHERS_2NDF
#define KEY_MAP_OTHERS_2NDF_			KEY_ORDER_OTHERS_2NDF
#else
#define KEY_MAP_OTHERS_2NDF_			ORDER_OTHERS_KEY
#endif


#ifdef IR_ORDER_OTHERS_2NDF
#define IR_MAP_OTHERS_2NDF_				IR_ORDER_OTHERS_2NDF
#else
#define IR_MAP_OTHERS_2NDF_				ORDER_OTHERS_IR
#endif


#ifdef KEY_ORDER_RADIO_2NDF
#define KEY_MAP_RADIO_2NDF_				KEY_ORDER_RADIO_2NDF
#else
#define KEY_MAP_RADIO_2NDF_				ORDER_RADIO_KEY
#endif


#ifdef IR_ORDER_RADIO_2NDF
#define IR_MAP_RADIO_2NDF_				IR_ORDER_RADIO_2NDF
#else
#define IR_MAP_RADIO_2NDF_				ORDER_RADIO_IR
#endif


#ifdef KEY_ORDER_PLAYER_2NDF
#define KEY_MAP_PLAYER_2NDF_			KEY_ORDER_PLAYER_2NDF
#else
#define KEY_MAP_PLAYER_2NDF_			ORDER_PLAYER_KEY
#endif


#ifdef IR_ORDER_PLAYER_2NDF
#define IR_MAP_PLAYER_2NDF_				IR_ORDER_PLAYER_2NDF
#else
#define IR_MAP_PLAYER_2NDF_				ORDER_PLAYER_IR
#endif


#ifdef KEY_ORDER_BT_2NDF
#define KEY_MAP_BT_2NDF_				KEY_ORDER_BT_2NDF
#else
#define KEY_MAP_BT_2NDF_				ORDER_BT_KEY
#endif

#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
#ifdef KEY_ORDER_PASSWORD_2NDF
#define KEY_MAP_PASSWORD_2NDF_			KEY_ORDER_PASSWORD_2NDF
#else
#define KEY_MAP_PASSWORD_2NDF_			ORDER_PASSWORD_KEY
#endif
#ifdef IR_ORDER_PASSWORD_2NDF
#define IR_MAP_PASSWORD_2NDF_			IR_ORDER_PASSWORD_2NDF
#else
#define IR_MAP_PASSWORD_2NDF_			ORDER_PASSWORD_IR
#endif
#endif

#ifdef IR_ORDER_BT_2NDF
#define IR_MAP_BT_2NDF_					IR_ORDER_BT_2NDF
#else
#define IR_MAP_BT_2NDF_					ORDER_BT_IR
#endif

static ORDER Order;
ext_check_f order_ext_check = NULL;

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/

#define ORDER_MAP_CFG(Tab)						\
{												\
	( sizeof(Tab)/sizeof(ORDER_CFG) ),			\
	Tab,										\
}


#define ORDER_MAP_TAB(KeyCfg,IrCfg)				\
{												\
	ORDER_MAP_CFG(KeyCfg),						\
	ORDER_MAP_CFG(IrCfg),						\
}

static const ORDER_MAP_FORMAT OrderMapTab[ORDER_ID_MAX]=
{
	ORDER_MAP_TAB(ORDER_SYSTEM_KEY, 	ORDER_SYSTEM_IR 		),	/*	SYSTEM				*/
	ORDER_MAP_TAB(ORDER_OTHERS_KEY, 	ORDER_OTHERS_IR 		),	/*	OTHERS				*/
	ORDER_MAP_TAB(ORDER_RADIO_KEY,		ORDER_RADIO_IR			),	/*	PROJECT-->RADIO 	*/
	ORDER_MAP_TAB(ORDER_PLAYER_KEY, 	ORDER_PLAYER_IR 		),	/*	PROJECT-->PLAYER	*/
	ORDER_MAP_TAB(0,					0						),	/*	PROJECT-->AUX		*/
	ORDER_MAP_TAB(ORDER_BT_KEY, 		ORDER_BT_IR 			),	/*	PROJECT-->TEL		*/
#if defined(DAB_MODLE)
#if defined(DAB_MODLE_IR)
	ORDER_MAP_TAB(ORDER_DAB_KEY,	ORDER_DAB_IR				),	/*	PROJECT-->DAB	*/
#else
	ORDER_MAP_TAB(ORDER_DAB_KEY,	0						),	/*	PROJECT-->DAB	*/
#endif
#endif
#if defined(_SUPPORT_TAPE_)
	ORDER_MAP_TAB(ORDER_TAPE_KEY,	0						),	/*	PROJECT-->TAPE	*/
#endif
#if defined(_ENABLE_MIC_REC_)
	ORDER_MAP_TAB(ORDER_MIC_KEY,	0						),	/*	PROJECT-->MIC	*/
#endif
#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
	ORDER_MAP_TAB(ORDER_PASSWORD_KEY, 	ORDER_PASSWORD_IR 		),	/*	PASSWORD		*/
#endif
#if REC_EN
	ORDER_MAP_TAB(ORDER_RECORD_KEY, 	0 		                ),	/*	PROJECT-->RECORD	*/
#endif
};

static const ORDER_MAP_FORMAT Order2ndfMapTab[ORDER_ID_MAX]=
{
	ORDER_MAP_TAB(KEY_MAP_SYSTEM_2NDF_, 	IR_MAP_SYSTEM_2NDF_ 		),	/*	SYSTEM				*/
	ORDER_MAP_TAB(KEY_MAP_OTHERS_2NDF_, 	IR_MAP_OTHERS_2NDF_ 		),	/*	OTHERS				*/
	ORDER_MAP_TAB(KEY_MAP_RADIO_2NDF_,		IR_MAP_RADIO_2NDF_			),	/*	PROJECT-->RADIO 	*/
	ORDER_MAP_TAB(KEY_MAP_PLAYER_2NDF_, 	IR_MAP_PLAYER_2NDF_ 		),	/*	PROJECT-->PLAYER	*/
	ORDER_MAP_TAB(0,						0							),	/*	PROJECT-->AUX		*/
	ORDER_MAP_TAB(KEY_MAP_BT_2NDF_, 		IR_MAP_BT_2NDF_ 			),	/*	PROJECT-->TEL		*/
#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
	ORDER_MAP_TAB(KEY_MAP_PASSWORD_2NDF_,	IR_MAP_PASSWORD_2NDF_		),	/*	PASSWORD		*/
#endif
};

/*-------------msg:按键值----------------------------------------------------------------------------------------------------------------------*/
/*------------order_type 0:主任务按键-----------------------------------------------------------------------------------------------------------------------*/
/*------------cfg_type:1:面板按键-2:遥控按键----------------------------------------------------------------------------------------------------------------------*/

static void task_msg_send(int msg, uint8 order_type, uint8 cfg_type, uint8 order_mode)
{
	if(order_ext_check)
	{
		if(order_ext_check(&msg))return;
	}

	if(global.sys_normal)
	{
		int msg_type = (cfg_type << 8) | order_mode;
		if(order_type == ORDER_TYPE_OTHERS)
		{
			msg_type |= MSG_EXT_OTHERS;
		}

		if(order_type == ORDER_TYPE_SYSTEM)
		{
			os_taskq_post_msg(MAIN_TASK_NAME, 2, msg, msg_type);
		}
		else
		{
			if(keymsg_task_name)
		    {
		    #if REC_EN
		    	if(order_type == ORDER_TYPE_OTHERS)
		    	{
		    		os_taskq_post_msg(MAIN_TASK_NAME, 2, msg, msg_type);
		    	}
		    	else
		    	{
		    		os_taskq_post_msg((char *)keymsg_task_name, 2, msg, msg_type);
		    	}
		    #else
		    	os_taskq_post_msg((char *)keymsg_task_name, 2, msg, msg_type);
		    #endif
		    }
		}
	}
}

#if defined(_ENABLE_BEEP_)
extern void set_beep_pwm(void);
static void OrderBeep(void)
{
	puts("OrderBeep\n");
	if(setup.aud_beep)
	{
		if(global.sys_power)
		{
		#if defined(_XM007_SPECIAL_MUTE_)
			if(global.aud_mute)
			{
				if(GET_KEY_KEY != KEY_SYSTEM_MODE)//MUTE的时候，按MODE不要BEEP声
				{
				#if defined(_XM007_MUSIC_BEEP_)
					if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
					{
						if((GET_KEY_KEY != KEY_VALUE_NEXT)&&(GET_KEY_KEY != KEY_VALUE_PREV))
						{
							set_beep_pwm();
						}
					}
					else
					{
						set_beep_pwm();
					}
				#else
					set_beep_pwm();
				#endif
				}
			}
			else
		#endif
			{
				set_beep_pwm();
			}
		}
	}
}
#endif

static ORDER_MAP_FORMAT const *GetOrderType(uint8 OrderType)
{
	int id = -1;

	switch(OrderType)
	{
		case ORDER_TYPE_SYSTEM:
			id = ORDER_ID_SYSTEM;
			break;
		case ORDER_TYPE_OTHERS:
			id = ORDER_ID_OTHERS;
			break;
		case ORDER_TYPE_PROJECT:
			if(Order.Project < KEY_PROJECT_MAX)
			{
				id = ORDER_ID_PROJECT_START + Order.Project;
			}
			break;
    #if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
    	case ORDER_TYPE_PASSWORD:
    		id = ORDER_ID_PASSWORD;
    		break;
    #endif
	}
	if(id != -1)
	{
		if(Order.Select == KEY_SELECT_DEFAULT)
		{
			return &OrderMapTab[id];
		}
		else if(Order.Select == KEY_SELECT_2NDF)
		{
			return &Order2ndfMapTab[id];
		}
	}
	return NULL;
}

void send_key_msg(int msg)
{
	if(Order.Project < KEY_PROJECT_MAX)
	{
		uint8 order_msg;
		uint8 check_flag = FALSE;

		if(msg == MSG_VOL_ADD)
		{
			order_msg = KEY_VALUE_ADD;
			check_flag = TRUE;
		}
		else if(msg == MSG_VOL_SUB)
		{
			order_msg = KEY_VALUE_SUB;
			check_flag = TRUE;
		}
	#if defined(_SUPPORT_VOL_ADD)
	    if(msg == MSG_VOL_ADD1)
		{
			order_msg = KEY_VALUE_ADD1;
			check_flag = TRUE;
		}
		else if(msg == MSG_VOL_SUB1)
		{
			order_msg = KEY_VALUE_SUB1;
			check_flag = TRUE;
		}
	#endif
    #if defined(_SUPPORT_VR2_)
       	if(msg == MSG_VOL_ADD2)
       	{
       		order_msg = KEY_VALUE_ADD2;
       		check_flag = TRUE;
       	}
       	else if(msg == MSG_VOL_SUB2)
       	{
       		order_msg = KEY_VALUE_SUB2;
       		check_flag = TRUE;
       	}
    #endif

		if(check_flag)
		{
			ORDER_MAP_FORMAT const *OrderMap = GetOrderType(ORDER_TYPE_PROJECT);
			if(OrderMap != NULL)
			{
				ORDER_CFG_FORMAT const *OrderCfgTab = &(OrderMap->Key);
				uint8 Index;
				ORDER_CFG OrderCfg;

				for(Index = 0; Index < OrderCfgTab->Length; Index++)
				{
					OrderCfg = (OrderCfgTab->OrderMap)[Index];

					if(order_msg == OrderCfg.Value)
					{
						if(OrderCfg.Mode == ORDER_MODE_DOWN)
						{
							msg = OrderCfg.Order;
							task_msg_send(OrderCfg.Order, ORDER_TYPE_PROJECT, ORDER_CFG_TYPE_KEY, ORDER_MODE_DOWN);
							return;
						}
					}
				}
			}
		}

		task_msg_send(msg, ORDER_TYPE_SYSTEM, ORDER_CFG_TYPE_KEY, ORDER_MODE_DOWN);
	}
/*#ifdef SUPPORT_CALL_NAMES
	else if(play_song_flag)
	{//播放提示音时需要调节音量大小
		if((msg == MSG_VOL_ADD)||(msg == MSG_VOL_SUB))
		{
            int msg_type = (ORDER_CFG_TYPE_KEY << 8) | ORDER_MODE_DOWN;
			os_taskq_post_msg(MAIN_TASK_NAME, 2, msg, msg_type);
		}
	}
#endif*/
}
//***********Type:1:面板按键 2:遥控按键
//*******OrderFormat
//*******Value:(KeyInfo.Value)
//*******State:(KeyInfo.State)
//*******UpTime:(KeyInfo.LastTime)
//*******p_DownTime:(KeyInfo.ValueTime)
static void OrderTypeProcess(	uint8 Type,
							ORDER_FORMAT *OrderFormat,
							uint8 Value,
							uint8 State,
							uint8 UpTime,
							uint8 *p_DownTime)
{
	uint8 	Index;
	uint8	OrderType;
	ORDER_MAP_FORMAT const *OrderMap;
	ORDER_CFG_FORMAT const *OrderCfgTab;
	ORDER_CFG OrderCfg;
	uint8 DownTime = *p_DownTime;

	//puts("OrderTypeProcess\n");
	if(OrderFormat->LastBuffer != State)
	{
		OrderFormat->LastBuffer = State;
		if(State == DOWN)
		{
			OrderFormat->State = ORDER_STATE_DOWN;
		}
		else if(State == UP)
		{
			if(OrderFormat->State == ORDER_STATE_DOWN)
			{
				OrderFormat->State = ORDER_STATE_UP;
			}
			else
			{
				OrderFormat->State = ORDER_STATE_NULL;
			}
		}
	}
	OrderFormat->Found = FALSE;

	for(OrderType = 0; OrderType < ORDER_TYPE_END; OrderType ++)
	{
		if((OrderMap = GetOrderType(OrderType)) == NULL)continue;

		if(Type == ORDER_CFG_TYPE_KEY)
		{//面板按键
			//puts("OrderTypeProcess111\n");
			OrderCfgTab = &(OrderMap->Key);
		}
		else if(Type == ORDER_CFG_TYPE_IR)
		{//遥控按键
			//puts("OrderTypeProcess222\n");
			OrderCfgTab = &(OrderMap->Ir);
		}
		else
		{
			//puts("OrderTypeProcess333\n");
			goto EXIT_PROCESS_ORDER;
		}

		for(Index = 0; Index < OrderCfgTab->Length; Index++)
		{
			OrderCfg = (OrderCfgTab->OrderMap)[Index];

			if(Value == OrderCfg.Value)
			{
				OrderFormat->Found = TRUE;
				if((State == UP)&&(OrderFormat->State == ORDER_STATE_UP))
				{//短按弹起
					if(OrderCfg.Mode == ORDER_MODE_UP_LESS_THAN)
					{
						if(UpTime < CONV_ORDER_TIME(1000))
						{
							goto ORDER_PROCESS_ENTER;
						}
					}
					else if(OrderCfg.Mode == ORDER_MODE_UP_MORE_THAN)
					{
						if(UpTime > CONV_ORDER_TIME(1000))
						{
						#if defined(_XM007_BEEP_CONTROL)
    						task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);
    						OrderFormat->State = ORDER_STATE_NULL;
    						goto EXIT_PROCESS_ORDER;
						#else
							goto ORDER_PROCESS_ENTER;
						#endif
						}
					}
				}
				else if((State == DOWN)&&((OrderFormat->State == ORDER_STATE_DOWN)||(OrderFormat->State == ORDER_STATE_DOWN_LOOP)))
				{
					if(OrderCfg.Mode == ORDER_MODE_DOWN)
					{
						if(DownTime == 0)
						{
							goto ORDER_PROCESS_ENTER;
						}
					}
					else if(OrderCfg.Mode == ORDER_MODE_DOWN_DELEY)
					{
					#if defined(_CHANGE_KEY_DELAY_)
					    if(DownTime == CONV_ORDER_TIME(1500))
						{
			ORDER_PROCESS_ENTER:
					    #if defined(_ENABLE_BEEP_)
					    	if(OrderCfg.Order != MSG_SYS_POWER)
					    	{
					    		OrderBeep();
					    	}
					    #endif
							task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);
							OrderFormat->State = ORDER_STATE_NULL;
							goto EXIT_PROCESS_ORDER;
						}
					#else
						if(DownTime == CONV_ORDER_TIME(1000))
						{
			ORDER_PROCESS_ENTER:
					    #if defined(_ENABLE_BEEP_)
					    #if !defined(_XM007_BEEP_CONTROL)//关机键也有BEEP声
					    	if(OrderCfg.Order != MSG_SYS_POWER)
					    #endif
					    	{
								puts("OrderBeep444\n");
					    		OrderBeep();
					    	}
					    #endif
							task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);
							OrderFormat->State = ORDER_STATE_NULL;
							goto EXIT_PROCESS_ORDER;
						}
					#endif
					}
				#if defined(_SL6812_TEST_LCD_)||defined(_BL7680_PASSWORD_SET_)
					else if(OrderCfg.Mode == ORDER_MODE_DOWN_DELEY_OTHERS)
					{
						puts("ORDER_MODE_DOWN_DELEY_OTHERS\n");
						if(DownTime == CONV_ORDER_TIME(3000))
						{
						#if defined(_ENABLE_BEEP_)
							if(OrderCfg.Order != MSG_SYS_POWER)
							{
								OrderBeep();
							}
						#endif
							task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);
							OrderFormat->State = ORDER_STATE_NULL;
							goto EXIT_PROCESS_ORDER;
						}
					}
				#endif
				#if defined(_SL6812_DELAY_KEY_)
    				else if(OrderCfg.Mode == ORDER_MODE_DOWN_DELEY_LONG)
    				{
    					puts("ORDER_MODE_DOWN_DELEY_LONG\n");
    					if(DownTime == CONV_ORDER_TIME(6700))
    					{
    					#if defined(_ENABLE_BEEP_)
    					#if !defined(_XM108_BEEP_CONTROL)//关机键也有BEEP声
    						if(OrderCfg.Order != MSG_SYS_POWER)
    					#endif
    						{
    							OrderBeep();
    						}
    					#endif
    						task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);
    						OrderFormat->State = ORDER_STATE_NULL;
    						goto EXIT_PROCESS_ORDER;
    					}
    				}
				#endif
					else if(OrderCfg.Mode == ORDER_MODE_DOWN_LOOP)
					{
						if(DownTime == 0 || DownTime >= CONV_ORDER_TIME(500))
						{
					    #if defined(_ENABLE_BEEP_)
					    #if !defined(_XM108_BEEP_CONTROL)//关机键也有BEEP声
					    	if(OrderCfg.Order != MSG_SYS_POWER)
					    #endif
					    	{
								puts("OrderBeep3333\n");
					    		OrderBeep();
					    	}
					    #endif
							task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);
							OrderFormat->State = ORDER_STATE_DOWN_LOOP;
							if(DownTime)
							{
								if(DownTime > 3)
								{
									DownTime -= 3;
								}
								else
								{
									DownTime = 0;
								}
								*p_DownTime = DownTime;
							}
							goto EXIT_PROCESS_ORDER;
						}
					}
					else if(OrderCfg.Mode == ORDER_MODE_DELEY_LOOP)
					{
						if(DownTime == CONV_ORDER_TIME(1000))
						{
					    #if defined(_ENABLE_BEEP_)
						#if !defined(_XM108_BEEP_CONTROL)//关机键也有BEEP声
					    	if(OrderCfg.Order != MSG_SYS_POWER)
						#endif
					    	{
								puts("OrderBeep2222\n");
					    		OrderBeep();
					    	}
					    #endif
							task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);

							DownTime = CONV_ORDER_TIME(2000);
							if(DownTime > 3)
							{
								DownTime -= 3;
							}
							else
							{
								DownTime = 0;
							}
							*p_DownTime = DownTime;
						}
						else if(DownTime >= CONV_ORDER_TIME(2000))
						{
							task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);

							if(DownTime > 3)
							{
								DownTime -= 3;
							}
							else
							{
								DownTime = 0;
							}
							*p_DownTime = DownTime;
						}
					}
				#if defined(_ZSBY5209_VOLUME_FUNTION_)
    				else if(OrderCfg.Mode == ORDER_MODE_OTHERS)
    				{
    					if(DownTime >= CONV_ORDER_TIME(1000))
    					{
    						task_msg_send(OrderCfg.Order, OrderType, Type, OrderCfg.Mode);
    						OrderFormat->State = ORDER_STATE_DOWN_LOOP;
    						if(DownTime)
    						{
    							if(DownTime > 3)
    							{
    								DownTime -= 3;
    							}
    							else
    							{
    								DownTime = 0;
    							}
    							*p_DownTime = DownTime;
    						}
    						goto EXIT_PROCESS_ORDER;
    					}
    				}	
				#endif
				}
			}
		}
	}

	if(OrderFormat->Found == FALSE)
	{
		OrderFormat->State = ORDER_STATE_NULL;
	}
EXIT_PROCESS_ORDER:
	OrderFormat->Found = FALSE;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static uint8 key_power_filter(uint16 order)
{
	if(GET_KEY_STATE != DOWN)
	{
		Order.Key.LastBuffer = GET_KEY_STATE;
		return TRUE;
	}
	if(GET_KEY_TIME_DOWN_TIME)return TRUE;
	GET_KEY_TIME_DOWN_TIME = INPUT_TIME_MAX;
#if defined(_ENABLE_BEEP_)
	puts("OrderBeep1111\n");
	OrderBeep();
#endif
	task_msg_send(order, ORDER_TYPE_SYSTEM, ORDER_CFG_TYPE_KEY, ORDER_MODE_DOWN);
	return TRUE;
}

static uint8 ir_power_filter(uint16 order)
{
	if(GET_IR_STATE != DOWN)
	{
		Order.Ir.LastBuffer = GET_IR_STATE;
		return TRUE;
	}
	if(GET_IR_TIME_DOWN_TIME)return TRUE;
	GET_IR_TIME_DOWN_TIME = INPUT_TIME_MAX;
#if defined(_ENABLE_BEEP_)
	OrderBeep();
#endif
	task_msg_send(order, ORDER_TYPE_SYSTEM, ORDER_CFG_TYPE_IR, ORDER_MODE_DOWN);
	return TRUE;
}

void key_process(void)
{
	uint8 HaveWorkFlag;

	HaveWorkFlag = FALSE;

	if(HaveWorkFlag == FALSE)
	{
		if(Order.Project >= KEY_PROJECT_MAX)
		{
		#if defined(ANY_KEY_POWER_ON)
			if(!global.sys_power)
			{
				GET_KEY_KEY =KEY_SYSTEM_POWER;
			}
		#endif
	    	if(GET_KEY_KEY == KEY_SYSTEM_POWER){;}
		#if defined(SUPPORT_PARTY_CONTROL)
			else if(GET_KEY_KEY == KEY_SYSTEM_POWER1){;}
		#endif
		#if defined(KEY_SYSTEM_EJECT)
			else if(GET_KEY_KEY == KEY_SYSTEM_EJECT){;}
		#endif
		#if defined(KEY_SYSTEM_DOOR)
			else if(GET_KEY_KEY == KEY_SYSTEM_DOOR){;}
		#endif
        #if defined(_SUPPORT_POWER_ON_PASSWORD_)
        	else if(GET_KEY_KEY == KEY_PASSWORD_NO_1){;}
        	else if(GET_KEY_KEY == KEY_PASSWORD_NO_2){;}
        	else if(GET_KEY_KEY == KEY_PASSWORD_NO_3){;}
        	else if(GET_KEY_KEY == KEY_PASSWORD_NO_4){;}
        	else if(GET_KEY_KEY == KEY_PASSWORD_NO_5){;}
        	else if(GET_KEY_KEY == KEY_PASSWORD_NO_6){;}
        	else if(GET_KEY_KEY == KEY_PASSWORD_ENTER){;}
		#elif defined(_SUPPORT_PASSWORD_DISPLAY_)
    		else if(GET_KEY_KEY == KEY_PASSWORD_NO_1){;}
    		else if(GET_KEY_KEY == KEY_PASSWORD_NO_2){;}
    		else if(GET_KEY_KEY == KEY_PASSWORD_NO_3){;}
    		else if(GET_KEY_KEY == KEY_PASSWORD_NO_4){;}
    		else if(GET_KEY_KEY == KEY_PASSWORD_ENTER){;}
		#endif
	    #if defined(KEY_SYSTEM_POWER_MUTE)
	    	else if(GET_KEY_KEY == KEY_SYSTEM_POWER_MUTE)
	    	{
	    		HaveWorkFlag = key_power_filter(MSG_SYS_POWER);
	    	}
	    #endif
        #if defined(KEY_SYSTEM_POWER_MUTE1)
        	else if(GET_KEY_KEY == KEY_SYSTEM_POWER_MUTE1)
        	{
        		HaveWorkFlag = key_power_filter(MSG_SYS_POWER);
        	}
        #endif
		#if defined(_POWER_SET_COLOR_)
	    	else if(GET_KEY_KEY == KEY_SYSTEM_POWER_COLOR)
	    	{
	    		HaveWorkFlag = key_power_filter(MSG_SYS_POWER);
	    	}
		#endif
	    #if defined(KEY_SYSTEM_POWER_MODE)
	    #if defined(_FY700B_PARTY_CONTROL_)||defined(_XZC1668_ADKEY_CONTROL_)||defined(_KLD3300_PARTY_CONTROL_)
	    	else if((GET_KEY_KEY == KEY_SYSTEM_POWER_MODE)||(GET_KEY_KEY == KEY_SYSTEM_MODE1))
	    	{
	    		if(!global.sys_power)
	    		{
	    			HaveWorkFlag = key_power_filter(MSG_SYS_POWER);
	    		}
	    	}
	    #else
	    	else if(GET_KEY_KEY == KEY_SYSTEM_POWER_MODE)
	    	{
	    		if(!global.sys_power)
	    		{
	    			HaveWorkFlag = key_power_filter(MSG_SYS_POWER);
	    		}
	    	}
	    #endif
	    #endif

	    #if defined(KEY_SYSTEM_POWER_SEL)
	    	else if(GET_KEY_KEY == KEY_SYSTEM_POWER_SEL)
	    	{
	    		if(!global.sys_power)
	    		{
	    			HaveWorkFlag = key_power_filter(MSG_SYS_POWER);
	    		}
	    	}
	    #endif
	    	else
	    	{
	    		HaveWorkFlag = TRUE;
	    	}
		}
	}

#if !defined(_XM003_SPECIAL_MUTE_)
#if !defined(_YH1701_SPECIAL_MUTE_)
#if !defined(_XM007_SPECIAL_MUTE_)
	if(HaveWorkFlag == FALSE)
	{
		if(global.aud_mute)
		{
			HaveWorkFlag = key_power_filter(MSG_SYS_MUTE);
		}
	}
#endif
#endif
#endif

#if defined(_YH1701_SPECIAL_MUTE_)
    if(global.aud_mute)
    {
    	if((GET_KEY_KEY == KEY_SYSTEM_POWER_MODE)||(GET_KEY_KEY == KEY_SYSTEM_POWER)
    	||(GET_KEY_KEY == KEY_SYSTEM_MODE)||(GET_KEY_KEY == KEY_VALUE_MUTE)
    	||(GET_KEY_KEY == KEY_VALUE_ADD)||(GET_KEY_KEY == KEY_VALUE_SUB))
    	{
    		if(HaveWorkFlag == FALSE)
    		{
    			OrderTypeProcess(
    						ORDER_CFG_TYPE_KEY,
    						&Order.Key,
    						GET_KEY_KEY,
    						GET_KEY_STATE,
    						GET_KEY_TIME_UP_TIME,
    						&GET_KEY_TIME_DOWN_TIME
    						);
    		}
    	}
    }
    else
    {
    	if(HaveWorkFlag == FALSE)
    	{
    		OrderTypeProcess(
    					ORDER_CFG_TYPE_KEY,
    					&Order.Key,
    					GET_KEY_KEY,
    					GET_KEY_STATE,
    					GET_KEY_TIME_UP_TIME,
    					&GET_KEY_TIME_DOWN_TIME
    					);
    	}
    }
#else
	if(HaveWorkFlag == FALSE)//到这
	{
		OrderTypeProcess(
					ORDER_CFG_TYPE_KEY,
					&Order.Key,
					GET_KEY_KEY,
					GET_KEY_STATE,
					GET_KEY_TIME_UP_TIME,
					&GET_KEY_TIME_DOWN_TIME
					);
	}
#endif
}

void ir_process(void)
{
	uint8 HaveWorkFlag;

	HaveWorkFlag = FALSE;

	if(HaveWorkFlag == FALSE)
	{
		if(Order.Project >= KEY_PROJECT_MAX)
		{
			#if defined(ANY_KEY_POWER_ON)
			if(!global.sys_power)
			{
				GET_IR_IR = IR_SYSTEM_POWER;
			}
			#endif
	    	if(GET_IR_IR == IR_SYSTEM_POWER)
			{;}
        #if defined(_REMOTE_TWO_TOGETHER_)
		#if !defined(DISABLE_POWER_FUNCTION_)
        	else if(GET_IR_IR == IR1_SYSTEM_POWER){;}
		#endif	
        #endif
		#if defined(_CHANGE_TWO_REMOTE_)
        	else if(GET_IR_IR == IR1_SYSTEM_POWER){;}
		#endif
	    #if defined(IR_SYSTEM_POWER_MUTE)
	    	else if(GET_IR_IR == IR_SYSTEM_POWER_MUTE)
	    	{
	    		HaveWorkFlag = ir_power_filter(MSG_SYS_POWER);
	    	}
	    #endif
		#if defined(_ADD_IR_FUNCTION_)
        #if defined(IR1_SYSTEM_POWER_MUTE)
	    	else if(GET_IR_IR == IR1_SYSTEM_POWER_MUTE)
	    	{
	    		HaveWorkFlag = ir_power_filter(MSG_SYS_POWER);
	    	}
        #endif
		#endif
	    #if defined(IR_SYSTEM_POWER_MODE)
	    	else if(GET_IR_IR == IR_SYSTEM_POWER_MODE)
	    	{
	    		HaveWorkFlag = ir_power_filter(MSG_SYS_POWER);
	    	}
	    #endif
		#if defined(IR_SYSTEM_EJECT)
			else if(GET_IR_IR == IR_SYSTEM_EJECT){;}
		#endif
        #if defined(_REMOTE_USE_TOGETHER_)
        	else if(GET_IR_IR == IR2_SYSTEM_POWER_MUTE)
        	{
        		HaveWorkFlag = ir_power_filter(MSG_SYS_POWER);
        	}
        #if defined(IR3_SYSTEM_POWER_MUTE)
        	else if(GET_IR_IR == IR3_SYSTEM_POWER_MUTE)
        	{
        		HaveWorkFlag = ir_power_filter(MSG_SYS_POWER);
        	}
        #endif
        #if defined(IR3_SYSTEM_POWER)
        	else if(GET_IR_IR == IR3_SYSTEM_POWER)
        	{
        		HaveWorkFlag = ir_power_filter(MSG_SYS_POWER);
        	}
        #endif
		#if defined(_ADD_POWER_FUNCTION_)
		    else if(GET_IR_IR == IR1_SYSTEM_POWER){;}
		#endif
        #if defined(_ADD_IR_POWER_FUNC_)
        	else if(GET_IR_IR == IR1_SYSTEM_POWER){;}
			else if(GET_IR_IR == IR2_SYSTEM_POWER){;}
        #endif	
        #endif
	    	else
	    	{
	    		HaveWorkFlag = TRUE;
	    	}
		}
	}

#if !defined(_YH1701_SPECIAL_MUTE_)
	if(HaveWorkFlag == FALSE)
	{
		if(global.aud_mute)
		{
			HaveWorkFlag = ir_power_filter(MSG_SYS_MUTE);
		}
	}
#endif

#if defined(_YH1701_SPECIAL_MUTE_)
	if(global.aud_mute)
	{
		if((GET_IR_IR == IR_SYSTEM_POWER)||(GET_IR_IR == IR_SYSTEM_MODE)||(GET_IR_IR ==IR_VALUE_MUTE)
		||(GET_IR_IR == IR_VALUE_ADD_VOL)||(GET_IR_IR == IR_VALUE_SUB_VOL))
		{
			if(HaveWorkFlag == FALSE)
			{
				OrderTypeProcess(
							ORDER_CFG_TYPE_IR,
							&Order.Ir,
							GET_IR_IR,
							GET_IR_STATE,
							GET_IR_TIME_UP_TIME,
							&GET_IR_TIME_DOWN_TIME
							);
			}
		}
	}
	else
	{
		if(HaveWorkFlag == FALSE)
		{
			OrderTypeProcess(
						ORDER_CFG_TYPE_IR,
						&Order.Ir,
						GET_IR_IR,
						GET_IR_STATE,
						GET_IR_TIME_UP_TIME,
						&GET_IR_TIME_DOWN_TIME
						);
		}
	}
#else
	if(HaveWorkFlag == FALSE)
	{
		OrderTypeProcess(
					ORDER_CFG_TYPE_IR,
					&Order.Ir,
					GET_IR_IR,
					GET_IR_STATE,
					GET_IR_TIME_UP_TIME,
					&GET_IR_TIME_DOWN_TIME
					);
	}
#endif
}

void order_init(void)
{
	order_puts("order_init\n");

	memset(&Order, 0, sizeof(Order));

	Order.Ir.LastBuffer = UP;
	Order.Key.LastBuffer = UP;
}

void order_project(uint8 project)
{
	order_printf("order_project:%d\n", project);
	Order.Project = project;
}

void order_select(uint8 select)
{
	//order_printf("order_select:%d\n", select);
	Order.Select = select;
}

