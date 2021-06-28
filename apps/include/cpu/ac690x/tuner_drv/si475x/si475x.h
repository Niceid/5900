#ifndef __SI475x_H__
#define __SI475x_H__


#if defined(_TUNER_DRIVER_SI475x_)

#if defined(_ENABLE_475x_LOW_ADDR_)
#define SI475x_SENB						0
#else
#define SI475x_SENB						0
#endif

#if SI475x_SENB
	#define SI475x_I2C_ADDR				0xC4
#else
	#define SI475x_I2C_ADDR				0xC0
#endif

#define SI475x_I2C_RATE					(30)// 20

#if !defined(_TUNER_DRIVER_SI473x_)
enum{
	TUNER_SEEK_UP_WRAP = 0,		//	向上扫描到电台停
	TUNER_SEEK_UP_HALT,			//	向上扫描到电台或波段边界停
	TUNER_SEEK_DOWN_WRAP,		//	向下扫描到电台停
	TUNER_SEEK_DOWN_HALT,		//	向下扫描到电台或波段边界停
};
#endif

typedef enum{
	SI475x_CMD_POWER_UP_REC_FM_STEP1 = 0,
	SI475x_CMD_POWER_UP_REC_FM_STEP3,
	
	SI475x_CMD_POWER_UP_REC_AM_STEP1,
	SI475x_CMD_POWER_UP_REC_AM_STEP3,
	SI475x_CMD_POWER_UP_REC_AM_STEP4,
	
	SI475x_CMD_POWER_UP_STEP2,

	SI475x_CMD_POWER_UP_RX_BOOT_LOADER,
	SI475x_CMD_POWER_UP_LOADER_PATCH1,
	SI475x_CMD_POWER_UP_LOADER_PATCH2,
	SI475x_CMD_POWER_UP_LOADER_PATCH3,
	SI475x_CMD_POWER_UP_LOADER_PATCH4,
	SI475x_CMD_POWER_UP_LOADER_PATCH5,
	
	SI475x_CMD_GET_INT_STATUS,
	
	SI475x_CMD_POWER_DOWN,
	SI475x_CMD_ANA_AUDIO_PIN_CFG,
	SI475x_CMD_AUDIO_MUTE,
	SI475x_CMD_AUDIO_UNMUTE,

	SI475x_CMD_SET_FM_FREQ,
	SI475x_CMD_FM_RSQ_STATUS,
	SI475x_CMD_FM_AGC_OVERRIDE,
	SI475x_CMD_FM_ACF_STATUS,
	SI475x_CMD_FM_AGC_FE_CONFIG,
	SI475x_CMD_FM_AGC_STATUS,
	SI475x_CMD_FM_R8000,
	SI475x_CMD_FM_R8001,
	SI475x_CMD_FM_SEEK_TUNE_RSSI_RESET,
	SI475x_CMD_FM_SEEK_TUNE_RSSI_SET,
	SI475x_CMD_FM_SEEK_TUNE_SNR_RESET,
	SI475x_CMD_FM_SEEK_TUNE_SNR_SET,

	SI475x_CMD_SET_FM_DEEMPHASIS_50US,
	SI475x_CMD_SET_FM_DEEMPHASIS_75US,
	SI475x_CMD_FM_TUNE_ERROR,
	SI475x_CMD_FM_SOFT_MUTE_ATTENUATION,
	SI475x_CMD_FM_SOFT_MUTE_TRIGGER_THRESHOLD,
	SI475x_CMD_FM_SOFT_MUTE_END_THRESHOLD,

	SI475x_CMD_FM_SEEK_START,
	SI475x_CMD_FM_SEEK_STOP,
	SI475x_CMD_FM_SEEK_BAND_BOTTOM,
	SI475x_CMD_FM_SEEK_BAND_TOP,
	SI475x_CMD_FM_SEEK_BAND_STEP,
	
	SI475x_CMD_FM_BLEND_RSSI_STEREO,
	SI475x_CMD_FM_BLEND_RSSI_MONO,
	SI475x_CMD_FM_BLEND_RSSI_STEREO_SEP,
	SI475x_CMD_FM_BLEND_RSSI_ATTACK_RATE,
	SI475x_CMD_FM_BLEND_RSSI_RELEASE_RATE,
	
	SI475x_CMD_FM_BLEND_MULTIPATH_ATTACK_RATE,
	SI475x_CMD_FM_BLEND_MULTIPATH_RELEASE_RATE,
	SI475x_CMD_FM_BLEND_MULTIPATH_STEREO,
	SI475x_CMD_FM_BLEND_MULTIPATH_MONO,
	SI475x_CMD_FM_BLEND_MULTIPATH_STEREO_SEP,
	
	SI475x_CMD_FM_HICUT_RSSI_THRESHOLDS,
	SI475x_CMD_FM_HICUT_RSSI_CUTOFF_FREQ,
	SI475x_CMD_FM_HICUT_RSSI_ATTACK_TIME,
	SI475x_CMD_FM_HICUT_RSSI_RELEASE_TIME,
	
	SI475x_CMD_FM_HICUT_MULTIPATH_THRESHOLDS,
	SI475x_CMD_FM_HICUT_MULTIPATH_RSSI_CUTOFF_FREQ,
	SI475x_CMD_FM_HICUT_MULTIPATH_RSSI_ATTACK_TIME,
	SI475x_CMD_FM_HICUT_MULTIPATH_RSSI_RELEASE_TIME,

	SI475x_CMD_FM_HICUT_USN_THRESHOLDS,
	SI475x_CMD_FM_HICUT_USN_RSSI_CUTOFF_FREQ,
	SI475x_CMD_FM_HICUT_USN_RSSI_ATTACK_TIME,
	SI475x_CMD_FM_HICUT_USN_RSSI_RELEASE_TIME,
	
	SI475x_CMD_FM_HIBLEND_RSSI_THRESHOLDS,
	SI475x_CMD_FM_HIBLEND_RSSI_CUTOFF_FREQ,
	SI475x_CMD_FM_HIBLEND_RSSI_ATTACK_TIME_,
	SI475x_CMD_FM_HIBLEND_RSSI_RELEASE_TIME_,

	SI475x_CMD_FM_HIBLEND_MULTIPATH_THRESHOLDS,
	SI475x_CMD_FM_HIBLEND_MULTIPATH_CUTOFF_FREQ,
	SI475x_CMD_FM_HIBLEND_MULTIPATH_ATTACK_TIME,
	SI475x_CMD_FM_HIBLEND_MULTIPATH_RELEASE_TIME,

	SI475x_CMD_FM_HIBLEND_USN_THRESHOLDS,
	SI475x_CMD_FM_HIBLEND_USN_CUTOFF_FREQ,
	SI475x_CMD_FM_HIBLEND_USN_ATTACK_TIME,
	SI475x_CMD_FM_HIBLEND_USN_RELEASE_TIME,

#if defined(_ENABLE_RADIO_RDS_)	
	SI475x_CMD_FM_RDS_DETECT,
	SI475x_CMD_FM_RDS_STATUS,
	SI475x_CMD_FM_RDS_INT_SOURCE,
	SI475x_CMD_FM_RDS_FIFO_COUNT,
	SI475x_CMD_FM_RDS_CONFIG,
#endif

	SI475x_CMD_SET_AM_FREQ,
	SI475x_CMD_AM_RSQ_STATUS,
	SI475x_CMD_AM_SOFT_MUTE_RATE,
	SI475x_CMD_AM_SOFT_MUTE_SLOPE,
	SI475x_CMD_AM_SOFT_MUTE_ATTENUATION,
	SI475x_CMD_AM_SOFT_MUTE_TRIGGER_THRESHOLD,
	SI475x_CMD_AM_SOFT_MUTE_END_THRESHOLD,

	SI475x_CMD_AM_SEEK_TUNE_RSSI_RESET,
	SI475x_CMD_AM_SEEK_TUNE_RSSI_SET,
	SI475x_CMD_AM_SEEK_TUNE_SNR_RESET,
	SI475x_CMD_AM_SEEK_TUNE_SNR_SET,

	SI475x_CMD_AM_SEEK_START,
	SI475x_CMD_AM_SEEK_STOP,
	SI475x_CMD_AM_SEEK_BAND_BOTTOM,
	SI475x_CMD_AM_SEEK_BAND_TOP,
	SI475x_CMD_AM_SEEK_BAND_STEP,
	SI475x_CMD_GET_PART_INFO,
	SI475x_CMD_AM_DEMOD_MODE,
/*-------------------------------------------------------------------------------------------------------------------------*/
	SI475x_CMD_ORDER_MAX,
}si475x_order_e;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

