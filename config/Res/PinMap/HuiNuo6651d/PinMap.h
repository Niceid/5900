#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

//	ENCODE--------GPIO PR2(PIN6)
#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN							AD_KEY_PB8
#endif
//	KEY--------GPIO PR2(PIN6)
#define ADC_KEY1_PIN							AD_KEY_PR2

//	KEY2--------GPIO PB10(PIN43)
#define ADC_KEY2_PIN							AD_KEY_PB10

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN								//do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)


//	ACC PIN--------GPIO PR3(PIN7)
#define ACC_GPIO								PORTR3

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)





//	AUDIO_MUTE PIN--------GPIO PR1(PIN5)
#define AUDIO_MUTE_GPIO							PORTR1
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);\
												   PORTR_HD(AUDIO_MUTE_GPIO, 1);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						{PORTR_OUT(AUDIO_MUTE_GPIO, 1);PORTR_HD(AUDIO_MUTE_GPIO, 0);}
#define AUDIO_MUTE_LOW()						{PORTR_OUT(AUDIO_MUTE_GPIO, 0);PORTR_HD(AUDIO_MUTE_GPIO, 0);}
#if defined(AUDIO_MUTE_SPECIAL_SET)
#define AUDIO_MUTE_ON()							//{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						//{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}
#define HAUDIO_MUTE_ON()						{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define HAUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}
#else
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}
#endif

//	SYS_POW PIN--------GPIO PB9(PIN44)
#define SYS_POW_GPIO							_B9_
#define SYS_POW_INIT()							do{JL_PORTB->PU &= ~SYS_POW_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_POW_GPIO; \
                                                   JL_PORTB->HD &= ~SYS_POW_GPIO;}while(0)
#define SYS_POW_OUT()							(JL_PORTB->DIR &= ~SYS_POW_GPIO)
#define SYS_POW_HIGH()							(JL_PORTB->OUT |= SYS_POW_GPIO)
#define SYS_POW_LOW()							(JL_PORTB->OUT &= ~SYS_POW_GPIO)
#define SYS_POW_ON()     						{SYS_POW_OUT(); SYS_POW_HIGH();}
#define SYS_POW_OFF()     						{SYS_POW_OUT(); SYS_POW_LOW();}
#define SYS_POW_IN()							(JL_PORTB->DIR |= SYS_POW_GPIO)



#if defined(SYS_IO_OUT_FM_CLK)
#if defined(SYS_IO_OUT_FM_24M_CLK_)
#define SYS_IO_OUT_FM_CLK_DISENABLE()				JL_IOMAP->CON1 &= ~BIT(13);\
												JL_IOMAP->CON1 &= ~BIT(12);\
												JL_IOMAP->CON1 &= ~BIT(11);\
												JL_PORTC->DIR |= BIT(0);	   \
												JL_PORTC->PU &= ~BIT(0);   \
												JL_PORTC->PD &= ~BIT(0);



#define SYS_IO_OUT_FM_CLK_ENABLE()					JL_IOMAP->CON1 |= BIT(13);\
												JL_IOMAP->CON1 &= ~BIT(12);\
												JL_IOMAP->CON1 |= BIT(11);\
												JL_PORTC->DIR &= ~BIT(0);\
												JL_PORTC->PU |= BIT(0);\
												JL_PORTC->PD |= BIT(0);

#endif
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



#if defined(DRIVE_lCD_CLEAR)
//lcd直驱com口重置PC12345   0011 1110
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


#if 0//defined(_VFD_DRIVER_IC_PT1621_)//pt1621  iic驱动
//	DOT_SDA--------GPIO PC1(PIN19)
#define LCD_DATA_GPIO						_B1_
#define LCD_DATA_INIT()						do{JL_PORTC->PU &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->PD &= ~LCD_DATA_GPIO; \
                                            JL_PORTC->HD &= ~LCD_DATA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTC->OUT |= LCD_DATA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTC->OUT &= ~LCD_DATA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTC->DIR &= ~LCD_DATA_GPIO)
#define LCD_DATA_IN()						(JL_PORTC->DIR |= LCD_DATA_GPIO)

//DOT_SCL--------GPIO PA15(PIN20)
#define LCD_CLK_GPIO						_B15_
#define LCD_CLK_INIT()						do{JL_PORTA->PU &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTA->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTA->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTA->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTA->DIR |= LCD_CLK_GPIO)

