#ifndef	__FM_BAND_H__
#define __FM_BAND_H__
#include "fm_engine.h"


#define RADIO_INFO_STATE_RDS_BIT					(0)
#define RADIO_INFO_STATE_RDS_SCREEN					(_B0_)
#define RADIO_INFO_STATE_NO_SUPPORT_RDS				(0<<RADIO_INFO_STATE_RDS_BIT)
#define RADIO_INFO_STATE_SUPPORT_RDS				(1<<RADIO_INFO_STATE_RDS_BIT)

#define RADIO_INFO_STATE_RBDS_BIT					(1)
#define RADIO_INFO_STATE_RBDS_SCREEN				(_B1_)
#define RADIO_INFO_STATE_NO_SUPPORT_RBDS			(0<<RADIO_INFO_STATE_RBDS_BIT)
#define RADIO_INFO_STATE_SUPPORT_RBDS				(1<<RADIO_INFO_STATE_RBDS_BIT)

#define RADIO_INFO_STATE_BAND_BIT					(2)
#define RADIO_INFO_STATE_BAND_SCREEN				(_B4_|_B3_|_B2_)
#define RADIO_INFO_STATE_AM							(0<<RADIO_INFO_STATE_BAND_BIT)
#define RADIO_INFO_STATE_FM							(1<<RADIO_INFO_STATE_BAND_BIT)
#define RADIO_INFO_STATE_SW							(2<<RADIO_INFO_STATE_BAND_BIT)
#define RADIO_INFO_STATE_LW							(3<<RADIO_INFO_STATE_BAND_BIT)
#define RADIO_INFO_STATE_WB							(4<<RADIO_INFO_STATE_BAND_BIT)

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

typedef enum
{
	AREA_FM_EUR = 0,
	AREA_AM_EUR,
	AREA_FM_USA,
	AREA_AM_USA,
#if defined(_SUPPORT_ORIT_ON_)
	AREA_FM_OIRT,
#endif
#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
	AREA_FM_LATIN,
#endif
#if defined(_SUPPORT_FOUR_AREA_)
	AREA_AM_LATIN,
	AREA_AM_ASIA,
#endif
#if defined(_ENABLE_RADIO_WB_)
	AREA_WB,
#endif
	AREA_MAX,
}radio_area_e;

typedef enum
{
	RADIO_STATE_IDLE = 0,
	RADIO_STATE_SEEK,
	RADIO_STATE_SCAN,
	RADIO_STATE_AUTO_SAVE,
	RADIO_STATE_BROWSE,
}radio_state_e;


typedef struct{
	uint8	State;
	uint16	MinFreq;
	uint16 	MaxFreq;
	uint8	ManualStep;
	uint8	AutoStep;
	uint8	DefaultLength;
	uint16	const *DefaultFreq;
}radio_area_info_t;


//频率
#define RADIO_PAINT_FREQ                          	(_B0_)
//电台号
#define RADIO_PAINT_CHANEL                        	(_B1_)
//标志
#define RADIO_PAINT_FLAG							(_B2_)
//PSN
#define RADIO_PAINT_PSN								(_B3_)
//状态
#define RADIO_PAINT_STATE							(_B4_)
//存台
#define RADIO_PAINT_CH							    (_B5_)

#define RADIO_DISP_FLAG_ST							(_B0_)
#define RADIO_DISP_FLAG_STO							(_B1_)
#define RADIO_DISP_FLAG_LOC							(_B2_)
#if defined(TD_ADD_BANDCH)
#define RADIO_DISP_FLAG_CH							(_B3_)
#endif

#if defined(_ENABLE_SAVE_FREQ_)
#define RADIO_DISP_FLAG_SAVE_CH						(_B4_)
#endif

#if defined(_WK5951CH_SPECIAL_DISPLAY_)
#define RADIO_DISP_FLAG_CH1							(_B5_)
#endif

typedef struct{
	uint8	state;
	uint16	freq;
	uint8	band;
	uint8	num;
	uint8	flag;
	uint8   *psn;
}radio_info_t;

