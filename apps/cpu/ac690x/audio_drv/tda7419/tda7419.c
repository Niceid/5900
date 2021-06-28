#include "tda7419/tda7419.h"
#include "iic.h"
#include "setup.h"

static tda7419_audio_t tda7419_audio;

#if defined(_ENABLE_PT2313_I2C_)
static void audio_scl_init(void)		{	I2C_SCL_INIT1();				};
static void audio_scl_h(void)			{	I2C_SCL_H1();					};
static void audio_scl_l(void)			{	I2C_SCL_L1();					};
static void audio_sda_init(void)		{	I2C_SDA_INIT1();				};
static void audio_sda_h(void)		{	I2C_SDA_H1();					};
static void audio_sda_l(void)			{	I2C_SDA_L1();					};
static uint8 audio_sda_get(void)		{	return (I2C_SDA_GET1()?1:0);	};
#else
static void audio_scl_init(void)		{	I2C_SCL_INIT();					};
static void audio_scl_h(void)			{	I2C_SCL_H();					};
static void audio_scl_l(void)			{	I2C_SCL_L();					};
static void audio_sda_init(void)		{	I2C_SDA_INIT();					};
static void audio_sda_h(void)		{	I2C_SDA_H();					};
static void audio_sda_l(void)			{	I2C_SDA_L();					};
static uint8 audio_sda_get(void)		{	return (I2C_SDA_GET()?1:0);		};
#endif

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

#if defined(_JL8273_SPECIAL_VOLUME_)
static const uint8 VolumeTab[41]=
{
	AUDIO_GAIN_DB(-80),AUDIO_GAIN_DB(-76),AUDIO_GAIN_DB(-72),AUDIO_GAIN_DB(-68),AUDIO_GAIN_DB(-64),	//0  1  2  3  4 
	AUDIO_GAIN_DB(-60),AUDIO_GAIN_DB(-57),AUDIO_GAIN_DB(-54),AUDIO_GAIN_DB(-51),AUDIO_GAIN_DB(-48),	//5  6  7  8  9
	AUDIO_GAIN_DB(-44),AUDIO_GAIN_DB(-40),AUDIO_GAIN_DB(-36),AUDIO_GAIN_DB(-32),AUDIO_GAIN_DB(-28),	//10 11 12 13 14
	AUDIO_GAIN_DB(-24),AUDIO_GAIN_DB(-23),AUDIO_GAIN_DB(-22),AUDIO_GAIN_DB(-21),AUDIO_GAIN_DB(-20),	//15 16 17 18 19
	AUDIO_GAIN_DB(-19),AUDIO_GAIN_DB(-18),AUDIO_GAIN_DB(-17),AUDIO_GAIN_DB(-16),AUDIO_GAIN_DB(-15),	//20 21 22 23 24
	AUDIO_GAIN_DB(-14),AUDIO_GAIN_DB(-13),AUDIO_GAIN_DB(-12), AUDIO_GAIN_DB(-11), AUDIO_GAIN_DB(-10),	//25 26 27 28 29
	AUDIO_GAIN_DB(-9), AUDIO_GAIN_DB(-9), AUDIO_GAIN_DB(-8), AUDIO_GAIN_DB(-8), AUDIO_GAIN_DB(-7),	//30 31 32 33 34
	AUDIO_GAIN_DB(-7), AUDIO_GAIN_DB(-6), AUDIO_GAIN_DB(-6), AUDIO_GAIN_DB(-5), AUDIO_GAIN_DB(-3),	//35 36 37 38 39
	AUDIO_GAIN_DB(-1)																				//40
};
#else
static const uint8 VolumeTab[41]=
{
	AUDIO_GAIN_DB(-80),AUDIO_GAIN_DB(-76),AUDIO_GAIN_DB(-72),AUDIO_GAIN_DB(-68),AUDIO_GAIN_DB(-64),	//0  1  2  3  4 
	AUDIO_GAIN_DB(-60),AUDIO_GAIN_DB(-57),AUDIO_GAIN_DB(-54),AUDIO_GAIN_DB(-51),AUDIO_GAIN_DB(-48),	//5  6  7  8  9
	AUDIO_GAIN_DB(-44),AUDIO_GAIN_DB(-40),AUDIO_GAIN_DB(-36),AUDIO_GAIN_DB(-32),AUDIO_GAIN_DB(-28),	//10 11 12 13 14
	AUDIO_GAIN_DB(-24),AUDIO_GAIN_DB(-22),AUDIO_GAIN_DB(-20),AUDIO_GAIN_DB(-18),AUDIO_GAIN_DB(-17),	//15 16 17 18 19
	AUDIO_GAIN_DB(-16),AUDIO_GAIN_DB(-15),AUDIO_GAIN_DB(-14),AUDIO_GAIN_DB(-13),AUDIO_GAIN_DB(-12),	//20 21 22 23 24
	AUDIO_GAIN_DB(-11),AUDIO_GAIN_DB(-10),AUDIO_GAIN_DB(-9), AUDIO_GAIN_DB(-8), AUDIO_GAIN_DB(-7),	//25 26 27 28 29
	AUDIO_GAIN_DB(-7), AUDIO_GAIN_DB(-6), AUDIO_GAIN_DB(-6), AUDIO_GAIN_DB(-5), AUDIO_GAIN_DB(-5),	//30 31 32 33 34
	AUDIO_GAIN_DB(-4), AUDIO_GAIN_DB(-4), AUDIO_GAIN_DB(-3), AUDIO_GAIN_DB(-3), AUDIO_GAIN_DB(-2),	//35 36 37 38 39
	AUDIO_GAIN_DB(-1)																				//40
};
#endif

