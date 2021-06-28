#include "sdk_cfg.h"
#include "ui_play/ui_play.h"
#include "music_info.h"
#include "setup.h"

#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_Music_Search(uint16 Tune,uint32 total);
#endif
static play_disp_t *play_disp = NULL;
extern u8 Get_id3Info_err;
extern u8 gDir_disp_flag;

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
static void play_id3_icon_disp(uint8 id3)
{
	uint16 disp_buf[6];
	uint8  disp_ctrl[5];

	disp_buf[0] = UI_ICON_FOLDER;	disp_ctrl[0] = (id3 == ID3_DISP_STATUS_FILE_NAME)? 1 : 0;
	disp_buf[1] = UI_ICON_FILE;		disp_ctrl[1] = (id3 == ID3_DISP_STATUS_DIR) 	 ? 1 : 0;
	disp_buf[2] = UI_ICON_MUSIC;	disp_ctrl[2] = (id3 == ID3_DISP_STATUS_SONG_NAME)? 1 : 0;
	disp_buf[3] = UI_ICON_ALBUM;	disp_ctrl[3] = (id3 == ID3_DISP_STATUS_ALBUM) 	 ? 1 : 0;
	disp_buf[4] = UI_ICON_ARTIST;	disp_ctrl[4] = (id3 == ID3_DISP_STATUS_ARTIST) 	 ? 1 : 0;
	disp_buf[5] = UI_ICON_NULL;

	ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
}

static void play_mode_icon_disp(uint8 mode)
{
#if defined(_ENABLE_RPT_ICON_)
    uint16 disp_buf[6];
    uint8  disp_ctrl[5];
    
    disp_buf[0] = UI_ICON_RPT;
    disp_buf[1] = UI_ICON_RDM;
    disp_buf[2] = UI_ICON_INT;
    disp_buf[3] = UI_ICON_RPT1;
    disp_buf[4] = UI_ICON_RPT_ALL;
    disp_buf[5] = UI_ICON_NULL;
    
    if((mode == PLAY_MODE_REPEAT_ONE)||(mode == PLAY_MODE_REPEAT_FOLDER))
    {
    	disp_ctrl[0] = 1;
    	disp_ctrl[3] = 1;
    	disp_ctrl[4] = 0;
	}
	else
	{
		disp_ctrl[0] = 0;
		if(mode == 0xFF)
		{
			disp_ctrl[3] = 0;
			disp_ctrl[4] = 0;
		}
		else
		{
			disp_ctrl[3] = 1;
			disp_ctrl[4] = 1;
		}
	}
    
    if(mode == PLAY_MODE_SHUFFLE)
    	disp_ctrl[1] = 1;
    else
    	disp_ctrl[1] = 0;
    
    if(mode == PLAY_MODE_INTRO)
    	disp_ctrl[2] = 1;
    else
    	disp_ctrl[2] = 0;

#else
	uint16 disp_buf[4];
	uint8  disp_ctrl[3];

	disp_buf[0] = UI_ICON_RPT;
	disp_buf[1] = UI_ICON_RDM;
	disp_buf[2] = UI_ICON_INT;
	disp_buf[3] = UI_ICON_NULL;


	if((mode == PLAY_MODE_REPEAT_ONE)||(mode == PLAY_MODE_REPEAT_FOLDER))
		disp_ctrl[0] = 1;
	else
		disp_ctrl[0] = 0;

	if(mode == PLAY_MODE_SHUFFLE)
		disp_ctrl[1] = 1;
	else
		disp_ctrl[1] = 0;

	if(mode == PLAY_MODE_INTRO)
		disp_ctrl[2] = 1;
	else
		disp_ctrl[2] = 0;
#endif

	ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
}

static void play_media_icon_disp(uint8 media)
{
#if defined(_SUPPORT_DISC_)
	uint16 disp_buf[8];
	uint8  disp_ctrl[7];
	//printf("play_media_icon_disp__%d",media);
	disp_buf[0] = UI_ICON_MP3; 		disp_ctrl[0] = (media == PLAY_MEDIA_MP3) ? 1 : 0;
	disp_buf[1] = UI_ICON_WMA; 		disp_ctrl[1] = (media == PLAY_MEDIA_WMA) ? 1 : 0;
	disp_buf[2] = UI_ICON_WAV; 		disp_ctrl[2] = (media == PLAY_MEDIA_WAV) ? 1 : 0;
	disp_buf[3] = UI_ICON_MP4; 		disp_ctrl[3] = (media == PLAY_MEDIA_MP4) ? 1 : 0;
	disp_buf[4] = UI_ICON_VCD; 		disp_ctrl[4] = (media == PLAY_MEDIA_VCD) ? 1 : 0;
	disp_buf[5] = UI_ICON_DVD; 		disp_ctrl[5] = (media == PLAY_MEDIA_DVD) ? 1 : 0;
	disp_buf[6] = UI_ICON_CD; 		disp_ctrl[6] = (media == PLAY_MEDIA_CD) ? 1 : 0;
	disp_buf[7] = UI_ICON_NULL;
#else
	uint16 disp_buf[6];
	uint8  disp_ctrl[5];

	disp_buf[0] = UI_ICON_MP3; 		disp_ctrl[0] = (media == PLAY_MEDIA_MP3) ? 1 : 0;
	disp_buf[1] = UI_ICON_WMA; 		disp_ctrl[1] = (media == PLAY_MEDIA_WMA) ? 1 : 0;
	disp_buf[2] = UI_ICON_WAV; 		disp_ctrl[2] = (media == PLAY_MEDIA_WAV) ? 1 : 0;
	disp_buf[3] = UI_ICON_FLAC; 	disp_ctrl[3] = (media == PLAY_MEDIA_FLAC)? 1 : 0;
	disp_buf[4] = UI_ICON_APE; 		disp_ctrl[4] = (media == PLAY_MEDIA_APE) ? 1 : 0;
	disp_buf[5] = UI_ICON_NULL;
#endif
	ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
}


#if defined(_XM003_SPECIAL_DISPLAY_)
static void play_speed_disp(void)
{
	uint8 Min;
	uint8 Sec;
	uint8 disp_buf[10];
	uint8 index;

	uint16 tune = ID3_CTRL_INFO.tune%10000;
	Min = ID3_CTRL_INFO.min;
	Sec = ID3_CTRL_INFO.sec;

	if(Min != ID3_CTRL_DISP.Min
	 ||Sec != ID3_CTRL_DISP.Sec
	 ||GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
	{
		ID3_CTRL_DISP.Min = Min;
		ID3_CTRL_DISP.Sec = Sec;

		Min%=100;
		Sec%=100;
		index = 0;

    	if(tune < 1000)
    	{
    		disp_buf[index++] =  tune/100 	+ '0';
			disp_buf[index++] = (tune%100)/10 + '0';
			disp_buf[index++] = (tune%100)%10 + '0';
		#if !defined(_XM003_SPECIAL_DISPLAY_)
        	disp_buf[index++] = '-';
		#endif
    	}
		else
		{
		#if !defined(_XM003_SPECIAL_DISPLAY_)
    		disp_buf[index++] = (tune/100)/10 + '0';
		#endif
			disp_buf[index++] = (tune/100)%10 + '0';
			disp_buf[index++] = (tune%100)/10 + '0';
			disp_buf[index++] = (tune%100)%10 + '0';
		}

    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';

		VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,VFD_DISP_TIME_CLOCK|0);
	}
}

#elif defined(_SPECIAL_FF_AND_FB_)
static void play_speed_disp(void)
{	
	uint8 Min;
	uint8 Sec;
	uint8 disp_buf[10];
	uint8 index;
	
	uint16 tune = ID3_CTRL_INFO.tune%10000;
	Min = ID3_CTRL_INFO.min;
	Sec = ID3_CTRL_INFO.sec;
	
	if(Min != ID3_CTRL_DISP.Min 
	 ||Sec != ID3_CTRL_DISP.Sec 
	 ||GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
	{
		ID3_CTRL_DISP.Min = Min;
		ID3_CTRL_DISP.Sec = Sec;
		
		Min%=100;
		Sec%=100;
		index = 0;

    	if(tune < 1000)
    	{
    		disp_buf[index++] =  tune/100 	+ '0';
			disp_buf[index++] = (tune%100)/10 + '0';
			disp_buf[index++] = (tune%100)%10 + '0';
        	disp_buf[index++] = '-';
    	}
		else
		{
    		disp_buf[index++] = (tune/100)/10 + '0';
			disp_buf[index++] = (tune/100)%10 + '0';
			disp_buf[index++] = (tune%100)/10 + '0';
			disp_buf[index++] = (tune%100)%10 + '0';
		}
	
    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	
		VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,VFD_DISP_TIME_CLOCK|0);
	}
}

#elif defined(_SL6812A_SPECIAL_DISPLAY_)
static void play_speed_disp(void)
{	
	uint8 Min;
	uint8 Sec;
	uint8 disp_buf[10];
	uint8 index;
	
	uint16 tune = ID3_CTRL_INFO.tune%10000;
	Min = ID3_CTRL_INFO.min;
	Sec = ID3_CTRL_INFO.sec;
	
	if(Min != ID3_CTRL_DISP.Min 
	 ||Sec != ID3_CTRL_DISP.Sec 
	 ||GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
	{
		ID3_CTRL_DISP.Min = Min;
		ID3_CTRL_DISP.Sec = Sec;
		
		Min%=100;
		Sec%=100;
		index = 0;

    	if(tune < 1000)
    	{
        	disp_buf[index++] = 'T';
    		disp_buf[index++] =  tune/100 	+ '0';
			disp_buf[index++] = (tune%100)/10 + '0';
			disp_buf[index++] = (tune%100)%10 + '0';
        	disp_buf[index++] = ' ';
    	}
		else
		{
        	disp_buf[index++] = 'T';
    		disp_buf[index++] = (tune/100)/10 + '0';
			disp_buf[index++] = (tune/100)%10 + '0';
			disp_buf[index++] = (tune%100)/10 + '0';
			disp_buf[index++] = (tune%100)%10 + '0';
		}
	
		disp_buf[index++] = ' ';
    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	
		VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,VFD_DISP_TIME_CLOCK|0);
	}
}

#else
static void play_speed_disp(void)
{
	uint8 Min;
	uint8 Sec;
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#else
	uint8 disp_buf[10];
#endif
	uint8 index;

	Min = ID3_CTRL_INFO.min;
	Sec = ID3_CTRL_INFO.sec;

#if defined(_FY5517_SPECIAL_DISPLAY_)||defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)||defined(SPECIAL_SLA6799_DISP)
	uint16 tune = ID3_CTRL_INFO.tune%10000;
#endif

	if(Min != ID3_CTRL_DISP.Min
	 ||Sec != ID3_CTRL_DISP.Sec
	 ||GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
	{
		ID3_CTRL_DISP.Min = Min;
		ID3_CTRL_DISP.Sec = Sec;

		Min%=100;
		Sec%=100;
		index = 0;
	#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
    #if defined(_LCD_3_DIAPLAY_)
		if(disp_buf[0] == '0')
		{
			disp_buf[0] = ' ';
		}
    #endif
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
		disp_buf[index++] = ' ';
    	if(ID3_CTRL_INFO.state == PLAY_STATUS_FORWARD)
    	{
    		disp_buf[index++] = 'F';
    		disp_buf[index++] = 'F';
    	}
    	else
    	{
    		disp_buf[index++] = 'F';
    		disp_buf[index++] = 'B';
    	}
		disp_buf[index++] = '-';
		disp_buf[index++] =  tune/100	+ '0';
		disp_buf[index++] = (tune%100)/10 + '0';
		disp_buf[index++] = (tune%100)%10 + '0';
    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	#elif defined(_FY7000_SPECIAL_DISPLAY_)
    	disp_buf[index++] = ' ';
    	disp_buf[index++] = ' ';
    	if(ID3_CTRL_INFO.state == PLAY_STATUS_FORWARD)
    	{
    		disp_buf[index++] = 'F';
    		disp_buf[index++] = 'F';
    		disp_buf[index++] = '-';
    		disp_buf[index++] = '>';
    	}
    	else
    	{
    		disp_buf[index++] = 'F';
    		disp_buf[index++] = 'B';
    		disp_buf[index++] = '-';
    		disp_buf[index++] = '<';
    	}
		#if defined (_SUPPORT_LCM_1618_DRIVE)
		disp_buf[index++] = ' ';
    	disp_buf[index++] = ' ';
    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
		disp_buf[index++] = ':';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
		#else
    	disp_buf[index++] = ' ';
    	disp_buf[index++] = ' ';
    	disp_buf[index++] = ' ';
    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
		#endif
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)
    	if(tune < 1000)
    	{
    		disp_buf[index++] = 'U';
    		disp_buf[index++] =  tune/100 	+ '0';
    		disp_buf[index++] = (tune%100)/10 + '0';
    		disp_buf[index++] = (tune%100)%10 + '0';
    	}
    	else
    	{
    		disp_buf[index++] = (tune/100)/10 + '0';
    		disp_buf[index++] = (tune/100)%10 + '0';
    		disp_buf[index++] = (tune%100)/10 + '0';
    		disp_buf[index++] = (tune%100)%10 + '0';
    	}
    	disp_buf[index++] = ' ';
    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
    	disp_buf[index++] = ' ';
	#elif defined(_SH1330_SPECIAL_DISPLAY_)
    	if(tune < 1000)
    	{
    		disp_buf[index++] = 'T';
    		disp_buf[index++] =  tune/100 	+ '0';
    		disp_buf[index++] = (tune%100)/10 + '0';
    		disp_buf[index++] = (tune%100)%10 + '0';
    	}
    	else
    	{
    		disp_buf[index++] = (tune/100)/10 + '0';
    		disp_buf[index++] = (tune/100)%10 + '0';
    		disp_buf[index++] = (tune%100)/10 + '0';
    		disp_buf[index++] = (tune%100)%10 + '0';
    	}
    	disp_buf[index++] = ' ';
		disp_buf[index++] = ' ';
    	disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	#elif defined(_FIVE_CHAR_DISPLAY_)
	#if defined(_MR889_SPECAIL_DISP_)
		if(ID3_CTRL_INFO.state == PLAY_STATUS_FORWARD)
		{
			disp_buf[index++] = 'F';
			disp_buf[index++] = 'F';
		}
		else
		{
			disp_buf[index++] = 'F';
			disp_buf[index++] = 'B';
		}
	    disp_buf[index++] = '-';
	    disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	#elif defined(SPECIAL_SLA6799_DISP)
	    if(ID3_CTRL_INFO.state == PLAY_STATUS_FORWARD)
		{
			disp_buf[index++] = tune/100 	+ '0';
			disp_buf[index++] = (tune%100)/10 + '0';
    		disp_buf[index++] = (tune%100)%10 + '0';
		}
		else
		{
			disp_buf[index++] = tune/100 	+ '0';
			disp_buf[index++] = (tune%100)/10 + '0';
    		disp_buf[index++] = (tune%100)%10 + '0';
		}
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	#else
		if(ID3_CTRL_INFO.state == PLAY_STATUS_FORWARD)
		{
			disp_buf[index++] = 'F';
			disp_buf[index++] = 'F';
		}
		else
		{
			disp_buf[index++] = 'F';
			disp_buf[index++] = 'B';
		}
	    disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	#endif
	#elif defined(_SEVEN_CHAR_DISPLAY_)
    	if(ID3_CTRL_INFO.state == PLAY_STATUS_FORWARD)
    	{
    		disp_buf[index++] = 'F';
    		disp_buf[index++] = 'F';
    		disp_buf[index++] = ' ';
    	}
    	else
    	{
    		disp_buf[index++] = 'F';
    		disp_buf[index++] = 'B';
    		disp_buf[index++] = ' ';
    	}
		disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	#elif defined(_YS_SPECIAL_DISP_)
		uint16 tune = ID3_CTRL_INFO.tune%10000;

    	if(tune < 100)
    	{
    		disp_buf[0] = 'T';
			disp_buf[1] = (tune%100)/10 + '0';
			disp_buf[2] = (tune%100)%10 + '0';
        	disp_buf[3] = '-';

			disp_buf[4] = Min/10 + '0';
			disp_buf[5] = Min%10 + '0';

			disp_buf[6] = Sec/10 + '0';
			disp_buf[7] = Sec%10 + '0';

			disp_buf[8] = 0;
    	}
    	else
    	{
    		disp_buf[0] = (tune/100)/10 + '0';
			disp_buf[1] = (tune/100)%10 + '0';
			disp_buf[2] = (tune%100)/10 + '0';
        	disp_buf[3] = '-';

    		disp_buf[4] = Min/10 + '0';
    		disp_buf[5] = Min%10 + '0';

			disp_buf[6] = Sec/10 + '0';
			disp_buf[7] = Sec%10 + '0';
			disp_buf[8] = 0;
    	}
	#elif defined(_FY7000D_FF_FB_DISPLAY_)
		uint16 tune = ID3_CTRL_INFO.tune%10000;

    	if(tune < 1000)
    	{
    		disp_buf[0] =  tune/100 	+ '0';
    		disp_buf[1] = (tune%100)/10 + '0';
    		disp_buf[2] = (tune%100)%10 + '0';
    		disp_buf[3] = '-';

    		disp_buf[4] = Min/10 + '0';
    		disp_buf[5] = Min%10 + '0';

    		disp_buf[6] = Sec/10 + '0';
    		disp_buf[7] = Sec%10 + '0';

    		disp_buf[8] = 0;
    	}
    	else
    	{
    		disp_buf[0] = (tune/100)/10 + '0';
    		disp_buf[1] = (tune/100)%10 + '0';
    		disp_buf[2] = (tune%100)/10 + '0';
    		disp_buf[3] = (tune%100)%10 + '0';

    		if(Min < 10)
    		{
    			disp_buf[4] = '-';
    			disp_buf[5] = Min%10 + '0';
    		}
    		else
    		{
    			disp_buf[4] = Min/10 + '0';
    			disp_buf[5] = Min%10 + '0';
    		}
    		disp_buf[6] = Sec/10 + '0';
    		disp_buf[7] = Sec%10 + '0';
    		disp_buf[8] = 0;
    	}
	#elif defined(_TS8010_FF_FB_DISPLAY_)||defined(_WLS316_SPECIAL_DISPLAY_)
		uint16 tune = ID3_CTRL_INFO.tune%10000;
    	if(tune < 1000)
    	{
    		disp_buf[0] =  tune/100 	+ '0';
    		disp_buf[1] = (tune%100)/10 + '0';
    		disp_buf[2] = (tune%100)%10 + '0';
    		disp_buf[3] = '-';

			disp_buf[4] = Min/10 + '0';
    		disp_buf[5] = Min%10 + '0';

    		disp_buf[6] = Sec/10 + '0';
    		disp_buf[7] = Sec%10 + '0';

    	}
    	else
    	{
    		disp_buf[0] = (tune/100)/10 + '0';
    		disp_buf[1] = (tune/100)%10 + '0';
    		disp_buf[2] = (tune%100)/10 + '0';
    		disp_buf[3] = (tune%100)%10 + '0';

    		if(Min < 10)
    		{
    			disp_buf[4] = '-';
    			disp_buf[5] = Min%10 + '0';
    		}
    		else
    		{
    			disp_buf[4] = Min/10 + '0';
    			disp_buf[5] = Min%10 + '0';
    		}
    		disp_buf[6] = Sec/10 + '0';
    		disp_buf[7] = Sec%10 + '0';
    	}
	#else
		if(ID3_CTRL_INFO.state == PLAY_STATUS_FORWARD)
		{
		#if defined(_ENABLE_A5900_USB_)
    		disp_buf[index++] = 'X';
    		disp_buf[index++] = '2';
			disp_buf[index++] = ' ';
			disp_buf[index++] = ' ';
		#else
			disp_buf[index++] = 'F';
			disp_buf[index++] = 'F';
			disp_buf[index++] = '-';
			disp_buf[index++] = '>';
		#endif
		}
		else
		{
		#if defined(_ENABLE_A5900_USB_)
			disp_buf[index++] = 'X';
			disp_buf[index++] = '2';
			disp_buf[index++] = ' ';
			disp_buf[index++] = ' ';
		#else
			disp_buf[index++] = 'F';
			disp_buf[index++] = 'B';
			disp_buf[index++] = '-';
			disp_buf[index++] = '<';
		#endif
		}
	    disp_buf[index++] = Min/10 + '0';
    	disp_buf[index++] = Min%10 + '0';
    	disp_buf[index++] = Sec/10 + '0';
    	disp_buf[index++] = Sec%10 + '0';
	#endif
		VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,VFD_DISP_TIME_CLOCK|0);
	}
}
#endif

static void play_time_disp(void)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#else
    uint8 disp_buf[10];
