
#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

#if defined(_SUPPORT_VR_)
#define ADC_ENCODE_PIN						AD_KEY_PR2
#endif

#if defined(_SPECIAL_DRIVER_ADC_)

/*----------------------------------------------------------------------------------------------------------------*/
//	KEY1  // PB8
/*----------------------------------------------------------------------------------------------------------------*/
#define ADC_KEY1_GPIO							_B8_

#define ADC_KEY1_INIT()							do{JL_PORTB->PU |= ADC_KEY1_GPIO; \
                                                   JL_PORTB->PD &= ADC_KEY1_GPIO; \
                                                   JL_PORTB->HD |= ~ADC_KEY1_GPIO;}while(0)
#define ADC_KEY1_HIGH()							(JL_PORTB->OUT |= ADC_KEY1_GPIO)
#define ADC_KEY1_LOW()							(JL_PORTB->OUT &= ~ADC_KEY1_GPIO)
#define ADC_KEY1_OUT()							(JL_PORTB->DIR &= ~ADC_KEY1_GPIO)
#define ADC_KEY1_IN()							(JL_PORTB->DIR |= ADC_KEY1_GPIO)

#define ADC_KEY1_GET()							(JL_PORTB->IN & ADC_KEY1_GPIO)
#define ADC_KEY1_CHECK()						(ADC_KEY1_GET() ? 0 : 1)

/*----------------------------------------------------------------------------------------------------------------*/
//	KEY2  // PB3
/*----------------------------------------------------------------------------------------------------------------*/
#define ADC_KEY2_GPIO							_B3_

#define ADC_KEY2_INIT()							do{JL_PORTB->PU |= ADC_KEY2_GPIO; \
                                                   JL_PORTB->PD &= ~ADC_KEY2_GPIO; \
                                                   JL_PORTB->HD |= ADC_KEY2_GPIO;}while(0)
#define ADC_KEY2_HIGH()							(JL_PORTB->OUT |= ADC_KEY2_GPIO)
#define ADC_KEY2_LOW()							(JL_PORTB->OUT &= ~ADC_KEY2_GPIO)
#define ADC_KEY2_OUT()							(JL_PORTB->DIR &= ~ADC_KEY2_GPIO)
#define ADC_KEY2_IN()							(JL_PORTB->DIR |= ADC_KEY2_GPIO)

#define ADC_KEY2_GET()							(JL_PORTB->IN & ADC_KEY2_GPIO)
#define ADC_KEY2_CHECK()						(ADC_KEY2_GET() ? 0 : 1)

/*----------------------------------------------------------------------------------------------------------------*/
//	KEY3  // PB7
/*----------------------------------------------------------------------------------------------------------------*/
#define ADC_KEY3_GPIO							_B7_

#define ADC_KEY3_INIT()							do{JL_PORTB->PU |= ADC_KEY3_GPIO; \
                                                   JL_PORTB->PD &= ~ADC_KEY3_GPIO; \
                                                   JL_PORTB->HD |= ADC_KEY3_GPIO;}while(0)
#define ADC_KEY3_HIGH()							(JL_PORTB->OUT |= ADC_KEY3_GPIO)
#define ADC_KEY3_LOW()							(JL_PORTB->OUT &= ~ADC_KEY3_GPIO)
#define ADC_KEY3_OUT()							(JL_PORTB->DIR &= ~ADC_KEY3_GPIO)
#define ADC_KEY3_IN()							(JL_PORTB->DIR |= ADC_KEY3_GPIO)

#define ADC_KEY3_GET()							(JL_PORTB->IN & ADC_KEY3_GPIO)
#define ADC_KEY3_CHECK()						(ADC_KEY3_GET() ? 0 : 1)

/*----------------------------------------------------------------------------------------------------------------*/
//	KEY4  // PB6
/*----------------------------------------------------------------------------------------------------------------*/
#define ADC_KEY4_GPIO							_B6_

#define ADC_KEY4_INIT()							do{JL_PORTB->PU |= ADC_KEY4_GPIO; \
                                                   JL_PORTB->PD &= ~ADC_KEY4_GPIO; \
                                                   JL_PORTB->HD |= ADC_KEY4_GPIO;}while(0)
#define ADC_KEY4_HIGH()							(JL_PORTB->OUT |= ADC_KEY4_GPIO)
#define ADC_KEY4_LOW()							(JL_PORTB->OUT &= ~ADC_KEY4_GPIO)
#define ADC_KEY4_OUT()							(JL_PORTB->DIR &= ~ADC_KEY4_GPIO)
#define ADC_KEY4_IN()							(JL_PORTB->DIR |= ADC_KEY4_GPIO)

#define ADC_KEY4_GET()							(JL_PORTB->IN & ADC_KEY4_GPIO)
#define ADC_KEY4_CHECK()						(ADC_KEY4_GET() ? 0 : 1)

#else
//	KEY--------GPIO PB8(PIN59)
#if !defined(_DISABLE_KEY_FUNTION_)
#define ADC_KEY1_PIN						AD_KEY_PB8
#endif

