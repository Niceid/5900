#ifndef __QN0835_H__
#define __QN0835_H__

#if defined(_TUNER_DRIVER_QN8035_)


#define QN8035_I2C_ADDR					0x20
			
#define QN8035_I2C_READ_ADDR			(QN8035_I2C_ADDR | _B0_)
#define QN8035_I2C_WRITE_ADDR			(QN8035_I2C_ADDR)

#if defined(_ENABLE_RADIO_AREA_INIT_)
#define QN8035_I2C_RATE					50//240//400//240//(20)// 20
#else
#define QN8035_I2C_RATE					20//240//400//240//(20)// 20
#endif

#if defined(SYS_IO_OUT_FM_CLK)
#if defined(SYS_IO_OUT_FM_24M_CLK_)
#define XTAL_QN8035_24MHZ
#elif defined(SYS_IO_OUT_FM_32768_CLK_)
#define XTAL_QN8035_32768HZ
#endif
#else
#define XTAL_QN8035_24MHZ
#endif
//#define XTAL_QN8035_32768HZ
//#define XTAL_QN8035_4MHZ
//#define XTAL_QN8035_6MHZ
//#define XTAL_QN8035_7_3728MHZ
//#define XTAL_QN8035_8MHZ
//#define XTAL_QN8035_10MHZ
//#define XTAL_QN8035_11_0592MHZ
//#define XTAL_QN8035_12MHZ
//#define XTAL_QN8035_16MHZ
//#define XTAL_QN8035_17_28MHZ
//#define XTAL_QN8035_20MHZ
//#define XTAL_QN8035_26MHZ
//#define XTAL_QN8035_27MHZ

#if defined(XTAL_QN8035_32768HZ)
//crystal clock is 32768HZ
#define QN8035_XTAL_DIV0                          	0x01
#define QN8035_XTAL_DIV1                           	0x08
#define QN8035_XTAL_DIV2                          	0x5c
#define QN8035_XTAL_DIV1_855						0x88
#define QN8035_XTAL_DIV2_855						0x3b

#elif defined(XTAL_QN8035_4MHZ)
//crystal clock is 4MHZ
#define QN8035_XTAL_DIV0                          	0x7a
#define QN8035_XTAL_DIV1                          	0x00
#define QN8035_XTAL_DIV2                         	0x54
#define QN8035_XTAL_DIV1_855						0x88
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_6MHZ)
//crystal clock is 6MHZ
#define QN8035_XTAL_DIV0                           	0xb7
#define QN8035_XTAL_DIV1                          	0x00
#define QN8035_XTAL_DIV2                          	0x54
#define QN8035_XTAL_DIV1_855						0x88
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_7_3728MHZ)
//crystal clock is 7.3728MHZ
#define QN8035_XTAL_DIV0                          	0xe1
#define QN8035_XTAL_DIV1                          	0x08
#define QN8035_XTAL_DIV2                          	0x5c
#define QN8035_XTAL_DIV1_855						0x88
#define QN8035_XTAL_DIV2_855						0x3b

#elif defined(XTAL_QN8035_8MHZ)
//crystal clock is 8MHZ
#define QN8035_XTAL_DIV0                           	0xf4
#define QN8035_XTAL_DIV1                           	0x00
#define QN8035_XTAL_DIV2                           	0x54
#define QN8035_XTAL_DIV1_855						0x88
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_10MHZ)
//crystal clock is 10MHZ
#define QN8035_XTAL_DIV0                          	0x31
#define QN8035_XTAL_DIV1                           	0x01
#define QN8035_XTAL_DIV2                          	0x54
#define QN8035_XTAL_DIV1_855						0x89
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_11_0592MHZ)
//crystal clock is 11.0592MHZ
#define QN8035_XTAL_DIV0                           	0x52
#define QN8035_XTAL_DIV1                          	0xa1
#define QN8035_XTAL_DIV2                          	0x70
#define QN8035_XTAL_DIV1_855						0x19
#define QN8035_XTAL_DIV2_855						0x50

