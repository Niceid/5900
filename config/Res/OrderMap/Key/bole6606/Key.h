#ifndef _KEY_ORDER_MAP_
#define _KEY_ORDER_MAP_


#define KEY_RESPONSE_NULL													0xAA

/*---------------------------------------------------------------------------------------------------
	系统
*---------------------------------------------------------------------------------------------------*/

#define KEY_SYSTEM_POWER													KEY_VALUE_POWER
#define KEY_SYSTEM_MODE														KEY_VALUE_MODE

#if defined(KEY_POWER_MUTE)
#define KEY_SYSTEM_POWER_MUTE												KEY_POWER_MUTE
#endif

#if defined(KEY_POWER_MODE)
	#define KEY_SYSTEM_POWER_MODE											KEY_POWER_MODE
#endif

#if defined(_ENABLE_RADIO_RDS_)
#define KEY_SYSTEM_AF														KEY_VALUE_ST
#define KEY_SYSTEM_TA														KEY_VALUE_LOC
#endif

/*---------------------------------------------------------------------------------------------------
	收音
/---------------------------------------------------------------------------------------------------*/

#define KEY_RADIO_M1														KEY_VALUE_M1
#define KEY_RADIO_M2														KEY_VALUE_M2
#define KEY_RADIO_M3														KEY_VALUE_M3
#define KEY_RADIO_M4														KEY_VALUE_M4
#define KEY_RADIO_M5														KEY_VALUE_M5
#define KEY_RADIO_M6														KEY_VALUE_M6

#define KEY_RADIO_BAND														KEY_VALUE_BAND
#define KEY_RADIO_AMS														KEY_VALUE_AMS
#define KEY_RADIO_NEXT														KEY_VALUE_NEXT
#define KEY_RADIO_BACK														KEY_VALUE_PREV
#if defined(_ENABLE_RADIO_RDS_)
#define KEY_RADIO_ST														KEY_RESPONSE_NULL
#define KEY_RADIO_LOC														KEY_RESPONSE_NULL
#else
#define KEY_RADIO_ST														KEY_VALUE_ST
#define KEY_RADIO_LOC														KEY_VALUE_LOC
#endif
#define KEY_RADIO_SCAN														KEY_VALUE_SCAN
#define KEY_RADIO_BAND_AMS													KEY_VALUE_BAND_AMS

#if defined(KEY_NAVI_UP)
#define KEY_RADIO_NAVI_UP													KEY_NAVI_UP
#endif
#if defined(KEY_NAVI_DOWN)
#define KEY_RADIO_NAVI_DOWN													KEY_NAVI_DOWN
#endif


#if defined(_ENABLE_RADIO_RDS_)

#define KEY_RDS_M1															KEY_VALUE_M1
#define KEY_RDS_M2															KEY_VALUE_M2
#define KEY_RDS_M3															KEY_VALUE_M3
#define KEY_RDS_M4															KEY_VALUE_M4
#define KEY_RDS_M5															KEY_VALUE_M5
#define KEY_RDS_M6															KEY_VALUE_M6

#define KEY_RDS_PTY															KEY_VALUE_ST
#define KEY_RDS_NEXT														KEY_VALUE_NEXT
#define KEY_RDS_BACK														KEY_VALUE_PREV

#endif

/*---------------------------------------------------------------------------------------------------
	放音
*---------------------------------------------------------------------------------------------------*/

#define KEY_PLAYER_M0														KEY_VALUE_NULL
#define KEY_PLAYER_M1														KEY_VALUE_M1
#define KEY_PLAYER_M2														KEY_VALUE_M2
#define KEY_PLAYER_M3														KEY_VALUE_M3
#define KEY_PLAYER_M4														KEY_VALUE_M4
#define KEY_PLAYER_M5														KEY_VALUE_M5
#define KEY_PLAYER_M6														KEY_VALUE_M6
#define KEY_PLAYER_M7														KEY_VALUE_NULL
#define KEY_PLAYER_M8														KEY_VALUE_NULL
#define KEY_PLAYER_M9														KEY_VALUE_NULL

