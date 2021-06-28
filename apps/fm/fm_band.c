#include "sdk_cfg.h"
#include "setup.h"
#include "fm_radio.h"
#include "fm_engine.h"
#include "fm_band.h"

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);

radio_ctrl_t *radio_ctrl = NULL;
radio_config_t *radio_config = NULL;
#if defined(_ENABLE_RADIO_RDS_)
radio_rds_t *radio_rds = NULL;
#endif

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

#define AREA_DEFAULT_FREQ(x)		(sizeof(x)/sizeof(uint16)),((uint16 *)x)

#if defined(_SUPPORT_FREQ_JAPAN_)
#if defined(_SPECIAL_FREQ_JAPAN_)
static const uint16 AreaFmEurDefaultFreq[]={7600,8750,9000,9800,10600,10800};
static const radio_area_info_t AreaFmEur=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_FM ,
	7600,
	10800,
    10,
	10,
	AREA_DEFAULT_FREQ(AreaFmEurDefaultFreq),
};
static const uint16 AreaAmEurDefaultFreq[]={522,603,999,1404,1620,522};
static const radio_area_info_t AreaAmEur=
{
	RADIO_INFO_STATE_AM ,
	522,
	1620,
	9,
	9,
	AREA_DEFAULT_FREQ(AreaAmEurDefaultFreq),
};
#else
static const uint16 AreaFmEurDefaultFreq[]={7600,7860,8300,8660,9000,7600};
static const radio_area_info_t AreaFmEur=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_FM ,
	7600,
	9000,
    10,
	10,
	AREA_DEFAULT_FREQ(AreaFmEurDefaultFreq),
};
static const uint16 AreaAmEurDefaultFreq[]={522,603,999,1404,1629,522};
static const radio_area_info_t AreaAmEur=
{
	RADIO_INFO_STATE_AM ,
	522,
	1629,
	9,
	9,
	AREA_DEFAULT_FREQ(AreaAmEurDefaultFreq),
};
#endif

#elif defined(_WLS1403_SPECIAL_FREQ_)
static const uint16 AreaFmEurDefaultFreq[]={7550,9000,9800,10600,10800,7550};
static const radio_area_info_t AreaFmEur=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_FM ,
	7550,
	10800,
    10,
	10,
	AREA_DEFAULT_FREQ(AreaFmEurDefaultFreq),
};

static const uint16 AreaAmEurDefaultFreq[]={522,603,999,1404,1620,522};
static const radio_area_info_t AreaAmEur=
{
	RADIO_INFO_STATE_AM ,
	522,
	1620,
	9,
	9,
	AREA_DEFAULT_FREQ(AreaAmEurDefaultFreq),
};

#elif defined(_HT5251_SPECIAL_FREQ_)
static const uint16 AreaFmEurDefaultFreq[]={8750,9010,9810,10610,10800,8750};
static const radio_area_info_t AreaFmEur=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
    10,
	10,
	AREA_DEFAULT_FREQ(AreaFmEurDefaultFreq),
};

static const uint16 AreaAmEurDefaultFreq[]={522,603,999,1404,1710,522};
static const radio_area_info_t AreaAmEur=
{
	RADIO_INFO_STATE_AM ,
	522,
	1710,
	9,
	9,
	AREA_DEFAULT_FREQ(AreaAmEurDefaultFreq),
};
#elif defined(_MIDDLE_EAST_FREQ_)
static const uint16 AreaFmEurDefaultFreq[]={8750,9000,9800,10600,10800,8750};
static const radio_area_info_t AreaFmEur=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmEurDefaultFreq),
};

static const uint16 AreaAmEurDefaultFreq[]={531,603,999,1404,1602,531};
static const radio_area_info_t AreaAmEur=
{
	RADIO_INFO_STATE_AM ,
	531,
	1602,
	9,
	9,
	AREA_DEFAULT_FREQ(AreaAmEurDefaultFreq),
};

