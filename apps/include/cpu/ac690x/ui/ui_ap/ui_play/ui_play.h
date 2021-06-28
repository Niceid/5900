#ifndef __UI_PLAY_H__
#define __UI_PLAY_H__
//here
enum{
	PLAY_SOURCE_NULL = 0,
	PLAY_SOURCE_USB,
	PLAY_SOURCE_SD,
};

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

#if defined(_SUPPORT_DISC_)
enum{
	PLAY_MEDIA_NULL = 0,
	PLAY_MEDIA_CD,
	PLAY_MEDIA_WMA,
	PLAY_MEDIA_ASF,
	PLAY_MEDIA_WAV,
	PLAY_MEDIA_MP3,
	PLAY_MEDIA_MP4,
	PLAY_MEDIA_VCD,
	PLAY_MEDIA_DVD,
	PLAY_MEDIA_JPG,
	PLAY_MEDIA_TXT,
	PLAY_MEDIA_UNKONW,
};
#else
enum{
	PLAY_MEDIA_MP3 = 0,
	PLAY_MEDIA_WMA,
	PLAY_MEDIA_WAV,
	PLAY_MEDIA_FLAC,
	PLAY_MEDIA_APE,
	PLAY_MEDIA_UNKONW,
};
#endif

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

enum{
	ID3_STATUS_INIT = 0,
	ID3_STATUS_DISP,
	ID3_STATUS_TRK_SCH,
#if defined(SUPPORT_NUM_SELECT_MUSIC)
	ID3_STATUS_SELECT_MUSIC,
#endif
};

enum{
	ID3_MODE_INIT = 0,
	ID3_MODE_RUN,
	ID3_MODE_EXIT,
};

/*----------------------------------------------------------------------------------------*/

enum{
	ID3_INIT_STATUS_LOAD = 0,
	ID3_INIT_STATUS_TOTAL,
	ID3_INIT_STATUS_DELAY,
	ID3_INIT_STATUS_PLAY,
	ID3_INIT_STATUS_END,
};

/*----------------------------------------------------------------------------------------*/

#if defined(_FIVE_CHAR_DISPLAY_)
#if defined(_SWL8809_DISPLAY_)
#define ID3_DISP_STATUS_CAHNGE							_B5_
#define ID3_DISP_STATUS_CAHNGE_CYCLE					_B4_
#else
#define ID3_DISP_STATUS_CAHNGE							_B4_
#define ID3_DISP_STATUS_CAHNGE_CYCLE					_B3_
#endif
#elif defined(_SEVEN_CHAR_DISPLAY_)
#define ID3_DISP_STATUS_CAHNGE							_B6_
#define ID3_DISP_STATUS_CAHNGE_CYCLE					_B5_
#elif defined(_XM003_SPECIAL_DISPLAY_)
#define ID3_DISP_STATUS_CAHNGE							_B6_
#define ID3_DISP_STATUS_CAHNGE_CYCLE					_B5_
#else
#define ID3_DISP_STATUS_CAHNGE							_B7_
#define ID3_DISP_STATUS_CAHNGE_CYCLE					_B6_
#endif

enum{
	ID3_DISP_TIME_MODE_NORMAL = 0,
	ID3_DISP_TIME_MODE_DOWN,
};

enum{
	ID3_DISP_TYPE_NULL = 0,
	ID3_DISP_TYPE_TITLE,
	ID3_DISP_TYPE_FAST,
	ID3_DISP_TYPE_FAST_DELAY,
	ID3_DISP_TYPE_ROLL,
	ID3_DISP_TYPE_END,
};

enum{
	ID3_DISP_STATUS_TIME = 0,
	ID3_DISP_STATUS_FILE_NAME,
	ID3_DISP_STATUS_SONG_NAME,
	ID3_DISP_STATUS_ARTIST,
	ID3_DISP_STATUS_ALBUM,
	ID3_DISP_STATUS_DIR,
	ID3_DISP_STATUS_MSG,
	ID3_DISP_STATUS_END,
};

/*----------------------------------------------------------------------------------------*/

enum{
	ID3_TRK_SCH_INPUT_NULL,
	ID3_TRK_SCH_INPUT_INC,
	ID3_TRK_SCH_INPUT_BUNT,
};
enum{
	ID3_TRK_SCH_STATUS_TITLE,
	ID3_TRK_SCH_STATUS_INPUT,
	ID3_TRK_SCH_STATUS_ERROR,
	ID3_TRK_SCH_STATUS_SEARCH,
	ID3_TRK_SCH_STATUS_END,
};

/*----------------------------------------------------------------------------------------*/
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
	uint16 	total;
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
}play_disp_info_t;

typedef struct{
	int sum;
	int	dir;
	int	cmd;
}play_input_t;

typedef struct{
	uint8 				Id;
	uint8 				NextId;
	uint8 				Mode;
	uint8				source;
	uint8				change_flag;
	
	play_disp_info_t	info;
	play_input_t		input;
		
#if defined(_ENABLE_DISK_DISC_TO_FLASH_)
	uint8				DiscTime;
	uint8				DiscState;
#endif

	union{
		struct{
			uint8 		Status;
			uint8 		Error;
		}Init;
		
		struct{
			uint8		Status;
			uint8		Mode;
			uint8		DispType;
			uint8		Change;
			uint16   	ChangeDelay;
			uint8		TitleBuf[VFD_DISP_LENGTH];	
			uint8		DispBuf[ID3_INFO_LENGTH];
			
			uint16  	Delay;
			
			uint8		MsgChange;
			uint8		MsgStatus;
			uint8		Msg[VFD_DISP_LENGTH];
			uint16		MsgTime;
			
			uint8		PlayState;
			uint16		Tune;
			uint8		Min;
			uint8		Sec;
		}Disp;
		
		struct{
			uint8 		Status;
			uint8 		Mode;
			uint16		Tune;
			
			uint8		Flash;
			uint8		Next;
			uint8		Change;
			uint8		AutoRun;
			uint16		OverTime;
		}Trk;
	}Ctrl;
}play_disp_t;


#define ID3_ID						(play_disp->Id)
#define ID3_ID_NEXT					(play_disp->NextId)
#define ID3_ID_SOURCE				(play_disp->source)

#define ID3_CTRL_INIT				(play_disp->Ctrl.Init)
#define ID3_CTRL_DISP				(play_disp->Ctrl.Disp)
#define ID3_CTRL_TRK				(play_disp->Ctrl.Trk)
#define ID3_CTRL_INFO				(play_disp->info)

#define ID3_POLLING_TIME			DISP_TYPE_POLLING
#define GET_ID3_DISP_DELAY()		(ID3_CTRL_DISP.Delay)
#define SET_ID3_DISP_DELAY(x)		(GET_ID3_DISP_DELAY() = (x)/ID3_POLLING_TIME)

#if defined(PICK_SONG_AUTO_ENTER)
#define ID3_SCH_OVER_TIME			(3500/ID3_POLLING_TIME)
#else
#define ID3_SCH_OVER_TIME			(6000/ID3_POLLING_TIME)
#endif

#define ID3_SCH_AUTO_RUN_TIME		(3000/ID3_POLLING_TIME)

#endif  /*  __UI_PLAY_H__  */

