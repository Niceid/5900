#include "sdk_cfg.h"
#include "dec/music_api.h"
#include "id3/id3.h"
#include "id3/id3_conv.h"
#include "music_info.h"
#include "file_operate/file_bs_deal.h"
#include "rcsp/rcsp_head.h"
#include "uicon/ui_con.h"
#include "setup.h"

u8 Get_id3Info_err=TRUE;

static uint8 id3_get_info[ID3_TYPE_MAX][2] =
{
//	读取开关,	最大长度
	{ 1,			100	},		//文件标题
	{ 1,			100	},		//作者
	{ 1,			100	},		//专辑
};

#define FILE_NAME_MAX			(100)
#define FOLDER_NAME_MAX			(100)

static uint8 *id3_buf_prt = NULL;
static uint32 id3_buf_len = 0;
static id3_info_t *id3_info = NULL;
static file_info_t *file_name_info = NULL;
static file_info_t *folder_name_info = NULL;

/*-----------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*
static void *music_get_file_id3(u32 len)
{
    uint8 *id3_buf = NULL;

    if(len <= 3*1024)
    {
        id3_buf = malloc(len);
		id3_buf_prt = id3_buf;
		id3_buf_len = len;
    }
	else
	{
		id3_buf_prt = NULL;
		id3_buf_len = 0;
	}

	id3_printf("music_get_file_id3 len = %x byte\n",len);
    id3_printf("music_get_file_id3 malloc = 0x%x\n",id3_buf);
    return id3_buf;
}

*/

void * music_get_file_id3(u32 len)
{
    u8* id3_buf = NULL;

	if(len < 6*1024)
		id3_buf = malloc(len);

    id3_printf("id3------len = %d byte\n",len);
	if(id3_buf != NULL)
	    id3_printf("id3_buf malloc = %x byte----\n",id3_buf);
	else
		id3_printf("id3_buf is NULL!!!");
/*
     if(id3_buf != NULL)
     {
          for(i = 0;i < 250;i++)
          {
              printf("%2x ",id3_buf[i]);
          }
     }
*/
    return id3_buf;
}


void music_file_id3_state(uint8 on_off)
{
	int i;

	if(on_off)
	{
		id3_puts("music_file_id3_state on\n");

		reg_get_id3v2_buf(music_get_file_id3);

		if(id3_info == NULL)
		{
			//id3_info = (id3_info_t *)malloc(sizeof(id3_info_t));
			id3_info = malloc_fun(id3_info, sizeof(id3_info_t), 0);
			ASSERT(id3_info);
			if(id3_info != NULL)
			{
				memset(id3_info, 0, sizeof(id3_info_t));

				for(i = 0; i < ID3_TYPE_MAX; i++)
				{
					if(id3_get_info[i][0])
					{
						//id3_info->buf[i] = (uint8 *)malloc(id3_get_info[i][1]);
						id3_info->buf[i] = malloc_fun(id3_info->buf[i], (id3_get_info[i][1]), 0);
						ASSERT(id3_info->buf[i]);
					}
				}
			}
		}
		

		if(file_name_info == NULL)
		{
			//file_name_info = (file_info_t *)malloc(sizeof(file_info_t));
			file_name_info = malloc_fun(file_name_info, sizeof(file_info_t), 0);
			ASSERT(file_name_info);
			if(file_name_info != NULL)
			{
				memset(file_name_info, 0, sizeof(file_info_t));
				//file_name_info->buf = (uint8 *)malloc(FILE_NAME_MAX);
				file_name_info->buf = malloc_fun(file_name_info->buf, FILE_NAME_MAX, 0);
				ASSERT(file_name_info->buf);
			}
		}

		if(folder_name_info == NULL)
		{
			//folder_name_info = (file_info_t *)malloc(sizeof(file_info_t));
			folder_name_info = malloc_fun(folder_name_info, sizeof(file_info_t), 0);
			ASSERT(folder_name_info);
			if(folder_name_info != NULL)
			{
				memset(folder_name_info, 0, sizeof(file_info_t));
				//folder_name_info->buf = (uint8 *)malloc(FOLDER_NAME_MAX);
				folder_name_info->buf = malloc_fun(folder_name_info->buf, FOLDER_NAME_MAX, 0);
				ASSERT(folder_name_info->buf);
			}
		}
	}
	else
	{
		id3_puts("music_file_id3_state off\n");
	#if 0
		if(id3_info != NULL)
		{
			for(i = 0; i < ID3_TYPE_MAX; i++)
			{
				if(id3_info->buf[i] != NULL)
				{
					free(id3_info->buf[i]);
					id3_info->buf[i] = NULL;
				}
			}

			free(id3_info);
			id3_info = NULL;
		}

		if(file_name_info != NULL)
		{
			if(file_name_info->buf != NULL)
			{
				free(file_name_info->buf);
				file_name_info->buf = NULL;
			}

			free(file_name_info);
			file_name_info = NULL;
		}

		if(folder_name_info != NULL)
		{
			if(folder_name_info->buf != NULL)
			{
				free(folder_name_info->buf);
				folder_name_info->buf = NULL;
			}

			free(folder_name_info);
			folder_name_info = NULL;
		}
	#endif
	}
}


uint8 music_file_id3_file(uint8 **name, uint32 *len)
{
	if(file_name_info != NULL)
	{
		if(file_name_info->buf != NULL)
		{
			*name 	= file_name_info->buf;
			*len  	= file_name_info->len;
			return TRUE;
		}
	}
	return FALSE;
}

uint8 music_file_id3_folder(uint8 **name, uint32 *len)
{
	if(folder_name_info != NULL)
	{
		if(folder_name_info->buf != NULL)
		{
			*name 	= folder_name_info->buf;
			*len  	= folder_name_info->len;
			return TRUE;
		}
	}
	return FALSE;
}

#ifdef _SUPPORT_ID3_
void id3_buf_zero(u8 *buf,u8 len)
{
    u8 i = 0;
    while(len--){
        buf[i++] = 0;
    }
    return;
}
#endif

uint8 music_file_id3_title(uint8 **name, uint32 *len)
{
#ifdef _SUPPORT_ID3_
	if(mp3_head->have_id3)
	{
		if(mp3_head->title_len != 0)
		{
			ew_printf("mp3_head->title_len:%d \n",mp3_head->title_len);
			put_u32hex((u32)mp3_head->title);
			put_buf(mp3_head->title,mp3_head->title_len);
			if(mp3_head->encode_flag == UNICODE_LE)
			{
				mp3_head->title_len = decode_fs_name(mp3_head->title, mp3_head->title_len, mp3_head->title, eNAME_TYPE_UNICODE);
				mp3_head->title_len = filtrate_char_sub(mp3_head->title, eNAME_TYPE_ASCII);
			}
			else
			{
				mp3_head->title_len = decode_fs_name(mp3_head->title, mp3_head->title_len, mp3_head->title, eNAME_TYPE_ASCII);
				mp3_head->title_len = filtrate_char_sub(mp3_head->title, eNAME_TYPE_ASCII);
			}
			ew_printf("mp3_head->title_len:%d\n",mp3_head->title_len);
			id3_printf("----title:%s\n",mp3_head->title);
			
		#if defined(_SUPPORT_CHAR_RUSSIAN_)
			id3_buf_zero(mp3_head->title+mp3_head->title_len,254-mp3_head->title_len);
			*name = mp3_head->title;
			*len  = mp3_head->title_len;
		#else
			*name = mp3_head->title;
			*len  = mp3_head->title_len;
			{
				u8 i;
				u8 j =0 ;
				u8 flag = 0;
				u8 *name_temp = mp3_head->title;
				for(i=0;i<mp3_head->title_len;i++)
				{
					if(!(((((*(mp3_head->title+i))>47 )&&((*(mp3_head->title+i))<58)))
					||((((*(mp3_head->title+i))>64 )&&((*(mp3_head->title+i))<91)))
					||((((*(mp3_head->title+i))>96 )&&((*(mp3_head->title+i))<123)))
					))
					{
						flag++;
					}
					else
					{
						if(flag >1)
						{
							*(name_temp+j)= ' ';
							flag = 0;
							j++;
						}
						*(name_temp+j)= *(mp3_head->title+i);
						j++;
					}
				}
				mp3_head->title_len = j;
			}
			id3_buf_zero(mp3_head->title+mp3_head->title_len,254-mp3_head->title_len);
		#endif
			return TRUE;
		}
	}
	return FALSE;
#else
	if(id3_info != NULL)
	{
		if(id3_info->buf[ID3_TYPE_TITLE] != NULL)
		{
			*name	= id3_info->buf[ID3_TYPE_TITLE];
			*len	= id3_info->len[ID3_TYPE_TITLE];
			return TRUE;
		}
	}
	return FALSE;
#endif
}

