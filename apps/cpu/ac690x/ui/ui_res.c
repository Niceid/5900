#include "sdk_cfg.h"
#include "ui_res.h"


static const char *ui_lang_tab[]=
{
#if defined(TD_ADD_LCD_NUM_4)//-------------------------------四个米八显示屏-------------------------------------
	//system
	"    ",// 0
#if defined(HuiNuo_6651D_Copy)||defined(_WLS1403_SPECIAL_DISP_)
	" ON ",
	" OFF",
#elif defined(HJ_UI_SET)
	"HELL",
	" BYE",
#elif defined(_JYL_LOGO_DISPLAY_)
    " OLA",
    " BYE",
#else
	"HELL",
	" BYE",
#endif
	//radio
#if defined(Tuobu_B324)
	" 1  ",
#elif defined(HuiNuo_6651D_Copy)//惠诺抄机fm模式不需要显示
	"AEAD",
#else
	"TUNE",
#endif
	" ST ",
	"MONO",// 5
	" LOC",
	" DX ",
#if defined(_ZY32_SPECIAL_DISPLAY_)
	"MANU",
#else
	"NANU",
#endif
	"AUTO",
    " EUR",//10
    " USA",
#if defined(DAB_MODLE)
   //DAB
	" DAB",
#endif
#if defined(EASYESY_APP_ONOFF)
	"APPN",
	"APPF",
#endif
	//music
#if defined(Tuobu_B324)
	" 2  ",
	" 3  ",
#else
	" USB",
	" SD ",
#endif
	"LOAD",
#if defined(_RPT_ALL_DISPLAY_)
	" OFF",
#else
	" ALL",//" RPT ALL",//15
#endif
#if defined(_RPT_ONE_DISPLAY_)
	" AEP",
#else
	" ONE",//" RPT ONE",
#endif
#if defined(_WLS1403_SPECIAL_DISP_)
	" FOL",
#else
	" DIR",//" RPT DIR",
#endif
	" ALL",//" RPT OFF",
#if defined(_RDM_ON_DISPLAY_)
	" PDN",
#elif defined(_ZY32_SPECIAL_DISPLAY_)
	" RDM",//" RDM ON ",
#else
	" RDN",//" RDM ON ",
#endif
	
	" OFF",//" RDM OFF",//20
#if defined(_ZY32_SPECIAL_DISPLAY_)
	"SCAN",//" INT ON ",
#else
	" INT",//" INT ON ",
#endif
	" OFF",//" INT OFF",

	//linein
	
#if defined(Tuobu_B324)
	" 4  ",
#elif defined(_ENABLE_AUX_MIC_DISP_)
    " MIC",
#else
	" AUH",
#endif
	//bluetooth
	
#if defined(HuiNuo_6651D_Copy)
	" BT ",
	"CNON",//25
	"CONF",
	" BT ",
	" BT ",
	" BT ",// BT PLAY
	" BT ",//BT PAUSE// 30
	" IN ",//CALL IN
	" OUT",//CALL OUT
	" BT ",
	" BT ",
	" ERR",// 35
	"RDAL",
	"DIAL",
#elif defined(_WLS1403_SPECIAL_DISP_)
	" BT ",
	"BTON",
	"BTOF",
	"BLUE",
	"BLUE",
	"BLUE",
	"BLUE",
	"RING",
	"CALL",
	" HFP",
	"BLUE",
	" ERR",
	"REDI",
	"DIAL",
#else
	" BT ",
	"BTON",//25
	"BTOF",
	"CONN",
	"PHON",
#if defined(_JYL314E_LED_DIAPLAY_)
	"MUS>",
#elif defined(_JTI2230_A2DP_DISPLAY_)
	"BTST",
#else
	"A2DP",
#endif
	"PHON",//BT PAUSE// 30
	" IN ",//CALL IN
	" OUT",//CALL OUT
	" HFP",
	"PHON",
	" ERR",// 35
	"RDAL",
	"DIAL",
#endif
#if defined(_SUPPORT_IPHONE_SIR_)
	"SIRI",
#endif

	//upgrade
	"UPDA",
	"UPOK",
	" ERR",//40

	//others
	
#if defined(HuiNuo_6651D_Copy)
	" V  ",
	"MUTE",
	"OFF ",
	"LUON",
	"LUOF",//45
	" DSP",
	" DSP",
	" DSP",
	" DSP",
	" DSP",//50
	" DSP",
	"BS  ",
	"HI  ",
	"BL  ",
#else
	"V   ",
	"MUTE",
	"OFF ",
	"LUON",
	"LUOF",//45
	"EQOF",
	"FLAT",
#if defined(_ZY32_SPECIAL_DISPLAY_)
	"USER",
	"VOCA",
	"ROCK",//50
#else
	"CLAS",
	" POP",
	"ROCH",//50
#endif
	"JAZZ",
	"BS  ",
	"TR  ",
	"BL  ",
#endif
	"FD  ",//55
	"SBON",
	"SBOF",
	"BPON",
	"BPOF",//59
#if REC_EN
	" REC",
#if defined(_ENABLE_RECORD_DISPLAY_)
	"PAUS",
#endif
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
	"BLUE",
	"GREN",
	"CYAN",
	"RED ",
	"PURP",
	"YELL",
#if defined(ENABLE_WHITE_DISPLAY)
	"HHIT",
#else
	"WHIT",
#endif
	"AUTO",
#endif

#if defined(_SUPPORT_MIC_OK_)
	" MIC",
#endif

#if defined(_SUPPORT_REVERSE_MODEL_)
    "REVE",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFFIC ",
	" EON TA ",
	"NO TP\\TA",
	"TA SEEK ",
	"PI SEEK ",
	"PTY SEEK",
#if defined(_CHANGE_ICON_DISP_)
    "AF N    ",
	"AF F    ",
	"TA N    ",
	"TA F    ",
#else
	" AF ON  ",
	" AF OFF ",
	" TA ON  ",
	" TA OFF ",
#endif	
	"TA PAUSE",
	" REG LOC",
	" REG DX ",
	" EON ON ",
	" EON OFF",
	" R LONG ",
	" R SHORT",
	"MASK ALL",
	"MASK DIP",
	" ALARM  ",
	" SWITCH ",
	"  SYNC  ",
	" ASYNC  ",
	" SEARCH ",
	"PTY OFF ",
	" MUSIC  ",
	" SPEECH ",
	" FOUND  ",
	"NO FOUND",

	/*RDS PTY*/
	"  NONE  ",
	" N E W S",
	" AFFAIRS",
	" I N F O",
	"  SPORT ",
	" EDUCATE",
	"  DRAMA ",
	" CULTURE",
	" SCIENCE",
	" VARIED ",
	" POP  M ",
	" ROCK M ",
	" EASY M ",
	" LIGHT M",
	"CLASSICS",
	" OTHER M",
	"WEATHER ",
	"FINANCE ",
	"CHILDREN",
	" SOCIAL ",
	"RELIGION",
	"PHONE IN",
	" TRAVEL ",
	"LEISURE ",
	"  JAZZ  ",
	"COUNTRY ",
	"NATION M",
	" OLDIES ",
	" FOLK M ",
	"DOCUMENT",
	"  TEST  ",
	"  ALARM ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
#endif

#elif defined(_LCD_3_DIAPLAY_)
	//system
	"    ",// 0
	" HI ",
	" BYE",
	//radio
	" TUN",
	" ST ",
	" MON",// 5
	" LOC",
	" DX ",
	" NAN",
	" AUT",
	" EUR",//10
	" USA",
	
#if defined(DAB_MODLE)
   //DAB
	" DAB",
#endif
#if defined(EASYESY_APP_ONOFF)
	" APN",
	" APF",
#endif
	//music
	" USB",
	" SD ",
	" LOA",
	" ALL",//" RPT ALL",//15
	" ONE",//" RPT ONE",
	" DIR",//" RPT DIR",
	" ALL",//" RPT OFF",
	" RDN",//" RDM ON ",	
	" OFF",//" RDM OFF",//20
	" INT",//" INT ON ",
	" OFF",//" INT OFF",
	
	//linein
	" AUH",
	
    //bluetooth
	" BT ",
	" BT ",//25
	" BT ",
	" CON",
	" BT ",

	" A2P",
	" BT ",//BT PAUSE// 30
	" RIN",//CALL IN
	" CAL",//CALL OUT
	" HFP",
	" PHO",
	" ERR",// 35
	" RDA",
	" DIA",
	
	//upgrade
	" UDA",
	" UOK",
	" ERR",//40
	
	//others
	" V	 ",
	" MU ",
	" OFF",
	" LUN",
	" LUF",//45
	" EQF",
	" FLA",
	" CLA",
	" POP",
	" ROC",//50
	" JAZ",
	" B  ",
	" T  ",
	" B  ",
	
	" F  ",//55
	" SBN",
	" SBF",
	" BPN",
	" BPF",//59
#if REC_EN
	" REC"
#endif
	
#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
	" BLU",
	" GRE",
	" CYA",
	" RED",
	" PUR",
	" YEL",
	" WHI",
	" AUT",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFFIC ",
	" EON TA ",
	"NO TP\\TA",
	"TA SEEK ",
	"PI SEEK ",
	"PTY SEEK",
	" AF ON  ",
	" AF OFF ",
	" TA ON  ",
	" TA OFF ",
	"TA PAUSE",
	" REG LOC",
	" REG DX ",
	" EON ON ",
	" EON OFF",
	" R LONG ",
	" R SHORT",
	"MASK ALL",
	"MASK DIP",
	" ALARM  ",
	" SWITCH ",
	"  SYNC  ",
	" ASYNC  ",
	" SEARCH ",
	"PTY OFF ",
	" MUSIC  ",
	" SPEECH ",
	" FOUND  ",
	"NO FOUND",

	/*RDS PTY*/
	"  NONE  ",
	" N E W S",
	" AFFAIRS",
	" I N F O",
	"  SPORT ",
	" EDUCATE",
	"  DRAMA ",
	" CULTURE",
	" SCIENCE",
	" VARIED ",
	" POP  M ",
	" ROCK M ",
	" EASY M ",
	" LIGHT M",
	"CLASSICS",
	" OTHER M",
	"WEATHER ",
	"FINANCE ",
	"CHILDREN",
	" SOCIAL ",
	"RELIGION",
	"PHONE IN",
	" TRAVEL ",
	"LEISURE ",
	"  JAZZ  ",
	"COUNTRY ",
	"NATION M",
	" OLDIES ",
	" FOLK M ",
	"DOCUMENT",
	"  TEST  ",
	"  ALARM ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
#endif

#elif defined(_FY7000_SPECIAL_DISPLAY_)//----------------飞音13个米八显示屏----------------------------------------------------
	//system
	"             ",// 0
#if defined(_FEIYIN_7000F_LOGO_)
	"  MTX AUDIO  ",
#elif defined(_FY_7000_LOGO_)
	"     AMA     ",
#elif defined(_FY7000_SPECIAL_LOGO_)
	"     ACV     ",	
#elif defined(_FEIYIN_7000A_LOGO_)
	"   PROLOGY   ",
#elif defined(_FEIYIN_8567_LOGO_)
	"   DYNASTY   ",
#elif defined(_FY_8567B_LOGO_)
	"    POWER    ",
#elif defined(_FY_8567D_LOGO_)
	"     ACV     ",
#elif defined(_FY_8567C_LOGO_)
	"   X-FIDER   ",
#elif defined(_FY8567A_POWER_LOGO_)
	"   MYSTERY   ",
#elif defined(_FY_7000B_LOGO_)
	"   ROADEX    ",
#elif defined(_FEIYIN_8567A_LOGO_)
	"  CITISOUND  ",
#elif defined(_FY8568A_LOGO_DISPLAY_)
	"    HELLO    ",
#else
	"   WELCOME   ",
#endif
#if defined(_FEIYIN_7000A_LOGO_)
	"   PROLOGY   ",
#elif defined(_FY7000_SPECIAL_LOGO_)
	"     ACV     ",
#else
	"  GOOD-BYE   ",
#endif
	//radio
	"    RADIO    ",
	"   STEREO    ",
	"    MONO     ",// 5
	"    LOCAL    ",
	"     DX      ",
	"   MANUAL    ",
	"    AUTO     ",
    "  AREA  EUR  ",//10
    "  AREA  USA  ",
#if defined(DAB_MODLE)
   //DAB
	"     DAB     ",
#endif
	
#if defined(EASYESY_APP_ONOFF)
	"  APP   ON   ",
	"  APP   OFF  ",
#endif
	//music
	"     USB     ",
	"   SD/MMC    ",
	"    LOAD     ",
	"   RPT  ALL  ",//15
	"   RPT  ONE  ",
	"   RPT  DIR  ",
	"   RPT  OFF  ",
	"   RDM  ON   ",
	"   RDM  OFF  ",//20
	"   INT  ON   ",
	"   INT  OFF  ",
	
#if defined(_SUPPORT_DISC_)
	"    DISC	  ",
	"    EJECT	  ",
#if defined(NEED_NO_DISC_DISP)
	"   NODISC    ",
#endif
#endif

	//linein
	"   AUX  IN   ",

	//bluetooth
	"     BT      ",
	"   BT  ON    ",//25
	"   BT  OFF   ",
	"   CONNECT   ",
	"    PHONE    ",
	"  BT  AUDIO  ",// BT PLAY
	"    PHONE    ",//BT PAUSE// 30
	"   RINGING   ",
	"   CALLING   ",
	"   BT HFP    ",
	"  BT  PHONE  ",
	"    ERROR    ",// 35
	"   REDIAL    ",
	"   DIAL->    ",

	//upgrade
	"   UPGRADE   ",
	"   SUCCEED   ",
	"    FAIL     ",//40

	//others
	"  VOL        ",
	"  MUTE  ON   ",
	"  MUTE  OFF  ",
	"  LOUD  ON   ",
	"  LOUD  OFF  ",//45
	"   EQ  OFF   ",
	"    FLAT     ",
	"   CLASSIC   ",
	"     POP     ",
	"    ROCK     ",//50
	"    JAZZ     ",
	"  BAS        ",
	"  TRE        ",
	"  BAL        ",
	"  FAD        ",//55
#if defined(_ENABLE_AP3768_EQ_MODE_)
	"   PEOPLE    ",
	"    SOFT     ",
	"  ELECTRIC   ",
#endif
	"  SUB1  ON   ",
	"  SUB1  OFF  ",
	"  BEEP  ON   ",
	"  BEEP  OFF  ",//59

#if REC_EN
	"   RECORD    ",
#if defined(_ENABLE_RECORD_DISPLAY_)
	"   PAUSE     ",
#endif
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	" SUB VOL     ",
#endif
	
#if defined(_SUPPORT_SUBWOOFER3_)||defined(_ENABLE_AP3768_SUBWOOFER_)
    "  SUB2  ON   ",
    "  SUB2  OFF  ",
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	"  LPF        ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "    BLUE     ",
    "    GREEN    ",
    "    CYAN     ",
    "     RED     ",
	"   PURPLE    ",
    "   YELLOW    ",
    "    WHITE    ",
    "    AUTO     ",
#endif

#if defined(_FAD_BAL_FUNTION_)
	"  FL         ",//前左
	"  FR         ",//前右
	"  RL         ",//后左
	"  RR         ",//后右
#endif

#if defined(_SUPPORT_10_EQ_FUNTION_)
	"EQ  60HZ     ",
	"EQ 150HZ     ",
	"EQ 400HZ     ",
	"EQ  1KHZ     ",
	"EQ  2KHZ     ",
	"EQ  3KHZ     ",
	"EQ  7KHZ     ",
	"EQ 10KHZ     ",
	"EQ 15KHZ     ",
	"EQ 20KHZ     ",
#endif

#if defined(_FY7555_LCD_DISPLAY_)
	"     SET     ",
#endif

#if defined(_FEIYIN_MENU_TIME_)
	"  TIME  SET  ",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"   TRAFFIC   ",
	"   EON  TA   ",
	"  NO TP\\TA  ",
	"  TA  SEEK   ",
	"  PI  SEEK   ",
	"  PTY  SEEK  ",
	"   AF  ON    ",
	"   AF  OFF   ",
	"   TA  ON    ",
	"   TA  OFF   ",
	"  TA  PAUSE  ",
	"  REG  LOC   ",
	"  REG  DX    ",
	"  EON  ON    ",
	"  EON  OFF   ",
	"   R  LONG   ",
	"   R  SHORT  ",
	"  MASK  ALL  ",
	"  MASK DIP   ",
	"    ALARM    ",
	"   SWITCH    ",
	"    SYNC     ",
	"    ASYNC    ",
	"   SEARCH    ",
	"  PTY  OFF   ",
	"    MUSIC    ",
	"   SPEECH    ",
	"    FOUND    ",
	"  NO  FOUND  ",

	/*RDS PTY*/
	"    NONE     ",
	"   N E W S   ",
	"   AFFAIRS   ",
	"   I N F O   ",
	"    SPORT    ",
	"   EDUCATE   ",
	"    DRAMA    ",
	"   CULTURE   ",
	"   SCIENCE   ",
	"   VARIED    ",
	"   POP  M    ",
	"   ROCK  M   ",
	"   EASY  M   ",
	"  LIGHT  M   ",
	"  CLASSICS   ",
	"   OTHER M   ",
	"   WEATHER   ",
	"   FINANCE   ",
	"  CHILDREN   ",
	"   SOCIAL    ",
	"  RELIGION   ",
	"  PHONE  IN  ",
	"   TRAVEL    ",
	"   LEISURE   ",
	"    JAZZ     ",
	"   COUNTRY   ",
	"  NATION  M  ",
	"   OLDIES    ",
	"   FOLK  M   ",
	"  DOCUMENT   ",
	"    TEST     ",
	"    ALARM    ",

	/*RBDS PTY*/
	"    NONE     ",
	"    NEWS     ",
	"   INFORM    ",
	"   SPORTS    ",
	"    TALK     ",
	"    ROCK     ",
	"  CLS  ROCK  ",
	"  ADLT  HIT  ",
	"  SOFT  RCK  ",
	"   TOP  40   ",
	"   COUNTRY   ",
	"   OLDIES    ",
	"    SOFT     ",
	"  NOSTALGA   ",
	"    JAZZ     ",
	"  CLASSICL   ",
	"     R_B     ",
	"  SOFT  R_B  ",
	"  LANGUAGE   ",
	"  REL  MUSC  ",
	"  REL  TALK  ",
	"  PERSNLTY   ",
	"   PUBLIC    ",
	"   COLLEGE   ",
	"   UNDEF1    ",
	"   UNDEF2    ",
	"   UNDEF3    ",
	"   UNDEF4    ",
	"   UNDEF5    ",
	"   WEATHER   ",
	"     TEST    ",
	"    ALERT    ",
#endif

#elif defined(_FY5517_SPECIAL_DISPLAY_)
	//system
	"           ",// 0
	"  WELCOME  ",
	" GOOD-BYE  ",
	//radio
	"   RADIO   ",
	"  STEREO   ",
	"   MONO    ",// 5
	"   LOCAL   ",
	"    DX     ",
	"  MANUAL   ",
	"   AUTO    ",
    " AREA EUR  ",//10
    " AREA USA  ",
   
#if defined(DAB_MODLE)
	   //DAB
	"    DAB    ",
#endif
#if defined(EASYESY_APP_ONOFF)
	" APP  ON   ",
	" APP  OFF  ",
#endif
	//music
	"    USB    ",
	"  SD/MMC   ",
	"   LOAD    ",

	" RPT  ALL  ",//15
	" RPT  ONE  ",
	" RPT  DIR  ",
	" RPT  OFF  ",
	" RDM  ON   ",
	" RDM  OFF  ",//20
	" INT  ON   ",
	" INT  OFF  ",

	//linein
	"  AUX  IN  ",
	//bluetooth
	"    BT     ",
	"  BT  ON   ",//25
	"  BT  OFF  ",
	"  CONNECT  ",
	"   PHONE   ",
	" BT AUDIO  ",// BT PLAY
	"   PHONE   ",//BT PAUSE// 30
	"  RINGING  ",
	"  CALLING  ",
	"  BT HFP   ",
	" BT PHONE  ",
	"   ERROR   ",// 35
	"  REDIAL   ",
	"  DIAL->   ",
	//upgrade
	"  UPGRADE  ",
	"  SUCCEED  ",
	"   FAIL    ",//40

	//others
	" VOL	    ",
	"  MUTE ON  ",
	"  MUTE OFF ",
	" LOUD ON   ",
	" LOUD OFF  ",//45
	"  EQ OFF   ",
	"   FLAT    ",
	"  CLASSIC  ",
	"    POP    ",
	"   ROCK    ",//50
	"   JAZZ    ",
	" BAS	    ",
	" TRE	    ",
	" BAL	    ",
	" FAD	    ",//55
    " SUB1 ON   ",
    " SUB1 OFF  ",
	" BEEP ON   ",
	" BEEP OFF  ",//59

#if REC_EN
	"  RECORD   ",
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUBVOL     ",
#endif
	
#if defined(_SUPPORT_SUBWOOFER3_)
    " SUB2  ON  ",
    " SUB2  OFF ",
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	" LPF       ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "   BLUE    ",
    "   GREEN   ",
    "   CYAN    ",
    "    RED    ",
	"  PURPLE   ",
    "  YELLOW   ",
    "   WHITE   ",
    "   AUTO    ",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"  TRAFFIC  ",
	"  EON TA   ",
	" NO TP\\TA ",
	"  TA SEEK  ",
	"  PI SEEK  ",
	" PTY SEEK  ",
	"  AF ON    ",
	"  AF OFF   ",
	"  TA ON    ",
	"  TA OFF   ",
	" TA PAUSE  ",
	"  REG LOC  ",
	"  REG DX   ",
	"  EON ON   ",
	"  EON OFF  ",
	"  R LONG   ",
	"  R SHORT  ",
	" MASK ALL  ",
	" MASK DIP  ",
	"   ALARM   ",
	"  SWITCH   ",
	"   SYNC    ",
	"   ASYNC   ",
	"  SEARCH   ",
	" PTY OFF   ",
	"   MUSIC   ",
	"  SPEECH   ",
	"   FOUND   ",
	" NO FOUND  ",

	/*RDS PTY*/
	"   NONE    ",
	"  N E W S  ",
	"  AFFAIRS  ",
	"  I N F O  ",
	"   SPORT   ",
	"  EDUCATE  ",
	"   DRAMA   ",
	"  CULTURE  ",
	"  SCIENCE  ",
	"  VARIED   ",
	"  POP  M   ",
	"  ROCK M   ",
	"  EASY M   ",
	"  LIGHT M  ",
	" CLASSICS  ",
	"  OTHER M  ",
	"  WEATHER  ",
	"  FINANCE  ",
	" CHILDREN  ",
	"  SOCIAL   ",
	" RELIGION  ",
	" PHONE IN  ",
	"   TRAVEL  ",
	"  LEISURE  ",
	"   JAZZ    ",
	"  COUNTRY  ",
	" NATION M  ",
	"  OLDIES   ",
	"  FOLK M   ",
	" DOCUMENT  ",
	"   TEST    ",
	"   ALARM   ",

	/*RBDS PTY*/
	"   NONE    ",
	"   NEWS    ",
	"  INFORM   ",
	"  SPORTS   ",
	"   TALK    ",
	"   ROCK    ",
	" CLS ROCK  ",
	" ADLT HIT  ",
	" SOFT RCK  ",
	"  TOP 40   ",
	"  COUNTRY  ",
	"  OLDIES   ",
	"   SOFT    ",
	" NOSTALGA  ",
	"   JAZZ    ",
	" CLASSICL  ",
	"    R_B    ",
	" SOFT R_B  ",
	" LANGUAGE  ",
	" REL MUSC  ",
	" REL TALK  ",
	" PERSNLTY  ",
	"  PUBLIC   ",
	"  COLLEGE  ",
	"  UNDEF1   ",
	"  UNDEF2   ",
	"  UNDEF3   ",
	"  UNDEF4   ",
	"  UNDEF5   ",
	"  WEATHER  ",
	"   TEST    ",
	"   ALERT   ",
#endif

#elif defined(_XZC1668_SPECIAL_DISPLAY_)
	//system
	"          ",// 0
#if defined(_XZC1668_LOGO_DISPLAY_)
	"    CHERY ",
	"    CHERY ",
#else
	"    HELLO ",
	"    BYEBYE",
#endif
	//radio
	"    RADIO ",
	"    STEREO",
	"    MONO  ",// 5
	"    SRC LO",//LOC
	"    SRC DX",//DX
	"    MANUAL",
	"    AUTO  ",
    "    EUROPE",//10
    "     USA  ",
   
#if defined(_SUPPORT_FOUR_AREA_)
	"    LATRM ",
	"    ASIA  ",
#endif

#if defined(DAB_MODLE)
	   //DAB
	"     DAB  ",
#endif
#if defined(EASYESY_APP_ONOFF)
	"    APP ON",
	"    APPOFF",
#endif
	//music
	"     USB  ",
	"    SD/MMC",
	"    LOAD  ",

	"    RPTALL",//15
	"    RPTONE",
	"    RPTDIR",
	"    RPTOFF",
	"    RDM ON",
	"    RDMOFF",//20
	"    INT ON",
	"    INTOFF",

	//linein
	"    AUX IN",
	//bluetooth
	"     BT   ",
	"    BT ON ",//25
	"    BT OFF",
	"    CONNET",
	"    PHONE ",
	"    A2DP  ",// BT PLAY
	"    PHONE ",//BT PAUSE// 30
	"    RING  ",
	"    CALL  ",
	"    BT HFP",
	"  BTPHONE ",
	"    ERROR ",// 35
	"    REDIAL",
	"    DIAL->",
	//upgrade
	"    UPG   ",
	"    SUCCED",
	"    FAIL  ",//40

	//others
	"    VOL   ",
	"    MUTE  ",
	"  MUTE OFF",
	"    LOU ON",
	"    LOUOFF",//45
	"    EQ OFF",
	"    VOCAL ",
	"    CLASSI",
	"     POP  ",
	"    ROCK  ",//50
	"    JAZZ  ",
	"    BAS   ",
	"    TRE   ",
	"    BAL   ",
	"    FAD   ",//55
    "    SUB ON",
    "    SUBOFF",
	"    BEP ON",
	"    BEPOFF",//59

#if REC_EN
	"    RECORD",
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUBV      ",
#endif
	
#if defined(_SUPPORT_SUBWOOFER3_)
    "SUB2 ON  ",
    "SUB2 OFF ",
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	" LPF      ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "    BLUE  ",
    "    GREEN ",
    "    CYAN  ",
    "     RED  ",
	"    PURPLE",
    "    YELLOW",
    "    WHITE ",
    "    AUTO  ",
#endif

#if defined(_ENABLE_MENU_FUNTION_)
	"    SETUP ",
	"   SVOL   ",
	"    LOG ON",
	"    LOGOFF",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"    TRAFFI",
	"    EON TA",
	"    NOTPTA",
	"    TASEEK",
	"    PISEEK",
	"    PTYSEK",
	"    AF ON ",
	"    AF OFF",
	"    TA ON ",
	"    TA OFF",
	"    TAPAUS",
	"    REGLOC",
	"    REG DX",
	"    EON ON",
	"    EONOFF",
	"    R LONG",
	"    R SHOR",
	"    MSKALL",
	"    MSKDIP",
	"    ALARM ",
	"    SWITCH",
	"    SYNC  ",
	"    ASYNC ",
	"    SEARCH",
	"    PTYOFF",
	"    MUSIC ",
	"    SPEECH",
	"    FOUND ",
	"    NOFOUD",

	/*RDS PTY*/
	"    NONE  ",
	"    NEWS  ",
	"    AFFAIR",
	"    INFO  ",
	"    SPORT ",
	"    EDUCAT",
	"    DRAMA ",
	"    CULTUR",
	"    SCIENC",
	"    VARIED",
	"    POP  M",
	"    ROCK M",
	"    EASY M",
	"    LIGHTM",
	"    CLASSI",
	"    OTHERM",
	"    WEATHE",
	"    FINANC",
	"    CHILDR",
	"    SOCIAL",
	"    RELIGI",
	"    PHONE ",
	"    TRAVEL",
	"    LEISUR",
	"    JAZZ  ",
	"    COUNTR",
	"    NATION",
	"    OLDIES",
	"    FOLK M",
	"    DOCUME",
	"    TEST  ",
	"    ALARM ",

	/*RBDS PTY*/
	"    NONE  ",
	"    NEWS  ",
	"    INFORM",
	"    SPORTS",
	"    TALK  ",
	"    ROCK  ",
	"    CLSROC",
	"    ADLTHI",
	"    SOFTRC",
	"    TOP 40",
	"    COUNTR",
	"    OLDIES",
	"    SOFT  ",
	"    NOSTAL",
	"    JAZZ  ",
	"    CLASSI",
	"    R_B   ",
	"    SOFT  ",
	"    LANGUA",
	"    RELMUS",
	"    RELTAL",
	"    PERSNL",
	"    PUBLIC",
	"    COLLEG",
	"    UNDEF1",
	"    UNDEF2",
	"    UNDEF3",
	"    UNDEF4",
	"    UNDEF5",
	"    WEATHE",
	"    TEST  ",
	"    ALERT ",
#endif

#elif defined(_SH1330_SPECIAL_DISPLAY_)
	//system
	"          ",// 0
#if defined(_ENABLE_SPECIAL_LOGO_DISP_)
    "PAZH-SAIPA",
#else
	"  HELLO   ",
#endif	
	"  BYEBYE  ",
	//radio
	"  RADIO   ",
	"  STEREO  ",
	"   MONO   ",// 5
	"   LOCAL  ",//LOC
	"   DX     ",//DX
	"   MANUAL ",
	"   AUTO   ",
    "   EUROPE ",//10
    "   USA    ",
   
#if defined(_SUPPORT_FOUR_AREA_)
	"    LATRM ",
	"    ASIA  ",
#endif

#if defined(DAB_MODLE)
	   //DAB
	"     DAB  ",
#endif
#if defined(EASYESY_APP_ONOFF)
	"  APP ON  ",
	"  APPOFF  ",
#endif
	//music
	"   USB    ",
	"   SD/MMC ",
	"   LOAD   ",

	"  RPTALL  ",//15
	"  RPTONE  ",
	"  RPTDIR  ",
	"  RPTOFF  ",
	"  RDM ON  ",
	"  RDMOFF  ",//20
#if defined(_ENABLE_PLAYER_INT_DISP_)
    "  SCN ON  ",
	"  SCN OFF ",
#else
	"  INT ON  ",
	"  INTOFF  ",
#endif	

	//linein
	"  AUX IN  ",
	//bluetooth
	"   BT     ",
	"  BT ON   ",//25
	"  BT OFF  ",
	"  CONNET  ",
	"  PHONE   ",
	"  A2DP    ",// BT PLAY
	"  PHONE   ",//BT PAUSE// 30
	"  RING    ",
	"  CALL    ",
	"  BT HFP  ",
	"  BTPHONE ",
	"  ERROR   ",// 35
	"  REDIAL  ",
	"  DIAL->  ",
	//upgrade
	"   UPG    ",
	"  SUCCED  ",
	"   FAIL   ",//40

	//others
	"    VOL   ",
	"    MUTE  ",
	"  MUTE OFF",
	"  LOUD ON ",
	" LOUD OFF ",//45
	"   EQ OFF ",
	"    VOCAL ",
	"    CLASSI",
	"     POP  ",
	"    ROCK  ",//50
	"    JAZZ  ",
	"    BAS   ",
	"    TRE   ",
	"    BAL   ",
	"    FAD   ",//55
    "    SUB ON",
    "    SUBOFF",
	"    BEP ON",
	"    BEPOFF",//59

#if REC_EN
	"    RECORD",
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUBV      ",
#endif
	
#if defined(_SUPPORT_SUBWOOFER3_)
    "SUB2 ON  ",
    "SUB2 OFF ",
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	" LPF      ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "    BLUE  ",
    "    GREEN ",
    "    CYAN  ",
    "     RED  ",
	"    PURPLE",
    "    YELLOW",
    "    WHITE ",
    "    AUTO  ",
#endif

#if defined(_SUPPORT_PASSWORD_DISPLAY_)
    " PWD **** ",
    "ERROR  01 ",
    "ERROR  02 ",
    "ERROR  03 ",
    "ERROR  04 ",
    "ERROR  05 ",
#endif

#if defined(_ENABLE_PASSWORD_SET_)
	" PWD  SET ",
	" PWD ---- ",
#endif

#if defined(_PASSWORD_TRUE_DISPLAY_)
	" CORRECT  ",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"  TRAFFI  ",
	"  EON TA  ",
	"  NOTPTA  ",
	"  TASEEK  ",
	"  PISEEK  ",
	"  PTYSEK  ",
	"  AF ON   ",
	"  AF OFF  ",
	"  TA ON   ",
	"  TA OFF  ",
	"  TAPAUS  ",
	"  REGLOC  ",
	"  REG DX  ",
	"  EON ON  ",
	"  EONOFF  ",
	"  R LONG  ",
	"  R SHOR  ",
	"  MSKALL  ",
	"  MSKDIP  ",
	"  ALARM   ",
	"  SWITCH  ",
	"  SYNC    ",
	"  ASYNC   ",
	"  SEARCH  ",
	"  PTYOFF  ",
	"  MUSIC   ",
	"  SPEECH  ",
	"  FOUND   ",
	"  NOFOUD  ",

	/*RDS PTY*/
	"  NONE    ",
	"  NEWS    ",
	"  AFFAIR  ",
	"  INFO    ",
	"  SPORT   ",
	"  EDUCAT  ",
	"  DRAMA   ",
	"  CULTUR  ",
	"  SCIENC  ",
	"  VARIED  ",
	"  POP  M  ",
	"  ROCK M  ",
	"  EASY M  ",
	"  LIGHTM  ",
	"  CLASSI  ",
	"  OTHERM  ",
	"  WEATHE  ",
	"  FINANC  ",
	"  CHILDR  ",
	"  SOCIAL  ",
	"  RELIGI  ",
	"  PHONE   ",
	"  TRAVEL  ",
	"  LEISUR  ",
	"  JAZZ    ",
	"  COUNTR  ",
	"  NATION  ",
	"  OLDIES  ",
	"  FOLK M  ",
	"  DOCUME  ",
	"  TEST    ",
	"  ALARM   ",

	/*RBDS PTY*/
	"  NONE    ",
	"  NEWS    ",
	"  INFORM  ",
	"  SPORTS  ",
	"  TALK    ",
	"  ROCK    ",
	"  CLSROC  ",
	"  ADLTHI  ",
	"  SOFTRC  ",
	"  TOP 40  ",
	"  COUNTR  ",
	"  OLDIES  ",
	"  SOFT    ",
	"  NOSTAL  ",
	"  JAZZ    ",
	"  CLASSI  ",
	"  R_B     ",
	"  SOFT    ",
	"  LANGUA  ",
	"  RELMUS  ",
	"  RELTAL  ",
	"  PERSNL  ",
	"  PUBLIC  ",
	"  COLLEG  ",
	"  UNDEF1  ",
	"  UNDEF2  ",
	"  UNDEF3  ",
	"  UNDEF4  ",
	"  UNDEF5  ",
	"  WEATHE  ",
	"  TEST    ",
	"  ALERT   ",
#endif

#elif defined(_4MIBA_4SUZIBA_)//--------------------------守卫龙5个米八4个数字八显示屏------------------------------------------
   //system
	"        ",// 0
	"HELL    ",

	"BYE     ",
	//radio
	"TUNE    ",
	"ST      ",
	"MONO    ",// 5
	"LOC     ",
	"DX      ",
	"NANU    ",
	"AUTO    ",
    "EUR     ",//10
    "USA     ",
    #if defined(DAB_MODLE)
   //DAB
	"DAB     ",
    #endif
#if defined(EASYESY_APP_ONOFF)
	"APP   ON",
	"APP  OFF",
#endif
	//music
	"USB     ",
	"SD      ",
	"LOAD    ",
	"ALL     ",//" RPT ALL",//15
	"ONE     ",//" RPT ONE",
	"DIR     ",//" RPT DIR",
	"ALL     ",//" RPT OFF",
	"RDN     ",//" RDM ON ",
	"OFF     ",//" RDM OFF",//20
	"INT     ",//" INT ON ",
	"OFF     ",//" INT OFF",
	
#if defined(_SUPPORT_DISC_)
	"DISC    ",
	"EJEC    ",
#if defined(NEED_NO_DISC_DISP)
	"NO DISC ",
#endif
#endif
	//linein
	"AUX     ",
	//bluetooth
	"BT      ",
	"BT   ON ",//25
	"BT   OFF",
	"CONN    ",
	"PHON    ",
	"A2DP    ",// BT PLAY
	"PHON    ",//BT PAUSE// 30
	"IN      ",//CALL IN
	"OUT     ",//CALL OUT
	"HFP     ",
	"PHON    ",
	"ERR     ",// 35
	"RDAL    ",
	"DIAL    ",

	//upgrade
	"UPDA    ",
	"UPOK    ",
	"ERR     ",//40

	//others
	"VOL	 ",
	"MUTE    ",
	"OFF     ",
	"LUON    ",
	"LUOF    ",//45
	"EQOF    ",
	"FLAT    ",
	"CLAS    ",
	"POP     ",
	"ROCH    ",//50
	"JAZZ    ",
	"BAS     ",
	"TRE     ",
	"BAL     ",
	"FAD     ",//55
	"SUB  ON ",
	"SUB  OFF",
	"BEEP ON ",
	"BEEP OFF",//59
	#if REC_EN
	"REC     "
	#endif
	#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	" SUB    ",
	#endif
	
	#if defined(AUDIO_SUB_LEVEL_SET)
	"CUT     ",
	#endif

	#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "BLUE    ",
    "GREEN   ",
    "CYAN    ",
    "RED     ",
	"PURPLE  ",
    "YELLOW  ",
    "WHITE   ",
    "AUTO    ",
	#endif
	#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFF   ",
	"EONTA   ",
	"NO TP   ",
	"TASEK   ",
	"PISEK   ",
	"PTYSK   ",
	"AF ON   ",
	"AF OFF  ",
	"TA ON   ",
	"TA OFF  ",
	"TAPAU   ",
	"REGLOC  ",
	"REGDX   ",
	"EONON   ",
	"EONOFF  ",
	"RLONG   ",
	"RSHOT   ",
	" ALL    ",
	" DIP    ",
	"ALARM   ",
	"SWITCH  ",
	" SYNC   ",
	"ASYNC   ",
	"SEARCH  ",
	"PTYOFF  ",
	"MUSIC   ",
	"SPEECH  ",
	"FOUND   ",
	"NOFOD   ",

	/*RDS PTY*/
	"NONE    ",
	"NEWS    ",
	"AFFAI   ",
	"INFO    ",
	"SPORT   ",
	"EDUCA   ",
	"DRAMA   ",
	"CULTU   ",
	"SCIEN   ",
	"VARIE   ",
	"POP M   ",
	"ROC M   ",
	"EASYM   ",
	"LIGHT   ",
	"CLASS   ",
	"OTHER   ",
	"WEATH   ",
	"FINAN   ",
	"CHILD   ",
	"SOCIA   ",
	"RELIG   ",
	"PHONE   ",
	"TRAVEL  ",
	"LEISU   ",
	"JAZZ    ",
	"COUNT   ",
	"NATIO   ",
	"OLDIE   ",
	"FOLKM   ",
	"DOCUM   ",
	"TEST    ",
	"ALARM   ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
	#endif
#elif defined(_FIVE_CHAR_DISPLAY_)//------五个字符显示------------------------
	//system
	"        ",// 0
#if defined(_TY2200_SPECIAL_DISP_)
    "  HI    ",
#elif defined(ENABLE_POWER_LOGO_)
    "JEBSO<  ",
#elif defined(SPECIAL_SLA6799_DISP)
    " HELLO  ",
#elif defined(_ENABLE_LOGO_DISP_)
    "  AURA  ",
#else
	"HELLO   ",
#endif	
#if defined(SPECIAL_JD_DISP)||defined(SPECIAL_SLA6799_DISP)
	"  BYE   ",
#elif defined(_TY2200_SPECIAL_DISP_)
	"  BYE   ",
#elif defined(_SL6802_SPECIA_DISPLAY_)
	"BA<BA<  ",
#else
	"BYEBYE  ",
#endif

	//radio
#if defined(_SL6802_SPECIA_DISPLAY_)||defined(_SUPPORT_SPECIAL_DISPLAY_)
    "RADYO   ",
#elif defined(SPECIAL_SLA6799_DISP)
    " RADIO  ",
#else
	"RADIO   ",
#endif	
	"STEREO  ",
	" MONO   ",
#if defined(_SL6802_SPECIA_DISPLAY_)
	"YEREL	 ",
#elif defined(SPECIAL_SLA6799_DISP)
	" LOCAL  ",
#else	
	"LOCAL   ",
#endif	
	"  DX    ",
	"MANUAL  ",
#if defined(_SL6802_SPECIA_DISPLAY_)
	" OTO    ",
#else	
	" AUTO	 ",
#endif	
	" EUR    ",
	" USA    ",
#if defined(DAB_MODLE)
	   //DAB
	" DAB    ",
#endif

#if defined(EASYESY_APP_ONOFF)
#if defined(_SWL8809_DISPLAY_)
	"APP ON	 ",
#else
	"APPON   ",
#endif
	"APPOFF  ",
#endif
	//music
#if defined(SPECIAL_SLA6799_DISP)
    "  USB   ",
    "   SD   ",
#else
	" USB    ",
	"  SD    ",
#endif		
	" LOAD   ",
#if defined(_RPT_ALL_DISPLAY_)
	"RPTOFF  ",//15
#elif defined(_TY2200_SPECIAL_DISP_)
	"  ALL   ",
#elif defined(SPECIAL_SLA6799_DISP)
	"  OFF   ",
#else
	"RPTALL  ",
#endif
#if defined(_TY2200_SPECIAL_DISP_)
    "  RPT   ",
	"  RPT   ",
	"  ALL   ",
#elif defined(SPECIAL_SLA6799_DISP)
	"  ONE   ",
	"  DIR   ",
	"  OFF   ",
#else
	"RPTONE  ",
	"RPTDIR  ",
	"RPTOFF  ",
#endif	
#if defined(_SWL8809_DISPLAY_)
	"RDM ON  ",
#elif defined(_TY2200_SPECIAL_DISP_)
	"  RDM   ",
#elif defined(_SL6802_SPECIA_DISPLAY_)
#if defined(_CHANGE_PLAYER_DISP_)
    "RDMAC   ",
#else
	"RDMKPL  ",
#endif	
#elif defined(SPECIAL_SLA6799_DISP)
	"  RDM   ",
#else
	"RDMON   ",
#endif
#if defined(_TY2200_SPECIAL_DISP_)
    "  ALL   ",
#elif defined(_SL6802_SPECIA_DISPLAY_)
#if defined(_CHANGE_PLAYER_DISP_)
    "RDMKPL  ",
#else
    "RDMAC   ",
#endif    
#elif defined(SPECIAL_SLA6799_DISP)
    "  OFF   ",
#else
	"RDMOFF  ",
#endif	
#if defined(_SWL8809_DISPLAY_)
	"INT ON  ",
#elif defined(_TY2200_SPECIAL_DISP_)
	"  INT   ",
#elif defined(_SL6802_SPECIA_DISPLAY_)
#if defined(_CHANGE_PLAYER_DISP_)
    "INTAC   ",
#else
	"INTKPL  ",
#endif	
#elif defined(SPECIAL_SLA6799_DISP)
	"  INT   ",
#else
	"INTON   ",
#endif
#if defined(_TY2200_SPECIAL_DISP_)
    "  ALL   ",
#elif defined(_SL6802_SPECIA_DISPLAY_)
#if defined(_CHANGE_PLAYER_DISP_)
    "INTKPL  ",
#else
    "INTAC   ",
#endif    
#elif defined(SPECIAL_SLA6799_DISP)
    "  OFF   ",
#else
	"INTOFF  ",
#endif	
#if defined(_SUPPORT_DISC_)
	" DISC   ",
	"EJECT   ",
#if defined(NEED_NO_DISC_DISP)
	"NODISC  ",
#endif
#endif

	//linein
#if defined(SPECIAL_SLA6799_DISP)
    "  AUX   ",
#else
	" AUX    ",
#endif	

#if defined(_SUPPORT_SUB_)
    "SUBON   ",
    "SUBOFF  ",
#endif

#if defined(_SEL_ADD_TIME_SELECT_)
#if defined(SPECIAL_JD_DISP)
	"TIME24  ",
	"TIME12  ",
#else
	"CLK 24  ",
	"CLK 12  ",
#endif
#endif

	//bluetooth
	"  BT    ",
#if defined(_TY2200_SPECIAL_DISP_)
    "  BT    ",
    "  BT    ",
#else	
	"BT ON   ",
	"BT OFF  ",
#endif	
#if defined(SPECIAL_JD_DISP)
	"  BT    ",
	"BT ON   ",
	"BT ON   ",
	"BT ON   ",	
#else
#if defined(_GX_SPECIAL_DISP_)
	"BT OFF  ",
#elif defined(_TY2200_SPECIAL_DISP_)
	"  BT    ",
#elif defined(_SUPPORT_SPECIAL_DISPLAY_)
    "BAGLA<  ",
#else
	"CONNECT ",
#endif
#if defined(_CHANGE_BT_DISP_)
    "  BT    ",
    "  BT    ",
    "  BT    ",
#elif defined(_SUPPORT_SPECIAL_DISPLAY_)
    " TEL    ",
	" A2DP   ",// BT PLAY
	" TEL    ",//BT PAUSE
#else
	"PHONE   ",
	" A2DP   ",// BT PLAY
	"PHONE   ",//BT PAUSE
#endif	
#endif

	" RING   ",
#if defined(_SUPPORT_SPECIAL_DISPLAY_)
    "ARAMA   ",
#else	
	" CALL   ",
#endif	
	"BT HFP  ",
#if defined(SPECIAL_JD_DISP)
	"BT ON   ",
#elif defined(_SUPPORT_SPECIAL_DISPLAY_)
	"  TEL   ",
#else
	" PHONE  ",
#endif
	"ERROR   ",
	"REDIAL  ",
	"DIAL->  ",
#if defined(_SUPPORT_IPHONE_SIR_)
	" SIRI   ",
#endif
	//upgrade
	" UPG    ",
	" SUCS   ",
	" FAIL   ",

	//others
#if defined(_SUPPORT_SPECIAL_DISPLAY_)
    "SES     ",
	"SESSI>  ",
#else
	"VOL     ",
	" MUTE   ",
#endif	
	"MUTE OFF",

#if defined(_SWL8809_DISPLAY_)
	"LOU ON  ",
#elif defined(SPECIAL_SLA6799_DISP)
    "LD ON   ",
#else
	"LOUON   ",
#endif
#if defined(SPECIAL_SLA6799_DISP)
    "LD OFF  ",
#else
	"LOUOFF  ",
#endif
	"EQ OFF  ",
#if defined(_TY2200_SPECIAL_DISP_)
    " COUN   ",
#else	
	" FLAT   ",
#endif
#if defined(SPECIAL_SLA6799_DISP)
    "CLASS   ",
	"  POP   ",
	" ROCK   ",
	" JAZZ   ",
#else
	"CLASS   ",
	" POP    ",
	" ROCK   ",
	" JAZZ   ",
#endif	
	"BAS     ",
	"TRE     ",
	"BAL     ",
	"FAD     ",
#if defined(_SUPPORT_SUBWOOFER3_)
    "SU1ON	 ",
    "SU1OFF  ",
#else
	"SUBON   ",
	"SUBOFF  ",
#endif
	"BEPON   ",
	"BEPOFF  ",

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUB     ",
#endif
#if defined(_SUPPORT_SUBWOOFER3_)
    "SU2ON  ",
    "SU2OFF ",
#endif	

#if defined(AUDIO_SUB_LEVEL_SET)
	"CUT     ",
#endif

#if defined(_ENABLE_7419_MIDDLE_)
	"MID     ",
#endif

#if defined(_ENABLE_ID3_ON_OFF_)||defined(_ENABLE_ID3_SET_)
    "ID3ON   ",
    "ID3OFF  ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
	" BLUE   ",
	"GREEN   ",
	" CYAN   ",
	" RED    ",
	" PURP   ",
	"YELOW   ",
	"WHITE   ",
	" AUTO   ",
#endif

#if defined(_MR889_SPECAIL_KEY_)
	"SPK ON  ",
	"SPK OFF ",
#endif
#if defined(_BT_DISCONNECT_DISPLAY_)
    "NO BT   ",
#endif

#if defined(_ENABLE_PLAY_ERROR_)
	"ERROR   ",
	"ERROR   ",
#endif
#if defined(_SD_NO_FILE_DISPLAY_ERR_)
	"ERROR   ",
#endif

#if defined(_ENABLE_RADIO_RDS_)
#if defined(_SWL8809_DISPLAY_)
	//band rds
    "TRAFFI  ",
    "EON TA  ",
    "NO TP   ",
    "TASEEK  ",
    "PISEEK  ",
    "PTYSEK  ",
    "AF ON   ",
    "AF OFF  ",
    "TA ON   ",
    "TA OFF  ",
    "TAPAUS  ",
    "REGLOC  ",
    "REG DX  ",
    "EON ON  ",
    "EONOFF  ",
    "R LONG  ",
    "RSHORT  ",
    " ALL    ",
    " DIP    ",
    "ALARM   ",
    "SWITCH  ",
    " SYNC   ",
    "ASYNC   ",
    "SEARCH  ",
    "PTYOFF  ",
    "MUSIC   ",
    "SPEECH  ",
    "FOUND   ",
    "NOFOUD  ",
    
    /*RDS PTY*/
    " NONE   ",
    " NEWS   ",
    "AFFAIR  ",
    " INFO   ",
    "SPORT   ",
    "EDUCAT  ",
    "DRAMA   ",
    "CULTUR  ",
    "SCIENC  ",
    "VARIED  ",
    "POP  M  ",
    "ROCK M  ",
    "EASY M  ",
    "LIGHT   ",
    "CLASSI  ",
    "OTHER   ",
    "WEATHE  ",
    "FINANC  ",
    "CHILDR  ",
    "SOCIAL  ",
    "RELIGI  ",
    "PHONE   ",
    "TRAVEL  ",
    "LEISUR  ",
    " JAZZ   ",
    "COUNTR  ",
    "NATION  ",
    "OLDIES  ",
    "FOLK M  ",
    "DOCUME  ",
    " TEST   ",
    " ALARM  ",
    
    /*RBDS PTY*/
    " NONE   ",
    " NEWS   ",
    "INFORM  ",
    "SPORTS  ",
    " TALK   ",
    " ROCK   ",
    " ROCK   ",
    " ADLT   ",
    " SOFT   ",
    "TOP 40  ",
    "COUNTR  ",
    "OLDIES  ",
    " SOFT   ",
    "NOSTAL  ",
    " JAZZ   ",
    "CLASSI  ",
    " R_B    ",
    " SOFT   ",
    "LANGUA  ",
    " MUSC   ",
    " TALK   ",
    "PERSNL  ",
    "PUBLIC  ",
    "COLLEG  ",
    "UNDEF1  ",
    "UNDEF2  ",
    "UNDEF3  ",
    "UNDEF4  ",
    "UNDEF5  ",
    "WEATHE  ",
    " TEST   ",
    " ALERT  ",

#else
	//band rds
	"TRAFF   ",
	"EONTA   ",
	"NO TP   ",
#if defined(_SL6802_SPECIA_DISPLAY_)
    "TASEK   ",
	"PISEK   ",
	"PTYSK   ",
	"AFKPL   ",
	"AFACK   ",
	"TAKPL   ",
	"TAACK   ",
#else	
	"TASEK   ",
	"PISEK   ",
	"PTYSK   ",
	"AF ON   ",
	"AF OFF  ",
	"TA ON   ",
	"TA OFF  ",
#endif	
	"TAPAU   ",
#if defined(SPECIAL_REG_DISP)
	"REGON   ",
	"REGOFF  ",
#else
	"REGLOC  ",
	"REGDX   ",
#endif	
	"EONON   ",
	"EONOFF  ",
	"RLONG   ",
	"RSHOT   ",
	" ALL    ",
	" DIP    ",
	"ALARM   ",
	"SWITCH  ",
	" SYNC   ",
	"ASYNC   ",
#if defined(ENABLE_TA_VOL_SET)
	"TA      ",
#endif
	"SEARCH  ",
	"PTYOFF  ",
#if defined(_SUPPORT_SPECIAL_DISPLAY_)
    "MUZIK   ",
#else	
	"MUSIC   ",
#endif	
	"SPEECH  ",
	"FOUND   ",
	"NOFOD   ",

	/*RDS PTY*/
	"NONE    ",
	"NEWS    ",
	"AFFAI   ",
	"INFO    ",
#if defined(_SUPPORT_SPECIAL_DISPLAY_)
    "SPOR    ",
#else
	"SPORT   ",
#endif	
	"EDUCA   ",
	"DRAMA   ",
	"CULTU   ",
	"SCIEN   ",
	"VARIE   ",
	"POP M   ",
	"ROC M   ",
	"EASYM   ",
	"LIGHT   ",
	"CLASS   ",
	"OTHER   ",
	"WEATH   ",
	"FINAN   ",
	"CHILD   ",
	"SOCIA   ",
	"RELIG   ",
	"PHONE   ",
	"TRAVEL  ",
	"LEISU   ",
	"JAZZ    ",
	"COUNT   ",
	"NATIO   ",
	"OLDIE   ",
	"FOLKM   ",
	"DOCUM   ",
	"TEST    ",
	"ALARM   ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
#endif
#endif

#elif defined(_PS329a_SPECIAL_DISPLAY_)
#if 1
#if 1
    //system
	"       ",
	"   HELL",
	"   BYE ",
	
   //radio
	"   TUNE",
	"   ST  ",
	"   MONO",// 5
   
#if defined(_GX6211_LOC_DISPLAY_)
    "LOC ON ",
    "LOC OFF",
#else
	"   LOCA",
	"   DX  ",
#endif 
    "   MANU",
	"   AUTO",
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "  EUR  ",
    "  USA  ",
#if defined(_SUPPORT_THREE_AREA_)
    " LATIN ",
#endif  
#else
    "   EUR ",//10
    "   USA ",
#endif
   
#if defined(DAB_MODLE)
	   //DAB
	"  DAB  ",
#endif
#if defined(_SUPPORT_TAPE_)
	" TAPE  ",
	" TAPE  ",
	" TAPE>>",
	" <<TAPE",
#endif
#if defined(EASYESY_APP_ONOFF)
	" APP ON ",
	" APP OFF",
#endif
     //music
	"   USB ",
	" SD/MMC",
	"   LOAD",
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "RPT ALL",//15
    "RPT ONE",
    "RPT DIR",
    "RPT OFF",
    "RDM ON ",
    "RDM OFF",//20
    "INT ON ",
    "INT OFF",
#else

#if defined(_RPT_ALL_DISPLAY_)
	"RPT OFF",//15
#else
	"RPT ALL",//15
#endif
	"RPT ONE",
	"RPT DIR",
	"RPT OFF",
	"RDM ON ",
	"RDM OFF",//20
	"INT ON ",
	"INT OFF",
#endif
#if defined(_SUPPORT_DISC_)
	"   DISC",
	"   EJEC",
#if defined(NEED_NO_DISC_DISP)
	"NO DISC",
#endif
#endif

	//linein
	"AUX IN ",

	//bluetooth
#if defined(_USE_FYDVD_BT_MODE_)
	"BT PHON",
#else
    "   BT  ",
#endif
	" BT ON ",//25
	" BT OFF",
#if defined(_USE_FYDVD_BT_MODE_)
	"NO LINK",
#else
	"   CONN",
#endif	
	"   PHON",
	"BT AUDI",// BT PLAY
	"   PHON",//BT PAUSE// 30	
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "RINGING",
    "CALLING",
#else
	"   RING",
	"   CALL",
#endif
	" BT HFP",
	"BT PHON",
	"   ERRO",// 35
	"   REDI",
	"DIA -> ",
	
#if defined(_SUPPORT_CALL_RECORDS_)
	"NO CALL",
#endif
	
#if defined(_GX6211_SPECIAL_DISPLAY_)
    //upgrade
    "UPGRADE",
    "SUCCEED",
#else
    "   GRAD",
    "   SUCC",
#endif
	"   FAIL",
	
    //other
	"VOL    ",
	"   MUTE",
	"   MUTE",
	"LOU ON ",
	"LOU OFF",//45
	" EQ OFF",
	"   FLAT",
	"   CLASS",
	"   POP ",
	"   ROCK",//50
	"   JAZZ",
	
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "BAS    ",
    "TRE    ",
    "BAL    ",
    "FAD    ",//55
#else
	"BAS	",
	"TRE	",
	"BAL    ",
	"FAD    ",//55
#endif
    "SUB ON ",
	"SUB OFF",
	"BEE ON ",
	"BEE OFF",//59
#if REC_EN
	"RECORD ",
#if defined(_ENABLE_RECORD_DISPLAY_)
	"PAUSE  ",
#endif
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUB    ",
#endif
	
#if defined(AUDIO_SUB_LEVEL_SET)
	"CUT    ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "   BLUE",
    "   GREE",
    "   CYAN",
    "   RED ",
	"   PURP",
    "   YELL",
    "   WHIT",
    "   AUTO",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFFIC",
	"EON TA ",
	"NOTP\\TA",
	"TA SEEK",
	"PI SEEK",
	"PTYSEEK",
	" AF ON ",
	" AF OFF",
	" TA ON ",
	" TA OFF",
	"TAPAUSE",
	"REG LOC",
	"REG DX ",
	"EON ON ",
	"EON OFF",
#if defined(_ENABLE_RDS_DISPLAY_)
	"RETURNL",
	"RETURNS",
#else
	"R LONG ",
	"R SHORT",
#endif
	"MASKALL",
	"MASKDIP",
#if defined(_ENABLE_RDS_DISPLAY_)
	"TA ALAR",
	"TA SEEK",
#else
	" ALARM ",
	" SWITCH",
#endif
	"  SYNC ",
	" ASYNC ",
	" SEARCH",
	"PTY OFF",
	" MUSIC ",
	" SPEECH",
	" FOUND ",
	"NOFOUND",

	/*RDS PTY*/
	" NONE  ",
	"N E W S",
	"AFFAIRS",
	"I N F O",
	" SPORT ",
	"EDUCATE",
	" DRAMA ",
	"CULTURE",
	"SCIENCE",
	"VARIED ",
	"POP  M ",
	"ROCK M ",
	"EASY M ",
	"LIGHT M",
	"CLASSIC",
	"OTHER M",
	"WEATHER",
	"FINANCE",
	"CHILDRE",
	"SOCIAL ",
	"RELIGIO",
	"PHONEIN",
	"TRAVEL ",
	"LEISURE",
	" JAZZ  ",
	"COUNTRY",
	"NATIONM",
	"OLDIES ",
	"FOLK M ",
	"DOCUMEN",
	" TEST  ",
	" ALARM ",

	/*RBDS PTY*/
	"  NONE ",
	"  NEWS ",
	" INFORM",
	" SPORTS",
	"  TALK ",
	"  ROCK ",
	"CLSROCK",
	"ADLTHIT",
	"SOFTRCK",
	" TOP 40",
	"COUNTRY",
	"OLDIES ",
	" SOFT  ",
	"NOSTALG",
	" JAZZ  ",
	"CLASSIC",
	"  R_B  ",
	"SOFTR_B",
	"LANGUAG",
	"RELMUSC",
	"RELTALK",
	"PERSNLT",
	"PUBLIC ",
	"COLLEGE",
	"UNDEF1 ",
	"UNDEF2 ",
	"UNDEF3 ",
	"UNDEF4 ",
	"UNDEF5 ",
	"WEATHER",
	" TEST  ",
	" ALERT ",
#endif
#endif

#else
    //system
	"       ",
	"   HELL",
	"   BYE ",
	
    //radio
	"   TUNE",
	"   ST  ",
	"   MONO",// 5
   
#if defined(_GX6211_LOC_DISPLAY_)
    "LOC ON ",
    "LOC OFF",
#else
	" LOCAL ",
	"   DX  ",
#endif 
    " MANUAL",
	"  AUTO ",
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "  EUR  ",
    "  USA  ",
#if defined(_SUPPORT_THREE_AREA_)
    "   LATI",
#endif
#else
    "   EUR ",//10
    "   USA ",
#endif
   
#if defined(DAB_MODLE)
	   //DAB
	"   DAB ",
#endif
#if defined(_SUPPORT_TAPE_)
	"   TAPE",
	"   TAPE",
	" TAPE>>",
	" <<TAPE",
#endif
#if defined(EASYESY_APP_ONOFF)
	"APP ON ",
	"APP OFF",
#endif
    //music
	"   USB ",
	" SD/MMC",
	"   LOAD",
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "RPT ALL",//15
    "RPT ONE",
    "RPT DIR",
    "RPT OFF",
    "RDM ON ",
    "RDM OFF",//20
    "INT ON ",
    "INT OFF",
#else

#if defined(_RPT_ALL_DISPLAY_)
	"RPT OFF",//15
#else
	"RPT ALL",//15
#endif
	"RPT ONE",
	"RPT DIR",
	"RPT OFF",
	"RDM ON ",
	"RDM OFF",//20
	"INT ON ",
	"INT OFF",
#endif

#if defined(_SUPPORT_DISC_)
	"   DISC",
	"   EJEC",
#if defined(NEED_NO_DISC_DISP)
	"NO DISC",
#endif
#endif

	//linein
	"AUX IN ",

	//bluetooth
#if defined(_USE_FYDVD_BT_MODE_)
	"BT PHON",
#else
    "   BT  ",
#endif
	" BT ON ",//25
	" BT OFF",
#if defined(_USE_FYDVD_BT_MODE_)
	"NO LINK",
#else
	"   CONN",
#endif	
	"   PHON",
	"BT AUDI",// BT PLAY
	"   PHON",//BT PAUSE// 30	
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "RINGING",
    "CALLING",
#else
	"   RING",
	"   CALL",
#endif
	" BT HFP",
	"BT PHON",
	"   ERRO",// 35
	"   REDI",
	"DIA -> ",
	
#if defined(_SUPPORT_CALL_RECORDS_)
	"NO CALL",
#endif
    //upgrade
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "UPGRADE",
    "SUCCEED",
#else
    "   GRAD",
    "   SUCC",
#endif
	"   FAIL",
	
    //other
	"VOL    ",
	"   MUTE",
	"   MUTE",
	"LOU ON ",
	"LOU OFF",//45
	" EQ OFF",
	"   FLAT",
	"   CLAS",
	"   POP ",
	"   ROCK",//50
	"   JAZZ",
	
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "BAS    ",
    "TRE    ",
    "BAL    ",
    "FAD    ",//55
#else
	"BAS	",
	"TRE	",
	"BAL	",
	"FAD	",//55
#endif
    "SUB ON ",
	"SUB OFF",
	"BEE ON ",
	"BEE OFF",//59
#if REC_EN
	"RECORD ",
#if defined(_ENABLE_RECORD_DISPLAY_)
	"   PAUS",
#endif
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUB    ",
#endif
	
#if defined(AUDIO_SUB_LEVEL_SET)
	"CUT    ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "   BLUE",
    "   GREE",
    "   CYAN",
    "   RED ",
	"   PURP",
    "   YELL",
    "   WHIT",
    "   AUTO",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFFIC",
	"EON TA ",
	"NOTP\\TA",
	"TA SEEK",
	"PI SEEK",
	"PTYSEEK",
	" AF ON ",
	" AF OFF",
	" TA ON ",
	" TA OFF",
	"TAPAUSE",
	"REG LOC",
	"REG DX ",
	"EON ON ",
	"EON OFF",
#if defined(_ENABLE_RDS_DISPLAY_)
	"RETURNL",
	"RETURNS",
#else
	"R LONG ",
	"R SHORT",
#endif
	"MASKALL",
	"MASKDIP",
#if defined(_ENABLE_RDS_DISPLAY_)
	"TA ALAR",
	"TA SEEK",
#else
	" ALARM ",
	" SWITCH",
#endif
	"  SYNC ",
	" ASYNC ",
	" SEARCH",
	"PTY OFF",
	" MUSIC ",
	" SPEECH",
	" FOUND ",
	"NOFOUND",

	/*RDS PTY*/
	" NONE  ",
	"N E W S",
	"AFFAIRS",
	"I N F O",
	" SPORT ",
	"EDUCATE",
	" DRAMA ",
	"CULTURE",
	"SCIENCE",
	"VARIED ",
	"POP  M ",
	"ROCK M ",
	"EASY M ",
	"LIGHT M",
	"CLASSIC",
	"OTHER M",
	"WEATHER",
	"FINANCE",
	"CHILDRE",
	"SOCIAL ",
	"RELIGIO",
	"PHONEIN",
	"TRAVEL ",
	"LEISURE",
	" JAZZ  ",
	"COUNTRY",
	"NATIONM",
	"OLDIES ",
	"FOLK M ",
	"DOCUMEN",
	" TEST  ",
	" ALARM ",

	/*RBDS PTY*/
	"  NONE ",
	"  NEWS ",
	" INFORM",
	" SPORTS",
	"  TALK ",
	"  ROCK ",
	"CLSROCK",
	"ADLTHIT",
	"SOFTRCK",
	" TOP 40",
	"COUNTRY",
	"OLDIES ",
	" SOFT  ",
	"NOSTALG",
	" JAZZ  ",
	"CLASSIC",
	"  R_B  ",
	"SOFTR_B",
	"LANGUAG",
	"RELMUSC",
	"RELTALK",
	"PERSNLT",
	"PUBLIC ",
	"COLLEGE",
	"UNDEF1 ",
	"UNDEF2 ",
	"UNDEF3 ",
	"UNDEF4 ",
	"UNDEF5 ",
	"WEATHER",
	" TEST  ",
	" ALERT ",
#endif
#endif

#elif defined(_SEVEN_CHAR_DISPLAY_)
    //system
	"       ",
#if defined(_FY2010_LCD_DISPLAY_)||defined(_ZSBY5209_SPECIAL_DISPLAY_)
    " HELLO ",
    "BYEBYE ",
#else
	"WELCOME",
	"GOODBYE",
#endif
	
   //radio
#if defined(_ZSBY5209_SPECIAL_DISPLAY_)
	" TUNE  ",
#else
	" RADIO ",
#endif
	" STEREO",
	"  MONO ",// 5
   
#if defined(_GX6211_LOC_DISPLAY_)
    "LOC ON ",
    "LOC OFF",
#else
	" LOCAL ",
	"   DX  ",
#endif 
    "MANUAL ",
	" AUTO  ",
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "  EUR  ",
    "  USA  ",
#if defined(_SUPPORT_THREE_AREA_)
    " LATIN ",
#endif  
#else
    "  EUR  ",//10
    "  USA  ",
#endif
   
#if defined(DAB_MODLE)
	   //DAB
	"  DAB  ",
#endif
#if defined(_SUPPORT_TAPE_)
	" TAPE  ",
	" TAPE  ",
	" TAPE>>",
	" <<TAPE",
#endif
#if defined(EASYESY_APP_ONOFF)
#if defined(_FY2010_LCD_DISPLAY_)
    "APPON  ",
    "APP OFF",
#else
	"APP ON ",
	"APP OFF",
#endif
#endif
     //music
	"  USB  ",
#if defined(_FY2010_LCD_DISPLAY_)
	"SD/MMC ",
#else
	" SD/MMC",
#endif
	"  LOAD ",
#if defined(_FY2010_LCD_DISPLAY_)
    "RPTALL",//15
    "RPTONE",
    "RPTDI]",
    "RPTOFF",
    "RDMON ",
    "RDMOFF",//20
    "INTON ",
    "INTOFF",
#else

#if defined(_RPT_ALL_DISPLAY_)
	"RPT OFF",//15
#else
	"RPT ALL",//15
#endif
	"RPT ONE",
	"RPT DIR",
	"RPT OFF",
	"RDM ON ",
	"RDM OFF",//20
	"INT ON ",
	"INT OFF",
#endif
#if defined(_SUPPORT_DISC_)
	" DISC  ",
	" EJECT ",
#if defined(NEED_NO_DISC_DISP)
	"NO DISC",
#endif
#endif

	//linein
#if defined(_FY2010_LCD_DISPLAY_)||defined(_ZSBY5209_SPECIAL_DISPLAY_)
	"  AUX  ",
#elif defined(_HT868_LCD_DISPLAY_)
	" AUX   ",
#else
	"AUX IN ",
#endif
#if defined(_SEL_ADD_TIME_SELECT_)
	"CLK 24  ",
	"CLK 12  ",
#endif

	//bluetooth
#if defined(_FY2010_LCD_DISPLAY_)
    "  BT   ",
	"BT ON  ",//25
	"BT OFF ",
	"CONNEC ",
	" PHONE ",
	" A2DP  ",// BT PLAY
	" PHONE ",//BT PAUSE// 30	
	" RING  ",
	" CALL  ",
	"BT HFP ",
	" PHONE ",
	"ERROR  ",// 35
	"REDIAL ",
	"DIAL-> ",
#else
#if defined(_USE_FYDVD_BT_MODE_)
	"BT PHON",
#else
    "  BT   ",
#endif
	" BT ON ",//25
	" BT OFF",
#if defined(_USE_FYDVD_BT_MODE_)
	"NO LINK",
#elif defined(_ZSBY5209_SPECIAL_DISPLAY_)
	"  BT   ",
#else
	"CONNECT",
#endif	
	" PHONE ",
#if defined(_ZSBY5209_SPECIAL_DISPLAY_)
	" A2DP  ",// BT PLAY
#else
	"BTAUDIO",// BT PLAY
#endif
	" PHONE ",//BT PAUSE// 30	
	"RINGING",
	"CALLING",
	" BT HFP",
	" PHONE ",
	" ERROR ",// 35
	" REDIAL",
	"DIAL-> ",
#endif

#if defined(_SUPPORT_CALL_RECORDS_)
	"NO CALL",
#endif
	
#if defined(_FY2010_LCD_DISPLAY_)
    "  UPG  ",
    "SUCED  ",
    " FAIL  ",
#else
    "UPGRADE",
    "SUCCEED",
	"  FAIL ",
#endif
    //other
#if defined(_FY2010_LCD_DISPLAY_)
    "VOL    ",
    "  MUTE ",
    "MUTEOFF",
    "LOU O[ ",
    "LOU OFF",//45
    "EQ OFF ",
    " FLAT  ",
    "CLASS  ",
    " POP   ",
    " ROCK  ",//50
    " JAZZ  ",
#else
	"VOL    ",
	"MUTE ON",
	"MUTEOFF",
	"LOUD ON",
	"LOUDOFF",//45
	" EQ OFF",
	"  FLAT ",
	"CLASSIC",
	"  POP  ",
	"  ROCK ",//50
	"  JAZZ ",
#endif

#if defined(_GX6211_SPECIAL_DISPLAY_)||defined(_FY2010_LCD_DISPLAY_)||defined(_HT868_LCD_DISPLAY_)
    "BAS    ",
    "TRE    ",
    "BAL    ",
    "FAD    ",//55
#else
	" BAS	",
	" TRE	",
	" BAL	",
	" FAD	",//55
#endif
#if defined(_SUPPORT_SUBWOOFER2_)
   "SUB1 O[",
   "SUB1OFF",
#else
    "SUB ON ",
	"SUB OFF",
#endif
	"BEEP ON",
	"BEEPOFF",//59
#if REC_EN
	"RECORD ",
#if defined(_ENABLE_RECORD_DISPLAY_)
	"PAUSE  ",
#endif
#endif

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUB    ",
#endif

#if defined(_SUPPORT_SUBWOOFER3_)
    "SUB2 O[",
    "SUB2OFF",
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	"CUT    ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
#if defined(_FY2010_LCD_DISPLAY_)
    " BLUE  ",
    "GREEN  ",
    " CYAN  ",
    "  RED  ",
	"PURPLE ",
    " YELL  ",
    "WHITE  ",
    " AUTO  ",
#else
    " BLUE	",
    " GREEN ",
    " CYAN	",
    "  RED	",
    "PURPLE ",
    "YELLOW	",
    " WHITE	",
    " AUTO	",
#endif
#endif

#if defined(_ENABLE_TIME_DISP_)
	"DISP ON ",
	"DISP OFF",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFFIC",
	"EON TA ",
	"NOTP\\TA",
	"TA SEEK",
	"PI SEEK",
	"PTYSEEK",
	" AF ON ",
	" AF OFF",
	" TA ON ",
	" TA OFF",
	"TAPAUSE",
	"REG LOC",
	"REG DX ",
	"EON ON ",
	"EON OFF",
#if defined(_ENABLE_RDS_DISPLAY_)
	"RETURNL",
	"RETURNS",
#else
	"R LONG ",
	"R SHORT",
#endif
	"MASKALL",
	"MASKDIP",
#if defined(_ENABLE_RDS_DISPLAY_)
	"TA ALAR",
	"TA SEEK",
#else
	" ALARM ",
	" SWITCH",
#endif
	"  SYNC ",
	" ASYNC ",
	" SEARCH",
	"PTY OFF",
	" MUSIC ",
	" SPEECH",
	" FOUND ",
	"NOFOUND",

	/*RDS PTY*/
	" NONE  ",
	"N E W S",
	"AFFAIRS",
	"I N F O",
	" SPORT ",
	"EDUCATE",
	" DRAMA ",
	"CULTURE",
	"SCIENCE",
	"VARIED ",
	"POP  M ",
	"ROCK M ",
	"EASY M ",
	"LIGHT M",
	"CLASSIC",
	"OTHER M",
	"WEATHER",
	"FINANCE",
	"CHILDRE",
	"SOCIAL ",
	"RELIGIO",
	"PHONEIN",
	"TRAVEL ",
	"LEISURE",
	" JAZZ  ",
	"COUNTRY",
	"NATIONM",
	"OLDIES ",
	"FOLK M ",
	"DOCUMEN",
	" TEST  ",
	" ALARM ",

	/*RBDS PTY*/
	"  NONE ",
	"  NEWS ",
	" INFORM",
	" SPORTS",
	"  TALK ",
	"  ROCK ",
	"CLSROCK",
	"ADLTHIT",
	"SOFTRCK",
	" TOP 40",
	"COUNTRY",
	"OLDIES ",
	" SOFT  ",
	"NOSTALG",
	" JAZZ  ",
	"CLASSIC",
	"  R_B  ",
	"SOFTR_B",
	"LANGUAG",
	"RELMUSC",
	"RELTALK",
	"PERSNLT",
	"PUBLIC ",
	"COLLEGE",
	"UNDEF1 ",
	"UNDEF2 ",
	"UNDEF3 ",
	"UNDEF4 ",
	"UNDEF5 ",
	"WEATHER",
	" TEST  ",
	" ALERT ",
#endif
#elif defined(_YS_SPECIAL_DISP_)
	//system
	"        ",// 0
	" WELCOME",
	"GOOD-BYE",
	//radio
	" RADIO  ",
	" STEREO ",
	"  MONO  ",// 5
	" LOCAL  ",
	"   DX   ",
	" MANUAL ",
	"  AUTO  ",
    "AREA EUR",//10
    "AREA USA",

   
#if defined(DAB_MODLE)
	   //DAB
	"  DAB   ",
#endif
#if defined(_SUPPORT_TAPE_)
	" TAPE   ",
	"   TAPE ",
	" TAPE>> ",
	" <<TAPE ",
#endif
#if defined(EASYESY_APP_ONOFF)
	" APP ON  ",
	" APP OFF ",
#endif
	//music
	"   USB  ",
	" SD/MMC ",
	"  LOAD  ",

	" RPT ALL",//15
	" RPT ONE",
	" RPT DIR",
	" RPT OFF",
	" RDM ON ",
	" RDM OFF",//20
	" INT ON ",
	" INT OFF",

#if defined(_SUPPORT_DISC_)
	"  DISC  ",
	"  EJECT ",
#if defined(NEED_NO_DISC_DISP)
	"NO DISC ",
#endif
#endif

	//linein
	" AUX IN ",
#if defined(_SEL_ADD_TIME_SELECT_)
	"TIME 24H",
	"TIME 12H",
#endif
	//bluetooth

	"   BT   ",
	" BT ON  ",//25
	" BT OFF ",
	"CONNECT ",
	" PHONE  ",
	"BT AUDIO",// BT PLAY
	" PHONE  ",//BT PAUSE// 30
	" RINGING",
	" CALLING",
	" BT HFP ",
	"BT PHONE",
	" ERROR  ",// 35
	" REDIAL ",
	"DIAL->  ",
#if defined(_SUPPORT_CALL_RECORDS_)
	"NO CALL ",
#endif
	//upgrade
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "UPGRADE ",
    "SUCCEED ",
#else
	" UPGRADE",
	" SUCCEED",
#endif
	"  FAIL  ",//40

	//others
	" VOL	 ",
	"  MUTE  ",
	"MUTE OFF",
	"LOUD ON ",
	"LOUD OFF",//45
	" EQ OFF ",
	"  FLAT  ",
	"  CLASS ",
	"   POP  ",
	"  ROCK  ",//50
	"  JAZZ  ",
	" BAS	 ",
	" TRE	 ",
	" BAL	 ",
	" FAD	 ",//55
#if defined(_CHANGE_DEF_VOL_)
    "P-VOL   ",
#endif
	" SUB ON ",
	" SUB OFF",
	"BEEP ON ",
	"BEEP OFF",//59

#if REC_EN
	" RECORD ",
#if defined(_ENABLE_RECORD_DISPLAY_)
	" PAUSE  ",
#endif
#endif

#if defined(_FY7000A_SPECIAL_DISPLAY_)
#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUBVOL  ",
#endif
	
#if defined(_SUPPORT_SUBWOOFER3_)
    "SUB2 ON ",
    "SUB2 OFF",
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	"LPF     ",
#endif

#else

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	" SUB    ",
#endif
	
#if defined(AUDIO_SUB_LEVEL_SET)
	"CUT     ",
#endif

#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "  BLUE  ",
    "  GREEN ",
    "  CYAN  ",
    "  RED   ",
	" PURPLE ",
    " YELLOW ",
    "  WHITE ",
    "  AUTO  ",
#endif

#if defined(_SUPPORT_10_EQ_FUNTION_)
	"60HZ    ",
	"150HZ   ",
	"400HZ   ",
	"1KHZ    ",
	"2KHZ    ",
	"3KHZ    ",
	"7KHZ    ",
	"10KHZ   ",
	"15KHZ   ",
	"20KHZ   ",
#endif

#if defined(SUPPORT_LED_ADJUST)
	"LED  LOW",
	"LED  MID",
	"LED HIGH",
#endif

#if defined(_ADJUST_A_B_SPK_)
	"A_SPK ON",
	"A_SPKOFF",
	"B_SPK ON",
	"B_SPKOFF",
#endif

#if defined(SUPPORT_ALARM_FUNC_)
	"ALARM ON",
	"ALARMOFF",
	"  ALARM ",
#endif

#if defined(_ENABLE_TIME_DISP_)
	"DISP ON ",
	"DISP OFF",
#endif

#if defined(_SUPPORT_MIC_OK_)
	" MIC IN ",
#endif

#if defined(_SUPPORT_CAN_MODEL_)
	"CAN MENU",
#endif

#if defined(_ENABLE_PASSWORD_INPUT_)
	"PWD ----",
	" ERROR  ",
#endif

#if defined(_SUPPORT_CAN_UART_)
	"ERR     ",
	"SP      ",
	"MI      ",
	"SOC     ",
	"DIR     ",
	"P1      ",
	"P2      ",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFFIC ",
	" EON TA ",
	"NO TP\\TA",
	"TA SEEK ",
	"PI SEEK ",
	"PTY SEEK",
	" AF ON  ",
	" AF OFF ",
	" TA ON  ",
	" TA OFF ",
	"TA PAUSE",
	" REG LOC",
	" REG DX ",
	" EON ON ",
	" EON OFF",
#if defined(_ENABLE_RDS_DISPLAY_)
	"RETURN L",
	"RETURN S",
#else
	" R LONG ",
	" R SHORT",
#endif
	"MASK ALL",
	"MASK DIP",
#if defined(_ENABLE_RDS_DISPLAY_)
	"TA ALARM",
	"TA SEEK ",
#else
	" ALARM  ",
	" SWITCH ",
#endif
	"  SYNC  ",
	" ASYNC  ",
	" SEARCH ",
	"PTY OFF ",
	" MUSIC  ",
	" SPEECH ",
	" FOUND  ",
	"NO FOUND",

	/*RDS PTY*/
	"  NONE  ",
	" N E W S",
	" AFFAIRS",
	" I N F O",
	"  SPORT ",
	" EDUCATE",
	"  DRAMA ",
	" CULTURE",
	" SCIENCE",
	" VARIED ",
	" POP  M ",
	" ROCK M ",
	" EASY M ",
	" LIGHT M",
	"CLASSICS",
	" OTHER M",
	"WEATHER ",
	"FINANCE ",
	"CHILDREN",
	" SOCIAL ",
	"RELIGION",
	"PHONE IN",
	" TRAVEL ",
	"LEISURE ",
	"  JAZZ  ",
	"COUNTRY ",
	"NATION M",
	" OLDIES ",
	" FOLK M ",
	"DOCUMENT",
	"  TEST  ",
	"  ALARM ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
#endif

#elif defined(_XM003_SPECIAL_DISPLAY_)
	//system
	"        ",
	" HELLO  ",
	"BYEBYE  ",
	
	//radio
	"TU/E    ",
	"STEREO  ",
	" MONO   ",
	"LOCAL   ",
	"  DH    ",
	"MA<UAL  ",
	"AUTO    ",
    "   EUR  ",
    "   USA  ",
    
#if defined(EASYESY_APP_ONOFF)
	"APP ON  ",
	"APP OFF ",
#endif

	//music
	"   USB  ",
	"    SD  ",
	"  LOAD  ",
	"RPTALL  ",
	"RPTONE  ",
	"RPTDIR  ",
	"RPTOFF  ",
	"RDMON   ",
	"RDMOFF  ",
	"INTON   ",
	"INTOFF  ",

	//linein
	" AUX    ",

	//bluetooth
	"   A2DP ",
	" BT ON  ",
	" BT OFF ",
	"   A2DP ",//CONNECT
	"   A2DP ",
	"   A2DP ",// BT PLAY
	"   A2DP ",//BT PAUSE
	"RING    ",
	"CALL    ",
	"BT HFP  ",
	"   A2DP ",
	"ERROR   ",
	"CALL    ",
	"CALL    ",

	//upgrade
	"  UPG   ",
	" SUCS   ",
	"FAIL    ",

	//others
	"VOL     ",
	"MUTE    ",
	"MUTE OFF",
	"LD--ON  ",
	"LD--OFF ",
	"DSP OFF ",
	"DSP ON  ",
	"DSP ON  ",
	"DSP ON  ",
	"DSP ON  ",
	"DSP ON  ",
	"BAS     ",
	"TRE     ",
	"BAL     ",
	"FAD     ",
	"SUBON   ",
	"SUBOFF  ",
	"BEPON   ",
	"BEPOFF  ",
	
#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
	" BLUE   ",
	"GREEN   ",
	" CYAN   ",
	" RED	 ",
	" PURP   ",
	"YELOW   ",
	"WHITE   ",
	" AUTO   ",
#endif

#if defined(_ENABLE_SUB_DETECT_)
	"RCA ON  ",
	"RCA OFF ",
#endif

#if defined(_XM003_MODE_DISPLAY_)
	"NO  SRC ",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFF   ",
	"EONTA   ",
	"NO TP   ",
	"TASEK   ",
	"PISEK   ",
	"PTYSK   ",
	"AF ON   ",
	"AF OFF  ",
	"TA ON   ",
	"TA OFF  ",
	"TAPAU   ",
	"REGLOC  ",
	"REGDX   ",
	"EONON   ",
	"EONOFF  ",
	"RLONG   ",
	"RSHOT   ",
	" ALL    ",
	" DIP    ",
	"ALARM   ",
	"SWITCH  ",
	" SYNC   ",
	"ASYNC   ",
#if defined(ENABLE_TA_VOL_SET)
	"TA      ",
#endif
	"SEARCH  ",
	"PTYOFF  ",
	"MUSIC   ",
	"SPEECH  ",
	"FOUND   ",
	"NOFOD   ",

	/*RDS PTY*/
	"NONE    ",
	"NEWS    ",
	"AFFAI   ",
	"INFO    ",
	"SPORT   ",
	"EDUCA   ",
	"DRAMA   ",
	"CULTU   ",
	"SCIEN   ",
	"VARIE   ",
	"POP M   ",
	"ROC M   ",
	"EASYM   ",
	"LIGHT   ",
	"CLASS   ",
	"OTHER   ",
	"WEATH   ",
	"FINAN   ",
	"CHILD   ",
	"SOCIA   ",
	"RELIG   ",
	"PHONE   ",
	"TRAVEL  ",
	"LEISU   ",
	"JAZZ    ",
	"COUNT   ",
	"NATIO   ",
	"OLDIE   ",
	"FOLKM   ",
	"DOCUM   ",
	"TEST    ",
	"ALARM   ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
#endif

#elif defined(_HKY1032_SPECIAL_DISPLAY_)
	//system
	"        ",// 0
	"    HELL",
	"    BYE ",
	
	//radio
	"    TUNE",
	"     ST ",
	"    MONO",// 5
	"    LOC ",
	"     DX ",
	"    MANU",
	"    AUTO",
    "    EUR ",
    "    USA ",
   
#if defined(EASYESY_APP_ONOFF)
	"    APON",
	"    APOF",
#endif
	//music
	"    USB ",
	"     SD ",
	"    LOAD",

	"     ALL",//15
	"     ONE",
	"     DIR",
	"     OFF",
	"     RDM",
	"     OFF",//20
	"     INT",
	"     OFF",

	//linein
	"    AUX ",
	//bluetooth
	"     BT ",
	"    BTON",//25
	"    BTOF",
	"    CONN",
	"    PHON",
	"    A2DP",// BT PLAY
	"    PHON",//BT PAUSE// 30
	"    RING",
	"    CALL",
	" BT HFP ",
	"    PHON",
	"    ERR ",// 35
	"    REDI",
	"DIAL->  ",
	//upgrade
	"    UPG ",
	"    SUCS",
	"    FAIL",//40

	//others
	"    V   ",
	"    MUTE",
	"MUTE OFF",
	"LOUD ON ",
	"LOUD OFF",//45
	" EQ OFF ",
	"  FLAT  ",
	" CLASSIC",
	"   POP  ",
	"  ROCK  ",//50
	"  JAZZ  ",
	" BAS	 ",
	" TRE	 ",
	" BAL	 ",
	" FAD	 ",//55
	" SUB ON ",
	" SUB OFF",
	"BEEP ON ",
	"BEEP OFF",//59

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFF   ",
	"EONTA   ",
	"NO TP   ",
	"TASEK   ",
	"PISEK   ",
	"PTYSK   ",
	"AF ON   ",
	"AF OFF  ",
	"TA ON   ",
	"TA OFF  ",
	"TAPAU   ",
	"REGLOC  ",
	"REGDX   ",
	"EONON   ",
	"EONOFF  ",
	"RLONG   ",
	"RSHOT   ",
	" ALL    ",
	" DIP    ",
	"ALARM   ",
	"SWITCH  ",
	" SYNC   ",
	"ASYNC   ",
	"SEARCH  ",
	"PTYOFF  ",
	"MUSIC   ",
	"SPEECH  ",
	"FOUND   ",
	"NOFOD   ",

	/*RDS PTY*/
	"NONE    ",
	"NEWS    ",
	"AFFAI   ",
	"INFO    ",
	"SPORT   ",
	"EDUCA   ",
	"DRAMA   ",
	"CULTU   ",
	"SCIEN   ",
	"VARIE   ",
	"POP M   ",
	"ROC M   ",
	"EASYM   ",
	"LIGHT   ",
	"CLASS   ",
	"OTHER   ",
	"WEATH   ",
	"FINAN   ",
	"CHILD   ",
	"SOCIA   ",
	"RELIG   ",
	"PHONE   ",
	"TRAVEL  ",
	"LEISU   ",
	"JAZZ    ",
	"COUNT   ",
	"NATIO   ",
	"OLDIE   ",
	"FOLKM   ",
	"DOCUM   ",
	"TEST    ",
	"ALARM   ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
#endif

#elif defined(_SL6812A_SPECIAL_DISPLAY_)//十个米吧显示
	"          ",// 0
	" WELCOME  ",
	" GOOD-BYE ",

	//radio
	"  RADIO   ",
	"  STEREO  ",
	"   MONO   ",// 5
#if defined(_SL6812_SPECIAL_DISPLAY_)
    "  LOCAL   ",
    " DISTANT  ",
#else
	"  LOCAL   ",
	"    DX    ",
#endif
	"  MANUAL  ",
	"   AUTO   ",
    " AREA EUR ",//10
    " AREA USA ",

#if defined(DAB_MODLE)
	   //DAB
	"   DAB    ",
#endif
#if defined(EASYESY_APP_ONOFF)
	" APP  ON   ",
	" APP  OFF  ",
#endif

	//music
	"   USB    ",
	"  SD/MMC  ",
	"   LOAD   ",
#if defined(_SL6812_SPECIAL_DISPLAY_)
	" RPT OFF  ",//15
	" RPT FILE ",
	"RPT FOLDER",
	" RPT OFF  ",
	" RDM ALL  ",
	" RDM OFF  ",//20
	" SCN ON   ",
	" SCN OFF  ",
#else
	" RPT  ALL ",//15
	" RPT  ONE ",
	" RPT  DIR ",
	" RPT  OFF ",
	" RDM  ON  ",
	" RDM  OFF ",//20
	" INT  ON  ",
	" INT  OFF ",
#endif
	//linein
	" AUX  IN  ",
#if defined(_SEL_ADD_TIME_SELECT_)
	"CLOCK  24 ",
	"CLOCK  12 ",
#endif

#if defined(_SL6812A_BT_DISPLAY_)
    "BT  MUSIC ",
    " BT  ON   ",//25
    " BT  OFF  ",
    
    "BT  MUSIC ",
    "BT  MUSIC ",
    "BT  MUSIC ",// BT PLAY
    "   PAUSE  ",//BT PAUSE// 30
    " CALL  IN ",
    " CALL OUT ",
    " BT  HFP  ",
    "BT  MUSIC ",
    "  ERROR   ",// 35
    "  REDIAL  ",
    " DIAL->   ",
#else
	"     BT   ",
	" BT  ON   ",//25
	" BT  OFF  ",

	" CONNECT  ",
	"  PHONE   ",
	" BT AUDIO ",// BT PLAY
	"  PHONE   ",//BT PAUSE// 30
	" RINGING  ",
	" CALLING  ",
	" BT  HFP  ",
	" BT PHONE ",
	"  ERROR   ",// 35
	"  REDIAL  ",
	" DIAL->   ",
#endif

#if defined(HANGUP_DISP_HANGUP)
	" END  CALL",
#endif

#if defined(_ENABLE_BT_AUDIO_DISP_)
	"NEXT TRACK",
	"PREV TRACK",
#endif

#if defined(_SL6812A_MODE_FUNTION_)
	" NO MEDIA ",
#endif

	" UPGRADE  ",
	" SUCCEED  ",
	"   FAIL   ",//40

	//others
	"  VOL     ",
	"   MUTE   ",
	" MUTE OFF ",
	" LOUD  ON ",
	" LOUD  OFF",//45
	" EQ  OFF  ",
	"   FLAT   ",
	" CLASSIC  ",
	"    POP   ",
	"   ROCK   ",//50
	"   JAZZ   ",
	"BASS      ",
	"TRED      ",
	"BAL       ",
	"FAD       ",//55
#if defined(_ENABLE_TALK_VOL_)
#if defined(_SL6812_SPECIAL_DISPLAY_)
	"BTVOL     ",
#else
	"TALK      ",
#endif
#endif
	" SUB  ON  ",
	" SUB  OFF ",
	"BEEP  ON  ",
	"BEEP  OFF ",//59

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "   BLUE   ",
    "  GREEN   ",
    "   CYAN   ",
    "   RED    ",
	"  PURPLE  ",
    "  YELLOW  ",
    "   WHITE  ",
    "   AUTO   ",
#endif

#if defined(_ENABLE_DEFAULT_CONTROL_)
	"S-VOL     ",
#endif

#if defined(_ENABLE_RDS_ON_OFF_)
	" RDS  ON  ",
	" RDS  OFF ",
#endif

#if defined(_SUPPORT_PASSWORD_DISPLAY_)
    " PWD **** ",
    "ERROR  01 ",
    "ERROR  02 ",
    "ERROR  03 ",
    "ERROR  04 ",
    "ERROR  05 ",
#endif

#if defined(_SL6812_KEY_FUNTION_)
	"   SPM    ",
#endif

#if defined(_ENABLE_PASSWORD_SET_)
	" PWD  SET ",
	" PWD ---- ",
#endif

#if defined(_PASSWORD_TRUE_DISPLAY_)
	" CORRECT  ",
#endif

#if defined(_ENABLE_PLAY_ERROR_)
	"USB REMOVE",
	"SD  REMOVE",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	" TRAFFIC  ",
	"  EON TA  ",
	" NO TP\TA ",
	" TA SEEK  ",
	" PI SEEK  ",
	" PTY SEEK ",
	" AF  ON   ",
	" AF  OFF  ",
	" TA  ON   ",
	" TA  OFF  ",
	"TA  PAUSE ",
	"  REG ON  ",//REG LOC
	"  REG OFF  ",//REG DX
	" EON  ON  ",
	" EON  OFF ",
	" R  LONG  ",
	" R  SHORT ",
	"MASK  ALL ",
	"MASK  DIP ",
	"  ALARM   ",
	"  SWITCH  ",
	"   SYNC   ",
	"  ASYNC   ",
	"  SEARCH  ",
	" PTY  OFF ",
	"  MUSIC   ",
	"  SPEECH  ",
	"  FOUND   ",
	" NO  FOUND",

	/*RDS PTY*/
	"   NONE   ",
	" N E W S  ",
	" AFFAIRS  ",
	" I N F O  ",
	"  SPORT   ",
	" EDUCATE  ",
	"  DRAMA   ",
	" CULTURE  ",
	" SCIENCE  ",
	" VARIED   ",
	" POP   M  ",
	" ROCK  M  ",
	" EASY  M  ",
	" LIGHT M  ",
	"CLASSICS  ",
	" OTHER M  ",
	" WEATHER  ",
	" FINANCE  ",
	" CHILDREN ",
	" SOCIAL   ",
	" RELIGION ",
	" PHONE IN ",
	"  TRAVEL  ",
	" LEISURE  ",
	"   JAZZ   ",
	" COUNTRY  ",
	" NATION M ",
	" OLDIES   ",
	"  FOLK M  ",
	" DOCUMENT ",
	"   TEST   ",
	"   ALARM  ",

	/*RBDS PTY*/
	"   NONE   ",
	"   NEWS   ",
	"  INFORM  ",
	"  SPORTS  ",
	"   TALK   ",
	"   ROCK   ",
	" CLS ROCK ",
	" ADLT HIT ",
	" SOFT RCK ",
	"  TOP 40  ",
	" COUNTRY  ",
	"  OLDIES  ",
	"   SOFT   ",
	" NOSTALGA ",
	"   JAZZ   ",
	" CLASSICL ",
	"    R_B   ",
	" SOFT R_B ",
	" LANGUAGE ",
	" REL MUSC ",
	" REL TALK ",
	" PERSNLTY ",
	"  PUBLIC  ",
	"  COLLEGE ",
	"  UNDEF1  ",
	"  UNDEF2  ",
	"  UNDEF3  ",
	"  UNDEF4  ",
	"  UNDEF5  ",
	"  WEATHER ",
	"   TEST   ",
	"   ALERT  ",
#endif

#else//----------------------------------默认8个标准米八屏----------------------------------

	//system
	"        ",// 0
#if defined(SPECIAL_JD_DISP)
	" HELLO  ",
	"  BYE   ",
#elif defined(_FEIYIN_7000A_LOGO_)
    "PROLOGY ",
    "PROLOGY ",
#elif defined(_FEIYIN_SPECIAL_LOGO_)
	"CYCLONE ",
	"CYCLONE ",
#elif defined(_FEIYIN_7000D_LOGO_)
	" SANSUI ",
	"GOOD-BYE",
#elif defined(_FEIYIN_7000D_LOGO1_)
	" SANSNI ",
	"GOOD-BYE",
#elif defined(_FEIYIN_7000B_LOGO_)
	"ROADSTAR",
	"GOOD-BYE",
#elif defined(_FEIYIN_7000DVD_LOGO_)
    " SUPER  ",
    "GOOD-BYE",
#elif defined(_FY7000_DVD_LOGO_)
    "AFRITEC ",
    "GOOD-BYE",
#elif defined(_FEIYIN_7000I_LOGO_)
	" POWER  ",
	"GOOD-BYE",
#elif defined(_FY7000A_DVD_LOGO_)
    "MA AUDIO",
    "GOOD-BYE",
#elif defined(_FY_7000B_LOGO_)
	" ROADEX ",
	"GOOD-BYE",
#elif defined(_FY_7000_LOGO_)
	"  AMA   ",
	"GOOD-BYE",
#elif defined(_FY_7000I_LOGO_)
    "  AIWA  ",
    "GOOD-BYE",
#elif defined(_FY7000G_POWER_LOGO_)
    "MYSTERY ",
    "GOOD-BYE",
#elif defined(_FY7000B_CHANGE_LOGO_)
	"  ACV   ",
	"  ACV   ",	
#elif defined(_FY7000G_CHANGE_LOGO_)
	"  DS18  ",
	"GOOD-BYE",
#elif defined(_FEIYIN_7000_LOGO_)
	"EPLUTUS ",
	"GOOD-BYE",
#elif defined(_FEIYIN_7000C_LOGO_)
	"MYSTERY ",
	"GOOD-BYE",
#elif defined(_TY2200A_SPECIAL_DISP_)
    "   HI   ",
	"   BYE  ",
#elif defined(_SH1323_SPECIAL_DISP_)
    "MIXTRACK",
	"GOOD-BYE",
#else
#if defined(_MR1901_SPECIAL_DISPLAY_)||defined(_ENABLE_LOGO_DISPLAY_)
	"  AURA  ",
#elif defined(_MR9982_SPECIAL_DISPLAY_)
	"    0000",	
#else
	"WELCOME ",
#endif
#if defined(_MR1901_SPECIAL_DISPLAY_)
	"GOODBYE ",
#else
	"GOOD-BYE",
#endif
#endif
	//radio
	" RADIO  ",
	" STEREO ",
	"  MONO  ",// 5
#if defined(_GX6211_LCD_DISPLAY_)
    " LOC ON ",
    " LOC OFF",
#elif defined(_SL6812_SPECIAL_DISPLAY_)
    " LOCAL  ",
    "DISTSNT ",
#elif defined(_KLD3000_SPECIAL_DISPLAY_)
    "  LOC   ",
    "   DX   ",
#else
	" LOCAL  ",
	"   DX   ",
#endif
	" MANUAL ",
	"  AUTO  ",
#if defined(_ENABLE_TL_SPECIAL_DISP_)
    "AREA EUR",//10
    "AREAUSA1",
    "AREAUSA2",
#else
#if defined(_GX6211_SPECIAL_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
    "  EUR   ",
    "  USA   ",
#if defined(_SUPPORT_THREE_AREA_)
    " LATIN  ",
#endif
#else
    "AREA EUR",//10
    "AREA USA",
#endif
#endif
   
#if defined(DAB_MODLE)
	   //DAB
	"  DAB   ",
#endif
#if defined(_SUPPORT_TAPE_)
	" TAPE   ",
	"   TAPE ",
	" TAPE>> ",
	" <<TAPE ",
#endif
#if defined(EASYESY_APP_ONOFF)
#if defined(_MR1901_SPECIAL_DISPLAY_)
    "APP ON   ",
    "APP OFF  ",
#else
	" APP ON  ",
	" APP OFF ",
#endif
#endif
	//music
	"  USB   ",
#if defined(SPECIAL_JD_DISP)||defined(_LT1064_SPECIAL_DISPLAY_)
	"   SD   ",
#else
	" SD/MMC ",
#endif
	"  LOAD  ",
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "RPT ALL ",//15
    "RPT ONE ",
    "RPT DIR ",
    "RPT OFF ",
    "RDM ON  ",
    "RDM OFF ",//20
    "INT ON  ",
    "INT OFF ",
#elif defined(_MR1901_SPECIAL_DISPLAY_)
    "RPT ALL ",//15
    "RPT ONE ",
    "RPTDIR  ",
    "RPT OFF ",
    "RDM ON  ",
    "RDM OFF ",//20
    "INT ON  ",
    "INT OFF ",
#elif defined(_TY2200A_SPECIAL_DISP_)
    "   ALL  ",//15
    "   RPT  ",
    "   RPT  ",
    "   RPT  ",
    "   RDM  ",
    "   ALL  ",//20
    "   INT  ",
    "   ALL   ",
#elif defined(_SL6812_SPECIAL_DISPLAY_)
	"RPT OFF ",//15
	"RPT FILE",
	"RPT DIR ",
	"RPT OFF ",
	"RDM ON  ",
	"RDM OFF ",//20
	"SCN ON  ",
	"SCN OFF ",
#elif defined(_KLD3000_SPECIAL_DISPLAY_)
    "RPT ALL ",//15
    "RPT ONE ",
    "RPT FOLD",
    "RPT OFF ",
    "RDM ON  ",
    "RDM OFF ",//20
    "INT ON  ",
    "INT OFF ",
#else

#if defined(_RPT_ALL_DISPLAY_)||defined(_LK8005_SPECIAL_DISPLAY_)||defined(_YONGHE_SPECIAL_DISPLAY_)
	" RPT OFF",//15
#else
	" RPT ALL",//15
#endif
	" RPT ONE",
	" RPT DIR",
	" RPT OFF",
	" RDM ON ",
	" RDM OFF",//20
	" INT ON ",
	" INT OFF",
#endif

#if defined(_SUPPORT_DISC_)
	"  DISC  ",
	"  EJECT ",
#if defined(NEED_NO_DISC_DISP)
	"NO DISC ",
#endif
#endif

	//linein
#if defined(SPECIAL_JD_DISP)
	"  AUX   ",
#elif defined(_MR1901_SPECIAL_DISPLAY_)
	"AUX IN  ",
#elif defined(_SUPPORT_AUX1_)
	"  AUX2  ",
	"  AUX1  ",	
#else
	" AUX IN ",
#endif
#if defined(_ENABLE_MODE_CHANNEL_)
	"OPTICAL ",
	"   TV   ",
	"  DVD   ",
#endif
#if defined(_SUPPORT_SUB_)
    " SUB IN ",
    " SUB EXT",
#endif
#if defined(_SEL_ADD_TIME_SELECT_)
#if defined(SPECIAL_JD_DISP)
	"TIME 24 ",
	"TIME 12 ",
#else
	"CLOCK 24",
	"CLOCK 12",
#endif
#endif
	//bluetooth
#if defined(_ENABLE_TL_SPECIAL_DISP_)
	"BT AUDIO",// BT PLAY
	"BT AUDIO",// BT PLAY
	"BT AUDIO",// BT PLAY
	"BT AUDIO",// BT PLAY
	"BT AUDIO",// BT PLAY
	"BT AUDIO",// BT PLAY
	"BT AUDIO",// BT PLAY
#else
#if defined(_USE_FYDVD_BT_MODE_)
	"BT PHONE",
#elif defined(_MR1901_SPECIAL_DISPLAY_)
	"  BT    ",
#elif defined(_KLD3300_SPECIAL_DISPLAY_)
	" NO BT  ",
#else
	"   BT   ",
#endif
#if defined(_TY2200A_SPECIAL_DISP_)
    "   BT   ",//25
	"   BT   ",
#elif defined(_KLD3300_SPECIAL_DISPLAY_)
    " LINKED ",//25
    " NO BT  ",
#else
	" BT ON  ",//25
	" BT OFF ",
#endif	
#if defined(_USE_FYDVD_BT_MODE_)
	" NO LINK",
#elif defined(_GX6211_LCD_DISPLAY_)
	" BT NO  ",
#elif defined(SPECIAL_JD_DISP)
	"   BT   ",
#elif defined(_BT_NO_PAIR_DISPLAY_)
	"NOT PAIR",
#elif defined(_MR1901_SPECIAL_DISPLAY_)
	"CONECT  ",
#elif defined(_TY2200A_SPECIAL_DISP_)
	"   BT   ",//25
#elif defined(_WLS316_SPECIAL_DISPLAY_)
	" PHONE  ",
#elif defined(_KLD3300_SPECIAL_DISPLAY_)
	" NO BT  ",
#elif defined(_YH1701_SPECIAL_DISPLAY_)
	" NO LINK",
#else
	"CONNECT ",
#endif
#if defined(SPECIAL_JD_DISP)||defined(_CHANGE_MODE_1_)
	" BT ON  ",
#elif defined(_CHANGE_BT_DISP_)||defined(_KLD3300_SPECIAL_DISPLAY_)
    "   BT   ",
#elif defined(_YH1701_SPECIAL_DISPLAY_)
	" LINK OK",
#elif defined(_SPECIAL_PAUSE_DISPLAY_)
	"PHONE BT",
#else
	" PHONE  ",
#endif
#if defined(SPECIAL_JD_DISP)
	" BT ON  ",
	" BT ON  ",
#elif defined(_CHANGE_MODE_1_)
	"BT AUDIO",// BT PLAY
	" BT ON  ",
#elif defined(_MR1901_SPECIAL_DISPLAY_)||defined(_LK8005_SPECIAL_DISPLAY_)
    "  A2DP  ",// BT PLAY
    " PHONE  ",//BT PAUSE// 30
#elif defined(_CHANGE_BT_DISP_)||defined(_KLD3300_SPECIAL_DISPLAY_)
    "   BT   ",// BT PLAY
	"   BT   ",//BT PAUSE// 30
#elif defined(_YH1701_SPECIAL_DISPLAY_)
	"BT AUDIO",// BT PLAY
	" PAUSE  ",//BT PAUSE
#elif defined(_SPECIAL_PAUSE_DISPLAY_)
    "MUSIC BT",// BT PLAY
    "PAUSE   ",//BT PAUSE// 30
#else
	"BT AUDIO",// BT PLAY
	" PHONE  ",//BT PAUSE// 30
#endif
#endif
#if defined(_GX6211_SPECIAL_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
    "RINGING ",
    "CALLING ",
#else
	"RINGING ",
	"CALLING ",
#endif
	" BT HFP ",
	"BT PHONE",
	" ERROR  ",// 35
	" REDIAL ",
	"DIAL->  ",
#if defined(_SUPPORT_CALL_RECORDS_)
	"NO CALL ",
#endif
#if defined(_SUPPORT_IPHONE_SIR_)
	"  SIRI  ",
#endif
#if defined(_TALKING_VOICE_DISPLAY_)
	"TRANSFER",
#endif
#if defined(HANGUP_DISP_HANGUP)
	"  END   ",
#endif

#if defined(_ENABLE_RESET_DISPLAY_)
	"RESET   ",
#endif

	//upgrade
#if defined(_GX6211_SPECIAL_DISPLAY_)
    "UPGRADE ",
    "SUCCEED ",
#elif defined(_MR1901_SPECIAL_DISPLAY_)
    "UPGRADE ",
    "SUCCEE  ",
#else
	" UPGRADE",
	" SUCCEED",
#endif
	"  FAIL  ",//40

	//others
#if defined(_EVB_VOLUME_100_)||defined(_MR1901_SPECIAL_DISPLAY_)
	"VOL     ",
#else
	" VOL	 ",
#endif
#if defined(_ENABLE_TL_SPECIAL_DISP_)||defined(_ENABLE_ZW5249_DISP_)||defined(_ENABLE_TS5258_DISP_)||defined(_MR1901_SPECIAL_DISPLAY_)||defined(_LT1064_SPECIAL_DISPLAY_)||defined(_WLS316_SPECIAL_DISPLAY_)||defined(_KLD3300_SPECIAL_DISPLAY_)||defined(_LK8005_SPECIAL_DISPLAY_)||defined(_ENABLE_TEL_MUTE_FLASH_)
	"  MUTE  ",
#else
	"MUTE ON ",
#endif	
	"MUTE OFF",
#if defined(_MR1901_SPECIAL_DISPLAY_)
    "LOUDON  ",
    "LOUDOFF ",//45
#elif defined(_KLD3000_SPECIAL_DISPLAY_)
    "XBASS ON",
    "XBASSOFF",//45
#else
	"LOUD ON ",
	"LOUD OFF",//45
#endif
#if defined(_WLS316_SPECIAL_DISPLAY_)||defined(_LK8005_SPECIAL_DISPLAY_)||defined(_FY7000G_SPECIAL_DISPLAY_)
	"DSP OFF ",
#else
	" EQ OFF ",
#endif
#if defined(_TY2200A_SPECIAL_DISP_)
    "  COUN  ",
#else	
	"  FLAT  ",
#endif	
#if defined(_MR1901_SPECIAL_DISPLAY_)
	"CLASSIC ",
#elif defined(_KLD3000_SPECIAL_DISPLAY_)
	" CLASS  ",
#else
	" CLASSIC",
#endif
	"   POP  ",
	"  ROCK  ",//50
	"  JAZZ  ",
#if defined(_GX6211_SPECIAL_DISPLAY_)||defined(_MR1901_SPECIAL_DISPLAY_)
    "BAS     ",
    "TRE     ",
    "BAL     ",
    "FAD     ",//55
#elif defined(_KLD3000_SPECIAL_DISPLAY_)
    "BASS    ",
    "TRE     ",
    "BAL     ",
    "FAD     ",//55
#else
	" BAS	 ",
	" TRE	 ",
	" BAL	 ",
	" FAD	 ",//55
#endif
#if defined(_ENABLE_AP3768_EQ_MODE_)
	" PEOPLE ",
	"  SOFT  ",
	"ELECTRIC",
#endif
#if defined(_SPECIAL_TL2000_SET_)		
	"A VOL ",
	"B VOL ",
#endif
#if defined(_ENABLE_TALK_VOL_)
#if defined(_SL6812_SPECIAL_DISPLAY_)
	"BTVOL   ",
#else
	"TALK    ",
#endif
#endif
#if defined(_ENABLE_BT_ON_OFF_)
	" BT ON  ",
	" BT OFF ",
#endif
#if defined(_FY7000A_SPECIAL_DISPLAY_)
    "SUB1 ON ",
    "SUB1 OFF",
#else
	" SUB ON ",
	" SUB OFF",
#endif
	"BEEP ON ",
	"BEEP OFF",//59

#if REC_EN
	" RECORD ",
#if defined(_ENABLE_RECORD_DISPLAY_)
	" PAUSE  ",
#endif
#endif

#if defined(_ENABLE_TEL_MUTE_DETECT_)
	"TEL MUTE ",
#endif

#if defined(_FY7000A_SPECIAL_DISPLAY_)
#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUBVOL  ",
#endif
	
#if defined(_SUPPORT_SUBWOOFER3_)||defined(_ENABLE_AP3768_SUBWOOFER_)
    "SUB2 ON ",
    "SUB2 OFF",
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	"LPF     ",
#endif

#else

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	" SUB    ",
#endif
#if defined(_SUPPORT_SUBWOOFER3_)
    "SUB2 ON ",
    "SUB2 OFF",
#endif	
#if defined(AUDIO_SUB_LEVEL_SET)
	"CUT     ",
#endif

#endif

#if defined(_ENABLE_7419_MIDDLE_)
	" MID    ",
#endif

#if defined(_ENABLE_ID3_ON_OFF_)
    "ID3 ON  ",
    "ID3 OFF ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
#if defined(_MR1901_SPECIAL_DISPLAY_)
    "  BLUE  ",
    " GREEN  ",
    "  CYAN  ",
    "  RED	 ",
    " PURPLE ",
    "YELLOW  ",
    " WHITE  ",
    "  AUTO  ",
#else
    "  BLUE  ",
    "  GREEN ",
    "  CYAN  ",
    "  RED   ",
	" PURPLE ",
    " YELLOW ",
    "  WHITE ",
    "  AUTO  ",
#endif
#endif

#if defined(_ENABLE_DEFAULT_CONTROL_)
	"SVOL    ",
#elif defined(ADD_ADJUST_DEFAULT_VOL)
	"DEVOL   ",
#endif

#if defined(_ENABLE_FAD_FUNTION_)
#if defined(SUPPORT_CHANGE_FAD_DISP_)
    " FAD IN ",
	" FAD OUT",
	" FAD I=0",
#elif defined(_ENABLE_FAD_RETURN_)
    " FAD OUT",
	" FAD IN ",
	" FAD I=0",
#else
	" FAD  F ",
	" FAD  R ",
	" FAD ALL",
#endif	
#endif

#if defined(_FAD_BAL_FUNTION_)
	" FL     ",//前左
	" FR     ",//前右
	" RL     ",//后左
	" RR     ",//后右
#endif

#if defined(_SUPPORT_10_EQ_FUNTION_)
	"60HZ    ",
	"150HZ   ",
	"400HZ   ",
	"1KHZ    ",
	"2KHZ    ",
	"3KHZ    ",
	"7KHZ    ",
	"10KHZ   ",
	"15KHZ   ",
	"20KHZ   ",
#endif

#if defined(SUPPORT_LED_ADJUST)
	"LED  LOW",
	"LED  MID",
	"LED HIGH",
#endif

#if defined(_ADJUST_A_B_SPK_)
	"A_SPK ON",
	"A_SPKOFF",
	"B_SPK ON",
	"B_SPKOFF",
#endif

#if defined(_ENABLE_RDS_ON_OFF_)
	" RDS ON ",
	" RDS OFF",
#endif

#if defined(SUPPORT_ALARM_FUNC_)
	"ALARM ON",
	"ALARMOFF",
	"  ALARM ",
#endif

#if defined(_ENABLE_TIME_DISP_)
	"DISP ON ",
	"DISP OFF",
#endif

#if defined(_SUPPORT_MIC_OK_)
	" MIC IN ",
#endif

#if defined(_SUPPORT_CAN_MODEL_)
	"CAN MENU",
#endif

#if defined(_ENABLE_PASSWORD_INPUT_)
	"PWD ----",
	" ERROR  ",
#endif

#if defined(_SUPPORT_CAN_UART_)
	"ERR     ",
	"SP      ",
	"MI      ",
	"SOC     ",
	"DIR     ",
	"P1      ",
	"P2      ",
#endif

#if defined(_SUPPORT_POWER_ON_PASSWORD_)
	"PWD ----",
	" ERROR  ",
#elif defined(_SUPPORT_PASSWORD_DISPLAY_)
    "PWD ****",
    "ERROR 01",
    "ERROR 02",
    "ERROR 03",
    "ERROR 04",
    "ERROR 05",
#endif

#if defined(_SL6812_KEY_FUNTION_)
	"  SPM   ",
#endif

#if defined(_ENABLE_PASSWORD_SET_)
	"PWD SET ",
	"PWD ----",
#endif

#if defined(_ENABLE_PLAY_ERROR_)
	"USB ERR ",
	" SD ERR ",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFFIC ",
	" EON TA ",
	"NO TP\TA",
	"TA SEEK ",
	"PI SEEK ",
	"PTY SEEK",
	" AF ON  ",
	" AF OFF ",
	" TA ON  ",
	" TA OFF ",
	"TA PAUSE",
#if defined(_FY7000G_COPY_DISPLAY_)
    " REG ON ",
    " REG OFF",
    " EON LOC",
    " EON DX ",
#else
	" REG LOC",
	" REG DX ",
	" EON ON ",
	" EON OFF",
#endif
#if defined(_ENABLE_RDS_DISPLAY_)
	"RETURN L",
	"RETURN S",
#else
	" R LONG ",
	" R SHORT",
#endif
	"MASK ALL",
	"MASK DIP",
#if defined(_ENABLE_RDS_DISPLAY_)
	"TA ALARM",
	"TA SEEK ",
#else
	" ALARM  ",
	" SWITCH ",
#endif
	"  SYNC  ",
	" ASYNC  ",
#if defined(ENABLE_TA_VOL_SET)
#if defined(_FY7000G_SPECIAL_DISPLAY_)
	"TA-V    ",
#else
	"TA      ",
#endif
#endif
	" SEARCH ",
	"PTY OFF ",
	" MUSIC  ",
	" SPEECH ",
	" FOUND  ",
	"NO FOUND",

	/*RDS PTY*/
	"  NONE  ",
	" N E W S",
	" AFFAIRS",
	" I N F O",
	"  SPORT ",
	" EDUCATE",
	"  DRAMA ",
	" CULTURE",
	" SCIENCE",
	" VARIED ",
	" POP  M ",
	" ROCK M ",
	" EASY M ",
	" LIGHT M",
	"CLASSICS",
	" OTHER M",
	"WEATHER ",
	"FINANCE ",
	"CHILDREN",
	" SOCIAL ",
	"RELIGION",
	"PHONE IN",
	" TRAVEL ",
	"LEISURE ",
	"  JAZZ  ",
	"COUNTRY ",
	"NATION M",
	" OLDIES ",
	" FOLK M ",
	"DOCUMENT",
	"  TEST  ",
	"  ALARM ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
#endif
#endif
};

