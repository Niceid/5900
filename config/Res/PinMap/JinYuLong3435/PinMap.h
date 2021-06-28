#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"


//	ACC PIN--------GPIO PB3 (PIN45)
#define ACC_GPIO								_B3_

#define ACC_INIT()								do{JL_PORTB->PU |= ACC_GPIO; \
                                         		JL_PORTB->PD &= ~ACC_GPIO; \
                                            	JL_PORTB->HD &= ~ACC_GPIO;}while(0)

#define ACC_GET()								(JL_PORTB->IN & ACC_GPIO)  
#define ACC_CHECK()								(ACC_GET() ? 1 : 0)

//	ENCODE--------GPIO PR2(PIN6)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif

//	SYS_POW PIN--------GPIO PA10(PIN26)

#define SYS_POW_GPIO							_B30_

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
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}

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

//	I2C_SCL PIN--------GPIO PA14(PIN21)
#define I2C_SCL_GPIO						_B14_
#define I2C_SCL_INIT()						do{JL_PORTA->PU |= I2C_SCL_GPIO; \
                                            JL_PORTA->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTA->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()						(JL_PORTA->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()						(JL_PORTA->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()						(JL_PORTA->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()						(JL_PORTA->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PA15(PIN20)
#define I2C_SDA_GPIO						_B15_

#define I2C_SDA_INIT()						do{JL_PORTA->PU |= I2C_SDA_GPIO; \
                                            JL_PORTA->PD &= ~I2C_SDA_GPIO; \
                                            JL_PORTA->HD &= ~I2C_SDA_GPIO;}while(0)
#define I2C_SDA_HIGH()						(JL_PORTA->OUT |= I2C_SDA_GPIO)
#define I2C_SDA_LOW()						(JL_PORTA->OUT &= ~I2C_SDA_GPIO)
#define I2C_SDA_OUT()						(JL_PORTA->DIR &= ~I2C_SDA_GPIO)
#define I2C_SDA_IN()						(JL_PORTA->DIR |= I2C_SDA_GPIO)
#define I2C_SDA_GET()						(JL_PORTA->IN & I2C_SDA_GPIO)
#define I2C_SDA_H()     					{I2C_SDA_OUT();I2C_SDA_HIGH();}
#define I2C_SDA_L()     					{I2C_SDA_OUT();I2C_SDA_LOW();}

//	KEY--------GPIO PA9(PIN27)
#define ADC_KEY1_PIN						AD_KEY_PA9

//	KEY2--------GPIO PB11(PIN42)
#define ADC_KEY2_PIN						AD_KEY_PB11

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO B12(41)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B12_

#define RADIO_RESET_INIT()						do{JL_PORTB->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTB->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTB->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTB->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTB->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

//	DTH PIN--------GPIO C11
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

#if defined(_SUPPORT_PANEL_LIGHT_)
/*----------PIN17(C3)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_RED_GPIO							_B3_
#define SYS_RED_INIT()							do{JL_PORTC->PU |= SYS_RED_GPIO; \
                                                   JL_PORTC->PD &= ~SYS_RED_GPIO; \
                                                   JL_PORTC->HD |= SYS_RED_GPIO;}while(0)
#define SYS_RED_HIGH()							(JL_PORTC->OUT |= SYS_RED_GPIO)
#define SYS_RED_LOW()							(JL_PORTC->OUT &= ~SYS_RED_GPIO)
#define SYS_RED_OUT()							(JL_PORTC->DIR &= ~SYS_RED_GPIO)
#define SYS_RED_IN()							(JL_PORTC->DIR |= SYS_RED_GPIO)
#define SYS_RED_ON()     						{SYS_RED_OUT(); SYS_RED_HIGH();}
#define SYS_RED_OFF()     						{SYS_RED_OUT(); SYS_RED_LOW();}

/*-----------PIN18(C2)-GREEN----------------------------------------------------------------------------------------------------*/
#define SYS_GREEN_GPIO							_B2_

#define SYS_GREEN_INIT()						do{JL_PORTC->PU |= SYS_GREEN_GPIO; \
                                                   JL_PORTC->PD &= ~SYS_GREEN_GPIO; \
                                                   JL_PORTC->HD |= SYS_GREEN_GPIO;}while(0)
#define SYS_GREEN_HIGH()						(JL_PORTC->OUT |= SYS_GREEN_GPIO)
#define SYS_GREEN_LOW()							(JL_PORTC->OUT &= ~SYS_GREEN_GPIO)
#define SYS_GREEN_OUT()							(JL_PORTC->DIR &= ~SYS_GREEN_GPIO)
#define SYS_GREEN_IN()							(JL_PORTC->DIR |= SYS_GREEN_GPIO)
#define SYS_GREEN_ON()     						{SYS_GREEN_OUT(); SYS_GREEN_HIGH();}
#define SYS_GREEN_OFF()     					{SYS_GREEN_OUT(); SYS_GREEN_LOW();}

/*-----------PIN19(C1)-BLUE--------------------------------------------------------------------------------------------------*/
#define SYS_BLUE_GPIO							_B1_

#define SYS_BLUE_INIT()							do{JL_PORTC->PU |= SYS_BLUE_GPIO; \
                                                   JL_PORTC->PD &= ~SYS_BLUE_GPIO; \
                                                   JL_PORTC->HD |= SYS_BLUE_GPIO;}while(0)
#define SYS_BLUE_HIGH()							(JL_PORTC->OUT |= SYS_BLUE_GPIO)
#define SYS_BLUE_LOW()							(JL_PORTC->OUT &= ~SYS_BLUE_GPIO)
#define SYS_BLUE_OUT()							(JL_PORTC->DIR &= ~SYS_BLUE_GPIO)
#define SYS_BLUE_IN()							(JL_PORTC->DIR |= SYS_BLUE_GPIO)
#define SYS_BLUE_ON()     						{SYS_BLUE_OUT(); SYS_BLUE_HIGH();}
#define SYS_BLUE_OFF()     						{SYS_BLUE_OUT(); SYS_BLUE_LOW();}

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

#if defined(_ENABLE_UART_LOW_)
//	DAB_RX PIN1--------GPIO PC5(PIN15)
#define AUDIO_RX_GPIO						_B5_
#define AUDIO_RX_INIT()						do{JL_PORTC->PU |= AUDIO_RX_GPIO; \
                                            JL_PORTC->PD &= ~AUDIO_RX_GPIO; \
                                            JL_PORTC->HD &= ~AUDIO_RX_GPIO;}while(0)
