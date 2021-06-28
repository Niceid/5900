#include "si475x/si475x.h"
#if defined(_SPECIAL_AUTOSEEK_MEMORY_)
#include "setup.h"
#endif

#if defined(_TUNER_DRIVER_SI475x_)

static si475x_order_t *Si475xOrder = NULL;
static si475x_ctrl_t *si475xCtrl = NULL;

static const uint16 fm_si475x_deal_spot_tab[] = {9600};
static const uint16 am_si475x_deal_spot_tab[] = {};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
//	公共命令
#if !defined(SI475X_POWERUP_RESET)
//static const uint8 CMD_SI475x_POWER_UP_REC_FM_STEP1[] 					= {6,0x01,0x77,0x27,0x05,0X10,0x01};
//static const uint8 CMD_SI475x_POWER_UP_REC_FM_STEP3[] 					= {6,0x01,0x77,0x27,0x03,0X10,0x11};
static const uint8 CMD_SI475x_POWER_UP_REC_FM_STEP1[] 					= {6,0x01,0x77,0x27,0x27,0X00,0x00};
static const uint8 CMD_SI475x_POWER_UP_REC_FM_STEP3[] 					= {6,0x01,0x77,0x27,0x23,0X10,0x11};
//static const uint8 CMD_SI475x_POWER_UP_REC_AM_STEP1[] 					= {6,0x01,0x77,0x27,0x05,0X20,0x01};
static const uint8 CMD_SI475x_POWER_UP_REC_AM_STEP1[] 					= {6,0x01,0x77,0x27,0x27,0X00,0x00};
//#else
//static const uint8 CMD_SI475x_POWER_UP_REC_FM_STEP1[] 					= {6,0x01,0x77,0x27,0x27,0X00,0x00};
//static const uint8 CMD_SI475x_POWER_UP_REC_FM_STEP3[] 					= {6,0x01,0x77,0x27,0x23,0X10,0x11};
//static const uint8 CMD_SI475x_POWER_UP_REC_AM_STEP1[] 					= {6,0x01,0x77,0x27,0x27,0X00,0x00};
#endif

static const uint8 CMD_SI475x_POWER_UP_REC_AM_STEP3[] 					= {6,0x01,0x77,0x27,0x23,0X20,0x11};

//这个没用
static const uint8 CMD_SI475x_POWER_UP_REC_AM_STEP4[] 					= {6,0x01,0x77,0x27,0x23,0X20,0x11};
/////////////

static const uint8 CMD_SI475x_POWER_UP_STEP2[] 							= {3,0xFB,0x06,0x80};

static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x27,0x23,0X00,0x11};
//static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x09,0x23,0X00,0x11};
//static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x0E,0x23,0X00,0x11};
//static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x00,0x23,0X00,0x11};
//static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x14,0x23,0X00,0x11};
//static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x1C,0x23,0X00,0x11};
//static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x2C,0x23,0X00,0x11};
//static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x3F,0x23,0X00,0x11};
//static const uint8 CMD_SI475x_POWER_UP_RX_BOOT_LOADER[] 				= {6,0x01,0x77,0x34,0x23,0X00,0x11};

static const uint8 CMD_SI475x_POWER_UP_LOADER_PATCH1[] 					= {8,0x02,0xA1,0xF5,0x12,0x17,0x42,0xCD,0x88};
static const uint8 CMD_SI475x_POWER_UP_LOADER_PATCH2[] 					= {8,0x02,0xD7,0x32,0x68,0xC8,0xEF,0x77,0x16};
static const uint8 CMD_SI475x_POWER_UP_LOADER_PATCH3[] 					= {8,0x02,0x9A,0xB7,0x00,0xFF,0x44,0x75,0x20};
static const uint8 CMD_SI475x_POWER_UP_LOADER_PATCH4[] 					= {8,0x02,0x01,0x85,0xAB,0x55,0x66,0x37,0x75};
static const uint8 CMD_SI475x_POWER_UP_LOADER_PATCH5[] 					= {8,0x02,0x89,0x48,0x57,0x85,0xF3,0x42,0x86};

static const uint8 CMD_SI475x_GET_INT_STATUS[] 							= {1,0x15};
static const uint8 CMD_SI475x_GET_PART_INFO[] 							= {1,0x02};

static const uint8 CMD_SI475x_POWER_DOWN[] 								= {2,0x11,0x00};
static const uint8 CMD_SI475x_ANA_AUDIO_PIN_CFG[] 						= {2,0x1B,2};

static const uint8 CMD_SI475x_AUDIO_MUTE[] 								= {6,0x13,0x00,0x03,0x01,0x00,0x03};
static const uint8 CMD_SI475x_AUDIO_UNMUTE[] 							= {6,0x13,0x00,0x03,0x01,0x00,0x00};

/*-------------------------------------------------------------------------------------------------------------------------*/
//	调频命令
static const uint8 CMD_SI475x_SET_FM_FREQ[]								= {2,0x30,0x00};
static const uint8 CMD_SI475x_FM_RSQ_STATUS[]							= {2,0x32,0x00};
static const uint8 CMD_SI475x_FM_AGC_OVERRIDE[]							= {6,0x13,0x00,0x07,0X10,0x00,0x00};
static const uint8 CMD_SI475x_FM_ACF_STATUS[]							= {2,0x35,0x00};

static const uint8 CMD_SI475x_FM_AGC_FE_CONFIG[]						= {6,0x13,0x00,0x07,0x00,0x00,0x01};
static const uint8 CMD_SI475x_FM_AGC_STATUS[]							= {6,0x13,0x00,0x07,0x01,0x02,0x30};

static const uint8 CMD_SI475x_FM_R8000[]								= {6,0x13,0x00,0x80,0x00,0x00,0x00};
static const uint8 CMD_SI475x_FM_R8001[]								= {6,0x13,0x00,0x80,0x01,0x00,0x00};

static const uint8 CMD_SI475x_FM_SEEK_TUNE_RSSI_RESET[]					= {6,0x13,0x00,0x20,0x04,0x00,0x00};
static const uint8 CMD_SI475x_FM_SEEK_TUNE_RSSI_SET[]					= {5,0x13,0x00,0x20,0x04,0x00};
static const uint8 CMD_SI475x_FM_SEEK_TUNE_SNR_RESET[]					= {6,0x13,0x00,0x20,0x03,0x00,0x00};
static const uint8 CMD_SI475x_FM_SEEK_TUNE_SNR_SET[]					= {5,0x13,0x00,0x20,0x03,0x00};

static const uint8 CMD_SI475x_SET_FM_DEEMPHASIS_50US[] 					= {6,0x13,0x00,0x03,0x02,0x00,0x01};
static const uint8 CMD_SI475x_SET_FM_DEEMPHASIS_75US[] 					= {6,0x13,0x00,0x03,0x02,0x00,0x00};
static const uint8 CMD_SI475x_FM_TUNE_ERROR[]							= {6,0x13,0x00,0x20,0x00,0x00,0x4B};

#if defined(SI475x_FM_SOFT_MUTE_ATTENUATION_VALUE)
static const uint8 CMD_SI475x_FM_SOFT_MUTE_ATTENUATION[]				= {6,0x13,0x00,0x04,0x00,0x00,SI475x_FM_SOFT_MUTE_ATTENUATION_VALUE};
#else
static const uint8 CMD_SI475x_FM_SOFT_MUTE_ATTENUATION[]				= {6,0x13,0x00,0x04,0x00,0x00,12};
#endif
#if defined(SI475x_FM_SOFT_MUTE_TRIGGER_THRESHOLD_VALUE)
static const uint8 CMD_SI475x_FM_SOFT_MUTE_TRIGGER_THRESHOLD[]			= {6,0x13,0x00,0x04,0x01,0x00,SI475x_FM_SOFT_MUTE_TRIGGER_THRESHOLD_VALUE};
#else
static const uint8 CMD_SI475x_FM_SOFT_MUTE_TRIGGER_THRESHOLD[]			= {6,0x13,0x00,0x04,0x01,0x00,2};
#endif
#if defined(SI475x_FM_SOFT_MUTE_END_THRESHOLD_VALUE)
static const uint8 CMD_SI475x_FM_SOFT_MUTE_END_THRESHOLD[]				= {6,0x13,0x00,0x04,0x02,0x00,SI475x_FM_SOFT_MUTE_END_THRESHOLD_VALUE};
#else
static const uint8 CMD_SI475x_FM_SOFT_MUTE_END_THRESHOLD[]				= {6,0x13,0x00,0x04,0x02,0x00,0};
#endif

static const uint8 CMD_SI475x_FM_SEEK_START[]							= {1,0x31};
static const uint8 CMD_SI475x_FM_SEEK_STOP[]							= {2,0x32,0x03};
static const uint8 CMD_SI475x_FM_SEEK_BAND_BOTTOM[]						= {4,0x13,0x00,0x11,0x00};
static const uint8 CMD_SI475x_FM_SEEK_BAND_TOP[]						= {4,0x13,0x00,0x11,0x01};
static const uint8 CMD_SI475x_FM_SEEK_BAND_STEP[]						= {5,0x13,0x00,0x11,0x02,0x00};

static const uint8 CMD_SI475x_FM_BLEND_RSSI_STEREO[]         			= {6,0x13,0x00,0x35,0x00,SI475x_TUNER_RSSI_STEREO_VALUE,SI475x_TUNER_RSSI_MONO_VALUE};
static const uint8 CMD_SI475x_FM_BLEND_RSSI_MONO[]          			= {6,0x13,0x00,0x35,0x00,120,120};
static const uint8 CMD_SI475x_FM_BLEND_RSSI_STEREO_SEP[]        		= {6,0x13,0x00,0x35,0x01,SI475x_TUNER_RSSI_STEREO_SEP_MAX,SI475x_TUNER_RSSI_STEREO_SEP_MIN};
static const uint8 CMD_SI475x_FM_BLEND_RSSI_ATTACK_RATE[]       		= {6,0x13,0x00,0x35,0x02,SI475x_TUNER_RSSI_ATTACK_TIME_H,SI475x_TUNER_RSSI_ATTACK_TIME_L};//0x0010,  0x03E8
static const uint8 CMD_SI475x_FM_BLEND_RSSI_RELEASE_RATE[]      		= {6,0x13,0x00,0x35,0x03,0x0F,0xA0};

static const uint8 CMD_SI475x_FM_BLEND_MULTIPATH_ATTACK_RATE[] 			= {6,0x13,0x00,0x35,0x0A,SI475x_TUNER_RSSI_ATTACK_TIME_H,SI475x_TUNER_RSSI_ATTACK_TIME_L};
static const uint8 CMD_SI475x_FM_BLEND_MULTIPATH_RELEASE_RATE[]			= {6,0x13,0x00,0x35,0x0B,0x0F,0xA0};
static const uint8 CMD_SI475x_FM_BLEND_MULTIPATH_STEREO[]   			= {6,0x13,0x00,0x35,0x08,127,127};
static const uint8 CMD_SI475x_FM_BLEND_MULTIPATH_MONO[]    				= {6,0x13,0x00,0x35,0x08,0,0};
static const uint8 CMD_SI475x_FM_BLEND_MULTIPATH_STEREO_SEP[]   		= {6,0x13,0x00,0x35,0x09,64,0};

static const uint8 CMD_SI475x_FM_BLEND_USN_ATTACK_RATE[] 				= {6,0x13,0x00,0x35,0x12,SI475x_TUNER_RSSI_ATTACK_TIME_H,SI475x_TUNER_RSSI_ATTACK_TIME_L};
static const uint8 CMD_SI475x_FM_BLEND_USN_RELEASE_RATE[]				= {6,0x13,0x00,0x35,0x13,0x0F,0xA0};
static const uint8 CMD_SI475x_FM_BLEND_USN_STEREO_DEFAULT[]   			= {6,0x13,0x00,0x35,0x10,127,127};
static const uint8 CMD_SI475x_FM_BLEND_USN_MONO_DEFAULT[]    			= {6,0x13,0x00,0x35,0x10,0,0};
static const uint8 CMD_SI475x_FM_BLEND_USN_STEREO_SEP[]   				= {6,0x13,0x00,0x35,0x11,100,0};

static const uint8 CMD_SI475x_FM_HICUT_RSSI_THRESHOLDS[]         		= {6,0x13,0x00,0x36,0x00,30,15};
static const uint8 CMD_SI475x_FM_HICUT_RSSI_CUTOFF_FREQ[]          		= {6,0x13,0x00,0x36,0x01,0xB4,80};
static const uint8 CMD_SI475x_FM_HICUT_RSSI_ATTACK_TIME_[]        		= {6,0x13,0x00,0x36,0x02,SI475x_TUNER_RSSI_ATTACK_TIME_H,SI475x_TUNER_RSSI_ATTACK_TIME_L};
static const uint8 CMD_SI475x_FM_HICUT_RSSI_RELEASE_TIME_[]           	= {6,0x13,0x00,0x36,0x03,0x0F,0xA0};

static const uint8 CMD_SI475x_FM_HICUT_MULTIPATH_THRESHOLDS[]         	= {6,0x13,0x00,0x36,0x08,50,80};
static const uint8 CMD_SI475x_FM_HICUT_MULTIPATH_RSSI_CUTOFF_FREQ[]		= {6,0x13,0x00,0x36,0x09,0xB4,80};
static const uint8 CMD_SI475x_FM_HICUT_MULTIPATH_RSSI_ATTACK_TIME_[]	= {6,0x13,0x00,0x36,0x0A,SI475x_TUNER_RSSI_ATTACK_TIME_H,SI475x_TUNER_RSSI_ATTACK_TIME_L};
static const uint8 CMD_SI475x_FM_HICUT_MULTIPATH_RSSI_RELEASE_TIME_[]	= {6,0x13,0x00,0x36,0x0B,0x0F,0xA0}; 

