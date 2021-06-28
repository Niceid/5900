#ifndef __UI_IDLE_H__
#define __UI_IDLE_H__
//enter here

enum{
	DISP_TYPE_CLEAN = 0,
	DISP_TYPE_RTC,
	DISP_TYPE_WELCOME,
	DISP_TYPE_GOODBYE,
	DISP_TYPE_ICON_ENABLE,
	DISP_TYPE_ICON_DISABLE,
#if defined(_SUPPORT_POWER_ON_PASSWORD_)||defined(_SUPPORT_PASSWORD_DISPLAY_)
	DISP_TYPE_PASSWORD,
#endif
};

typedef struct{
	uint8 disp_type;
}idle_disp_info_t;

#if defined(_SPECIAL_LOGO_DISPLAY_)||defined(_SPECIAL_POWER_OFF_DISPLAY_)
typedef struct{
	uint8	status;
	uint8	point;
	uint8	step;
	uint8	timer;
}idle_disp_ctrl_t;
#endif

#if defined(_FY7555_LOGO_DISPLAY_)
typedef struct{
	uint8	status;
	uint8	point;
	uint8	step;
	uint8	timer;
}idle_disp_ctrl_t;
#endif

typedef struct{
	idle_disp_info_t disp_info;
#if defined(_SPECIAL_LOGO_DISPLAY_)
	idle_disp_ctrl_t disp_ctrl;
#endif
#if defined(_FY7555_LOGO_DISPLAY_)
	idle_disp_ctrl_t disp_ctrl;
#endif
#if defined(_SPECIAL_POWER_OFF_DISPLAY_)
	idle_disp_ctrl_t disp_ctrl1;
#endif
}idle_disp_t;


#endif  /*  __UI_IDLE_H__  */