uint8 music_file_id3_artist(uint8 **name, uint32 *len)
{
#ifdef _SUPPORT_ID3_
	if(mp3_head->have_id3)
	{
		if(mp3_head->artist_len != 0)
		{
			ew_printf("mp3_head->artist_len:%d\n",mp3_head->artist_len);
			put_u32hex((u32)mp3_head->artist);
			put_buf(mp3_head->artist,mp3_head->artist_len);
			if(mp3_head->encode_flag == UNICODE_LE)
			{
				mp3_head->artist_len = decode_fs_name(mp3_head->artist, mp3_head->artist_len, mp3_head->artist, eNAME_TYPE_UNICODE);
				mp3_head->artist_len = filtrate_char_sub(mp3_head->artist, eNAME_TYPE_ASCII);
			}
			else
			{
				mp3_head->artist_len = decode_fs_name(mp3_head->artist, mp3_head->artist_len, mp3_head->artist, eNAME_TYPE_ASCII);
				mp3_head->artist_len = filtrate_char_sub(mp3_head->artist, eNAME_TYPE_ASCII);
			}
			ew_printf("decode_mp3_head->artist_len:%d\n",mp3_head->artist_len);
		    id3_printf("----artist:%s\n",mp3_head->artist);
		#if defined(_SUPPORT_CHAR_RUSSIAN_)
			id3_buf_zero(mp3_head->artist+mp3_head->artist_len,254-mp3_head->artist_len);
			*name = mp3_head->artist;
			*len  = mp3_head->artist_len;
		#else
			*name = mp3_head->artist;
			*len  = mp3_head->artist_len;
			{
				u8 i;
				u8 j =0 ;
				u8 flag = 0;
				u8 *name_temp = mp3_head->artist;
				for(i=0;i<mp3_head->artist_len;i++)
				{
					if(!(((((*(mp3_head->artist+i))>47 )&&((*(mp3_head->artist+i))<58)))
					||((((*(mp3_head->artist+i))>64 )&&((*(mp3_head->artist+i))<91)))
					||((((*(mp3_head->artist+i))>96 )&&((*(mp3_head->artist+i))<123)))
					))
					{
						flag++;
					}
					else
					{
						if(flag >1)
						{
							*(name_temp+j)= ' ';
							flag = 0;
							j++;
						}
						*(name_temp+j)= *(mp3_head->artist+i);
						j++;
					}
				}
				mp3_head->artist_len = j;
			}
			id3_buf_zero(mp3_head->artist+mp3_head->artist_len,254-mp3_head->artist_len);
    	#endif
			return TRUE;
		}
	}
	return FALSE;
#else
	if(id3_info != NULL)
	{
		if(id3_info->buf[ID3_TYPE_ARTIST] != NULL)
		{
			*name	= id3_info->buf[ID3_TYPE_ARTIST];
			*len	= id3_info->len[ID3_TYPE_ARTIST];
			return TRUE;
		}
	}
	return FALSE;
#endif

}

uint8 music_file_id3_album(uint8 **name, uint32 *len)
{
#ifdef _SUPPORT_ID3_
	if(mp3_head->have_id3)
	{
		if(mp3_head->album_len != 0)
		{
			ew_printf("mp3_head->album_len:%d\n",mp3_head->album_len);
			put_u32hex((u32)mp3_head->album);
			put_buf(mp3_head->album,mp3_head->album_len);
			if(mp3_head->encode_flag == UNICODE_LE)
			{
				mp3_head->album_len = decode_fs_name(mp3_head->album, mp3_head->album_len, mp3_head->album, eNAME_TYPE_UNICODE);
				mp3_head->album_len = filtrate_char_sub(mp3_head->album, eNAME_TYPE_ASCII);
			}
			else
			{
				mp3_head->album_len = decode_fs_name(mp3_head->album, mp3_head->album_len, mp3_head->album, eNAME_TYPE_ASCII);
				mp3_head->album_len = filtrate_char_sub(mp3_head->album, eNAME_TYPE_ASCII);
			}
			ew_printf("mp3_head->album_len:%d\n",mp3_head->album_len);
			id3_printf("----album:%s\n",mp3_head->album);
			//put_buf(mp3_head->album,mp3_head->album_len);
		#if defined(_SUPPORT_CHAR_RUSSIAN_)
			id3_buf_zero(mp3_head->album+mp3_head->album_len,254-mp3_head->album_len);
			*name = mp3_head->album;
			*len  = mp3_head->album_len;
		#else
			*name = mp3_head->album;
			*len  = mp3_head->album_len;
			{
				u8 i;
				u8 j =0 ;
				u8 flag = 0;
				u8 *name_temp = mp3_head->album;
				for(i=0;i<mp3_head->album_len;i++)
				{
					if(!(((((*(mp3_head->album+i))>47 )&&((*(mp3_head->album+i))<58)))
					||((((*(mp3_head->album+i))>64 )&&((*(mp3_head->album+i))<91)))
					||((((*(mp3_head->album+i))>96 )&&((*(mp3_head->album+i))<123)))
					))
					{
						flag++;
					}
					else
					{
						if(flag >1)
						{
							*(name_temp+j)= ' ';
							flag = 0;
							j++;
						}
						*(name_temp+j)= *(mp3_head->album+i);
						j++;
					}
				}
				mp3_head->album_len = j;
			}
			id3_buf_zero(mp3_head->album+mp3_head->album_len,254-mp3_head->album_len);
    	#endif
			return TRUE;
		}
	}
	return FALSE;
#else
	if(id3_info != NULL)
	{
		if(id3_info->buf[ID3_TYPE_ALBUM] != NULL)
		{
			*name	= id3_info->buf[ID3_TYPE_ALBUM];
			*len	= id3_info->len[ID3_TYPE_ALBUM];
			return TRUE;
		}
	}
	return FALSE;
#endif
	
}
extern dec_inf_t * get_dec_inf_api(void *priv);
#define CLUS_READ_BIG_U32(a)   ((*((u8*)(a)) <<24) + (*((u8*)(a)+1)<<16) + (*((u8*)(a)+2)<<8) + *((u8*)(a)+3))

