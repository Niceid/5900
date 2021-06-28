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


//	DOT_SDA--------GPIO PA15(PIN20)
#define LCD_SDA_GPIO						_B30_
#define LCD_DATA_INIT()						do{JL_PORTA->PU &= ~LCD_SDA_GPIO; \
                                            JL_PORTA->PD &= ~LCD_SDA_GPIO; \
                                            JL_PORTA->HD &= ~LCD_SDA_GPIO;}while(0)
#define LCD_DATA_HIGH()						(JL_PORTA->OUT |= LCD_SDA_GPIO)
#define LCD_DATA_LOW()						(JL_PORTA->OUT &= ~LCD_SDA_GPIO)
#define LCD_DATA_OUT()						(JL_PORTA->DIR &= ~LCD_SDA_GPIO)
#define LCD_DATA_IN()						(JL_PORTA->DIR |= LCD_SDA_GPIO)

//DOT_SCL--------GPIO PA14(PIN21)
#define LCD_CLK_GPIO						_B30_
#define LCD_CLK_INIT()						do{JL_PORTA->PU &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->PD &= ~LCD_CLK_GPIO; \
                                            JL_PORTA->HD &= ~LCD_CLK_GPIO;}while(0)
#define LCD_CLK_HIGH()						(JL_PORTA->OUT |= LCD_CLK_GPIO)
#define LCD_CLK_LOW()						(JL_PORTA->OUT &= ~LCD_CLK_GPIO)
#define LCD_CLK_OUT()						(JL_PORTA->DIR &= ~LCD_CLK_GPIO)
#define LCD_CLK_IN()						(JL_PORTA->DIR |= LCD_CLK_GPIO) 

//NULL--------GPIO PC0(PIN22)
#define LCD_CE_GPIO							_B30_
#define LCD_CE_INIT()						do{JL_PORTC->PU &= ~LCD_CE_GPIO; \
                                            JL_PORTC->PD &= ~LCD_CE_GPIO; \
                                            JL_PORTC->HD &= ~LCD_CE_GPIO;}while(0)
#define LCD_CE_HIGH()						(JL_PORTC->OUT |= LCD_CE_GPIO)
#define LCD_CE_LOW()						(JL_PORTC->OUT &= ~LCD_CE_GPIO)
#define LCD_CE_OUT()						(JL_PORTC->DIR &= ~LCD_CE_GPIO)
#define LCD_CE_IN()							(JL_PORTC->DIR |= LCD_CE_GPIO)
                                            
//	I2C_SCL PIN--------GPIO PA2(PINB12)
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

//	I2C_SDA PIN--------GPIO PA1(PINB11)
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

//	KEY--------GPIO PA9(PIN43)
#if !defined(_DISABLE_K1_FUNTION_)
#define ADC_KEY1_PIN						AD_KEY_PB10
#endif

//	KEY2--------GPIO PA3(PIN44)
#define ADC_KEY2_PIN						AD_KEY_PB9

//	IR PIN--------GPIO PB3(PIN45)
#define IR_INIT_PIN							do{JL_PORTB->DIR |= _B3_; JL_PORTB->DIE |= _B3_;}while(0)

/*----------------------------------------------------------------------------------------------------------------*/
//	RADIO_RESET PIN--------GPIO C3
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

//	DTH PIN--------GPIO C11
#if defined(_SUPPORT_DTH_)

#define DTH_GPIO								_B30_

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
#if defined( DAB_UART_ONOFF  )

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
#endif

/*----------------------------------------------------------------------------------------------------------------*/
//	LCM_RESET PIN--------GPIO PC2(PIN19)
#define LCM_RESET_GPIO						_B2_
#define LCM_RESET_INIT()					do{JL_PORTC->PU &= ~LCM_RESET_GPIO; \
                                            JL_PORTC->PD &= ~LCM_RESET_GPIO; \
                                            JL_PORTC->HD &= ~LCM_RESET_GPIO;}while(0)
