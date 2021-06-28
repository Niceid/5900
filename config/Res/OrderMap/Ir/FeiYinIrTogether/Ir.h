#ifndef _ORDER_IR_
#define _ORDER_IR_


#define IR_RESPONSE_NULL													0xFF//0xAA

/*---------------------------------------------------------------------------------------------------
	系统
*---------------------------------------------------------------------------------------------------*/

#define IR_SYSTEM_POWER														IR_VALUE1_POWER
#define IR_SYSTEM_MODE														IR_VALUE1_MODE

#if defined(_ENABLE_RADIO_RDS_)
#define IR_RDS_AF															IR_VALUE1_M7
#define IR_RDS_TA															IR_VALUE1_M8
#endif
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
#define IR_RADIO_FORWARD													IR_VALUE1_FORWARD
#define IR_RADIO_BACKWARD													IR_VALUE1_BACKWARD
//#define IR_RADIO_UP_PRESET													IR_VALUE1_UP
//#define IR_RADIO_DOWN_PRESET												IR_VALUE1_DOWN
//#define IR_RADIO_PREV_PRESET												IR_VALUE1_LEFT
//#define IR_RADIO_NEXT_PRESET												IR_VALUE1_RIGHT

#define IR_RADIO_ST															IR_VALUE1_ST
#define IR_RADIO_LOC														IR_VALUE1_LOC
//#define IR_RADIO_SCAN														IR_VALUE1_MENU

#if defined(_ENABLE_RADIO_RDS_)

#define IR_RDS_M1															IR_VALUE1_M1
#define IR_RDS_M2															IR_VALUE1_M2
#define IR_RDS_M3															IR_VALUE1_M3
#define IR_RDS_M4															IR_VALUE1_M4
#define IR_RDS_M5															IR_VALUE1_M5
#define IR_RDS_M6															IR_VALUE1_M6

#define IR_RDS_PTY															IR_VALUE1_M9
#define IR_RDS_NEXT															IR_VALUE1_NEXT
#define IR_RDS_BACK															IR_VALUE1_BACK

#endif

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

#define IR_PLAYER_UP														IR_VALUE1_UP
#define IR_PLAYER_DOWN														IR_VALUE1_DOWN
#define IR_PLAYER_LEFT														IR_VALUE1_LEFT
#define IR_PLAYER_RIGHT														IR_VALUE1_RIGHT
#define IR_PLAYER_ENTER														IR_VALUE1_ENTER
#define IR_PLAYER_SEL														IR_VALUE1_SEL

#define IR_PLAYER_NEXT														IR_VALUE1_NEXT
#define IR_PLAYER_BACK														IR_VALUE1_BACK
#define IR_PLAYER_FORWARD													IR_VALUE1_FORWARD
#define IR_PLAYER_BACKWARD													IR_VALUE1_BACKWARD

#define IR_PLAYER_ID3														IR_VALUE1_BAND
#define IR_PLAYER_SCH														IR_VALUE1_AMS

#define IR_PLAYER_PLAYPAUSE													IR_VALUE1_PLAY
#define IR_PLAYER_STOP														IR_VALUE1_STOP

#define IR_PLAYER_RPT														IR_VALUE1_RPT

#define IR_PLAYER_CHANNEL													IR_VALUE1_ST
#define IR_PLAYER_OSD														IR_VALUE1_BAND 
#define IR_PLAYER_PBC														IR_VALUE1_MENU
#define IR_PLAYER_GOTO														IR_VALUE1_GOTO
#define IR_PLAYER_ZOOM														IR_VALUE1_ZOOM
#define IR_PLAYER_FORMAT													IR_VALUE1_LOC
#define IR_PLAYER_SETUP														IR_VALUE1_SETUP
#define IR_PLAYER_SUB_T														IR_VALUE1_SUB_T
#define IR_PLAYER_SLOW														IR_VALUE1_SLOW
#define IR_PLAYER_PROGRAM													IR_VALUE1_GOTO
#define IR_PLAYER_ANGLE														IR_VALUE1_ANGLE
#define IR_PLAYER_TITLE														IR_VALUE1_AMS

#define IR_PLAYER_ADD10														IR_VALUE1_M10
#define IR_PLAYER_ADD														IR_VALUE1_ADD_VOL
#define IR_PLAYER_SUB														IR_VALUE1_SUB_VOL

