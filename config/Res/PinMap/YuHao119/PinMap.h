#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__
#include "rtc/rtc_api.h"

#if defined(_FY700B_PARTY_CONTROL_)

//	ACC PIN--------GPIO PB3 (PIN45)
#define ACC_GPIO								_B3_

#define ACC_INIT()								do{JL_PORTB->PU |= ACC_GPIO; \
                                         		JL_PORTB->PD &= ~ACC_GPIO; \
                                            	JL_PORTB->HD &= ~ACC_GPIO;}while(0)

#define ACC_GET()								(JL_PORTB->IN & ACC_GPIO)                                           
#define ACC_CHECK()								(ACC_GET() ? 1 : 0)

#else
//	ACC PIN--------GPIO PR1(PIN5)

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

//	DAB_RX PIN1--------GPIO PC5(PIN15)
#define DAB_RX_GPIO							_B5_
#define DAB_RX_INIT()						do{JL_PORTC->PU |= DAB_RX_GPIO; \
                                            JL_PORTC->PD &= ~DAB_RX_GPIO; \
                                            JL_PORTC->HD &= ~DAB_RX_GPIO;}while(0)
#define DAB_RX_OUT()						(JL_PORTC->DIR &= ~DAB_RX_GPIO)
#define DAB_RX_IN()							(JL_PORTC->DIR |= DAB_RX_GPIO)
#define DAB_RX_HIGH()						(JL_PORTC->OUT |= DAB_RX_GPIO)
#define DAB_RX_LOW()						(JL_PORTC->OUT &= ~DAB_RX_GPIO)

//	DAB_TX PIN2--------GPIO PC4(PIN16)
#define DAB_TX_GPIO							_B4_
#define DAB_TX_INIT()						do{JL_PORTC->PU |= DAB_TX_GPIO; \
                                            JL_PORTC->PD &= ~DAB_TX_GPIO; \
                                            JL_PORTC->HD &= ~DAB_TX_GPIO;}while(0)
#define DAB_TX_OUT()						(JL_PORTC->DIR &= ~DAB_TX_GPIO)
#define DAB_TX_IN()							(JL_PORTC->DIR |= DAB_TX_GPIO)
#define DAB_TX_HIGH()						(JL_PORTC->OUT |= DAB_TX_GPIO)
#define DAB_TX_LOW()						(JL_PORTC->OUT &= ~DAB_TX_GPIO)


//	SYS_POW PIN--------GPIO PR3(PIN7)
#if defined(_FY700B_PARTY_CONTROL_)
#define SYS_POW_GPIO							_B30_
#else
#define SYS_POW_GPIO							_B10_
#endif

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

//	AUDIO_MUTE PIN--------GPIO PA11(PIN25)
#define AUDIO_MUTE_GPIO							_B11_
#define AUDIO_MUTE_INIT()						do{JL_PORTA->PU &= ~AUDIO_MUTE_GPIO; \
                                           		 JL_PORTA->PD &= ~AUDIO_MUTE_GPIO;}while(0)
#define AUDIO_MUTE_OUT()						(JL_PORTA->DIR &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_HIGH()						(JL_PORTA->OUT |= AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_LOW()						(JL_PORTA->OUT &= ~AUDIO_MUTE_GPIO)
#define AUDIO_MUTE_ON()							{AUDIO_MUTE_OUT();AUDIO_MUTE_HIGH();}
#define AUDIO_MUTE_OFF()						{AUDIO_MUTE_OUT();AUDIO_MUTE_LOW();}

//	DOT_SDA--------GPIO PA15(PIN20)
#define LCD_SDA_GPIO						_B15_
#define LCD_DATA_INIT()						do{JL_PORTA->PU &= ~LCD_SDA_GPIO; \
                                            JL_PORTA->PD &= ~LCD_SDA_GPIO; \
                                            JL_PORTA->HD &= ~LCD_SDA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTA->OUT |= LCD_SDA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTA->OUT &= ~LCD_SDA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTA->DIR &= ~LCD_SDA_GPIO)
#define LCD_DATA_IN()						(JL_PORTA->DIR |= LCD_SDA_GPIO)

//DOT_SCL--------GPIO PA14(PIN21)
#define LCD_CLK_GPIO						_B14_
#define LCD_CLK_INIT()						do{JL_PORTA->PU &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTA->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTA->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTA->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTA->DIR |= LCD_CLK_GPIO) 

//NULL--------GPIO PC0(PIN22)
#define LCD_CE_GPIO							_B0_
#define LCD_CE_INIT()						do{JL_PORTC->PU &= ~LCD_CE_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()						(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()						(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()						(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()							(JL_PORTC->DIR |= LCD_CE_GPIO)

//	I2C_SCL PIN--------GPIO PA2(PIN30)
#define I2C_SCL_GPIO						_B2_
#define I2C_SCL_INIT()						do{JL_PORTA->PU |= I2C_SCL_GPIO; \
                                            JL_PORTA->PD &= ~I2C_SCL_GPIO; \
                                            JL_PORTA->HD &= ~I2C_SCL_GPIO;}while(0)
