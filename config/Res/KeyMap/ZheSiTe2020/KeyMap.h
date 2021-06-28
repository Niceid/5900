#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

//��������
#define ADC1_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC1_KEY2_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC1_KEY3_TEST_VALUE				KEY_ADC_VALUE(22)
#define ADC1_KEY4_TEST_VALUE				KEY_ADC_VALUE(45)

#define ADC2_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC2_KEY2_TEST_VALUE				KEY_ADC_VALUE(3)
#define ADC2_KEY3_TEST_VALUE				KEY_ADC_VALUE(7)
#define ADC2_KEY4_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC2_KEY5_TEST_VALUE				KEY_ADC_VALUE(16)
#define ADC2_KEY6_TEST_VALUE				KEY_ADC_VALUE(22)
#define ADC2_KEY7_TEST_VALUE				KEY_ADC_VALUE(27)
#define ADC2_KEY8_TEST_VALUE				KEY_ADC_VALUE(31)
#define ADC2_KEY9_TEST_VALUE				KEY_ADC_VALUE(49)

/*-------------------------------------------------------------------*/

enum{//enter here
	KEY_VALUE_NULL,// 0
/*---------------------------------------------*/
	KEY_VALUE_POWER_MUTE,// 1
	KEY_VALUE_ANSWER,
	KEY_VALUE_SEL,
	KEY_VALUE_BAND_AMS,
/*---------------------------------------------*/
	KEY_VALUE_M1,
	KEY_VALUE_M2,// 15
	KEY_VALUE_M3,
	KEY_VALUE_M4,
	KEY_VALUE_M5,
	KEY_VALUE_M6,
	KEY_VALUE_NEXT,// 20
	KEY_VALUE_PREV,
	KEY_VALUE_REJECT,
/*---------------------------------------------*/
	KEY_VALUE_BAND,
	KEY_VALUE_MODE,
	KEY_VALUE_TIME_MUTE,
	KEY_VALUE_LOC,
	KEY_VALUE_NAVI_UP,//26
	KEY_VALUE_EQ_LOUD,// 2
	KEY_VALUE_ST,// 3
	KEY_VALUE_AMS,// 4
	KEY_VALUE_POWER_MODE,// 5
	KEY_VALUE_MUTE,
	KEY_VALUE_ADD,
	KEY_VALUE_TIME,// 10
	KEY_VALUE_POWER,
	KEY_VALUE_SUB,
	KEY_VALUE_NAVI_DOWN,
	KEY_VALUE_SCAN,
	KEY_VALUE_LOUD,
	KEY_VALUE_EQ,
	KEY_VALUE_END,	// 30	
};

#define KEY_NAVI_UP				KEY_VALUE_NAVI_UP
#define KEY_NAVI_DOWN			KEY_VALUE_NAVI_DOWN

#define KEY_POWER_MODE			KEY_VALUE_POWER_MODE
#define KEY_POWER_MUTE		    KEY_VALUE_POWER_MUTE

#define KEY_TIME_MUTE		    KEY_VALUE_TIME_MUTE

#endif /* _KEY_MAP_DEMO_H_ */

