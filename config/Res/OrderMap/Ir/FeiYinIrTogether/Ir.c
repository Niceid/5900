#include "Ir.h"
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------系统功能码------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
const ORDER_CFG ORDER_SYSTEM_IR[]=
{	
	{IR_SYSTEM_POWER,			MSG_MODE_DOWN,				MSG_SYS_POWER	},
	{IR_SYSTEM_MODE,			MSG_MODE_DOWN,				MSG_SYS_MODE	},
#if defined(_ENABLE_RADIO_RDS_)
	{IR_RDS_AF,					MSG_MODE_DOWN_DELEY,		MSG_RDS_AF		},
	{IR_RDS_TA,					MSG_MODE_DOWN_DELEY,		MSG_RDS_TA		},	
#endif

	{IR2_SYSTEM_POWER,			MSG_MODE_DOWN,				MSG_SYS_POWER	},
#if defined(_CONNECT_BT_PHONE_)
	{IR2_SYSTEM_MODE,			MSG_MODE_UP_LESS_THAN,		MSG_SYS_MODE	},
	{IR2_SYSTEM_MODE,			MSG_MODE_DOWN_DELEY,		MSG_SYS_CONNECT },
#else
	{IR2_SYSTEM_MODE,			MSG_MODE_DOWN,				MSG_SYS_MODE	},
#endif

#if defined(USE_FY_ADD_3KEY) 
#if defined(_ENABLE_RADIO_RDS_)
	{IR2_SETTING_CLK2,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_AF	},
	{IR2_SETTING_CLK2,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_AF	},
	{IR2_RADIO_TA,				MSG_MODE_DOWN_DELEY,		MSG_RDS_TA		},
#endif
#endif
	{IR2_SYSTEM_POWER_MUTE,		MSG_MODE_UP_LESS_THAN,		MSG_SYS_MUTE	},
	{IR2_SYSTEM_POWER_MUTE,		MSG_MODE_DOWN_DELEY,		MSG_SYS_POWER	},
	
#if !defined(USE_FY_ANOTHER_REMOTE)
	{IR3_SYSTEM_POWER_MUTE, 	MSG_MODE_UP_LESS_THAN,		MSG_SYS_MUTE	},
	{IR3_SYSTEM_POWER_MUTE, 	MSG_MODE_DOWN_DELEY,		MSG_SYS_POWER	},
	{IR3_SYSTEM_MODE,			MSG_MODE_DOWN,				MSG_SYS_MODE	},
#else
	{IR3_SYSTEM_POWER,			MSG_MODE_DOWN,				MSG_SYS_POWER				},
	{IR3_SYSTEM_MODE,			MSG_MODE_DOWN,				MSG_SYS_MODE				},
#if defined(_ENABLE_RADIO_RDS_)
	{IR3_SYSTEM_AF,				MSG_MODE_UP_LESS_THAN,		MSG_RDS_AF					},
	{IR3_SYSTEM_AF,				MSG_MODE_DOWN_DELEY,		MSG_RDS_REG					},
	{IR3_SYSTEM_TA,				MSG_MODE_UP_LESS_THAN,		MSG_RDS_TA					},
	{IR3_SYSTEM_TA,				MSG_MODE_DOWN_DELEY,		MSG_RDS_EON					},
#endif
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

	{IR_RADIO_BAND,				MSG_MODE_DOWN,				MSG_FM_BAND					},
	
	{IR_RADIO_BACK,				MSG_MODE_DOWN_LOOP,			MSG_FM_PREV_SEEK			},
	{IR_RADIO_NEXT,				MSG_MODE_DOWN_LOOP,			MSG_FM_NEXT_SEEK			},
	{IR_RADIO_BACKWARD,			MSG_MODE_DOWN,				MSG_FM_PREV_STEP 			},
	{IR_RADIO_FORWARD,			MSG_MODE_DOWN,				MSG_FM_NEXT_STEP 			},

	//{IR_RADIO_UP_PRESET,		MSG_MODE_DOWN,				MSG_FM_PREV_STATION			},
	//{IR_RADIO_PREV_PRESET,		MSG_MODE_DOWN,				MSG_FM_PREV_STATION			},
	//{IR_RADIO_DOWN_PRESET, 		MSG_MODE_DOWN,				MSG_FM_NEXT_STATION			},
	//{IR_RADIO_NEXT_PRESET,		MSG_MODE_DOWN,				MSG_FM_NEXT_STATION			},

	{IR_RADIO_ST,				MSG_MODE_DOWN,				MSG_FM_ST					},
	{IR_RADIO_LOC,				MSG_MODE_DOWN,				MSG_FM_LOC					},
	{IR_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{IR_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,		MSG_FM_BROWSE				},
#if defined(_ENABLE_RADIO_RDS_)
	{IR_RDS_PTY,				MSG_MODE_DOWN,				MSG_RDS_PTY 				},
#endif

//	{IR_RADIO_SCAN,				MSG_MODE_DOWN,				MSG_FM_SCAN					},
/***************************************************************************/
/***************************************************************************/
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

	{IR2_RADIO_BAND,				MSG_MODE_DOWN,				MSG_FM_BAND					},
	
	{IR2_RADIO_NEXT,				MSG_MODE_UP_LESS_THAN ,		MSG_FM_NEXT_SEEK			},	
	{IR2_RADIO_NEXT,				MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_STEP			},
	{IR2_RADIO_BACK,				MSG_MODE_UP_LESS_THAN ,		MSG_FM_PREV_SEEK			},
	{IR2_RADIO_BACK,				MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_STEP			},
#if defined(USE_FY_ADD_3KEY) 
	{IR2_RADIO_AMS2,				MSG_MODE_UP_LESS_THAN,		MSG_FM_AUTO_SAVE				},
#endif
//	{IR2_RADIO_ST,				MSG_MODE_DOWN,				MSG_FM_ST					},
//	{IR2_RADIO_LOC,				MSG_MODE_DOWN,				MSG_FM_LOC					},
	{IR2_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{IR2_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,		MSG_FM_BROWSE				},

/***************************************************************************/
/***************************************************************************/
#if !defined(USE_FY_ANOTHER_REMOTE)
	{IR3_RADIO_M1,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M1					},
	{IR3_RADIO_M2,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M2					},
	{IR3_RADIO_M3,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M3					},
	{IR3_RADIO_M4,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M4					},
	{IR3_RADIO_M5,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M5					},
	{IR3_RADIO_M6,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M6					},
	
	{IR3_RADIO_M1,				MSG_MODE_DOWN_DELEY,		MSG_FM_S1					},
	{IR3_RADIO_M2,				MSG_MODE_DOWN_DELEY,		MSG_FM_S2					},
	{IR3_RADIO_M3,				MSG_MODE_DOWN_DELEY,		MSG_FM_S3					},
	{IR3_RADIO_M4,				MSG_MODE_DOWN_DELEY,		MSG_FM_S4					},
	{IR3_RADIO_M5,				MSG_MODE_DOWN_DELEY,		MSG_FM_S5					},
	{IR3_RADIO_M6,				MSG_MODE_DOWN_DELEY,		MSG_FM_S6					},

	{IR3_RADIO_BAND,			MSG_MODE_DOWN,				MSG_FM_BAND					},
	
	{IR3_RADIO_NEXT,			MSG_MODE_UP_LESS_THAN ,		MSG_FM_NEXT_SEEK			},	
	{IR3_RADIO_NEXT,			MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_STEP			},
	{IR3_RADIO_BACK,			MSG_MODE_UP_LESS_THAN ,		MSG_FM_PREV_SEEK			},
	{IR3_RADIO_BACK,			MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_STEP			},


	{IR3_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{IR3_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,		MSG_FM_BROWSE				},
	
	{IR3_RADIO_ST,				MSG_MODE_DOWN,				MSG_FM_ST					},
#else	

	{IR3_RADIO_M1,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M1					},
	{IR3_RADIO_M2,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M2					},
	{IR3_RADIO_M3,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M3					},
	{IR3_RADIO_M4,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M4					},
	{IR3_RADIO_M5,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M5					},
	{IR3_RADIO_M6,				MSG_MODE_UP_LESS_THAN,		MSG_FM_M6					},
	    
	{IR3_RADIO_M1,				MSG_MODE_DOWN_DELEY,		MSG_FM_S1					},
	{IR3_RADIO_M2,				MSG_MODE_DOWN_DELEY,		MSG_FM_S2					},
	{IR3_RADIO_M3,				MSG_MODE_DOWN_DELEY,		MSG_FM_S3					},
	{IR3_RADIO_M4,				MSG_MODE_DOWN_DELEY,		MSG_FM_S4					},
	{IR3_RADIO_M5,				MSG_MODE_DOWN_DELEY,		MSG_FM_S5					},
	{IR3_RADIO_M6,				MSG_MODE_DOWN_DELEY,		MSG_FM_S6					},
      
	{IR3_RADIO_BAND,				MSG_MODE_DOWN,			    MSG_FM_BAND					},
      
	{IR3_RADIO_NEXT,			    MSG_MODE_UP_LESS_THAN ,		MSG_FM_NEXT_SEEK			},	
	{IR3_RADIO_NEXT,			    MSG_MODE_DOWN_DELEY,		MSG_FM_NEXT_STEP			},
	{IR3_RADIO_BACK,				MSG_MODE_UP_LESS_THAN ,		MSG_FM_PREV_SEEK			},
	{IR3_RADIO_BACK,				MSG_MODE_DOWN_DELEY,		MSG_FM_PREV_STEP			},
      
	{IR3_RADIO_ST,				MSG_MODE_DOWN,			    MSG_FM_ST					},
	{IR3_RADIO_LOC,				MSG_MODE_DOWN,			    MSG_FM_LOC					},
	{IR3_RADIO_AMS,				MSG_MODE_DOWN_DELEY,		MSG_FM_AUTO_SAVE			},
	{IR3_RADIO_AMS,				MSG_MODE_UP_LESS_THAN,	    MSG_FM_BROWSE				},
	{IR3_RADIO_SCAN,				MSG_MODE_DOWN,			    MSG_FM_SCAN					},

#if defined(_ENABLE_RADIO_RDS_)
	{IR3_RDS_PTY,				MSG_MODE_DOWN,				MSG_RDS_PTY 				},
#endif

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
	
	{IR_RDS_PTY, 				MSG_MODE_DOWN,				MSG_RDS_PTY					},
	{IR_RDS_NEXT, 				MSG_MODE_DOWN,				MSG_RDS_NEXT				},
	{IR_RDS_BACK, 				MSG_MODE_DOWN,				MSG_RDS_BACK				},
	
	{IR_RADIO_BAND,				MSG_MODE_DOWN,				MSG_FM_BAND					},
/***************************************************************************/
/***************************************************************************/
	{IR2_RDS_M1,					MSG_MODE_DOWN,				MSG_RDS_M1					},
	{IR2_RDS_M2,					MSG_MODE_DOWN,				MSG_RDS_M2					},
	{IR2_RDS_M3,					MSG_MODE_DOWN,				MSG_RDS_M3					},
	{IR2_RDS_M4,					MSG_MODE_DOWN,				MSG_RDS_M4					},
	{IR2_RDS_M5,					MSG_MODE_DOWN,				MSG_RDS_M5					},
 	{IR2_RDS_M6,					MSG_MODE_DOWN,				MSG_RDS_M6					},
	
	//{IR2_RDS_PTY, 				MSG_MODE_DOWN,				MSG_RDS_PTY					},
	{IR2_RDS_NEXT, 				MSG_MODE_DOWN,				MSG_RDS_NEXT				},
	{IR2_RDS_BACK, 				MSG_MODE_DOWN,				MSG_RDS_BACK				},
	
	{IR2_RADIO_BAND,				MSG_MODE_DOWN,				MSG_FM_BAND					},
/***************************************************************************/
/***************************************************************************/
#if !defined(USE_FY_ANOTHER_REMOTE)
	{IR3_RDS_M1,					MSG_MODE_DOWN,				MSG_RDS_M1					},
	{IR3_RDS_M2,					MSG_MODE_DOWN,				MSG_RDS_M2					},
	{IR3_RDS_M3,					MSG_MODE_DOWN,				MSG_RDS_M3					},
	{IR3_RDS_M4,					MSG_MODE_DOWN,				MSG_RDS_M4					},
	{IR3_RDS_M5,					MSG_MODE_DOWN,				MSG_RDS_M5					},
 	{IR3_RDS_M6,					MSG_MODE_DOWN,				MSG_RDS_M6					},
	
//	{IR3_RDS_PTY, 				MSG_MODE_DOWN,				MSG_RDS_PTY					},
	{IR3_RDS_NEXT, 				MSG_MODE_DOWN,				MSG_RDS_NEXT				},
	{IR3_RDS_BACK, 				MSG_MODE_DOWN,				MSG_RDS_BACK				},
	
	{IR3_RADIO_BAND,				MSG_MODE_DOWN,				MSG_FM_BAND					},
#else
	{IR3_RDS_M1,					MSG_MODE_DOWN,				MSG_RDS_M1					},
	{IR3_RDS_M2,					MSG_MODE_DOWN,				MSG_RDS_M2					},
	{IR3_RDS_M3,					MSG_MODE_DOWN,				MSG_RDS_M3					},
	{IR3_RDS_M4,					MSG_MODE_DOWN,				MSG_RDS_M4					},
	{IR3_RDS_M5,					MSG_MODE_DOWN,				MSG_RDS_M5					},
 	{IR3_RDS_M6,					MSG_MODE_DOWN,				MSG_RDS_M6					},
	    
	{IR3_RDS_PTY, 				MSG_MODE_DOWN,				MSG_RDS_PTY					},
	{IR3_RDS_NEXT, 				MSG_MODE_DOWN,				MSG_RDS_NEXT				},
	{IR3_RDS_BACK, 				MSG_MODE_DOWN,				MSG_RDS_BACK				},
	    
	{IR3_RADIO_BAND,				MSG_MODE_DOWN,				MSG_FM_BAND					},
#endif
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
	{IR_PLAYER_M0,				MSG_MODE_DOWN,				MSG_MUSIC_0					},
	{IR_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{IR_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{IR_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{IR_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{IR_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
	{IR_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},	
	
#if defined(_ENABLE_RADIO_RDS_)
	{IR_PLAYER_M7,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_7 				},
	{IR_PLAYER_M8,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_8 				},
	{IR_PLAYER_M9,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_9 				},	
#else
	{IR_PLAYER_M7,				MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR_PLAYER_M8,				MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{IR_PLAYER_M9,				MSG_MODE_DOWN,				MSG_MUSIC_9					},	
#endif

	{IR_PLAYER_UP,				MSG_MODE_DOWN,				MSG_MUSIC_UP				},
	{IR_PLAYER_DOWN,			MSG_MODE_DOWN,				MSG_MUSIC_DOWN				},
	{IR_PLAYER_LEFT,			MSG_MODE_DOWN,				MSG_MUSIC_LEFT				},
	{IR_PLAYER_RIGHT,			MSG_MODE_DOWN,				MSG_MUSIC_RIGHT				},
	{IR_PLAYER_ENTER,			MSG_MODE_DOWN,				MSG_MUSIC_ENTER				},
	
	{IR_PLAYER_PLAYPAUSE,		MSG_MODE_DOWN,				MSG_MUSIC_PP				},
	{IR_PLAYER_STOP,			MSG_MODE_DOWN,				MSG_MUSIC_STOP				},
	
	{IR_PLAYER_NEXT,			MSG_MODE_DOWN,				MSG_MUSIC_NEXT_FILE			},
	{IR_PLAYER_BACK,			MSG_MODE_DOWN,				MSG_MUSIC_PREV_FILE			},
	
	{IR_PLAYER_BACKWARD,		MSG_MODE_DOWN,				MSG_MUSIC_FR 				},
	{IR_PLAYER_FORWARD,			MSG_MODE_DOWN,				MSG_MUSIC_FF				},
	
	{IR_PLAYER_ID3,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_ID3				},
	{IR_PLAYER_SCH,				MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SEARCH			},

	{IR_PLAYER_RPT,				MSG_MODE_DOWN,				MSG_MUSIC_RPT				},

	{IR_PLAYER_CHANNEL,			MSG_MODE_DOWN,				MSG_MUSIC_CHANNEL			},
	{IR_PLAYER_OSD,				MSG_MODE_UP_LESS_THAN ,		MSG_MUSIC_OSD				},
	{IR_PLAYER_PBC,				MSG_MODE_DOWN,				MSG_MUSIC_PBC				},
	{IR_PLAYER_GOTO,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_GOTO				},
	{IR_PLAYER_ZOOM,			MSG_MODE_DOWN,				MSG_MUSIC_ZOOM				},
	{IR_PLAYER_FORMAT,			MSG_MODE_DOWN,				MSG_MUSIC_FORMAT			},
	{IR_PLAYER_SETUP,			MSG_MODE_DOWN,				MSG_MUSIC_SETUP				},
	{IR_PLAYER_SLOW,			MSG_MODE_DOWN,				MSG_MUSIC_SLOW				},
	{IR_PLAYER_PROGRAM,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_PROGRAM			},
	{IR_PLAYER_ANGLE,			MSG_MODE_DOWN,				MSG_MUSIC_ANGLE				},
	{IR_PLAYER_TITLE,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_TITLE				},
	{IR_PLAYER_SUB_T,			MSG_MODE_DOWN,				MSG_MUSIC_SUB_T				},
	{IR_PLAYER_ADD10,			MSG_MODE_DOWN,				MSG_MUSIC_ADD_10 			},
/*****************************************************************************/
/*****************************************************************************/
	{IR2_PLAYER_M0,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_0					},
	{IR2_PLAYER_M1,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_1					},
	{IR2_PLAYER_M2,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_2					},
	{IR2_PLAYER_M3,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_3					},
	{IR2_PLAYER_M4,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_4					},
	{IR2_PLAYER_M5,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_5					},
	{IR2_PLAYER_M6,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_6					},	
	{IR2_PLAYER_M7,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_7					},
	{IR2_PLAYER_M8,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_8					},
	{IR2_PLAYER_M9,				MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_9					},	

	{IR2_PLAYER_STEUP,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SETUP 	},
	{IR2_PLAYER_OSD, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_OSD		},
	{IR2_PLAYER_AUDIO,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_CHANNEL	},
	{IR2_PLAYER_ANGLE,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_ANGLE},
	{IR2_PLAYER_RPT, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_RPT	},
	{IR2_PLAYER_RDM, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_RDM},
	{IR2_PLAYER_INT, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_INT},
	{IR2_PLAYER_SUB_T,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SUB_T 	},
	{IR2_PLAYER_TITLE,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_TITLE 	},	
	{IR2_PLAYER_ZOOM,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_ZOOM		},



	{IR2_PLAYER_LEFT,			MSG_MODE_DOWN_LOOP,			MSG_MUSIC_LEFT		},
	{IR2_PLAYER_RIGHT,			MSG_MODE_DOWN_LOOP,			MSG_MUSIC_RIGHT 	},
	{IR2_PLAYER_UP,				MSG_MODE_DOWN_LOOP,			MSG_MUSIC_UP		},
	{IR2_PLAYER_DOWN,			MSG_MODE_DOWN_LOOP,			MSG_MUSIC_DOWN		},
	{IR2_PLAYER_ENTER,			MSG_MODE_DOWN,				MSG_MUSIC_ENTER},

	
	{IR2_PLAYER_PAUSE,			MSG_MODE_DOWN,				MSG_MUSIC_PLAY		},
	{IR2_PLAYER_STOP,			MSG_MODE_DOWN,				MSG_MUSIC_STOP		},

	{IR2_PLAYER_PREV,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FILE},
	{IR2_PLAYER_NEXT,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FILE},
	{IR2_PLAYER_PREV,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_FR},
	{IR2_PLAYER_NEXT,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_FF},
		
	{IR2_PLAYER_SLOW,			MSG_MODE_DOWN,				MSG_MUSIC_SLOW		}, //2014.04.24 mjs ORDER_MODE_DOWN_DELEY
	{IR2_PLAYER_PBC, 			MSG_MODE_DOWN,				MSG_MUSIC_PBC		}, //2014.04.24 mjs ORDER_MODE_DOWN_DELEY
	{IR2_PLAYER_SYS, 			MSG_MODE_DOWN,				MSG_MUSIC_FORMAT	},
#if defined(USE_FY_ADD_3KEY) 
	{IR2_PLAYER_ADD10,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_ADD_10	},
#endif
/*****************************************************************************/
/*****************************************************************************/
#if !defined(USE_FY_ANOTHER_REMOTE)
	{IR3_PLAYER_M0, 			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_SETUP 			},
	{IR3_PLAYER_M1, 			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PLAY 				},
	{IR3_PLAYER_M2, 			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_INT 				},
	{IR3_PLAYER_M3, 			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_RPT 				},
	{IR3_PLAYER_M4, 			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_RDM 				},
	{IR3_PLAYER_M5, 			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_CHANNEL 			},
	{IR3_PLAYER_M6, 			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PBC 				},	
	{IR3_PLAYER_M7, 			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_OSD 				},

	{IR3_PLAYER_M0, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_0 				},
	{IR3_PLAYER_M1, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_1 				},
	{IR3_PLAYER_M2, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_2 				},
	{IR3_PLAYER_M3, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_3 				},
	{IR3_PLAYER_M4, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_4 				},
	{IR3_PLAYER_M5, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_5 				},
	{IR3_PLAYER_M6, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_6 				},	
	{IR3_PLAYER_M7, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_7 				},
	{IR3_PLAYER_M8, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_8 				},	
	{IR3_PLAYER_M9, 			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_9 				},

	
	{IR3_PLAYER_TITLE,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_TITLE 			},	
	{IR3_PLAYER_SCH,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SEARCH 			},	
	{IR3_PLAYER_FORMAT,			MSG_MODE_UP_LESS_THAN, 		MSG_MUSIC_FORMAT 			},
	{IR3_PLAYER_MP3JPGMP4,		MSG_MODE_DOWN_DELEY, 	    MSG_MUSIC_MP3_JPG_MP4		},
	
	{IR3_PLAYER_UP, 			MSG_MODE_DOWN_LOOP, 		MSG_MUSIC_UP		},
	{IR3_PLAYER_DOWN,			MSG_MODE_DOWN_LOOP, 		MSG_MUSIC_DOWN		},
	{IR3_PLAYER_LEFT, 			MSG_MODE_UP_LESS_THAN, 		MSG_MUSIC_LEFT		},
	{IR3_PLAYER_RIGHT,			MSG_MODE_UP_LESS_THAN, 		MSG_MUSIC_RIGHT		},
	
	{IR3_PLAYER_STOP,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_STOP		},
	{IR3_PLAYER_GOTO,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_GOTO		},
	{IR3_PLAYER_SUB_T,			MSG_MODE_DOWN,				MSG_MUSIC_SUB_T 	},
#else
	{IR3_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_PP				},
	{IR3_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_INT				},
	{IR3_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_RPT				},
	{IR3_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_RDM				},
	{IR3_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_CHANNEL			},
	{IR3_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_STOP				},
      
	{IR3_PLAYER_NEXT,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_NEXT_FILE 		},
	{IR3_PLAYER_NEXT,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FF				},
	{IR3_PLAYER_NEXT,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},
      
	{IR3_PLAYER_BACK,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_PREV_FILE 		},
	{IR3_PLAYER_BACK,			MSG_MODE_DELEY_LOOP,		MSG_MUSIC_FR				},
	{IR3_PLAYER_BACK,			MSG_MODE_UP_MORE_THAN,		MSG_MUSIC_FFR_DONE			},

	{IR3_PLAYER_FORMAT,			MSG_MODE_UP_LESS_THAN, 		MSG_MUSIC_FORMAT 			},
	{IR3_PLAYER_MP3JPGMP4,		MSG_MODE_DOWN_DELEY, 	    MSG_MUSIC_MP3_JPG_MP4		},
	{IR3_PLAYER_TITLE,			MSG_MODE_UP_LESS_THAN,		MSG_MUSIC_TITLE 			},	
	{IR3_PLAYER_SCH,			MSG_MODE_DOWN_DELEY,		MSG_MUSIC_SEARCH 			},	
#endif
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
/*****************************************************************************/
/*****************************************************************************/
	{IR2_PLAYER_M0,				MSG_MODE_DOWN,				MSG_MUSIC_0					},
	{IR2_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{IR2_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{IR2_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{IR2_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{IR2_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
	{IR2_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},	
	{IR2_PLAYER_M7,				MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR2_PLAYER_M8,				MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{IR2_PLAYER_M9,				MSG_MODE_DOWN,				MSG_MUSIC_9					},	
	
	{IR2_PLAYER_SEL, 			MSG_MODE_DOWN,				MSG_MUSIC_SEL				},
	{IR2_PLAYER_ADD, 			MSG_MODE_DOWN,				MSG_MUSIC_ADD 				},
	{IR2_PLAYER_SUB, 			MSG_MODE_DOWN,				MSG_MUSIC_SUB 				},
/*****************************************************************************/
/*****************************************************************************/
#if !defined(USE_FY_ANOTHER_REMOTE)
	{IR3_PLAYER_M0,				MSG_MODE_DOWN,				MSG_MUSIC_0					},
	{IR3_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{IR3_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{IR3_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{IR3_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{IR3_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
	{IR3_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},	
	{IR3_PLAYER_M7,				MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR3_PLAYER_M8,				MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{IR3_PLAYER_M9,				MSG_MODE_DOWN,				MSG_MUSIC_9					},	
	
	{IR3_PLAYER_ADD, 			MSG_MODE_DOWN,				MSG_MUSIC_ADD 				},
	{IR3_PLAYER_SUB, 			MSG_MODE_DOWN,				MSG_MUSIC_SUB 				},
#else
	{IR3_PLAYER_M0,				MSG_MODE_DOWN,				MSG_MUSIC_0					},
	{IR3_PLAYER_M1,				MSG_MODE_DOWN,				MSG_MUSIC_1					},
	{IR3_PLAYER_M2,				MSG_MODE_DOWN,				MSG_MUSIC_2					},
	{IR3_PLAYER_M3,				MSG_MODE_DOWN,				MSG_MUSIC_3					},
	{IR3_PLAYER_M4,				MSG_MODE_DOWN,				MSG_MUSIC_4					},
	{IR3_PLAYER_M5,				MSG_MODE_DOWN,				MSG_MUSIC_5					},
 	{IR3_PLAYER_M6,				MSG_MODE_DOWN,				MSG_MUSIC_6					},
	{IR3_PLAYER_M7, 			    MSG_MODE_DOWN,				MSG_MUSIC_7					},
	{IR3_PLAYER_M8, 			    MSG_MODE_DOWN,				MSG_MUSIC_8					},
	{IR3_PLAYER_M9, 			    MSG_MODE_DOWN,				MSG_MUSIC_9					},
	    
	{IR3_PLAYER_ADD, 			MSG_MODE_DOWN,				MSG_MUSIC_ADD 				},
	{IR3_PLAYER_SUB, 			MSG_MODE_DOWN,				MSG_MUSIC_SUB 				},
	{IR3_PLAYER_ENTER, 			MSG_MODE_DOWN,				MSG_MUSIC_ENTER 			},
	    
	{IR3_PLAYER_SCH,			    MSG_MODE_DOWN,				MSG_MUSIC_SEARCH			},
#endif

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
	{IR_BT_REJECT,				MSG_MODE_DOWN,				MSG_BT_CALL_REJECT			},
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
	{IR2_BT_PAUSE,				MSG_MODE_DOWN,			MSG_BT_PP					},
	{IR2_BT_NEXT,				MSG_MODE_DOWN,			MSG_BT_NEXT_FILE			},
	{IR2_BT_BACK,				MSG_MODE_DOWN,			MSG_BT_PREV_FILE			},
	{IR2_BT_REDIAL,				MSG_MODE_DOWN_DELEY,	MSG_BT_REDIAL				},
	{IR2_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,	MSG_BT_ANSWER_CALL			},
	{IR2_BT_REJECT,				MSG_MODE_DOWN,			MSG_BT_CALL_REJECT			},
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
#if !defined(USE_FY_ANOTHER_REMOTE)
	{IR3_BT_PAUSE,				MSG_MODE_DOWN,			MSG_BT_PP					},
	{IR3_BT_NEXT,				MSG_MODE_DOWN,			MSG_BT_NEXT_FILE			},
	{IR3_BT_BACK,				MSG_MODE_DOWN,			MSG_BT_PREV_FILE			},
	{IR3_BT_ANSWER, 			MSG_MODE_UP_LESS_THAN,	MSG_BT_ANSWER_CALL			},
	{IR3_BT_ANSWER, 			MSG_MODE_DOWN_DELEY,	MSG_BT_REDIAL				},
#else
	{IR3_BT_PAUSE,				MSG_MODE_DOWN,			MSG_BT_PP					},
	{IR3_BT_NEXT,				MSG_MODE_DOWN,			MSG_BT_NEXT_FILE			},
	{IR3_BT_BACK,				MSG_MODE_DOWN,			MSG_BT_PREV_FILE			},
	{IR3_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,	MSG_BT_ANSWER_CALL			},
	{IR3_BT_ANSWER,				MSG_MODE_DOWN_DELEY,	MSG_BT_REDIAL				},
	{IR3_BT_REJECT,				MSG_MODE_DOWN,			MSG_BT_CALL_REJECT			},
#endif
};	

const ORDER_CFG ORDER_BT_IR_2NDF[]=
{
	{IR_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_CALL			},
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
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
	{IR2_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_CALL			},
	{IR2_BT_M0,					MSG_MODE_UP_LESS_THAN,		MSG_BT_0					},
	{IR2_BT_M1,					MSG_MODE_DOWN,				MSG_BT_1					},
	{IR2_BT_M2,					MSG_MODE_DOWN,				MSG_BT_2					},
	{IR2_BT_M3,					MSG_MODE_DOWN,				MSG_BT_3					},
	{IR2_BT_M4,					MSG_MODE_DOWN,				MSG_BT_4					},
	{IR2_BT_M5,					MSG_MODE_DOWN,				MSG_BT_5					},
	{IR2_BT_M6,					MSG_MODE_DOWN,				MSG_BT_6					},	
	{IR2_BT_M7,					MSG_MODE_DOWN,				MSG_BT_7					},
	{IR2_BT_M8,					MSG_MODE_DOWN,				MSG_BT_8					},
	{IR2_BT_M9,					MSG_MODE_DOWN,				MSG_BT_9					},	
	{IR2_BT_M0,					MSG_MODE_DOWN_DELEY,		MSG_BT_ADD					},
	{IR2_BT_STAR,				MSG_MODE_DOWN,				MSG_BT_STAR					},
	{IR2_BT_WELL,				MSG_MODE_DOWN,				MSG_BT_WELL					},
    {IR2_BT_DEL,				MSG_MODE_DOWN_LOOP,			MSG_BT_DEL					},
    
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
#if !defined(USE_FY_ANOTHER_REMOTE)
	{IR3_BT_ANSWER, 			MSG_MODE_UP_LESS_THAN,		MSG_BT_ANSWER_CALL			},
	{IR3_BT_M0, 				MSG_MODE_DOWN,				MSG_BT_0					},
	{IR3_BT_M1, 				MSG_MODE_DOWN,				MSG_BT_1					},
	{IR3_BT_M2, 				MSG_MODE_DOWN,				MSG_BT_2					},
	{IR3_BT_M3, 				MSG_MODE_DOWN,				MSG_BT_3					},
	{IR3_BT_M4, 				MSG_MODE_DOWN,				MSG_BT_4					},
	{IR3_BT_M5, 				MSG_MODE_DOWN,				MSG_BT_5					},
	{IR3_BT_M6, 				MSG_MODE_DOWN,				MSG_BT_6					},	
	{IR3_BT_M7, 				MSG_MODE_DOWN,				MSG_BT_7					},
	{IR3_BT_M8, 				MSG_MODE_DOWN,				MSG_BT_8					},
	{IR3_BT_M9, 				MSG_MODE_DOWN,				MSG_BT_9					},	
	{IR3_BT_M0, 				MSG_MODE_DOWN_DELEY,		MSG_BT_ADD					},
	{IR3_BT_STAR,				MSG_MODE_DOWN,				MSG_BT_STAR 				},
	{IR3_BT_WELL,				MSG_MODE_DOWN,				MSG_BT_WELL 				},
	{IR3_BT_DEL,				MSG_MODE_DOWN_LOOP, 		MSG_BT_DEL					},
#else
	{IR3_BT_ANSWER,				MSG_MODE_UP_LESS_THAN,	MSG_BT_ANSWER_CALL			},
	{IR3_BT_M0,					MSG_MODE_UP_LESS_THAN,	MSG_BT_0					},
	{IR3_BT_M1,					MSG_MODE_DOWN,			MSG_BT_1					},
	{IR3_BT_M2,					MSG_MODE_DOWN,			MSG_BT_2					},
	{IR3_BT_M3,					MSG_MODE_DOWN,			MSG_BT_3					},
	{IR3_BT_M4,					MSG_MODE_DOWN,			MSG_BT_4					},
	{IR3_BT_M5,					MSG_MODE_DOWN,			MSG_BT_5					},
	{IR3_BT_M6,					MSG_MODE_DOWN,			MSG_BT_6					},	
	{IR3_BT_M7,					MSG_MODE_DOWN,			MSG_BT_7					},
	{IR3_BT_M8,					MSG_MODE_DOWN,			MSG_BT_8					},
	{IR3_BT_M9,					MSG_MODE_DOWN,			MSG_BT_9					},	
	{IR3_BT_M0,					MSG_MODE_DOWN_DELEY,	MSG_BT_ADD					},
	{IR3_BT_STAR,				MSG_MODE_DOWN,			MSG_BT_STAR					},
	{IR3_BT_WELL,				MSG_MODE_DOWN,			MSG_BT_WELL					},
#endif	
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
	{IR_SETTING_EQ,				MSG_MODE_DOWN,				MSG_AUD_EQ					},
	{IR_SETTING_LOUD,			MSG_MODE_DOWN,				MSG_AUD_LOUD				},
	{IR_SETTING_MUTE,			MSG_MODE_DOWN,				MSG_SYS_MUTE				},
	{IR_SETTING_CLK,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_DISP				},
	{IR_SETTING_CLK,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_ADJ				},
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
#if defined(USE_FY_ADD_3KEY) 
	{IR2_SETTING_SEL2,			MSG_MODE_UP_LESS_THAN,		MSG_USER_SEL				},
	{IR2_SETTING_SEL2,			MSG_MODE_DOWN_DELEY,		MSG_USER_FUNC2				},
#endif

	{IR2_SETTING_SEL,			MSG_MODE_UP_LESS_THAN,		MSG_USER_SEL				},
	{IR2_SETTING_SEL,			MSG_MODE_DOWN_DELEY,		MSG_USER_FUNC				},
	{IR2_SETTING_ADD,			MSG_MODE_DOWN_LOOP,			MSG_VOL_ADD					},
	{IR2_SETTING_SUB,			MSG_MODE_DOWN_LOOP,			MSG_VOL_SUB					},
	{IR2_SETTING_EQ_LOUD,		MSG_MODE_UP_LESS_THAN,		MSG_AUD_EQ					},
	{IR2_SETTING_EQ_LOUD,		MSG_MODE_DOWN_DELEY,		MSG_AUD_LOUD				},

#if !defined(_ENABLE_RADIO_RDS_)
	{IR2_SETTING_CLK2,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_DISP				},
	{IR2_SETTING_CLK2,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_ADJ				},
#endif

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
#if !defined(USE_FY_ANOTHER_REMOTE)
	{IR3_SETTING_SEL,			MSG_MODE_UP_LESS_THAN,		MSG_USER_SEL				},
	{IR3_SETTING_SEL,			MSG_MODE_DOWN_DELEY,		MSG_USER_FUNC				},
	{IR3_SETTING_ADD,			MSG_MODE_DOWN_LOOP, 		MSG_VOL_ADD 				},
	{IR3_SETTING_SUB,			MSG_MODE_DOWN_LOOP, 		MSG_VOL_SUB 				},
	{IR3_SETTING_CLK,			MSG_MODE_UP_LESS_THAN,		MSG_ENTER_CLOCK				},
	{IR3_SETTING_CLK,			MSG_MODE_DOWN_DELEY,		MSG_ENTER_CLOCK				},
#else
	{IR3_SETTING_SEL,			MSG_MODE_UP_LESS_THAN,		MSG_USER_SEL				},
	{IR3_SETTING_SEL,			MSG_MODE_DOWN_DELEY,		MSG_USER_FUNC				},
	{IR3_SETTING_ADD,			MSG_MODE_DOWN_LOOP,			MSG_VOL_ADD					},
	{IR3_SETTING_SUB,			MSG_MODE_DOWN_LOOP,			MSG_VOL_SUB					},
	{IR3_SETTING_EQ, 			MSG_MODE_UP_LESS_THAN,		MSG_AUD_EQ					},
	{IR3_SETTING_EQ, 			MSG_MODE_DOWN_DELEY,		MSG_AUD_LOUD				},
	{IR3_SETTING_LOUD,			MSG_MODE_DOWN,				MSG_AUD_LOUD				},
	{IR3_SETTING_MUTE,			MSG_MODE_DOWN,				MSG_SYS_MUTE				},
	{IR3_SETTING_CLK,			MSG_MODE_UP_LESS_THAN,		MSG_CLOCK_DISP				},
	{IR3_SETTING_CLK,			MSG_MODE_DOWN_DELEY,		MSG_CLOCK_ADJ				},
#endif
};



