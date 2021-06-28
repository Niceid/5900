#ifndef __MUSIC_H__
#define __MUSIC_H__


#define MUSIC_DEBUG
#ifdef MUSIC_DEBUG
#define music_printf 		ew_printf
#define music_puts 			ew_puts
#else
#define music_printf(...)
#define music_puts(...)
#endif

#define MUSIC_POLLING_TIME		(500)


/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/

typedef enum{
    SD0_DEVICE,
    SD1_DEVICE,
    USB_DEVICE,
    NO_DEVICE = 0xff,
}UI_DEVICE;

enum{
	PLAY_ERROR_NULL = 0,
	PLAY_ERROR_DRIVER,
	PLAY_ERROR_INIT,
	PLAY_ERROR_NO_FILE,
	PLAY_ERROR_FILE,
	PLAY_ERROR_PLAYER,
	PLAY_ERROR_FW,
	PLAY_ERROR_OTHER,
};

enum{
	PLAY_STATUS_IDLE = 0,
	PLAY_STATUS_PLAYE,
	PLAY_STATUS_PAUSE,
	PLAY_STATUS_STOP,
	PLAY_STATUS_FORWARD,
	PLAY_STATUS_BACKWARD,
};

enum{
	PLAY_MODE_REPEAT_OFF = 0,
	PLAY_MODE_REPEAT_ONE,
	PLAY_MODE_REPEAT_FOLDER,
	PLAY_MODE_REPEAT_ALL,
	PLAY_MODE_SHUFFLE,
	PLAY_MODE_INTRO,
};

enum{
	PLAY_MEDIA_MP3 = 0,
	PLAY_MEDIA_WMA,
	PLAY_MEDIA_WAV,
	PLAY_MEDIA_FLAC,
	PLAY_MEDIA_APE,
	PLAY_MEDIA_UNKONW,
};

enum{
	PLAY_MODE_DISP_REPEAT_OFF = 0,
	PLAY_MODE_DISP_REPEAT_ALL,
	PLAY_MODE_DISP_REPEAT_ONE,
	PLAY_MODE_DISP_REPEAT_FOLDER,
	PLAY_MODE_DISP_SHUFFLE_OFF,
	PLAY_MODE_DISP_SHUFFLE_ON,
	PLAY_MODE_DISP_INTRO_OFF,
	PLAY_MODE_DISP_INTRO_ON,
};

enum{
	PLAY_CMD_NULL = 0,
	PLAY_CMD_0,
	PLAY_CMD_1,
	PLAY_CMD_2,
	PLAY_CMD_3,
	PLAY_CMD_4,
	PLAY_CMD_5,
	PLAY_CMD_6,
	PLAY_CMD_7,
	PLAY_CMD_8,
	PLAY_CMD_9,
	PLAY_CMD_DELETE,
	PLAY_CMD_ENTER,

	PLAY_CMD_SEL_ADD,
	PLAY_CMD_SEL_SUB,
	PLAY_CMD_DIR_UP,
	PLAY_CMD_DIR_DOWN,
	PLAY_CMD_MODE,
	PLAY_CMD_SEARCH,
	PLAY_CMD_SEARCH_DELETE,
	PLAY_CMD_ID3,

	PLAY_CMD_MAX,
};

#if defined(_SUPPORT_ID3_)
enum{
	PLAY_ID3_FILE = 0,
	PLAY_ID3_FOLDER,
	PLAY_ID3_TITLE,
	PLAY_ID3_ARTIST,
	PLAY_ID3_ALBUM,
	PLAY_ID3_MAX,
};
#endif

typedef struct{
	uint8 	state;
	uint16 	total;//文件总数
	uint16 	tune;
	uint8 	min;
	uint8 	sec;
	uint8 	mode;
	uint8	error;
	uint8	media;
	uint8	change;
#if defined(_SUPPORT_ID3_)
	uint8	*id3_info[PLAY_ID3_MAX];
	uint32	id3_len[PLAY_ID3_MAX];
#endif
    uint8	play_flag;
	uint8	pause_flag;
	uint8 	stop_flag;
}music_play_info_t;

typedef struct{
	music_play_info_t play_info;
	uint8 ffr_state;
	uint8 intro_flag;
	uint8 intro_timer;
}music_ctrl_t;

struct music_cfg{
	const char *dev;
};

extern u8 play_mode;
extern u8 eq_mode;
extern void *music_reverb;
extern UI_DEVICE music_device;
#ifdef	TD_ADD_NEW_APP
extern u8 read_list_ready_flag;
extern void BtAppComInit(void);
extern void recv_get_list_cmd(u8 data1,u8 data2,u8 data3,u8 data4,u8 data5);
extern void BtComSendPoll(void);
extern void MusicListPackSend(void);
extern void file_brose_close(void);
extern void compare_play_mode(void);
#endif

#endif /* __MUSIC_H__ */

