#include "sdk_cfg.h"
#include "setup.h"
#include "disp.h"
#include "lcd/lcd5_9/lcd5_9.h"
#include "includes.h"

lcd_info_t lcd_info;


/*----------------------------------------------------------------------------*/
uint8 disp_buf[12];
uint16 disp_data=0;
uint16 disp_temp=0;

/*----------------------------------------------------------------------------*/

void lcd_hard_init(uint8 enable)
{
	if(enable)
	{
		JL_CLOCK->CLK_CON1 &= ~(BIT(5)|BIT(4));	 //[5:4]:OSC
		
		//JL_LCDC->CON0 = 0x0007<<4;	//[6:4]:VLCDS 3.3V
		//JL_LCDC->CON0 = 0x0006<<4;	//[6:4]:VLCDS 3.2V
		//JL_LCDC->CON0 = 0x0005<<4;	//[6:4]:VLCDS 3.1V
		//JL_LCDC->CON0 = 0x0004<<4;	//[6:4]:VLCDS 3.0V
		//JL_LCDC->CON0 = 0x0003<<4;	//[6:4]:VLCDS 2.9V
		//JL_LCDC->CON0 = 0x0002<<4;	//[6:4]:VLCDS 2.8V
		//JL_LCDC->CON0 = 0x0001<<4;	//[6:4]:VLCDS 2.7V
		//JL_LCDC->CON0 = 0x0000<<4;	//[6:4]:VLCDS 2.6V
		#if defined(_LCD_LCD_COM5_SEG9_VDD_CHANGE)
		JL_LCDC->CON0 = 0x0007<<4;	//[6:4]:VLCDS 3.1V
		
		JL_LCDC->CON0 |= BIT(7);	//[7]:32KHZ/64
		JL_LCDC->CON0 |= 0x0001<<12;  //[13:12]:CHGMOD 一直强充电
		JL_LCDC->CON0 |= 0x0003<<2;  //[3:2]:0x01:1/2 Bias。0x02:1/3。0x03:1/4
		
		JL_LCDC->CON0 |= 0x00002<<14;// 设置COM //5个COM
		#else
		JL_LCDC->CON0 = 0x0000<<4;	//[6:4]:VLCDS 2.6V
		
		JL_LCDC->CON0 |= BIT(7);	//[7]:32KHZ/64
		JL_LCDC->CON0 |= 0x0001<<12;  //[13:12]:CHGMOD 一直强充电
		JL_LCDC->CON0 |= 0x0002<<2;  //[3:2]:0x01:1/2 Bias。0x02:1/3。0x03:1/4
		
		JL_LCDC->CON0 |= 0x00002<<14;// 设置COM //5个COM
		#endif
		// 设置SEG		
		#if defined(HuiNuo_6651D_Copy)
		JL_LCDC->SEG_IOEN0 = 0xCE1E;
		#else
		JL_LCDC->SEG_IOEN0 = 0xCE1C;
		JL_LCDC->SEG_IOEN1 = 0x01;
		#endif

		JL_LCDC->CON0 |= 0x01 ;  //[0] LCDCEN 使能这个模块
	}
	else
	{
		JL_LCDC->CON0 |= 0x00 ; // 关掉内部模块
	}
}

#if defined(HuiNuo_6651D_Copy)

#define SET_COM_VAL(reg, val)			do{	disp_temp = (reg); 					\
								   		disp_temp &= ~0xCE1E; 					\
										disp_temp |=(val&0xCE1E);				\
										(reg) = disp_temp;						\
									}while(0)
									

#define SET_COM(reg1, val)	do{SET_COM_VAL(reg1, val);}while(0)


#define COM0_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTA->DIE,disp_data); }while(0)
#define COM1_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTA->HD,disp_data); }while(0)
#define COM2_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTA->PD,disp_data); }while(0)
#define COM3_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTA->PU,disp_data); }while(0)
#define COM4_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTA->OUT,disp_data); }while(0)
#define COM5_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTA->DIR,disp_data); }while(0)

#else

#define SET_COM_H(reg, val)			do{	disp_temp = (reg); 						\
								   		disp_temp &= ~0xCE1C; 					\
										disp_temp |=(val&0xCE1C);\
										(reg) = disp_temp;						\
									}while(0)
									
#define SET_COM_L(reg, val)			do{	disp_temp = (reg); 						\
										disp_temp &= ~0x0001;					\
										disp_temp |= (val&0x0001);	\
										(reg) = disp_temp;						\
									}while(0)

#define SET_COM(regl, regh, val)	do{ SET_COM_L(regl, val); SET_COM_H(regh, val);}while(0)


#define COM0_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->DIE,JL_PORTA->DIE,disp_data); }while(0)
#define COM1_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->HD,JL_PORTA->HD,disp_data); }while(0)
#define COM2_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->PD,JL_PORTA->PD,disp_data); }while(0)
#define COM3_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->PU,JL_PORTA->PU,disp_data); }while(0)
#define COM4_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->OUT,JL_PORTA->OUT,disp_data); }while(0)
#define COM5_SEG(x)		do{ disp_data =x; SET_COM(JL_PORTC->DIR,JL_PORTA->DIR,disp_data); }while(0)

#endif


void lcd_polling(void)
{
#if !defined(_SUPPORT_DEBUG_)
    
#ifdef	TD_ADD_LCD_TEST
	u8 c_byte,c_bit;
	c_byte=gDispPos/8;
	c_bit=gDispPos%8;
	memset(&lcd_info, 0XFF, sizeof(lcd_info));
	if(ghalf_sec)
    {
		lcd_info.buf[0][c_byte]&=(~(1<<c_bit));;
	}
    memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[0],12);
	SYS_POW_ON();

#else
	memcpy((u8*)disp_buf,(uint8*)lcd_info.buf[lcd_info.disp],12);
#endif	
	COM0_SEG(((disp_buf[0]<<8)|disp_buf[1]));
	COM1_SEG(((disp_buf[2]<<8)|disp_buf[3]));
    COM2_SEG(((disp_buf[4]<<8)|disp_buf[5]));
    COM3_SEG(((disp_buf[6]<<8)|disp_buf[7]));
    COM4_SEG(((disp_buf[8]<<8)|disp_buf[9]));
    COM5_SEG(((disp_buf[10]<<8)|disp_buf[11]));
#endif
}

