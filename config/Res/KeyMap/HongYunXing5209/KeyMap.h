#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

//????????
#if defined(_ADD_KEY_VALUE_)
#define ADC1_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC1_KEY2_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC1_KEY3_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC1_KEY4_TEST_VALUE				KEY_ADC_VALUE(19)
#define ADC1_KEY5_TEST_VALUE				KEY_ADC_VALUE(27)
#define ADC1_KEY6_TEST_VALUE				KEY_ADC_VALUE(33)
#define ADC1_KEY7_TEST_VALUE				KEY_ADC_VALUE(44)
#define ADC1_KEY8_TEST_VALUE				KEY_ADC_VALUE(51)

#define ADC2_KEY1_TEST_VALUE				KEY_ADC_VALUE(1)
#define ADC2_KEY2_TEST_VALUE				KEY_ADC_VALUE(4)
#define ADC2_KEY3_TEST_VALUE				KEY_ADC_VALUE(8)
#define ADC2_KEY4_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC2_KEY5_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC2_KEY6_TEST_VALUE				KEY_ADC_VALUE(19)
#define ADC2_KEY7_TEST_VALUE				KEY_ADC_VALUE(25)
#define ADC2_KEY8_TEST_VALUE				KEY_ADC_VALUE(42)
#define ADC2_KEY9_TEST_VALUE				KEY_ADC_VALUE(50)
#define ADC2_KEY10_TEST_VALUE				KEY_ADC_VALUE(57)

#else
#define ADC1_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC1_KEY2_TEST_VALUE				KEY_ADC_VALUE(11)
#define ADC1_KEY3_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC1_KEY4_TEST_VALUE				KEY_ADC_VALUE(19)
#define ADC1_KEY5_TEST_VALUE				KEY_ADC_VALUE(33)
#define ADC1_KEY6_TEST_VALUE				KEY_ADC_VALUE(44)
#if defined(_ADD_KEY_FUNCTION_)
#define ADC1_KEY7_TEST_VALUE				KEY_ADC_VALUE(51)
#endif

#define ADC2_KEY1_TEST_VALUE				KEY_ADC_VALUE(1)
#define ADC2_KEY2_TEST_VALUE				KEY_ADC_VALUE(4)
#define ADC2_KEY3_TEST_VALUE				KEY_ADC_VALUE(8)
#define ADC2_KEY4_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC2_KEY5_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC2_KEY6_TEST_VALUE				KEY_ADC_VALUE(19)
#define ADC2_KEY7_TEST_VALUE				KEY_ADC_VALUE(25)
#define ADC2_KEY8_TEST_VALUE				KEY_ADC_VALUE(50)
#define ADC2_KEY9_TEST_VALUE				KEY_ADC_VALUE(57)
#endif

/*-------------------------------------------------------------------*/
#if defined(_ADD_KEY_VALUE_)
enum{
	KEY_VALUE_NULL,// 0
/*---------------------------------------------*/
	KEY_VALUE_POWER,// 1
	KEY_VALUE_NEXT,// 20
	KEY_VALUE_MODE,
	KEY_VALUE_PREV,
    KEY_VALUE_MUTE,
	KEY_VALUE_TIME,// 10
	KEY_VALUE_SEL,
	KEY_VALUE_EQ_LOUD,// 2
/*---------------------------------------------*/
	KEY_VALUE_M4,
	KEY_VALUE_M5,
	KEY_VALUE_M6,
	KEY_VALUE_M1,
	KEY_VALUE_M2,// 15
	KEY_VALUE_M3,
	KEY_VALUE_EJECT,// 3
	KEY_VALUE_POWER_MODE,// 5
	KEY_VALUE_BAND,
	KEY_VALUE_AMS,
/*---------------------------------------------*/
	KEY_VALUE_ST,
	KEY_VALUE_LOUD,
	KEY_VALUE_ADD,
	KEY_VALUE_EQ,
	KEY_VALUE_SUB,
	KEY_VALUE_NAVI_DOWN,
	KEY_VALUE_POWER_MUTE,
	KEY_VALUE_BAND_AMS,
	KEY_VALUE_NAVI_UP,//26
	KEY_VALUE_LOC,
	KEY_VALUE_TIME_MUTE,// 4
	KEY_VALUE_SCAN,
	KEY_VALUE_END,	// 30	
};

#else
enum{
	KEY_VALUE_NULL,// 0
/*---------------------------------------------*/
	KEY_VALUE_POWER,// 1
	KEY_VALUE_NEXT,// 20
	KEY_VALUE_MODE,
	KEY_VALUE_PREV,
	KEY_VALUE_TIME,// 10
	KEY_VALUE_SEL,
#if defined(_ADD_KEY_FUNCTION_)
    KEY_VALUE_MUTE,
#endif	
/*---------------------------------------------*/
	KEY_VALUE_M4,
	KEY_VALUE_M5,
	KEY_VALUE_M6,
	KEY_VALUE_M1,
	KEY_VALUE_M2,// 15
	KEY_VALUE_M3,
	KEY_VALUE_EJECT,// 3
	KEY_VALUE_BAND,
	KEY_VALUE_AMS,
/*---------------------------------------------*/
	KEY_VALUE_ST,
	KEY_VALUE_LOUD,
	KEY_VALUE_POWER_MODE,// 5
	KEY_VALUE_ADD,
	KEY_VALUE_EQ,
	KEY_VALUE_SUB,
	KEY_VALUE_NAVI_DOWN,
	KEY_VALUE_POWER_MUTE,
	KEY_VALUE_BAND_AMS,
	KEY_VALUE_NAVI_UP,//26
	KEY_VALUE_LOC,
	KEY_VALUE_EQ_LOUD,// 2
	KEY_VALUE_TIME_MUTE,// 4
	KEY_VALUE_SCAN,
	KEY_VALUE_END,	// 30	
};
#endif

#define KEY_NAVI_UP				KEY_VALUE_NAVI_UP
#define KEY_NAVI_DOWN			KEY_VALUE_NAVI_DOWN

#define KEY_POWER_MODE			KEY_VALUE_POWER_MODE
#define KEY_POWER_MUTE		    KEY_VALUE_POWER_MUTE

#define KEY_TIME_MUTE		    KEY_VALUE_TIME_MUTE

#endif /* _KEY_MAP_DEMO_H_ */

