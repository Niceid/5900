#ifndef _IR_ORDER_MAP_
#define _IR_ORDER_MAP_


#define IR_RESPONSE_NULL													0xAA

/*---------------------------------------------------------------------------------------------------
	系统
*---------------------------------------------------------------------------------------------------*/

#define IR_SYSTEM_POWER														IR_VALUE_POWER
#define IR_SYSTEM_MODE														IR_VALUE_MODE

#define IR1_SYSTEM_POWER													IR_VALUE1_POWER
#define IR1_SYSTEM_MODE														IR_VALUE1_MODE

#if defined(IR_POWER_MUTE)
#define IR_SYSTEM_POWER_MUTE												IR_VALUE_POWER_MUTE
#endif

#if defined(IR_POWER_MODE)
	#define IR_SYSTEM_POWER_MODE											IR_VALUE_POWER_MODE
#endif

/*---------------------------------------------------------------------------------------------------
	收音
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

#define IR1_RADIO_M1														IR_VALUE1_M1
#define IR1_RADIO_M2														IR_VALUE1_M2
#define IR1_RADIO_M3														IR_VALUE1_M3
#define IR1_RADIO_M4														IR_VALUE1_M4
#define IR1_RADIO_M5														IR_VALUE1_M5
#define IR1_RADIO_M6														IR_VALUE1_M6

#define IR1_RADIO_BAND														IR_VALUE1_BAND
#define IR1_RADIO_AMS														IR_VALUE1_AMS
#define IR1_RADIO_NEXT														IR_VALUE1_NEXT
#define IR1_RADIO_BACK														IR_VALUE1_BACK
#define IR1_RADIO_ST														IR_VALUE1_ST
#define IR1_RADIO_LOC														IR_VALUE1_LOC
#define IR1_RADIO_SCAN														IR_VALUE1_SCAN

#define IR1_RADIO_NEXT_STEP													IR_VALUE1_SEEK_UP
#define IR1_RADIO_BACK_STEP													IR_VALUE1_SEEK_DOWN

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

#define IR1_RDS_M1															IR_VALUE1_M1
#define IR1_RDS_M2															IR_VALUE1_M2
#define IR1_RDS_M3															IR_VALUE1_M3
#define IR1_RDS_M4															IR_VALUE1_M4
#define IR1_RDS_M5															IR_VALUE1_M5
#define IR1_RDS_M6															IR_VALUE1_M6

#define IR1_RDS_PTY															IR_VALUE1_BAND
#define IR1_RDS_NEXT														IR_VALUE1_NEXT
#define IR1_RDS_BACK														IR_VALUE1_BACK

#endif

/*---------------------------------------------------------------------------------------------------
	放音
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


#define IR1_PLAYER_M0														IR_VALUE1_M0
#define IR1_PLAYER_M1														IR_VALUE1_M1
#define IR1_PLAYER_M2														IR_VALUE1_M2
#define IR1_PLAYER_M3														IR_VALUE1_M3
#define IR1_PLAYER_M4														IR_VALUE1_M4
#define IR1_PLAYER_M5														IR_VALUE1_M5
#define IR1_PLAYER_M6														IR_VALUE1_M6
#define IR1_PLAYER_M7														IR_VALUE1_M7
#define IR1_PLAYER_M8														IR_VALUE1_M8
#define IR1_PLAYER_M9														IR_VALUE1_M9

#define IR1_PLAYER_NEXT														IR_VALUE1_NEXT
#define IR1_PLAYER_BACK														IR_VALUE1_BACK

#define IR1_PLAYER_ADD														IR_VALUE1_ADD_VOL
#define IR1_PLAYER_SUB														IR_VALUE1_SUB_VOL

#define IR1_PLAYER_ID3														IR_VALUE1_BAND
#define IR1_PLAYER_SCH														IR_VALUE1_AMS

#define IR1_PLAYER_SEL														IR_VALUE1_SEL

#define IR1_PLAYER_STOP														IR_VALUE1_STOP
#define IR1_PLAYER_RETURN													IR_VALUE1_STOP
#define IR1_PLAYER_PLAYPAUSE												IR_VALUE1_PLAY

#define IR1_PLAYER_TITLE													IR_VALUE1_TITLE
#define IR1_PLAYER_SETUP													IR_VALUE1_SETUP
#define IR1_PLAYER_PBC														IR_VALUE1_PBC
#define IR1_PLAYER_CHANNEL													IR_VALUE1_AUDIO

#define IR1_PLAYER_FORWARD													IR_VALUE1_SEEK_UP
#define IR1_PLAYER_BACKWARD													IR_VALUE1_SEEK_DOWN

#define IR1_PLAYER_UP														IR_VALUE1_UP
#define IR1_PLAYER_DOWN														IR_VALUE1_DOWN
#define IR1_PLAYER_LEFT														IR_VALUE1_LEFT
#define IR1_PLAYER_RIGHT													IR_VALUE1_RIGHT
#define IR1_PLAYER_ENTER													IR_VALUE1_ENTER

/*---------------------------------------------------------------------------------------------------
	蓝牙
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

#define IR1_BT_PAUSE														IR_VALUE1_PLAY
#define IR1_BT_NEXT															IR_VALUE1_NEXT
#define IR1_BT_BACK															IR_VALUE1_BACK
#define IR1_BT_ANSWER														IR_VALUE1_BAND
#define IR1_BT_REJECT														IR_VALUE1_NULL

/*---------------------------------------------------------------------------------------------------
	录音
*---------------------------------------------------------------------------------------------------*/

#define IR_RECORD_PAUSE														IR_VALUE_PLAY
#define IR_RECORD_SAVE														IR_VALUE_M2
#define IR_RECORD_MODE														IR_VALUE_M3

/*---------------------------------------------------------------------------------------------------
	设置
/---------------------------------------------------------------------------------------------------*/

#define IR_SETTING_SEL														IR_VALUE_SEL
#define IR_SETTING_ADD														IR_VALUE_ADD_VOL
#define IR_SETTING_SUB														IR_VALUE_SUB_VOL
#define IR_SETTING_MUTE														IR_VALUE_MUTE

#define IR1_SETTING_SEL														IR_VALUE1_SEL
#define IR1_SETTING_ADD														IR_VALUE1_ADD_VOL
#define IR1_SETTING_SUB														IR_VALUE1_SUB_VOL
#define IR1_SETTING_EQ														IR_VALUE1_EQ
#define IR1_SETTING_LOUD													IR_VALUE1_LOUD
#define IR1_SETTING_MUTE													IR_VALUE1_MUTE
#define IR1_SETTING_CLK														IR_VALUE1_CLK

#endif /* _IR_ORDER_MAP_*/


