#include "ar1019/ar1019.h"

#if defined(_TUNER_DRIVER_AR1019_)

static ar1019_ctrl_t *ar1019_ctrl = NULL;
static const uint16 fm_ar1019_deal_spot_tab[] = {9600};

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
#define INTERNAL_XO   // mark this line if you're using external reference clock

#ifdef INTERNAL_XO

#if 1
static const uint16 AR1019RegInit[18]=
{ //using  AR1000 XO function
	0xFFFB,		// R0 -- the first writable register .
	0x5B15,		// R1.
	0xD0B9,		// R2.
	0xA014,		// R3.
	0x0780,		// R4.
	0x28AB,		// R5.
	0x6400,		// R6.
	0x1EE7,		// R7.
	0x7141,		// R8.
	0x007D,		// R9.
	0x82C6,		// R10.  disable wrap
	0x4E55,		// R11. <--- 
	0x970C,		// R12.
	0xB845,		// R13.
	0xFE2D,		// R14.
	0x8197,		// R15. //0x8197, 50KHz	
	0x04A1,		// R16.
	0xDF6A		// R17.
};

#else
static const uint16 AR1019RegInit[18]=
{ //using  AR1000 XO function
	0xFFFB,		// R0 -- the first writable register .
	0x5B17,		// R1.
	0xD0B9,		// R2.
	0xA000|AR1019_TUNER_RSSI_FM,	// R3.
	0x0780,		// R4.
	0x28AB,		// R5.
	0x6400,		// R6.
	0x1EE7,		// R7.
	0x7141,		// R8.
	0x007D,		// R9.
	0x82C6,		// R10.  disable wrap
	0x4E55,		// R11. <--- 
	0x970C,		// R12.
	0xB845,		// R13.
	0xFC2D,		// R14.
	0x8197,		// R15. //0x8197, 50KHz	
	0x04A1,		// R16.
	0xDF6A		// R17.
};
#endif

#else
static const uint16 AR1019RegInit[18]=
{
// using External 32.768KHz Reference Clock
	0xFF7B,		// R0 -- the first writable register .  (disable xo_en)
	0x5B17,		// R1.
	0xD4B9,		// R2.
	0xA000|AR1019_TUNER_RSSI_FM,	// R3.
	0x0780,		// R4.
	0x28AB,		// R5.
	0x6400,		// R6.
	0x1EE7,		// R7.
	0x7141,		// R8.
	0x007D,		// R9.
	0x81C6,		// R10.  disable wrap
	0x4F55,		// R11. <--- (disable xo_output)
	0x970C,		// R12.
	0xB845,		// R13.
	0xFC2D,		// R14.
	0x8197,		// R15. //0x8197, 50KHz	
	0x04E1,		// R16.
	0xDF6A		// R17.
};
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static uint8 Read_AR1019_Data(uint8 startReg, uint16 *value, uint16 length)
{
	uint8 result = FALSE;
	
	if(!length)return TRUE;

	i2c_data_init(AR1019_I2C_ADDR, AR1019_I2C_RATE, &radio_gpio);
	if(i2c_data_w(&startReg, 1))
	{
		result = i2c_data_wr(value, length);
	}
	i2c_data_exit();
	
	return result;	
}

static uint8 Write_AR1019_Data(uint8 startReg, const uint16 *value, uint16 length)
{
	uint8 result = FALSE;
	
	if(!length)return TRUE;
	
	uint8 tempData[length * 2 + 1];
	uint8 i;
	uint8 len = 0;
	tempData[len++] = startReg;
	for(i = 0; i < length; i++)
	{
		tempData[len++] = _u16to8h_(value[i]);
		tempData[len++] = _u16to8l_(value[i]);
	}
	
	i2c_data_init(AR1019_I2C_ADDR, AR1019_I2C_RATE, &radio_gpio);
	result = i2c_data_w(tempData, len);
	i2c_data_exit();
	
	return result;
}

static uint8 Read_AR1019_RegMap(uint8 startReg, uint16 length)
{
	if(!Read_AR1019_Data(startReg, &_u16_(ar1019_ctrl->reg, startReg), length))return FALSE;
	return TRUE;
}

static uint8 Write_AR1019_RegMap(uint8 startReg, uint16 length)
{
	if(!Write_AR1019_Data(startReg, &_u16_(ar1019_ctrl->reg, startReg), length))return FALSE;
	return TRUE;
}