#define LCM_RESET_OUT()						(JL_PORTC->DIR &= ~LCM_RESET_GPIO)
#define LCM_RESET_IN()						(JL_PORTC->DIR |= LCM_RESET_GPIO)
#define LCM_RESET_HIGH()					(JL_PORTC->OUT |= LCM_RESET_GPIO)
#define LCM_RESET_LOW()						(JL_PORTC->OUT &= ~LCM_RESET_GPIO)

//	LCM_RD PIN--------GPIO PA15(PIN20)
#define LCM_RD_GPIO							_B15_
#define LCM_RD_INIT()						do{JL_PORTA->PU &= ~LCM_RD_GPIO; \
                                            JL_PORTA->PD &= ~LCM_RD_GPIO; \
                                            JL_PORTA->HD &= ~LCM_RD_GPIO;}while(0)
#define LCM_RD_OUT()						(JL_PORTA->DIR &= ~LCM_RD_GPIO)
#define LCM_RD_IN()							(JL_PORTA->DIR |= LCM_RD_GPIO)
#define LCM_RD_HIGH()						(JL_PORTA->OUT |= LCM_RD_GPIO)
#define LCM_RD_LOW()						(JL_PORTA->OUT &= ~LCM_RD_GPIO)

//	LCM_WR PIN--------GPIO PC1(PIN19)
#define LCM_WR_GPIO							_B1_
#define LCM_WR_INIT()						do{JL_PORTC->PU &= ~LCM_WR_GPIO; \
                                            JL_PORTC->PD &= ~LCM_WR_GPIO; \
                                            JL_PORTC->HD &= ~LCM_WR_GPIO;}while(0)
#define LCM_WR_OUT()						(JL_PORTC->DIR &= ~LCM_WR_GPIO)
#define LCM_WR_IN()							(JL_PORTC->DIR |= LCM_WR_GPIO)
#define LCM_WR_HIGH()						(JL_PORTC->OUT |= LCM_WR_GPIO)
#define LCM_WR_LOW()						(JL_PORTC->OUT &= ~LCM_WR_GPIO)

//	LCM_CD PIN--------GPIO PA14(PIN21)
#define LCM_CD_GPIO							_B14_
#define LCM_CD_INIT()						do{JL_PORTA->PU &= ~LCM_CD_GPIO; \
                                            JL_PORTA->PD &= ~LCM_CD_GPIO; \
                                            JL_PORTA->HD &= ~LCM_CD_GPIO;}while(0)
#define LCM_CD_OUT()						(JL_PORTA->DIR &= ~LCM_CD_GPIO)
#define LCM_CD_IN()							(JL_PORTA->DIR |= LCM_CD_GPIO)
#define LCM_CD_HIGH()						(JL_PORTA->OUT |= LCM_CD_GPIO)
#define LCM_CD_LOW()						(JL_PORTA->OUT &= ~LCM_CD_GPIO)

//	LCM_CS0 PIN--------GPIO PC0(PIN22)
#define LCM_CS0_GPIO						_B0_
#define LCM_CS0_INIT()						do{JL_PORTC->PU &= ~LCM_CS0_GPIO; \
                                            JL_PORTC->PD &= ~LCM_CS0_GPIO; \
                                            JL_PORTC->HD &= ~LCM_CS0_GPIO;}while(0)
#define LCM_CS0_OUT()						(JL_PORTC->DIR &= ~LCM_CS0_GPIO)
#define LCM_CS0_IN()						(JL_PORTC->DIR |= LCM_CS0_GPIO)
#define LCM_CS0_HIGH()						(JL_PORTC->OUT |= LCM_CS0_GPIO)
#define LCM_CS0_LOW()						(JL_PORTC->OUT &= ~LCM_CS0_GPIO)