#define I2C_SCL_HIGH()						(JL_PORTA->OUT |= I2C_SCL_GPIO)
#define I2C_SCL_LOW()						(JL_PORTA->OUT &= ~I2C_SCL_GPIO)
#define I2C_SCL_OUT()						(JL_PORTA->DIR &= ~I2C_SCL_GPIO)
#define I2C_SCL_IN()						(JL_PORTA->DIR |= I2C_SCL_GPIO)
#define I2C_SCL_H()     					{I2C_SCL_OUT();I2C_SCL_HIGH();}
#define I2C_SCL_L()     					{I2C_SCL_OUT();I2C_SCL_LOW();}

//	I2C_SDA PIN--------GPIO PA1(PIN31)
#define I2C_SDA_GPIO						_B1_

#define I2C_SDA_INIT()						do{JL_PORTA->PU |= I2C_SDA_GPIO; \
                                            JL_PORTA->PD &= ~I2C_SDA_GPIO; \
                                            JL_PORTA->HD &= ~I2C_SDA_GPIO;}while(0)
#define I2C_SDA_HIGH()						(JL_PORTA->OUT |= I2C_SDA_GPIO)
#define I2C_SDA_LOW()						(JL_PORTA->OUT &= ~I2C_SDA_GPIO)
#define I2C_SDA_OUT()						(JL_PORTA->DIR &= ~I2C_SDA_GPIO)
#define I2C_SDA_IN()						(JL_PORTA->DIR |= I2C_SDA_GPIO)
#define I2C_SDA_GET()						(JL_PORTA->IN & I2C_SDA_GPIO)
#define I2C_SDA_H()     					{I2C_SDA_OUT();I2C_SDA_HIGH();}
#define I2C_SDA_L()     					{I2C_SDA_OUT();I2C_SDA_LOW();}

//	KEY--------GPIO PA9(PIN27)
#define ADC_KEY1_PIN						AD_KEY_PA9

//	KEY2--------GPIO PA3(PIN29)
#define ADC_KEY2_PIN						AD_KEY_PA3

#if defined(_FY700B_PARTY_CONTROL_)
//	KEY3--------GPIO PR1(PIN5)
#define ADC_KEY3_PIN						AD_KEY_PR1

//	KEY4--------GPIO PA10(PIN26)
#define ADC_KEY4_PIN						AD_KEY_PA10

#endif

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO A4
/*----------------------------------------------------------------------------------------------------------------*/
#define RADIO_RESET_GPIO						_B4_

#define RADIO_RESET_INIT()						do{JL_PORTA->PU &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->PD &= ~RADIO_RESET_GPIO; \
                                                   JL_PORTA->HD &= ~RADIO_RESET_GPIO;}while(0)
#define RADIO_RESET_HIGH()						(JL_PORTA->OUT |= RADIO_RESET_GPIO)
#define RADIO_RESET_LOW()						(JL_PORTA->OUT &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_OUT()						(JL_PORTA->DIR &= ~RADIO_RESET_GPIO)
#define RADIO_RESET_IN()						(JL_PORTA->DIR |= RADIO_RESET_GPIO)
#define RADIO_RESET_ON()     					{RADIO_RESET_OUT();RADIO_RESET_LOW();}
#define RADIO_RESET_OFF()     					{RADIO_RESET_OUT();RADIO_RESET_HIGH();}

//	DTH PIN--------GPIO C11
#if defined(_SUPPORT_DTH_)

#define DTH_GPIO								_B9_

#define DTH_INIT()								do{JL_PORTB->PU |= DTH_GPIO; \
                                               	   JL_PORTB->PD &= ~DTH_GPIO; \
                                               	   JL_PORTB->HD |= DTH_GPIO;}while(0)
#define DTH_HIGH()								(JL_PORTB->OUT |= DTH_GPIO)
#define DTH_LOW()								(JL_PORTB->OUT &= ~DTH_GPIO)
#define DTH_OUT()								(JL_PORTB->DIR &= ~DTH_GPIO)
#define DTH_IN()								(JL_PORTB->DIR |= DTH_GPIO)
#define DTH_GET()								(JL_PORTB->IN & DTH_GPIO)
#define DTH_CHECK()								(DTH_GET() ? 0 : 1)

#endif

#if defined(_SUPPORT_PANEL_LIGHT_)
/*----------PIN17(C3)-REG-----------------------------------------------------------------------------------------------------*/
#define SYS_RED_GPIO							_B3_

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

/*-----------PIN19(C1)-BLUE--------------------------------------------------------------------------------------------------*/
#define SYS_BLUE_GPIO							_B1_

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

#if defined(_SUPPORT_SUBWOOFER2_)
/*----------------------------------------------------------------------------------------------------------------*/
//	SUB PIN(41)--------GPIO B12
/*----------------------------------------------------------------------------------------------------------------*/
#define SYS_SUB_GPIO							_B12_

