#ifndef __TDA7708_H__
#define __TDA7708_H__

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#define TDA7708_I2C_ADDR					(0xC2)
#define TDA7708_I2C_RATE				    (40)//20

#define FM		0
#define AM		1
#define WB		2
#define SW		3

typedef enum{
	TDA7708_COM_1,					//No autoinc, no burst, 32bit format
	TDA7708_COM_2,					//No autoinc, no burst, 24bit format
	TDA7708_COM_3,					//autoinc, burst, 32bit format 
	TDA7708_COM_4,					//autoinc, burst, 24bit format
}tda7708_com_e;

typedef enum{
	TDA7708_BAND_FM = 0,
	TDA7708_BAND_AM,
	TDA7708_BAND_WB,
	TDA7708_BAND_SW,
	TDA7708_BAND_MAX,
}tda7708_band_e;

typedef enum{
	TDA7708_TUNER_BAND_NULL = 0,
	TDA7708_TUNER_BAND_REC_FM,
	TDA7708_TUNER_BAND_REC_MW,
	TDA7708_TUNER_BAND_REC_SW,
	TDA7708_TUNER_BAND_REC_BW,
}tda7708_tuner_band_e;

typedef enum{
	TDA7708_AREA_USA = 0,
	TDA7708_AREA_LATIN,
	TDA7708_AREA_EUROPE,
	TDA7708_AREA_OIRT,
	TDA7708_AREA_JAPAN,
}tda7708_area_e;

typedef enum{
	TDA7708_DSP_IDLE = 0,
	TDA7708_DSP_FM,
	TDA7708_DSP_AM_EuJp,
	TDA7708_DSP_AM_Us,
	TDA7708_DSP_WB,
}tda7708_dsp_e;

typedef enum{
	TDA7708_HIT_BOOT = 0,
	TDA7708_HIT_FM_PATCH,
	TDA7708_HIT_AM_PATCH,
	TDA7708_HIT_WX_PATCH,
	TDA7708_HIT_FM_WSP,
	TDA7708_HIT_AM_WSP,
	TDA7708_HIT_WX_WSP,
}tda7708_hit_e;

typedef enum {
	ACCESS_DIRECT,
	ACCESS_COMMAND
}eTuner_Access_Mode ;

//Tuner Channel
typedef enum {
	eTuner_Channel_None = 0,
	eTuner_Channel_Main = 1,
	eTuner_Channel_Background = 2,
	eTuner_Channel_Both
}eTuner_Channel;

enum{
	TUNER_STATE_IDLE = 0,
	TUNER_STATE_START,
	TUNER_STATE_LEVEL,
	TUNER_STATE_DETECT,
};

typedef struct _Star_RDS_Conifg {
	uint32 CSR;
	uint32 NBR;
}Star_RDS_Conifg;


typedef struct _stTunerBGStationInfor {
	uint32 Freq;
	char Smeter;
	u16 PI;
	uint8  PS[8];
	uint8  bBestSignal;
	uint8  bRemoved;
} StarBGStationInfor;

//Tuner Mode
typedef enum {
	eTuner_Mode_Standby = 0,
	eTuner_Mode_FM,
	eTuner_Mode_WX,
	eTuner_Mode_DAB_Band_3,
	eTuner_Mode_DAB_Band_L,
	eTuner_Mode_AM_EU,
	eTuner_Mode_AM_US,
	eTuner_Mode_DRM30,
	eTuner_Mode_DRM_Plus
} eTuner_Mode;

typedef struct _TunerStruct{
	uint8 WorkMode;
	uint8 WaitTimer10ms;
	uint8 TunerError;
	uint8 TunerErrorCnt;
	uint8 VPAmode;
	uint32 mainFreq;
	uint32 backgroundFreq;	/*Background freq*/
	eTuner_Mode Band;	
	Star_RDS_Conifg rdsConfig[2];
	
	uint8  seekDir;
	uint32 seekStep;
	uint8 SeekType;
}TunerStruct;
extern TunerStruct Tuner;

#define MODE_DIRECT_WRITE		0xE0
#define MODE_DIRECT_READ		0x60
#define MODE_CMD_WRITE			0xF0
#define MODE_CMD_READ			0x70


typedef struct _stTunerProcessFeature {
	uint8 dum1;   /*fixed 0, not used*/
	uint8 dum2;   /*fixed 0, not used*/
	uint8 dum3;   /*fixed 0, not used*/
	unsigned VPA 		: 1;
	unsigned 			: 3;
	unsigned HDBBFilter	: 1;
	unsigned 			: 3;	
} stTunerProcessFeature;

typedef union  _uTunerProcessFeature{
	uint32 value;
	stTunerProcessFeature	processFeature;
} uTunerProcessFeature;


