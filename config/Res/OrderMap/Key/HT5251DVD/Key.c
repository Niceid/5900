#include "Key.h"


/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------系统功能码---------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_SYSTEM_KEY[]=
{/*********value********************mode************************order******/
	{KEY_SYSTEM_POWER,			MSG_MODE_DOWN,				MSG_SYS_POWER				},
	{KEY_SYSTEM_MODE,			MSG_MODE_DOWN,				MSG_SYS_MODE				},

#if defined(KEY_SYSTEM_POWER_MUTE)
	{KEY_SYSTEM_POWER_MUTE,		MSG_MODE_UP_LESS_THAN,		MSG_SYS_MUTE				},
	{KEY_SYSTEM_POWER_MUTE,		MSG_MODE_DOWN_DELEY,		MSG_SYS_POWER				},
#endif

#if defined(KEY_SYSTEM_POWER_MODE)
	{KEY_SYSTEM_POWER_MODE,		MSG_MODE_UP_LESS_THAN,		MSG_SYS_MODE				},
	{KEY_SYSTEM_POWER_MODE,		MSG_MODE_DOWN_DELEY,		MSG_SYS_POWER				},
#endif	

#if defined(KEY_SYSTEM_AF)
	{KEY_SYSTEM_AF,				MSG_MODE_DOWN,				MSG_RDS_AF					},
#endif

#if defined(KEY_SYSTEM_TA)
	{KEY_SYSTEM_TA,				MSG_MODE_DOWN,				MSG_RDS_TA					},
#endif

#if defined(KEY_SYSTEM_AF_TA)
	{KEY_SYSTEM_AF_TA,			MSG_MODE_UP_LESS_THAN,		MSG_RDS_AF					},
	{KEY_SYSTEM_AF_TA,			MSG_MODE_DOWN_DELEY,		MSG_RDS_TA					},
#endif

#if defined(_SUPPORT_DISC_)
	{KEY_SYSTEM_EJECT,			MSG_MODE_DOWN,				MSG_SYS_EJECT 				},
#endif

#if defined(_ENABLE_FAD_FUNTION_)
#if defined(_ENABLE_FAD_RETURN_)
    {KEY_VALUE_FAD_SINGLE, 		MSG_MODE_UP_LESS_THAN,		MSG_FAD_SINGLE_R			},
	{KEY_VALUE_FAD_SINGLE, 		MSG_MODE_DOWN_DELEY,		MSG_FAD_SINGLE_F 			},
	{KEY_VALUE_FAD_ALL,			MSG_MODE_DOWN,				MSG_FAD_ALL					},
#else
	{KEY_VALUE_FAD_SINGLE, 		MSG_MODE_UP_LESS_THAN,		MSG_FAD_SINGLE_F			},
	{KEY_VALUE_FAD_SINGLE, 		MSG_MODE_DOWN_DELEY,		MSG_FAD_SINGLE_R 			},
	{KEY_VALUE_FAD_ALL,			MSG_MODE_DOWN,				MSG_FAD_ALL					},
#endif	
#endif
};

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------收音功能码---------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_RADIO_KEY[]=
{
	{KEY_RADIO_M1,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M1					},
	{KEY_RADIO_M2,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M2					},
	{KEY_RADIO_M3,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M3					},
	{KEY_RADIO_M4,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M4					},
	{KEY_RADIO_M5,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M5					},
	{KEY_RADIO_M6,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M6					},

	{KEY_RADIO_M1,				MSG_MODE_DOWN_DELEY,		MSG_FM_S1					},
	{KEY_RADIO_M2,				MSG_MODE_DOWN_DELEY,		MSG_FM_S2					},
	{KEY_RADIO_M3,				MSG_MODE_DOWN_DELEY,		MSG_FM_S3					},
	{KEY_RADIO_M4,				MSG_MODE_DOWN_DELEY,		MSG_FM_S4					},
	{KEY_RADIO_M5,				MSG_MODE_DOWN_DELEY,		MSG_FM_S5					},
	{KEY_RADIO_M6,				MSG_MODE_DOWN_DELEY,		MSG_FM_S6					},
	{KEY_RADIO_BAND,			MSG_MODE_DOWN,				MSG_FM_BAND					},

	{KEY_RADIO_NEXT,			MSG_MODE_UP_LESS_THAN ,		MSG_FM_NEXT_SEEK			},	
	{KEY_RADIO_NEXT,			MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_STEP			},
	{KEY_RADIO_BACK,			MSG_MODE_UP_LESS_THAN ,		MSG_FM_PREV_SEEK			},
	{KEY_RADIO_BACK,			MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_STEP			},
	
	{KEY_RADIO_ST,				MSG_MODE_DOWN,				MSG_FM_ST					},
	{KEY_RADIO_LOC,				MSG_MODE_DOWN,				MSG_FM_LOC					},
	{KEY_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{KEY_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,		MSG_FM_BROWSE				},
	{KEY_RADIO_SCAN,			MSG_MODE_DOWN,				MSG_FM_SCAN					},

	{KEY_RADIO_BAND_AMS,		MSG_MODE_UP_LESS_THAN,		MSG_FM_BAND					},
	{KEY_RADIO_BAND_AMS,		MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},

#if defined(_ENABLE_RADIO_RDS_)
	{KEY_RDS_PTY,				MSG_MODE_DOWN,				MSG_RDS_PTY 				},
#endif

#if defined(_ENABLE_SAVE_FREQ_)
#if defined(KEY_RADIO_NAVI_UP)
	{KEY_RADIO_NAVI_UP,			MSG_MODE_UP_LESS_THAN,		MSG_FM_SLECT_UP 			},
	{KEY_RADIO_NAVI_UP,			MSG_MODE_DOWN_DELEY,		MSG_FM_SLECT_UP 			},
#endif
#if defined(KEY_RADIO_NAVI_DOWN)
	{KEY_RADIO_NAVI_DOWN,		MSG_MODE_UP_LESS_THAN,		MSG_FM_SLECT_DOWN			},
	{KEY_RADIO_NAVI_DOWN,		MSG_MODE_DOWN_DELEY,		MSG_FM_SLECT_DOWN			},
#endif
#endif
};	

#if defined(_ENABLE_RADIO_RDS_)

const ORDER_CFG ORDER_RADIO_KEY_2NDF[]=
{
	{KEY_RDS_M1,				MSG_MODE_DOWN,				MSG_RDS_M1					},
	{KEY_RDS_M2,				MSG_MODE_DOWN,				MSG_RDS_M2					},
	{KEY_RDS_M3,				MSG_MODE_DOWN,				MSG_RDS_M3					},
	{KEY_RDS_M4,				MSG_MODE_DOWN,				MSG_RDS_M4					},
	{KEY_RDS_M5,				MSG_MODE_DOWN,				MSG_RDS_M5					},
 	{KEY_RDS_M6,				MSG_MODE_DOWN,				MSG_RDS_M6					},
	
	{KEY_RDS_PTY, 				MSG_MODE_DOWN,				MSG_RDS_PTY					},
	{KEY_RDS_NEXT, 				MSG_MODE_DOWN,				MSG_RDS_NEXT				},
	{KEY_RDS_BACK, 				MSG_MODE_DOWN,				MSG_RDS_BACK				},
	
	{KEY_RADIO_BAND,			MSG_MODE_DOWN,				MSG_FM_BAND					},
};

#define KEY_ORDER_RADIO_2NDF 	ORDER_RADIO_KEY_2NDF

#endif

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------播放功能码---------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_PLAYER_KEY[]=
{
    
	{KEY_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_PP				},
	{KEY_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_INT				},
	{KEY_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_RPT				},
	{KEY_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_RDM				},
	
	{KEY_PLAYER_M5,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_SUB_10			},
	{KEY_PLAYER_M5,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_PREV_FOLDER		},

	{KEY_PLAYER_M6,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_ADD_10			},
	{KEY_PLAYER_M6,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_NEXT_FOLDER		},

	{KEY_PLAYER_NEXT,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FILE			},
	{KEY_PLAYER_NEXT,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_FF				},
	
	{KEY_PLAYER_BACK,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FILE			},
	{KEY_PLAYER_BACK,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_FR				},
	
	{KEY_PLAYER_ID3,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_MP3_JPG_MP4		},
	{KEY_PLAYER_ID3,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_FORMAT			},
	{KEY_PLAYER_SCH,			MSG_MODE_DOWN,				MSG_MUSIC_SEARCH			},
	
#if defined(_ENABLE_TRK_FUNCTION_)
	{KEY_PLAYER_ID3_SCH,		MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_ID3		        },
    {KEY_PLAYER_ID3_SCH,		MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SEARCH			},
#else
	{KEY_PLAYER_ID3_SCH,		MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_MP3_JPG_MP4		},
	{KEY_PLAYER_ID3_SCH,		MSG_MODE_DOWN_DELEY,		MSG_MUSIC_FORMAT			},
#endif	

	{KEY_VALUE_NAVI_UP,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PP				},
	{KEY_VALUE_NAVI_UP, 		MSG_MODE_DOWN_DELEY,		MSG_MUSIC_NEXT_FOLDER		},
	{KEY_VALUE_NAVI_DOWN,		MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PLAY_MODE 		},
	{KEY_VALUE_NAVI_DOWN,		MSG_MODE_DOWN_DELEY,		MSG_MUSIC_PREV_FOLDER		},
};

const ORDER_CFG ORDER_PLAYER_KEY_2NDF[]=
{
 	{KEY_PLAYER_M0,				MSG_MODE_DOWN,				MSG_MUSIC_0					},
	{KEY_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{KEY_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{KEY_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{KEY_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{KEY_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
 	{KEY_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},
	{KEY_PLAYER_M7, 			MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{KEY_PLAYER_M8, 			MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{KEY_PLAYER_M9, 			MSG_MODE_DOWN,				MSG_MUSIC_9					},
	
	{KEY_PLAYER_ADD, 			MSG_MODE_DOWN,				MSG_MUSIC_ADD 				},
	{KEY_PLAYER_SUB, 			MSG_MODE_DOWN,				MSG_MUSIC_SUB 				},
	{KEY_PLAYER_ENTER, 			MSG_MODE_DOWN,				MSG_MUSIC_ENTER 			},
	
	{KEY_PLAYER_SCH,			MSG_MODE_DOWN,				MSG_MUSIC_SEARCH			},
	{KEY_PLAYER_ID3_SCH,		MSG_MODE_DOWN,				MSG_MUSIC_SEARCH			},
};

#define KEY_ORDER_PLAYER_2NDF 	ORDER_PLAYER_KEY_2NDF

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------蓝牙功能码---------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_BT_KEY[]=
{
	{KEY_BT_PAUSE1,				MSG_MODE_DOWN,				MSG_BT_PP					},
	{KEY_BT_PAUSE,				MSG_MODE_DOWN,				MSG_BT_PP					},
	{KEY_BT_NEXT,				MSG_MODE_DOWN,				MSG_BT_NEXT_FILE			},
	{KEY_BT_BACK,				MSG_MODE_DOWN,				MSG_BT_PREV_FILE			},
#if defined(_ENABLE_KEY_REDIAL_)
	{KEY_BT_ANSWER, 			MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_REDIAL		},
	{KEY_BT_ANSWER_2,			MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_REDIAL		},
#else
	{KEY_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_CALL			},
	{KEY_BT_ANSWER_2,			MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_CALL			},
#endif
	{KEY_BT_REJECT,				MSG_MODE_DOWN,				MSG_BT_CALL_REJECT			},

#if defined(_ENABLE_KWY_CONNECT_BT__)
	{KEY_CONNECT_BT_PHONE_, 	MSG_MODE_DOWN_DELEY,		MSG_BT_CONNECT_CTL			},
#endif

};	

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------其它功能码---------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_OTHERS_KEY[]=
{
#if defined(KEY_OTHERS_TIME_MUTE)
	{KEY_OTHERS_TIME_MUTE,		MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_MUTE				},
	{KEY_OTHERS_TIME_MUTE,		MSG_MODE_DOWN_DELEY,		MSG_CLOCK_MUTE				},
#endif
#if defined(_ENABLE_CLK_TIME_SET)
	{KEY_OTHERS_DISP,			MSG_MODE_UP_LESS_THAN,		MSG_CLK_TIME				},
	{KEY_OTHERS_DISP,			MSG_MODE_DOWN_DELEY,		MSG_CLK_TIME				},
#else
	{KEY_OTHERS_DISP,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_DISP				},
	{KEY_OTHERS_DISP,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_ADJ				},
#endif
#if defined(_ENABLE_SEL_TIME_)
	{KEY_OTHERS_SEL,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_SEL				},
	{KEY_OTHERS_SEL,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_SEL				},
#else
	{KEY_OTHERS_SEL,			MSG_MODE_UP_LESS_THAN,		MSG_USER_SEL				},
	{KEY_OTHERS_SEL,			MSG_MODE_DOWN_DELEY,		MSG_USER_FUNC				},
#endif
	{KEY_OTHERS_ADD,			MSG_MODE_DOWN_LOOP,			MSG_VOL_ADD					},
	{KEY_OTHERS_SUB,			MSG_MODE_DOWN_LOOP,			MSG_VOL_SUB					},
	{KEY_OTHERS_EQ,				MSG_MODE_DOWN,				MSG_AUD_EQ					},
	{KEY_OTHERS_LOUD,			MSG_MODE_DOWN,				MSG_AUD_LOUD				},
	{KEY_OTHERS_MUTE,           MSG_MODE_DOWN,      		MSG_SYS_MUTE                },
	{KEY_OTHERS_EQ_LOUD,		MSG_MODE_UP_LESS_THAN,		MSG_AUD_EQ					},
	{KEY_OTHERS_EQ_LOUD,		MSG_MODE_DOWN_DELEY,		MSG_AUD_LOUD				},
};

