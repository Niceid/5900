#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

#define LCD_CE_GPIO
#define LCD_CE_INIT()
#define LCD_CE_HIGH()
#define LCD_CE_LOW()
#define LCD_CE_OUT()
#define LCD_CE_IN()
#define LCD_DATA_GPIO
#define LCD_DATA_INIT()
#define LCD_DATA_HIGH()
#define LCD_DATA_LOW()
#define LCD_DATA_OUT()
#define LCD_DATA_IN()
#define LCD_CLK_GPIO
#define LCD_CLK_INIT()
#define LCD_CLK_HIGH()
#define LCD_CLK_LOW()
#define LCD_CLK_OUT()
#define LCD_CLK_IN()

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


//	DAB_RX PIN1--------GPIO PC5(PIN15)
#define DAB_RX_GPIO							_B5_
#define DAB_RX_INIT()						do{JL_PORTC->PU &= ~DAB_RX_GPIO; \
                                            JL_PORTC->PD &= ~DAB_RX_GPIO; \
                                            JL_PORTC->HD &= ~DAB_RX_GPIO;}while(0)
#define DAB_RX_OUT()						(JL_PORTC->DIR &= ~DAB_RX_GPIO)
#define DAB_RX_IN()							(JL_PORTC->DIR |= DAB_RX_GPIO)
#define DAB_RX_HIGH()						(JL_PORTC->OUT |= DAB_RX_GPIO)
#define DAB_RX_LOW()						(JL_PORTC->OUT &= ~DAB_RX_GPIO)

//	DAB_TX PIN2--------GPIO PC4(PIN16)
#define DAB_TX_GPIO							_B4_
#define DAB_TX_INIT()						do{JL_PORTC->PU &= ~DAB_TX_GPIO; \
                                            JL_PORTC->PD &= ~DAB_TX_GPIO; \
                                            JL_PORTC->HD &= ~DAB_TX_GPIO;}while(0)
#define DAB_TX_OUT()						(JL_PORTC->DIR &= ~DAB_TX_GPIO)
#define DAB_TX_IN()							(JL_PORTC->DIR |= DAB_TX_GPIO)
#define DAB_TX_HIGH()						(JL_PORTC->OUT |= DAB_TX_GPIO)
#define DAB_TX_LOW()						(JL_PORTC->OUT &= ~DAB_TX_GPIO)

//	DAB_RESET PIN3--------GPIO PC3(PIN17)
#define DAB_RESET_GPIO						_B3_
#define DAB_RESET_INIT()					do{JL_PORTC->PU &= ~DAB_RESET_GPIO; \
                                            JL_PORTC->PD &= ~DAB_RESET_GPIO; \
                                            JL_PORTC->HD &= ~DAB_RESET_GPIO;}while(0)
#define DAB_RESET_OUT()						(JL_PORTC->DIR &= ~DAB_RESET_GPIO)
#define DAB_RESET_HIGH()					(JL_PORTC->OUT |= DAB_RESET_GPIO)
#define DAB_RESET_LOW()						(JL_PORTC->OUT &= ~DAB_RESET_GPIO)

//NULL--------GPIO PC2(PIN18)
#define DOT_A0_GPIO							_B2_
#define DOT_A0_INIT()						do{JL_PORTC->PU &= ~DOT_A0_GPIO; \
                                            JL_PORTC->PD &= ~DOT_A0_GPIO; \
                                            JL_PORTC->HD &= ~DOT_A0_GPIO;}while(0)
#define DOT_A0_IN()							(JL_PORTC->DIR |= DOT_A0_GPIO)

//	LCM_RESET PIN5--------GPIO PC1(PIN19)
#define LCM_RESET_GPIO						_B1_
#define LCM_RESET_INIT()					do{JL_PORTC->PU &= ~LCM_RESET_GPIO; \
                                            JL_PORTC->PD &= ~LCM_RESET_GPIO; \
                                            JL_PORTC->HD &= ~LCM_RESET_GPIO;}while(0)
#define LCM_RESET_OUT()						(JL_PORTC->DIR &= ~LCM_RESET_GPIO)
#define LCM_RESET_IN()						(JL_PORTC->DIR |= LCM_RESET_GPIO)
#define LCM_RESET_HIGH()					(JL_PORTC->OUT |= LCM_RESET_GPIO)
#define LCM_RESET_LOW()						(JL_PORTC->OUT &= ~LCM_RESET_GPIO)

//	DOT_SDA--------GPIO PA15(PIN20)
#define LCM_SDA_GPIO						_B15_
#define LCM_SDA_INIT()						do{JL_PORTA->PU &=~LCM_SDA_GPIO; \
                                            JL_PORTA->PD &= ~LCM_SDA_GPIO; \
                                            JL_PORTA->HD &= ~LCM_SDA_GPIO;}while(0)