#else
#if defined(_XM003_SPECIAL_FREQ_)||defined(_KLD3300_FREQ_AUSTRALIA_)
static const uint16 AreaFmEurDefaultFreq[]={8750,9010,9810,10610,10800,8750};
#elif defined(_SPECIAL_NUMBER_SAVE_)
static const uint16 AreaFmEurDefaultFreq[]={8750,9000,9800,10600,10800,8750,8750,8750,8750,8750};
#elif defined(_WLGK3001_SPECIAL_FREQ_)
static const uint16 AreaFmEurDefaultFreq[]={8750,9000,9800,10600,10800,10800};
#else
static const uint16 AreaFmEurDefaultFreq[]={8750,9000,9800,10600,10800,8750};
#endif
static const radio_area_info_t AreaFmEur=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
#if defined(TD_ADD_LCD_NUM_4)||defined(_EUR_STEP_100K_)||defined(_LCD_3_DIAPLAY_)
#if defined(_LED_EUR_STEP_50K_)||defined(_FEIYIN_LED_EUR_STEP_)
	5,
#else
	10,
#endif
#else
	5,
#endif
#if defined(_EUR_STEP_50K_)||defined(_LED_EUR_STEP_50K_)
	5,
#else
	10,
#endif
	AREA_DEFAULT_FREQ(AreaFmEurDefaultFreq),
};
static const uint16 AreaAmEurDefaultFreq[]={522,603,999,1404,1620,522};
static const radio_area_info_t AreaAmEur=
{
	RADIO_INFO_STATE_AM ,
	522,
#if defined(_YS_SEL_SETTING_)
	1629,
#else
	1620,
#endif
	9,
	9,
	AREA_DEFAULT_FREQ(AreaAmEurDefaultFreq),
};
#endif
/*---俄罗斯频率--------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_ORIT_ON_)
static const uint16 AreaFmOirtDefaultFreq[]={6500,6710,7040,7250,7400,6500};
static const radio_area_info_t AreaFmOirt=
{
	RADIO_INFO_STATE_FM,
	6500,
	7400,
	5,		//	50k
	10,		//	100k
	AREA_DEFAULT_FREQ(AreaFmOirtDefaultFreq),
};
#endif

/*-----------------------------------------------------------------------------------------------------------*/
#if defined(HYJ_BANDSET)
static const uint16 AreaFmUsaDefaultFreq[]={8750,9010,9810,10610,10790,8750};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8750,
	10790,
    10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={530,600,1000,1400,1620,530};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	530,
	1620,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(JYL_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={7600,8750,9000,9800,10600,10800};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	7600,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};
	
static const uint16 AreaAmUsaDefaultFreq[]={520,600,1000,1400,1710,520};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	520,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(JYL3801E_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={8000,8750,9000,9800,10600,10800};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8000,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};
	
static const uint16 AreaAmUsaDefaultFreq[]={520,600,1000,1400,1710,520};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	520,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(JYL3802E_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={7610,8750,9010,9810,10610,10790};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	7610,
	10790,
	20,
	20,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};
	
