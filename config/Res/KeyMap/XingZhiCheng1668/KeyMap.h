#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_


#define ADC1_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC1_KEY2_TEST_VALUE				KEY_ADC_VALUE(31)
#define ADC1_KEY3_TEST_VALUE				KEY_ADC_VALUE(38)
#define ADC1_KEY4_TEST_VALUE				KEY_ADC_VALUE(46)
#define ADC1_KEY5_TEST_VALUE				KEY_ADC_VALUE(55)
#define ADC1_KEY6_TEST_VALUE				KEY_ADC_VALUE(61)

#define ADC2_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC2_KEY2_TEST_VALUE				KEY_ADC_VALUE(2)
#define ADC2_KEY3_TEST_VALUE				KEY_ADC_VALUE(4)
#define ADC2_KEY4_TEST_VALUE				KEY_ADC_VALUE(7)
#define ADC2_KEY5_TEST_VALUE				KEY_ADC_VALUE(10)
#define ADC2_KEY6_TEST_VALUE				KEY_ADC_VALUE(14)
#define ADC2_KEY7_TEST_VALUE				KEY_ADC_VALUE(31)
#define ADC2_KEY8_TEST_VALUE				KEY_ADC_VALUE(38)
#define ADC2_KEY9_TEST_VALUE				KEY_ADC_VALUE(46)
#define ADC2_KEY10_TEST_VALUE				KEY_ADC_VALUE(55)
#define ADC2_KEY11_TEST_VALUE				KEY_ADC_VALUE(61)

#if defined(_XZC1668_SPECIAL_KEY1_)
#define ADC3_KEY1_TEST_VALUE				KEY_ADC_VALUE(4)	
#define ADC3_KEY2_TEST_VALUE				KEY_ADC_VALUE(14)
#define ADC3_KEY3_TEST_VALUE				KEY_ADC_VALUE(27)
#define ADC3_KEY4_TEST_VALUE				KEY_ADC_VALUE(41)
#define ADC3_KEY5_TEST_VALUE				KEY_ADC_VALUE(55)	// 35

#define ADC4_KEY1_TEST_VALUE				KEY_ADC_VALUE(4)	// 9
#define ADC4_KEY2_TEST_VALUE				KEY_ADC_VALUE(14)	// 18
#define ADC4_KEY3_TEST_VALUE				KEY_ADC_VALUE(27)	// 25
#define ADC4_KEY4_TEST_VALUE				KEY_ADC_VALUE(41)	// 30
#define ADC4_KEY5_TEST_VALUE				KEY_ADC_VALUE(55)	// 35

#else

#if defined(_XZC1668_ADKEY_CONTROL_)
#if defined(_XZC1668_SPECIAL_KEY_)
#define ADC3_KEY1_TEST_VALUE				KEY_ADC_VALUE(2)	
#define ADC3_KEY2_TEST_VALUE				KEY_ADC_VALUE(6)
#else
#define ADC3_KEY1_TEST_VALUE				KEY_ADC_VALUE(3)	
#define ADC3_KEY2_TEST_VALUE				KEY_ADC_VALUE(5)
#endif

#define ADC3_KEY3_TEST_VALUE				KEY_ADC_VALUE(10)
#define ADC3_KEY4_TEST_VALUE				KEY_ADC_VALUE(18)
#define ADC3_KEY5_TEST_VALUE				KEY_ADC_VALUE(33)	// 35

#if defined(_XZC1668_SPECIAL_KEY_)
#define ADC4_KEY1_TEST_VALUE				KEY_ADC_VALUE(2)	// 9
#define ADC4_KEY2_TEST_VALUE				KEY_ADC_VALUE(6)	// 18
#else
#define ADC4_KEY1_TEST_VALUE				KEY_ADC_VALUE(3)	// 9
#define ADC4_KEY2_TEST_VALUE				KEY_ADC_VALUE(5)	// 18
#endif
#define ADC4_KEY3_TEST_VALUE				KEY_ADC_VALUE(10)	// 25
#define ADC4_KEY4_TEST_VALUE				KEY_ADC_VALUE(18)	// 30
#define ADC4_KEY5_TEST_VALUE				KEY_ADC_VALUE(33)	// 35
#endif
#endif

/*-------------------------------------------------------------------*/

enum{
	KEY_VALUE_NULL,
/*---------------------------------------------*/
	KEY_VALUE_POWER,
	KEY_VALUE_AM,
	KEY_VALUE_EQ,
	KEY_VALUE_SCAN,
	KEY_VALUE_MUTE,
	KEY_VALUE_FM,
/*---------------------------------------------*/
	KEY_VALUE_M1,
	KEY_VALUE_M2,
	KEY_VALUE_M3,
	KEY_VALUE_M4,
	KEY_VALUE_M5,
	KEY_VALUE_M6,
	KEY_VALUE_PREV,
	KEY_VALUE_MODE,
	KEY_VALUE_AMS,
	KEY_VALUE_SEL,
	KEY_VALUE_NEXT,
/*---------------------------------------------*/
#if defined(_XZC1668_ADKEY_CONTROL_)
	KEY_VALUE_PREV1,							// 31
	KEY_VALUE_NEXT1,							// 30
	KEY_VALUE_ADD1, 						// 32
	KEY_VALUE_SUB1, 							// 33
	KEY_VALUE_MODE1,							// 27
	
	KEY_VALUE_PREV2,							// 28
	KEY_VALUE_NEXT2,							// 30
	KEY_VALUE_ADD2, 						// 32
	KEY_VALUE_SUB2, 							// 33
	KEY_VALUE_MODE2,							// 27
#endif
/*---------------------------------------------*/
	KEY_VALUE_ST,
	KEY_VALUE_LOUD,
	KEY_VALUE_POWER_MODE,
	KEY_VALUE_POWER_MUTE,
	KEY_VALUE_TIME,
	KEY_VALUE_EQ_LOUD,
	KEY_VALUE_NAVI_UP,
	KEY_VALUE_NAVI_DOWN,
	KEY_VALUE_BAND,
	KEY_VALUE_LOC,	
	KEY_VALUE_TIME_MUTE,
	KEY_VALUE_SUB,								// 27
	KEY_VALUE_ADD,								// 28
	KEY_VALUE_BAND_AMS, 					// 29
	KEY_VALUE_END,								// 30
};

#define KEY_NAVI_UP				KEY_VALUE_NAVI_UP
#define KEY_NAVI_DOWN			KEY_VALUE_NAVI_DOWN

#define KEY_POWER_MODE			KEY_VALUE_POWER_MODE
#define KEY_POWER_MUTE		    KEY_VALUE_POWER_MUTE

#if defined(_XZC1668_ADKEY_CONTROL_)
#define KEY_POWER_MODE1			KEY_VALUE_MODE1
#endif

#define KEY_TIME_MUTE		    KEY_VALUE_TIME_MUTE

#endif /* _KEY_MAP_DEMO_H_ */

