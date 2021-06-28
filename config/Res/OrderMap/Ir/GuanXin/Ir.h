#ifndef _IR_ORDER_MAP_
#define _IR_ORDER_MAP_


#define IR_RESPONSE_NULL													0xAA

/*---------------------------------------------------------------------------------------------------
	系统
*---------------------------------------------------------------------------------------------------*/

#define IR_SYSTEM_POWER														IR_VALUE1_POWER
#define IR_SYSTEM_MODE														IR_VALUE1_MODE

#if defined(IR_POWER_MUTE)
#define IR_SYSTEM_POWER_MUTE												IR_VALUE1_POWER_MUTE
#endif

#if defined(IR_POWER_MODE)
	#define IR_SYSTEM_POWER_MODE											IR_VALUE1_POWER_MODE
#endif

#define IR1_SYSTEM_POWER													IR_VALUE2_POWER
#define IR1_SYSTEM_MODE														IR_VALUE2_MODE


#define IR2_SYSTEM_POWER												    IR_VALUE3_POWER
#define IR2_SYSTEM_MODE														IR_VALUE3_MODE
#define IR2_SYSTEM_POWER_MUTE												IR_VALUE3_POWER_MUTE

/*---------------------------------------------------------------------------------------------------
	收音
/---------------------------------------------------------------------------------------------------*/

#define IR_RADIO_M1															IR_VALUE1_M1
#define IR_RADIO_M2															IR_VALUE1_M2
#define IR_RADIO_M3															IR_VALUE1_M3
#define IR_RADIO_M4															IR_VALUE1_M4
#define IR_RADIO_M5															IR_VALUE1_M5
#define IR_RADIO_M6															IR_VALUE1_M6

#define IR_RADIO_BAND														IR_VALUE1_BAND
#define IR_RADIO_AMS														IR_VALUE1_AMS
#define IR_RADIO_NEXT														IR_VALUE1_NEXT
#define IR_RADIO_BACK														IR_VALUE1_BACK
#define IR_RADIO_ST															IR_VALUE1_ST
#define IR_RADIO_LOC														IR_VALUE1_LOC
#define IR_RADIO_SCAN														IR_VALUE1_SCAN

#if defined(_ENABLE_RADIO_RDS_)

#define IR_RDS_M1															IR_VALUE1_M1
#define IR_RDS_M2															IR_VALUE1_M2
#define IR_RDS_M3															IR_VALUE1_M3
#define IR_RDS_M4															IR_VALUE1_M4
#define IR_RDS_M5															IR_VALUE1_M5
#define IR_RDS_M6															IR_VALUE1_M6

#define IR_RDS_PTY															IR_VALUE1_BAND
#define IR_RDS_NEXT															IR_VALUE1_NEXT
#define IR_RDS_BACK															IR_VALUE1_BACK

#endif

#define IR1_RADIO_M1														IR_VALUE2_M1
#define IR1_RADIO_M2														IR_VALUE2_M2
#define IR1_RADIO_M3														IR_VALUE2_M3
#define IR1_RADIO_M4														IR_VALUE2_M4
#define IR1_RADIO_M5														IR_VALUE2_M5
#define IR1_RADIO_M6														IR_VALUE2_M6

#define IR1_RADIO_BAND														IR_VALUE2_BAND
#define IR1_RADIO_AMS														IR_VALUE2_AMS
#define IR1_RADIO_NEXT														IR_VALUE2_NEXT
#define IR1_RADIO_BACK														IR_VALUE2_BACK
#define IR1_RADIO_ST														IR_VALUE2_ST
#define IR1_RADIO_LOC														IR_VALUE2_LOC
#define IR1_RADIO_SCAN														IR_VALUE2_SCAN

#if defined(_ENABLE_RADIO_RDS_)

#define IR1_RDS_M1															IR_VALUE2_M1
#define IR1_RDS_M2															IR_VALUE2_M2
#define IR1_RDS_M3															IR_VALUE2_M3
#define IR1_RDS_M4															IR_VALUE2_M4
#define IR1_RDS_M5															IR_VALUE2_M5
#define IR1_RDS_M6															IR_VALUE2_M6

#define IR1_RDS_PTY															IR_VALUE2_BAND
#define IR1_RDS_NEXT														IR_VALUE2_NEXT
#define IR1_RDS_BACK														IR_VALUE2_BACK

#endif

#define IR2_RADIO_M1														IR_VALUE3_M1
#define IR2_RADIO_M2														IR_VALUE3_M2
#define IR2_RADIO_M3														IR_VALUE3_M3
#define IR2_RADIO_M4														IR_VALUE3_M4
#define IR2_RADIO_M5														IR_VALUE3_M5
#define IR2_RADIO_M6														IR_VALUE3_M6
#define IR2_RADIO_NEXT														IR_VALUE3_NEXT
#define IR2_RADIO_BACK														IR_VALUE3_BACK

