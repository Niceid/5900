#ifndef __AUDIO_DRV_H__
#define __AUDIO_DRV_H__



#define AUD_DRV_DEBUG
#ifdef AUD_DRV_DEBUG
#define aud_drv_printf 			ew_printf
#define aud_drv_puts 			ew_puts
#else
#define aud_drv_printf(...)
#define aud_drv_puts(...)
#endif


#if defined(_SUPPORT_SUBWOOFER_)||defined(_ENABLE_AP3768_SUBWOOFER_)
enum{
	AUDIO_SUBWOOFER_OFF = 0,
	AUDIO_SUBWOOFER_ON,
	AUDIO_SUBWOOFER_MAX,
};
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
enum{
	AUDIO_SUBWOOFER_OFF = 0,
	AUDIO_SUBWOOFER_ON,
	AUDIO_SUBWOOFER_MAX,
};
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
#if !defined(AUDIO_SUB_VOL_MAX)
#define AUDIO_SUB_VOL_MAX			(20)
#endif
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
enum{
	AUDIO_SUBWOOFER_CUT_80= 0,
	AUDIO_SUBWOOFER_CUT_120,
	AUDIO_SUBWOOFER_CUT_160,
};
#endif

enum{
	AUDIO_CHANNEL_RADIO_ON = 0,
	AUDIO_CHANNEL_RADIO_OFF,
	
	AUDIO_CHANNEL_MUSIC_ON,
	AUDIO_CHANNEL_MUSIC_OFF,
	
	AUDIO_CHANNEL_AUX_ON,
	AUDIO_CHANNEL_AUX_OFF,

#if defined(_SUPPORT_AUX1_)	
	AUDIO_CHANNEL_AUX1_ON,
	AUDIO_CHANNEL_AUX1_OFF,
#endif

#if defined(_SUPPORT_DISC_)
	AUDIO_CHANNEL_DISC_ON,
	AUDIO_CHANNEL_DISC_OFF,
#endif
#if defined(ENABLE_PT2312_CHANNEL)||defined(_ENABLE_DAB_VOLUME_)||defined(ENABLE_DAB_PT2313_CHANNEL)||defined(ENABLE_PT2313_CHANNEL)||defined(_ENABLE_DAB_OT2348_)
	AUDIO_CHANNEL_DAB_ON,
	AUDIO_CHANNEL_DAB_OFF,
#endif
#if defined(_SUPPORT_TAPE_)
    AUDIO_CHANNEL_TAPE_ON,
    AUDIO_CHANNEL_TAPE_OFF,
#endif
#if defined(_SUPPORT_MIC_OK_)
    AUDIO_CHANNEL_MIC_ON,
    AUDIO_CHANNEL_MIC_OFF,
#endif
#if defined(_SUPPORT_REVERSE_MODEL_)
    AUDIO_CHANNEL_REV_ON,
    AUDIO_CHANNEL_REV_OFF,
#endif
#if defined(_AUDIO_DRIVER_AP3768_)
	AUDIO_CHANNEL_RDS_ON,
	AUDIO_CHANNEL_RDS_OFF,
#endif

#if defined(_ENABLE_MODE_CHANNEL_)
	AUDIO_CHANNEL_OPTICAL_ON,
	AUDIO_CHANNEL_OPTICAL_OFF,

	AUDIO_CHANNEL_TV_ON,
	AUDIO_CHANNEL_TV_OFF,

	AUDIO_CHANNEL_DVD_ON,
	AUDIO_CHANNEL_DVD_OFF,
#endif
	AUDIO_CHANNEL_MAX,
};

enum{
	AUDIO_LOUDNESS_OFF = 0,	
	AUDIO_LOUDNESS_ON,
	AUDIO_LOUDNESS_MAX,
};

enum{
	AUDIO_GATE_PLUS_11_25 = 0,
	AUDIO_GATE_PLUS_7_5,
	AUDIO_GATE_PLUS_3_75,
	AUDIO_GATE_PLUS_0,
	AUDIO_GATE_PLUS_MAX,
};

#if defined(_ENABLE_AP3768_EQ_MODE_)
enum{
	AUDIO_EQ_OFF = 0,
	AUDIO_EQ_FLAT,
	AUDIO_EQ_CLASSIC,
	AUDIO_EQ_POP,
	AUDIO_EQ_ROCK,
	AUDIO_EQ_JAZZ,
	AUDIO_EQ_PEOPLE,
	AUDIO_EQ_SOFT,
	AUDIO_EQ_ELECTRIC,
	AUDIO_EQ_MAX,
};

#else
enum{
	AUDIO_EQ_OFF = 0,
	AUDIO_EQ_FLAT,
	AUDIO_EQ_CLASSIC,
	AUDIO_EQ_POP,
	AUDIO_EQ_ROCK,
	AUDIO_EQ_JAZZ,
	AUDIO_EQ_MAX,
};
#endif