#endif
	uint16 tune = ID3_CTRL_INFO.tune%10000;
	uint8 min = ID3_CTRL_INFO.min%100;
	uint8 sec = ID3_CTRL_INFO.sec%100;

	if(tune != ID3_CTRL_DISP.Tune
	|| min  != ID3_CTRL_DISP.Min
	|| sec  != ID3_CTRL_DISP.Sec
	|| GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
	{
		ID3_CTRL_DISP.Tune 	= tune;
		ID3_CTRL_DISP.Min 	= min;
		ID3_CTRL_DISP.Sec 	= sec;
	#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
        disp_buf[0] = min/10 + '0';
        disp_buf[1] = min%10 + '0';
        disp_buf[2] = sec/10 + '0';
        disp_buf[3] = sec%10 + '0';
	#if defined(_LCD_3_DIAPLAY_)
		if(disp_buf[0] == '0')
		{
			disp_buf[0] = ' ';
		}
	#if defined(_LCD_3_DIAPLAY_)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
	#endif
	
	#endif
	#elif defined(_XM003_SPECIAL_DISPLAY_)
		if(tune < 1000)
		{
			disp_buf[0] =  tune/100 	+ '0';
			disp_buf[1] = (tune%100)/10 + '0';
			disp_buf[2] = (tune%100)%10 + '0';
		}
		else
		{
			disp_buf[0] = (tune/100)%10 + '0';
			disp_buf[1] = (tune%100)/10 + '0';
			disp_buf[2] = (tune%100)%10 + '0';
		}

		disp_buf[3] = min/10 + '0';
		disp_buf[4] = min%10 + '0';
		disp_buf[5] = sec/10 + '0';
		disp_buf[6] = sec%10 + '0';
		disp_buf[7] = ' ';
	#elif defined(_FY7000_SPECIAL_DISPLAY_)
    	disp_buf[0] = ' ';
    	disp_buf[1] = ' ';
		disp_buf[2] = (tune/100)/10 + '0';
		disp_buf[3] = (tune/100)%10 + '0';
		disp_buf[4] = (tune%100)/10 + '0';
		disp_buf[5] = (tune%100)%10 + '0';
    	disp_buf[6] = ' ';
    	disp_buf[7] = ' ';
		#if defined (_SUPPORT_LCM_1618_DRIVE)
		disp_buf[8] = min/10 + '0';
		disp_buf[9] = min%10 + '0';
		disp_buf[10] = ':';
		disp_buf[11] = sec/10 + '0';
		disp_buf[12] = sec%10 + '0';
		disp_buf[13] = 0;
		#else
    	disp_buf[8] = ' ';
		disp_buf[9] = min/10 + '0';
		disp_buf[10] = min%10 + '0';
		disp_buf[11] = sec/10 + '0';
		disp_buf[12] = sec%10 + '0';
		disp_buf[13] = 0;
		#endif
	#elif defined(_SL6812A_SPECIAL_DISPLAY_)
		if(tune < 1000)
		{
			disp_buf[0] = 'T';
    		disp_buf[1] =  tune/100 	+ '0';
    		disp_buf[2] = (tune%100)/10 + '0';
    		disp_buf[3] = (tune%100)%10 + '0';
			disp_buf[4] = ' ';
		}
		else
		{
			disp_buf[0] = 'T';
			disp_buf[1] = (tune/100)/10 + '0';
			disp_buf[2] = (tune/100)%10 + '0';
			disp_buf[3] = (tune%100)/10 + '0';
			disp_buf[4] = (tune%100)%10 + '0';
		}
		disp_buf[5] = ' ';
		disp_buf[6] = min/10 + '0';
		disp_buf[7] = min%10 + '0';
		disp_buf[8] = sec/10 + '0';
		disp_buf[9] = sec%10 + '0';
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)
		if(tune < 1000)
		{
			disp_buf[0] = 'U';
    		disp_buf[1] =  tune/100 	+ '0';
    		disp_buf[2] = (tune%100)/10 + '0';
    		disp_buf[3] = (tune%100)%10 + '0';
		}
		else
		{
			disp_buf[0] = (tune/100)/10 + '0';
			disp_buf[1] = (tune/100)%10 + '0';
			disp_buf[2] = (tune%100)/10 + '0';
			disp_buf[3] = (tune%100)%10 + '0';
		}
    	disp_buf[4] = ' ';
    	disp_buf[5] = min/10 + '0';
    	disp_buf[6] = min%10 + '0';
    	disp_buf[7] = sec/10 + '0';
    	disp_buf[8] = sec%10 + '0';
    	disp_buf[9] = 0;
	#elif defined(_SH1330_SPECIAL_DISPLAY_)
		if(tune < 1000)
		{
			disp_buf[0] = 'T';
    		disp_buf[1] =  tune/100 	+ '0';
    		disp_buf[2] = (tune%100)/10 + '0';
    		disp_buf[3] = (tune%100)%10 + '0';
		}
		else
		{
			disp_buf[0] = (tune/100)/10 + '0';
			disp_buf[1] = (tune/100)%10 + '0';
			disp_buf[2] = (tune%100)/10 + '0';
			disp_buf[3] = (tune%100)%10 + '0';
		}
    	disp_buf[4] = ' ';
		disp_buf[5] = ' ';
    	disp_buf[6] = min/10 + '0';
    	disp_buf[7] = min%10 + '0';
    	disp_buf[8] = sec/10 + '0';
    	disp_buf[9] = sec%10 + '0';
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
		if(compare_task_name("MusicTaskUsb"))
		{
			disp_buf[0] = 'U';
			disp_buf[1] = 'S';
			disp_buf[2] = 'B';
		}
		else if(compare_task_name("MusicTaskCard"))
		{
			disp_buf[0] = ' ';
			disp_buf[1] = 'S';
			disp_buf[2] = 'D';
		}
		disp_buf[3] = '-';
		disp_buf[4] =  tune/100 	+ '0';
		disp_buf[5] = (tune%100)/10 + '0';
		disp_buf[6] = (tune%100)%10 + '0';
    	disp_buf[7] = min/10 + '0';
    	disp_buf[8] = min%10 + '0';
    	disp_buf[9] = sec/10 + '0';
    	disp_buf[10] = sec%10 + '0';
	#elif defined(_HH_SPECIAL_DISPLAY_)
	    if(tune < 1000)
    	{
    		disp_buf[0] =  tune/100 	+ '0';
    		disp_buf[1] = (tune%100)/10 + '0';
    		disp_buf[2] = (tune%100)%10 + '0';
    	}
    	else
    	{
    		disp_buf[0] = (tune/100)%10 + '0';
    		disp_buf[1] = (tune%100)/10 + '0';
    		disp_buf[2] = (tune%100)%10 + '0';
    	}
        disp_buf[3] = '-';
    	disp_buf[4] = min/10 + '0';
    	disp_buf[5] = min%10 + '0';
    	disp_buf[6] = sec/10 + '0';
    	disp_buf[7] = sec%10 + '0';
		disp_buf[8] = ' ';
	#elif defined(_FIVE_CHAR_DISPLAY_)
	#if defined(_MR889_SPECAIL_DISP_)
    	if(tune < 1000)
    	{
    	}
    	else
    	{
    		disp_buf[0] = (tune/100)%10 + '0';
    		disp_buf[1] = (tune%100)/10 + '0';
    		disp_buf[2] = (tune%100)%10 + '0';
    	}

    	disp_buf[3] = '-';
    	disp_buf[4] = min%10 + '0';

		disp_buf[5] = sec/10 + '0';
    	disp_buf[6] = sec%10 + '0';

    	disp_buf[7] = ' ';
	#else
    	if(tune < 1000)
    	{
    		disp_buf[0] =  tune/100 	+ '0';
    		disp_buf[1] = (tune%100)/10 + '0';
    		disp_buf[2] = (tune%100)%10 + '0';
    	}
    	else
    	{
    		disp_buf[0] = (tune/100)%10 + '0';
    		disp_buf[1] = (tune%100)/10 + '0';
    		disp_buf[2] = (tune%100)%10 + '0';
    	}

    	disp_buf[3] = min%10 + '0';

    	disp_buf[4] = sec/10 + '0';
    	disp_buf[5] = sec%10 + '0';

    	disp_buf[6] = ' ';
    	disp_buf[7] = ' ';
	#endif
	#elif defined(_SEVEN_CHAR_DISPLAY_)
		disp_buf[0] = (tune/100)%10 + '0';
		disp_buf[1] = (tune%100)/10 + '0';
		disp_buf[2] = (tune%100)%10 + '0';
		if(min < 10)
		{
			disp_buf[3] = ' ';
			disp_buf[4] = min%10 + '0';
		}
		else
		{
			disp_buf[3] = min/10 + '0';
			disp_buf[4] = min%10 + '0';
		}
		disp_buf[5] = sec/10 + '0';
		disp_buf[6] = sec%10 + '0';
	#elif defined(_YS_SPECIAL_DISP_)
    	if(tune < 100)
    	{
    		disp_buf[0] = 'T';
			disp_buf[1] = (tune%100)/10 + '0';
			disp_buf[2] = (tune%100)%10 + '0';
        	disp_buf[3] = '-';

			disp_buf[4] = min/10 + '0';
			disp_buf[5] = min%10 + '0';

			disp_buf[6] = sec/10 + '0';
			disp_buf[7] = sec%10 + '0';

			disp_buf[8] = 0;
    	}
    	else
    	{
    		disp_buf[0] = (tune/100)/10 + '0';
			disp_buf[1] = (tune/100)%10 + '0';
			disp_buf[2] = (tune%100)/10 + '0';
        	disp_buf[3] = '-';

    		disp_buf[4] = min/10 + '0';
    		disp_buf[5] = min%10 + '0';

			disp_buf[6] = sec/10 + '0';
			disp_buf[7] = sec%10 + '0';
			disp_buf[8] = 0;
    	}
	#elif defined(_MR1901_SPECIAL_DISPLAY_)
    	disp_buf[0] = (tune/100)/10 + '0';
    	disp_buf[1] = (tune/100)%10 + '0';
    	disp_buf[2] = (tune%100)/10 + '0';
    	disp_buf[3] = (tune%100)%10 + '0';

		disp_buf[4] = min/10 + '0';
		disp_buf[5] = min%10 + '0';
    	disp_buf[6] = sec/10 + '0';
    	disp_buf[7] = sec%10 + '0';
    	disp_buf[8] = 0;
	#else
    	if(tune < 1000)
    	{
    		disp_buf[0] =  tune/100 	+ '0';
			disp_buf[1] = (tune%100)/10 + '0';
			disp_buf[2] = (tune%100)%10 + '0';
        	disp_buf[3] = '-';

			disp_buf[4] = min/10 + '0';
			disp_buf[5] = min%10 + '0';

			disp_buf[6] = sec/10 + '0';
			disp_buf[7] = sec%10 + '0';

			disp_buf[8] = 0;
    	}
    	else
    	{
    		disp_buf[0] = (tune/100)/10 + '0';
			disp_buf[1] = (tune/100)%10 + '0';
			disp_buf[2] = (tune%100)/10 + '0';
			disp_buf[3] = (tune%100)%10 + '0';

	    	if(min < 10)
	    	{
	    		disp_buf[4] = '-';
	    		disp_buf[5] = min%10 + '0';
	    	}
	    	else
	    	{
	    		disp_buf[4] = min/10 + '0';
	    		disp_buf[5] = min%10 + '0';
	    	}
			disp_buf[6] = sec/10 + '0';
			disp_buf[7] = sec%10 + '0';
			disp_buf[8] = 0;
    	}
	#endif
    #if defined(_ENABLE_CLOCK_DISPLAY_)
    	ui_disp.playflg = TRUE;
    #endif
		VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,VFD_DISP_TIME_CLOCK|0);
	}
}

#if defined(_YS_SPECIAL_DISP_)
static void PlayerDispPlayModeTime(void)
{
    uint8 disp_buf[10];

	uint16 tune = ID3_CTRL_INFO.tune%10000;
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);

	//if(tune != ID3_CTRL_DISP.Tune)
	{
		ID3_CTRL_DISP.Tune 	= tune;
		if(play_disp->info.mode == PLAY_MODE_INTRO)
		{
			disp_buf[0] = 'S';
			disp_buf[1] = 'C';
			disp_buf[2] = 'N';
		}
		else if(play_disp->info.mode == PLAY_MODE_REPEAT_ONE)
		{
			disp_buf[0] = 'R';
			disp_buf[1] = 'P';
			disp_buf[2] = 'T';
		}
		else if(play_disp->info.mode == PLAY_MODE_SHUFFLE)
		{
			disp_buf[0] = 'R';
			disp_buf[1] = 'D';
			disp_buf[2] = 'M';
		}
		disp_buf[3] = '-';
		disp_buf[4] = 'T';
		disp_buf[5] = ((tune/100)%10)+'0';
		disp_buf[6] = ((tune/10)%10)+'0';
		disp_buf[7] = (tune%10)+'0';
	}
	VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,0);
}
#endif


