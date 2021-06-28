#include "sdk_cfg.h"
#include "setup.h"
#include "disp.h"
#include "lcd/1602/Gc16265i.h"

lcd_info_t lcd_info;
#define		Slave	0x7c

static const uint8 init_command[11]=
{
	0x7c,
	0x38,//DL=1:8-BIT MODE;N=1:2-LINE MODE;DH=0:5X8 DOT;IS=0:NORMAL INSTRUCTION;
	0x39,//DL=1:8-BIT MODE;N=1:1-LINE MODE;DH=1:5X8 DOT;IS=1:EXTENSION INSTRUCTION;
	0x18,//BS=0:BIAS=1/5;F2,F1,F0=1,0,0:FREQUENCY:183HZ
	0x54,
	0x57,//ion=1:icon display on;bon=1:booster circuit is turned on;contrast set c5,c4=1,0
	0x6a,/*fon=1:internal follower circuit is turn on;v0 generator amplified ratio,
	rab2,rab1,rab0=100:1+rb/ra=2*/
	0x78,//CONTRAST SET,C3,C2,C1,C0=0000,Vop=4.5v
	0x0c,/*d=1:display on;c=0:cursor is turned off;b=0:cursor blink is off*/
	0x01,/*clear display*/
	0x06/*1/d=1:address is increased by 1;s=0:shift is not performed*/
};
static const uint8 da_font1[16]=
{"STANDARD COG LCM"};

static const uint8 da_font2[16]=
{"   TMGC16265I   "};

static const uint8 icom[16]=
{	
	0x1f,0x1f,0x1f,0x1c,0x05,0x1f,0x0f,0x1f,
	0x17,0x00,0x0f,0x1f,0x1f,0x1f,0x1f,0x1f,
};
void st7032s_delay(uint16 time)
{
	while(time)
	{
		asm volatile ("nop");
		time--;
	}
}
void st7032s_start(void)
{
    DOT_CS_LOW();
    DOT_SDA_INIT();
    DOT_SCL_INIT();
    DOT_SDA_OUT();
    DOT_SCL_OUT();
    DOT_SDA_HIGH();
    DOT_SCL_HIGH();
    st7032s_delay(10);
    DOT_SDA_LOW();
    st7032s_delay(10);
    DOT_SCL_LOW();
}
void st7032s_stop(void)
{
    DOT_SDA_LOW();
    st7032s_delay(10);
    DOT_SCL_HIGH();
    st7032s_delay(10);
    DOT_SDA_HIGH();
    st7032s_delay(10);
    DOT_CS_HIGH();
}
void st7032s_write_one_bye(uint8 data)
{
	uint8 i;
    DOT_SDA_OUT();
    st7032s_delay(5);
	for(i=0;i<8;i++)
	{
		if((data&0x80)==0x80)
		{
            DOT_SDA_HIGH();
		}
		else
		{
            DOT_SDA_LOW();
		}
        st7032s_delay(5);
        DOT_SCL_HIGH();
        st7032s_delay(5);
		data=data<<1;
        DOT_SCL_LOW();
	}
    DOT_SDA_IN();
    st7032s_delay(5);
    DOT_SCL_HIGH();
    st7032s_delay(5);
    if(DOT_SDA_GET())
    {
        //      printf("ack err\n");
    }
    else
    {
        //      printf("ack ok\n");
    }
    DOT_SCL_LOW();
    st7032s_delay(5);
}
void st7032s_init(void)
{
	uint8 i;
    DOT_RESET_INIT();
    DOT_RESET_OUT();
    DOT_RESET_LOW();
	st7032s_delay(500);
    DOT_RESET_HIGH();
	st7032s_delay(500);
    DOT_CS_INIT();
    DOT_CS_OUT();
    DOT_CS_HIGH();
    DOT_A0_INIT();
    DOT_A0_OUT();
    DOT_A0_LOW();
	st7032s_start();
	for(i=0;i<11;i++)
	{
		st7032s_write_one_bye(init_command[i]);
	}
	st7032s_stop();
}
void disp_front1(void)
{
	uint8 i;
	st7032s_start();
	st7032s_write_one_bye(Slave);
	st7032s_write_one_bye(0x80);//Command
    st7032s_write_one_bye(0x38);
	st7032s_write_one_bye(0x80);//adress
	st7032s_write_one_bye(0x40);//Command
    st7032s_write_one_bye(0x40);
	for(i=0;i<16;i++)
	{
		st7032s_write_one_bye(0xff);
	}
	st7032s_stop();
	
	st7032s_start();
	st7032s_write_one_bye(Slave);
	st7032s_write_one_bye(0x80);
	st7032s_write_one_bye(0x80);
	st7032s_write_one_bye(0x40);
	for(i=0;i<16;i++)
	{
		st7032s_write_one_bye(0x00);
	}
	st7032s_stop();

	st7032s_start();
	st7032s_write_one_bye(Slave);
	st7032s_write_one_bye(0x80); //Write command start
	st7032s_write_one_bye(0xc0);
	st7032s_write_one_bye(0x40); //write data start
	for(i=0;i<16;i++)
	{
		st7032s_write_one_bye(0x00);
	}
	st7032s_stop();

	st7032s_start();
	st7032s_write_one_bye(Slave);
	st7032s_write_one_bye(0x80);
	st7032s_write_one_bye(0x39);
	st7032s_write_one_bye(0x80);   	   	
	st7032s_write_one_bye(0x40);
	st7032s_write_one_bye(0x40);     	
	for(i=0;i<16;i++)
	{
		st7032s_write_one_bye(0xff);
	}
	st7032s_stop();
}
void lcd_polling(void)
{
    disp_front1();
}
void lcd_hard_init(uint8 open_close)
{
//    if(open_close)
    {
		st7032s_init();
    }
}
