#ifndef _USER_INIT_H_
#define _USER_INIT_H_

#define __FIRM_INFO__								"EvbDemo1"
#define __PRODUCT_INFO__							"EvbDemo1"
#define __PRODUCT_DEMO__


/*
	系统功能支持
*/
#define _SUPPORT_RADIO_
#define _SUPPORT_PLAYER_
#define _SUPPORT_AUX_
#define _SUPPORT_BT_
//#define DAB_MODLE               //dab和收音单独模式
//#define _SUPPORT_USB_PC_
#if	defined(DAB_MODLE)
//#define	 DAB_USER_UART_DMA
#define	TD_ADD_W2DAB_UART		1
#define DAB_MAX_RADIO_NUM       100       //单个数组的最大容量
#define DAB_MAX_RADIO_NUM_ALL   DAB_MAX_RADIO_NUM   //DAB总共的节目总数

//#define YHX_W2DAB_FIRST_SEEK  //第一次上电自动搜台
#endif

//#define use_usb_as_uart   				//使用USB端口作为串口调试
//#define _SUPPORT_DEBUG_				//支持调试功能
//#define _SUPPORT_PB6PB7_			//支持调试功能

//#define _SUPPORT_PANEL_LIGHT_		//	支持七彩灯
#if defined(_SUPPORT_PANEL_LIGHT_)
	#define _SUPPORT_PANEL_PWM_		//	支持PWM 七彩灯
#endif

//#define	TD_ADD_USER_LCD	         //使用用户真值表
//#define TD_ADD_LCD_TEST                     //LCD屏调试

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
	
	#define AUDIO_VOLUME_DEFAULT		16

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

	#if defined(_TUNER_DRIVER_AC69xx_)

	#endif

	#if defined(_TUNER_DRIVER_SI473x_)

		#define SI473x_TUNER_MONO_VALUE 				(5)
		#define SI473x_TUNER_STEREO_VALUE				(10)

		#define SI473x_TUNER_RSSI_MONO_VALUE			(5)
		#define SI473x_TUNER_RSSI_STEREO_VALUE			(10)

		#define SI473x_FM_SOFT_MUTE_ATTENUATION 		(25)
		#define SI473x_FM_SOFT_MUTE_SNR 				(3)
		#define SI473x_TUNER_RSSI_FM_VALUE				(25)

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
	
    #if defined(_TUNER_DRIVER_EW730_)
    
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
#if defined(_SUPPORT_DEBUG_)||defined(use_usb_as_uart)
	
	#define __EW_DEBUG__
	
#endif


/*---------------------------------------------------
	面板选择
---------------------------------------------------*/
#define LCD_MAP_NAME					EvbDemo2
#define KEY_MAP_NAME					EasyWay
#define ORDER_KEY_MAP_NAME				EasyWay

/*
	显示驱动
*/
//#define _UI_DRIVER_VFD_
#define _UI_DRIVER_LCD_

#if defined(_UI_DRIVER_LCD_)
	#define _LCD_DRIVER_A4901_			//

#elif defined(_UI_DRIVER_VFD_)
	
	#define _VFD_DRIVER_IC_PT1621_
	//#define _VFD_DRIVER_IC_LC75823_
	//#define _VFD_DRIVER_IC_LC75824_
#endif

/*---------------------------------------------------
	遥控选择
---------------------------------------------------*/
#define IR_MAP_NAME						DemoSmall
#define ORDER_IR_MAP_NAME				DemoSmall

/*---------------------------------------------------
	引脚选择
---------------------------------------------------*/
#define PIN_MAP_NAME					EvbDemo

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
		FUNC_RDS_PTY

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
#if defined(_SUPPORT_PANEL_LIGHT_)

#define COMMON_SETTING_FUNC_LIST		\
	    FUNC_SEL_7COLOR,				\
		FUNC_SEL_VOLUME,				\
		FUNC_SEL_BASS,					\
		FUNC_SEL_TREBLE,				\
		FUNC_SEL_BAL,					\
		FUNC_SEL_FAD,					\
		FUNC_SEL_LOUD,					\
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
#endif

#define COMMON_FUNC_FUNC_LIST			\
		COMM_FUNC_OP_RDS				\
		COMM_FUNC_OP_SUBWOOFER			\
		COMM_FUNC_OP_BEEP


#define COMMON_SETTING_EQ_LIST			\
		AUDIO_EQ_OFF,					\
	    AUDIO_EQ_POP,                   \
		AUDIO_EQ_ROCK,					\
		AUDIO_EQ_CLASSIC
		
#if defined(_SUPPORT_PANEL_LIGHT_)
#define COMMON_SETTING_COLOR			\
		FUNC_SEL_AUTO,					\
		FUNC_SEL_BLUE,					\
		FUNC_SEL_GREEN, 				\
		FUNC_SEL_CYAN,					\
		FUNC_SEL_RED,					\
		FUNC_SEL_FUCHSINE,				\
		FUNC_SEL_YELLOW,				\
		FUNC_SEL_WHITE
	
#endif

/*---------------------------------------------------
	特殊功能配置
----------------------------------------------------*/
#if defined(_SUPPORT_PANEL_LIGHT_)
#define ENABLE_COLOP_SETTING        //修改菜单顺序
#endif

#if defined(_ENABLE_RADIO_RDS_)
#define _ENABLE_RDS_VOLUME_        //RDS音量跟其它音量保持一致
#define _DISABLE_MODE_FUNTION_     //收到RDS后，按MODE键不要有作用
#define _DISABLE_SYSTEM_MODE_      //收到RDS后，插U盘和SD卡不要转模式
#define _DISABLE_RDS_REPLACE_      //自动搜到AF电台不要自动保存到预存的电台上
#define _ENABLE_BT_FIRST_          //修改蓝牙优先级比交通台高
#define _DISABLE_RDS_MENMORY_      //不保存RDS信息
#define _CLEAN_RDS_PSN_
#endif

#if	defined(DAB_MODLE)
#define _ENABLE_RADIO_MUTE_        // 修改退出收音时MUTE掉收音输出
#endif

#define _ENABLE_SAVE_FREQ_         // 修改CH 键的存台功能

#define _FIVE_CHAR_DISPLAY_         //五个米吧显示

//#define ID3_RDS_TOGETHER            //ID3死机问题

#define  _FEIYIN_473x_C40_ADJUST_   //解决FM80-100db波形失真

#define _AUX_REDUCE_AUD_GATE_       //降低AUX输出增益

#endif /* _USER_INIT_H_ */

