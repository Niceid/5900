#ifndef __FM_RADIO_H__
#define __FM_RADIO_H__
#if REC_EN
#include "encode/record.h"
#include "encode/encode.h"


#endif


#define FM_DEBUG
#ifdef FM_DEBUG
#define fm_printf 		ew_printf
#define fm_puts 		ew_puts
#else
#define fm_printf(...)
#define fm_puts(...)
#endif



#endif /* __FM_RADIO_H__ */

