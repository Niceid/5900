#ifndef __KEY_DRV_H__
#define __KEY_DRV_H__


#include "key_drv_ad.h"
#include "key_drv_ir.h"
#include "key_drv_vr.h"
#include "key_drv_voice.h"


#define KEY_UART_DEBUG
#ifdef KEY_UART_DEBUG
    #define key_puts           ew_puts
    #define key_printf         ew_printf
#else
    #define key_puts(...)
    #define key_printf(...)
#endif


/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

enum{
	KEY_PROJECT_RADIO = 0,
	KEY_PROJECT_PLAYER,
	KEY_PROJECT_AUX,
	KEY_PROJECT_TEL,
	KEY_PROJECT_DAB,
#if defined(_SUPPORT_TAPE_)
	KEY_PROJECT_TAPE,
#endif
#if defined(_SUPPORT_MIC_OK_)
	KEY_PROJECT_MIC,
#endif
#if defined(_SUPPORT_CAN_MODEL_)
	KEY_PROJECT_CAN,
#endif
#if defined(_SUPPORT_REVERSE_MODEL_)
    KEY_PROJECT_REVE,
#endif
	KEY_PROJECT_REC,
	KEY_PROJECT_MAX,
	
};

enum{
	KEY_SELECT_DEFAULT = 0,//默认功能
	KEY_SELECT_2NDF,//复用功能
	KEY_SELECT_MAX,
};

typedef struct{
	uint8	Value;
	uint8	State;//up down 判别按下或弹起
	uint8 	LastValue;
	uint8 	NowValue;
	uint8 	ValueTime;
	uint8	LastTime;
	uint8 	Buffer[2];//buf[0]:当前按键索引   buf[1]:上一个按键的索引
}INPUT_INFO_FORMAT;

#define KEY_TIME_POLLING						(40)
#define INPUT_TIME_POLLING						KEY_TIME_POLLING//40
#if defined(_SL6812_DELAY_KEY_)
#define INPUT_TIME_MAX							(10000/INPUT_TIME_POLLING)//125
#else
#define INPUT_TIME_MAX							(5000/INPUT_TIME_POLLING)//125
#endif

#define INPUT_VALUE_NULL						KEY_VALUE_NULL//0

#define GET_KEY_KEY								(KeyInfo.Value)
#define GET_KEY_STATE							(KeyInfo.State)
#define GET_KEY_TIME_DOWN_TIME					(KeyInfo.ValueTime)
#define GET_KEY_TIME_UP_TIME					(KeyInfo.LastTime)

#define GET_IR_IR								(IrInfo.Value)
#define GET_IR_STATE							(IrInfo.State)
#define GET_IR_TIME_DOWN_TIME					(IrInfo.ValueTime)
#define GET_IR_TIME_UP_TIME						(IrInfo.LastTime)


extern INPUT_INFO_FORMAT KeyInfo;
extern INPUT_INFO_FORMAT IrInfo;
extern const char *keymsg_task_name;

/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

extern void key_init(void);
extern void key_msg_register(const char *, uint8);
extern void key_msg_select(uint8);

/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

#endif /* __KEY_DRV_H__ */

