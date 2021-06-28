#include "sdk_cfg.h"
#include "setup.h"
#include "rtc.h"
#include "audio_drv.h"
#include "disp.h"
#include "ui_drv.h"
#include "ui_common.h"
#include "ui_res.h"
#include "ui_api.h"
#if defined(_SL6812_TEST_LCD_)
#include "bluetooth/avctp_user.h"
#include "dev_manage/dev_ctl.h"
#include "file_operate/file_op.h"
#endif
#if defined(SUPPORT_ALARM_FUNC_)
#include "alarm/alarm.h"
#endif

#if defined(_ENABLE_MIC_REC_)
extern uint16 rec_time_count,rec_status;
extern void rec_time_disp(uint8 level,uint16 rec_time);
#include "setup.h"
#endif

#define UI_DEBUG
#ifdef UI_DEBUG
#define ui_printf 		ew_printf
#define ui_puts 		ew_puts
#else
#define ui_printf(...)
#define ui_puts(...)
#endif

ui_disp_t ui_disp;
extern func_sel_check_f func_sel_check;
#if defined(_USER_TFT_SEND_)
extern void TFT_Tx_Sel(uint8 SelType);	
#endif

/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/

static void show_volume_set(uint8* dispchar,uint8 volume)
{
#if defined(TD_ADD_LCD_NUM_4)
	dispchar[2] = volume / 10 + '0';
	dispchar[3] = volume % 10 + '0';
#elif defined(_XM003_SPECIAL_DISPLAY_)
	dispchar[5] = volume / 10 + '0';
	dispchar[6] = volume % 10 + '0';
#elif defined(_LCD_3_DIAPLAY_)
    if(volume < 10)
    {
		dispchar[3] = volume + '0';
    }
    else
    {
    	dispchar[2] = volume / 10 + '0';
	    dispchar[3] = volume % 10 + '0';
    }	
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(volume < 10)
    {
		dispchar[8] = ' ';
    	dispchar[9] = volume + '0';
    }
    else
    {
    	dispchar[8] = volume / 10 + '0';
    	dispchar[9] = volume % 10 + '0';
    }
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    if(volume < 10)
    {
    	dispchar[7] = ' ';
    	dispchar[8] = volume + '0';
    }
    else
    {
    	dispchar[7] = volume / 10 + '0';
    	dispchar[8] = volume % 10 + '0';
    }
#elif defined(_FIVE_CHAR_DISPLAY_)
#if defined(_CHANGE_LCD_DISPLAY_)
    if(volume < 10)
	{
	    dispchar[4] = '0';
		dispchar[5] = volume + '0';
	}
	else
	{
		dispchar[4] = volume / 10 + '0';
		dispchar[5] = volume % 10 + '0';
	}
#else
	if(volume < 10)
	{
		dispchar[5] = volume + '0';
	}
	else
	{
		dispchar[4] = volume / 10 + '0';
		dispchar[5] = volume % 10 + '0';
	}
#endif	
#elif defined(_SEVEN_CHAR_DISPLAY_)
    if(volume < 10)
    {
    	dispchar[5] = volume + '0';
    }
    else
    {
    	dispchar[4] = volume / 10 + '0';
    	dispchar[5] = volume % 10 + '0';
    }			
#elif defined(_EVB_VOLUME_100_)
	if(volume < 10)
	{
		dispchar[4] = ' ';
		dispchar[5] = ' ';
		dispchar[6] = volume + '0';
	}
	else
	{
		if(volume == 100)
		{
			dispchar[4] = '1';
			dispchar[5] = '0';
			dispchar[6] = '0';
		}
		else
		{
			dispchar[4] = ' ';
			dispchar[5] = volume / 10 + '0';
			dispchar[6] = volume % 10 + '0';
		}
	}
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(volume < 10)
    {
    	dispchar[8] = ' ';
    	dispchar[9] = volume + '0';
    }
    else
    {
    	dispchar[8] = volume / 10 + '0';
    	dispchar[9] = volume % 10 + '0';
    }
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(volume < 10)
    {
    	dispchar[7] = ' ';
    	dispchar[8] = volume + '0';
    }
    else
    {
    	dispchar[7] = volume / 10 + '0';
    	dispchar[8] = volume % 10 + '0';
    }
#else
	if(volume < 10)
	{
	#if defined(_KLD3000_SPECIAL_DISPLAY_)
    	dispchar[5] = '0';
    	dispchar[6] = volume + '0';
	#else
		dispchar[5] = ' ';
		dispchar[6] = volume + '0';
	#endif
	}
	else
	{
		dispchar[5] = volume / 10 + '0';
		dispchar[6] = volume % 10 + '0';
	}
#endif
}

#if defined(_SPECIAL_TL2000_SET_)
static void show_a_volume_set(uint8* dispchar,uint8 volume)
{
	if(volume < 10)
	{
		dispchar[5] = ' ';
		dispchar[6] = volume + '0';
	}
	else
	{
		dispchar[5] = volume / 10 + '0';
		dispchar[6] = volume % 10 + '0';
	}
}
static void show_b_volume_set(uint8* dispchar,uint8 volume)
{
	if(volume < 10)
	{
		dispchar[5] = ' ';
		dispchar[6] = volume + '0';
	}
	else
	{
		dispchar[5] = volume / 10 + '0';
		dispchar[6] = volume % 10 + '0';
	}
}
#endif

#if defined(_CHANGE_DEF_VOL_)
static void show_p_vol_set(uint8* dispchar,uint8 volume)
{
	if(volume < 10)
	{
		dispchar[6] = ' ';
		dispchar[7] = volume + '0';
	}
	else
	{
		dispchar[6] = volume / 10 + '0';
		dispchar[7] = volume % 10 + '0';
	}
}
#endif

#if defined(_ENABLE_TALK_VOL_)
static void show_talk_vol_set(uint8* dispchar,uint8 talkvolume)
{
#if defined(_SL6812_SPECIAL_DISPLAY_)
#if defined(_SL6812A_SPECIAL_DISPLAY_)
	dispchar[8] = talkvolume / 10 + '0';
	dispchar[9] = talkvolume % 10 + '0';
#else
    if(talkvolume < 10)
    {
    	dispchar[6] = ' ';
    	dispchar[7] = talkvolume + '0';
    }
    else
    {
    	dispchar[6] = talkvolume / 10 + '0';
    	dispchar[7] = talkvolume % 10 + '0';
    }
#endif
#else
	if(talkvolume < 10)
	{
		dispchar[5] = ' ';
		dispchar[6] = talkvolume + '0';
	}
	else
	{
		dispchar[5] = talkvolume / 10 + '0';
		dispchar[6] = talkvolume % 10 + '0';
	}
#endif
}
#endif

#if defined(_ENABLE_7419_MIDDLE_)
static void show_middle_set(uint8* dispchar,int8 middle)
{
#if defined(_FIVE_CHAR_DISPLAY_)
    if(middle<0)
    {
    	dispchar[4] = '-';
    	middle = -middle;
    }
    else if(middle>0)
    {
    	dispchar[4] = '+';
    }
    else
    {
    	dispchar[4] = ' ';
    }
    dispchar[5] = '0'+ middle;
#else
	if(middle < 0)
	{
		dispchar[5] = '-';
		middle = -middle;
	}
	else if(middle > 0)
	{
		dispchar[5] = '+';
	}
	else
	{
		dispchar[5] = ' ';
	}
	dispchar[6] = '0'+ middle;
#endif
}
#endif

static void show_bass_set(uint8* dispchar,int8 bass)
{
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	if(bass < 0)
	{
	    bass = -bass;
	    dispchar[2] = '-';
        dispchar[3] = '0'+ bass;
	}
	else if(bass > 0)
	{
	    #if defined(HuiNuo_6651D_Copy)
		dispchar[2] = ' ';
		#else
	    dispchar[2] = '0';
		#endif
        dispchar[3] = '0'+ bass;
	}
	else
	{
	    
	    dispchar[2] = '0';
	    dispchar[3] = '0';
	}
#elif defined(_XM003_SPECIAL_DISPLAY_)
    if(bass<0)
    {
    	dispchar[5] = '-';
    	bass = -bass;
    }
    else if(bass>0)
    {
    	dispchar[5] = '+';
    }
    else
    {
    	dispchar[5] = ' ';
    }
    dispchar[6] = '0'+ bass;
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(bass < 0)
    {
    	dispchar[8] = '-';
    	bass = -bass;
    }
    else if(bass > 0)
    {
    	dispchar[8] = '+';
    }
    else
    {
    	dispchar[8] = ' ';
    }
    dispchar[9] = '0'+ bass;
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(bass < 0)
    {
    	dispchar[8] = '-';
    	bass = -bass;
    }
    else if(bass > 0)
    {
    	dispchar[8] = '+';
    }
    else
    {
    	dispchar[8] = ' ';
    }
    dispchar[9] = '0'+ bass;
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(bass < 0)
    {
    	dispchar[8] = '-';
    	bass = -bass;
    }
    else if(bass > 0)
    {
    	dispchar[8] = '+';
    }
    else
    {
    	dispchar[8] = ' ';
    }
    dispchar[9] = '0'+ bass;
#elif defined(_FY5517_SPECIAL_DISPLAY_)
#if defined(FY_SPECIAL_BAS_TRE_)
    if(bass < 0)
    {
    	dispchar[5] = '-';
    	bass = -bass;
    	if(bass < 10)
    	{
    		dispchar[6] = bass + '0';
    	}
    	else
    	{
    		dispchar[7] = bass%10 + '0';
    		dispchar[6] = bass/10 + '0';
    	}
    }
    else if(bass > 0)
    {
    	dispchar[5] = '+';
    	if(bass < 10)
    	{
    		dispchar[6] = bass + '0';
    	}
    	else
    	{
    		dispchar[7] = bass%10 + '0';
    		dispchar[6] = bass/10 + '0';
    	}
    }
    else
    {
		dispchar[5] = ' ';
		dispchar[6] = '0';
		dispchar[7] = ' ';
    }
#else
    if(bass < 0)
    {
    	dispchar[7] = '-';
    	bass = -bass;
    }
    else if(bass > 0)
    {
    	dispchar[7] = '+';
    }
    else
    {
    	dispchar[7] = ' ';
    }
    dispchar[8] = '0'+ bass;
#endif

#elif defined(_4MIBA_4SUZIBA_)
	if(bass < 0)
	{
		dispchar[5] = 'L';
		bass = -bass;
	}
	else if(bass > 0)
	{
		dispchar[5] = 'H';
	}
	else
	{
		dispchar[5] = ' ';
	}
	dispchar[6] = '0'+ bass;
#elif defined(_FIVE_CHAR_DISPLAY_)
	if(bass<0)
	{
		dispchar[4] = '-';
		bass = -bass;
	}
	else if(bass>0)
	{
		dispchar[4] = '+';
	}
	else
	{
		dispchar[4] = ' ';
	}
	dispchar[5] = '0'+ bass;
#elif defined(_SEVEN_CHAR_DISPLAY_)
    if(bass < 0)
    {
    	dispchar[4] = '-';
    	bass = -bass;
    }
    else if(bass > 0)
    {
    #if defined(_ZSBY5209_SPECIAL_DISPLAY_)
		dispchar[4] = ' ';
	#else
    	dispchar[4] = '+';
	#endif
    }
    else
    {
    	dispchar[4] = ' ';
    }
    dispchar[5] = '0'+ bass;
#elif defined(_SPECIAL_PT2313_SET_)
    if(bass < 0)
    {
    	dispchar[5] = '-';
    	bass = -bass;
    	if(bass < 10)
    	{
    		dispchar[6] = bass + '0';
    	}
    	else
    	{
    		dispchar[7] = bass%10 + '0';
    		dispchar[6] = bass/10 + '0';
    	}
    }
    else if(bass > 0)
    {
    	dispchar[5] = '+';
    	if(bass < 10)
    	{
    		dispchar[6] = bass + '0';
    	}
    	else
    	{
    		dispchar[7] = bass%10 + '0';
    		dispchar[6] = bass/10 + '0';
    	}
    }
    else
    {
		dispchar[5] = ' ';
		dispchar[6] = '0';
		dispchar[7] = ' ';
    }
#else
	if(bass < 0)
	{
		dispchar[5] = '-';
		bass = -bass;
	}
	else if(bass > 0)
	{
		dispchar[5] = '+';
	}
	else
	{
		dispchar[5] = ' ';
	}
	dispchar[6] = '0'+ bass;
#endif
}

static void show_treble_set(uint8* dispchar,int8 treble)
{
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	if(treble < 0)
	{
	    treble = -treble;
	    dispchar[2] = '-';
        dispchar[3] = '0'+ treble;
	}
	else if(treble > 0)
	{
	#if defined(HuiNuo_6651D_Copy)
		dispchar[2] = ' ';
	#else
	    dispchar[2] = '0';
	#endif
        dispchar[3] = '0'+ treble;
	}
	else
	{
	    dispchar[2] = '0';
	    dispchar[3] = '0';
	}
#elif defined(_XM003_SPECIAL_DISPLAY_)
	if(treble<0)
	{
		dispchar[5] = '-';
		treble = -treble;
	}
	else if(treble>0)
	{
		dispchar[5] = '+';
	}
	else
	{
		dispchar[5] = ' ';
	}
	dispchar[6] = '0'+ treble;
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(treble < 0)
    {
    	dispchar[8] = '-';
    	treble = -treble;
    }
    else if(treble > 0)
    {
    	dispchar[8] = '+';
    }
    else
    {
    	dispchar[8] = ' ';
    }
    dispchar[9] = '0'+ treble;
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(treble < 0)
    {
    	dispchar[8] = '-';
    	treble = -treble;
    }
    else if(treble > 0)
    {
    	dispchar[8] = '+';
    }
    else
    {
    	dispchar[8] = ' ';
    }
    dispchar[9] = '0'+ treble;
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(treble < 0)
    {
    	dispchar[8] = '-';
    	treble = -treble;
    }
    else if(treble > 0)
    {
    	dispchar[8] = '+';
    }
    else
    {
    	dispchar[8] = ' ';
    }
    dispchar[9] = '0'+ treble;
#elif defined(_FY5517_SPECIAL_DISPLAY_)
#if defined(FY_SPECIAL_BAS_TRE_)
    if(treble < 0)
	{
		dispchar[5] = '-';
		treble = -treble;
		if(treble < 10)
		{
			dispchar[6] = treble + '0';
		}
		else
		{
			dispchar[7] = treble%10 + '0';
			dispchar[6] = treble/10 + '0';
		}
	}
	else if(treble > 0)
	{
		dispchar[5] = '+';
		if(treble < 10)
		{
			dispchar[6] = treble + '0';
		}
		else
		{
			dispchar[7] = treble%10 + '0';
			dispchar[6] = treble/10 + '0';
		}
	}
	else
	{
		dispchar[5] = ' ';
		dispchar[6] = '0';
		dispchar[7] = ' ';
	}
#else
    if(treble < 0)
    {
    	dispchar[7] = '-';
    	treble = -treble;
    }
    else if(treble > 0)
    {
    	dispchar[7] = '+';
    }
    else
    {
    	dispchar[7] = ' ';
    }
    dispchar[8] = '0'+ treble;
#endif	
#elif defined(_4MIBA_4SUZIBA_)
	if(treble < 0)
	{
		dispchar[5] = 'L';
		treble = -treble;
	}
	else if(treble > 0)
	{
		dispchar[5] = 'H';
	}
	else
	{
		dispchar[5] = ' ';
	}
	dispchar[6] = '0'+ treble;
#elif defined(_FIVE_CHAR_DISPLAY_)
	if(treble<0)
	{
		dispchar[4] = '-';
		treble = -treble;
	}
	else if(treble>0)
	{
		dispchar[4] = '+';
	}
	else
	{
		dispchar[4] = ' ';
	}
	dispchar[5] = '0'+ treble;
#elif defined(_SEVEN_CHAR_DISPLAY_)
    if(treble < 0)
    {
    	dispchar[4] = '-';
    	treble = -treble;
    }
    else if(treble > 0)
    {
    #if defined(_ZSBY5209_SPECIAL_DISPLAY_)
    	dispchar[4] = ' ';
    #else
    	dispchar[4] = '+';
    #endif
    }
    else
    {
    	dispchar[4] = ' ';
    }
    dispchar[5] = '0'+ treble;
#elif defined(_SPECIAL_PT2313_SET_)
    if(treble < 0)
    {
    	dispchar[5] = '-';
    	treble = -treble;
    	if(treble < 10)
    	{
    		dispchar[6] = treble + '0';
    	}
    	else
    	{
    		dispchar[7] = treble%10 + '0';
    		dispchar[6] = treble/10 + '0';
    	}
    }
    else if(treble > 0)
    {
    	dispchar[5] = '+';
    	if(treble < 10)
    	{
    		dispchar[6] = treble + '0';
    	}
    	else
    	{
    		dispchar[7] = treble%10 + '0';
    		dispchar[6] = treble/10 + '0';
    	}
    }
    else
    {
    	dispchar[5] = ' ';
    	dispchar[6] = '0';
    	dispchar[7] = ' ';
    }
#else
	if(treble < 0)
	{
		dispchar[5] = '-';
		treble = -treble;
	}
	else if(treble > 0)
	{
		dispchar[5] = '+';
	}
	else
	{
		dispchar[5] = ' ';
	}
	dispchar[6] = '0'+ treble;
#endif
}

