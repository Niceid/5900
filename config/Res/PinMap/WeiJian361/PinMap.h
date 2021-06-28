#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

//	ACC PIN--------GPIO PR1(PIN5)
#define ACC_GPIO								PORTR1

#if defined(_ENABLE_ACC_DETECT_)
#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 0);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 1 : 0)
#else
#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)
#endif

//	ENCODE--------GPIO PR2(PIN6)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif

//	SYS_POW PIN--------GPIO PA10(PIN26)
#define SYS_POW_GPIO							_B10_
#define SYS_POW_INIT()							do{JL_PORTA->PU &= ~SYS_POW_GPIO; \
                                           		 JL_PORTA->PD &= ~SYS_POW_GPIO;}while(0)
#define SYS_POW_OUT()							(JL_PORTA->DIR &= ~SYS_POW_GPIO)
#define SYS_POW_HIGH()							(JL_PORTA->OUT |= SYS_POW_GPIO)
#define SYS_POW_LOW()							(JL_PORTA->OUT &= ~SYS_POW_GPIO)
#define SYS_POW_ON()     						{SYS_POW_OUT(); SYS_POW_HIGH();}
#define SYS_POW_OFF()     						{SYS_POW_OUT(); SYS_POW_LOW();}
#define SYS_POW_IN()							do{(JL_PORTA->PU |= SYS_POW_GPIO);	\
                                                (JL_PORTA->PD &= ~SYS_POW_GPIO);	\
                                                (JL_PORTA->DIR |= SYS_POW_GPIO);	\
												(JL_PORTA->DIE |= SYS_POW_GPIO);	\
												(JL_PORTA->HD |= SYS_POW_GPIO);}while(0)

//	AUDIO_MUTE PIN--------GPIO PA11(PIN25)
#define AUDIO_MUTE_GPIO							_B11_
#define AUDIO_MUTE_INIT()						do{JL_PORTA->PU &= ~AUDIO_MUTE_GPIO; \
                                           		 JL_PORTA->PD &= ~AUDIO_MUTE_GPIO;}while(0)
#define AUDIO_MUTE_OUT()						(JL_PORTA->DIR &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_HIGH()						(JL_PORTA->OUT |= AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_LOW()						(JL_PORTA->OUT &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}

//	DOT_SDA--------GPIO PA15(PIN20)
#define LCD_DATA_GPIO						_B15_
#define LCD_DATA_INIT()						do{JL_PORTA->PU &= ~LCD_DATA_GPIO; \
                                            JL_PORTA->PD &= ~LCD_DATA_GPIO; \
                                            JL_PORTA->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTA->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTA->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTA->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()						(JL_PORTA->DIR |= LCD_DATA_GPIO)

//DOT_SCL--------GPIO PA14(PIN21)
#define LCD_CLK_GPIO						_B14_
#define LCD_CLK_INIT()						do{JL_PORTA->PU &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTA->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTA->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTA->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTA->DIR |= LCD_CLK_GPIO) 

//NULL--------GPIO PC0(PIN22)
#define LCD_CE_GPIO							_B0_
#define LCD_CE_INIT()						do{JL_PORTC->PU &= ~LCD_CE_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()						(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()						(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()						(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()							(JL_PORTC->DIR |= LCD_CE_GPIO)
                                            
