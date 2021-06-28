#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_


#define ADC1_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC1_KEY2_TEST_VALUE				KEY_ADC_VALUE(5)
#define ADC1_KEY3_TEST_VALUE				KEY_ADC_VALUE(9)
#define ADC1_KEY4_TEST_VALUE				KEY_ADC_VALUE(14)
#define ADC1_KEY5_TEST_VALUE				KEY_ADC_VALUE(19)
#define ADC1_KEY6_TEST_VALUE				KEY_ADC_VALUE(23)
#define ADC1_KEY7_TEST_VALUE				KEY_ADC_VALUE(27)
#define ADC1_KEY8_TEST_VALUE				KEY_ADC_VALUE(31)
#define ADC1_KEY9_TEST_VALUE				KEY_ADC_VALUE(44)
#define ADC1_KEY10_TEST_VALUE				KEY_ADC_VALUE(49)

/*-------------------------------------------------------------------*/

enum{//enter here
	KEY_VALUE_NULL,// 0
/*---------------------------------------------*/
	KEY_VALUE_POWER,
	KEY_VALUE_MODE,// 15
	KEY_VALUE_M1,// 1
	KEY_VALUE_M2,
	KEY_VALUE_M3,
	KEY_VALUE_TEMP,// 温度设置
	KEY_VALUE_WARM,//加热时间
	KEY_VALUE_WARM_POWER,//加热开关
	KEY_VALUE_NEXT,
	KEY_VALUE_PREV,
/*---------------------------------------------*/
	KEY_VALUE_ADD,
	KEY_VALUE_SUB,
	KEY_VALUE_ADD2,
	KEY_VALUE_SUB2,
	KEY_VALUE_M4,// 10
	KEY_VALUE_M5,
	KEY_VALUE_M6,
	KEY_VALUE_POWER_MUTE,
	KEY_VALUE_BAND_AMS,
	KEY_VALUE_SEL,
	KEY_VALUE_TIME,
	KEY_VALUE_EQ_LOUD,// 2
	KEY_VALUE_ST,// 3
	KEY_VALUE_TIME_MUTE,// 4
	KEY_VALUE_POWER_MODE,// 5
	KEY_VALUE_MUTE,
	KEY_VALUE_NAVI_DOWN,
	KEY_VALUE_BAND,
	KEY_VALUE_AMS,
	KEY_VALUE_LOC,
	KEY_VALUE_NAVI_UP,//26
/*---------------------------------------------*/
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

