#ifndef __PT2313A_H__
#define __PT2313A_H__

#if defined(_SUPPORT_TWO_PT2313_)

#define PT2313_I2C_ADDR1					0x88
#define PT2313_I2C_RATE1					80//	70//100K

typedef enum{
	PT2313A_REG_VOLUME = 0,
	PT2313A_REG_LR,			//	��ǰ
	PT2313A_REG_RR,			//	��ǰ
	PT2313A_REG_LF,			//	���
	PT2313A_REG_RF,			//	�Һ�
	PT2313A_REG_SW,			//	ͨ��
	PT2313A_REG_BASS,		//	����
	PT2313A_REG_TREB,		//	����
	PT2313A_REG_SIZE,
}pt2313A_reg_map_e;


typedef struct{
/*����*/
	uint8 	mute_last;
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
}pt2313A_audio_t;

#endif

#endif //__PT2313A_H__

