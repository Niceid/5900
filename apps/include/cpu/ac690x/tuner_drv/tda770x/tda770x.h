#ifndef __TDA770x_H__
#define __TDA770x_H__

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#define TDA770x_I2C_ADDR					(0xC2)
#define TDA770x_I2C_RATE				    (20)

typedef enum{
	TDA770x_COM_1,					//No autoinc, no burst, 32bit format
	TDA770x_COM_2,					//No autoinc, no burst, 24bit format
	TDA770x_COM_3,					//autoinc, burst, 32bit format 
	TDA770x_COM_4,					//autoinc, burst, 24bit format
}tda770x_com_e;

typedef enum{
	TDA770x_BAND_FM = 0,
	TDA770x_BAND_AM,
	TDA770x_BAND_WB,
	TDA770x_BAND_SW,
	TDA770x_BAND_MAX,
}tda770x_band_e;

typedef enum{
	TDA770x_TUNER_BAND_NULL = 0,
	TDA770x_TUNER_BAND_REC_FM,
	TDA770x_TUNER_BAND_REC_MW,
	TDA770x_TUNER_BAND_REC_SW,
	TDA770x_TUNER_BAND_REC_BW,
}tda770x_tuner_band_e;

typedef enum{
	TDA770x_AREA_USA = 0,
	TDA770x_AREA_LATIN,
	TDA770x_AREA_EUROPE,
	TDA770x_AREA_OIRT,
	TDA770x_AREA_JAPAN,
}tda770x_area_e;

typedef enum{
	TDA770x_DSP_IDLE = 0,
	TDA770x_DSP_FM,
	TDA770x_DSP_AM_EUJP,
	TDA770x_DSP_AM_US,
	TDA770x_DSP_WB,
}tda770x_dsp_e;

typedef enum{
	TDA770x_HIT_BOOT = 0,
	TDA770x_HIT_FM_PATCH,
	TDA770x_HIT_AM_PATCH,
	TDA770x_HIT_WX_PATCH,
	TDA770x_HIT_FM_WSP,
	TDA770x_HIT_AM_WSP,
	TDA770x_HIT_WX_WSP,
}tda770x_hit_e;

enum{
	TUNER_STATE_IDLE = 0,
	TUNER_STATE_START,
	TUNER_STATE_LEVEL,
	TUNER_STATE_DETECT,
};

//Command
#define CmdCode_ReadXmen                 0x01
#define CmdCode_ReadYmen                 0x02
#define CmdCode_WriteXmen                0x04
#define CmdCode_WriteYmen                0x05
#define CmdCode_ReadDMAmem               0x00
#define CmdCode_WriteDMAmem              0x03
#define CmdCode_Writemen                 0x1F
#define CmdCode_Readmen                  0x1E

#define CmdCode_SetFreq                  0x08
#define InjectSide_Auto                  0x00
#define InjectSide_FixedLow              0x01
#define InjectSide_FixedHigh             0x02

#define CmdCode_SetFEReg                 0x09
#define CmdCode_SetBand                  0x0a
#define CmdCode_SetDynIS                 0x0b
#define CmdCode_SetSeekTH                0x14
#define CmdCode_StartManuSeek            0x15

#define CmdCode_AFCheck                  0x07
#define CmdCode_AFSwitch                 0x10
#define CmdCode_AFStart                  0x0d
#define CmdCode_AFMeasure                0x0e
#define CmdCode_AFEnd                    0x0f

#define CmdCode_SetDDC                   0x1D

#define CmdCode_StartAlignment           0x0c
#define AlignVCO_EUUSA                   0x00	//87.5MHz-108MHz
#define AlignVCO_Japan                   0x01	//76MHz-90MHz
#define AlignVCO                         0x02	//only align VCO

#define CmdCode_StartAutoSeek            0x16
#define AutoEnd                          0x01
#define WaitCmdEnd                       0x00

#define CmdCode_SeekEnd                  0x17
#define SeekContinue                     0x01
#define SeekStopMuted                    0x02
#define SeekStopUnmuted                  0x03

#define CmdCode_ReadSeekStatus           0x18
#define SeekStatus_RstQaul               0x01
#define SeekStatus_NoRstQaul             0x00
#define SeekStatus_ManualSeek            0x01
#define SeekStatus_AutoSeek              0x02
#define SeekStatus_ASE                   0x04
#define SeekStatus_Seekok                0x01
#define SeekStatus_FullCircle            0x02

#define CmdCode_ReadTDS                  0x11
#define CmdCode_ReadFEReg                0x12
#define CmdCode_ReadRDSQual              0x13

#define CmdCode_IR                       0x19
#define OnlineIR_FM                      0x00
#define HybridIR_AM                      0x01
#define HybridIR_FM                      0x02
#define IRalignment                      0x03
#define HybridIR_WX                      0x04

#define CmdCode_SetDiss                  0x1a
#define Diss_FM_Auto                     0x00
#define Diss_FM_Manual                   0x01
#define Diss_FM_Special                  0x02
#define Diss_AM_Neutral                  0x0a
#define Diss_WX_Neutral                  0x0b
//BW for manual and special FM mode
#define Diss_FM_Manual_BW0               0x00//(narrowest)
#define Diss_FM_Manual_BW1               0x01
#define Diss_FM_Manual_BW2               0x02
#define Diss_FM_Manual_BW3               0x03
#define Diss_FM_Manual_BW4               0x04
#define Diss_FM_Manual_BW5               0x05
#define Diss_FM_Manual_BW6               0x06//(widest)