/*---------------------------------------------------------------------------------------------------
	蓝牙
*---------------------------------------------------------------------------------------------------*/

#define IR_BT_M0															IR_VALUE1_M0
#define IR_BT_M1															IR_VALUE1_M1
#define IR_BT_M2															IR_VALUE1_M2
#define IR_BT_M3															IR_VALUE1_M3
#define IR_BT_M4															IR_VALUE1_M4
#define IR_BT_M5															IR_VALUE1_M5
#define IR_BT_M6															IR_VALUE1_M6
#define IR_BT_M7															IR_VALUE1_M7
#define IR_BT_M8															IR_VALUE1_M8
#define IR_BT_M9															IR_VALUE1_M9
#define IR_BT_STAR															IR_VALUE1_M10
#define IR_BT_WELL															IR_VALUE1_MENU
#define IR_BT_DEL															IR_VALUE1_BACKWARD

#define IR_BT_PAUSE															IR_VALUE1_PLAY
#define IR_BT_NEXT															IR_VALUE1_NEXT
#define IR_BT_BACK															IR_VALUE1_BACK
#define IR_BT_ANSWER														IR_VALUE1_SLOW
#define IR_BT_REDIAL														IR_VALUE1_SLOW
#define IR_BT_REJECT														IR_VALUE1_CLK

/*---------------------------------------------------------------------------------------------------
	录音
*---------------------------------------------------------------------------------------------------*/

#define IR_RECORD_PAUSE														IR_VALUE1_PLAY
#define IR_RECORD_SAVE														IR_VALUE1_M2
#define IR_RECORD_MODE														IR_VALUE1_M3

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


///////////////-----IR2---////////////////////////////////////////////////

/*---------------------------------------------------------------------------------------------------
	系统
*---------------------------------------------------------------------------------------------------*/

#define IR2_SYSTEM_POWER														IR_VALUE2_POWER
#define IR2_SYSTEM_MODE														IR_VALUE2_MODE

#define IR2_SYSTEM_POWER_MUTE												IR_VALUE2_POWER_MUTE

/*---------------------------------------------------------------------------------------------------
	收音
/---------------------------------------------------------------------------------------------------*/

#define IR2_RADIO_M1															IR_VALUE2_M1
#define IR2_RADIO_M2															IR_VALUE2_M2
#define IR2_RADIO_M3															IR_VALUE2_M3
#define IR2_RADIO_M4															IR_VALUE2_M4
#define IR2_RADIO_M5															IR_VALUE2_M5
#define IR2_RADIO_M6															IR_VALUE2_M6

#define IR2_RADIO_BAND															IR_VALUE2_REJECT

#define IR2_RADIO_NEXT															IR_VALUE2_NEXT
#define IR2_RADIO_BACK															IR_VALUE2_BACK

#define IR2_RADIO_LOC															IR_VALUE2_M7
#define IR2_RADIO_ST															IR_VALUE2_M8
#define IR2_RADIO_AMS															IR_VALUE2_M9
#if defined(USE_FY_ADD_3KEY)  //新遥控多了几个按键如下
#define IR2_RADIO_AMS2                                                          IR_VALUE2_AMS
#define IR2_RADIO_TA                                                            IR_VALUE2_AMS
#endif
#if defined(_ENABLE_RADIO_RDS_)

#define IR2_RDS_M1																IR_VALUE2_M1
#define IR2_RDS_M2																IR_VALUE2_M2
#define IR2_RDS_M3																IR_VALUE2_M3
#define IR2_RDS_M4																IR_VALUE2_M4
#define IR2_RDS_M5																IR_VALUE2_M5
#define IR2_RDS_M6																IR_VALUE2_M6

//#define IR2_RDS_PTY																IR_VALUE1_M7
#define IR2_RDS_NEXT															IR_VALUE1_NEXT
#define IR2_RDS_BACK															IR_VALUE1_BACK

#endif