#define SYS_SUB_INIT()							do{JL_PORTB->PU |= SYS_SUB_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_SUB_GPIO; \
                                                   JL_PORTB->HD |= SYS_SUB_GPIO;}while(0)
#define SYS_SUB_HIGH()							(JL_PORTB->OUT |= SYS_SUB_GPIO)
#define SYS_SUB_LOW()							(JL_PORTB->OUT &= ~SYS_SUB_GPIO)
#define SYS_SUB_OUT()							(JL_PORTB->DIR &= ~SYS_SUB_GPIO)
#define SYS_SUB_IN()							(JL_PORTB->DIR |= SYS_SUB_GPIO)
#define SYS_SUB_OFF()     						{SYS_SUB_OUT(); SYS_SUB_HIGH();}
#define SYS_SUB_ON()     						{SYS_SUB_OUT(); SYS_SUB_LOW();}

/*----------------------------------------------------------------------------------------------------------------*/
//	SUB PIN(42)--------GPIO B11
/*----------------------------------------------------------------------------------------------------------------*/
#define SYS_SUB2_GPIO							_B11_

#define SYS_SUB2_INIT()							do{JL_PORTB->PU |= SYS_SUB2_GPIO; \
                                                   JL_PORTB->PD &= ~SYS_SUB2_GPIO; \
                                                   JL_PORTB->HD |= SYS_SUB2_GPIO;}while(0)
#define SYS_SUB2_HIGH()							(JL_PORTB->OUT |= SYS_SUB2_GPIO)
#define SYS_SUB2_LOW()							(JL_PORTB->OUT &= ~SYS_SUB2_GPIO)
#define SYS_SUB2_OUT()							(JL_PORTB->DIR &= ~SYS_SUB2_GPIO)
#define SYS_SUB2_IN()							(JL_PORTB->DIR |= SYS_SUB2_GPIO)
#define SYS_SUB2_OFF()     						{SYS_SUB2_OUT(); SYS_SUB2_HIGH();}
#define SYS_SUB2_ON()     						{SYS_SUB2_OUT(); SYS_SUB2_LOW();}

#endif

#if defined(_ENABLE_INH_CONTROL_)
#define LCD_INH_GPIO						_B10_

#define LCD_INH_INIT()						do{JL_PORTB->PU &= ~LCD_INH_GPIO; \
                                               JL_PORTB->PD &= ~LCD_INH_GPIO; \
                                               JL_PORTB->HD &= ~LCD_INH_GPIO;}while(0)
#define LCD_INH_HIGH()						(JL_PORTB->OUT |= LCD_INH_GPIO)
#define LCD_INH_LOW()						(JL_PORTB->OUT &= ~LCD_INH_GPIO)
#define LCD_INH_OUT()						(JL_PORTB->DIR &= ~LCD_INH_GPIO)
#define LCD_INH_IN()						(JL_PORTB->DIR |= LCD_INH_GPIO)
#define LCD_INH_ON()     					{LCD_INH_OUT();LCD_INH_LOW();}
#define LCD_INH_OFF()     					{LCD_INH_OUT();LCD_INH_HIGH();}
#endif


#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_RADIO_LIGHT_)||defined(_ENABLE_AMP_LIGHT_)
#define AMP_CONTROL_GPIO						PORTR0
#define AMP_CONTROL_INIT()						do{PORTR_PU(AMP_CONTROL_GPIO, 0);\
                                                 PORTR_PD(AMP_CONTROL_GPIO, 0);}while(0)
#define AMP_CONTROL_OUT()						(PORTR_DIR(AMP_CONTROL_GPIO, 0))
#define AMP_CONTROL_HIGH()						(PORTR_OUT(AMP_CONTROL_GPIO, 1))
#define AMP_CONTROL_LOW()						(PORTR_OUT(AMP_CONTROL_GPIO, 0))
#define AMP_CONTROL_OFF()						{AMP_CONTROL_OUT();AMP_CONTROL_LOW();}
#define AMP_CONTROL_ON()						{AMP_CONTROL_OUT();AMP_CONTROL_HIGH();}

#endif

#if defined(_ENABLE_RCA_MUTE_)
#define RCA_MUTE_GPIO						PORTR3
#define RCA_MUTE_INIT()						do{PORTR_PU(RCA_MUTE_GPIO, 0);\
                                            PORTR_PD(RCA_MUTE_GPIO, 0);}while(0)
#define RCA_MUTE_OUT()						(PORTR_DIR(RCA_MUTE_GPIO, 0))
#define RCA_MUTE_HIGH()						(PORTR_OUT(RCA_MUTE_GPIO, 1))
#define RCA_MUTE_LOW()						(PORTR_OUT(RCA_MUTE_GPIO, 0))
#define RCA_MUTE_OFF()						{RCA_MUTE_OUT();RCA_MUTE_LOW();}
#define RCA_MUTE_ON()						{RCA_MUTE_OUT();RCA_MUTE_HIGH();}
#endif

#endif

