#ifndef __PT7314_H__
#define __PT7314_H__


#define PT7314_I2C_ADDR					0x88
#define PT7314_I2C_RATE					40	//	40//100K

typedef enum{
	PT7314_REG_VOLUME = 0,
	PT7314_REG_LR,			//	��ǰ
	PT7314_REG_RR,			//	��ǰ
	PT7314_REG_LF,			//	���
	PT7314_REG_RF,			//	�Һ�
	PT7314_REG_SW,			//	ͨ��
	PT7314_REG_BASS,		//	����
	PT7314_REG_TREB,		//	����
	PT7314_REG_SIZE,
}pt7314_reg_map_e;


typedef struct{
/*����*/
	uint8 	mute_last;
#if defined(_SUPPORT_SUBWOOFER_)
	uint8	subwoofer_last;
#endif

/*״̬*/
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

