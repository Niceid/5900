
#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

//	ACC PIN--------GPIO PR1(PIN9)
#define ACC_GPIO								PORTR1

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 0);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)

//	ENCODE--------GPIO PR2(PIN10)
#if defined(_SUPPORT_VR_)
#if defined(_VR_SCAN_IO_)
/*----------------------------------------------------------------------------------------------------------------*/
//	VOL+
/*----------------------------------------------------------------------------------------------------------------*/
#define VOL_POSITIVE_GPIO						_B0_


#define VOL_POSITIVE_INIT()						do{JL_PORTA->PU |= VOL_POSITIVE_GPIO; \
                                               	 JL_PORTA->PD &= ~VOL_POSITIVE_GPIO; \
                                               	 JL_PORTA->HD |= VOL_POSITIVE_GPIO;}while(0)
#define VOL_POSITIVE_HIGH()						(JL_PORTA->OUT |= VOL_POSITIVE_GPIO)
#define VOL_POSITIVE_LOW()						(JL_PORTA->OUT &= ~VOL_POSITIVE_GPIO)
#define VOL_POSITIVE_OUT()						(JL_PORTA->DIR &= ~VOL_POSITIVE_GPIO)
#define VOL_POSITIVE_IN()						(JL_PORTA->DIR |= VOL_POSITIVE_GPIO)

#define VOL_POSITIVE_GET()						(JL_PORTA->IN & VOL_POSITIVE_GPIO)
#define VOL_POSITIVE_CHECK()					(VOL_POSITIVE_GET() ? 0 : 1)

/*----------------------------------------------------------------------------------------------------------------*/
//	VOL-
/*----------------------------------------------------------------------------------------------------------------*/
#define VOL_NEGATIVE_GPIO						_B3_


#define VOL_NEGATIVE_INIT()						do{JL_PORTA->PU |= VOL_NEGATIVE_GPIO; \
                                               	 JL_PORTA->PD &= ~VOL_NEGATIVE_GPIO; \
                                               	 JL_PORTA->HD |= VOL_NEGATIVE_GPIO;}while(0)
#define VOL_NEGATIVE_HIGH()						(JL_PORTA->OUT |= VOL_NEGATIVE_GPIO)
#define VOL_NEGATIVE_LOW()						(JL_PORTA->OUT &= ~VOL_NEGATIVE_GPIO)
#define VOL_NEGATIVE_OUT()						(JL_PORTA->DIR &= ~VOL_NEGATIVE_GPIO)
#define VOL_NEGATIVE_IN()						(JL_PORTA->DIR |= VOL_NEGATIVE_GPIO)

#define VOL_NEGATIVE_GET()						(JL_PORTA->IN & VOL_NEGATIVE_GPIO)
#define VOL_NEGATIVE_CHECK()					(VOL_NEGATIVE_GET() ? 0 : 1)

#else
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif
#endif


#if defined(_JTI2230_RTC_CLK_)||1
#define SYS_POW_GPIO							_B3_
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

#else
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

#endif
//	DOT_SDA--------GPIO PC3(PIN22)
#define LCD_DATA_GPIO						_B30_
#define LCD_DATA_INIT()						do{JL_PORTC->PU &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->PD &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTC->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTC->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTC->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()						(JL_PORTC->DIR |= LCD_DATA_GPIO)

//DOT_SCL--------GPIO PC4(PIN21)
#define LCD_CLK_GPIO						_B30_
#define LCD_CLK_INIT()						do{JL_PORTC->PU &= ~LCD_CLK_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTC->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTC->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTC->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTC->DIR |= LCD_CLK_GPIO)

//NULL--------GPIO PC5(PIN20)
#define LCD_CE_GPIO							_B30_
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

//	IR PIN--------GPIO PB3(PIN58)
#if defined(_JTI2230_RTC_CLK_)||1
#define IR_INIT_PIN	
#else
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)
#endif

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO B10(PIN56)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B10_

#define RADIO_RESET_INIT()						do{JL_PORTB->PU |= RADIO_RESET_GPIO; \
                                                   JL_PORTB->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->HD |= RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTB->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTB->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTB->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTB->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

#endif

#if defined(_E2PROM_DRIVER_AT24CXX_)

