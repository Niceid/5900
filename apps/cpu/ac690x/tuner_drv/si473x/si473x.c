#include "si473x/si473x.h"
#include "rds/rds_dec.h"
#include "fm_radio.h"

#if defined(_ENABLE_RADIO_MUTE_)
extern u32 compare_task_name(const char *task_name);
#endif

#if defined(_TUNER_DRIVER_SI473x_)

static si473x_order_t *Si473xOrder = NULL;
static si473x_ctrl_t *si473xCtrl = NULL;

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
//	公共命令
static const uint8 CMD_SI473x_POWER_UP_REC_FM[] 				= {3,0x01,0x10,0x05};
static const uint8 CMD_SI473x_POWER_UP_REC_AM[] 				= {3,0x01,0x11,0x05};
static const uint8 CMD_SI473x_POWER_UP_REC_WB[] 				= {3,0x01,0x13,0x05};
static const uint8 CMD_SI473x_POWER_UP_TRN_FM[] 				= {3,0x01,0x12,0x05};
static const uint8 CMD_SI473x_GET_REV[] 						= {1,0x10};
static const uint8 CMD_SI473x_POWER_DOWN[] 						= {1,0x11};
static const uint8 CMD_SI473x_GET_INT_STATUS[] 					= {1,0x14};

static const uint8 CMD_SI473x_AUDIO_VOLUME[] 					= {5,0x12,0x00,0x40,0x00,0x00};
static const uint8 CMD_SI473x_AUDIO_MUTE[] 						= {6,0x12,0x00,0x40,0x01,0x00,0x03};
static const uint8 CMD_SI473x_AUDIO_UNMUTE[] 					= {6,0x12,0x00,0x40,0x01,0x00,0x00};

static const uint8 CMD_SI473x_GPIO_DIR[]						= {1,0x80};
static const uint8 CMD_SI473x_GPIO_DATA[]						= {1,0x81};
/*-------------------------------------------------------------------------------------------------------------------------*/
//	调频命令
static const uint8 CMD_SI473x_SET_FM_FREQ[]						= {2,0x20,0x00};
static const uint8 CMD_SI473x_FM_TUNE_STATUS[] 					= {2,0x22,0x00};
static const uint8 CMD_SI473x_FM_RSQ_STATUS[]					= {2,0x23,0x01};
static const uint8 CMD_SI473x_FM_AGC_STATUS[]					= {1,0x27};

#if defined(_FEIYIN_473x_C40_ADJUST_)
static const uint8 CMD_SI473x_FM_AGC_OVERRIDE[]					= {3,0x28,0x00,0x1A};
#else
static const uint8 CMD_SI473x_FM_AGC_OVERRIDE[]					= {3,0x28,0x01,0x00};
#endif

static const uint8 CMD_SI473x_FM_SEEK_TUNE_SNR_SET[]			= {5,0x12,0x00,0x14,0x03,0x00};
static const uint8 CMD_SI473x_FM_SEEK_TUNE_RSSI_SET[]			= {5,0x12,0x00,0x14,0x04,0x00};

static const uint8 CMD_SI473x_SET_FM_DEEMPHASIS_50US[] 			= {6,0x12,0x00,0x11,0x00,0x00,0x01};
static const uint8 CMD_SI473x_SET_FM_DEEMPHASIS_75US[] 			= {6,0x12,0x00,0x11,0x00,0x00,0x02};
static const uint8 CMD_SI473x_FM_BLEND_STEREO_ON[]				= {6,0x12,0x00,0x11,0x05,0x00,SI473x_TUNER_STEREO_VALUE};
static const uint8 CMD_SI473x_FM_BLEND_MONO_OFF[]				= {6,0x12,0x00,0x11,0x06,0x00,SI473x_TUNER_MONO_VALUE};
static const uint8 CMD_SI473x_FM_BLEND_STEREO_OFF[]				= {6,0x12,0x00,0x11,0x05,0x00,127};
static const uint8 CMD_SI473x_FM_BLEND_MONO_ON[]				= {6,0x12,0x00,0x11,0x06,0x00,127};	
static const uint8 CMD_SI473x_FM_TUNE_ERROR[]					= {6,0x12,0x00,0x11,0x08,0x00,SI473x_TUNER_FM_TUNE_ERROR};	

static const uint8 CMD_SI473x_FM_SOFT_MUTE_RATE[]				= {6,0x12,0x00,0x13,0x00,0x00,64};

#if defined(SI473x_FM_SOFT_MUTE_ATTENUATION)
static const uint8 CMD_SI473x_FM_SOFT_MUTE_ATTENUATION[]		= {6,0x12,0x00,0x13,0x02,0x00,SI473x_FM_SOFT_MUTE_ATTENUATION};
#else
static const uint8 CMD_SI473x_FM_SOFT_MUTE_ATTENUATION[]		= {6,0x12,0x00,0x13,0x02,0x00,8};
#endif

#if defined(SI473x_FM_SOFT_MUTE_SNR)
static const uint8 CMD_SI473x_FM_SOFT_MUTE_SNR[]				= {6,0x12,0x00,0x13,0x03,0x00,SI473x_FM_SOFT_MUTE_SNR};
#else
static const uint8 CMD_SI473x_FM_SOFT_MUTE_SNR[]				= {6,0x12,0x00,0x13,0x03,0x00,2};
#endif

#if defined(_SI473x_VERSION_3020_)
#if defined(SI473x_FM_SOFT_MUTE_ATTENUATION_B20)
static const uint8 CMD_SI473x_FM_SOFT_MUTE_ATTENUATION_B20[]	= {6,0x12,0x00,0x13,0x02,0x00,SI473x_FM_SOFT_MUTE_ATTENUATION_B20};
#else
static const uint8 CMD_SI473x_FM_SOFT_MUTE_ATTENUATION_B20[]	= {6,0x12,0x00,0x13,0x02,0x00,8};
#endif

#if defined(SI473x_FM_SOFT_MUTE_SNR_B20)
static const uint8 CMD_SI473x_FM_SOFT_MUTE_SNR_B20[]			= {6,0x12,0x00,0x13,0x03,0x00,SI473x_FM_SOFT_MUTE_SNR_B20};
#else
static const uint8 CMD_SI473x_FM_SOFT_MUTE_SNR_B20[]			= {6,0x12,0x00,0x13,0x03,0x00,2};
#endif
#endif

static const uint8 CMD_SI473x_GET_FM_BLEND_STEREO[]				= {4,0x13,0x00,0x11,0x05};
static const uint8 CMD_SI473x_GET_FM_BLEND_MONO[]				= {4,0x13,0x00,0x11,0x06};

#if defined(_ENABLE_RADIO_RDS_)
//  RDS命令
static const uint8 CMD_SI473x_FM_RDS_DETECT[]					= {2,0x24,0x03};
static const uint8 CMD_SI473x_FM_RDS_STATUS[]					= {2,0x24,0x01};
static const uint8 CMD_SI473x_FM_RDS_INT_SOURCE[]				= {6,0x12,0x00,0x15,0x00,0x00,0x01};
static const uint8 CMD_SI473x_FM_RDS_FIFO_COUNT[]				= {6,0x12,0x00,0x15,0x01,0x00,1};
static const uint8 CMD_SI473x_FM_RDS_CONFIG[]					= {6,0x12,0x00,0x15,0x02,0xFF,0x01};
static const uint8 CMD_SI473x_FM_RDS_RESET[]					= {2,0x24,0x03};
#endif

#if defined(_SI473x_VERSION_3060_)

static const uint8 CMD_SI473x_FM_BLEND_RSSI_STEREO_ON[]         = {6,0x12,0x00,0x18,0x00,0x00,SI473x_TUNER_RSSI_STEREO_VALUE};
static const uint8 CMD_SI473x_FM_BLEND_RSSI_MONO_OFF[]          = {6,0x12,0x00,0x18,0x01,0x00,SI473x_TUNER_RSSI_MONO_VALUE};
static const uint8 CMD_SI473x_FM_BLEND_RSSI_STEREO_OFF[]        = {6,0x12,0x00,0x18,0x00,0x00,127};
static const uint8 CMD_SI473x_FM_BLEND_RSSI_MONO_ON[]           = {6,0x12,0x00,0x18,0x01,0x00,127};
static const uint8 CMD_SI473x_FM_BLEND_RSSI_ATTACK_RATE[]       = {6,0x12,0x00,0x18,0x02,0x0F,0xA0};
static const uint8 CMD_SI473x_FM_BLEND_RSSI_RELEASE_RATE[]      = {6,0x12,0x00,0x18,0x03,0x01,0x90};

static const uint8 CMD_SI473x_FM_BLEND_SNR_STEREO_ON[]         	= {6,0x12,0x00,0x18,0x04,0x00,SI473x_TUNER_SNR_STEREO_VALUE};
static const uint8 CMD_SI473x_FM_BLEND_SNR_MONO_OFF[]          	= {6,0x12,0x00,0x18,0x05,0x00,SI473x_TUNER_SNR_MONO_VALUE};
static const uint8 CMD_SI473x_FM_BLEND_SNR_STEREO_OFF[]        	= {6,0x12,0x00,0x18,0x04,0x00,127};
static const uint8 CMD_SI473x_FM_BLEND_SNR_MONO_ON[]           	= {6,0x12,0x00,0x18,0x05,0x00,127};  
static const uint8 CMD_SI473x_FM_BLEND_SNR_ATTACK_RATE[]       	= {6,0x12,0x00,0x18,0x06,0x0F,0xA0};
static const uint8 CMD_SI473x_FM_BLEND_SNR_RELEASE_RATE[]      	= {6,0x12,0x00,0x18,0x07,0x01,0x90};

