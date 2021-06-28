#ifndef __RDS_COM_H__
#define __RDS_COM_H__

#if defined(_ENABLE_RADIO_RDS_)

#include "rds_radio.h"

/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/

typedef enum{
	RDS_APP_ID_IDLE = 0,
	RDS_APP_ID_AF,
	RDS_APP_ID_TA,
	RDS_APP_ID_PI,
	RDS_APP_ID_PTY,
	RDS_APP_ID_EON,
	RDS_APP_ID_TRAFFIC,
	RDS_APP_ID_ALARM,
}radio_rds_id_e;

typedef enum{
 	RDS_APP_MODE_INIT = 0,
	RDS_APP_MODE_RUN,
	RDS_APP_MODE_EXIT,
}radio_rds_mode_e;

typedef enum{
	RDS_APP_AF_STATE_INIT = 0,
	RDS_APP_AF_STATE_FREQ,
	RDS_APP_AF_STATE_SEEK,
	RDS_APP_AF_STATE_DETECT,
	RDS_APP_AF_STATE_FOUND,
	RDS_APP_AF_STATE_NO_FOUND,
	RDS_APP_AF_STATE_END,
}radio_rds_af_state_e;

typedef enum{
	RDS_APP_TA_STATE_INIT = 0,
	RDS_APP_TA_STATE_FREQ,
	RDS_APP_TA_STATE_SEEK,
	RDS_APP_TA_STATE_DETECT,
	RDS_APP_TA_STATE_FOUND,
	RDS_APP_TA_STATE_NO_FOUND,
	RDS_APP_TA_STATE_CONTROL,
	RDS_APP_TA_STATE_END,
}radio_rds_ta_state_e;

typedef enum{
	RDS_APP_PI_STATE_INIT = 0,
	RDS_APP_PI_STATE_FREQ,
	RDS_APP_PI_STATE_SEEK,
	RDS_APP_PI_STATE_DETECT,
	RDS_APP_PI_STATE_FOUND,
	RDS_APP_PI_STATE_NO_FOUND,
	RDS_APP_PI_STATE_END,
}radio_rds_pi_state_e;

typedef enum{
	RDS_APP_PTY_STATE_INIT = 0,
	RDS_APP_PTY_STATE_FREQ,
	RDS_APP_PTY_STATE_SEEK,
	RDS_APP_PTY_STATE_DETECT,
	RDS_APP_PTY_STATE_FOUND,
	RDS_APP_PTY_STATE_NO_FOUND,
	RDS_APP_PTY_STATE_END
}radio_rds_pty_state_e;

typedef enum{
	RDS_APP_EON_STATE_INIT = 0,
	RDS_APP_EON_STATE_FREQ,
	RDS_APP_EON_STATE_SEEK,
	RDS_APP_EON_STATE_DETECT,
	RDS_APP_EON_STATE_FOUND,
	RDS_APP_EON_STATE_NO_FOUND,
	RDS_APP_EON_STATE_ENTER,
	RDS_APP_EON_STATE_IDLE,
	RDS_APP_EON_STATE_EXIT,
	RDS_APP_EON_STATE_END,
}radio_rds_eon_state_e;

typedef enum{
	RDS_APP_EON_MODE_AF = 0,
	RDS_APP_EON_MODE_MAPPED,
}radio_rds_eon_mode_e;

typedef enum{
	RDS_APP_TRAFFIC_STATE_INIT = 0,
	RDS_APP_TRAFFIC_STATE_ENTER,
	RDS_APP_TRAFFIC_STATE_FREQ,
	RDS_APP_TRAFFIC_STATE_SEEK,
	RDS_APP_TRAFFIC_STATE_DETECT,
	RDS_APP_TRAFFIC_STATE_FOUND,
	RDS_APP_TRAFFIC_STATE_NO_FOUND,
	RDS_APP_TRAFFIC_STATE_RESULT,
	RDS_APP_TRAFFIC_STATE_IDLE,
	RDS_APP_TRAFFIC_STATE_EXIT,
	RDS_APP_TRAFFIC_STATE_END,
}radio_rds_traffic_state_e;