/*-----------PIN63(AB6)-----------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SDA_GPIO						_B6_

#define E2PROM_I2C_SDA_INIT()					do{JL_PORTB->PU |= E2PROM_I2C_SDA_GPIO; \
                                                   JL_PORTB->PD &= ~E2PROM_I2C_SDA_GPIO; \
                                                   JL_PORTB->HD |= E2PROM_I2C_SDA_GPIO;}while(0)
#define E2PROM_I2C_SDA_HIGH()					(JL_PORTB->OUT |= E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_LOW()					(JL_PORTB->OUT &= ~E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_OUT()					(JL_PORTB->DIR &= ~E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_IN()						(JL_PORTB->DIR |= E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_GET()					(JL_PORTB->IN & E2PROM_I2C_SDA_GPIO)
#define E2PROM_I2C_SDA_H()     					E2PROM_I2C_SDA_IN()
#define E2PROM_I2C_SDA_L()     					{E2PROM_I2C_SDA_OUT();E2PROM_I2C_SDA_LOW(); }

/*-----------PIN61(B7)---------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SCL_GPIO						_B7_

#define E2PROM_I2C_SCL_INIT()					do{JL_PORTB->PU |= E2PROM_I2C_SCL_GPIO; \
                                                   JL_PORTB->PD &= ~E2PROM_I2C_SCL_GPIO; \
                                                   JL_PORTB->HD |= E2PROM_I2C_SCL_GPIO;}while(0)
#define E2PROM_I2C_SCL_HIGH()					(JL_PORTB->OUT |= E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_LOW()					(JL_PORTB->OUT &= ~E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_OUT()					(JL_PORTB->DIR &= ~E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_IN()						(JL_PORTB->DIR |= E2PROM_I2C_SCL_GPIO)
#define E2PROM_I2C_SCL_H()     					E2PROM_I2C_SCL_IN()
#define E2PROM_I2C_SCL_L()     					{E2PROM_I2C_SCL_OUT();E2PROM_I2C_SCL_LOW();}
#endif

#if defined(_ENABLE_AMPLIFIER_CONTROL_)

#define AMPLIFIER_GPIO								PORTR2

#define AMPLIFIER_INIT()							do{PORTR_HD(AMPLIFIER_GPIO, 0);	\
												   	   PORTR_PU(AMPLIFIER_GPIO, 0); \
                                                   	   PORTR_PD(AMPLIFIER_GPIO, 0);}while(0)
#define AMPLIFIER_HIGH()							(PORTR_OUT(AMPLIFIER_GPIO, 1))
#define AMPLIFIER_LOW()								(PORTR_OUT(AMPLIFIER_GPIO, 0))
#define AMPLIFIER_OUT()								(PORTR_DIR(AMPLIFIER_GPIO, 0))
#define AMPLIFIER_IN()								(PORTR_DIR(AMPLIFIER_GPIO, 1))
#endif


#if defined(_ENABLE_BUG_DETECT_)
#define BUG_GPIO								_B8_

#define BUG_INIT()								do{JL_PORTA->PU |= BUG_GPIO; \
                                               	   JL_PORTA->PD &= ~BUG_GPIO; \
                                               	   JL_PORTA->HD |= BUG_GPIO;}while(0)
#define BUG_HIGH()								(JL_PORTA->OUT |= BUG_GPIO)
#define BUG_LOW()								(JL_PORTA->OUT &= ~BUG_GPIO)
#define BUG_OUT()								(JL_PORTA->DIR &= ~BUG_GPIO)
#define BUG_IN()								(JL_PORTA->DIR |= BUG_GPIO)

#define BUG_GET()								(JL_PORTA->IN & BUG_GPIO)
#define BUG_CHECK()								(BUG_GET() ? 0 : 1)
#endif

/*-------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------*/
#if 1
//	LED COM4--------GPIO PC5(PIN20)
#define LED_COM4_GPIO							_B5_
#define LED_COM4_OFF()						do{JL_PORTC->OUT &= ~LED_COM4_GPIO;\
												JL_PORTC->DIR |=  LED_COM4_GPIO;\
												JL_PORTC->PU &= ~LED_COM4_GPIO; \
                                          		JL_PORTC->PD &= ~LED_COM4_GPIO; \
                                       			JL_PORTC->HD &= ~LED_COM4_GPIO;}while(0)