/*---------------------------------------------------------------------------------------------------
	放音
*---------------------------------------------------------------------------------------------------*/
#define IR2_PLAYER_M0														IR_VALUE2_M0
#define IR2_PLAYER_M1														IR_VALUE2_M1
#define IR2_PLAYER_M2														IR_VALUE2_M2
#define IR2_PLAYER_M3														IR_VALUE2_M3
#define IR2_PLAYER_M4														IR_VALUE2_M4
#define IR2_PLAYER_M5														IR_VALUE2_M5
#define IR2_PLAYER_M6														IR_VALUE2_M6
#define IR2_PLAYER_M7														IR_VALUE2_M7
#define IR2_PLAYER_M8														IR_VALUE2_M8
#define IR2_PLAYER_M9														IR_VALUE2_M9

	
#define IR2_PLAYER_PREV														IR_VALUE2_BACK
#define IR2_PLAYER_NEXT														IR_VALUE2_NEXT
#define IR2_PLAYER_PAUSE													IR_VALUE2_PLAY	
#define IR2_PLAYER_STOP														IR_VALUE2_REJECT
	
#define IR2_PLAYER_LEFT														IR_VALUE2_LEFT
#define IR2_PLAYER_RIGHT													IR_VALUE2_RIGHT
#define IR2_PLAYER_UP														IR_VALUE2_UP
#define IR2_PLAYER_DOWN														IR_VALUE2_DOWN
#define IR2_PLAYER_ENTER													IR_VALUE2_ENTER
	
#define IR2_PLAYER_STEUP													IR_VALUE2_M0
#define IR2_PLAYER_OSD														IR_VALUE2_M1
#define IR2_PLAYER_AUDIO													IR_VALUE2_M2
#define IR2_PLAYER_ANGLE													IR_VALUE2_M3
#define IR2_PLAYER_RPT														IR_VALUE2_M4
#define IR2_PLAYER_RDM														IR_VALUE2_M5
#define IR2_PLAYER_INT														IR_VALUE2_M6
#define IR2_PLAYER_SUB_T													IR_VALUE2_M7
#define IR2_PLAYER_TITLE													IR_VALUE2_M8
#define IR2_PLAYER_ZOOM														IR_VALUE2_M9
#define IR2_PLAYER_SLOW														IR_VALUE2_START
#define IR2_PLAYER_PBC														IR_VALUE2_WELL
#define IR2_PLAYER_SYS														IR_VALUE2_CLEAR
	
#define IR2_PLAYER_SEL														IR_RESPONSE_NULL
#define IR2_PLAYER_ADD														IR_VALUE2_ADD
#define IR2_PLAYER_SUB														IR_VALUE2_SUB
#if defined(USE_FY_ADD_3KEY) 
#define IR2_PLAYER_ADD10													IR_VALUE2_AMS
#endif
/*---------------------------------------------------------------------------------------------------
	电话
---------------------------------------------------------------------------------------------------*/
#define IR2_BT_ANSWER														IR_VALUE2_ANSWER
#define IR2_BT_REJECT														IR_VALUE2_REJECT
	
#define IR2_BT_M0															IR_VALUE2_M0
#define IR2_BT_M1															IR_VALUE2_M1
#define IR2_BT_M2															IR_VALUE2_M2
#define IR2_BT_M3															IR_VALUE2_M3
#define IR2_BT_M4															IR_VALUE2_M4
#define IR2_BT_M5															IR_VALUE2_M5
#define IR2_BT_M6															IR_VALUE2_M6
#define IR2_BT_M7															IR_VALUE2_M7
#define IR2_BT_M8															IR_VALUE2_M8
#define IR2_BT_M9															IR_VALUE2_M9
#define IR2_BT_WELL															IR_VALUE2_WELL
#define IR2_BT_STAR															IR_VALUE2_START
	
#define IR2_BT_PAUSE														IR_VALUE2_PLAY
#define IR2_BT_NEXT															IR_VALUE2_NEXT
#define IR2_BT_BACK															IR_VALUE2_BACK


#define IR2_BT_DEL															IR_VALUE2_CLEAR
#define IR2_BT_REDIAL														IR_VALUE2_ANSWER

/*---------------------------------------------------------------------------------------------------
	设置
/---------------------------------------------------------------------------------------------------*/