static const uint8 CMD_SI475x_FM_HICUT_USN_THRESHOLDS[]					= {6,0x13,0x00,0x36,0x10,37,15};
static const uint8 CMD_SI475x_FM_HICUT_USN_RSSI_CUTOFF_FREQ[]			= {6,0x13,0x00,0x36,0x11,0xB4,80};
static const uint8 CMD_SI475x_FM_HICUT_USN_RSSI_ATTACK_TIME_[]			= {6,0x13,0x00,0x36,0x12,SI475x_TUNER_RSSI_ATTACK_TIME_H,SI475x_TUNER_RSSI_ATTACK_TIME_L};
static const uint8 CMD_SI475x_FM_HICUT_USN_RSSI_RELEASE_TIME_[]			= {6,0x13,0x00,0x36,0x13,0x0F,0xA0};

static const uint8 CMD_SI475x_FM_HIBLEND_RSSI_THRESHOLDS[]         		= {6,0x13,0x00,0x37,0x00,37,15};
static const uint8 CMD_SI475x_FM_HIBLEND_RSSI_CUTOFF_FREQ[]          	= {6,0x13,0x00,0x37,0x01,0xB4,80};
static const uint8 CMD_SI475x_FM_HIBLEND_RSSI_ATTACK_TIME_[]        	= {6,0x13,0x00,0x37,0x02,0x00,0x04};
static const uint8 CMD_SI475x_FM_HIBLEND_RSSI_RELEASE_TIME_[]           = {6,0x13,0x00,0x37,0x03,0x1F,0x40};

static const uint8 CMD_SI475x_FM_HIBLEND_MULTIPATH_THRESHOLDS[]			= {6,0x13,0x00,0x37,0x08,25,60};
static const uint8 CMD_SI475x_FM_HIBLEND_MULTIPATH_CUTOFF_FREQ[]		= {6,0x13,0x00,0x37,0x09,0xB4,80};
static const uint8 CMD_SI475x_FM_HIBLEND_MULTIPATH_ATTACK_TIME_[]		= {6,0x13,0x00,0x37,0x0A,0x00,0x04};
static const uint8 CMD_SI475x_FM_HIBLEND_MULTIPATH_RELEASE_TIME_[]		= {6,0x13,0x00,0x37,0x0B,0x1F,0x40};

static const uint8 CMD_SI475x_FM_HIBLEND_USN_THRESHOLDS[]				= {6,0x13,0x00,0x37,0x10,55,30};
static const uint8 CMD_SI475x_FM_HIBLEND_USN_CUTOFF_FREQ[]				= {6,0x13,0x00,0x37,0x11,0xB4,80};
static const uint8 CMD_SI475x_FM_HIBLEND_USN_ATTACK_TIME_[]				= {6,0x13,0x00,0x37,0x12,0x00,0x04};
static const uint8 CMD_SI475x_FM_HIBLEND_USN_RELEASE_TIME_[]			= {6,0x13,0x00,0x37,0x13,0x1F,0x40};

#if defined(_ENABLE_RADIO_RDS_)
/*-------------------------------------------------------------------------------------------------------------------------*/
//  RDS命令
static const uint8 CMD_SI475x_FM_RDS_DETECT[]							= {2,0x36,0x03};
static const uint8 CMD_SI475x_FM_RDS_STATUS[]							= {2,0x36,0x01};
static const uint8 CMD_SI475x_FM_RDS_INT_SOURCE[]						= {6,0x13,0x00,0x40,0x00,0x00,0x01};
static const uint8 CMD_SI475x_FM_RDS_FIFO_COUNT[]						= {6,0x13,0x00,0x40,0x01,0x00,0x01};
static const uint8 CMD_SI475x_FM_RDS_CONFIG[]							= {6,0x13,0x00,0x40,0x02,0x00,0x01};
#endif

/*-------------------------------------------------------------------------------------------------------------------------*/
//	调幅命令
static const uint8 CMD_SI475x_SET_AM_FREQ[]								= {2,0x40,0x00};
static const uint8 CMD_SI475x_AM_RSQ_STATUS[]							= {2,0x42,0x00};

static const uint8 CMD_SI475x_AM_SOFT_MUTE_RATE[]						= {6,0x13,0x00,0x04,0x03,0x01,0xF4};
static const uint8 CMD_SI475x_AM_SOFT_MUTE_SLOPE[]						= {6,0x13,0x00,0x04,0x04,0x00,0X78};

#if defined(SI475x_AM_SOFT_MUTE_ATTENUATION_VALUE)
static const uint8 CMD_SI475x_AM_SOFT_MUTE_ATTENUATION[]				= {6,0x13,0x00,0x04,0x00,0x00,SI475x_AM_SOFT_MUTE_ATTENUATION_VALUE};
#else
static const uint8 CMD_SI475x_AM_SOFT_MUTE_ATTENUATION[]				= {6,0x13,0x00,0x04,0x00,0x00,20};
#endif
			
#if defined(SI475x_AM_SOFT_MUTE_TRIGGER_THRESHOLD_VALUE)
static const uint8 CMD_SI475x_AM_SOFT_MUTE_TRIGGER_THRESHOLD[]			= {6,0x13,0x00,0x04,0x01,0x00,SI475x_AM_SOFT_MUTE_TRIGGER_THRESHOLD_VALUE};
#else
static const uint8 CMD_SI475x_AM_SOFT_MUTE_TRIGGER_THRESHOLD[]			= {6,0x13,0x00,0x04,0x01,0x00,7};
#endif
#if defined(SI475x_AM_SOFT_MUTE_END_THRESHOLD_VALUE)
static const uint8 CMD_SI475x_AM_SOFT_MUTE_END_THRESHOLD[]				= {6,0x13,0x00,0x04,0x02,0x00,SI475x_AM_SOFT_MUTE_END_THRESHOLD_VALUE};
#else
static const uint8 CMD_SI475x_AM_SOFT_MUTE_END_THRESHOLD[]				= {6,0x13,0x00,0x04,0x02,0x00,0};
#endif

static const uint8 CMD_SI475x_AM_SEEK_TUNE_RSSI_RESET[]					= {6,0x13,0x00,0x20,0x04,0x00,0x00};
static const uint8 CMD_SI475x_AM_SEEK_TUNE_RSSI_SET[]					= {5,0x13,0x00,0x20,0x04,0x00};
static const uint8 CMD_SI475x_AM_SEEK_TUNE_SNR_RESET[]					= {6,0x13,0x00,0x20,0x03,0x00,0x00};
static const uint8 CMD_SI475x_AM_SEEK_TUNE_SNR_SET[]					= {5,0x13,0x00,0x20,0x03,0x00};

static const uint8 CMD_SI475x_AM_SEEK_START[]							= {1,0x41};
static const uint8 CMD_SI475x_AM_SEEK_STOP[]							= {2,0x42,0x03};
static const uint8 CMD_SI475x_AM_SEEK_BAND_BOTTOM[]						= {4,0x13,0x00,0x11,0x00};
static const uint8 CMD_SI475x_AM_SEEK_BAND_TOP[]						= {4,0x13,0x00,0x11,0x01};
static const uint8 CMD_SI475x_AM_SEEK_BAND_STEP[]						= {5,0x13,0x00,0x11,0x02,0x00};

static const uint8 CMD_SI475x_AM_DEMOD_MODE[]							= {6,0x13,0x00,0x23,0x00,0x01,0x00};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static const uint8 *CMD_SI475x_TAB[]=
{
/*-------------------------------------------------------------------------------------------------------------------------*/
//	公共命令
	CMD_SI475x_POWER_UP_REC_FM_STEP1,
	CMD_SI475x_POWER_UP_REC_FM_STEP3,
	
	CMD_SI475x_POWER_UP_REC_AM_STEP1,
	CMD_SI475x_POWER_UP_REC_AM_STEP3,
	CMD_SI475x_POWER_UP_REC_AM_STEP4,
	
	CMD_SI475x_POWER_UP_STEP2,
	
	CMD_SI475x_POWER_UP_RX_BOOT_LOADER,
	CMD_SI475x_POWER_UP_LOADER_PATCH1,
	CMD_SI475x_POWER_UP_LOADER_PATCH2,
	CMD_SI475x_POWER_UP_LOADER_PATCH3,
	CMD_SI475x_POWER_UP_LOADER_PATCH4,
	CMD_SI475x_POWER_UP_LOADER_PATCH5, 
	
	CMD_SI475x_GET_INT_STATUS,
	
	CMD_SI475x_POWER_DOWN,
	CMD_SI475x_ANA_AUDIO_PIN_CFG,
	CMD_SI475x_AUDIO_MUTE,
	CMD_SI475x_AUDIO_UNMUTE,
/*-------------------------------------------------------------------------------------------------------------------------*/
//	调频命令
	CMD_SI475x_SET_FM_FREQ,
	CMD_SI475x_FM_RSQ_STATUS,
	CMD_SI475x_FM_AGC_OVERRIDE,
	CMD_SI475x_FM_ACF_STATUS,
	CMD_SI475x_FM_AGC_FE_CONFIG,
	CMD_SI475x_FM_AGC_STATUS,

	CMD_SI475x_FM_R8000,
	CMD_SI475x_FM_R8001,
	CMD_SI475x_FM_SEEK_TUNE_RSSI_RESET,
	CMD_SI475x_FM_SEEK_TUNE_RSSI_SET,
	CMD_SI475x_FM_SEEK_TUNE_SNR_RESET,
	CMD_SI475x_FM_SEEK_TUNE_SNR_SET,

	CMD_SI475x_SET_FM_DEEMPHASIS_50US,
	CMD_SI475x_SET_FM_DEEMPHASIS_75US,
	CMD_SI475x_FM_TUNE_ERROR,
	CMD_SI475x_FM_SOFT_MUTE_ATTENUATION,
	CMD_SI475x_FM_SOFT_MUTE_TRIGGER_THRESHOLD,
	CMD_SI475x_FM_SOFT_MUTE_END_THRESHOLD,

	CMD_SI475x_FM_SEEK_START,
	CMD_SI475x_FM_SEEK_STOP,
	CMD_SI475x_FM_SEEK_BAND_BOTTOM,
	CMD_SI475x_FM_SEEK_BAND_TOP,
	CMD_SI475x_FM_SEEK_BAND_STEP,
	
	CMD_SI475x_FM_BLEND_RSSI_STEREO,
	CMD_SI475x_FM_BLEND_RSSI_MONO,
	CMD_SI475x_FM_BLEND_RSSI_STEREO_SEP,
	CMD_SI475x_FM_BLEND_RSSI_ATTACK_RATE,
	CMD_SI475x_FM_BLEND_RSSI_RELEASE_RATE,
	
	CMD_SI475x_FM_BLEND_MULTIPATH_ATTACK_RATE,
	CMD_SI475x_FM_BLEND_MULTIPATH_RELEASE_RATE,
	CMD_SI475x_FM_BLEND_MULTIPATH_STEREO,
	CMD_SI475x_FM_BLEND_MULTIPATH_MONO,
	CMD_SI475x_FM_BLEND_MULTIPATH_STEREO_SEP,
	
	CMD_SI475x_FM_HICUT_RSSI_THRESHOLDS,
	CMD_SI475x_FM_HICUT_RSSI_CUTOFF_FREQ,
	CMD_SI475x_FM_HICUT_RSSI_ATTACK_TIME_,
	CMD_SI475x_FM_HICUT_RSSI_RELEASE_TIME_,
	
	CMD_SI475x_FM_HICUT_MULTIPATH_THRESHOLDS,
	CMD_SI475x_FM_HICUT_MULTIPATH_RSSI_CUTOFF_FREQ,
	CMD_SI475x_FM_HICUT_MULTIPATH_RSSI_ATTACK_TIME_,
	CMD_SI475x_FM_HICUT_MULTIPATH_RSSI_RELEASE_TIME_,

	CMD_SI475x_FM_HICUT_USN_THRESHOLDS,
	CMD_SI475x_FM_HICUT_USN_RSSI_CUTOFF_FREQ,
	CMD_SI475x_FM_HICUT_USN_RSSI_ATTACK_TIME_,
	CMD_SI475x_FM_HICUT_USN_RSSI_RELEASE_TIME_,
	
	CMD_SI475x_FM_HIBLEND_RSSI_THRESHOLDS,
	CMD_SI475x_FM_HIBLEND_RSSI_CUTOFF_FREQ,
	CMD_SI475x_FM_HIBLEND_RSSI_ATTACK_TIME_,
	CMD_SI475x_FM_HIBLEND_RSSI_RELEASE_TIME_,

	CMD_SI475x_FM_HIBLEND_MULTIPATH_THRESHOLDS,
	CMD_SI475x_FM_HIBLEND_MULTIPATH_CUTOFF_FREQ,
	CMD_SI475x_FM_HIBLEND_MULTIPATH_ATTACK_TIME_,
	CMD_SI475x_FM_HIBLEND_MULTIPATH_RELEASE_TIME_,

	CMD_SI475x_FM_HIBLEND_USN_THRESHOLDS,
	CMD_SI475x_FM_HIBLEND_USN_CUTOFF_FREQ,
	CMD_SI475x_FM_HIBLEND_USN_ATTACK_TIME_,
	CMD_SI475x_FM_HIBLEND_USN_RELEASE_TIME_,

#if defined(_ENABLE_RADIO_RDS_)	
/*-------------------------------------------------------------------------------------------------------------------------*/
//  RDS命令
	CMD_SI475x_FM_RDS_DETECT,
	CMD_SI475x_FM_RDS_STATUS,
	CMD_SI475x_FM_RDS_INT_SOURCE,
	CMD_SI475x_FM_RDS_FIFO_COUNT,
	CMD_SI475x_FM_RDS_CONFIG,
#endif

/*-------------------------------------------------------------------------------------------------------------------------*/
//	调幅命令
	CMD_SI475x_SET_AM_FREQ,
	CMD_SI475x_AM_RSQ_STATUS,
	CMD_SI475x_AM_SOFT_MUTE_RATE,
	CMD_SI475x_AM_SOFT_MUTE_SLOPE,
	CMD_SI475x_AM_SOFT_MUTE_ATTENUATION,
	CMD_SI475x_AM_SOFT_MUTE_TRIGGER_THRESHOLD,
	CMD_SI475x_AM_SOFT_MUTE_END_THRESHOLD,

	CMD_SI475x_AM_SEEK_TUNE_RSSI_RESET,
	CMD_SI475x_AM_SEEK_TUNE_RSSI_SET,
	CMD_SI475x_AM_SEEK_TUNE_SNR_RESET,
	CMD_SI475x_AM_SEEK_TUNE_SNR_SET,

	CMD_SI475x_AM_SEEK_START,
	CMD_SI475x_AM_SEEK_STOP,
	CMD_SI475x_AM_SEEK_BAND_BOTTOM,
	CMD_SI475x_AM_SEEK_BAND_TOP,
	CMD_SI475x_AM_SEEK_BAND_STEP,
	CMD_SI475x_GET_PART_INFO, 
	CMD_SI475x_AM_DEMOD_MODE,
};