#define LED_COM4_ON()						do{JL_PORTC->OUT |= LED_COM4_GPIO;\
												JL_PORTC->DIR &= ~LED_COM4_GPIO;\
												JL_PORTC->HD |= LED_COM4_GPIO;}while(0)

#define LED_COM4_DISABLE()						do{JL_PORTC->DIR |= LED_COM4_GPIO; \
												JL_PORTC->HD &= ~LED_COM4_GPIO;}while(0)

//	LED COM3--------GPIO PC4(PIN21)
#define LED_COM3_GPIO							_B4_
#define LED_COM3_OFF()						do{JL_PORTC->OUT &= ~LED_COM3_GPIO;\
												JL_PORTC->DIR |=  LED_COM3_GPIO;\
												JL_PORTC->PU &= ~LED_COM3_GPIO; \
                                          		JL_PORTC->PD &= ~LED_COM3_GPIO; \
                                       			JL_PORTC->HD &= ~LED_COM3_GPIO;}while(0)
#define LED_COM3_ON()						do{JL_PORTC->OUT |= LED_COM3_GPIO;\
												JL_PORTC->DIR &= ~LED_COM3_GPIO;\
												JL_PORTC->HD |= LED_COM3_GPIO;}while(0)

#define LED_COM3_DISABLE()						do{JL_PORTC->DIR |= LED_COM3_GPIO; \
												JL_PORTC->HD &= ~LED_COM3_GPIO;}while(0)

//	LED COM2--------GPIO PC3(PIN22)
#define LED_COM2_GPIO							_B3_
#define LED_COM2_OFF()						do{JL_PORTC->OUT &= ~LED_COM2_GPIO;\
												JL_PORTC->DIR |=  LED_COM2_GPIO;\
												JL_PORTC->PU &= ~LED_COM2_GPIO; \
                                          		JL_PORTC->PD &= ~LED_COM2_GPIO; \
                                       			JL_PORTC->HD &= ~LED_COM2_GPIO;}while(0)
#define LED_COM2_ON()						do{JL_PORTC->OUT |= LED_COM2_GPIO;\
												JL_PORTC->DIR &= ~LED_COM2_GPIO;\
												JL_PORTC->HD |= LED_COM2_GPIO;}while(0)

#define LED_COM2_DISABLE()						do{JL_PORTC->DIR |= LED_COM2_GPIO; \
												JL_PORTC->HD &= ~LED_COM2_GPIO;}while(0)

//	LED COM1--------GPIO PC2(PIN23)
#define LED_COM1_GPIO							_B2_
#define LED_COM1_OFF()						do{JL_PORTC->OUT &= ~LED_COM1_GPIO;\
												JL_PORTC->DIR |=  LED_COM1_GPIO;\
												JL_PORTC->PU &= ~LED_COM1_GPIO; \
                                          		JL_PORTC->PD &= ~LED_COM1_GPIO; \
                                       			JL_PORTC->HD &= ~LED_COM1_GPIO;}while(0)
#define LED_COM1_ON()						do{JL_PORTC->OUT |= LED_COM1_GPIO;\
												JL_PORTC->DIR &= ~LED_COM1_GPIO;\
												JL_PORTC->HD |= LED_COM1_GPIO;}while(0)

#define LED_COM1_DISABLE()						do{JL_PORTC->DIR |= LED_COM1_GPIO; \
												JL_PORTC->HD &= ~LED_COM1_GPIO;}while(0)

//	LED COM0--------GPIO PC1(PIN24)
#define LED_COM0_GPIO						_B1_
#define LED_COM0_OFF()						do{JL_PORTC->OUT &= ~LED_COM0_GPIO;\
												JL_PORTC->DIR |=  LED_COM0_GPIO;\
												JL_PORTC->PU &= ~LED_COM0_GPIO; \
                                          		JL_PORTC->PD &= ~LED_COM0_GPIO; \
                                       			JL_PORTC->HD &= ~LED_COM0_GPIO;}while(0)
#define LED_COM0_ON()						do{JL_PORTC->OUT |= LED_COM0_GPIO;\
												JL_PORTC->DIR &= ~LED_COM0_GPIO;\
												JL_PORTC->HD |= LED_COM0_GPIO;}while(0)

