#include "sdk_cfg.h"
#include "fm_engine.h"
#include "iic.h"
#include "tuner_drv.h"
#if defined(_ENABLE_ST_MONO_)||defined(_RADIO_FM_GATE_FYDVD_)||defined(_SI474X_REDUCE_AUD_GATE_)||defined(_ENABLE_RADIO_OUTPUT_)
#include "setup.h"
#endif


#if defined(_SUPPORT_RADIO_)

#define TUNER_DEBUG
#ifdef TUNER_DEBUG
#define tuner_printf 		ew_printf
#define tuner_puts 			ew_puts
#else
#define tuner_printf(...)
#define tuner_puts(...)
#endif
extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);
static fm_drv_e tuner_drv_type;

static void radio_scl_init(void)		{	I2C_SCL_INIT();					};
static void radio_scl_h(void)			{	I2C_SCL_H();					};
static void radio_scl_l(void)			{	I2C_SCL_L();					};
static void radio_sda_init(void)		{	I2C_SDA_INIT();					};
static void radio_sda_h(void)		{	I2C_SDA_H();					};
static void radio_sda_l(void)			{	I2C_SDA_L();					};
static uint8 radio_sda_get(void)		{	return (I2C_SDA_GET()?1:0);		};

static const i2c_gpio_t radio_gpio = 
{
	radio_scl_init,
	radio_scl_h,
	radio_scl_l,
	radio_sda_init,
	radio_sda_h,
	radio_sda_l,
	radio_sda_get,
};

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_TUNER_DRIVER_RESET_)
static void tuner_reset_ctrl(uint8 on_off)
{
	RADIO_RESET_INIT();
	if(on_off)
	{
		RADIO_RESET_ON();
	}
	else
	{
		RADIO_RESET_OFF();
	}
}

static void tuner_reset_disable(void)
{
	RADIO_RESET_INIT();
	RADIO_RESET_IN();
}

#endif

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_TUNER_DRIVER_AC69xx_)
	#include "ac69xx\ac69xx.c"
#endif
#if defined(_TUNER_DRIVER_SI473x_)
	#include "si473x\si473x.c"
#endif
#if defined(_TUNER_DRIVER_SI470x_)
	#include "si470x\si470x.c"
#endif
#if defined(_TUNER_DRIVER_QN8035_)
	#include "qn8035\qn8035.c"
#endif
#if defined(_TUNER_DRIVER_QN8065_)
	#include "qn8065\qn8065.c"
#endif
#if defined(_SUPPORT_C912DAB_)
	#include "C912DAB\C912dab.c"
#endif
#if defined(_SUPPORT_DAB_H868DAB)
	#include "H868DAB\H868DAB.c"
#endif

#if defined(_TUNER_DRIVER_AR1019_)
	#include "ar1019\ar1019.c"
#endif
#if defined(_TUNER_DRIVER_RDA580x_)
	#include "rda580x\rda580x.c"
#endif
#if defined(_TUNER_DRIVER_LT4730_)
	#include "lt4730\lt4730.c"
#endif
#if defined(_TUNER_DRIVER_TEA685x_)
	#include "tea685x\tea685x.c"
#endif
#if defined(_TUNER_DRIVER_TDA778x_)
	#include "tda778x\tda778x.c"
#endif
#if defined(_TUNER_DRIVER_TDA770x_)
	#include "tda770x\tda770x.c"
#endif
#if defined(_TUNER_DRIVER_EW730_)
	#include "ew730\ew730.c"
#endif
#if defined(_TUNER_DRIVER_RTC6218_)
	#include "rtc6218\rtc6218.c"
#endif

#if defined(_TUNER_DRIVER_SI475x_)
	#include "si475x\si475x.c"
#endif

#if defined(_TUNER_DRIVER_TDA7708_)
	#include "tda7708\tda7708.c"
#endif

static const tuner_drv_fun_t *tuner_drv_fun[] =
{
#if defined(_TUNER_DRIVER_SI473x_)
	&tuner_si473x_drv_fun,
#endif
#if defined(_TUNER_DRIVER_SI470x_)
	&tuner_si470x_drv_fun,
#endif
#if defined(_TUNER_DRIVER_EW730_)
	&tuner_ew730_drv_fun,
#endif
#if defined(_TUNER_DRIVER_QN8035_)
	&tuner_qn8035_drv_fun,
#endif
#if defined(_TUNER_DRIVER_QN8065_)
	&tuner_qn8065_drv_fun,
#endif
#if defined(_SUPPORT_C912DAB_)
	&tuner_c912dab_drv_fun,
#endif
#if defined(_SUPPORT_DAB_H868DAB)
	&tuner_H868dab_drv_fun,
#endif

#if defined(_TUNER_DRIVER_AR1019_)
	&tuner_ar1019_drv_fun,
#endif
#if defined(_TUNER_DRIVER_RDA580x_)
	&tuner_rda580x_drv_fun,
#endif
#if defined(_TUNER_DRIVER_LT4730_)
	&tuner_lt4730_drv_fun,
#endif
#if defined(_TUNER_DRIVER_TEA685x_)
	&tuner_tea685x_drv_fun,
#endif
#if defined(_TUNER_DRIVER_TDA778x_)
	&tuner_tda778x_drv_fun,
#endif
#if defined(_TUNER_DRIVER_TDA770x_)
	&tuner_tda770x_drv_fun,
#endif
#if defined(_TUNER_DRIVER_RTC6218_)
	&tuner_rtc6218_drv_fun,
#endif
#if defined(_TUNER_DRIVER_SI475x_)
	&tuner_si475x_drv_fun,
#endif
#if defined(_TUNER_DRIVER_TDA7708_)
	&tuner_tda7708_drv_fun,
#endif
#if defined(_TUNER_DRIVER_AC69xx_)
	&tuner_ac69xx_drv_fun,
#endif
};

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