//NULL--------GPIO PC4(PIN15)
#define LCD_CE_GPIO							_B5_
#define LCD_CE_INIT()						do{JL_PORTC->PU &= ~LCD_CE_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()						(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()						(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()						(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()							(JL_PORTC->DIR |= LCD_CE_GPIO)
#endif


#if defined(_SUPPORT_RADIO_)// 音效ic pt2313   和收音ic  共用iic
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


/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO B13(PIN32)复用MIC
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B30_

#define RADIO_RESET_INIT()						do{JL_PORTB->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTB->HD &= ~RADIO_RESET_GPIO;}while(0)/**/
#define RADIO_RESET_HIGH()						(JL_PORTB->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTB->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTB->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTB->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}
#endif

#if 0//defined(_ENABLE_PT2313_I2C_)// 音效ic pt2313   独立io
//	I2C_SCL PIN--------GPIO PORTR2(PIN6)
#define I2C_SCL_GPIO1						PORTR2

#define I2C_SCL_INIT1()						do{PORTR_PU(I2C_SCL_GPIO1, 1);\
                                               PORTR_PD(I2C_SCL_GPIO1, 0);\
                                               PORTR_HD(I2C_SCL_GPIO1, 0);}while(0)

#define I2C_SCL_HIGH1()						(PORTR_OUT(I2C_SCL_GPIO1, 1))
#define I2C_SCL_LOW1()						(PORTR_OUT(I2C_SCL_GPIO1, 0))
#define I2C_SCL_OUT1()						(PORTR_DIR(I2C_SCL_GPIO1, 0))
#define I2C_SCL_IN1()						(PORTR_DIR(I2C_SCL_GPIO1, 1))
#define I2C_SCL_H1()     					{I2C_SCL_OUT1();I2C_SCL_HIGH1();}
#define I2C_SCL_L1()     					{I2C_SCL_OUT1();I2C_SCL_LOW1();}

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
#define I2C_SDA_H1()     					{I2C_SDA_OUT1();I2C_SDA_HIGH1();}
#define I2C_SDA_L1()     					{I2C_SDA_OUT1();I2C_SDA_LOW1();}
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

#if 0
//	LED PIN1--------GPIO PC5(PIN15)
#define LED_PIN1_GPIO							_B30_
#define LED_PIN1_INIT()							do{JL_PORTC->PU &= ~LED_PIN1_GPIO; \
                                                   JL_PORTC->PD &= ~LED_PIN1_GPIO; \
                                                   JL_PORTC->HD &= ~LED_PIN1_GPIO;}while(0)
#define LED_PIN1_OUT()							(JL_PORTC->DIR &= ~LED_PIN1_GPIO)
#define LED_PIN1_IN()							(JL_PORTC->DIR |= LED_PIN1_GPIO)
#define LED_PIN1_HIGH()							(JL_PORTC->OUT |= LED_PIN1_GPIO)
#define LED_PIN1_LOW()							(JL_PORTC->OUT &= ~LED_PIN1_GPIO)
#define COM1_M()   								do{LED_PIN1_IN();\
												JL_PORTC->PU |= LED_PIN1_GPIO;\
												JL_PORTC->PD |= LED_PIN1_GPIO;}while(0)//PC5
#define COM1_H()   {LED_PIN1_OUT();JL_PORTC->PD &= ~LED_PIN1_GPIO;JL_PORTC->PU |= LED_PIN1_GPIO;LED_PIN1_HIGH();}
#define COM1_L()   {LED_PIN1_OUT();JL_PORTC->PD |= LED_PIN1_GPIO;JL_PORTC->PU &= ~LED_PIN1_GPIO;LED_PIN1_LOW();}

//	LED PIN2--------GPIO PC4(PIN16)
#define LED_PIN2_GPIO							_B30_
#define LED_PIN2_INIT()							do{JL_PORTC->PU &= ~LED_PIN2_GPIO; \
                                                   JL_PORTC->PD &= ~LED_PIN2_GPIO; \
                                                   JL_PORTC->HD &= ~LED_PIN2_GPIO;}while(0)
#define LED_PIN2_OUT()							(JL_PORTC->DIR &= ~LED_PIN2_GPIO)
#define LED_PIN2_IN()							(JL_PORTC->DIR |= LED_PIN2_GPIO)
#define LED_PIN2_HIGH()							(JL_PORTC->OUT |= LED_PIN2_GPIO)
#define LED_PIN2_LOW()							(JL_PORTC->OUT &= ~LED_PIN2_GPIO)
#define COM2_M()   								do{LED_PIN2_IN();\
												JL_PORTC->PU |= LED_PIN2_GPIO;\
												JL_PORTC->PD |= LED_PIN2_GPIO;}while(0)//PC4
