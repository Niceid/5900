#ifndef __LINEIN2_TASK_H__
#define __LINEIN2_TASK_H__

#define AUX1_DEBUG
#ifdef AUX1_DEBUG
    #define aux1_puts     ew_puts
    #define aux1_printf   ew_printf
#else
    #define aux1_puts(...)
    #define aux1_printf(...)
#endif


#define LINEIN1_POLLING_TIME			(100)
#if REC_EN || DAC2IIS_EN
	#define AUX1_AD_ENABLE		1
#else
	#define AUX1_AD_ENABLE		0
#endif


#endif /* __LINEIN2_TASK_H__ */