#define LED_COM0_DISABLE()						do{JL_PORTC->DIR |= LED_COM0_GPIO; \
												JL_PORTC->HD &= ~LED_COM0_GPIO;}while(0)

#endif

#if 0

//	LED_P0 PIN--------GPIO B6
/*----------------------------------------------------------------------------------------------------------------*/
#define LED_P0_GPIO								_B1_

#define LED_P0_INIT()							do{JL_PORTC->PU &= ~LED_P0_GPIO; \
                                                   JL_PORTC->PD &= ~LED_P0_GPIO; \
                                                   JL_PORTC->HD |= LED_P0_GPIO;}while(0)                          
#define LED_P0_HIGH()							(JL_PORTC->OUT |= LED_P0_GPIO)
#define LED_P0_LOW()							(JL_PORTC->OUT &= ~LED_P0_GPIO)
#define LED_P0_OUT()							(JL_PORTC->DIR &= ~LED_P0_GPIO)
#define LED_P0_IN()								(JL_PORTC->DIR |= LED_P0_GPIO)
#define LED_P0_DISABLE()						{LED_P0_IN();  JL_PORTC->HD &= ~LED_P0_GPIO;}
#define LED_P0_ENABLE()							{LED_P0_OUT(); JL_PORTC->HD |= LED_P0_GPIO;}     

#define LED_P1_GPIO								_B2_

#define LED_P1_INIT()							do{JL_PORTC->PU &= ~LED_P1_GPIO; \
                                                   JL_PORTC->PD &= ~LED_P1_GPIO; \
                                                   JL_PORTC->HD |= LED_P1_GPIO;}while(0)                          
#define LED_P1_HIGH()							(JL_PORTC->OUT |= LED_P1_GPIO)
#define LED_P1_LOW()							(JL_PORTC->OUT &= ~LED_P1_GPIO)
#define LED_P1_OUT()							(JL_PORTC->DIR &= ~LED_P1_GPIO)
#define LED_P1_IN()								(JL_PORTC->DIR |= LED_P1_GPIO)
#define LED_P1_DISABLE()						{LED_P1_IN();  JL_PORTC->HD &= ~LED_P1_GPIO;}
#define LED_P1_ENABLE()							{LED_P1_OUT(); JL_PORTC->HD |= LED_P1_GPIO;}     

#define LED_P2_GPIO								_B3_

#define LED_P2_INIT()							do{JL_PORTC->PU &= ~LED_P2_GPIO; \
                                                   JL_PORTC->PD &= ~LED_P2_GPIO; \
                                                   JL_PORTC->HD |= LED_P2_GPIO;}while(0)                          
#define LED_P2_HIGH()							(JL_PORTC->OUT |= LED_P2_GPIO)
#define LED_P2_LOW()							(JL_PORTC->OUT &= ~LED_P2_GPIO)
#define LED_P2_OUT()							(JL_PORTC->DIR &= ~LED_P2_GPIO)
#define LED_P2_IN()								(JL_PORTC->DIR |= LED_P2_GPIO)
#define LED_P2_DISABLE()						{LED_P2_IN();  JL_PORTC->HD &= ~LED_P2_GPIO;}
#define LED_P2_ENABLE()							{LED_P2_OUT(); JL_PORTC->HD |= LED_P2_GPIO;}     


#define LED_P3_GPIO								_B4_

#define LED_P3_INIT()							do{JL_PORTC->PU &= ~LED_P3_GPIO; \
                                                   JL_PORTC->PD &= ~LED_P3_GPIO; \
                                                   JL_PORTC->HD |= LED_P3_GPIO;}while(0)                          
#define LED_P3_HIGH()							(JL_PORTC->OUT |= LED_P3_GPIO)
#define LED_P3_LOW()							(JL_PORTC->OUT &= ~LED_P3_GPIO)
#define LED_P3_OUT()							(JL_PORTC->DIR &= ~LED_P3_GPIO)
#define LED_P3_IN()								(JL_PORTC->DIR |= LED_P3_GPIO)
#define LED_P3_DISABLE()						{LED_P3_IN();  JL_PORTC->HD &= ~LED_P3_GPIO;}
#define LED_P3_ENABLE()							{LED_P3_OUT(); JL_PORTC->HD |= LED_P3_GPIO;}     

#define LED_P4_GPIO								_B5_

