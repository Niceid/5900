#include "sdk_cfg.h"
#include "setup.h"
#include "fm_engine.h"
#include "tuner_drv.h"
#include "ui_res.h"
#include "ui_api.h"
#include "rds/rds_com.h"
#include "rds/rds_ctrl.h"


#if defined(_ENABLE_RADIO_RDS_)

#define RDS_CTRL_DEBUG
#ifdef RDS_CTRL_DEBUG
#define rds_ctrl_printf 		ew_printf
#define rds_ctrl_puts 			ew_puts
#else
#define rds_ctrl_printf(...)
#define rds_ctrl_puts(...)
#endif

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);

rds_app_t *rds_app = NULL;

static const uint8 rds_pty_music_tab[]=
{
	RDS_PTY_POP_M,
	RDS_PTY_ROCK_M,
	RDS_PTY_EASY_M,
	RDS_PTY_LIGTH_M,
	RDS_PTY_CLASSICS,
	RDS_PTY_OTHER_M,
	RDS_PTY_JAZZ,
	RDS_PTY_COUNTRY,
	RDS_PTY_NATION_M,
	RDS_PTY_OLDIES,
	RDS_PTY_FOLK_M,
};

static const uint8 rds_pty_speech_tab[]=
{
	RDS_PTY_NEWS,
	RDS_PTY_AFFAIRS,
	RDS_PTY_INFO,
	RDS_PTY_SPORT,
	RDS_PTY_EDUCATE,
	RDS_PTY_DRAMA,
	RDS_PTY_CULTURE,
	RDS_PTY_SCIENCE,
	RDS_PTY_VARIED,
	RDS_PTY_WEATHER,
	RDS_PTY_FINANCE,
	RDS_PTY_CHILDREN,
	RDS_PTY_SOCIAL,
	RDS_PTY_RELIGION,
	RDS_PTY_PHONE_IN,
	RDS_PTY_TRAVEL,
	RDS_PTY_LEISURE,
	RDS_PTY_DOCUMENT,
};

static const uint8 rds_pty_number_music_tab[6][2]=
{
	{RDS_PTY_POP_M,				RDS_PTY_ROCK_M			},
	{RDS_PTY_EASY_M,			RDS_PTY_LIGTH_M			},
	{RDS_PTY_CLASSICS,			RDS_PTY_OTHER_M			},
	{RDS_PTY_JAZZ,				RDS_PTY_COUNTRY			},
	{RDS_PTY_NATION_M,			RDS_PTY_OLDIES			},
	{RDS_PTY_FOLK_M,			0						},
};

static const uint8 rds_pty_number_speech_tab[6][3]=
{
	{RDS_PTY_NEWS,		RDS_PTY_AFFAIRS,	RDS_PTY_INFO		},
	{RDS_PTY_SPORT,		RDS_PTY_EDUCATE,	RDS_PTY_DRAMA		},
	{RDS_PTY_CULTURE,	RDS_PTY_SCIENCE,	RDS_PTY_VARIED		},
	{RDS_PTY_WEATHER,	RDS_PTY_FINANCE,	RDS_PTY_CHILDREN	},
	{RDS_PTY_SOCIAL,	RDS_PTY_RELIGION,	RDS_PTY_PHONE_IN	},
	{RDS_PTY_TRAVEL,	RDS_PTY_LEISURE,	RDS_PTY_DOCUMENT	},
};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
uint8 rds_cmd_tuner_rds_enable(uint8 enable, uint16 timer)
{
	typedef struct{
		uint8 enable;
		uint16 timer;
		uint8 result;
	}tuner_rds_ctrl_t;

	tuner_rds_ctrl_t rds_ctrl;
	rds_ctrl.enable = enable;
	rds_ctrl.timer 	= timer;
	rds_ctrl.result = FALSE;

	if(tuner_drv_func.tuner_setting(TUR_SET_RDS, &rds_ctrl))
	{
		return rds_ctrl.result;
	}

	return FALSE;
}

uint8 rds_cmd_tuner_freq(uint16 freq)
{
	rds_ctrl_printf("rds_tuner_freq:%d\n", freq);
	return tuner_drv_func.tuner_freq(freq);
}

uint8 rds_cmd_tuner_app(radio_rds_app_t *rds_app)
{
	if(tuner_drv_func.tuner_setting(TUR_SET_RDS_DECODE, NULL))
	{
		return tuner_drv_func.tuner_info(TUR_INFO_RDS_APP, rds_app);
	}
	return FALSE;
}

uint8 rds_cmd_tuner_info(radio_rds_info_t *rds_info)
{
	if(tuner_drv_func.tuner_setting(TUR_SET_RDS_DECODE, NULL))
	{
		return tuner_drv_func.tuner_info(TUR_INFO_RDS_INFO, rds_info);
	}
	return FALSE;
}

uint8 rds_cmd_reset(void)
{
	return tuner_drv_func.tuner_setting(TUR_SET_RDS_RESET, (void *)0);
}

uint8 rds_cmd_eon_enable(void)
{
	return tuner_drv_func.tuner_setting(TUR_SET_RDS_EON, (void *)0);
}