typedef enum{
	RDS_APP_ALARM_STATE_INIT = 0,
	RDS_APP_ALARM_STATE_END,
}radio_rds_alarm_state_e;

typedef enum{
	RDS_ALARM_NULL = 0,
	RDS_ALARM_NO_TP_TA,
	RDS_ALARM_EON,
	RDS_ALARM_TRAFFIC,
}radio_rds_alarm_e;

typedef enum{
	RDS_ICON_CLOSE = 0,
	RDS_ICON_OPEN,
	RDS_ICON_FLASH,
}rds_icon_ctrl_e;

typedef enum{
	RDS_ICON_AF = 0,
	RDS_ICON_TA,
	RDS_ICON_TP,
	RDS_ICON_PTY,
	RDS_ICON_REG,
	RDS_ICON_EON,
	RDS_ICON_MAX,
}rds_icon_id_e;

typedef enum{
	RDS_PTY_SEARCH_OFF = 0,
	RDS_PTY_SEARCH_MUSIC,
	RDS_PTY_SEARCH_SPEECH,
}rds_typ_search_e;

enum{
	RDS_PTY_NONE = 0,				//0		(无)--------无PTY类型
	RDS_PTY_NEWS,					//1     (语言组)----新闻
	RDS_PTY_AFFAIRS,				//2     (语言组)----时事, 新闻
	RDS_PTY_INFO,					//3     (语言组)----通知
	RDS_PTY_SPORT,					//4     (语言组)----体育
	RDS_PTY_EDUCATE,				//5		(语言组)----教育
	RDS_PTY_DRAMA,					//6		(语言组)----戏剧
	RDS_PTY_CULTURE,				//7		(语言组)----文化
	RDS_PTY_SCIENCE,				//8		(语言组)----科学
	RDS_PTY_VARIED,					//9		(语言组)----其它语言类型
	RDS_PTY_POP_M,					//10	(音乐组)----流行音乐
	RDS_PTY_ROCK_M,					//11	(音乐组)----摇滚音乐
	RDS_PTY_EASY_M,					//12	(音乐组)----舒缓音乐
	RDS_PTY_LIGTH_M,				//13	(音乐组)----轻音乐
	RDS_PTY_CLASSICS,				//14	(音乐组)----古典音乐
	RDS_PTY_OTHER_M,				//15	(音乐组)----其它音乐
	RDS_PTY_WEATHER,				//16	(语言组)----天气
	RDS_PTY_FINANCE,				//17	(语言组)----经济
	RDS_PTY_CHILDREN,				//18	(语言组)----儿童乐园
	RDS_PTY_SOCIAL,					//19	(语言组)----社会
	RDS_PTY_RELIGION,				//20	(语言组)----宗教
	RDS_PTY_PHONE_IN,				//21	(特殊组)----电话输入
	RDS_PTY_TRAVEL,					//22	(语言组)----旅行
	RDS_PTY_LEISURE,				//23	(语言组)----休闲
	RDS_PTY_JAZZ,					//24	(音乐组)----爵士乐
	RDS_PTY_COUNTRY,				//25	(音乐组)----乡村音乐
	RDS_PTY_NATION_M,				//26	(音乐组)----民族音乐
	RDS_PTY_OLDIES,					//27	(音乐组)----旧歌翻唱
	RDS_PTY_FOLK_M,					//28	(音乐组)----民间音乐
	RDS_PTY_DOCUMENT,				//29	(语言组)----记录片
	RDS_PTY_TEST,					//30	(特殊组)----测试
	RDS_PTY_ALARM,					//31	(特殊组)----警报
};

