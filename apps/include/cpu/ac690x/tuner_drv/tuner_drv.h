#ifndef __TUNER_DRV_H__
#define __TUNER_DRV_H__

typedef enum
{
#if defined(_TUNER_DRIVER_SI473x_)
	TUN_DRV_SI473x,
#endif
#if defined(_TUNER_DRIVER_SI470x_)
	TUN_DRV_SI470x,
#endif
#if defined(_TUNER_DRIVER_EW730_)
	TUN_DRV_EW730,
#endif
#if defined(_TUNER_DRIVER_QN8035_)
	TUN_DRV_QN8035,
#endif
#if defined(_TUNER_DRIVER_QN8065_)
	TUN_DRV_QN8065,
#endif
#if defined(_SUPPORT_C912DAB_)
	TUN_DRV_DAB912,
#endif
#if defined(_TUNER_DRIVER_AR1019_)
	TUN_DRV_AR1019,
#endif
#if defined(_TUNER_DRIVER_RDA580x_)
	TUN_DRV_RDA580x,
#endif
#if defined(_TUNER_DRIVER_LT4730_)
	TUN_DRV_LT4730,
#endif
#if defined(_TUNER_DRIVER_TEA685x_)
	TUN_DRV_TEA685x,
#endif
#if defined(_TUNER_DRIVER_TDA778x_)
	TUN_DRV_TDA778x,
#endif
#if defined(_TUNER_DRIVER_TDA770x_)
	TUN_DRV_TDA770x,
#endif
#if defined(_TUNER_DRIVER_RTC6218_)
	TUN_DRV_RTC6218,
#endif
#if defined(_TUNER_DRIVER_SI475x_)
	TUN_DRV_SI475x,
#endif
#if defined(_TUNER_DRIVER_TDA7708_)
	TUN_DRV_TDA7708,
#endif
#if defined(_TUNER_DRIVER_AC69xx_)
	TUN_DRV_AC69xx,
#endif
	TUN_DRV_NULL,
}fm_drv_e;

typedef enum
{
	TUR_SET_MUTE = 0,
	TUR_SET_STEREO,

	/*RDS*/
	TUR_SET_RDS,
	TUR_SET_RDS_RESET,
	TUR_SET_RDS_DECODE,
	TUR_SET_RDS_EON,
	TUR_SET_RDS_SAVE,
}fm_setting_e;

typedef enum
{
	TUR_INFO_DRV = 0,
	
	TUR_INFO_FM_RSSI,
	TUR_INFO_FM_LOC_RSSI,
	TUR_INFO_FM_DEAD_SPOT_LENGTH,
	TUR_INFO_FM_DEAD_SPOT_TAB,
	
	TUR_INFO_AM_RSSI,
	TUR_INFO_AM_LOC_RSSI,
	TUR_INFO_AM_DEAD_SPOT_LENGTH,
	TUR_INFO_AM_DEAD_SPOT_TAB,
	
	TUR_INFO_AUTOSEEK,
	TUR_INFO_FM_ONLY,
	
	/*RDS*/
	TUR_INFO_RDS,
	TUR_INFO_RDS_INFO,
	TUR_INFO_RDS_APP,
	TUR_INFO_RDS_LOAD,
}fm_info_e;


typedef struct{
	uint8 (*tuner_detect)(void);
	uint8 (*tuner_init)(radio_band_e, uint16, uint16, uint16);
	uint8 (*tuner_exit)(void);
	uint8 (*tuner_freq)(uint16);
	uint8 (*tuner_status)(engine_status_t *);
	uint8 (*tuner_search)(uint16, engine_status_t *);
	uint8 (*tuner_hard)(engine_hard_t *);
	uint8 (*tuner_setting)(fm_setting_e, void *);
	uint8 (*tuner_info)(fm_info_e, void *);
}tuner_drv_fun_t;

extern void tuner_drv_init(void);
extern const tuner_drv_fun_t tuner_drv_func;
extern uint8 drv_tuner_detect(void);

#endif /* __TUNER_DRV_H__ */
