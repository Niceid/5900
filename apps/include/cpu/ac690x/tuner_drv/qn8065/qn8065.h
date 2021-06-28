#ifndef __QN0865_H__
#define __QN0865_H__

#if defined(_TUNER_DRIVER_QN8065_)


#define QN8065_I2C_ADDR					0x20
			
#define QN8065_I2C_READ_ADDR			(QN8065_I2C_ADDR | _B0_)
#define QN8065_I2C_WRITE_ADDR			(QN8065_I2C_ADDR)

#define QN8065_I2C_RATE					(80)// 20

typedef enum
{
	QN8065_TUNE_STEP_50KHZ = 0,
	QN8065_TUNE_STEP_100KHZ,
	QN8065_TUNE_STEP_200KHZ,
}qn8065_tuner_step_e;
#if defined(TD_ADD_KB1069)||defined(_ENABLE_XTAL_24M_)
#define XTAL_QN8065_24MHZ				1
#else
#define XTAL_QN8065_32768HZ			1
#endif
//#define XTAL_QN8065_32768HZ
//#define XTAL_QN8065_4MHZ
//#define XTAL_QN8065_6MHZ
//#define XTAL_QN8065_7_3728MHZ
//#define XTAL_QN8065_8MHZ
//#define XTAL_QN8065_10MHZ
//#define XTAL_QN8065_11_0592MHZ
//#define XTAL_QN8065_12MHZ
//#define XTAL_QN8065_16MHZ
//#define XTAL_QN8065_17_28MHZ
//#define XTAL_QN8065_20MHZ
//#define XTAL_QN8065_24MHZ
//#define XTAL_QN8065_26MHZ
//#define XTAL_QN8065_27MHZ
#if defined(XTAL_QN8065_32768HZ)
//crystal clock is 32768HZ
#define QN8065_XTAL_DIV0                   0x01
#define QN8065_XTAL_DIV1                   0x80
#define QN8065_XTAL_DIV2                   0x6e

#elif defined(XTAL_QN8065_4MHZ)
//crystal clock is 4MHZ
#define QN8065_XTAL_DIV0                   0x7a
#define QN8065_XTAL_DIV1                   0x68
#define QN8065_XTAL_DIV2                   0x66

#elif defined(XTAL_QN8065_6MHZ)
//crystal clock is 6MHZ
#define QN8065_XTAL_DIV0                   0xb7
#define QN8065_XTAL_DIV1                   0x68
#define QN8065_XTAL_DIV2                   0x66

#elif defined(XTAL_QN8065_7_3728MHZ)
//crystal clock is 7.3728MHZ
#define QN8065_XTAL_DIV0                   0xe1
#define QN8065_XTAL_DIV1                   0x80
#define QN8065_XTAL_DIV2                   0x6e

#elif defined(XTAL_QN8065_8MHZ)
//crystal clock is 8MHZ
#define QN8065_XTAL_DIV0                   0xf4
#define QN8065_XTAL_DIV1                   0x68
#define QN8065_XTAL_DIV2                   0x66

#elif defined(XTAL_QN8065_10MHZ)
//crystal clock is 10MHZ
#define QN8065_XTAL_DIV0                   0x31
#define QN8065_XTAL_DIV1                   0x69
#define QN8065_XTAL_DIV2                   0x66

#elif defined(XTAL_QN8065_11_0592MHZ)
//crystal clock is 11.0592MHZ
#define QN8065_XTAL_DIV0                   0x52
#define QN8065_XTAL_DIV1                   0x59
#define QN8065_XTAL_DIV2                   0x83

#elif defined(XTAL_QN8065_12MHZ)
//crystal clock is 12MHZ
#define QN8065_XTAL_DIV0                   0x6e
#define QN8065_XTAL_DIV1                   0x69
#define QN8065_XTAL_DIV2                   0x66


#elif defined(XTAL_QN8065_16MHZ)
//crystal clock is 16MHZ
#define QN8065_XTAL_DIV0                   0xe8
#define QN8065_XTAL_DIV1                   0x69
#define QN8065_XTAL_DIV2                   0x66

#elif defined(XTAL_QN8065_17_28MHZ)
//crystal clock is 17.28MHZ
#define QN8065_XTAL_DIV0                   
#define QN8065_XTAL_DIV1                  
#define QN8065_XTAL_DIV2                  

