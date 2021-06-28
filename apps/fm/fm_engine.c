#include "sdk_cfg.h"
#include "ui_res.h"
#include "ui_api.h"
#include "key_drv.h"
#include "fm_engine.h"
#include "fm_radio.h"
#include "tuner_drv.h"
#include "rds/rds_ctrl.h"
#include "setup.h"
#include "audio_drv.h"
#include "timer.h"
#include "fm_band.h"
#include "disp.h"



#ifdef	TD_ADD_SPP
extern void Spp_Tx_Radio_Info(uint16 freq);
#endif		
#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_Radio_Info(uint16 freq);
#endif

#if defined(_SUPPORT_RADIO_)

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);
static void fm_engine_create(void);
static void fm_engine_delete(void);
static uint8 fmengine_get_status(engine_status_t *);

/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
#if defined(_FM_VOLSET_USE_CHIP_)
#if (AUDIO_VOLUME_MAX == 40)
static const uint8 Si4730_Vol_Set[41]=
{
	0,
	1 ,3 ,5 ,7 ,9 ,10,12,14,16,18,
	20,22,24,26,28,30,32,34,36,38,
	40,42,44,46,48,49,50,51,52,53,
	54,55,56,57,58,59,60,61,62,63,
};

static const uint8 Ew730_Vol_Set[41]=
{
	0,
	1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,
	11,12,13,14,15,16,17,18,19,20,
	21,22,23,23,24,24,25,25,26,26,
	27,27,28,28,29,29,30,30,31,31,
};


#endif

#endif


typedef struct{
	uint8	type;					//收音驱动类型

	uint8  	fm_threshold;			//调频模组默认搜台门限值
	uint8  	fm_loc_threshold;		//调频模组LOC搜台门限值
	uint8	fm_dead_spot_length;	//FM死点长度
	uint16	*fm_dead_spot_tab;		//FM死点表

	uint8  	am_threshold;			//调幅模组默认搜台门限值
	uint8  	am_loc_threshold;		//调幅模组LOC搜台门限值
	uint8	am_dead_spot_length;	//AM死点长度
	uint16	*am_dead_spot_tab;		//AM死点表

	uint8	hard_auto;				//自动搜索功能
	uint8	fm_only;				//单FM 收音
#if defined(_ENABLE_RADIO_RDS_)
	uint8	rds_support;			//支持RDS
#endif
}engine_info_t;

typedef struct{
	void 	*return_func;			//	返回接口
	uint8 	hard_auto;
	uint8 	flag;
	uint8 	timer_enter;
	uint8 	refur_count;
	uint8 	refur_flag;
	uint8 	round_flag;
	uint16 	freq;
	uint16 	prev_freq;
	uint8 	round_count;
	uint8 	dir;
	engine_auto_seek_t info;
#if defined(_ENABLE_RADIO_RDS_)
	uint8 	rds_enable;			//	RDS 使能
	uint8 	rds_front;			//	RDS 前后台标志
	void 	*rds_func;			//	RDS 回调接口
	void	*rds_busy;			//	RDS 忙检测接口
	uint8 	rds_channel;		//	RDS 通道转换保存
	uint16	rds_pi;
	uint8	wait_flag;
	uint16	wait_timer;

	uint8	rds_mute;
	uint8	rds_txt;
#endif
}engine_ctrl_t;

typedef struct{
	engine_status_t status;
	engine_info_t info;
	engine_ctrl_t ctrl;
	int	timer;
}fm_engine_t;

static fm_engine_t *fm_engine = NULL;
static uint8 fm_engine_run = FALSE;
static uint8 fm_engine_work = FALSE;
static uint8 fm_sem_flag = FALSE;
static OS_SEM fm_eng_sem;


#if defined(_ENABLE_RADIO_TIME_)
#define FM_ENGINE_POLLING_TIME						(5)
#else
#define FM_ENGINE_POLLING_TIME						(50)//50
#endif

/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_RADIO_RDS_)
static uint8 fmengine_rds_det(void)
{
	if(fm_engine->ctrl.rds_enable)
	{
    #if defined(_RADIO_RDS_SAVE_ALL_STATION_)
    	if(setup.rds.ta)
    #else
    	if(setup.rds.af || setup.rds.ta)
    #endif
		{
			return TRUE;
		}
	}
	return FALSE;
}
#endif

static uint16 fmengine_next_freq(void)
{
	if(fm_engine->ctrl.dir == UP)
	{
		if(fm_engine->status.freq + fm_engine->ctrl.info.step > fm_engine->ctrl.info.max_freq)
		{
			return (fm_engine->ctrl.info.min_freq);
		}
		else
		{
			return (fm_engine->status.freq + fm_engine->ctrl.info.step);
		}
	}
	else
	{
		if(fm_engine->status.freq - fm_engine->ctrl.info.step < fm_engine->ctrl.info.min_freq)
		{
			return (fm_engine->ctrl.info.max_freq);
		}
		else
		{
			return (fm_engine->status.freq - fm_engine->ctrl.info.step);
		}
	}
}

static uint8 fmengine_dead_spot_threshold_offset(void)
{
	uint16 freq = fm_engine->status.freq;
	uint8  index;

	switch(fm_engine->status.band)
	{
		case RADIO_BAND_FM:
			if(fm_engine->info.fm_dead_spot_length)
			{
				for(index = 0; index < fm_engine->info.fm_dead_spot_length; index++)
				{
					if(fm_engine->info.fm_dead_spot_tab[index]== freq)return 10;
				}
			}
			break;

		case RADIO_BAND_AM:
			if(fm_engine->info.am_dead_spot_length)
			{
				for(index = 0; index < fm_engine->info.am_dead_spot_length; index++)
				{
					if(fm_engine->info.am_dead_spot_tab[index]== freq)return 10;
				}
			}
			break;

		default:
			break;
	}

	return 0;
}

static uint8 fmengine_auto_threshold(void)
{//搜台 大于门限值 返回真 负责为假
	uint8 threshold;

	if(fm_engine->status.freq < fm_engine->ctrl.info.min_freq)return FALSE;
	if(fm_engine->status.freq > fm_engine->ctrl.info.max_freq)return FALSE;

	if(fm_engine->status.valid)
	{
		threshold = fmengine_dead_spot_threshold_offset();
        printf("threshold111=%d===\n",threshold);
		//threshold+=18;
		switch(fm_engine->status.band)
		{
			case RADIO_BAND_FM:
				if((fm_engine->ctrl.info.mode & AUTO_SEEK_MASK_THRESHOLD) == AUTO_SEEK_MODE_DX)
				{//FM->DX
					threshold += fm_engine->info.fm_threshold;
				}
				else
				{//FM->LOC
					threshold += fm_engine->info.fm_loc_threshold;
				}
				break;

			case RADIO_BAND_AM:
				if((fm_engine->ctrl.info.mode & AUTO_SEEK_MASK_THRESHOLD) == AUTO_SEEK_MODE_DX)
				{//AM->DX
					threshold += fm_engine->info.am_threshold;
				}
				else
				{//FM-LOC
					threshold += fm_engine->info.am_loc_threshold;
				}
				break;

			default:
				return FALSE;
		}
		printf("threshold222=%d===\n",threshold);
		printf("signal333334=%d===\n",fm_engine->status.signal);

	#if defined(_DISABLE_FM_AGC_)
		if((fm_engine->status.freq >= 8750)&&(fm_engine->status.freq <= 9600))
		{
			if(fm_engine->status.signal > (threshold-10))
			{//当前信号强度大于门限值，为有台
				printf("freq5555=%8d===\n",fm_engine->status.freq);
				return TRUE;
			}
		}
		else if((fm_engine->status.freq > 9600)&&(fm_engine->status.freq <= 10300))
		{
			if(fm_engine->status.signal > (threshold-5))
			{//当前信号强度大于门限值，为有台
				printf("freq5555=%8d===\n",fm_engine->status.freq);
				return TRUE;
			}
		}
		else
		{
			if(fm_engine->status.signal > threshold)
			{//当前信号强度大于门限值，为有台
				printf("freq5555=%8d===\n",fm_engine->status.freq);
				return TRUE;
			}
		}
	#else
		if(fm_engine->status.signal > threshold)
		{//当前信号强度大于门限值，为有台
			printf("freq5555=%8d===\n",fm_engine->status.freq);
        #if defined(_ADD_SPECIAL_CH_DISP_)
			global.fm_disp = TRUE;
			global.fm_time = 3;// 1/2秒
        #endif
			return TRUE;
		}
	#endif
	}
	return FALSE;
}

