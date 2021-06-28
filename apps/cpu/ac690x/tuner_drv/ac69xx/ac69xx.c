#include "ac69xx/ac69xx.h"
#include "play_sel/play_sel.h"
#include "dac/src_api.h"
#include "dac/dac_api.h"
#include "fm_inside_api.h"
#include "clock_interface.h"
#include "setup.h"


#if defined(_TUNER_DRIVER_AC69xx_)

#define AC69xx_DEBUG
#if defined(AC69xx_DEBUG)
	#define __DEBUG_AC69xx				ew_printf
#else
	#define __DEBUG_AC69xx(...)
#endif

struct res_freq_analy
{
    s32 rssi;      //
    s32 ifc1;      // zero crossing
    s32 ifc2;      //
    s32 ifcc;      // ifc1 - ifc2
};
struct res_freq_analy  fm_para;
extern void fmrx_freq_analy(struct res_freq_analy *parm, u32 dly);
extern char f_dat13_log(unsigned short dat);

extern char get_curr_freq_rssi(void) ;
 
 //在需要的地方调用下面函数即可.
 //fmrx_freq_analy(&fm_para,0);      //printf("\nfm_msg=  0x%x\n",msg[0]);
 //s8 rssi_db = f_dat13_log(fm_para.rssi);    //rssi转成db数值表示
 //printf("fm_rssi = %d, db = %d\n",fm_para.rssi,rssi_db);
 
 //注意, 要等待FM初始化完后, 才能调用 fmrx_freq_analy,  否则FM内部变量空间没有malloc, 会导致程序挂掉.

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

#define FM_DAC_OUT_SAMPLERATE  			32000L
#define FM_FADEIN_VAL					100

extern void set_fm_dac_out_fun(void *);
extern uint8 fm_set_stereo(uint8);
extern uint8 fm_set_abw(uint8);
extern uint8 fm_get_pilot(void);
extern uint8 get_fm_stereo(void);
extern void fm_set_parm(s8 pwr);

static ac69xx_ctrl_t *ac69xx_ctrl = NULL;	
static const uint16 fm_ac69xx_deal_spot_tab[] = {9600};

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static void fm_inside_dac_out(int16 *buf, uint32 len)
{
	if(ac69xx_ctrl->fadein_cnt)
	{
		ac69xx_ctrl->fadein_cnt--;
		return;
	}
	
	if(play_sel_busy())return;
	
    if(ac69xx_ctrl->audio_mute)
    {
		memset(buf, 0x00, len);
    }
	
	cbuf_write_dac(buf, len);
}

static void fm_inside_data_out(void * buf, uint32 len)
{
    src_write_data_api(buf,len);
}