#define AUDIO_RX_OUT()						(JL_PORTC->DIR &= ~AUDIO_RX_GPIO)
#define AUDIO_RX_IN()						(JL_PORTC->DIR |= AUDIO_RX_GPIO)
#define AUDIO_RX_HIGH()						(JL_PORTC->OUT |= AUDIO_RX_GPIO)
#define AUDIO_RX_LOW()						(JL_PORTC->OUT &= ~AUDIO_RX_GPIO)

//	DAB_TX PIN2--------GPIO PC4(PIN16)
#define AUDIO_TX_GPIO						_B4_
#define AUDIO_TX_INIT()						do{JL_PORTC->PU |= AUDIO_TX_GPIO; \
                                            JL_PORTC->PD &= ~AUDIO_TX_GPIO; \
                                            JL_PORTC->HD &= ~AUDIO_TX_GPIO;}while(0)
#define AUDIO_TX_OUT()						(JL_PORTC->DIR &= ~AUDIO_TX_GPIO)
#define AUDIO_TX_IN()						(JL_PORTC->DIR |= AUDIO_TX_GPIO)
#define AUDIO_TX_HIGH()						(JL_PORTC->OUT |= AUDIO_TX_GPIO)
#define AUDIO_TX_LOW()						(JL_PORTC->OUT &= ~AUDIO_TX_GPIO)
#endif

#endif

