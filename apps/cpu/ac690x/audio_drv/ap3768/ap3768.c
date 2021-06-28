#include "ap3768/ap3768.h"
#include "setup.h"
#include "vm/vm_api.h"
#include "ui_api.h"

#if defined(_ENABLE_RADIO_OUTPUT_)
#include "tuner_drv.h"
#endif

#if defined(_AUDIO_DRIVER_AP3768_)

#if defined(TD_ADD_NEW_APP)
ap3768_config_t *ap3768_config = NULL;
extern u8 sppbuf[30];
EQ_INFO eq_info;
extern uint8 ap3768_save_info(const ap3768_config_t *addr);
extern u8 crc_checksum(u8 *data,u16 len);
extern void  Notify_data_send(u8 *data,u16 len);
#endif

static ap3768_audio_t ap3768_audio;

extern void audio_uart_write(char a);

void Audio_Send_Data(uint8 *data,uint8 length)
{
	uint8 i;
	uint8 buffer[100];
	uint8 CheckSum = 0;
	uint8 Sendlength = 0;
	
	buffer[0] = 0xFF;
	for(i=0;i<length;i++)
	{
		CheckSum += data[i];
		buffer[i+1] = data[i];
	}
	buffer[i+1] = CheckSum-1;
	Sendlength = i+2;
		
//	ew_printf("Sendlength:%d,CheckSum:0x%x\n",Sendlength,CheckSum);
	for(i=0;i<Sendlength;i++)
	{
//		ew_printf("data[%d]:0x%x\n",i,buffer[i]);
		audio_uart_write(buffer[i]);
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
    uint8  CheckSum;
    uint16 data_Len;
}audio_buffer_t;

audio_buffer_t UART_POP;


void AudioPushReceive(uint8 data)
{
    UART_POP.buffer[UART_POP.write++] = data;
    if(UART_POP.write >= SERVO_BUFFER_SIZE)
    {
        UART_POP.write = 0;
	}
}

uint8 AudioPopReceive(uint8 *data)
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

#if defined(_ENABLE_DSP_RESET_)
static void dsp_reset_ctrl(uint8 on_off)
{
	DSP_RESET_INIT();
	if(on_off)
	{
		DSP_RESET_ON();
	}
	else
	{
		DSP_RESET_OFF();
	}
}
#endif

void Audio_Pop_Work(void )
{
//都是数据位，从实际数据的02开始
#if defined(_AP3768_DETECT_VOL_)
	if(compare_task_name("LineinTask"))
	{
		if(UART_POP.receive_data[0] == 0x02)
		{
			if((UART_POP.receive_data[1] == 0x00)&&(UART_POP.receive_data[2] == 0x00)
			&&(UART_POP.receive_data[2] == 0x00)&&(UART_POP.receive_data[4] == 0x00)
			&&(UART_POP.receive_data[5] == 0x00)&&(UART_POP.receive_data[6] == 0x00)
			&&(UART_POP.receive_data[7] == 0x00))
			{
				if((++global.send_time1) >= 2)
				{
					AUDIO_MUTE_ON();
			    #if defined(_ENABLE_RCA_MUTE_)
			    	RCA_MUTE_ON();
			    #endif
			    #if defined(_DSP_RCA_MUTE_)
			    	//DSP_RCA_MUTE_ON();
			    #endif
					global.send_time1 = 0;
				}
			}
			else
			{
				global.send_time1 = 0;
				AUDIO_MUTE_OFF();
		    #if defined(_ENABLE_RCA_MUTE_)
		    	RCA_MUTE_OFF();
		    #endif
		    #if defined(_DSP_RCA_MUTE_)
		    	//DSP_RCA_MUTE_OFF();
		    #endif
			}
		}
	}
#endif

#if defined(_ENABLE_DSP_RESET_)
	if(global.reset_first)
	{
		if((UART_POP.receive_data[0] == 0x00)&&(UART_POP.receive_data[1] == 0x00)
		&&(UART_POP.receive_data[2] == 0x01)&&(UART_POP.receive_data[3] == 0x02)
		&&(UART_POP.receive_data[4] == 0x00)&&(UART_POP.receive_data[5] == 0x00)
		&&(UART_POP.receive_data[6] == 0x00)&&(UART_POP.receive_data[7] == 0x00))
		{
			global.reset_flag = 0;
			global.reset_first = 0;
		}
		else
		{
			global.reset_flag = 1;
		}
		
		if((global.sys_power)&&(global.reset_flag))
		{
			if((++global.send_time) >= (300/12))//延时1秒
			{
				dsp_reset_ctrl(OFF);
				OSTimeDly(10);
				dsp_reset_ctrl(ON);
				OSTimeDly(10);
				global.reset_flag = 0;
				global.send_time = 0;
			}
		}
	}
#endif

	#if 1
    	/*printf("readData ");
        for(u8 i=0;i<8;i++)
        {
    		printf("0x%x ",UART_POP.receive_data[i]);
    	}
    	printf("\n");*/
	#else
    	 ew_printf("readData0:0x%x\n",UART_POP.receive_data[0]);
    	 ew_printf("readData1:0x%x\n",UART_POP.receive_data[1]);
    	 ew_printf("readData2:0x%x\n",UART_POP.receive_data[2]);
    	 ew_printf("readData3:0x%x\n",UART_POP.receive_data[3]);
    	 ew_printf("readData4:0x%x\n",UART_POP.receive_data[4]);
    	 ew_printf("readData5:0x%x\n",UART_POP.receive_data[5]);
    	 ew_printf("readData6:0x%x\n",UART_POP.receive_data[6]);
    	 ew_printf("readData7:0x%x\n",UART_POP.receive_data[7]);
	#endif
}

void AudioDriverServer(void)
{
	uint8 readData;

#if defined(_AP3768_DETECT_VOL_)
	if(compare_task_name("LineinTask"))
	{
		if((++global.send_time) >= (1000/12))// 1秒发一次
		{
			global.send_time = 0;
			u8 energy_value_send_buf[11]={0x08,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
			Audio_Send_Data(energy_value_send_buf,11);
		}
	}
	else
	{
		global.send_time = 0;
		global.send_time1 = 0;
	}
#endif

	while(AudioPopReceive(&readData))
	{
		//ew_printf("readData11:0x%x\n",readData);
	    switch(UART_POP.flag)
        {
			case 0x00://帧头0xFF
				//ew_puts("case0\n");
                UART_POP.flag=0x00;
                UART_POP.index=0x00;
                UART_POP.CheckSum =0x00;
                UART_POP.data_Len=0x00;
                if(readData==0xFF)
               	{
    				UART_POP.flag=0x01;
    			}
				break;
			case 0x01://数据长度
				//ew_puts("case1\n");
			    if(readData)
			    {
                    UART_POP.data_Len = readData;
                    UART_POP.flag=0x02;
					UART_POP.CheckSum+=readData;
                 }
                 else
                 {
                     UART_POP.flag=0x00;
                 }
				break;
			case 0x02://数据
				//ew_puts("case2\n");
			    if(UART_POP.index == UART_POP.data_Len)//CheckSum是帧头+长度+数据
				{
					if((UART_POP.CheckSum-1) == readData)
					{
						//ew_printf("readData222:0x%x\n",readData);
                        Audio_Pop_Work();//数据处理
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
					//ew_puts("case4\n");
                    if(UART_POP.index >= LEN_MAX) //出错机制
                    {
                        UART_POP.flag=0x00;
                    }
					//ew_printf("readData333:0x%x\n",readData);
                    UART_POP.CheckSum+=readData;
                    UART_POP.receive_data[UART_POP.index++]=readData;
                }
				//ew_printf("readData[%d]:0x%x\n",UART_POP.index,readData);
				break;
			default:
				UART_POP.flag =0x00;
				break;
		}
	}
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
#if defined(_ENABLE_AMP_LIGHT_)
static uint8 ap3768_audio_init;
#endif
static void audio_mute_ctrl(uint8 mute)
{
	AUDIO_MUTE_INIT();
#if defined(_ENABLE_RCA_MUTE_)
	RCA_MUTE_INIT();
#endif
	if(mute)
	{
	#if defined(_MUTE_ON_CLOSE_AMP_)
		if(ap3768_audio_init)
		{
			ap3768_audio_init = 0;
		}
	#if !defined(_DISABLE_AMP_OFF_)
		else
		{
			AMP_CONTROL_OFF();
		}
		OSTimeDly(16);
	#endif
	#endif
		AUDIO_MUTE_ON();
    #if defined(_ENABLE_RCA_MUTE_)
    	if(setup.aud_vol != 0)
    	{
    		RCA_MUTE_ON();
    	}
    #endif
	}
	else
	{
		AUDIO_MUTE_OFF();
    #if defined(_ENABLE_RCA_MUTE_)
    	RCA_MUTE_OFF();
    #endif
    #if defined(_MUTE_ON_CLOSE_AMP_)
	#if !defined(_DISABLE_AMP_OFF_)
    	OSTimeDly(5);
    	AMP_CONTROL_ON();
    #endif
	#endif
	}
}

void audio_reg_init(void)
{
	ew_puts("----audio_reg_init----\n");
	memset(&ap3768_audio, 0, sizeof(ap3768_audio));
		
	ap3768_audio.channel 	= 0;
	ap3768_audio.loud 		= AUDIO_LOUDNESS_OFF;
	ap3768_audio.gate 		= AUDIO_GATE_PLUS_11_25;
	ap3768_audio.volume 	= AUDIO_VOLUME_DEFAULT;
	ap3768_audio.mute_last 	= ap3768_audio.mute = ENABLE;

    u8 eq_init_send_buf1[11]={0x03,0x00,0x14,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00};
    Audio_Send_Data(eq_init_send_buf1,11);
    
    u8 eq_init_send_buf2[11]={0x03,0x00,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00};
    Audio_Send_Data(eq_init_send_buf2,11);

	u8 eq_init_send_buf3[11]={0x01,0x00,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00};
	Audio_Send_Data(eq_init_send_buf3,11);

	audio_mute_ctrl(ENABLE);
}

const static uint32 EQBandGain[49]=                    
{
	0x010078,0x010073,0x01006E,0x010069,0x010064,0x01005F,0x01005A,0x010055,0x010050,0x01004B,0x010046,0x010041,//0~11
	0x01003C,0x010037,0x010032,0x01002D,0x010028,0x010023,0x01001E,0x010019,0x010014,0x01000F,0x01000A,0x010005,//12~23
	0x000000,																																																		//24
	0x000005,0x00000A,0x00000F,0x000014,0x000019,0x00001E,0x000023,0x000028,0x00002D,0x000032,0x000037,0x00003C,//25~36
	0x000041,0x000046,0x00004B,0x000050,0x000055,0x00005A,0x00005F,0x000064,0x000069,0x00006E,0x000073,0x000078,//37~48
};

const static uint32 EQType[EX_DSP_EQ_TYPE_MAX]=                    
{
	1,0,2,3,4,5,6,7,8
};

static const uint16 ex_dsp_freq[EQ_BAND_MAX]=
{
	20,25,32,40,50,				63,80,100,125,160,
	200,250,315,400,500,		630,800,1000,1250,1600,
	2000,2500,3150,4000,5000,	6300,8000,10000,12500,16000,
	20000,
};

static const EXT_EQ_EFFECT ex_eq_effect[EX_EFFECT_EQ_MAX] =
{
//name     					    20	25	32	40	50	63	80	100	125	160	200	250	315	400	500	630	800	1k	1.25k	1.6k		2k	2.5k		3.15k	4k	5k	6.3k		8k	10k	12.5k	16k	20k 
	{EX_EFFECT_EQ_STAND,	 {24,	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 	24, 	24, 24, 	24, 	24, 24, 24, 	24, 24, 24, 24, 24}},
	{EX_EFFECT_EQ_JAZZ, 	 {25,	25, 26, 30, 31, 32, 33, 33, 33, 29, 26, 25, 24, 22, 20, 22, 23, 24, 24, 	25, 	25, 26, 	28, 	26, 26, 28, 	29, 30, 32, 27, 24}},
	{EX_EFFECT_EQ_POP,		 {24,	25, 25, 27, 28, 29, 31, 30, 31, 32, 34, 31, 30, 27, 25, 24, 23, 22, 19, 	20, 	19, 22, 	24, 	26, 29, 29, 	30, 30, 32, 26, 24}},
	{EX_EFFECT_EQ_CLASS,	 {24,	24, 25, 26, 26, 27, 26, 26, 28, 26, 25, 24, 24, 24, 24, 24, 24, 24, 24, 	24, 	24, 24, 	24, 	24, 25, 27, 	29, 30, 32, 26, 24}},
	{EX_EFFECT_EQ_PEOPLE,    {23,   23, 22, 19, 19, 18, 18, 20, 25, 24, 25, 27, 30, 32, 35, 35, 34, 34, 33, 	33, 	36, 34, 	34, 	29, 26, 25, 	23, 20, 17, 21, 24}},
	{EX_EFFECT_EQ_SOFT, 	 {24,	23, 22, 20, 20, 22, 26, 25, 26, 25, 23, 22, 20, 21, 23, 23, 24, 24, 24, 	24, 	23, 22, 	20, 	22, 24, 26, 	28, 27, 28, 25, 24}},
	{EX_EFFECT_EQ_ROCK, 	 {25,	25, 27, 31, 34, 36, 38, 38, 39, 34, 30, 23, 17, 18, 19, 21, 24, 26, 28, 	27, 	26, 27, 	30, 	30, 32, 34, 	38, 38, 40, 31, 26}},
	{EX_EFFECT_EQ_METAL,	 {26,	29, 31, 38, 40, 44, 44, 40, 39, 33, 31, 27, 25, 22, 20, 22, 22, 22, 20, 	22, 	24, 24, 	26, 	27, 30, 33, 	35, 36, 36, 29, 25}},
	{EX_EFFECT_EQ_ELECTRIC,  {24,   24, 24, 24, 34, 34, 34, 24, 30, 30, 30, 24, 24, 24, 18, 18, 18, 18, 18, 	24, 	24, 24, 	24, 	24, 24, 34, 	34, 36, 36, 36, 34}},
};

void ex_dsp_write_eq(uint8 id,uint8 GainIndex)
{
	if(id > 30) return; 
	uint8  arraysum[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint32 Gain = EQBandGain[GainIndex];
	//uint8 Type = EQType[setup.eq_mode];
	uint16 freq = ap3768_audio.eq_mode.freq;
	if(id > 30) return; 	
	arraysum[0] = 0x00;
	arraysum[1] = 0x00;
	arraysum[2] = 0x3C;
	arraysum[3] = id;
	arraysum[4] = 0x01;
	arraysum[5] = (Gain>>16)&0xff;
	arraysum[6] = (Gain>>8)&0xff;
	arraysum[7] = Gain&0xff;
	arraysum[8] = (freq>>8)&0xff;
	arraysum[9] = freq&0xff;
	arraysum[10] = 0x3C;//bandwidth
	
	Audio_Send_Data(arraysum,11);
}

void ex_dsp_write_eqGain(uint8 id,uint8 GainIndex)
{
	if(id > 30) return; 			
	ex_dsp_write_eq(id,GainIndex);
}

void ex_dsp_write_eq_effect(EX_DSP_EQ_EFFECT eq_Effect)
{
	uint8 i;
	uint8 j;
	if(eq_Effect >= EX_EFFECT_EQ_MAX) return;
	for(i = 0; i < EX_EFFECT_EQ_MAX; i++)
	{
		if(eq_Effect == ex_eq_effect[i].name)
			break;
	}
		
	for(j = 0; j < EQ_BAND_MAX;j++)
	{
		ap3768_audio.eq_mode.freq = ex_dsp_freq[j];
		ex_dsp_write_eqGain(j,48-(ex_eq_effect[i].val[j]));
	}
}

static const uint8 ToneTabe1[]=
{
	 0, 0, 0, 0, 0, 0, 0,
	0,
	1,1,1,1,1, 1, 1
};

static const uint8 ToneTabe2[]=
{
	 0, 0, 0, 0, 0, 0, 0,
	0,
	0,0,0,0,0, 0, 0
};

static const uint8 ToneTabe3[]=
{
	0x82, 0x6E, 0x5A, 0x46, 0x32, 0x1E, 0x0A,
	0,
	0x0A,  0x1E, 0x32, 0x46, 0x5A, 0x6E, 0x78
};

/*static const uint8 ToneBass[]=
{
	 0xD2, 0xB4, 0x96, 0x78, 0x5A, 0x3C, 0x1E,
	0,
	0x1E,  0x3C, 0x5A, 0x78, 0x96, 0xB4, 0xD2
};*/

#if defined(_FAD_BAL_FUNTION_)
static const uint8 ToneTabe4[]=
{
	0x10,
	0x0F, 0x0E, 0x0C, 0x0A, 0x09, 0x07, 0x05, 0x03, 0x01, 0x00
};
#else
static const uint8 ToneTabe4[]=
{
	0x00, 0x01, 0x03, 0x05, 0x07, 0x09, 0x0A, 0x0C, 0x0E, 0x0F,
	0x10,
	0x0F, 0x0E, 0x0C, 0x0A, 0x09, 0x07, 0x05, 0x03, 0x01, 0x00
};
#endif

#if (AUDIO_VOLUME_MAX == 40)
#if defined(_AP3768_VOLUME_SET_)
static const uint8 VolumeTab[41]=
{
 	0, 1,  1, 2, 3, 4, 5, 7, 9,11,
	13,15,17,19,21,23,26,29,32,35,
	38,41,44,47,50,53,56,59,62,65,
 	68,71,74,77,80,83,86,89,92,96,100
};

#else
static const uint8 VolumeTab[41]=
{
 	0, 1,  2, 3, 4, 6, 8, 10, 12,14,
	16,18,20,22,24,26,28,30,32,35,
	38,41,44,47,50,53,56,59,62,65,
 	68,71,74,77,80,83,86,89,92,96,100
};

#endif
#endif

#if defined(_ENABLE_RADIO_OUTPUT_)
#if defined(_ENABLE_SI4705_OUTPUT_)
static const uint8 VolumeTab1[41]=
{
 	0, 1,  1, 2, 3, 4, 5, 7, 9,11,
	13,15,17,19,21,23,26,29,32,35,
	38,41,44,47,50,53,56,59,62,65,
 	68,70,72,74,76,78,80,82,84,86,88
};
#else
static const uint8 VolumeTab1[41]=
{
 	0, 1,  1, 2, 3, 4, 5, 7, 9,11,
	13,15,17,19,21,23,26,29,32,35,
	38,41,44,47,50,53,56,59,62,65,
 	68,71,74,77,80,82,84,86,88,90,92
};
#endif
#endif

#if defined(_FY7000H_RADIO_OUTPUT_)
static const uint8 VolumeTab2[41]=
{
 	0, 1,  1, 2, 3, 4, 5, 7, 9,11,
	13,15,17,19,21,23,26,29,32,35,
	38,41,44,47,50,53,56,59,62,65,
 	68,71,74,77,80,83,86,89,92,94,96
};
#endif

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
uint8  volume =0;
uint8  eq_mode1=0;
uint8  loud=0;
int8   bas=0;
int8   tre=0;
#if defined(_FAD_BAL_FUNTION_)
int8  fl=0;
int8  fr=0;
int8  rl=0;
int8  rr=0;
#else
int8  lr=0;
int8  fr=0;
#endif
uint8  ap3768_mute=0;
void audio_reg_set(void)
{
	uint8  mute_flag;
	
	if((ap3768_audio.mute)||(!ap3768_audio.volume))
	{
		mute_flag = ENABLE;
	}
	else 
	{
		mute_flag = DISABLE;
	}
	
	if(ap3768_audio.mute_last != mute_flag)
	{
		if(mute_flag)
		{
			audio_mute_ctrl(TRUE);
			
			OSTimeDly(2);
		}
	}
	
	if(mute_flag)
	{
		u8 mute_send_buf[11]={0x05,0x00,0x00,0x00,0x00,AUDIO_VOLUME_MAX,0x00,0x00,0x00,0x00,0x00};
		Audio_Send_Data(mute_send_buf,11);
		ap3768_mute = TRUE;
	}
	else
    {
    	switch(ap3768_audio.eq)
    	{
    	#if defined(_ENABLE_AP3768_EQ_MODE_)
    		case AUDIO_EQ_OFF:
    			setup.eq_mode = 0;
    			break;
    		case AUDIO_EQ_FLAT:
    			setup.eq_mode = 7;
    			break;
    		case AUDIO_EQ_CLASSIC:
    			setup.eq_mode = 3;
    			break;
    		case AUDIO_EQ_POP:
    			setup.eq_mode = 2;
    			break;
    		case AUDIO_EQ_ROCK:
    			setup.eq_mode = 6;
    			break;
    		case AUDIO_EQ_JAZZ:
    			setup.eq_mode = 1;
    			break;
    		case AUDIO_EQ_PEOPLE:
    			setup.eq_mode = 4;
    			break;
    		case AUDIO_EQ_SOFT:
    			setup.eq_mode = 5;
    			break;
    		case AUDIO_EQ_ELECTRIC:
    			setup.eq_mode = 8;
    			break;
    			
    	#else
    	
    		case AUDIO_EQ_OFF:
    			setup.eq_mode = 0;
    			break;
    		case AUDIO_EQ_FLAT:
    			setup.eq_mode = 2;
    			break;
    		case AUDIO_EQ_CLASSIC:
    			setup.eq_mode = 3;
    			break;
    		case AUDIO_EQ_POP:
    			setup.eq_mode = 4;
    			break;
    		case AUDIO_EQ_ROCK:
    			setup.eq_mode = 5;
    			break;
    		case AUDIO_EQ_JAZZ:
    			setup.eq_mode = 6;
    			break;
    	#endif
    	
    		default:
    			setup.eq_mode = 0;
    			break;
    	}
    
		//printf("eq_mode1:%d\n",eq_mode1);
    	if(eq_mode1 != ap3768_audio.eq)
    	{
    		eq_mode1 = ap3768_audio.eq;
    
    		if(ap3768_audio.eq == AUDIO_EQ_OFF)
    		{
    			ex_dsp_write_eq_effect(0x00);
    		}
		#ifdef	TD_ADD_NEW_APP
            else if(ap3768_audio.eq == AUDIO_EQ_PEOPLE)
            {
            	puts("----uart_send_user_data----\n");
				/*if(setup.acc_ap3768)
				{
					puts("----uart_send_user_data111----\n");
					uart_send_user_data1();
					setup.acc_ap3768 = 0;
				}
				else*/
				{
					puts("----uart_send_user_data222----\n");
					uart_send_user_data();
				}
            }
		#endif
    		else
    		{
    			ex_dsp_write_eq_effect(setup.eq_mode);
    		}
    	}
    
    	  if(ap3768_audio.eq == AUDIO_EQ_OFF)
    	  {
    		  //ew_puts("AUDIO_EQ_OFF666\n");
    		  if(bas != ap3768_audio.bass)
    		  {
    			  //ew_puts("AUDIO_EQ_OFF777\n");
    			  bas = ap3768_audio.bass;
			   //#if defined(_FEIYIN_BASS_SET_)
    			  u8 bas_send_buf1[11]={0x00,0x00,0x3C,0x07,0x01,ToneTabe1[ap3768_audio.bass+7],ToneTabe2[ap3768_audio.bass+7],ToneTabe3[ap3768_audio.bass+7],0x00,0x64,0x78};//100HZ低音
			   	  Audio_Send_Data(bas_send_buf1,11);
			   //#else
				  //u8 bas_send_buf2[11]={0x00,0x00,0x3C,0x08,0x01,ToneTabe1[ap3768_audio.bass+7],ToneTabe2[ap3768_audio.bass+7],ToneTabe3[ap3768_audio.bass+7],0x00,0x7D,0x78};//125HZ低音
			      //Audio_Send_Data(bas_send_buf2,11);
			   //#endif
			   	  //u8 bas_send_buf3[11]={0x00,0x00,0x3C,0x09,0x01,ToneTabe1[ap3768_audio.bass+7],ToneTabe2[ap3768_audio.bass+7],ToneTabe3[ap3768_audio.bass+7],0x00,0x7F,0x78};//127HZ低音
			   	  //Audio_Send_Data(bas_send_buf3,11);
    		  }
    		  
    		  if(tre != ap3768_audio.treble)
    		  {
    			  //ew_puts("AUDIO_EQ_OFF888\n");
    			  tre = ap3768_audio.treble;
    			  u8 tre_send_buf[11]={0x00,0x00,0x3C,0x1B,0x01,ToneTabe1[ap3768_audio.treble+7],ToneTabe2[ap3768_audio.treble+7],ToneTabe3[ap3768_audio.treble+7],0x27,0x10,0x78};//10KHZ高音
    			  Audio_Send_Data(tre_send_buf,11);
    		  }
    	  }
    	  
    	  if((volume != ap3768_audio.volume)||(ap3768_mute))
    	  {
    		  //ew_puts("AUDIO_EQ_OFF999\n");
    		  //ew_printf("ap3768_audio.volume:%d\n",ap3768_audio.volume);
    		  ap3768_mute = FALSE;
    		  volume = ap3768_audio.volume;
    		  //u8 volume_send_buf[11]={0x05,0x00,0x00,ap3768_audio.volume,0x00,AUDIO_VOLUME_MAX,0x00,0x00,0x00,0x00,0x00};
		  #if defined(_AP3768_VOLUME_SET_)
		  	  if(ap3768_audio.volume == 1)//250级
		  	  {
				  u8 volume_send_buf[11]={0x05,0x00,0x00,0x01,0x00,0xFA,0x00,0x00,0x00,0x00,0x00};
				  Audio_Send_Data(volume_send_buf,11);
			  }
			  else//100级
			  {
			  #if defined(_ENABLE_RADIO_OUTPUT_)
    			  if((compare_task_name("FMRadioTask"))&&(global.tuner_drive == TUN_DRV_SI473x))
    			  {
				  #if defined(_ENABLE_SI4705_OUTPUT_)
				  	  u8 volume_send_buf[11]={0x05,0x00,0x00,VolumeTab1[ap3768_audio.volume],0x00,0x64,0x00,0x00,0x00,0x00,0x00};
				  #else
					  u8 volume_send_buf[11]={0x05,0x00,0x00,VolumeTab1[ap3768_audio.volume],0x00,0x64,0x00,0x00,0x00,0x00,0x00};
				  #endif
					  Audio_Send_Data(volume_send_buf,11);
    			  }
			  #if defined(_FY7000H_RADIO_OUTPUT_)
				  else if((compare_task_name("FMRadioTask"))&&(global.tuner_drive == TUN_DRV_QN8035))
				  {
					  u8 volume_send_buf[11]={0x05,0x00,0x00,VolumeTab2[ap3768_audio.volume],0x00,0x64,0x00,0x00,0x00,0x00,0x00};
					  Audio_Send_Data(volume_send_buf,11);
				  }
			  #endif
    			  else
    			  {
					  u8 volume_send_buf[11]={0x05,0x00,0x00,VolumeTab[ap3768_audio.volume],0x00,0x64,0x00,0x00,0x00,0x00,0x00};
					  Audio_Send_Data(volume_send_buf,11);
    			  }
			  #else
				  u8 volume_send_buf[11]={0x05,0x00,0x00,VolumeTab[ap3768_audio.volume],0x00,0x64,0x00,0x00,0x00,0x00,0x00};
				  Audio_Send_Data(volume_send_buf,11);
			  #endif
			  }
		  #else
			  u8 volume_send_buf[11]={0x05,0x00,0x00,VolumeTab[ap3768_audio.volume],0x00,0x64,0x00,0x00,0x00,0x00,0x00};
		  	  Audio_Send_Data(volume_send_buf,11);
		  #endif
    	  }
    
	 #if defined(_FAD_BAL_FUNTION_)
	 	 if(fl != ap3768_audio.fl)
	 	 {
			 fl = ap3768_audio.fl;

			 u8 vol_buf1=0;
			 u8 vol_buf2=0;
			 
			 vol_buf1 =(((fl+50)*0x2000/100)>>8)&0xff;//音量高字节
			 vol_buf2 =((fl+50)*0x2000/100)&0xff;//音量低字节
			 
			 //ew_printf("vol_buf1:0x%x,vol_buf2:0x%x\n",vol_buf1,vol_buf2);
			 u8 fl_send_buf[11]={0x02,0x00,0x10,vol_buf1,vol_buf2,0x00,0x00,0x00,0x00,0x00,0x00};
			 Audio_Send_Data(fl_send_buf,11);
			 
			 ap3768_config->ap3768_memory2[4][0] = (fl+50);
			 	
			 sppbuf[0] =0xff;
			 sppbuf[1] =0xaa;
			 sppbuf[2] =9;//长度
			 sppbuf[3] ='Z';
			 sppbuf[4] ='F';
			 sppbuf[5] =0x5;
			 sppbuf[6] =0x2;
			 sppbuf[7]=4;//通道
			 sppbuf[8]=ap3768_config->ap3768_memory2[4][0];//输出音量
			 sppbuf[9]=ap3768_config->ap3768_memory2[4][1];//延迟
			 sppbuf[10]=ap3768_config->ap3768_memory2[4][2];//反向
			 sppbuf[11] = crc_checksum((u8 *)sppbuf,11);
			 printf("channel-1:%d",sppbuf[9]);
			 if(!global.app_data)
			 {
				 Notify_data_send(sppbuf,12);
			 }
			 if(!global.app_reset)
			 {
				 ap3768_save_info(ap3768_config);
			 }
		 }
	 	 if(fr != ap3768_audio.fr)
	 	 {
			 fr = ap3768_audio.fr;
			 u8 vol_buf3=0;
			 u8 vol_buf4=0;
			 vol_buf3 =(((fr+50)*0x2000/100)>>8)&0xff;//音量高字节
			 vol_buf4 =((fr+50)*0x2000/100)&0xff;//音量低字节
			 u8 fr_send_buf[11]={0x02,0x00,0x20,vol_buf3,vol_buf4,0x00,0x00,0x00,0x00,0x00,0x00};
			 Audio_Send_Data(fr_send_buf,11);

			 ap3768_config->ap3768_memory2[5][0] = (fr+50);
			 	
			 sppbuf[0] =0xff;
			 sppbuf[1] =0xaa;
			 sppbuf[2] =9;//长度
			 sppbuf[3] ='Z';
			 sppbuf[4] ='F';
			 sppbuf[5] =0x5;
			 sppbuf[6] =0x2;
			 sppbuf[7]=5;//通道
			 sppbuf[8]=ap3768_config->ap3768_memory2[5][0];//输出音量
			 sppbuf[9]=ap3768_config->ap3768_memory2[5][1];//延迟
			 sppbuf[10]=ap3768_config->ap3768_memory2[5][2];//反向
			 sppbuf[11] = crc_checksum((u8 *)sppbuf,11);
			 printf("channel-2:%d",sppbuf[9]);
			 if(!global.app_data)
			 {
				 Notify_data_send(sppbuf,12);
			 }
			 if(!global.app_reset)
			 {
				 ap3768_save_info(ap3768_config);
			 }
		 }

	 	 if(rl != ap3768_audio.rl)
	 	 {
			 rl = ap3768_audio.rl;
			 u8 vol_buf5=0;
			 u8 vol_buf6=0;
			 vol_buf5 =(((rl+50)*0x2000/100)>>8)&0xff;//音量高字节
			 vol_buf6 =((rl+50)*0x2000/100)&0xff;//音量低字节
			 u8 rl_send_buf[11]={0x02,0x00,0x04,vol_buf5,vol_buf6,0x00,0x00,0x00,0x00,0x00,0x00};
			 Audio_Send_Data(rl_send_buf,11);

    		 ap3768_config->ap3768_memory2[2][0] = (rl+50);
    			
    		 sppbuf[0] =0xff;
    		 sppbuf[1] =0xaa;
    		 sppbuf[2] =9;//长度
    		 sppbuf[3] ='Z';
    		 sppbuf[4] ='F';
    		 sppbuf[5] =0x5;
    		 sppbuf[6] =0x2;
    		 sppbuf[7]=2;//通道
    		 sppbuf[8]=ap3768_config->ap3768_memory2[2][0];//输出音量
    		 sppbuf[9]=ap3768_config->ap3768_memory2[2][1];//延迟
    		 sppbuf[10]=ap3768_config->ap3768_memory2[2][2];//反向
    		 sppbuf[11] = crc_checksum((u8 *)sppbuf,11);
			 printf("channel-3:%d",sppbuf[9]);
			 if(!global.app_data)
			 {
				 Notify_data_send(sppbuf,12);
			 }
			 if(!global.app_reset)
			 {
				 ap3768_save_info(ap3768_config);
			 }
		 }
	 	 if(rr != ap3768_audio.rr)
	 	 {
			 rr = ap3768_audio.rr;
			 u8 vol_buf7=0;
			 u8 vol_buf8=0;
			 vol_buf7 =(((rr+50)*0x2000/100)>>8)&0xff;//音量高字节
			 vol_buf8 =((rr+50)*0x2000/100)&0xff;//音量低字节
			 u8 rr_send_buf[11]={0x02,0x00,0x08,vol_buf7,vol_buf8,0x00,0x00,0x00,0x00,0x00,0x00};
			 Audio_Send_Data(rr_send_buf,11);

    		 ap3768_config->ap3768_memory2[3][0] = (rr+50);
    			
    		 sppbuf[0] =0xff;
    		 sppbuf[1] =0xaa;
    		 sppbuf[2] =9;//长度
    		 sppbuf[3] ='Z';
    		 sppbuf[4] ='F';
    		 sppbuf[5] =0x5;
    		 sppbuf[6] =0x2;
    		 sppbuf[7]=3;//通道
    		 sppbuf[8]=ap3768_config->ap3768_memory2[3][0];//输出音量
    		 sppbuf[9]=ap3768_config->ap3768_memory2[3][1];//延迟
    		 sppbuf[10]=ap3768_config->ap3768_memory2[3][2];//反向
    		 sppbuf[11] = crc_checksum((u8 *)sppbuf,11);
			 printf("channel-4:%d",sppbuf[9]);
			 if(!global.app_data)
			 {
			 	global.app_data = FALSE;
				 Notify_data_send(sppbuf,12);
			 }
			 if(!global.app_reset)
			 {
				 global.app_reset = FALSE;
				 ap3768_save_info(ap3768_config);
			 }
		 }
	#else
    	  if(lr != ap3768_audio.lr)
    	  {
    		  lr = ap3768_audio.lr;
    		  if(ap3768_audio.lr > 0)
    		  { 
    			  u8 lf_send_buf[11]={0x02,0x00,0x10,ToneTabe4[ap3768_audio.lr+10],0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    			  Audio_Send_Data(lf_send_buf,11);
    			  
    			  u8 lr_send_buf[11]={0x02,0x00,0x04,ToneTabe4[ap3768_audio.lr+10],0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    			  Audio_Send_Data(lr_send_buf,11);
    		  }
    		  else
    		  {
    			  u8 rf_send_buf[11]={0x02,0x00,0x20,ToneTabe4[ap3768_audio.lr+10],0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    			  Audio_Send_Data(rf_send_buf,11);
    
    			  u8 rr_send_buf[11]={0x02,0x00,0x08,ToneTabe4[ap3768_audio.lr+10],0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    			  Audio_Send_Data(rr_send_buf,11);
    		  }
    	  }
    	  
    	  if(fr != ap3768_audio.fr)
    	  {
    		  fr = ap3768_audio.fr;
    		  if(ap3768_audio.fr > 0)
    		  {
    			  u8 lr_send_buf[11]={0x02,0x00,0x04,ToneTabe4[ap3768_audio.fr+10],0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    			  Audio_Send_Data(lr_send_buf,11);
    
    			  u8 rr_send_buf[11]={0x02,0x00,0x08,ToneTabe4[ap3768_audio.fr+10],0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    			  Audio_Send_Data(rr_send_buf,11);
    		  }
    		  else
    		  {
    			  u8 lf_send_buf[11]={0x02,0x00,0x10,ToneTabe4[ap3768_audio.fr+10],0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    			  Audio_Send_Data(lf_send_buf,11);
    
    			  u8 rf_send_buf[11]={0x02,0x00,0x20,ToneTabe4[ap3768_audio.fr+10],0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    			  Audio_Send_Data(rf_send_buf,11);
    		  }
    	  }
    #endif
	
    #if defined(_ENABLE_AP3768_SUBWOOFER_)
    	if(ap3768_audio.subwoofer1)
    	{
    		u8 sub1_send_buf1[11]={0x02,0x00,0x02,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    		Audio_Send_Data(sub1_send_buf1,11);
    	}
    	else
    	{
    		u8 sub1_send_buf2[11]={0x02,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    		Audio_Send_Data(sub1_send_buf2,11);
    	}
    	
    	if(ap3768_audio.subwoofer2)
    	{
    		u8 sub2_send_buf1[11]={0x02,0x00,0x01,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    		Audio_Send_Data(sub2_send_buf1,11);
    	}
    	else
    	{
    		u8 sub2_send_buf2[11]={0x02,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    		Audio_Send_Data(sub2_send_buf2,11);
    	}
    #endif
    }

    if(ap3768_audio.mute_last != mute_flag)
    {
    	if(!mute_flag)
    	{ 		
    		audio_mute_ctrl(FALSE);
    	}
    }
    
    ap3768_audio.mute_last = mute_flag;
}

uint8 audio_reg_ctrl(uint8 ctrl, int para)
{
	switch(ctrl)
	{
		/*!设置静音*/
		case AUD_CTRL_MUTE:
			{
				uint8 mute = (uint8)para;
				ap3768_audio.mute = mute;
			}
			break;
		/*!设置通道*/
		case AUD_CTRL_CHANNEL:
			break;
		/*!设置放大*/
		case AUD_CTRL_GATE:
			{
				uint8 gate = (uint8)para;
				if(gate < AUDIO_GATE_PLUS_MAX)
				{
					ap3768_audio.gate = gate;
				}
			}
			break;
		/*!设置响度*/
		case AUD_CTRL_LOUD:
			{
				uint8 loud = (uint8)para;
				if(loud < AUDIO_LOUDNESS_MAX)
				{
					ap3768_audio.loud = loud;
				}
			}
			break;
		/*!设置音效*/
		case AUD_CTRL_EQ:
			{
				uint8 eq = (uint8)para;
				if(eq < AUDIO_EQ_MAX)
				{
					ap3768_audio.eq = eq;
				}
			}
			break;
		/*!设置音量*/
		case AUD_CTRL_VOLUME:
			{
				uint8 volume = (uint8)para;				
				if(volume > AUDIO_VOLUME_MAX)
				{
					ap3768_audio.volume = AUDIO_VOLUME_MAX;
				}
				else
				{
					ap3768_audio.volume = volume;
				}
			}
			break;
		/*!设置低音*/
		case AUD_CTRL_BASS:
			{
				int8 bass = (int8)para;
				if(bass > AUDIO_BASS_MAX)
				{
					ap3768_audio.bass = AUDIO_BASS_MAX;
				}
				else if(bass < AUDIO_BASS_MIN)
				{
					ap3768_audio.bass = AUDIO_BASS_MIN;
				}
				else
				{
					ap3768_audio.bass = bass;
				}
			}
			break;
		/*!设置高音*/
		case AUD_CTRL_TREBLE:
			{
				int8 treble = (int8)para;
				if(treble > AUDIO_TREBLE_MAX)
				{
					ap3768_audio.treble = AUDIO_TREBLE_MAX;
				}
				else if(treble < AUDIO_TREBLE_MIN)
				{
					ap3768_audio.treble = AUDIO_TREBLE_MIN;
				}
				else
				{
					ap3768_audio.treble = treble;
				}
			}
			break;
	#if defined(_FAD_BAL_FUNTION_)
    	case AUD_CTRL_FL:
    		{
    			int8 fl = (int8)para;
				if(fl > AUDIO_LR_MAX)
				{
					ap3768_audio.fl = AUDIO_LR_MAX;
				}
				else if(fl < AUDIO_LR_MIN)
				{
					ap3768_audio.fl = AUDIO_LR_MIN;
				}
				else
				{
					ap3768_audio.fl = fl;
				}
    		}
    		break;
		case AUD_CTRL_FR:
			{
				int8 fr = (int8)para;
				if(fr > AUDIO_LR_MAX)
				{
					ap3768_audio.fr = AUDIO_LR_MAX;
				}
				else if(fr < AUDIO_LR_MIN)
				{
					ap3768_audio.fr = AUDIO_LR_MIN;
				}
				else
				{
					ap3768_audio.fr = fr;
				}
			}
			break;
		case AUD_CTRL_RL:
			{
				int8 rl = (int8)para;
				if(rl > AUDIO_LR_MAX)
				{
					ap3768_audio.rl = AUDIO_LR_MAX;
				}
				else if(rl < AUDIO_LR_MIN)
				{
					ap3768_audio.rl = AUDIO_LR_MIN;
				}
				else
				{
					ap3768_audio.rl = rl;
				}
			}
			break;
		case AUD_CTRL_RR:
			{
				int8 rr = (int8)para;
				if(rr > AUDIO_LR_MAX)
				{
					ap3768_audio.rr = AUDIO_LR_MAX;
				}
				else if(rr < AUDIO_LR_MIN)
				{
					ap3768_audio.rr = AUDIO_LR_MIN;
				}
				else
				{
					ap3768_audio.rr = rr;
				}
			}
			break;
	#else
		/*!设置左右平衡*/
		case AUD_CTRL_LR:
			{
				int8 lr = (int8)para;
				if(lr > AUDIO_LR_MAX)
				{
					ap3768_audio.lr = AUDIO_LR_MAX;
				}
				else if(lr < AUDIO_LR_MIN)
				{
					ap3768_audio.lr = AUDIO_LR_MIN;
				}
				else
				{
					ap3768_audio.lr = lr;
				}
			}
			break;
		/*!设置前后平衡*/
		case AUD_CTRL_FR:
			{
				int8 fr = (int8)para;
				if(fr > AUDIO_FR_MAX)
				{
					ap3768_audio.fr = AUDIO_FR_MAX;
				}
				else if(fr < AUDIO_FR_MIN)
				{
					ap3768_audio.fr = AUDIO_FR_MIN;
				}
				else
				{
					ap3768_audio.fr = fr;
				}
			}
			break;
	#endif
			
	#if defined(_ENABLE_AP3768_SUBWOOFER_)
		/*!设置重低音*/
		case AUD_CTRL_SUBWOOFER1:
			{
				uint8 subwoofer1 = (uint8)para;
				if(subwoofer1 < AUDIO_SUBWOOFER_MAX)
				{
					ap3768_audio.subwoofer1 = subwoofer1;
				}
			}
			break;

    	case AUD_CTRL_SUBWOOFER2:
    		{
    			uint8 subwoofer2 = (uint8)para;
    			if(subwoofer2 < AUDIO_SUBWOOFER_MAX)
    			{
					ap3768_audio.subwoofer2 = subwoofer2;
    			}
    		}
    		break;
    #endif
	
		/*!得到当前通道*/
		case AUD_GET_CHANNEL:
			*((uint8 *)para) = ap3768_audio.channel;
			return FALSE;
		default:
			return FALSE;
	}
	
	return TRUE;
}

#ifdef	TD_ADD_NEW_APP
extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);

uint32 ap3768_vm_sum(wm_ap3768_t *vm_ap3768)
{
	uint32 back_sum = vm_ap3768->check;
	vm_ap3768->check = 0;
	uint32 sum = 0;
	for(int i = 0; i < sizeof(wm_ap3768_t); i++)
	{
		sum += ((uint8 *)vm_ap3768)[i];
	}
	vm_ap3768->check = back_sum;
	return (~sum)+1;
}
uint8 ap3768_save_info(const ap3768_config_t *addr)
{//保存AP3768信息到内存中
	wm_ap3768_t *wm_ap3768 = (wm_ap3768_t *)malloc(VM_AP3768_LEN);
    memset(wm_ap3768, 0, VM_AP3768_LEN);
	if(wm_ap3768 == NULL)
	{
		printf("ap3768_save_info_error_malloc\n");
        return FALSE;
	}
	wm_ap3768->title = VM_AP3768_TITLE;
	wm_ap3768->ap3768= *addr;
	wm_ap3768->check = ap3768_vm_sum(wm_ap3768);
	
	int length = vm_write_api(VM_AP3768_INFO, wm_ap3768);

	free(wm_ap3768);

	printf("ap3768_save_info_length:%d \r\n", length);
	if(length == VM_AP3768_LEN)
	{
		return TRUE;
	}
	return FALSE;
}

uint8 ap3768_read_info(ap3768_config_t *addr)
{//读AP3768记忆内容
	printf("ap3768_read_info\n");

	wm_ap3768_t *wm_ap3768 = (wm_ap3768_t *)malloc(VM_AP3768_LEN);
    memset(wm_ap3768, 0, VM_AP3768_LEN);
	if(wm_ap3768 == NULL)
	{
		printf("ap3768_read_info_error_malloc\n");
        return FALSE;
	}

	if(vm_read_api(VM_AP3768_INFO, wm_ap3768) != VM_AP3768_LEN)
	{
		free(wm_ap3768);
		printf("ap3768_check_error_read\n");
		return FALSE;
	}

	if(wm_ap3768->title != VM_AP3768_TITLE)
	{
		free(wm_ap3768);
		printf("ap3768_check_error_title\n");
		return FALSE;
	}

	if(ap3768_vm_sum(wm_ap3768) != wm_ap3768->check)
	{
		free(wm_ap3768);
		printf("ap3768_check_error_sum\n");
		return FALSE;
	}

	*addr = wm_ap3768->ap3768;
	free(wm_ap3768);
	
	printf("ap3768_read_info_succeed\n");
	return TRUE;
}


void ap3768_memory_init(void)
{//初始化记忆内存
    if(ap3768_config ==NULL)
    {
		ap3768_config = (ap3768_config_t *)malloc_fun(ap3768_config ,sizeof(ap3768_config_t), 0);
        memset(ap3768_config, 0, sizeof(ap3768_config_t));
		if(ap3768_config == NULL)
		{
			printf("ap3768_config err\n");
	        while(1);
		}
	}

    if(!ap3768_read_info(ap3768_config))
    {
        for(u8 j = 0; j < EQ_BAND_MAX;j++)
        {
            ap3768_config->ap3768_dsp_type[j] =0x1;//默认dsp类型
            ap3768_config->ap3768_memory1[j][0] =24;//默认增益
            ap3768_config->ap3768_memory1[j][1] =120;//默认频宽
        }
		for(u8 j = 0; j < 6;j++)
		{
            ap3768_config->ap3768_memory2[j][0]=50;//输出增益
            ap3768_config->ap3768_memory2[j][1]=0;//输出延迟
            ap3768_config->ap3768_memory2[j][2]=0;//默认正向
		}
        ap3768_save_info(ap3768_config);
		printf("---init_ap3768_memory----\r\n");
    }
	else
	{
		printf("ap3768_have_info\r\n");
	#if 0
        for(u8 j = 0; j < EQ_BAND_MAX;j++)
        {
        	printf("memory1:%d-%d-%d\r\n",ap3768_config->ap3768_dsp_type,ap3768_config->ap3768_memory1[j][0],ap3768_config->ap3768_memory1[j][1]);
        }
		for(u8 j = 0; j < 6;j++)
		{
        	printf("memory2:%d-%d-%d\r\n",ap3768_config->ap3768_memory2[j][0],ap3768_config->ap3768_memory2[j][1],ap3768_config->ap3768_memory2[j][1]);
		}
	#endif
	}
}
void exit_ap3768(void)
{
    printf("ap3768_free\r\n");
    free_fun((void**)&ap3768_config);
}
u8 channel_buf[6]={0x1,0x2,0x4,0x8,0x10,0x20};
void recv_ap3768_delay_and_reverse(u8 *data)
{//data0:输出通道 1:输出音量 2:延迟 3:反向
    u8 send_buf[11];
    u16 vol_buf=0;
	if(data[1] != ap3768_config->ap3768_memory2[data[0]][0])
	{//输出音量不一样
        vol_buf = ap3768_config->ap3768_memory2[data[0]][0]=data[1];//输出音量
    	send_buf[0]=0x02;//输出音量命令
    	send_buf[1]=0x00;//null
    	send_buf[2] =channel_buf[data[0]];//输出通道
        send_buf[3] =((vol_buf*0x2000/100)>>8)&0xff;//音量高字节
    	send_buf[4]=(vol_buf*0x2000/100)&0xff;//音量低字节
        send_buf[5] =0x0;//null
        send_buf[6] =0x0;//null
        send_buf[7] =0x0;//null
        send_buf[8] =0x0;//null
        send_buf[9] =0x0;//null
        send_buf[10] =0x0;//null
		Audio_Send_Data(send_buf,11);
        ap3768_save_info(ap3768_config);

		//printf("vol ");
		//printf(" %x ",send_buf[3]);
		//printf(" %x ",send_buf[4]);
	}
	if(data[2] != ap3768_config->ap3768_memory2[data[0]][1])
	{//延迟不一样
        ap3768_config->ap3768_memory2[data[0]][1]=data[2];//延迟
    	send_buf[0]=0x04;//延迟命令
    	send_buf[1]=0x00;//null
    	send_buf[2]=ap3768_config->ap3768_memory2[0][1];//第一通道延迟
		send_buf[3]=ap3768_config->ap3768_memory2[1][1];//第二通道延迟
		send_buf[4]=ap3768_config->ap3768_memory2[2][1];//第三通道延迟
		send_buf[5]=ap3768_config->ap3768_memory2[3][1];//第四通道延迟
		send_buf[6]=ap3768_config->ap3768_memory2[4][1];//第五通道延迟
		send_buf[7]=ap3768_config->ap3768_memory2[5][1];//第六通道延迟
        send_buf[8] =0x0;//null
        send_buf[9] =0x0;//null
        send_buf[10] =0x0;//null
		Audio_Send_Data(send_buf,11);
        ap3768_save_info(ap3768_config);
	}
	if(data[3] !=  ap3768_config->ap3768_memory2[data[0]][2])
	{//反向不一样
        ap3768_config->ap3768_memory2[data[0]][2]=data[3];//反向
    	send_buf[0]=0x06;//反向命令
    	send_buf[1]=0x00;//null
    	send_buf[2]=ap3768_config->ap3768_memory2[0][2];//第一通道反向
		send_buf[3]=ap3768_config->ap3768_memory2[1][2];//第二通道反向
		send_buf[4]=ap3768_config->ap3768_memory2[2][2];//第三通道反向
		send_buf[5]=ap3768_config->ap3768_memory2[3][2];//第四通道反向
		send_buf[6]=ap3768_config->ap3768_memory2[4][2];//第五通道反向
		send_buf[7]=ap3768_config->ap3768_memory2[5][2];//第六通道反向
        send_buf[8] =0x0;//null
        send_buf[9] =0x0;//null
        send_buf[10] =0x0;//null
		Audio_Send_Data(send_buf,11);
        ap3768_save_info(ap3768_config);
	}
}

void recv_ap3768_freq_and_gain(u8 *data)
{// data0:输出通道 1:0-30频段 2:EQ频点高字节 3:EQ频点低字节 4:dsp类型 5:增益 6:频宽 7:checksum
	if(data[1] > 30)
	{//频段接收错误	
        printf("ble_recv_index_err\r\n");
		return; 
	}
	uint8  arraysum[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    ap3768_config->ap3768_dsp_type[data[1]] =data[4];
    ap3768_config->ap3768_memory1[data[1]][0] =data[5];
    ap3768_config->ap3768_memory1[data[1]][1] =data[6];
	arraysum[0] = 0x00;//cmd 调节EQ
	arraysum[1] = 0x00;//null
	arraysum[2] = 0x3C;//通道
	arraysum[3] = data[1];//频段
	arraysum[4] = data[4];//dsp类型
    arraysum[5] = (EQBandGain[(48-data[5])]>>16)&0xff;//正负
	arraysum[6] = (EQBandGain[(48-data[5])]>>8)&0xff;//EQ增益高字节
	arraysum[7] = (EQBandGain[(48-data[5])]&0xff);//EQ增益低字节
	arraysum[8] = data[2];//频率高字节
	arraysum[9] = data[3];//频率低字节
	arraysum[10] = data[6];//频宽
	
	printf("gain ");
	for(u8 i=0;i<11;i++)
	{
		printf(" %x ",arraysum[i]);
	}
	printf("--\n");
	
	Audio_Send_Data(arraysum,11);
    ap3768_save_info(ap3768_config);
}


void uart_send_user_data(void)
{//发送用户模式数据给app
	uint8  arraysum[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    u8 gain=0;
    u8 bandwidth=0;
    u16 vol_buf=0;
	
	ap3768_read_info(ap3768_config);

    for(u8 j = 0; j < EQ_BAND_MAX;j++)
    {
        gain =ap3768_config->ap3768_memory1[j][0];
        bandwidth = ap3768_config->ap3768_memory1[j][1];

		arraysum[0] = 0x00;//cmd 调节EQ
		arraysum[1] = 0x00;//null
		arraysum[2] = 0x3C;//通道
		arraysum[3] = j;//频段
		arraysum[4] = ap3768_config->ap3768_dsp_type[j];//dsp类型
	    arraysum[5] = (EQBandGain[(48-gain)]>>16)&0xff;//正负
		arraysum[6] = (EQBandGain[(48-gain)]>>8)&0xff;//EQ增益高字节
		arraysum[7] = (EQBandGain[(48-gain)]&0xff);//EQ增益低字节
		arraysum[8] = (ex_dsp_freq[j]>>8)&0xff;//频率高字节
		arraysum[9] = ex_dsp_freq[j]&0xff;//频率低字节
		arraysum[10] = bandwidth;//频宽
		Audio_Send_Data(arraysum,11);
		
		printf("data ");
		for(u8 i=0;i<11;i++)
		{
			printf(" %x ",arraysum[i]);
		}
		printf("--\n");
	}

    for(u8 j = 0; j < 6;j++)
    {
        vol_buf = ap3768_config->ap3768_memory2[j][0];//输出音量
    	arraysum[0]=0x02;//输出音量命令
    	arraysum[1]=0x00;//null
    	arraysum[2] =channel_buf[j];//输出通道
        arraysum[3] =((vol_buf*0x2000/100)>>8)&0xff;//音量高字节
    	arraysum[4]=(vol_buf*0x2000/100)&0xff;//音量低字节
        arraysum[5] =0x0;//null
        arraysum[6] =0x0;//null
        arraysum[7] =0x0;//null
        arraysum[8] =0x0;//null
        arraysum[9] =0x0;//null
        arraysum[10] =0x0;//null
		Audio_Send_Data(arraysum,11);
    }
    
    arraysum[0]=0x04;//延迟命令
    arraysum[1]=0x00;//null
    arraysum[2]=ap3768_config->ap3768_memory2[0][1];//第一通道延迟
    arraysum[3]=ap3768_config->ap3768_memory2[1][1];//第二通道延迟
    arraysum[4]=ap3768_config->ap3768_memory2[2][1];//第三通道延迟
    arraysum[5]=ap3768_config->ap3768_memory2[3][1];//第四通道延迟
    arraysum[6]=ap3768_config->ap3768_memory2[4][1];//第五通道延迟
    arraysum[7]=ap3768_config->ap3768_memory2[5][1];//第六通道延迟
    arraysum[8] =0x0;//null
    arraysum[9] =0x0;//null
    arraysum[10] =0x0;//null
    Audio_Send_Data(arraysum,11);
    
    arraysum[0]=0x06;//反向命令
    arraysum[1]=0x00;//null
    arraysum[2]=ap3768_config->ap3768_memory2[0][2];//第一通道反向
    arraysum[3]=ap3768_config->ap3768_memory2[1][2];//第二通道反向
    arraysum[4]=ap3768_config->ap3768_memory2[2][2];//第三通道反向
    arraysum[5]=ap3768_config->ap3768_memory2[3][2];//第四通道反向
    arraysum[6]=ap3768_config->ap3768_memory2[4][2];//第五通道反向
    arraysum[7]=ap3768_config->ap3768_memory2[5][2];//第六通道反向
    arraysum[8] =0x0;//null
    arraysum[9] =0x0;//null
    arraysum[10] =0x0;//null
    Audio_Send_Data(arraysum,11);
}
void ap3768_info_to_app(void)
{//发送初始化数据给APP
	puts("ap3768_info_to_app\n");
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
	sppbuf[2] =12;//长度
	sppbuf[3] ='Z';
	sppbuf[4] ='F';
	sppbuf[5] =0x5;
	sppbuf[6] =0x1;
	sppbuf[7] =0x3c;
	for(u8 j = 0; j < EQ_BAND_MAX;j++)
	{
		sppbuf[8]=j;//频段
		sppbuf[9]=(ex_dsp_freq[j]>>8)&0xff;//频段对应频率高字节
		sppbuf[10]=ex_dsp_freq[j]&0xff;//频段对应频率低字节
		sppbuf[11]=ap3768_config->ap3768_dsp_type[j];//DSP类型
		sppbuf[12]=ap3768_config->ap3768_memory1[j][0];//eq_info.gain[j];//增益
		sppbuf[13]=ap3768_config->ap3768_memory1[j][1];//eq_info.bandwidth[j];//频宽
		sppbuf[14] = crc_checksum((u8 *)sppbuf,14);
		Notify_data_send(sppbuf,15);

		printf("band:%d type:%d\n",sppbuf[8],sppbuf[11]);
	}
	
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
	sppbuf[2] =9;//长度
	sppbuf[3] ='Z';
	sppbuf[4] ='F';
	sppbuf[5] =0x5;
	sppbuf[6] =0x2;
	for(u8 j = 0; j < 6;j++)
	{
        sppbuf[7]=j;//通道
        sppbuf[8]=ap3768_config->ap3768_memory2[j][0];//输出音量
        sppbuf[9]=ap3768_config->ap3768_memory2[j][1];//延迟
        sppbuf[10]=ap3768_config->ap3768_memory2[j][2];//反向
		sppbuf[11] = crc_checksum((u8 *)sppbuf,11);
		Notify_data_send(sppbuf,12);
	}
}
void ap3768_reset(void)
{
	uint8  arraysum[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    u16 vol_buf=0;
#if defined(_FAD_BAL_FUNTION_)
    ap3768_audio.fl=ap3768_audio.fr=ap3768_audio.rl=ap3768_audio.rr=0;
    fl=fr=rl=rr=0xff;
	
    setup.aud_fl= 0;
    setup.aud_fr= 0;
    setup.aud_rl= 0;
    setup.aud_rr= 0;
#else
    ap3768_audio.lr=ap3768_audio.fr=0;
	lr=fr=0xff;
#endif
#if defined(_ENABLE_AP3768_SUBWOOFER_)
    setup.aud_sub = 1;
    setup.aud_sub1 = 1;
    ap3768_audio.subwoofer1 = 1;
    ap3768_audio.subwoofer2 = 1;
#endif

	global.app_data = TRUE;;
	global.app_reset = TRUE;;
	
	setup.aud_eq = AUDIO_EQ_OFF;
    ui_show_clean();
    audio_reg_ctrl(AUD_CTRL_EQ,setup.aud_eq);
    audio_reg_set();
    for(u8 j = 0; j < EQ_BAND_MAX;j++)
    {
        ap3768_config->ap3768_dsp_type[j] =0x1;//默认dsp类型
        ap3768_config->ap3768_memory1[j][0] =24;//默认增益
        ap3768_config->ap3768_memory1[j][1] =120;//默认频宽
    }
    for(u8 j = 0; j < 6;j++)
    {
        ap3768_config->ap3768_memory2[j][0]=50;//输出增益
        ap3768_config->ap3768_memory2[j][1]=0;//输出延迟
        ap3768_config->ap3768_memory2[j][2]=0;//默认正向
    }
    ap3768_save_info(ap3768_config);
    for(u8 j = 0; j < 6;j++)
    {
        vol_buf = ap3768_config->ap3768_memory2[j][0];//输出音量
    	arraysum[0]=0x02;//输出音量命令
    	arraysum[1]=0x00;//null
    	arraysum[2] =channel_buf[j];//输出通道
        arraysum[3] =((vol_buf*0x2000/100)>>8)&0xff;//音量高字节
    	arraysum[4]=(vol_buf*0x2000/100)&0xff;//音量低字节
        arraysum[5] =0x0;//null
        arraysum[6] =0x0;//null
        arraysum[7] =0x0;//null
        arraysum[8] =0x0;//null
        arraysum[9] =0x0;//null
        arraysum[10] =0x0;//null
		Audio_Send_Data(arraysum,11);
    }
    
    arraysum[0]=0x04;//延迟命令
    arraysum[1]=0x00;//null
    arraysum[2]=ap3768_config->ap3768_memory2[0][1];//第一通道延迟
    arraysum[3]=ap3768_config->ap3768_memory2[1][1];//第二通道延迟
    arraysum[4]=ap3768_config->ap3768_memory2[2][1];//第三通道延迟
    arraysum[5]=ap3768_config->ap3768_memory2[3][1];//第四通道延迟
    arraysum[6]=ap3768_config->ap3768_memory2[4][1];//第五通道延迟
    arraysum[7]=ap3768_config->ap3768_memory2[5][1];//第六通道延迟
    arraysum[8] =0x0;//null
    arraysum[9] =0x0;//null
    arraysum[10] =0x0;//null
    Audio_Send_Data(arraysum,11);
    
    arraysum[0]=0x06;//反向命令
    arraysum[1]=0x00;//null
    arraysum[2]=ap3768_config->ap3768_memory2[0][2];//第一通道反向
    arraysum[3]=ap3768_config->ap3768_memory2[1][2];//第二通道反向
    arraysum[4]=ap3768_config->ap3768_memory2[2][2];//第三通道反向
    arraysum[5]=ap3768_config->ap3768_memory2[3][2];//第四通道反向
    arraysum[6]=ap3768_config->ap3768_memory2[4][2];//第五通道反向
    arraysum[7]=ap3768_config->ap3768_memory2[5][2];//第六通道反向
    arraysum[8] =0x0;//null
    arraysum[9] =0x0;//null
    arraysum[10] =0x0;//null
    Audio_Send_Data(arraysum,11);
}
#endif
#endif

