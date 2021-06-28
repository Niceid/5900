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
#include "AMT630A\AMT630A.h"
#include "audio_drv.h"
#include "fm_band.h"
#include "ui_api.h"
#include "bluetooth/avctp_user.h"



#if defined(_USER_TFT_DISP_)
extern void *malloc_fun(void *ptr, u32 len, char *info);
extern void free_fun(void **ptr);

TFT_DISP_t *TFT_DISP= NULL;//TFT显示方面的数据结构体
TFT_BUFFER_t *UART_POP= NULL;//串口方面的数据结构体


extern void TFT_uart_write(char a);

void TFT_Send_N_Data(uint8*data,uint8 length)
{
	uint8 i;
	uint8 buffer[100];
	uint8 CheckSum = 0;
	uint8 Sendlength = 0;
	
	buffer[0] = 0x55;
	buffer[1] = 0xAA;
	buffer[2] = length+2;
	buffer[3] = 0x51;
	for(i=0;i<length;i++)
	{
		CheckSum += data[i];
		buffer[i+4] = data[i];
	}
	buffer[i+4] = CheckSum+0x51;
	if(buffer[i+4]>0xFF)
	buffer[i+4]= buffer[i+4]&0xFF;
	Sendlength = i+5;
	
	ew_printf("Sendlength:%d,CheckSum:0x%x\n",Sendlength,CheckSum+0x51);
	for(i=0;i<Sendlength;i++)
	{
		ew_printf("data[%d]:0x%x\n",i,buffer[i]);
		TFT_uart_write(buffer[i]);
	}
}	

void tft_disp_init(void)
{

   malloc_stats();
   UART_POP = (TFT_BUFFER_t *)malloc_fun(UART_POP ,sizeof(TFT_BUFFER_t), 0);
   if(UART_POP == NULL)
   {
       printf("-------------UART_POP err\n");
       while(1);
   }
   
   malloc_stats();
   TFT_DISP = (TFT_DISP_t *)malloc_fun(TFT_DISP ,sizeof(TFT_DISP_t), 0);
   if(TFT_DISP == NULL)
   {
       printf("-------------TFT_DISP err\n");
       while(1);
   }
}

void TFTPushReceive(uint8 data)
{
    if(UART_POP == NULL)
    {
        return;
    }
    UART_POP->buffer[UART_POP->write++] = data;
    if(UART_POP->write >= SERVO_BUFFER_SIZE)
        UART_POP->write = 0;
}

 u8 TFTPopReceive(uint8 *data)
{
    if(UART_POP == NULL)
    {
        return FALSE;
    }
    if(UART_POP->read != UART_POP->write)
    {
        *data = UART_POP->buffer[UART_POP->read++];
        if(UART_POP->read >= SERVO_BUFFER_SIZE)
            UART_POP->read = 0;
        return TRUE;
    }
    return FALSE;
}


 void TFT_Pop_Work(void )
 {
     if((UART_POP->receive_data[0]==0x52)&&(UART_POP->receive_data[1]==0x01))
     {
        // printf("aaaaaaaaaaaaaaaa\n");
         TFT_DISP->sys_status=UART_POP->receive_data[2];
         TFT_DISP->media     =UART_POP->receive_data[3];
     }
}
 