#elif defined(XTAL_QN8035_12MHZ)
//crystal clock is 12MHZ
#define QN8035_XTAL_DIV0                         	0x6e
#define QN8035_XTAL_DIV1                         	0x01
#define QN8035_XTAL_DIV2                        	0x54
#define QN8035_XTAL_DIV1_855						0x89
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_16MHZ)
//crystal clock is 16MHZ
#define QN8035_XTAL_DIV0                           	0xe8
#define QN8035_XTAL_DIV1                          	0x01
#define QN8035_XTAL_DIV2                        	0x54
#define QN8035_XTAL_DIV1_855						0x89
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_17_28MHZ)
//crystal clock is 17.28MHZ
#define QN8035_XTAL_DIV0                          	0x0F
#define QN8035_XTAL_DIV1                         	0xF2
#define QN8035_XTAL_DIV2                         	0x52
#define QN8035_XTAL_DIV1_855						0x89
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_20MHZ)
//crystal clock is 20MHZ
#define QN8035_XTAL_DIV0                           	0x62
#define QN8035_XTAL_DIV1                          	0x02
#define QN8035_XTAL_DIV2                         	0x54
#define QN8035_XTAL_DIV1_855						0x8a
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_24MHZ)
//crystal clock is 24MHZ
#define QN8035_XTAL_DIV0                          	0xdc
#define QN8035_XTAL_DIV1                         	0x02
#define QN8035_XTAL_DIV2                         	0x54
#define QN8035_XTAL_DIV1_855						0x8a
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_26MHZ)
//crystal clock is 26MHZ
#define QN8035_XTAL_DIV0                         	0x19
#define QN8035_XTAL_DIV1                       		0x03
#define QN8035_XTAL_DIV2                        	0x54
#define QN8035_XTAL_DIV1_855						0x8b
#define QN8035_XTAL_DIV2_855						0x33

#elif defined(XTAL_QN8035_27MHZ)
//crystal clock is 27MHZ
#define QN8035_XTAL_DIV0                    		0x38
#define QN8035_XTAL_DIV1                      		0x73
#define QN8035_XTAL_DIV2                     		0x5c
#define QN8035_XTAL_DIV1_855						0xfb
#define QN8035_XTAL_DIV2_855						0x3b

#endif


/*------------------------------------------------------------------------------------------------------------*/
//	这部分寄存器有内部MAPPING
#define QN8035_REG_SYSTEM1							(0x00)
#define QN8035_REG_CCA								(0x01)
#define QN8035_REG_SNR								(0x02)
#define QN8035_REG_RSSISIG							(0x03)
#define QN8035_REG_STATUS1							(0x04)
#define QN8035_REG_CID1								(0x05)
#define QN8035_REG_CID2								(0x06)
#define QN8035_REG_CH								(0x07)
#define QN8035_REG_CH_START							(0x08)
#define QN8035_REG_CH_STOP							(0x09)
#define QN8035_REG_CH_STEP							(0x0A)
#define QN8035_REG_RDSD0							(0x0B)
#define QN8035_REG_RDSD1							(0x0C)
#define QN8035_REG_RDSD2							(0x0D)
#define QN8035_REG_RDSD3							(0x0E)
#define QN8035_REG_RDSD4							(0x0F)
#define QN8035_REG_RDSD5							(0x10)
#define QN8035_REG_RDSD6							(0x11)
#define QN8035_REG_RDSD7							(0x12)
#define QN8035_REG_STATUS2							(0x13)
#define QN8035_REG_VOL_CTL							(0x14)
#define QN8035_REG_XTAL_DIV0						(0x15)
#define QN8035_REG_XTAL_DIV1						(0x16)
#define QN8035_REG_XTAL_DIV2						(0x17)
#define QN8035_REG_INT_CTRL							(0x18)