static void show_bal_set(uint8* dispchar,int8 bal)
{
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	if(bal < 0)
	{
	    bal = -bal;
	#if defined(HuiNuo_6651D_Copy)
		dispchar[2] = '-';
	#elif defined(_WLS1403_SPECIAL_DISP_)
		dispchar[2] = 'F';
	#else
	    dispchar[2] = 'L';
	#endif
	    dispchar[3] = bal + '0';
	}
	else if(bal > 0)
	{
	   #if defined(HuiNuo_6651D_Copy)
		dispchar[2] = ' ';
	#else
	    dispchar[2] = 'R';
	#endif
	    dispchar[3] = bal + '0';
	}
	else
	{
	    dispchar[2] = '0';
	    dispchar[3] = '0';
	}
#elif defined(_XM003_SPECIAL_DISPLAY_)
	if(bal<0)
	{
		bal = -bal;
		dispchar[4] = bal + '0';
		dispchar[5] = '-';
		dispchar[6] = '-';
	}
	else if(bal>0)
	{
		dispchar[4] = '-';
		dispchar[5] = '-';
		dispchar[6] = bal + '0';
	}
	else
	{
		dispchar[4] = '-';
		dispchar[5] = '0';
		dispchar[6] = '-';
	}
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(bal < 0)
    {
    	dispchar[10] = 'L';
    	bal = -bal;
    	if(bal < 10)
    	{
    		dispchar[9] = bal + '0';
    	}
    	else
    	{
    		dispchar[9] = bal%10 + '0';
    		dispchar[8] = bal/10 + '0';
    	}
    }
    else if(bal > 0)
    {
    	dispchar[10] = 'R';
    	if(bal < 10)
    	{
    		dispchar[9] = bal + '0';
    	}
    	else
    	{
    		dispchar[9] = bal%10 + '0';
    		dispchar[8] = bal/10 + '0';
    	}
    }
    else
    {
    	dispchar[8] = 'L';
    	dispchar[9] = '=';
    	dispchar[10] = 'R';
    }
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(bal < 0)
    {
    	dispchar[8] = 'L';
    	bal = -bal;
    	dispchar[7] = bal + '0';
    }
    else if(bal > 0)
    {
    	dispchar[8] = 'R';
    	dispchar[7] = bal + '0';
    }
    else
    {
    	dispchar[7] = 'L';
    	dispchar[8] = '=';
    	dispchar[9] = 'R';
    }
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(bal < 0)
    {
    	dispchar[8] = 'L';
    	bal = -bal;
    	dispchar[9] = bal + '0';
    }
    else if(bal > 0)
    {
    	dispchar[8] = 'R';
    	dispchar[9] = bal + '0';
    }
    else
    {
    	dispchar[8] = ' ';
    	dispchar[9] = '0';
    }
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    if(bal < 0)
    {
    	dispchar[8] = 'L';
    	bal = -bal;
    	if(bal < 10)
    	{
    		dispchar[7] = bal + '0';
    	}
    	else
    	{
    		dispchar[7] = bal%10 + '0';
    		dispchar[6] = bal/10 + '0';
    	}
    }
    else if(bal > 0)
    {
    	dispchar[8] = 'R';
    	if(bal < 10)
    	{
    		dispchar[7] = bal + '0';
    	}
    	else
    	{
    		dispchar[7] = bal%10 + '0';
    		dispchar[6] = bal/10 + '0';
    	}
    }
    else
    {
    	dispchar[6] = 'L';
    	dispchar[7] = '=';
    	dispchar[8] = 'R';
    }
#elif defined(_FIVE_CHAR_DISPLAY_)
#if defined(SPECIAL_JD_DISP)||defined(SPECIAL_SLA6799_DISP)
	if(bal<0)
	{
		bal = -bal;
		dispchar[4] = 'L';
		dispchar[5] = bal + '0';
	}
	else if(bal>0)
	{
		dispchar[4] = 'R';
		dispchar[5] = bal + '0';
	}
	else
	{
		dispchar[4] = ' ';
		dispchar[5] = '0';
	}
#else
	if(bal<0)
	{
		bal = -bal;
		dispchar[4] = 'L';
		dispchar[5] = bal + '0';
	}
	else if(bal>0)
	{
		dispchar[4] = 'R';
		dispchar[5] = bal + '0';
	}
	else
	{
		dispchar[4] = '0';
		dispchar[5] = '0';
	}
#endif
#elif defined(_YS_SPECIAL_DISP_)
	if(bal < 0)
	{
		dispchar[5] = 'L';
		bal = -bal;
		if(bal < 10)
		{
			dispchar[6] = bal + '0';
		}
	}
	else if(bal > 0)
	{
		dispchar[5] = 'R';
		if(bal < 10)
		{
			dispchar[6] = bal + '0';
		}
	}
	else
	{
    	dispchar[6] = '0';
	}
#else
#if defined(SPECIAL_JD_DISP)
	if(bal < 0)
	{
		dispchar[5] = 'L';
		bal = -bal;
	}
	else if(bal > 0)
	{
		dispchar[5] = 'R';
	}
	else
	{
		dispchar[5] = ' ';
	}
	dispchar[6] = '0'+ bal;
#elif defined(_KLD3000_SPECIAL_DISPLAY_)
	if(bal < 0)
	{
		bal = -bal;
		dispchar[5] = 'L';
		dispchar[6] = '0'+ bal;
	}
	else if(bal > 0)
	{
		dispchar[5] = 'R';
		dispchar[6] = '0'+ bal;
	}
	else
	{
		dispchar[4] = ' ';
		dispchar[5] = ' ';
		dispchar[6] = '0';
	}
#else
	if(bal < 0)
	{
	#if defined(_FY2010_LCD_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
    	dispchar[4] = 'L';
    	bal = -bal;
    	if(bal < 10)
    	{
    		dispchar[5] = bal + '0';
    	}
    	else
    	{
    		dispchar[6] = bal%10 + '0';
    		dispchar[5] = bal/10 + '0';
    	}
    #elif defined(_TY2200A_SPECIAL_DISP_)
		dispchar[5] = 'L';
		bal = -bal;
		if(bal < 10)
		{
			dispchar[6] = bal + '0';
		}
		else
		{
			dispchar[7] = bal%10 + '0';
			dispchar[6] = bal/10 + '0';
		}
	#else
		dispchar[6] = 'L';
		bal = -bal;
		if(bal < 10)
		{
			dispchar[5] = bal + '0';
		}
		else
		{
			dispchar[5] = bal%10 + '0';
			dispchar[4] = bal/10 + '0';
		}
	#endif
	}
	else if(bal > 0)
	{
	#if defined(_FY2010_LCD_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
    	dispchar[4] = 'R';
    	if(bal < 10)
    	{
    		dispchar[5] = bal + '0';
    	}
    	else
    	{
    		dispchar[6] = bal%10 + '0';
    		dispchar[5] = bal/10 + '0';
    	}
    #elif defined(_TY2200A_SPECIAL_DISP_)
		dispchar[5] = 'R';
		if(bal < 10)
		{
			dispchar[6] = bal + '0';
		}
		else
		{
			dispchar[7] = bal%10 + '0';
			dispchar[6] = bal/10 + '0';
		}
	#else
		dispchar[6] = 'R';
		if(bal < 10)
		{
			dispchar[5] = bal + '0';
		}
		else
		{
			dispchar[5] = bal%10 + '0';
			dispchar[4] = bal/10 + '0';
		}
	#endif
	}
	else
	{
	#if defined(_GX6211_SPECIAL_DISPLAY_)||defined(_SEVEN_CHAR_DISPLAY_)
	#if defined(_FY2010_LCD_DISPLAY_)
    	dispchar[4] = 'R';
    	dispchar[5] = '=';
    	dispchar[6] = 'L';
	#else
    	dispchar[4] = 'L';
    	dispchar[5] = '=';
    	dispchar[6] = 'R';
	#endif
	#elif defined(_MR1901_SPECIAL_DISPLAY_)
    	dispchar[4] = 'R';
    	dispchar[5] = '=';
    	dispchar[6] = 'L';
	#else
    	dispchar[5] = 'L';
    	dispchar[6] = '=';
    	dispchar[7] = 'R';
	#endif
	}
#endif
#endif
}

static void show_fad_set(uint8* dispchar,int8 fad)
{
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	if(fad < 0)
	{
	    fad = -fad;
	#if defined(_SPECIAL_FAD_DISPLAY_)
		dispchar[2] = 'F';
	#else
	    dispchar[2] = 'R';
	#endif
	    dispchar[3] = fad + '0';
	}
	else if(fad > 0)
	{
	#if defined(_SPECIAL_FAD_DISPLAY_)
		dispchar[2] = 'R';
	#else
	    dispchar[2] = 'F';
	#endif
	    dispchar[3] = fad + '0';
	}
	else
	{
	    dispchar[2] = '0';
	    dispchar[3] = '0';
	}
#elif defined(_XM003_SPECIAL_DISPLAY_)
	if(fad<0)
	{
		fad = -fad;
		dispchar[4] = '>';
		dispchar[5] = fad + '0';
		dispchar[6] = '>';
	}
	else if(fad>0)
	{
		dispchar[4] = '_';
		dispchar[5] = fad + '0';
		dispchar[6] = '_';
	}
	else
	{
		dispchar[4] = '-';
		dispchar[5] = '0';
		dispchar[6] = '-';
	}
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    if(fad < 0)
    {
    	dispchar[10] = 'R';
    	fad = -fad;
    	if(fad < 10)
    	{
    		dispchar[9] = fad + '0';
    	}
    	else
    	{
    		dispchar[9] = fad%10 + '0';
    		dispchar[8] = fad/10 + '0';
    	}
    }
    else if(fad > 0)
    {
    	dispchar[10] = 'F';
    	if(fad < 10)
    	{
    		dispchar[9] = fad + '0';
    	}
    	else
    	{
    		dispchar[9] = fad%10 + '0';
    		dispchar[8] = fad/10 + '0';
    	}
    }
    else
    {
    	dispchar[8] = 'F';
    	dispchar[9] = '=';
    	dispchar[10] = 'R';
    }
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    if(fad < 0)
    {
    	dispchar[8] = 'R';
    	fad = -fad;
    	dispchar[7] = fad + '0';
    }
    else if(fad > 0)
    {
    	dispchar[8] = 'F';
    	dispchar[7] = fad + '0';
    }
    else
    {
    	dispchar[7] = 'F';
    	dispchar[8] = '=';
    	dispchar[9] = 'R';
    }
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    if(fad < 0)
    {
    	dispchar[8] = 'R';
    	fad = -fad;
    	dispchar[9] = fad + '0';
    }
    else if(fad > 0)
    {
    	dispchar[8] = 'F';
    	dispchar[9] = fad + '0';
    }
    else
    {
    	dispchar[8] = ' ';
    	dispchar[9] = '0';
    }
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	if(fad < 0)
	{
		dispchar[8] = 'R';
		fad = -fad;
		if(fad < 10)
		{
			dispchar[7] = fad + '0';
		}
		else
		{
			dispchar[7] = fad%10 + '0';
			dispchar[6] = fad/10 + '0';
		}
	}
	else if(fad > 0)
	{
		dispchar[8] = 'F';
		if(fad < 10)
		{
			dispchar[7] = fad + '0';
		}
		else
		{
			dispchar[7] = fad%10 + '0';
			dispchar[6] = fad/10 + '0';
		}
	}
	else
	{
		dispchar[6] = 'F';
		dispchar[7] = '=';
		dispchar[8] = 'R';
	}
#elif defined(_FIVE_CHAR_DISPLAY_)
#if defined(SPECIAL_JD_DISP)||defined(SPECIAL_SLA6799_DISP)
	if(fad<0)
	{
		fad = -fad;
		dispchar[4] = 'R';
		dispchar[5] = fad + '0';
	}
	else if(fad>0)
	{
		dispchar[4] = 'F';
		dispchar[5] = fad + '0';
	}
	else
	{
		dispchar[4] = ' ';
		dispchar[5] = '0';
	}
#else
	if(fad<0)
	{
		fad = -fad;
		dispchar[4] = 'R';
		dispchar[5] = fad + '0';
	}
	else if(fad>0)
	{
		dispchar[4] = 'F';
		dispchar[5] = fad + '0';
	}
	else
	{
		dispchar[4] = '0';
		dispchar[5] = '0';
	}
#endif
#else
#if defined(SPECIAL_JD_DISP)
	if(fad < 0)
	{
		dispchar[5] = 'R';
		fad = -fad;
	}
	else if(fad > 0)
	{
		dispchar[5] = 'F';
	}
	else
	{
		dispchar[5] = ' ';
	}
	dispchar[6] = '0'+ fad;
#elif defined(_KLD3000_SPECIAL_DISPLAY_)
    if(fad < 0)
    {
    	fad = -fad;
    	dispchar[5] = 'R';
		dispchar[6] = '0'+ fad;
    }
    else if(fad > 0)
    {
    	dispchar[5] = 'F';
		dispchar[6] = '0'+ fad;
    }
    else
    {
		dispchar[4] = ' ';
    	dispchar[5] = ' ';
    	dispchar[6] = '0';
    }
#else
	if(fad < 0)
	{
	#if defined(_FY2010_LCD_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
    	dispchar[4] = 'R';
    	fad = -fad;
    	if(fad < 10)
    	{
    		dispchar[5] = fad + '0';
    	}
    	else
    	{
    		dispchar[6] = fad%10 + '0';
    		dispchar[5] = fad/10 + '0';
    	}
	#elif defined(_TY2200A_SPECIAL_DISP_)
	    dispchar[5] = 'R';
		fad = -fad;
		if(fad < 10)
		{
			dispchar[6] = fad + '0';
		}
		else
		{
			dispchar[7] = fad%10 + '0';
			dispchar[6] = fad/10 + '0';
		}
	#else
		dispchar[6] = 'R';
		fad = -fad;
		if(fad < 10)
		{
			dispchar[5] = fad + '0';
		}
		else
		{
			dispchar[5] = fad%10 + '0';
			dispchar[4] = fad/10 + '0';
		}
	#endif
	}
	else if(fad > 0)
	{
	#if defined(_FY2010_LCD_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
    	dispchar[4] = 'F';
    	if(fad < 10)
    	{
    		dispchar[5] = fad + '0';
    	}
    	else
    	{
    		dispchar[6] = fad%10 + '0';
    		dispchar[5] = fad/10 + '0';
    	}
	#elif defined(_TY2200A_SPECIAL_DISP_)
	    dispchar[5] = 'F';
		if(fad < 10)
		{
			dispchar[6] = fad + '0';
		}
		else
		{
			dispchar[7] = fad%10 + '0';
			dispchar[6] = fad/10 + '0';
		}
	#else
		dispchar[6] = 'F';
		if(fad < 10)
		{
			dispchar[5] = fad + '0';
		}
		else
		{
			dispchar[5] = fad%10 + '0';
			dispchar[4] = fad/10 + '0';
		}
	#endif
	}
	else
	{
	#if defined(_GX6211_SPECIAL_DISPLAY_)||defined(_SEVEN_CHAR_DISPLAY_)
	#if defined(_FY2010_LCD_DISPLAY_)
    	dispchar[4] = 'R';
    	dispchar[5] = '=';
    	dispchar[6] = 'F';
	#else
    	dispchar[4] = 'F';
    	dispchar[5] = '=';
    	dispchar[6] = 'R';
	#endif
	#elif defined(_MR1901_SPECIAL_DISPLAY_)
    	dispchar[4] = 'R';
    	dispchar[5] = '=';
    	dispchar[6] = 'F';
	#elif defined(_KLD3000_SPECIAL_DISPLAY_)
    	dispchar[5] = ' ';
    	dispchar[6] = '0';
    	dispchar[7] = ' ';
	#else
		dispchar[5] = 'F';
    	dispchar[6] = '=';
    	dispchar[7] = 'R';
	#endif
	}
#endif
#endif
}

#if defined(_ENABLE_MENU_FUNTION_)
static void show_default_vol_set(uint8* dispchar,uint8 vol)
{
	dispchar[9] = vol%10 + '0';
	dispchar[8] = vol/10 + '0';
}
#endif

#if defined(_ENABLE_DEFAULT_CONTROL_)||defined(ADD_ADJUST_DEFAULT_VOL)
static void show_default_vol_set(uint8* dispchar,uint8 vol)
{
#if defined(_SL6812A_SPECIAL_DISPLAY_)
    dispchar[8] = vol%10 + '0';
    dispchar[7] = vol/10 + '0';
#else
	dispchar[7] = vol%10 + '0';
	dispchar[6] = vol/10 + '0';
#endif
}
#endif

#if defined(ENABLE_TA_VOL_SET)
static void show_tavol_set(uint8* dispchar,uint8 tavol)
{
#if defined(_FIVE_CHAR_DISPLAY_)
    dispchar[4] = tavol / 10 + '0';
    dispchar[5] = tavol % 10 + '0';
#else
	dispchar[6] = tavol / 10 + '0';
	dispchar[7] = tavol % 10 + '0';
#endif
}
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
static void show_sub_set(uint8* dispchar,uint8 sub)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
    dispchar[11] = sub%10 + '0';
    dispchar[10] = sub/10 + '0';
#elif defined(_FY7000A_SPECIAL_DISPLAY_)
    dispchar[7] = sub%10 + '0';
    dispchar[6] = sub/10 + '0';
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    dispchar[8] = sub%10 + '0';
    dispchar[7] = sub/10 + '0';
#elif defined(_FIVE_CHAR_DISPLAY_)
    dispchar[5] = sub%10 + '0';
    dispchar[4] = sub/10 + '0';
#else
	dispchar[6] = sub%10 + '0';
	dispchar[5] = sub/10 + '0';
