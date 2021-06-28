#include "sdk_cfg.h"
#include "dab/dab.h"
#include "dac/dac_api.h"
#include "key_drv.h"
#include "dac/dac_api.h"
#include "dac/ladc.h"
#include "encode/encode.h"
#include "dac/dac.h"
#include "dac/dac_api.h"
#include "rcsp/rcsp_interface.h"
#include "sys_detect.h"
#include "disp.h"
#include "timer.h"
#include "clock.h"
#include "audio_drv.h"
#include "dac/eq.h"
#include "music.h"
#include "ui_api.h"
#include "sdk_cfg.h"
#include "setup.h"
#include "sys_detect.h"
#include "ew3048\ew3048.h"


#if defined(_SUPPORT_CAN_UART_)
extern void mcu_uart_write(char a);

void Mcu_Send_Data(uint8 *data,uint8 length)
{
	uint8 i;
	uint8 buffer[100];
	uint8 CheckSum = 0;
	uint8 Sendlength = 0;
	
	buffer[0] = 0xFF;
	buffer[1] = 0xAA;
	buffer[2] = length+2;
	buffer[3] = 0x01;
	for(i=0;i<length;i++)
	{
		CheckSum += data[i];
		buffer[i+4] = data[i];
	}
	buffer[i+4] = CheckSum+1;
	Sendlength = i+5;
	
	//ew_printf("Sendlength:%d,CheckSum:0x%x\n",Sendlength,CheckSum);
	for(i=0;i<Sendlength;i++)
	{
		//ew_printf("data[%d]:0x%x\n",i,buffer[i]);
		mcu_uart_write(buffer[i]);
	}
}


#define     SERVO_BUFFER_SIZE		  200
#define     LEN_MAX   				  30

typedef struct{
	uint16 read;
	uint16 write;
	uint8  buffer[SERVO_BUFFER_SIZE];
    uint8  receive_data[LEN_MAX];
	uint8  flag;
	uint16 index;
    uint16 CheckSum;
    uint16 data_Len;
}mcu_buffer_t;

mcu_buffer_t UART_POP;


void McuPushReceive(uint8 data)
{
    UART_POP.buffer[UART_POP.write++] = data;
    if(UART_POP.write >= SERVO_BUFFER_SIZE)
    {
        UART_POP.write = 0;
	}
}

uint8 McuPopReceive(uint8 *data)
{
    if(UART_POP.read != UART_POP.write)
    {
        *data = UART_POP.buffer[UART_POP.read++];
        if(UART_POP.read >= SERVO_BUFFER_SIZE)
        {
            UART_POP.read = 0;
		}
        return TRUE;
    }
    return FALSE;
}

void Mcu_Pop_Work(void )
{
	uint8 temph;
	uint8 templ;
	uint16 temp;

	 ew_printf("readData0:0x%x\n",UART_POP.receive_data[3]);
	 ew_printf("readData1:0x%x\n",UART_POP.receive_data[4]);
	 ew_printf("readData2:0x%x\n",UART_POP.receive_data[5]);
     if((UART_POP.receive_data[3]==0x04)&&(UART_POP.receive_data[4]==0xD2))
     {
		global.can.speed = (UART_POP.receive_data[6])*256 + UART_POP.receive_data[5];
		global.can.total_distance = (UART_POP.receive_data[9])*65536 + (UART_POP.receive_data[8])*256 + UART_POP.receive_data[7];
		global.can.electric = UART_POP.receive_data[10];
		global.can.gears = UART_POP.receive_data[11];
		global.can.err = UART_POP.receive_data[12];
		ew_printf("global.can.speed:%d\n",global.can.speed);
		ew_printf("global.can.total_distance:%d\n",global.can.total_distance);
		ew_printf("global.can.electric:%d\n",global.can.electric);
		ew_printf("global.can.gears:%d\n",global.can.gears);
		ew_printf("global.can.err:%d\n",global.can.err);
     }
}

void McuDriverServer(void)
{
	uint8 readData;
		
	while(McuPopReceive(&readData))
	{
		ew_printf("readData:0x%x\n",readData);
	    switch(UART_POP.flag)
        {
			case 0x00://帧头     0xFF
                UART_POP.flag=0x00;
                UART_POP.index=0x00;
                UART_POP.CheckSum =0x00;
                UART_POP.data_Len=0x00;
                if(readData==0xFF)
               	{
					UART_POP.flag=0x01;
				}
				break;
			case 0x01://帧头     0xAA
                if(readData == 0xAA)//
                {
                    UART_POP.flag=0x02;
                }
                else
                {
                    UART_POP.flag=0x00;
                }
				break;
			case 0x02://数据长度
			    if(readData)
			    {
                    UART_POP.data_Len = readData;
                    UART_POP.flag=0x03;
                }
                else
                {
                    UART_POP.flag=0x00;
                }
				break;
			case 0x03://数据
			    if(UART_POP.index == UART_POP.data_Len -1)
				{
					if(UART_POP.CheckSum == readData)
					{
						//ew_printf("readData:0x%x\n",readData);
                        Mcu_Pop_Work();//数据处理
						memset(UART_POP.receive_data, 0, LEN_MAX);//清空接收数据
						UART_POP.flag=0x00;
                    }
                    else
                    {
                        UART_POP.flag=0x00;
                    }
				}
                else
                {
                    if(UART_POP.index >= LEN_MAX) //出错机制
                    {
                        UART_POP.flag=0x00;
                    }
                    UART_POP.CheckSum+=readData;
                    UART_POP.receive_data[UART_POP.index++]=readData;
                }
				ew_printf("readData[%d]:0x%x\n",UART_POP.index,readData);
				break;
			default:
				UART_POP.flag =0x00;
				break;
		}
	}
}
#endif