static const uint8 CMD_SI473x_FM_BLEND_MULTIPATH_STEREO_ON[]   	= {6,0x12,0x00,0x18,0x08,0x00,SI473x_TUNER_MULTIPATH_STEREO_VALUE};
static const uint8 CMD_SI473x_FM_BLEND_MULTIPATH_MONO_OFF[]    	= {6,0x12,0x00,0x18,0x09,0x00,SI473x_TUNER_MULTIPATH_MONO_VALUE};
static const uint8 CMD_SI473x_FM_BLEND_MULTIPATH_STEREO_OFF[]  	= {6,0x12,0x00,0x18,0x08,0x00,0};
static const uint8 CMD_SI473x_FM_BLEND_MULTIPATH_MONO_ON[]     	= {6,0x12,0x00,0x18,0x09,0x00,0};  
static const uint8 CMD_SI473x_FM_BLEND_MULTIPATH_ATTACK_RATE[] 	= {6,0x12,0x00,0x18,0x0A,0x0F,0xA0};
static const uint8 CMD_SI473x_FM_BLEND_MULTIPATH_RELEASE_RATE[]	= {6,0x12,0x00,0x18,0x0B,0x00,0x28};

static const uint8 CMD_SI473x_FM_BLEND_MULTIPATH_STEREO_DEFAULT[]			= {6,0x12,0x00,0x18,0x08,0x00,100};
static const uint8 CMD_SI473x_FM_BLEND_MULTIPATH_MONO_DEFAULT[]				= {6,0x12,0x00,0x18,0x09,0x00,100};

static const uint8 CMD_SI473x_FM_HICUT_HIGH_THRESHOLD_DEFAULT[]				= {6,0x12,0x00,0x1A,0x00,0x00,24};
static const uint8 CMD_SI473x_FM_HICUT_LOW_THRESHOLD_DEFAULT[]				= {6,0x12,0x00,0x1A,0x01,0x00,15};
//static const uint8 CMD_SI473x_FM_HICUT_ATTACK_RATE_DEFAULT[]					= {6,0x12,0x00,0x1A,0x02,0x4E,0x20};
//static const uint8 CMD_SI473x_FM_HICUT_RELEASE_RATE_DEFAULT[]					= {6,0x12,0x00,0x1A,0x03,0x00,0x14};  
static const uint8 CMD_SI473x_FM_HICUT_MULTIPATH_TRIGGER_THRESHOLD_DEFAULT[]= {6,0x12,0x00,0x1A,0x04,0x00,20};
static const uint8 CMD_SI473x_FM_HICUT_MULTIPATH_END_THRESHOLD_DEFAULT[]	= {6,0x12,0x00,0x1A,0x05,0x00,60};
static const uint8 CMD_SI473x_FM_HICUT_OFF_FREQUENCY_DEFAULT[]				= {6,0x12,0x00,0x1A,0x06,0x00,0};//0x03

#endif

static const uint8 CMD_SI473x_FM_SEEK_START[]					= {1,0x21};
static const uint8 CMD_SI473x_FM_SEEK_STOP[]					= {2,0x22,0x03};
static const uint8 CMD_SI473x_FM_SEEK_BAND_BOTTOM[]				= {4,0x12,0x00,0x14,0x00};
static const uint8 CMD_SI473x_FM_SEEK_BAND_TOP[]				= {4,0x12,0x00,0x14,0x01};
static const uint8 CMD_SI473x_FM_SEEK_BAND_STEP[]				= {5,0x12,0x00,0x14,0x02,0x00};

/*-------------------------------------------------------------------------------------------------------------------------*/
//	调幅命令
static const uint8 CMD_SI473x_SET_AM_FREQ[]						= {2,0x40,0x00};
static const uint8 CMD_SI473x_AM_TUNE_STATUS[] 					= {2,0x42,0x00};
static const uint8 CMD_SI473x_AM_RSQ_STATUS[]					= {2,0x43,0x01};

static const uint8 CMD_SI473x_SET_AM_DEEMPHASIS_OFF[] 			= {6,0x12,0x00,0x31,0x00,0x00,0x00};
static const uint8 CMD_SI473x_SET_AM_DEEMPHASIS_50US[] 			= {6,0x12,0x00,0x31,0x00,0x00,0x01};

static const uint8 CMD_SI473x_AM_SOFT_MUTE_RATE[]				= {6,0x12,0x00,0x33,0x00,0x00,0x40};
static const uint8 CMD_SI473x_AM_SOFT_MUTE_SLOPE[]				= {6,0x12,0x00,0x33,0x01,0x00,2};

#if defined(SI473x_AM_SOFT_MUTE_ATTENUATION)
static const uint8 CMD_SI473x_AM_SOFT_MUTE_ATTENUATION[]		= {6,0x12,0x00,0x33,0x02,0x00,SI473x_AM_SOFT_MUTE_ATTENUATION};
#else
static const uint8 CMD_SI473x_AM_SOFT_MUTE_ATTENUATION[]		= {6,0x12,0x00,0x33,0x02,0x00,20};
#endif

#if defined(SI473x_AM_SOFT_MUTE_SNR)
static const uint8 CMD_SI473x_AM_SOFT_MUTE_SNR[]				= {6,0x12,0x00,0x33,0x03,0x00,SI473x_AM_SOFT_MUTE_SNR};
#else
static const uint8 CMD_SI473x_AM_SOFT_MUTE_SNR[]				= {6,0x12,0x00,0x33,0x03,0x00,10};
#endif

static const uint8 CMD_SI473x_AM_SEEK_TUNE_SNR_SET[]			= {5,0x12,0x00,0x34,0x03,0x00};
static const uint8 CMD_SI473x_AM_SEEK_TUNE_RSSI_SET[]			= {5,0x12,0x00,0x34,0x04,0x00};

#if defined(_SI473x_VERSION_3040_)
#if defined(_CHANGE_SI473x_INPUT_)
static const uint8 CMD_SI473x_AM_AUTO_VOL_CONTROL_MAX_GAIN[]	= {6,0x12,0x00,0x31,0x03,0x78,0x43}; //该参数更改0x7843 2019.6.14
#else
static const uint8 CMD_SI473x_AM_AUTO_VOL_CONTROL_MAX_GAIN[]	= {6,0x12,0x00,0x31,0x03,0x2E,0xE0};
#endif
#endif

#if defined(FEIYIN_473x_C40_ADJUST)&&defined(_SI473x_VERSION_3040_)
static const uint8 CMD_SI473x_AM_AUTO_VOL_CONTROL_MAX_GAIN2[]	= {6,0x12,0x00,0x31,0x03,0x40,0x00};
static const uint8 CMD_SI473x_AM_FRONTEND_AGC_CONTROL[]			= {6,0x12,0x00,0x37,0x05,0x00,0x00};
#endif

static const uint8 CMD_SI473x_AM_SEEK_START[]					= {1,0x41};
static const uint8 CMD_SI473x_AM_SEEK_STOP[]					= {2,0x42,0x03};
static const uint8 CMD_SI473x_AM_SEEK_BAND_BOTTOM[]				= {4,0x12,0x00,0x34,0x00};
static const uint8 CMD_SI473x_AM_SEEK_BAND_TOP[]				= {4,0x12,0x00,0x34,0x01};
static const uint8 CMD_SI473x_AM_SEEK_BAND_STEP[]				= {5,0x12,0x00,0x34,0x02,0x00};
static const uint8 CMD_SI473x_GET_FM_FREQ[]						= {2,0x22,0x01};