static const uint16 AreaAmUsaDefaultFreq[]={520,600,1000,1400,1710,520};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	520,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(_LK_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={8750,9010,9810,10610,10800,8750};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={530,600,1000,1400,1710,530};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	530,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(RADIO_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={8750,9000,9800,10600,10800,8750};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={520,600,1000,1400,1710,520};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	520,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(_GD_RADIO_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={8750,9000,9800,10600,10800,8750};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};
#if defined(_CHANGE_AM_FREQ_)
static const uint16 AreaAmUsaDefaultFreq[]={520,630,1000,1440,1620,520};
#else
static const uint16 AreaAmUsaDefaultFreq[]={520,603,999,1404,1620,520};
#endif
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	520,
	1620,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(_EVB_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={8750,9000,9800,10600,10800,8750};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={530,600,1000,1400,1710,530};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	530,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(_WJ361_FREQ_AUSTRALIA_)
static const uint16 AreaFmUsaDefaultFreq[]={8750,9000,9800,10600,10800,8750};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={522,603,999,1404,1710,522};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	522,
	1710,
	9,
	9,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(_LK_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={8750,9010,9810,10610,10800,8750};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8750,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={530,600,1000,1400,1710,530};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	530,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(JTI_FREQ_LA_BRAZIL_)
static const uint16 AreaFmUsaDefaultFreq[]={7610,9010,9810,10610,10790,7610};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	7610,
	10790,
	20,
	20,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={530,600,1000,1400,1710,530};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	530,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};

#elif defined(_ZW5249_SPECIAL_FREQ_)
static const uint16 AreaFmUsaDefaultFreq[]={8770,9010,9810,10610,10790,8770};
static const radio_area_info_t AreaFmUsa=
{
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
	8770,
	10790,
	20,
	20,
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={530,600,1000,1400,1710,530};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	530,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};
#else

#if defined(_SPECIAL_NUMBER_SAVE_)
#if defined(_ENABLE_AUS_FREQ_)
static const uint16 AreaFmUsaDefaultFreq[]={8750,9000,9800,10600,10800,8750,8750,8750,8750,8750};
#else
static const uint16 AreaFmUsaDefaultFreq[]={8750,9010,9810,10610,10790,8750,8750,8750,8750,8750};
#endif
#else
static const uint16 AreaFmUsaDefaultFreq[]={8750,9010,9810,10610,10790,8750};
#endif
static const radio_area_info_t AreaFmUsa=
{
#if defined(_DISABLE_RDS_FUNTION_)
	RADIO_INFO_STATE_FM ,
#else
	RADIO_INFO_STATE_SUPPORT_RDS | RADIO_INFO_STATE_SUPPORT_RBDS | RADIO_INFO_STATE_FM ,
#endif
	8750,
#if defined(_ENABLE_AUS_FREQ_)
	10800,
#else
	10790,
#endif
#if defined(_USA_STEP_200K_)
	20,
#else
	10,
#endif

#if defined(_USA_STEP_100K_)
	10,
#else
	20,
#endif
	AREA_DEFAULT_FREQ(AreaFmUsaDefaultFreq),
};

static const uint16 AreaAmUsaDefaultFreq[]={530,600,1000,1400,1710,530};
static const radio_area_info_t AreaAmUsa=
{
	RADIO_INFO_STATE_AM ,
	530,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmUsaDefaultFreq),
};
#endif

#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
static const uint16 AreaFmLatinDefaultFreq[]={8750,9000,9800,10600,10800,8750};
static const radio_area_info_t AreaFmLatin=
{
	RADIO_INFO_STATE_FM ,
	8750,
	10800,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaFmLatinDefaultFreq),
};
#endif

#if defined(_SUPPORT_FOUR_AREA_)
static const uint16 AreaAmLatinDefaultFreq[]={520,600,1000,1400,1710,520};
static const radio_area_info_t AreaAmLatin=
{
	RADIO_INFO_STATE_AM ,
	520,
	1710,
	10,
	10,
	AREA_DEFAULT_FREQ(AreaAmLatinDefaultFreq),
};

static const uint16 AreaAmAsiaDefaultFreq[]={531,603,999,1404,1629,531};
static const radio_area_info_t AreaAmAsia=
{
	RADIO_INFO_STATE_AM ,
	531,
	1629,
	9,
	9,
	AREA_DEFAULT_FREQ(AreaAmAsiaDefaultFreq),
};
#endif

#if defined(_ENABLE_RADIO_WB_)
//static const uint16 AreaWbDefaultFreq[]={162400,162425,162450,162475,162500,162525,162550};
static const uint16 AreaWbDefaultFreq[]={64960,64970,64980,64990,65000,65010,65020};//实际显示频率65020*25/10
static const radio_area_info_t AreaWb=
{
	RADIO_INFO_STATE_WB,
	64960,
	65020,
	1,
	1,
	AREA_DEFAULT_FREQ(AreaWbDefaultFreq),
};
#endif

static const radio_area_info_t *raido_area_info_tab[]=
{
	&AreaFmEur,
	&AreaAmEur,
	&AreaFmUsa,
	&AreaAmUsa,
#if defined(_SUPPORT_ORIT_ON_)
	&AreaFmOirt,
#endif
#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
	&AreaFmLatin,
#endif
#if defined(_SUPPORT_FOUR_AREA_)
	&AreaAmLatin,
	&AreaAmAsia,
#endif
#if defined(_ENABLE_RADIO_WB_)
	&AreaWb,
#endif
};

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

static radio_area_info_t const *GetIndexRadioInfo(radio_area_e Area)
{
	Area %= AREA_MAX;
	return raido_area_info_tab[Area];
}

uint8 GetIndexBandState(radio_area_e Area)
{
	return GetIndexRadioInfo(Area)->State;
}

uint16 GetIndexBandMinFreq(radio_area_e Area)
{
	return GetIndexRadioInfo(Area)->MinFreq;
}

uint16 GetIndexBandMaxFreq(radio_area_e Area)
{
	return GetIndexRadioInfo(Area)->MaxFreq;
}

uint8 GetIndexBandAutoStep(radio_area_e Area)
{
	return GetIndexRadioInfo(Area)->AutoStep;
}

uint8 GetIndexBandManualStep(radio_area_e Area)
{
	return GetIndexRadioInfo(Area)->ManualStep;
}

uint8 GetIndexBandPosLength(radio_area_e Area)
{
	return GetIndexRadioInfo(Area)->DefaultLength;
}

uint16 GetIndexBandPosFreq(radio_area_e Area,uint8 Pos)
{
	uint8 Len;
	Len = GetIndexRadioInfo(Area)->DefaultLength;
	Pos%=Len;
	return GetIndexRadioInfo(Area)->DefaultFreq[Pos];
}

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_SUPPORT_AREA_)
#if defined(RADIO_AREA_EUR_LIST)
static const uint8 radio_area_eur_tab[]=
{
	RADIO_AREA_EUR_LIST
};
#endif
#if defined(RADIO_AREA_USA_LIST)
static const uint8 radio_area_usa_tab[]=
{
	RADIO_AREA_USA_LIST
};
#endif
#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
static const uint8 radio_area_latin_tab[]=
{
	RADIO_AREA_LATIN_LIST
};
#endif
#if defined(_SUPPORT_FOUR_AREA_)
static const uint8 radio_area_asia_tab[]=
{
	RADIO_AREA_ASIA_LIST
};
#endif
#endif

