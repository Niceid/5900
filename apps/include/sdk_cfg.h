/*********************************************************************************************
    *   Filename        : sdk_cfg.h

    *   Description     : Config for Sound Box Case

    *   Author          : Bingquan

    *   Email           : bingquan_cai@zh-jieli.com

    *   Last modifiled  : 2016-12-01 15:36

    *   Copyright:(c)JIELI  2011-2016  @ , All Rights Reserved.
*********************************************************************************************/

#ifndef _CONFIG_
#define _CONFIG_

#include "includes.h"
#include "dac/dac_api.h"


///任务堆栈大小配置测试
#define VM_TASK_STACK_SIZE          (1024 * 1)
#define MAIN_TASK_STACK_SIZE        (1024 * 2)
#define MUSIC_TASK_STACK_SIZE       (1024 * 4)
#define MUSIC_PHY_TASK_STACK_SIZE   (1024 * 4)
#define LINEIN_TASK_STACK_SIZE      (1024 * 1)
#define TONE_TASK_STACK_SIZE        (1024 * 4)
#define TONE_PHY_TASK_STACK_SIZE    (1024 * 2)
#define UI_TASK_STACK_SIZE          (1024 * 2)


//配置对箱
///< 蓝牙对箱使能,对箱暂时不开放.ble,后台和app
#define   BT_TWS                      0
///对箱从机开机是否进入被连接还是回链状态
 #define    TWS_SLAVE_WAIT_CON         0
///对箱主从是否一起关机
#define   BT_TWS_POWEROFF_TOGETHER    0

///对箱没连接成功之前，通过按键开启可发现使能,即主从同时按下配对按键才进行配对
#define    BT_TWS_SCAN_ENBLE         0

/********************************************************************************/
/*
 *          --------调试类配置
 */
////<开启系统打印调试功能
//#define __DEBUG
///<串口升级
#define UART_UPDATA_EN          0
#define	TD_ADD_ALEN				1



#ifdef	TD_ADD_ALEN

//#define	TD_ADD_IPONE_CHARGE		1

//#define	TD_ADD_DISP_CHECKSUM	1

#define	TD_ADD_SPP				 //带手机app控制
//#define	TD_ADD_NEW_APP			 //新APP协议 使用新APP时 TD_ADD_APP也需定义 未做分开处理

#define	SYS_MODE_CHANG_NOPP       //切换模式进入usb或者sd初始化完成后播放暂停键才有作用，后果引起死机

#if defined(TD_ADD_SPP)
#define EASYESY_APP_ONOFF           //手机app连接显示提示
#else
#define ENABLE_TWO_PHONE_CONNECT    //两部手机同时连接一个信号,此时不能打开ble，这样两台手机连接时会死机
#endif


#define TD_ADD_ACCOFF_TIMEOUT_RESET 1//关ACC超时没有处理 表示死机 需重启

#endif
/********************************************************************************/
///<电量监测
#define SYS_LVD_EN              1
#if (BT_TWS == 1)
	#define BT_KTV_EN			0
#else
///<蓝牙K歌宝
	#define BT_KTV_EN			0
#endif

///<变声
#define PITCH_EN				0	///>变调不能与啸叫抑制同时使能


///<啸叫抑制
#define HOWLING_SUPPRESSION_EN	0	///>啸叫不能与变调同时使能

///<变速变调
#define SPEED_PITCH_EN			0	///>注意该功能开启要考虑芯片性能， 相应提高系统频率，并且尽量不要跟其他cpu占用率高的应用同时打开

/********************************************************************************/
/*
 *           --------手机app控制 配置 (暂不支持)
 */
///配置是否支持手机app功能，(暂不支持)
#if (BT_TWS == 1)
   #define SUPPORT_APP_RCSP_EN    0
   #define RCSP_LIGHT_HW	      0
#define BLE_FINGER_SPINNER_EN  0
#else
///配置是否支持手机app功能，  (1-支持,0-不支持)
   #define SUPPORT_APP_RCSP_EN   0// 此宏是杰理自己app使用的宏，可以关闭不用
///配置是否支持蓝牙灯硬件控制 (1-支持,0-不支持)
   #define RCSP_LIGHT_HW	     0
   ///配置是否支持指尖陀螺BLE数据传输 (1-支持,0-不支持) //此配置和 SUPPORT_APP_RCSP_EN不能同时打开
   #define BLE_FINGER_SPINNER_EN 0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------电源类配置
 */
///   0:  no change
#define    PWR_NO_CHANGE        0
///   1:  LDOIN 5v -> VDDIO 3.3v -> DVDD 1.2v
#define    PWR_LDO33            1
///   2:  LDOIN 5v -> LDO   1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_LDO15            2
///   3:  LDOIN 5v -> DCDC  1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_DCDC15           3