//	KEY2--------GPIO PB9(PIN57)
#if !defined(_DISABLE_KEY_FUNTION_)
#define ADC_KEY2_PIN						AD_KEY_PB9
#endif

#endif
//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN								//do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)


//	ACC PIN--------GPIO PR3(PIN7)
#define ACC_GPIO								PORTR1

#define ACC_INIT()								do{PORTR_PU(ACC_GPIO, 1);	\
                                                   PORTR_PD(ACC_GPIO, 0);	\
                                                   PORTR_DIR(ACC_GPIO, 1);	\
												   PORTR_DIE(ACC_GPIO, 1);	\
												   PORTR_HD(ACC_GPIO, 1);}while(0)
#define ACC_GET()								(PORTR_IN(ACC_GPIO))
#define ACC_CHECK()								(ACC_GET() ? 0 : 1)





//	AUDIO_MUTE PIN--------GPIO PR3(PIN11)
#define AUDIO_MUTE_GPIO							PORTR3
#define AUDIO_MUTE_INIT()						do{PORTR_PU(AUDIO_MUTE_GPIO, 0);\
                                                   PORTR_PD(AUDIO_MUTE_GPIO, 0);\
												   PORTR_HD(AUDIO_MUTE_GPIO, 1);}while(0)
#define AUDIO_MUTE_OUT()						(PORTR_DIR(AUDIO_MUTE_GPIO, 0))
#define AUDIO_MUTE_HIGH()						{PORTR_OUT(AUDIO_MUTE_GPIO, 1);PORTR_HD(AUDIO_MUTE_GPIO, 0);}
#define AUDIO_MUTE_LOW()						{PORTR_OUT(AUDIO_MUTE_GPIO, 0);PORTR_HD(AUDIO_MUTE_GPIO, 0);}
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}

//	SYS_POW PIN--------GPIO PB9(PIN44)
#define SYS_POW_GPIO							_B30_
#define SYS_POW_INIT()							do{JL_PORTB->PU &= ~SYS_POW_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_POW_GPIO; \
                                                   JL_PORTB->HD &= ~SYS_POW_GPIO;}while(0)
#define SYS_POW_OUT()							(JL_PORTB->DIR &= ~SYS_POW_GPIO)
#define SYS_POW_HIGH()							(JL_PORTB->OUT |= SYS_POW_GPIO)
#define SYS_POW_LOW()							(JL_PORTB->OUT &= ~SYS_POW_GPIO)
#define SYS_POW_ON()     						{SYS_POW_OUT(); SYS_POW_HIGH();}
#define SYS_POW_OFF()     						{SYS_POW_OUT(); SYS_POW_LOW();}
#define SYS_POW_IN()							(JL_PORTB->DIR |= SYS_POW_GPIO)

//	I2C_SCL PIN--------GPIO PB9(PIN57)
#define I2C_SCL_GPIO						_B9_

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

//	I2C_SDA PIN--------GPIO PB10(PIN56)
#define I2C_SDA_GPIO						_B10_

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

#if defined(_SUPPORT_TOUCH_KEY_)
/*----------------------------------------------------------------------------------------------------------------*/
#define TOUCH_CLK_GPIO							_B6_

#define TOUCH_CLK_INIT()						do{JL_PORTB->PU &= ~TOUCH_CLK_GPIO;\
                                                 JL_PORTB->PD &= ~TOUCH_CLK_GPIO;\
                                                 JL_PORTB->HD &= ~TOUCH_CLK_GPIO;}while(0)
#define TOUCH_CLK_HIGH()						(JL_PORTB->OUT |= TOUCH_CLK_GPIO)
#define TOUCH_CLK_LOW()							(JL_PORTB->OUT &= ~TOUCH_CLK_GPIO)
#define TOUCH_CLK_OUT()							(JL_PORTB->DIR &= ~TOUCH_CLK_GPIO)
#define TOUCH_CLK_IN()							(JL_PORTB->DIR |= TOUCH_CLK_GPIO)
#define TOUCH_CLK_GET()							(JL_PORTB->IN & TOUCH_CLK_GPIO)
//#define TOUCH_CLK_H()     						{TOUCH_CLK_OUT(); TOUCH_CLK_HIGH();} //TOUCH_CLK_IN()
#define TOUCH_CLK_H()     						TOUCH_CLK_IN()
#define TOUCH_CLK_L()     						{TOUCH_CLK_OUT(); TOUCH_CLK_LOW();}

/*----------------------------------------------------------------------------------------------------------------*/
#define TOUCH_DATA_GPIO							_B7_

#define TOUCH_DATA_INIT()						do{JL_PORTB->PU &= ~TOUCH_DATA_GPIO;\
                                                 JL_PORTB->PD &= ~TOUCH_DATA_GPIO;\
                                                 JL_PORTB->HD &= ~TOUCH_DATA_GPIO;}while(0)
