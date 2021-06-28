#ifndef	__FM_API_H__
#define __FM_API_H__
//enter here

typedef enum{
	RADIO_SEL_M0 = 0,
	RADIO_SEL_M1,
	RADIO_SEL_M2,
	RADIO_SEL_M3,
	RADIO_SEL_M4,
	RADIO_SEL_M5,
	RADIO_SEL_M6,
	RADIO_SEL_M7,
	RADIO_SEL_M8,
	RADIO_SEL_M9,
	RADIO_SEL_M10,
	RADIO_SEL_ADD,
	RADIO_SEL_SUB,
}radio_sel_e;

#if defined(_ENABLE_RADIO_TIME_)
#define RADIO_POLLING_TIME					(250)
#else
#define RADIO_POLLING_TIME					(500)//500
#endif


extern void fm_init(void);
extern void fm_exit(void);
extern void cmd_radio_save_number(radio_sel_e);
extern void cmd_radio_load_next(void);
extern void cmd_radio_load_prev(void);
extern void cmd_radio_load_number(radio_sel_e);
extern void cmd_radio_freq_prev(void);
extern void cmd_radio_freq_next(void);
extern void cmd_radio_seek_next(void);
extern void cmd_radio_seek_prev(void);
extern void cmd_radio_stereo(void);
extern void cmd_radio_loc(void);
extern void cmd_radio_browse(void);
extern void cmd_radio_auto_save(void);
extern void cmd_radio_scan(void);
extern void cmd_radio_band(void);
extern void cmd_radio_stop(void);
extern void Radio_CH_Add(void);
extern void Radio_CH_Sub(void);
extern void Radio_Ch_Save(void);
#if defined(_ENABLE_RADIO_RDS_)
extern void cmd_radio_psn(const uint8 *);
#endif
extern void radio_polling_time(void);

#if defined(_ENABLE_SAVE_FREQ_)
extern void cmd_radio_slect_up(void);
extern void cmd_radio_save_up(void);
extern void cmd_radio_slect_down(void);
extern void cmd_radio_save_down(void);
#endif

#if defined(_EVB_SEEK_FUNTION_)
extern void cmd_radio_freq_prev1(void);
extern void cmd_radio_freq_next1(void);
#endif

#if defined(_XZC1668_RADIO_FUNTION_)
extern void cmd_radio_fm(void);
extern void cmd_radio_am(void);
#endif

#endif /* __FM_API_H__ */
