#include "sdk_cfg.h"
#include "setup.h"
#include "rds/rds_com.h"

#define RDS_APP_DEBUG
#ifdef RDS_APP_DEBUG
#define rds_app_printf 			ew_printf
#define rds_app_puts 			ew_puts
#else
#define rds_app_printf(...)
#define rds_app_puts(...)
#endif


#if defined(_ENABLE_RADIO_RDS_)

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static uint16 rds_af_to_freq(uint8 af_freq)
{
	return ((((uint16)af_freq) + 875) * 10);
}

static uint8 rds_freq_to_af(uint16 freq)
{
	return ((uint8)(freq / 10 - 875));
}

static uint8 rds_signal_detect(void)
{
	if(rds_app->dec.signal > 0x80)return TRUE;
	return FALSE;
}

static void rds_save_freq(uint8 af_freq)
{
	rds_cmd_engine_freq(rds_af_to_freq(af_freq));
}

static void rds_set_af_freq(uint8 af_freq)
{
	if(af_freq != rds_freq_to_af(rds_app->dec.freq))
	{
		rds_cmd_tuner_freq(rds_af_to_freq(af_freq));
		memset(&rds_app->dec, 0, sizeof(rds_app->dec));
	}
}

static uint8 rds_get_af_freq(void)
{
	return rds_freq_to_af(rds_app->dec.freq);
}

static uint8 rds_found_state(uint8 *found,uint8 *level)
{
	if(rds_app->dec.stc)
	{
		*found = rds_app->dec.found;
		*level = rds_app->dec.level;
	}
	return rds_app->dec.stc;
}

static void rds_freq_resumt(uint8 now_freq, uint8 orig_freq)
{
	if(now_freq != orig_freq)
	{
		rds_set_af_freq(orig_freq);
	}

	if(rds_app->ctrl.found)
	{
		rds_app->ctrl.backup_disable = FALSE;
		rds_save_freq(orig_freq);
	}
}

static void rds_mute_audio(uint8 mute)
{
	rds_cmd_engine_mute(mute);
}

