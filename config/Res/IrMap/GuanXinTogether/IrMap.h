#ifndef _IR_MAP_H_
#define _IR_MAP_H_


/**************************************************/
/**************************************************/
/**************************************************/
#define IR_USER_CODE1				0xEF00
#define IR_INVALIDATE				0xF0
/**************************************************/
/**************************************************/
/**************************************************/

#define IR1_CODE_POWER_MUTE 			0x00
#define IR1_CODE_ADD_VOL				0x02

#define IR1_CODE_CLK					0x04
#define IR1_CODE_BACK				    0x05
#define IR1_CODE_SEL					0x06
#define IR1_CODE_NEXT				    0x07

#define IR1_CODE_SUB_VOL				0x0A

#define IR1_CODE_MODE				    0x0C
#define IR1_CODE_BAND				    0x0D
#define IR1_CODE_AMS					0x0E
#define IR1_CODE_ST					    0x0F

#define IR1_CODE_M1					    0x10
#define IR1_CODE_M2					    0x11
#define IR1_CODE_M3					    0x12
#define IR1_CODE_SCAN				    0x13

#define IR1_CODE_M4					    0x14
#define IR1_CODE_M5					    0x15
#define IR1_CODE_M6					    0x16

/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR1_CODE_EQ					IR_INVALIDATE
#define IR1_CODE_LOC				IR_INVALIDATE
#define IR1_CODE_LOUD				IR_INVALIDATE
#define IR1_CODE_MUTE				IR_INVALIDATE
#define IR1_CODE_DOOR				IR_INVALIDATE
#define IR1_CODE_PTY			    IR_INVALIDATE
#define IR1_CODE_TA					IR_INVALIDATE
#define IR1_CODE_AF					IR_INVALIDATE


/**************************************************/
/*********************TAB**************************/
/**************************************************/

enum{
	IR_VALUE1_NULL,
	IR_VALUE1_POWER,
	IR_VALUE1_MODE,
	IR_VALUE1_PLAY,
	IR_VALUE1_STOP,
	IR_VALUE1_MUTE,
	IR_VALUE1_BAND,
	IR_VALUE1_SCAN,
	IR_VALUE1_AMS,
	IR_VALUE1_ST,
	IR_VALUE1_LOC,
	IR_VALUE1_LOUD,
	IR_VALUE1_EQ,
	IR_VALUE1_CLK,
	IR_VALUE1_EJECT,
	IR_VALUE1_M0,
	IR_VALUE1_M1,
	IR_VALUE1_M2,
	IR_VALUE1_M3,
	IR_VALUE1_M4,
	IR_VALUE1_M5,
	IR_VALUE1_M6,
	IR_VALUE1_M7,
	IR_VALUE1_M8,
	IR_VALUE1_M9,
	IR_VALUE1_M10,
	IR_VALUE1_LEFT,
	IR_VALUE1_RIGHT,
	IR_VALUE1_UP,
	IR_VALUE1_DOWN,
	IR_VALUE1_ENTER,
	IR_VALUE1_RETURN,
	IR_VALUE1_HOME,
	IR_VALUE1_BACK,
	IR_VALUE1_NEXT,
	IR_VALUE1_FORWARD,
	IR_VALUE1_BACKWARD,
	IR_VALUE1_ADD_VOL,
	IR_VALUE1_SUB_VOL,
	IR_VALUE1_SEL,
	IR_VALUE1_MENU,
	IR_VALUE1_SETUP,
	IR_VALUE1_TITLE,
	IR_VALUE1_SUB_T,
	IR_VALUE1_ANGLE,
	IR_VALUE1_RPT,
	IR_VALUE1_SLOW,
	IR_VALUE1_ZOOM,
	IR_VALUE1_PBC,
	IR_VALUE1_OSD,
	IR_VALUE1_AUDIO,
	IR_VALUE1_POWER_MUTE,
};


#define IR_MAP_TAB1										\
	{IR1_CODE_M1,			IR_VALUE1_M1},				\
	{IR1_CODE_M2,			IR_VALUE1_M2},				\
	{IR1_CODE_M3,			IR_VALUE1_M3},				\
	{IR1_CODE_M4,			IR_VALUE1_M4},				\
	{IR1_CODE_M5,			IR_VALUE1_M5},				\
	{IR1_CODE_M6,			IR_VALUE1_M6},				\
	{IR1_CODE_POWER_MUTE,	IR_VALUE1_POWER_MUTE},		\
	{IR1_CODE_MODE,			IR_VALUE1_MODE},				\
	{IR1_CODE_BAND,			IR_VALUE1_BAND},				\
	{IR1_CODE_AMS,			IR_VALUE1_AMS},				\
	{IR1_CODE_ST,			IR_VALUE1_ST},				\
	{IR1_CODE_ADD_VOL,		IR_VALUE1_ADD_VOL},			\
	{IR1_CODE_SUB_VOL,		IR_VALUE1_SUB_VOL},			\
	{IR1_CODE_SEL,			IR_VALUE1_SEL},				\
	{IR1_CODE_BACK,			IR_VALUE1_BACK},				\
	{IR1_CODE_NEXT,			IR_VALUE1_NEXT},				\
	{IR1_CODE_CLK,			IR_VALUE1_CLK},				\
	{IR1_CODE_SCAN,			IR_VALUE1_SCAN},				\


