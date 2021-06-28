#ifndef __AMT630A_TASK_H__
#define __AMT630A_TASK_H__
#include "music.h"

#if defined(_USER_TFT_DISP_)

typedef enum{
	TFT_STATUS__OFF = 0x00,
	TFT_STATUS__ON,
	TFT_STATUS__SLOW,
	TFT_STATUS__NULL,
}enum_sys_status;//系统状态

typedef enum{
	MEDIA_SRC__IDLE = 0x00,
	MEDIA_SRC__RADIO,
	MEDIA_SRC__USB,
	MEDIA_SRC__SDCARD,
	MEDIA_SRC__BT,
	MEDIA_SRC__BTMUSIC,
	MEDIA_SRC__AUX,
	MEDIA_SRC__USB__LOADING,
	MEDIA_SRC__SD__LOADING,
	MEDIA_SRC__ASTERN,
	MEDIA_SRC__NULL,
}enum_Media;//媒体类型

typedef enum{
	AUDIO_EQ__NULL=-1,
	AUDIO_EQ__ON,
	AUDIO_EQ__OFF,
	AUDIO_EQ__CLASSIC,
	AUDIO_EQ__ROCK,
	AUDIO_EQ__POP,
	AUDIO_EQ__JAZZ,
	AUDIO_EQ__FLAT,
	AUDIO_EQ__ELECTRIC,
	AUDIO_EQ__CUSTOM,
}enum_EQ;

typedef enum{
    SET__OFF = 0x00,
    SET__ON,
}enum_onoff;

typedef enum{
	STYLE__NULL = -1,
	STYLE__STANDARD,
	STYLE__LIGHT,
	STYLE__SOFT,
	STYLE__BRIGHT,
	STYLE__CUSTOM,
}enum_Type;

typedef enum{
	BAND__FM1 = 0,
	BAND__FM2,
	BAND__FM3,
	BAND__AM1,
	BAND__AM2,
	BAND__MAX,
}enum_CurrentBand;

typedef enum{
	RADIO_STATE__IDLE = 0,
	RADIO_STATE__SEEK,
	RADIO_STATE__SCAN,
	RADIO_STATE__AUTO_SAVE,
	RADIO_STATE__BROWSE,
}enum_radio_state;

typedef enum{
	workINT=0,
	Connect,
	linkend,
}enum_BTWorkStates;

typedef enum{
	none=0,
	idleCall,
	incomingCall,
	outgoingCall,
	talking,
}enum_BTCallStates;

typedef enum{
	device__null = 0,
	device__host,
	device__phone,
}enum_BtCallDevice;

typedef enum{
	music__none,
	music__play,
	music__pause,
	music__stop,
}enum_BTMusicStates;

typedef enum{
	STATE__STOP = 0,
	STATE__PLAY,
	STATE__PAUSE,
	STATE__FORD,
	STATE__BACK,
	STATE__MAX,
}enum_paly_status;//播放状态

typedef enum{
	REPEAT__OFF = 0,
	REPEAT__ONE,
	REPEAT__DIR,
	REPEAT__ALL,
	REPEAT__MAX,
}enum_rpt;//重复播放状态

typedef enum{
	RANDOM__OFF = 0,
	RANDOM__ON,
	RANDOM__MAX,
}enum_rdm;//随机播放状态
typedef enum{
	INT__OFF = 2,
	INT__ON,
	INT__MAX,
}enum_int;//浏览播放状态

