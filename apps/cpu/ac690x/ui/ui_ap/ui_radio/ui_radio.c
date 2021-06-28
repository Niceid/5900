#include "sdk_cfg.h"
#include "ui_radio/ui_radio.h"
#include "rds/rds_ctrl.h"
#include "fm_band.h"

extern void clear_wdt(void);//

#if defined(_ENABLE_RECORD_RADIO_)
extern uint16 rec_time_count,rec_status;
extern void rec_time_disp(uint8 level,uint16 rec_time);
#include "setup.h"
#endif

static radio_disp_t *radio_disp = NULL;
#if defined(SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
void FM_BAND_DISP(u8 ONOFF)
{
	u8 BAND_NUM=0;
	if(ONOFF==ON)
	{
		if(GET_RADIO_BAND <=2)
		{
			BAND_NUM =GET_RADIO_BAND+1;
		}
		else
		{
			BAND_NUM =(GET_RADIO_BAND-3)+1;
		}
		if(BAND_NUM ==1)
		{
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_RSSI1);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI2);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI3);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI4);
		}
		else if(BAND_NUM ==2)
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI1);
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_RSSI2);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI3);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI4);

		}
		else if(BAND_NUM ==3)
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI1);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI2);
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_RSSI3);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI4);

		}
		else if(BAND_NUM ==4)
		{
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI1);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI2);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI3);
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_RSSI4);
		}
	}
	else if(ONOFF==OFF)
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI1);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI2);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI3);
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI4);
	}
}
#endif

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
static void ui_show_radio_band(uint8 fm_band)
{
	uint16 disp_buf[4];
	uint8  disp_ctrl[3];

	disp_buf[0] = UI_ICON_MHZ;		disp_ctrl[0] = (fm_band ? 1 : 0);
	disp_buf[1] = UI_ICON_KHZ;		disp_ctrl[1] = (fm_band ? 0 : 1);
#if defined(_SPECIAL_LCD_4_DIAPLAY_)
	if((!disp_realtime_check())&&(ui_disp.now_level != DISP_LEVEL_SETTING))
	{
		disp_buf[2] = UI_ICON_FM;		disp_ctrl[2] = (fm_band ? 1 : 0);
	}
#else
	disp_buf[2] = UI_ICON_FM;		disp_ctrl[2] = (fm_band ? 1 : 0);
#endif
	disp_buf[3] = UI_ICON_NULL;
	ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
}

static void ui_show_radio_icon(uint8 st, uint8 sto, uint8 loc)
{
	uint16 disp_buf[4];
	uint8  disp_ctrl[3];

	disp_buf[0] = UI_ICON_ST;		disp_ctrl[0] = st;
	disp_buf[1] = UI_ICON_STO;		disp_ctrl[1] = sto;
	disp_buf[2] = UI_ICON_LOC;		disp_ctrl[2] = loc;
	disp_buf[3] = UI_ICON_NULL;

	ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
}

static void ui_show_radio_psn(const uint8 *psn)
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

	show_clear_icon(DISP_LEVEL_NORMAL);
	memcpy(dispchar, psn, sizeof(dispchar));
#if defined(_FIVE_CHAR_DISPLAY_)
	dispchar[5] = ' ';
    dispchar[6] = ' ';
	dispchar[7] = ' ';
#endif
#if defined(_DISABLE_CLOSE_POINT_)
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
#endif
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
}

#if defined(TD_ADD_BANDCH)
#if defined(_SPECIAL_WK_FM_DISP_)
void ui_show_bandch(void)
{
	uint8 dispchar[6];
    if(GET_BAND_FREQ_INDEX ==0)
    {
        dispchar[2] = ' ';
        if(GET_RADIO_BAND <=2)
        {
            dispchar[3] = 'F';
            dispchar[4] =GET_RADIO_BAND+'1';
        }
        else
        {
            dispchar[3] = 'A';
            dispchar[4] =(GET_RADIO_BAND-3)+'1';
        }
    	dispchar[5] = ' ';
    }
    else
    {
	    	dispchar[0] = ' ';
	    	dispchar[1] = ' ';
	    if(GET_RADIO_BAND <=2)
	    {
	    	dispchar[2] = 'F';
	        dispchar[3] =GET_RADIO_BAND+'1';
	    }
	    else
	    {
	    	dispchar[2] = 'A';
	        dispchar[3] =(GET_RADIO_BAND-3)+'1';
	    }
	    dispchar[4] ='P';
	    dispchar[5] =GET_BAND_FREQ_INDEX+'0';
    }
	dispchar[6] =' ';
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);
}

#elif defined(_LCD_3_DIAPLAY_)
void ui_show_bandch(void)
{
	uint8 dispchar[4];
	if(GET_BAND_FREQ_INDEX ==0)
	{
		dispchar[0] = ' ';
		if(GET_RADIO_BAND <=2)
		{
			dispchar[1] = 'F';
			dispchar[2] =GET_RADIO_BAND+'1';
		}
		else
		{
			dispchar[1] = 'A';
			dispchar[2] =(GET_RADIO_BAND-3)+'1';
		}
		dispchar[3] = ' ';
	}
	else
	{
		if(GET_RADIO_BAND <=2)
		{
			dispchar[1] =GET_RADIO_BAND+'1';
		}
		else
		{
			dispchar[1] =(GET_RADIO_BAND-3)+'1';
		}
		dispchar[2] ='P';
		dispchar[3] =GET_BAND_FREQ_INDEX+'0';
	}
#if defined(SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
	FM_BAND_DISP(OFF);
#endif
	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
}
#else
void ui_show_bandch(void)
{
#if defined(_ZSBY5209_CH_DISPLAY_)
	uint8 dispchar[7];

	dispchar[0] = ' ';
	dispchar[1] = ' ';
	dispchar[2] = 'P';
	dispchar[3] =GET_BAND_FREQ_INDEX+'0';
	dispchar[4] = ' ';
	dispchar[5] = ' ';
	dispchar[6] = ' ';
	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
	
#else
	uint8 dispchar[4];
    if(GET_BAND_FREQ_INDEX ==0)
    {
        dispchar[0] = ' ';
        if(GET_RADIO_BAND <=2)
        {
            dispchar[1] = 'F';
            dispchar[2] =GET_RADIO_BAND+'1';
        }
        else
        {
            dispchar[1] = 'A';
            dispchar[2] =(GET_RADIO_BAND-3)+'1';
        }
    	dispchar[3] = ' ';
    }
    else
    {
	    if(GET_RADIO_BAND <=2)
	    {
	    	dispchar[0] = 'F';
	        dispchar[1] =GET_RADIO_BAND+'1';
	    }
	    else
	    {
	    	dispchar[0] = 'A';
	        dispchar[1] =(GET_RADIO_BAND-3)+'1';
	    }
	    dispchar[2] ='P';
	    dispchar[3] =GET_BAND_FREQ_INDEX+'0';
    }
#if defined(HuiNuo_6651D_Copy)
	dispchar[0] ='C';
	dispchar[1] ='H';
	dispchar[2] ='0';
	dispchar[3] =GET_BAND_FREQ_INDEX+'0';
#endif

#if defined(SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
	FM_BAND_DISP(OFF);
#endif
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);
#endif
}
#endif

#if defined(_JTI_POWER_LOGO_)
void ui_show_bandch1(void)
{
	uint8 dispchar[4];
    if(setup.ch_index ==0)
    {
        dispchar[0] = ' ';
        if(setup.band_index <=2)
        {
            dispchar[1] = 'F';
            dispchar[2] =setup.band_index+'1';
        }
        else
        {
            dispchar[1] = 'A';
            dispchar[2] =(setup.band_index-3)+'1';
        }
    	dispchar[3] = ' ';
    }
    else
    {
	    if(setup.band_index <=2)
	    {
	    	dispchar[0] = 'F';
	        dispchar[1] =setup.band_index+'1';
	    }
	    else
	    {
	    	dispchar[0] = 'A';
	        dispchar[1] =(setup.band_index-3)+'1';
	    }
	    dispchar[2] ='P';
	    dispchar[3] =setup.ch_index+'0';
    }
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);
    ui_disp.now_level = DISP_LEVEL_SETTING;
    ui_disp.timer = 1000;
}
#endif

#if defined(_ENABLE_CH_DISPLAY_)
#if defined(_LCD_3_DIAPLAY_)
static void ui_show_radio_band_num(uint16 freq, uint8 band, uint8 num)
{
	uint8 dispchar[4];

	dispchar[0] = ' ';
	if(GET_RADIO_BAND <=2)
	{
		dispchar[1] =GET_RADIO_BAND+'1';
	}
	else
	{
		dispchar[1] =(GET_RADIO_BAND-3)+'1';
	}
	dispchar[2] ='P';
	dispchar[3] =num+'0';
	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);	
#if defined(_LCD_3_DIAPLAY_)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
#endif
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
}
#else
static void ui_show_radio_band_num(uint16 freq, uint8 band, uint8 num)
{
    uint8 dispchar[4];
	uint8 fm_flag = (freq > 2000)?1:0;

	show_clear_icon(DISP_LEVEL_NORMAL);

	memset(dispchar, ' ', sizeof(dispchar));

	if(freq != 0xFFFF)
	{
		if(num)
		{
        	uint8 index = 0;
			if(fm_flag)
			{
				dispchar[index++] = 'F';
			}
			else
			{
				dispchar[index++] = 'A';
			}

			dispchar[index++] = band + '0';

			dispchar[index++] = 'P';

			dispchar[index++] = num+'0';
		}
		else
		{
	    	uint8 index = 1;
			if(fm_flag)
			{
				dispchar[index++] = 'F';
			}
			else
			{
				dispchar[index++] = 'A';
			}

			dispchar[index++] = band + '0';
		}
    }

#if defined(_SPECIAL_LCD_4_DIAPLAY_)
	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_FM);
#endif
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
}
#endif

#endif
#endif