#define TOUCH_DATA_HIGH()						(JL_PORTB->OUT |= TOUCH_DATA_GPIO)
#define TOUCH_DATA_LOW()						(JL_PORTB->OUT &= ~TOUCH_DATA_GPIO)
#define TOUCH_DATA_OUT()						(JL_PORTB->DIR &= ~TOUCH_DATA_GPIO)
#define TOUCH_DATA_IN()							(JL_PORTB->DIR |= TOUCH_DATA_GPIO)
#define TOUCH_DATA_GET()						(JL_PORTB->IN & TOUCH_DATA_GPIO)
//#define TOUCH_DATA_H()     						{TOUCH_DATA_OUT(); TOUCH_DATA_HIGH();} //TOUCH_DATA_IN()
#define TOUCH_DATA_H()     						TOUCH_DATA_IN()
#define TOUCH_DATA_L()     						{TOUCH_DATA_OUT(); TOUCH_DATA_LOW();}
#endif

#if defined(_ENABLE_BEEP_)
/*----------PIN54(B12)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_BEEP_GPIO							_B12_

#define SYS_BEEP_INIT()							do{JL_PORTB->PU |= SYS_BEEP_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_BEEP_GPIO; \
                                                   JL_PORTB->HD |= SYS_BEEP_GPIO;}while(0)
#define SYS_BEEP_HIGH()							(JL_PORTB->OUT |= SYS_BEEP_GPIO)
#define SYS_BEEP_LOW()							(JL_PORTB->OUT &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_OUT()							(JL_PORTB->DIR &= ~SYS_BEEP_GPIO)
#define SYS_BEEP_IN()							(JL_PORTB->DIR |= SYS_BEEP_GPIO)
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

/*---------低音炮PIN1(PB5)---------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_SUB_DETECT_)
#define SYS_SUB_GPIO							_B5_

#define SYS_SUB_INIT()							do{JL_PORTB->PU &= ~SYS_SUB_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_SUB_GPIO; \
                                                   JL_PORTB->HD &= ~SYS_SUB_GPIO;}while(0)
#define SYS_SUB_HIGH()							(JL_PORTB->OUT |= SYS_SUB_GPIO)
#define SYS_SUB_LOW()							(JL_PORTB->OUT &= ~SYS_SUB_GPIO)
#define SYS_SUB_OUT()							(JL_PORTB->DIR &= ~SYS_SUB_GPIO)
#define SYS_SUB_IN()							(JL_PORTB->DIR |= SYS_SUB_GPIO)
#define SYS_SUB_ON()     						{SYS_SUB_OUT(); SYS_SUB_HIGH();}
#define SYS_SUB_OFF()     						{SYS_SUB_OUT(); SYS_SUB_LOW();}
#endif

/*-------背光灯控制PIN55(PB11)-----------------------------------------------------------------------------------------------------*/
#if defined(_KEY_LIGHT_CONTROL_)

#define SYS_LIGHT_GPIO							_B11_

#define SYS_LIGHT_INIT()						 do{JL_PORTB->PU |= SYS_LIGHT_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_LIGHT_GPIO; \
                                                   JL_PORTB->HD |= SYS_LIGHT_GPIO;}while(0)
#define SYS_LIGHT_HIGH()						(JL_PORTB->OUT |= SYS_LIGHT_GPIO)
#define SYS_LIGHT_LOW()							(JL_PORTB->OUT &= ~SYS_LIGHT_GPIO)
#define SYS_LIGHT_OUT()							(JL_PORTB->DIR &= ~SYS_LIGHT_GPIO)
#define SYS_LIGHT_IN()							(JL_PORTB->DIR |= SYS_LIGHT_GPIO)
#define SYS_LIGHT_ON()     						{SYS_LIGHT_OUT(); SYS_LIGHT_HIGH();}
#define SYS_LIGHT_OFF()     					{SYS_LIGHT_OUT(); SYS_LIGHT_LOW();}
#endif

/*-------USB供电PIN1(PB5)-----------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_USB_CONTROL_)
#if defined(_ENABLE_USB_PIN_)
#define SYS_USB_GPIO							_B3_
#else
#define SYS_USB_GPIO							_B5_
#endif

#define SYS_USB_INIT()						 do{JL_PORTB->PU |= SYS_USB_GPIO; \
                                        		JL_PORTB->PD &= ~SYS_USB_GPIO; \
                                        		JL_PORTB->HD |= SYS_USB_GPIO;}while(0)
#define SYS_USB_HIGH()							(JL_PORTB->OUT |= SYS_USB_GPIO)
#define SYS_USB_LOW()							(JL_PORTB->OUT &= ~SYS_USB_GPIO)
#define SYS_USB_OUT()							(JL_PORTB->DIR &= ~SYS_USB_GPIO)
#define SYS_USB_IN()							(JL_PORTB->DIR |= SYS_USB_GPIO)
#define SYS_USB_ON()     						{SYS_USB_OUT(); SYS_USB_HIGH();}
#define SYS_USB_OFF()     						{SYS_USB_OUT(); SYS_USB_LOW();}
#endif

#endif