uint8 rds_cmd_eon_disable(void)
{
	return tuner_drv_func.tuner_setting(TUR_SET_RDS_EON, (void *)1);
}
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

enum{
	RDS_ENGINE_CMD_NULL = 0,
	RDS_ENGINE_CMD_FREQ,
	RDS_ENGINE_CMD_MUTE,
	RDS_ENGINE_CMD_TRAFFIC,
	RDS_ENGINE_CMD_ICON,
	RDS_ENGINE_CMD_TXT,
	RDS_ENGINE_CMD_PSN,
	RDS_ENGINE_CMD_DATE,
};

void rds_cmd_engine_freq(uint16 freq)
{
	if(rds_app->ext.func != NULL)
	{
		rds_app->ext.func(RDS_ENGINE_CMD_FREQ, (void *)(int)freq);
	}
}

void rds_cmd_engine_mute(uint8 mute)
{
	if(rds_app->ext.func != NULL)
	{
		rds_app->ext.func(RDS_ENGINE_CMD_MUTE, (void *)(int)mute);
	}
}

void rds_cmd_engine_traffic(uint8 traffic)
{
	if(rds_app->ext.func != NULL)
	{
		rds_app->ext.func(RDS_ENGINE_CMD_TRAFFIC, (void *)(int)traffic);
	}
}

static void rds_cmd_engine_icon(uint8 icon, uint8 ctrl)
{
	if(rds_app->ext.func != NULL)
	{
		int info = (ctrl<<8) | icon;
		rds_app->ext.func(RDS_ENGINE_CMD_ICON, (void *)info);
	}
}

void rds_cmd_engine_txt(uint8 txt)
{
	if(rds_app->ext.func != NULL)
	{
		if(txt < RDS_TXT_MAX)
		{
			uint8 alarm;
			if(txt <= RDS_TXT_EON_TA)
			{
				alarm = TRUE;
			}
			else
			{
				alarm = FALSE;
			}

			int txt_ctrl = (alarm << 8) | txt;
			rds_app->ext.func(RDS_ENGINE_CMD_TXT, (void *)txt_ctrl);
		}
	}
}

static void rds_cmd_engine_psn(void)
{
	if(rds_app->ext.func != NULL)
	{
		rds_app->ext.func(RDS_ENGINE_CMD_PSN, (void *)rds_app->ctrl.psn.buffer);
	}
}

static void rds_cmd_engine_data(void)
{
	if(rds_app->ext.func != NULL)
	{
		rds_app->ext.func(RDS_ENGINE_CMD_DATE, (void *)&rds_app->ctrl.date.year);
	}
}

static void rds_ctrl_icon_disp(void)
{
	uint8 index;
	uint8 now_icon;
	uint8 prev_icon;
	uint8 ctrl;

enum{
	RDS_ICON_CTRL_CLOSE = _B0_,
	RDS_ICON_CTRL_OPEN = _B1_,
	RDS_ICON_CTRL_FLASH_ENABLE = _B2_,
	RDS_ICON_CTRL_FLASH_DISABLE = _B3_,
};

	for(index = 0; index < RDS_ICON_MAX; index ++)
	{
		now_icon 	= rds_app->sys_ctrl.icon[index];
		prev_icon 	= rds_app->sys_ctrl.prev_icon[index];

		if(now_icon != prev_icon)
		{
			ctrl = 0;
			switch(now_icon)
			{
				case RDS_ICON_CLOSE:
					ctrl |= RDS_ICON_CTRL_CLOSE;
					if(prev_icon == RDS_ICON_FLASH)
					{
						ctrl |= RDS_ICON_CTRL_FLASH_DISABLE;
					}
					break;
				case RDS_ICON_OPEN:
					ctrl |= RDS_ICON_CTRL_OPEN;
					if(prev_icon == RDS_ICON_FLASH)
					{
						ctrl |= RDS_ICON_CTRL_FLASH_DISABLE;
					}
					break;
				case RDS_ICON_FLASH:
					ctrl |= RDS_ICON_CTRL_FLASH_ENABLE;
					break;
			}

			if(ctrl)
			{
				rds_cmd_engine_icon(index, ctrl);
			}
			rds_app->sys_ctrl.prev_icon[index] = now_icon;
		}
	}
}

