#include "sdk_cfg.h"
#include "ui_drv.h"


#define UI_DRV_DEBUG
#ifdef UI_DRV_DEBUG
#define ui_drv_printf 			ew_printf
#define ui_drv_puts 			ew_puts
#else
#define ui_drv_printf(...)
#define ui_drv_puts(...)
#endif


#if defined(_UI_DRIVER_VFD_)||defined(_LCD_DRIVER_A4900B_)||defined(_LCD_DRIVER_A4901_)||defined(_LCD_COM5_SEG8)
	#include "lcd\lcd_res.c"
	#include "lcd\lcd_drv.c"
#endif

