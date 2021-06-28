#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"


//	ACC PIN--------GPIO PR1(PIN5)
#define ACC_GPIO								PORTR1

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)

//	ENCODE--------GPIO PR2(PIN6)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif

//	SYS_POW PIN--------GPIO PR3(PIN7)
#define SYS_POW_GPIO							PORTR3
#define SYS_POW_INIT()							do{PORTR_PU(SYS_POW_GPIO, 0);\
                                                   PORTR_PD(SYS_POW_GPIO, 0);}while(0)
#define SYS_POW_OUT()							(PORTR_DIR(SYS_POW_GPIO, 0))
#define SYS_POW_HIGH()							(PORTR_OUT(SYS_POW_GPIO, 1))
#define SYS_POW_LOW()							(PORTR_OUT(SYS_POW_GPIO, 0))
#define SYS_POW_ON()     						{SYS_POW_OUT(); SYS_POW_HIGH();}
#define SYS_POW_OFF()     						{SYS_POW_OUT(); SYS_POW_LOW();}
#define SYS_POW_IN()							do{PORTR_PU(SYS_POW_GPIO, 1);	\
                                                PORTR_PD(SYS_POW_GPIO, 0);	\
                                                PORTR_DIR(SYS_POW_GPIO, 1);	\
												PORTR_DIE(SYS_POW_GPIO, 1);	\
												PORTR_HD(SYS_POW_GPIO, 1);}while(0)

//	AUDIO_MUTE PIN--------GPIO PR0(PIN9)
#define AUDIO_MUTE_GPIO							PORTR0
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						(PORTR_OUT(AUDIO_MUTE_GPIO, 1))
#define AUDIO_MUTE_LOW()						(PORTR_OUT(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}

#if defined(_ENABLE_1621_POWER_)
/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CLK PIN--------GPIO C0(PIN22)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_CLK_GPIO								_B0_