#define IR2_SETTING_SEL														IR_RESPONSE_NULL
#define IR2_SETTING_ADD														IR_VALUE2_ADD
#define IR2_SETTING_SUB														IR_VALUE2_SUB
#define IR2_SETTING_EQ_LOUD													IR_VALUE2_EQ_LOUD
#define IR2_SETTING_MUTE													IR_VALUE2_POWER_MUTE
#if defined(USE_FY_ADD_3KEY) 
#define IR2_SETTING_SEL2													IR_VALUE2_SEL
#define IR2_SETTING_CLK2													IR_VALUE2_CLK
#endif


///////////////-----IR3---////////////////////////////////////////////////

#if !defined(USE_FY_ANOTHER_REMOTE)
/*---------------------------------------------------------------------------------------------------
	系统
*---------------------------------------------------------------------------------------------------*/

#define IR3_SYSTEM_POWER_MUTE												IR_VALUE3_POWER_MUTE
#define IR3_SYSTEM_MODE														IR_VALUE3_MODE

/*---------------------------------------------------------------------------------------------------
	收音
/---------------------------------------------------------------------------------------------------*/
#define IR3_RADIO_M1															IR_VALUE3_M1
#define IR3_RADIO_M2															IR_VALUE3_M2
#define IR3_RADIO_M3															IR_VALUE3_M3
#define IR3_RADIO_M4															IR_VALUE3_M4
#define IR3_RADIO_M5															IR_VALUE3_M5
#define IR3_RADIO_M6															IR_VALUE3_M6

#define IR3_RADIO_BAND															IR_VALUE3_BAND
#define IR3_RADIO_AMS															IR_VALUE3_AMS
#define IR3_RADIO_NEXT															IR_VALUE3_RIGHT
#define IR3_RADIO_BACK															IR_VALUE3_LEFT
#define IR3_RADIO_NEXT_STEP														IR_VALUE3_RIGHT
#define IR3_RADIO_BACK_STEP														IR_VALUE3_LEFT
#define IR3_RADIO_ST															IR_VALUE3_SUB_T

#if defined(_ENABLE_RADIO_RDS_)

#define IR3_RDS_M1																IR_VALUE3_M1
#define IR3_RDS_M2																IR_VALUE3_M2
#define IR3_RDS_M3																IR_VALUE3_M3
#define IR3_RDS_M4																IR_VALUE3_M4
#define IR3_RDS_M5																IR_VALUE3_M5
#define IR3_RDS_M6																IR_VALUE3_M6

//#define IR2_RDS_PTY																IR_VALUE1_M7
#define IR3_RDS_NEXT															IR_VALUE1_NEXT
#define IR3_RDS_BACK															IR_VALUE1_BACK

#endif


/*---------------------------------------------------------------------------------------------------
	放音
*---------------------------------------------------------------------------------------------------*/
#define IR3_PLAYER_M0														IR_VALUE3_M0
#define IR3_PLAYER_M1														IR_VALUE3_M1
#define IR3_PLAYER_M2														IR_VALUE3_M2
#define IR3_PLAYER_M3														IR_VALUE3_M3
#define IR3_PLAYER_M4														IR_VALUE3_M4
#define IR3_PLAYER_M5														IR_VALUE3_M5
#define IR3_PLAYER_M6														IR_VALUE3_M6
#define IR3_PLAYER_M7														IR_VALUE3_M7
#define IR3_PLAYER_M8														IR_VALUE3_LEFT
#define IR3_PLAYER_M9														IR_VALUE3_RIGHT

#define IR3_PLAYER_MP3JPGMP4										        IR_VALUE3_BAND
#define IR3_PLAYER_FORMAT									                IR_VALUE3_BAND
#define IR3_PLAYER_SCH														IR_VALUE3_AMS
#define IR3_PLAYER_TITLE													IR_VALUE3_AMS

#define IR3_PLAYER_UP														IR_VALUE3_UP
#define IR3_PLAYER_DOWN														IR_VALUE3_DOWN
#define IR3_PLAYER_LEFT														IR_VALUE3_LEFT
#define IR3_PLAYER_RIGHT													IR_VALUE3_RIGHT
#define IR3_PLAYER_ENTER													IR_VALUE3_ENTER

#define IR3_PLAYER_SUB_T													IR_VALUE3_SUB_T
#define IR3_PLAYER_ST													IR_VALUE3_SUB_T


