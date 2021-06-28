#ifndef __DISC_STATE_H__
#define __DISC_STATE_H__

#include "easyway_config.h"
#include "typedef.h"


#if defined(_SUPPORT_DISC_)


typedef enum{
	SERVO_TYPE_NULL = 0,
	SERVO_TYPE_STATUS,
	SERVO_TYPE_ORDER,
	SERVO_TYPE_VFD,
#if defined(TD_ADD_UPDATA)
	SERVO_TYPE_UPDATA,
#endif
}SERVO_TYPE_e;

typedef enum{
	SERVO_STATUS_TYPE_SOURCE = 0,
	SERVO_STATUS_TYPE_MODE,
	SERVO_STATUS_TYPE_SEARCH,
	SERVO_STATUS_TYPE_MEDIA,
	SERVO_STATUS_TYPE_ERROR,
}SERVO_STATUS_TYPE_e;

/*-------------------------------------------------------------------------------------------------------------------*/

typedef enum{
	SERVO_SOURCE_NULL = 0,
	SERVO_SOURCE_DISC,
	SERVO_SOURCE_USB,
	SERVO_SOURCE_CARD,	
}SERVO_SOURCE_e;

/*-------------------------------------------------------------------------------------------------------------------*/

typedef enum{
	SERVO_STATUS_IDLE = 0,
	SERVO_STATUS_PLAYE,
	SERVO_STATUS_PAUSE,
	SERVO_STATUS_STOP,
	SERVO_STATUS_FORWARD,
	SERVO_STATUS_BACKWARD,
	SERVO_STATUS_UPGRADE,
}SERVO_STATUS_e;

/*-------------------------------------------------------------------------------------------------------------------*/

typedef enum{
	SERVO_MODE_REPEAT_OFF = 0,
	SERVO_MODE_REPEAT_ONE,
	SERVO_MODE_REPEAT_DIR,
	SERVO_MODE_REPEAT_ALL,
	SERVO_MODE_SHUFFLE_ON,
	SERVO_MODE_INTRO_ON,
	SERVO_MODE_ERROR,
	SERVO_MODE_END,
}SERVO_MODE_e;

/*-------------------------------------------------------------------------------------------------------------------*/

typedef enum{
	SERVO_SEARCH_MODE_INIT = 0,
	SERVO_SEARCH_MODE_NORMAL,
	SERVO_SEARCH_MODE_TRK_SCH,
	SERVO_SEARCH_MODE_FILE_SCH,
	SERVO_SEARCH_MODE_DIR_SCH,	
}SERVO_SEARCH_MODE_e;
//	init status
typedef enum{
	SERVO_SEARCH_STATUS_INIT_LOAD,
	SERVO_SEARCH_STATUS_INIT_MEDIA,
	SERVO_SEARCH_STATUS_INIT_TOTAL,
	SERVO_SEARCH_STATUS_INIT_DELAY,
	SERVO_SEARCH_STATUS_INIT_PLAY,
	SERVO_SEARCH_STATUS_INIT_END,
}SERVO_SEARCH_STATUS_INIT_e;
//	disp status
typedef enum{
	SERVO_SEARCH_STATUS_DISP_TIME = 0,
	SERVO_SEARCH_STATUS_DISP_FILE_NAME,
	SERVO_SEARCH_STATUS_DISP_SONG_NAME,
	SERVO_SEARCH_STATUS_DISP_ARTIST,
	SERVO_SEARCH_STATUS_DISP_ALBUM,
	SERVO_SEARCH_STATUS_DISP_DIR,
	SERVO_SEARCH_STATUS_DISP_MSG,
}SERVO_SEARCH_STATUS_DISP_e;
//	track search status	
typedef enum{
	SERVO_SEARCH_STATUS_TRK_SCH_TITLE = 0,
	SERVO_SEARCH_STATUS_TRK_SCH_INPUT,
	SERVO_SEARCH_STATUS_TRK_SCH_ERROR,
	SERVO_SEARCH_STATUS_TRK_SCH_SEARCH,
	SERVO_SEARCH_STATUS_TRK_SCH_END,
}SERVO_SEARCH_STATUS_TRK_SCH_e;
//	file search status
typedef enum{
	SERVO_SEARCH_STATUS_FILE_SCH_TITLE = 0,
	SERVO_SEARCH_STATUS_FILE_SCH_INPUT,
	SERVO_SEARCH_STATUS_FILE_SCH_SEARCH,
	SERVO_SEARCH_STATUS_FILE_SCH_ERROR,
	SERVO_SEARCH_STATUS_FILE_SCH_SELECT,
	SERVO_SEARCH_STATUS_FILE_SCH_PLAYER,
	SERVO_SEARCH_STATUS_FILE_SCH_END,
}SERVO_SEARCH_STATUS_FILE_SCH_e;
//	dir search status
typedef enum{
	SERVO_SEARCH_STATUS_DIR_SCH_TITLE = 0,
	SERVO_SEARCH_STATUS_DIR_SCH_DIR_SELECT, 
	SERVO_SEARCH_STATUS_DIR_SCH_FILE_SELECT,
	SERVO_SEARCH_STATUS_DIR_SCH_PLAYER,
	SERVO_SEARCH_STATUS_DIR_SCH_END,
}SERVO_SEARCH_STATUS_DIR_SCH_e;