typedef struct{
	engine_status_t 	eng_status;
	engine_auto_seek_t 	auto_seek;
	radio_info_t		radio_info;
	
	uint8			fm_only;
	uint8			eng_mute;
	radio_state_e	state;
	uint8			auto_loc;
	uint8			sto;
	uint8			dir;
	
	uint8   		st_last;
	
	uint8			seek_soft;
	uint8			seek_result;
	uint16			seek_wati;
	
	uint8			BrowseBand;
	uint8			BrowseIndex;
	
	uint8			manual_flag;//true:手动搜台  false:自动搜台
	uint16  		manual_timer;//手动时间

	uint8			paint_flag;
	uint8			flash_counter;
	uint8			flash_state;
	uint8			flash_paint;
	uint8			disp_state;
	uint16 			disp_freq;
	uint8			disp_band;
	uint8			disp_index;
	uint8			disp_flag;
	
#if defined(_EVB_SPECIAL_SEEK__)
		uint16			save_freq;
#endif
#if defined(TD_ADD_BANDCH)
	uint8			disp_bandch_flag;//显示当前的band和电台号
#endif
#if defined(_WK5951CH_SPECIAL_DISPLAY_)
	uint8			band_flag;
#endif
#if defined(_ENABLE_SAVE_FREQ_)
    uint8			save_flag;
    uint8			saveIndex;
	uint8			flash_flag;
#endif
#if defined(_SPECIAL_AUTOSEEK_MEMORY_)
    uint8		    rssi1[7];				// 自动扫描信号强度保存值
    uint16		    freq1[7];				// 自动扫描频率保存值
    uint8		    saveindex;				// 
#elif defined(_YH1701_AUTOSEEK_MEMORY_)
	uint8			rssi1[11];				// 自动扫描信号强度保存值
	uint16			freq1[11];				// 自动扫描频率保存值
	uint8			saveindex;				// 
#endif
}radio_ctrl_t;

typedef struct{
	uint8			Band[RADIO_BAND_SIZE_MAX];////0:FM_EUR 1:AM_EUR 2:FM_USA 3:AM_USA
	uint8			Length;
}radio_band_area_t;

typedef struct{
	uint8	Band;
	uint8	BandLength;//波段数量3FM+2AM
	uint8	FreqLength;
	uint8	Index[RADIO_BAND_SIZE_MAX];								//	收音所显示的频率位置
	uint16 	Freq[RADIO_BAND_SIZE_MAX][RADIO_FREQ_SIZE_MAX];			//	收音频率
}radio_band_info_t;

typedef struct{
	uint8	Loc;//0:DX 1:LOC
	uint8	AmLoc;
	uint8	St;//0:关ST 1:开ST
#if defined(_SUPPORT_AREA_)
	uint8 	area;//收音区域
#endif
}radio_state_t;

typedef struct
{ 
	radio_state_t			radio_info;
	radio_band_area_t		BandArea;
	radio_band_info_t		BandInfo;
}radio_config_t;

#if defined(_ENABLE_RADIO_RDS_)
#include "rds/rds_radio.h"

#if !defined(_DISABLE_RDS_MENMORY_)
typedef struct{
	radio_rds_station_t rds_station[RADIO_FM_BAND_SIZE_MAX][RADIO_FREQ_SIZE_MAX];
}radio_vm_rds_t;
#endif

typedef struct{
	struct{
		uint16 	pi;
		uint8 	signal;
	}rds_station[RADIO_FM_BAND_SIZE_MAX][RADIO_FREQ_SIZE_MAX];
}radio_ams_rds_t;

typedef struct{
	uint8 enable;
	uint8 first_band;
	uint8 rds_psn[8];
	radio_ams_rds_t	rds_ams;
#if !defined(_DISABLE_RDS_MENMORY_)
	uint8 enter_flag;
	radio_vm_rds_t 	rds_vm;
#endif
}radio_rds_t;

extern radio_rds_t *radio_rds;

#endif

