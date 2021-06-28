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

//	SYS_POW PIN--------GPIO PR0(PIN13)
#define SYS_POW_GPIO							PORTR0
#define SYS_POW_INIT()							do{PORTR_PU(SYS_POW_GPIO, 1);\
                                                   PORTR_PD(SYS_POW_GPIO, 0);}while(0)
                                                                                                      
#define SYS_POW_OUT()							(PORTR_DIR(SYS_POW_GPIO, 0))
#define SYS_POW_HIGH()							(PORTR_OUT(SYS_POW_GPIO, 1))
#define SYS_POW_LOW()							(PORTR_OUT(SYS_POW_GPIO, 0))
#if defined(_ENABLE_POWER_LOW_OPEN_)
#define SYS_POW_ON()     						{SYS_POW_OUT(); SYS_POW_LOW();}
#define SYS_POW_OFF()     						{SYS_POW_OUT(); SYS_POW_HIGH();}
#else
#define SYS_POW_ON()     						{SYS_POW_OUT(); SYS_POW_HIGH();}
#define SYS_POW_OFF()     						{SYS_POW_OUT(); SYS_POW_LOW();}
#endif
#define SYS_POW_IN()							do{PORTR_PU(SYS_POW_GPIO, 1);	\
                                                PORTR_PD(SYS_POW_GPIO, 0);	\
                                                PORTR_DIR(SYS_POW_GPIO, 1);	\
												PORTR_DIE(SYS_POW_GPIO, 1);	\
												PORTR_HD(SYS_POW_GPIO, 1);}while(0)

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
#if defined(_DECODER_AUDIO_CONTROL_)
#define I2C_SCL_GPIO						_B30_
#else
#define I2C_SCL_GPIO						_B11_
#endif

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
#if defined(_DECODER_AUDIO_CONTROL_)
#define I2C_SDA_GPIO						_B30_
#else
#define I2C_SDA_GPIO						_B12_
#endif

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

#if defined(_SUPPORT_PANEL_LIGHT_)
/*----------PIN1(B5)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_RED_GPIO							_B5_

#define SYS_RED_INIT()							do{JL_PORTB->PU |= SYS_RED_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_RED_GPIO; \
                                                   JL_PORTB->HD |= SYS_RED_GPIO;}while(0)
#define SYS_RED_HIGH()							(JL_PORTB->OUT |= SYS_RED_GPIO)
#define SYS_RED_LOW()							(JL_PORTB->OUT &= ~SYS_RED_GPIO)
#define SYS_RED_OUT()							(JL_PORTB->DIR &= ~SYS_RED_GPIO)
#define SYS_RED_IN()							(JL_PORTB->DIR |= SYS_RED_GPIO)
#define SYS_RED_ON()     						{SYS_RED_OUT(); SYS_RED_HIGH();}
#define SYS_RED_OFF()     						{SYS_RED_OUT(); SYS_RED_LOW();}

/*-----------PIN61(B7)-GREEN----------------------------------------------------------------------------------------------------*/
#define SYS_GREEN_GPIO							_B7_

#define SYS_GREEN_INIT()						do{JL_PORTB->PU |= SYS_GREEN_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_GREEN_GPIO; \
                                                   JL_PORTB->HD |= SYS_GREEN_GPIO;}while(0)
#define SYS_GREEN_HIGH()						(JL_PORTB->OUT |= SYS_GREEN_GPIO)
#define SYS_GREEN_LOW()							(JL_PORTB->OUT &= ~SYS_GREEN_GPIO)
#define SYS_GREEN_OUT()							(JL_PORTB->DIR &= ~SYS_GREEN_GPIO)
#define SYS_GREEN_IN()							(JL_PORTB->DIR |= SYS_GREEN_GPIO)
#define SYS_GREEN_ON()     						{SYS_GREEN_OUT(); SYS_GREEN_HIGH();}
#define SYS_GREEN_OFF()     					{SYS_GREEN_OUT(); SYS_GREEN_LOW();}

