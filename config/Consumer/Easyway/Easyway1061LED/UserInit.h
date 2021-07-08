#ifndef _USER_INIT_H_
#define _USER_INIT_H_




#define __FIRM_INFO__								"EASYWAY"
#define __PRODUCT_INFO__							"easyway1061_7pinled"
#define __PRODUCT_EASYWAY_DEVELOPMENT__


/*
	系统功能支持
*/
#define _SUPPORT_RADIO_
#define _SUPPORT_PLAYER_
#define _SUPPORT_AUX_
#define _SUPPORT_BT_
//#define _SUPPORT_USB_PC_



#define TD_ADD_KB1069

//#define	TD_ADD_LCD_TEST             //LCD屏调试
//#define  use_usb_as_uart    			//使用USB端口作为串口调试
#define  use_mic_as_uart    			//使用mic端口作为串口调试


#define	TD_ADD_USER_LCD	            //使用用户真值表
#define DRIVE_LCD_LED				//支持LCD或LED直驱


#define DRIVE_LED_7pin_001

#if defined(TD_ADD_KB1069)
//#define SUPPORT_CALL_NAMES	1//支持开机语音播报
#define TD_ADD_BANDCH			//显示BANDCH
#define TD_ADD_LCD_NUM_4		//4个数码管
#define TD_ADD_AUX_IN_MCU		//AUX进入MCU
#endif
/*---------------------------------------------------
	系统
---------------------------------------------------*/
#define _EASYWAY_MODIFY_			//	威益德公司特殊定义

#define _SUPPORT_ACC_				//	支持ACC控制
//#define _SUPPORT_DTH_				//	支持DTH控制

#define _SUPPORT_AREA_       		//支持区域选择功能
#if defined(_SUPPORT_AREA_)
	#define SYS_DEFAULT_AREA		SETUP_AREA_EUR
#endif

//#define _ENABLE_I2C_DRV_

/*---------------------------------------------------
	音效处理
---------------------------------------------------*/
//	音效处理芯片驱动
#define _SUPPORT_AUDIO_DRIVER_
#if defined(_SUPPORT_AUDIO_DRIVER_)
	#define _AUDIO_DRIVER_PT2313_

	#ifndef _ENABLE_I2C_DRV_
		#define _ENABLE_I2C_DRV_
	#endif

#else
	#define AUDIO_VOLUME_MAX			30
	#define AUDIO_VOLUME_DEFAULT		12
#endif

#define _ENABLE_VBAR_				//	支持频谱显示
//#define _ENABLE_BEEP_				//	支持BEEP输出

//#define _SUPPORT_SUBWOOFER_	    	//	支持重低音

#if defined(_ENABLE_VBAR_)

	#define VBAR_DC									(10)

	#define AUDIO_AMP_ATTENUATION_OCTUPLE			(1)
	#define	AUDIO_AMP_ATTENUATION_FOURFOLD			(2)
	#define	AUDIO_AMP_ATTENUATION_DOUBLENESS		(3)
	#define	AUDIO_AMP_ZERO							(4)
	#define	AUDIO_PLUS_DOUBLENESS					(5)
	#define	AUDIO_PLUS_FOURFOLD						(6)
	#define	AUDIO_PLUS_OCTUPLE						(7)

	#define AUDIO_BAR_VALUE_TYPE					AUDIO_AMP_ATTENUATION_DOUBLENESS

#endif

/*---------------------------------------------------
	按键功能
---------------------------------------------------*/
//	按键扫描驱动方式
#define _KEY_DRIVER_ADC_
#if defined(_KEY_DRIVER_ADC_)
	//#define _SCAN_SINGLE_ADC_		//	单ADC
#endif

//	支持编码器使用
#define _SUPPORT_VR_
#if defined(_SUPPORT_VR_)
	//	方向改变
	//#define _VR_SCAN_CHANGE_

	//	VR类型
	//#define _VR_SEG_CODE_SIGNAL_
	#define _VR_SEC_CODE_DOUBLE_
