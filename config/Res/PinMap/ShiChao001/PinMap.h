#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

//	ACC PIN--------GPIO PR1(PIN9)
#define ACC_GPIO								_B30_

#define ACC_INIT()								do{JL_PORTA->PU |= ACC_GPIO; \
                                               	   JL_PORTA->PD &= ~ACC_GPIO; \
                                               	   JL_PORTA->HD |= ACC_GPIO;}while(0)
#define ACC_GET()								(JL_PORTA->IN & ACC_GPIO)
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)

//	ENCODE--------GPIO PR2(PIN10)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif


//	SYS_POW PIN--------GPIO PR0(PIN13)
#define SYS_POW_GPIO							PORTR0
#define SYS_POW_INIT()							do{PORTR_PU(SYS_POW_GPIO, 1);\
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

//	AUDIO_MUTE PIN--------GPIO PR3(PIN11) //无MUTE脚
#define AUDIO_MUTE_GPIO							_B30_
#define AUDIO_MUTE_INIT()						do{JL_PORTA->PU &= ~AUDIO_MUTE_GPIO; \
                                                   JL_PORTA->PD &= ~AUDIO_MUTE_GPIO; \
                                                   JL_PORTA->HD &= ~AUDIO_MUTE_GPIO;}while(0)
#define AUDIO_MUTE_OUT()						(JL_PORTA->DIR &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_HIGH()						(JL_PORTA->OUT |= AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_LOW()						(JL_PORTA->OUT &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}
                                            
//	I2C_SCL PIN--------GPIO PB11(PIN55)
#define I2C_SCL_GPIO						_B30_

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
#define I2C_SDA_GPIO						_B30_

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

