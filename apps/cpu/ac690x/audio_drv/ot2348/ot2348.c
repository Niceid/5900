#include "ot2348/ot2348.h"
#include "iic.h"
#if defined(_RADIO_FM_TDA7703_GATE_)||defined(_RADIO_FM_TDA778x_GATE_)||defined(_RADIO_FM_SI4732_GATE_)
#include "tuner_drv.h"
#endif

static ot2348_audio_t ot2348_audio;

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
#if (AUDIO_VOLUME_MAX == 63)
static const uint8 VolumeTab[64]=
{
	 240,0xA7,0xA3,0xA0,0x95,0x92,0x87,0x84,0x80,0x76,
	0x75,0x74,0x73,0x72,0x71,0x70,0x67,0x66,0x65,0x64,
	0x63,0x62,0x61,0x60,0x57,0x56,0x55,0x54,0x53,0x52,
	0x51,0x50,0x47,0x46,0x45,0x44,0x43,0x42,0x41,0x40,
	0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x27,0x26,
	0x25,0x24,0x23,0x22,0x21,0x20,0x17,0x16,0x15,0x14,
    0x13,0x12,0x11,0x10
};

#else

#if defined(_FY7000_VOLUME_STEP_)
#if defined(_FY1688_EVB_VOLUME_)
static const uint8 VolumeTab[41]=
{
	 240,0xA7,0xA3,0x95,0x90,0x85,0x80,0x75,0x70,0x65,
    0x60,0x55,0x53,0x51,0x49,0x47,0x45,0x43,0x40,0x38,
    0x36,0x34,0x32,0x30,0x28,0x26,0x24,0x22,0x20,0x18,
    0x17,0x16,0x16,0x15,0x14,0x14,0x13,0x12,0x11,0x11,
    0x10
};
#else
static const uint8 VolumeTab[41]=
{
	 240,0xA7,0xA3,0xA0,0x95,0x92,0x87,0x84,0x80,0x77,
    0x75,0x72,0x70,0x66,0x64,0x62,0x60,0x55,0x53,0x50,
    0x45,0x42,0x40,0x35,0x33,0x31,0x30,0x26,0x24,0x23,
    0x21,0x20,0x16,0x15,0x14,0x13,0x12,0x11,0x10,0x01,
    0x02
};
#endif

#elif defined(_LT3014_VOLUME_STEP_)
static const uint8 VolumeTab[41]=
{
	 240,0xA7,0xA3,0xA0,0x95,0x92,0x87,0x84,0x80,0x77,
    0x75,0x72,0x70,0x66,0x64,0x62,0x60,0x55,0x53,0x51,
    0x50,0x47,0x46,0x45,0x44,0x43,0x42,0x41,0x40,0x37,
    0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x27,0x26,0x25,
    0x24
};

#elif defined(_FY7000A_VOLUME_STEP_)
#if 1
static const uint8 VolumeTab[41]=
{
	 240,0xA7,0xA3,0xA0,0x95,0x92,0x87,0x84,0x80,0x77,
    0x75,0x72,0x70,0x66,0x64,0x62,0x60,0x55,0x53,0x50,
    0x46,0x45,0x43,0x42,0x41,0x40,0x37,0x36,0x35,0x34,
    0x33,0x32,0x31,0x30,0x25,0x24,0x23,0x21,0x20,0x16,
    0x15
};

#else
static const uint8 VolumeTab[41]=
{
	 240,0xA7,0xA3,0xA0,0x95,0x92,0x87,0x84,0x80,0x77,
    0x75,0x72,0x70,0x66,0x64,0x62,0x60,0x55,0x53,0x50,
    0x47,0x46,0x45,0x44,0x43,0x42,0x41,0x40,0x37,0x36,
    0x35,0x34,0x33,0x32,0x31,0x30,0x27,0x26,0x25,0x24,
    0x23
};
#endif
#else
#if (AUDIO_VOLUME_MAX == 31)
static const uint8 VolumeTab[32]=
{
	 240,0x97,0x92,0x85,0x80,0x74,0x71,0x65,0x61,0x55,
    0x51,0x45,0x42,0x40,0x36,0x34,0x32,0x30,
    0x26,0x24,0x22,0x20,0x17,0x16,
    0x15,0x14,0x13,0x12,0x11,0x10,0x01,0x02
};
#else
static const uint8 VolumeTab[41]=
{
	 240,0xA7,0xA3,0xA0,0x95,0x92,0x87,0x84,0x80,0x77,
    0x75,0x72,0x70,0x66,0x64,0x62,0x60,0x55,0x53,0x50,
    0x45,0x42,0x40,0x35,0x33,0x31,0x30,0x26,0x24,0x23,
    0x21,0x20,0x16,0x14,0x12,0x10,0x02,0x04,0x06,0x08,
    0x09
};
#endif
#endif
#endif