/*---------------------------------------------------------------------------------------------------
	放音
*---------------------------------------------------------------------------------------------------*/

#define IR_PLAYER_M0														IR_VALUE1_M0
#define IR_PLAYER_M1														IR_VALUE1_M1
#define IR_PLAYER_M2														IR_VALUE1_M2
#define IR_PLAYER_M3														IR_VALUE1_M3
#define IR_PLAYER_M4														IR_VALUE1_M4
#define IR_PLAYER_M5														IR_VALUE1_M5
#define IR_PLAYER_M6														IR_VALUE1_M6
#define IR_PLAYER_M7														IR_VALUE1_M7
#define IR_PLAYER_M8														IR_VALUE1_M8
#define IR_PLAYER_M9														IR_VALUE1_M9

#define IR_PLAYER_NEXT														IR_VALUE1_NEXT
#define IR_PLAYER_BACK														IR_VALUE1_BACK

#define IR_PLAYER_ADD														IR_VALUE1_ADD_VOL
#define IR_PLAYER_SUB														IR_VALUE1_SUB_VOL

#define IR_PLAYER_ID3														IR_VALUE1_BAND
#define IR_PLAYER_SCH														IR_VALUE1_AMS

#define IR_PLAYER_ENTER														IR_VALUE1_SEL

#define IR1_PLAYER_M0														IR_VALUE2_M0
#define IR1_PLAYER_M1														IR_VALUE2_M1
#define IR1_PLAYER_M2														IR_VALUE2_M2
#define IR1_PLAYER_M3														IR_VALUE2_M3
#define IR1_PLAYER_M4														IR_VALUE2_M4
#define IR1_PLAYER_M5														IR_VALUE2_M5
#define IR1_PLAYER_M6														IR_VALUE2_M6
#define IR1_PLAYER_M7														IR_VALUE2_M7
#define IR1_PLAYER_M8														IR_VALUE2_M8
#define IR1_PLAYER_M9														IR_VALUE2_M9

#define IR1_PLAYER_NEXT														IR_VALUE2_NEXT
#define IR1_PLAYER_BACK														IR_VALUE2_BACK

#define IR1_PLAYER_ADD														IR_VALUE2_ADD_VOL
#define IR1_PLAYER_SUB														IR_VALUE2_SUB_VOL

#define IR1_PLAYER_ID3														IR_VALUE2_BAND
#define IR1_PLAYER_SCH														IR_VALUE2_M0

#define IR1_PLAYER_ENTER													IR_VALUE2_SEL

#define IR1_PLAYER_PLAY														IR_VALUE2_PLAY

#define IR2_PLAYER_M0														IR_VALUE3_M0
#define IR2_PLAYER_M1														IR_VALUE3_M1
#define IR2_PLAYER_M2														IR_VALUE3_M2
#define IR2_PLAYER_M3														IR_VALUE3_M3
#define IR2_PLAYER_M4														IR_VALUE3_M4
#define IR2_PLAYER_M5														IR_VALUE3_M5
#define IR2_PLAYER_M6														IR_VALUE3_M6
#define IR2_PLAYER_M7														IR_VALUE3_M7
#define IR2_PLAYER_M8														IR_VALUE3_M8
#define IR2_PLAYER_M9														IR_VALUE3_M9
#define IR2_PLAYER_NEXT														IR_VALUE3_NEXT
#define IR2_PLAYER_BACK														IR_VALUE3_BACK
#define IR2_PLAYER_ADD														IR_VALUE3_ADD_VOL
#define IR2_PLAYER_SUB														IR_VALUE3_SUB_VOL
#define IR2_PLAYER_SCH														IR_VALUE3_M0

#define IR2_PLAYER_PLAY														IR_VALUE3_PLAY

/*---------------------------------------------------------------------------------------------------
	蓝牙
*---------------------------------------------------------------------------------------------------*/

#define IR_BT_M0															IR_VALUE1_NULL
#define IR_BT_M1															IR_VALUE1_NULL
#define IR_BT_M2															IR_VALUE1_NULL
#define IR_BT_M3															IR_VALUE1_NULL
#define IR_BT_M4															IR_VALUE1_NULL
#define IR_BT_M5															IR_VALUE1_NULL
#define IR_BT_M6															IR_VALUE1_NULL
#define IR_BT_M7															IR_VALUE1_NULL
#define IR_BT_M8															IR_VALUE1_NULL
#define IR_BT_M9															IR_VALUE1_NULL
#define IR_BT_STAR															IR_VALUE1_NULL
#define IR_BT_WELL															IR_VALUE1_NULL
#define IR_BT_DEL															IR_VALUE1_NULL

