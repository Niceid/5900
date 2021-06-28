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


///�����ջ��С���ò���
#define VM_TASK_STACK_SIZE          (1024 * 1)
#define MAIN_TASK_STACK_SIZE        (1024 * 2)
#define MUSIC_TASK_STACK_SIZE       (1024 * 4)
#define MUSIC_PHY_TASK_STACK_SIZE   (1024 * 4)
#define LINEIN_TASK_STACK_SIZE      (1024 * 1)
#define TONE_TASK_STACK_SIZE        (1024 * 4)
#define TONE_PHY_TASK_STACK_SIZE    (1024 * 2)
#define UI_TASK_STACK_SIZE          (1024 * 2)


//���ö���
///< ��������ʹ��,������ʱ������.ble,��̨��app
#define   BT_TWS                      0
///����ӻ������Ƿ���뱻���ӻ��ǻ���״̬
 #define    TWS_SLAVE_WAIT_CON         0
///���������Ƿ�һ��ػ�
#define   BT_TWS_POWEROFF_TOGETHER    0

///����û���ӳɹ�֮ǰ��ͨ�����������ɷ���ʹ��,������ͬʱ������԰����Ž������
#define    BT_TWS_SCAN_ENBLE         0

/********************************************************************************/
/*
 *          --------����������
 */
////<����ϵͳ��ӡ���Թ���
//#define __DEBUG
///<��������
#define UART_UPDATA_EN          0
#define	TD_ADD_ALEN				1



#ifdef	TD_ADD_ALEN

//#define	TD_ADD_IPONE_CHARGE		1

//#define	TD_ADD_DISP_CHECKSUM	1

#define	TD_ADD_SPP				 //���ֻ�app����
//#define	TD_ADD_NEW_APP			 //��APPЭ�� ʹ����APPʱ TD_ADD_APPҲ�趨�� δ���ֿ�����

#define	SYS_MODE_CHANG_NOPP       //�л�ģʽ����usb����sd��ʼ����ɺ󲥷���ͣ���������ã������������

#if defined(TD_ADD_SPP)
#define EASYESY_APP_ONOFF           //�ֻ�app������ʾ��ʾ
#else
#define ENABLE_TWO_PHONE_CONNECT    //�����ֻ�ͬʱ����һ���ź�,��ʱ���ܴ�ble��������̨�ֻ�����ʱ������
#endif


#define TD_ADD_ACCOFF_TIMEOUT_RESET 1//��ACC��ʱû�д��� ��ʾ���� ������

#endif
/********************************************************************************/
///<�������
#define SYS_LVD_EN              1
#if (BT_TWS == 1)
	#define BT_KTV_EN			0
#else
///<����K�豦
	#define BT_KTV_EN			0
#endif

///<����
#define PITCH_EN				0	///>���������Х������ͬʱʹ��


///<Х������
#define HOWLING_SUPPRESSION_EN	0	///>Х�в�������ͬʱʹ��

///<���ٱ��
#define SPEED_PITCH_EN			0	///>ע��ù��ܿ���Ҫ����оƬ���ܣ� ��Ӧ���ϵͳƵ�ʣ����Ҿ�����Ҫ������cpuռ���ʸߵ�Ӧ��ͬʱ��

/********************************************************************************/
/*
 *           --------�ֻ�app���� ���� (�ݲ�֧��)
 */
///�����Ƿ�֧���ֻ�app���ܣ�(�ݲ�֧��)
#if (BT_TWS == 1)
   #define SUPPORT_APP_RCSP_EN    0
   #define RCSP_LIGHT_HW	      0
#define BLE_FINGER_SPINNER_EN  0
#else
///�����Ƿ�֧���ֻ�app���ܣ�  (1-֧��,0-��֧��)
   #define SUPPORT_APP_RCSP_EN   0// �˺��ǽ����Լ�appʹ�õĺ꣬���Թرղ���
///�����Ƿ�֧��������Ӳ������ (1-֧��,0-��֧��)
   #define RCSP_LIGHT_HW	     0
   ///�����Ƿ�֧��ָ������BLE���ݴ��� (1-֧��,0-��֧��) //�����ú� SUPPORT_APP_RCSP_EN����ͬʱ��
   #define BLE_FINGER_SPINNER_EN 0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------��Դ������
 */
///   0:  no change
#define    PWR_NO_CHANGE        0
///   1:  LDOIN 5v -> VDDIO 3.3v -> DVDD 1.2v
#define    PWR_LDO33            1
///   2:  LDOIN 5v -> LDO   1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_LDO15            2
///   3:  LDOIN 5v -> DCDC  1.5v -> DVDD 1.2v, support bluetooth
#define    PWR_DCDC15           3

///��ѡ���ã�PWR_NO_CHANGE/PWR_LDO33/PWR_LDO15/PWR_DCDC15
#define PWR_MODE_SELECT         PWR_LDO15
//DCDCʱBTAVDD,��LDOģʽʱ���Ժ��� 0:1.61v  1:1.51v  2:1.43v  3:1.35v
//AC6904��װҪ��1.51v,��Ӧ����Ϊ1
#define VDC15_LEV     3

//0:0.703V  1:0.675V  2:0.631V  3:0.592V
//4:0.559V  5:0.527V  6:0.493V  7:0.462V 
//LC6904��װ��������Ϊ5
#define POWER_DOWN_DVDD_LEV      7

///�����������Զ��ػ���ʱ��u16���ͣ�0��ʾ���Զ��ػ�
#define AUTO_SHUT_DOWN_TIME     0

///<����˫������
#define KEY_DOUBLE_CLICK        0
/********************************************************************************/
///<��ص����ͣ��Ƿ��л���Դ�������
#define SWITCH_PWR_CONFIG		0
/*
	SYS_LDO_Level:3.53v-3.34v-3.18v-3.04v-2.87v-2.73v-2.62v-2.52v
	FM_LDO_Level:3.3v-3.04v-2.76v-2.5v
*/
///<Normal LDO level
#if defined(_SUPPORT_LDO_LEVEL_)
#define SYS_LDO_NORMAL_LEVEL	0	//range:0~7:FM_LDO��VDDIO��һ�𣬽���:level=2
#else
#define SYS_LDO_NORMAL_LEVEL	1	//range:0~7:FM_LDO��VDDIO��һ�𣬽���:level=2
#endif
#define FM_LDO_NORMAL_LEVEL		0	//range:0~3
///<Reduce LDO level
#define SYS_LDO_REDUCE_LEVEL	3	//range:0~7
#define FM_LDO_REDUCE_LEVEL		1	//range:0~3

/********************************************************************************/
/*
 *           --------��Ч������
 */
///<EQģ�鿪��
#define EQ_EN			        1
//���EQ,��1��������0�رգ���ϵͳʱ��Ҫ��ߣ�һ��Ҫ192M��
//�����󲻼�����ǰ��eq�ļ������µ�EQ���ߣ�ע��eq�ļ�����Ϊ"cfg_eq.bin"
//�������eq�ٶ�Ҫ��ߣ���֧�������ʽ����֧�ֶ���
#define USE_SOFTWARE_EQ   		0
///<EQ uart online debug
#define EQ_UART_BDG	    		0
///<dac�����ϲ�
#define DAC_SOUNDTRACK_COMPOUND 0
///<�Զ�mute
#if BT_KTV_EN
	#define DAC_AUTO_MUTE_EN		0
#else
	#define DAC_AUTO_MUTE_EN		1
#endif
///<������
#define KEY_TONE_EN     	    0
///<��0��ʾʹ��Ĭ������
#define SYS_DEFAULT_VOL         0
///<��������
#define VOCAL_REMOVER			0

/********************************************************************************/