static void play_error_disp(uint8 error)
{
	switch(error)
	{
		case PLAY_ERROR_DRIVER:
			switch(ID3_ID_SOURCE)
			{
				case PLAY_SOURCE_USB:
				#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
					VdfDispString("err ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("   No  Usb   ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("    No Usb",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("  No  Usb  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString("  No  Usb ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FIVE_CHAR_DISPLAY_)
				#if defined(SPECIAL_SLA6799_DISP)
				    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
				#else
				    VdfDispString("NoUsb   ",VFD_DISP_TYPE_TIME,0);
				#endif
				#elif defined(_XM003_SPECIAL_DISPLAY_)
					VdfDispString("No Usb  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_HKY1032_SPECIAL_DISPLAY_)
					VdfDispString("     No ",VFD_DISP_TYPE_TIME,0);
				#else
					VdfDispString("No Usb",VFD_DISP_TYPE_TIME,0);
				#endif
					break;
				case PLAY_SOURCE_SD:
				#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
			    	VdfDispString("err ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("   No  Sd    ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("    No Sd ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString("  No  Sd  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("  No  Sd   ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XM003_SPECIAL_DISPLAY_)
					VdfDispString("No  Sd  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FIVE_CHAR_DISPLAY_)
				#if defined(SPECIAL_SLA6799_DISP)
				    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
				#else
				    VdfDispString("No Sd   ",VFD_DISP_TYPE_TIME,0);
				#endif
				#elif defined(_HKY1032_SPECIAL_DISPLAY_)
					VdfDispString("     No ",VFD_DISP_TYPE_TIME,0);
				#else
			    	VdfDispString("No Sd",VFD_DISP_TYPE_TIME,0);
				#endif
					break;
				default:
					break;
			}
			break;
		case PLAY_ERROR_INIT:
			switch(ID3_ID_SOURCE)
			{
				case PLAY_SOURCE_USB:
				#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
					VdfDispString("err ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("   No  Usb   ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("  No  Usb  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("    No Usb",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XM003_SPECIAL_DISPLAY_)
					VdfDispString("No Usb  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString("  No  Usb ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FIVE_CHAR_DISPLAY_)
				#if defined(SPECIAL_SLA6799_DISP)
				    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
				#else
				    VdfDispString("NoUsb   ",VFD_DISP_TYPE_TIME,0);
				#endif
				#elif defined(_HKY1032_SPECIAL_DISPLAY_)
					VdfDispString("     No ",VFD_DISP_TYPE_TIME,0);
				#else
					VdfDispString("No Usb",VFD_DISP_TYPE_TIME,0);
				#endif
					break;
				case PLAY_SOURCE_SD:
				#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
			    	VdfDispString("err ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("   No  Sd    ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("    No Sd ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("  No  Sd   ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XM003_SPECIAL_DISPLAY_)
					VdfDispString("No  Sd  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FIVE_CHAR_DISPLAY_)
				#if defined(SPECIAL_SLA6799_DISP)
				    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
				#else
				    VdfDispString("No Sd   ",VFD_DISP_TYPE_TIME,0);
				#endif
				#elif defined(_HKY1032_SPECIAL_DISPLAY_)
					VdfDispString("     No ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString("  No  Sd  ",VFD_DISP_TYPE_TIME,0);
				#else
			    	VdfDispString("No Sd",VFD_DISP_TYPE_TIME,0);
				#endif
					break;
				default:
					break;
			}
			break;
			case PLAY_ERROR_NO_FILE:
			#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
		    	VdfDispString("err ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY7000_SPECIAL_DISPLAY_)
				VdfDispString("  No  File   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				VdfDispString(" No  File  ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				VdfDispString("    NoFile",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XM003_SPECIAL_DISPLAY_)
				VdfDispString("No F1le  ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FIVE_CHAR_DISPLAY_)
			#if defined(SPECIAL_SLA6799_DISP)
			    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#else
				VdfDispString("NoFile  ",VFD_DISP_TYPE_TIME,0);
			#endif
			#elif defined(_HKY1032_SPECIAL_DISPLAY_)
				VdfDispString(" 	No ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				VdfDispString(" No	File ",VFD_DISP_TYPE_TIME,0);
			#else
		    	VdfDispString("No File",VFD_DISP_TYPE_TIME,0);
			#endif
				break;
			case PLAY_ERROR_FILE:
			#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
		    	VdfDispString("err ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY7000_SPECIAL_DISPLAY_)
				VdfDispString("  Err  File  ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				VdfDispString("    Error ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XM003_SPECIAL_DISPLAY_)
				//VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FIVE_CHAR_DISPLAY_)
			    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				VdfDispString("Error File",VFD_DISP_TYPE_TIME,0);
			#elif defined(_HKY1032_SPECIAL_DISPLAY_)
				VdfDispString("     ERR",VFD_DISP_TYPE_TIME,0);
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				VdfDispString(" Err File ",VFD_DISP_TYPE_TIME,0);
			#else
		    	VdfDispString("Err File",VFD_DISP_TYPE_TIME,0);
			#endif
				break;
			case PLAY_ERROR_PLAYER:
			#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
		    	VdfDispString("err ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY7000_SPECIAL_DISPLAY_)
				VdfDispString("  Err  Play  ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				VdfDispString("    Error ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				VdfDispString(" Err  Play ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XM003_SPECIAL_DISPLAY_)
				//VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FIVE_CHAR_DISPLAY_)
			    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_MR1901_SPECIAL_DISPLAY_)
				VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_HKY1032_SPECIAL_DISPLAY_)
				VdfDispString("     ERR",VFD_DISP_TYPE_TIME,0);
			#elif defined(_HKY1032_SPECIAL_DISPLAY_)
				VdfDispString("     ERR",VFD_DISP_TYPE_TIME,0);
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				VdfDispString(" Err Play ",VFD_DISP_TYPE_TIME,0);
			#else
		    	VdfDispString("Err Play",VFD_DISP_TYPE_TIME,0);
			#endif
				break;
			case PLAY_ERROR_FW:
			#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
		    	VdfDispString("err ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY7000_SPECIAL_DISPLAY_)
				VdfDispString("   Err  Fw   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				VdfDispString("  Err  Fw  ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				VdfDispString("    Error ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XM003_SPECIAL_DISPLAY_)
				//VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FIVE_CHAR_DISPLAY_)
			    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_HKY1032_SPECIAL_DISPLAY_)
				VdfDispString("     ERR",VFD_DISP_TYPE_TIME,0);
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				VdfDispString(" Err  Fw  ",VFD_DISP_TYPE_TIME,0);
			#else
		    	VdfDispString("Err Fw",VFD_DISP_TYPE_TIME,0);
			#endif
				break;
			case PLAY_ERROR_OTHER:
			#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
		    	VdfDispString("err",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY7000_SPECIAL_DISPLAY_)
				VdfDispString("    Error    ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				VdfDispString("    Error ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				VdfDispString("   Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XM003_SPECIAL_DISPLAY_)
				//VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FIVE_CHAR_DISPLAY_)
			    VdfDispString("Error   ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_HKY1032_SPECIAL_DISPLAY_)
				VdfDispString("     ERR",VFD_DISP_TYPE_TIME,0);
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				VdfDispString("  Error   ",VFD_DISP_TYPE_TIME,0);
			#else
		    	VdfDispString("Error",VFD_DISP_TYPE_TIME,0);
			#endif
				break;
			default:
				break;
	}
}

static uint8 play_msg_disp(uint8 *msg,uint16 time)
{
	int Length;

	if(ID3_ID != ID3_STATUS_DISP)return FALSE;
	if(ID3_CTRL_INFO.state != PLAY_STATUS_PLAYE)return FALSE;

	Length = strlen((char *)msg);
	if(!Length)return FALSE;

	ID3_CTRL_DISP.MsgStatus = ID3_CTRL_DISP.Status;
	if(Length > sizeof(ID3_CTRL_DISP.Msg)-1)Length = sizeof(ID3_CTRL_DISP.Msg)-1;
	memset(ID3_CTRL_DISP.Msg, 0, sizeof(ID3_CTRL_DISP.Msg));
	memcpy(ID3_CTRL_DISP.Msg, msg, Length);
	ID3_CTRL_DISP.MsgTime = time;
	ID3_CTRL_DISP.MsgChange = TRUE;
	ID3_CTRL_DISP.Status = ID3_DISP_STATUS_MSG;

	return TRUE;
}


static void play_dir_disp(void)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	char disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	char disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	char disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	char disp_buf[12];
#else
	char disp_buf[10];
#endif
	uint16 tune = music_dir_number%10000;
#if defined(TD_ADD_LCD_NUM_4)
	disp_buf[0] = 'D';
	disp_buf[1] = (tune/100)%10 + '0';
	disp_buf[2] = (tune%100)/10 + '0';
	disp_buf[3] = (tune%100)%10 + '0';
#elif defined(_LCD_3_DIAPLAY_)
	disp_buf[0] = ' ';
	disp_buf[1] = (tune/100)%10 + '0';
	disp_buf[2] = (tune%100)/10 + '0';
	disp_buf[3] = (tune%100)%10 + '0';
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    disp_buf[0] = ' ';
    disp_buf[1] = ' ';
    disp_buf[2] = ' ';
    disp_buf[3] = 'D';
    disp_buf[4] = 'I';
    disp_buf[5] = 'R';
    disp_buf[6] = ' ';
    disp_buf[7] = ' ';
    disp_buf[8] = (tune/100)%10 + '0';
    disp_buf[9] = (tune%100)/10 + '0';
    disp_buf[10] = (tune%100)%10 + '0';
    disp_buf[11] = ' ';
    disp_buf[12] = ' ';
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    disp_buf[0] = ' ';
    disp_buf[1] = ' ';
    disp_buf[2] = ' ';
    disp_buf[3] = ' ';
    disp_buf[4] = 'D';
    disp_buf[5] = 'I';
    disp_buf[6] = 'R';
    disp_buf[7] = (tune/100)%10 + '0';
    disp_buf[8] = (tune%100)/10 + '0';
    disp_buf[9] = (tune%100)%10 + '0';
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint16 tune1 = ID3_CTRL_INFO.tune%10000;
    disp_buf[0] = 'F';
    disp_buf[1] = (tune/100)%10 + '0';
    disp_buf[2] = (tune%100)/10 + '0';
    disp_buf[3] = (tune%100)%10 + '0';
    disp_buf[4] = '-';
    disp_buf[5] = 'T';
	if(tune < 1000)
	{
		disp_buf[6] =  tune1/100 	+ '0';
		disp_buf[7] = (tune1%100)/10 + '0';
		disp_buf[8] = (tune1%100)%10 + '0';
		disp_buf[9] = ' ';
	}
	else
	{
		disp_buf[6] = (tune1/100)/10 + '0';
		disp_buf[7] = (tune1/100)%10 + '0';
		disp_buf[8] = (tune1%100)/10 + '0';
		disp_buf[9] = (tune1%100)%10 + '0';
	}
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    disp_buf[0] = ' ';
    disp_buf[1] = 'D';
    disp_buf[2] = 'I';
    disp_buf[3] = 'R';
    disp_buf[4] = ' ';
    disp_buf[5] = ' ';
    disp_buf[6] = (tune/100)%10 + '0';
    disp_buf[7] = (tune%100)/10 + '0';
    disp_buf[8] = (tune%100)%10 + '0';
    disp_buf[9] = ' ';
    disp_buf[10] = ' ';
#elif defined(_FIVE_CHAR_DISPLAY_)
    disp_buf[0] = 'D';
    disp_buf[1] = 'I';
    disp_buf[2] = 'R';

    disp_buf[3] = (tune/100)%10 + '0';
    disp_buf[4] = (tune%100)/10 + '0';
    disp_buf[5] = (tune%100)%10 + '0';
    disp_buf[6] = ' ';
#else
#if defined(SPECIAL_JD_DISP)
	disp_buf[0] = 'D';
	disp_buf[1] = 'I';
	disp_buf[2] = 'R';
	disp_buf[3] = (tune/100)%10 + '0';
	disp_buf[4] = (tune%100)/10 + '0';
	disp_buf[5] = (tune%100)%10 + '0';
	disp_buf[6] = ' ';
	disp_buf[7] = ' ';
#else
	disp_buf[0] = 'D';
	disp_buf[1] = 'I';
	disp_buf[2] = 'R';
	disp_buf[3] = '-';
	disp_buf[4] = (tune/100)%10 + '0';
	disp_buf[5] = (tune%100)/10 + '0';
	disp_buf[6] = (tune%100)%10 + '0';
	disp_buf[7] = ' ';
#endif
#endif
	play_msg_disp((uint8 *)disp_buf, 1000);
}

#if defined(_SL6812A_SPECIAL_DISPLAY_)
static void play_trk10_disp(uint8 up_down)
{
	char disp_buf[12];
    disp_buf[0] = ' ';
    disp_buf[1] = '1';
    disp_buf[2] = '0';
    disp_buf[3] = 'T';
    disp_buf[4] = 'R';
    disp_buf[5] = 'K';
    disp_buf[6] = ' ';
    if(up_down)
    {
		disp_buf[7] = '>';
		disp_buf[8] = '>';
	}
	else
	{
		disp_buf[7] = '<';
		disp_buf[8] = '<';
	}
    disp_buf[9] = ' ';
	play_msg_disp((uint8 *)disp_buf, 1000);
}
#endif
static void play_media_disp(void)
{
#if !defined(TD_ADD_LCD_NUM_4)
#if !defined(_WLS316_SPECIAL_DISPLAY_)
#if !defined(SPECIAL_SLA6799_DISP)
#if !defined(_ENABLE_PLAY_DISPLAY_)
	static const char *media_type_tab[][2]=
	{
	#if defined(_FY7000_SPECIAL_DISPLAY_)
    	{	" MP3 ",	"  MP3  "	},
    	{	" WMA ",	"  WMA  "	},
    	{	" WAV ",	"  WAV  "	},
    	{	"FLAC ",	" FLAC  "	},
    	{	" APE ",	"  APE  "	},
    	{	" --- ",	"  ---  "	},
	#elif defined(_MR1901_SPECIAL_DISPLAY_)
    	{	"MP3 ",		"MP3 "	},
    	{	"WMA ",		"WMA "	},
    	{	"WAV ",		"WAV "	},
    	{	"FLA ",		"FLA "	},
    	{	"APE ",		"APE "	},
    	{	"--- ",		"--- "	},
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
    	{	" MP3 ",	"  MP3	"	},
    	{	" WMA ",	"  WMA	"	},
    	{	" WAV ",	"  WAV	"	},
    	{	" FLA ",	"  FLA	"	},
    	{	" APE ",	"  APE	"	},
    	{	" --- ",	"  ---	"	},
	#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    	{	" MP3 ",	" MP3 "	},
    	{	" WMA ",	" WMA "	},
    	{	" WAV ",	" WAV "	},
    	{	" FLA ",	" FLA "	},
    	{	" APE ",	" APE "	},
    	{	" --- ",	" --- "	},
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    	{	"    MP3", 	"    MP3"	},
    	{	"    WMA", 	"    WMA"	},
    	{	"    WAU", 	"    WAU"	},
    	{	"    FLA", 	"    FLA"	},
    	{	"    APE", 	"    APE"	},
    	{	"    ---", 	"    ---"	},
	#elif defined(_XM003_SPECIAL_DISPLAY_)
    	{	"MP3 ",		"MP3 "	},
    	{	"WMA ",		"WMA "	},
    	{	"WAU ",		"WAU "	},
    	{	"FLA ",		"FLA "	},
    	{	"APE ",		"APE "	},
    	{	"--- ",		"--- "	},
	#elif defined(_FIVE_CHAR_DISPLAY_)
	#if defined(SPECIAL_JD_DISP)
		{	"TN",		"TN"	},
		{	"TN",		"TN"	},
		{	"TN",		"TN"	},
		{	"TN",		"TN"	},
		{	"TN",		"TN"	},
		{	"---",		"---"	},
	#else
		{	"MP3",		"MP3"	},
		{	"WMA",		"WMA"	},
		{	"WAV",		"WAV"	},
		{	"FLA",		"FLA"	},
		{	"APE",		"APE"	},
		{	"---",		"---"	},
	#endif
	#elif defined(_SEVEN_CHAR_DISPLAY_)&&defined(_PS329a_SPECIAL_DISPLAY_)
	    {	"MP3",		"MP3"	},
		{	"WMA",		"WMA"	},
		{	"WAV",		"WAV"	},
		{	"FLA",		"FLA"	},
		{	"APE",		"APE"	},
		{	"---",		"---"	},
	#elif defined(_SEVEN_CHAR_DISPLAY_)
		{	"MP3 ",		"MP3 "	},
		{	"WMA ",		"WMA "	},
		{	"WAV ",		"WAV "	},
		{	"FLA ",		"FLA "	},
		{	"APE ",		"APE "	},
		{	"--- ",		"--- "	},
	#else
	#if defined(SPECIAL_JD_DISP)
		{	"TN",	"TN"	},
		{	"TN",	"TN"	},
		{	"TN",	"TN"	},
		{	"TN",	"TN"	},
		{	"TN",	"TN"	},
		{	"--",	"--"	},
	#else
		{	" MP3 ",	"MP3 "	},
		{	" WMA ",	"WMA "	},
		{	" WAV ",	"WAV "	},
		{	" FLA ",	"FLA "	},
		{	" APE ",	"APE "	},
		{	" --- ",	"--- "	},
	#endif
	#endif
	};
#endif
#endif
#endif
#endif

#if defined(_FY7000_SPECIAL_DISPLAY_)
	char disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	char disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	char disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	char disp_buf[12];
#else
	char disp_buf[10];
#endif
	uint16 tune = ID3_CTRL_INFO.tune%10000;

	play_media_icon_disp(ID3_CTRL_INFO.media);
#if defined(_LCD_3_DIAPLAY_)//显示当前曲目
	disp_buf[0] = ' ';
	disp_buf[1] = (tune/100)%10 + '0';
	disp_buf[2] = (tune%100)/10 + '0';
	disp_buf[3] = (tune%100)%10 + '0';
#elif defined(TD_ADD_LCD_NUM_4)
#if defined(_WLS1403_COPY_DISPLAY_)
	if(tune < 1000)
	{
		disp_buf[0] = ' ';
		disp_buf[1] = tune/100 + '0';
		disp_buf[2] = (tune%100)/10 + '0';
		disp_buf[3] = (tune%100)%10 + '0';
	}
	else
	{
		disp_buf[0] = (tune/100)/10 + '0';
		disp_buf[1] = (tune/100)%10 + '0';
		disp_buf[2] = (tune%100)/10 + '0';
		disp_buf[3] = (tune%100)%10 + '0';
	}
#else
	disp_buf[0] = (tune/100)/10 + '0';
	disp_buf[1] = (tune/100)%10 + '0';
	disp_buf[2] = (tune%100)/10 + '0';
	disp_buf[3] = (tune%100)%10 + '0';
#endif
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    strcpy(disp_buf, media_type_tab[ID3_CTRL_INFO.media][1]);
    uint8 length = strlen(disp_buf);

    disp_buf[length++] = (tune/100)/10 + '0';
    disp_buf[length++] = (tune/100)%10 + '0';
    disp_buf[length++] = (tune%100)/10 + '0';
    disp_buf[length++] = (tune%100)%10 + '0';
    disp_buf[length++] = ' ';
    disp_buf[length++] = ' ';
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	if(global.total_flag)
	{
		global.total_flag = FALSE;
		disp_buf[0] = 'F';
		disp_buf[1] = 'I';
		disp_buf[2] = 'L';
		disp_buf[3] = 'E';
		disp_buf[4] = ' ';
		disp_buf[5] = ' ';
		if(tune < 1000)
		{
			disp_buf[6] = ' ';
			disp_buf[7] =  tune/100 + '0';
			disp_buf[8] = (tune%100)/10 + '0';
			disp_buf[9] = (tune%100)%10 + '0';
		}
		else
		{
			disp_buf[6] = (tune/100)/10 + '0';
			disp_buf[7] = (tune/100)%10 + '0';
			disp_buf[8] = (tune%100)/10 + '0';
			disp_buf[9] = (tune%100)%10 + '0';
		}
	}
	else
	{
		uint8 min = ID3_CTRL_INFO.min%100;
		uint8 sec = ID3_CTRL_INFO.sec%100;
		if(tune < 1000)
		{
			disp_buf[0] = 'T';
    		disp_buf[1] =  tune/100 	+ '0';
    		disp_buf[2] = (tune%100)/10 + '0';
    		disp_buf[3] = (tune%100)%10 + '0';
			disp_buf[4] = ' ';
		}
		else
		{
			disp_buf[0] = 'T';
			disp_buf[1] = (tune/100)/10 + '0';
			disp_buf[2] = (tune/100)%10 + '0';
			disp_buf[3] = (tune%100)/10 + '0';
			disp_buf[4] = (tune%100)%10 + '0';
		}
		disp_buf[5] = ' ';
		disp_buf[6] = min/10 + '0';
		disp_buf[7] = min%10 + '0';
		disp_buf[8] = sec/10 + '0';
		disp_buf[9] = sec%10 + '0';
	}
#elif defined(_MR1901_SPECIAL_DISPLAY_)
    strcpy(disp_buf, media_type_tab[ID3_CTRL_INFO.media][1]);
    uint8 length = strlen(disp_buf);

    disp_buf[length++] = (tune/100)/10 + '0';
    disp_buf[length++] = (tune/100)%10 + '0';
    disp_buf[length++] = (tune%100)/10 + '0';
    disp_buf[length++] = (tune%100)%10 + '0';
#elif defined(_XZC1668_SPECIAL_DISPLAY_)
    if(tune < 1000)
    {
    	disp_buf[0] = 'U';
    	disp_buf[1] =  tune/100	+ '0';
    	disp_buf[2] = (tune%100)/10 + '0';
    	disp_buf[3] = (tune%100)%10 + '0';
    }
    else
    {
    	disp_buf[0] = (tune/100)/10 + '0';
    	disp_buf[1] = (tune/100)%10 + '0';
    	disp_buf[2] = (tune%100)/10 + '0';
    	disp_buf[3] = (tune%100)%10 + '0';
    }
    disp_buf[4] = ' ';
    disp_buf[5] = '0';
    disp_buf[6] = '0';
    disp_buf[7] = '0';
    disp_buf[8] = '0';
    disp_buf[9] = ' ';
#elif defined(_ENABLE_PLAY_DISPLAY_)
	uint8 min = ID3_CTRL_INFO.min%100;
	uint8 sec = ID3_CTRL_INFO.sec%100;
    if(tune < 1000)
    {
    	disp_buf[0] =  tune/100 + '0';
    	disp_buf[1] = (tune%100)/10 + '0';
    	disp_buf[2] = (tune%100)%10 + '0';
    	disp_buf[3] = '-';
    	disp_buf[4] = min/10 + '0';
    	disp_buf[5] = min%10 + '0';
    
    	disp_buf[6] = sec/10 + '0';
    	disp_buf[7] = sec%10 + '0';
    }
    else
    {
    	disp_buf[0] = (tune/100)/10 + '0';
    	disp_buf[1] = (tune/100)%10 + '0';
    	disp_buf[2] = (tune%100)/10 + '0';
    	disp_buf[3] = (tune%100)%10 + '0';
    	if(min < 10)
    	{
    		disp_buf[4] = '-';
    		disp_buf[5] = min%10 + '0';
    	}
    	else
    	{
    		disp_buf[4] = min/10 + '0';
    		disp_buf[5] = min%10 + '0';
    	}
    	disp_buf[6] = sec/10 + '0';
    	disp_buf[7] = sec%10 + '0';
    }
#elif defined(_SH1330_SPECIAL_DISPLAY_)
    if(tune < 1000)
    {
    	disp_buf[0] = 'U';
    	disp_buf[1] =  tune/100	+ '0';
    	disp_buf[2] = (tune%100)/10 + '0';
    	disp_buf[3] = (tune%100)%10 + '0';
    }
    else
    {
    	disp_buf[0] = (tune/100)/10 + '0';
    	disp_buf[1] = (tune/100)%10 + '0';
    	disp_buf[2] = (tune%100)/10 + '0';
    	disp_buf[3] = (tune%100)%10 + '0';
    }
    disp_buf[4] = ' ';
    disp_buf[5] = ' ';
    disp_buf[6] = '0';
    disp_buf[7] = '0';
    disp_buf[8] = '0';
    disp_buf[9] = '0';		
#elif defined(_WLS316_SPECIAL_DISPLAY_)
	uint8 min = ID3_CTRL_INFO.min%100;
	uint8 sec = ID3_CTRL_INFO.sec%100;
    if(tune < 1000)
    {
    	disp_buf[0] =  tune/100 	+ '0';
    	disp_buf[1] = (tune%100)/10 + '0';
    	disp_buf[2] = (tune%100)%10 + '0';
    	disp_buf[3] = '-';
    
    	disp_buf[4] = min/10 + '0';
    	disp_buf[5] = min%10 + '0';
    
    	disp_buf[6] = sec/10 + '0';
    	disp_buf[7] = sec%10 + '0';
    
    	disp_buf[8] = 0;
    }
    else
    {
    	disp_buf[0] = (tune/100)/10 + '0';
    	disp_buf[1] = (tune/100)%10 + '0';
    	disp_buf[2] = (tune%100)/10 + '0';
    	disp_buf[3] = (tune%100)%10 + '0';
    
    	if(min < 10)
    	{
    		disp_buf[4] = '-';
    		disp_buf[5] = min%10 + '0';
    	}
    	else
    	{
    		disp_buf[4] = min/10 + '0';
    		disp_buf[5] = min%10 + '0';
    	}
    	disp_buf[6] = sec/10 + '0';
    	disp_buf[7] = sec%10 + '0';
    	disp_buf[8] = 0;
    }
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	if(tune < 1000)
	{
		strcpy(disp_buf, media_type_tab[ID3_CTRL_INFO.media][1]);
		uint8 length = strlen(disp_buf);
		disp_buf[length++] =  tune/100 	   + '0';
		disp_buf[length++] = (tune%100)/10 + '0';
		disp_buf[length++] = (tune%100)%10 + '0';
		disp_buf[length++] = ' ';
		disp_buf[length++] = 0;
	}
	else
	{
		strcpy(disp_buf, media_type_tab[ID3_CTRL_INFO.media][0]);
		uint8 length = strlen(disp_buf);
		disp_buf[length++] = ' ';
		disp_buf[length++] = (tune/100)/10 + '0';
		disp_buf[length++] = (tune/100)%10 + '0';
		disp_buf[length++] = (tune%100)/10 + '0';
		disp_buf[length++] = (tune%100)%10 + '0';
		disp_buf[length++] = ' ';
		disp_buf[length++] = 0;
	}
#elif defined(SPECIAL_SLA6799_DISP)
	uint8 min = ID3_CTRL_INFO.min%100;
	uint8 sec = ID3_CTRL_INFO.sec%100;
    if(tune < 1000)
    {
    	disp_buf[0] =  tune/100 	+ '0';
    	disp_buf[1] = (tune%100)/10 + '0';
    	disp_buf[2] = (tune%100)%10 + '0';
    
    	disp_buf[3] = min%10 + '0';

    	disp_buf[4] = sec/10 + '0';
    	disp_buf[5] = sec%10 + '0';

    	disp_buf[6] = ' ';
    	disp_buf[7] = ' ';
    }
    else
    {
    	disp_buf[0] =  tune/100 	+ '0';
    	disp_buf[1] = (tune%100)/10 + '0';
    	disp_buf[2] = (tune%100)%10 + '0';
    
    	disp_buf[3] = min%10 + '0';

    	disp_buf[4] = sec/10 + '0';
    	disp_buf[5] = sec%10 + '0';

    	disp_buf[6] = ' ';
    	disp_buf[7] = ' ';
    }
	ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_CLOCK);
#else
	if(tune < 1000)
	{
		strcpy(disp_buf, media_type_tab[ID3_CTRL_INFO.media][0]);
		uint8 length = strlen(disp_buf);

	#if defined(SPECIAL_JD_DISP)
		disp_buf[length++] =  '0';
		disp_buf[length++] =  tune/100 	   + '0';
		disp_buf[length++] = (tune%100)/10 + '0';
		disp_buf[length++] = (tune%100)%10 + '0';
		disp_buf[length++] =  ' ';
		disp_buf[length++] =  ' ';
	#else
	#if defined(_PS329a_SPECIAL_DISPLAY_)
	    disp_buf[length++] = ' ';
	    disp_buf[length++] = (tune/100)%10 + '0';
	    disp_buf[length++] = (tune%100)/10 + '0';
	    disp_buf[length++] = (tune%100)%10 + '0';
	#else
		disp_buf[length++] =  tune/100 	   + '0';
		disp_buf[length++] = (tune%100)/10 + '0';
		disp_buf[length++] = (tune%100)%10 + '0';
	#endif
	#endif
	}
	else
	{
		strcpy(disp_buf, media_type_tab[ID3_CTRL_INFO.media][1]);
		uint8 length = strlen(disp_buf);

	#if defined(_FIVE_CHAR_DISPLAY_)||defined(_XM003_SPECIAL_DISPLAY_)
	#if defined(SPECIAL_JD_DISP)
		disp_buf[length++] = (tune/100)/10 + '0';
		disp_buf[length++] = (tune/100)%10 + '0';
		disp_buf[length++] = (tune%100)/10 + '0';
		disp_buf[length++] = (tune%100)%10 + '0';
	#elif defined(_MR889_SPECAIL_DISP_)
		disp_buf[length++] = (tune/100)/10 + '0';
		disp_buf[length++] = (tune/100)%10 + '0';
		disp_buf[length++] = (tune%100)/10 + '0';
		disp_buf[length++] = (tune%100)%10 + '0';
	#else
		disp_buf[length++] = (tune/100)%10 + '0';
		disp_buf[length++] = (tune%100)/10 + '0';
		disp_buf[length++] = (tune%100)%10 + '0';
	#endif
	#else

		disp_buf[length++] = (tune/100)/10 + '0';
		disp_buf[length++] = (tune/100)%10 + '0';
		disp_buf[length++] = (tune%100)/10 + '0';
		disp_buf[length++] = (tune%100)%10 + '0';
	#if defined(SPECIAL_JD_DISP)
		disp_buf[length++] =  ' ';
		disp_buf[length++] =  ' ';
	#endif
	#endif
	}
#endif
#if defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_WLS316_SPECIAL_DISPLAY_)||defined(_ENABLE_PLAY_DISPLAY_)
	global.song_state = TRUE;
	play_msg_disp((uint8 *)disp_buf, 500);
#elif defined(_SL6812A_SPECIAL_DISPLAY_)||defined(SPECIAL_SLA6799_DISP)
    global.song_state = TRUE;
    play_msg_disp((uint8 *)disp_buf, 1000);
#else
	play_msg_disp((uint8 *)disp_buf, 1000);
#endif
}

#if defined(_SUPPORT_ID3_)

#if defined(_SUPPORT_CHAR_RUSSIAN_)

#define CHAR_RUSSIAN_LENGTH					(33)
#define CHAR_RUSSIAN_START					(0xC0+0x00)//自编码映射西里尔字母到AscII区域
#define CHAR_RUSSIAN_ESPECIALLY				(CHAR_RUSSIAN_START+ CHAR_RUSSIAN_LENGTH - 1)
#define CHAR_RUSSIAN_END					(CHAR_RUSSIAN_ESPECIALLY)

#endif

static uint8 play_Id3_disp_support(uint8 c)
{
static const uint8 Id3InfoTextIndex[]=
{
	' ',
	'.',
	'|',
	'-',
	'+',
	'=',
	'\'',
	'_',
	'>',
	'<',
	'[',
	']',
	'*',
	'/',
	'\\',
	'#',
};

	uint8 Pos;

	if(c >= 'A' && c <= 'Z')return TRUE;
	else if(c >= 'a' && c <= 'z')return TRUE;
	else if(c >= '0' && c <= '9')return TRUE;
	else
	{
		for(Pos = 0;Pos <sizeof(Id3InfoTextIndex);Pos ++)
		{
			if(c == Id3InfoTextIndex[Pos])return TRUE;
		}
    #if defined(_SUPPORT_CHAR_RUSSIAN_)
    	if(c>= CHAR_RUSSIAN_START && c<= CHAR_RUSSIAN_END)
    	{
    		return TRUE;
    	}
    #endif
	}
	return FALSE;
}

static void play_Id3_disp_format(uint8 *Data)
{
	int Pos;
	int Len;
	int CopyLen;
	uint8 check_null = TRUE;

	Len = strlen((char *)Data);
	if(!Len)return;

	Pos = Len - 1;
	do{
		if(check_null)
		{
			if(Data[Pos] == ' ')
			{
				goto CHECK_ID3_NULL;
			}
			else
			{
				check_null = FALSE;
			}
		}

		if(!play_Id3_disp_support(Data[Pos]))
		{
		CHECK_ID3_NULL:
			CopyLen = Len - Pos;
			Len --;
			memcpy(Data + Pos,Data + Pos + 1,CopyLen);
		}
	}while(Pos --);

	for(Pos = 0;Pos < Len;Pos ++)
	{
		if(Data[Pos] != ' ')break;
	}

	if(Pos == Len)
	{
		memset(Data, 0, Len);
	}
}

static uint8 play_disp_roll_init(uint8 *title,uint8 *str)
{
	int LenTitle;
	int LenStr;

	memset(ID3_CTRL_DISP.DispBuf, 0, sizeof(ID3_CTRL_DISP.DispBuf));
	memcpy(ID3_CTRL_DISP.DispBuf, str, strlen((char *)str));

	play_Id3_disp_format(ID3_CTRL_DISP.DispBuf);

	LenTitle = strlen((char *)title);
	LenStr   = strlen((char *)ID3_CTRL_DISP.DispBuf);

	if(!LenTitle || !LenStr)
	{
		ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_END;
		return FALSE;
	}

	memset(ID3_CTRL_DISP.TitleBuf,0,sizeof(ID3_CTRL_DISP.TitleBuf));
	if(LenTitle > VFD_DISP_SIZE)LenTitle = VFD_DISP_SIZE;
	memcpy(ID3_CTRL_DISP.TitleBuf,title,LenTitle);

	ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_NULL;
	return TRUE;
}

static uint8 play_disp_roll_polling(void)
{
	switch(ID3_CTRL_DISP.DispType)
	{
		case ID3_DISP_TYPE_NULL:
	    #if defined(_ADD_DIR_NAME_)
	    	if(gDir_disp_flag)
	    	{
	    		uint8 buf[8];
				puts("--------gDir_disp_flag--------\n");
	    		memset(buf,' ',sizeof(buf));
	    		VdfDispString(buf,VFD_DISP_TYPE_TIME,1);
				gDir_disp_flag =FALSE;
	    	}
	    	else
	    #endif
			VdfDispString(ID3_CTRL_DISP.TitleBuf,VFD_DISP_TYPE_TIME,1000);
			ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_TITLE;
		case ID3_DISP_TYPE_TITLE:
			if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
			{
			#if defined(_FY7000_SPECIAL_DISPLAY_)
				uint8 disp_buf[15];
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				uint8 disp_buf[13];
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				uint8 disp_buf[12];
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				uint8 disp_buf[12];
			#else
				uint8 disp_buf[10];
			#endif

				memset(disp_buf,0,sizeof(disp_buf));
				memcpy(disp_buf,ID3_CTRL_DISP.DispBuf,VFD_DISP_SIZE);
				VdfDispString(disp_buf,VFD_DISP_TYPE_ROLL,100 | VFD_DISP_ROLL_ADD_NULL | VFD_DISP_ROLL_CUT_LEN);
				ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_FAST;
			}
			break;
		case ID3_DISP_TYPE_FAST:
			if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
			{
				VdfDispString(ID3_CTRL_DISP.DispBuf,VFD_DISP_TYPE_TIME,600);
				ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_FAST_DELAY;
			}
			break;
		case ID3_DISP_TYPE_FAST_DELAY:
			if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
			{
				if(strlen((char *)ID3_CTRL_DISP.DispBuf) > VFD_DISP_SIZE)
				{
				#if defined(_SUPPORT_LCM_1618_DRIVE)||defined(ENABLE_ID3_SLOW)//滚动慢点
					VdfDispString(ID3_CTRL_DISP.DispBuf,VFD_DISP_TYPE_ROLL,600 | VFD_DISP_ROLL_CUT_NULL);
				#else
					VdfDispString(ID3_CTRL_DISP.DispBuf,VFD_DISP_TYPE_ROLL,300 | VFD_DISP_ROLL_CUT_NULL);
				#endif
					ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_ROLL;
				}
				else ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_END;
			}
			break;
		case ID3_DISP_TYPE_ROLL:
			if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
			{
				ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_END;
			}
			break;
		case ID3_DISP_TYPE_END:
		default:
			return TRUE;
	}
	return FALSE;
}

#endif

static uint8 play_next_disp_change(uint8 *id)
{
#if defined(_SUPPORT_ID3_)

typedef struct{
	uint8 id;
	const char *title;
}id3_show_info_t;

#if defined(_FIVE_CHAR_DISPLAY_)
#if defined(_SUPPORT_SPECIAL_DISPLAY_)
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"Title"		    },
	{ID3_DISP_STATUS_SONG_NAME, 	"-Song-"		},
	{ID3_DISP_STATUS_ARTIST,		"Artis-"		},
	{ID3_DISP_STATUS_ALBUM, 		"Album-"		},
	{ID3_DISP_STATUS_DIR,			"Klaso-"		},
};
#elif defined(SPECIAL_SLA6799_DISP)
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"File"		    },
	{ID3_DISP_STATUS_DIR,			"Dir"			},
	{ID3_DISP_STATUS_SONG_NAME, 	"Title"			},
	{ID3_DISP_STATUS_ARTIST,		"Artist"		},
	{ID3_DISP_STATUS_ALBUM, 		"Album"		    },
};
#else
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"Title"		    },
	{ID3_DISP_STATUS_SONG_NAME, 	"-Song-"		},
	{ID3_DISP_STATUS_ARTIST,		"Artis-"		},
	{ID3_DISP_STATUS_ALBUM, 		"Album-"		},
	{ID3_DISP_STATUS_DIR,			"Folde-"		},
};
#endif

#elif defined(_SEVEN_CHAR_DISPLAY_)
#if defined(_FY2010_LCD_DISPLAY_)
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"Title"			},
	{ID3_DISP_STATUS_SONG_NAME, 	"Song"			},
	{ID3_DISP_STATUS_ARTIST,		"Artis"		    },
	{ID3_DISP_STATUS_ALBUM, 		"Album"			},
	{ID3_DISP_STATUS_DIR,			"Dir"		    },
};
#else
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"-Title-"		},
	{ID3_DISP_STATUS_SONG_NAME, 	"-Song-"		},
	{ID3_DISP_STATUS_ARTIST,		"-Artist"		},
	{ID3_DISP_STATUS_ALBUM, 		"-Album-"		},
	{ID3_DISP_STATUS_DIR,			"-Folder"		},
};
#endif
#elif defined(_MR1901_SPECIAL_DISPLAY_)
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"Title-"		},
	{ID3_DISP_STATUS_SONG_NAME, 	"Song-"			},
	{ID3_DISP_STATUS_ARTIST,		"Artist-"		},
	{ID3_DISP_STATUS_ALBUM, 		"Album-"		},
	{ID3_DISP_STATUS_DIR,			"Folder-"		},
};
#elif defined(_WLS316_SPECIAL_DISPLAY_)
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"File"		    },
	{ID3_DISP_STATUS_DIR,			"Dir"			},
	{ID3_DISP_STATUS_SONG_NAME, 	"Title"			},
	{ID3_DISP_STATUS_ARTIST,		"Artist"		},
	{ID3_DISP_STATUS_ALBUM, 		"Album"		    },
};
#elif defined(_LK8005_SPECIAL_DISPLAY_)
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"-Title-"		},
	{ID3_DISP_STATUS_SONG_NAME, 	"-Song-"		},
	{ID3_DISP_STATUS_ARTIST,		"-Artist-"		},
	{ID3_DISP_STATUS_ALBUM, 		"-Album-"		},
	{ID3_DISP_STATUS_DIR,			"-Dir-"		    },
};
#elif defined(_ADD_ID3_DISPLAY_)
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"Titl"		    },
	{ID3_DISP_STATUS_SONG_NAME, 	"Song"		},
	{ID3_DISP_STATUS_ARTIST,		"Arti"		},
	{ID3_DISP_STATUS_ALBUM, 		"Albu"		},
	{ID3_DISP_STATUS_DIR,			"Fold"		},
};
#else
static const id3_show_info_t Id3ShowInfoTab[]=
{
	{ID3_DISP_STATUS_TIME,			" " 			},
	{ID3_DISP_STATUS_FILE_NAME, 	"-Title-"		},
	{ID3_DISP_STATUS_SONG_NAME, 	"-Song-"		},
	{ID3_DISP_STATUS_ARTIST,		"-Artist-"		},
	{ID3_DISP_STATUS_ALBUM, 		"-Album-"		},
	{ID3_DISP_STATUS_DIR,			"-Folder-"		},
};
#endif

#define Id3ShowInfoLength 			(sizeof(Id3ShowInfoTab)/sizeof(Id3ShowInfoTab[0]))

	uint8 	Index;
	uint8	NextId;
	uint8	*disp_buf = NULL;
	uint32 	length = 0;
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8	title_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8	title_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8	title_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8	title_buf[12];
#else
	uint8	title_buf[10];
#endif

	memset(title_buf, 0, sizeof(title_buf));

	for(Index = 0; Index < Id3ShowInfoLength; Index ++)
	{
		if(*id == Id3ShowInfoTab[Index].id)break;
	}
	if(Index == Id3ShowInfoLength)
	{
		Index = 0;
	}
	else
	{
		if(Index < (Id3ShowInfoLength - 1))Index ++;
		else Index = 0;
	}

    if(id3_v2_head.id3_len > 0)
	{
        mp3_head->id3_buf = id3_v2_head.id3_buf;
        mp3_head->id3_len = id3_v2_head.id3_len;
    }
	else if(id3_v1_head.id3_len > 0)
	{
        mp3_head->id3_buf = id3_v1_head.id3_buf;
        mp3_head->id3_len = id3_v1_head.id3_len;
    }

	/*if(mp3_head->id3_buf == NULL)
	{
	#if !defined(ID3_TYPE_MODIFIER)
		Index =0;
	#endif
	}*/

#if defined(TD_ADD_LCD_NUM_4)||defined(_XM003_SPECIAL_DISPLAY_)||defined(_DISABLE_ID3_FUNTION_)||defined(_LCD_3_DIAPLAY_)
	Get_id3Info_err =TRUE;
#endif
	if(Get_id3Info_err == TRUE)
	{//ID3信息获取失败  只显示歌曲播放信息
	#if !defined(ID3_TYPE_MODIFIER)
		Index =0;
	#endif
		ew_printf("no_id3=====:%d\n",Index);
	}
	NextId = Id3ShowInfoTab[Index].id;
	*id = NextId;
	//ew_printf("id3disp=====:%d\n",Index);
	memcpy(title_buf, Id3ShowInfoTab[Index].title, strlen(Id3ShowInfoTab[Index].title));

	switch(NextId)
	{
		case ID3_DISP_STATUS_TIME:
		#if	defined(_ENABLE_SPEED_ID3_DISPLAY_)
			SET_ID3_DISP_DELAY(5000);
		#else
			SET_ID3_DISP_DELAY(10000);
		#endif
			goto id3_disp_icon;

		case ID3_DISP_STATUS_FILE_NAME:
			disp_buf 	= ID3_CTRL_INFO.id3_info[PLAY_ID3_FILE];
			length 		= ID3_CTRL_INFO.id3_len[PLAY_ID3_FILE];
			break;

		case ID3_DISP_STATUS_SONG_NAME:
			disp_buf 	= ID3_CTRL_INFO.id3_info[PLAY_ID3_TITLE];
			length 		= ID3_CTRL_INFO.id3_len[PLAY_ID3_TITLE];
			break;

		case ID3_DISP_STATUS_ARTIST:
			disp_buf 	= ID3_CTRL_INFO.id3_info[PLAY_ID3_ARTIST];
			length 		= ID3_CTRL_INFO.id3_len[PLAY_ID3_ARTIST];
			break;

		case ID3_DISP_STATUS_ALBUM:
			disp_buf 	= ID3_CTRL_INFO.id3_info[PLAY_ID3_ALBUM];
			length 		= ID3_CTRL_INFO.id3_len[PLAY_ID3_ALBUM];
			break;

		case ID3_DISP_STATUS_DIR:
			disp_buf 	= ID3_CTRL_INFO.id3_info[PLAY_ID3_FOLDER];
			length 		= ID3_CTRL_INFO.id3_len[PLAY_ID3_FOLDER];
			break;
	}

	if((disp_buf == NULL)||(length == 0))
	{
		ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_END;
		return FALSE;
	}
	else
	{
		if(!play_disp_roll_init(title_buf,disp_buf))return FALSE;
	}

id3_disp_icon:
	play_id3_icon_disp(NextId);

#else

	*id = ID3_DISP_STATUS_TIME;
	SET_ID3_DISP_DELAY(10000);

#endif

	return TRUE;
}

static void play_run_disp(void)
{
	uint8 Next;
	uint8 Enter;

	Next  = FALSE;
	Enter = FALSE;

#if defined(_ACC_OFF_POWER_LOGO_)
	if(global.bt_off)
	{
		VdfDispString(NULL,VFD_DISP_TYPE_NULL,0);
		return;
	}
#endif

#if defined(_ENABLE_PLAY_ERROR_)
	if(global.play_err)
	{
		global.play_err = FALSE;
		if(compare_task_name(MUSIC_USB_NAME))
		{
			ui_show_setting(LANG_ID_USB_ERR, 0);
			ui_disp.timer = 1000;
			ui_show_normal(LANG_ID_USB_ERR);
		}
		else if(compare_task_name(MUSIC_CARD_NAME))
		{
			ui_show_setting(LANG_ID_SD_ERR, 0);
			ui_disp.timer = 1000;
			ui_show_normal(LANG_ID_SD_ERR);
		}
		return;
	}
#endif

#if !defined(_YS_SPECIAL_DISP_)
	if(ID3_CTRL_DISP.ChangeDelay)ID3_CTRL_DISP.ChangeDelay --;

#if defined(_DIR_NAME_BUG_)
	if((gDir_disp_flag)&&(!global.first_flag))
	{
		play_disp->change_flag = TRUE;
		global.first_flag = TRUE;
	}
#endif
	
	if(play_disp->change_flag && !ID3_CTRL_DISP.ChangeDelay)
	{
		ID3_CTRL_DISP.Status = ID3_DISP_STATUS_MSG;
		play_id3_icon_disp(ID3_DISP_STATUS_TIME);
	#if defined(_ADD_DIR_NUMBER_)
        if(gDir_disp_flag)
        {
            play_dir_disp();
        }
	#if defined(_SL6812A_SPECIAL_DISPLAY_)
		else if(global.play_trk10)
		{
			play_trk10_disp(global.trk10_up);
		}
	#endif
        else
        {
    		play_media_disp();
    	}
        gDir_disp_flag = FALSE;
	#if defined(_SL6812A_SPECIAL_DISPLAY_)
		global.play_trk10 = FALSE;
	#endif
	
    #elif defined(_ADD_DIR_NAME_)
    	if(gDir_disp_flag)
    	{
    		ID3_CTRL_DISP.Status = ID3_DISP_STATUS_ALBUM;
    		ID3_CTRL_DISP.DispType = ID3_DISP_TYPE_END;

    		play_media_icon_disp(ID3_CTRL_INFO.media);
    	}
    	else
    	{
    		play_media_disp();
    	}
	#else
		play_media_disp();
	#endif
		play_disp->change_flag = FALSE;
	}
#endif
#if defined(_SUPPORT_ID3_)
	else
	{
   #if defined(_ENABLE_ID3_SET_)
   	  if(!setup.id3_on_flag)
	  {
		  play_time_disp();
		ID3_CTRL_DISP.Status = ID3_DISP_STATUS_TIME;
	  }
   #endif	
		play_media_icon_disp(ID3_CTRL_INFO.media);
		if(ID3_CTRL_DISP.Change & ID3_DISP_STATUS_CAHNGE)
		{
			if(ID3_CTRL_DISP.Change & ID3_DISP_STATUS_CAHNGE_CYCLE)
			{
				Next = TRUE;
			}
			else
			{
				uint8 Status = (ID3_CTRL_DISP.Change & 0x0F);
				if(Status)
				{
					Status --;
				}
				else
				{
					Status = ID3_DISP_STATUS_DIR;
				}
				ID3_CTRL_DISP.Status = Status;
				Enter = TRUE;
			}
		}
	}
#endif

ID3_STATUS_DISP_RUN_ANGIN:
	switch(ID3_CTRL_DISP.Status)
	{
		case ID3_DISP_STATUS_TIME:
		#if defined(_ENABLE_ID3_ON_OFF_)||defined(_ENABLE_ID3_SET_)
			if(setup.id3_on_flag)
			{
				if(Next || Enter || !GET_ID3_DISP_DELAY())
				{
					if(Enter)Enter = FALSE;
					if(!play_next_disp_change(&ID3_CTRL_DISP.Status))
						goto ID3_STATUS_DISP_RUN_ANGIN;
					if(Next)Next  = FALSE;
				}
				else if(GET_ID3_DISP_DELAY())
				{
					GET_ID3_DISP_DELAY()--;
					play_time_disp();
				}
			}
			else
			{
				play_time_disp();
			}
		#else
		#if defined(_DISABLE_ID3_INFO_)
    		if(Next)
    		{
    			if(Enter)Enter = FALSE;
    			if(!play_next_disp_change(&ID3_CTRL_DISP.Status))
    				goto ID3_STATUS_DISP_RUN_ANGIN;
    			if(Next)Next  = FALSE;
    		}
    		else
		#else
			if(Next || Enter || !GET_ID3_DISP_DELAY())
			{
				if(Enter)Enter = FALSE;
				if(!play_next_disp_change(&ID3_CTRL_DISP.Status))
					goto ID3_STATUS_DISP_RUN_ANGIN;
				if(Next)Next  = FALSE;
			}
			else if(GET_ID3_DISP_DELAY())
		#endif
			{
				GET_ID3_DISP_DELAY()--;
			#if defined(_YS_SPECIAL_DISP_)
				if(play_disp->info.mode == PLAY_MODE_REPEAT_ALL)
				{
					play_time_disp();
				}
				else
				{
					PlayerDispPlayModeTime();
				}
			#else
				play_time_disp();
			#endif
			}
		#endif	
			break;

	#if defined(_SUPPORT_ID3_)
		case ID3_DISP_STATUS_FILE_NAME:
		case ID3_DISP_STATUS_SONG_NAME:
		case ID3_DISP_STATUS_ARTIST:
		case ID3_DISP_STATUS_ALBUM:
		case ID3_DISP_STATUS_DIR:
			if(Next || Enter || play_disp_roll_polling())
			{
				if(Enter)Enter = FALSE;
				if(!play_next_disp_change(&ID3_CTRL_DISP.Status))
					goto ID3_STATUS_DISP_RUN_ANGIN;
				if(Next)Next  = FALSE;
			}
			break;
	#endif

		case ID3_DISP_STATUS_MSG:
			if(ID3_CTRL_DISP.MsgChange)
			{
				VdfDispString(ID3_CTRL_DISP.Msg,VFD_DISP_TYPE_TIME,ID3_CTRL_DISP.MsgTime);
				ID3_CTRL_DISP.MsgChange = FALSE;
			}
			else
			{
				if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
				{
					if(ID3_CTRL_DISP.MsgStatus != ID3_DISP_STATUS_MSG)
					{
						ID3_CTRL_DISP.Status = ID3_CTRL_DISP.MsgStatus;
					}
					else
					{
						ID3_CTRL_DISP.Status = ID3_DISP_STATUS_TIME;
					#if	defined(_ENABLE_SPEED_ID3_DISPLAY_)
						SET_ID3_DISP_DELAY(5000);
					#else
						SET_ID3_DISP_DELAY(10000);
					#endif
					}
				}
			}
			break;

		default:
			break;
	}
	ID3_CTRL_DISP.Change = FALSE;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static uint8 Id3StatusInitInit(void)
{
	memset(&ID3_CTRL_INIT,0,sizeof(ID3_CTRL_INIT));
	ID3_CTRL_INIT.Status = ID3_INIT_STATUS_LOAD;
	ID3_CTRL_INIT.Error = PLAY_ERROR_NULL;
	return TRUE;
}
static uint8 Id3StatusInitRun(void)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#else
	uint8 disp_buf[10];
#endif

	switch(ID3_CTRL_INIT.Status)
	{
		case ID3_INIT_STATUS_LOAD:
		#if defined(_SUPPORT_LCM_1618_DRIVE)// 修改没有USB\SD卡的LoGo
			if(!ui_disp.timer)
		#endif
			{
				if(ID3_CTRL_INFO.error == PLAY_ERROR_NULL)
				{
				#if defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("    Load 	",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("   Load    ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString(" READING  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("     USB  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_KLD3000_SPECIAL_DISPLAY_)
					VdfDispString("  USB   ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XM003_SPECIAL_DISPLAY_)
					if(compare_task_name("MusicTaskUsb"))
					{
						VdfDispString("   USB  ",VFD_DISP_TYPE_TIME,0);
					}
					else
					{
						VdfDispString("    SD  ",VFD_DISP_TYPE_TIME,0);
					}
				#elif defined(_HKY1032_SPECIAL_DISPLAY_)
					VdfDispString("    load",VFD_DISP_TYPE_TIME,0);
				#else
					VdfDispString("Load",VFD_DISP_TYPE_TIME,0);
				#endif
				}
			#if defined(_XM003_SPECIAL_DISPLAY_)||defined(_XZC1668_SPECIAL_DISPLAY_)
				ID3_CTRL_INIT.Status = ID3_INIT_STATUS_PLAY;
			#else
				ID3_CTRL_INIT.Status = ID3_INIT_STATUS_TOTAL;
			#endif
			}
			break;
		case ID3_INIT_STATUS_TOTAL:
			if(ID3_CTRL_INFO.error == PLAY_ERROR_NULL)
			{
				play_disp->change_flag = TRUE;

                ID3_CTRL_INFO.total =music_cur_filetotal;
				if(ID3_CTRL_INFO.total)
				{
				#if defined(_ENABLE_FOLD_TOTAL_)
					global.total_flag = TRUE;
					uint16 total = music_dir_total%10000;
				#else
					uint16 total = ID3_CTRL_INFO.total%10000;
				#endif
				
				#if defined(TD_ADD_LCD_NUM_4)//显示总曲目
				#if defined(HuiNuo_6651D_Copy)
				    disp_buf[0] = ' ';
					disp_buf[1] =  total/100	+ '0';
		    		disp_buf[2] = (total%100)/10 + '0';
		    		disp_buf[3] = (total%100)%10 + '0';
				#elif defined(_WLS1403_COPY_DISPLAY_)
					if(total < 1000)
					{
						disp_buf[0] = ' ';
    					disp_buf[1] = total/100	+ '0';
    					disp_buf[2] = (total%100)/10 + '0';
    					disp_buf[3] = (total%100)%10 + '0';

					}
					else
					{
						disp_buf[0] = (total/100)/10 + '0';
						disp_buf[1] = (total/100)%10 + '0';
						disp_buf[2] = (total%100)/10 + '0';
						disp_buf[3] = (total%100)%10 + '0';
					}
				#else
    				disp_buf[0] = (total/100)/10 + '0';
    				disp_buf[1] = (total/100)%10 + '0';
		    		disp_buf[2] = (total%100)/10 + '0';
		    		disp_buf[3] = (total%100)%10 + '0';
				#endif
				#elif defined(_LCD_3_DIAPLAY_)
				    disp_buf[0] = ' ';
					disp_buf[1] =  total/100	+ '0';
		    		disp_buf[2] = (total%100)/10 + '0';
		    		disp_buf[3] = (total%100)%10 + '0';

				#elif defined(_FY7000_SPECIAL_DISPLAY_)
					disp_buf[0] = ' ';
					disp_buf[1] = ' ';
    				disp_buf[2] = 'T';
    				disp_buf[3] = 'O';
    				disp_buf[4] = 'L';
    				disp_buf[5] = ' ';
    				disp_buf[6] = ' ';

					disp_buf[7] = (total/100)/10 + '0';
					disp_buf[8] = (total/100)%10 + '0';
					disp_buf[9] = (total%100)/10 + '0';
					disp_buf[10] = (total%100)%10 + '0';
    				disp_buf[11] = ' ';
    				disp_buf[12] = ' ';
					disp_buf[13] = 0;
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				#if defined(_ENABLE_FOLD_TOTAL_)
    				disp_buf[0] = 'F';
    				disp_buf[1] = 'O';
    				disp_buf[2] = 'L';
    				disp_buf[3] = 'D';
    				disp_buf[4] = ' ';
    				disp_buf[5] = ' ';
					if(total < 1000)
					{
						disp_buf[6] = ' ';
			    		disp_buf[7] =  total/100	+ '0';
			    		disp_buf[8] = (total%100)/10 + '0';
			    		disp_buf[9] = (total%100)%10 + '0';
					}
					else
					{
						disp_buf[6] = (total/100)/10 + '0';
						disp_buf[7] = (total/100)%10 + '0';
						disp_buf[8] = (total%100)/10 + '0';
						disp_buf[9] = (total%100)%10 + '0';
					}
				#else
    				disp_buf[0] = ' ';
    				disp_buf[1] = 'T';
    				disp_buf[2] = 'O';
    				disp_buf[3] = 'L';
    				disp_buf[4] = ' ';
    				disp_buf[5] = (total/100)/10 + '0';
    				disp_buf[6] = (total/100)%10 + '0';
    				disp_buf[7] = (total%100)/10 + '0';
    				disp_buf[8] = (total%100)%10 + '0';
    				disp_buf[9] = ' ';
				#endif
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)
    				disp_buf[0] = ' ';
    				disp_buf[1] = 'T';
    				disp_buf[2] = 'O';
    				disp_buf[3] = 'L';
    				disp_buf[4] = ' ';
    				disp_buf[5] = (total/100)/10 + '0';
    				disp_buf[6] = (total/100)%10 + '0';
    				disp_buf[7] = (total%100)/10 + '0';
    				disp_buf[8] = (total%100)%10 + '0';
    				disp_buf[9] = ' ';
				#elif defined(_SH1330_SPECIAL_DISPLAY_)
    				disp_buf[0] = ' ';
    				disp_buf[1] = 'T';
    				disp_buf[2] = 'O';
    				disp_buf[3] = 'L';
    				disp_buf[4] = ' ';
    				disp_buf[5] = ' ';		
    				disp_buf[6] = (total/100)/10 + '0';
    				disp_buf[7] = (total/100)%10 + '0';
    				disp_buf[8] = (total%100)/10 + '0';
    				disp_buf[9] = (total%100)%10 + '0';
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
    				disp_buf[0] = ' ';
    				disp_buf[1] = 'T';
    				disp_buf[2] = 'O';
    				disp_buf[3] = 'L';
    				disp_buf[4] = ' ';
    				disp_buf[5] = ' ';
    				disp_buf[6] = (total/100)/10 + '0';
    				disp_buf[7] = (total/100)%10 + '0';
    				disp_buf[8] = (total%100)/10 + '0';
    				disp_buf[9] = (total%100)%10 + '0';
    				disp_buf[10] = ' ';
				#elif defined(_HH_SPECIAL_DISPLAY_)
    				disp_buf[0] = ' ';
    				disp_buf[1] = 'T';
    				disp_buf[2] = 'O';
    				disp_buf[3] = 'L';
    				disp_buf[4] = ' ';
    				disp_buf[5] = (total/100)/10 + '0';
    				disp_buf[6] = (total/100)%10 + '0';
    				disp_buf[7] = (total%100)/10 + '0';
    				disp_buf[8] = (total%100)%10 + '0';
				#elif defined(_FIVE_CHAR_DISPLAY_)
				#if defined(SPECIAL_JD_DISP)
    				disp_buf[0] = ' ';
    				disp_buf[1] = ' ';
					disp_buf[2] = ' ';
    				disp_buf[3] = ' ';
    				disp_buf[4] = ' ';
    				disp_buf[5] = ' ';
				#else
    				disp_buf[0] = 'T';
    				disp_buf[1] = 'o';
    				disp_buf[2] = 'l';

    				disp_buf[3] = (total/100)%10 + '0';
    				disp_buf[4] = (total%100)/10 + '0';
    				disp_buf[5] = (total%100)%10 + '0';
    				disp_buf[6] = ' ';
				#endif
				#elif defined(_SEVEN_CHAR_DISPLAY_)
    				disp_buf[0] = 'T';
    				disp_buf[1] = 'o';
    				disp_buf[2] = 'l';

				#if defined(_PS329a_SPECIAL_DISPLAY_)
					disp_buf[3] = (total/100)/10 + '0';
					disp_buf[4] = (total/100)%10 + '0';
					disp_buf[5] = (total%100)/10 + '0';
					disp_buf[6] = (total%100)%10 + '0';
					disp_buf[7] = ' ';
				#else
					disp_buf[3] = ' ';
    				disp_buf[4] = (total/100)%10 + '0';
    				disp_buf[5] = (total%100)/10 + '0';
    				disp_buf[6] = (total%100)%10 + '0';
				#endif
				#elif defined(_MR1901_SPECIAL_DISPLAY_)
    				disp_buf[0] = 'T';
    				disp_buf[1] = 'o';
    				disp_buf[2] = 'l';
    				disp_buf[3] = ' ';
					disp_buf[4] = (total/100)/10 + '0';
					disp_buf[5] = (total/100)%10 + '0';
					disp_buf[6] = (total%100)/10 + '0';
					disp_buf[7] = (total%100)%10 + '0';
					disp_buf[8] = 0;
				#else
					disp_buf[0] = 'T';
					disp_buf[1] = 'o';
					disp_buf[2] = 'l';
					disp_buf[3] = ' ';

			    	if(total < 1000)
			    	{
			    		disp_buf[4] =  total/100	+ '0';
			    		disp_buf[5] = (total%100)/10 + '0';
			    		disp_buf[6] = (total%100)%10 + '0';
			    		disp_buf[7] = 0;
			    	}
			    	else
			    	{
			    		disp_buf[4] = (total/100)/10 + '0';
			    		disp_buf[5] = (total/100)%10 + '0';
			    		disp_buf[6] = (total%100)/10 + '0';
			    		disp_buf[7] = (total%100)%10 + '0';
			    		disp_buf[8] = 0;
			    	}
				#if defined(SPECIAL_JD_DISP)||defined(_YS_SPECIAL_DISP_)
					disp_buf[0] = ' ';
					disp_buf[1] = ' ';
					disp_buf[2] = ' ';
					disp_buf[3] = ' ';
					disp_buf[4] = ' ';
					disp_buf[5] = ' ';
					disp_buf[6] = ' ';
					disp_buf[7] = ' ';
				#endif
				#endif
				#if defined(SPECIAL_JD_DISP)||defined(_YS_SPECIAL_DISP_)
					ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
					VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,1);
				#else
					VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,1000);
				#endif
					ID3_CTRL_INIT.Status = ID3_INIT_STATUS_DELAY;
				}
				else
				{
				#if defined(_SUPPORT_LCM_1618_DRIVE)// 修改没有USB\SD卡的LoGo
					if(!ui_disp.timer)
				#endif
					{
    					if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
    					{
    					#if defined(_FY7000_SPECIAL_DISPLAY_)
    						VdfDispString("    Load     ",VFD_DISP_TYPE_TIME,0);
						#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
							VdfDispString("     USB  ",VFD_DISP_TYPE_TIME,0);
						#elif defined(_FY5517_SPECIAL_DISPLAY_)
							VdfDispString("   Load   ",VFD_DISP_TYPE_TIME,0);
						#elif defined(_KLD3000_SPECIAL_DISPLAY_)
							VdfDispString("  USB   ",VFD_DISP_TYPE_TIME,0);
						#elif defined(_SL6812A_SPECIAL_DISPLAY_)
							VdfDispString(" READING  ",VFD_DISP_TYPE_TIME,0);
						#elif defined(_HKY1032_SPECIAL_DISPLAY_)
							VdfDispString("    load",VFD_DISP_TYPE_TIME,0);
    					#else
    		            	VdfDispString("Load",VFD_DISP_TYPE_TIME,0);
    					#endif
    					}
					}
				}
			}
			else if(ID3_CTRL_INIT.Error != ID3_CTRL_INFO.error)
			{
				ID3_CTRL_INIT.Error = ID3_CTRL_INFO.error;
				play_error_disp(ID3_CTRL_INFO.error);
			}
			break;
		case ID3_INIT_STATUS_DELAY:
			if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
			{
				ID3_CTRL_INIT.Status = ID3_INIT_STATUS_PLAY;
			}
			break;
		case ID3_INIT_STATUS_PLAY:
			if(ID3_CTRL_INFO.state == PLAY_STATUS_PLAYE)
			{
				ID3_CTRL_INIT.Status = ID3_INIT_STATUS_END;
			}
			break;
		case ID3_INIT_STATUS_END:
			ID3_ID_NEXT = ID3_STATUS_DISP;
			break;
	}

	return TRUE;
}
static uint8 Id3StatusInitExit(void)
{
	memset(&ID3_CTRL_INIT,0,sizeof(ID3_CTRL_INIT));
	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static uint8 Id3StatusDispInit(void)
{
	memset(&ID3_CTRL_DISP,0,sizeof(ID3_CTRL_DISP));
	ID3_CTRL_DISP.Status 	= ID3_DISP_STATUS_TIME;
	ID3_CTRL_DISP.Mode 		= ID3_DISP_TIME_MODE_NORMAL;
	ID3_CTRL_DISP.PlayState = PLAY_STATUS_IDLE;
	play_id3_icon_disp(ID3_DISP_STATUS_TIME);
#if	defined(_ENABLE_SPEED_ID3_DISPLAY_)
	SET_ID3_DISP_DELAY(5000);
#else
	SET_ID3_DISP_DELAY(10000);
#endif
	return TRUE;
}
static uint8 Id3StatusDispRun(void)
{
#if defined(_EVB_PAUSE_DISPLAY_)
	uint8 disp_buf[10];
	uint8 min = ID3_CTRL_INFO.min%100;
	uint8 sec = ID3_CTRL_INFO.sec%100;

	if(ID3_CTRL_INFO.state == PLAY_STATUS_PAUSE)
	{
		global.pause_flag = TRUE;
	}
	else
	{
		global.pause_flag = FALSE;
	}
#endif

    if(ID3_CTRL_INFO.state == PLAY_STATUS_PAUSE)
    {
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PAUSE);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAY);
	#if defined(_ENABLE_PAUSE_LIGHT_)
		global.pause_flg = FALSE;
	#endif
	}
	else if(ID3_CTRL_INFO.state == PLAY_STATUS_STOP)
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PAUSE);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAY);
	#if defined(_ENABLE_PAUSE_LIGHT_)
		global.pause_flg = FALSE;
	#endif
	}
	else if(ID3_CTRL_INFO.state == PLAY_STATUS_PLAYE)
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PLAY);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PAUSE);
	#if defined(_ENABLE_PAUSE_LIGHT_)
		global.pause_flg = TRUE;
	#endif
	
	}

	if(ID3_CTRL_INFO.error != PLAY_ERROR_NULL)
	{
		ID3_CTRL_DISP.PlayState = PLAY_STATUS_IDLE;
		play_error_disp(ID3_CTRL_INFO.error);
	}
	else
	{
		switch(ID3_CTRL_INFO.state)
		{
			case PLAY_STATUS_IDLE:
				if(ID3_CTRL_DISP.PlayState != PLAY_STATUS_IDLE)
				{
				#if defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("    Load     ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("     USB  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("   Load  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString(" READING  ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_KLD3000_SPECIAL_DISPLAY_)
					VdfDispString("  USB   ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_HKY1032_SPECIAL_DISPLAY_)
					VdfDispString("    load",VFD_DISP_TYPE_TIME,0);
				#else
			    	VdfDispString("Load",VFD_DISP_TYPE_TIME,0);
				#endif
			    #if defined(_ENABLE_TIME_DISP_)
			    	global.timer_count = 5000;
			    #endif
				}
				break;
			case PLAY_STATUS_PLAYE:
				if(ID3_CTRL_DISP.PlayState != PLAY_STATUS_PLAYE)
				{
					VdfDispString(NULL,VFD_DISP_TYPE_NULL,0);
				}
				play_run_disp();
				break;
			case PLAY_STATUS_PAUSE:
				if(ID3_CTRL_DISP.PlayState != PLAY_STATUS_PAUSE)
				{
				#if defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("    Pause    ",VFD_DISP_TYPE_WINK,500);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("   Pause   ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("    Pause ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_YS_SPECIAL_DISP_)
			    	VdfDispString("Pause",VFD_DISP_TYPE_TIME,0);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString("  Pause   ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_HKY1032_SPECIAL_DISPLAY_)
					VdfDispString("    Paus",VFD_DISP_TYPE_TIME,500);
				#elif defined(_SL6802_SPECIA_DISPLAY_)
					VdfDispString("DURAKL  ",VFD_DISP_TYPE_WINK,500);
				#elif defined(_EVB_PAUSE_DISPLAY_)
        			disp_buf[0] = min/10 + '0';
        			disp_buf[1] = min%10 + '0';
        			disp_buf[2] = sec/10 + '0';
        			disp_buf[3] = sec%10 + '0';
    				disp_buf[4] = ' ';
        			disp_buf[5] = ' ';
        			disp_buf[6] = ' ';
        			disp_buf[7] = ' ';
					VdfDispString(disp_buf,VFD_DISP_TYPE_WINK,500);
				#elif defined(SPECIAL_SLA6799_DISP)
				    VdfDispString(" Pause",VFD_DISP_TYPE_TIME,0);
				#elif defined(_KLD3000_SPECIAL_DISPLAY_)
					VdfDispString("  Pause ",VFD_DISP_TYPE_TIME,0);
				#elif defined(_SPECIAL_PAUSE_DISPLAY_)
					VdfDispString("Pause   ",VFD_DISP_TYPE_WINK,500);
				#else
			    	VdfDispString("Pause",VFD_DISP_TYPE_WINK,500);
				#endif
				}
				break;
			case PLAY_STATUS_STOP:
				if(ID3_CTRL_DISP.PlayState != PLAY_STATUS_STOP)
				{
				#if defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("    Stop     ",VFD_DISP_TYPE_WINK,500);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("   Stop    ",VFD_DISP_TYPE_WINK,500);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("    Stop  ",VFD_DISP_TYPE_WINK,500);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString("   Stop   ",VFD_DISP_TYPE_WINK,500);
				#elif defined(_HKY1032_SPECIAL_DISPLAY_)
					VdfDispString("    Stop",VFD_DISP_TYPE_WINK,500);
				#else
			    	VdfDispString("Stop",VFD_DISP_TYPE_WINK,500);
				#endif
				}
				break;
			case PLAY_STATUS_FORWARD:
			case PLAY_STATUS_BACKWARD:
				play_speed_disp();
				break;
			default:break;
		}
		ID3_CTRL_DISP.PlayState = ID3_CTRL_INFO.state;
	}
	return TRUE;
}
static uint8 Id3StatusDispExit(void)
{
	play_id3_icon_disp(ID3_DISP_STATUS_END);
	memset(&ID3_CTRL_DISP,0,sizeof(ID3_CTRL_DISP));
	return TRUE;
}

#if defined(SUPPORT_NUM_SELECT_MUSIC)
void get_id3_disp_status(void)
{
	if(play_disp == NULL)return;
	if(ID3_ID != ID3_STATUS_SELECT_MUSIC)
	{
	    ID3_ID_NEXT = ID3_STATUS_SELECT_MUSIC;
	}
}
uint8 Id3SelectMusicInit(void)
{
	memset(&ID3_CTRL_TRK,0,sizeof(ID3_CTRL_TRK));
	os_taskq_post_msg("MusicTask", 2, MSG_MUSIC_DISP, 1);
	return TRUE;
}

uint8 Id3SelectMusicRun(void)
{
	//uint16 	Tune;
#if defined (TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	uint8 	disp_buf[4];
#else
	uint8 	disp_buf[8];
#endif

    memset(&disp_buf,' ', sizeof(disp_buf));
    if((play_disp->input.cmd >= PLAY_CMD_0)
    &&(play_disp->input.cmd <= PLAY_CMD_9))
    {
        uint8 Number = play_disp->input.cmd - PLAY_CMD_0;
        ID3_CTRL_TRK.Tune= (ID3_CTRL_TRK.Tune*10)%10000;
        ID3_CTRL_TRK.Tune = ID3_CTRL_TRK.Tune+Number;
		ID3_CTRL_TRK.OverTime =0;
	#if defined (TD_ADD_LCD_NUM_4)
		disp_buf[0]	=  (ID3_CTRL_TRK.Tune%10000/1000)+'0';
		disp_buf[1]	=  (ID3_CTRL_TRK.Tune%1000/100)+'0';
		disp_buf[2]	=  (ID3_CTRL_TRK.Tune%100/10)+'0';
		disp_buf[3]	=  (ID3_CTRL_TRK.Tune%10)+'0';
		if(ID3_CTRL_TRK.Tune<10)
		{
			disp_buf[0]	= ' ';
			disp_buf[1]	= ' ';
			disp_buf[2]	= ' ';
		}
		else if(ID3_CTRL_TRK.Tune<100)
		{
			disp_buf[0]	= ' ';
			disp_buf[1]	= ' ';
		}
		else if(ID3_CTRL_TRK.Tune<1000)
		{
			disp_buf[0]	= ' ';
		}
	#else
		disp_buf[0] = ' ';
		disp_buf[1] = ' ';
		disp_buf[2]	=  (ID3_CTRL_TRK.Tune%10000/1000)+'0';
		disp_buf[3]	=  (ID3_CTRL_TRK.Tune%1000/100)+'0';
		disp_buf[4]	=  (ID3_CTRL_TRK.Tune%100/10)+'0';
		disp_buf[5]	=  (ID3_CTRL_TRK.Tune%10)+'0';
		disp_buf[6] = ' ';
		disp_buf[7] = ' ';
		if(ID3_CTRL_TRK.Tune<10)
		{
			disp_buf[2]	= ' ';
			disp_buf[3]	= ' ';
		}
		else if(ID3_CTRL_TRK.Tune<100)
		{
			disp_buf[2]	= ' ';
		}
	#endif
	#if defined(_XM003_SPECIAL_DISPLAY_)
		VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,3000);
	#else
		VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,6000);
	#endif
    }
    /*else if(play_disp->input.cmd == PLAY_CMD_ENTER)
    {
        if(!ID3_CTRL_TRK.Tune)
        {
            VdfDispString("    NULL",VFD_DISP_TYPE_TIME,2000);
        }
        else if(ID3_CTRL_TRK.Tune > ID3_CTRL_INFO.total)
        {
            disp_buf[4]     =  'M';
            disp_buf[5]     =  (ID3_CTRL_INFO.total%1000/100)+'0';
            disp_buf[6]     =  (ID3_CTRL_INFO.total%100/10)+'0';
            disp_buf[7]     =  (ID3_CTRL_INFO.total%10)+'0';
            VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,2000);
        }
        else
        {
			ID3_ID_NEXT = ID3_STATUS_DISP;
            os_taskq_post_msg("MusicTask", 2, MSG_MUSIC_TRK, ID3_CTRL_TRK.Tune);
        }
    }*/
    else
    {
		if(ID3_CTRL_TRK.OverTime < ID3_SCH_OVER_TIME)
		{
			ID3_CTRL_TRK.OverTime ++;
		}
		else
		{
			ID3_ID_NEXT = ID3_STATUS_DISP;
	    	if((ID3_CTRL_TRK.Tune > 0)&&(ID3_CTRL_TRK.Tune <= ID3_CTRL_INFO.total))
	    	{
	    	#if defined(_ENABLE_RPT_DIR_BUG_)
				if(setup.rpt_start == MUSIC_RPT_FOLDER)
				{
					if((setup.first_dir == 0)||(setup.last_dir == 0))
					{
						os_taskq_post_msg("MusicTask", 2, MSG_MUSIC_TRK, ID3_CTRL_TRK.Tune);
					}
					else
					{
						if((ID3_CTRL_TRK.Tune >= setup.first_dir)&&(ID3_CTRL_TRK.Tune <= setup.last_dir))
						{
							os_taskq_post_msg("MusicTask", 2, MSG_MUSIC_TRK, ID3_CTRL_TRK.Tune);
						}
					}
				}
				else
				{
					os_taskq_post_msg("MusicTask", 2, MSG_MUSIC_TRK, ID3_CTRL_TRK.Tune);
				}
			#else
				os_taskq_post_msg("MusicTask", 2, MSG_MUSIC_TRK, ID3_CTRL_TRK.Tune);
			#endif
	    	}
		}
    }
    play_disp->input.cmd = PLAY_CMD_NULL;
	return TRUE;
}
uint8 Id3SelectMusicExit(void)
{
	os_taskq_post_msg("MusicTask", 2, MSG_MUSIC_DISP, 0);
	memset(&ID3_CTRL_TRK,0,sizeof(ID3_CTRL_TRK));
	return TRUE;
}
#endif

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
#if defined(_FIVE_CHAR_DISPLAY_)
static void Id3TrkSchIncChangeDisp(uint16 Tune)
{
	uint8 disp_buf[10];
	const char *str;

	Tune %= 10000;

	if(!Tune)
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    str = "TRK---";
	#else
		str = "T ***";
	#endif
	}
	else if(Tune < 10)
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    str = "TRK--%d";
	#else
		str = "T **%d";
	#endif
		ID3_CTRL_TRK.AutoRun = TRUE;
	}
	else if(Tune < 100)
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    str = "TRK-%d";
	#else
		str = "T *%d";
	#endif
		ID3_CTRL_TRK.AutoRun = TRUE;
	}
	else
	{
		str = "TRK%d";
		ID3_CTRL_TRK.AutoRun = TRUE;
	}

	sprintf((char *)disp_buf,str,Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 0);
}

