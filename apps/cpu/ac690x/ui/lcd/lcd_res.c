#include "sdk_cfg.h"

#if defined(LCD_MAP_NAME)
#include LCD_MAP_DIR_H
#include LCD_MAP_DIR_C
#endif
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
#include LED_MAP_DIR_H
#include LED_MAP_DIR_C
#endif
#if defined(SUPPORT_ANTENNA_DISPLAY_)|| defined (SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
#if defined(LI_L21)
static const uint16 ICON_RSSI1_TAB[] = {LI_L21, LI_NULL};
#endif
#if defined(LI_L22)
static const uint16 ICON_RSSI2_TAB[] = {LI_L22, LI_NULL};
#endif
#if defined(LI_L23)
static const uint16 ICON_RSSI3_TAB[] = {LI_L23, LI_NULL};
#endif
#if defined(LI_L24)
static const uint16 ICON_RSSI4_TAB[] = {LI_L24, LI_NULL};
#endif
#endif

#if defined(_ENABLE_VOLUME_CONTROL_)
static const uint16 ICON_ZONE1_TAB[] = {LI_ZONE1, LI_NULL};
static const uint16 ICON_ZONE2_TAB[] = {LI_ZONE2, LI_NULL};
static const uint16 ICON_ZONE3_TAB[] = {LI_ZONE3, LI_NULL};
static const uint16 ICON_ZONE4_TAB[] = {LI_ZONE4, LI_NULL};
#endif

#if defined(LI_R1)
static const uint16 ICON_R1_TAB[] = {LI_R1, LI_NULL};
#endif
#if defined(LI_R2)
static const uint16 ICON_R2_TAB[] = {LI_R2, LI_NULL};
#endif
#if defined(LI_R3)
static const uint16 ICON_R3_TAB[] = {LI_R3, LI_NULL};
#endif
#if defined(LI_R4)
static const uint16 ICON_R4_TAB[] = {LI_R4, LI_NULL};
#endif

#if defined(LI_MUTE)
static const uint16 ICON_MUTE_TAB[] = {LI_MUTE, LI_NULL};
#endif
#if defined(LI_LOUD)
static const uint16 ICON_LOUD_TAB[] = {LI_LOUD, LI_NULL};
#endif
#if defined(LI_EQ)
static const uint16 ICON_EQ_TAB[] = {LI_EQ, LI_NULL};
#endif
#if defined(LI_POP)
static const uint16 ICON_POP_TAB[] = {LI_POP, LI_NULL};
#endif
#if defined(LI_FLAT)
static const uint16 ICON_FLAT_TAB[] = {LI_FLAT, LI_NULL};
#endif
#if defined(LI_ROCK)
static const uint16 ICON_ROCK_TAB[] = {LI_ROCK, LI_NULL};
#endif
#if defined(LI_CLASS)
static const uint16 ICON_CLASS_TAB[] = {LI_CLASS, LI_NULL};
#endif
#if defined(LI_JAZZ)
static const uint16 ICON_JAZZ_TAB[] = {LI_JAZZ, LI_NULL};
#endif

#if defined(LI_INT)
static const uint16 ICON_INT_TAB[] = {LI_INT, LI_NULL};
#endif
#if defined(LI_RDM)
static const uint16 ICON_RDM_TAB[] = {LI_RDM, LI_NULL};
#endif
#if defined(LI_RPT)
static const uint16 ICON_RPT_TAB[] = {LI_RPT, LI_NULL};
#endif

#if defined(LI_AF)
static const uint16 ICON_AF_TAB[] = {LI_AF, LI_NULL};
#endif
#if defined(LI_TA)
static const uint16 ICON_TA_TAB[] = {LI_TA, LI_NULL};
#endif
#if defined(LI_TP)
static const uint16 ICON_TP_TAB[] = {LI_TP, LI_NULL};
#endif
#if defined(LI_PTY)
static const uint16 ICON_PTY_TAB[] = {LI_PTY, LI_NULL};
#endif
#if defined(LI_EON)
static const uint16 ICON_EON_TAB[] = {LI_EON, LI_NULL};
#endif
#if defined(LI_REG)
static const uint16 ICON_REG_TAB[] = {LI_REG, LI_NULL};
#endif

#if defined(LI_FILE)
static const uint16 ICON_FILE_TAB[] = {LI_FILE, LI_NULL};
#endif
#if defined(LI_FOLDER)
static const uint16 ICON_FOLDER_TAB[] = {LI_FOLDER, LI_NULL};
#endif
#if defined(LI_MUSIC)
static const uint16 ICON_MUSIC_TAB[] = {LI_MUSIC, LI_NULL};
#endif
#if defined(LI_ALBUM)
static const uint16 ICON_ALBUM_TAB[] = {LI_ALBUM, LI_NULL};
#endif
#if defined(LI_ARTIST)
static const uint16 ICON_ARTIST_TAB[] = {LI_ARTIST, LI_NULL};
#endif

#if defined(LI_VBAR_1)
static const uint16 ICON_VBAR_1_TAB[] = {LI_VBAR_1, LI_NULL};
#endif
#if defined(LI_VBAR_2)
static const uint16 ICON_VBAR_2_TAB[] = {LI_VBAR_2, LI_NULL};
#endif
#if defined(LI_VBAR_3)
static const uint16 ICON_VBAR_3_TAB[] = {LI_VBAR_3, LI_NULL};
#endif
#if defined(LI_VBAR_4)
static const uint16 ICON_VBAR_4_TAB[] = {LI_VBAR_4, LI_NULL};
#endif
#if defined(LI_VBAR_5)
static const uint16 ICON_VBAR_5_TAB[] = {LI_VBAR_5, LI_NULL};
#endif
#if defined(LI_VBAR_6)
static const uint16 ICON_VBAR_6_TAB[] = {LI_VBAR_6, LI_NULL};
#endif
#if defined(LI_VBAR_7)
static const uint16 ICON_VBAR_7_TAB[] = {LI_VBAR_7, LI_NULL};
#endif
#if defined(LI_VBAR_8)
static const uint16 ICON_VBAR_8_TAB[] = {LI_VBAR_8, LI_NULL};
#endif
#if defined(LI_VBAR_9)
static const uint16 ICON_VBAR_9_TAB[] = {LI_VBAR_9, LI_NULL};
#endif
#if defined(LI_VBAR_10)
static const uint16 ICON_VBAR_10_TAB[] = {LI_VBAR_10, LI_NULL};
#endif

#if defined(SUPPORT_TWO_VBVR_DISPLAY_)
#if defined(LI_VBAR1_1)
static const uint16 ICON_VBAR1_1_TAB[] = {LI_VBAR1_1, LI_NULL};
#endif
#if defined(LI_VBAR2_2)
static const uint16 ICON_VBAR2_2_TAB[] = {LI_VBAR2_2, LI_NULL};
#endif
#if defined(LI_VBAR3_3)
static const uint16 ICON_VBAR3_3_TAB[] = {LI_VBAR3_3, LI_NULL};
#endif
#if defined(LI_VBAR4_4)
static const uint16 ICON_VBAR4_4_TAB[] = {LI_VBAR4_4, LI_NULL};
#endif
#if defined(LI_VBAR5_5)
static const uint16 ICON_VBAR5_5_TAB[] = {LI_VBAR5_5, LI_NULL};
#endif
#if defined(LI_VBAR6_6)
static const uint16 ICON_VBAR6_6_TAB[] = {LI_VBAR6_6, LI_NULL};
#endif
#if defined(LI_VBAR7_7)
static const uint16 ICON_VBAR7_7_TAB[] = {LI_VBAR7_7, LI_NULL};
#endif
#if defined(LI_VBAR8_8)
static const uint16 ICON_VBAR8_8_TAB[] = {LI_VBAR8_8, LI_NULL};
#endif
#if defined(LI_VBAR9_9)
static const uint16 ICON_VBAR9_9_TAB[] = {LI_VBAR9_9, LI_NULL};
#endif
#if defined(LI_VBAR10_10)
static const uint16 ICON_VBAR10_10_TAB[] = {LI_VBAR10_10, LI_NULL};
#endif