void rds_ctrl_icon_sync(void)
{
	if(setup.rds.af)
	{
		if(rds_app->dec.af_det)
		{
			rds_app->sys_ctrl.icon[RDS_ICON_AF] = RDS_ICON_OPEN;
		}
		else
		{
			rds_app->sys_ctrl.icon[RDS_ICON_AF] = RDS_ICON_FLASH;
		}
	}
	else
	{
		rds_app->sys_ctrl.icon[RDS_ICON_AF] = RDS_ICON_CLOSE;
	}

	if(rds_app->dec.tp)
	{
		rds_app->sys_ctrl.icon[RDS_ICON_TP] = RDS_ICON_OPEN;
	}
	else
	{
		rds_app->sys_ctrl.icon[RDS_ICON_TP] = RDS_ICON_CLOSE;
	}

	if(setup.rds.ta)
	{
		if((rds_app->next_id == RDS_APP_ID_TRAFFIC)
		 ||((rds_app->next_id == RDS_APP_ID_ALARM)&&(rds_app->ctrl.alarm == RDS_ALARM_TRAFFIC)))
		{
		#if defined(_ENABLE_TA_ICON_)
		    rds_app->sys_ctrl.icon[RDS_ICON_TA] = RDS_ICON_OPEN;
		#else
			rds_app->sys_ctrl.icon[RDS_ICON_TA] = RDS_ICON_FLASH;
		#endif
		}
		else
		{
		#if defined(_ENABLE_TA_ICON_)
		    rds_app->sys_ctrl.icon[RDS_ICON_TA] = RDS_ICON_FLASH;
		#else
			rds_app->sys_ctrl.icon[RDS_ICON_TA] = RDS_ICON_OPEN;
		#endif
        #if defined(_DISABLE_TP_ICON_)
        	rds_app->sys_ctrl.icon[RDS_ICON_TP] = RDS_ICON_CLOSE;
        #endif
		}
	}
	else
	{
		rds_app->sys_ctrl.icon[RDS_ICON_TA] = RDS_ICON_CLOSE;
	}

	if(rds_app->pty_search.enable)
	{
		if(rds_app->next_id == RDS_APP_ID_PTY)
		{
			rds_app->sys_ctrl.icon[RDS_ICON_PTY] = RDS_ICON_FLASH;
		}
		else
		{
			rds_app->sys_ctrl.icon[RDS_ICON_PTY] = RDS_ICON_OPEN;
		}
	}
	else
	{
		rds_app->sys_ctrl.icon[RDS_ICON_PTY] = RDS_ICON_CLOSE;
	}

	if(rds_app->dec.af.succeed)
	{
		if(rds_app->dec.af.method > 1)
		{
			rds_app->sys_ctrl.icon[RDS_ICON_REG] = RDS_ICON_OPEN;
		}
		else
		{
			rds_app->sys_ctrl.icon[RDS_ICON_REG] = RDS_ICON_CLOSE;
		}
	}
	else
	{
		rds_app->sys_ctrl.icon[RDS_ICON_REG] = RDS_ICON_CLOSE;
	}

	if(setup.rds.eon && rds_app->dec.eon_det)
	{
		if((rds_app->next_id == RDS_APP_ID_EON)
		 ||((rds_app->next_id == RDS_APP_ID_ALARM)&&(rds_app->ctrl.alarm == RDS_ALARM_EON)))
		{
			rds_app->sys_ctrl.icon[RDS_ICON_EON] = RDS_ICON_FLASH;
		}
		else
		{
			rds_app->sys_ctrl.icon[RDS_ICON_EON] = RDS_ICON_OPEN;
		}
	}
	else
	{
		rds_app->sys_ctrl.icon[RDS_ICON_EON] = RDS_ICON_CLOSE;
	}

	rds_ctrl_icon_disp();
}

void rds_ctrl_psn_sync(void)
{
	if(rds_app->dec.psn.succeed)
	{
		if(memcmp(rds_app->ctrl.psn.buffer, rds_app->dec.psn.buffer, 8))
		{
			memcpy(rds_app->ctrl.psn.buffer, rds_app->dec.psn.buffer, 8);
			rds_cmd_engine_psn();
		}
	}
	else
	{
		static const uint8 psn_null_tab[]={0,0,0,0,0,0,0,0};
		if(memcmp(rds_app->ctrl.psn.buffer, psn_null_tab, 8))
		{
			memcpy(rds_app->ctrl.psn.buffer, psn_null_tab, 8);
			rds_cmd_engine_psn();
		}
	}
}