//	LCM_DATA0 PIN--------GPIO PA1(PIN31)
#define LCM_DATA0_GPIO						_B1_
#define LCM_DATA0_INIT()					do{JL_PORTA->PU &= ~LCM_DATA0_GPIO; \
                                            JL_PORTA->PD &= ~LCM_DATA0_GPIO; \
                                            JL_PORTA->HD &= ~LCM_DATA0_GPIO;}while(0)
#define LCM_DATA0_OUT()						(JL_PORTA->DIR &= ~LCM_DATA0_GPIO)
#define LCM_DATA0_IN()						(JL_PORTA->DIR |= LCM_DATA0_GPIO)
#define LCM_DATA0_GET()						(JL_PORTA->IN & LCM_DATA0_GPIO)
#define LCM_DATA0_HIGH()					(JL_PORTA->OUT |= LCM_DATA0_GPIO)
#define LCM_DATA0_LOW()						(JL_PORTA->OUT &= ~LCM_DATA0_GPIO)

//	LCM_DATA1 PIN--------GPIO PA2(PIN30)
#define LCM_DATA1_GPIO						_B2_
#define LCM_DATA1_INIT()					do{JL_PORTA->PU &= ~LCM_DATA1_GPIO; \
                                            JL_PORTA->PD &= ~LCM_DATA1_GPIO; \
                                            JL_PORTA->HD &= ~LCM_DATA1_GPIO;}while(0)
#define LCM_DATA1_OUT()						(JL_PORTA->DIR &= ~LCM_DATA1_GPIO)
#define LCM_DATA1_IN()						(JL_PORTA->DIR |= LCM_DATA1_GPIO)
#define LCM_DATA1_GET()						(JL_PORTA->IN & LCM_DATA1_GPIO)
#define LCM_DATA1_HIGH()					(JL_PORTA->OUT |= LCM_DATA1_GPIO)
#define LCM_DATA1_LOW()						(JL_PORTA->OUT &= ~LCM_DATA1_GPIO)

//	LCM_DATA2 PIN--------GPIO PA3(PIN29)
#define LCM_DATA2_GPIO						_B3_
#define LCM_DATA2_INIT()					do{JL_PORTA->PU &= ~LCM_DATA2_GPIO; \
                                            JL_PORTA->PD &= ~LCM_DATA2_GPIO; \
                                            JL_PORTA->HD &= ~LCM_DATA2_GPIO;}while(0)
#define LCM_DATA2_OUT()						(JL_PORTA->DIR &= ~LCM_DATA2_GPIO)
#define LCM_DATA2_IN()						(JL_PORTA->DIR |= LCM_DATA2_GPIO)
#define LCM_DATA2_GET()						(JL_PORTA->IN & LCM_DATA2_GPIO)
#define LCM_DATA2_HIGH()					(JL_PORTA->OUT |= LCM_DATA2_GPIO)
#define LCM_DATA2_LOW()						(JL_PORTA->OUT &= ~LCM_DATA2_GPIO)

//	LCM_DATA3 PIN--------GPIO PA4(PIN28)
#define LCM_DATA3_GPIO						_B4_
#define LCM_DATA3_INIT()					do{JL_PORTA->PU &= ~LCM_DATA3_GPIO; \
                                            JL_PORTA->PD &= ~LCM_DATA3_GPIO; \
                                            JL_PORTA->HD &= ~LCM_DATA3_GPIO;}while(0)
#define LCM_DATA3_OUT()						(JL_PORTA->DIR &= ~LCM_DATA3_GPIO)
#define LCM_DATA3_IN()						(JL_PORTA->DIR |= LCM_DATA3_GPIO)
#define LCM_DATA3_GET()						(JL_PORTA->IN & LCM_DATA3_GPIO)
#define LCM_DATA3_HIGH()					(JL_PORTA->OUT |= LCM_DATA3_GPIO)
#define LCM_DATA3_LOW()						(JL_PORTA->OUT &= ~LCM_DATA3_GPIO)

