#ifndef _IR_MAP_H_
#define _IR_MAP_H_

/**************************************************/
/**************************************************/
/**************************************************/
#define IR_USER_CODE				0xFF00
#define IR_INVALIDATE				0xF0
/**************************************************/
/**************************************************/
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
	IR_VALUE_REJECT,
	IR_VALUE_ANSWER,
	IR_VALUE_MODE1,
	IR_VALUE_MUTE1,
	IR_VALUE_BACK1,
	IR_VALUE_NEXT1,
	IR_VALUE_ADD_VOL1,
	IR_VALUE_SUB_VOL1,
	IR_VALUE_SEL1,
	IR_VALUE_BAND1,
};


#define IR_CODE_POWER 				0x45
#define IR_CODE_MUTE				0x46
#define IR_CODE_MODE				0x47

#define IR_CODE_M1					0x44
#define IR_CODE_M2					0x40
#define IR_CODE_M3					0x43

#define IR_CODE_M4					0x07
#define IR_CODE_M5					0x15
#define IR_CODE_M6					0x09

#define IR_CODE_LOUD				0x16
#define IR_CODE_ADD_VOL				0x19
#define IR_CODE_EQ					0x0D

#define IR_CODE_BACK				0x0C
#define IR_CODE_SEL					0x18
#define IR_CODE_NEXT				0x5E

#define IR_CODE_AMS					0x08
#define IR_CODE_SUB_VOL				0x1C
#define IR_CODE_BAND				0x5A

#define IR_CODE_STOP				0x42
#define IR_CODE_ST					0x52
#define IR_CODE_CLK					0x4A

#define IR_CODE_ADD_VOL1			0x4F
#define IR_CODE_BACK1				0x56
#define IR_CODE_SEL1				0x57
#define IR_CODE_NEXT1				0x59

#define IR_CODE_SUB_VOL1			0x55

#define IR_CODE_MODE1				0x54
#define IR_CODE_MENU				0x53
#define IR_CODE_BAND1				0x5B

#define IR_CODE_ANSWER				0x51
#define IR_CODE_REJECT 				0x5F

#define IR_CODE_MUTE1				0x41

/**************************************************/
/*********************END**************************/
/**************************************************/
#define IR_CODE_LOC				IR_INVALIDATE

#define IR_MAP_TAB										\
	{IR_CODE_POWER,			IR_VALUE_POWER},			\
	{IR_CODE_STOP,			IR_VALUE_STOP},				\
	{IR_CODE_MUTE,			IR_VALUE_MUTE},				\
	{IR_CODE_BAND,			IR_VALUE_BAND},				\
	{IR_CODE_LOUD,			IR_VALUE_LOUD},				\
	{IR_CODE_CLK,			IR_VALUE_CLK},				\
	{IR_CODE_MODE,			IR_VALUE_MODE},				\
	{IR_CODE_AMS,			IR_VALUE_AMS},				\
	{IR_CODE_LOC,			IR_VALUE_LOC},				\
	{IR_CODE_ST,			IR_VALUE_ST},				\
	{IR_CODE_ADD_VOL,		IR_VALUE_ADD_VOL},			\
	{IR_CODE_BACK,			IR_VALUE_BACK},				\
	{IR_CODE_SEL,			IR_VALUE_SEL},				\
	{IR_CODE_NEXT,			IR_VALUE_NEXT},				\
	{IR_CODE_EQ,			IR_VALUE_EQ},				\
	{IR_CODE_SUB_VOL,		IR_VALUE_SUB_VOL},			\
	{IR_CODE_M1,			IR_VALUE_M1},				\
	{IR_CODE_M2,			IR_VALUE_M2},				\
	{IR_CODE_M3,			IR_VALUE_M3},				\
	{IR_CODE_M4,			IR_VALUE_M4},				\
	{IR_CODE_M5,			IR_VALUE_M5},				\
	{IR_CODE_M6,			IR_VALUE_M6},				\
	{IR_CODE_MUTE1,			IR_VALUE_MUTE1},			\
	{IR_CODE_BAND1,			IR_VALUE_BAND1},			\
	{IR_CODE_MODE1,			IR_VALUE_MODE1},			\
	{IR_CODE_ADD_VOL1,		IR_VALUE_ADD_VOL1},			\
	{IR_CODE_BACK1,			IR_VALUE_BACK1},			\
	{IR_CODE_SEL1,			IR_VALUE_SEL1},				\
	{IR_CODE_NEXT1,			IR_VALUE_NEXT1},			\	
	{IR_CODE_SUB_VOL1,		IR_VALUE_SUB_VOL1},			\
	{IR_CODE_REJECT,		IR_VALUE_REJECT},			\
	{IR_CODE_ANSWER,		IR_VALUE_ANSWER},			\
	{IR_CODE_MENU,			IR_VALUE_MENU},				\

/**************************************************/
/*********************END**************************/
/**************************************************/
#define IR_USER_CODE1				0x7F80
#define IR1_INVALIDATE				0xF0
/**************************************************/
/**************************************************/
/**************************************************/