#if defined(_ENABLE_RADIO_WB_)
static const uint8 CMD_SI473x_SET_WB_FREQ[]						= {2,0x50,0x00};
static const uint8 CMD_SI473x_WB_TUNE_STATUS[]					= {2,0x52,0x01};
static const uint8 CMD_SI473x_WB_ASQ_STATUS[]					= {2,0x55,0x01};
#endif

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static const uint8 *CMD_SI473x_TAB[]=
{
/*-------------------------------------------------------------------------------------------------------------------------*/
//	公共命令
	CMD_SI473x_POWER_UP_REC_FM,//0
	CMD_SI473x_POWER_UP_REC_AM,
	CMD_SI473x_POWER_UP_REC_WB,
	CMD_SI473x_POWER_UP_TRN_FM,
	CMD_SI473x_GET_REV,	
	CMD_SI473x_POWER_DOWN,//5
	CMD_SI473x_GET_INT_STATUS,

	CMD_SI473x_AUDIO_VOLUME,
	CMD_SI473x_AUDIO_MUTE,
	CMD_SI473x_AUDIO_UNMUTE,

	CMD_SI473x_GPIO_DIR,//10
	CMD_SI473x_GPIO_DATA,
/*-------------------------------------------------------------------------------------------------------------------------*/
//	调频命令
	CMD_SI473x_SET_FM_FREQ,
	CMD_SI473x_FM_TUNE_STATUS,
	CMD_SI473x_FM_RSQ_STATUS,
	CMD_SI473x_FM_AGC_STATUS,//15
	CMD_SI473x_FM_AGC_OVERRIDE,

	CMD_SI473x_FM_SEEK_TUNE_SNR_SET,
	CMD_SI473x_FM_SEEK_TUNE_RSSI_SET,

	CMD_SI473x_SET_FM_DEEMPHASIS_50US,
	CMD_SI473x_SET_FM_DEEMPHASIS_75US,//20
	CMD_SI473x_FM_BLEND_STEREO_ON,
	CMD_SI473x_FM_BLEND_MONO_OFF,
	CMD_SI473x_FM_BLEND_STEREO_OFF,
	CMD_SI473x_FM_BLEND_MONO_ON,
	CMD_SI473x_FM_TUNE_ERROR,//25
	CMD_SI473x_FM_SOFT_MUTE_RATE,
	CMD_SI473x_FM_SOFT_MUTE_ATTENUATION,
	CMD_SI473x_FM_SOFT_MUTE_SNR,
#if defined(_SI473x_VERSION_3020_)
	CMD_SI473x_FM_SOFT_MUTE_ATTENUATION_B20,
	CMD_SI473x_FM_SOFT_MUTE_SNR_B20,//30
#endif

	CMD_SI473x_GET_FM_BLEND_STEREO,
	CMD_SI473x_GET_FM_BLEND_MONO,

#if defined(_ENABLE_RADIO_RDS_)	
//  RDS命令
	CMD_SI473x_FM_RDS_DETECT,
	CMD_SI473x_FM_RDS_STATUS,
	CMD_SI473x_FM_RDS_INT_SOURCE,
	CMD_SI473x_FM_RDS_FIFO_COUNT,
	CMD_SI473x_FM_RDS_CONFIG,
	CMD_SI473x_FM_RDS_RESET,
#endif

#if defined(_SI473x_VERSION_3060_)
	CMD_SI473x_FM_BLEND_RSSI_STEREO_ON,
	CMD_SI473x_FM_BLEND_RSSI_MONO_OFF,
	CMD_SI473x_FM_BLEND_RSSI_STEREO_OFF,//35
	CMD_SI473x_FM_BLEND_RSSI_MONO_ON,
	CMD_SI473x_FM_BLEND_RSSI_ATTACK_RATE,
	CMD_SI473x_FM_BLEND_RSSI_RELEASE_RATE,

	CMD_SI473x_FM_BLEND_SNR_STEREO_ON,
	CMD_SI473x_FM_BLEND_SNR_MONO_OFF,//40
	CMD_SI473x_FM_BLEND_SNR_STEREO_OFF,
	CMD_SI473x_FM_BLEND_SNR_MONO_ON,
	CMD_SI473x_FM_BLEND_SNR_ATTACK_RATE,
	CMD_SI473x_FM_BLEND_SNR_RELEASE_RATE,

	CMD_SI473x_FM_BLEND_MULTIPATH_STEREO_ON,//45
	CMD_SI473x_FM_BLEND_MULTIPATH_MONO_OFF,
	CMD_SI473x_FM_BLEND_MULTIPATH_STEREO_OFF,
	CMD_SI473x_FM_BLEND_MULTIPATH_MONO_ON,
	CMD_SI473x_FM_BLEND_MULTIPATH_ATTACK_RATE,
	CMD_SI473x_FM_BLEND_MULTIPATH_RELEASE_RATE,//50

	CMD_SI473x_FM_BLEND_MULTIPATH_STEREO_DEFAULT,
	CMD_SI473x_FM_BLEND_MULTIPATH_MONO_DEFAULT,

	CMD_SI473x_FM_HICUT_HIGH_THRESHOLD_DEFAULT,
	CMD_SI473x_FM_HICUT_LOW_THRESHOLD_DEFAULT,
	CMD_SI473x_FM_HICUT_MULTIPATH_TRIGGER_THRESHOLD_DEFAULT,//55
	CMD_SI473x_FM_HICUT_MULTIPATH_END_THRESHOLD_DEFAULT,
	CMD_SI473x_FM_HICUT_OFF_FREQUENCY_DEFAULT,
#endif

	CMD_SI473x_FM_SEEK_START,
	CMD_SI473x_FM_SEEK_STOP,
	CMD_SI473x_FM_SEEK_BAND_BOTTOM,//60硬件搜台最低频率
	CMD_SI473x_FM_SEEK_BAND_TOP,//硬件搜台最大频率
	CMD_SI473x_FM_SEEK_BAND_STEP,//硬件搜台步进

/*-------------------------------------------------------------------------------------------------------------------------*/
//	调幅命令
	CMD_SI473x_SET_AM_FREQ,
	CMD_SI473x_AM_TUNE_STATUS,
	CMD_SI473x_AM_RSQ_STATUS,//65
	
	CMD_SI473x_SET_AM_DEEMPHASIS_OFF,
	CMD_SI473x_SET_AM_DEEMPHASIS_50US,

	CMD_SI473x_AM_SOFT_MUTE_RATE,
	CMD_SI473x_AM_SOFT_MUTE_SLOPE,
	CMD_SI473x_AM_SOFT_MUTE_ATTENUATION,//70
	CMD_SI473x_AM_SOFT_MUTE_SNR,

	CMD_SI473x_AM_SEEK_TUNE_SNR_SET,
	CMD_SI473x_AM_SEEK_TUNE_RSSI_SET,
	
#if defined(_SI473x_VERSION_3040_)|| defined(_SI473x_VERSION_3060_)
	CMD_SI473x_AM_AUTO_VOL_CONTROL_MAX_GAIN,
#endif

#if defined(FEIYIN_473x_C40_ADJUST)&&defined(_SI473x_VERSION_3040_)
	CMD_SI473x_AM_AUTO_VOL_CONTROL_MAX_GAIN2,
	CMD_SI473x_AM_FRONTEND_AGC_CONTROL,
#endif

	CMD_SI473x_AM_SEEK_START,//75
	CMD_SI473x_AM_SEEK_STOP,
	CMD_SI473x_AM_SEEK_BAND_BOTTOM,
	CMD_SI473x_AM_SEEK_BAND_TOP,
	CMD_SI473x_AM_SEEK_BAND_STEP,
	CMD_SI473x_GET_FM_FREQ,
#if defined(_ENABLE_RADIO_WB_)
//	WB命令
	CMD_SI473x_SET_WB_FREQ,
	CMD_SI473x_WB_TUNE_STATUS,
	CMD_SI473x_WB_ASQ_STATUS,
#endif
};

static const uint16 fm_si473x_deal_spot_tab[] = {9600};
static const uint16 am_si473x_deal_spot_tab[] = {};

/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

void CopyComToSi473xOrder(si473x_order_e Order)
{//把命令拷贝到4730buff中,其中第一个字节为命令的长度
	if(Order < SI473x_CMD_ORDER_MAX)
	{
		const uint8 *tab_buf = CMD_SI473x_TAB[Order];
		uint8 length = tab_buf[0];
		uint8 index;
		
		for(index = 0; index < length; index++)
		{
			Si473xOrder->buffer[index] = tab_buf[1+index];
		}
		Si473xOrder->length = length;
	}
}

