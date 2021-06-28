#include "pt7314/pt7314.h"
#include "iic.h"
#include "setup.h"
//#include "ui_api.h"

static pt7314_audio_t pt7314_audio;

static void audio_scl_init(void)		{	I2C_SCL_INIT();					};
static void audio_scl_h(void)			{	I2C_SCL_H();					};
static void audio_scl_l(void)			{	I2C_SCL_L();					};
static void audio_sda_init(void)		{	I2C_SDA_INIT();					};
static void audio_sda_h(void)		{	I2C_SDA_H();					};
static void audio_sda_l(void)			{	I2C_SDA_L();					};
static uint8 audio_sda_get(void)		{	return (I2C_SDA_GET()?1:0);		};


static const i2c_gpio_t audio_gpio = 
{
	audio_scl_init,
	audio_scl_h,
	audio_scl_l,
	audio_sda_init,
	audio_sda_h,
	audio_sda_l,
	audio_sda_get,
};

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#if (AUDIO_VOLUME_MAX == 40)
static const uint8 VolumeTab[41]=
{
	63,60,57,55,53,51,49,47,45,43,
	41,39,37,35,33,31,29,27,25,23,
	21,19,18,17,16,15,14,13,12,11,
	10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
	0,
};
#elif (AUDIO_VOLUME_MAX == 63)
static const uint8 VolumeTab[64]=
{
    63,62,61,60,59,58,57,56,55,54,53,52,51,
	50,49,48,47,46,45,44,43,42,41,
    40,39,38,37,36,35,34,33,32,31,
    30,29,28,27,26,25,24,23,22,21,
    20,19,18,17,16,15,14,13,12,11,
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
    0
};
#endif

#if (AUDIO_ATTEN_WIDE == 10)
static const uint8 PhTabe[]=
{
	0,		/*0*/
	3,		/*1*/
	6,		/*2*/
	9,		/*3*/
	12,		/*4*/
	15,		/*5*/
	18,		/*6*/
	21,		/*7*/
	24,		/*8*/
	27,		/*9*/
	31,		/*10*/
};
#elif (AUDIO_ATTEN_WIDE == 7)
static const uint8 PhTabe[]=
{
	0,		/*0*/
	4,		/*1*/
	8,		/*2*/
	12,		/*3*/
	16,		/*4*/
	21,		/*5*/
	26,		/*6*/
	31,		/*7*/
};
#endif

#if defined(_SPECIAL_TRE_AND_BASS_)
static const uint8 ToneTabe[]=
{
	 4, 4, 5, 5, 5, 6, 6,
	15,
	14,14,13,13,13, 12, 12
};
#else
static const uint8 ToneTabe[]=
{
	 0, 1, 2, 3, 4, 5, 6,
	15,
	14,13,12,11,10, 9, 8
};
#endif

static const int8 EqTabe[][2]=
{
	{0,0}, 		//	EQ_FLAT
	{1,1},		//	EQ_CLASSIC
	{-2,-1},	//	EQ_POP
	{3,1},		//	EQ_ROCK
	{0,2}		//	EQ_JAZZ
};

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
static void audio_mute_ctrl(uint8 mute)
{
	AUDIO_MUTE_INIT();
	if(mute)
	{
    	AUDIO_MUTE_ON();
	#if defined(_ENABLE_VOLUME_CONTROL_)
		if(setup.zone1_start)
		{
			SYS_ZONE1_ON();
		}
		if(setup.zone2_start)
		{
			SYS_ZONE2_ON();
		}
		if(setup.zone3_start)
		{
			SYS_ZONE3_ON();
		}
		if(setup.zone4_start)
		{
			SYS_ZONE4_ON();
		}
	#endif
	}
	else
	{
		AUDIO_MUTE_OFF();
	#if defined(_ENABLE_VOLUME_CONTROL_)
		if(setup.zone1_start)
		{
			SYS_ZONE1_OFF();
		}
		if(setup.zone2_start)
		{
			SYS_ZONE2_OFF();
		}
		if(setup.zone3_start)
		{
			SYS_ZONE3_OFF();
		}
		if(setup.zone4_start)
		{
			SYS_ZONE4_OFF();
		}
	#endif
	}
}