typedef struct{
    u8  head;     // 头码
    u8  cmd;      // 命令
    //-------------Panel,系统状态---------------------
	enum_sys_status  sys_status; //系统状态
    enum_Media       media;      //媒体类型
    //-------------audio音频信息----------------------
    u8 vol;                      //音量
    u8 mute;					 //静音
    u8 volmax;                   //音量范围  
	u8 fad_bal_max;			    //前后左右范围
	u8 bass_trebl_max;          //高低音音量范围
    enum_EQ EQ;                  //音效
    enum_onoff LOUD;             //响度
    u8 TREBL;                    //
    u8 MIDDLE;
    u8 BASS;
    u8 Blance;
    u8 Fad;
    //-------------TFT显示调节信息-------------------
    enum_Type Type;
    u8 BRIGHT;
    u8 CONTRAST;
    u8 CHORAM;
    u8 SATURATION;
    u8 SHARP;
    //-------------Radio信息-------------------------
    u8 FREQ_H;
    u8 FREQ_L;
    //-------------Radio Freq set--------------------
    enum_CurrentBand CurrentBand;
    u8 Index;
    u8 FREQ_INDEX_H;
    u8 FREQ_INDEX_L;
    //-------------Radio state--------------------
    enum_onoff  radio_loc;
    enum_onoff  radio_st;
    enum_onoff  radio_sto;
    enum_radio_state radio_state;
    //-------------Bluetooth信息--------------------
    enum_BTWorkStates BTWorkState;
    //-------------BTCallNumber--------------------
	u8 number; //tel 号码
	u8 len;  //号码长度
	u8 dial_flag;//拨号标志
    //-------------BTMusicStates/ BTCallStates--------------------
    enum_BTCallStates  BTCallStates;
    enum_BtCallDevice  BtCallDevice;
    enum_BTMusicStates BTMusicStates;
    //-------------Music播放信息--------------------
    u8 second ;         //(播放时间)
    u8 minute;          //(播放时间)
    u8 hour ;           //(播放时间)

    //-------------曲目信息--------------------
    u8 Index_h;         //当前曲目 Index 高位
    u8 Index_l;         //当前曲目 Index 低位
    u8 total_h;         //总曲目 total 高位
    u8 total_l;         //总曲目total 低位
    //-------------播放状态信息--------------------
    enum_paly_status  paly_status;//播放状态
    enum_rpt          REPEAT;     //重复播放状态
    enum_rdm          RANDOM;     //随机播放状态
    enum_int		  Int;		//浏览播放状态
    //CLOKC
    u8 hour1;		//时钟的小时
	u8 minute1;		//时钟的分钟
	u8 state;		//时钟显示状态(设置、显示)
	u8 select;		// 时钟设置选择(分钟、小时)
}TFT_DISP_t;//TFT显示方面的数据结构体

#define     SERVO_BUFFER_SIZE		  200
#define     LEN_MAX   				  100
typedef struct{
	u16 read;
	u16 write;
	u8  buffer[SERVO_BUFFER_SIZE];
    u8  receive_data[LEN_MAX];
	u8  flag;
	u16 inidx;
    u16 CheckSum;
    u16 data_Len;
}TFT_BUFFER_t;

#define  CMD_SYSTEM_STATUS      	  0x01 //系统状态设置及显示
#define  CMD_AUDIO_INFO      		  0x02 //audio信息显示及设置
#define  CMD_SET_SEL      		 	  0x03 //设置菜单选项
#define  CMD_TFT_DSIPLAY_INFO      	  0x04 //设置显示屏色调
#define  CMD_RADIO_FIRQ_DISPLAY       0x05 //收音频率显示
#define  CMD_RADIO_STATUS         	  0x06 //收音状态
#define  CMD_BT_CONNENCTE_STATUS      0x07 //蓝牙连接状态
#define  CMD_BT_NUMBER_DISPLAY        0x08 //蓝牙号码显示(拨打、接听、来电)
#define  CMD_BT_TEL_TIME_DISPLAY 	  0x09 //通话时间显示
#define  CMD_MUSIC_SONG_TIME_INFO     0x0a //播放歌曲信息显示
#define  CMD_MUSIC_SONG_STATUS        0x0b //播放状态设置及显示
#define  CMD_ID3_DISPLAY        	  0x0c //ID3显示
#define  CMD_CLOCK_DISPLAY        	  0x0d //时钟显示
#define  CMD_SEARCH_DISPLAY        	  0x0e //选曲显示

extern void TFTPushReceive(uint8 data);
extern uint8 TFTPopReceive(uint8 *data);
extern void TFTDriverServer(void);
extern void tft_disp_init(void);
extern void TFT_Tx_System_Status(void);
extern void TFT_Tx_Audio(void);
extern void TFT_Tx_Sel(uint8 SelType);
extern void TFT_Tx_Dsiplay(uint8 disp_type,uint8 chroma,uint8 bright,uint8 sharp,uint8 contrast);
extern void TFT_Tx_Radio_Info(uint16 freq);
extern void TFT_Tx_Radio_Status(void);
extern void TFT_Tx_BT_Status(uint8 connect_status,uint8 call_status,uint8 music_status,uint8 sco_status);
extern void TFT_Tx_BT_number_Display(uint8 *num,uint8 len,uint8 dial_flag);
extern void TFT_Tx_BT_Tel_Time(uint32 tel_time);
extern void TFT_Tx_Music_Clock_songe_Info(uint8 device,uint32 total_time,uint32 Curplaytime,uint32 total_trk,uint32 Cur_trk);
extern void TFT_Tx_Music_Status(uint8 state,uint8 mode);
extern void TFT_Tx_Clock_Display(uint8 hour,uint8 minute,uint8 state,uint8 select);
extern void TFT_Tx_Music_Search(uint16 Tune,uint32 total);
extern void TFT_Tx_ID3_Info_Display(music_play_info_t id3buff);


#endif
#endif /* __LINEIN_TASK_H__ */