/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

void CopyComToSi475xOrder(si475x_order_e Order)
{
	if(Order < SI475x_CMD_ORDER_MAX)
	{
		const uint8 *tab_buf = CMD_SI475x_TAB[Order];
		uint8 length = tab_buf[0];
		uint8 index;
		
		for(index = 0; index < length; index++)
		{
			Si475xOrder->buffer[index] = tab_buf[1+index];
		}
		Si475xOrder->length = length;
	}
}

static uint8 SendDataToSi475x(void)
{
	i2c_data_init(SI475x_I2C_ADDR, SI475x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(Si475xOrder->buffer, Si475xOrder->length);
	i2c_data_exit();
	return result;
}

static uint8 ReceDataForSi475x(uint8 length)
{
	i2c_data_init(SI475x_I2C_ADDR, SI475x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_r(Si475xOrder->buffer, length);
	i2c_data_exit();
	return result;
}

static uint8 WaitSi475xCts(void)
{
	uint16 i;
	
	ew_puts("WaitSi475xCts\n");
	for(i = 0; i < 2500; i++)
	{
		if(ReceDataForSi475x(1))
		{
			if(Si475xOrder->buffer[0] & _B7_)return TRUE;
		}
		else break;
	}
	ew_puts("WaitSi475xCts1111\n");
	return FALSE;
}

static uint8 SendSi475xData(void)
{
	ew_puts("SendSi475xData\n");
	if(!SendDataToSi475x())return FALSE;
	ew_puts("SendSi475xData111\n");
	if(!WaitSi475xCts())return FALSE;
	ew_puts("SendSi475xData222\n");
	return TRUE;
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if 1 //客户提供的参数
static const uint16 SI475xFMProperty[][2] =
{
    {0x0300, 0x003F},
	{0x0301, 0x0000},
    {0x0302, 0x0001},
	{0x0400, 0x0008},
	{0x0401, 0x0008},
	{0x0402, 0x0000},
	{0x0403, 0x01F4},
	{0x0404, 0x0078},
	{0x0405, 0x0080},
	{0x0406, 0x0008},
	{0x0407, 0x0000},
	{0x0408, 0x01F4},
	{0x0409, 0x0078},
	{0x040A, 0x0000},
	{0x040B, 0x0000},
	{0x040C, 0x0001},
	{0x040D, 0x0001},
	{0x040E, 0x335A},
	{0x040F, 0x0000},
	{0x0410, 0x0080},
	{0x0411, 0x0100},
    {0x0700, 0x0080},
	{0x0701, 0x0270},
	{0x0702, 0x0004},
	{0x0703, 0x0050},
	{0x0704, 0x4F05},
    {0x0705, 0x00F8},
    {0x0706, 0x00F8},
	{0x0707, 0x5103},
	{0x0710, 0x0000},
	{0x071B, 0x0000},
    {0x071C, 0x0057},
	{0x071D, 0x140E},
    {0x1006, 0x0050},
	{0x1007, 0x0050},
	{0x1008, 0x0096},
	{0x1100, 0x222E},
    {0x1101, 0x2A30},
	{0x1102, 0x000A},
    {0x1104, 0x006E},
	{0x1200, 0x0000},
	{0x1201, 0x007F},
	{0x1202, 0xFF80},
	{0x1203, 0x007F},
	{0x1204, 0xFF80},
	{0x1207, 0x007F},
	{0x1208, 0x0000},
	{0x1300, 0x0000},
	{0x1301, 0x001F},
	{0x1302, 0x0000},
	{0x1303, 0x0000},
	{0x1304, 0x0000},
	{0x1305, 0x0000},
    {0x1306, 0x0004},
	{0x2000, 0x004B},
	{0x2001, 0x0004},
	{0x2002, 0x0004},
	{0x2003, 0x0000},
	{0x2004, 0x0000},
	{0x2005, 0x001E},
	{0x2006, 0x0064},
	{0x2007, 0x0002},
	{0x2008, 0x0078},
	{0x2009, 0x0000},
	{0x200A, 0x503C},
	{0x200B, 0xFFFF},
	{0x2100, 0x0012},
    {0x2101, 0x000A},
    {0x2105, 0x0CCD},
    {0x2106, 0x0019},
    {0x2109, 0x0001},
    {0x210A, 0x000A},
    {0x210B, 0x1210},
    {0x2200, 0x9637},
	{0x2201, 0x000C},
	{0x2202, 0xFFFC},
	{0x2203, 0x0010},
	{0x2204, 0x0800},
    {0x2205, 0x963C},
	{0x2206, 0x0007},
	{0x2207, 0x0014},
	{0x2208, 0x0C80},
	{0x2209, 0x0140},
	{0x220A, 0x9650},
	{0x220B, 0xFFF6},
	{0x220C, 0x000A},
	{0x220D, 0x0C80},
	{0x220E, 0x0140},
	{0x220F, 0x100D},
	{0x2210, 0x140A},
	{0x2214, 0x0200},
	{0x2215, 0x9680},
	{0x2216, 0x0008},
	{0x2217, 0x0013},
	{0x2218, 0x1000},
	{0x2219, 0x0200},
	{0x221A, 0x9646},
	{0x221B, 0x0032},
	{0x221C, 0x005A},
	{0x221D, 0x0800},
	{0x221E, 0x0080},
	{0x221F, 0x9696},
	{0x2220, 0x0000},
	{0x2221, 0x0000},
	{0x2222, 0x0001},
	{0x2223, 0x0002},
	{0x3105, 0x0000},
	{0x3106, 0x0000},
    {0x3200, 0x0604},
    {0x3207, 0x0002},
    {0x3208, 0xFDF9},
    {0x3209, 0x0804},
    {0x320A, 0x0001},
	{0x320B, 0x0001},
    {0x320C, 0x0501},
    {0x320D, 0x0804},
	{0x320E, 0x0001},
	{0x320F, 0x0001},
    {0x3400, 0x800D},
	{0x3401, 0xFCFA},
	{0x3402, 0x0000},
    {0x3500, 0x1E3C},
	{0x3501, 0x2D00},
	{0x3502, 0x0010},
	{0x3503, 0x0FA0},
    {0x3508, 0x1E3C},
	{0x3509, 0x2D14},
	{0x350A, 0x0010},
	{0x350B, 0x0FA0},
	{0x3510, 0x341B},
	{0x3511, 0x2D00},
	{0x3512, 0x0010},
	{0x3513, 0x0FA0},
    {0x3600, 0x230F},
	{0x3601, 0xB428},
	{0x3602, 0x0010},
    {0x3603, 0x07D0},
    {0x3608, 0x0A14},
    {0x3609, 0xB450},
    {0x360A, 0x0010},
    {0x360B, 0x07D0},
    {0x3610, 0x250F},
    {0x3611, 0xB450},
	{0x3612, 0x0010},
    {0x3613, 0x07D0},
    {0x3700, 0x371B},
	{0x3701, 0xB41E},
	{0x3702, 0x0004},
    {0x3703, 0x0FA0},
    {0x3708, 0x050F},
    {0x3709, 0x6450},
	{0x370A, 0x0004},
    {0x370B, 0x0FA0},
	{0x3710, 0x371E},
    {0x3711, 0x961E},
	{0x3712, 0x0004},
    {0x3713, 0x0FA0},
	{0x4000, 0x0001},
	{0x4001, 0x0001},
	{0x4002, 0x0001},
	{0x4003, 0x1111},
};

#else
static const uint16 SI475xFMProperty[][2] =
{
    {0x0302, 0x0001},
    {0x0705, 0x00F8},
    {0x0706, 0x00F8},
    {0x071C, 0x0057},
    {0x1101, 0x2A30},
    {0x1104, 0x0050},
    {0x1306, 0x0003},
    {0x2101, 0x000A},
    {0x2105, 0x2000},
    {0x2106, 0x0019},
    {0x2109, 0x0001},
    {0x210A, 0x000A},
    {0x210B, 0x1210},
    {0x2205, 0x963C},
    {0x3200, 0x0604},
    {0x3207, 0x0002},
    {0x3208, 0xFDF9},
    {0x3209, 0x0804},
    {0x320C, 0x0501},
    {0x320D, 0x0804},
    {0x3400, 0x800D},
    {0x3508, 0x0A14},
    {0x3603, 0x07D0},
    {0x3608, 0x0A14},
    {0x3609, 0x961E},
    {0x360B, 0x07D0},
    {0x3611, 0xB41E},
    {0x3613, 0x07D0},
    {0x3703, 0x0FA0},
    {0x3708, 0x050F},
    {0x3709, 0x961E},
    {0x370B, 0x0FA0},
    {0x3711, 0x961E},
    {0x3713, 0x0FA0},
 /*   {0x0302, 0x0001},
    {0x0705, 0x00F8},
    {0x0706, 0x00F8},
    {0x071C, 0x0057},
    {0x1101, 0x2A30},
    {0x1104, 0x0014},//0x0050
    {0x1306, 0x0000},//0x0003     0x0001
    {0x2000, 0x007E},//add
    {0x2101, 0x0004},//0x000A
    {0x2105, 0x0CCD},
    {0x2106, 0x0019},
    {0x2109, 0x1200},//0x0001
    {0x210A, 0x000A},
    {0x210B, 0x1210},
    {0x2205, 0x3232},//0x963C
    {0x3200, 0x0604},
    {0x3207, 0x0002},
    {0x3208, 0xFDF9},
    {0x3209, 0x0804},
    {0x320C, 0x0501},
    {0x320D, 0x0804},
    {0x3400, 0x800D},
    {0x3508, 0x0A14},
    {0x3603, 0x07D0},
    {0x3608, 0x0A14},
    {0x3609, 0x961E},
    {0x360B, 0x07D0},
    {0x3611, 0xB41E},
    {0x3613, 0x07D0},
    {0x3703, 0x0FA0},
    {0x3708, 0x050F},
    {0x3709, 0x961E},
    {0x370B, 0x0FA0},
    {0x3711, 0x961E},
    {0x3713, 0x0FA0},
    {0x0501, 0x4272},//add
    {0x0700, 0xF094},//add
    {0x070E, 0x5904},//add*/
};
#endif

static const UINT16 SI475xAMProperty[][2] =
{
	{0x0501,0x4272},
	{0x070E,0x5904},
	{0x1100,0x020A},
	{0x1101,0x06AE},
	{0x1102,0x0009},
	{0x2000,0x007E},
	{0x2109,0x1200},
	{0x2300,0x0100},
};

/// Si475x-A40 FM 0xA3F6 patch.
static const uint8 SI475xFMPatch[] = 
{
    0x04,0x21,0x0B,0xFC,0x00,0x00,0x01,0xC6,
    0x05,0x8E,0xE4,0x2C,0x01,0x33,0x83,0x92,
    0x15,0x29,0x80,0xAB,0xBC,0xF3,0xD5,0xD8,
    0x05,0x81,0xC2,0xFB,0x6B,0xE5,0x4B,0x9B,
    0x1A,0xCD,0xA9,0x24,0xC6,0x52,0x0F,0x39,
    0x05,0x59,0x2E,0xB3,0x71,0xD7,0xD2,0x30,
    0x1A,0xBD,0x36,0x32,0xDA,0x63,0x5A,0x75,
    0x05,0x1D,0x8A,0x1E,0xC5,0x77,0xC0,0x31,
    0x1A,0x70,0xCA,0xA2,0xD0,0x75,0xD4,0xC1,
    0x05,0x4B,0xA1,0xAD,0x8D,0x40,0xD0,0x75,
    0x17,0x43,0x0E,0xF5,0xD2,0x83,0x89,0x1A,
    0x16,0xE5,0xB0,0x81,0x9C,0x5D,0xD8,0x45,
    0x05,0xE0,0x60,0x44,0xA2,0x25,0xB2,0xEA,
    0x1F,0x87,0x68,0xE5,0x15,0x26,0xE7,0xDF,
    0x17,0x82,0xE2,0xE5,0xD3,0xCC,0x07,0x65,
    0x1F,0x1F,0x30,0xB3,0xE6,0xE9,0xC3,0xFE,
    0x1F,0xE6,0xE8,0x2F,0xEC,0x46,0xFB,0xFB,
    0x17,0xCD,0x59,0x15,0xA7,0x89,0x70,0xCB,
    0x17,0x8E,0x76,0x41,0xB4,0xD1,0xCF,0x5A,
    0x17,0x26,0xE1,0xD8,0x36,0xB0,0x70,0xE9,
    0x17,0x62,0xAD,0x4D,0x42,0x82,0xB2,0x8D,
    0x17,0x16,0xAD,0x08,0x0A,0x05,0x83,0x4B,
    0x1F,0xDC,0xAF,0x54,0xDB,0x56,0xB3,0x83,
    0x1F,0x8C,0x9E,0xA2,0xE2,0x9F,0xCF,0xE3,
    0x1F,0x2A,0xA3,0x12,0xB3,0xE5,0x7A,0x49,
    0x17,0xE4,0xD2,0xD9,0xC8,0x53,0x33,0xE1,
    0x17,0xF3,0xEB,0xB6,0xBA,0x56,0xEA,0x37,
    0x17,0x59,0x8D,0x5F,0x3D,0x75,0xE5,0xF1,
    0x17,0xED,0x7C,0x7B,0x29,0x8D,0x83,0x62,
    0x1F,0x26,0x5F,0x93,0xD1,0xC4,0x23,0xAC,
    0x1F,0xAD,0x03,0x3B,0x84,0x2B,0x7E,0x65,
    0x17,0x51,0x48,0xDA,0x6D,0x3F,0x88,0x43,
    0x17,0x29,0x09,0x39,0xBB,0x2B,0x75,0xF2,
    0x1F,0x42,0xCB,0xA1,0xFD,0xEC,0xC7,0x6E,
    0x17,0x5D,0x44,0x75,0x99,0x41,0xC9,0xC3,
    0x17,0x20,0xCA,0x40,0xE0,0xFD,0x7F,0x3A,
    0x1F,0xB8,0x7F,0xB3,0xBE,0x94,0xC5,0xF0,
    0x1F,0xF2,0xFC,0x0D,0x4C,0x07,0xDB,0x34,
    0x1F,0xD3,0x60,0x1F,0xC3,0x7D,0x77,0x36,
    0x17,0x29,0xFF,0x5B,0x98,0xA2,0x77,0x22,
    0x17,0x8D,0x5A,0x7B,0x18,0xAB,0x5C,0x74,
    0x1F,0x6E,0xC1,0xFC,0x50,0xB9,0x0D,0xB4,
    0x1F,0x2E,0x60,0x2A,0xAA,0xFA,0x6A,0x18,
    0x1F,0x3B,0xD3,0xE9,0xE8,0xB1,0x5A,0xA6,
    0x1F,0x2C,0x86,0x0D,0xB9,0xD3,0xCC,0x45,
    0x1F,0xBC,0xC9,0x98,0x7F,0x74,0xD1,0xE1,
    0x1F,0x9D,0xD7,0x0A,0x25,0xD0,0x66,0x14,
    0x17,0x9D,0xA3,0xD5,0x72,0x67,0xF0,0x79,
    0x1F,0xCB,0x71,0xB1,0x03,0xFB,0x83,0x8C,
    0x17,0x1B,0xC1,0xE7,0x7E,0x08,0xCC,0x73,
    0x17,0xFA,0xD9,0xC5,0x34,0xFC,0xDC,0x63,
    0x17,0x89,0xDF,0x06,0x97,0x9E,0x5F,0x39,
    0x1F,0xDE,0xFA,0x8C,0xF2,0x82,0x0A,0xC6,
    0x1F,0x42,0x35,0xBD,0x5E,0xAD,0xB2,0xD3,
    0x17,0xEB,0x5F,0x8A,0xB8,0x72,0x7E,0xD6,
    0x1F,0x95,0x56,0x2B,0xE4,0xEC,0xA6,0xF8,
    0x17,0xB4,0xD3,0x28,0xAC,0x16,0x73,0x17,
    0x1F,0xE1,0x0D,0x13,0xA0,0x8B,0x1E,0x80,
    0x17,0x01,0xB2,0xD9,0xBE,0x9F,0x56,0x7C,
    0x17,0x74,0xEF,0xB1,0x30,0x4F,0xC0,0xB2,
    0x1F,0x88,0x94,0xE4,0x44,0x4B,0xCD,0x10,
    0x17,0xD5,0x82,0x95,0xC6,0xDA,0x0C,0x14,
    0x17,0x14,0xA1,0x92,0xD7,0xEE,0x3C,0x9E,
    0x1F,0xA0,0x5A,0xC5,0xF1,0xAE,0x8A,0xAE,
    0x17,0x55,0xCB,0x54,0x91,0xB7,0x23,0x3F,
    0x17,0xB9,0x33,0x03,0xD6,0xEC,0x41,0x25,
    0x17,0x41,0xB8,0x97,0x8E,0x8E,0x84,0xFE,
    0x19,0x90,0xFC,0x35,0x77,0x4C,0x71,0x18,
    0x05,0xE3,0xE2,0xA9,0x6A,0x9A,0xF6,0x85,
    0x1B,0x44,0x0B,0x62,0x0A,0x10,0x3A,0xAD,
    0x05,0x8F,0xD8,0x7A,0x22,0x53,0x84,0x03,
    0x1A,0xE6,0xA5,0x29,0xD4,0x57,0x6A,0x31,
    0x05,0xCE,0x5C,0x54,0x8F,0x7F,0x4D,0xCE,
    0x1D,0x72,0x61,0x22,0x5D,0x4B,0x1D,0x43,
    0x05,0x5B,0xAF,0x2A,0xAB,0x3F,0xA0,0x6C,
    0x1D,0x77,0xC5,0x61,0x4F,0x61,0xDA,0xDC,
    0x05,0x45,0x9A,0x9B,0x8A,0x2C,0x30,0xCB,
    0x17,0x57,0xB6,0xFB,0xDB,0x56,0xAE,0x93,
    0x13,0x8D,0x01,0x1A,0xE7,0x68,0x3C,0x7D,
    0x05,0x37,0x99,0x6E,0xA4,0x8A,0x5B,0x38,
    0x1F,0xD9,0xDB,0x36,0xB2,0xF4,0x2A,0x6E,
    0x1F,0x71,0x5D,0x30,0x70,0xBD,0xD1,0x94,
    0x1F,0x3C,0x59,0xB8,0xFA,0xCB,0x24,0x93,
    0x17,0x4D,0x67,0x97,0xD7,0xF1,0xD8,0x51,
    0x1F,0x19,0x5D,0x9E,0x45,0x43,0x15,0xA7,
    0x1F,0xAB,0xC1,0xAF,0xCE,0x93,0xBA,0x79,
    0x17,0x8E,0x34,0x74,0x27,0x57,0x19,0x95,
    0x1F,0x1B,0x0F,0x56,0x0D,0xD8,0x05,0x59,
    0x17,0x53,0xFA,0x4F,0xBF,0x33,0xF4,0xB3,
    0x17,0x9B,0x9C,0x66,0x04,0x3B,0xB4,0x17,
    0x1F,0xB4,0xA8,0xD7,0xE9,0x02,0x79,0x37,
    0x17,0xE7,0x6A,0xFB,0x1F,0x65,0x70,0xCF,
    0x1F,0xBF,0x54,0x20,0x97,0xBD,0xC5,0xD8,
    0x1F,0x59,0x91,0x7B,0xB2,0xD9,0x33,0xD9,
    0x17,0xDB,0x2A,0xA0,0xF0,0x80,0x18,0xC4,
    0x1F,0x26,0xA9,0xD2,0x8F,0x36,0x3C,0xA4,
    0x17,0x24,0x0C,0xE2,0xE9,0xDC,0xA9,0xEC,
    0x1F,0x1F,0xCE,0x49,0x80,0x31,0xDE,0x21,
    0x17,0x93,0xCF,0x1A,0xC5,0xC5,0xFE,0x2C,
    0x17,0x33,0xEC,0x4A,0xD4,0x49,0x44,0x98,
    0x17,0x3D,0x4B,0x30,0x30,0x21,0xD6,0x42,
    0x17,0xB3,0x74,0x93,0x6A,0x13,0x67,0x92,
    0x17,0x73,0xE6,0xBB,0x8C,0x97,0x2B,0x06,
    0x1F,0xE7,0xF3,0xE8,0x0F,0x9A,0x78,0x0F,
    0x17,0x48,0x08,0x20,0x45,0xD9,0x49,0x55,
    0x17,0x43,0xDC,0xD6,0x23,0x2E,0xE4,0xD6,
    0x17,0xAC,0x2F,0xEE,0xA1,0x49,0x89,0x2C,
    0x17,0x40,0x1E,0xB8,0x28,0xB2,0xD7,0xB5,
    0x1F,0xF7,0x8A,0x71,0xF3,0xC7,0x79,0x8D,
    0x1F,0xFA,0xF0,0xBE,0x3C,0x7F,0x19,0xA5,
    0x1F,0x98,0x8E,0xF2,0xC6,0x59,0x51,0x03,
    0x1F,0x0A,0x8A,0x62,0x21,0x45,0xB9,0x0E,
    0x1F,0xD7,0x9F,0x43,0x64,0xA6,0x82,0xA8,
    0x17,0x54,0x23,0x8D,0x52,0xED,0x34,0x21,
    0x17,0x47,0x1A,0x29,0x79,0xD3,0xD0,0x88,
    0x17,0xCE,0x3A,0xC6,0x4C,0x26,0x2D,0x54,
    0x1F,0xE8,0x35,0x86,0x53,0xD3,0xDC,0x3B,
    0x1F,0x5A,0x73,0xEE,0x35,0x67,0x98,0xD6,
    0x17,0x76,0x89,0x33,0x8C,0xDF,0x35,0x88,
    0x17,0x9A,0x8C,0xF2,0xFC,0x55,0xE9,0xE4,
    0x17,0x03,0x60,0xDB,0x61,0x74,0x5F,0x7C,
    0x1F,0x55,0x0B,0x40,0x5A,0x29,0x7F,0x07,
    0x17,0xBD,0xC9,0xAD,0x36,0xAB,0xF8,0xCD,
    0x17,0xE5,0x2A,0x78,0xDF,0x83,0x0D,0xB3,
    0x17,0x68,0x8B,0xBA,0x5A,0xB6,0x95,0x33,
    0x17,0x5B,0x68,0x2C,0xD7,0x35,0x03,0x7D,
    0x1F,0x82,0x37,0x12,0x6D,0xF2,0xAA,0x6C,
    0x1F,0xC6,0xA0,0xD5,0x18,0xE0,0xA2,0x91,
    0x1F,0x59,0xBB,0xE5,0x2C,0xD3,0xA3,0x8E,
    0x17,0x2C,0xB9,0x9B,0xD3,0xDD,0xCC,0x3F,
    0x1F,0x10,0x79,0x82,0xAF,0x6D,0x7E,0x57,
    0x1F,0x77,0xD8,0x3F,0x91,0x75,0x43,0xFA,
    0x1F,0x6C,0x4E,0x8F,0x14,0x0B,0x3E,0xE3,
    0x17,0x2F,0xEC,0x53,0xDA,0x63,0xAE,0x4B,
    0x1F,0x09,0x3E,0xF5,0xBE,0x0F,0xF0,0xA6,
    0x1F,0x75,0x77,0xF3,0x3A,0xA4,0x52,0xFF,
    0x17,0x74,0xBE,0x59,0x2B,0xED,0x60,0x1C,
    0x1F,0x24,0x2C,0x94,0x90,0x75,0xD1,0x4A,
    0x1F,0xF1,0x4D,0x48,0x6A,0xA7,0x46,0x05,
    0x17,0xA8,0x3C,0x89,0x1E,0xE4,0x26,0xB0,
    0x1F,0xA8,0xE3,0x47,0xA2,0xD3,0xEB,0x66,
    0x1F,0xA2,0xFA,0x35,0x40,0xDD,0xF2,0xE5,
    0x1F,0x87,0x9A,0xED,0x0F,0xB4,0x43,0x3D,
    0x1F,0x6E,0x96,0x54,0x53,0x08,0x90,0x6D,
    0x1F,0xC4,0xD3,0xCD,0x2C,0xC6,0x22,0xEE,
    0x17,0x0B,0x89,0x40,0x8B,0x6C,0x2C,0x85,
    0x1F,0x6E,0xBA,0xCA,0xE0,0xA5,0x12,0x51,
    0x17,0xE8,0x64,0x4C,0x60,0xB7,0x68,0x8A,
    0x17,0x61,0x26,0x48,0x1C,0x68,0x99,0x48,
    0x17,0x79,0x29,0x01,0x04,0x82,0x03,0xB9,
    0x1F,0xF5,0x1E,0x94,0x4F,0x79,0xB0,0x08,
    0x17,0x12,0xEF,0xE6,0x82,0x9A,0x9E,0x1C,
    0x1F,0x15,0x85,0x91,0x0B,0xD8,0x03,0xFF,
    0x1F,0xB6,0xFB,0xF0,0x05,0x00,0x08,0x19,
    0x17,0xD4,0x17,0xB5,0x91,0xAE,0x72,0x6E,
    0x17,0x02,0x03,0x2F,0x50,0x85,0x0C,0x56,
    0x17,0xA4,0x9B,0x71,0x0F,0xBA,0x56,0xD8,
    0x17,0x52,0x6E,0x09,0xF9,0xC1,0x2D,0x5E,
    0x17,0x9E,0xEF,0xAB,0x9B,0xA5,0x82,0x2E,
    0x17,0xC4,0xA3,0x10,0xBA,0x46,0x8C,0xBD,
    0x1F,0xB2,0x07,0xF9,0x00,0xB4,0x8F,0xD5,
    0x17,0xBB,0xAD,0x97,0x49,0x52,0xFB,0x15,
    0x17,0x81,0xEC,0x35,0x7D,0x93,0x7B,0x27,
    0x1F,0x22,0x76,0xF3,0xCB,0xBA,0x9C,0x10,
    0x1F,0x0D,0xA7,0x0D,0xC6,0xAB,0x0C,0x4C,
    0x17,0x56,0x8F,0xB4,0x6D,0xF1,0xB9,0x0E,
    0x17,0x9F,0x0D,0x81,0x71,0xB5,0x1A,0x8B,
    0x17,0xFD,0x7F,0x78,0x5A,0x75,0x73,0x77,
    0x1F,0xC6,0xC6,0xDD,0x2D,0xB8,0xA0,0xC9,
    0x17,0xA9,0x8D,0x3C,0x79,0xBF,0x4A,0xC7,
    0x17,0x33,0x1D,0x6D,0x65,0x78,0x5E,0x9C,
    0x17,0xAB,0x7F,0xBA,0x5A,0x62,0x19,0x57,
    0x17,0x9F,0x79,0x68,0xB9,0x5C,0xE5,0x82,
    0x1F,0xBA,0xD1,0xC5,0x2A,0xCA,0x8D,0xBB,
    0x1F,0x9B,0xA1,0x2C,0xB2,0x85,0x66,0x04,
    0x17,0x01,0xF7,0xCB,0xA0,0x76,0xD8,0x5F,
    0x1F,0xA7,0x55,0x2F,0xDA,0x3E,0xE8,0x58,
    0x1F,0x26,0xA7,0x62,0x16,0xC1,0x1A,0xBE,
    0x1F,0x18,0x9D,0xE6,0xAB,0xE6,0x87,0x51,
    0x1F,0xD8,0xDE,0x02,0xC0,0x51,0x3B,0xF4,
    0x17,0x0A,0x2E,0x3D,0xBF,0x43,0xC4,0x2F,
    0x17,0x64,0x84,0x9B,0x1F,0xFE,0x74,0xAA,
    0x17,0x7C,0x0C,0x1A,0x99,0xF8,0x4C,0xB5,
    0x17,0x2C,0xC5,0xEF,0xA7,0x94,0xC8,0x6D,
    0x1F,0x27,0xE0,0x72,0x2A,0x16,0x71,0x7E,
    0x17,0xAB,0xF2,0xDF,0x14,0x9A,0x8B,0x4E,
    0x1F,0x5C,0xBB,0xFA,0xC2,0xFC,0x74,0x06,
    0x1F,0x53,0x63,0x1E,0x82,0xE4,0x5D,0xE3,
    0x17,0xD8,0x57,0xF9,0xD3,0x11,0x81,0x9C,
    0x1F,0xCE,0xF4,0x5F,0x16,0x4F,0xFC,0x8F,
    0x17,0xF5,0xD0,0xA2,0x47,0x20,0xC5,0x46,
    0x17,0x4F,0x79,0x9D,0x74,0x05,0xAB,0x81,
    0x17,0x53,0xF2,0x64,0xCC,0xDC,0x9A,0x45,
    0x17,0xBD,0x98,0x0D,0x3F,0xEB,0x8C,0x43,
    0x1F,0xDE,0x27,0x0F,0xF3,0x93,0xCC,0xF8,
    0x17,0x4A,0x9F,0x6C,0x07,0xF9,0x3B,0x0B,
    0x17,0xD8,0x72,0x63,0x04,0xFA,0x74,0x02,
    0x1F,0x7D,0x4E,0x64,0x74,0x3D,0xF8,0xB5,
    0x17,0xEC,0xBB,0xB2,0x6D,0x77,0x38,0x91,
    0x17,0x38,0xD4,0xF0,0x60,0x32,0x14,0xB5,
    0x1F,0x8B,0x73,0x97,0x99,0xE4,0x4F,0x90,
    0x1F,0xD5,0xD8,0x2D,0x94,0x5F,0x3B,0x52,
    0x1F,0xA3,0xAE,0x08,0x5A,0x6C,0xF5,0xD6,
    0x17,0x59,0x14,0x99,0x08,0x64,0xDE,0x27,
    0x1F,0xD0,0xCB,0xA1,0xED,0x80,0xDB,0x82,
    0x1F,0x1A,0x00,0xE3,0x06,0xB7,0xA4,0x91,
    0x1F,0xF8,0x5C,0x04,0x99,0x34,0x5B,0x38,
    0x17,0x36,0x6A,0x7B,0x8C,0x1E,0x86,0xB6,
    0x17,0x48,0x54,0x96,0x61,0x5F,0xD8,0x26,
    0x1F,0x94,0xAE,0xCA,0x5A,0x16,0xAD,0xED,
    0x17,0xF2,0x82,0xB3,0x6D,0x16,0xBA,0x8F,
    0x17,0xD8,0xCF,0xD0,0x73,0xE9,0x21,0xA8,
    0x17,0xAD,0xE4,0xF7,0x41,0x78,0x4B,0xD8,
    0x1F,0x4C,0x54,0x0E,0xE9,0x1A,0xBC,0xDF,
    0x1F,0x21,0x19,0x08,0x11,0x11,0x57,0xA5,
    0x1F,0x7C,0xEE,0x86,0x82,0xAC,0xE4,0x98,
    0x1F,0x31,0x80,0xF9,0x98,0x99,0x27,0x27,
    0x1F,0xFE,0xC9,0xB2,0x16,0x37,0x0F,0x09,
    0x1F,0x3F,0x3D,0xBA,0x15,0x8A,0xFD,0x8B,
    0x1F,0xA0,0x2F,0x65,0x89,0x61,0x17,0xDC,
    0x17,0xBD,0x9C,0x21,0x9A,0x86,0x67,0x98,
    0x1F,0x8D,0xC3,0x45,0x02,0x64,0xA4,0xFD,
    0x1F,0xC9,0x38,0x54,0x60,0x21,0x7B,0x6C,
    0x17,0x54,0x26,0x7E,0xAC,0xAD,0xFC,0x9F,
    0x17,0x1B,0x7D,0x4F,0x77,0xA0,0x8B,0xCB,
    0x17,0x62,0x0D,0x8B,0x19,0x34,0x3E,0x30,
    0x1F,0xDB,0x7A,0x20,0x03,0x24,0xEC,0xF3,
    0x1F,0xD1,0x76,0xA0,0x09,0x57,0x94,0x73,
    0x17,0x47,0xD2,0xF7,0x05,0x59,0x41,0xBF,
    0x17,0x7D,0x16,0x1C,0xB4,0xC9,0xB2,0x32,
    0x1F,0xC3,0x76,0x3E,0x61,0xD5,0xAA,0x8E,
    0x17,0x21,0x5A,0x40,0xD6,0xC4,0x66,0xE4,
    0x17,0x04,0xFB,0x57,0xED,0x04,0xD8,0xE2,
    0x17,0x82,0xFE,0x66,0x9F,0x26,0xD1,0xF0,
    0x17,0xB2,0x6E,0xB0,0x05,0x6D,0xA4,0x31,
    0x17,0xB7,0xFF,0x9E,0xAC,0xF8,0x5F,0x12,
    0x1F,0xEE,0x14,0x27,0x64,0x7E,0x81,0x34,
    0x1F,0x4E,0xF8,0x3D,0x8F,0x41,0xE3,0x63,
    0x1C,0x0F,0xA5,0xB0,0xDE,0x82,0xD0,0x87,
    0x05,0xB6,0x27,0x26,0x2D,0x6D,0xEA,0xC2,
    0x1E,0xA8,0x78,0xD8,0x2C,0xAE,0x9C,0xB5,
    0x05,0x3F,0xE9,0x64,0x0D,0xB9,0x41,0x6B,
    0x17,0xB7,0x74,0x92,0xEE,0x5E,0x50,0xFE,
    0x1F,0x29,0xAB,0x83,0x9B,0xD6,0x8A,0xD4,
    0x17,0xCA,0x38,0x2A,0xBA,0x5D,0xC9,0xD9,
    0x13,0xF7,0x8E,0x87,0xB3,0x2B,0xFC,0x8F,
    0x05,0xA3,0x61,0x90,0xFD,0x9A,0xA3,0xF6
};

static const uint8 SI475xAMPatch[]  = {
	0x04,0x22,0xD9,0x26,0x00,0x00,0x96,0x6A,
	0x05,0x15,0x3A,0xD6,0x97,0x57,0x6D,0x03,
	0x1A,0x37,0x61,0xCF,0xEA,0x69,0xD7,0x13,
	0x05,0xE3,0xCD,0xD5,0xD0,0xDD,0x18,0x75,
	0x12,0x43,0xF6,0x5A,0x8E,0x95,0xC8,0x1B,
	0x05,0x40,0xFE,0x25,0xE8,0xE2,0x09,0x35,
	0x17,0x6D,0x80,0x6C,0x58,0x63,0x10,0x5C,
	0x1F,0x1E,0x7E,0x4E,0x39,0x45,0xE6,0x86,
	0x1F,0x10,0xB0,0xB5,0x48,0x16,0x6D,0x68,
	0x1F,0x47,0x8D,0x9D,0xAC,0x33,0x57,0x1A,
	0x1F,0xB9,0x17,0x6F,0x5F,0xD9,0x54,0x83,
	0x1F,0x10,0x36,0xEE,0x8B,0x4E,0x72,0x65,
	0x1F,0x4F,0x09,0x9D,0x2C,0x55,0x83,0x16,
	0x1F,0x5E,0x38,0xC9,0x47,0xC0,0xE6,0x0B,
	0x1F,0xE3,0xE7,0x9E,0x68,0x88,0x4B,0x8D,
	0x1F,0x0F,0x65,0xCA,0xB1,0x10,0x42,0x8D,
	0x17,0xE5,0xA0,0xAB,0x39,0x7C,0x05,0xA7,
	0x1F,0x55,0xB4,0x5C,0xCD,0x77,0xD3,0x98,
	0x1F,0x98,0xC2,0xB1,0xF2,0x75,0xFC,0x00,
	0x1F,0x84,0x7D,0x33,0x1C,0x5B,0x30,0xBC,
	0x17,0xAE,0x61,0xD1,0x2C,0xF4,0x9D,0x40,
	0x17,0x1F,0x59,0x54,0xB9,0x41,0x3F,0x6A,
	0x17,0xDE,0x47,0xF0,0xD3,0xBF,0xD3,0xD8,
	0x1F,0xF1,0x62,0x34,0x93,0xB9,0xCE,0x7D,
	0x1F,0xC8,0x11,0x88,0xF7,0xA8,0x17,0x0A,
	0x17,0xDB,0x88,0x81,0x7B,0xB9,0x69,0x5D,
	0x12,0xE1,0xF7,0xF9,0x20,0x6D,0x93,0x5E,
	0x05,0x5B,0x67,0x9C,0x5B,0x0D,0x89,0x54,
	0x12,0xD5,0x9F,0xDD,0x48,0x61,0x84,0xBE,
	0x05,0xDA,0x9F,0x7A,0xE9,0xC3,0x02,0x34,
	0x15,0x16,0xD3,0x0D,0x1C,0x4F,0xE9,0xCC,
	0x05,0x7F,0xB7,0xBA,0x88,0xAE,0x32,0xF3,
	0x1D,0x8F,0xE5,0x58,0x23,0x39,0x93,0xBF,
	0x05,0x40,0x4C,0x56,0x93,0x84,0x4E,0xA2,
	0x17,0xC1,0xF8,0xA6,0x76,0xDD,0x3F,0xDC,
	0x17,0x8D,0xD8,0x68,0xB6,0xAC,0x63,0x20,
	0x1F,0xD6,0x2D,0x57,0x96,0x74,0x93,0x73,
	0x1F,0x65,0x06,0x90,0x74,0xCA,0x0F,0x08,
	0x1F,0xFA,0xF9,0xFF,0x23,0x0B,0x65,0xC9,
	0x1F,0x5E,0x76,0xAF,0x78,0x79,0x2B,0x93,
	0x17,0xCF,0x47,0x5E,0x0B,0x63,0xE0,0xE6,
	0x1F,0x29,0x5C,0x53,0xC0,0x9E,0xC3,0x89,
	0x17,0xCC,0x6E,0x7A,0x21,0x9F,0x1E,0x0A,
	0x1F,0x6D,0x6A,0x53,0x4A,0xDB,0x2A,0x37,
	0x17,0xD3,0xA9,0xA9,0x96,0xF6,0xF2,0x29,
	0x17,0x1F,0xE0,0x0D,0x71,0x08,0x85,0xCD,
	0x17,0xDA,0x45,0x7E,0xF7,0xB8,0xB5,0x90,
	0x1F,0xA8,0x2E,0x92,0xA5,0x4F,0xC4,0x1F,
	0x17,0xC5,0xFF,0x90,0xA3,0x78,0x42,0x32,
	0x1F,0x2C,0x29,0x37,0xBD,0xC8,0xDF,0x28,
	0x17,0x21,0x7C,0x22,0x45,0xEF,0xC2,0xCA,
	0x17,0xD5,0x08,0x6A,0xD3,0x93,0xC6,0x70,
	0x1C,0xFA,0xCC,0xDA,0x7D,0x92,0x7F,0x51,
	0x05,0x1E,0x7A,0x64,0x59,0xF6,0x26,0xFC,
	0x1B,0x83,0x18,0x11,0x11,0x30,0xA5,0x24,
	0x05,0x91,0x7A,0xF7,0xD3,0xDE,0xBB,0x82,
	0x1B,0x4B,0x03,0x59,0x74,0xCF,0xED,0x85,
	0x05,0xE3,0x51,0x47,0x31,0x75,0xA5,0x58,
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 SetSi475xFMProperty(void)
{
	int i;
	
	for(i = 0; i < (sizeof(SI475xFMProperty)/4); i++)
	{
		Si475xOrder->length = 0;
		Si475xOrder->buffer[Si475xOrder->length++] = 0x13;
		Si475xOrder->buffer[Si475xOrder->length++] = 0x00;
		Si475xOrder->buffer[Si475xOrder->length++] = _u16to8h_(SI475xFMProperty[i][0]);
		Si475xOrder->buffer[Si475xOrder->length++] = _u16to8l_(SI475xFMProperty[i][0]);
		Si475xOrder->buffer[Si475xOrder->length++] = _u16to8h_(SI475xFMProperty[i][1]);
		Si475xOrder->buffer[Si475xOrder->length++] = _u16to8l_(SI475xFMProperty[i][1]);
		
		if(!SendSi475xData())return FALSE;
	}
	return TRUE;
}

static uint8 SetSi475xAMProperty(void)
{
	int i;
	
	for(i = 0; i < (sizeof(SI475xAMProperty)/4); i++)
	{
		Si475xOrder->length = 0;
		Si475xOrder->buffer[Si475xOrder->length++] = 0x13;
		Si475xOrder->buffer[Si475xOrder->length++] = 0x00;
		Si475xOrder->buffer[Si475xOrder->length++] = _u16to8h_(SI475xAMProperty[i][0]);
		Si475xOrder->buffer[Si475xOrder->length++] = _u16to8l_(SI475xAMProperty[i][0]);
		Si475xOrder->buffer[Si475xOrder->length++] = _u16to8h_(SI475xAMProperty[i][1]);
		Si475xOrder->buffer[Si475xOrder->length++] = _u16to8l_(SI475xAMProperty[i][1]);
		
		if(!SendSi475xData())return FALSE;
	}
	return TRUE;
}

static uint8 SetSi475xFMPatch(void)
{
	int i;
	
	for(i = 0; i < (sizeof(SI475xFMPatch)/8); i++)
	{
		Si475xOrder->length = 0;
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xFMPatch[i*8 + 0];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xFMPatch[i*8 + 1];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xFMPatch[i*8 + 2];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xFMPatch[i*8 + 3];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xFMPatch[i*8 + 4];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xFMPatch[i*8 + 5];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xFMPatch[i*8 + 6];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xFMPatch[i*8 + 7];
		
		if(!SendSi475xData())return FALSE;
	}
	return TRUE;
}

static uint8 SetSi475xAMPatch(void)
{
	int i;
	
	for(i = 0; i < (sizeof(SI475xAMPatch)/8); i++)
	{
		Si475xOrder->length = 0;
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xAMPatch[i*8 + 0];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xAMPatch[i*8 + 1];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xAMPatch[i*8 + 2];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xAMPatch[i*8 + 3];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xAMPatch[i*8 + 4];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xAMPatch[i*8 + 5];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xAMPatch[i*8 + 6];
		Si475xOrder->buffer[Si475xOrder->length++] = SI475xAMPatch[i*8 + 7];
		
		if(!SendSi475xData())return FALSE;
	}
	return TRUE;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

static uint8 SendSi475xOrder(si475x_order_e Order, uint8 read_length)
{
	uint8 result = TRUE;
	
	if(Order < SI475x_CMD_ORDER_MAX)
	{
		CopyComToSi475xOrder(Order);
		
		if((Order == SI475x_CMD_POWER_UP_REC_FM_STEP1)
		|| (Order == SI475x_CMD_POWER_UP_REC_FM_STEP3)
		|| (Order == SI475x_CMD_POWER_UP_REC_AM_STEP1)
		|| (Order == SI475x_CMD_POWER_UP_REC_AM_STEP3)
		|| (Order == SI475x_CMD_POWER_UP_REC_AM_STEP4))
		{
			result = SendDataToSi475x();			
		}
		else
		{
			result = SendSi475xData();	
		}
	}

	//ew_printf("result:%d\n",result);
	if(result)
	{
		if(read_length)
		{
			result = ReceDataForSi475x(read_length);
		}
	}
	//ew_printf("result11:%d\n",result);
	return result;
}


static uint8 GetSi475xStc(uint8 *stc)
{
	if(!SendSi475xOrder(SI475x_CMD_GET_INT_STATUS, 0))return FALSE;
	if(Si475xOrder->buffer[0] & _B0_)
	{
		*stc = TRUE;
	}
	else 
	{
		*stc = FALSE;
	}
	return TRUE;
}

static uint8 CheckSi475xStc(void)
{
	uint8 index;
	uint8 stc;
	
	for(index = 0; index < 50; index++)
	{
		if(!GetSi475xStc(&stc))return FALSE;
		if(stc)break;
		
		OSTimeDly(1);
	}
	
	if(index == 50)return FALSE;
	return TRUE;
}

static uint8 SendSi475xFmFreq(uint16 Freq)
{
	CopyComToSi475xOrder(SI475x_CMD_SET_FM_FREQ);
	
	Si475xOrder->buffer[Si475xOrder->length ++] = _u16to8h_(Freq);
	Si475xOrder->buffer[Si475xOrder->length ++] = _u16to8l_(Freq);
	if(!SendSi475xData())return FALSE;
	
	return TRUE;
}

static uint8 SendSi475xAmFreq(uint16 Freq)
{
	CopyComToSi475xOrder(SI475x_CMD_SET_AM_FREQ);
	
	Si475xOrder->buffer[Si475xOrder->length ++] = _u16to8h_(Freq);
	Si475xOrder->buffer[Si475xOrder->length ++] = _u16to8l_(Freq);
	
	if(!SendSi475xData())return FALSE;
	
	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 SetSi475xSeekSignal(uint8 Snr,uint8 Rssi)
{
	si475x_order_e OrderSnr;
	si475x_order_e OrderRssi;
	
	switch(si475xCtrl->TunerBand)
	{
		case SI475x_TUNER_BAND_REC_FM:
			OrderSnr	= SI475x_CMD_FM_SEEK_TUNE_SNR_SET;
			OrderRssi	= SI475x_CMD_FM_SEEK_TUNE_RSSI_SET;
			break;
		case SI475x_TUNER_BAND_REC_AM:
			OrderSnr	= SI475x_CMD_AM_SEEK_TUNE_SNR_SET;
			OrderRssi	= SI475x_CMD_AM_SEEK_TUNE_RSSI_SET;
			break;
		default:
			return FALSE;
	}
	
	CopyComToSi475xOrder(OrderSnr);
	Si475xOrder->buffer[Si475xOrder->length ++] = Snr;
	if(!SendSi475xData())return FALSE;
	
	CopyComToSi475xOrder(OrderRssi);
	Si475xOrder->buffer[Si475xOrder->length ++] = Rssi;
	if(!SendSi475xData())return FALSE;
	
	return TRUE;	
}

static uint8 SetSi475xSeekFreq(uint16 MinFreq,uint16 MaxFreq,uint8 Step)
{
	si475x_order_e OrderMin;
	si475x_order_e OrderMax;
	si475x_order_e OrderStep;
	
	MinFreq -= (MinFreq % Step);
	MaxFreq -= (MaxFreq % Step);
	if(MinFreq > MaxFreq)return FALSE;
	
	switch(si475xCtrl->TunerBand)
	{
		case SI475x_TUNER_BAND_REC_FM:
			OrderMin  = SI475x_CMD_FM_SEEK_BAND_BOTTOM;
			OrderMax  = SI475x_CMD_FM_SEEK_BAND_TOP;
			OrderStep = SI475x_CMD_FM_SEEK_BAND_STEP;
			break;
		case SI475x_TUNER_BAND_REC_AM:
			OrderMin  = SI475x_CMD_AM_SEEK_BAND_BOTTOM;
			OrderMax  = SI475x_CMD_AM_SEEK_BAND_TOP;
			OrderStep = SI475x_CMD_AM_SEEK_BAND_STEP;
			break;
		default:
			return FALSE;
	}
	
	CopyComToSi475xOrder(OrderMin);
	Si475xOrder->buffer[Si475xOrder->length ++] = _u16to8h_(MinFreq);
	Si475xOrder->buffer[Si475xOrder->length ++] = _u16to8l_(MinFreq);
	if(!SendSi475xData())return FALSE;
	
	CopyComToSi475xOrder(OrderMax);
	Si475xOrder->buffer[Si475xOrder->length ++] = _u16to8h_(MaxFreq);
	Si475xOrder->buffer[Si475xOrder->length ++] = _u16to8l_(MaxFreq);
	if(!SendSi475xData())return FALSE;
	
	CopyComToSi475xOrder(OrderStep);
	Si475xOrder->buffer[Si475xOrder->length ++] = Step;
	if(!SendSi475xData())return FALSE;
	
	return TRUE;
}

static uint8 SetSi475xSeekStart(uint8 Mode)
{
	si475x_order_e Order;
	uint8 Arg;
	
	switch(si475xCtrl->TunerBand)
	{
		case SI475x_TUNER_BAND_REC_FM:
			Order = SI475x_CMD_FM_SEEK_START;
			break;
		case SI475x_TUNER_BAND_REC_AM:
			Order = SI475x_CMD_AM_SEEK_START;
			break;
		default:return FALSE;
	}

	switch(Mode)
	{
		case SI475x_TUNER_SEEK_UP_WRAP:
			Arg = ((1<<3)|(1<<2));
			break;
		case SI475x_TUNER_SEEK_UP_HALT:
			Arg = ((1<<3)|(0<<2));
			break;
		case SI475x_TUNER_SEEK_DOWN_WRAP:
			Arg = ((0<<3)|(1<<2));
			break;
		case SI475x_TUNER_SEEK_DOWN_HALT:
			Arg = ((0<<3)|(0<<2));
			break;
	}
	
	CopyComToSi475xOrder(Order);
	
	Si475xOrder->buffer[Si475xOrder->length ++] = Arg;
	if(!SendSi475xData())return FALSE;
	
	return TRUE;
}

static uint8 SetSi475xSeekStop(void)
{
	si475x_order_e Order;
	
	switch(si475xCtrl->TunerBand)
	{
		case SI475x_TUNER_BAND_REC_FM:
			Order = SI475x_CMD_FM_SEEK_STOP;
			break;
		case SI475x_TUNER_BAND_REC_AM:
			Order = SI475x_CMD_AM_SEEK_STOP;
			break;
		default:return FALSE;
	}
	
	CopyComToSi475xOrder(Order);
	if(!SendSi475xData())return FALSE;
	
	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 si475x_set_init(void)
{
	ew_puts("si475x_set_init\n");
	if(si475xCtrl->init_succeed)
	{
		si475xCtrl->init_succeed = FALSE;
		
		ew_puts("si475x_set_init111\n");
		if(!SendSi475xOrder(SI475x_CMD_AUDIO_MUTE, 0))return FALSE;
		
		OSTimeDly(1);
		
		ew_puts("si475x_set_init222\n");
		if(!SendSi475xOrder(SI475x_CMD_POWER_DOWN, 0))return FALSE;
		
		OSTimeDly(5);
	}
	
	ew_puts("si475x_set_init333\n");
	switch(si475xCtrl->TunerBand)
	{
		case SI475x_TUNER_BAND_REC_FM:
			if(!SendSi475xOrder(SI475x_CMD_POWER_UP_REC_FM_STEP1, 0))return FALSE;
			break;
		case SI475x_TUNER_BAND_REC_AM:
			if(!SendSi475xOrder(SI475x_CMD_POWER_UP_REC_AM_STEP1, 0))return FALSE;
			break;
		default:
			return FALSE;
	}
	
	OSTimeDly(20);

	ew_puts("si475x_set_init444\n");
	if(!SendSi475xOrder(SI475x_CMD_POWER_UP_STEP2, 0))return FALSE;
	
#if defined(_SPECIAL_475x_INIT_)
	OSTimeDly(10);
#else
	OSTimeDly(2);
#endif
	
	ew_puts("si475x_set_init555\n");
	if(!SendSi475xOrder(SI475x_CMD_POWER_UP_RX_BOOT_LOADER, 0))return FALSE;
	
#if defined(_SPECIAL_475x_INIT_)
	OSTimeDly(10);
#endif

	ew_puts("si475x_set_init666\n");
	switch(si475xCtrl->TunerBand)
	{
		case SI475x_TUNER_BAND_REC_FM:
			ew_puts("si475x_set_init777\n");
			if(!SetSi475xFMPatch())return FALSE;
			ew_puts("si475x_set_init888\n");
			if(!SendSi475xOrder(SI475x_CMD_POWER_UP_REC_FM_STEP3, 0))return FALSE;
			OSTimeDly(12);
			
			//	配置去加重参数		
			ew_puts("si475x_set_init999\n");
			if(!SendSi475xOrder(SI475x_CMD_SET_FM_DEEMPHASIS_50US, 0))return FALSE; 
			ew_puts("si475x_set_init000\n");
						
			//	配置SOFT MUTE
			if(!SendSi475xOrder(SI475x_CMD_FM_SOFT_MUTE_ATTENUATION, 0))return FALSE;
			ew_puts("si475x_set_init0011\n");
			if(!SendSi475xOrder(SI475x_CMD_FM_SOFT_MUTE_TRIGGER_THRESHOLD, 0))return FALSE;
			ew_puts("si475x_set_init0022\n");
			if(!SendSi475xOrder(SI475x_CMD_FM_SOFT_MUTE_END_THRESHOLD, 0))return FALSE;
			
			ew_puts("si475x_set_init0033\n");
			if(!SendSi475xOrder(SI475x_CMD_FM_BLEND_RSSI_STEREO_SEP, 0))return FALSE; 
			ew_puts("si475x_set_init0044\n");
			if(!SendSi475xOrder(SI475x_CMD_FM_BLEND_MULTIPATH_STEREO_SEP, 0))return FALSE;	
			
			ew_puts("si475x_set_init0055\n");
			if(!SetSi475xSeekSignal(SI475x_TUNER_SNR_FM, SI475x_TUNER_RSSI_FM_DEFAULT))return FALSE;
			
			ew_puts("si475x_set_init0066\n");
			if(!SendSi475xOrder(SI475x_CMD_FM_R8000, 0))return FALSE;
			ew_puts("si475x_set_init0077\n");
			if(!SendSi475xOrder(SI475x_CMD_FM_R8001, 0))return FALSE;
			
			ew_puts("si475x_set_init0088\n");
			if(!SetSi475xFMProperty())return FALSE;
			ew_puts("si475x_set_init0099\n");
			if(!SendSi475xOrder(SI475x_CMD_ANA_AUDIO_PIN_CFG, 0))return FALSE;
			
			ew_puts("si475x_set_init1100\n");
			//	RDS配制
		#if defined(_ENABLE_RADIO_RDS_)
			if(!SendSi475xOrder(SI475x_CMD_FM_RDS_FIFO_COUNT, 0))return FALSE;
			if(!SendSi475xOrder(SI475x_CMD_FM_RDS_INT_SOURCE, 0))return FALSE;
			if(!SendSi475xOrder(SI475x_CMD_FM_RDS_CONFIG, 0))return FALSE;
			if(!SendSi475xOrder(SI475x_CMD_FM_RDS_STATUS, 0))return FALSE;
		#endif
			break;
		case SI475x_TUNER_BAND_REC_AM:	
			if(!SetSi475xAMPatch())return FALSE;
			if(!SendSi475xOrder(SI475x_CMD_POWER_UP_REC_AM_STEP3, 0))return FALSE;
			OSTimeDly(12);
			
			//	配置SOFT MUTE
			if(!SendSi475xOrder(SI475x_CMD_AM_SOFT_MUTE_ATTENUATION, 0))return FALSE;
			if(!SendSi475xOrder(SI475x_CMD_AM_SOFT_MUTE_TRIGGER_THRESHOLD, 0))return FALSE;
			if(!SendSi475xOrder(SI475x_CMD_AM_SOFT_MUTE_END_THRESHOLD, 0))return FALSE;
			
			if(!SetSi475xSeekSignal(SI475x_TUNER_SNR_AM, SI475x_TUNER_RSSI_AM_DEFAULT))return FALSE;
			
			if(!SetSi475xAMProperty())return FALSE;	
			if(!SendSi475xOrder(SI475x_CMD_ANA_AUDIO_PIN_CFG, 0))return FALSE;
			if(!SendSi475xOrder(SI475x_CMD_AM_DEMOD_MODE, 0))return FALSE;
			break;
		default:
			return FALSE;
	}

	return TRUE;
}

static uint8 si475x_set_mute(uint8 mute)
{
	if(mute)
	{
		if(!SendSi475xOrder(SI475x_CMD_AUDIO_MUTE, 0))return FALSE;
	}
	else
	{
		if(!SendSi475xOrder(SI475x_CMD_AUDIO_UNMUTE, 0))return FALSE;
	}
	
	return TRUE;
}

static uint8 si475x_set_stereo(uint8 stereo)
{
	if(stereo)
	{
		if(!SendSi475xOrder(SI475x_CMD_FM_BLEND_RSSI_STEREO, 0))return FALSE;
		if(!SendSi475xOrder(SI475x_CMD_FM_BLEND_MULTIPATH_STEREO, 0))return FALSE;
	}
	else
	{
		if(!SendSi475xOrder(SI475x_CMD_FM_BLEND_RSSI_MONO, 0))return FALSE;
		if(!SendSi475xOrder(SI475x_CMD_FM_BLEND_MULTIPATH_MONO, 0))return FALSE;
	}
	return TRUE;
}

static uint8 si475x_reset_autoseek(void)
{
	if(si475xCtrl->TunerAutoSeek)
	{	
		uint8 stc;
		
		if(!GetSi475xStc(&stc))return FALSE;
		
		if(!stc)
		{
			if(!SetSi475xSeekStop())return FALSE;
			
			OSTimeDly(5);
			
			if(!CheckSi475xStc())return FALSE;
		}
		
		si475xCtrl->TunerAutoSeek = FALSE;
	}
	
	return TRUE;
}

#if defined(_ENABLE_RADIO_RDS_)

static void si475x_rds_reset(void)
{
	if(si475xCtrl->tuner_rds.enable)
	{
		rds_dec_reset();
		si475xCtrl->tuner_rds.discard = 1;
		si475xCtrl->tuner_rds.quality = 0;
	}
}

static void si475x_rds_decode(void)
{

	if(si475xCtrl->tuner_rds.quality >= si475xCtrl->tuner_rds.factor)
	{
		si475xCtrl->tuner_rds.quality -= si475xCtrl->tuner_rds.factor;
	}
	rds_dec_timer();
	
	if(!SendSi475xOrder(SI475x_CMD_GET_INT_STATUS, 0))return;
	if(!(Si475xOrder->buffer[0] & _B0_))return;
	if(!(Si475xOrder->buffer[0] & _B2_))return;
	
	if(si475xCtrl->tuner_rds.discard)
	{
		//if(!SendSi475xOrder(SI475x_CMD_FM_RDS_RESET, 0))return;
		si475xCtrl->tuner_rds.discard--;
	}
	else
	{
		uint8 max_loop = 10;
		do{
			if(!SendSi475xOrder(SI475x_CMD_FM_RDS_STATUS, 17))return;
			if((Si475xFmRdsStatus.Resp7.BleA == 0)
			 &&(Si475xFmRdsStatus.Resp7.BleB == 0)
			 &&(Si475xFmRdsStatus.Resp7.BleC == 0)
			 &&(Si475xFmRdsStatus.Resp7.BleD == 0))
			{
				rds_input_t input;
				
				input.block_ah = Si475xFmRdsStatus.BlockA_h;
				input.block_al = Si475xFmRdsStatus.BlockA_l;
				input.block_bh = Si475xFmRdsStatus.BlockB_h;
				input.block_bl = Si475xFmRdsStatus.BlockB_l;
				input.block_ch = Si475xFmRdsStatus.BlockC_h;
				input.block_cl = Si475xFmRdsStatus.BlockC_l;
				input.block_dh = Si475xFmRdsStatus.BlockD_h;
				input.block_dl = Si475xFmRdsStatus.BlockD_l;
				
			#if 1
				ew_printf("Blo:0x%02x", Si475xFmRdsStatus.BlockA_h);
				ew_printf("%02x ", Si475xFmRdsStatus.BlockA_l);
				ew_printf("0x%02x", Si475xFmRdsStatus.BlockB_h);
				ew_printf("%02x ", Si475xFmRdsStatus.BlockB_l);
				ew_printf("0x%02x", Si475xFmRdsStatus.BlockC_h);
				ew_printf("%02x ", Si475xFmRdsStatus.BlockC_l);
				ew_printf("0x%02x", Si475xFmRdsStatus.BlockD_h);
				ew_printf("%02x \n", Si475xFmRdsStatus.BlockD_l);
			#endif
				si475xCtrl->tuner_rds.quality = 0xFF;
				
				rds_stream_dec(&input);
			}
			else 
			{
			
			ew_puts("---err---\n");
				if(Si475xFmRdsStatus.Resp7.BleA == 0)
				{
					//ew_puts("si475x_rds_only_block_a\n");
					si475xCtrl->tuner_rds.quality = 0xFF;
					rds_block_a_dec(Si475xFmRdsStatus.BlockA_h, Si475xFmRdsStatus.BlockA_l);
				}
				
				if(Si475xFmRdsStatus.Resp7.BleB == 0)
				{
					//ew_puts("si475x_rds_only_block_b\n");
					si475xCtrl->tuner_rds.quality = 0xFF;
					rds_block_b_dec(Si475xFmRdsStatus.BlockB_h, Si475xFmRdsStatus.BlockB_l);
				}
				
				if(Si475xFmRdsStatus.Resp7.BleC == 0)
				{
					//ew_puts("si475x_rds_only_block_c\n");
					si475xCtrl->tuner_rds.quality = 0xFF;
					rds_block_c_dec(Si475xFmRdsStatus.BlockC_h, Si475xFmRdsStatus.BlockC_l);
				}
				
				if(Si475xFmRdsStatus.Resp7.BleD == 0)
				{
					//ew_puts("si475x_rds_only_block_d\n");
					si475xCtrl->tuner_rds.quality = 0xFF;
					rds_block_d_dec(Si475xFmRdsStatus.BlockD_h, Si475xFmRdsStatus.BlockD_l);
				}
			}
			max_loop--;
		}
		while((Si475xFmRdsStatus.RdsFifoUsed > 1) && max_loop);
	}
}

#endif

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_si475x_detect(void);
static uint8 tuner_si475x_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_si475x_exit(void);
static uint8 tuner_si475x_freq(uint16);
static uint8 tuner_si475x_status(engine_status_t *);
static uint8 tuner_si475x_search(uint16, engine_status_t *);
static uint8 tuner_si475x_hard(engine_hard_t *);
static uint8 tuner_si475x_setting(fm_setting_e, void *);
static uint8 tuner_si475x_info(fm_info_e, void *);


static uint8 tuner_si475x_detect(void)
{
	uint8 result;
	
	i2c_data_init(SI475x_I2C_ADDR, SI475x_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	i2c_data_exit();
	ew_printf("tuner_si475x_detect:%d\n", result);
	return result;
}

static uint8 tuner_si475x_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	ew_printf("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);

	if(Si475xOrder == NULL)
	{
		Si475xOrder = (si475x_order_t *)malloc_fun(Si475xOrder, sizeof(si475x_order_t), 0);
		if(Si475xOrder == NULL)return FALSE;
	}
	
	if(si475xCtrl == NULL)
	{
		si475xCtrl = (si475x_ctrl_t *)malloc_fun(si475xCtrl, sizeof(si475x_ctrl_t), 0);
		if(si475xCtrl == NULL)
		{
			free_fun((void **)&Si475xOrder);
			return FALSE;
		}
	}
	ew_puts("si475x_memory_ok\n");

#if defined(_YH6000A_475x_INIT_)
	si475xCtrl->tuner_band = 0;
#endif

	switch(band)
	{
		case RADIO_BAND_FM:
			ew_puts("RADIO_BAND_FM\n");
			if(min_freq > max_freq)goto si475x_init_error;
			ew_puts("RADIO_BAND_FM111\n");
			if(min_freq < 6400)goto si475x_init_error;
			ew_puts("RADIO_BAND_FM222\n");
			if(max_freq > 10800)goto si475x_init_error;
			ew_puts("RADIO_BAND_FM333\n");
			si475xCtrl->TunerBand = SI475x_TUNER_BAND_REC_FM;
			break;
			
		case RADIO_BAND_AM:
			ew_puts("RADIO_BAND_AM\n");
			if(min_freq > max_freq)goto si475x_init_error;
			if(min_freq < 520)goto si475x_init_error;
			if(max_freq > 1710)goto si475x_init_error;
			si475xCtrl->TunerBand = SI475x_TUNER_BAND_REC_AM;
			break;
				
		default:
			ew_puts("RADIO_BAND_FM444\n");
			goto si475x_init_error;
			break;
	}
	
	//ew_printf("tuner_band:%d\n",si475xCtrl->tuner_band);
	//ew_printf("band:%d\n",band);
	if(si475xCtrl->tuner_band == band)
	{
		if(si475xCtrl->init_succeed)
		{
			if(si475xCtrl->now_freq == freq)
			{
				si475xCtrl->min_freq = min_freq;
				si475xCtrl->max_freq = max_freq;
				ew_puts("RADIO_BAND_FM555\n");
				if(!tuner_si475x_setting(TUR_SET_MUTE, OFF))goto si475x_init_error;
				ew_puts("RADIO_BAND_FM666\n");
				return TRUE;
			}
			else
			{
				ew_puts("RADIO_BAND_FM777\n");
				goto si475x_init_freq;
			}
		}
		else 
		{
			ew_puts("RADIO_BAND_FM888\n");
			goto si475x_init_error;
		}
	}

#if defined(_SPECIAL_475x_INIT_)
	si475x_set_init();
	OSTimeDly(5);
	if(!si475x_set_init())goto si475x_init_error;
	OSTimeDly(1);
#else
	if(!si475x_set_init())goto si475x_init_error;
    OSTimeDly(5);
#endif

si475x_init_freq:
	si475xCtrl->tuner_band = band;
	si475xCtrl->min_freq = min_freq;
	si475xCtrl->max_freq = max_freq;
	si475xCtrl->init_succeed = TRUE;
	si475xCtrl->TunerAutoSeek = FALSE;
	
	engine_status_t status;
	if(!tuner_si475x_search(freq, &status))goto si475x_init_error;
	OSTimeDly(10);
#if defined(_SPECIAL_475x_INIT_)
	if(!tuner_si475x_search(freq, &status))goto si475x_init_error;
	OSTimeDly(8);
#endif
	if(!tuner_si475x_search(freq, &status))goto si475x_init_error;
	if(!tuner_si475x_setting(TUR_SET_MUTE, OFF))goto si475x_init_error;
	ew_puts("SI475x Init Succeed!!!!!\n");
#if defined(_RADIO_RESET_INIT_)
	global.radio_init = FALSE;
#endif
#if defined(_RADIO_RESET_MODE_)
	global.radio_init_error = FALSE;
#endif
	return TRUE;
	
si475x_init_error:
	free_fun((void **)&Si475xOrder);
	free_fun((void **)&si475xCtrl);
#if defined(_RADIO_RESET_INIT_)
	global.radio_init = TRUE;
#endif
#if defined(_RADIO_RESET_MODE_)
	global.radio_init_time1++;
	if(global.radio_init_time1 >= 3)
	{
		global.radio_init_error = FALSE;
		global.radio_init_time1 = 0;
	}
	else
	{
		global.radio_init_error = TRUE;
	}
#endif
	ew_puts("SI475x Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_si475x_exit(void)
{
	if(Si475xOrder && si475xCtrl)
	{
		si475x_reset_autoseek();
		SendSi475xOrder(SI475x_CMD_POWER_DOWN, 0);
		free_fun((void **)&Si475xOrder);
		free_fun((void **)&si475xCtrl);
	}
	return TRUE;
}

static uint8 tuner_si475x_freq(uint16 freq)
{
	ew_printf("tuner_si475x_freq:%d\n",freq);
	if((Si475xOrder == NULL)||(si475xCtrl == NULL))return FALSE;
	if(!si475xCtrl->init_succeed)return FALSE;
	if(freq < si475xCtrl->min_freq)return FALSE;
	if(freq > si475xCtrl->max_freq)return FALSE;		
	
	switch(si475xCtrl->TunerBand)
	{
		case SI475x_TUNER_BAND_REC_FM:
			if(!SendSi475xFmFreq(freq))return FALSE;
			break;
			
		case SI475x_TUNER_BAND_REC_AM:
			if(!SendSi475xAmFreq(freq))return FALSE;
			break;
			
		default:return FALSE;
	}
#if defined(_ENABLE_RADIO_RDS_)
	si475x_rds_reset();
#endif
	si475xCtrl->now_freq = freq;
	return TRUE;
}

static uint8 tuner_si475x_status(engine_status_t *status)
{
	//ew_puts("tuner_si475x_status\n");
#if defined(_RADIO_RESET_INIT_)&&0
	if(global.radio_init)
	{
		//global.radio_init_time++;
			
		RADIO_RESET_ON();
				
		OSTimeDly(20);
		
		RADIO_RESET_OFF();
		
		OSTimeDly(300);
		
		si475xCtrl->now_freq = 0x00;
		tuner_si475x_init(RADIO_BAND_FM,7610,10790,status->freq);
		
		//if(global.radio_init_time == 5)//复位五次
		{
			global.radio_init = FALSE;
			//global.radio_init_time = 0;
		}
	}
#else
	if((Si475xOrder == NULL)||(si475xCtrl == NULL))return FALSE;
	if(!si475xCtrl->init_succeed)return FALSE;
#endif

	switch(si475xCtrl->TunerBand)
	{
		case SI475x_TUNER_BAND_REC_FM:
			if(si475xCtrl->TunerAutoSeek)
			{
				if(!SendSi475xOrder(SI475x_CMD_FM_RSQ_STATUS, sizeof(Si475xFmRsqStatus)))return FALSE;
				
				//当前调谐状态
				if(Si475xFmRsqStatus.Status.StcInt)
				{
					status->stc = TRUE;
				}
				else
				{
					status->stc = FALSE;
				}
				
				//信号强度
				status->signal = Si475xFmRsqStatus.Rssi;
				
				//是否有无电台
				status->valid = FALSE;
				if(!Si475xFmRsqStatus.Resp2.Afc)
				{
					if(Si475xFmRsqStatus.Resp2.Valid)
					{
						status->valid = TRUE;
					}
				}
				
				//单声道
				status->stereo = FALSE;
				
				//当前频率
				si475xCtrl->now_freq = _u8tou16_(Si475xFmRsqStatus.Freq_h, Si475xFmRsqStatus.Freq_l);
			}
			else
			{				
				if(!SendSi475xOrder(SI475x_CMD_FM_RSQ_STATUS, sizeof(Si475xFmRsqStatus)))return FALSE;

				//当前调谐状态
				if(Si475xFmRsqStatus.Status.StcInt)
				{
					status->stc = TRUE;
				}
				else
				{
					status->stc = FALSE;
				}
				
				//信号强度
				status->signal = Si475xFmRsqStatus.Rssi;
				
			#if defined(_SPECIAL_AUTOSEEK_MEMORY_)
				setup.rssi = status->signal;
				//ew_printf("setup.rssi:%d\n",setup.rssi);
			#endif
			
			
				//是否有无电台
				status->valid = FALSE;
			
				if(!Si475xFmRsqStatus.Resp2.Afc)
				{
					if(Si475xFmRsqStatus.Resp2.Valid)
					{
						status->valid = TRUE;
					}
				}
				
				//立体声
				if(!SendSi475xOrder(SI475x_CMD_FM_ACF_STATUS, sizeof(Si475xFmAcfStatus)))return FALSE;
				if(Si475xFmAcfStatus.Resp7.Pilot && Si475xFmAcfStatus.Resp7.Stblend)
				{
					status->stereo = TRUE;
				}
				else 
				{
					status->stereo = FALSE;
				}
			}
			
			//当前频率
			status->freq = si475xCtrl->now_freq;
			
			//当前波段
			status->band = RADIO_BAND_FM;
			break;
			
		case SI475x_TUNER_BAND_REC_AM:
			if(!SendSi475xOrder(SI475x_CMD_AM_RSQ_STATUS, sizeof(Si475xAmRsqStatus)))return FALSE;

			//当前调谐状态
			if(Si475xAmRsqStatus.Status.StcInt)
			{
				status->stc = TRUE;
			}
			else
			{
				status->stc = FALSE;
			}
			
			//信号强度
			status->signal = Si475xAmRsqStatus.Rssi;
			
	    #if defined(_SPECIAL_AUTOSEEK_MEMORY_)
	    	setup.rssi = status->signal;
	    #endif
			//是否有无电台
			status->valid = FALSE;
			if(!Si475xAmRsqStatus.Resp2.Afc)
			{
				if(Si475xAmRsqStatus.Resp2.Valid)
				{
					status->valid = TRUE;
				}
			}
			
			//当前频率
			si475xCtrl->now_freq = _u8tou16_(Si475xAmRsqStatus.Freq_h, Si475xAmRsqStatus.Freq_l);

			//立体声
			status->stereo = FALSE;
			
			//当前频率
			status->freq = si475xCtrl->now_freq;
			
			//当前波段
			status->band = RADIO_BAND_AM;
			break;
		default:return FALSE;
	}	
	
	OSTimeDly(5);
	
	//ew_printf("status->freq:%d\n",status->freq);
	//ew_printf("status->signal:%d\n",status->signal);
	
	return TRUE;
}

static uint8 tuner_si475x_search(uint16 freq, engine_status_t *status)
{
	ew_puts("tuner_si475x_search\n");
	if((Si475xOrder == NULL)||(si475xCtrl == NULL))return FALSE;
	if(!si475xCtrl->init_succeed)return FALSE;
	
	if(!tuner_si475x_freq(freq))return FALSE;
	
	OSTimeDly(5);
	
	if(!CheckSi475xStc())return FALSE;
	
	if(status->band == RADIO_BAND_AM)
	{//修改AM 在STC 置位后延时读RSSI
		OSTimeDly(2);
	}
	
	if(!tuner_si475x_status(status))return FALSE;
	
	return TRUE;
}

static uint8 tuner_si475x_hard(engine_hard_t *hard)
{
	ew_puts("tuner_si475x_hard\n");
	if((Si475xOrder == NULL)||(si475xCtrl == NULL))return FALSE;
	if(!si475xCtrl->init_succeed)return FALSE;
		
	if(hard->enable)
	{
		ew_puts("tuner_si475x_hard11111\n");
		engine_hard_start_t *start = &hard->ctrl.start;
		engine_status_t status;
		
		if(start->min_freq < si475xCtrl->min_freq)return FALSE;
		if(start->max_freq > si475xCtrl->max_freq)return FALSE;	
		if(start->start_freq < start->min_freq)return FALSE;
		if(start->start_freq > start->max_freq)return FALSE;		
		
		if(!si475x_reset_autoseek())return FALSE;
		
		if(!tuner_si475x_search(start->start_freq, &status))return FALSE;
		
		if(!SetSi475xSeekFreq(start->min_freq, start->max_freq, start->step))return FALSE;
		
		if(start->dir)
		{
			if(!SetSi475xSeekStart(TUNER_SEEK_UP_WRAP))return FALSE;
		}
		else
		{
			if(!SetSi475xSeekStart(TUNER_SEEK_DOWN_WRAP))return FALSE;
		}
	#if defined(_ENABLE_RADIO_RDS_)
		si475x_rds_reset();
	#endif
		si475xCtrl->TunerAutoSeek = TRUE;
	}
	else
	{
		ew_puts("tuner_si475x_hard222222\n");
		engine_hard_stop_t *stop = &hard->ctrl.stop;
		
		if(stop->stop_freq == 0xFFFF)
		{
			if(!si475x_reset_autoseek())return FALSE;
			
			si475xCtrl->TunerAutoSeek = TRUE;
			if(!tuner_si475x_status(stop->status))
			{
				si475xCtrl->TunerAutoSeek = FALSE;
				return FALSE;
			}
			si475xCtrl->TunerAutoSeek = FALSE;
			
			if(stop->status->freq < si475xCtrl->min_freq)
			{
				if(!tuner_si475x_search(si475xCtrl->min_freq, stop->status))return FALSE;
			}
			else if(stop->status->freq > si475xCtrl->max_freq)
			{
				if(!tuner_si475x_search(si475xCtrl->max_freq, stop->status))return FALSE;
			}
		}
		else
		{
			if(stop->stop_freq < si475xCtrl->min_freq)return FALSE;
			if(stop->stop_freq > si475xCtrl->max_freq)return FALSE;	
			
			if(!si475x_reset_autoseek())return FALSE;
			if(!tuner_si475x_search(stop->stop_freq, stop->status))return FALSE;
		}
	}
	
	return TRUE;
}

static uint8 tuner_si475x_setting(fm_setting_e type, void *set)
{
	//ew_puts("tuner_si475x_setting\n");
	if((Si475xOrder == NULL)||(si475xCtrl == NULL))return FALSE;
	if(!si475xCtrl->init_succeed)return FALSE;
	
	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!si475x_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(si475xCtrl->TunerBand == SI475x_TUNER_BAND_REC_FM)
				{
					if(!si475x_set_stereo(stereo))return FALSE;
				}
				else return FALSE;
			}
			break;
			
	#if defined(_ENABLE_RADIO_RDS_)
		case TUR_SET_RDS:
			{
				typedef struct{
					uint8 enable;
					uint16 timer;
					uint8 result;
				}tuner_rds_ctrl_t;
				tuner_rds_ctrl_t *rds_ctrl = ((tuner_rds_ctrl_t *)set);
				
				ew_printf("rds enable:%d, timer:%d\n", rds_ctrl->enable, rds_ctrl->timer);
				
				if(rds_ctrl->enable)
				{
					if(si475xCtrl->TunerBand == SI475x_TUNER_BAND_REC_FM)
					{
						if((si475xCtrl->min_freq >= 8750) && (si475xCtrl->max_freq <= 10800))
						{
							if(!si475xCtrl->tuner_rds.enable)
							{
								int check = FALSE;
								if(!tuner_si475x_info(TUR_INFO_RDS, &check))return FALSE;
								if(check)
								{
									si475xCtrl->tuner_rds.factor = 255/(_RDS_QUALITY_TIME_/rds_ctrl->timer);
									si475xCtrl->tuner_rds.enable = TRUE;
									si475x_rds_reset();
									rds_dec_det_time(rds_ctrl->timer);
									rds_ctrl->result = TRUE;
								}
							}
						}
						else return FALSE;
					}
					else return FALSE;
				}
				else
				{
					if(si475xCtrl->tuner_rds.enable)
					{
						si475xCtrl->tuner_rds.enable = FALSE;
						rds_ctrl->result = TRUE;
					}
					else return FALSE;
				}
			}
			break;

		case TUR_SET_RDS_RESET:
			{
				if(si475xCtrl->tuner_rds.enable)
				{
					si475x_rds_reset();
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_DECODE:
			{
				if(si475xCtrl->tuner_rds.enable)
				{
					si475x_rds_decode();
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_EON:
			{
				if(si475xCtrl->tuner_rds.enable)
				{
					uint8 disable = (uint8)set;
					rds_dec_eon(disable);
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_SAVE:
			if(set != NULL)
			{
				if(si475xCtrl->tuner_rds.enable)
				{
					radio_rds_station_t *write_station = (radio_rds_station_t *)set;
					rds_dec_save(write_station);
				}
				else return FALSE;
			}
			else return FALSE;
			break;
			
	#endif
			
		default:
			return FALSE;
	}
	
	return TRUE;
}

static uint8 tuner_si475x_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_SI475x;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI475x_TUNER_RSSI_FM_DEFAULT;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI475x_TUNER_RSSI_LOC_FM;
			}
			break;

		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_si475x_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_si475x_deal_spot_tab;
			}
			break;
		
		case TUR_INFO_AM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI475x_TUNER_RSSI_AM_DEFAULT;
			}
			break;
			
		case TUR_INFO_AM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI475x_TUNER_RSSI_LOC_AM;
			}
			break;

		case TUR_INFO_AM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(am_si475x_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_AM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)am_si475x_deal_spot_tab;
			}
			break;
			
		case TUR_INFO_AUTOSEEK:
			if(info != NULL)
			{
		    	*(uint8 *)info = DISABLE; //DISABLE;//
			}
			break;
			
	#if defined(_DISABLE_RADIO_AM_)
    	case TUR_INFO_FM_ONLY:
    		if(info != NULL)
    		{
    			*(uint8 *)info = TRUE;
    		}
    		break;
	#endif
	
	#if defined(_ENABLE_RADIO_RDS_)
		case TUR_INFO_RDS:
			if(info != NULL)
			{
				*(uint8 *)info = TRUE;
			}
			break;
			
		case TUR_INFO_RDS_INFO:
			if(info != NULL)
			{
				if((Si475xOrder == NULL)||(si475xCtrl == NULL))return FALSE;
				if(!si475xCtrl->init_succeed)return FALSE;
				if(!si475xCtrl->tuner_rds.enable)return FALSE;
				
				radio_rds_info_t *rds_info = (radio_rds_info_t *)info;
				rds_dec_info(rds_info);
			}
			break;
			
		case TUR_INFO_RDS_APP:
			if(info != NULL)
			{
				if((Si475xOrder == NULL)||(si475xCtrl == NULL))return FALSE;
				if(!si475xCtrl->init_succeed)return FALSE;
				if(!si475xCtrl->tuner_rds.enable)return FALSE;
				
				radio_rds_app_t *rds_app = (radio_rds_app_t *)info;
				engine_status_t status;
				if(!tuner_si475x_status(&status))return FALSE;
				rds_app->freq	= status.freq;
				rds_app->level	= status.signal;
				rds_app->stc	= status.stc;
				rds_app->found	= status.valid;
				rds_app->signal = si475xCtrl->tuner_rds.quality;
				rds_dec_app(rds_app);
			}
			break;
			
		case TUR_INFO_RDS_LOAD:
			if(info != NULL)
			{
				if((Si475xOrder == NULL)||(si475xCtrl == NULL))return FALSE;
				if(!si475xCtrl->init_succeed)return FALSE;
				if(!si475xCtrl->tuner_rds.enable)return FALSE;
				
				radio_rds_station_t *read_station = (radio_rds_station_t *)info;
				rds_dec_load(read_station);
			}
			break;
	#endif
			
		default:
			return FALSE;
	}
	
	return TRUE;
}


const tuner_drv_fun_t tuner_si475x_drv_fun =
{
	tuner_si475x_detect,
	tuner_si475x_init,
	tuner_si475x_exit,
	tuner_si475x_freq,
	tuner_si475x_status,
	tuner_si475x_search,
	tuner_si475x_hard,
	tuner_si475x_setting,
	tuner_si475x_info,
};

#endif

