#include "sdk_cfg.h"
#include "ui_res.h"
#include "ui_api.h"
#include "fm_radio.h"
#include "fm_api.h"
#include "fm_engine.h"
#include "fm_band.h"
#include "rds/rds_ctrl.h"
#include "dac.h"
#include "vm/vm_api.h"
#include "ladc.h"
#include "audio_drv.h"
#include "setup.h"
#include "disp.h"
#include "rtc.h"
#include "ui_api.h"
#include "play_sel/play_sel.h"
#if defined(_RADIO_FM_GATE_FYDVD_)||defined(_SI474X_REDUCE_AUD_GATE_)||defined(_ENABLE_RADIO_OUTPUT_)
#include "tuner_drv.h"
#endif
#if defined(_SUPPORT_EXT_MEMORY_)
#include "ext_memory/ext_memory.h"
#endif

#include "rds/rds_ctrl.h"
#include "rds/rds_com.h"

#ifdef TD_ADD_SPP
extern void Spp_Tx_Radio_Info(uint16 freq);
#endif
#ifdef TD_ADD_NEW_APP
extern void new_app_radio_init(void);
extern void new_app_send_bandinfo(void);
extern void new_app_radio_bandinfo(u8 band,u8 channel,u16 freq);
#endif
#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_Radio_Info(uint16 freq);
extern void TFT_Tx_Radio_Status(void);
#endif
#if defined(_SUPPORT_RADIO_)

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);
static void cmd_radio_area_set(setup_area_e);
#if defined(_ENABLE_RADIO_RDS_)
static uint8 radio_engine_reply(uint8, uint16, uint16, uint8);
#else
static uint8 radio_engine_reply(uint8, uint16);
#endif
extern void radio_disp_polling(void);

#if defined(_ENABLE_RDS_ON_OFF_)
extern void cmd_radio_rds_onoff(uint8 onoff);
#endif

#if defined(TD_ADD_NEW_APP)
#if defined(_ENABLE_RADIO_RDS_)
typedef struct{
	uint8  rds_af;
	uint8  rds_ta;
	uint8  rds_pty;//pty节目类型
	uint8  rds_pty_type;//pty类型
}rds_infomation;

rds_infomation rds;
#endif
#endif

/*-----------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------*/
#define VM_FM_TITLE				0x64123913
typedef struct{
	uint32			title;
	radio_config_t 	fm;
	uint32			check;
}wm_fm_t;

static uint32 fm_vm_sum(wm_fm_t *vm_fm)
{
	uint32 back_sum = vm_fm->check;
	vm_fm->check = 0;

	uint32 sum = 0;
	int i;

	for(i = 0; i < sizeof(wm_fm_t); i++)
	{
		sum += ((uint8 *)vm_fm)[i];
	}

	vm_fm->check = back_sum;
	return (~sum)+1;
}

static uint8 fm_save_info(const radio_config_t *addr)
{
	wm_fm_t *wm_fm = (wm_fm_t *)malloc(VM_FM_INFO_LEN);
	if(wm_fm == NULL)
	{
		fm_puts("fm_save_info_error_malloc\n");
        return FALSE;
	}

	wm_fm->title = VM_FM_TITLE;
	wm_fm->fm = *addr;
	wm_fm->check = fm_vm_sum(wm_fm);

	int length = vm_write_api(VM_FM_INFO, wm_fm);

	free(wm_fm);

	fm_printf("fm_save_info_length:%d \r\n", length);
	if(length == VM_FM_INFO_LEN)return TRUE;

	return FALSE;
}

static uint8 fm_read_info(radio_config_t *addr)
{
	fm_puts("fm_read_info\n");

	wm_fm_t *wm_fm = (wm_fm_t *)malloc(VM_FM_INFO_LEN);
	if(wm_fm == NULL)
	{
		fm_puts("fm_read_info_error_malloc\n");
        return FALSE;
	}

	if(vm_read_api(VM_FM_INFO, wm_fm) != VM_FM_INFO_LEN)
	{
		free(wm_fm);
		fm_puts("fm_check_error_read\n");
		return FALSE;
	}

	if(wm_fm->title != VM_FM_TITLE)
	{
		free(wm_fm);
		fm_puts("fm_check_error_title\n");
		return FALSE;
	}

	if(fm_vm_sum(wm_fm) != wm_fm->check)
	{
		free(wm_fm);
		fm_puts("fm_check_error_sum\n");
		return FALSE;
	}

	*addr = wm_fm->fm;
	free(wm_fm);
	
#if defined(_SUPPORT_EXT_MEMORY_)
	ext_memory_msg(MSG_EXT_MEMORY_RADIO, radio_config, NULL);
#endif

	fm_puts("fm_read_info_succeed\n");
	return TRUE;
}

#if defined(_ENABLE_RADIO_RDS_)

#if !defined(_DISABLE_RDS_MENMORY_)
#define VM_RDS_TITLE				0x24638126

typedef struct{
	uint32			title;
	radio_vm_rds_t 	rds;
	uint32			check;
}wm_rds_t;

static uint32 rds_vm_sum(wm_rds_t *vm_rds)
{
	uint32 back_sum = vm_rds->check;
	vm_rds->check = 0;

	uint32 sum = 0;
	int i;

	for(i = 0; i < sizeof(wm_rds_t); i++)
	{
		sum += ((uint8 *)vm_rds)[i];
	}

	vm_rds->check = back_sum;
	return (~sum)+1;
}

static uint8 fm_save_rds(const radio_vm_rds_t *addr)
{
	wm_rds_t *wm_rds = (wm_rds_t *)malloc(VM_FM_RDS_LEN * VM_FM_RDS_BLOCK);
	if(wm_rds == NULL)
	{
		fm_puts("fm_save_rds_error_malloc\n");
        return FALSE;
	}

	wm_rds->title = VM_RDS_TITLE;
	wm_rds->rds = *addr;
	wm_rds->check = rds_vm_sum(wm_rds);

	int length_1 = vm_write_api(VM_FM_RDS1, &(((uint8 *)wm_rds)[VM_FM_RDS_LEN * 0]));
	int length_2 = vm_write_api(VM_FM_RDS2, &(((uint8 *)wm_rds)[VM_FM_RDS_LEN * 1]));
	int length_3 = vm_write_api(VM_FM_RDS3, &(((uint8 *)wm_rds)[VM_FM_RDS_LEN * 2]));
	int length_4 = vm_write_api(VM_FM_RDS4, &(((uint8 *)wm_rds)[VM_FM_RDS_LEN * 3]));

	free(wm_rds);

	fm_printf("fm_save_info--->length_1:%d,length_2:%d,length_3:%d,length_4:%d\n", length_1, length_2, length_3, length_4);
	if((length_1 == VM_FM_RDS_LEN) && (length_2 == VM_FM_RDS_LEN) && (length_3 == VM_FM_RDS_LEN) && (length_4 == VM_FM_RDS_LEN))return TRUE;

	return FALSE;
}

static uint8 fm_read_rds(radio_vm_rds_t *addr)
{
	wm_rds_t *wm_rds = (wm_rds_t *)malloc(VM_FM_RDS_LEN * VM_FM_RDS_BLOCK);
	if(wm_rds == NULL)
	{
		fm_puts("fm_read_rds_error_malloc\n");
        return FALSE;
	}

	if(vm_read_api(VM_FM_RDS1, &(((uint8 *)wm_rds)[VM_FM_RDS_LEN * 0])) != VM_FM_RDS_LEN)
	{
		free(wm_rds);
		fm_puts("rds1_check_error_read\n");
		return FALSE;
	}
	if(vm_read_api(VM_FM_RDS2, &(((uint8 *)wm_rds)[VM_FM_RDS_LEN * 1])) != VM_FM_RDS_LEN)
	{
		free(wm_rds);
		fm_puts("rds2_check_error_read\n");
		return FALSE;
	}
	if(vm_read_api(VM_FM_RDS3, &(((uint8 *)wm_rds)[VM_FM_RDS_LEN * 2])) != VM_FM_RDS_LEN)
	{
		free(wm_rds);
		fm_puts("rds3_check_error_read\n");
		return FALSE;
	}
	if(vm_read_api(VM_FM_RDS4, &(((uint8 *)wm_rds)[VM_FM_RDS_LEN * 3])) != VM_FM_RDS_LEN)
	{
		free(wm_rds);
		fm_puts("rds4_check_error_read\n");
		return FALSE;
	}

	if(wm_rds->title != VM_RDS_TITLE)
	{
		free(wm_rds);
		fm_puts("rds_check_error_title\n");
		return FALSE;
	}

	if(rds_vm_sum(wm_rds) != wm_rds->check)
	{
		free(wm_rds);
		fm_puts("rds_check_error_sum\n");
		return FALSE;
	}

	*addr = wm_rds->rds;
	free(wm_rds);

	fm_puts("fm_read_rds_succeed\n");
	return TRUE;
}

#endif
#endif

#if defined(_ENABLE_RDS_CTRL_)
radio_rds_t radio_rds_val;
#endif

static void fm_malloc(void)
{
	radio_ctrl = (radio_ctrl_t *)malloc_fun(radio_ctrl ,sizeof(radio_ctrl_t), 0);
	if(radio_ctrl == NULL)
	{
		fm_puts("radio_ctrl err\n");
        while(1);
	}
	radio_config = (radio_config_t *)malloc_fun(radio_config ,sizeof(radio_config_t), 0);
	if(radio_config == NULL)
	{
		fm_puts("radio_config err\n");
        while(1);
	}
#if defined(_ENABLE_RADIO_RDS_)
#if defined(_ENABLE_RDS_CTRL_)
	radio_rds = &radio_rds_val;
#else
	radio_rds = (radio_rds_t *)malloc_fun(radio_rds ,sizeof(radio_rds_t), 0);
#endif
	if(radio_rds == NULL)
	{
		fm_puts("radio_rds err\n");
        while(1);
	}
#endif
}

static void fm_free(void)
{
	fm_puts("fm_free\n");

	free_fun((void**)&radio_ctrl);
	free_fun((void**)&radio_config);
#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_ENABLE_RDS_CTRL_)
	free_fun((void**)&radio_rds);
#endif
#endif
}

static void fm_data_init(void)
{
	memset(radio_config, 0, sizeof(radio_config_t));
#if defined(_SUPPORT_AREA_)
	radio_config->radio_info.area = setup.area;
#endif

	RadioInitBand();
	RadioInitFreq();
	SET_RADIO_LOC(FALSE);
#if defined(_ENABLE_DEFAULT_MONO_)
	SET_RADIO_ST(FALSE);
#else
	SET_RADIO_ST(TRUE);
#endif
#if defined(_ENABLE_ST_MONO_)
	setup.st_flag = TRUE;
#endif

#if defined(SUPPORT_AM_LOC)
	SET_RADIO_AMLOC(FALSE);
#endif
}

static void fm_load_cfg(void)
{
	fm_puts("fm_load_cfg\n");

    if(vm_check_reset(VM_RESET_FM) || (!fm_read_info(radio_config)))
    {
    	fm_puts("fm_date_init\n");
	#if defined(_SUPPORT_EXT_MEMORY_)
		static uint8 check_flag;
		if(ext_memory_msg(MSG_EXT_MEMORY_RADIO, radio_config, &check_flag))
		{
			fm_printf("%s, check_flag:%d\n", __FUNCTION__, check_flag);
			if(!check_flag)
			{
				fm_data_init();
			}
		}
		else
	#endif
		{
			fm_data_init();
		}
		fm_save_info(radio_config);
    }
#ifdef TD_ADD_NEW_APP
	new_app_radio_init();
	OSTimeDly(15);
	new_app_send_bandinfo();
	OSTimeDly(15);
#endif

#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_DISABLE_RDS_MENMORY_)
	if(vm_check_reset(VM_RESET_FM) || (!fm_read_rds(&radio_rds->rds_vm)))
	{
		memset(&radio_rds->rds_vm, 0, sizeof(radio_vm_rds_t));
		fm_save_rds(&radio_rds->rds_vm);
	}
#endif

	uint8 band;
	for(band = 0;band < GET_RADIO_BAND_LENGTH; band++)
	{
		if(INDEX_RADIO_STATE_FM(band))
		{
			radio_rds->first_band = band;
			break;
		}
	}
#endif

	vm_clean_reset(VM_RESET_FM);
}

static void radio_save_cfg(void)
{
	fm_puts("radio_save_cfg\n");

	radio_config_t *wm_fm = NULL;
	wm_fm = (radio_config_t *)malloc_fun(wm_fm ,sizeof(radio_config_t), 0);
    if(fm_read_info(wm_fm))
    {
		if(memcmp(radio_config, wm_fm, sizeof(radio_config_t)))
		{
			fm_puts("fm_date_save---1---\n");
			fm_save_info(radio_config);
		}
	}
	else
	{
		fm_puts("fm_date_save---2---\n");
		fm_save_info(radio_config);
	}
	free_fun((void**)&wm_fm);

#if defined(_SUPPORT_EXT_MEMORY_)
	ext_memory_msg(MSG_EXT_MEMORY_RADIO, NULL, NULL);
#endif

#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_DISABLE_RDS_MENMORY_)
	radio_vm_rds_t *wm_rds = NULL;
	wm_rds = (radio_vm_rds_t *)malloc_fun(wm_rds ,sizeof(radio_vm_rds_t), 0);
    if(fm_read_rds(wm_rds))
    {
		if(memcmp(&radio_rds->rds_vm, wm_rds, sizeof(radio_vm_rds_t)))
		{
			fm_puts("fm_rds_save---1---\n");
			fm_save_rds(&radio_rds->rds_vm);
		}
	}
	else
	{
		fm_puts("fm_rds_save---2---\n");
		fm_save_rds(&radio_rds->rds_vm);
	}
	free_fun((void**)&wm_rds);
#endif
#endif
}