static void Id3TrkSchBuntChangeDisp(uint16 Tune)
{
	uint8 disp_buf[10];
	const char *str;

	Tune %= 10000;

	if(Tune)
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    if(Tune < 10)
		{
			str = "TRK--%d";
		}
		else if(Tune < 100)
		{
			str = "TRK-%d";
		}
		else
		{
			str = "TRK%d";
		}
	#else
		if(Tune < 10)
		{
			str = "T --%d";
		}
		else if(Tune < 100)
		{
			str = "T -%d";
		}
		else
		{
			str = "T %d";
		}
	#endif	
		ID3_CTRL_TRK.Change = TRUE;
	}
	else
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    str = "TRK---";
	#else
		str = "T ***";
	#endif
		ID3_CTRL_TRK.Change = FALSE;
	}

	sprintf((char *)disp_buf,str,Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, ID3_CTRL_TRK.Change?1500:0);
}

static void Id3TrkSchBuntChangeDisp2(uint16 Tune)
{
	uint8 disp_buf[10];
	const char *str;

	Tune %= 10000;
	if(Tune < 10)
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    str = "TRK--%d";
	#else
		str = "T -%d*";
	#endif
		ID3_CTRL_TRK.Next = TRUE;
	}
	else if(Tune < 100)
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    str = "TRK-%d";
	#else
		str = "T %d*";
	#endif
		ID3_CTRL_TRK.Next = TRUE;
	}
	else
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    str = "TRK%d";
	#else
		str = "T %d";
	#endif
		ID3_CTRL_TRK.Next = FALSE;
	}

	sprintf((char *)disp_buf, str, Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 500);
}