#endif
111
/*---------------------------------------------------
	收音功能
---------------------------------------------------*/
#if defined(_SUPPORT_RADIO_)

	//	收音高频头
	#define _TUNER_DRIVER_AC69xx_
	#define _TUNER_DRIVER_SI473x_
	#define _TUNER_DRIVER_SI470x_
	#define _TUNER_DRIVER_EW730_
	#define _TUNER_DRIVER_QN8035_
	//#define _TUNER_DRIVER_QN8065_
	//#define _TUNER_DRIVER_RDA580x_
	//#define _TUNER_DRIVER_AR1019_
	//#define _TUNER_DRIVER_LT4730_
	//#define _TUNER_DRIVER_TEA685x_
	//#define _TUNER_DRIVER_TDA778x_

	#if defined(_TUNER_DRIVER_AC69xx_)

	#endif

	#if defined(_TUNER_DRIVER_SI473x_)

		#define SI473x_TUNER_MONO_VALUE 				(5)
		#define SI473x_TUNER_STEREO_VALUE				(10)

		#define SI473x_TUNER_RSSI_MONO_VALUE			(5)
		#define SI473x_TUNER_RSSI_STEREO_VALUE			(10)

		#define SI473x_FM_SOFT_MUTE_ATTENUATION 		(25)
		#define SI473x_FM_SOFT_MUTE_SNR 				(3)
		#define SI473x_TUNER_RSSI_FM_VALUE				(21)

		#define SI473x_TUNER_RSSI_AM_VALUE				(30)// 40时是不带RDS
		#define SI473x_AM_SOFT_MUTE_ATTENUATION 		(25)
		#define SI473x_AM_SOFT_MUTE_SNR 				(10)

		#ifndef _TUNER_DRIVER_RESET_
			#define _TUNER_DRIVER_RESET_
		#endif
		#ifndef _ENABLE_I2C_DRV_
			#define _ENABLE_I2C_DRV_
		#endif
	#endif

	#if defined(_TUNER_DRIVER_SI470x_)


		#ifndef _TUNER_DRIVER_RESET_
			#define _TUNER_DRIVER_RESET_
		#endif
		#ifndef _ENABLE_I2C_DRV_
			#define _ENABLE_I2C_DRV_
		#endif
	#endif

	#if defined(_TUNER_DRIVER_QN8035_)

		#ifndef _ENABLE_I2C_DRV_
			#define _ENABLE_I2C_DRV_
		#endif
	#endif

#if defined(_TUNER_DRIVER_QN8065_)

	#ifndef _ENABLE_I2C_DRV_
		#define _ENABLE_I2C_DRV_
	#endif
#endif
	#if defined(_TUNER_DRIVER_RDA580x_)

		#ifndef _ENABLE_I2C_DRV_
			#define _ENABLE_I2C_DRV_
		#endif
	#endif

	//#define _ENABLE_RADIO_RDS_			//	开收音RDS功能
	#if defined(_ENABLE_RADIO_RDS_)

	#endif

	#define RADIO_FM_BAND_SIZE_MAX				(3)
	#define RADIO_AM_BAND_SIZE_MAX				(2)
	#define RADIO_BAND_SIZE_MAX					(RADIO_FM_BAND_SIZE_MAX + RADIO_AM_BAND_SIZE_MAX)
	#define RADIO_MEMORY_SIZE_MAX				(6)
	#define RADIO_FREQ_SIZE_MAX					(1+RADIO_MEMORY_SIZE_MAX)


	#if defined(_SUPPORT_AREA_)

		#define RADIO_AREA_EUR_LIST		\
				AREA_FM_EUR,			\
				AREA_FM_EUR,			\
				AREA_FM_EUR,			\
				AREA_AM_EUR,			\
				AREA_AM_EUR,


		#define RADIO_AREA_USA_LIST		\
				AREA_FM_USA,			\
				AREA_FM_USA,			\
				AREA_FM_USA,			\
				AREA_AM_USA,			\
				AREA_AM_USA,

	#endif
	#define _SUPPORT_REAL_TIME_REFRESH_FREQ_	//实时刷新频率显示 防止自动搜台显示跟不上的问题
#endif

/*---------------------------------------------------
	放音功能
---------------------------------------------------*/
#if defined(_SUPPORT_PLAYER_)

	#define _SUPPORT_USB_
	#define _SUPPORT_SD_

	#define _SUPPORT_ID3_

#endif

/*---------------------------------------------------
	调试选择
---------------------------------------------------*/
#if defined(_SUPPORT_DEBUG_)||defined(use_mic_as_uart)||defined(use_usb_as_uart)

	#define __EW_DEBUG__

#endif

/*---------------------------------------------------
	面板选择
---------------------------------------------------*/
#define LCD_MAP_NAME					EASYWAY1061_led7pin     // 7pin led
#define KEY_MAP_NAME					EasyWay
#define ORDER_KEY_MAP_NAME				EasyWay