static uint8 SendDataToSi473x(void)
{//把4730buf的数据写入4730寄存器中
	i2c_data_init(SI473x_I2C_ADDR, SI473x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(Si473xOrder->buffer, Si473xOrder->length);
	i2c_data_exit();
	return result;
}

static uint8 ReceDataForSi473x(uint8 length)
{//读取4730的数据到4730buf中
	i2c_data_init(SI473x_I2C_ADDR, SI473x_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_r(Si473xOrder->buffer, length);
	i2c_data_exit();
	return result;
}

static uint8 WaitSi473xCts(void)
{
	uint8 i;
	
	for(i = 0; i < 100; i++)
	{
		if(ReceDataForSi473x(1))
		{
			if(Si473xOrder->buffer[0] & _B7_)return TRUE;
		}
		else break;
	}
	return FALSE;
}

static uint8 SendSi473xData(void)
{
	if(!SendDataToSi473x())return FALSE;
	if(!WaitSi473xCts())return FALSE;
	return TRUE;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

static uint8 SendSi473xOrder(si473x_order_e Order, uint8 read_length)
{//发送4730命令
	uint8 result = TRUE;
	
	if(Order < SI473x_CMD_ORDER_MAX)
	{
		CopyComToSi473xOrder(Order);	
		if((Order == SI473x_CMD_POWER_UP_REC_FM)
		|| (Order == SI473x_CMD_POWER_UP_REC_AM)
		|| (Order == SI473x_CMD_POWER_UP_REC_WB)
		|| (Order == SI473x_CMD_POWER_UP_TRN_FM))
		{
			result = SendDataToSi473x();
		}
		else
		{
			result = SendSi473xData();
		}
	}
	
	if(result)
	{
		if(read_length)
		{//命令发送完 判断是否需要返回数据
			result = ReceDataForSi473x(read_length);
		}
	}
	
	return result;
}

static uint8 GetSi473xStc(uint8 *stc)
{//获取STC标志位
	if(!SendSi473xOrder(SI473x_CMD_GET_INT_STATUS, 0))return FALSE;
	
	if(Si473xOrder->buffer[0] & _B0_)
	{
		*stc = TRUE;
	}
	else 
	{
		*stc = FALSE;
	}

	return TRUE;
}

static uint8 CheckSi473xStc(void)
{
	uint8 index;
	uint8 stc;

	for(index = 0; index < 50; index++)
	{
		if(!GetSi473xStc(&stc))return FALSE;
		if(stc)break;
		
		OSTimeDly(1);
	}
	
	if(index == 50)return FALSE;
	return TRUE;
}

static uint8 SendSi473xFmFreq(uint16 Freq)
{//写4730FM的频率
	CopyComToSi473xOrder(SI473x_CMD_SET_FM_FREQ);
	
	Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8h_(Freq);
	Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8l_(Freq);
	//Si473xOrder->buffer[Si473xOrder->Length ++] = 0x00;
	if(!SendSi473xData())return FALSE;
	
	return TRUE;
}

static uint8 SendSi473xAmFreq(uint16 Freq)
{//写4730AM的频率
	CopyComToSi473xOrder(SI473x_CMD_SET_AM_FREQ);
	
	Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8h_(Freq);
	Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8l_(Freq);

#if defined(_SI473x_VERSION_3040_)|| defined(_SI473x_VERSION_3060_)						
	Si473xOrder->buffer[Si473xOrder->length ++] = 0;
	Si473xOrder->buffer[Si473xOrder->length ++] = 1;
#else
	Si473xOrder->buffer[Si473xOrder->length ++] = 0x00;
	Si473xOrder->buffer[Si473xOrder->length ++] = 0x00;
#endif
	
	if(!SendSi473xData())return FALSE;
	
	return TRUE;
}

#if defined(_ENABLE_RADIO_WB_)
 static uint8 SendSi473xWbFreq(uint16 Freq)
 {//写4730WB的频率
	 CopyComToSi473xOrder(SI473x_CMD_SET_WB_FREQ);
	 
	 Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8h_(Freq);
	 Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8l_(Freq);
	 if(!SendSi473xData())return FALSE;
	 
	 return TRUE;
 }
#endif

 uint8 SendSi473xVolume(uint8 Volume)
{//写4730的音量	
	if(Volume > 63)Volume = 63;
	
	CopyComToSi473xOrder(SI473x_CMD_AUDIO_VOLUME);
	
	Si473xOrder->buffer[Si473xOrder->length ++] = Volume;
	if(!SendSi473xData())return FALSE;
	
	return TRUE;
}

static uint8 SetSi473xSeekSignal(uint8 Snr,uint8 Rssi)
{//设置搜台的SNR及RSSI门限值
	si473x_order_e OrderSnr;
	si473x_order_e OrderRssi;
	
	switch(si473xCtrl->TunerBand)
	{
		case SI473x_TUNER_BAND_REC_FM:
			OrderSnr	= SI473x_CMD_FM_SEEK_TUNE_SNR_SET;
			OrderRssi	= SI473x_CMD_FM_SEEK_TUNE_RSSI_SET;
			break;
		case SI473x_TUNER_BAND_REC_AM:
			OrderSnr	= SI473x_CMD_AM_SEEK_TUNE_SNR_SET;
			OrderRssi	= SI473x_CMD_AM_SEEK_TUNE_RSSI_SET;
			break;
		default:
			return FALSE;
	}
	
	CopyComToSi473xOrder(OrderSnr);
	Si473xOrder->buffer[Si473xOrder->length ++] = Snr;
	if(!SendSi473xData())return FALSE;
	
	CopyComToSi473xOrder(OrderRssi);
	Si473xOrder->buffer[Si473xOrder->length ++] = Rssi;
	if(!SendSi473xData())return FALSE;
	
	return TRUE;	
}

static uint8 SetSi473xSeekFreq(uint16 MinFreq,uint16 MaxFreq,uint8 Step)
{//设置硬件搜台的最小、最大频率 及搜台的步进
	si473x_order_e OrderMin;
	si473x_order_e OrderMax;
	si473x_order_e OrderStep;
	
	MinFreq -= (MinFreq % Step);
	MaxFreq -= (MaxFreq % Step);
	if(MinFreq > MaxFreq)return FALSE;
	
	switch(si473xCtrl->TunerBand)
	{
		case SI473x_TUNER_BAND_REC_FM:
			OrderMin  = SI473x_CMD_FM_SEEK_BAND_BOTTOM;
			OrderMax  = SI473x_CMD_FM_SEEK_BAND_TOP;
			OrderStep = SI473x_CMD_FM_SEEK_BAND_STEP;
			break;
		case SI473x_TUNER_BAND_REC_AM:
			OrderMin  = SI473x_CMD_AM_SEEK_BAND_BOTTOM;
			OrderMax  = SI473x_CMD_AM_SEEK_BAND_TOP;
			OrderStep = SI473x_CMD_AM_SEEK_BAND_STEP;
			break;
		default:
			return FALSE;
	}
	
	CopyComToSi473xOrder(OrderMin);
	Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8h_(MinFreq);
	Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8l_(MinFreq);
	if(!SendSi473xData())return FALSE;
	
	CopyComToSi473xOrder(OrderMax);
	Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8h_(MaxFreq);
	Si473xOrder->buffer[Si473xOrder->length ++] = _u16to8l_(MaxFreq);
	if(!SendSi473xData())return FALSE;
	
	CopyComToSi473xOrder(OrderStep);
	Si473xOrder->buffer[Si473xOrder->length ++] = Step;
	if(!SendSi473xData())return FALSE;
	
	return TRUE;
}

static uint8 SetSi473xSeekStart(uint8 Mode)
{//开始硬件搜台
	si473x_order_e Order;
	uint8 Arg;
	
	switch(si473xCtrl->TunerBand)
	{
		case SI473x_TUNER_BAND_REC_FM:
			Order = SI473x_CMD_FM_SEEK_START;
			break;
		case SI473x_TUNER_BAND_REC_AM:
			Order = SI473x_CMD_AM_SEEK_START;
			break;
		default:return FALSE;
	}

	switch(Mode)
	{//bit3:up->1  down->0   bit2:wrap->1 halt->0
		case TUNER_SEEK_UP_WRAP:
			Arg = ((1<<3)|(1<<2));
			break;
		case TUNER_SEEK_UP_HALT:
			Arg = ((1<<3)|(0<<2));
			break;
		case TUNER_SEEK_DOWN_WRAP:
			Arg = ((0<<3)|(1<<2));
			break;
		case TUNER_SEEK_DOWN_HALT:
			Arg = ((0<<3)|(0<<2));
			break;
	}
	
	CopyComToSi473xOrder(Order);
	
	Si473xOrder->buffer[Si473xOrder->length ++] = Arg;
	if(!SendSi473xData())return FALSE;
	
	return TRUE;
}

static uint8 SetSi473xSeekStop(void)
{//停止硬件搜台
	si473x_order_e Order;
	
	switch(si473xCtrl->TunerBand)
	{
		case SI473x_TUNER_BAND_REC_FM:
			Order = SI473x_CMD_FM_SEEK_STOP;
			break;
		case SI473x_TUNER_BAND_REC_AM:
			Order = SI473x_CMD_AM_SEEK_STOP;
			break;
		default:return FALSE;
	}
	
	CopyComToSi473xOrder(Order);
	if(!SendSi473xData())return FALSE;
	
	return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 si473x_set_init(void)
{
	if(si473xCtrl->init_succeed)
	{
		si473xCtrl->init_succeed = FALSE;
		
		if(!SendSi473xOrder(SI473x_CMD_AUDIO_MUTE, 0))return FALSE;
		
		OSTimeDly(1);
		
		if(!SendSi473xOrder(SI473x_CMD_POWER_DOWN, 0))return FALSE;
		
		OSTimeDly(5);
	}
	switch(si473xCtrl->TunerBand)
	{
		case SI473x_TUNER_BAND_REC_FM:
			if(!SendSi473xOrder(SI473x_CMD_POWER_UP_REC_FM, 0))return FALSE;
			break;
		case SI473x_TUNER_BAND_REC_AM:
			if(!SendSi473xOrder(SI473x_CMD_POWER_UP_REC_AM, 0))return FALSE;
			break;
		case SI473x_TUNER_BAND_REC_WB:
			if(!SendSi473xOrder(SI473x_CMD_POWER_UP_REC_WB, 0))return FALSE;
			break;
		case SI473x_TUNER_BAND_TRN_FM:
			if(!SendSi473xOrder(SI473x_CMD_POWER_UP_TRN_FM, 0))return FALSE;
			break;
		default:return FALSE;
	}
	
	OSTimeDly(20);

	if(!SendSi473xOrder(SI473x_CMD_GET_REV, sizeof(TUNER_REVISION_FORMAT)))return FALSE;
	
	if((TunerRevision.MajorRev == 0x34)&&(TunerRevision.MinorRev == 0x30)&&(TunerRevision.Chip ==0x43))
	{//C40
		si473xCtrl->TunerVersion = SI473x_TUNER_VERSION_C40;
	}
	else if((TunerRevision.MajorRev == 0x36)&&(TunerRevision.MinorRev == 0x30)&&(TunerRevision.Chip ==0x44))
	{//D60	
		si473xCtrl->TunerVersion = SI473x_TUNER_VERSION_D60;
	}
	else
	{//B20
		si473xCtrl->TunerVersion = SI473x_TUNER_VERSION_B20;
	}
	
	switch(si473xCtrl->TunerBand)
	{
		case SI473x_TUNER_BAND_REC_FM:
			//------------------------------------------------------------------------
			//	配置去加重参数		
		#if 1
			//EUROPE
			if(!SendSi473xOrder(SI473x_CMD_SET_FM_DEEMPHASIS_50US, 0))return FALSE;	
		#else
			//USA
			if(!SendSi473xOrder(SI473x_CMD_SET_FM_DEEMPHASIS_75US, 0))return FALSE;
		#endif
			
			//	配置频偏值
			if(!SendSi473xOrder(SI473x_CMD_FM_TUNE_ERROR, 0))return FALSE;
		
			if(si473xCtrl->TunerVersion == SI473x_TUNER_VERSION_B20)
			{
				//	配置软件最大静音衰减
				if(!SendSi473xOrder(SI473x_CMD_FM_SOFT_MUTE_ATTENUATION_B20, 0))return FALSE;
				//	配置软件静音NSR开端
				if(!SendSi473xOrder(SI473x_CMD_FM_SOFT_MUTE_SNR_B20, 0))return FALSE;
			}
			else
			{
				//	配置软件最大静音衰减
				if(!SendSi473xOrder(SI473x_CMD_FM_SOFT_MUTE_ATTENUATION, 0))return FALSE;
			
				//	配置软件静音NSR开端
				if(!SendSi473xOrder(SI473x_CMD_FM_SOFT_MUTE_SNR, 0))return FALSE;
			}
			
			//	配置高频AGC(自动增益控制)控制
			if(!SendSi473xOrder(SI473x_CMD_FM_AGC_OVERRIDE, 0))return FALSE;
			
		//------------------------------------------------------------------------
		//	开机FM哮叫
		#if 0
			if(!SetSi473xSeekSignal(0, 0))return FALSE;
		#else
			if(!SetSi473xSeekSignal(SI473x_TUNER_SNR_FM, SI473x_TUNER_RSSI_FM))return FALSE;
		#endif
		//------------------------------------------------------------------------
		//	配制音量
		#if 0
			if(!SendSi473xVolume(60))return FALSE;
		#endif
		
        #if defined(_ENABLE_RADIO_RDS_)
            if(!SendSi473xOrder(SI473x_CMD_FM_RDS_FIFO_COUNT, 0))return FALSE;
            if(!SendSi473xOrder(SI473x_CMD_FM_RDS_INT_SOURCE, 0))return FALSE;
            if(!SendSi473xOrder(SI473x_CMD_FM_RDS_CONFIG, 0))return FALSE;
            if(!SendSi473xOrder(SI473x_CMD_FM_RDS_STATUS, 0))return FALSE;
        #endif
			
			if(si473xCtrl->TunerVersion == SI473x_TUNER_VERSION_D60)
			{
				if(!SendSi473xOrder(SI473x_CMD_FM_HICUT_SNR_HIGH_THRESHOLD_DEFAULT, 0))return FALSE;
				if(!SendSi473xOrder(SI473x_CMD_FM_HICUT_SNR_LOW_THRESHOLD_DEFAULT, 0))return FALSE;
				//if(!SendSi473xOrder(SI473x_CMD_FM_HICUT_ATTACK_RATE_DEFAULT, 0))return FALSE;
				//if(!SendSi473xOrder(SI473x_CMD_FM_HICUT_RELEASE_RATE_DEFAULT, 0))return FALSE;
				//if(!SendSi473xOrder(SI473x_CMD_FM_HICUT_MULTIPATH_TRIGGER_THRESHOLD_DEFAULT, 0))return FALSE;
				if(!SendSi473xOrder(SI473x_CMD_FM_HICUT_MULTIPATH_END_THRESHOLD_DEFAULT, 0))return FALSE;
				if(!SendSi473xOrder(SI473x_CMD_FM_HICUT_OFF_FREQUENCY_DEFAULT, 0))return FALSE;
			}
		//------------------------------------------------------------------------
			break;
		case SI473x_TUNER_BAND_REC_AM:	
		//------------------------------------------------------------------------
		//	配置去加重参数		
			if(!SendSi473xOrder(SI473x_CMD_SET_AM_DEEMPHASIS_50US, 0))return FALSE;
		//------------------------------------------------------------------------
			//	配置软件最大静音衰减
			if(!SendSi473xOrder(SI473x_CMD_AM_SOFT_MUTE_ATTENUATION, 0))return FALSE;

			//	配置软件静音NSR开端
			if(!SendSi473xOrder(SI473x_CMD_AM_SOFT_MUTE_SNR, 0))return FALSE;
		//------------------------------------------------------------------------
		//	开机AM哮叫	
		#if 0
			if(!SetSi473xSeekSignal(0, 0))return FALSE;
		#else
			if(!SetSi473xSeekSignal(SI473x_TUNER_SNR_AM, SI473x_TUNER_RSSI_AM))return FALSE;
		#endif
		//------------------------------------------------------------------------
		//	
		
		#if defined(FEIYIN_473x_C40_ADJUST)
			if(!SendSi473xOrder(SI473x_CMD_AM_AUTO_VOL_CONTROL_MAX_GAIN2, 0))return FALSE;
			if(!SendSi473xOrder(SI473x_CMD_AM_FRONTEND_AGC_CONTROL, 0))return FALSE;
		#else
			if((si473xCtrl->TunerVersion == SI473x_TUNER_VERSION_C40)
			 ||(si473xCtrl->TunerVersion == SI473x_TUNER_VERSION_D60))
			{
				if(!SendSi473xOrder(SI473x_CMD_AM_AUTO_VOL_CONTROL_MAX_GAIN, 0))return FALSE;
			}
		#endif
		//------------------------------------------------------------------------
		//	配制音量
		#if 0
			if(!SendSi473xVolume(63))return FALSE;
		#endif
		//------------------------------------------------------------------------			
			break;
		
    #if defined(_ENABLE_RADIO_WB_)
    	case SI473x_TUNER_BAND_REC_WB:
    		break;
    #endif

		default:
			return FALSE;
	}

	return TRUE;
}

static uint8 si473x_set_mute(uint8 mute)
{
	if(mute)
	{//4730mute
		if(!SendSi473xOrder(SI473x_CMD_AUDIO_MUTE, 0))return FALSE;
	}
	else
	{//4730unmute
		if(!SendSi473xOrder(SI473x_CMD_AUDIO_UNMUTE, 0))return FALSE;
	}
	return TRUE;
}

static uint8 si473x_set_stereo(uint8 stereo)
{
#if defined(_SI473x_VERSION_3060_)
	if(si473xCtrl->TunerVersion == SI473x_TUNER_VERSION_D60)
	{
		if(stereo)
		{
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEDN_RSSI_STERO_ON_THRESHOLD, 0))return FALSE;					
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEDN_RSSI_MONO_OFF_THRESHOLD, 0))return FALSE; 
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_SNR_STERO_ON_THRESHOLD, 0))return FALSE;				
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_SNR_MONO_OFF_THRESHOLD, 0))return FALSE;	
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_MULTIPATH_STERO_THRESHOLD_DEFAULT, 0))return FALSE;					
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_MULTIPATH_STERO_THRESHOLD_DEFAULT, 0))return FALSE;
		}
		else
		{
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEDN_RSSI_STERO_OFF_THRESHOLD, 0))return FALSE;					
 			if(!SendSi473xOrder(SI473x_CMD_FM_BLEDN_RSSI_MONO_ON_THRESHOLD, 0))return FALSE; 
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_SNR_STERO_OFF_THRESHOLD, 0))return FALSE;					
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_SNR_MONO_ON_THRESHOLD, 0))return FALSE;	
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_MULTIPATH_STERO_THRESHOLD_DEFAULT, 0))return FALSE;					
			if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_MULTIPATH_STERO_THRESHOLD_DEFAULT, 0))return FALSE;
		}
	}
	else