/*-----------PIN63(B6)-BLUE--------------------------------------------------------------------------------------------------*/
#define SYS_BLUE_GPIO							_B6_

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

#if defined(SUPPORT_LED_ADJUST)
#define BACK_LED_LOW_GPIO						_B12_

#define BACK_LED_LOW_INIT()						do{JL_PORTA->PU |= BACK_LED_LOW_GPIO; \
                                                   JL_PORTA->PD &= ~BACK_LED_LOW_GPIO; \
                                                   JL_PORTA->HD |= BACK_LED_LOW_GPIO;}while(0)

#define BACK_LED_LOW_HIGH()						(JL_PORTA->OUT |= BACK_LED_LOW_GPIO)
#define BACK_LED_LOW_LOW()						(JL_PORTA->OUT &= ~BACK_LED_LOW_GPIO)
#define BACK_LED_LOW_OUT()						(JL_PORTA->DIR &= ~BACK_LED_LOW_GPIO)
#define BACK_LED_LOW_ON()						BACK_LED_LOW_LOW()
#define BACK_LED_LOW_OFF()						BACK_LED_LOW_HIGH()


#define BACK_LED_MIN_GPIO						_B13_

#define BACK_LED_MIN_INIT()						do{JL_PORTA->PU |= BACK_LED_MIN_GPIO; \
                                                   JL_PORTA->PD &= ~BACK_LED_MIN_GPIO; \
                                                   JL_PORTA->HD |= BACK_LED_MIN_GPIO;}while(0)

#define BACK_LED_MIN_HIGH()						(JL_PORTA->OUT |= BACK_LED_MIN_GPIO)
#define BACK_LED_MIN_LOW()						(JL_PORTA->OUT &= ~BACK_LED_MIN_GPIO)
#define BACK_LED_MIN_OUT()						(JL_PORTA->DIR &= ~BACK_LED_MIN_GPIO)
#define BACK_LED_MIN_ON()						BACK_LED_MIN_LOW()
#define BACK_LED_MIN_OFF()						BACK_LED_MIN_HIGH()
#endif

#define AUDIO_A_SPK_GPIO						_B3_

#define AUDIO_A_SPK_INIT()						do{JL_PORTA->PU |= AUDIO_A_SPK_GPIO; \
                                                   JL_PORTA->PD &= ~AUDIO_A_SPK_GPIO; \
                                                   JL_PORTA->HD |= AUDIO_A_SPK_GPIO;}while(0)

#define AUDIO_A_SPK_HIGH()						(JL_PORTA->OUT |= AUDIO_A_SPK_GPIO)
#define AUDIO_A_SPK_LOW()						(JL_PORTA->OUT &= ~AUDIO_A_SPK_GPIO)
#define AUDIO_A_SPK_OUT()						(JL_PORTA->DIR &= ~AUDIO_A_SPK_GPIO)


#define AUDIO_B_SPK_GPIO							_B2_

#define AUDIO_B_SPK_INIT()						do{JL_PORTA->PU |= AUDIO_B_SPK_GPIO; \
                                                   JL_PORTA->PD &= ~AUDIO_B_SPK_GPIO; \
                                                   JL_PORTA->HD |= AUDIO_B_SPK_GPIO;}while(0)

#define AUDIO_B_SPK_HIGH()						(JL_PORTA->OUT |= AUDIO_B_SPK_GPIO)
#define AUDIO_B_SPK_LOW()						(JL_PORTA->OUT &= ~AUDIO_B_SPK_GPIO)
#define AUDIO_B_SPK_OUT()						(JL_PORTA->DIR &= ~AUDIO_B_SPK_GPIO)

