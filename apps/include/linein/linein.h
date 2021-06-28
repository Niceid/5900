#ifndef __LINEIN_TASK_H__
#define __LINEIN_TASK_H__

#define AUX_DEBUG
#ifdef AUX_DEBUG
    #define aux_puts     ew_puts
    #define aux_printf   ew_printf
#else
    #define aux_puts(...)
    #define aux_printf(...)
#endif


#define LINEIN_POLLING_TIME			(100)
#if REC_EN || DAC2IIS_EN
	#define AUX_AD_ENABLE		1
#else
	#define AUX_AD_ENABLE		0
#endif


#endif /* __LINEIN_TASK_H__ */

