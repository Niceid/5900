#ifndef __RDS_RADIO_H__
#define __RDS_RADIO_H__

#if defined(_ENABLE_RADIO_RDS_)


#define _RDS_AF_LIST_MAX_					(25)
#define _RDS_EON_MAPPED_MAX_				(10)
#define _RDS_EON_LIST_MAX_					(5)

/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/

typedef struct{
	uint8  succeed;
	uint8 buffer[8];
}radio_rds_psn_t;

typedef struct{
	uint8 	succeed;
	uint8 	method;
	uint8 	buffer[_RDS_AF_LIST_MAX_];
}radio_rds_af_t;

typedef struct{
	uint8  	succeed;
	uint16	year;
	uint8	month;
	uint8	day;
	uint8	hour;
	uint8	min;
}radio_rds_date_t;

typedef struct{
	uint8 	length;
	uint8 	freq[_RDS_EON_MAPPED_MAX_];
}radio_rds_eon_mapped_t;

typedef struct{
	uint16					pi;
	uint8					ta;
	uint8					ta_have;
	uint8					tp;
	uint8					pty;
	radio_rds_psn_t			psn;
	radio_rds_af_t			af;
	radio_rds_eon_mapped_t	mapped;
}radio_rds_eon_t;

typedef struct{
	uint16	pi;
	uint8	ta;
	uint8	ta_have;
	uint8	tp;
	uint8	pty;
}radio_rds_info_t;

typedef struct{
	uint16				freq;
	uint8				level;
	uint8				stc;
	uint8				found;
	
	uint8				signal;
	uint8				af_det;
	uint8 				eon_det;
	
	uint16				pi;
	uint8				ms;
	uint8				ta;
	uint8				ta_have;
	uint8				tp;
	uint8				pty;
	
	radio_rds_psn_t		psn;
	radio_rds_af_t		af;
	radio_rds_date_t 	date;
	radio_rds_eon_t		eon[_RDS_EON_LIST_MAX_];
}radio_rds_app_t;

typedef struct{
	uint16	pi;
	uint8 	ms;
	uint8	pty;
	uint8 	psn_succeed;
	uint8 	psn_buffer[8];
	uint8 	af_succeed;
	uint8 	af_method;
	uint8 	af_buffer[_RDS_AF_LIST_MAX_];
}radio_rds_station_t;

/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

#endif

#endif /* __RDS_RADIO_H__ */


