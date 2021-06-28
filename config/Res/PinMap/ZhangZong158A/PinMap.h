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

//	ENCODE--------GPIO PA4(PIN28)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PA4
#endif

//	SYS_POW PIN--------GPIO PB10(PIN43)

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
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}

//	DOT_SDA--------GPIO PC1(PIN19)
#define LCD_DATA_GPIO						_B30_
#define LCD_DATA_INIT()						do{JL_PORTC->PU &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->PD &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTC->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTC->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTC->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()						(JL_PORTC->DIR |= LCD_DATA_GPIO)

//DOT_SCL--------GPIO PA15(PIN20)
#define LCD_CLK_GPIO						_B30_
#define LCD_CLK_INIT()						do{JL_PORTA->PU &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTA->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTA->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTA->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTA->DIR |= LCD_CLK_GPIO) 

//NULL--------GPIO PC4(PIN15)
#define LCD_CE_GPIO							_B30_
#define LCD_CE_INIT()						do{JL_PORTC->PU &= ~LCD_CE_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()						(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()						(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()						(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()							(JL_PORTC->DIR |= LCD_CE_GPIO)

#if 0
//	I2C_SDA PIN--------GPIO PC0(PIN22)
#define I2C_SDA_GPIO						_B30_
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

//	I2C_SCL PIN--------GPIO PA14(PIN21)
#define I2C_SCL_GPIO						_B30_

#define I2C_SCL_INIT()						do{JL_PORTA->PU |= I2C_SCL_GPIO; \
                                            JL_PORTA->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTA->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()						(JL_PORTA->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()						(JL_PORTA->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()						(JL_PORTA->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()						(JL_PORTA->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}

#else
//	I2C_SCL PIN--------GPIO PORTR2(PIN6)
#define I2C_SCL_GPIO						PORTR2

#define I2C_SCL_INIT()						do{PORTR_PU(I2C_SCL_GPIO, 1);\
                                               PORTR_PD(I2C_SCL_GPIO, 0);\
                                               PORTR_HD(I2C_SCL_GPIO, 0);}while(0)

#define I2C_SCL_HIGH()						(PORTR_OUT(I2C_SCL_GPIO, 1))
#define I2C_SCL_LOW()						(PORTR_OUT(I2C_SCL_GPIO, 0))
#define I2C_SCL_OUT()						(PORTR_DIR(I2C_SCL_GPIO, 0))
#define I2C_SCL_IN()						(PORTR_DIR(I2C_SCL_GPIO, 1))
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PORTR3(PIN7)
#define I2C_SDA_GPIO						PORTR3

#define I2C_SDA_INIT()						do{PORTR_PU(I2C_SDA_GPIO, 1);\
                                               PORTR_PD(I2C_SDA_GPIO, 0);\
                                               PORTR_HD(I2C_SDA_GPIO, 0);}while(0)

#define I2C_SDA_HIGH()						(PORTR_OUT(I2C_SDA_GPIO, 1))
#define I2C_SDA_LOW()						(PORTR_OUT(I2C_SDA_GPIO, 0))
#define I2C_SDA_OUT()						(PORTR_DIR(I2C_SDA_GPIO, 0))
#define I2C_SDA_IN()						(PORTR_DIR(I2C_SDA_GPIO, 1))
#define I2C_SDA_GET()						(PORTR_IN(I2C_SDA_GPIO))
#define I2C_SDA_H()     					{I2C_SDA_OUT();I2C_SDA_HIGH();}
#define I2C_SDA_L()     					{I2C_SDA_OUT();I2C_SDA_LOW();}
#endif

#if defined(_ENABLE_PT2313_I2C_)
//	I2C_SCL PIN--------GPIO PORTR2(PIN6)
#define I2C_SCL_GPIO1						PORTR2

#define I2C_SCL_INIT1()						do{PORTR_PU(I2C_SCL_GPIO1, 1);\
                                               PORTR_PD(I2C_SCL_GPIO1, 0);\
                                               PORTR_HD(I2C_SCL_GPIO1, 0);}while(0)

