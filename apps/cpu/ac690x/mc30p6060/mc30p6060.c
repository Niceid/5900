#include "includes.h"
#include "mc30p6060/mc30p6060.h"
#include "setup.h"

#if defined(_ENABLE_TIME3_DELAY_)

extern void delay_1ms();

static void delay_nms(u32 n)//在USB和蓝牙音乐时会断音，所以暂时不用这个延时
{
    while (n--) {
        delay_1ms();
    }
}

void LightSendData(uint8 Data)
{
	uint8 i;

	for (i=0; i<8; i++)
	{
		SYS_30P60_LIGHT_L();
		delay_nms(2);
		SYS_30P60_LIGHT_H();
		if (Data &0x01)
		{
			delay_nms(6);
		}
		else
		{
			delay_nms(2);
		}
		Data >>= 1;
	}
}

#if defined(_ENABLE_LIGHT_SHADE_)
void LightDriverIrCmd(uint8 cmd,uint8 red,uint8 green,uint8 blue)
{
	SYS_30P60_LIGHT_H();
	delay_nms(1);
	SYS_30P60_LIGHT_L();
	delay_nms(10);

	LightSendData(cmd);
	LightSendData(red);
	LightSendData(green);
	LightSendData(blue);
	SYS_30P60_LIGHT_L();
	delay_nms(1);
	SYS_30P60_LIGHT_H();
}
#else
void LightDriverIrCmd(uint8 cmd)
{
	SYS_30P60_LIGHT_H();
	delay_nms(1);
	SYS_30P60_LIGHT_L();
	delay_nms(10);

	LightSendData(cmd);
	SYS_30P60_LIGHT_L();
	delay_nms(1);
	SYS_30P60_LIGHT_H();
}
#endif
#endif

#if 0
void send_data_num(void)///模拟一直发送，方便示波器测量波形
{
    u8 ret = 0;
    u8 i = 0;
    u8 data[3] = {0x11,0x22,0x33};
    for(i = 0; i < 3;i = i)
	{
        ret = LightDriverIrCmd(data[i]);
        if(ret){
            i++;
        }
        os_time_dly(1);///得看前后2个字节是否需要连贯
    }
}
#endif