#if defined(_RADIO_FM_TDA7703_GATE_)||defined(_RADIO_FM_TDA778x_GATE_)
static const uint8 VolumeTab1[41]=
{
	 240,0xA7,0xA3,0xA0,0x95,0x92,0x87,0x84,0x80,0x77,
    0x77,0x75,0x75,0x72,0x70,0x66,0x64,0x62,0x57,0x55,
    0x50,0x47,0x44,0x42,0x40,0x34,0x33,0x32,0x31,0x27,
    0x27,0x26,0x25,0x24,0x23,0x21,0x20,0x16,0x15,0x14,
    0x13
};
#endif

#if defined(_RADIO_FM_SI4732_GATE_)
static const uint8 VolumeTab1[41]=
{
	 240,0xA7,0xA3,0xA0,0x95,0x92,0x87,0x84,0x80,0x77,
    0x75,0x72,0x70,0x66,0x64,0x62,0x60,0x55,0x53,0x50,
    0x45,0x42,0x40,0x35,0x33,0x31,0x30,0x26,0x24,0x23,
    0x21,0x20,0x16,0x15,0x14,0x13,0x12,0x12,0x11,0x10,
    0x01
};
#endif

#if (AUDIO_ATTEN_WIDE == 10)
static const uint8 PhTabe[]=
{
	0x0F,		/*0*/
	0x0A,		/*1*/
	0x05,		/*2*/
	0,			/*3*/
	0x14,		/*4*/
	0x26,		/*5*/
	0x40,		/*6*/
	0x53,		/*7*/
	0x65,		/*8*/
	0x76,		/*9*/
	240,		/*10*/
};
#elif (AUDIO_ATTEN_WIDE == 15)
static const uint8 PhTabe[]=
{
	0x0F,		/*0*/
	0x0E,		/*1*/
	0x0D,		/*2*/
	0x0C,		/*3*/
	0x0B,		/*4*/
	0x0A,		/*5*/
	0x07,		 /*6*/
	0x05,		/*7*/
	0x02,		 /*8*/
	0,		    /*9*/
	0x10,		/*10*/
	0x20,		/*6*/
	0x30,		/*7*/
	0x40,		/*8*/
	0x50,		/*9*/
	240,		/*10*/
};

#elif (AUDIO_ATTEN_WIDE == 7)
static const uint8 PhTabe[]=
{
	0x0F,		/*0*/
	0x0A,		/*1*/
	0x05,		/*2*/
	0,			/*3*/
	0x14,		/*4*/
	0x40,		/*5*/
	0x65,		/*6*/
	240,		/*7*/
};
#endif

#if defined(FY_SPECIAL_BAS_TRE_)
static const uint8 ToneTabe[]=
{
    0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0X0A, 0X09, 0X08, 0x07, 0x06, 0X05, 0x04, 0X03, 0x02, 0X01,
	0x10,
	0x11 ,0X12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0X1E, 0x1F
};