/*
	显示驱动
*/
#define _UI_DRIVER_VFD_

#if defined(_UI_DRIVER_VFD_)
	//#define _VFD_DRIVER_IC_PT1621_
	//#define _VFD_DRIVER_IC_LC75823_
	//#define _VFD_DRIVER_IC_LC75824_
#endif

/*---------------------------------------------------
	遥控选择
---------------------------------------------------*/
#define IR_MAP_NAME						DemoSmall//DemoBig//
#define ORDER_IR_MAP_NAME				DemoSmall//DemoBig//
/*---------------------------------------------------
	引脚选择
---------------------------------------------------*/
#define PIN_MAP_NAME					easyway_7pin_led

/*---------------------------------------------------
	设置选择
---------------------------------------------------*/
#if defined(_SUPPORT_AREA_)
#define COMM_FUNC_OP_AREA 				FUNC_SEL_AREA,

#define COMMON_SETTING_AREA_LIST		\
		SETUP_AREA_EUR,					\
		SETUP_AREA_USA,

#else
#define COMM_FUNC_OP_AREA
#endif

//	重低音选择设置
#if defined(_SUPPORT_SUBWOOFER_)
#define COMM_FUNC_OP_SUBWOOFER 			FUNC_SEL_SUBWOOFER,
#else
#define COMM_FUNC_OP_SUBWOOFER
#endif

//	BEEP 音选择设置
#if defined(_ENABLE_BEEP_)
#define COMM_FUNC_OP_BEEP				FUNC_SEL_BEEP
#else
#define COMM_FUNC_OP_BEEP
#endif

//	RDS 功能选择设置
#if defined(_ENABLE_RADIO_RDS_)
#define COMM_FUNC_OP_RDS				\
		FUNC_RDS_AF,					\
		FUNC_RDS_TA,					\
		FUNC_RDS_PTY,					\
		FUNC_RDS_REG,					\
		FUNC_RDS_EON,					\
		FUNC_RDS_L_S,					\
		FUNC_RDS_MASK, 					\
		FUNC_RDS_ALARM,					\
		FUNC_RDS_TIME,

#else
#define COMM_FUNC_OP_RDS
#endif

/*************************************************************************/
#if !defined(_SUPPORT_AUDIO_DRIVER_)

#define COMMON_SETTING_FUNC_LIST		\
		FUNC_SEL_VOLUME,				\
	    FUNC_SEL_BASS,                  \
	    FUNC_SEL_TREBLE,                \
		FUNC_SEL_BAL,					\
	    FUNC_SEL_LOUD,                  \
		FUNC_SEL_EQ,					\
		FUNC_SEL_LOC,					\
		FUNC_SEL_ST

#else

#define COMMON_SETTING_FUNC_LIST		\
		FUNC_SEL_VOLUME,				\
		FUNC_SEL_BASS,					\
		FUNC_SEL_TREBLE,				\
		FUNC_SEL_BAL,					\
		FUNC_SEL_FAD,					\
		FUNC_SEL_LOUD,					\
		FUNC_SEL_EQ,					\
		FUNC_SEL_LOC,					\
		FUNC_SEL_ST

#endif

#define COMMON_FUNC_FUNC_LIST			\
		COMM_FUNC_OP_RDS				\
		COMM_FUNC_OP_SUBWOOFER			\
		COMM_FUNC_OP_AREA				\
		COMM_FUNC_OP_BEEP


#define COMMON_SETTING_EQ_LIST			\
		AUDIO_EQ_OFF,					\
	    AUDIO_EQ_POP,                   \
		AUDIO_EQ_CLASSIC,				\
		AUDIO_EQ_ROCK,



/*---------------------------------------------------
	特殊功能配置
----------------------------------------------------*/
#define  no_disp_87_5            //修改存台时直接驱动led会显示87.5频率
#define TD_ADD_AUX_IN_MCU        //使用mcu的aux
#define _AUX_REDUCE_AUD_GATE_       //降低AUX输出增益
//#define ICON_FM_POINT_TOGETHER      //图标FM和数字点一起

//#define _ENABLE_DISK_DISC_TO_FLASH_		//放音时碟片图标转动

#define ADC_USER_VOLTAGE_FLUCTUATION_   //adc涡轮采集值波动处理

#endif /* _USER_INIT_H_ */

