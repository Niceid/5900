#include "pt2313A/pt2313A.h"
#include "iic.h"
#include "setup.h"
#include "ui_api.h"

#if defined(_SUPPORT_TWO_PT2313_)

extern uint8 fm_inside_check(void);


static pt2313A_audio_t pt2313A_audio;

static void audio_scl_init1(void)		{	I2C_SCL_INIT1();					};
static void audio_scl_h1(void)			{	I2C_SCL_H1();					};
static void audio_scl_l1(void)			{	I2C_SCL_L1();					};
static void audio_sda_init1(void)		{	I2C_SDA_INIT1();					};
static void audio_sda_h1(void)		{	I2C_SDA_H1();					};
static void audio_sda_l1(void)			{	I2C_SDA_L1();					};
static uint8 audio_sda_get1(void)		{	return (I2C_SDA_GET1()?1:0);		};

static const i2c_gpio_t audio_gpio1 =
{
	audio_scl_init1,
	audio_scl_h1,
	audio_scl_l1,
	audio_sda_init1,
	audio_sda_h1,
	audio_sda_l1,
	audio_sda_get1,
};

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#if (AUDIO_VOLUME_MAX == 40)
static const uint8 VolumeTab1[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,0,0
};
#endif

#if defined(_HKY1032_RADIO_OUTPUT_)
static const uint8 VolumeTab2[41]=
{
63,52,47,43,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,
23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5
};
#endif

#if (AUDIO_ATTEN_WIDE == 10)
static const uint8 PhTabe1[]=
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
static const uint8 PhTabe1[]=
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

static const uint8 ToneTabe1[]=
{
	 0, 1, 2, 3, 4, 5, 6,
	15,
	14,13,12,11,10, 9, 8
};

static const int8 EqTabe1[][2]=
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
static void audio_mute_ctrl1(uint8 mute)
{
	AUDIO_MUTE_INIT();
	if(mute)
	{
		AUDIO_MUTE_ON();
	}
	else
	{
		AUDIO_MUTE_OFF();
	}
}

