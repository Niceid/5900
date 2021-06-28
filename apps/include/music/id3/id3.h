#ifndef __ID3_H__
#define __ID3_H__


#define ID3_DEBUG
#ifdef ID3_DEBUG
#define id3_printf 		ew_printf
#define id3_puts 		ew_puts
#else
#define id3_printf(...)
#define id3_puts(...)
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

//end
//ID3获取支持类型
typedef enum
{
    ID3_DECODE_MP3 = 0,
    ID3_DECODE_WMA,
    ID3_DECODE_FLAC,
    ID3_DECODE_MAX,
} id3_decode_e;

typedef enum
{
    ID3_TYPE_TITLE = 0,	//文件标题
    ID3_TYPE_ARTIST,	//艺术家
    ID3_TYPE_ALBUM,		//专辑
    ID3_TYPE_MAX,
} id3_type_e;

//获取ID3的信息结构
typedef struct
{
    uint8 	*buf[ID3_TYPE_MAX];
    uint32 	len[ID3_TYPE_MAX];
}id3_info_t;

typedef struct
{
    uint8 	*buf;
    uint32 	len;
}file_info_t;

typedef struct
{
	id3_decode_e 	decode;
    id3_info_t 		*id3_info;
	const uint8 	*id3_prt;
	uint32 			id3_len;
}id3_t;

typedef struct{
	uint16 			id3_check; 				//查找ID3要素标记
	const uint8 	*id3_data; 				//ID3 数据缓存
	uint32 			id3_len;				//ID3 数据空间长度
	uint32 			id3_pos; 				//ID3 数据偏移
	id3_info_t 		*id3_info; 				//提供的存储结构
	uint8			id3_type[ID3_TYPE_MAX];	//id3 数据格式
}id3_ctrl_t;


#if defined(_MUSICID3_ADJUST_)
extern u8(*id3_tmp_buf)[256];
#else
extern u8 id3_tmp_buf[5][256];
#endif

uint8 music_file_id3_file(uint8 **name, uint32 *len);
uint8 music_file_id3_folder(uint8 **name, uint32 *len);
uint8 music_file_id3_title(uint8 **name, uint32 *len);
uint8 music_file_id3_artist(uint8 **name, uint32 *len);
uint8 music_file_id3_album(uint8 **name, uint32 *len);


void music_file_id3_state(uint8 on_off);
void music_file_info(MUSIC_OP_API *m_op_api);
void music_file_id3(MUSIC_OP_API *m_op_api);


#endif /* __ID3_H__ */

