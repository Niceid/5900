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


//	AUDIO_MUTE PIN--------GPIO PR0(PIN9)
#define AUDIO_MUTE_GPIO							PORTR0
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						(PORTR_OUT(AUDIO_MUTE_GPIO, 1))
#define AUDIO_MUTE_LOW()						(PORTR_OUT(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}


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

//-----------------------------7pin  led-----------------------------
//	SEG6--------GPIO PA3(PIN29)======================================================
#define SEG6_GPIO							_B1_
#define SEG6_OFF()							do{JL_PORTC->OUT &= ~SEG6_GPIO;\
											JL_PORTC->DIR |=  SEG6_GPIO;\
											JL_PORTC->PU &= ~SEG6_GPIO; \
                                          	JL_PORTC->PD &= ~SEG6_GPIO; \
                                       		JL_PORTC->HD &= ~SEG6_GPIO;}while(0)

#define SEG6_LOW()							do{JL_PORTC->OUT &= ~SEG6_GPIO; \
                                              JL_PORTC->DIR &= ~SEG6_GPIO;\
											   JL_PORTC->HD |= SEG6_GPIO;}while(0)

#define SEG6_HIGH()							do{JL_PORTC->OUT |= SEG6_GPIO;\
												JL_PORTC->DIR &= ~SEG6_GPIO;}while(0)

#define SEG6_DISABLE()							do{JL_PORTC->DIR |= SEG6_GPIO; \
												JL_PORTC->HD &= ~SEG6_GPIO;}while(0)

//	SEG5--------GPIO PA4(PIN28)=====================================================
#define SEG5_GPIO							_B15_
#define SEG5_OFF()							do{JL_PORTA->OUT &= ~SEG5_GPIO;\
											JL_PORTA->DIR |=  SEG5_GPIO;\
											JL_PORTA->PU &= ~SEG5_GPIO; \
                                          	JL_PORTA->PD &= ~SEG5_GPIO; \
                                       		JL_PORTA->HD &= ~SEG5_GPIO;}while(0)

#define SEG5_LOW()							do{JL_PORTA->OUT &= ~SEG5_GPIO; \
                                              JL_PORTA->DIR &= ~SEG5_GPIO;\
											   JL_PORTA->HD |= SEG5_GPIO;}while(0)

#define SEG5_HIGH()							do{JL_PORTA->OUT |= SEG5_GPIO;\
												JL_PORTA->DIR &= ~SEG5_GPIO;}while(0)

#define SEG5_DISABLE()							do{JL_PORTA->DIR |= SEG5_GPIO; \
												  JL_PORTA->HD &= ~SEG5_GPIO;}while(0)

//	SEG4--------GPIO PA9(PIN27)=====================================================
#define SEG4_GPIO							_B14_
#define SEG4_OFF()							do{JL_PORTA->OUT &= ~SEG4_GPIO;\
											JL_PORTA->DIR |=  SEG4_GPIO;\
											JL_PORTA->PU &= ~SEG4_GPIO; \
                                          	JL_PORTA->PD &= ~SEG4_GPIO; \
                                       		JL_PORTA->HD &= ~SEG4_GPIO;}while(0)

#define SEG4_LOW()							do{JL_PORTA->OUT &= ~SEG4_GPIO; \
                                              JL_PORTA->DIR &= ~SEG4_GPIO;\
											   JL_PORTA->HD |= SEG4_GPIO;}while(0)

#define SEG4_HIGH()							do{JL_PORTA->OUT |= SEG4_GPIO;\
												JL_PORTA->DIR &= ~SEG4_GPIO;}while(0)

#define SEG4_DISABLE()							do{JL_PORTA->DIR |= SEG4_GPIO; \
                                              JL_PORTA->HD &= ~SEG4_GPIO;}while(0)

//	SEG3--------GPIO PA10(PIN26)======================================================
#define SEG3_GPIO							_B0_
#define SEG3_OFF()							do{JL_PORTC->OUT &= ~SEG3_GPIO;\
											JL_PORTC->DIR |=  SEG3_GPIO;\
											JL_PORTC->PU &= ~SEG3_GPIO; \
                                          	JL_PORTC->PD &= ~SEG3_GPIO; \
                                       		JL_PORTC->HD &= ~SEG3_GPIO;}while(0)

#define SEG3_LOW()							do{JL_PORTC->OUT &= ~SEG3_GPIO; \
                                              JL_PORTC->DIR &= ~SEG3_GPIO;\
											   JL_PORTC->HD |= SEG3_GPIO;}while(0)

#define SEG3_HIGH()							do{JL_PORTC->OUT |= SEG3_GPIO;\
												JL_PORTC->DIR &= ~SEG3_GPIO;}while(0)

#define SEG3_DISABLE()							do{JL_PORTC->DIR |= SEG3_GPIO; \
                                              JL_PORTC->HD &= ~SEG3_GPIO;}while(0)

//	SEG2--------GPIO PA11(PIN25)=====================================================
#define SEG2_GPIO							_B11_
#define SEG2_OFF()							do{JL_PORTA->OUT &= ~SEG2_GPIO;\
											JL_PORTA->DIR |=  SEG2_GPIO;\
											JL_PORTA->PU &= ~SEG2_GPIO; \
                                          	JL_PORTA->PD &= ~SEG2_GPIO; \
                                       		JL_PORTA->HD &= ~SEG2_GPIO;}while(0)

#define SEG2_LOW()							do{JL_PORTA->OUT &= ~SEG2_GPIO; \
                                              JL_PORTA->DIR &= ~SEG2_GPIO;\
											   JL_PORTA->HD |= SEG2_GPIO;}while(0)

