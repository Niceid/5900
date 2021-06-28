#include "lt4730/lt4730.h"


#if defined(_TUNER_DRIVER_LT4730_)

static lt4730_reg_t *lt4730_reg = NULL;
static lt4730_ctrl_t *lt4730_ctrl = NULL;

static const uint16 fm_lt4730_deal_spot_tab[] = {9600};
static const uint16 am_lt4730_deal_spot_tab[] = {};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static uint8 send_byte_to_lt4730(uint8 reg_addr, uint8 data)
{	
	uint8 transfer[2];
	
	transfer[0] = reg_addr;
	transfer[1] = data;	
	i2c_data_init(LT4730_I2C_ADDR, LT4730_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(transfer, sizeof(transfer));
	i2c_data_exit();
	return result;
}

static uint8 read_byte_for_lt4730(uint8 reg_addr, uint8 *buffer)
{
	i2c_data_init(LT4730_I2C_ADDR, LT4730_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(&reg_addr, 1);
	if(result)
	{
		result = i2c_data_r(buffer, 1);
	}
	i2c_data_exit();
	return result;
}

static uint8 send_data_to_lt4730(lt4730_reg_e reg_addr)
{	
	return send_byte_to_lt4730(reg_addr, _au8_(lt4730_reg, reg_addr));
}

static uint8 read_data_for_lt4730(lt4730_reg_e reg_addr)
{
	return read_byte_for_lt4730(reg_addr, &_au8_(lt4730_reg, reg_addr));
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_lt4730_detect(void);
static uint8 tuner_lt4730_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_lt4730_exit(void);
static uint8 tuner_lt4730_freq(uint16);
static uint8 tuner_lt4730_status(engine_status_t *);
static uint8 tuner_lt4730_search(uint16, engine_status_t *);
static uint8 tuner_lt4730_hard(engine_hard_t *);
static uint8 tuner_lt4730_setting(fm_setting_e, void *);
static uint8 tuner_lt4730_info(fm_info_e, void *);


static uint8 tuner_lt4730_detect(void)
{
	uint8 result;
	
	i2c_data_init(LT4730_I2C_ADDR, LT4730_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	i2c_data_exit();
	
	tuner_printf("tuner_lt4730_detect:%d\n", result);
	return result;
}

static uint8 lt4730_set_crystal(void)
{
	uint16 div_reg;
	uint8  high_data;
	
	div_reg = ((uint16)(((((float)LT4730_CRY_FREQ)/32768.00)*10 + 0.5)/10));

	high_data  =  _u16to8h_(div_reg);
	high_data &=  0x07;
	high_data <<= 5;

	if(!send_byte_to_lt4730(LT4730_REG_XTALDIV0,	_u16to8l_(div_reg)))return FALSE;
	
	if(!send_byte_to_lt4730(LT4730_REG_XTALDIV1,	high_data))return FALSE;

	if(!send_byte_to_lt4730(LT4730_REG_DIVDML,		0x80))return FALSE;

	if(!send_byte_to_lt4730(LT4730_REG_DIVDMM,		0xCB))return FALSE;

	if(!send_byte_to_lt4730(LT4730_REG_XTALDIV1,	0x06))return FALSE;

	return TRUE;
}

static uint8 lt4730_set_init(void)
{
	uint8 temp;

    lt4730_reg->System.Soft_Reset = 1;
    lt4730_reg->System.Ch_Mode = 1;
    if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
    
    if(!send_byte_to_lt4730(0x60, 0x10))return FALSE;							// 2013.9.7
    
    if(!lt4730_set_crystal())return FALSE;
    
    if(!read_data_for_lt4730(LT4730_REG_ENABLESTATUS))return FALSE;
    lt4730_reg->EnableStaus.Ref_Mode = 2;// 3									// 2013.12.31修改单独晶体时在AM波段关ACC后再开AM无信号
    lt4730_reg->EnableStaus.Demp = 1;
    if(!send_data_to_lt4730(LT4730_REG_ENABLESTATUS))return FALSE;				//启动晶体
    
    lt4730_reg->System.Soft_Reset = 0;
    lt4730_reg->System.Ch_Mode = 0;
    lt4730_reg->System.Calibration = 1;
    if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
    lt4730_reg->System.Calibration = 0;
    if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
    
    if(!lt4730_power)
    {
    #if defined(_RADIO_INIT_DELAY_)
    	OSTimeDly(150);//50
    #else
		OSTimeDly(50);
	#endif
    }
    else
    {
    	OSTimeDly(10);
    }
    
    if(!read_data_for_lt4730(LT4730_REG_VOLCTRL))return FALSE;					//音量使用缺省值，最大
    
    if(!read_byte_for_lt4730(LT4730_REG_VREGL, &temp))return FALSE;
    temp &= ((uint8)(~0x0C));
    temp |= 0x08;
    if(!send_byte_to_lt4730(LT4730_REG_VREGL, temp))return FALSE;
    
    if(!send_byte_to_lt4730(LT4730_REG_SEEK_RSSI_SEL,0x62))return FALSE;		// add in 2013.04.02 XYF
    
    if(!send_byte_to_lt4730(LT4730_REG_ST,0x88))return FALSE;					//处理FM左右声道分离反
    
    if(!lt4730_power)
    {
    #if defined(_RADIO_INIT_DELAY_)
		OSTimeDly(50);// 100
	#else
		OSTimeDly(100);
	#endif
    }
    
    if(lt4730_TunerBand == LT4730_TUNER_BAND_FM)
    {
    	if(!send_byte_to_lt4730(LT4730_REG_NHCC, 0x20))return FALSE;			//0XB5	2013.5.24
    	if(!send_byte_to_lt4730(LT4730_REG_NCC_FITER, 0xAF))return FALSE;		//0xB0	2013.5.24
    	if(!send_byte_to_lt4730(LT4730_REG_75K, 0x51))return FALSE; 			//2013.8.6
    	if(!send_byte_to_lt4730(LT4730_REG_75K2, 0x3A))return FALSE;			//2013.8.6
    	if(!send_byte_to_lt4730(LT4730_REG_RESPONSE,0x2D))return FALSE; 		//add  2013.8.5
    	
    	//if(!send_byte_to_lt4730(LT4730_REG_ENABLESTATUS,0xF3))return FALSE;		//最大值
    	if(!send_byte_to_lt4730(LT4730_REG_HCCSTART, 0xA1))return FALSE;		//80
    
    #if defined(REDUCE_FREUENCY_RESPONSE_)
    	if(!send_byte_to_lt4730(LT4730_REG_SOFTMUTE_TH, 0x80))return FALSE; 		//add  2013.5.24
    
    	lt4730_reg->EnableStaus.Demp=0;										//2013.7.24   // FM频响
    	lt4730_reg->EnableStaus.Vol=7;									   //2013.8.1	 // FM增益比
    	if(!send_data_to_lt4730(LT4730_REG_ENABLESTATUS))return FALSE;		//add  2013.5.24
    #endif
    
    	lt4730_reg->EnableStaus.Demp=0; 									//2013.7.24   // FM频响
    	lt4730_reg->EnableStaus.Vol=7;									   //2013.8.1	 // FM增益比
    	if(!send_data_to_lt4730(LT4730_REG_ENABLESTATUS))return FALSE;		//add  2013.5.24
    	if(!send_byte_to_lt4730(LT4730_REG_VOLCTRL, 0x16))return FALSE; 		//add  2013.5.24

    	lt4730_reg->System.Ch_Mode		= 1;
    	lt4730_reg->System.Rx_En 		= 1;
    	lt4730_reg->System.Band			= 2;
    	lt4730_reg->System.Calibration	= 1;									// 2013.7.31
    	if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
    	lt4730_reg->System.Calibration	= 0;									// 2013.7.31
    	if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
    
    	lt4730_reg->CcaRxSnr.Seek_Snr_Th 		= LT4730_TUNER_SNR_FM;
    	lt4730_reg->HRssiThresh.Seek_Rssi_Hth	= LT4730_TUNER_HIGH_RSSI_FM;
    	lt4730_reg->LRssiThresh.Seek_Rssi_Lth	= LT4730_TUNER_LOW_RSSI_FM;
    	
    	lt4730_reg->SeekHighCount.Seek_F_Err 	= LT4730_TUNER_IFCNT_HIGH;
    	lt4730_reg->SeekLowCount 				= LT4730_TUNER_IFCNT_LOW;
    
    	lt4730_reg->HighChannel.Seek_Step		= LT473x_FM_STEP_50KHZ;
    }
    else if(lt4730_TunerBand == LT4730_TUNER_BAND_AM)
    {
	#if defined(_ENABLE_AM_RESPONSE_)
		if(!send_byte_to_lt4730(LT4730_REG_HCCSTART, 0xA8))return FALSE; // 改AM频响
	#else
    	if(!send_byte_to_lt4730(LT4730_REG_HCCSTART, 0xBF))return FALSE;
	#endif
    	
    	lt4730_reg->HighChannel.Lo_Freq = 1;
    	if(!send_data_to_lt4730(LT4730_REG_HIGHCHANNEL))return FALSE;
    	//if(!send_byte_to_lt4730(LT4730_REG_HIGHCHANNEL, 0x80))return FALSE;
    	
    	if(!send_byte_to_lt4730(LT4730_REG_AMLNA, 0x33))return FALSE;
    	if(!send_byte_to_lt4730(LT4730_REG_DIG, 0x69))return FALSE;
    	if(!send_byte_to_lt4730(LT4730_REG_AM_MODULATION, 0x24))return FALSE;	//往0x51寄存器写入0x25,解决AM在60%调制失真
    	if(!send_byte_to_lt4730(LT4730_REG_FLL, 0x84))return FALSE; 			//2013.8.1
    	
    	lt4730_reg->EnableStaus.Vol=7;											//2013.5.20
    	if(!send_data_to_lt4730(LT4730_REG_ENABLESTATUS))return FALSE;
    
    	lt4730_reg->VolCtrl.Vol=5;												// 1
    	lt4730_reg->VolCtrl.Res=2;												// 3
    	if(!send_data_to_lt4730(LT4730_REG_VOLCTRL))return FALSE;
    
    	if(!send_byte_to_lt4730(0x6d, 0x41))return FALSE;
		
    #if defined(_ENABLE_AM_RESPONSE_)// 关掉
    	if(!send_byte_to_lt4730(0x47, 0x50))return FALSE;						// REG_SOFTMUTE_TH
    #else
    	if(!send_byte_to_lt4730(0x47, 0x4A))return FALSE;						// REG_SOFTMUTE_TH
    #endif
	
    	lt4730_reg->System.Ch_Mode	= 1;
    	lt4730_reg->System.Rx_En 	= 1;
    	lt4730_reg->System.Band		= 0;
    	lt4730_reg->System.Calibration	= 1;									// 2014.5.12
    	if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
    	lt4730_reg->System.Calibration	= 0;									// 2014.5.12，修改AM关ACC后无信号
    	if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
    
    	if(!send_byte_to_lt4730(LT4730_REG_AM_TUNE,0x00))return FALSE;			// 2013.8.6
    	OSTimeDly(2);
    	if(!send_byte_to_lt4730(LT4730_REG_AM_TUNE,0x80))return FALSE;			// 2013.8.6
    
    	lt4730_reg->CcaRxSnr.Seek_Snr_Th 		= LT4730_TUNER_SNR_AM;
    	lt4730_reg->HRssiThresh.Seek_Rssi_Hth	= LT4730_TUNER_HIGH_RSSI_AM;
    	lt4730_reg->LRssiThresh.Seek_Rssi_Lth	= LT4730_TUNER_LOW_RSSI_AM;
    	
    	lt4730_reg->SeekHighCount.Seek_F_Err 	= LT4730_TUNER_IFCNT_HIGH;
    	lt4730_reg->SeekLowCount 				= LT4730_TUNER_IFCNT_LOW;	
    
    	lt4730_reg->HighChannel.Seek_Step		= LT473x_AM_STEP_1KHZ;
    }
    else return FALSE;
    
    OSTimeDly(5);
    
    if(!send_data_to_lt4730(LT4730_REG_CCARXSNR))return FALSE;
    if(!send_data_to_lt4730(LT4730_REG_HRSSITHRESH))return FALSE;
    if(!send_data_to_lt4730(LT4730_REG_LRSSITHRESH))return FALSE;
    
    if(!send_data_to_lt4730(LT4730_REG_SEEKHIGHCOUNT))return FALSE;
    if(!send_data_to_lt4730(LT4730_REG_SEEKLOWCOUNT))return FALSE;
    
    if(!send_data_to_lt4730(LT4730_REG_HIGHCHANNEL))return FALSE;
    
    if(!read_data_for_lt4730(LT4730_REG_XTALDIV1))return FALSE;
    
    if(!read_data_for_lt4730(LT4730_REG_SYSTEM))return FALSE;

     return TRUE;
}

static uint8 tuner_lt4730_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{	
	if(lt4730_reg == NULL)
	{
		lt4730_reg = (lt4730_reg_t *)malloc_fun(lt4730_reg, sizeof(lt4730_reg_t), 0);
		if(lt4730_reg == NULL)return FALSE;
	}
				
	if(lt4730_ctrl == NULL)
	{
		lt4730_ctrl = (lt4730_ctrl_t *)malloc_fun(lt4730_ctrl, sizeof(lt4730_ctrl_t), 0);
		if(lt4730_ctrl == NULL)
		{
			free_fun((void **)&lt4730_reg);
			return FALSE;
		}
	}

	tuner_puts("lt4730_memory_ok\n");
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto lt4730_init_error;
			if(min_freq < 6400)goto lt4730_init_error;
			if(max_freq > 10800)goto lt4730_init_error;
			lt4730_ctrl->TunerBand = LT4730_TUNER_BAND_FM;
			break;
			
		case RADIO_BAND_AM:
			if(min_freq > max_freq)goto lt4730_init_error;
			if(min_freq < 520)goto lt4730_init_error;
			if(max_freq > 1710)goto lt4730_init_error;
			lt4730_ctrl->TunerBand = LT4730_TUNER_BAND_AM;
			break;
			
		default:
			goto lt4730_init_error;
			break;
	}
	
	if(lt4730_ctrl->band == band)
	{
		if(lt4730_ctrl->init_succeed)
		{
			if(lt4730_ctrl->now_freq == freq)
			{
				lt4730_ctrl->min_freq = min_freq;
				lt4730_ctrl->max_freq = max_freq;
				if(!tuner_lt4730_setting(TUR_SET_MUTE, OFF))goto lt4730_init_error;
				return TRUE;
			}
			else
			{
				goto lt4730_init_freq;
			}
		}
		else 
		{
			goto lt4730_init_error;
		}
	}
		
	if(!lt4730_set_init())goto lt4730_init_error;
		
lt4730_init_freq:
	lt4730_ctrl->band = band;
	lt4730_ctrl->min_freq = min_freq;
	lt4730_ctrl->max_freq = max_freq;
	lt4730_ctrl->init_succeed = TRUE;
		
	engine_status_t status;
	
	lt4730_power = TRUE;
	lt4730_found = FALSE;
	if(!tuner_lt4730_search(freq, &status))goto lt4730_init_error;
	OSTimeDly(10);
	if(!tuner_lt4730_search(freq, &status))goto lt4730_init_error;
	if(!tuner_lt4730_setting(TUR_SET_MUTE, OFF))goto lt4730_init_error;
	return TRUE;
	
lt4730_init_error:
	
	lt4730_power = FALSE;
	free_fun((void **)&lt4730_reg);
	free_fun((void **)&lt4730_ctrl);
	return FALSE;
}

static uint8 tuner_lt4730_exit(void)
{
	tuner_puts("tuner_lt4730_exit\n");
	if(lt4730_reg&&lt4730_ctrl)
	{
		free_fun((void **)&lt4730_reg);
		free_fun((void **)&lt4730_ctrl);
	}
	
	return TRUE;
}

static uint8 lt4730_set_mute(uint8 mute)
{
	if(mute)
	{
		lt4730_reg->VolCtrl.Mute1_En = TRUE;	
		lt4730_reg->VolCtrl.Mute2_En = TRUE;			
	}	
	else
	{
		lt4730_reg->VolCtrl.Mute1_En = FALSE;	
		lt4730_reg->VolCtrl.Mute2_En = FALSE;		
	}
	return send_data_to_lt4730(LT4730_REG_VOLCTRL);
}

static uint8 lt4730_set_stereo(uint8 stereo)
{	
	if(stereo)
	{
		lt4730_reg->XtalDiv1.Mono = FALSE;
	}
	else
	{
		lt4730_reg->XtalDiv1.Mono = TRUE;
	}	
	return send_data_to_lt4730(LT4730_REG_XTALDIV1);
}

static uint8 lt4730_set_scan(uint16 freq)
{	
	if(lt4730_TunerBand == LT4730_TUNER_BAND_FM)
	{
		freq = (freq - 6000)/5;
	}
	
	lt4730_reg->HCcaStart.Seek_F_Start 	= _u16to8h_(freq);
	lt4730_reg->LCcaStart 			 	= _u16to8l_(freq);
	lt4730_reg->HCcaStop.Seek_F_Stop 	= _u16to8h_(freq);
	lt4730_reg->LCcaStop 				= _u16to8l_(freq);

	if(!send_byte_to_lt4730(LT4730_REG_DIG, 0x00))return FALSE;				// 0x80 2013.8.21
	
	if(!send_data_to_lt4730(LT4730_REG_HCCASTART))return FALSE;
    if(!send_data_to_lt4730(LT4730_REG_LCCASTART))return FALSE;
	if(!send_data_to_lt4730(LT4730_REG_HCCASTOP))return FALSE;
	if(!send_data_to_lt4730(LT4730_REG_LCCASTOP))return FALSE;
	
	lt4730_reg->HighChannel.Seek_Step = 0;
	if(!send_data_to_lt4730(LT4730_REG_HIGHCHANNEL))return FALSE;
	
	lt4730_reg->System.Ch_Mode = 0;
	lt4730_reg->System.Seek_Mode = 1;
	if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
	
	return TRUE;
}

static uint8 lt4730_set_freq(uint16 freq)
{
	if(!lt4730_reg->System.Ch_Mode)
	{
		lt4730_reg->System.Ch_Mode = 1;
		if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;
	}

	if(lt4730_TunerBand == LT4730_TUNER_BAND_FM)
	{
		if(freq == 9800)
		{// 2013.5.27 解决98.0这个点的灵敏度12DB
			lt4730_reg->HighChannel.Lo_Freq = 1;
		}
		else
		{
			lt4730_reg->HighChannel.Lo_Freq = 0;
		}
		freq = (freq - 6000)/5;
	}
	else
	{
		lt4730_reg->HighChannel.Lo_Freq = 1;
	}
	
	lt4730_reg->HighChannel.Ch_Freq 	= _u16to8h_(freq);
	if(!send_data_to_lt4730(LT4730_REG_HIGHCHANNEL))return FALSE;
    lt4730_reg->LowChannel 			= _u16to8l_(freq);
	if(!send_data_to_lt4730(LT4730_REG_LOWCHANNEL))return FALSE;
	
	lt4730_reg->System.Ch_Mode 		= 1;
	lt4730_reg->System.Rx_En 		= 1;
	if(lt4730_TunerBand == LT4730_TUNER_BAND_FM)
	{
		lt4730_reg->System.Band = 2;
	}
	else
	{
		lt4730_reg->System.Band = 0;
	}
	if(!send_data_to_lt4730(LT4730_REG_SYSTEM))return FALSE;

	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static uint8 tuner_lt4730_freq(uint16 freq)
{	
	uint8 index;
	
	if((lt4730_reg == NULL)||(lt4730_ctrl == NULL))return FALSE;
	if(!lt4730_ctrl->init_succeed)return FALSE;
	if(freq < lt4730_ctrl->min_freq)return FALSE;
	if(freq > lt4730_ctrl->max_freq)return FALSE;

	lt4730_found = FALSE;
#if defined(REDUCE_AM_NOISE_)
	lt4730_am_lock = TRUE;
#endif

	lt4730_reg->HighChannel.Lo_Freq = 0;
	send_data_to_lt4730(LT4730_REG_HIGHCHANNEL);

	if(!lt4730_set_scan(freq))return FALSE;

	for(index = 0; index < 50; index++)// 50
	{
		OSTimeDly(1);
		
		if(!read_data_for_lt4730(LT4730_REG_SYSTEM))return FALSE;
		
		if(!lt4730_reg->System.Seek_Mode)break;
	}
	
	if(index == 50)return FALSE;// 50
	
	if(!read_data_for_lt4730(LT4730_REG_CCASTATUS))return FALSE;	
	if(!lt4730_reg->CcaStatus.Seek_Status)
	{
		lt4730_reg->HighChannel.Lo_Freq = 1;
		send_data_to_lt4730(LT4730_REG_HIGHCHANNEL);
		if(!lt4730_set_scan(freq))return FALSE;
		OSTimeDly(10);
	}
	
	if(!read_data_for_lt4730(LT4730_REG_CCASTATUS))return FALSE;
	if(!lt4730_reg->CcaStatus.Seek_Status)
	{
		lt4730_found = TRUE;
	}
	
	if(lt4730_TunerBand == LT4730_TUNER_BAND_AM)
	{
		//if(!lt4730_set_freq(freq))return FALSE;
	}

	lt4730_ctrl->now_freq = freq;

	return TRUE;
}

static uint8 tuner_lt4730_status(engine_status_t *status)
{	
	if((lt4730_reg == NULL)||(lt4730_ctrl == NULL))return FALSE;
	if(!lt4730_ctrl->init_succeed)return FALSE;

	status->freq 	= lt4730_ctrl->now_freq;
	status->signal 	= 0;
	status->valid 	= FALSE;
	status->stereo 	= FALSE;
	status->stc 	= FALSE;

	if(lt4730_TunerBand == LT4730_TUNER_BAND_FM)
	{
		status->band = RADIO_BAND_FM;
	}
	else if(lt4730_TunerBand == LT4730_TUNER_BAND_AM)
	{
		status->band = RADIO_BAND_AM;
	}
	
	if(!read_data_for_lt4730(LT4730_REG_CCASTATUS))return FALSE;
	if(!read_data_for_lt4730(LT4730_REG_RSSIVALUE))return FALSE;
	//if(!read_data_for_lt4730(LT4730_REG_SNRVALUE))return FALSE;

	status->stc 	= TRUE;
	status->signal 	= lt4730_reg->RssiValue;
	status->valid    = lt4730_found;

	if(lt4730_TunerBand == LT4730_TUNER_BAND_FM)
	{
		if(!lt4730_reg->CcaStatus.St_Mono_Status)
		{
			if(status->signal>=(LT4730_TUNER_RSSI_FM-3))
			{
				status->stereo	= TRUE;
			}
		}
	}	
#if defined(REDUCE_AM_NOISE_)
    else if(lt4730_TunerBand == LT4730_TUNER_BAND_AM)
    {    
   		bool AmLock = lt4730_reg->CcaStatus.Res3;
		
		//if(lt4730_am_lock != AmLock)
		{
			if(AmLock)
			{
				if((lt4730_reg->CcaStatus.Res2) == 0) // 读0x0B的第五位
				{
					if(!send_byte_to_lt4730(LT4730_REG_AM_TUNE,0x00))return FALSE;
					OSTimeDly(1);
					if(!send_byte_to_lt4730(LT4730_REG_AM_TUNE,0x80))return FALSE;
				}
				
				if(!send_byte_to_lt4730(LT4730_REG_SOFTMUTE,0x00))return FALSE;
				if(!send_byte_to_lt4730(LT4730_REG_VOLCTRL,0x16))return FALSE;
			}
			else
			{
			#if defined(DONGHE_REDUCE_AM_NOISE_)
    			//if(!send_byte_to_lt4730(LT4730_REG_SOFTMUTE,0x9F))return FALSE;
    			if(!send_byte_to_lt4730(LT4730_REG_VOLCTRL,0x11))return FALSE;
			#else
				if(!send_byte_to_lt4730(LT4730_REG_SOFTMUTE,0x9F))return FALSE;
				if(!send_byte_to_lt4730(LT4730_REG_VOLCTRL,0x00))return FALSE;
			#endif
				
				if(!send_byte_to_lt4730(LT4730_REG_AM_TUNE,0x00))return FALSE;
				OSTimeDly(1);
				if(!send_byte_to_lt4730(LT4730_REG_AM_TUNE,0x80))return FALSE;
			}
			
			lt4730_am_lock = AmLock;
		}
	}
#endif

	return TRUE;
}

static uint8 tuner_lt4730_search(uint16 freq, engine_status_t *status)
{
	if((lt4730_reg == NULL)||(lt4730_ctrl == NULL))return FALSE;
	if(!lt4730_ctrl->init_succeed)return FALSE;

	if(freq < lt4730_ctrl->min_freq)return FALSE;
	if(freq > lt4730_ctrl->max_freq)return FALSE;
	
	if(!tuner_lt4730_freq(freq))return FALSE;
		
	if(!tuner_lt4730_status(status))return FALSE;
	
	return TRUE;
}

static uint8 tuner_lt4730_setting(fm_setting_e type, void *set)
{
	tuner_printf("%s, type:%d\n", __FUNCTION__, type);
	if((lt4730_reg == NULL)||(lt4730_ctrl == NULL))return FALSE;
	if(!lt4730_ctrl->init_succeed)return FALSE;

	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!lt4730_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(!lt4730_set_stereo(stereo))return FALSE;
			}
			break;
			
		default:
			return FALSE;
	}
	
	tuner_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_lt4730_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_LT4730;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = LT4730_TUNER_RSSI_FM;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = LT4730_TUNER_RSSI_LOC_FM;
			}
			break;

		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_lt4730_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_lt4730_deal_spot_tab;
			}
			break;
		
		case TUR_INFO_AM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = LT4730_TUNER_RSSI_AM;
			}
			break;
			
		case TUR_INFO_AM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = LT4730_TUNER_RSSI_LOC_AM;
			}
			break;

		case TUR_INFO_AM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(am_lt4730_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_AM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)am_lt4730_deal_spot_tab;
			}
			break;
			
		case TUR_INFO_AUTOSEEK:
			if(info != NULL)
			{
				*(uint8 *)info = DISABLE;
			}
			break;
			
		default:
			return FALSE;
	}
	
	return TRUE;
}


const tuner_drv_fun_t tuner_lt4730_drv_fun =
{
	tuner_lt4730_detect,
	tuner_lt4730_init,
	tuner_lt4730_exit,
	tuner_lt4730_freq,
	tuner_lt4730_status,
	tuner_lt4730_search,
	NULL,
	tuner_lt4730_setting,
	tuner_lt4730_info,
};

#endif

