#include "si470x/si470x.h"
#include "rds/rds_dec.h"

#if defined(_TUNER_DRIVER_SI470x_)

static si470x_ctrl_t *si470xCtrl = NULL;
static const uint16 fm_si470x_deal_spot_tab[] = {9600};


static uint8 tuner_si470x_detect(void);
static uint8 tuner_si470x_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_si470x_exit(void);
static uint8 tuner_si470x_freq(uint16);
static uint8 tuner_si470x_status(engine_status_t *);
static uint8 tuner_si470x_search(uint16, engine_status_t *);
static uint8 tuner_si470x_hard(engine_hard_t *);
static uint8 tuner_si470x_setting(fm_setting_e, void *);
static uint8 tuner_si470x_info(fm_info_e, void *);

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

static uint8 send_data_to_si470x(uint8 length)
{
	i2c_data_init(SI470x_I2C_ADDR, SI470x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_ww((uint16 *)&si470xCtrl->order.PowerCfg, length);
	i2c_data_exit();
	return result;
}

static uint8 read_data_for_si470x(uint8 length)
{
	i2c_data_init(SI470x_I2C_ADDR, SI470x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_wr((uint16 *)&si470xCtrl->order.StatusRssi, length);
	i2c_data_exit();
	return result;
}

static void si470x_reg_Init(void)
{
	si470xCtrl->order.PowerCfg.Enable = 1;
	si470xCtrl->order.PowerCfg.Disable = 0;
	si470xCtrl->order.PowerCfg.Seek = 0;
	si470xCtrl->order.PowerCfg.SeekUp = 0;
	si470xCtrl->order.PowerCfg.SkMode = 0;
#if defined(_ENABLE_RADIO_RDS_)
	si470xCtrl->order.PowerCfg.RdsM = 1;
#else
	si470xCtrl->order.PowerCfg.RdsM = 0;
#endif
	si470xCtrl->order.PowerCfg.Mono = 0;
	si470xCtrl->order.PowerCfg.DMute = 0;
	si470xCtrl->order.PowerCfg.DSMute = 0;
/*-----------------------------------------------------------------------------*/	
	si470xCtrl->order.Channel.Chan = 0;
	si470xCtrl->order.Channel.Tune = 0;
/*-----------------------------------------------------------------------------*/
	si470xCtrl->order.SysConfig1.Gpio1 = 0;
	si470xCtrl->order.SysConfig1.Gpio2 = 0;
	si470xCtrl->order.SysConfig1.Gpio3 = 0;
#if defined(_SI470x_TUNER_C19_ST_MODIFY_)
	si470xCtrl->order.SysConfig1.BlndAdj = 0;
#else
	si470xCtrl->order.SysConfig1.BlndAdj = 2;
#endif
	si470xCtrl->order.SysConfig1.Agc = 0;
	si470xCtrl->order.SysConfig1.De = 0;
#if defined(_ENABLE_RADIO_RDS_)
	si470xCtrl->order.SysConfig1.Rds = 1;
#else
	si470xCtrl->order.SysConfig1.Rds = 0;
#endif
	si470xCtrl->order.SysConfig1.StcIen = 0;
	si470xCtrl->order.SysConfig1.RdsIen = 0;
/*-----------------------------------------------------------------------------*/
	si470xCtrl->order.SysConfig2.Volume = 15;
#if defined(_USA_STEP_200K_)&&0
	si470xCtrl->order.SysConfig2.Space	= SI470x_TUNE_AUTO_SEEK_200KHZ;
#else
	si470xCtrl->order.SysConfig2.Space 	= SI470x_TUNE_AUTO_SEEK_50KHZ;
#endif
	si470xCtrl->order.SysConfig2.Band	= SI470x_TUNER_BAND_USA;
	si470xCtrl->order.SysConfig2.SeekTh = SI470x_TUNE_AUTO_SEEK_RSSI;
/*-----------------------------------------------------------------------------*/
	si470xCtrl->order.SysConfig3.SkCnt 	= SI470x_TUNE_AUTO_SEEK_CNT;
	si470xCtrl->order.SysConfig3.SkSnr 	= SI470x_TUNE_AUTO_SEEK_SNR;
	si470xCtrl->order.SysConfig3.VolExt = 0;
	si470xCtrl->order.SysConfig3.SMuteA = 0;
	si470xCtrl->order.SysConfig3.SMuteR = 0;	
}

static uint8 si470x_set_power(uint8 power)
{
	uint8 result = FALSE;
	
	if(power)
	{
		si470x_reg_Init();
		result = send_data_to_si470x(5);
	}
	else
	{
		memset(&si470xCtrl->order, 0, sizeof(si470xCtrl->order));
		si470xCtrl->order.Test1.XOscEen = TRUE;
		si470xCtrl->order.Test1.Rev 	= 0x100;
		result = send_data_to_si470x(6);
	}
	
	return result;
}

#if defined(_SI470x_TUNER_C19_ST_MODIFY_)

/*-------------------------------------------------------------------------------------------------------------------------*/
static const uint16 CMD_SI470x_VERIFY_COMMAND[]	= {0x0000,0x0000,0x0000,	0x00FF};
static const uint16 CMD_SI470x_SET_STERO_RSSI[]	= {SI470x_TUNER_RSSI_ST,  	0x0000,0x0300,0x0007};
static const uint16 CMD_SI470x_SET_MONO_RSSI[]	= {SI470x_TUNER_RSSI_MONO,	0x0000,0x0301,0x0007};
static const uint16 CMD_SI470x_GET_STERO_RSSI[]	= {0x0000,0x0000,0x0300,	0x0008};
static const uint16 CMD_SI470x_GET_MONO_RSSI[]	= {0x0000,0x0000,0x0301,	0x0008};
/*-------------------------------------------------------------------------------------------------------------------------*/

static const uint32 CMD_SI470x_TAB[]=
{
	(uint32)CMD_SI470x_VERIFY_COMMAND,
	(uint32)CMD_SI470x_SET_STERO_RSSI,
	(uint32)CMD_SI470x_SET_MONO_RSSI,
	(uint32)CMD_SI470x_GET_STERO_RSSI,
	(uint32)CMD_SI470x_GET_MONO_RSSI,
};

#define _SI470x_TUNER_GET_PRO_ERRO_				(10)

static uint8 si470x_cmd_support(void)
{
	read_data_for_si470x(8);
	
	if(si470xCtrl->order.ChipId.Rev == 0x04)//RevC
	{
		return TRUE;
	}
	return FALSE;
}

static uint8 si470x_cmd_enable(void)
{
	uint8 result = TRUE;
#if defined(_ENABLE_RADIO_RDS_)
	si470xCtrl->order.SysConfig1.Rds = 0;
	result = send_data_to_si470x(3);
#endif
	return result;
}

static uint8 si470x_cmd_disable(void)
{
	uint8 result = TRUE;
#if defined(_ENABLE_RADIO_RDS_)
	si470xCtrl->order.SysConfig1.Rds = 1;
	result = send_data_to_si470x(3);
#endif
	return result;
}

static void si470x_order_copy(si470x_order_e order)
{
	uint16 const *tab_buf;	
	
	if(order < SI470x_ORDER_END)
	{
		tab_buf = (uint16 const *)CMD_SI470x_TAB[order];
		
		si470xCtrl->order.RdsA = *tab_buf++;
		si470xCtrl->order.RdsB = *tab_buf++;
		si470xCtrl->order.RdsC = *tab_buf++;
		si470xCtrl->order.RdsD = *tab_buf;
	}
}

static uint8 si470x_send_data(void)
{
	if(send_data_to_si470x(14))
	{
		OSTimeDly(1);
		return TRUE;
	}
	return FALSE;
}

static uint8 si470x_send_order(si470x_order_e order)
{
	if(order < SI470x_ORDER_END)
	{
		si470x_order_copy(order);
		return si470x_send_data();
	}
	return FALSE;
}

static uint8 si470x_get_property(si470x_order_e order)
{
	uint8 i;

	if(order < SI470x_ORDER_END)
	{
		if(si470x_send_order(order))
		{
			for(i = 0; i < _SI470x_TUNER_GET_PRO_ERRO_; i++)
			{
				if(read_data_for_si470x(6))
				{
					if(_u16to8l_(si470xCtrl->order.RdsD) == 0x00)
					{
						return TRUE;
					}
				}
				else break;
			}
		}
	}
	return FALSE;
}

static uint8 si470x_set_property(si470x_order_e set_order, si470x_order_e get_order, uint16 check)
{
	if(si470x_send_order(set_order))	
	{
		if(si470x_get_property(get_order))
		{
			if(check == si470xCtrl->order.RdsA)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

static uint8 si470x_set_cmd(void)
{
	uint8 result = FALSE;

	if(!si470x_cmd_support())return TRUE;
	
	if(!si470x_cmd_enable())goto tuner_set_return;

	if(!si470x_get_property(SI470x_VERIFY_COMMAND))goto tuner_set_return;

	if(!si470x_set_property(SI470x_SET_STERO_RSSI, SI470x_GET_STERO_RSSI, SI470x_TUNER_RSSI_ST))goto tuner_set_return;
	
	if(!si470x_set_property(SI470x_SET_MONO_RSSI,  SI470x_GET_MONO_RSSI,  SI470x_TUNER_RSSI_MONO))goto tuner_set_return;
	
	result = TRUE;
	
tuner_set_return:
	
	if(!si470x_cmd_disable())
	{
		result = FALSE;
	}
	
	OSTimeDly(10);
	
	return result;
}

#endif

static uint8 si470x_set_init(void)
{
	if(!si470x_set_power(FALSE))return FALSE;
	OSTimeDly(50);
	
	if(!si470x_set_power(TRUE))return FALSE;
	OSTimeDly(50);
	
#if defined(_SI470x_TUNER_C19_ST_MODIFY_)
	if(!si470x_set_cmd())return FALSE;
#endif
	return TRUE;
}

static uint8 si470x_set_mute(uint8 mute)
{
	if(mute)
	{
		si470xCtrl->order.PowerCfg.DMute = FALSE;
	}
	else
	{
		si470xCtrl->order.PowerCfg.DMute = TRUE;	
	}
	return send_data_to_si470x(1);
}

static uint8 si470x_set_stereo(uint8 stereo)
{
	if(stereo)
	{
		si470xCtrl->order.PowerCfg.Mono = FALSE;	
	}
	else
	{
		si470xCtrl->order.PowerCfg.Mono = TRUE;	
	}
	return send_data_to_si470x(1);
}

static uint8 si470x_set_freq(uint16 freq, uint8 length, uint8 wait_stc)
{
	uint8 	index;
	uint16 	step;
	
	//tuner_printf("si470x_set_freq:%d, wait_stc:%d\n", freq, wait_stc);
	
	if(si470xCtrl->order.Channel.Tune)
	{
		si470xCtrl->order.Channel.Tune = FALSE;
	
		if(!send_data_to_si470x(2))return FALSE;
		
		OSTimeDly(5);// add
	}
	
	if(si470xCtrl->order.SysConfig2.Space == SI470x_TUNE_AUTO_SEEK_200KHZ)
	{
		 step = 20;
	}
	else if(si470xCtrl->order.SysConfig2.Space == SI470x_TUNE_AUTO_SEEK_100KHZ)
	{
		step = 10;
	}
	else
	{
		step = 5;
	}	
	
    si470xCtrl->order.Channel.Chan = (freq - 8750)/step;

	si470xCtrl->order.Channel.Tune = TRUE;	
	
	if(!send_data_to_si470x(length))return FALSE;	
	
	si470xCtrl->now_freq = freq;
	
	if(!wait_stc)
	{
		return TRUE;
	}
	
	OSTimeDly(3);
	
	for(index = 0; index < 50; index++)
	{
		if(!read_data_for_si470x(1))return FALSE;
		
		if(si470xCtrl->order.StatusRssi.Stc)
		{
			break;
		}		
		
		OSTimeDly(1);
	}
	
	if(index == 50)return FALSE;	
	
	OSTimeDly(1);
	
	return TRUE;
}

static uint8 si470x_set_autoseek_start(uint8 dir)
{	
	tuner_puts("si470x_set_autoseek_start\n");
	si470xCtrl->order.PowerCfg.SkMode 	= 0;
	
	si470xCtrl->order.Channel.Tune 		= FALSE;
	
	si470xCtrl->order.SysConfig2.SeekTh = SI470x_TUNE_AUTO_SEEK_RSSI;
	
	si470xCtrl->order.SysConfig3.SkSnr 	= SI470x_TUNE_AUTO_SEEK_SNR;
	
	si470xCtrl->order.SysConfig3.SkCnt 	= SI470x_TUNE_AUTO_SEEK_CNT;
	
	if(dir)
	{
		si470xCtrl->order.PowerCfg.SeekUp = TRUE;	
	}
	else
	{
		si470xCtrl->order.PowerCfg.SeekUp = FALSE;	
	}
	
	si470xCtrl->order.PowerCfg.Seek = ENABLE;
	
	if(!send_data_to_si470x(5))return FALSE;
	
	si470xCtrl->TunerAutoSeek = TRUE;
	
	return TRUE;
}

static uint8 si470x_set_autoseek_stop(void)
{	
	tuner_puts("si470x_set_autoseek_stop\n");
	si470xCtrl->order.PowerCfg.Seek = DISABLE;
	
	return send_data_to_si470x(1);
}

static uint8 si470x_reset_autoseek(void)
{
	if(si470xCtrl->TunerAutoSeek)
	{
		tuner_puts("si470x_reset_autoseek\n");
		
		if(!si470x_set_autoseek_stop())return FALSE;
		
		OSTimeDly(5);
		
		engine_status_t status;
		if(!tuner_si470x_status(&status))return FALSE;
		
		si470xCtrl->order.Channel.Tune = FALSE;
		
		si470xCtrl->TunerAutoSeek = FALSE;
	}
	
	return TRUE;
}

#if defined(_ENABLE_RADIO_RDS_)

static void si470x_rds_reset(void)
{
	if(si470xCtrl->tuner_rds.enable)
	{
		rds_dec_reset();
		si470xCtrl->tuner_rds.discard = 1;
		si470xCtrl->tuner_rds.quality = 0;
	}
}

static void si470x_rds_decode(void)
{
	if(si470xCtrl->tuner_rds.quality >= si470xCtrl->tuner_rds.factor)
	{
		si470xCtrl->tuner_rds.quality -= si470xCtrl->tuner_rds.factor;
	}
	
	rds_dec_timer();
	
	if(!read_data_for_si470x(1))return;
	if((!si470xCtrl->order.StatusRssi.Stc)
	 ||(!si470xCtrl->order.StatusRssi.Rdss)
	 ||(!si470xCtrl->order.StatusRssi.Rdsr))return;
	
	if(!read_data_for_si470x(6))return;	
			
	if((si470xCtrl->order.StatusRssi.BlerA == 0)
	 &&(si470xCtrl->order.ReadChan.BlerB == 0)
	 &&(si470xCtrl->order.ReadChan.BlerC == 0)
	 &&(si470xCtrl->order.ReadChan.BlerD == 0))
	{
		rds_input_t input;
				
		input.block_ah = _u16to8h_(si470xCtrl->order.RdsA);
		input.block_al = _u16to8l_(si470xCtrl->order.RdsA);
		input.block_bh = _u16to8h_(si470xCtrl->order.RdsB);
		input.block_bl = _u16to8l_(si470xCtrl->order.RdsB);
		input.block_ch = _u16to8h_(si470xCtrl->order.RdsC);
		input.block_cl = _u16to8l_(si470xCtrl->order.RdsC);
		input.block_dh = _u16to8h_(si470xCtrl->order.RdsD);
		input.block_dl = _u16to8l_(si470xCtrl->order.RdsD);
		
		si470xCtrl->tuner_rds.quality = 0xFF;
		
		rds_stream_dec(&input);
	}
	/*
	else 
	{		
		if(si470xCtrl->order.StatusRssi.BlerA == 0)
		{
			tuner_puts("si470x_rds_only_block_a\n");
			si470xCtrl->tuner_rds.quality = 0xFF;
			rds_block_a_dec(_u16to8h_(si470xCtrl->order.RdsA), _u16to8l_(si470xCtrl->order.RdsA));
		}
		
		if(si470xCtrl->order.ReadChan.BlerB == 0)
		{
			tuner_puts("si470x_rds_only_block_b\n");
			si470xCtrl->tuner_rds.quality = 0xFF;
			rds_block_b_dec(_u16to8h_(si470xCtrl->order.RdsB), _u16to8l_(si470xCtrl->order.RdsB));
		}
		
		if(si470xCtrl->order.ReadChan.BlerC == 0)
		{
			tuner_puts("si470x_rds_only_block_c\n");
			si470xCtrl->tuner_rds.quality = 0xFF;
			rds_block_c_dec(_u16to8h_(si470xCtrl->order.RdsC), _u16to8l_(si470xCtrl->order.RdsC));
		}
		
		if(si470xCtrl->order.ReadChan.BlerD == 0)
		{
			tuner_puts("si470x_rds_only_block_d\n");
			si470xCtrl->tuner_rds.quality = 0xFF;
			rds_block_d_dec(_u16to8h_(si470xCtrl->order.RdsD), _u16to8l_(si470xCtrl->order.RdsD));
		}
	}*/	
}

#endif

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 tuner_si470x_detect(void)
{
	uint8 result;
	uint16 buffer[7];
	
	tuner_puts("tuner_si470x_detect\n");
	
	i2c_data_init(SI470x_I2C_ADDR, SI470x_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	if(!result)
	{
		i2c_data_exit();
		return FALSE;
	}
	
	result = i2c_data_wr(buffer, 7);
	if(!result)
	{
		i2c_data_exit();
		return FALSE;
	}
	
	if(_u16_(buffer, 6) != 0x1242)
	{
		result = FALSE;
	}
	
	i2c_data_exit();
	
	tuner_printf("tuner_si470x_detect:%d\n", result);
	return result;
}

static uint8 tuner_si470x_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	if(si470xCtrl == NULL)
	{
		si470xCtrl = (si470x_ctrl_t *)malloc_fun(si470xCtrl, sizeof(si470x_ctrl_t), 0);
		if(si470xCtrl == NULL)
		{
			return FALSE;
		}
	}

	tuner_puts("si470x_memory_ok\n");
	tuner_printf("band:%d, min_freq:%d, max_freq:%d, freq:%d\n", band, min_freq, max_freq, freq);
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto si470x_init_error;
			if(min_freq < 8750)goto si470x_init_error;
			if(max_freq > 10800)goto si470x_init_error;
			break;
			
		default:
			goto si470x_init_error;
			break;
	}
	
	if(si470xCtrl->tuner_band == band)
	{
		if(si470xCtrl->init_succeed)
		{
			if(si470xCtrl->now_freq == freq)
			{
				si470xCtrl->min_freq = min_freq;
				si470xCtrl->max_freq = max_freq;
				if(!tuner_si470x_setting(TUR_SET_MUTE, OFF))goto si470x_init_error;
				tuner_puts("SI470x Init2 Succeed!!!!!\n");
				return TRUE;
			}
			else
			{
				goto si470x_init_freq;
			}
		}
		else 
		{
			goto si470x_init_error;
		}
	}
	
	if(!si470x_set_init())goto si470x_init_error;

si470x_init_freq:
	si470xCtrl->tuner_band = band;
	si470xCtrl->min_freq = min_freq;
	si470xCtrl->max_freq = max_freq;
	si470xCtrl->init_succeed = TRUE;
	si470xCtrl->TunerAutoSeek = FALSE;
	
	engine_status_t status;
	if(!tuner_si470x_search(freq, &status))goto si470x_init_error;
	if(!tuner_si470x_setting(TUR_SET_MUTE, OFF))goto si470x_init_error;	
	tuner_puts("SI470x Init Succeed!!!!!\n");
	return TRUE;
	
si470x_init_error:
	free_fun((void **)&si470xCtrl);
	tuner_puts("SI470x Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_si470x_exit(void)
{
	if(si470xCtrl)
	{
		si470x_reset_autoseek();		
		//sys_mute_ctrl(ENABLE, 300);// 退出收音有杂音
		si470x_set_power(FALSE);
		free_fun((void **)&si470xCtrl);
	}
	
	return TRUE;
}

static uint8 tuner_si470x_freq(uint16 freq)
{
	tuner_printf("tuner_si470x_freq:%d\n", freq);
	
	if(si470xCtrl == NULL)return FALSE;
	if(!si470xCtrl->init_succeed)return FALSE;
	if(freq < si470xCtrl->min_freq)return FALSE;
	if(freq > si470xCtrl->max_freq)return FALSE;
	
	if(!si470x_reset_autoseek())return FALSE;
			
	if(!si470x_set_freq(freq, 2, FALSE))return FALSE;
	
#if defined(_ENABLE_RADIO_RDS_)
	si470x_rds_reset();
#endif

	return TRUE;
}

static uint8 tuner_si470x_status(engine_status_t *status)
{		
	if(si470xCtrl == NULL)return FALSE;
	if(!si470xCtrl->init_succeed)return FALSE;
	
	if(si470xCtrl->TunerAutoSeek)
	{
		uint16 freq;
		uint16 step;
		
	#if defined(_USA_STEP_200K_)
		OSTimeDly(5);
	#endif
	
		if(!read_data_for_si470x(2))return FALSE;
				
		freq = si470xCtrl->order.ReadChan.ReadChan;
		
		if(si470xCtrl->order.SysConfig2.Space == SI470x_TUNE_AUTO_SEEK_200KHZ)
		{
			 step = 20;
		}
		else if(si470xCtrl->order.SysConfig2.Space == SI470x_TUNE_AUTO_SEEK_100KHZ)
		{
			step = 10;
		}
		else
		{
			step = 5;
		}
		
		freq *= step;
		
		freq += 8750;
		
		si470xCtrl->now_freq = freq;
		
		//seek complete
		if(si470xCtrl->order.StatusRssi.Stc || si470xCtrl->order.StatusRssi.Sf)
		{
			status->stc = TRUE;
		}
		else
		{
			status->stc = FALSE;
		}		
	}
	else
	{		
		if(!read_data_for_si470x(1))return FALSE;
		
		//seek complete
		if(si470xCtrl->order.StatusRssi.Stc)
		{
			status->stc = TRUE;
		}
		else
		{
			status->stc = FALSE;
		}
	}
		
	//stereo
	if(si470xCtrl->order.StatusRssi.St)
	{
		status->stereo = TRUE;
	}
	else
	{
		status->stereo = FALSE;
	}	

	//intensity
	status->signal = si470xCtrl->order.StatusRssi.Rssi;

	//当前频率
	status->freq = si470xCtrl->now_freq;

	//当前波段
	status->band = RADIO_BAND_FM;
	
	//是否有无电台
	status->valid = FALSE;
	if(!si470xCtrl->order.StatusRssi.Afc)
	{
		if(status->signal > SI470x_TUNER_RSSI_FM)
		{
			status->valid = TRUE;
		}
	}
	
	//tuner_printf("tuner_si470x_status--->stc:%d ,valid:%d\n", status->stc, status->valid);
	return TRUE;
}

static uint8 tuner_si470x_search(uint16 freq, engine_status_t *status)
{
	tuner_puts("tuner_si470x_search\n");
	if(si470xCtrl == NULL)return FALSE;
	if(!si470xCtrl->init_succeed)return FALSE;
	if(freq < si470xCtrl->min_freq)return FALSE;
	if(freq > si470xCtrl->max_freq)return FALSE;
	
	if(!si470x_reset_autoseek())return FALSE;
	
	if(!si470x_set_freq(freq, 2, TRUE))return FALSE;
	
	if(!tuner_si470x_status(status))return FALSE;
	
	return TRUE;
}

static uint8 tuner_si470x_hard(engine_hard_t *hard)
{
	tuner_puts("tuner_si470x_hard\n");
	if(si470xCtrl == NULL)return FALSE;
	if(!si470xCtrl->init_succeed)return FALSE;
	
	if(hard->enable)
	{
		engine_hard_start_t *start = &hard->ctrl.start;
		si470x_tuner_span_e span;
		
		if(start->min_freq < si470xCtrl->min_freq)return FALSE;
		if(start->max_freq > si470xCtrl->max_freq)return FALSE;	
		if(start->start_freq < start->min_freq)return FALSE;
		if(start->start_freq > start->max_freq)return FALSE;		
		
		if(!si470x_reset_autoseek())return FALSE;
		
		if(start->step == 20)
		{
			 span = SI470x_TUNE_AUTO_SEEK_200KHZ;
		}
		else if(start->step == 10)
		{
			span = SI470x_TUNE_AUTO_SEEK_100KHZ;
		}
		else
		{
			span = SI470x_TUNE_AUTO_SEEK_50KHZ;
		}
		
		if(si470xCtrl->order.SysConfig2.Space != span)
		{
			si470xCtrl->order.SysConfig2.Space = span;
			
			if(!si470x_set_freq(start->start_freq, 4, TRUE))return FALSE;
		}
		else
		{
			if(start->start_freq != si470xCtrl->now_freq)
			{
				if(!si470x_set_freq(start->start_freq, 2, TRUE))return FALSE;
			}
		}
		
		if(!si470x_set_autoseek_start(start->dir))return FALSE;
		tuner_puts("tuner_si470x_hard--->start succeed\n");
	}
	else
	{
		engine_hard_stop_t *stop = &hard->ctrl.stop;
		
		if(stop->stop_freq == 0xFFFF)
		{
			if(!si470x_reset_autoseek())return FALSE;
			
			if(si470xCtrl->now_freq < si470xCtrl->min_freq)
			{
				if(!tuner_si470x_search(si470xCtrl->min_freq, stop->status))return FALSE;
			}
			else if(si470xCtrl->now_freq > si470xCtrl->max_freq)
			{
				if(!tuner_si470x_search(si470xCtrl->max_freq, stop->status))return FALSE;
			}
			else
			{
				if(!tuner_si470x_search(si470xCtrl->now_freq, stop->status))return FALSE;
			}
		}
		else
		{
			if(stop->stop_freq < si470xCtrl->min_freq)return FALSE;
			if(stop->stop_freq > si470xCtrl->max_freq)return FALSE;	
			
			if(!si470x_reset_autoseek())return FALSE;
			if(!tuner_si470x_search(stop->stop_freq, stop->status))return FALSE;
		}
		tuner_puts("tuner_si470x_hard--->stop succeed\n");
	}
	
	return TRUE;
}

static uint8 tuner_si470x_setting(fm_setting_e type, void *set)
{
	//tuner_puts("tuner_si470x_setting\n");
	if(si470xCtrl == NULL)return FALSE;
	if(!si470xCtrl->init_succeed)return FALSE;
	
	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				tuner_printf("tuner_si470x_setting mute:%d\n", mute);
				if(!si470x_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(!si470x_set_stereo(stereo))return FALSE;
			}
			break;

	#if defined(_ENABLE_RADIO_RDS_)
		case TUR_SET_RDS:
			{
				typedef struct{
					uint8 enable;
					uint16 timer;
					uint8 result;
				}tuner_rds_ctrl_t;
				tuner_rds_ctrl_t *rds_ctrl = ((tuner_rds_ctrl_t *)set);
				
				tuner_printf("rds enable:%d, timer:%d\n", rds_ctrl->enable, rds_ctrl->timer);
				
				if(rds_ctrl->enable)
				{
					if((si470xCtrl->min_freq >= 8750) && (si470xCtrl->max_freq <= 10800))
					{
						if(!si470xCtrl->tuner_rds.enable)
						{
							int check = FALSE;
							if(!tuner_si470x_info(TUR_INFO_RDS, &check))return FALSE;
							if(check)
							{
								si470xCtrl->tuner_rds.factor = 255/(_RDS_QUALITY_TIME_/rds_ctrl->timer);
								si470xCtrl->tuner_rds.enable = TRUE;
								si470x_rds_reset();
								rds_dec_det_time(rds_ctrl->timer);
								rds_ctrl->result = TRUE;
							}
						}
					}
					else return FALSE;
				}
				else
				{
					if(si470xCtrl->tuner_rds.enable)
					{
						si470xCtrl->tuner_rds.enable = FALSE;
						rds_ctrl->result = TRUE;
					}
					else return FALSE;
				}
			}
			break;

		case TUR_SET_RDS_RESET:
			{
				if(si470xCtrl->tuner_rds.enable)
				{
					si470x_rds_reset();
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_DECODE:
			{
				if(si470xCtrl->tuner_rds.enable)
				{
					si470x_rds_decode();
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_EON:
			{
				if(si470xCtrl->tuner_rds.enable)
				{
					uint8 disable = (uint8)set;
					rds_dec_eon(disable);
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_SAVE:
			if(set != NULL)
			{
				if(si470xCtrl->tuner_rds.enable)
				{
					radio_rds_station_t *write_station = (radio_rds_station_t *)set;
					rds_dec_save(write_station);
				}
				else return FALSE;
			}
			else return FALSE;
			break;
			
	#endif
	
		default:
			return FALSE;
	}
	
	return TRUE;
}

static uint8 tuner_si470x_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_SI470x;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI470x_TUNER_RSSI_FM;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI470x_TUNER_RSSI_LOC_FM;
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_si470x_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_si470x_deal_spot_tab;
			}
			break;
			
		case TUR_INFO_AUTOSEEK:
			if(info != NULL)
			{
				*(uint8 *)info = ENABLE; //DISABLE;//
			}
			break;
			
		case TUR_INFO_FM_ONLY:
			if(info != NULL)
			{
				*(uint8 *)info = TRUE;
			}
			break;
			
	#if defined(_ENABLE_RADIO_RDS_)
		case TUR_INFO_RDS:
			if(info != NULL)
			{
				*(uint8 *)info = TRUE;
			}
			break;
			
		case TUR_INFO_RDS_INFO:
			if(info != NULL)
			{
				if(si470xCtrl == NULL)return FALSE;
				if(!si470xCtrl->init_succeed)return FALSE;
				if(!si470xCtrl->tuner_rds.enable)return FALSE;
				
				radio_rds_info_t *rds_info = (radio_rds_info_t *)info;
				rds_dec_info(rds_info);
			}
			break;
			
		case TUR_INFO_RDS_APP:
			if(info != NULL)
			{
				if(si470xCtrl == NULL)return FALSE;
				if(!si470xCtrl->init_succeed)return FALSE;
				if(!si470xCtrl->tuner_rds.enable)return FALSE;
				
				radio_rds_app_t *rds_app = (radio_rds_app_t *)info;
				engine_status_t status;
				if(!tuner_si470x_status(&status))return FALSE;
				rds_app->freq	= status.freq;
				rds_app->level	= status.signal;
				rds_app->stc	= status.stc;
				rds_app->found	= status.valid;
				rds_app->signal = si470xCtrl->tuner_rds.quality;
				rds_dec_app(rds_app);
			}
			break;

		case TUR_INFO_RDS_LOAD:
			if(info != NULL)
			{
				if(si470xCtrl == NULL)return FALSE;
				if(!si470xCtrl->init_succeed)return FALSE;
				if(!si470xCtrl->tuner_rds.enable)return FALSE;
				
				radio_rds_station_t *read_station = (radio_rds_station_t *)info;
				rds_dec_load(read_station);
			}
			break;
	#endif
			
		default:
			return FALSE;
	}
	
	return TRUE;
}


const tuner_drv_fun_t tuner_si470x_drv_fun =
{
	tuner_si470x_detect,
	tuner_si470x_init,
	tuner_si470x_exit,
	tuner_si470x_freq,
	tuner_si470x_status,
	tuner_si470x_search,
	tuner_si470x_hard,
	tuner_si470x_setting,
	tuner_si470x_info,
};

#endif