#define SEG2_HIGH()							do{JL_PORTA->OUT |= SEG2_GPIO;\
												JL_PORTA->DIR &= ~SEG2_GPIO;}while(0)

#define SEG2_DISABLE()							do{JL_PORTA->DIR |= SEG2_GPIO; \
												  JL_PORTA->HD &= ~SEG2_GPIO;}while(0)

//	SEG1--------GPIO PA14(PIN21)=========================================================
#define SEG1_GPIO							_B10_
#define SEG1_OFF()							do{JL_PORTA->OUT &= ~SEG1_GPIO;\
											JL_PORTA->DIR |=  SEG1_GPIO;\
											JL_PORTA->PU &= ~SEG1_GPIO; \
                                          	JL_PORTA->PD &= ~SEG1_GPIO; \
                                       		JL_PORTA->HD &= ~SEG1_GPIO;}while(0)

#define SEG1_LOW()							do{JL_PORTA->OUT &= ~SEG1_GPIO; \
                                              JL_PORTA->DIR &= ~SEG1_GPIO;\
											   JL_PORTA->HD |= SEG1_GPIO;}while(0)

#define SEG1_HIGH()							do{JL_PORTA->OUT |= SEG1_GPIO;\
												JL_PORTA->DIR &= ~SEG1_GPIO;}while(0)

#define SEG1_DISABLE()							do{JL_PORTA->DIR |= SEG1_GPIO; \
												  JL_PORTA->HD &= ~SEG1_GPIO;}while(0)

//	SEG0--------GPIO PA15(PIN20)==========================================================
#define SEG0_GPIO							_B9_
#define SEG0_OFF()							do{JL_PORTA->OUT &= ~SEG0_GPIO;\
											JL_PORTA->DIR |=  SEG0_GPIO;\
											JL_PORTA->PU &= ~SEG0_GPIO; \
                                          	JL_PORTA->PD &= ~SEG0_GPIO; \
                                       		JL_PORTA->HD &= ~SEG0_GPIO;}while(0)

#define SEG0_LOW()							do{JL_PORTA->OUT &= ~SEG0_GPIO; \
                                              JL_PORTA->DIR &= ~SEG0_GPIO;\
											   JL_PORTA->HD |= SEG0_GPIO;}while(0)

#define SEG0_HIGH()							do{JL_PORTA->OUT |= SEG0_GPIO;\
												JL_PORTA->DIR &= ~SEG0_GPIO;}while(0)

#define SEG0_DISABLE()							do{JL_PORTA->DIR |= SEG0_GPIO; \
                                              JL_PORTA->HD &= ~SEG0_GPIO;}while(0)

//	I2C_SCL PIN--------GPIO PB12(PIN41)
#define I2C_SCL_GPIO							_B12_
#if defined(_DISABLE_I2C_PULL_)
#define I2C_SCL_INIT()						do{JL_PORTB->PU &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#else
#define I2C_SCL_INIT()						do{JL_PORTB->PU |= I2C_SCL_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#endif

#define I2C_SCL_HIGH()							(JL_PORTB->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()							(JL_PORTB->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()							(JL_PORTB->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()							(JL_PORTB->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     						{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     						{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PB11(PIN42)
#define I2C_SDA_GPIO							_B11_

#if defined(_DISABLE_I2C_PULL_)
#define I2C_SDA_INIT()						do{JL_PORTB->PU &= ~I2C_SDA_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SDA_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SDA_GPIO;}while(0)
#else
#define I2C_SDA_INIT()						do{JL_PORTB->PU |= I2C_SDA_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SDA_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SDA_GPIO;}while(0)
#endif

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

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN								do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

#if defined(_SUPPORT_PANEL_LIGHT_)
/*----------PIN19(C1)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_RED_GPIO							_B1_

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

/*-----------PIN17(C3)-BLUE--------------------------------------------------------------------------------------------------*/
#define SYS_BLUE_GPIO							_B3_

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

/*----------------------------------------------------------------------------------------------------------------*/
//	BEEP--------GPIO PA1(PIN31)
/*----------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_BEEP_)
#if defined(_CHANGE_BEEP_PIN_)         //pin28
#define SYS_BEEP_GPIO							_B4_

#define SYS_BEEP_INIT()							do{JL_PORTA->PU |= SYS_BEEP_GPIO; \
                                               	   JL_PORTA->PD &= ~SYS_BEEP_GPIO; \
                                               	   JL_PORTA->HD |= SYS_BEEP_GPIO;}while(0)
#define SYS_BEEP_HIGH()							(JL_PORTA->OUT |= SYS_BEEP_GPIO)
#define SYS_BEEP_LOW()							(JL_PORTA->OUT &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_OUT()							(JL_PORTA->DIR &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_IN()							(JL_PORTA->DIR |= SYS_BEEP_GPIO)

#else
#define SYS_BEEP_GPIO							_B1_

#define SYS_BEEP_INIT()							do{JL_PORTA->PU |= SYS_BEEP_GPIO; \
                                               	   JL_PORTA->PD &= ~SYS_BEEP_GPIO; \
                                               	   JL_PORTA->HD |= SYS_BEEP_GPIO;}while(0)
#define SYS_BEEP_HIGH()							(JL_PORTA->OUT |= SYS_BEEP_GPIO)
#define SYS_BEEP_LOW()							(JL_PORTA->OUT &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_OUT()							(JL_PORTA->DIR &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_IN()							(JL_PORTA->DIR |= SYS_BEEP_GPIO)
#endif

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