#define LED_P4_INIT()							do{JL_PORTC->PU &= ~LED_P4_GPIO; \
                                                   JL_PORTC->PD &= ~LED_P4_GPIO; \
                                                   JL_PORTC->HD |= LED_P4_GPIO;}while(0)                          
#define LED_P4_HIGH()							(JL_PORTC->OUT |= LED_P4_GPIO)
#define LED_P4_LOW()							(JL_PORTC->OUT &= ~LED_P4_GPIO)
#define LED_P4_OUT()							(JL_PORTC->DIR &= ~LED_P4_GPIO)
#define LED_P4_IN()								(JL_PORTC->DIR |= LED_P4_GPIO)
#define LED_P4_DISABLE()						{LED_P4_IN();  JL_PORTC->HD &= ~LED_P4_GPIO;}
#define LED_P4_ENABLE()							{LED_P4_OUT(); JL_PORTC->HD |= LED_P4_GPIO;}     

/*----------------------------------------------------------------------------------------------------------------*/
//	LED_P5 PIN--------GPIO B3
/*----------------------------------------------------------------------------------------------------------------*/
#define LED_P5_GPIO								_B9_

#define LED_P5_INIT()							do{JL_PORTA->PU &= ~LED_P5_GPIO; \
                                                   JL_PORTA->PD &= ~LED_P5_GPIO; \
                                                   JL_PORTA->HD &= ~LED_P5_GPIO;}while(0)
#define LED_P5_HIGH()							(JL_PORTA->OUT |= LED_P5_GPIO)
#define LED_P5_LOW()							(JL_PORTA->OUT &= ~LED_P5_GPIO)
#define LED_P5_OUT()							(JL_PORTA->DIR &= ~LED_P5_GPIO)
#define LED_P5_IN()								(JL_PORTA->DIR |= LED_P5_GPIO)
#define LED_P5_DISABLE()						{LED_P5_IN();  JL_PORTA->HD &= ~LED_P5_GPIO;}
#define LED_P5_ENABLE()							{LED_P5_OUT(); JL_PORTA->HD |= LED_P5_GPIO;}     

#define LED_P6_GPIO								_B10_

#define LED_P6_INIT()							do{JL_PORTA->PU &= ~LED_P6_GPIO; \
                                                   JL_PORTA->PD &= ~LED_P6_GPIO; \
                                                   JL_PORTA->HD &= ~LED_P6_GPIO;}while(0)
#define LED_P6_HIGH()							(JL_PORTA->OUT |= LED_P6_GPIO)
#define LED_P6_LOW()							(JL_PORTA->OUT &= ~LED_P6_GPIO)
#define LED_P6_OUT()							(JL_PORTA->DIR &= ~LED_P6_GPIO)
#define LED_P6_IN()								(JL_PORTA->DIR |= LED_P6_GPIO)
#define LED_P6_DISABLE()						{LED_P6_IN();  JL_PORTA->HD &= ~LED_P6_GPIO;}
#define LED_P6_ENABLE()							{LED_P6_OUT(); JL_PORTA->HD |= LED_P6_GPIO;}     

#define LED_P7_GPIO								_B11_

#define LED_P7_INIT()							do{JL_PORTA->PU &= ~LED_P7_GPIO; \
                                                   JL_PORTA->PD &= ~LED_P7_GPIO; \
                                                   JL_PORTA->HD &= ~LED_P7_GPIO;}while(0)
#define LED_P7_HIGH()							(JL_PORTA->OUT |= LED_P7_GPIO)
#define LED_P7_LOW()							(JL_PORTA->OUT &= ~LED_P7_GPIO)
#define LED_P7_OUT()							(JL_PORTA->DIR &= ~LED_P7_GPIO)
#define LED_P7_IN()								(JL_PORTA->DIR |= LED_P5_GPIO)
#define LED_P7_DISABLE()						{LED_P7_IN();  JL_PORTA->HD &= ~LED_P7_GPIO;}
#define LED_P7_ENABLE()							{LED_P7_OUT(); JL_PORTA->HD |= LED_P7_GPIO;}     

#define LED_P8_GPIO								_B12_

#define LED_P8_INIT()							do{JL_PORTA->PU &= ~LED_P8_GPIO; \
                                                   JL_PORTA->PD &= ~LED_P8_GPIO; \
                                                   JL_PORTA->HD &= ~LED_P8_GPIO;}while(0)
