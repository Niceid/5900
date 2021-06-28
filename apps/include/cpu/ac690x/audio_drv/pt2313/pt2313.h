#ifndef __PT2313_H__
#define __PT2313_H__


#define PT2313_I2C_ADDR					0x88
#define PT2313_I2C_RATE					80//	70//100K

typedef enum{
	PT2313_REG_VOLUME = 0,
	PT2313_REG_LR,			//	��ǰ
	PT2313_REG_RR,			//	��ǰ
	PT2313_REG_LF,			//	���
	PT2313_REG_RF,			//	�Һ�
	PT2313_REG_SW,			//	ͨ��
	PT2313_REG_BASS,		//	����
	PT2313_REG_TREB,		//	����
	PT2313_REG_SIZE,
}pt2313_reg_map_e;

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
#if defined(_SPECIAL_2313_CONTROL_)
	int8  reg[8];
#endif
#if defined(_SPECIAL_TL2000_SET_)
	int8 	A_volume;
	int8 	B_volume;
#endif
}pt2313_audio_t;

#ifdef	TD_ADD_NEW_APP
extern void pt2313_reset(void);
#endif

#endif //__PT2313_H__