static const uint8 SubwooferTab[41]=
{
	AUDIO_GAIN_DB(-80),AUDIO_GAIN_DB(-76),AUDIO_GAIN_DB(-72),AUDIO_GAIN_DB(-68),AUDIO_GAIN_DB(-64),	//0  1  2  3  4 
	AUDIO_GAIN_DB(-60),AUDIO_GAIN_DB(-57),AUDIO_GAIN_DB(-54),AUDIO_GAIN_DB(-51),AUDIO_GAIN_DB(-48),	//5  6  7  8  9
	AUDIO_GAIN_DB(-44),AUDIO_GAIN_DB(-40),AUDIO_GAIN_DB(-36),AUDIO_GAIN_DB(-32),AUDIO_GAIN_DB(-28),	//10 11 12 13 14
	AUDIO_GAIN_DB(-24),AUDIO_GAIN_DB(-22),AUDIO_GAIN_DB(-20),AUDIO_GAIN_DB(-18),AUDIO_GAIN_DB(-17),	//15 16 17 18 19
	AUDIO_GAIN_DB(-16),AUDIO_GAIN_DB(-15),AUDIO_GAIN_DB(-14),AUDIO_GAIN_DB(-13),AUDIO_GAIN_DB(-12),	//20 21 22 23 24
	AUDIO_GAIN_DB(-11),AUDIO_GAIN_DB(-10),AUDIO_GAIN_DB(-9), AUDIO_GAIN_DB(-8), AUDIO_GAIN_DB(-7),	//25 26 27 28 29
	AUDIO_GAIN_DB(-5), AUDIO_GAIN_DB(-3), AUDIO_GAIN_DB(-1), AUDIO_GAIN_DB(1),  AUDIO_GAIN_DB(3),	//30 31 32 33 34
	AUDIO_GAIN_DB(5),  AUDIO_GAIN_DB(7),  AUDIO_GAIN_DB(9),  AUDIO_GAIN_DB(11), AUDIO_GAIN_DB(13),	//35 36 37 38 39
	AUDIO_GAIN_DB(15)																				//40
};

