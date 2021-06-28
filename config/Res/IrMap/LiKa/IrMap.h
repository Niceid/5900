#ifndef _IR_MAP_H_
#define _IR_MAP_H_
//here

/**************************************************/
/**************************************************/
/**************************************************/
#define IR_USER_CODE				0xFF00
#define IR_INVALIDATE				0xF0
/**************************************************/
/**************************************************/
/**************************************************/

#define IR_CODE_POWER 				0x0C
#define IR_CODE_MODE				0x11

#define IR_CODE_MUTE				0x12
#define IR_CODE_ADD_VOL				0x14
#define IR_CODE_CLK					0x16

#define IR_CODE_BACK				0x07
#define IR_CODE_SEL					0x13
#define IR_CODE_NEXT				0x06

#define IR_CODE_BAND				0x08
#define IR_CODE_SUB_VOL				0x15
#define IR_CODE_AMS					0x09

#define IR_CODE_LOUD				0x0F
#define IR_CODE_ST				    0x0E
#define IR_CODE_LOC				    0x10

#define IR_CODE_M1					0x00
#define IR_CODE_M2					0x01
#define IR_CODE_M3					0x02

#define IR_CODE_M4					0x03
#define IR_CODE_M5					0x04
#define IR_CODE_M6					0x05

/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR_CODE_SCAN				IR_INVALIDATE
#define IR_CODE_PAUSE				IR_INVALIDATE
#define IR_CODE_DOOR				IR_INVALIDATE
#define IR_CODE_PTY					IR_INVALIDATE
#define IR_CODE_TA					IR_INVALIDATE
#define IR_CODE_AF					IR_INVALIDATE
#define IR_CODE_EQ					IR_INVALIDATE


/**************************************************/
/*********************TAB**************************/
/**************************************************/

enum{
	IR_VALUE_NULL,
	IR_VALUE_POWER,
	IR_VALUE_MODE,
	IR_VALUE_PLAY,
	IR_VALUE_STOP,
	IR_VALUE_MUTE,
	IR_VALUE_BAND,
	IR_VALUE_SCAN,
	IR_VALUE_AMS,
	IR_VALUE_ST,
	IR_VALUE_LOC,
	IR_VALUE_LOUD,
	IR_VALUE_EQ,
	IR_VALUE_CLK,
	IR_VALUE_EJECT,
	IR_VALUE_M0,
	IR_VALUE_M1,
	IR_VALUE_M2,
	IR_VALUE_M3,
	IR_VALUE_M4,
	IR_VALUE_M5,
	IR_VALUE_M6,
	IR_VALUE_M7,
	IR_VALUE_M8,
	IR_VALUE_M9,
	IR_VALUE_M10,
	IR_VALUE_LEFT,
	IR_VALUE_RIGHT,
	IR_VALUE_UP,
	IR_VALUE_DOWN,
	IR_VALUE_ENTER,
	IR_VALUE_RETURN,
	IR_VALUE_HOME,
	IR_VALUE_BACK,
	IR_VALUE_NEXT,
	IR_VALUE_FORWARD,
	IR_VALUE_BACKWARD,
	IR_VALUE_ADD_VOL,
	IR_VALUE_SUB_VOL,
	IR_VALUE_SEL,
	IR_VALUE_MENU,
	IR_VALUE_SETUP,
	IR_VALUE_TITLE,
	IR_VALUE_SUB_T,
	IR_VALUE_ANGLE,
	IR_VALUE_RPT,
	IR_VALUE_SLOW,
	IR_VALUE_ZOOM,
	IR_VALUE_PBC,
	IR_VALUE_OSD,
	IR_VALUE_AUDIO,
	IR_VALUE_GOTO,
	IR_VALUE_END,
};


#define IR_MAP_TAB										\
	{IR_CODE_M1,			IR_VALUE_M1},				\
	{IR_CODE_M2,			IR_VALUE_M2},				\
	{IR_CODE_M3,			IR_VALUE_M3},				\
	{IR_CODE_M4,			IR_VALUE_M4},				\
	{IR_CODE_M5,			IR_VALUE_M5},				\
	{IR_CODE_M6,			IR_VALUE_M6},				\
	{IR_CODE_POWER,			IR_VALUE_POWER},			\
	{IR_CODE_MODE,			IR_VALUE_MODE},				\
	{IR_CODE_MUTE,			IR_VALUE_MUTE},				\
	{IR_CODE_BAND,			IR_VALUE_BAND},				\
	{IR_CODE_AMS,			IR_VALUE_AMS},				\
	{IR_CODE_LOC,			IR_VALUE_LOC},				\
	{IR_CODE_ST,			IR_VALUE_ST},				\
	{IR_CODE_EQ,			IR_VALUE_EQ},				\
	{IR_CODE_ADD_VOL,		IR_VALUE_ADD_VOL},			\
	{IR_CODE_SUB_VOL,		IR_VALUE_SUB_VOL},			\
	{IR_CODE_SEL,			IR_VALUE_SEL},				\
	{IR_CODE_BACK,			IR_VALUE_BACK},				\
	{IR_CODE_NEXT,			IR_VALUE_NEXT},				\
	{IR_CODE_CLK,			IR_VALUE_CLK},				\
	{IR_CODE_LOUD,			IR_VALUE_LOUD},				\
		
#endif /* _IR_MAP_H_ */


