#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include "typedef.h"
#include "typedef_config.h"
#include "easyway_config.h"
#include "string.h"
#include "stdarg.h"
#include "mem/malloc.h"
#include "hw_cpu.h"
#include "rtos/os_api.h"
#include "rtos/os_cfg.h"
#include "rtos/task_manage.h"
#include "common/common.h"
#include "common/app_cfg.h"
#include "common/error.h"
#include "common/list.h"
#include "common/msg.h"
#include "common/printf.h"
#include "uart.h"


#define MAIN_TASK_NAME        	"MainTask"
#define MUSIC_TASK_NAME       	"MusicTask"
#define MUSIC_USB_NAME 			"MusicTaskUsb"
#define MUSIC_CARD_NAME 		"MusicTaskCard"
#define RECORD_TASK_NAME      	"RECTask"
#define FM_TASK_NAME		  	"FMRadioTask"
#define FM_ENGINE_TASK_NAME   	"FMEngTask"
#define LINEIN_TASK_NAME      	"LineinTask"
#define UDISK_TASK_NAME       	"USBdevTask"
#define BT_TASK_NAME       		"BtUserTask"
#define BTMSG_TASK_NAME       	"btmsg"
#define BTSTACK_TASK_NAME     	"BtStackTask"
#define PC_TASK_NAME        	"USBdevTask"
#define PC_CARD_TASK_NAME   	"PcCardTask"
#define IDLE_TASK_NAME        	"IdleTask"
#define RCSP_TASK_NAME 		  	"RCSP_Task"
#define RDEF_LIGHT_TASK_NAME	"LightTask"
#define DISP_TASK_NAME       	"DispTask"
#if defined(_SUPPORT_AUX1_)
#define LINEIN1_TASK_NAME      	"AuxTask"
extern const struct task_info linein1_task_info;
#endif
#if defined(DAB_MODLE)
#define DAB_TASK_NAME      		"DABTask"
extern const struct task_info dab_task_info;
#endif
#if defined(_SUPPORT_TAPE_)
#define TAPE_TASK_NAME      	"TapeTask"
extern const struct task_info tape_task_info;
#endif
#if defined(_SUPPORT_MIC_OK_)
#define MIC_TASK_NAME      		"MicTask"
extern const struct task_info mic_task_info;
#endif
#if defined(_SUPPORT_CAN_MODEL_)
#define CAN_TASK_NAME      		"CanTask"
extern const struct task_info can_task_info;
#endif

#if defined(_SUPPORT_REVERSE_MODEL_)
#define REVERSE_TASK_NAME      		"ReverseTask"
extern const struct task_info reverse_task_info;
#endif

extern const struct task_info idle_task_info;
extern const struct task_info fm_radio_task_info;
extern const struct task_info music_usb_task_info;
extern const struct task_info music_card_task_info;
extern const struct task_info linein_task_info;
extern const struct task_info bluetooth_task_info;

#endif /* __INCLUDES_H__ */

