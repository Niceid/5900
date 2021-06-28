#include "sdk_cfg.h"
#include "setup.h"
#include "lcd/tm1628/tm1628.h"
#include "disp.h"

lcd_info_t lcd_info;


void Lcd_Delay(int time)
{
	int i;
	for(i = 0; i < time; i++)
	{
		asm volatile ("nop");	
	}
}

#define LCD_DELAY()				Lcd_Delay(5)


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void lcd_hard_port_init(uint8 enable)
{
	if(enable)
	{
		LCD_CE_INIT();		LCD_CE_OUT();	LCD_CE_HIGH();
		LCD_DATA_INIT();	LCD_DATA_OUT();	LCD_DATA_HIGH();
		LCD_CLK_INIT();		LCD_CLK_OUT();	LCD_CLK_HIGH();
	}
	else
	{
		LCD_CE_INIT();		LCD_CE_IN();
		LCD_CE_INIT();		LCD_DATA_IN();
		LCD_CE_INIT();		LCD_CLK_IN();
	}
}

/*----------------------------------------------------------------------------*/

void LcdWriteByte(uint8 Data)
{
    uint8 i;
	
    for(i=0;i<8;i++)
    {
        if(Data & 0x01)
            LCD_DATA_HIGH();
        else
            LCD_DATA_LOW();
        LCD_CLK_LOW();
		LCD_DELAY();
		Data>>=1;
        LCD_CLK_HIGH();
        LCD_DELAY();
    }
}
void LcdWriteCmd(uint8 Cmd)
{
	LCD_CE_LOW();
    //LCD_DELAY;
	LcdWriteByte(Cmd);
    //LCD_DELAY;
	LCD_CE_HIGH();
	LCD_DATA_HIGH();
}

void lcd_polling(void)
{
	uint8 i;
	uint8 *disp_buf;

	lcd_hard_port_init(TRUE);	
	
#ifdef	TD_ADD_LCD_TEST
	u8 c_byte,c_bit;
	c_byte=gDispPos/8;
	c_bit=gDispPos%8;
	memset(&lcd_info, 0xff, sizeof(lcd_info));
	if(ghalf_sec)
	{
		lcd_info.buf[0][c_byte]&=(~(1<<c_bit));
	}
	disp_buf = lcd_info.buf[0];
	SYS_POW_ON();
#else
	disp_buf = lcd_info.buf[lcd_info.disp];
#endif

	LCD_CLK_HIGH();
    LCD_DATA_HIGH();
	LCD_CE_HIGH();
	
	//��������
    LcdWriteCmd(0x03);    		//������ʾģʽ (7λCOM��10��SEG)
    LcdWriteCmd(0x40);    		//����д�Դ����������(���õ�ַ�Զ���һ)
    //��������
    LCD_CE_LOW();
    LCD_DELAY();
    LcdWriteByte(0xc0);    		//������ʼ��ַ(00H)
    LCD_DELAY();
    for(i=0;i<_LCD_LEVEL_LENGTH_;i++)//14 bytes
    {
		LcdWriteByte(disp_buf[i]);
		//LcdWriteByte(0xFF) ;// ȫ������
    }
	//���ͽ���
    LCD_CE_HIGH();
    LCD_DELAY();
	//��������
	LcdWriteCmd(0x89); //0x8c   		//������ʾ��������()
	LCD_DELAY();
}

/*----------------------------------------------------------------------------*/

void lcd_hard_init(uint8 open_close)
{
	memset(&lcd_info, 0, sizeof(lcd_info));

	lcd_polling();
	
	if(!open_close)
	{
		lcd_hard_port_init(FALSE);
	}
}