/*-------------------------------------------------------------------------------------------------------------------*/

typedef enum{
	SERVO_MEDIA_NULL = 0,
	SERVO_MEDIA_CD,
	SERVO_MEDIA_WMA,
	SERVO_MEDIA_ASF,
	SERVO_MEDIA_WAV,
	SERVO_MEDIA_MP3,
	SERVO_MEDIA_MP4,
	SERVO_MEDIA_VCD,
	SERVO_MEDIA_DVD,
	SERVO_MEDIA_JPG,
	SERVO_MEDIA_TXT,
	SERVO_MEDIA_UNKONW,
}SERVO_MEDIA_e;

/*-------------------------------------------------------------------------------------------------------------------*/

typedef enum{
	SERVO_ERROR_NULL,
	SERVO_ERROR_DRIVER,
	SERVO_ERROR_FILE,
	SERVO_ERROR_NO_FILE,	
}SERVO_ERROR_e;

/*-------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------*/

typedef struct{
	uint8 source;
	uint8 mode;
	uint8 search;
	uint8 media;
	uint8 error;
}__attribute__ ((packed)) PLAYER_STATUS_t;


typedef struct{
	char *name;
	uint8 normal;
	SERVO_SOURCE_e 	source;
	SERVO_STATUS_e 	status;
	SERVO_MODE_e 	mode;
	SERVO_SEARCH_MODE_e search_mode;
	uint8 search_status;
	SERVO_MEDIA_e 	media;
	SERVO_ERROR_e 	error;
	uint8 esp_flag;
	uint8 vfd_buffer[20];
}PLAYER_STATE_t;


#define GET_SERVO_STATUS(x)						((PLAYER_STATUS_t *)(x))
#define GET_SERVO_STATUS_ESP(x)					((GET_SERVO_STATUS(x)->source) >> 4)
#define GET_SERVO_STATUS_SOURCE(x)				((GET_SERVO_STATUS(x)->source) & 0x0F)
#define GET_SERVO_STATUS_STATUS(x)				((GET_SERVO_STATUS(x)->mode) >> 4)
#define GET_SERVO_STATUS_MODE(x)				((GET_SERVO_STATUS(x)->mode) & 0x0F)
#define GET_SERVO_STATUS_SEARCH_MODE(x)			((GET_SERVO_STATUS(x)->search) >> 4)
#define GET_SERVO_STATUS_SEARCH_STATUS(x)		((GET_SERVO_STATUS(x)->search) & 0x0F)
#define GET_SERVO_STATUS_MEDIA(x)				(GET_SERVO_STATUS(x)->media)
#define GET_SERVO_STATUS_ERROR(x)				(GET_SERVO_STATUS(x)->error)	


extern void disc_state_receive(void);
extern void disc_state_init(char *);
extern void disc_state_exit(void);

extern uint8 player_get_normal(void);

#endif


#endif/*__DISC_H__*/