#endif
}
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
#if defined(_FY7000_SPECIAL_DISPLAY_)
static void show_sub_cut_set(uint8* dispchar,uint8 sub_cut)
{
	dispchar[5] = ' ';
	dispchar[6] = ' ';
	if(sub_cut == 0)
	{
		dispchar[10] = 'H';
		dispchar[9] = '0';
		dispchar[8] = '8';
		dispchar[7] = ' ';
	}
	else
	{
		dispchar[7] = '1';
		if(sub_cut == 1)
			dispchar[8] = '2';
		else
			dispchar[8] = '6';
		dispchar[9] = '0';
		dispchar[10] = 'H';
	}
}
#elif defined(_FY5517_SPECIAL_DISPLAY_)
static void show_sub_cut_set(uint8* dispchar,uint8 sub_cut)
{
	dispchar[6] = ' ';
	if(sub_cut == 0)
	{
		dispchar[9] = 'H';
		dispchar[8] = '0';
		dispchar[7] = '8';
	}
	else
	{
		dispchar[6] = '1';
		if(sub_cut == 1)
			dispchar[7] = '2';
		else
			dispchar[7] = '6';
		dispchar[8] = '0';
		dispchar[9] = 'H';
	}
}
#elif defined(_FY7000A_SPECIAL_DISPLAY_)
static void show_sub_cut_set(uint8* dispchar,uint8 sub_cut)
{
	dispchar[3] = ' ';
	if(sub_cut == 0)
	{
		dispchar[7] = 'H';
		dispchar[6] = '0';
		dispchar[5] = '8';
		dispchar[4] = ' ';
	}
	else
	{
		dispchar[4] = '1';
		if(sub_cut == 1)
			dispchar[5] = '2';
		else
			dispchar[5] = '6';
		dispchar[6] = '0';
		dispchar[7] = 'H';
	}
}
#else
static void show_sub_cut_set(uint8* dispchar,uint8 sub_cut)
{
#if defined(_FIVE_CHAR_DISPLAY_)
    if(sub_cut == 0)
    {
    	dispchar[5] = '0';
    	dispchar[4] = '8';
		dispchar[3] = ' ';
    }
    else
    {
    	dispchar[3] = '1';
    	if(sub_cut == 1)
    		dispchar[4] = '2';
    	else
    		dispchar[4] = '6';
    	dispchar[5] = '0';
    }
#else
	dispchar[3] = '-';
	if(sub_cut == 0)
	{
		dispchar[5] = '0';
		dispchar[4] = '8';
	}
	else
	{
		dispchar[4] = '1';
		if(sub_cut == 1)
			dispchar[5] = '2';
		else
			dispchar[5] = '6';
		dispchar[6] = '0';
	}
#endif
}
#endif
#endif