#endif
	{
		if(stereo)
		{
			if(!SendSi473xOrder(SI473x_CMD_GET_FM_BLEND_STEREO, sizeof(FmBlendStereo)))return FALSE;
			if(FmBlendStereo.Value.Stereo != SI473x_TUNER_STEREO_VALUE) 
			{
				if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_STEREO_ON, 0))return FALSE;
			}
		 
			if(!SendSi473xOrder(SI473x_CMD_GET_FM_BLEND_MONO, sizeof(FmBlendMono)))return FALSE;
			if(FmBlendMono.Value.Mono != SI473x_TUNER_MONO_VALUE) 
			{	
				if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_MONO_OFF, 0))return FALSE;
			}
		}
		else
		{
			if(!SendSi473xOrder(SI473x_CMD_GET_FM_BLEND_STEREO, sizeof(FmBlendStereo)))return FALSE;
			if(FmBlendStereo.Value.Stereo != 127) 
			{
				if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_STEREO_OFF, 0))return FALSE;	
			}
			
			if(!SendSi473xOrder(SI473x_CMD_GET_FM_BLEND_MONO, sizeof(FmBlendMono)))return FALSE;
			if(FmBlendMono.Value.Mono != 127) 
			{
				if(!SendSi473xOrder(SI473x_CMD_FM_BLEND_MONO_ON, 0))return FALSE;
			}
		}
	}
	
	return TRUE;
}