typedef struct{
	uint8 buffer[30];
	uint8 length;
}__attribute__ ((packed)) si475x_order_t;

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 AcfInt:1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;
	}Status;
	uint8 	ChipRev;
	uint8 	PartNum;
	uint8 	MajorRev;
	uint8 	MinorRev;
	uint8 	BuildRev;
	uint8 	Resp1;
	uint8 	Resp2;
	uint8 	RomId; 
}__attribute__ ((packed)) SI475x_TUNER_REVISION_FORMAT;

#define Si475xTunerRevision									(*((SI475x_TUNER_REVISION_FORMAT *)(&Si475xOrder->buffer)))	

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 AcfInt:1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;
	}Status;
	uint8 Resp1;	
	struct{
		uint8 SMute:1;
		uint8 :7;
	}Resp2;
	struct{
		uint8 Smattn:5;
		uint8 :3;
	}Resp3;
	uint8 Chbw;
	uint8 Hicut;
	uint8 Hiblend;
	struct{
		uint8 Stblend:7;
		uint8 Pilot:1;
	}Resp7;
	uint8 Lowcut;
}__attribute__ ((packed)) SI475x_TUNER_ACF_STATUS_FORMAT;

#define Si475xFmAcfStatus									(*((SI475x_TUNER_ACF_STATUS_FORMAT *)(Si475xOrder->buffer)))	

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 AcfInt:1;
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
		uint8 Injside:1;
		uint8 :1;
		uint8 RssiReady:1;
		uint8 SnrReady:1;
		uint8 :1;
		uint8 Bltf:1;
	}Resp2;
	uint8 Freq_h;
	uint8 Freq_l;
	uint8 FreqOff;
	uint8 Rssi;
	uint8 Snr;
	uint8 Issi;
	uint8 LAssi;
	uint8 HAssi;
	uint8 Mult;
	uint8 Dev;
	uint8 Res1;
	uint8 Res2;
	uint8 Assi200;
	uint8 Usn;
	uint8 Pilotdev;
	uint8 Rdsdev;
	uint8 Assi200dev;
	uint8 Strongdev;
	uint8 Rdspi_h;
	uint8 Rdspi_l;
}__attribute__ ((packed)) SI475x_FM_RSQ_STATUS_FORMAT;