typedef enum {
	eMute  		= 0x0,
	eUnmute 	= 0x03
} eMuteAction;


typedef enum {
	eAutoMode = 0,
	eForceMono,
	eForceStereo	//For test, normally not used
} eStereoMode;

typedef enum{
	eSeekIdle=0X00,
	eSeekAuto,
	eSeekManu
}eSeekMode;

#define SCSR0_ADDRESS								0x20100

//Command Code
//Basic commands
#define CMD_CODE_TUNER_PING						0x00
#define CMD_CODE_TUNER_READ		      			0x1E
#define CMD_CODE_TUNER_WRITE  					0x1F
#define CMD_CODE_TUNER_CHANGE_BAND 				0x0A
#define CMD_CODE_TUNER_TUNE						0x08
#define CMD_CODE_TUNER_SET_FM_PROC				0x11
#define CMD_CODE_TUNER_GET_RECEPTION_QUALITY 	0x12
#define CMD_CODE_TUNER_GET_CHANNEL_QUALITY	   	0x13
#define CMD_CODE_TUNER_MUTE						0x16
#define CMD_CODE_TUNER_FM_STEREO_MODE			0x17
#define CMD_CODE_TUNER_SET_VICS_OUTPUT			0x1C
#define CMD_CODE_DEVICE_BB_IF_ON_OFF			0x33
#define CMD_CODE_DEVICE_Set_BB_PROC				0x30

//RDS commands
#define CMD_CODE_TUNER_RDSBUFFER_SET 			0x1A
#define CMD_CODE_TUNER_RDSBUFFER_READ 			0x1B

//AF check commands
#define CMD_CODE_TUNER_AF_START					0x20
#define CMD_CODE_TUNER_AF_END					0x21
#define CMD_CODE_TUNER_AF_CHECK          			0x22
#define CMD_CODE_TUNER_AF_SWITCH        			0x23
#define CMD_CODE_TUNER_GET_AF_QUALITY 			0x24

//Seek commands
#define CMD_CODE_TUNER_SEEK_START        			0x26
#define CMD_CODE_TUNER_SEEK_END            			0x27
#define CMD_CODE_TUNER_GET_SEEK_STATUS 			0x28
#define CMD_CODE_TUNER_SET_SEEK_TRESHOLD 		0x29

//Configuration commands
#define CMD_CODE_TUNER_SET_HD_BLEND          		0x14
#define CMD_CODE_TUNER_CONF_HD_BLEND      		0x15
#define CMD_CODE_TUNER_SET_AUDIO_IF            		0x0D
#define CMD_CODE_TUNER_SET_BB_IF	               		0x04
#define CMD_CODE_TUNER_CONF_BB_SAI             		0x05
#define CMD_CODE_TUNER_CONF_JESD204          		0x06

//WX SAME commands
#define CMD_CODE_TUNER_READ_SAMEBUFF 			0x1D



//Debugging commands
#define CMD_CODE_DEVICE_SET_DISS					0x31
#define CMD_CODE_DEVICE_GET_WSP_STATUS			0x32

#define CMD_MAX_CONSECUTIVE_24BIT_WORDS  		29   /* for DSP memory, max : 29, for No-DSP memory max : 14  */

/* Used for Star_CmdProtocol_WR, if inParamNum == CMD_READIN_PARAM_NUM_FROM_HEADER, 
    Will read in the answer comand header first, then got the following parameters num from the header and read out. */
#define CMD_READIN_PARAM_NUM_FROM_HEADER		0xFF
#define CMD_READIN_ANSWER_HEADER_ONLY			0x00
#define CMD_CHECK_ERROR 				/*If defined, will check the command communication error, ex. checksum etc.*/

//Command parameter's position in buffer(Radio.DataBuff)
#define CMD_HEADER (Radio.DataBuff)
#define CMD_PARAM(x) (Radio.DataBuff + 3*(x))


//Command error definition
#define ERR_CMD_NONE						0x00
#define ERR_CMD_CHECKSUM_DSP				0x80	/*checksum is not correct on DSP side*/
#define ERR_CMD_ID_UNKNOW					0x40	/*unrecognized command ID*/
#define ERR_CMD_COLLISION					0x20	/*command blocked by command interpreter*/
#define ERR_CMD_CMD_ID_NOT_SAME 			0x10	/*the command id in command answer header is not same as the one send out */
#define ERR_CMD_CHECKSUM_ANSWER 			0x08   	/*the command answer's  checksum is not correct.*/

#define ERR_TUNER_FAILURE					0x01

//Tuner start up sequence states defination
#define TUNER_STARTUP_IDLE					0x0
#define TUNER_STARTUP_REQ					0x1
#define TUNER_STARTUP_WAIT_DSPINIT		0x2
#define TUNER_STARTUP_CHANGE_BAND		0x3