#define LED_P8_HIGH()							(JL_PORTA->OUT |= LED_P8_GPIO)
#define LED_P8_LOW()							(JL_PORTA->OUT &= ~LED_P8_GPIO)
#define LED_P8_OUT()							(JL_PORTA->DIR &= ~LED_P8_GPIO)
#define LED_P8_IN()								(JL_PORTA->DIR |= LED_P8_GPIO)
#define LED_P8_DISABLE()						{LED_P8_IN();  JL_PORTA->HD &= ~LED_P8_GPIO;}
#define LED_P8_ENABLE()							{LED_P8_OUT(); JL_PORTA->HD |= LED_P8_GPIO;}     

#define LED_P9_GPIO								_B13_

#define LED_P9_INIT()							do{JL_PORTA->PU &= ~LED_P9_GPIO; \
                                                   JL_PORTA->PD &= ~LED_P9_GPIO; \
                                                   JL_PORTA->HD &= ~LED_P9_GPIO;}while(0)
#define LED_P9_HIGH()							(JL_PORTA->OUT |= LED_P9_GPIO)
#define LED_P9_LOW()							(JL_PORTA->OUT &= ~LED_P9_GPIO)
#define LED_P9_OUT()							(JL_PORTA->DIR &= ~LED_P9_GPIO)
#define LED_P9_IN()								(JL_PORTA->DIR |= LED_P9_GPIO)
#define LED_P9_DISABLE()						{LED_P9_IN();  JL_PORTA->HD &= ~LED_P9_GPIO;}
#define LED_P9_ENABLE()							{LED_P9_OUT(); JL_PORTA->HD |= LED_P9_GPIO;}     

#define LED_P10_GPIO							_B14_

#define LED_P10_INIT()							do{JL_PORTA->PU &= ~LED_P10_GPIO; \
                                                   JL_PORTA->PD &= ~LED_P10_GPIO; \
                                                   JL_PORTA->HD &= ~LED_P10_GPIO;}while(0)
#define LED_P10_HIGH()							(JL_PORTA->OUT |= LED_P10_GPIO)
#define LED_P10_LOW()							(JL_PORTA->OUT &= ~LED_P10_GPIO)
#define LED_P10_OUT()							(JL_PORTA->DIR &= ~LED_P10_GPIO)
#define LED_P10_IN()							(JL_PORTA->DIR |= LED_P10_GPIO)
#define LED_P10_DISABLE()						{LED_P10_IN();  JL_PORTA->HD &= ~LED_P10_GPIO;}
#define LED_P10_ENABLE()						{LED_P10_OUT(); JL_PORTA->HD |= LED_P10_GPIO;}     

#define LED_P11_GPIO							_B15_

#define LED_P11_INIT()							do{JL_PORTA->PU &= ~LED_P11_GPIO; \
                                                   JL_PORTA->PD &= ~LED_P11_GPIO; \
                                                   JL_PORTA->HD &= ~LED_P11_GPIO;}while(0)
#define LED_P11_HIGH()							(JL_PORTA->OUT |= LED_P11_GPIO)
#define LED_P11_LOW()							(JL_PORTA->OUT &= ~LED_P11_GPIO)
#define LED_P11_OUT()							(JL_PORTA->DIR &= ~LED_P11_GPIO)
#define LED_P11_IN()							(JL_PORTA->DIR |= LED_P11_GPIO)
#define LED_P11_DISABLE()						{LED_P11_IN();  JL_PORTA->HD &= ~LED_P11_GPIO;}
#define LED_P11_ENABLE()						{LED_P11_OUT(); JL_PORTA->HD |= LED_P11_GPIO;}     

#else
//	SEG7--------GPIO PA15(PIN26)
#define SEG6_GPIO							_B15_
#define SEG6_OFF()							do{JL_PORTA->OUT &= ~SEG6_GPIO;\
											JL_PORTA->DIR |=  SEG6_GPIO;\
											JL_PORTA->PU &= ~SEG6_GPIO; \
                                          	JL_PORTA->PD &= ~SEG6_GPIO; \
                                       		JL_PORTA->HD &= ~SEG6_GPIO;}while(0)

