#ifndef __TEA685x_H__
#define __TEA685x_H__

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#define TEA685x_I2C_ADDR					(0xC8)

#if defined(_ENABLE_TEA685x_INIT_)
#define TEA685x_I2C_RATE				    (60)//143K
#else
#define TEA685x_I2C_RATE				    (30)//232K

#endif

typedef enum
{
	TEA685x_Cmd_Tune_To = 1,
	TEA685x_Cmd_Set_Tune_Options =2,
	TEA685x_Cmd_Set_Bandwidth =10,
	TEA685x_Cmd_Set_RFAGC =11,
	TEA685x_Cmd_Set_Antenna =12,
	
	TEA685x_Cmd_Set_MphSuppression = 20,
	TEF668X_Cmd_Set_ChannelEqualizer = 22,	//only Lithio avaliable
	TEA685x_Cmd_Set_NoiseBlanker = 23,
	TEA685x_Cmd_Set_NoiseBlanker_Audio = 24,
	
	TEA685x_Cmd_Set_DigitalRadio = 30,
	TEA685x_Cmd_Set_Deemphasis = 31,
	TEF668X_Cmd_Set_StereoImprovement = 32,	//only Lithio avaliable
	
	TEA685x_Cmd_Set_LevelStep = 38,
	TEA685x_Cmd_Set_LevelOffset = 39,
	
	TEA685x_Cmd_Set_Softmute_Time = 40,
	TEA685x_Cmd_Set_Softmute_Mod = 41,
	TEA685x_Cmd_Set_Softmute_Level = 42,
	TEA685x_Cmd_Set_Softmute_Noise = 43,
	TEA685x_Cmd_Set_Softmute_Mph = 44,
	TEA685x_Cmd_Set_Softmute_Max = 45,
	
	TEA685x_Cmd_Set_Highcut_Time = 50,
	TEA685x_Cmd_Set_Highcut_Mod = 51,
	TEA685x_Cmd_Set_Highcut_Level = 52,
	TEA685x_Cmd_Set_Highcut_Noise = 53,
	TEA685x_Cmd_Set_Highcut_Mph = 54,
	TEA685x_Cmd_Set_Highcut_Max = 55,
	TEA685x_Cmd_Set_Highcut_Min = 56,
	TEA685x_Cmd_Set_Lowcut_Min = 58,
	
	TEA685x_Cmd_Set_Stereo_Time = 60,
	TEA685x_Cmd_Set_Stereo_Mod = 61,
	TEA685x_Cmd_Set_Stereo_Level = 62,
	TEA685x_Cmd_Set_Stereo_Noise = 63,
	TEA685x_Cmd_Set_Stereo_Mph = 64,
	TEA685x_Cmd_Set_Stereo_Max = 65,
	TEA685x_Cmd_Set_Stereo_Min = 66,
	
	TEF668X_Cmd_Set_StHiBlend_Time = 70,	//only Lithio avaliable
	TEF668X_Cmd_Set_StHiBlend_Mod = 71, //only Lithio avaliable
	TEF668X_Cmd_Set_StHiBlend_Level  = 72, //only Lithio avaliable
	TEF668X_Cmd_Set_StHiBlend_Noise  = 73, //only Lithio avaliable
	TEF668X_Cmd_Set_StHiBlend_Mph  = 74, //only Lithio avaliable
	TEF668X_Cmd_Set_StHiBlend_Max  = 75, //only Lithio avaliable
	TEF668X_Cmd_Set_StHiBlend_Min  = 76, //only Lithio avaliable
	
	TEA685x_Cmd_Set_Scaler = 80,
	TEA685x_Cmd_Set_RDS = 81,
	TEA685x_Cmd_Set_QualityStatus = 82,
	TEA685x_Cmd_Set_DR_Blend = 83,
	TEA685x_Cmd_Set_DR_Options = 84,
	TEA685x_Cmd_Set_Specials = 85,
	
	TEF668X_Cmd_Set_StBandBlend_Time = 90,	//only Lithio avaliable
	TEF668X_Cmd_Set_StBandBlend_Gain = 91, //only Lithio avaliable
	TEF668X_Cmd_Set_StBandBlend_Bias  = 92, //only Lithio avaliable
	
	TEA685x_Cmd_Get_Quality_Status = 128,
	TEA685x_Cmd_Get_Quality_Data = 129,
	TEA685x_Cmd_Get_RDS_Status = 130,
	TEA685x_Cmd_Get_RDS_Data = 131,
	TEA685x_Cmd_Get_AGC = 132,
	TEA685x_Cmd_Get_Signal_Status = 133,
	TEA685x_Cmd_Get_Processing_Status = 134,
	TEA685x_Cmd_Get_Interface_Status = 135,
} TEA685x_RADIO_COMMAND;