#if defined(_SUPPORT_10_EQ_FUNTION_)
static void show_60hz_set(uint8* dispchar,int8 eq60hz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
    if(eq60hz < 0)
    {
    	dispchar[6] = '-';
    	eq60hz = -eq60hz;
    }
    else if(eq60hz > 0)
    {
    	dispchar[6] = '+';
    }
    else
    {
    	dispchar[6] = ' ';
    }
    dispchar[7] = '0'+ eq60hz;
#else
    if(eq60hz < 0)
    {
    	dispchar[10] = '-';
    	eq60hz = -eq60hz;
    }
    else if(eq60hz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq60hz;
#endif
}

static void show_150hz_set(uint8* dispchar,int8 eq150hz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
    if(eq150hz < 0)
    {
    	dispchar[6] = '-';
    	eq150hz = -eq150hz;
    }
    else if(eq150hz > 0)
    {
    	dispchar[6] = '+';
    }
    else
    {
    	dispchar[6] = ' ';
    }
    dispchar[7] = '0'+ eq150hz;
#else
    if(eq150hz < 0)
    {
    	dispchar[10] = '-';
    	eq150hz = -eq150hz;
    }
    else if(eq150hz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq150hz;
#endif
}

static void show_400hz_set(uint8* dispchar,int8 eq400hz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
	if(eq400hz < 0)
	{
		dispchar[6] = '-';
		eq400hz = -eq400hz;
	}
	else if(eq400hz > 0)
	{
		dispchar[6] = '+';
	}
	else
	{
		dispchar[6] = ' ';
	}
	dispchar[7] = '0'+ eq400hz;
#else
    if(eq400hz < 0)
    {
    	dispchar[10] = '-';
    	eq400hz = -eq400hz;
    }
    else if(eq400hz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq400hz;
#endif
}

static void show_1khz_set(uint8* dispchar,int8 eq1khz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
	if(eq1khz < 0)
	{
		dispchar[6] = '-';
		eq1khz = -eq1khz;
	}
	else if(eq1khz > 0)
	{
		dispchar[6] = '+';
	}
	else
	{
		dispchar[6] = ' ';
	}
	dispchar[7] = '0'+ eq1khz;
#else
    if(eq1khz < 0)
    {
    	dispchar[10] = '-';
    	eq1khz = -eq1khz;
    }
    else if(eq1khz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq1khz;
#endif
}

static void show_2khz_set(uint8* dispchar,int8 eq2khz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
	if(eq2khz < 0)
	{
		dispchar[6] = '-';
		eq2khz = -eq2khz;
	}
	else if(eq2khz > 0)
	{
		dispchar[6] = '+';
	}
	else
	{
		dispchar[6] = ' ';
	}
	dispchar[7] = '0'+ eq2khz;
#else
    if(eq2khz < 0)
    {
    	dispchar[10] = '-';
    	eq2khz = -eq2khz;
    }
    else if(eq2khz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq2khz;
#endif
}

static void show_3khz_set(uint8* dispchar,int8 eq3khz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
	if(eq3khz < 0)
	{
		dispchar[6] = '-';
		eq3khz = -eq3khz;
	}
	else if(eq3khz > 0)
	{
		dispchar[6] = '+';
	}
	else
	{
		dispchar[6] = ' ';
	}
	dispchar[7] = '0'+ eq3khz;
#else
    if(eq3khz < 0)
    {
    	dispchar[10] = '-';
    	eq3khz = -eq3khz;
    }
    else if(eq3khz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq3khz;
#endif
}

static void show_7khz_set(uint8* dispchar,int8 eq7khz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
	if(eq7khz < 0)
	{
		dispchar[6] = '-';
		eq7khz = -eq7khz;
	}
	else if(eq7khz > 0)
	{
		dispchar[6] = '+';
	}
	else
	{
		dispchar[6] = ' ';
	}
	dispchar[7] = '0'+ eq7khz;
#else
    if(eq7khz < 0)
    {
    	dispchar[10] = '-';
    	eq7khz = -eq7khz;
    }
    else if(eq7khz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq7khz;
#endif
}

static void show_10khz_set(uint8* dispchar,int8 eq10khz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
	if(eq10khz < 0)
	{
		dispchar[6] = '-';
		eq10khz = -eq10khz;
	}
	else if(eq10khz > 0)
	{
		dispchar[6] = '+';
	}
	else
	{
		dispchar[6] = ' ';
	}
	dispchar[7] = '0'+ eq10khz;
#else
    if(eq10khz < 0)
    {
    	dispchar[10] = '-';
    	eq10khz = -eq10khz;
    }
    else if(eq10khz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq10khz;
#endif
}

static void show_15khz_set(uint8* dispchar,int8 eq15khz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
	if(eq15khz < 0)
	{
		dispchar[6] = '-';
		eq15khz = -eq15khz;
	}
	else if(eq15khz > 0)
	{
		dispchar[6] = '+';
	}
	else
	{
		dispchar[6] = ' ';
	}
	dispchar[7] = '0'+ eq15khz;
#else
    if(eq15khz < 0)
    {
    	dispchar[10] = '-';
    	eq15khz = -eq15khz;
    }
    else if(eq15khz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq15khz;
#endif
}

static void show_20khz_set(uint8* dispchar,int8 eq20khz)
{
#if defined(_FY7000A_SPECIAL_DISPLAY_)
	if(eq20khz < 0)
	{
		dispchar[6] = '-';
		eq20khz = -eq20khz;
	}
	else if(eq20khz > 0)
	{
		dispchar[6] = '+';
	}
	else
	{
		dispchar[6] = ' ';
	}
	dispchar[7] = '0'+ eq20khz;
#else
    if(eq20khz < 0)
    {
    	dispchar[10] = '-';
    	eq20khz = -eq20khz;
    }
    else if(eq20khz > 0)
    {
    	dispchar[10] = '+';
    }
    else
    {
    	dispchar[10] = ' ';
    }
    dispchar[11] = '0'+ eq20khz;
#endif
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_CAN_UART_)
static void show_can_err_set(uint8* dispchar,int32 err)
{
    if(err < 10)
    {
    	dispchar[6] = err + '0';
	}
    else if((err < 100)&&(err > 9))
    {
    	dispchar[5] = err/10 + '0';
    	dispchar[6] = err%10 + '0';
    }
    else
    {
    	dispchar[5] = err/100 + '0';
    	dispchar[6] = (err%100)/10 + '0';
    	dispchar[7] = (err%100)%10 + '0';
    }
}

static void show_can_speed_set(uint8* dispchar,int32 speed)
{
    if(speed < 10)
    {
    	dispchar[6] = speed + '0';
    }
    else if((speed < 100)&&(speed > 9))
    {
    	dispchar[5] = speed/10 + '0';
    	dispchar[7] = speed%10 + '0';
		dispchar[6] = '.';
    }
	else
    {
    	dispchar[4] = speed/100 + '0';
    	dispchar[5] = (speed%100)/10 + '0';
    	dispchar[7] = (speed%100)%10 + '0';
		dispchar[6] = '.';
    }
}

static void show_can_distance_set(uint8* dispchar,int32 distance)
{
    if(distance < 10)
    {
    	dispchar[6] = distance + '0';
    }
    else if(distance < 100)
    {
    	dispchar[5] = distance/10 + '0';
    	dispchar[7] = distance%10 + '0';
		dispchar[6] = '.';
    }
    else if((distance > 99)&&(distance < 1000))
    {
    	dispchar[4] = distance/100 + '0';
    	dispchar[5] = (distance%100)/10 + '0';
    	dispchar[7] = (distance%100)%10 + '0';
		dispchar[6] = '.';
    }
    else if((distance > 999)&&(distance < 10000))
    {
    	dispchar[3] = (distance/100)/10 + '0';
    	dispchar[4] = (distance/100)%10 + '0';
    	dispchar[5] = (distance%100)/10 + '0';
    	dispchar[7] = (distance%100)%10 + '0';
		dispchar[6] = '.';
    }
    else if((distance > 9999)&&(distance < 100000))
    {
    	dispchar[2] = distance/10000+ '0';
    	dispchar[3] = (distance%10000)/1000 + '0';
    	dispchar[4] = ((distance%10000)%1000)/100 + '0';
    	dispchar[5] = (((distance%10000)%1000)%100)/10 + '0';
    	dispchar[7] = (((distance%10000)%1000)%100)%10 + '0';
		dispchar[6] = '.';
    }
    else if((distance > 99999)&&(distance < 1000000))
    {
    	dispchar[3] = distance/100000+ '0';
    	dispchar[4] = (distance%100000)/10000 + '0';
    	dispchar[5] = ((distance%100000)%10000)/1000 + '0';
    	dispchar[6] = (((distance%100000)%10000)%1000)/100 + '0';
    	dispchar[7] = ((((distance%100000)%10000)%1000)%100)/10 + '0';
    	//dispchar[7] = ((((distance%100000)%10000)%1000)%100)%10 + '0';
    }
	else
	{
    	dispchar[2] = distance/1000000+ '0';
    	dispchar[3] = (distance%1000000)/100000 + '0';
    	dispchar[4] = ((distance%1000000)%100000)/10000 + '0';
    	dispchar[5] = (((distance%1000000)%100000)%10000)/1000 + '0';
    	dispchar[6] = ((((distance%1000000)%100000)%10000)%1000)/100 + '0';
    	dispchar[7] = (((((distance%1000000)%100000)%10000)%1000)%100)/10 + '0';
	}
}

static void show_can_eleltric_set(uint8* dispchar,int32 eleltric)
{
    if(eleltric < 10)
    {
    	dispchar[6] = eleltric + '0';
    }
    else if((eleltric < 100)&&(eleltric > 9))
    {
    	dispchar[5] = eleltric/10 + '0';
    	dispchar[6] = eleltric%10 + '0';
    }
    else
    {
    	dispchar[5] = '1';
    	dispchar[6] = '0';
    	dispchar[7] = '0';
    }
}

static void show_can_gears_set(uint8* dispchar,int32 gears)
{
    dispchar[6] = gears + '0';
}

static void show_can_p1_set(uint8* dispchar,int8 p1)
{
    dispchar[6] = p1 + '0';
}

static void show_can_p2_set(uint8* dispchar,int8 p2)
{
    dispchar[6] = p2 + '0';
}

#endif

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
#if defined(_FAD_BAL_FUNTION_)
#if defined(_FY7000_SPECIAL_DISPLAY_)
static void show_fl_set(uint8* dispchar,int8 fl)
{
	if(fl < 0)
	{
		dispchar[7] = '-';
		fl = -fl;
		if(fl < 10)
		{
			dispchar[8] = fl + '0';
		}
		else
		{
			dispchar[9] = fl%10 + '0';
			dispchar[8] = fl/10 + '0';
		}

	}
	else if(fl > 0)
	{
		dispchar[7] = '+';
		if(fl < 10)
		{
			dispchar[8] = fl + '0';
		}
		else
		{
			dispchar[9] = fl%10 + '0';
			dispchar[8] = fl/10 + '0';
		}
	}
	else
	{
		dispchar[7] = ' ';
		dispchar[8] = '0';
		dispchar[9] = ' ';
	}
}
static void show_fr_set(uint8* dispchar,int8 fr)
{
	if(fr < 0)
	{
		dispchar[7] = '-';
		fr = -fr;
		if(fr < 10)
		{
			dispchar[8] = fr + '0';
		}
		else
		{
			dispchar[9] = fr%10 + '0';
			dispchar[8] = fr/10 + '0';
		}

	}
	else if(fr > 0)
	{
		dispchar[7] = '+';
		if(fr < 10)
		{
			dispchar[8] = fr + '0';
		}
		else
		{
			dispchar[9] = fr%10 + '0';
			dispchar[8] = fr/10 + '0';
		}
	}
	else
	{
		dispchar[7] = ' ';
		dispchar[8] = '0';
		dispchar[9] = ' ';
	}
}

static void show_rl_set(uint8* dispchar,int8 rl)
{
	if(rl < 0)
	{
		dispchar[7] = '-';
		rl = -rl;
		if(rl < 10)
		{
			dispchar[8] = rl + '0';
		}
		else
		{
			dispchar[9] = rl%10 + '0';
			dispchar[8] = rl/10 + '0';
		}

	}
	else if(rl > 0)
	{
		dispchar[7] = '+';
		if(rl < 10)
		{
			dispchar[8] = rl + '0';
		}
		else
		{
			dispchar[9] = rl%10 + '0';
			dispchar[8] = rl/10 + '0';
		}
	}
	else
	{
		dispchar[7] = ' ';
		dispchar[8] = '0';
		dispchar[9] = ' ';
	}
}

static void show_rr_set(uint8* dispchar,int8 rr)
{
	if(rr < 0)
	{
		dispchar[7] = '-';
		rr = -rr;
		if(rr < 10)
		{
			dispchar[8] = rr + '0';
		}
		else
		{
			dispchar[9] = rr%10 + '0';
			dispchar[8] = rr/10 + '0';
		}

	}
	else if(rr > 0)
	{
		dispchar[7] = '+';
		if(rr < 10)
		{
			dispchar[8] = rr + '0';
		}
		else
		{
			dispchar[9] = rr%10 + '0';
			dispchar[8] = rr/10 + '0';
		}
	}
	else
	{
		dispchar[7] = ' ';
		dispchar[8] = '0';
		dispchar[9] = ' ';
	}
}

#else
static void show_fl_set(uint8* dispchar,int8 fl)
{
	if(fl < 0)
	{
		dispchar[4] = '-';
		fl = -fl;
		if(fl < 10)
		{
			dispchar[5] = fl + '0';
		}
		else
		{
			dispchar[6] = fl%10 + '0';
			dispchar[5] = fl/10 + '0';
		}

	}
	else if(fl > 0)
	{
		dispchar[4] = '+';
		if(fl < 10)
		{
			dispchar[5] = fl + '0';
		}
		else
		{
			dispchar[6] = fl%10 + '0';
			dispchar[5] = fl/10 + '0';
		}
	}
	else
	{
		dispchar[4] = ' ';
		dispchar[5] = '0';
		dispchar[6] = ' ';
	}
}
static void show_fr_set(uint8* dispchar,int8 fr)
{
	if(fr < 0)
	{
		dispchar[4] = '-';
		fr = -fr;
		if(fr < 10)
		{
			dispchar[5] = fr + '0';
		}
		else
		{
			dispchar[6] = fr%10 + '0';
			dispchar[5] = fr/10 + '0';
		}

	}
	else if(fr > 0)
	{
		dispchar[4] = '+';
		if(fr < 10)
		{
			dispchar[5] = fr + '0';
		}
		else
		{
			dispchar[6] = fr%10 + '0';
			dispchar[5] = fr/10 + '0';
		}
	}
	else
	{
		dispchar[4] = ' ';
		dispchar[5] = '0';
		dispchar[6] = ' ';
	}
}

static void show_rl_set(uint8* dispchar,int8 rl)
{
	if(rl < 0)
	{
		dispchar[4] = '-';
		rl = -rl;
		if(rl < 10)
		{
			dispchar[5] = rl + '0';
		}
		else
		{
			dispchar[6] = rl%10 + '0';
			dispchar[5] = rl/10 + '0';
		}

	}
	else if(rl > 0)
	{
		dispchar[4] = '+';
		if(rl < 10)
		{
			dispchar[5] = rl + '0';
		}
		else
		{
			dispchar[6] = rl%10 + '0';
			dispchar[5] = rl/10 + '0';
		}
	}
	else
	{
		dispchar[4] = ' ';
		dispchar[5] = '0';
		dispchar[6] = ' ';
	}
}

static void show_rr_set(uint8* dispchar,int8 rr)
{
	if(rr < 0)
	{
		dispchar[4] = '-';
		rr = -rr;
		if(rr < 10)
		{
			dispchar[5] = rr + '0';
		}
		else
		{
			dispchar[6] = rr%10 + '0';
			dispchar[5] = rr/10 + '0';
		}

	}
	else if(rr > 0)
	{
		dispchar[4] = '+';
		if(rr < 10)
		{
			dispchar[5] = rr + '0';
		}
		else
		{
			dispchar[6] = rr%10 + '0';
			dispchar[5] = rr/10 + '0';
		}
	}
	else
	{
		dispchar[4] = ' ';
		dispchar[5] = '0';
		dispchar[6] = ' ';
	}
}
#endif
#endif

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
void show_clear_icon(uint8 level)
{
#if defined(_FIVE_CHAR_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
	uint16 disp_buf[4];
	uint8  disp_ctrl[3];

	disp_buf[0] = UI_ICON_CLOCK;	disp_ctrl[0] = 0;
	disp_buf[1] = UI_ICON_POINT;	disp_ctrl[1] = 0;
	disp_buf[2] = UI_ICON_DASH; 	disp_ctrl[2] = 0;
	disp_buf[3] = UI_ICON_NULL;
#elif defined(_ENABLE_RADIO_WB_)
#if defined(_ENABLE_WB_DISPLAY_)
    uint16 disp_buf[4];
    uint8  disp_ctrl[3];
    
    disp_buf[0] = UI_ICON_CLOCK;	disp_ctrl[0] = 0;
    disp_buf[1] = UI_ICON_POINT;	disp_ctrl[1] = 0;
    disp_buf[2] = UI_ICON_WB_POINT; disp_ctrl[2] = 0;
    disp_buf[3] = UI_ICON_NULL;
#else
    uint16 disp_buf[3];
    uint8  disp_ctrl[2];
    
    disp_buf[0] = UI_ICON_CLOCK;	disp_ctrl[0] = 0;
    disp_buf[1] = UI_ICON_POINT;	disp_ctrl[1] = 0;
    disp_buf[2] = UI_ICON_NULL;
#endif

#else
	uint16 disp_buf[3];
	uint8  disp_ctrl[2];

	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = 0;
	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = 0;

	disp_buf[2] = UI_ICON_NULL;
#endif

	ui_drv_func.icon_ctrl(level, disp_buf, disp_ctrl);
}

#if defined(_ACC_OFF_POWER_LOGO_)
void show_clear_num(uint8 level)
{
	ui_show_num(level, 0xFF);
	ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_CH);
#if defined(_SPECIAL_LCD_4_DIAPLAY_)
	ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_FM);
#endif
}

#else
static void show_clear_num(uint8 level)
{
	ui_show_num(level, 0xFF);
	ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_CH);
#if defined(_SPECIAL_LCD_4_DIAPLAY_)
	ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_FM);
#endif
}
#endif

static void show_eq_icon(uint8 eq)
{
	uint16 disp_buf[7];
	uint8  disp_ctrl[6];

	disp_buf[0] = UI_ICON_EQ;		disp_ctrl[0] = ((eq == AUDIO_EQ_OFF)?0:1);
	disp_buf[1] = UI_ICON_EQ_POP;	disp_ctrl[1] = ((eq == AUDIO_EQ_POP)?1:0);
	disp_buf[2] = UI_ICON_EQ_FLAT;	disp_ctrl[2] = ((eq == AUDIO_EQ_FLAT)?1:0);
	disp_buf[3] = UI_ICON_EQ_ROCK;	disp_ctrl[3] = ((eq == AUDIO_EQ_ROCK)?1:0);
	disp_buf[4] = UI_ICON_EQ_CLASS;	disp_ctrl[4] = ((eq == AUDIO_EQ_CLASSIC)?1:0);
	disp_buf[5] = UI_ICON_EQ_JAZZ;	disp_ctrl[5] = ((eq == AUDIO_EQ_JAZZ)?1:0);
	disp_buf[6] = UI_ICON_NULL;

	ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
}

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/

void ui_show_init(void)
{
	memset(&ui_disp, 0, sizeof(ui_disp));
	ui_disp.front_level = DISP_LEVEL_NORMAL;
	ui_disp.now_level 	= DISP_LEVEL_NORMAL;
	ui_disp.prev_level 	= DISP_LEVEL_MAX;
	ui_puts("ui_show_init\n");
}

void ui_show_copy(ui_lang_id_e disp_id, uint8 *copy)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 dispchar[13];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 dispchar[11];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#else
	uint8 dispchar[8];
#endif

	if(ui_lang_res_search(disp_id, dispchar, sizeof(dispchar)))
	{
	#if defined(_FY7000_SPECIAL_DISPLAY_)
		memcpy(copy, dispchar, 13);
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
		memcpy(copy, dispchar, 11);
	#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
		memcpy(copy, dispchar, 10);
	#elif defined(_SL6812A_SPECIAL_DISPLAY_)
		memcpy(copy, dispchar, 10);
	#else
    	memcpy(copy, dispchar, 8);
	#endif
	}
}

void ui_show_string(uint8 level, const uint8 *str)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 dispchar[13];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 dispchar[11];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#else
	uint8 dispchar[8];
#endif
	uint8 i;
	uint8 flag = TRUE;

	for(i = 0; i < sizeof(dispchar); i++)
	{
		if(str[i] && flag)
		{
			dispchar[i] = str[i];
		}
		else
		{
			flag = FALSE;
			dispchar[i] = 0;
		}
	}
	ui_drv_func.display(level, 0, dispchar, sizeof(dispchar));
}

#if defined(_FY7555_LCD_DISPLAY_)
void ui_show_string1(uint8 level, const uint8 *str)
{
	uint8 dispchar[8];
	uint8 i;
	uint8 flag = TRUE;

	for(i = 0; i < sizeof(dispchar); i++)
	{
		if(str[i] && flag)
		{
			dispchar[i] = str[i];
		}
		else
		{
			flag = FALSE;
			dispchar[i] = 0;
		}
	}
	ui_drv_func.display_buff(level, 0, dispchar, sizeof(dispchar));
}
#endif

void ui_show_num(uint8 level, uint8 num)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	ui_drv_func.display(level, 13,&num, 1);
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	ui_drv_func.display(level, 11,&num, 1);
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	ui_drv_func.display(level, 10,&num, 1);
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	ui_drv_func.display(level, 10,&num, 1);
#else
	ui_drv_func.display(level, 8,&num, 1);
#endif
}

void ui_show_normal(ui_lang_id_e disp_id)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 dispchar[13];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 dispchar[11];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#else
	uint8 dispchar[8];
#endif

	if(ui_lang_res_search(disp_id, dispchar, sizeof(dispchar)))
	{
		show_clear_num(DISP_LEVEL_NORMAL);
		show_clear_icon(DISP_LEVEL_NORMAL);
		ui_show_string(DISP_LEVEL_NORMAL, dispchar);
	}
}

#if defined(_FY7555_LCD_DISPLAY_)
void ui_show_normal1(ui_lang_id_e disp_id)
{
	uint8 dispchar[8];

	if(ui_lang_lcd_search(disp_id, dispchar, sizeof(dispchar)))
	{
		ui_show_string1(DISP_LEVEL_NORMAL, dispchar);
	}
}
#endif

#if defined(_SUPPORT_CAN_UART_)
void ui_show_setting1(ui_lang_id_e disp_id,int32 value)
{
	uint8 dispchar[8];

	if(ui_lang_res_search(disp_id, dispchar, sizeof(dispchar)))
	{
    #if defined(_SUPPORT_CAN_UART_)
    	if(disp_id == LANG_ID_CAN_ERR)
    	{
    		show_can_err_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_CAN_SPEED)
    	{
    		show_can_speed_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_CAN_DISTANCE)
    	{
    		show_can_distance_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_CAN_ELECTRIC)
    	{
    		show_can_eleltric_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_CAN_GEARS)
    	{
    		show_can_gears_set(dispchar,value);
    	}
    #endif
	
		show_clear_num(DISP_LEVEL_SETTING);
		show_clear_icon(DISP_LEVEL_SETTING);
		ui_show_string(DISP_LEVEL_SETTING, dispchar);

		ui_disp.now_level = DISP_LEVEL_SETTING;
		ui_disp.timer = 3000;
	}
}
#endif

#if defined(_FY7555_LCD_DISPLAY_)
void ui_show_setting1(ui_lang_id_e disp_id,int32 value)
{
	uint8 dispchar[8];

	if(ui_lang_lcd_search(disp_id, dispchar, sizeof(dispchar)))
	{
		if(disp_id == LANG_ID_OTHERS_VOLUME)
		{
			//show_volume_set(dispchar,value);
		}
	
		ui_show_string1(DISP_LEVEL_SETTING, dispchar);

		ui_disp.now_level = DISP_LEVEL_SETTING;
		ui_disp.timer = 3000;
	}
}
#endif

void ui_show_setting(ui_lang_id_e disp_id,int8 value)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 dispchar[13];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 dispchar[11];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#else
	uint8 dispchar[8];
#endif

#if defined(_ENABLE_CLOCK_DISPLAY_)
	show_clear_icon(DISP_LEVEL_SETTING);
#endif

	if(ui_lang_res_search(disp_id, dispchar, sizeof(dispchar)))
	{
		if(disp_id == LANG_ID_OTHERS_VOLUME)
		{
			show_volume_set(dispchar,value);
		}
		if(disp_id == LANG_ID_OTHERS_BASS)
		{
			show_bass_set(dispchar,value);
		#if defined(_ENABLE_CLOCK_DISPLAY_)
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
		#endif
		}
    #if defined(_ENABLE_7419_MIDDLE_)
    	else if(disp_id == LANG_ID_OTHERS_MIDDLE)
    	{
    		show_middle_set(dispchar,value);
	    #if defined(_ENABLE_CLOCK_DISPLAY_)
	    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
	    #endif
    	}
    #endif
		else if(disp_id == LANG_ID_OTHERS_TREBLE)
		{
			show_treble_set(dispchar,value);
	    #if defined(_ENABLE_CLOCK_DISPLAY_)
	    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
	    #endif
		}
		else if(disp_id == LANG_ID_OTHERS_BAL)
		{
			show_bal_set(dispchar,value);
		#if defined(_WLS1403_SPECIAL_DISP_)||defined(_MENU_DISPLAY_CLOCK_ICON_)
	    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
		#endif
		}
		else if(disp_id == LANG_ID_OTHERS_FAD)
		{
			show_fad_set(dispchar,value);
	    #if defined(_WLS1403_SPECIAL_DISP_)||defined(_MENU_DISPLAY_CLOCK_ICON_)
	    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
	    #endif
		}
	#if defined(_SPECIAL_TL2000_SET_)
		if(disp_id == LANG_ID_OTHERS_A_VOLUME)
		{
			show_a_volume_set(dispchar,value);
		}
		if(disp_id == LANG_ID_OTHERS_B_VOLUME)
		{
			show_b_volume_set(dispchar,value);
		}
	#endif
	#if defined(_ENABLE_TALK_VOL_)
		if(disp_id == LANG_ID_OTHERS_TAIK_VOLUME)
		{
			show_talk_vol_set(dispchar,value);
		}
	#endif
	#if defined(_CHANGE_DEF_VOL_)
		if(disp_id == LANG_ID_OTHERS_P_VOL)
		{
			show_p_vol_set(dispchar,value);
		}
	#endif
    #if defined(_SUPPORT_SUBWOOFER_MODE2_)
    	else if(disp_id == LANG_ID_SUB)
    	{
    		show_sub_set(dispchar,value);
    	}
    #endif
    #if defined(AUDIO_SUB_LEVEL_SET)
    	else if(disp_id == LANG_ID_SUB_CUT_CTRL)
    	{
    		show_sub_cut_set(dispchar,value);
    	}
    #endif
	#if defined(_ENABLE_MENU_FUNTION_)||defined(_ENABLE_DEFAULT_CONTROL_)||defined(ADD_ADJUST_DEFAULT_VOL)
    	else if(disp_id == LANG_ID_OTHERS_DEFAULT_VOLUME)
    	{
    		show_default_vol_set(dispchar,value);
    	}
	#endif
    #if defined(ENABLE_TA_VOL_SET)
    	else if(disp_id == LANG_ID_RDS_TA_VOL)
    	{
    		show_tavol_set(dispchar,setup.rds.volume);
    	}
    #endif
	#if defined(_FAD_BAL_FUNTION_)
    	else if(disp_id == LANG_ID_FL)
    	{
    		show_fl_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_FR)
    	{
    		show_fr_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_RL)
    	{
    		show_rl_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_RR)
    	{
    		show_rr_set(dispchar,value);
    	}
	#endif
	#if defined(_SUPPORT_10_EQ_FUNTION_)
    	else if(disp_id == LANG_ID_OTHERS_60HZ)
    	{
    		show_60hz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_150HZ)
    	{
    		show_150hz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_400HZ)
    	{
    		show_400hz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_1KHZ)
    	{
    		show_1khz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_2KHZ)
    	{
    		show_2khz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_3KHZ)
    	{
    		show_3khz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_7KHZ)
    	{
    		show_7khz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_10KHZ)
    	{
    		show_10khz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_15KHZ)
    	{
    		show_15khz_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_OTHERS_20KHZ)
    	{
    		show_20khz_set(dispchar,value);
    	}
	#endif
    #if defined(_SUPPORT_CAN_UART_)
    	else if(disp_id == LANG_ID_CAN_P1)
    	{
    		show_can_p1_set(dispchar,value);
    	}
    	else if(disp_id == LANG_ID_CAN_P2)
    	{
    		show_can_p2_set(dispchar,value);
    	}
    #endif
    #if defined(_WLS1403_SPECIAL_DISP_)
    	else if(disp_id == LANG_ID_OTHERS_LOUD_ON)
    	{
    		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
    	}
    	else if(disp_id == LANG_ID_OTHERS_LOUD_OFF)
    	{
    		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
    	}
    #endif
		show_clear_num(DISP_LEVEL_SETTING);
	#if !defined(_ENABLE_CLOCK_DISPLAY_)
    	show_clear_icon(DISP_LEVEL_SETTING);
	#endif
		ui_show_string(DISP_LEVEL_SETTING, dispchar);

		ui_disp.now_level = DISP_LEVEL_SETTING;
	#ifdef	TD_ADD_ALEN
		if((disp_id ==LANG_ID_RADIO_MANUAL)
		||(disp_id ==LANG_ID_RADIO_AUTO))
		{//收音手自动 显示1.6S就行
			ui_disp.timer = 1600;
		}
		else
	#endif
		{
        #if defined(_ENABLE_MENU_DISPLAY_)
        	ui_disp.timer = 5000;
        #else
        	ui_disp.timer = 3000;
        #endif
		#if defined(_USER_TFT_SEND_)
			ui_disp.sel_timer = 3000;
		#endif
		}
	}
}

void ui_show_mute(uint8 mute)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	if(mute)
	{
	#if defined(HuiNuo_6651D_Copy)
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_MUTE);
		ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_MUTE);
	#elif defined(_SPECIAL_MUTE_FLASH_)
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_MUTE);
	#else
		ui_show_setting(LANG_ID_OTHERS_MUTE_ON,0);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_MUTE);
		ui_disp.timer = -1;
	#endif
	#if defined(_ENABLE_MENU_SET_)||defined(_ENABLE_NEXT_PRVE_SET_)
		global.mute_key_flag = TRUE;
	#endif
	}
	else
	{
		ui_disp.timer = 0;

	#if defined(HuiNuo_6651D_Copy)
		ui_drv_func.flash_disable( UI_ICON_MUTE);
	#endif
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MUTE);
	}
	ui_disp.func_sel = FUNC_SEL_NULL;
}

#if defined(_SUPPORT_DISC_)
void ui_show_disc_eject(uint8 eject)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	if(eject)
	{
		ui_show_setting(LANG_ID_DISC_EJECT,0);
		ui_disp.timer = -1;
	}
	else
	{
		ui_disp.timer = 0;
	}
	ui_disp.func_sel = FUNC_SEL_NULL;
}

#if defined(NEED_NO_DISC_DISP)
void ui_show_no_disc(uint8 eject)
{
	ui_disp.func_type = FUNC_TYPE_SEL;
	if(eject)
	{
		ui_show_setting(LANG_ID_NO_DISC,0);
		ui_disp.timer = -1;
	}
	else
	{
		ui_disp.timer = 0;
	}
	ui_disp.func_sel = FUNC_SEL_NULL;
}
#endif
#endif

void ui_show_vbar(uint8 enable, uint8 vol)
{
	if(enable)
	{
		ui_drv_func.vbar_disp(UI_LEVEL_ALL, vol);
	}
	else
	{
		ui_drv_func.vbar_clean(UI_LEVEL_ALL);
	}
}