#endif

#if defined(LI_SUBWOOFER)
static const uint16 ICON_SUBWOOFER_TAB[] = {LI_SUBWOOFER, LI_NULL};
#endif
#if defined(LI_BLUETOOTH)
static const uint16 ICON_BLUETOOTH_TAB[] = {LI_BLUETOOTH, LI_NULL};
#endif
#if defined(LI_RADIO)
static const uint16 ICON_RADIO_TAB[] = {LI_RADIO, LI_NULL};
#endif
#if defined(LI_TRACK)
static const uint16 ICON_TRACK_TAB[] = {LI_TRACK, LI_NULL};
#endif
#if defined(LI_DISC)
static const uint16 ICON_DISC_TAB[] = {LI_DISC, LI_NULL};
#endif
#if defined(LI_USB)
static const uint16 ICON_USB_TAB[] = {LI_USB, LI_NULL};
#endif
#if defined(LI_SD)
static const uint16 ICON_SD_TAB[] = {LI_SD, LI_NULL};
#endif
#if defined(LI_PLAY)
static const uint16 ICON_PLAY_TAB[] = {LI_PLAY, LI_NULL};
#endif
#if defined(LI_PAUSE)
static const uint16 ICON_PAUSE_TAB[] = {LI_PAUSE, LI_NULL};
#endif

#if defined(LI_AUX)
static const uint16 ICON_AUX_TAB[] = {LI_AUX, LI_NULL};
#endif
#if defined(LI_FLASH)
static const uint16 ICON_FLASH_TAB[] = {LI_FLASH, LI_NULL};
#endif

#if defined(LI_MP3)
static const uint16 ICON_MP3_TAB[] = {LI_MP3, LI_NULL};
#endif
#if defined(LI_WMA)
static const uint16 ICON_WMA_TAB[] = {LI_WMA, LI_NULL};
#endif
#if defined(LI_WAV)
static const uint16 ICON_WAV_TAB[] = {LI_WAV, LI_NULL};
#endif
#if defined(LI_FLAC)
static const uint16 ICON_FLAC_TAB[] = {LI_FLAC, LI_NULL};
#endif
#if defined(LI_APE)
static const uint16 ICON_APE_TAB[] = {LI_APE, LI_NULL};
#endif

#if defined(LI_CLOCK)
static const uint16 ICON_CLOCK_TAB[] = {LI_CLOCK, LI_NULL};
#endif
#if defined(LI_POINT)
static const uint16 ICON_POINT_TAB[] = {LI_POINT, LI_NULL};
#endif
#if defined(LI_T1)
static const uint16 ICON_T1_TAB[] = {LI_T1, LI_NULL};
#endif
#if defined(LI_CH)
static const uint16 ICON_CH_TAB[] = {LI_CH, LI_NULL};
#endif
#if defined(LI_ST)
static const uint16 ICON_ST_TAB[] = {LI_ST, LI_NULL};
#endif
#if defined(LI_MONO)
static const uint16 ICON_MONO_TAB[] = {LI_MONO, LI_NULL};
#endif
#if defined(LI_STO)
static const uint16 ICON_STO_TAB[] = {LI_STO, LI_NULL};
#endif
#if defined(LI_LOC)
static const uint16 ICON_LOC_TAB[] = {LI_LOC, LI_NULL};
#endif
#if defined(LI_MHZ)
static const uint16 ICON_MHZ_TAB[] = {LI_MHZ, LI_NULL};
#endif
#if defined(LI_KHZ)
static const uint16 ICON_KHZ_TAB[] = {LI_KHZ, LI_NULL};
#endif
#if defined(LI_FM)
static const uint16 ICON_FM_TAB[] = {LI_FM, LI_NULL};
#endif

#if defined(LI_POWER)
static const uint16 ICON_POWER_TAB[] = {LI_POWER, LI_NULL};
#endif


#if defined(_SWL8809_DISPLAY_)||defined(_JL8224B_ICON_DISPLAY_)||defined(_XM003_SPECIAL_DISPLAY_)||defined(_ENABLE_L1_DISP_)
#if defined(LI_POWER2)
static const uint16 ICON_POWER2_TAB[] = {LI_POWER2, LI_NULL};
#endif
#endif

#if defined(LI_DASH)
static const uint16 ICON_DASH_TAB[] = {LI_DASH, LI_NULL};
#endif

#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_LCD_3_DIAPLAY_)||defined(_FY7555_CLOCK_ICON_DISPLAY_)
static const uint16 ICON_AM_TAB[] = {LI_AM, LI_NULL};
static const uint16 ICON_PM_TAB[] = {LI_PM, LI_NULL};
static const uint16 ICON_TIME_TAB[] = {LI_TIME, LI_NULL};
#endif

#if defined(_ENABLE_RPT_ICON_)
static const uint16 ICON_RPT1_TAB[] = {LI_RPT1, LI_NULL};
static const uint16 ICON_RPT_ALL_TAB[] = {LI_RPT_ALL, LI_NULL};
#endif

#if defined(_FY5517_SPECIAL_DISPLAY_)
static const uint16 ICON_CLOCK2_TAB[] = {LI_CLOCK2, LI_NULL};
#endif

#if defined(_SUPPORT_DISC_)
#if defined(LI_MP4)
static const uint16 ICON_MP4_TAB[] = {LI_MP4, LI_NULL};
#endif

#if defined(LI_CD)
static const uint16 ICON_CD_TAB[] = {LI_CD, LI_NULL};
#endif

#if defined(LI_VCD)
static const uint16 ICON_VCD_TAB[] = {LI_VCD, LI_NULL};
#endif

#if defined(LI_DVD)
static const uint16 ICON_DVD_TAB[] = {LI_DVD, LI_NULL};
#endif
#endif

#if defined(_XM003_ANSWER_ICON_)
static const uint16 ICON_ANSWER_TAB[] = {LI_ANSWER, LI_NULL};
#endif
#if defined(_ENABLE_PLAYER_ICON_)
static const uint16 ICON_PLAYER_TAB[] = {LI_PLAYER, LI_NULL};
#endif

#if defined(_SL6812_TEST_LCD_)
static const uint16 ICON_LCD_TAB[] = {LI_LCD, LI_NULL};
#endif

#if defined(_ENABLE_TEMP_ICON_DISPLAY_)
static const uint16 ICON_TEMP_TAB[] = {LI_TEMP, LI_NULL};
static const uint16 ICON_WARM_TAB[] = {LI_WARM, LI_NULL};
static const uint16 ICON_SET_TAB[] = {LI_SET, LI_NULL};
#endif

#if defined(_ENABLE_WB_DISPLAY_)
static const uint16 ICON_WB_TAB[] = {LI_WB_POINT, LI_NULL};
#endif