#elif defined(XTAL_QN8065_20MHZ)
//crystal clock is 20MHZ
#define QN8065_XTAL_DIV0                   0x62
#define QN8065_XTAL_DIV1                   0x6a
#define QN8065_XTAL_DIV2                   0x66

#elif defined(XTAL_QN8065_24MHZ)
//crystal clock is 24MHZ
#define QN8065_XTAL_DIV0                   0xdc
#define QN8065_XTAL_DIV1                   0x6a
#define QN8065_XTAL_DIV2                   0x66

#elif defined(XTAL_QN8065_26MHZ)
//crystal clock is 26MHZ
#define QN8065_XTAL_DIV0                   0x19
#define QN8065_XTAL_DIV1                   0x6b
#define QN8065_XTAL_DIV2                   0x66

#elif defined(XTAL_QN8065_27MHZ)
//crystal clock is 27MHZ
#define QN8065_XTAL_DIV0                   0x38
#define QN8065_XTAL_DIV1                   0xf3
#define QN8065_XTAL_DIV2                   0x6e

#endif


/*------------------------------------------------------------------------------------------------------------*/
//	这部分寄存器有内部MAPPING
#define QN8065_REG_SYSTEM1							(0x00)
#define QN8065_REG_VOL_CTL							(0x01)
#define QN8065_REG_SNR								(0x02)
#define QN8065_REG_RSSISIG							(0x03)
#define QN8065_REG_STATUS1							(0x04)
#define QN8065_REG_CID1								(0x05)
#define QN8065_REG_CID2								(0x06)
#define QN8065_REG_CH								(0x07)
#define QN8065_REG_CH_START							(0x08)
#define QN8065_REG_CH_STOP							(0x09)
#define QN8065_REG_CH_STEP							(0x0A)
#define QN8065_REG_CCA								(0x0B)
#define	QN8065_REG_CCA_SNR_TH   					(0x0c)
#define	QN8065_REG_CCA_CNT 					    	(0x0d)

#define QN8065_REG_XTAL_DIV0						(0x0E)
#define QN8065_REG_XTAL_DIV1						(0x0F)
#define QN8065_REG_XTAL_DIV2						(0x10)


#define QN8065_REG_CCA1								(0x19)
#define QN8065_REG_CCA2								(0x1a)
#define QN8065_REG_CCA_TIME                        	(0x1b)

#define QN8065_REG_CCOND1                          	(0x20)
#define QN8065_REG_CCOND2                         	(0x21)



#define QN8065_REG_SMSTART							(0x25)
#define QN8065_REG_SNCSTART							(0x26)
#define QN8065_REG_HCCSTART							(0x27)

#define QN8065_REG_REF                         		(0x2a)

#define QN8065_REG_FLL                         		(0x31)


#define QN8065_RSSI_THERSHOLD						(37)		//30
#define QN8065_SNR_THERSHOLD						(15)		// 8
#define QND_SINE_WAVE_CLOCK             (0x00)    //inject sine-wave clock  
#define QND_DIGITAL_CLOCK               (0x80)    //inject digital clock,default is inject digital 






#define CCA_NSNR_20MS                   (0x00 << 2)
#define CCA_NSNR_40MS                   (0x01 << 2)
#define CCA_NSNR_60MS                   (0x02 << 2)
#define CCA_NSNR_100MS                  (0x03 << 2)

#define CCA_NRSSI_10MS                  (0x00 << 4)
#define CCA_NRSSI_20MS                  (0x01 << 4)
#define CCA_NRSSI_40MS                  (0x02 << 4)

#define CCA_NRSSI_60MS                  (0x03 << 4)

#define CCA_NAGC_10MS                   (0x00 << 6)
#define CCA_NAGC_20MS                   (0x01 << 6)
#define CCA_NAGC_40MS                   (0x02 << 6)

#define CCA_NAGC_60MS                   (0x03 << 6)


#define ST_MO_RX                        0x01
#define QN8065_CH		    			0x03
#define QN8065_CH_START     			0x0c
#define QN8065_CH_STOP      			0x30

#define QN8065_SMSTART_VAL              (20)//(12)
#define QN8065_HCCSTART_VAL				(18)
#define QN8065_SNCSTART_VAL				(51)



#define FREQ2CHREG(freq)                ((freq-6000)/5)
#define CHREG2FREQ(ch)                  (ch*5+6000)

/******************************************/
/******************************************/
/******************************************/

