#ifndef __SI470x_H__
#define __SI470x_H__


#if defined(_TUNER_DRIVER_SI470x_)

#define SI470x_I2C_ADDR					(0x20)
#define SI470x_I2C_READ_ADDR			(SI470x_I2C_ADDR | _B0_)
#define SI470x_I2C_WRITE_ADDR			(SI470x_I2C_ADDR)

#define SI470x_I2C_RATE					(20)

//#define _SI470x_TUNER_C19_ST_MODIFY_


typedef enum
{
    SI470x_TUNER_BAND_USA = 0,				//china/USA band   	87---108MHZ
    SI470x_TUNER_BAND_WIDE,					//Japan wide band   	76---108MHZ
    SI470x_TUNER_BAND_JAPAN,				//Japan band    		76---90
//   SI470x_TUNER_BAND_WORD,				//wrod band  			64---108MHZ	
}si470x_tuner_band_e;

typedef enum
{
	SI470x_TUNE_AUTO_SEEK_200KHZ = 0,
	SI470x_TUNE_AUTO_SEEK_100KHZ,
	SI470x_TUNE_AUTO_SEEK_50KHZ,
}si470x_tuner_span_e;

#if defined(_SI470x_TUNER_C19_ST_MODIFY_)

typedef enum{
	SI470x_VERIFY_COMMAND,
	SI470x_SET_STERO_RSSI,
	SI470x_SET_MONO_RSSI,
	SI470x_GET_STERO_RSSI,
	SI470x_GET_MONO_RSSI,
	SI470x_ORDER_END,
}si470x_order_e;

#endif


typedef struct{
	struct{
		uint16 Enable:1;
		uint16 :5;
		uint16 Disable:1;
		uint16 :1;
		uint16 Seek:1;
		uint16 SeekUp:1;
		uint16 SkMode:1;
		uint16 RdsM:1;
		uint16 :1;
		uint16 Mono:1;
		uint16 DMute:1;
		uint16 DSMute:1;
	}PowerCfg;

	struct{
		uint16 Chan:10;
		uint16 :5;
		uint16 Tune:1;
	}Channel;

	struct{
		uint16 Gpio1:2;
		uint16 Gpio2:2;
		uint16 Gpio3:2;
		uint16 BlndAdj:2;
		uint16 :2;
		uint16 Agc:1;
		uint16 De:1;
		uint16 Rds:1;
		uint16 :1;
		uint16 StcIen:1;
		uint16 RdsIen:1;
	}SysConfig1;

	struct{
		uint16 Volume:4;
		uint16 Space:2;
		uint16 Band:2;
		uint16 SeekTh:8;
	}SysConfig2;

	struct{
		uint16 SkCnt:4;
		uint16 SkSnr:4;
		uint16 VolExt:1;
		uint16 :3;
		uint16 SMuteA:2;
		uint16 SMuteR:2;
	}SysConfig3;

	struct{
		uint16 Rev:14;
		uint16 AhixEn:1;
		uint16 XOscEen:1;
	}Test1;

	uint16 Test2;

	uint16 BootConfig;

	struct{
		uint16 Rssi:8;
		uint16 St:1;
		uint16 BlerA:2;
		uint16 Rdss:1;
		uint16 Afc:1;
		uint16 Sf:1;
		uint16 Stc:1;
		uint16 Rdsr:1;
	}StatusRssi;

	struct{
		uint16 ReadChan:10;
		uint16 BlerD:2;
		uint16 BlerC:2;
		uint16 BlerB:2;
	}ReadChan;

	uint16 RdsA;
	uint16 RdsB;
	uint16 RdsC;
	uint16 RdsD;

	struct{
		uint16 MfgId:12;
		uint16 Pn:4;
	}DeviceId;

	struct{
		uint16 FirmWare:6;
		uint16 Dev:4;
		uint16 Rev:6;
	}ChipId;
}__attribute__ ((packed)) si470x_order_t;

#if defined(_ENABLE_RADIO_RDS_)

typedef struct{
	uint8	enable;
	uint8	factor;				//	信号品质衰减因子
	uint8  	quality;			//	信号强度("0xFF"最强，"0"表示没有信号)
	uint8	discard;			//	解码丢弃数据个数
}si470x_rds_t;

#endif

typedef struct{
	uint8	TunerAutoSeek;
	uint8 	tuner_band;
	uint8  	init_succeed;
	
	uint16 	now_freq;
	uint16 	min_freq;
	uint16 	max_freq;
	
	si470x_order_t order;
#if defined(_ENABLE_RADIO_RDS_)
	si470x_rds_t tuner_rds;
#endif
}si470x_ctrl_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if !defined(SI470x_TUNER_RSSI_FM_VALUE)
	#define SI470x_TUNER_RSSI_FM_VALUE				(16)
#endif

#define SI470x_TUNER_RSSI_FM						(SI470x_TUNER_RSSI_FM_VALUE)


#if !defined(SI470x_TUNER_RSSI_LOC_FM_VALUE)
	#define SI470x_TUNER_RSSI_LOC_FM_VALUE			(SI470x_TUNER_RSSI_FM + 20)
#endif
#define SI470x_TUNER_RSSI_LOC_FM					(SI470x_TUNER_RSSI_LOC_FM_VALUE)


#if !defined(SI470x_TUNER_RSSI_FM_RDS_VALUE)
	#define SI470x_TUNER_RSSI_FM_RDS_VALUE			(SI470x_TUNER_RSSI_FM + 3)
#endif
#define SI470x_TUNER_RSSI_RDS_FM					(SI470x_TUNER_RSSI_FM_RDS_VALUE)

#if !defined(RADIO_SOFT_MUTE_RSSI)
	#define RADIO_SOFT_MUTE_RSSI					(4)
#endif										


#if !defined(SI470x_TUNER_AUTO_SEEK_RSSI_FM)
	#define SI470x_TUNER_AUTO_SEEK_RSSI_FM			(SI470x_TUNER_RSSI_FM - 5)
#endif
#define SI470x_TUNE_AUTO_SEEK_RSSI					SI470x_TUNER_AUTO_SEEK_RSSI_FM

#define SI470x_TUNE_AUTO_SEEK_SNR					(6)
#define SI470x_TUNE_AUTO_SEEK_CNT					(4)


#if defined(_SI470x_TUNER_C19_ST_MODIFY_)

#if !defined(SI470x_TUNER_RSSI_ST)										
#define SI470x_TUNER_RSSI_ST						(7)
#endif
#if !defined(SI470x_TUNER_RSSI_MONO)
#define SI470x_TUNER_RSSI_MONO						(2)
#endif

#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

extern const tuner_drv_fun_t tuner_si470x_drv_fun;

#endif

#endif /* __SI470x_H__ */