#if defined(_FY7555_VBAR_DISPLAY_)
static const uint16 ICON_L1P_TAB[] = {LI_L1P, LI_NULL};
static const uint16 ICON_L1Q_TAB[] = {LI_L1Q, LI_NULL};
static const uint16 ICON_L1R_TAB[] = {LI_L1R, LI_NULL};
static const uint16 ICON_L1S_TAB[] = {LI_L1S, LI_NULL};
static const uint16 ICON_L1T_TAB[] = {LI_L1T, LI_NULL};
static const uint16 ICON_L1U_TAB[] = {LI_L1U, LI_NULL};
static const uint16 ICON_L1V_TAB[] = {LI_L1V, LI_NULL};

static const uint16 ICON_L2P_TAB[] = {LI_L2P, LI_NULL};
static const uint16 ICON_L2Q_TAB[] = {LI_L2Q, LI_NULL};
static const uint16 ICON_L2R_TAB[] = {LI_L2R, LI_NULL};
static const uint16 ICON_L2S_TAB[] = {LI_L2S, LI_NULL};
static const uint16 ICON_L2T_TAB[] = {LI_L2T, LI_NULL};
static const uint16 ICON_L2U_TAB[] = {LI_L2U, LI_NULL};
static const uint16 ICON_L2V_TAB[] = {LI_L2V, LI_NULL};

static const uint16 ICON_L3P_TAB[] = {LI_L3P, LI_NULL};
static const uint16 ICON_L3Q_TAB[] = {LI_L3Q, LI_NULL};
static const uint16 ICON_L3R_TAB[] = {LI_L3R, LI_NULL};
static const uint16 ICON_L3S_TAB[] = {LI_L3S, LI_NULL};
static const uint16 ICON_L3T_TAB[] = {LI_L3T, LI_NULL};
static const uint16 ICON_L3U_TAB[] = {LI_L3U, LI_NULL};
static const uint16 ICON_L3V_TAB[] = {LI_L3V, LI_NULL};

static const uint16 ICON_L4P_TAB[] = {LI_L4P, LI_NULL};
static const uint16 ICON_L4Q_TAB[] = {LI_L4Q, LI_NULL};
static const uint16 ICON_L4R_TAB[] = {LI_L4R, LI_NULL};
static const uint16 ICON_L4S_TAB[] = {LI_L4S, LI_NULL};
static const uint16 ICON_L4T_TAB[] = {LI_L4T, LI_NULL};
static const uint16 ICON_L4U_TAB[] = {LI_L4U, LI_NULL};
static const uint16 ICON_L4V_TAB[] = {LI_L4V, LI_NULL};

static const uint16 ICON_L5P_TAB[] = {LI_L5P, LI_NULL};
static const uint16 ICON_L5Q_TAB[] = {LI_L5Q, LI_NULL};
static const uint16 ICON_L5R_TAB[] = {LI_L5R, LI_NULL};
static const uint16 ICON_L5S_TAB[] = {LI_L5S, LI_NULL};
static const uint16 ICON_L5T_TAB[] = {LI_L5T, LI_NULL};
static const uint16 ICON_L5U_TAB[] = {LI_L5U, LI_NULL};
static const uint16 ICON_L5V_TAB[] = {LI_L5V, LI_NULL};

static const uint16 ICON_L6P_TAB[] = {LI_L6P, LI_NULL};
static const uint16 ICON_L6Q_TAB[] = {LI_L6Q, LI_NULL};
static const uint16 ICON_L6R_TAB[] = {LI_L6R, LI_NULL};
static const uint16 ICON_L6S_TAB[] = {LI_L6S, LI_NULL};
static const uint16 ICON_L6T_TAB[] = {LI_L6T, LI_NULL};
static const uint16 ICON_L6U_TAB[] = {LI_L6U, LI_NULL};
static const uint16 ICON_L6V_TAB[] = {LI_L6V, LI_NULL};

static const uint16 ICON_L7P_TAB[] = {LI_L7P, LI_NULL};
static const uint16 ICON_L7Q_TAB[] = {LI_L7Q, LI_NULL};
static const uint16 ICON_L7R_TAB[] = {LI_L7R, LI_NULL};
static const uint16 ICON_L7S_TAB[] = {LI_L7S, LI_NULL};
static const uint16 ICON_L7T_TAB[] = {LI_L7T, LI_NULL};
static const uint16 ICON_L7U_TAB[] = {LI_L7U, LI_NULL};
static const uint16 ICON_L7V_TAB[] = {LI_L7V, LI_NULL};

static const uint16 ICON_L8P_TAB[] = {LI_L8P, LI_NULL};
static const uint16 ICON_L8Q_TAB[] = {LI_L8Q, LI_NULL};
static const uint16 ICON_L8R_TAB[] = {LI_L8R, LI_NULL};
static const uint16 ICON_L8S_TAB[] = {LI_L8S, LI_NULL};
static const uint16 ICON_L8T_TAB[] = {LI_L8T, LI_NULL};
static const uint16 ICON_L8U_TAB[] = {LI_L8U, LI_NULL};
static const uint16 ICON_L8V_TAB[] = {LI_L8V, LI_NULL};

static const uint16 ICON_L9P_TAB[] = {LI_L9P, LI_NULL};
static const uint16 ICON_L9Q_TAB[] = {LI_L9Q, LI_NULL};
static const uint16 ICON_L9R_TAB[] = {LI_L9R, LI_NULL};
static const uint16 ICON_L9S_TAB[] = {LI_L9S, LI_NULL};
static const uint16 ICON_L9T_TAB[] = {LI_L9T, LI_NULL};
static const uint16 ICON_L9U_TAB[] = {LI_L9U, LI_NULL};
static const uint16 ICON_L9V_TAB[] = {LI_L9V, LI_NULL};

static const uint16 ICON_L10P_TAB[] = {LI_L10P, LI_NULL};
static const uint16 ICON_L10Q_TAB[] = {LI_L10Q, LI_NULL};
static const uint16 ICON_L10R_TAB[] = {LI_L10R, LI_NULL};
static const uint16 ICON_L10S_TAB[] = {LI_L10S, LI_NULL};
static const uint16 ICON_L10T_TAB[] = {LI_L10T, LI_NULL};
static const uint16 ICON_L10U_TAB[] = {LI_L10U, LI_NULL};
static const uint16 ICON_L10V_TAB[] = {LI_L10V, LI_NULL};

static const uint16 ICON_L11P_TAB[] = {LI_L11P, LI_NULL};
static const uint16 ICON_L11Q_TAB[] = {LI_L11Q, LI_NULL};
static const uint16 ICON_L11R_TAB[] = {LI_L11R, LI_NULL};
static const uint16 ICON_L11S_TAB[] = {LI_L11S, LI_NULL};
static const uint16 ICON_L11T_TAB[] = {LI_L11T, LI_NULL};
static const uint16 ICON_L11U_TAB[] = {LI_L11U, LI_NULL};
static const uint16 ICON_L11V_TAB[] = {LI_L11V, LI_NULL};

static const uint16 ICON_L12P_TAB[] = {LI_L12P, LI_NULL};
static const uint16 ICON_L12Q_TAB[] = {LI_L12Q, LI_NULL};
static const uint16 ICON_L12R_TAB[] = {LI_L12R, LI_NULL};
static const uint16 ICON_L12S_TAB[] = {LI_L12S, LI_NULL};
static const uint16 ICON_L12T_TAB[] = {LI_L12T, LI_NULL};
static const uint16 ICON_L12U_TAB[] = {LI_L12U, LI_NULL};
static const uint16 ICON_L12V_TAB[] = {LI_L12V, LI_NULL};