#if defined(_USER_TFT_SEND_)
void TFT_Tx_System_Status(void)
{
 	uint8 TFT_buff[3]={0};
	TFT_buff[0]=CMD_SYSTEM_STATUS;	
    if(compare_task_name(IDLE_TASK_NAME))
	{
		TFT_buff[1]= TFT_STATUS__OFF;
		TFT_buff[2]= MEDIA_SRC__IDLE;	
	}
	else
	{	TFT_buff[1]= TFT_STATUS__ON;
		if(compare_task_name(FM_TASK_NAME))
		{
			TFT_buff[2]=MEDIA_SRC__RADIO;
		}
		else if(compare_task_name(MUSIC_USB_NAME))
		{
			if(setup.usb_standby)
			{
				TFT_buff[2]=MEDIA_SRC__USB__LOADING;
			}
			else
			{
				TFT_buff[2]=MEDIA_SRC__USB;
			}
		}
		else if(compare_task_name(MUSIC_CARD_NAME))
		{
			if(setup.sd_standby)
			{
				TFT_buff[2]=MEDIA_SRC__SD__LOADING;
			}
			else
			{
				TFT_buff[2]=MEDIA_SRC__SDCARD;
			}
		}
		else if(compare_task_name(LINEIN_TASK_NAME))
		{
			TFT_buff[2]=MEDIA_SRC__AUX;
		}
		else if(compare_task_name(BT_TASK_NAME))
		{
			if(get_bt_connect_status() == BT_STATUS_PLAYING_MUSIC)
			{
				TFT_buff[2]=MEDIA_SRC__BTMUSIC;
			}
			else
			{
				TFT_buff[2]=MEDIA_SRC__BT;
			}
		}
	}
	TFT_Send_N_Data(TFT_buff,3);
}
 
void TFT_Tx_Audio(void)
{
	uint8 TFT_buff[12]={0};
	TFT_buff[0] =CMD_AUDIO_INFO;//音效相关信息
	TFT_buff[1] = global.aud_mute;//MUTE
	TFT_buff[2] = AUDIO_VOLUME_MAX;//音量最大值
	TFT_buff[3] = (AUDIO_ATTEN_WIDE*2);//前后左右最大值 0~20或者0~14
	TFT_buff[4] = (AUDIO_BASS_MAX*2);//高低音最大值 0~20或者0~14
	TFT_buff[5] = setup.aud_vol;//当前音量值
	TFT_buff[6] = setup.aud_eq;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
	TFT_buff[7] = setup.aud_loud;//0:没有打开LOUD开关  1:打开LOUD开关
	TFT_buff[8] = (setup.aud_treble+AUDIO_TREBLE_MAX);//高音0~+14
	TFT_buff[9] = (setup.aud_bass+AUDIO_BASS_MAX);//低音值 0~+14
	TFT_buff[10] = (setup.aud_bal+AUDIO_ATTEN_WIDE);//左右 0~14或者0~+20
	TFT_buff[11] = (setup.aud_fad+AUDIO_ATTEN_WIDE);//前后0~14或者0-~+20
	TFT_Send_N_Data(TFT_buff,12);
}
void TFT_Tx_Sel(uint8 SelType)
{
	uint8 TFT_buff[4]={0};
	TFT_buff[0] = CMD_SET_SEL;//设置菜单
	TFT_buff[1] = SelType;//设置类型
	TFT_buff[2] = setup.loc_flg;// 0:OFF 1:NO
	TFT_buff[3] = setup.mono_flg;// 0:OFF 1:NO
	TFT_Send_N_Data(TFT_buff,4);
}
void TFT_Tx_Sel_init(void)
{
	uint8 TFT_buff[14]={0};
	TFT_buff[0] =CMD_AUDIO_INFO;//音效相关信息
	TFT_buff[1] = global.aud_mute;//MUTE
	TFT_buff[2] = AUDIO_VOLUME_MAX;//音量最大值
	TFT_buff[3] = (AUDIO_ATTEN_WIDE*2);//前后左右最大值 0~20或者0~14
	TFT_buff[4] = (AUDIO_BASS_MAX*2);//高低音最大值 0~20或者0~14
	TFT_buff[5] = setup.aud_vol;//当前音量值
	TFT_buff[6] = setup.aud_eq;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
	TFT_buff[7] = setup.aud_loud;//0:没有打开LOUD开关  1:打开LOUD开关
	TFT_buff[8] = (setup.aud_treble+AUDIO_TREBLE_MAX);//高音0~+14
	TFT_buff[9] = (setup.aud_bass+AUDIO_BASS_MAX);//低音值 0~+14
	TFT_buff[10] = (setup.aud_bal+AUDIO_ATTEN_WIDE);//左右 0~14或者0~+20
	TFT_buff[11] = (setup.aud_fad+AUDIO_ATTEN_WIDE);//前后0~14或者0-~+20
	TFT_buff[12] = setup.loc_flg;// 0:OFF 1:NO
	TFT_buff[13] = setup.mono_flg;// 0:OFF 1:NO
	TFT_Send_N_Data(TFT_buff,14);
}
void TFT_Tx_Sel1(uint8 SelType)
{
	uint8 TFT_buff[3]={0};
	TFT_buff[0] = CMD_SET_SEL;//设置菜单
	TFT_buff[1] = SelType;//设置类型
	switch(SelType)
	{
		case FUNC_SEL_VOLUME:
			TFT_buff[2] = setup.aud_vol;
			break;
		case FUNC_SEL_BASS:
			TFT_buff[2] = setup.aud_bass;
			break;	
		case FUNC_SEL_TREBLE:
			TFT_buff[2] = setup.aud_treble;
			break;
		case FUNC_SEL_BAL:
			TFT_buff[2] = setup.aud_bal;
			break;
		case FUNC_SEL_VOLUME:
			TFT_buff[2] = setup.loc_flg;
			break;
		case FUNC_SEL_FAD:
			TFT_buff[2] = setup.aud_fad;
			break;
		case FUNC_SEL_LOUD:
			TFT_buff[2] = setup.aud_loud;
			break;
		case FUNC_SEL_EQ:
			TFT_buff[2] = setup.aud_eq;
			break;
		case FUNC_SEL_LOC:					//0x 0b
			TFT_buff[2] = setup.loc_flg;
			break;	
		case FUNC_SEL_ST:
			TFT_buff[2] = setup.mono_flg;	//0x 0c
			break;		
		default:
			break;
	}	
	TFT_Send_N_Data(TFT_buff,3);
}

