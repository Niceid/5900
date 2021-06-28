#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

//��������
#define ADC1_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC1_KEY2_TEST_VALUE				KEY_ADC_VALUE(3)
#define ADC1_KEY3_TEST_VALUE				KEY_ADC_VALUE(7)
#define ADC1_KEY4_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC1_KEY5_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC1_KEY6_TEST_VALUE				KEY_ADC_VALUE(21)
#define ADC1_KEY7_TEST_VALUE				KEY_ADC_VALUE(27)
#define ADC1_KEY8_TEST_VALUE				KEY_ADC_VALUE(31)
#define ADC1_KEY9_TEST_VALUE				KEY_ADC_VALUE(36)
#define ADC1_KEY10_TEST_VALUE				KEY_ADC_VALUE(44)
#define ADC1_KEY11_TEST_VALUE				KEY_ADC_VALUE(51)
#define ADC1_KEY12_TEST_VALUE				KEY_ADC_VALUE(56)
#define ADC1_KEY13_TEST_VALUE				KEY_ADC_VALUE(60)

#define ADC2_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC2_KEY2_TEST_VALUE				KEY_ADC_VALUE(3)
#define ADC2_KEY3_TEST_VALUE				KEY_ADC_VALUE(7)
#define ADC2_KEY4_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC2_KEY5_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC2_KEY6_TEST_VALUE				KEY_ADC_VALUE(21)
#define ADC2_KEY7_TEST_VALUE				KEY_ADC_VALUE(27)
#define ADC2_KEY8_TEST_VALUE				KEY_ADC_VALUE(31)
#define ADC2_KEY9_TEST_VALUE				KEY_ADC_VALUE(36)
#define ADC2_KEY10_TEST_VALUE				KEY_ADC_VALUE(44)
#define ADC2_KEY11_TEST_VALUE				KEY_ADC_VALUE(51)
#define ADC2_KEY12_TEST_VALUE				KEY_ADC_VALUE(56)
#define ADC2_KEY13_TEST_VALUE				KEY_ADC_VALUE(60)

/*-------------------------------------------------------------------*/
enum{//enter here
	KEY_VALUE_NULL,// 0
/*---------------------------------------------*/
	KEY_VALUE_POWER,// 1
	KEY_VALUE_EQ_LOUD,// 2
	KEY_VALUE_ST,// 3
	KEY_VALUE_BAND_AMS,
	KEY_VALUE_POWER_MODE,// 5
	KEY_VALUE_MODE,
#if defined(_ENABLE_CHANGE_KEY_MAP_)
	KEY_VALUE_TIME_MUTE,// 4
#else
	KEY_VALUE_MUTE,
#endif
	KEY_VALUE_SEL,
#if defined(_ENABLE_SET_FUNTION_)
    KEY_VALUE_SEL1,
#else
	KEY_VALUE_ADD,
#endif	
	KEY_VALUE_TIME,// 10
	KEY_VALUE_POWER_MUTE,
	KEY_VALUE_SUB,
	KEY_VALUE_NAVI_DOWN,
/*---------------------------------------------*/
	KEY_VALUE_M1,
	KEY_VALUE_M2,// 15
	KEY_VALUE_M3,
	KEY_VALUE_M4,
	KEY_VALUE_M5,
	KEY_VALUE_M6,
	KEY_VALUE_NEXT,// 20
	KEY_VALUE_PREV,
	KEY_VALUE_BAND,
	KEY_VALUE_AMS,
#if defined(_SPECIAL_KEY_FUNCTION_)
    KEY_VALUE_LOUD,
#else
	KEY_VALUE_SCAN,
#endif
#if defined(_ENABLE_SET_COLOR_)
	KEY_VALUE_LIGHT,
#else
	KEY_VALUE_LOC,
#endif
	KEY_VALUE_NAVI_UP,//26
/*---------------------------------------------*/
#if defined(_ENABLE_CHANGE_KEY_MAP_)
	KEY_VALUE_MUTE,
#else
	KEY_VALUE_TIME_MUTE,// 4
#endif
#if defined(_ENABLE_SET_COLOR_)
	KEY_VALUE_LOC,
#endif
#if defined(_SPECIAL_KEY_FUNCTION_)
	KEY_VALUE_SCAN,
#else
	KEY_VALUE_LOUD,
#endif
#if defined(_ENABLE_SET_FUNTION_)
    KEY_VALUE_ADD,
#endif
	KEY_VALUE_EQ,
	KEY_VALUE_END,	// 30
};


#define KEY_NAVI_UP				KEY_VALUE_NAVI_UP
#define KEY_NAVI_DOWN			KEY_VALUE_NAVI_DOWN

#define KEY_POWER_MODE			KEY_VALUE_POWER_MODE
#define KEY_POWER_MUTE		    KEY_VALUE_POWER_MUTE

#define KEY_TIME_MUTE		    KEY_VALUE_TIME_MUTE

#endif /* _KEY_MAP_DEMO_H_ */