#define TUNER_INIT_SUCESS_AFFE42			0xAFFE4200
#define ADDRESS_OF_CMD_REG_BUFFER		0x020180	/* Fixed address for command*/


//Tuner Injection side
#define INJECTION_SIDE_AUTO 				0x00
#define INJECTION_SIDE_LOW					0x01   /*Force low injection side */
#define INJECTION_SIDE_HIGH 				0x02   /*Force high injection side */


//Seek  
#define SEEK_UP   							0x00
#define SEEK_DOWN 							0x01
#define SEEK_MANUAL 						0x00
#define SEEK_AUTO   							0x01
#define SEEK_END_UNMUTED					0x00
#define SEEK_END_MUTED						0x01
#define SEEK_STATUS_SEEKOK  				0x80
#define SEEK_STATUS_FULL_CIRCLE 			0x40 

//Customized seek threshold with manual seek method
#define SEEK_THRESHOLD_NOTUSED			0x7F

//#define SEEK_AM_FS_THRESHOLD				0x14   
#define SEEK_AM_FS_THRESHOLD				0x0E   
#define SEEK_AM_DT_THRESHOLD   				0x28 
#define SEEK_AM_SNR_THRESHOLD				0x66
#define SEEK_AM_ADJ_THRESHOLD   			0x46

#define SEEK_WX_FS_THRESHOLD				0x10  /*Need fine tune*/
#define SEEK_WX_DT_THRESHOLD				0x30


#define SEEK_FM_FS_THRESHOLD    			0x0E
#define SEEK_FM_DT_THRESHOLD    			0x1B
#define SEEK_FM_MPP_THRESHOLD  				0x60
#define SEEK_FM_MPXN_THRESHOLD 				0xC0  
#define SEEK_FM_SNR_THRESHOLD				0x40
#define SEEK_FM_ADJ_THRESHOLD 				0x64
#define SEEK_FM_VPA_coCH_THRESHOLD			0xff


#define SEEK_FM_FS_SCALMIN_MPXN 			0x0E
#define SEEK_FM_FS_SCALMAX_MPXN 			0x20
#define SEEK_FM_FS_SCALMIN_MP				0x18
#define SEEK_FM_FS_SCALMAX_MP				0x3C
#define SEEK_FM_FS_SCALMIN_ADJ			0x14
#define SEEK_FM_FS_SCALMAX_ADJ			0x28

#define SEEK_FM_FS_THRESHOLD_DELTA  		0x05
#define SEEK_FM_MPXN_FACTOR				0.4
#define SEEK_FM_MP_FACTOR					0.5
#define SEEK_FM_ADJ_FACTOR				0.7

#define SEEK_FM_WAIT_T1 					5
#define SEEK_FM_WAIT_T2					1
#define SEEK_FM_WAIT_T3					15
#define SEEK_FM_WAIT_T4					1
#define SEEK_FM_GET_QUALITY_CNT_N1 		2
#define SEEK_FM_GET_QUALITY_CNT_N2 		10

#define SEEK_AMWX_WAIT_T1 				70
#define SEEK_AMWX_WAIT_T2					1
#define SEEK_AMWX_GET_QUALITY_CNT_N1 	10


//RDS 
#define RDS_BOFL_RESET  					/* if defined, will reset RDS buffer when BOFL was true.*/

#define RDS_NORMALMODE_NRQST				0x08

#define RDS_CSR_RESET						0x09  /*Reset bit:  1: reset*/
#define RDS_CSR_ERRTHRESH					0x04  /*ERRTHRESH bit 4~6:  0x6: ignore the filter, all blocks will be saved to buffer*/
#define RDS_CSR_IREN						0x02  /*IREN bit:  1:  IRQ mode, 0 : no interrupt generated to MCU*/
#define RDS_CSR_RDS_RBDS					0x01  /*RDS_RBDS mode bit: 0: RDS; 1: RBDS*/
#define RDS_CSR_ENA							0x00  /*RDS Buffer enable bit: 1: enable, 0: disable*/

#define RDS_NBR_NRQST						0x00  /*bit 0~7:  NRQST: 0~20, Number of blocks acquired to mark the buffer ready condition in normal operation mode.*/

#define RDS_FM_FS_THRESHOLD_FAST			0x14  /*20dBuV*/
#define RDS_FM_FS_THRESHOLD_SLOW			0x28  /*40dBuV*/
#define RDS_FM_MP_THRESHOLD					SEEK_FM_MPP_THRESHOLD
#define RDS_FM_MPXN_THRESHOLD				SEEK_FM_MPXN_THRESHOLD
#define RDS_FM_DT_THRESHOLD					SEEK_FM_DT_THRESHOLD
#define RDS_FM_SNR_THRESHOLD				SEEK_FM_SNR_THRESHOLD
#define RDS_FM_ADJ_THRESHOLD				SEEK_FM_ADJ_THRESHOLD