static const uint16 ICON_L13P_TAB[] = {LI_L13P, LI_NULL};
static const uint16 ICON_L13Q_TAB[] = {LI_L13Q, LI_NULL};
static const uint16 ICON_L13R_TAB[] = {LI_L13R, LI_NULL};
static const uint16 ICON_L13S_TAB[] = {LI_L13S, LI_NULL};
static const uint16 ICON_L13T_TAB[] = {LI_L13T, LI_NULL};
static const uint16 ICON_L13U_TAB[] = {LI_L13U, LI_NULL};
static const uint16 ICON_L13V_TAB[] = {LI_L13V, LI_NULL};
#endif

#if defined(DEBGE_LED_AND_LCD_TOGETHER)&&1
static const uint16 ICON_TIME1_TAB[] = {LI_TIME1, LI_NULL};
static const uint16 ICON_TIME2_TAB[] = {LI_TIME2, LI_NULL};

static const uint16 *LCD_ICON_MAP1[]=
{
	NULL,
#if defined(LI_TIME1)
	ICON_TIME1_TAB,
#else
	NULL,
#endif
#if defined(LI_TIME2)
	ICON_TIME2_TAB,
#else
	NULL,
#endif
};
#endif

static const uint16 *LCD_ICON_MAP[]=
{
	NULL,

#if defined(LI_R1)
	ICON_R1_TAB,
#else
	NULL,
#endif
#if defined(LI_R2)
	ICON_R2_TAB,
#else
	NULL,
#endif
#if defined(LI_R3)
	ICON_R3_TAB,
#else
	NULL,
#endif
#if defined(LI_R4)
	ICON_R4_TAB,
#else
	NULL,
#endif

#if defined(LI_MUTE)
	ICON_MUTE_TAB,
#else
	NULL,
#endif
#if defined(LI_LOUD)
	ICON_LOUD_TAB,
#else
	NULL,
#endif
#if defined(LI_EQ)
	ICON_EQ_TAB,
#else
	NULL,
#endif
#if defined(LI_POP)
	ICON_POP_TAB,
#else
	NULL,
#endif
#if defined(LI_FLAT)
	ICON_FLAT_TAB,
#else
	NULL,
#endif
#if defined(LI_ROCK)
	ICON_ROCK_TAB,
#else
	NULL,
#endif
#if defined(LI_CLASS)
	ICON_CLASS_TAB,
#else
	NULL,
#endif
#if defined(LI_JAZZ)
	ICON_JAZZ_TAB,
#else
	NULL,
#endif

#if defined(LI_INT)
	ICON_INT_TAB,
#else
	NULL,
#endif
#if defined(LI_RDM)
	ICON_RDM_TAB,
#else
	NULL,
#endif
#if defined(LI_RPT)
	ICON_RPT_TAB,
#else
	NULL,
#endif

#if defined(LI_AF)
	ICON_AF_TAB,
#else
	NULL,
#endif
#if defined(LI_TA)
	ICON_TA_TAB,
#else
	NULL,
#endif
#if defined(LI_TP)
	ICON_TP_TAB,
#else
	NULL,
#endif
#if defined(LI_PTY)
	ICON_PTY_TAB,
#else
	NULL,
#endif
#if defined(LI_EON)
	ICON_EON_TAB,
#else
	NULL,
#endif
#if defined(LI_REG)
	ICON_REG_TAB,
#else
	NULL,
#endif

#if defined(LI_FILE)
	ICON_FILE_TAB,
#else
	NULL,
#endif
#if defined(LI_FOLDER)
	ICON_FOLDER_TAB,
#else
	NULL,
#endif
#if defined(LI_MUSIC)
	ICON_MUSIC_TAB,
#else
	NULL,
#endif
#if defined(LI_ALBUM)
	ICON_ALBUM_TAB,
#else
	NULL,
#endif
#if defined(LI_ARTIST)
	ICON_ARTIST_TAB,
#else
	NULL,
#endif

#if defined(LI_VBAR_1)
	ICON_VBAR_1_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_2)
	ICON_VBAR_2_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_3)
	ICON_VBAR_3_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_4)
	ICON_VBAR_4_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_5)
	ICON_VBAR_5_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_6)
	ICON_VBAR_6_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_7)
	ICON_VBAR_7_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_8)
	ICON_VBAR_8_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_9)
	ICON_VBAR_9_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR_10)
	ICON_VBAR_10_TAB,
#else
	NULL,
#endif
#if defined(SUPPORT_TWO_VBVR_DISPLAY_)
#if defined(LI_VBAR1_1)
	ICON_VBAR1_1_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR2_2)
	ICON_VBAR2_2_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR3_3)
	ICON_VBAR3_3_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR4_4)
	ICON_VBAR4_4_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR5_5)
	ICON_VBAR5_5_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR6_6)
	ICON_VBAR6_6_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR7_7)
	ICON_VBAR7_7_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR8_8)
	ICON_VBAR8_8_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR9_9)
	ICON_VBAR9_9_TAB,
#else
	NULL,
#endif
#if defined(LI_VBAR10_10)
	ICON_VBAR10_10_TAB,
#else
	NULL,
#endif

#endif

#if defined(LI_SUBWOOFER)
	ICON_SUBWOOFER_TAB,
#else
	NULL,
#endif
#if defined(LI_BLUETOOTH)
	ICON_BLUETOOTH_TAB,
#else
	NULL,
#endif
#if defined(LI_RADIO)
	ICON_RADIO_TAB,
#else
	NULL,
#endif
#if defined(LI_TRACK)
	ICON_TRACK_TAB,
#else
	NULL,
#endif
#if defined(LI_DISC)
	ICON_DISC_TAB,
#else
	NULL,
#endif
#if defined(LI_USB)
	ICON_USB_TAB,
#else
	NULL,
#endif
#if defined(LI_SD)
	ICON_SD_TAB,
#else
	NULL,
#endif
#if defined(LI_PLAY)
	ICON_PLAY_TAB,
#else
	NULL,
#endif
#if defined(LI_PAUSE)
	ICON_PAUSE_TAB,
#else
	NULL,
#endif
#if defined(LI_AUX)
	ICON_AUX_TAB,
#else
	NULL,
#endif
#if defined(LI_FLASH)
	ICON_FLASH_TAB,
#else
	NULL,
#endif

#if defined(LI_MP3)
	ICON_MP3_TAB,
#else
	NULL,
#endif
#if defined(LI_WMA)
	ICON_WMA_TAB,
#else
	NULL,
#endif
#if defined(LI_WAV)
	ICON_WAV_TAB,
#else
	NULL,
#endif
#if defined(LI_FLAC)
	ICON_FLAC_TAB,
#else
	NULL,
#endif
#if defined(LI_APE)
	ICON_APE_TAB,
#else
	NULL,
#endif

#if defined(LI_CLOCK)
	ICON_CLOCK_TAB,
#else
	NULL,
#endif
#if defined(LI_POINT)
	ICON_POINT_TAB,
#else
	NULL,
#endif
#if defined(LI_T1)
	ICON_T1_TAB,
#else
	NULL,
#endif
#if defined(LI_CH)
	ICON_CH_TAB,
#else
	NULL,
#endif
#if defined(LI_ST)
	ICON_ST_TAB,
#else
	NULL,
#endif
#if defined(LI_MONO)
	ICON_MONO_TAB,
#else
	NULL,
#endif
#if defined(LI_STO)
	ICON_STO_TAB,
#else
	NULL,
#endif
#if defined(LI_LOC)
	ICON_LOC_TAB,
#else
	NULL,
#endif
#if defined(LI_MHZ)
	ICON_MHZ_TAB,
#else
	NULL,