#if defined(_ADD_SPECIAL_CH_DISP_)
void chang_fm_disp(void)
{
   uint8 dispchar[8];
   dispchar[0] = ' ';
   dispchar[1] = ' ';
   dispchar[2] = ' ';
   dispchar[3] = 'P';
   dispchar[4] = GET_BAND_FREQ_INDEX+'0';
   dispchar[5] = ' ';
   dispchar[6] = ' ';
   dispchar[7] = ' ';
   ui_show_string(DISP_LEVEL_NORMAL, dispchar);
   ui_show_radio_icon(0, 0, 0);
   show_clear_icon(DISP_LEVEL_NORMAL);
   show_clear_num(DISP_LEVEL_NORMAL);
}
#endif

//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
#if defined(TD_ADD_CHADD_SUB)
void disp_add_sub(void)
{
	uint8 dispchar[4];
	if(GET_RADIO_BAND <=2)
	{
		dispchar[0] = 'F';
		dispchar[1] =GET_RADIO_BAND+'1';
	}
	else
	{
		dispchar[0] = 'A';
		dispchar[1] =(GET_RADIO_BAND-3)+'1';
	}
	dispchar[2] ='P';
	dispchar[3] =global.save_radio_ch+'0';
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
#if defined(SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
	FM_BAND_DISP(OFF);
#endif
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);
}
#endif

#if defined(no_disp_87_5)
	extern u8 auto_save_freq_FLAG ;
	extern uint16 auto_save_freq ;
#endif

#if defined(_SPECIAL_RADIO_DISPLAY_)
static void ui_show_radio_freq(uint16 freq, uint8 band,uint8 num)
{
	uint8 dispchar[8];
	uint8 index = 0;
	uint8 fm_flag = (freq > 2000)?1:0;
	
	show_clear_icon(DISP_LEVEL_NORMAL);
	
	memset(dispchar, ' ', sizeof(dispchar));

	ew_printf("num:%d\n",num);
	
	if(freq != 0xFFFF)
	{		
		if(fm_flag)
		{
			dispchar[index++] = 'F';
			dispchar[index++] = 'M';
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
		}
		else
		{
    	#if defined(_SUPPORT_AREA_)
			if(setup.area == SETUP_AREA_USA)
			{
				dispchar[index++] = 'M';
				dispchar[index++] = 'W';
			}
			else
			{
				dispchar[index++] = 'A';
				dispchar[index++] = 'M';
    		}
    	#else
			dispchar[index++] = 'A';
			dispchar[index++] = 'M';
    	#endif
		}
		
		if(num < 10)
		{
			if(num == 1)
			{
				dispchar[index++] = '|';
			}
			else if(num == 0)
			{
				dispchar[index++] = ' ';
			}
			else
			{
				dispchar[index++] = num + '0';
			}
		}
		else if(num == 10)
		{
			dispchar[index++] = '0';
		}
	
		index = 7;
			
		while(freq)
		{
			dispchar[index] = (freq%10)+'0';
			freq /= 10;
			index--;
		}
		
		if(fm_flag)
		{
			if(dispchar[7] == '0')
			{
				dispchar[7] = ' ';
			}
		}
		ui_show_radio_band(fm_flag);
	}
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
}
#else
static void ui_show_radio_freq(uint16 freq, uint8 band)
{
#if defined(_ENABLE_TIME_DISP_)
	global.timer_count = 4000;
#endif

#if defined(_SUPPORT_C912DAB_)
	uint8 dispchar[8];
	memcpy(dispchar,Disp_dab_buf,8);
    if(Disp_dab_buf[8]!=0x20)
    {
    	ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    }
    else
    {
    	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    }
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);
#elif defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	uint8 dispchar[4];
	uint8 fm_flag = (freq > 2000)?1:0;
	memset(dispchar, ' ', sizeof(dispchar));

	#if defined(no_disp_87_5)
		if(auto_save_freq_FLAG ==1)
		{
			auto_save_freq_FLAG =0;
			if(freq!=auto_save_freq)
			{
				freq=auto_save_freq;
			}
		}
	#endif

    if(freq != 0xFFFF)
    {
		//printf("====dispfreq=%8d\n",freq);

	    if(freq > 2000)
	    {
			if(freq/ 10000)
			{
			    dispchar[0]      = freq/10000+'0';
			}
	        else
	        {
	        	dispchar[0]      = ' ';
			}
			dispchar[1]  = (freq%10000)/1000+'0';
			dispchar[2]  = (freq%1000)/100+'0';
			dispchar[3]  = freq%100/10+'0';
	        ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
	        ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
		#if defined(_SPECIAL_LCD_4_DIAPLAY_)||defined(_LCD_3_DIAPLAY_)
			//ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_FM);
		#endif
		#if defined(_LCD_3_DIAPLAY_)
		    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PM);
		#endif
			#if defined(SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
			FM_BAND_DISP(ON);
			#endif
		}
		else
		{
		    if(freq/1000)
		    {
		        dispchar[0]      =freq/1000+'0';
			}
			else
			{
		        dispchar[0]      =' ';
			}
		    dispchar[1]  = (freq%1000)/100+'0';
		    dispchar[2]  = freq%100/10+'0';
		    dispchar[3]  = freq%10+'0';
	        ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
	        ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
	    }
		ui_show_radio_band(fm_flag);
    }
    else if(GET_RADIO_STATE==RADIO_STATE_BROWSE)
    {//浏览时需要闪烁小数点
        ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    }
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);

#elif defined(_FY7000_SPECIAL_DISPLAY_)

    uint8 dispchar[13];
    uint8 fm_flag = (freq > 2000)?1:0;

    show_clear_icon(DISP_LEVEL_NORMAL);

    memset(dispchar, ' ', sizeof(dispchar));

#if defined(_SUPPORT_LCM_1618_DRIVE)
	if(freq != 0xFFFF)
    {
    	if(fm_flag)
    	{
    		dispchar[0] = 'F';
    		dispchar[1] = 'M';
    		//ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    	}
    	else
    	{
    	#if defined(_SUPPORT_AREA_)
    		if(setup.area == SETUP_AREA_USA)
    		{
    			dispchar[0] = 'M';
    			dispchar[1] = 'W';
    		}
    		else
    		{
    			dispchar[0] = 'A';
    			dispchar[1] = 'M';
    		}
    	#else
    		dispchar[0] = 'A';
    		dispchar[1] = 'M';
    	#endif

    	}

    	if(band < 10)
    	{
    		if(band == 1)
    		{
    			dispchar[2] = '1';
    		}
    		else
    		{
    			dispchar[2] = band + '0';
    		}
    	}
        dispchar[3] = ' ';
    	if(fm_flag)
    	{
			dispchar[4]=(freq/10000)+'0';
			dispchar[5]=((freq/1000)%10)+'0';
			dispchar[6]=((freq/100)%10)+'0';
			dispchar[7]='.';
			dispchar[8]=((freq/10)%10)+'0';
			dispchar[9]=(freq%10)+'0';
    		if(dispchar[4] == '0')
    		{
    			dispchar[4] = ' ';
    		}
    		if(dispchar[9] == '0')
    		{
    			dispchar[9] = ' ';
    		}
			dispchar[10]  = 'M';
			dispchar[11]  = 'H';
			dispchar[12]  = 'z';
    	}
		else
		{
			dispchar[4] = ' ';
			dispchar[5] = freq/1000+'0';
			dispchar[6]  = (freq%1000)/100+'0';
		    dispchar[7]  = freq%100/10+'0';
		    dispchar[8]  = freq%10+'0';
		    dispchar[9]  = ' ';

    		if(dispchar[5] == '0')
    		{
    			dispchar[5] = ' ';
    		}
			dispchar[10]  = 'K';
			dispchar[11]  = 'H';
			dispchar[12]  = 'z';
		}

    	ui_show_radio_band(fm_flag);
    }
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);

#elif defined(_FY7555_SPECIAL_DISPLAY_)
    uint8 index = 0;
    if(freq != 0xFFFF)
    {
    	dispchar[index++] = ' ';
    	if(fm_flag)
    	{
    		dispchar[index++] = 'F';
    		dispchar[index++] = 'M';
    	}
    	else
    	{
    	#if defined(_SUPPORT_AREA_)
    		if(setup.area == SETUP_AREA_USA)
    		{
    			dispchar[index++] = 'M';
    			dispchar[index++] = 'W';
    		}
    		else
    		{
    			dispchar[index++] = 'A';
    			dispchar[index++] = 'M';
    		}
    	#else
    		dispchar[index++] = 'A';
    		dispchar[index++] = 'M';
    	#endif
    
    	}
    
    	if(band < 10)
    	{
    		if(band == 1)
    		{
    			dispchar[index++] = '|';
    		}
    		else
    		{
    			dispchar[index++] = band + '0';
    		}
    	}
    
		dispchar[index++]  = ' ';
    	if(fm_flag)
    	{
    		dispchar[index++]=(freq/10000)+'0';
    		dispchar[index++]=((freq/1000)%10)+'0';
    		dispchar[index++]=((freq/100)%10)+'0';
    		dispchar[index++]='.';
    		dispchar[index++]=((freq/10)%10)+'0';
    		dispchar[index++]=(freq%10)+'0';
    		if(dispchar[10] == '0')
    		{
    			dispchar[10] = ' ';
    		}
    		if(dispchar[5] == '0')
    		{
    			dispchar[5] = ' ';
    		}
    
    		dispchar[index++]  = ' ';
    	}
    	else
    	{
    		dispchar[index++] = ' ';
    		dispchar[index++]  = freq/1000+'0';
    		dispchar[index++]  = (freq%1000)/100+'0';
    		dispchar[index++]  = freq%100/10+'0';
    		dispchar[index++]  = freq%10+'0';
    
    		if(dispchar[6] == '0')
    		{
    			dispchar[6] = ' ';
    		}
    		dispchar[index++]  = ' ';
    		dispchar[index++]  = ' ';
    	}
    
		dispchar[index++]  = radio_disp->info.num + '0';
		if(dispchar[12] == '0')
		{
			dispchar[12] = ' ';
		}
    	ui_show_radio_band(fm_flag);
    }
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);
#else

	uint8 index = 0;
    if(freq != 0xFFFF)
    {
		dispchar[index++] = ' ';
		dispchar[index++] = ' ';
    	if(fm_flag)
    	{
    		dispchar[index++] = 'F';
    		dispchar[index++] = 'M';
    		//ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    	}
    	else
    	{
    	#if defined(_SUPPORT_AREA_)
    		if(setup.area == SETUP_AREA_USA)
    		{
    			dispchar[index++] = 'M';
    			dispchar[index++] = 'W';
    		}
    		else
    		{
    			dispchar[index++] = 'A';
    			dispchar[index++] = 'M';
    		}
    	#else
    		dispchar[index++] = 'A';
    		dispchar[index++] = 'M';
    	#endif

    	}

    	if(band < 10)
    	{
    		if(band == 1)
    		{
    			dispchar[index++] = '|';
    		}
    		else
    		{
    			dispchar[index++] = band + '0';
    		}
    	}

		dispchar[index++] = ' ';
    	if(fm_flag)
    	{
			dispchar[index++]=(freq/10000)+'0';
			dispchar[index++]=((freq/1000)%10)+'0';
			dispchar[index++]=((freq/100)%10)+'0';
			dispchar[index++]='.';
			dispchar[index++]=((freq/10)%10)+'0';
			dispchar[index++]=(freq%10)+'0';
    		if(dispchar[11] == '0')
    		{
    			dispchar[11] = ' ';
    		}
    		if(dispchar[6] == '0')
    		{
    			dispchar[6] = ' ';
    		}

			dispchar[index++]  = ' ';
    	}
		else
		{
			dispchar[index++] = ' ';
			dispchar[index++]  = freq/1000+'0';
		    dispchar[index++]  = (freq%1000)/100+'0';
		    dispchar[index++]  = freq%100/10+'0';
		    dispchar[index++]  = freq%10+'0';

    		if(dispchar[7] == '0')
    		{
    			dispchar[7] = ' ';
    		}
			dispchar[index++]  = ' ';
			dispchar[index++]  = ' ';
		}

    	ui_show_radio_band(fm_flag);
    }
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);
#endif

