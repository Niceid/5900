#ifndef __LT4730_H__
#define __LT4730_H__

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#define LT4730_I2C_ADDR					(0x20)
			
#define LT4730_I2C_READ_ADDR			(LT4730_I2C_ADDR | _B0_)
#define LT4730_I2C_WRITE_ADDR			(LT4730_I2C_ADDR)

#define LT4730_I2C_DELAY				(5)

#define LT4730_I2C_RATE				    (20)

#if !defined(LT4730_CRY_FREQ)
	#define LT4730_CRY_FREQ				(32768)
#endif

//寄存器定义
typedef enum{
	LT4730_REG_SYSTEM = 0,
	LT4730_REG_VOLCTRL,
	LT4730_REG_ENABLESTATUS,
	LT4730_REG_XTALDIV0,
	LT4730_REG_XTALDIV1,
	LT4730_REG_DIVDMM,
	LT4730_REG_DIVDML,
	LT4730_REG_HIGHCHANNEL,
	LT4730_REG_LOWCHANNEL,
	LT4730_REG_RSSIVALUE,
	LT4730_REG_SNRVALUE,
	LT4730_REG_CCASTATUS,
	LT4730_REG_SEEKSNR,
	LT4730_REG_CCARXSNR,
	LT4730_REG_HRSSITHRESH,
	LT4730_REG_LRSSITHRESH,
	LT4730_REG_SEEKHIGHCOUNT,
	LT4730_REG_SEEKLOWCOUNT,
	LT4730_REG_HCCASTART,
	LT4730_REG_LCCASTART,
	LT4730_REG_LCCASTOP,
	LT4730_REG_HCCASTOP,
	LT4730_REG_AUDIOEQUBASS, 			//	低音
	LT4730_REG_AUDIOEQUMID, 			//	中音
	LT4730_REG_AUDIOEQUMAX,  			//	0x18    	//高音
	LT4730_REG_RDSSTATUS, 				//	0x19
	LT4730_REG_I2SINFO = 0x23, 			//	i2s
	LT4730_REG_ST = 0x27, 				//	修改FM左右分离反 
	LT4730_REG_AM_TUNE = 0x28,			//	 add 2013.8.6 //
	LT4730_REG_AGCTHD = 0x2C, 			//	0x2c
	LT4730_REG_SEEK_RSSI_SEL=0x31,		//  	add in 2013.04.02 XYF
	LT4730_REG_NCC_FITER = 0x36, 		//	0x36
	LT4730_REG_RESPONSE = 0x37,			//   add 2013.5.24 //FM灵敏度
	LT4730_REG_75K = 0x39,			    //   add 2013.8.6 // 75K频偏
	LT4730_REG_75K2 = 0x3A,			    //   add 2013.8.6 //75K频偏
	//LT4730_REG_PLTTH = 0x40, 			//  	处理步进50K停台偏的问题 软件做延时处理  add in 2013.04.02 XYF
	LT4730_REG_SNC_VALUE = 0x42, 		//	0x42
	LT4730_REG_NHCC, 					//	0x43
	LT4730_REG_SOFTMUTE, 				//	0x44
	LT4730_REG_SNC_TH, 					//	0x45
	LT4730_REG_HCCSTART, 				//	0x46
	LT4730_REG_SOFTMUTE_TH,				//	0x47
	LT4730_REG_AM_MODULATION = 0x51,    //    0x51
	LT4730_REG_DIG = 0x52,    			//  	0x52
	LT4730_REG_FLL = 0x54, 				//   2013.8.1 判断AM锁定
	LT4730_REG_AMSNRLOCK = 0x57,  		//	0x57
	LT4730_REG_AMHRSSILOCK, 			//	0x58
	LT4730_REG_AMLRSSILOCK, 			//	0x59
	LT4730_REG_VREGL = 0x5F,			//	0x5f
	LT4730_REG_AMLNA=0x6A 			    //	0x6a
}lt4730_reg_e;

typedef enum
{
	LT473x_FM_STEP_50KHZ = 0,
	LT473x_FM_STEP_100KHZ,
	LT473x_FM_STEP_200KHZ,
}lt4730_fm_step_e;

typedef enum
{
	LT473x_AM_STEP_1KHZ = 0,
	LT473x_AM_STEP_9KHZ,
	LT473x_AM_STEP_10KHZ,
}lt4730_am_step_e;