static uint8 radio_band_loc_state(void)
{
	if(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE)
	{
		if(GET_RADIO_AUTOLOC)return TRUE;
	}
	else
	{
		if(RADIO_STATE_FM)
		{
			if(GET_RADIO_LOC)return TRUE;
		}
    #if defined(SUPPORT_AM_LOC)
    	else if(RADIO_STATE_AM)
    	{
    		if(GET_RADIO_AMLOC)return TRUE;
    	}
    #endif
	}
	return FALSE;
}

static uint8 radio_band_st_state(void)
{
	if(RADIO_STATE_FM)
	{
		if(GET_RADIO_ST)return TRUE;
	}
	return FALSE;
}

static uint8 radio_band_sto_state(void)
{
	if(RADIO_STATE_FM)
	{
		if(GET_RADIO_STO)return TRUE;	
	#if defined(_USER_TFT_SEND_)
		 TFT_Tx_Radio_Status();
	#endif
	}
	return FALSE;
}

static void *radio_com_set_check(func_sel_e func_sel)
{
	switch(func_sel)
	{
		case FUNC_SEL_LOC:
			if(GET_RADIO_STATE == RADIO_STATE_IDLE)
			{
				if(RADIO_STATE_FM)
				{
					return (&GET_RADIO_LOC);
				}
		    #if defined(SUPPORT_AM_LOC)
		    	else if(RADIO_STATE_AM)
		    	{
		    		return (&GET_RADIO_AMLOC);
		    	}
		    #endif
			}
			break;

		case FUNC_SEL_ST:
			if(GET_RADIO_STATE == RADIO_STATE_IDLE)
			{
				if(RADIO_STATE_FM)
				{
					return (&GET_RADIO_ST);
				}
			}
			break;

	#if defined(_SUPPORT_AREA_)
		case FUNC_SEL_AREA:
			{
				return cmd_radio_area_set;
			}
			break;
	#endif
	
		default:
			return NULL;
	}

	return NULL;
}

static void radio_disp_info(void)
{
	uint8 flash_flag = FALSE;
	if(radio_ctrl->flash_counter)
	{
		if(radio_ctrl->flash_state)
		{
		#if defined(_ENABLE_SAVE_FREQ_)
			if(radio_ctrl->flash_paint & (RADIO_PAINT_FREQ|RADIO_PAINT_CHANEL|RADIO_PAINT_CH))
		#else
			if(radio_ctrl->flash_paint & (RADIO_PAINT_FREQ|RADIO_PAINT_CHANEL))
		#endif
			{
				flash_flag = TRUE;
			}
		}
	}

	radio_ctrl->radio_info.state = radio_ctrl->disp_state;

#if defined(_ENABLE_SAVE_FREQ_)
	if(radio_ctrl->save_flag)
	{
		radio_ctrl->radio_info.freq = radio_ctrl->disp_freq;
	}
	else
	{
		radio_ctrl->radio_info.freq = flash_flag ? 0xFFFF : radio_ctrl->disp_freq;
	}
#else
	radio_ctrl->radio_info.freq = flash_flag ? 0xFFFF : radio_ctrl->disp_freq;
#endif

	radio_ctrl->radio_info.band = GetBandNo(radio_ctrl->disp_band);

#if defined(_ENABLE_SAVE_FREQ_)
	if(radio_ctrl->save_flag)
	{
		radio_ctrl->radio_info.num	= flash_flag ? 0 : radio_ctrl->saveIndex;
	}
	else
	{
		radio_ctrl->radio_info.num	= flash_flag ? 0 : radio_ctrl->disp_index;
	}
#else
	radio_ctrl->radio_info.num	= flash_flag ? 0 : radio_ctrl->disp_index;
#endif

	radio_ctrl->radio_info.flag = radio_ctrl->disp_flag;
#if defined(_ENABLE_RADIO_RDS_)
	radio_ctrl->radio_info.psn	= radio_rds->rds_psn;
#endif

	disp_send_param(DISP_EVENT_FREQ, &radio_ctrl->radio_info);
}


static void radio_flash_disable(void)
{
	uint8 flash_flag = FALSE;

	if(radio_ctrl->flash_counter)
	{
		if(radio_ctrl->flash_state)
		{
		#if defined(_ENABLE_SAVE_FREQ_)
			if(radio_ctrl->flash_paint & (RADIO_PAINT_FREQ|RADIO_PAINT_CHANEL|RADIO_PAINT_CH))
		#else
	    	if(radio_ctrl->flash_paint & (RADIO_PAINT_FREQ|RADIO_PAINT_CHANEL))
		#endif
			{
				flash_flag = TRUE;
			}
		}
	}

	if(flash_flag)
	{
		radio_ctrl->flash_counter 	= 0;
		radio_ctrl->flash_state 	= 0;
		radio_ctrl->flash_paint 	= 0;
		radio_disp_info();
	}
}

static void radio_flash_enable(uint8 counter, uint8 state, uint16 paint)
{
	radio_flash_disable();
	radio_ctrl->flash_counter 	= counter;
	radio_ctrl->flash_state 	= state;
	radio_ctrl->flash_paint 	= paint;
}

#if defined(_ENABLE_RADIO_RDS_)
static uint8 radio_rds_busy(void)
{
	if(GET_RADIO_STATE == RADIO_STATE_IDLE)
	{
		return FALSE;
	}
	return TRUE;
}

#if !defined(_DISABLE_RDS_MENMORY_)
static void load_broadcast_rds_info(void)
{
	if(radio_rds_check())
	{
		if((radio_rds->enter_flag) && (radio_ctrl->state == RADIO_STATE_IDLE) && (GET_BAND_FREQ_INDEX <= GET_RADIO_FREQ_LENGTH))
		{
			radio_rds->enter_flag = FALSE;
			radio_rds_station_t *rds_station = &(radio_rds->rds_vm.rds_station[GET_RADIO_BAND - radio_rds->first_band][GET_BAND_FREQ_INDEX]);

			if(rds_station->psn_succeed)
			{
				memcpy(radio_rds->rds_psn, rds_station->psn_buffer, 8);
			}
			else
			{
				memset(radio_rds->rds_psn, 0, 8);
			}
			radio_engine_rds_save(rds_station);
		}
	}
}

static void save_broadcast_rds_info(void)
{
	if(radio_rds_check())
	{
		if((!radio_rds->enter_flag) && (radio_ctrl->state == RADIO_STATE_IDLE) && (GET_BAND_FREQ_INDEX <= GET_RADIO_FREQ_LENGTH))
		{
			radio_rds->enter_flag = TRUE;
			radio_rds_station_t *rds_station = &(radio_rds->rds_vm.rds_station[GET_RADIO_BAND - radio_rds->first_band][GET_BAND_FREQ_INDEX]);

			radio_engine_rds_load(rds_station);
		}
	}
}

static void clean_broadcast_rds_info(void)
{
	if(radio_rds_check())
	{
		radio_rds_station_t *rds_station = &(radio_rds->rds_vm.rds_station[GET_RADIO_BAND - radio_rds->first_band][GET_BAND_FREQ_INDEX]);
		memset(rds_station, 0, sizeof(radio_rds_station_t));
		radio_engine_rds_load(rds_station);
	}
}
#endif
#endif

void fm_init(void)
{
	fm_puts("fm_init\n");

	fm_malloc();

	fm_load_cfg();

    fm_engine_init();

	radio_engine_get_fm_only();

#if defined(TD_ADD_BANDCH)
#if !defined(HuiNuo_6651D_Copy)//惠诺抄机切换band不需要显示
	radio_ctrl->disp_bandch_flag=TRUE;
#endif
#endif
	radio_engine_init();//发送初始化命令

	radio_engine_get_status();
	
#if defined(_SPECIAL_RDS_DELAY_)
	OSTimeDly(5);
#endif

#if defined(_ENABLE_RADIO_RDS_)
	radio_engine_rds_state();
#if defined(_SPECIAL_RDS_DELAY_)
	OSTimeDly(15);
#endif
	radio_engine_rds_front(TRUE);
	radio_engine_rds_busy(radio_rds_busy);
#if !defined(_DISABLE_RDS_MENMORY_)
	radio_rds->enter_flag = TRUE;
	load_broadcast_rds_info();
#endif
#endif

	if(!GET_RADIO_ST)
	{
		radio_ctrl->st_last = ENABLE;
	}

	radio_engine_return(radio_engine_reply);
	disp_send_param(DISP_EVENT_CALLBACK, radio_com_set_check);

#if defined(_ENABLE_RDS_ON_OFF_)
	if(global.rds_flag)
	{
		cmd_radio_rds_onoff(setup.rds_onoff);
		global.rds_flag = FALSE;
	}
#endif

#if defined(_ENABLE_READ_ST_TIME_)
	global.st_time = 0;
#endif

#if defined(_ENABLE_SPECIAL_MUTE_)
	sys_mute_ctrl(DISABLE, 1);
#elif defined(_ENABLE_REJECT_MUTE_)
	sys_mute_ctrl(ENABLE, 1500);//1500
#else
	sys_mute_ctrl(ENABLE, 500);
#endif
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RADIO_ON, TRUE);
#if defined(_RADIO_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#elif defined(_RADIO_FM_GATE_FYDVD_)
	if(global.tuner_drive == TUN_DRV_QN8035)
	{
		audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
	}
	else
	{
		audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
	}
/*#elif defined(_SI474X_REDUCE_AUD_GATE_)
	if(global.tuner_drive == TUN_DRV_SI473x)
	{
		audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
	}
	else
	{
		audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
	}*/
#elif defined(_FY5517_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#elif defined(_HKY1032_REDUCE_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#else
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
#endif

#if defined(_DECODER_MUSIC_GAIN_)
	audio_drv_ctrl(AUD_CTRL_LR, setup.aud_bal, TRUE);
	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
#endif
#if defined(_WK5951CH_SPECIAL_DISPLAY_)
	radio_ctrl->band_flag = TRUE;
#endif

#if defined(_SUPPORT_TWO_PT2313_)
	audio_drv_ctrl1(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RADIO_ON, TRUE);
	audio_drv_ctrl1(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#endif

#if defined(_KLD3000_RADIO_LIGHT_)
	AMP_CONTROL_ON();
#endif

#if defined(_DISABLE_AMP_OFF_)
	if(global.amp_flag)
	{
		AMP_CONTROL_ON();
		global.amp_flag = FALSE;
	}
#endif
}

void radio_seek_stop(void)
//static void radio_seek_stop(void)
{
	radio_engine_auto_break();
	SET_BAND_TEMP_FREQ(radio_ctrl->eng_status.freq);
	radio_disp_polling();
	radio_engine_mute(OFF);
	//sys_mute_ctrl(DISABLE, 200);//
}

void fm_exit(void)
{
	fm_puts("fm_exit\n");
#if !defined(_ENABLE_SPECIAL_MUTE_)
#if defined(_DISABLE_POWER_OFF_MUTE_)
	if(global.power_mute)
	{
		global.power_mute = FALSE;
	}
	else
	{
		sys_mute_ctrl(ENABLE, 0);
	}
#else
	sys_mute_ctrl(ENABLE, 0);
#endif
#endif

#if defined(_KLD3000_RADIO_LIGHT_)
	AMP_CONTROL_OFF();
#endif

#if defined(_DISABLE_VOL_SEEK_)
	global.seek_up_flag = FALSE;
#endif

#if defined(_SEEK_REFRESH_FREQ_)
	global.pty_flag = 0;
#endif

#if defined(_XIMA_SPECIAL_SEEK_)
	global.radio_seek = 0;
#endif

#if defined(_DISABLE_FM_ICON_)
	global.radio_stast = 0;
#endif

#if defined(_ENABLE_READ_ST_TIME_)
	global.st_time = 0;
#endif

#if defined(_SPECIAL_CH_DISPLAY_)
	if(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE)
	{
		SET_RADIO_FREQ_INDEX(0,0);
		SET_RADIO_FREQ_INDEX(1,0);
		SET_RADIO_FREQ_INDEX(2,0);
	}
#endif

	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RADIO_OFF, TRUE);

#if defined(_SUPPORT_TWO_PT2313_)
	audio_drv_ctrl1(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RADIO_OFF, TRUE);
#endif

#if defined(_STOP_RADIO_SEEK_)
	if((GET_RADIO_STATE == RADIO_STATE_SEEK)||(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE))
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
	}
#endif

#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_DISABLE_RDS_MENMORY_)
	save_broadcast_rds_info();
#endif
	if(!radio_rds->enable)
#endif
	{
		radio_engine_mute(ON);
	}

	radio_engine_return(NULL);
	disp_send_param(DISP_EVENT_CALLBACK, NULL);
	
#if defined(_ENABLE_RADIO_RDS_)
	radio_engine_rds_busy(NULL);
	radio_engine_rds_front(FALSE);
#endif

#if defined(_SPECIAL_RDS_DELAY_)
	OSTimeDly(50);
#endif

#if defined(TD_ADD_NEW_APP)
#if defined(_ENABLE_RADIO_RDS_)
	setup.pty_type = 0;
	rds.rds_pty = 2;//重新转到收音要发一次
#endif
#endif

	radio_save_cfg();

	fm_engine_exit();

	fm_free();
}

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