#endif
#if defined(LI_KHZ)
	ICON_KHZ_TAB,
#else
	NULL,
#endif
#if defined(LI_FM)
	ICON_FM_TAB,
#else
	NULL,
#endif
#if defined(LI_POWER)
	ICON_POWER_TAB,
#else
	NULL,
#endif
#if defined(LI_DASH)
	ICON_DASH_TAB,
#else
	NULL,
#endif

#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)||defined(_LCD_3_DIAPLAY_)||defined(_FY7555_CLOCK_ICON_DISPLAY_)
#if defined(LI_AM)
	ICON_AM_TAB,
#else
	NULL,
#endif
#if defined(LI_PM)
	ICON_PM_TAB,
#else
	NULL,
#endif
#if defined(LI_TIME)
	ICON_TIME_TAB,
#else
	NULL,
#endif
#endif

#if defined(_ENABLE_RPT_ICON_)
#if defined(LI_RPT1)
	ICON_RPT1_TAB,
#else
	NULL,
#endif
#if defined(LI_RPT_ALL)
	ICON_RPT_ALL_TAB,
#else
	NULL,
#endif
#endif

#if defined(_ENABLE_WB_DISPLAY_)
#if defined(LI_WB_POINT)
	ICON_WB_TAB,
#else
	NULL,
#endif
#endif

#if defined(_FY5517_SPECIAL_DISPLAY_)
#if defined(LI_CLOCK2)
	ICON_CLOCK2_TAB,
#else
	NULL,
#endif
#endif

#if defined(_SWL8809_DISPLAY_)||defined(_JL8224B_ICON_DISPLAY_)||defined(_XM003_SPECIAL_DISPLAY_)||defined(_ENABLE_L1_DISP_)
#if defined(LI_POWER2)
	ICON_POWER2_TAB,
#else
	NULL,
#endif
#endif
#if defined(_SUPPORT_DISC_)
#if defined(LI_MP4)
	ICON_MP4_TAB,
#else
	NULL,
#endif

#if defined(LI_CD)
	ICON_CD_TAB,
#else
	NULL,
#endif

#if defined(LI_VCD)
	ICON_VCD_TAB,
#else
	NULL,
#endif

#if defined(LI_DVD)
	ICON_DVD_TAB,
#else
	NULL,
#endif

#endif
#if defined(SUPPORT_ANTENNA_DISPLAY_)|| defined (SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
#if defined(LI_L21)
	ICON_RSSI1_TAB,
#else
	NULL,
#endif
#if defined(LI_L22)
	ICON_RSSI2_TAB,
#else
	NULL,
#endif
#if defined(LI_L23)
	ICON_RSSI3_TAB,
#else
	NULL,
#endif
#if defined(LI_L24)
	ICON_RSSI4_TAB,
#else
	NULL,
#endif
#endif

#if defined(_ENABLE_VOLUME_CONTROL_)
#if defined(LI_ZONE1)
	ICON_ZONE1_TAB,
#else
	NULL,
#endif
#if defined(LI_ZONE2)
	ICON_ZONE2_TAB,
#else
	NULL,
#endif
#if defined(LI_ZONE3)
	ICON_ZONE3_TAB,
#else
	NULL,
#endif
#if defined(LI_ZONE4)
	ICON_ZONE4_TAB,
#else
	NULL,
#endif
#endif

#if defined(_XM003_ANSWER_ICON_)
#if defined(LI_ANSWER)
	ICON_ANSWER_TAB,
#else
	NULL,
#endif
#endif
#if defined(_ENABLE_PLAYER_ICON_)
#if defined(LI_PLAYER)
	ICON_PLAYER_TAB,
#else
	NULL,
#endif
#endif

#if defined(_SL6812_TEST_LCD_)
#if defined(LI_LCD)
	ICON_LCD_TAB,
#else
	NULL,
#endif
#endif

#if defined(_ENABLE_TEMP_ICON_DISPLAY_)
#if defined(LI_TEMP)
	ICON_TEMP_TAB,
#else
	NULL,
#endif
#if defined(LI_WARM)
	ICON_WARM_TAB,
#else
	NULL,
#endif
#if defined(LI_SET)
	ICON_SET_TAB,
#else
	NULL,
#endif
#endif

#if defined(_FY7555_VBAR_DISPLAY_)
	ICON_L1P_TAB,
	ICON_L1Q_TAB,
	ICON_L1R_TAB,
	ICON_L1S_TAB,
	ICON_L1T_TAB,
	ICON_L1U_TAB,
	ICON_L1V_TAB,

    ICON_L2P_TAB,
    ICON_L2Q_TAB,
    ICON_L2R_TAB,
    ICON_L2S_TAB,
    ICON_L2T_TAB,
    ICON_L2U_TAB,
    ICON_L2V_TAB,
    
    ICON_L3P_TAB,
    ICON_L3Q_TAB,
    ICON_L3R_TAB,
    ICON_L3S_TAB,
    ICON_L3T_TAB,
    ICON_L3U_TAB,
    ICON_L3V_TAB,
    
    ICON_L4P_TAB,
    ICON_L4Q_TAB,
    ICON_L4R_TAB,
    ICON_L4S_TAB,
    ICON_L4T_TAB,
    ICON_L4U_TAB,
    ICON_L4V_TAB,
    
    ICON_L5P_TAB,
    ICON_L5Q_TAB,
    ICON_L5R_TAB,
    ICON_L5S_TAB,
    ICON_L5T_TAB,
    ICON_L5U_TAB,
    ICON_L5V_TAB,
    
    ICON_L6P_TAB,
    ICON_L6Q_TAB,
    ICON_L6R_TAB,
    ICON_L6S_TAB,
    ICON_L6T_TAB,
    ICON_L6U_TAB,
    ICON_L6V_TAB,
    
    ICON_L7P_TAB,
    ICON_L7Q_TAB,
    ICON_L7R_TAB,
    ICON_L7S_TAB,
    ICON_L7T_TAB,
    ICON_L7U_TAB,
    ICON_L7V_TAB,
    
    ICON_L8P_TAB,
    ICON_L8Q_TAB,
    ICON_L8R_TAB,
    ICON_L8S_TAB,
    ICON_L8T_TAB,
    ICON_L8U_TAB,
    ICON_L8V_TAB,
    
    ICON_L9P_TAB,
    ICON_L9Q_TAB,
    ICON_L9R_TAB,
    ICON_L9S_TAB,
    ICON_L9T_TAB,
    ICON_L9U_TAB,
    ICON_L9V_TAB,
    
    ICON_L10P_TAB,
    ICON_L10Q_TAB,
    ICON_L10R_TAB,
    ICON_L10S_TAB,
    ICON_L10T_TAB,
    ICON_L10U_TAB,
    ICON_L10V_TAB,
    
    ICON_L11P_TAB,
    ICON_L11Q_TAB,
    ICON_L11R_TAB,
    ICON_L11S_TAB,
    ICON_L11T_TAB,
    ICON_L11U_TAB,
    ICON_L11V_TAB,
    
    ICON_L12P_TAB,
    ICON_L12Q_TAB,
    ICON_L12R_TAB,
    ICON_L12S_TAB,
    ICON_L12T_TAB,
    ICON_L12U_TAB,
    ICON_L12V_TAB,

    ICON_L13P_TAB,
    ICON_L13Q_TAB,
    ICON_L13R_TAB,
    ICON_L13S_TAB,
    ICON_L13T_TAB,
    ICON_L13U_TAB,
    ICON_L13V_TAB,
#endif
};