static uint8 UserAddBand(radio_area_e Area)
{
	uint8 Index;
	uint8 Enter;

	if((Area >= AREA_MAX) && (radio_config->BandArea.Length >= sizeof(radio_config->BandArea.Band)) )return FALSE;

	for(Index = 0;Index < radio_config->BandArea.Length;Index ++)
	{
		if(radio_config->BandArea.Band[Index] == Area)
		{
			break;
		}
	}

	if(Index == radio_config->BandArea.Length)
	{
		radio_config->BandArea.Band[radio_config->BandArea.Length] = Area;
	}
	else
	{
		Enter = Index;
		for(Index = radio_config->BandArea.Length;Index > Enter;Index --)
		{
			radio_config->BandArea.Band[Index] = radio_config->BandArea.Band[Index-1];
		}
		radio_config->BandArea.Band[Index] = Area;
	}
	radio_config->BandArea.Length ++;
	return TRUE;
}

static uint8 UserDelBand(radio_area_e Area)
{
	uint8 Index;

	if(!radio_config->BandArea.Length)return FALSE;
	for(Index = 0;Index < radio_config->BandArea.Length;Index ++)
	{
		if(radio_config->BandArea.Band[Index] == Area)
		{
			break;
		}
	}
	if(Index == radio_config->BandArea.Length)return FALSE;

	for(;Index < (radio_config->BandArea.Length-1);Index ++)
	{
		radio_config->BandArea.Band[Index] = radio_config->BandArea.Band[Index+1];
	}
	radio_config->BandArea.Band[Index] = 0;
	radio_config->BandArea.Length --;
	return TRUE;
}

void RadioInitBand(void)
{
	uint8 index;

	memset(&radio_config->BandArea, 0, sizeof(radio_config->BandArea));

#if defined(_SUPPORT_THREE_AREA_)||defined(_SUPPORT_FOUR_AREA_)
    if(radio_config->radio_info.area == SETUP_AREA_USA)
    {
    	for(index = 0; index < sizeof(radio_area_usa_tab); index++)
    	{
    		UserAddBand(radio_area_usa_tab[index]);
    	}
    }
    else if(radio_config->radio_info.area == SETUP_AREA_EUR)
    {
    	for(index = 0; index < sizeof(radio_area_eur_tab); index++)
    	{
    		UserAddBand(radio_area_eur_tab[index]);
    	}
    }
    else if(radio_config->radio_info.area == SETUP_AREA_LATIN)
    {
    	for(index = 0; index < sizeof(radio_area_latin_tab); index++)
    	{
    		UserAddBand(radio_area_latin_tab[index]);
    	}
    }
#if defined(_SUPPORT_FOUR_AREA_)
    else if(radio_config->radio_info.area == SETUP_AREA_ASIA)
    {
    	for(index = 0; index < sizeof(radio_area_asia_tab); index++)
    	{
    		UserAddBand(radio_area_asia_tab[index]);
    	}
    }
#endif
#else
	if(radio_config->radio_info.area == SETUP_AREA_USA)
	{
		for(index = 0; index < sizeof(radio_area_usa_tab); index++)
		{
			UserAddBand(radio_area_usa_tab[index]);
		}
	}
	else
	{
		for(index = 0; index < sizeof(radio_area_eur_tab); index++)
		{
			UserAddBand(radio_area_eur_tab[index]);
		}
	}
#endif

	SET_RADIO_BAND_LENGTH(USER_BAND_LENGTH);
	SET_RADIO_FREQ_LENGTH(RADIO_POS_LENGTH);
}