#define IR3_PLAYER_GOTO														IR_VALUE3_GOTO
#define IR3_PLAYER_STOP														IR_VALUE3_GOTO

#define IR3_PLAYER_ADD														IR_VALUE3_ADD
#define IR3_PLAYER_SUB														IR_VALUE3_SUB
	
/*---------------------------------------------------------------------------------------------------
	电话
---------------------------------------------------------------------------------------------------*/
#define IR3_BT_ANSWER														IR_VALUE3_BAND
//#define IR3_BT_REJECT														IR_VALUE3_MODE
	                                                    
#define IR3_BT_M0															IR_VALUE3_M0
#define IR3_BT_M1															IR_VALUE3_M1
#define IR3_BT_M2															IR_VALUE3_M2
#define IR3_BT_M3															IR_VALUE3_M3
#define IR3_BT_M4															IR_VALUE3_M4
#define IR3_BT_M5															IR_VALUE3_M5
#define IR3_BT_M6															IR_VALUE3_M6
#define IR3_BT_M7															IR_VALUE3_M7
#define IR3_BT_M8															IR_VALUE3_LEFT
#define IR3_BT_M9															IR_VALUE3_RIGHT
#define IR3_BT_WELL															IR_VALUE3_UP
#define IR3_BT_STAR															IR_VALUE3_DOWN
	                                                    
#define IR3_BT_PAUSE														IR_VALUE3_M1
#define IR3_BT_NEXT															IR_VALUE3_RIGHT
#define IR3_BT_BACK															IR_VALUE3_LEFT
                                                      
                                                      
#define IR3_BT_DEL															IR_VALUE3_MODE
//#define IR3_BT_REDIAL														IR_VALUE3_ANSWER

/*---------------------------------------------------------------------------------------------------
	设置
/---------------------------------------------------------------------------------------------------*/

#define IR3_SETTING_SEL													IR_RESPONSE_NULL
#define IR3_SETTING_ADD													IR_VALUE3_ADD
#define IR3_SETTING_SUB													IR_VALUE3_SUB
#define IR3_SETTING_MUTE												IR_VALUE3_POWER_MUTE
#define IR3_SETTING_CLK													IR_VALUE3_ENTER

#else
/*---------------------------------------------------------------------------------------------------
	系统
*---------------------------------------------------------------------------------------------------*/

#define IR3_SYSTEM_POWER														IR_VALUE3_POWER
#define IR3_SYSTEM_MODE														IR_VALUE3_MODE

#if defined(IR_POWER_MUTE)
#define IR3_SYSTEM_POWER_MUTE												IR_VALUE3_POWER_MUTE
#endif

#if defined(IR_POWER_MODE)
	#define IR3_SYSTEM_POWER_MODE											IR_VALUE3_POWER_MODE
#endif

#if defined(_ENABLE_RADIO_RDS_)
#define IR3_SYSTEM_AF														IR_VALUE3_M7
#define IR3_SYSTEM_TA														IR_VALUE3_START
#endif

/*---------------------------------------------------------------------------------------------------
	收音
/---------------------------------------------------------------------------------------------------*/

#define IR3_RADIO_M1															IR_VALUE3_M1
#define IR3_RADIO_M2															IR_VALUE3_M2
#define IR3_RADIO_M3															IR_VALUE3_M3
#define IR3_RADIO_M4															IR_VALUE3_M4
#define IR3_RADIO_M5															IR_VALUE3_M5
#define IR3_RADIO_M6															IR_VALUE3_M6
                                                           
#define IR3_RADIO_BAND														IR_VALUE3_BAND
#define IR3_RADIO_AMS														 	IR_VALUE3_AMS
#define IR3_RADIO_NEXT														IR_VALUE3_NEXT
#define IR3_RADIO_BACK														IR_VALUE3_BACK
#define IR3_RADIO_ST															IR_VALUE3_ST
#define IR3_RADIO_LOC															IR_VALUE3_LOC
#define IR3_RADIO_SCAN														IR_VALUE3_SCAN
                                                           
#if defined(_ENABLE_RADIO_RDS_)                            
                                                           
#define IR3_RDS_M1																IR_VALUE3_M1
#define IR3_RDS_M2																IR_VALUE3_M2
#define IR3_RDS_M3																IR_VALUE3_M3
#define IR3_RDS_M4																IR_VALUE3_M4
#define IR3_RDS_M5																IR_VALUE3_M5
#define IR3_RDS_M6																IR_VALUE3_M6
                                                           
