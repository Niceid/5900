#include "sdk_cfg.h"
#include "music.h"
#include "dec/music_api.h"
#include "vm/vm_api.h"
#include "dac/dac_api.h"
#include "dev_manage/drv_manage.h"
#include "key_drv.h"
#include "music_info.h"
#include "fat/tff.h"
#include "fat/fs_io.h"
#include "lyrics_api.h"
#include "dac/eq_api.h"
#include "dac/dac_api.h"
#include "dac/dac.h"
#include "dac/eq.h"
#include "music_decrypt.h"
#include "clock_interface.h"
#include "clock.h"
#include "disp.h"
#include "setup.h"
#include "timer.h"
#include "sys_detect.h"
#include "dec/decoder_phy.h"
#include "vm/vm.h"
#include "rcsp/rcsp_interface.h"
#include "id3/id3.h"
#include "audio_drv.h"
#include "echo/echo_api.h"
#include "play_sel/play_sel.h"
#include "audio_drv.h"

#if defined(SUPPORT_NUM_SELECT_MUSIC)
extern void get_id3_disp_status(void);
#endif

#define MUSIC_DEC_PHY_NAME 		"MusicPhyDec"
#ifdef	TD_ADD_ALEN
uint8 decode_ok =0;
#endif

#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime;
extern void Spp_Tx_Music(void);
#endif

#ifdef 	TD_ADD_NEW_APP
#if !defined(_DISABLE_MUSIC_LIST_)
u8 Listbrowse_over=0;
static u32 dir_total = 0;
static u32 dir_cur = 0;
static u8 filter_ext_name[20] = "WAVMP3WMAFLAAPE"; ///WAV WMA MP3 FLA APE
static u32 last_dir_clust;
static u32 cur_dir_clust;
extern volatile u8 new_lg_dev_let;
extern u8 spp_connect_falg;

u8 *ListBuf;
#define BIG_FRAME_BUF               (250)
#define EXIT_BS    0xFFFFFFFF
#define MAX_DIR_NUM 10
#define APP_SEND_SONG_NUM           10
FS_DIR_INFO *ls_info = NULL;
static MUSIC_OP_API *m_mapi = NULL;
static MUSIC_OP_API *d_mapi = NULL;
extern void lg_dev_close(void *parm);
extern u32 file_operate_ini(FILE_OPERATE *fapi, void *phy_dev_chg);
extern u32 lg_dev_mount(void *parm,u8 first_let,u32 total_lgdev);
extern u32 file_bs_open_handle(MUSIC_OP_API *m_op_api,u8 *ext_name);
extern void file_bs_close_handle(MUSIC_OP_API *m_op_api);
extern u32 file_bs_get_dir_info(MUSIC_OP_API *m_op_api,FS_DIR_INFO *buf,u16 start_sn,u16 get_cnt);
extern u32 file_bs_entern_dir(MUSIC_OP_API *m_op_api,FS_DIR_INFO *dir_info);
extern u32 file_bs_exit_dir(MUSIC_OP_API *m_op_api);
extern void  Notify_data_send(u8 *data,u16 len);
extern u8 crc_checksum(u8 *data,u16 len);

#define FRAME_BUF                   (50)
#define BT_API_SEND_BUF             (250)//(200)
#define FRAME_CNT                   4
typedef struct{
    u8 Length;
    u8 WritePoint;
    u8 ReadPoint;
    u8 combuf[FRAME_CNT][FRAME_BUF];
    u8 framelen[FRAME_CNT];
    u8 bigbuf[BIG_FRAME_BUF];
    u8 bigbuf_len;
    u8 bigbuf_flag;
}BT_COM_FRAME;

BT_COM_FRAME BtComFrame;

enum{
    sysmod_order=1,
    radiomod_order,
    playermod_oder,
    auxmod_order,
    btmod_order,
    alarmclock_order,   // 闹钟定时功能
    lightmod_order, // 七彩灯
    micdsp_order,   // MIC dsp
    audioSet_oder,  // 放音 音效设置
};

enum{
    write_mod=0,
    read_mod,
};

/*--------------Music----------------*/
enum{
    file_list=1,    // 获取文件列表
    curr_song,  // 当前播放的歌曲
    play_pause, // 播放暂停
    curr_total_time,    // 当前歌曲播放总时间
    curr_play_time,     // 当前播放时间
    pre_song,       // 上一曲
    next_song,      // 下一曲
    play_rpt,       // 播放模式
    play_rdm,       // 播放模式
    play_sel_song,  // 播放选定的歌曲
    song_fresh,     //  歌曲列表刷新标志
    rptrdm_status,   // rpt rdm 状态
    usb_sd_det,     // USB SD卡状态
};

enum{
    media_usb=0,
    media_sd,
};

enum
{
    up_start=0,
    up_check,
    up_data,
    up_wait,
    up_over,
    up_err,
};
typedef struct _MUSIC_LIST
{
    u8 up_state;
    u8 up_check;
    u8 up_flag;
    u8 up_timer;
    u32 file_num;
    u8 send_num;
    u8 send_cnt;    // 发送歌曲数计数
    u8 browse_over;
    u8 snd_err;
}MUSIC_LIST;
MUSIC_LIST MusicList;

#define RECV_FIFO_SIZE							100
#define COM_DATA_SIZE							50
#endif
#endif

#ifdef  _SUPPORT_ID3_
FS_DISP_NAME music_name_buf;//buf need malloc
#endif
extern void ble_server_disconnect(void);
extern u32 file_operate_get_total_lgdev();
extern void *file_operate_get_next_phydev();
extern void *sup_dec_phy_op(void *priv  ,void *data, u32 len);
extern u32 get_decode_time(void);

#if defined(_ENABLE_RCA_MUTE_)
extern void set_sub_mute(void);
#endif

extern const char dec_file_ext[][3];
extern MP3_ID3 id3_v1_head;
extern MP3_ID3 id3_v2_head;

u8 stop_start;
u8 play_mode = REPEAT_ALL;
u8 eq_mode = 0;
u8 stop_flag = 0;
void *music_reverb;
UI_DEVICE music_device = NO_DEVICE;
static music_ctrl_t *music_ctrl = NULL;
MUSIC_OP_API *music_operate_mapi = NULL;
#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_Music_Clock_songe_Info(uint8 device,uint32 total_time,uint32 Curplaytime,uint32 total_trk,uint32 Cur_trk);
extern void TFT_Tx_Music_Status(uint8 state,uint8 mode);
extern void TFT_Tx_System_Status(void);

#endif
void music_enter_updata(void)
{
	echo_exit_api(&music_reverb);
}

