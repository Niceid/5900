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

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO PC3(PIN17)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B3_

#define RADIO_RESET_INIT()						do{JL_PORTC->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTC->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTC->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTC->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTC->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTC->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTC->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

#if defined(_SUPPORT_PANEL_LIGHT_)
/*----------PIN28(A4)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_RED_GPIO							_B4_

#define SYS_RED_INIT()							do{JL_PORTA->PU |= SYS_RED_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_RED_GPIO; \
                                                   JL_PORTA->HD |= SYS_RED_GPIO;}while(0)
#define SYS_RED_HIGH()							(JL_PORTA->OUT |= SYS_RED_GPIO)
#define SYS_RED_LOW()							(JL_PORTA->OUT &= ~SYS_RED_GPIO)
#define SYS_RED_OUT()							(JL_PORTA->DIR &= ~SYS_RED_GPIO)
#define SYS_RED_IN()							(JL_PORTA->DIR |= SYS_RED_GPIO)
#define SYS_RED_ON()     						{SYS_RED_OUT(); SYS_RED_HIGH();}
#define SYS_RED_OFF()     						{SYS_RED_OUT(); SYS_RED_LOW();}

/*-----------PIN31(A1)-GREEN----------------------------------------------------------------------------------------------------*/
#define SYS_GREEN_GPIO							_B1_

#define SYS_GREEN_INIT()						do{JL_PORTA->PU |= SYS_GREEN_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_GREEN_GPIO; \
                                                   JL_PORTA->HD |= SYS_GREEN_GPIO;}while(0)
#define SYS_GREEN_HIGH()						(JL_PORTA->OUT |= SYS_GREEN_GPIO)
#define SYS_GREEN_LOW()							(JL_PORTA->OUT &= ~SYS_GREEN_GPIO)
#define SYS_GREEN_OUT()							(JL_PORTA->DIR &= ~SYS_GREEN_GPIO)
#define SYS_GREEN_IN()							(JL_PORTA->DIR |= SYS_GREEN_GPIO)
#define SYS_GREEN_ON()     						{SYS_GREEN_OUT(); SYS_GREEN_HIGH();}
#define SYS_GREEN_OFF()     					{SYS_GREEN_OUT(); SYS_GREEN_LOW();}

/*-----------PIN27(A9)-BLUE--------------------------------------------------------------------------------------------------*/
#define SYS_BLUE_GPIO							_B9_

#define SYS_BLUE_INIT()							do{JL_PORTA->PU |= SYS_BLUE_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_BLUE_GPIO; \
                                                   JL_PORTA->HD |= SYS_BLUE_GPIO;}while(0)
#define SYS_BLUE_HIGH()							(JL_PORTA->OUT |= SYS_BLUE_GPIO)
#define SYS_BLUE_LOW()							(JL_PORTA->OUT &= ~SYS_BLUE_GPIO)
#define SYS_BLUE_OUT()							(JL_PORTA->DIR &= ~SYS_BLUE_GPIO)
#define SYS_BLUE_IN()							(JL_PORTA->DIR |= SYS_BLUE_GPIO)
#define SYS_BLUE_ON()     						{SYS_BLUE_OUT(); SYS_BLUE_HIGH();}
#define SYS_BLUE_OFF()     						{SYS_BLUE_OUT(); SYS_BLUE_LOW();}

#endif

//	DTH PIN--------GPIO C11
#if defined(_SUPPORT_DTH_)

#define DTH_GPIO								_B1_

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

/*-----------PIN29(A3)---------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_TAPE_)
#define TAPE_DETCET_GPIO						_B3_

#define TAPE_DETCET_INIT()						do{JL_PORTA->PU |= TAPE_DETCET_GPIO; \
                                               	   JL_PORTA->PD &= ~TAPE_DETCET_GPIO; \
                                               	   JL_PORTA->HD |= TAPE_DETCET_GPIO;}while(0)
#define TAPE_DETCET_HIGH()						(JL_PORTA->OUT |= TAPE_DETCET_GPIO)
#define TAPE_DETCET_LOW()						(JL_PORTA->OUT &= ~TAPE_DETCET_GPIO)
#define TAPE_DETCET_OUT()						(JL_PORTA->DIR &= ~TAPE_DETCET_GPIO)
#define TAPE_DETCET_IN()						(JL_PORTA->DIR |= TAPE_DETCET_GPIO)
#define TAPE_DETCET_GET()						(JL_PORTA->IN & TAPE_DETCET_GPIO)
#define TAPE_DETCET_CHECK()						(TAPE_DETCET_GET() ? 0 : 1)

/*-----------PIN27(A9)---------------------------------------------------------------------------------------------------*/
#define TAPE_AB_GPIO							_B9_

#define TAPE_AB_INIT()							do{JL_PORTA->PU |= TAPE_AB_GPIO; \
                                               	   JL_PORTA->PD &= ~TAPE_AB_GPIO; \
                                               	   JL_PORTA->HD |= TAPE_AB_GPIO;}while(0)
#define TAPE_AB_HIGH()							(JL_PORTA->OUT |= TAPE_AB_GPIO)
#define TAPE_AB_LOW()							(JL_PORTA->OUT &= ~TAPE_AB_GPIO)
#define TAPE_AB_OUT()							(JL_PORTA->DIR &= ~TAPE_AB_GPIO)
#define TAPE_AB_IN()							(JL_PORTA->DIR |= TAPE_AB_GPIO)
#define TAPE_AB_GET()							(JL_PORTA->IN & TAPE_AB_GPIO)
#define TAPE_AB_CHECK()							(TAPE_AB_GET() ? 1 : 0)

/*-----------PIN28(A4)---------------------------------------------------------------------------------------------------*/
#define TAPE_POWER_GPIO							_B4_

#define TAPE_POWER_INIT()						do{JL_PORTA->PU |= TAPE_POWER_GPIO; \
                                                   JL_PORTA->PD &= ~TAPE_POWER_GPIO; \
                                                   JL_PORTA->HD |= TAPE_POWER_GPIO;}while(0)
#define TAPE_POWER_HIGH()						(JL_PORTA->OUT |= TAPE_POWER_GPIO)
#define TAPE_POWER_LOW()						(JL_PORTA->OUT &= ~TAPE_POWER_GPIO)
#define TAPE_POWER_OUT()						(JL_PORTA->DIR &= ~TAPE_POWER_GPIO)
#define TAPE_POWER_IN()							(JL_PORTA->DIR |= TAPE_POWER_GPIO)
#define TAPE_POWER_ON()     					{TAPE_POWER_OUT(); TAPE_POWER_HIGH();}
#define TAPE_POWER_OFF()     					{TAPE_POWER_OUT(); TAPE_POWER_LOW();}

#endif

#endif

