
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
#if defined(_VR_SCAN_IO_)
/*----------------------------------------------------------------------------------------------------------------*/
//	VOL+
/*----------------------------------------------------------------------------------------------------------------*/
#define VOL_POSITIVE_GPIO						_B5_


#define VOL_POSITIVE_INIT()						do{JL_PORTB->PU |= VOL_POSITIVE_GPIO; \
                                               	 JL_PORTB->PD &= ~VOL_POSITIVE_GPIO; \
                                               	 JL_PORTB->HD |= VOL_POSITIVE_GPIO;}while(0)
#define VOL_POSITIVE_HIGH()						(JL_PORTB->OUT |= VOL_POSITIVE_GPIO)
#define VOL_POSITIVE_LOW()						(JL_PORTB->OUT &= ~VOL_POSITIVE_GPIO)
#define VOL_POSITIVE_OUT()						(JL_PORTB->DIR &= ~VOL_POSITIVE_GPIO)
#define VOL_POSITIVE_IN()						(JL_PORTB->DIR |= VOL_POSITIVE_GPIO)

#define VOL_POSITIVE_GET()						(JL_PORTB->IN & VOL_POSITIVE_GPIO)
#define VOL_POSITIVE_CHECK()					(VOL_POSITIVE_GET() ? 0 : 1)

/*----------------------------------------------------------------------------------------------------------------*/
//	VOL-
/*----------------------------------------------------------------------------------------------------------------*/
#define VOL_NEGATIVE_GPIO						_B6_


#define VOL_NEGATIVE_INIT()						do{JL_PORTB->PU |= VOL_NEGATIVE_GPIO; \
                                               	 JL_PORTB->PD &= ~VOL_NEGATIVE_GPIO; \
                                               	 JL_PORTB->HD |= VOL_NEGATIVE_GPIO;}while(0)
#define VOL_NEGATIVE_HIGH()						(JL_PORTB->OUT |= VOL_NEGATIVE_GPIO)
#define VOL_NEGATIVE_LOW()						(JL_PORTB->OUT &= ~VOL_NEGATIVE_GPIO)
#define VOL_NEGATIVE_OUT()						(JL_PORTB->DIR &= ~VOL_NEGATIVE_GPIO)
#define VOL_NEGATIVE_IN()						(JL_PORTB->DIR |= VOL_NEGATIVE_GPIO)

#define VOL_NEGATIVE_GET()						(JL_PORTB->IN & VOL_NEGATIVE_GPIO)
#define VOL_NEGATIVE_CHECK()					(VOL_NEGATIVE_GET() ? 0 : 1)

#else
#define ADC_ENCODE_PIN							AD_KEY_PR2
#endif
#endif

#if defined(_SUPPORT_VR2_)
#define ADC_ENCODE2_PIN							AD_KEY_PB9
#endif

//	SYS_POW PIN--------GPIO PR0(PIN13)
#define SYS_POW_GPIO							PORTR0
#define SYS_POW_INIT()							do{PORTR_PU(SYS_POW_GPIO, 1);\
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


#if defined(SYS_IO_OUT_FM_CLK)
#if defined(SYS_IO_OUT_FM_32768_CLK_)
#define SYS_IO_OUT_FM_CLK_DISENABLE()			JL_IOMAP->CON1 &= ~BIT(13);\
												JL_IOMAP->CON1 &= ~BIT(12);\
												JL_IOMAP->CON1 &= ~BIT(11);\
												JL_PORTC->DIR |= BIT(0);	   \
												JL_PORTC->PU &= ~BIT(0);   \
												JL_PORTC->PD &= ~BIT(0);



#define SYS_IO_OUT_FM_CLK_ENABLE()				JL_IOMAP->CON1 |= BIT(13);\
												JL_IOMAP->CON1 &= ~BIT(12);\
												JL_IOMAP->CON1 &= ~BIT(11);\
												JL_PORTC->DIR &= ~BIT(0);\
												JL_PORTC->PU |= BIT(0);\
												JL_PORTC->PD |= BIT(0);

#endif
#endif

//	AUDIO_MUTE PIN--------GPIO PR3(PIN11)
#define AUDIO_MUTE_GPIO							PORTR3
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						(PORTR_OUT(AUDIO_MUTE_GPIO, 1))
#define AUDIO_MUTE_LOW()						(PORTR_OUT(AUDIO_MUTE_GPIO, 0))
#if defined(_ENABLE_LOW_MUTE_)
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT(); AUDIO_MUTE_LOW();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT(); AUDIO_MUTE_HIGH();}
#else
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT(); AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT(); AUDIO_MUTE_LOW();}
#endif

//	I2C_SCL PIN--------GPIO PB11(PIN55)
#if defined(_DECODER_AUDIO_CONTROL_)
#define I2C_SCL_GPIO						_B30_
#else
#define I2C_SCL_GPIO						_B11_
#endif