#define LCD_CLK_INIT()							do{JL_PORTC->PU &= ~LCD_CLK_GPIO; \
                                                   JL_PORTC->PD &= ~LCD_CLK_GPIO; \
                                                   JL_PORTC->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()							(JL_PORTC->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()							(JL_PORTC->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()							(JL_PORTC->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()							(JL_PORTC->DIR |= LCD_CLK_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_DATA PIN--------GPIO A14(PIN21)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_DATA_GPIO							_B14_

#define LCD_DATA_INIT()							do{JL_PORTA->PU &= ~LCD_DATA_GPIO; \
                                                   JL_PORTA->PD &= ~LCD_DATA_GPIO; \
                                                   JL_PORTA->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()							(JL_PORTA->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()							(JL_PORTA->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()							(JL_PORTA->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()							(JL_PORTA->DIR |= LCD_DATA_GPIO) 
/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CEPIN--------GPIO A15(PIN20)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_CE_GPIO								_B15_

#define LCD_CE_INIT()							do{JL_PORTA->PU &= ~LCD_CE_GPIO; \
                                                   JL_PORTA->PD &= ~LCD_CE_GPIO; \
                                                   JL_PORTA->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()							(JL_PORTA->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()							(JL_PORTA->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()							(JL_PORTA->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()								(JL_PORTA->DIR |= LCD_CE_GPIO)

#else
/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CE PIN--------GPIO C0(PIN22)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_CE_GPIO								_B0_

#define LCD_CE_INIT()							do{JL_PORTC->PU &= ~LCD_CE_GPIO; \
                                                   JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                                   JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()							(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()							(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()							(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()								(JL_PORTC->DIR |= LCD_CE_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CLK PIN--------GPIO A14(PIN21)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_CLK_GPIO							_B14_

#define LCD_CLK_INIT()							do{JL_PORTA->PU &= ~LCD_CLK_GPIO; \
                                                   JL_PORTA->PD &= ~LCD_CLK_GPIO; \
                                                   JL_PORTA->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()							(JL_PORTA->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()							(JL_PORTA->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()							(JL_PORTA->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()							(JL_PORTA->DIR |= LCD_CLK_GPIO) 
/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_DATA PIN--------GPIO A15(PIN20)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_DATA_GPIO							_B15_

#define LCD_DATA_INIT()							do{JL_PORTA->PU &= ~LCD_DATA_GPIO; \
                                                   JL_PORTA->PD &= ~LCD_DATA_GPIO; \
                                                   JL_PORTA->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()							(JL_PORTA->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()							(JL_PORTA->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()							(JL_PORTA->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()							(JL_PORTA->DIR |= LCD_DATA_GPIO)

#endif

//	I2C_SCL PIN--------GPIO PB12(PIN41)
#define I2C_SCL_GPIO						_B12_
#define I2C_SCL_INIT()						do{JL_PORTB->PU |= I2C_SCL_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()						(JL_PORTB->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()						(JL_PORTB->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()						(JL_PORTB->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()						(JL_PORTB->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PB11(PIN42)
#define I2C_SDA_GPIO						_B11_

#define I2C_SDA_INIT()						do{JL_PORTB->PU |= I2C_SDA_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SDA_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SDA_GPIO;}while(0)
#define I2C_SDA_HIGH()						(JL_PORTB->OUT |= I2C_SDA_GPIO)
#define I2C_SDA_LOW()						(JL_PORTB->OUT &= ~I2C_SDA_GPIO)
#define I2C_SDA_OUT()						(JL_PORTB->DIR &= ~I2C_SDA_GPIO)
#define I2C_SDA_IN()						(JL_PORTB->DIR |= I2C_SDA_GPIO)
#define I2C_SDA_GET()						(JL_PORTB->IN & I2C_SDA_GPIO)
#define I2C_SDA_H()     					{I2C_SDA_OUT();I2C_SDA_HIGH();}
#define I2C_SDA_L()     					{I2C_SDA_OUT();I2C_SDA_LOW();}

//	KEY--------GPIO PB10(PIN43)
#define ADC_KEY1_PIN						AD_KEY_PB10

//	KEY2--------GPIO PB9(PIN44)
#define ADC_KEY2_PIN						AD_KEY_PB9

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

#if defined(_ENABLE_DISC_DETECT_)
#define ADC_DISC_PIN						AD_KEY_PA10
#endif

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO A3(PIN29)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B3_

#define RADIO_RESET_INIT()						do{JL_PORTA->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTA->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTA->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTA->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTA->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

//	DTH PIN--------GPIO C4(PIN16)
#if defined(_SUPPORT_DTH_)

#define DTH_GPIO								_B4_

#define DTH_INIT()								do{JL_PORTC->PU |= DTH_GPIO; \
                                               	   JL_PORTC->PD &= ~DTH_GPIO; \
                                               	   JL_PORTC->HD |= DTH_GPIO;}while(0)
#define DTH_HIGH()								(JL_PORTC->OUT |= DTH_GPIO)
#define DTH_LOW()								(JL_PORTC->OUT &= ~DTH_GPIO)
#define DTH_OUT()								(JL_PORTC->DIR &= ~DTH_GPIO)
#define DTH_IN()								(JL_PORTC->DIR |= DTH_GPIO)
#define DTH_GET()								(JL_PORTC->IN & DTH_GPIO)
#define DTH_CHECK()								(DTH_GET() ? 0 : 1)
#endif

#if defined(_SUPPORT_PANEL_LIGHT_)
/*----------PIN48(B0)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_RED_GPIO							_B0_

#define SYS_RED_INIT()							do{JL_PORTB->PU |= SYS_RED_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_RED_GPIO; \
                                                   JL_PORTB->HD |= SYS_RED_GPIO;}while(0)
#define SYS_RED_HIGH()							(JL_PORTB->OUT |= SYS_RED_GPIO)
#define SYS_RED_LOW()							(JL_PORTB->OUT &= ~SYS_RED_GPIO)
#define SYS_RED_OUT()							(JL_PORTB->DIR &= ~SYS_RED_GPIO)
#define SYS_RED_IN()							(JL_PORTB->DIR |= SYS_RED_GPIO)
#define SYS_RED_ON()     						{SYS_RED_OUT(); SYS_RED_HIGH();}
#define SYS_RED_OFF()     						{SYS_RED_OUT(); SYS_RED_LOW();}

/*-----------PIN47(B1)-GREEN----------------------------------------------------------------------------------------------------*/
#define SYS_GREEN_GPIO							_B1_

#define SYS_GREEN_INIT()						do{JL_PORTB->PU |= SYS_GREEN_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_GREEN_GPIO; \
                                                   JL_PORTB->HD |= SYS_GREEN_GPIO;}while(0)
#define SYS_GREEN_HIGH()						(JL_PORTB->OUT |= SYS_GREEN_GPIO)
#define SYS_GREEN_LOW()							(JL_PORTB->OUT &= ~SYS_GREEN_GPIO)
#define SYS_GREEN_OUT()							(JL_PORTB->DIR &= ~SYS_GREEN_GPIO)
#define SYS_GREEN_IN()							(JL_PORTB->DIR |= SYS_GREEN_GPIO)
#define SYS_GREEN_ON()     						{SYS_GREEN_OUT(); SYS_GREEN_HIGH();}
#define SYS_GREEN_OFF()     					{SYS_GREEN_OUT(); SYS_GREEN_LOW();}

/*-----------PIN46(B2)-BLUE--------------------------------------------------------------------------------------------------*/
#define SYS_BLUE_GPIO							_B2_

#define SYS_BLUE_INIT()							do{JL_PORTB->PU |= SYS_BLUE_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_BLUE_GPIO; \
                                                   JL_PORTB->HD |= SYS_BLUE_GPIO;}while(0)
#define SYS_BLUE_HIGH()							(JL_PORTB->OUT |= SYS_BLUE_GPIO)
#define SYS_BLUE_LOW()							(JL_PORTB->OUT &= ~SYS_BLUE_GPIO)
#define SYS_BLUE_OUT()							(JL_PORTB->DIR &= ~SYS_BLUE_GPIO)
#define SYS_BLUE_IN()							(JL_PORTB->DIR |= SYS_BLUE_GPIO)
#define SYS_BLUE_ON()     						{SYS_BLUE_OUT(); SYS_BLUE_HIGH();}
#define SYS_BLUE_OFF()     						{SYS_BLUE_OUT(); SYS_BLUE_LOW();}

#endif

/*----------------------------------------------------------------------------------------------------------------*/
//	DISC_SWA PIN28--------GPIO PA4
/*----------------------------------------------------------------------------------------------------------------*/
#define DISC_SWA_GPIO							_B4_

#define DISC_SWA_INIT()							do{JL_PORTA->PU |= DISC_SWA_GPIO; \
                                               	   JL_PORTA->PD &= ~DISC_SWA_GPIO; \
                                               	   JL_PORTA->HD |= DISC_SWA_GPIO;}while(0)
#define DISC_SWA_HIGH()							(JL_PORTA->OUT |= DISC_SWA_GPIO)
#define DISC_SWA_LOW()							(JL_PORTA->OUT &= ~DISC_SWA_GPIO)
#define DISC_SWA_OUT()							(JL_PORTA->DIR &= ~DISC_SWA_GPIO)
#define DISC_SWA_IN()							(JL_PORTA->DIR |= DISC_SWA_GPIO)
#define DISC_SWA_GET()							(JL_PORTA->IN & DISC_SWA_GPIO)
#define DISC_SWA_CHECK()						(DISC_SWA_GET() ? 1 : 0)

/*----------------------------------------------------------------------------------------------------------------*/
//	DISC_SWB PIN27--------GPIO PA9
/*----------------------------------------------------------------------------------------------------------------*/
#define DISC_SWB_GPIO							_B9_

#define DISC_SWB_INIT()							do{JL_PORTA->PU |= DISC_SWB_GPIO; \
                                               	   JL_PORTA->PD &= ~DISC_SWB_GPIO; \
                                               	   JL_PORTA->HD |= DISC_SWB_GPIO;}while(0)
#define DISC_SWB_HIGH()							(JL_PORTA->OUT |= DISC_SWB_GPIO)
#define DISC_SWB_LOW()							(JL_PORTA->OUT &= ~DISC_SWB_GPIO)
#define DISC_SWB_OUT()							(JL_PORTA->DIR &= ~DISC_SWB_GPIO)
#define DISC_SWB_IN()							(JL_PORTA->DIR |= DISC_SWB_GPIO)
#define DISC_SWB_GET()							(JL_PORTA->IN & DISC_SWB_GPIO)
#define DISC_SWB_CHECK()						(DISC_SWB_GET() ? 1 : 0)

/*----------------------------------------------------------------------------------------------------------------*/
//	DISC_SWC PIN26--------GPIO PA10
/*----------------------------------------------------------------------------------------------------------------*/
#define DISC_SWC_GPIO							_B10_

#define DISC_SWC_INIT()							do{JL_PORTA->PU |= DISC_SWC_GPIO; \
                                               	   JL_PORTA->PD &= ~DISC_SWC_GPIO; \
                                               	   JL_PORTA->HD |= DISC_SWC_GPIO;}while(0)
#define DISC_SWC_HIGH()							(JL_PORTA->OUT |= DISC_SWC_GPIO)
#define DISC_SWC_LOW()							(JL_PORTA->OUT &= ~DISC_SWC_GPIO)
#define DISC_SWC_OUT()							(JL_PORTA->DIR &= ~DISC_SWC_GPIO)
#define DISC_SWC_IN()							(JL_PORTA->DIR |= DISC_SWC_GPIO)
#define DISC_SWC_GET()							(JL_PORTA->IN & DISC_SWC_GPIO)
#if !defined(_ENABLE_DISC_DETECT_)
#define DISC_SWC_CHECK()						(DISC_SWC_GET() ? 0 : 1)
#define DETECT_DISC_CHECK()						 DISC_SWC_CHECK()
#endif

/*----------------------------------------------------------------------------------------------------------------*/
//	DETECT_USB PIN25--------GPIO PA11
/*----------------------------------------------------------------------------------------------------------------*/
#define DETECT_USB_GPIO							_B11_

#define DETECT_USB_INIT()						do{JL_PORTA->PU |= DETECT_USB_GPIO; \
                                               	   JL_PORTA->PD &= ~DETECT_USB_GPIO; \
                                               	   JL_PORTA->HD |= DETECT_USB_GPIO;}while(0)
#define DETECT_USB_HIGH()						(JL_PORTA->OUT |= DETECT_USB_GPIO)
#define DETECT_USB_LOW()						(JL_PORTA->OUT &= ~DETECT_USB_GPIO)
#define DETECT_USB_OUT()						(JL_PORTA->DIR &= ~DETECT_USB_GPIO)
#define DETECT_USB_IN()							(JL_PORTA->DIR |= DETECT_USB_GPIO)
#define DETECT_USB_GET()						(JL_PORTA->IN & DETECT_USB_GPIO)
#define DETECT_USB_CHECK()						(DETECT_USB_GET() ? 0 : 1)

/*----------------------------------------------------------------------------------------------------------------*/
//	DETECT_SD PIN26--------GPIO PA10
/*----------------------------------------------------------------------------------------------------------------*/
#define DETECT_SD_GPIO							_B10_

#define DETECT_SD_INIT()						do{JL_PORTA->PU |= DETECT_SD_GPIO; \
                                               	   JL_PORTA->PD &= ~DETECT_SD_GPIO; \
                                               	   JL_PORTA->HD |= DETECT_SD_GPIO;}while(0)
#define DETECT_SD_HIGH()						(JL_PORTA->OUT |= DETECT_SD_GPIO)
#define DETECT_SD_LOW()							(JL_PORTA->OUT &= ~DETECT_SD_GPIO)
#define DETECT_SD_OUT()							(JL_PORTA->DIR &= ~DETECT_SD_GPIO)
#define DETECT_SD_IN()							(JL_PORTA->DIR |= DETECT_SD_GPIO)
#define DETECT_SD_GET()							(JL_PORTA->IN & DETECT_SD_GPIO)
#if !defined(_ENABLE_DISC_DETECT_)
#define DETECT_SD_CHECK()						(DETECT_SD_GET() ? 1 : 0)
#endif

/*----------------------------------------------------------------------------------------------------------------*/
//	DISC_OPEN PIN19--------GPIO PC1
/*----------------------------------------------------------------------------------------------------------------*/
#define DISC_OPEN_GPIO							_B1_
#define DISC_OPEN_INIT()						do{JL_PORTC->PU |= DISC_OPEN_GPIO; \
                                          		   JL_PORTC->PD &= ~DISC_OPEN_GPIO; \
                                        		   JL_PORTC->HD &= ~DISC_OPEN_GPIO;}while(0)
#define DISC_OPEN_HIGH()						(JL_PORTC->OUT |= DISC_OPEN_GPIO)
#define DISC_OPEN_LOW()							(JL_PORTC->OUT &= ~DISC_OPEN_GPIO)
#define DISC_OPEN_OUT()							(JL_PORTC->DIR &= ~DISC_OPEN_GPIO)
#define DISC_OPEN_IN()							(JL_PORTC->DIR |= DISC_OPEN_GPIO)
#define DISC_OPEN_GET()							(JL_PORTC->IN & DISC_OPEN_GPIO)
#define DISC_OPEN_ON()							{DISC_OPEN_LOW(); DISC_OPEN_OUT();}
#define DISC_OPEN_OFF()							DISC_OPEN_IN()

/*----------------------------------------------------------------------------------------------------------------*/
//	DISC_CLOSE PIN18--------GPIO PC2
/*----------------------------------------------------------------------------------------------------------------*/
#define DISC_CLOSE_GPIO							_B2_
#define DISC_CLOSE_INIT()						do{JL_PORTC->PU |= DISC_CLOSE_GPIO; \
                                          		   JL_PORTC->PD &= ~DISC_CLOSE_GPIO; \
                                        		   JL_PORTC->HD &= ~DISC_CLOSE_GPIO;}while(0)
#define DISC_CLOSE_HIGH()						(JL_PORTC->OUT |= DISC_CLOSE_GPIO)
#define DISC_CLOSE_LOW()						(JL_PORTC->OUT &= ~DISC_CLOSE_GPIO)
#define DISC_CLOSE_OUT()						(JL_PORTC->DIR &= ~DISC_CLOSE_GPIO)
#define DISC_CLOSE_IN()							(JL_PORTC->DIR |= DISC_CLOSE_GPIO)
#define DISC_CLOSE_GET()						(JL_PORTC->IN & DISC_CLOSE_GPIO)
#define DISC_CLOSE_ON()							{DISC_CLOSE_LOW(); DISC_CLOSE_OUT();}
#define DISC_CLOSE_OFF()						 DISC_CLOSE_IN()

/*----------------------------------------------------------------------------------------------------------------*/
//	DISC_POWER PIN15--------GPIO PC5
/*----------------------------------------------------------------------------------------------------------------*/
#define DISC_POWER_GPIO							_B5_

#define DISC_POWER_INIT()						do{JL_PORTC->PU |= DISC_POWER_GPIO; \
                                          		   JL_PORTC->PD &= ~DISC_POWER_GPIO; \
                                        		   JL_PORTC->HD &= ~DISC_POWER_GPIO;}while(0)
#define DISC_POWER_HIGH()						(JL_PORTC->OUT |= DISC_POWER_GPIO)
#define DISC_POWER_LOW()						(JL_PORTC->OUT &= ~DISC_POWER_GPIO)
#define DISC_POWER_OUT()						(JL_PORTC->DIR &= ~DISC_POWER_GPIO)
#define DISC_POWER_IN()							(JL_PORTC->DIR |= DISC_POWER_GPIO)
#define DISC_POWER_ON()							{DISC_POWER_HIGH(); DISC_POWER_OUT();}
#define DISC_POWER_OFF()						{DISC_POWER_LOW(); DISC_POWER_OUT();}
/*----------------------------------------------------------------------------------------------------------------*/
//	DISC_RESET PIN17--------GPIO PC3
/*----------------------------------------------------------------------------------------------------------------*/
#define DISC_RESET_GPIO							_B3_

#define DISC_RESET_INIT()						do{JL_PORTC->PU |= DISC_RESET_GPIO; \
                                          		   JL_PORTC->PD &= ~DISC_RESET_GPIO; \
                                        		   JL_PORTC->HD &= ~DISC_RESET_GPIO;}while(0)
#define DISC_RESET_HIGH()						(JL_PORTC->OUT |= DISC_RESET_GPIO)
#define DISC_RESET_LOW()						(JL_PORTC->OUT &= ~DISC_RESET_GPIO)
#define DISC_RESET_OUT()						(JL_PORTC->DIR &= ~DISC_RESET_GPIO)
#define DISC_RESET_IN()							(JL_PORTC->DIR |= DISC_RESET_GPIO)
#define DISC_RESET_ON()							{DISC_RESET_LOW(); DISC_RESET_OUT();}
#define DISC_RESET_OFF()						{DISC_RESET_HIGH(); DISC_RESET_OUT();}

/*----------------------------------------------------------------------------------------------------------------*/
//	ACC_POW PIN16--------GPIO PC4
/*----------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_ACC_POWER_)//47½Å
#define ACC_POW_GPIO							_B1_

#define ACC_POW_INIT()							do{JL_PORTB->PU &= ~ACC_POW_GPIO; \
                                                   JL_PORTB->PD &= ~ACC_POW_GPIO; \
                                                   JL_PORTB->HD &= ~ACC_POW_GPIO;}while(0)
#define ACC_POW_HIGH()							(JL_PORTB->OUT |= ACC_POW_GPIO)
#define ACC_POW_LOW()							(JL_PORTB->OUT &= ~ACC_POW_GPIO)
#define ACC_POW_OUT()							(JL_PORTB->DIR &= ~ACC_POW_GPIO)
#define ACC_POW_IN()							(JL_PORTB->DIR |= ACC_POW_GPIO)
#define ACC_POW_ON()     						{ACC_POW_OUT(); ACC_POW_LOW();}
#define ACC_POW_OFF()     						{ACC_POW_OUT(); ACC_POW_HIGH();}

#else
#define ACC_POW_GPIO							_B30_

#define ACC_POW_INIT()							do{JL_PORTC->PU &= ~ACC_POW_GPIO; \
                                                   JL_PORTC->PD &= ~ACC_POW_GPIO; \
                                                   JL_PORTC->HD &= ~ACC_POW_GPIO;}while(0)
#define ACC_POW_HIGH()							(JL_PORTC->OUT |= ACC_POW_GPIO)
#define ACC_POW_LOW()							(JL_PORTC->OUT &= ~ACC_POW_GPIO)
#define ACC_POW_OUT()							(JL_PORTC->DIR &= ~ACC_POW_GPIO)
#define ACC_POW_IN()							(JL_PORTC->DIR |= ACC_POW_GPIO)
#define ACC_POW_ON()     						{ACC_POW_OUT(); ACC_POW_HIGH();}
#define ACC_POW_OFF()     						{ACC_POW_OUT(); ACC_POW_LOW();}
#endif
#endif