static void Id3TrkSchBuntChangeFlash(uint16 Tune)
{
	uint8 disp_buf[10];
	const char *str;

	Tune %= 10000;
	if(Tune < 10)
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    if(ID3_CTRL_TRK.Flash)
		{
			str = "TRK--%d";
		}
		else
		{
			str = "TRK-%d ";
		}
	#else
		if(ID3_CTRL_TRK.Flash)
		{
			str = "T -%d*";
		}
		else
		{
			str = "T -%d ";
		}
	#endif	
	}
	else if(Tune <100)
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    if(ID3_CTRL_TRK.Flash)
		{
			str = "TRK-%d";
		}
		else
		{
			str = "TRK%d ";
		}
	#else
		if(ID3_CTRL_TRK.Flash)
		{
			str = "T %d*";
		}
		else
		{
			str = "T %d ";
		}
	#endif	
	}
	else
	{
	#if defined(SPECIAL_SLA6799_DISP)
	    if(ID3_CTRL_TRK.Flash)
		{
			str = "TRK%d";
		}
		else
		{
			str = "TRK%d";
			Tune/=10;
		}
	#else
		if(ID3_CTRL_TRK.Flash)
		{
			str = "T %d";
		}
		else
		{
			str = "T %d ";
			Tune/=10;
		}
	#endif	
	}

	ID3_CTRL_TRK.Flash = !ID3_CTRL_TRK.Flash;
	sprintf((char *)disp_buf, str, Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 500);
}
#elif defined(_SEVEN_CHAR_DISPLAY_)&& !defined(_PS329a_SPECIAL_DISPLAY_)
static void Id3TrkSchIncChangeDisp(uint16 Tune)
{
	uint8 disp_buf[10];
	const char *str;
	Tune %= 10000;
	if(ID3_CTRL_INFO.total < 1000)
	{
		if(!Tune)
		{
		#if defined(_FY2010_LCD_DISPLAY_)
			str = "TRK ---";
		#else
			str = "TRK ***";
		#endif
		}
		else if(Tune < 10)
		{
		#if defined(_FY2010_LCD_DISPLAY_)
			str = "TRK --%d";
		#else
			str = "TRK **%d";
		#endif
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
		else if(Tune < 100)
		{
		#if defined(_FY2010_LCD_DISPLAY_)
			str = "TRK -%d";
		#else
			str = "TRK *%d";
		#endif
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
		else
		{
			str = "TRK %d";
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
	}
	sprintf((char *)disp_buf,str,Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 0);
}

static void Id3TrkSchBuntChangeDisp(uint16 Tune)
{
	uint8 disp_buf[10];
	const char *str;
	Tune %= 10000;
	if(ID3_CTRL_INFO.total < 1000)
	{
		if(Tune)
		{
			if(Tune < 10)
			{
				str = "TRK --%d";
			}
			else if(Tune < 100)
			{
				str = "TRK -%d";
			}
			else
			{
				str = "TRK %d";
			}
			ID3_CTRL_TRK.Change = TRUE;
		}
		else
		{
		#if defined(_FY2010_LCD_DISPLAY_)
			str = "TRK ---";
		#else
			str = "TRK ***";
		#endif
			ID3_CTRL_TRK.Change = FALSE;
		}
	}
	sprintf((char *)disp_buf,str,Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, ID3_CTRL_TRK.Change?1500:0);
}

static void Id3TrkSchBuntChangeDisp2(uint16 Tune)
{

	uint8 disp_buf[10];
	const char *str;
	Tune %= 10000;
	if(ID3_CTRL_INFO.total < 1000)
	{
		if(Tune < 10)
		{
			str = "TRK -%d*";
			ID3_CTRL_TRK.Next = TRUE;
		}
		else if(Tune < 100)
		{
			str = "TRK %d*";
			ID3_CTRL_TRK.Next = TRUE;
		}
		else
		{
			str = "TRK %d";
			ID3_CTRL_TRK.Next = FALSE;
		}
	}
	sprintf((char *)disp_buf, str, Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 500);
}

static void Id3TrkSchBuntChangeFlash(uint16 Tune)
{

	uint8 disp_buf[10];
	const char *str;
	Tune %= 10000;
	if(ID3_CTRL_INFO.total < 1000)
	{
		if(Tune < 10)
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK -%d*";
			}
			else
			{
				str = "TRK -%d ";
			}
		}
		else if(Tune <100)
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK %d*";
			}
			else
			{
				str = "TRK %d ";
			}
		}
		else
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK %d";
			}
			else
			{
				str = "TRK %d ";
				Tune/=10;
			}
		}
	}
	ID3_CTRL_TRK.Flash = !ID3_CTRL_TRK.Flash;
	sprintf((char *)disp_buf, str, Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 500);
}