#define KEY_PLAYER_ADD														KEY_VALUE_ADD
#define KEY_PLAYER_SUB														KEY_VALUE_SUB
#define KEY_PLAYER_ENTER													KEY_VALUE_SEL

#define KEY_PLAYER_NEXT														KEY_VALUE_NEXT
#define KEY_PLAYER_BACK														KEY_VALUE_PREV
#define KEY_PLAYER_ID3														KEY_VALUE_BAND
#define KEY_PLAYER_SCH														KEY_VALUE_AMS
#define KEY_PLAYER_ID3_SCH													KEY_VALUE_BAND_AMS
#define KEY_PLAYER_CHSUB													KEY_VALUE_NAVI_UP
#define KEY_PLAYER_CHADD													KEY_VALUE_NAVI_DOWN

/*---------------------------------------------------------------------------------------------------
	蓝牙
*---------------------------------------------------------------------------------------------------*/

#define KEY_BT_PAUSE														KEY_VALUE_M1
#define KEY_BT_NEXT															KEY_VALUE_NEXT
#define KEY_BT_BACK															KEY_VALUE_PREV
#define KEY_BT_ANSWER														KEY_VALUE_BAND
#define KEY_BT_ANSWER_2														KEY_VALUE_BAND_AMS
#define KEY_BT_REJECT														KEY_VALUE_NULL
#define KEY_BT_REDIAL													    KEY_VALUE_BAND_AMS

#if defined(_SUPPORT_CALL_RECORDS_)
#define KEY_BT_INFO															KEY_VALUE_M2
#define KEY_BT_ADD															KEY_VALUE_ADD
#define KEY_BT_SUB															KEY_VALUE_SUB
#endif

#if defined(_SUPPORT_IPHONE_SIR_)
#define KEY_BT_SIR															KEY_VALUE_M2
#endif

/*---------------------------------------------------------------------------------------------------
	录音
*---------------------------------------------------------------------------------------------------*/

#define KEY_RECORD_PAUSE													KEY_VALUE_M1
#define KEY_RECORD_SAVE														KEY_VALUE_M2
#define KEY_RECORD_MODE														KEY_VALUE_M3

/*---------------------------------------------------------------------------------------------------
	其它
/---------------------------------------------------------------------------------------------------*/
#if defined(KEY_TIME_MUTE)
#define KEY_OTHERS_TIME_MUTE 												KEY_VALUE_TIME_MUTE
#endif

#define KEY_OTHERS_DISP														KEY_VALUE_TIME
#define KEY_OTHERS_SEL														KEY_VALUE_SEL
#define KEY_OTHERS_ADD														KEY_VALUE_ADD
#define KEY_OTHERS_SUB														KEY_VALUE_SUB
#define KEY_OTHERS_EQ														KEY_VALUE_EQ
#define KEY_OTHERS_LOUD														KEY_VALUE_LOUD
#define KEY_OTHERS_MUTE														KEY_VALUE_MUTE
#define KEY_OTHERS_EQ_LOUD													KEY_VALUE_EQ_LOUD

/*---------------------------------------------------------------------------------------------------
	密码
/---------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_PASSWORD_DISPLAY_)

#define KEY_PASSWORD_NO_1													KEY_VALUE_M1
#define KEY_PASSWORD_NO_2													KEY_VALUE_M2
#define KEY_PASSWORD_NO_3													KEY_VALUE_M3
#define KEY_PASSWORD_NO_4													KEY_VALUE_M4

#define KEY_PASSWORD_NO_5													KEY_VALUE_M5
#define KEY_PASSWORD_NO_6													KEY_VALUE_M6

#define KEY_PASSWORD_ENTER													KEY_VALUE_SEL
#endif

#endif /* _KEY_ORDER_MAP_ */