static void fm_inside_src_ctl(uint8 flag)
{
    if(flag == 1)
    {
    	src_param_t src_p;
		
        src_p.in_chinc = 1;
        src_p.in_spinc = 2;
        src_p.out_chinc = 1;
        src_p.out_spinc = 2;
        src_p.in_rate = 41667;
        src_p.out_rate = FM_DAC_OUT_SAMPLERATE;
        src_p.nchannel = 2;
        src_p.output_cbk = (void *)fm_inside_dac_out;
        src_init_api(&src_p);
    }
    else
    {
        src_exit_api();
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 ac69xx_set_mute(uint8 mute)
{
	ac69xx_ctrl->audio_mute = mute;
	return TRUE;
}

static uint8 ac69xx_set_stereo(uint8 stereo)
{
	if(stereo)
	{
		fm_set_stereo(127);
	}
	else
	{
		fm_set_stereo(0);
	}
	return TRUE;
}

#if 0
void ac69xx_auto_set(void)
{
    u8 dat1, dat2;

	fmrx_freq_analy(&fm_para,0);
 	s8 rssi_db = f_dat13_log(fm_para.rssi);
 	
	dat1 = rssi_db + AC69xx_TUNER_RSSI_FM_RSSI;
    dat2 = fm_get_pilot();
	
	printf("dat1:%d\n",dat1);
	
    fm_set_abw(dat1);

	printf("dat1---:%d\n",dat1);
	printf("dat2:%d\n",dat2);
    if(dat2 == 127)
    {
       ac69xx_set_stereo(dat1);
    }
    else
    {
       ac69xx_set_stereo(0);
    }
}
#endif

static uint8 ac69xx_set_freq(uint16 fre)
{
	uint32 freq;
	freq = fre * 10;
	
	return set_fm_inside_freq(freq);
}

static void ac69xx_set_enable(uint8 flag)
{
	if(flag)
	{
		set_fm_dac_out_fun(fm_inside_data_out);
		
	    fm_inside_src_ctl(1);
		
	    dac_set_samplerate(FM_DAC_OUT_SAMPLERATE,1);
		
		fm_inside_module_open();
		
		//fm_set_parm(-20);
		
		//FM搜台参数1设置：分别对应：	//nrs	//nc1	//nc2	//ncc
	#if defined(_ENABLE_AC69xx_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	28, 	75, 	65, 	60);
	#elif defined(_HYX1039_AC69xx_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	2,		40, 	35, 	30);
	#elif defined(_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	28,		25, 	25, 	25);
	#elif defined(_WLS8010B_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	14, 	25, 	25, 	25);
	#elif defined(_SH1308F_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	 7, 	25, 	25, 	25);
	#elif defined(_NXO6301_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	20, 	25, 	25, 	25);
	#elif defined(_WLS6215_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	15, 	25, 	25, 	25);
	#elif defined(_HYM6222_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	15, 	25, 	25, 	25);
	#elif defined(_RX5235_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	15, 	25, 	25, 	25);
	#elif defined(_ENABLE_AC69xx_HY1037_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	36,		25, 	25, 	25);
	#else
	#if defined(_WLS8010_ENABLE_AC69xx_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	4,		55, 	45, 	40);
	#else
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	4,		20, 	20, 	20);
	#endif
	#endif
		
		//FM搜台参数2设置：分别对应：	//noise_scan	//noise_pga	//cnr	//(reserve)	//agc_max	//(reserve)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG2, 	0, 				8, 			2,		10, 			9, 			0);
		//fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG2,	0,				6,			2,		10, 			9,			0);
		
		//系统时钟获取，用于内部FM模块
		fm_inside_io_ctrl(SET_FM_INSIDE_OSC_CLK, OSC_Hz/1000000);
		
		//FM内部晶振选择，0：BT_OSC	,1：RTC_OSC
		fm_inside_io_ctrl(SET_FM_INSIDE_OSC_SEL, 0);
		
		//FM搜台信息打印
		//fm_inside_io_ctrl(SET_FM_INSIDE_PRINTF, 1);
		
	    fm_inside_on(0);
		
		fm_set_stereo(127);
		/* fm_set_abw(48); */
        fm_set_mod(1);
	}
	else
	{
		fm_inside_off();
		
    	fm_inside_src_ctl(0);
	}
}

static uint8 ac69xx_get_id(void)
{
	return read_fm_inside_id();
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_ac69xx_detect(void);
static uint8 tuner_ac69xx_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_ac69xx_exit(void);
static uint8 tuner_ac69xx_freq(uint16);
static uint8 tuner_ac69xx_status(engine_status_t *);
static uint8 tuner_ac69xx_search(uint16, engine_status_t *);
static uint8 tuner_ac69xx_setting(fm_setting_e, void *);
static uint8 tuner_ac69xx_info(fm_info_e, void *);


static uint8 tuner_ac69xx_detect(void)
{
	__DEBUG_AC69xx("%s, r:%d\n", __FUNCTION__, 1);
	
	return TRUE;
}

static uint8 tuner_ac69xx_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	__DEBUG_AC69xx("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);
	
	if(ac69xx_ctrl == NULL)
	{
		ac69xx_ctrl = (ac69xx_ctrl_t *)malloc_fun(ac69xx_ctrl, sizeof(ac69xx_ctrl_t), 0);
		if(ac69xx_ctrl == NULL)return FALSE;
	}
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto ac69xx_init_error;
			if(min_freq < 7600)goto ac69xx_init_error;
			if(max_freq > 10800)goto ac69xx_init_error;
			ac69xx_ctrl->tuner_band = AC69xx_TUNER_BAND_FM;
			break;
			
		default:
			goto ac69xx_init_error;
			break;
	}

	__DEBUG_AC69xx("band111:%d, band:%d,init_succeed:%d\n", ac69xx_ctrl->band, band,ac69xx_ctrl->init_succeed);

	if(ac69xx_ctrl->band == band)
	{
		if(ac69xx_ctrl->init_succeed)
		{
			if(ac69xx_ctrl->now_freq == freq)
			{
				ac69xx_ctrl->min_freq = min_freq;
				ac69xx_ctrl->max_freq = max_freq;
				if(!tuner_ac69xx_setting(TUR_SET_MUTE, OFF))goto ac69xx_init_error;
				return TRUE;
			}
			else
			{
				goto ac69xx_init_succeed;
			}
		}
		else 
		{
			goto ac69xx_init_error;
		}
	}
	
    ac69xx_set_enable(ENABLE);
	
ac69xx_init_succeed:
	
	ac69xx_ctrl->band 			= band;
	ac69xx_ctrl->min_freq 		= min_freq;
	ac69xx_ctrl->max_freq 		= max_freq;
	ac69xx_ctrl->init_succeed 	= TRUE;
	ac69xx_ctrl->audio_mute		= TRUE;
	ac69xx_ctrl->fadein_cnt 	= FM_FADEIN_VAL;
	
	engine_status_t status;
	if(!tuner_ac69xx_search(freq, &status))goto ac69xx_init_error;
	if(!tuner_ac69xx_setting(TUR_SET_MUTE, OFF))goto ac69xx_init_error;

	__DEBUG_AC69xx("AC69xx Init Succeed!!!!!\n");
	return TRUE;
	
ac69xx_init_error:
	
	free_fun((void **)&ac69xx_ctrl);
	
	__DEBUG_AC69xx("AC69xx Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_ac69xx_exit(void)
{
	__DEBUG_AC69xx("%s\n", __FUNCTION__);

	if(ac69xx_ctrl)
	{
		if(ac69xx_ctrl->init_succeed)
		{
			ac69xx_set_enable(DISABLE);
		}
		
		free_fun((void **)&ac69xx_ctrl);
	}
	
	return TRUE;
}

static uint8 tuner_ac69xx_freq(uint16 freq)
{
	__DEBUG_AC69xx("%s, freq:%d\n", __FUNCTION__, freq);
	if(ac69xx_ctrl == NULL)return FALSE;
	if(!ac69xx_ctrl->init_succeed)return FALSE;
	if(freq < ac69xx_ctrl->min_freq)return FALSE;
	if(freq > ac69xx_ctrl->max_freq)return FALSE;	
	
#if !defined(_WLS8010_ENABLE_AC69xx_RSSI_)
#if !defined(_ENABLE_AC69xx_RSSI_)
#if !defined(_HYX1039_AC69xx_RSSI_)
	if(setup.loc_flg)
	{
	#if defined(_ENABLE_AC69xx_AGC_RSSI_)||defined(_WLS6215_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	300,	25, 	25, 	25);
	#elif defined(_RX5235_ENABLE_AC69xx_AGC_RSSI_)
	    fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	98, 	25, 	25, 	25);//98是37DB
	#elif defined(_WLS8010B_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	250, 	25, 	25, 	25);
	#elif defined(_SH1308F_ENABLE_AC69xx_AGC_RSSI_)
	    fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	150, 	25, 	25, 	25);
	#elif defined(_FAS3529_ENABLE_AC69xx_AGC_RSSI_)
	    fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	50, 	25, 	25, 	25);
	#elif defined(_NXO6301_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	250,	25, 	25, 	25);
	#elif defined(_HYM6222_ENABLE_AC69xx_LOC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	50,	    25, 	25, 	25);
	#elif defined(_HYM6222A_ENABLE_AC69xx_LOC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	98,	    25, 	25, 	25);
	#elif defined(_ENABLE_AC69xx_HY1037_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	250,	25, 	25, 	25);
	#else
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	35,		20, 	20, 	20);//98是37DB
	#endif
	}
	else
	{
	#if defined(_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	28, 	25, 	25, 	25);
	#elif defined(_WLS8010B_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	14, 	25, 	25, 	25);
	#elif defined(_SH1308F_ENABLE_AC69xx_AGC_RSSI_)
	    fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	 7, 	25, 	25, 	25);
	#elif defined(_RX5235_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	15, 	25, 	25, 	25);
	#elif defined(_WLS6215_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	15, 	25, 	25, 	25);
	#elif defined(_HYM6222_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	15, 	25, 	25, 	25);
	#elif defined(_NXO6301_ENABLE_AC69xx_AGC_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	20, 	25, 	25, 	25);
	#elif defined(_ENABLE_AC69xx_HY1037_RSSI_)
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	36, 	25, 	25, 	25);
	#else
		fm_inside_io_ctrl(SET_FM_INSIDE_SCAN_ARG1,	4,		20, 	20, 	20);//98是17DB
	#endif
	}