#if defined(_FY7555_LCD_DISPLAY_)
static const char *ui_lang_tab1[]=
{
	//system
	"        ",// 0
#if defined(_FEIYIN_7000A_LOGO_)
    "PROLOGY ",
    "PROLOGY ",
#else
	"WELCOME ",
	"GOOD-BYE",
#endif
	//radio
	" RADIO  ",
	" STEREO ",
	"  MONO  ",// 5
	" LOCAL  ",
	"   DX   ",
	" MANUAL ",
	"  AUTO  ",
    "AREA EUR",//10
    "AREA USA",
   
#if defined(DAB_MODLE)
	   //DAB
	"  DAB   ",
#endif
#if defined(EASYESY_APP_ONOFF)
	" APP ON  ",
	" APP OFF ",
#endif
	//music
	"  USB   ",
	" SD/MMC ",
	"  LOAD  ",
	" RPT ALL",//15
	" RPT ONE",
	" RPT DIR",
	" RPT OFF",
	" RDM ON ",
	" RDM OFF",//20
	" INT ON ",
	" INT OFF",

#if defined(_SUPPORT_DISC_)
	"  DISC  ",
	"  EJECT ",
#if defined(NEED_NO_DISC_DISP)
	"NO DISC ",
#endif
#endif

	//linein
	" AUX IN ",
#if defined(_SEL_ADD_TIME_SELECT_)
	"CLOCK 24",
	"CLOCK 12",
#endif
	//bluetooth
	"   BT   ",
	" BT ON  ",//25
	" BT OFF ",
	"CONNECT ",
	" PHONE  ",
	"BT AUDIO",// BT PLAY
	" PHONE  ",//BT PAUSE// 30
	" RINGING",
	" CALLING",
	" BT HFP ",
	"BT PHONE",
	" ERROR  ",// 35
	" REDIAL ",
	"DIAL->  ",
#if defined(_SUPPORT_IPHONE_SIR_)
	"  SIRI  ",
#endif
#if defined(_TALKING_VOICE_DISPLAY_)
	"TRANSFER",
#endif
	//upgrade
	" UPGRADE",
	" SUCCEED",
	"  FAIL  ",//40

	//others
	" VOL	 ",
	"MUTE ON ",
	"MUTE OFF",
	"LOUD ON ",
	"LOUD OFF",//45
	" EQ OFF ",
	"  FLAT  ",
	" CLASSIC",
	"   POP  ",
	"  ROCK  ",//50
	"  JAZZ  ",
	" BAS	 ",
	" TRE	 ",
	" BAL	 ",
	" FAD	 ",//55
#if defined(_ENABLE_BT_ON_OFF_)
	" BT ON  ",
	" BT OFF ",
#endif
#if defined(_FY7000A_SPECIAL_DISPLAY_)
    "SUB1 ON ",
    "SUB1 OFF",
#else
	" SUB ON ",
	" SUB OFF",
#endif
	"BEEP ON ",
	"BEEP OFF",//59

#if REC_EN
	" RECORD ",
#if defined(_ENABLE_RECORD_DISPLAY_)
	" PAUSE  ",
#endif
#endif

#if defined(_FY7000A_SPECIAL_DISPLAY_)
#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	"SUBVOL  ",
#endif
	
#if defined(_SUPPORT_SUBWOOFER3_)||defined(_ENABLE_AP3768_SUBWOOFER_)
    "SUB2 ON ",
    "SUB2 OFF",
#endif

#if defined(AUDIO_SUB_LEVEL_SET)
	"LPF     ",
#endif

#else

#if defined(_SUPPORT_SUBWOOFER_MODE2_)
	" SUB    ",
#endif
#if defined(_SUPPORT_SUBWOOFER3_)
    "SUB2 ON ",
    "SUB2 OFF",
#endif	
#if defined(AUDIO_SUB_LEVEL_SET)
	"CUT     ",
#endif

#endif

#if defined(_ENABLE_7419_MIDDLE_)
	" MID    ",
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
    "  BLUE  ",
    "  GREEN ",
    "  CYAN  ",
    "  RED   ",
	" PURPLE ",
    " YELLOW ",
    "  WHITE ",
    "  AUTO  ",
#endif

#if defined(_FAD_BAL_FUNTION_)
	" FL     ",//前左
	" FR     ",//前右
	" RL     ",//后左
	" RR     ",//后右
#endif

#if defined(_FY7555_LCD_DISPLAY_)
	"  SET   ",
#endif

#if defined(_ENABLE_RADIO_RDS_)
	//band rds
	"TRAFFIC ",
	" EON TA ",
	"NO TP\\TA",
	"TA SEEK ",
	"PI SEEK ",
	"PTY SEEK",
	" AF ON  ",
	" AF OFF ",
	" TA ON  ",
	" TA OFF ",
	"TA PAUSE",
	" REG LOC",
	" REG DX ",
	" EON ON ",
	" EON OFF",
#if defined(_ENABLE_RDS_DISPLAY_)
	"RETURN L",
	"RETURN S",
#else
	" R LONG ",
	" R SHORT",
#endif
	"MASK ALL",
	"MASK DIP",
#if defined(_ENABLE_RDS_DISPLAY_)
	"TA ALARM",
	"TA SEEK ",
#else
	" ALARM  ",
	" SWITCH ",
#endif
	"  SYNC  ",
	" ASYNC  ",
	" SEARCH ",
	"PTY OFF ",
	" MUSIC  ",
	" SPEECH ",
	" FOUND  ",
	"NO FOUND",

	/*RDS PTY*/
	"  NONE  ",
	" N E W S",
	" AFFAIRS",
	" I N F O",
	"  SPORT ",
	" EDUCATE",
	"  DRAMA ",
	" CULTURE",
	" SCIENCE",
	" VARIED ",
	" POP  M ",
	" ROCK M ",
	" EASY M ",
	" LIGHT M",
	"CLASSICS",
	" OTHER M",
	"WEATHER ",
	"FINANCE ",
	"CHILDREN",
	" SOCIAL ",
	"RELIGION",
	"PHONE IN",
	" TRAVEL ",
	"LEISURE ",
	"  JAZZ  ",
	"COUNTRY ",
	"NATION M",
	" OLDIES ",
	" FOLK M ",
	"DOCUMENT",
	"  TEST  ",
	"  ALARM ",

	/*RBDS PTY*/
	"  NONE  ",
	"  NEWS  ",
	" INFORM ",
	" SPORTS ",
	"  TALK  ",
	"  ROCK  ",
	"CLS ROCK",
	"ADLT HIT",
	"SOFT RCK",
	" TOP 40 ",
	" COUNTRY",
	" OLDIES ",
	"  SOFT  ",
	"NOSTALGA",
	"  JAZZ  ",
	"CLASSICL",
	"   R_B  ",
	"SOFT R_B",
	"LANGUAGE",
	"REL MUSC",
	"REL TALK",
	"PERSNLTY",
	" PUBLIC ",
	" COLLEGE",
	" UNDEF1 ",
	" UNDEF2 ",
	" UNDEF3 ",
	" UNDEF4 ",
	" UNDEF5 ",
	" WEATHER",
	"  TEST  ",
	"  ALERT ",
#endif
};
#endif

uint8 ui_lang_res_search(ui_lang_id_e disp_id, uint8 *buffer, uint8 length)
{
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	length =4;
#endif
	if(disp_id < LANG_ID_MAX)
	{
		const char *p_str;
		uint8 p_length;
		
		p_str	 = ui_lang_tab[disp_id];
		p_length = strlen(p_str);
		
		if(p_length < length)
		{
			memset(&buffer[p_length], 0, (length - p_length));
		}
		else
		{
			p_length = length;
		}
		
		memcpy(buffer, p_str, p_length);
		
		return TRUE;
	}
	
	return FALSE;
}

#if defined(_FY7555_LCD_DISPLAY_)
uint8 ui_lang_lcd_search(ui_lang_id_e disp_id, uint8 *buffer, uint8 length)
{
	if(disp_id < LANG_ID_MAX)
	{
		const char *p_str;
		uint8 p_length;
		
		p_str	 = ui_lang_tab1[disp_id];
		p_length = strlen(p_str);
		
		if(p_length < length)
		{
			memset(&buffer[p_length], 0, (length - p_length));
		}
		else
		{
			p_length = length;
		}
		
		memcpy(buffer, p_str, p_length);
		
		return TRUE;
	}
	
	return FALSE;
}
#endif