#define RDS_FM_BG_CHECK_RDS_THRESHOLD	0x13  /* >= 20dBuV will check RDS during background scan. */

#define SET_BUFFER_WITH_U32(pDST, SOURCE) \
	(*(pDST))	 = (((SOURCE) >> 16) & 0xFF); \
	(*(pDST + 1)) = (((SOURCE) >>   8) & 0xFF); \
	(*(pDST + 2)) = (((SOURCE) & 0xFF));

#define SET_BUFFER_WITH_U8(pDST, S1, S2, S3) \
	(*(pDST)) 	 = (S1); \
	(*(pDST + 1)) = (S2); \
	(*(pDST + 2)) = (S3);

#define SET_BUFFER_CHECKSUM(wordNum) Star_SetBuffer_CheckSum((wordNum))

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
#define SW_FREQ                    		1840//1725
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

#define FSscalingthrmin 				185		// 195
#define FSscalingthrmax 				205		// 205

#define FreqInFM(x)						(((x) >= FM_START_FREQ)&&((x) <= FM_STOP_FREQ))
#define FreqInAM(x)                 				((x)<((FM_FREQ)))
#define FreqInMW(x)						(((x) >= MW_START_FREQ)&&((x) <= MW_STOP_FREQ))
#define FreqInOirtFM(x)					(((x) >= FM_OIRT_START_FREQ)&&((x) <= FM_OIRT_STOP_FREQ))
#define FreqInJapanFM(x)				(((x) >= FM_JAPAN_START_FREQ)&&((x) <= FM_JAPAN_STOP_FREQ))
#define FreqInWBFM(x)					(((x) >= FM_WB_START_FREQ)&&((x) <= FM_WB_STOP_FREQ))
#define FreqInMWAM(x)					(((x) > LW_FREQ)&&((x) < SW_FREQ))
#define FreqInLWAM(x)					((x) < LW_FREQ)
#define FreqInSWAM(x)					(((x) < FM_FREQ)&&((x) > SW_FREQ))

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_RADIO_RDS_)
typedef struct{
	uint8	enable;
	uint8	factor;				//	信号品质衰减因子
	uint8  	quality;			//	信号强度("0xFF"最强，"0"表示没有信号)
	uint8	discard;			//	解码丢弃数据个数

	uint8 	get_block;
	uint8 	block_cnt;
	
	uint16 	block_a;
	uint16 	block_b;
	uint16 	block_c;
	uint16 	block_d;
}tda7708_rds_t;
#endif

typedef struct{
	uint8 	smeter;
	uint16 	freq;
	uint8 	station;
	uint8 	full_cycle;

	uint8	detuning;
	uint8	multipath;
	uint8	adj_channel;
	unsigned char MpxNoise;
	unsigned char SNR;
	unsigned char Deviation;
	unsigned F_Stereo:1;//---1: stereo; 0: mono
	unsigned char IFBW;
	unsigned char CurrentBand;
}tda7708_status_t;

typedef struct{
	uint8 addr[3];
	uint8 buffer[40];//40
	uint8 length;
}tda7708_order_t;

typedef struct Radio_stuct {
	unsigned char ChipType;
	unsigned char SeekMode;
	unsigned char WorkMode;
	//unsigned char StartupMode;
	unsigned char CurrentBand;
	unsigned int ReqFreq;
	unsigned int Freq;
	unsigned int BackupFreq;
	unsigned int ScanBackupFreq;
	unsigned long BgScanFreq;
	//unsigned char TunerBuff[40];
	unsigned int MaxFreq;
	unsigned int MinFreq;
	unsigned char Step;
	unsigned char SeekStep;
	unsigned char Area;
	signed char Direction;

	signed char Smeter;
	unsigned char Multipath;
	signed char AdjChannel;
	unsigned char Detuning;
	unsigned char Deviation;
	
	unsigned char MpxNoise;
	unsigned char SNR;

	unsigned char AddressBuff[3];
	unsigned char DataBuff[100];
	unsigned char IFBW;
	unsigned char IS;

	unsigned F_GoodStation:1;
	unsigned F_SeekFullCycle:1;
	unsigned F_AutoSeek:1;
	unsigned F_TunerDispReq:1;
	unsigned F_ManualSeek:1;
	unsigned F_LocDx:1;
	unsigned F_Scan:1;
	unsigned F_Stereo:1;//---1: stereo; 0: mono
	
	unsigned F_QualDisplay:1;

	eTuner_Channel  TunerChannel;
	unsigned char ChkCounter;
} RadioStuct;

