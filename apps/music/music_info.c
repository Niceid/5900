#include "sdk_cfg.h"
#include "dec/decoder_phy.h"
#include "music.h"

u32 music_playfile_total_time;
u32 music_cur_filenum,music_cur_filetotal,music_dir_number;
u32 play_file_clust;

#if defined(_ENABLE_FOLD_TOTAL_)
u32 music_dir_total;
#endif

extern u32 get_decode_time(void);

const decoder_ops_t *(*find_dec_ops[])(void) AT(.const_tab);
const decoder_ops_t *(*find_dec_ops[])(void) =
{
#if DEC_TYPE_SBC_ENABLE
	(void*)get_sbcd_ops,
#endif

#if DEC_TYPE_WAV_ENABLE
	(void*)get_wav_ops,
#endif

#if DEC_TYPE_WMA_ENABLE
	(void*)get_wma_ops,
#endif

#if DEC_TYPE_AAC_ENABLE
	(void*)get_aac_ops,
#endif

#if DEC_TYPE_FLAC_ENABLE
    (void*)get_flac_ops,
#endif

#if DEC_TYPE_APE_ENABLE
    (void*)get_ape_ops,
#endif

#if DEC_TYPE_MP3_ENABLE
	(void*)get_mp3_ops,
#endif

#if DEC_TYPE_F1A_ENABLE
	(void*)get_f1a_ops,
#endif
};

#define MP3_FFFR_ENABLE 	1
#define SBC_FFFR_ENABLE 	1
#define WAV_FFFR_ENABLE 	1
#define WMA_FFFR_ENABLE 	1
#define AAC_FFFR_ENABLE 	1
#define FLAC_FFFR_ENABLE 	1	//FLAC暂不支持快进快退
#define APE_FFFR_ENABLE 	1
#define F1A_FFFR_ENABLE 	0

const u8 dec_type_fffr_enable[] = {
#if DEC_TYPE_SBC_ENABLE
	SBC_FFFR_ENABLE,
#endif

#if DEC_TYPE_WAV_ENABLE
	WAV_FFFR_ENABLE,
#endif

#if DEC_TYPE_WMA_ENABLE
	WMA_FFFR_ENABLE,
#endif

#if DEC_TYPE_AAC_ENABLE
	AAC_FFFR_ENABLE,
#endif

#if DEC_TYPE_FLAC_ENABLE
    FLAC_FFFR_ENABLE,
#endif

#if DEC_TYPE_APE_ENABLE
    APE_FFFR_ENABLE,
#endif

#if DEC_TYPE_MP3_ENABLE
	MP3_FFFR_ENABLE,
#endif

#if DEC_TYPE_F1A_ENABLE
	F1A_FFFR_ENABLE,
#endif
};

const char dec_file_ext[][3] =
{
#if DEC_TYPE_MP3_ENABLE
    {"MP1"},
    {"MP2"},
    {"MP3"},
#endif // DEC_TYPE_MP3_ENABLE

#if DEC_TYPE_WMA_ENABLE
    {"WMA"},
#endif // DEC_TYPE_WMA_ENABLE

#if DEC_TYPE_WAV_ENABLE
    {"WAV"},
#endif // DEC_TYPE_WAV_ENABLE

#if DEC_TYPE_FLAC_ENABLE
    {"FLA"},
#endif // DEC_TYPE_FLAC_ENABLE

#if DEC_TYPE_APE_ENABLE
    {"APE"},
#endif // DEC_TYPE_APE_ENABLE

#if DEC_TYPE_F1A_ENABLE
    {"F1A"},
#endif // DEC_TYPE_APE_ENABLE

#if MUSIC_DECRYPT_EN
    {"SMP"},
#endif //SMP_FILE
    {'\0'},
};


u32 get_find_dec_ops_type_cnt(void)//call by dec_lib
{
    return (sizeof(find_dec_ops)/sizeof(find_dec_ops[0]));
}

void set_flac_sys_freq(u8 flag)
{
}

void *get_break_point_a(void *buf)
{
	return NULL;
}

void clean_ab_rpt(void)
{
}

u8 get_floder_name(char* ptr,u32 filecount)
{
    if(strcmp(ptr,"JL_REC     ") == 0)
    {
        music_printf("---ptr %s--\n",ptr);
        music_printf("---filecount %d\n",filecount);
        return 1;
    }

    return 0;
}

void get_floder_totalfile(u32 filecount)
{
    music_printf("---totalfile %d\n",filecount);
}