#if (AUDIO_ATTEN_WIDE == 10)
static const uint8 PhTabe[]=
{
	AUDIO_GAIN_DB(10),		/*0*/
	AUDIO_GAIN_DB(8),		/*1*/
	AUDIO_GAIN_DB(6),		/*2*/
	AUDIO_GAIN_DB(4),		/*3*/
	AUDIO_GAIN_DB(2),		/*4*/
	AUDIO_GAIN_DB(0),		/*5*/
	AUDIO_GAIN_DB(-10),		/*6*/
	AUDIO_GAIN_DB(-20),		/*7*/
	AUDIO_GAIN_DB(-40),		/*8*/
	AUDIO_GAIN_DB(-60),		/*9*/
	AUDIO_GAIN_DB(-80),		/*10*/
};
#elif (AUDIO_ATTEN_WIDE == 7)
static const uint8 PhTabe[]=
{
	AUDIO_GAIN_DB(10),		/*0*/
	AUDIO_GAIN_DB(7),		/*1*/
	AUDIO_GAIN_DB(3),		/*2*/
	AUDIO_GAIN_DB(0),		/*3*/
	AUDIO_GAIN_DB(-10),		/*4*/
	AUDIO_GAIN_DB(-20),		/*5*/
	AUDIO_GAIN_DB(-50),		/*6*/
	AUDIO_GAIN_DB(-80),		/*7*/
};
#endif

static const uint8 ToneTabe[]=
{
 	0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,
	0x00,
	0x12,0x14,0x16,0x18,0x1A,0x1C,0x1E,
};

static const int8 EqTabe[][3]=
{
//	BASS	MIDDLE 	TREBLE
	{0,		0,		0		}, 		//	EQ_FLAT
	{1,		0,		1		},		//	EQ_CLASSIC
	{-2,	0,		-1		},		//	EQ_POP
	{5,		0,		3		},		//	EQ_ROCK
	{0,		0,		2		},		//	EQ_JAZZ
};

static const uint8 InputGainTab[AUDIO_GATE_PLUS_MAX] = 
{
	15,
	13,
	7,
	2,
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
	}
	else
	{
		AUDIO_MUTE_OFF();
	}
}

#if defined(_SUPPORT_SUBWOOFER_)
static void audio_subwoofer_ctrl(uint8 subwoofer)
{
	if(tda7419_audio.subwoofer_last != subwoofer)
	{
		tda7419_audio.subwoofer_last = subwoofer;
	}
}
#endif

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

void audio_reg_init(void)
{
	memset(&tda7419_audio, 0, sizeof(tda7419_audio));
		
	tda7419_audio.channel = 0;
	
	tda7419_audio.loudness.enable 		= AUDIO_LOUDNESS_OFF;
	tda7419_audio.loudness.centerfreq 	= AUDIO_LOUDNESS_CENTER_FREQ_DEFAULT;
	tda7419_audio.loudness.gain 		= AUDIO_LOUDNESS_GAIN_DEFAULT;
	
#if defined(_SUPPORT_SUBWOOFER_)
	tda7419_audio.subwoofer.enable	= AUDIO_SUBWOOFER_OFF;
#endif
    tda7419_audio.subwoofer.cutfreq = AUDIO_SUBWOOFER_CUT_FREQ_DEFAULT;
    tda7419_audio.subwoofer.gain 	= AUDIO_SUBWOOFER_GAIN_DEFAULT;

	tda7419_audio.gate 		= AUDIO_GATE_PLUS_11_25;
	tda7419_audio.volume 	= AUDIO_VOLUME_DEFAULT;
	tda7419_audio.mute_last = tda7419_audio.mute = ENABLE;
	
	audio_mute_ctrl(ENABLE);
}

