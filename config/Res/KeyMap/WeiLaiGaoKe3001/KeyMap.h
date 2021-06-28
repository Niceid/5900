#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_


#define ADC1_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC1_KEY2_TEST_VALUE				KEY_ADC_VALUE(3)
#define ADC1_KEY3_TEST_VALUE				KEY_ADC_VALUE(7)
#define ADC1_KEY4_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC1_KEY5_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC1_KEY6_TEST_VALUE				KEY_ADC_VALUE(22)
#define ADC1_KEY7_TEST_VALUE				KEY_ADC_VALUE(27)
#define ADC1_KEY8_TEST_VALUE				KEY_ADC_VALUE(32)
#define ADC1_KEY9_TEST_VALUE				KEY_ADC_VALUE(49)


#define ADC2_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC2_KEY2_TEST_VALUE				KEY_ADC_VALUE(27)
#define ADC2_KEY3_TEST_VALUE				KEY_ADC_VALUE(31)
#define ADC2_KEY4_TEST_VALUE				KEY_ADC_VALUE(49)

#if defined(SUPPORT_PARTY_CONTROL)
#define ADC3_KEY1_TEST_VALUE				KEY_ADC_VALUE(11)	
#define ADC3_KEY2_TEST_VALUE				KEY_ADC_VALUE(18)
#define ADC3_KEY3_TEST_VALUE				KEY_ADC_VALUE(28)
#define ADC3_KEY4_TEST_VALUE				KEY_ADC_VALUE(39)
#define ADC3_KEY5_TEST_VALUE				KEY_ADC_VALUE(46)	// 35
#define ADC3_KEY6_TEST_VALUE				KEY_ADC_VALUE(53)	// 39
#define ADC3_KEY7_TEST_VALUE				KEY_ADC_VALUE(59)	// 39
#endif

/*-------------------------------------------------------------------*/

enum{
	KEY_VALUE_NULL,
/*---------------------------------------------*/
	KEY_VALUE_M1,				
	KEY_VALUE_M2,				
	KEY_VALUE_M3,				
	KEY_VALUE_M4,				
	KEY_VALUE_M5,				
	KEY_VALUE_M6,
	KEY_VALUE_NEXT,
	KEY_VALUE_PREV,
	KEY_VALUE_AMS,
/*---------------------------------------------*/
	KEY_VALUE_POWER_MUTE,
	KEY_VALUE_ANSWER,
	KEY_VALUE_REJECT,
	KEY_VALUE_SEL,
/*---------------------------------------------*/
#if defined(SUPPORT_PARTY_CONTROL)
	KEY_VALUE_MODE1,							// 27
	KEY_VALUE_ADD, 						// 32
	KEY_VALUE_SUB, 							// 33
	KEY_VALUE_NEXT1,							// 30
	KEY_VALUE_PREV1,							// 31
    KEY_VALUE_REJECT1,
    KEY_VALUE_ANSWER1,
#endif
	KEY_VALUE_POWER,
	KEY_VALUE_TIME,
	KEY_VALUE_EQ,
	KEY_VALUE_NAVI_UP,
	KEY_VALUE_NAVI_DOWN,
	KEY_VALUE_EQ_LOUD,
	KEY_VALUE_POWER1,
	KEY_VALUE_ST,
	KEY_VALUE_LOUD,
	KEY_VALUE_POWER_MODE,
	KEY_VALUE_MODE,
	KEY_VALUE_TIME_MUTE,
	KEY_VALUE_BAND,
	KEY_VALUE_BAND_AMS,
	KEY_VALUE_LOC,	
	KEY_VALUE_MUTE,
	KEY_VALUE_SCAN, 					// 29
	KEY_VALUE_END,								// 30
};

#define KEY_NAVI_UP				KEY_VALUE_NAVI_UP
#define KEY_NAVI_DOWN			KEY_VALUE_NAVI_DOWN

#define KEY_POWER_MODE			KEY_VALUE_POWER_MODE
#define KEY_POWER_MUTE		    KEY_VALUE_POWER_MUTE

#if defined(_FY700B_PARTY_CONTROL_)
#define KEY_POWER_MODE1			KEY_VALUE_MODE1
#endif

#define KEY_TIME_MUTE		    KEY_VALUE_TIME_MUTE

#endif /* _KEY_MAP_DEMO_H_ */
