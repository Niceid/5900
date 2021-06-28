#ifndef __SI473x_H__
#define __SI473x_H__


#if defined(_TUNER_DRIVER_SI473x_)


#define _SI473x_VERSION_3020_
#define _SI473x_VERSION_3040_
#define _SI473x_VERSION_3060_


#if defined(_ENABLE_473x_LOW_ADDR_)
#define SI473x_SENB						0
#else
#define SI473x_SENB						1
#endif

#if SI473x_SENB
	#define SI473x_I2C_ADDR				0xC6
#else
	#define SI473x_I2C_ADDR				0x22
#endif

#define SI473x_I2C_RATE					(30)


typedef enum{
//	公共命令
	SI473x_CMD_POWER_UP_REC_FM = 0,// 0
	SI473x_CMD_POWER_UP_REC_AM,
	SI473x_CMD_POWER_UP_REC_WB,
	SI473x_CMD_POWER_UP_TRN_FM,
	SI473x_CMD_GET_REV,
	SI473x_CMD_POWER_DOWN,// 5
	SI473x_CMD_GET_INT_STATUS,//0x14命令

	SI473x_CMD_AUDIO_VOLUME,
	SI473x_CMD_AUDIO_MUTE,
	SI473x_CMD_AUDIO_UNMUTE,

	SI473x_CMD_GPIO_DIR,//10
	SI473x_CMD_GPIO_DATA,
/*-------------------------------------------------------------------------------------------------------------------------*/
//	调频命令
 	SI473x_CMD_SET_FM_FREQ,
	SI473x_CMD_FM_TUNE_STATUS,//{2,0x22,0x00}	
	SI473x_CMD_FM_RSQ_STATUS,//{2,0x23,0x01}
	SI473x_CMD_FM_AGC_STATUS,//15
	SI473x_CMD_FM_AGC_OVERRIDE,	

	SI473x_CMD_FM_SEEK_TUNE_SNR_SET,
	SI473x_CMD_FM_SEEK_TUNE_RSSI_SET,

	SI473x_CMD_SET_FM_DEEMPHASIS_50US,
	SI473x_CMD_SET_FM_DEEMPHASIS_75US,//20
	SI473x_CMD_FM_BLEND_STEREO_ON,
	SI473x_CMD_FM_BLEND_MONO_OFF,
	SI473x_CMD_FM_BLEND_STEREO_OFF,
	SI473x_CMD_FM_BLEND_MONO_ON,
	SI473x_CMD_FM_TUNE_ERROR,//25
	
	SI473x_CMD_FM_SOFT_MUTE_RATE,
	SI473x_CMD_FM_SOFT_MUTE_ATTENUATION,
	SI473x_CMD_FM_SOFT_MUTE_SNR,
#if defined(_SI473x_VERSION_3020_)
	SI473x_CMD_FM_SOFT_MUTE_ATTENUATION_B20,
	SI473x_CMD_FM_SOFT_MUTE_SNR_B20,//30
#endif	
	
	SI473x_CMD_GET_FM_BLEND_STEREO,
	SI473x_CMD_GET_FM_BLEND_MONO,

#if defined(_ENABLE_RADIO_RDS_)
/*-------------------------------------------------------------------------------------------------------------------------*/
//  RDS命令
	SI473x_CMD_FM_RDS_DETECT,//{2,0x24,0x03}
	SI473x_CMD_FM_RDS_STATUS,//{2,0x24,0x01}
	SI473x_CMD_FM_RDS_INT_SOURCE,//{6,0x12,0x00,0x15,0x00,0x00,0x01}
	SI473x_CMD_FM_RDS_FIFO_COUNT,//{6,0x12,0x00,0x15,0x01,0x00,1}
	SI473x_CMD_FM_RDS_CONFIG,//{6,0x12,0x00,0x15,0x02,0xFF,0x01}
	SI473x_CMD_FM_RDS_RESET,//{2,0x24,0x03}
#endif

#if defined(_SI473x_VERSION_3060_)
	SI473x_CMD_FM_BLEDN_RSSI_STERO_ON_THRESHOLD,
	SI473x_CMD_FM_BLEDN_RSSI_MONO_OFF_THRESHOLD,
	SI473x_CMD_FM_BLEDN_RSSI_STERO_OFF_THRESHOLD,//35
	SI473x_CMD_FM_BLEDN_RSSI_MONO_ON_THRESHOLD,
	SI473x_CMD_FM_BLEND_RSSI_ATTACK_RATE,
	SI473x_CMD_FM_BLEND_RSSI_RELEASE_RATE,
	SI473x_CMD_FM_BLEND_SNR_STERO_ON_THRESHOLD,
	SI473x_CMD_FM_BLEND_SNR_MONO_OFF_THRESHOLD,//40
	SI473x_CMD_FM_BLEND_SNR_STERO_OFF_THRESHOLD,
	SI473x_CMD_FM_BLEND_SNR_MONO_ON_THRESHOLD,
	SI473x_CMD_FM_BLEND_SNR_ATTACK_RATE,
	SI473x_CMD_FM_BLEND_SNR_RELEASE_RATE,
	SI473x_CMD_FM_BLEND_MULTIPATH_STERO_ON_THRESHOLD,//45
	SI473x_CMD_FM_BLEND_MULTIPATH_MONO_OFF_THRESHOLD,
	SI473x_CMD_FM_BLEND_MULTIPATH_STERO_OFF_THRESHOLD,
	SI473x_CMD_FM_BLEND_MULTIPATH_MONO_ON_THRESHOLD,	
	SI473x_CMD_FM_BLEND_MULTIPATH_ATTACK_RATE,
	SI473x_CMD_FM_BLEND_MULTIPATH_RELEASE_RATE,//50

	SI473x_CMD_FM_BLEND_MULTIPATH_STERO_THRESHOLD_DEFAULT,
	SI473x_CMD_FM_BLEND_MULTIPATH_MONO_THRESHOLD_DEFAULT,

	SI473x_CMD_FM_HICUT_SNR_HIGH_THRESHOLD_DEFAULT,
	SI473x_CMD_FM_HICUT_SNR_LOW_THRESHOLD_DEFAULT,
	SI473x_CMD_FM_HICUT_MULTIPATH_TRIGGER_THRESHOLD_DEFAULT,//55
	SI473x_CMD_FM_HICUT_MULTIPATH_END_THRESHOLD_DEFAULT,
	SI473x_CMD_FM_HICUT_OFF_FREQUENCY_DEFAULT,
#endif

	SI473x_CMD_FM_SEEK_START,
	SI473x_CMD_FM_SEEK_STOP,
	SI473x_CMD_FM_SEEK_BAND_BOTTOM,//60
	SI473x_CMD_FM_SEEK_BAND_TOP,
	SI473x_CMD_FM_SEEK_BAND_STEP,
	
/*-------------------------------------------------------------------------------------------------------------------------*/
//	调幅命令
	SI473x_CMD_SET_AM_FREQ,
	SI473x_CMD_AM_TUNE_STATUS,
	SI473x_CMD_AM_RSQ_STATUS,//65
	
	SI473x_CMD_SET_AM_DEEMPHASIS_OFF,
	SI473x_CMD_SET_AM_DEEMPHASIS_50US,

	SI473x_CMD_AM_SOFT_MUTE_RATE,
	SI473x_CMD_AM_SOFT_MUTE_SLOPE,
	SI473x_CMD_AM_SOFT_MUTE_ATTENUATION,//70
	SI473x_CMD_AM_SOFT_MUTE_SNR,

	SI473x_CMD_AM_SEEK_TUNE_SNR_SET,
	SI473x_CMD_AM_SEEK_TUNE_RSSI_SET,

#if defined(_SI473x_VERSION_3040_)|| defined(_SI473x_VERSION_3060_)
	SI473x_CMD_AM_AUTO_VOL_CONTROL_MAX_GAIN,
#endif
#if defined(FEIYIN_473x_C40_ADJUST)&&defined(_SI473x_VERSION_3040_)
	SI473x_CMD_AM_AUTO_VOL_CONTROL_MAX_GAIN2,
	SI473x_CMD_AM_FRONTEND_AGC_CONTROL,
#endif

	SI473x_CMD_AM_SEEK_START,//75
	SI473x_CMD_AM_SEEK_STOP,
	SI473x_CMD_AM_SEEK_BAND_BOTTOM,
	SI473x_CMD_AM_SEEK_BAND_TOP,
	SI473x_CMD_AM_SEEK_BAND_STEP,
	
	SI473x_CMD_GET_FREQ,
	
#if defined(_ENABLE_RADIO_WB_)
	//	WB命令
	SI473x_CMD_SET_WB_FREQ,
    SI473x_CMD_WB_TUNE_STATUS,
    SI473x_CMD_WB_ASQ_STATUS,
#endif
/*-------------------------------------------------------------------------------------------------------------------------*/
	SI473x_CMD_ORDER_MAX,//80
}si473x_order_e;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

