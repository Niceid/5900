#ifndef __KEY_DRV_AD_H__
#define __KEY_DRV_AD_H__

#include "sdk_cfg.h"

//AD channel define
#define AD_KEY_PA3    		(0x0<<8)
#define AD_KEY_PA4    		(0x1<<8)
#define AD_KEY_PA5    		(0x2<<8)
#define AD_KEY_PA6    		(0x3<<8)
#define AD_KEY_PA9    		(0x4<<8)
#define AD_KEY_PA10   		(0x5<<8)
#define AD_KEY_PB7    		(0x6<<8)
#define AD_KEY_PB8    		(0x7<<8)
#define AD_KEY_PB9    		(0x8<<8)
#define AD_KEY_PB10   		(0x9<<8)
#define AD_KEY_PB11   		(0xA<<8)
#define AD_KEY_PB12   		(0xB<<8)
#define AD_KEY_PR1    		(0xC<<8)
#define AD_KEY_PR2    		(0xD<<8)
#define AD_KEY_LDOIN  		(0xE<<8)
#define AD_KEY_LDOREF 		(0xF<<8)


#if defined(ADC_KEY1_PIN)
#define AD_KEY_KEY1   		ADC_KEY1_PIN
#endif

#if defined(ADC_KEY2_PIN)
#define AD_KEY_KEY2			ADC_KEY2_PIN
#endif

#if defined(_FY700B_PARTY_CONTROL_)||defined(_XZC1668_ADKEY_CONTROL_)||defined(_KLD3300_PARTY_CONTROL_)||defined(_SUPPORT_SPECIAL_KEY_FUNCTION_)||defined(_YJ3993_ADKEY_CONTROL_)
#define AD_KEY_KEY3			ADC_KEY3_PIN
#if !defined(_DISABLE_PARTY_KEY2_)
#define AD_KEY_KEY4			ADC_KEY4_PIN
#endif
#endif

#if defined(ADC_ENCODE_PIN)
#define AD_KEY_ENCODE		ADC_ENCODE_PIN
#endif

#if defined(ADC_ENCODE2_PIN)
#define AD_KEY_ENCODE2		ADC_ENCODE2_PIN
#endif

#if defined(ADC_ACC_PIN)
#define AD_KEY_ACC			ADC_ACC_PIN
#endif

#if defined(_SUPPORT_REAL_VBAR_)
#define AD_KEY_VBAR		    ADC_VBAR_PIN
#endif

#if defined(SUPPORT_LOW_PWR_DET)
#define AD_KEY_BATT_DET		    BATT_DET_PIN
#endif

#if defined(_SUPPORT_USB_DET)
#define AD_KEY_USB_DET			ADC_USB_CHRG_PIN
#endif

#if defined(_ENABLE_DISC_DETECT_)
#define AD_KEY_DISC   		ADC_DISC_PIN
#endif
#if defined(SUPPORT_PARTY_CONTROL)||defined(_SL6812_PARTY_CONTROL_)
#define AD_KEY_KEY3			ADC_KEY3_PIN
#endif

enum
{
#if defined(AD_KEY_KEY1)
	AD_CH_KEY1,
#endif

#if defined(AD_KEY_KEY2)
	AD_CH_KEY2,
#endif

#if defined(AD_KEY_ENCODE)
	AD_CH_ENCODE,
#endif

#if defined(_SUPPORT_VR2_)
	AD_CH_ENCODE2,
#endif

#if defined(AD_KEY_ACC)
	AD_CH_ACC,
#endif

#if defined(_SUPPORT_REAL_VBAR_)
	AD_CH_VBAR,
#endif

#if defined(SUPPORT_LOW_PWR_DET)
	AD_CH_BATT_DET,
#endif

#if defined(_SUPPORT_USB_DET)
	AD_CH_USB_DET,
#endif

#if defined(_ENABLE_DISC_DETECT_)
	AD_CH_DISC,
#endif
#if defined(SUPPORT_PARTY_CONTROL)||defined(_SL6812_PARTY_CONTROL_)
	AD_CH_KEY3,
#endif

#if defined(_FY700B_PARTY_CONTROL_)||defined(_XZC1668_ADKEY_CONTROL_)||defined(_KLD3300_PARTY_CONTROL_)||defined(_SUPPORT_SPECIAL_KEY_FUNCTION_)||defined(_YJ3993_ADKEY_CONTROL_)
	AD_CH_KEY3,

#if !defined(_DISABLE_PARTY_KEY2_)
	AD_CH_KEY4,
#endif
#endif
    AD_CH_MAX,
};

#if defined(_SUPPORT_TOUCH_KEY_)
extern uint8 touch_key_scan(void);
#endif