#elif defined(_FY5517_SPECIAL_DISPLAY_)
    uint8 dispchar[11];
    uint8 index = 0;
    uint8 fm_flag = (freq > 2000)?1:0;

    show_clear_icon(DISP_LEVEL_NORMAL);

    memset(dispchar, ' ', sizeof(dispchar));

    if(freq != 0xFFFF)
    {
    	if(fm_flag)
    	{
    		dispchar[index++] = 'F';
    		dispchar[index++] = 'M';
    		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    	}
    	else
    	{
    	#if defined(_SUPPORT_AREA_)
    		if(setup.area == SETUP_AREA_USA)
    		{
    			dispchar[index++] = 'M';
    			dispchar[index++] = 'W';
    		}
    		else
    		{
    			dispchar[index++] = 'A';
    			dispchar[index++] = 'M';
    		}
    	#else
    		dispchar[index++] = 'A';
    		dispchar[index++] = 'M';
    	#endif

    	}

    	if(band < 10)
    	{
    		if(band == 1)
    		{
    			dispchar[index++] = '|';
    		}
    		else
    		{
    			dispchar[index++] = band + '0';
    		}
    	}
		dispchar[index++] = '-';
		dispchar[index++] = radio_disp->info.num + '0';

		if(radio_disp->info.num == 0)
		{
			dispchar[3] = ' ';
			dispchar[4] = ' ';
		}

    	if(fm_flag)
    	{
			index = 10;
		}
		else
		{
			index = 9;
		}
    	while(freq)
    	{
    		dispchar[index] = (freq%10)+'0';
    		freq /= 10;
    		index--;
    	}
    	ui_show_radio_band(fm_flag);
    }
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);

#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    uint8 dispchar[10];
    uint8 index = 0;
    uint8 fm_flag = (freq > 2000)?1:0;

    show_clear_icon(DISP_LEVEL_NORMAL);

    memset(dispchar, ' ', sizeof(dispchar));

    if(freq != 0xFFFF)
    {
    	if(fm_flag)
    	{
    		dispchar[index++] = 'F';
    		dispchar[index++] = 'M';
    		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    	}
    	else
    	{
    		dispchar[index++] = 'A';
    		dispchar[index++] = 'M';
    	}

    	if(band < 10)
    	{
    		if(band == 1)
    		{
    			dispchar[index++] = '|';
    		}
    		else
    		{
    			dispchar[index++] = band + '0';
    		}
    	}
    	dispchar[index++] = ' ';

    	if(fm_flag)
    	{
    		index = 8;
    	}
    	else
    	{
    		index = 7;
    	}
    	while(freq)
    	{
    		dispchar[index] = (freq%10)+'0';
    		freq /= 10;
    		index--;
    	}
    	ui_show_radio_band(fm_flag);
    }
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);

#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    uint8 dispchar[10];
    uint8 index = 0;
    uint8 fm_flag = (freq > 2000)?1:0;

    show_clear_icon(DISP_LEVEL_NORMAL);

    memset(dispchar, ' ', sizeof(dispchar));

    if(freq != 0xFFFF)
    {
    	if(fm_flag)
    	{
    		dispchar[index++] = 'F';
    		dispchar[index++] = 'M';
    		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    	}
    	else
    	{
    		dispchar[index++] = 'A';
    		dispchar[index++] = 'M';
    	}

    	if(band < 10)
    	{
    		if(band == 1)
    		{
    			dispchar[index++] = '|';
    		}
    		else
    		{
    			dispchar[index++] = band + '0';
    		}
    	}
    	dispchar[index++] = ' ';
    	dispchar[index++] = ' ';

    	if(fm_flag)
    	{
    		index = 9;
    	}
    	else
    	{
    		index = 8;
    	}
    	while(freq)
    	{
    		dispchar[index] = (freq%10)+'0';
    		freq /= 10;
    		index--;
    	}
    	ui_show_radio_band(fm_flag);

	#if defined(_DISABLE_FM_ICON_)
		if(global.radio_stast)
		{
    		ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
    		ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_MHZ);
    		ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_KHZ);
		}
	#endif
    }
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);

#elif defined(_YS_SPECIAL_DISP_)
	uint8 dispchar[8];
	uint8 index = 0;
	uint8 fm_flag = (freq > 2000)?1:0;

	show_clear_icon(DISP_LEVEL_NORMAL);

	memset(dispchar, ' ', sizeof(dispchar));

	if(fm_flag)
	{
		index = 4;
	}
	else
	{
		index = 3;
	}
	while(freq)
	{
		dispchar[index] = (freq%10)+'0';
		freq /= 10;
		index--;
	}

	if(fm_flag)
	{
		if(dispchar[4] == '0')
		{
			dispchar[4] = ' ';
		}
	}
	index = 5;
	if(freq != 0xFFFF)
	{
		if(fm_flag)
		{
			dispchar[index++] = 'F';
	    	dispchar[index++] = 'M';
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
		}
		else
		{
		#if defined(_SUPPORT_AREA_)
		#if defined(_SUPPORT_THREE_AREA_)
			if((setup.area == SETUP_AREA_USA)||(setup.area == SETUP_AREA_LATIN))
		#else
			if(setup.area == SETUP_AREA_USA)
		#endif
			{
				dispchar[index++] = 'M';
	    		dispchar[index++] = 'W';
			}
			else
			{
				dispchar[index++] = 'A';
	    		dispchar[index++] = 'M';
			}
		#else
	    	dispchar[index++] = 'A';
	    	dispchar[index++] = 'M';
		#endif

		}

		if(band < 10)
		{
			dispchar[index++] = band + '0';
		}
		ui_show_radio_band(fm_flag);
	}
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);

#elif defined(_ENABLE_RADIO_WB_)
    uint8 dispchar[8];
    uint8 index = 0;
	u32 xFreq;
	
    show_clear_icon(DISP_LEVEL_NORMAL);
    
    memset(dispchar, ' ', sizeof(dispchar));
	if(freq != 0xFFFF)
	{
		if(freq >= 16240)//WB
		{
		#if defined(_ENABLE_WB_DISPLAY_)
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_WB_POINT);
		#endif
			dispchar[0] = 'W';
			dispchar[1] = 'B';
			xFreq = (freq*25);
			xFreq = (xFreq/10);
			
    		dispchar[2]=(xFreq/100000)+'0';
    		dispchar[3]=((xFreq/10000)%10)+'0';
    		dispchar[4]=((xFreq/1000)%10)+'0';
    		dispchar[5]=((xFreq/100)%10)+'0';
    		dispchar[6]=((xFreq/10)%10)+'0';
    		dispchar[7]=(xFreq%10)+'0';
		}
		else if(freq >= 8700)//FM
		{
			dispchar[index++] = 'F';
			dispchar[index++] = 'M';
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
			if(band < 10)
			{
				if(band == 1)
				{
					dispchar[index++] = '|';
				}
				else
				{
					dispchar[index++] = band + '0';
				}
			}
			index = 7;
			while(freq)
			{
				dispchar[index] = (freq%10)+'0';
				freq /= 10;
				index--;
			}
			if(dispchar[7] == '0')
			{
				dispchar[7] = ' ';
			}
		}
		else//AM
		{
			dispchar[index++] = 'A';
			dispchar[index++] = 'M';
			if(band < 10)
			{
				if(band == 1)
				{
					dispchar[index++] = '|';
				}
				else
				{
					dispchar[index++] = band + '0';
				}
			}
			index = 7;
			while(freq)
			{
				dispchar[index] = (freq%10)+'0';
				freq /= 10;
				index--;
			}
		}
	}
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);