#define IR_POWER_MUTE		    IR_VALUE1_POWER_MUTE

/**************************************************/
/**************************************************/
/**************************************************/
#define IR_USER_CODE2				0xFD02

/**************************************************/
/**************************************************/
/**************************************************/

#define IR2_CODE_POWER 				0x00
#define IR2_CODE_PAUSE				0x01
#define IR2_CODE_MODE				0x02

#define IR2_CODE_EQ					0x04
#define IR2_CODE_SUB_VOL			0x05
#define IR2_CODE_ADD_VOL			0x06

#define IR2_CODE_M0					0x08
#define IR2_CODE_BACK				0x09
#define IR2_CODE_NEXT				0x0A

#define IR2_CODE_M1					0x0C
#define IR2_CODE_M2					0x0D
#define IR2_CODE_M3					0x0E

#define IR2_CODE_M4					0x10
#define IR2_CODE_M5					0x11
#define IR2_CODE_M6					0x12

#define IR2_CODE_M7					0x14
#define IR2_CODE_M8					0x15
#define IR2_CODE_M9					0x16

/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR2_CODE_BAND				IR_INVALIDATE
#define IR2_CODE_AMS				IR_INVALIDATE
#define IR2_CODE_SEL				IR_INVALIDATE
#define IR2_CODE_CLK				IR_INVALIDATE
#define IR2_CODE_LOC				IR_INVALIDATE
#define IR2_CODE_ST				    IR_INVALIDATE
#define IR2_CODE_SCAN				IR_INVALIDATE
#define IR2_CODE_LOUD				IR_INVALIDATE
#define IR2_CODE_MUTE				IR_INVALIDATE
#define IR2_CODE_DOOR				IR_INVALIDATE
#define IR2_CODE_PTY				IR_INVALIDATE
#define IR2_CODE_TA					IR_INVALIDATE
#define IR2_CODE_AF					IR_INVALIDATE


/**************************************************/
/*********************TAB**************************/
/**************************************************/

enum{
	IR_VALUE2_INVALIDATE= 70,
	IR_VALUE2_POWER,
	IR_VALUE2_MODE,
	IR_VALUE2_PLAY,
	IR_VALUE2_STOP,
	IR_VALUE2_MUTE,
	IR_VALUE2_BAND,
	IR_VALUE2_SCAN,
	IR_VALUE2_AMS,
	IR_VALUE2_ST,
	IR_VALUE2_LOC,
	IR_VALUE2_LOUD,
	IR_VALUE2_EQ,
	IR_VALUE2_CLK,
	IR_VALUE2_EJECT,
	IR_VALUE2_M0,
	IR_VALUE2_M1,
	IR_VALUE2_M2,
	IR_VALUE2_M3,
	IR_VALUE2_M4,
	IR_VALUE2_M5,
	IR_VALUE2_M6,
	IR_VALUE2_M7,
	IR_VALUE2_M8,
	IR_VALUE2_M9,
	IR_VALUE2_M10,
	IR_VALUE2_LEFT,
	IR_VALUE2_RIGHT,
	IR_VALUE2_UP,
	IR_VALUE2_DOWN,
	IR_VALUE2_ENTER,
	IR_VALUE2_RETURN,
	IR_VALUE2_HOME,
	IR_VALUE2_BACK,
	IR_VALUE2_NEXT,
	IR_VALUE2_FORWARD,
	IR_VALUE2_BACKWARD,
	IR_VALUE2_ADD_VOL,
	IR_VALUE2_SUB_VOL,
	IR_VALUE2_SEL,
	IR_VALUE2_MENU,
	IR_VALUE2_SETUP,
	IR_VALUE2_TITLE,
	IR_VALUE2_SUB_T,
	IR_VALUE2_ANGLE,
	IR_VALUE2_RPT,
	IR_VALUE2_SLOW,
	IR_VALUE2_ZOOM,
	IR_VALUE2_PBC,
	IR_VALUE2_OSD,
	IR_VALUE2_AUDIO,
	IR_VALUE2_GOTO,
	IR_VALUE2_END,
};