void TFT_Tx_Dsiplay(uint8 disp_type,uint8 chroma,uint8 bright,uint8 sharp,uint8 contrast)
{
	uint8 TFT_buff[6]={0};
	TFT_buff[0] = CMD_TFT_DSIPLAY_INFO;//音效相关信息
	TFT_buff[1] = disp_type;//类型
	TFT_buff[2] = chroma;//色度
	TFT_buff[3] = bright;//亮度
	TFT_buff[4] = sharp;//对比度
	TFT_buff[5] = contrast;//清晰度
	TFT_Send_N_Data(TFT_buff,6);
}

void TFT_Tx_Radio_Info(uint16 freq)
{
	uint8 TFT_buff[5]={0};
	TFT_buff[0] = CMD_RADIO_FIRQ_DISPLAY;
    TFT_buff[1] = GET_RADIO_BAND;//0:FM1 1:FM2 2:FM3 3:AM1 4:AM2
	TFT_buff[2] =((freq>>8)&0xff);//频率高字节
	TFT_buff[3] =(freq&0xff);//频率低字节
	TFT_buff[4] = GET_BAND_FREQ_INDEX; //0~6
	TFT_Send_N_Data(TFT_buff,5);
}
void TFT_Tx_Radio_Status(void)
{
	uint8 TFT_buff[5]={0};
	TFT_buff[0] = CMD_RADIO_STATUS;
    TFT_buff[1] = setup.loc_flg;// 0:OFF 1:NO
    TFT_buff[2] = setup.mono_flg;// 0:OFF 1:NO
	TFT_buff[3] = GET_RADIO_STO;// 0:OFF 1:NO	
	TFT_buff[4] = GET_RADIO_STATE;// 1:扫台2:SCAN 3:自动搜台4:浏览电台
	TFT_Send_N_Data(TFT_buff,5);
}
void TFT_Tx_BT_Status(uint8 connect_status,uint8 call_status,uint8 sco_status,uint8 music_status)
{	
	uint8 TFT_buff[5]={0};
	TFT_buff[0]= CMD_BT_CONNENCTE_STATUS;
	TFT_buff[1]= connect_status;// 1:未连接2已连接
	TFT_buff[2]= call_status;// 1:空闲2:响铃3:打出电话4:正在通话中
	TFT_buff[3]= sco_status;// 1:声音在主机2:声音在手机
	TFT_buff[4]= music_status;// 0:空闲1:暂停2:播放
	TFT_Send_N_Data(TFT_buff,5);
}
void TFT_Tx_BT_number_Display(uint8 *num,uint8 len,uint8 dial_flag)
{
	uint8 TFT_buff[20]={0};
	uint8 i;
	TFT_buff[0]= CMD_BT_NUMBER_DISPLAY;
	TFT_buff[1]= dial_flag;// 0:没有拨号 1:正在拨号
	TFT_buff[2]= len;//号码的长度
	memcpy(&TFT_buff[3],num,len);//num 传入ASSIC码 1:33 2:34
	for(i=0;i<len;i++)
	{
		 printf("TFT_Tx_BT_Dial_number num:%x\n",TFT_buff[i+3]);
	}
	TFT_Send_N_Data(TFT_buff,len+3);
}
void TFT_Tx_BT_Tel_Time(uint32 tel_time)
{
	uint8 TFT_buff[7]={0};
	TFT_buff[0]= CMD_BT_TEL_TIME_DISPLAY;
	
	TFT_buff[1]= ((tel_time/10)/3600)/10;//当前通话时间时的十位
	TFT_buff[2]= (((tel_time/10)/3600))%10;//当前通话时间时的个位
	TFT_buff[3]= (((tel_time/10)%3600)/60)/10;//当前通话时间分的十位
	TFT_buff[4]= (((tel_time/10)%3600)/60)%10;//当前通话时间分的个位
	TFT_buff[5]= (((tel_time/10)%3600)%60)/10;//当前通话时间秒的十位
	TFT_buff[6]= (((tel_time/10)%3600)%60)%10;//当前通话时间秒的个位
	
	printf("TFT_Tx_BT_Tel_Time:%d\n",tel_time);
	 
	TFT_Send_N_Data(TFT_buff,7);
}