typedef struct Radio_Detect {
uint8 Smeter;
unsigned char Detuning;
unsigned char Deviation;
unsigned char Multipath;
unsigned char MpxNoise;
unsigned char SNR;
uint8 AdjChannel;
unsigned char IFBW;
} RadioDetect;

typedef struct{
	uint8 init_succeed;
	radio_band_e band;
	tda7708_tuner_band_e TunerBand;
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
	tda7708_area_e area;
	tda7708_order_t order;
	uint32 pilotth;
	uint8 cmd_check;
	
#if defined(_ENABLE_RADIO_RDS_)
	tda7708_rds_t tuner_rds;
#endif
	uint8 re_seek_flag;
	uint16 re_freq;
}tda7708_ctrl_t;

#define tda7708_order									(tda7708_ctrl->order)
#define tda7708_read_addr								((uint8 *)&(tda7708_order.buffer[0]))
#define tda7708_write_addr								((uint8 *)&(tda7708_order.buffer[3]))


void RadioRefreshFreq(unsigned long freq);

#if defined(_ENABLE_RADIO_RDS_)
/* RDSDISP Types definations */
#define RDSDISP_IDLE			0
#define RDSDISP_PS				1
#define RDSDISP_TRAFFIC			2
#define RDSDISP_PTY				3
#define RDSDISP_PTYSELECT		4


typedef struct _RDS_APP_FLAGS_
{
	unsigned F_AFEnable		  	:1;
	unsigned F_TAEnable		  	:1;
	unsigned F_PTYEnable	  		:1;
	unsigned F_REGEnable	  	:1;
	unsigned F_EONEnable	  	:1;
	unsigned F_PIMuteEnable	  	:1;
	unsigned F_InTASwitch	  	:1;
	unsigned F_InEONTASwitch  	:1;
	
	unsigned F_PISeek		  	:1;
	unsigned F_PTYSelect	  		:1;
	unsigned F_NoPTY		  	:1;
	unsigned F_RDSDispReq	  	:1;
	
	unsigned F_Pad				:4;
	
} RDSAppFlagsStruct;

typedef union  _RDS_APP_STATE_{
	u16					Data;
	RDSAppFlagsStruct   Flags;
} RDSAppStateStruct;


typedef enum {
	RDS_APP_AF_ENABLE = 0,
	RDS_APP_TA_ENABLE,
	RDS_APP_PTY_ENABLE,
	RDS_APP_REG_ENABLE,
	RDS_APP_EON_ENABLE,
	RDS_APP_PIMUTE_ENABLE,
	RDS_APP_IN_TA_SWITCH,
	RDS_APP_IN_EON_SWITCH,
	
	RDS_APP_PI_SEEK,
	RDS_APP_PTY_SELECT,
	RDS_APP_NoPTY,
	RDS_APP_RDSDispReq,

	RDS_APP_MAX  /* The end */
}eRDSApp_FunctionType;


typedef struct _RDSAPP_TIMERStructure {
	u8 RDSDispTimer;
	u8 PTYSelectTimer;
	u8 REAL_MainRDSBufferReadTimer;
	u8 REAL_BackgroundRDSBufferReadTimer;	
} RDSApp_TimersStruct;


typedef enum {
	RDSApp_TIMER_MainRDSBufferReadTimer,
	RDSApp_TIMER_BackgroundRDSBufferReadTimer,
	RDSApp_TIMER_RDSDispTimer,
	RDSApp_TIMER_PTYSelectTimer,
	RDSApp_TIMER_NUM
} eRDSApp_Timer_Type;

typedef struct _RDS_APP_DATA_
{
	u8 RDSDispMode;
	u8 PTYSelectType;
	RDSApp_TimersStruct Timers;
	RDSAppStateStruct     States;
}RDSAppDataStruct;

extern RDSAppDataStruct RDSApp;

void RDSInit(void);
void RDSMain(void);
void RDSApp_Timer10ms(void);
void RDSApp_Timer100ms(void);
u8 RDSApp_GetState(eRDSApp_FunctionType rdsAppFunction);


/* Definations */
#define EONBuffSize		15

//RDS Group Type checking definiations
#define GROUP0(x)		(((x) >> 1) == 0)
#define GROUP0A(x)		((x) == 0x00)
#define GROUP0B(x)		((x) == 0x01)

#define GROUP2(x)		(((x) >> 1) == 2)
#define GROUP2A(x)		((x) == 0x04)
#define GROUP2B(x)		((x) == 0x05)

#define GROUP4(x)		(((x) >> 1) == 4)
#define GROUP4A(x)		((x) == 0x08)

#define GROUP14(x)		(((x) >> 1) == 14)
#define GROUP14A(x)		((x) == 0x1C)
#define GROUP14B(x)		((x) == 0x1D)

