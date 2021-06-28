
#ifndef __EASYWAY_CONFIG_H__
#define __EASYWAY_CONFIG_H__
extern u8 Interrupt;
extern u8 usb_ERR;

#define _ENABLE_VERSION_VIEW_    // 按密码查看版本号

#define _MUSICID3_ADJUST_        //id3信息使用动态内存空间分配

#define _ENABLE_ST_MONO_   		 //修改上电默认单声道

#define _ENABLE_USB_MODE_        //修改充电的时候不能转模式

#define _DISABLE_MUSIC_LIST_     //屏蔽放音列表功能

#define _EVB_SEEK_FUNTION_       //修改收音手动和自动分两个按键,新APP用

/*****************************************************************
以下定义的是用户的公司
**********5900方案*********************************************/
#define __Easyway					//调试版  公版

/*******************************************************
以下定义的是用户公司的产品
*******************************************************/

#if defined(__Easyway)          //	调试版
	#define USER_FIRM			Easyway

	#define USER_PRODUCT		Easyway1061      //13脚直驱屏(5*8)

#endif


#if !defined(USER_FIRM)
	#error("木有公司名称!")
#elif !defined(USER_PRODUCT)
	#error("木有产品型号!")
#else
	#define USER_CONFIG_DIR									USER_FIRM/USER_PRODUCT
#endif

#define USER_CONFIG_ROOT									.

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
#define  USER_INIT_H										<USER_CONFIG_ROOT/Consumer/USER_CONFIG_DIR/UserInit.h>
#include USER_INIT_H

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*
	AUTO CONFIG
*/
//	显示
#if defined(LCD_MAP_NAME)
	#define LCD_MAP_DIR										USER_CONFIG_ROOT/Res/UiMap/Lcd/LCD_MAP_NAME
#endif
#if defined(LED_MAP_NAME)
	#define LED_MAP_DIR										USER_CONFIG_ROOT/Res/UiMap/Led/LED_MAP_NAME
#endif

//	按键
#if defined(KEY_MAP_NAME)
	#define KEY_MAP_DIR										USER_CONFIG_ROOT/Res/KeyMap/KEY_MAP_NAME
#else
	#error("木有按键资源!")
#endif
//	遥控
#if defined(IR_MAP_NAME)
	#define IR_MAP_DIR										USER_CONFIG_ROOT/Res/IrMap/IR_MAP_NAME
#else
	#error("木有遥控资源!")
#endif
//	命令
//	(按键命令)
#if defined(ORDER_KEY_MAP_NAME)
	#define ORDER_KEY_MAP_DIR								USER_CONFIG_ROOT/Res/OrderMap/Key/ORDER_KEY_MAP_NAME
#else
	#error("木有按键命令资源!")
#endif
//	(遥控命令)
#if defined(ORDER_IR_MAP_NAME)
	#define ORDER_IR_MAP_DIR								USER_CONFIG_ROOT/Res/OrderMap/Ir/ORDER_IR_MAP_NAME
#else
	#error("木有遥控命令资源!")
#endif
//	引脚
#if defined(PIN_MAP_NAME)
	#define PIN_MAP_DIR										USER_CONFIG_ROOT/Res/PinMap/PIN_MAP_NAME
#else
	#error("木有引脚资源!")
#endif

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
//	显示
#define LCD_MAP_DIR_H										<LCD_MAP_DIR/LcdMap.h>
#define LCD_MAP_DIR_C										<LCD_MAP_DIR/LcdMap.c>
#define LED_MAP_DIR_H										<LED_MAP_DIR/LedMap.h>
#define LED_MAP_DIR_C										<LED_MAP_DIR/LedMap.c>

//	按键
#define KEY_MAP_DIR_H										<KEY_MAP_DIR/KeyMap.h>

//	遥控
#define IR_MAP_DIR_H										<IR_MAP_DIR/IrMap.h>

//	命令
//	(按键命令)
#define ORDER_KEY_MAP_DIR_H									<ORDER_KEY_MAP_DIR/Key.h>
#define ORDER_KEY_MAP_DIR_C									<ORDER_KEY_MAP_DIR/Key.c>

//	(遥控命令)
#define ORDER_IR_MAP_DIR_H									<ORDER_IR_MAP_DIR/Ir.h>
#define ORDER_IR_MAP_DIR_C									<ORDER_IR_MAP_DIR/Ir.c>

//	引脚
#define PIN_MAP_DIR_H										<PIN_MAP_DIR/PinMap.h>

#include PIN_MAP_DIR_H


#endif /* __EASYWAY_CONFIG_H__ */