#if defined(_LCD_DRIVER_A4900B_)||defined(_LCD_DRIVER_A4901_)||defined(_LCD_COM5_SEG8)
static const uint8 LCD_ICON_LIST[8]=
{
	_B0_,_B1_,_B2_,_B3_,_B4_,_B5_,_B6_,_B7_,
};
#else
static const uint8 LCD_ICON_LIST[8]=
{
	_B7_,_B6_,_B5_,_B4_,_B3_,_B2_,_B1_,_B0_,
};
#endif

#ifdef	TD_ADD_USER_LCD
static const uint16 SEG13_FONT[]=
{
	VFD_DIGITAL_0,
	VFD_DIGITAL_1,
	VFD_DIGITAL_2,
	VFD_DIGITAL_3,
	VFD_DIGITAL_4,
	VFD_DIGITAL_5,
	VFD_DIGITAL_6,
	VFD_DIGITAL_7,
	VFD_DIGITAL_8,
	VFD_DIGITAL_9,
	VFD_DIGITAL_A,
	VFD_DIGITAL_B,
	VFD_DIGITAL_C,
	VFD_DIGITAL_D,
	VFD_DIGITAL_E,
	VFD_DIGITAL_F,
	VFD_DIGITAL_G,
	VFD_DIGITAL_H,
	VFD_DIGITAL_I,
	VFD_DIGITAL_J,
	VFD_DIGITAL_K,
	VFD_DIGITAL_L,
	VFD_DIGITAL_M,
	VFD_DIGITAL_N,
	VFD_DIGITAL_O,
	VFD_DIGITAL_P,
	VFD_DIGITAL_Q,
	VFD_DIGITAL_R,
	VFD_DIGITAL_S,
	VFD_DIGITAL_T,
	VFD_DIGITAL_U,
	VFD_DIGITAL_V,
	VFD_DIGITAL_W,
	VFD_DIGITAL_X,
	VFD_DIGITAL_Y,
	VFD_DIGITAL_Z,
	VFD_DIGITAL_NULL,//¿Õ36
	VFD_DIGITAL_01,/*.*/
	VFD_DIGITAL_02,/*|*/
	VFD_DIGITAL_03,/*-*/
	VFD_DIGITAL_04,/*+*/
	VFD_DIGITAL_05,/*=*/
	VFD_DIGITAL_06,/*'*/
	VFD_DIGITAL_07,/*_*/
	VFD_DIGITAL_08,/*>*/
	VFD_DIGITAL_09,/*<*/
	VFD_DIGITAL_10,/*[*/
	VFD_DIGITAL_11,/*]*/
	VFD_DIGITAL_12,/***/
	VFD_DIGITAL_13,/*/*/
	VFD_DIGITAL_14,/*\*/
	VFD_DIGITAL_15,/*#*/
};
#else
#if defined(_SPECIAL_LCD_DISP_)||defined(_ZSBY5209_SPECIAL_DISPLAY_)
static const uint16 SEG13_FONT[]=
{
	SF0|SF1|SF2|SFA|SFB|SFC,			/*0*/
	SF2|SFB,							/*1*/
	SF0|SF2|SF5|SF7|SFA|SFC,			/*2*/
	SF0|SF2|SF5|SF7|SFB|SFC,			/*3*/
	SF1|SF5|SF7|SF2|SFB,				/*4*/
	SF0|SF1|SF5|SF7|SFB|SFC,			/*5*/
	SF0|SF1|SFA|SFC|SFB|SF5|SF7,		/*6*/	
	SF1|SF0|SF2|SFB,					/*7*/
	SF0|SF1|SFA|SFC|SFB|SF2|SF5|SF7,	/*8*/
	SF0|SF1|SF5|SF7|SF2|SFB|SFC,		/*9*/ 
	SFA|SF1|SF0|SF2|SFB|SF5|SF7,		/*A*/
	SF1|SF5|SFC|SFB|SFA|SF7,			/*B*/// b
	
	SF0|SF1|SFA|SFC,					/*C*/
	SF2|SF5|SFA|SFC|SFB|SF7,			/*D*/// d
	SF0|SF1|SF5|SF7|SFA|SFC,			/*E*/
	SF0|SF1|SF5|SF7|SFA,				/*F*/
	SF0|SF1|SF5|SF7|SF2|SFB|SFC,		/*G*///g
	SF1|SF2|SF5|SF7|SFA|SFB,			/*H*/
	SF2|SFB,						    /*I*/// 1
	SF2|SFB|SFC,						/*J*/
	SF1|SFA|SF5|SF4|SF9,				/*K*/
	SFA|SFC|SF1,						/*L*/
#if defined(_MR_CHANGE_DISP_)
    SF1|SF2|SF0|SFA|SFB,			    /*M*/// n
#else
	SF1|SF2|SF5|SF7|SFA|SFB,			/*M*/// H
#endif	
	SF1|SF2|SF0|SFA|SFB,			    /*N*/// n
   	SF0|SF1|SFA|SFB|SFC|SF2,			/*O*/	
	SFA|SF1|SF5|SF7|SF2|SF0,			/*P*/
	SF0|SF2|SF1|SF5|SFB|SF7,		    /*Q*/// q
	SFA|SF5|SF7,		                /*R*/// r
	SF0|SF5|SF1|SF7|SFB|SFC,			/*S*/
	SF1|SF5|SF7|SFA|SFC,				/*T*/ // t
	
	SF1|SF2|SFA|SFB|SFC,				/*U*/
	SF1|SF2|SFA|SFB|SFC,				/*V*/// u
	SF1|SF2|SF5|SF7|SFA|SFB,			/*W*/
	SF1|SF2|SF5|SF7|SFA|SFB,			/*X*/// H
	SF1|SF2|SF5|SF7|SFB|SFC,			/*Y*/
	SF0|SF2|SF5|SF7|SFA|SFC,			/*Z*/// 2
	0,									/* */
	SF9,								/*.*/
	SF6,								/*|*/
	SF5|SF7,							/*-*/
	SF5|SF7|SF6,						/*+*/
	SF5|SF7|SFC,						/*=*/
	SF4,								/*'*/
	SFC,								/*_*/
	SF3|SF8,							/*>*/
	SF4|SF9,							/*<*/
	SF0|SF1|SFA|SFC,					/*[*/
	SF0|SF2|SFB|SFC,					/*]*/
	SF3|SF4|SF5|SF6|SF7|SF8|SF9,		/***/
	SF4|SF8,							/*/*/
	SF3|SF9,							/*\*/
	SF0|SF1|SF2|SF3|SF4|SF5|SF6|SF7|SF8|SF9|SFA|SFB|SFC,/*#*/

#if defined(_SUPPORT_CHAR_RUSSIAN_)

	SFA|SF1|SF0|SF2|SFB|SF5|SF7,							/*	§¡ */
	SF0|SF1|SFA|SFB|SFC|SF5|SF7,							/*	§¢ */
	SF0|SF1|SF4|SF5|SF7|SFA|SFB|SFC,						/*	§£ */
	SF0|SF1|SFA,											/*	§¤ */
	SF2|SF4|SF8|SFB|SFC,									/*	§¥ */
	SF0|SF1|SF5|SF7|SFA|SFC,								/*	§¦ */
	SF1|SF2|SF5|SF6|SF7|SFA|SFB,							/*	§¨ */
	SF0|SF2|SF5|SF7|SFB|SFC,								/*	§© */
	SF1|SF2|SF8|SF4|SFA|SFB,								/*	§ª */
	SF0|SF1|SF2|SF8|SF4|SFA|SFB,							/*	§« */
	SF1|SFA|SF5|SF4|SF9,									/*	§¬ */
	SF2|SF4|SF8|SFB,										/*	§­ */
	SF1|SFA|SFB|SF2|SF3|SF4,								/*	§® */
	SF1|SF2|SF5|SF7|SFA|SFB,								/*	§¯ */
	SF0|SF1|SFA|SFB|SFC|SF2,								/*	§° */
	SF0|SF1|SF2|SFA|SFB,									/*	§± */

	SFA|SF1|SF5|SF7|SF2|SF0,								/*	§² */
	SF0|SF1|SFA|SFC,										/*	§³ */
	SF0|SF6,												/*	§´ */
	SF1|SF2|SF5|SF7|SFB|SFC,								/*	§µ */
	SF0|SF1|SF2|SF5|SF6|SF7,								/*	§¶ */
	SF3|SF4|SF8|SF9,										/*	§· */
	SF1|SF6|SFA|SFC,										/*	§¸ */
	SF1|SF5|SF7|SF2|SFB,									/*	§¹ */
	SF1|SF2|SF6|SFA|SFB|SFC,								/*	§º */
	SF1|SF2|SF6|SF9|SFA|SFB|SFC,							/*	§» */
	SF0|SF4|SF7|SF8|SFB|SFC,								/*	§¼ */
	SF1|SF2|SF5|SF9|SFA|SFB|SFC,							/*	§½ */
	SF1|SF5|SF9|SFA|SFC,									/*	§¾ */
	SF0|SF2|SF5|SF7|SFB|SFC,								/*	§¿ */
	SF1|SF2|SF4|SF5|SF9|SFA|SFB,							/*	§À */
	SF0|SF1|SF2|SF5|SF7|SF8|SFB,							/*	§Á */

	SF0|SF1|SF5|SF7|SFA|SFC,								/*	§§ */
#endif
};
#else
static const uint16 SEG13_FONT[]=
{
#if defined(_SL6812A_SPECIAL_DISPLAY_)
	SF0|SF1|SF2|SF4|SF8|SFA|SFB|SFC,	/*0*/
#else
	SF0|SF1|SF2|SFA|SFB|SFC,			/*0*/
#endif
	SF2|SFB,							/*1*/
	SF0|SF2|SF5|SF7|SFA|SFC,			/*2*/
	SF0|SF2|SF5|SF7|SFB|SFC,			/*3*/
	SF1|SF5|SF7|SF2|SFB,				/*4*/
	SF0|SF1|SF5|SF7|SFB|SFC,			/*5*/
	SF0|SF1|SFA|SFC|SFB|SF5|SF7,		/*6*/
#if defined(SPECIAL_JD_DISP)
	SF0|SF2|SFB,						/*7*/
#else
	SF1|SF0|SF2|SFB,					/*7*/
#endif
	SF0|SF1|SFA|SFC|SFB|SF2|SF5|SF7,	/*8*/
	SF0|SF1|SF5|SF7|SF2|SFB|SFC,		/*9*/
	SFA|SF1|SF0|SF2|SFB|SF5|SF7,		/*A*/
	SF0|SF6|SFC|SFB|SF2|SF7,			/*B*/
	SF0|SF1|SFA|SFC,					/*C*/
	SF0|SF6|SFC|SFB|SF2,				/*D*/
#if defined(_FY5517_SPECIAL_DISPLAY_)
    SF0|SF1|SF5|SFA|SFC,				/*E*/
    SF0|SF1|SF5|SFA,					/*F*/
#else
	SF0|SF1|SF5|SF7|SFA|SFC,			/*E*/
	SF0|SF1|SF5|SF7|SFA,				/*F*/
#endif
#if defined(SPECIAL_JD_DISP)||defined(_FY7000D_MIBA_DISPLAY_)
	SF0|SF1|SFA|SFB|SFC,				/*G*/
#else
	SF0|SF1|SFA|SFB|SFC|SF7,			/*G*/
#endif
	SF1|SF2|SF5|SF7|SFA|SFB,			/*H*/
	SF0|SF6|SFC,						/*I*/
#if defined(_SL6812A_SPECIAL_DISPLAY_)
	SF2|SFB|SFC|SFA,					/*J*/
#else
	SF2|SFB|SFC,						/*J*/
#endif
	SF1|SFA|SF5|SF4|SF9,				/*K*/
	SFA|SFC|SF1,						/*L*/
#if defined(_XM003_SPECIAL_DISPLAY_)
	SF1|SFA|SFB|SF2|SF0|SF6,			/*m*/
#else
	SF1|SFA|SFB|SF2|SF3|SF4,			/*M*/
#endif
	SF1|SF2|SF3|SF9|SFA|SFB,			/*N*/
   	SF0|SF1|SFA|SFB|SFC|SF2,			/*O*/
	SFA|SF1|SF5|SF7|SF2|SF0,			/*P*/
	SF0|SF2|SF1|SFA|SFB|SFC|SF9,		/*Q*/
	SFA|SF0|SF1|SF5|SF7|SF2|SF9,		/*R*/
#if defined(_FY5517_SPECIAL_DISPLAY_)
	SF0|SF3|SF7|SFB|SFC,				/*S*/
#else
	SF0|SF5|SF1|SF7|SFB|SFC,			/*S*/
#endif
	SF0|SF6,							/*T*/
	SF1|SF2|SFA|SFB|SFC,				/*U*/
	SF1|SFA|SF8|SF4,					/*V*/
	SF1|SF2|SF8|SF9|SFA|SFB,			/*W*/
	SF3|SF4|SF8|SF9,					/*X*/
	SF1|SF2|SF5|SF7|SFB|SFC,			/*Y*/
	SF0|SF4|SF8|SFC,					/*Z*/
	0,									/* */	//36
	SF9,								/*.*/
	SF6,								/*|*/
	SF5|SF7,							/*-*/
	SF5|SF7|SF6,						/*+*/
	SF5|SF7|SFC,						/*=*/
	SF4,								/*'*/
	SFC,								/*_*/
#if defined(_XM003_SPECIAL_DISPLAY_)
	SF0,								/*>*/
	SF1|SF2|SF0|SFA|SFB,				/*<*///n
#elif defined(_SUPPORT_SPECIAL_DISPLAY_)||defined(ENABLE_POWER_LOGO_)
    SF0|SF2|SF5|SF7|SFA|SFC,			/*>*///z
	SF1|SF2|SF0|SFA|SFB,				/*<*///n
#else
	SF3|SF8,							/*>*/
#if defined(_SL6802_SPECIA_DISPLAY_)
	SF1|SF5|SF7|SF2|SFB|SFC,			/*<*///y
#else	
	SF4|SF9,							/*<*/
#endif
#endif
#if defined(_FY2010_LCD_DISPLAY_)
	SF1|SFA|SF0|SF2|SFB,				/*[*/ // n
	SFA|SF5|SF7,					    /*]*///r
#elif defined(_SUPPORT_TIME_DISPLAY_)
	SF1|SF2|SFC|SF6|SFA|SFB,			/*[*///w
    SF1|SF2|SF0|SF6|SFA|SFB,			/*]*///m
#else
	SF0|SF1|SFA|SFC,					/*[*/
	SF0|SF2|SFB|SFC,					/*]*/
#endif
	SF3|SF4|SF5|SF6|SF7|SF8|SF9,		/***/
#if defined(_XM003_SPECIAL_DISPLAY_)||defined(_SUPPORT_TIME_DISPLAY_)
	SF1|SFA|SF0|SF2|SFB,				/*/*/ // n
#else
	SF4|SF8,							/*/*/
#endif
	SF3|SF9,							/*\*/
	SF0|SF1|SF2|SF3|SF4|SF5|SF6|SF7|SF8|SF9|SFA|SFB|SFC,/*#*/

#if defined(_SUPPORT_CHAR_RUSSIAN_)

	SFA|SF1|SF0|SF2|SFB|SF5|SF7,							/*	§¡ */
	SF0|SF1|SFA|SFB|SFC|SF5|SF7,							/*	§¢ */
	SF0|SF1|SF4|SF5|SF7|SFA|SFB|SFC,						/*	§£ */
	SF0|SF1|SFA,											/*	§¤ */
	SF2|SF4|SF8|SFB|SFC,									/*	§¥ */
	SF0|SF1|SF5|SF7|SFA|SFC,								/*	§¦ */
	SF1|SF2|SF5|SF6|SF7|SFA|SFB,							/*	§¨ */
	SF0|SF2|SF5|SF7|SFB|SFC,								/*	§© */
	SF1|SF2|SF8|SF4|SFA|SFB,								/*	§ª */
	SF0|SF1|SF2|SF8|SF4|SFA|SFB,							/*	§« */
	SF1|SFA|SF5|SF4|SF9,									/*	§¬ */
	SF2|SF4|SF8|SFB,										/*	§­ */
	SF1|SFA|SFB|SF2|SF3|SF4,								/*	§® */
	SF1|SF2|SF5|SF7|SFA|SFB,								/*	§¯ */
	SF0|SF1|SFA|SFB|SFC|SF2,								/*	§° */
	SF0|SF1|SF2|SFA|SFB,									/*	§± */

	SFA|SF1|SF5|SF7|SF2|SF0,								/*	§² */
	SF0|SF1|SFA|SFC,										/*	§³ */
	SF0|SF6,												/*	§´ */
	SF1|SF2|SF5|SF7|SFB|SFC,								/*	§µ */
	SF0|SF1|SF2|SF5|SF6|SF7,								/*	§¶ */
	SF3|SF4|SF8|SF9,										/*	§· */
	SF1|SF6|SFA|SFC,										/*	§¸ */
	SF1|SF5|SF7|SF2|SFB,									/*	§¹ */
	SF1|SF2|SF6|SFA|SFB|SFC,								/*	§º */
	SF1|SF2|SF6|SF9|SFA|SFB|SFC,							/*	§» */
	SF0|SF4|SF7|SF8|SFB|SFC,								/*	§¼ */
	SF1|SF2|SF5|SF9|SFA|SFB|SFC,							/*	§½ */
	SF1|SF5|SF9|SFA|SFC,									/*	§¾ */
	SF0|SF2|SF5|SF7|SFB|SFC,								/*	§¿ */
	SF1|SF2|SF4|SF5|SF9|SFA|SFB,							/*	§À */
	SF0|SF1|SF2|SF5|SF7|SF8|SFB,							/*	§Á */

	SF0|SF1|SF5|SF7|SFA|SFC,								/*	§§ */
#endif
};
#endif
#endif