void RadioInitFreq(void)
{
	uint8 Band;
	uint8 Freq;
	uint8 FreqLength;

	for(Band = 0; Band < GET_RADIO_BAND_LENGTH; Band++)
	{
		FreqLength = INDEX_RADIO_POS_LENGTH(Band);
		SET_RADIO_TEMP_FREQ(Band,INDEX_RADIO_POS_FREQ(Band,0));
		SET_RADIO_FREQ_INDEX(Band,1);
		for(Freq = 0;Freq < FreqLength;Freq++)
		{
			SET_RADIO_SAVE_FREQ(Band,Freq,INDEX_RADIO_POS_FREQ(Band,Freq));
		}
	}
	
#if defined(_HN6651D_SPECIAL_FREQ_)||defined(_XM003_SPECIAL_FREQ_)
	for(Band = 1; Band < 3; Band++)
	{
		for(Freq = 0;Freq < FreqLength;Freq++)
		{
			SET_RADIO_SAVE_FREQ(Band,Freq,8750);
		}
	}
#endif
}

void RadioInitBandFreq(void)
{
#if !defined(_SPECIAL_BROWSE_FUNCTION)
	uint8 Band;
	uint8 NowBandInfo;
#endif
#if !defined(_XM003_SPECIAL_FREQ_)
	uint8 Freq;
#endif
	uint8 FreqLength;

#if defined(_SPECIAL_BROWSE_FUNCTION)
	SET_RADIO_TEMP_FREQ(GET_RADIO_BAND,INDEX_RADIO_POS_FREQ(GET_RADIO_BAND,0));
	SET_RADIO_FREQ_INDEX(GET_RADIO_BAND,1);
	FreqLength = INDEX_RADIO_POS_LENGTH(GET_RADIO_BAND);
	for(Freq = 0;Freq < FreqLength;Freq++)
	{
		SET_RADIO_SAVE_FREQ(GET_RADIO_BAND,Freq,INDEX_RADIO_POS_FREQ(GET_RADIO_BAND,Freq));
	}
#else
	NowBandInfo = USER_BAND_INFO;
	for(Band = 0;Band < GET_RADIO_BAND_LENGTH;Band++)
	{
		if(NowBandInfo == USER_BAND_AREA_INFO(Band))
		{
			SET_RADIO_TEMP_FREQ(Band,INDEX_RADIO_POS_FREQ(Band,0));
			SET_RADIO_FREQ_INDEX(Band,1);
			FreqLength = INDEX_RADIO_POS_LENGTH(Band);
		#if !defined(_XM003_SPECIAL_FREQ_)//第二次AMS扫描中断不要清原来已经存好的电台
			for(Freq = 0;Freq < FreqLength;Freq++)
			{
				SET_RADIO_SAVE_FREQ(Band,Freq,INDEX_RADIO_POS_FREQ(Band,Freq));
			}
		#endif
		}
	}
#endif
}

uint8 SearchFristBand(void)
{
	uint8 Band;
	uint8 NowBandInfo;

	NowBandInfo = USER_BAND_INFO;

	for(Band = 0;Band < GET_RADIO_BAND_LENGTH;Band++)
	{
		if(NowBandInfo == USER_BAND_AREA_INFO(Band))
		{
			break;
		}
	}
	return Band;
}

