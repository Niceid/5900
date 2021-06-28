#include "ew730/ew730.h"

#if defined(_TUNER_DRIVER_EW730_)

static ew730_ctrl_t *ew730_ctrl = NULL;
static const uint16 fm_ew730_deal_spot_tab[] = {9600};
extern volatile u32 g_rand_seed;

static const uint16 am_ew730_deal_spot_tab[] = {};

static const  uint8 FM_VOL_CODE[17]=
			{0, 6,10,15,17,18,19,20,21,22,23,24,25,26,27,28,29};
static const  uint8 AM_VOL_CODE[17]=
			{0, 6,10,14,16,18,20,21,22,23,24,25,26,27,28,29,30};

static uint8 channel_flag = 0;
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 send_ew730_reg(uint8 reg_addr,uint16 data)
{	
	uint8 transfer[3];
			
	transfer[0] = reg_addr;
	transfer[1] = _u16to8h_(data);
	transfer[2] = _u16to8l_(data);

	i2c_data_init(EW730_I2C_ADDR, EW730_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(transfer, sizeof(transfer));
	i2c_data_exit();
//	ew_printf("send_ew730_reg--result:%d\n",result);
	return result;
}

static uint8 read_ew730_reg(uint8 reg_addr, uint16 *data)
{
	uint8 result;
	uint8 read_byte[2];
	
	i2c_data_init(EW730_I2C_ADDR, EW730_I2C_RATE, &radio_gpio);
	/*if(result = i2c_data_w_r(&reg_addr, 1, read_byte, 2))
	{
		*data = _u8tou16_(read_byte[0], read_byte[1]);
	}*/
	result = i2c_data_w_r(&reg_addr, 1, read_byte, 2);
	if(result)
	{
		*data = _u8tou16_(read_byte[0], read_byte[1]);
	}
	i2c_data_exit();
	return result;
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_RADIO_RDS_)
static void ew730_rds_reset(void)
{
	if(ew730_ctrl->tuner_rds.enable)
	{
		rds_dec_reset();
		ew730_ctrl->tuner_rds.discard = 1;
		ew730_ctrl->tuner_rds.quality = 0;
	}
}

static void ew730_rds_decode(void)
{
	if(!ew730_ctrl->tuner_rds.enable)return;	
	if(ew730_ctrl->tuner_rds.quality >= ew730_ctrl->tuner_rds.factor)
	{
		ew730_ctrl->tuner_rds.quality -= ew730_ctrl->tuner_rds.factor;
	}
	rds_dec_timer();
	uint16 r4_read,r7_read,ra_read,rb_read;
	if(!read_ew730_reg(0x4,&r4_read))return;
	if(!read_ew730_reg(0xa,&ra_read))return;
	if(!read_ew730_reg(0xb,&rb_read))return;

	//__DEBUG_EW730("r1_read:%x, r4_read:%x, ra_read:%x, rb_read:%x\n",  r1_read, r4_read, ra_read, rb_read);
	//__DEBUG_EW730("1 r4_read :%x\n",  r4_read);
	//__DEBUG_EW730("1 ra_read :%x\n",  ra_read);
	//__DEBUG_EW730("1 rb_read :%x\n",  rb_read);

	if(!send_ew730_reg(0x04, r4_read|0x1000))return;

	//if(!read_ew730_reg(0x4,&r4_read))return;
	//__DEBUG_EW730("2 r4_read :%x\n",  r4_read);
	//__DEBUG_EW730("2 ra_read :%x\n",  ra_read);
	//__DEBUG_EW730("2 rb_read :%x\n",  rb_read);

	if(!read_ew730_reg(0x7,&r7_read))return;
	//__DEBUG_EW730("2 r7_read :%x\n",  r7_read);

	//__DEBUG_EW730("2 ra_read :%x\n",  ra_read);
	///*
	if((ra_read&0x8000)==0)
	{
		//__DEBUG_EW730("retrun ray\n");
		return;
	}
	if((ra_read&0x0800)==0)
	{
		//__DEBUG_EW730("retrun syn\n");
		return;
	}
	
	uint16 rc_read,rd_read,re_read,rf_read;	
	ew730_ctrl->tuner_rds.ble_a = (rb_read>>14);
	ew730_ctrl->tuner_rds.ble_b = (rb_read>>12)&0x3;
	ew730_ctrl->tuner_rds.ble_c = (rb_read>>10)&0x3;
	ew730_ctrl->tuner_rds.ble_d = (rb_read>>8)&0x3;
	
	if(!read_ew730_reg(0xc,&rc_read))return;
	if(!read_ew730_reg(0xd,&rd_read))return;
	if(!read_ew730_reg(0xe,&re_read))return;
	if(!read_ew730_reg(0xf,&rf_read))return;

	if(	(ew730_ctrl->tuner_rds.ble_a == 0)
	 && (ew730_ctrl->tuner_rds.ble_b == 0)
	 && (ew730_ctrl->tuner_rds.ble_c == 0)
	 && (ew730_ctrl->tuner_rds.ble_d == 0))
	{
		rds_input_t input;
		
		input.block_ah = _u16to8h_(rc_read);
		input.block_al = _u16to8l_(rc_read);
		input.block_bh = _u16to8h_(rd_read);
		input.block_bl = _u16to8l_(rd_read);
		input.block_ch = _u16to8h_(re_read);
		input.block_cl = _u16to8l_(re_read);
		input.block_dh = _u16to8h_(rf_read);
		input.block_dl = _u16to8l_(rf_read);
		
		ew730_ctrl->tuner_rds.quality = 0xFF;
		
		//__DEBUG_EW730("enter--->rds_stream_dec\n");
		
		rds_stream_dec(&input);
	}
	else
	{
		if(ew730_ctrl->tuner_rds.ble_a == 0)
		{
			//__DEBUG_EW730("enter--->ew730_ctrl->tuner_rds.block_b ==%d \n",ew730_ctrl->tuner_rds.block_b);
			ew730_ctrl->tuner_rds.quality = 0xFF;
			rds_block_a_dec(_u16to8h_(rc_read), _u16to8l_(rc_read));
		}
		
		if(ew730_ctrl->tuner_rds.ble_b == 0)
		{
			//__DEBUG_EW730("enter--->rds_stream_block_b\n");
			ew730_ctrl->tuner_rds.quality = 0xFF;
			rds_block_b_dec(_u16to8h_(rd_read), _u16to8l_(rd_read));
		}
		
		if(ew730_ctrl->tuner_rds.ble_c == 0)
		{
			//__DEBUG_EW730("enter--->rds_stream_block_c\n");
			ew730_ctrl->tuner_rds.quality = 0xFF;
			rds_block_c_dec(_u16to8h_(re_read), _u16to8l_(re_read));
		}
		
		if(ew730_ctrl->tuner_rds.ble_d == 0)
		{
			//__DEBUG_EW730("enter--->rds_stream_block_d\n");
			ew730_ctrl->tuner_rds.quality = 0xFF;
			rds_block_d_dec(_u16to8h_(rf_read), _u16to8l_(rf_read));
		}
	}
	//*/
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
#if defined(EW730_ALWAYS_IN_STATUS)
static uint8 ew730_set_vol(uint8 flag)
{
	uint16 voltemp = 0;
	read_ew730_reg(0x12,&voltemp);
	voltemp &= ~EW730_LVOLUME;
	voltemp &= ~EW730_RVOLUME;
	if(flag)
	{
		if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
		{
			voltemp |= ((0x3 << 10) |(FM_VOL_CODE[16] << 5) | FM_VOL_CODE[16]);
		}
		else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
		{
			voltemp |= ((0x3 << 10) |(AM_VOL_CODE[16] << 5) | AM_VOL_CODE[16]);
		}
	}
	else
	{
		if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
		{
			voltemp |= ((0x3 << 10) |(FM_VOL_CODE[8] << 5) | FM_VOL_CODE[8]);
		}
		else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
		{
			voltemp |= ((0x3 << 10) |(AM_VOL_CODE[5] << 5) | AM_VOL_CODE[5]);
		}
	}

	ew_printf("%s, voltemp:%d \n", __FUNCTION__, voltemp);

	if(!send_ew730_reg(0x12,voltemp))return FALSE;	
	return TRUE;
}
#endif
uint8 EW730_SET_VOL(uint8 VOL)
{
	if(VOL>31)
	{
		VOL=31;
	}
	uint16 voltemp = 0;
	read_ew730_reg(0x12,&voltemp);
	voltemp &= ~EW730_LVOLUME;
	voltemp &= ~EW730_RVOLUME;
	
	voltemp |= ((0x3 << 10) |(VOL<< 5) | VOL);
	
	ew_printf("%s, voltemp:%d \n", __FUNCTION__, voltemp);
	if(!send_ew730_reg(0x12,voltemp))return FALSE;	
	return TRUE;
}
static uint8 ew730_set_mute(uint8 mute)
{
	uint16 voltemp = 0;
	read_ew730_reg(0x12,&voltemp);
	voltemp &= ~EW730_LVOLUME;
	voltemp &= ~EW730_RVOLUME;
	if(mute)
    { 
		voltemp &= ~(0x3 << 10);
    }
    else
    { 
	#if defined(EW730_ALWAYS_IN_STATUS)
		if(channel_flag)
		{
			if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
			{
				voltemp |= ((0x3 << 10) |(FM_VOL_CODE[16] << 5) | FM_VOL_CODE[16]);
			}
			else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
			{
				voltemp |= ((0x3 << 10) |(AM_VOL_CODE[16] << 5) | AM_VOL_CODE[16]);
			}
		}
		else
		{
			if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
			{
				voltemp |= ((0x3 << 10) |(FM_VOL_CODE[8] << 5) | FM_VOL_CODE[8]);
			}
			else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
			{
				voltemp |= ((0x3 << 10) |(AM_VOL_CODE[5] << 5) | AM_VOL_CODE[5]);
			}
		}
	#else
		if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
		{
			voltemp |= ((0x3 << 10) |(FM_VOL_CODE[16] << 5) | FM_VOL_CODE[16]);
		}
		else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
		{
			voltemp |= ((0x3 << 10) |(AM_VOL_CODE[16] << 5) | AM_VOL_CODE[16]);
		}
	#endif
    }
	if(!send_ew730_reg(0x12,voltemp))return FALSE;	
	return TRUE;
}
static uint8 ew730_set_stereo(uint8 stereo)
{
	uint16 st_v;
		
	if(stereo)
    {
        st_v = FALSE;
    }
    else
    {
        st_v = TRUE;
    }
	if(!read_ew730_reg(EW730_REG_02H, &(ew730_ctrl->reg.reg_02h)))return FALSE;
	ew730_ctrl->reg.reg_02h &= ~(0x01 << 13);
	ew730_ctrl->reg.reg_02h |= (st_v << 13);
	if(!send_ew730_reg(EW730_REG_02H, ew730_ctrl->reg.reg_02h))return FALSE;
	return TRUE;
}


static uint8 ew730_set_init(void)
{
	ew_puts("ew730_set_init\n");

	//uint16 	temp_read;
	
	//reset fm module
	if(!send_ew730_reg(0x00,0x16AA ))return FALSE;
    OSTimeDly(5);
	if(!send_ew730_reg(0x00,0x96AA))return FALSE;
    OSTimeDly(5);
#if defined(_FY700B_EW730_ADJ_SEARCH_)
	if(!send_ew730_reg(0x07,0xD200))return FALSE;	
#else
	if(!send_ew730_reg(0x07,0xf000))return FALSE;
#endif
    OSTimeDly(5);

	if(!send_ew730_reg(0x05,0x0211))return FALSE;//set rssi 
	if(!send_ew730_reg(0x11,0x057F))return FALSE;//set rssi 

	if(!send_ew730_reg(0xA0,0x85A9))return FALSE;
	//if(!send_ew730_reg(0x02,0x6000))return FALSE;
	
#if defined(_FY700B_EW730_ADJ_SEARCH_)
	if(!send_ew730_reg(0x53,0x2E2E))return FALSE; // add 2019.04.25
#endif

#if defined(_ENABLE_ST_LIGHT_)
	if(!send_ew730_reg(0x02,0xC200))return FALSE;// ÐÞ¸ÄSTµãµÆ
#else
	if(!send_ew730_reg(0x02,0xC000))return FALSE;
#endif

	
	ew_puts("ew730_set_init--runing\n");

//	if(!read_ew730_reg(0x26,&temp_read))return FALSE;
//	temp_read |= 0x8000;
//	if(!send_ew730_reg(0x26,temp_read))return FALSE;
//	if(!send_ew730_reg(0x26,temp_read))return FALSE;
    OSTimeDly(1);

	if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
	{
		if(!send_ew730_reg(0x13,0x0101))return FALSE;
	}
	else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
	{
		if(!send_ew730_reg(0x13,0x4101))return FALSE;
	}
#if defined(_ENABLE_FREQ_32768HZ_)
	if(!send_ew730_reg(0x06,0x8000|EW730_32_768K))return FALSE;
#else
	if(!send_ew730_reg(0x06,0x8000|EW730_24M))return FALSE;
#endif
    OSTimeDly(30);
	ew_puts("ew730_set_init--OK\n");
	
	return TRUE;
	
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_ew730_detect(void);
static uint8 tuner_ew730_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_ew730_exit(void);
static uint8 tuner_ew730_freq(uint16);
static uint8 tuner_ew730_status(engine_status_t *);
static uint8 tuner_ew730_search(uint16, engine_status_t *);
static uint8 tuner_ew730_setting(fm_setting_e, void *);
static uint8 tuner_ew730_info(fm_info_e, void *);


static uint8 tuner_ew730_detect(void)
{	
	uint8 result;
	
	i2c_data_init(EW730_I2C_ADDR, EW730_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	i2c_data_exit();
	
	ew_printf("tuner_ew730_detect:%d\n", result);
	return result;
}

static uint8 tuner_ew730_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{

	ew_printf("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);

	if(ew730_ctrl == NULL)
	{
		ew730_ctrl = (ew730_ctrl_t *)malloc_fun(ew730_ctrl, sizeof(ew730_ctrl_t), 0);
		if(ew730_ctrl == NULL)return FALSE;
	}
	
	ew_puts("ew730_memory_ok\n");
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto ew730_init_error;
			if(min_freq < 7600)goto ew730_init_error;
			if(max_freq > 10800)goto ew730_init_error;
			ew730_ctrl->TunerBand = EW730_BAND_REC_FM;
			break;
		case RADIO_BAND_AM:
			if(min_freq > max_freq)goto ew730_init_error;
			if(min_freq < 520)goto ew730_init_error;
			if(max_freq > 1710)goto ew730_init_error;
			ew730_ctrl->TunerBand = EW730_BAND_REC_AM;
			break;
		default:
			goto ew730_init_error;
			break;
	}
		
	if(ew730_ctrl->tuner_band == band)
	{
		if(ew730_ctrl->init_succeed)
		{
			if(ew730_ctrl->now_freq == freq)
			{
				ew730_ctrl->min_freq = min_freq;
				ew730_ctrl->max_freq = max_freq;
				if(!tuner_ew730_setting(TUR_SET_MUTE, OFF))goto ew730_init_error;
				return TRUE;
			}
			else
			{
				goto ew730_init_freq;
			}
		}
		else 
		{
			goto ew730_init_error;
		}
	}
	
	if(!ew730_set_init())goto ew730_init_error;
#if defined(_SPECIAL_EW730_INIT_)
	OSTimeDly(5);
	if(!ew730_set_init())goto ew730_init_error;
#endif
ew730_init_freq:
	ew730_ctrl->tuner_band 	= band;
	ew730_ctrl->min_freq 		= min_freq;
	ew730_ctrl->max_freq 		= max_freq;
	ew730_ctrl->init_succeed 	= TRUE;
	
	engine_status_t status;
	if(!tuner_ew730_search(freq, &status))goto ew730_init_error;
	OSTimeDly(10);
	if(!tuner_ew730_search(freq, &status))goto ew730_init_error;
	if(!tuner_ew730_setting(TUR_SET_MUTE, OFF))goto ew730_init_error;
	ew_puts("EW730 Init Succeed!!!!!\n");
	return TRUE;
	
ew730_init_error:
	free_fun((void **)&ew730_ctrl);
	ew_puts("EW730 Init Fail!!!!!\n");
	
	return FALSE;
}

static uint8 tuner_ew730_power_off(void)
{
	send_ew730_reg(0x06,0x0000);
	send_ew730_reg(0x00,0x16AA);
	return TRUE;
}

static uint8 tuner_ew730_exit(void)
{
	ew_puts("tuner_ew730_exit\n");
	
	if(ew730_ctrl)
	{
		tuner_ew730_power_off();
		free_fun((void **)&ew730_ctrl);
	}
	return TRUE;
}

static uint8 tuner_ew730_freq(uint16 freq)
{
	ew_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if(ew730_ctrl == NULL)return FALSE;
	if(!ew730_ctrl->init_succeed)return FALSE;
	if(freq < ew730_ctrl->min_freq)return FALSE;
	if(freq > ew730_ctrl->max_freq)return FALSE;
	
	if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
	{
		if(!send_ew730_reg(0x03, 0x0000))return FALSE;
    	OSTimeDly(2);
		if(!send_ew730_reg(0x03, EW730_TUNE|freq))return FALSE;
	}
	else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
	{
		if(!send_ew730_reg(0x03, 0x0000))return FALSE;
    	OSTimeDly(2);
		if(!send_ew730_reg(0x03, EW730_TUNE|freq))return FALSE;
 	}
	
#if defined(_ENABLE_RADIO_RDS_)
	ew730_rds_reset();
#endif

	ew730_ctrl->now_freq = freq;
	ew_puts("tuner_ew730_freq----succeed\n");
	return TRUE;
}

static uint8 tuner_ew730_status(engine_status_t *status)
{
	//ew_printf("%s\n", __FUNCTION__);
	if(ew730_ctrl == NULL)return FALSE;
	if(!ew730_ctrl->init_succeed)return FALSE;
	
#if defined(_ENABLE_RADIO_MUTE_)
	if(compare_task_name("DABTask"))
	{
		ew730_set_mute(TRUE);
	}
	else
	{
		ew730_set_mute(FALSE);
	}
#endif

	status->stc 	= FALSE;
	status->stereo 	= FALSE;
	status->signal 	= 0;
	status->valid	= FALSE;
	status->freq 	= ew730_ctrl->now_freq;
	
	if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
	{
		status->band = RADIO_BAND_FM;
	#if defined(_ENABLE_EW730_AM_)
		OSTimeDly(7);
	#endif
	}
	else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
	{
		status->band = RADIO_BAND_AM;
	#if defined(_ENABLE_EW730_AM_)
		OSTimeDly(10);
	#endif
	}
	
#if defined(_ENABLE_EW730_DELAY_TIME_)
    OSTimeDly(10);
#elif defined(_ENABLE_EW730_DELAY_)
#if !defined(_ENABLE_EW730_AM_)
    OSTimeDly(5);
#endif
#endif

	status->stc = TRUE;

	//ew_printf("status->freq:%d\n", status->freq);

#if defined(_FY700B_EW730_ADJ_SEARCH_)
	uint16 r8_read,rb_read,snr,rssi,ra_read,st;
	uint16 rd_snr,rd_rssi,rd_st;
	uint8 index;
	uint8 rd_index;	
	
	rd_snr = 0 ;
	rd_rssi = 0 ;
	rd_st = 0 ;
	rd_index = 0 ;
	
	for(index = 0; (index < 10) && (status->valid == 0); index++)
	{
		if(!read_ew730_reg(0x8,&r8_read))return FALSE;
		if(!read_ew730_reg(0xa,&ra_read))return FALSE;
		if(!read_ew730_reg(0xb,&rb_read))return FALSE;
		rd_snr += (r8_read & 0x0020);
		rd_rssi += (rb_read & 0x00ff);
		rd_st	+= ((ra_read & 0x0400)>>8);
		rd_index++;	

		//ew_printf("read ...snr:0x%x--read.....-rssi:%d\n", (r8_read ),(rb_read & 0x00ff));	
	}
	
	if(rd_index)
	{
		snr  = (rd_snr  /rd_index );
		rssi = (rd_rssi / rd_index);
		st 	 = (rd_st / rd_index);
	}
	else
	{
		return FALSE;	
	}
	
	status->signal = rssi;
	status->valid = FALSE;
	status->stereo= FALSE;

	//ew_printf("snr:%d---rssi:%d\n", snr,rssi);
#if defined(EW730_ALWAYS_IN_STATUS)
	channel_flag = FALSE;
#endif
	if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
	{
		if((snr==0)&&(rssi>=EW730_TUNER_RSSI_FM)) //(rssi>=28) && (snr>=4) && (0==afc)
		{
		#if defined(_ENABLE_EW730_AMS_)
    		if(global.ams_flag)
    		{
    			if((status->freq%10) == 5)
    			{
    				if(status->freq == 9215)
    				{
						if(rssi>33)
						{
							status->valid = TRUE;
						}
    				}
    				else if(status->freq == 8775)
    				{
						if(rssi>=51)
						{
							status->valid = TRUE;
						}
    				}
    				else
    				{
    					if(rssi>=(EW730_TUNER_RSSI_FM+7))
    					{
    						status->valid = TRUE;
    					}
    				}
    			}
    			else
    			{
    				if(status->freq == 9600)
    				{
    					if(rssi>42)
    					{
    						status->valid = TRUE;
    					}
    				}
    				else if(status->freq == 9610)
    				{
    					if(rssi>40)
    					{
    						status->valid = TRUE;
    					}
    				}
    				else if((status->freq >= 10400)&&(status->freq <= 10520))
    				{
    					if(rssi>36)
    					{
    						status->valid = TRUE;
    					}
    				}
    				else
    				{
    					if(rssi>=(EW730_TUNER_RSSI_FM+7))
    					{
    						status->valid = TRUE;
    					}
    				}
    			}
    		}
    		else
    		{
    			if((status->freq%10) == 5)
    			{
    				if(rssi>=(EW730_TUNER_RSSI_FM+4))
    				{
						if(status->freq == 9215)
						{
							if(rssi>=33)
							{
								status->valid = TRUE;
							}
						}
						else if(status->freq == 8775)
						{
							if(rssi>=50)
							{
								status->valid = TRUE;
							}
						}
						else
						{
							status->valid = TRUE;
						}
    				}
    			}
    			else
    			{
    				if(status->freq == 9600)
    				{
    					if(rssi>42)
    					{
    						status->valid = TRUE;
    					}
    				}
    				else if(status->freq == 9610)
    				{
    					if(rssi>40)
    					{
    						status->valid = TRUE;
    					}
    				}
    				else
    				{
    					status->valid = TRUE;
    				}
    			}
    		}
		#else
			status->valid = TRUE;
		#endif
		
		#if defined(EW730_ALWAYS_IN_STATUS)
			channel_flag = TRUE;
		#endif
		#if defined(_FY700B_EW730_ST_)
			if(!setup.mono_flg)
		#else
			if(st)
		#endif
			{
				status->stereo = TRUE;
			}
		}
	}
	else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
	{
	#if defined(_ENABLE_EW730_AM_)
		if(status->freq > 990)
		{
			if((snr == 0)&&(rssi>=(EW730_TUNER_RSSI_AM-7)))
			{
				status->valid = TRUE;
			}
		}
		else
		{
			if((snr == 0)&&(rssi>=EW730_TUNER_RSSI_AM))
			{
				status->valid = TRUE;
			}
		}
	#else
		if((snr == 0)&&(rssi>=EW730_TUNER_RSSI_AM))
		{
			status->valid = TRUE;
		#if defined(EW730_ALWAYS_IN_STATUS)
			channel_flag = TRUE;
		#endif
		}
	#endif
	}

#else
	uint16 r8_read,rb_read,snr,rssi,ra_read,st;
	uint8 index;
	for(index = 0; (index < 4) && (status->valid == 0); index++)
	{
		if(!read_ew730_reg(0x8,&r8_read))return FALSE;
		if(!read_ew730_reg(0xa,&ra_read))return FALSE;
		if(!read_ew730_reg(0xb,&rb_read))return FALSE;
		snr 	= r8_read & 0x0020;
		rssi 	= rb_read & 0x00ff;
		st 		= (ra_read & 0x0400)>>8;
		status->signal = rssi;
		status->valid = FALSE;
		status->stereo= FALSE;

		//ew_printf("snr:%d---rssi:%d\n", snr,rssi);
	#if defined(EW730_ALWAYS_IN_STATUS)
		channel_flag = FALSE;
	#endif
		if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
		{
			if((snr==0)&&(rssi>=EW730_TUNER_RSSI_FM)) //(rssi>=28) && (snr>=4) && (0==afc)
			{
				status->valid = TRUE;
			#if defined(EW730_ALWAYS_IN_STATUS)
				channel_flag = TRUE;
			#endif
				if(st)
				{
					status->stereo = TRUE;
				}
			}

		}
		else if(ew730_ctrl->TunerBand == EW730_BAND_REC_AM)
		{
			if((snr == 0)&&(rssi>=EW730_TUNER_RSSI_AM))
			{
				status->valid = TRUE;
			#if defined(EW730_ALWAYS_IN_STATUS)
				channel_flag = TRUE;
			#endif

			}
		}
	}
#endif

#if defined(EW730_ALWAYS_IN_STATUS)
	ew730_set_vol(channel_flag);
#endif
	
	//ew_printf("%s, snr:%d---rssi:%d\n", __FUNCTION__, snr,rssi);
	return TRUE;
}

static uint8 tuner_ew730_search(uint16 freq, engine_status_t *status)
{
	ew_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if(ew730_ctrl == NULL)return FALSE;
	if(!ew730_ctrl->init_succeed)return FALSE;
	if(freq < ew730_ctrl->min_freq)return FALSE;
	if(freq > ew730_ctrl->max_freq)return FALSE;
	
	if(!tuner_ew730_freq(freq))return FALSE;
	
	uint8 index;
	for(index = 0; index < 50; index++)
	{
		if(!tuner_ew730_status(status))return FALSE;
		if(status->stc)
		{
			break;
		}
		OSTimeDly(1);
	}
	if(index == 50)return FALSE;
		
	ew_printf("%s---->succeed\n", __FUNCTION__);
	
	return TRUE;
}

static uint8 tuner_ew730_setting(fm_setting_e type, void *set)
{
	//ew_printf("%s, type:%d\n", __FUNCTION__, type);
	if(ew730_ctrl == NULL)return FALSE;
	if(!ew730_ctrl->init_succeed)return FALSE;
	
	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!ew730_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
				{
					if(!ew730_set_stereo(stereo))return FALSE;
				}
				else return FALSE;
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
				
				if(rds_ctrl->enable)
				{
					if(ew730_ctrl->TunerBand == EW730_BAND_REC_FM)
					{
						if((ew730_ctrl->min_freq >= 8750) && (ew730_ctrl->max_freq <= 10800))
						{
							if(!ew730_ctrl->tuner_rds.enable)
							{
								int check = FALSE;
								if(!tuner_ew730_info(TUR_INFO_RDS, &check))return FALSE;
								
								if(check)
								{
									ew730_ctrl->tuner_rds.factor = 255/(_RDS_QUALITY_TIME_/rds_ctrl->timer);
									ew730_ctrl->tuner_rds.enable = TRUE;
									ew730_rds_reset();
									rds_dec_det_time(rds_ctrl->timer);
									rds_ctrl->result = TRUE;
								}
							}
						}
						else return FALSE;
					}
					else return FALSE;
				}
				else
				{
					if(ew730_ctrl->tuner_rds.enable)
					{
						ew730_ctrl->tuner_rds.enable = FALSE;
						rds_ctrl->result = TRUE;
					}
					else return FALSE;
				}
			}
			break;
		
		case TUR_SET_RDS_RESET:
			{
				if(ew730_ctrl->tuner_rds.enable)
				{
					ew730_rds_reset();
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_DECODE:
			{
				if(ew730_ctrl->tuner_rds.enable)
				{
					ew730_rds_decode();
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_EON:
			{
				if(ew730_ctrl->tuner_rds.enable)
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
				if(ew730_ctrl->tuner_rds.enable)
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
	//ew_printf("%s---->succeed\n", __FUNCTION__);
	
	return TRUE;
}
static uint8 tuner_ew730_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_EW730;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = EW730_TUNER_RSSI_FM;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = EW730_TUNER_RSSI_LOC_FM;
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_ew730_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_ew730_deal_spot_tab;
			}
			break;
			
		case TUR_INFO_AM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = EW730_TUNER_RSSI_AM;
			}
			break;
			
		case TUR_INFO_AM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = EW730_TUNER_RSSI_LOC_AM;
			}
			break;
		
		case TUR_INFO_AM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(am_ew730_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_AM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)am_ew730_deal_spot_tab;
			}
			break;
			
		case TUR_INFO_AUTOSEEK:
			if(info != NULL)
			{
				*(uint8 *)info = FALSE;
			}
			break;
			
    #if defined(_DISABLE_EW730_AM_)
    	case TUR_INFO_FM_ONLY:
    		if(info != NULL)
    		{
    			*(uint8 *)info = TRUE;
    		}
    		break;
    #endif
			
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
				if(ew730_ctrl == NULL)return FALSE;
				if(!ew730_ctrl->init_succeed)return FALSE;
				if(!ew730_ctrl->tuner_rds.enable)return FALSE;
				
				radio_rds_info_t *rds_info = (radio_rds_info_t *)info;
				rds_dec_info(rds_info);
			}
			break;
			
		case TUR_INFO_RDS_APP:
			if(info != NULL)
			{
				if(ew730_ctrl == NULL)return FALSE;
				if(!ew730_ctrl->init_succeed)return FALSE;
				if(!ew730_ctrl->tuner_rds.enable)return FALSE;
				
				radio_rds_app_t *rds_app = (radio_rds_app_t *)info;
				engine_status_t status;
				if(!tuner_ew730_status(&status))return FALSE;
				rds_app->freq	= status.freq;
				rds_app->level	= status.signal;
				rds_app->stc	= status.stc;
				rds_app->found	= status.valid;
				rds_app->signal = ew730_ctrl->tuner_rds.quality;
				rds_dec_app(rds_app);
			}
			break;
			
		case TUR_INFO_RDS_LOAD:
			if(info != NULL)
			{
				if(ew730_ctrl == NULL)return FALSE;
				if(!ew730_ctrl->init_succeed)return FALSE;
				if(!ew730_ctrl->tuner_rds.enable)return FALSE;
				
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

const tuner_drv_fun_t tuner_ew730_drv_fun =
{
	tuner_ew730_detect,
	tuner_ew730_init,
	tuner_ew730_exit,
	tuner_ew730_freq,
	tuner_ew730_status,
	tuner_ew730_search,
	NULL,
	tuner_ew730_setting,
	tuner_ew730_info,
};

#endif