#define COM2_H()   {LED_PIN2_OUT();JL_PORTC->PD &= ~LED_PIN2_GPIO;JL_PORTC->PU |= LED_PIN2_GPIO;LED_PIN2_HIGH();}
#define COM2_L()   {LED_PIN2_OUT();JL_PORTC->PD |= LED_PIN2_GPIO;JL_PORTC->PU &= ~LED_PIN2_GPIO;LED_PIN2_LOW();}

//	LED PIN3--------GPIO PC3(PIN17)
#define LED_PIN3_GPIO							_B30_
#define LED_PIN3_INIT()							do{JL_PORTC->PU &= ~LED_PIN3_GPIO; \
                                                   JL_PORTC->PD &= ~LED_PIN3_GPIO; \
                                                   JL_PORTC->HD &= ~LED_PIN3_GPIO;}while(0)
#define LED_PIN3_OUT()							(JL_PORTC->DIR &= ~LED_PIN3_GPIO)
#define LED_PIN3_IN()							(JL_PORTC->DIR |= LED_PIN3_GPIO)
#define LED_PIN3_HIGH()							(JL_PORTC->OUT |= LED_PIN3_GPIO)
#define LED_PIN3_LOW()							(JL_PORTC->OUT &= ~LED_PIN3_GPIO)
#define COM3_M()   								do{LED_PIN3_IN();\
												JL_PORTC->PU |= LED_PIN3_GPIO;\
												JL_PORTC->PD |= LED_PIN3_GPIO;}while(0)//PC3
#define COM3_H()   {LED_PIN3_OUT();JL_PORTC->PD &= ~LED_PIN3_GPIO;JL_PORTC->PU |= LED_PIN3_GPIO;LED_PIN3_HIGH();}
#define COM3_L()   {LED_PIN3_OUT();JL_PORTC->PD |= LED_PIN3_GPIO;JL_PORTC->PU &= ~LED_PIN3_GPIO;LED_PIN3_LOW();}

//	LED PIN4--------GPIO PC2(PIN18)
#define LED_PIN4_GPIO							_B30_
#define LED_PIN4_INIT()							do{JL_PORTC->PU &= ~LED_PIN4_GPIO; \
                                                   JL_PORTC->PD &= ~LED_PIN4_GPIO; \
                                                   JL_PORTC->HD &= ~LED_PIN4_GPIO;}while(0)
#define LED_PIN4_OUT()							(JL_PORTC->DIR &= ~LED_PIN4_GPIO)
#define LED_PIN4_IN()							(JL_PORTC->DIR |= LED_PIN4_GPIO)
#define LED_PIN4_HIGH()							(JL_PORTC->OUT |= LED_PIN4_GPIO)
#define LED_PIN4_LOW()							(JL_PORTC->OUT &= ~LED_PIN4_GPIO)
#define COM4_M()   								do{LED_PIN4_IN();\
												JL_PORTC->PU |= LED_PIN4_GPIO;\
												JL_PORTC->PD |= LED_PIN4_GPIO;}while(0)//PC2
#define COM4_H()   {LED_PIN4_OUT();JL_PORTC->PD &= ~LED_PIN4_GPIO;JL_PORTC->PU |= LED_PIN4_GPIO;LED_PIN4_HIGH();}
#define COM4_L()   {LED_PIN4_OUT();JL_PORTC->PD |= LED_PIN4_GPIO;JL_PORTC->PU &= ~LED_PIN4_GPIO;LED_PIN4_LOW();}

//	LED PIN5--------GPIO PC1(PIN19)
#define LED_PIN5_GPIO							_B30_
#define LED_PIN5_INIT()							do{JL_PORTC->PU &= ~LED_PIN5_GPIO; \
                                                   JL_PORTC->PD &= ~LED_PIN5_GPIO; \
                                                   JL_PORTC->HD &= ~LED_PIN5_GPIO;}while(0)
#define LED_PIN5_OUT()							(JL_PORTC->DIR &= ~LED_PIN5_GPIO)
#define LED_PIN5_IN()							(JL_PORTC->DIR |= LED_PIN5_GPIO)
#define LED_PIN5_HIGH()							(JL_PORTC->OUT |= LED_PIN5_GPIO)
#define LED_PIN5_LOW()							(JL_PORTC->OUT &= ~LED_PIN5_GPIO)
#define COM5_M()   								do{LED_PIN5_IN();\
												JL_PORTC->PU |= LED_PIN5_GPIO;\
												JL_PORTC->PD |= LED_PIN5_GPIO;}while(0)//PC1
