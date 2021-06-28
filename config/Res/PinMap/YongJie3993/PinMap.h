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

//	AUDIO_MUTE PIN--------GPIO PA11(PIN25)
#define AUDIO_MUTE_GPIO							_B11_
#define AUDIO_MUTE_INIT()						do{JL_PORTA->PU &= ~AUDIO_MUTE_GPIO; \
                                           		 JL_PORTA->PD &= ~AUDIO_MUTE_GPIO;}while(0)
#define AUDIO_MUTE_OUT()						(JL_PORTA->DIR &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_HIGH()						(JL_PORTA->OUT |= AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_LOW()						(JL_PORTA->OUT &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}

#if defined(_ENABLE_TDA7265_MUTE_)
//	AUDIO_MUTE PIN--------GPIO PC2(PIN18)
#define TDA7265_MUTE_GPIO						_B2_
#define TDA7265_MUTE_INIT()						do{JL_PORTC->PU &= ~TDA7265_MUTE_GPIO; \
                                           		 JL_PORTC->PD &= ~TDA7265_MUTE_GPIO;}while(0)
#define TDA7265_MUTE_OUT()						(JL_PORTC->DIR &= ~TDA7265_MUTE_GPIO)
#define TDA7265_MUTE_HIGH()						(JL_PORTC->OUT |= TDA7265_MUTE_GPIO)
#define TDA7265_MUTE_LOW()						(JL_PORTC->OUT &= ~TDA7265_MUTE_GPIO)
#define TDA7265_MUTE_ON()						{TDA7265_MUTE_OUT();TDA7265_MUTE_HIGH();}
#define TDA7265_MUTE_OFF()						{TDA7265_MUTE_OUT();TDA7265_MUTE_LOW();}
#endif

//SDA--------GPIO PA15(PIN20)
#define LCD_SDA_GPIO						_B15_
#define LCD_DATA_INIT()						do{JL_PORTA->PU &= ~LCD_SDA_GPIO; \
                                            JL_PORTA->PD &= ~LCD_SDA_GPIO; \
                                            JL_PORTA->HD &= ~LCD_SDA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTA->OUT |= LCD_SDA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTA->OUT &= ~LCD_SDA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTA->DIR &= ~LCD_SDA_GPIO)
#define LCD_DATA_IN()						(JL_PORTA->DIR |= LCD_SDA_GPIO)

//SCL--------GPIO PA14(PIN21)
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

#if defined(_ENABLE_PT2313_I2C_)
//	I2C_SCL PIN--------GPIO PC5(PIN15)
#define I2C_SCL_GPIO1						_B5_
#define I2C_SCL_INIT1()						do{JL_PORTC->PU |= I2C_SCL_GPIO1; \
                                            JL_PORTC->PD &= ~I2C_SCL_GPIO1; \
                                            JL_PORTC->HD &= ~I2C_SCL_GPIO1;}while(0)
#define I2C_SCL_HIGH1()						(JL_PORTC->OUT |= I2C_SCL_GPIO1)
#define I2C_SCL_LOW1()						(JL_PORTC->OUT &= ~I2C_SCL_GPIO1)
#define I2C_SCL_OUT1()						(JL_PORTC->DIR &= ~I2C_SCL_GPIO1)
#define I2C_SCL_IN1()						(JL_PORTC->DIR |= I2C_SCL_GPIO1)
#define I2C_SCL_H1()     					{I2C_SCL_OUT1();I2C_SCL_HIGH1();}
#define I2C_SCL_L1()     					{I2C_SCL_OUT1();I2C_SCL_LOW1();}

//	I2C_SDA PIN--------GPIO PC4(PIN16)
#define I2C_SDA_GPIO1						_B4_

#define I2C_SDA_INIT1()						do{JL_PORTC->PU |= I2C_SDA_GPIO1; \
                                            JL_PORTC->PD &= ~I2C_SDA_GPIO1; \
                                            JL_PORTC->HD &= ~I2C_SDA_GPIO1;}while(0)
#define I2C_SDA_HIGH1()						(JL_PORTC->OUT |= I2C_SDA_GPIO1)
#define I2C_SDA_LOW1()						(JL_PORTC->OUT &= ~I2C_SDA_GPIO1)
#define I2C_SDA_OUT1()						(JL_PORTC->DIR &= ~I2C_SDA_GPIO1)
#define I2C_SDA_IN1()						(JL_PORTC->DIR |= I2C_SDA_GPIO1)
#define I2C_SDA_GET1()						(JL_PORTC->IN & I2C_SDA_GPIO1)
#define I2C_SDA_H1()     					{I2C_SDA_OUT1();I2C_SDA_HIGH1();}
#define I2C_SDA_L1()     					{I2C_SDA_OUT1();I2C_SDA_LOW1();}
#endif

//	KEY--------GPIO PB10(PIN43)
#define ADC_KEY1_PIN						AD_KEY_PB10

//	KEY2--------GPIO PB9(PIN44)
#define ADC_KEY2_PIN						AD_KEY_PB9

#if defined(_YJ3993_ADKEY_CONTROL_)
//	KEY3--------GPIO PR1(PIN27)
#define ADC_KEY3_PIN						AD_KEY_PA9

//	KEY4--------GPIO PA10(PIN26)
#define ADC_KEY4_PIN						AD_KEY_PA10
#endif

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO PA4(PIN28)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B4_

#define RADIO_RESET_INIT()						do{JL_PORTA->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTA->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTA->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTA->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTA->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

/*----------------------------------------------------------------------------------------------------------------*/
//	ACC POWER PIN--------GPIO PC3(PIN17)
/*----------------------------------------------------------------------------------------------------------------*/
#if defined(_ACC_POWER_CONTROL_)
#define ACC_POWER_GPIO							_B3_

#define ACC_POWER_INIT()						do{JL_PORTC->PU |= ACC_POWER_GPIO; \
                                                   JL_PORTC->PD &= ~ACC_POWER_GPIO; \
                                                   JL_PORTC->HD |= ACC_POWER_GPIO;}while(0)
#define ACC_POWER_HIGH()						(JL_PORTC->OUT |= ACC_POWER_GPIO)
#define ACC_POWER_LOW()							(JL_PORTC->OUT &= ~ACC_POWER_GPIO)
#define ACC_POWER_OUT()							(JL_PORTC->DIR &= ~ACC_POWER_GPIO)
#define ACC_POWER_IN()							(JL_PORTC->DIR |= ACC_POWER_GPIO)
#define ACC_POWER_OFF()     					{ACC_POWER_OUT(); ACC_POWER_LOW();}
#define ACC_POWER_ON()     						{ACC_POWER_OUT(); ACC_POWER_HIGH();}
#endif

#endif