//	I2C_SCL PIN--------GPIO PC5(PIN15)
#define I2C_SCL_GPIO						_B5_
#define I2C_SCL_INIT()						do{JL_PORTC->PU |= I2C_SCL_GPIO; \
                                            JL_PORTC->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTC->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()						(JL_PORTC->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()						(JL_PORTC->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()						(JL_PORTC->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()						(JL_PORTC->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PC4(PIN16)
#define I2C_SDA_GPIO						_B4_

#define I2C_SDA_INIT()						do{JL_PORTC->PU |= I2C_SDA_GPIO; \
                                            JL_PORTC->PD &= ~I2C_SDA_GPIO; \
                                            JL_PORTC->HD &= ~I2C_SDA_GPIO;}while(0)
#define I2C_SDA_HIGH()						(JL_PORTC->OUT |= I2C_SDA_GPIO)
#define I2C_SDA_LOW()						(JL_PORTC->OUT &= ~I2C_SDA_GPIO)
#define I2C_SDA_OUT()						(JL_PORTC->DIR &= ~I2C_SDA_GPIO)
#define I2C_SDA_IN()						(JL_PORTC->DIR |= I2C_SDA_GPIO)
#define I2C_SDA_GET()						(JL_PORTC->IN & I2C_SDA_GPIO)
#define I2C_SDA_H()     					{I2C_SDA_OUT();I2C_SDA_HIGH();}
#define I2C_SDA_L()     					{I2C_SDA_OUT();I2C_SDA_LOW();}

#if defined(_E2PROM_DRIVER_AT24CXX_)

/*-----------PIN18(C2)-----------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SDA_GPIO						_B2_

#define E2PROM_I2C_SDA_INIT()					do{JL_PORTC->PU |= E2PROM_I2C_SDA_GPIO; \
                                                   JL_PORTC->PD &= ~E2PROM_I2C_SDA_GPIO; \
                                                   JL_PORTC->HD |= E2PROM_I2C_SDA_GPIO;}while(0)
#define E2PROM_I2C_SDA_HIGH()					(JL_PORTC->OUT |= E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_LOW()					(JL_PORTC->OUT &= ~E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_OUT()					(JL_PORTC->DIR &= ~E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_IN()						(JL_PORTC->DIR |= E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_GET()					(JL_PORTC->IN & E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_H()     					E2PROM_I2C_SDA_IN()
#define E2PROM_I2C_SDA_L()     					{E2PROM_I2C_SDA_OUT();E2PROM_I2C_SDA_LOW(); }

/*-----------PIN17(C3)---------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SCL_GPIO						_B3_

#define E2PROM_I2C_SCL_INIT()					do{JL_PORTC->PU |= E2PROM_I2C_SCL_GPIO; \
                                                   JL_PORTC->PD &= ~E2PROM_I2C_SCL_GPIO; \
                                                   JL_PORTC->HD |= E2PROM_I2C_SCL_GPIO;}while(0)
#define E2PROM_I2C_SCL_HIGH()					(JL_PORTC->OUT |= E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_LOW()					(JL_PORTC->OUT &= ~E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_OUT()					(JL_PORTC->DIR &= ~E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_IN()						(JL_PORTC->DIR |= E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_H()     					E2PROM_I2C_SCL_IN()
#define E2PROM_I2C_SCL_L()     					{E2PROM_I2C_SCL_OUT();E2PROM_I2C_SCL_LOW();}
#endif

//	KEY--------GPIO PB10(PIN43)
#define ADC_KEY1_PIN						AD_KEY_PA9

//	KEY2--------GPIO PB9(PIN44)
#define ADC_KEY2_PIN						AD_KEY_PA3

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO PC3(PIN17)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B30_

#define RADIO_RESET_INIT()						do{JL_PORTC->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTC->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTC->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTC->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTC->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTC->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTC->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

/*-------------------------------------------------------------------------------------------------------------*/
//	DTH PIN44--------GPIO B9
#if defined(_SUPPORT_DTH_)

#define DTH_GPIO								_B9_

#define DTH_INIT()								do{JL_PORTB->PU |= DTH_GPIO; \
                                               	   JL_PORTB->PD &= ~DTH_GPIO; \
                                               	   JL_PORTB->HD |= DTH_GPIO;}while(0)
#define DTH_HIGH()								(JL_PORTB->OUT |= DTH_GPIO)
#define DTH_LOW()								(JL_PORTB->OUT &= ~DTH_GPIO)
#define DTH_OUT()								(JL_PORTB->DIR &= ~DTH_GPIO)
#define DTH_IN()								(JL_PORTB->DIR |= DTH_GPIO)
#define DTH_GET()								(JL_PORTB->IN & DTH_GPIO)
#define DTH_CHECK()								(DTH_GET() ? 0 : 1)
#endif

/*----------------PIN42---------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_REAL_VBAR_)
#define ADC_VBAR_PIN							AD_KEY_PB11
#endif

#if defined(_SUPPORT_SUBWOOFER2_)
/*----------------------------------------------------------------------------------------------------------------*/
//	SUB PIN(41)--------GPIO B12
/*----------------------------------------------------------------------------------------------------------------*/
#define SYS_SUB_GPIO							_B12_

#define SYS_SUB_INIT()							do{JL_PORTB->PU |= SYS_SUB_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_SUB_GPIO; \
                                                   JL_PORTB->HD |= SYS_SUB_GPIO;}while(0)
#define SYS_SUB_HIGH()							(JL_PORTB->OUT |= SYS_SUB_GPIO)
#define SYS_SUB_LOW()							(JL_PORTB->OUT &= ~SYS_SUB_GPIO)
#define SYS_SUB_OUT()							(JL_PORTB->DIR &= ~SYS_SUB_GPIO)
#define SYS_SUB_IN()							(JL_PORTB->DIR |= SYS_SUB_GPIO)
#define SYS_SUB_OFF()     						{SYS_SUB_OUT(); SYS_SUB_LOW();}
#define SYS_SUB_ON()     						{SYS_SUB_OUT(); SYS_SUB_HIGH();}
#endif

#if defined(_ENABLE_INH_CONTROL_)
#define LCD_INH_GPIO						_B10_

#define LCD_INH_INIT()						do{JL_PORTB->PU &= ~LCD_INH_GPIO; \
                                               JL_PORTB->PD &= ~LCD_INH_GPIO; \
                                               JL_PORTB->HD &= ~LCD_INH_GPIO;}while(0)
