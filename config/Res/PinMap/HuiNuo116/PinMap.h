#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"


//	ENCODE--------GPIO PR2(PIN6)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif


#if defined(_SUPPORT_VR2_)
#define ADC_ENCODE2_PIN		                    AD_KEY_PB10
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

//	AUDIO_MUTE PIN--------GPIO PR0(PIN9)
#define AUDIO_MUTE_GPIO							PORTR0
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						(PORTR_OUT(AUDIO_MUTE_GPIO, 1))
#define AUDIO_MUTE_LOW()						(PORTR_OUT(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}

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


//	I2C_SCL PIN--------GPIO PB6(PIN47)
#define I2C_SCL_GPIO							_B1_//_B6_
#define I2C_SCL_INIT()							do{JL_PORTB->PU |= I2C_SCL_GPIO; \
                                                   JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                                   JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()							(JL_PORTB->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()							(JL_PORTB->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()							(JL_PORTB->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()							(JL_PORTB->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     						{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     						{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PB5(PIN48)
#define I2C_SDA_GPIO							_B0_//_B5_

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
#if !defined(_DISABLE_K1_FUNTION_)
#define ADC_KEY1_PIN							AD_KEY_PB10
#endif

//	KEY2--------GPIO PB9(PIN44)
#define ADC_KEY2_PIN							AD_KEY_PB9

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN								do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)


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

#if defined(DRIVE_lCD_CLEAR)
//lcd直驱com口重置PC12345   0011 1111
#define LCD_COM_GPIO							0X3E
#define LCD_COM_INIT()						do{JL_PORTC->PU &= ~LCD_COM_GPIO; \
                                            JL_PORTC->PD &= ~LCD_COM_GPIO; \
                                            JL_PORTC->HD &= ~LCD_COM_GPIO;}while(0)
#define LCD_COM_OUT()						(JL_PORTC->DIR &= ~LCD_COM_GPIO)
#define LCD_COM_IN()							(JL_PORTC->DIR |= LCD_COM_GPIO)
#define LCD_COM_HIGH()						(JL_PORTC->OUT |= LCD_COM_GPIO)
#define LCD_COM_LOW()						(JL_PORTC->OUT &= ~LCD_COM_GPIO)
#define LCD_COM_OFF()						{LCD_COM_IN();}//{LCD_COM_OUT();LCD_COM_LOW();}


//lcd直驱seg口重置 PA 1 2 3 4 9 10 11 14 15  
#define LCD_SEG_GPIO							0XCE1E
#define LCD_SEG_INIT()						do{JL_PORTA->PU &= ~LCD_SEG_GPIO; \
                                            JL_PORTA->PD &= ~LCD_SEG_GPIO; \
                                            JL_PORTA->HD &= ~LCD_SEG_GPIO;}while(0)
#define LCD_SEG_HIGH()						(JL_PORTA->OUT |= LCD_SEG_GPIO)
#define LCD_SEG_LOW()						(JL_PORTA->OUT &= ~LCD_SEG_GPIO)
#define LCD_SEG_OUT()						(JL_PORTA->DIR &= ~LCD_SEG_GPIO)
#define LCD_SEG_IN()						(JL_PORTA->DIR |= LCD_SEG_GPIO)
#define LCD_SEG_OFF()						{LCD_SEG_IN();}//{LCD_SEG_OUT();LCD_SEG_LOW();}

#endif

#endif