static uint8 fmengine_hard_auto_start(uint16 start_freq)
{
	if(fm_engine->ctrl.hard_auto)return TRUE;

	if(!start_freq)
	{
		start_freq = fm_engine->status.freq;
	}

	if(fm_engine->ctrl.dir == UP)
	{
		if(fm_engine->status.freq + fm_engine->ctrl.info.step > fm_engine->ctrl.info.max_freq)
		{
			start_freq = fm_engine->ctrl.info.min_freq;
		}
	}
	else
	{
		if(fm_engine->status.freq - fm_engine->ctrl.info.step < fm_engine->ctrl.info.min_freq)
		{
			start_freq = fm_engine->ctrl.info.max_freq;
		}
	}

	engine_hard_t hard;
	hard.enable = ENABLE;
	hard.ctrl.start.start_freq 	= start_freq;
	hard.ctrl.start.min_freq 	= fm_engine->ctrl.info.min_freq;
    hard.ctrl.start.max_freq 	= fm_engine->ctrl.info.max_freq;
    hard.ctrl.start.step 		= fm_engine->ctrl.info.step;
	hard.ctrl.start.dir 		= fm_engine->ctrl.dir;

	uint8 result = tuner_drv_func.tuner_hard(&hard);

	if(result)
	{
		fm_engine->ctrl.hard_auto = TRUE;
	}

	return result;
}

static uint8 fmengine_hard_auto_stop(uint16 stop_freq)
{
	if(!fm_engine->ctrl.hard_auto)return TRUE;

	if(stop_freq != 0xFFFF)
	{
		if(!stop_freq)
		{
			stop_freq = fm_engine->status.freq;
		}

		if(fm_engine->status.freq > fm_engine->ctrl.info.max_freq)
		{
			stop_freq = fm_engine->ctrl.info.max_freq;
		}
		else if(fm_engine->status.freq < fm_engine->ctrl.info.min_freq)
		{
			stop_freq = fm_engine->ctrl.info.min_freq;
		}
	}

	engine_hard_t hard;
	hard.enable = DISABLE;
	hard.ctrl.stop.stop_freq = stop_freq;
	hard.ctrl.stop.status 	 = &fm_engine->status;

	uint8 result = tuner_drv_func.tuner_hard(&hard);

	if(result)
	{
		fm_engine->ctrl.hard_auto = FALSE;
	}

	return result;
}

static uint8 fmengine_check_complete(void)
{//检测硬件搜台是否完成  完成为TRUE  否则为FALSE
	uint8 loop;

	for(loop = 0; loop < 50; loop++)
	{
		if(!fmengine_get_status(NULL))return FALSE;

		if(fm_engine->status.stc)
		{
			return TRUE;
		}
		OSTimeDly(1);
	}

	return FALSE;
}