static void get_dev_change(void * dev)
{
#if 0//def MUSIC_PROMPT_EN
	if(dev == usb)
	{
		puts("----usb---\n");
        tone_play_by_name(NULL, 1, BPF_PC_MP3);
		while(play_sel_busy())
		{
			os_time_dly(1);
		}
	}
	else if(dev == sd0)
	{
		puts("----sd0---\n");
        tone_play_by_name(NULL, 1, BPF_MUSIC_MP3);
		while(play_sel_busy())
		{
			os_time_dly(1);
		}
	}
	else if(dev == sd1)
	{
		puts("----sd1---\n");
		tone_play_by_name(NULL, 1, BPF_MUSIC_MP3);
		while(play_sel_busy())
		{
			os_time_dly(1);
		}
	}
#endif
}

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
static void music_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{
		if((msg >= MUSIC1_DISPLAY_BEGIN)&&(msg <= MUSIC1_DISPLAY_END))
		{
			disp_send_msg(DISP_EVENT_RESET);
		}
		else if((msg >= MUSIC2_DISPLAY_BEGIN)&&(msg <= MUSIC2_DISPLAY_END))
		{
			disp_send_msg(DISP_EVENT_RESET);
		}
	}
}
#if defined(TD_ADD_NEW_APP)
extern u32 play_file_clust;
extern void new_app_MusicInfo(uint8 curdev,uint32 total_trk,uint32 Cur_trk,uint32 total_time,uint32 Curplaytime,u8 rpt_mode,u8 playstate,u32 filesclust);
#elif defined(TD_ADD_SPP)
extern void Spp_Tx_MusicInfo(uint8 curdev,uint32 total_trk,uint32 Cur_trk,uint32 total_time,uint32 Curplaytime);
#endif
#if defined(_USER_TFT_SEND_)
uint8 p_state=PLAY_STATUS_PLAYE;
uint8 p_mode=PLAY_MODE_REPEAT_ALL;
#endif
const void *task_name;
static void music_disp_info(MUSIC_OP_API *m_op_api)
{
	//ew_puts("music_disp_info\n");
	music_play_info_t play_info;
	memset(&play_info, 0, sizeof(play_info));
	
	//ew_printf("phy_ops->status:%x\n",(int)m_op_api->dop_api->dec_api.phy_ops->status);
	
	switch((int)m_op_api->dop_api->dec_api.phy_ops->status)
	{
		default:
			play_info.state = PLAY_STATUS_IDLE;
			break;

		case DECODER_STOP:
		case 0x8000:
		case 8:	
		case 4: 
		case 0x1000900: 
			play_info.state = PLAY_STATUS_STOP;
			break;

		case DECODER_PAUSE:
			play_info.state = PLAY_STATUS_PAUSE;
			break;

		case DECODER_PLAY:
			if(music_ctrl->ffr_state)
			{
				play_info.state = music_ctrl->ffr_state;
			}
			else
			{
				play_info.state = PLAY_STATUS_PLAYE;
			}
			break;
	}

	_FIL_HDL *f_h = (_FIL_HDL *)(m_op_api->fop_api->cur_lgdev_info->lg_hdl->file_hdl);
	FIL *pf = (FIL *)(f_h->hdl);

	//play_info->total = pf->fs_msg.file_counter;
	play_info.total = music_ctrl->play_info.total;

	//play_info.tune  = m_op_api->dop_api->file_num;
	play_info.tune  = pf->fs_msg.file_number;
    music_cur_filenum =pf->fs_msg.file_number;
    music_cur_filetotal =music_ctrl->play_info.total;

	extern u32 get_decode_time(void);
	u32 play_time = get_decode_time();
	play_info.min = play_time/60;
	play_info.sec = play_time%60;

	if(music_ctrl->intro_flag)
	{
		play_info.mode = PLAY_MODE_INTRO;
    #if defined(_ACC_OFF_MEMORY_INT_)
	    setup.int_flag = TRUE;
	#endif
	#if defined(_MODE_MEMORY_INT_)
	    global.int_memory = TRUE;
	#endif
	}
	else
	{
		switch(play_mode)
		{
			default:
			case REPEAT_ALL:
				play_info.mode = PLAY_MODE_REPEAT_ALL;
				break;

			case REPEAT_FOLDER:
				play_info.mode = PLAY_MODE_REPEAT_FOLDER;
				break;

			case REPEAT_ONE:
			#if defined(_ACC_OFF_MEMORY_INT_)
				setup.int_flag = FALSE;
			#endif
            #if defined(_MODE_MEMORY_INT_)
            	global.int_memory = FALSE;
            #endif
				play_info.mode = PLAY_MODE_REPEAT_ONE;
				break;

			case REPEAT_RANDOM:
            #if defined(_ACC_OFF_MEMORY_INT_)
				setup.int_flag = FALSE;
            #endif
			#if defined(_MODE_MEMORY_INT_)
				global.int_memory = FALSE;
			#endif
				play_info.mode = PLAY_MODE_SHUFFLE;
				break;
		}

	}

#if defined(_USER_TFT_SEND_)
    if((p_state!=play_info.state)||(p_mode!=play_info.mode))
    {
    	p_state = play_info.state;
    	p_mode = play_info.mode;
    	TFT_Tx_Music_Status(p_state,p_mode);//如果一直发送会导致时间命令接收不了
    }
#endif
	//music_printf("task_name:%s\n",task_name);
	//music_printf("curr_task->name:%s\n",curr_task->name);
	
	//music_printf("stop_flag:%d\n",stop_flag);
#if defined(_SPECIAL_PREV_FILE_)
	if((music_ctrl->play_info.tune != play_info.tune)||(curr_task->name != task_name)||stop_flag||global.prev_flag)
	{
		global.prev_flag = FALSE;
#else
	if((music_ctrl->play_info.tune != play_info.tune)||(curr_task->name != task_name)||stop_flag)
	{
#endif
		stop_flag = FALSE;
		task_name = curr_task->name;

		play_info.change = TRUE;

		char *media_name = m_op_api->dop_api->dec_api.phy_ops->dec_ops->name;
		music_printf("file_media:%s\n",media_name);

		//GET_SYS_MEM_STAT();//打印RAM信息

		if(!strcasecmp(media_name, "MP3"))
		{
			play_info.media = PLAY_MEDIA_MP3;
			ew_puts("PLAY_MEDIA_MP3\n");
		}
		else if(!strcasecmp(media_name, "WMA"))
		{
			play_info.media = PLAY_MEDIA_WMA;
			ew_puts("PLAY_MEDIA_WMA\n");
		}
		else if(!strcasecmp(media_name, "WAV"))
		{
			play_info.media = PLAY_MEDIA_WAV;
			ew_puts("PLAY_MEDIA_WAV\n");
		}
		else if(!strcasecmp(media_name, "FLA"))
		{
			play_info.media = PLAY_MEDIA_FLAC;
			ew_puts("PLAY_MEDIA_FLAC\n");
		}
		else if(!strcasecmp(media_name, "APE"))
		{
			play_info.media = PLAY_MEDIA_APE;
			ew_puts("PLAY_MEDIA_APE\n");
		}
		else
		{
			play_info.media = PLAY_MEDIA_UNKONW;
			ew_puts("PLAY_MEDIA_UNKONW\n");
		}

	#if defined(_SUPPORT_ID3_)
		//music_printf("11111111111111111=0x%x\n",mp3_head->encode_flag);
      	music_file_id3_file(&play_info.id3_info[PLAY_ID3_FILE], 		&play_info.id3_len[PLAY_ID3_FILE]);
      	music_file_id3_folder(&play_info.id3_info[PLAY_ID3_FOLDER], 	&play_info.id3_len[PLAY_ID3_FOLDER]);
		music_file_id3_title(&play_info.id3_info[PLAY_ID3_TITLE],   	&play_info.id3_len[PLAY_ID3_TITLE]);
		music_file_id3_artist(&play_info.id3_info[PLAY_ID3_ARTIST], 	&play_info.id3_len[PLAY_ID3_ARTIST]);
		music_file_id3_album(&play_info.id3_info[PLAY_ID3_ALBUM],   	&play_info.id3_len[PLAY_ID3_ALBUM]);
	#endif
	}
	else
	{
		//music_printf("222222222222222222\n");
		play_info.media = music_ctrl->play_info.media;

	#if defined(SYS_MODE_CHANG_NOPP)
		global.music_pp_enable=1;//MUSIC  OK
	#endif

	#if defined(_SUPPORT_ID3_)
		uint8 i = 0;
		for(i = 0; i < PLAY_ID3_MAX; i++)
		{
			play_info.id3_info[i] = music_ctrl->play_info.id3_info[i];
			play_info.id3_len[i]  = music_ctrl->play_info.id3_len[i];
			//music_printf("play_info.id3_info=%x,play_info.id3_len=%x",play_info.id3_info[i],play_info.id3_len[i]);
		}
	#endif
	}

	//ew_printf("music_device:0x%x\n",music_device);
	//ew_printf("music_cur_filetotal:%d\n",music_cur_filetotal);
	//ew_printf("music_cur_filenum:%d\n",music_cur_filenum);
	//ew_printf("music_playfile_total_time:%d\n",music_playfile_total_time);
	//ew_printf("play_time:%d\n",play_time);
	if(memcmp(&music_ctrl->play_info, &play_info, sizeof(music_play_info_t)))
	{
	#ifdef TD_ADD_SPP
    	if((music_device == SD0_DEVICE) || (music_device == SD1_DEVICE))
    	{
    		setup.sd_total_time = music_playfile_total_time;
    	}
    	else if(music_device == USB_DEVICE)
    	{
    		setup.usb_total_time = music_playfile_total_time;
    	}		
	#ifdef TD_ADD_NEW_APP
        //printf("11111:0x%08x\r\n",play_file_clust);
	#if defined(_DISABLE_MUSIC_LIST_)
		new_app_MusicInfo(music_device,music_cur_filetotal,music_cur_filenum,music_playfile_total_time,play_time,play_mode,play_info.state,0);
	#else
		new_app_MusicInfo(music_device,music_cur_filetotal,music_cur_filenum,music_playfile_total_time,play_time,play_mode,play_info.state,play_file_clust);
	#endif
		read_list_ready_flag=TRUE;
	#else
		Spp_Tx_MusicInfo(music_device,music_cur_filetotal,music_cur_filenum,music_playfile_total_time,play_time);
	#endif
	#endif
	#if defined(_USER_TFT_SEND_)
		TFT_Tx_Music_Clock_songe_Info(music_device,music_playfile_total_time,play_time,music_cur_filetotal,music_cur_filenum);
	#endif
		memcpy(&music_ctrl->play_info, &play_info, sizeof(music_play_info_t));

		disp_send_param(DISP_EVENT_PLAY, &music_ctrl->play_info);
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
#if (SPEED_PITCH_EN)
static void *music_decoder_output_to_speed_pitch(void *priv, void *buf, u32 len)
{
	PS_OBJ *obj = (PS_OBJ *)priv;
	speed_pitch_main(obj, buf, len);
	return priv;		
}
static u32 music_decoder_set_speed_pitch_info(void *priv, dec_inf_t *inf, tbool wait)
{
	MUSIC_OP_API *mapi = (MUSIC_OP_API *)priv;
	speed_pitch_set_samplerate((PS_OBJ *)mapi->ps_api, inf->sr);
	if(mapi->dop_api->io->set_info._cb)
	{
		printf("\n--func=%s, line=%d\n", __FUNCTION__, __LINE__);	
		mapi->dop_api->io->set_info._cb(mapi->dop_api->io->set_info.priv, inf, wait);
	}
	return 0;		
}
static tbool music_decoder_set_speed_pitch_enable(MUSIC_OP_API *mapi)
{
	if(mapi == NULL)		
		return false;
	PS_OBJ *tmp_ps_obj = speed_pitch_creat();
	if(tmp_ps_obj == NULL)
	{
		mapi->ps_api = NULL;	
		printf("\n--func=%s, line=%d\n", __FUNCTION__, __LINE__);
		return false;
	}

	speed_pitch_set_output(tmp_ps_obj, mapi->dop_api->io->output.output, mapi->dop_api->io->output.priv);
	mapi->dop_api->io->output.priv = (void *)tmp_ps_obj;
	mapi->dop_api->io->output.output = (void *)music_decoder_output_to_speed_pitch;

	speed_pitch_set_set_info_cbk(tmp_ps_obj, music_decoder_set_speed_pitch_info, (void *)mapi);
	speed_pitch_set_track_nums(tmp_ps_obj, 2);
	speed_pitch_set_speed(tmp_ps_obj, PS_SPEED_DEFAULT_VAL);
	speed_pitch_set_pitch(tmp_ps_obj, PS_PITCHT_DEFAULT_VAL);

	mapi->ps_api = (void *)tmp_ps_obj;

	return true;
}
static void music_decoder_set_speed(MUSIC_OP_API *mapi, u16 val)
{
	if(mapi == NULL)
		return ;
	speed_pitch_set_speed((PS_OBJ *)mapi->ps_api, val);
}

static void music_decoder_set_pitch(MUSIC_OP_API *mapi, u16 val)
{
	if(mapi == NULL)
		return ;
	speed_pitch_set_pitch((PS_OBJ *)mapi->ps_api, val);
}

static u16 music_decoder_get_cur_speed(MUSIC_OP_API *mapi)
{
	if(mapi == NULL)
		return 0;
	return speed_pitch_get_cur_speed((PS_OBJ *)mapi->ps_api);		
}

static u16 music_decoder_get_cur_pitch(MUSIC_OP_API *mapi)
{
	if(mapi == NULL)
		return 0;
	return speed_pitch_get_cur_pitch((PS_OBJ *)mapi->ps_api);		
}


const u16 speed_tab[5] = 
{
	PS_SPEED_DEFAULT_VAL,
	PS_SPEED_DEFAULT_VAL + 10,
	PS_SPEED_DEFAULT_VAL + 20,	
	PS_SPEED_DEFAULT_VAL - 20,
	PS_SPEED_DEFAULT_VAL - 10,
};
const u16 pitch_tab[5] = 
{
	PS_PITCHT_DEFAULT_VAL,
	PS_PITCHT_DEFAULT_VAL + 10000,
	PS_PITCHT_DEFAULT_VAL + 20000,	
	PS_PITCHT_DEFAULT_VAL - 20000,
	PS_PITCHT_DEFAULT_VAL - 10000,
};
static u8 speed_mode = 0;
static u8 pitch_mode = 0;
#endif//SPEED_PITCH_EN
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/

static MUSIC_OP_API *music_decoder_init(void)
{
	MUSIC_OP_API *mapi = NULL;
    u32 err;

    mapi = malloc_fun(mapi, sizeof(MUSIC_OP_API), 0);
    if(NULL == mapi)
    {
        music_puts("if(NULL == mapi)\n");
        while(1);
    }

	/*-------解码接口资源申请----------------*/
    mapi->dop_api = malloc_fun(mapi->dop_api, sizeof(_MUSIC_API), 0);
    if(NULL == mapi->dop_api)
    {
        music_puts("if(NULL == mapi->dop_api)\n");
        while(1);
    }

    /*-------文件操作器资源申请--------------*/
    mapi->fop_api = malloc_fun(mapi->fop_api, sizeof(FILE_OPERATE), 0);
    if(NULL == mapi->fop_api)
    {
        music_puts(" if(NULL==mapi->fop_api)\n");
        while(1);
    }

	//使能响应的解码库
#if	defined(_DISABLE_FLAC_APE_)
    mapi->dop_api->dec_api.type_enable = DEC_PHY_MP3 | DEC_PHY_WAV |  \
    									 DEC_PHY_WMA;

#elif defined(_DISABLE_PLAY_WMA_WAV_)
    mapi->dop_api->dec_api.type_enable = DEC_PHY_MP3 | DEC_PHY_FLAC |  \
										 DEC_PHY_APE ;
#else
    mapi->dop_api->dec_api.type_enable = DEC_PHY_MP3 | DEC_PHY_WAV |  \
										 DEC_PHY_WMA | DEC_PHY_FLAC | \
										 DEC_PHY_APE ;
#endif

    mapi->dop_api->file_type = (void*)dec_file_ext;

    mapi->dop_api->dec_api.dec_phy_name = MUSIC_DEC_PHY_NAME;
    err = os_task_create_ext(decoder_phy_task, 0, TaskMusicPhyDecPrio, 5, mapi->dop_api->dec_api.dec_phy_name, MUSIC_PHY_TASK_STACK_SIZE);
    if(OS_NO_ERR != err)
    {
        music_printf("decoder_phy_task err=%08x\n", err);
        while(1);
    }

    if(NULL == mapi->dop_api->io)
    {
        mapi->dop_api->io = malloc_fun(mapi->dop_api->io, sizeof(DEC_API_IO), 0);
        if(NULL == mapi->dop_api->io)
        {
            music_puts(" if(NULL == mapi->dop_api->dec_io)\n");
            while(1);
        }
    }

    mapi->dop_api->io->father_name = MUSIC_TASK_NAME;//解码结束的消息返回给谁
    mapi->dop_api->file_num = 1;
    mapi->dop_api->dec_api.eq = eq_mode;

#if defined(_DISABLE_RPT_AND_RDM_)
#if defined(_BT_CALL_MEMORY_RPT_)
	if(global.memory_rpt)
	{
		music_ctrl->intro_flag=FALSE;
		play_mode = REPEAT_ALL;
	}
	else
	{
		global.memory_rpt = 1;
	}
#else
	music_ctrl->intro_flag=FALSE;
	play_mode = REPEAT_ALL;
#endif
#if defined(_ENABLE_RPT_RDM_FUNTION_)
	setup.rdm_start = FALSE;
	setup.rpt_start = FALSE;
#endif
#endif

    /*-------文件操作器初始化信息资源申请--------------*/
    mapi->fop_api->fop_init = malloc_fun(mapi->fop_api->fop_init, sizeof(FILE_OPERATE_INIT), 0);
    if(mapi->fop_api->fop_init == NULL)
    {
        music_puts("mem err fop_init\n");
        while(1);
    }
    mapi->fop_api->fop_init->cur_play_mode = play_mode;
    mapi->dop_api->brk = malloc_fun(mapi->dop_api->brk,sizeof(FS_BRK_POINT),0);
    mapi->dop_api->dec_api.bp_buff = &mapi->dop_api->brk->inf.norm_brk_buff[0];
    mapi->dop_api->dec_api.flac_bp_buff = &mapi->dop_api->brk->inf.flac_brk_buff[0];

	mapi->dop_api->io->set_info.priv = NULL;
	mapi->dop_api->io->set_info._cb = (void *)set_music_sr;

	mapi->dop_api->io->output.priv = NULL;
	mapi->dop_api->io->output.output = sup_dec_phy_op;

	mapi->dop_api->dec_api.op_priv = NULL;
	mapi->dop_api->dec_api.dec_op_clr = (void *)sup_dec_op_clear;
	mapi->dop_api->dec_api.dec_op_over = (void *)sup_dec_op_over;
		
#if (SPEED_PITCH_EN)
	music_decoder_set_speed_pitch_enable(mapi);
	music_decoder_set_speed(mapi, speed_tab[speed_mode]);
	music_decoder_set_pitch(mapi, pitch_tab[pitch_mode]);
#endif//SPEED_PITCH_EN

    return mapi;
}

void music_stop_decoder(MUSIC_OP_API *parm)
{
    _MUSIC_API *mapi;
	u32 err;

	music_puts("music_stop_decoder\n");

	mapi= parm->dop_api;
	err = stop_decoder(&mapi->dec_api);

	//music_puts("music_stop_decoder1111\n");
    if(STOP_DEC_GOT_BRKINFO & err)
    {
		//music_puts("music_stop_decoder222\n");
        file_operate_ctl(FOP_SAVE_BPINFO,parm->fop_api,&mapi->brk->inf,0);
    }
    else if(STOP_DEC_GOT_FLACBRKINFO & err)
    {
		//music_puts("music_stop_decoder333\n");
        file_operate_ctl(FOP_SAVE_FLACBPINFO,parm->fop_api,&mapi->brk->inf,0);
    }
#if USB_DISK_EN
    usb->dev_io->ioctrl(0,DEV_SET_REMAIN);
#endif
	//music_puts("music_stop_decoder444\n");
}

static void before_decoder_callback(MUSIC_OP_API *m_op_api,u32 dev_let)
{
    if(dev_let != 'A')
    {
	#if LRC_LYRICS_EN
        if(0 == lrc_find(m_op_api))
        {

        }
        else
        {
            music_puts("lrc_init err\n");
        }
	#endif
    }
    else
    {
        music_puts("play a music\n");
    }

#if MUSIC_DECRYPT_EN
	get_decode_file_info(m_op_api->dop_api->io->f_p);
#endif
	vm_check_all(0);
}

static void after_decoder_callback(MUSIC_OP_API *m_op_api,u32 dev_let)
{
	if(0 == strcmp(m_op_api->dop_api->dec_api.phy_ops->dec_ops->name, "FLA"))//瑙ｇ爜鍣‵LAC
	{
		set_sys_freq(FLAC_SYS_Hz);
	}
	else if(0 == strcmp(m_op_api->dop_api->dec_api.phy_ops->dec_ops->name, "APE"))//瑙ｇ爜鍣ˋPE
	{
		set_sys_freq(APE_SYS_Hz);
	}
	else
	{
		set_sys_freq(SYS_Hz);
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
static void music_channel_enter(void)
{  
#if defined(_ENABLE_SPECIAL_MUTE_)
	sys_mute_ctrl(DISABLE, 1);
#elif defined(_ENABLE_REJECT_MUTE_)
	sys_mute_ctrl(ENABLE, 3000);//1500
#elif defined(_DISABLE_SPECIAL_MUTE)
     sys_mute_ctrl(DISABLE, 1);
#else
	sys_mute_ctrl(ENABLE, 1500);
#endif

	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_ON, TRUE);
#if defined(JYL3861E_CHANNEL_PLAY_GATE)||defined(_W116_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#elif defined(_FY5517_REDUCE_AUD_GATE_)||defined(_HY3002_CHANNEL_PLAY_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#elif defined(_HKY1032_REDUCE_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#elif defined(_XM007_REDUCE_AUD_GATE_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#else
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
#endif

#if defined(_DECODER_MUSIC_GAIN_)
	audio_drv_ctrl(AUD_CTRL_LR, setup.aud_bal, TRUE);
	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
#endif

#if defined(_SUPPORT_TWO_PT2313_)
    audio_drv_ctrl1(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_ON, TRUE);
    audio_drv_ctrl1(AUD_CTRL_GATE, AUDIO_GATE_PLUS_3_75, TRUE);
#endif

#if defined(_ENABLE_POWER_LOGO_)
	global.power_up_flag = FALSE;
#endif
}

static void music_channel_exit(void)
{
#if !defined(_ENABLE_SPECIAL_MUTE_)
	sys_mute_ctrl(ENABLE, 0);
#endif

#if defined(_ENABLE_BT_FIRST_)
#if defined(_AUDIO_DRIVER_AP3768_)
	if(global.rds_ta)
	{
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RDS_OFF, TRUE);
		OSTimeDly(10);
	}
#endif
#endif
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_OFF, TRUE);

#if defined(_SUPPORT_TWO_PT2313_)
	audio_drv_ctrl1(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_MUSIC_OFF, TRUE);
#endif
}

#ifdef TD_ADD_ALEN
u8 gDir_disp_flag=FALSE;
#endif
static uint32 music_play_api(MUSIC_OP_API *m_op_api,ENUM_DEV_SEL_MODE dev_sel,uint32 dev_let,uint32 file_sel,void *param)
{
    uint32 err;

	//music_puts("play api in\n");
#ifdef TD_ADD_ALEN
    if((file_sel ==PLAY_NEXT_FOLDER)||(file_sel ==PLAY_PRE_FOLDER))
    {
        gDir_disp_flag =TRUE;//显示文件夹序号
        
	#if defined(_DIR_NAME_BUG_)
		global.first_flag = FALSE;
	#endif
    }
    else
    {
	    gDir_disp_flag =FALSE;
    }
#endif
	err = music_play(m_op_api, dev_sel, dev_let, (ENUM_FILE_SELECT_MODE)file_sel, param);

	//music_printf("err:%d\n",err);
#if defined(_SUPPORT_ID3_)
#if defined(_MUSICID3_ADJUST_)
	extern void free_file_id3_buf(void);
	free_file_id3_buf();
#endif
	mp3_head->have_id3 = 0;
	mp3_head->artist = NULL;
	mp3_head->artist_len = 0;
	mp3_head->title = NULL;
	mp3_head->title_len = 0;
	mp3_head->album = NULL;
	mp3_head->album_len = 0;
//	  printf("mp3_head->id3_buf malloc = %x byte----\n",mp3_head->id3_buf);
//	  printf("id3->len = %d byte\n",mp3_head->id3_len);
//	  put_buf(mp3_head->id3_buf,mp3_head->id3_len);
   // printf("id3_1->len = %d byte\n",id3_v1_head.id3_len);
   // put_buf(id3_v1_head.id3_buf,id3_v1_head.id3_len);
	//printf("id3_2->len = %d byte\n",id3_v2_head.id3_len);
   // put_buf(id3_v2_head.id3_buf,id3_v2_head.id3_len);
#endif

    if(err == SUCC_MUSIC_START_DEC)
    {
    #if defined(_SUPPORT_SD_)
    	if((music_device == SD0_DEVICE) || (music_device == SD1_DEVICE))
		{
			//music_printf("11111music_device= %d\n",music_device);
			setup.sd_break = TRUE;
		}
		else
	#endif
		if(music_device == USB_DEVICE)
		{
			//music_printf("2222music_device= %d\n",music_device);
			setup.usb_break = TRUE;
		}

		u32 t_fnum; 	///<文件夹总数（根目录为1）
		u32 c_fnum; 	///<当前文件夹编号（根目录为1）
		file_operate_ctl(FOP_GET_FOLDER_INFO,m_op_api->fop_api,&t_fnum,&c_fnum);
		//music_printf("t_fnum = %d\n",t_fnum);
		//music_printf("c_fnum = %d\n",c_fnum);
		music_ctrl->play_info.total = m_op_api->fop_api->cur_lgdev_info->total_file_num;
        music_dir_number =c_fnum;
	#if defined(_ENABLE_FOLD_TOTAL_)
		music_dir_total =t_fnum;
	#endif
	
#ifdef  _SUPPORT_ID3_
	if(id3_v1_head.id3_len){
		//get_id3_v1
	}

	if(id3_v2_head.id3_len){
		//get_id3_v2
	}
#if defined(_MUSICID3_ADJUST_)
	extern u8 * get_file_id3_buf(void);
	if(NULL != get_file_id3_buf()){
       get_music_tag();
	}
#else
      get_music_tag();
#endif
#endif

   	#if defined(_SUPPORT_ID3_)
		music_file_info(m_op_api);
		//music_file_id3(m_op_api);
	#endif

		music_disp_info(m_op_api);
	#ifdef	TD_ADD_ALEN
		decode_ok =1;
	#endif
    }

#ifdef	TD_ADD_ALEN
	else
	{
		decode_ok =0;
	}
#endif
	 if(err != FILE_OP_NO_ERR)
	{
	#if defined(_SUPPORT_ID3_)
		mp3_head->id3_buf = NULL;
		mp3_head->id3_len = 0;
	#endif
	}

    return err;
}

static uint32 music_play_by_sclust(MUSIC_OP_API *mapi,uint32 sclust)
{
    printf("-----music_play_by_sclust:%d\n",sclust);
    uint32 err;
    mapi->fop_api->fop_init->filesclust = sclust;
    err = music_play_api(mapi,DEV_SEL_CUR,0,FOP_OPEN_FILE_BYSCLUCT,&(mapi->dop_api->file_num));
    return err;
}

#ifdef	TD_ADD_ALEN
extern const struct task_info *task_list[];
DEV_HANDLE music_next_dev_type(void)
{
	DEV_HANDLE next_dev_hl;
	next_dev_hl = (DEV_HANDLE)file_operate_get_next_phydev();
	if(next_dev_hl == usb) {
		puts("next_dev:usb\n");
		curr_task = (struct task_info *) task_list[2];
	}
	else if(next_dev_hl == sd1) {
		puts("next_dev:sd\n");
		curr_task = (struct task_info *) task_list[3];
	}
	else {
		puts("next_dev:NULL\n");
	}
	return next_dev_hl;
}
#endif
static void music_timer(void *param)
{
	os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_TIMER);
}


u8 cancel_FR_flags = 0;
static void music_task(void *p)
{
    MUSIC_OP_API *mapi = NULL;
    uint32 err;
    uint8 enable = 1;
    uint8 file_auto_next = 1;
    uint8 wait_exit = 0;
	uint8 wait_ok=0;
    int msg[6];
	music_play_info_t play_info;
#if defined(_ENABLE_RADIO_RDS_)
	uint8 rds_pause = FALSE;
#endif
#ifdef	TD_ADD_ALEN
	decode_ok =0;
#endif
	Interrupt =FALSE;
    music_puts("\n************************music TASK********************\n");
#ifdef  _SUPPORT_ID3_
    mp3_head = malloc_fun(mp3_head,sizeof(MP3_ID3_INFO),0);
    if(NULL == mp3_head){
        puts("NULL == mp3_head\n");
        while(1);
    }
    reg_get_id3v2_buf(music_get_file_id3);
#endif

	reg_get_dev_change(get_dev_change);
	music_reverb = NULL;
	music_ctrl = malloc_fun(music_ctrl, sizeof(music_ctrl_t), 0);
	if(NULL == music_ctrl)
    {
    	music_puts("music_ctrl malloc err!!\n");
        while(1);
	}

#if !defined (_ENABLE_USB_SAVE_)
#if !defined (_ENABLE_USB_DELAY_)
#if defined(_SUPPORT_SD_)
	if((music_device == SD0_DEVICE)||(music_device == SD1_DEVICE))
	{
		disp_send_msg(DISP_EVENT_SD);
#if defined(_USER_TFT_SEND_)
	setup.sd_standby = TRUE;
#endif
	}
	else
#endif
	if(music_device == USB_DEVICE)
	{
		disp_send_msg(DISP_EVENT_USB);

#if defined(_USER_TFT_SEND_)
	setup.usb_standby = TRUE;
#endif
	}
#endif
#endif

#if defined(_USER_TFT_SEND_)
	TFT_Tx_System_Status();
#endif

#if MUSIC_DECRYPT_EN
	cipher_init(0x12345678);
#endif

#if EQ_EN
    eq_enable();
    eq_mode = get_eq_default_mode();
#endif

#if defined(_AUDIO_USE_DECODER_)
	if(setup.aud_eq == 0)
	{
		audio_drv_ctrl(AUD_CTRL_BASS,	setup.aud_bass, 	FALSE);
		audio_drv_ctrl(AUD_CTRL_TREBLE, setup.aud_treble,	FALSE);
	}
	else
	{
		audio_drv_ctrl(AUD_CTRL_EQ, 	setup.aud_eq,		FALSE);
	}
	//audio_drv_ctrl(AUD_CTRL_LOUD, setup.aud_loud, 	FALSE);
#endif

	play_info.pause_flag = FALSE;

	//music_puts("music_decoder_init\n");

    mapi = music_decoder_init();
	music_decoder_info(before_decoder_callback, after_decoder_callback);

#if LRC_LYRICS_EN
    lrc_init();
#endif

#if defined(_SUPPORT_ID3_)
	music_file_id3_state(ON);
#endif

	if(setup.usb_standby)
	{
		usb->dev_io->mount(NULL);
		setup.usb_standby = FALSE;
	}
	if(setup.sd_standby)
	{
		sd1->dev_io->mount(NULL);
		setup.sd_standby = FALSE;
	}

	music_channel_enter();

	os_taskq_post_msg(MUSIC_TASK_NAME, 1, SYS_EVENT_BEGIN_DEC);
	//timer_reg_func(music_timer, NULL, MUSIC_POLLING_TIME);

	music_operate_mapi = mapi;
	//music_puts("music start ...\n");
#ifdef	TD_ADD_SPP
	Spp_Tx_Music();
#endif
#if defined(_USER_TFT_SEND_)
	OSTimeDly(10);
	TFT_Tx_System_Status();
#endif

#if defined(ACC_OFF_REMEBER_PLAY_STATE)
	if(setup.memory_play)
	{
		play_mode = setup.play_flag;
		setup.memory_play = FALSE;
		if(setup.rdm_flag)
        {
            mapi->fop_api->fop_init->cur_play_mode = play_mode;
    	    disp_send_msg(MSG_MUSIC_RDM);
			setup.rdm_flag = FALSE;
    	}
		else if(setup.rpt_flag)
		{
		    mapi->fop_api->fop_init->cur_play_mode = play_mode;
            disp_send_msg(MSG_MUSIC_RPT);
			setup.rpt_flag = FALSE;
		}
	}
#endif

#if defined(_ACC_OFF_MEMORY_INT_)
    if(setup.int_flag)
    {
        music_ctrl->intro_flag = TRUE;
	    disp_send_msg(MSG_MUSIC_INT);
		setup.int_flag = FALSE;
	}
#endif
#if defined(_MODE_MEMORY_INT_)
    if(global.int_memory)
    {
        music_ctrl->intro_flag = TRUE;
	    disp_send_msg(MSG_MUSIC_INT);
		global.int_memory = FALSE;
	}
#endif
	while(1)
    {
        memset(msg, 0x00, sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
		music_reset_common(msg[0], msg[1]);

        err = 0;
        if ((!enable) && (SYS_EVENT_DEL_TASK != msg[0]))
        {
            continue;
        }

        mapi->dop_api->dec_api.save_brk = 0;

        if(wait_exit)
        {//<进入删除状态，不再响应其他消息
			if(wait_ok)
			{
				err = os_taskq_post(MAIN_TASK_NAME, 1, MSG_CHANGE_WORKMODE);
				if(err == OS_NO_ERR)
				{
					wait_ok = 0;
				}
			}
            if(msg[0] != SYS_EVENT_DEL_TASK)
            {
                music_printf("msg ddddd %08x\n", msg[0]);
                continue;
            }
        }

		if(cancel_FR_flags)
		{
			if(msg[0] == MSG_MUSIC_FR)continue;
			if(msg[0] == MSG_MUSIC_FFR_DONE)
			{
				music_puts("clear cancel_FR_flags\n");
				cancel_FR_flags = 0;
			}
		}

		switch(msg[0])
        {
        	case SYS_EVENT_DEL_TASK:	//请求删除music任务
        		music_puts("music----SYS_EVENT_DEL_TASK---\n");
			#ifdef	TD_ADD_ALEN
				decode_ok =0;
			#endif
			#ifdef	TD_ADD_NEW_APP
			#if !defined(_DISABLE_MUSIC_LIST_)
				file_brose_close();
			#endif
			#endif
				stop_flag = FALSE;
				music_channel_exit();
				timer_unreg_func(music_timer);

				mapi->dop_api->dec_api.save_brk = 1;
	            music_stop_decoder(mapi);
			#if defined(DISABLE_PLAYMODE_MEMORY)
				music_ctrl->intro_flag=FALSE;
                play_mode=REPEAT_ALL;
		    #endif
			
            #if defined(_NO_REMEMBER_PLAY_STATUS_)
				if(global.play_status_flag)
				{
					music_ctrl->intro_flag = FALSE;
					play_mode=REPEAT_ALL;
					global.play_status_flag = FALSE;
				}
            #endif
				music_operate_mapi = NULL;
				set_sys_freq(SYS_Hz);

			#if defined(_SUPPORT_ID3_)
				music_file_id3_state(OFF);
			#endif
			#if MUSIC_DECRYPT_EN
				cipher_close();
			#endif
			#if EQ_EN
	            eq_disable();
			#endif

	            if(mapi->dop_api->dec_api.dec_phy_name)
	            {
	                if(os_task_del_req(mapi->dop_api->dec_api.dec_phy_name) != OS_TASK_NOT_EXIST)
	                {
	                    os_taskq_post_event(mapi->dop_api->dec_api.dec_phy_name, 1, SYS_EVENT_DEL_TASK);
	                    do{
	                        OSTimeDly(1);
	                    }while(os_task_del_req(mapi->dop_api->dec_api.dec_phy_name) != OS_TASK_NOT_EXIST);
	                    mapi->dop_api->dec_api.dec_phy_name = NULL;
	                    music_puts("del music dec phy succ11\n");
	                }
	            }
	            if(mapi->dop_api->dec_api.ef_enable && mapi->dop_api->dec_api.ef_out_name)
	            {
	                if(os_task_del_req(mapi->dop_api->dec_api.ef_out_name) != OS_TASK_NOT_EXIST)
	                {
	                    os_taskq_post_event(mapi->dop_api->dec_api.ef_out_name, 1, SYS_EVENT_DEL_TASK);
	                    do{
	                        OSTimeDly(1);
	                    }while(os_task_del_req(mapi->dop_api->dec_api.ef_out_name) != OS_TASK_NOT_EXIST);
	                    mapi->dop_api->dec_api.ef_out_name = NULL;
	                    music_puts("del music ef_out_name succ\n");
	                }
	            }
	            enable = 0;
			#if LRC_LYRICS_EN
	            lrc_exit();
			#endif

	            ///<关闭文件操作器，释放资源
	            file_operate_ctl(FOP_CLOSE_LOGDEV,mapi->fop_api,0,0);
	            ///<释放文件操作器初始化信息

			#if !defined(_ENABLE_SPECIAL_MUTE_)
	            sys_mute_ctrl(ENABLE, 0);
			#endif

	            if(mapi->fop_api)
	            {
	                if(mapi->fop_api->fop_init)
	                {
	                    free_fun((void**)&mapi->fop_api->fop_init);
	                }
	                free_fun((void**)&mapi->fop_api);
	            }
				
				if(mapi->ps_api)
    			{
    				free_fun((void**)&mapi->ps_api);
    			}
				
	            free_fun((void**)&mapi->dop_api->brk);
	            free_fun((void**)&mapi->dop_api->io);
	            free_fun((void**)&mapi->dop_api);
	            free_fun((void**)&mapi);
			#ifdef  _SUPPORT_ID3_
                free_fun((void**)&mp3_head);
			#if defined(_MUSICID3_ADJUST_)
			    extern void free_file_id3_buf(void);
				free_file_id3_buf();
			#endif
            #endif

	            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
	                os_task_del_res_name(OS_TASK_SELF);//确认可删除，此函数不再返回
				}
				break;

	        case SYS_EVENT_PLAY_SEL_END:
	            music_puts("music----SYS_EVENT_PLAY_SEL_END---\n");

	        case SYS_EVENT_BEGIN_DEC:
				music_puts("music----SYS_EVENT_BEGIN_DEC---\n");
	            mapi->dop_api->dec_api.save_brk = 0;
			#if defined(_SUPPORT_SD_)
				if(music_device == SD0_DEVICE)
				{
					music_puts("music begin sd0 card dec\n");
					err = music_play_api(mapi,DEV_SEL_SPEC,(u32)(sd0),setup.sd_break ? PLAY_BREAK_POINT : PLAY_FIRST_FILE,&(mapi->dop_api->file_num));
				}
				else if(music_device == SD1_DEVICE)
				{
					music_puts("music begin sd1 card dec\n");
					err = music_play_api(mapi,DEV_SEL_SPEC,(u32)(sd1),setup.sd_break ? PLAY_BREAK_POINT : PLAY_FIRST_FILE,&(mapi->dop_api->file_num));
				}
				else
			#endif
				if(music_device == USB_DEVICE)
				{
					music_printf("1111music begin usb dec\n");
					ew_printf("setup.usb_break:%d\n",setup.usb_break);
					err = music_play_api(mapi,DEV_SEL_SPEC,(u32)(usb),setup.usb_break ? PLAY_BREAK_POINT : PLAY_FIRST_FILE,&(mapi->dop_api->file_num));
				}
				else
				{
					music_puts("2222music begin dec\n");
                	err = music_play_api(mapi,DEV_SEL_SPEC,0,PLAY_BREAK_POINT,&(mapi->dop_api->file_num));
				}
		    #if defined(_ENABLE_RADIO_TIME_)
		    	timer_reg_func(music_timer, NULL, 1000);
		    #else
		    	timer_reg_func(music_timer, NULL, MUSIC_POLLING_TIME);
		    #endif
				break;

	        case SYS_EVENT_DEC_SET_END:
	            music_puts("music----SYS_EVENT_DEC_SET_END---\n");
	            break;

	        case SYS_EVENT_DEC_FR_END:
	            music_puts("music----SYS_EVENT_DEC_FR_END---\n");
	            music_printf("decoder stop file end:%d\n",mapi->dop_api->file_num);
	            err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
				if(err == FILE_OP_NO_ERR )
				{
					cancel_FR_flags = 1;
				}
				break;

	        case SYS_EVENT_DEC_FF_END:
	        case SYS_EVENT_DEC_END:
	            music_puts("music----SYS_EVENT_DEC_FF_END---\n");
	            music_puts("decoder stop\n");
	            err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_AUTO_NEXT,&(mapi->dop_api->file_num));
	            break;

	        case SYS_EVENT_DEV_OFFLINE:
	            music_printf("music----SYS_EVENT_DEV_OFFLINE---\n");
				{
		            lg_dev_info *tmplgdev = mapi->fop_api->cur_lgdev_info;
		            if(tmplgdev)
		            {
		                if(tmplgdev->lg_hdl->phydev_item == (void *)msg[1])
		                {
		                    if((!mapi->dop_api->dec_api.phy_ops) || (DECODER_PAUSE != mapi->dop_api->dec_api.phy_ops->status))
		                    {//< 解码设备非暂停状态，有解码自己报错
								if(stop_start)
								{
									music_printf("music----SYS_EVENT_DEV_OFFLINE555---\n");
								#ifdef	TD_ADD_ALEN
									decode_ok =0;
								#endif
									stop_start = FALSE;
    								mapi->dop_api->dec_api.save_brk = 1;
    								mapi->dop_api->file_num = 1;
									music_stop_decoder(mapi);
									file_operate_ctl(FOP_CLOSE_LOGDEV,0,0,0);
									wait_exit = 1;
									wait_ok = 1;
									global.charge_flag = TRUE;
									err = FILE_OP_NO_ERR;
								}
		                        break;
		                    }//else的解码暂停状态，有下一个消息SYS_EVENT_DEC_DEVICE_ERR处理
		                }
		                else
		                {
		                    //<非解码设备
							break;
		                }
		            }
		            else
					{
						break;
		            }
	            }
				
	        case SYS_EVENT_DEC_DEVICE_ERR:
	            music_puts("music----SYS_EVENT_DEC_DEVICE_ERR---\n");
			#ifdef	TD_ADD_ALEN
				decode_ok =0;
			#endif
			
		    #if defined(_ENABLE_USB_MODE_)
			#if !defined(_EVB_USB_CHANGE_MODE_)
		    	global.charge_flag = TRUE;
		    #endif
			#endif

    			mapi->dop_api->dec_api.save_brk = 1;
    			mapi->dop_api->file_num = 1;
    		#ifdef	TD_ADD_ALEN
    			//music_next_dev_type();
    		#endif
    			music_stop_decoder(mapi);
    			file_operate_ctl(FOP_CLOSE_LOGDEV,0,0,0);
    			wait_exit = 1;
    			wait_ok = 1;
    			err = FILE_OP_NO_ERR;
	            break;
		#if defined(_ENABLE_RADIO_RDS_)
			case SYS_EVENT_TA_ENTER:
				music_puts("music----SYS_EVENT_TA_ENTER---\n");
				if(mapi->dop_api->dec_api.phy_ops->status == DECODER_PLAY)
				{
					rds_pause = TRUE;
					pp_decode(&mapi->dop_api->dec_api);
				}
				break;

			case SYS_EVENT_TA_EXIT:
				music_puts("music----SYS_EVENT_TA_EXIT---\n");
				if(rds_pause)
				{
					rds_pause = FALSE;
					if(mapi->dop_api->dec_api.phy_ops->status == DECODER_PAUSE)
					{
						pp_decode(&mapi->dop_api->dec_api);
					}
				}
				break;
		#endif

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	        case MSG_MUSIC_NEXT_FILE://下一曲
		    #if defined(_ENABLE_RCA_MUTE_)//修改放音RCA有杂音
				set_sub_mute();
				OSTimeDly(2);
		    #endif
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			
		    #if defined(_XM007_MUSIC_BEEP_)
		    	if(global.aud_mute)
		    	{
					sys_mute_ctrl(ENABLE, 1000);
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif

		    #if defined(_ENABLE_A5900_USB_)
		    	if((music_ctrl->ffr_state = PLAY_STATUS_FORWARD)
		    		||(music_ctrl->ffr_state = PLAY_STATUS_BACKWARD))
		    	{
		    		sys_mute_ctrl(DISABLE, 0);
		    		music_ctrl->ffr_state = 0;
		    	}
		    #endif

			#ifdef	TD_ADD_LCD_TEST
				gDispPos --;
				printf("gDispPos=%d\n",gDispPos);
			#else
	            music_puts("KEY_EVENT_NEXT\n");
			#if defined(_WLS_SPECIAL_MUTE_)
				sys_mute_ctrl(ENABLE, 1300);
			#else
				sys_mute_ctrl(ENABLE, 500);
			#endif
				file_auto_next = 1;
				play_info.pause_flag = FALSE;
				music_puts("-----MSG_MUSIC_NEXT_FILE\n");
	            err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FILE,&(mapi->dop_api->file_num));
			#endif
			#if defined(_NO_MEMORY_PLAY_MODE)
				music_ctrl->intro_flag = FALSE;
				play_mode=REPEAT_ALL;
			#endif
	            break;

	        case MSG_MUSIC_PREV_FILE://上一曲
		    #if defined(_ENABLE_RCA_MUTE_)//修改放音RCA有杂音
		    	set_sub_mute();
				OSTimeDly(2);
		    #endif
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			
			#if defined(_XM007_MUSIC_BEEP_)
    			if(global.aud_mute)
    			{
					sys_mute_ctrl(ENABLE, 1000);
    				global.aud_mute = !global.aud_mute;
    				func_mute_control();
    			}
			#endif
			
            #if defined(_ENABLE_A5900_USB_)
            	if((music_ctrl->ffr_state = PLAY_STATUS_FORWARD)
            		||(music_ctrl->ffr_state = PLAY_STATUS_BACKWARD))
            	{
            		sys_mute_ctrl(DISABLE, 0);
            		music_ctrl->ffr_state = 0;
            	}
            #endif

			#ifdef	TD_ADD_LCD_TEST
				gDispPos ++;
				printf("gDispPos=%d\n",gDispPos);
			#else
	            music_puts("KEY_EVENT_PREV\n");
		    #if defined(_WLS_SPECIAL_MUTE_)
		    	sys_mute_ctrl(ENABLE, 1300);
		    #else
		    	sys_mute_ctrl(ENABLE, 500);
		    #endif
				file_auto_next = 0;
				play_info.pause_flag = FALSE;
			#if defined(_SPECIAL_PREV_FILE_)
				if((music_ctrl->play_info.min < 1) && (music_ctrl->play_info.sec < 5))
				{
					global.prev_flag = FALSE;
					err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_PREV_FILE,&(mapi->dop_api->file_num));
				}
				else if((music_ctrl->play_info.min > 0) || (music_ctrl->play_info.sec > 4))
				{
					global.prev_flag = TRUE;
					err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
				}
			#else
				err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_PREV_FILE,&(mapi->dop_api->file_num));
			#endif
			#endif
			#if defined(_NO_MEMORY_PLAY_MODE)
				music_ctrl->intro_flag = FALSE;
				play_mode=REPEAT_ALL;
			#endif
	            break;

			case MSG_MUSIC_PP://播放、暂停
			case MSG_MUSIC_PLAY:
    		case MSG_MUSIC_PAUSE:
				{
					music_puts("MSG_MUSIC_PP\n");
					stop_start = FALSE;
				#if defined(SYS_MODE_CHANG_NOPP)
					if(!global.music_pp_enable)
					{
						break;
					}
				#endif
			    #if defined(_XM003_SPECIAL_MUTE_)
			    	if(global.aud_mute)
			    	{
			    		global.aud_mute = !global.aud_mute;
			    		func_mute_control();
			    	}
			    #endif
				#if defined(_ENABLE_A5900_USB_)
    				if((music_ctrl->ffr_state == PLAY_STATUS_FORWARD)
    					||(music_ctrl->ffr_state == PLAY_STATUS_BACKWARD))
    				{
    					sys_mute_ctrl(DISABLE, 0);
    					music_ctrl->ffr_state = 0;
    				}
    				else
				#endif
    				{
    
    		            _DECODE_STATUS pp_s;
    		            pp_s = pp_decode(&mapi->dop_api->dec_api);
    		            if(DECODER_PAUSE == pp_s)
    		            {//暂停
    		            	music_puts("*PAUSE*\n");
    					#if defined(PAUSE_FOR_MUTE_)
    						sys_mute_ctrl(ENABLE, 0);
    					#endif
    						play_info.pause_flag = TRUE;
    					#if USB_DISK_EN
    		                usb->dev_io->ioctrl(0,DEV_SET_REMAIN);
    					#endif
    		            }
    		            else
    		            {
    		                if(DECODER_PLAY == pp_s)
    		                {//播放
    		                	music_puts("*PLAY*\n");
    						#if defined(PAUSE_FOR_MUTE_)
    							sys_mute_ctrl(DISABLE, 0);
    						#endif
    							play_info.pause_flag = FALSE;
    							if(play_info.play_flag)
    							{
    								play_info.play_flag = FALSE;
    							}
    		                }
    		                else
    		                {
    		                	music_puts("----*STOP*----\n");
    							stop_flag = TRUE;
    		                    err = music_play_api(mapi, DEV_SEL_CUR, 0, PLAY_FIRST_FILE, &(mapi->dop_api->file_num));
    		                }
    		            }
    					music_disp_info(mapi);
    				}
				}
	            break;

			case MSG_MUSIC_STOP:
				stop_start = TRUE;
			#if defined(_YS_MUSIC_SPEC_FUNC_)
                mapi->dop_api->file_num = 1;
                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_FIRST_FILE,&(mapi->dop_api->file_num));
			#else
                music_puts("MSG_MUSIC_STOP\r\n");
				_DECODE_STATUS pp_s;
				pp_s = stop_decoder(&mapi->dop_api->dec_api);
				music_disp_info(mapi);
			#endif
                break;

			case MSG_MUSIC_FF://快进
	            music_puts("MSG_MUSIC_FF\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			
		    #if defined(_XM007_MUSIC_BEEP_)
		    	if(global.aud_mute)
		    	{
		    		sys_mute_ctrl(ENABLE, 1000);
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			
		    	global.ff_fb_flag = TRUE;
				sys_mute_ctrl(ENABLE, 0);
			#if defined(_FF_FB_SPECIAL_FUNCTION_)
			    if(!global.ff_name)
			    {
                    global.ff_name = TRUE;
				}
				else
				{
					global.ff_name = FALSE;
				}
			#endif
			#if defined(_FF_FB_TIME_DISP_)
			    if(0 == dec_ff_fr(&mapi->dop_api->dec_api, PLAY_MOD_FF, 1))
	            {
	                music_ctrl->ffr_state = PLAY_STATUS_FORWARD;
	            }
			#else
				if(0 == dec_ff_fr(&mapi->dop_api->dec_api, PLAY_MOD_FF, 4))
	            {
	                music_ctrl->ffr_state = PLAY_STATUS_FORWARD;
	            }
			#endif	
	            break;

	        case MSG_MUSIC_FR://快退
	            music_puts("MSG_MUSIC_FR\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
			
		    #if defined(_XM007_MUSIC_BEEP_)
		    	if(global.aud_mute)
		    	{
		    		sys_mute_ctrl(ENABLE, 1000);
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
						
		    	global.ff_fb_flag = TRUE;
				sys_mute_ctrl(ENABLE, 0);
			#if defined(_FF_FB_SPECIAL_FUNCTION_)
				if(!global.fb_name)
			    {
                    global.fb_name = TRUE;
				}
				else
				{
					global.fb_name = FALSE;
				}
			#endif
			#if defined(_FF_FB_TIME_DISP_)
			    if(0 == dec_ff_fr(&mapi->dop_api->dec_api, PLAY_MOD_FB, 1))
	            {
	                music_ctrl->ffr_state = PLAY_STATUS_BACKWARD;
	            }
			#else
				if(0 == dec_ff_fr(&mapi->dop_api->dec_api, PLAY_MOD_FB, 4))
	            {
	                music_ctrl->ffr_state = PLAY_STATUS_BACKWARD;
	            }
			#endif	
	            break;

	        case MSG_MUSIC_FFR_DONE://快进快退释放
				music_puts("MSG_MUSIC_FFR_DONE\n");
		    #if defined(_XM003_SPECIAL_MUTE_)||defined(_XM007_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
				sys_mute_ctrl(DISABLE, 500);
				music_ctrl->ffr_state = 0;
	            break;

		#if EQ_EN
			case MSG_MUSIC_EQ:
				if(mapi && mapi->dop_api)
				{
					eq_mode++;
					if(eq_mode > 5)
					{
						eq_mode = 0;
					}
					eq_mode_sw(eq_mode);
					music_printf("MSG_MUSIC_EQ %d\n ", eq_mode);
				}
				break;
		#endif

			case MSG_MUSIC_NEXT_FOLDER://文件夹加
				music_puts("MSG_MUSIC_NEXT_FOLDER\n");
				{
			    #if defined(_ENABLE_RCA_MUTE_)//修改放音RCA有杂音
			    	set_sub_mute();
					OSTimeDly(2);
			    #endif
			    #if defined(_WLS_SPECIAL_MUTE_)
			    	sys_mute_ctrl(ENABLE, 1300);
			    #else
			    	sys_mute_ctrl(ENABLE, 500);
			    #endif
			    #if defined(_XM003_SPECIAL_MUTE_)
			    	if(global.aud_mute)
			    	{
			    		global.aud_mute = !global.aud_mute;
			    		func_mute_control();
			    	}
			    #endif
				
			    #if defined(_RPT_DIR_FUNTION_)
			    	if(setup.rpt_start == MUSIC_RPT_FOLDER)
			    	{
			    		setup.rpt_start = MUSIC_RPT_ALL;
			    		setup.rpt_flag1 = TRUE;
			    	}
			    #endif
					play_info.pause_flag = FALSE;
					file_auto_next = 1;
					err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FOLDER,&(mapi->dop_api->file_num));
				#if defined(_NO_MEMORY_PLAY_MODE)
					music_ctrl->intro_flag = FALSE;
					play_mode=REPEAT_ALL;
				#endif
				}
				break;

		    case MSG_MUSIC_PREV_FOLDER://文件夹减
				music_puts("MSG_MUSIC_PREV_FOLDER\n");
				{
			    #if defined(_ENABLE_RCA_MUTE_)//修改放音RCA有杂音
			    	set_sub_mute();
					OSTimeDly(2);
			    #endif
			    #if defined(_WLS_SPECIAL_MUTE_)
			    	sys_mute_ctrl(ENABLE, 1300);
			    #else
			    	sys_mute_ctrl(ENABLE, 500);
			    #endif
			    #if defined(_XM003_SPECIAL_MUTE_)
			    	if(global.aud_mute)
			    	{
			    		global.aud_mute = !global.aud_mute;
			    		func_mute_control();
			    	}
			    #endif
			    #if defined(_RPT_DIR_FUNTION_)
			    	if(setup.rpt_start == MUSIC_RPT_FOLDER)
			    	{
			    		setup.rpt_start = MUSIC_RPT_ALL;
			    		setup.rpt_flag1 = TRUE;
			    	}
			    #endif
					play_info.pause_flag = FALSE;
					file_auto_next = 0;
					err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_PRE_FOLDER,&(mapi->dop_api->file_num));
				#if defined(_NO_MEMORY_PLAY_MODE)
					music_ctrl->intro_flag = FALSE;
					play_mode=REPEAT_ALL;
				#endif
				}
				break;

		    case MSG_MUSIC_ADD_10://加十
				music_puts("MSG_MUSIC_ADD_10\n");
				{
					uint8 i;
					uint16 next_tune = music_ctrl->play_info.tune;
					uint16 total_tune = music_ctrl->play_info.total;
			
				#if defined(_SL6812A_SPECIAL_DISPLAY_)
					global.play_trk10 = TRUE;
					global.trk10_up = 1;
				#endif
				
			    #if defined(_ENABLE_RCA_MUTE_)//修改放音RCA有杂音
			    	set_sub_mute();
					OSTimeDly(2);
			    #endif
			    #if defined(_WLS_SPECIAL_MUTE_)
			    	sys_mute_ctrl(ENABLE, 1300);
			    #else
			    	sys_mute_ctrl(ENABLE, 500);
			    #endif
			    #if defined(_XM003_SPECIAL_MUTE_)
			    	if(global.aud_mute)
			    	{
			    		global.aud_mute = !global.aud_mute;
			    		func_mute_control();
			    	}
			    #endif
					play_info.pause_flag = FALSE;
					for(i = 0; i < 10; i++)
					{
						if(next_tune >= total_tune)
						{
						#if defined(_SPECIAL_MUSIC_ADD10_)
							next_tune = total_tune;
						#else
							next_tune = 1;
						#endif
						}
						else
						{
							next_tune++;
						}
					}

					file_auto_next = 1;
					mapi->dop_api->file_num = next_tune;
               	 	err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
				#if defined(_NO_MEMORY_PLAY_MODE)
					music_ctrl->intro_flag = FALSE;
					play_mode=REPEAT_ALL;
				#endif
				}
				break;

		    case MSG_MUSIC_SUB_10://减十
				music_puts("MSG_MUSIC_SUB_10\n");
				{
					uint8 i;
					uint16 next_tune = music_ctrl->play_info.tune;
					uint16 total_tune = music_ctrl->play_info.total;
					
			    #if defined(_SL6812A_SPECIAL_DISPLAY_)
			    	global.play_trk10 = TRUE;
					global.trk10_up = 0;
			    #endif

                #if defined(_ENABLE_RCA_MUTE_)//修改放音RCA有杂音
                	set_sub_mute();
					OSTimeDly(2);
                #endif
			    #if defined(_WLS_SPECIAL_MUTE_)
			    	sys_mute_ctrl(ENABLE, 1300);
			    #else
			    	sys_mute_ctrl(ENABLE, 500);
			    #endif
			    #if defined(_XM003_SPECIAL_MUTE_)
			    	if(global.aud_mute)
			    	{
			    		global.aud_mute = !global.aud_mute;
			    		func_mute_control();
			    	}
			    #endif
					play_info.pause_flag = FALSE;
					for(i = 0; i < 10; i++)
					{
						if(next_tune <= 1)
						{
						#if defined(_SPECIAL_MUSIC_ADD10_)
							next_tune = 1;
						#else
							next_tune = total_tune;
						#endif
						}
						else
						{
							next_tune--;
						}
					}

					file_auto_next = 0;
					mapi->dop_api->file_num = next_tune;
               	 	err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
				#if defined(_NO_MEMORY_PLAY_MODE)
					music_ctrl->intro_flag = FALSE;
					play_mode=REPEAT_ALL;
				#endif
				}
				break;

			case MSG_MUSIC_ID3://ID3信息切换
				music_puts("MSG_MUSIC_ID3\n");
				disp_send_param(DISP_EVENT_PLAY_CMD, PLAY_CMD_ID3);
				break;

		    case MSG_MUSIC_SEARCH://编码器选曲
				music_puts("MSG_MUSIC_SEARCH\n");
				disp_send_param(DISP_EVENT_PLAY_CMD, PLAY_CMD_SEARCH_DELETE);
				break;
		#if defined(TD_ADD_NEW_APP)
			case MSG_MUSIC_RPT1:
				music_puts("MSG_MUSIC_RPT1\n");
			#if defined(_XM003_SPECIAL_MUTE_)
				if(global.aud_mute)
				{
					global.aud_mute = !global.aud_mute;
					func_mute_control();
				}
			#endif
				if(mapi && mapi->fop_api && mapi->fop_api->fop_init)
				{
					switch(play_mode)
					{
						case REPEAT_ALL:
							play_mode = REPEAT_ONE;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ONE);
							break;
						case REPEAT_ONE:
							play_mode = REPEAT_RANDOM;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_SHUFFLE_ON);
							break;
					#if defined(DISABLE_RPT_DIR)
    					case REPEAT_RANDOM:
    						play_mode = REPEAT_ALL;
    						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
    						break;
					#else
                		case REPEAT_RANDOM:
							play_mode = REPEAT_FOLDER;
                            disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_FOLDER);
                			break;
						case REPEAT_FOLDER:
							play_mode = REPEAT_ALL;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
							break;
                	#endif
						default:
							play_mode = REPEAT_ALL;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
							break;
					}
					music_ctrl->intro_flag = FALSE;
					mapi->fop_api->fop_init->cur_play_mode = play_mode;
					music_disp_info(mapi);
				}
				if(play_mode == REPEAT_RANDOM)
				{
    			#if defined(_WLS_SPECIAL_MUTE_)
			    	sys_mute_ctrl(ENABLE, 1300);
    			#else
			    	sys_mute_ctrl(ENABLE, 500);
    			#endif
					file_auto_next = 1;
					play_info.pause_flag = FALSE;
	            	err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FILE,&(mapi->dop_api->file_num));
				}
				break;
		#endif
		
	        case MSG_MUSIC_RPT://
				music_puts("MSG_MUSIC_RPT\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
	            if(mapi && mapi->fop_api && mapi->fop_api->fop_init)
	            {
	            #if defined(_KLD3000_RPT_FUNTION_)
    				switch(play_mode)
    				{
    					case REPEAT_ALL:
    						play_mode = REPEAT_FOLDER;
    						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_FOLDER);
    						break;
    					case REPEAT_FOLDER:
    						play_mode = REPEAT_ONE;
    						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ONE);
    						break;
    				
    					case REPEAT_ONE:
    						play_mode = REPEAT_ALL;
    						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
    						break;
    				
    					default:
    						play_mode = REPEAT_ALL;
    						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
    						break;
    				}
				#else
					switch(play_mode)
					{
						case REPEAT_ALL:
							play_mode = REPEAT_ONE;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ONE);
							break;
					#if defined(DISABLE_RPT_DIR)
    					case REPEAT_ONE:
    						play_mode = REPEAT_ALL;
    						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
    						break;
					#else
						case REPEAT_ONE:
							play_mode = REPEAT_FOLDER;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_FOLDER);
							break;

						case REPEAT_FOLDER:
							play_mode = REPEAT_ALL;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
							break;
					#endif

						default:
							play_mode = REPEAT_ALL;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
							break;
					}
				#endif
					music_ctrl->intro_flag = FALSE;
					mapi->fop_api->fop_init->cur_play_mode = play_mode;
					music_disp_info(mapi);
				}
	            break;

			case MSG_MUSIC_RDM:
				music_puts("MSG_MUSIC_RDM\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
				if(mapi && mapi->fop_api && mapi->fop_api->fop_init)
				{
			    #if defined(RDM_NEXT_FILE_RNDM)
			    	music_ctrl->intro_flag = FALSE;
			    #endif
					if(play_mode == REPEAT_RANDOM)
					{
						play_mode = REPEAT_ALL;
				    #if defined(RDM_NEXT_FILE_RNDM)
				    	mapi->fop_api->fop_init->cur_play_mode = play_mode;
				    	music_disp_info(mapi);
				    #endif
						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_SHUFFLE_OFF);
					}
					else
					{
						play_mode = REPEAT_RANDOM;
				    #if defined(RDM_NEXT_FILE_RNDM)
				    	mapi->fop_api->fop_init->cur_play_mode = play_mode;
				    	music_disp_info(mapi);
				    #endif
						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_SHUFFLE_ON);
				    #if defined(_SPECIAL_RDM_FUNTION_)
				    	err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FILE,&(mapi->dop_api->file_num));
				    #endif
					}
				#if !defined(RDM_NEXT_FILE_RNDM)
					music_ctrl->intro_flag = FALSE;
					mapi->fop_api->fop_init->cur_play_mode = play_mode;
					music_disp_info(mapi);
				#endif
				}

			#if defined(_YS_MUSIC_SPEC_FUNC_)
				if(play_mode == REPEAT_RANDOM)
				{
			    	#if defined(_WLS_SPECIAL_MUTE_)
			    	sys_mute_ctrl(ENABLE, 1300);
			    	#else
			    	sys_mute_ctrl(ENABLE, 500);
			    	#endif
					file_auto_next = 1;
					play_info.pause_flag = FALSE;
	            	err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FILE,&(mapi->dop_api->file_num));
				}
			#endif
				
				break;

		    case MSG_MUSIC_INT:
				music_puts("MSG_MUSIC_INT\n");
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
				if(mapi && mapi->fop_api && mapi->fop_api->fop_init)
				{
					music_ctrl->intro_flag = !music_ctrl->intro_flag;
					if(music_ctrl->intro_flag)
					{
						if(play_info.pause_flag)
						{
							play_info.pause_flag = FALSE;
							os_taskq_post_msg(MUSIC_TASK_NAME, 1, MSG_MUSIC_PLAY);
						}
						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_INTRO_ON);
						play_info.play_flag = FALSE;
					#if defined(_PLAYER_INT_ON_)	
						err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_FIRST_FILE,&(mapi->dop_api->file_num));
					#endif
					}
					else
					{
						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_INTRO_OFF);
					}
					music_ctrl->intro_timer = 0;
					play_mode = REPEAT_ALL;
					mapi->fop_api->fop_init->cur_play_mode = play_mode;
					music_disp_info(mapi);
				}
				break;

			case MSG_MUSIC_PLAY_MODE:
				music_puts("MSG_MUSIC_PLAY_MODE\n");
			#if defined(_ENABLE_NUMBER6_BEEP_)
				break;
			#endif
				if(mapi && mapi->fop_api && mapi->fop_api->fop_init)
				{
					if(music_ctrl->intro_flag)
					{
						music_ctrl->intro_flag = FALSE;
						play_mode = REPEAT_RANDOM;
						disp_send_msg(MSG_MUSIC_RDM);
						disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_SHUFFLE_ON);
					}
					else
					{
						switch(play_mode)
						{
							case REPEAT_ALL:
								play_mode = REPEAT_ONE;
								disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ONE);
								break;
							case REPEAT_ONE:
								music_ctrl->intro_flag = TRUE;
								disp_send_msg(MSG_MUSIC_INT);
								disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_INTRO_ON);
								break;
							case REPEAT_RANDOM:
								play_mode = REPEAT_ALL;
							#if defined(_WLS316_SPECIAL_DISPLAY_)
								disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_SHUFFLE_OFF);
							#else
								disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
							#endif
								break;
						#if defined(ENABLE_RPT_DIR)		
							case REPEAT_FOLDER:
							play_mode = REPEAT_ALL;
							disp_send_param2(DISP_EVENT_PLAY_CMD, PLAY_CMD_MODE, PLAY_MODE_DISP_REPEAT_ALL);
							break;
						#endif
						}
					}
					mapi->fop_api->fop_init->cur_play_mode = play_mode;
					music_disp_info(mapi);
				}
				break;

			case MSG_MUSIC_0 ... MSG_MUSIC_9:
			#if defined(SUPPORT_NUM_SELECT_MUSIC)
				get_id3_disp_status();
			#endif
		    #if defined(_XM003_SPECIAL_MUTE_)
		    	if(global.aud_mute)
		    	{
		    		global.aud_mute = !global.aud_mute;
		    		func_mute_control();
		    	}
		    #endif
				music_printf("MSG_MUSIC_NUMBER:%d\n", msg[0] - MSG_MUSIC_0);
				disp_send_param(DISP_EVENT_PLAY_CMD, msg[0] - MSG_MUSIC_0 + PLAY_CMD_0);
				break;

			case MSG_MUSIC_ADD://编码器选曲加 order_select:1  0:默认功能  1:复用功能
				music_puts("MSG_MUSIC_ADD\n");
				disp_send_param(DISP_EVENT_PLAY_CMD, PLAY_CMD_SEL_ADD);
				break;

			case MSG_MUSIC_SUB://编码器选曲减order_select:1  0:默认功能  1:复用功能
				music_puts("MSG_MUSIC_SUB\n");
				disp_send_param(DISP_EVENT_PLAY_CMD, PLAY_CMD_SEL_SUB);
				break;

			case MSG_MUSIC_DELETE:
				music_puts("MSG_MUSIC_DELETE\n");
				disp_send_param(DISP_EVENT_PLAY_CMD, PLAY_CMD_DELETE);
				break;

	    #if defined(_SUPPORT_DISC_)
	    	case MSG_MUSIC_SEL:
	    #endif
			case MSG_MUSIC_ENTER://选曲后按SEL按键确定order_select:1 0:默认功能  1:复用功能
				music_puts("MSG_MUSIC_ENTER\n");
				sys_mute_ctrl(ENABLE, 500);
				disp_send_param(DISP_EVENT_PLAY_CMD, PLAY_CMD_ENTER);
				break;

			case MSG_MUSIC_TRK://选曲后确定执行这里
				music_printf("MSG_MUSIC_TRK:%d\n", msg[1]);
				file_auto_next = 1;
				mapi->dop_api->file_num = msg[1];
                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
			#if defined(_NO_MEMORY_PLAY_MODE)
				music_ctrl->intro_flag = FALSE;
				play_mode=REPEAT_ALL;
			#endif
				break;

			case MSG_MUSIC_DISP://选曲OK后 执行这里取消第二功能
				{
					music_printf("MSG_MUSIC_DISP:%d\n", msg[1]);
					uint8 search = msg[1];
					if(search)
					{
						key_msg_select(KEY_SELECT_2NDF);
					#if defined(_ENABLE_MODE_PLAY_)
						global.ndf2_flg = TRUE;
					#endif
					}
					else
					{
						key_msg_select(KEY_SELECT_DEFAULT);
					#if defined(_ENABLE_MODE_PLAY_)
						global.ndf2_flg = FALSE;
					#endif
					}
				}
				break;

			case MSG_MUSIC_TIMER:
			#ifdef	TD_ADD_ALEN
				if(decode_ok ==0)
					break;
			#endif

				//修改在快进快退时，再按上下曲，显示不更新
            #if defined(_FF_FB_SPECIAL_FUNCTION_)
			    if(global.ff_name)
			    {
			       if(0 == dec_ff_fr(&mapi->dop_api->dec_api, PLAY_MOD_FF, 4))
	               {
	                   music_ctrl->ffr_state = PLAY_STATUS_FORWARD;
					   global.fb_name= FALSE;
	               }
			    }
				else if(global.fb_name)
				{
				  if(0 == dec_ff_fr(&mapi->dop_api->dec_api, PLAY_MOD_FB, 4))
	              {
	                   music_ctrl->ffr_state = PLAY_STATUS_BACKWARD;
					   global.ff_name= FALSE;
	              }
			    }
				else
				{
				   if(global.ff_fb_flag)
		    	   {
		    		os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FFR_DONE);
		    		global.ff_fb_flag = FALSE;
		    	   }
				}
            #elif defined(_ENABLE_A5900_USB_)
            	if(music_ctrl->ffr_state == PLAY_STATUS_FORWARD)
            	{
            		ew_puts("MSG_MUSIC_FF\r\n");
            		sys_mute_ctrl(ENABLE, 0);
            		dec_ff_fr(&mapi->dop_api->dec_api,PLAY_MOD_FF,4);//
            		music_ctrl->ffr_state = PLAY_STATUS_FORWARD;
            	}
            	else if(music_ctrl->ffr_state == PLAY_STATUS_BACKWARD)
            	{
            		ew_puts("MSG_MUSIC_FF\r\n");
            		sys_mute_ctrl(ENABLE, 0);
            		dec_ff_fr(&mapi->dop_api->dec_api,PLAY_MOD_FB,4);//
            		music_ctrl->ffr_state = PLAY_STATUS_BACKWARD;
            	}
			#else	
		    	if(global.ff_fb_flag)
		    	{
		    		os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_FFR_DONE);
		    		global.ff_fb_flag = FALSE;
		    	}
			#endif	
				
				music_disp_info(mapi);
    			if(music_ctrl->intro_flag)
    			{
    			    if(!play_info.play_flag)
    			    {
						if((music_ctrl->play_info.min > 0) || (music_ctrl->play_info.sec > 10))
						{
							music_puts("----INTRO_NEXT\n");
							music_ctrl->intro_timer = 0;
							file_auto_next = 1;
							err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FILE,&(mapi->dop_api->file_num));
						}
						else if(music_ctrl->play_info.sec < 11)
						{
							music_ctrl->intro_timer++;
						#if defined(_XIMA_MUSIC_INT_)
							if(music_ctrl->intro_timer > 21)
						#else
							if(music_ctrl->intro_timer > 20)
						#endif
							{
								music_ctrl->intro_timer = 0;
								file_auto_next = 1;
								music_puts("----MSG_MUSIC_TIMER\n");
								err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FILE,&(mapi->dop_api->file_num));
							}
						}
					}
    			}
				
		    #if defined(_ALARM_CLOCK_RING_)
		    	if(setup.alarm_beep)
		    	{
		    		if(!play_info.pause_flag)
		    		{
		    			os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PAUSE);
		    			setup.alarm_flag = TRUE;
		    		}
		    	}
		    	else
		    	{
		    		if(setup.alarm_flag)
		    		{
		    			os_taskq_post_msg("MusicTask", 1, MSG_MUSIC_PLAY);
		    			setup.alarm_flag = FALSE;
		    		}
		    	}
		    #endif
			
		    #if defined(_ENABLE_RPT_RDM_FUNTION_)
		    	if(setup.rpt_start != setup.rpt_start1)
		    	{
		    		setup.rpt_start1 = setup.rpt_start;
		    		if(mapi && mapi->fop_api && mapi->fop_api->fop_init)
		    		{
		    			switch(setup.rpt_start)
		    			{
		    				case MUSIC_RPT_ONE:
		    					play_mode = REPEAT_ONE;
		    					break;
		    				case MUSIC_RPT_FOLDER:
		    					play_mode = REPEAT_FOLDER;
		    					break;
		    				case MUSIC_RPT_ALL:
		    					play_mode = REPEAT_ALL;
		    					break;
		    				default:
		    					play_mode = REPEAT_ALL;
		    					break;
		    			}
		    			music_ctrl->intro_flag = FALSE;
		    			mapi->fop_api->fop_init->cur_play_mode = play_mode;
		    			music_disp_info(mapi);
		    		}
		    	}
		    	
		    	if(setup.rdm_start != setup.rdm_start1)
		    	{
		    		setup.rdm_start1 = setup.rdm_start;
		    		if(mapi && mapi->fop_api && mapi->fop_api->fop_init)
		    		{
		    			if(setup.rdm_start)
		    			{
		    				play_mode = REPEAT_RANDOM;
		    			}
		    			else
		    			{
		    				play_mode = REPEAT_ALL;
		    			}
		    			music_ctrl->intro_flag = FALSE;
		    			mapi->fop_api->fop_init->cur_play_mode = play_mode;
		    			music_disp_info(mapi);
		    		}
		    	}
		    #endif
						
                break;

	    #if (SPEED_PITCH_EN)
	    	case MSG_MUSIC_SET_SPEED:
	    		speed_mode ++;
	    		if(speed_mode >= (sizeof(speed_tab)/sizeof(speed_tab[0])))
	    		{
	    			speed_mode = 0;
	    		}
	    		printf("speed_mode = %d\n", speed_mode);
	    		music_decoder_set_speed(mapi, speed_tab[speed_mode]);
	    		break;
	    	case MSG_MUSIC_SET_PITCH:
	    		pitch_mode ++;
	    		if(pitch_mode >= (sizeof(pitch_tab)/sizeof(pitch_tab[0])))
	    		{
	    			pitch_mode = 0;
	    		}
	    		printf("pitch_mode = %d\n", pitch_mode);
	    		music_decoder_set_pitch(mapi, pitch_tab[pitch_mode]);
	    		break;
	    #endif//SPEED_PITCH_EN

	        case MSG_MUSIC_U_SD:
	            music_puts("MSG_MUSIC_U_SD\n");
	            if(file_operate_get_total_phydev()>1)
	            {
	                mapi->dop_api->dec_api.save_brk = 1;
	                err = music_play_api(mapi,DEV_SEL_NEXT,0,PLAY_BREAK_POINT,&(mapi->dop_api->file_num));
	            }
	            break;

	        case SYS_EVENT_DECODE_END:
	            break;

	        case MSG_MUSIC_PLAY_SN:
	            music_puts("MSG_MUSIC_PLAY_SN\n");
	            if ((msg[1] <= mapi->fop_api->cur_lgdev_info->total_file_num)&&(msg[1] != 0))
	            {
	                mapi->dop_api->file_num = msg[1];
	                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
	            }
	            break;

	        case MSG_MUSIC_DEL_FILE:
				music_puts("KEY_EVENT_NEXT\n");
				{
		            file_auto_next = 1;
		            music_stop_decoder(mapi);

		            _FIL_HDL *f_h = (_FIL_HDL *)(mapi->fop_api->cur_lgdev_info->lg_hdl->file_hdl);
		            FIL *pf = (FIL *)(f_h->hdl);
		            err = f_unlink(pf);
		            music_printf("del file %08x\n",err);

		            file_operate_ctl(FOP_GET_TOTALFILE_NUM,mapi->fop_api,mapi->dop_api->file_type,0);
		            if(0 == mapi->fop_api->cur_lgdev_info->total_file_num)
		            {
						 music_printf("11111MSG_MUSIC_DEL_FILE\n");
		                err = music_play_api(mapi,DEV_SEL_NEXT,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
		            }
		            else
		            {
						 music_printf("22222MSG_MUSIC_DEL_FILE\n");
		                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
		            }
				}
	            break;

	        case SYS_EVENT_DEV_ONLINE:
	            music_puts("music SYS_EVENT_DEV_ONLINE\r");
	            music_puts("AA\r");
	            mapi->dop_api->dec_api.save_brk = 1;
	            mapi->dop_api->file_num = 1;
	            err = music_play_api(mapi,DEV_SEL_SPEC,msg[1],PLAY_BREAK_POINT,&(mapi->dop_api->file_num));
	            dec_phy_printf("BB\r");
	            break;

	        case MSG_MUSIC_SPC_FILE:
				music_puts("music MSG_MUSIC_SPC_FILE\r");
	            file_auto_next = 1;
	            err = music_play_by_sclust(mapi,msg[1]);
	            break;

	        case SYS_EVENT_DEV_BEGIN_MOUNT:
	            music_puts("new dev mount,stop\n");
	            mapi->dop_api->dec_api.save_brk = 1;
	            music_stop_decoder(mapi);
	            break;

	        case SYS_EVENT_DEV_MOUNT_ERR:
				music_puts("dev mount err\n");
	            mapi->dop_api->dec_api.save_brk = 0;
	            err = music_play_api(mapi,DEV_SEL_NEXT,0,PLAY_BREAK_POINT,&(mapi->dop_api->file_num));
	            break;
				
	    #ifdef	TD_ADD_NEW_APP
	    	case SYS_EVENT_GET_FILE_LIST:
	    	#if !defined(_DISABLE_MUSIC_LIST_)
	    		recv_get_list_cmd(msg[1],msg[2],msg[3],msg[4],msg[5]);
	    	#endif
	    		break;
	    	case SYS_BLE_LINK_OK:
	    	case SYS_BLE_DIS_LINK:
	    	#if !defined(_DISABLE_MUSIC_LIST_)
	    		file_brose_close();
	    		Listbrowse_over = 0;
	    	#endif
	    		break;
	    #endif

	        default:
				task_msg_default(msg[0], msg[1]);
	            break;
        }

        u32 totalerrfile = 0;

        while(0 != err)
        {
            music_printf("--------music err:%02x\n",err);
	    #if defined(_ENABLE_USB_MODE_)
		#if defined(_SL6812A_USB_NO_FILE_)
			if((err == 17)||(err == 15))
		#elif defined(_LIANKE_USB_CHANGE_MODE_)
			if((err == 17)||(err == 07))
		#else
	    	if((err == 17)||(err == 15)||(err == 07))
		#endif
	    	{
	    		global.charge_flag = TRUE;
	    	}
	    #endif

        #if defined(_SD_NO_FILE_DISPLAY_ERR_)
			if(err == 07)
			{
				global.music_error = TRUE;
			}
        #endif
		
            u32 status;
            switch(err)
			{
	            case FILE_OP_ERR_INIT:              ///<文件选择器初始化错误
	            case FILE_OP_ERR_OP_HDL:            ///<文件选择器指针错误
	            case FILE_OP_ERR_LGDEV_NULL:        ///<没有设备
	            case FILE_OP_ERR_NO_FILE_ALLDEV:    ///<没有文件（所有设备）
	            case FILE_OP_ERR_LGDEV_MOUNT:
	            case FILE_OP_ERR_NO_FILE_ONEDEV:    ///<当前选择的设备没有文件
					wait_exit = 1;					///<进入等待退出状态
					wait_ok = 1;
				#ifdef	TD_ADD_ALEN
					decode_ok =0;
				#endif
					music_printf("music ap exit:%d\r", music_device);
			    #if defined(_ENABLE_TIME_DISP_)
			    	global.timer_count = 4000;
			    #endif
				#if defined(_SUPPORT_SD_)
					if((music_device == SD0_DEVICE) || (music_device == SD1_DEVICE))
					{
					#if !defined(ENBLE_SD_MEMORY)
						setup.sd_break =0;//解决SD时关机 拔SD 再插SD有记忆的问题
					#endif

					}
					else
				#endif
					if(music_device == USB_DEVICE)
					{
					#if !defined(ENBLE_USB_MEMORY)
					    setup.usb_break = FALSE;
					#endif
					}
					file_operate_ctl(FOP_CLOSE_LOGDEV,0,0,0);
					if(OS_NO_ERR == os_taskq_post(MAIN_TASK_NAME, 1, MSG_CHANGE_WORKMODE))
					{
						wait_ok = 0;
					}
	                err = 0;
	                break;

	            case FILE_OP_ERR_OPEN_BPFILE:
	                music_printf("err:%02x\r",err);
	                mapi->dop_api->file_num = 1;
	                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_FIRST_FILE,&(mapi->dop_api->file_num));
	                break;

	            case FILE_OP_ERR_NUM:               ///<选择文件的序号出错
	                mapi->dop_api->file_num = 1;
					 music_printf("----22222err:%02x\r",err);
	                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
	                break;

				case SUCC_MUSIC_START_DEC:
					music_puts("SUCC_MUSIC_START_DEC\r");
	                err = 0;
	                file_auto_next = 1;
				#if defined(SYS_MODE_CHANG_NOPP)
					global.music_pp_enable=1;//MUSIC  OK
				#endif
				
			    #if defined(_RPT_DIR_FUNTION_)
			    	if(setup.rpt_flag)
			    	{
			    		setup.first_dir = 0;
			    		setup.last_dir = 0;
			    		setup.rpt_flag1 = FALSE;
			    	}
			    #endif

				#ifdef	TD_ADD_ALEN
					decode_ok =1;
				#endif

	                break;

	            case FILE_OP_ERR_LGDEV_NO_FIND:     ///<没找到指定的逻辑设备
				#ifdef	TD_ADD_IPONE_CHARGE
                    if(music_device == USB_DEVICE)
                    {
                        usb_ERR =TRUE;
                    }
				#endif
	                mapi->dop_api->file_num = 1;
					music_puts("----FILE_OP_ERR_LGDEV_NO_FIND\r");
	                err = music_play_api(mapi,DEV_SEL_NEXT,0,PLAY_SPEC_FILE,&(mapi->dop_api->file_num));
					mapi->dop_api->dec_api.save_brk = 1;
					music_stop_decoder(mapi);
				    err=FILE_OP_ERR_LGDEV_NULL;
					break;

	            case FILE_OP_ERR_OPEN_FILE:         ///<打开文件失败
	            case ERR_MUSIC_START_DEC:
	                status = file_operate_ctl(FOP_DEV_STATUS,mapi->fop_api,0,0);
	                if(status == FILE_OP_ERR_OP_HDL)
	                {///<逻辑设备不再链表
	                    err = FILE_OP_ERR_OP_HDL;
	                    break;
	                }
	                else if(!status)
	                {///<逻辑设备掉线
	                    err = FILE_OP_ERR_LGDEV_NO_FIND;
	                    break;
	                }
	                totalerrfile++;
	                music_printf("---total err =%08x \n",totalerrfile);
	                music_printf("---total_file_num =%08x \n",mapi->fop_api->cur_lgdev_info->total_file_num);
	                if(totalerrfile >= mapi->fop_api->cur_lgdev_info->total_file_num)
	                {///<当前设备中音乐文件全部不可以解码，做好标识
	                    file_operate_ctl(FOP_ALLFILE_ERR_LGDEV,mapi->fop_api,0,0);
	                    err = FILE_OP_ERR_NO_FILE_ONEDEV;
	                    totalerrfile = 0;
	                    break;
	                }
	                if(file_auto_next)
	                {
	                    music_puts("err next\r");
	                    err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FILE,&(mapi->dop_api->file_num));
	                }
	                else
	                {
	                    music_puts("err priv\r");
	                    err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_PREV_FILE,&(mapi->dop_api->file_num));
	                }
	                if((err!=FILE_OP_ERR_OPEN_FILE)&&(err!=ERR_MUSIC_START_DEC))
	                {///<不再是顺序找，清空错误数字
	                    totalerrfile=0;
	                }
	                break;

	            case ERR_MUSIC_API_NULL:
	                err = 0;
	                break;

	            case FILE_OP_ERR_END_FILE:	///最后一首向后
	            	music_puts("--FILE_OP_ERR_END_FILE\r");
	                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_FIRST_FILE,&(mapi->dop_api->file_num));
	                break;

	            case FILE_OP_ERR_PRE_FILE:	//第一首向前
	                 music_puts("---FILE_OP_ERR_PRE_FILE\r");
	                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_LAST_FILE,&(mapi->dop_api->file_num));
	                break;

	            default:
	                music_printf("err default:%02x\r",err);
	                err = music_play_api(mapi,DEV_SEL_CUR,0,PLAY_NEXT_FILE,0);
	                music_puts("err my \n");
	                break;
			}
        }
	}
}
#if defined(USB_SD_DIS_TONE_PLAY)
u8 USB_SD_DIS_TONE_PLAY_FLAG=0;
#endif