#ifdef	TD_ADD_NEW_APP
void ui_show_clean(void)
{
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_LOUD);
	show_eq_icon(AUDIO_EQ_MAX);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EQ);

}
#endif

#if defined(_SUPPORT_TWO_PT2313_)
void ui_show_setting1(ui_lang_id_e disp_id,int32 value)
{
	uint8 dispchar[8];

	if(ui_lang_res_search(disp_id, dispchar, sizeof(dispchar)))
	{
    	if(disp_id == LANG_ID_OTHERS_VOLUME)
    	{
    		show_volume_set(dispchar,value);
    	}
		
		show_clear_num(DISP_LEVEL_SETTING);
		show_clear_icon(DISP_LEVEL_SETTING);
		ui_show_string(DISP_LEVEL_SETTING, dispchar);

		ui_disp.now_level = DISP_LEVEL_SETTING;
		ui_disp.timer = 3000;
	}
}

void ui_show_sel1(func_sel_e func_sel)
{
	ui_lang_id_e lang_id = LANG_ID_NULL;
	int8 disp_value = 0;
	lang_id = LANG_ID_OTHERS_VOLUME;
	disp_value = get_cur_vol2();
	ui_show_setting1(lang_id,disp_value);
}
#endif

void ui_show_sel(func_sel_e func_sel)
{
	ui_lang_id_e lang_id = LANG_ID_NULL;
	int8 disp_value = 0;
#if defined(_SUPPORT_CAN_UART_)
	int32 disp_value1 = 0;
#endif

	switch(func_sel)
	{
		case FUNC_SEL_VOLUME:
			lang_id = LANG_ID_OTHERS_VOLUME;
			disp_value = get_cur_vol();
			break;
		case FUNC_SEL_BASS:
			lang_id = LANG_ID_OTHERS_BASS;
			disp_value = setup.aud_bass;
			break;
    #if defined(_ENABLE_7419_MIDDLE_)
    	case FUNC_SEL_MIDDLE:
    		lang_id = LANG_ID_OTHERS_MIDDLE;
    		disp_value = setup.aud_middle;
    		break;
    #endif
		case FUNC_SEL_TREBLE:
			lang_id = LANG_ID_OTHERS_TREBLE;
			disp_value = setup.aud_treble;
			break;
	#if !defined(_FAD_BAL_FUNTION_)
		case FUNC_SEL_BAL:
			lang_id = LANG_ID_OTHERS_BAL;
			disp_value = setup.aud_bal;
			break;
		case FUNC_SEL_FAD:
			lang_id = LANG_ID_OTHERS_FAD;
			disp_value = setup.aud_fad;
			break;
	#endif
		case FUNC_SEL_LOUD:
			if(setup.aud_loud == AUDIO_LOUDNESS_OFF)
	        {
				lang_id = LANG_ID_OTHERS_LOUD_OFF;
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_LOUD);
	        }
			else
			{
				lang_id = LANG_ID_OTHERS_LOUD_ON;
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_LOUD);
			}
			break;
		case FUNC_SEL_EQ:
			switch(setup.aud_eq)
			{
				case AUDIO_EQ_OFF:
					lang_id = LANG_ID_OTHERS_EQ_OFF;
					break;
				case AUDIO_EQ_FLAT:
					lang_id = LANG_ID_OTHERS_EQ_FLAT;
					break;
				case AUDIO_EQ_CLASSIC:
					lang_id = LANG_ID_OTHERS_EQ_CLASSIC;
					break;
				case AUDIO_EQ_POP:
					lang_id = LANG_ID_OTHERS_EQ_POP;
					break;
				case AUDIO_EQ_ROCK:
					lang_id = LANG_ID_OTHERS_EQ_ROCK;
					break;
				case AUDIO_EQ_JAZZ:
					lang_id = LANG_ID_OTHERS_EQ_JAZZ;
					break;
			#if defined(_ENABLE_AP3768_EQ_MODE_)
    			case AUDIO_EQ_PEOPLE:
    				lang_id = LANG_ID_OTHERS_EQ_PEOPLE;
    				break;
				case AUDIO_EQ_SOFT:
					lang_id = LANG_ID_OTHERS_EQ_SOFT;
					break;
				case AUDIO_EQ_ELECTRIC:
					lang_id = LANG_ID_OTHERS_EQ_ELECTRIC;
					break;
			#endif
				default:
					lang_id = LANG_ID_OTHERS_EQ_OFF;
					break;
			}
			show_eq_icon(setup.aud_eq);
			break;
		
    #if defined(_ENABLE_RPT_RDM_FUNTION_)
    	case FUNC_SEL_RPT:
    		if(setup.rpt_start == MUSIC_RPT_ONE)
    		{
    			lang_id = LANG_ID_MUSIC_MODE_REPEAT_ONE;
    			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RPT);
    		}
    		else if(setup.rpt_start == MUSIC_RPT_FOLDER)
    		{
    			lang_id = LANG_ID_MUSIC_MODE_REPEAT_FOLDER;
    			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RPT);
    		}
    		else
    		{
    			lang_id = LANG_ID_MUSIC_MODE_REPEAT_OFF;
    			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RPT);
    		}
    		break;
    
    	case FUNC_SEL_RDM:
    		if(setup.rdm_start)
    		{
    			lang_id = LANG_ID_MUSIC_MODE_RDM_ON;
    			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RDM);
    		}
    		else
    		{
    			lang_id = LANG_ID_MUSIC_MODE_RDM_OFF;
    			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RDM);
    		}
    		break;
    #endif
	
	#if defined(_SPECIAL_TL2000_SET_)		
		case FUNC_SEL_A_VOLUME:
			lang_id = LANG_ID_OTHERS_A_VOLUME;
			disp_value = setup.aud_A_volume;
			break;
		case FUNC_SEL_B_VOLUME:
			lang_id = LANG_ID_OTHERS_B_VOLUME;
			disp_value = setup.aud_B_volume;
			break;
	#endif
    #if defined(_ENABLE_TALK_VOL_)
    	case FUNC_SEL_TALK_VOL:
			lang_id = LANG_ID_OTHERS_TAIK_VOLUME;
			disp_value = setup.talk_volume;
    		break;
    #endif
	#if defined(_ENABLE_ID3_ON_OFF_)||defined(_ENABLE_ID3_SET_)
    	case FUNC_SEL_ID3:
    		if(setup.id3_on_flag)
    		{
    			lang_id = LANG_ID_ID3_ON;
    		}
    		else
    		{
    			lang_id = LANG_ID_ID3_OFF;
    		}
    		break;
	#endif
	#if defined(_ENABLE_BT_ON_OFF_)
    	case FUNC_SEL_BT_ON:
			if(setup.bt_on)
	        {
				lang_id = LANG_ID_OTHERS_BT_ON;
	        }
			else
			{
				lang_id = LANG_ID_OTHERS_BT_OFF;
			}
    		break;
	#endif
	#if defined(_CHANGE_DEF_VOL_)		
		case FUNC_SEL_DEFVOLUME:
			lang_id = LANG_ID_OTHERS_P_VOL;
			disp_value = setup.DefVol;
			break;
	#endif
	#if defined(_SUPPORT_SUBWOOFER_)
		case FUNC_SEL_SUBWOOFER:
			if(setup.aud_subwoofer == AUDIO_SUBWOOFER_OFF)
	        {
				lang_id = LANG_ID_OTHERS_SUBWOOFER_OFF;
				ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
	        }
			else
			{
				lang_id = LANG_ID_OTHERS_SUBWOOFER_ON;
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
			}
			break;
    #endif
    #if defined(_SUPPORT_SUBWOOFER2_)||defined(_ENABLE_AP3768_SUBWOOFER_)
    	case FUNC_SEL_SUB:
        	if(setup.aud_sub == 0)
        	{
        		lang_id = LANG_ID_OTHERS_SUBWOOFER_OFF;
        		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
        	}
        	else
        	{
        		lang_id = LANG_ID_OTHERS_SUBWOOFER_ON;
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SUBWOOFER);
        	}
        	break;
    #endif
	#if defined(_SUPPORT_SUBWOOFER3_)||defined(_ENABLE_AP3768_SUBWOOFER_)
    	case FUNC_SEL_SUB1:
    		if(setup.aud_sub1 == 0)
    		{
    			lang_id = LANG_ID_OTHERS_SUBWOOFER1_OFF;
    		}
    		else
    		{
    			lang_id = LANG_ID_OTHERS_SUBWOOFER1_ON;
    		}
    		break;
	#endif
    #if defined(_SUPPORT_SUBWOOFER_MODE2_)
    	case FUNC_SEL_SUBWOOFER:
    		lang_id = LANG_ID_SUB;
    		disp_value = setup.aud_subwoofer;
    		break;
    #endif

    #if defined(AUDIO_SUB_LEVEL_SET)
    	case FUNC_SEL_SUB_CUT_CTRL:
    		lang_id = LANG_ID_SUB_CUT_CTRL;
    		disp_value = setup.aud_sub_ctrl;
    		break;
    #endif
	
	#if defined(_FAD_BAL_FUNTION_)
		case FUNC_SEL_FL:
			lang_id = LANG_ID_FL;
			disp_value = setup.aud_fl;
    		break;
		case FUNC_SEL_FR:
			lang_id = LANG_ID_FR;
			disp_value = setup.aud_fr;
			break;
		case FUNC_SEL_RL:
			lang_id = LANG_ID_RL;
			disp_value = setup.aud_rl;
			break;
		case FUNC_SEL_RR:
			lang_id = LANG_ID_RR;
			disp_value = setup.aud_rr;
			break;
	#endif
	
	#if defined(_SUPPORT_10_EQ_FUNTION_)
    	case FUNC_SEL_60HZ:
			lang_id = LANG_ID_OTHERS_60HZ;
			disp_value = setup.aud_60hz;
    		break;
		case FUNC_SEL_150HZ:
			lang_id = LANG_ID_OTHERS_150HZ;
			disp_value = setup.aud_150hz;
			break;
		case FUNC_SEL_400HZ:
			lang_id = LANG_ID_OTHERS_400HZ;
			disp_value = setup.aud_400hz;
			break;
		case FUNC_SEL_1KHZ:
			lang_id = LANG_ID_OTHERS_1KHZ;
			disp_value = setup.aud_1khz;
			break;
		case FUNC_SEL_2KHZ:
			lang_id = LANG_ID_OTHERS_2KHZ;
			disp_value = setup.aud_2khz;
			break;
		case FUNC_SEL_3KHZ:
			lang_id = LANG_ID_OTHERS_3KHZ;
			disp_value = setup.aud_3khz;
			break;
		case FUNC_SEL_7KHZ:
			lang_id = LANG_ID_OTHERS_7KHZ;
			disp_value = setup.aud_7khz;
			break;
		case FUNC_SEL_10KHZ:
			lang_id = LANG_ID_OTHERS_10KHZ;
			disp_value = setup.aud_10khz;
			break;
		case FUNC_SEL_15KHZ:
			lang_id = LANG_ID_OTHERS_15KHZ;
			disp_value = setup.aud_15khz;
			break;
		case FUNC_SEL_20KHZ:
			lang_id = LANG_ID_OTHERS_20KHZ;
			disp_value = setup.aud_20khz;
			break;
	#endif
	#if defined(_ENABLE_BEEP_)
		case FUNC_SEL_BEEP:
			if(setup.aud_beep)
	        {
	        	lang_id = LANG_ID_OTHERS_BEEP_ON;
	        }
			else
			{
				lang_id = LANG_ID_OTHERS_BEEP_OFF;
			}
			break;
	#endif
	#if defined(_SUPPORT_AREA_)
    	case FUNC_SEL_AREA:
			switch(setup.area)
			{
				default:
				case SETUP_AREA_EUR:
					lang_id = LANG_ID_RADIO_AREA_EUR;
				#if defined(_SEL_ADD_TIME_SELECT_)
				#if !defined(_DISABLE_TIME_AREA_)
			    	setup.rtc_disp = RTC_DISP_24H;
			    #endif
				#endif
					break;
				case SETUP_AREA_USA:
					lang_id = LANG_ID_RADIO_AREA_USA;
				#if defined(_SEL_ADD_TIME_SELECT_)
				#if !defined(_DISABLE_TIME_AREA_)
			    	setup.rtc_disp = RTC_DISP_12H;
			    #endif
				#endif
					break;
			#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
    			case SETUP_AREA_LATIN:
    				lang_id = LANG_ID_RADIO_AREA_LATIN;
				#if defined(_SEL_ADD_TIME_SELECT_)
			    	setup.rtc_disp = RTC_DISP_12H;
			    #endif
    				break;
			#endif
		    #if defined(_SUPPORT_FOUR_AREA_)
		    	case SETUP_AREA_ASIA:
		    		lang_id = LANG_ID_RADIO_AREA_ASIA;
		        #if defined(_SEL_ADD_TIME_SELECT_)
		        	setup.rtc_disp = RTC_DISP_24H;
		        #endif
		    		break;
		    #endif
			}
			break;
    #endif
	#if defined(_SEL_ADD_TIME_SELECT_)
    	case FUNC_SEL_CLOCK:
    		switch(setup.rtc_disp)
    		{
    			default:
    			case RTC_DISP_24H:
    				lang_id = LANG_ID_24H;
    				break;
    			case RTC_DISP_12H:
    				lang_id = LANG_ID_12H;
    				break;
    		}
    		break;
	#endif

    #if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
	#if defined(_SUPPORT_THREE_LIGHT_)
    	case FUNC_SEL_7COLOR:
    		switch(setup.State)
    		{
    			case PANEL_LIGHT_STATE_RED:
    				lang_id = LANG_ID_RED;
    				break;
    			case PANEL_LIGHT_STATE_BLUE:
    				lang_id = LANG_ID_BLUE;
    				break;
    			case PANEL_LIGHT_STATE_FUCHSINE:
    				lang_id = LANG_ID_FUCHSINE;
    				break;
    			case PANEL_LIGHT_STATE_AUTO:
    				lang_id = LANG_ID_AUTO;
    				break;
    			default:
    				break;
    		}
    		break;

	#else
    	case FUNC_SEL_7COLOR:
    		ew_printf("setup.State:%d\n",setup.State);
		#if defined(_ENABLE_LIGHT_SHADE_)
			if(setup.State == 8)
			{
				ew_printf("setup.save_light1:%d\n",setup.save_light1);
				switch(setup.save_light1)
				{
					case PANEL_LIGHT_STATE_BLUE:
						lang_id = LANG_ID_BLUE;
						break;
					case PANEL_LIGHT_STATE_GREEN:
						lang_id = LANG_ID_GREEN;
						break;
					case PANEL_LIGHT_STATE_CYAN:
						lang_id = LANG_ID_CYAN;
						break;
					case PANEL_LIGHT_STATE_RED:
						lang_id = LANG_ID_RED;
						break;
					case PANEL_LIGHT_STATE_FUCHSINE:
						lang_id = LANG_ID_FUCHSINE;
						break;
					case PANEL_LIGHT_STATE_YELLOW:
						lang_id = LANG_ID_YELLOW;
						break;
					case PANEL_LIGHT_STATE_WHITE:
						lang_id = LANG_ID_WHITE;
						break;
					case PANEL_LIGHT_STATE_AUTO:
						lang_id = LANG_ID_AUTO;
						break;
					default:
						break;
				}
			}
			else
			{
				switch(setup.State)
				{
					case PANEL_LIGHT_STATE_BLUE:
						lang_id = LANG_ID_BLUE;
						break;
					case PANEL_LIGHT_STATE_GREEN:
						lang_id = LANG_ID_GREEN;
						break;
					case PANEL_LIGHT_STATE_CYAN:
						lang_id = LANG_ID_CYAN;
						break;
					case PANEL_LIGHT_STATE_RED:
						lang_id = LANG_ID_RED;
						break;
					case PANEL_LIGHT_STATE_FUCHSINE:
						lang_id = LANG_ID_FUCHSINE;
						break;
					case PANEL_LIGHT_STATE_YELLOW:
						lang_id = LANG_ID_YELLOW;
						break;
					case PANEL_LIGHT_STATE_WHITE:
						lang_id = LANG_ID_WHITE;
						break;
					case PANEL_LIGHT_STATE_AUTO:
						lang_id = LANG_ID_AUTO;
						break;
					default:
						break;
				}
			}
		#else
    		switch(setup.State)
    		{
    			case PANEL_LIGHT_STATE_BLUE:
    				lang_id = LANG_ID_BLUE;
    				break;
    			case PANEL_LIGHT_STATE_GREEN:
    				lang_id = LANG_ID_GREEN;
    				break;
    			case PANEL_LIGHT_STATE_CYAN:
    				lang_id = LANG_ID_CYAN;
    				break;
    			case PANEL_LIGHT_STATE_RED:
    				lang_id = LANG_ID_RED;
    				break;
    			case PANEL_LIGHT_STATE_FUCHSINE:
    				lang_id = LANG_ID_FUCHSINE;
    				break;
    			case PANEL_LIGHT_STATE_YELLOW:
    				lang_id = LANG_ID_YELLOW;
    				break;
    			case PANEL_LIGHT_STATE_WHITE:
    				lang_id = LANG_ID_WHITE;
    				break;
    			case PANEL_LIGHT_STATE_AUTO:
    				lang_id = LANG_ID_AUTO;
    				break;
    			default:
    				break;
    		}
		#endif
    		break;
	#endif
    #endif

	#if defined(SUPPORT_LED_ADJUST)
    	case FUNC_SEL_LED:
    		ew_printf("setup.LedState:%d\n",setup.LedState);
    		switch(setup.LedState)
    		{
    			case BACK_LED_LOW:
    				lang_id = LANG_ID_LED_LOW;
    				break;
    			case BACK_LED_MID:
    				lang_id = LANG_ID_LED_MID;
    				break;
    			case BACK_LED_HIGH:
    				lang_id = LANG_ID_LED_HIGH;
    				break;
    			default:
    				break;
    		}
    		break;	
	#endif
		
	#if defined(_ADJUST_A_B_SPK_)
		case FUNC_SEL_A_SPK:
			if(setup.A_SPK)
	        {
	        	lang_id = LANG_ID_A_SPK_ON;
	        }
			else
			{
				lang_id = LANG_ID_A_SPK_OFF;
			}
			break;
		case FUNC_SEL_B_SPK:
			if(setup.B_SPK)
	        {
	        	lang_id = LANG_ID_B_SPK_ON;
	        }
			else
			{
				lang_id = LANG_ID_B_SPK_OFF;
			}
			break;
	#endif

	#if defined(_MR889_SPECAIL_KEY_)
		case FUNC_SEL_CONTROL_SPK:
			if(setup.CONTROL_SPK)
	        {
	        	lang_id = LANG_ID_CONTROL_SPK_ON;
	        }
			else
			{
				lang_id = LANG_ID_CONTROL_SPK_OFF;
			}
			break;
	#endif	
	
	#if defined(_ENABLE_RDS_ON_OFF_)
    	case FUNC_RDS_ONOFF:
    		if(setup.rds_onoff)
    		{
    			lang_id = LANG_ID_RDS_ON;
    		}
    		else
    		{
    			lang_id = LANG_ID_RDS_OFF;
    		}
    		break;
	#endif
	
	#if defined(SUPPORT_ALARM_FUNC_)
		case FUNC_SEL_ALARM:
			if(setup.alarm)
	        {
	        	lang_id = LANG_ID_OTHERS_ALARM_ON;
	        }
			else
			{
				lang_id = LANG_ID_OTHERS_ALARM_OFF;
			}
			break;
	#endif	
	
	#if defined(_ENABLE_TIME_DISP_)
    	case FUNC_SEL_TIME_DISP:
    		if(setup.time_disp)
    		{
    			lang_id = LANG_ID_OTHERS_DISP_ON;
    		}
    		else
    		{
    			lang_id = LANG_ID_OTHERS_DISP_OFF;
    		}
    		break;
	#endif
	
    #if defined(_SUPPORT_CAN_UART_)
    	case FUNC_SEL_CAN_DISP:
			//ew_printf("global.can.disp:%d\n",global.can.disp);
			switch(global.can.disp)
			{
				case AUDIO_CAN_ERR:
					lang_id = LANG_ID_CAN_ERR;
					disp_value1 = global.can.err;
					break;
				case AUDIO_CAN_SPEED:
					lang_id = LANG_ID_CAN_SPEED;
					disp_value1 = global.can.speed;
					break;
				case AUDIO_CAN_DISTANCE:
					lang_id = LANG_ID_CAN_DISTANCE;
					disp_value1 = global.can.total_distance;
					break;
				case AUDIO_CAN_ELECTRIC:
					lang_id = LANG_ID_CAN_ELECTRIC;
					disp_value1 = global.can.electric;
					break;
				case AUDIO_CAN_GEARS:
					lang_id = LANG_ID_CAN_GEARS;
					disp_value1 = global.can.gears;
					break;
				default:
					lang_id = LANG_ID_CAN_ERR;
					disp_value1 = global.can.err;
					break;
			}
    		break;
    		
    	case FUNC_SEL_MODE:
			lang_id = LANG_ID_CAN_P1;
			disp_value = setup.can_mode;
    		break;
    		
    	case FUNC_SEL_SPEED:
			lang_id = LANG_ID_CAN_P2;
			disp_value = setup.can_speed;
    		break;
    #endif
	
    #if defined(_SUPPORT_SUB_)
    	case FUNC_SEL_SUB:
        	if(setup.sub_start)
        	{
        		lang_id = LANG_ID_OTHERS_SUB_EXT;
        	}
        	else
        	{
        		lang_id = LANG_ID_OTHERS_SUB_IN;
        	}
        	break;
    #endif

	#if defined(_ENABLE_MENU_FUNTION_)
		case FUNC_SEL_SETUP:
			if(setup.setup)
	        {
	        	lang_id = LANG_ID_OTHERS_SETUP;
	        }
			else
			{
				lang_id = LANG_ID_OTHERS_SETUP;
			}
    		break;
			
		case FUNC_SEL_DEFAULT_VOL:
			lang_id = LANG_ID_OTHERS_DEFAULT_VOLUME;
			disp_value = setup.default_vol;
			break;
			
		case FUNC_SEL_LOG:
			if(setup.log)
	        {
	        	lang_id = LANG_ID_OTHERS_LOG_ON;
	        }
			else
			{
				lang_id = LANG_ID_OTHERS_LOG_OFF;
			}
			break;
	#endif

	#if defined(_ENABLE_DEFAULT_CONTROL_)||defined(ADD_ADJUST_DEFAULT_VOL)
    	case FUNC_SEL_DEFAULT_VOL:
    		lang_id = LANG_ID_OTHERS_DEFAULT_VOLUME;
    		disp_value = setup.default_vol;
    		break;
	#endif
	
		case FUNC_SEL_LOC:
			{
				uint8 *com_loc;
				if((func_sel_check != NULL)&&((com_loc = (uint8 *)func_sel_check(FUNC_SEL_LOC)) != NULL))
				{
					if(*com_loc)
					{
						lang_id = LANG_ID_RADIO_LOC_ON;
						setup.loc_flg = TRUE;
					}
					else
					{
						lang_id = LANG_ID_RADIO_LOC_OFF;
						setup.loc_flg = FALSE;
					}
				}
			}
			break;
		case FUNC_SEL_ST:
			{
				uint8 *com_st;
				if((func_sel_check != NULL)&&((com_st = (uint8 *)func_sel_check(FUNC_SEL_ST)) != NULL))
				{
					if(*com_st)
					{
						lang_id = LANG_ID_RADIO_ST_ON;
						setup.mono_flg = FALSE;
					}
					else
					{
						lang_id = LANG_ID_RADIO_ST_OFF;
						setup.mono_flg = TRUE;
					}
				}
			}
			break;
		default:
		#if defined(_ENABLE_RADIO_RDS_)
			if((func_sel > FUNC_RDS_BEGIN)&&(func_sel < FUNC_RDS_END))
			{
				extern uint8 radio_rds_lang(func_sel_e, ui_lang_id_e *);
				if(radio_rds_lang(func_sel, &lang_id))break;
			}
		#endif
			return;
	}