static uint8 fmengine_work_normal(void)
{
	if(fm_engine->ctrl.hard_auto)
	{
		if(!fmengine_hard_auto_stop(0xFFFF))return FALSE;
	}
	else
	{
		if(!fmengine_check_complete())return FALSE;
	}

	return TRUE;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 fmengine_get_status(engine_status_t *status)
{
    engine_status_t engine_status_tmp;
    if(tuner_drv_func.tuner_status(&engine_status_tmp))
    {
        memcpy(&fm_engine->status, &engine_status_tmp, sizeof(engine_status_t));
		if(status != NULL)
		{
			*status = fm_engine->status;
		}
		return TRUE;
    }
	return FALSE;
}

static uint8 fmengine_get_stereo(uint8 *stereo)
{
	if(fmengine_get_status(NULL))
	{
		*stereo = fm_engine->status.stereo;
		return TRUE;
	}
	return FALSE;
}

static uint8 fm_module_init(engine_init_t *init)
{
	if(tuner_drv_func.tuner_init(init->band, init->min_freq, init->max_freq, init->freq))
	{
		return fmengine_get_status(NULL);
	}
	return FALSE;
}

static uint8 fmengine_exit(void)
{
    return tuner_drv_func.tuner_exit();
}

static uint8 fmengine_mute(uint8 mute)
{
    return tuner_drv_func.tuner_setting(TUR_SET_MUTE, (void *)(int)mute);
}

static uint8 fmengine_set_freq(uint16 freq)
{
	if(tuner_drv_func.tuner_freq(freq))
	{
		fm_engine->status.freq = freq;
		return TRUE;
	}
	return FALSE;
}

static uint8 fmengine_set_stereo(uint8 stereo)
{
	return tuner_drv_func.tuner_setting(TUR_SET_STEREO, (void *)(int)stereo);
}

static uint8 fmengine_seek_threshold(void)
{
	uint8 threshold;

	fm_engine_printf("fm_engine->status.valid:%d\n", fm_engine->status.valid);
	if(fm_engine->status.valid)
	{
		threshold = fmengine_dead_spot_threshold_offset();

		switch(fm_engine->status.band)
		{
			case RADIO_BAND_FM:
				threshold += fm_engine->info.fm_threshold;
				break;
			case RADIO_BAND_AM:
				threshold += fm_engine->info.am_threshold;
				break;
			default:
				return FALSE;
		}

		fm_engine_printf("fm_engine->status.signal:%d,threshold:%d\n", fm_engine->status.signal,threshold);
		if(fm_engine->status.signal > threshold)
		{
			return TRUE;
		}
	}

	return FALSE;
}

static uint8 fmengine_set_softseek(uint16 freq, engine_status_t *status)
{
	if(tuner_drv_func.tuner_search(freq, &fm_engine->status))
	{
		uint8 valid = fmengine_seek_threshold();
	#ifdef  TD_ADD_NEW_APP
		extern void new_app_radio_bandinfo(u8 band,u8 channel,u16 freq);
		new_app_radio_bandinfo(GET_RADIO_BAND,GET_BAND_FREQ_INDEX,GET_BAND_FREQ(GET_BAND_FREQ_INDEX));
	#endif
		if(status != NULL)
		{
			memcpy(status, &fm_engine->status, sizeof(engine_status_t));
			status->valid = valid;
		}

		return TRUE;
	}

	return FALSE;
}

static uint8 fmengine_auto_seek(engine_auto_seek_t* auto_seek)
{
	if(!fmengine_work_normal())return FALSE;

	fm_engine->ctrl.flag = FALSE;

    //软件搜台接口函数参数
    fm_engine->ctrl.info.min_freq 	= auto_seek->min_freq;
	fm_engine->ctrl.info.max_freq 	= auto_seek->max_freq;
	fm_engine->ctrl.info.step 		= auto_seek->step;
	fm_engine->ctrl.info.mode 		= auto_seek->mode;

	if((fm_engine->ctrl.info.mode & AUTO_SEEK_MASK_DIR) == AUTO_SEEK_MODE_UP)
	{
		fm_engine->ctrl.dir = UP;
	}
	else
	{
		fm_engine->ctrl.dir = DOWN;
	}

	if(fm_engine->info.hard_auto)
	{
		switch(fm_engine->ctrl.info.mode & AUTO_SEEK_MASK_START)
		{
			case AUTO_SEEK_MODE_CURRENT:
				fm_engine->ctrl.freq = fm_engine->status.freq;
				break;
			case AUTO_SEEK_MODE_MIN:
				fm_engine->ctrl.freq = fm_engine->ctrl.info.min_freq;
				break;
			case AUTO_SEEK_MODE_MAX:
				fm_engine->ctrl.freq = fm_engine->ctrl.info.max_freq;
				break;
		}

		if(!fmengine_hard_auto_start(fm_engine->ctrl.freq))return FALSE;
	}
	else
	{
		switch(fm_engine->ctrl.info.mode & AUTO_SEEK_MASK_START)
		{
			case AUTO_SEEK_MODE_CURRENT:
				fm_engine->ctrl.freq = fm_engine->status.freq;
				if(!tuner_drv_func.tuner_freq(fmengine_next_freq()))return FALSE;
				break;
			case AUTO_SEEK_MODE_MIN:
				fm_engine->ctrl.freq = fm_engine->ctrl.info.min_freq;
				if(!tuner_drv_func.tuner_freq(fm_engine->ctrl.freq))return FALSE;
				break;
			case AUTO_SEEK_MODE_MAX:
				fm_engine->ctrl.freq = fm_engine->ctrl.info.max_freq;
				if(!tuner_drv_func.tuner_freq(fm_engine->ctrl.freq))return FALSE;
				break;
		}
	}

	fm_engine->ctrl.flag 		= TRUE;
	fm_engine->ctrl.refur_flag 	= TRUE;
	fm_engine->ctrl.round_flag 	= FALSE;
	fm_engine->ctrl.prev_freq 	= fm_engine->ctrl.freq;
	fm_engine->ctrl.round_count = 0;
#if defined(_ENABLE_RADIO_RDS_)
	fm_engine->ctrl.wait_flag 	= FALSE;
#endif

	return TRUE;
}

static uint8 fmengine_auto_break(engine_status_t *status)
{
	if(!fmengine_work_normal())return FALSE;

	fm_engine->ctrl.flag = FALSE;
#if defined(_ENABLE_RADIO_RDS_)
	fm_engine->ctrl.wait_flag = FALSE;
#endif

	if(status != NULL)
	{
		memcpy(status, &fm_engine->status, sizeof(engine_status_t));
	}

	return TRUE;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/

static void fmengine_auto_refur_check(void)
{
#if defined(_TUNER_DRIVER_AC69xx_)
	if(fm_engine->info.type == TUN_DRV_AC69xx)
	{
		fm_engine->ctrl.refur_flag = TRUE;
	}
	else
#endif
#if defined(_TUNER_DRIVER_SI475x_)
	if(fm_engine->info.type == TUN_DRV_SI475x)
	{
		fm_engine->ctrl.refur_flag = TRUE;
	}
	else
#endif
#if defined(_TUNER_DRIVER_TDA778x_)
	if(fm_engine->info.type == TUN_DRV_TDA778x)
	{
		fm_engine->ctrl.refur_flag = TRUE;
	}
	else
#endif
#if defined(_TUNER_DRIVER_TDA7708_)
	if(fm_engine->info.type == TUN_DRV_TDA7708)
	{
		fm_engine->ctrl.refur_flag = TRUE;
	}
	else
#endif
	{
	#if !defined(_SUPPORT_REAL_TIME_REFRESH_FREQ_)
		if((++fm_engine->ctrl.refur_count) >= (100/FM_ENGINE_POLLING_TIME))
	#endif
		{
			fm_engine->ctrl.refur_count = 0;
			fm_engine->ctrl.refur_flag = TRUE;
		}
	}
}

static uint8 fmengine_auto_limit(void)
{
	if(fm_engine->ctrl.prev_freq == fm_engine->status.freq)return FALSE;
	if(!fm_engine->ctrl.hard_auto)
	{
		fm_engine->ctrl.prev_freq = fm_engine->status.freq;
		return FALSE;
	}

	uint8 limit = FALSE;
	uint8 seek_mode = fm_engine->ctrl.info.mode & AUTO_SEEK_MASK_SEEK;

	if((fm_engine->status.freq < fm_engine->ctrl.info.min_freq) || (fm_engine->status.freq > fm_engine->ctrl.info.max_freq))
	{
		limit = TRUE;
	}

	if(seek_mode == AUTO_SEEK_MODE_ROUND)
	{
		if(fm_engine->ctrl.round_flag)
		{
			if(fm_engine->ctrl.dir == UP)
			{
				if(fm_engine->status.freq < fm_engine->ctrl.prev_freq)
				{
					fm_engine->ctrl.round_count++;
				}
				if(fm_engine->ctrl.round_count == 1)
				{
					if(fm_engine->status.freq >= fm_engine->ctrl.freq)
					{
						limit = TRUE;
					}
				}
				else if(fm_engine->ctrl.round_count > 1)
				{
					limit = TRUE;
				}
			}
			else
			{
				if(fm_engine->status.freq > fm_engine->ctrl.prev_freq)
				{
					fm_engine->ctrl.round_count++;
				}
				if(fm_engine->ctrl.round_count == 1)
				{
					if(fm_engine->status.freq <= fm_engine->ctrl.freq)
					{
						limit = TRUE;
					}
				}
				else if(fm_engine->ctrl.round_count > 1)
				{
					limit = TRUE;
				}
			}
		}
		else
		{
			fm_engine->ctrl.round_flag = TRUE;
		}
	}

	fm_engine->ctrl.prev_freq = fm_engine->status.freq;
	return limit;
}

static uint8 fmengine_post_msg(auto_reply_e reply)
{
	fm_engine->ctrl.refur_flag = FALSE;
	fm_engine->ctrl.refur_count = 0;

#if defined(_ENABLE_RADIO_RDS_)

	if(fm_engine->ctrl.return_func != NULL)
	{
		typedef uint8 (*callback_func)(uint8, uint16, uint16, uint8 );

		if(fmengine_rds_det())
		{
			reply |= AUTO_REPLY_RDS;
		}
		return ((callback_func)fm_engine->ctrl.return_func)(reply, fm_engine->status.freq, fm_engine->ctrl.rds_pi, fm_engine->status.signal);
	}

#else

	if(fm_engine->ctrl.return_func != NULL)
	{
		typedef uint8 (*callback_func)(uint8, uint16);

		return ((callback_func)fm_engine->ctrl.return_func)(reply, fm_engine->status.freq);
	}

#endif

	return FALSE;
}

extern void radio_seek_stop(void);

static void fmengine_auto_detect(void)
{
    uint8 result;
	uint8 limit;

	fmengine_auto_refur_check();

#if defined(_ENABLE_RADIO_RDS_)
	if(fmengine_rds_det())
	{
		if(fm_engine->ctrl.wait_flag)
		{
			goto fm_auto_rds_det;
		}
	}
#endif

	if(!fmengine_get_status(NULL))
	{
		if(fm_engine->ctrl.hard_auto)
		{
			fmengine_hard_auto_stop(0);
		}
		fm_engine->ctrl.flag = FALSE;
		fm_engine_puts("auto_detect_status--->err\n");
		fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_REFUR);
		return;
	}

	limit = fmengine_auto_limit();
	if(limit)
	{
		fm_engine_printf("fmengine_auto_limit--->freq:%d\n", fm_engine->status.freq);
		uint16 stop_freq = 0;
		if(fm_engine->status.freq < fm_engine->ctrl.info.min_freq)
		{
			stop_freq = fm_engine->ctrl.info.min_freq;
		}
		else if(fm_engine->status.freq > fm_engine->ctrl.info.max_freq)
		{
			stop_freq = fm_engine->ctrl.info.max_freq;
		}

		if(!fmengine_hard_auto_stop(stop_freq))
		{
			fm_engine->ctrl.flag = FALSE;
			fm_puts("auto_limit_stop--->err\n");
			fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_REFUR);
			return;
		}
		goto auto_seek_limit;
	}
	
#if defined(_EVB_SPECIAL_SEEK__)
#if defined(_TUNER_DRIVER_TDA778x_)
	ew_printf("-save_freq-:%d\n",radio_ctrl->save_freq);
	ew_printf("fm_engine->status.freq:%d\n",fm_engine->status.freq);
	if(fm_engine->status.freq == radio_ctrl->save_freq)
	{
		ew_puts("-----_TUNER_DRIVER_TDA778x_-----\n");
		radio_ctrl->save_freq = 0;
		fm_engine->ctrl.flag = FALSE;
		fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_STATION|AUTO_REPLY_REFUR);
	}