typedef struct{
	uint8 buffer[30];
	uint8 length;
}__attribute__ ((packed)) si473x_order_t;

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;
	}Status;
	uint8 	Pn;
	uint8 	MajorRev;
	uint8 	MinorRev;
	uint8 	Patch_h;
	uint8 	Patch_l;
	uint8 	MajorCom;
	uint8 	MinorCom;
	uint8 	Chip; 	
}__attribute__ ((packed)) TUNER_REVISION_FORMAT;

#define TunerRevision									(*((TUNER_REVISION_FORMAT *)(Si473xOrder->buffer)))	

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;
	}Status;
	struct{
		uint8 Valid:1;
		uint8 Afc:1;
		uint8 :5;
		uint8 Limit:1;
	}Resp1;
	uint8 	Freq_h;
	uint8 	Freq_l;
	uint8 	Rssi;
	uint8 	Snr;
	uint8 	Res;
	uint8 	AntCap;
}__attribute__ ((packed)) FM_TUNER_STATUS_FORMAT;

#define FmTunerStatus									(*((FM_TUNER_STATUS_FORMAT *)(Si473xOrder->buffer)))				 

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :2;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;
	}Status;
	struct{
		uint8 Valid:1;
		uint8 Afc:1;
		uint8 :5;
		uint8 Limit:1;
	}Resp1;
	uint8 	Freq_h;
	uint8 	Freq_l;
	uint8 	Rssi;
	uint8 	Snr;
	uint8 	AntCap_h;
	uint8 	AntCap_l;
}__attribute__ ((packed)) AM_TUNER_STATUS_FORMAT;

