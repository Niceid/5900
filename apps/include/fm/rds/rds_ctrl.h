#ifndef __RDS_CTRL__
#define __RDS_CTRL__

#if defined(_ENABLE_RADIO_RDS_)

extern uint8 rds_ctrl_enable(void);
extern uint8 rds_ctrl_enter(uint16, void **, void *);
extern uint8 rds_ctrl_exit(void);
extern void rds_ctrl_reset(void);

extern void rds_ctrl_busy_func(void *);
extern void rds_ctrl_fast_det(void);
extern void rds_ctrl_ta_disable(uint8);
extern void rds_ctrl_ext_info(void *);
extern uint8 rds_ctrl_pause(void);

enum{
	RDS_FOUND_WAIT = 0,
	RDS_FOUND_SUCCEED,
	RDS_FOUND_FINISH,
};
extern uint8 rds_info_found(uint16 *pi);

enum{
	RADIO_RDS_IDLE = 0,
	RADIO_RDS_TA_SEEK,
	RADIO_RDS_PI_SEEK,
	RADIO_RDS_PTY_SEARCH,
	RADIO_RDS_TA,
	RADIO_RDS_EON,
	RADIO_RDS_ALARM,
};

enum{
	RADIO_RDS_CTRL_NULL = 0,
	RADIO_RDS_CTRL_ADD,
	RADIO_RDS_CTRL_SUB,
	RADIO_RDS_CTRL_NEXT,
	RADIO_RDS_CTRL_PREV,
	RADIO_RDS_CTRL_M1,
	RADIO_RDS_CTRL_M2,
	RADIO_RDS_CTRL_M3,
	RADIO_RDS_CTRL_M4,
	RADIO_RDS_CTRL_M5,
	RADIO_RDS_CTRL_M6,
#if defined(_ENABLE_NEW_APP_RDS_)
	RADIO_RDS_CTRL_PTY,
#endif
};

extern uint8 rds_app_state(void);
extern uint8 radio_rds_check(void);
extern uint8 radio_rds_sel(func_sel_e);
extern uint8 radio_rds_ctrl(func_sel_e, uint8);


#endif

#endif /* __RDS_CTRL__ */