typedef enum
{
	TEA685x_Cmd_Set_Volume = 10,
	TEA685x_Cmd_Set_Mute = 11,
	TEA685x_Cmd_Set_Input = 12,
	TEA685x_Cmd_Set_Output_Source = 13,
	
	TEA685x_Cmd_Set_Ana_Out = 21,
	TEA685x_Cmd_Set_Dig_IO = 22,
	TEA685x_Cmd_Set_Input_Scaler = 23,
	TEA685x_Cmd_Set_WaveGen = 24
}TEA685x_AUDIO_COMMAND;

typedef enum
{
	TEA685x_Cmd_Set_OperationMode = 1,
	TEA685x_Cmd_Set_GPIO = 3,
	TEA685x_Cmd_Set_ReferenceClock = 4,
	TEA685x_Cmd_Activate = 5,

	TEA685x_Cmd_Get_Operation_Status = 128,
	TEA685x_Cmd_Get_GPIO_Status = 129,
	TEA685x_Cmd_Get_Identification = 130,
	TEA685x_Cmd_Get_LastWrite = 131
}TEA685x_APPL_COMMAND;


/***********************/
/* Write mode commands */
/***********************/

#define TEA685x_COMMAND_TUNETO                  0x01U
#define TEA685x_COMMAND_SETTUNEOPTIONS          0x02U
#define TEA685x_COMMAND_SETBANDWIDTH            0x0AU
#define TEA685x_COMMAND_SETRFAGC                0x0BU
#define TEA685x_COMMAND_SETANTENNA              0x0CU

#define TEA685x_COMMAND_SETMPHSUPPRESSION       0x14U
#define TEA685x_COMMAND_SETCHANNELEQUALIZER     0x16U
#define TEA685x_COMMAND_SETNOISEBLANKER         0x17U
#define TEA685x_COMMAND_SETNOISEBLANKERAUDIO    0x18U

#define TEA685x_COMMAND_SETDIGITALRADIO         0x1EU
#define TEA685x_COMMAND_SETDEEMPHASIS           0x1FU

#define TEA685x_COMMAND_SETLEVELOFFSET          0x27U

#define TEA685x_COMMAND_SETSOFTMUTETIME         0x28U
#define TEA685x_COMMAND_SETSOFTMUTEMOD          0x29U
#define TEA685x_COMMAND_SETSOFTMUTELEVEL        0x2AU
#define TEA685x_COMMAND_SETSOFTMUTENOISE        0x2BU
#define TEA685x_COMMAND_SETSOFTMUTEMPH          0x2CU
#define TEA685x_COMMAND_SETSOFTMUTEMAX          0x2DU

#define TEA685x_COMMAND_SETHIGHCUTTIME          0x32U
#define TEA685x_COMMAND_SETHIGHCUTMOD           0x33U
#define TEA685x_COMMAND_SETHIGHCUTLEVEL         0x34U
#define TEA685x_COMMAND_SETHIGHCUTNOISE         0x35U
#define TEA685x_COMMAND_SETHIGHCUTMPH           0x36U
#define TEA685x_COMMAND_SETHIGHCUTMAX           0x37U
#define TEA685x_COMMAND_SETHIGHCUTMIN           0x38U
#define TEA685x_COMMAND_SETLOWCUTMAX            0x39U
#define TEA685x_COMMAND_SETLOWCUTMIN            0x3AU

