#ifndef __AP3768_H__
#define __AP3768_H__

#if defined(_AUDIO_DRIVER_AP3768_)

extern void AudioPushReceive(u8 data);
extern u8 AudioPopReceive(u8 *data);
extern void AudioDriverServer(void);
extern void audio_disp_init(void);
extern void Audio_Send_Data(u8 *data,u8 length);

#endif

typedef enum 
{
	EX_DSP_EQ_TYPE_PEAK,
	EX_DSP_EQ_TYPE_LS,
	EX_DSP_EQ_TYPE_HS,
	EX_DSP_EQ_TYPE_BLPF1,
	EX_DSP_EQ_TYPE_BHPF1,
	EX_DSP_EQ_TYPE_BLPF2,
	EX_DSP_EQ_TYPE_BHPF2,
	EX_DSP_EQ_TYPE_HC,
	EX_DSP_EQ_TYPE_LC,
	EX_DSP_EQ_TYPE_MAX,
}EX_DSP_EQ_TYPE;


typedef enum 
{
	EX_EFFECT_EQ_STAND,			//标准
	EX_EFFECT_EQ_JAZZ,			//爵士
	EX_EFFECT_EQ_POP,			//流行
	EX_EFFECT_EQ_CLASS,			//古典
	EX_EFFECT_EQ_PEOPLE,		//人声
	EX_EFFECT_EQ_SOFT,			//柔和
	EX_EFFECT_EQ_ROCK,			//摇滚
	EX_EFFECT_EQ_METAL,			//重金属
	EX_EFFECT_EQ_ELECTRIC,
	EX_EFFECT_EQ_MAX,
}EX_DSP_EQ_EFFECT;

#define EQ_BAND_MAX				31//最大31

typedef struct  {
	EX_DSP_EQ_EFFECT name;
	uint8 val[31];
	uint32 freq;
}EXT_EQ_EFFECT ;	

typedef struct{
/*控制*/
	uint8 	mute_last;
#if defined(_SUPPORT_SUBWOOFER_)
	uint8	subwoofer_last;
#endif

/*状态*/
	uint8	mute;
	uint8	channel;
	uint8	loud;
	uint8	gate;
	uint8 	volume;
	int8 	bass;
	int8 	treble;
#if defined(_FAD_BAL_FUNTION_)
	int8	fl;
	int8	fr;
	int8	rl;
	int8	rr;
#else
	int8 	lr;
	int8 	fr;
#endif
	uint8 	eq;
#if defined(_ENABLE_AP3768_SUBWOOFER_)
	uint8	subwoofer1;
	uint8	subwoofer2;
#endif
	EXT_EQ_EFFECT eq_mode;
}ap3768_audio_t;

typedef struct  
{
	u8 bandwidth[EQ_BAND_MAX];//(1~120)--q*10
	u8 gain[EQ_BAND_MAX];//(-320~120)--0x000038(-5.6db )--0x010038(+5.6db)
}EQ_INFO;		
#if defined(TD_ADD_NEW_APP)
#define VM_AP3768_TITLE				0x00256312
typedef struct
{ 
    uint8 ap3768_dsp_type[31];//dsp类型
    uint8 ap3768_memory1[31][2];//31个频段对应的增益 频宽 
    uint8 ap3768_memory2[6][3];//6个输出通道的输出音量 延迟 反向
}ap3768_config_t;

typedef struct{
	uint32				title;
	ap3768_config_t 	ap3768;
	uint32				check;
}wm_ap3768_t;
extern void ap3768_memory_init(void);
extern void ap3768_info_to_app(void);
extern void ap3768_reset(void);
extern void uart_send_user_data(void);
extern void uart_send_user_data1(void);
#endif
#endif //__AP3768_H__


