
#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

//	ACC PIN--------GPIO PR1(PIN9)
#define ACC_GPIO								PORTR1

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
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

//	DOT_SDA--------GPIO PC3(PIN22)
#define LCD_DATA_GPIO						_B3_
#define LCD_DATA_INIT()						do{JL_PORTC->PU &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->PD &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTC->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTC->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTC->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()						(JL_PORTC->DIR |= LCD_DATA_GPIO)

//DOT_SCL--------GPIO PC4(PIN21)
#define LCD_CLK_GPIO						_B4_
#define LCD_CLK_INIT()						do{JL_PORTC->PU &= ~LCD_CLK_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTC->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTC->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTC->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTC->DIR |= LCD_CLK_GPIO)

//NULL--------GPIO PC5(PIN20)
#define LCD_CE_GPIO							_B5_
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

//	I2C_SCL PIN--------GPIO PB6(PIN63)
#define I2C_SCL_GPIO						_B6_

#define I2C_SCL_INIT()						do{JL_PORTB->PU |= I2C_SCL_GPIO;\
                                            JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()						(JL_PORTB->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()						(JL_PORTB->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()						(JL_PORTB->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()						(JL_PORTB->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PB0(PIN64)
#define I2C_SDA_GPIO						_B0_

#define I2C_SDA_INIT()						do{JL_PORTB->PU |= I2C_SDA_GPIO;\
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
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO B10(PIN56)NULL
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

#if defined(_ENABLE_IPHONE_CHARGE_)
/*----------------------------------------------------------------------------------------------------------------*/
//	CHARGE PIN56--------GPIO PB10
/*----------------------------------------------------------------------------------------------------------------*/

#define SYS_CHARGE_GPIO							_B10_

#define SYS_CHARGE_INIT()						do{JL_PORTB->PU |= SYS_CHARGE_GPIO; \
												JL_PORTB->PD &= ~SYS_CHARGE_GPIO; \
												JL_PORTB->HD |= SYS_CHARGE_GPIO;}while(0)
#define SYS_CHARGE_HIGH()						(JL_PORTB->OUT |= SYS_CHARGE_GPIO)
#define SYS_CHARGE_LOW()						(JL_PORTB->OUT &= ~SYS_CHARGE_GPIO)
#define SYS_CHARGE_OUT()						(JL_PORTB->DIR &= ~SYS_CHARGE_GPIO)
#define SYS_CHARGE_IN()							(JL_PORTB->DIR |= SYS_CHARGE_GPIO)
#define SYS_CHARGE_ON()     					{SYS_CHARGE_OUT(); SYS_CHARGE_HIGH();}
#define SYS_CHARGE_OFF()     					{SYS_CHARGE_OUT(); SYS_CHARGE_LOW();}
#endif

#if defined(_ENABLE_VOLUME_CONTROL_)
#define SYS_ZONE1_GPIO							_B8_

#define SYS_ZONE1_INIT()						 do{JL_PORTA->PU &= ~SYS_ZONE1_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_ZONE1_GPIO; \
                                                   JL_PORTA->HD &= ~SYS_ZONE1_GPIO;}while(0)
#define SYS_ZONE1_HIGH()						(JL_PORTA->OUT |= SYS_ZONE1_GPIO)
#define SYS_ZONE1_LOW()							(JL_PORTA->OUT &= ~SYS_ZONE1_GPIO)
#define SYS_ZONE1_OUT()							(JL_PORTA->DIR &= ~SYS_ZONE1_GPIO)
#define SYS_ZONE1_IN()							(JL_PORTA->DIR |= SYS_ZONE1_GPIO)
#define SYS_ZONE1_ON()     						{SYS_ZONE1_OUT(); SYS_ZONE1_HIGH();}
#define SYS_ZONE1_OFF()     					{SYS_ZONE1_OUT(); SYS_ZONE1_LOW();}


#define SYS_ZONE2_GPIO							_B9_

#define SYS_ZONE2_INIT()						 do{JL_PORTA->PU &= ~SYS_ZONE2_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_ZONE2_GPIO; \
                                                   JL_PORTA->HD &= ~SYS_ZONE2_GPIO;}while(0)
#define SYS_ZONE2_HIGH()						(JL_PORTA->OUT |= SYS_ZONE2_GPIO)
#define SYS_ZONE2_LOW()							(JL_PORTA->OUT &= ~SYS_ZONE2_GPIO)
#define SYS_ZONE2_OUT()							(JL_PORTA->DIR &= ~SYS_ZONE2_GPIO)
#define SYS_ZONE2_IN()							(JL_PORTA->DIR |= SYS_ZONE2_GPIO)
#define SYS_ZONE2_ON()     						{SYS_ZONE2_OUT(); SYS_ZONE2_HIGH();}
#define SYS_ZONE2_OFF()     					{SYS_ZONE2_OUT(); SYS_ZONE2_LOW();}

#define SYS_ZONE3_GPIO							_B10_

#define SYS_ZONE3_INIT()						 do{JL_PORTA->PU &= ~SYS_ZONE3_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_ZONE3_GPIO; \
                                                   JL_PORTA->HD &= ~SYS_ZONE3_GPIO;}while(0)
#define SYS_ZONE3_HIGH()						(JL_PORTA->OUT |= SYS_ZONE3_GPIO)
#define SYS_ZONE3_LOW()							(JL_PORTA->OUT &= ~SYS_ZONE3_GPIO)
#define SYS_ZONE3_OUT()							(JL_PORTA->DIR &= ~SYS_ZONE3_GPIO)
#define SYS_ZONE3_IN()							(JL_PORTA->DIR |= SYS_ZONE3_GPIO)
#define SYS_ZONE3_ON()     						{SYS_ZONE3_OUT(); SYS_ZONE3_HIGH();}
#define SYS_ZONE3_OFF()     					{SYS_ZONE3_OUT(); SYS_ZONE3_LOW();}