#define LCM_SDA_OUT()						(JL_PORTA->DIR &= ~LCM_SDA_GPIO)
#define LCM_SDA_IN()						(JL_PORTA->DIR |= LCM_SDA_GPIO)
#define LCM_SDA_HIGH()						(JL_PORTA->OUT |= LCM_SDA_GPIO)
#define LCM_SDA_LOW()						(JL_PORTA->OUT &= ~LCM_SDA_GPIO)
#define LCM_SDA_GET()						(JL_PORTA->IN & LCM_SDA_GPIO)

//DOT_SCL--------GPIO PA14(PIN21)
#define LCM_SCL_GPIO						_B14_
#define LCM_SCL_INIT()						do{JL_PORTA->PU &=~LCM_SCL_GPIO; \
                                            JL_PORTA->PD &= ~LCM_SCL_GPIO; \
                                            JL_PORTA->HD &= ~LCM_SCL_GPIO;}while(0)
#define LCM_SCL_OUT()						(JL_PORTA->DIR &= ~LCM_SCL_GPIO)
#define LCM_SCL_IN()						(JL_PORTA->DIR |= LCM_SCL_GPIO)
#define LCM_SCL_HIGH()						(JL_PORTA->OUT |= LCM_SCL_GPIO)
#define LCM_SCL_LOW()						(JL_PORTA->OUT &= ~LCM_SCL_GPIO)

//NULL--------GPIO PC0(PIN22)
#define DOT_CS_GPIO							_B0_
#define DOT_CS_INIT()						do{JL_PORTC->PU &= ~DOT_CS_GPIO; \
                                            JL_PORTC->PD &= ~DOT_CS_GPIO; \
                                            JL_PORTC->HD &= ~DOT_CS_GPIO;}while(0)
#define DOT_CS_OUT()						(JL_PORTC->DIR &= ~DOT_CS_GPIO)
#define DOT_CS_IN()							(JL_PORTC->DIR |= DOT_CS_GPIO)
#define DOT_CS_HIGH()						(JL_PORTC->OUT |= DOT_CS_GPIO)
#define DOT_CS_LOW()						(JL_PORTC->OUT &= ~DOT_CS_GPIO)


//DOT_AO--------GPIO PC0(PIN22)
#define DOT_AO_GPIO							_B10_
#define DOT_AO_INIT()						do{JL_PORTA->PU &= ~DOT_AO_GPIO; \
                                            JL_PORTA->PD &= ~DOT_AO_GPIO; \
                                            JL_PORTA->HD &= ~DOT_AO_GPIO;}while(0)
#define DOT_AO_OUT()						(JL_PORTA->DIR &= ~DOT_AO_GPIO)
#define DOT_AO_IN()							(JL_PORTA->DIR |= DOT_AO_GPIO)
#define DOT_AO_HIGH()						(JL_PORTA->OUT |= DOT_AO_GPIO)
#define DOT_AO_LOW()						(JL_PORTA->OUT &= ~DOT_AO_GPIO)


//LCM POWER--------GPIO PA11(PIN25)
#define LCM_PWR_GPIO						_B11_
#define LCM_PWR_INIT()						do{JL_PORTA->PU |= LCM_PWR_GPIO; \
                                            JL_PORTA->PD &= ~LCM_PWR_GPIO; \
                                            JL_PORTA->HD &= ~LCM_PWR_GPIO;}while(0)
#define LCM_POW_OUT()						(JL_PORTA->DIR &= ~LCM_PWR_GPIO)
#define LCM_PWR_ON()						(JL_PORTA->OUT &= ~LCM_PWR_GPIO)
#define LCM_PWR_OFF()						(JL_PORTA->OUT |= LCM_PWR_GPIO)

//NULL--------GPIO PA10(PIN26)
#define PIN26_NULL_GPIO						_B10_
#define PIN26_NULL_INIT()					do{JL_PORTA->PU &= ~PIN26_NULL_GPIO; \
                                            JL_PORTA->PD &= ~PIN26_NULL_GPIO; \
                                            JL_PORTA->HD &= ~PIN26_NULL_GPIO;}while(0)

//DAB_POW--------GPIO PA9(PIN27)
#define DAB_POW_GPIO						_B9_
#define DAB_POW_INIT()						do{JL_PORTA->PU &= ~DAB_POW_GPIO; \
                                            JL_PORTA->PD &= ~DAB_POW_GPIO; \
                                            JL_PORTA->HD &= ~DAB_POW_GPIO;}while(0)
#define DAB_POW_OUT()						(JL_PORTA->DIR &= ~DAB_POW_GPIO)
#define DAB_POW_ON()						(JL_PORTA->OUT |= DAB_POW_GPIO)
#define DAB_POW_OFF()						(JL_PORTA->OUT &= ~DAB_POW_GPIO)

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
//	RADIO_RESET PIN--------GPIO B13(PIN32)¸´ÓÃMIC
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B13_

#define RADIO_RESET_INIT()						/*do{JL_PORTB->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->HD &= ~RADIO_RESET_GPIO;}while(0)*/
#define RADIO_RESET_HIGH()						//(JL_PORTB->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						//(JL_PORTB->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						//(JL_PORTB->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						//(JL_PORTB->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					//{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					//{RADIO_RESET_OUT();RADIO_RESET_HIGH();}
#endif
