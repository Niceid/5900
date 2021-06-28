#ifndef _IR_ORDER_MAP_
#define _IR_ORDER_MAP_


#define IR_RESPONSE_NULL													0xAA

/*---------------------------------------------------------------------------------------------------
	ϵͳ
*---------------------------------------------------------------------------------------------------*/

#define IR_SYSTEM_POWER														IR_VALUE_POWER
#define IR_SYSTEM_MODE														IR_VALUE_MODE

#if defined(IR_POWER_MUTE)
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

#define IR_RADIO_NEXT_STEP													IR_VALUE_SEEK_UP
#define IR_RADIO_BACK_STEP													IR_VALUE_SEEK_DOWN

#define IR_RADIO_ST															IR_VALUE_ST
#define IR_RADIO_LOC														IR_VALUE_LOC


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

#define IR_PLAYER_M0														IR_VALUE_M0
#define IR_PLAYER_M1														IR_VALUE_M1
#define IR_PLAYER_M2														IR_VALUE_M2
#define IR_PLAYER_M3														IR_VALUE_M3
#define IR_PLAYER_M4														IR_VALUE_M4
#define IR_PLAYER_M5														IR_VALUE_M5
#define IR_PLAYER_M6														IR_VALUE_M6
#define IR_PLAYER_M7														IR_VALUE_M7
#define IR_PLAYER_M8														IR_VALUE_M8
#define IR_PLAYER_M9														IR_VALUE_M9

#define IR_PLAYER_UP														IR_VALUE_UP
#define IR_PLAYER_DOWN														IR_VALUE_DOWN
#define IR_PLAYER_LEFT														IR_VALUE_LEFT
#define IR_PLAYER_RIGHT														IR_VALUE_RIGHT
#define IR_PLAYER_ENTER														IR_VALUE_ENTER
#define IR_PLAYER_SEL														IR_VALUE_SEL

#define IR_PLAYER_NEXT														IR_VALUE_NEXT
#define IR_PLAYER_BACK														IR_VALUE_BACK

#define IR_PLAYER_FORWARD													IR_VALUE_SEEK_UP
#define IR_PLAYER_BACKWARD													IR_VALUE_SEEK_DOWN

#define IR_PLAYER_ID3														IR_VALUE_BAND
#define IR_PLAYER_SCH														IR_VALUE_AMS
#define IR_PLAYER_FORMAT													IR_VALUE_BAND

#define IR_PLAYER_CHANNEL													IR_VALUE_AUDIO

#define IR_PLAYER_PLAYPAUSE													IR_VALUE_PLAY

#define IR_PLAYER_RPT														IR_VALUE_AMS

#define IR_PLAYER_PROGRAM													IR_VALUE_ST
#define IR_PLAYER_OSD														IR_VALUE_OSD 
#define IR_PLAYER_PBC														IR_VALUE_PBC
#define IR_PLAYER_GOTO														IR_VALUE_GOTO
#define IR_PLAYER_ZOOM														IR_VALUE_ZOOM
#define IR_PLAYER_RDM														IR_VALUE_LOC
#define IR_PLAYER_SETUP														IR_VALUE_SETUP
#define IR_PLAYER_SUB_T														IR_VALUE_SUB_T
#define IR_PLAYER_SLOW														IR_VALUE_SLOW
#define IR_PLAYER_ANGLE														IR_VALUE_ANGLE
#define IR_PLAYER_TITLE														IR_VALUE_TITLE

#define IR_PLAYER_ADD10														IR_VALUE_ADD10

#define IR_PLAYER_ADD														IR_VALUE_ADD_VOL
#define IR_PLAYER_SUB														IR_VALUE_SUB_VOL

#define IR_PLAYER_RETURN													IR_VALUE_STOP
#define IR_PLAYER_STOP														IR_VALUE_STOP

/*---------------------------------------------------------------------------------------------------
	����
*---------------------------------------------------------------------------------------------------*/

#define IR_BT_M0															IR_VALUE_M0
#define IR_BT_M1															IR_VALUE_M1
#define IR_BT_M2															IR_VALUE_M2
#define IR_BT_M3															IR_VALUE_M3
#define IR_BT_M4															IR_VALUE_M4
#define IR_BT_M5															IR_VALUE_M5
#define IR_BT_M6															IR_VALUE_M6
#define IR_BT_M7															IR_VALUE_M7
#define IR_BT_M8															IR_VALUE_M8
#define IR_BT_M9															IR_VALUE_M9
#define IR_BT_WELL															IR_VALUE_NULL
#define IR_BT_DEL															IR_VALUE_NULL

#define IR_BT_PAUSE															IR_VALUE_PLAY
#define IR_BT_NEXT															IR_VALUE_NEXT
#define IR_BT_BACK															IR_VALUE_BACK
#define IR_BT_ANSWER														IR_VALUE_BAND
#define IR_BT_REJECT														IR_VALUE_MODE
#define IR_BT_REDIAL														IR_VALUE_BAND

/*---------------------------------------------------------------------------------------------------
	¼��
*---------------------------------------------------------------------------------------------------*/

#define IR_RECORD_PAUSE														IR_VALUE_PLAY
#define IR_RECORD_SAVE														IR_VALUE_M2
#define IR_RECORD_MODE														IR_VALUE_M3

/*---------------------------------------------------------------------------------------------------
	����
/---------------------------------------------------------------------------------------------------*/

#define IR_SETTING_SEL														IR_VALUE_SEL
#define IR_SETTING_ADD														IR_VALUE_ADD_VOL
#define IR_SETTING_SUB														IR_VALUE_SUB_VOL
#define IR_SETTING_MUTE														IR_VALUE_MUTE

#endif /* _IR_ORDER_MAP_*/