static uint8 rds_ctrl_eon_check(void)
{
	uint8 index;
	radio_rds_eon_t *now_eon;

	if(rds_app->dec.pi)
	{
		for(index = 0; index < _RDS_EON_LIST_MAX_; index++)
		{
			now_eon = &rds_app->dec.eon[index];

			if(now_eon->pi)
			{
				if(now_eon->ta && now_eon->tp)
				{
					if(now_eon->af.succeed || now_eon->mapped.length)
					{
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

static uint8 rds_ctrl_af_list(uint8 index, radio_rds_af_t *af, uint8 reg)
{
enum{
	RDS_AF_METHOD_NULL = 0,
	RDS_AF_METHOD_A,
	RDS_AF_METHOD_B,
	RDS_AF_METHOD_B_2,
};

	uint8 now_freq;
	uint8 *p;
	uint8 i;

	if(af->succeed)
	{
		if(af->method == RDS_AF_METHOD_A)
		{
			if((index < _RDS_AF_LIST_MAX_)&&(af->buffer[index]))
			{
				return af->buffer[index];
			}
		}
		else if((af->method == RDS_AF_METHOD_B)||(af->method == RDS_AF_METHOD_B_2))
		{
			now_freq = af->buffer[0];
			p = &(af->buffer[1]);

			if((index < ((_RDS_AF_LIST_MAX_-1)/2))&& now_freq)
			{
				for(i = 0; i < index; i++)
				{
					if(p[0] && p[1])
					{
						if(reg)
						{
							if(p[0] > p[1])
							{
								p+=2;
								continue;
							}
						}
					}
					else break;
					p+=2;
				}

				if(i == index)
				{
					if(p[0] && p[1])
					{
						if(now_freq == p[0])
						{
							return p[1];
						}
						else
						{
							return p[0];
						}
					}
				}
			}
		}
	}
	return 0;
}

static void rds_ctrl_dec_backup(void)
{
	if(rds_app->ctrl.backup_disable)return;
	
	if(rds_app->dec.pi)
	{
		rds_app->backup.pi 	= rds_app->dec.pi;
		rds_app->backup.pty = rds_app->dec.pty;
		rds_app->backup.ms	= rds_app->dec.ms;
		
		if(rds_app->dec.psn.succeed)
		{
			memcpy(&rds_app->backup.psn, &rds_app->dec.psn, sizeof(rds_app->backup.psn));
		}
		else
		{
			rds_app->backup.psn.succeed = FALSE;
		}
		
		if(rds_app->dec.af.succeed)
		{
			memcpy(&rds_app->backup.af, &rds_app->dec.af, sizeof(rds_app->backup.af));
		}
		else
		{
			rds_app->backup.af.succeed = FALSE;
		}
		
		rds_app->ctrl.backup_disable = TRUE;
	}
}

static uint8 rds_ctrl_pty_delay(void)
{
	rds_pty_search_t *pty = &rds_app->pty_search;
	
	if(pty->delay)
	{
		if(pty->delay >= rds_app->count)
		{
			pty->delay -= rds_app->count;
		}
		else
		{
			pty->delay = 0;
		}
		if(pty->delay)return FALSE;
	}
	return TRUE;
}

static void rds_ctrl_pty_check(void)
{
	rds_pty_search_t *pty = &rds_app->pty_search;

	if(pty->cmd != PTY_SEARCH_CMD_NULL)
	{
		uint8 next_cmd = PTY_SEARCH_CMD_NULL;
		
		switch(pty->cmd)
		{
			case PTY_SEARCH_CMD_ON:
				rds_app_puts("PTY_SEARCH_CMD_ON\n");
				pty->delay = 2000;
				next_cmd = PTY_SEARCH_RUN_ON;
				break;
				
			case PTY_SEARCH_CMD_OFF:
				rds_app_puts("PTY_SEARCH_CMD_OFF\n");
				pty->delay = 0;
				if(rds_app->id == RDS_APP_ID_PTY)
				{
					rds_app->ctrl.exit = TRUE;
				}
				pty->re_start = FALSE;
				break;
				
			case PTY_SEARCH_CMD_FOUND:
				rds_app_puts("PTY_SEARCH_CMD_FOUND\n");
				pty->delay = 2000;
				next_cmd = PTY_SEARCH_RUN_RESULT;
				rds_cmd_engine_txt(RDS_TXT_FOUND);
				break;
				
			case PTY_SEARCH_CMD_NO_FOUND:
				rds_app_puts("PTY_SEARCH_CMD_NO_FOUND\n");
				pty->delay = 2000;
				next_cmd = PTY_SEARCH_RUN_RESULT;
				rds_cmd_engine_txt(RDS_TXT_NO_FOUND);
				break;
				
			case PTY_SEARCH_RUN_ON:
				if(!rds_ctrl_pty_delay())return;
				rds_app_puts("PTY_SEARCH_RUN_ON\n");
				switch(rds_app->id)
				{
					default:
					case RDS_APP_ID_IDLE:
						rds_app->next_id = RDS_APP_ID_PTY;
						break;
						
					case RDS_APP_ID_AF:
					case RDS_APP_ID_PTY:
						pty->re_start = TRUE;
						break;
						
					case RDS_APP_ID_TA:
					case RDS_APP_ID_PI:
					case RDS_APP_ID_EON:
					case RDS_APP_ID_TRAFFIC:
					case RDS_APP_ID_ALARM:
						rds_app->ctrl.exit = TRUE;
						pty->re_start = TRUE;
						break;
				}
				break;
				
			case PTY_SEARCH_RUN_RESULT:
				if(!rds_ctrl_pty_delay())return;
				rds_app_puts("PTY_SEARCH_RUN_RESULT\n");
				rds_cmd_engine_txt(RDS_TXT_NULL);
				break;
		}
		
		if(next_cmd != PTY_SEARCH_CMD_NULL)
		{
			pty->cmd = next_cmd;
		}
		else
		{
			pty->cmd = PTY_SEARCH_CMD_NULL;
		}
	}
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

static uint8 rds_return_time(void)
{
#if defined(_ENABLE_RDS_TIME_)
	uint8 titck = (RDS_SIGNAL_DETECT_TIME/300) + 1;
#else
	uint8 titck = (RDS_SIGNAL_DETECT_TIME/1000) + 1;
#endif

	if(setup.rds.long_short)
	{
		return (90/titck);
	}
	else
	{
		return (30/titck);
	}
}

static void rds_app_interval_detect(void)
{
	if(rds_app->ctrl.traffic_interval > rds_app->count)
	{
		rds_app->ctrl.traffic_interval -= rds_app->count;
	}
	else
	{
		rds_app->ctrl.traffic_interval = 0;
	}

	if(rds_app->ctrl.eon_interval > rds_app->count)
	{
		rds_app->ctrl.eon_interval -= rds_app->count;
	}
	else
	{
		rds_app->ctrl.eon_interval = 0;
	}

	if(rds_app->ctrl.pause_interval > rds_app->count)
	{
		rds_app->ctrl.pause_interval -= rds_app->count;
	}
	else
	{
		rds_app->ctrl.pause_interval = 0;

		if(rds_app->ctrl.pause_ta && (!rds_ta_ok()))
		{
			rds_app->ctrl.pause_ta = FALSE;
		}
		if(rds_app->ctrl.pause_eon && (!rds_eon_ok()))
		{
			rds_app->ctrl.pause_eon = FALSE;
		}
	}
}

static void rds_app_fast_det(void)
{
	if(!rds_app->ext.fast_det)return;

	if(rds_signal_detect())
	{
		rds_app->ext.fast_det = FALSE;
	}
	else
	{
		if(rds_app->ext.fast_det)
		{
			if(rds_app->dec.stc)
			{
				if(!rds_app->dec.found)
				{
					goto fast_idle_det;
				}
			}

			if(rds_app->ctrl.time > RDS_DECODE_WAITING_TIME)
			{
			fast_idle_det:
				if((rds_app->dec.af.succeed)
				 ||(rds_app->ctrl.backup_disable && rds_app->backup.af.succeed))
				{
					rds_app->next_id = RDS_APP_ID_AF;
				}
				else if((rds_app->dec.pi)
					  ||(rds_app->ctrl.backup_disable && rds_app->backup.pi))
				{
					rds_app->next_id = RDS_APP_ID_PI;
				}
				rds_app->ext.fast_det = FALSE;
			}
		}
	}
}

static void rds_app_af_detect(void)
{
	rds_app_fast_det();

	if(rds_signal_detect())
	{
		rds_app->ctrl.time = 0;
		rds_app->ext.fast_det = FALSE;
		global.rds_af = TRUE;// 搜到AF
	}
	else
	{
		global.rds_af = FALSE;
		if((rds_app->ctrl.time += rds_app->count) > RDS_SIGNAL_DETECT_TIME)
		{
			rds_app->ctrl.time = 0;
			if((++rds_app->ctrl.af_count) > rds_return_time())
			{
				rds_app->ctrl.af_count = 0;
				rds_app->next_id = RDS_APP_ID_PI;
			}
			else
			{
				rds_app->next_id = RDS_APP_ID_AF;
			}
		}
	}
}

static void rds_app_ta_detect(void)
{
	rds_app_interval_detect();

#if defined(_DISABLE_RDS_TP_SINGLE_)
	if(rds_app->dec.ta)
#else
	if(rds_app->dec.ta || rds_app->dec.tp)
#endif
	{
		rds_app->ctrl.time = 0;
		rds_app->ext.fast_det = FALSE;

		if((!rds_app->ctrl.pause_ta)&&(rds_ta_ok()))
		{
			if(!rds_app->ctrl.traffic_interval)
			{
				if(setup.rds.alarm)
				{
					rds_app->ctrl.alarm = RDS_ALARM_TRAFFIC;
					rds_app->next_id = RDS_APP_ID_ALARM;
				}
				else
				{
					rds_app->next_id = RDS_APP_ID_TRAFFIC;
				}
			}
		}
		else if((!rds_app->ctrl.pause_eon)&&(rds_eon_ok()))
		{
			if(!rds_app->ctrl.eon_interval)
			{
				if(setup.rds.alarm)
				{
					rds_app->ctrl.alarm = RDS_ALARM_EON;
					rds_app->next_id = RDS_APP_ID_ALARM;
				}
				else
				{
					rds_app->next_id = RDS_APP_ID_EON;
				}
			}
		}
	}
	else
	{
		if((rds_app->ctrl.time += rds_app->count) > RDS_SIGNAL_DETECT_TIME)
		{
			rds_app->ctrl.time = 0;
			if(setup.rds.alarm)
			{
				rds_app->ctrl.alarm = RDS_ALARM_NO_TP_TA;
				rds_app->next_id = RDS_APP_ID_ALARM;
			}
			else
			{
				rds_app->next_id = RDS_APP_ID_TA;
			}
		}
	}
}

static void rds_app_ta_and_af_detect(void)
{
	rds_app_interval_detect();

#if defined(_DISABLE_RDS_TP_SINGLE_)
	if(rds_app->dec.ta)
#else
	if(rds_app->dec.ta || rds_app->dec.tp)
#endif
	{
		rds_app->ctrl.time = 0;
		rds_app->ext.fast_det = FALSE;

		if((!rds_app->ctrl.pause_ta)&&(rds_ta_ok()))
		{
			if(!rds_app->ctrl.traffic_interval)
			{
				if(setup.rds.alarm)
				{
					rds_app->ctrl.alarm = RDS_ALARM_TRAFFIC;
					rds_app->next_id = RDS_APP_ID_ALARM;
				}
				else
				{
					rds_app->next_id = RDS_APP_ID_TRAFFIC;
				}
			}
		}
		else if((!rds_app->ctrl.pause_eon)&&(rds_eon_ok()))
		{
			if(!rds_app->ctrl.eon_interval)
			{
				if(setup.rds.alarm)
				{
					rds_app->ctrl.alarm = RDS_ALARM_EON;
					rds_app->next_id = RDS_APP_ID_ALARM;
				}
				else
				{
					rds_app->next_id = RDS_APP_ID_EON;
				}
			}
		}
	}
	else
	{
		rds_app_fast_det();

		if(rds_signal_detect())
		{
			if((rds_app->ctrl.time += rds_app->count) > RDS_SIGNAL_DETECT_TIME)
			{
				rds_app->ctrl.time = 0;
				if(setup.rds.alarm)
				{
					rds_app->ctrl.alarm = RDS_ALARM_NO_TP_TA;
					rds_app->next_id = RDS_APP_ID_ALARM;
				}
				else
				{
					rds_app->next_id = RDS_APP_ID_TA;
				}
			}
		}
		else
		{
			if(!rds_app->ctrl.af_count)
			{
				if((rds_app->ctrl.time += rds_app->count) > RDS_SIGNAL_DETECT_TIME)
				{
					rds_app->ctrl.time = 0;
					rds_app->ctrl.af_count ++;

					if(rds_app->ctrl.backup_disable)
					{
						if(!rds_app->backup.af.succeed)
						{
							goto rds_af_check_no_found;
						}
					}
					else
					{
						if(!rds_app->dec.af.succeed)
						{
							goto rds_af_check_no_found;
						}
					}

					rds_app->next_id = RDS_APP_ID_AF;
				}
			}
			else
			{
				if((rds_app->ctrl.time += rds_app->count) > RDS_SIGNAL_DETECT_TIME)
				{
				rds_af_check_no_found:
					rds_app->ctrl.time = 0;
					rds_app->ctrl.af_count = 0;

					if(setup.rds.alarm)
					{
						rds_app->ctrl.alarm = RDS_ALARM_NO_TP_TA;
						rds_app->next_id = RDS_APP_ID_ALARM;
					}
					else
					{
						rds_app->next_id = RDS_APP_ID_TA;
					}
				}
			}
		}
	}
}

static uint8 rds_app_idle_init(void)
{
	rds_app_puts("rds_app_idle_init\n");
	rds_app->ctrl.init 	= TRUE;
	rds_app->ctrl.exit 	= FALSE;
	rds_app->ctrl.alarm = RDS_ALARM_NULL;
	
	if(rds_app->ctrl.found)
	{
		rds_app->ctrl.found 	= FALSE;
		rds_app->ctrl.af_count = 0;
	}
	
	if(rds_app->pty_search.re_start)
	{
		rds_app->pty_search.re_start = FALSE;
		rds_app->next_id = RDS_APP_ID_PTY;
	}
	return TRUE;
}

static uint8 rds_app_idle_run(void)
{
	if((setup.rds.ta && !rds_app->ext.ta_disable) && setup.rds.af)
	{
		rds_app_ta_and_af_detect();
	}
	else if(setup.rds.ta && !rds_app->ext.ta_disable)
	{
		rds_app_ta_detect();
	}
	else if(setup.rds.af)
	{
		rds_app_af_detect();
	}
	
	rds_ctrl_psn_sync();
	rds_ctrl_data_sync();
	
    rds_ctrl_icon_sync();

	return TRUE;
}

static uint8 rds_app_idle_exit(void)
{
	rds_app_puts("rds_app_idle_exit\n");
	if(!rds_app->ctrl.init)return TRUE;
	rds_app->ctrl.init = FALSE;
	rds_app->ctrl.time = 0;
	rds_app->ext.fast_det = FALSE;
	rds_ctrl_icon_sync();
 	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

static uint8 rds_app_af_init(void)
{
	rds_app_puts("rds_app_af_init\n");
	if(rds_app->ctrl.backup_disable)
	{
		if(!rds_app->backup.af.succeed)
		{
			rds_app->ctrl.backup_disable = FALSE;
			if(!rds_app->dec.af.succeed)return FALSE;
		}
	}
	else
	{
		if(!rds_app->dec.af.succeed)return FALSE;
	}

	memset(&RDS_APP_AF, 0, sizeof(RDS_APP_AF));

	rds_ctrl_dec_backup();
	
	RDS_APP_AF.State 			= RDS_APP_AF_STATE_INIT;
	
	RDS_APP_AF.OrigFreq 		= rds_get_af_freq();
	
	rds_app->ctrl.init		= TRUE;
	
	rds_app->ctrl.found	= FALSE;
	
	rds_mute_audio(TRUE);
	
	return TRUE;
}

static uint8 rds_app_af_run(void)
{
	uint8 Temp;
	
	switch(RDS_APP_AF.State)
	{
		case RDS_APP_AF_STATE_INIT:
			RDS_APP_AF.NowFreq 	= RDS_APP_AF.OrigFreq;
			RDS_APP_AF.State 	= RDS_APP_AF_STATE_FREQ;
			return FALSE;
			
		case RDS_APP_AF_STATE_FREQ:
			Temp = rds_ctrl_af_list(RDS_APP_AF.Index++, &rds_app->backup.af, setup.rds.reg);
			if(Temp)
			{
				if(Temp == RDS_APP_AF.OrigFreq)return FALSE;
				if(Temp == RDS_APP_AF.NowFreq)return FALSE;
				RDS_APP_AF.NowFreq = Temp;
				rds_set_af_freq(RDS_APP_AF.NowFreq);
				RDS_APP_AF.State = RDS_APP_AF_STATE_SEEK;
			}
			else
			{
				RDS_APP_AF.State = RDS_APP_AF_STATE_END;
				return FALSE;
			}
			break;
			
		case RDS_APP_AF_STATE_SEEK:
			if(rds_found_state(&Temp, &RDS_APP_AF.NowLevel))
			{
				if(Temp)
				{
					RDS_APP_AF.Time  = RDS_DECODE_WAITING_TIME;
				 	RDS_APP_AF.State = RDS_APP_AF_STATE_DETECT;
				}
				else
				{
					RDS_APP_AF.State = RDS_APP_AF_STATE_FREQ;
					return FALSE;
				}
			}
			break;
			
		case RDS_APP_AF_STATE_DETECT:
			if(rds_app->dec.pi)
			{
				RDS_APP_AF.State = RDS_APP_AF_STATE_FOUND;
				return FALSE;
			}
			else
			{
				if(RDS_APP_AF.Time > rds_app->count)
				{
					RDS_APP_AF.Time -= rds_app->count;
				}
				else
				{
				 	RDS_APP_AF.State = RDS_APP_AF_STATE_FOUND;
					return FALSE;
				}
			}
			break;
			
		case RDS_APP_AF_STATE_FOUND:
			if(RDS_APP_AF.NowLevel > RDS_APP_AF.HighLevel)
			{
				RDS_APP_AF.HighLevel = RDS_APP_AF.NowLevel;
				RDS_APP_AF.FristFreq = RDS_APP_AF.NowFreq;
			}
			
			if(rds_app->dec.pi)
			{
				if(RDS_APP_AF.NowLevel > RDS_APP_AF.RdsHighLevel)
				{
					RDS_APP_AF.RdsHighLevel = RDS_APP_AF.NowLevel;
					RDS_APP_AF.RdsFristFreq = RDS_APP_AF.NowFreq;
				}
			}
			
			if(rds_app->backup.pi == rds_app->dec.pi)
			{
				if(RDS_APP_AF.NowLevel > RDS_APP_AF.PiHighLevel)
				{
					RDS_APP_AF.PiHighLevel = RDS_APP_AF.NowLevel;
					RDS_APP_AF.PiFristFreq = RDS_APP_AF.NowFreq;
				}
			}
		case RDS_APP_AF_STATE_NO_FOUND:
			RDS_APP_AF.State = RDS_APP_AF_STATE_FREQ;
			return FALSE;
			
		case RDS_APP_AF_STATE_END:
			if(setup.rds.mask)
			{
				if(RDS_APP_AF.PiFristFreq)
				{
					RDS_APP_AF.OrigFreq = RDS_APP_AF.PiFristFreq;
					rds_app->ctrl.found	= TRUE;
				}
			#if !defined(_ENABLE_RDS_BUG_)	
				else if(RDS_APP_AF.RdsFristFreq)
				{
					RDS_APP_AF.OrigFreq	= RDS_APP_AF.RdsFristFreq;
					rds_app->ctrl.found	= TRUE;
				}
			#endif	
			}
			else
			{
				if(RDS_APP_AF.PiFristFreq)
				{
					RDS_APP_AF.OrigFreq = RDS_APP_AF.PiFristFreq;
					rds_app->ctrl.found	= TRUE;
				}
				else if(RDS_APP_AF.RdsFristFreq)
				{
					RDS_APP_AF.OrigFreq = RDS_APP_AF.RdsFristFreq;
					rds_app->ctrl.found	= TRUE;
				}
				else if(RDS_APP_AF.FristFreq)
				{
					RDS_APP_AF.OrigFreq = RDS_APP_AF.FristFreq;
					rds_app->ctrl.found	= TRUE;
				}
			}
			rds_app->next_id = RDS_APP_ID_IDLE;
			break;
	}
	
	return TRUE;
}

static uint8 rds_app_af_exit(void)
{
	rds_app_puts("rds_app_af_exit\n");
	if(!rds_app->ctrl.init)return TRUE;
	rds_app->ctrl.init = FALSE;

	rds_freq_resumt(RDS_APP_AF.NowFreq, RDS_APP_AF.OrigFreq);

	rds_mute_audio(FALSE);

	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

static uint8 rds_app_ta_init(void)
{
	rds_app_puts("rds_app_ta_init\n");
	memset(&RDS_APP_TA, 0, sizeof(RDS_APP_TA));

	rds_ctrl_dec_backup();

	RDS_APP_TA.State 	= RDS_APP_TA_STATE_INIT;
	RDS_APP_TA.OrigFreq	= rds_get_af_freq();
	rds_app->ctrl.init	= TRUE;
	rds_app->ctrl.found	= FALSE;
	
	rds_mute_audio(TRUE);
	
	rds_cmd_engine_txt(RDS_TXT_TA_SEEK);
	
	return TRUE;
}

static uint8 rds_app_ta_run(void)
{
	uint8 Temp;

	switch(RDS_APP_TA.State)
	{
		case RDS_APP_TA_STATE_INIT:
			RDS_APP_TA.NowFreq 	= RDS_APP_TA.OrigFreq;
			RDS_APP_TA.State 	= RDS_APP_TA_STATE_FREQ;
			return FALSE;

		case RDS_APP_TA_STATE_FREQ:
			if(RDS_APP_TA.NowFreq < 204)
			{
			 	RDS_APP_TA.NowFreq ++;
			}
			else RDS_APP_TA.NowFreq = 0;

			if(RDS_APP_TA.FreqCount++ > 204)
			{
				RDS_APP_TA.FreqCount = 0;
				RDS_APP_TA.State = RDS_APP_TA_STATE_CONTROL;
				return FALSE;
			}
			else
			{
				if(rds_app->ctrl.exit)
				{
					rds_app->ctrl.exit = FALSE;
					RDS_APP_TA.State = RDS_APP_TA_STATE_END;
					return FALSE;
				}
				rds_set_af_freq(RDS_APP_TA.NowFreq);
				RDS_APP_TA.State = RDS_APP_TA_STATE_SEEK;
			}
			break;

		case RDS_APP_TA_STATE_SEEK:
			if(rds_found_state(&Temp, &RDS_APP_TA.NowLevel))
			{
				if(Temp)
				{
					RDS_APP_TA.Time  = RDS_DECODE_WAITING_TIME;
				 	RDS_APP_TA.State = RDS_APP_TA_STATE_DETECT;
				}
				else
				{
					RDS_APP_TA.State = RDS_APP_TA_STATE_FREQ;
					return FALSE;
				}
			}
			break;

		case RDS_APP_TA_STATE_DETECT:
		#if defined(_DISABLE_RDS_TP_SINGLE_)
			if(rds_app->dec.ta_have)
		#else
			if(rds_app->dec.tp)
			{
				RDS_APP_TA.State = RDS_APP_TA_STATE_FOUND;
				return FALSE;
			}
			else if(rds_app->dec.ta_have)
		#endif
			{
				if(rds_app->dec.ta)
				{
					RDS_APP_TA.State = RDS_APP_TA_STATE_FOUND;
					return FALSE;
				}
				else
				{
					RDS_APP_TA.State = RDS_APP_TA_STATE_NO_FOUND;
					return FALSE;
				}
			}

			if(RDS_APP_TA.Time > rds_app->count)
			{
				RDS_APP_TA.Time -= rds_app->count;
			}
			else
			{
			 	RDS_APP_TA.State = RDS_APP_TA_STATE_NO_FOUND;
				return FALSE;
			}
			break;

		case RDS_APP_TA_STATE_FOUND:
		#if 1
			if(RDS_APP_TA.NowLevel > RDS_APP_TA.HighLevel)
			{
				RDS_APP_TA.HighLevel = RDS_APP_TA.NowLevel;
				RDS_APP_TA.FristFreq = RDS_APP_TA.NowFreq;
			}
			RDS_APP_TA.State = RDS_APP_TA_STATE_CONTROL;
			return FALSE;

		#else
			if(rds_app->backup.pi == rds_app->dec.pi)
			{
				if(RDS_APP_TA.NowLevel > RDS_APP_TA.PiHighLevel)
				{
					RDS_APP_TA.PiHighLevel = RDS_APP_TA.NowLevel;
					RDS_APP_TA.PiFristFreq = RDS_APP_TA.NowFreq;
				}
			}

			if(RDS_APP_TA.NowLevel > RDS_APP_TA.HighLevel)
			{
				RDS_APP_TA.HighLevel = RDS_APP_TA.NowLevel;
				RDS_APP_TA.FristFreq = RDS_APP_TA.NowFreq;
			}
		#endif
		case RDS_APP_TA_STATE_NO_FOUND:
			RDS_APP_TA.State = RDS_APP_TA_STATE_FREQ;
			return FALSE;

		case RDS_APP_TA_STATE_CONTROL:
			if(RDS_APP_TA.PiFristFreq || RDS_APP_TA.FristFreq)
			{
				RDS_APP_TA.State = RDS_APP_TA_STATE_END;
			}
			else
			{
				RDS_APP_TA.PiFristFreq = RDS_APP_TA.FristFreq = 0;
				RDS_APP_TA.PiHighLevel = RDS_APP_TA.HighLevel = 0;

				RDS_APP_TA.State = RDS_APP_TA_STATE_FREQ;
			}
			return FALSE;

		case RDS_APP_TA_STATE_END:
			if(RDS_APP_TA.PiFristFreq)
			{
				RDS_APP_TA.OrigFreq	= RDS_APP_TA.PiFristFreq;
				rds_app->ctrl.found	= TRUE;
			}
			else if(RDS_APP_TA.FristFreq)
			{
				RDS_APP_TA.OrigFreq	= RDS_APP_TA.FristFreq;
				rds_app->ctrl.found	= TRUE;
			}
			rds_app->next_id = RDS_APP_ID_IDLE;
			break;
	}

	return TRUE;
}

static uint8 rds_app_ta_exit(void)
{
	rds_app_puts("rds_app_ta_exit\n");
	if(!rds_app->ctrl.init)return TRUE;
	rds_app->ctrl.init = FALSE;

	rds_freq_resumt(RDS_APP_TA.NowFreq, RDS_APP_TA.OrigFreq);

	rds_cmd_engine_txt(RDS_TXT_NULL);
	rds_mute_audio(FALSE);
	
	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

static uint8 rds_app_pi_init(void)
{
	rds_app_puts("rds_app_pi_init\n");
	if(rds_app->ctrl.backup_disable)
	{
		if(!rds_app->backup.pi)
		{
			rds_app->ctrl.backup_disable = FALSE;
			if(!rds_app->dec.pi)return FALSE;
		}
	}
	else
	{
		if(!rds_app->dec.pi)return FALSE;
	}

	memset(&RDS_APP_PI,0,sizeof(RDS_APP_PI));
	
	rds_ctrl_dec_backup();
	
	RDS_APP_PI.State	= RDS_APP_PI_STATE_INIT;
	RDS_APP_PI.OrigFreq = rds_get_af_freq();
	rds_app->ctrl.init	= TRUE;
	rds_app->ctrl.found	= FALSE;

	rds_mute_audio(TRUE);
	rds_cmd_engine_txt(RDS_TXT_PI_SEEK);
	
	return TRUE;
}

static uint8 rds_app_pi_run(void)
{
	uint8 Temp;

	switch(RDS_APP_PI.State)
	{
		case RDS_APP_PI_STATE_INIT:
			RDS_APP_PI.NowFreq	= RDS_APP_PI.OrigFreq;
			RDS_APP_PI.State	= RDS_APP_PI_STATE_FREQ;
			return FALSE;

		case RDS_APP_PI_STATE_FREQ:
			if(RDS_APP_PI.NowFreq < 204)
			{
				RDS_APP_PI.NowFreq ++;
			}
			else RDS_APP_PI.NowFreq = 0;

			if(RDS_APP_PI.FreqCount ++ > 204)
			{
				RDS_APP_PI.FreqCount = 0;
				RDS_APP_PI.State = RDS_APP_PI_STATE_END;
				return FALSE;
			}
			else
			{
				if(rds_app->ctrl.exit)
				{
					rds_app->ctrl.exit = FALSE;
					RDS_APP_PI.State = RDS_APP_PI_STATE_END;
					return FALSE;
				}
				rds_set_af_freq(RDS_APP_PI.NowFreq);
				RDS_APP_PI.State = RDS_APP_PI_STATE_SEEK;
			}
			break;

		case RDS_APP_PI_STATE_SEEK:
			if(rds_found_state(&Temp,&RDS_APP_PI.NowLevel))
			{
				if(Temp)
				{
					RDS_APP_PI.Time  = RDS_DECODE_WAITING_TIME;
					RDS_APP_PI.State = RDS_APP_PI_STATE_DETECT;
				}
				else
				{
					RDS_APP_PI.State = RDS_APP_PI_STATE_FREQ;
					return FALSE;
				}
			}
			break;

		case RDS_APP_PI_STATE_DETECT:
			if(rds_app->dec.pi)
			{
				RDS_APP_PI.State = RDS_APP_PI_STATE_FOUND;
				return FALSE;
			}
			else
			{
				if(RDS_APP_PI.Time > rds_app->count)
				{
					RDS_APP_PI.Time -= rds_app->count;
				}
				else
				{
					RDS_APP_PI.State = RDS_APP_PI_STATE_NO_FOUND;
					return FALSE;
				}
			}
			break;
		case RDS_APP_PI_STATE_FOUND:
			if(rds_app->backup.pi == rds_app->dec.pi)
			{
				if(RDS_APP_PI.NowLevel > RDS_APP_PI.HighLevel)
				{
					RDS_APP_PI.HighLevel = RDS_APP_PI.NowLevel;
					RDS_APP_PI.FristFreq = RDS_APP_PI.NowFreq;
				}
			}

			if(rds_app->backup.pty == rds_app->dec.pty)
			{
				if(RDS_APP_PI.NowLevel > RDS_APP_PI.PtyHighLevel)
				{
					RDS_APP_PI.PtyHighLevel = RDS_APP_PI.NowLevel;
					RDS_APP_PI.PtyFristFreq = RDS_APP_PI.NowFreq;
				}
			}

			if(rds_app->backup.ms == rds_app->dec.ms)
			{
				if(RDS_APP_PI.NowLevel > RDS_APP_PI.MsHighLevel)
				{
					RDS_APP_PI.MsHighLevel = RDS_APP_PI.NowLevel;
					RDS_APP_PI.MsFristFreq = RDS_APP_PI.NowFreq;
				}
			}

			if(RDS_APP_PI.NowLevel > RDS_APP_PI.RdsHighLevel)
			{
				RDS_APP_PI.RdsHighLevel = RDS_APP_PI.NowLevel;
				RDS_APP_PI.RdsFristFreq = RDS_APP_PI.NowFreq;
			}
		case RDS_APP_PI_STATE_NO_FOUND:
			RDS_APP_PI.State = RDS_APP_PI_STATE_FREQ;
			return FALSE;

		case RDS_APP_PI_STATE_END:
			if(RDS_APP_PI.FristFreq)
			{
				RDS_APP_PI.OrigFreq = RDS_APP_PI.FristFreq;
				rds_app->ctrl.found	= TRUE;
			}
			else
			{
				if(!setup.rds.mask)
				{
					if(RDS_APP_PI.PtyFristFreq)
					{
						RDS_APP_PI.OrigFreq = RDS_APP_PI.PtyFristFreq;
						rds_app->ctrl.found	= TRUE;
					}
					else if(RDS_APP_PI.MsFristFreq)
					{
						RDS_APP_PI.OrigFreq = RDS_APP_PI.MsFristFreq;
						rds_app->ctrl.found	= TRUE;
					}
					else if(RDS_APP_PI.RdsFristFreq)
					{
						RDS_APP_PI.OrigFreq = RDS_APP_PI.RdsFristFreq;
						rds_app->ctrl.found	= TRUE;
					}
				}
			}
			rds_app->next_id = RDS_APP_ID_IDLE;
			break;
	}
	return TRUE;
}

static uint8 rds_app_pi_exit(void)
{
	rds_app_puts("rds_app_pi_exit\n");
	if(!rds_app->ctrl.init)return TRUE;
	rds_app->ctrl.init = FALSE;
	
	rds_freq_resumt(RDS_APP_PI.NowFreq, RDS_APP_PI.OrigFreq);
	
	rds_cmd_engine_txt(RDS_TXT_NULL);
	rds_mute_audio(FALSE);
	
	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

static uint8 rds_app_pty_init(void)
{
	rds_app_puts("rds_app_pty_init\n");
	memset(&RDS_APP_PTY,0,sizeof(RDS_APP_PTY));

	rds_ctrl_dec_backup();

	RDS_APP_PTY.OrigFreq= rds_get_af_freq();
	RDS_APP_PTY.State 	= RDS_APP_PTY_STATE_INIT;
	rds_app->ctrl.init	= TRUE;
	rds_app->ctrl.found	= FALSE;
	
	rds_mute_audio(TRUE);
	rds_cmd_engine_txt(RDS_TXT_PTY_SEEK);
	
	return TRUE;
}

static uint8 rds_app_pty_run(void)
{
enum{
	RDS_TYPE_RDS = 0,
	RDS_TYPE_RBDS,
};
	uint8 Temp;

	rds_app_puts("rds_app_pty_run\n");

	switch(RDS_APP_PTY.State)
	{
		case RDS_APP_PTY_STATE_INIT:
			RDS_APP_PTY.NowFreq 	= RDS_APP_PTY.OrigFreq;
			RDS_APP_PTY.State		= RDS_APP_PTY_STATE_FREQ;
			RDS_APP_PTY.FreqCount	= 0;
			RDS_APP_PTY.AlarmFound	= FALSE;
			RDS_APP_PTY.HighLevel	= 0;
			RDS_APP_PTY.FristFreq	= 0;
			RDS_APP_PTY.CheckBuffer	= rds_app->pty_search.buffer;
			RDS_APP_PTY.CheckLength	= rds_app->pty_search.length;
			RDS_APP_PTY.UpDown		= rds_app->pty_search.up_down;
			return FALSE;
			
		case RDS_APP_PTY_STATE_FREQ:
			if(RDS_APP_PTY.UpDown == UP)
			{
				if(RDS_APP_PTY.NowFreq < 204)
				{
					RDS_APP_PTY.NowFreq ++;
				}
				else RDS_APP_PTY.NowFreq = 0;
			}
			else
			{
				if(RDS_APP_PTY.NowFreq)
				{
					RDS_APP_PTY.NowFreq --;
				}
				else RDS_APP_PTY.NowFreq = 204;
			}
			
			if(rds_app->pty_search.re_start)
			{
				rds_app->pty_search.re_start = FALSE;
				RDS_APP_PTY.State = RDS_APP_PTY_STATE_INIT;
				return FALSE;
			}
			else
			{
				if(RDS_APP_PTY.FreqCount ++ > 204)
				{
					RDS_APP_PTY.FreqCount = 0;
					RDS_APP_PTY.State = RDS_APP_PTY_STATE_END;
					return FALSE;
				}
				else
				{
					if(rds_app->ctrl.exit)
					{
						rds_app->ctrl.exit = FALSE;
						RDS_APP_PTY.ExitFlag = TRUE;
						RDS_APP_PTY.State = RDS_APP_PTY_STATE_END;
						return FALSE;
					}
					rds_set_af_freq(RDS_APP_PTY.NowFreq);
					RDS_APP_PTY.State = RDS_APP_PTY_STATE_SEEK;
				}
			}
			break;

		case RDS_APP_PTY_STATE_SEEK:
			if(rds_found_state(&Temp, &RDS_APP_PTY.NowLevel))
			{
				if(Temp)
				{
					RDS_APP_PTY.Time  = RDS_DECODE_WAITING_TIME;
					RDS_APP_PTY.State = RDS_APP_PTY_STATE_DETECT;
				}
				else
				{
					RDS_APP_PTY.State = RDS_APP_PTY_STATE_FREQ;
					return FALSE;
				}
			}
			break;

		case RDS_APP_PTY_STATE_DETECT:
			if(rds_app->dec.pi)
			{
				if(setup.rds.type == RDS_TYPE_RDS)
				{
					if(rds_app->dec.pty == RDS_PTY_ALARM)
					{
						RDS_APP_PTY.AlarmFound = TRUE;
						RDS_APP_PTY.State = RDS_APP_PTY_STATE_FOUND;
						return FALSE;
					}
				}
				else if(setup.rds.type == RDS_TYPE_RBDS)
				{
					if(rds_app->dec.pty == RDS_PTY_USA_ALERT)
					{
						RDS_APP_PTY.AlarmFound = TRUE;
						RDS_APP_PTY.State = RDS_APP_PTY_STATE_FOUND;
						return FALSE;
					}
				}
				
				for(Temp = 0; Temp < RDS_APP_PTY.CheckLength; Temp ++)
				{
					if(RDS_APP_PTY.CheckBuffer[Temp] == rds_app->dec.pty)break;
				}
				
				if(Temp != RDS_APP_PTY.CheckLength)
				{
					RDS_APP_PTY.State = RDS_APP_PTY_STATE_FOUND;
				}
				else
				{
					RDS_APP_PTY.State = RDS_APP_PTY_STATE_NO_FOUND;
				}
				return FALSE;
			}
			else
			{
				if(RDS_APP_PTY.Time > rds_app->count)
				{
					RDS_APP_PTY.Time -= rds_app->count;
				}
				else
				{
					RDS_APP_PTY.State = RDS_APP_PTY_STATE_NO_FOUND;
					return FALSE;
				}
			}
			break;

		case RDS_APP_PTY_STATE_FOUND:
			if(RDS_APP_PTY.AlarmFound)
			{
				RDS_APP_PTY.State = RDS_APP_PTY_STATE_END;
				return FALSE;
			}
			else
			{
			#if 0
				if(RDS_APP_PTY.NowLevel > RDS_APP_PTY.HighLevel)
				{
					RDS_APP_PTY.HighLevel = RDS_APP_PTY.NowLevel;
					RDS_APP_PTY.FristFreq = RDS_APP_PTY.NowFreq;
				}
			#else
				RDS_APP_PTY.HighLevel = RDS_APP_PTY.NowLevel;
				RDS_APP_PTY.FristFreq = RDS_APP_PTY.NowFreq;
				RDS_APP_PTY.State = RDS_APP_PTY_STATE_END;
				return FALSE;
			#endif
			}
		case RDS_APP_PTY_STATE_NO_FOUND:
			RDS_APP_PTY.State = RDS_APP_PTY_STATE_FREQ;
			return FALSE;

		case RDS_APP_PTY_STATE_END:
			if(RDS_APP_PTY.AlarmFound)
			{
				RDS_APP_PTY.OrigFreq = RDS_APP_PTY.NowFreq;
				rds_app->ctrl.found = TRUE;

			}
			else if(RDS_APP_PTY.FristFreq)
			{
				RDS_APP_PTY.OrigFreq = RDS_APP_PTY.FristFreq;
				rds_app->ctrl.found = TRUE;
			}
			rds_app->next_id = RDS_APP_ID_IDLE;
			break;
	}

	return TRUE;
}

static uint8 rds_app_pty_exit(void)
{
	rds_app_puts("rds_app_pty_exit\n");
	if(!rds_app->ctrl.init)return TRUE;
	rds_app->ctrl.init = FALSE;
	
	if(RDS_APP_PTY.ExitFlag)
	{
		rds_cmd_engine_txt(RDS_TXT_NULL);
	}
	else
	{
		if(rds_app->ctrl.found)
		{
			rds_app->pty_search.cmd = PTY_SEARCH_CMD_FOUND;
		}
		else
		{
			rds_app->pty_search.cmd = PTY_SEARCH_CMD_NO_FOUND;
		}
	}
	
	rds_freq_resumt(RDS_APP_PTY.NowFreq, RDS_APP_PTY.OrigFreq);
	rds_mute_audio(FALSE);
	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
static void rds_dec_backup_eon(void)
{
	uint8 index;
	radio_rds_eon_t *now_eon;

	for(index = 0; index < _RDS_EON_LIST_MAX_; index++)
	{
		now_eon = &rds_app->dec.eon[index];
		if(now_eon->pi)
		{
			if(now_eon->ta && now_eon->tp)
			{
				if(now_eon->af.succeed || now_eon->mapped.length)
				{
					RDS_APP_EON.BackUp.pi = now_eon->pi;

					if(now_eon->af.succeed)
					{
						memcpy(&RDS_APP_EON.BackUp.af,  	&now_eon->af, 		sizeof(RDS_APP_EON.BackUp.af));
					}

					if(now_eon->mapped.length)
					{
						memcpy(&RDS_APP_EON.BackUp.mapped,	&now_eon->mapped, 	sizeof(RDS_APP_EON.BackUp.mapped));
					}

					return;
				}
			}
		}
	}
}

static uint8 rds_app_eon_init(void)
{
	rds_app_puts("rds_app_eon_init\n");
	if(!rds_ctrl_eon_check())return FALSE;
	
	rds_cmd_eon_disable();
	
	memset(&RDS_APP_EON,0,sizeof(RDS_APP_EON));
	
	rds_ctrl_dec_backup();
	rds_dec_backup_eon();
	
	RDS_APP_EON.State 	= RDS_APP_EON_STATE_INIT;
	RDS_APP_EON.OrigFreq= rds_get_af_freq();
	RDS_APP_EON.NowFreq = RDS_APP_EON.OrigFreq;
	rds_app->ctrl.init	= TRUE;
	rds_app->ctrl.found	= FALSE;
	
	return TRUE;
}

static uint8 rds_app_eon_run(void)
{
	uint8 Temp;

	switch(RDS_APP_EON.State)
	{
		case RDS_APP_EON_STATE_INIT:
			if(RDS_APP_EON.BackUp.af.succeed)
			{
				RDS_APP_EON.Mode = RDS_APP_EON_MODE_AF;
				RDS_APP_EON.State = RDS_APP_EON_STATE_FREQ;
				rds_mute_audio(TRUE);
			}
			else if(RDS_APP_EON.BackUp.mapped.length)
			{
				RDS_APP_EON.Mode = RDS_APP_EON_MODE_MAPPED;
				RDS_APP_EON.State = RDS_APP_EON_STATE_FREQ;
				rds_mute_audio(TRUE);
			}
			else
			{
				RDS_APP_EON.State = RDS_APP_EON_STATE_EXIT;
			}
			return FALSE;
			
		case RDS_APP_EON_STATE_FREQ:
			switch(RDS_APP_EON.Mode)
			{
				case RDS_APP_EON_MODE_AF:
					Temp = rds_ctrl_af_list(RDS_APP_EON.Index++, &RDS_APP_EON.BackUp.af, setup.rds.reg);
					if(Temp)
					{
						if(Temp == RDS_APP_EON.OrigFreq)return FALSE;
						if(Temp == RDS_APP_EON.NowFreq)return FALSE;
						RDS_APP_EON.NowFreq = Temp;
						rds_set_af_freq(RDS_APP_EON.NowFreq);
						RDS_APP_EON.State = RDS_APP_EON_STATE_SEEK;
					}
					else
					{
						RDS_APP_EON.Index = 0;
						RDS_APP_EON.State = RDS_APP_EON_STATE_ENTER;
						return FALSE;
					}
					break;
				case RDS_APP_EON_MODE_MAPPED:
					if(RDS_APP_EON.Index < RDS_APP_EON.BackUp.mapped.length)
					{
						Temp = RDS_APP_EON.BackUp.mapped.freq[RDS_APP_EON.Index++];
						if(Temp == RDS_APP_EON.OrigFreq)return FALSE;
						if(Temp == RDS_APP_EON.NowFreq)return FALSE;
						RDS_APP_EON.NowFreq = Temp;
						rds_set_af_freq(RDS_APP_EON.NowFreq);
						RDS_APP_EON.State = RDS_APP_EON_STATE_SEEK;
					}
					else
					{
						RDS_APP_EON.Index = 0;
						RDS_APP_EON.State = RDS_APP_EON_STATE_ENTER;
						return FALSE;
					}
					break;
			}
			break;

		case RDS_APP_EON_STATE_SEEK:
			if(rds_found_state(&Temp,&RDS_APP_EON.NowLevel))
			{
				if(Temp)
				{
					RDS_APP_EON.Time  = RDS_DECODE_WAITING_TIME;
				 	RDS_APP_EON.State = RDS_APP_EON_STATE_DETECT;
				}
				else
				{
					RDS_APP_EON.State = RDS_APP_EON_STATE_FREQ;
					return FALSE;
				}
			}
			break;

		case RDS_APP_EON_STATE_DETECT:
			if((rds_app->dec.ta_have)&&(rds_eon_ta_ok()))
			{
				RDS_APP_EON.State = RDS_APP_EON_STATE_FOUND;
				return FALSE;
			}
			else
			{
				if(rds_app->dec.ta_have)
				{
					if(!rds_eon_ta_ok())
					{
						RDS_APP_EON.State = RDS_APP_EON_STATE_NO_FOUND;
						return FALSE;
					}
				}
				
				if(RDS_APP_EON.Time > rds_app->count)
				{
					RDS_APP_EON.Time -= rds_app->count;
				}
				else
				{
					RDS_APP_EON.State = RDS_APP_EON_STATE_NO_FOUND;
					return FALSE;
				}
			}
			break;

		case RDS_APP_EON_STATE_FOUND:
			if(RDS_APP_EON.NowLevel > RDS_APP_EON.HighLevel)
			{
				RDS_APP_EON.FristFreq = RDS_APP_EON.NowFreq;
				RDS_APP_EON.HighLevel = RDS_APP_EON.NowLevel;
			}
			
			if(RDS_APP_EON.BackUp.pi == rds_app->dec.pi)
			{
				if(RDS_APP_EON.NowLevel > RDS_APP_EON.PiHighLevel)
				{
					RDS_APP_EON.PiFristFreq = RDS_APP_EON.NowFreq;
					RDS_APP_EON.PiHighLevel = RDS_APP_EON.NowLevel;
				}
			}
		case RDS_APP_EON_STATE_NO_FOUND:
			RDS_APP_EON.State = RDS_APP_EON_STATE_FREQ;
			return FALSE;

		case RDS_APP_EON_STATE_ENTER:
			switch(RDS_APP_EON.Mode)
			{
				case RDS_APP_EON_MODE_AF:
					if(RDS_APP_EON.PiFristFreq)
					{
						RDS_APP_EON.NowFreq = RDS_APP_EON.PiFristFreq;
						rds_set_af_freq(RDS_APP_EON.NowFreq);
						RDS_APP_EON.Time 	= RDS_SIGNAL_DETECT_TIME;
						RDS_APP_EON.Found 	= RDS_CHECK_FOUND_COUNT;
						RDS_APP_EON.Signal 	= RDS_TRAFFIC_DETECT_TIME;
						RDS_APP_EON.State 	= RDS_APP_EON_STATE_IDLE;
						rds_mute_audio(FALSE);
					}
					else if(RDS_APP_EON.FristFreq)
					{
						RDS_APP_EON.NowFreq = RDS_APP_EON.FristFreq;
						rds_set_af_freq(RDS_APP_EON.NowFreq);
						RDS_APP_EON.Time 	= RDS_SIGNAL_DETECT_TIME;
						RDS_APP_EON.Found 	= RDS_CHECK_FOUND_COUNT;
						RDS_APP_EON.Signal 	= RDS_TRAFFIC_DETECT_TIME;
						RDS_APP_EON.State 	= RDS_APP_EON_STATE_IDLE;
						rds_mute_audio(FALSE);
					}
					else
					{
						RDS_APP_EON.Mode 	= RDS_APP_EON_MODE_MAPPED;
						RDS_APP_EON.State 	= RDS_APP_EON_STATE_FREQ;
					}
					break;
				case RDS_APP_EON_MODE_MAPPED:
					if(RDS_APP_EON.PiFristFreq)
					{
						RDS_APP_EON.NowFreq = RDS_APP_EON.PiFristFreq;
						rds_set_af_freq(RDS_APP_EON.NowFreq);
						RDS_APP_EON.Time 	= RDS_SIGNAL_DETECT_TIME;
						RDS_APP_EON.Signal 	= RDS_TRAFFIC_DETECT_TIME;
						RDS_APP_EON.Found 	= RDS_CHECK_FOUND_COUNT;
						RDS_APP_EON.State 	= RDS_APP_EON_STATE_IDLE;
						rds_mute_audio(FALSE);
					}
					else if(RDS_APP_EON.FristFreq)
					{
						RDS_APP_EON.NowFreq = RDS_APP_EON.FristFreq;
						rds_set_af_freq(RDS_APP_EON.NowFreq);
						RDS_APP_EON.Time 	= RDS_SIGNAL_DETECT_TIME;
						RDS_APP_EON.Found 	= RDS_CHECK_FOUND_COUNT;
						RDS_APP_EON.Signal 	= RDS_TRAFFIC_DETECT_TIME;
						RDS_APP_EON.Mode 	= RDS_APP_EON_MODE_AF;
						RDS_APP_EON.State 	= RDS_APP_EON_STATE_IDLE;
						rds_mute_audio(FALSE);
					}
					else
					{
						RDS_APP_EON.State 	= RDS_APP_EON_STATE_EXIT;
					}
					break;
			}
			
			if(RDS_APP_EON.State == RDS_APP_EON_STATE_IDLE)
			{
				if(!RDS_APP_EON.Change)
				{
					RDS_APP_EON.Change = TRUE;
					rds_cmd_engine_txt(RDS_TXT_EON_TA);
					rds_cmd_engine_traffic(ENABLE);
				}
			}
			return FALSE;
			
		case RDS_APP_EON_STATE_IDLE:
			if(rds_app->dec.found)
			{
				RDS_APP_EON.Found = RDS_CHECK_FOUND_COUNT;
			}
			
			if(rds_eon_ta_ok())
			{
				RDS_APP_EON.Time = RDS_SIGNAL_DETECT_TIME;
			}
			else
			{
				if(rds_signal_detect())
				{
					if(RDS_APP_EON.Signal > rds_app->count)
					{
						RDS_APP_EON.Signal -= rds_app->count;
					}
					else
					{
						RDS_APP_EON.State = RDS_APP_EON_STATE_EXIT;
						return FALSE;
					}
				}
				else
				{
					if(!rds_app->dec.found)
					{
						if(RDS_APP_EON.Found)
						{
							if(!(--RDS_APP_EON.Found))
							{
								RDS_APP_EON.Time = 0;
							}
						}
					}

					RDS_APP_EON.Signal = RDS_TRAFFIC_DETECT_TIME;
					if(RDS_APP_EON.Time > rds_app->count)
					{
						RDS_APP_EON.Time -= rds_app->count;
					}
					else
					{
						RDS_APP_EON.PiFristFreq = RDS_APP_EON.FristFreq = 0;
						RDS_APP_EON.PiHighLevel = RDS_APP_EON.HighLevel = 0;
						RDS_APP_EON.State = RDS_APP_EON_STATE_INIT;
						return FALSE;
					}
				}
			}

			if(rds_app->ctrl.exit)
			{
				rds_app->ctrl.exit = FALSE;
				RDS_APP_EON.State = RDS_APP_EON_STATE_EXIT;
				return FALSE;
			}
			break;

		case RDS_APP_EON_STATE_EXIT:
			RDS_APP_EON.State = RDS_APP_EON_STATE_END;
			return FALSE;

		case RDS_APP_EON_STATE_END:
			rds_mute_audio(TRUE);
			rds_app->next_id = RDS_APP_ID_IDLE;
			break;
	}

	return TRUE;
}

static uint8 rds_app_eon_exit(void)
{
	rds_app_puts("rds_app_eon_exit\n");
	if(!rds_app->ctrl.init)return TRUE;
	rds_app->ctrl.init = FALSE;
	
	if(RDS_APP_EON.Change)
	{
		rds_cmd_engine_txt(RDS_TXT_NULL);
		rds_cmd_engine_traffic(DISABLE);
		RDS_APP_EON.Change = FALSE;
	}
	
	rds_freq_resumt(RDS_APP_EON.NowFreq, RDS_APP_EON.OrigFreq);
	
	rds_app->ctrl.eon_interval = RDS_INTERVAL_EON_MIN_TIME;
	
	if(setup.rds.eon)
	{
		rds_cmd_eon_enable();
	}
	
	if(rds_app->ctrl.backup_disable)
	{//	EON回来的时候防止原有频率已经失调了，可以快速检测一下。
		rds_app->ext.fast_det = TRUE;
	}
	
	rds_mute_audio(FALSE);
	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

static uint8 rds_app_traffic_init(void)
{
	rds_app_puts("rds_app_traffic_init\n");
	if(!rds_app->dec.pi)return FALSE;
	
	memset(&RDS_APP_TRAFFIC,0,sizeof(RDS_APP_TRAFFIC));
	
	rds_ctrl_dec_backup();
	
	RDS_APP_TRAFFIC.State		= RDS_APP_TRAFFIC_STATE_INIT;
	RDS_APP_TRAFFIC.OrigFreq	= rds_get_af_freq();
	rds_app->ctrl.init = TRUE;
	rds_app->ctrl.found	= FALSE;
	rds_cmd_engine_txt(RDS_TXT_TRAFFIC);
	rds_cmd_engine_traffic(ENABLE);
	
	return TRUE;
}

static uint8 rds_app_traffic_run(void)
{
	uint8 Temp;

	switch(RDS_APP_TRAFFIC.State)
	{
		case RDS_APP_TRAFFIC_STATE_INIT:
			RDS_APP_TRAFFIC.NowFreq 	= RDS_APP_TRAFFIC.OrigFreq;
			RDS_APP_TRAFFIC.AfOrigFreq 	= RDS_APP_TRAFFIC.OrigFreq;
			RDS_APP_TRAFFIC.Time 		= RDS_SIGNAL_DETECT_TIME;
			RDS_APP_TRAFFIC.Found 		= RDS_CHECK_FOUND_COUNT;
			RDS_APP_TRAFFIC.Signal 		= RDS_TRAFFIC_DETECT_TIME;
			RDS_APP_TRAFFIC.State 		= RDS_APP_TRAFFIC_STATE_IDLE;
			return FALSE;
			
		case RDS_APP_TRAFFIC_STATE_ENTER:
			if(rds_app->backup.af.succeed)
			{
				RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_FREQ;
				rds_mute_audio(TRUE);
			}
			else if(rds_app->dec.af.succeed)
			{
				rds_app->ctrl.backup_disable = FALSE;
				rds_ctrl_dec_backup();
				RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_FREQ;
				rds_mute_audio(TRUE);
			}
			else
			{
				RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_EXIT;
			}
			return FALSE;
			
		case RDS_APP_TRAFFIC_STATE_FREQ:
			Temp = rds_ctrl_af_list(RDS_APP_TRAFFIC.Index++, &rds_app->backup.af, setup.rds.reg);
			if(Temp)
			{
				if(Temp == RDS_APP_TRAFFIC.NowFreq)return FALSE;
				if(Temp == RDS_APP_TRAFFIC.AfOrigFreq)return FALSE;
				RDS_APP_TRAFFIC.NowFreq = Temp;
				rds_set_af_freq(RDS_APP_TRAFFIC.NowFreq);
				RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_SEEK;
			}
			else
			{
				RDS_APP_TRAFFIC.Index = 0;
				RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_RESULT;
				return FALSE;
			}
			break;
			
		case RDS_APP_TRAFFIC_STATE_SEEK:
			if(rds_found_state(&Temp, &RDS_APP_TRAFFIC.NowLevel))
			{
				if(Temp)
				{
					RDS_APP_TRAFFIC.Time  = RDS_DECODE_WAITING_TIME;
				 	RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_DETECT;
				}
				else
				{
					RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_FREQ;
					return FALSE;
				}
			}
			break;
			
		case RDS_APP_TRAFFIC_STATE_DETECT:
			if((rds_app->dec.ta_have)&&(rds_ta_ok()))
			{
				RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_FOUND;
				return FALSE;
			}
			else
			{
				if(rds_app->dec.ta_have)
				{
					if(!rds_ta_ok())
					{
						RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_NO_FOUND;
						return FALSE;
					}
				}
				
				if(RDS_APP_TRAFFIC.Time > rds_app->count)
				{
					RDS_APP_TRAFFIC.Time -= rds_app->count;
				}
				else
				{
					RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_NO_FOUND;
					return FALSE;
				}
			}
			break;
			
		case RDS_APP_TRAFFIC_STATE_FOUND:
			if(RDS_APP_TRAFFIC.NowLevel > RDS_APP_TRAFFIC.HighLevel)
			{
				RDS_APP_TRAFFIC.HighLevel = RDS_APP_TRAFFIC.NowLevel;
				RDS_APP_TRAFFIC.FristFreq = RDS_APP_TRAFFIC.NowFreq;
			}
			
			if(rds_app->backup.pi == rds_app->dec.pi)
			{
				if(RDS_APP_TRAFFIC.NowLevel > RDS_APP_TRAFFIC.PiHighLevel)
				{
					RDS_APP_TRAFFIC.PiHighLevel = RDS_APP_TRAFFIC.NowLevel;
					RDS_APP_TRAFFIC.PiFristFreq = RDS_APP_TRAFFIC.NowFreq;
				}
			}
			
		case RDS_APP_TRAFFIC_STATE_NO_FOUND:
			RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_FREQ;
			return FALSE;
			
		case RDS_APP_TRAFFIC_STATE_RESULT:
			if(RDS_APP_TRAFFIC.PiFristFreq)
			{
				RDS_APP_TRAFFIC.AfOrigFreq = RDS_APP_TRAFFIC.PiFristFreq;
				rds_set_af_freq(RDS_APP_TRAFFIC.PiFristFreq);
				rds_save_freq(RDS_APP_TRAFFIC.PiFristFreq);
				RDS_APP_TRAFFIC.Time 	= RDS_SIGNAL_DETECT_TIME;
				RDS_APP_TRAFFIC.Found 	= RDS_CHECK_FOUND_COUNT;
				RDS_APP_TRAFFIC.Signal 	= RDS_TRAFFIC_DETECT_TIME;
				RDS_APP_TRAFFIC.State 	= RDS_APP_TRAFFIC_STATE_IDLE;
				rds_mute_audio(FALSE);
			}
			else if(RDS_APP_TRAFFIC.FristFreq)
			{
				RDS_APP_TRAFFIC.AfOrigFreq = RDS_APP_TRAFFIC.FristFreq;
				rds_set_af_freq(RDS_APP_TRAFFIC.FristFreq);
				rds_save_freq(RDS_APP_TRAFFIC.FristFreq);
				RDS_APP_TRAFFIC.Time 	= RDS_SIGNAL_DETECT_TIME;
				RDS_APP_TRAFFIC.Found 	= RDS_CHECK_FOUND_COUNT;
				RDS_APP_TRAFFIC.Signal 	= RDS_TRAFFIC_DETECT_TIME;
				RDS_APP_TRAFFIC.State 	= RDS_APP_TRAFFIC_STATE_IDLE;
				rds_mute_audio(FALSE);
			}
			else
			{
				if(RDS_APP_TRAFFIC.OrigFreq != RDS_APP_TRAFFIC.AfOrigFreq)
				{
					RDS_APP_TRAFFIC.OrigFreq = RDS_APP_TRAFFIC.AfOrigFreq;
				}
				RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_END;
			}
			return FALSE;
			
		case RDS_APP_TRAFFIC_STATE_IDLE:
			if(rds_app->dec.found)
			{
				RDS_APP_TRAFFIC.Found = RDS_CHECK_FOUND_COUNT;
			}
			
			if(rds_ta_ok())
			{
			 	RDS_APP_TRAFFIC.Time = RDS_SIGNAL_DETECT_TIME;
			}
			else
			{
				if(rds_signal_detect())
				{
					if(RDS_APP_TRAFFIC.Signal > rds_app->count)
					{
						RDS_APP_TRAFFIC.Signal -= rds_app->count;
					}
					else
					{
						if(RDS_APP_TRAFFIC.OrigFreq != RDS_APP_TRAFFIC.AfOrigFreq)
						{
							rds_app->ctrl.found = TRUE;
							RDS_APP_TRAFFIC.OrigFreq = RDS_APP_TRAFFIC.AfOrigFreq;
						}
						RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_END;
						return FALSE;
					}
				}
				else
				{
					if(!rds_app->dec.found)
					{
						if(RDS_APP_TRAFFIC.Found)
						{
							if(!(--RDS_APP_TRAFFIC.Found))
							{
								RDS_APP_TRAFFIC.Time = 0;
							}
						}
					}
					
					RDS_APP_TRAFFIC.Signal = RDS_TRAFFIC_DETECT_TIME;
					if(RDS_APP_TRAFFIC.Time > rds_app->count)
					{
						RDS_APP_TRAFFIC.Time -= rds_app->count;
					}
					else
					{
						RDS_APP_TRAFFIC.PiFristFreq = RDS_APP_TRAFFIC.FristFreq = 0;
						RDS_APP_TRAFFIC.PiHighLevel = RDS_APP_TRAFFIC.HighLevel = 0;
						RDS_APP_TRAFFIC.State 		= RDS_APP_TRAFFIC_STATE_ENTER;
						return FALSE;
					}
				}
			}
			
			if(rds_app->ctrl.exit)
			{
				rds_app->ctrl.exit = FALSE;
				if(rds_signal_detect())
				{
					if(RDS_APP_TRAFFIC.OrigFreq != RDS_APP_TRAFFIC.AfOrigFreq)
					{
						rds_app->ctrl.found = TRUE;
						RDS_APP_TRAFFIC.OrigFreq = RDS_APP_TRAFFIC.AfOrigFreq;
					}
				}
				RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_END;
				return FALSE;
			}
			break;
			
		case RDS_APP_TRAFFIC_STATE_EXIT:
			RDS_APP_TRAFFIC.State = RDS_APP_TRAFFIC_STATE_END;
			return FALSE;
			
		case RDS_APP_TRAFFIC_STATE_END:
			rds_app->next_id = RDS_APP_ID_IDLE;
			break;
	}

	return TRUE;
}

static uint8 rds_app_traffic_exit(void)
{
	rds_app_puts("rds_app_traffic_exit\n");
	if(!rds_app->ctrl.init)return TRUE;
	rds_app->ctrl.init = FALSE;
	
	rds_freq_resumt(RDS_APP_TRAFFIC.NowFreq, RDS_APP_TRAFFIC.OrigFreq);
	
	rds_app->ctrl.traffic_interval = RDS_INTERVAL_TRAFFIC_MIN_TIME;

	rds_cmd_engine_txt(RDS_TXT_NULL);
	rds_cmd_engine_traffic(DISABLE);
	rds_mute_audio(FALSE);
	
	return TRUE;
}

/*---------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------*/
static void rds_app_alarm_txt(void)
{
	switch(rds_app->ctrl.alarm)
	{
		case RDS_ALARM_NULL:
			rds_cmd_engine_txt(RDS_TXT_NULL);
			break;
			
		case RDS_ALARM_NO_TP_TA:
			rds_cmd_engine_txt(RDS_TXT_NO_TP_TA);
			break;
			
		case RDS_ALARM_EON:
			rds_cmd_engine_txt(RDS_TXT_EON_TA);
			break;
			
		case RDS_ALARM_TRAFFIC:
			rds_cmd_engine_txt(RDS_TXT_TRAFFIC);
			break;
	}
}

static uint8 rds_app_alarm_init(void)
{
	rds_app_puts("rds_app_alarm_init\n");
	if(rds_app->ctrl.alarm == RDS_ALARM_NULL)return FALSE;
	
	memset(&RDS_APP_ALARM,0,sizeof(RDS_APP_ALARM));
	
	RDS_APP_ALARM.State	= RDS_APP_ALARM_STATE_INIT;
	
	rds_app->ctrl.init	= TRUE;
	
	rds_app->ctrl.found	= FALSE;
	
	RDS_APP_ALARM.Flash = TRUE;
	
	rds_app_alarm_txt();
	
	return TRUE;
}

static uint8 rds_app_alarm_run(void)
{
	if((RDS_APP_ALARM.Time += rds_app->count) > 500)
	{
		RDS_APP_ALARM.Time = 0;
		
		RDS_APP_ALARM.Flash = !RDS_APP_ALARM.Flash;
		
		if(RDS_APP_ALARM.Flash)
		{
			rds_app_alarm_txt();
		}
		else
		{
			rds_cmd_engine_txt(RDS_TXT_FLASH);
		}
	}
	
	switch(rds_app->ctrl.alarm)
	{
		case RDS_ALARM_NO_TP_TA:
		#if defined(_DISABLE_RDS_TP_SINGLE_)
			if(rds_app->dec.ta)
		#else
			if(rds_app->dec.ta || rds_app->dec.tp)
		#endif
			{
				rds_app->next_id = RDS_APP_ID_IDLE;
			}
			break;
		case RDS_ALARM_EON:
			if(!rds_eon_ok())
			{
				rds_app->next_id = RDS_APP_ID_IDLE;
			}
			break;
		case RDS_ALARM_TRAFFIC:
			if(!rds_ta_ok())
			{
				rds_app->next_id = RDS_APP_ID_IDLE;
			}
			break;
		default:
			break;
	}
	
	if(rds_app->ctrl.exit)
	{
		rds_app->ctrl.exit = FALSE;
		rds_app->next_id = RDS_APP_ID_IDLE;
	}
	
	return TRUE;
}

static uint8 rds_app_alarm_exit(void)
{
	rds_app_puts("rds_app_alarm_exit\n");
	if(!rds_app->ctrl.init)return TRUE;
	
	rds_app->ctrl.init = FALSE;
	
	rds_app->ctrl.alarm = RDS_ALARM_NULL;
	
	rds_app_alarm_txt();
	
	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
typedef uint8 (*rds_func_f)(void);

typedef struct{
	rds_func_f init;
	rds_func_f run;
	rds_func_f exit;
}rds_app_func_t;

static const rds_app_func_t rds_app_func[]=
{
	{rds_app_idle_init,		rds_app_idle_run,		rds_app_idle_exit		},
	{rds_app_af_init,		rds_app_af_run,			rds_app_af_exit			},
	{rds_app_ta_init,		rds_app_ta_run,			rds_app_ta_exit			},
	{rds_app_pi_init,		rds_app_pi_run,			rds_app_pi_exit			},
	{rds_app_pty_init,		rds_app_pty_run,		rds_app_pty_exit		},
	{rds_app_eon_init,		rds_app_eon_run,		rds_app_eon_exit		},
	{rds_app_traffic_init,	rds_app_traffic_run,	rds_app_traffic_exit	},
	{rds_app_alarm_init,	rds_app_alarm_run,		rds_app_alarm_exit		},
};

void rds_app_polling(void)
{	
	if(!rds_cmd_tuner_app(&rds_app->dec))return;
	
	rds_ctrl_pty_check();
		
rds_polling_again:
	
 	if(rds_app->next_id != rds_app->id)
	{
		rds_app->mode = RDS_APP_MODE_EXIT;
	}
			
	switch(rds_app->mode)
	{
		case RDS_APP_MODE_INIT:
			if(rds_app_func[rds_app->id].init())
			{
				rds_app->mode = RDS_APP_MODE_RUN;
			}
			else
			{
				rds_app->next_id = RDS_APP_ID_IDLE;
			}
			goto rds_polling_again;
			
		case RDS_APP_MODE_RUN:
			while(!rds_app_func[rds_app->id].run());
			break;
			
		case RDS_APP_MODE_EXIT:
			if(rds_app_func[rds_app->id].exit())
			{
				rds_app->id = rds_app->next_id;
				rds_app->mode = RDS_APP_MODE_INIT;
				goto rds_polling_again;
			}
			break;
	}
}

void rds_app_reset(void)
{
	rds_app_puts("rds_app_reset\n");
	
	if((rds_app->id != RDS_APP_ID_IDLE)||(rds_app->mode != RDS_APP_MODE_INIT))
	{
		while(!rds_app_func[rds_app->id].exit());
		rds_app->id = rds_app->next_id = RDS_APP_ID_IDLE;
		rds_app->mode = RDS_APP_MODE_INIT;
	}
	
	rds_cmd_reset();
	
	memset(&rds_app->ctrl, 	0,  sizeof(rds_app->ctrl));
	memset(&rds_app->dec, 	0,  sizeof(rds_app->dec));
	
	rds_app->pty_search.cmd = PTY_SEARCH_CMD_NULL;
	rds_app->pty_search.re_start = FALSE;
}

#endif

