#ifndef _IR_MAP_H_
#define _IR_MAP_H_

/**************************************************/
/**************************************************/
/**************************************************/
#define IR_USER_CODE1				0xF708
#define IR_INVALIDATE				0xF0
/**************************************************/
/**************************************************/
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
	IR_VALUE1_GOTO,
};


#define IR1_CODE_POWER			0x0B			
#define IR1_CODE_PLAY			0x47
#define IR1_CODE_STOP			0x54
#define IR1_CODE_MUTE			0x42

#define IR1_CODE_BAND			0x49
#define IR1_CODE_UP				0x4E
#define IR1_CODE_CLK			0x41
#define IR1_CODE_MODE			0x4D

#define IR1_CODE_LEFT			0x0D
#define IR1_CODE_ENTER			0x56
#define IR1_CODE_RIGHT			0x16
#define IR1_CODE_AMS			0x4B

#define IR1_CODE_LOC			0x08
#define IR1_CODE_DOWN			0x11
#define IR1_CODE_ST				0x48
#define IR1_CODE_SLOW			0x43

#define IR1_CODE_SETUP			0x09
#define IR1_CODE_GOTO			0x0C
#define IR1_CODE_ADD_VOL		0x55
#define IR1_CODE_FORWARD		0x17

#define IR1_CODE_SUB_T			0x0F
#define IR1_CODE_BACK			0x10
#define IR1_CODE_SEL			0x4C
#define IR1_CODE_NEXT			0x57

#define IR1_CODE_RPT			0x19
#define IR1_CODE_EQ				0x03
#define IR1_CODE_SUB_VOL		0x40
#define IR1_CODE_BACKWARD		0x52

#define IR1_CODE_M1				0x4F
#define IR1_CODE_M2				0x44
#define IR1_CODE_M3				0x50
#define IR1_CODE_M4				0x0E

#define IR1_CODE_M5				0x13
#define IR1_CODE_M6				0x12
#define IR1_CODE_M7				0x15
#define IR1_CODE_M8				0x53

#define IR1_CODE_M9				0x45
#define IR1_CODE_M0				0x14
#define IR1_CODE_M10			0x51
#define IR1_CODE_MENU			0x18

/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR_MAP_TAB1										\
	{IR1_CODE_POWER,		IR_VALUE1_POWER},			\
	{IR1_CODE_PLAY,			IR_VALUE1_PLAY},			\
	{IR1_CODE_STOP,			IR_VALUE1_STOP},			\
	{IR1_CODE_MUTE,			IR_VALUE1_MUTE},			\
	{IR1_CODE_BAND,			IR_VALUE1_BAND},			\
	{IR1_CODE_UP,			IR_VALUE1_UP},				\
	{IR1_CODE_CLK,			IR_VALUE1_CLK},				\
	{IR1_CODE_MODE,			IR_VALUE1_MODE},			\
	{IR1_CODE_LEFT,			IR_VALUE1_LEFT},			\
	{IR1_CODE_ENTER,		IR_VALUE1_ENTER},			\
	{IR1_CODE_RIGHT,		IR_VALUE1_RIGHT},			\
	{IR1_CODE_AMS,			IR_VALUE1_AMS},				\
	{IR1_CODE_LOC,			IR_VALUE1_LOC},				\
	{IR1_CODE_DOWN,			IR_VALUE1_DOWN},			\
	{IR1_CODE_ST,			IR_VALUE1_ST},				\
	{IR1_CODE_SLOW,			IR_VALUE1_SLOW},			\
	{IR1_CODE_SETUP,		IR_VALUE1_SETUP},			\
	{IR1_CODE_GOTO,			IR_VALUE1_GOTO},			\
	{IR1_CODE_ADD_VOL,		IR_VALUE1_ADD_VOL},			\
	{IR1_CODE_FORWARD,		IR_VALUE1_FORWARD},			\
	{IR1_CODE_SUB_T,		IR_VALUE1_SUB_T},			\
	{IR1_CODE_BACK,			IR_VALUE1_BACK},			\
	{IR1_CODE_SEL,			IR_VALUE1_SEL},				\
	{IR1_CODE_NEXT,			IR_VALUE1_NEXT},			\
	{IR1_CODE_RPT,			IR_VALUE1_RPT},				\
	{IR1_CODE_EQ,			IR_VALUE1_EQ},				\
	{IR1_CODE_SUB_VOL,		IR_VALUE1_SUB_VOL},			\
	{IR1_CODE_BACKWARD,		IR_VALUE1_BACKWARD},		\
	{IR1_CODE_M1,			IR_VALUE1_M1},				\
	{IR1_CODE_M2,			IR_VALUE1_M2},				\
	{IR1_CODE_M3,			IR_VALUE1_M3},				\
	{IR1_CODE_M4,			IR_VALUE1_M4},				\
	{IR1_CODE_M5,			IR_VALUE1_M5},				\
	{IR1_CODE_M6,			IR_VALUE1_M6},				\
	{IR1_CODE_M7,			IR_VALUE1_M7},				\
	{IR1_CODE_M8,			IR_VALUE1_M8},				\
	{IR1_CODE_M9,			IR_VALUE1_M9},				\
	{IR1_CODE_M0,			IR_VALUE1_M0},				\
	{IR1_CODE_M10,			IR_VALUE1_M10},				\
	{IR1_CODE_MENU,			IR_VALUE1_MENU},			\