#else
	uint8 dispchar[8];
	uint8 index = 0;
	uint8 fm_flag = (freq > 2000)?1:0;

	show_clear_icon(DISP_LEVEL_NORMAL);

	memset(dispchar, ' ', sizeof(dispchar));

	if(freq != 0xFFFF)
	{
		if(fm_flag)
		{
		#if defined(_FIVE_CHAR_DISPLAY_)||defined(_SEVEN_CHAR_DISPLAY_)
		#if defined(_PS329a_SPECIAL_DISPLAY_)||defined(_SPECIAL_CHAR_DISPLAY_)
    		dispchar[index++] = 'F';
    		dispchar[index++] = 'M';
		#else
			dispchar[index++] = 'F';
		#endif
		#else
			dispchar[index++] = 'F';
	    	dispchar[index++] = 'M';
		#endif
			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);

		#if defined(SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
			FM_BAND_DISP(ON);
		#endif
		}
		else
		{
		#if defined(_SUPPORT_AREA_)
		#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
			if((setup.area == SETUP_AREA_USA)||(setup.area == SETUP_AREA_LATIN))
		#else
			if(setup.area == SETUP_AREA_USA)
		#endif
			{
			#if defined(_FIVE_CHAR_DISPLAY_)||defined(_SEVEN_CHAR_DISPLAY_)
				dispchar[index++] = 'A';
			#elif defined(_USA_DISPLAY_AM_)||defined(_LK8005_SPECIAL_DISPLAY_)||defined(_YONGHE_SPECIAL_DISPLAY_)
    			dispchar[index++] = 'A';
    			dispchar[index++] = 'M';
			#else
				dispchar[index++] = 'M';
	    		dispchar[index++] = 'W';
			#endif
			}
			else
			{
			#if defined(_FIVE_CHAR_DISPLAY_)||defined(_ZSBY5209_SPECIAL_DISPLAY_)
				dispchar[index++] = 'A';
			#else
			#if defined(_TS8010_CHAR_DISPLAY_)||defined(_YONGHE_SPECIAL_DISPLAY_)
		        dispchar[index++] = 'M';
	    	    dispchar[index++] = 'W';
		    #else
				dispchar[index++] = 'A';
	    		dispchar[index++] = 'M';
			#endif
			#endif
			}
		#else
	    	dispchar[index++] = 'A';
	    	dispchar[index++] = 'M';
		#endif

		}

		if(band < 10)
		{
			if(band == 1)
			{
			#if defined(_ZSBY5209_SPECIAL_DISPLAY_)
				dispchar[index++] = '1';
			#else
				dispchar[index++] = '|';
			#endif
			}
			else
			{
				dispchar[index++] = band + '0';
			}
		}

	#if defined(_FIVE_CHAR_DISPLAY_)
		if(fm_flag)
		{
			index = 6;
		}
		else
		{
			index = 5;
		}
	#elif defined(_SEVEN_CHAR_DISPLAY_)
		#if defined(_PS329a_SPECIAL_DISPLAY_)||defined(_SPECIAL_CHAR_DISPLAY_)
			if(fm_flag)
			{
				index = 7;
			}
			else
			{
				index = 6;
			}
		#else
			index = 6;
		#endif
	#elif defined(_HKY1032_SPECIAL_DISPLAY_)
		index = 8;
	#else
	#if defined(_TS8010_CHAR_DISPLAY_)
	    if(fm_flag)
		{
			index = 8;
		}
		else
		{
			index = 7;
		}
	#elif defined(_HH_SPECIAL_DISPLAY_)
	    if(fm_flag)
		{
			index = 7;
		}
		else
		{
			index = 6;
		}			
	#else
		index = 7;
	#endif
	#endif

		while(freq)
		{
			dispchar[index] = (freq%10)+'0';
			freq /= 10;
			index--;
		}

		if(fm_flag)
		{
		#if defined(_SWL_FREQ_DISP_)||defined(_TS8010_CHAR_DISPLAY_)
			if(dispchar[7] == '0')
			{
				dispchar[7] = '0';
			}
		#else
		#if !defined(_HKY1032_SPECIAL_DISPLAY_)
		#if !defined(_WLGK_SPECIAL_FREQ_)
			if(dispchar[7] == '0')
			{
				dispchar[7] = ' ';
			}
		#endif
		#endif
		#endif
		}
	#if defined(_4MIBA_4SUZIBA_)
		if(dispchar[3] == '1')
		{
			dispchar[3]=' ';
			#if defined(_FM_NUM_1_USER_DISC_ICON_)
			ui_drv_func.icon_open(0, UI_ICON_DISC);
			#endif
		}
		else
		{
			dispchar[3]=' ';
			#if defined(_FM_NUM_1_USER_DISC_ICON_)
			ui_drv_func.icon_close(0, UI_ICON_DISC);
			#endif
		}
	#elif defined(_FIVE_CHAR_DISPLAY_)||defined(_SEVEN_CHAR_DISPLAY_)
		if(fm_flag)
		{
		#if defined(_PS329a_SPECIAL_DISPLAY_)||defined(_SPECIAL_CHAR_DISPLAY_)
			if(dispchar[7] == '0')
			{
				dispchar[7] = ' ';
			}
		#else
			if(dispchar[6] == '0')
			{
				dispchar[6] = ' ';
			}
		#endif
		}
	#endif

		ui_show_radio_band(fm_flag);
	}
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
#endif
}
#endif

static void ui_show_radio_num(uint8 num)
{
#if !defined(_SUPPORT_C912DAB_)
	if(!num)
	{
		num = 0xFF;
	}
	if(num == 0xFF)
	{
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CH);
	}
	else
	{
		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CH);
	}
	ui_show_num(DISP_LEVEL_NORMAL, num);
#endif
}

static void ui_show_radio_clean(void)
{
	uint16 disp_buf[4];
	uint8  disp_ctrl[3];

	disp_buf[0] = UI_ICON_MHZ;		disp_ctrl[0] = 0;
	disp_buf[1] = UI_ICON_KHZ;		disp_ctrl[1] = 0;
	disp_buf[2] = UI_ICON_FM;		disp_ctrl[2] = 0;
	disp_buf[3] = UI_ICON_NULL;

	ui_drv_func.icon_ctrl(UI_LEVEL_ALL, disp_buf, disp_ctrl);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CH);

	ui_show_radio_icon(0, 0, 0);
	show_clear_icon(DISP_LEVEL_NORMAL);
	show_clear_num(DISP_LEVEL_NORMAL);
    ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MONO);

#if defined(_FM_NUM_1_USER_DISC_ICON_)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_DISC);
#endif
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_PTY);
	ui_drv_func.flash_disable(UI_ICON_PTY);
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

void ui_show_radio_init(void)
{
	radio_disp = (radio_disp_t *)malloc_fun(radio_disp ,sizeof(radio_disp_t), 0);
	if(radio_disp == NULL)
	{
        while(1);
	}
	ui_puts("ui_show_radio_init\n");


	ui_show_radio_clean();

#if defined(_DISABLE_RADIO_LOGO_)
#if defined(_ENABLE_POWER_LOGO_)
	if(global.power_up_flag)
	{
	#if defined(_JTI_POWER_LOGO_)
		ui_show_bandch1();
	#else
		global.power_up_flag = FALSE;
	#if defined(_DISABLE_BT_MEMORY_)
		if(global.radio_flag)
		{
			ui_show_setting(LANG_ID_RADIO, 0);
			ui_disp.timer = 3;//50
			ui_show_normal(LANG_ID_RADIO);
		}
		else
		{
			ui_show_radio_freq(setup.radio_freq, setup.radio_band);
			ui_show_radio_num(setup.radio_num);
		}
		global.radio_flag = FALSE;
	#else
		ui_show_radio_freq(setup.radio_freq, setup.radio_band);
		ui_show_radio_num(setup.radio_num);
	#endif
	#endif
	}
	else
	{
		ui_show_setting(LANG_ID_RADIO, 0);
		ui_disp.timer = 3;//50
		ui_show_normal(LANG_ID_RADIO);
	}
#else
    ui_show_radio_freq(setup.radio_freq, setup.radio_band);
    ui_show_radio_num(setup.radio_num);
#endif

	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RADIO);

#elif defined(_WLS1403_SPECIAL_DISP_)
	ui_disp.timer = 1;
#else

#if !defined(_YS_SPECIAL_DISP_)
#if !defined(SPECIAL_JD_DISP)
#if !defined(_SUPPORT_LCM_1618_DRIVE)//
	ui_show_setting(LANG_ID_RADIO, 0);
#endif
#if defined(_SUPPORT_C912DAB_)
	ui_disp.timer = 2500;
#elif defined(_ENABLE_RADIO_LOGO_TIME_)
	ui_disp.timer = 3;//50
#elif defined(_SL6812A_RADIO_TIME_)
	ui_disp.timer = 2000;
#else
	ui_disp.timer = 1000;
#endif
	ui_show_normal(LANG_ID_RADIO);
	ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RADIO);
#endif
#endif
#endif

#if defined(_FY7555_LCD_DISPLAY_)
	ui_show_setting1(LANG_ID_RADIO, 0);
	ui_show_normal1(LANG_ID_RADIO);
#endif
}

void ui_show_radio_exit(void)
{
	ui_puts("ui_show_radio_exit\n");

	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RADIO);

	ui_show_radio_clean();
#if defined(SUPPORT_ANTENNA_DISPLAY_)|| defined (SUPPORT_ANTENNA_DISPLAY_AS_FMBAND)
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI1);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI2);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI3);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI4);
#endif

	if(radio_disp != NULL)
	{
		free_fun((void**)&radio_disp);
	}
}
#if defined(SUPPORT_ANTENNA_DISPLAY_)
void antenna_icon_polling(void)
{
	uint8 fm_flag = (radio_disp->info.freq > 2000)?1:0;

	//ew_printf("radio_disp---->info.freq:%d,\n", radio_disp->info.freq);
	//ew_printf("setup.rssi:%d,\n", setup.rssi);

	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI1);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI2);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI3);
	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_RSSI4);