#define GROUP15A(x)		((x) == 0x1E)
#define GROUP15B(x)		((x) == 0x1F)

/* Structures */
typedef struct _RDS_BUFFER_STRUCT_ {
	u8 BlockCnt;			
	u8 BufferData[100];
} RDSBufferStruct;


typedef struct _RDS_RAQDTAT_STRUCT_ {
	u8   BlockCnt;
	u8   LastBlockCnt;

	u16 BlockA;
	u16 BlockB;
	u16 BlockC;
	u16 BlockCp;
	u16 BlockD;
	u16 BlockLastA;
	u16 BlockLastB;
	u16 BlockLastC;
	u16 BlockLastCp;
	u16 BlockLastD;

	u8 RDSData[4];

	unsigned F_GetOneGroup:1;
	unsigned F_GetBlockA:1;
	unsigned F_GetBlockB:1;
	unsigned F_GetBlockC:1;
	unsigned F_GetBlockCp:1;
	unsigned F_GetBlockD:1;
	unsigned F_BlockA:1;
	unsigned F_BlockB:1;
	unsigned F_BlockC:1;
	unsigned F_BlockCp:1;
	unsigned F_BlockD:1;
	unsigned F_RDSStation:1;

} RDSRawBlocksStruct;

typedef struct BitChar {
	unsigned B0:1;
	unsigned B1:1;
	unsigned B2:1;
	unsigned B3:1;
	unsigned B4:1;
	unsigned B5:1;
	unsigned B6:1;
	unsigned B7:1;
} SBitChar;

typedef union  CharField{
	unsigned char	byte;
	SBitChar	field;
} UCharField;

typedef struct _RDS_BLOCK_STRUCT_ {
	u16 REAL_BlockLastA;
	u16 REAL_BlockLastB;
	u16 REAL_BlockLastC;
	u16 REAL_BlockLastCp;
	u16 REAL_BlockLastD;
	u8   REAL_Level;
	u8   REAL_MaxLevel;
} RDS_Block_Infor;


typedef struct _RDS_PIStructure {
	u16 REAL_PI;
	u16 REAL_PILast;
	u16 REAL_PILast2;
	u16 REAL_PIBackup;
} RDS_PI_Data;


typedef struct _RDS_PTYStructure {
	u8 REAL_PTY;
	u8 REAL_PTYLast;
} RDS_PTY_Data;


typedef struct _RDS_PSStucture {
	u8   REAL_PS[8];
	u16 REAL_PSCRC;
	u16 REAL_PSCRCTemp;
} RDS_PS_Data;


typedef struct _RDS_MJD_TimeStructure {
	u8   REAL_TimeHour;
	u8   REAL_TimeMin;
	U32 REAL_MJD;
} RDS_MJD_Time_Data;


typedef struct _RDS_AFStructure {
	u8 REAL_AF[25];
	u8 REAL_AFFreqBackup;
	u8 REAL_AFNum;

	u8 REAL_AFMethod[2];
} RDS_AF_Data;

typedef struct _RDS_EONStructure {
	u8	REAL_EONAF[25];
	u8	REAL_EONAFNum;
	u8	REAL_EONAFTimer;
	u16 REAL_EONPI;
	u8   REAL_EONPS[8];

	u8	REAL_EONAFSmeter[25];
	u8	REAL_EONAFNumBackup;
	u16 REAL_EONPIBackup;
	u8   REAL_EONPSBackup[8];
	u8   REAL_EONExitTimer;

	u16 REAL_EONSave_EONPI[EONBuffSize];
	u8   REAL_EONSave_EONPS[EONBuffSize][8];
	u8   REAL_EONSave_EONAF[EONBuffSize][25];
	u8   REAL_EONSave_EONAFNum[EONBuffSize];
	u8   REAL_EONSave_EONCheckDisable[EONBuffSize];

	UCharField REAL_EONSave_Flags[EONBuffSize];
		
	u8	REAL_EONCheck;
	u8	REAL_EONMode;
} RDS_EON_Data;


#define REAL_EONSave_EONTP(RDSDecodeData, x)			RDSDecodeData.EONInfor.REAL_EONSave_Flags[x].field.B0
#define REAL_EONSave_EONTPLast(RDSDecodeData, x)		RDSDecodeData.EONInfor.REAL_EONSave_Flags[x].field.B1
#define REAL_EONSave_EONTA(RDSDecodeData, x)			RDSDecodeData.EONInfor.REAL_EONSave_Flags[x].field.B2
#define REAL_EONSave_EONTALast(RDSDecodeData, x)		RDSDecodeData.EONInfor.REAL_EONSave_Flags[x].field.B3
#define REAL_EONSave_F_GetNewEONTP(RDSDecodeData, x)	RDSDecodeData.EONInfor.REAL_EONSave_Flags[x].field.B4
#define REAL_EONSave_F_GetNewEONTA(RDSDecodeData, x)	RDSDecodeData.EONInfor.REAL_EONSave_Flags[x].field.B5