#endif
#endif

	if(fm_engine->status.stc)
	{
		if(fm_engine->ctrl.hard_auto)
		{
			if(!fmengine_hard_auto_stop(0))
			{
				fm_engine->ctrl.flag = FALSE;
				fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_REFUR);
				return;
			}
		}

	#if defined(_ENABLE_RADIO_RDS_)
    	if(fmengine_rds_det() && fmengine_auto_threshold())
    	{
    		fm_engine->ctrl.wait_flag 	= TRUE;
    		fm_engine->ctrl.wait_timer 	= 1000;
    		fmengine_post_msg(AUTO_REPLY_REFUR);

    	fm_auto_rds_det:
			if(fm_engine->ctrl.wait_timer > FM_ENGINE_POLLING_TIME)
			{
				fm_engine->ctrl.wait_timer -= FM_ENGINE_POLLING_TIME;
				switch(rds_info_found(&fm_engine->ctrl.rds_pi))
				{
					case RDS_FOUND_WAIT:return;
					case RDS_FOUND_FINISH:
						fm_engine->status.valid = FALSE;
						break;
				}
				fm_engine->ctrl.wait_flag = FALSE;
			}
			else
			{
				fm_engine->status.valid = FALSE;
				fm_engine->ctrl.wait_flag = FALSE;
			}
    	}
	#endif
	
	auto_seek_limit:
		switch(fm_engine->ctrl.info.mode & AUTO_SEEK_MASK_SEEK)
		{
			case AUTO_SEEK_MODE_STATION:
				if((!limit) && fmengine_auto_threshold())
				{
				#if defined(_ADD_SPECIAL_CH_DISP_)
					global.fm_disp = FALSE;
				#endif
					fm_engine_puts("auto_seek_station--->stop|station\n");
					fm_engine->ctrl.flag = FALSE;
					fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_STATION|AUTO_REPLY_REFUR);
				}
				else
				{
					if(fm_engine->ctrl.refur_flag)
					{
						fmengine_post_msg(AUTO_REPLY_REFUR);
					}
				
				#if defined(_EVB_SPECIAL_SEEK__)
					if(GET_BAND_TEMP_FREQ == radio_ctrl->save_freq)
					{
						fm_engine->ctrl.flag = FALSE;
						fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_STATION|AUTO_REPLY_REFUR);
					}
				#endif
				#if defined(_ENABLE_RADIO_RDS_)
					if(fm_engine->ctrl.rds_enable)
					{
						rds_ctrl_reset();
					}
				#endif

					if(fm_engine->info.hard_auto)
					{
						result = fmengine_hard_auto_start(0);
					}
					else
					{
						result = tuner_drv_func.tuner_freq(fmengine_next_freq());
					}
					if(!result)
					{
						fm_engine->ctrl.flag = FALSE;
						fm_engine_puts("auto_seek_station---err--->stop\n");
						fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_REFUR);
						return;
					}
				}
				break;

			case AUTO_SEEK_MODE_ROUND:
				if(limit)
				{
					fm_engine_puts("fmengine_auto_round_limit\n");
					fm_engine->ctrl.flag = FALSE;
					fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_REFUR);
				}
				else
				{
					if(fm_engine->ctrl.round_flag && (fm_engine->ctrl.freq == fm_engine->status.freq))
					{
						fm_engine->ctrl.flag = FALSE;
						if(fmengine_auto_threshold())
						{
						#if defined(_ADD_SPECIAL_CH_DISP_)
						    global.fm_disp = FALSE;
						#endif
							fm_engine_puts("auto_seek_round---111--->stop|station\n");
							fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_STATION|AUTO_REPLY_REFUR);
						}
						else
						{
							fm_engine_puts("auto_seek_round---111--->stop\n");
							fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_REFUR);
						}
					}
					else
					{
						fm_engine->ctrl.round_flag = TRUE;
						if(fmengine_auto_threshold())
						{
							fm_engine_puts("auto_seek_round---222--->station\n");
							uint8 stop = fmengine_post_msg(AUTO_REPLY_STATION|AUTO_REPLY_REFUR);
							if(stop)
							{
								fm_engine->ctrl.flag = FALSE;
								fm_engine_puts("auto_seek_round---over_stop---\n");
								return;
							}
						}
						else
						{
							if(fm_engine->ctrl.refur_flag)
							{
								fmengine_post_msg(AUTO_REPLY_REFUR);
							}
						}

					#if defined(_ENABLE_RADIO_RDS_)
						if(fm_engine->ctrl.rds_enable)
						{
							rds_ctrl_reset();
						}
					#endif

						if(fm_engine->info.hard_auto)
						{
							result = fmengine_hard_auto_start(0);
						}
						else
						{
							result = tuner_drv_func.tuner_freq(fmengine_next_freq());
						}
						if(!result)
						{
							fm_engine->ctrl.flag = FALSE;
							fm_engine_puts("auto_seek_round---err--->stop\n");
							fmengine_post_msg(AUTO_REPLY_STOP|AUTO_REPLY_REFUR);
							return;
						}
					}
				}
				break;
		}
	}
	else
	{
		if(fm_engine->ctrl.hard_auto)
		{
			if(fm_engine->ctrl.refur_flag)
			{
				fmengine_post_msg(AUTO_REPLY_REFUR);
			}
		}
	}
}

#if defined(_ENABLE_RADIO_RDS_)

static int fm_engine_order_check(int *msg)
{
	int result = TRUE;

#if defined(_DISABLE_SYSTEM_MODE_)

#if defined(_DISABLE_MODE_FUNTION_)
	switch(*msg)
	{
		case MSG_SYS_MODE:
			//rds_ctrl_pause();
			result = TRUE;
			break;

		case MSG_SYS_MUTE ... MSG_RDS_REG:
		case MSG_SYS_POWER:
	#if defined(_ENABLE_BT_FIRST_)
		case MSG_BT_ANSWER_CALL://交通台来电可以接听电话
	#endif
			result = FALSE;
			break;
	}
#else

#if defined(_ENABLE_POWER_OFF_)
	switch(*msg)
	{
		case MSG_SYS_POWER ... MSG_RDS_REG:
	#if defined(_ENABLE_BT_FIRST_)
		case MSG_BT_ANSWER_CALL://交通台来电可以接听电话
	#endif
			result = FALSE;
			break;
	}
#else
	switch(*msg)
	{
		case MSG_SYS_POWER:
			rds_ctrl_pause();
			result = TRUE;
			break;

		case MSG_SYS_MODE ... MSG_RDS_REG:
	#if defined(_ENABLE_BT_FIRST_)
		case MSG_BT_ANSWER_CALL://交通台来电可以接听电话
	#endif
	#if defined(_ENABLE_BT_REJECT_)
		case MSG_BT_CALL_REJECT://交通台来电可以挂断电话
	#endif

			result = FALSE;
			break;
	}
#endif
#endif

#else

#if defined(_ENABLE_POWER_OFF_)
    switch(*msg)
    {
    	case MSG_SYS_POWER:
    		result = FALSE;
    		break;
    	case MSG_SYS_MODE:
    		rds_ctrl_pause();
    		result = TRUE;
    		break;
    
    	case MSG_SYS_MUTE ... MSG_RDS_REG:
    #if defined(_ENABLE_BT_FIRST_)
    	case MSG_BT_ANSWER_CALL://交通台来电可以接听电话
    #endif
    		result = FALSE;
    		break;
    }

#else
	switch(*msg)
	{
		case MSG_SYS_POWER:
		case MSG_SYS_MODE:
			rds_ctrl_pause();
			result = TRUE;
			break;

		case MSG_SYS_MUTE ... MSG_RDS_REG:
	#if defined(_ENABLE_BT_FIRST_)
		case MSG_BT_ANSWER_CALL://交通台来电可以接听电话
	#endif
			result = FALSE;
			break;
	}
#endif
#endif
	return result;
}