#else

static void Id3TrkSchIncChangeDisp(uint16 Tune)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#else
	uint8 disp_buf[10];
#endif
	const char *str;

	Tune %= 10000;
#if defined(TD_ADD_LCD_NUM_4)
	if(!Tune)
	{
	    str = "----";
	}
	else if(Tune < 10)
	{
	    str = "---%d-";
	    ID3_CTRL_TRK.AutoRun = TRUE;
	}
	else if(Tune < 100)
	{
	    str = "--%d--";
	    ID3_CTRL_TRK.AutoRun = TRUE;
	}
	else if(Tune < 1000)
	{
	    str = "-%d---";
	    ID3_CTRL_TRK.AutoRun = TRUE;
	}
	else
	{
	    str = "%d----";
	    ID3_CTRL_TRK.AutoRun = TRUE;
	}
#elif defined(_PS329a_SPECIAL_DISPLAY_)
    if(!Tune)
    {
    	str = "TRK**** ";
    }
    else if(Tune < 10)
    {
    	str = "TRK***%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 100)
    {
    	str = "  TRK**%d  ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = "TRK*%d   ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else
    {
    	str = "TRK%d    ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }

#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(!Tune)
    {
    	str = "  TRK  ****  ";
    }
    else if(Tune < 10)
    {
    	str = "  TRK  ***%d  ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 100)
    {
    	str = "  TRK  **%d  ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = "  TRK  *%d  ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else
    {
    	str = "  TRK  %d  ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    if(!Tune)
    {
    	str = " TRK  **** ";
    }
    else if(Tune < 10)
    {
    	str = " TRK  ***%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 100)
    {
    	str = " TRK  **%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = " TRK  *%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else
    {
    	str = " TRK  %d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(!Tune)
    {
    	str = " TRK **** ";
    }
    else if(Tune < 10)
    {
    	str = " TRK ***%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 100)
    {
    	str = " TRK **%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = " TRK *%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else
    {
    	str = " TRK %d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(!Tune)
    {
    	str = "    TRK***";
    }
    else if(Tune < 10)
    {
    	str = "    TRK**%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else if(Tune < 100)
    {
    	str = "    TRK*%d  ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
    else
    {
    	str = "     TRK%d ";
    	ID3_CTRL_TRK.AutoRun = TRUE;
    }
#elif defined(_MR1901_SPECIAL_DISPLAY_)
	if(!Tune)
	{
		str = "TRK ----";
	}
	else if(Tune < 10)
	{
		str = "TRK ---%d";
		ID3_CTRL_TRK.AutoRun = TRUE;
	}
	else if(Tune < 100)
	{
		str = "TRK --%d";
		ID3_CTRL_TRK.AutoRun = TRUE;
	}
	else if(Tune < 1000)
	{
		str = "TRK -%d";
		ID3_CTRL_TRK.AutoRun = TRUE;
	}
	else
	{
		str = "TRK %d";
		ID3_CTRL_TRK.AutoRun = TRUE;
	}
#else
	if(ID3_CTRL_INFO.total < 1000)
	{
		if(!Tune)
		{
			str = "TRK ***";
		}
		else if(Tune < 10)
		{
			str = "TRK **%d";
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
		else if(Tune < 100)
		{
			str = "TRK *%d";
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
		else
		{
			str = "TRK %d";
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
	}
	else
	{
		if(!Tune)
		{
			str = "TRK ****";
		}
		else if(Tune < 10)
		{
			str = "TRK ***%d";
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
		else if(Tune < 100)
		{
			str = "TRK **%d";
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
		else if(Tune < 1000)
		{
			str = "TRK *%d";
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
		else
		{
			str = "TRK %d";
			ID3_CTRL_TRK.AutoRun = TRUE;
		}
	}
#endif
	sprintf((char *)disp_buf,str,Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 0);
}

static void Id3TrkSchBuntChangeDisp(uint16 Tune)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#else
	uint8 disp_buf[10];
#endif
	const char *str;

	Tune %= 10000;
#if defined(TD_ADD_LCD_NUM_4)
	if(Tune)
	{
	    if(Tune < 10)
	    {
	        str = "---%d";
	    }
	    else if(Tune < 100)
	    {
	        str = "--%d";
	    }
	    else if(Tune < 1000)
	    {
	        str = "-%d";
	    }
		else
		{
			str = "%d";
		}
	    ID3_CTRL_TRK.Change = TRUE;
	}
	else
	{
	    str = "----";
	    ID3_CTRL_TRK.Change = FALSE;
	}
#elif defined(_PS329a_SPECIAL_DISPLAY_)
    if(Tune)
    {
    	if(Tune < 10)
    	{
    		str = "TRK---%d ";
    	}
    	else if(Tune < 100)
    	{
    		str = "TRK--%d ";
    	}
    	else if(Tune < 1000)
    	{
    		str = "TRK-%d ";
    	}
    	else
    	{
    		str = "TRK%d ";
    	}
    	ID3_CTRL_TRK.Change = TRUE;
    }
    else
    {
    	str = "TRK**** ";
    	ID3_CTRL_TRK.Change = FALSE;
    }

#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(Tune)
    {
    	if(Tune < 10)
    	{
    		str = "  TRK  ---%d  ";
    	}
    	else if(Tune < 100)
    	{
    		str = "  TRK  --%d  ";
    	}
    	else if(Tune < 1000)
    	{
    		str = "  TRK  -%d  ";
    	}
    	else
    	{
    		str = "  TRK  %d  ";
    	}
    	ID3_CTRL_TRK.Change = TRUE;
    }
    else
    {
    	str = "  TRK  ****  ";
    	ID3_CTRL_TRK.Change = FALSE;
    }
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(Tune)
    {
    	if(Tune < 10)
    	{
    		str = "    TRK--%d ";
    	}
    	else if(Tune < 100)
    	{
    		str = "    TRK-%d ";
    	}
    	else
    	{
    		str = "    TRK%d ";
    	}
    	ID3_CTRL_TRK.Change = TRUE;
    }
    else
    {
    	str = "    TRK***";
    	ID3_CTRL_TRK.Change = FALSE;
    }
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    if(Tune)
    {
    	if(Tune < 10)
    	{
    		str = " TRK  ---%d ";
    	}
    	else if(Tune < 100)
    	{
    		str = " TRK  --%d ";
    	}
    	else if(Tune < 1000)
    	{
    		str = " TRK  -%d ";
    	}
    	else
    	{
    		str = " TRK  %d ";
    	}
    	ID3_CTRL_TRK.Change = TRUE;
    }
    else
    {
    	str = "  TRK  ****	";
    	ID3_CTRL_TRK.Change = FALSE;
    }
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(Tune)
    {
    	if(Tune < 10)
    	{
    		str = " TRK ---%d ";
    	}
    	else if(Tune < 100)
    	{
    		str = " TRK --%d ";
    	}
    	else if(Tune < 1000)
    	{
    		str = " TRK -%d ";
    	}
    	else
    	{
    		str = " TRK %d ";
    	}
    	ID3_CTRL_TRK.Change = TRUE;
    }
    else
    {
    	str = "  TRK ****	";
    	ID3_CTRL_TRK.Change = FALSE;
    }
#elif defined(_MR1901_SPECIAL_DISPLAY_)
    if(Tune)
    {
    	if(Tune < 10)
    	{
    		str = "TRK ---%d";
    	}
    	else if(Tune < 100)
    	{
    		str = "TRK --%d";
    	}
    	else if(Tune < 1000)
    	{
    		str = "TRK -%d";
    	}
    	else
    	{
    		str = "TRK %d";
    	}
    	ID3_CTRL_TRK.Change = TRUE;
    }
    else
    {
    	str = "TRK ----";
    	ID3_CTRL_TRK.Change = FALSE;
    }
#else
	if(ID3_CTRL_INFO.total < 1000)
	{
		if(Tune)
		{
			if(Tune < 10)
			{
				str = "TRK --%d";
			}
			else if(Tune < 100)
			{
				str = "TRK -%d";
			}
			else
			{
				str = "TRK %d";
			}
			ID3_CTRL_TRK.Change = TRUE;
		}
		else
		{
			str = "TRK ***";
			ID3_CTRL_TRK.Change = FALSE;
		}
	}
	else
	{
		if(Tune)
		{
			if(Tune < 10)
			{
				str = "TRK ---%d";
			}
			else if(Tune < 100)
			{
				str = "TRK --%d";
			}
			else if(Tune < 1000)
			{
				str = "TRK -%d";
			}
			else
			{
				str = "TRK %d";
			}
			ID3_CTRL_TRK.Change = TRUE;
		}
		else
		{
			str = "TRK ****";
			ID3_CTRL_TRK.Change = FALSE;
		}
	}
#endif
	sprintf((char *)disp_buf,str,Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, ID3_CTRL_TRK.Change?1500:0);
}

static void Id3TrkSchBuntChangeDisp2(uint16 Tune)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#else
	uint8 disp_buf[10];
#endif
	const char *str;

	Tune %= 10000;
#if defined(TD_ADD_LCD_NUM_4)
	if(Tune < 10)
	{
	    str = "--%d-";
	    ID3_CTRL_TRK.Next = TRUE;
	}
	else if(Tune < 100)
	{
	    str = "-%d-";
	    ID3_CTRL_TRK.Next = TRUE;
	}
	else if(Tune < 1000)
	{
	    str = "%d-";
	    ID3_CTRL_TRK.Next = TRUE;
	}
	else
	{
	    str = "%d";
	    ID3_CTRL_TRK.Next = FALSE;
	}
#elif defined(_PS329a_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	str = "TRK--%d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 100)
    {
    	str = "TRK-%d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = "TRK%d*  ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else
    {
    	str = "TRK%4d  ";
    	ID3_CTRL_TRK.Next = FALSE;
    }

#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	str = "  TRK  --%d*  ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 100)
    {
    	str = "  TRK  -%d*  ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = "  TRK  %d*  ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else
    {
    	str = "  TRK  %4d  ";
    	ID3_CTRL_TRK.Next = FALSE;
    }
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	str = "    TRK-%d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 100)
    {
    	str = "    TRK-%d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else
    {
    	str = "    TRK%3d ";
    	ID3_CTRL_TRK.Next = FALSE;
    }
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	str = " TRK  --%d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 100)
    {
    	str = " TRK  -%d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = " TRK  %d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else
    {
    	str = " TRK  %4d ";
    	ID3_CTRL_TRK.Next = FALSE;
    }
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	str = " TRK --%d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 100)
    {
    	str = " TRK -%d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = " TRK %d* ";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else
    {
    	str = " TRK %4d ";
    	ID3_CTRL_TRK.Next = FALSE;
    }
#elif defined(_MR1901_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	str = "TRK --%d-";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 100)
    {
    	str = "TRK -%d-";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else if(Tune < 1000)
    {
    	str = "TRK %d-";
    	ID3_CTRL_TRK.Next = TRUE;
    }
    else
    {
    	str = "TRK %4d";
    	ID3_CTRL_TRK.Next = FALSE;
    }
#else
	if(ID3_CTRL_INFO.total < 1000)
	{
		if(Tune < 10)
		{
			str = "TRK -%d*";
			ID3_CTRL_TRK.Next = TRUE;
		}
		else if(Tune < 100)
		{
			str = "TRK %d*";
			ID3_CTRL_TRK.Next = TRUE;
		}
		else
		{
			str = "TRK %d";
			ID3_CTRL_TRK.Next = FALSE;
		}
	}
	else
	{
		if(Tune < 10)
		{
			str = "TRK --%d*";
			ID3_CTRL_TRK.Next = TRUE;
		}
		else if(Tune < 100)
		{
			str = "TRK -%d*";
			ID3_CTRL_TRK.Next = TRUE;
		}
		else if(Tune < 1000)
		{
			str = "TRK %d*";
			ID3_CTRL_TRK.Next = TRUE;
		}
		else
		{
			str = "TRK %4d";
			ID3_CTRL_TRK.Next = FALSE;
		}
	}
#endif
	sprintf((char *)disp_buf, str, Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 500);
}

static void Id3TrkSchBuntChangeFlash(uint16 Tune)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 disp_buf[12];
#else
	uint8 disp_buf[10];
#endif
	const char *str;

	Tune %= 10000;
#if defined(TD_ADD_LCD_NUM_4)
	if(Tune < 10)
	{
	    if(ID3_CTRL_TRK.Flash)
	    {
	        str = "--%d-";
	    }
	    else
	    {
	        str = "--%d ";
	    }
	}
	else if(Tune <100)
	{
	    if(ID3_CTRL_TRK.Flash)
	    {
	        str = "-%d-";
	    }
	    else
	    {
	        str = "-%d ";
	    }
	}
	else if(Tune <1000)
	{
	    if(ID3_CTRL_TRK.Flash)
	    {
	        str = "%d-";
	    }
	    else
	    {
	        str = "%d ";
	    }
	}
	else
	{
	    if(ID3_CTRL_TRK.Flash)
	    {
	        str = "%d";
	    }
	    else
	    {
	        str = "%d ";
	        Tune/=10;
	    }
	}
#elif defined(_PS329a_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "TRK--%d* ";
    	}
    	else
    	{
    		str = "TRK--%d  ";
    	}
    }
    else if(Tune <100)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "TRK-%d* ";
    	}
    	else
    	{
    		str = "TRK-%d  ";
    	}
    }
    else if(Tune <1000)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "TRK%d* ";
    	}
    	else
    	{
    		str = "TRK%d  ";
    	}
    }
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "  TRK  --%d*  ";
    	}
    	else
    	{
    		str = "  TRK  --%d   ";
    	}
    }
    else if(Tune <100)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "  TRK  -%d*  ";
    	}
    	else
    	{
    		str = "  TRK  -%d   ";
    	}
    }
    else if(Tune <1000)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "  TRK  %d*  ";
    	}
    	else
    	{
    		str = "  TRK  %d   ";
    	}
    }
    else
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "  TRK  %d  ";

    	}
    	else
    	{
    		str = "  TRK  %d   ";
    		Tune/=10;
    	}
    }
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	if(Tune < 10)
	{
		if(ID3_CTRL_TRK.Flash)
		{
			str = "    TRK-%d*";
		}
		else
		{
			str = "    TRK-%d ";
		}
	}
	else if(Tune <100)
	{
		if(ID3_CTRL_TRK.Flash)
		{
			str = "    TRK%d*";
		}
		else
		{
			str = "    TRK%d ";
		}
	}
	else
	{
		if(ID3_CTRL_TRK.Flash)
		{
			str = "    TRK%d";
		}
		else
		{
			str = "    TRK%d ";
			Tune/=10;
		}
	}
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = " TRK  --%d* ";
    	}
    	else
    	{
    		str = " TRK  --%d  ";
    	}
    }
    else if(Tune <100)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = " TRK  -%d* ";
    	}
    	else
    	{
    		str = " TRK  -%d  ";
    	}
    }
    else if(Tune <1000)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = " TRK  %d* ";
    	}
    	else
    	{
    		str = " TRK  %d   ";
    	}
    }
    else
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = " TRK  %d ";

    	}
    	else
    	{
    		str = " TRK  %d  ";
    		Tune/=10;
    	}
    }
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = " TRK --%d* ";
    	}
    	else
    	{
    		str = " TRK --%d  ";
    	}
    }
    else if(Tune <100)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = " TRK -%d* ";
    	}
    	else
    	{
    		str = " TRK -%d  ";
    	}
    }
    else if(Tune <1000)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = " TRK %d* ";
    	}
    	else
    	{
    		str = " TRK %d   ";
    	}
    }
    else
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = " TRK %d ";
    
    	}
    	else
    	{
    		str = " TRK %d  ";
    		Tune/=10;
    	}
    }