typedef struct{
	struct{
		uint8 Band:2;
		uint8 Rx_En:1;
		uint8 Standby:1;
		uint8 Ch_Mode:1;
		uint8 Seek_Mode:1;
		uint8 Calibration:1;
		uint8 Soft_Reset:1;
	}System;

	struct{
		uint8 Vol:3;
		uint8 Res:3;			//注意:这个缺省值为010，一定要写进去
		uint8 Mute2_En:1;
		uint8 Mute1_En:1;
	}VolCtrl;

	struct{
		uint8 Ref_Mode:2;
		uint8 :2;
		uint8 Demp:1;
		uint8 Vol:3;
	}EnableStaus;

	uint8 XtalDiv0;

	struct{
		uint8 Div1:4;
		uint8 Mono:1;
		uint8 Div0:3;
	}XtalDiv1;

	uint8 DivDmm;
	
	uint8 DivDml;

	struct{
		uint8 Ch_Freq:5;
		uint8 Seek_Step:2;
		uint8 Lo_Freq:1;
	}HighChannel;

	uint8 LowChannel;
	
	uint8 RssiValue;
	
	uint8 SnrValue;

	struct{
		uint8 :3;
		uint8 Seek_Status:1;
		uint8 Agc1:1;
		uint8 Res2:1;
		uint8 St_Mono_Status:1;
		uint8 Res3:1;
	}CcaStatus;

	struct{
		uint8 Seek_Snr:7;
		uint8 :1;
	}SeekSnr;

	struct{
		uint8 Seek_Snr_Th:6;
		uint8 :2;
	}CcaRxSnr;

	struct{
		uint8 Seek_Rssi_Hth:7;
		uint8 :1;
	}HRssiThresh;

	struct{
		uint8 Seek_Rssi_Lth:7;
		uint8 :1;
	}LRssiThresh;

	struct{
		uint8 Seek_F_Err:6;
		uint8 :2;
	}SeekHighCount;
		
	uint8 SeekLowCount;

	struct{
		uint8 Seek_F_Start:5;
		uint8 :3;
	}HCcaStart;

	uint8 LCcaStart;
	
	uint8 LCcaStop;

	struct{
		uint8 :3;
		uint8 Seek_F_Stop:5;
	}HCcaStop;

	struct{
		uint8 Snc:5;
		uint8 :1;
		uint8 Snc_En:1;
	}SncValue;

	struct{
		uint8 Hi_Cut:4;
		uint8 :3;
		uint8 Hicut_En:1;
	}HicutValue;

	struct{
		uint8 Sm_En:1;
		uint8 :2;
		uint8 Sm:5;
	}SoftMute;

	struct{
		uint8 :1;
		uint8 Snc_Th:7;
	}SncCcaValue;

	struct{
		uint8 :1;
		uint8 Sm_Th:7;
	}SmCcaValue;
}__attribute__ ((packed)) lt4730_reg_t;


/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

typedef enum{
	LT4730_TUNER_BAND_FM = 0,
	LT4730_TUNER_BAND_AM,
}lt4730_tuner_band_e;


typedef struct{
	uint8 					init_succeed;
	radio_band_e 			band;
	uint8					valid;
	uint8					stereo;
	bool					power;
	lt4730_tuner_band_e 	TunerBand;
	bool					found;
	
	uint16 					now_freq;
	uint16 					min_freq;
	uint16 					max_freq;
	
#if defined(REDUCE_AM_NOISE_)
	bool					am_lock;
#endif
}lt4730_ctrl_t;


#define lt4730_power								(lt4730_ctrl->power)
#define lt4730_TunerBand							(lt4730_ctrl->TunerBand)
#define lt4730_found								(lt4730_ctrl->found)
#if defined(REDUCE_AM_NOISE_)
#define lt4730_am_lock								(lt4730_ctrl->am_lock)
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if !defined(LT4730_TUNER_RSSI_FM_VALUE)
	#define LT4730_TUNER_RSSI_FM_VALUE				(60)
#endif

#define LT4730_TUNER_RSSI_FM						(LT4730_TUNER_RSSI_FM_VALUE)


#if !defined(LT4730_TUNER_RSSI_LOC_FM_VALUE)
	#define LT4730_TUNER_RSSI_LOC_FM_VALUE			(LT4730_TUNER_RSSI_FM + 20)
#endif

#define LT4730_TUNER_RSSI_LOC_FM					(LT4730_TUNER_RSSI_LOC_FM_VALUE)

#define LT4730_TUNER_SNR_FM							(12)// 12
#define LT4730_TUNER_LOW_RSSI_FM					LT4730_TUNER_RSSI_FM
#define LT4730_TUNER_LOC_LOW_RSSI_FM				LT4730_TUNER_RSSI_LOC_FM_VALUE // 开LOC
#define LT4730_TUNER_HIGH_RSSI_FM			    	(0x6A)


#if !defined(LT4730_TUNER_RSSI_AM_VALUE)
	#define LT4730_TUNER_RSSI_AM_VALUE				(80)// 
#endif

#define LT4730_TUNER_RSSI_AM						(LT4730_TUNER_RSSI_AM_VALUE)

#if defined(_ENABLE_LOC_FUNTION_)
#define LT4730_TUNER_RSSI_LOC_AM					(LT4730_TUNER_RSSI_AM + 37)
#else
#define LT4730_TUNER_RSSI_LOC_AM					(LT4730_TUNER_RSSI_AM + 25)
#endif

#define LT4730_TUNER_SNR_AM							(15)
#define LT4730_TUNER_LOW_RSSI_AM			    	LT4730_TUNER_RSSI_AM
#define LT4730_TUNER_HIGH_RSSI_AM			    	(0x7F)


#define LT4730_TUNER_IFCNT_HIGH						(1) 
#define LT4730_TUNER_IFCNT_LOW				    	(0) 

#define LT4730_TUNER_RSSI_MAX			    		(0x6A)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#endif /*_TUNER_LT4730_H_ */