#define IR_BT_PAUSE															IR_VALUE1_M1
#define IR_BT_NEXT															IR_VALUE1_NEXT
#define IR_BT_BACK															IR_VALUE1_BACK
#if defined(_GX_CHANGE_KEYMAP_)
#define IR_BT_ANSWER														IR_RESPONSE_NULL
#define IR_BT_REJECT														IR_VALUE1_BAND
#define IR_BT_REDIAL                                                        IR_VALUE1_M5
#else
#define IR_BT_ANSWER														IR_RESPONSE_NULL
#define IR_BT_REJECT														IR_VALUE1_BAND
#define IR_BT_REDIAL                                                        IR_VALUE1_MODE
#endif

#if defined(_GX_CHANGE_KEYMAP_)
#define IR_SYSTEM_CONNECT                                                   IR_VALUE1_M6
#endif

#define IR1_BT_M0															IR_VALUE1_NULL
#define IR1_BT_M1															IR_VALUE1_NULL
#define IR1_BT_M2															IR_VALUE1_NULL
#define IR1_BT_M3															IR_VALUE1_NULL
#define IR1_BT_M4															IR_VALUE1_NULL
#define IR1_BT_M5															IR_VALUE1_NULL
#define IR1_BT_M6															IR_VALUE1_NULL
#define IR1_BT_M7															IR_VALUE1_NULL
#define IR1_BT_M8															IR_VALUE1_NULL
#define IR1_BT_M9															IR_VALUE1_NULL
#define IR1_BT_STAR															IR_VALUE1_NULL
#define IR1_BT_WELL															IR_VALUE1_NULL
#define IR1_BT_DEL															IR_VALUE1_NULL

#define IR1_BT_PAUSE														IR_VALUE2_PLAY
#define IR1_BT_NEXT															IR_VALUE2_NEXT
#define IR1_BT_BACK															IR_VALUE2_BACK
#define IR1_BT_ANSWER														IR_RESPONSE_NULL
#define IR1_BT_REJECT														IR_VALUE2_BAND
#define IR1_BT_REDIAL                                                       IR_VALUE2_MODE


#define IR2_BT_PAUSE														IR_VALUE3_PLAY
#define IR2_BT_NEXT															IR_VALUE3_NEXT
#define IR2_BT_BACK															IR_VALUE3_BACK
#define IR2_BT_ANSWER														IR_RESPONSE_NULL
#define IR2_BT_REJECT														IR_VALUE3_BAND
#define IR2_BT_REDIAL                                                       IR_VALUE3_MODE

/*---------------------------------------------------------------------------------------------------
	录音
*---------------------------------------------------------------------------------------------------*/

#define IR_RECORD_PAUSE														IR_VALUE1_PLAY
#define IR_RECORD_SAVE														KEY_VALUE_M2
#define IR_RECORD_MODE														KEY_VALUE_M3

#define IR1_RECORD_PAUSE													IR_VALUE2_PLAY

#define IR2_RECORD_PAUSE													IR_VALUE3_PLAY

/*---------------------------------------------------------------------------------------------------
	设置
/---------------------------------------------------------------------------------------------------*/

#define IR_SETTING_SEL														IR_VALUE1_SEL
#define IR_SETTING_ADD														IR_VALUE1_ADD_VOL
#define IR_SETTING_SUB														IR_VALUE1_SUB_VOL
#define IR_SETTING_EQ														IR_VALUE1_EQ
#define IR_SETTING_LOUD														IR_VALUE1_LOUD
#define IR_SETTING_MUTE														IR_VALUE1_MUTE
#define IR_SETTING_CLK														IR_VALUE1_CLK

#define IR1_SETTING_SEL														IR_VALUE2_SEL
#define IR1_SETTING_ADD														IR_VALUE2_ADD_VOL
#define IR1_SETTING_SUB														IR_VALUE2_SUB_VOL
#define IR1_SETTING_EQ														IR_VALUE2_EQ
#define IR1_SETTING_LOUD													IR_VALUE2_LOUD
#define IR1_SETTING_MUTE													IR_VALUE2_MUTE
#define IR1_SETTING_CLK														IR_VALUE2_CLK


#define IR2_SETTING_EQ														IR_VALUE3_EQ
#define IR2_SETTING_ADD														IR_VALUE3_ADD_VOL
#define IR2_SETTING_SUB														IR_VALUE3_SUB_VOL

#if defined(IR_ORDER_2NDF)
	#include "ir_func.c"
#endif

#endif /* _IR_ORDER_MAP_*/