#endif
#endif
#endif

    if(ac69xx_set_freq(freq))
	{
		ac69xx_ctrl->valid = TRUE;
	}
	else
	{
		ac69xx_ctrl->valid = FALSE;
	}
	
	//printf("ac69xx_ctrl->rssi:%d\n",ac69xx_ctrl->rssi);
	
	ac69xx_ctrl->now_freq = freq;
	return TRUE;
}

static uint8 tuner_ac69xx_status(engine_status_t *status)
{	
	if(ac69xx_ctrl == NULL)return FALSE;
	if(!ac69xx_ctrl->init_succeed)return FALSE;
	
	status->stereo  = FALSE;
	status->freq 	= ac69xx_ctrl->now_freq;
	status->valid	= ac69xx_ctrl->valid;

#if defined(_ENABLE_AC69xx_RSSI_)||defined(_HYX1039_AC69xx_RSSI_)||defined(_WLS8010_ENABLE_AC69xx_RSSI_)
	if(setup.loc_flg)
	{
		if(status->valid)
		{
			fmrx_freq_analy(&fm_para,0);		//printf("\nfm_msg=  0x%x\n",msg[0]);
			s8 rssi_db = f_dat13_log(fm_para.rssi);  //rssi转成db数值表示
			
			status->signal	= rssi_db + AC69xx_TUNER_RSSI_FM_RSSI;
		}
	}
	else
#endif
	{
		//status->signal	= get_curr_freq_rssi();
		status->signal	= AC69xx_TUNER_RSSI_FM_RSSI;
	}
 		
	//printf("fm_para.rssi = %d\n, rssi_db = %d\n",fm_para.rssi,rssi_db);

#if defined(_ENABLE_RADIO_STO_)
	if(global.sto_flag)
	{
		fmrx_freq_analy(&fm_para,0);		//printf("\nfm_msg=  0x%x\n",msg[0]);
		s8 rssi_db = f_dat13_log(fm_para.rssi);  //rssi转成db数值表示
		
		status->signal	= rssi_db + AC69xx_TUNER_RSSI_FM_RSSI;
		
		if(status->signal > 35)
		{
			status->stereo = TRUE;
		}
	}
#else
	status->stereo	= get_fm_stereo();
#endif

	//printf("status->signal:%d\n",status->signal);
	//printf("status->stereo:%d\n",get_fm_stereo());
	//printf("status->valid:%d\n",status->valid);
	
	status->band 	= RADIO_BAND_FM;
	status->stc		= TRUE;
		
	return TRUE;
}