void TFT_Tx_Music_Clock_songe_Info(uint8 device,uint32 total_time,uint32 Curplaytime,uint32 total_trk,uint32 Cur_trk)
{
	uint8 TFT_buff[18]={0};
	TFT_buff[0] =CMD_MUSIC_SONG_TIME_INFO;
	TFT_buff[1] = device;// 1:sd 2:usb  0xff:no device

	TFT_buff[2]= (total_time/3600)/10;//总时间时的十位
	TFT_buff[3]= (total_time/3600)%10;//总时间时的个位
	TFT_buff[4]= ((total_time%3600)/60)/10;//总时间分的个位
	TFT_buff[5]= ((total_time%3600)/60)%10;//总时间分的个位
	TFT_buff[6]= ((total_time%3600)%60)/10;//总时间秒的个位
	TFT_buff[7]= ((total_time%3600)%60)%10;//总时间秒的个位
	
	TFT_buff[8]= (Curplaytime/3600)/10;//当前时间时的十位
	TFT_buff[9]= (Curplaytime/3600)%10;//当前时间时的个位
	TFT_buff[10]= ((Curplaytime%3600)/60)/10;//当前时间分的十位 
	TFT_buff[11]= ((Curplaytime%3600)/60)%10;//当前时间分的个位
	TFT_buff[12]= ((Curplaytime%3600)%60)/10;//当前时间秒的十位
	TFT_buff[13]= ((Curplaytime%3600)%60)%10;//当前时间秒的个位

	TFT_buff[14] =((Cur_trk>>8)&0xff);//当前曲目高位
	TFT_buff[15] =(Cur_trk&0xff);//当前曲目低位		
	
	TFT_buff[16] =((total_trk>>8)&0xff);//总曲目高位
	TFT_buff[17] =(total_trk&0xff);//总曲目低位
	
	//printf("TFT_Tx_Music_Song_Info total_time=%d Curplaytime=%d total_trk=%d Cur_trk=%d \n",total_time,Curplaytime,total_trk,Cur_trk);
	TFT_Send_N_Data(TFT_buff,18);

}
void TFT_Tx_Music_Status(uint8 state,uint8 mode)
{
	uint8 TFT_buff[5]={0};
	TFT_buff[0] =CMD_MUSIC_SONG_STATUS;
	TFT_buff[1] =state;//1:暂停2:播放3:停止4:快进，下一首5:快退，上一首

	if(mode==PLAY_MODE_REPEAT_ALL)
		TFT_buff[2] =REPEAT__ALL;
	else if(mode==PLAY_MODE_REPEAT_FOLDER)
		TFT_buff[2] =REPEAT__DIR;
	else if(mode==PLAY_MODE_REPEAT_ONE)
		TFT_buff[2] =REPEAT__ONE;
	else if(mode==PLAY_MODE_SHUFFLE)
		TFT_buff[3] =RANDOM__ON;
	else if(mode==PLAY_MODE_INTRO)
		TFT_buff[4] =INT__ON;
	TFT_Send_N_Data(TFT_buff,5);
}
void TFT_Tx_ID3_Info_Display(music_play_info_t id3buff)
{
	uint8 TFT_buff[11]={0};
	TFT_buff[0] = CMD_ID3_DISPLAY;
	memcpy(&TFT_buff[1], id3buff.id3_len, sizeof(id3buff.id3_len));
	memcpy(&TFT_buff[6], id3buff.id3_info, sizeof(id3buff.id3_info));
	TFT_Send_N_Data(TFT_buff,11);
}

