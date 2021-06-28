#ifndef _IR_MAP_H_
#define _IR_MAP_H_


/**************************************************/
/**************************************************/
/**************************************************/
#define IR_USER_CODE				0xF708
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
	IR_VALUE_ADD10,
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
	IR_VALUE_SEEK_DOWN,
	IR_VALUE_SEEK_UP,
	IR_VALUE_END,
};


#define IR_CODE_POWER			0x0B
#define IR_CODE_MODE			0x0C
#define IR_CODE_PLAY 			0x54
#define IR_CODE_MUTE 			0x48

#define IR_CODE_BAND 			0x41
#define IR_CODE_UP 				0x01
#define IR_CODE_TITLE 			0x02
#define IR_CODE_SUB_T 			0x03

#define IR_CODE_LEFT 			0x4A
#define IR_CODE_ENTER			0x04
#define IR_CODE_RIGHT 			0x05
#define IR_CODE_SETUP			0x06

#define IR_CODE_STOP			0x14
#define IR_CODE_DOWN 			0x07
#define IR_CODE_ANGLE 			0x08
#define IR_CODE_SLOW			0x09

#define IR_CODE_AMS 			0x15
#define IR_CODE_ST 				0x51
#define IR_CODE_ADD_VOL 		0x52
#define IR_CODE_ZOOM 			0x00

#define IR_CODE_LOC				0x1F
#define IR_CODE_BACK			0x1C
#define IR_CODE_SEL				0x1D
#define IR_CODE_NEXT			0x53

#define IR_CODE_PBC 			0x1A
#define IR_CODE_OSD				0x59
#define IR_CODE_SUB_VOL			0x40
#define IR_CODE_AUDIO			0x16

#define IR_CODE_M1 				0x46
#define IR_CODE_M2 				0x19
#define IR_CODE_M3 				0x0D
#define IR_CODE_M4 				0x4B
					   	
#define IR_CODE_M5 				0x5F
#define IR_CODE_M6 				0x5B
#define IR_CODE_M7 				0x4F
#define IR_CODE_M8 				0x57
          
#define IR_CODE_M9 				0x5E
#define IR_CODE_M0 				0x5A
#define IR_CODE_ADD10 			0x4E
#define IR_CODE_GOTO 			0x56
                     
/**************************************************/
/*********************END**************************/
/**************************************************/

#define IR_MAP_TAB										\
	{IR_CODE_POWER,			IR_VALUE_POWER},			\
	{IR_CODE_MODE,			IR_VALUE_MODE},				\
	{IR_CODE_PLAY,			IR_VALUE_PLAY},				\
	{IR_CODE_BAND,			IR_VALUE_BAND},				\
	{IR_CODE_MUTE,			IR_VALUE_MUTE},				\
	{IR_CODE_UP,			IR_VALUE_UP},				\
	{IR_CODE_TITLE,			IR_VALUE_TITLE},			\
	{IR_CODE_SUB_T,			IR_VALUE_SUB_T},			\
	{IR_CODE_LEFT,			IR_VALUE_LEFT},				\
	{IR_CODE_ENTER,			IR_VALUE_ENTER},			\
	{IR_CODE_RIGHT,			IR_VALUE_RIGHT},			\
	{IR_CODE_SETUP,			IR_VALUE_SETUP},			\
	{IR_CODE_STOP,			IR_VALUE_STOP},				\
	{IR_CODE_DOWN,			IR_VALUE_DOWN},				\
	{IR_CODE_ANGLE,			IR_VALUE_ANGLE},			\
	{IR_CODE_SLOW,			IR_VALUE_SLOW},				\
	{IR_CODE_AMS,			IR_VALUE_AMS},				\
	{IR_CODE_ST,			IR_VALUE_ST},				\
	{IR_CODE_ADD_VOL,		IR_VALUE_ADD_VOL},			\
	{IR_CODE_ZOOM,			IR_VALUE_ZOOM},				\
	{IR_CODE_LOC,			IR_VALUE_LOC},				\
	{IR_CODE_BACK,			IR_VALUE_BACK},				\
	{IR_CODE_SEL,			IR_VALUE_SEL},				\
	{IR_CODE_NEXT,			IR_VALUE_NEXT},				\
	{IR_CODE_PBC,			IR_VALUE_PBC},				\
	{IR_CODE_OSD,			IR_VALUE_OSD},				\
	{IR_CODE_SUB_VOL,		IR_VALUE_SUB_VOL},			\
	{IR_CODE_AUDIO,			IR_VALUE_AUDIO},			\
	{IR_CODE_M1,			IR_VALUE_M1},				\
	{IR_CODE_M2,			IR_VALUE_M2},				\
	{IR_CODE_M3,			IR_VALUE_M3},				\
	{IR_CODE_M4,			IR_VALUE_M4},				\
	{IR_CODE_M5,			IR_VALUE_M5},				\
	{IR_CODE_M6,			IR_VALUE_M6},				\
	{IR_CODE_M7,			IR_VALUE_M7},				\
	{IR_CODE_M8,			IR_VALUE_M8},				\
	{IR_CODE_M9,			IR_VALUE_M9},				\
	{IR_CODE_M0,			IR_VALUE_M0},				\
	{IR_CODE_ADD10,			IR_VALUE_ADD10},			\
	{IR_CODE_GOTO,			IR_VALUE_GOTO},				\

#endif /* _IR_MAP_H_ */