/*--------------------第一组--------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CLK PIN--------GPIO B6(PIN63)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_CLK_GPIO								_B6_

#define LCD_CLK_INIT()							do{JL_PORTB->PU &= ~LCD_CLK_GPIO; \
                                                   JL_PORTB->PD &= ~LCD_CLK_GPIO; \
                                                   JL_PORTB->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()							(JL_PORTB->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()							(JL_PORTB->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()							(JL_PORTB->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()							(JL_PORTB->DIR |= LCD_CLK_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_DATA PIN--------GPIO B7(PIN61)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_DATA_GPIO							_B7_

#define LCD_DATA_INIT()							do{JL_PORTB->PU &= ~LCD_DATA_GPIO; \
                                                   JL_PORTB->PD &= ~LCD_DATA_GPIO; \
                                                   JL_PORTB->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()							(JL_PORTB->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()							(JL_PORTB->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()							(JL_PORTB->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()							(JL_PORTB->DIR |= LCD_DATA_GPIO) 

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CEPIN--------GPIO B5(PIN1)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD_CE_GPIO								_B5_

#define LCD_CE_INIT()							do{JL_PORTB->PU &= ~LCD_CE_GPIO; \
                                                   JL_PORTB->PD &= ~LCD_CE_GPIO; \
                                                   JL_PORTB->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()							(JL_PORTB->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()							(JL_PORTB->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()							(JL_PORTB->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()								(JL_PORTB->DIR |= LCD_CE_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
/*-------------------第二组-------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CLK PIN--------GPIO  PR2(PIN10)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD1_CLK_GPIO							PORTR2

#define LCD1_CLK_INIT()							do{PORTR_PU(LCD1_CLK_GPIO, 1);\
                                               	   PORTR_PD(LCD1_CLK_GPIO, 0);\
                                                   PORTR_HD(LCD1_CLK_GPIO, 0);}while(0)
#define LCD1_CLK_HIGH()							(PORTR_OUT(LCD1_CLK_GPIO, 1))
#define LCD1_CLK_LOW()							(PORTR_OUT(LCD1_CLK_GPIO, 0))
#define LCD1_CLK_OUT()							(PORTR_DIR(LCD1_CLK_GPIO, 0))
#define LCD1_CLK_IN()							(PORTR_DIR(LCD1_CLK_GPIO, 1))

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_DATA PIN--------GPIO PR1(PIN9)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD1_DATA_GPIO							PORTR1

#define LCD1_DATA_INIT()						do{PORTR_PU(LCD1_DATA_GPIO, 1);\
                                               	   PORTR_PD(LCD1_DATA_GPIO, 0);\
                                                   PORTR_HD(LCD1_DATA_GPIO, 0);}while(0)
#define LCD1_DATA_HIGH()						(PORTR_OUT(LCD1_DATA_GPIO, 1))
#define LCD1_DATA_LOW()							(PORTR_OUT(LCD1_DATA_GPIO, 0))
#define LCD1_DATA_OUT()							(PORTR_DIR(LCD1_DATA_GPIO, 0))
#define LCD1_DATA_IN()							(PORTR_DIR(LCD1_DATA_GPIO, 1))

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CEPIN--------GPIO PR3(PIN11)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD1_CE_GPIO							PORTR1

#define LCD1_CE_INIT()							do{PORTR_PU(LCD1_CE_GPIO, 1);\
                                               	   PORTR_PD(LCD1_CE_GPIO, 0);\
                                                   PORTR_HD(LCD1_CE_GPIO, 0);}while(0)
#define LCD1_CE_HIGH()							(PORTR_OUT(LCD1_CE_GPIO, 1))
#define LCD1_CE_LOW()							(PORTR_OUT(LCD1_CE_GPIO, 0))
#define LCD1_CE_OUT()							(PORTR_DIR(LCD1_CE_GPIO, 0))
#define LCD1_CE_IN()							(PORTR_DIR(LCD1_CE_GPIO, 1))

/*----------------------------------------------------------------------------------------------------------------*/
/*-------------------第三组-------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CLK PIN--------GPIO A5(PIN39)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD2_CLK_GPIO								_B5_

#define LCD2_CLK_INIT()							do{JL_PORTA->PU &= ~LCD2_CLK_GPIO; \
                                                   JL_PORTA->PD &= ~LCD2_CLK_GPIO; \
                                                   JL_PORTA->HD &= ~LCD2_CLK_GPIO;}while(0)
#define LCD2_CLK_HIGH()							(JL_PORTA->OUT |= LCD2_CLK_GPIO)
#define LCD2_CLK_LOW()							(JL_PORTA->OUT &= ~LCD2_CLK_GPIO)
#define LCD2_CLK_OUT()							(JL_PORTA->DIR &= ~LCD2_CLK_GPIO)
#define LCD2_CLK_IN()							(JL_PORTA->DIR |= LCD2_CLK_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_DATA PIN--------GPIO A6(PIN38)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD2_DATA_GPIO								_B6_

#define LCD2_DATA_INIT()						do{JL_PORTA->PU &= ~LCD2_DATA_GPIO; \
                                                   JL_PORTA->PD &= ~LCD2_DATA_GPIO; \
                                                   JL_PORTA->HD &= ~LCD2_DATA_GPIO;}while(0)
#define LCD2_DATA_HIGH()						(JL_PORTA->OUT |= LCD2_DATA_GPIO)
#define LCD2_DATA_LOW()							(JL_PORTA->OUT &= ~LCD2_DATA_GPIO)
#define LCD2_DATA_OUT()							(JL_PORTA->DIR &= ~LCD2_DATA_GPIO)
#define LCD2_DATA_IN()							(JL_PORTA->DIR |= LCD2_DATA_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CEPIN--------GPIO A4(PIN40)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD2_CE_GPIO								_B4_

#define LCD2_CE_INIT()							do{JL_PORTA->PU &= ~LCD2_CE_GPIO; \
                                                   JL_PORTA->PD &= ~LCD2_CE_GPIO; \
                                                   JL_PORTA->HD &= ~LCD2_CE_GPIO;}while(0)
#define LCD2_CE_HIGH()							(JL_PORTA->OUT |= LCD2_CE_GPIO)
#define LCD2_CE_LOW()							(JL_PORTA->OUT &= ~LCD2_CE_GPIO)
#define LCD2_CE_OUT()							(JL_PORTA->DIR &= ~LCD2_CE_GPIO)
#define LCD2_CE_IN()							(JL_PORTA->DIR |= LCD2_CE_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
/*-------------------第四组-------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CLK PIN--------GPIO B8(PIN36)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD3_CLK_GPIO							_B8_

#define LCD3_CLK_INIT()							do{JL_PORTA->PU &= ~LCD3_CLK_GPIO; \
                                                   JL_PORTA->PD &= ~LCD3_CLK_GPIO; \
                                                   JL_PORTA->HD &= ~LCD3_CLK_GPIO;}while(0)
#define LCD3_CLK_HIGH()							(JL_PORTA->OUT |= LCD3_CLK_GPIO)
#define LCD3_CLK_LOW()							(JL_PORTA->OUT &= ~LCD3_CLK_GPIO)
#define LCD3_CLK_OUT()							(JL_PORTA->DIR &= ~LCD3_CLK_GPIO)
#define LCD3_CLK_IN()							(JL_PORTA->DIR |= LCD3_CLK_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_DATA PIN--------GPIO A9(PIN35)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD3_DATA_GPIO							_B9_

#define LCD3_DATA_INIT()						do{JL_PORTA->PU &= ~LCD3_DATA_GPIO; \
                                                   JL_PORTA->PD &= ~LCD3_DATA_GPIO; \
                                                   JL_PORTA->HD &= ~LCD3_DATA_GPIO;}while(0)
#define LCD3_DATA_HIGH()						(JL_PORTA->OUT |= LCD3_DATA_GPIO)
#define LCD3_DATA_LOW()							(JL_PORTA->OUT &= ~LCD3_DATA_GPIO)
#define LCD3_DATA_OUT()							(JL_PORTA->DIR &= ~LCD3_DATA_GPIO)
#define LCD3_DATA_IN()							(JL_PORTA->DIR |= LCD3_DATA_GPIO)

/*----------------------------------------------------------------------------------------------------------------*/
//	LCD_CEPIN--------GPIO A7(PIN37)
/*----------------------------------------------------------------------------------------------------------------*/
#define LCD3_CE_GPIO							_B7_

#define LCD3_CE_INIT()							do{JL_PORTA->PU &= ~LCD3_CE_GPIO; \
                                                   JL_PORTA->PD &= ~LCD3_CE_GPIO; \
                                                   JL_PORTA->HD &= ~LCD3_CE_GPIO;}while(0)
#define LCD3_CE_HIGH()							(JL_PORTA->OUT |= LCD3_CE_GPIO)
#define LCD3_CE_LOW()							(JL_PORTA->OUT &= ~LCD3_CE_GPIO)
#define LCD3_CE_OUT()							(JL_PORTA->DIR &= ~LCD3_CE_GPIO)
#define LCD3_CE_IN()							(JL_PORTA->DIR |= LCD3_CE_GPIO)

//	KEY--------GPIO PB8(PIN59)
//#define ADC_KEY1_PIN						AD_KEY_PB8

//	KEY2--------GPIO PB9(PIN57)
//#define ADC_KEY2_PIN						AD_KEY_PB9

//	IR PIN--------GPIO PB3(PIN58)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO B10(PIN56)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B30_

#define RADIO_RESET_INIT()						do{JL_PORTB->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTB->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTB->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTB->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTB->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

#if defined(_SUPPORT_PANEL_LIGHT_)&&0
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

#endif