uint8 SearchNextBand(uint8 Band)
{
	uint8 IndexBand;
	uint8 NextBand;
	uint8 NowBandInfo;

	NowBandInfo = USER_BAND_AREA_INFO(Band);
	NextBand = Band;

	for(IndexBand = 0;IndexBand < GET_RADIO_BAND_LENGTH;IndexBand++)
	{
		if(NextBand < GET_RADIO_BAND_LENGTH-1)NextBand++;
		else NextBand = 0;

		if(NowBandInfo == USER_BAND_AREA_INFO(NextBand))
		{
			break;
		}
	}

	return NextBand;
}

uint8 SearchPrevBand(uint8 Band)
{
	uint8 IndexBand;
	uint8 PrevBand;
	uint8 NowBandInfo;

	NowBandInfo = USER_BAND_AREA_INFO(Band);
	PrevBand = Band;

	for(IndexBand = 0;IndexBand < GET_RADIO_BAND_LENGTH;IndexBand++)
	{
		if(PrevBand)PrevBand--;
		else PrevBand = GET_RADIO_BAND_LENGTH-1;

		if(NowBandInfo == USER_BAND_AREA_INFO(PrevBand))
		{
			break;
		}
	}
	return PrevBand;
}

void BandIndexReset(void)
{
	uint8 Band;
	uint8 NowBandInfo;

	NowBandInfo = USER_BAND_INFO;
	for(Band = 0;Band < GET_RADIO_BAND_LENGTH;Band++)
	{
		if(NowBandInfo == USER_BAND_AREA_INFO(Band))
		{
			SET_RADIO_FREQ_INDEX(Band,1);
		}
	}
}

uint8 GetBandNo(uint8 Band)
{
	uint8 BandNo;
	uint8 Index;

	BandNo = 0;

	if(INDEX_RADIO_STATE_AM(Band))
	{//	AM
		for(Index = 0;Index <= Band;Index ++)
		{
			if(INDEX_RADIO_STATE_AM(Index))
			{
				BandNo++;
			}
		}
	}
	else if(INDEX_RADIO_STATE_FM(Band))
	{//	FM
		for(Index = 0;Index <= Band;Index ++)
		{
			if(INDEX_RADIO_STATE_FM(Index))
			{
				BandNo++;
			}
		}
	}
	else if(INDEX_RADIO_STATE_SW(Band))
	{//	SW
		for(Index = 0;Index <= Band;Index ++)
		{
			if(INDEX_RADIO_STATE_SW(Index))
			{
				BandNo++;
			}
		}
	}
	else if(INDEX_RADIO_STATE_LW(Band))
	{//	LW
		for(Index = 0;Index <= Band;Index ++)
		{
			if(INDEX_RADIO_STATE_LW(Index))
			{
				BandNo++;
			}
		}
	}
	else if(INDEX_RADIO_STATE_WB(Band))
	{//	WB
		for(Index = 0;Index <= Band;Index ++)
		{
			if(INDEX_RADIO_STATE_WB(Index))
			{
				BandNo++;
			}
		}
	}
	return BandNo;
}

uint8 SearchFreqNo(uint16 Freq)
{
	uint8 	Band;
	uint8	NowBandInfo;
	uint8 	Length;
	uint8 	Index;

	NowBandInfo = USER_BAND_INFO;

	for(Band = 0;Band < GET_RADIO_BAND_LENGTH;Band++)
	{
		if(NowBandInfo == USER_BAND_AREA_INFO(Band))
		{
			Length = GET_RADIO_FREQ_INDEX(Band);
			for(Index = 0;Index < Length;Index++)
			{
				if(Freq == GET_RADIO_SAVE_FREQ(Band,Index))
					return TRUE;
			}
		}
	}

	return FALSE;
}

void RadioBandTotal(uint8 *Fm,uint8 *Am)
{
	uint8 Length;
	uint8 Band;
	uint8 FmTotal;
	uint8 AmTotal;
	Length = USER_BAND_LENGTH;
	FmTotal = AmTotal = 0;

	for(Band = 0; Band < Length; Band ++)
	{
		if(INDEX_RADIO_STATE_FM(Band))FmTotal++;
		else AmTotal++;
	}
	*Fm = FmTotal;
	*Am = AmTotal;
}


void RadioBandStart(uint8 *Fm,uint8 *Am)
{
	uint8 Length;
	uint8 Band;
	uint8 FmStart;
	uint8 AmStart;
	Length = USER_BAND_LENGTH;
	FmStart = AmStart = -1;

	for(Band = 0; Band < Length; Band ++)
	{
		if(INDEX_RADIO_STATE_FM(Band))
		{
			FmStart = Band;
			break;
		}
	}

	for(Band = 0; Band < Length; Band ++)
	{
		if(INDEX_RADIO_STATE_AM(Band))
		{
			AmStart = Band;
			break;
		}
	}

	*Fm = FmStart;
	*Am = AmStart;
}