//#if defined(_SI473x_RSSI_DISPLAY_)
	if(global.radio_type)
	{
		if(fm_flag)
		{
			if(setup.rssi < 17)
			{
			}
			else if((setup.rssi >= 17)&&(setup.rssi < 30))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
			}
			else if((setup.rssi >= 30)&&(setup.rssi < 45))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
			}
			else if((setup.rssi >= 45)&&(setup.rssi < 60))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI3);
			}
			else if(setup.rssi >= 60)
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI3);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI4);
			}
		}
		else
		{
			if(setup.rssi <= 35)
			{
			}
			else if((setup.rssi > 35)&&(setup.rssi < 48))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
			}
			else if((setup.rssi >= 48)&&(setup.rssi < 58))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
			}
			else if((setup.rssi >= 58)&&(setup.rssi < 70))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI3);
			}
			else if(setup.rssi >= 70)
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI3);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI4);
			}
		}

	}
	else
	{
		if(fm_flag)
        {
        	if(setup.rssi < 54)
        	{
        	}
        	else if((setup.rssi >= 54)&&(setup.rssi < 64))
        	{
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
        	}
        	else if((setup.rssi >= 64)&&(setup.rssi < 80))
        	{
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
        	}
        	else if((setup.rssi >= 80)&&(setup.rssi < 94))
        	{
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI3);
        	}
        	else if(setup.rssi >= 94)
        	{
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI3);
        		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI4);
        	}
        }
		else
		{
			if(setup.rssi <= 81)
			{
			}
			else if((setup.rssi > 81)&&(setup.rssi < 101))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
			}
			else if((setup.rssi >= 101)&&(setup.rssi < 114))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
			}
			else if((setup.rssi >= 114)&&(setup.rssi < 125))
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI3);
			}
			else if(setup.rssi >= 125)
			{
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI1);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI2);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI3);
				ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_RSSI4);
			}
		}
	}
//#endif
}
#endif

void ui_show_radio_polling(void)
{
#if defined(_ENABLE_RECORD_RADIO_)
    if(rec_status == rec_start)
    {
		global.record_flag = TRUE;
    	rec_time_disp(DISP_LEVEL_NORMAL,rec_time_count);
    	if(ui_disp.timer == 0)
    	{
		#if defined(_FIVE_CHAR_DISPLAY_)
    		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
    		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_DASH);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);
		#elif defined(_FY7000_SPECIAL_DISPLAY_)
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_CLOCK);
		#else
    		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_POINT);
		#endif
    	}
    }
    else if(rec_status == rec_pp)
    {
	#if defined(_FIVE_CHAR_DISPLAY_)
    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_DASH);
	#elif defined(_FY7000_SPECIAL_DISPLAY_)
		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_CLOCK);
	#else
    	ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_POINT);
	#endif
    	ui_show_normal(LANG_ID_PAUSE);
    }
    else if(rec_status == rec_stop)
#endif
    {
    #if defined(_ENABLE_RECORD_RADIO_)
    	if(global.record_flag)
    	{
    		global.record_flag = FALSE;
    		ui_show_radio_freq(radio_disp->info.freq, radio_disp->info.band);
    		ui_show_radio_num(radio_disp->info.num);
    	}
    #endif

    #if defined(SUPPORT_ANTENNA_DISPLAY_)
    	if(global.sys_power)
    	{
    		antenna_icon_polling();
    	}
    #endif

    #if defined(_SUPPORT_C912DAB_)
     	uint8 dispchar[8];
    	memcpy(dispchar,Disp_dab_buf,8);
        if(Disp_dab_buf[8]!=0x20)
        {
        	ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_POINT);
        }
        else
        {
        	ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
        }
    	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
    #else
    #if defined(TD_ADD_BANDCH)
    	if(radio_disp->ch_flag)
    	{
    		if(radio_disp->ch_timer)
    		{
    			radio_disp->ch_timer--;
    		}
    		else
    		{
    			ui_show_radio_freq(radio_disp->info.freq, radio_disp->info.band);
    			radio_disp->ch_flag = FALSE;
    		}
    	}
    #endif
    #if defined(TD_ADD_CHADD_SUB)
    	if(global.SaveRadio)
    	{
    		if(global.SaveRadioTime)
    		{
            	disp_add_sub();
                global.SaveRadioTime --;
    		}
    		else
    		{
    			ui_show_radio_freq(radio_disp->info.freq, radio_disp->info.band);
            	global.SaveRadio =FALSE;
    		}
    	}
    #endif
#if defined(_WK5951CH_SPECIAL_DISPLAY_)
    if(radio_disp->ch_flag1)
    {
    	if(radio_disp->ch_timer1)
    	{
    		radio_disp->ch_timer1--;
    	}
    	else
    	{
    		ui_show_radio_freq(radio_disp->info.freq, radio_disp->info.band);
    		radio_disp->ch_flag1 = FALSE;
    	}
    }
#endif
    #endif
        if(setup.mono_flg)
        {
    		ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_MONO);
        }
    	else
    	{
    		ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MONO);
    	}


    #if defined(_ENABLE_TIME_DISP_)//同一频率，刷新一次显示
    	if(setup.time_disp)
    	{
    		if(global.work_flag)
    		{
    			global.work_flag = FALSE;
    			ui_show_radio_freq(radio_disp->info.freq, radio_disp->info.band);
    			ui_show_radio_num(radio_disp->info.num);
    		}
    	}
    	else
    	{
    		if(global.work_flag1)
    		{
    			global.work_flag1 = FALSE;
    			ui_show_radio_freq(radio_disp->info.freq, radio_disp->info.band);
    			ui_show_radio_num(radio_disp->info.num);
    		}
    	}
    #endif
	
    #if defined(TD_ADD_LCD_NUM_4)
    	if(global.version1)
    	{
    		global.version_time++;
    		if(global.version_time >= 3000/RADIO_POLLING_TIME)
    		{
    			global.version1 = FALSE;
    			global.version_time = 0;
    			ui_show_radio_freq(radio_disp->info.freq, radio_disp->info.band);
    			ui_show_radio_num(radio_disp->info.num);
    		}
    	}
    #endif

    #if defined(_DISABLE_FM_ICON_)
    	if(!global.radio_stast)
    	{
    		ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
    		if(radio_disp->info.freq > 2000)
    		{
    			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_MHZ);
    		}
    		else
    		{
    			ui_drv_func.icon_open(DISP_LEVEL_NORMAL, UI_ICON_KHZ);
    		}
    	}
    #endif

    #if defined(_SL6812_TEST_LCD_)
    	if(global.lcd_mode_flag)
    	{
    		puts("_SL6812_TEST_LCD_1111");
    		global.lcd_mode_flag = FALSE;
    		if(radio_disp->info.flag & RADIO_DISP_STO)
    		{
    			ui_show_radio_icon(0, 1, 0);
    		}
    		ui_show_radio_freq(radio_disp->info.freq, radio_disp->info.band);
    	}
    #endif
    }
}

#if defined(_WK5951CH_SPECIAL_DISPLAY_)
static void ui_show_radio_band_num1(uint8 num)
{
	uint8 dispchar[6];

    dispchar[0] = ' ';
    dispchar[1] = ' ';
    if(GET_RADIO_BAND <=2)
    {
    	dispchar[2] = 'F';
        dispchar[3] =GET_RADIO_BAND+'1';
    }
    else
    {
    	dispchar[2] = 'A';
        dispchar[3] =(GET_RADIO_BAND-3)+'1';
    }
    dispchar[4] ='P';
    dispchar[5] =num+'0';
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_RADIO);
    ui_drv_func.icon_close(DISP_LEVEL_NORMAL, UI_ICON_POINT);
    ui_show_string(DISP_LEVEL_NORMAL, dispchar);
}
#endif

