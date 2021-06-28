
#ifndef __EASYWAY_CONFIG_H__
#define __EASYWAY_CONFIG_H__
extern u8 Interrupt;
extern u8 usb_ERR;

#define _ENABLE_VERSION_VIEW_    // ������鿴�汾��

#define _MUSICID3_ADJUST_        //id3��Ϣʹ�ö�̬�ڴ�ռ����

#define _ENABLE_ST_MONO_   		 //�޸��ϵ�Ĭ�ϵ�����

#define _ENABLE_USB_MODE_        //�޸ĳ���ʱ����תģʽ

#define _DISABLE_MUSIC_LIST_     //���η����б���

#define _EVB_SEEK_FUNTION_       //�޸������ֶ����Զ�����������,��APP��

/*****************************************************************
���¶�������û��Ĺ�˾
**********5900����*********************************************/
#define __Easyway					//���԰�  ����

/*******************************************************
���¶�������û���˾�Ĳ�Ʒ
*******************************************************/

#if defined(__Easyway)          //	���԰�
	#define USER_FIRM			Easyway

	#define USER_PRODUCT		Easyway1061      //13��ֱ����(5*8)

#endif


#if !defined(USER_FIRM)
	#error("ľ�й�˾����!")
#elif !defined(USER_PRODUCT)
	#error("ľ�в�Ʒ�ͺ�!")
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
//	��ʾ
#if defined(LCD_MAP_NAME)
	#define LCD_MAP_DIR										USER_CONFIG_ROOT/Res/UiMap/Lcd/LCD_MAP_NAME
#endif
#if defined(LED_MAP_NAME)
	#define LED_MAP_DIR										USER_CONFIG_ROOT/Res/UiMap/Led/LED_MAP_NAME
#endif

//	����
#if defined(KEY_MAP_NAME)
	#define KEY_MAP_DIR										USER_CONFIG_ROOT/Res/KeyMap/KEY_MAP_NAME
#else
	#error("ľ�а�����Դ!")
#endif
//	ң��
#if defined(IR_MAP_NAME)
	#define IR_MAP_DIR										USER_CONFIG_ROOT/Res/IrMap/IR_MAP_NAME
#else
	#error("ľ��ң����Դ!")
#endif
//	����
//	(��������)
#if defined(ORDER_KEY_MAP_NAME)
	#define ORDER_KEY_MAP_DIR								USER_CONFIG_ROOT/Res/OrderMap/Key/ORDER_KEY_MAP_NAME
#else
	#error("ľ�а���������Դ!")
#endif
//	(ң������)
#if defined(ORDER_IR_MAP_NAME)
	#define ORDER_IR_MAP_DIR								USER_CONFIG_ROOT/Res/OrderMap/Ir/ORDER_IR_MAP_NAME
#else
	#error("ľ��ң��������Դ!")
#endif
//	����
#if defined(PIN_MAP_NAME)
	#define PIN_MAP_DIR										USER_CONFIG_ROOT/Res/PinMap/PIN_MAP_NAME
#else
	#error("ľ��������Դ!")
#endif

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
//	��ʾ
#define LCD_MAP_DIR_H										<LCD_MAP_DIR/LcdMap.h>
#define LCD_MAP_DIR_C										<LCD_MAP_DIR/LcdMap.c>
#define LED_MAP_DIR_H										<LED_MAP_DIR/LedMap.h>
#define LED_MAP_DIR_C										<LED_MAP_DIR/LedMap.c>

//	����
#define KEY_MAP_DIR_H										<KEY_MAP_DIR/KeyMap.h>

//	ң��
#define IR_MAP_DIR_H										<IR_MAP_DIR/IrMap.h>

//	����
//	(��������)
#define ORDER_KEY_MAP_DIR_H									<ORDER_KEY_MAP_DIR/Key.h>
#define ORDER_KEY_MAP_DIR_C									<ORDER_KEY_MAP_DIR/Key.c>

//	(ң������)
#define ORDER_IR_MAP_DIR_H									<ORDER_IR_MAP_DIR/Ir.h>
#define ORDER_IR_MAP_DIR_C									<ORDER_IR_MAP_DIR/Ir.c>

//	����
#define PIN_MAP_DIR_H										<PIN_MAP_DIR/PinMap.h>

#include PIN_MAP_DIR_H


#endif /* __EASYWAY_CONFIG_H__ */