#define SEG6_ON()							do{JL_PORTA->OUT &= ~SEG6_GPIO; \
                                              JL_PORTA->DIR &= ~SEG6_GPIO;}while(0)

//	SEG6--------GPIO PA14(PIN27)
#define SEG5_GPIO							_B14_
#define SEG5_OFF()							do{JL_PORTA->OUT &= ~SEG5_GPIO;\
											JL_PORTA->DIR |=  SEG5_GPIO;\
											JL_PORTA->PU &= ~SEG5_GPIO; \
                                          	JL_PORTA->PD &= ~SEG5_GPIO; \
                                       		JL_PORTA->HD &= ~SEG5_GPIO;}while(0)

#define SEG5_ON()							do{JL_PORTA->OUT &= ~SEG5_GPIO; \
                                              JL_PORTA->DIR &= ~SEG5_GPIO;}while(0)


//	SEG5--------GPIO PA13(PIN31)
#define SEG4_GPIO							_B13_
#define SEG4_OFF()							do{JL_PORTA->OUT &= ~SEG4_GPIO;\
											JL_PORTA->DIR |=  SEG4_GPIO;\
											JL_PORTA->PU &= ~SEG4_GPIO; \
                                          	JL_PORTA->PD &= ~SEG4_GPIO; \
                                       		JL_PORTA->HD &= ~SEG4_GPIO;}while(0)

#define SEG4_ON()							do{JL_PORTA->OUT &= ~SEG4_GPIO; \
                                              JL_PORTA->DIR &= ~SEG4_GPIO;}while(0)

//	SEG4--------GPIO PA12(PIN32)
#define SEG3_GPIO							_B12_
#define SEG3_OFF()							do{JL_PORTA->OUT &= ~SEG3_GPIO;\
											JL_PORTA->DIR |=  SEG3_GPIO;\
											JL_PORTA->PU &= ~SEG3_GPIO; \
                                          	JL_PORTA->PD &= ~SEG3_GPIO; \
                                       		JL_PORTA->HD &= ~SEG3_GPIO;}while(0)

#define SEG3_ON()							do{JL_PORTA->OUT &= ~SEG3_GPIO; \
                                              JL_PORTA->DIR &= ~SEG3_GPIO;}while(0)

//	SEG3--------GPIO PA11(PIN33)
#define SEG2_GPIO							_B11_
#define SEG2_OFF()							do{JL_PORTA->OUT &= ~SEG2_GPIO;\
											JL_PORTA->DIR |=  SEG2_GPIO;\
											JL_PORTA->PU &= ~SEG2_GPIO; \
                                          	JL_PORTA->PD &= ~SEG2_GPIO; \
                                       		JL_PORTA->HD &= ~SEG2_GPIO;}while(0)

#define SEG2_ON()							do{JL_PORTA->OUT &= ~SEG2_GPIO; \
                                              JL_PORTA->DIR &= ~SEG2_GPIO;}while(0)

//	SEG2--------GPIO PA10(PIN34)
#define SEG1_GPIO							_B10_
#define SEG1_OFF()							do{JL_PORTA->OUT &= ~SEG1_GPIO;\
											JL_PORTA->DIR |=  SEG1_GPIO;\
											JL_PORTA->PU &= ~SEG1_GPIO; \
                                          	JL_PORTA->PD &= ~SEG1_GPIO; \
                                       		JL_PORTA->HD &= ~SEG1_GPIO;}while(0)

#define SEG1_ON()							do{JL_PORTA->OUT &= ~SEG1_GPIO; \
                                              JL_PORTA->DIR &= ~SEG1_GPIO;}while(0)

//	SEG0--------GPIO PA9(PIN35)
#define SEG0_GPIO							_B9_
#define SEG0_OFF()							do{JL_PORTA->OUT &= ~SEG0_GPIO;\
											JL_PORTA->DIR |=  SEG0_GPIO;\
											JL_PORTA->PU &= ~SEG0_GPIO; \
                                          	JL_PORTA->PD &= ~SEG0_GPIO; \
                                       		JL_PORTA->HD &= ~SEG0_GPIO;}while(0)

#define SEG0_ON()							do{JL_PORTA->OUT &= ~SEG0_GPIO; \
                                              JL_PORTA->DIR &= ~SEG0_GPIO;}while(0)

#endif