#define Si475xFmRsqStatus									(*((SI475x_FM_RSQ_STATUS_FORMAT *)(Si475xOrder->buffer)))	

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 Acfint:1;
		uint8 :1;
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
		uint8 Injside:1;
		uint8 :1;
		uint8 RssiReady:1;
		uint8 SnrReady:1;
		uint8 :1;	
		uint8 Bltf:1;
	}Resp2;
	uint8 Freq_h;
	uint8 Freq_l;
	uint8 FreqOff;
	uint8 Rssi;
	uint8 Snr;
	uint8 Res;
	uint8 Lassi;
	uint8 Hassi;
	uint8 Iqsnr;
	uint8 Mod;
}__attribute__ ((packed)) SI475x_AM_RSQ_STATUS_FORMAT;

#define Si475xAmRsqStatus									(*((SI475x_AM_RSQ_STATUS_FORMAT *)(Si475xOrder->buffer)))	

/*------------------------------------------------------------------------------------------------------------*/
typedef struct{
	struct{
		uint8 StcInt:1;
		uint8 AcfInt:1;
		uint8 RdsInt:1;
		uint8 RsqInt:1;
		uint8 :2;
		uint8 Err:1;
		uint8 Cts:1;		
	}Status;  
	struct{
		uint8 RdsFifoInt:1;
		uint8 RdsSyncInt:1;
		uint8 :1;
		uint8 RdsPiInt:1;
		uint8 RdsPtPtyInt:1;
		uint8 :3;
	}Resp1;
	struct{
		uint8 RdsFiFoLost:1;
		uint8 RdsSync:1;
		uint8 :1;
		uint8 PiValid:1;
		uint8 PtPtyValid:1;
		uint8 :3;
	}Resp2;
	struct{
		uint8 Pty:5;
		uint8 Tp:1;
		uint8 :2;
	}Resp3;
	uint8 Pi_h;
	uint8 Pi_l;
	uint8 RdsFifoUsed;
	struct{
		uint8 BleD:2;
		uint8 BleC:2;
		uint8 BleB:2;
		uint8 BleA:2;
	}Resp7;
	uint8 BlockA_h;
	uint8 BlockA_l;
	uint8 BlockB_h;
	uint8 BlockB_l;
	uint8 BlockC_h;
	uint8 BlockC_l;
	uint8 BlockD_h;
	uint8 BlockD_l;
	uint8 IqSnr;
}__attribute__ ((packed)) SI475x_FM_RDS_STATUS_FORMAT;

#define Si475xFmRdsStatus								(*((SI475x_FM_RDS_STATUS_FORMAT *)(Si475xOrder->buffer)))

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

enum{
	SI475x_TUNER_BAND_REC_FM = 0,
	SI475x_TUNER_BAND_REC_AM,
	SI475x_TUNER_BAND_REC_WB,
	SI475x_TUNER_BAND_TRN_FM,
};