void rds_ctrl_data_sync(void)
{
#if !defined(_DIS_RDS_TIME_CHECK_)
	if(!setup.rds.time)return;
	if(!rds_app->dec.date.succeed)return;
	
	radio_rds_date_t dec_date = rds_app->dec.date;
	radio_rds_date_t *ctrl_date = &rds_app->ctrl.date;
	
	if((dec_date.year  < 1900)||(dec_date.year	> 2099))
	{
		dec_date.year = 2000;
	}
	if((dec_date.month < 1)  ||(dec_date.month > 12))
	{
		dec_date.month = 1;
	}
	if((dec_date.day   < 1)  ||(dec_date.day   > 31))
	{
		dec_date.day = 1;
	}
	if((dec_date.hour  < 0)  ||(dec_date.hour  > 23))
	{
		dec_date.hour = 0;
	}
	if((dec_date.min   < 0)  ||(dec_date.min   > 59))
	{
		dec_date.min = 0;
	}
	
	if((ctrl_date->year  != dec_date.year)
	 ||(ctrl_date->month != dec_date.month)
	 ||(ctrl_date->day	 != dec_date.day)
	 ||(ctrl_date->hour  != dec_date.hour)
	 ||(ctrl_date->min	 != dec_date.min))
	{
		*ctrl_date = dec_date;
		rds_cmd_engine_data();
	}
#endif
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
uint8 rds_ctrl_enable(void)
{
	if(setup.rds.enable)
	{
		if(rds_app != NULL)
		{
			if(rds_app->enable)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

uint8 rds_ctrl_enter(uint16 timer, void **func, void *ext_func)
{
	rds_ctrl_puts("rds_ctrl_enter\n");
	if(!setup.rds.enable)return FALSE;

	if(rds_app == NULL)
	{
		rds_app = (rds_app_t *)malloc_fun(rds_app ,sizeof(rds_app_t), 0);
		if(rds_app == NULL)
		{
			rds_ctrl_puts("rds_ctrl_enter err\n");
	        return FALSE;
		}
	}

	if(!rds_app->enable)
	{
		if(rds_cmd_tuner_rds_enable(ENABLE, timer))
		{
			if(setup.rds.eon)
			{
				rds_cmd_eon_enable();
			}
			else
			{
				rds_cmd_eon_disable();
			}
			*func = rds_app_polling;
			rds_app->ctrl.traffic_interval = RDS_INTERVAL_TRAFFIC_MIN_TIME;
			rds_app->ctrl.eon_interval = RDS_INTERVAL_EON_MIN_TIME;
			rds_app->count = timer;
			rds_app->ext.func = (rds_ext_func)ext_func;
			rds_app->enable = TRUE;
			rds_ctrl_puts("rds_ctrl_enter succeed--1---\n");
			return TRUE;
		}
		else
		{
	    #if defined(_SPECIAL_RDS_DELAY_)
	    	OSTimeDly(15);
	    #endif
			free_fun((void**)&rds_app);
			rds_ctrl_puts("rds_ctrl_enter succeed--2---\n");
		}
	}
	else
	{
		*func = rds_app_polling;
		rds_app->count = timer;
		rds_app->ext.func = (rds_ext_func)ext_func;
		rds_ctrl_puts("rds_ctrl_enter succeed---3---\n");
		return TRUE;
	}

	rds_ctrl_puts("rds_ctrl_enter fail\n");
	return FALSE;
}

uint8 rds_ctrl_exit(void)
{
	if(rds_app == NULL)return FALSE;

	uint8 result = FALSE;
	if(rds_app->enable)
	{
		rds_app_reset();
		
		if(rds_app->pty_search.enable)
		{
			os_taskq_post_msg(FM_TASK_NAME, 2, MSG_FM_RDS_PTY, DISABLE);
			rds_app->pty_search.enable = FALSE;
		}
		
		memset(rds_app->sys_ctrl.icon, RDS_ICON_CLOSE, sizeof(rds_app->sys_ctrl.icon));
		rds_ctrl_icon_disp();
		
		rds_cmd_eon_disable();
		rds_cmd_tuner_rds_enable(DISABLE, 0);
		result = TRUE;
		rds_ctrl_puts("rds_ctrl_exit succeed\n");
	}

#if defined(_SPECIAL_RDS_DELAY_)
	OSTimeDly(15);
#endif
	free_fun((void**)&rds_app);
	return result;
}

void rds_ctrl_reset(void)
{
	if(rds_app != NULL)
	{
		if(rds_app->enable)
		{
			rds_app_reset();
			rds_app->ctrl.traffic_interval = RDS_INTERVAL_TRAFFIC_MIN_TIME;
			rds_app->ctrl.eon_interval = RDS_INTERVAL_EON_MIN_TIME;
			rds_ctrl_puts("rds_ctrl_reset succeed\n");
		}
	}
}

void rds_ctrl_busy_func(void *busy)
{
	if(rds_ctrl_enable())
	{
		rds_app->ext.busy = (rds_ext_busy)busy;
	}
}

void rds_ctrl_fast_det(void)
{
	if(rds_ctrl_enable())
	{
		rds_app->ext.fast_det = TRUE;
	}
}

void rds_ctrl_ta_disable(uint8 disable)
{
	if(rds_ctrl_enable())
	{
		if(disable != rds_app->ext.ta_disable)
		{
			if(setup.rds.ta)
			{
				if(disable)
				{
					if((rds_app->id == RDS_APP_ID_TA)
					 ||(rds_app->id == RDS_APP_ID_EON)
					 ||(rds_app->id == RDS_APP_ID_TRAFFIC))
					{
						rds_app->ctrl.exit = TRUE;
					}
					else if((rds_app->next_id == RDS_APP_ID_TA)
						  ||(rds_app->next_id == RDS_APP_ID_EON)
						  ||(rds_app->next_id == RDS_APP_ID_TRAFFIC))
					{
						rds_app->next_id = RDS_APP_ID_IDLE;
					}
					
					rds_app->ctrl.time 				= 0;
					rds_app->ctrl.traffic_interval	= RDS_INTERVAL_TRAFFIC_MIN_TIME;
					rds_app->ctrl.eon_interval		= RDS_INTERVAL_EON_MIN_TIME;
					rds_app->ctrl.pause_interval 	= 0;
					rds_app->ctrl.af_count 			= 0;
					rds_app->ctrl.pause_ta			= FALSE;
					rds_app->ctrl.pause_eon			= FALSE;
					
				    rds_ctrl_icon_sync();
				}
				else
				{
					
				}
			}
			rds_app->ext.ta_disable = disable;
		}
	}
}

void rds_ctrl_ext_info(void *ext_info)
{
	typedef struct{
		uint16 freq;
		uint8 psn[8];
		uint8 pty;
	}ext_info_t;

	ext_info_t *info = (ext_info_t *)ext_info;

	info->freq = rds_app->dec.freq;
	info->pty = rds_app->dec.pty;
	memcpy(info->psn, rds_app->ctrl.psn.buffer, 8);
}

uint8 rds_ctrl_pause(void)
{
	if(!rds_ctrl_enable())return FALSE;

	rds_ctrl_printf("%s, rds_id:%d\n", __FUNCTION__, rds_app->id);
	if(rds_app->id == RDS_APP_ID_EON)
	{
		rds_app->ctrl.pause_eon = TRUE;
	}
	else if(rds_app->id == RDS_APP_ID_TRAFFIC)
	{
		rds_app->ctrl.pause_ta = TRUE;
	}
	else if(rds_app->id == RDS_APP_ID_ALARM)
	{
		if(rds_app->ctrl.alarm == RDS_ALARM_EON)
		{
			rds_app->ctrl.pause_eon = TRUE;
		}
		else if(rds_app->ctrl.alarm == RDS_ALARM_TRAFFIC)
		{
			rds_app->ctrl.pause_ta = TRUE;
		}
		else return FALSE;
	}
	else return FALSE;

	rds_app->ctrl.pause_interval = RDS_INTERVAL_PAUSE_MIN_TIME;
	rds_app->ctrl.exit = TRUE;

	return TRUE;
}

uint8 rds_info_found(uint16 *pi)
{
	uint8 found = RDS_FOUND_WAIT;
	radio_rds_info_t rds_info;

	if(rds_cmd_tuner_info(&rds_info))
	{
		*pi = rds_info.pi;
		if(rds_info.pi)
		{
			found = RDS_FOUND_SUCCEED;
		}

		if(setup.rds.ta)
		{
			if((rds_info.ta_have) && (rds_info.ta || rds_info.tp))
			{

			}
			else
			{
				found = RDS_FOUND_WAIT;
			}
		}
	}

	return found;
}

uint8 rds_app_state(void)
{
	if(!rds_ctrl_enable())return RADIO_RDS_IDLE;

	switch(rds_app->id)
	{
		default:
		case RDS_APP_ID_IDLE:
		case RDS_APP_ID_AF:
			return RADIO_RDS_IDLE;

		case RDS_APP_ID_TA:
			if(rds_app->mode == RDS_APP_MODE_RUN)
			{
				return RADIO_RDS_TA_SEEK;
			}
			break;

		case RDS_APP_ID_PI:
			if(rds_app->mode == RDS_APP_MODE_RUN)
			{
				return RADIO_RDS_PI_SEEK;
			}
			break;

		case RDS_APP_ID_PTY:
			if(rds_app->mode == RDS_APP_MODE_RUN)
			{
				return RADIO_RDS_PTY_SEARCH;
			}
			break;

		case RDS_APP_ID_EON:
			if(rds_app->mode == RDS_APP_MODE_RUN)
			{
				if(RDS_APP_EON.State == RDS_APP_EON_STATE_IDLE)
				{
					return RADIO_RDS_EON;
				}
			}
			break;

		case RDS_APP_ID_TRAFFIC:
			if(rds_app->mode == RDS_APP_MODE_RUN)
			{
				if(RDS_APP_TRAFFIC.State == RDS_APP_TRAFFIC_STATE_IDLE)
				{
					return RADIO_RDS_TA;
				}
			}
			break;

		case RDS_APP_ID_ALARM:
			if(rds_app->mode == RDS_APP_MODE_RUN)
			{
				return RADIO_RDS_ALARM;
			}
			break;
	}

	return RADIO_RDS_IDLE;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static void rds_ctrl_af(void)
{
#if !defined(_SL6812A_MENU_SET_)
	setup.rds.af = !setup.rds.af;
#endif

	rds_ctrl_printf("%s, af:%d\n", __FUNCTION__, setup.rds.af);

	if(!setup.rds.af)
	{
		if(rds_app->id == RDS_APP_ID_PI)
		{
			rds_app->ctrl.exit = TRUE;
		}
	}
	rds_app->ctrl.time = 0;
	rds_app->ctrl.af_count = 0;
	rds_ctrl_icon_sync();
}

static void rds_ctrl_ta(void)
{
	rds_ctrl_printf("%s, ta:%d\n", __FUNCTION__, setup.rds.ta);
#if 0
	if(rds_ctrl_pause())
	{
		rds_app->ctrl.pause_disp = TRUE;
		return;
	}
#endif

#if !defined(_SL6812A_MENU_SET_)
	setup.rds.ta = !setup.rds.ta;
#endif

	if(!setup.rds.ta)
	{
		if((rds_app->id == RDS_APP_ID_TA)||(rds_app->id == RDS_APP_ID_EON)||(rds_app->id == RDS_APP_ID_TRAFFIC))
		{
			rds_app->ctrl.exit = TRUE;
		}
	}

	rds_app->ctrl.time 				= 0;
	rds_app->ctrl.traffic_interval	= RDS_INTERVAL_TRAFFIC_MIN_TIME;
	rds_app->ctrl.eon_interval		= RDS_INTERVAL_EON_MIN_TIME;
	rds_app->ctrl.pause_interval 	= 0;
	rds_app->ctrl.af_count 			= 0;
	rds_app->ctrl.pause_ta			= FALSE;
	rds_app->ctrl.pause_eon			= FALSE;

    rds_ctrl_icon_sync();
}

static void rds_ctrl_reg(void)
{
#if !defined(_SL6812A_MENU_SET_)
#if !defined(_RDS_KEY_FUNTION_)
	setup.rds.reg = !setup.rds.reg;
#endif
#endif
	rds_ctrl_printf("%s, reg:%d\n", __FUNCTION__, setup.rds.reg);
}

static void rds_ctrl_eon(void)
{
#if !defined(_RDS_KEY_FUNTION_)
	setup.rds.eon = !setup.rds.eon;
#endif
	rds_ctrl_printf("%s, eon:%d\n", __FUNCTION__, setup.rds.eon);

	if(setup.rds.eon)
	{
		if(rds_app->id != RDS_APP_ID_EON)
		{
			rds_cmd_eon_enable();
		}
	}
	else
	{
		if(rds_app->id == RDS_APP_ID_EON)
		{
			rds_app->ctrl.exit = TRUE;
		}
		else
		{
			rds_cmd_eon_disable();

			if(rds_app->id == RDS_APP_ID_ALARM)
			{
				if(rds_app->ctrl.alarm == RDS_ALARM_EON)
				{
					rds_app->ctrl.exit = TRUE;
				}
			}
		}
	}
}

static void rds_ctrl_long_short(void)
{
	setup.rds.long_short = !setup.rds.long_short;
	rds_ctrl_printf("%s, long_short:%d\n", __FUNCTION__, setup.rds.long_short);
}

static void rds_ctrl_mask(void)
{
	setup.rds.mask = !setup.rds.mask;
	rds_ctrl_printf("%s, mask:%d\n", __FUNCTION__, setup.rds.mask);
}

static void rds_ctrl_alarm(void)
{
	setup.rds.alarm = !setup.rds.alarm;
	rds_ctrl_printf("%s, alarm:%d\n", __FUNCTION__, setup.rds.alarm);

	if(setup.rds.alarm)
	{
		if(rds_app->ctrl.alarm != RDS_ALARM_NULL)
		{
			if((rds_app->id == RDS_APP_ID_TA)
			 ||(rds_app->id == RDS_APP_ID_EON)
			 ||(rds_app->id == RDS_APP_ID_TRAFFIC))
			{
				goto rds_alarm_exit;
			}
		}
	}
	else
	{
		if(rds_app->id == RDS_APP_ID_ALARM)
		{
			rds_app->ctrl.traffic_interval	= RDS_INTERVAL_TRAFFIC_MIN_TIME;
			rds_app->ctrl.eon_interval		= RDS_INTERVAL_EON_MIN_TIME;
			rds_app->ctrl.pause_interval 	= 0;

		rds_alarm_exit:
			rds_app->ctrl.af_count 		= 0;
			rds_app->ctrl.pause_ta		= FALSE;
			rds_app->ctrl.pause_eon		= FALSE;
			rds_app->ctrl.exit 			= TRUE;
		}
	}
}

static void rds_ctrl_time(void)
{
	setup.rds.time = !setup.rds.time;
	rds_ctrl_printf("%s, time:%d\n", __FUNCTION__, setup.rds.time);
}

static void rds_ctrl_pty(uint8 ctrl_type)
{
	rds_pty_search_t *pty = &rds_app->pty_search;
	uint8 i,j;
	uint8 max;
	const uint8 *tab;
	
	rds_ctrl_printf("rds_ctrl_pty:%d\n", ctrl_type);
	switch(ctrl_type)
	{
		case RADIO_RDS_CTRL_ADD:
			switch(pty->search)
			{
				default:
				case RDS_PTY_SEARCH_OFF:
				#if defined(TD_ADD_NEW_APP)
					setup.pty_type = 0;
				#endif
					pty->search = RDS_PTY_SEARCH_SPEECH;
					pty->buffer = rds_pty_speech_tab;
					pty->length = sizeof(rds_pty_speech_tab);
					break;
				case RDS_PTY_SEARCH_SPEECH:
					pty->search = RDS_PTY_SEARCH_MUSIC;
					pty->buffer = rds_pty_music_tab;
					pty->length = sizeof(rds_pty_music_tab);
					break;
				case RDS_PTY_SEARCH_MUSIC:
					pty->search = RDS_PTY_SEARCH_OFF;
					break;
			}
		#if defined(TD_ADD_NEW_APP)
			global.pty_type = pty->search;
		#endif
			break;
			
		case RADIO_RDS_CTRL_SUB:
			switch(pty->search)
			{
				default:
				case RDS_PTY_SEARCH_OFF:
					pty->search = RDS_PTY_SEARCH_MUSIC;
					pty->buffer = rds_pty_music_tab;
					pty->length = sizeof(rds_pty_music_tab);
					break;
				case RDS_PTY_SEARCH_MUSIC:
					pty->search = RDS_PTY_SEARCH_SPEECH;
					pty->buffer = rds_pty_speech_tab;
					pty->length = sizeof(rds_pty_speech_tab);
					break;
				case RDS_PTY_SEARCH_SPEECH:
					pty->search = RDS_PTY_SEARCH_OFF;
					
					break;
			}
			break;
			
		case RADIO_RDS_CTRL_NEXT:
			if(pty->search == RDS_PTY_SEARCH_MUSIC)
			{
				max = sizeof(rds_pty_music_tab);
				tab = rds_pty_music_tab;
			}
			else if(pty->search == RDS_PTY_SEARCH_SPEECH)
			{
				max = sizeof(rds_pty_speech_tab);
				tab = rds_pty_speech_tab;
			}
			else break;
			
			for(i = 0; i < max; i++)
			{
				if(pty->buffer == &(tab[i]))
				{
					break;
				}
			}
			if(i == max)
			{
				pty->buffer = &(tab[0]);
			}
			else
			{
				for(j = 0; j < max; j++)
				{
					if(++i >= max)i = 0;
					
					if(tab[i])
					{
						break;
					}
				}
				pty->buffer = &(tab[i]);
			}
			pty->length = 1;
	    #if defined(TD_ADD_NEW_APP)
			setup.pty_type = pty->buffer[0];
	    	//printf("pty->buffer1:0x%x\n",pty->buffer[0]);
	    #endif
			break;

		case RADIO_RDS_CTRL_PREV:
			if(pty->search == RDS_PTY_SEARCH_MUSIC)
			{
				max = sizeof(rds_pty_music_tab);
				tab = rds_pty_music_tab;
			}
			else if(pty->search == RDS_PTY_SEARCH_SPEECH)
			{
				max = sizeof(rds_pty_speech_tab);
				tab = rds_pty_speech_tab;
			}
			else break;
			
			for(i = 0; i < max; i++)
			{
				if(pty->buffer == &(tab[i]))
				{
					break;
				}
			}
			if(i == max)
			{
				pty->buffer = &(tab[0]);
			}
			else
			{
				for(j = 0; j < max; j++)
				{
					if(i)i--;
					else
					{
						i = max - 1;
					}
					if(tab[i])
					{
						break;
					}
				}
				pty->buffer = &(tab[i]);
			}
			pty->length = 1;
	    #if defined(TD_ADD_NEW_APP)
			setup.pty_type = pty->buffer[0];
	    	//printf("pty->buffer2:0x%x\n",pty->buffer[0]);
	    #endif
			break;
			
		case RADIO_RDS_CTRL_M1 ... RADIO_RDS_CTRL_M6:
			if(pty->search == RDS_PTY_SEARCH_MUSIC)
			{
				max = 2;
				tab = rds_pty_number_music_tab[ctrl_type - RADIO_RDS_CTRL_M1];
			}
			else if(pty->search == RDS_PTY_SEARCH_SPEECH)
			{
				max = 3;
				tab = rds_pty_number_speech_tab[ctrl_type - RADIO_RDS_CTRL_M1];
			}
			else break;
			
			for(i = 0; i < max; i++)
			{
				if(pty->buffer == &(tab[i]))
				{
					break;
				}
			}
			if(i == max)
			{
				pty->buffer = &(tab[0]);
			}
			else
			{
				for(j = 0; j < max; j++)
				{
					if(++i >= max)i = 0;
					
					if(tab[i])
					{
						break;
					}
				}
				pty->buffer = &(tab[i]);
			}
			pty->length = 1;
		
		#if defined(TD_ADD_NEW_APP)
			setup.pty_type = pty->buffer[0];
			//printf("pty->buffer:0x%x\n",pty->buffer[0]);
		#endif
			break;
		
	#if defined(TD_ADD_NEW_APP)
		case RADIO_RDS_CTRL_PTY:
			pty->length = 1;
			pty->buffer = &global.save_rds_pty1;
			break;
	#endif
	}
	
	if(pty->search != RDS_PTY_SEARCH_OFF)
	{
    #if defined(_SEEK_REFRESH_FREQ_)
    	global.pty_flag = 1;
    #endif
		pty->enable = TRUE;
		pty->up_down = UP;
		pty->cmd = PTY_SEARCH_CMD_ON;
		os_taskq_post_msg(FM_TASK_NAME, 2, MSG_FM_RDS_PTY, ENABLE);
	}
	else
	{
    #if defined(_SEEK_REFRESH_FREQ_)
    	global.pty_flag = 0;
    #endif
		pty->enable = FALSE;
		pty->cmd = PTY_SEARCH_CMD_OFF;
		os_taskq_post_msg(FM_TASK_NAME, 2, MSG_FM_RDS_PTY, DISABLE);
	}
	
	rds_ctrl_icon_sync();
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

uint8 radio_rds_check(void)
{
	if(rds_ctrl_enable())
	{
		if(rds_app->ext.busy != NULL)
		{
			if(rds_app->ext.busy())return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

uint8 radio_rds_lang(func_sel_e func_sel, ui_lang_id_e *lang_id)
{
	if(!radio_rds_check())return FALSE;

	switch(func_sel)
	{
		case FUNC_RDS_AF:
			if(setup.rds.af)
			{
				*lang_id = LANG_ID_RDS_AF_ON;
			}
			else
			{
				*lang_id = LANG_ID_RDS_AF_OFF;
			}
			break;

		case FUNC_RDS_TA:
		#if 0
			if(rds_app->ctrl.pause_disp)
			{
				rds_app->ctrl.pause_disp = FALSE;
				*lang_id = LANG_ID_RDS_TA_PAUSE;
			}
			else
		#endif
			{
				if(setup.rds.ta)
				{

					*lang_id = LANG_ID_RDS_TA_ON;
				}
				else
				{
					*lang_id = LANG_ID_RDS_TA_OFF;
				}
			}
			break;

		case FUNC_RDS_REG:
			if(setup.rds.reg)
			{
				*lang_id = LANG_ID_RDS_REG_LOC;
			}
			else
			{
				*lang_id = LANG_ID_RDS_REG_DX;
			}
			break;

		case FUNC_RDS_EON:
			if(setup.rds.eon)
			{
				*lang_id = LANG_ID_RDS_EON_ON;
			}
			else
			{
				*lang_id = LANG_ID_RDS_EON_OFF;
			}
			break;

		case FUNC_RDS_L_S:
			if(setup.rds.long_short)
			{
				*lang_id = LANG_ID_RDS_RETURN_LONG;
			}
			else
			{
				*lang_id = LANG_ID_RDS_RETURN_SHORT;
			}
			break;

		case FUNC_RDS_MASK:
			if(setup.rds.mask)
			{
				*lang_id = LANG_ID_RDS_MASK_ALL;
			}
			else
			{
				*lang_id = LANG_ID_RDS_MASK_DIP;
			}
			break;

		case FUNC_RDS_ALARM:
			if(setup.rds.alarm)
			{
				*lang_id = LANG_ID_RDS_ALARM_ON;
			}
			else
			{
				*lang_id = LANG_ID_RDS_ALARM_OFF;
			}
			break;

		case FUNC_RDS_TIME:
			if(setup.rds.time)
			{
				*lang_id = LANG_ID_RDS_TIME_SYNC;
			}
			else
			{
				*lang_id = LANG_ID_RDS_TIME_ASYNC;
			}
			break;
			
		case FUNC_RDS_PTY:
			{
				rds_pty_search_t *pty = &rds_app->pty_search;
				
	    		switch(pty->search)
	    		{
	    			default:
	    			case RDS_PTY_SEARCH_OFF:
	    				*lang_id = LANG_ID_RDS_PTY_OFF;
	    				break;
						
	    			case RDS_PTY_SEARCH_MUSIC:
						if(pty->length == 1)
						{
							*lang_id = pty->buffer[0] + LANG_ID_RDS_PTY_0;
						}
						else
						{
							*lang_id = LANG_ID_RDS_PTY_MUSIC;
						}
	    				break;
						
	    			case RDS_PTY_SEARCH_SPEECH:
						if(pty->length == 1)
						{
							*lang_id = pty->buffer[0] + LANG_ID_RDS_PTY_0;
						}
						else
						{
							*lang_id = LANG_ID_RDS_PTY_SPEECH;
						}
	    				break;
	    		}
			}
            break;
    #if defined(ENABLE_TA_VOL_SET)
    	case FUNC_RDS_TA_VOL:
    		*lang_id = LANG_ID_RDS_TA_VOL;
    		break;
    #endif
		default:
			return FALSE;
	}

	return TRUE;
}

uint8 radio_rds_ctrl(func_sel_e func_sel, uint8 ctrl_type)
{
	if(!radio_rds_check())return FALSE;

	switch(func_sel)
	{
		case FUNC_RDS_AF:
			rds_ctrl_af();
			break;

		case FUNC_RDS_TA:
			rds_ctrl_ta();
			break;

		case FUNC_RDS_REG:
			rds_ctrl_reg();
			break;

		case FUNC_RDS_EON:
			rds_ctrl_eon();
			break;

		case FUNC_RDS_L_S:
			rds_ctrl_long_short();
			break;

		case FUNC_RDS_MASK:
			rds_ctrl_mask();
			break;

		case FUNC_RDS_ALARM:
			rds_ctrl_alarm();
			break;

		case FUNC_RDS_TIME:
			rds_ctrl_time();
			break;

		case FUNC_RDS_PTY:
			rds_ctrl_pty(ctrl_type);
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

uint8 radio_rds_sel(func_sel_e func_sel)
{
	if(!radio_rds_check())return FALSE;

	switch(func_sel)
	{
		case FUNC_RDS_AF:
		case FUNC_RDS_TA:
		case FUNC_RDS_REG:
		case FUNC_RDS_L_S:
		case FUNC_RDS_MASK:
		case FUNC_RDS_TIME:
    #if defined(ENABLE_TA_VOL_SET)
		case FUNC_RDS_TA_VOL:
    #endif
			break;

		case FUNC_RDS_EON:
        #if defined(_DAOAN_RDS_DISPLAY_)
        	break;
        #endif
		case FUNC_RDS_ALARM:
		#if !defined(_ENABLE_RDS_DISPLAY_)
			if(!setup.rds.ta)
			{
				return FALSE;
			}
		#endif
			break;

		case FUNC_RDS_PTY:
			if(rds_app->ext.busy == NULL)
			{
				return FALSE;
			}
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

#endif