#define I2C_SCL_HIGH1()						(PORTR_OUT(I2C_SCL_GPIO1, 1))
#define I2C_SCL_LOW1()						(PORTR_OUT(I2C_SCL_GPIO1, 0))
#define I2C_SCL_OUT1()						(PORTR_DIR(I2C_SCL_GPIO1, 0))
#define I2C_SCL_IN1()						(PORTR_DIR(I2C_SCL_GPIO1, 1))
//#define I2C_SCL_H1()     					{I2C_SCL_OUT1();I2C_SCL_HIGH1();}
//#define I2C_SCL_L1()     					{I2C_SCL_OUT1();I2C_SCL_LOW1();}
#define I2C_SCL_H1()     					I2C_SCL_HIGH1()
#define I2C_SCL_L1()     					I2C_SCL_LOW1()

//	I2C_SDA PIN--------GPIO PORTR3(PIN7)
#define I2C_SDA_GPIO1						PORTR3

#define I2C_SDA_INIT1()						do{PORTR_PU(I2C_SDA_GPIO1, 1);\
                                               PORTR_PD(I2C_SDA_GPIO1, 0);\
                                               PORTR_HD(I2C_SDA_GPIO1, 0);}while(0)

#define I2C_SDA_HIGH1()						(PORTR_OUT(I2C_SDA_GPIO1, 1))
#define I2C_SDA_LOW1()						(PORTR_OUT(I2C_SDA_GPIO1, 0))
#define I2C_SDA_OUT1()						(PORTR_DIR(I2C_SDA_GPIO1, 0))
#define I2C_SDA_IN1()						(PORTR_DIR(I2C_SDA_GPIO1, 1))
#define I2C_SDA_GET1()						(PORTR_IN(I2C_SDA_GPIO1))
//#define I2C_SDA_H1()     					{I2C_SDA_OUT1();I2C_SDA_HIGH1();}
//#define I2C_SDA_L1()     					{I2C_SDA_OUT1();I2C_SDA_LOW1();}
#define I2C_SDA_H1()     					I2C_SDA_HIGH1()
#define I2C_SDA_L1()     					I2C_SDA_LOW1()
#endif

//-----------------------------7pin  led-----------------------------
//	SEG6--------GPIO PC5(PIN15)======================================================
#define SEG6_GPIO							_B5_
#define SEG6_OFF()							do{JL_PORTC->OUT &= ~SEG6_GPIO;\
											JL_PORTC->DIR |=  SEG6_GPIO;\
											JL_PORTC->PU &= ~SEG6_GPIO; \
                                          	JL_PORTC->PD &= ~SEG6_GPIO; \
                                       		JL_PORTC->HD &= ~SEG6_GPIO;}while(0)

#define SEG6_LOW()							do{JL_PORTC->OUT &= ~SEG6_GPIO; \
                                              JL_PORTC->DIR &= ~SEG6_GPIO;}while(0)
                                              
#define SEG6_HIGH()							do{JL_PORTC->OUT |= SEG6_GPIO;\
											JL_PORTC->DIR &= ~SEG6_GPIO;\
											JL_PORTC->HD |= SEG6_GPIO;}while(0)

#define SEG6_DISABLE()							do{JL_PORTC->DIR |= SEG6_GPIO; \
												JL_PORTC->HD &= ~SEG6_GPIO;}while(0)

//	SEG5--------GPIO PC2(PIN18)=====================================================
#define SEG5_GPIO							_B2_
#define SEG5_OFF()							do{JL_PORTC->OUT &= ~SEG5_GPIO;\
											JL_PORTC->DIR |=  SEG5_GPIO;\
											JL_PORTC->PU &= ~SEG5_GPIO; \
                                          	JL_PORTC->PD &= ~SEG5_GPIO; \
                                       		JL_PORTC->HD &= ~SEG5_GPIO;}while(0)