static uint8 tuner_ac69xx_search(uint16 freq, engine_status_t *status)
{
	__DEBUG_AC69xx("%s, freq:%d\n", __FUNCTION__, freq);
	if(ac69xx_ctrl == NULL)return FALSE;
	if(!ac69xx_ctrl->init_succeed)return FALSE;
	if(freq < ac69xx_ctrl->min_freq)return FALSE;
	if(freq > ac69xx_ctrl->max_freq)return FALSE;	
	
	if(!tuner_ac69xx_freq(freq))return FALSE;
	
	if(!tuner_ac69xx_status(status))return FALSE;
	
	__DEBUG_AC69xx("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_ac69xx_setting(fm_setting_e type, void *set)
{
	__DEBUG_AC69xx("%s, type:%d\n", __FUNCTION__, type);
	
	if(ac69xx_ctrl == NULL)return FALSE;
	if(!ac69xx_ctrl->init_succeed)return FALSE;

	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!ac69xx_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(!ac69xx_set_stereo(stereo))return FALSE;
			}
			break;
			
		default:
			return FALSE;
	}
	
	__DEBUG_AC69xx("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}
static uint8 tuner_ac69xx_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_AC69xx;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = AC69xx_TUNER_RSSI_FM;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = AC69xx_TUNER_RSSI_LOC_FM;
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_ac69xx_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_ac69xx_deal_spot_tab;
			}
			break;
			
		case TUR_INFO_AUTOSEEK:
			if(info != NULL)
			{
				*(uint8 *)info = FALSE;
			}
			break;
			
		case TUR_INFO_FM_ONLY:
			if(info != NULL)
			{
				*(uint8 *)info = TRUE;
			}
			break;
			
		default:
			return FALSE;
	}
	return TRUE;
}

const tuner_drv_fun_t tuner_ac69xx_drv_fun =
{
	tuner_ac69xx_detect,
	tuner_ac69xx_init,
	tuner_ac69xx_exit,
	tuner_ac69xx_freq,
	tuner_ac69xx_status,
	tuner_ac69xx_search,
	NULL,
	tuner_ac69xx_setting,
	tuner_ac69xx_info,
};

#endif

