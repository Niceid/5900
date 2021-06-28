#ifndef __ORDER_H__
#define __ORDER_H__


#include KEY_MAP_DIR_H
#include IR_MAP_DIR_H


#define ORDER_DEBUG
#ifdef ORDER_DEBUG
#define order_printf 		ew_printf
#define order_puts 			ew_puts
#else
#define order_printf(...)
#define order_puts(...)
#endif


enum{
	ORDER_MODE_NULL = 0,
	ORDER_MODE_DOWN,
	ORDER_MODE_DOWN_DELEY,
	ORDER_MODE_DOWN_LOOP,
	ORDER_MODE_DELEY_LOOP,
	ORDER_MODE_UP_LESS_THAN,
	ORDER_MODE_UP_MORE_THAN,
#if defined(_ZSBY5209_VOLUME_FUNTION_)
	ORDER_MODE_OTHERS,
#endif
#if defined(_SL6812_TEST_LCD_)||defined(_BL7680_PASSWORD_SET_)
	ORDER_MODE_DOWN_DELEY_OTHERS,
#endif
#if defined(_SL6812_DELAY_KEY_)
	ORDER_MODE_DOWN_DELEY_LONG,
#endif
	ORDER_MODE_MAX,
};

enum{
	ORDER_TYPE_SYSTEM = 0,
	ORDER_TYPE_PROJECT,
	ORDER_TYPE_OTHERS,
#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
	ORDER_TYPE_PASSWORD,
#endif
	ORDER_TYPE_END,
};

enum{
	ORDER_ID_SYSTEM = 0,
	ORDER_ID_OTHERS,
	ORDER_ID_RADIO,
	ORDER_ID_PLAYER,
	ORDER_ID_AUX,
	ORDER_ID_TEL,
#if defined(DAB_MODLE)
	ORDER_ID_DAB,
#endif
#if defined(_SUPPORT_TAPE_)
	ORDER_ID_TAPE,
#endif
#if defined(_ENABLE_MIC_REC_)
	ORDER_ID_MIC,
#endif
#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
	ORDER_ID_PASSWORD,
#endif
	ORDER_ID_RECORD,
	ORDER_ID_MAX,
};

#define ORDER_ID_PROJECT_START				ORDER_ID_RADIO// 2

enum{
	ORDER_STATE_NULL = 0,
	ORDER_STATE_DOWN,
	ORDER_STATE_DOWN_LOOP,
	ORDER_STATE_UP,
	ORDER_STATE_MAX,
};

enum{
	ORDER_CFG_TYPE_NULL = 0,
	ORDER_CFG_TYPE_KEY,
	ORDER_CFG_TYPE_IR,
	ORDER_CFG_TYPE_MAX,
};

typedef struct{
	uint8	Value;
	uint8	Mode;
	uint16	Order;
}ORDER_CFG;

typedef struct{
	uint8 	Length;
	ORDER_CFG const *OrderMap;
}ORDER_CFG_FORMAT;

typedef struct{
	ORDER_CFG_FORMAT Key;
	ORDER_CFG_FORMAT Ir;
}ORDER_MAP_FORMAT;

typedef struct{
	uint8	Found;//判别按键索引是否找到
	uint8	LastBuffer;
	uint8	State;
}ORDER_FORMAT;

typedef struct{
	uint8			Project;//0:radio 1:usb/sd 2:aux 3:bt
	uint8			Select;
	ORDER_FORMAT 	Key;
	ORDER_FORMAT 	Ir;
}ORDER;

#define CONV_ORDER_TIME(x)						(x/INPUT_TIME_POLLING)//X/40


extern void send_key_msg(int);
extern void order_init(void);
extern void order_project(uint8);
extern void order_select(uint8);
extern void key_process(void);
extern void ir_process(void);

#endif /* __ORDER_H__ */