void ui_show_radio_play(void *info)
{
	if(radio_disp == NULL)return;
	radio_disp_info_t *disp_info = (radio_disp_info_t *)info;

	if(radio_disp->info.state != disp_info->state)
	{
	#if defined(_ENABLE_RADIO_RDS_)
		if(rds_ctrl_enable() && (setup.rds.af || setup.rds.ta))
		{
		enum
		{
			RADIO_STATE_IDLE = 0,
			RADIO_STATE_SEEK,
			RADIO_STATE_SCAN,
			RADIO_STATE_AUTO_SAVE,
			RADIO_STATE_BROWSE,
		};
			switch(disp_info->state)
			{
				case RADIO_STATE_SEEK:
				case RADIO_STATE_SCAN:
				case RADIO_STATE_AUTO_SAVE:
				#if defined(_RDS_SEARCH_DISP_FREQ_)
					radio_disp->state_flag = FALSE;
					radio_disp->info.freq = 0;
				#else
					radio_disp->state_flag = TRUE;
					ui_show_normal(LANG_ID_RDS_SEARCH);
				#endif
					break;

				default:
					radio_disp->state_flag = FALSE;
					radio_disp->info.freq = 0;
					break;
			}
		}
		else
		{
			if(radio_disp->state_flag)
			{
				radio_disp->state_flag = FALSE;
				radio_disp->info.freq = 0;
			}
		}
	#endif

		radio_disp->info.state = disp_info->state;
	}

	if(disp_info->psn != NULL)
	{
		ui_printf("radio_disp---->psn:%c%c%c%c%c%c%c%c\n",
							disp_info->psn[0],
							disp_info->psn[1],
							disp_info->psn[2],
							disp_info->psn[3],
							disp_info->psn[4],
							disp_info->psn[5],
							disp_info->psn[6],
							disp_info->psn[7]);

		if(memcmp(radio_disp->psn, disp_info->psn, 8))
		{
			memcpy(radio_disp->psn, disp_info->psn, 8);

			if(radio_disp->psn[0])
			{
				radio_disp->psn_flag = TRUE;
				ui_show_radio_psn(radio_disp->psn);
			}
			else
			{
				radio_disp->psn_flag = FALSE;
				radio_disp->info.freq = 0;
			}
		}
	}
#if defined(TD_ADD_BANDCH)
	if(disp_info->flag & RADIO_DISP_CH)
	{
		radio_disp->ch_flag = TRUE;
	#if defined(_ENABLE_CH_DISPLAY_)
	#if defined(_JTI_POWER_LOGO_)&&0
    	if(global.power_up_flag)
    	{
    		global.power_up_flag = FALSE;
    	}
		else
		{
			radio_disp->ch_timer = 1000/RADIO_POLLING_TIME;
			ui_show_radio_band_num(disp_info->freq, disp_info->band, disp_info->num);
		}
	#else
    	radio_disp->ch_timer = 1000/RADIO_POLLING_TIME;
    	ui_show_radio_band_num(disp_info->freq, disp_info->band, disp_info->num);
	#endif
	#else
	#if defined(_ZSBY5209_CH_DISPLAY_)
    	if((disp_info->num)&&(disp_info->num != 0xFF))
    	{
    		radio_disp->ch_timer = 1000/RADIO_POLLING_TIME;
    		ui_show_bandch();
    	}
    	else
    	{
    		ui_show_radio_freq(disp_info->freq, disp_info->band);
    	}
	#else
		if(disp_info->num)
		{
			radio_disp->ch_timer = 1000/RADIO_POLLING_TIME;
            ui_show_bandch();
		}
	#endif
	#endif
		disp_info->flag &= ~RADIO_DISP_CH;
	}
#endif
#if defined(_WK5951CH_SPECIAL_DISPLAY_)
    if(disp_info->flag & RADIO_DISP_CH1)
    {
    	radio_disp->ch_flag1 = TRUE;
    	radio_disp->ch_timer1 = 700/RADIO_POLLING_TIME;
    	ui_show_radio_band_num1(radio_ctrl->saveIndex);
    	disp_info->flag &= ~RADIO_DISP_CH1;
    }
#endif

#if defined(_SPECIAL_RADIO_DISPLAY_)
	if((radio_disp->info.freq != disp_info->freq)||(radio_disp->info.band != disp_info->band)
	||(radio_disp->info.num != disp_info->num))
	{		
		radio_disp->info.freq = disp_info->freq;
		radio_disp->info.band = disp_info->band;
		radio_disp->info.num = disp_info->num;
		
		if((!radio_disp->state_flag)
		&& (!radio_disp->psn_flag)
		)
		{
			ui_show_radio_freq(disp_info->freq, disp_info->band, disp_info->num);
		}
	}
#else
	//printf("disp_info->freq:%d\n",disp_info->freq);
	if((radio_disp->info.freq != disp_info->freq)||(radio_disp->info.band != disp_info->band))
	{
		radio_disp->info.freq = disp_info->freq;
		radio_disp->info.band = disp_info->band;
	#if defined(TD_ADD_CHADD_SUB)
        global.SaveRadio=FALSE;
		global.SaveRadioTime =0;;
	#endif
		if(((!radio_disp->state_flag) && (!radio_disp->psn_flag))
	#if defined(TD_ADD_BANDCH)
		&& (!radio_disp->ch_flag)
	#endif
	#if defined(_WK5951CH_SPECIAL_DISPLAY_)
		&& (!radio_disp->ch_flag1)
	#endif
        )
		{
        #if defined(_DISABLE_RADIO_LOGO_)
        	setup.radio_freq = disp_info->freq;
			setup.radio_band = disp_info->band;
        #endif

		#if defined(_ADD_SPECIAL_CH_DISP_)
		    if(global.fm_disp)
		    {
		        global.fm_time--;
				if(global.fm_time == 0)
				{
                    global.fm_disp = FALSE;
				}
				else
				{
                    chang_fm_disp();
				}				
			}
			else
			{
               ui_show_radio_freq(disp_info->freq, disp_info->band);
			}
		#elif defined(_SPECIAL_RADIO_DISPLAY_)
			ui_show_radio_freq(disp_info->freq, disp_info->band, disp_info->num);
        #else
			ui_show_radio_freq(disp_info->freq, disp_info->band);
		#endif	
		}
	}
#endif

    if(radio_disp->info.num != disp_info->num)
    {//CH通道有变化
    	radio_disp->info.num = disp_info->num;
    	ui_show_radio_num(disp_info->num);
    #if defined(_DISABLE_RADIO_LOGO_)
    	setup.radio_num = disp_info->num;
    #endif
    #if defined(_FY5517_SPECIAL_DISPLAY_)
    	ui_show_radio_freq(disp_info->freq, disp_info->band);
    #endif
    }

	if(radio_disp->info.flag != disp_info->flag)
	{
		uint8 st  = FALSE;
		uint8 sto = FALSE;
		uint8 loc = FALSE;

		if(disp_info->flag & RADIO_DISP_ST)	st  = TRUE;
		if(disp_info->flag & RADIO_DISP_STO)sto = TRUE;
		if(disp_info->flag & RADIO_DISP_LOC)loc = TRUE;
		ui_show_radio_icon(st, sto, loc);
		radio_disp->info.flag = disp_info->flag;
	}
}

void ui_show_radio_st(uint8 st)
{
	if(st)
	{
		if(!(radio_disp->info.flag & RADIO_DISP_ST))
		{
			ui_show_setting(LANG_ID_RADIO_ST_ON, 0);
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_ST);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_MONO);
			radio_disp->info.flag |= RADIO_DISP_ST;
			setup.mono_flg = FALSE;
		}
	}
	else
	{
		if(radio_disp->info.flag & RADIO_DISP_ST)
		{
			ui_show_setting(LANG_ID_RADIO_ST_OFF, 0);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_ST);
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_MONO);
			radio_disp->info.flag &= (~RADIO_DISP_ST);
			setup.mono_flg = TRUE;
		}
	}
}