#if defined(_ENABLE_ALARM_CLOCK_)
/*----------PIN43(A1)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_BEEP_GPIO							_B1_

#define SYS_BEEP_INIT()							do{JL_PORTA->PU |= SYS_BEEP_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_BEEP_GPIO; \
                                                   JL_PORTA->HD |= SYS_BEEP_GPIO;}while(0)
#define SYS_BEEP_HIGH()							(JL_PORTA->OUT |= SYS_BEEP_GPIO)
#define SYS_BEEP_LOW()							(JL_PORTA->OUT &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_OUT()							(JL_PORTA->DIR &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_IN()							(JL_PORTA->DIR |= SYS_BEEP_GPIO)
#define SYS_BEEP_ON()     						SYS_BEEP_HIGH()
#define SYS_BEEP_OFF()     						SYS_BEEP_LOW()

#define SYS_RED_INIT()							SYS_BEEP_INIT()
#define SYS_RED_HIGH()							SYS_BEEP_HIGH()
#define SYS_RED_LOW()							SYS_BEEP_LOW()
#define SYS_RED_OUT()							SYS_BEEP_OUT()
#define SYS_RED_IN()							SYS_BEEP_IN()
#define SYS_RED_ON()     						SYS_BEEP_ON()
#define SYS_RED_OFF()     						SYS_BEEP_OFF()

#define SYS_GREEN_INIT()						SYS_BEEP_INIT()
#define SYS_GREEN_HIGH()						SYS_BEEP_HIGH()
#define SYS_GREEN_LOW()							SYS_BEEP_LOW()
#define SYS_GREEN_OUT()							SYS_BEEP_OUT()
#define SYS_GREEN_IN()							SYS_BEEP_IN()
#define SYS_GREEN_ON()     						SYS_BEEP_ON()
#define SYS_GREEN_OFF()     					SYS_BEEP_OFF()

#define SYS_BLUE_INIT()							SYS_BEEP_INIT()
#define SYS_BLUE_HIGH()							SYS_BEEP_HIGH()
#define SYS_BLUE_LOW()							SYS_BEEP_LOW()
#define SYS_BLUE_OUT()							SYS_BEEP_OUT()
#define SYS_BLUE_IN()							SYS_BEEP_IN()
#define SYS_BLUE_ON()     						SYS_BEEP_ON()
#define SYS_BLUE_OFF()     						SYS_BEEP_OFF()

#endif

#if defined(_E2PROM_DRIVER_AT24CXX_)

/*-----------PIN61(B7)-GREEN----------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SDA_GPIO						_B7_

#define E2PROM_I2C_SDA_INIT()					do{JL_PORTB->PU |= SYS_BEEP_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_BEEP_GPIO; \
                                                   JL_PORTB->HD |= SYS_BEEP_GPIO;}while(0)
#define E2PROM_I2C_SDA_HIGH()					(JL_PORTB->OUT |= E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_LOW()					(JL_PORTB->OUT &= ~E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_OUT()					(JL_PORTB->DIR &= ~E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_IN()						(JL_PORTB->DIR |= E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_GET()					(JL_PORTB->IN & E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_H()     					E2PROM_I2C_SDA_IN()
#define E2PROM_I2C_SDA_L()     					{E2PROM_I2C_SDA_OUT();E2PROM_I2C_SDA_LOW(); }
/*-----------PIN63(B6)-BLUE--------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SCL_GPIO						_B6_

#define E2PROM_I2C_SCL_INIT()					do{JL_PORTB->PU |= SYS_BEEP_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_BEEP_GPIO; \
                                                   JL_PORTB->HD |= SYS_BEEP_GPIO;}while(0)
#define E2PROM_I2C_SCL_HIGH()					(JL_PORTB->OUT |= E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_LOW()					(JL_PORTB->OUT &= ~E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_OUT()					(JL_PORTB->DIR &= ~E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_IN()						(JL_PORTB->DIR |= E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_H()     					E2PROM_I2C_SCL_IN()
#define E2PROM_I2C_SCL_L()     					{E2PROM_I2C_SCL_OUT();E2PROM_I2C_SCL_LOW();}
#endif

#endif