static void fm_engine_rds(int cmd, void *param)
{
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

	//fm_puts("fm_engine_rds\n");

	switch(cmd)
	{
		case RDS_ENGINE_CMD_FREQ:
			{
				uint16 freq = (uint16)param;

				//fm_engine_puts("fm_engine_rds\n");
				fm_engine_printf("%s--->freq:%d\n", __FUNCTION__, freq);
				if(fm_engine->ctrl.rds_front)
				{
					fm_engine->status.freq = freq;
			    #if defined(_DISABLE_RDS_REPLACE_)
			    	fmengine_post_msg(AUTO_REPLY_RESET|AUTO_REPLY_REFUR);
			    #else
			    	fmengine_post_msg(AUTO_REPLY_SEARCH|AUTO_REPLY_REFUR);
			    #endif
				}
				setup.rds.cur_freq = freq;
			}
			break;

		case RDS_ENGINE_CMD_MUTE:
			{
				uint8 mute = (uint8)param;
				
				ew_puts("RDS_ENGINE_CMD_MUTE\n");
				fm_engine->ctrl.rds_mute = mute;
				
				if(global.aud_mute)
				{
					disp_send_msg(DISP_EVENT_MUTE);
				}
				
				fm_engine_printf("%s--->mute:%d\n", __FUNCTION__, mute);
				if(fm_engine->ctrl.rds_front)
				{
					os_taskq_post_msg(DISP_TASK_NAME, 2, DISP_EVENT_INSIDE_MUTE, param);
				}
			}
			break;

		case RDS_ENGINE_CMD_TRAFFIC:
			{
				extern ext_check_f order_ext_check;
				uint8 traffic = (uint8)param;

				fm_engine_printf("%s--->traffic:%d, front:%d\n", __FUNCTION__, traffic, fm_engine->ctrl.rds_front);
				global.rds_ta = traffic;
				if(traffic)
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, SYS_EVENT_TA_ENTER);
					order_ext_check = fm_engine_order_check;
				}
				else
				{
					os_taskq_post_msg(MAIN_TASK_NAME, 1, SYS_EVENT_TA_EXIT);
					order_ext_check = NULL;
				}

				if(!fm_engine->ctrl.rds_front)
				{
					ew_puts("RDS_ENGINE_CMD_TRAFFIC\n");
					if(traffic)
					{
						ew_puts("RDS_ENGINE_CMD_TRAFFIC1111\n");
					#if defined(_BT_PAUSE_MUTE_)
						sys_mute_ctrl(DISABLE, 50);
					#endif
						fm_engine->ctrl.rds_channel = audio_drv_channel();
			        #if defined(_AUDIO_DRIVER_AP3768_)
			        	if(audio_drv_channel() == AUDIO_CHANNEL_MUSIC_ON)
			        	{
			        		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_OFF, TRUE);
			        	}
			        	else if(audio_drv_channel() == AUDIO_CHANNEL_AUX_ON)
			        	{
			        		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_AUX_OFF, TRUE);
			        	}
			        	OSTimeDly(10);
			        	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RDS_ON, TRUE);
			        #else
			           audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RADIO_ON, TRUE);
			        #endif
					}
			    #if defined(_AUDIO_DRIVER_AP3768_)
			    	else if(AUDIO_CHANNEL_RDS_ON == audio_drv_channel())
			    	{
			    		ew_puts("RDS_ENGINE_CMD_TRAFFIC222\n");
			    		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RDS_OFF, TRUE);
			    		OSTimeDly(10);
			    		audio_drv_ctrl(AUD_CTRL_CHANNEL, fm_engine->ctrl.rds_channel, TRUE);
			    	}
			    #else
			    	else if(AUDIO_CHANNEL_RADIO_ON == audio_drv_channel())
			    	{
			    		ew_puts("RDS_ENGINE_CMD_TRAFFIC222\n");
			    		audio_drv_ctrl(AUD_CTRL_CHANNEL, fm_engine->ctrl.rds_channel, TRUE);
			    	}
			    #endif
				}
				audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
			#if KEY_TONE_EN
			#if defined(_ENABLE_BEEP_)
				if(traffic && setup.aud_beep)
				{
					send_key_voice(500);
				}
			#endif
			#endif
			}
			break;

		case RDS_ENGINE_CMD_ICON:
			{
				int ctrl = (int)param;
				
				//fm_engine_printf("%s--->rds_icon:0x%x\n", __FUNCTION__, ctrl);
				os_taskq_post_event(DISP_TASK_NAME, 2, DISP_EVENT_RDS_ICON, ctrl);
			}
			break;

		case RDS_ENGINE_CMD_TXT:
			{
				uint8 alarm = (((int)param) >> 8) & 0xFF;
				uint8 txt 	= ((int)param) & 0xFF;
				
				fm_engine->ctrl.rds_txt = txt;
				fm_engine_printf("%s--->alarm:%d, txt:%d\n", __FUNCTION__, alarm, txt);
				if(alarm || fm_engine->ctrl.rds_front)
				{
					os_taskq_post_msg(DISP_TASK_NAME, 2, DISP_EVENT_RDS_TXT, txt);
				}
			}
			break;

		case RDS_ENGINE_CMD_PSN:
			{
				if(fm_engine->ctrl.rds_front)
				{
					int psn = (int)param;
					
					fm_engine_printf("%s--->rds_psn:0x%x\n", __FUNCTION__, psn);
					os_taskq_post_msg(FM_TASK_NAME, 2, MSG_FM_RDS_PSN, psn);
				}
			}
			break;

		case RDS_ENGINE_CMD_DATE:
			{
				int data = (int)param;
				
				//fm_engine_printf("%s--->rds_data:0x%x\n", __FUNCTION__, data);
				os_taskq_post_msg(DISP_TASK_NAME, 2, DISP_EVENT_RDS_DATA, data);
			}
			break;

		default:
			break;
	}
}

void radio_rds_init(void)
{
	fm_engine_puts("radio_rds_init\n");

	if(setup.rds.enable)
	{
		fm_engine_create();

		if(fm_engine_work)
		{
			if(fm_engine->info.rds_support)
			{
				fm_engine_msg(MSG_FMENGINE_RDS_BACK_START, NULL, NULL);
				fm_engine_msg(MSG_FMENGINE_RDS_ENABLE, (void *)ENABLE, NULL);
			}
			else
			{
				fm_engine_delete();
			}
		}
	}
}

void radio_rds_exit(void)
{
	fm_engine_puts("radio_rds_exit\n");
	fm_engine_delete();
}

#endif

