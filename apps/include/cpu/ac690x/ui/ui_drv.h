#ifndef __UI_DRV_H__
#define __UI_DRV_H__


#define UI_LEVEL_MAX						(3)
#define UI_LEVEL_ALL						(0xFF)


typedef enum{
	UI_ICON_NULL = 0,// 0
	
	UI_ICON_R1,
	UI_ICON_R2,
	UI_ICON_R3,
	UI_ICON_R4,
	
	UI_ICON_MUTE,// 5
	UI_ICON_LOUD,
	
	UI_ICON_EQ,
	UI_ICON_EQ_POP,
	UI_ICON_EQ_FLAT,
	UI_ICON_EQ_ROCK,// 10
	UI_ICON_EQ_CLASS,
	UI_ICON_EQ_JAZZ,
	
	UI_ICON_INT,
	UI_ICON_RDM,
	UI_ICON_RPT,// 15

	UI_ICON_AF,
	UI_ICON_TA,
	UI_ICON_TP,
	UI_ICON_PTY,
	UI_ICON_EON, //20
	UI_ICON_REG,
	
	UI_ICON_FILE,
	UI_ICON_FOLDER,
	UI_ICON_MUSIC,
	UI_ICON_ALBUM, //25
	UI_ICON_ARTIST,
	
	UI_ICON_VBAR_1,
	UI_ICON_VBAR_2,
	UI_ICON_VBAR_3,
	UI_ICON_VBAR_4, // 30
	UI_ICON_VBAR_5,
	UI_ICON_VBAR_6,
	UI_ICON_VBAR_7,
	UI_ICON_VBAR_8,
	UI_ICON_VBAR_9,//35
	UI_ICON_VBAR_10,
	
#if defined(SUPPORT_TWO_VBVR_DISPLAY_)
	UI_ICON_VBAR1_1,
	UI_ICON_VBAR2_2,
	UI_ICON_VBAR3_3,
	UI_ICON_VBAR4_4,
	UI_ICON_VBAR5_5,
	UI_ICON_VBAR6_6,
	UI_ICON_VBAR7_7,
	UI_ICON_VBAR8_8,
	UI_ICON_VBAR9_9,
	UI_ICON_VBAR10_10,
#endif

	UI_ICON_SUBWOOFER,
	UI_ICON_BLUETOOTH,
	
	UI_ICON_RADIO,
	UI_ICON_TRACK,//40
	UI_ICON_DISC,
	UI_ICON_USB,
	UI_ICON_SD,
	UI_ICON_PLAY,
	UI_ICON_PAUSE,//45
	UI_ICON_AUX,
	UI_ICON_FLASH,
	
	UI_ICON_MP3,
	UI_ICON_WMA,
	UI_ICON_WAV,//50
	UI_ICON_FLAC,
	UI_ICON_APE,

	UI_ICON_CLOCK,
	UI_ICON_POINT,
	UI_ICON_T1,//55
	UI_ICON_CH,
	UI_ICON_ST,
	UI_ICON_MONO,
	UI_ICON_STO,
	UI_ICON_LOC,//60
	UI_ICON_MHZ,
	UI_ICON_KHZ,
	UI_ICON_FM,
	UI_ICON_POWER,
	UI_ICON_DASH,//65
#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_LCD_3_DIAPLAY_)||defined(_FY7555_CLOCK_ICON_DISPLAY_)
	UI_ICON_AM,
	UI_ICON_PM,
	UI_ICON_TIME,
#endif

#if defined(_ENABLE_RPT_ICON_)
	UI_ICON_RPT1,
	UI_ICON_RPT_ALL,
#endif

#if defined(_ENABLE_WB_DISPLAY_)
	UI_ICON_WB_POINT,
#endif
#if defined(_FY5517_SPECIAL_DISPLAY_)
	UI_ICON_CLOCK2,
#endif

#if defined(_SWL8809_DISPLAY_)||defined(_JL8224B_ICON_DISPLAY_)||defined(_XM003_SPECIAL_DISPLAY_)||defined(_ENABLE_L1_DISP_)
	UI_ICON_POWER2,
#endif
#if defined(_SUPPORT_DISC_)
	UI_ICON_MP4,
	UI_ICON_CD,
	UI_ICON_VCD,
	UI_ICON_DVD,
#endif
#if defined(SUPPORT_ANTENNA_DISPLAY_)|| defined (SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
	UI_ICON_RSSI1,
	UI_ICON_RSSI2,
	UI_ICON_RSSI3,
	UI_ICON_RSSI4,
#endif
#if defined(_ENABLE_VOLUME_CONTROL_)
	UI_ICON_ZONE1,
	UI_ICON_ZONE2,
	UI_ICON_ZONE3,
	UI_ICON_ZONE4,
#endif
#if defined(_XM003_ANSWER_ICON_)
	UI_ICON_ANSWER,
#endif
#if defined(_ENABLE_PLAYER_ICON_)
	UI_ICON_PLAYER,
#endif

#if defined(_SL6812_TEST_LCD_)
	UI_ICON_LCD,
#endif

#if defined(_ENABLE_TEMP_ICON_DISPLAY_)
	UI_ICON_TEMP,
	UI_ICON_WARM,
	UI_ICON_SET,
#endif

#if defined(_FY7555_VBAR_DISPLAY_)
	UI_ICON_L1P,
	UI_ICON_L1Q,
	UI_ICON_L1R,
	UI_ICON_L1S,
	UI_ICON_L1T,
	UI_ICON_L1U,
	UI_ICON_L1V,

    UI_ICON_L2P,
    UI_ICON_L2Q,
    UI_ICON_L2R,
    UI_ICON_L2S,
    UI_ICON_L2T,
    UI_ICON_L2U,
    UI_ICON_L2V,
    
    UI_ICON_L3P,
    UI_ICON_L3Q,
    UI_ICON_L3R,
    UI_ICON_L3S,
    UI_ICON_L3T,
    UI_ICON_L3U,
    UI_ICON_L3V,
    
    UI_ICON_L4P,
    UI_ICON_L4Q,
    UI_ICON_L4R,
    UI_ICON_L4S,
    UI_ICON_L4T,
    UI_ICON_L4U,
    UI_ICON_L4V,
    
    UI_ICON_L5P,
    UI_ICON_L5Q,
    UI_ICON_L5R,
    UI_ICON_L5S,
    UI_ICON_L5T,
    UI_ICON_L5U,
    UI_ICON_L5V,
    
    UI_ICON_L6P,
    UI_ICON_L6Q,
    UI_ICON_L6R,
    UI_ICON_L6S,
    UI_ICON_L6T,
    UI_ICON_L6U,
    UI_ICON_L6V,
    
    UI_ICON_L7P,
    UI_ICON_L7Q,
    UI_ICON_L7R,
    UI_ICON_L7S,
    UI_ICON_L7T,
    UI_ICON_L7U,
    UI_ICON_L7V,
    
    UI_ICON_L8P,
    UI_ICON_L8Q,
    UI_ICON_L8R,
    UI_ICON_L8S,
    UI_ICON_L8T,
    UI_ICON_L8U,
    UI_ICON_L8V,
    
    UI_ICON_L9P,
    UI_ICON_L9Q,
    UI_ICON_L9R,
    UI_ICON_L9S,
    UI_ICON_L9T,
    UI_ICON_L9U,
    UI_ICON_L9V,
    
    UI_ICON_L10P,
    UI_ICON_L10Q,
    UI_ICON_L10R,
    UI_ICON_L10S,
    UI_ICON_L10T,
    UI_ICON_L10U,
    UI_ICON_L10V,
    
    UI_ICON_L11P,
    UI_ICON_L11Q,
    UI_ICON_L11R,
    UI_ICON_L11S,
    UI_ICON_L11T,
    UI_ICON_L11U,
    UI_ICON_L11V,
    
    UI_ICON_L12P,
    UI_ICON_L12Q,
    UI_ICON_L12R,
    UI_ICON_L12S,
    UI_ICON_L12T,
    UI_ICON_L12U,
    UI_ICON_L12V,
    
    UI_ICON_L13P,
    UI_ICON_L13Q,
    UI_ICON_L13R,
    UI_ICON_L13S,
    UI_ICON_L13T,
    UI_ICON_L13U,
    UI_ICON_L13V,
#endif
	UI_ICON_MAX,//66
}ui_icon_e;
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
typedef enum{
	UI_ICON_NULL1 = 0,// 0
	UI_ICON_TIME1,
	UI_ICON_TIME2,
	UI_ICON_MAX1,
}ui_icon_e1;
#endif

