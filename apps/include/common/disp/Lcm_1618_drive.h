#ifndef _LCM_1618_DRIVE_H
#define _LCM_1618_DRIVE_H

extern void Lcm_Gpio_Init(void);
extern void Lcm_Drive_Init(void);
extern void Lcm_Disp_xx_yy_data(u8 *data,u8 data_size);
extern u8 vol_set_disp_det(u8 data);
extern void lcm_clear(void);
extern void lcm_disp_line (void);
extern uint8 Clear_1_2_page_disp_flag ,Clear_page_disp_flag;
extern uint8 enable_disp_set_icom;


enum{
	LCM_ICON_AF=0,
	LCM_ICON_TA,
	LCM_ICON_TP,
	LCM_ICON_PTY,
	LCM_ICON_EQ_POP,
	LCM_ICON_EQ_FLAT,
	LCM_ICON_EQ_ROCK,
	LCM_ICON_EQ_CLASS,
	LCM_ICON_EQ_JAZZ,
	LCM_ICON_USB,
	LCM_ICON_SD,
	LCM_ICON_MP3,
	LCM_ICON_WMA,
	LCM_ICON_WAV,
	LCM_ICON_BLUETOOTH,
	LCM_ICON_CH,
	LCM_ICON_STO,
	LCM_ICON_CLK,
	LCM_ICOM_MAX,
};

extern u8 LCM_ICOM[LCM_ICOM_MAX][4];

#endif