#if defined(_DISABLE_I2C_PULL_)
#define I2C_SCL_INIT()						do{JL_PORTB->PU &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#else
#define I2C_SCL_INIT()						do{JL_PORTB->PU |= I2C_SCL_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SCL_GPIO;}while(0)
#endif

#define I2C_SCL_HIGH()						(JL_PORTB->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()						(JL_PORTB->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()						(JL_PORTB->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()						(JL_PORTB->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PB12(PIN54)
#if defined(_DECODER_AUDIO_CONTROL_)
#define I2C_SDA_GPIO						_B30_
#else
#define I2C_SDA_GPIO						_B12_
#endif

#if defined(_DISABLE_I2C_PULL_)
#define I2C_SDA_INIT()						do{JL_PORTB->PU &= ~I2C_SDA_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SDA_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SDA_GPIO;}while(0)
#else
#define I2C_SDA_INIT()						do{JL_PORTB->PU |= I2C_SDA_GPIO; \
                                            JL_PORTB->PD &= ~I2C_SDA_GPIO; \
                                            JL_PORTB->HD &= ~I2C_SDA_GPIO;}while(0)
#endif
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
#if !defined(_SUPPORT_VR2_)
#define ADC_KEY2_PIN						AD_KEY_PB9
#endif

//	IR PIN--------GPIO PB3(PIN58)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

#if defined(_ENABLE_RESET_PIN_)
/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO PA0(PIN44)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B0_

#define RADIO_RESET_INIT()						do{JL_PORTA->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTA->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTA->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTA->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTA->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

#elif defined(_DA2303_ENABLE_RESET_PIN_)
/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO B6(PIN63)
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B6_

#define RADIO_RESET_INIT()						do{JL_PORTB->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTB->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTB->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTB->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTB->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

#else
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

#if defined(_SUPPORT_PANEL_LIGHT_)
/*----------PIN1(B5)-REG-----------------------------------------------------------------------------------------------------*/
#if defined(_CHANGE_PANEL_PIN_)
#define SYS_RED_GPIO							_B6_
#else
#define SYS_RED_GPIO							_B5_
#endif
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
#if defined(_CHANGE_PANEL_PIN_)
#define SYS_BLUE_GPIO							_B5_
#else
#define SYS_BLUE_GPIO							_B6_
#endif
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

#if defined(_ENABLE_INH_CONTROL_)
#define LCD_INH_GPIO						_B12_

#define LCD_INH_INIT()						do{JL_PORTA->PU &= ~LCD_INH_GPIO; \
                                               JL_PORTA->PD &= ~LCD_INH_GPIO; \
                                               JL_PORTA->HD &= ~LCD_INH_GPIO;}while(0)
#define LCD_INH_HIGH()						(JL_PORTA->OUT |= LCD_INH_GPIO)
#define LCD_INH_LOW()						(JL_PORTA->OUT &= ~LCD_INH_GPIO)
#define LCD_INH_OUT()						(JL_PORTA->DIR &= ~LCD_INH_GPIO)
#define LCD_INH_IN()						(JL_PORTA->DIR |= LCD_INH_GPIO)
#define LCD_INH_ON()     					{LCD_INH_OUT();LCD_INH_LOW();}
#define LCD_INH_OFF()     					{LCD_INH_OUT();LCD_INH_HIGH();}
#endif

#if defined(_SUPPORT_REAL_VBAR_)
#define ADC_VBAR_PIN							AD_KEY_PB7
#endif

#if defined(_SUPPORT_SUB_)
/*----------------------------------------------------------------------------------------------------------------*/
//	SUB PIN(43)--------GPIO A1
/*----------------------------------------------------------------------------------------------------------------*/

#define SYS_SUB_GPIO							_B1_

#define SYS_SUB_INIT()							 do{JL_PORTA->PU &= ~SYS_SUB_GPIO; \
                                                   JL_PORTA->PD &= ~SYS_SUB_GPIO; \
                                                   JL_PORTA->HD &= ~SYS_SUB_GPIO;}while(0)
#define SYS_SUB_HIGH()							(JL_PORTA->OUT |= SYS_SUB_GPIO)
#define SYS_SUB_LOW()							(JL_PORTA->OUT &= ~SYS_SUB_GPIO)
#define SYS_SUB_OUT()							(JL_PORTA->DIR &= ~SYS_SUB_GPIO)
#define SYS_SUB_IN()							(JL_PORTA->DIR |= SYS_SUB_GPIO)
#if defined(_CHANGE_SUB_OFF_)
#define SYS_SUB_ON()     						{SYS_SUB_OUT(); SYS_SUB_HIGH();}
#define SYS_SUB_OFF()     						{SYS_SUB_OUT(); SYS_SUB_LOW();}
#else
#define SYS_SUB_ON()     						{SYS_SUB_OUT(); SYS_SUB_LOW();}
#define SYS_SUB_OFF()     						{SYS_SUB_OUT(); SYS_SUB_HIGH();}
#endif
#endif

#endif


