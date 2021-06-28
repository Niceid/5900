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

#define AUTO_SEEK_MODE_DOWN				(0<<0)	//	向下扫描
#define AUTO_SEEK_MODE_UP				(1<<0)	//	向上扫描

#define AUTO_SEEK_MODE_CURRENT			(0<<1)	//	从当前点开始扫描
#define AUTO_SEEK_MODE_MIN				(1<<1)	//	从最小频点开始扫描
#define AUTO_SEEK_MODE_MAX				(2<<1)	//	从最大频点开始扫描

#define AUTO_SEEK_MODE_STATION			(0<<3)	//	扫描到电台后停止
#define AUTO_SEEK_MODE_ROUND			(1<<3)	//	扫描一圈停止

#define AUTO_SEEK_MODE_DX				(0<<4)	//	远程门限
#define AUTO_SEEK_MODE_LOC				(1<<4)	//	近程门限

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
	radio_band_e	band;					//当前波段
    uint16          freq;           		//当前频率
    uint16			min_freq;				//最小频率
	uint16			max_freq;				//最大频率
}engine_init_t;

typedef struct
{
    uint16      freq;						//当前频率
    uint8       signal;						//当前频点信号强度
    uint8		valid;						//有效电台
    uint8       stereo;						// 1、立体声0 、单声道
    uint8      	band;						//当前波段
    uint8 		stc;						//硬件seek 是否完成标志，主要用于硬件seek 需要
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
	/* 设置返回接口*/
	MSG_FMENGINE_RETURN,
	/* 设置频点*/
    MSG_FMENGINE_SETFREQ,
    /* 静音或解除静音*/
    MSG_FMENGINE_SETMUTE,
    /* 启动软件搜台*/
    MSG_FMENGINE_SET_SOFTSEEK,
    /* 获取当前状态信息*/
    MSG_FMENGINE_GET_STATUS,
    /* 获取立体声状态信息*/
    MSG_FMENGINE_GET_STEREO,
    /* 获取单FM 状态信息*/
    MSG_FMENGINE_GET_FM_ONLY,
    /* FM 驱动IC  初始化配置*/
	MSG_FMENGINE_INIT,
	/* FM 驱动IC  退出*/
	MSG_FMENGINE_EXIT,
	/*	设置立体声解调*/
	MSG_FMENGINE_SET_STEREO,
	/* 启动软件搜台*/
	MSG_FMENGINE_AUTO_SEEK,
	/* 退出自动搜台*/
	MSG_FMENGINE_AUTO_BREAK,
	/* 获取当前模组状态*/
	MSG_FMENGINE_GET_MODUAL_STATUS,
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
	/* 设置音量*/
	MSG_FMENGINE_VOLUME = 0x2100,

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
	/* 获取RDS 开关状态*/
    MSG_FMENGINE_RDS_STATE = 0x2800,
	/*RDS 后台启动*/
	MSG_FMENGINE_RDS_BACK_START,
	MSG_FMENGINE_RDS_BACK_RUN,
	/*RDS 开关*/
	MSG_FMENGINE_RDS_ENABLE,
	/*RDS 前后台设置*/
	MSG_FMENGINE_RDS_FRONT,
	/*RDS 忙检测*/
	MSG_FMENGINE_RDS_BUSY,
	/*RDS 快速检测*/
	MSG_FMENGINE_RDS_FAST,
	/*RDS 忙信息保存*/
	MSG_FMENGINE_RDS_SAVE,
	/*RDS 忙信息加载*/
	MSG_FMENGINE_RDS_LOAD,
	
	/*RDS 获取信息*/
	MSG_FMENGINE_RDS_INFO = 0x3000,
	/*RDS TA 使能控制*/
	MSG_FMENGINE_RDS_TA_DSABLE = 0x3100,
	/*RDS 定时运行*/
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

