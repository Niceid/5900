#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_


#define ADC1_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC1_KEY2_TEST_VALUE				KEY_ADC_VALUE(2)
#define ADC1_KEY3_TEST_VALUE				KEY_ADC_VALUE(4)
#define ADC1_KEY4_TEST_VALUE				KEY_ADC_VALUE(8)
#define ADC1_KEY5_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC1_KEY6_TEST_VALUE				KEY_ADC_VALUE(17)
#define ADC1_KEY7_TEST_VALUE				KEY_ADC_VALUE(23)
#define ADC1_KEY8_TEST_VALUE				KEY_ADC_VALUE(29)
#define ADC1_KEY9_TEST_VALUE				KEY_ADC_VALUE(36)
#define ADC1_KEY10_TEST_VALUE				KEY_ADC_VALUE(42)
#define ADC1_KEY11_TEST_VALUE				KEY_ADC_VALUE(56)

#if defined(_ENABLE_RADIO_RDS_)
#define ADC2_KEY1_TEST_VALUE				KEY_ADC_VALUE(0)
#define ADC2_KEY2_TEST_VALUE				KEY_ADC_VALUE(2)
#define ADC2_KEY3_TEST_VALUE				KEY_ADC_VALUE(4)
#define ADC2_KEY4_TEST_VALUE				KEY_ADC_VALUE(8)
#define ADC2_KEY5_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC2_KEY6_TEST_VALUE				KEY_ADC_VALUE(17)
#define ADC2_KEY7_TEST_VALUE				KEY_ADC_VALUE(23)
#define ADC2_KEY8_TEST_VALUE				KEY_ADC_VALUE(52)
#else
#define ADC2_KEY1_TEST_VALUE				KEY_ADC_VALUE(2)
#define ADC2_KEY2_TEST_VALUE				KEY_ADC_VALUE(4)
#define ADC2_KEY3_TEST_VALUE				KEY_ADC_VALUE(12)
#define ADC2_KEY4_TEST_VALUE				KEY_ADC_VALUE(23)
#define ADC2_KEY5_TEST_VALUE				KEY_ADC_VALUE(52)
#endif

/*-------------------------------------------------------------------*/
#if defined(_ENABLE_RADIO_RDS_)
enum{
	 KEY_VALUE_NULL 			//	0
/*Key1*/
    ,KEY_VALUE_M1			 //  13
    ,KEY_VALUE_M2			 //  14
    ,KEY_VALUE_M3			 //  15
    ,KEY_VALUE_M4			 //  16
    ,KEY_VALUE_M5			 //  17
    ,KEY_VALUE_M6			 //  18
    ,KEY_VALUE_BAND			//	6
    ,KEY_VALUE_NEXT			 //  19
    ,KEY_VALUE_PREV			 //  20
    ,KEY_VALUE_SEL 			//	8
    ,KEY_VALUE_AMS 			 //  22  
/*Key2*/	
	,KEY_VALUE_MUTE		 //  7
	,KEY_VALUE_TIME		   //  10  
	,KEY_VALUE_POWER			  //  1
	,KEY_VALUE_EQ				  //  2 
    ,KEY_VALUE_LOC
    ,KEY_VALUE_SCAN
	,KEY_VALUE_MODE			 //  21
	,KEY_VALUE_ST				  //  3
//---------------------------------------------//
/*δʹ??*/
    ,KEY_VALUE_ADD 		 // 	
    ,KEY_VALUE_POWER_MUTE	   //  23
    ,KEY_VALUE_SUB 		//
    ,KEY_VALUE_MENU			 //  
    ,KEY_VALUE_ANSWER			 //  24   
	,KEY_VALUE_LOUD			 //  4
	,KEY_VALUE_POWER_MODE		 //  5
	,KEY_VALUE_BAND_AMS
	,KEY_VALUE_NAVI_DOWN
	,KEY_VALUE_TIME_MUTE
	,KEY_VALUE_EQ_LOUD
	,KEY_VALUE_PTY		          //  
	,KEY_VALUE_NAVI_UP
//---------------------------------------------//
};

#else

enum{
	 KEY_VALUE_NULL 			//	0
/*Key1*/
    ,KEY_VALUE_M1			 //  13
    ,KEY_VALUE_M2			 //  14
    ,KEY_VALUE_M3			 //  15
    ,KEY_VALUE_M4			 //  16
    ,KEY_VALUE_M5			 //  17
    ,KEY_VALUE_M6			 //  18
    ,KEY_VALUE_BAND			//	6
    ,KEY_VALUE_NEXT			 //  19
    ,KEY_VALUE_PREV			 //  20
    ,KEY_VALUE_SEL 			//	8
    ,KEY_VALUE_AMS 			 //  22  
/*Key2*/	
	,KEY_VALUE_TIME		   //  10  
	,KEY_VALUE_POWER			  //  1
	,KEY_VALUE_EQ				  //  2 
	,KEY_VALUE_MODE			 //  21
	,KEY_VALUE_ST				  //  3
//---------------------------------------------//
/*δʹ??*/
    ,KEY_VALUE_ADD 		 // 	
    ,KEY_VALUE_POWER_MUTE	   //  23
    ,KEY_VALUE_SUB 		//
    ,KEY_VALUE_MENU			 //  
    ,KEY_VALUE_ANSWER			 //  24   
	,KEY_VALUE_LOUD			 //  4
	,KEY_VALUE_POWER_MODE		 //  5
	,KEY_VALUE_MUTE			//	7
	,KEY_VALUE_BAND_AMS
	,KEY_VALUE_NAVI_DOWN
	,KEY_VALUE_TIME_MUTE
	,KEY_VALUE_EQ_LOUD
	,KEY_VALUE_PTY		          //  
	,KEY_VALUE_SCAN
	,KEY_VALUE_NAVI_UP
	,KEY_VALUE_LOC
//---------------------------------------------//
};
#endif

#define KEY_NAVI_UP				KEY_VALUE_NAVI_UP
#define KEY_NAVI_DOWN			KEY_VALUE_NAVI_DOWN

#define KEY_POWER_MODE			KEY_VALUE_POWER_MODE
#define KEY_POWER_MUTE		    KEY_VALUE_POWER_MUTE

#define KEY_TIME_MUTE		    KEY_VALUE_TIME_MUTE

#endif /* _KEY_MAP_DEMO_H_ */