#define COM5_H()   {LED_PIN5_OUT();JL_PORTC->PD &= ~LED_PIN5_GPIO;JL_PORTC->PU |= LED_PIN5_GPIO;LED_PIN5_HIGH();}
#define COM5_L()   {LED_PIN5_OUT();JL_PORTC->PD |= LED_PIN5_GPIO;JL_PORTC->PU &= ~LED_PIN5_GPIO;LED_PIN5_LOW();}





//	LED PIN6--------GPIO PA11(PIN25)
#define LED_PIN6_GPIO							_B30_
#define LED_PIN6_INIT()							do{JL_PORTA->PU &= ~LED_PIN6_GPIO; \
                                                   JL_PORTA->PD &= ~LED_PIN6_GPIO; \
                                                   JL_PORTA->HD &= ~LED_PIN6_GPIO;}while(0)
#define LED_PIN6_OUT()							(JL_PORTA->DIR &= ~LED_PIN6_GPIO)
#define LED_PIN6_IN()							(JL_PORTA->DIR |= LED_PIN6_GPIO)
#define LED_PIN6_HIGH()							(JL_PORTA->OUT |= LED_PIN6_GPIO)
#define LED_PIN6_LOW()							(JL_PORTA->OUT &= ~LED_PIN6_GPIO)


//	LED PIN7--------GPIO PA10(PIN26)
#define LED_PIN7_GPIO							_B30_
#define LED_PIN7_INIT()							do{JL_PORTA->PU &= ~LED_PIN7_GPIO; \
                                                   JL_PORTA->PD &= ~LED_PIN7_GPIO; \
                                                   JL_PORTA->HD &= ~LED_PIN7_GPIO;}while(0)
#define LED_PIN7_OUT()							(JL_PORTA->DIR &= ~LED_PIN7_GPIO)
#define LED_PIN7_IN()							(JL_PORTA->DIR |= LED_PIN7_GPIO)
#define LED_PIN7_HIGH()							(JL_PORTA->OUT |= LED_PIN7_GPIO)
#define LED_PIN7_LOW()							(JL_PORTA->OUT &= ~LED_PIN7_GPIO)



//	SEG7--------GPIO PA15(PIN20)
#define SEG7_GPIO							_B30_
#define SEG7_INIT()							do{JL_PORTA->PU &= ~SEG7_GPIO; \
                                            JL_PORTA->PD &= ~SEG7_GPIO; \
                                            JL_PORTA->HD &= ~SEG7_GPIO;}while(0)
#define SEG7_OUT()							(JL_PORTA->DIR &= ~SEG7_GPIO)
#define SEG7_IN()							(JL_PORTA->DIR |= SEG7_GPIO)
#define SEG7_HIGH()							(JL_PORTA->OUT |= SEG7_GPIO)
#define SEG7_LOW()							(JL_PORTA->OUT &= ~SEG7_GPIO)


//	SEG8--------GPIO PC0(PIN22)
#define SEG8_GPIO							_B30_
#define SEG8_INIT()							do{JL_PORTC->PU &= ~SEG8_GPIO; \
                                            JL_PORTC->PD &= ~SEG8_GPIO; \
                                            JL_PORTC->HD &= ~SEG8_GPIO;}while(0)
#define SEG8_OUT()							(JL_PORTC->DIR &= ~SEG8_GPIO)
#define SEG8_IN()							(JL_PORTC->DIR |= SEG8_GPIO)
#define SEG8_HIGH()							(JL_PORTC->OUT |= SEG8_GPIO)
#define SEG8_LOW()							(JL_PORTC->OUT &= ~SEG8_GPIO)


//	SEG6--------GPIO PA14(PIN21)
#define SEG6_GPIO							_B30_
#define SEG6_INIT()							do{JL_PORTA->PU &= ~SEG6_GPIO; \
                                            JL_PORTA->PD &= ~SEG6_GPIO; \
                                            JL_PORTA->HD &= ~SEG6_GPIO;}while(0)
#define SEG6_OUT()							(JL_PORTA->DIR &= ~SEG6_GPIO)
#define SEG6_IN()							(JL_PORTA->DIR |= SEG6_GPIO)
#define SEG6_HIGH()							(JL_PORTA->OUT |= SEG6_GPIO)
#define SEG6_LOW()							(JL_PORTA->OUT &= ~SEG6_GPIO)