#define AmTunerStatus									(*((AM_TUNER_STATUS_FORMAT *)(Si473xOrder->buffer)))

/*------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_RADIO_WB_)
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :2;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;
	}Status;
	struct{
		uint8 Valid:1;
		uint8 Afc:1;
		uint8 :5;
		uint8 Limit:1;
	}Resp1;
	//uint8 	Freq_h;
	//uint8 	Freq_l;
	uint16 	Freq;
	uint8 	Rssi;
	uint8 	Snr;
}__attribute__ ((packed)) WB_TUNER_STATUS_FORMAT;


#define WbTunerStatus									(*((WB_TUNER_STATUS_FORMAT *)(Si473xOrder->buffer)))
#endif
/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;
	}Status;
	struct{
	 	uint8 RssiLint:1;
		uint8 RssiHint:1;
		uint8 SnrLint:1;
		uint8 SnrHint:1;
		uint8 :3;
		uint8 Blendint:1;	
	}Resp1;
	struct{
		uint8 Valid:1;
		uint8 Afc:1;
		uint8 :1;
		uint8 Smute:1;
		uint8 :4;
	}Resp2;
	struct{
		uint8 Stblend:7;
		uint8 Pilot:1;
	}Resp3;
	uint8 Rssi;
	uint8 Snr;
	uint8 Mult;
	uint8 FreqOffset;
}__attribute__ ((packed)) FM_RSQ_STATUS_FORMAT;

#define FmRsqStatus									(*((FM_RSQ_STATUS_FORMAT *)(Si473xOrder->buffer)))	

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :2;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;
	}Status;
	struct{
	 	uint8 RssiLint:1;
		uint8 RssiHint:1;
		uint8 SnrLint:1;
		uint8 SnrHint:1;
		uint8 :4;
	}Resp1;
	struct{
		uint8 Valid:1;
		uint8 Afc:1;
		uint8 :1;
		uint8 Smute:1;
		uint8 :4;
	}Resp2;
	uint8 Res;
	uint8 Rssi;
	uint8 Snr;
}__attribute__ ((packed)) AM_RSQ_STATUS_FORMAT;

#define AmRsqStatus									(*((AM_RSQ_STATUS_FORMAT *)(Si473xOrder->buffer)))	

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 Res1:1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 Res2:2;
		uint8 Err:1;
		uint8 Cts:1;		
	}Status;
	struct{
		uint8 Agc:1;
		uint8 Res:7;
	}Resp1;	
	struct{
		uint8 Gain:5;
		uint8 Res:3;
	}Resp2;	
}__attribute__ ((packed)) FM_AGC_STATUS_FORMAT;

#define FmAgcStatus									(*((FM_AGC_STATUS_FORMAT *)(Si473xOrder->buffer)))	

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;		
	}Status;
	uint8 Resp1;
	uint8 Resp2;
	struct{
		uint8 Stereo:7;
		uint8 :1;
	}Value;
}__attribute__ ((packed)) FM_BLEND_STEREO_FORMAT;

#define FmBlendStereo								(*((FM_BLEND_STEREO_FORMAT *)(Si473xOrder->buffer)))

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;		
	}Status;
	uint8 Resp1;
	uint8 Resp2;
	struct{
		uint8 Mono:7;
		uint8 :1;
	}Value;
}__attribute__ ((packed)) FM_BLEND_MONO_FORMAT;

#define FmBlendMono									(*((FM_BLEND_MONO_FORMAT *)(Si473xOrder->buffer)))

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 :1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;		
	}Status;

	struct{
		uint8 RdsRecv:1;
		uint8 RdsSyncLost:1;
		uint8 RdsSyncFound:1;
		uint8 Rev:5;
	}Resp1;

	struct{
		uint8 RdsSync:1;
		uint8 :1;
		uint8 GrpLost:1;
		uint8 Rev:5;
	}Resp2;

	uint8 RdsFifoUsed;

	uint8 BlockA_h;
	uint8 BlockA_l;
	uint8 BlockB_h;
	uint8 BlockB_l;
	uint8 BlockC_h;
	uint8 BlockC_l;
	uint8 BlockD_h;
	uint8 BlockD_l;
	
	struct{
		uint8 BleD:2;
		uint8 BleC:2;
		uint8 BleB:2;
		uint8 BleA:2;
	}Resp12;
}__attribute__ ((packed)) FM_RDS_STATUS_FORMAT;

#define FmRdsStatus									(*((FM_RDS_STATUS_FORMAT *)(Si473xOrder->buffer)))

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

enum{
	SI473x_TUNER_BAND_REC_FM = 0,
	SI473x_TUNER_BAND_REC_AM,
	SI473x_TUNER_BAND_REC_WB,
	SI473x_TUNER_BAND_TRN_FM,
};

enum{
	SI473x_TUNER_VERSION_B20 = 0,
	SI473x_TUNER_VERSION_C40,
	SI473x_TUNER_VERSION_D60,
};

enum{
	TUNER_SEEK_UP_WRAP = 0,		//	向上扫描到电台停
	TUNER_SEEK_UP_HALT,			//	向上扫描到电台或波段边界停
	TUNER_SEEK_DOWN_WRAP,		//	向下扫描到电台停
	TUNER_SEEK_DOWN_HALT,		//	向下扫描到电台或波段边界停
};


#if defined(_ENABLE_RADIO_RDS_)
typedef struct{
	uint8	enable;
	uint8	factor;				//	信号品质衰减因子
	uint8  	quality;			//	信号强度("0xFF"最强，"0"表示没有信号)
	uint8	discard;			//	解码丢弃数据个数
}si473x_rds_t;
#endif

typedef struct{
	uint8 	TunerBand;
	uint8 	TunerVersion;
	uint8	TunerAutoSeek;
	uint8 	tuner_band;
	uint16 	now_freq;
	uint16 	min_freq;
	uint16 	max_freq;
	uint8  	init_succeed;
#if defined(_ENABLE_RADIO_RDS_)
	si473x_rds_t tuner_rds;
#endif
}si473x_ctrl_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if defined(SI473x_TUNER_RSSI_FM_VALUE)
	#define SI473x_TUNER_RSSI_FM_DEFAULT				(SI473x_TUNER_RSSI_FM_VALUE)						//	dBu
#else
	#define SI473x_TUNER_RSSI_FM_DEFAULT				(14)												//	dBu
#endif
#if defined(SI473x_TUNER_SNR_FM_VALUE)
	#define SI473x_TUNER_SNR_FM_DEFAULT					(SI473x_TUNER_SNR_FM_VALUE)							//	dBu
#else
	#define SI473x_TUNER_SNR_FM_DEFAULT					(2)													//	dBu
#endif
#if defined(SI473x_TUNER_SNR_OIRT_VALUE)
	#define SI473x_TUNER_SNR_OIRT_DEFAULT				(SI473x_TUNER_SNR_OIRT_VALUE)						//	dBu
#else
	#define SI473x_TUNER_SNR_OIRT_DEFAULT				(10)												//	dBu
#endif
#if !defined(SI473x_TUNER_RSSI_RDS_FM_OFFSET)
	#define SI473x_TUNER_RSSI_RDS_FM_OFFSET				(5)													//	dBu
#endif

#define SI473x_TUNER_RSSI_FM							SI473x_TUNER_RSSI_FM_DEFAULT						//	21dBu
#if !defined(SI473x_TUNER_RSSI_LOC_FM)
#define SI473x_TUNER_RSSI_LOC_FM						(SI473x_TUNER_RSSI_FM + 20)							//	dBu
#endif
#define SI473x_TUNER_RSSI_RDS_FM						(SI473x_TUNER_RSSI_FM + SI473x_TUNER_RSSI_RDS_FM_OFFSET)//	dBu
#define SI473x_TUNER_RSSI_OIRT							(SI473x_TUNER_RSSI_FM)								//	dBu
#if !defined(SI473x_TUNER_RSSI_LOC_OIRT)
#define SI473x_TUNER_RSSI_LOC_OIRT						(SI473x_TUNER_RSSI_OIRT + 20)						//	dBu
#endif

#define SI473x_TUNER_SNR_FM								SI473x_TUNER_SNR_FM_DEFAULT							//	2 dBu
#define SI473x_TUNER_SNR_OIRT							SI473x_TUNER_SNR_OIRT_DEFAULT						//10	dBu


#if !defined(SI473x_FM_SOFT_MUTE_ATTENUATION)
#define SI473x_FM_SOFT_MUTE_ATTENUATION					(25)
#endif
#if !defined(SI473x_FM_SOFT_MUTE_SNR)
#define SI473x_FM_SOFT_MUTE_SNR							(3)	
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/

#if defined(_SI473x_VERSION_3060_)

#if !defined(SI473x_TUNER_RSSI_MONO_VALUE)
#define SI473x_TUNER_RSSI_MONO_VALUE               		(31)   							//31
#endif
#if !defined(SI473x_TUNER_RSSI_STEREO_VALUE)
#define SI473x_TUNER_RSSI_STEREO_VALUE             		(49)      						//49 
#endif
#if !defined(SI473x_TUNER_SNR_MONO_VALUE)
#define SI473x_TUNER_SNR_MONO_VALUE             		(3)   							//14
#endif
#if !defined(SI473x_TUNER_SNR_STEREO_VALUE)
#define SI473x_TUNER_SNR_STEREO_VALUE       			(7)      						//27 
#endif
#if !defined(SI473x_TUNER_MULTIPATH_MONO_VALUE)
#define SI473x_TUNER_MULTIPATH_MONO_VALUE    			(40)   							//60
#endif
#if !defined(SI473x_TUNER_MULTIPATH_STEREO_VALUE)
#define SI473x_TUNER_MULTIPATH_STEREO_VALUE				(20)      						//20
#endif

#if !defined(SI473x_TUNER_HICUT_HIGH_VALUE)
#define SI473x_TUNER_HICUT_HIGH_VALUE                	(30)   							//24
#endif
#if !defined(SI473x_TUNER_HICUT_LOW_VALUE)
#define SI473x_TUNER_HICUT_LOW_VALUE      				(20)   							//15
#endif

#if !defined(SI473x_TUNER_HICUT_MULTIPATH_TRIGGER_VALUE)
#define SI473x_TUNER_HICUT_MULTIPATH_TRIGGER_VALUE  	(20)   							//20
#endif
#if !defined(SI473x_TUNER_HICUT_MULTIPATH_END_VALUE)
#define SI473x_TUNER_HICUT_MULTIPATH_END_VALUE 			(40)   							//60
#endif

#endif

#if !defined(SI473x_TUNER_MONO_VALUE)
	#define SI473x_TUNER_MONO_VALUE						(10)							//	dBu
#endif

#if !defined(SI473x_TUNER_STEREO_VALUE)
	#define SI473x_TUNER_STEREO_VALUE					(15)							//	dBu
#endif

#if !defined(SI473x_TUNER_FM_TUNE_ERROR)											
	#define SI473x_TUNER_FM_TUNE_ERROR					(20)							//	kHz
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/

#if defined(SI473x_TUNER_RSSI_AM_VALUE)
	#define SI473x_TUNER_RSSI_AM_DEFAULT				(SI473x_TUNER_RSSI_AM_VALUE)	//	dBu
#else
	#define SI473x_TUNER_RSSI_AM_DEFAULT				(15)							//	dBu
#endif
#if defined(SI473x_TUNER_SNR_AM_VALUE)
	#define SI473x_TUNER_SNR_AM_DEFAULT					(SI473x_TUNER_SNR_AM_VALUE)		//	dBu
#else
	#define SI473x_TUNER_SNR_AM_DEFAULT					(10)							//	dBu
#endif


#define SI473x_TUNER_RSSI_AM							SI473x_TUNER_RSSI_AM_DEFAULT	//	dBu
#define SI473x_TUNER_RSSI_LOC_AM						(SI473x_TUNER_RSSI_AM + 35)		//	dBu

#define SI473x_TUNER_SNR_AM								SI473x_TUNER_SNR_AM_DEFAULT		//	dBu


#if !defined(SI473x_AM_SOFT_MUTE_ATTENUATION)
	#define SI473x_AM_SOFT_MUTE_ATTENUATION				(25)
#endif
#if !defined(SI473x_AM_SOFT_MUTE_SNR)
	#define SI473x_AM_SOFT_MUTE_SNR						(10)	
#endif

#if defined(_ENABLE_RADIO_WB_)

#define SI473x_RSSI_WB_OFFSET			(0) 							//	dBu
#define SI473x_RSSI_WB					(SI473x_RSSI_WB_OFFSET + 5)	   	//	dBu

#define SI473x_SNR_WB_OFFSET			(0)								//	dBu
#define SI473x_SNR_WB					(SI473x_SNR_WB_OFFSET + 1)		//	dBu

#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

extern const tuner_drv_fun_t tuner_si473x_drv_fun;

#endif

#endif /* __SI473x_H__ */