#define IR3_RDS_PTY																IR_VALUE3_WELL
#define IR3_RDS_NEXT															IR_VALUE3_NEXT
#define IR3_RDS_BACK															IR_VALUE3_BACK
                                                           
#endif

/*---------------------------------------------------------------------------------------------------
	放音
*---------------------------------------------------------------------------------------------------*/

#define IR3_PLAYER_M0														IR_VALUE3_CLK
#define IR3_PLAYER_M1														IR_VALUE3_M1
#define IR3_PLAYER_M2														IR_VALUE3_M2
#define IR3_PLAYER_M3														IR_VALUE3_M3
#define IR3_PLAYER_M4														IR_VALUE3_M4
#define IR3_PLAYER_M5														IR_VALUE3_M5
#define IR3_PLAYER_M6														IR_VALUE3_M6
#define IR3_PLAYER_M7														IR_VALUE3_M7
#define IR3_PLAYER_M8														IR_VALUE3_BACK
#define IR3_PLAYER_M9														IR_VALUE3_NEXT
                                                         
#define IR3_PLAYER_NEXT														IR_VALUE3_NEXT
#define IR3_PLAYER_BACK														IR_VALUE3_BACK
                                                         
#define IR3_PLAYER_ADD														IR_VALUE3_ADD_VOL
#define IR3_PLAYER_SUB														IR_VALUE3_SUB_VOL
                                                         
#define IR3_PLAYER_MP3JPGMP4										        IR_VALUE3_BAND
#define IR3_PLAYER_FORMAT									                IR_VALUE3_BAND
#define IR3_PLAYER_SCH														IR_VALUE3_AMS
#define IR3_PLAYER_TITLE													IR_VALUE3_AMS
                                                         
#define IR3_PLAYER_ENTER													IR_VALUE3_SEL

/*---------------------------------------------------------------------------------------------------
	蓝牙
*---------------------------------------------------------------------------------------------------*/

#define IR3_BT_M0															IR_VALUE3_CLK
#define IR3_BT_M1															IR_VALUE3_M1
#define IR3_BT_M2															IR_VALUE3_M2
#define IR3_BT_M3															IR_VALUE3_M3
#define IR3_BT_M4															IR_VALUE3_M4
#define IR3_BT_M5															IR_VALUE3_M5
#define IR3_BT_M6															IR_VALUE3_M6
#define IR3_BT_M7															IR_VALUE3_M7
#define IR3_BT_M8															IR_VALUE3_BACK
#define IR3_BT_M9															IR_VALUE3_NEXT
#define IR3_BT_STAR														IR_VALUE3_START
#define IR3_BT_WELL														IR_VALUE3_WELL
#define IR3_BT_DEL														IR_VALUE3_MODE
                                                       
#define IR3_BT_PAUSE													IR_VALUE3_M1
#define IR3_BT_NEXT														IR_VALUE3_NEXT
#define IR3_BT_BACK														IR_VALUE3_BACK
#define IR3_BT_ANSWER													IR_VALUE3_BAND
#define IR3_BT_REJECT													IR_VALUE3_MODE
           
/*---------------------------------------------------------------------------------------------------
	录音
*---------------------------------------------------------------------------------------------------*/

#define IR3_RECORD_PAUSE													IR_VALUE3_PLAY
#define IR3_RECORD_SAVE														IR_VALUE3_M2
#define IR3_RECORD_MODE														IR_VALUE3_M3
           
/*---------------------------------------------------------------------------------------------------
	设置
/---------------------------------------------------------------------------------------------------*/

#define IR3_SETTING_SEL														IR_VALUE3_SEL
#define IR3_SETTING_ADD														IR_VALUE3_ADD_VOL
#define IR3_SETTING_SUB														IR_VALUE3_SUB_VOL
#define IR3_SETTING_EQ														IR_VALUE3_EQ
#define IR3_SETTING_LOUD													IR_VALUE3_LOUD
#define IR3_SETTING_MUTE													IR_VALUE3_MUTE
#define IR3_SETTING_CLK														IR_VALUE3_CLK



#endif

#endif
