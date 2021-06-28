#include "ui_disc/ui_disc.h"
#include "disp.h"
#include "key_drv_ad.h"


#if defined(_SUPPORT_DISC_)

static disc_disp_t *disc_disp = NULL;

static void ui_show_disc_clean(void)
{
	play_id3_icon_disp(0xFF);
	play_mode_icon_disp(0xFF);
	play_media_icon_disp(0xFF);
	
	show_clear_icon(DISP_LEVEL_NORMAL);
	show_clear_num(DISP_LEVEL_NORMAL);
	
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAY);
    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PAUSE);

#if defined(DISP_SCH_ICON)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SCH);
#endif
	
#if defined(_SIMULATIVE_VBAR_)
    global.pause_flg = FALSE;
#endif
#if defined(RPT_ON_CHANGE_KEY_FUNCV)
    global.rpt_flg = FALSE;
#endif

}

void ui_show_disc_init(uint8 source)
{
	disc_disp = (disc_disp_t *)malloc_fun(disc_disp ,sizeof(disc_disp_t), 0);
	if(disc_disp == NULL)
	{
		ew_puts("ui_show_disc_init err\n");
        while(1);
	}
	disc_disp->source = source;
	
	ui_show_disc_clean();
	
	if(disc_disp->source == DISC_SOURCE_DISC)
	{
		ui_show_setting(LANG_ID_MUSIC_DISC, 0);
		ui_show_normal(LANG_ID_MUSIC_DISC);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R4);
		
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_DISC);
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_FLASH);
	}
	else if(disc_disp->source == DISC_SOURCE_USB)
	{
		ui_show_setting(LANG_ID_MUSIC_USB, 0);
		ui_show_normal(LANG_ID_MUSIC_USB);
	#if defined(_DISABLE_USB_SD_FLASH_)
	#else
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_USB);
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_FLASH);
	#endif
	}
	else if(disc_disp->source == DISC_SOURCE_CARD)
	{
		ui_show_setting(LANG_ID_MUSIC_SD, 0);
		ui_show_normal(LANG_ID_MUSIC_SD);
	#if defined(_DISABLE_USB_SD_FLASH_)
	#else
    	ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_SD);
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_FLASH);
	#endif
	}
}

void ui_show_disc_exit(void)
{
	ui_show_disc_clean();
	
	if(disc_disp != NULL)
	{
		if(disc_disp->source == DISC_SOURCE_DISC)
		{
			ui_drv_func.flash_disable(UI_ICON_DISC);
			ui_drv_func.flash_disable(UI_ICON_FLASH);
			
			if(DETECT_DISC_CHECK())
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R4);
			}
			else
			{
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R4);
				
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_DISC);
			}
			
		}
		else if(disc_disp->source == DISC_SOURCE_USB)
		{
	    	ui_drv_func.flash_disable(UI_ICON_USB);
			ui_drv_func.flash_disable(UI_ICON_FLASH);
		}
		else if(disc_disp->source == DISC_SOURCE_CARD)
		{
	    	ui_drv_func.flash_disable(UI_ICON_SD);
			ui_drv_func.flash_disable(UI_ICON_FLASH);
		}
		
		free_fun((void**)&disc_disp);
	}
}