/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
static void fm_engine_timer(void *param)
{
#if defined(_SUPPORT_C912DAB_)
	if(gC912detOk)
	{
        os_taskq_post_msg(FM_ENGINE_TASK_NAME, 1, MSG_FMENGINE_TIMER);
	}
	else
#endif
	if(fm_engine->ctrl.flag)
	{
		if(!fm_engine->ctrl.timer_enter)
		{
			os_taskq_post_msg(FM_ENGINE_TASK_NAME, 1, MSG_FMENGINE_TIMER);
		}
	}
#if defined(_ENABLE_RADIO_RDS_)
	else if(fm_engine->ctrl.rds_enable)
	{
		if(!fm_engine->ctrl.timer_enter)
		{
			os_taskq_post_msg(FM_ENGINE_TASK_NAME, 1, MSG_FMENGINE_RDS_POLLING);
		}
	}
#endif
}
#if defined(_TUNER_DRIVER_SI473x_)
extern uint8 SendSi473xVolume(uint8);
#endif
#if defined(_TUNER_DRIVER_EW730_)
extern uint8 EW730_SET_VOL(uint8 VOL);
#endif
static void fm_engine_task(void *p)
{
    int msg[3];
	uint8 tuner_succeed = FALSE;
	uint8 tuner_init = FALSE;//初始化OK 值为TRUE
	uint16 tuner_freq;
	OS_SEM *sem;

	fm_puts("fm_engine_task\n");

	fm_engine = (fm_engine_t *)malloc_fun(fm_engine ,sizeof(fm_engine_t), 0);
	if(fm_engine == NULL)
	{
		fm_puts("fm_engine err\n");
        while(1);
	}

	if(tuner_drv_func.tuner_detect())
	{
		tuner_drv_func.tuner_info(TUR_INFO_DRV, 				&fm_engine->info.type);

		tuner_drv_func.tuner_info(TUR_INFO_FM_RSSI, 			&fm_engine->info.fm_threshold);
		tuner_drv_func.tuner_info(TUR_INFO_FM_LOC_RSSI,			&fm_engine->info.fm_loc_threshold);
		tuner_drv_func.tuner_info(TUR_INFO_FM_DEAD_SPOT_LENGTH, &fm_engine->info.fm_dead_spot_length);
		tuner_drv_func.tuner_info(TUR_INFO_FM_DEAD_SPOT_TAB,	&fm_engine->info.fm_dead_spot_tab);

		tuner_drv_func.tuner_info(TUR_INFO_AM_RSSI, 			&fm_engine->info.am_threshold);
		tuner_drv_func.tuner_info(TUR_INFO_AM_LOC_RSSI, 		&fm_engine->info.am_loc_threshold);
		tuner_drv_func.tuner_info(TUR_INFO_AM_DEAD_SPOT_LENGTH, &fm_engine->info.am_dead_spot_length);
		tuner_drv_func.tuner_info(TUR_INFO_AM_DEAD_SPOT_TAB,	&fm_engine->info.am_dead_spot_tab);

		tuner_drv_func.tuner_info(TUR_INFO_AUTOSEEK, 			&fm_engine->info.hard_auto);
		tuner_drv_func.tuner_info(TUR_INFO_FM_ONLY, 			&fm_engine->info.fm_only);
	#if defined(_ENABLE_RADIO_RDS_)
		tuner_drv_func.tuner_info(TUR_INFO_RDS, 				&fm_engine->info.rds_support);
	#endif
    #if defined(_ENABLE_ST_MONO_)
	#if defined(_TUNER_DRIVER_AC69xx_)
	#if defined(_ENABLE_6218_ST_MONO_)
	    if(fm_engine->info.type == TUN_DRV_RTC6218||fm_engine->info.type == TUN_DRV_AC69xx)	
	#else		
    	if(fm_engine->info.type == TUN_DRV_AC69xx)
	#endif
    	{
    	#if !defined(_DISABLE_DEFAULT_MONO_)
    		if(setup.st_flag)
    		{
    			SET_RADIO_ST(FALSE);
    			setup.st_flag = FALSE;
    		}
		#endif
    	}
    #endif
	#endif
	
	#if defined(SUPPORT_ANTENNA_DISPLAY_)
		if(fm_engine->info.type == TUN_DRV_SI473x)
		{
			global.radio_type = TRUE;
		}
		else
		{
			global.radio_type = FALSE;
		}
	#endif
	
		timer_reg_func(fm_engine_timer, NULL, FM_ENGINE_POLLING_TIME);
	
		tuner_succeed = TRUE;
	}
	else
	{
		fm_engine->info.type = TUN_DRV_NULL;
	#ifdef TD_ADD_USER_UART
		Close_Uart2();
	#endif
	}

	fm_engine_work = TRUE;

    while(1)
    {
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);

		if(!tuner_succeed)
		{
			if(msg[0] != SYS_EVENT_DEL_TASK)
			{
				continue;
			}
		}

		sem = NULL;

        switch(msg[0])
		{
	        case SYS_EVENT_DEL_TASK:
				fm_puts("fm_engine--->SYS_EVENT_DEL_TASK\n");
	            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
	            	fm_engine_work = FALSE;
					if(tuner_succeed)
					{
						timer_unreg_func(fm_engine_timer);
					#if defined(_ENABLE_RADIO_RDS_)
						if(fm_engine->ctrl.rds_enable)
						{
							rds_ctrl_exit();
						}
					#endif
						fmengine_exit();
					}
					free_fun((void**)&fm_engine);
	                os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case MSG_FMENGINE_TIMER:
			#if defined(_SUPPORT_C912DAB_)
				if(gC912detOk)
				{
					uart_interrupt_receive_write(0x71);
					uart_interrupt_receive_write(0x31);
				}
				else
			#endif
				if(fm_engine->ctrl.flag)
				{
					fm_engine->ctrl.timer_enter = TRUE;
					fmengine_auto_detect();
					fm_engine->ctrl.timer_enter = FALSE;
				}
				break;

			/* 返回接口*/
			case MSG_FMENGINE_RETURN:
				{
					
					fm_printf("MSG_FMENGINE_RETURN:0x%x\n", msg[1]);
					fm_engine->ctrl.return_func = (void *)msg[1];
				}
				break;

			/* 设置频点*/
			case MSG_FMENGINE_SETFREQ:
				if(tuner_init)
				{
					uint16 freq = (uint16)msg[1];
					fm_printf("MSG_FMENGINE_SETFREQ, freq:%d\n", freq);

				#if defined(_ENABLE_RADIO_RDS_)
					if(fm_engine->ctrl.rds_enable)
					{
						rds_ctrl_reset();
					}
				#endif

					fmengine_set_freq(freq);
				}
				else
				{
					uint16 freq = (uint16)msg[1];
					tuner_freq = freq;
				}
				break;

		    /* 静音或解除静音*/
		    case MSG_FMENGINE_SETMUTE:
				fm_puts("MSG_FMENGINE_SETMUTE\n");
				if(tuner_init)
				{
					uint8 mute = (uint8)msg[1];
					fm_printf("MSG_FMENGINE_SETMUTE, mute:%d\n", mute);
					fmengine_mute(mute);
		    	}
				break;

		    /* 启动软件搜台*/
		    case MSG_FMENGINE_SET_SOFTSEEK:
				if(tuner_init)
				{
					uint16 freq = (uint16)msg[1];
					fm_printf("MSG_FMENGINE_SET_SOFTSEEK, freq:%d\n", freq);

				#if defined(_ENABLE_RADIO_RDS_)
					if(fm_engine->ctrl.rds_enable)
					{
						rds_ctrl_reset();
					}
				#endif

					engine_status_t *status = ((engine_status_t *)msg[2]);
					fmengine_set_softseek(freq, status);
		    	}
				sem = &fm_eng_sem;//同步命令
				break;

		    /* 获取当前状态信息*/
		    case MSG_FMENGINE_GET_STATUS:
				if(tuner_init)
				{
					//fm_puts("MSG_FMENGINE_GET_STATUS\n");
					engine_status_t *status = ((engine_status_t *)msg[2]);
					fmengine_get_status(status);
		    	}
				else
				{
					engine_status_t *status = ((engine_status_t *)msg[2]);
					status->freq = tuner_freq;
				}
				break;

		    /* 获取立体声状态信息*/
		    case MSG_FMENGINE_GET_STEREO:
				if(tuner_init)
				{
					//fm_puts("MSG_FMENGINE_GET_STEREO\n");
					uint8 *stereo = (uint8 *)msg[2];
					fmengine_get_stereo(stereo);
		    	}
				else
				{
					uint8 *stereo = (uint8 *)msg[2];
					*stereo = FALSE;
				}
				break;

			/* 获取单FM 状态信息*/
			case MSG_FMENGINE_GET_FM_ONLY:
				{
					fm_puts("MSG_FMENGINE_GET_FM_ONLY\n");
					uint8 *fm_only = (uint8 *)msg[2];
					*fm_only = fm_engine->info.fm_only;
		    	}
				break;

		    /* FM 驱动IC  初始化配置*/
			case MSG_FMENGINE_INIT:
				{
					fm_puts("MSG_FMENGINE_INIT\n");

				#if defined(_ENABLE_RADIO_RDS_)
					if(tuner_init)
					{
						engine_status_t status;
						fmengine_get_status(&status);

						if(status.band == RADIO_BAND_FM)
						{//	保存当前频率
							setup.rds.cur_freq = status.freq;
							fm_printf("MSG_FMENGINE_INIT---rds_freq:%d\n", status.freq);
						}

						if(fm_engine->ctrl.rds_enable)
						{//	关闭RDS
							if(rds_ctrl_exit())
							{
								fm_engine->ctrl.rds_enable = FALSE;
							}
						}
					}
				#endif
				     

					engine_init_t *init = ((engine_init_t *)msg[1]);
					tuner_freq = init->freq;
				#ifdef	TD_ADD_SPP
					Spp_Tx_Radio_Info(tuner_freq);
				#endif		
				#if defined(_USER_TFT_SEND_)
					TFT_Tx_Radio_Info(tuner_freq);
					OSTimeDly(10);
				#endif
					tuner_init = fm_module_init(init);

				#if defined(_ENABLE_RADIO_RDS_)
					if(tuner_init && fm_engine->info.rds_support)
					{//	打开RDS
						if(rds_ctrl_enter(FM_ENGINE_POLLING_TIME, &fm_engine->ctrl.rds_func, fm_engine_rds))
						{
							fm_engine->ctrl.rds_enable = TRUE;
							rds_ctrl_busy_func(fm_engine->ctrl.rds_busy);
						}
					}
				#endif
					fm_printf("tuner_init:%d\n", tuner_init);
					sem = (OS_SEM *)msg[2];//同步命令
				}
				break;

			case MSG_FMENGINE_EXIT:
				if(tuner_init)
				{
					fm_puts("MSG_FMENGINE_EXIT\n");

				#if defined(_ENABLE_RADIO_RDS_)
					if(fm_engine->ctrl.rds_enable)
					{
						if(rds_ctrl_exit())
						{
							fm_engine->ctrl.rds_enable = FALSE;
						}
					}
				#endif

					fmengine_exit();
					tuner_init = FALSE;
				}
				sem = (OS_SEM *)msg[2];//同步命令
				break;

			/*	设置立体声解调*/
			case MSG_FMENGINE_SET_STEREO:
				if(tuner_init)
				{
					uint8 stereo = (uint8)msg[1];
					fm_printf("MSG_FMENGINE_SET_STEREO, stereo:%d\n", stereo);
					fmengine_set_stereo(stereo);
				#if defined(FM_INIT_MUTE_HJ)
					//extern void sys_mute_ctrl(uint8 mute, uint16 timer);
					//sys_mute_ctrl(DISABLE, 0);
			    #if defined(_ENABLE_BAND_MUTE_)
			    	if(!global.mute_flag)
			    	{
			    		AUDIO_MUTE_OFF();
			    	}
			    #else
			    	AUDIO_MUTE_OFF();
			    #endif
				#endif
				}
				break;

			/* 启动软件搜台*/
			case MSG_FMENGINE_AUTO_SEEK:
				if(tuner_init)
				{
					fm_puts("MSG_FMENGINE_AUTO_SEEK\n");

				#if defined(_ENABLE_RADIO_RDS_)
					if(fm_engine->ctrl.rds_enable)
					{
						rds_ctrl_reset();
					}
				#endif

					engine_auto_seek_t *auto_seek = ((engine_auto_seek_t *)msg[1]);
					fmengine_auto_seek(auto_seek);
				}
				break;

			/* 退出自动搜台*/
			case MSG_FMENGINE_AUTO_BREAK:
				if(tuner_init)
				{
					fm_puts("MSG_FMENGINE_AUTO_BREAK\n");
					engine_status_t *status = ((engine_status_t *)msg[2]);
					fmengine_auto_break(status);
				}
				sem = (OS_SEM *)msg[1];//同步命令
				break;

			/* 获取当前模组状态*/
			case MSG_FMENGINE_GET_MODUAL_STATUS:
				if(tuner_init)
				{
					fm_puts("MSG_FMENGINE_GET_MODUAL_STATUS\n");
					engine_status_t *status = ((engine_status_t *)msg[2]);
					*status = fm_engine->status;
		    	}
				else
				{
					engine_status_t *status = ((engine_status_t *)msg[2]);
					status->freq = tuner_freq;
				}
				break;
		
		#if defined(_FM_VOLSET_USE_CHIP_)
			case MSG_FMENGINE_VOLUME:
				if(tuner_init)
				{
					uint8 volume = (uint8)msg[1];
					
					fm_printf("MSG_FMENGINE_VOLUME:%d\n", volume);
				#if defined(_TUNER_DRIVER_SI473x_)
					if(fm_engine->info.type == TUN_DRV_SI473x)
					{
						SendSi473xVolume(Si4730_Vol_Set[volume]);
					}
				#endif
					
				#if defined(_TUNER_DRIVER_EW730_)
					if(fm_engine->info.type == TUN_DRV_EW730)
					{
						EW730_SET_VOL(Ew730_Vol_Set[volume]);
					}
				#endif
		    	}
				break;
		#endif
		#if defined(_ENABLE_RADIO_RDS_)
			case MSG_FMENGINE_RDS_STATE:
				{
					fm_puts("MSG_FMENGINE_RDS_STATE\n");
					uint8 *rds_enable = (uint8 *)msg[2];
					*rds_enable = fm_engine->ctrl.rds_enable;
				}
				break;

			/*RDS 后台启动*/
			case MSG_FMENGINE_RDS_BACK_START:
			case MSG_FMENGINE_RDS_BACK_RUN:
				if(fm_engine->info.rds_support)
				{
					fm_puts("MSG_FMENGINE_RDS_BACK_START\n");

					engine_init_t init;
					init.band 		= RADIO_BAND_FM;
				    init.freq 		= setup.rds.cur_freq;
				    init.min_freq 	= 8750;
					init.max_freq 	= 10800;
					tuner_freq 		= init.freq;
					tuner_init 		= fm_module_init(&init);

					fm_printf("RDS_tuner_init:%d\n", tuner_init);
				}
				sem = (OS_SEM *)msg[2];//同步命令
				break;

			/*RDS 开关*/
			case MSG_FMENGINE_RDS_ENABLE:

				if(tuner_init)
				{
					fm_printf("MSG_FMENGINE_RDS_ENABLE, rds_support:%d, msg[1]:%d\n", fm_engine->info.rds_support, msg[1]);
					if(fm_engine->info.rds_support)
					{
						uint8 enable = (uint8)msg[1];
						if(enable)
						{
							if(rds_ctrl_enter(FM_ENGINE_POLLING_TIME, &fm_engine->ctrl.rds_func, fm_engine_rds))
							{
								fm_puts("rds enter\n");
								fm_engine->ctrl.rds_enable = TRUE;
								fm_engine->ctrl.rds_front = FALSE;
							}
						}
						else
						{
							if(rds_ctrl_exit())
							{
								fm_puts("rds exit\n");
								fm_engine->ctrl.rds_enable = FALSE;
							}
						}
					}
				}
				break;

			/*RDS 前后台设置*/
			case MSG_FMENGINE_RDS_FRONT:
				if(fm_engine->info.rds_support)
				{
					uint8 front = msg[1];
					fm_printf("MSG_FMENGINE_RDS_FRONT:%d\n", front);

					if(front)
					{//	前台
						if(fm_engine->ctrl.rds_enable)
						{
							if(fm_engine->ctrl.rds_mute)
							{
								os_taskq_post_msg(DISP_TASK_NAME, 2, DISP_EVENT_INSIDE_MUTE, TRUE);
							}

							//if(fm_engine->ctrl.rds_txt)
							//{
							//	os_taskq_post_msg("DispTask", 2, DISP_EVENT_RDS_TXT, fm_engine->ctrl.rds_txt);
							//}
						}
					}
					else
					{//	后台
						if(!fm_engine->ctrl.rds_enable)
						{
							fm_engine_msg(MSG_FMENGINE_RDS_BACK_RUN, NULL, NULL);
							fm_engine_msg(MSG_FMENGINE_RDS_ENABLE, (void *)ENABLE, NULL);
						}
						else
						{
							if(fm_engine->ctrl.rds_mute)
							{
								os_taskq_post_msg(DISP_TASK_NAME, 2, DISP_EVENT_INSIDE_MUTE, FALSE);
							}

							if(fm_engine->ctrl.rds_txt)
							{
								os_taskq_post_msg(DISP_TASK_NAME, 2, DISP_EVENT_RDS_TXT, 0);
							}
						}
					}

					fm_engine->ctrl.rds_front = front;
				}
				break;

			/*RDS 忙信息保存*/
			case MSG_FMENGINE_RDS_SAVE:
				if(fm_engine->ctrl.rds_enable)
				{
					tuner_drv_func.tuner_setting(TUR_SET_RDS_SAVE, (void *)msg[1]);
				}
				break;

			/*RDS 忙信息加载*/
			case MSG_FMENGINE_RDS_LOAD:
				if(fm_engine->ctrl.rds_enable)
				{
					tuner_drv_func.tuner_info(TUR_INFO_RDS_LOAD, (void *)msg[1]);
				}
				break;

			/*RDS 忙检测*/
			case MSG_FMENGINE_RDS_BUSY:
				fm_puts("MSG_FMENGINE_RDS_BUSY\n");
				fm_engine->ctrl.rds_busy = (void *)msg[1];
				if(fm_engine->ctrl.rds_enable)
				{
					rds_ctrl_busy_func(fm_engine->ctrl.rds_busy);
				}
				break;

			/*RDS 快速检测*/
			case MSG_FMENGINE_RDS_FAST:
				if(fm_engine->ctrl.rds_enable)
				{
					fm_puts("MSG_FMENGINE_RDS_FAST\n");
					rds_ctrl_fast_det();
				}
				break;

			/*RDS TA 使能控制*/
			case MSG_FMENGINE_RDS_TA_DSABLE:
				fm_printf("MSG_FMENGINE_RDS_FAST --- rds_enable:%d, disable:%d\n", fm_engine->ctrl.rds_enable, msg[1]);
				if(fm_engine->ctrl.rds_enable)
				{
					uint8 disable = (uint8)msg[1];
					rds_ctrl_ta_disable(disable);
				}
				break;

			/*RDS 获取信息*/
			case MSG_FMENGINE_RDS_INFO:
				if(fm_engine->ctrl.rds_enable)
				{
					rds_ctrl_ext_info((void *)msg[2]);
				}
				sem = (OS_SEM *)msg[1];//同步命令
				break;

			/*RDS 定时运行*/
			case MSG_FMENGINE_RDS_POLLING:
				if(fm_engine->ctrl.rds_enable)
				{
					fm_engine->ctrl.timer_enter = TRUE;
					((V_FUNC_V)fm_engine->ctrl.rds_func)();
					fm_engine->ctrl.timer_enter = FALSE;
				}
				break;
		#endif

	        default:
	            break;
		}

		if(sem != NULL)
		{//	同步消息
			os_sem_set(sem,0);
			os_sem_post(sem);
		}
    }
}