///可选配置：PWR_NO_CHANGE/PWR_LDO33/PWR_LDO15/PWR_DCDC15
#define PWR_MODE_SELECT         PWR_LDO15
//DCDC时BTAVDD,用LDO模式时可以忽略 0:1.61v  1:1.51v  2:1.43v  3:1.35v
//AC6904封装要用1.51v,即应设置为1
#define VDC15_LEV     3

//0:0.703V  1:0.675V  2:0.631V  3:0.592V
//4:0.559V  5:0.527V  6:0.493V  7:0.462V 
//LC6904封装建议设置为5
#define POWER_DOWN_DVDD_LEV      7

///蓝牙无连接自动关机计时，u16类型，0表示不自动关机
#define AUTO_SHUT_DOWN_TIME     0

///<按键双击功能
#define KEY_DOUBLE_CLICK        0
/********************************************************************************/
///<电池电量低，是否切换电源输出配置
#define SWITCH_PWR_CONFIG		0
/*
	SYS_LDO_Level:3.53v-3.34v-3.18v-3.04v-2.87v-2.73v-2.62v-2.52v
	FM_LDO_Level:3.3v-3.04v-2.76v-2.5v
*/
///<Normal LDO level
#if defined(_SUPPORT_LDO_LEVEL_)
#define SYS_LDO_NORMAL_LEVEL	0	//range:0~7:FM_LDO和VDDIO绑一起，建议:level=2
#else
#define SYS_LDO_NORMAL_LEVEL	1	//range:0~7:FM_LDO和VDDIO绑一起，建议:level=2
#endif
#define FM_LDO_NORMAL_LEVEL		0	//range:0~3
///<Reduce LDO level
#define SYS_LDO_REDUCE_LEVEL	3	//range:0~7
#define FM_LDO_REDUCE_LEVEL		1	//range:0~3

/********************************************************************************/
/*
 *           --------音效类配置
 */
///<EQ模块开关
#define EQ_EN			        1
//软件EQ,置1开启，置0关闭，对系统时钟要求高，一般要192M，
//开启后不兼容以前的eq文件，用新的EQ工具，注意eq文件名改为"cfg_eq.bin"
//由于软件eq速度要求高，不支持无损格式，不支持对箱
#define USE_SOFTWARE_EQ   		0
///<EQ uart online debug
#define EQ_UART_BDG	    		0
///<dac声道合并
#define DAC_SOUNDTRACK_COMPOUND 0
///<自动mute
#if BT_KTV_EN
	#define DAC_AUTO_MUTE_EN		0
#else
	#define DAC_AUTO_MUTE_EN		1
#endif
///<按键音
#define KEY_TONE_EN     	    0
///<非0表示使用默认音量
#define SYS_DEFAULT_VOL         0
///<人声消除
#define VOCAL_REMOVER			0

/********************************************************************************/


/********************************************************************************/
/*
 *           --------DAC VCOMO 配置
 */
///是否选择VCMO直推耳机
#define VCOMO_EN 	            0
///可选配置：DAC_L_R_CHANNEL/DAC_L_CHANNEL/DAC_R_CHANNEL
#define DAC_CHANNEL_SLECT       DAC_L_R_CHANNEL
//DAC 声音输出到IIS
#define DAC2IIS_EN                      0     //1:使能DAC声音从IIS以固定采样率44.1K输出。
  #if(DAC2IIS_EN == 1)
     #define DAC2IIS_OUTCLK_AUTO_CLOSE  O    //1:当没有声音时，自动关闭MCLK,SCLK,LRCLK时钟输出。 0:不关闭
     #define IISCHIP_WM8978_EN          1    //1: 使能WM8978 IIS芯片。
  #else
     #define DAC2IIS_OUTCLK_AUTO_CLOSE  0    //1:当没有声音时，自动关闭MCLK,SCLK,LRCLK时钟输出。 0:不关闭
     #define IISCHIP_WM8978_EN          0    //1: 使能WM8978 IIS芯片。
  #endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------外设类配置
 */
#if (BT_TWS == 1)
	#define SDMMC0_EN           0
	#define SDMMC1_EN           0
	#define USB_DISK_EN         0
	#define USB_PC_EN           0
#else
#define SDMMC0_EN           0
#if defined(_SUPPORT_DISC_)
#define SDMMC1_EN           0
#else
#if defined(_SUPPORT_SD_)
#define SDMMC1_EN           1
#else
#define SDMMC1_EN           0
#endif
#endif

#if defined(use_usb_as_uart)||defined(_SUPPORT_DISC_)
#if defined(_NEW_DVD_PIN_)
#define USB_DISK_EN         1
#else
#define USB_DISK_EN         0
#endif