enum{
	IR_VALUE1_NULL = 70,
	IR_VALUE1_POWER,
	IR_VALUE1_MUTE,
	IR_VALUE1_POWER_MUTE,
	IR_VALUE1_PLAY,
	IR_VALUE1_EQ_LOUD,
	IR_VALUE1_MODE,
	IR_VALUE1_BAND,
	IR_VALUE1_BAND_AMS,
	IR_VALUE1_CLEAR,
	IR_VALUE1_REJECT,
	IR_VALUE1_UP,
	IR_VALUE1_ANSWER,
	IR_VALUE1_LEFT,
	IR_VALUE1_ENTER,
	IR_VALUE1_RIGHT,
	IR_VALUE1_BACK,
	IR_VALUE1_DOWN,
	IR_VALUE1_NEXT,
	IR_VALUE1_M1,
	IR_VALUE1_M2,
	IR_VALUE1_M3,
	IR_VALUE1_M4,
	IR_VALUE1_M5,
	IR_VALUE1_M6,
	IR_VALUE1_M7,
	IR_VALUE1_ADD,
	IR_VALUE1_M8	,
	IR_VALUE1_M9,
	IR_VALUE1_SUB,
	IR_VALUE1_M0	,
	IR_VALUE1_START,
	IR_VALUE1_WELL,
	IR_VALUE1_CLK,
	IR_VALUE1_SEL,
	IR_VALUE1_AMS,
};

#if defined(DEBUG_IR1_CODE)
#define IR1_CODE_POWER				0X1C
#define IR1_CODE_MUTE				0x41

#define IR1_CODE_ADD				0x4F
#define IR1_CODE_BACK				0x56
#define IR1_CODE_SEL				0x57
#define IR1_CODE_SPEED				0x59
#define IR1_CODE_SUB				0x55

#define IR1_CODE_MODE 				0x54
#define IR1_CODE_BAND_AMS			0x5B

#define IR1_CODE_PLAY 				0x02

#define IR1_CODE_ANSWER				0x51
#define IR1_CODE_REJECT 			0x5F

#else
#define IR1_CODE_POWER				0X1A
#define IR1_CODE_MUTE				0x01

#define IR1_CODE_ADD				0x02
#define IR1_CODE_BACK				0x1E
#define IR1_CODE_SEL				0x03
#define IR1_CODE_SPEED				0x00
#define IR1_CODE_SUB				0x0D

#define IR1_CODE_MODE 				0x12
#define IR1_CODE_BAND_AMS			0x04

#define IR1_CODE_PLAY 				0x05

#define IR1_CODE_ANSWER				0x0A
#define IR1_CODE_REJECT 			0x1B
#endif

#define IR1_CODE_M1					IR_INVALIDATE
#define IR1_CODE_M2					IR_INVALIDATE
#define IR1_CODE_M3					IR_INVALIDATE
#define IR1_CODE_M4					IR_INVALIDATE
#define IR1_CODE_M5					IR_INVALIDATE
#define IR1_CODE_M6					IR_INVALIDATE
#define IR1_CODE_POWER_MUTE			IR_INVALIDATE
#define IR1_CODE_AMS				IR_INVALIDATE
#define IR1_CODE_CLK				IR_INVALIDATE

/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR_MAP_TAB1										\
	{IR1_CODE_POWER,		IR_VALUE1_POWER},			\
	{IR1_CODE_MUTE,			IR_VALUE1_MUTE},			\
	{IR1_CODE_POWER_MUTE,	IR_VALUE1_POWER_MUTE},		\
	{IR1_CODE_PLAY,			IR_VALUE1_PLAY},			\
	{IR1_CODE_MODE,			IR_VALUE1_MODE},			\
	{IR1_CODE_BAND_AMS,		IR_VALUE1_BAND_AMS},		\
	{IR1_CODE_REJECT,		IR_VALUE1_REJECT},			\
	{IR1_CODE_ANSWER,		IR_VALUE1_ANSWER},			\
	{IR1_CODE_BACK,			IR_VALUE1_BACK},			\
	{IR1_CODE_SPEED,		IR_VALUE1_NEXT},			\
	{IR1_CODE_M1,			IR_VALUE1_M1},				\
	{IR1_CODE_M2,			IR_VALUE1_M2},				\
	{IR1_CODE_M3,			IR_VALUE1_M3},				\
	{IR1_CODE_M4,			IR_VALUE1_M4},				\
	{IR1_CODE_M5,			IR_VALUE1_M5},				\
	{IR1_CODE_M6,			IR_VALUE1_M6},				\
	{IR1_CODE_ADD,			IR_VALUE1_ADD},				\
	{IR1_CODE_SUB,			IR_VALUE1_SUB},				\
	{IR1_CODE_CLK,			IR_VALUE1_CLK},				\
	{IR1_CODE_SEL,			IR_VALUE1_SEL}, 			\
	{IR1_CODE_AMS,			IR_VALUE1_AMS}, 			\

/**************************************************/
/**************************************************/
/**************************************************/

#endif /* _IR_MAP_H_ */





