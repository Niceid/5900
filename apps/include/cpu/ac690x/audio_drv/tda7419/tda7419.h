#ifndef __TDA7419_H__
#define __TDA7419_H__


#define TDA7419_I2C_ADDR					0x88
#define TDA7419_I2C_RATE					40	//	40//100K


typedef enum{
	TDA7419_REG_MAIN_SOURCE = 0,//	��ͨ��	0
	TDA7419_REG_MAIN_LOUD,		//	LOUD	1
	TDA7419_REG_SOFT_MUTE,		//	���MUTE2
	TDA7419_REG_VOLUME,			//	����	3
	TDA7419_REG_TREBLE,			//	����	4
	TDA7419_REG_MIDDLE,			//	����	5
	TDA7419_REG_BASS,			//	����	6
	TDA7419_REG_SECOND_SOURCE,	//	�ڶ�ͨ��7
	TDA7419_REG_FREQ,			//	Ƶ������8
	TDA7419_REG_MIXING,			//	����	9
	TDA7419_REG_LF,				//	��ǰ	10
	TDA7419_REG_RF,				//	��ǰ	11
	TDA7419_REG_LR,				//	���	12
	TDA7419_REG_RR,				//	�Һ�	13
	TDA7419_REG_MIXING_LEVEL,	//	ͨ��	14
	TDA7419_REG_SUBWOOFER,		//	�ص���	15
	TDA7419_REG_SPECTRUM,		//	Ƶ�׷���16
	TDA7419_REG_TESTING,		//	����	17
	TDA7419_REG_SIZE,			//			18
}tda7419_reg_map_e;

enum{
	AUDIO_LOUDNESS_FLAT = 0,
	AUDIO_LOUDNESS_CEN_FREQ_400,
	AUDIO_LOUDNESS_CEN_FREQ_800,
	AUDIO_LOUDNESS_CEN_FREQ_2400,
};

enum{
	AUDIO_SUBWOOFER_FLAT = 0,
	AUDIO_SUBWOOFER_CUT_FREQ_80,
	AUDIO_SUBWOOFER_CUT_FREQ_120,
	AUDIO_SUBWOOFER_CUT_FREQ_160,
};

enum{
	TREBLE_CENTER_FREQ_10KHZ,
	TREBLE_CENTER_FREQ_12P5KHZ,
	TREBLE_CENTER_FREQ_15KHZ,
	TREBLE_CENTER_FREQ_17P5KHZ
};

#define AUDIO_SUBWOOFER_VOLUME_MIN			(0)
#define AUDIO_SUBWOOFER_VOLUME_MAX			(40)

#if defined(_ENABLE_LOUD_FUNTION_)
#define AUDIO_LOUDNESS_CENTER_FREQ_DEFAULT	AUDIO_LOUDNESS_CEN_FREQ_400
#else
#define AUDIO_LOUDNESS_CENTER_FREQ_DEFAULT	AUDIO_LOUDNESS_CEN_FREQ_2400
#endif

#define AUDIO_LOUDNESS_GAIN_DEFAULT			0

#define AUDIO_SUBWOOFER_CUT_FREQ_DEFAULT	AUDIO_SUBWOOFER_CUT_FREQ_80
#define AUDIO_SUBWOOFER_GAIN_DEFAULT		(40)


#define AUDIO_GAIN_DB(x)		((uint8)(((x)>=0)?((x)&0xF):(-(x)+16)))

#define TREBLE_CENTER_FREQ		(TREBLE_CENTER_FREQ_17P5KHZ << 5)

typedef struct{
/*����*/
	uint8 	mute_last;
	uint8	subwoofer_last;
/*״̬*/
	uint8	mute;
	uint8	channel;
	uint8	gate;
	uint8 	volume;
	int8 	bass;
	int8 	middle;
	int8 	treble;
	int8 	lr;
	int8 	fr;
	uint8 	eq;
	struct{
		uint8 enable:1;
		uint8 centerfreq:2;
		uint8 gain:4;
	}loudness;
	struct{
		uint8 enable:1;
		uint8 cutfreq:2;
		uint8 gain;
	}subwoofer;
}tda7419_audio_t;

#endif //