/**************************************************/
/**************************************************/
/***************19-08205-00************************/
#define IR_USER_CODE2				0xFE01
/**************************************************/
/**************************************************/
/**************************************************/

enum{
	IR_VALUE2_INVALIDATE= 70,
	IR_VALUE2_POWER,			
	IR_VALUE2_POWER_MUTE,
	IR_VALUE2_PLAY,
	IR_VALUE2_EQ_LOUD,		
	IR_VALUE2_MODE, 			
	IR_VALUE2_CLEAR,			
	IR_VALUE2_REJECT,
	IR_VALUE2_UP,			
	IR_VALUE2_ANSWER,			
	IR_VALUE2_LEFT, 
	IR_VALUE2_ENTER,
	IR_VALUE2_RIGHT,
	IR_VALUE2_BACK, 
	IR_VALUE2_DOWN, 
	IR_VALUE2_NEXT, 
	IR_VALUE2_M1,			
	IR_VALUE2_M2,			
	IR_VALUE2_M3,			
	IR_VALUE2_M4,			
	IR_VALUE2_M5,			
	IR_VALUE2_M6,			
	IR_VALUE2_M7,			
	IR_VALUE2_ADD,
	IR_VALUE2_M8	,		
	IR_VALUE2_M9,			
	IR_VALUE2_SUB,
	IR_VALUE2_M0	,		
	IR_VALUE2_START,
	IR_VALUE2_WELL,	
	IR_VALUE2_CLK,
	IR_VALUE2_SEL,
	IR_VALUE2_AMS,
};


#define IR2_CODE_POWER				IR_VALUE2_INVALIDATE
#define IR2_CODE_POWER_MUTE			0x0B
#define IR2_CODE_PLAY 				0x47

#define IR2_CODE_EQ_LOUD 			0x0C
#define IR2_CODE_MODE 				0x4D
#define IR2_CODE_CLEAR 				0x53

#define IR2_CODE_REJECT 			0x41
#define IR2_CODE_ANSWER				0x43

#define IR2_CODE_UP					0x08
#define IR2_CODE_LEFT				0x10
#define IR2_CODE_RIGHT				0x57
#define IR2_CODE_DOWN				0x14
#define IR2_CODE_ENTER				0x4B

//#define IR_POWER_MUTE			IR2_POWER_MUTE

#define IR2_CODE_BACK				0x52
#define IR2_CODE_SPEED				0x17

#define IR2_CODE_M1					0x09
#define IR2_CODE_M2					0x05
#define IR2_CODE_M3					0x19

#define IR2_CODE_M4					0x48
#define IR2_CODE_M5					0x4E
#define IR2_CODE_M6					0x18