#elif defined(_MR1901_SPECIAL_DISPLAY_)
    if(Tune < 10)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "TRK --%d-";
    	}
    	else
    	{
    		str = "TRK --%d ";
    	}
    }
    else if(Tune <100)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "TRK -%d-";
    	}
    	else
    	{
    		str = "TRK -%d ";
    	}
    }
    else if(Tune <1000)
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "TRK %d-";
    	}
    	else
    	{
    		str = "TRK %d ";
    	}
    }
    else
    {
    	if(ID3_CTRL_TRK.Flash)
    	{
    		str = "TRK %d";

    	}
    	else
    	{
    		str = "TRK %d ";
    		Tune/=10;
    	}
    }
#else
	if(ID3_CTRL_INFO.total < 1000)
	{
		if(Tune < 10)
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK -%d*";
			}
			else
			{
				str = "TRK -%d ";
			}
		}
		else if(Tune <100)
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK %d*";
			}
			else
			{
				str = "TRK %d ";
			}
		}
		else
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK %d";
			}
			else
			{
				str = "TRK %d ";
				Tune/=10;
			}
		}
	}
	else
	{
		if(Tune < 10)
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK --%d*";
			}
			else
			{
				str = "TRK --%d ";
			}
		}
		else if(Tune <100)
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK -%d*";
			}
			else
			{
				str = "TRK -%d ";
			}
		}
		else if(Tune <1000)
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK %d*";
			}
			else
			{
				str = "TRK %d ";
			}
		}
		else
		{
			if(ID3_CTRL_TRK.Flash)
			{
				str = "TRK %d";

			}
			else
			{
				str = "TRK %d ";
				Tune/=10;
			}
		}
	}
#endif
	ID3_CTRL_TRK.Flash = !ID3_CTRL_TRK.Flash;
	sprintf((char *)disp_buf, str, Tune);
	VdfDispString(disp_buf, VFD_DISP_TYPE_TIME, 500);
}
#endif

static uint8 Id3StatusTrkSchInit(void)
{
	memset(&ID3_CTRL_TRK,0,sizeof(ID3_CTRL_TRK));
	ID3_CTRL_TRK.Status = ID3_TRK_SCH_STATUS_TITLE;
	ID3_CTRL_TRK.Mode = ID3_TRK_SCH_INPUT_BUNT;
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
#if defined(_WLS1403_COPY_DISPLAY_)
	VdfDispString("SCH ",VFD_DISP_TYPE_TIME,1000);
#else
	VdfDispString(" SCH",VFD_DISP_TYPE_TIME,1000);
#endif
#elif defined(_PS329a_SPECIAL_DISPLAY_)
	VdfDispString("TRK SCH",VFD_DISP_TYPE_TIME,1000);
#elif defined(_FY7000_SPECIAL_DISPLAY_)
	VdfDispString("   TRK  SCH  ",VFD_DISP_TYPE_TIME,1000);
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	VdfDispString("    TRKSCH",VFD_DISP_TYPE_TIME,1000);
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	VdfDispString(" TRK  SCH  ",VFD_DISP_TYPE_TIME,1000);
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	VdfDispString(" TRK  SCH ",VFD_DISP_TYPE_TIME,1000);
#elif defined(_FIVE_CHAR_DISPLAY_)
#if defined(SPECIAL_SLA6799_DISP)
    VdfDispString("TRK--- ",VFD_DISP_TYPE_TIME,1000);
#else
    VdfDispString("TRKSCH",VFD_DISP_TYPE_TIME,1000);
#endif
#elif defined(_WLS316_SPECIAL_DISPLAY_)
	VdfDispString("SEARCH ",VFD_DISP_TYPE_TIME,1000);
#else
	VdfDispString("TRK SCH",VFD_DISP_TYPE_TIME,1000);
#endif
	os_taskq_post_msg(MUSIC_TASK_NAME, 2, MSG_MUSIC_DISP, 1);
	return TRUE;
}

static uint8 Id3StatusTrkSchRun(void)
{
	uint16 	Tune;
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8	disp_buf[15];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8	disp_buf[13];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8	disp_buf[12];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8	disp_buf[12];
#else
	uint8 	disp_buf[10];
#endif

	switch(ID3_CTRL_TRK.Status)
	{
		case ID3_TRK_SCH_STATUS_TITLE:
			if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
			{
				ID3_CTRL_TRK.Status = ID3_TRK_SCH_STATUS_INPUT;
				ID3_CTRL_TRK.Tune = 0;
				ID3_CTRL_TRK.Change = FALSE;
				ID3_CTRL_TRK.Next = FALSE;
				ID3_CTRL_TRK.OverTime = 0;
				ID3_CTRL_TRK.AutoRun = FALSE;
			#if defined(TD_ADD_LCD_NUM_4)
				VdfDispString("----",VFD_DISP_TYPE_TIME,0);
			#elif defined(_PS329a_SPECIAL_DISPLAY_)
    			VdfDispString("TRK****",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY7000_SPECIAL_DISPLAY_)
    			VdfDispString("  TRK  ****  ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				VdfDispString(" TRK  **** ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				VdfDispString("    TRK*** ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				VdfDispString(" TRK **** ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FIVE_CHAR_DISPLAY_)
			#if defined(SPECIAL_SLA6799_DISP)
    			VdfDispString("TRK--- ",VFD_DISP_TYPE_TIME,0);
			#else
    			VdfDispString("T ***",VFD_DISP_TYPE_TIME,0);
			#endif
			#elif defined(_MR1901_SPECIAL_DISPLAY_)
				VdfDispString("TRK ----",VFD_DISP_TYPE_TIME,0);
			#elif defined(_WLS316_SPECIAL_DISPLAY_)
				VdfDispString("TRK ----",VFD_DISP_TYPE_TIME,0);
			#else
				if(ID3_CTRL_INFO.total < 1000)
				{
					VdfDispString("TRK ***",VFD_DISP_TYPE_TIME,0);
				}
				else
				{
					VdfDispString("TRK ****",VFD_DISP_TYPE_TIME,0);
				}
			#endif
				memset(&play_disp->input, 0, sizeof(play_disp->input));
			}
			break;
		case ID3_TRK_SCH_STATUS_INPUT:
			Tune = ID3_CTRL_TRK.Tune;

			if(play_disp->input.sum)
			{
				while(play_disp->input.sum)
				{
				#if defined(SPECIAL_AMS_TRK)
					if(play_disp->input.sum > 0)
					{
						if((Tune < ID3_CTRL_INFO.total) && (Tune < 9999))Tune ++;
						else Tune = 1;
						play_disp->input.sum --;
					}
					else if(play_disp->input.sum < 0)
					{
						if(Tune > 1)Tune --;
						else
						{
							Tune = ID3_CTRL_INFO.total;
							if(Tune > 9999)Tune = 9999;
						}
						play_disp->input.sum ++;
					}
				#else
					if(play_disp->input.sum > 0)
					{
						if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_INC)
						{
							if((Tune < ID3_CTRL_INFO.total) && (Tune < 9999))Tune ++;
							else Tune = 1;
						}
						else if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_BUNT)
						{
							if(ID3_CTRL_TRK.Next == TRUE)
							{
								Tune *= 10;
								ID3_CTRL_TRK.Next = FALSE;
							}
							else
							{
								if((Tune % 10) < 9)Tune ++;
								else Tune -= 9;
							}
							if(!Tune)Tune = 1;
						}
						play_disp->input.sum --;
					}
					else if(play_disp->input.sum < 0)
					{
						if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_INC)
						{
							if(Tune > 1)Tune --;
							else
							{
								Tune = ID3_CTRL_INFO.total;
								if(Tune > 9999)Tune = 9999;
							}
						}
						else if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_BUNT)
						{
							if(ID3_CTRL_TRK.Next == TRUE)
							{
								Tune *= 10;
								Tune += 9;
								ID3_CTRL_TRK.Next = FALSE;
							}
							else
							{
								if(Tune % 10)Tune --;
								else Tune += 9;
							}

							if(!Tune)Tune = 9;
						}
						play_disp->input.sum ++;
					}
				#endif
				}
			}
			else if((play_disp->input.cmd >= PLAY_CMD_0) && (play_disp->input.cmd <= PLAY_CMD_9))
			{
				uint8 Number = play_disp->input.cmd - PLAY_CMD_0;

			#if defined(SPECIAL_AMS_TRK)
				if(ID3_CTRL_INFO.total < 1000)
				{
					if(Tune < 100)
					{
						Tune *=  10;
						Tune +=  Number;
						Tune %=  10000;
					}
				}
				else
				{
					if(Tune < 1000)
					{
						Tune *=  10;
						Tune +=  Number;
						Tune %=  10000;
					}
				}
			#else
				if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_INC)
				{
					Tune *=  10;
					Tune +=  Number;
					Tune %=  10000;
				}
				else if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_BUNT)
				{
					if(ID3_CTRL_TRK.Next == TRUE)
					{
						Tune *= 10;
						Tune += Number;
						ID3_CTRL_TRK.Next = FALSE;
					}
					else
					{
						Tune -= (Tune % 10);
						Tune +=  Number;
					}
				}
			#endif
				play_disp->input.cmd= PLAY_CMD_NULL;
			}
			else if(play_disp->input.cmd == PLAY_CMD_DELETE)
			{
				if(Tune)
				{
					Tune /= 10;
				}
				play_disp->input.cmd = PLAY_CMD_NULL;
			}
			else if(play_disp->input.cmd == PLAY_CMD_ENTER)
			{
				if(!ID3_CTRL_TRK.Tune)
				{
				#if defined(_FY7000_SPECIAL_DISPLAY_)
					VdfDispString("    Null     ",VFD_DISP_TYPE_TIME,2000);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					VdfDispString("   Null    ",VFD_DISP_TYPE_TIME,2000);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					VdfDispString("    Null  ",VFD_DISP_TYPE_TIME,2000);
				#elif defined(_PS329a_SPECIAL_DISPLAY_)
			    	VdfDispString("   Null ",VFD_DISP_TYPE_TIME,2000);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					VdfDispString("   Null   ",VFD_DISP_TYPE_TIME,2000);
				#else
			    	VdfDispString("Null",VFD_DISP_TYPE_TIME,2000);
				#endif
					ID3_CTRL_TRK.Status = ID3_TRK_SCH_STATUS_ERROR;
				}
				else if(ID3_CTRL_TRK.Tune > ID3_CTRL_INFO.total)
				{
				#if defined(TD_ADD_LCD_NUM_4)
				#if defined(_WLS1403_COPY_DISPLAY_)
    				if(ID3_CTRL_INFO.total < 1000)
    				{
    					sprintf((char *)disp_buf, " %03d", ID3_CTRL_INFO.total);
    				}
    				else
    				{
						sprintf((char *)disp_buf, "%04d", ID3_CTRL_INFO.total);
    				}
				#else
                    sprintf((char *)disp_buf, "%04d", ID3_CTRL_INFO.total);
				#endif
				#elif defined(_FY7000_SPECIAL_DISPLAY_)
					sprintf((char *)disp_buf, "  Max  %04d  ", ID3_CTRL_INFO.total%10000);
				#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
					sprintf((char *)disp_buf, "    Max%03d ", ID3_CTRL_INFO.total);
				#elif defined(_FY5517_SPECIAL_DISPLAY_)
					sprintf((char *)disp_buf, " Max  %04d ", ID3_CTRL_INFO.total%10000);
				#elif defined(_MR1901_SPECIAL_DISPLAY_)
					sprintf((char *)disp_buf, "Max %04d", ID3_CTRL_INFO.total%10000);
				#elif defined(_SL6812A_SPECIAL_DISPLAY_)
					sprintf((char *)disp_buf, " Max %04d ", ID3_CTRL_INFO.total%10000);
				#else
					if(ID3_CTRL_INFO.total < 1000)
					{
					#if defined(_FIVE_CHAR_DISPLAY_)
						sprintf((char *)disp_buf, "Max%03d", ID3_CTRL_INFO.total);
					#else
						sprintf((char *)disp_buf, "Max %03d", ID3_CTRL_INFO.total);
					#endif
					}
					else
					{
					#if defined(_FIVE_CHAR_DISPLAY_)
						sprintf((char *)disp_buf, "M %04d", ID3_CTRL_INFO.total%10000);
					#elif defined(_PS329a_SPECIAL_DISPLAY_)&&defined(_SEVEN_CHAR_DISPLAY_)
						sprintf((char *)disp_buf, "Max%03d", ID3_CTRL_INFO.total);
					#elif defined(_SEVEN_CHAR_DISPLAY_)
						sprintf((char *)disp_buf, "Max %03d", ID3_CTRL_INFO.total);
					#else
						sprintf((char *)disp_buf, "Max %04d", ID3_CTRL_INFO.total%10000);
					#endif
					}
				#endif
					VdfDispString(disp_buf,VFD_DISP_TYPE_TIME,2000);
					ID3_CTRL_TRK.Status = ID3_TRK_SCH_STATUS_ERROR;
				}
				else
				{
					if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_BUNT)
					{
						if(!ID3_CTRL_TRK.Change)
						{
							if(ID3_CTRL_TRK.Flash)
							{
								Id3TrkSchBuntChangeFlash(ID3_CTRL_TRK.Tune);
							}
						}
					}
					ID3_CTRL_TRK.Status = ID3_TRK_SCH_STATUS_SEARCH;
				}
				ID3_CTRL_TRK.OverTime = 0;
				play_disp->input.cmd = PLAY_CMD_NULL;
			}

		#if !defined(SPECIAL_AMS_TRK)
			if(ID3_CTRL_TRK.AutoRun)
			{
				if(ID3_CTRL_TRK.OverTime > ID3_SCH_AUTO_RUN_TIME)
				{
					ID3_CTRL_TRK.AutoRun = FALSE;
					play_disp->input.cmd = PLAY_CMD_ENTER;
				}
			}
		#endif

			if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_INC)
			{
				if(Tune != ID3_CTRL_TRK.Tune)
				{
					ID3_CTRL_TRK.Tune = Tune;
					ID3_CTRL_TRK.OverTime = 0;
				#if !defined(SPECIAL_AMS_TRK)
					Id3TrkSchIncChangeDisp(Tune);
				#endif
				#if defined(_USER_TFT_SEND_)
					TFT_Tx_Music_Search(Tune,music_cur_filetotal);
				#endif
				}
			}
			else if(ID3_CTRL_TRK.Mode == ID3_TRK_SCH_INPUT_BUNT)
			{
				if(Tune != ID3_CTRL_TRK.Tune)
				{
					ID3_CTRL_TRK.Tune = Tune;
					ID3_CTRL_TRK.OverTime = 0;
					Id3TrkSchBuntChangeDisp(Tune);
				}
				else
				{
				#if !defined(SPECIAL_AMS_TRK)
					if(ID3_CTRL_TRK.Change)
					{
						if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
						{
							Id3TrkSchBuntChangeDisp2(Tune);
							ID3_CTRL_TRK.Change = FALSE;
							ID3_CTRL_TRK.Flash = FALSE;
						}
					}
					else
					{
						if(GET_VFD_TYPE() == VFD_DISP_TYPE_NULL)
						{
							Id3TrkSchBuntChangeFlash(Tune);
						}
					}
				#endif
				}
			}
			break;
		case ID3_TRK_SCH_STATUS_ERROR:
			if((GET_VFD_TYPE() == VFD_DISP_TYPE_NULL) || (play_disp->input.cmd == PLAY_CMD_ENTER))
			{
				ID3_CTRL_TRK.Status = ID3_TRK_SCH_STATUS_INPUT;
				ID3_CTRL_TRK.Tune = 0;
				ID3_CTRL_TRK.Change = FALSE;
				ID3_CTRL_TRK.Next = FALSE;
				ID3_CTRL_TRK.OverTime = 0;
				ID3_CTRL_TRK.AutoRun = FALSE;
				memset(&play_disp->input, 0, sizeof(play_disp->input));
			#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
				VdfDispString("----",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY7000_SPECIAL_DISPLAY_)
				VdfDispString("  TRK  ****  ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				VdfDispString(" TRK  **** ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				VdfDispString(" TRK **** ",VFD_DISP_TYPE_TIME,0);
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				VdfDispString("    TRK***",VFD_DISP_TYPE_TIME,0);
			#elif defined(_FIVE_CHAR_DISPLAY_)
			#if defined(SPECIAL_SLA6799_DISP)
			    VdfDispString("TRK--- ",VFD_DISP_TYPE_TIME,0);
			#else
    			VdfDispString("T ***",VFD_DISP_TYPE_TIME,0);
			#endif
			#elif defined(_MR1901_SPECIAL_DISPLAY_)
				VdfDispString("TRK ----",VFD_DISP_TYPE_TIME,0);
			#elif defined(_SEVEN_CHAR_DISPLAY_)
			#if defined(_FY2010_LCD_DISPLAY_)
				VdfDispString("TRK ---",VFD_DISP_TYPE_TIME,0);
			#else
				VdfDispString("TRK ***",VFD_DISP_TYPE_TIME,0);
			#endif
			#else
				if(ID3_CTRL_INFO.total < 1000)
				{
					VdfDispString("TRK ***",VFD_DISP_TYPE_TIME,0);
				}
				else
				{
					VdfDispString("TRK ****",VFD_DISP_TYPE_TIME,0);
				}
			#endif
			}
			break;
		case ID3_TRK_SCH_STATUS_SEARCH:
			os_taskq_post_msg(MUSIC_TASK_NAME, 2, MSG_MUSIC_TRK, ID3_CTRL_TRK.Tune);
			ID3_CTRL_TRK.Status = ID3_TRK_SCH_STATUS_END;
			break;
		case ID3_TRK_SCH_STATUS_END:
			ID3_ID_NEXT = ID3_STATUS_DISP;
			break;
	}

	if(ID3_CTRL_TRK.OverTime < ID3_SCH_OVER_TIME)ID3_CTRL_TRK.OverTime ++;
	else
	{
		ID3_ID_NEXT = ID3_STATUS_DISP;
    #if defined(PICK_SONG_AUTO_ENTER)
    	if((ID3_CTRL_TRK.Tune > 0)&&(ID3_CTRL_TRK.Tune <= ID3_CTRL_INFO.total))
    	{
    		os_taskq_post_msg("MusicTask", 2, MSG_MUSIC_TRK, ID3_CTRL_TRK.Tune);
    	}
    #endif
	}
	return TRUE;
}

