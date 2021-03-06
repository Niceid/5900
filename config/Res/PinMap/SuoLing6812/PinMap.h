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

//	SYS_POW PIN--------GPIO PA12(PIN32)

#define SYS_POW_GPIO							_B12_

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
#define ADC_KEY2_PIN						AD_KEY_PB9

#if defined(_SL6812_PARTY_CONTROL_)
//	KEY3--------GPIO PA4(PIN28)
#define ADC_KEY3_PIN						AD_KEY_PA4
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

#if defined(_ENABLE_INH_CONTROL_)
#define LCD_INH_GPIO						_B2_

#define LCD_INH_INIT()						do{JL_PORTC->PU &= ~LCD_INH_GPIO; \
                                               JL_PORTC->PD &= ~LCD_INH_GPIO; \
                                               JL_PORTC->HD &= ~LCD_INH_GPIO;}while(0)
#define LCD_INH_HIGH()						(JL_PORTC->OUT |= LCD_INH_GPIO)
#define LCD_INH_LOW()						(JL_PORTC->OUT &= ~LCD_INH_GPIO)
#define LCD_INH_OUT()						(JL_PORTC->DIR &= ~LCD_INH_GPIO)
#define LCD_INH_IN()						(JL_PORTC->DIR |= LCD_INH_GPIO)
#define LCD_INH_ON()     					{LCD_INH_OUT();LCD_INH_HIGH();}//??????????????
#define LCD_INH_OFF()     					{LCD_INH_OUT();LCD_INH_LOW();}
#endif

#if defined(_SL6812_ILL_LIGHT_)
/*-----------PIN36(A8)-ILL_DETECT--------------------------------------------------------------------------------------------------*/
#define ILL_DETECT_GPIO							_B8_

#define ILL_DETECT_INIT()						do{JL_PORTA->PU |= ILL_DETECT_GPIO; \
                                               	   JL_PORTA->PD &= ~ILL_DETECT_GPIO; \
                                               	   JL_PORTA->HD |= ILL_DETECT_GPIO;}while(0)
#define ILL_DETECT_HIGH()						(JL_PORTA->OUT |= ILL_DETECT_GPIO)
#define ILL_DETECT_LOW()						(JL_PORTA->OUT &= ~ILL_DETECT_GPIO)
#define ILL_DETECT_OUT()						(JL_PORTA->DIR &= ~ILL_DETECT_GPIO)
#define ILL_DETECT_IN()							(JL_PORTA->DIR |= ILL_DETECT_GPIO)
#define ILL_DETECT_GET()						(JL_PORTA->IN & ILL_DETECT_GPIO)
#define ILL_DETECT_CHECK()						(ILL_DETECT_GET() ? 0 : 1)

/*-----------PIN37(A7)-ILL_LIGHT--------------------------------------------------------------------------------------------------*/
#define ILL_LIGHT_GPIO							_B7_

#define ILL_LIGHT_INIT()						do{JL_PORTA->PU |= ILL_LIGHT_GPIO; \
                                                   JL_PORTA->PD &= ~ILL_LIGHT_GPIO; \
                                                   JL_PORTA->HD |= ILL_LIGHT_GPIO;}while(0)
#define ILL_LIGHT_HIGH()						(JL_PORTA->OUT |= ILL_LIGHT_GPIO)
#define ILL_LIGHT_LOW()							(JL_PORTA->OUT &= ~ILL_LIGHT_GPIO)
#define ILL_LIGHT_OUT()							(JL_PORTA->DIR &= ~ILL_LIGHT_GPIO)
#define ILL_LIGHT_IN()							(JL_PORTA->DIR |= ILL_LIGHT_GPIO)
#define ILL_LIGHT_ON()     						{ILL_LIGHT_OUT();ILL_LIGHT_HIGH();}
#define ILL_LIGHT_OFF()     					{ILL_LIGHT_OUT();ILL_LIGHT_LOW();}

#endif

#if defined(_SL6812_AUX_DETECT_)
/*-----------PIN33(A11)-AUX_DETECT--------------------------------------------------------------------------------------------------*/
#define AUX_DET_GPIO							_B11_
#define AUX_DET_INIT()							do{JL_PORTA->PU |= AUX_DET_GPIO; \
                                                JL_PORTA->PD &= ~AUX_DET_GPIO; \
                                                JL_PORTA->HD &= ~AUX_DET_GPIO;}while(0)
#define AUX_DET_IN()							(JL_PORTA->DIR |= AUX_DET_GPIO)
#define AUX_DET_GET()							(JL_PORTA->IN & AUX_DET_GPIO)
#define AUX_DET_CHECK()							(AUX_DET_GET() ? 0 : 1)

#endif

#if defined(_ENABLE_BEEP_)
/*---PIN43(A1)--BEEP-----------------------------------------------------------------------------------------------------------*/
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

#endif