#define IR2_CODE_M7					0x0D
#define IR2_CODE_M8					0x56
#define IR2_CODE_M9 				0x16

#define IR2_CODE_STAR				0x49
#define IR2_CODE_M0					0x11
#define IR2_CODE_WELL				0x15

#define IR2_CODE_ADD				0x55
#define IR2_CODE_SUB				0x40

#if defined(USE_FY_ADD_3KEY)  //新遥控多了几个按键如下
#define IR2_CODE_CLK				0x01
#define IR2_CODE_SEL				0x54
#define IR2_CODE_AMS				0x42
#endif

/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR_MAP_TAB2										\
	{IR2_CODE_POWER,		IR_VALUE2_POWER},			\
	{IR2_CODE_POWER_MUTE,	IR_VALUE2_POWER_MUTE},		\
	{IR2_CODE_PLAY,			IR_VALUE2_PLAY},			\
	{IR2_CODE_EQ_LOUD,		IR_VALUE2_EQ_LOUD},			\
	{IR2_CODE_MODE,			IR_VALUE2_MODE},			\
	{IR2_CODE_CLEAR,		IR_VALUE2_CLEAR},			\
	{IR2_CODE_REJECT,		IR_VALUE2_REJECT},			\
	{IR2_CODE_UP,			IR_VALUE2_UP},				\
	{IR2_CODE_ANSWER,		IR_VALUE2_ANSWER},			\
	{IR2_CODE_LEFT,			IR_VALUE2_LEFT},			\
	{IR2_CODE_ENTER,		IR_VALUE2_ENTER},			\
	{IR2_CODE_RIGHT,		IR_VALUE2_RIGHT},			\
	{IR2_CODE_BACK,			IR_VALUE2_BACK},			\
	{IR2_CODE_DOWN,			IR_VALUE2_DOWN},			\
	{IR2_CODE_SPEED,		IR_VALUE2_NEXT},			\
	{IR2_CODE_M1,			IR_VALUE2_M1},				\
	{IR2_CODE_M2,			IR_VALUE2_M2},				\
	{IR2_CODE_M3,			IR_VALUE2_M3},				\
	{IR2_CODE_M4,			IR_VALUE2_M4},				\
	{IR2_CODE_M5,			IR_VALUE2_M5},				\
	{IR2_CODE_M6,			IR_VALUE2_M6},				\
	{IR2_CODE_M7,			IR_VALUE2_M7},				\
	{IR2_CODE_ADD,			IR_VALUE2_ADD},				\
	{IR2_CODE_M8,			IR_VALUE2_M8},				\
	{IR2_CODE_M9,			IR_VALUE2_M9},				\
	{IR2_CODE_SUB,			IR_VALUE2_SUB},				\
	{IR2_CODE_M0,			IR_VALUE2_M0},				\
	{IR2_CODE_STAR,			IR_VALUE2_START},			\
	{IR2_CODE_WELL,			IR_VALUE2_WELL},			\
	{IR2_CODE_CLK,			IR_VALUE2_CLK},				\
	{IR2_CODE_SEL,			IR_VALUE2_SEL}, 			\
	{IR2_CODE_AMS,			IR_VALUE2_AMS}, 			\
//#endif

/**************************************************/
/**************************************************/
/**************************************************/
#define IR_USER_CODE3				0xFF00
/**************************************************/
/**************************************************/
/**************************************************/
#if !defined(USE_FY_ANOTHER_REMOTE)
enum{
	IR_VALUE3_INVALIDATE= 120,
	IR_VALUE3_POWER_MUTE,
	IR_VALUE3_MODE, 	
	IR_VALUE3_BAND, 
	IR_VALUE3_ADD,
	IR_VALUE3_UP,			
	IR_VALUE3_AMS,			
	IR_VALUE3_LEFT, 
	IR_VALUE3_ENTER,
	IR_VALUE3_RIGHT,
	IR_VALUE3_SUB,
	IR_VALUE3_DOWN, 
	IR_VALUE3_M0,		
	IR_VALUE3_M1,			
	IR_VALUE3_M2,			
	IR_VALUE3_M3,			
	IR_VALUE3_M4,			
	IR_VALUE3_M5,			
	IR_VALUE3_M6,			
	IR_VALUE3_M7,			
	IR_VALUE3_SUB_T,			
	IR_VALUE3_GOTO,			
};


