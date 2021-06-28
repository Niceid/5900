#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"


//	ENCODE--------GPIO PR2(PIN6)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PB8
#endif
//	KEY--------GPIO PA10(PIN26)
#define ADC_KEY1_PIN							AD_KEY_PA10

//	KEY2--------GPIO PA9(PIN27)
#define ADC_KEY2_PIN							AD_KEY_PA9

//	IR PIN--------GPIO PB12(PIN41)
#define IR_INIT_PIN								do{JL_PORTB->DIR |= _B12_; JL_PORTB->DIE |= _B12_;}while(0)


//	ACC PIN--------空
#define ACC_GPIO								PORTR1

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)



//	AUDIO_MUTE PIN--------GPIO PA11(PIN25)
#define AUDIO_MUTE_GPIO							_B11_
#define AUDIO_MUTE_INIT()						do{JL_PORTA->PU &= ~AUDIO_MUTE_GPIO; \
                                                   JL_PORTA->PD &= ~AUDIO_MUTE_GPIO; \
                                                   JL_PORTA->HD &= ~AUDIO_MUTE_GPIO;}while(0)
#define AUDIO_MUTE_OUT()						(JL_PORTA->DIR &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_HIGH()						(JL_PORTA->OUT |= AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_LOW()						(JL_PORTA->OUT &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}

//	SYS_POW PIN--------GPIO PB10(PIN43)
#define SYS_POW_GPIO							_B10_
#define SYS_POW_INIT()							do{JL_PORTB->PU &= ~SYS_POW_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_POW_GPIO; \
                                                   JL_PORTB->HD &= ~SYS_POW_GPIO;}while(0)
#define SYS_POW_OUT()							(JL_PORTB->DIR &= ~SYS_POW_GPIO)
#define SYS_POW_HIGH()							(JL_PORTB->OUT |= SYS_POW_GPIO)
#define SYS_POW_LOW()							(JL_PORTB->OUT &= ~SYS_POW_GPIO)
#define SYS_POW_ON()     						{SYS_POW_OUT(); SYS_POW_HIGH();}
#define SYS_POW_OFF()     						{SYS_POW_OUT(); SYS_POW_LOW();}
#define SYS_POW_IN()							(JL_PORTB->DIR |= SYS_POW_GPIO)

#if defined(_VFD_DRIVER_IC_TM1628_)//TM1628  iic驱动
//	DOT_SDA--------GPIO PC1(PIN19)
#define LCD_DATA_GPIO						_B1_
#define LCD_DATA_INIT()						do{JL_PORTC->PU |= LCD_DATA_GPIO; \
                                            JL_PORTC->PD &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTC->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTC->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTC->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()						(JL_PORTC->DIR |= LCD_DATA_GPIO)

//DOT_SCL--------GPIO PA15(PIN20)
#define LCD_CLK_GPIO						_B15_
#define LCD_CLK_INIT()						do{JL_PORTA->PU |= LCD_CLK_GPIO; \
                                            JL_PORTA->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTA->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTA->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTA->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTA->DIR |= LCD_CLK_GPIO)

//DOT_CE--------GPIO PC5(PIN15)
#define LCD_CE_GPIO							_B5_
#define LCD_CE_INIT()						do{JL_PORTC->PU |= LCD_CE_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()						(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()						(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()						(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()							(JL_PORTC->DIR |= LCD_CE_GPIO)
#endif


#if defined(_SUPPORT_RADIO_)// 音效ic pt2313   和收音ic  共用iic
//	I2C_SCL PIN--------GPIO PA14(PIN21)
#define I2C_SCL_GPIO							_B14_
#define I2C_SCL_INIT()							do{JL_PORTA->PU |= I2C_SCL_GPIO; \
                                                   JL_PORTA->PD &= ~I2C_SCL_GPIO; \
                                                   JL_PORTA->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()							(JL_PORTA->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()							(JL_PORTA->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()							(JL_PORTA->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()							(JL_PORTA->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     						{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     						{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PC0(PIN22)
#define I2C_SDA_GPIO							_B0_

#define I2C_SDA_INIT()							do{JL_PORTC->PU |= I2C_SDA_GPIO; \
                                                   JL_PORTC->PD &= ~I2C_SDA_GPIO; \
                                                   JL_PORTC->HD &= ~I2C_SDA_GPIO;}while(0)
#define I2C_SDA_HIGH()							(JL_PORTC->OUT |= I2C_SDA_GPIO)
#define I2C_SDA_LOW()							(JL_PORTC->OUT &= ~I2C_SDA_GPIO)
#define I2C_SDA_OUT()							(JL_PORTC->DIR &= ~I2C_SDA_GPIO)
#define I2C_SDA_IN()							(JL_PORTC->DIR |= I2C_SDA_GPIO)
#define I2C_SDA_GET()							(JL_PORTC->IN & I2C_SDA_GPIO)
#define I2C_SDA_H()     						{I2C_SDA_OUT();I2C_SDA_HIGH();}
#define I2C_SDA_L()     						{I2C_SDA_OUT();I2C_SDA_LOW();}

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO PB9(PIN44)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B9_

#define RADIO_RESET_INIT()						do{JL_PORTB->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTB->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTB->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTB->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTB->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}
#endif


//	DTH PIN--------GPIO C11
#if defined(_SUPPORT_DTH_)
#define DTH_GPIO								_B30_
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

#endif