#else
static const uint8 ToneTabe[]=
{
	0x0F, 0x0D, 0x0B, 0x09, 0x07, 0x05, 0x02,
	0x10,
	0x12 ,0x15, 0x17, 0x19, 0x1B, 0x1D, 0x1F
};
#endif
#if defined(_SUPPORT_SUBWOOFER_MODE2_)
static const uint8 SubVolumeTab[21]=
{
	 240, 0xA0,0x92,0x80,0x72,0x70,0x62,0x53,0x40,0x31,
	 0x26,0x21,0x20,0x12,0x02,0x04,0x06,0x08,0x0A,0x0C,
	 0x0F,
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
#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
static uint8 pt2348_audio_init;
#endif

#if defined(_ENABLE_AMP_LIGHT_)
static void audio_mute_ctrl(uint8 mute)
{
	AUDIO_MUTE_INIT();
#if defined(_ENABLE_RCA_MUTE_)
    RCA_MUTE_INIT();
#endif
	if(mute)
	{
	#if defined(_MUTE_ON_CLOSE_AMP_)
		if(pt2348_audio_init)
		{
			pt2348_audio_init = 0;
		}
	#if !defined(_DISABLE_AMP_OFF_)
		else
		{
			AMP_CONTROL_OFF();
		}
		OSTimeDly(16);
	#endif
	#else
		if(global.amp_power)
		{
			if(pt2348_audio_init)
			{
				pt2348_audio_init = 0;
			}
			else
			{
				AMP_CONTROL_OFF();
			}
			OSTimeDly(16);
			global.amp_power = FALSE;
		}
	#endif
	#if defined(_ENABLE_BEEP_)
		if(setup.aud_beep)
		{
			if(global.beep_time)
			{
		    #if defined(_ENABLE_AMP_MUTE_)
		    	if(global.sys_power)
		    	{
		    		AMP_CONTROL_OFF();
		    	}
		    #endif
				AUDIO_MUTE_OFF();
			}
			else
			{
				AUDIO_MUTE_ON();
			}
		}
		else
		{
		#if defined(_ENABLE_AMP_MUTE_)
			if(global.sys_power)
			{
				AMP_CONTROL_OFF();
			}
		#endif
			AUDIO_MUTE_ON();
		}
	#else
		AUDIO_MUTE_ON();
    #if defined(_ENABLE_RCA_MUTE_)
	   if(setup.aud_vol != 0)
	   {
	   	   RCA_MUTE_ON();
	   }
    #endif
	#endif
	#if defined(_SUPPORT_SUBWOOFER2_)&&0
		if(setup.aud_sub)
		{
			SYS_SUB_OFF();
		}
		if(setup.aud_sub1)
		{
			SYS_SUB2_OFF();
		}
	#endif
	}
	else
	{
		AUDIO_MUTE_OFF();
    #if defined(_ENABLE_RCA_MUTE_)
    	RCA_MUTE_OFF();
    #endif
	#if defined(_MUTE_ON_CLOSE_AMP_)
	#if !defined(_DISABLE_AMP_OFF_)
    	OSTimeDly(5);
    	AMP_CONTROL_ON();
    #endif
	#else
		if(global.amp_power)
		{
			OSTimeDly(16);
			AMP_CONTROL_ON();
			global.amp_power = FALSE;
		}
	#endif
    #if defined(_ENABLE_AMP_MUTE_)
    	if(global.sys_power)
    	{
    		AMP_CONTROL_ON();
    	}
    #endif
	#if defined(_SUPPORT_SUBWOOFER2_)&&0
		if(setup.aud_sub)
		{
			SYS_SUB_ON();
		}
		if(setup.aud_sub1)
		{
			SYS_SUB2_ON();
		}
	#endif
	}
}

#else
static void audio_mute_ctrl(uint8 mute)
{
	AUDIO_MUTE_INIT();
	if(mute)
	{
    #if defined(_SUPPORT_AMP_CONTROL_)
    	if(pt2348_audio_init)
    	{
    		pt2348_audio_init = 0;
    	}
    	else
    	{
    		AMP_CONTROL_OFF();
    	}
    	if(global.number_flag)
    	{
    		global.number_flag = 0 ;
    	}
    	else
    	{
    		OSTimeDly(16);
    	}	
    	AUDIO_MUTE_ON();
    	if(setup.aud_sub)
    	{
    		SYS_SUB_OFF();
    	}
    	if(setup.aud_sub1)
    	{
    		SYS_SUB2_OFF();
    	}
	#elif defined(_ENABLE_BEEP_)
		if(setup.aud_beep)
		{
			if(global.beep_time)
			{
				AUDIO_MUTE_OFF();
			#if defined(SET_BEEP_FUNCV)
				global.beep_mute_flag = TRUE;
			#endif
			}
			else
			{
				AUDIO_MUTE_ON();
			}
		}
		else
		{
			AUDIO_MUTE_ON();
		}
    #else
    	AUDIO_MUTE_ON();
    #endif
    #if defined(_ENABLE_SUB_MUTE_)
    	if(setup.aud_sub)
    	{
    		SYS_SUB_OFF();
    	}
    	if(setup.aud_sub1)
    	{
    		SYS_SUB2_OFF();
    	}
    #endif
	}
	else
	{
		AUDIO_MUTE_OFF();
    #if defined(_SUPPORT_AMP_CONTROL_)
    	if(setup.aud_sub)
    	{
    		SYS_SUB_ON();
    	}
    	if(setup.aud_sub1)
    	{
    		SYS_SUB2_ON();
    	}
    	if(global.number_flag)
    	{
    		global.number_flag = 0 ;
    	}
    	else
    	{
    		OSTimeDly(16);
    	}	
    	AMP_CONTROL_ON();
    #endif
	}
}
#endif

#if defined(_SUPPORT_SUBWOOFER_)
static void audio_subwoofer_ctrl(uint8 subwoofer)
{
	if(pt2348_audio.subwoofer_last != subwoofer)
	{
		pt2348_audio.subwoofer_last = subwoofer;
	}
}
#endif

static uint8 audio_switch_value(void)
{
	uint8 value;
	
	value = ((uint8)ot2348_audio.channel)&0x07;
	if(ot2348_audio.gate == AUDIO_GATE_PLUS_11_25)
	{
		value |= 0x78;
	}
	else if(ot2348_audio.gate == AUDIO_GATE_PLUS_7_5)
	{
		value |=  0x38;	//
	}
	else if(ot2348_audio.gate == AUDIO_GATE_PLUS_3_75)
	{
		value |=  0x18;
	}
	else if(ot2348_audio.gate == AUDIO_GATE_PLUS_0)
	{
		value |= 0x00;
	}
	//ew_printf("audio_switch_value:%d\n",value);
	return value;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

void audio_reg_init(void)
{
	memset(&ot2348_audio, 0, sizeof(ot2348_audio));
		
	ot2348_audio.channel 	= 3;
	ot2348_audio.loud 		= AUDIO_LOUDNESS_OFF;
#if defined(_SUPPORT_SUBWOOFER_)
	ot2348_audio.subwoofer	= AUDIO_SUBWOOFER_OFF;
#endif
	ot2348_audio.sub_ctrl 	= CUT_OFF_80;
	ot2348_audio.gate 		= AUDIO_GATE_PLUS_7_5;
	ot2348_audio.volume 	= AUDIO_VOLUME_DEFAULT;
	ot2348_audio.mute_last 	= ot2348_audio.mute = ENABLE;
#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
	pt2348_audio_init = TRUE;
#endif
	
	audio_mute_ctrl(ENABLE);
}

void audio_reg_set(void)
{
	uint8  mute_flag;
	//uint8  subwoofer;
	uint8 index;
	int8  audio_reg[OT2348_REG_SIZE];
	
	if((ot2348_audio.mute)||(!ot2348_audio.volume))
	{
		mute_flag = ENABLE;
	}
	else 
	{
		mute_flag = DISABLE;
	}
	
/*	if(mute_flag ||(pt2348_audio.subwoofer == AUDIO_SUBWOOFER_OFF))
	{
		subwoofer = OFF;
	}
	else
	{
		subwoofer = ON;
	}
*/	
	if(ot2348_audio.mute_last != mute_flag)
	{
		if(mute_flag)
		{
			audio_mute_ctrl(TRUE);
			
			OSTimeDly(2);
		}
	}

	memset(audio_reg, 0, sizeof(audio_reg));
	
#if defined(_ENABLE_RCA_MUTE_)
	if((ot2348_audio.volume != 0)&&(mute_flag))
	{
		audio_reg[OT2348_REG_SW] 		= 0x05;
		audio_reg[OT2348_REG_SUB] 		= 0x3f; 
		audio_reg[OT2348_REG_VOLUME] 	= VolumeTab[0];
		audio_reg[OT2348_REG_BASS] 		= 0xf0; 
		audio_reg[OT2348_REG_TREB] 		= 0x00; 
		audio_reg[OT2348_REG_MIX] 		= 
		audio_reg[OT2348_REG_LF] 		= 
		audio_reg[OT2348_REG_RF] 		= 
		audio_reg[OT2348_REG_LR]		= 
		audio_reg[OT2348_REG_RR] 		=
		audio_reg[OT2348_REG_SUBTEN] 	= 0xf0;
		audio_reg[OT2348_REG_2SW] 		= 0x05;
	}
	else
#else
	if(mute_flag)
	{
		audio_reg[OT2348_REG_SW] 		= 0x05;
		audio_reg[OT2348_REG_SUB] 		= 0x3f; 
		audio_reg[OT2348_REG_VOLUME] 	= VolumeTab[0];
		audio_reg[OT2348_REG_BASS] 		= 0xf0; 
		audio_reg[OT2348_REG_TREB] 		= 0x00; 
		audio_reg[OT2348_REG_MIX] 		= 
		audio_reg[OT2348_REG_LF] 		= 
		audio_reg[OT2348_REG_RF] 		= 
		audio_reg[OT2348_REG_LR]		= 
		audio_reg[OT2348_REG_RR] 		=
		audio_reg[OT2348_REG_SUBTEN] 	= 0xf0;
		audio_reg[OT2348_REG_2SW] 		= 0x05;
	}
	else
#endif
	{
		/*---------------------------------------------*/
		audio_reg[OT2348_REG_SW]  = (0x7F&audio_switch_value());
		audio_reg[OT2348_REG_SUB] = 0x0F;
		audio_reg[OT2348_REG_SUB] |= (((~ot2348_audio.loud)&0x01)<<4);
		audio_reg[OT2348_REG_SUB] |= (ot2348_audio.sub_ctrl<<5);

	#if defined(_RADIO_FM_TDA7703_GATE_)
		if ((compare_task_name("FMRadioTask"))&&(global.tuner_drive == TUN_DRV_TDA770x))
		{
			audio_reg[OT2348_REG_VOLUME] = VolumeTab1[ot2348_audio.volume];
		}
		else
		{
			audio_reg[OT2348_REG_VOLUME] = VolumeTab[ot2348_audio.volume];
		}
	#elif defined(_RADIO_FM_TDA778x_GATE_)
		if ((compare_task_name("FMRadioTask"))&&(global.tuner_drive == TUN_DRV_TDA778x))
		{
			audio_reg[OT2348_REG_VOLUME] = VolumeTab1[ot2348_audio.volume];
		}
		else
		{
			audio_reg[OT2348_REG_VOLUME] = VolumeTab[ot2348_audio.volume];
		}
	#elif defined(_RADIO_FM_SI4732_GATE_)
		if ((compare_task_name("FMRadioTask"))&&(global.tuner_drive == TUN_DRV_SI473x))
		{
			audio_reg[OT2348_REG_VOLUME] = VolumeTab1[ot2348_audio.volume];
		}
		else
		{
			audio_reg[OT2348_REG_VOLUME] = VolumeTab[ot2348_audio.volume];
		}	
	#else
		audio_reg[OT2348_REG_VOLUME] = VolumeTab[ot2348_audio.volume];
	#endif
	
		if(ot2348_audio.eq == AUDIO_EQ_OFF)
		{
			audio_reg[OT2348_REG_BASS] = ot2348_audio.bass;
			audio_reg[OT2348_REG_TREB] = ot2348_audio.treble;
		}
		else
		{
			audio_reg[OT2348_REG_BASS] = EqTabe[ot2348_audio.eq - 1][0];
			audio_reg[OT2348_REG_TREB] = EqTabe[ot2348_audio.eq - 1][1];	
		}		
  #if defined(FY_SPECIAL_BAS_TRE_)
        audio_reg[OT2348_REG_BASS] = (ToneTabe[audio_reg[OT2348_REG_BASS]+15])&0x1F;
        audio_reg[OT2348_REG_TREB] = (ToneTabe[audio_reg[OT2348_REG_TREB]+15])&0x1F;
  #else
		audio_reg[OT2348_REG_BASS] = (ToneTabe[audio_reg[OT2348_REG_BASS]+7])&0x1F;
		audio_reg[OT2348_REG_TREB] = (ToneTabe[audio_reg[OT2348_REG_TREB]+7])&0x1F;
  #endif
		audio_reg[OT2348_REG_BASS] |= 0xE0;
		audio_reg[OT2348_REG_MIX]  = 0xf0;
		/*---------------------------------------------*/
		if(ot2348_audio.lr > 0)
		{
			audio_reg[OT2348_REG_LR] = (uint8)(ot2348_audio.lr);
			audio_reg[OT2348_REG_LF] = (uint8)(ot2348_audio.lr); 
		}
		else
		{
			audio_reg[OT2348_REG_RR] = (uint8)(-ot2348_audio.lr);
			audio_reg[OT2348_REG_RF] = (uint8)(-ot2348_audio.lr);
		}
		
		if(ot2348_audio.fr > 0)
		{
			audio_reg[OT2348_REG_LR]  += (uint8)(ot2348_audio.fr);
			audio_reg[OT2348_REG_RR]  += (uint8)(ot2348_audio.fr);
		}
		else
		{
			audio_reg[OT2348_REG_LF] += (uint8)(-ot2348_audio.fr);
			audio_reg[OT2348_REG_RF] += (uint8)(-ot2348_audio.fr);
		}
		
		for(index = OT2348_REG_LF; index < (OT2348_REG_LF + 4); index ++)
		{
			if(audio_reg[index] > AUDIO_ATTEN_WIDE)
				audio_reg[index] = AUDIO_ATTEN_WIDE;
			audio_reg[index] = PhTabe[ audio_reg[index]];
		}
		
	#if defined(_SUPPORT_SUBWOOFER_)
		if(ot2348_audio.subwoofer == AUDIO_SUBWOOFER_ON)
		{
			audio_reg[OT2348_REG_SUBTEN] = 0x0f;		//VolumeTab[pt2348_audio.volume];//0x00;
		}
		else
		{
			audio_reg[OT2348_REG_SUBTEN] = 0xf0;
		}
	#elif defined(_SUPPORT_SUBWOOFER_MODE2_)
		audio_reg[OT2348_REG_SUBTEN] = SubVolumeTab[ot2348_audio.sub_volume];
	#endif
		/*---------------------------------------------*/
		audio_reg[OT2348_REG_2SW] = (0x7F&audio_switch_value());
	}	
	
	audio_reg[OT2348_REG_ADR] = 0x00;//0x00;
	i2c_data_init(OT2348_I2C_ADDR, OT2348_I2C_RATE, &audio_gpio);
	i2c_data_w((uint8 *)audio_reg, sizeof(audio_reg));
	i2c_data_exit();
	
	if(ot2348_audio.mute_last != mute_flag)
	{
		if(!mute_flag)
		{			
			audio_mute_ctrl(FALSE);
		}
	}
	
	ot2348_audio.mute_last = mute_flag;
	
}

uint8 audio_reg_ctrl(uint8 ctrl, int para)
{
	switch(ctrl)
	{
		/*!设置静音*/
		case AUD_CTRL_MUTE:
			{
				uint8 mute = (uint8)para;
				ot2348_audio.mute = mute;
			}
			break;
		/*!设置通道*/
		case AUD_CTRL_CHANNEL:
			{
				uint8 channel = (uint8)para;

				if(channel < AUDIO_CHANNEL_MAX)
				{
				#if defined(_SUPPORT_DISC_)
    				switch(channel)
    				{
    					case AUDIO_CHANNEL_RADIO_ON:
    					case AUDIO_CHANNEL_RADIO_OFF:
    						if(fm_inside_check())
    						{
    							ot2348_audio.channel = 2;
    						}
    						else
    						{
    							ot2348_audio.channel = 2;
    						}
    						break;
    						
						case AUDIO_CHANNEL_MUSIC_ON:
						case AUDIO_CHANNEL_MUSIC_OFF:
						case AUDIO_CHANNEL_AUX_ON:
						case AUDIO_CHANNEL_AUX_OFF:
							ot2348_audio.channel = 3;
							break;
							
						case AUDIO_CHANNEL_DISC_ON:
						case AUDIO_CHANNEL_DISC_OFF:
							ot2348_audio.channel = 4;
							break;
    				}
				#else
					switch(channel)
					{
						case AUDIO_CHANNEL_RADIO_ON:
						case AUDIO_CHANNEL_RADIO_OFF:
							if(fm_inside_check())
							{
								ot2348_audio.channel = 2;
							}
							else
							{
								ot2348_audio.channel = 2;
							}
							break;
							
						case AUDIO_CHANNEL_MUSIC_ON:
						case AUDIO_CHANNEL_MUSIC_OFF:
							ot2348_audio.channel = 4;
							break;
						
					#if defined(_ENABLE_DAB_OT2348_)
						case AUDIO_CHANNEL_DAB_ON:
						case AUDIO_CHANNEL_DAB_OFF:
							ot2348_audio.channel = 1;
							break;
					#endif

						case AUDIO_CHANNEL_AUX_ON:
						case AUDIO_CHANNEL_AUX_OFF:
							ot2348_audio.channel = 3;
							break;
					}
				#endif
				}
			}
			break;
		/*!设置放大*/
		case AUD_CTRL_GATE:
			{
				uint8 gate = (uint8)para;
				if(gate < AUDIO_GATE_PLUS_MAX)
				{
					ot2348_audio.gate = gate;
				}
			}
			break;
		/*!设置响度*/
		case AUD_CTRL_LOUD:
			{
				uint8 loud = (uint8)para;
				if(loud < AUDIO_LOUDNESS_MAX)
				{
					ot2348_audio.loud = loud;
					ew_printf("ot2348_audio.loud:%d\n",ot2348_audio.loud);
				}
			}
			break;
		/*!设置音效*/
		case AUD_CTRL_EQ:
			{
				uint8 eq = (uint8)para;
				if(eq < AUDIO_EQ_MAX)
				{
					ot2348_audio.eq = eq;
				}
			}
			break;
		/*!设置音量*/
		case AUD_CTRL_VOLUME:
			{
				uint8 volume = (uint8)para;
				if(volume > AUDIO_VOLUME_MAX)
				{
					ot2348_audio.volume = AUDIO_VOLUME_MAX;
				}
				else
				{
					ot2348_audio.volume = volume;
				}
			}
			break;
		/*!设置低音*/
		case AUD_CTRL_BASS:
			{
				int8 bass = (int8)para;
				if(bass > AUDIO_BASS_MAX)
				{
					ot2348_audio.bass = AUDIO_BASS_MAX;
				}
				else if(bass < AUDIO_BASS_MIN)
				{
					ot2348_audio.bass = AUDIO_BASS_MIN;
				}
				else
				{
					ot2348_audio.bass = bass;
				}
			}
			break;
		/*!设置高音*/
		case AUD_CTRL_TREBLE:
			{
				int8 treble = (int8)para;
				if(treble > AUDIO_TREBLE_MAX)
				{
					ot2348_audio.treble = AUDIO_TREBLE_MAX;
				}
				else if(treble < AUDIO_TREBLE_MIN)
				{
					ot2348_audio.treble = AUDIO_TREBLE_MIN;
				}
				else
				{
					ot2348_audio.treble = treble;
				}
			}
			break;
		/*!设置左右平衡*/
		case AUD_CTRL_LR:
			{
				int8 lr = (int8)para;
				if(lr > AUDIO_LR_MAX)
				{
					ot2348_audio.lr = AUDIO_LR_MAX;
				}
				else if(lr < AUDIO_LR_MIN)
				{
					ot2348_audio.lr = AUDIO_LR_MIN;
				}
				else
				{
					ot2348_audio.lr = lr;
				}
			}
			break;
		/*!设置前后平衡*/
		case AUD_CTRL_FR:
			{
				int8 fr = (int8)para;
				if(fr > AUDIO_FR_MAX)
				{
					ot2348_audio.fr = AUDIO_FR_MAX;
				}
				else if(fr < AUDIO_FR_MIN)
				{
					ot2348_audio.fr = AUDIO_FR_MIN;
				}
				else
				{
					ot2348_audio.fr = fr;
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
					ot2348_audio.subwoofer = subwoofer;
					ew_printf("ot2348_audio.subwoofer:%d\n",pt2348_audio.subwoofer);
				}
			}
			break;
	#elif defined(_SUPPORT_SUBWOOFER_MODE2_)
		case AUD_CTRL_SUBWOOFER:
			{
				uint8 sub_vol = (uint8)para;
				if(sub_vol > AUDIO_SUB_VOL_MAX)
				{
					ot2348_audio.sub_volume = AUDIO_SUB_VOL_MAX;
				}
				else
				{
					ot2348_audio.sub_volume = sub_vol;
				}
			}
			break;
	#endif

	#if defined(AUDIO_SUB_LEVEL_SET)
		case AUD_CTRL_SUB_CUT:
			{
				uint8 sub_ctrl = (uint8)para;
				if(sub_ctrl > AUDIO_SUBWOOFER_CUT_160)
				{
					ot2348_audio.sub_ctrl = AUDIO_SUBWOOFER_CUT_160+1;
				}
				else
				{
					ot2348_audio.sub_ctrl = sub_ctrl+1;
				}
			}
			break;
	#endif
	
	#if defined(_SUPPORT_10_EQ_FUNTION_)
    	case AUD_CTRL_60HZ:
    		{
    			int8 eq60hz = (int8)para;
    			if(eq60hz > AUDIO_60HZ_MAX)
    			{
    				ot2348_audio.bass = AUDIO_60HZ_MAX;
    			}
    			else if(eq60hz < AUDIO_60HZ_MIN)
    			{
    				ot2348_audio.bass = AUDIO_60HZ_MIN;
    			}
    			else
    			{
    				ot2348_audio.bass = eq60hz;
    			}
    		}
    		break;
		case AUD_CTRL_150HZ:
			{
				int8 eq150hz = (int8)para;
				if(eq150hz > AUDIO_150HZ_MAX)
				{
					ot2348_audio.bass = AUDIO_150HZ_MAX;
				}
				else if(eq150hz < AUDIO_60HZ_MIN)
				{
					ot2348_audio.bass = AUDIO_150HZ_MIN;
				}
				else
				{
					ot2348_audio.bass = eq150hz;
				}
			}
			break;
		case AUD_CTRL_400HZ:
			{
				int8 eq400hz = (int8)para;
				if(eq400hz > AUDIO_400HZ_MAX)
				{
					ot2348_audio.bass = AUDIO_400HZ_MAX;
				}
				else if(eq400hz < AUDIO_400HZ_MIN)
				{
					ot2348_audio.bass = AUDIO_400HZ_MIN;
				}
				else
				{
					ot2348_audio.bass = eq400hz;
				}
			}
			break;
		case AUD_CTRL_1KHZ:
			{
				int8 eq1khz = (int8)para;
				if(eq1khz > AUDIO_1KHZ_MAX)
				{
					ot2348_audio.bass = AUDIO_1KHZ_MAX;
				}
				else if(eq1khz < AUDIO_1KHZ_MIN)
				{
					ot2348_audio.bass = AUDIO_1KHZ_MIN;
				}
				else
				{
					ot2348_audio.bass = eq1khz;
				}
			}
			break;
		case AUD_CTRL_2KHZ:
			{
				int8 eq2khz = (int8)para;
				if(eq2khz > AUDIO_2KHZ_MAX)
				{
					ot2348_audio.bass = AUDIO_2KHZ_MAX;
				}
				else if(eq2khz < AUDIO_1KHZ_MIN)
				{
					ot2348_audio.bass = AUDIO_1KHZ_MIN;
				}
				else
				{
					ot2348_audio.bass = eq2khz;
				}
			}
			break;
		case AUD_CTRL_3KHZ:
			{
				int8 eq3khz = (int8)para;
				if(eq3khz > AUDIO_3KHZ_MAX)
				{
					ot2348_audio.treble = AUDIO_3KHZ_MAX;
				}
				else if(eq3khz < AUDIO_1KHZ_MIN)
				{
					ot2348_audio.treble = AUDIO_3KHZ_MIN;
				}
				else
				{
					ot2348_audio.treble = eq3khz;
				}
			}
			break;
		case AUD_CTRL_7KHZ:
			{
				int8 eq7khz = (int8)para;
				if(eq7khz > AUDIO_7KHZ_MAX)
				{
					ot2348_audio.treble = AUDIO_7KHZ_MAX;
				}
				else if(eq7khz < AUDIO_7KHZ_MIN)
				{
					ot2348_audio.treble = AUDIO_7KHZ_MIN;
				}
				else
				{
					ot2348_audio.treble = eq7khz;
				}
			}
			break;
		case AUD_CTRL_10KHZ:
			{
				int8 eq10khz = (int8)para;
				if(eq10khz > AUDIO_10KHZ_MAX)
				{
					ot2348_audio.treble = AUDIO_10KHZ_MAX;
				}
				else if(eq10khz < AUDIO_10KHZ_MIN)
				{
					ot2348_audio.treble = AUDIO_10KHZ_MIN;
				}
				else
				{
					ot2348_audio.treble = eq10khz;
				}
			}
			break;
		case AUD_CTRL_15KHZ:
			{
				int8 eq15khz = (int8)para;
				if(eq15khz > AUDIO_15KHZ_MAX)
				{
					ot2348_audio.treble = AUDIO_15KHZ_MAX;
				}
				else if(eq15khz < AUDIO_1KHZ_MIN)
				{
					ot2348_audio.treble = AUDIO_15KHZ_MIN;
				}
				else
				{
					ot2348_audio.treble = eq15khz;
				}
			}
			break;
		case AUD_CTRL_20KHZ:
			{
				int8 eq20khz = (int8)para;
				if(eq20khz > AUDIO_1KHZ_MAX)
				{
					ot2348_audio.treble = AUDIO_20KHZ_MAX;
				}
				else if(eq20khz < AUDIO_20KHZ_MIN)
				{
					ot2348_audio.treble = AUDIO_20KHZ_MIN;
				}
				else
				{
					ot2348_audio.treble = eq20khz;
				}
			}
			break;
	#endif
	
	#if defined(_SUPPORT_SOUND_RECOVER_)
		/*!*/
		case AUD_CTRL_SOUND_RECOVER:
			{
				uint8 sound_recover = (uint8)para;
				if(sound_recover == AUDIO_SOUND_RECOV_ON)
				{
					memset(&ot2348_audio.bass, 0,5*sizeof(int));
				}
			}
			break;
	#endif
		/*!得到当前通道*/
		case AUD_GET_CHANNEL:
			*((uint8 *)para) = ot2348_audio.channel;
			
			return FALSE;
			
		default:
			return FALSE;
	}
	
	return TRUE;
}