#define IR3_CODE_POWER_MUTE			0x45
#define IR3_CODE_MODE				0x46
#define IR3_CODE_BAND				0x47

#define IR3_CODE_ADD 				0x44
#define IR3_CODE_UP					0x40
#define IR3_CODE_AMS 				0x43

#define IR3_CODE_LEFT				0x07
#define IR3_CODE_ENTER				0x15
#define IR3_CODE_RIGHT				0x09


#define IR3_CODE_SUB				0x16
#define IR3_CODE_DOWN				0x19
#define IR3_CODE_M0					0x0D


#define IR3_CODE_M1					0x0C
#define IR3_CODE_M2					0x18
#define IR3_CODE_M3					0x5E

#define IR3_CODE_M4					0x08
#define IR3_CODE_M5					0x1C
#define IR3_CODE_M6					0x5A

#define IR3_CODE_M7					0x42
#define IR3_CODE_SUB_T				0x52
#define IR3_CODE_GOTO 				0x4A

/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR_MAP_TAB3										\
	{IR3_CODE_POWER_MUTE,	IR_VALUE3_POWER_MUTE},		\
	{IR3_CODE_MODE,			IR_VALUE3_MODE},			\
	{IR3_CODE_BAND,			IR_VALUE3_BAND},			\
	{IR3_CODE_ADD,			IR_VALUE3_ADD},				\
	{IR3_CODE_UP,			IR_VALUE3_UP},				\
	{IR3_CODE_AMS,			IR_VALUE3_AMS},				\
	{IR3_CODE_LEFT,			IR_VALUE3_LEFT},			\
	{IR3_CODE_ENTER,		IR_VALUE3_ENTER},			\
	{IR3_CODE_RIGHT,		IR_VALUE3_RIGHT},			\
	{IR3_CODE_SUB,			IR_VALUE3_SUB},				\
	{IR3_CODE_DOWN,			IR_VALUE3_DOWN},			\
	{IR3_CODE_M0,			IR_VALUE3_M0},				\
	{IR3_CODE_M1,			IR_VALUE3_M1},				\
	{IR3_CODE_M2,			IR_VALUE3_M2},				\
	{IR3_CODE_M3,			IR_VALUE3_M3},				\
	{IR3_CODE_M4,			IR_VALUE3_M4},				\
	{IR3_CODE_M5,			IR_VALUE3_M5},				\
	{IR3_CODE_M6,			IR_VALUE3_M6},				\
	{IR3_CODE_M7,			IR_VALUE3_M7},				\
	{IR3_CODE_SUB_T,		IR_VALUE3_SUB_T},			\
	{IR3_CODE_GOTO,			IR_VALUE3_GOTO},			\

#else
#define IR3_CODE_POWER 				0x45
#define IR3_CODE_MODE				0x46
#define IR3_CODE_MUTE				0x47
          
#define IR3_CODE_BAND				0x44
#define IR3_CODE_ADD_VOL			0x40
#define IR3_CODE_AMS				0x43
          
#define IR3_CODE_BACK				0x07
#define IR3_CODE_SEL				0x15
#define IR3_CODE_NEXT				0x09
          
#define IR3_CODE_EQ					0x16
#define IR3_CODE_SUB_VOL			0x19
#define IR3_CODE_CLK				0x0D
          
#define IR3_CODE_M1					0x0C
#define IR3_CODE_M2					0x18
#define IR3_CODE_M3					0x5E
          
#define IR3_CODE_M4					0x08
#define IR3_CODE_M5					0x1C
#define IR3_CODE_M6					0x5A
          
#define IR3_CODE_M7					0x42
#define IR3_CODE_START				0x52
#define IR3_CODE_WELL				0x4A