enum{
	RDS_PTY_USA_NONE = 0,		//0		(无)--------无PTY类型
	RDS_PTY_USA_NEWS,           //1
	RDS_PTY_USA_INFORM,  		//2
	RDS_PTY_USA_SPORTS,			//3
	RDS_PTY_USA_TALK,			//4
	RDS_PTY_USA_ROCK,			//5
	RDS_PTY_USA_CLS_ROCK,		//6
	RDS_PTY_USA_ADLT_HIT,		//7
	RDS_PTY_USA_SOFT_RCK,		//8
	RDS_PTY_USA_TOP_40,			//9
	RDS_PTY_USA_COUNTRY,		//10
	RDS_PTY_USA_OLDIES,			//11
	RDS_PTY_USA_SOFT,			//12
	RDS_PTY_USA_NOSTALGIA,	    //13
	RDS_PTY_USA_JAZZ,			//14
	RDS_PTY_USA_CLASSICL,		//15
	RDS_PTY_USA_R_B,			//16
	RDS_PTY_USA_SOFT_R_B,		//17
	RDS_PTY_USA_LANGUAGE,		//18
	RDS_PTY_USA_REL_MUSC,		//19
	RDS_PTY_USA_REL_TALK,		//20
	RDS_PTY_USA_PERSNLTY,		//21
	RDS_PTY_USA_PUBLIC,			//22
	RDS_PTY_USA_COLLEGE,		//23
	RDS_PTY_USA_UNDEF1,			//24
	RDS_PTY_USA_UNDEF2,			//25
	RDS_PTY_USA_UNDEF3,			//26
	RDS_PTY_USA_UNDEF4,			//27
	RDS_PTY_USA_UNDEF5,			//28
	RDS_PTY_USA_WEATHER,		//29
	RDS_PTY_USA_TEST,			//30
	RDS_PTY_USA_ALERT,			//31
};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_RDS_TIME_)
#define RDS_SIGNAL_DETECT_TIME						(1200)	//	RDS 等待需要信号时间
#define RDS_TRAFFIC_DETECT_TIME						(300)	//	RDS 在交通信号的情况下，控制信号返回等待时长
#define RDS_CHECK_FOUND_COUNT						(5)		//	RDS 检测收音驱动连续次数没有发现电台
#define RDS_DECODE_WAITING_TIME						(300)	//	最长RDS 解码等待时间
#define RDS_INTERVAL_TRAFFIC_MIN_TIME				(900)	//	每次TRAFFIC 信号来的最小间隔
#define RDS_INTERVAL_EON_MIN_TIME					(900)	//	每次EON 信号来的最小间隔
#define RDS_INTERVAL_PAUSE_MIN_TIME					(900)	//	每次PAUSE 控制的最小间隔
#else
#define RDS_SIGNAL_DETECT_TIME						(4000)	//	RDS 等待需要信号时间
#define RDS_TRAFFIC_DETECT_TIME						(1000)	//	RDS 在交通信号的情况下，控制信号返回等待时长
#define RDS_CHECK_FOUND_COUNT						(5)		//	RDS 检测收音驱动连续次数没有发现电台
#define RDS_DECODE_WAITING_TIME						(1000)	//	最长RDS 解码等待时间
#define RDS_INTERVAL_TRAFFIC_MIN_TIME				(3000)	//	每次TRAFFIC 信号来的最小间隔
#define RDS_INTERVAL_EON_MIN_TIME					(3000)	//	每次EON 信号来的最小间隔
#define RDS_INTERVAL_PAUSE_MIN_TIME					(3000)	//	每次PAUSE 控制的最小间隔
#endif


//#define RDS_EON_CHECK_CURRENT_STATUS						//	EON 检测当前电台状态


enum{
	PTY_SEARCH_CMD_NULL = 0,
	PTY_SEARCH_CMD_ON,
	PTY_SEARCH_CMD_OFF,
	PTY_SEARCH_CMD_FOUND,
	PTY_SEARCH_CMD_NO_FOUND,

	PTY_SEARCH_RUN_ON = 0x80,
	PTY_SEARCH_RUN_RESULT,
};

typedef struct{
	uint8	search;			//	PTY 搜索类型
	uint8	cmd;			//	PTY 搜索命令
	uint8	enable;			//	PTY 功能使能
	uint16	delay;			//	PTY 搜索延时
	const uint8 *buffer;	//	PTY 搜索数据缓冲
	uint8 	length;			//	PTY 搜索长度缓冲
	uint8	up_down;		//	PTY 搜索方向
	uint8	re_start;		//	PTY 搜索重新开始
}rds_pty_search_t;


