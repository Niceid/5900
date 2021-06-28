#ifndef __RDS_DEC_H__
#define __RDS_DEC_H__

#if defined(_ENABLE_RADIO_RDS_)

//#define RDS_DEC_DEBUG
#ifdef RDS_DEC_DEBUG
#define rds_dec_printf 			ew_printf
#define rds_dec_puts 			ew_puts
#else
#define rds_dec_printf(...)
#define rds_dec_puts(...)
#endif


#include "rds_radio.h"

#if defined(_ENABLE_RDS_TIME_)
#define _RDS_QUALITY_TIME_					(600)
#define _RDS_OVER_TIME_						(450)
#define _RDS_EON_TIME_						(600)
#else
#define _RDS_QUALITY_TIME_					(2000)
#define _RDS_OVER_TIME_						(1500)
#define _RDS_EON_TIME_						(2000)
#endif

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

typedef enum{
	RDS_TYPE_RDS = 0,
	RDS_TYPE_RBDS,
}rds_type_e;

typedef enum{
	RDS_AF_STATE_NULL = 0,
	RDS_AF_STATE_LENGTH,
	RDS_AF_STATE_DATA,
	RDS_AF_STATE_SUCCEED,
}rds_af_state_e;

typedef enum{
	RDS_AF_METHOD_NULL = 0,
	RDS_AF_METHOD_A,
	RDS_AF_METHOD_B,
	RDS_AF_METHOD_B_2,	
}rds_af_method_e;

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

typedef struct{
	rds_af_state_e 	state;
	rds_af_method_e method;
	uint8 length;
	uint8 total;
	uint8 buffer[_RDS_AF_LIST_MAX_];//25 Byte
}rds_af_t;

typedef struct{
	uint8 	succeed;
	uint8 	ctrl;
	uint8 	buffer[8];
	uint8 	data[8];
}rds_psn_t;

typedef struct{
	uint8	succeed;
	uint8	year;
	uint8	month;
	uint8	day;
	uint8	hour;
	uint8	min;
}rds_mjd_t;

typedef struct{
	uint16		pi;
	uint8		ta;
	uint8		ta_have;
	uint8		tp;
	uint8		pty;
	rds_psn_t	psn;
	rds_af_t	af;
	struct{
		uint8 	length;
		uint8 	freq[_RDS_EON_MAPPED_MAX_];
	}mapped;
}rds_eon_t;

typedef struct{
	uint8			signal;
	
	uint16 			pi;//电台识别码
	uint8			ms;
	uint8			ta;
	uint8			ta_have;
	uint8			tp;
	uint8			pty;//节目类型 Block2的bit5-bit9
	
	rds_psn_t		psn;
	rds_af_t		af;
	rds_mjd_t		mjd;
	rds_eon_t		eon[_RDS_EON_LIST_MAX_];
}rds_decode_t;

typedef struct{
	rds_type_e 		type;	//	RDS解码类型
	uint8			eon_disale;
	
	uint16			timer_over;
	uint16			timer_eon;
	uint16			timer_eon_list[_RDS_EON_LIST_MAX_];// 5 BYTE
}rds_ctrl_t;

typedef struct{
	uint8 	block_ah;
	uint8 	block_al;
	uint8 	block_bh;
	uint8 	block_bl;
	uint8 	block_ch;
	uint8 	block_cl;
	uint8 	block_dh;
	uint8 	block_dl;
}rds_input_t;

typedef struct{
	uint16			det_time;
	rds_decode_t 	decode;
	rds_ctrl_t		ctrl;
	rds_input_t		input;//RDS的4块
}rds_dec_t;

/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

extern void rds_stream_dec(const rds_input_t *);
extern void rds_block_a_dec(uint8, uint8);
extern void rds_block_b_dec(uint8, uint8);
extern void rds_block_c_dec(uint8, uint8);
extern void rds_block_d_dec(uint8, uint8);

extern void rds_dec_timer(void);
extern void rds_dec_reset(void);
extern void rds_dec_info(radio_rds_info_t *);
extern void rds_dec_app(radio_rds_app_t *);
extern void rds_dec_load(radio_rds_station_t *);
extern void rds_dec_save(const radio_rds_station_t *);
extern void rds_dec_eon(uint8);
extern void rds_dec_det_time(uint16);

#endif

#endif /* __RDS_DEC_H__ */