void audio_reg_set(void)
{
	uint8  mute_flag;
	//uint8  subwoofer;
	uint8  index;
	int8   audio_reg[TDA7419_REG_SIZE];
	
	if((tda7419_audio.mute)||(!tda7419_audio.volume))
	{
		mute_flag = ENABLE;
	}
	else 
	{
		mute_flag = DISABLE;
	}
	
	if(tda7419_audio.mute_last != mute_flag)
	{
		if(mute_flag)
		{
			audio_mute_ctrl(TRUE);
			
			OSTimeDly(2);
		}
	}

	memset(audio_reg, 0, sizeof(audio_reg));
	
   	audio_reg[TDA7419_REG_MAIN_SOURCE] = (tda7419_audio.channel<<0) | (InputGainTab[tda7419_audio.gate] << 3) | (1<<7);
	audio_reg[TDA7419_REG_SPECTRUM] = (1<<6);

#if defined(_SUPPORT_SUBWOOFER_)
	if(tda7419_audio.subwoofer.enable == AUDIO_SUBWOOFER_ON)
	{
		if(mute_flag)
		{
			audio_reg[TDA7419_REG_MIXING] = (0<<3) | (0x07);
			audio_reg[TDA7419_REG_SUBWOOFER] = (3 << 5);
		}
		else
		{
			audio_reg[TDA7419_REG_MIXING] = (0<<3) | (0x07);
			audio_reg[TDA7419_REG_SUBWOOFER] = SubwooferTab[tda7419_audio.subwoofer.gain];
		}
	}
	else
	{
		audio_reg[TDA7419_REG_MIXING] = (0<<3) | (0x07);
		audio_reg[TDA7419_REG_SUBWOOFER] = (3 << 5);
	}
#endif

	if(tda7419_audio.loudness.enable == AUDIO_LOUDNESS_ON)
	{
		audio_reg[TDA7419_REG_MAIN_LOUD] = (tda7419_audio.loudness.gain<<0) | (tda7419_audio.loudness.centerfreq)<<4 | (0<<6) | (0<<7);
	}
	else
	{
	 	audio_reg[TDA7419_REG_MAIN_LOUD] = (10<<0) | (AUDIO_LOUDNESS_FLAT)<<4 | (0<<6) | (0<<7);
	}

	audio_reg[TDA7419_REG_SOFT_MUTE] = (1<<0) | (1<<1) | (1<<2) | (0<<4) | (0<<7);	
	audio_reg[TDA7419_REG_FREQ]   = (tda7419_audio.subwoofer.cutfreq<<0)|(1<<2)|(0<<4)|(0<<6)|(0<<7);

	if(mute_flag)
	{
		audio_reg[TDA7419_REG_VOLUME] = VolumeTab[0];
		audio_reg[TDA7419_REG_LR] = 0x60;
		audio_reg[TDA7419_REG_RR] = 0x60;
		audio_reg[TDA7419_REG_LF] = 0x60;
		audio_reg[TDA7419_REG_RF] = 0x60;
	}
	else
	{
		audio_reg[TDA7419_REG_VOLUME] = VolumeTab[tda7419_audio.volume];
		/*---------------------------------------------*/
		if(tda7419_audio.lr > 0)
		{
			audio_reg[TDA7419_REG_LR] = 
			audio_reg[TDA7419_REG_LF] = (uint8)(tda7419_audio.lr); 
		}
		else
		{
			audio_reg[TDA7419_REG_RR] = 
			audio_reg[TDA7419_REG_RF] = (uint8)(-tda7419_audio.lr);
		}
		
		if(tda7419_audio.fr > 0)
		{
			audio_reg[TDA7419_REG_LR] += (uint8)(tda7419_audio.fr);
			audio_reg[TDA7419_REG_RR] += (uint8)(tda7419_audio.fr);
		}
		else
		{
			audio_reg[TDA7419_REG_LF] += (uint8)(-tda7419_audio.fr);
			audio_reg[TDA7419_REG_RF] += (uint8)(-tda7419_audio.fr);
		}
		
		for(index = TDA7419_REG_LF; index < (TDA7419_REG_LF + 4); index ++)
		{
			if(audio_reg[index] > AUDIO_ATTEN_WIDE)
				audio_reg[index] = AUDIO_ATTEN_WIDE;
			
			audio_reg[index] = PhTabe[ audio_reg[index] ];
		}
		
		if(tda7419_audio.eq == AUDIO_EQ_OFF)
		{
			audio_reg[TDA7419_REG_BASS] 	= tda7419_audio.bass;
			audio_reg[TDA7419_REG_MIDDLE] 	= tda7419_audio.middle;
			audio_reg[TDA7419_REG_TREBLE] 	= tda7419_audio.treble;
		}
		else
		{
			audio_reg[TDA7419_REG_BASS] 	= EqTabe[tda7419_audio.eq - 1][0];
			audio_reg[TDA7419_REG_MIDDLE] 	= EqTabe[tda7419_audio.eq - 1][1];
			audio_reg[TDA7419_REG_TREBLE] 	= EqTabe[tda7419_audio.eq - 1][2];
		}
		
		audio_reg[TDA7419_REG_BASS] 	= ToneTabe[audio_reg[TDA7419_REG_BASS]+7];
		audio_reg[TDA7419_REG_MIDDLE] 	= ToneTabe[audio_reg[TDA7419_REG_MIDDLE]+7];
		audio_reg[TDA7419_REG_TREBLE] 	= ToneTabe[audio_reg[TDA7419_REG_TREBLE]+7];
	}	
	
	audio_reg[TDA7419_REG_TREBLE] |= (0x80 | (TREBLE_CENTER_FREQ_10KHZ << 5));
	audio_reg[TDA7419_REG_MIDDLE] |= 0x80;
	audio_reg[TDA7419_REG_BASS]   |= 0x80;

	i2c_data_init(TDA7419_I2C_ADDR, TDA7419_I2C_RATE, &audio_gpio);
	i2c_data_w3((uint8 *)audio_reg, sizeof(audio_reg));
	i2c_data_exit();
	
	if(tda7419_audio.mute_last != mute_flag)
	{
		if(!mute_flag)
		{			
			audio_mute_ctrl(FALSE);
		}
	}
	
	tda7419_audio.mute_last = mute_flag;
}