typedef struct{
/****************************************************************************************************/
//	应用控制(打开、关闭RDS 及频率转换时清除数据)
	uint16				time;						//	等待时间计数
	uint16				traffic_interval;			//	下次交通信号最小间隔时间
	uint16				eon_interval;				//	下次EON信号最小间隔时间
	uint16				pause_interval;				//	暂停后最小判断时间
	uint8				af_count;					//	AF跳转等待时间计数

	radio_rds_alarm_e 	alarm;						//	警告类型

	uint8				init;						//	进入初始化成功标志
	uint8				exit;						//	强制我退出标志
	uint8				found;						//	发现有效电台标志

	uint8				pause_ta;					//	TA功能暂停
	uint8  				pause_eon;					//	EON功能暂停
	uint8				pause_disp;					//	交通信息暂停显示

	uint8				backup_disable;				//	数据备份禁止

	radio_rds_psn_t		psn;						//	当前RDS PSN
	radio_rds_date_t 	date;						//	当前RDS 时间
}rds_app_ap_ctrl_t;

typedef struct{
	uint8	channel;								//	通道转换保存
	uint8 	ap_vol;									//	AP 音量保存
	uint8 	icon[RDS_ICON_MAX];
	uint8 	prev_icon[RDS_ICON_MAX];
}rds_app_sys_ctrl_t;

typedef uint8 (*rds_ext_busy)(void);
typedef void (*rds_ext_func)(int, void *);

typedef struct{
	uint8			fast_det;						//	信号条件快速侦测
	uint8			ta_disable;						//	TA 信号检测不使能
	rds_ext_busy 	busy;							//	检查前台应用是否禁止
	rds_ext_func 	func;							//	外部接口
}rds_app_ext_t;

typedef struct{
	uint16			pi;
	radio_rds_af_t 	af;
	struct{
		uint8 		length;
		uint8 		freq[_RDS_EON_MAPPED_MAX_];
	}mapped;
}rds_app_eon_backup_t;

typedef struct{
	uint16				pi;
	uint8				ms;
	uint8  				pty;
	radio_rds_psn_t		psn;
	radio_rds_af_t 		af;
}rds_app_backup_t;

typedef struct{
	radio_rds_af_state_e 		State;
	uint16						Time;

	uint8						FreqCount;
	uint8  						Index;
	uint8 						OrigFreq;
	uint8 						NowFreq;
	uint8 						NowLevel;
	uint8 						FristFreq;
	uint8						HighLevel;
	uint8						RdsFristFreq;
	uint8						RdsHighLevel;
	uint8						PiFristFreq;
	uint8						PiHighLevel;
}rds_app_af_t;

typedef struct{
	radio_rds_ta_state_e 		State;
	uint16						Time;

	uint8						FreqCount;
	uint8  						OrigFreq;
	uint8 						NowFreq;
	uint8 						NowLevel;
	uint8 						FristFreq;
	uint8						HighLevel;
	uint8 						PiFristFreq;
	uint8						PiHighLevel;
	uint8						FirstSeek;
}rds_app_ta_t;

typedef struct{
	radio_rds_pi_state_e 		State;
	uint16						Time;

	uint8						FreqCount;
	uint8  						OrigFreq;
	uint8 						NowFreq;
	uint8 						NowLevel;
	uint8 						FristFreq;
	uint8						HighLevel;
	uint8						PtyFristFreq;
	uint8						PtyHighLevel;
	uint8						MsFristFreq;
	uint8						MsHighLevel;
	uint8						RdsFristFreq;
	uint8						RdsHighLevel;
}rds_app_pi_t;

typedef struct{
	radio_rds_pty_state_e 		State;
	uint16						Time;

	uint8						FreqCount;
	uint8  						OrigFreq;
	uint8 						NowFreq;
	uint8 						NowLevel;
	uint8 						FristFreq;
	uint8						HighLevel;
	uint8						AlarmFound;
	const uint8 				*CheckBuffer;
	uint8 						CheckLength;
	uint8						UpDown;
	uint8						ExitFlag;
}rds_app_pty_t;