static void fm_engine_create(void)
{
	if(!fm_engine_run)
	{
		fm_puts("fm_engine_create\n");

		uint8 err;

		if(!fm_sem_flag)
		{
			err = os_sem_create(&fm_eng_sem, 0);
			if(err != OS_NO_ERR)
			{
				fm_puts("fm_engine_sem_err\n");
				return;
			}
			fm_sem_flag = TRUE;
		}

	    err = os_task_create(	fm_engine_task,
			                   (void *)0,
			                   TaskFMEngPrio,
			                   50
							#if OS_TIME_SLICE_EN > 0
			                   ,1
							#endif
			                   ,FM_ENGINE_TASK_NAME);

		if(err == OS_NO_ERR)
		{
			fm_engine_run = TRUE;

			while(!fm_engine_work)
			{
				OSTimeDly(1);
			}
			fm_puts("fm_engine_create, finish\n");
		}
		else
		{
			fm_puts("fm_engine_create, err\n");
		}
	}
}

static void fm_engine_delete(void)
{
	if(fm_engine_run)
	{
		fm_puts("fm_engine_delete\n");

	    if(os_task_del_req(FM_ENGINE_TASK_NAME) != OS_TASK_NOT_EXIST)
	    {
	        os_taskq_post_event(FM_ENGINE_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
	        do
	        {
	            OSTimeDly(1);
	        }
	        while(os_task_del_req(FM_ENGINE_TASK_NAME) != OS_TASK_NOT_EXIST);
	    }

		fm_engine_run = FALSE;
		fm_puts("fm_engine_delete, finish\n");
	}
}

void fm_engine_init(void)
{
	fm_puts("fm_engine_init\n");
	fm_engine_create();
}

void fm_engine_exit(void)
{
	fm_puts("fm_engine_exit\n");

#if defined(_ENABLE_RADIO_RDS_)
	if(fm_engine_work)
	{
		if(!fm_engine->info.rds_support)
		{
			fm_engine_delete();
		}
	}
#else
	fm_engine_delete();
#endif
}

uint8 fm_engine_msg(int type, void *send_addr, void *reply_addr)
{
	if(!fm_engine_work)return FALSE;

	uint8 sem_flag = FALSE;

	if((type == MSG_FMENGINE_INIT)
	 ||(type == MSG_FMENGINE_EXIT))
	{
		reply_addr = &fm_eng_sem;
		sem_flag = TRUE;
	}
	else if(type == MSG_FMENGINE_SET_SOFTSEEK)
	{
		sem_flag = TRUE;
	}
	else if(type == MSG_FMENGINE_AUTO_BREAK)
	{
		send_addr = &fm_eng_sem;
		sem_flag = TRUE;
	}
#if defined(_ENABLE_RADIO_RDS_)
	else if(type == MSG_FMENGINE_RDS_BACK_START)
	{
		reply_addr = &fm_eng_sem;
		sem_flag = TRUE;
	}
	else if(type == MSG_FMENGINE_RDS_INFO)
	{
		send_addr = &fm_eng_sem;
		sem_flag = TRUE;
	}
#endif

	os_taskq_post_event(FM_ENGINE_TASK_NAME, 3, type, send_addr, reply_addr);

	if(sem_flag)
	{
		uint8 err = os_sem_pend(&fm_eng_sem, 0);
		if(err != OS_NO_ERR)
	    {
	    	fm_printf("fm_engine_msg:%d---->err\n", type);
			return FALSE;
	    }
	}

	return TRUE;
}

uint8 fm_inside_check(void)
{
#if defined(_TUNER_DRIVER_AC69xx_)
	if(fm_engine_work)
	{
		if(fm_engine != NULL)
		{
			if(fm_engine->info.type == TUN_DRV_AC69xx)
			{
				return TRUE;
			}
		}
	}
#endif
	return FALSE;
}

#endif