#if defined(_SUPPORT_CAN_UART_)
	if(func_sel == FUNC_SEL_CAN_DISP)
	{
		ui_show_setting1(lang_id,disp_value1);
	}
	else
	{
		ui_show_setting(lang_id,disp_value);
	}
#else
	ui_show_setting(lang_id,disp_value);
#endif

#if defined(_FY7555_LCD_DISPLAY_)
	ui_show_setting1(LANG_ID_SET, 0);
#endif
}

#if defined(_ENABLE_REALTIME_DIAPLAY_)||defined(_TEMP_WARM_DISPLAY_)
void ui_show_string1(uint8 level, const uint8 *str)
{
	uint8 dispchar[4];
	uint8 i;
	uint8 flag = TRUE;

	for(i = 0; i < sizeof(dispchar); i++)
	{
		if(str[i] && flag)
		{
			dispchar[i] = str[i];
		}
		else
		{
			flag = FALSE;
			dispchar[i] = 0;
		}
	}
	
#if defined(DEBGE_LED_AND_LCD_TOGETHER)
	ui_drv_func.led_display_icon(level, 0, dispchar, sizeof(dispchar));
#else
	ui_drv_func.display_icon(level, 0, dispchar, sizeof(dispchar));
#endif
}

void ui_show_rtc(const uint8 *dispchar, uint8 clock_dot)
{
#if defined(_FY5517_POWER_OFF_TIME_)
	uint8 dispchar1[4];
#endif

	if(dispchar != NULL)
	{
	#if defined(_FY5517_POWER_OFF_TIME_)
		if(global.sys_power)
		{
			ui_show_string1(UI_LEVEL_ALL, dispchar);
		}
		else
		{
			dispchar1[0] = ' ';
			dispchar1[1] = ' ';
			dispchar1[2] = ' ';
			dispchar1[3] = ' ';
			ui_show_string(DISP_LEVEL_SETTING, dispchar);
			ui_show_string1(UI_LEVEL_ALL, dispchar1);
		}
	#elif defined(_KLD3300_POWER_OFF_TIME_)
		if(global.sys_power)
		{
			ui_show_string1(UI_LEVEL_ALL, dispchar);
		}
		else
		{
			dispchar1[9] = ' ';
			dispchar1[10] = ' ';
			dispchar1[11] = ' ';
			dispchar1[12] = ' ';
			ui_show_string(DISP_LEVEL_SETTING, dispchar);
			ui_show_string1(UI_LEVEL_ALL, dispchar1);
		}		
	#else
		ui_show_string1(UI_LEVEL_ALL, dispchar);
	#endif
	}

	if(clock_dot != 0xFF)
	{
	#if defined(DEBGE_LED_AND_LCD_TOGETHER)
		uint16 disp_buf[3];
		uint8  disp_ctrl[2];

    	disp_buf[0] = UI_ICON_TIME1;	disp_ctrl[0] = clock_dot;
		disp_buf[1] = UI_ICON_TIME2;	disp_ctrl[1] = clock_dot;
		disp_buf[2] = UI_ICON_NULL1;
		ui_drv_func.drv_led_icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
	#else
		uint16 disp_buf[2];
		uint8  disp_ctrl[1];

	#if defined(_FY5517_POWER_OFF_TIME_)||defined(_KLD3300_POWER_OFF_TIME_)
		disp_buf[1] = UI_ICON_NULL;
		if(global.sys_power)
		{
			disp_buf[0] = UI_ICON_TIME; disp_ctrl[0] = clock_dot;
			ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
		}
		else
		{
			disp_buf[0] = UI_ICON_CLOCK; disp_ctrl[0] = clock_dot;
			ui_drv_func.icon_ctrl(DISP_LEVEL_SETTING, disp_buf, disp_ctrl);
		#if defined(_SPECIAL_POWER_OFF_DIAPLAY_)
			ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_TIME);
		#endif	
		}
	#elif defined(_TEMP_WARM_DISPLAY_)
    	disp_buf[0] = UI_ICON_NULL; disp_ctrl[0] = clock_dot;
    	disp_buf[1] = UI_ICON_NULL;
    	ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
	#else
    	disp_buf[0] = UI_ICON_TIME;	disp_ctrl[0] = clock_dot;
		disp_buf[1] = UI_ICON_NULL;
		ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
	#endif
	#endif
	}
}

#elif defined(_SUPPORT_TIME_DISPLAY_)
void ui_show_string1(uint8 level, const uint8 *str)
{
	uint8 dispchar[13];
	uint8 i;
	uint8 flag = TRUE;

	for(i = 0; i < sizeof(dispchar); i++)
	{
		if(str[i] && flag)
		{
			dispchar[i] = str[i];
		}
		else
		{
			flag = FALSE;
			dispchar[i] = 0;
			
		}
	}
	ui_drv_func.display_icon(level, 0, dispchar, sizeof(dispchar));
}

void ui_show_rtc(const uint8 *dispchar, uint8 clock_dot)
{
	if(dispchar != NULL)
	{
		ui_show_string1(UI_LEVEL_ALL, dispchar);
	}

	if(clock_dot != 0xFF)
	{
		uint16 disp_buf[2];
		uint8  disp_ctrl[1];

    	disp_buf[0] = UI_ICON_TIME;	disp_ctrl[0] = clock_dot;
		disp_buf[1] = UI_ICON_NULL;
		ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
	}
}

#elif defined(_ENABLE_TIME_DISP_)
void ui_show_rtc(const uint8 *dispchar, uint8 clock_dot)
{
	if(setup.time_disp)
	{
		if(dispchar != NULL)
		{
			ui_show_string(UI_LEVEL_ALL, dispchar);
			ui_show_num(UI_LEVEL_ALL, 0xFF);
			ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_CH);
		}
		
		if(clock_dot != 0xFF)
		{
			uint16 disp_buf[3];
			uint8  disp_ctrl[2];
		
	    #if defined(_ENABLE_TIME_DISPLAY_)
	    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = clock_dot;
	    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = clock_dot;
	    #else
	    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = 0;
	    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = clock_dot;
	    #endif
			disp_buf[2] = UI_ICON_NULL;
		
			ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
		}
	}
	else
	{
		if(dispchar != NULL)
		{
			ui_show_string(DISP_LEVEL_SETTING, dispchar);
			ui_show_num(DISP_LEVEL_SETTING, 0xFF);
			ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_CH);
		}
		
		if(clock_dot != 0xFF)
		{
			uint16 disp_buf[3];
			uint8  disp_ctrl[2];
		
	    #if defined(_ENABLE_TIME_DISPLAY_)
	    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = clock_dot;
	    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = clock_dot;
	    #else
	    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = 0;
	    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = clock_dot;
	    #endif
			disp_buf[2] = UI_ICON_NULL;
		
			ui_drv_func.icon_ctrl(DISP_LEVEL_SETTING, disp_buf, disp_ctrl);
		}
	}
}

#else
void ui_show_rtc(const uint8 *dispchar, uint8 clock_dot)
{
	if(dispchar != NULL)
	{
		ui_show_string(DISP_LEVEL_SETTING, dispchar);
		ui_show_num(DISP_LEVEL_SETTING, 0xFF);
		ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_CH);
	}

	if(clock_dot != 0xFF)
	{
		uint16 disp_buf[3];
		uint8  disp_ctrl[2];

	#if defined(_ENABLE_TIME_DISPLAY_)
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = clock_dot;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = clock_dot;
	#else
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = 0;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = clock_dot;
	#endif
		disp_buf[2] = UI_ICON_NULL;
	
	#if defined(_SPECIAL_LCD_4_DIAPLAY_)
		ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_FM);
	#endif	
		ui_drv_func.icon_ctrl(DISP_LEVEL_SETTING, disp_buf, disp_ctrl);
	}
}
#endif

#if defined(SUPPORT_ALARM_FUNC_)
void ui_show_rtc1(const uint8 *dispchar, uint8 clock_dot)
{
	if(dispchar != NULL)
	{
		ui_show_string(DISP_LEVEL_SETTING, dispchar);
		ui_show_num(DISP_LEVEL_SETTING, 0xFF);
		ui_drv_func.icon_close(DISP_LEVEL_SETTING, UI_ICON_CH);
	}

	if(clock_dot != 0xFF)
	{
		uint16 disp_buf[3];
		uint8  disp_ctrl[2];

    #if defined(_ENABLE_TIME_DISPLAY_)
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = clock_dot;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = clock_dot;
    #else
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = 0;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = clock_dot;
    #endif
		disp_buf[2] = UI_ICON_NULL;
		ui_drv_func.icon_ctrl(DISP_LEVEL_SETTING, disp_buf, disp_ctrl);
	}
}
#endif

void ui_show_com(const uint8 *dispchar, uint8 point_dot)
{
	if(dispchar != NULL)
	{
		ui_show_string(DISP_LEVEL_NORMAL, dispchar);
	}

	if(point_dot != 0xFF)
	{
	#if defined(_FIVE_CHAR_DISPLAY_)
		uint16 disp_buf[4];
		uint8  disp_ctrl[3];
	#else
		uint16 disp_buf[3];
		uint8  disp_ctrl[2];
	#endif

    #if defined(_ENABLE_TIME_DISPLAY_)
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = point_dot;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = point_dot;
		disp_buf[2] = UI_ICON_NULL;
	#elif defined(_FY5517_SPECIAL_DISPLAY_)
    	disp_buf[0] = UI_ICON_CLOCK2;	disp_ctrl[0] = point_dot;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = point_dot;
    	disp_buf[2] = UI_ICON_NULL;
    #elif defined(_FIVE_CHAR_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
		disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = 0;
		disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = point_dot;
		disp_buf[2] = UI_ICON_DASH; 	disp_ctrl[2] = point_dot;
		disp_buf[3] = UI_ICON_NULL;
	#elif defined(_JL8253_TIME_DISPLAY_)
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = point_dot;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = 0;
    	disp_buf[2] = UI_ICON_NULL;
	#elif defined(_HH5259_TIME_DISPLAY_)
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = point_dot;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = 0;
    	disp_buf[2] = UI_ICON_NULL;	
	#elif defined(_JL8258_TIME_DISPLAY_)
		disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = point_dot;
		disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = 0;
		disp_buf[2] = UI_ICON_NULL;
	#elif defined(_FY2010_LCD_DISPLAY_)
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = point_dot;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = 0;
    	disp_buf[2] = UI_ICON_NULL;
	#elif defined(_SL6812_PLAY_TIME_)
	#if defined(_SL6812A_SPECIAL_DISPLAY_)
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = 0;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = point_dot;
    	disp_buf[2] = UI_ICON_NULL;
	#else
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = point_dot;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = 0;
    	disp_buf[2] = UI_ICON_NULL;
	#endif
	#else
    	disp_buf[0] = UI_ICON_POINT;	disp_ctrl[0] = 0;
    	disp_buf[1] = UI_ICON_CLOCK;	disp_ctrl[1] = point_dot;
		disp_buf[2] = UI_ICON_NULL;
    #endif

		ui_drv_func.icon_ctrl(DISP_LEVEL_NORMAL, disp_buf, disp_ctrl);
	}
}

