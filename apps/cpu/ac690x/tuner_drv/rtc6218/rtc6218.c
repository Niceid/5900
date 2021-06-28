#include "rtc6218/rtc6218.h"
#include "fm_band.h"

#if defined(_TUNER_DRIVER_RTC6218_)
static rtc6218_ctrl_t *rtc6218_ctrl = NULL;
static const uint16 fm_rtc6218_deal_spot_tab[] = {9600};
extern volatile u32 g_rand_seed;
static const uint16 am_rtc6218_deal_spot_tab[] = {};
uint8 TuneResult = 0;
u16 loop_counter = 0;

static const  uint8 RTC62XX_turnoff[30]=
{
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x16,0xAA
};

static const  uint8 RTC62XX_turnon[30]=
{
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x96,0xAA
};

#if defined(_SUPPORT_FREQ_JAPAN_)
static const  uint8  RTC62XX_power_up[16]=
{
    0x40,0x3f,0x14,0x00,0x00,0x00,0x00,0x0c,0x80,0x05,0xB4,0x00,0x00,0x00,
    0x40,0x50
};  // last byte is spike threshold, the larger the more, recommand under 0x50, default 0x10
#else
static const  uint8  RTC62XX_power_up[16]=
{
    0x40,0x3f,0x04,0x00,0x00,0x00,0x00,0x0c,0x80,0x05,0xB4,0x00,0x00,0x00,
    0x40,0x50
};  // last byte is spike threshold, the larger the more, recommand under 0x50, default 0x10
#endif
//改停台阀值在powerup数组最后两个字节，都是越大台越多，默认是0x40，0x50，
//0x01(12M)0x00(32.768)

//从02寄存器开始，每个寄存器两个字节
#if defined(_SUPPORT_FREQ_JAPAN_)
static const  uint8  RTC62XX_Initial_value[14]=
{
	0x40,0x3f,0x14,0x00,0x00,0x00,0x00,0x0c,0x80,0x05,0xB4,0x00,0xe0,0x00
};	// RSSI_TH = 0x0c
#else
static const  uint8  RTC62XX_Initial_value[14]=
{
	0x40,0x3f,0x04,0x00,0x00,0x00,0x00,0x0c,0x80,0x05,0xB4,0x00,0xe0,0x00
};	// RSSI_TH = 0x0c
#endif
//0x01(12M);0x00(32.768);0x05(24M)(倒数第五个)


static const  uint8  RTC62XX_Initial_value1[14] =
{
    0x00,0x72,0x00,0xFF,0x00,0x1F,0x03,0xFF,0x56,0xD1,0x13,0xB7,0x00,0x00
};

