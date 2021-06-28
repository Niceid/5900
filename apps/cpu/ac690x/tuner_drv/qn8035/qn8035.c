#include "qn8035/qn8035.h"


#if defined(_TUNER_DRIVER_QN8035_)

static qn8035_reg_t *qn8035_reg = NULL;
static qn8035_ctrl_t *qn8035_ctrl = NULL;
static const uint16 fm_qn8035_deal_spot_tab[] = {9600};

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 send_data_to_qn8035(uint8 start_addr, uint8 *buffer, uint8 length)
{	
	uint8 transfer[length + 1];
	transfer[0] = start_addr;
	memcpy(&transfer[1], buffer, length);
	i2c_data_init(QN8035_I2C_ADDR, QN8035_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(transfer, length + 1);
	i2c_data_exit();
	return result;
}

static uint8 read_data_for_qn8035(uint8 start_addr, uint8 *buffer, uint8 length)
{
	i2c_data_init(QN8035_I2C_ADDR, QN8035_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(&start_addr, 1);
	if(result)
	{
		result = i2c_data_r(buffer, length);
	}
	i2c_data_exit();
	return result;
}

static uint8 send_data_to_qn8035_reg(uint8 start_addr, uint8 length)
{
	return send_data_to_qn8035(start_addr, &_au8_(qn8035_reg, start_addr), length);
}

static uint8 read_data_for_qn8035_reg(uint8 start_addr, uint8 length)
{
	return read_data_for_qn8035(start_addr, &_au8_(qn8035_reg, start_addr), length);
}	

static uint8 send_data_to_qn8035_byte(uint8 addr, uint8 data)
{
	return send_data_to_qn8035(addr, &data, 1);
}

static uint8 read_data_for_qn8035_byte(uint8 addr, uint8 *data)
{
	return read_data_for_qn8035(addr, data, 1);
}

static uint8 change_for_qn8035_byte(uint8 reg, uint8 bit_mask, uint8 data) 
{
	uint8 temp;
	
	if(!read_data_for_qn8035_byte(reg, &temp))return FALSE;
	temp &= ((uint8)(~bit_mask));
	temp |= data;
	if(!send_data_to_qn8035_byte(reg, temp))return FALSE;
	
	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 qn8035_set_init(void)
{
	tuner_puts("qn8035_set_init\n");
	qn8035_reg->system_1.swrst = TRUE;
	qn8035_reg->system_1.rx_req = TRUE;
	if(!send_data_to_qn8035_reg(QN8035_REG_SYSTEM1,1))return FALSE;
	
	OSTimeDly(2);
	
	if(!read_data_for_qn8035_reg(QN8035_REG_SYSTEM1, sizeof(qn8035_reg_t)))return FALSE;

	if(qn8035_reg->cid_2.cid_5 != (QN8035_CHIPID_QN8035>>2))return FALSE;

	if(!read_data_for_qn8035_byte(0x58, &qn8035_ctrl->qn8035b_id))return FALSE;
	
	qn8035_ctrl->qn8035b_id &= 0x1F;

	if(qn8035_ctrl->qn8035b_id == 0x13)// 
	{
		if(!send_data_to_qn8035_byte(0x58, qn8035_ctrl->qn8035b_id))return FALSE;
	}
	
	qn8035_reg->cca.xtal_inj = 0; // 0:正弦波输入;1:方波
	if(!send_data_to_qn8035_reg(QN8035_REG_CCA, 1))return FALSE;
	
	//pll_dlt = 28.5Mhz/(XTAL/xtal_div)/512 - 442368
	//根据输入频率XTAL = 32768 Hz 设计
	//xtal_div = 1, pll_dlt = 2945
	_u8_(qn8035_reg->xtal_div_0, 0) = QN8035_XTAL_DIV0;
	_u8_(qn8035_reg->xtal_div_1, 0) = QN8035_XTAL_DIV1;
	_u8_(qn8035_reg->xtal_div_2, 0) = QN8035_XTAL_DIV2;
	if(!send_data_to_qn8035_reg(QN8035_REG_XTAL_DIV0, 3))return FALSE;

	OSTimeDly(10);// 2
	
	if(!send_data_to_qn8035_byte(0x54, 0x47))return FALSE;		//mod PLL setting
	
	if(!send_data_to_qn8035_byte(QN8035_REG_SMP_HLD_THRD,0xC4))return FALSE;
	
	if(!send_data_to_qn8035_byte(0x33, 0x9C))return FALSE;		//mod PLL setting
	
	if(!send_data_to_qn8035_byte(0x2D, 0xD6))return FALSE;		//notch filter threshold adjusting
	
	if(!send_data_to_qn8035_byte(0x43, 0x10))return FALSE;		//notch filter threshold enable 
	
	if(!change_for_qn8035_byte(QN8035_REG_SMSTART,	0x7F, QN8035_SMSTART_VAL))return FALSE;
	if(!change_for_qn8035_byte(QN8035_REG_SNCSTART, 0x7F, QN8035_SNCSTART_VAL))return FALSE;

#if defined(_ENABLE_QN8035_VOLUME_)
	if(!send_data_to_qn8035_byte(0x32, 0x41))return FALSE;
	//if(!send_data_to_qn8035_byte(QN8035_REG_NCCFIR3, 0x1A))return FALSE;
	//if(!change_for_qn8035_byte(QN8035_REG_HCCSTART, 0x7F, 0))return FALSE;
#else
	if(!change_for_qn8035_byte(QN8035_REG_NCCFIR3, 0x70, 0x70))return FALSE;
	if(!change_for_qn8035_byte(QN8035_REG_HCCSTART, 0x7F, QN8035_HCCSTART_VAL))return FALSE;
#endif

	if(qn8035_reg->cid_1.cid_2 == QN8035_CHIPSUBID_A1)
	{
		if(!change_for_qn8035_byte(0x47,0x0c,0x08))return FALSE;
	}
	
	_u8_(qn8035_reg->system_1, 0) = 0x00;
	qn8035_reg->system_1.cca_ch_dis = TRUE;
	qn8035_reg->system_1.rx_req = TRUE;
	if(!send_data_to_qn8035_reg(QN8035_REG_SYSTEM1,1))return FALSE;
	
	OSTimeDly(1);// 1
	
	if(!send_data_to_qn8035_byte(0x4F,					0x00))return FALSE; 			//enable auto tunning in CCA mode
	
	if(!change_for_qn8035_byte(QN8035_REG_GAIN_SEL, 	0x08,	0x08))return FALSE; 	//NFILT program is enabled 
	//selection filter:filter3
	if(!change_for_qn8035_byte(QN8035_REG_CCA1, 		0x30,	0x30))return FALSE;
	//Enable the channel condition filter3 adaptation,Let ccfilter3 adjust freely  
	if(!change_for_qn8035_byte(QN8035_REG_SYSTEM_CTL2,	0x40,	0x00))return FALSE;
	if(!send_data_to_qn8035_byte(QN8035_REG_PLT1,		0x55))return FALSE;  
	if(!send_data_to_qn8035_byte(QN8035_REG_CCA_CNT1,	0x00))return FALSE;
	if(!change_for_qn8035_byte(QN8035_REG_CCA_CNT2, 	0x3F,	0x01))return FALSE; 	//before is 0x03,change in 2012.09.4LX
	//selection the time of CCA FSM wait SNR calculator to settle:20ms
	//0x00: 	20ms(default)
	//0x40: 	40ms
	//0x80: 	60ms
	//0xC0: 	100m
	if(!change_for_qn8035_byte(QN8035_REG_CCA_SNR_TH_1 , 0xC0, 0x00))return FALSE;	
	//selection the time of CCA FSM wait RF front end and AGC to settle:20ms
	//0x00: 	10ms
	//0x40: 	20ms(default)
	//0x80: 	40ms
	//0xC0: 	60ms							
	if(!change_for_qn8035_byte(QN8035_REG_CCA_SNR_TH_2, 0xC0, 0x40))return FALSE;  
	if(!change_for_qn8035_byte(QN8035_REG_CCA,			0x3F, QN8035_RSSI_THERSHOLD))return FALSE;	//setting RSSI threshold for CCA
	if(!change_for_qn8035_byte(QN8035_REG_CCA_SNR_TH_1, 0x3F, QN8035_SNR_THERSHOLD))return FALSE;	//setting SNR threshold for CCA

	if(!send_data_to_qn8035_byte(0x1B,0x00))return FALSE;
	
	if(!change_for_qn8035_byte(0x2C,0x3F,0x12))return FALSE;
	
	if(!change_for_qn8035_byte(0x1D,0x40,0x00))return FALSE;
	
	if(!change_for_qn8035_byte(0x41,0x0F,0x0A))return FALSE;
	
	if(!send_data_to_qn8035_byte(0x45,0x50))return FALSE;
	
	if(!change_for_qn8035_byte(0x3E,0x80,0x80))return FALSE;
	
	if(!change_for_qn8035_byte(0x41,0xE0,0xc0))return FALSE;

#if defined(_RADIO_QN8035_GATE_)
	if(!change_for_qn8035_byte(QN8035_REG_VOL_CTL,0x3F,0x1F))return FALSE;//-3DB
#elif defined(_FY1401_RADIO_QN8035_GATE_)
	if(!change_for_qn8035_byte(QN8035_REG_VOL_CTL,0x3F,0x2F))return FALSE;//-4DB	
#elif defined(_JYL3802E_QN8035_GATE_)
	if(!change_for_qn8035_byte(QN8035_REG_VOL_CTL,0x3F,0x17))return FALSE;//-2DB
#elif defined(_FEIYIN_QN8035_GATE_)
	if(!change_for_qn8035_byte(QN8035_REG_VOL_CTL,0x3F,0x0F))return FALSE;//-1DB
#endif

	if(qn8035_reg->cid_1.cid_2 == QN8035_CHIPSUBID_A0)
	{
		if(!change_for_qn8035_byte(0x42,0x10,0x10))return FALSE;
	}
	
	return TRUE;
}

static uint8 qn8035_set_mute(uint8 mute)
{
	if(mute)
	{
		qn8035_reg->vol_ctl.mute_en = TRUE;
	}
	else
	{
		qn8035_reg->vol_ctl.mute_en = FALSE;
	}
	return send_data_to_qn8035_reg(QN8035_REG_VOL_CTL, 1);
}

static uint8 qn8035_set_stereo(uint8 stereo)
{
	if(stereo)
	{
		qn8035_reg->system_1.force_mo = FALSE;	
	}
	else
	{
		qn8035_reg->system_1.force_mo = TRUE;	
	}
#if defined(_QN8035_MONO_ACC_DELAY_)
	send_data_to_qn8035_reg(QN8035_REG_SYSTEM1, 1);
	//OSTimeDly(1);// 1
#endif
	return send_data_to_qn8035_reg(QN8035_REG_SYSTEM1, 1);
}

static uint8 qn8035_set_imr(uint16 freq)
{
	uint8 imr_flag = FALSE;
	
	if(qn8035_ctrl->qn8035b_id == 0x13)
	{
		if((freq == 9340)||(freq == 9390)||(freq == 9530)||(freq == 9980)||(freq == 10480))
		{
			imr_flag = 1;
		}
	}	
    else if((qn8035_reg->cid_1.cid_2 == QN8035_CHIPSUBID_A0)||(qn8035_reg->cid_1.cid_2 == QN8035_CHIPSUBID_A1))
    {
        if(qn8035_reg->cid_1.cid_2 == QN8035_CHIPSUBID_A1)
		{
			if(freq == 10710)
            {
                imr_flag = 1;
            }
		}		
    }
	
	qn8035_reg->cca.imr = imr_flag;
	if(!send_data_to_qn8035_reg(QN8035_REG_CCA, 1))return FALSE;
	
	return TRUE;
}

static uint8 qn8035_set_scan(uint16 freq) 
{
	freq = (freq-6000)/5;

	qn8035_reg->ch_l 				= _u16to8l_(freq);
	qn8035_reg->ch_step.ch_h 		= _u16to8h_(freq);
	
	qn8035_reg->ch_start_l 			= _u16to8l_(freq);
	qn8035_reg->ch_step.ch_start_h 	= _u16to8h_(freq);
	
	qn8035_reg->ch_stop_l 			= _u16to8l_(freq);
	qn8035_reg->ch_step.ch_stop_h 	= _u16to8h_(freq);

	if(!send_data_to_qn8035_reg(QN8035_REG_CH, 4))return FALSE;

	qn8035_reg->system_1.cca_ch_dis 	= 1;
	if(!send_data_to_qn8035_reg(QN8035_REG_SYSTEM1, 1))return FALSE;
	
	return TRUE;
}

static uint8 qn8035_set_freq(uint16 freq)
{
	uint8 reg;
	
#if defined(_ENABLE_QN8035_SEEK_)
	change_for_qn8035_byte(QN8035_REG_REF,0x0F,0x0A);
#else
	if(!change_for_qn8035_byte(QN8035_REG_REF,0x0F,0x0A))return FALSE;
#endif
	if(!qn8035_set_imr(freq))return FALSE;
	if(!qn8035_set_scan(freq))return FALSE;
	
	qn8035_reg->system_1.cca_ch_dis = 0;
	qn8035_reg->system_1.chsc = 1;
	if(!send_data_to_qn8035_reg(QN8035_REG_SYSTEM1, 1))return FALSE;
	
	if(!send_data_to_qn8035_byte(0x4F,0x80))return FALSE;
	
	if(!read_data_for_qn8035_byte(0x4F, &reg))return FALSE;
	reg >>= 1;
	if(!send_data_to_qn8035_byte(0x4F,reg))return FALSE;
	
	OSTimeDly(1);
	if(!change_for_qn8035_byte(QN8035_REG_REF, 0x0F,0x00))return FALSE;
	
	return TRUE;
}

static uint8 qn8035_check_chsc(uint8 *chsc)
{
	if(!read_data_for_qn8035_reg(QN8035_REG_SYSTEM1, sizeof(qn8035_reg_t)))return FALSE;
	*chsc = !qn8035_reg->system_1.chsc;
	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_qn8035_detect(void);
static uint8 tuner_qn8035_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_qn8035_exit(void);
static uint8 tuner_qn8035_freq(uint16);
static uint8 tuner_qn8035_status(engine_status_t *);
static uint8 tuner_qn8035_search(uint16, engine_status_t *);
static uint8 tuner_qn8035_hard(engine_hard_t *);
static uint8 tuner_qn8035_setting(fm_setting_e, void *);
static uint8 tuner_qn8035_info(fm_info_e, void *);


static uint8 tuner_qn8035_detect(void)
{	
	uint8 result;
	uint8 chip_id;
		
	tuner_printf("tuner_qn8035_detect\n");
	i2c_data_init(QN8035_I2C_ADDR, QN8035_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	if(!result)
	{
		i2c_data_exit();
		return FALSE;
	}
	i2c_data_exit();
	
	result = read_data_for_qn8035_byte(QN8035_REG_CID2, &chip_id);
	if(!result)
	{
		return FALSE;
	}
	
	if((chip_id&0xfc) != QN8035_CHIPID_QN8035)
	{
		result = FALSE;
	}
	
	tuner_printf("tuner_qn8035_detect:%d\n", result);
	return result;
}

static uint8 tuner_qn8035_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	tuner_printf("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);
	
	if(qn8035_reg == NULL)
	{
		qn8035_reg = (qn8035_reg_t *)malloc_fun(qn8035_reg, sizeof(qn8035_reg_t), 0);
		if(qn8035_reg == NULL)return FALSE;
	}
	
	if(qn8035_ctrl == NULL)
	{
		qn8035_ctrl = (qn8035_ctrl_t *)malloc_fun(qn8035_ctrl, sizeof(qn8035_ctrl_t), 0);
		if(qn8035_ctrl == NULL)
		{
			free_fun((void **)&qn8035_reg);
			return FALSE;
		}
	}
	tuner_puts("qn8035_memory_ok\n");
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto qn8035_init_error;
		#if defined(_WLS1403_SPECIAL_FREQ_)
			if(min_freq < 7550)goto qn8035_init_error;
		#else
			if(min_freq < 7600)goto qn8035_init_error;
		#endif
			if(max_freq > 10800)goto qn8035_init_error;
			break;
			
		default:
			goto qn8035_init_error;
			break;
	}
	
	if(qn8035_ctrl->band == band)
	{
		if(qn8035_ctrl->init_succeed)
		{
			if(qn8035_ctrl->now_freq == freq)
			{
				qn8035_ctrl->min_freq		= min_freq;
				qn8035_ctrl->max_freq		= max_freq;
				if(!tuner_qn8035_setting(TUR_SET_MUTE, OFF))goto qn8035_init_error;
				return TRUE;
			}
			else
			{
				goto qn8035_init_freq;
			}
		}
		else 
		{
			goto qn8035_init_error;
		}
	}
	
	if(!qn8035_set_init())goto qn8035_init_error;

qn8035_init_freq:
	qn8035_ctrl->band 			= band;
	qn8035_ctrl->min_freq 		= min_freq;
	qn8035_ctrl->max_freq 		= max_freq;
	qn8035_ctrl->init_succeed 	= TRUE;
	
	engine_status_t status;
	if(!tuner_qn8035_search(freq, &status))goto qn8035_init_error;
#if defined(_ENABLE_RADIO_INIT_)||defined(_ENABLE_RADIO_AREA_INIT_)
	OSTimeDly(10);
	if(!tuner_qn8035_search(freq, &status))goto qn8035_init_error;
#endif
	if(!tuner_qn8035_setting(TUR_SET_MUTE, OFF))goto qn8035_init_error;
	tuner_puts("QN8035 Init Succeed!!!!!\n");
	return TRUE;
	
qn8035_init_error:
	free_fun((void **)&qn8035_reg);
	free_fun((void **)&qn8035_ctrl);
	tuner_puts("QN8035 Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_qn8035_exit(void)
{
	tuner_puts("tuner_qn8035_exit\n");
	
	if(qn8035_reg && qn8035_ctrl)
	{
		qn8035_reg->system_1.standby = TRUE;
		send_data_to_qn8035_reg(QN8035_REG_SYSTEM1,1);
		
		free_fun((void **)&qn8035_reg);
		free_fun((void **)&qn8035_ctrl);
	}
	
	return TRUE;
}

static uint8 tuner_qn8035_freq(uint16 freq)
{
	tuner_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if((qn8035_reg == NULL)||(qn8035_ctrl == NULL))return FALSE;
	if(!qn8035_ctrl->init_succeed)return FALSE;
	if(freq < qn8035_ctrl->min_freq)return FALSE;
	if(freq > qn8035_ctrl->max_freq)return FALSE;	
	
	tuner_puts("tuner_qn8035_freq----succeed111\n");
	uint8 chsc;
	uint8 loop = 50;
    while(TRUE)
	{
	#if defined(_ENABLE_QN8035_SEEK_)
		qn8035_check_chsc(&chsc);
	#else
		if(!qn8035_check_chsc(&chsc))return FALSE;
	#endif
		if(chsc)break;
		OSTimeDly(1);
		if(!(--loop))return FALSE;
	}
	
	tuner_puts("tuner_qn8035_freq----succeed222\n");
	if(!qn8035_set_freq(freq))return FALSE;
	
	qn8035_ctrl->stereo = 0x00;	
	qn8035_ctrl->now_freq = freq;
	tuner_puts("tuner_qn8035_freq----succeed\n");
	return TRUE;
}

static uint8 tuner_qn8035_status(engine_status_t *status)
{
	//tuner_printf("%s\n", __FUNCTION__);
	if((qn8035_reg == NULL)||(qn8035_ctrl == NULL))return FALSE;
	if(!qn8035_ctrl->init_succeed)return FALSE;
	
	uint8 chsc;
	status->freq 	= qn8035_ctrl->now_freq;
	status->signal 	= 0;
	status->valid 	= FALSE;
	status->stereo 	= FALSE;
	status->band 	= RADIO_BAND_FM;
	status->stc 	= FALSE;

#if defined(_ENABLE_QN8035_DELAY_)
	OSTimeDly(1);
#endif
	
	if(!qn8035_check_chsc(&chsc))return FALSE;
	if(!chsc)return TRUE;
	
	if(!read_data_for_qn8035_reg(QN8035_REG_STATUS1, 1))return FALSE;
	if(!read_data_for_qn8035_reg(QN8035_REG_RSSISIG, 1))return FALSE;

	status->stc 	= TRUE;
	status->signal 	= qn8035_reg->rssi;
	
	qn8035_ctrl->stereo <<= 1;
	if(qn8035_reg->snr > QN8035_TUNER_ST_SNR)
	{
		if(!qn8035_reg->status_1.st_mo_rx)
		{
			qn8035_ctrl->stereo |= _B0_;
		}
	}
	if((qn8035_ctrl->stereo & (_B1_|_B0_)) == (_B1_|_B0_))
	{
	#if defined(_ENABLE_ST_LIGHT_)
		if(status->signal >= (QN8035_TUNER_RSSI_FM+7))
	#else
		if(status->signal >= QN8035_TUNER_RSSI_FM)
	#endif
		{
			status->stereo = TRUE;
		}
	}
	
	//ew_printf("status->signal=%d\n",status->signal);
	//ew_printf("rxcca_fail=%d\n",qn8035_reg->status_1.rxcca_fail);
	if(!qn8035_reg->status_1.rxcca_fail)
	{
		status->valid = TRUE;
	}

#if defined(QN8035_TUNER_FREQ_9600_OK)
	if(status->signal>QN8035_TUNER_FREQ_9600_OK)
	{
		if(status->freq==9600)
		status->valid = TRUE;
	}
#endif
	//tuner_printf("----status->valid=%d  status->signal=%d\n", status->valid ,status->signal);
	//tuner_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_qn8035_search(uint16 freq, engine_status_t *status)
{
	tuner_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if((qn8035_reg == NULL)||(qn8035_ctrl == NULL))return FALSE;
	if(!qn8035_ctrl->init_succeed)return FALSE;
	if(freq < qn8035_ctrl->min_freq)return FALSE;
	if(freq > qn8035_ctrl->max_freq)return FALSE;	
	
	if(!tuner_qn8035_freq(freq))return FALSE;
	OSTimeDly(5);
		
	uint8 index;
	for(index = 0; index < 50; index++)
	{
		if(!tuner_qn8035_status(status))return FALSE;
		if(status->stc)break;
		OSTimeDly(1);
	}
	
	tuner_printf("index:%d\n",index);
	tuner_printf("status->stc:%d\n",status->stc);
	if(index == 50)return FALSE;	
	tuner_puts("tuner_qn8035_3333\n");
	
	tuner_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_qn8035_setting(fm_setting_e type, void *set)
{
	tuner_printf("%s, type:%d\n", __FUNCTION__, type);
	if((qn8035_reg == NULL)||(qn8035_ctrl == NULL))return FALSE;
	if(!qn8035_ctrl->init_succeed)return FALSE;

	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!qn8035_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(!qn8035_set_stereo(stereo))return FALSE;
			}
			break;
			
		default:
			return FALSE;
	}
	
	tuner_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_qn8035_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_QN8035;
				
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = QN8035_TUNER_RSSI_FM;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = QN8035_TUNER_RSSI_LOC_FM;
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_qn8035_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_qn8035_deal_spot_tab;
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

const tuner_drv_fun_t tuner_qn8035_drv_fun =
{
	tuner_qn8035_detect,
	tuner_qn8035_init,
	tuner_qn8035_exit,
	tuner_qn8035_freq,
	tuner_qn8035_status,
	tuner_qn8035_search,
	NULL,
	tuner_qn8035_setting,
	tuner_qn8035_info,
};

#endif