#if defined(DEBGE_LED_AND_LCD_TOGETHER)
static const uint16 SEG13_FONT1[]=
{
	VFD_DIGITAL_0,
	VFD_DIGITAL_1,
	VFD_DIGITAL_2,
	VFD_DIGITAL_3,
	VFD_DIGITAL_4,
	VFD_DIGITAL_5,
	VFD_DIGITAL_6,
	VFD_DIGITAL_7,
	VFD_DIGITAL_8,
	VFD_DIGITAL_9,
	VFD_DIGITAL_A,
	VFD_DIGITAL_B,
	VFD_DIGITAL_C,
	VFD_DIGITAL_D,
	VFD_DIGITAL_E,
	VFD_DIGITAL_F,
	VFD_DIGITAL_G,
	VFD_DIGITAL_H,
	VFD_DIGITAL_I,
	VFD_DIGITAL_J,
	VFD_DIGITAL_K,
	VFD_DIGITAL_L,
	VFD_DIGITAL_M,
	VFD_DIGITAL_N,
	VFD_DIGITAL_O,
	VFD_DIGITAL_P,
	VFD_DIGITAL_Q,
	VFD_DIGITAL_R,
	VFD_DIGITAL_S,
	VFD_DIGITAL_T,
	VFD_DIGITAL_U,
	VFD_DIGITAL_V,
	VFD_DIGITAL_W,
	VFD_DIGITAL_X,
	VFD_DIGITAL_Y,
	VFD_DIGITAL_Z,
	VFD_DIGITAL_NULL,//¿Õ36
	VFD_DIGITAL_01,/*.*/
	VFD_DIGITAL_02,/*|*/
	VFD_DIGITAL_03,/*-*/
	VFD_DIGITAL_04,/*+*/
	VFD_DIGITAL_05,/*=*/
	VFD_DIGITAL_06,/*'*/
	VFD_DIGITAL_07,/*_*/
	VFD_DIGITAL_08,/*>*/
	VFD_DIGITAL_09,/*<*/
	VFD_DIGITAL_10,/*[*/
	VFD_DIGITAL_11,/*]*/
	VFD_DIGITAL_12,/***/
	VFD_DIGITAL_13,/*/*/
	VFD_DIGITAL_14,/*\*/
	VFD_DIGITAL_15,/*#*/
};
#endif