void ui_show_disc_play(void *info)
{
	if(disc_disp == NULL)return;
	disc_disp_info_t *disp_info = (disc_disp_info_t *)info;
	uint8 dot_flag = FALSE;
	uint8 id3_flag = FALSE;
	
	if(disc_disp->info.status != disp_info->status)
	{
		disc_disp->info.status = disp_info->status;
		
		if(disc_disp->info.status == DISC_STATUS_PAUSE)
	    {
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PAUSE);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAY);
	    #if defined(_SIMULATIVE_VBAR_)
	    	global.pause_flg = TRUE;
	    #endif
		}
		else if(disc_disp->info.status == DISC_STATUS_STOP
			 || disc_disp->info.status == DISC_STATUS_FORWARD
			 || disc_disp->info.status == DISC_STATUS_BACKWARD)
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PAUSE);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAY);
	    #if defined(_SIMULATIVE_VBAR_)
	    	global.pause_flg = TRUE;
	    #endif
		}
		else if(disc_disp->info.status == DISC_STATUS_PLAYE)
		{
	    #if defined(_DA975_SPECIAL_MUTE_)
	    	global.load_flag = FALSE;
	    #endif
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PLAY);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PAUSE);
	    #if defined(_SIMULATIVE_VBAR_)
	    	global.pause_flg = FALSE;
	    #endif
		}
	}

	if(disc_disp->info.mode != disp_info->mode)
	{
		disc_disp->info.mode = disp_info->mode;
	#if defined(RPT_ON_CHANGE_KEY_FUNCV)
		global.rpt_flg = FALSE;
	#endif
		switch(disc_disp->info.mode)
		{
			case DISC_MODE_REPEAT_ONE:
				play_mode_icon_disp(PLAY_MODE_REPEAT_ONE);
			#if defined(RPT_ON_CHANGE_KEY_FUNCV)
				global.rpt_flg = TRUE;
			#endif
				break;
				
			case DISC_MODE_REPEAT_FOLDER:
				play_mode_icon_disp(PLAY_MODE_REPEAT_FOLDER);
			#if defined(RPT_ON_CHANGE_KEY_FUNCV)
				global.rpt_flg = TRUE;
			#endif
				break;
				
			case DISC_MODE_REPEAT_ALL:
				play_mode_icon_disp(PLAY_MODE_REPEAT_ALL);
				break;
				
			case DISC_MODE_SHUFFLE:
				play_mode_icon_disp(PLAY_MODE_SHUFFLE);
				break;
				
			case DISC_MODE_INTRO:
				play_mode_icon_disp(PLAY_MODE_INTRO);
				break;
				
			default:
				play_mode_icon_disp(0xFF);
				break;
		}
	}

	if(disc_disp->info.search_mode != disp_info->search_mode)
	{
		if(disc_disp->info.search_mode == DISC_SEARCH_MODE_NORMAL)
		{
			id3_flag = TRUE;
		}
		else if(disp_info->search_mode == DISC_SEARCH_MODE_NORMAL)
		{
			id3_flag = TRUE;
		}
		
		disc_disp->info.search_mode = disp_info->search_mode;
	}

	if(disc_disp->info.search_status != disp_info->search_status)
	{
		disc_disp->info.search_status = disp_info->search_status;
		if(disc_disp->info.search_mode == DISC_SEARCH_MODE_NORMAL)
		{
			id3_flag = TRUE;
			
		}
	#if defined(DISP_SCH_ICON)
		else if(disc_disp->info.search_mode != DISC_SEARCH_MODE_INIT)
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SCH);
		}
	#endif
	}
	
	if(id3_flag)
	{
	#if defined(DISP_SCH_ICON)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SCH);
	#endif
		if(disc_disp->info.search_mode == DISC_SEARCH_MODE_NORMAL)
		{
			//ew_printf("disc_disp->info.search_status:%d\r\n",disc_disp->info.search_status);

			switch(disc_disp->info.search_status)
			{
				case DISC_SEARCH_STATUS_DISP_FILE_NAME:
					play_id3_icon_disp(ID3_DISP_STATUS_FILE_NAME);
					break;
					
				case DISC_SEARCH_STATUS_DISP_SONG_NAME:
					play_id3_icon_disp(ID3_DISP_STATUS_SONG_NAME);
					break;
					
				case DISC_SEARCH_STATUS_DISP_ARTIST:
					play_id3_icon_disp(ID3_DISP_STATUS_ARTIST);
					break;
					
				case DISC_SEARCH_STATUS_DISP_ALBUM:
					play_id3_icon_disp(ID3_DISP_STATUS_ALBUM);
					break;
					
				case DISC_SEARCH_STATUS_DISP_DIR:
					play_id3_icon_disp(ID3_DISP_STATUS_DIR);
					break;
					
				default:
					play_id3_icon_disp(0xFF);
					break;
			}
		}
		else
		{
			play_id3_icon_disp(0xFF);
		}
	}
	if(disc_disp->info.media != disp_info->media)
	{
		disc_disp->info.media = disp_info->media;
		
		ew_printf("disc_disp->info.media:%d\r\n",disc_disp->info.media);
		
		switch(disc_disp->info.media)
		{
			case DISC_MEDIA_MP3:
				play_media_icon_disp(PLAY_MEDIA_MP3);
				break;
				
			case DISC_MEDIA_WMA:
				play_media_icon_disp(DISC_MEDIA_WMA);
				break;
				
			case DISC_MEDIA_WAV:
				play_media_icon_disp(DISC_MEDIA_WAV);
				break;
				
			case DISC_MEDIA_CD:
				play_media_icon_disp(DISC_MEDIA_CD);
				break;
				
			case DISC_MEDIA_MP4:
				play_media_icon_disp(DISC_MEDIA_MP4);
				break;
			case DISC_MEDIA_VCD:
				play_media_icon_disp(DISC_MEDIA_VCD);
				break;
				
			case DISC_MEDIA_DVD:
				play_media_icon_disp(DISC_MEDIA_DVD);
				break;
				
			case DISC_MEDIA_ASF:
			case DISC_MEDIA_JPG:
			case DISC_MEDIA_TXT:	
			default:
				play_media_icon_disp(0xFF);
				break;
		}		
	}

	if(disc_disp->info.error != disp_info->error)
	{
		disc_disp->info.error = disp_info->error;
	}
	
	if(memcmp(disc_disp->info.disp_buffer, disp_info->disp_buffer, sizeof(disc_disp->info.disp_buffer)))
	{
		memcpy(disc_disp->info.disp_buffer, disp_info->disp_buffer, sizeof(disc_disp->info.disp_buffer));
		show_clear_icon(DISP_LEVEL_NORMAL);
		ui_show_string(DISP_LEVEL_NORMAL, disc_disp->info.disp_buffer);
		dot_flag = TRUE;
	}
	
	if(disc_disp->info.disp_num != disp_info->disp_num)
	{
		uint8 num = disc_disp->info.disp_num = disp_info->disp_num;
		
		if(!num)num = 0xFF;
		ui_show_num(DISP_LEVEL_NORMAL, num);
	}

	if(dot_flag || (disc_disp->info.dot_flag != disp_info->dot_flag))
	{
		disc_disp->info.dot_flag = disp_info->dot_flag;
		if(disc_disp->info.dot_flag)
		{
		#if defined(CLOCK_REPALCE_POINT)
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_DASH);
		#elif defined(CLOCK_REPALCE_POINT2)
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
		#elif defined(_ENABLE_TIME_DISPLAY_)
    		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
    		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
		#else
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
		#endif
		}
		else
		{
		#if defined(CLOCK_REPALCE_POINT)
			ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
			ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_DASH);
		#elif defined(CLOCK_REPALCE_POINT2)
			ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
		#elif defined(_ENABLE_TIME_DISPLAY_)
			ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
			ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
		#else
			ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
		#endif
		}
	}
}

