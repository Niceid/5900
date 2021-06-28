#ifndef __DISC_H__
#define __DISC_H__

#include "easyway_config.h"
#include "typedef.h"


#if defined(_SUPPORT_DISC_)

#define DISC_DEBUG

#ifdef DISC_DEBUG
    #define disc_puts     ew_puts
    #define disc_printf   ew_printf
#else
    #define disc_puts(...)
    #define disc_printf(...)
#endif

#if defined(_ENABLE_RADIO_TIME_)
#define DISC_POLLING_DISP_TIME		200
#else
#define DISC_POLLING_DISP_TIME		100
#endif

#define DISC_TASK_NAME        		"DiscTask"
#define PLAYER_DISC_TASK_NAME   	DISC_TASK_NAME"Disc"
#define PLAYER_USB_TASK_NAME    	DISC_TASK_NAME"Usb"
#define PLAYER_CARD_TASK_NAME    	DISC_TASK_NAME"Card"

typedef struct{
	uint8 status;
	uint8 mode;
	uint8 search_mode;
	uint8 search_status;
	uint8 media;
	uint8 error;
#if defined(_FY7000C_DISPLAY_BUFFER_)
    uint8 disp_buffer[13];
#else
	uint8 disp_buffer[8];
#endif
	uint8 disp_num;
	uint8 dot_flag;
}disc_info_t;

typedef struct{
	disc_info_t info;
}disc_ctrl_t;

extern const struct task_info player_disc_task_info;
extern const struct task_info player_usb_task_info;
extern const struct task_info player_card_task_info;

#endif

#endif/*__DISC_H__*/