/**************************************************/
/*********************TAB**************************/
/**************************************************/

enum{
	IR_VALUE3_INVALIDATE= 120,
	IR_VALUE3_POWER,
	IR_VALUE3_MODE,
	IR_VALUE3_PLAY,
	IR_VALUE3_STOP,
	IR_VALUE3_MUTE,
	IR_VALUE3_BAND,
	IR_VALUE3_SCAN,
	IR_VALUE3_AMS,
	IR_VALUE3_ST,
	IR_VALUE3_LOC,
	IR_VALUE3_LOUD,
	IR_VALUE3_EQ,
	IR_VALUE3_CLK,
	IR_VALUE3_EJECT,
	IR_VALUE3_M0,
	IR_VALUE3_M1,
	IR_VALUE3_M2,
	IR_VALUE3_M3,
	IR_VALUE3_M4,
	IR_VALUE3_M5,
	IR_VALUE3_M6,
	IR_VALUE3_M7,
	IR_VALUE3_M8,
	IR_VALUE3_M9,
	IR_VALUE3_M10,
	IR_VALUE3_LEFT,
	IR_VALUE3_RIGHT,
	IR_VALUE3_UP,
	IR_VALUE3_DOWN,
	IR_VALUE3_ENTER,
	IR_VALUE3_RETURN,
	IR_VALUE3_HOME,
	IR_VALUE3_BACK,
	IR_VALUE3_NEXT,
	IR_VALUE3_FORWARD,
	IR_VALUE3_BACKWARD,
	IR_VALUE3_ADD_VOL,
	IR_VALUE3_SUB_VOL,
	IR_VALUE3_SEL,
	IR_VALUE3_MENU,
	IR_VALUE3_SETUP,
	IR_VALUE3_TITLE,
	IR_VALUE3_SUB_T,
	IR_VALUE3_ANGLE,
	IR_VALUE3_RPT,
	IR_VALUE3_SLOW,
	IR_VALUE3_ZOOM,
	IR_VALUE3_PBC,
	IR_VALUE3_OSD,
	IR_VALUE3_AUDIO,
	IR_VALUE3_GOTO,
	IR_VALUE3_START,
	IR_VALUE3_WELL,
};


#define IR_MAP_TAB3										\
	{IR3_CODE_M1,			IR_VALUE3_M1},				\												
	{IR3_CODE_M2,			IR_VALUE3_M2},				\												
	{IR3_CODE_M3,			IR_VALUE3_M3},				\												
	{IR3_CODE_M4,			IR_VALUE3_M4},				\												
	{IR3_CODE_M5,			IR_VALUE3_M5},				\												
	{IR3_CODE_M6,			IR_VALUE3_M6},				\												
	{IR3_CODE_POWER,		IR_VALUE3_POWER},			\
	{IR3_CODE_MODE,			IR_VALUE3_MODE},			\
	{IR3_CODE_MUTE,			IR_VALUE3_MUTE},			\
	{IR3_CODE_BAND,			IR_VALUE3_BAND},			\
	{IR3_CODE_AMS,			IR_VALUE3_AMS},				\
	{IR3_CODE_EQ,			IR_VALUE3_EQ},				\
	{IR3_CODE_ADD_VOL,		IR_VALUE3_ADD_VOL},			\
	{IR3_CODE_SUB_VOL,		IR_VALUE3_SUB_VOL},			\
	{IR3_CODE_SEL,			IR_VALUE3_SEL},				\
	{IR3_CODE_BACK,			IR_VALUE3_BACK},			\
	{IR3_CODE_NEXT,			IR_VALUE3_NEXT},			\
	{IR3_CODE_CLK,			IR_VALUE3_CLK},				\
	{IR3_CODE_M7,			IR_VALUE3_M7},				\
	{IR3_CODE_START,		IR_VALUE3_START},			\
	{IR3_CODE_WELL,			IR_VALUE3_WELL},			\

#endif



#endif /* _IR_MAP_H_ */