typedef struct _RDS_MANAGE_FLAGS {
	unsigned REAL_TP					: 1;
	unsigned REAL_TPLast				: 1;
	unsigned REAL_TA				: 1;
	unsigned REAL_TALast				: 1;
	unsigned REAL_EONTP				: 1;
	unsigned REAL_EONTPLast			: 1;
	unsigned REAL_EONTA				: 1;
	unsigned REAL_EONTALast			: 1;
	
	unsigned REAL_F_GetOneGroup	: 1;
	unsigned REAL_F_BlockA			: 1;
	unsigned REAL_F_BlockB			: 1;
	unsigned REAL_F_BlockC			: 1;
	unsigned REAL_F_BlockCp			: 1;
	unsigned REAL_F_BlockD			: 1;
	unsigned REAL_F_RDSStation		: 1;
	unsigned REAL_F_AFMethodB		: 1;
	
	unsigned REAL_F_GetNewPI		: 1;
	unsigned REAL_F_GetNewPTY		: 1;
	unsigned REAL_F_GetNewTP		: 1;
	unsigned REAL_F_GetNewTA		: 1;
	unsigned REAL_F_GetNewEONTP	: 1;
	unsigned REAL_F_GetNewEONTA	: 1;
	unsigned REAL_F_PSGet0			: 1;
	unsigned REAL_F_PSGet1			: 1;
	
	unsigned REAL_F_PSGet2			: 1;
	unsigned REAL_F_PSGet3			: 1;
	unsigned REAL_F_PSExist			: 1;
	unsigned REAL_F_GetNewPS		: 1;
	unsigned REAL_F_GetNewCT		: 1;
	unsigned REAL_F_GetNewAF		: 1;
	unsigned REAL_F_GetAFList		: 1;
	unsigned REAL_F_GetNewRT		: 1;
	
	unsigned REAL_F_RTABFlag		: 1;
	unsigned REAL_F_RDSStationSlow	: 1;
	unsigned REAL_F_InEONTA		: 1;
	unsigned REAL_F_InEONTAReal	: 1;
	unsigned REAL_F_NoMatchEON		: 1;
	
} RDS_FLAGS;


typedef struct _RDS_DataStructure {
	RDS_Block_Infor Blocks;
	RDS_PI_Data PIInfor;
	RDS_PTY_Data  PTYInfor;
	u8    REAL_Group;
	RDS_PS_Data PSInfor;
	RDS_MJD_Time_Data MJD_TimeInfor;
	RDS_AF_Data	AFInfor;
	RDS_EON_Data EONInfor;
	u8 REAL_RT[64];
	RDS_FLAGS Flags;

}RDS_DataStructure;


typedef struct _RDSDATA_TIMERStructure {
	u8 REAL_RDSStationDelayTimer;

} RDSData_TimersStruct;


typedef enum {
	RDSData_TIMER_RDSStationDelayTimer,
	RDSData_TIMER_NUM
} eRDSData_Timer_Type;


typedef struct _RDSDATA_DECODE_STRUCT_ {
	RDSBufferStruct   		RawDataBuffer;		//Raw RDS data 
	RDSRawBlocksStruct 	BlocksBuffer;		//Raw data => blocks    (RDS   =>  RDSBlocksBuffer)
	RDS_DataStructure   	DecodedData; 			/* Main tuner and background tuner RDS decoded data etc.  */
} RDSData_Process;

typedef struct _RDSDATA_MNG_STRUCT_ {
	RDSData_Process		RDSData_Main;

#ifdef TUNER_APP_BACKGROUND
	RDSData_Process		RDSData_Background;
#endif
	RDSData_TimersStruct  Timers;
} RDSData_Struct;


extern RDSData_Struct RDSData;


void RDSData_Init(eTuner_Channel tunerChannel, u8 bResetRDSBuffer);
void RDSData_AFCheckInit(eTuner_Channel tunerChannel);


void RDSData_Timer1s(void);
void RDSData_Timer100ms(void);

void RDSData_Decode(eTuner_Channel tunerChannel);


// PS
u8   RDSData_FlagPSExist(eTuner_Channel tunerChannel);
u8   RDSData_FlagPSNew(eTuner_Channel tunerChannel);
u8   RDSData_GetPS(eTuner_Channel tunerChannel, u8 num);
void RDSData_FlagPSNewClear(eTuner_Channel tunerChannel);