static uint8 check_detect_ar1019(void)
{
	uint8 result = FALSE;
	uint16 chipId;
	
	if(Read_AR1019_Data(AR1019_REG_ADDR_CHIPID, &chipId, 1))
	{
		if(chipId == 0x1010)
		{
			result = TRUE;
		}
	}
	
	return result;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

static uint8 ar1019_set_power(uint8 power)
{
	ar1019_ctrl->reg.AR1019_REG0.Enable = power;
	if(!Write_AR1019_RegMap(AR1019_REG_ADDR_R0, 1))return FALSE;
	return TRUE;
}

static uint8 ar1019_set_stereo(uint8 stereo)
{
	ar1019_ctrl->reg.AR1019_REG1.Mono = !stereo;
	if(!Write_AR1019_RegMap(AR1019_REG_ADDR_R1, 1))return FALSE;
	return TRUE;
}

static uint8 ar1019_set_mute(uint8 mute)
{
	ar1019_ctrl->reg.AR1019_REG1.Hmute = mute;
	ar1019_ctrl->reg.AR1019_REG1.Smute = mute;
	if(!Write_AR1019_RegMap(AR1019_REG_ADDR_R1, 1))return FALSE;
	return TRUE;
}

static uint8 ar1019_set_freq(uint16 freq, uint8 tuneFlag)
{
	uint16 writeFreq;

	writeFreq = (freq - 6900)/5;
	ar1019_ctrl->reg.AR1019_REG2.Tune = tuneFlag;
	ar1019_ctrl->reg.AR1019_REG2.Chan0 = writeFreq & 0x0001;
	ar1019_ctrl->reg.AR1019_REG2.Chan1_9 = (writeFreq >> 1) & 0x01FF;
	if(!Write_AR1019_RegMap(AR1019_REG_ADDR_R2, 1))return FALSE;
	return TRUE;
}

static uint8 ar1019_set_seek_handle(uint8 state)
{
	if(state)
	{
		ar1019_ctrl->reg.AR1019_REG17 = 0xE36A;
	}
	else
	{
		ar1019_ctrl->reg.AR1019_REG17 = 0xDF6A;
	}
	if(!Write_AR1019_RegMap(AR1019_REG_ADDR_R17, 1))return FALSE;
	return TRUE;
}

static uint8 ar1019_get_freq(uint16 *freq)
{
	uint16 readFreq;
	
	if(!Read_AR1019_RegMap(AR1019_REG_ADDR_STATUS, 1))return FALSE;
	readFreq = ar1019_ctrl->reg.Status.ReadChan1_9;
	readFreq = (readFreq << 1) + ar1019_ctrl->reg.Status.ReadChan0;
	readFreq = readFreq * 5 + 6900;
	*freq = readFreq;
	return TRUE;
}

static uint8 ar1019_set_init(void)
{
	tuner_puts("ar1019_set_init\n");
	
	memcpy(&ar1019_ctrl->reg, AR1019RegInit, sizeof(AR1019RegInit));
	
	if(!ar1019_set_power(FALSE))return FALSE;
	if(!Write_AR1019_RegMap(AR1019_REG_ADDR_R1, 17))return FALSE;
	if(!ar1019_set_power(TRUE))return FALSE;
	
	int i;
	for(i = 0; i < 100; i++)
	{
		OSTimeDly(1);
		if(!Read_AR1019_RegMap(AR1019_REG_ADDR_STATUS, 1))return FALSE;
		if(ar1019_ctrl->reg.Status.Stc)break;
	}
	if(i == 100)return FALSE;
	
	return TRUE;
}

static uint8 turn_ar1019_seek(uint8 ctrl)
{
	ar1019_ctrl->reg.AR1019_REG3.Seek = ctrl;	
	if(!Write_AR1019_RegMap(AR1019_REG_ADDR_R3, 1))return FALSE;
	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_ar1019_detect(void);
static uint8 tuner_ar1019_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_ar1019_exit(void);
static uint8 tuner_ar1019_freq(uint16);
static uint8 tuner_ar1019_status(engine_status_t *);
static uint8 tuner_ar1019_search(uint16, engine_status_t *);
static uint8 tuner_ar1019_hard(engine_hard_t *);
static uint8 tuner_ar1019_setting(fm_setting_e, void *);
static uint8 tuner_ar1019_info(fm_info_e, void *);

static uint8 tuner_ar1019_detect(void)
{
	uint8 r = check_detect_ar1019();
	
	tuner_printf("%s, r:%d\n", __FUNCTION__, r);
	return r;
}

static uint8 tuner_ar1019_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{				
	tuner_printf("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);
	
	if(ar1019_ctrl == NULL)
	{
		ar1019_ctrl = (ar1019_ctrl_t *)malloc_fun(ar1019_ctrl, sizeof(ar1019_ctrl_t), 0);
		if(ar1019_ctrl == NULL)
		{
			return FALSE;
		}
	}
	
	tuner_puts("ar1019_memory_ok\n");
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto ar1019_init_error;
			if(min_freq < 8750)goto ar1019_init_error;
			if(max_freq > 10800)goto ar1019_init_error;
			break;
			
		default:
			goto ar1019_init_error;
			break;
	}
	
	if(ar1019_ctrl->band == band)
	{
		if(ar1019_ctrl->init_succeed)
		{
			if(ar1019_ctrl->now_freq == freq)
			{
				ar1019_ctrl->min_freq = min_freq;
				ar1019_ctrl->max_freq = max_freq;
				if(!tuner_ar1019_setting(TUR_SET_MUTE, OFF))goto ar1019_init_error;
				return TRUE;
			}
			else
			{
				goto ar1019_init_freq;
			}
		}
		else 
		{
			goto ar1019_init_error;
		}
	}

	if(!ar1019_set_init())goto ar1019_init_error;
	
ar1019_init_freq:
	ar1019_ctrl->band = band;
	ar1019_ctrl->min_freq = min_freq;
	ar1019_ctrl->max_freq = max_freq;
	ar1019_ctrl->init_succeed = TRUE;
	
	engine_status_t status;
	if(!tuner_ar1019_search(freq, &status))goto ar1019_init_error;
	OSTimeDly(10);
	if(!tuner_ar1019_search(freq, &status))goto ar1019_init_error;
	if(!tuner_ar1019_setting(TUR_SET_MUTE, OFF))goto ar1019_init_error;
	tuner_puts("AR1019 Init Succeed!!!!!\n");
	return TRUE;
	
ar1019_init_error:
	free_fun((void **)&ar1019_ctrl);
	tuner_puts("AR1019 Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_ar1019_exit(void)
{
	tuner_puts("tuner_ar1019_exit\n");
	if(ar1019_ctrl)
	{
		ar1019_set_power(FALSE);
		free_fun((void **)&ar1019_ctrl);
	}
	return TRUE;
}

static uint8 tuner_ar1019_freq(uint16 freq)
{
    if(ar1019_ctrl == NULL)return FALSE;
    if(!ar1019_ctrl->init_succeed)return FALSE;
    if(freq < ar1019_ctrl->min_freq)return FALSE;
    if(freq > ar1019_ctrl->max_freq)return FALSE;
    
    ew_printf("%s, freq:%d\n", __FUNCTION__, freq);
    
    if(!turn_ar1019_seek(FALSE))return FALSE;
    if(freq == 8750)
    {
    	if(!ar1019_set_freq(freq,FALSE))return FALSE;
    	if(!ar1019_set_freq(freq,TRUE))return FALSE;
    }
    else
    {
    	if(!ar1019_set_freq(freq-5,FALSE))return FALSE;
    	if(!ar1019_set_seek_handle(TRUE))return FALSE;
    	if(!turn_ar1019_seek(TRUE))return FALSE;
    }
    
    return TRUE;
}

static uint8 tuner_ar1019_status(engine_status_t *status)
{		
	if(ar1019_ctrl == NULL)return FALSE;
	if(!ar1019_ctrl->init_succeed)return FALSE;
	
	status->freq 	= ar1019_ctrl->now_freq;
	status->signal  = 0;
	status->valid 	= FALSE;
	status->stereo 	= FALSE;
	status->band 	= RADIO_BAND_FM;
	status->stc		= FALSE;
		
	OSTimeDly(1);
	if(!ar1019_get_freq(&ar1019_ctrl->now_freq))return FALSE;
	status->freq = ar1019_ctrl->now_freq;
	status->stc = ar1019_ctrl->reg.Status.Stc;
	
	//ew_printf("Status.Stc:%d\n",ar1019_ctrl->reg.Status.Stc);
	
	if(ar1019_ctrl->reg.Status.Stc)
	{
		status->stereo = ar1019_ctrl->reg.Status.St;
		
		if(!Read_AR1019_RegMap(AR1019_REG_ADDR_RSSI, 1))return FALSE;
		status->signal = ar1019_ctrl->reg.Rssi.Rssi;
		
		if(status->freq == 8750)
		{
			if(ar1019_ctrl->reg.Rssi.Rssi < 25)
			{
				ar1019_ctrl->reg.Status.Sf = TRUE;
			}
		}
		if(!ar1019_ctrl->reg.Status.Sf
		 &&(ar1019_ctrl->reg.Rssi.Rssi  >= AR1019_TUNER_RSSI_FM)
		 &&(ar1019_ctrl->reg.Rssi.IfCnt > 242)
		 &&(ar1019_ctrl->reg.Rssi.IfCnt < 272))
		{
			if((status->freq%10) == 5)
			{
				if(ar1019_ctrl->reg.Rssi.Rssi  >= (AR1019_TUNER_RSSI_FM+5))
				{
					status->valid = TRUE;
				}
			}
			else
			{
				status->valid = TRUE;
			}
		}
	#if defined(_ENABLE_AR1019_RSSI_)
		else
		{
			if((ar1019_ctrl->reg.Rssi.Rssi	>= AR1019_TUNER_RSSI_FM)
			 &&(ar1019_ctrl->reg.Rssi.IfCnt > 242)
			 &&(ar1019_ctrl->reg.Rssi.IfCnt < 272))
			{
				status->valid = TRUE;
			}
		}
	#endif
	}
		
	//ew_printf("Status.freq:%d\n",ar1019_ctrl->now_freq);
	//ew_printf("Status.Sf:%d\n",ar1019_ctrl->reg.Status.Sf);
	//ew_printf("Rssi.Rssi:%d\n",ar1019_ctrl->reg.Rssi.Rssi);
	//ew_printf("Rssi.IfCnt:%d\n",ar1019_ctrl->reg.Rssi.IfCnt);
	
	return TRUE;
}

static uint8 tuner_ar1019_search(uint16 freq, engine_status_t *status)
{
    if(ar1019_ctrl == NULL)return FALSE;
    if(!ar1019_ctrl->init_succeed)return FALSE;
    if(freq < ar1019_ctrl->min_freq)return FALSE;
    if(freq > ar1019_ctrl->max_freq)return FALSE;
    
    ew_printf("%s, freq:%d\n", __FUNCTION__, freq);
    
    if(!tuner_ar1019_freq(freq))return FALSE;
    
    OSTimeDly(3);
    
    uint8 time_over = 50;
    do{
    	OSTimeDly(1);
    	if(!tuner_ar1019_status(status))return FALSE;
    }while(!status->stc && (--time_over));
    
    return TRUE;
}

static uint8 tuner_ar1019_setting(fm_setting_e type, void *set)
{
	if(ar1019_ctrl == NULL)return FALSE;
	if(!ar1019_ctrl->init_succeed)return FALSE;
	
	tuner_printf("%s, type:%d\n", __FUNCTION__, type);
	
	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!ar1019_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(!ar1019_set_stereo(stereo))return FALSE;
			}
			break;
			
		default:
			return FALSE;
	}
	return TRUE;
}

static uint8 tuner_ar1019_info(fm_info_e type, void *info)
{
	tuner_printf("%s, type:%d\n", __FUNCTION__, type);
	
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_AR1019;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = AR1019_TUNER_RSSI_FM;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = AR1019_TUNER_RSSI_LOC_FM;
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_ar1019_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_ar1019_deal_spot_tab;
			}
			break;
			
		case TUR_INFO_AUTOSEEK:
			if(info != NULL)
			{
				*(uint8 *)info = DISABLE;
			}
			break;
			
		case TUR_INFO_FM_ONLY:
			if(info != NULL)
			{
				*(uint8 *)info = ENABLE;
			}
			break;
			
		default:
			return FALSE;
	}
	return TRUE;
}


const tuner_drv_fun_t tuner_ar1019_drv_fun =
{
	tuner_ar1019_detect,
	tuner_ar1019_init,
	tuner_ar1019_exit,
	tuner_ar1019_freq,
	tuner_ar1019_status,
	tuner_ar1019_search,
	NULL,
	tuner_ar1019_setting,
	tuner_ar1019_info,
};

#endif

