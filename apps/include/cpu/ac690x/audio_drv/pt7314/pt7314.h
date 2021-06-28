#ifndef __PT7314_H__
#define __PT7314_H__


#define PT7314_I2C_ADDR					0x88
#define PT7314_I2C_RATE					40	//	40//100K

typedef enum{
	PT7314_REG_VOLUME = 0,
	PT7314_REG_LR,			//	左前
	PT7314_REG_RR,			//	右前
	PT7314_REG_LF,			//	左后
	PT7314_REG_RF,			//	右后
	PT7314_REG_SW,			//	通道
	PT7314_REG_BASS,		//	低音
	PT7314_REG_TREB,		//	高音
	PT7314_REG_SIZE,
}pt7314_reg_map_e;


typedef struct{
/*控制*/
	uint8 	mute_last;
#if defined(_SUPPORT_SUBWOOFER_)
	uint8	subwoofer_last;
#endif

/*状态*/
	uint8	mute;
	uint8	channel;
	uint8	loud;
	uint8	gate;
	uint8 	volume;
	int8 	bass;
	int8 	treble;
	int8 	lr;
	int8 	fr;
	uint8 	eq;
#if defined(_SUPPORT_SUBWOOFER_)
	uint8	subwoofer;
#endif
}pt7314_audio_t;

#endif //__PT7314_H__