#if defined(_SUPPORT_SUBWOOFER_)
static void audio_subwoofer_ctrl(uint8 subwoofer)
{
	if(pt7314_audio.subwoofer_last != subwoofer)
	{
		AUDIO_SUB_INIT();
		if(subwoofer)
		{
			AUDIO_SUB_ON();
		}
		else
		{
			AUDIO_SUB_OFF();
		}
		pt7314_audio.subwoofer_last = subwoofer;
	}
}
#endif

static uint8 audio_switch_value(void)
{
	uint8 value;
	
	value = ((uint8)pt7314_audio.channel)&0x03;
	if(pt7314_audio.loud == AUDIO_LOUDNESS_OFF)
		value|= _B2_;
	value |= (((uint8)pt7314_audio.gate)<<3);
	
	return value;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

void audio_reg_init(void)
{
	memset(&pt7314_audio, 0, sizeof(pt7314_audio));
		
	pt7314_audio.channel 	= 0;
	pt7314_audio.loud 		= AUDIO_LOUDNESS_OFF;
#if defined(_SUPPORT_SUBWOOFER_)
	pt7314_audio.subwoofer	= AUDIO_SUBWOOFER_OFF;
#endif
	pt7314_audio.gate 		= AUDIO_GATE_PLUS_11_25;
	pt7314_audio.volume 	= AUDIO_VOLUME_DEFAULT;
	pt7314_audio.mute_last 	= pt7314_audio.mute = ENABLE;
	
	audio_mute_ctrl(ENABLE);
}

void audio_reg_set(void)
{
	uint8  mute_flag;
#if defined(_SUPPORT_SUBWOOFER_)
	uint8  subwoofer;
#endif
	uint8 index;
	int8  audio_reg[PT7314_REG_SIZE];
	
	if((pt7314_audio.mute)||(!pt7314_audio.volume))
	{
		mute_flag = ENABLE;
	}
	else 
	{
		mute_flag = DISABLE;
	}
	
#if defined(_SUPPORT_SUBWOOFER_)
	if(mute_flag ||(pt7314_audio.subwoofer == AUDIO_SUBWOOFER_OFF))
	{
		subwoofer = OFF;
	}
	else
	{
		subwoofer = ON;
	}
#endif
	if(pt7314_audio.mute_last != mute_flag)
	{
		if(mute_flag)
		{
			audio_mute_ctrl(TRUE);
			
			OSTimeDly(2);
			
		#if defined(_SUPPORT_SUBWOOFER_)
			audio_subwoofer_ctrl(subwoofer);
		#endif
		}
	}

	memset(audio_reg, 0, sizeof(audio_reg));
	
	if(mute_flag)
	{
		audio_reg[PT7314_REG_VOLUME] = VolumeTab[0];
		audio_reg[PT7314_REG_LR] = 0xC0|0x1F; 
		audio_reg[PT7314_REG_RR] = 0xE0|0x1F;
		audio_reg[PT7314_REG_LF] = 0x80|0x1F;
		audio_reg[PT7314_REG_RF] = 0xA0|0x1F;
		audio_reg[PT7314_REG_SW] = 0x5C|audio_switch_value();
	}
	else
	{
		audio_reg[PT7314_REG_VOLUME] = VolumeTab[pt7314_audio.volume];
		/*---------------------------------------------*/
		if(pt7314_audio.lr > 0)
		{
			audio_reg[PT7314_REG_LR] = 
			audio_reg[PT7314_REG_LF] = (uint8)(pt7314_audio.lr); 
		}
		else
		{
			audio_reg[PT7314_REG_RR] = 
			audio_reg[PT7314_REG_RF] = (uint8)(-pt7314_audio.lr);
		}
		
		if(pt7314_audio.fr > 0)
		{
			audio_reg[PT7314_REG_LR] += (uint8)(pt7314_audio.fr);
			audio_reg[PT7314_REG_RR] += (uint8)(pt7314_audio.fr);
		}
		else
		{
			audio_reg[PT7314_REG_LF] += (uint8)(-pt7314_audio.fr);
			audio_reg[PT7314_REG_RF] += (uint8)(-pt7314_audio.fr);
		}
		
		for(index = PT7314_REG_LR; index < (PT7314_REG_LR + 4); index ++)
		{
			if(audio_reg[index] > AUDIO_ATTEN_WIDE)
				audio_reg[index] = AUDIO_ATTEN_WIDE;
			
			audio_reg[index] = PhTabe[ audio_reg[index] ];
		}
		
		/*---------------------------------------------*/
		audio_reg[PT7314_REG_LR] |= 0xC0; 
		audio_reg[PT7314_REG_RR] |= 0xE0;
		audio_reg[PT7314_REG_LF] |= 0x80;
		audio_reg[PT7314_REG_RF] |= 0xA0;
		audio_reg[PT7314_REG_SW]  = 0x40|audio_switch_value();
		/*---------------------------------------------*/
		if(pt7314_audio.eq == AUDIO_EQ_OFF)
		{
			audio_reg[PT7314_REG_BASS] = pt7314_audio.bass;
			audio_reg[PT7314_REG_TREB] = pt7314_audio.treble;
		}
		else
		{
			audio_reg[PT7314_REG_BASS] = EqTabe[pt7314_audio.eq - 1][0];
			audio_reg[PT7314_REG_TREB] = EqTabe[pt7314_audio.eq - 1][1];	
		}
		audio_reg[PT7314_REG_BASS] = ToneTabe[audio_reg[PT7314_REG_BASS]+7];
		audio_reg[PT7314_REG_TREB] = ToneTabe[audio_reg[PT7314_REG_TREB]+7];
	}	
	
	audio_reg[PT7314_REG_BASS] |= 0x60;
	audio_reg[PT7314_REG_TREB] |= 0x70;

	i2c_data_init(PT7314_I2C_ADDR, PT7314_I2C_RATE, &audio_gpio);
	i2c_data_w((uint8 *)audio_reg, sizeof(audio_reg));
	i2c_data_exit();
	
	if(pt7314_audio.mute_last != mute_flag)
	{
		if(!mute_flag)
		{			
			audio_mute_ctrl(FALSE);
		}
	}
	
	pt7314_audio.mute_last = mute_flag;
	
#if defined(_SUPPORT_SUBWOOFER_)
	audio_subwoofer_ctrl(subwoofer);
#endif
}

uint8 audio_reg_ctrl(uint8 ctrl, int para)
{
	switch(ctrl)
	{
		/*!设置静音*/
		case AUD_CTRL_MUTE:
			{
				uint8 mute = (uint8)para;
				pt7314_audio.mute = mute;
			}
			break;
		/*!设置通道*/
		case AUD_CTRL_CHANNEL:
			{
				uint8 channel = (uint8)para;
				if(channel < AUDIO_CHANNEL_MAX)
				{
					switch(channel)
					{
						case AUDIO_CHANNEL_RADIO_ON:
						case AUDIO_CHANNEL_RADIO_OFF:
							pt7314_audio.channel = 3;
							break;
							
						case AUDIO_CHANNEL_MUSIC_ON:
						case AUDIO_CHANNEL_MUSIC_OFF:
							pt7314_audio.channel = 3;
							break;
						
						case AUDIO_CHANNEL_AUX_ON:
						case AUDIO_CHANNEL_AUX_OFF:
					    	pt7314_audio.channel = 3;
							break;
							
					#if defined(_ENABLE_MODE_CHANNEL_)
    					case AUDIO_CHANNEL_OPTICAL_ON:
    					case AUDIO_CHANNEL_OPTICAL_OFF:
    						pt7314_audio.channel = 0;
    						break;
							
						case AUDIO_CHANNEL_TV_ON:
						case AUDIO_CHANNEL_TV_OFF:
					    	pt7314_audio.channel = 1;
							break;
							
						case AUDIO_CHANNEL_DVD_ON:
						case AUDIO_CHANNEL_DVD_OFF:
							pt7314_audio.channel = 2;
							break;
					#endif
					}
				}
			}
			break;
		/*!设置放大*/
		case AUD_CTRL_GATE:
			{
				uint8 gate = (uint8)para;
				if(gate < AUDIO_GATE_PLUS_MAX)
				{
					pt7314_audio.gate = gate;
				}
			}
			break;
		/*!设置响度*/
		case AUD_CTRL_LOUD:
			{
				uint8 loud = (uint8)para;
				if(loud < AUDIO_LOUDNESS_MAX)
				{
					pt7314_audio.loud = loud;
				}
			}
			break;
		/*!设置音效*/
		case AUD_CTRL_EQ:
			{
				uint8 eq = (uint8)para;
				if(eq < AUDIO_EQ_MAX)
				{
					pt7314_audio.eq = eq;
				}
			}
			break;
		/*!设置音量*/
		case AUD_CTRL_VOLUME:
			{
				uint8 volume = (uint8)para;
				if(volume > AUDIO_VOLUME_MAX)
				{
					pt7314_audio.volume = AUDIO_VOLUME_MAX;
				}
				else
				{
					pt7314_audio.volume = volume;
				}
			}
			break;
		/*!设置低音*/
		case AUD_CTRL_BASS:
			{
				int8 bass = (int8)para;
				if(bass > AUDIO_BASS_MAX)
				{
					pt7314_audio.bass = AUDIO_BASS_MAX;
				}
				else if(bass < AUDIO_BASS_MIN)
				{
					pt7314_audio.bass = AUDIO_BASS_MIN;
				}
				else
				{
					pt7314_audio.bass = bass;
				}
			}
			break;
		/*!设置高音*/
		case AUD_CTRL_TREBLE:
			{
				int8 treble = (int8)para;
				if(treble > AUDIO_TREBLE_MAX)
				{
					pt7314_audio.treble = AUDIO_TREBLE_MAX;
				}
				else if(treble < AUDIO_TREBLE_MIN)
				{
					pt7314_audio.treble = AUDIO_TREBLE_MIN;
				}
				else
				{
					pt7314_audio.treble = treble;
				}
			}
			break;
		/*!设置左右平衡*/
		case AUD_CTRL_LR:
			{
				int8 lr = (int8)para;
				if(lr > AUDIO_LR_MAX)
				{
					pt7314_audio.lr = AUDIO_LR_MAX;
				}
				else if(lr < AUDIO_LR_MIN)
				{
					pt7314_audio.lr = AUDIO_LR_MIN;
				}
				else
				{
					pt7314_audio.lr = lr;
				}
			}
			break;
		/*!设置前后平衡*/
		case AUD_CTRL_FR:
			{
				int8 fr = (int8)para;
				if(fr > AUDIO_FR_MAX)
				{
					pt7314_audio.fr = AUDIO_FR_MAX;
				}
				else if(fr < AUDIO_FR_MIN)
				{
					pt7314_audio.fr = AUDIO_FR_MIN;
				}
				else
				{
					pt7314_audio.fr = fr;
				}
			}
			break;
	#if defined(_SUPPORT_SUBWOOFER_)
		/*!设置重低音*/
		case AUD_CTRL_SUBWOOFER:
			{
				uint8 subwoofer = (uint8)para;
				if(subwoofer < AUDIO_SUBWOOFER_MAX)
				{
					pt7314_audio.subwoofer = subwoofer;
				}
			}
			break;
	#endif
	
		/*!得到当前通道*/
		case AUD_GET_CHANNEL:
			*((uint8 *)para) = pt7314_audio.channel;
			return FALSE;
			
		default:
			return FALSE;
	}
	
	return TRUE;
}


