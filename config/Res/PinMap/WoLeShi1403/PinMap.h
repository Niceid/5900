#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

//	ACC PIN--------GPIO PR1(PIN9)
#if defined(_SUPPORT_ACC_)
#define ACC_GPIO								PORTR1

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)
#endif

//	ENCODE--------GPIO PR2(PIN6)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif

//	SYS_POW PIN--------GPIO PR0(PIN13)
#define SYS_POW_GPIO							PORTR0
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

//	AUDIO_MUTE PIN--------GPIO PR3(PIN11)
#define AUDIO_MUTE_GPIO							PORTR3
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						(PORTR_OUT(AUDIO_MUTE_GPIO, 1))
#define AUDIO_MUTE_LOW()						(PORTR_OUT(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}

//	LED COM3--------GPIO PA4(PIN40)
#define LED_COM3_GPIO						_B4_
#define LED_COM3_OFF()						do{JL_PORTA->OUT &= ~LED_COM3_GPIO;\
												JL_PORTA->DIR |=  LED_COM3_GPIO;\
												JL_PORTA->PU &= ~LED_COM3_GPIO; \
                                          		JL_PORTA->PD &= ~LED_COM3_GPIO; \
                                       			JL_PORTA->HD &= ~LED_COM3_GPIO;}while(0)
#define LED_COM3_ON()						do{JL_PORTA->OUT |= LED_COM3_GPIO;\
												JL_PORTA->DIR &= ~LED_COM3_GPIO;\
												JL_PORTA->HD |= LED_COM3_GPIO;}while(0)

//	LED COM2--------GPIO PA7(PIN37)
#define LED_COM2_GPIO						_B7_
#define LED_COM2_OFF()						do{JL_PORTA->OUT &= ~LED_COM2_GPIO;\
												JL_PORTA->DIR |=  LED_COM2_GPIO;\
												JL_PORTA->PU &= ~LED_COM2_GPIO; \
                                          		JL_PORTA->PD &= ~LED_COM2_GPIO; \
                                       			JL_PORTA->HD &= ~LED_COM2_GPIO;}while(0)
#define LED_COM2_ON()						do{JL_PORTA->OUT |= LED_COM2_GPIO;\
												JL_PORTA->DIR &= ~LED_COM2_GPIO;\
												JL_PORTA->HD |= LED_COM2_GPIO;}while(0)

//	LED COM1--------GPIO PA8(PIN36)
#define LED_COM1_GPIO						_B8_
#define LED_COM1_OFF()						do{JL_PORTA->OUT &= ~LED_COM1_GPIO;\
												JL_PORTA->DIR |=  LED_COM1_GPIO;\
												JL_PORTA->PU &= ~LED_COM1_GPIO; \
                                          		JL_PORTA->PD &= ~LED_COM1_GPIO; \
                                       			JL_PORTA->HD &= ~LED_COM1_GPIO;}while(0)
#define LED_COM1_ON()						do{JL_PORTA->OUT |= LED_COM1_GPIO;\
												JL_PORTA->DIR &= ~LED_COM1_GPIO;\
												JL_PORTA->HD |= LED_COM1_GPIO;}while(0)

//	LED COM0--------GPIO PA10(PIN34)
#define LED_COM0_GPIO						_B10_
#define LED_COM0_OFF()						do{JL_PORTA->OUT &= ~LED_COM0_GPIO;\
												JL_PORTA->DIR |=  LED_COM0_GPIO;\
												JL_PORTA->PU &= ~LED_COM0_GPIO; \
                                          		JL_PORTA->PD &= ~LED_COM0_GPIO; \
                                       			JL_PORTA->HD &= ~LED_COM0_GPIO;}while(0)
#define LED_COM0_ON()						do{JL_PORTA->OUT |= LED_COM0_GPIO;\
												JL_PORTA->DIR &= ~LED_COM0_GPIO;\
												JL_PORTA->HD |= LED_COM0_GPIO;}while(0)

//	SEG8--------GPIO PA13(PIN31)
#define SEG7_GPIO							_B13_
#define SEG7_OFF()							do{JL_PORTA->OUT &= ~SEG7_GPIO;\
											JL_PORTA->DIR |=  SEG7_GPIO;\
											JL_PORTA->PU &= ~SEG7_GPIO; \
                                          	JL_PORTA->PD &= ~SEG7_GPIO; \
                                       		JL_PORTA->HD &= ~SEG7_GPIO;}while(0)

#define SEG7_ON()							do{JL_PORTA->OUT &= ~SEG7_GPIO; \
                                              JL_PORTA->DIR &= ~SEG7_GPIO;}while(0)

