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
	{IR_SYSTEM_POWER_MUTE,		MSG_MODE_UP_LESS_THAN,		MSG_SYS_MUTE				},
	{IR_SYSTEM_POWER_MUTE,		MSG_MODE_DOWN_DELEY,		MSG_SYS_POWER				},

	{IR1_SYSTEM_POWER,			MSG_MODE_DOWN,				MSG_SYS_POWER				},
	{IR1_SYSTEM_MODE,			MSG_MODE_UP_LESS_THAN,		MSG_SYS_MODE				},

	
	{IR2_SYSTEM_MODE,			MSG_MODE_UP_LESS_THAN,		MSG_SYS_MODE				},
	{IR2_SYSTEM_POWER,			MSG_MODE_DOWN,				MSG_SYS_POWER				},
    {IR2_SYSTEM_POWER_MUTE,		MSG_MODE_UP_LESS_THAN,		MSG_SYS_MUTE				},
	{IR2_SYSTEM_POWER_MUTE,		MSG_MODE_DOWN_DELEY,		MSG_SYS_POWER				},
	
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

	{IR_RADIO_BAND,				MSG_MODE_UP_LESS_THAN,		MSG_FM_BAND					},

	{IR_RADIO_NEXT,			    MSG_MODE_UP_LESS_THAN ,		MSG_FM_NEXT_SEEK			},	
	{IR_RADIO_NEXT,			    MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_STEP			},
	{IR_RADIO_BACK,				MSG_MODE_UP_LESS_THAN ,		MSG_FM_PREV_SEEK			},
	{IR_RADIO_BACK,				MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_STEP			},

	{IR_RADIO_ST,				MSG_MODE_DOWN,			    MSG_FM_ST					},
	{IR_RADIO_LOC,				MSG_MODE_DOWN,			    MSG_FM_LOC					},
	{IR_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{IR_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,	    MSG_FM_BROWSE				},
	{IR_RADIO_SCAN,				MSG_MODE_DOWN,			    MSG_FM_SCAN					},

	{IR1_RADIO_M1,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M1					},
	{IR1_RADIO_M2,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M2					},
	{IR1_RADIO_M3,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M3					},
	{IR1_RADIO_M4,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M4					},
	{IR1_RADIO_M5,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M5					},
	{IR1_RADIO_M6,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M6					},
	
	{IR1_RADIO_M1,				MSG_MODE_DOWN_DELEY,		MSG_FM_S1					},
	{IR1_RADIO_M2,				MSG_MODE_DOWN_DELEY,		MSG_FM_S2					},
	{IR1_RADIO_M3,				MSG_MODE_DOWN_DELEY,		MSG_FM_S3					},
	{IR1_RADIO_M4,				MSG_MODE_DOWN_DELEY,		MSG_FM_S4					},
	{IR1_RADIO_M5,				MSG_MODE_DOWN_DELEY,		MSG_FM_S5					},
	{IR1_RADIO_M6,				MSG_MODE_DOWN_DELEY,		MSG_FM_S6					},

	{IR1_RADIO_BAND,			MSG_MODE_DOWN,			    MSG_FM_BAND					},

	{IR1_RADIO_NEXT,			MSG_MODE_UP_LESS_THAN ,		MSG_FM_NEXT_SEEK			},	
	{IR1_RADIO_NEXT,			MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_STEP			},
	{IR1_RADIO_BACK,		    MSG_MODE_UP_LESS_THAN ,		MSG_FM_PREV_SEEK			},
	{IR1_RADIO_BACK,			MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_STEP			},

	{IR1_RADIO_ST,				MSG_MODE_DOWN,			    MSG_FM_ST					},
	{IR1_RADIO_LOC,				MSG_MODE_DOWN,			    MSG_FM_LOC					},
	{IR1_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{IR1_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,	    MSG_FM_BROWSE				},
	{IR1_RADIO_SCAN,			MSG_MODE_DOWN,			    MSG_FM_SCAN					},

	{IR2_RADIO_M1,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M1					},
	{IR2_RADIO_M2,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M2					},
	{IR2_RADIO_M3,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M3					},
	{IR2_RADIO_M4,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M4					},
	{IR2_RADIO_M5,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M5					},
	{IR2_RADIO_M6,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M6					},
	{IR2_RADIO_M1,				MSG_MODE_DOWN_DELEY,		MSG_FM_S1					},
	{IR2_RADIO_M2,				MSG_MODE_DOWN_DELEY,		MSG_FM_S2					},
	{IR2_RADIO_M3,				MSG_MODE_DOWN_DELEY,		MSG_FM_S3					},
	{IR2_RADIO_M4,				MSG_MODE_DOWN_DELEY,		MSG_FM_S4					},
	{IR2_RADIO_M5,				MSG_MODE_DOWN_DELEY,		MSG_FM_S5					},
	{IR2_RADIO_M6,				MSG_MODE_DOWN_DELEY,		MSG_FM_S6					},
	{IR2_RADIO_NEXT,			MSG_MODE_UP_LESS_THAN ,		MSG_FM_NEXT_SEEK			},	
	{IR2_RADIO_NEXT,			MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_STEP			},
	{IR2_RADIO_BACK,			MSG_MODE_UP_LESS_THAN ,		MSG_FM_PREV_SEEK			},
	{IR2_RADIO_BACK,			MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_STEP			},
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
	
	{IR_RDS_PTY, 				MSG_MODE_DOWN,				MSG_RDS_PTY					},
	{IR_RDS_NEXT, 				MSG_MODE_DOWN,				MSG_RDS_NEXT				},
	{IR_RDS_BACK, 				MSG_MODE_DOWN,				MSG_RDS_BACK				},
	
	{IR_RADIO_BAND,				MSG_MODE_UP_LESS_THAN,		MSG_FM_BAND					},

	{IR1_RDS_M1,				MSG_MODE_DOWN,				MSG_RDS_M1					},
	{IR1_RDS_M2,				MSG_MODE_DOWN,				MSG_RDS_M2					},
	{IR1_RDS_M3,				MSG_MODE_DOWN,				MSG_RDS_M3					},
	{IR1_RDS_M4,				MSG_MODE_DOWN,				MSG_RDS_M4					},
	{IR1_RDS_M5,				MSG_MODE_DOWN,				MSG_RDS_M5					},
 	{IR1_RDS_M6,				MSG_MODE_DOWN,				MSG_RDS_M6					},
	
	{IR1_RDS_PTY, 				MSG_MODE_DOWN,				MSG_RDS_PTY					},
	{IR1_RDS_NEXT, 				MSG_MODE_DOWN,				MSG_RDS_NEXT				},
	{IR1_RDS_BACK, 				MSG_MODE_DOWN,				MSG_RDS_BACK				},
	
	{IR1_RADIO_BAND,			MSG_MODE_DOWN,				MSG_FM_BAND					},
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
	{IR_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_RPT				},
	{IR_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_RDM				},
	{IR_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_INT				},
	{IR_PLAYER_M5,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_SUB_10			},
	{IR_PLAYER_M5,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_PREV_FOLDER		},
	{IR_PLAYER_M6,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_ADD_10		    },
	{IR_PLAYER_M6,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_NEXT_FOLDER		},

	{IR_PLAYER_NEXT,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FILE			},
	{IR_PLAYER_NEXT,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FF				},
	{IR_PLAYER_NEXT,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
	
	{IR_PLAYER_BACK,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FILE			},
	{IR_PLAYER_BACK,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FR				},
	{IR_PLAYER_BACK,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
	
	{IR_PLAYER_ID3,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_ID3				},
	{IR_PLAYER_SCH,				MSG_MODE_DOWN,			    MSG_MUSIC_SEARCH			},

	{IR1_PLAYER_NEXT,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FILE			},
	{IR1_PLAYER_NEXT,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FF				},
	{IR1_PLAYER_NEXT,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
	
	{IR1_PLAYER_BACK,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FILE			},
	{IR1_PLAYER_BACK,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FR				},
	{IR1_PLAYER_BACK,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
	
	{IR1_PLAYER_ID3,			MSG_MODE_DOWN,			    MSG_MUSIC_ID3				},
	{IR1_PLAYER_SCH,			MSG_MODE_DOWN_DELEY,	    MSG_MUSIC_SEARCH			},

	{IR1_PLAYER_PLAY,			MSG_MODE_DOWN,			    MSG_MUSIC_PP				},

	//{IR2_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_PP				},
	//{IR2_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_RPT				},
	//{IR2_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_RDM				},
	//{IR2_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_INT				},
	//{IR2_PLAYER_M5,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_SUB_10			},
	//{IR2_PLAYER_M5,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_PREV_FOLDER		},
	//{IR2_PLAYER_M6,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_ADD_10		    },
	//{IR2_PLAYER_M6,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_NEXT_FOLDER		},
	{IR2_PLAYER_NEXT,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FILE			},
	{IR2_PLAYER_NEXT,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FF				},
	{IR2_PLAYER_NEXT,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
	{IR2_PLAYER_BACK,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FILE			},
	{IR2_PLAYER_BACK,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FR				},
	{IR2_PLAYER_BACK,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
	{IR2_PLAYER_SCH,			MSG_MODE_DOWN_DELEY,	    MSG_MUSIC_SEARCH			},
	{IR2_PLAYER_PLAY,			MSG_MODE_DOWN,			    MSG_MUSIC_PP				},
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

	{IR1_PLAYER_M0,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_0					},
	{IR1_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{IR1_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{IR1_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{IR1_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{IR1_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
 	{IR1_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},
	{IR1_PLAYER_M7, 			MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR1_PLAYER_M8, 			MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{IR1_PLAYER_M9, 			MSG_MODE_DOWN,				MSG_MUSIC_9					},
	
	{IR1_PLAYER_ADD, 			MSG_MODE_DOWN,				MSG_MUSIC_ADD 				},
	{IR1_PLAYER_SUB, 			MSG_MODE_DOWN,				MSG_MUSIC_SUB 				},
	{IR1_PLAYER_ENTER, 			MSG_MODE_DOWN,				MSG_MUSIC_ENTER 			},
	
	{IR1_PLAYER_SCH,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SEARCH			},

	{IR2_PLAYER_M0,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_0					},
	{IR2_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{IR2_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{IR2_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{IR2_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{IR2_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
 	{IR2_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},
 	{IR2_PLAYER_M7,				MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR2_PLAYER_M8,				MSG_MODE_DOWN,				MSG_MUSIC_8					},
 	{IR2_PLAYER_M9,				MSG_MODE_DOWN,				MSG_MUSIC_9					},
	{IR2_PLAYER_ADD, 			MSG_MODE_DOWN,				MSG_MUSIC_ADD 				},
	{IR2_PLAYER_SUB, 			MSG_MODE_DOWN,				MSG_MUSIC_SUB 				},
	{IR2_PLAYER_SCH,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SEARCH			},
};

#define IR_ORDER_PLAYER_2NDF 	ORDER_PLAYER_IR_2NDF

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------蓝牙功能码---------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_BT_IR[]=
{
	{IR_BT_PAUSE,				MSG_MODE_DOWN,			MSG_BT_PP					},
	{IR_BT_NEXT,				MSG_MODE_DOWN,			MSG_BT_NEXT_FILE			},
	{IR_BT_BACK,				MSG_MODE_DOWN,			MSG_BT_PREV_FILE			},
	{IR_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,	MSG_BT_ANSWER_CALL			},
	{IR_BT_REDIAL,				MSG_MODE_DOWN_DELEY,	MSG_BT_REDIAL		},
	{IR_BT_REJECT,				MSG_MODE_UP_LESS_THAN,	MSG_BT_CALL_REJECT			},
	
#if defined(_GX_CHANGE_KEYMAP_)
#if defined(_CONNECT_BT_PHONE_)
	{IR_SYSTEM_CONNECT, 		MSG_MODE_DOWN_DELEY,		MSG_BT_CONNECT_CTL 		},
#endif	
#endif
	{IR_BT_M0,					MSG_MODE_UP_LESS_THAN,	MSG_BT_0					},
	{IR_BT_M0,					MSG_MODE_DOWN_DELEY,	MSG_BT_ADD					},
	{IR_BT_M1,					MSG_MODE_DOWN,			MSG_BT_1					},
	{IR_BT_M2,					MSG_MODE_DOWN,			MSG_BT_2					},
	{IR_BT_M3,					MSG_MODE_DOWN,			MSG_BT_3					},
	{IR_BT_M4,					MSG_MODE_DOWN,			MSG_BT_4					},
	{IR_BT_M5,					MSG_MODE_DOWN,			MSG_BT_5					},
	{IR_BT_M6,					MSG_MODE_DOWN,			MSG_BT_6					},	
	{IR_BT_M7,					MSG_MODE_DOWN,			MSG_BT_7					},
	{IR_BT_M8,					MSG_MODE_DOWN,			MSG_BT_8					},
	{IR_BT_M9,					MSG_MODE_DOWN,			MSG_BT_9					},	
	{IR_BT_STAR,				MSG_MODE_DOWN,			MSG_BT_STAR					},
	{IR_BT_WELL,				MSG_MODE_DOWN,			MSG_BT_WELL					},
	{IR_BT_DEL,					MSG_MODE_DOWN_LOOP,		MSG_BT_DEL					},

	{IR1_BT_PAUSE,				MSG_MODE_DOWN,			MSG_BT_PP					},
	{IR1_BT_NEXT,				MSG_MODE_DOWN,			MSG_BT_NEXT_FILE			},
	{IR1_BT_BACK,				MSG_MODE_DOWN,			MSG_BT_PREV_FILE			},
	{IR1_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,	MSG_BT_ANSWER_CALL			},
	{IR1_BT_REDIAL,				MSG_MODE_DOWN_DELEY,	MSG_BT_REDIAL			},
	{IR1_BT_REJECT,				MSG_MODE_DOWN,			MSG_BT_CALL_REJECT			},

	{IR1_BT_M0,					MSG_MODE_UP_LESS_THAN,	MSG_BT_0					},
	{IR1_BT_M0,					MSG_MODE_DOWN_DELEY,	MSG_BT_ADD					},
	{IR1_BT_M1,					MSG_MODE_DOWN,			MSG_BT_1					},
	{IR1_BT_M2,					MSG_MODE_DOWN,			MSG_BT_2					},
	{IR1_BT_M3,					MSG_MODE_DOWN,			MSG_BT_3					},
	{IR1_BT_M4,					MSG_MODE_DOWN,			MSG_BT_4					},
	{IR1_BT_M5,					MSG_MODE_DOWN,			MSG_BT_5					},
	{IR1_BT_M6,					MSG_MODE_DOWN,			MSG_BT_6					},	
	{IR1_BT_M7,					MSG_MODE_DOWN,			MSG_BT_7					},
	{IR1_BT_M8,					MSG_MODE_DOWN,			MSG_BT_8					},
	{IR1_BT_M9,					MSG_MODE_DOWN,			MSG_BT_9					},	
	{IR1_BT_STAR,				MSG_MODE_DOWN,			MSG_BT_STAR					},
	{IR1_BT_WELL,				MSG_MODE_DOWN,			MSG_BT_WELL					},
	{IR1_BT_DEL,				MSG_MODE_DOWN_LOOP,		MSG_BT_DEL					},

    {IR2_BT_PAUSE,				MSG_MODE_DOWN,			MSG_BT_PP					},
	{IR2_BT_NEXT,				MSG_MODE_DOWN,			MSG_BT_NEXT_FILE			},
	{IR2_BT_BACK,				MSG_MODE_DOWN,			MSG_BT_PREV_FILE			},
	{IR2_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,	MSG_BT_ANSWER_CALL			},
	{IR2_BT_REDIAL,				MSG_MODE_DOWN_DELEY,	MSG_BT_REDIAL		},
	{IR2_BT_REJECT,				MSG_MODE_DOWN,	        MSG_BT_CALL_REJECT			},

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

	{IR_VALUE1_BAND, 			MSG_MODE_DOWN_DELEY,		MSG_AUD_EQ					},
	{IR_VALUE1_MODE,			MSG_MODE_DOWN_DELEY,		MSG_AUD_LOUD				},

	{IR_SETTING_EQ, 			MSG_MODE_DOWN,				MSG_AUD_EQ					},
	{IR_SETTING_LOUD,			MSG_MODE_DOWN,				MSG_AUD_LOUD				},
	{IR_SETTING_MUTE,			MSG_MODE_DOWN,				MSG_SYS_MUTE				},
	{IR_SETTING_CLK,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_DISP				},
	{IR_SETTING_CLK,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_ADJ				},

	{IR1_SETTING_SEL,			MSG_MODE_UP_LESS_THAN,		MSG_USER_SEL				},
	{IR1_SETTING_SEL,			MSG_MODE_DOWN_DELEY,		MSG_USER_FUNC				},
	{IR1_SETTING_ADD,			MSG_MODE_DOWN_LOOP,			MSG_VOL_ADD					},
	{IR1_SETTING_SUB,			MSG_MODE_DOWN_LOOP,			MSG_VOL_SUB					},
	{IR1_SETTING_EQ, 			MSG_MODE_DOWN,				MSG_AUD_EQ					},
	{IR1_SETTING_LOUD,			MSG_MODE_DOWN,				MSG_AUD_LOUD				},
	{IR1_SETTING_MUTE,			MSG_MODE_DOWN,				MSG_SYS_MUTE				},
	{IR1_SETTING_CLK,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_DISP				},
	{IR1_SETTING_CLK,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_ADJ				},

	{IR2_SETTING_ADD,			MSG_MODE_DOWN_LOOP, 		MSG_VOL_ADD 				},
	{IR2_SETTING_SUB,			MSG_MODE_DOWN_LOOP, 		MSG_VOL_SUB 				},
	{IR2_SETTING_EQ, 			MSG_MODE_DOWN,				MSG_AUD_EQ					},	
};

