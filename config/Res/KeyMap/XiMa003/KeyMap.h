#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_


/*-------------------------------------------------------------------*/
enum{
	KEY_VALUE_NULL,
/*---------------------------------------------*/
	KEY_VALUE_POWER_MUTE1,
	
	KEY_VALUE_ADD,
	KEY_VALUE_SUB,
	KEY_VALUE_AMS,//
	KEY_VALUE_TIME,//
	KEY_VALUE_BAND_AMS,
	KEY_VALUE_M4,
	KEY_VALUE_PREV, 	
	KEY_VALUE_M1,
	KEY_VALUE_M5,
	KEY_VALUE_POWER_MUTE,
	KEY_VALUE_M2,
	KEY_VALUE_MODE,
	KEY_VALUE_NEXT,
	KEY_VALUE_M3,
	KEY_VALUE_EQ_CLK,
	KEY_VALUE_M6,
/*---------------------------------------------*/
	KEY_VALUE_MUTE,
	KEY_VALUE_POWER,
	KEY_VALUE_LOUD,
	KEY_VALUE_EQ,
	KEY_VALUE_M0,
	KEY_VALUE_M7,
	KEY_VALUE_M8,
	KEY_VALUE_M9,
	KEY_VALUE_EQ_LOUD,
	KEY_VALUE_TIME_MUTE,
	KEY_VALUE_REJECT,
	KEY_VALUE_POWER_MODE,
	KEY_VALUE_SEL,
	KEY_VALUE_POWER_SEL,
	KEY_VALUE_NAVI_UP,
	KEY_VALUE_BAND,
	KEY_VALUE_LOC,
	KEY_VALUE_NAVI_DOWN,
	KEY_VALUE_ST,
	KEY_VALUE_SCAN,
	KEY_VALUE_END,		
};

#define KEY_NAVI_UP				KEY_VALUE_NAVI_UP
#define KEY_NAVI_DOWN			KEY_VALUE_NAVI_DOWN

#define KEY_POWER_MODE			KEY_VALUE_POWER_MODE
#define KEY_POWER_MUTE		    KEY_VALUE_POWER_MUTE
#define KEY_POWER_MUTE1		    KEY_VALUE_POWER_MUTE1
#define KEY_POWER_SEL		    KEY_VALUE_POWER_SEL

#define KEY_TIME_MUTE		    KEY_VALUE_TIME_MUTE

#endif /* _KEY_MAP_DEMO_H_ */