#define QN8065_CHIPID_QN8000                   		0x00
#define QN8065_CHIPID_QN8005                   		0x20
#define QN8065_CHIPID_QN8005B1                 		0x21
#define QN8065_CHIPID_QN8006                   		0x30
#define QN8065_CHIPID_QN8006LB                 		0x71
#define QN8065_CHIPID_QN8007B1                 		0x11
#define QN8065_CHIPID_QN8007                   		0x10
#define QN8065_CHIPID_QN8006A1                 		0x30
#define QN8065_CHIPID_QN8006B1                 		0x31
#define QN8065_CHIPID_QN8016                   		0xe0
#define QN8065_CHIPID_QN8016_1                 		0xb0
#define QN8065_CHIPID_QN8015                   		0xa0
#define QN8065_CHIPID_QN8065                   		0x8C
#define QN8065_CHIPID_QN8067                   		0xd0
#define QN8065_CHIPID_QN8065N                  		0xa0
#define QN8065_CHIPID_QN8026                   		0x3C
#define QN8065_CHIPID_QN8027                   		0x40
#define QN8065_CHIPID_QN8025                   		0x80
#define QN8065_CHIPID_QN8035                   		0x84
#define QN8065_CHIPID_QN8036                   		0x34
#define QN8065_CHIPID_QN8301                   		0x44

#define QN8065_CHIPSUBID_A0              			0x01
#define QN8065_CHIPSUBID_A1              			0x02


enum{
	QN8065_TUNER_BAND_NULL = 0,
	QN8065_TUNER_BAND_FM,
};


typedef struct{
	struct{
		uint8 cca_ch_dis:1;
		uint8 chsc:1;
		uint8 force_mo:1;
		uint8 rds_en:1;
		uint8 rx_req:1;
		uint8 standby:1;
		uint8 recal:1;
		uint8 swrst:1;
	}system_1;
	
	struct{
		uint8 gain_ana:3;
		uint8 gain_dig:3;
		uint8 tc:1;
		uint8 mute_en:1;
	}vol_ctl;
	
	uint8 snr;

	uint8 rssi;

	struct{
		uint8 st_mo_rx:1;
		uint8 rx_agc_err:1;
		uint8 rx_agc_set:1;
		uint8 rxcca_fail:1;
		uint8 fsm:3;
		uint8 cap_sh:1;
	}status_1;

	struct{
		uint8 cid_2:2;
		uint8 cid_1:3;
		uint8 cid_0:3;
	}cid_1;

	struct{
		uint8 cid_4:2;
		uint8 cid_5:6;
	}cid_2;

	uint8 ch_l;

	uint8 ch_start_l;

	uint8 ch_stop_l;

	struct{
		uint8 ch_h:2;
		uint8 ch_start_h:2;
		uint8 ch_stop_h:2;
		uint8 fstep:2;
	}ch_step;

	uint8 cca;

	uint8 cca_snr_th;
	uint8 cca_snr_cnt;

	struct{
		uint8 xtal_div_l;
	}xtal_div_0;

	struct{
		uint8 xtal_div_h:3;
		uint8 pll_dlt_l:5;
	}xtal_div_1;

	struct{
		uint8 pll_dlt_h;
	}xtal_div_2;

}__attribute__ ((packed)) qn8065_reg_t;

typedef struct{
	uint8 			init_succeed;
	radio_band_e 	band;
	uint16 			now_freq;
	uint16 			min_freq;
	uint16 			max_freq;
	uint8			valid;
	uint8			qn8065b_id;
	uint8			stereo;
}qn8065_ctrl_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if !defined(QN8065_TUNER_RSSI_FM_VALUE)
	#define QN8065_TUNER_RSSI_FM_VALUE				(61)//(61)
#endif

#define QN8065_TUNER_RSSI_FM						(QN8065_TUNER_RSSI_FM_VALUE)

#if !defined(QN8065_TUNER_RSSI_LOC_FM_VALUE)
	#define QN8065_TUNER_RSSI_LOC_FM_VALUE			(QN8065_TUNER_RSSI_FM + 18)
#endif
#define QN8065_TUNER_RSSI_LOC_FM					(QN8065_TUNER_RSSI_LOC_FM_VALUE)

#define QN8065_TUNER_SNR_FM							(30)//(12)//(18)

#define QN8065_TUNER_ST_SNR							(22)//(20)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/


extern const tuner_drv_fun_t tuner_qn8065_drv_fun;


#endif

#endif
