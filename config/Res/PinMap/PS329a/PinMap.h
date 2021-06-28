#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"


//	ENCODE--------GPIO PR2(PIN6)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif

//	ACC PIN--------GPIO PR1(PIN5)
#define ACC_GPIO								PORTR1

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)
#if defined(PLANE_LIGHT_FUNC)
//	ILL_LIGHT_DET PIN--------GPIO A4(PIN28)//
#define ILL_LIGHT_DET_GPIO								_B4_
#define ILL_LIGHT_DET_INIT()								do{JL_PORTA->PU |= ILL_LIGHT_DET_GPIO; \
                                               	   JL_PORTA->PD &= ~ILL_LIGHT_DET_GPIO; \
                                               	   JL_PORTA->HD |= ILL_LIGHT_DET_GPIO;}while(0)
#define ILL_LIGHT_DET_HIGH()								(JL_PORTA->OUT |= ILL_LIGHT_DET_GPIO)
#define ILL_LIGHT_DET_LOW()								(JL_PORTA->OUT &= ~ILL_LIGHT_DET_GPIO)
#define ILL_LIGHT_DET_OUT()								(JL_PORTA->DIR &= ~ILL_LIGHT_DET_GPIO)
#define ILL_LIGHT_DET_IN()								(JL_PORTA->DIR |= ILL_LIGHT_DET_GPIO)
#define ILL_LIGHT_DET_GET()								(JL_PORTA->IN & ILL_LIGHT_DET_GPIO)
#define ILL_LIGHT_DET_CHECK()								(ILL_LIGHT_DET_GET() ? 0 : 1)

//	KB_LIGHT PIN--------GPIO A11(PIN25) 小灯功能 控制开关
#define KB_LIGHT_GPIO						_B11_

#define KB_LIGHT_INIT()						do{JL_PORTA->PU &= ~KB_LIGHT_GPIO; \
                                                   JL_PORTA->PD &= ~KB_LIGHT_GPIO; \
                                                   JL_PORTA->HD &= ~KB_LIGHT_GPIO;}while(0)
#define KB_LIGHT_HIGH()						 (JL_PORTA->OUT |= KB_LIGHT_GPIO)
#define KB_LIGHT_LOW()						 (JL_PORTA->OUT &= ~KB_LIGHT_GPIO)
#define KB_LIGHT_OUT()						 (JL_PORTA->DIR &= ~KB_LIGHT_GPIO)
#define KB_LIGHT_IN()					     (JL_PORTA->DIR |= KB_LIGHT_GPIO) 
#define KB_LIGHT_ON()     					{KB_LIGHT_OUT();KB_LIGHT_HIGH();}
#define KB_LIGHT_OFF()     					{KB_LIGHT_OUT();KB_LIGHT_LOW();}

//	LED PIN--------GPIO A10(PIN26)	面板LED灯开关控制脚
#define SYS_POWOR_LED_GPIO							    _B10_

#define SYS_POWOR_LED_INIT()							   do{JL_PORTA->PU &= ~SYS_POWOR_LED_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_POWOR_LED_GPIO; \
                                                   JL_PORTA->HD &= ~SYS_POWOR_LED_GPIO;}while(0)
#define SYS_POWOR_LED_HIGH()							    (JL_PORTA->OUT |= SYS_POWOR_LED_GPIO)
#define SYS_POWOR_LED_LOW()							    (JL_PORTA->OUT &= ~SYS_POWOR_LED_GPIO)
#define SYS_POWOR_LED_OUT()							    (JL_PORTA->DIR &= ~SYS_POWOR_LED_GPIO)
#define SYS_POWOR_LED_IN()							    (JL_PORTA->DIR |= SYS_POWOR_LED_GPIO) 

#define SYS_POWOR_LED_ON()     					{SYS_POWOR_LED_OUT();SYS_POWOR_LED_HIGH();}
#define SYS_POWOR_LED_OFF()     					{SYS_POWOR_LED_OUT();SYS_POWOR_LED_LOW();}
#endif
//	BATT_DET PIN--------GPIO A9(PIN27)电压异常检测脚
#if defined(SUPPORT_LOW_PWR_DET)
#define BATT_DET_PIN							AD_KEY_PA9
#endif

//	LCD_CE PIN--------GPIO C0(PIN22)
#define LCD_CE_GPIO								_B0_