typedef struct{
	radio_rds_eon_state_e 		State;
	uint16						Time;
	uint8						Found;
	uint16						Signal;

	uint8 						Mode;
	uint8  						Index;
	uint8						Change;
	uint8  						OrigFreq;
	uint8 						NowFreq;
	uint8 						NowLevel;
	uint8 						FristFreq;
	uint8						HighLevel;
	uint8						PiFristFreq;
	uint8						PiHighLevel;
	rds_app_eon_backup_t		BackUp;
}rds_app_eon_t;

typedef struct{
	radio_rds_traffic_state_e 	State;
	uint16						Time;
	uint8						Found;
	uint16						Signal;

	uint8  						Index;
	uint8  						OrigFreq;
	uint8						AfOrigFreq;
	uint8 						NowFreq;
	uint8 						NowLevel;
	uint8 						FristFreq;
	uint8						HighLevel;
	uint8						PiFristFreq;
	uint8						PiHighLevel;
}rds_app_traffic_t;

typedef struct{
	radio_rds_alarm_state_e 	State;
	uint16						Time;
	uint8						Flash;
}rds_app_alarm_t;

typedef struct{
	uint8 enable;
	uint16 count;
	radio_rds_id_e 			id;
	radio_rds_id_e 			next_id;
	radio_rds_mode_e 		mode;

	radio_rds_app_t 		dec;
	rds_app_backup_t 		backup;
	rds_app_ap_ctrl_t		ctrl;
	rds_app_sys_ctrl_t		sys_ctrl;
	rds_pty_search_t 		pty_search;
	rds_app_ext_t			ext;

	union{
		rds_app_af_t 		af;
		rds_app_ta_t 		ta;
		rds_app_pi_t		pi;
		rds_app_pty_t		pty;
		rds_app_eon_t		eon;
		rds_app_traffic_t	traffic;
		rds_app_alarm_t		alarm;
	}type;
}rds_app_t;

extern rds_app_t *rds_app;

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

#define rds_ta_ok()							(rds_app->dec.tp && rds_app->dec.ta)
#define rds_eon_ok()						rds_ctrl_eon_check()
#define rds_eon_ta_ok()						rds_ta_ok()

#define RDS_APP_AF							(rds_app->type.af)
#define RDS_APP_TA							(rds_app->type.ta)
#define RDS_APP_PI							(rds_app->type.pi)
#define RDS_APP_PTY							(rds_app->type.pty)
#define RDS_APP_EON							(rds_app->type.eon)
#define RDS_APP_TRAFFIC						(rds_app->type.traffic)
#define RDS_APP_ALARM						(rds_app->type.alarm)

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
void rds_app_polling(void);
void rds_app_reset(void);
uint8 rds_cmd_tuner_rds_enable(uint8, uint16);
uint8 rds_cmd_tuner_freq(uint16);
uint8 rds_cmd_tuner_app(radio_rds_app_t *);
uint8 rds_cmd_tuner_info(radio_rds_info_t *);
uint8 rds_cmd_reset(void);
uint8 rds_cmd_eon_enable(void);
uint8 rds_cmd_eon_disable(void);

void rds_cmd_engine_freq(uint16);
void rds_cmd_engine_mute(uint8);
void rds_cmd_engine_traffic(uint8);

enum{
	RDS_TXT_NULL = 0,
	RDS_TXT_FLASH,
	RDS_TXT_TRAFFIC,
	RDS_TXT_EON_TA,
	RDS_TXT_TA_SEEK,
	RDS_TXT_PI_SEEK,
	RDS_TXT_PTY_SEEK,
	RDS_TXT_FOUND,
	RDS_TXT_NO_FOUND,
	RDS_TXT_NO_TP_TA,
	RDS_TXT_MAX,
};
void rds_cmd_engine_txt(uint8);

void rds_ctrl_icon_sync(void);
void rds_ctrl_psn_sync(void);
void rds_ctrl_data_sync(void);

#endif /* _ENABLE_RADIO_RDS_ */

#endif /* __RDS_COM_H__ */