/*------------------------------------------------------------------------------------------------------------*/
#define QN8035_REG_SMP_HLD_THRD						(0x19)
#define QN8035_REG_RXAGC_GAIN						(0x1A)
#define QN8035_REG_GAIN_SEL							(0x1B)
#define QN8035_REG_SYSTEM_CTL1						(0x1C)
#define QN8035_REG_SYSTEM_CTL2						(0x1D)
#define QN8035_REG_RDSCOSTAS						(0x1E)
#define QN8035_REG_REG_TEST							(0x1F)
#define QN8035_REG_STATUS4							(0x20)
#define QN8035_REG_RDSAGC2							(0x21)
#define QN8035_REG_CCA1								(0x27)
#define QN8035_REG_CCA2								(0x28)
#define QN8035_REG_CCA3								(0x29)
#define QN8035_REG_CCA4								(0x2A)
#define QN8035_REG_CCA5								(0x2B)
#define QN8035_REG_PLT1                 			(0x2F)
#define QN8035_REG_PLT2                 			(0x30)
#define QN8035_REG_SMSTART							(0x34)
#define QN8035_REG_SNCSTART							(0x35)
#define QN8035_REG_HCCSTART							(0x36)
#define QN8035_REG_CCA_CNT1							(0x37)
#define QN8035_REG_CCA_CNT2							(0x38)
#define QN8035_REG_CCA_SNR_TH_1						(0x39)
#define QN8035_REG_CCA_SNR_TH_2						(0x3A)
#define QN8035_REG_NCCFIR3         					(0x40)
#define QN8035_REG_NCO_COMP_VAL						(0x46)
#define QN8035_REG_REF              				(0x49)
#define QN8035_REG_DAC								(0x4C)
/*------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_QN8035_SOFTMUTE_)
#define QN8035_SMSTART_VAL                    		12
#elif defined(_BJ8716_QN8035_SOFTMUTE_)
#define QN8035_SMSTART_VAL                    		0
#else
#define QN8035_SMSTART_VAL                    		18//18
#endif

#define QN8035_HCCSTART_VAL							18
#define QN8035_SNCSTART_VAL							51

#define QN8035_CHIPID_QN8000                   		0x00
#define QN8035_CHIPID_QN8005                   		0x20
#define QN8035_CHIPID_QN8005B1                 		0x21
#define QN8035_CHIPID_QN8006                   		0x30
#define QN8035_CHIPID_QN8006LB                 		0x71
#define QN8035_CHIPID_QN8007B1                 		0x11
#define QN8035_CHIPID_QN8007                   		0x10
#define QN8035_CHIPID_QN8006A1                 		0x30
#define QN8035_CHIPID_QN8006B1                 		0x31
#define QN8035_CHIPID_QN8016                   		0xe0
#define QN8035_CHIPID_QN8016_1                 		0xb0
#define QN8035_CHIPID_QN8015                   		0xa0
#define QN8035_CHIPID_QN8065                   		0xa0
#define QN8035_CHIPID_QN8067                   		0xd0
#define QN8035_CHIPID_QN8065N                  		0xa0
#define QN8035_CHIPID_QN8026                   		0x3C
#define QN8035_CHIPID_QN8027                   		0x40
#define QN8035_CHIPID_QN8025                   		0x80
#define QN8035_CHIPID_QN8035                   		0x84
#define QN8035_CHIPID_QN8036                   		0x34
#define QN8035_CHIPID_QN8301                   		0x44

#define QN8035_CHIPSUBID_A0              			0x01
#define QN8035_CHIPSUBID_A1              			0x02

#define QN8035_RSSI_THERSHOLD						30
#define QN8035_SNR_THERSHOLD						8

enum{
	QN8035_TUNER_BAND_NULL = 0,
	QN8035_TUNER_BAND_FM,
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
		uint8 rxccad:6;
		uint8 imr:1;
		uint8 xtal_inj:1;
	}cca;

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

	uint8 rds_0;
	uint8 rds_1;
	uint8 rds_2;
	uint8 rds_3;
	uint8 rds_4;
	uint8 rds_5;
	uint8 rds_6;
	uint8 rds_7;

	struct{
		uint8 rds_b3_err:1;
		uint8 rds_b2_err:1;
		uint8 rds_b1_err:1;
		uint8 rds_b0_err:1;
		uint8 rds_sync:1;
		uint8 rds_c0_c1:1;
		uint8 e_det:1;
		uint8 rds_rx_upd:1;
	}status_2;

	struct{
		uint8 gain_ana:3;
		uint8 gain_dig:3;
		uint8 tc:1;
		uint8 mute_en:1;
	}vol_ctl;

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

	struct{
		uint8 :3;
		uint8 rds_4k_en:1;
		uint8 s_1k_en:1;
		uint8 rds_only:1;
		uint8 cca_int_en:1;
		uint8 rds_int_en:1;
	}int_ctrl;
}__attribute__ ((packed)) qn8035_reg_t;

typedef struct{
	uint8 			init_succeed;
	radio_band_e 	band;
	uint16 			now_freq;
	uint16 			min_freq;
	uint16 			max_freq;
	uint8			valid;
	uint8			qn8035b_id;
	uint8			stereo;
}qn8035_ctrl_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if !defined(QN8035_TUNER_RSSI_FM_VALUE)
	#define QN8035_TUNER_RSSI_FM_VALUE				(61)
#endif

#define QN8035_TUNER_RSSI_FM						(QN8035_TUNER_RSSI_FM_VALUE)

#if !defined(QN8035_TUNER_RSSI_LOC_FM_VALUE)
	#define QN8035_TUNER_RSSI_LOC_FM_VALUE			(QN8035_TUNER_RSSI_FM + 18)
#endif
#define QN8035_TUNER_RSSI_LOC_FM					(QN8035_TUNER_RSSI_LOC_FM_VALUE)

#if defined(_ENABLE_RADIO_SNR_)
#define QN8035_TUNER_SNR_FM							(6)//(18)
#else
#define QN8035_TUNER_SNR_FM							(12)//(18)
#endif

#define QN8035_TUNER_ST_SNR							(20)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/


extern const tuner_drv_fun_t tuner_qn8035_drv_fun;

#endif

#endif /* __QN0835_H__ */

