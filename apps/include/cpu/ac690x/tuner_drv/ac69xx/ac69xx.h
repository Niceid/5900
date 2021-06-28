#ifndef __AC69xx_H__
#define __AC69xx_H__

#if defined(_TUNER_DRIVER_AC69xx_)

enum{
	AC69xx_TUNER_BAND_NULL = 0,
	AC69xx_TUNER_BAND_FM,
};

typedef struct{
	uint8 			init_succeed;
	radio_band_e 	band;
	uint8 			tuner_band;
	uint16 			now_freq;
	uint16 			min_freq;
	uint16 			max_freq;
	uint8			valid;
	uint8			audio_mute;
	uint8			fadein_cnt;
	s8				rssi_db;
	//uint8			rssi;
}ac69xx_ctrl_t;

extern const tuner_drv_fun_t tuner_ac69xx_drv_fun;

#if !defined(AC69xx_TUNER_RSSI_FM_RSSI)               //内置收音信号强度转换值  -50 到-4  对应19db到66db
	#define AC69xx_TUNER_RSSI_FM_RSSI				(68)
#endif

#if !defined(AC69xx_TUNER_RSSI_FM_VALUE)
	#define AC69xx_TUNER_RSSI_FM_VALUE				(22)
#endif

#define AC69xx_TUNER_RSSI_FM						(AC69xx_TUNER_RSSI_FM_VALUE)


#if !defined(AC69xx_TUNER_RSSI_LOC_FM_VALUE)
	#define AC69xx_TUNER_RSSI_LOC_FM_VALUE			(AC69xx_TUNER_RSSI_FM+18)
#endif
#define AC69xx_TUNER_RSSI_LOC_FM					(AC69xx_TUNER_RSSI_LOC_FM_VALUE)

#endif

#endif /* __AC69xx_H__ */