#if defined(_ENABLE_RADIO_RDS_)
void ui_show_rds_icon(int icon_ctrl)
{
enum{
	RDS_ICON_AF = 0,
	RDS_ICON_TA,
	RDS_ICON_TP,
	RDS_ICON_PTY,
	RDS_ICON_REG,
	RDS_ICON_EON,
	RDS_ICON_MAX,
};
enum{
	RDS_ICON_CTRL_CLOSE = _B0_,
	RDS_ICON_CTRL_OPEN = _B1_,
	RDS_ICON_CTRL_FLASH_ENABLE = _B2_,
	RDS_ICON_CTRL_FLASH_DISABLE = _B3_,
};

	uint8 rds_icon = icon_ctrl & 0xFF;
	uint8 rds_ctrl = (icon_ctrl >> 8) & 0xFF;
	ui_icon_e ui_icon = UI_ICON_NULL;

	switch(rds_icon)
	{
		case RDS_ICON_AF:	ui_icon = UI_ICON_AF;	break;
		case RDS_ICON_TA:	ui_icon = UI_ICON_TA;	break;
		case RDS_ICON_TP:	ui_icon = UI_ICON_TP;	break;
		case RDS_ICON_PTY:	ui_icon = UI_ICON_PTY;	break;
		case RDS_ICON_REG:	ui_icon = UI_ICON_REG;	break;
		case RDS_ICON_EON:	ui_icon = UI_ICON_EON;	break;
	}

	if(ui_icon != UI_ICON_NULL)
	{
		if(rds_ctrl & RDS_ICON_CTRL_FLASH_ENABLE)
		{
			ui_drv_func.flash_enable(UI_LEVEL_ALL, ui_icon);
		}
		if(rds_ctrl & RDS_ICON_CTRL_FLASH_DISABLE)
		{
			ui_drv_func.flash_disable(ui_icon);
		}
		if(rds_ctrl & RDS_ICON_CTRL_OPEN)
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, ui_icon);
		}
		if(rds_ctrl & RDS_ICON_CTRL_CLOSE)
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, ui_icon);
		}
	}
}

void ui_show_rds_txt(uint8 txt)
{
enum{
	RDS_TXT_NULL = 0,
	RDS_TXT_FLASH,
	RDS_TXT_TRAFFIC,
	RDS_TXT_EON_TA,
	RDS_TXT_TA_SEEK,
	RDS_TXT_PI_SEEK,
	RDS_TXT_PTY_SEEK,
	RDS_TXT_FOUND,
	RDS_TXT_NO_FOUND,
	RDS_TXT_NO_TP_TA,
	RDS_TXT_MAX,
};

	if(txt < RDS_TXT_MAX)
	{
		if(txt == RDS_TXT_NULL)
		{
			ui_disp.front_level = DISP_LEVEL_NORMAL;
		}
		else
		{
			ui_lang_id_e disp_id = LANG_ID_MAX;

			switch(txt)
			{
				case RDS_TXT_FLASH:		disp_id = LANG_ID_NULL;			break;
				case RDS_TXT_TRAFFIC:	disp_id = LANG_ID_RDS_TRAFFIC;	break;
				case RDS_TXT_EON_TA:	disp_id = LANG_ID_RDS_EON_TA;	break;
				case RDS_TXT_TA_SEEK:	disp_id = LANG_ID_RDS_TA_SEEK;	break;
				case RDS_TXT_PI_SEEK:	disp_id = LANG_ID_RDS_PI_SEEK;	break;
				case RDS_TXT_PTY_SEEK:	disp_id = LANG_ID_RDS_PTY_SEEK;	break;
				case RDS_TXT_FOUND:		disp_id = LANG_ID_RDS_FOUND;	break;
				case RDS_TXT_NO_FOUND:	disp_id = LANG_ID_RDS_NO_FOUND;	break;
				case RDS_TXT_NO_TP_TA:	disp_id = LANG_ID_RDS_NO_TP_TA;	break;
			}

			if(disp_id != LANG_ID_MAX)
			{
			#if defined(_FY7000_SPECIAL_DISPLAY_)
				uint8 dispchar[13];
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
				uint8 dispchar[11];
			#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
				uint8 dispchar[10];
			#elif defined(_SL6812A_SPECIAL_DISPLAY_)
				uint8 dispchar[10];
			#else
				uint8 dispchar[8];
			#endif

				if(ui_lang_res_search(disp_id, dispchar, sizeof(dispchar)))
				{
					show_clear_num(DISP_LEVEL_BACK);
					show_clear_icon(DISP_LEVEL_BACK);
					ui_show_string(DISP_LEVEL_BACK, dispchar);
					ui_disp.front_level = DISP_LEVEL_BACK;
				}
			}
		}
	}
}
#endif

#if defined(_SUPPORT_DISC_)
void ui_show_disc_icon(uint8 on_off)
{
	if(on_off)
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R1);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R2);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R3);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_R4);

		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_DISC);
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
#endif

void ui_show_usb_icon(uint8 on_off)
{
	disp_func_reset();
	if(on_off)
	{
		ew_puts("----UI_ICON_USB_ON\n");
    #if defined(_ENABLE_IPHONE_CHARGE_)
    	if(global.charge_flg)
    	{
    		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
    	}
    #else
    	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
    #endif
	}
	else
	{
		ew_puts("----UI_ICON_USB_OFF\n");
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_USB);
	#if !defined(_DISABLE_USB_FLASH_)
		ui_drv_func.flash_disable(UI_ICON_USB);
	#endif
	#if defined(_CHANGE_MUTE_DISP_) 
	    if(global.aud_mute)
		{
		    ui_show_setting(LANG_ID_OTHERS_MUTE_ON,0);
		}
	#endif	
	}
}

void ui_show_sd_icon(uint8 on_off)
{
	disp_func_reset();
	if(on_off)
	{
		ew_puts("----UI_ICON_SD_ON\n");
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SD);
	}
	else
	{
		ew_puts("----UI_ICON_SD_OFF\n");
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SD);
		ui_drv_func.flash_disable(UI_ICON_SD);
	#if defined(_CHANGE_MUTE_DISP_) 
		if(global.aud_mute)
		{
		    ui_show_setting(LANG_ID_OTHERS_MUTE_ON,0);
		}
	#endif	
	}
}

void ui_show_bt_icon(uint8 on_off)
{
	disp_func_reset();
	if(on_off)
	{
    #if defined(BT_ICON_BLINK)
    	ui_drv_func.flash_disable(UI_ICON_BLUETOOTH);
    #endif
        ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
    #if !defined(_ENABLE_TL_SPECIAL_DISP_)
	#if !defined(_WLS1403_SPECIAL_DISP_)
	#if !defined(_SL6812A_BT_DISPLAY_)
    	ui_show_setting(LANG_ID_BT_ON, 0);
    #endif
	#endif
	#endif
	}
	else
	{
    #if defined(BT_ICON_BLINK)
    	ui_drv_func.flash_enable(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
    #else
    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
    #endif
	#if !defined(_ENABLE_TL_SPECIAL_DISP_)
	#if defined(_DISABLE_ACC_OFF_DISPLAY_)
		if(global.bt_off)
		{
			global.bt_off = FALSE;
			ui_show_setting(LANG_ID_NULL, 0);
		}
		else
		{
			ui_show_setting(LANG_ID_BT_OFF, 0);
		}
	#elif defined(_ACC_OFF_DISP_LOGO_)
    	if(!global.bt_off)
    	{
    		ui_show_setting(LANG_ID_BT_OFF, 0);
    	}
	#elif defined(_ACC_OFF_POWER_LOGO_)
		if(!global.bt_off)
		{
			ui_show_setting(LANG_ID_BT_OFF, 0);
		}
	#else
	#if !defined(_WLS1403_SPECIAL_DISP_)
	#if !defined(_SL6812A_BT_DISPLAY_)
		ui_show_setting(LANG_ID_BT_OFF, 0);
	#endif
	#endif
	#endif
	#endif
	}
}

void ui_show_enable(uint8 enable)
{
	if(enable)
	{
		ui_drv_func.open();
	}
	else
	{
		ui_drv_func.close();
	}
}

#if defined(_CLOSE_EQ_ICON_)
void close_eq_icon_polling(void)
{
	if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard"))||(compare_task_name("BtUserTask")))
	{
		show_eq_icon(setup.aud_eq);
	}
	else
	{
		show_eq_icon(AUDIO_EQ_MAX);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EQ);
	}
}
#endif

#if defined(TD_ADD_LCD_NUM_4)
extern void radio_version_show1(void);
#endif

#if defined(_SL6812_TEST_LCD_)
extern void rds_ctrl_icon_sync(void);
extern u8 get_bt_connect_status(void);

static uint8 lcd_bt_state(void)
{
	switch(get_bt_connect_status())
	{
		case BT_STATUS_CONNECTING:
		case BT_STATUS_TAKEING_PHONE:
		case BT_STATUS_PLAYING_MUSIC:
			return 1;
	}
	return 0;
}

static uint8 lcd_usb_state(void)
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

static uint8 lcd_card_state(void)
{
#if defined(_SUPPORT_SD_)
	if(file_operate_get_total_phydev())
	{
		if(dev_get_online_status(sd1))
		{
			return TRUE;
		}
	}
#endif
	return FALSE;
}

void clean_lcd_icon(void)
{
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_TP);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_AF);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_TA);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PTY);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_EON);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_LOUD);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_INT);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RPT);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RDM);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_STO);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RADIO);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_AUX);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_USB);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_SD);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_LCD);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MHZ);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_KHZ);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MP3);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_WMA);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MUTE);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_TRACK);//无用
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_DISC);//无用
	
	global.lcd_mode_flag = TRUE;
	//rds_ctrl_icon_sync();
	if(lcd_bt_state())
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_BLUETOOTH);
	}
	if(compare_task_name("FMRadioTask"))
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RADIO);
		if(lcd_card_state())
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SD);
		}
		if(lcd_usb_state())
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
		}
	}
	else if(compare_task_name("LineinTask"))
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_AUX);
		if(lcd_card_state())
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SD);
		}
		if(lcd_usb_state())
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
		}
	}
	else if(compare_task_name("BtUserTask"))
	{
		if(lcd_card_state())
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SD);
		}
		if(lcd_usb_state())
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
		}
	}
	else if(compare_task_name("MusicTaskUsb"))
	{
		if(lcd_card_state())
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_SD);
		}
	}
	else if(compare_task_name("MusicTaskCard"))
	{
		if(lcd_usb_state())
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_USB);
		}
	}
	
	if(setup.aud_loud == AUDIO_LOUDNESS_ON)
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_LOUD);
	}

}
#endif

void ui_show_polling(void)
{
#if defined(_ENABLE_TIME_DISP_)
	if(setup.time_disp)
	{
		if(global.timer_count > DISP_POLLING_TIME)
		{
			global.timer_count -= DISP_POLLING_TIME;
		}
		else
		{
			global.timer_count = 0;
		}
	}
#endif

#if defined(_CLOSE_EQ_ICON_)
	if(global.sys_power)
	{
		close_eq_icon_polling();
	}
#endif

#if defined(SUPPORT_ALARM_FUNC_)	
#if defined(_ENABLE_TIME_DISP_)
	if(setup.time_disp)
	{
		if((ui_disp.now_level == UI_LEVEL_ALL)&&(global.time_flag))
		{
			if(ui_disp.func_sel == FUNC_SEL_TIME)
			{
				rtc_disp();
			}
		}
	}
	else
	{
		if((ui_disp.now_level == DISP_LEVEL_SETTING)&&(global.time_flag))
		{
			if(ui_disp.func_sel == FUNC_SEL_TIME)
			{
				rtc_disp();
			}
		}
	}
#endif

	if((ui_disp.now_level == DISP_LEVEL_SETTING)&&(global.alarm_flag))
	{
		if(ui_disp.func_sel == FUNC_SEL_TIME)
		{
			rtc_disp1();
		}
	}
#else

#if !defined(_ENABLE_REALTIME_DIAPLAY_)
#if !defined(_SUPPORT_TIME_DISPLAY_)
#if !defined(_TEMP_WARM_DISPLAY_)
	if(ui_disp.now_level == DISP_LEVEL_SETTING)
	{
		if(ui_disp.func_sel == FUNC_SEL_TIME)
		{
			rtc_disp();
		}
	}
#endif
#endif
#endif
#endif

#if defined(_USER_TFT_SEND_)&&1
	if(ui_disp.prev_level != ui_disp.now_level)
	{
		ui_drv_func.level(ui_disp.now_level);
	}
	else
	{
		if(ui_disp.sel_timer != -1)
		{
			if(ui_disp.sel_timer > DISP_POLLING_TIME)
			{
				ui_disp.sel_timer -= DISP_POLLING_TIME;
			}
			else
			{
				ui_disp.sel_timer = 0;
				if(ui_disp.now_level != ui_disp.front_level)
				{
					ui_disp.now_level = ui_disp.front_level;
					ui_drv_func.level(ui_disp.now_level);
					ui_disp.func_type = FUNC_TYPE_SEL;
					ui_disp.func_sel  = FUNC_SEL_NULL;
					OSTimeDly(10);
					TFT_Tx_Sel(ui_disp.func_sel);
					OSTimeDly(10);
				}
			}
		}
	}
#endif

	if(ui_disp.prev_level != ui_disp.now_level)
	{
		ui_drv_func.level(ui_disp.now_level);
	}
	else
	{
		if(ui_disp.timer != -1)
		{
			if(ui_disp.timer > DISP_POLLING_TIME)
			{
				ui_disp.timer -= DISP_POLLING_TIME;
			}
			else
			{
		    #if defined(_ENABLE_CLOCK_DISPLAY_)
		    	if(!ui_disp.playflg)
		    	{
		    		ui_disp.playflg = FALSE;
		    		if(!ui_disp.talkflg)
		    		{
		    			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
		    		}
		    	}
		    #endif
            #if defined(_LCD_3_DIAPLAY_)
			    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
			#endif
			
		    #if defined(SUPPORT_ALARM_FUNC_)
		    	global.time_flag = FALSE;
		    	global.alarm_flag = FALSE;
		    #endif
			
			#if defined(_KLD3300_VERSION_POINT_)
				if(global.view_flag)
				{
					ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);
					global.view_flag = FALSE;
				}
			#endif
			
			#if defined(_RDS_KEY_FUNTION_)
				global.reg_flag = FALSE;
				global.eon_flag = FALSE;
			#endif
			
			#if defined(_SPECIAL_MENU_FUNTION_)
				global.set_flag = FALSE;
			#endif

			#if defined(_ENABLE_TIME_DISP_)
				if(global.work_flag)
				{
					global.work_flag1 = TRUE;
					global.work_flag = FALSE;
				}
			#endif
				
		    #if defined(_ENABLE_PASSWORD_INPUT_)
    			//setup.password_input = 0;
    			//setup.password_number = 0;
				//setup.password = FALSE;
		    #endif
			
		    #if defined(TD_ADD_LCD_NUM_4)
		    	if(global.version)
		    	{
		    		radio_version_show1();
		    		global.version = FALSE;
		    		global.version1 = TRUE;
		    	}
		    #endif
			
			#if defined(_ENABLE_MENU_SET_)
				global.menu_flag = FALSE;
			#endif
			
			#if defined(_FEIYIN_RDS_SET_)
				global.sel_rds = FALSE;
			#endif
			
		    #if defined(_DISABLE_SEL_VOLUME_)
		    	global.sel_flag = FALSE;
		    #endif
			
		    #if defined(_ENABLE_SEL_FUNTION_)
		    	global.sel_mode = FALSE;
		    #endif
			
				ui_disp.timer = 0;
				if(ui_disp.now_level != ui_disp.front_level)
				{
			    #if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
			    	ew_puts("password_input999\n");
				//#if !defined(_PASSWORD_INPUT_BLINK_)
			    	password_disp_polling();
				//#endif
				#if defined(_SUPPORT_PASSWORD_DISPLAY_)
					if(global.err_flag)
					{
						global.err_flag = FALSE;
						setup.first_power_flag = TRUE;
					}
			    #endif
				#endif

				#if defined(_SL6812_TEST_LCD_)
					if(global.lcd_flag)
					{
						puts("_SL6812_TEST_LCD_\n");
						global.lcd_flag = FALSE;
						clean_lcd_icon();
					}
				#endif
					
				#if defined(_ENABLE_PASSWORD_SET_)
					if(global.pwd_set_ok)
					{
						global.pwd_set_ok = FALSE;
						os_taskq_post_msg("IdleTask", 1, MSG_IDLE_EXIT);
					}
				#endif
				
				#if defined(_PASSWORD_TRUE_DISPLAY_)
    				if(global.pwd_set_true)
    				{
    					global.pwd_set_true = FALSE;
    					os_taskq_post_msg("IdleTask", 1, MSG_IDLE_EXIT);
    				}
				#endif
				
					ui_disp.now_level = ui_disp.front_level;
					ui_drv_func.level(ui_disp.now_level);
					ui_disp.func_type = FUNC_TYPE_SEL;
					ui_disp.func_sel  = FUNC_SEL_NULL;
				}
			}
		}
		
	#if defined(_FY5517_TIME_DISPLAY_)
		if(ui_disp.timer1 != -1)
		{
			if(ui_disp.timer1 > DISP_POLLING_TIME)
			{
				ui_disp.timer1 -= DISP_POLLING_TIME;
			}
			else
			{
				global.time_set = FALSE;
				ui_disp.timer1 = 0;
			}
		}
	#endif
	}
	
	ui_disp.prev_level = ui_disp.now_level;
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);