extern radio_ctrl_t *radio_ctrl;
extern radio_config_t *radio_config;

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
#define GET_RADIO_LOC								(radio_config->radio_info.Loc)
#define SET_RADIO_LOC(x)							(GET_RADIO_LOC = x)

#define GET_RADIO_AMLOC								(radio_config->radio_info.AmLoc)
#define SET_RADIO_AMLOC(x)							(GET_RADIO_AMLOC = x)

#define GET_RADIO_ST								(radio_config->radio_info.St)
#define SET_RADIO_ST(x)								(GET_RADIO_ST = x)

#define GET_RADIO_AREA								(radio_config->radio_info.area)
#define SET_RADIO_AREA(x)							(GET_RADIO_AREA = x)

#define GET_RADIO_STATE								(radio_ctrl->state)
#define SET_RADIO_STATE(x)							(GET_RADIO_STATE = x)

#define GET_RADIO_AUTOLOC							(radio_ctrl->auto_loc)
#define SET_RADIO_AUTOLOC(x)						(GET_RADIO_AUTOLOC = x)

#define GET_RADIO_STO								(radio_ctrl->sto)
#define SET_RADIO_STO(x)							(GET_RADIO_STO = x)

#define GET_RADIO_DIR_FLAG							(radio_ctrl->dir)
#define SET_RADIO_DIR_UP							(GET_RADIO_DIR_FLAG = UP)
#define SET_RADIO_DIR_DOWN							(GET_RADIO_DIR_FLAG = DOWN)

#define GET_RADIO_BAND								(radio_config->BandInfo.Band)
#define SET_RADIO_BAND(x)							(GET_RADIO_BAND = x)

#define GET_RADIO_BAND_LENGTH						(radio_config->BandInfo.BandLength)
#define SET_RADIO_BAND_LENGTH(x)					(GET_RADIO_BAND_LENGTH = x)

#define GET_RADIO_FREQ_LENGTH						(radio_config->BandInfo.FreqLength)
#define SET_RADIO_FREQ_LENGTH(x)					(GET_RADIO_FREQ_LENGTH = x)

#define GET_RADIO_FREQ_INDEX(Band)					(radio_config->BandInfo.Index[Band])
#define SET_RADIO_FREQ_INDEX(Band,Value)			(GET_RADIO_FREQ_INDEX(Band) = Value)

#define GET_BAND_FREQ_INDEX							(GET_RADIO_FREQ_INDEX(GET_RADIO_BAND))
#define SET_BAND_FREQ_INDEX(Value)					(GET_BAND_FREQ_INDEX = Value)

#define GET_RADIO_FREQ(Band,Index)					(radio_config->BandInfo.Freq[Band][Index])
#define SET_RADIO_FREQ(Band,Index,Value)			(GET_RADIO_FREQ(Band,Index) = Value)

#define GET_BAND_FREQ(Index)						(GET_RADIO_FREQ(GET_RADIO_BAND,Index))
#define SET_BAND_FREQ(Index,Value)					(GET_BAND_FREQ(Index) = Value)

#define GET_BAND_CURRENT_FREQ						(GET_BAND_FREQ(GET_BAND_FREQ_INDEX))
#define SET_BAND_CURRENT_FREQ(Value)				(GET_BAND_CURRENT_FREQ = Value)

#define GET_RADIO_SAVE_FREQ(Band,Index)				(GET_RADIO_FREQ(Band,Index+1))
#define SET_RADIO_SAVE_FREQ(Band,Index,Value)		(GET_RADIO_SAVE_FREQ(Band,Index) = Value)

#define GET_BAND_SAVE_FREQ(Index)					(GET_BAND_FREQ(Index+1))
#define SET_BAND_SAVE_FREQ(Index,Value)				(GET_BAND_SAVE_FREQ(Index) = Value)

#define GET_RADIO_TEMP_FREQ(Band)					(GET_RADIO_FREQ(Band,0))
#define SET_RADIO_TEMP_FREQ(Band,Value)				(GET_RADIO_TEMP_FREQ(Band) = Value)

