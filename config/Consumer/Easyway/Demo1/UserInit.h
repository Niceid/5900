#ifndef _USER_INIT_H_
#define _USER_INIT_H_

#define __FIRM_INFO__								"EvbDemo1"
#define __PRODUCT_INFO__							"EvbDemo1"
#define __PRODUCT_DEMO__


/*
	ϵͳ����֧��
*/
#define _SUPPORT_RADIO_
#define _SUPPORT_PLAYER_
#define _SUPPORT_AUX_
#define _SUPPORT_BT_
//#define DAB_MODLE               //dab����������ģʽ
//#define _SUPPORT_USB_PC_
#if	defined(DAB_MODLE)
//#define	 DAB_USER_UART_DMA
#define	TD_ADD_W2DAB_UART		1
#define DAB_MAX_RADIO_NUM       100       //����������������
#define DAB_MAX_RADIO_NUM_ALL   DAB_MAX_RADIO_NUM   //DAB�ܹ��Ľ�Ŀ����

//#define YHX_W2DAB_FIRST_SEEK  //��һ���ϵ��Զ���̨
#endif

//#define use_usb_as_uart   				//ʹ��USB�˿���Ϊ���ڵ���
//#define _SUPPORT_DEBUG_				//֧�ֵ��Թ���
//#define _SUPPORT_PB6PB7_			//֧�ֵ��Թ���

//#define _SUPPORT_PANEL_LIGHT_		//	֧���߲ʵ�
#if defined(_SUPPORT_PANEL_LIGHT_)
	#define _SUPPORT_PANEL_PWM_		//	֧��PWM �߲ʵ�
#endif

//#define	TD_ADD_USER_LCD	         //ʹ���û���ֵ��
//#define TD_ADD_LCD_TEST                     //LCD������

/*---------------------------------------------------
	ϵͳ
---------------------------------------------------*/
#define _EASYWAY_MODIFY_			//	����¹�˾���ⶨ��

#define _SUPPORT_ACC_				//	֧��ACC����
//#define _SUPPORT_DTH_				//	֧��DTH����

#define _SUPPORT_AREA_       		//֧������ѡ����
#if defined(_SUPPORT_AREA_)
	#define SYS_DEFAULT_AREA		SETUP_AREA_EUR
#endif

//#define _ENABLE_I2C_DRV_

/*---------------------------------------------------
	��Ч����
---------------------------------------------------*/
//	��Ч����оƬ����
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

#define _ENABLE_VBAR_				//	֧��Ƶ����ʾ
//#define _ENABLE_BEEP_				//	֧��BEEP���

//#define _SUPPORT_SUBWOOFER_	    	//	֧���ص���

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
	��������
---------------------------------------------------*/
//	����ɨ��������ʽ
#define _KEY_DRIVER_ADC_
#if defined(_KEY_DRIVER_ADC_)
	//#define _SCAN_SINGLE_ADC_		//	��ADC
#endif

//	֧�ֱ�����ʹ��
#define _SUPPORT_VR_
#if defined(_SUPPORT_VR_)
	//	����ı�
	//#define _VR_SCAN_CHANGE_

	//	VR����
	//#define _VR_SEG_CODE_SIGNAL_
	#define _VR_SEC_CODE_DOUBLE_
#endif

/*---------------------------------------------------
	��������
---------------------------------------------------*/
#if defined(_SUPPORT_RADIO_)

	//	������Ƶͷ
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

		#define SI473x_TUNER_RSSI_AM_VALUE				(30)// 40ʱ�ǲ���RDS
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

	//#define _ENABLE_RADIO_RDS_			//	������RDS����

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
	#define _SUPPORT_REAL_TIME_REFRESH_FREQ_	//ʵʱˢ��Ƶ����ʾ ��ֹ�Զ���̨��ʾ�����ϵ�����
#endif

/*---------------------------------------------------
	��������
---------------------------------------------------*/
#if defined(_SUPPORT_PLAYER_)

	#define _SUPPORT_USB_
	#define _SUPPORT_SD_

	#define _SUPPORT_ID3_

#endif

/*---------------------------------------------------
	����ѡ��
---------------------------------------------------*/
#if defined(_SUPPORT_DEBUG_)||defined(use_usb_as_uart)
	
	#define __EW_DEBUG__
	
#endif


/*---------------------------------------------------
	���ѡ��
---------------------------------------------------*/
#define LCD_MAP_NAME					EvbDemo2
#define KEY_MAP_NAME					EasyWay
#define ORDER_KEY_MAP_NAME				EasyWay

/*
	��ʾ����
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
	ң��ѡ��
---------------------------------------------------*/
#define IR_MAP_NAME						DemoSmall
#define ORDER_IR_MAP_NAME				DemoSmall

/*---------------------------------------------------
	����ѡ��
---------------------------------------------------*/
#define PIN_MAP_NAME					EvbDemo

/*---------------------------------------------------
	����ѡ��
---------------------------------------------------*/
#if defined(_SUPPORT_AREA_)
#define COMM_FUNC_OP_AREA 				FUNC_SEL_AREA,

#define COMMON_SETTING_AREA_LIST		\
		SETUP_AREA_EUR,					\
		SETUP_AREA_USA,

#else
#define COMM_FUNC_OP_AREA
#endif

//	�ص���ѡ������
#if defined(_SUPPORT_SUBWOOFER_)
#define COMM_FUNC_OP_SUBWOOFER 			FUNC_SEL_SUBWOOFER,
#else
#define COMM_FUNC_OP_SUBWOOFER
#endif

//	BEEP ��ѡ������
#if defined(_ENABLE_BEEP_)
#define COMM_FUNC_OP_BEEP				FUNC_SEL_BEEP
#else
#define COMM_FUNC_OP_BEEP
#endif

//	RDS ����ѡ������
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
	���⹦������
----------------------------------------------------*/
#if defined(_SUPPORT_PANEL_LIGHT_)
#define ENABLE_COLOP_SETTING        //�޸Ĳ˵�˳��
#endif

#if defined(_ENABLE_RADIO_RDS_)
#define _ENABLE_RDS_VOLUME_        //RDS������������������һ��
#define _DISABLE_MODE_FUNTION_     //�յ�RDS�󣬰�MODE����Ҫ������
#define _DISABLE_SYSTEM_MODE_      //�յ�RDS�󣬲�U�̺�SD����Ҫתģʽ
#define _DISABLE_RDS_REPLACE_      //�Զ��ѵ�AF��̨��Ҫ�Զ����浽Ԥ��ĵ�̨��
#define _ENABLE_BT_FIRST_          //�޸��������ȼ��Ƚ�̨ͨ��
#define _DISABLE_RDS_MENMORY_      //������RDS��Ϣ
#define _CLEAN_RDS_PSN_
#endif

#if	defined(DAB_MODLE)
#define _ENABLE_RADIO_MUTE_        // �޸��˳�����ʱMUTE���������
#endif

#define _ENABLE_SAVE_FREQ_         // �޸�CH ���Ĵ�̨����

#define _FIVE_CHAR_DISPLAY_         //����װ���ʾ

//#define ID3_RDS_TOGETHER            //ID3��������

#define  _FEIYIN_473x_C40_ADJUST_   //���FM80-100db����ʧ��

#define _AUX_REDUCE_AUD_GATE_       //����AUX�������

#endif /* _USER_INIT_H_ */

