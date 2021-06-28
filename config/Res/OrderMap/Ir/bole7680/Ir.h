#ifndef _IR_ORDER_MAP_
#define _IR_ORDER_MAP_


#define IR_RESPONSE_NULL													0xAA

/*---------------------------------------------------------------------------------------------------
	ϵͳ
*---------------------------------------------------------------------------------------------------*/

#define IR_SYSTEM_POWER														IR_RESPONSE_NULL
#define IR_SYSTEM_MODE														IR_VALUE_MODE


#if defined(IR_CODE_POWER_MUTE)
#define IR_SYSTEM_POWER_MUTE												IR_VALUE_POWER_MUTE
#endif

#if defined(IR_POWER_MODE)
	#define IR_SYSTEM_POWER_MODE											IR_VALUE_POWER_MODE
#endif

/*---------------------------------------------------------------------------------------------------
	����
/---------------------------------------------------------------------------------------------------*/

#define IR_RADIO_M1															IR_VALUE_M1
#define IR_RADIO_M2															IR_VALUE_M2
#define IR_RADIO_M3															IR_VALUE_M3
#define IR_RADIO_M4															IR_VALUE_M4
#define IR_RADIO_M5															IR_VALUE_M5
#define IR_RADIO_M6															IR_VALUE_M6

#define IR_RADIO_BAND														IR_VALUE_BAND
#define IR_RADIO_AMS														IR_VALUE_AMS
#define IR_RADIO_NEXT														IR_VALUE_NEXT
#define IR_RADIO_BACK														IR_VALUE_BACK
#define IR_RADIO_ST															IR_VALUE_ST
#define IR_RADIO_LOC														IR_VALUE_LOC
#if !defined(_BL_DISABLE_SCAN_FUNCTION_)
#define IR_RADIO_SCAN														IR_VALUE_SCAN
#endif

#if defined(_ENABLE_RADIO_RDS_)

#define IR_RDS_M1															IR_VALUE_M1
#define IR_RDS_M2															IR_VALUE_M2
#define IR_RDS_M3															IR_VALUE_M3
#define IR_RDS_M4															IR_VALUE_M4
#define IR_RDS_M5															IR_VALUE_M5
#define IR_RDS_M6															IR_VALUE_M6

#define IR_RDS_PTY															IR_VALUE_BAND
#define IR_RDS_NEXT															IR_VALUE_NEXT
#define IR_RDS_BACK															IR_VALUE_BACK

#endif

/*---------------------------------------------------------------------------------------------------
	����
*---------------------------------------------------------------------------------------------------*/

#define IR_PLAYER_M0														IR_VALUE_MODE
#define IR_PLAYER_M1														IR_VALUE_M1
#define IR_PLAYER_M2														IR_VALUE_M2
#define IR_PLAYER_M3														IR_VALUE_M3
#define IR_PLAYER_M4														IR_VALUE_M4
#define IR_PLAYER_M5														IR_VALUE_M5
#define IR_PLAYER_M6														IR_VALUE_M6
#define IR_PLAYER_M7														IR_VALUE_LOUD
#define IR_PLAYER_M8														IR_VALUE_ST
#define IR_PLAYER_M9														IR_VALUE_EQ

#define IR_PLAYER_NEXT														IR_VALUE_NEXT
#define IR_PLAYER_BACK														IR_VALUE_BACK

#define IR_PLAYER_ADD														IR_VALUE_ADD_VOL
#define IR_PLAYER_SUB														IR_VALUE_SUB_VOL

#define IR_PLAYER_ID3														IR_VALUE_BAND
#define IR_PLAYER_SCH														IR_VALUE_AMS

#define IR_PLAYER_ENTER														IR_VALUE_SEL

/*---------------------------------------------------------------------------------------------------
	����
*---------------------------------------------------------------------------------------------------*/

#define IR_BT_M0															IR_VALUE_MODE
#define IR_BT_M1															IR_VALUE_M1
#define IR_BT_M2															IR_VALUE_M2
#define IR_BT_M3															IR_VALUE_M3
#define IR_BT_M4															IR_VALUE_M4
#define IR_BT_M5															IR_VALUE_M5
#define IR_BT_M6															IR_VALUE_M6
#define IR_BT_M7															IR_VALUE_LOUD
#define IR_BT_M8															IR_VALUE_ST
#define IR_BT_M9															IR_VALUE_EQ
#define IR_BT_STAR															IR_VALUE_AMS
#define IR_BT_WELL															IR_VALUE_SCAN
#define IR_BT_DEL															IR_VALUE_REJECT

#define IR_BT_PAUSE															IR_VALUE_M1
#define IR_BT_NEXT															IR_VALUE_NEXT
#define IR_BT_BACK															IR_VALUE_BACK
#define IR_BT_ANSWER														IR_VALUE_ANSWER
#define IR_BT_REJECT														IR_VALUE_REJECT
#if defined(BL_IR_BT_REDIAL)
#define IR_BT_REDIAL                                                        IR_VALUE_ANSWER
#else
#define IR_BT_REDIAL                                                        IR_VALUE_BAND
#endif
#if defined(_SUPPORT_CALL_RECORDS_)
#define IR_BT_INFO															IR_VALUE_M2
#define IR_BT_ADD															IR_VALUE_ADD_VOL
#define IR_BT_SUB															IR_VALUE_SUB_VOL
#endif

#if defined(_SPECIAL_SIR_FUNTION_)
#define IR_BT_SIRI                                                          IR_VALUE_M2
#endif

/*---------------------------------------------------------------------------------------------------
	¼��
*---------------------------------------------------------------------------------------------------*/

#define IR_RECORD_PAUSE														IR_VALUE_PLAY
#define IR_RECORD_SAVE														KEY_VALUE_M2
#define IR_RECORD_MODE														KEY_VALUE_M3

/*---------------------------------------------------------------------------------------------------
	����
/---------------------------------------------------------------------------------------------------*/

#define IR_SETTING_SEL														IR_VALUE_SEL
#define IR_SETTING_ADD														IR_VALUE_ADD_VOL
#define IR_SETTING_SUB														IR_VALUE_SUB_VOL
#define IR_SETTING_EQ														IR_VALUE_EQ
#define IR_SETTING_LOUD														IR_VALUE_LOUD
#define IR_SETTING_MUTE														IR_VALUE_MUTE
#define IR_SETTING_CLK														IR_VALUE_CLK


#if defined(IR_ORDER_2NDF)
	#include "ir_func.c"
#endif

#endif /* _IR_ORDER_MAP_*/