#define CmdCode_IBOC_Blender  			 0x06
#define CmdCode_MuSICA        			 0x20

#define LW_FREQ							300
#define AM_FREQ							6450
#define SW_FREQ                    		1725
#define FM_FREQ							6450

#define FM_START_FREQ					6500		//	65MHz
#define FM_STOP_FREQ					10800		//	108MHz
#define FM_WB_START_FREQ				62300		//	623MHz
#define FM_WB_STOP_FREQ					62600		//	626MHz
#define FM_OIRT_START_FREQ				6500		//	65MHz
#define FM_OIRT_STOP_FREQ				7500		//	75MHz
#define FM_JAPAN_START_FREQ				7500		//	75MHz
#define FM_JAPAN_STOP_FREQ				9100		//	91MHz

#define MW_START_FREQ					520			//	520MHz
#define MW_STOP_FREQ					1710		//	1710MHz

#define FSscalingthrmin 				205		// 195
#define FSscalingthrmax 				215		// 205

#define FreqInFM(x)						(((x) >= FM_START_FREQ)&&((x) <= FM_STOP_FREQ))
#define FreqInMW(x)						(((x) >= MW_START_FREQ)&&((x) <= MW_STOP_FREQ))
#define FreqInOirtFM(x)					(((x) >= FM_OIRT_START_FREQ)&&((x) <= FM_OIRT_STOP_FREQ))
#define FreqInJapanFM(x)				(((x) >= FM_JAPAN_START_FREQ)&&((x) <= FM_JAPAN_STOP_FREQ))
#define FreqInWBFM(x)					(((x) >= FM_WB_START_FREQ)&&((x) <= FM_WB_STOP_FREQ))
#define FreqInMWAM(x)					(((x) > LW_FREQ)&&((x) < SW_FREQ))
#define FreqInLWAM(x)					((x) < LW_FREQ)
#define FreqInSWAM(x)					(((x) < FM_FREQ)&&((x) > SW_FREQ))

#define PILOT_THS_ADDR 					0x01B05D

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

typedef struct{
	uint8 	smeter;
	uint16 	freq;
	uint8 	station;
	uint8 	full_cycle;

	uint8	detuning;
	uint8	multipath;
	uint8	adj_channel;
}tda770x_status_t;

typedef struct{
	uint8 addr[3];
	uint8 buffer[40];
	uint8 length;
}tda770x_order_t;

typedef struct{
	uint8 init_succeed;
	radio_band_e band;
	tda770x_tuner_band_e TunerBand;
	uint16 now_freq;
	uint16 min_freq;
	uint16 max_freq;
	uint8 tuner_state;
	uint32 last_sys_timer;
	uint8 signal;
	uint8 detun;
	uint8 dummy;
    uint8 station;
    uint8 TunerAutoSeek;
	uint8 loc;
	tda770x_area_e area;
	tda770x_order_t order;
	uint32 pilotth;
	uint8 cmd_check;
}tda770x_ctrl_t;

#define tda770x_order									(tda770x_ctrl->order)
#define tda770x_read_addr								((uint8 *)&(tda770x_order.buffer[0]))
#define tda770x_write_addr								((uint8 *)&(tda770x_order.buffer[3]))

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if !defined(TDA770x_TUNER_RSSI_FM_OFFSET)
	#define TDA770x_TUNER_RSSI_FM_OFFSET				(160<<16)													//	dBu
#endif
#define TDA770x_TUNER_RSSI_FM_DEFAULT					(TDA770x_TUNER_RSSI_FM_OFFSET + (20<<16))					//	dBu


#if !defined(TDA770x_TUNER_RSSI_FM)
#define TDA770x_TUNER_RSSI_FM							TDA770x_TUNER_RSSI_FM_DEFAULT
#endif

#if !defined(TDA770x_TUNER_RSSI_LOC_FM)	
#define TDA770x_TUNER_RSSI_LOC_FM						(TDA770x_TUNER_RSSI_FM + (20<<16))							//	dBu
#endif

#define TDA770x_TUNER_FM_TUNE_RSSI						(180)//178

#define TDA770x_TUNER_FM_LOC_TUNE_RSSI					(TDA770x_TUNER_FM_TUNE_RSSI + 28)

#if !defined(TDA770x_TUNER_RSSI_AM_OFFSET)
	#define TDA770x_TUNER_RSSI_AM_OFFSET				(155<<16) 													//	dBu
#endif
#define TDA770x_TUNER_RSSI_AM_DEFAULT					(TDA770x_TUNER_RSSI_AM_OFFSET + (15<<16))					//	dBu

#if !defined(TDA770x_TUNER_RSSI_AM)
#define TDA770x_TUNER_RSSI_AM							TDA770x_TUNER_RSSI_AM_DEFAULT	   							//	dBu
#endif

#if !defined(TDA770x_TUNER_RSSI_LOC_AM)
#define TDA770x_TUNER_RSSI_LOC_AM						(TDA770x_TUNER_RSSI_AM + (15<<16))							//	dBu
#endif

#define TDA770x_TUNER_AM_TUNE_RSSI						(167)

#define TDA770x_TUNER_AM_LOC_TUNE_RSSI					(TDA770x_TUNER_AM_TUNE_RSSI + 20)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#endif /*_TUNER_TDA770x_H_ */