//	SEG5--------GPIO PA11(PIN25)
#define SEG5_GPIO							_B30_
#define SEG5_INIT()							do{JL_PORTA->PU &= ~SEG5_GPIO; \
                                            JL_PORTA->PD &= ~SEG5_GPIO; \
                                            JL_PORTA->HD &= ~SEG5_GPIO;}while(0)
#define SEG5_OUT()							(JL_PORTA->DIR &= ~SEG5_GPIO)
#define SEG5_IN()							(JL_PORTA->DIR |= SEG5_GPIO)
#define SEG5_HIGH()							(JL_PORTA->OUT |= SEG5_GPIO)
#define SEG5_LOW()							(JL_PORTA->OUT &= ~SEG5_GPIO)


//	SEG4--------GPIO PA10(PIN26)
#define SEG4_GPIO							_B30_
#define SEG4_INIT()							do{JL_PORTA->PU &= ~SEG4_GPIO; \
                                            JL_PORTA->PD &= ~SEG4_GPIO; \
                                            JL_PORTA->HD &= ~SEG4_GPIO;}while(0)
#define SEG4_OUT()							(JL_PORTA->DIR &= ~SEG4_GPIO)
#define SEG4_IN()							(JL_PORTA->DIR |= SEG4_GPIO)
#define SEG4_HIGH()							(JL_PORTA->OUT |= SEG4_GPIO)
#define SEG4_LOW()							(JL_PORTA->OUT &= ~SEG4_GPIO)

//	SEG3--------GPIO PA9(PIN27)
#define SEG3_GPIO							_B30_
#define SEG3_INIT()							do{JL_PORTA->PU &= ~SEG3_GPIO; \
                                            JL_PORTA->PD &= ~SEG3_GPIO; \
                                            JL_PORTA->HD &= ~SEG3_GPIO;}while(0)
#define SEG3_OUT()							(JL_PORTA->DIR &= ~SEG3_GPIO)
#define SEG3_IN()							(JL_PORTA->DIR |= SEG3_GPIO)
#define SEG3_HIGH()							(JL_PORTA->OUT |= SEG3_GPIO)
#define SEG3_LOW()							(JL_PORTA->OUT &= ~SEG3_GPIO)

#define LED7PIN_DET_INIT()					do{JL_PORTA->PU &= ~SEG3_GPIO; \
                                            JL_PORTA->PD |= SEG3_GPIO; \
                                            JL_PORTA->HD &= ~SEG3_GPIO;\
                                            JL_PORTA->DIR |= SEG3_GPIO;}while(0)
#define LED7PIN_DET_GET()					(JL_PORTA->IN & SEG3_GPIO)
#define LED7PIN_DET_EXIT()					do{JL_PORTA->PU &= ~SEG3_GPIO; \
                                            JL_PORTA->PD &= ~SEG3_GPIO; \
                                            JL_PORTA->HD &= ~SEG3_GPIO;}while(0)

//	SEG2--------GPIO PA4(PIN28)
#define SEG2_GPIO							_B30_
#define SEG2_INIT()							do{JL_PORTA->PU &= ~SEG2_GPIO; \
                                            JL_PORTA->PD &= ~SEG2_GPIO; \
                                            JL_PORTA->HD &= ~SEG2_GPIO;}while(0)
#define SEG2_OUT()							(JL_PORTA->DIR &= ~SEG2_GPIO)
#define SEG2_IN()							(JL_PORTA->DIR |= SEG2_GPIO)
#define SEG2_HIGH()							(JL_PORTA->OUT |= SEG2_GPIO)
#define SEG2_LOW()							(JL_PORTA->OUT &= ~SEG2_GPIO)

//	SEG1--------GPIO PA3(PIN29)
#define SEG1_GPIO							_B30_
#define SEG1_INIT()							do{JL_PORTA->PU &= ~SEG1_GPIO; \
                                            JL_PORTA->PD &= ~SEG1_GPIO; \
                                            JL_PORTA->HD &= ~SEG1_GPIO;}while(0)
#define SEG1_OUT()							(JL_PORTA->DIR &= ~SEG1_GPIO)
#define SEG1_IN()							(JL_PORTA->DIR |= SEG1_GPIO)
#define SEG1_HIGH()							(JL_PORTA->OUT |= SEG1_GPIO)
#define SEG1_LOW()							(JL_PORTA->OUT &= ~SEG1_GPIO)

#endif

#endif