#define TEA685x_COMMAND_SETSTEREOTIME           0x3CU
#define TEA685x_COMMAND_SETSTEREOMOD            0x3DU
#define TEA685x_COMMAND_SETSTEREOLEVEL          0x3EU
#define TEA685x_COMMAND_SETSTEREONOISE          0x3FU
#define TEA685x_COMMAND_SETSTEREOMPH            0x40U
#define TEA685x_COMMAND_SETSTEREOMAX            0x42U

#define TEA685x_COMMAND_SETSTHIBLENDTIME        0x46U
#define TEA685x_COMMAND_SETSTHIBLENDMOD         0x47U
#define TEA685x_COMMAND_SETSTHIBLENDLEVEL       0x48U
#define TEA685x_COMMAND_SETSTHIBLENDNOISE       0x49U
#define TEA685x_COMMAND_SETSTHIBLENDMPH         0x4AU
#define TEA685x_COMMAND_SETSTHIBLENDMAX         0x4BU
#define TEA685x_COMMAND_SETSTHIBLENDMIN         0x4CU

#define TEA685x_COMMAND_SETSCALER               0x50U
#define TEA685x_COMMAND_SETRDS                  0x51U
#define TEA685x_COMMAND_SETQUALITYSTATUS        0x52U
#define TEA685x_COMMAND_SETDRBLEND              0x53U
#define TEA685x_COMMAND_SETDROPTIONS            0x54U
#define TEA685x_COMMAND_SETSPECIALS             0x55U

#define TEA685x_COMMAND_SETVOLUME               0x0AU
#define TEA685x_COMMAND_SETMUTE                 0x0BU
#define TEA685x_COMMAND_SETINPUT                0x0CU

#define TEA685x_COMMAND_SETANAOUT               0x15U
#define TEA685x_COMMAND_SETDIGIO                0x16U
#define TEA685x_COMMAND_SETINPUTSCALER          0x17U
#define TEA685x_COMMAND_SETWAVEGEN              0x18U

#define TEA685x_COMMAND_SETOPERATIONMODE        0x01U
#define TEA685x_COMMAND_SETKEYCODE              0x02U
#define TEA685x_COMMAND_SETGPIO                 0x03U
#define TEA685x_COMMAND_SETREFERENCECLOCK       0x04U
#define TEA685x_COMMAND_ACTIVATE                0x05U

/**********************/
/* Read mode commands */
/**********************/

#define TEA685x_API_READ_LENWR                  3
#define TEA685x_API_READ_QSTATUS_LEN            2
#define TEA685x_API_READ_QDATA_LEN              14
#define TEA685x_API_READ_AGC_LEN                4
#define TEA685x_API_READ_RDSSTATUS_LEN          2
#define TEA685x_API_READ_RDSDATA_LEN            10
#define TEA685x_API_READ_BLOCKERR_LEN           2

#define TEA685x_COMMAND_GETQUALITYSTATUS        0x80U
#define TEA685x_COMMAND_GETQUALITYDATA          0x81U
#define TEA685x_COMMAND_GETRDSSTATUS            0x82U
#define TEA685x_COMMAND_GETRDSDATA              0x83U
#define TEA685x_COMMAND_GETAGC                  0x84U
#define TEA685x_COMMAND_GETSIGNALSTATUS         0x85U
#define TEA685x_COMMAND_GETPROCESSINGSTATUS     0x86U
#define TEA685x_COMMAND_GETINTERFACESTATUS      0x87U

#define TEA685x_COMMAND_GETOPERATIONSTATUS      0x80U
#define TEA685x_COMMAND_GETGPIOSTATUS           0x81U
#define TEA685x_COMMAND_GETIDENTIFICATION       0x82U
#define TEA685x_COMMAND_GETLASTWRITE            0x83U

/****************/
/* Byte offsets */
/****************/

#define TEA685x_INDEX_TUNETO_MODE               1
#define TEA685x_INDEX_TUNETO_FREQUENCY          2

/**************/
/* Byte masks */
/**************/

/* Get_Quality */
#define TEA685x_GETQUALITY_STATUS_TIMESTAMP_MSK 0x01FF
#define TEA685x_GETQUALITY_STATUS_TIMESTAMP_POS 0

#define TEA685x_GETQUALITY_STATUS_AFUPDATE_MSK  0x8000
#define TEA685x_GETQUALITY_STATUS_AFUPDATE_POS  15