uint8 audio_reg_ctrl(uint8 ctrl, int para)
{
	switch(ctrl)
	{
		/*!设置静音*/
		case AUD_CTRL_MUTE:
			{
				uint8 mute = (uint8)para;
				tda7419_audio.mute = mute;
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
						#if defined(_ENABLE_7419_CHANNEL_)
    						if(fm_inside_check())
    						{
    							tda7419_audio.channel = 0;
    						}
    						else
    						{
    							tda7419_audio.channel = 0;
    						}
						#else
							if(fm_inside_check())
							{
								tda7419_audio.channel = 1;
							}
							else
							{
								tda7419_audio.channel = 1;
							}
						#endif
							break;
							
						case AUDIO_CHANNEL_MUSIC_ON:
						case AUDIO_CHANNEL_MUSIC_OFF:
						#if defined(_ENABLE_7419_CHANNEL_)
							tda7419_audio.channel = 3;
						#else
							tda7419_audio.channel = 0;
						#endif
							break;
						
						case AUDIO_CHANNEL_AUX_ON:
						case AUDIO_CHANNEL_AUX_OFF:
						#if defined(_ENABLE_7419_CHANNEL_)
							tda7419_audio.channel = 2;
						#else
					    	tda7419_audio.channel = 3;
						#endif
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
					tda7419_audio.gate = gate;
				}
			}
			break;
		/*!设置响度*/
		case AUD_CTRL_LOUD:
			{
				uint8 loud = (uint8)para;
				if(loud < AUDIO_LOUDNESS_MAX)
				{
					tda7419_audio.loudness.enable = loud;
				}
			}
			break;
		/*!设置音效*/
		case AUD_CTRL_EQ:
			{
				uint8 eq = (uint8)para;
				if(eq < AUDIO_EQ_MAX)
				{
					tda7419_audio.eq = eq;
				}
			}
			break;
		/*!设置音量*/
		case AUD_CTRL_VOLUME:
			{
				uint8 volume = (uint8)para;
				if(volume > AUDIO_VOLUME_MAX)
				{
					tda7419_audio.volume = AUDIO_VOLUME_MAX;
				}
				else
				{
					tda7419_audio.volume = volume;
				}
			}
			break;
		/*!设置低音*/
		case AUD_CTRL_BASS:
			{
				int8 bass = (int8)para;
				if(bass > AUDIO_BASS_MAX)
				{
					tda7419_audio.bass = AUDIO_BASS_MAX;
				}
				else if(bass < AUDIO_BASS_MIN)
				{
					tda7419_audio.bass = AUDIO_BASS_MIN;
				}
				else
				{
					tda7419_audio.bass = bass;
				}
			}
			break;
		/*!设置中音*/
	#if defined(_ENABLE_7419_MIDDLE_)
		case AUD_CTRL_MIDDLE:
			{
				int8 middle = (int8)para;
				if(middle > AUDIO_MIDDLE_MAX)
				{
					tda7419_audio.middle = AUDIO_MIDDLE_MAX;
				}
				else if(middle < AUDIO_MIDDLE_MIN)
				{
					tda7419_audio.middle = AUDIO_MIDDLE_MIN;
				}
				else
				{
					tda7419_audio.middle = middle;
				}
			}
			break;
	#endif
		/*!设置高音*/
		case AUD_CTRL_TREBLE:
			{
				int8 treble = (int8)para;
				if(treble > AUDIO_TREBLE_MAX)
				{
					tda7419_audio.treble = AUDIO_TREBLE_MAX;
				}
				else if(treble < AUDIO_TREBLE_MIN)
				{
					tda7419_audio.treble = AUDIO_TREBLE_MIN;
				}
				else
				{
					tda7419_audio.treble = treble;
				}
			}
			break;
		/*!设置左右平衡*/
		case AUD_CTRL_LR:
			{
				int8 lr = (int8)para;
				if(lr > AUDIO_LR_MAX)
				{
					tda7419_audio.lr = AUDIO_LR_MAX;
				}
				else if(lr < AUDIO_LR_MIN)
				{
					tda7419_audio.lr = AUDIO_LR_MIN;
				}
				else
				{
					tda7419_audio.lr = lr;
				}
			}
			break;
		/*!设置前后平衡*/
		case AUD_CTRL_FR:
			{
				int8 fr = (int8)para;
				if(fr > AUDIO_FR_MAX)
				{
					tda7419_audio.fr = AUDIO_FR_MAX;
				}
				else if(fr < AUDIO_FR_MIN)
				{
					tda7419_audio.fr = AUDIO_FR_MIN;
				}
				else
				{
					tda7419_audio.fr = fr;
				}
			}
			break;
		/*!设置重低音*/
	#if defined(_SUPPORT_SUBWOOFER_)
		case AUD_CTRL_SUBWOOFER:
			{
				uint8 subwoofer = (uint8)para;
				if(subwoofer < AUDIO_SUBWOOFER_MAX)
				{
					tda7419_audio.subwoofer.enable = subwoofer;
				}
			}
			break;
	#endif
    #if defined(AUDIO_SUB_LEVEL_SET)
    	case AUD_CTRL_SUB_CUT:
    		{
    			uint8 sub_ctrl = (uint8)para;
    			if(sub_ctrl > AUDIO_SUBWOOFER_CUT_FREQ_160)
    			{
    				tda7419_audio.subwoofer.cutfreq = (AUDIO_SUBWOOFER_CUT_FREQ_160+1);
    			}
    			else
    			{
    				tda7419_audio.subwoofer.cutfreq = (sub_ctrl+1);
    			}
    		}
    		break;
    #endif
		/*!得到当前通道*/
		case AUD_GET_CHANNEL:
			*((uint8 *)para) = tda7419_audio.channel;
			return FALSE;
			
		default:
			return FALSE;
	}
	return TRUE;
}