#define GET_BAND_TEMP_FREQ							(GET_BAND_FREQ(0))
#define SET_BAND_TEMP_FREQ(Value)					(GET_BAND_TEMP_FREQ = Value)

#if defined(_ENABLE_RADIO_RDS_)

#define GET_RADIO_RDS_INFO(Band,Index)				(radio_rds->rds_ams.rds_station[(Band) - radio_rds->first_band][Index])
#define SET_RADIO_RDS_INFO(Band,Index,Value)		(GET_RADIO_RDS_INFO(Band,Index) = (Value))

#define GET_BAND_RDS_INFO(Index)					(GET_RADIO_RDS_INFO(GET_RADIO_BAND,Index))
#define SET_BAND_RDS_INFO(Index,Value)				(GET_BAND_RDS_INFO(Index) = (Value))

#define GET_BAND_RDS_CURRENT_INFO					(GET_BAND_RDS_INFO(GET_BAND_FREQ_INDEX))
#define SET_BAND_RDS_CURRENT_INFO(Value)			(GET_BAND_RDS_CURRENT_INFO = (Value))

#endif

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

extern uint8 GetIndexBandState(radio_area_e);
extern uint16 GetIndexBandMinFreq(radio_area_e);
extern uint16 GetIndexBandMaxFreq(radio_area_e);
extern uint8 GetIndexBandAutoStep(radio_area_e);
extern uint8 GetIndexBandManualStep(radio_area_e);
extern uint8 GetIndexBandPosLength(radio_area_e);
extern uint16 GetIndexBandPosFreq(radio_area_e,uint8);

#define USER_BAND_LENGTH							(radio_config->BandArea.Length)
#define USER_BAND_AREA_INFO(band)					(radio_config->BandArea.Band[band])
#define USER_BAND_INFO								USER_BAND_AREA_INFO(GET_RADIO_BAND)

#define INDEX_RADIO_STATE(band)						(GetIndexBandState(USER_BAND_AREA_INFO(band)))
#define INDEX_RADIO_MIN_FREQ(band)					(GetIndexBandMinFreq(USER_BAND_AREA_INFO(band)))
#define INDEX_RADIO_MAX_FREQ(band)					(GetIndexBandMaxFreq(USER_BAND_AREA_INFO(band)))
#define INDEX_RADIO_MANUAL_STEP(band)				(GetIndexBandManualStep(USER_BAND_AREA_INFO(band)))
#define INDEX_RADIO_AUTO_STEP(band)					(GetIndexBandAutoStep(USER_BAND_AREA_INFO(band)))
#define INDEX_RADIO_POS_LENGTH(band)				(GetIndexBandPosLength(USER_BAND_AREA_INFO(band)))
#define INDEX_RADIO_POS_FREQ(band,pos)				(GetIndexBandPosFreq(USER_BAND_AREA_INFO(band),pos))


#define RADIO_STATE									INDEX_RADIO_STATE(GET_RADIO_BAND)
#define RADIO_MIN_FREQ								INDEX_RADIO_MIN_FREQ(GET_RADIO_BAND)
#define RADIO_MAX_FREQ								INDEX_RADIO_MAX_FREQ(GET_RADIO_BAND)
#define RADIO_MANUAL_STEP							INDEX_RADIO_MANUAL_STEP(GET_RADIO_BAND)
#define RADIO_AUTO_STEP								INDEX_RADIO_AUTO_STEP(GET_RADIO_BAND)
#define RADIO_POS_LENGTH							INDEX_RADIO_POS_LENGTH(GET_RADIO_BAND)
#define RADIO_POS_FREQ(pos)							INDEX_RADIO_POS_FREQ(GET_RADIO_BAND,pos)


#define INDEX_RADIO_STATE_NO_RDS(band)				((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_RDS_SCREEN) == RADIO_INFO_STATE_NO_SUPPORT_RDS)
#define INDEX_RADIO_STATE_RDS(band)					((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_RDS_SCREEN) == RADIO_INFO_STATE_SUPPORT_RDS)

