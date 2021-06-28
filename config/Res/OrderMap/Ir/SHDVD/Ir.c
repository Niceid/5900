#include "Ir.h"

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------系统功能码------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_SYSTEM_IR[]=
{
	{IR_SYSTEM_POWER,			MSG_MODE_DOWN,				MSG_SYS_POWER				},
	{IR_SYSTEM_MODE,			MSG_MODE_UP_LESS_THAN,		MSG_SYS_MODE				},
	
};

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------收音功能码------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_RADIO_IR[]=
{
	{IR_RADIO_M1,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M1					},
	{IR_RADIO_M2,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M2					},
	{IR_RADIO_M3,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M3					},
	{IR_RADIO_M4,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M4					},
	{IR_RADIO_M5,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M5					},
	{IR_RADIO_M6,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M6					},
	
	{IR_RADIO_M1,				MSG_MODE_DOWN_DELEY,		MSG_FM_S1					},
	{IR_RADIO_M2,				MSG_MODE_DOWN_DELEY,		MSG_FM_S2					},
	{IR_RADIO_M3,				MSG_MODE_DOWN_DELEY,		MSG_FM_S3					},
	{IR_RADIO_M4,				MSG_MODE_DOWN_DELEY,		MSG_FM_S4					},
	{IR_RADIO_M5,				MSG_MODE_DOWN_DELEY,		MSG_FM_S5					},
	{IR_RADIO_M6,				MSG_MODE_DOWN_DELEY,		MSG_FM_S6					},

	{IR_RADIO_BAND,				MSG_MODE_DOWN,				MSG_FM_BAND					},
	
	{IR_RADIO_BACK,				MSG_MODE_UP_LESS_THAN,		MSG_FM_PREV_SEEK			},
	{IR_RADIO_NEXT,				MSG_MODE_UP_LESS_THAN,		MSG_FM_NEXT_SEEK			},
	{IR_RADIO_BACK, 			MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_SEEK			},
	{IR_RADIO_NEXT, 			MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_SEEK			},
	
	{IR_RADIO_UP_PRESET,		MSG_MODE_DOWN,				MSG_FM_PREV_STATION			},
	{IR_RADIO_PREV_PRESET,		MSG_MODE_DOWN,				MSG_FM_PREV_STATION			},
	{IR_RADIO_DOWN_PRESET, 		MSG_MODE_DOWN,				MSG_FM_NEXT_STATION			},
	{IR_RADIO_NEXT_PRESET,		MSG_MODE_DOWN,				MSG_FM_NEXT_STATION			},

	{IR_RADIO_ST,				MSG_MODE_DOWN,				MSG_FM_ST					},
	{IR_RADIO_LOC,				MSG_MODE_DOWN,				MSG_FM_LOC					},
	{IR_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{IR_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,		MSG_FM_BROWSE				},
	
	{IR_RADIO_NAVI_UP, 			MSG_MODE_DOWN,				MSG_FM_SLECT_UP 		},
	{IR_RADIO_NAVI_DOWN,		MSG_MODE_DOWN,				MSG_FM_SLECT_DOWN 		},
};

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------播放功能码------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_PLAYER_IR[]=
{	
	{IR_PLAYER_M0,				MSG_MODE_DOWN,				MSG_MUSIC_0					},
	{IR_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{IR_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{IR_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{IR_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{IR_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
	{IR_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},	
	{IR_PLAYER_M7,				MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR_PLAYER_M8,				MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{IR_PLAYER_M9,				MSG_MODE_DOWN,				MSG_MUSIC_9					},	

	{IR_PLAYER_UP,				MSG_MODE_DOWN,				MSG_MUSIC_UP				},
	{IR_PLAYER_DOWN,			MSG_MODE_DOWN,				MSG_MUSIC_DOWN				},

	{IR_PLAYER_LEFT,			MSG_MODE_DOWN,				MSG_MUSIC_LEFT				},
	{IR_PLAYER_RIGHT,			MSG_MODE_DOWN,				MSG_MUSIC_RIGHT				},
	{IR_PLAYER_ENTER,			MSG_MODE_DOWN,				MSG_MUSIC_ENTER				},
	
	{IR_PLAYER_PLAYPAUSE,		MSG_MODE_DOWN,				MSG_MUSIC_PP				},
	{IR_PLAYER_STOP,			MSG_MODE_DOWN,				MSG_MUSIC_STOP				},
	
	{IR_PLAYER_NEXT,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FILE			},
	{IR_PLAYER_NEXT,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_FF				},
	
	{IR_PLAYER_BACK,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FILE			},
	{IR_PLAYER_BACK,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_FR				},

	{IR_PLAYER_ID3,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_ID3				},
		
	{IR_PLAYER_SCH,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SEARCH			},
		
	{IR_PLAYER_ADD10,			MSG_MODE_DOWN,				MSG_MUSIC_ADD_10			},
	
	{IR_PLAYER_CHANNEL,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_CHANNEL			},
	{IR_PLAYER_OSD,				MSG_MODE_DOWN_DELEY ,		MSG_MUSIC_OSD				},
	{IR_PLAYER_PBC,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PBC				},
	{IR_PLAYER_GOTO,			MSG_MODE_DOWN,				MSG_MUSIC_GOTO				},
	{IR_PLAYER_ZOOM,			MSG_MODE_DOWN,				MSG_MUSIC_ZOOM				},
	{IR_PLAYER_SETUP,			MSG_MODE_DOWN,				MSG_MUSIC_SETUP				},
	{IR_PLAYER_SLOW,			MSG_MODE_DOWN,				MSG_MUSIC_SLOW				},
	{IR_PLAYER_TITLE,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_TITLE 			},
	{IR_PLAYER_SUB_T,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SUB_T 			},
	{IR_PLAYER_PROGRAM,			MSG_MODE_DOWN,				MSG_MUSIC_PROGRAM			},
	{IR_PLAYER_ANGLE,			MSG_MODE_DOWN,				MSG_MUSIC_ANGLE				},

    {IR_PLAYER_NAV_UP,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_ADD_10			},
	{IR_PLAYER_NAV_UP,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_NEXT_FOLDER		},
	{IR_PLAYER_NAV_DOWN,		MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_SUB_10			},
	{IR_PLAYER_NAV_DOWN,		MSG_MODE_DOWN_DELEY,		MSG_MUSIC_PREV_FOLDER		},

};

const ORDER_CFG ORDER_PLAYER_IR_2NDF[]=
{

	{IR_PLAYER_M0,				MSG_MODE_DOWN,				MSG_MUSIC_0					},
	{IR_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{IR_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{IR_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{IR_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{IR_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
	{IR_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},	
	{IR_PLAYER_M7,				MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR_PLAYER_M8,				MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{IR_PLAYER_M9,				MSG_MODE_DOWN,				MSG_MUSIC_9					},	

	{IR_PLAYER_ADD, 			MSG_MODE_DOWN,				MSG_MUSIC_ADD 				},
	{IR_PLAYER_SUB, 			MSG_MODE_DOWN,				MSG_MUSIC_SUB 				},
	{IR_PLAYER_SEL, 			MSG_MODE_DOWN,				MSG_MUSIC_SEL				},
};

#define IR_ORDER_PLAYER_2NDF 	ORDER_PLAYER_IR_2NDF


/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------蓝牙功能码---------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_BT_IR[]=
{

	{IR_BT_PAUSE,				MSG_MODE_DOWN,				MSG_BT_PP					},
	{IR_BT_NEXT,				MSG_MODE_DOWN,				MSG_BT_NEXT_FILE			},
	{IR_BT_BACK,				MSG_MODE_DOWN,				MSG_BT_PREV_FILE			},
	{IR_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_CALL			},
	{IR_BT_REDIAL, 				MSG_MODE_DOWN_DELEY,		MSG_BT_REDIAL				},
};	

const ORDER_CFG ORDER_BT_IR_2NDF[]=
{

	{IR_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_CALL			},
	{IR_BT_REJECT,				MSG_MODE_DOWN,				MSG_BT_CALL_REJECT			},
	
	{IR_BT_M0,					MSG_MODE_UP_LESS_THAN,		MSG_BT_0					},
	{IR_BT_M0,					MSG_MODE_DOWN_DELEY,		MSG_BT_ADD					},
	{IR_BT_M1,					MSG_MODE_DOWN,				MSG_BT_1					},
	{IR_BT_M2,					MSG_MODE_DOWN,				MSG_BT_2					},
	{IR_BT_M3,					MSG_MODE_DOWN,				MSG_BT_3					},
	{IR_BT_M4,					MSG_MODE_DOWN,				MSG_BT_4					},
	{IR_BT_M5,					MSG_MODE_DOWN,				MSG_BT_5					},
	{IR_BT_M6,					MSG_MODE_DOWN,				MSG_BT_6					},	
	{IR_BT_M7,					MSG_MODE_DOWN,				MSG_BT_7					},
	{IR_BT_M8,					MSG_MODE_DOWN,				MSG_BT_8					},
	{IR_BT_M9,					MSG_MODE_DOWN,				MSG_BT_9					},	
	{IR_BT_STAR,				MSG_MODE_DOWN,				MSG_BT_STAR					},
	{IR_BT_WELL,				MSG_MODE_DOWN,				MSG_BT_WELL					},
	{IR_BT_DEL,					MSG_MODE_DOWN_LOOP,			MSG_BT_DEL					},
};

#define IR_ORDER_BT_2NDF 	ORDER_BT_IR_2NDF

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------音效功能码------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_OTHERS_IR[]=
{
	{IR_SETTING_SEL,			MSG_MODE_UP_LESS_THAN,		MSG_USER_SEL				},
	{IR_SETTING_SEL,			MSG_MODE_DOWN_DELEY,		MSG_USER_FUNC				},
	{IR_SETTING_ADD,			MSG_MODE_DOWN_LOOP,			MSG_VOL_ADD					},
	{IR_SETTING_SUB,			MSG_MODE_DOWN_LOOP,			MSG_VOL_SUB					},
	{IR_SETTING_MUTE,			MSG_MODE_DOWN,				MSG_SYS_MUTE				},
};