static uint8 Id3StatusTrkSchExit(void)
{
	os_taskq_post_msg(MUSIC_TASK_NAME, 2, MSG_MUSIC_DISP, 0);
	memset(&ID3_CTRL_TRK,0,sizeof(ID3_CTRL_TRK));
	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

typedef uint8 (*FUNC_ID3_MODE_INIT)(void);
typedef uint8 (*FUNC_ID3_MODE_RUN)(void);
typedef uint8 (*FUNC_ID3_MODE_EXIT)(void);

typedef struct{
	FUNC_ID3_MODE_INIT  Id3ModeInit;
	FUNC_ID3_MODE_RUN  	Id3ModeRun;
	FUNC_ID3_MODE_EXIT 	Id3ModeExit;
}ID3_MODE;

static const ID3_MODE Id3Mode[]=
{
	{Id3StatusInitInit,		Id3StatusInitRun,		Id3StatusInitExit		},
	{Id3StatusDispInit,		Id3StatusDispRun,		Id3StatusDispExit		},
	{Id3StatusTrkSchInit,	Id3StatusTrkSchRun,		Id3StatusTrkSchExit		},
#if defined(SUPPORT_NUM_SELECT_MUSIC)
	{Id3SelectMusicInit,	Id3SelectMusicRun,		Id3SelectMusicExit		},
#endif
};

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 PlayDelSuperpose(void)
{
	if(play_disp == NULL)return FALSE;

	if(ID3_ID == ID3_STATUS_TRK_SCH)
	{
		switch(ID3_CTRL_TRK.Status)
		{
			case ID3_TRK_SCH_STATUS_TITLE:
				return FALSE;
			case ID3_TRK_SCH_STATUS_INPUT:
				if(ID3_CTRL_TRK.Tune)return TRUE;
				else return FALSE;
				break;
			case ID3_TRK_SCH_STATUS_ERROR:
			case ID3_TRK_SCH_STATUS_SEARCH:
			case ID3_TRK_SCH_STATUS_END:
				return TRUE;
		}
	}
	return FALSE;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

static void ui_show_play_clean(void)
{
	play_id3_icon_disp(0xFF);
	play_mode_icon_disp(0xFF);
	play_media_icon_disp(0xFF);

	show_clear_icon(DISP_LEVEL_NORMAL);
	show_clear_num(DISP_LEVEL_NORMAL);

#if defined(_ENABLE_DISK_DISC_TO_FLASH_)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
#endif
    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAY);
    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PAUSE);
#if defined(_ENABLE_PLAYER_ICON_)
    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAYER);
#endif
#if defined(_FY5517_SPECIAL_DISPLAY_)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK2);
#endif
}

void ui_show_play_init(uint8 source)
{
	play_disp = (play_disp_t *)malloc_fun(play_disp ,sizeof(play_disp_t), 0);
	if(play_disp == NULL)
	{
		ui_puts("ui_show_play_init err\n");
        while(1);
	}

	ui_show_play_clean();

	ID3_ID_SOURCE = source;

	if(ID3_ID_SOURCE == PLAY_SOURCE_USB)
	{
        music_cur_filetotal =0;
	#if !defined(_SUPPORT_LCM_1618_DRIVE)
		ui_show_setting(LANG_ID_MUSIC_USB, 0);
	#endif
		ui_disp.timer = 1000;
		ui_show_normal(LANG_ID_MUSIC_USB);
	#if defined(USB_SD_MODE_WORK_ONOFF)||defined(_DISABLE_USB_FLASH_)||defined(_DISABLE_USB_BLINK_)
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
	#else
	    ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_USB);
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_FLASH);
	#endif
	#if defined(_FY7555_LCD_DISPLAY_)
    	ui_show_setting1(LANG_ID_MUSIC_USB, 0);
    	ui_show_normal1(LANG_ID_MUSIC_USB);
	#endif
	}
	else if(ID3_ID_SOURCE == PLAY_SOURCE_SD)
	{
        music_cur_filetotal =0;
	#if !defined(_SUPPORT_LCM_1618_DRIVE)
		ui_show_setting(LANG_ID_MUSIC_SD, 0);
	#endif
		ui_disp.timer = 1000;
		ui_show_normal(LANG_ID_MUSIC_SD);
	#if defined(USB_SD_MODE_WORK_ONOFF)||defined(_DISABLE_USB_BLINK_)
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SD);
	#else
    	ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_SD);
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_FLASH);
	#endif
    #if defined(_FY7555_LCD_DISPLAY_)
    	ui_show_setting1(LANG_ID_MUSIC_SD, 0);
    	ui_show_normal1(LANG_ID_MUSIC_SD);
    #endif
	}
#if defined(_ENABLE_PLAYER_ICON_)
    ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PLAYER);
#endif

	VfdDispInit();
}

 uint8 play_usb_state(void)
{
	if(file_operate_get_total_phydev())
	{
		if(dev_get_online_status(usb))
		{
			return TRUE;
		}
	}
	return FALSE;
}

 uint8 play_sd_state(void)
{
	if(file_operate_get_total_phydev())
	{
		if(dev_get_online_status(sd1))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void ui_show_play_exit(void)
{
	ui_puts("ui_show_play_exit\n");

	ui_show_play_clean();

	if(play_disp != NULL)
	{
		if(ID3_ID_SOURCE == PLAY_SOURCE_USB)
		{
			//ew_printf("usb_state:%d\n",play_usb_state());
			if(play_usb_state())
			{
		    #if defined(USB_SD_MODE_WORK_ONOFF)
		    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_USB);
			#elif defined(_DISABLE_USB_FLASH_)
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
			#elif defined(_EVB_USB_CHANGE_MODE_)
				if(global.charge_flag)
				{
					ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_USB);
				}
				else
				{
					ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
				}
		    	ui_drv_func.flash_disable(UI_ICON_USB);
		    	ui_drv_func.flash_disable(UI_ICON_FLASH);
		    #else
		    	ui_drv_func.flash_disable(UI_ICON_USB);
		    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
		    	ui_drv_func.flash_disable(UI_ICON_FLASH);
		    #endif
			}
		}
		else if(ID3_ID_SOURCE == PLAY_SOURCE_SD)
		{
			if(play_sd_state())
			{
		    #if defined(USB_SD_MODE_WORK_ONOFF)
		    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SD);
		    #else
		    	ui_drv_func.flash_disable(UI_ICON_SD);
		    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SD);
		    	ui_drv_func.flash_disable(UI_ICON_FLASH);
		    #endif
			}
		}
    #if defined(_ENABLE_CLOCK_DISPLAY_)
    	ui_disp.playflg = FALSE;
    #endif
		free_fun((void**)&play_disp);
	}
}

#if defined(_ENABLE_DISK_DISC_TO_FLASH_)
void DiscPolling(void)
{
#if defined(_ENABLE_PAUSE_DISC_DISPLAY_)
    if(ID3_CTRL_DISP.PlayState == PLAY_STATUS_PAUSE)
    {
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
	}
	else
#endif
    {
    	if(play_disp->DiscTime)play_disp->DiscTime--;
    	else
    	{
    		play_disp->DiscTime = 333/VFD_DISP_POLLING_TIME;
    		switch(play_disp->DiscState)
    		{
    			case 0:
    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
    				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R3);
    				play_disp->DiscState = 1;
    				break;
    			case 1:
    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
    				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R2);
    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
    				play_disp->DiscState = 2;
    				break;
    			case 2:
    				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_R1);
    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
    				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
    				play_disp->DiscState = 0;
    				break;
    		}
    	}
    }
}
#endif

void ui_show_play_polling(void)
{
	if(play_disp == NULL)return;
	uint8 Flag = FALSE;

	if(ID3_ID_NEXT != ID3_ID)
	{
		play_disp->Mode = ID3_MODE_EXIT;
	}

	switch(play_disp->Mode)
	{
		case ID3_MODE_INIT:
			Flag = Id3Mode[play_disp->Id].Id3ModeInit();
			if(Flag)
			{
				play_disp->Mode = ID3_MODE_RUN;
			}
			break;
		case ID3_MODE_RUN:
			Flag = Id3Mode[play_disp->Id].Id3ModeRun();
			break;
		case ID3_MODE_EXIT:
			Flag = Id3Mode[play_disp->Id].Id3ModeExit();
			if(Flag)
			{
				ID3_ID = ID3_ID_NEXT;
				play_disp->Mode = ID3_MODE_INIT;
			}
			break;
	}

	VfdDispPolling();

#if defined(_FIVE_CHAR_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
	if((ID3_CTRL_DISP.PlayState == PLAY_STATUS_FORWARD)
	|| (ID3_CTRL_DISP.PlayState == PLAY_STATUS_BACKWARD))
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_DASH);
	}
#endif

#if defined(_ENABLE_DISK_DISC_TO_FLASH_)
    DiscPolling();
#endif

}

#if defined(_YS_SPECIAL_DISP_)
static play_disp_info_t *media_disp = NULL;
#endif

void ui_show_play_play(void *info)
{
	if(play_disp == NULL)return;

	play_disp_info_t *play_disp_info = &play_disp->info;
	play_disp_info_t *disp_info = (play_disp_info_t *)info;

	if(play_disp_info->mode != disp_info->mode)
	{
		play_mode_icon_disp(disp_info->mode);
	}

#if defined(_SL6812_TEST_LCD_)
	if(global.lcd_mode_flag)
	{
		global.lcd_mode_flag = FALSE;
		play_mode_icon_disp(disp_info->mode);
		play_media_icon_disp(ID3_CTRL_INFO.media);
	}
#endif

#if defined(_YS_SPECIAL_DISP_)
	if((media_disp->media != disp_info->media)||
		((media_disp->media == PLAY_MEDIA_MP3)&&(disp_info->media == PLAY_MEDIA_MP3)))
	{
		media_disp->media = disp_info->media;

		ew_printf("media_disp->media:%d\r\n",media_disp->media);

		switch(media_disp->media)
		{
			case PLAY_MEDIA_MP3:
				play_media_icon_disp(PLAY_MEDIA_MP3);
				break;

			case PLAY_MEDIA_WMA:
				play_media_icon_disp(PLAY_MEDIA_WMA);
				break;

			case PLAY_MEDIA_WAV:
				play_media_icon_disp(PLAY_MEDIA_WAV);
				break;

			case PLAY_MEDIA_FLAC:
				play_media_icon_disp(PLAY_MEDIA_FLAC);
				break;

			case PLAY_MEDIA_APE:
				play_media_icon_disp(PLAY_MEDIA_APE);
				break;

			default:
				play_media_icon_disp(0xFF);
				break;
		}
	}
#endif

	if(disp_info->change)
	{
		play_disp->change_flag = TRUE;
	}

	*play_disp_info = *disp_info;
}

void ui_show_play_cmd(int cmd, void *param)
{
	if(play_disp == NULL)return;
	if(cmd < PLAY_CMD_MAX)
	{
		switch(cmd)
		{
			case PLAY_CMD_0 ... PLAY_CMD_9:
			case PLAY_CMD_DELETE:
			case PLAY_CMD_ENTER:
				{
					play_disp->input.cmd = cmd;
				}
				break;

			case PLAY_CMD_SEL_ADD:
				{
					play_disp->input.sum++;
				}
				break;

			case PLAY_CMD_SEL_SUB:
				{
					play_disp->input.sum--;
				}
				break;

			case PLAY_CMD_DIR_UP:
				{
					play_disp->input.dir++;
				}
				break;

			case PLAY_CMD_DIR_DOWN:
				{
					play_disp->input.dir--;
				}
				break;

			case PLAY_CMD_MODE:
				#if !defined(_YS_SPECIAL_DISP_)
				{
					uint8 mode = (uint8)param;

					switch(mode)
					{
						case PLAY_MODE_DISP_REPEAT_OFF:
						#if !defined(_XM003_SPECIAL_DISPLAY_)
						#if !defined(_XZC1668_SPECIAL_DISPLAY_)
							ui_show_setting(LANG_ID_MUSIC_MODE_REPEAT_OFF, 0);
						#endif
						#endif
							break;

						case PLAY_MODE_DISP_REPEAT_ALL:
						#if !defined(_XM003_SPECIAL_DISPLAY_)
						#if !defined(_XZC1668_SPECIAL_DISPLAY_)
							ui_show_setting(LANG_ID_MUSIC_MODE_REPEAT_ALL, 0);
						#endif
						#endif
							break;

						case PLAY_MODE_DISP_REPEAT_ONE:
						#if !defined(_XM003_SPECIAL_DISPLAY_)
						#if !defined(_XZC1668_SPECIAL_DISPLAY_)
							ui_show_setting(LANG_ID_MUSIC_MODE_REPEAT_ONE, 0);
						#endif
						#endif
							break;

						case PLAY_MODE_DISP_REPEAT_FOLDER:
						#if !defined(_XM003_SPECIAL_DISPLAY_)
						#if !defined(_XZC1668_SPECIAL_DISPLAY_)
							ui_show_setting(LANG_ID_MUSIC_MODE_REPEAT_FOLDER, 0);
						#endif
						#endif
							break;

						case PLAY_MODE_DISP_SHUFFLE_OFF:
						#if !defined(_XM003_SPECIAL_DISPLAY_)
						#if !defined(_XZC1668_SPECIAL_DISPLAY_)
							ui_show_setting(LANG_ID_MUSIC_MODE_RDM_OFF, 0);
						#endif
						#endif
							break;

						case PLAY_MODE_DISP_SHUFFLE_ON:
						#if !defined(_XM003_SPECIAL_DISPLAY_)
						#if !defined(_XZC1668_SPECIAL_DISPLAY_)
							ui_show_setting(LANG_ID_MUSIC_MODE_RDM_ON, 0);
						#endif
						#endif
							break;

						case PLAY_MODE_DISP_INTRO_OFF:
						#if !defined(_XM003_SPECIAL_DISPLAY_)
						#if !defined(_XZC1668_SPECIAL_DISPLAY_)
							ui_show_setting(LANG_ID_MUSIC_MODE_INT_OFF, 0);
						#endif
						#endif
						#if defined(_ACC_OFF_MEMORY_INT_)
						    setup.int_flag = FALSE;
						#endif
							break;

						case PLAY_MODE_DISP_INTRO_ON:
						#if !defined(_XM003_SPECIAL_DISPLAY_)
						#if !defined(_XZC1668_SPECIAL_DISPLAY_)
							ui_show_setting(LANG_ID_MUSIC_MODE_INT_ON, 0);
						#endif
						#endif
							break;

					}
				}
				#endif
				break;

			case PLAY_CMD_SEARCH_DELETE:
				if(PlayDelSuperpose())
				{
					play_disp->input.cmd = PLAY_CMD_DELETE;
					break;
				}

			case PLAY_CMD_SEARCH:
				{
					if((ID3_ID == ID3_STATUS_DISP)&&(ID3_ID_NEXT == ID3_STATUS_DISP))
					{
						ID3_ID_NEXT = ID3_STATUS_TRK_SCH;
					}
					else if((ID3_ID == ID3_STATUS_TRK_SCH)&&(ID3_ID_NEXT == ID3_STATUS_TRK_SCH))
					{
						ID3_ID_NEXT = ID3_STATUS_DISP;
					}
				}
				break;

			case PLAY_CMD_ID3:
				{
					if((ID3_ID == ID3_STATUS_DISP)
					  &&(ID3_ID_NEXT == ID3_STATUS_DISP)
					  &&(play_disp->Mode = ID3_MODE_RUN))
					{
						if(ID3_CTRL_INFO.state == PLAY_STATUS_PLAYE)
						{
							ID3_CTRL_DISP.Change = ID3_DISP_STATUS_CAHNGE | ID3_DISP_STATUS_CAHNGE_CYCLE;
						}
					}
				}
				break;
		}
	}
}

#if defined(_SUPPORT_LCM_DRIVE)
void Lcm_disp_id3(void)
{
    if(Get_id3Info_err== TRUE)
    {
        lLcdStr[0]='<';
        lLcdStr[1]='N';
        lLcdStr[2]='U';
        lLcdStr[3]='L';
        lLcdStr[4]='L';
        lLcdStr[5]='>';
		Lcm_Disp_8_16(2,40,lLcdStr,6);
    }
    else
    {
		Lcm_Disp_8_16(2,4,ID3_CTRL_INFO.id3_info[PLAY_ID3_TITLE],ID3_CTRL_INFO.id3_len[PLAY_ID3_TITLE]);
	}
}
#endif

