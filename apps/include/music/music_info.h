#ifndef __MUSIC_INFO_H__
#define __MUSIC_INFO_H__


#include "typedef.h"
#include "dec/music_api.h"
#include "fat/tff.h"

#ifdef  _SUPPORT_ID3_
typedef struct _ID3V2_PARSE
{
    u8 isv34;
    u8 unsync;
    u8 tunsync;
    u8 id3v2_version;
    u8 id2v3_flags;
    u8 taghdrlen;
    s32 id3v2_len;				// ID3 总长度, 但不包帧头和Footer preset
    s32 tlen;					// 当前ID3 frame长度，不包含帧头
}ID3V2_PARSE;
typedef struct _MP3_ID3_1
{
    u8 *id3_buf;
    u32 id3_len;
    u32 id3_ptr;
    u32 id3_e_ptr;
    u8  have_id3;
    u8  encode_flag;
    u8 *title;
    u8  title_len;
    u8 *artist;
    u8  artist_len;
    u8 *album;
    u8  album_len;
}MP3_ID3_INFO;
extern MP3_ID3_INFO  *mp3_head;
extern MP3_ID3       id3_v1_head;
extern MP3_ID3       id3_v2_head;
#define ID3_DISP_BUF_LEN            128
#define ID3v2_HEADER_SIZE 10
#define  SHOW_BUFF_LEN    512
#define  NUM_A   6
extern FS_DISP_NAME music_name_buf;//buf need malloc
extern void get_music_tag(void);
extern void get_patch_info(void );
#endif

extern u32 music_playfile_total_time;
extern u32 music_cur_filenum,music_cur_filetotal,music_dir_number;

#if defined(_ENABLE_FOLD_TOTAL_)
extern u32 music_dir_total;
#endif
void music_file_info(MUSIC_OP_API *m_op_api);
u8 get_floder_name(char* ptr,u32 filecount);
void get_floder_totalfile(u32 filecount);
void * music_get_file_id3(u32 len);

#endif