// CT
u8   RDSData_FlagCTNew(eTuner_Channel tunerChannel);
void RDSData_FlagCTNewClear(eTuner_Channel tunerChannel);
u8   RDSData_GetTimeHour(eTuner_Channel tunerChannel);
u8   RDSData_GetTimeMin(eTuner_Channel tunerChannel);
U32 RDSData_GetMJD(void);


// PTY
u8   RDSData_FlagPTYNew(eTuner_Channel tunerChannel);
u8   RDSData_GetPTY(eTuner_Channel tunerChannel);


// TA/TP
u8   RDSData_FlagTP(eTuner_Channel tunerChannel);
u8   RDSData_FlagTA(eTuner_Channel tunerChannel);
u8   RDSData_FlagTPNew(eTuner_Channel tunerChannel);
u8   RDSData_FlagTANew(eTuner_Channel tunerChannel);
void RDSData_FlagTANewClear(eTuner_Channel tunerChannel);
void RDSData_FlagTPNewClear(eTuner_Channel tunerChannel);
void RDSData_ClearTAFlags(eTuner_Channel tunerChannel);
//EON
u8   RDSData_FlagEONTA(eTuner_Channel tunerChannel);
u8   RDSData_FlagEONTANew(eTuner_Channel tunerChannel);
u8   RDSData_FlagEONNoMatch(eTuner_Channel tunerChannel);

// PI
u16 RDSData_GetPI(eTuner_Channel tunerChannel);
u16 RDSData_GetPIBackup(eTuner_Channel tunerChannel);
u8   RDSData_FlagPIJudge(eTuner_Channel tunerChannel);
u8   RDSData_FlagPINew(eTuner_Channel tunerChannel);
u8   RDSData_GetDataBlock( eTuner_Channel tunerChannel);
void RDSData_SetPI(eTuner_Channel tunerChannel, u16 input);
void RDSData_SetPIBackup(eTuner_Channel tunerChannel, u16 input);

// RT
u8   RDSData_GetRT(eTuner_Channel tunerChannel, u8 num);

// AF
u8   RDSData_FlagAFListNew(eTuner_Channel tunerChannel);
u8   RDSData_GetAFNum(eTuner_Channel tunerChannel);
u8   RDSData_GetAF(eTuner_Channel tunerChannel, u8 num);
void RDSData_SetAFNum(eTuner_Channel tunerChannel, u8 input);
void RDSData_SetAF(eTuner_Channel tunerChannel, u8 num, u8 input);


u8   RDSData_FlagRDSStation(eTuner_Channel tunerChannel);
void RDSData_SetDataBlock(eTuner_Channel tunerChannel, u8 input);

u8   ChangFreq16BitTo8Bit(u16 inputfreq);
u16 ChangFreq8BitTo16Bit(u8 inputfreq);
U32 ChangFreq8BitTo32Bit(u8 inputfreq);


#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
#define TDA7708_TUNER_AM_TUNE_RSSI						(14)
#define TDA7708_TUNER_FM_TUNE_RSSI						(18)

#if !defined(TDA7708_TUNER_RSSI_FM)
#define TDA7708_TUNER_RSSI_FM							TDA7708_TUNER_FM_TUNE_RSSI
#endif

#if !defined(TDA7708_TUNER_RSSI_LOC_FM)	
#define TDA7708_TUNER_RSSI_LOC_FM						(TDA7708_TUNER_FM_TUNE_RSSI + 20)							//	dBu
#endif

#define TDA7708_TUNER_ST_RSSI							(15)


#if !defined(TDA7708_TUNER_RSSI_AM)
#define TDA7708_TUNER_RSSI_AM							TDA7708_TUNER_AM_TUNE_RSSI	   							//	dBu
#endif

#if !defined(TDA7708_TUNER_RSSI_LOC_AM)
#define TDA7708_TUNER_RSSI_LOC_AM						(TDA7708_TUNER_RSSI_AM + 20)							//	dBu
#endif


/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

#if !defined(TDA7708_TUNER_SEEK_RSSI_FM)
#define TDA7708_TUNER_SEEK_RSSI_FM						180
#define TDA7708_TUNER_SEEK_RSSI_LOC_FM					(TDA7708_TUNER_SEEK_RSSI_FM + 18)
#else
#define TDA7708_TUNER_SEEK_RSSI_LOC_FM					(TDA7708_TUNER_SEEK_RSSI_FM + 18)
#endif
#if !defined(TDA7708_TUNER_SEEK_RSSI_AM)
#define TDA7708_TUNER_SEEK_RSSI_AM						164
#define TDA7708_TUNER_SEEK_RSSI_LOC_AM					(TDA7708_TUNER_SEEK_RSSI_AM + 15)
#else
#define TDA7708_TUNER_SEEK_RSSI_LOC_AM					(TDA7708_TUNER_SEEK_RSSI_AM + 15)
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#endif /*_TUNER_TDA7708_H_ */