#if defined(_ENABLE_RADIO_RDS_)
uint8 radio_rds_total_freq(void)
{
	uint8 band;
	uint8 index;
	uint8 total = 0;
	uint8 now_band_info;

	now_band_info = USER_BAND_INFO;
	
	for(band = 0; band <= GET_RADIO_BAND_LENGTH; band ++)
	{
		if(now_band_info == USER_BAND_AREA_INFO(band))
		{
			for(index = 1; index <= INDEX_RADIO_POS_LENGTH(band); index++)
			{
				if(GET_RADIO_RDS_INFO(band, index).pi)
				{
					total++;
				}
			}
		}
	}
	
	return total;
}

uint16 *radio_rds_index_freq(uint8 pos)
{
	uint8 band;
	uint8 index;
	uint8 now_band_info;
	uint8 freq_length;
	uint8 count = 0;
	
	now_band_info = USER_BAND_INFO;
	
	for(band = 0;band < GET_RADIO_BAND_LENGTH; band ++)
	{
		if(now_band_info == USER_BAND_AREA_INFO(band))
		{
			freq_length = INDEX_RADIO_POS_LENGTH(band);
			for(index = 0; index < freq_length; index++)
			{
				if(pos == count)
				{
					return &GET_RADIO_SAVE_FREQ(band,index);
				}
				count++;
			}
		}
	}
	
	return NULL;
}

void radio_rds_taxis_freq(void)
{
	uint8 total;
	uint8 i,j;
	uint16 *temp1,*temp2;
	uint16 temp;
	
	total = radio_rds_total_freq();
	
	if(total > 1)
	{
		for(i = 1; i < total; i ++)
		{
			for(j = 0; j < (total - i); j++)
			{
				temp1 = radio_rds_index_freq(j);
				temp2 = radio_rds_index_freq(j + 1);
				
				if((*temp1)>(*temp2))
				{
					temp 	 = (*temp1);
					(*temp1) = (*temp2);
					(*temp2) = temp;
				}
			}
		}
	}
}
#endif

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/

uint8 radio_engine_init(void)
{
	fm_puts("radio_engine_init\n");

	radio_band_e band;
	uint8 result = FALSE;
	engine_init_t *engine_init = NULL;
	engine_init = (engine_init_t *)malloc_fun(engine_init ,sizeof(engine_init_t),0);
	if(engine_init == NULL)return result;

	if(RADIO_STATE_FM)
	{
		band = RADIO_BAND_FM;
	}
	else if(RADIO_STATE_AM)
	{
		band = RADIO_BAND_AM;
	}
#if defined(_ENABLE_RADIO_WB_)
	else if(RADIO_STATE_WB)
	{
		band = RADIO_BAND_WB;
	}
#endif
	else
	{
		free_fun((void**)&engine_init);
		return FALSE;
	}

	//当前频点
	engine_init->freq = GET_BAND_TEMP_FREQ;
	//波段
	engine_init->band = band;
	//最小频率
	engine_init->min_freq = RADIO_MIN_FREQ;
	//最大频率
	engine_init->max_freq = RADIO_MAX_FREQ;

	result = fm_engine_msg(MSG_FMENGINE_INIT, engine_init, NULL);

	free_fun((void**)&engine_init);

	return result;
}

uint8 radio_engine_return(void *func)
{
	fm_printf("%s, func:0x%x\n", __FUNCTION__, func);//func->radio_engine_return
	
	return fm_engine_msg(MSG_FMENGINE_RETURN, func, NULL);
}

uint8 radio_engine_freq(uint16 freq)
{
	fm_printf("%s, freq:%d\n", __FUNCTION__, freq);
#if defined(_ADD_SPECIAL_CH_DISP_)	
	global.fm_disp = FALSE;
#endif
	return fm_engine_msg(MSG_FMENGINE_SETFREQ, (void *)(int)freq, NULL);
}