#define AUDIO_VOLUME_MIN					(0)
#if !defined(AUDIO_VOLUME_MAX)
#define AUDIO_VOLUME_MAX					(40)
#endif
#if !defined(AUDIO_VOLUME_DEFAULT)
#define AUDIO_VOLUME_DEFAULT				(16)
#endif

#if defined(_CHANGE_DEF_VOL_)
#define CHANGE_VOLUME_DEFAULT				(28)
#endif

#if defined(_SPECIAL_TL2000_SET_)
#define AUDIO_A_VOLUME_MIN					(0)
#define AUDIO_B_VOLUME_MIN					(0)
#if defined(AUDIO_VOLUME_MAX_30_)
#define AUDIO_A_VOLUME_MAX					(30)
#define AUDIO_B_VOLUME_MAX					(30)
#else
#define AUDIO_A_VOLUME_MAX					(50)
#define AUDIO_B_VOLUME_MAX					(50)
#endif
#define AUDIO_A_VOLUME_DEFAULT				(15)
#define AUDIO_B_VOLUME_DEFAULT				(15)
#endif

#if !defined(AUDIO_DECODE_VOLUME_MAX)
#define AUDIO_DECODE_VOLUME_MAX				(30)
#endif

#if defined(HuiNuo_6651D_Copy)
#define AUDIO_BASS_MIN						(-6)
#define AUDIO_BASS_MAX					 	(6)
#define AUDIO_BASS_DEFAULT					(0)

#define AUDIO_TREBLE_MIN					(-6)
#define AUDIO_TREBLE_MAX					(6)
#define AUDIO_TREBLE_DEFAULT				(0)
#elif defined(FY_SPECIAL_BAS_TRE_)
#define AUDIO_BASS_MIN						(-15)
#define AUDIO_BASS_MAX					 	(15)
#define AUDIO_BASS_DEFAULT					(0)

#define AUDIO_TREBLE_MIN					(-15)
#define AUDIO_TREBLE_MAX					(15)
#define AUDIO_TREBLE_DEFAULT				(0)

#elif defined(_SPECIAL_PT2313_SET_)
#define AUDIO_BASS_MIN						(-12)
#define AUDIO_BASS_MAX					 	(12)
#define AUDIO_BASS_DEFAULT					(0)

#define AUDIO_TREBLE_MIN					(-12)
#define AUDIO_TREBLE_MAX					(12)
#define AUDIO_TREBLE_DEFAULT				(0)
#else
#define AUDIO_BASS_MIN						(-7)
#define AUDIO_BASS_MAX					 	(7)
#define AUDIO_BASS_DEFAULT					(0)

#define AUDIO_TREBLE_MIN					(-7)
#define AUDIO_TREBLE_MAX					(7)
#define AUDIO_TREBLE_DEFAULT				(0)
#endif

#if defined(_ENABLE_7419_MIDDLE_)
#define AUDIO_MIDDLE_MIN					(-7)
#define AUDIO_MIDDLE_MAX					 (7)
#define AUDIO_MIDDLE_DEFAULT				(0)
#endif

#if defined(HuiNuo_6651D_Copy)
#define AUDIO_ATTEN_WIDE					(6)
#elif defined(TD_ADD_LCD_NUM_4)||defined(_FIVE_CHAR_DISPLAY_)||defined(_CHANGE_AUDIO_ATTEN_WIDE_)||defined(_LCD_3_DIAPLAY_)
#define AUDIO_ATTEN_WIDE					(7)
#elif defined(_EVB_SPECIAL_DISPLAY_)
#define AUDIO_ATTEN_WIDE					(7)
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
#define AUDIO_ATTEN_WIDE					(7)
#elif defined(_XM003_SPECIAL_DISPLAY_)
#define AUDIO_ATTEN_WIDE					(7)
#elif defined(_WLS316_SPECIAL_DISPLAY_)
#define AUDIO_ATTEN_WIDE					(7)
#elif defined(_SPECIAL_PT2313_SET_)
#define AUDIO_ATTEN_WIDE					(12)
#elif defined(FY_SPECIAL_BAS_TRE_)
#define AUDIO_ATTEN_WIDE					(15)
#elif defined(_FAD_BAL_FUNTION_)
#define AUDIO_ATTEN_WIDE					(50)
#else
#define AUDIO_ATTEN_WIDE					(10)
#endif
#define AUDIO_LR_MIN						(-AUDIO_ATTEN_WIDE)
#define AUDIO_LR_MAX					 	(AUDIO_ATTEN_WIDE)
#define AUDIO_LR_DEFAULT					(0)

#define AUDIO_FR_MIN						(-AUDIO_ATTEN_WIDE)
#define AUDIO_FR_MAX					 	(AUDIO_ATTEN_WIDE)
#define AUDIO_FR_DEFAULT					(0)