#else
#define USB_DISK_EN         1
#endif

#endif
//usb_sd引脚复用，需要测试兼容性
#define USB_SD0_MULT_EN     0	//<需要测试兼容性
#define USB_SD1_MULT_EN		0	//<需要测试兼容性

#if(USB_SD0_MULT_EN == 1)||(USB_SD1_MULT_EN == 1)
	#undef USB_PC_EN
	#define USB_PC_EN       0
#endif
#define POWERUP_DEV_IGNORE       ///<插着设备上电是否响应U盘或SD

/********************************************************************************/

/********************************************************************************/
/*
 *           --------蓝牙类配置
 */
#include "bluetooth/bluetooth_api.h"


///可选配置：NORMAL_MODE/TEST_BQB_MODE/TEST_FCC_MODE/TEST_FRE_OFF_MODE/TEST_BOX_MODE
#define BT_MODE             NORMAL_MODE

#if (BT_MODE == TEST_FCC_MODE)
  #undef UART_UPDATA_EN
  #define UART_UPDATA_EN    0
#endif

///模拟配置
#define BT_ANALOG_CFG       0
#define BT_XOSC             0//

///<蓝牙后台
#if (SUPPORT_APP_RCSP_EN == 1)
	#if defined(__PRODUCT_songshang_v01__)||defined(_DISABLE_BT_MODE_)
	#define BT_BACKMODE         0   //松尚加录音的机器不能打开蓝牙后台
	#else
	#define BT_BACKMODE         1
	#endif
#else
	#if defined(__PRODUCT_songshang_v01__)//||defined(_DISABLE_BT_MODE_)
	#define BT_BACKMODE         0   //松尚加录音的机器不能打开蓝牙后台
	#else
	#define BT_BACKMODE         1
	#endif
#endif


///dependency
#if (BT_BACKMODE == 0)
    ///<HID拍照的独立模式只支持非后台
    #define BT_HID_INDEPENDENT_MODE  0
#endif
///<来电报号
//#define BT_PHONE_NUMBER    1

/*
 *           --------蓝牙低功耗设置
 */
///可选配置：SNIFF_EN/SNIFF_TOW_CONN_ENTER_POWERDOWN_EN
#define SNIFF_MODE_CONF     0

///可选配置：BT_POWER_DOWN_EN/BT_POWER_OFF_EN
#define BT_LOW_POWER_MODE   0

#define BT_OSC              0
#define RTC_OSCH            1
#define RTC_OSCL            2

///可选配置：BT_OSC/RTC_OSCH/RTC_OSCL
#define LOWPOWER_OSC_TYPE   BT_OSC

///可选配置：32768L//24000000L
#define LOWPOWER_OSC_HZ     24000000L

///可选配置：BT_BREDR_EN/BT_BLE_EN/(BT_BREDR_EN|BT_BLE_EN)
#if (BT_TWS == 1)
	#define BLE_BREDR_MODE      (BT_BREDR_EN)
#elif (BT_KTV_EN == 1)
	#define BLE_BREDR_MODE      (BT_BREDR_EN)
#else
#if defined(TD_ADD_SPP)
#define BLE_BREDR_MODE      (BT_BREDR_EN|BT_BLE_EN)
#else
#define BLE_BREDR_MODE        BT_BREDR_EN
#endif
#define BLE_GATT_ROLE_CFG    0 ///0--SERVER,1--CLIENT
#endif
///TWS 固定左右耳选择 可选配置：TWS_CHANNEL_LEFT/TWS_CHANNEL_RIGHT)
#define    BT_TWS_CHANNEL_SELECT         0////固定左右耳时,左耳为主,右耳不进行搜索配对


#if (BLE_BREDR_MODE == BT_BLE_EN) //仅单模才打开ble升级,需替换支持ble升级的uboot.bin
#define BLE_APP_UPDATE_SUPPORT_EN	0 
#else
#define BLE_APP_UPDATE_SUPPORT_EN   0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------芯片封装配置
 */
///RTCVDD口没有绑出来要置1,目前对应封装芯片AC6905
#define RTCVDD_TYPE              0
#define BTAVDD_TYPE              0

/********************************************************************************/


/********************************************************************************/
/*
 *           --------MUSIC MACRO
 */
//SMP加密文件支持
#define MUSIC_DECRYPT_EN 		0
//AB断点支持
#define MUSIC_AB_RPT_EN 		0

///<MP3
#define DEC_TYPE_MP3_ENABLE     1
///<SBC
#define DEC_TYPE_SBC_ENABLE     1
///<AAC
#define DEC_TYPE_AAC_ENABLE		0

