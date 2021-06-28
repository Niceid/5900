#ifndef __FM_ENGINE_H__
#define __FM_ENGINE_H__


#define FM_ENGINE_DEBUG
#ifdef FM_ENGINE_DEBUG
#define fm_engine_printf 		ew_printf
#define fm_engine_puts 			ew_puts
#else
#define fm_engine_printf(...)
#define fm_engine_puts(...)
#endif


typedef enum
{
	RADIO_BAND_NULL = 0,
	RADIO_BAND_FM,
	RADIO_BAND_AM,
#if defined(_ENABLE_RADIO_WB_)
	RADIO_BAND_WB,
#endif
}radio_band_e;


#define AUTO_SEEK_MASK_DIR				(_B0_)
#define AUTO_SEEK_MASK_START			(_B2_|_B1_)
#define AUTO_SEEK_MASK_SEEK				(_B3_)
#define AUTO_SEEK_MASK_THRESHOLD		(_B4_)

#define AUTO_SEEK_MODE_DOWN				(0<<0)	//	����ɨ��
#define AUTO_SEEK_MODE_UP				(1<<0)	//	����ɨ��

#define AUTO_SEEK_MODE_CURRENT			(0<<1)	//	�ӵ�ǰ�㿪ʼɨ��
#define AUTO_SEEK_MODE_MIN				(1<<1)	//	����СƵ�㿪ʼɨ��
#define AUTO_SEEK_MODE_MAX				(2<<1)	//	�����Ƶ�㿪ʼɨ��

#define AUTO_SEEK_MODE_STATION			(0<<3)	//	ɨ�赽��̨��ֹͣ
#define AUTO_SEEK_MODE_ROUND			(1<<3)	//	ɨ��һȦֹͣ

#define AUTO_SEEK_MODE_DX				(0<<4)	//	Զ������
#define AUTO_SEEK_MODE_LOC				(1<<4)	//	��������

typedef enum
{
    AUTO_REPLY_REFUR 	= _B0_,
	AUTO_REPLY_STATION 	= _B1_,
	AUTO_REPLY_STOP 	= _B2_,
	AUTO_REPLY_RESET	= _B3_,
	
	AUTO_REPLY_SEARCH	= _B6_,
	AUTO_REPLY_RDS		= _B7_,
}auto_reply_e;


typedef struct
{
	radio_band_e	band;					//��ǰ����
    uint16          freq;           		//��ǰƵ��
    uint16			min_freq;				//��СƵ��
	uint16			max_freq;				//���Ƶ��
}engine_init_t;

typedef struct
{
    uint16      freq;						//��ǰƵ��
    uint8       signal;						//��ǰƵ���ź�ǿ��
    uint8		valid;						//��Ч��̨
    uint8       stereo;						// 1��������0 ��������
    uint8      	band;						//��ǰ����
    uint8 		stc;						//Ӳ��seek �Ƿ���ɱ�־����Ҫ����Ӳ��seek ��Ҫ
}engine_status_t;

typedef struct{
	uint16		start_freq;
	uint16 		min_freq;
	uint16 		max_freq;
	uint8		step;
	uint8 		dir;
}engine_hard_start_t;

typedef struct{
	uint16				stop_freq;
	engine_status_t 	*status;
}engine_hard_stop_t;

typedef struct{
	uint8	enable;
	union{
		engine_hard_start_t	start;
		engine_hard_stop_t	stop;
	}ctrl;
}engine_hard_t;

typedef struct
{
	uint16 	min_freq;
    uint16 	max_freq;
    uint8	step;
	uint8	mode;
}engine_auto_seek_t;


enum{
	MSG_FMENGINE_TIMER = 0x2000,
	/* ���÷��ؽӿ�*/
	MSG_FMENGINE_RETURN,
	/* ����Ƶ��*/
    MSG_FMENGINE_SETFREQ,
    /* ������������*/
    MSG_FMENGINE_SETMUTE,
    /* ���������̨*/
    MSG_FMENGINE_SET_SOFTSEEK,
    /* ��ȡ��ǰ״̬��Ϣ*/
    MSG_FMENGINE_GET_STATUS,
    /* ��ȡ������״̬��Ϣ*/
    MSG_FMENGINE_GET_STEREO,
    /* ��ȡ��FM ״̬��Ϣ*/
    MSG_FMENGINE_GET_FM_ONLY,
    /* FM ����IC  ��ʼ������*/
	MSG_FMENGINE_INIT,
	/* FM ����IC  �˳�*/
	MSG_FMENGINE_EXIT,
	/*	�������������*/
	MSG_FMENGINE_SET_STEREO,
	/* ���������̨*/
	MSG_FMENGINE_AUTO_SEEK,
	/* �˳��Զ���̨*/
	MSG_FMENGINE_AUTO_BREAK,
	/* ��ȡ��ǰģ��״̬*/
	MSG_FMENGINE_GET_MODUAL_STATUS,
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
	/* ��������*/
	MSG_FMENGINE_VOLUME = 0x2100,

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
	/* ��ȡRDS ����״̬*/
    MSG_FMENGINE_RDS_STATE = 0x2800,
	/*RDS ��̨����*/
	MSG_FMENGINE_RDS_BACK_START,
	MSG_FMENGINE_RDS_BACK_RUN,
	/*RDS ����*/
	MSG_FMENGINE_RDS_ENABLE,
	/*RDS ǰ��̨����*/
	MSG_FMENGINE_RDS_FRONT,
	/*RDS æ���*/
	MSG_FMENGINE_RDS_BUSY,
	/*RDS ���ټ��*/
	MSG_FMENGINE_RDS_FAST,
	/*RDS æ��Ϣ����*/
	MSG_FMENGINE_RDS_SAVE,
	/*RDS æ��Ϣ����*/
	MSG_FMENGINE_RDS_LOAD,
	
	/*RDS ��ȡ��Ϣ*/
	MSG_FMENGINE_RDS_INFO = 0x3000,
	/*RDS TA ʹ�ܿ���*/
	MSG_FMENGINE_RDS_TA_DSABLE = 0x3100,
	/*RDS ��ʱ����*/
	MSG_FMENGINE_RDS_POLLING,
	MSG_FMENGINE_RDS_POLLING_2,
};


extern void fm_engine_init(void);
extern void fm_engine_exit(void);
extern uint8 fm_engine_msg(int, void *, void *);
extern uint8 fm_inside_check(void);

#if defined(_ENABLE_RADIO_RDS_)
extern void radio_rds_init(void);
extern void radio_rds_exit(void);
#endif


#endif /* __FM_ENGINE_H__ */