void TFT_Tx_Clock_Display(uint8 hour,uint8 minute,uint8 state,uint8 select)
{
	uint8 TFT_buff[5]={0};
	TFT_buff[0] = CMD_CLOCK_DISPLAY;
	TFT_buff[1] = hour;		//时钟的小时
	TFT_buff[2] = minute;	//时钟的分钟
	TFT_buff[3] = state;	//时钟显示状态(0:显示1:设置)
	TFT_buff[4] = select;	//时钟 设置选择(0:分钟1:小时)
	TFT_Send_N_Data(TFT_buff,5);
}

void TFT_Tx_Music_Search(uint16 Tune,uint32 total)
{
	uint8 TFT_buff[5]={0};
	TFT_buff[0] = CMD_SEARCH_DISPLAY;
	TFT_buff[1] =((total>>8)&0xff);//总曲目高位
	TFT_buff[2] =(total&0xff);//总曲目低位
	TFT_buff[3] =((Tune>>8)&0xff);//当前选择曲目高位
	TFT_buff[4] =(Tune&0xff);//当前选择曲目低位
	printf("Tune:%d\n",Tune);
	TFT_Send_N_Data(TFT_buff,5);
}

#endif

void TFTDriverServer(void)
{
	u8 readData;
    if(UART_POP == NULL)
    {
        return;
    }
	while(TFTPopReceive(&readData))
	{
		switch(UART_POP->flag)
        {
			case 0x00://帧头     0x55
                UART_POP->flag=0x00;
                UART_POP->inidx=0x00;
                UART_POP->CheckSum =0x00;
                UART_POP->data_Len=0x00;
                if(readData==0x55)
               	{
					UART_POP->flag=0x01;
				}
				break;
			case 0x01://帧头     0xaa
                if(readData == 0xaa)//同步命令
                {
                    UART_POP->flag=0x02;
                }
                else
                {
                    UART_POP->flag=0x00;
                }
				break;
			case 0x02://数据长度
			    if(readData)
			    {
                    UART_POP->data_Len = readData;
                    UART_POP->flag=0x03;
                }
                else
                {
                    UART_POP->flag=0x00;
                }
				break;
			case 0x03://数据
			    if(UART_POP->inidx == UART_POP->data_Len -1)
				{
					if(UART_POP->CheckSum == readData)
					{
                       
                        TFT_Pop_Work();//数据处理
						memset(UART_POP->receive_data, 0, LEN_MAX);//清空接受数据
						UART_POP->flag=0x00;
                    }
                    else
                    {
                         UART_POP->flag=0x00;
                    }
				}
                else
                {
                   
                    if(UART_POP->inidx>=LEN_MAX)//容错机制
                    {
                         UART_POP->flag=0x00;
                    }
                    UART_POP->CheckSum+=readData;
                    UART_POP->receive_data[UART_POP->inidx++]=readData;
                }
				break;
			default:
				UART_POP->flag =0x00;
				break;
		}
	}
}
#endif