#define TEA685x_FM_FREQUENCY_MIN				6500		//step 10kHz
#define TEA685x_FM_FREQUENCY_MAX				10800
#define TEA685x_FM_FREQUENCY_STEP				10

#define TEA685x_LW_FREQUENCY_MIN				144			//step 1kHz
#define TEA685x_LW_FREQUENCY_MAX				288
#define TEA685x_LW_FREQUENCY_STEP				1

#define TEA685x_MW_FREQUENCY_MIN				522			//step 1kHz
#define TEA685x_MW_FREQUENCY_MAX				1710
#define TEA685x_MW_FREQUENCY_STEP				1

#define TEA685x_SW_FREQUENCY_MIN				2300		//step 1kHz
#define TEA685x_SW_FREQUENCY_MAX				27000
#define TEA685x_SW_FREQUENCY_STEP				1

#define TEA685x_Is_FM_Freq(a)					(((a) >= TEA685x_FM_FREQUENCY_MIN)&&((a) <= TEA685x_FM_FREQUENCY_MAX))
#define TEA685x_Is_LW_Freq(a)					(((a) >= TEA685x_LW_FREQUENCY_MIN)&&((a) <= TEA685x_LW_FREQUENCY_MAX))
#define TEA685x_Is_MW_Freq(a)					(((a) >= TEA685x_MW_FREQUENCY_MIN)&&((a) <= TEA685x_MW_FREQUENCY_MAX))
#define TEA685x_Is_SW_Freq(a)					(((a) >= TEA685x_SW_FREQUENCY_MIN)&&((a) <= TEA685x_SW_FREQUENCY_MAX))
#define TEA685x_Is_AM_Freq(a)					((TEA685x_Is_LW_Freq(a))||(TEA685x_Is_MW_Freq(a))||(TEA685x_Is_SW_Freq(a)))


/* tuner mode */
#define Radio_POWERDOWNMODE 					0x00
#define Radio_PRESETMODE 						0x10
#define Radio_SEARCHMODE 						0x20
#define Radio_AFUPDATEMODE 						0x30
#define Radio_JUMPMODE 							0x40
#define Radio_CHECKMODE 						0x50
#define Radio_ENDMODE 							0x70

#define TUNER_LEVEL_3dB							3
#define TUNER_LEVEL_5dB							5
#define TUNER_LEVEL_8dB							8
#define TUNER_LEVEL_10dB						10
#define TUNER_LEVEL_14dB						14
#define TUNER_LEVEL_15dB						15
#define TUNER_LEVEL_20dB						20
#define TUNER_LEVEL_25dB						25
#define TUNER_LEVEL_30dB						30
#define TUNER_LEVEL_35dB						35
#define TUNER_LEVEL_38dB						38
#define TUNER_LEVEL_40dB						40
#define TUNER_LEVEL_45dB						45
#define TUNER_LEVEL_50dB						50
#define TUNER_LEVEL_55dB						55
#define TUNER_LEVEL_60dB						60

#define TUNER_PERCENT_27						27
#define TUNER_PERCENT_23						23
#define TUNER_PERCENT_20						20

#define TUNER_OFFSET_10KHz						100
#define TUNER_OFFSET_1500Hz						15
#define TUNER_OFFSET_300KHz						300


#define RADIO_FM_LEVEL_AVAILABLE_TIME			4	//5 ms after tuning
#define RADIO_AM_LEVEL_AVAILABLE_TIME			36	//36 ms after tuning
#define RADIO_USN_AVAILABLE_TIME				34	//34ms


typedef enum{
	TEA685x_TUNER_BAND_NULL = 0,
	TEA685x_TUNER_BAND_REC_FM,
	TEA685x_TUNER_BAND_REC_LW,
	TEA685x_TUNER_BAND_REC_MW,
	TEA685x_TUNER_BAND_REC_SW,
}tea685x_tuner_band_e;


typedef enum
{
	TEA685X_MODULE_FM 		= 32,
	TEA685X_MODULE_AM 		= 33,
	TEA685X_MODULE_AUDIO   	= 48,
	TEA685X_MODULE_APPL  	= 64
}TEA685X_MODULE_e;