static void radio_set_freq(void)
{
	if(GET_BAND_TEMP_FREQ > RADIO_MAX_FREQ)
	{
		SET_BAND_TEMP_FREQ(RADIO_MIN_FREQ);
	}
	else if(GET_BAND_TEMP_FREQ < RADIO_MIN_FREQ)
	{
		SET_BAND_TEMP_FREQ(RADIO_MAX_FREQ);
	}

	SET_RADIO_STO(FALSE);

	if(radio_ctrl->seek_soft)
	{
		radio_ctrl->seek_soft = FALSE;
		radio_ctrl->seek_wati = 0;

		if(radio_engine_softseek(GET_BAND_TEMP_FREQ))
		{
			radio_ctrl->seek_result = _B7_|_B0_;
		}
		else
		{
			radio_ctrl->seek_result = _B7_;
		}
	}
	else
	{
		radio_engine_freq(GET_BAND_TEMP_FREQ);
	}
}

static uint8 radio_load_broadcast(uint8 Broadcast)
{
	if(Broadcast > GET_RADIO_FREQ_LENGTH)return FALSE;

#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_DISABLE_RDS_MENMORY_)
	save_broadcast_rds_info();
#endif
#endif

#if defined(_ENABLE_MR1093_SET_FM_)
	SET_BAND_TEMP_FREQ(GET_BAND_FREQ(Broadcast));
	radio_set_freq();
	SET_BAND_FREQ_INDEX(Broadcast);
	OSTimeDly(2);
#endif

	SET_BAND_TEMP_FREQ(GET_BAND_FREQ(Broadcast));
	radio_set_freq();
	SET_BAND_FREQ_INDEX(Broadcast);

#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_DISABLE_RDS_MENMORY_)
	load_broadcast_rds_info();
#endif
#if defined(_CLEAN_RDS_PSN_)
	memset(radio_rds->rds_psn, 0, 8);
#endif
#endif

	radio_disp_polling();
	return TRUE;
}

static uint8 radio_save_broadcast(uint8 Broadcast)
{
	if(GET_RADIO_STATE != RADIO_STATE_IDLE)return FALSE;
	if(Broadcast > GET_RADIO_FREQ_LENGTH)return FALSE;
	SET_BAND_FREQ_INDEX(Broadcast);
	SET_BAND_FREQ(Broadcast, GET_BAND_TEMP_FREQ);

#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_DISABLE_RDS_MENMORY_)
	clean_broadcast_rds_info();
#endif
#endif
	return TRUE;
}

static void radio_set_band(uint8 Band)
{
	if(Band > GET_RADIO_BAND_LENGTH)return;

	SET_RADIO_STATE(RADIO_STATE_IDLE);
	SET_RADIO_BAND(Band);
	SET_RADIO_FREQ_LENGTH(RADIO_POS_LENGTH);
	radio_load_broadcast(GET_BAND_FREQ_INDEX);
}

static void radio_auto_seek(uint8 Mode)
{
	uint16 CurrentFreq;
	uint8 Freq;

	radio_engine_mute(ON);

	if((Mode & AUTO_SEEK_MASK_START) == AUTO_SEEK_MODE_CURRENT)
	{
		CurrentFreq = GET_BAND_TEMP_FREQ;
		Freq = (CurrentFreq - RADIO_MIN_FREQ)%RADIO_AUTO_STEP;

		if(Freq)
		{
			CurrentFreq -= Freq;
			if((Mode&AUTO_SEEK_MASK_DIR)==AUTO_SEEK_MODE_UP)
			{
				CurrentFreq += RADIO_AUTO_STEP;
			}
			SET_BAND_TEMP_FREQ(CurrentFreq);
			radio_set_freq();
			radio_engine_mute(ON);
		}
	}

	SET_RADIO_STO(FALSE);
	if(radio_band_loc_state())
	{
		Mode |= AUTO_SEEK_MODE_LOC;
	}

	radio_engine_auto_seek(	RADIO_MIN_FREQ,
							RADIO_MAX_FREQ,
							RADIO_AUTO_STEP,
							Mode);

#if defined(_ENABLE_RADIO_RDS_)
	memset(radio_rds->rds_psn, 0, 8);
#endif
}

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

void cmd_radio_save_number(radio_sel_e Sel)
{
	switch(GET_RADIO_STATE)
	{
		case RADIO_STATE_IDLE:
			radio_save_broadcast(Sel);
			break;

		case RADIO_STATE_SCAN:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			radio_save_broadcast(Sel);
			break;
			
    #if defined(_XM003_SPECIAL_SEEK_)
    	case RADIO_STATE_SEEK:
		#if defined(_XIMA_SPECIAL_SEEK_)
			radio_seek_stop();
		#endif
    		global.seek_flag = TRUE;
    		SET_RADIO_STATE(RADIO_STATE_IDLE);
    		radio_save_broadcast(Sel);
		#if !defined(_XIMA_SPECIAL_SEEK_)
    		SET_RADIO_STATE(RADIO_STATE_SEEK);
		#endif
    	case RADIO_STATE_AUTO_SAVE:
	    #if defined(_XIMA_SPECIAL_SEEK_)
	    	radio_seek_stop();
	    #endif
    		global.seek_flag = TRUE;
    		SET_RADIO_STATE(RADIO_STATE_IDLE);
    		radio_save_broadcast(Sel);
		#if !defined(_XIMA_SPECIAL_SEEK_)
    		SET_RADIO_STATE(RADIO_STATE_AUTO_SAVE);
		#endif
    		break;
    #endif

		default:return;
	}
#if defined(TD_ADD_BANDCH)
#if !defined(TD_ADD_CHADD_SUB)
	radio_ctrl->disp_bandch_flag=TRUE;
#endif
#endif

	radio_flash_enable(2, TRUE, RADIO_PAINT_CHANEL);
	radio_ctrl->manual_flag = FALSE;

#if defined(_ENABLE_SAVE_FREQ_)
	radio_ctrl->save_flag = FALSE;
	radio_ctrl->flash_flag = FALSE;
#endif
}

void cmd_radio_load_next(void)
{
	uint8 Index;

	if((GET_RADIO_STATE == RADIO_STATE_SEEK)||(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE))
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
	}
	else if(GET_RADIO_STATE == RADIO_STATE_BROWSE)
	{
		SET_RADIO_STATE(RADIO_STATE_IDLE);
	}

#if defined(_ENABLE_NEVUP_MUTE_)
	sys_mute_ctrl(ENABLE, 1000);
#else
	sys_mute_ctrl(ENABLE, 500);
#endif

#if defined(TD_ADD_BANDCH)
	radio_ctrl->disp_bandch_flag=TRUE;
#endif

	Index = GET_BAND_FREQ_INDEX;
	if(Index >= GET_RADIO_FREQ_LENGTH)
	{
		Index = RADIO_SEL_M1;
	}
	else
	{
		Index ++;
	}
	radio_load_broadcast(Index);
	radio_flash_disable();
	radio_ctrl->manual_flag = FALSE;
#if defined(_ENABLE_SAVE_FREQ_)
	radio_ctrl->save_flag = FALSE;
	radio_ctrl->flash_flag = FALSE;
#endif

#if defined(_ENABLE_RADIO_RDS_)
	if(radio_rds->enable)
	{
		radio_engine_rds_fast();
	}
#endif
}

void cmd_radio_load_prev(void)
{
	uint8 Index;

	if((GET_RADIO_STATE == RADIO_STATE_SEEK)||(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE))
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
	}
	else if(GET_RADIO_STATE == RADIO_STATE_BROWSE)
	{
		SET_RADIO_STATE(RADIO_STATE_IDLE);
	}

	sys_mute_ctrl(ENABLE, 500);
#if defined(TD_ADD_BANDCH)
	radio_ctrl->disp_bandch_flag=TRUE;
#endif

	Index = GET_BAND_FREQ_INDEX;
	if(Index <= RADIO_SEL_M1)
	{
		Index = GET_RADIO_FREQ_LENGTH;
	}
	else
	{
		Index --;
	}
	radio_load_broadcast(Index);
	radio_flash_disable();
	radio_ctrl->manual_flag = FALSE;

#if defined(_ENABLE_SAVE_FREQ_)
	radio_ctrl->save_flag = FALSE;
	radio_ctrl->flash_flag = FALSE;
#endif

#if defined(_ENABLE_RADIO_RDS_)
	if(radio_rds->enable)
	{
		radio_engine_rds_fast();
	}
#endif
}

#if defined(_ENABLE_SAVE_FREQ_)
void cmd_radio_slect_up(void)
{
#if defined(STATION_SLECT_MAKE_SEEK_STOP)
	if(GET_RADIO_STATE!=RADIO_STATE_IDLE)
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
	}
	else
#endif
	if(radio_ctrl->save_flag)
	{
    #if defined(_ENABLE_RADIO_WB_)
    	if(RADIO_STATE_WB)
    	{
    		if(radio_ctrl->saveIndex < 7)
    		{
    			radio_ctrl->saveIndex++;
    		}
    		else
    		{
    			radio_ctrl->saveIndex = 1;
    		}
    	}
    	else
    	{
    		if(radio_ctrl->saveIndex < 6)
    		{
    			radio_ctrl->saveIndex++;
    		}
    		else
    		{
    			radio_ctrl->saveIndex = 1;
    		}
    	}
    #else
	#if defined(_SPECIAL_NUMBER_SAVE_)
		if(radio_ctrl->saveIndex < 9)
	#else
		if(radio_ctrl->saveIndex < 6)
	#endif
		{
			radio_ctrl->saveIndex++;
		}
		else
		{
			radio_ctrl->saveIndex = 1;
		}
    #endif
		radio_flash_enable(8, TRUE, RADIO_PAINT_CH);
	}
	else
	{
		cmd_radio_load_next();
	}
}

void cmd_radio_save_up(void)
{
	if(!radio_ctrl->flash_flag)
	{
		radio_ctrl->save_flag = TRUE;
		radio_ctrl->flash_flag = TRUE;
		radio_ctrl->saveIndex = 1;
		radio_flash_enable(8, TRUE, RADIO_PAINT_CH);
	}
	else
	{
    	SET_RADIO_STATE(RADIO_STATE_IDLE);
    	radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
    	radio_disp_polling();
    	radio_save_broadcast(radio_ctrl->saveIndex);
    	radio_ctrl->flash_flag = FALSE;
    	radio_ctrl->save_flag = FALSE;
	}
}

void cmd_radio_slect_down(void)
{
#if defined(STATION_SLECT_MAKE_SEEK_STOP)
	if(GET_RADIO_STATE!=RADIO_STATE_IDLE)
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
	}
	else
#endif
	if(radio_ctrl->save_flag)
	{
    #if defined(_ENABLE_RADIO_WB_)
    	if(RADIO_STATE_WB)
    	{
    		if(radio_ctrl->saveIndex == 1)
    		{
    			radio_ctrl->saveIndex = 7;
    		}
    		else
    		{
    			radio_ctrl->saveIndex--;
    		}
    	}
    	else
    	{
    		if(radio_ctrl->saveIndex == 1)
    		{
    			radio_ctrl->saveIndex = 6;
    		}
    		else
    		{
    			radio_ctrl->saveIndex--;
    		}
    	}
    #else
		if(radio_ctrl->saveIndex == 1)
		{
		#if defined(_SPECIAL_NUMBER_SAVE_)
			radio_ctrl->saveIndex = 10;
		#else
			radio_ctrl->saveIndex = 6;
		#endif
		}
		else
		{
			radio_ctrl->saveIndex--;
		}
	#endif
		radio_flash_enable(8, TRUE, RADIO_PAINT_CH);
	}
	else
	{
		cmd_radio_load_prev();
	}
}

void cmd_radio_save_down(void)
{
	if(!radio_ctrl->flash_flag)
	{
		radio_ctrl->save_flag = TRUE;
		radio_ctrl->flash_flag = TRUE;
		radio_ctrl->saveIndex = 1;
		radio_flash_enable(8, TRUE, RADIO_PAINT_CH);
	}
	else
	{
    	SET_RADIO_STATE(RADIO_STATE_IDLE);
		radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
    	radio_save_broadcast(radio_ctrl->saveIndex);
    	radio_disp_polling();
		radio_ctrl->flash_flag = FALSE;
		radio_ctrl->save_flag = FALSE;
	}
}
#endif