static void music_task_init(void *priv)
{
	u32 err;
#if defined(SYS_MODE_CHANG_NOPP)
	global.music_pp_enable=0;
#endif

#if defined(_NO_MEMORY_PLAY_MODE)
	music_ctrl->intro_flag = FALSE;
	play_mode=REPEAT_ALL;
#endif
	if(!strcmp((char *)priv,MUSIC_USB_NAME))
	{
		music_device = USB_DEVICE;
	}
	else if(!strcmp((char *)priv,MUSIC_CARD_NAME))
	{
		music_device = SD1_DEVICE;
	}
	else
	{
		music_device = NO_DEVICE;
	}

#if defined(SD_FM_I2C_PIN_COMMON_USE)
	extern void release_sd1_controller(void);
		release_sd1_controller();
#endif

 #if defined(USB_SD_MODE_WORK_ONOFF)
	if((music_device == SD0_DEVICE)||(music_device == SD1_DEVICE))
	{
		disp_send_param(DISP_EVENT_ICON_SD, ON);
	}
	else if(music_device == USB_DEVICE)
	{
		disp_send_param(DISP_EVENT_ICON_USB, ON);
	}
 #endif

#if defined (_ENABLE_USB_SAVE_)||defined(_ENABLE_USB_DELAY_)
 	#if defined(_SUPPORT_SD_)
		if((music_device == SD0_DEVICE)||(music_device == SD1_DEVICE))
		{
			disp_send_msg(DISP_EVENT_SD);
		}
		else
	#endif
		if(music_device == USB_DEVICE)
		{
			disp_send_msg(DISP_EVENT_USB);
		}
 #endif
	 music_puts("------------111111MSG_MUSIC_SUB\n");

 #if defined(_ENABLE_TONE_PLAY_)
 	#if defined(USB_SD_DIS_TONE_PLAY)
    if(USB_SD_DIS_TONE_PLAY_FLAG)//usb sd切换时不需要报语音
    {
		sys_mute_ctrl(DISABLE, 0);
		music_puts("------------2222MSG_MUSIC_SUB\n");
	}
	else
	#endif
	{
	#if defined(USB_SD_DIS_TONE_PLAY)
		USB_SD_DIS_TONE_PLAY_FLAG=1;
	#endif
		//sys_mute_ctrl(DISABLE, 300);
	 	//dac_mute(0,1);//解码有声
		music_puts("------------3333MSG_MUSIC_SUB\n");
	 	sys_mute_ctrl(DISABLE, 0);
		AUDIO_MUTE_OFF();//功放有声
		tone_play_by_name(MUSIC_TASK_NAME, 1, BPF_MUSIC_MP3);
	}
 #endif
     
#if defined(SD_FM_I2C_PIN_COMMON_USE)||defined(_AUX_TONE_PLAY_)
 	if(setup.acc_off_flag)
 	{
		setup.acc_off_flag = FALSE;
		os_time_dly(200);	 //延时1s，防止开关acc优盘播歌无记忆
	}
#endif
#ifdef	TD_ADD_NEW_APP
#if !defined(_DISABLE_MUSIC_LIST_)
     BtAppComInit();
#endif
#endif
	//os_time_dly(50);
 	err = os_task_create_ext(music_task, 0, TaskMusicPrio, 50, MUSIC_TASK_NAME, MUSIC_TASK_STACK_SIZE);
    if(OS_NO_ERR == err)
    {
        key_msg_register(MUSIC_TASK_NAME, KEY_PROJECT_PLAYER);
    }
}