extern uint8 adc_value(uint8);
#if defined(_SUPPORT_USB_DET)
extern uint16 adc_value_u16(uint8);
#endif
extern void ad_key_init(void);
extern uint8 ad_key_scan(void);
#if defined(_SUPPORT_REAL_VBAR_)
extern uint8 ad_vbar_scan(void);
#endif

#if defined(AD_KEY_KEY1)
#define GET_KEY1_DATA					adc_value(AD_CH_KEY1)
#endif

#if defined(AD_KEY_KEY2)
#define GET_KEY2_DATA					adc_value(AD_CH_KEY2)
#endif

#if defined(_FY700B_PARTY_CONTROL_)||defined(_XZC1668_ADKEY_CONTROL_)||defined(_KLD3300_PARTY_CONTROL_)||defined(_SUPPORT_SPECIAL_KEY_FUNCTION_)||defined(_YJ3993_ADKEY_CONTROL_)
#define GET_KEY3_DATA					adc_value(AD_CH_KEY3)
#if !defined(_DISABLE_PARTY_KEY2_)
#define GET_KEY4_DATA					adc_value(AD_CH_KEY4)
#endif
#endif

#if defined(AD_KEY_ENCODE)
#define GET_ENCODE_DATA					adc_value(AD_CH_ENCODE)
#endif

#if defined(_SUPPORT_VR2_)
#define GET_ENCODE2_DATA				adc_value(AD_CH_ENCODE2)
#endif

#if defined(AD_KEY_ACC)
#define GET_ACC_DATA					adc_value(AD_CH_ACC)
#endif

#if defined(_SUPPORT_REAL_VBAR_)
#define GET_VBAR_DATA					adc_value(AD_CH_VBAR)
#endif

#if defined(SUPPORT_LOW_PWR_DET)
#define GET_BATT_DET_DATA				adc_value(AD_CH_BATT_DET)
#define BAT_PWR_DET_CHECK()				(((GET_BATT_DET_DATA>=16)&&(GET_BATT_DET_DATA <=31)) ? 1 : 0)
#endif

#if defined(_SUPPORT_USB_DET)
#define GET_USB_DET_DATA				adc_value_u16(AD_CH_USB_DET)
#define USB_SWITCH_CHECK()				(((GET_USB_DET_DATA>=23)&&(GET_USB_DET_DATA <=180)) ? 1 : 0)
#endif

#if defined(SUPPORT_PARTY_CONTROL)||defined(_SL6812_PARTY_CONTROL_)
#if defined(_SPECIAL_PARTY_CONTROL_)
#define GET_KEY3_DATA					adc_value_party(AD_CH_KEY3)
#else
#define GET_KEY3_DATA					adc_value(AD_CH_KEY3)
#endif
#endif


#if defined(_ENABLE_DISC_DETECT_)
//#if defined(_SPECIAL_DISC_DETECT_)//DISC:1.95V(37); SD:1.47V(29); ͬʱ1.1V(19)
#define GET_DISC_DATA					adc_value(AD_CH_DISC)
#define DETECT_DISC_CHECK()				((((GET_DISC_DATA > 34)&&(GET_DISC_DATA < 43))||((GET_DISC_DATA > 15)&&(GET_DISC_DATA < 24))) ? 1 : 0)
#define DISC_SWC_CHECK()				((((GET_DISC_DATA > 34)&&(GET_DISC_DATA < 43))||((GET_DISC_DATA > 15)&&(GET_DISC_DATA < 24))) ? 1 : 0)
#define DETECT_SD_CHECK()				((((GET_DISC_DATA > 24)&&(GET_DISC_DATA < 33))||((GET_DISC_DATA > 15)&&(GET_DISC_DATA < 24))) ? 1 : 0)

//#else
//DISC:2.44V(41); SD:1.7V(29); ͬʱ1.36V(23)
/*#define GET_DISC_DATA					adc_value(AD_CH_DISC)
#define DETECT_DISC_CHECK()				((((GET_DISC_DATA > 37)&&(GET_DISC_DATA < 50))||((GET_DISC_DATA > 19)&&(GET_DISC_DATA < 27))) ? 1 : 0)
#define DISC_SWC_CHECK()				((((GET_DISC_DATA > 37)&&(GET_DISC_DATA < 50))||((GET_DISC_DATA > 19)&&(GET_DISC_DATA < 27))) ? 1 : 0)

#define DETECT_SD_CHECK()				((((GET_DISC_DATA > 26)&&(GET_DISC_DATA < 35))||((GET_DISC_DATA > 19)&&(GET_DISC_DATA < 27))) ? 1 : 0)
#endif*/
#endif

#endif/*__KEY_DRV_AD_H__*/
