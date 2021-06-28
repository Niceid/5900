
#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

//	ACC PIN--------GPIO PR1(PIN9)
#define ACC_GPIO								PORTR1

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)

//	ENCODE--------GPIO PR2(PIN10)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif

#if defined(_SUPPORT_VR2_)
#define ADC_ENCODE2_PIN							AD_KEY_PB9
#endif

//	SYS_POW PIN--------GPIO PB5(PIN1)
#define SYS_POW_GPIO							_B5_
#define SYS_POW_INIT()							do{JL_PORTB->PU &= ~SYS_POW_GPIO; \
                                           		 JL_PORTB->PD &= ~SYS_POW_GPIO;}while(0)
#define SYS_POW_OUT()							(JL_PORTB->DIR &= ~SYS_POW_GPIO)
#define SYS_POW_HIGH()							(JL_PORTB->OUT |= SYS_POW_GPIO)
#define SYS_POW_LOW()							(JL_PORTB->OUT &= ~SYS_POW_GPIO)
#define SYS_POW_ON()     						{SYS_POW_OUT(); SYS_POW_HIGH();}
#define SYS_POW_OFF()     						{SYS_POW_OUT(); SYS_POW_LOW();}
#define SYS_POW_IN()							do{(JL_PORTB->PU |= SYS_POW_GPIO);	\
                                                (JL_PORTB->PD &= ~SYS_POW_GPIO);	\
                                                (JL_PORTB->DIR |= SYS_POW_GPIO);	\
												(JL_PORTB->DIE |= SYS_POW_GPIO);	\
												(JL_PORTB->HD |= SYS_POW_GPIO);}while(0)

//	DOT_SDA--------GPIO PC3(PIN22)
#define LCD_DATA_GPIO						_B3_
#define LCD_DATA_INIT()						do{JL_PORTC->PU &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->PD &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTC->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTC->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTC->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()						(JL_PORTC->DIR |= LCD_DATA_GPIO)

//DOT_SCL--------GPIO PC4(PIN21)
#define LCD_CLK_GPIO						_B4_
#define LCD_CLK_INIT()						do{JL_PORTC->PU &= ~LCD_CLK_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTC->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTC->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTC->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTC->DIR |= LCD_CLK_GPIO)

//NULL--------GPIO PC5(PIN20)
#define LCD_CE_GPIO							_B5_
#define LCD_CE_INIT()						do{JL_PORTC->PU &= ~LCD_CE_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()						(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()						(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()						(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()							(JL_PORTC->DIR |= LCD_CE_GPIO)


//	AUDIO_MUTE PIN--------GPIO PR3(PIN11)
#define AUDIO_MUTE_GPIO							PORTR3
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						(PORTR_OUT(AUDIO_MUTE_GPIO, 1))
#define AUDIO_MUTE_LOW()						(PORTR_OUT(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT(); AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT(); AUDIO_MUTE_LOW();}

//	I2C_SCL PIN--------GPIO PB11(PIN55)
#define I2C_SCL_GPIO						_B11_
#define I2C_SCL_INIT()						do{JL_PORTB->PU |= I2C_SCL_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()						(JL_PORTB->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()						(JL_PORTB->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()						(JL_PORTB->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()						(JL_PORTB->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}


//	I2C_SDA PIN--------GPIO PB12(PIN54)
#define I2C_SDA_GPIO						_B12_
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

//	KEY--------GPIO PB8(PIN59)

#define ADC_KEY1_PIN						AD_KEY_PB8

//	KEY2--------GPIO PB9(PIN57)
#if !defined(_SUPPORT_VR2_)
#define ADC_KEY2_PIN						AD_KEY_PB9
#endif

//	IR PIN--------GPIO PB3(PIN58)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO B10(PIN56)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B10_

#define RADIO_RESET_INIT()						do{JL_PORTB->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTB->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTB->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTB->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTB->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}


#if defined(_E2PROM_DRIVER_AT24CXX_)

/*-----------PIN63(B6)-----------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SDA_GPIO						_B6_

#define E2PROM_I2C_SDA_INIT()					do{JL_PORTB->PU |= E2PROM_I2C_SDA_GPIO; \
                                                   JL_PORTB->PD &= ~E2PROM_I2C_SDA_GPIO; \
                                                   JL_PORTB->HD |= E2PROM_I2C_SDA_GPIO;}while(0)
#define E2PROM_I2C_SDA_HIGH()					(JL_PORTB->OUT |= E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_LOW()					(JL_PORTB->OUT &= ~E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_OUT()					(JL_PORTB->DIR &= ~E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_IN()						(JL_PORTB->DIR |= E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_GET()					(JL_PORTB->IN & E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_H()     					E2PROM_I2C_SDA_IN()
#define E2PROM_I2C_SDA_L()     					{E2PROM_I2C_SDA_OUT();E2PROM_I2C_SDA_LOW(); }

/*-----------PIN61(B7)---------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SCL_GPIO						_B7_

#define E2PROM_I2C_SCL_INIT()					do{JL_PORTB->PU |= E2PROM_I2C_SCL_GPIO; \
                                                   JL_PORTB->PD &= ~E2PROM_I2C_SCL_GPIO; \
                                                   JL_PORTB->HD |= E2PROM_I2C_SCL_GPIO;}while(0)
#define E2PROM_I2C_SCL_HIGH()					(JL_PORTB->OUT |= E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_LOW()					(JL_PORTB->OUT &= ~E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_OUT()					(JL_PORTB->DIR &= ~E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_IN()						(JL_PORTB->DIR |= E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_H()     					E2PROM_I2C_SCL_IN()
#define E2PROM_I2C_SCL_L()     					{E2PROM_I2C_SCL_OUT();E2PROM_I2C_SCL_LOW();}
#endif

#endif