typedef enum
{
	TEA685x_TUNINGACTION_STANDBY	  = 0, 
	TEA685x_TUNINGACTION_PRESET 	  = 1, /*!< Tune to new program with short mute time */
	TEA685x_TUNINGACTION_SEARCH 	  = 2, /*!< Tune to new program and stay muted */
	TEA685x_TUNINGACTION_AF_UPDATE	  = 3, /*!< Tune to alternative frequency, store quality and tune back with inaudible mute */
	TEA685x_TUNINGACTION_JUMP		  = 4, /*!< Tune to alternative frequency with short inaudible mute  */
	TEA685x_TUNINGACTION_CHECK		  = 5, /*!< Tune to alternative frequency and stay muted */
	TEA685x_TUNINGACTION_END		  = 7  /*!< Release the mute of a Search/Check action (frequency is ignored) */
}TEA685x_TUNING_ACTION_e;

typedef enum{
	TEA685x_TUNER_POWER_ON,
	TEA685x_TUNER_BOOT_STATE,
	TEA685x_TUNER_IDLE_STATE,
	TEA685x_TUNER_WAIT_ACTIVE,
	TEA685x_TUNER_ACTIVE_STATE,
	TEA685x_TUNER_NOT_EXIST,	
	TEA685x_TUNER_LAST,
}TEA685x_TUNER_STATE_e;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_RADIO_RDS_)
typedef struct{
	uint8	enable;
	uint8	factor;				//	信号品质衰减因子
	uint8  	quality;			//	信号强度("0xFF"最强，"0"表示没有信号)
	uint8	discard;			//	解码丢弃数据个数
}tea685x_rds_t;
#endif

enum{
	TEA685x_STATE_IDLE = 0,
	TEA685x_STATE_START,
	TEA685x_STATE_LEVEL,
	TEA685x_STATE_DETECT,
};

typedef struct{
	uint8 init_succeed;
	radio_band_e band;
	tea685x_tuner_band_e TunerBand;
	uint16 now_freq;
	uint16 min_freq;
	uint16 max_freq;
	uint8 tuner_state;
	uint32 last_sys_timer;
	uint8 signal;
    uint8 station;
	
#if defined(_ENABLE_RADIO_RDS_)
	tea685x_rds_t tuner_rds;
#endif	
}tea685x_ctrl_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#define FM_USN_DISTURBANCE							TUNER_PERCENT_23					//TUNER_PERCENT_27		//disturbance = 27%
#define FM_WAM_DISTURBANCE							TUNER_PERCENT_23					//disturbance = 23%

#define FM_FREQ_OFFSET								TUNER_OFFSET_10KHz					//offset = 10k, step=100Hz
#define AM_FREQ_OFFSET								TUNER_OFFSET_1500Hz					//offset = 1.5k, step=100Hz

#define STEREO_Separation_Level_H  					TUNER_LEVEL_20dB					//Stereo indication by LEVEL
#define STEREO_Separation_Level_L  					TUNER_LEVEL_10dB					//Stereo indication by LEVEL

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if !defined(TEA685x_TUNER_RSSI_FM_VALUE)
	#define TEA685x_TUNER_RSSI_FM_VALUE				(20)
#endif

#define TEA685x_TUNER_RSSI_FM						(TEA685x_TUNER_RSSI_FM_VALUE)


#if !defined(TEA685x_TUNER_RSSI_LOC_FM_VALUE)
	#define TEA685x_TUNER_RSSI_LOC_FM_VALUE			(TEA685x_TUNER_RSSI_FM + 20)
#endif

#define TEA685x_TUNER_RSSI_LOC_FM					(TEA685x_TUNER_RSSI_LOC_FM_VALUE)


#if !defined(TEA685x_TUNER_RSSI_AM_VALUE)
	#define TEA685x_TUNER_RSSI_AM_VALUE				(30)
#endif

#define TEA685x_TUNER_RSSI_AM						(TEA685x_TUNER_RSSI_AM_VALUE)

#define TEA685x_TUNER_RSSI_LOC_AM					(TEA685x_TUNER_RSSI_AM + 20)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#endif /*_TUNER_TEA685x_H_ */