#define INDEX_RADIO_STATE_NO_RBDS(band)				((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_RBDS_SCREEN) == RADIO_INFO_STATE_NO_SUPPORT_RBDS)
#define INDEX_RADIO_STATE_RBDS(band)				((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_RBDS_SCREEN) == RADIO_INFO_STATE_SUPPORT_RBDS)

#define INDEX_RADIO_STATE_AM(band)					((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_BAND_SCREEN) == RADIO_INFO_STATE_AM)
#define INDEX_RADIO_STATE_FM(band)					((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_BAND_SCREEN) == RADIO_INFO_STATE_FM)
#define INDEX_RADIO_STATE_SW(band)					((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_BAND_SCREEN) == RADIO_INFO_STATE_SW)
#define INDEX_RADIO_STATE_LW(band)					((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_BAND_SCREEN) == RADIO_INFO_STATE_LW)
#define INDEX_RADIO_STATE_WB(band)					((INDEX_RADIO_STATE(band) & RADIO_INFO_STATE_BAND_SCREEN) == RADIO_INFO_STATE_WB)


#define RADIO_STATE_NO_RDS							INDEX_RADIO_STATE_NO_RDS(GET_RADIO_BAND)
#define RADIO_STATE_RDS								INDEX_RADIO_STATE_RDS(GET_RADIO_BAND)

#define RADIO_STATE_NO_RBDS							INDEX_RADIO_STATE_NO_RBDS(GET_RADIO_BAND)
#define RADIO_STATE_RBDS							INDEX_RADIO_STATE_RBDS(GET_RADIO_BAND)

#define RADIO_STATE_AM								INDEX_RADIO_STATE_AM(GET_RADIO_BAND)
#define RADIO_STATE_FM								INDEX_RADIO_STATE_FM(GET_RADIO_BAND)
#define RADIO_STATE_SW								INDEX_RADIO_STATE_SW(GET_RADIO_BAND)
#define RADIO_STATE_LW								INDEX_RADIO_STATE_LW(GET_RADIO_BAND)
#define RADIO_STATE_WB								INDEX_RADIO_STATE_WB(GET_RADIO_BAND)

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
extern void RadioInitBand(void);
extern void RadioInitFreq(void);
extern void RadioInitBandFreq(void);
extern uint8 SearchFristBand(void);
extern uint8 SearchNextBand(uint8);
extern uint8 SearchPrevBand(uint8);
extern void BandIndexReset(void);
extern uint8 GetBandNo(uint8);
extern uint8 SearchFreqNo(uint16);
extern void RadioBandTotal(uint8 *,uint8 *);
extern void RadioBandStart(uint8 *,uint8 *);
#if defined(_ENABLE_RADIO_RDS_)
extern uint8 radio_rds_total_freq(void);
extern uint16 *radio_rds_index_freq(uint8);
extern void radio_rds_taxis_freq(void);
#endif

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

extern uint8 radio_engine_init(void);
extern uint8 radio_engine_return(void *);
extern uint8 radio_engine_freq(uint16);
extern uint8 radio_engine_softseek(uint16);
extern uint8 radio_engine_stereo(uint8);
extern uint8 radio_engine_mute(uint8);
extern uint8 radio_engine_auto_seek(uint16, uint16, uint8, uint8);
extern uint8 radio_engine_auto_break(void);
extern uint8 radio_engine_get_status(void);
extern uint8 radio_engine_get_stereo(void);
extern uint8 radio_engine_get_fm_only(void);

#if defined(_ENABLE_RADIO_RDS_)

extern uint8 radio_engine_rds_state(void);
extern uint8 radio_engine_rds_enable(uint8);
extern uint8 radio_engine_rds_front(uint8);
extern uint8 radio_engine_rds_busy(void *);
extern uint8 radio_engine_rds_fast(void);
extern uint8 radio_engine_rds_save(radio_rds_station_t *);
extern uint8 radio_engine_rds_load(radio_rds_station_t *);

#endif

#endif /* __FM_BAND_H__ */