typedef struct{
	void (*open)(void);
	void (*close)(void);
	void (*refur)(uint8);
	void (*clear)(uint8);
	void (*icon_open)(uint8, uint16);
	void (*icon_close)(uint8, uint16);
	void (*icon_ctrl)(uint8, uint16 *, uint8 *);
	void (*display)(uint8, uint8, uint8 *, uint8);
#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_TEMP_WARM_DISPLAY_)
	void (*display_icon)(uint8, uint8, uint8 *, uint8);
#endif
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
	void (*led_display_icon)(uint8, uint8, uint8 *, uint8);
	void (*drv_led_icon_ctrl)(uint8, uint16 *, uint8 *);
#endif
#if defined(_FY7555_LCD_DISPLAY_)
	void (*display_buff)(uint8, uint8, uint8 *, uint8);
#endif
	void (*vbar_disp)(uint8, uint8);
	void (*vbar_clean)(uint8);
	void (*level)(uint8);
	void (*flash_enable)(uint8, uint16);
	void (*flash_disable)(uint16);
#if defined(_SL6812_TEST_LCD_)
	void (*open_screen)(uint8);
#endif
}ui_drv_fun_t;

extern const ui_drv_fun_t ui_drv_func;
extern void ui_drv_updata(uint8);


#endif/*__UI_DRV_H__*/