/*-----------------------------------------------------------------------------------------------------------------------*/
//	idle display
#include "ui_ap\ui_idle\ui_idle.c"

/*-----------------------------------------------------------------------------------------------------------------------*/
//	radio display
#include "ui_ap\ui_radio\ui_radio.c"

/*-----------------------------------------------------------------------------------------------------------------------*/
//	play display
#include "ui_ap\ui_play\ui_play.c"

/*-----------------------------------------------------------------------------------------------------------------------*/
//	play disc
#if defined(_SUPPORT_DISC_)
#include "ui_ap\ui_disc\ui_disc.c"
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/
//	play display
#include "ui_ap\ui_bt\ui_bt.c"

/*-----------------------------------------------------------------------------------------------------------------------*/
//	aux display
void ui_show_aux_init(void)
{
	ui_puts("ui_show_aux_init\n");
	disp_func_reset();
#if defined(_ENABLE_MODE_CHANNEL_)
	if(setup.mode_start == 0)
	{
		ui_show_setting(LANG_ID_AUX_IN, 0);
		ui_show_normal(LANG_ID_AUX_IN);
	}
	else if(setup.mode_start == 1)
	{
		ui_show_setting(LANG_ID_OPTICAL, 0);
		ui_show_normal(LANG_ID_OPTICAL);
	}
	else if(setup.mode_start == 2)
	{
		ui_show_setting(LANG_ID_TV, 0);
		ui_show_normal(LANG_ID_TV);
	}
	else if(setup.mode_start == 3)
	{
		ui_show_setting(LANG_ID_DVD, 0);
		ui_show_normal(LANG_ID_DVD);
	}
#else
#if !defined(_SUPPORT_LCM_1618_DRIVE)
	ui_show_setting(LANG_ID_AUX_IN, 0);
#endif
	ui_show_normal(LANG_ID_AUX_IN);
#endif
	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_AUX);

#if defined(_FY7555_LCD_DISPLAY_)
	ui_show_setting1(LANG_ID_AUX_IN, 0);
	ui_show_normal1(LANG_ID_AUX_IN);
#endif
}

void ui_show_aux_exit(void)
{
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_AUX);
}


void ui_show_aux_polling(void)
{  
#if defined(_ENABLE_MIC_REC_)
    if(rec_status == rec_start)
    {
         global.mic_record = TRUE;
  	     rec_time_disp(DISP_LEVEL_NORMAL,rec_time_count);
   	     if(ui_disp.timer == 0)
    	 {
    		 ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
    		 ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);
    	 }
    }
    else if(rec_status == rec_pp)
    {
    	 ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
    	 ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);	  
    	 ui_show_normal(LANG_ID_PAUSE);
    }
    else if(rec_status == rec_stop)
    {
        if(global.mic_record)
     	{
        	global.mic_record = FALSE;
        	ui_show_normal(LANG_ID_AUX_IN);
     	}
  	    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);	  
    }
#endif
}

#if defined(_ENABLE_AUX_VERSION_)||defined(_ENABLE_MODE_VERSION_)
static uint8 version_mouth_to_number1[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static uint8 version_info_date1[] = __DATE__;
static uint8 version_info_time1[] = __TIME__;

void ui_show_aux_version(void)
{
#if defined(_KLD3300_VERSION_VIEW_)
	uint8 dispchar[8] = {_KLD3300_VERSION_STR_};
#else
	uint8 dispchar[8];
	uint8 index;

	memset(dispchar, ' ', sizeof(dispchar));

	for(index = 0; index < 12; index++)
	{
		if(!memcmp(version_info_date1, version_mouth_to_number1[index], 3))
		{
			break;
		}
	}

	if(index < 12)
	{
		index++;
	}
	else
	{
		index = 0;
	}
	dispchar[0] = index/10 + '0';
	dispchar[1] = index%10 + '0';
	dispchar[2] = (version_info_date1[4]== ' ')? '0' : version_info_date1[4];
	dispchar[3] = (version_info_date1[5]== ' ')? '0' : version_info_date1[5];
	dispchar[4] = version_info_time1[0];
	dispchar[5] = version_info_time1[1];
	dispchar[6] = version_info_time1[3];
	dispchar[7] = version_info_time1[4];
#endif

#if defined(_KLD3300_VERSION_POINT_)
	global.view_flag = TRUE;
	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POINT);
#else
	show_clear_icon(DISP_LEVEL_SETTING);
#endif

	ui_show_string(DISP_LEVEL_SETTING, dispchar);
	ui_disp.now_level = DISP_LEVEL_SETTING;
	ui_disp.timer = 3000;
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_AUX1_)
void ui_show_aux1_init(void)
{
	ew_puts("ui_show_aux1_init\n");
	ui_show_setting(LANG_ID_AUX1_IN, 0);
	ui_show_normal(LANG_ID_AUX1_IN);
}
void ui_show_aux1_exit(void)
{
	//ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_AUX);
}
void ui_show_aux1_polling(void)
{

}
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_MIC_OK_)
void ui_show_mic_init(void)
{
	ui_puts("ui_show_mic_init\n");
	disp_func_reset();
	ui_show_setting(LANG_ID_MIC, 0);
	ui_show_normal(LANG_ID_MIC);
}

void ui_show_mic_exit(void)
{

}

void ui_show_mic_polling(void)
{

}
#endif
/*-----------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_CAN_MODEL_)
void ui_show_can_init(void)
{
	ui_puts("ui_show_can_init\n");
	disp_func_reset();
	ui_show_setting(LANG_ID_PASSWORD, 0);
	ui_show_normal(LANG_ID_PASSWORD);
}

void ui_show_can_exit(void)
{

}

void ui_show_can_polling(void)
{

}
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/

#if defined(_SUPPORT_REVERSE_MODEL_)
void ui_show_reverse_init(void)
{
	ui_puts("ui_show_reverse_init\n");
	disp_func_reset();
	ui_show_setting(LANG_ID_REVE, 0);
	ui_show_normal(LANG_ID_REVE);
}

void ui_show_reverse_exit(void)
{

}

void ui_show_reverse_polling(void)
{

}
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/
//	dab display
#if defined(DAB_MODLE)
extern void dab_info_play(void);

void ui_show_dab_init(void)
{
	ui_puts("ui_show_dab_init\n");
	disp_func_reset();
#if !defined(_SUPPORT_LCM_1618_DRIVE)
	ui_show_setting(LANG_ID_DAB, 0);
#endif
	ui_show_normal(LANG_ID_DAB);
}
void ui_show_dab_exit(void)
{
	ui_puts("ui_show_dab_exit\n");

	show_clear_icon(DISP_LEVEL_NORMAL);
	show_clear_num(DISP_LEVEL_NORMAL);
}
void ui_show_dab_polling(void)
{
	dab_info_play();
}
#endif

#if defined(EASYESY_APP_ONOFF)
void ui_APP_ONOFF(u8 onoff)
{
	ui_puts("ui_APP_ONOFF\n");
	if(global.sys_power)
	{
		if(onoff)
		{
			ui_show_setting(LANG_ID_APP_ON, 0);
		}
		else
		{
			ui_show_setting(LANG_ID_APP_OFF, 0);
		}
		ui_disp.timer = 1500;
	}
	//ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_AUX);
}				
#endif

//
#if REC_EN
void ui_show_rec_init(void)
{
	ui_puts("ui_show_rec_init\n");
	disp_func_reset();
	ui_show_setting(LANG_ID_RECORD, 0);
	ui_disp.timer = 1500;
	ui_show_normal(LANG_ID_RECORD);
}
void ui_show_rec_exit(void)
{
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_AUX);
}

extern uint16 rec_time_count,rec_status;

void rec_time_disp(uint8 level,uint16 rec_time)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#else
    uint8 disp_buf[8];
#endif

#if defined(_ENABLE_RECORD_RADIO_)
	uint8 min = (rec_time/2)/60;
	uint8 sec = (rec_time/2)%60;
#else
	uint8 min = rec_time/60;
	uint8 sec = rec_time%60;
#endif

#if defined(TD_ADD_LCD_NUM_4)
    disp_buf[0] = min/10 + '0';
    disp_buf[1] = min%10 + '0';
    disp_buf[2] = sec/10 + '0';
    disp_buf[3] = sec%10 + '0';
#elif defined(_FIVE_CHAR_DISPLAY_)
	disp_buf[0] =  'R';
	disp_buf[1] =  'E';
	disp_buf[2] =  'C';
    disp_buf[3] = min%10 + '0';
    disp_buf[4] = sec/10 + '0';
    disp_buf[5] = sec%10 + '0';
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    disp_buf[0] =  ' ';
    disp_buf[1] =  'R';
    disp_buf[2] =  'E';
    disp_buf[3] =  'C';
    disp_buf[4] =  'O';
    disp_buf[5] =  'R';
    disp_buf[6] =  'D';
    disp_buf[7] =  ' ';
    disp_buf[8] =  ' ';
    disp_buf[9] = min/10 + '0';
    disp_buf[10] = min%10 + '0';
    disp_buf[11] = sec/10 + '0';
    disp_buf[12] = sec%10 + '0';
#else
    disp_buf[0] =  'R';
    disp_buf[1] =  'E';
    disp_buf[2] =  'C';
    disp_buf[3] =  ' ';
    disp_buf[4] = min/10 + '0';
    disp_buf[5] = min%10 + '0';
    disp_buf[6] = sec/10 + '0';
    disp_buf[7] = sec%10 + '0';
#endif
#if defined(_ENABLE_RECORD_RADIO_)||defined(_ENABLE_MIC_REC_)
	show_clear_num(DISP_LEVEL_NORMAL);
#endif
	ui_show_string(level, disp_buf);
}
void rec_num_disp(uint8 level,uint16 NUM)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 disp_buf[13];
#else
    uint8 disp_buf[8];
#endif
	uint8 aa,bb,cc,dd,ee,ff;
	
	memset(disp_buf, ' ', sizeof(disp_buf));
	
	aa = NUM/1000;
	bb = NUM%1000;
	cc = bb/100;
	dd = bb%100;
	ee = dd/10;
	ff = dd%10;

#if defined(TD_ADD_LCD_NUM_4)
    disp_buf[0] = aa + '0';
    disp_buf[1] = cc + '0';
    disp_buf[2] = ee + '0';
    disp_buf[3] = ff + '0';
#elif defined(_FIVE_CHAR_DISPLAY_)
    disp_buf[0] =  'R';
    disp_buf[1] =  'E';
    disp_buf[2] =  'C';
    disp_buf[3] = cc + '0';
    disp_buf[4] = ee + '0';
    disp_buf[5] = ff + '0';
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    disp_buf[0] =  ' ';
    disp_buf[1] =  'R';
    disp_buf[2] =  'E';
    disp_buf[3] =  'C';
    disp_buf[4] =  'O';
    disp_buf[5] =  'R';
    disp_buf[6] =  'D';
    disp_buf[7] =  ' ';
    disp_buf[8] =  ' ';
    disp_buf[9] = aa/10 + '0';
    disp_buf[10] = cc%10 + '0';
    disp_buf[11] = ee/10 + '0';
    disp_buf[12] = ff%10 + '0';
#else
    disp_buf[0] =  'R';
    disp_buf[1] =  'E';
    disp_buf[2] =  'C';
    disp_buf[3] =  ' ';
    disp_buf[4] = aa + '0';
    disp_buf[5] = cc + '0';
    disp_buf[6] = ee + '0';
    disp_buf[7] = ff + '0';
#endif
	ui_show_string(level, disp_buf);
}

extern u32 rec_fname_cnt;       //录音文件名计数

void ui_show_rec_polling(void)
{
	if(rec_status == rec_start)
    {
		rec_time_disp(DISP_LEVEL_NORMAL,rec_time_count);
	#if defined(TD_ADD_LCD_NUM_4)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PAUSE);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PLAY);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
	#else
		if(ui_disp.timer == 0)
		{
		#if defined(_FIVE_CHAR_DISPLAY_)
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_DASH);
		#elif defined(_FY7000_SPECIAL_DISPLAY_)
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
		#else
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POINT);
		#endif
		}
	#endif
	}
	else if(rec_status == rec_pp)
	{
	#if defined(TD_ADD_LCD_NUM_4)
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_PAUSE);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAY);
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
	#else
	#if defined(_FIVE_CHAR_DISPLAY_)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_DASH);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
	#elif defined(_FY7000_SPECIAL_DISPLAY_)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
	#else
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);
	#endif
		ui_show_normal(LANG_ID_PAUSE);
	#endif
	}
	else if(rec_status == rec_stop)
	{
		ui_show_normal(LANG_ID_RECORD);
	#if defined(TD_ADD_LCD_NUM_4)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PAUSE);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PLAY);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
	#else
	#if defined(_FIVE_CHAR_DISPLAY_)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_DASH);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
	#elif defined(_FY7000_SPECIAL_DISPLAY_)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
	#else
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);	
    #endif
	#endif
	}
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_TAPE_)
void ui_show_tape_init(void)
{
	ui_puts("ui_show_tape_init\n");
	disp_func_reset();
	//ui_show_setting(LANG_ID_TAPE, 0);
	//ui_show_normal(LANG_ID_TAPE);
}

void ui_show_tape_exit(void)
{
	ui_puts("ui_show_tape_exit\n");

	show_clear_icon(DISP_LEVEL_NORMAL);
	show_clear_num(DISP_LEVEL_NORMAL);
}

void ui_show_tape_polling(void)
{
#if defined(_SUPPORT_RECORD_)
	if(rec_status == rec_start)
	{
		rec_time_disp(DISP_LEVEL_NORMAL,rec_time_count);
		if(ui_disp.timer == 0)
		{
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POINT);
		}
		global.tape_a_time = 0;
		global.tape_b_time = 0;
	}
	else if(rec_status == rec_pp)
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);	
		ui_show_normal(LANG_ID_PAUSE);
	}
	else if(rec_status == rec_stop)
	{
		if(TAPE_AB_CHECK())
		{
			global.tape_b_time = 0;
			if(++global.tape_a_time <= (500/DISP_POLLING_TIME))
			{
				ui_show_normal(LANG_ID_TAPE_A);
			}
			else if((++global.tape_a_time <= (1000/DISP_POLLING_TIME))&&(++global.tape_a_time > (500/DISP_POLLING_TIME)))
			{
				ui_show_normal(LANG_ID_TAPE);
			}
			else
			{
				global.tape_a_time = 0;
			}
		}
		else
		{
			global.tape_a_time = 0;
			if(++global.tape_b_time <= (500/DISP_POLLING_TIME))
			{
				ui_show_normal(LANG_ID_TAPE_B);
			}
			else if((++global.tape_b_time <= (1000/DISP_POLLING_TIME))&&(++global.tape_b_time > (500/DISP_POLLING_TIME)))
			{
				ui_show_normal(LANG_ID_TAPE1);
			}
			else
			{
				global.tape_b_time = 0;
			}
		}
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);	
	}
#endif
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

#ifdef	TD_ADD_DISP_CHECKSUM
void ui_show_checksum(void)
{
    uint8 buf[10];
	uint8 dispchar[8];
    memcpy((u8*)buf,__DATE__,10);
    if(memcmp((u8*)buf,"Jan",3)==0)
    {//1月
	    dispchar[0]='0';
	    dispchar[1]='1';
    }
    else if(memcmp((u8*)buf,"Feb",3)==0)
    {//2月
	    dispchar[0]='0';
	    dispchar[1]='2';
    }
    else if(memcmp((u8*)buf,"Mar",3)==0)
    {//3月
	    dispchar[0]='0';
	    dispchar[1]='3';
    }
    else if(memcmp((u8*)buf,"Apr",3)==0)
    {//4月
	    dispchar[0]='0';
	    dispchar[1]='4';
    }
    else if(memcmp((u8*)buf,"May",3)==0)
    {//5月
	    dispchar[0]='0';
	    dispchar[1]='5';
    }
    else if(memcmp((u8*)buf,"Jun",3)==0)
    {//6月
	    dispchar[0]='0';
	    dispchar[1]='6';
    }
    else if(memcmp((u8*)buf,"Jul",3)==0)
    {//7月
	    dispchar[0]='0';
	    dispchar[1]='7';
    }
    else if(memcmp((u8*)buf,"Aug",3)==0)
    {//8月
	    dispchar[0]='0';
	    dispchar[1]='8';
    }
    else if(memcmp((u8*)buf,"Sep",3)==0)
    {//9月
	    dispchar[0]='0';
	    dispchar[1]='9';
    }
    else if(memcmp((u8*)buf,"Oct",3)==0)
    {//10月
	    dispchar[0]='1';
	    dispchar[1]='0';
    }
    else if(memcmp((u8*)buf,"Nov",3)==0)
    {//11月
	    dispchar[0]='1';
	    dispchar[1]='1';
    }
    else if(memcmp((u8*)buf,"Dec",3)==0)
    {//12月
	    dispchar[0]='1';
	    dispchar[1]='2';
    }
    dispchar[2]=buf[4];
    dispchar[3]=buf[5];
    memcpy((u8*)buf,__TIME__,10);
    dispchar[0]=buf[0];
    dispchar[1]=buf[1];
    dispchar[2]=buf[3];
    dispchar[3]=buf[4];
    show_clear_num(DISP_LEVEL_SETTING);
    show_clear_icon(DISP_LEVEL_SETTING);
    ui_show_string(DISP_LEVEL_SETTING, dispchar);
    ui_disp.now_level = DISP_LEVEL_SETTING;
    ui_disp.timer = 5000;
}
#endif