///<3K_code_space
#if defined(_DISABLE_PLAY_WMA_WAV_)
#define DEC_TYPE_WAV_ENABLE     0
#else
#define DEC_TYPE_WAV_ENABLE     1
#endif
///<5K_code_space
#if defined(DAB_MODLE)||defined(_DISABLE_FLAC_APE_)
#if defined(_ENABLE_PLAY_FLAC_APE_)
#define DEC_TYPE_FLAC_ENABLE    1
#define DEC_TYPE_APE_ENABLE     1
#else
#if defined(_ENABLE_FLAC_TYPE_)
#define DEC_TYPE_FLAC_ENABLE    1
#else
#define DEC_TYPE_FLAC_ENABLE    0
#endif
#define DEC_TYPE_APE_ENABLE     0
#endif
#else
#define DEC_TYPE_FLAC_ENABLE    1
///<8K_code_space
#define DEC_TYPE_APE_ENABLE     1
#endif
///<30K_code_space
#if defined(_DISABLE_PLAY_WMA_WAV_)
#define DEC_TYPE_WMA_ENABLE     0
#else
#define DEC_TYPE_WMA_ENABLE     1
#endif
///<30K_code_space
#define DEC_TYPE_F1A_ENABLE     0

/********************************************************************************/

/********************************************************************************/
/*
 *           --------RTC MACRO
 */
///<标准SDK RTC时钟模式
#if (BT_TWS == 1)
    #define RTC_CLK_EN          0
#else
#define RTC_CLK_EN          	1
#endif
///dependency
#if (RTC_CLK_EN == 1)
    ///<RTC闹钟模式
    #define RTC_ALM_EN          0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------REC MACRO
 */
///dependency

///dependency



///>独立MIC录音模式
#if defined(__PRODUCT_songshang_v01__)||defined(_SUPPORT_RECORD_)
#define REC_MODE_EN				1
#else
#define REC_MODE_EN				0
#endif
///>录音源选择,	0:other		1:dac
#if (BT_KTV_EN == 1)
	#define REC_SOURCE			1
#else
	#define REC_SOURCE			1
#endif


#if (BT_BACKMODE == 1)
    ///<不支持与蓝牙后台开启录音
#if defined(_SUPPORT_RECORD_)
	#define REC_EN			   1
#else
    #define REC_EN             0
#endif
#else
    ///<标准SDK录音功能
    #if defined(__PRODUCT_songshang_v01__)||defined(_SUPPORT_RECORD_)
    #define REC_EN            1
	#else
	#define REC_EN            0
	#endif
#endif

#if (REC_EN == 1)
    ///<蓝牙录音使能
	#define BT_REC_EN		1
    ///<MIC录音使能
	#define MIC_REC_EN		1
    ///<FM录音使能
	#define FM_REC_EN		1
    ///<AUX录音使能
	#define AUX_REC_EN		1
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------UI MACRO
 */
///<UI_显示
#if (RCSP_LIGHT_HW == 1)
#define UI_ENABLE                0
#else
#define UI_ENABLE                0
#endif
///dependency
#if (UI_ENABLE == 1)
    #define LCD_128X64_EN        0      ///<lcd 支持
    #define LED_7_EN             0      ///<led 支持
#else
    ///都不支持
    #define LCD_128X64_EN        0
    #define LED_7_EN             0
#endif

#if ((LCD_128X64_EN == 1) && (LED_7_EN == 1))
#error  "UI driver support only one"
#endif

#if (LCD_128X64_EN == 1)
    ///菜单显示
    #define LCD_SUPPORT_MENU     1
    ///LRC歌词显示
    #define LRC_LYRICS_EN        1
#else
    #define LCD_SUPPORT_MENU     0
    #define LRC_LYRICS_EN        0
#endif

/********************************************************************************/
#if (BT_TWS == 1)
///<蓝牙对箱使能,对箱暂时不开放ble,后台和app.
  #if ((BT_KTV_EN)||(SUPPORT_APP_RCSP_EN)||(BLE_BREDR_MODE&BT_BLE_EN))
  #error  "-------stereo not support this config ,please check sdk_cfg.h------------"
  #endif
#endif

#if (BT_LOW_POWER_MODE == BT_POWER_DOWN_EN)
  #if ((SDMMC0_EN)||(SDMMC1_EN)||(USB_DISK_EN)||(UI_ENABLE))
  #error  "-------powerdown not support this config ,please check sdk_cfg.h------------"
  #endif
#endif


#if (BT_MODE != NORMAL_MODE)
  #if ((SNIFF_MODE_CONF)||(BT_LOW_POWER_MODE))
  #error  "-------BT MODE not support this config ,please check sdk_cfg.h------------"
  #endif
#endif

#endif