void ui_show_disc_polling(void)
{
	if(disc_disp == NULL)return;
	
	if(disc_disp->source == DISC_SOURCE_DISC)
	{
	    if(disc_disp->info.status == DISC_STATUS_PAUSE
		|| disc_disp->info.status == DISC_STATUS_STOP) 
	    {
	    	if(disc_disp->disc_state != 0xFF)
	    	{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R4);
				disc_disp->disc_state = 0xFF;
	    	}
		}
		else
	    {
	    	if(disc_disp->disc_time)disc_disp->disc_time--;
	    	else
	    	{
	    		disc_disp->disc_time = 333/VFD_DISP_POLLING_TIME;
	    		switch(disc_disp->disc_state)
	    		{
	    			default:
				#if defined(_ENABLE_DISC_FLASH_)
    				case 0:
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
    					ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R4);
    					disc_disp->disc_state = 1;
    					break;
    				case 1:
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R4);
    					ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
    					disc_disp->disc_state = 2;
    					break;
    				case 2:
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R4);
    					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
    					ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
    					disc_disp->disc_state = 3;
    					break;
					case 3:
						ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R4);
						ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
						ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
						ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
						disc_disp->disc_state = 0;
						break;

				#else
	    			case 0:
	    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
	    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
	    				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
	    				disc_disp->disc_state = 1;
	    				break;
	    			case 1:
	    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
	    				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
	    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
	    				disc_disp->disc_state = 2;
	    				break;
	    			case 2:
	    				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
	    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
	    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
	    				disc_disp->disc_state = 0;
	    				break;
				#endif
	    		}
	    	}
	    }
	}
}
#endif