//	LCM_DATA4 PIN--------GPIO PA9(PIN27)
#define LCM_DATA4_GPIO						_B9_
#define LCM_DATA4_INIT()					do{JL_PORTA->PU &= ~LCM_DATA4_GPIO; \
                                            JL_PORTA->PD &= ~LCM_DATA4_GPIO; \
                                            JL_PORTA->HD &= ~LCM_DATA4_GPIO;}while(0)
#define LCM_DATA4_OUT()						(JL_PORTA->DIR &= ~LCM_DATA4_GPIO)
#define LCM_DATA4_IN()						(JL_PORTA->DIR |= LCM_DATA4_GPIO)
#define LCM_DATA4_GET()						(JL_PORTA->IN & LCM_DATA4_GPIO)
#define LCM_DATA4_HIGH()					(JL_PORTA->OUT |= LCM_DATA4_GPIO)
#define LCM_DATA4_LOW()						(JL_PORTA->OUT &= ~LCM_DATA4_GPIO)

//	LCM_DATA5 PIN--------GPIO PA10(PIN26)
#define LCM_DATA5_GPIO						_B10_
#define LCM_DATA5_INIT()					do{JL_PORTA->PU &= ~LCM_DATA5_GPIO; \
                                            JL_PORTA->PD &= ~LCM_DATA5_GPIO; \
                                            JL_PORTA->HD &= ~LCM_DATA5_GPIO;}while(0)
#define LCM_DATA5_OUT()						(JL_PORTA->DIR &= ~LCM_DATA5_GPIO)
#define LCM_DATA5_IN()						(JL_PORTA->DIR |= LCM_DATA5_GPIO)
#define LCM_DATA5_GET()						(JL_PORTA->IN & LCM_DATA5_GPIO)
#define LCM_DATA5_HIGH()					(JL_PORTA->OUT |= LCM_DATA5_GPIO)
#define LCM_DATA5_LOW()						(JL_PORTA->OUT &= ~LCM_DATA5_GPIO)

//	LCM_DATA6 PIN--------GPIO PA11(PIN25)
#define LCM_DATA6_GPIO						_B11_
#define LCM_DATA6_INIT()					do{JL_PORTA->PU &= ~LCM_DATA6_GPIO; \
                                            JL_PORTA->PD &= ~LCM_DATA6_GPIO; \
                                            JL_PORTA->HD &= ~LCM_DATA6_GPIO;}while(0)
#define LCM_DATA6_OUT()						(JL_PORTA->DIR &= ~LCM_DATA6_GPIO)
#define LCM_DATA6_IN()						(JL_PORTA->DIR |= LCM_DATA6_GPIO)
#define LCM_DATA6_GET()						(JL_PORTA->IN & LCM_DATA6_GPIO)
#define LCM_DATA6_HIGH()					(JL_PORTA->OUT |= LCM_DATA6_GPIO)
#define LCM_DATA6_LOW()						(JL_PORTA->OUT &= ~LCM_DATA6_GPIO)

//	LCM_DATA7 PIN--------GPIO PB10(PIN43)
#define LCM_DATA7_GPIO						_B10_
#define LCM_DATA7_INIT()					do{JL_PORTB->PU &= ~LCM_DATA7_GPIO; \
                                            JL_PORTB->PD &= ~LCM_DATA7_GPIO; \
                                            JL_PORTB->HD &= ~LCM_DATA7_GPIO;}while(0)
#define LCM_DATA7_OUT()						(JL_PORTB->DIR &= ~LCM_DATA7_GPIO)
#define LCM_DATA7_IN()						(JL_PORTB->DIR |= LCM_DATA7_GPIO)
#define LCM_DATA7_GET()						(JL_PORTB->IN & LCM_DATA7_GPIO)
#define LCM_DATA7_HIGH()					(JL_PORTB->OUT |= LCM_DATA7_GPIO)
#define LCM_DATA7_LOW()						(JL_PORTB->OUT &= ~LCM_DATA7_GPIO)
#endif

