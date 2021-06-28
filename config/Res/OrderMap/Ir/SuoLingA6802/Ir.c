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
	
#if defined(_ENABLE_RADIO_RDS_)
	{IR_RDS_AF, 				MSG_MODE_DOWN_DELEY,		MSG_RDS_AF		},
	{IR_RDS_TA, 				MSG_MODE_DOWN_DELEY,		MSG_RDS_TA		},
#endif

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

	{IR_RADIO_BAND,				MSG_MODE_DOWN,			    MSG_FM_BAND					},

#if defined(_SPECIAL_RADIO_SEEK_)
    {IR_RADIO_NEXT, 			MSG_MODE_UP_LESS_THAN , 	MSG_FM_NEXT_STEP			},	
    {IR_RADIO_NEXT, 			MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_SEEK			},
    {IR_RADIO_BACK, 			MSG_MODE_UP_LESS_THAN , 	MSG_FM_PREV_STEP			},
    {IR_RADIO_BACK, 			MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_SEEK			},
#else
	{IR_RADIO_NEXT,			    MSG_MODE_UP_LESS_THAN ,		MSG_FM_NEXT_SEEK			},	
	{IR_RADIO_NEXT,			    MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_STEP			},
	{IR_RADIO_BACK,				MSG_MODE_UP_LESS_THAN ,		MSG_FM_PREV_SEEK			},
	{IR_RADIO_BACK,				MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_STEP			},
#endif

	{IR_RADIO_ST,				MSG_MODE_UP_LESS_THAN,	    MSG_FM_ST					},
	{IR_RADIO_LOC,				MSG_MODE_UP_LESS_THAN,		MSG_FM_LOC					},
	{IR_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{IR_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,	    MSG_FM_BROWSE				},
	{IR_RADIO_SCAN,				MSG_MODE_DOWN,			    MSG_FM_SCAN					},

#if defined(_ENABLE_RADIO_RDS_)
	{IR_RDS_PTY,				MSG_MODE_DOWN_DELEY,		MSG_RDS_PTY 				},
#endif

};

#if defined(_ENABLE_RADIO_RDS_)

const ORDER_CFG ORDER_RADIO_IR_2NDF[]=
{
	{IR_RDS_M1,					MSG_MODE_DOWN,				MSG_RDS_M1					},
	{IR_RDS_M2,					MSG_MODE_DOWN,				MSG_RDS_M2					},
	{IR_RDS_M3,					MSG_MODE_DOWN,				MSG_RDS_M3					},
	{IR_RDS_M4,					MSG_MODE_DOWN,				MSG_RDS_M4					},
	{IR_RDS_M5,					MSG_MODE_DOWN,				MSG_RDS_M5					},
 	{IR_RDS_M6,					MSG_MODE_DOWN,				MSG_RDS_M6					},
	
	{IR_RDS_PTY, 				MSG_MODE_DOWN_DELEY,		MSG_RDS_PTY					},
	{IR_RDS_NEXT, 				MSG_MODE_DOWN,				MSG_RDS_NEXT				},
	{IR_RDS_BACK, 				MSG_MODE_DOWN,				MSG_RDS_BACK				},
	
	{IR_RADIO_BAND,				MSG_MODE_DOWN,				MSG_FM_BAND					},
};

#define IR_ORDER_RADIO_2NDF 	ORDER_RADIO_IR_2NDF

#endif

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------播放功能码------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_PLAYER_IR[]=
{
	{IR_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_PP				},
	{IR_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_INT				},
	{IR_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_RPT				},
	{IR_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_RDM				},
	{IR_PLAYER_M5,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FOLDER		},
	{IR_PLAYER_M5,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SUB_10		    },
	{IR_PLAYER_M6,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FOLDER		},
	{IR_PLAYER_M6,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_ADD_10		    },

	{IR_PLAYER_NEXT,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FILE			},
	{IR_PLAYER_NEXT,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FF				},
	{IR_PLAYER_NEXT,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
	
	{IR_PLAYER_BACK,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FILE			},
	{IR_PLAYER_BACK,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FR				},
	{IR_PLAYER_BACK,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
	
	{IR_PLAYER_ID3,				MSG_MODE_DOWN,			    MSG_MUSIC_ID3				},
	{IR_PLAYER_SCH,				MSG_MODE_DOWN,			    MSG_MUSIC_SEARCH			},
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
	{IR_PLAYER_M7, 			    MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR_PLAYER_M8, 			    MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{IR_PLAYER_M9, 			    MSG_MODE_DOWN,				MSG_MUSIC_9					},
	
	{IR_PLAYER_ADD, 			MSG_MODE_DOWN,				MSG_MUSIC_ADD 				},
	{IR_PLAYER_SUB, 			MSG_MODE_DOWN,				MSG_MUSIC_SUB 				},
	{IR_PLAYER_ENTER, 			MSG_MODE_DOWN,				MSG_MUSIC_ENTER 			},
	
	{IR_PLAYER_SCH,			    MSG_MODE_DOWN,				MSG_MUSIC_SEARCH			},
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
	{IR_BT_REJECT,				MSG_MODE_DOWN,				MSG_BT_CALL_REJECT			},
	{IR_BT_REDIAL,				MSG_MODE_DOWN_DELEY,	    MSG_BT_REDIAL				},
};	

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
	{IR_SETTING_EQ, 			MSG_MODE_DOWN,				MSG_AUD_EQ					},
	{IR_SETTING_LOUD,			MSG_MODE_UP_LESS_THAN,		MSG_AUD_LOUD				},
	{IR_SETTING_MUTE,			MSG_MODE_DOWN,				MSG_SYS_MUTE				},
	{IR_SETTING_CLK,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_DISP				},
	{IR_SETTING_CLK,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_ADJ				},
};