static const uint8 PsTextIndex[]=
{
	' ',
	'.',
	'|',
	'-',
	'+',
	'=',
	'\'',
	'_',
	'>',
	'<',
	'[',
	']',
	'*',
	'/',
	'\\',
	'#'
};

#define CHAR_0_ADDR			0
#define CHAR_A_ADDR			10
#define CHAR_SPF_ADDR		36
#define CHAR_SHOW_NULL		CHAR_SPF_ADDR


#if defined(_SUPPORT_CHAR_RUSSIAN_)

#define CHAR_RUSSIAN_ADDR					(CHAR_SPF_ADDR+sizeof(PsTextIndex))

#define CHAR_RUSSIAN_LENGTH1				(33)
#define CHAR_RUSSIAN_START1					(0xC0+0x00)
#define CHAR_RUSSIAN_ESPECIALLY1			(CHAR_RUSSIAN_START1+ CHAR_RUSSIAN_LENGTH1 - 1)
#define CHAR_RUSSIAN_END1					(CHAR_RUSSIAN_ESPECIALLY1)

#endif

static uint8 get_text_index(uint8 text)
{
	uint8 i;

	if((text > ('0'-1))&&(text < ('9'+1)))
	{
		text = (text - '0') + CHAR_0_ADDR;
	}
	else
	{
		if(text == 0)return CHAR_SPF_ADDR;

		for(i = 0;i < sizeof(PsTextIndex); i++)
		{
			if(text == PsTextIndex[i])
			{
				return (i + CHAR_SPF_ADDR);
			}
		}

    #if defined(_SUPPORT_CHAR_RUSSIAN_)
    	if(text>= CHAR_RUSSIAN_START1 && text<= CHAR_RUSSIAN_END1)
    	{
    		return (CHAR_RUSSIAN_ADDR+(text-CHAR_RUSSIAN_START1));
    	}
    #endif

		if((text > ('a'-1))&&(text < ('z'+1)))
		{
			text = (text - 'a' + 'A');
		}

		if((text > ('A'-1))&&(text < ('Z'+1)))
		{
			text=(text - 'A') + CHAR_A_ADDR;
		}
		else
		{
			text = CHAR_SHOW_NULL;
		}
	}

	return text;
}