/********************************************************************************/
/*
 *           --------DAC VCOMO ����
 */
///�Ƿ�ѡ��VCMOֱ�ƶ���
#define VCOMO_EN 	            0
///��ѡ���ã�DAC_L_R_CHANNEL/DAC_L_CHANNEL/DAC_R_CHANNEL
#define DAC_CHANNEL_SLECT       DAC_L_R_CHANNEL
//DAC ���������IIS
#define DAC2IIS_EN                      0     //1:ʹ��DAC������IIS�Թ̶�������44.1K�����
  #if(DAC2IIS_EN == 1)
     #define DAC2IIS_OUTCLK_AUTO_CLOSE  O    //1:��û������ʱ���Զ��ر�MCLK,SCLK,LRCLKʱ������� 0:���ر�
     #define IISCHIP_WM8978_EN          1    //1: ʹ��WM8978 IISоƬ��
  #else
     #define DAC2IIS_OUTCLK_AUTO_CLOSE  0    //1:��û������ʱ���Զ��ر�MCLK,SCLK,LRCLKʱ������� 0:���ر�
     #define IISCHIP_WM8978_EN          0    //1: ʹ��WM8978 IISоƬ��
  #endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------����������
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
//usb_sd���Ÿ��ã���Ҫ���Լ�����
#define USB_SD0_MULT_EN     0	//<��Ҫ���Լ�����
#define USB_SD1_MULT_EN		0	//<��Ҫ���Լ�����

#if(USB_SD0_MULT_EN == 1)||(USB_SD1_MULT_EN == 1)
	#undef USB_PC_EN
	#define USB_PC_EN       0
#endif
#define POWERUP_DEV_IGNORE       ///<�����豸�ϵ��Ƿ���ӦU�̻�SD

/********************************************************************************/

/********************************************************************************/
/*
 *           --------����������
 */
#include "bluetooth/bluetooth_api.h"


///��ѡ���ã�NORMAL_MODE/TEST_BQB_MODE/TEST_FCC_MODE/TEST_FRE_OFF_MODE/TEST_BOX_MODE
#define BT_MODE             NORMAL_MODE

#if (BT_MODE == TEST_FCC_MODE)
  #undef UART_UPDATA_EN
  #define UART_UPDATA_EN    0
#endif

///ģ������
#define BT_ANALOG_CFG       0
#define BT_XOSC             0//

///<������̨
#if (SUPPORT_APP_RCSP_EN == 1)
	#if defined(__PRODUCT_songshang_v01__)||defined(_DISABLE_BT_MODE_)
	#define BT_BACKMODE         0   //���м�¼���Ļ������ܴ�������̨
	#else
	#define BT_BACKMODE         1
	#endif
#else
	#if defined(__PRODUCT_songshang_v01__)//||defined(_DISABLE_BT_MODE_)
	#define BT_BACKMODE         0   //���м�¼���Ļ������ܴ�������̨
	#else
	#define BT_BACKMODE         1
	#endif
#endif


///dependency
#if (BT_BACKMODE == 0)
    ///<HID���յĶ���ģʽֻ֧�ַǺ�̨
    #define BT_HID_INDEPENDENT_MODE  0
#endif
///<���籨��
//#define BT_PHONE_NUMBER    1

/*
 *           --------�����͹�������
 */
///��ѡ���ã�SNIFF_EN/SNIFF_TOW_CONN_ENTER_POWERDOWN_EN
#define SNIFF_MODE_CONF     0

///��ѡ���ã�BT_POWER_DOWN_EN/BT_POWER_OFF_EN
#define BT_LOW_POWER_MODE   0

#define BT_OSC              0
#define RTC_OSCH            1
#define RTC_OSCL            2

///��ѡ���ã�BT_OSC/RTC_OSCH/RTC_OSCL
#define LOWPOWER_OSC_TYPE   BT_OSC

///��ѡ���ã�32768L//24000000L
#define LOWPOWER_OSC_HZ     24000000L