#define LCD_INH_HIGH()						(JL_PORTB->OUT |= LCD_INH_GPIO)
#define LCD_INH_LOW()						(JL_PORTB->OUT &= ~LCD_INH_GPIO)
#define LCD_INH_OUT()						(JL_PORTB->DIR &= ~LCD_INH_GPIO)
#define LCD_INH_IN()						(JL_PORTB->DIR |= LCD_INH_GPIO)
#define LCD_INH_ON()     					{LCD_INH_OUT();LCD_INH_LOW();}
#define LCD_INH_OFF()     					{LCD_INH_OUT();LCD_INH_HIGH();}
#endif

/*--------PIN19(PC1)-----------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_USB_BATT_CONTROL_)
#define USB_CONTROL_GPIO						_B1_

#define USB_CONTROL_INIT()						do{JL_PORTC->PU &= ~USB_CONTROL_GPIO; \
                                               	   JL_PORTC->PD &= ~USB_CONTROL_GPIO; \
                                               	   JL_PORTC->HD &= ~USB_CONTROL_GPIO;}while(0)
#define USB_CONTROL_HIGH()						(JL_PORTC->OUT |= USB_CONTROL_GPIO)
#define USB_CONTROL_LOW()						(JL_PORTC->OUT &= ~USB_CONTROL_GPIO)
#define USB_CONTROL_OUT()						(JL_PORTC->DIR &= ~USB_CONTROL_GPIO)
#define USB_CONTROL_IN()						(JL_PORTC->DIR |= USB_CONTROL_GPIO)
#define USB_CONTROL_ON()     					{USB_CONTROL_OUT();USB_CONTROL_HIGH();}
#define USB_CONTROL_OFF()     					{USB_CONTROL_OUT();USB_CONTROL_LOW();}
#endif

#if defined(_ENABLE_TEL_MUTE_DETECT_)

#define BT_TEL_MUTE_GPIO						PORTR3

#define BT_TEL_MUTE_INIT()						do{PORTR_PU(BT_TEL_MUTE_GPIO, 0); \
                                               	   PORTR_PD(BT_TEL_MUTE_GPIO, 0); \
                                               	   PORTR_HD(BT_TEL_MUTE_GPIO, 0);}while(0)
#define BT_TEL_MUTE_HIGH()						(PORTR_OUT(BT_TEL_MUTE_GPIO, 1))
#define BT_TEL_MUTE_LOW()						(PORTR_OUT(BT_TEL_MUTE_GPIO, 0))
#define BT_TEL_MUTE_OUT()						(PORTR_DIR(BT_TEL_MUTE_GPIO, 0))
#define BT_TEL_MUTE_IN()						(PORTR_DIR(BT_TEL_MUTE_GPIO, 1))

#define BT_TEL_MUTE_GET()						(PORTR_IN(BT_TEL_MUTE_GPIO))
#define BT_TEL_MUTE_CHECK()						(BT_TEL_MUTE_GET() ? 1 : 0)

#endif
/*-------------------------------------------------------------------------------------------------------------*/

#endif