#if defined(_SPECIAL_NUMBER6_FUNTION_)
void cmd_radio_load_number(radio_sel_e Sel)
{
	if(Sel == 6)
	{
		if(radio_ctrl->manual_flag)
		{
			if(GET_RADIO_STATE == RADIO_STATE_IDLE)
			{
				cmd_radio_freq_next();
				radio_ctrl->manual_timer = (5000/RADIO_POLLING_TIME);
			}
			else
			{
				radio_ctrl->manual_flag = FALSE;
			}
		}
		else
		{
    		sys_mute_ctrl(ENABLE, 500);
    		switch(GET_RADIO_STATE)
    		{
    			case RADIO_STATE_SCAN:
    				cmd_radio_save_number(Sel);
    				break;
    			case RADIO_STATE_SEEK:
    			case RADIO_STATE_AUTO_SAVE:
    				radio_seek_stop();
    				SET_RADIO_STATE(RADIO_STATE_IDLE);
    				break;
    			case RADIO_STATE_BROWSE:
    				SET_RADIO_STATE(RADIO_STATE_IDLE);
    			case RADIO_STATE_IDLE:
    			#if defined(TD_ADD_BANDCH)
    				radio_ctrl->disp_bandch_flag=TRUE;
    			#endif
    				if(Sel == RADIO_SEL_ADD)
    				{
    					cmd_radio_load_next();
    				}
    				else if(Sel == RADIO_SEL_SUB)
    				{
    					cmd_radio_load_prev();
    				}
    				else
    				{
    					radio_load_broadcast(Sel);
    				}
    			#if defined(_ENABLE_RADIO_RDS_)
    				if(radio_rds->enable)
    				{
    					radio_engine_rds_fast();
    				}
    			#endif
    				break;
    			default:return;
    		}
    	
    		radio_flash_disable();
    		radio_ctrl->manual_flag = FALSE;
    	#if defined(_ENABLE_SAVE_FREQ_)
    		radio_ctrl->save_flag = FALSE;
    		radio_ctrl->flash_flag = FALSE;
    		radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
    	#endif
    	
    	#if defined(_ENABLE_VERSION_VIEW_)
    		disp_send_param(DISP_EVENT_VERSION,Sel);
    	#endif
    	}
	}
	else
	{
		sys_mute_ctrl(ENABLE, 500);
		switch(GET_RADIO_STATE)
		{
			case RADIO_STATE_SCAN:
				cmd_radio_save_number(Sel);
				break;
			case RADIO_STATE_SEEK:
			case RADIO_STATE_AUTO_SAVE:
				radio_seek_stop();
				SET_RADIO_STATE(RADIO_STATE_IDLE);
				break;
			case RADIO_STATE_BROWSE:
				SET_RADIO_STATE(RADIO_STATE_IDLE);
			case RADIO_STATE_IDLE:
			#if defined(TD_ADD_BANDCH)
				radio_ctrl->disp_bandch_flag=TRUE;
			#endif
				if(Sel == RADIO_SEL_ADD)
				{
					cmd_radio_load_next();
				}
				else if(Sel == RADIO_SEL_SUB)
				{
					cmd_radio_load_prev();
				}
				else
				{
					radio_load_broadcast(Sel);
				}
			#if defined(_ENABLE_RADIO_RDS_)
				if(radio_rds->enable)
				{
					radio_engine_rds_fast();
				}
			#endif
				break;
			default:return;
		}
	
		radio_flash_disable();
		radio_ctrl->manual_flag = FALSE;
	#if defined(_ENABLE_SAVE_FREQ_)
		radio_ctrl->save_flag = FALSE;
		radio_ctrl->flash_flag = FALSE;
		radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
	#endif
	
	#if defined(_ENABLE_VERSION_VIEW_)
		disp_send_param(DISP_EVENT_VERSION,Sel);
	#endif
	}
}

#else

void cmd_radio_load_number(radio_sel_e Sel)
{
#if defined(_ENABLE_TIME_DISP_)
	global.work_flag = TRUE;
#endif

#if defined(_SUPPORT_C912DAB_)
    u8 cmd[2];
    if(gC912detOk)
    {
        cmd[0]=(0xc0+Sel);
        cmd[1]=(0x80+Sel);
        uart_interrupt_receive_write(cmd[0]);
        uart_interrupt_receive_write(cmd[1]);
    }
    else
#endif
    {
    #if defined(_SUPPORT_AMP_CONTROL_)		
    	global.number_flag = TRUE;
    #endif
	#if !defined(_DISABLE_MUTE_CONTROL_)
	#if defined(_XM007_RADIO_BEEP_)
    	sys_mute_ctrl(ENABLE, 200);
	#else
		sys_mute_ctrl(ENABLE, 500);
	#endif
    #endif
	
    #if defined(_XM003_SPECIAL_MUTE_)
    	if(global.aud_mute)
    	{
    		global.aud_mute = !global.aud_mute;
    		func_mute_control();
    	}
    #endif
        switch(GET_RADIO_STATE)
    	{
    		case RADIO_STATE_SCAN:
    			cmd_radio_save_number(Sel);
    			break;
    		case RADIO_STATE_SEEK:
    		case RADIO_STATE_AUTO_SAVE:
    			radio_seek_stop();
			#if defined(_XM003_SPECIAL_SEEK_)
				radio_load_broadcast(Sel);
			#endif
    			SET_RADIO_STATE(RADIO_STATE_IDLE);
    			break;
    		case RADIO_STATE_BROWSE:
    			SET_RADIO_STATE(RADIO_STATE_IDLE);
    		case RADIO_STATE_IDLE:
    		#if defined(TD_ADD_BANDCH)
    			radio_ctrl->disp_bandch_flag=TRUE;
    		#endif
    			if(Sel == RADIO_SEL_ADD)
    			{
    				cmd_radio_load_next();
    			}
    			else if(Sel == RADIO_SEL_SUB)
    			{
    				cmd_radio_load_prev();
    			}
    			else
    			{
			    #if defined(_XM003_SPECIAL_SEEK_)
			    	if(global.seek_flag)
			    	{
			    		radio_seek_stop();
			    		global.seek_flag = FALSE;
			    	}
			    #endif
    				radio_load_broadcast(Sel);
    			}
    		#if defined(_ENABLE_RADIO_RDS_)
    			if(radio_rds->enable)
    			{
    				radio_engine_rds_fast();
    			}
    		#endif
    			break;
    		default:return;
    	}

    	radio_flash_disable();
    	radio_ctrl->manual_flag = FALSE;
    #if defined(_ENABLE_SAVE_FREQ_)
    	radio_ctrl->save_flag = FALSE;
    	radio_ctrl->flash_flag = FALSE;
    	radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
    #endif
	}
#ifdef TD_ADD_NEW_APP
	new_app_radio_bandinfo(GET_RADIO_BAND,GET_BAND_FREQ_INDEX,GET_BAND_FREQ(GET_BAND_FREQ_INDEX));
#endif
#if defined(_ENABLE_VERSION_VIEW_)
	disp_send_param(DISP_EVENT_VERSION,Sel);
#endif
}
#endif

void cmd_radio_freq_prev(void)
{
#if !defined(_SPECIAL_RADIO_SEEK_)
#if !defined(_TY2200_RADIO_SEEK_)
    if(!radio_ctrl->manual_flag)
    {//当前是自动  长按后变手动
    	radio_ctrl->manual_flag = TRUE;
    	radio_ctrl->manual_timer = (5000/RADIO_POLLING_TIME);
	#if !defined(_ENABLE_MENU_SET_)
    	disp_send_param(DISP_EVENT_MANUAL, 0);
	#endif
    #if defined(_ENABLE_NEXT_PRVE_SET_)
    	OSTimeDly(200);
    #endif
    #if defined(_XIMA_SPECIAL_SEEK_)
    	if((GET_RADIO_STATE==RADIO_STATE_SEEK)||(GET_RADIO_STATE==RADIO_STATE_AUTO_SAVE))
    	{
    		radio_seek_stop();
    		SET_RADIO_STATE(RADIO_STATE_IDLE);
    	}
    #endif
    }
    else
#endif		
#endif
    {
    	if(GET_RADIO_STATE == RADIO_STATE_SEEK)
    	{
    		radio_seek_stop();
    		SET_RADIO_STATE(RADIO_STATE_IDLE);
    		return;
    	}
    	if(GET_RADIO_STATE != RADIO_STATE_IDLE)return;

    #if defined(_ENABLE_RADIO_RDS_)
	#if !defined(_DISABLE_RDS_MENMORY_)
    	save_broadcast_rds_info();
	#endif
	#endif

    	sys_mute_ctrl(ENABLE, 500);

    #if defined(_XM003_SPECIAL_MUTE_)
    	if(global.aud_mute)
    	{
    		global.aud_mute = !global.aud_mute;
    		func_mute_control();
    	}
    #endif
	
    #if defined(_ENABLE_NEXT_PRVE_SET_)
    	radio_ctrl->manual_flag = TRUE;
    	radio_ctrl->manual_timer = (5000/RADIO_POLLING_TIME);
    #endif

    	GET_BAND_TEMP_FREQ -= RADIO_MANUAL_STEP;
    	radio_set_freq();
    	SET_BAND_FREQ_INDEX(0);
    	radio_flash_disable();
    #if defined(_ENABLE_RADIO_RDS_)
		memset(radio_rds->rds_psn, 0, 8);
    #endif
		radio_disp_polling();
    }
}

void cmd_radio_freq_next(void)
{
#if !defined(_SPECIAL_RADIO_SEEK_)
#if !defined(_TY2200_RADIO_SEEK_)
    if(!radio_ctrl->manual_flag)
    {//当前是自动  长按后变手动
		radio_ctrl->manual_flag = TRUE;
    	radio_ctrl->manual_timer = (5000/RADIO_POLLING_TIME);
	#if !defined(_ENABLE_MENU_SET_)
		disp_send_param(DISP_EVENT_MANUAL, 0);
	#endif
    #if defined(_ENABLE_NEXT_PRVE_SET_)
    	OSTimeDly(200);
    #endif
	
	#if defined(_XIMA_SPECIAL_SEEK_)
		if((GET_RADIO_STATE==RADIO_STATE_SEEK)||(GET_RADIO_STATE==RADIO_STATE_AUTO_SAVE))
		{
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
		}
	#endif
    }
	else
#endif		
#endif
	{
    	if(GET_RADIO_STATE==RADIO_STATE_SEEK)
    	{
    		 radio_seek_stop();
    		 SET_RADIO_STATE(RADIO_STATE_IDLE);
    		 return;
    	}
    	if(GET_RADIO_STATE != RADIO_STATE_IDLE)return;

    #if defined(_ENABLE_RADIO_RDS_)
	#if !defined(_DISABLE_RDS_MENMORY_)
    	save_broadcast_rds_info();
	#endif
	#endif

    	sys_mute_ctrl(ENABLE, 500);

    #if defined(_XM003_SPECIAL_MUTE_)
    	if(global.aud_mute)
    	{
    		global.aud_mute = !global.aud_mute;
    		func_mute_control();
    	}
    #endif

	#if defined(_ENABLE_NEXT_PRVE_SET_)
		radio_ctrl->manual_flag = TRUE;
		radio_ctrl->manual_timer = (5000/RADIO_POLLING_TIME);
	#endif
	
    	GET_BAND_TEMP_FREQ += RADIO_MANUAL_STEP;
    	radio_set_freq();
    	SET_BAND_FREQ_INDEX(0);
    	radio_flash_disable();
    #if defined(_ENABLE_RADIO_RDS_)
		memset(radio_rds->rds_psn, 0, 8);
    #endif
		radio_disp_polling();
	}
}

#if defined(_EVB_SEEK_FUNTION_)
void cmd_radio_freq_prev1(void)
{
	if(GET_RADIO_STATE == RADIO_STATE_SEEK)
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
		return;
	}
	if(GET_RADIO_STATE != RADIO_STATE_IDLE)return;

#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_DISABLE_RDS_MENMORY_)
	save_broadcast_rds_info();
#endif
#endif

	sys_mute_ctrl(ENABLE, 500);

	GET_BAND_TEMP_FREQ -= RADIO_MANUAL_STEP;
	radio_set_freq();
	SET_BAND_FREQ_INDEX(0);
	radio_flash_disable();
#if defined(_ENABLE_RADIO_RDS_)
	memset(radio_rds->rds_psn, 0, 8);
#endif
	radio_disp_polling();
}

void cmd_radio_freq_next1(void)
{
	if(GET_RADIO_STATE==RADIO_STATE_SEEK)
	{
		 radio_seek_stop();
		 SET_RADIO_STATE(RADIO_STATE_IDLE);
		 return;
	}
	if(GET_RADIO_STATE != RADIO_STATE_IDLE)return;

#if defined(_ENABLE_RADIO_RDS_)
#if !defined(_DISABLE_RDS_MENMORY_)
	save_broadcast_rds_info();
#endif
#endif

	sys_mute_ctrl(ENABLE, 500);

	GET_BAND_TEMP_FREQ += RADIO_MANUAL_STEP;
	radio_set_freq();
	SET_BAND_FREQ_INDEX(0);
	radio_flash_disable();
#if defined(_ENABLE_RADIO_RDS_)
	memset(radio_rds->rds_psn, 0, 8);
#endif
	radio_disp_polling();
}
#endif

