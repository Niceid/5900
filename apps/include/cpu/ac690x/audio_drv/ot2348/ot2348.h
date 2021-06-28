#ifndef __OT2348_H__
#define __OT2348_H__


#define OT2348_I2C_ADDR					0x88
#define OT2348_I2C_RATE					40	//	40//100K

typedef enum{
	OT2348_REG_ADR,			//	�׵�ַ
	OT2348_REG_SW,			//	ͨ��
	OT2348_REG_SUB,			//	�ص���
	OT2348_REG_VOLUME,		//   ����
	OT2348_REG_BASS,		//	����
	OT2348_REG_TREB,		//	����
	OT2348_REG_MIX,			//	����
	OT2348_REG_LF,			//	���
	OT2348_REG_RF,			//	�Һ�
	OT2348_REG_LR,			//	��ǰ
	OT2348_REG_RR,			//	��ǰ
	OT2348_REG_SUBTEN,		//	�ص������� 
	OT2348_REG_2SW,			//	ͨ��ѡ��2���� 
	OT2348_REG_SIZE,
}ot2348_reg_map_e;


enum{
	CUT_OFF_80 = 1,
	CUT_OFF_120,
	CUT_OFF_160,
};


typedef struct{
/*����*/
	uint8 	mute_last;
	uint8	subwoofer_last;
	
/*״̬*/
	uint8	channel;
	uint8	subwoofer;
	uint8 	volume;
	int8 	bass;
	int8 	treble;
	uint8   mix;
	int8 	fr;
	int8 	lr;
	uint8 	subten;
	uint8	channel2;
	
	uint8	mute;
	uint8	loud;
	uint8	sub_ctrl;
	uint8	gate;
	uint8 	eq;
#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	uint8 	sub_volume;
#endif
}ot2348_audio_t;


#endif //__OT2348_H__