void music_file_info(MUSIC_OP_API *m_op_api)
{
	uint8 read_file_name = FALSE;
	uint8 read_folder_name = FALSE;

	if(file_name_info != NULL)
	{
		if(file_name_info->buf != NULL)
		{
			read_file_name = TRUE;
		}
	}

	if(folder_name_info != NULL)
	{
		if(folder_name_info->buf != NULL)
		{
			read_folder_name = TRUE;
		}
	}

	if((!read_file_name) && (!read_folder_name))return;

	void *buffer = malloc(128 + ALIGN_4BYTE(sizeof(LONG_FILE_NAME)) *2);
	if(buffer == NULL)return;

	FS_DISP_NAME music_info;
	music_info.tpath = buffer;
	music_info.file_name = (LONG_FILE_NAME *)(buffer + 128);
	music_info.dir_name = (LONG_FILE_NAME *)(buffer + 128 + ALIGN_4BYTE(sizeof(LONG_FILE_NAME)));

	music_info.tpath[0] = 0;
	music_info.file_name->lfn_cnt = 0;
    music_info.dir_name->lfn_cnt = 0;
	
    file_operate_ctl(FOP_GET_FILE_NAME, m_op_api->fop_api, 0, &music_info);
	//OSTimeDly(10);
    id3_printf("music_file_path:%s\n", music_info.tpath);
    play_file_clust = get_sclust(m_op_api->fop_api->cur_lgdev_info->lg_hdl->file_hdl);
    play_file_clust = CLUS_READ_BIG_U32(&play_file_clust);

    dec_inf_t * dec_info = get_dec_inf_api(m_op_api->dop_api->dec_api.phy_ops);

#if defined(TD_ADD_SPP)
	if((music_device == SD0_DEVICE)||(music_device == SD1_DEVICE))
	{
		if(dec_info->total_time == 0)
		{
			music_playfile_total_time = setup.sd_total_time;
		}
		else
		{
			music_playfile_total_time =  dec_info->total_time;
		}
	}
	else if(music_device == USB_DEVICE)
	{
		if(dec_info->total_time == 0)
		{
			music_playfile_total_time = setup.usb_total_time;
		}
		else
		{
			music_playfile_total_time =  dec_info->total_time;
		}
	}
#endif

	ew_printf("play_time111:%d\n",music_playfile_total_time);
	ew_printf("dec_info->total_time:%d\n",dec_info->total_time);
	
	LONG_FILE_NAME *file_name = music_info.file_name;
	LONG_FILE_NAME* dir_name = music_info.dir_name;
	const char *path_name = music_info.tpath;
	uint32 path_len = strlen(music_info.tpath);
//----------------------file_name----------------------

	if(file_name->lfn_cnt)
	{
		id3_puts("long_file_name\n");
		if(file_name->lfn_cnt >128)
		{
			goto music_file_path_len;
		}
		else
		{
			id3_printf("----len:%d\n",file_name->lfn_cnt);
			put_buf((uint8 *)file_name->lfn,file_name->lfn_cnt);
	
			//file_name->lfn_cnt = file_comm_long_name_fix((uint8 *)file_name->lfn, file_name->lfn_cnt);//增加结束符
		
			if(file_name->lfn_cnt)
			{
						
				file_name->lfn_cnt = decode_fs_name(file_name->lfn, file_name->lfn_cnt, file_name->lfn, eNAME_TYPE_UNICODE);
			
				file_name->lfn_cnt = filtrate_char_sub(file_name->lfn, eNAME_TYPE_ASCII);
				id3_printf("----file_name_length:%d\n",file_name->lfn_cnt);
				id3_printf("----file_name:%s\n",file_name->lfn);
			}
			while(path_len)
			{
				if(path_name[--path_len] == '/')break;
			}
		}
	}
	else if(path_len)
	{
	music_file_path_len:
		id3_puts("short_file_name\n");
		file_name->lfn_cnt = 0;
		while(path_len)
		{
			if(path_name[--path_len] == '/')break;
			file_name->lfn[file_name->lfn_cnt++] = path_name[path_len];
		}
		if(file_name->lfn_cnt)
		{
			uint16 i;
			uint16 j = file_name->lfn_cnt - 1;
			for(i = 0; i < file_name->lfn_cnt/2; i++,j--)
			{
				char temp = file_name->lfn[i];
				file_name->lfn[i] = file_name->lfn[j];
				file_name->lfn[j] = temp;
			}
			file_name->lfn[file_name->lfn_cnt] = '\0';
			file_name->lfn_cnt = decode_fs_name(file_name->lfn, file_name->lfn_cnt, file_name->lfn, eNAME_TYPE_ASCII);
			file_name->lfn_cnt = filtrate_char_sub(file_name->lfn, eNAME_TYPE_ASCII);
			id3_printf("----file_name_length:%d\n",file_name->lfn_cnt);
			id3_printf("----file_name:%s\n",file_name->lfn);
		}
	}

	if(read_file_name)
	{
		file_name_info->len = file_name->lfn_cnt;
		if(file_name_info->len > (FILE_NAME_MAX - 1))
		{
			file_name_info->len = (FILE_NAME_MAX - 1);
		}
		memcpy(file_name_info->buf, file_name->lfn, file_name_info->len);
		file_name_info->buf[file_name_info->len] = 0;
	}

//----------------------folder_name----------------------

	/*if(dir_name->lfn_cnt)
	{
		id3_puts("long_folder_name\n");
		dir_name->lfn_cnt = file_comm_long_name_fix((uint8 *)dir_name->lfn, dir_name->lfn_cnt);//增加结束符
		if(dir_name->lfn_cnt)
		{
			dir_name->lfn_cnt = decode_fs_name(dir_name->lfn, dir_name->lfn_cnt, dir_name->lfn, eNAME_TYPE_UNICODE);
			dir_name->lfn_cnt = filtrate_char_sub(dir_name->lfn, eNAME_TYPE_ASCII);
			id3_printf("----folder_name_length:%d\n",dir_name->lfn_cnt);
			id3_printf("----folder_name:%s\n",dir_name->lfn);
		}
		while(path_len)
		{
			if(path_name[--path_len] == '/')break;
		}
	}
	else*/
	if(path_len)
	{
		if(dir_name->lfn_cnt)
    	{
    		id3_puts("long_folder_name\n");
    		dir_name->lfn_cnt = file_comm_long_name_fix((uint8 *)dir_name->lfn, dir_name->lfn_cnt);//增加结束符
    		if(dir_name->lfn_cnt)
    		{
    			dir_name->lfn_cnt = decode_fs_name(dir_name->lfn, dir_name->lfn_cnt, dir_name->lfn, eNAME_TYPE_UNICODE);
    			dir_name->lfn_cnt = filtrate_char_sub(dir_name->lfn, eNAME_TYPE_ASCII);
    			id3_printf("----folder_name_length:%d\n",dir_name->lfn_cnt);
    			id3_printf("----folder_name:%s\n",dir_name->lfn);
    		}
    		while(path_len)
    		{
    			if(path_name[--path_len] == '/')break;
    		}
    	}
		else
		{
    		id3_puts("short_folder_name\n");
    		dir_name->lfn_cnt = 0;
    		while(path_len)
    		{
    			if(path_name[--path_len] == '/')break;
    			dir_name->lfn[dir_name->lfn_cnt++] = path_name[path_len];
    		}
    		if(dir_name->lfn_cnt)
    		{
    			uint16 i;
    			uint16 j = dir_name->lfn_cnt - 1;
    			for(i = 0; i < dir_name->lfn_cnt/2; i++,j--)
    			{
    				char temp = dir_name->lfn[i];
    				dir_name->lfn[i] = dir_name->lfn[j];
    				dir_name->lfn[j] = temp;
    			}
    			dir_name->lfn[dir_name->lfn_cnt] = '\0';
    			dir_name->lfn_cnt = decode_fs_name(dir_name->lfn, dir_name->lfn_cnt, dir_name->lfn, eNAME_TYPE_ASCII);
    			dir_name->lfn_cnt = filtrate_char_sub(dir_name->lfn, eNAME_TYPE_ASCII);
    			id3_printf("----folder_name_length:%d\n",dir_name->lfn_cnt);
    			id3_printf("----folder_name:%s\n",dir_name->lfn);
    		}
		}
	}
	else
	{
		strcpy(dir_name->lfn, "ROOT");
		dir_name->lfn_cnt = strlen(dir_name->lfn);
	}

	if(read_folder_name)
	{
		folder_name_info->len = dir_name->lfn_cnt;
		if(folder_name_info->len > (FOLDER_NAME_MAX - 1))
		{
			folder_name_info->len = (FOLDER_NAME_MAX - 1);
		}
		memcpy(folder_name_info->buf, dir_name->lfn, folder_name_info->len);
		folder_name_info->buf[folder_name_info->len] = 0;
	}

	free(buffer);
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

typedef uint8 (*id3_func_f)(id3_ctrl_t *);
extern uint8 get_id3_mp3(id3_ctrl_t *);
extern uint8 get_id3_wma(id3_ctrl_t *);
extern uint8 get_id3_flac(id3_ctrl_t *);

static const id3_func_f get_id3_func[ID3_DECODE_MAX] =
{
	get_id3_mp3,
	get_id3_wma,
	get_id3_flac,
};

static uint8 music_get_id3_info(id3_t *id3)
{
    if(id3 == NULL)
    {
        return FALSE;
    }

	if((id3->decode >= ID3_DECODE_MAX)||(id3->id3_info == NULL)||(id3->id3_prt == NULL)||(id3->id3_len == 0))
	{
		return FALSE;
	}

	id3_ctrl_t id3_ctrl;
	memset(&id3_ctrl, 0, sizeof(id3_ctrl_t));

	id3_ctrl.id3_data 	= id3->id3_prt;
	id3_ctrl.id3_len 	= id3->id3_len;
    id3_ctrl.id3_info 	= id3->id3_info;

	uint8 i;
    for(i = 0; i < ID3_TYPE_MAX; i++)
    {
        if((id3_ctrl.id3_info->buf[i] != NULL) && (id3_ctrl.id3_info->len[i] >= 2))
        {
            id3_ctrl.id3_check |= (1<<i); 					//置获取标志
            *((uint16 *)id3_ctrl.id3_info->buf[i]) = 0; 	//预填入结束符号
        }
    }

	uint16 id3_check = id3_ctrl.id3_check;
	uint8 ret_val = FALSE;
	id3_printf("music_id3_check:0x%x, id3_len:%d\n", id3_check, id3_ctrl.id3_len);

	if(id3_check)
	{
	    if(get_id3_func[id3->decode] != NULL)
		{
	        ret_val = get_id3_func[id3->decode](&id3_ctrl);
		}
	}

	if(ret_val)
	{
		id3_check ^= id3_ctrl.id3_check;

		for(i = 0; i < ID3_TYPE_MAX; i++)
		{
			if(id3_ctrl.id3_info->buf[i] != NULL)
			{
				if(id3_check & (1<<i))
				{
					id3_printf("123---%d---id3_len:%d, id3_type:%d\n", i, id3_ctrl.id3_info->len[i], id3_ctrl.id3_type[i]);
					id3_ctrl.id3_info->len[i] = decode_fs_name(id3_ctrl.id3_info->buf[i], id3_ctrl.id3_info->len[i], id3_ctrl.id3_info->buf[i], id3_ctrl.id3_type[i]);
				}
				else
				{
					id3_ctrl.id3_info->len[i] = 0;
				}
				id3_printf("456---%d---id3_len:%d\n", i, id3_ctrl.id3_info->len[i]);
			}
		}
	}

    return ret_val;
}

void music_file_id3(MUSIC_OP_API *m_op_api)
{
	if(id3_info != NULL)
	{
		uint8 decode = ID3_DECODE_MAX;
		char *media_name = m_op_api->dop_api->dec_api.phy_ops->dec_ops->name;
		ew_printf("iiiiiddddd3333:%s\n",media_name);
		if(!strcasecmp(media_name, "MP3"))
		{
			decode = ID3_DECODE_MP3;
		}
		else if(!strcasecmp(media_name, "WMA"))
		{
			decode = ID3_DECODE_WMA;
		}
		else if(!strcasecmp(media_name, "FLA"))
		{
			decode = ID3_DECODE_FLAC;
		}
		ew_printf("decode:%s\n",decode);

		if(decode < ID3_DECODE_MAX)
		{
			id3_printf("music id3 decode :%d\n", decode);

			id3_t id3;
			int i;
			for(i = 0; i < ID3_TYPE_MAX; i++)
			{
				if(id3_info->buf[i] != NULL)
				{
					id3_info->len[i] = id3_get_info[i][1];
				}
				else
				{
					id3_info->len[i] = 0;
				}
			}

			id3.decode 	 = decode;
			id3.id3_info = id3_info;
			id3.id3_prt  = id3_buf_prt;
			id3.id3_len  = id3_buf_len;

			//music_get_id3_info(&id3);
			Get_id3Info_err =FALSE;

			/*if(music_get_id3_info(&id3))
			{
				Get_id3Info_err =FALSE;
				if(id3_info->buf[ID3_TYPE_TITLE] 	!= NULL)	id3_printf("music_id3_title:%s\n",  	id3_info->buf[ID3_TYPE_TITLE]);
				if(id3_info->buf[ID3_TYPE_ARTIST] 	!= NULL)	id3_printf("music_id3_artist:%s\n", 	id3_info->buf[ID3_TYPE_ARTIST]);
				if(id3_info->buf[ID3_TYPE_ALBUM] 	!= NULL)	id3_printf("music_id3_album:%s\n",  	id3_info->buf[ID3_TYPE_ALBUM]);
			}
			else
			{
				Get_id3Info_err =TRUE;
			}*/
		}
		/*
		Get_id3Info_err =FALSE;
		id3_info->buf[ID3_TYPE_ARTIST]=mp3_head->artist;
		id3_info->len[ID3_TYPE_ARTIST]=mp3_head->artist_len/2;

		id3_info->buf[ID3_TYPE_TITLE]=mp3_head->title;
		id3_info->len[ID3_TYPE_TITLE]=mp3_head->title_len/2;

		id3_info->buf[ID3_TYPE_ALBUM]=mp3_head->album;
		id3_info->len[ID3_TYPE_ALBUM]=mp3_head->album_len/2;
		
		if(id3_info->buf[ID3_TYPE_TITLE]	!= NULL)	id3_printf("music_id3_title:%s\n",		id3_info->buf[ID3_TYPE_TITLE]);
		if(id3_info->buf[ID3_TYPE_ARTIST]	!= NULL)	id3_printf("music_id3_artist:%s\n", 	id3_info->buf[ID3_TYPE_ARTIST]);
		if(id3_info->buf[ID3_TYPE_ALBUM]	!= NULL)	id3_printf("music_id3_album:%s\n",		id3_info->buf[ID3_TYPE_ALBUM]);
        */
	}
}


#ifdef  _SUPPORT_ID3_
#define UTF8		0x05
MP3_ID3_INFO *mp3_head;
#if defined(_MUSICID3_ADJUST_)
u8(*id3_tmp_buf)[256] = NULL;
#else
//u8 id3_tmp_buf[5][256];
u8 id3_tmp_buf[5][256] __attribute__((aligned(4)));
u8 id3_tag_buf[ID3_DISP_BUF_LEN];
#endif

ID3V2_PARSE s_id3v2;
u8 ID3_title_v2_34[] = "TPE1TALBTIT2";// ID3V2.3/4需要查找的FrameID Table
u8 ID3_title_v2_2[] = "TP1TALTT2"; // ID3V2.2需要查找的FrameID Table
u8 id3_tag_len;
char  *dir_filename_buf;
u16  show_buf_len = 0;
u8 name_len = 0;


#if defined(_MUSICID3_ADJUST_)
u8 * get_file_id3_buf(void)
{
    u8* parse_buf = NULL;

	puts("get_file_id3_buf\n");
	parse_buf = malloc(5 * 256);

    id3_printf("------len = %d byte\n",5 * 256);
    id3_printf("get_file_id3_buf malloc addr= %x ----\n",parse_buf);
	//put_u32hex((u32)parse_buf);

	if(parse_buf != NULL){
		id3_tmp_buf = (u8 (*)[256])parse_buf;
	}
    return parse_buf;
}
void free_file_id3_buf(void)
{
	puts("free_file_id3_buf\n");

	if(id3_tmp_buf != NULL){
		puts("free_file_id3_buf1\n");
		free(id3_tmp_buf);
		id3_tmp_buf = NULL;
	}
}
#endif

u8 ff_id3v2_match_2(const u8  *buf)
{
    return  buf[0]         == 'I' &&
            buf[1]         == 'D' &&
            buf[2]         == '3' &&
            buf[3]         != 0xff &&
            buf[4]         != 0xff &&
            (buf[6] & 0x80) ==    0 &&
            (buf[7] & 0x80) ==    0 &&
            (buf[8] & 0x80) ==    0 &&
            (buf[9] & 0x80) ==    0;
}

u32 ff_id3v2_frame_len(const u8  * buf)
{
    u32 len = ((u32)(buf[0] & 0x7f) << 21) +
              ((u32)(buf[1] & 0x7f) << 14) +
              (((u32)buf[2] & 0x7f) << 7) +
              (buf[3] & 0x7f);
    return len;
}
void ff_id3v2_parse_header(const u8  *buf, ID3V2_PARSE  *id3)
{
    u32 tlen;

    id3->isv34 = 0;
    id3->tunsync = 0;
    id3->id3v2_len = ff_id3v2_frame_len(buf + 6);//标签大小，4个字节，每个字节最高位不用
    id3->id3v2_version = buf[3];//id3版本
    id3->id2v3_flags = buf[5];
    switch (id3->id3v2_version)
    {
        case 2:
            if (id3->id2v3_flags & 0x40)//bit6
            {
                return;
            }
            id3->taghdrlen = 6;//tag head len
            break;
        case 3:
        case 4:
            id3->isv34 = 1;
            id3->taghdrlen = 10;
            break;
        default:
            return;
    }
    id3->unsync = ((id3->id2v3_flags & 0x80) == 0)?(0):(1);//bit7

    if (id3->isv34 && id3->id2v3_flags & 0x40) /*bit6 Extended header present, just skip over it */
    {
        mp3_head->id3_e_ptr += 4;
        tlen = ff_id3v2_frame_len(buf + 10);
        mp3_head->id3_ptr += mp3_head->id3_e_ptr;
        mp3_head->id3_ptr += id3->tlen;
        mp3_head->id3_e_ptr = 0;
    }
}
u8 ff_id3v2_check(const u8  * buf, ID3V2_PARSE  *id3)
{
    mp3_head->id3_ptr += mp3_head->id3_e_ptr;
    mp3_head->id3_e_ptr = ID3v2_HEADER_SIZE;
    if(ff_id3v2_match_2(buf))
    {
        ff_id3v2_parse_header(buf, id3);
        return 1;
    }
    return 0;
}

void ff_cal_id3v2_tag_len_2(const u8  * buf)//底层定义了
{
    u32 len = ff_id3v2_frame_len(buf + 6) + ID3v2_HEADER_SIZE;
    if (buf[5] & 0x10){
        len += ID3v2_HEADER_SIZE;
    }
}

u32 int4_l(const u8  * buf)
{
    u32   len =  ((u32)buf[0])*0x1000000
               + ((u32)buf[1])*0x10000
               +((u32)buf[2])*0x100
               + ((u32)buf[3]);
    return len;
}

u16 int2_l(const u8  * buf)
{
     u16 len = ((u16)(buf[0])*0x100)
        + buf[1];

    return len;
}

u32 int3_l(const u8  * buf)
{    u32   len =  ((u32)buf[0])*0x10000
                + ((u32)buf[1])*0x100
                + (u32)buf[2] ;

    return len;
}

/*
buffer的长度为256 bytes,每一个TAG读取的长度不超过256 bytes
*/
u8 ff_id3v2_parse( u8  *buf, ID3V2_PARSE  *id3)
{//每个标签帧都有一个10个字节的帧头和至少一个字节的不固定长度的内容组成。
//    printf("id3->id3v2_len=%04x %04x  ",(u16)(id3->id3v2_len>>16),(u16)id3->id3v2_len);
//	printf("taghdrlen= %04x %04x\n",(u16)(id3->taghdrlen>>16),(u16)id3->taghdrlen);
    if (id3->id3v2_len >= id3->taghdrlen)
    {
        buf[4] = 0;
        if (id3->isv34)
        {
            buf += mp3_head->id3_e_ptr;
            mp3_head->id3_ptr += mp3_head->id3_e_ptr;
            mp3_head->id3_e_ptr = 10;
            if (id3->id3v2_version==3)
            {
                //tlen = get_be32(s->pb);
                id3->tlen = int4_l(buf + 4);//帧内容大小
            }
            else{
                //tlen = get_size(s->pb, 4);
                id3->tlen = ff_id3v2_frame_len(buf + 4);
            }
            id3->tunsync = ((int2_l(buf + 8) & 0x02)==0)?(0):(1);
            //tflags = get_be16(s->pb);
            //tunsync = tflags & 0x02;
        }
        else
        {
//            f_read(0,buf, 6);					/* 取一个frame的header */
            buf += mp3_head->id3_e_ptr;
            mp3_head->id3_ptr += mp3_head->id3_e_ptr;
            mp3_head->id3_e_ptr = 6;
		//    printf_buf(buf,6);
		    //get_buffer(s->pb, tag, 3);
            //tag[3] = 0;
            //tlen = get_be24(s->pb);
            id3->tlen = int3_l(buf + 3);
            buf[3] = 0;
        }
        id3->id3v2_len -= id3->taghdrlen + id3->tlen;

        if (id3->id3v2_len < 0)
            goto _id3v2_exit;

        //id3->next_frame = f_tell_fptr() + tlen;

        if (!buf[0])
        {
//            f_seek(SEEK_CUR, id3->tlen, id3->tlen>>16);
//            buf += mp3_head->id3_e_ptr;
            mp3_head->id3_ptr += mp3_head->id3_e_ptr;
            mp3_head->id3_ptr += id3->tlen;
            mp3_head->id3_e_ptr = 0;
            goto _id3v2_exit;
        }
        else //if (buf[0] == 'T')
        {
            return 1;
        }
    }

_id3v2_exit:
    if (id3->id3v2_len > 0)
    {
        /* Skip padding */
//        f_seek(SEEK_CUR, id3->id3v2_len, id3->id3v2_len>>16);
            mp3_head->id3_ptr += mp3_head->id3_e_ptr;
            mp3_head->id3_ptr += id3->tlen;
            mp3_head->id3_e_ptr = 0;
    }
    if (id3->id3v2_version == 4 && id3->id2v3_flags & 0x10) /* Footer preset, always 10 bytes, skip over it */
    {
//        f_seek(SEEK_CUR, 10 ,0);
        mp3_head->id3_ptr += mp3_head->id3_e_ptr;
        mp3_head->id3_ptr += 10;
        mp3_head->id3_e_ptr = 0 ;
    }
    return 0;
}
int get_utf8_size(const unsigned char pInput)
{
    unsigned char ch = pInput;

    if(ch< 0x80) return 0;
    if(ch>=0x80 && ch<0xC0) return -1;
    if(ch>=0xC0 && ch<0xE0) return 2;
    if(ch>=0xE0 && ch<0xF0) return 3;
    if(ch>=0xF0 && ch<0xF8) return 4;
    if(ch>=0xF8 && ch<0xFC) return 5;
    if(ch>=0xFC) return 6;

    return 0;
}

int utf8_2_unicode_one(const unsigned char* in, u16 *unicode)
{
    char b1, b2, b3, b4, b5, b6;
    if(!unicode)
        return -1;
    *unicode = 0x0;
    int utfbytes = get_utf8_size(*in);
    unsigned char *out = (unsigned char *) unicode;

    switch(utfbytes)
    {
        case 0:
            *out     = *in;
            utfbytes    += 1;
            break;
        case 2:
            b1 = *in;
            b2 = *(in + 1);
            if ( (b2 & 0xC0) != 0x80 )
                return 0;
            *out     = (b1 << 6) + (b2 & 0x3F);
            *(out+1) = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *in;
            b2 = *(in + 1);
            b3 = *(in + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return 0;
            *out     = (b2 << 6) + (b3 & 0x3F);
            *(out+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *in;
            b2 = *(in + 1);
            b3 = *(in + 2);
            b4 = *(in + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) )
                return 0;
            *out     = (b3 << 6) + (b4 & 0x3F);
            *(out+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(out+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *in;
            b2 = *(in + 1);
            b3 = *(in + 2);
            b4 = *(in + 3);
            b5 = *(in + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return 0;
            *out     = (b4 << 6) + (b5 & 0x3F);
            *(out+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(out+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(out+3) = (b1 << 6);
            break;
        case 6:
            b1 = *in;
            b2 = *(in + 1);
            b3 = *(in + 2);
            b4 = *(in + 3);
            b5 = *(in + 4);
            b6 = *(in + 5);
            if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                    || ((b6 & 0xC0) != 0x80) )
                return 0;
            *out     = (b5 << 6) + (b6 & 0x3F);
            *(out+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(out+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(out+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
            break;
    }

    return utfbytes;
}

int utf8_2_unicode(u8 *utf8, u8 utf8_len, u8 *unicode, u8 unic_len)
{
    int t_len = 0;
    int r_len = 0;
    u16 unic = 0;
    int utf8_offset = 0;

    while((utf8_len)>0)
    {
        t_len = utf8_2_unicode_one((const unsigned char*)(utf8+utf8_offset), &unic);

        utf8_len -= (u8)t_len;
        utf8_offset += t_len;
        if(t_len == 0)
            break;
        if(!unicode)
            return -1;

        if((r_len*2) > unic_len)
            break;

        *unicode++ = (unic >> 8)&0xff;
        *unicode++ = unic&0xff;
        r_len ++;
    }

//    printf("unic len:%d\r", r_len);
    return r_len*2;
}

u8 ascii_format(u8 *buf, u8 format, u8 len)
{
    u8 i, j;
    u8 *ptr;
    u8 len_tmp;
    ptr = buf;
    switch (format)
    { /* encoding type */
        case 0: /* ISO-8859-1 (0 - 255 maps directly into unicode) */
            mp3_head->encode_flag = ANSI;
            my_memcpy(id3_tmp_buf[3] , buf , len);
			id3_tmp_buf[3][len] = 0;
//			len_tmp = utf8_2_unicode(id3_tmp_buf[3],len,id3_tmp_buf[4],255 );
//			len = len_tmp;
            for(i = 0; i < len; i++)
            {
                id3_tmp_buf[4][2*i] = 0x00;
                id3_tmp_buf[4][2*i+1] = id3_tmp_buf[3][i];
            }
            len = len*2;
			for (i = 0; i < len; i += 2)
			{
		    	if ((id3_tmp_buf[4][i] == 0) && (id3_tmp_buf[4][i + 1] == 0))
		    	{
                    id3_tmp_buf[4][i] = 0x20;
                    id3_tmp_buf[4][i + 1] = 0x00;
		    	}
		    	j  = id3_tmp_buf[4][i + 1];
				id3_tmp_buf[4][i+ 1] = id3_tmp_buf[4][i];
				id3_tmp_buf[4][i] = j ;
			}
	   		break;

        case 1: /* UTF-16 with BOM */
            mp3_head->encode_flag = UNICODE_LE;
            buf += 2;//编码字符0x01后跟着FFFE小端
            len -= 2;
            for (i = 0; i < len; i += 2)
			{
		    	if ((buf[i] == 0) && (buf[i + 1] == 0))
		    	{
		       		 buf[i] = 0x20;
		       		 buf[i + 1] = 0x00;
		    	}
			}
            break;
        case 3: /* UTF-8 */
            mp3_head->encode_flag = UTF8;
            my_memcpy(id3_tmp_buf[3] , buf , len);
			id3_tmp_buf[3][len] = 0;
			len_tmp = utf8_2_unicode(id3_tmp_buf[3],len,id3_tmp_buf[4],255 );
			len = len_tmp;
			for (i = 0; i < len; i += 2)
			{
		    	if ((id3_tmp_buf[4][i] == 0) && (id3_tmp_buf[4][i + 1] == 0))
		    	{
                    id3_tmp_buf[4][i] = 0x20;
                    id3_tmp_buf[4][i + 1] = 0x00;
		    	}
		    	j  = id3_tmp_buf[4][i + 1];
				id3_tmp_buf[4][i+ 1] = id3_tmp_buf[4][i];
				id3_tmp_buf[4][i] = j ;
			}
            break;
        case 2: /* UTF-16BE without BOM */
        default:    //编码字符0x02后跟着FEFF大端
            mp3_head->encode_flag = UNICODE_LE;
			for (i = 0; i < len; i += 2)
			{
		    	if ((buf[i] == 0) && (buf[i + 1] == 0))
		    	{
		       		 buf[i] = 0x20;
		       		 buf[i + 1] = 0x00;
		    	}
		    	j  = buf[i + 1];
				buf[i+ 1]  = buf[i];
				buf[i] = j ;
			}
			break;
    }
    return len;
}

u8 my_memcmp(u8 *s1, u8 *s2, u8 len)
{
    u8 i;
    for (i = 0; i < len; i++)
    {
        if ((*(s1 + i )) != (*(s2 + i)))
            return 1;
    }
    return 0;
}

/*
buf的长度必须大于512 + 128 bytes
如果正确,返回buf为TAG指针
*/
u8 ff_id3v1_match(const u8  *buf)
{
//    u32 file_len;

//    file_len = f_tell_fsize(0);

//    if (file_len >= ID3v1_TAG_SIZE)
//    {
////        f_seek(SEEK_SET, (file_len - ID3v1_TAG_SIZE), (file_len - ID3v1_TAG_SIZE)>>16);
////		printf_buf((u8 xdata *)&fat_ptr1,sizeof(FIL));
//    }
//    else											//文件长度小于128 Bytes
//    {
//        return 0;
//    }
//
    //f_seek(SEEK_SET, 0);					//文件指针复位

    if (buf[0] == 'T' && buf[1] == 'A' && buf[2] == 'G')
    {
        return 1;
    }
    else
        return 0;
}
u8 id3_strlen(u8 * buf)
{
    u8 i = 0;

    while(*(buf+i) != 0x00){
        i++;
		if(i == 0xFF)break;
    }
    return i;
}
void get_mp3_v1_tag(void)
{
    u8 i,j,k,n;

	id3_printf("get_mp3_v1_tag\n");

    mp3_head->id3_ptr = 0;
    mp3_head->id3_e_ptr = 0;

    if(mp3_head->id3_len != 128)
	{
        return;
    }
    if(ff_id3v1_match(mp3_head->id3_buf))
	{
        mp3_head->id3_ptr += 3;
        mp3_head->id3_e_ptr = 30;
        for(i = 0;i < 3;i++ )
		{
			id3_printf("get_mp3_v1_tag111\n");
            k = id3_strlen(mp3_head->id3_buf+mp3_head->id3_ptr);
			ew_printf("K:%d\n",k);
            j=k;
            id3_tmp_buf[i][0] = j;
            my_memcpy(id3_tmp_buf[i] + 2, mp3_head->id3_buf+mp3_head->id3_ptr, j);
            mp3_head->encode_flag = ANSI;
            mp3_head->id3_ptr += mp3_head->id3_e_ptr;
            mp3_head->id3_e_ptr = 30;
        }
    }
	id3_tag_len = 0;
    for(i = 0;i < 3;i++)
	{ //标题-作者-专辑
        if(i==0) n=1;     //将显示顺序改成:TPE1作者-TIT2歌曲-TALB专辑
        if(i==1) n=0;
        if(i==2) n=2;
        k = id3_tmp_buf[n][0];//内容大小
        j = ID3_DISP_BUF_LEN - id3_tag_len;//最多128
        k = k >= j?j:k;
        if (k > 0)
        {
		#if !defined(_MUSICID3_ADJUST_)
            my_memcpy(id3_tag_buf + id3_tag_len, id3_tmp_buf[n] + 2, k);
			if((id3_tag_len + k + 1 ) < ID3_DISP_BUF_LEN )
			{
			    if(mp3_head->encode_flag == ANSI){
                    id3_tag_buf[id3_tag_len + k] = 0x20;
                    id3_tag_buf[id3_tag_len + k+1] = 0x20;
                    id3_tag_len = id3_tag_len+k+2;
			    }else{
                    id3_tag_buf[id3_tag_len + k] = 0x20;
                    id3_tag_buf[id3_tag_len + k + 1] = 0x00;
                    id3_tag_len = id3_tag_len+k+2;
			    }
			}
		#endif
        }
    }//put_buf(id3_tag_buf,id3_tag_len);

    puts("\n 111 show  id3 info\n");

    /*for(i = 0;i<id3_tag_len;i++)
    {
         putchar(id3_tag_buf[i]);
    }*/

}
void get_mp3_v2_tag(void)
{
    u8  i,j,k,m,n;
    u8  *id3_table;
    //char  *id3_table;

    mp3_head->id3_ptr = 0;
    mp3_head->id3_e_ptr = 0;
    my_memset((u8*)&s_id3v2, 0, sizeof(ID3V2_PARSE));
    while (ff_id3v2_check(mp3_head->id3_buf + mp3_head->id3_ptr, &s_id3v2)) //检测获取ID3头的头信息
    {
		ff_cal_id3v2_tag_len_2(mp3_head->id3_buf + mp3_head->id3_ptr);
		while (ff_id3v2_parse(mp3_head->id3_buf + mp3_head->id3_ptr, &s_id3v2))
        {
			if (s_id3v2.id3v2_version == 2)
            {
                id3_table = ID3_title_v2_2;
                j = 3;
            }
            else
            {
                id3_table = ID3_title_v2_34;
                j = 4;
            }
//            k = 0;
            for (i = 0; i < 3; i++)
            {
				if (!my_memcmp(mp3_head->id3_buf + mp3_head->id3_ptr, id3_table + i*j, j))
                {
                    if (s_id3v2.tlen > 127)
                        k = 127;
                    else
                        k = s_id3v2.tlen;
                    s_id3v2.tlen = 0;
                    mp3_head->id3_ptr += mp3_head->id3_e_ptr;
                    mp3_head->id3_e_ptr = k;
                    j = ascii_format(mp3_head->id3_buf+mp3_head->id3_ptr + 1, (mp3_head->id3_buf+ mp3_head->id3_ptr)[0], k - 1);			//第一个字节为字符的编码，实际的内容从第二个字符开始。内容的长度要减去编码字符
//                    printf("mp3_head->id3_ptr:%x\n",mp3_head->id3_ptr);
					id3_tmp_buf[i][0] = (j < ID3_DISP_BUF_LEN) ? j : ID3_DISP_BUF_LEN;
					m = (mp3_head->id3_buf+ mp3_head->id3_ptr)[0] == 1 ? 3 : 1;
					//标记TAG内容前几个字节
					if(mp3_head->encode_flag == ANSI){
                         my_memcpy(id3_tmp_buf[i] + 2, id3_tmp_buf[4], j);
						 mp3_head->encode_flag = UNICODE_LE;
					}
					else if(mp3_head->encode_flag == UNICODE_LE){
                         my_memcpy(id3_tmp_buf[i] + 2, mp3_head->id3_buf + mp3_head->id3_ptr + m, j);
					}else{
                         my_memcpy(id3_tmp_buf[i] + 2, id3_tmp_buf[4], j);
                         mp3_head->encode_flag = UNICODE_LE;
					}
                    break;
                }
                else{
                    k = 0;
                }
            }
                mp3_head->id3_e_ptr += s_id3v2.tlen;
                mp3_head->id3_ptr += mp3_head->id3_e_ptr;
                mp3_head->id3_e_ptr = 0;
        }
    }
	id3_tag_len = 0;
    for (i = 0; i < 3; i++)//显示顺序:TPE1-TALB-TIT2
    {
        if(i==0) n=0;     //将显示顺序改成:TPE1作者-TIT2歌曲-TALB专辑
        if(i==1) n=2;
        if(i==2) n=1;
        k = id3_tmp_buf[n][0];//内容大小
        j = ID3_DISP_BUF_LEN - id3_tag_len;//最多128
        k = k >= j?j:k;
        if (k > 0)
        {
		#if !defined(_MUSICID3_ADJUST_)
            my_memcpy(id3_tag_buf + id3_tag_len, id3_tmp_buf[n] + 1, k);
			if((id3_tag_len + k + 1 ) < ID3_DISP_BUF_LEN )
			{
			    if(mp3_head->encode_flag == ANSI){
                    id3_tag_buf[id3_tag_len + k] = 0x20;
                    id3_tag_len = id3_tag_len+k+1;
			    }else{
                    id3_tag_buf[id3_tag_len + k] = 0x20;
                    id3_tag_buf[id3_tag_len + k + 1] = 0x00;
                    id3_tag_len = id3_tag_len+k+2;
			    }
			}
		#endif
        }
    }

    //puts("\n 222 show  id3 info\n");
    /*id3_printf("id3_tag_len=%d",id3_tag_len);
    id3_printf("\n>>>");
    for(i = 0;i<id3_tag_len;i++)
    {
         //putchar(id3_tag_buf[i]);
         id3_printf("%c",id3_tag_buf[i]);
    }
	id3_printf("<<<\n");*/


}
void get_music_tag(void)
{
	bool id3_version = 0;

	//u16 i;

    mp3_head->have_id3 = 0;

    if(id3_v2_head.id3_len > 0){
        id3_puts("id3_version2\n");
        mp3_head->id3_buf = id3_v2_head.id3_buf;
        mp3_head->id3_len = id3_v2_head.id3_len;
        id3_version = 2;
    }else if(id3_v1_head.id3_len > 0){
        id3_puts("id3_version1\n");
        mp3_head->id3_buf = id3_v1_head.id3_buf;
        mp3_head->id3_len = id3_v1_head.id3_len;
        id3_version = 1;
    }

	id3_puts("\n==11111==\n");

	if(mp3_head->id3_buf == NULL){
        return;
	}

	id3_printf("mp3_head->id3_len = %d\n",mp3_head->id3_len);

    /*for(i = 0;i < 600;i++)
    {
        //id3_printf("%c ",mp3_head->id3_buf[i]);
    }*/
    my_memset((u8  *)id3_tmp_buf, 0, 256*5);
#if !defined(_MUSICID3_ADJUST_)
    my_memset((u8  *)id3_tag_buf, 0, ID3_DISP_BUF_LEN);
#endif
    if(id3_version == 2){
        get_mp3_v2_tag();
		Get_id3Info_err =FALSE;
    }else if(id3_version == 1){
        get_mp3_v1_tag();
		Get_id3Info_err =FALSE;
    }else{
        return;
    }
	    if(id3_version == 2){
			mp3_head->artist = &id3_tmp_buf[0][2];
			mp3_head->artist_len = id3_tmp_buf[0][0];
			mp3_head->title = &id3_tmp_buf[2][2];
			mp3_head->title_len = id3_tmp_buf[2][0];
			mp3_head->album = &id3_tmp_buf[1][2];
			mp3_head->album_len = id3_tmp_buf[1][0];
		}else if(id3_version == 1){
			mp3_head->artist = &id3_tmp_buf[1][2];
			mp3_head->artist_len = id3_tmp_buf[1][0];
			mp3_head->title = &id3_tmp_buf[0][2];
			mp3_head->title_len = id3_tmp_buf[0][0];
			mp3_head->album = &id3_tmp_buf[2][2];
			mp3_head->album_len = id3_tmp_buf[2][0];
		}else{
			return;
		}

    if (id3_tmp_buf[2][0] == 0)//将显示顺序改成:TPE1作者-TIT2歌曲-TALB专辑
    {
        if (id3_tmp_buf[3][0])							//获取长文件名
        {
            mp3_head->have_id3  = 0;
        }
        else
        {
            mp3_head->have_id3  = 0;
        }
    }else{
        mp3_head->have_id3 = 1;
    }

    /*printf("artist_len=%d>",mp3_head->artist_len);
    for(u8 f=0;f<mp3_head->artist_len;f++)
    {
    	printf("%c",mp3_head->artist[f]);
    }
    printf("<\n");
    
    printf("title_len=%d>",mp3_head->title_len);
    for(u8 f=0;f<mp3_head->title_len;f++)
    {
    	printf("%c",mp3_head->title[f]);
    }
    printf("<\n");
    
    printf("album_len=%d>",mp3_head->album_len);
    for(u8 f=0;f<mp3_head->album_len;f++)
    {
    	printf("%c",mp3_head->album[f]);
    }
    printf("<\n");*/

}

#if 0
void get_patch_info(void )
{
    u16 i,j,patch_len = 0;

    id3_printf("\nget_patch_info\n");

    while(music_name_buf.tpath[patch_len] != '\0')
    {
          //id3_puts(music_name_buf.tpath[patch_len]);
          id3_printf("%c",music_name_buf.tpath[patch_len]);
          patch_len++;
          if(patch_len > 128)
          {
                break;
          }
    }

    id3_printf("patch_len = %d\n",patch_len);

    for(i = 0;i<SHOW_BUFF_LEN;i++)
    {

          if(i<NUM_A)
          {
              dir_filename_buf[i] = ' ';
          }

          if(i == (0+NUM_A))
            dir_filename_buf[i] = 'D';
          if(i == (1+NUM_A))
           dir_filename_buf[i] = 'I';
          if(i == (2+NUM_A))
           dir_filename_buf[i] = 'R';
          if(i == (3+NUM_A))
           dir_filename_buf[i] = ':';

          puts("--1--\n");
          if(i > (3+NUM_A))
          {
                if(patch_len > 13)
                {
                      puts("--2--\n");
                      if((i > (3+NUM_A))&&(i<((3+NUM_A+1)+(patch_len - 13))))
                      {
                            puts("--3--\n");
                           dir_filename_buf[i] =  music_name_buf.tpath[i-(4+NUM_A)];
                      }
                      else
                      {
                            puts("--4--\n");
                            if(i == (4+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = ' ';
                            if(i == (5+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = ' ';
                            if(i == (6+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = 'F';
                            if(i == (7+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = 'I';
                            if(i == (8+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = 'L';
                            if(i == (9+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = 'E';
                            if(i == (10+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = ' ';
                            if(i == (11+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = 'N';
                            if(i == (12+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = 'A';
                            if(i == (13+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = 'M';
                            if(i == (14+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = 'E';
                            if(i == (15+NUM_A+(patch_len - 13)))  dir_filename_buf[i] = ':';

                            if(i > (15+NUM_A+(patch_len - 13)))
                            {
                                  if((i > (15+NUM_A+(patch_len - 13))) && (i <= ((15+NUM_A+(patch_len - 13))+ name_len)))
                                  {
                                        puts("--5--\n");
                                         dir_filename_buf[i] = music_name_buf.file_name->lfn[i-(16+NUM_A+(patch_len - 13))];
                                  }
                                  else
                                  {
                                        puts("--6--\n");
                                        //dir_filename_buf[i] = '\0';
                                        break;
                                  }
                            }

                      }

                }
                else
                {
                      puts("--7--\n");
                      if((i > 3+NUM_A)&&(i<(4+NUM_A+patch_len )))
                      {
                            puts("--8--\n");
                           dir_filename_buf[i] =  music_name_buf.tpath[i-(4+NUM_A)];
                      }
                      else
                      {
                            if(i == (4+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = ' ';
                            if(i == (5+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = ' ';
                            if(i == (6+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = 'F';
                            if(i == (7+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = 'I';
                            if(i == (8+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = 'L';
                            if(i == (9+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = 'E';
                            if(i == (10+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = ' ';
                            if(i == (11+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = 'N';
                            if(i == (12+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = 'A';
                            if(i == (13+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = 'M';
                            if(i == (14+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = 'E';
                            if(i == (15+NUM_A+(patch_len - 0)))  dir_filename_buf[i] = ':';

                            puts("--9--\n");

                            if(i > (15+NUM_A+(patch_len - 0)))
                            {
                                  if((i > (15+NUM_A+(patch_len - 0))) && (i <= ((15+NUM_A+(patch_len - 0))+ name_len)))
                                  {
                                        puts("--10--\n");
                                         dir_filename_buf[i] = music_name_buf.file_name->lfn[i-(16+NUM_A+(patch_len - 0))];
                                  }
                                  else
                                  {
                                        puts("--11--\n");
                                        //dir_filename_buf[i] = '\0';
                                        break;
                                  }
                            }


                      }
                }
          }

          printf(" i = %d  %c\n",i,dir_filename_buf[i]);

    }


      show_buf_len = i;

      printf("show_buf_len = %d  id3_tag_len = %d \n",show_buf_len ,id3_tag_len);

    for(i = 0;i<show_buf_len;i++)
    {
         putchar(dir_filename_buf[i]);
    }



/*
    for(i = 0;i<id3_tag_len;i++)
    {
         putchar(id3_tag_buf[i]);
    }
*/

       dir_filename_buf[show_buf_len+1] = ' ';
       dir_filename_buf[show_buf_len+2] = ' ';
       dir_filename_buf[show_buf_len+3] = 'T';
       dir_filename_buf[show_buf_len+4] = 'I';
       dir_filename_buf[show_buf_len+5] = 'F';
       dir_filename_buf[show_buf_len+6] = 'I';
       dir_filename_buf[show_buf_len+7] = ':';

       show_buf_len = show_buf_len+8;


      if((show_buf_len + id3_tag_len) < SHOW_BUFF_LEN)
      {
           memcpy(&dir_filename_buf[show_buf_len],id3_tag_buf,id3_tag_len);
           show_buf_len = show_buf_len + id3_tag_len;
      }
      else
      {
           memcpy(&dir_filename_buf[show_buf_len],id3_tag_buf,SHOW_BUFF_LEN-show_buf_len);
           show_buf_len = SHOW_BUFF_LEN - 1;
      }


     printf("\n--show_buf_len = %d  \n",show_buf_len );

    for(i = 0;i<show_buf_len;i++)
    {
         //putchar(dir_filename_buf[i]);
        if(dir_filename_buf[i] == '\0')
        {

            for(j = i;j < (show_buf_len - 1); j++)
            {
                dir_filename_buf[j] = dir_filename_buf[j+1];
            }

            show_buf_len = show_buf_len - 1;
        }
    }

     printf("\n--222 show_buf_len = %d  \n",show_buf_len );

    for(i = 0;i<show_buf_len;i++)
    {
         putchar(dir_filename_buf[i]);
    }


}
#endif
#endif