#define SEG5_LOW()							do{JL_PORTC->OUT &= ~SEG5_GPIO; \
                                              JL_PORTC->DIR &= ~SEG5_GPIO;}while(0)
#define SEG5_HIGH()							do{JL_PORTC->OUT |= SEG5_GPIO;\
											JL_PORTC->DIR &= ~SEG5_GPIO;\
											JL_PORTC->HD |= SEG5_GPIO;}while(0)

#define SEG5_DISABLE()						do{JL_PORTC->DIR |= SEG5_GPIO; \
											JL_PORTC->HD &= ~SEG5_GPIO;}while(0)

//	SEG4--------GPIO PA14(PIN21)=====================================================
#define SEG4_GPIO							_B14_
#define SEG4_OFF()							do{JL_PORTA->OUT &= ~SEG4_GPIO;\
											JL_PORTA->DIR |=  SEG4_GPIO;\
											JL_PORTA->PU &= ~SEG4_GPIO; \
                                          	JL_PORTA->PD &= ~SEG4_GPIO; \
                                       		JL_PORTA->HD &= ~SEG4_GPIO;}while(0)

#define SEG4_LOW()							do{JL_PORTA->OUT &= ~SEG4_GPIO; \
                                              JL_PORTA->DIR &= ~SEG4_GPIO;}while(0)
#define SEG4_HIGH()							do{JL_PORTA->OUT |= SEG4_GPIO;\
											JL_PORTA->DIR &= ~SEG4_GPIO;\
											JL_PORTA->HD |= SEG4_GPIO;}while(0)

#define SEG4_DISABLE()						do{JL_PORTA->DIR |= SEG4_GPIO; \
											JL_PORTA->HD &= ~SEG4_GPIO;}while(0)

//	SEG3--------GPIO PA15(PIN20)======================================================
#define SEG3_GPIO							_B15_
#define SEG3_OFF()							do{JL_PORTA->OUT &= ~SEG3_GPIO;\
											JL_PORTA->DIR |=  SEG3_GPIO;\
											JL_PORTA->PU &= ~SEG3_GPIO; \
                                          	JL_PORTA->PD &= ~SEG3_GPIO; \
                                       		JL_PORTA->HD &= ~SEG3_GPIO;}while(0)

#define SEG3_LOW()							do{JL_PORTA->OUT &= ~SEG3_GPIO; \
                                              JL_PORTA->DIR &= ~SEG3_GPIO;}while(0)
#define SEG3_HIGH()							do{JL_PORTA->OUT |= SEG3_GPIO;\
											JL_PORTA->DIR &= ~SEG3_GPIO;\
											JL_PORTA->HD |= SEG3_GPIO;}while(0)

#define SEG3_DISABLE()						do{JL_PORTA->DIR |= SEG3_GPIO; \
											JL_PORTA->HD &= ~SEG3_GPIO;}while(0)

//	SEG2--------GPIO PC1(PIN19)=====================================================
#define SEG2_GPIO							_B1_
#define SEG2_OFF()							do{JL_PORTC->OUT &= ~SEG2_GPIO;\
											JL_PORTC->DIR |=  SEG2_GPIO;\
											JL_PORTC->PU &= ~SEG2_GPIO; \
                                          	JL_PORTC->PD &= ~SEG2_GPIO; \
                                       		JL_PORTC->HD &= ~SEG2_GPIO;}while(0)

#define SEG2_LOW()							do{JL_PORTC->OUT &= ~SEG2_GPIO; \
                                              JL_PORTC->DIR &= ~SEG2_GPIO;}while(0)
#define SEG2_HIGH()							do{JL_PORTC->OUT |= SEG2_GPIO;\
											JL_PORTC->DIR &= ~SEG2_GPIO;\
											JL_PORTC->HD |= SEG2_GPIO;}while(0)