static void music_task_exit(void)
{
	music_puts("music_task_exit\n");
	key_msg_register(NULL, KEY_PROJECT_MAX);

#if defined(ACC_OFF_REMEBER_PLAY_STATE)
    if(setup.memory_play)
    {
        if(play_mode == REPEAT_RANDOM)
    	{
    		setup.play_flag = REPEAT_RANDOM;
			setup.rdm_flag = TRUE;
    	}
    	else if(play_mode == REPEAT_ONE)
    	{
    		setup.play_flag = REPEAT_ONE;
			setup.rpt_flag = TRUE;
    	}
    	else
    	{
    	    setup.play_flag = REPEAT_ALL;
    	}
	}
#endif

	if(os_task_del_req(MUSIC_TASK_NAME) != OS_TASK_NOT_EXIST)
	{
		music_puts("music_task_exit111\n");
		os_taskq_post_event(MUSIC_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
		music_puts("music_task_exit222\n");
		do{
			OSTimeDly(1);
		} while(os_task_del_req(MUSIC_TASK_NAME) != OS_TASK_NOT_EXIST);
		music_puts("del_music_task22: succ\n");
	}

	music_device = NO_DEVICE;

#if defined(_USER_TFT_SEND_)
	TFT_Tx_Music_Clock_songe_Info(music_device,0,0,0,0);
#endif
}


TASK_REGISTER(music_usb_task_info) = {
	.name = MUSIC_USB_NAME,
	.init = music_task_init,
	.exit = music_task_exit,
};

TASK_REGISTER(music_card_task_info) = {
	.name = MUSIC_CARD_NAME,
	.init = music_task_init,
	.exit = music_task_exit,
};
#ifdef	TD_ADD_NEW_APP
#if !defined(_DISABLE_MUSIC_LIST_)
extern u8 sppbuf[30];

void puts_lfn(u8 *lfn,u32 lfn_cnt)
{
    u32 i;
    for(i = 0; i < lfn_cnt/2; i++){
        extern u16 ConvertUTF16toGB2312(u16 utf);
        extern u16 ConvertUTF16toGBK(u16 utf);
        extern u8 unicode_to_utf8(u32 unic, u8 *pOutput, u8 outSize);
        u16 tmp = (u16)lfn[i*2+1] << 8 | lfn[i*2];
        u8 utf[6];
        memset(utf, 0, 6);
        u16 len = unicode_to_utf8((u32)tmp, utf, 6);
        /* put_buf(utf,6); */
        /* printf("0x%x,0x%x...",tmp, tmp2); */
        for(u8 i = 0; i < len; i++){
            putchar((u8)utf[i]);
            /* putchar((u8)tmp2>>8); */
        }
        /* puts("..."); */
    }
    puts("\n");
}

void ls_puts(u32 num)
{
    u32 i;
    for(i = 0; i < num; i++)
    {
        printf("sclust:%d,",ls_info[i].sclust);
        if(ls_info[i].dir_type)
        {
            printf("file,");
        }
        else
		{
            printf("dir,");
        }
        if(ls_info[i].fn_type)
        {
            printf("lfn,");
            //printf("len:%d,",ls_info[i].lfn_buf.lfn_cnt);
            puts_lfn((u8 *)ls_info[i].lfn_buf.lfn,ls_info[i].lfn_buf.lfn_cnt);
        }
        else
        {
            printf("sfn,");
            printf("len:%d,%s\n",ls_info[i].lfn_buf.lfn_cnt,ls_info[i].lfn_buf.lfn);
        }
    }
}

void ls_dir_close(void)
{
    dir_total = 0;
    dir_cur = 0;
    if(ls_info)
    {
        free(ls_info);
        ls_info = NULL;
    }
}

u32 ls_dir_init(void)
{
    dir_total = 0;
    dir_cur = 0;
    if(ls_info == NULL)
    {
        printf("FS_DIR_INFO len:%d\n",sizeof(FS_DIR_INFO));
        ls_info = malloc(sizeof(FS_DIR_INFO) * MAX_DIR_NUM);
        if(NULL == ls_info)
        {
            printf("FS_DIR_INFO is NULL");
            return 1;
        }
        memset(ls_info, 0x00, sizeof(FS_DIR_INFO)*MAX_DIR_NUM);
    }
    return 0;
}
u32 ls_open(void)
{
    u32 err;
    u32 phy_dev_chg;
    printf("ls_open\n");
	if(m_mapi != NULL)
	{
		d_mapi = m_mapi;
	}
	else
	{
	    d_mapi = malloc_fun(d_mapi, sizeof(MUSIC_OP_API), 0);
	    if(NULL == d_mapi)
	    {
	        printf("MUSIC_OP_API is NULL\n");
	        return 1;
	    }
	    d_mapi->fop_api = malloc_fun(d_mapi->fop_api,sizeof(FILE_OPERATE),0);
	    if (NULL == d_mapi->fop_api) 
	    {
	        printf("FILE_OPERATE is NULL\n");
	        return 1;
	    }
	    d_mapi->fop_api->fop_init = malloc_fun(d_mapi->fop_api->fop_init,sizeof(FILE_OPERATE_INIT),0);
	    if(NULL == d_mapi->fop_api->fop_init) 
	    {
	        printf("FILE_OPERATE_INIT is NULL\n");
	        return 1;
	    }
	    d_mapi->fop_api->fop_init->cur_dev_mode = DEV_SEL_SPEC;
	    d_mapi->fop_api->fop_init->dev_let = new_lg_dev_let;
	    /* new_lg_dev_let is 最新挂载设备 */
	    err = file_operate_ini(d_mapi->fop_api, &phy_dev_chg);
	    if (FILE_OP_NO_ERR != err) 
	    {
	        printf("file_operate_init ERR\n");
	        return 1;
	    }
	    err = lg_dev_mount(d_mapi->fop_api->cur_lgdev_info->lg_hdl->phydev_item,
	                       d_mapi->fop_api->cur_lgdev_info->dev_let, 1);
	    if (FILE_OP_NO_ERR != err) {
	        printf("lg_dev_mount ERR\n");
	        return 1;
	    }
	}
    if(ls_dir_init())
    {
        return 1;
    }
    printf("ls_open end\n");
    return 0;
}
void ls_close(void)
{
    printf("ls_close\r\n");
    if(d_mapi == NULL)
    {
        return;
    }
	if(m_mapi != NULL)
	{
		m_mapi = NULL;
		d_mapi = NULL;
	}
	else
	{
	    fs_close(d_mapi->fop_api->cur_lgdev_info->lg_hdl->file_hdl);
	    lg_dev_close(d_mapi->fop_api->cur_lgdev_info);
	    if(d_mapi->fop_api->fop_init)
	    {
	        free_fun((void**)&d_mapi->fop_api->fop_init);
	    }
	    if(d_mapi->fop_api)
	    {
	        free_fun((void**)&d_mapi->fop_api);
	    }
	    if(d_mapi)
	    {
	        free_fun((void**)&d_mapi);
	    }
	}
    ls_dir_close();
    printf("ls_close end\n");
}
u32 cd_root(void)
{
    u32 dir_num;
    printf("cd _root\n");
    if(d_mapi == NULL)
    {
        if(ls_open())
        {
            ls_close();
            return 0;/* 返回0 表示 错误或者无文件 */
        }
    }
    dir_num = file_bs_open_handle(d_mapi, filter_ext_name);
    return dir_num;
}
void put_sclust(void)
{
    _FIL_HDL *f_h = (_FIL_HDL *)(d_mapi->fop_api->cur_lgdev_info->lg_hdl->ui_fhdl);
    FIL *pf = (FIL *)(f_h->hdl);
    printf("deepth:%d\n",pf->fs_msg.deepth);
    printf("start_sclust:%d\n",pf->f_dj[pf->fs_msg.deepth].sclust);
}
u32 cd(FS_DIR_INFO *dir_info)
{
	u32 get_dir = 0;
	if(dir_info != NULL)
	{
		printf("cd sclust:%d\n",dir_info->sclust);
	}
	else
	{
		printf("cd NULL\n");
	}
    if(file_operate_get_total_phydev() == 0)
    {
        if(d_mapi != NULL)
        {
            file_bs_close_handle(d_mapi);
            ls_close();
        }
        return EXIT_BS;
    }
    if(d_mapi == NULL)
    {
        dir_info = NULL;
        dir_total = cd_root();
        printf("root total:%d\n",dir_total);
        if(dir_total == 0)
        {
            file_bs_close_handle(d_mapi);
            ls_close();
            dir_cur = 0;
            return EXIT_BS;
        }
        dir_cur = 1;
    }
    if((dir_info == NULL)&&(dir_total != 0))
    {
        printf("ls dir_cur:%d\n",dir_cur);
        memset(ls_info, 0x00, sizeof(FS_DIR_INFO)*MAX_DIR_NUM);
        get_dir = file_bs_get_dir_info(d_mapi,ls_info,dir_cur,MAX_DIR_NUM);
        dir_cur += get_dir;
        printf("ls dir_cur:%d\n",dir_cur);
        ls_puts(get_dir);
    }
    else if(dir_info != NULL)
    {
        dir_cur = 1;
        dir_total = file_bs_entern_dir(d_mapi,dir_info);
        printf("dir total:%d\n",dir_total);
        printf("ls dir_cur:%d\n",dir_cur);
        memset(ls_info, 0x00, sizeof(FS_DIR_INFO)*MAX_DIR_NUM);
        get_dir = file_bs_get_dir_info(d_mapi,ls_info,dir_cur,MAX_DIR_NUM);
        dir_cur += get_dir;
        printf("ls dir_cur:%d\n",dir_cur);
        ls_puts(get_dir);
    }
    put_sclust();
    puts("cd end\n");
    return get_dir;
}
u32 get_cur_deepth(void)
{
	_FIL_HDL *f_h = (_FIL_HDL *)(d_mapi->fop_api->cur_lgdev_info->lg_hdl->ui_fhdl);
	FIL *pf = (FIL *)(f_h->hdl);
	return pf->fs_msg.deepth;
}
u32 get_cur_sclust(void)
{
	_FIL_HDL *f_h = (_FIL_HDL *)(d_mapi->fop_api->cur_lgdev_info->lg_hdl->ui_fhdl);
	FIL *pf = (FIL *)(f_h->hdl);
	return pf->f_dj[pf->fs_msg.deepth].sclust;
}
u32 select_dir(u8 flag)
{
    printf("select_dir:%d\n",flag);
//    int i = 0;
    u32 get_dir = 0;
    u32 dir_pos = 0;
    u32 cur_deepth = 0;
    u32 cur_clust = 0;
    cur_deepth = get_cur_deepth();
    cur_clust = get_cur_sclust();
    printf("cur_deepth:%d\n",cur_deepth);
    printf("cur_clust:%d\n",cur_clust);
    if(flag)
    {
        /* 先回退上一级目录 */
        if(cur_deepth == 0){
            return EXIT_BS;
        }
        last_dir_clust = cur_clust;
        printf("last_dir_clust:%d\n",last_dir_clust);
        dir_total = file_bs_exit_dir(d_mapi);
    }
    while(dir_pos <= dir_total)
    {
        memset(ls_info, 0x00, sizeof(FS_DIR_INFO)*MAX_DIR_NUM);
        get_dir = file_bs_get_dir_info(d_mapi,ls_info,dir_pos,MAX_DIR_NUM);
        dir_pos += get_dir;
        for(int i = 0; i < get_dir; i++)
        {
            printf("ls_info[i].sclust:%d,last_dir_clust:%d\n",ls_info[i].sclust,last_dir_clust);
            if(flag == 0)
            {
                if(ls_info[i].dir_type == 0)
                {
                    return cd(ls_info+i);
                }
            }
            else
            {
                if(ls_info[i].sclust == last_dir_clust)
                {
                    if((i+1<get_dir)&&(ls_info[i+1].dir_type == 0))
                    {
                        return cd(ls_info+i+1);
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    return 0;
}

u32 ls_10_m(MUSIC_OP_API *mapi)
{
    u32 get_dir = 0;
	if(mapi != NULL)
	{
		m_mapi = mapi;
	}
    if(d_mapi == NULL)
    {
        printf("ls_10 A\n");
        return cd(NULL);
    }
    if(dir_cur <= dir_total)
	{/* 继续获取本目录的文件信息 */
        printf("ls_10 B\n");
        return cd(NULL);
	}
	else
	{/* 进入下一级目录 */
        printf("ls_10 C\n");
        get_dir = select_dir(0);
        if(get_dir == 0)
        {/* 下一级目录没有文件 */
            cur_dir_clust = get_cur_sclust();
            last_dir_clust = cur_dir_clust;
            while(!get_dir)
            {
                get_dir = select_dir(1);
            }
            /* return get_dir; */
        }
        else
        {
            printf("ls_10 D\n");
            /* return get_dir; */
        }
    }
    if(get_dir == EXIT_BS)
    {/* 返回-1，表示结束，已关闭文件浏览器 */
        printf("ls_10 E\n");
        file_bs_close_handle(d_mapi);
        ls_close();
		Listbrowse_over =1;
    }
	printf("ls_10 end\n");
    return get_dir;
}

u8 unicode_to_utf8(u32 unic, u8 *pOutput, u8 outSize)
{
    if( unic <= 0x0000007F )
    {
        // * U-00000000 - U-0000007F:  0xxxxxxx
        *pOutput     = (unic & 0x7F);
        return 1;
    }
    else if( unic >= 0x00000080 && unic <= 0x000007FF )
    {
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        *(pOutput+1) = (unic & 0x3F) | 0x80;
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if( unic >= 0x00000800 && unic <= 0x0000FFFF )
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *(pOutput+2) = (unic & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if( unic >= 0x00010000 && unic <= 0x001FFFFF )
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+3) = (unic & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if( unic >= 0x00200000 && unic <= 0x03FFFFFF )
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+4) = (unic & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+5) = (unic & 0x3F) | 0x80;
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;
        return 6;
    }
    return 0;
}

void file_brose_close(void)
{
    if(d_mapi!=NULL)
    {
        file_bs_close_handle(d_mapi);
        ls_close();
    }
    dir_total=0;
    dir_cur = 0;
    cur_dir_clust = 0;
    last_dir_clust = 0;
}

void puts_lfn_test(u8 *lfn,u32 lfn_cnt)
{
#if 1
    u32 i;
    for(i = 0; i < lfn_cnt/2; i++)
    {
        extern u16 ConvertUTF16toGB2312(u16 utf);
        extern u16 ConvertUTF16toGBK(u16 utf);
        extern u8 unicode_to_utf8(u32 unic, u8 *pOutput, u8 outSize);
        u16 tmp = (u16)lfn[i*2+1] << 8 | lfn[i*2];
        u8 utf[6];
        memset(utf, 0, 6);
        u16 len = unicode_to_utf8((u32)tmp, utf, 6);
        /* put_buf(utf,6); */
        /* printf("0x%x,0x%x...",tmp, tmp2); */
        for(u8 i = 0; i < len; i++){
            putchar((u8)utf[i]);
            /* putchar((u8)tmp2>>8); */
        }
        /* puts("..."); */
    }
    puts("\n");
#endif
}

// 大数据发送BUF
u8 BtApiBigFramePush(u8 *data,u16 len)
{
    printf("BtApiBigFramePush\n");
    if(len>BIG_FRAME_BUF) 
    {
    	return FALSE;
	}
    printf("BtApiBigFramePush ok\n");
    BtComFrame.bigbuf_flag = TRUE;
    BtComFrame.bigbuf_len = len;
    memcpy(BtComFrame.bigbuf,data,len);
    return TRUE;
}
// 上传文件列表
void PackFile(void)
{
    u8 i=0;
    u8 j=0;
    char temp;
    char temp2;
    char tempname[11];

	ListBuf[0]= 0xFF;
	ListBuf[1]= 0xAA;
	ListBuf[3]= 'Z';
	ListBuf[4]='A';

	ListBuf[5] = 0x6;
	ListBuf[6] = 0x1;
    ListBuf[7] = _u32to8hh_(ls_info[MusicList.send_num].sclust);
    ListBuf[8] = _u32to8hl_(ls_info[MusicList.send_num].sclust);
    ListBuf[9] = _u32to8lh_(ls_info[MusicList.send_num].sclust);
    ListBuf[10] = _u32to8ll_(ls_info[MusicList.send_num].sclust);
	
    ListBuf[11] = _u16to8h_(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);
    ListBuf[12] = _u16to8l_(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);

	// 过滤 FF FF数据
    temp2 = 0xff;
	if(ls_info[MusicList.send_num].fn_type==0)
	{
		ls_info[MusicList.send_num].lfn_buf.lfn_cnt = 11;
		memcpy(tempname,ls_info[MusicList.send_num].lfn_buf.lfn,11);
		printf("%s\n",(u8 *)tempname);
		for(i=0;i<11;i++)
		{
			if(tempname[i]==32)
			{
				;
			}
			else  
			{
				ls_info[MusicList.send_num].lfn_buf.lfn[j++]=tempname[i];
			}
		}
        if((ls_info[MusicList.send_num].lfn_buf.lfn[j-1]==65)
        &&(ls_info[MusicList.send_num].lfn_buf.lfn[j-2]==77)
        &&(ls_info[MusicList.send_num].lfn_buf.lfn[j-3]==87))
        {
			ls_info[MusicList.send_num].lfn_buf.lfn[j]=65;
			ls_info[MusicList.send_num].lfn_buf.lfn[j-1]=77;
			ls_info[MusicList.send_num].lfn_buf.lfn[j-2]=87;
        }
        else
        {
			ls_info[MusicList.send_num].lfn_buf.lfn[j]='3';
			ls_info[MusicList.send_num].lfn_buf.lfn[j-1]='p';
			ls_info[MusicList.send_num].lfn_buf.lfn[j-2]='m';
		}
        ls_info[MusicList.send_num].lfn_buf.lfn[j-3]='.';
        ls_info[MusicList.send_num].lfn_buf.lfn_cnt = j+1;
	    for(i=0;i<ls_info[MusicList.send_num].lfn_buf.lfn_cnt;i++)
	    {
	        ListBuf[14+i] = ls_info[MusicList.send_num].lfn_buf.lfn[i];
	    }
	    ListBuf[11] = _u16to8h_(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);
	    ListBuf[12] = _u16to8l_(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);
		ListBuf[13] =0x1;
	}
	else
	{
		for(i=0;i<(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);i++)
	    {
	    	if(ls_info[MusicList.send_num].lfn_buf.lfn[i]=='.')
	    	{
	    		if(ls_info[MusicList.send_num].lfn_buf.lfn[i+2]=='m')
	    		{
	    			if(ls_info[MusicList.send_num].lfn_buf.lfn[i+4]=='p')
	    			{
	    				ls_info[MusicList.send_num].lfn_buf.lfn_cnt = i+8;
	    				break;
	    			}
	    		}
	    		else if(ls_info[MusicList.send_num].lfn_buf.lfn[i+2]=='w')
	    		{
	    			if(ls_info[MusicList.send_num].lfn_buf.lfn[i+4]=='m')
	    			{
	    				ls_info[MusicList.send_num].lfn_buf.lfn_cnt = i+8;
	    				break;
	    			}
	    			else if(ls_info[MusicList.send_num].lfn_buf.lfn[i+4]=='a')
	    			{
	    				ls_info[MusicList.send_num].lfn_buf.lfn_cnt = i+8;
	    				break;
	    			}
	    		}
	    		else if(ls_info[MusicList.send_num].lfn_buf.lfn[i+2]=='f')
	    		{
	    			if(ls_info[MusicList.send_num].lfn_buf.lfn[i+4]=='l')
	    			{
	    				ls_info[MusicList.send_num].lfn_buf.lfn_cnt = i+8;
	    				break;
	    			}
	    		}
	    	}
	    }
	    for(i=0;i<(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);)
	    {
	        temp = ls_info[MusicList.send_num].lfn_buf.lfn[i];
	        ls_info[MusicList.send_num].lfn_buf.lfn[i] = ls_info[MusicList.send_num].lfn_buf.lfn[i+1];
	        ls_info[MusicList.send_num].lfn_buf.lfn[i+1] = temp;
			i=i+2;
	    }
	    for(i=0;i<(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);i++)
	    {
	        ListBuf[14+i] = ls_info[MusicList.send_num].lfn_buf.lfn[i];
            if(((ls_info[MusicList.send_num].lfn_buf.lfn[i] == 51)
            &&(ls_info[MusicList.send_num].lfn_buf.lfn[i-2] == 80)
            &&(ls_info[MusicList.send_num].lfn_buf.lfn[i-4] == 77))
			||((ls_info[MusicList.send_num].lfn_buf.lfn[i] == 101)
			&&(ls_info[MusicList.send_num].lfn_buf.lfn[i-2] == 112)
			&&(ls_info[MusicList.send_num].lfn_buf.lfn[i-4] == 97)))
            {
				ls_info[MusicList.send_num].lfn_buf.lfn_cnt=i+1;
            }
	    }
	    ListBuf[11] = _u16to8h_(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);
	    ListBuf[12] = _u16to8l_(ls_info[MusicList.send_num].lfn_buf.lfn_cnt);
		ListBuf[13] =0x0;
	}
    ListBuf[14+i] = crc_checksum((u8 *)ListBuf,14+i);
    ListBuf[2]= 12+i;
	BtApiBigFramePush(ListBuf,ListBuf[2]+2);
}

u8 BtApiFramPush(u8 *data,u16 len)
{
	if(BtComFrame.Length < 4)
	{
	    BtComFrame.framelen[BtComFrame.WritePoint]=len;
        memcpy(BtComFrame.combuf[BtComFrame.WritePoint],data,len);
        #if 0
        for(i=0;i<len;i++)
        {
            btapi_printf("%d ",BtComFrame.combuf[BtComFrame.WritePoint][i]);
        }
        btapi_printf("\n");
        #endif
        BtComFrame.WritePoint++;

		if(BtComFrame.WritePoint >= 4)
			BtComFrame.WritePoint = 0;
		BtComFrame.Length++;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

void FileListUp_end(void)
{
    u8 pbuf[8];
	pbuf[0]= 0xFF;
	pbuf[1]= 0xAA;
	pbuf[2]= 0x5;
	pbuf[3]= 'Z';
	pbuf[4]='A';
	pbuf[5] = 0x6;
	pbuf[6] = 0x0;
    pbuf[7] = crc_checksum((u8 *)pbuf,7);
	BtApiBigFramePush(pbuf,8);
}

void MusicListPackSend(void)
{
    if(!MusicList.up_flag) 
    {
    	return;
	}
    if(MusicList.up_timer)
    {
        MusicList.up_timer--;
        return;
    }
    switch(MusicList.up_state)
    {
        case up_start:  // 开始上传  获取文件数量
            if(Listbrowse_over) 
            {
            	MusicList.file_num=0xFFFFFFFF;
            }
	        else 
	        {
	        	MusicList.file_num=ls_10_m(music_operate_mapi);	// 返回值有时候会经常是1
            }
            printf("MusicListfile_num: %d\n",MusicList.file_num);

            if((MusicList.file_num>0)&&(MusicList.file_num<=10))
            {
                MusicList.up_state = up_check;
            }
            else if(MusicList.file_num==0xFFFFFFFF)
            {
                MusicList.up_state = up_over;
		 		Listbrowse_over = 1;
            }
            else
            {
                MusicList.file_num = 0;
            }

	    	if(MusicList.file_num==1)
      	    {
      	    	if(ls_info[0].dir_type==0)
      	    	{
		 			MusicList.up_state = up_over;
		 		}
      	    }
            MusicList.send_num = 0;
            break;
        case up_check:
            if(ls_info==NULL)
            {
                MusicList.up_state =  up_over;
                break;
            }
            printf("MusicListsend_num: %d\n",MusicList.send_num);
            if(MusicList.send_num<MusicList.file_num)
            {
                if(ls_info[MusicList.send_num].dir_type)
                {
                   MusicList.up_state =  up_data;
                }
                else
                {
                    MusicList.send_num++;
                }
            }
            else
            {
               MusicList.up_state =  up_wait;
            }
            break;
        case up_data:
            if(ListBuf == NULL)
            {
                ListBuf = (u8 *)malloc(250);
                memset(ListBuf, 0x00, 250);
                printf("malloc lisfBuf\r\n");
            }
            if(ls_info[MusicList.send_num].lfn_buf.lfn_cnt>245)
            {
            	;    // 长度太长 过滤掉
            }
            else
            {
				PackFile();
			}
            MusicList.up_timer = 4;
            MusicList.up_state =  up_wait;
            MusicList.send_num++;
            MusicList.send_cnt++;
            break;
        case up_wait:
            if(MusicList.send_cnt<APP_SEND_SONG_NUM)
            {
                if(MusicList.send_num<MusicList.file_num) 
                {
                	MusicList.up_state =  up_check;
                }
                else 
				{
                	MusicList.up_state =  up_start;
				}
            }
            else
            {
                if(MusicList.send_num<MusicList.file_num) 
                {
                	MusicList.up_state =  up_check;
                }
                else 
                {
                	MusicList.up_state =  up_over;
                }
            }
            break;
        case up_over:
            MusicList.up_flag = FALSE;
            if(ListBuf)
            {
                free(ListBuf);
                ListBuf = NULL;
                printf("free lisfBuf\r\n");
			}
            MusicList.send_cnt = 0;
            FileListUp_end();
            printf("MusicListPackSend up_over\n");
            break;
		default:
			break;
    }
}
void BtAppComInit(void)
{
    memset(&BtComFrame,0,sizeof(BtComFrame));
    memset(&MusicList,0,sizeof(MusicList));
}
void recv_get_list_cmd(u8 data1,u8 data2,u8 data3,u8 data4,u8 data5)
{//接收到获取文件列表命令
    u32 song=0;
	u32 song_num=0;
	
    printf("recv_get_list_cmd:0x%x-0x%x-0x%x-0x%x-0x%x\r\n",data1,data2,data3,data4,data5);
    if(read_list_ready_flag == FALSE)
    {
		printf("get_list_music_is_not_ready\r\n");
		return;
    }
    switch(data1)
    {
		case 0x0://获取列表
            if(data2==0x0)
            {//停止获取
                MusicList.up_flag = FALSE;
                if(ListBuf)
                {
                    free(ListBuf);
                    ListBuf = NULL;
                    printf("free lisfBuf\r\n");
                }
                MusicList.send_cnt = 0;
            }
            else if(data2==0x1)
            {//继续获取
                if(compare_task_name(MUSIC_USB_NAME))
                {//usb mode
                    if(!MusicList.up_flag)
                    {
                        MusicList.up_flag = 1;
                        MusicList.up_state =  up_start;
                    }
                }
                else if(compare_task_name(MUSIC_CARD_NAME))
                {//sd mode
                    if(!MusicList.up_flag)
                    {
                        MusicList.up_flag = 1;
                        MusicList.up_state =  up_start;
                    }
                }
            }
			break;
		case 0x1://选曲
            song = _u8tou32_(data2,data3,data4,data5);
			song_num = (data5<<24)+(data4<<16)+(data3<<8)+data2;
			printf("song:0x%08x\n",song);
			printf("song_num:0x%08x\n",song_num);
			printf("play_file_clust:0x%08x\n",play_file_clust);
			if(song_num != play_file_clust)
			{
				os_taskq_post_msg(MUSIC_TASK_NAME, 2, MSG_MUSIC_SPC_FILE,song);
			}
			break;
    }
}

u8 BtApiFramePop(u8 *fram,u8 *len)
{
	if(BtComFrame.Length)
	{
	    *fram = BtComFrame.ReadPoint++;
        *len = BtComFrame.framelen[*fram];
		if(BtComFrame.ReadPoint >= 4)
		{
			BtComFrame.ReadPoint = 0;
		}
		BtComFrame.Length--;
	}
	else
	{
		BtComFrame.Length =0;
		BtComFrame.ReadPoint =0;
		BtComFrame.WritePoint = 0;
        return FALSE;
	}
	return TRUE;
}

void BtComSendPoll(void)
{
	u8 frame=0;
	u8 len=0;
	if(!spp_connect_falg) 

	{
		return;
    }

	if(BtComFrame.bigbuf_flag)
	{
		BtComFrame.bigbuf_flag = 0;
       //btapi_printf("send big data start\n");
       //for(i=0;i<BtComFrame.bigbuf_len;i++)
       //{
       //    btapi_printf("%x ",BtComFrame.bigbuf[i]);
       //}
       Notify_data_send(BtComFrame.bigbuf,BtComFrame.bigbuf_len);
       memset(BtComFrame.bigbuf,0,BIG_FRAME_BUF);
       BtComFrame.bigbuf_len = 0;
       return;
    }

    if(!BtApiFramePop(&frame,&len)) 
    {
    	return;
	}
    Notify_data_send(&BtComFrame.combuf[frame][0],len);
}
void clr_play_mode_list(u8 playmode)
{
    Listbrowse_over = 0;
	sppbuf[0]= 0xFF;
	sppbuf[1]= 0xAA;
	sppbuf[2]= 0x5;
	sppbuf[3]= 'Z';
	sppbuf[4]='A';
	sppbuf[5] = 0xa;
	sppbuf[6] = playmode;
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
    Notify_data_send(sppbuf,8);
}
void compare_play_mode(void)
{
    static u8 last_mode=0x2;
    static u8 cur_mode=0x2;
    if(compare_task_name(MUSIC_USB_NAME))
    {//usb mode
    	cur_mode =0x02;
    }
    else if(compare_task_name(MUSIC_CARD_NAME))
    {//sd mode
    	cur_mode =0x01;
    }
	if(cur_mode != last_mode)
	{
//		os_taskq_post_event(MAIN_TASK_NAME,2,SYS_CLR_LIST,last_mode);
        clr_play_mode_list(last_mode);
		last_mode = cur_mode;
	}
}
#endif
#endif

