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

//	SYS_POW PIN--------GPIO PB43(PIN41)
#define SYS_POW_GPIO							_B10_

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

//	AUDIO_MUTE PIN--------GPIO PA11(PIN25)
#define AUDIO_MUTE_GPIO							_B11_
#define AUDIO_MUTE_INIT()						do{JL_PORTA->PU &= ~AUDIO_MUTE_GPIO; \
                                           		 JL_PORTA->PD &= ~AUDIO_MUTE_GPIO;}while(0)
#define AUDIO_MUTE_OUT()						(JL_PORTA->DIR &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_HIGH()						(JL_PORTA->OUT |= AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_LOW()						(JL_PORTA->OUT &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}

//	DOT_SDA--------GPIO PA15(PIN20)
#define LCD_SDA_GPIO						_B15_
#define LCD_DATA_INIT()						do{JL_PORTA->PU &= ~LCD_SDA_GPIO; \
                                            JL_PORTA->PD &= ~LCD_SDA_GPIO; \
                                            JL_PORTA->HD &= ~LCD_SDA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTA->OUT |= LCD_SDA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTA->OUT &= ~LCD_SDA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTA->DIR &= ~LCD_SDA_GPIO)
#define LCD_DATA_IN()						(JL_PORTA->DIR |= LCD_SDA_GPIO)

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

//	KEY--------GPIO PA9(PIN27)
#define ADC_KEY1_PIN						AD_KEY_PA9

//	KEY2--------GPIO PA3(PIN29)
#define ADC_KEY2_PIN						AD_KEY_PA3

#if defined(SUPPORT_PARTY_CONTROL)
//	KEY3--------GPIO PA10(PIN26)
#define ADC_KEY3_PIN						AD_KEY_PA10
#endif

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO A4
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


#if defined(_ENABLE_RCA_MUTE_)
#define RCA_MUTE_GPIO						PORTR3
#define RCA_MUTE_INIT()						do{PORTR_PU(RCA_MUTE_GPIO, 0);\
                                            PORTR_PD(RCA_MUTE_GPIO, 0);}while(0)
#define RCA_MUTE_OUT()						(PORTR_DIR(RCA_MUTE_GPIO, 0))
#define RCA_MUTE_HIGH()						(PORTR_OUT(RCA_MUTE_GPIO, 1))
#define RCA_MUTE_LOW()						(PORTR_OUT(RCA_MUTE_GPIO, 0))
#define RCA_MUTE_OFF()						{RCA_MUTE_OUT();RCA_MUTE_LOW();}
#define RCA_MUTE_ON()						{RCA_MUTE_OUT();RCA_MUTE_HIGH();}
#endif

#if defined(_ENABLE_AMP_LIGHT_)
#define AMP_CONTROL_GPIO						_B30_
#define AMP_CONTROL_INIT()						do{JL_PORTB->PU |= AMP_CONTROL_GPIO; \
                                                   JL_PORTB->PD &= ~AMP_CONTROL_GPIO; \
                                                   JL_PORTB->HD |= AMP_CONTROL_GPIO;}while(0)
#define AMP_CONTROL_OUT()						(JL_PORTB->DIR &= ~AMP_CONTROL_GPIO)
#define AMP_CONTROL_HIGH()						(JL_PORTB->OUT |= AMP_CONTROL_GPIO)
#define AMP_CONTROL_LOW()						(JL_PORTB->OUT &= ~AMP_CONTROL_GPIO)
#define AMP_CONTROL_OFF()						{AMP_CONTROL_OUT();AMP_CONTROL_LOW();}
#define AMP_CONTROL_ON()						{AMP_CONTROL_OUT();AMP_CONTROL_HIGH();}
#endif


#if defined(_ENABLE_BEEP_)
/*----------PIN42(B11)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_BEEP_GPIO							_B11_

#define SYS_BEEP_INIT()							do{JL_PORTB->PU |= SYS_BEEP_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_BEEP_GPIO; \
                                                   JL_PORTB->HD |= SYS_BEEP_GPIO;}while(0)
#define SYS_BEEP_HIGH()							(JL_PORTB->OUT |= SYS_BEEP_GPIO)
#define SYS_BEEP_LOW()							(JL_PORTB->OUT &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_OUT()							(JL_PORTB->DIR &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_IN()							(JL_PORTB->DIR |= SYS_BEEP_GPIO)
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

#endif