uint8 drv_tuner_detect(void)
{
	tuner_printf("drv_tuner_detect, tuner_type:%d\n", tuner_drv_type);
	if(tuner_drv_type == TUN_DRV_NULL)
	{
		uint8 type;
    #if defined(_SPECIAL_RADIO_INIT_)
    	if(setup.radio)
    	{
    		tuner_reset_ctrl(ON);
    		OSTimeDly(20);
    		tuner_reset_ctrl(OFF);
    		OSTimeDly(40);
    		setup.radio = FALSE;
    	}
    #else
        #if defined(_TUNER_DRIVER_RESET_)
        	tuner_reset_ctrl(ON);
        #if defined(_SUPPORT_C912DAB_)
        	DAB_RESET_INIT();
        	DAB_RESET_OUT();
        	DAB_RESET_HIGH();
        #endif
        	OSTimeDly(20);
        	
        	tuner_reset_ctrl(OFF);
        #if defined(_SUPPORT_C912DAB_)
        	DAB_RESET_INIT();
        	DAB_RESET_OUT();
        	DAB_RESET_LOW();
        #endif
        	
        	OSTimeDly(40);
        #endif
    #endif
		
		for(type = 0; type < TUN_DRV_NULL; type++)
		{
			if(tuner_drv_fun[type]->tuner_detect != NULL)
			{
				if(tuner_drv_fun[type]->tuner_detect())
				{
					tuner_printf("tuner_type:%d\n", type);
					tuner_drv_type = type;
				#if defined(_ENABLE_ST_MONO_)||defined(_RADIO_FM_GATE_FYDVD_)||defined(_SI474X_REDUCE_AUD_GATE_)||defined(_ENABLE_RADIO_OUTPUT_)
					global.tuner_drive =tuner_drv_type;
				#endif
					return TRUE;
				}
			}
		}
		tuner_puts("tuner_type_fail!!!\n");
		return FALSE;
	}
	return TRUE;
}
static uint8 drv_tuner_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	if(tuner_drv_type < TUN_DRV_NULL)
	{
		if(tuner_drv_fun[tuner_drv_type]->tuner_init != NULL)
		{
			return tuner_drv_fun[tuner_drv_type]->tuner_init(band, min_freq, max_freq, freq);
		}
	}
	return FALSE;
}
static uint8 drv_tuner_exit(void)
{
	uint8 result = FALSE;
	if(tuner_drv_type < TUN_DRV_NULL)
	{
		if(tuner_drv_fun[tuner_drv_type]->tuner_exit != NULL)
		{
			result = tuner_drv_fun[tuner_drv_type]->tuner_exit();
		}
		tuner_drv_type = TUN_DRV_NULL;
	#if defined(_TUNER_DRIVER_RESET_)
	#if !defined(_ENABLE_RADIO_RESET_)
		tuner_reset_disable();
	#endif
	#endif
	}
	return result;
}
static uint8 drv_tuner_freq(uint16 freq)
{
	if(tuner_drv_type < TUN_DRV_NULL)
	{
		if(tuner_drv_fun[tuner_drv_type]->tuner_freq != NULL)
		{
			return tuner_drv_fun[tuner_drv_type]->tuner_freq(freq);
		}
	}
	return FALSE;
}
static uint8 drv_tuner_status(engine_status_t *status)
{
	if(tuner_drv_type < TUN_DRV_NULL)
	{
		if(tuner_drv_fun[tuner_drv_type]->tuner_status != NULL)
		{
			return tuner_drv_fun[tuner_drv_type]->tuner_status(status);
		}
	}
	return FALSE;
}
static uint8 drv_tuner_search(uint16 freq, engine_status_t *status)
{
	if(tuner_drv_type < TUN_DRV_NULL)
	{
		if(tuner_drv_fun[tuner_drv_type]->tuner_search != NULL)
		{
			return tuner_drv_fun[tuner_drv_type]->tuner_search(freq, status);
		}
	}
	return FALSE;
}
static uint8 drv_tuner_hard(engine_hard_t *hard)
{
	if(tuner_drv_type < TUN_DRV_NULL)
	{
		if(tuner_drv_fun[tuner_drv_type]->tuner_hard != NULL)
		{
			return tuner_drv_fun[tuner_drv_type]->tuner_hard(hard);
		}
	}
	return FALSE;
}
static uint8 drv_tuner_setting(fm_setting_e type, void *set)
{
	if(tuner_drv_type < TUN_DRV_NULL)
	{
		if(tuner_drv_fun[tuner_drv_type]->tuner_setting != NULL)
		{
			return tuner_drv_fun[tuner_drv_type]->tuner_setting(type, set);
		}
	}
	return FALSE;
}
static uint8 drv_tuner_info(fm_info_e type, void *info)
{
	if(tuner_drv_type < TUN_DRV_NULL)
	{
		if(tuner_drv_fun[tuner_drv_type]->tuner_info != NULL)
		{
			return tuner_drv_fun[tuner_drv_type]->tuner_info(type, info);
		}
	}
	return FALSE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

void tuner_drv_init(void)
{
	tuner_drv_type = TUN_DRV_NULL;
	tuner_puts("tuner_drv_init\n");
}

const tuner_drv_fun_t tuner_drv_func =
{
	drv_tuner_detect,
	drv_tuner_init,
	drv_tuner_exit,
	drv_tuner_freq,
	drv_tuner_status,
	drv_tuner_search,
	drv_tuner_hard,
	drv_tuner_setting,
	drv_tuner_info,
};

#endif