#if defined(_SUPPORT_10_EQ_FUNTION_)
#define AUDIO_60HZ_MIN						(-7)
#define AUDIO_60HZ_MAX					 	(7)
#define AUDIO_60HZ_DEFAULT					(0)

#define AUDIO_150HZ_MIN						(-7)
#define AUDIO_150HZ_MAX					 	(7)
#define AUDIO_150HZ_DEFAULT					(0)

#define AUDIO_400HZ_MIN						(-7)
#define AUDIO_400HZ_MAX					 	(7)
#define AUDIO_400HZ_DEFAULT					(0)

#define AUDIO_1KHZ_MIN						(-7)
#define AUDIO_1KHZ_MAX					 	(7)
#define AUDIO_1KHZ_DEFAULT					(0)

#define AUDIO_2KHZ_MIN						(-7)
#define AUDIO_2KHZ_MAX					 	(7)
#define AUDIO_2KHZ_DEFAULT					(0)

#define AUDIO_3KHZ_MIN						(-7)
#define AUDIO_3KHZ_MAX					 	(7)
#define AUDIO_3KHZ_DEFAULT					(0)

#define AUDIO_7KHZ_MIN						(-7)
#define AUDIO_7KHZ_MAX					 	(7)
#define AUDIO_7KHZ_DEFAULT					(0)

#define AUDIO_10KHZ_MIN						(-7)
#define AUDIO_10KHZ_MAX					 	(7)
#define AUDIO_10KHZ_DEFAULT					(0)

#define AUDIO_15KHZ_MIN						(-7)
#define AUDIO_15KHZ_MAX					 	(7)
#define AUDIO_15KHZ_DEFAULT					(0)

#define AUDIO_20KHZ_MIN						(-7)
#define AUDIO_20KHZ_MAX					 	(7)
#define AUDIO_20KHZ_DEFAULT					(0)
#endif

enum{
	/*!设置静音*/
	AUD_CTRL_MUTE = 0,
	/*!设置通道*/
	AUD_CTRL_CHANNEL,
	/*!设置放大*/
	AUD_CTRL_GATE,
	/*!设置响度*/
	AUD_CTRL_LOUD,
	/*!设置音效*/
	AUD_CTRL_EQ,
	/*!设置音量*/
	AUD_CTRL_VOLUME,
	/*!设置低音*/
	AUD_CTRL_BASS,
	/*!设置高音*/
	AUD_CTRL_TREBLE,
#if !defined(_FAD_BAL_FUNTION_)
	/*!设置左右平衡*/
	AUD_CTRL_LR,
	/*!设置前后平衡*/
	AUD_CTRL_FR,
#endif
#if defined(_ENABLE_7419_MIDDLE_)
	/*!设置中音*/
	AUD_CTRL_MIDDLE,
#endif
#if defined(_ENABLE_AP3768_SUBWOOFER_)
	AUD_CTRL_SUBWOOFER1,
	AUD_CTRL_SUBWOOFER2,
#endif
#if defined(_SUPPORT_SUBWOOFER_)||defined(_ENABLE_SUB_OUTPUT_)
	/*!设置重低音*/
	AUD_CTRL_SUBWOOFER,
#endif
#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	/*!设置重低音*/
	AUD_CTRL_SUBWOOFER,
#endif
#if defined(AUDIO_SUB_LEVEL_SET)
	AUD_CTRL_SUB_CUT,
#endif
#if defined(_FAD_BAL_FUNTION_)
	AUD_CTRL_FL,
	AUD_CTRL_FR,
	AUD_CTRL_RL,
	AUD_CTRL_RR,
#endif
#if defined(_SUPPORT_10_EQ_FUNTION_)
	AUD_CTRL_60HZ,
	AUD_CTRL_150HZ,
	AUD_CTRL_400HZ,
	AUD_CTRL_1KHZ,
	AUD_CTRL_2KHZ,
	AUD_CTRL_3KHZ,
	AUD_CTRL_7KHZ,
	AUD_CTRL_10KHZ,
	AUD_CTRL_15KHZ,
	AUD_CTRL_20KHZ,
#endif
	/*!得到当前通道*/
	AUD_GET_CHANNEL,
#if defined(_SPECIAL_TL2000_SET_)
	AUD_CTRL_A_VOLUME,
	AUD_CTRL_B_VOLUME,
#endif
};

extern uint8 audio_drv_channel(void);

extern void audio_drv_init(void);
extern void audio_drv_exit(void);
extern void audio_drv_ctrl(uint8, int, uint8);

#if defined(_SUPPORT_TWO_PT2313_)
extern void audio_drv_ctrl1(uint8, int, uint8);
#endif

#endif /* __AUDIO_DRV_H__ */