static const  uint8  RTC62XX_CKHopping[46] =
{
	0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x70,0x00, 0x20,0x00,
	0x05,0x0F, 0x0E,0x85, 0x5A,0xA6, 0xDC,0x57, 0x80,0x00, 0x00,0xA3, 0x00,0xA3,
	0xD0,0x08, 0x7F,0x80, 0x3C,0x08, 0xA6,0xCF, 0x81,0x00, 0x00,0x00, 0x01,0x40,
	0x47,0x00, 0x00,0x00
};
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 send_rtc6218_reg(const uint8 *buffer, uint8 length)
{	
	i2c_data_init(RTC6218_I2C_ADDR, RTC6218_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(buffer, length);
	i2c_data_exit();
	return result;
}

static uint8 read_rtc6218_reg(uint8 *buffer, uint8 length)
{
	i2c_data_init(RTC6218_I2C_ADDR, RTC6218_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_r(buffer, length);
	i2c_data_exit();
	return result;
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
static uint8 rtc6218_set_vol(uint8 flag)
{
	uint8 voltemp[2]= {0x40,0x3F};
	send_rtc6218_reg(&(voltemp[0]),2);
	ew_printf("%s, voltemp:%d \n", __FUNCTION__, voltemp);
	return TRUE;
}
static uint8 rtc6218_set_mute(uint8 mute)
{
	uint8 voltemp[2] ={0x00,0x3F}; 
	if(!mute)
		voltemp[0] = 0x40;
	if(!send_rtc6218_reg(&(voltemp[0]),2)) return FALSE;
	return TRUE;
	
}

static uint8 rtc6218_get_stereo(void)
{
	uint8 RTC6218_reg_St[2];
	read_rtc6218_reg(&(RTC6218_reg_St[0]), 2);
	if(RTC6218_reg_St[0]&0x04)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
static uint8 rtc6218_set_stereo(uint8 stereo)
{
	uint8 TempData[2]={0x40,0x3F};
	
	if(!stereo)
    {
        TempData[0]=0x60;
    }
	if(!send_rtc6218_reg(&(TempData[0]), 2))return FALSE;
	return TRUE;
}
static uint8 RTC6218_Read_ID(void)  //读id
{
    u8 RTC62XX_reg_data[14];
    read_rtc6218_reg(&(RTC62XX_reg_data[0]), 14);
	ew_printf("Read RTC62XX_reg_data[12]:0x%x\n",RTC62XX_reg_data[12]);
	ew_printf("Read RTC62XX_reg_data[13]:0x%x\n",RTC62XX_reg_data[13]);

    if(((RTC62XX_reg_data[12] << 8) | RTC62XX_reg_data[13])==0x1688)
    {      
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static uint8 SetRTC6218PowerUp(void)
{
	if(!send_rtc6218_reg((u8 *)&(RTC62XX_power_up[0]), 16))return FALSE;//power up
	return TRUE;
}
static uint8 rtc6218_get_rssi(uint8 *rssi)
{
	uint8 RTC6218_reg_rssi[4];
	if(!read_rtc6218_reg(&(RTC6218_reg_rssi[0]), 4))return FALSE;
	*rssi = RTC6218_reg_rssi[3];
    return TRUE;
}

static uint8 rtc6218_set_init(void)
{
	ew_puts("rtc6218_set_init\n");
	if(!send_rtc6218_reg((u8 *)&(RTC62XX_turnoff[0]), 30))return FALSE;//power off
	OSTimeDly(2);
	if(!send_rtc6218_reg((u8 *)&(RTC62XX_turnon[0]), 30))return FALSE;
	OSTimeDly(2);
	if(!send_rtc6218_reg((u8 *)&(RTC62XX_CKHopping[0]),46))return FALSE;
	OSTimeDly(2);
	if(!send_rtc6218_reg((u8 *)&(RTC62XX_power_up[0]), 16))return FALSE;//power up
	OSTimeDly(10);
	ew_puts("rtc6218_set_init--OK\n");
	return TRUE;
	
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_rtc6218_detect(void);
static uint8 tuner_rtc6218_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_rtc6218_exit(void);
static uint8 tuner_rtc6218_freq(uint16);
static uint8 tuner_rtc6218_status(engine_status_t *);
static uint8 tuner_rtc6218_search(uint16, engine_status_t *);
static uint8 tuner_rtc6218_setting(fm_setting_e, void *);
static uint8 tuner_rtc6218_info(fm_info_e, void *);


static uint8 tuner_rtc6218_detect(void)
{	
	uint8 result;
	
	i2c_data_init(RTC6218_I2C_ADDR, RTC6218_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	i2c_data_exit();
	
	ew_printf("tuner_rtc6218_detect:%d\n", result);
	return result;
}

static uint8 tuner_rtc6218_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{

	ew_printf("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);

	if(rtc6218_ctrl == NULL)
	{
		rtc6218_ctrl = (rtc6218_ctrl_t *)malloc_fun(rtc6218_ctrl, sizeof(rtc6218_ctrl_t), 0);
		if(rtc6218_ctrl == NULL)return FALSE;
	}
	
	ew_puts("rtc6218_memory_ok\n");
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto rtc6218_init_error;
			if(min_freq < 7600)goto rtc6218_init_error;
			if(max_freq > 10800)goto rtc6218_init_error;
			break;
		default:
			goto rtc6218_init_error;
			break;
	}
		
	if(rtc6218_ctrl->tuner_band == band)
	{
		if(rtc6218_ctrl->init_succeed)
		{
			if(rtc6218_ctrl->now_freq == freq)
			{
				rtc6218_ctrl->min_freq = min_freq;
				rtc6218_ctrl->max_freq = max_freq;
				if(!tuner_rtc6218_setting(TUR_SET_MUTE, OFF))goto rtc6218_init_error;
				return TRUE;
			}
			else
			{
				goto rtc6218_init_freq;
			}
		}
		else 
		{
			goto rtc6218_init_error;
		}
	}
	
	if(!rtc6218_set_init())goto rtc6218_init_error;
rtc6218_init_freq:
	rtc6218_ctrl->tuner_band 	= band;
	rtc6218_ctrl->min_freq 		= min_freq;
	rtc6218_ctrl->max_freq 		= max_freq;
	rtc6218_ctrl->init_succeed 	= TRUE;
	
	engine_status_t status;
	if(!tuner_rtc6218_search(freq, &status))goto rtc6218_init_error;
	if(!tuner_rtc6218_setting(TUR_SET_MUTE, OFF))goto rtc6218_init_error;
	ew_puts("RTC6218 Init Succeed!!!!!\n");
	return TRUE;
	
rtc6218_init_error:
	free_fun((void **)&rtc6218_ctrl);
	ew_puts("RTC6218 Init Fail!!!!!\n");
	
	return FALSE;
}

static uint8 tuner_rtc6218_power_off(void)
{
	if(!send_rtc6218_reg((u8 *)&(RTC62XX_turnoff[0]), 30))return FALSE;//power off
	return TRUE;
}

static uint8 tuner_rtc6218_exit(void)
{
	ew_puts("tuner_rtc6218_exit\n");
	
	if(rtc6218_ctrl)
	{
		tuner_rtc6218_power_off();
		free_fun((void **)&rtc6218_ctrl);
	}
	return TRUE;
}

static uint8 tuner_rtc6218_freq(uint16 freq)
{
	u8 RTC6218_reg_data[24]={0};
	u16  freq_reg_data;
#if defined(_SUPPORT_FREQ_JAPAN_)
    u8	RTC6218_channel_start_tune[] = {0x40,0x3f,0x94,0xCA};	//108.0MHz
    u8	RTC6218_channel_stop_tune[] = {0x40,0x3f,0x14,0xCA};
#else
    u8  RTC6218_channel_start_tune[] = {0x40,0x3f,0x84,0xCA};	//108.0MHz
    u8  RTC6218_channel_stop_tune[] = {0x40,0x3f,0x04,0xCA};
#endif

	ew_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if(rtc6218_ctrl == NULL)return FALSE;
	if(!rtc6218_ctrl->init_succeed)return FALSE;
	if(freq < rtc6218_ctrl->min_freq)return FALSE;
	if(freq > rtc6218_ctrl->max_freq)return FALSE;
	
    //my_printf("Freq=%d\n", Freq);
#if defined(_SUPPORT_FREQ_JAPAN_)
	freq_reg_data = (freq - 7600)/(100/10);
#else
    freq_reg_data = (freq - 8750)/(100/10);
#endif
    RTC6218_channel_start_tune[3] = freq_reg_data & 0xff;
    RTC6218_channel_start_tune[2] = (RTC6218_channel_start_tune[2] & 0xfc) | (freq_reg_data >> 8);
    RTC6218_channel_stop_tune[3] = RTC6218_channel_start_tune[3] ;

	RTC6218_channel_start_tune[2]&= 0x7f;
    send_rtc6218_reg(&(RTC6218_channel_start_tune[0]), 4);
	RTC6218_channel_start_tune[2]|= 0x80;
    send_rtc6218_reg(&(RTC6218_channel_start_tune[0]), 4);
    //wait STC=1
   do
    {
        read_rtc6218_reg(&(RTC6218_reg_data[0]), 2);
		OSTimeDly(1);
        loop_counter++;
    }
 	while(((RTC6218_reg_data[0]&0x40) == 0) && (loop_counter < 60));		//for loop_counter, when tune, the loop time must > 60ms

    if(loop_counter == 60)
        return 0;
    loop_counter = 0;
	
	//ew_printf("  loop_counter1:%d--- \n", loop_counter);
	
    //clear tune bit
    send_rtc6218_reg(&(RTC6218_channel_stop_tune[0]), 4);
    //wait STC=0
    do
    {
       read_rtc6218_reg(&(RTC6218_reg_data[0]), 2);
       loop_counter++;
   }
   while(((RTC6218_reg_data[0]&0x40) != 0) && (loop_counter < 60));

   if(loop_counter == 60)
		   return 0;
   
   //ew_printf("  loop_counter2:%d--- \n", loop_counter);
   
	if(!send_rtc6218_reg((u8 *)&(RTC62XX_Initial_value[0]), 14))return FALSE;
	if(!read_rtc6218_reg(&(RTC6218_reg_data[0]), 24))return FALSE;
	if(!send_rtc6218_reg((u8 *)&RTC62XX_Initial_value1[0], 14))return FALSE;
	if((RTC6218_reg_data[22]&0x80)!= 0)
	{
		TuneResult = 0;
	}
	else
	{
		TuneResult = 1;
	}
	//ew_printf("  RTC6218_reg_data[22]:0x%x--- \n", RTC6218_reg_data[22]);
	//ew_printf("  TuneResultXX:%d--- \n", TuneResult);
	rtc6218_ctrl->now_freq = freq;
	//ew_puts("tuner_rtc6218_freq----succeed\n");
	return TRUE;
}

static uint8 tuner_rtc6218_status(engine_status_t *status)
{
	//ew_printf("%s\n", __FUNCTION__);
	if(rtc6218_ctrl == NULL)return FALSE;
	if(!rtc6218_ctrl->init_succeed)return FALSE;
	status->stc 	= FALSE;
	status->stereo 	= FALSE;
	status->signal 	= 0;
	status->valid	= FALSE;
	status->freq 	= rtc6218_ctrl->now_freq;
	status->band = RADIO_BAND_FM;
	status->stc = TRUE;
	
#if !defined(_DISABLE_ST_)
	uint16 st;
#endif
	uint8 rssi;
	uint8 index;
	
	OSTimeDly(1);
	
	for(index = 0; (index < 4) && (status->valid == 0); index++)
	{
	#if defined(_DISABLE_ST_)||defined(ST_READ_DELAY_)
		if(GET_RADIO_STATE != RADIO_STATE_IDLE)
	#endif
		{
			if(!rtc6218_get_rssi(&rssi))return FALSE;
		}
	#if !defined(_DISABLE_ST_)
	#if defined(ST_READ_DELAY_)
		global.st_read_time = 30000/500;
		if(global.st_read_flag)
		{
			st = rtc6218_get_stereo();
		}
	#else
		st = rtc6218_get_stereo();
	#endif
	#endif
		status->signal = rssi;
		status->valid = FALSE;
		status->stereo= FALSE;
	//valid
		if((rssi>=RTC6218_TUNER_RSSI_FM)&&TuneResult) 
		{
			status->valid = TRUE;
		#if !defined(_DISABLE_ST_)
			if(st)
			{
				status->stereo = TRUE;
			}
		#endif
		}
	}	
	ew_printf("  TuneResult:%d---rssi_xx:%d \n", TuneResult,rssi);
	return TRUE;
}

static uint8 tuner_rtc6218_search(uint16 freq, engine_status_t *status)
{
	ew_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if(rtc6218_ctrl == NULL)return FALSE;
	if(!rtc6218_ctrl->init_succeed)return FALSE;
	if(freq < rtc6218_ctrl->min_freq)return FALSE;
	if(freq > rtc6218_ctrl->max_freq)return FALSE;
	
	if(!tuner_rtc6218_freq(freq))return FALSE;
	uint8 index;
	for(index = 0; index < 50; index++)
	{	
		if(!tuner_rtc6218_status(status))return FALSE;
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

static uint8 tuner_rtc6218_setting(fm_setting_e type, void *set)
{
	ew_printf("%s, type:%d\n", __FUNCTION__, type);
	if(rtc6218_ctrl == NULL)return FALSE;
	if(!rtc6218_ctrl->init_succeed)return FALSE;
	
	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!rtc6218_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(!rtc6218_set_stereo(stereo))return FALSE;
				else return FALSE;
			}
			break;
			
		default:
			return FALSE;
	}
	ew_printf("%s---->succeed\n", __FUNCTION__);
	
	return TRUE;
}
static uint8 tuner_rtc6218_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_RTC6218;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = RTC6218_TUNER_RSSI_FM;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = RTC6218_TUNER_RSSI_LOC_FM;
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_rtc6218_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_rtc6218_deal_spot_tab;
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

const tuner_drv_fun_t tuner_rtc6218_drv_fun =
{
	tuner_rtc6218_detect,
	tuner_rtc6218_init,
	tuner_rtc6218_exit,
	tuner_rtc6218_freq,
	tuner_rtc6218_status,
	tuner_rtc6218_search,
	NULL,
	tuner_rtc6218_setting,
	tuner_rtc6218_info,
};

#endif