#define SYS_ZONE4_GPIO							_B11_

#define SYS_ZONE4_INIT()						 do{JL_PORTA->PU &= ~SYS_ZONE4_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_ZONE4_GPIO; \
                                                   JL_PORTA->HD &= ~SYS_ZONE4_GPIO;}while(0)
#define SYS_ZONE4_HIGH()						(JL_PORTA->OUT |= SYS_ZONE4_GPIO)
#define SYS_ZONE4_LOW()							(JL_PORTA->OUT &= ~SYS_ZONE4_GPIO)
#define SYS_ZONE4_OUT()							(JL_PORTA->DIR &= ~SYS_ZONE4_GPIO)
#define SYS_ZONE4_IN()							(JL_PORTA->DIR |= SYS_ZONE4_GPIO)
#define SYS_ZONE4_ON()     						{SYS_ZONE4_OUT(); SYS_ZONE4_HIGH();}
#define SYS_ZONE4_OFF()     					{SYS_ZONE4_OUT(); SYS_ZONE4_LOW();}

#endif

#if defined(_E2PROM_DRIVER_AT24CXX_)

/*-----------PIN62(B1)-----------------------------------------------------------------------------------------------------*/
#define E2PROM_I2C_SDA_GPIO						_B1_

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

#if defined(_YH1701_SPECIAL_FUNTION_)
/*-----------PIN1(B5)---------------------------------------------------------------------------------------------------*/
#define SYS_MCU_GPIO							_B5_

#define SYS_MCU_INIT()							do{JL_PORTB->PU |= SYS_MCU_GPIO; \
                                              	   JL_PORTB->PD &= ~SYS_MCU_GPIO; \
                                            	   JL_PORTB->HD |= SYS_MCU_GPIO;}while(0)
#define SYS_MCU_HIGH()							(JL_PORTB->OUT |= SYS_MCU_GPIO)
#define SYS_MCU_LOW()							(JL_PORTB->OUT &= ~SYS_MCU_GPIO)
#define SYS_MCU_OUT()							(JL_PORTB->DIR &= ~SYS_MCU_GPIO)
#define SYS_MCU_IN()							(JL_PORTB->DIR |= SYS_MCU_GPIO)
#define SYS_MCU_ON()     						{SYS_MCU_OUT(); SYS_MCU_HIGH();}
#define SYS_MCU_OFF()     						{SYS_MCU_OUT(); SYS_MCU_LOW();}
#endif

#if defined(_SUPPORT_SUB_)
/*----------------------------------------------------------------------------------------------------------------*/
//	SUB PIN(52)--------GPIO A9
/*----------------------------------------------------------------------------------------------------------------*/
#if defined(_DISBLE_MODE_OPTICAL_)
#define SYS_SUB_GPIO							_B30_
#else
#define SYS_SUB_GPIO							_B6_
#endif

#define SYS_SUB_INIT()							 do{JL_PORTA->PU &= ~SYS_SUB_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_SUB_GPIO; \
                                                   JL_PORTA->HD &= ~SYS_SUB_GPIO;}while(0)
#define SYS_SUB_HIGH()							(JL_PORTA->OUT |= SYS_SUB_GPIO)
#define SYS_SUB_LOW()							(JL_PORTA->OUT &= ~SYS_SUB_GPIO)
#define SYS_SUB_OUT()							(JL_PORTA->DIR &= ~SYS_SUB_GPIO)
#define SYS_SUB_IN()							(JL_PORTA->DIR |= SYS_SUB_GPIO)
#define SYS_SUB_ON()     						{SYS_SUB_OUT(); SYS_SUB_HIGH();}
#define SYS_SUB_OFF()     						{SYS_SUB_OUT(); SYS_SUB_LOW();}
#endif

#if defined(_ENABLE_OPTICAL_CONTROL_)
/*----------------------------------------------------------------------------------------------------------------*/
//	OPTICAL PIN(38)--------GPIO A6
/*----------------------------------------------------------------------------------------------------------------*/
#if defined(_DISBLE_MODE_OPTICAL_)
#define SYS_OPTICAL_GPIO						_B30_
#else
#define SYS_OPTICAL_GPIO						_B2_
#endif

#define SYS_OPTICAL_INIT()						do{JL_PORTB->PU &= ~SYS_OPTICAL_GPIO; \
                                                JL_PORTB->PD &= ~SYS_OPTICAL_GPIO; \
                                                JL_PORTB->HD &= ~SYS_OPTICAL_GPIO;}while(0)
#define SYS_OPTICAL_HIGH()						(JL_PORTB->OUT |= SYS_OPTICAL_GPIO)
#define SYS_OPTICAL_LOW()						(JL_PORTB->OUT &= ~SYS_OPTICAL_GPIO)
#define SYS_OPTICAL_OUT()						(JL_PORTB->DIR &= ~SYS_OPTICAL_GPIO)
#define SYS_OPTICAL_IN()						(JL_PORTB->DIR |= SYS_OPTICAL_GPIO)
#define SYS_OPTICAL_ON()     					{SYS_OPTICAL_OUT(); SYS_OPTICAL_HIGH();}
#define SYS_OPTICAL_OFF()     					{SYS_OPTICAL_OUT(); SYS_OPTICAL_LOW();}
#endif

#endif