enum{
	SI475x_TUNER_SEEK_UP_WRAP = 0,		//	向上扫描到电台停
	SI475x_TUNER_SEEK_UP_HALT,			//	向上扫描到电台或波段边界停
	SI475x_TUNER_SEEK_DOWN_WRAP,		//	向下扫描到电台停
	SI475x_TUNER_SEEK_DOWN_HALT,		//	向下扫描到电台或波段边界停
};


#if defined(_ENABLE_RADIO_RDS_)
typedef struct{
	uint8	enable;
	uint8	factor;				//	信号品质衰减因子
	uint8  	quality;			//	信号强度("0xFF"最强，"0"表示没有信号)
	uint8	discard;			//	解码丢弃数据个数
}si475x_rds_t;
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
	si475x_rds_t tuner_rds;
#endif
}si475x_ctrl_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if defined(SI475x_TUNER_RSSI_FM_VALUE)
	#define SI475x_TUNER_RSSI_FM_DEFAULT					(SI475x_TUNER_RSSI_FM_VALUE)						//	dBu
#else
	#define SI475x_TUNER_RSSI_FM_DEFAULT					(4)												//	dBu
#endif

#if !defined(SI475x_TUNER_RSSI_LOC_FM)
	#define SI475x_TUNER_RSSI_LOC_FM						(SI475x_TUNER_RSSI_FM_DEFAULT + 19)				//	dBu
#endif
#if !defined(SI475x_TUNER_RSSI_OIRT)
	#define SI475x_TUNER_RSSI_OIRT							(SI475x_TUNER_RSSI_FM_DEFAULT)
#endif
#if !defined(SI475x_TUNER_RSSI_LOC_OIRT)
	#define SI475x_TUNER_RSSI_LOC_OIRT						(SI475x_TUNER_RSSI_OIRT + 20)			//	dBu
#endif

#if !defined(SI475x_TUNER_SNR_FM)
	#define	SI475x_TUNER_SNR_FM								(8)
#endif
#define SI475x_TUNER_SNR_OIRT								(SI475x_TUNER_SNR_FM)					//	dBu

#if defined(SI475x_TUNER_SNR_LOC_FM_VALUE)
	#define SI475x_TUNER_SNR_LOC_FM							(SI475x_TUNER_SNR_LOC_FM_VALUE)			//	dBu
#else
	#define SI475x_TUNER_SNR_LOC_FM							(SI475x_TUNER_SNR_FM + 2)				//	dBu
#endif

/*-----------------------------------------------------------------------------------------*/
#if !defined(SI475x_TUNER_RSSI_MONO_VALUE)
	#define SI475x_TUNER_RSSI_MONO_VALUE                	(9)
#endif
#if !defined(SI475x_TUNER_RSSI_STEREO_VALUE)
	#define SI475x_TUNER_RSSI_STEREO_VALUE              	(49)
#endif

#if !defined(SI475x_TUNER_RSSI_STEREO_SEP_MAX)
	#define SI475x_TUNER_RSSI_STEREO_SEP_MAX              	(64)
#endif
#if !defined(SI475x_TUNER_RSSI_STEREO_SEP_MIN)
	#define SI475x_TUNER_RSSI_STEREO_SEP_MIN              	(0)
#endif

#if !defined(SI475x_TUNER_RSSI_ATTACK_TIME_H)//0x0010
	#define SI475x_TUNER_RSSI_ATTACK_TIME_H              	(0x00)//(0x0B)
#endif
#if !defined(SI475x_TUNER_RSSI_ATTACK_TIME_L)
	#define SI475x_TUNER_RSSI_ATTACK_TIME_L              	(0x10)//(0xB8)
#endif

/*-----------------------------------------------------------------------------------------*/
#if defined(SI475x_TUNER_RSSI_AM_VALUE)
	#define SI475x_TUNER_RSSI_AM_DEFAULT					(SI475x_TUNER_RSSI_AM_VALUE)	//	dBu
#else
	#define SI475x_TUNER_RSSI_AM_DEFAULT					(13)							//	dBu
#endif

#if !defined(SI475x_TUNER_RSSI_LOC_AM)
	#define SI475x_TUNER_RSSI_LOC_AM						(SI475x_TUNER_RSSI_AM_DEFAULT + 20)	//	dBu
#endif
#if !defined(SI475x_TUNER_SNR_AM)							
	#define SI475x_TUNER_SNR_AM								(9)							//	dBu
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

extern const tuner_drv_fun_t tuner_si475x_drv_fun;

#endif

#endif /* __SI473x_H__ */