static uint8 si473x_reset_autoseek(void)
{
	if(si473xCtrl->TunerAutoSeek)
	{	
		uint8 stc;

		puts("si473x_reset_autoseek\n");
		if(!GetSi473xStc(&stc))return FALSE;
		
		if(!stc)
		{
			if(!SetSi473xSeekStop())return FALSE;
			
			OSTimeDly(5);
			
			puts("si473x_reset_autoseek1111\n");
			if(!CheckSi473xStc())return FALSE;
		}
		
		si473xCtrl->TunerAutoSeek = FALSE;
	}
	
	return TRUE;
}

#if defined(_ENABLE_RADIO_RDS_)

static void si473x_rds_reset(void)
{
	if(si473xCtrl->tuner_rds.enable)
	{
		rds_dec_reset();
		si473xCtrl->tuner_rds.discard = 1;
		si473xCtrl->tuner_rds.quality = 0;
	}
}

static void si473x_rds_decode(void)
{
	if(si473xCtrl->tuner_rds.quality >= si473xCtrl->tuner_rds.factor)
	{
		si473xCtrl->tuner_rds.quality -= si473xCtrl->tuner_rds.factor;
	}
	rds_dec_timer();
	
	if(!SendSi473xOrder(SI473x_CMD_GET_INT_STATUS, 0))return;
	if(!(Si473xOrder->buffer[0] & _B0_))return;
	if(!(Si473xOrder->buffer[0] & _B2_))return;
	
	if(si473xCtrl->tuner_rds.discard)
	{
		if(!SendSi473xOrder(SI473x_CMD_FM_RDS_RESET, 0))return;
		si473xCtrl->tuner_rds.discard--;
	}
	else
	{
		uint8 max_loop = 10;
		do{
			if(!SendSi473xOrder(SI473x_CMD_FM_RDS_STATUS, sizeof(FM_RDS_STATUS_FORMAT)))return;
			
			if((FmRdsStatus.Resp12.BleA == 0)
			 &&(FmRdsStatus.Resp12.BleB == 0)
			 &&(FmRdsStatus.Resp12.BleC == 0)
			 &&(FmRdsStatus.Resp12.BleD == 0))
			{
				rds_input_t input;
				
				input.block_ah = FmRdsStatus.BlockA_h;
				input.block_al = FmRdsStatus.BlockA_l;
				input.block_bh = FmRdsStatus.BlockB_h;
				input.block_bl = FmRdsStatus.BlockB_l;
				input.block_ch = FmRdsStatus.BlockC_h;
				input.block_cl = FmRdsStatus.BlockC_l;
				input.block_dh = FmRdsStatus.BlockD_h;
				input.block_dl = FmRdsStatus.BlockD_l;
				
				si473xCtrl->tuner_rds.quality = 0xFF;
				
				rds_stream_dec(&input);
			}
			else 
			{
				if(FmRdsStatus.Resp12.BleA == 0)
				{
					//tuner_puts("si473x_rds_only_block_a\n");
					si473xCtrl->tuner_rds.quality = 0xFF;
					rds_block_a_dec(FmRdsStatus.BlockA_h, FmRdsStatus.BlockA_l);
				}
				
				if(FmRdsStatus.Resp12.BleB == 0)
				{
					//tuner_puts("si473x_rds_only_block_b\n");
					si473xCtrl->tuner_rds.quality = 0xFF;
					rds_block_b_dec(FmRdsStatus.BlockB_h, FmRdsStatus.BlockB_l);
				}
				
				if(FmRdsStatus.Resp12.BleC == 0)
				{
					//tuner_puts("si473x_rds_only_block_c\n");
					si473xCtrl->tuner_rds.quality = 0xFF;
					rds_block_c_dec(FmRdsStatus.BlockC_h, FmRdsStatus.BlockC_l);
				}
				
				if(FmRdsStatus.Resp12.BleD == 0)
				{
					//tuner_puts("si473x_rds_only_block_d\n");
					si473xCtrl->tuner_rds.quality = 0xFF;
					rds_block_d_dec(FmRdsStatus.BlockD_h, FmRdsStatus.BlockD_l);
				}
			}
			max_loop--;
		}
		while((FmRdsStatus.RdsFifoUsed > 1) && max_loop);
	}
}

#endif

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tuner_si473x_detect(void);
static uint8 tuner_si473x_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_si473x_exit(void);
static uint8 tuner_si473x_freq(uint16);
static uint8 tuner_si473x_status(engine_status_t *);
static uint8 tuner_si473x_search(uint16, engine_status_t *);
static uint8 tuner_si473x_hard(engine_hard_t *);
static uint8 tuner_si473x_setting(fm_setting_e, void *);
static uint8 tuner_si473x_info(fm_info_e, void *);


static uint8 tuner_si473x_detect(void)
{
	uint8 result;
	
	i2c_data_init(SI473x_I2C_ADDR, SI473x_I2C_RATE, &radio_gpio);
	result = i2c_data_w(NULL, 0);
	i2c_data_exit();
	//tuner_puts("tuner_si473x_detect succeed\n");
	tuner_printf("tuner_si473x_detect:%d\n", result);
	return result;
}

static uint8 tuner_si473x_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	if(Si473xOrder == NULL)
	{
		Si473xOrder = (si473x_order_t *)malloc_fun(Si473xOrder, sizeof(si473x_order_t), 0);
		if(Si473xOrder == NULL)return FALSE;
	}
	
	if(si473xCtrl == NULL)
	{
		si473xCtrl = (si473x_ctrl_t *)malloc_fun(si473xCtrl, sizeof(si473x_ctrl_t), 0);
		if(si473xCtrl == NULL)
		{
			free_fun((void **)&Si473xOrder);
			return FALSE;
		}
	}
	tuner_puts("si473x_memory_ok\n");
	
	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto si473x_init_error;
			if(min_freq < 6400)goto si473x_init_error;
			if(max_freq > 10800)goto si473x_init_error;
			si473xCtrl->TunerBand = SI473x_TUNER_BAND_REC_FM;
			break;
			
		case RADIO_BAND_AM:
			if(min_freq > max_freq)goto si473x_init_error;
			if(min_freq < 520)goto si473x_init_error;
			if(max_freq > 1710)goto si473x_init_error;
			si473xCtrl->TunerBand = SI473x_TUNER_BAND_REC_AM;
			break;
			
    #if defined(_ENABLE_RADIO_WB_)
    	case RADIO_BAND_WB:
    		if(min_freq > max_freq)goto si473x_init_error;
    		if(min_freq < 64960)goto si473x_init_error;
    		if(max_freq > 65020)goto si473x_init_error;
    		//if(min_freq < 16240)goto si473x_init_error;
    		//if(max_freq > 16255)goto si473x_init_error;
    		si473xCtrl->TunerBand = SI473x_TUNER_BAND_REC_WB;
    		break;
    #endif

		default:
			goto si473x_init_error;
			break;
	}
	
	if(si473xCtrl->tuner_band == band)
	{
		if(si473xCtrl->init_succeed)
		{
			if(si473xCtrl->now_freq == freq)
			{
				si473xCtrl->min_freq = min_freq;
				si473xCtrl->max_freq = max_freq;
				if(!tuner_si473x_setting(TUR_SET_MUTE, OFF))goto si473x_init_error;
				return TRUE;
			}
			else
			{
				goto si473x_init_freq;
			}
		}
		else 
		{
			goto si473x_init_error;
		}
	}
	
	if(!si473x_set_init())goto si473x_init_error;
#if defined(_SPECIAL_473x_INIT_)
	OSTimeDly(5);
	if(!si473x_set_init())goto si473x_init_error;
#endif

si473x_init_freq:
	si473xCtrl->tuner_band = band;
	si473xCtrl->min_freq = min_freq;
	si473xCtrl->max_freq = max_freq;
	si473xCtrl->init_succeed = TRUE;
	si473xCtrl->TunerAutoSeek = FALSE;
	
	engine_status_t status;
	if(!tuner_si473x_search(freq, &status))goto si473x_init_error;
	OSTimeDly(10);
#if defined(_SPECIAL_473x_INIT_)
	if(!tuner_si473x_search(freq, &status))goto si473x_init_error;
	OSTimeDly(8);
#endif
	if(!tuner_si473x_search(freq, &status))goto si473x_init_error;
	if(!tuner_si473x_setting(TUR_SET_MUTE, OFF))goto si473x_init_error;
	tuner_puts("SI473x Init Succeed!!!!!\n");
	return TRUE;
	