void ui_show_radio_loc(uint8 loc)
{
	if(loc)
	{
		if(!(radio_disp->info.flag & RADIO_DISP_LOC))
		{
			ui_show_setting(LANG_ID_RADIO_LOC_ON, 0);
			ui_drv_func.icon_open(UI_LEVEL_ALL, UI_ICON_LOC);
			radio_disp->info.flag |= RADIO_DISP_LOC;
			setup.loc_flg = TRUE;
		}
	}
	else
	{
		if(radio_disp->info.flag & RADIO_DISP_LOC)
		{
			ui_show_setting(LANG_ID_RADIO_LOC_OFF, 0);
			ui_drv_func.icon_close(UI_LEVEL_ALL, UI_ICON_LOC);
			radio_disp->info.flag &= (~RADIO_DISP_LOC);
			setup.loc_flg = FALSE;
		}
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_ENABLE_VERSION_VIEW_)

uint8 version_password[VERSION_PASSWORD_SIZE];

static uint8 version_mouth_to_number[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static uint8 version_info_date[] = __DATE__;
static uint8 version_info_time[] = __TIME__;
#if defined(_ENABLE_CHANGE_VERSION_DISP_)
static uint8 version_user_password[VERSION_PASSWORD_SIZE] = "315426";
#else
static uint8 version_user_password[VERSION_PASSWORD_SIZE] = "112212";
#endif

void radio_version_show(void)
{
#if defined(_ENABLE_USER_DEFINED_VERSION_VIEW_)
	uint8 dispchar[8] = {USER_DEFINED_VERSION_STR};
#else

#if defined(_UI_DISP_4_CHAR_)
	uint8 dispchar[4];
#elif defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 dispchar[13];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 dispchar[11];
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
	uint8 dispchar[10];
#elif defined(_FIVE_CHAR_DISPLAY_)
	uint8 dispchar[6];
#elif defined(_SEVEN_CHAR_DISPLAY_)
	uint8 dispchar[9];
#else
	uint8 dispchar[8];
#endif
	uint8 index;

	memset(dispchar, ' ', sizeof(dispchar));

	for(index = 0; index < 12; index++)
	{
		if(!memcmp(version_info_date, version_mouth_to_number[index], 3))
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

#if defined(TD_ADD_LCD_NUM_4)
    dispchar[0] = index/10 + '0';
    dispchar[1] = index%10 + '0';
    dispchar[2] = (version_info_date[4]== ' ')? '0' : version_info_date[4];
    dispchar[3] = (version_info_date[5]== ' ')? '0' : version_info_date[5];
	global.version = TRUE;
#elif defined(_LCD_3_DIAPLAY_)
    dispchar[0] = version_info_time[0];
    dispchar[1] = version_info_time[1];
    dispchar[2] = version_info_time[3];
    dispchar[3] = version_info_time[4];
#elif defined(_FY7000_SPECIAL_DISPLAY_)
    dispchar[0] = ' ';
    dispchar[1] = ' ';
    dispchar[2] = index/10 + '0';
    dispchar[3] = index%10 + '0';
    dispchar[4] = (version_info_date[4]== ' ')? '0' : version_info_date[4];
    dispchar[5] = (version_info_date[5]== ' ')? '0' : version_info_date[5];
    dispchar[6] = version_info_time[0];
    dispchar[7] = version_info_time[1];
    dispchar[8] = version_info_time[3];
    dispchar[9] = version_info_time[4];
    dispchar[10] = ' ';
    dispchar[11] = ' ';
    dispchar[12] = ' ';
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
    dispchar[0] = ' ';
    dispchar[1] = index/10 + '0';
    dispchar[2] = index%10 + '0';
    dispchar[3] = (version_info_date[4]== ' ')? '0' : version_info_date[4];
    dispchar[4] = (version_info_date[5]== ' ')? '0' : version_info_date[5];
    dispchar[5] = version_info_time[0];
    dispchar[6] = version_info_time[1];
    dispchar[7] = version_info_time[3];
    dispchar[8] = version_info_time[4];
    dispchar[9] = ' ';
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
    dispchar[0] = index/10 + '0';
    dispchar[1] = index%10 + '0';
    dispchar[2] = (version_info_date[4]== ' ')? '0' : version_info_date[4];
    dispchar[3] = (version_info_date[5]== ' ')? '0' : version_info_date[5];
    dispchar[4] = version_info_time[0];
    dispchar[5] = version_info_time[1];
    dispchar[6] = version_info_time[3];
    dispchar[7] = version_info_time[4];
    dispchar[8] = ' ';
    dispchar[9] = ' ';
#elif defined(_FY5517_SPECIAL_DISPLAY_)
    dispchar[0] = ' ';
    dispchar[1] = index/10 + '0';
    dispchar[2] = index%10 + '0';
    dispchar[3] = (version_info_date[4]== ' ')? '0' : version_info_date[4];
    dispchar[4] = (version_info_date[5]== ' ')? '0' : version_info_date[5];
    dispchar[5] = version_info_time[0];
    dispchar[6] = version_info_time[1];
    dispchar[7] = version_info_time[3];
    dispchar[8] = version_info_time[4];
    dispchar[9] = ' ';
    dispchar[10] = ' ';
#else
	dispchar[0] = index/10 + '0';
	dispchar[1] = index%10 + '0';
	dispchar[2] = (version_info_date[4]== ' ')? '0' : version_info_date[4];
	dispchar[3] = (version_info_date[5]== ' ')? '0' : version_info_date[5];
#if defined(_FIVE_CHAR_DISPLAY_)
	dispchar[4] = version_info_time[3];
	dispchar[5] = version_info_time[4];
	dispchar[6] = ' ';
	dispchar[7] = ' ';
#elif defined(_SEVEN_CHAR_DISPLAY_)
	dispchar[4] = version_info_time[0];
	dispchar[5] = version_info_time[1];
	dispchar[6] = version_info_time[3];
	dispchar[7] = ' ';
	dispchar[8] = version_info_time[4];
#else
	dispchar[4] = version_info_time[0];
	dispchar[5] = version_info_time[1];
	dispchar[6] = version_info_time[3];
	dispchar[7] = version_info_time[4];
#endif
#endif
#endif
	show_clear_icon(DISP_LEVEL_SETTING);
#if defined(_SPECIAL_LCD_4_DIAPLAY_)||defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
	show_clear_num(DISP_LEVEL_SETTING);
#endif
	ui_show_string(DISP_LEVEL_SETTING, dispchar);
	ui_disp.now_level = DISP_LEVEL_SETTING;
#if defined(TD_ADD_LCD_NUM_4)
#if defined(_JYL3802E_VERSION_VIEW_)
	ui_disp.timer = 5000;
#else
	ui_disp.timer = 3000;
#endif
#else
	ui_disp.timer = 5000;
#endif
}

#if defined(TD_ADD_LCD_NUM_4)
void radio_version_show1(void)
{
	uint8 dispchar[4];

	memset(dispchar, ' ', sizeof(dispchar));

    dispchar[0] = version_info_time[0];
    dispchar[1] = version_info_time[1];
    dispchar[2] = version_info_time[3];
    dispchar[3] = version_info_time[4];
	show_clear_icon(DISP_LEVEL_NORMAL);
#if defined(_SPECIAL_LCD_4_DIAPLAY_)
	show_clear_num(DISP_LEVEL_NORMAL);
#endif	
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);
	ui_disp.now_level = DISP_LEVEL_SETTING;
	ui_disp.timer = 3000;
}
#endif

void ui_show_radio_version(uint8 sel)
{
	uint8 index;
	uint8 number;

	number = sel + '0';
	for(index = 0; index < (VERSION_PASSWORD_SIZE - 1); index++)
	{
		version_password[index] = version_password[index + 1];
	}

	version_password[index] = number;

	if(!memcmp(version_password, version_user_password, VERSION_PASSWORD_SIZE))
	{
		memset(version_password, 0, sizeof(version_password));

		radio_version_show();
	}
}
#endif

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
#if defined(ENABLE_PASSWORD_RESET)
uint8 version_password1[VERSION_PASSWORD_SIZE1];
static uint8 version_user_password1[VERSION_PASSWORD_SIZE1] = "116616";
extern void ext_memory_clear(void);

void ui_show_soft_reset(uint8 sel)
{
	uint8 index;
	uint8 number;

	ew_puts("ui_show_soft_reset\n");
	number = sel + '0';
	for(index = 0; index < (VERSION_PASSWORD_SIZE1 - 1); index++)
	{
		version_password1[index] = version_password1[index + 1];
	}
	
	version_password1[index] = number;
	
	if(!memcmp(version_password1, version_user_password1, VERSION_PASSWORD_SIZE1))
	{
		ew_puts("ui_show_soft_reset111\n");
		memset(version_password1, 0, sizeof(version_password1));
		ext_memory_clear();
		OSTimeDly(20);
		JL_POWER->CON |= BIT(4);
	}
}
#endif

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_SUPPORT_LCM_DRIVE)
extern const uint8 spec_char_Index[];
u8 Digt_8_16buf[][16]=
{
    {0x00,0x07,0x08,0x10,0x10,0x10,0x08,0x07,0x00,0xF0,0x08,0x04,0x04,0x04,0x08,0xF0},/*"0",0*/
	{0x00,0x00,0x04,0x0C,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00},/*"1",0*/
	{0x00,0x06,0x08,0x10,0x10,0x08,0x07,0x00,0x00,0x0C,0x14,0x24,0x44,0x84,0x04,0x00},/*"2",0*/
	{0x00,0x0C,0x10,0x10,0x10,0x10,0x0F,0x00,0x00,0x18,0x04,0x04,0x84,0x84,0x78,0x00},/*"3",0*/
	{0x00,0x00,0x00,0x03,0x04,0x08,0x1F,0x00,0x00,0x60,0xA0,0x20,0x20,0x20,0xFC,0x20},/*"4",0*/
	{0x00,0x1F,0x12,0x12,0x12,0x12,0x11,0x00,0x00,0x18,0x04,0x04,0x04,0x04,0xF8,0x00},/*"5",0*/
	{0x00,0x07,0x08,0x11,0x11,0x11,0x0C,0x00,0x00,0xF0,0x88,0x04,0x04,0x04,0xF8,0x00},/*"6",0*/
	{0x00,0x10,0x10,0x10,0x10,0x13,0x1C,0x00,0x00,0x00,0x00,0x0C,0x70,0x80,0x00,0x00},/*"7",0*/
	{0x00,0x06,0x09,0x10,0x10,0x10,0x09,0x06,0x00,0x30,0x48,0x84,0x84,0x84,0x48,0x30},/*"8",0*/
	{0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,0x00,0x00,0x8C,0x44,0x44,0x88,0xF0,0x00},/*"9",0*/

    {0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x98,0x24,0x24,0x24,0x28,0xFC,0x00},/*"a",0*/
	{0x00,0x1F,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0xFC,0x88,0x04,0x04,0x04,0xF8,0x00},/*"b",0*/
	{0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x70,0x88,0x04,0x04,0x04,0x04,0x88,0x00},/*"c",0*/
	{0x00,0x00,0x01,0x01,0x01,0x00,0x1F,0x00,0x00,0xF8,0x04,0x04,0x04,0x88,0xFC,0x00},/*"d",0*/
	{0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x70,0xA8,0x24,0x24,0x24,0xA4,0x68,0x00},/*"e",0*/
	{0x00,0x01,0x01,0x0F,0x11,0x11,0x11,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00},/*"f",0*/
	{0x00,0x03,0x04,0x04,0x04,0x07,0x04,0x00,0x08,0xB4,0x54,0x54,0x54,0x94,0x08,0x00},/*"g",0*/
	{0x00,0x1F,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0xFC,0x80,0x00,0x00,0x00,0xFC,0x00},/*"h",0*/
	{0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00},/*"i",0*/
	{0x00,0x00,0x00,0x00,0x67,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0xF8,0x00,0x00,0x00},/*"j",0*/
	{0x00,0x1F,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xFC,0x20,0x40,0xB0,0x08,0x04,0x00},/*"k",0*/
	{0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00},/*"l",0*/
	{0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0xFC,0x00,0x00,0xFC,0x00,0x00,0xFC,0x00},/*"m",0*/
	{0x00,0x01,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0xFC,0x80,0x00,0x00,0x00,0xFC,0x00},/*"n",0*/
	{0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x70,0x88,0x04,0x04,0x04,0x88,0x70,0x00},/*"o",0*/
	{0x00,0x0F,0x04,0x08,0x08,0x08,0x07,0x00,0x00,0xFc,0x40,0x20,0x20,0x20,0xC0,0x00},/*"p",0*/
	{0x00,0x07,0x08,0x08,0x08,0x04,0x0F,0x00,0x00,0xC0,0x20,0x20,0x20,0x40,0xFc,0x00},/*"q",0*/
	{0x00,0x00,0x01,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0xFC,0x80,0x80,0x00,0x00,0x00},/*"r",0*/
	{0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xC8,0x24,0x24,0x24,0x24,0x98,0x00},/*"s",0*/
	{0x00,0x01,0x01,0x07,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0xF8,0x04,0x04,0x04,0x00},/*"t",0*/
	{0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0xF8,0x04,0x04,0x04,0x08,0xFC,0x00},/*"u",0*/
	{0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0xE0,0x18,0x04,0x18,0xE0,0x00,0x00},/*"v",0*/
	{0x01,0x00,0x00,0x01,0x00,0x00,0x01,0x00,0xF0,0x0C,0x70,0x80,0x70,0x0C,0xF0,0x00},/*"w",0*/
	{0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x04,0x88,0x50,0x20,0x50,0x88,0x04,0x00},/*"x",0*/
	{0x04,0x03,0x00,0x00,0x00,0x03,0x04,0x00,0x00,0x84,0x64,0x18,0x60,0x80,0x00,0x00},/*"y",0*/
	{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x0C,0x14,0x24,0x44,0x84,0x04,0x00},/*"z",0*/

    {0x00,0x00,0x07,0x18,0x18,0x07,0x00,0x00,0x0C,0xF0,0x20,0x20,0x20,0x20,0xF0,0x0C},/*"A",0*/
	{0x1F,0x10,0x10,0x10,0x10,0x09,0x06,0x00,0xFC,0x84,0x84,0x84,0x84,0x48,0x30,0x00},/*"B",0*/
	{0x07,0x08,0x10,0x10,0x10,0x08,0x06,0x00,0xF0,0x08,0x04,0x04,0x04,0x08,0x30,0x00},/*"C",0*/
	{0x1F,0x10,0x10,0x10,0x10,0x08,0x07,0x00,0xFC,0x04,0x04,0x04,0x04,0x08,0xF0,0x00},/*"D",0*/
	{0x00,0x1F,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0xFC,0x84,0x84,0x84,0x84,0x04,0x00},/*"E",0*/
	{0x1F,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0xFC,0x80,0x80,0x80,0x80,0x80,0x00,0x00},/*"F",0*/
	{0x07,0x08,0x10,0x10,0x10,0x08,0x06,0x00,0xF0,0x08,0x04,0x04,0x84,0x88,0xFC,0x00},/*"G",0*/
	{0x00,0x1F,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0xFC,0x80,0x80,0x80,0x80,0xFC,0x00},/*"H",0*/
	{0x00,0x00,0x10,0x1F,0x10,0x00,0x00,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,0x00},/*"I",0*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x38,0x04,0x04,0x04,0x04,0xF8,0x00},/*"J",0*/
	{0x00,0x1F,0x00,0x01,0x02,0x04,0x08,0x10,0x00,0xFC,0x80,0x00,0xC0,0x20,0x18,0x04},/*"K",0*/
	{0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0x00},/*"L",0*/
	{0x00,0x1F,0x07,0x00,0x00,0x00,0x07,0x1F,0x00,0xFC,0x00,0xF0,0x0C,0xF0,0x00,0xFC},/*"M",0*/
	{0x00,0x1F,0x0C,0x03,0x00,0x00,0x1F,0x00,0x00,0xFC,0x00,0x80,0x60,0x18,0xFC,0x00},/*"N",0*/
	{0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,0x00,0xF0,0x08,0x04,0x04,0x08,0xF0,0x00},/*"O",0*/
	{0x1F,0x10,0x10,0x10,0x10,0x08,0x07,0x00,0xFC,0x40,0x40,0x40,0x40,0x80,0x00,0x00},/*"P",0*/
	{0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,0x00,0xF0,0x08,0x04,0x34,0x08,0xF4,0x00},/*"Q",0*/
	{0x1F,0x10,0x10,0x10,0x10,0x09,0x06,0x00,0xFC,0x80,0x80,0x80,0xE0,0x18,0x04,0x00},/*"R",0*/
	{0x06,0x09,0x11,0x10,0x10,0x08,0x04,0x00,0x30,0x08,0x04,0x84,0x84,0x48,0x30,0x00},/*"S",0*/
	{0x10,0x10,0x10,0x1F,0x10,0x10,0x10,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00},/*"T",0*/
	{0x1F,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0xF0,0x08,0x04,0x04,0x04,0x08,0xF0,0x00},/*"U",0*/
	{0x18,0x07,0x00,0x00,0x00,0x07,0x18,0x00,0x00,0x80,0x70,0x0C,0x70,0x80,0x00,0x00},/*"V",0*/
	{0x1F,0x00,0x03,0x1C,0x03,0x00,0x1F,0x00,0xC0,0x3C,0xC0,0x00,0xC0,0x3C,0xC0,0x00},/*"W",0*/
	{0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x00,0x04,0x18,0x60,0x80,0x60,0x18,0x04,0x00},/*"X",0*/
	{0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00},/*"Y",0*/
	{0x10,0x10,0x10,0x10,0x13,0x14,0x18,0x00,0x0C,0x34,0x44,0x84,0x04,0x04,0x04,0x00},/*"Z",0*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
	{0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE4,0x00,0x00,0x00,0x00},/*"!",0*/
	{0x00,0x08,0x10,0x20,0x08,0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",0*/
	{0x02,0x03,0x1E,0x02,0x03,0x1E,0x02,0x00,0x20,0xFC,0x20,0x20,0xFC,0x20,0x20,0x00},/*"#",0*/
	{0x00,0x38,0x44,0xFF,0x42,0x41,0x30,0x00,0x00,0xE0,0x10,0xFC,0x10,0x10,0xE0,0x00},/*"$",0*/
	{0x7C,0x44,0x7C,0x07,0x19,0x62,0x03,0x00,0x00,0x10,0xE0,0x00,0xE0,0x10,0xE0,0x00},/*"%",0*/
	{0x00,0x0F,0x10,0x11,0x0E,0x00,0x00,0x00,0x78,0x84,0xC4,0x24,0x18,0x68,0x04,0x00},/*"&",0*/
	{0x00,0x03,0x0C,0x10,0x20,0x00,0x00,0x00,0x00,0xC0,0x30,0x08,0x04,0x00,0x00,0x00},/*"(",0*/
	{0x00,0x00,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x00,0x04,0x08,0x30,0xC0,0x00,0x00},/*")",0*/
	{0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x90,0x90,0x60,0xFC,0x60,0x90,0x90,0x00},/*"*",0*/
	{0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x20,0x20,0x20,0xFC,0x20,0x20,0x20,0x00},/*"+",0*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x18,0x00,0x00,0x00},/*",",0*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00},/*"-",0*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00},/*".",0*/
	{0x00,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00},/*"/",0*/
	{0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00},/*":",0*/
	{0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x24,0x38,0x00,0x00,0x00},/*";",0*/
	{0x00,0x01,0x02,0x04,0x08,0x10,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x00,0x00},/*"<",0*/
	{0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00},/*"=",0*/
	{0x00,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x04,0x08,0x10,0x20,0x40,0x80,0x00},/*">",0*/
	{0x00,0x0C,0x10,0x10,0x10,0x11,0x0E,0x00,0x00,0x00,0x00,0x64,0x80,0x00,0x00,0x00},/*"?",0*/
	{0x00,0x00,0x00,0x7F,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x04,0x04,0x00,0x00},/*"[",0*/
	{0x00,0x00,0x00,0x40,0x7F,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0xFC,0x00,0x00,0x00},/*"]",0*/
	{0x00,0x02,0x0C,0x30,0x30,0x0C,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",0*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x00,0x00},/*"_",0*/
	{0x00,0x00,0x01,0x7E,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x04,0x00,0x00,0x00},/*"{",0*/
	{0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00},/*"|",0*/
	{0x00,0x00,0x00,0x40,0x7E,0x01,0x00,0x00,0x00,0x00,0x00,0x04,0xFC,0x00,0x00,0x00},/*"}",0*/
	{0x00,0x06,0x08,0x08,0x06,0x02,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"~",0*/
};
void Lcm_Disp_8_16(u8 x_start,u8 y_start,u8 *data,u8 data_size)
{
	u8 *p_disp_buf;
	u8 curr_disp_page=0;
	if(((8*data_size)+y_start) >132)
	{//y坐标越界 退出
		return;
	}
	for(u8 col=x_start;col<(x_start+2);col++)
	{//页数
		curr_disp_page=(col-x_start);
		Lcm_Start();
		Lcm_Write(0x70); // set slave address
		Lcm_Write(0xb3-col);  //set page address
		Lcm_Write(0x10+(4>>4));//set column address
		Lcm_Write(0x00+(4&0x0f));//
		Lcm_Stop();
		Lcm_Start();
		Lcm_Write(0x72); // set slave address
        for(u8 i=0;i<y_start;i++)
        {
            Lcm_Write(0x00);
        }
		for(u8 y=0;y<data_size;y++)
		{
            if(data[y]>='a'&&data[y]<='z')
            {
                p_disp_buf=Digt_8_16buf[(data[y]-'a')+10];
            }
            else if(data[y]>='A'&&data[y]<='Z')
            {
                p_disp_buf=Digt_8_16buf[(data[y]-'A')+36];
            }
			else if(data[y]>=0&&data[y]<=9)
			{
				p_disp_buf=Digt_8_16buf[data[y]];
			}
			else if(data[y]>='0'&&data[y]<='9')
			{
				p_disp_buf=Digt_8_16buf[data[y]-'0'];
			}
            else
            {
                p_disp_buf=Digt_8_16buf[62];
                for(u8 i = 0;i < 29; i++)
                {
                    if(data[y] == spec_char_Index[i])
                    {
                        p_disp_buf=Digt_8_16buf[i+62];
                        break;
                    }
                }
            }
            for(u8 y_8bit=0;y_8bit<8;y_8bit++)
            {
                switch(curr_disp_page)
                {
					case 0:
                        Lcm_Write(p_disp_buf[y_8bit]);
						break;
					case 1:
                        Lcm_Write(p_disp_buf[y_8bit+8]);
						break;
                }
			}
		}
        for(u8 i=(y_start+data_size*8);i<128;i++)
        {
            Lcm_Write(0x00);
        }
		Lcm_Stop();
	}
}
void Lcm_Disp_8_16_jump(u8 x_start,u8 y_start,u8 *data,u8 data_size)
{
	u8 *p_disp_buf;
	u8 curr_disp_page=0;
	if(((8*data_size)+y_start) >132)
	{//y坐标越界 退出
		return;
	}
	for(u8 col=x_start;col<(x_start+2);col++)
	{//页数
		curr_disp_page=(col-x_start);
		Lcm_Start();
		Lcm_Write(0x70); // set slave address
		Lcm_Write(0xb3-col);  //set page address
		Lcm_Write(0x10+((y_start+4)>>4));//set column address
		Lcm_Write(0x00+((y_start+4)&0x0f));//
		Lcm_Stop();
		Lcm_Start();
		Lcm_Write(0x72); // set slave address
		for(u8 y=0;y<data_size;y++)
		{
            if(data[y]>='a'&&data[y]<='z')
            {
                p_disp_buf=Digt_8_16buf[(data[y]-'a')+10];
            }
            else if(data[y]>='A'&&data[y]<='Z')
            {
                p_disp_buf=Digt_8_16buf[(data[y]-'A')+36];
            }
			else if(data[y]>=0&&data[y]<=9)
			{
				p_disp_buf=Digt_8_16buf[data[y]];
			}
			else if(data[y]>='0'&&data[y]<='9')
			{
				p_disp_buf=Digt_8_16buf[data[y]-'0'];
			}
            else
            {
                p_disp_buf=Digt_8_16buf[62];
                for(u8 i = 0;i < 29; i++)
                {
                    if(data[y] == spec_char_Index[i])
                    {
                        p_disp_buf=Digt_8_16buf[i+62];
                        break;
                    }
                }
            }
            for(u8 y_8bit=0;y_8bit<8;y_8bit++)
            {
                switch(curr_disp_page)
                {
					case 0:
                        Lcm_Write(p_disp_buf[y_8bit]|0x40);
						break;
					case 1:
                        Lcm_Write(p_disp_buf[y_8bit+8]|0x01);
						break;
                }
			}
		}
        for(u8 i=(y_start+data_size*8);i<128;i++)
        {
            switch(curr_disp_page)
            {
                case 0:
                    Lcm_Write(0x40);
                    break;
                case 1:
                    Lcm_Write(0x01);
                    break;
            }
        }
		Lcm_Stop();
	}
}
void lcm_disp_radio(void)
{
    Lcm_Disp_8_16(0,0,Disp_dab_buf,16);
    Lcm_Disp_8_16(2,0,&Disp_dab_buf[16],16);
}
#endif