#define IR_MAP_TAB2										\
	{IR2_CODE_M1,			IR_VALUE2_M1},				\
	{IR2_CODE_M2,			IR_VALUE2_M2},				\
	{IR2_CODE_M3,			IR_VALUE2_M3},				\
	{IR2_CODE_M4,			IR_VALUE2_M4},				\
	{IR2_CODE_M5,			IR_VALUE2_M5},				\
	{IR2_CODE_M6,			IR_VALUE2_M6},				\
	{IR2_CODE_M7,			IR_VALUE2_M7},				\
	{IR2_CODE_M8,			IR_VALUE2_M8},				\
	{IR2_CODE_M9,			IR_VALUE2_M9},				\
	{IR2_CODE_M0,			IR_VALUE2_M0},				\
	{IR2_CODE_POWER,		IR_VALUE2_POWER},			\
	{IR2_CODE_MODE,			IR_VALUE2_MODE},				\
	{IR2_CODE_MUTE,			IR_VALUE2_MUTE},				\
	{IR2_CODE_BAND,			IR_VALUE2_BAND},				\
	{IR2_CODE_AMS,			IR_VALUE2_AMS},				\
	{IR2_CODE_LOC,			IR_VALUE2_LOC},				\
	{IR2_CODE_PAUSE,		IR_VALUE2_PLAY},				\
	{IR2_CODE_EQ,			IR_VALUE2_EQ},				\
	{IR2_CODE_ADD_VOL,		IR_VALUE2_ADD_VOL},			\
	{IR2_CODE_SUB_VOL,		IR_VALUE2_SUB_VOL},			\
	{IR2_CODE_SEL,			IR_VALUE2_SEL},				\
	{IR2_CODE_BACK,			IR_VALUE2_BACK},				\
	{IR2_CODE_NEXT,			IR_VALUE2_NEXT},				\
	{IR2_CODE_CLK,			IR_VALUE2_CLK},				\
		

/**************************************************/
/**************************************************/
/**************************************************/
#define IR_USER_CODE3				0xFF00

/**************************************************/
/**************************************************/
/**************************************************/

#define IR3_CODE_POWER_MUTE			0x09
#define IR3_CODE_ADD_VOL			0x1F

#define IR3_CODE_CLK				0x19
#define IR3_CODE_BACK				0x1B
#define IR3_CODE_SEL				0x11
#define IR3_CODE_NEXT				0x15

#define IR3_CODE_SUB_VOL			0x16

#define IR3_CODE_MODE				0x40
#define IR3_CODE_BAND				0x4C
#define IR3_CODE_AMS 				0x04
#define IR3_CODE_ST				    0x00

#define IR3_CODE_M1					0x0A
#define IR3_CODE_M2					0x1E
#define IR3_CODE_M3					0x0E
#define IR3_CODE_SCAN				0x1A

#define IR3_CODE_M4					0x1C
#define IR3_CODE_M5					0x14
#define IR3_CODE_M6					0x0F

/**************************************************/
/*********************TAB**************************/
/**************************************************/

enum{
	IR_VALUE3_INVALIDATE= 120,
	IR_VALUE3_POWER_MUTE,
	IR_VALUE3_MODE, 	
	IR_VALUE3_BAND, 
	IR_VALUE3_ADD_VOL,
	IR_VALUE3_CLK,			
	IR_VALUE3_AMS,			
	IR_VALUE3_BACK, 
	IR_VALUE3_SEL,
	IR_VALUE3_NEXT,
	IR_VALUE3_SUB_VOL,
	IR_VALUE3_ST, 
	IR_VALUE3_SCAN,		
	IR_VALUE3_M1,			
	IR_VALUE3_M2,			
	IR_VALUE3_M3,			
	IR_VALUE3_M4,			
	IR_VALUE3_M5,			
	IR_VALUE3_M6,			
};

/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR_MAP_TAB3										\
	{IR3_CODE_POWER_MUTE,	IR_VALUE3_POWER_MUTE},		\
	{IR3_CODE_MODE,			IR_VALUE3_MODE},			\
	{IR3_CODE_BAND,			IR_VALUE3_BAND},			\
	{IR3_CODE_ADD_VOL,		IR_VALUE3_ADD_VOL},			\
	{IR3_CODE_CLK,			IR_VALUE3_CLK},				\
	{IR3_CODE_AMS,			IR_VALUE3_AMS},				\
	{IR3_CODE_BACK,			IR_VALUE3_BACK},			\
	{IR3_CODE_SEL,		    IR_VALUE3_SEL},			    \
	{IR3_CODE_NEXT,		    IR_VALUE3_NEXT},			\
	{IR3_CODE_SUB_VOL,		IR_VALUE3_SUB_VOL},			\
	{IR3_CODE_ST,			IR_VALUE3_ST},			    \
	{IR3_CODE_SCAN,			IR_VALUE3_SCAN},			\
	{IR3_CODE_M1,			IR_VALUE3_M1},				\
	{IR3_CODE_M2,			IR_VALUE3_M2},				\
	{IR3_CODE_M3,			IR_VALUE3_M3},				\
	{IR3_CODE_M4,			IR_VALUE3_M4},				\
	{IR3_CODE_M5,			IR_VALUE3_M5},				\
	{IR3_CODE_M6,			IR_VALUE3_M6},				\
	
#endif /* _IR_MAP_H_ */


