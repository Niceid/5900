#include "qn8065/qn8065.h"
#include "disp.h"
#include "setup.h"

#if defined(_TUNER_DRIVER_QN8065_)

static qn8065_reg_t *qn8065_reg = NULL;
static qn8065_ctrl_t *qn8065_ctrl = NULL;
static const uint16 fm_qn8065_deal_spot_tab[] = {9600};

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 send_data_to_qn8065(uint8 start_addr, uint8 *buffer, uint8 length)
{
	uint8 transfer[length + 1];
	transfer[0] = start_addr;
	memcpy(&transfer[1], buffer, length);
	i2c_data_init(QN8065_I2C_ADDR, QN8065_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(transfer, length + 1);
	i2c_data_exit();
	return result;
}

static uint8 read_data_for_qn8065(uint8 start_addr, uint8 *buffer, uint8 length)
{
	i2c_data_init(QN8065_I2C_ADDR, QN8065_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(&start_addr, 1);
	if(result)
	{
		result = i2c_data_r(buffer, length);
	}
	i2c_data_exit();
	return result;
}

static uint8 send_data_to_qn8065_reg(uint8 start_addr, uint8 length)
{
	return send_data_to_qn8065(start_addr, &_au8_(qn8065_reg, start_addr), length);
}

static uint8 read_data_for_qn8065_reg(uint8 start_addr, uint8 length)
{
	return read_data_for_qn8065(start_addr, &_au8_(qn8065_reg, start_addr), length);
}

static uint8 send_data_to_qn8065_byte(uint8 addr, uint8 data)
{
	return send_data_to_qn8065(addr, &data, 1);
}

static uint8 read_data_for_qn8065_byte(uint8 addr, uint8 *data)
{
	return read_data_for_qn8065(addr, data, 1);
}

static uint8 change_for_qn8065_byte(uint8 reg, uint8 bit_mask, uint8 data)
{
	uint8 temp;

	if(!read_data_for_qn8065_byte(reg, &temp))return FALSE;
	temp &= ((uint8)(~bit_mask));
	temp |= data;
	if(!send_data_to_qn8065_byte(reg, temp))return FALSE;

	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

uint16 qn8065_get_frq()
{
    uint8 tCh;
    uint8  tStep;
    uint16 ch = 0;
    // set to reg: CH_STEP
    //send_data_to_qn8065_byte(0x00,0x11);
    change_for_qn8065_byte(QN8065_REG_SYSTEM1,0x11,0x11);
    read_data_for_qn8065_byte(QN8065_REG_CH_STEP,&tStep);
    tStep &= QN8065_CH;
    ch  =  tStep ;
    read_data_for_qn8065_byte(QN8065_REG_CH,&tCh);
    ch = (ch<<8)+tCh;
    return CHREG2FREQ(ch);
}


static uint8 qn8065_set_mute(uint8 mute)
{
	return send_data_to_qn8065_byte(QN8065_REG_REF, mute?0x14:0x10);
}

static uint8 qn8065_set_init(void)
{
	if(!send_data_to_qn8065_byte(QN8065_REG_SYSTEM1,0x81)) return FALSE;
	OSTimeDly(2);

	if(!send_data_to_qn8065_byte(QN8065_REG_CCA, QND_SINE_WAVE_CLOCK))return FALSE;
    send_data_to_qn8065_byte(QN8065_REG_XTAL_DIV0, QN8065_XTAL_DIV0);
    send_data_to_qn8065_byte(QN8065_REG_XTAL_DIV1, QN8065_XTAL_DIV1);
    send_data_to_qn8065_byte(QN8065_REG_XTAL_DIV2, QN8065_XTAL_DIV2);
	OSTimeDly(5);// 2

    send_data_to_qn8065_byte(QN8065_REG_CCA1, 0x96);
    send_data_to_qn8065_byte(QN8065_REG_CCOND1, 0xb6);

    send_data_to_qn8065_byte(QN8065_REG_SMSTART,0x80|QN8065_SMSTART_VAL); //set SMSTART
    send_data_to_qn8065_byte(QN8065_REG_SNCSTART,0x80|QN8065_SNCSTART_VAL); //set SNCSTART
    send_data_to_qn8065_byte(QN8065_REG_HCCSTART,0x80|QN8065_HCCSTART_VAL); //set HCCSTART

	OSTimeDly(1);// 1

	send_data_to_qn8065_byte(QN8065_REG_XTAL_DIV2, QN8065_XTAL_DIV2);

    //set IF counter error value is 256 = 4*64
    send_data_to_qn8065_byte(QN8065_REG_CCA_CNT, 4);
    //set CCA time
    send_data_to_qn8065_byte(QN8065_REG_CCA_TIME, CCA_NAGC_40MS|CCA_NSNR_20MS|CCA_NRSSI_10MS);
    //set RSSI threshold for CCA
    read_data_for_qn8065_reg(QN8065_REG_CCA, 1);

    send_data_to_qn8065_byte(QN8065_REG_CCA, ((qn8065_reg->cca&(~0x3f))|QN8065_RSSI_THERSHOLD));

    //set SNR threshold for CCA
	read_data_for_qn8065_reg(QN8065_REG_CCA_SNR_TH,1);

	//send_data_to_qn8065_byte(QN8065_REG_CCA_SNR_TH,QN8065_SNR_THERSHOLD);
	send_data_to_qn8065_byte(QN8065_REG_CCA_SNR_TH,((qn8065_reg->cca_snr_th&(~0x3f))|QN8065_SNR_THERSHOLD));
	//TRACE("rssiTH=%d, snrTH=%d\n", rssiTH,snrTH);

	send_data_to_qn8065_byte(0x11,0xc9);

	OSTimeDly(2);// 1

	//wake up
   // read_data_for_qn8065_reg(QN8065_REG_SYSTEM1,1);
	//qn8065_reg->system_1.standby  = 0;
	//send_data_to_qn8065_reg(QN8065_REG_SYSTEM1,1);
	change_for_qn8065_byte(QN8065_REG_SYSTEM1,0x20,0x00);

    qn8065_set_mute(ENABLE);
    OSTimeDly(10); //avoid noise from sleep to wakeup mode during.
    qn8065_set_mute(DISABLE);

	return TRUE;
}

static uint8 qn8065_set_stereo(uint8 stereo)
{
	uint8 value;
	if(stereo)
	{
//		qn8065_reg->system_1.force_mo = FALSE;
		value = 0x00;
	}
	else
	{
//		qn8065_reg->system_1.force_mo = TRUE;
		value = 0x04;

	}
//	return send_data_to_qn8065_reg(QN8065_REG_SYSTEM1, 1);
	return change_for_qn8065_byte(QN8065_REG_SYSTEM1,0x04,value);
}

static uint8 qn8065_set_scan(uint16 start_freq,uint16 stop_freq)
{
    uint8 temp;
	uint16 start,stop;
    //TRACE("start:%d,stop:%d,step:%d \n",start,stop, step);
    start = (start_freq-6000)/5;
    //fored the stop is bigger than start
	//if(dir)
  	//	stop = ((freq+5)-6000)/5;
	//else
	//	stop = ((freq-5)-6000)/5;
	stop = (stop_freq-6000)/5;

	//writing higher bits of channel frequency,CCA channel start,
	//stop and step index at first
	temp = (uint8) ((start >> 8) & QN8065_CH);
	temp |= ((uint8)(start >> 6) & QN8065_CH_START);
	temp |= ((uint8) (stop >> 4) & QN8065_CH_STOP);
	temp |= (0<< 6);	//5 10 20
	
	send_data_to_qn8065_byte(QN8065_REG_CH_STEP, temp);
	//writing lower 8 bits of CCA channel start index
	send_data_to_qn8065_byte(QN8065_REG_CH_START, (uint8)start);
	//writing lower 8 bits of CCA channel stop index
	send_data_to_qn8065_byte(QN8065_REG_CH_STOP, (uint8)(stop));
	//writing lower 8 bits of channel index
	send_data_to_qn8065_byte(QN8065_REG_CH, (uint8)start);

	return TRUE;
}

static uint8 qn8065_set_freq(uint16 freq)
{
#if defined(XTAL_QN8065_32768HZ)
	uint8 stereoFlag = 0;
	uint8 reg;
	read_data_for_qn8065_reg(QN8065_REG_STATUS1,1);
	stereoFlag = (qn8065_reg->status_1.st_mo_rx) ? 0:1;
	//improving some special stations snr when working clock is 32768Hz
	if (freq == 10030 || freq == 10090 || freq == 10100 || freq == 10110
		|| freq == 10150 || freq == 10160 || freq == 10220)
	{
		//frequency is added 50k only system clock is 32K.
		send_data_to_qn8065_byte(QN8065_REG_XTAL_DIV2, 0x67);
		//add 50K
		freq += 5;
	}
	//restore XTAL_DIV2 right value if the station is not a special station
	else
	{
		send_data_to_qn8065_byte(QN8065_REG_XTAL_DIV2, QN8065_XTAL_DIV2);
	}
#endif
    send_data_to_qn8065_byte(QN8065_REG_FLL,0x84); //enable 2k speed
    //enter idle mode and disable CCA, with user write into frequency

    //send_data_to_qn8065_byte(QN8065_REG_SYSTEM1, 0x01);
    change_for_qn8065_byte(QN8065_REG_SYSTEM1, 0x13,0x01);
    qn8065_set_scan(freq,freq);
    //enter rx mode and disable CCA, with user write into frequency
    //send_data_to_qn8065_byte(QN8065_REG_SYSTEM1, 0x11);
    change_for_qn8065_byte(QN8065_REG_SYSTEM1, 0x13,0x11);

    OSTimeDly(1);
    send_data_to_qn8065_byte(QN8065_REG_FLL, 0x04);//disable 2k speed ]
#if defined(XTAL_QN8065_32768HZ)
	read_data_for_qn8065_byte(QN8065_REG_CCA1,&reg);
	//disable bypass 32k spur rejection filter if is stereo
	if (stereoFlag)
	{
		reg &= ~0x20;
	}
	//enable bypass 32k spur rejection filter if is mono
	else
	{
		reg |= 0x20;
	}
	send_data_to_qn8065_byte(QN8065_REG_CCA1, reg);
#endif
    return TRUE;
}

static uint8 qn8065_check_chsc(uint8 *chsc)
{
	if(!read_data_for_qn8065_reg(QN8065_REG_SYSTEM1, 1))return FALSE;
	*chsc = !qn8065_reg->system_1.chsc;
	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_qn8065_detect(void);
static uint8 tuner_qn8065_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_qn8065_exit(void);
static uint8 tuner_qn8065_freq(uint16);
static uint8 tuner_qn8065_status(engine_status_t *);
static uint8 tuner_qn8065_search(uint16, engine_status_t *);
static uint8 tuner_qn8065_hard(engine_hard_t *);
static uint8 tuner_qn8065_setting(fm_setting_e, void *);
static uint8 tuner_qn8065_info(fm_info_e, void *);


static uint8 tuner_qn8065_detect(void)
{
	uint8 result;
	uint8 chip_id;

	i2c_data_init(QN8065_I2C_ADDR, QN8065_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	if(!result)
	{
		//ew_puts("RadioI2cFail\n" );
		i2c_data_exit();
		return FALSE;
	}
	i2c_data_exit();

	result = read_data_for_qn8065_byte(QN8065_REG_CID2, &chip_id);
	//ew_printf("tuner_qn8065_detect:%d--chip_id:%d\n", result,(chip_id&0xfc));
	if(!result)
	{
		return FALSE;
	}

	if((chip_id&0xfc) != QN8065_CHIPID_QN8065)
	{
		result = FALSE;
	}

	return result;
}

static uint8 tuner_qn8065_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	//ew_printf("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);

	sys_mute_ctrl(ENABLE, 3000);

	if(qn8065_reg == NULL)
	{
		qn8065_reg = (qn8065_reg_t *)malloc_fun(qn8065_reg, sizeof(qn8065_reg_t), 0);
		if(qn8065_reg == NULL)return FALSE;
	}

	if(qn8065_ctrl == NULL)
	{
		qn8065_ctrl = (qn8065_ctrl_t *)malloc_fun(qn8065_ctrl, sizeof(qn8065_ctrl_t), 0);
		if(qn8065_ctrl == NULL)
		{
			free_fun((void **)&qn8065_reg);
			return FALSE;
		}
	}
	//ew_puts("qn8065_memory_ok\n");

	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto qn8065_init_error;

		#if defined(JYL_FREQ_LA_BRAZIL_)||defined(_SUPPORT_FREQ_JAPAN_)||defined(JTI_FREQ_LA_BRAZIL_)
			if(min_freq < 7600)goto qn8065_init_error;
		#else
			if(min_freq < 8700)goto qn8065_init_error;
		#endif
			if(max_freq > 10800)goto qn8065_init_error;
			break;

		default:
			goto qn8065_init_error;
			break;
	}

	if(qn8065_ctrl->band == band)
	{
		if(qn8065_ctrl->init_succeed)
		{
			if(qn8065_ctrl->now_freq == freq)
			{
				qn8065_ctrl->min_freq		= min_freq;
				qn8065_ctrl->max_freq		= max_freq;
				if(!tuner_qn8065_setting(TUR_SET_MUTE, OFF))goto qn8065_init_error;
				return TRUE;
			}
			else
			{
				goto qn8065_init_freq;
			}
		}
		else
		{
			goto qn8065_init_error;
		}
	}

	if(!qn8065_set_init())goto qn8065_init_error;

qn8065_init_freq:
	qn8065_ctrl->band 			= band;
	qn8065_ctrl->min_freq 		= min_freq;
	qn8065_ctrl->max_freq 		= max_freq;
	qn8065_ctrl->init_succeed 	= TRUE;

	engine_status_t status;
	if(!tuner_qn8065_search(freq, &status))goto qn8065_init_error;
#if defined(_TUNER_DRIVER_QN8065_)
	global.set_freq_flag = TRUE;
#endif

	if(!tuner_qn8065_setting(TUR_SET_MUTE, OFF))goto qn8065_init_error;
	//ew_puts("QN8065 Init Succeed!!!!!\n");
	return TRUE;

qn8065_init_error:
	free_fun((void **)&qn8065_reg);
	free_fun((void **)&qn8065_ctrl);
	//ew_puts("QN8065 Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_qn8065_exit(void)
{
	//ew_puts("tuner_qn8065_exit\n");

	if(qn8065_reg && qn8065_ctrl)
	{
		free_fun((void **)&qn8065_reg);
		free_fun((void **)&qn8065_ctrl);
	}

	return TRUE;
}

static uint8 reset_freq_flag;
static uint8 tuner_qn8065_freq(uint16 freq)
{
	//ew_printf("%s, freq:%d\n", __FUNCTION__, freq);
	uint16 temp_freq;
	if((qn8065_reg == NULL)||(qn8065_ctrl == NULL))return FALSE;
	if(!qn8065_ctrl->init_succeed)return FALSE;
	if(freq < qn8065_ctrl->min_freq)return FALSE;
	if(freq > qn8065_ctrl->max_freq)return FALSE;
   	temp_freq = qn8065_get_frq();
	if(temp_freq < freq)
    	qn8065_set_scan(freq,freq+5);
	else
    	qn8065_set_scan(freq,freq-5);
  	change_for_qn8065_byte(QN8065_REG_SYSTEM1,0x13,0x12);
	/**********/
	/*uint8 chsc;
	uint8 loop = 200;
    while(TRUE)
	{
		qn8065_check_chsc(&chsc);
		if(!chsc)
		{
			OSTimeDly(1);
		}
		else
		{
			break;
		}
		ew_printf("loop:%d---chsc:%d\n ", loop,chsc);
		if(!(--loop))return FALSE;
	}*/
	qn8065_ctrl->stereo = 0x00;
	qn8065_ctrl->now_freq = freq;
	//ew_printf("tuner_qn8065_freq----succeed\n");

//	if(global.set_freq_flag)
//	{
//		reset_freq_flag  = TRUE;
//		global.set_freq_flag = FALSE;
//	}
	return TRUE;
}
static uint8 tuner_qn8065_status(engine_status_t *status)
{
	//ew_printf("%s\n ", __FUNCTION__);
	if((qn8065_reg == NULL)||(qn8065_ctrl == NULL))return FALSE;
	if(!qn8065_ctrl->init_succeed)return FALSE;

	uint8 chsc;
	uint8 loop = 200;
	status->freq 	= qn8065_ctrl->now_freq;
	status->signal 	= 0;
	status->valid 	= FALSE;
	status->stereo 	= FALSE;
	status->band 	= RADIO_BAND_FM;
	status->stc 	= FALSE;


    while(TRUE)
	{
		qn8065_check_chsc(&chsc);
		if(!chsc)
		{
			OSTimeDly(1);
		}
		else
		{
			break;
		}
		//ew_printf("loop2:%d---chsc2:%d\n ", loop,chsc);
		if(!(--loop))return FALSE;
	}

	if(!read_data_for_qn8065_reg(QN8065_REG_SNR,3))return FALSE;
	status->stc 	= TRUE;
	status->signal 	= qn8065_reg->rssi;

#if 0
	{
		ew_printf(" \n\n");
		uint8 i,byte;
		for(i=0;i<16;i++)
		{
			read_data_for_qn8065_byte(QN8065_REG_SYSTEM1+i,&byte);
			ew_printf(" REG_%d:%x ",(uint16)i,byte);
		}
		ew_printf(" \n\n");

	}
#endif
	if(!qn8065_reg->status_1.rxcca_fail)
	{
		status->valid = TRUE;
	}

	//ew_printf(" qn8065_reg->snr:%d---st_mo_rx:%d \n",qn8065_reg->snr,qn8065_reg->status_1.st_mo_rx);

	qn8065_ctrl->stereo <<= 1;
	if(qn8065_reg->snr > QN8065_TUNER_ST_SNR)
	{
		if(!qn8065_reg->status_1.st_mo_rx)
		{
			qn8065_ctrl->stereo |= _B0_;
		}
	}
	if((qn8065_ctrl->stereo & (_B1_|_B0_)) == (_B1_|_B0_))
	{
		if(status->signal >= QN8065_TUNER_RSSI_FM)
		{
			status->stereo = TRUE;
		}
	}
	if(global.set_freq_flag)
	{
	//	reset_freq_flag = FALSE;
		global.set_freq_flag = FALSE;
		qn8065_set_freq(status->freq);
		//ew_printf("-----reset freq------ \n");
	}
	//ew_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_qn8065_search(uint16 freq, engine_status_t *status)
{
	//ew_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if((qn8065_reg == NULL)||(qn8065_ctrl == NULL))return FALSE;
	if(!qn8065_ctrl->init_succeed)return FALSE;
	if(freq < qn8065_ctrl->min_freq)return FALSE;
	if(freq > qn8065_ctrl->max_freq)return FALSE;
	//qn8065_set_freq(freq);
	//OSTimeDly(1);
	if(!tuner_qn8065_freq(freq))return FALSE;
  	//send_data_to_qn8065_byte(QN8065_REG_SYSTEM1,0x12);
  	//change_for_qn8065_byte(QN8065_REG_SYSTEM1,0x13,0x12);
	OSTimeDly(4);

	uint8 index;
	for(index = 0; index < 50; index++)
	{
		if(!tuner_qn8065_status(status))return FALSE;
		if(status->stc)break;
		OSTimeDly(1);
	}
	if(index == 50)return FALSE;

	//ew_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_qn8065_setting(fm_setting_e type, void *set)
{
	//ew_printf("%s, type:%d\n", __FUNCTION__, type);
	if((qn8065_reg == NULL)||(qn8065_ctrl == NULL))return FALSE;
	if(!qn8065_ctrl->init_succeed)return FALSE;

	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!qn8065_set_mute(mute))return FALSE;
			}
			break;

		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(!qn8065_set_stereo(stereo))return FALSE;
			}
			break;

		default:
			return FALSE;
	}

	//ew_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}
static uint8 tuner_qn8065_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_QN8065;
			}
			break;

		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = QN8065_TUNER_RSSI_FM;
			}
			break;

		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = QN8065_TUNER_RSSI_LOC_FM;
			}
			break;

		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_qn8065_deal_spot_tab)/sizeof(uint16);
			}
			break;

		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_qn8065_deal_spot_tab;
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
void Qn8065WokeOkDet(void)
{
	static uint8 errcnt=0;
	if(qn8065_ctrl->now_freq != qn8065_get_frq())
	{
		if(errcnt++>3)
		{
			errcnt =0;
			os_taskq_post_msg(FM_TASK_NAME, 1, MSG_FM_ERR);
		}
	}
	else
	{
		errcnt =0;
	}
}
const tuner_drv_fun_t tuner_qn8065_drv_fun =
{
	tuner_qn8065_detect,
	tuner_qn8065_init,
	tuner_qn8065_exit,
	tuner_qn8065_freq,
	tuner_qn8065_status,
	tuner_qn8065_search,
	NULL,
	tuner_qn8065_setting,
	tuner_qn8065_info,
};

#endif
