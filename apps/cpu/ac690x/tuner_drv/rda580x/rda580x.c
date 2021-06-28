#include "rda580x/rda580x.h"
#if defined(_YH1701_AUTOSEEK_MEMORY_)
#include "setup.h"
#endif

#if defined(_TUNER_DRIVER_RDA580x_)

static rda580x_ctrl_t *rda580x_ctrl = NULL;
static const uint16 fm_rda580x_deal_spot_tab[] = {9600};
extern volatile u32 g_rand_seed;

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 send_byte_to_rda580x(const uint8 *buffer, uint8 length)
{
	i2c_data_init(RDA580x_I2C_ADDR, RDA580x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(buffer, length);
	i2c_data_exit();
	return result;
}

static uint8 send_word_to_rda580x(const uint16 *buffer, uint8 length)
{
	i2c_data_init(RDA580x_I2C_ADDR, RDA580x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_ww(buffer, length);
	i2c_data_exit();
	return result;
}

static uint8 send_data_to_rda580x(uint8 length)
{
	i2c_data_init(RDA580x_I2C_ADDR, RDA580x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_ww((uint16 *)&rda580x_ctrl->reg.reg2, length);
	i2c_data_exit();
	return result;
}

static uint8 read_data_for_rda580x(uint8 length)
{
	i2c_data_init(RDA580x_I2C_ADDR, RDA580x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_wr((uint16 *)&rda580x_ctrl->reg.reg10, length);
	i2c_data_exit();
	return result;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static const uint16 rda580x_init_reg[]=
{//炬力提供数据
    0x8405,		//02H
    0x1A00,
    0x0400,
    0x88AF,		//05h
    0x6000,
	0x32C6,
    0x0000,
    0x0000,
    0x4468,
    0x5d8f,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0xf017,		//10h
    0x4779,
    0x2d00,
    0xb042,
    0x2211,
    0xf831,		//15h
    0xc3c4,
    0x1fce,
    0x9500,		//0x94,0x12,//18h
    0x00a8,
    0xc400,
    0xe7df,
    0x34dc,
    0x816a,
    0x4608,
    0x0086,
    0x0661,		//20h
    0x0000,
    0x109e,
    0x23c8,
    0x0406,
    0x060b,		//25h
    0xe105,
    0xbb6c,
    0x2bec,
    0x090f,
    0x3414,
    0x1450,
    0x096d,
    0x2d96,
    0x01da,
    0x2a7b,
    0x0821,		//30h
    0x13d5,
    0x48f1,
    0x00bc,
    0x0820,
    0x0a24,		//35h
    0x0b82,
    0x25c7,
    0x2be0,
    0x3edd,
    0x2cb9,
    0x0c95,
    0x088d,
    0x0485,
    0x0080,
    0x0000,
    0x5804,		//40h
    0x5804,
    0x3835,
    0x2f2b,
    0x2722,
    0x1b15,
    0x0f08,
    0x03fd,
    0xfdfd,
    0x17af,
    0xffff,
    0xff00,
    0x2e94,
    0xc84e,
    0x00fd,
    0xd498,
};

static const uint16 ew_rda5801_init_reg[]= 
{
#if defined(_ENABLE_FREQ_24MHZ_)
	0xC051,
#else
	0xC001,
#endif
	0x0012,		//50KHz 
#if defined(_ENABLE_RADIO_SOFTMUTE)
	0x0600,// 打开softmute
#else
	0x0400,
#endif
	0x86BF, 	//05H     
	0x4000,
	0x56C6,
	0x0000,
	0x0000,
	0x0000, 	//0AH
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0000,
	0x0006,  	//10H
	0x0019, 
	0x2A11,
	0x002E,
	0x2A30,
	0xB83C,  	//15H
	0x9000,
	0x2A91,
	0x8412,
	0x00A8,
	0xC400,		//AH
	0xE000,
	0x301D,
	0x816A,
	0x4608,
	0x0086,    	// 1FH
	0x0661,		//20H
	0x0000,
	0x109E,
	0x26CD,
	0x0408,
	0x0608, 
	0xE105,
	0x3B6C,
	0x2BEC,
	0x090F,
	0x3414,		// 2AH
	0x1450,
	0x096D,
	0x2D96,
	0x01DA,
	0x2A7B,
	0x0821,    	//30H
	0x13D5,
	0x4891,
	0x00bc,
	0x0896,   	//34H
	0x153C,   	//35H
	0x0B80,   	//36H
	0x25C7,   	//37H
	0x0000,		//38H
	0x3C58,
	0x2A50,
	0x262C,
	0x0608,
	0x0200,
	0x0000,
};

static const uint16 ew_rda5802_init_reg[]=
{
#if defined(_ENABLE_FREQ_24MHZ_)
	0xC051,
#else
	0xC001,
#endif
	0x0012,        
#if defined(_ENABLE_RADIO_SOFTMUTE)
	0x0600,// 打开softmute
#else
	0x0400,
#endif
	0x88AF, 	//05H: 
	0x0000,        
	0x5EC6,        
	0x5096,        
	0x0000,        
	0x4000, 	//0AH: 
	0x008F,        
	0x0000,        
	0x0000,        
	0x0000,        
	0x0000,        
	0xF005, 	//10H: 
	0x9000,        
	0xF484,        
	0x7001,        
	0x40F0,        
	0x2180, 	//15H: 
	0x7AD0,        
	0x3E40,        
	0x55A9,        
	0xE848,        
	0x5080, // 
	0x0000,        
	0x37B6,        
	0x400C,        
	0x079B,        
	0x4C1D,        
	0x8111,		//20H: 
	0x45C0,        
};

static const uint16 ew_rda5804_init_reg[]= 
{
#if defined(_ENABLE_FREQ_24MHZ_)
	0xC051,
#else
	0xC001,
#endif
	0x1b92,
	0x0C00,
	0x86ad,
	0x8000,
	0x5F1A,
	0x5ec6,
	0x0000,
	0x406e,
	0x2d80,
	0x5803,
	0x5804,
	0x5804,
	0x5804,
	0x0047,
	0x9000,
	0xF587,
	0x7F0B,
	0x04F1,
	0x5Ec0,
	0x41e0,
	0x506f,
	0x5592,
	0x007d,
	0x10C0,//1
	0x0780,
	0x411d,//1	
	0x4006,
	0x1f9B,
	0x4c2b,//1
	0x8110, 	//20H: 
	0x45a0,		// 21H
	0x553F, 	//22H
	0xaf40, 
	0x0681,
	0x1b2a,		//25H
	0x0D04,
	0x809F, 
	0x178A,
	0xD349,
	0x1142,
	0xA0C4,
	0x3EBB,
	0x0000,
	0x5804,
	0x5804,
	0x5804,
	0x0074,
	0x3D00,
	0x030C,
	0x2F68,
	0x3877, 	//35H
};

static const uint16 ew_rda5808_init_reg[]= 
{
#if defined(_ENABLE_FREQ_24MHZ_)
	0xC051,
#else
	0xC001,
#endif
    0x0012,
#if defined(_ENABLE_RADIO_SOFTMUTE)
	0x0600,// 打开softmute
#else
	0x0400,
#endif
    0x88ad,		//05h	 0xC6ad
    0x6000,
    0X32C6,
    0x0000,
    0x0000,
    0x0000,		//0x0ah
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,		//0x10h
    0x0019,
    0x2a11,
    0xB042,  
    0x2A11,  
    0xb831,		//0x15h 
    0xc000,
    0x2a91,
    0x9400,
    0x00a8,
    0xc400,		//0x1ah
    0xF7cF,   
    0x1214,		//0x1ch0x2ADC//2016.06.02修改高温5807收音搜不到台
    0x806F, 
    0x4608,
    0x0086,
    0x0661,		//0x20H
    0x0000,
    0x109E,
    0x23C8,
    0x0406,
    0x0E1C,		//0x25H
};

static const uint16 rdabt_fm580x_init[][2] =
{
    {0x3F,0x0001},//page 1
    {0x2D,0x002A},//page 1
    {0x3F,0x0000},//page 1
};

static const uint16 rdabt_fm580x_deinit[][2] =
{
    {0x3F,0x0001},//page 1
    {0x2D,0x00AA},//page 1
    {0x3F,0x0000},//page 1
};

static uint8 rda580x_trans_one_reg(uint8 i2c_addr, uint8 reg_addr, uint16 data)
{
	uint8 trans_buf[4];
	
    //加入设备地址
    trans_buf[0] = i2c_addr;
	//寄存器地址
    trans_buf[1] = reg_addr;
    //寄存器内容高字节
    trans_buf[2] = _u16to8h_(data);
    //寄存器内容低字节
    trans_buf[3] = _u16to8l_(data);
	
	return send_byte_to_rda580x(trans_buf, 4);
}

static void radbt_fm580x_patch_on(void)
{
    uint8 i;
    //写特殊地址

    for(i = 0; i < 3; i++)
    {
		rda580x_trans_one_reg(RDA580x_I2C_SPECIAL_ADDR, (uint8)rdabt_fm580x_init[i][0], rdabt_fm580x_init[i][1]);
    }
	
    OSTimeDly(1);
}

static void radbt_fm580x_patch_off(void)
{
    uint8 i;
    //写特殊地址
	
    for(i = 0; i < 3; i++)
    {
		rda580x_trans_one_reg(RDA580x_I2C_SPECIAL_ADDR, (uint8)rdabt_fm580x_deinit[i][0], rdabt_fm580x_deinit[i][1]);
    }
	
    OSTimeDly(1);
}

static uint8 rda580x_init_start(const uint16 *tab_buf, uint8 length)
{
	uint8 i;
	
	//寄存器设置
    for(i = 0; i < 6; i++)
    {
		((uint16 *)&rda580x_ctrl->reg.reg2)[i] = tab_buf[i];
    }
	
    //start
    rda580x_ctrl->reg.reg2.enable = ENABLE;
	rda580x_ctrl->reg.reg2.soft_reset = DISABLE;
	if(!send_data_to_rda580x(1))return FALSE;
    OSTimeDly(1);
	
	//reset fm module
	rda580x_ctrl->reg.reg2.enable = DISABLE;
	rda580x_ctrl->reg.reg2.soft_reset = ENABLE;
	if(!send_data_to_rda580x(1))return FALSE;
    OSTimeDly(1);
	
	//start
    rda580x_ctrl->reg.reg2.enable = ENABLE;
	rda580x_ctrl->reg.reg2.soft_reset = DISABLE;
	if(!send_data_to_rda580x(1))return FALSE;
	
	//delay50ms
	OSTimeDly(5);
	
    //intialization
	if(!send_word_to_rda580x(tab_buf, length))return FALSE;
	
	return TRUE;
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

static uint8 rda580x_set_init(void)
{
	tuner_puts("rda580x_set_init\n");
	
	radbt_fm580x_patch_on();
	
	//reset fm module
	rda580x_ctrl->reg.reg2.enable 		= DISABLE;
	rda580x_ctrl->reg.reg2.soft_reset 	= ENABLE;
	if(!send_data_to_rda580x(1))return FALSE;
	tuner_puts("rda580x_set_init----111----\n");
	
    //delay50ms
    OSTimeDly(5);
	
	//读10个寄存器
   	if(!read_data_for_rda580x(5))return FALSE;
	tuner_printf("rda580x_set_init----222----, device_id:0x%x\n", rda580x_ctrl->reg.reg14_device_id);
	
	if(rda580x_ctrl->reg.reg14_device_id == 0x5801) 
	{//5801
		if(!rda580x_init_start(ew_rda5801_init_reg, sizeof(ew_rda5801_init_reg)/2))return FALSE;
	}	
	else if((rda580x_ctrl->reg.reg14_device_id == 0x5802)||(rda580x_ctrl->reg.reg14_device_id == 0x5803)) 
	{//5802 5803
		if(!rda580x_init_start(ew_rda5802_init_reg, sizeof(ew_rda5802_init_reg)/2))return FALSE;
	}
	else if(rda580x_ctrl->reg.reg14_device_id == 0x5804) 
	{//5804
		if(!rda580x_init_start(ew_rda5804_init_reg, sizeof(ew_rda5804_init_reg)/2))return FALSE;
	}
	else if(rda580x_ctrl->reg.reg14_device_id == 0x5808) 
    {//5808
		if(!rda580x_init_start(ew_rda5808_init_reg, sizeof(ew_rda5808_init_reg)/2))return FALSE;
    }
	else return FALSE;
	
	return TRUE;
}

static uint8 rda580x_power_down(void)
{
	if(!rda580x_ctrl->init_succeed)return FALSE;
	
	//sys_mute_ctrl(ENABLE, 300);// 退出收音有杂音
	rda580x_ctrl->reg.reg2.hiz_dis = 0;
	rda580x_ctrl->reg.reg2.mute_dis = 0;
	rda580x_ctrl->reg.reg2.enable = DISABLE;
	if(!send_data_to_rda580x(1))return FALSE;
	
	OSTimeDly(1);
	radbt_fm580x_patch_off();
	return TRUE;
}

static uint8 rda580x_set_mute(uint8 mute)
{
	uint8 high_z,mute_v;
	
	if(mute)
    {//设置解除静音
        high_z = 1;
        mute_v = 0;
    }
    else
    {//设置静音
        high_z = 1;
        mute_v = 1;
    }
	
	rda580x_ctrl->reg.reg2.hiz_dis = high_z;
	rda580x_ctrl->reg.reg2.mute_dis= mute_v;
	
	if(!send_data_to_rda580x(1))return FALSE;
	
	return TRUE;
}

static uint8 rda580x_set_stereo(uint8 stereo)
{
	uint8 st_v;
		
	if(stereo)
    {
        st_v = FALSE;
    }
    else
    {
        st_v = TRUE;
    }
	rda580x_ctrl->reg.reg2.mono = st_v;
	
	if(!send_data_to_rda580x(1))return FALSE;
	
	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_rda580x_detect(void);
static uint8 tuner_rda580x_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_rda580x_exit(void);
static uint8 tuner_rda580x_freq(uint16);
static uint8 tuner_rda580x_status(engine_status_t *);
static uint8 tuner_rda580x_search(uint16, engine_status_t *);
static uint8 tuner_rda580x_hard(engine_hard_t *);
static uint8 tuner_rda580x_setting(fm_setting_e, void *);
static uint8 tuner_rda580x_info(fm_info_e, void *);


static uint8 tuner_rda580x_detect(void)
{	
	uint8 result;
	
	i2c_data_init(RDA580x_I2C_ADDR, RDA580x_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	i2c_data_exit();
	
	tuner_printf("tuner_rda580x_detect:%d\n", result);
	return result;
}

static uint8 tuner_rda580x_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	tuner_printf("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);

	if(rda580x_ctrl == NULL)
	{
		rda580x_ctrl = (rda580x_ctrl_t *)malloc_fun(rda580x_ctrl, sizeof(rda580x_ctrl_t), 0);
		if(rda580x_ctrl == NULL)return FALSE;
	}
	
	tuner_puts("rda580x_memory_ok\n");
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto rda580x_init_error;
			if(min_freq < 8700)goto rda580x_init_error;
			if(max_freq > 10800)goto rda580x_init_error;
			break;
			
		default:
			goto rda580x_init_error;
			break;
	}
		
	if(rda580x_ctrl->tuner_band == band)
	{
		if(rda580x_ctrl->init_succeed)
		{
			if(rda580x_ctrl->now_freq == freq)
			{
				rda580x_ctrl->min_freq		= min_freq;
				rda580x_ctrl->max_freq		= max_freq;
				if(!tuner_rda580x_setting(TUR_SET_MUTE, OFF))goto rda580x_init_error;
				return TRUE;
			}
			else
			{
				goto rda580x_init_freq;
			}
		}
		else 
		{
			goto rda580x_init_error;
		}
	}
	
	uint8 init_count = 10;
	while(--init_count)
	{
		if(!rda580x_set_init())goto rda580x_init_error;
		OSTimeDly(5);
		
		uint8 index = 20;
		while(--index)
		{
			if(!read_data_for_rda580x(1))goto rda580x_init_error;
			
			if(rda580x_ctrl->reg.reg10.stc)break;
			
			OSTimeDly(1);
		}
		if(index)break;
	}
	if(!init_count)goto rda580x_init_error;
	
rda580x_init_freq:
	rda580x_ctrl->tuner_band 	= band;
	rda580x_ctrl->min_freq 		= min_freq;
	rda580x_ctrl->max_freq 		= max_freq;
	rda580x_ctrl->init_succeed 	= TRUE;
	
	engine_status_t status;
	if(!tuner_rda580x_search(freq, &status))goto rda580x_init_error;
	//if(!tuner_rda580x_setting(TUR_SET_MUTE, OFF))goto rda580x_init_error;
	
	//OSTimeDly(10);
	//if(!tuner_rda580x_search(freq, &status))goto rda580x_init_error;
	if(!tuner_rda580x_setting(TUR_SET_MUTE, OFF))goto rda580x_init_error;
	tuner_puts("RDA580x Init Succeed!!!!!\n");
	return TRUE;
	
rda580x_init_error:
	free_fun((void **)&rda580x_ctrl);
	tuner_puts("RDA580x Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_rda580x_exit(void)
{
	tuner_puts("tuner_rda580x_exit\n");
	
	if(rda580x_ctrl)
	{
		rda580x_power_down();
		free_fun((void **)&rda580x_ctrl);
	}
	
	return TRUE;
}

static uint8 tuner_rda580x_freq(uint16 freq)
{
	tuner_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if(rda580x_ctrl == NULL)return FALSE;
	if(!rda580x_ctrl->init_succeed)return FALSE;
	if(freq < rda580x_ctrl->min_freq)return FALSE;
	if(freq > rda580x_ctrl->max_freq)return FALSE;
	
	uint16 freq_chan;
	rda580x_tuner_band_e band;
	rda580x_tuner_span_e span;
	
	if(freq < 7600)
	{//OIRT
		band = RDA580x_TUNER_BAND_OIRT;
		span = RDA580x_TUNER_SPAN_50KHZ;
		freq_chan = (freq - 6500)/5;
	}
	else 
	{//世界频率
		band = RDA580x_TUNER_BAND_WORD;
		span = RDA580x_TUNER_SPAN_50KHZ;
		freq_chan = (freq - 7600)/5;
	}
	
	//set freq
	rda580x_ctrl->reg.reg3.chan = freq_chan;

	//set band
	rda580x_ctrl->reg.reg3.band = band;

	//set span
	rda580x_ctrl->reg.reg3.span = span;

	//set tune
	rda580x_ctrl->reg.reg3.tune = TRUE;

	//set soft mute
	//rda580x_ctrl->reg.reg4.soft_mute = TRUE;
	
	if(!send_data_to_rda580x(2))return FALSE;
	
	rda580x_ctrl->now_freq = freq;
	rda580x_ctrl->last_sys_timer = g_rand_seed;
	rda580x_ctrl->freq_change_flag = TRUE;
	rda580x_ctrl->freq_rssi_max = 0;
	
	tuner_puts("tuner_rda580x_freq----succeed\n");
	return TRUE;
}

static uint8 tuner_rda580x_status(engine_status_t *status)
{
	//tuner_printf("%s\n", __FUNCTION__);
	if(rda580x_ctrl == NULL)return FALSE;
	if(!rda580x_ctrl->init_succeed)return FALSE;
	
	status->stc 	= FALSE;
	status->stereo 	= FALSE;
	status->signal 	= 0;
	status->valid	= FALSE;
	status->freq 	= rda580x_ctrl->now_freq;
	status->band 	= RADIO_BAND_FM;
	
	if(rda580x_ctrl->freq_change_flag)
	{
		if((g_rand_seed - rda580x_ctrl->last_sys_timer) < (30/2))
		{
			return TRUE;
		}
		else
		{
			rda580x_ctrl->freq_change_flag = FALSE;
		}
	}

	if(!read_data_for_rda580x(2))return FALSE;
	
	//seek complete
    if(rda580x_ctrl->reg.reg10.stc)
    {
		status->stc = TRUE;
	}
	else
	{
		status->stc = FALSE;
	}

	//intensity
	status->signal = rda580x_ctrl->reg.reg11.rssi;
	
#if defined(_YH1701_AUTOSEEK_MEMORY_)
	setup.rssi = status->signal;
#endif

    //stereo
	if(rda580x_ctrl->reg.reg10.st)
	{
	    if(status->signal >= RDA580x_TUNER_RSSI_FM)
	    {
			status->stereo = TRUE;
		}
	}
	else
	{
		status->stereo = FALSE;
	}
	
	status->valid = rda580x_ctrl->reg.reg11.valid;

	if(status->valid)
	{
		if(status->stc)
		{
			if(status->signal > (rda580x_ctrl->freq_rssi_max))
			{
				rda580x_ctrl->freq_rssi_max = status->signal;
				status->stc = FALSE;
			}
		}
	}
	
	//tuner_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_rda580x_search(uint16 freq, engine_status_t *status)
{
	tuner_printf("%s, freq:%d\n", __FUNCTION__, freq);
	if(rda580x_ctrl == NULL)return FALSE;
	if(!rda580x_ctrl->init_succeed)return FALSE;
	if(freq < rda580x_ctrl->min_freq)return FALSE;
	if(freq > rda580x_ctrl->max_freq)return FALSE;	
	
	if(!tuner_rda580x_freq(freq))return FALSE;
	
	OSTimeDly(5);
	
	uint8 index;
	uint8 count = 0;
	for(index = 0; index < 50; index++)
	{
		if(!tuner_rda580x_status(status))return FALSE;
		if(status->stc)
		{
			if(++count >= 5)
			{
				break;
			}
		}
		else
		{
			count = 0;
		}
		OSTimeDly(1);
	}
	
	if(index == 50)return FALSE;
		
	tuner_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}

static uint8 tuner_rda580x_setting(fm_setting_e type, void *set)
{
	tuner_printf("%s, type:%d\n", __FUNCTION__, type);
	if(rda580x_ctrl == NULL)return FALSE;
	if(!rda580x_ctrl->init_succeed)return FALSE;
	
#if defined(_ENABLE_ST_MEMORY_)
	uint8 stereo1 = (uint8)set;
	if(setup.first_st_flag)
	{
		setup.first_st_flag = FALSE;
		if(!rda580x_set_stereo(stereo1))return FALSE;
	}
#endif

	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!rda580x_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(!rda580x_set_stereo(stereo))return FALSE;
			}
			break;
			
		default:
			return FALSE;
	}
	
	tuner_printf("%s---->succeed\n", __FUNCTION__);
	return TRUE;
}
static uint8 tuner_rda580x_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_RDA580x;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = RDA580x_TUNER_RSSI_FM;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = RDA580x_TUNER_RSSI_LOC_FM;
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_rda580x_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_rda580x_deal_spot_tab;
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

const tuner_drv_fun_t tuner_rda580x_drv_fun =
{
	tuner_rda580x_detect,
	tuner_rda580x_init,
	tuner_rda580x_exit,
	tuner_rda580x_freq,
	tuner_rda580x_status,
	tuner_rda580x_search,
	NULL,
	tuner_rda580x_setting,
	tuner_rda580x_info,
};

#endif

