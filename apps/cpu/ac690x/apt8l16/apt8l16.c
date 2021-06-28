#include "includes.h"
#include "apt8l16/apt8l16.h"
#include "setup.h"

#if defined(_SUPPORT_TOUCH_KEY_)

static void Touch_Delay(uint8 time)
{
	uint8 i;
	for(i = 0; i < time; i++)
	{
		asm volatile ("nop");	
	}
}

#define TOUCH_DELAY            		Touch_Delay(70)//40时是210K；60时是140K

void Delay1Ms(uint16 n)
{
    uint16 i = 0;
	while(n--)
	{
	    for(i=0;i<20040;i++)//1.05MS
		{
			asm volatile ("nop");
		}
	}
}

#if 0
void Delay100Us(uint16 n)//100US函数
{
    uint16 i = 0;
	while(n--)
	{
	    for(i=0;i<2100;i++)//2080是88US
		{
			asm volatile ("nop");
		}
	}
}
#endif

void TouchI2cStart(void)
{
	TOUCH_DATA_H();					//SDA=1
	TOUCH_DELAY;
	TOUCH_CLK_H();					//SCL=1
	TOUCH_DELAY;
	TOUCH_DATA_L();					//SDA=0
	TOUCH_DELAY;
	TOUCH_CLK_L();					//SCL=0
	TOUCH_DELAY;
}

void TouchI2cStop(void)
{
	TOUCH_DATA_L();					//SDA=0
	TOUCH_DELAY;
	TOUCH_CLK_L();					//SCL=1
	TOUCH_DELAY;
	TOUCH_DATA_H();					//SDA=1
	TOUCH_DELAY;
	TOUCH_CLK_H();					//SCL=0
	TOUCH_DELAY;
}

void  APTTouchACK(void)
{
	TOUCH_DATA_L();
    TOUCH_DELAY;
    TOUCH_CLK_H();
    TOUCH_DELAY;
    TOUCH_CLK_L();
    TOUCH_DELAY;
}

void  APTTouchNoACK(void)
{
	TOUCH_DATA_H();
    TOUCH_DELAY;
    TOUCH_CLK_H();
    TOUCH_DELAY;
    TOUCH_CLK_L();
    TOUCH_DELAY;
}

void TouchI2cWrite(uint8 Data)
{
	uint8 i;
	
	for(i = 0; i < 8; i++)
	{
		if(Data & 0x80)
			TOUCH_DATA_H();
		else
			TOUCH_DATA_L();
		TOUCH_DELAY;
		TOUCH_CLK_H();
		TOUCH_DELAY;
		TOUCH_CLK_L();
		Data = Data<<1;
 	}
}

uint8 TouchI2cRead(void)
{
	uint8 i;
	uint8 data = 0;
	
	TOUCH_DATA_H();
	for(i = 0; i < 8; i++)
	{
		data = data << 1;
		TOUCH_CLK_H();
		TOUCH_DELAY;
		if(TOUCH_DATA_GET())
		data = data|0x01;
		TOUCH_CLK_L();
		TOUCH_DELAY;
 	}
	TOUCH_DATA_OUT();
	return (data);
}
/*----------------------------------------------------------------------------*/

void Write_Touch_Value(uint8 Reg,uint8 Value)
{
    TouchI2cStart();
    TouchI2cWrite(APT8L16_I2C_WRITE_ADDR);
	APTTouchACK();
    TouchI2cWrite(Reg);
	APTTouchACK();
    TouchI2cWrite(Value);
	APTTouchACK();
    TouchI2cStop();
}

uint8 Read_Touch_Value(uint8 Reg)
{
	uint8 Data = 0;

	TouchI2cStart();
	TouchI2cWrite(APT8L16_I2C_WRITE_ADDR);
	APTTouchACK();
	TouchI2cWrite(Reg);
	
	APTTouchACK();
	TouchI2cStart();
	TouchI2cWrite(APT8L16_I2C_READ_ADDR);
	APTTouchACK();
	Data=TouchI2cRead();
	
	APTTouchNoACK();
	TouchI2cStop();
	return Data;
}


void  APTTouchChipSetInOut(uint8 SetInOut)
{
    uint8 ValueA,ValueB,TimeCnt;
    if(SetInOut==1)
    ValueA=0x5a;
    else
    ValueA=0x00;
    TimeCnt=10;
    do{
        Write_Touch_Value(0x3a,ValueA);
        TOUCH_DELAY;
        ValueB = Read_Touch_Value(0x3a);
        if(ValueB!=ValueA)
        {
           Delay1Ms(20);//20MS
           TimeCnt--;
        }
    }while((ValueB!=ValueA)&&(TimeCnt!=0));
	
	//ew_printf("ValueA:0x%x\n",ValueA);
	//ew_printf("ValueB:0x%x\n",ValueB);
}

void  APTTouchChipRegSet(uint8 RegAdd,uint8 RegValueA)
{
    uint8 RegValueB,TimeCnt;
    TimeCnt=10;
    do{
         Write_Touch_Value(RegAdd,RegValueA);
   	  	 TOUCH_DELAY;
         RegValueB=Read_Touch_Value(RegAdd);
         if(RegValueB!=RegValueA)
          {
   		  	Delay1Ms(10);//10MS
            TimeCnt--;
          }
      }while((RegValueB!=RegValueA)&&(TimeCnt!=0));
}

/*----------------------------------------------------------------------------*/
static uint8 APT8L16Data[16]={0x05,0x04,0x04,0x04,0x04,0x03,0x04,0x05,0x06,0x06,0x02,0x05,0x04,0x05,0x06,0x06};//倒数第6个0x03
static uint8 RegAdd[14]={0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d};
static uint8 RegDat[14]={0x02,0x01,0x20,0x00,0x80,0x00,0x08,0x02,0x02,0x10,0x10,0x04,0x08,0x00};

void touch_key_init(void)
{		
	uint8 i;
	uint8 DataBuffer;
	
	puts("touch_key_init\n");
	
	TOUCH_DATA_OUT();
	TOUCH_CLK_OUT();
    TouchI2cStop();
	
	Delay1Ms(150);
	APTTouchChipSetInOut(1);
	Delay1Ms(10);
	APTTouchChipSetInOut(0);
	Delay1Ms(10);

	APTTouchChipSetInOut(1);

	for(i=0;i<14;)
	{
        Write_Touch_Value(RegAdd[i],RegDat[i]);
        Delay1Ms(1);//1MS
        DataBuffer = Read_Touch_Value(RegAdd[i]);
        if(DataBuffer == RegDat[i])
        {
			i++;
		}
		else
		{
			i = 14;
		}
        Delay1Ms(1);//1MS
	}

	for(i=0;i<16;i++)
	{
	    APTTouchChipRegSet(i,APT8L16Data[i]);
	}
	APTTouchChipSetInOut(0);
	Delay1Ms(1000);//500MS
	setup.touch_init = 1;
	puts("touch_key_init_sucess\n");
}
#endif