//	SEG7--------GPIO PA11(PIN33)
#define SEG6_GPIO							_B11_
#define SEG6_OFF()							do{JL_PORTA->OUT &= ~SEG6_GPIO;\
											JL_PORTA->DIR |=  SEG6_GPIO;\
											JL_PORTA->PU &= ~SEG6_GPIO; \
                                          	JL_PORTA->PD &= ~SEG6_GPIO; \
                                       		JL_PORTA->HD &= ~SEG6_GPIO;}while(0)

#define SEG6_ON()							do{JL_PORTA->OUT &= ~SEG6_GPIO; \
                                              JL_PORTA->DIR &= ~SEG6_GPIO;}while(0)

//	SEG6--------GPIO PA6(PIN38)
#define SEG5_GPIO							_B6_
#define SEG5_OFF()							do{JL_PORTA->OUT &= ~SEG5_GPIO;\
											JL_PORTA->DIR |=  SEG5_GPIO;\
											JL_PORTA->PU &= ~SEG5_GPIO; \
                                          	JL_PORTA->PD &= ~SEG5_GPIO; \
                                       		JL_PORTA->HD &= ~SEG5_GPIO;}while(0)

#define SEG5_ON()							do{JL_PORTA->OUT &= ~SEG5_GPIO; \
                                              JL_PORTA->DIR &= ~SEG5_GPIO;}while(0)

//	SEG5--------GPIO PA15(PIN26)
#define SEG4_GPIO							_B15_
#define SEG4_OFF()							do{JL_PORTA->OUT &= ~SEG4_GPIO;\
											JL_PORTA->DIR |=  SEG4_GPIO;\
											JL_PORTA->PU &= ~SEG4_GPIO; \
                                          	JL_PORTA->PD &= ~SEG4_GPIO; \
                                       		JL_PORTA->HD &= ~SEG4_GPIO;}while(0)

#define SEG4_ON()							do{JL_PORTA->OUT &= ~SEG4_GPIO; \
                                              JL_PORTA->DIR &= ~SEG4_GPIO;}while(0)

//	SEG4--------GPIO PA14(PIN27)
#define SEG3_GPIO							_B14_
#define SEG3_OFF()							do{JL_PORTA->OUT &= ~SEG3_GPIO;\
											JL_PORTA->DIR |=  SEG3_GPIO;\
											JL_PORTA->PU &= ~SEG3_GPIO; \
                                          	JL_PORTA->PD &= ~SEG3_GPIO; \
                                       		JL_PORTA->HD &= ~SEG3_GPIO;}while(0)

#define SEG3_ON()							do{JL_PORTA->OUT &= ~SEG3_GPIO; \
                                              JL_PORTA->DIR &= ~SEG3_GPIO;}while(0)

//	SEG3--------GPIO PA12(PIN32)
#define SEG2_GPIO							_B12_
#define SEG2_OFF()							do{JL_PORTA->OUT &= ~SEG2_GPIO;\
											JL_PORTA->DIR |=  SEG2_GPIO;\
											JL_PORTA->PU &= ~SEG2_GPIO; \
                                          	JL_PORTA->PD &= ~SEG2_GPIO; \
                                       		JL_PORTA->HD &= ~SEG2_GPIO;}while(0)

#define SEG2_ON()							do{JL_PORTA->OUT &= ~SEG2_GPIO; \
                                              JL_PORTA->DIR &= ~SEG2_GPIO;}while(0)

//	SEG2--------GPIO PA9(PIN35)
#define SEG1_GPIO							_B9_
#define SEG1_OFF()							do{JL_PORTA->OUT &= ~SEG1_GPIO;\
											JL_PORTA->DIR |=  SEG1_GPIO;\
											JL_PORTA->PU &= ~SEG1_GPIO; \
                                          	JL_PORTA->PD &= ~SEG1_GPIO; \
                                       		JL_PORTA->HD &= ~SEG1_GPIO;}while(0)

#define SEG1_ON()							do{JL_PORTA->OUT &= ~SEG1_GPIO; \
                                              JL_PORTA->DIR &= ~SEG1_GPIO;}while(0)

//	SEG0--------GPIO PA5(PIN39)
#define SEG0_GPIO							_B5_
#define SEG0_OFF()							do{JL_PORTA->OUT &= ~SEG0_GPIO;\
											JL_PORTA->DIR |=  SEG0_GPIO;\
											JL_PORTA->PU &= ~SEG0_GPIO; \
                                          	JL_PORTA->PD &= ~SEG0_GPIO; \
                                       		JL_PORTA->HD &= ~SEG0_GPIO;}while(0)

#define SEG0_ON()							do{JL_PORTA->OUT &= ~SEG0_GPIO; \
                                              JL_PORTA->DIR &= ~SEG0_GPIO;}while(0)


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

//	KEY--------GPIO PB10(PIN43)
#define ADC_KEY1_PIN							AD_KEY_PB8

//	KEY2--------GPIO PB9(PIN44)
#define ADC_KEY2_PIN							AD_KEY_PB9

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN								do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

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

#endif