static uint8 audio_switch_value1(void)
{
	uint8 value;

	value = ((uint8)pt2313A_audio.channel)&0x03;//低两位为2313通道
	if(pt2313A_audio.loud == AUDIO_LOUDNESS_OFF)
		value|= _B2_;//B2为LOUD开关
	value |= (((uint8)pt2313A_audio.gate)<<3);//bit3/bit4为增益 00:11.25

	return value;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

void audio_reg_init1(void)
{
	memset(&pt2313A_audio, 0, sizeof(pt2313A_audio));

	pt2313A_audio.channel 	= 0;
	pt2313A_audio.loud 		= AUDIO_LOUDNESS_OFF;
	pt2313A_audio.gate 		= AUDIO_GATE_PLUS_11_25;
	pt2313A_audio.volume 	= AUDIO_VOLUME_DEFAULT;
	pt2313A_audio.mute_last = pt2313A_audio.mute = ENABLE;
	audio_mute_ctrl1(ENABLE);
}


void audio_reg_set1(void)
{
	uint8  mute_flag;

	uint8 index;
	int8  audio_reg[PT2313A_REG_SIZE];

	if((pt2313A_audio.mute)||((!setup.aud_vol2)&&(!setup.aud_vol)))
	{
		mute_flag = ENABLE;
	}
	else
	{
		mute_flag = DISABLE;
	}

	if(pt2313A_audio.mute_last != mute_flag)
	{
		if(mute_flag)
		{
			audio_mute_ctrl1(TRUE);
			OSTimeDly(2);
		}
	}

	memset(audio_reg, 0, sizeof(audio_reg));

	printf("pt2313A_audio.volume:%d\n",pt2313A_audio.volume);
	if(mute_flag)
	{
		audio_reg[PT2313A_REG_VOLUME] = VolumeTab1[0];
		audio_reg[PT2313A_REG_LR] = (int8)(0xC0|0x1F);
		audio_reg[PT2313A_REG_RR] = (int8)(0xE0|0x1F);
		audio_reg[PT2313A_REG_LF] = (int8)(0x80|0x1F);
		audio_reg[PT2313A_REG_RF] = (int8)(0xA0|0x1F);
		audio_reg[PT2313A_REG_SW] = (int8)(0x5C|audio_switch_value1());
	}
	else
	{
	#if defined(_HKY1032_RADIO_OUTPUT_)
		if(compare_task_name("FMRadioTask"))
		{
			audio_reg[PT2313A_REG_VOLUME] = VolumeTab2[pt2313A_audio.volume];
		}
		else
		{
			audio_reg[PT2313A_REG_VOLUME] = VolumeTab1[pt2313A_audio.volume];
		}
	#else
		audio_reg[PT2313A_REG_VOLUME] = VolumeTab1[pt2313A_audio.volume];
	#endif
		/*---------------------------------------------*/
		if(pt2313A_audio.lr > 0)
		{
			audio_reg[PT2313A_REG_LR] =
			audio_reg[PT2313A_REG_LF] = (uint8)(pt2313A_audio.lr);
		}
		else
		{
			audio_reg[PT2313A_REG_RR] =
			audio_reg[PT2313A_REG_RF] = (uint8)(-pt2313A_audio.lr);

		}
		if(pt2313A_audio.fr > 0)
		{
			audio_reg[PT2313A_REG_LR] += (uint8)(pt2313A_audio.fr);
			audio_reg[PT2313A_REG_RR] += (uint8)(pt2313A_audio.fr);
		}
		else
		{
			audio_reg[PT2313A_REG_LF] += (uint8)(-pt2313A_audio.fr);
			audio_reg[PT2313A_REG_RF] += (uint8)(-pt2313A_audio.fr);
		}

		for(index = PT2313A_REG_LR; index < (PT2313A_REG_LR + 4); index ++)
		{
			if(audio_reg[index] > AUDIO_ATTEN_WIDE)
				audio_reg[index] = AUDIO_ATTEN_WIDE;

			audio_reg[index] = PhTabe1[ audio_reg[index] ];
		}

		/*---------------------------------------------*/
		audio_reg[PT2313A_REG_LR] |= 0xC0;
		audio_reg[PT2313A_REG_RR] |= 0xE0;
		audio_reg[PT2313A_REG_LF] |= 0x80;
		audio_reg[PT2313A_REG_RF] |= 0xA0;
		audio_reg[PT2313A_REG_SW]  = 0x40|audio_switch_value1();
		/*---------------------------------------------*/
		if(pt2313A_audio.eq == AUDIO_EQ_OFF)
		{
			audio_reg[PT2313A_REG_BASS] = pt2313A_audio.bass;
			audio_reg[PT2313A_REG_TREB] = pt2313A_audio.treble;
		}
		else
		{
			audio_reg[PT2313A_REG_BASS] = EqTabe1[pt2313A_audio.eq - 1][0];
			audio_reg[PT2313A_REG_TREB] = EqTabe1[pt2313A_audio.eq - 1][1];
		}
		audio_reg[PT2313A_REG_BASS] = ToneTabe1[audio_reg[PT2313A_REG_BASS]+7];
		audio_reg[PT2313A_REG_TREB] = ToneTabe1[audio_reg[PT2313A_REG_TREB]+7];
	}

	audio_reg[PT2313A_REG_BASS] |= 0x60;
	audio_reg[PT2313A_REG_TREB] |= 0x70;

    i2c_data_init(PT2313_I2C_ADDR1, PT2313_I2C_RATE1, &audio_gpio1);
    i2c_data_w((uint8 *)audio_reg, sizeof(audio_reg));
    i2c_data_exit();

	puts("audio_reg_set1111\n");
	if(pt2313A_audio.mute_last != mute_flag)
	{
		if(!mute_flag)
		{
			puts("audio_reg_set2222\n");
			audio_mute_ctrl1(FALSE);
		}
	}

	pt2313A_audio.mute_last = mute_flag;
}

uint8 audio_reg_ctrl1(uint8 ctrl, int para)
{
	switch(ctrl)
	{
		/*!设置静音*/
		case AUD_CTRL_MUTE:
			{
				uint8 mute = (uint8)para;
				pt2313A_audio.mute = mute;
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
							if(fm_inside_check())
							{
								pt2313A_audio.channel = 1;
							}
							else
							{
								pt2313A_audio.channel = 2;
							}
							break;

						case AUDIO_CHANNEL_MUSIC_ON:
						case AUDIO_CHANNEL_MUSIC_OFF:
							pt2313A_audio.channel = 1;
							break;

						case AUDIO_CHANNEL_AUX_ON:
						case AUDIO_CHANNEL_AUX_OFF:
					    	pt2313A_audio.channel = 0;
							break;
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
					pt2313A_audio.gate = gate;
				}
			}
			break;
		/*!设置响度*/
		case AUD_CTRL_LOUD:
			{
				uint8 loud = (uint8)para;
				if(loud < AUDIO_LOUDNESS_MAX)
				{
					pt2313A_audio.loud = loud;
				}
			}
			break;
		/*!设置音效*/
		case AUD_CTRL_EQ:
			{
				uint8 eq = (uint8)para;
				if(eq < AUDIO_EQ_MAX)
				{
					pt2313A_audio.eq = eq;
				}
			}
			break;
		/*!设置音量*/
		case AUD_CTRL_VOLUME:
			{
				uint8 volume = (uint8)para;
				if(volume > AUDIO_VOLUME_MAX)
				{
					pt2313A_audio.volume = AUDIO_VOLUME_MAX;
				}
				else
				{
					pt2313A_audio.volume = volume;
				}
			}
			break;
		/*!设置低音*/
		case AUD_CTRL_BASS:
			{
				int8 bass = (int8)para;
				if(bass > AUDIO_BASS_MAX)
				{
					pt2313A_audio.bass = AUDIO_BASS_MAX;
				}
				else if(bass < AUDIO_BASS_MIN)
				{
					pt2313A_audio.bass = AUDIO_BASS_MIN;
				}
				else
				{
					pt2313A_audio.bass = bass;
				}
			}
			break;
		/*!设置高音*/
		case AUD_CTRL_TREBLE:
			{
				int8 treble = (int8)para;
				if(treble > AUDIO_TREBLE_MAX)
				{
					pt2313A_audio.treble = AUDIO_TREBLE_MAX;
				}
				else if(treble < AUDIO_TREBLE_MIN)
				{
					pt2313A_audio.treble = AUDIO_TREBLE_MIN;
				}
				else
				{
					pt2313A_audio.treble = treble;
				}
			}
			break;
		/*!设置左右平衡*/
		case AUD_CTRL_LR:
			{
				int8 lr = (int8)para;
				if(lr > AUDIO_LR_MAX)
				{
					pt2313A_audio.lr = AUDIO_LR_MAX;
				}
				else if(lr < AUDIO_LR_MIN)
				{
					pt2313A_audio.lr = AUDIO_LR_MIN;
				}
				else
				{
					pt2313A_audio.lr = lr;
				}
			}
			break;
		/*!设置前后平衡*/
		case AUD_CTRL_FR:
			{
				int8 fr = (int8)para;
				if(fr > AUDIO_FR_MAX)
				{
					pt2313A_audio.fr = AUDIO_FR_MAX;
				}
				else if(fr < AUDIO_FR_MIN)
				{
					pt2313A_audio.fr = AUDIO_FR_MIN;
				}
				else
				{
					pt2313A_audio.fr = fr;
				}
			}
			break;
		/*!得到当前通道*/
		case AUD_GET_CHANNEL:
			*((uint8 *)para) = pt2313A_audio.channel;
			return FALSE;
			
		default:
			return FALSE;
	}

	return TRUE;
}

#endif