///��ѡ���ã�BT_BREDR_EN/BT_BLE_EN/(BT_BREDR_EN|BT_BLE_EN)
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
///TWS �̶����Ҷ�ѡ�� ��ѡ���ã�TWS_CHANNEL_LEFT/TWS_CHANNEL_RIGHT)
#define    BT_TWS_CHANNEL_SELECT         0////�̶����Ҷ�ʱ,���Ϊ��,�Ҷ��������������


#if (BLE_BREDR_MODE == BT_BLE_EN) //����ģ�Ŵ�ble����,���滻֧��ble������uboot.bin
#define BLE_APP_UPDATE_SUPPORT_EN	0 
#else
#define BLE_APP_UPDATE_SUPPORT_EN   0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------оƬ��װ����
 */
///RTCVDD��û�а����Ҫ��1,Ŀǰ��Ӧ��װоƬAC6905
#define RTCVDD_TYPE              0
#define BTAVDD_TYPE              0

/********************************************************************************/


/********************************************************************************/
/*
 *           --------MUSIC MACRO
 */
//SMP�����ļ�֧��
#define MUSIC_DECRYPT_EN 		0
//AB�ϵ�֧��
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
///<��׼SDK RTCʱ��ģʽ
#if (BT_TWS == 1)
    #define RTC_CLK_EN          0
#else
#define RTC_CLK_EN          	1
#endif
///dependency
#if (RTC_CLK_EN == 1)
    ///<RTC����ģʽ
    #define RTC_ALM_EN          0
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------REC MACRO
 */
///dependency

///dependency



///>����MIC¼��ģʽ
#if defined(__PRODUCT_songshang_v01__)||defined(_SUPPORT_RECORD_)
#define REC_MODE_EN				1
#else
#define REC_MODE_EN				0
#endif
///>¼��Դѡ��,	0:other		1:dac
#if (BT_KTV_EN == 1)
	#define REC_SOURCE			1
#else
	#define REC_SOURCE			1
#endif


#if (BT_BACKMODE == 1)
    ///<��֧����������̨����¼��
#if defined(_SUPPORT_RECORD_)
	#define REC_EN			   1
#else
    #define REC_EN             0
#endif
#else
    ///<��׼SDK¼������
    #if defined(__PRODUCT_songshang_v01__)||defined(_SUPPORT_RECORD_)
    #define REC_EN            1
	#else
	#define REC_EN            0
	#endif
#endif

#if (REC_EN == 1)
    ///<����¼��ʹ��
	#define BT_REC_EN		1
    ///<MIC¼��ʹ��
	#define MIC_REC_EN		1
    ///<FM¼��ʹ��
	#define FM_REC_EN		1
    ///<AUX¼��ʹ��
	#define AUX_REC_EN		1
#endif

/********************************************************************************/

/********************************************************************************/
/*
 *           --------UI MACRO
 */
///<UI_��ʾ
#if (RCSP_LIGHT_HW == 1)
#define UI_ENABLE                0
#else
#define UI_ENABLE                0
#endif
///dependency
#if (UI_ENABLE == 1)
    #define LCD_128X64_EN        0      ///<lcd ֧��
    #define LED_7_EN             0      ///<led ֧��
#else
    ///����֧��
    #define LCD_128X64_EN        0
    #define LED_7_EN             0
#endif

#if ((LCD_128X64_EN == 1) && (LED_7_EN == 1))
#error  "UI driver support only one"
#endif

#if (LCD_128X64_EN == 1)
    ///�˵���ʾ
    #define LCD_SUPPORT_MENU     1
    ///LRC�����ʾ
    #define LRC_LYRICS_EN        1
#else
    #define LCD_SUPPORT_MENU     0
    #define LRC_LYRICS_EN        0
#endif

/********************************************************************************/
#if (BT_TWS == 1)
///<��������ʹ��,������ʱ������ble,��̨��app.
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