#define SEG2_DISABLE()						do{JL_PORTC->DIR |= SEG2_GPIO; \
											JL_PORTC->HD &= ~SEG2_GPIO;}while(0)

//	SEG1--------GPIO PC3(PIN17)=========================================================
#define SEG1_GPIO							_B3_
#define SEG1_OFF()							do{JL_PORTC->OUT &= ~SEG1_GPIO;\
											JL_PORTC->DIR |=  SEG1_GPIO;\
											JL_PORTC->PU &= ~SEG1_GPIO; \
                                          	JL_PORTC->PD &= ~SEG1_GPIO; \
                                       		JL_PORTC->HD &= ~SEG1_GPIO;}while(0)

#define SEG1_LOW()							do{JL_PORTC->OUT &= ~SEG1_GPIO; \
                                              JL_PORTC->DIR &= ~SEG1_GPIO;}while(0)
#define SEG1_HIGH()							do{JL_PORTC->OUT |= SEG1_GPIO;\
											JL_PORTC->DIR &= ~SEG1_GPIO;\
											JL_PORTC->HD |= SEG1_GPIO;}while(0)

#define SEG1_DISABLE()						do{JL_PORTC->DIR |= SEG1_GPIO; \
											JL_PORTC->HD &= ~SEG1_GPIO;}while(0)

//	SEG0--------GPIO PAC5(PIN16)==========================================================
#define SEG0_GPIO							_B4_
#define SEG0_OFF()							do{JL_PORTC->OUT &= ~SEG0_GPIO;\
											JL_PORTC->DIR |=  SEG0_GPIO;\
											JL_PORTC->PU &= ~SEG0_GPIO; \
                                          	JL_PORTC->PD &= ~SEG0_GPIO; \
                                       		JL_PORTC->HD &= ~SEG0_GPIO;}while(0)

#define SEG0_LOW()							do{JL_PORTC->OUT &= ~SEG0_GPIO; \
                                              JL_PORTC->DIR &= ~SEG0_GPIO;}while(0)
#define SEG0_HIGH()							do{JL_PORTC->OUT |= SEG0_GPIO;\
											JL_PORTC->DIR &= ~SEG0_GPIO;\
											JL_PORTC->HD |= SEG0_GPIO;}while(0)

#define SEG0_DISABLE()						do{JL_PORTC->DIR |= SEG0_GPIO; \
											JL_PORTC->HD &= ~SEG0_GPIO;}while(0)

//	KEY1--------GPIO PA10(PIN26)
#define ADC_KEY1_PIN						AD_KEY_PA10

//	KEY2--------GPIO PA9(PIN27)
#define ADC_KEY2_PIN						AD_KEY_PA9

//	IR PIN--------GPIO PB12(PIN41)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B12_; JL_PORTB->DIE |= _B12_;}while(0)

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

#if defined(_SUPPORT_PANEL_LIGHT_)
/*----------PIN22(PC0)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_RED_GPIO							_B0_

#define SYS_RED_INIT()							do{JL_PORTC->PU |= SYS_RED_GPIO; \
                                                   JL_PORTC->PD &= ~SYS_RED_GPIO; \
                                                   JL_PORTC->HD |= SYS_RED_GPIO;}while(0)
#define SYS_RED_HIGH()							(JL_PORTC->OUT |= SYS_RED_GPIO)
#define SYS_RED_LOW()							(JL_PORTC->OUT &= ~SYS_RED_GPIO)
#define SYS_RED_OUT()							(JL_PORTC->DIR &= ~SYS_RED_GPIO)
#define SYS_RED_IN()							(JL_PORTC->DIR |= SYS_RED_GPIO)
#define SYS_RED_ON()     						{SYS_RED_OUT(); SYS_RED_HIGH();}
#define SYS_RED_OFF()     						{SYS_RED_OUT(); SYS_RED_LOW();}

/*-----------PIN42(PB11)-BLUE--------------------------------------------------------------------------------------------------*/
#define SYS_BLUE_GPIO							_B11_

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

#endif