void cmd_radio_seek_next(void)
{
	if(radio_ctrl->manual_flag)
	{
		if(GET_RADIO_STATE == RADIO_STATE_IDLE)
		{
			cmd_radio_freq_next();
			radio_ctrl->manual_timer = (5000/RADIO_POLLING_TIME);
		}
		else
		{
			radio_ctrl->manual_flag = FALSE;
		}
	}
	else
	{
		switch(GET_RADIO_STATE)
		{
			case RADIO_STATE_IDLE:
	    #if defined(_XM003_SPECIAL_SEEK_)
	    	case RADIO_STATE_BROWSE:
	    	case RADIO_STATE_AUTO_SAVE:
	    #endif

			#if defined(_EVB_SPECIAL_SEEK__)
				radio_ctrl->save_freq = GET_BAND_TEMP_FREQ;
				ew_printf("save_freq:%d\n",radio_ctrl->save_freq);
			#endif
				sys_mute_ctrl(ENABLE, 0);
			#if defined(AUDIO_MUTE_SPECIAL_SET)
				HAUDIO_MUTE_ON();
			#endif

		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif

				SET_RADIO_STATE(RADIO_STATE_SEEK);
				SET_RADIO_DIR_UP;
				radio_auto_seek(AUTO_SEEK_MODE_UP|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
				break;
		#ifdef	TD_ADD_ALEN
			default://解决AMS时，按上下曲不能停止搜台的问题
				radio_seek_stop();
				SET_RADIO_STATE(RADIO_STATE_IDLE);
				break;
		#else

			case RADIO_STATE_SEEK:
				if(GET_RADIO_DIR_FLAG == DOWN)
				{
				#if defined(_ENABLE_SEEK_)
					radio_seek_stop();
					SET_RADIO_STATE(RADIO_STATE_IDLE);
				#else
					SET_RADIO_DIR_UP;
					radio_seek_stop();
					radio_auto_seek(AUTO_SEEK_MODE_UP|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
				#endif
				}
				else
				{
					radio_seek_stop();
					SET_RADIO_STATE(RADIO_STATE_IDLE);
				}
				break;

			case RADIO_STATE_SCAN:
				SET_RADIO_STATE(RADIO_STATE_SEEK);
				if(GET_RADIO_DIR_FLAG == DOWN)
				{
					SET_RADIO_DIR_UP;
					radio_seek_stop();
					radio_auto_seek(AUTO_SEEK_MODE_DOWN|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
				}
				break;

			default:return;
		#endif
		}

		SET_BAND_FREQ_INDEX(0);
		radio_flash_disable();
		radio_ctrl->manual_flag = FALSE;
    #if defined(_ENABLE_SAVE_FREQ_)
    	radio_ctrl->save_flag = FALSE;
    	radio_ctrl->flash_flag = FALSE;
    	radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
    #endif
	}
}

void cmd_radio_seek_prev(void)
{
	if(radio_ctrl->manual_flag)
	{
	    if(GET_RADIO_STATE == RADIO_STATE_IDLE)
	    {
			cmd_radio_freq_prev();
			radio_ctrl->manual_timer = (5000/RADIO_POLLING_TIME);
		}
		else
		{
			radio_ctrl->manual_flag = FALSE;
		}
	}
	else
	{
		switch(GET_RADIO_STATE)
		{
			case RADIO_STATE_IDLE:
	    #if defined(_XM003_SPECIAL_SEEK_)
	    	case RADIO_STATE_BROWSE:
	    	case RADIO_STATE_AUTO_SAVE:
	    #endif

			#if defined(_EVB_SPECIAL_SEEK__)
    		    radio_ctrl->save_freq = GET_BAND_TEMP_FREQ;
			#endif
				sys_mute_ctrl(ENABLE, 0);
			#if defined(AUDIO_MUTE_SPECIAL_SET)
				HAUDIO_MUTE_ON();
			#endif
			
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif

				SET_RADIO_STATE(RADIO_STATE_SEEK);
				SET_RADIO_DIR_DOWN;
				radio_auto_seek(AUTO_SEEK_MODE_DOWN|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
				break;
		#ifdef	TD_ADD_ALEN
			default://解决AMS时，按上下曲不能停止搜台的问题
				radio_seek_stop();
				SET_RADIO_STATE(RADIO_STATE_IDLE);
				break;
		#else
			case RADIO_STATE_SEEK:
				if(GET_RADIO_DIR_FLAG == UP)
				{
				#if defined(_ENABLE_SEEK_)
					radio_seek_stop();
					SET_RADIO_STATE(RADIO_STATE_IDLE);
				#else
					SET_RADIO_DIR_DOWN;
					radio_seek_stop();
					radio_auto_seek(AUTO_SEEK_MODE_DOWN|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
				#endif
				}
				else
				{
					radio_seek_stop();
					SET_RADIO_STATE(RADIO_STATE_IDLE);
				}
				break;

			case RADIO_STATE_SCAN:
				SET_RADIO_STATE(RADIO_STATE_SEEK);
				if(GET_RADIO_DIR_FLAG == UP)
				{
					SET_RADIO_DIR_DOWN;
					radio_seek_stop();
					radio_auto_seek(AUTO_SEEK_MODE_DOWN|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
				}
				break;

			default:return;
		#endif
		}

		SET_BAND_FREQ_INDEX(0);
		radio_flash_disable();
		radio_ctrl->manual_flag = FALSE;
    #if defined(_ENABLE_SAVE_FREQ_)
    	radio_ctrl->save_flag = FALSE;
        radio_ctrl->flash_flag = FALSE;
    	radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
    #endif
	}
}

void cmd_radio_stereo(void)
{
	if(GET_RADIO_STATE != RADIO_STATE_IDLE)return;
	if(RADIO_STATE_FM)
	{
		if(GET_RADIO_ST)
		{
			SET_RADIO_ST(FALSE);
			disp_send_param(DISP_EVENT_ST, 0);
		}
		else
		{
			SET_RADIO_ST(TRUE);
			disp_send_param(DISP_EVENT_ST, 1);
		}	
		#if defined(_USER_TFT_SEND_)
		   TFT_Tx_Radio_Status();
		#endif
	}
}

void cmd_radio_loc(void)
{
	if(GET_RADIO_STATE != RADIO_STATE_IDLE)return;

	if(RADIO_STATE_FM)
	{
		if(GET_RADIO_LOC)
		{
			SET_RADIO_LOC(FALSE);
			disp_send_param(DISP_EVENT_LOC, 0);
		}
		else
		{
			SET_RADIO_LOC(TRUE);
			disp_send_param(DISP_EVENT_LOC, 1);
		}
		
		#if defined(_USER_TFT_SEND_)
		   TFT_Tx_Radio_Status();
		#endif
	}
#if defined(SUPPORT_AM_LOC)
	else if(RADIO_STATE_AM)
	{
		if(GET_RADIO_AMLOC)
		{
			SET_RADIO_AMLOC(FALSE);
			disp_send_param(DISP_EVENT_LOC, 0);
		}
		else
		{
			SET_RADIO_AMLOC(TRUE);
			disp_send_param(DISP_EVENT_LOC, 1);
		}
	}
#endif
}

void cmd_radio_browse(void)
{
#if defined(_ENABLE_WB_FUNTION_)
	if(RADIO_STATE_WB)return;
#endif
	switch(GET_RADIO_STATE)
	{
		case RADIO_STATE_IDLE:
	    #if defined(_WJ361_BROWSE_MUTE_)
	    	global.browse_flag = TRUE;
	    #endif
			sys_mute_ctrl(ENABLE, 0);
		#if defined(AUDIO_MUTE_SPECIAL_SET)
			HAUDIO_MUTE_ON();
		#endif
			radio_ctrl->BrowseBand  = GET_RADIO_BAND;
		#if !defined(_SPECIAL_BROWSE_FUNCTION)
			SET_RADIO_BAND(SearchFristBand());
		#endif
			radio_ctrl->BrowseIndex = GET_BAND_FREQ_INDEX;
			SET_BAND_FREQ_INDEX(1);
			OSTimeDly(10);
			radio_ctrl->seek_soft = TRUE;
			radio_set_band(GET_RADIO_BAND);
			SET_RADIO_STATE(RADIO_STATE_BROWSE);
			break;

		case RADIO_STATE_SEEK:
		case RADIO_STATE_SCAN:
		case RADIO_STATE_AUTO_SAVE:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			break;

		case RADIO_STATE_BROWSE:
    		radio_seek_stop();
    		SET_RADIO_STATE(RADIO_STATE_IDLE);
    		radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ);
			break;

		default:return;
	}

	radio_flash_disable();
	radio_ctrl->manual_flag = FALSE;
#if defined(_ENABLE_SAVE_FREQ_)
	radio_ctrl->save_flag = FALSE;
	radio_ctrl->flash_flag = FALSE;
#endif
}

void cmd_radio_auto_save(void)
{
#if defined(_ENABLE_WB_FUNTION_)
	if(RADIO_STATE_WB)return;
#endif
	switch(GET_RADIO_STATE)
	{
		case RADIO_STATE_SEEK:
		case RADIO_STATE_SCAN:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
		case RADIO_STATE_IDLE:
	#if !defined(_FY7000_AMS_MUTE_)
	#if !defined(_ENABLE_RCA_MUTE_)
	#if !defined(_ENABLE_AMS_MUTE_)
		RADIO_AUTO_SAVE:
	#endif
	#endif
	#endif
			sys_mute_ctrl(ENABLE, 0);
		#if defined(AUDIO_MUTE_SPECIAL_SET)
			HAUDIO_MUTE_ON();
		#endif
		
        #if defined(_XM003_SPECIAL_MUTE_)
        	if(global.aud_mute)
        	{
        		global.aud_mute = !global.aud_mute;
        		func_mute_control();
        	}
        #endif
		
	    #if !defined(_SPECIAL_BROWSE_FUNCTION)
	    	SET_RADIO_BAND(SearchFristBand());
	    #endif
			RadioInitBandFreq();

		#if !defined(CANCEL_LOC_WHEN_AMS)
			if(RADIO_STATE_FM)
			{
			#if defined(_ENABLE_ST_MONO_)&&0//内置收音扫描只扫一遍
				if(fm_inside_check())
				{
					SET_RADIO_AUTOLOC(FALSE);
				}
				else
			#endif
				{
					SET_RADIO_AUTOLOC(TRUE);
					setup.loc_flg = TRUE;
				}
			}
	    #if defined(SUPPORT_AM_LOC)
	    	else if(RADIO_STATE_AM)
	    	{
	    		SET_RADIO_AUTOLOC(TRUE);
	    	}
	    #endif
			else
		#endif
			{
				SET_RADIO_AUTOLOC(FALSE);
			}

		#if defined(_ENABLE_RADIO_RDS_)
			if(radio_rds->enable)
			{
				memset(&radio_rds->rds_ams, 0, sizeof(radio_ams_rds_t));
			#if !defined(_DISABLE_RDS_MENMORY_)
				memset(&radio_rds->rds_vm, 0, sizeof(radio_vm_rds_t));
			#endif

		    #if defined(_RADIO_RDS_SAVE_ALL_STATION_)
		    	if(setup.rds.ta)
		    #else
		    	if(setup.rds.af || setup.rds.ta)
		    #endif
				{
					SET_RADIO_AUTOLOC(FALSE);
				}
			}
		#endif

			SET_RADIO_STATE(RADIO_STATE_AUTO_SAVE);
	    #if defined(_SPECIAL_AUTOSEEK_MEMORY_)||defined(_YH1701_AUTOSEEK_MEMORY_)
	    	radio_ctrl->saveindex = 0;
	    #endif
		#ifdef  TD_ADD_NEW_APP
			new_app_radio_bandinfo(GET_RADIO_BAND,GET_BAND_FREQ_INDEX,GET_BAND_FREQ(GET_BAND_FREQ_INDEX));
		#endif
			radio_auto_seek(AUTO_SEEK_MODE_UP|AUTO_SEEK_MODE_MIN|AUTO_SEEK_MODE_ROUND);
			break;
		case RADIO_STATE_AUTO_SAVE:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			break;
		case RADIO_STATE_BROWSE:
		#if defined(_FY7000_AMS_MUTE_)||defined(_ENABLE_RCA_MUTE_)||defined(_ENABLE_AMS_MUTE_)
    		radio_seek_stop();
    		SET_RADIO_STATE(RADIO_STATE_IDLE);
    		radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ);
		#else
			cmd_radio_browse();
			goto RADIO_AUTO_SAVE;
		#endif
		default:return;
	}

	radio_flash_disable();
	radio_ctrl->manual_flag = FALSE;
#if defined(_ENABLE_SAVE_FREQ_)
	radio_ctrl->save_flag = FALSE;
	radio_ctrl->flash_flag = FALSE;
	radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
#endif
}

void cmd_radio_scan(void)
{
	switch(GET_RADIO_STATE)
	{
		case RADIO_STATE_BROWSE:
			SET_RADIO_STATE(RADIO_STATE_IDLE);
		case RADIO_STATE_IDLE:
			sys_mute_ctrl(ENABLE, 0);
			
		#if defined(AUDIO_MUTE_SPECIAL_SET)
			HAUDIO_MUTE_ON();
		#endif
			SET_RADIO_STATE(RADIO_STATE_SCAN);
			SET_RADIO_DIR_UP;
			radio_seek_stop();
			radio_auto_seek(AUTO_SEEK_MODE_UP|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
			break;

		case RADIO_STATE_SEEK:
			SET_RADIO_STATE(RADIO_STATE_SCAN);
			if(GET_RADIO_DIR_FLAG == DOWN)
			{
				SET_RADIO_DIR_UP;
				radio_seek_stop();
				radio_auto_seek(AUTO_SEEK_MODE_UP|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
			}
			break;

		case RADIO_STATE_SCAN:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			break;

		case RADIO_STATE_AUTO_SAVE:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			cmd_radio_auto_save();
			break;
		default:return;
	}

	SET_BAND_FREQ_INDEX(0);
	radio_flash_disable();
	radio_ctrl->manual_flag = FALSE;
}

#if defined(_XZC1668_RADIO_FUNTION_)
uint8 radio_band = 0;
#endif
void cmd_radio_band(void)
{
	uint8 prev_band;
	uint8 Band;
	
#if defined(_ENABLE_BAND_MUTE_)
	global.mute_flag = TRUE;
	global.mute_time = 25;
#endif

#ifdef	TD_ADD_ALEN
#if !defined(_XZC1668_RADIO_FUNTION_)
	if(GET_RADIO_STATE !=RADIO_STATE_IDLE)
	{//解决在收音任务时，按BAND键会加BAND的问题
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
		radio_flash_disable();
		return;
	}
#endif
#else
	switch(GET_RADIO_STATE)
	{
		case RADIO_STATE_SEEK:
		case RADIO_STATE_SCAN:
		case RADIO_STATE_AUTO_SAVE:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			radio_flash_disable();
			break;
		default:
			break;
	}
#endif

#if defined(_TUNER_DRIVER_EW730_)||defined(_BL7890_BAND_MUTE_)||defined(_EVB_BAND_MUTE_)||defined(_ENABLE_RADIO_WB_)
	sys_mute_ctrl(ENABLE, 1000);
#else
#if !defined(_ENABLE_BAND_MUTE_)
#if !defined(_DISABLE_MUTE_CONTROL_)
    sys_mute_ctrl(ENABLE, 500);
#endif
#endif
#endif

#if defined(_WLGK_BAND_MUTE_)
	sys_mute_ctrl(ENABLE, 2000);
#endif

#if defined(_XM003_SPECIAL_MUTE_)
	if(global.aud_mute)
	{
		global.aud_mute = !global.aud_mute;
		func_mute_control();
	}
#endif

	prev_band = Band = GET_RADIO_BAND;

next_band:
#if defined(_XZC1668_RADIO_FUNTION_)
	if(setup.fm_flag)
	{
		if(Band == 3)
		{
			Band = 4;
			setup.am_band = 3;
		}
		else if(Band == 4)
		{
			Band = 3;
			setup.am_band = 4;
		}
	}
	else
	{
		if(Band == 0)
		{
			Band = 1;
			setup.fm_band = 0;
		}
		else if(Band == 1)
		{
			Band = 2;
			setup.fm_band = 1;
		}
		else if(Band == 2)
		{
			Band = 0;
			setup.fm_band = 2;
		}
	}
#else
	if(Band < (GET_RADIO_BAND_LENGTH - 1))Band++;
	else Band = 0;
#endif
	if(Band == GET_RADIO_BAND)return;

	if(radio_ctrl->fm_only)
	{
	#if !defined(_XZC1668_RADIO_FUNTION_)
		if(!INDEX_RADIO_STATE_FM(Band))
		{
			goto next_band;
		}
	#endif
	}

	ew_printf("prev_band:%d\n",prev_band);
	ew_printf("Band:%d\n",Band);
	if(prev_band != Band)
	{
		ew_printf("Band111\n");
		if(Band > GET_RADIO_BAND_LENGTH)return;
		ew_printf("Band222\n");
	#if defined(TD_ADD_BANDCH)
	#if !defined(HuiNuo_6651D_Copy)//惠诺抄机切换band不需要显示
		radio_ctrl->disp_bandch_flag=TRUE;
	#endif
	#endif

    #if defined(_ENABLE_SAVE_FREQ_)
    	radio_ctrl->save_flag = FALSE;
        radio_ctrl->flash_flag = FALSE;
    	radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ|RADIO_PAINT_CH);
    #endif

		SET_RADIO_STATE(RADIO_STATE_IDLE);

		SET_RADIO_BAND(Band);
		SET_RADIO_FREQ_LENGTH(RADIO_POS_LENGTH);
		
		if(((INDEX_RADIO_STATE(prev_band)&RADIO_INFO_STATE_BAND_SCREEN)!=(INDEX_RADIO_STATE(Band)&RADIO_INFO_STATE_BAND_SCREEN))
    #if defined(_ENABLE_RADIO_RDS_)||defined(_SUPPORT_ORIT_ON_)
    	||((INDEX_RADIO_STATE(prev_band)&(RADIO_INFO_STATE_RDS_SCREEN|RADIO_INFO_STATE_RBDS_SCREEN))
    	 !=(INDEX_RADIO_STATE(Band)&(RADIO_INFO_STATE_RDS_SCREEN|RADIO_INFO_STATE_RBDS_SCREEN)))
    #if defined(_XZC1668_RADIO_FUNTION_)
    	||(band != setup.fm_flag)
    #endif
    #endif
	#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
		||((INDEX_RADIO_STATE(prev_band)&(RADIO_INFO_STATE_RDS_SCREEN|RADIO_INFO_STATE_RBDS_SCREEN))!=(INDEX_RADIO_STATE(Band)&(RADIO_INFO_STATE_RDS_SCREEN|RADIO_INFO_STATE_RBDS_SCREEN)))
    #if defined(_XZC1668_RADIO_FUNTION_)
    	||(radio_band != setup.fm_flag)
    #endif
	#endif
		)
		{
			ew_puts("Band333\n");
			
		#if defined(_XZC1668_RADIO_FUNTION_)
			radio_band = setup.fm_flag;
		#endif
		
			radio_engine_mute(ON);
			if(!radio_engine_init())
			{
				goto next_band;
			}
			radio_engine_mute(OFF);

		#if defined(_ENABLE_RADIO_RDS_)
			radio_engine_rds_state();
		#endif
		}
		radio_load_broadcast(GET_BAND_FREQ_INDEX);
	#ifdef TD_ADD_NEW_APP
		new_app_send_bandinfo();//每次切band需要重置6个电台
	#endif
		radio_flash_disable();
		radio_ctrl->manual_flag = FALSE;
	#if defined(_ENABLE_RADIO_RDS_)
		if(radio_rds->enable)
		{
			radio_engine_rds_fast();
		}
	#endif
	}
}

#if defined(_XZC1668_RADIO_FUNTION_)
#if 1
void cmd_radio_fm(void)
{
	ew_printf("GET_FM_BAND:%d\n",GET_RADIO_BAND);
	if(GET_RADIO_STATE != RADIO_STATE_IDLE)
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
		radio_flash_disable();
		OSTimeDly(1);
	}
	setup.fm_flag = FALSE;
	if(GET_RADIO_BAND == 0)
	{
		cmd_radio_band();
	}
	else if(GET_RADIO_BAND == 1)
	{
		cmd_radio_band();
	}
	else if(GET_RADIO_BAND == 2)
	{
		cmd_radio_band();
	}
	else if((GET_RADIO_BAND == 3)||(GET_RADIO_BAND == 4))
	{
		SET_RADIO_BAND(setup.fm_band);
		cmd_radio_band();
	}
}
void cmd_radio_am(void)
{
	ew_printf("GET_AM_BAND:%d\n",GET_RADIO_BAND);
	if(GET_RADIO_STATE != RADIO_STATE_IDLE)
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
		radio_flash_disable();
		OSTimeDly(1);
	}
	
	setup.fm_flag = TRUE;
	if(GET_RADIO_BAND == 3)
	{
		cmd_radio_band();
	}
	else if(GET_RADIO_BAND == 4)
	{
		cmd_radio_band();
	}
	else if((GET_RADIO_BAND == 0)||(GET_RADIO_BAND == 1)||(GET_RADIO_BAND == 2))
	{
		SET_RADIO_BAND(setup.am_band);
		cmd_radio_band();
	}
}

#else
void cmd_radio_fm(void)
{
	ew_printf("GET_FM_BAND:%d\n",GET_RADIO_BAND);
	if(GET_RADIO_BAND == 3)
	{
		if(GET_RADIO_STATE != RADIO_STATE_IDLE)
		{
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			radio_flash_disable();
			OSTimeDly(1);
		}
		SET_RADIO_BAND(4);
		cmd_radio_band();
	}
}
void cmd_radio_am(void)
{
	ew_printf("GET_AM_BAND:%d\n",GET_RADIO_BAND);
	if((GET_RADIO_BAND == 0)||(GET_RADIO_BAND == 1))
	{
		if(GET_RADIO_STATE != RADIO_STATE_IDLE)
		{
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			radio_flash_disable();
			OSTimeDly(1);
		}
		SET_RADIO_BAND(2);
		cmd_radio_band();
	}
}
#endif
#endif

void cmd_radio_stop(void)
{
	switch(GET_RADIO_STATE)
	{
		case RADIO_STATE_SEEK:
		case RADIO_STATE_SCAN:
		case RADIO_STATE_AUTO_SAVE:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			radio_flash_disable();
			break;

		default:
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			break;
	}
}

#if defined(_SUPPORT_AREA_)
static void cmd_radio_area_set(setup_area_e area)
{
	uint8 band = 0;// GET_RADIO_BAND
	uint8 freq_index = 0;// GET_BAND_FREQ_INDEX
	uint8 band_state;

	radio_config->radio_info.area = area;

	sys_mute_ctrl(ENABLE, 300);//

#if defined(_XZC1668_RADIO_FUNTION_)
	radio_band = FALSE;
#endif

	band_state = (INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_BAND_SCREEN);

	switch(GET_RADIO_STATE)
	{
		case RADIO_STATE_SEEK:
		case RADIO_STATE_SCAN:
		case RADIO_STATE_AUTO_SAVE:
			radio_seek_stop();
			SET_RADIO_STATE(RADIO_STATE_IDLE);
			break;

		case RADIO_STATE_BROWSE:
			cmd_radio_browse();
			break;

		default:
			break;
	}

	RadioInitBand();
	RadioInitFreq();

	if(band_state != (INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_BAND_SCREEN))
	{
		for(band = 0;band < GET_RADIO_BAND_LENGTH; band++)
		{
			if(band_state == (INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_BAND_SCREEN))
			{
				break;
			}
		}
	}

	SET_RADIO_BAND(band);
	SET_BAND_FREQ_INDEX(freq_index);
	SET_RADIO_FREQ_LENGTH(RADIO_POS_LENGTH);
	radio_ctrl->manual_flag = FALSE;

#if defined(_ENABLE_SAVE_FREQ_)
	radio_ctrl->save_flag = FALSE;
	radio_ctrl->flash_flag = FALSE;
#endif

#if defined(_ENABLE_RADIO_RDS_)
#if defined(_DISABLE_RDS_FUNTION_)
	if(setup.area == SETUP_AREA_USA)
	{
		setup.rds.af = FALSE;
		setup.rds.ta = FALSE;
    	if(rds_app->pty_search.enable)
    	{
    		os_taskq_post_msg("FMRadioTask", 2, MSG_FM_RDS_PTY, DISABLE);
    		rds_app->pty_search.enable = FALSE;
    		rds_cmd_engine_txt(RDS_TXT_NULL);
    	}
	}
	else if(setup.area == SETUP_AREA_EUR)
	{
		setup.rds.af = TRUE;
		rds_app->pty_search.enable = TRUE;
	}
#else
	setup.rds.af = TRUE;
	setup.rds.ta = FALSE;
	if(rds_app->pty_search.enable)
	{
		os_taskq_post_msg("FMRadioTask", 2, MSG_FM_RDS_PTY, DISABLE);
		rds_app->pty_search.enable = FALSE;
		rds_cmd_engine_txt(RDS_TXT_NULL);
	}
#endif

	radio_engine_rds_state();
	radio_engine_rds_front(TRUE);
	radio_engine_rds_busy(radio_rds_busy);
#if !defined(_DISABLE_RDS_MENMORY_)
	radio_rds->enter_flag = TRUE;
	load_broadcast_rds_info();
#endif
	rds_ctrl_reset();
#endif

#if !defined(_DISABLE_FM_INIT_)
	radio_engine_init();
#endif
	radio_load_broadcast(GET_BAND_FREQ_INDEX);
	radio_flash_disable();
}
#endif

#if defined(_ENABLE_RDS_ON_OFF_)
void cmd_radio_rds_onoff(uint8 onoff)
{
	printf("onoff:%d\n",onoff);
	if(!onoff)
	{
		setup.rds.af = FALSE;
		setup.rds.ta = FALSE;
		setup.rds.reg = FALSE;
		radio_engine_rds_state();
		radio_engine_rds_front(TRUE);
		radio_engine_rds_busy(radio_rds_busy);
	#if !defined(_DISABLE_RDS_MENMORY_)
		radio_rds->enter_flag = TRUE;
		load_broadcast_rds_info();
	#endif
		rds_ctrl_reset();
	
	#if !defined(_DISABLE_FM_INIT_)
		radio_engine_init();
	#endif
		radio_load_broadcast(GET_BAND_FREQ_INDEX);
		radio_flash_disable();
	}
}
#endif

#if defined(_ENABLE_RADIO_RDS_)
void cmd_radio_psn(const uint8 *psn)
{
	memcpy(radio_rds->rds_psn, psn, 8);
	radio_ctrl->paint_flag |= RADIO_PAINT_PSN;
}
#endif

#if defined(_SPECIAL_AUTOSEEK_MEMORY_)||defined(_YH1701_AUTOSEEK_MEMORY_)
void RadioFreqRssiSort(uint8* pMRssi,uint16* pMFreq,uint8 Point)
{
	uint8 i,j;
	uint8 temprssi;
	uint16 tempfreq;
	for(i=0;i<Point;i++)
	{
		for(j=i+1;j<Point;j++)
		{
			if(pMRssi[i]<pMRssi[j])
			{
				temprssi = pMRssi[i];
				tempfreq = pMFreq[i];
				pMRssi[i] = pMRssi[j];
				pMFreq[i] = pMFreq[j];
				pMRssi[j] = temprssi;
				pMFreq[j] = tempfreq;
			}
		}
	}
}

void RadioFreqSort(uint16* pMFreq,uint8 Point)
{
	uint8 i,j;
	uint16 tempfreq;
	for(i=0;i<Point;i++)
	{
		for(j=i+1;j<Point;j++)
		{
		#if !defined(_WJ361_AUTOSEEK_MEMORY_)
			if(pMFreq[i]>pMFreq[j])
		#endif
			{
				tempfreq = pMFreq[i];
				pMFreq[i] = pMFreq[j];
				pMFreq[j] = tempfreq;
			}
		}
	}
}
#endif

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 radio_auto_change_band(void)
{
	uint8 Band;
	Band = SearchNextBand(GET_RADIO_BAND);
	if(Band <= GET_RADIO_BAND)return FALSE;
	SET_RADIO_BAND(Band);
	SET_RADIO_FREQ_LENGTH(INDEX_RADIO_POS_LENGTH(Band));
#ifdef  TD_ADD_NEW_APP
	new_app_send_bandinfo();
#endif
	return TRUE;
}

void radio_disp_polling(void)
{
	//printf("radio_ctrl->disp_freq=%8d\n",radio_ctrl->disp_freq);
	//printf("GET_BAND_TEMP_FREQ=%8d\n",GET_BAND_TEMP_FREQ);

	if(radio_ctrl->disp_state != GET_RADIO_STATE)
	{
		radio_ctrl->disp_state = GET_RADIO_STATE;
		radio_ctrl->paint_flag |= RADIO_PAINT_STATE;
		#if defined(_USER_TFT_SEND_)
		  TFT_Tx_Radio_Status();
		#endif
	}

	if(radio_ctrl->disp_freq != GET_BAND_TEMP_FREQ)
	{
		radio_ctrl->disp_freq = GET_BAND_TEMP_FREQ;
		radio_ctrl->paint_flag |= RADIO_PAINT_FREQ;
	}

	if(radio_ctrl->disp_band != GET_RADIO_BAND)
	{
		radio_ctrl->disp_band = GET_RADIO_BAND;
		radio_ctrl->paint_flag |= RADIO_PAINT_FREQ;
	}

	if(radio_ctrl->disp_index != GET_BAND_FREQ_INDEX)
	{
		radio_ctrl->disp_index = GET_BAND_FREQ_INDEX;
		radio_ctrl->paint_flag |= RADIO_PAINT_CHANEL;
	}

	uint8 disp_flag;
	disp_flag  = (radio_band_st_state()  ? RADIO_DISP_FLAG_ST  : 0);
	disp_flag |= (radio_band_sto_state() ? RADIO_DISP_FLAG_STO : 0);
	disp_flag |= (radio_band_loc_state() ? RADIO_DISP_FLAG_LOC : 0);

#if defined(TD_ADD_BANDCH)
	if(radio_ctrl->disp_bandch_flag == TRUE)
	{
		disp_flag |= RADIO_DISP_FLAG_CH;
		radio_ctrl->disp_bandch_flag = FALSE;
	}
#endif
#if defined(_WK5951CH_SPECIAL_DISPLAY_)
	if(radio_ctrl->band_flag)
	{
		 disp_flag |= RADIO_DISP_FLAG_CH1;
		 radio_ctrl->band_flag = FALSE;
	}
#endif

#if defined(_ENABLE_SAVE_FREQ_)
	if(radio_ctrl->save_flag)
	{
		 disp_flag |= RADIO_DISP_FLAG_SAVE_CH;
	}
#endif

	if(radio_ctrl->disp_flag != disp_flag)
	{
		radio_ctrl->disp_flag = disp_flag;
		radio_ctrl->paint_flag |= RADIO_PAINT_FLAG;
	}
#if defined(SPECIAL_RDS_PSN)
	if((radio_ctrl->paint_flag)||(!setup.rds.af))
	{
	//if((disp_flag != 0x03)||(!setup.rds.af))// 收到RDS信号disp_flag为3
		if(!setup.rds.af)// 收到RDS信号disp_flag为3
		{
			memset(radio_rds->rds_psn, 0, 8);
		}
	#ifdef	TD_ADD_SPP
        Spp_Tx_Radio_Info(GET_BAND_TEMP_FREQ);
	#endif
		radio_disp_info();
		radio_ctrl->paint_flag = 0;
	}
#else
    if(radio_ctrl->paint_flag)
    {
	#ifdef	TD_ADD_SPP
        Spp_Tx_Radio_Info(GET_BAND_TEMP_FREQ);
    #if defined(TD_ADD_NEW_APP)
		if(GET_RADIO_STATE == RADIO_STATE_IDLE)//修改AMS存台后到第一个电台播放不会高亮
		{
			new_app_radio_bandinfo(GET_RADIO_BAND,GET_BAND_FREQ_INDEX,GET_BAND_FREQ(GET_BAND_FREQ_INDEX));
		}
    #endif
	#endif
	#if defined(_USER_TFT_SEND_)
		TFT_Tx_Radio_Info(GET_BAND_TEMP_FREQ);
	#endif

    	radio_disp_info();
    	radio_ctrl->paint_flag = 0;
    }
#endif	
}


#if defined(no_disp_87_5)
	u8 auto_save_freq_FLAG =0;
	uint16 auto_save_freq = 0;
#endif

#if defined(_ENABLE_RADIO_RDS_)
static uint8 radio_engine_reply(uint8 reply, uint16 freq, uint16 pi, uint8 signal)
#else
static uint8 radio_engine_reply(uint8 reply, uint16 freq)
#endif
{
#if defined(_SPECIAL_AUTOSEEK_MEMORY_)||defined(_YH1701_AUTOSEEK_MEMORY_)
	uint8 i;
	uint8 rssi;
	rssi = setup.rssi;
	//ew_printf("setup.rssi--A:%d\n", setup.rssi);
#endif
	uint8 stop = FALSE;

#if defined(_ENABLE_RADIO_RDS_)
	uint8 rds_auto = FALSE;

	if(reply & AUTO_REPLY_RDS)
	{
		rds_auto = TRUE;
	}

	if(reply & AUTO_REPLY_SEARCH)
	{
		SET_BAND_CURRENT_FREQ(freq);
	}
#endif

	if(reply & AUTO_REPLY_REFUR)
	{
		SET_BAND_TEMP_FREQ(freq);
	}

	if(reply & AUTO_REPLY_RESET)
	{
		SET_BAND_FREQ_INDEX(0);
	}

	if(reply & AUTO_REPLY_STATION)
	{
		if(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE)
		{
			uint8 result = TRUE;

			if(GET_RADIO_AUTOLOC)
			{//开LOC扫描

			}
			else
			{//关LOC扫描
				if(SearchFreqNo(GET_BAND_TEMP_FREQ))
				{
					result = FALSE;
				}
			}
			if(result)
			{
			#if defined(_ENABLE_RADIO_RDS_)
				if(rds_auto)
				{
					if(pi)
					{
						uint8 frist_band;
						uint8 index;

						frist_band = SearchFristBand();

						while(TRUE)
						{
							for(index = 1; index < GET_RADIO_FREQ_INDEX(frist_band); index++)
							{
								if(pi == GET_RADIO_RDS_INFO(frist_band,index).pi)
								{
									if(signal > GET_RADIO_RDS_INFO(frist_band,index).signal)
									{
										GET_RADIO_RDS_INFO(frist_band,index).signal = signal;
										SET_RADIO_FREQ(frist_band,index,GET_BAND_TEMP_FREQ);
									}
									goto exit_auto_station;
								}
							}

							if(frist_band == GET_RADIO_BAND)break;

							frist_band++;
						}
					}
					else goto exit_auto_station;
				}
			#endif

	     	#if defined(no_disp_87_5)
		    	auto_save_freq_FLAG =1;
				auto_save_freq=GET_BAND_TEMP_FREQ;
				fm_printf("auto_save_freq:%d\n", auto_save_freq);
			#endif
			
		    #if defined(_SPECIAL_AUTOSEEK_MEMORY_)
		    	if(radio_ctrl->saveindex < 6)
		    	{
		    		radio_ctrl->rssi1[radio_ctrl->saveindex] = rssi;
		    		radio_ctrl->freq1[radio_ctrl->saveindex] = GET_BAND_TEMP_FREQ;
		    		radio_ctrl->saveindex++;
		    	}
		    	else
		    	{
		    		radio_ctrl->saveindex = 6;
		    		radio_ctrl->rssi1[6] = rssi;
		    		radio_ctrl->freq1[6] = GET_BAND_TEMP_FREQ;
		    		RadioFreqRssiSort(radio_ctrl->rssi1,radio_ctrl->freq1,7);
		    	}
			#elif defined(_YH1701_AUTOSEEK_MEMORY_)
				if(radio_ctrl->saveindex < 10)
				{
					radio_ctrl->rssi1[radio_ctrl->saveindex] = rssi;
					radio_ctrl->freq1[radio_ctrl->saveindex] = GET_BAND_TEMP_FREQ;
					radio_ctrl->saveindex++;
				}
				else
				{
					radio_ctrl->saveindex = 10;
					radio_ctrl->rssi1[10] = rssi;
					radio_ctrl->freq1[10] = GET_BAND_TEMP_FREQ;
					RadioFreqRssiSort(radio_ctrl->rssi1,radio_ctrl->freq1,11);
				}
		    #else
		    	SET_BAND_CURRENT_FREQ(GET_BAND_TEMP_FREQ);
		    #endif
			#if defined(_ENABLE_RADIO_RDS_)
				if(rds_auto)
				{
					GET_BAND_RDS_CURRENT_INFO.pi 	 = pi;
					GET_BAND_RDS_CURRENT_INFO.signal = signal;
				}
			#endif
			#ifdef  TD_ADD_NEW_APP
				new_app_radio_bandinfo(GET_RADIO_BAND,GET_BAND_FREQ_INDEX,GET_BAND_FREQ(GET_BAND_FREQ_INDEX));
			#endif
				if((++GET_BAND_FREQ_INDEX) > GET_RADIO_FREQ_LENGTH)
				{
					SET_BAND_FREQ_INDEX(GET_RADIO_FREQ_LENGTH);
				#if !defined(_SPECIAL_BROWSE_FUNCTION)
					if(radio_auto_change_band() == FALSE)
					{
						stop = TRUE;
						fm_puts("auto save full\n");
						goto EXIT_AUTO_SAVE;
					}
				#endif
				}
			}
		}
	}

#if defined(_ENABLE_RADIO_RDS_)
exit_auto_station:
#endif

	if(reply & AUTO_REPLY_STOP)
	{
		switch(GET_RADIO_STATE)
		{
			case RADIO_STATE_IDLE:
			case RADIO_STATE_BROWSE:
				radio_engine_mute(OFF);
				break;

			case RADIO_STATE_SEEK:
				radio_engine_mute(OFF);
				SET_RADIO_STATE(RADIO_STATE_IDLE);
				break;

			case RADIO_STATE_SCAN:
				radio_engine_mute(OFF);
				radio_ctrl->seek_result = _B7_|_B0_;
				break;

			case RADIO_STATE_AUTO_SAVE:
			#if !defined(_SPECIAL_BROWSE_FUNCTION)
				if(GET_RADIO_AUTOLOC)
				{	//	开LOC扫描
					fm_puts("auto save close loc\n");
					SET_RADIO_AUTOLOC(FALSE);
					setup.loc_flg = FALSE;
					radio_auto_seek(AUTO_SEEK_MODE_UP|AUTO_SEEK_MODE_MIN|AUTO_SEEK_MODE_ROUND);
				}
				else
			#endif
				{	//	关LOC扫描
			#if !defined(_SPECIAL_BROWSE_FUNCTION)
				EXIT_AUTO_SAVE:
			#endif
					fm_puts("auto save finish\n");
				#if defined(_ENABLE_RADIO_RDS_)
					if(rds_auto)
					{
						radio_rds_taxis_freq();
					}
				#endif
					BandIndexReset();
					SET_RADIO_STATE(RADIO_STATE_IDLE);
				#if !defined(_SPECIAL_BROWSE_FUNCTION)
					SET_RADIO_BAND(SearchFristBand());
				#endif
					radio_set_band(GET_RADIO_BAND);
					radio_engine_mute(OFF);

			    #if defined(_SPECIAL_AUTOSEEK_MEMORY_)||defined(_YH1701_AUTOSEEK_MEMORY_)
			    	RadioFreqSort(radio_ctrl->freq1,radio_ctrl->saveindex);
			    	for(i = 0;i < radio_ctrl->saveindex;i++)
			    	{
			    		SET_RADIO_SAVE_FREQ(GET_RADIO_BAND,i,radio_ctrl->freq1[i]);
			    	}
			    #endif

				#if !defined(_CANCEL_TUNER_BROWSE_)
					os_taskq_post_msg(FM_TASK_NAME, 1, MSG_FM_BROWSE);
				#endif
				}
				break;
		}
	}

	radio_disp_polling();
	return stop;
}

static void radio_timer_sto_process(void)
{
	if(RADIO_STATE_FM)
	{
		if(GET_RADIO_ST)
		{
			if(radio_ctrl->st_last != ENABLE)
			{
				if(radio_engine_stereo(ENABLE))
				{
					radio_ctrl->st_last = ENABLE;
				}
			}
			else
			{
			#if !defined(_DISABLE_READ_ST_)
			#if defined(_ENABLE_READ_ST_TIME_)
				if(global.st_time == (30000/RADIO_POLLING_TIME))//30秒进一次
				{
					global.st_time = 0;
					if(radio_engine_get_stereo())
					{
						if(radio_ctrl->eng_status.stereo)
						{
							SET_RADIO_STO(TRUE);
						}
						else
						{
							SET_RADIO_STO(FALSE);
						}
					}
					else
					{
						SET_RADIO_STO(FALSE);
					}
				}
			#else
				if(radio_engine_get_stereo())
				{
					if(radio_ctrl->eng_status.stereo)
					{
						SET_RADIO_STO(TRUE);
					}
					else
					{
						SET_RADIO_STO(FALSE);
					}
				}
				else
				{
					SET_RADIO_STO(FALSE);
				}
			#endif
			#endif
			}
		}
		else
		{
			if(radio_ctrl->st_last != DISABLE)
			{
				if(radio_engine_stereo(DISABLE))
				{
					radio_ctrl->st_last = DISABLE;
				}
			}
			SET_RADIO_STO(FALSE);
		}
	}
	else
	{
		radio_ctrl->st_last = 0xFF;
	}
}

static void radio_timer_scan_process(void)
{
	radio_auto_seek(AUTO_SEEK_MODE_UP|AUTO_SEEK_MODE_CURRENT|AUTO_SEEK_MODE_STATION);
}

static void radio_timer_browse_process(void)
{
#if defined(_ENABLE_RADIO_RDS_)
	memset(radio_rds->rds_psn, 0, 8);
#endif
	radio_ctrl->seek_result = 0;
	if(GET_BAND_FREQ_INDEX < GET_RADIO_FREQ_LENGTH)
	{
		GET_BAND_FREQ_INDEX ++;
		radio_ctrl->seek_soft = TRUE;
		radio_load_broadcast(GET_BAND_FREQ_INDEX);
	}
	else
	{
		SET_BAND_FREQ_INDEX(radio_ctrl->BrowseIndex);

	#if defined(_SPECIAL_BROWSE_FUNCTION)
		radio_set_band(radio_ctrl->BrowseBand);
	#else
		uint8 Band = SearchNextBand(GET_RADIO_BAND);
		if(Band <= GET_RADIO_BAND)
		{
			radio_set_band(radio_ctrl->BrowseBand);
		}
		else
		{
			SET_RADIO_BAND(Band);
			radio_ctrl->BrowseIndex = GET_BAND_FREQ_INDEX;
			SET_BAND_FREQ_INDEX(1);
			radio_ctrl->seek_soft = TRUE;
			radio_set_band(Band);
			SET_RADIO_STATE(RADIO_STATE_BROWSE);
		}
	#endif
	}
}

#if defined(_WJ361_BROWSE_MUTE_)
uint8 browse_flag;
#endif


#if defined(TD_ADD_NEW_APP)
#if defined(_ENABLE_RADIO_RDS_)
extern void Spp_Tx_Radio_Rds(uint8 ta,uint8 af,uint8 pty);

u8 rds_info_compare(void)
{
	u8 mm=0;
	if(rds.rds_af != setup.rds.af)
	{
		rds.rds_af = setup.rds.af;
		mm++;
	}
	if(rds.rds_ta != setup.rds.ta)
	{
		rds.rds_ta = setup.rds.ta;
		mm++;
	}
	if(rds.rds_pty != setup.pty_type)
	{
		rds.rds_pty = setup.pty_type;
		mm++;
	}
	if(rds.rds_pty_type != global.pty_type)
	{
		rds.rds_pty_type = global.pty_type;
		mm++;
	}
	return mm;
}

#endif
#endif

void radio_polling_time(void)
{
#if defined(SUPPORT_ANTENNA_DISPLAY_)
	radio_engine_get_status();
#if defined(__69FM_RSSI_SIGNAL__)//内置收音直接是信号强度db数   20到66db
	setup.rssi = radio_ctrl->eng_status.signal+37;
#else
	setup.rssi = radio_ctrl->eng_status.signal;
#endif
#endif

#if defined(TD_ADD_NEW_APP)
#if defined(_ENABLE_RADIO_RDS_)
	if((rds_info_compare())&&(!global.rds_info))
	{
		global.rds_info = FALSE;
		global.save_rds_af = setup.rds.af;
		global.save_rds_ta = setup.rds.ta;
		global.save_rds_pty = setup.pty_type;
		Spp_Tx_Radio_Rds(setup.rds.ta,setup.rds.af,setup.pty_type);
	}

	if(global.rds_info)
	{
		global.rds_info = FALSE;
	}
#endif
#endif

#if defined(_DISABLE_VOL_SEEK_)
    if((GET_RADIO_STATE == RADIO_STATE_SEEK)||(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE))
    {
    	global.seek_up_flag = TRUE;
    }
    else
    {
    	global.seek_up_flag = FALSE;
    }
#endif

#if defined(_XIMA_SPECIAL_SEEK_)
    if((GET_RADIO_STATE == RADIO_STATE_SEEK)||(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE))
    {
    	global.radio_seek = TRUE;
    }
    else
    {
    	global.radio_seek = FALSE;
    }
#endif

#if defined(_JTI_POWER_LOGO_)
	setup.band_index = GET_RADIO_BAND;
	setup.ch_index = GET_BAND_FREQ_INDEX;
#endif

#if defined(_XZC1668_MODE_FUNTION_)
	if(RADIO_STATE_FM)
	{
		global.fm_am_status = TRUE;
	}
	else
	{
		global.fm_am_status = FALSE;
	}
#endif

#if defined(_DISABLE_FM_ICON_)
	if((GET_RADIO_STATE == RADIO_STATE_IDLE)||(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE))
	{
		global.radio_stast = FALSE;
	}
	else
	{
		global.radio_stast = TRUE;
	}
#endif

#if defined(_WJ361_BROWSE_MUTE_)
    if((global.browse_flag)&&(GET_RADIO_STATE == RADIO_STATE_BROWSE))
    {
		radio_engine_mute(ON);
    	global.browse_flag = FALSE;
    }
	else
	{
		if(!global.aud_mute || (setup.aud_vol != 0))
		{
			if(GET_RADIO_STATE != RADIO_STATE_IDLE)
			{
				radio_engine_mute(OFF);
			}
		}
	}
	
	if(global.browse)
	{
		if(GET_RADIO_STATE == RADIO_STATE_IDLE)
		{
			radio_engine_mute(OFF);
			global.browse = FALSE;
		}
	}
#endif

#if defined(_ENABLE_EW730_AMS_)
	if(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE)
	{
		global.ams_flag = TRUE;
	}
	else
	{
		global.ams_flag = FALSE;
	}
#endif

#if defined(_ENABLE_RADIO_STO_)
	uint8 sto_flag;
	if(GET_RADIO_STATE == RADIO_STATE_IDLE)
	{
		global.sto_flag = TRUE;
	}
	else
	{
		global.sto_flag = FALSE;
	}
#endif

#if defined(ST_READ_DELAY_)
	if(global.st_read_time)
	{
		global.st_read_time--;
		if(!global.st_read_time)
		{
			global.st_read_flag=1;
		}
	}
#endif

#if defined(_ALARM_CLOCK_RING_)
	if(setup.alarm_beep)
	{
		radio_engine_mute(ON);
		setup.alarm_flag = TRUE;
	}
	else
	{
		if(setup.alarm_flag)
		{
			radio_engine_mute(OFF);
			setup.alarm_flag = FALSE;
		}
	}
#endif

#if defined(_ENABLE_BROWSE_CH_)
	if(GET_RADIO_STATE == RADIO_STATE_BROWSE)
	{
		radio_ctrl->disp_bandch_flag=TRUE;
	}
#endif

#if defined(_SUPPORT_AMP_CONTROL_)
	if(GET_RADIO_STATE == RADIO_STATE_SEEK)
	{
		sys_mute_ctrl(ENABLE, 0);//	
		AUDIO_MUTE_ON();
	}
#endif

	if(GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE)
	{
		sys_mute_ctrl(ENABLE, 0);//	
	#if defined(AUDIO_MUTE_SPECIAL_SET)
		HAUDIO_MUTE_ON();
	#endif
	}

	if(GET_RADIO_STATE == RADIO_STATE_SCAN)
	{
		if(!global.scan_flg)
		{
			sys_mute_ctrl(ENABLE, 0);//
			
		#if defined(AUDIO_MUTE_SPECIAL_SET)
			HAUDIO_MUTE_ON();
		#endif
		}
	}

	if(GET_RADIO_STATE == RADIO_STATE_IDLE)
	{
	    if(global.usb_flg)// 修改在收音时插USB会有收音的杂音
	    {
			global.usb_flg = FALSE;
		}
		else
		{
			sys_mute_ctrl(DISABLE, 200);// 如果没有，扫描停下台后会无声
			
		#if defined(AUDIO_MUTE_SPECIAL_SET)
			HAUDIO_MUTE_OFF();
		#endif
		}
	#if defined(_ENABLE_READ_ST_TIME_)
		global.st_time++;
	#endif
		radio_timer_sto_process();
	}
	else if((GET_RADIO_STATE == RADIO_STATE_SCAN)||(GET_RADIO_STATE == RADIO_STATE_BROWSE))
	{
		if(radio_ctrl->seek_wati)
		{
			radio_timer_sto_process();
		}
	#if defined(_ENABLE_READ_ST_TIME_)
		global.st_time = 0;
	#endif
	}
#if defined(_ENABLE_READ_ST_TIME_)
	else
	{
		global.st_time = 0;
	}
#endif

	if(radio_ctrl->manual_flag)
	{
		if(GET_RADIO_STATE != RADIO_STATE_IDLE)
		{
			radio_ctrl->manual_flag = FALSE;
			radio_ctrl->manual_timer =0;
		}
		else
		{
			if(radio_ctrl->manual_timer)
			{
				radio_ctrl->manual_timer--;
			}
			if(!radio_ctrl->manual_timer)
			{
				radio_ctrl->manual_flag = FALSE;
				disp_send_param(DISP_EVENT_AUTO, 0);
			}
		}
	}

	if(radio_ctrl->seek_result & _B7_)
	{
		sys_mute_ctrl(DISABLE, 0);//
		if(radio_ctrl->seek_result & _B0_)
		{
			if(GET_RADIO_STATE == RADIO_STATE_SCAN)
			{
				global.scan_flg = TRUE;
				radio_ctrl->seek_wati = (5000/RADIO_POLLING_TIME);
				radio_flash_enable(10, FALSE, RADIO_PAINT_FREQ);
			}
			else if(GET_RADIO_STATE == RADIO_STATE_BROWSE)
			{
			#if defined(_WJ361_BROWSE_MUTE_)
				global.browse_flag = FALSE;
				global.browse = TRUE;
			#endif
			#if defined(TD_ADD_BANDCH)
				radio_ctrl->disp_bandch_flag=TRUE;
			#endif
				radio_ctrl->seek_wati = (5000/RADIO_POLLING_TIME);
			#if defined(_DISABLE_BROWSE_FLASH_)
			    radio_flash_enable(0, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ);
			#else
				radio_flash_enable(10, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ);
			#endif
			}
		}
		else
		{
			if(GET_RADIO_STATE == RADIO_STATE_BROWSE)
			{
			#if defined(_WJ361_BROWSE_MUTE_)
				global.browse_flag = TRUE;
				global.browse = TRUE;
			#endif
			#if defined(_FY7000_AMS_MUTE_)
				FEIYIN_MUTE_ON();
			#endif
			#if defined(_WLS316_SPECIAL_BROWSE_)
				radio_ctrl->seek_wati = (5000/RADIO_POLLING_TIME);
				radio_flash_enable(10, FALSE, RADIO_PAINT_CHANEL|RADIO_PAINT_FREQ);
			#else
				sys_mute_ctrl(ENABLE, 0);//
				radio_ctrl->seek_wati = (500/RADIO_POLLING_TIME);
			#endif
			}
		}
		radio_ctrl->seek_result = 0;
	}

	if(radio_ctrl->seek_wati)
	{
		radio_ctrl->seek_wati--;
		if(!radio_ctrl->seek_wati)
		{
		#if defined(_ENABLE_BROWSE_MUTE_)
			sys_mute_ctrl(ENABLE, 1000);//
		#else
			sys_mute_ctrl(ENABLE, 500);//
		#endif
			global.scan_flg = FALSE;

			if(GET_RADIO_STATE == RADIO_STATE_SCAN)
			{
				sys_mute_ctrl(ENABLE, 500);//
				radio_flash_disable();
				radio_timer_scan_process();
			}
			else if(GET_RADIO_STATE == RADIO_STATE_BROWSE)
			{
				radio_flash_disable();
				radio_timer_browse_process();
			}
		}
	}

	if(radio_ctrl->flash_counter)
	{
		radio_ctrl->flash_state = !radio_ctrl->flash_state;
		if(radio_ctrl->flash_paint & RADIO_PAINT_FREQ)
		{
			radio_ctrl->paint_flag |= RADIO_PAINT_FREQ;
		}
		if(radio_ctrl->flash_paint & RADIO_PAINT_CHANEL)
		{
			radio_ctrl->paint_flag |= RADIO_PAINT_CHANEL;
		}
    #if defined(_ENABLE_SAVE_FREQ_)
    	if(radio_ctrl->flash_paint & RADIO_PAINT_CH)
    	{
    		radio_ctrl->paint_flag |= RADIO_PAINT_CH;
    	}
    #endif
		if(!radio_ctrl->flash_state)
		{
			radio_ctrl->flash_counter--;
		}
	}
#if defined(_ENABLE_SAVE_FREQ_)
	else
	{
		radio_ctrl->save_flag = FALSE;
		radio_ctrl->flash_flag = FALSE;
	}
#endif
	radio_disp_polling();
}

#if defined(TD_ADD_CHADD_SUB)
void Radio_CH_Add(void)
{
	if(global.SaveRadio)
	{
        if(global.save_radio_ch <6)
            global.save_radio_ch++;
        else
            global.save_radio_ch =1;
		global.SaveRadioTime =50;
	}
	else
	{
        cmd_radio_load_next();
	}
}
void Radio_CH_Sub(void)
{

	if(global.SaveRadio)
	{
        if(global.save_radio_ch >1)
            global.save_radio_ch--;
        else
            global.save_radio_ch =6;
		global.SaveRadioTime =50;
	}
	else
	{
        cmd_radio_load_prev();
	}
}
void Radio_Ch_Save(void)
{
	if(global.SaveRadio)
	{
        cmd_radio_save_number(global.save_radio_ch);
		global.SaveRadioTime =0;
	}
	else
	{
		if((GET_BAND_FREQ_INDEX <1)||(GET_BAND_FREQ_INDEX >6))
		{
			global.save_radio_ch=1;
		}
		else
		{
            global.save_radio_ch=GET_BAND_FREQ_INDEX;
		}
		global.SaveRadio=TRUE;
		global.SaveRadioTime =50;
	}
}
#endif

#ifdef TD_ADD_NEW_APP
void recv_app_freq_pro(u8 freq_h,u8 freq_l)
{
    u16 freq=0;
    freq =freq_h;
    freq <<=8;
    freq |= freq_l;
	if((GET_RADIO_STATE == RADIO_STATE_AUTO_SAVE)||(GET_RADIO_STATE == RADIO_STATE_SEEK)||(GET_RADIO_STATE == RADIO_STATE_SCAN))
	{
		radio_seek_stop();
		SET_RADIO_STATE(RADIO_STATE_IDLE);
	}
	sys_mute_ctrl(ENABLE, 500);
	SET_BAND_TEMP_FREQ(freq);
	radio_set_freq();
	SET_BAND_FREQ_INDEX(0);
	radio_flash_disable();
#if defined(_ENABLE_RADIO_RDS_)
	memset(radio_rds->rds_psn, 0, 8);
#endif
	radio_disp_polling();
}
#endif
#endif