uint8 radio_engine_softseek(uint16 freq)
{
	fm_printf("%s, freq:%d\n", __FUNCTION__, freq);
	
	if(fm_engine_msg(MSG_FMENGINE_SET_SOFTSEEK, (void *)(int)freq, &radio_ctrl->eng_status))
	{
		return radio_ctrl->eng_status.valid;
	}
	
	return FALSE;
}

uint8 radio_engine_stereo(uint8 stereo)
{
	fm_printf("%s, stereo:%d\n", __FUNCTION__, stereo);
	
	return fm_engine_msg(MSG_FMENGINE_SET_STEREO, (void *)(int)stereo, NULL);
}

uint8 radio_engine_mute(uint8 mute)
{
	//fm_puts("radio_engine_mute\n");
	if(radio_ctrl->eng_mute != mute)
	{
		fm_printf("%s, mute:%d\n", __FUNCTION__, mute);
		radio_ctrl->eng_mute = mute;
		return fm_engine_msg(MSG_FMENGINE_SETMUTE, (void *)(int)mute, NULL);
	}
	return TRUE;
}

uint8 radio_engine_auto_seek(uint16 min_freq, uint16 max_freq, uint8 step, uint8 mode)
{
	fm_printf("%s, min_freq:%d, max_freq:%d, step:%d, mode:%d\n", __FUNCTION__, min_freq, max_freq, step, mode);
	
	radio_ctrl->auto_seek.min_freq = min_freq;
	radio_ctrl->auto_seek.max_freq = max_freq;
	radio_ctrl->auto_seek.step 	= step;
	radio_ctrl->auto_seek.mode 	= mode;
	
	return fm_engine_msg(MSG_FMENGINE_AUTO_SEEK, &radio_ctrl->auto_seek, NULL);
}

uint8 radio_engine_auto_break(void)
{
	fm_puts("radio_engine_auto_break\n");

	return fm_engine_msg(MSG_FMENGINE_AUTO_BREAK, NULL, &radio_ctrl->eng_status);
}

uint8 radio_engine_get_status(void)
{
	//fm_puts("radio_engine_get_status\n");

	return fm_engine_msg(MSG_FMENGINE_GET_STATUS, NULL, &radio_ctrl->eng_status);
}

uint8 radio_engine_get_stereo(void)
{
	//fm_puts("radio_engine_get_stereo\n");
	
	return fm_engine_msg(MSG_FMENGINE_GET_STEREO, NULL, &radio_ctrl->eng_status.stereo);
}

uint8 radio_engine_get_fm_only(void)
{
	fm_puts("radio_engine_get_fm_only\n");

	return fm_engine_msg(MSG_FMENGINE_GET_FM_ONLY, NULL, &radio_ctrl->fm_only);
}

#if defined(_ENABLE_RADIO_RDS_)

uint8 radio_engine_rds_state(void)
{
	fm_puts("radio_engine_rds_state\n");
	
	return fm_engine_msg(MSG_FMENGINE_RDS_STATE, NULL, &radio_rds->enable);
}

uint8 radio_engine_rds_enable(uint8 enable)
{
	fm_puts("radio_engine_rds_enable\n");

	return fm_engine_msg(MSG_FMENGINE_RDS_ENABLE, (void *)(int)enable, NULL);
}

uint8 radio_engine_rds_front(uint8 front)
{
	fm_puts("radio_engine_rds_front\n");

	return fm_engine_msg(MSG_FMENGINE_RDS_FRONT, (void *)(int)front, NULL);
}

uint8 radio_engine_rds_busy(void *busy)
{
	fm_puts("radio_engine_rds_busy\n");

	return fm_engine_msg(MSG_FMENGINE_RDS_BUSY, busy, NULL);
}

uint8 radio_engine_rds_fast(void)
{
	fm_puts("radio_engine_rds_fast\n");

	return fm_engine_msg(MSG_FMENGINE_RDS_FAST, NULL, NULL);
}

uint8 radio_engine_rds_save(radio_rds_station_t *rds_station)
{
	fm_puts("radio_engine_rds_save\n");

	return fm_engine_msg(MSG_FMENGINE_RDS_SAVE, rds_station, NULL);
}

uint8 radio_engine_rds_load(radio_rds_station_t *rds_station)
{
	fm_puts("radio_engine_rds_load\n");

	return fm_engine_msg(MSG_FMENGINE_RDS_LOAD, rds_station, NULL);
}

#endif