#define LCD_CE_INIT()							do{JL_PORTC->PU &= ~LCD_CE_GPIO; \
                                                   JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                                   JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()							(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()							(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()							(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()								(JL_PORTC->DIR |= LCD_CE_GPIO)

//	LCD_DATA PIN--------GPIO A15(PIN20)
#define LCD_DATA_GPIO							_B15_

#define LCD_DATA_INIT()							do{JL_PORTA->PU &= ~LCD_DATA_GPIO; \
                                                   JL_PORTA->PD &= ~LCD_DATA_GPIO; \
                                                   JL_PORTA->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()							(JL_PORTA->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()							(JL_PORTA->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()							(JL_PORTA->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()							(JL_PORTA->DIR |= LCD_DATA_GPIO)

//	LCD_CLK PIN--------GPIO A14(PIN21)
#define LCD_CLK_GPIO							_B14_

#define LCD_CLK_INIT()							do{JL_PORTA->PU &= ~LCD_CLK_GPIO; \
                                                   JL_PORTA->PD &= ~LCD_CLK_GPIO; \
                                                   JL_PORTA->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()							(JL_PORTA->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()							(JL_PORTA->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()							(JL_PORTA->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()							(JL_PORTA->DIR |= LCD_CLK_GPIO) 


//	AUDIO_MUTE PIN--------GPIO PR0(PIN9)
#define AUDIO_MUTE_GPIO							PORTR0
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						(PORTR_OUT(AUDIO_MUTE_GPIO, 1))
#define AUDIO_MUTE_LOW()						(PORTR_OUT(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}

//	SYS_POW PIN--------GPIO PR3(PIN7)
#define SYS_POW_GPIO							PORTR3
#define SYS_POW_INIT()							do{PORTR_PU(SYS_POW_GPIO, 0);\
                                                   PORTR_PD(SYS_POW_GPIO, 0);}while(0)
#define SYS_POW_OUT()							(PORTR_DIR(SYS_POW_GPIO, 0))
#define SYS_POW_HIGH()							(PORTR_OUT(SYS_POW_GPIO, 1))
#define SYS_POW_LOW()							(PORTR_OUT(SYS_POW_GPIO, 0))
#if defined(_ENABLE_SYSPOW_LOW_)
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


//	I2C_SCL PIN--------GPIO PB12(PIN41)
#define I2C_SCL_GPIO							_B12_
#define I2C_SCL_INIT()							do{JL_PORTB->PU |= I2C_SCL_GPIO; \
                                                   JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                                   JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()							(JL_PORTB->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()							(JL_PORTB->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()							(JL_PORTB->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()							(JL_PORTB->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     						{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     						{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PB11(PIN42)
#define I2C_SDA_GPIO							_B11_

#define I2C_SDA_INIT()							do{JL_PORTB->PU |= I2C_SDA_GPIO; \
                                                   JL_PORTB->PD &= ~I2C_SDA_GPIO; \
                                                   JL_PORTB->HD &= ~I2C_SDA_GPIO;}while(0)
#define I2C_SDA_HIGH()							(JL_PORTB->OUT |= I2C_SDA_GPIO)
#define I2C_SDA_LOW()							(JL_PORTB->OUT &= ~I2C_SDA_GPIO)
#define I2C_SDA_OUT()							(JL_PORTB->DIR &= ~I2C_SDA_GPIO)
#define I2C_SDA_IN()							(JL_PORTB->DIR |= I2C_SDA_GPIO)
#define I2C_SDA_GET()							(JL_PORTB->IN & I2C_SDA_GPIO)
#define I2C_SDA_H()     						{I2C_SDA_OUT();I2C_SDA_HIGH();}
#define I2C_SDA_L()     						{I2C_SDA_OUT();I2C_SDA_LOW();}

//	KEY--------GPIO PB10(PIN43)
#define ADC_KEY1_PIN							AD_KEY_PB10

//	KEY2--------GPIO PB9(PIN44)
#define ADC_KEY2_PIN							AD_KEY_PB9
#if defined(SUPPORT_PARTY_CONTROL)
//	KEY3--------GPIO PB9(PIN29)
#define ADC_KEY3_PIN							AD_KEY_PA3
#endif
//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN								do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)


#if defined(_CHANGE_RADIO_RST_PIN_)
/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO PA2(PIN30)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B2_

#define RADIO_RESET_INIT()						do{JL_PORTA->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTA->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTA->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTA->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTA->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

#else
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
#endif

//	DTH PIN--------GPIO C11
#if defined(_SUPPORT_DTH_)

#define DTH_GPIO								_B0_

#define DTH_INIT()								do{JL_PORTA->PU |= DTH_GPIO; \
                                               	   JL_PORTA->PD &= ~DTH_GPIO; \
                                               	   JL_PORTA->HD |= DTH_GPIO;}while(0)
#define DTH_HIGH()								(JL_PORTA->OUT |= DTH_GPIO)
#define DTH_LOW()								(JL_PORTA->OUT &= ~DTH_GPIO)
#define DTH_OUT()								(JL_PORTA->DIR &= ~DTH_GPIO)
#define DTH_IN()								(JL_PORTA->DIR |= DTH_GPIO)
#define DTH_GET()								(JL_PORTA->IN & DTH_GPIO)
#define DTH_CHECK()								(DTH_GET() ? 0 : 1)

#endif

//lcd直驱com口重置PC012345   0011 1111
#define LCD_COM_GPIO						0X3F
#define LCD_COM_INIT()						do{JL_PORTC->PU &= ~LCD_COM_GPIO; \
                                            JL_PORTC->PD &= ~LCD_COM_GPIO; \
                                            JL_PORTC->HD &= ~LCD_COM_GPIO;}while(0)
#define LCD_COM_OUT()						(JL_PORTC->DIR &= ~LCD_COM_GPIO)
#define LCD_COM_IN()						(JL_PORTC->DIR |= LCD_COM_GPIO)
#define LCD_COM_HIGH()						(JL_PORTC->OUT |= LCD_COM_GPIO)
#define LCD_COM_LOW()						(JL_PORTC->OUT &= ~LCD_COM_GPIO)
#define LCD_COM_OFF()						{LCD_COM_IN();}


//lcd直驱seg口重置 PA 3 4 9 10 11 14 15  
#define LCD_SEG_GPIO						0XCF18
#define LCD_SEG_INIT()						do{JL_PORTA->PU &= ~LCD_SEG_GPIO; \
                                            JL_PORTA->PD &= ~LCD_SEG_GPIO; \
                                            JL_PORTA->HD &= ~LCD_SEG_GPIO;}while(0)
#define LCD_SEG_OUT()						(JL_PORTA->DIR &= ~LCD_SEG_GPIO)
#define LCD_SEG_IN()						(JL_PORTA->DIR |= LCD_SEG_GPIO)
#define LCD_SEG_HIGH()						(JL_PORTA->OUT |= LCD_SEG_GPIO)
#define LCD_SEG_LOW()						(JL_PORTA->OUT &= ~LCD_SEG_GPIO)
#define LCD_SEG_OFF()						{LCD_SEG_IN();}

#endif