si473x_init_error:
	free_fun((void **)&Si473xOrder);
	free_fun((void **)&si473xCtrl);
	tuner_puts("SI473x Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_si473x_exit(void)
{
	ew_printf("tuner_si473x_exit\n");
	if(Si473xOrder && si473xCtrl)
	{
		si473x_reset_autoseek();
	#if !defined(_ENABLE_RADIO_RESET_)
		SendSi473xOrder(SI473x_CMD_POWER_DOWN, 0);
	#endif
		free_fun((void **)&Si473xOrder);
		free_fun((void **)&si473xCtrl);
	}
	return TRUE;
}

static uint8 tuner_si473x_freq(uint16 freq)
{
	ew_printf("tuner_si473x_freq:%d\n",freq);
	if((Si473xOrder == NULL)||(si473xCtrl == NULL))return FALSE;
	if(!si473xCtrl->init_succeed)return FALSE;
	if(freq < si473xCtrl->min_freq)return FALSE;
	if(freq > si473xCtrl->max_freq)return FALSE;		
	
	switch(si473xCtrl->TunerBand)
	{
		case SI473x_TUNER_BAND_REC_FM:
			if(!SendSi473xFmFreq(freq))return FALSE;
			break;
			
		case SI473x_TUNER_BAND_REC_AM:
			if(!SendSi473xAmFreq(freq))return FALSE;
			break;
			
    #if defined(_ENABLE_RADIO_WB_)
    	case SI473x_TUNER_BAND_REC_WB:
    		if(!SendSi473xWbFreq(freq))return FALSE;
    		break;
    #endif

		default:return FALSE;
	}
#if defined(_ENABLE_RADIO_RDS_)
	si473x_rds_reset();
#endif

#if defined(DELAY_SEEK_SPEED)
	OSTimeDly(10);
#elif defined(_JYL_DELAY_SEEK_SPEED_)
	OSTimeDly(5);
#elif defined(_ZY32_DELAY_SEEK_SPEED_)
	OSTimeDly(2);
#elif defined(_KLD3800_SEEK_DELAY_)
	OSTimeDly(20);
#endif

	si473xCtrl->now_freq = freq;
	return TRUE;
}

static uint8 tuner_si473x_status(engine_status_t *status)
{
	if((Si473xOrder == NULL)||(si473xCtrl == NULL))return FALSE;
	if(!si473xCtrl->init_succeed)return FALSE;
		
#if defined(_ENABLE_RADIO_MUTE_)
    if(compare_task_name("DABTask"))
    {
		SendSi473xOrder(SI473x_CMD_AUDIO_MUTE, 0);
	}
	else
	{
		SendSi473xOrder(SI473x_CMD_AUDIO_UNMUTE, 0);
	}
#endif

	switch(si473xCtrl->TunerBand)
	{
		case SI473x_TUNER_BAND_REC_FM:
			if(si473xCtrl->TunerAutoSeek)
			{
				if(!SendSi473xOrder(SI473x_CMD_FM_TUNE_STATUS, sizeof(FM_TUNER_STATUS_FORMAT)))return FALSE;
				
				//当前调谐状态
				if(FmTunerStatus.Status.StcInt)
				{
					status->stc = TRUE;
				}
				else
				{
					status->stc = FALSE;
				}
				
				//信号强度
				status->signal = FmTunerStatus.Rssi;
				//printf("473Xsignal=:%d\n",FmTunerStatus.Rssi);
				//是否有无电台
				status->valid = FALSE;
				
				if(!FmTunerStatus.Resp1.Afc)
				{
					if(FmTunerStatus.Resp1.Valid)
					{
						status->valid = TRUE;
					}
				}
				
				//单声道
				status->stereo = FALSE;
				//当前频率
				si473xCtrl->now_freq = _u8tou16_(FmTunerStatus.Freq_h, FmTunerStatus.Freq_l);
			}
			else
			{
			#if defined(_EVB_DELAY_FM_SEEK_)
				OSTimeDly(4);
			#endif
				if(!SendSi473xOrder(SI473x_CMD_FM_RSQ_STATUS, sizeof(FM_RSQ_STATUS_FORMAT)))
					return false;
				//当前调谐状态
				if(FmRsqStatus.Status.StcInt)
				{
					status->stc = TRUE;
				}
				else
				{
					status->stc = FALSE;
				}
				
				//信号强度
				status->signal = FmRsqStatus.Rssi;
				
				//是否有无电台
				status->valid = FALSE;
				if(!FmRsqStatus.Resp2.Afc)
				{
					if(FmRsqStatus.Resp2.Valid)
					{
					#if defined(_ENABLE_SI474x_SEEK_)
						if((si473xCtrl->now_freq >= 8750)&&(si473xCtrl->now_freq <= 9600))
						{
							if(si473xCtrl->now_freq == 8850)
							{
								if(status->signal >= (SI473x_TUNER_RSSI_FM_DEFAULT+25))
								{
									status->valid = TRUE;
								}
							}
							else
							{
								if(status->signal >= (SI473x_TUNER_RSSI_FM_DEFAULT+12))
								{
									status->valid = TRUE;
								}
							}
						}
						else if((si473xCtrl->now_freq > 9600)&&(si473xCtrl->now_freq <= 10300))
						{
							if(status->signal >= (SI473x_TUNER_RSSI_FM_DEFAULT+6))
							{
								status->valid = TRUE;
							}
						}
						else
						{
							status->valid = TRUE;
						}
					#else
						status->valid = TRUE;
					#endif
					}
				}
				
				//立体声
				//ew_printf("Pilot:%d\n",FmRsqStatus.Resp3.Pilot);
				//ew_printf("status->signal:%d\n",status->signal);
				if(FmRsqStatus.Resp3.Pilot)
				{
				#if defined(_ENABLE_STO_ICON_)
					status->stereo = TRUE;
				#else
				    if(status->signal >= SI473x_TUNER_RSSI_FM_DEFAULT)
				    {
						status->stereo = TRUE;
					}
					else
					{
						status->stereo = FALSE;
					}
				#endif	
				}
				else 
				{
					status->stereo = FALSE;
				}
			}
			
			//当前频率
			status->freq = si473xCtrl->now_freq;
			
			//当前波段
			status->band = RADIO_BAND_FM;
			break;
			
		case SI473x_TUNER_BAND_REC_AM:
			if(si473xCtrl->TunerAutoSeek)
			{
				if(!SendSi473xOrder(SI473x_CMD_AM_TUNE_STATUS, sizeof(AmTunerStatus)))return FALSE;
				
				//当前调谐状态
				if(AmTunerStatus.Status.StcInt)
				{
					status->stc = TRUE;
				}
				else
				{
					status->stc = FALSE;
				}
				
				//信号强度
				status->signal = AmTunerStatus.Rssi;
				
				//是否有无电台
				status->valid = FALSE;
				if(!AmTunerStatus.Resp1.Afc)
				{
					if(AmTunerStatus.Resp1.Valid)
					{
						status->valid = TRUE;
					}
				}
				
				//当前频率
				si473xCtrl->now_freq = _u8tou16_(AmTunerStatus.Freq_h, AmTunerStatus.Freq_l);
			}
			else
			{
				if(!SendSi473xOrder(SI473x_CMD_AM_RSQ_STATUS, sizeof(AM_RSQ_STATUS_FORMAT)))return FALSE;

				//当前调谐状态
				if(AmRsqStatus.Status.StcInt)
				{
					status->stc = TRUE;
				}
				else
				{
					status->stc = FALSE;
				}
				
				//信号强度
				status->signal = AmRsqStatus.Rssi;
				
				//是否有无电台
				status->valid = FALSE;
				if(!AmRsqStatus.Resp2.Afc)
				{
					if(AmRsqStatus.Resp2.Valid)
					{
						status->valid = TRUE;
					}
				}
			}
			
			//立体声
			status->stereo = FALSE;
			
			//当前频率
			status->freq = si473xCtrl->now_freq;
			
			//当前波段
			status->band = RADIO_BAND_AM;
			break;
			
    #if defined(_ENABLE_RADIO_WB_)
    	case SI473x_TUNER_BAND_REC_WB:
    		if(!SendSi473xOrder(SI473x_CMD_WB_TUNE_STATUS, sizeof(WbTunerStatus)))return FALSE;
    
    		//当前调谐状态
    		if(WbTunerStatus.Status.StcInt)
    		{
    			status->stc = TRUE;
    		}
    		else
    		{
    			//status->stc = FALSE;
    			status->stc = TRUE;
    		}
    		
    		//信号强度
    		status->signal = WbTunerStatus.Rssi;
			
    		printf("status->signal:%d\n",status->signal);
    		printf("WbTunerStatus.Resp1.Afc:%d\n",WbTunerStatus.Resp1.Afc);
    		printf("WbTunerStatus.Resp1.Valid:%d\n",WbTunerStatus.Resp1.Valid);
			
    		//是否有无电台
    		status->valid = FALSE;
    		if(!WbTunerStatus.Resp1.Afc)
    		{
    			if(WbTunerStatus.Resp1.Valid)
    			{
    				if(status->signal > SI473x_RSSI_WB)
    				{
    					status->valid = TRUE;
    				}
    			}
    		}
    					
    		//立体声
    		status->stereo = FALSE;
    		
    		//当前频率
    		status->freq = si473xCtrl->now_freq;
    		
    		//当前波段
    		status->band = RADIO_BAND_WB;
    	break;
    #endif
	
		default:return FALSE;
	}	
	
	return TRUE;
}

static uint8 tuner_si473x_search(uint16 freq, engine_status_t *status)
{
	ew_printf("tuner_si473x_search\n");
	if((Si473xOrder == NULL)||(si473xCtrl == NULL))return FALSE;
	if(!si473xCtrl->init_succeed)return FALSE;
	
	if(!tuner_si473x_freq(freq))return FALSE;
	
	OSTimeDly(5);	
	
	if(!CheckSi473xStc())return FALSE;
	
	if(status->band == RADIO_BAND_AM)
	{//修改AM 在STC 置位后延时读RSSI
		OSTimeDly(2);
	}
	
#if defined(_ENABLE_RADIO_WB_)
	if(si473xCtrl->TunerBand == SI473x_TUNER_BAND_REC_WB)
	{//修改WB 在STC 置位后延时读RSSI
		OSTimeDly(2);
	}
#endif
	
	if(!tuner_si473x_status(status))return FALSE;
	
	return TRUE;
}

static uint8 tuner_si473x_hard(engine_hard_t *hard)
{
	ew_printf("tuner_si473x_hard\n");
	if((Si473xOrder == NULL)||(si473xCtrl == NULL))return FALSE;
	if(!si473xCtrl->init_succeed)return FALSE;
		
	if(hard->enable)
	{
		engine_hard_start_t *start = &hard->ctrl.start;
		engine_status_t status;
		
		if(start->min_freq < si473xCtrl->min_freq)return FALSE;
		if(start->max_freq > si473xCtrl->max_freq)return FALSE;	
		if(start->start_freq < start->min_freq)return FALSE;
		if(start->start_freq > start->max_freq)return FALSE;		
		
		if(!si473x_reset_autoseek())return FALSE;
		
		if(!tuner_si473x_search(start->start_freq, &status))return FALSE;
		
		if(!SetSi473xSeekFreq(start->min_freq, start->max_freq, start->step))return FALSE;
		
		if(start->dir)
		{
			if(!SetSi473xSeekStart(TUNER_SEEK_UP_WRAP))return FALSE;
		}
		else
		{
			if(!SetSi473xSeekStart(TUNER_SEEK_DOWN_WRAP))return FALSE;
		}
	#if defined(_ENABLE_RADIO_RDS_)
		si473x_rds_reset();
	#endif
		si473xCtrl->TunerAutoSeek = TRUE;
	}
	else
	{
		engine_hard_stop_t *stop = &hard->ctrl.stop;
		
		if(stop->stop_freq == 0xFFFF)
		{
			if(!si473x_reset_autoseek())return FALSE;
			
			si473xCtrl->TunerAutoSeek = TRUE;
			if(!tuner_si473x_status(stop->status))
			{
				si473xCtrl->TunerAutoSeek = FALSE;
				return FALSE;
			}
			si473xCtrl->TunerAutoSeek = FALSE;
			
			if(stop->status->freq < si473xCtrl->min_freq)
			{
				if(!tuner_si473x_search(si473xCtrl->min_freq, stop->status))return FALSE;
			}
			else if(stop->status->freq > si473xCtrl->max_freq)
			{
				if(!tuner_si473x_search(si473xCtrl->max_freq, stop->status))return FALSE;
			}
		}
		else
		{
			if(stop->stop_freq < si473xCtrl->min_freq)return FALSE;
			if(stop->stop_freq > si473xCtrl->max_freq)return FALSE;	
			
			if(!si473x_reset_autoseek())return FALSE;
			if(!tuner_si473x_search(stop->stop_freq, stop->status))return FALSE;
		}
	}
	
	return TRUE;
}

static uint8 tuner_si473x_setting(fm_setting_e type, void *set)
{
	if((Si473xOrder == NULL)||(si473xCtrl == NULL))return FALSE;
	if(!si473xCtrl->init_succeed)return FALSE;
	
	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				if(!si473x_set_mute(mute))return FALSE;
			}
			break;
			
		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(si473xCtrl->TunerBand == SI473x_TUNER_BAND_REC_FM)
				{
					if(!si473x_set_stereo(stereo))return FALSE;
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
				
				tuner_printf("rds enable:%d, timer:%d\n", rds_ctrl->enable, rds_ctrl->timer);
				
				if(rds_ctrl->enable)
				{
					if(si473xCtrl->TunerBand == SI473x_TUNER_BAND_REC_FM)
					{
						if((si473xCtrl->min_freq >= 8750) && (si473xCtrl->max_freq <= 10800))
						{
							if(!si473xCtrl->tuner_rds.enable)
							{
								int check = FALSE;
								if(!tuner_si473x_info(TUR_INFO_RDS, &check))return FALSE;
								if(check)
								{
									si473xCtrl->tuner_rds.factor = 255/(_RDS_QUALITY_TIME_/rds_ctrl->timer);
									si473xCtrl->tuner_rds.enable = TRUE;
									si473x_rds_reset();
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
					if(si473xCtrl->tuner_rds.enable)
					{
						si473xCtrl->tuner_rds.enable = FALSE;
						rds_ctrl->result = TRUE;
					}
					else return FALSE;
				}
			}
			break;

		case TUR_SET_RDS_RESET:
			{
				if(si473xCtrl->tuner_rds.enable)
				{
					si473x_rds_reset();
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_DECODE:
			{
				if(si473xCtrl->tuner_rds.enable)
				{
					si473x_rds_decode();
				}
				else return FALSE;
			}
			break;
			
		case TUR_SET_RDS_EON:
			{
				if(si473xCtrl->tuner_rds.enable)
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
				if(si473xCtrl->tuner_rds.enable)
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

static uint8 tuner_si473x_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_SI473x;
			}
			break;
			
		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI473x_TUNER_RSSI_FM_DEFAULT;
			}
			break;
			
		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI473x_TUNER_RSSI_LOC_FM;
			}
			break;

		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_si473x_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_si473x_deal_spot_tab;
			}
			break;
		
		case TUR_INFO_AM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI473x_TUNER_RSSI_AM_DEFAULT;
			}
			break;
			
		case TUR_INFO_AM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = SI473x_TUNER_RSSI_LOC_AM;
			}
			break;

		case TUR_INFO_AM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(am_si473x_deal_spot_tab)/sizeof(uint16);
			}
			break;
			
		case TUR_INFO_AM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)am_si473x_deal_spot_tab;
			}
			break;
			
		case TUR_INFO_AUTOSEEK:
			if(info != NULL)
			{
		    #if defined(DISABLE_473x_AUTE_SEEK)
		    	*(uint8 *)info = DISABLE;//
		    #else
		    	*(uint8 *)info = ENABLE; //DISABLE;//
		    #endif
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
				if((Si473xOrder == NULL)||(si473xCtrl == NULL))return FALSE;
				if(!si473xCtrl->init_succeed)return FALSE;
				if(!si473xCtrl->tuner_rds.enable)return FALSE;
				
				radio_rds_info_t *rds_info = (radio_rds_info_t *)info;
				rds_dec_info(rds_info);
			}
			break;
			
		case TUR_INFO_RDS_APP:
			if(info != NULL)
			{
				if((Si473xOrder == NULL)||(si473xCtrl == NULL))return FALSE;
				if(!si473xCtrl->init_succeed)return FALSE;
				if(!si473xCtrl->tuner_rds.enable)return FALSE;
				
				radio_rds_app_t *rds_app = (radio_rds_app_t *)info;
				engine_status_t status;
				if(!tuner_si473x_status(&status))return FALSE;
				rds_app->freq	= status.freq;
				rds_app->level	= status.signal;
				rds_app->stc	= status.stc;
				rds_app->found	= status.valid;
				rds_app->signal = si473xCtrl->tuner_rds.quality;
				rds_dec_app(rds_app);
			}
			break;
			
		case TUR_INFO_RDS_LOAD:
			if(info != NULL)
			{
				if((Si473xOrder == NULL)||(si473xCtrl == NULL))return FALSE;
				if(!si473xCtrl->init_succeed)return FALSE;
				if(!si473xCtrl->tuner_rds.enable)return FALSE;
				
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
void Si4730WokeOkDet(void)
{
	static u8 radioerrcnt=0;
	u16 buf0=0;
	SendSi473xOrder(SI473x_CMD_GET_FREQ, sizeof(Si473xOrder->buffer));
	buf0 = (Si473xOrder->buffer[2]*256);
	buf0 +=Si473xOrder->buffer[3];
	if(si473xCtrl->now_freq != buf0)
	{
		radioerrcnt ++;
		if(radioerrcnt>2)
		{
			radioerrcnt =0;
			os_taskq_post_msg(FM_TASK_NAME, 1, MSG_FM_ERR);
		}
	}
}
const tuner_drv_fun_t tuner_si473x_drv_fun =
{
	tuner_si473x_detect,
	tuner_si473x_init,
	tuner_si473x_exit,
	tuner_si473x_freq,
	tuner_si473x_status,
	tuner_si473x_search,
	tuner_si473x_hard,
	tuner_si473x_setting,
	tuner_si473x_info,
};

#endif

