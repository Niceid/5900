#include "lcd/wp003/wp003.h"
#include "disp.h"

lcd_info_t lcd_info;

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_hard_port_init(uint8 enable)
{
	if(enable)
	{
		JL_CLOCK->CLK_CON1 &= ~(BIT(5)|BIT(4));	 //[5:4]:OSC

	#if defined(_ENABLE_LCD_CON_)
	   	JL_LCDC->CON0 = 0x0004<<4;	//[6:4]:VLCDS 3.0V
	#else
		JL_LCDC->CON0 = 0x0007<<4;	//[6:4]:VLCDS 3.3V
	#endif	
		//JL_LCDC->CON0 = 0x0004<<4;	//[6:4]:VLCDS 3.0V
		//JL_LCDC->CON0 = 0x0002<<4;	//[6:4]:VLCDS 2.8V

		JL_LCDC->CON0 |= BIT(7);	//[7]:32KHZ/64
		JL_LCDC->CON0 |= 0x0001<<12;  //[13:12]:CHGMOD 一直强充电
	#if defined(_CHANGE_lCD_BIAS_)
	    JL_LCDC->CON0 |= 0x0001<<2;  //[3:2]:0x01:1/2 Bias。0x02:1/3。0x03:1/4
	#else
		JL_LCDC->CON0 |= 0x0002<<2;  //[3:2]:0x01:1/2 Bias。0x02:1/3。0x03:1/4
	#endif	
		JL_LCDC->CON0 |= 0x0002<<14;// 设置COM //5个COM
		
		// 设置SEG		
		JL_LCDC->SEG_IOEN0 = 0xFF00;

		JL_LCDC->CON0 |= 0x01 ;  //[0] LCDCEN 使能这个模块
	}
	else
	{
		JL_LCDC->CON0 |= 0x00 ; // 关掉内部模块
	}
}


#define SET_COM_VAL(reg, val)			do{	disp_temp = (reg); 					\
								   		disp_temp &= 0x00FF; 					\
										disp_temp |=(val&0xFF00);				\
										(reg) = disp_temp;						\
									}while(0)
									

#define SET_COM(reg1, val)	do{SET_COM_VAL(reg1, val);}while(0)


#define COM0_SEG(x)		do{ disp_data = _u8tou16_(disp_buf[1], disp_buf[0]); SET_COM(JL_PORTA->DIE,disp_data); }while(0)
#define COM1_SEG(x)		do{ disp_data = _u8tou16_(disp_buf[3], disp_buf[2]); SET_COM(JL_PORTA->HD,disp_data); }while(0)
#define COM2_SEG(x)		do{ disp_data = _u8tou16_(disp_buf[5], disp_buf[4]); SET_COM(JL_PORTA->PD,disp_data); }while(0)
#define COM3_SEG(x)		do{ disp_data = _u8tou16_(disp_buf[7], disp_buf[6]); SET_COM(JL_PORTA->PU,disp_data); }while(0)
#define COM4_SEG(x)		do{ disp_data = _u8tou16_(disp_buf[9], disp_buf[8]); SET_COM(JL_PORTA->OUT,disp_data); }while(0)

void lcd_polling(void)
{
	uint8 *disp_buf = lcd_info.buf[lcd_info.disp];
	uint16 disp_data;
	uint16 disp_temp;

    COM0_SEG(disp_buf);
    COM1_SEG(disp_buf);
    COM2_SEG(disp_buf);
    COM3_SEG(disp_buf);
	COM4_SEG(disp_buf);
}

void lcd_hard_init(uint8 open_close)
{
	memset(&lcd_info, 0, sizeof(lcd_info));
	
	if(open_close)
	{
		lcd_hard_port_init(TRUE);
		lcd_polling();
	}
	else
	{
		lcd_polling();
		lcd_hard_port_init(FALSE);
	}
}

