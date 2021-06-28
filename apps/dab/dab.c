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


#define DAB_DEBUG
#ifdef DAB_DEBUG
    #define dab_puts     ew_puts
    #define dab_printf   ew_printf
#else
    #define dab_puts(...)
    #define dab_printf(...)
#endif



#if defined(DAB_MODLE)
#if defined(_FY7000_SPECIAL_DISPLAY_)

static const char *DAB_DISP_PTY_INFO[]=
{
	"     None    ",// 0
	"     News    ",//
	"    Affair   ",//
	" Information ",//
	"    Sports   ",//
	"  Education  ",//
	"    Drama    ",//
	"    Culture  ",//
	"   Science   ",//
	"   Varied    ",//
	"   PopMusic  ",//
	"  RockMusic  ",//
	"  EasyMusic  ",//
	"  LightMusic ",//
	"   Classics  ",//
	" OthersMusic ",//
	"   Weather   ",//
	"    Fiance   ",//
	"    Child    ",//
	"SocialAffair ",//
	"   Religion  ",//
	"   PhoneIn   ",//
	"   Travel    ",//
	"   Leisure   ",//
	"  JazzMusic  ",//
	"CountryMusic ",//
	"NationaMusic ",//
	"OldiesMusic  ",//
	"   FolkMusic ",//
	" Documentary ",//
	"  AlarmTest  ",//
	"    Alarm    "//
};

#elif defined(_FY5517_SPECIAL_DISPLAY_)
static const char *DAB_DISP_PTY_INFO[]=
{
	"   None    ",// 0
	"   News    ",//
	"  Affair   ",//
	"Information",//
	"  Sports   ",//
	" Education ",//
	"   Drama   ",//
	"  Culture  ",//
	"  Science  ",//
	"  Varied   ",//
	" PopMusic  ",//
	" RockMusic ",//
	" EasyMusic ",//
	" LightMusic",//
	" Classics  ",//
	"OthersMusic",//
	"  Weather  ",//
	"  Fiance   ",//
	"   Child   ",//
	"SocialAffair",//
	" Religion  ",//
	"  PhoneIn  ",//
	"  Travel   ",//
	"  Leisure  ",//
	" JazzMusic ",//
	"CountryMusic",//
	"NationaMusic",//
	"OldiesMusic",//
	" FolkMusic ",//
	"Documentary",//
	" AlarmTest ",//
	"   Alarm   "//
};

#else

static const char *DAB_DISP_PTY_INFO[]=
{
	"None    ",// 0
	"News    ",//
	"Affair  ",//
	"Information ",//
	"Sports  ",//
	"Education ",//
	"Drama   ",//
	"Culture ",//
	"Science ",//
	"Varied  ",//
	"PopMusic",//
	"RockMusic ",//
	"EasyMusic ",//
	"LightMusic ",//
	"Classics",//
	"OthersMusic ",//
	"Weather ",//
	"Finance ",//
	"Child   ",//
	"SocialAffair ",//
	"Religion",//
	"PhoneIn ",//
	"Travel  ",//
	"Leisure ",//
	"JazzMusic ",//
	"CountryMusic ",//
	"NationaMusic ",//
	"OldiesMusic ",//
	"FolkMusic ",//
	"Documentary ",//
	"AlarmTest ",//
	"Alarm   "//
};
#endif

extern void *malloc_fun(void *ptr, u32 len, char *info);
extern void free_fun(void **ptr);
#define get_mute_status   is_dac_mute
extern void W2dab_uart_write(char a);
extern void W2dab_get_appoint_radio_info_cmd(u16 num);//
extern void W2dab_set_appoint_radio_play_cmd(u16 num);//

extern void W2dab_get_appoint_radio_dls_cmd(void);//
extern void W2dab_get_appoint_radio_SIGNAL_STRENGTH_cmd(void);//
extern void W2dab_SET__radio_LINK_cmd(u8 data);//

extern void W2dab_CMD_SET_SEEK(void);//
extern void show_clear_icon(uint8 level);
extern void ui_show_string(uint8, const uint8 *);

#if defined(_ENABLE_RADIO_RDS_)
extern setup_variable_t setup;
u8 dab_rds_af_save=0;//保存af上一状态
u8 dab_rds_ta_save=0;//保存ta上一状态
#endif
u16 radio_total=0;//搜索到节目数
u8 gSeach_elec=0;//搜台进度
u16 RADIO_INFO_DISP_SERACH=0;
u8 DAB_ACC =0;//机器acc上电
u8 DAB_VDD =0;//机器B+上电
u8  dab_select_flag=0;//涡轮选台标志
int RADIO_CURRENT_STATION=0;
u8  SET_LINK =0;
u8  RADIO_download_OK=0;
u8  DAB_NOT_SEEK=0;
u8  DAB_DISP_PTY_SIZE;//
u8  DAB_DISP_DLS_POINT=0;//
u8  DAB_DISP_PTY_POINT=0;//
u8  RADIO_SEEK_STATION_OLD=0;
u8  RADIO_SEEK_STATION_COUNT=0;
u8  DAB_DISP_TEPY_OLD=0;
u16 RADIO_DLS_DET_TIME =0;//
u16 RADIO_SIGNAL_DET_TIME =0;//
u16 RADIO_AUTO_PLAY_TIME=0;//
u16 dab_select_TIME=0;
u16 RADIO_INFO_DISP_TIME =0;
u16 RADIO_INFO_DISP_TIME_NOW =0;
u16  RADIO_SEEK_STATION=0;
u8 DAB_DISP_NAME_POINT=0;

#if defined(DAB_NAME_USER_16BYTE)
u8 RADIO_OLD_STATION =0;
#endif

#if defined(UART_Servo_Pop)
#if defined(TD_ADD_W2DAB_UART)

u8 gW2dab_curstatus=0;//模块的工作状态
u8 DAB_DISP_TEPY=0;//dab模式要现实的内容类型
u8 DAB_DISP_DLS[130];//DLS信息最长128字节
u8 DAB_DISP_DLS_SIZE=0;//DLS信息长度
u8 DAB_DISP_PLAYPAUSE=0;//dab播放状态
u8 RADIO_CURRENT_STATION_INFO_OK = 0;//短文件信息接收完成
u8 Yhy_Cmd_Tepy=0;//dab返回数据类型
u16 Yhy_Data_len=0;//dab返回数据长度
#if defined(DAB_NAME_USER_16BYTE)
u8 DAB_DISPINFO1[DAB_MAX_RADIO_NUM][22]__attribute__((aligned(4)));//缓存电台的内存1
#else
u8 DAB_DISPINFO1[DAB_MAX_RADIO_NUM][14]__attribute__((aligned(4)));//缓存电台的内存1
#endif

u8 DAB_DISPINFO_SIGNAL_STRENGTH=0;//信号强度
u8 DAB_DISPINFO_PTY=0;//节目pty类型
u16 DAB_DISPINFO_BIT=0;//节目比特率
u32 DAB_DISPINFO_FREQUENCY=0;//节目频率
int DAB_DISPINFO_SIGNAL_STRENGTH_flag=0;//信号强度计数标志
#endif


#define     SERVO_BUFFER_SIZE		  200
#define     LEN_MAX   				  130
typedef struct{
	u16 read;
	u16 write;
	u8 buffer[SERVO_BUFFER_SIZE];
	u8 flag;
	u16 inidx;
	u8 tepy;
	u16 data_Len;
	u8 Yhx_user_data[LEN_MAX];
	u8 receive_data[LEN_MAX];
}DAB_BUFFER_t;

DAB_BUFFER_t *UART_POP= NULL;

void DABPushReceive(u8 data)
{
	if(UART_POP!=NULL)
	{
    	DAB_BUFFER_t *buffer = UART_POP;

    	buffer->buffer[buffer->write++] = data;
    	if(buffer->write >= SERVO_BUFFER_SIZE)
    		buffer->write = 0;
	}
}

static u8 DABPopReceive(u8 *data)
{
	if(UART_POP!=NULL)
	{
	DAB_BUFFER_t *buffer = UART_POP;

	if(buffer->read != buffer->write)
	{
		*data = buffer->buffer[buffer->read++];
		if(buffer->read >= SERVO_BUFFER_SIZE)
			buffer->read = 0;
		//printf("%x ",data[0]);
		return TRUE;
	}
	}
	return FALSE;
}
void DABDriverServer(void)
{
	u8 index;
	u8 readData;
	if(UART_POP==NULL)
	{
      return;
	}
	while(DABPopReceive(&readData))
	{
		switch(UART_POP->flag)
        {
			#if 1
			case 0x00:
                if(readData==0xaa)
               	{
					UART_POP->flag=0x01;
				}
				UART_POP->data_Len=0;
				UART_POP->tepy=0;
			   	UART_POP->inidx=0;
				break;
			case 0x01://索引
                if((readData ==0x81)//搜索
				 ||(readData ==0x82)//播放
				 ||(readData ==0x8C)//设置link标记
                 ||(readData ==0x85)//停止播放
                 ||(readData ==0xa0)//获取当前工作状态
                 ||(readData ==0xa1)//获取DLS信息
                 ||(readData ==0xa2)//获取调试信息
                 ||(readData ==0xa3)//获取节目信息
                 ||(readData ==0xa4)//获取节目总数
                 ||(readData ==0xa5)//获取当前信号电平
                 ||(readData ==0xa6)//获取指定节目内容
                 ||(readData ==0xa7)//获取指定节目内容(短文件名)
                 ||(readData ==0xB1))//DAB模块发送给主机LINK状态。
                {
					UART_POP->tepy=readData;
                    UART_POP->flag=0x02;
                }
                else
                {
                     UART_POP->flag=0x00;
                }
				break;
			case 0x02://数据长度高八位
                UART_POP->data_Len = readData;
                UART_POP->data_Len = (UART_POP->data_Len<<8);
                UART_POP->flag=0x03;
				break;
			case 0x03://数据长度低八位
                UART_POP->data_Len |=readData;
                 UART_POP->flag=0x04;
				break;
			case 0x04://数据
				UART_POP->receive_data[UART_POP->inidx++]=readData;
				if(UART_POP->inidx > LEN_MAX)
				{
					UART_POP->flag=0x00;
					UART_POP->inidx=0;
					memset(UART_POP->receive_data, 0, LEN_MAX);//清空接受数据
					UART_POP->data_Len=0;
					UART_POP->tepy=0;
				}
				else if(UART_POP->receive_data[UART_POP->inidx-1]== 0xbb)
				{
					if((UART_POP->inidx-1)==UART_POP->data_Len)
					{
						//printf("\n---\n");
						UART_POP->flag=0x00;
						UART_POP->inidx=0;
						Yhy_Data_len=UART_POP->data_Len;
						Yhy_Cmd_Tepy=UART_POP->tepy;
						memcpy(UART_POP->Yhx_user_data,UART_POP->receive_data,UART_POP->data_Len);
						os_taskq_post(MAIN_TASK_NAME,2,MSG_uart_reveice,0x88);
						memset(UART_POP->receive_data, 0, LEN_MAX);//清空接受数据

					}
					else
					{
						UART_POP->flag=0x00;
						UART_POP->inidx=0;
						memset(UART_POP->receive_data, 0, LEN_MAX);//清空接受数据
						UART_POP->data_Len=0;
						UART_POP->tepy=0;
					}
				}

				break;
			default:
				UART_POP->flag =0x00;
				break;
				#endif
		}
	}

}

void Yhx_Dab_Pop_Work(void)
{
	switch(Yhy_Cmd_Tepy)//处理索引对应的数据
 	{
		case 0x81://搜索中返回三个字节  data1:进度  data2|ata3:节目数高低八位
			if((UART_POP->Yhx_user_data[0]>=0)&&(UART_POP->Yhx_user_data[0]<=100))
			{
				if(UART_POP->Yhx_user_data[0])
				{
					gSeach_elec = UART_POP->Yhx_user_data[0];//搜索进度
				}
			}
        	radio_total=UART_POP->Yhx_user_data[1];
       		radio_total <<=8;

	       	radio_total |=UART_POP->Yhx_user_data[2];;//搜索到节目数
	       	if(radio_total>=DAB_MAX_RADIO_NUM_ALL)
	       	{
			 	radio_total = DAB_MAX_RADIO_NUM_ALL;
			}
	       	DAB_DISP_TEPY=dab_disp_serach;
			DAB_NOT_SEEK=1;
			break;
		case 0x82://播放
		    DAB_DISP_PLAYPAUSE=1;//播放中
		    W2dab_SET__radio_LINK_cmd(1);//设置link
			break;
		case 0x8c://设置link标记
				//DAB_DISP_TEPY=10;
			break;
		case 0x85://停止播放
			    DAB_DISP_PLAYPAUSE=0;//暂停
			break;
		case 0xa0://获取当前工作状态 0:busy 1:ready 2:playing 3:stop
               gW2dab_curstatus=UART_POP->Yhx_user_data[0];
			break;
		case 0xa1://获取DLS信息
			if(Yhy_Data_len)
			{
				DAB_DISP_DLS_SIZE=Yhy_Data_len;
				printf("---DLS=%d---\n",DAB_DISP_DLS_SIZE);
				memcpy(DAB_DISP_DLS,&UART_POP->Yhx_user_data[0],DAB_DISP_DLS_SIZE);
			}
			break;
		case 0xa2://获取调试信息
			break;
		case 0xa3://获取节目信息
			break;
		case 0xa4://DAB节目总数  两个字节
            radio_total=UART_POP->Yhx_user_data[0];
           	radio_total <<=8;
          	radio_total |=UART_POP->Yhx_user_data[1];
			if(radio_total>=DAB_MAX_RADIO_NUM_ALL)
           	{
				radio_total = DAB_MAX_RADIO_NUM_ALL;
			}
			break;
		case 0xa5://获取当前信号电平
			DAB_DISPINFO_SIGNAL_STRENGTH=UART_POP->Yhx_user_data[0];
			if(DAB_DISPINFO_SIGNAL_STRENGTH<=0)
			{
				DAB_DISPINFO_SIGNAL_STRENGTH_flag++;
			}
			else
			{
				DAB_DISPINFO_SIGNAL_STRENGTH_flag=0;
			}
			if(DAB_DISPINFO_SIGNAL_STRENGTH_flag>=5)
			{
				DAB_DISPINFO_SIGNAL_STRENGTH_flag=5;
			}
			else if(DAB_DISPINFO_SIGNAL_STRENGTH_flag<=0)
			{
			    DAB_DISPINFO_SIGNAL_STRENGTH_flag=0;
			}
			break;
	#if defined(DAB_NAME_USER_16BYTE)
		case 0xa6://获取指定节目内容(16字节文件名)
			memcpy(&DAB_DISPINFO1[RADIO_SEEK_STATION],&UART_POP->Yhx_user_data[2],22);
	#else
		case 0xa7://获取指定节目内容(8字节短文件名)
			memcpy(&DAB_DISPINFO1[RADIO_SEEK_STATION],&UART_POP->Yhx_user_data[2],14);
	#endif
			if(RADIO_CURRENT_STATION_INFO_OK==1)
			{
				if(RADIO_SEEK_STATION==(radio_total-1))
				{
					RADIO_download_OK=1;
					RADIO_CURRENT_STATION=0;
					#if defined(DAB_CURRENT_STATION_MEMORY)
					if(setup.DAB_CURRENT_STATION!=0)
					{
						if(setup.DAB_CURRENT_STATION <= (radio_total-1))
						{
							RADIO_CURRENT_STATION=setup.DAB_CURRENT_STATION;
						}
					}
					#endif

					RADIO_INFO_DISP_TIME=0;
					DAB_DISP_TEPY=dab_disp_total;
					RADIO_CURRENT_STATION_INFO_OK=0;
				}
				else if(RADIO_SEEK_STATION<(radio_total-1))
				{
					RADIO_SEEK_STATION++;
					W2dab_get_appoint_radio_info_cmd(RADIO_SEEK_STATION+1);//
				}
			}

			break;
		case 0xb1://DAB模块发送给主机LINK状态。
		 	if(UART_POP->Yhx_user_data[0]==1)//link中
		     {
				printf("-----link ing-----\n");
				// DAB_DISP_TEPY=dab_disp_linking;
			 }
			else if(UART_POP->Yhx_user_data[0]==2)//link完成
			{
				printf("-----link ok-----\n");
				DAB_DISP_TEPY=dab_disp_name;
			#if defined(DAB_NAME_USER_16BYTE)
				DAB_DISP_NAME_POINT=0;
				RADIO_INFO_DISP_TIME_NOW=0;
			#endif
			}
			break;
		default:
			break;
	}

}

void W2dab_Send_N_Data(u8 *data,u8 length)
{
	for(u8 i=0;i<length;i++)
	{
		W2dab_uart_write(data[i]);
	}
}
#endif


#if defined(DAB_MODLE)
//#define DAB_DEBUG_MODLE   //DAB调试模式

#if defined(DAB_DEBUG_MODLE)
#define  CMD_SET_SEEK      			  0x01//搜索
#define  CMD_SET_PLAY      			  0x82//播放(0x82 debug command)
#define  CMD_SET_LINK                 0x0c//设置link标记
#define  CMD_SET_STOP                 0x05//停止播放
#define  CMD_GET_RADIO_STATUS         0x20//获取当前工作状态
#define  CMD_GET_DLS                  0xa1//获取DLS信息(0xa1 debug command)
#define  CMD_GET_DEBUG_INFO           0x22//获取调试信息
#define  CMD_GET_ALL_RADIOINFO        0xa3//获取所有节目信息(0xa3 debug command)
#define  CMD_GET_TOTAL                0xa4//获取节目总数(0xa4 debug command)
#define  CMD_GET_SIGNAL_STRENGTH      0x25//获取当前信号电平
#define  CMD_GET_APPOINT_RADIOINFO    0xa6//获取指定节目内容(a6 is debug)
#define  CMD_GET_APPOINT_RADIOINFO_NAME    0xa7//获取指定节目内容（短文件名）(a7 is debug)

#else
#define  CMD_SET_SEEK      			  0x01//搜索
#define  CMD_SET_PLAY      			  0x02//播放(0x82 debug command)
#define  CMD_SET_LINK                 0x0c//设置link标记
#define  CMD_SET_STOP                 0x05//停止播放
#define  CMD_GET_RADIO_STATUS         0x20//获取当前工作状态
#define  CMD_GET_DLS                  0x21//获取DLS信息(0xa1 debug command)
#define  CMD_GET_DEBUG_INFO           0x22//获取调试信息
#define  CMD_GET_ALL_RADIOINFO        0x23//获取所有节目信息(0xa3 debug command)
#define  CMD_GET_TOTAL                0x24//获取节目总数(0xa4 debug command)
#define  CMD_GET_SIGNAL_STRENGTH      0x25//获取当前信号电平
#define  CMD_GET_APPOINT_RADIOINFO    0x26//获取指定节目内容(a6 is debug)
#define  CMD_GET_APPOINT_RADIOINFO_NAME    0x27//获取指定节目内容（短文件名）(a7 is debug)
#endif

void W2dab_get_radio_total_cmd(void)//获取节目总数
{
    u8 mcu_send_buf[5]={0xaa,CMD_GET_TOTAL,0x00,0x00,0xbb};
    gSeach_elec=0;
    radio_total =0;
    W2dab_Send_N_Data(mcu_send_buf,5);
}

void W2dab_CMD_SET_SEEK(void)//搜索
{
    u8 mcu_send_buf[5]={0xaa,CMD_SET_SEEK,0x00,0x00,0xbb};
    gSeach_elec=0;
    radio_total =0;
    W2dab_Send_N_Data(mcu_send_buf,5);
}

void W2dab_get_appoint_radio_info_cmd(u16 num)//获取指定节目信息(短文件名)
{
#if defined(DAB_NAME_USER_16BYTE)
    u8 mcu_send_buf[5]={0xaa,CMD_GET_APPOINT_RADIOINFO,0x00,0x00,0xbb};
#else
    u8 mcu_send_buf[5]={0xaa,CMD_GET_APPOINT_RADIOINFO_NAME,0x00,0x00,0xbb};
#endif

	mcu_send_buf[2] = num>>8;
	mcu_send_buf[3] = num&0xff;
    W2dab_Send_N_Data(mcu_send_buf,5);
}

void W2dab_get_appoint_radio_dls_cmd(void)//获取DLS信息
{
    u8 mcu_send_buf[5]={0xaa,CMD_GET_DLS,0x00,0x00,0xbb};
    W2dab_Send_N_Data(mcu_send_buf,5);
}


void W2dab_get_appoint_radio_SIGNAL_STRENGTH_cmd(void)//获取信号强度
{
    u8 mcu_send_buf[5]={0xaa,CMD_GET_SIGNAL_STRENGTH,0x00,0x00,0xbb};
    W2dab_Send_N_Data(mcu_send_buf,5);
}


void W2dab_SET__radio_LINK_cmd(u8 data)//设置link
{
    u8 mcu_send_buf[5]={0xaa,CMD_SET_LINK,0x00,0x00,0xbb};
	mcu_send_buf[2] = data>>8;
	mcu_send_buf[3] = data&0xff;
    W2dab_Send_N_Data(mcu_send_buf,5);
}


void W2dab_set_appoint_radio_play_cmd(u16 num)//播放
{
    u8 mcu_send_buf[5]={0xaa,CMD_SET_PLAY,0x00,0x00,0xbb};
	mcu_send_buf[2] = num>>8;
	mcu_send_buf[3] = num&0xff;
    W2dab_Send_N_Data(mcu_send_buf,5);
}
void W2dab_set_appoint_radio_psuse_cmd(u16 num)//停止
{
    u8 mcu_send_buf[5]={0xaa,CMD_SET_STOP,0x00,0x00,0xbb};
	mcu_send_buf[2] = num>>8;
	mcu_send_buf[3] = num&0xff;
    W2dab_Send_N_Data(mcu_send_buf,5);
}

void W2dab_get_radio_info_cmd(void)//获取所有节目信息
{
    u8 mcu_send_buf[5]={0xaa,CMD_GET_ALL_RADIOINFO,0x00,0x00,0xbb};
    W2dab_Send_N_Data(mcu_send_buf,5);
}

#endif

void tuner_Yhx_w2dab_exit(void)
{
	gW2dab_curstatus=0;
	RADIO_AUTO_PLAY_TIME=0;//清除电台自动播放
	DAB_DISP_DLS_SIZE=0;//清除dls缓存信息长度
	DAB_DISP_PLAYPAUSE=0;//清除播放标志
	DAB_NOT_SEEK=0;
	DAB_DISP_TEPY=dab_disp_null;
	W2dab_set_appoint_radio_psuse_cmd((RADIO_CURRENT_STATION+1));//停止当前电台
	dab_puts("Yhx_w2dab_exit_ok\r\n");
}

uint8 tuner_Yhx_w2dab_detect(void)//获取当前工作状态
{
    u8 cnt=0;
    u8 mcu_send_buf[5]={0xaa,CMD_GET_RADIO_STATUS,0x00,0x00,0xbb};//获取当前工作状态
    while((!gW2dab_curstatus)&&(cnt<7))
    {
	#if defined(_CHANGE_MODE_1_)
		if(global.dab_detect_flag)
		{
			global.dab_detect_flag = FALSE;
		}
		else
	#endif
		{
        	W2dab_Send_N_Data(mcu_send_buf,5);
		}
	#if defined(DAB_ACC_ON_ERROR_)
		if(DAB_ACC)
		{
			os_time_dly(50);
		}
		else
	#endif
		{
		#if defined(_DAB_POWER_OFF_ERROR_)
	        os_time_dly(50);
		#else
			os_time_dly(10);
		#endif
		}
    	cnt++;
		//dab_printf("cnt=%d\r\n",cnt);
        //dab_printf("gW2dab_curstatus11111=%d\r\n",gW2dab_curstatus);
	}
    if(gW2dab_curstatus !=0)
    {
        dab_printf("w2dab_det_ok=%d\r\n",gW2dab_curstatus);
		return TRUE;
    }
    else
    {
		dab_puts("w2dab_det_NO\r\n");
		return FALSE;
	}
}

#if defined(bole_100_err_set)
u8 dab_sys_power_flag=0;
#endif


uint8 tuner_Yhx_w2dab_init(void)
{
    u8 mcu_send_buf[5]={0xaa,CMD_GET_TOTAL,0x00,0x00,0xbb};//获取节目总数

#if defined( bole_100_err_set)
    //if((dab_sys_power_flag == 1)||(DAB_ACC==1))//给dab断电后上电都要重新搜台
    {
        DAB_ACC=0;
        dab_sys_power_flag=0;

        //DAB_DISP_TEPY=dab_disp_serach;
        W2dab_CMD_SET_SEEK();
        return 1;
    }
#endif

#if defined(_ENABLE_DAB_SEEK_)
	if(global.dab_seek_flag)
	{
        W2dab_CMD_SET_SEEK();
		global.dab_seek_flag = FALSE;
	}
#endif

#if defined(_CHANGE_MODE_1_)
	if(global.dab_init_flag)
	{
		global.dab_init_flag = FALSE;
	}
	else
#endif
	{
    	W2dab_Send_N_Data(mcu_send_buf,5);
	}
    os_time_dly(10);
	if(radio_total == 0)//没有节目
    {
		DAB_VDD=0;
        dab_puts("-------w2dab no radio signle----111111---------\r\n");
        gSeach_elec=0;
        radio_total =0;
		DAB_DISP_TEPY=dab_disp_no_station;
    }
	else if((radio_total !=0 )&&(( DAB_ACC == 1 )||(DAB_VDD==1)))//有节目 上电
	{
		 dab_puts("-------w2dab no radio signle----222222---------\r\n");
		DAB_ACC=0;
		DAB_VDD=0;
		RADIO_INFO_DISP_TIME=0;
		RADIO_download_OK=0;
       	DAB_DISP_TEPY=dab_disp_downinfo;//下载数据
	}
    else//切换模式
    {
        dab_puts("-------w2dab get radio info------333333----------\r\n");
       // mcu_send_buf[1]=CMD_GET_ALL_RADIOINFO;//获取所有节目信息
       RADIO_INFO_DISP_TIME=0;
       DAB_DISP_TEPY=dab_disp_total;//显示电台总数
    }

    return TRUE;
}

void dab_info_play(void)
{
#if defined(_FY7000_SPECIAL_DISPLAY_)
	uint8 dispchar[13];
#elif defined(_FY5517_SPECIAL_DISPLAY_)
	uint8 dispchar[11];
#else
	uint8 dispchar[8];
#endif
    u8 mm=0;
    show_clear_icon(DISP_LEVEL_NORMAL);
	/*
	if(DAB_DISP_PLAYPAUSE==1)
	{
		if(++RADIO_SIGNAL_DET_TIME>=20)//
		{
			W2dab_get_appoint_radio_SIGNAL_STRENGTH_cmd();
			RADIO_SIGNAL_DET_TIME=0;
		}
	}*/

	if((DAB_DISP_PLAYPAUSE==1)&&(DAB_NOT_SEEK!=1))//
	{
		if(DAB_DISP_TEPY!=dab_disp_no_server)
		{
			if(++RADIO_DLS_DET_TIME>=70)//
			{
				RADIO_DLS_DET_TIME=0;
				W2dab_get_appoint_radio_dls_cmd();//
			}
		}

		if(++RADIO_SIGNAL_DET_TIME>=20)//
		{
			W2dab_get_appoint_radio_SIGNAL_STRENGTH_cmd();
			RADIO_SIGNAL_DET_TIME=0;
		}
		/*******************************************/
		if(DAB_DISP_TEPY!=dab_disp_no_server)
		{
			DAB_DISP_TEPY_OLD=DAB_DISP_TEPY;
		}
		if(DAB_DISPINFO_SIGNAL_STRENGTH_flag>=3)
		{
			DAB_DISP_TEPY=dab_disp_no_server;
		}
		else
		{
			DAB_DISP_TEPY=DAB_DISP_TEPY_OLD;
		}
	}

	if(RADIO_AUTO_PLAY_TIME)
	{
		RADIO_AUTO_PLAY_TIME--;
		if(RADIO_AUTO_PLAY_TIME==0)
		{
			memset(DAB_DISP_DLS, 0x00, DAB_DISP_DLS_SIZE);//
			DAB_DISP_DLS_SIZE=0;//
			DAB_DISP_PLAYPAUSE=0;//
			dab_select_flag=0;
			W2dab_set_appoint_radio_play_cmd((RADIO_CURRENT_STATION+1));//
		}
	}

#if defined(dab_select_station)
	if(dab_select_TIME)
	{
		dab_select_TIME--;
		if(dab_select_TIME==0)
		{
			if(RADIO_AUTO_PLAY_TIME==0)
			{
				dab_select_flag=0;
			}
		}
	}
#endif

	switch(DAB_DISP_TEPY)
    {
		case dab_disp_null://显示空
        #if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0]=' ';
			dispchar[1]=' ';
			dispchar[2]=' ';
			dispchar[3]=' ';
			dispchar[4]=' ';
			dispchar[5]=' ';
			dispchar[6]=' ';
			dispchar[7]=' ';
			dispchar[8]=' ';
			dispchar[9]=' ';
			dispchar[10]=' ';
			dispchar[11]=' ';
			dispchar[12]=' ';
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0]=' ';
    		dispchar[1]=' ';
    		dispchar[2]=' ';
    		dispchar[3]=' ';
    		dispchar[4]=' ';
    		dispchar[5]=' ';
    		dispchar[6]=' ';
    		dispchar[7]=' ';
    		dispchar[8]=' ';
    		dispchar[9]=' ';
    		dispchar[10]=' ';
		#else
			dispchar[0]=' ';
			dispchar[1]=' ';
			dispchar[2]=' ';
			dispchar[3]=' ';
			dispchar[4]=' ';
			dispchar[5]=' ';
			dispchar[6]=' ';
			dispchar[7]=' ';
		#endif
			break;

		case dab_disp_serach://搜台进度
		#if defined(_FY7000_SPECIAL_DISPLAY_)
        #if defined( bole_100_err_set)
            if(dab_sys_power_flag==0)
            {
                dispchar[0]='>';
    			dispchar[1]='>';
    			dispchar[2]='>';
    			dispchar[3]='D';
    			dispchar[4]='A';
    			dispchar[5]='B';
    			dispchar[6]='I';
    			dispchar[7]='N';
    			dispchar[8]='I';
    			dispchar[9]='T';
    			dispchar[10]='>';
    			dispchar[11]='>';
    			dispchar[12]='>';
            }
            else
        #endif
            {
    			dispchar[0]=' ';
    			dispchar[1]='S';
    			dispchar[2]='E';
    			dispchar[3]='A';
    			dispchar[4]='R';
    			dispchar[5]='C';
    			dispchar[6]='H';
    			dispchar[7]=' ';
    			dispchar[8]=' ';
    			dispchar[9]=(gSeach_elec%1000/100)+0x30;
    			dispchar[10]=(gSeach_elec%100/10)+0x30;
    			dispchar[11]=(gSeach_elec%10)+0x30;
    			dispchar[12]='%';
    			if(dispchar[9]==0x30)
    			{
    				dispchar[9]=' ';
    			}
            }
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
		    {
		    	dispchar[0]='S';
		    	dispchar[1]='E';
		    	dispchar[2]='A';
		    	dispchar[3]='R';
		    	dispchar[4]='C';
		    	dispchar[5]='H';
		    	dispchar[6]=' ';
		    	dispchar[7]=(gSeach_elec%1000/100)+0x30;
		    	dispchar[8]=(gSeach_elec%100/10)+0x30;
		    	dispchar[9]=(gSeach_elec%10)+0x30;
		    	dispchar[10]=' ';
		    	if(dispchar[9]==0x30)
		    	{
		    		dispchar[9]=' ';
		    	}
		    }
		#elif defined(_WLS316_SPECIAL_DISPLAY_)
			{
		    	dispchar[0]='S';
		    	dispchar[1]='C';
		    	dispchar[2]='A';
		    	dispchar[3]='N';
		    	dispchar[4]=' ';
		    	dispchar[5]=(gSeach_elec%1000/100)+0x30;
		    	dispchar[6]=(gSeach_elec%100/10)+0x30;
		    	dispchar[7]=(gSeach_elec%10)+0x30;
		    	if(dispchar[5]==0x30)
		    	{
		    		dispchar[5]=' ';
		    	}
		    }
		#elif defined(_FIVE_CHAR_DISPLAY_)
    		dispchar[0]='S';
    		dispchar[1]='E';
    		dispchar[2]='A';
    		dispchar[3]='R';
    		dispchar[4]='C';
    		dispchar[5]='H';
    		dispchar[6]=' ';
    		dispchar[7]=' ';
		#else
			if(gSeach_elec>=100)
		    {
				dispchar[0]='>';
				dispchar[1]='>';
				dispchar[2]='>';
				dispchar[3]='>';
				dispchar[4]='>';
				dispchar[5]=' ';

			}
			else if(gSeach_elec>80)
			{
				dispchar[0]='>';
				dispchar[1]='>';
				dispchar[2]='>';
				dispchar[3]='>';
				dispchar[4]='>';
				dispchar[5]=' ';

			}
			else if(gSeach_elec>65)
			{
				dispchar[0]='>';
				dispchar[1]='>';
				dispchar[2]='>';
				dispchar[3]='>';
				dispchar[4]=' ';
				dispchar[5]=' ';

			}
			else if(gSeach_elec>45)
		    {
				dispchar[0]='>';
				dispchar[1]='>';
				dispchar[2]='>';
				dispchar[3]=' ';
				dispchar[4]=' ';
				dispchar[5]=' ';

			}
			else if(gSeach_elec>25)
			{
				dispchar[0]='>';
				dispchar[1]='>';
				dispchar[2]=' ';
				dispchar[3]=' ';
				dispchar[4]=' ';
				dispchar[5]=' ';

			}
			else
			{
				dispchar[0]='>';
				dispchar[1]=' ';
				dispchar[2]=' ';
				dispchar[3]=' ';
				dispchar[4]=' ';
				dispchar[5]=' ';

			}
			dispchar[5]=(gSeach_elec%1000/100)+0x30;
			dispchar[6]=(gSeach_elec%100/10)+0x30;
			dispchar[7]=(gSeach_elec%10)+0x30;
			if(dispchar[5]==0x30)
			{
				dispchar[5]=' ';
			}
		#endif

			if(gSeach_elec==100)
			{
				RADIO_INFO_DISP_SERACH=0;
				if(radio_total==0)
				{
					DAB_DISP_TEPY=dab_disp_no_station;
				}
				else
				{
					DAB_DISP_TEPY=dab_disp_downinfo;
					RADIO_download_OK=0;
				#if defined(DAB_CURRENT_STATION_MEMORY)
					setup.DAB_CURRENT_STATION=0;
				#endif
				#if defined(DAB_NUM_SAVE_STATION)
					setup.DAB_NUM_STATION[0]=0;
					setup.DAB_NUM_STATION[1]=1;
					setup.DAB_NUM_STATION[2]=2;
					setup.DAB_NUM_STATION[3]=3;
					setup.DAB_NUM_STATION[4]=4;
					setup.DAB_NUM_STATION[5]=5;
				#endif
				}
			}
			else
			{
			    RADIO_AUTO_PLAY_TIME=0;//清除电台自动播放
				DAB_DISP_DLS_SIZE=0;//清除dls缓存信息长度
				DAB_DISP_PLAYPAUSE=0;//清除播放标志
				radio_total=0;//清除电台数
				RADIO_INFO_DISP_TIME=0;
			}
			DAB_NOT_SEEK=1;
			break;

		case dab_disp_name://电台名
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			#if defined(DAB_NAME_USER_16BYTE)
    			if(RADIO_CURRENT_STATION!=RADIO_OLD_STATION)
    			{
    				RADIO_OLD_STATION =RADIO_CURRENT_STATION;
    				DAB_DISP_NAME_POINT=0;
    				RADIO_INFO_DISP_TIME_NOW=0;
    			}
    			memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],13);
    			if(++RADIO_INFO_DISP_TIME_NOW>=20)
    			{
    				RADIO_INFO_DISP_TIME_NOW=0;
    			    if((16-DAB_DISP_NAME_POINT)>=13)
    			    {
    					memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],13);
    			    }
    				else
    				{
    					DAB_DISP_NAME_POINT=0;
    					memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],13);
    				}
    				DAB_DISP_NAME_POINT++;
    			}
			#else
				dispchar[0]=' ';
				dispchar[1]=' ';
				dispchar[2]=DAB_DISPINFO1[RADIO_CURRENT_STATION][6];
				dispchar[3]=DAB_DISPINFO1[RADIO_CURRENT_STATION][7];
				dispchar[4]=DAB_DISPINFO1[RADIO_CURRENT_STATION][8];
				dispchar[5]=DAB_DISPINFO1[RADIO_CURRENT_STATION][9];
				dispchar[6]=DAB_DISPINFO1[RADIO_CURRENT_STATION][10];
				dispchar[7]=DAB_DISPINFO1[RADIO_CURRENT_STATION][11];
				dispchar[8]=DAB_DISPINFO1[RADIO_CURRENT_STATION][12];
				dispchar[9]=DAB_DISPINFO1[RADIO_CURRENT_STATION][13];
				dispchar[10]=' ';
				dispchar[11]=' ';
				dispchar[12]=' ';
			#endif
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
		    #if defined(DAB_NAME_USER_16BYTE)
		    	if(RADIO_CURRENT_STATION!=RADIO_OLD_STATION)
		    	{
		    		RADIO_OLD_STATION =RADIO_CURRENT_STATION;
		    		DAB_DISP_NAME_POINT=0;
		    		RADIO_INFO_DISP_TIME_NOW=0;
		    	}
		    	memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],11);
		    	if(++RADIO_INFO_DISP_TIME_NOW>=20)
		    	{
		    		RADIO_INFO_DISP_TIME_NOW=0;
		    		if((16-DAB_DISP_NAME_POINT)>=11)
		    		{
		    			memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],11);
		    		}
		    		else
		    		{
		    			DAB_DISP_NAME_POINT=0;
		    			memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],11);
		    		}
		    		DAB_DISP_NAME_POINT++;
		    	}
		    #else
		    		dispchar[0]=' ';
		    		dispchar[1]=DAB_DISPINFO1[RADIO_CURRENT_STATION][6];
		    		dispchar[2]=DAB_DISPINFO1[RADIO_CURRENT_STATION][7];
		    		dispchar[3]=DAB_DISPINFO1[RADIO_CURRENT_STATION][8];
		    		dispchar[4]=DAB_DISPINFO1[RADIO_CURRENT_STATION][9];
		    		dispchar[5]=DAB_DISPINFO1[RADIO_CURRENT_STATION][10];
		    		dispchar[6]=DAB_DISPINFO1[RADIO_CURRENT_STATION][11];
		    		dispchar[7]=DAB_DISPINFO1[RADIO_CURRENT_STATION][12];
		    		dispchar[8]=DAB_DISPINFO1[RADIO_CURRENT_STATION][13];
		    		dispchar[9]=' ';
		    		dispchar[10]=' ';
		    #endif
			
		#elif defined(_FIVE_CHAR_DISPLAY_)
    		dispchar[0]=DAB_DISPINFO1[RADIO_CURRENT_STATION][6];
    		dispchar[1]=DAB_DISPINFO1[RADIO_CURRENT_STATION][7];
    		dispchar[2]=DAB_DISPINFO1[RADIO_CURRENT_STATION][8];
    		dispchar[3]=DAB_DISPINFO1[RADIO_CURRENT_STATION][9];
    		dispchar[4]=DAB_DISPINFO1[RADIO_CURRENT_STATION][10];
    		dispchar[5]=' ';
    		dispchar[6]=' ';
    		dispchar[7]=' ';
		#else
			#if defined(DAB_NAME_USER_16BYTE)
			if(RADIO_CURRENT_STATION!=RADIO_OLD_STATION)
			{
				RADIO_OLD_STATION =RADIO_CURRENT_STATION;
				DAB_DISP_NAME_POINT=0;
				RADIO_INFO_DISP_TIME_NOW=0;
			}
			memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],8);
			if(++RADIO_INFO_DISP_TIME_NOW>=15)
			{
				RADIO_INFO_DISP_TIME_NOW=0;
			    if((16-DAB_DISP_NAME_POINT)>=8)
			    {
					memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],8);
			    }
				else
				{
					DAB_DISP_NAME_POINT=0;
					memcpy(dispchar,&DAB_DISPINFO1[RADIO_CURRENT_STATION][6+DAB_DISP_NAME_POINT],8);
				}
				DAB_DISP_NAME_POINT++;
			}
			#else
				dispchar[0]=DAB_DISPINFO1[RADIO_CURRENT_STATION][6];
				dispchar[1]=DAB_DISPINFO1[RADIO_CURRENT_STATION][7];
				dispchar[2]=DAB_DISPINFO1[RADIO_CURRENT_STATION][8];
				dispchar[3]=DAB_DISPINFO1[RADIO_CURRENT_STATION][9];
				dispchar[4]=DAB_DISPINFO1[RADIO_CURRENT_STATION][10];
				dispchar[5]=DAB_DISPINFO1[RADIO_CURRENT_STATION][11];
				dispchar[6]=DAB_DISPINFO1[RADIO_CURRENT_STATION][12];
				dispchar[7]=DAB_DISPINFO1[RADIO_CURRENT_STATION][13];
			#endif
		#endif

			if(++RADIO_INFO_DISP_TIME>=100)//10S
			{
				DAB_DISP_NAME_POINT=0;
				RADIO_INFO_DISP_TIME=0;
				if(DAB_DISP_DLS_SIZE)
				{
					DAB_DISP_TEPY=dab_disp_dls;
				}
			}
			break;
		case dab_disp_total://	电台总数
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0]=' ';
			dispchar[1]=' ';
			dispchar[2]='T';
			dispchar[3]='O';
			dispchar[4]='T';
			dispchar[5]='A';
			dispchar[6]='L';
			dispchar[7]=' ';
			dispchar[8]=' ';
			if(radio_total%1000/100)
			{
			 	dispchar[9]=(radio_total%1000/100)+0x30;
			 	dispchar[10]=(radio_total%100/10)+0x30;
			}
			else
			{
				dispchar[9]=' ';
				if(radio_total%100/10)
				{
					dispchar[10]=(radio_total%100/10)+0x30;
				}
				else
				{
					dispchar[10]=' ';
				}
			}

			dispchar[11]=(radio_total%10)+0x30;
			dispchar[12]=' ';

		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0]='T';
    		dispchar[1]='O';
    		dispchar[2]='T';
    		dispchar[3]='A';
    		dispchar[4]='L';
    		dispchar[5]=' ';
    		dispchar[6]=' ';
    		if(radio_total%1000/100)
    		{
    			dispchar[7]=(radio_total%1000/100)+0x30;
    			dispchar[8]=(radio_total%100/10)+0x30;
    		}
    		else
    		{
    			dispchar[7]=' ';
    			if(radio_total%100/10)
    			{
    				dispchar[8]=(radio_total%100/10)+0x30;
    			}
    			else
    			{
    				dispchar[8]=' ';
    			}
    		}

    		dispchar[9]=(radio_total%10)+0x30;
    		dispchar[10]=' ';

		#else
			dispchar[0]=' ';
			dispchar[1]=' ';
			if(radio_total%1000/100)
			{
			 	dispchar[2]=(radio_total%1000/100)+0x30;
			 	dispchar[3]=(radio_total%100/10)+0x30;
			}
			else
			{
				dispchar[2]=' ';
				if(radio_total%100/10)
				{
					dispchar[3]=(radio_total%100/10)+0x30;
				}
				else
				{
					dispchar[3]=' ';
				}
			}
			dispchar[4]=(radio_total%10)+0x30;
			dispchar[5]=' ';
			dispchar[6]=' ';
			dispchar[7]=' ';
		#endif
			if(++RADIO_INFO_DISP_TIME>=20)// 2S
			{
				RADIO_INFO_DISP_TIME=0;
				DAB_DISP_TEPY=dab_disp_name;
				#if defined(DAB_NAME_USER_16BYTE)
				DAB_DISP_NAME_POINT=0;
				RADIO_INFO_DISP_TIME_NOW=0;
				#endif
				DAB_NOT_SEEK=0;
				W2dab_set_appoint_radio_play_cmd(RADIO_CURRENT_STATION+1);//
				os_time_dly(20);//200ms
				//W2dab_SET__radio_LINK_cmd(1);//
			}

			break;
		case dab_disp_no_station://没有电台
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0]=' ';
			dispchar[1]=' ';
			dispchar[2]='N';
			dispchar[3]='O';
			dispchar[4]=' ';
			dispchar[5]=' ';
			dispchar[6]='S';
			dispchar[7]='T';
			dispchar[8]='A';
			dispchar[9]='T';
			dispchar[10]='I';
			dispchar[11]='O';
			dispchar[12]='N';
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0]='N';
    		dispchar[1]='O';
    		dispchar[2]=' ';
    		dispchar[3]='S';
    		dispchar[4]='T';
    		dispchar[5]='A';
    		dispchar[6]='T';
    		dispchar[7]='I';
    		dispchar[8]='O';
    		dispchar[9]='N';
    		dispchar[10]=' ';
		#elif defined(_WLS316_SPECIAL_DISPLAY_)
    		dispchar[0]=' ';
    		dispchar[1]=' ';
    		dispchar[2]='D';
    		dispchar[3]='A';
    		dispchar[4]='B';
    		dispchar[5]=' ';
    		dispchar[6]=' ';
    		dispchar[7]=' ';
		#elif defined(_FIVE_CHAR_DISPLAY_)
    		dispchar[0]='N';
    		dispchar[1]='O';
    		dispchar[2]='D';
    		dispchar[3]='A';
    		dispchar[4]='B';
    		dispchar[5]=' ';
    		dispchar[6]=' ';
    		dispchar[7]=' ';
		#else
			dispchar[0]='N';
			dispchar[1]='O';
			dispchar[2]=' ';
			dispchar[3]='R';
			dispchar[4]='A';
			dispchar[5]='D';
			dispchar[6]='I';
			dispchar[7]='O';
		#endif
			break;
		case dab_disp_no_server://信号差
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0]=' ';
			dispchar[1]=' ';
			dispchar[2]=' ';
			dispchar[3]='N';
			dispchar[4]='O';
			dispchar[5]=' ';
			dispchar[6]='S';
			dispchar[7]='E';
			dispchar[8]='R';
			dispchar[9]='V';
			dispchar[10]='E';
			dispchar[11]=' ';
			dispchar[12]=' ';
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0]=' ';
    		dispchar[1]='N';
    		dispchar[2]='O';
    		dispchar[3]=' ';
    		dispchar[4]=' ';
    		dispchar[5]='S';
    		dispchar[6]='E';
    		dispchar[7]='R';
    		dispchar[8]='V';
    		dispchar[9]='E';
    		dispchar[10]=' ';
		#else
			#if defined(low_sig_disp)
			dispchar[0]='L';
			dispchar[1]='I';
			dispchar[2]='N';
			dispchar[3]='K';
			dispchar[4]='.';
			#if defined(_FIVE_CHAR_DISPLAY_)
			dispchar[5]=' ';
			dispchar[6]=' ';
			dispchar[7]=' ';
			#else
			dispchar[5]='.';
			dispchar[6]='.';
			dispchar[7]='.';
			#endif
			#else
			dispchar[0]='N';
			dispchar[1]='O';
			dispchar[2]=' ';
			dispchar[3]='S';
			dispchar[4]='E';
			dispchar[5]='R';
			dispchar[6]='V';
			dispchar[7]='E';
			#endif
		#endif
			break;
		case dab_disp_boder://波特率
			{
		   	 	DAB_DISPINFO_FREQUENCY=((DAB_DISPINFO1[RADIO_CURRENT_STATION][1]<<16 )|(DAB_DISPINFO1[RADIO_CURRENT_STATION][2]<<8 )|DAB_DISPINFO1[RADIO_CURRENT_STATION][3]);
			}
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0]=' ';
			if(DAB_DISPINFO_FREQUENCY%1000000/100000)
			{
			 	dispchar[1]=(DAB_DISPINFO_FREQUENCY%1000000/100000)+0x30;
			 	dispchar[2]=(DAB_DISPINFO_FREQUENCY%100000/10000)+0x30;
			}
			else
			{
				dispchar[1]=' ';
				if(DAB_DISPINFO_FREQUENCY%100000/10000)
				{
					dispchar[2]=(DAB_DISPINFO_FREQUENCY%100000/10000)+0x30;
				}
				else
				{
					dispchar[2]=' ';
				}
			}
			dispchar[3]='0'+(DAB_DISPINFO_FREQUENCY%10000/1000);

			dispchar[4]='.';
			dispchar[5]='0'+(DAB_DISPINFO_FREQUENCY%1000/100);
			dispchar[6]='0'+(DAB_DISPINFO_FREQUENCY%100/10);
			dispchar[7]='0'+(DAB_DISPINFO_FREQUENCY%10);
			dispchar[8]='M';
			dispchar[9]='h';
			dispchar[10]='z';
			dispchar[11]=' ';
			dispchar[12]=' ';
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0]=' ';
    		if(DAB_DISPINFO_FREQUENCY%1000000/100000)
    		{
    			dispchar[1]=(DAB_DISPINFO_FREQUENCY%1000000/100000)+0x30;
    			dispchar[2]=(DAB_DISPINFO_FREQUENCY%100000/10000)+0x30;
    		}
    		else
    		{
    			dispchar[1]=' ';
    			if(DAB_DISPINFO_FREQUENCY%100000/10000)
    			{
    				dispchar[2]=(DAB_DISPINFO_FREQUENCY%100000/10000)+0x30;
    			}
    			else
    			{
    				dispchar[2]=' ';
    			}
    		}
    		dispchar[3]='0'+(DAB_DISPINFO_FREQUENCY%10000/1000);

    		dispchar[4]='.';
    		dispchar[5]='0'+(DAB_DISPINFO_FREQUENCY%1000/100);
    		dispchar[6]='0'+(DAB_DISPINFO_FREQUENCY%100/10);
    		dispchar[7]='0'+(DAB_DISPINFO_FREQUENCY%10);
    		dispchar[8]='M';
    		dispchar[9]=' ';
    		dispchar[10]=' ';
		#elif defined(_FIVE_CHAR_DISPLAY_)
			if(DAB_DISPINFO_FREQUENCY%100000/10000)
			{
				dispchar[0]=(DAB_DISPINFO_FREQUENCY%100000/10000)+0x30;
			}
			else
			{
				dispchar[0]=' ';
			}
    		dispchar[1]='0'+(DAB_DISPINFO_FREQUENCY%10000/1000);
    		dispchar[2]='.';
    		dispchar[3]='0'+(DAB_DISPINFO_FREQUENCY%1000/100);
    		dispchar[4]='0'+(DAB_DISPINFO_FREQUENCY%100/10);
    		dispchar[5]='0'+(DAB_DISPINFO_FREQUENCY%10);
			dispchar[6]=' ';
			dispchar[7]=' ';
		#else
			if(DAB_DISPINFO_FREQUENCY%1000000/100000)
			{
			 	dispchar[0]=(DAB_DISPINFO_FREQUENCY%1000000/100000)+0x30;
			 	dispchar[1]=(DAB_DISPINFO_FREQUENCY%100000/10000)+0x30;
			}
			else
			{
				dispchar[0]=' ';
				if(DAB_DISPINFO_FREQUENCY%100000/10000)
				{
					dispchar[1]=(DAB_DISPINFO_FREQUENCY%100000/10000)+0x30;
				}
				else
				{
					dispchar[1]=' ';
				}
			}
			dispchar[2]='0'+(DAB_DISPINFO_FREQUENCY%10000/1000);
			dispchar[3]='.';
			dispchar[4]='0'+(DAB_DISPINFO_FREQUENCY%1000/100);
			dispchar[5]='0'+(DAB_DISPINFO_FREQUENCY%100/10);
			dispchar[6]='0'+(DAB_DISPINFO_FREQUENCY%10);
			dispchar[7]='M';
		#endif
			//#if defined(DAB_DISP_AUTO)
			if(++RADIO_INFO_DISP_TIME>=80) // 8S
			{
				RADIO_INFO_DISP_TIME=0;
				DAB_DISP_TEPY=dab_disp_name;
				#if defined(DAB_NAME_USER_16BYTE)
				DAB_DISP_NAME_POINT=0;
				RADIO_INFO_DISP_TIME_NOW=0;
				#endif
			}
			//#endif
			break;
		case dab_disp_bit://比特率
			{
		    	DAB_DISPINFO_BIT =((DAB_DISPINFO1[RADIO_CURRENT_STATION][4]<<8 )|DAB_DISPINFO1[RADIO_CURRENT_STATION][5]);
			}
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0]=' ';
			dispchar[1]=' ';
			if(DAB_DISPINFO_BIT%1000/100)
			{
			 	dispchar[2]=(DAB_DISPINFO_BIT%1000/100)+0x30;
			 	dispchar[3]=(DAB_DISPINFO_BIT%100/10)+0x30;
			}
			else
			{
				dispchar[2]=' ';
				if(DAB_DISPINFO_BIT%100/10)
				{
					dispchar[3]=(DAB_DISPINFO_BIT%100/10)+0x30;
				}
				else
				{
					dispchar[3]=' ';
				}
			}
			dispchar[4]='0'+(DAB_DISPINFO_BIT%10);
			dispchar[5]=' ';
			dispchar[6]=' ';
			dispchar[7]='K';
			dispchar[8]='b';
			dispchar[9]='p';
			dispchar[10]='s';
			dispchar[11]=' ';
			dispchar[12]=' ';
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0]=' ';
    		if(DAB_DISPINFO_BIT%1000/100)
    		{
    			dispchar[1]=(DAB_DISPINFO_BIT%1000/100)+0x30;
    			dispchar[2]=(DAB_DISPINFO_BIT%100/10)+0x30;
    		}
    		else
    		{
    			dispchar[1]=' ';
    			if(DAB_DISPINFO_BIT%100/10)
    			{
    				dispchar[2]=(DAB_DISPINFO_BIT%100/10)+0x30;
    			}
    			else
    			{
    				dispchar[2]=' ';
    			}
    		}
    		dispchar[3]='0'+(DAB_DISPINFO_BIT%10);
    		dispchar[4]=' ';
    		dispchar[5]=' ';
    		dispchar[6]='K';
    		dispchar[7]='B';
    		dispchar[8]='P';
    		dispchar[9]='S';
    		dispchar[10]=' ';
		#else
			if(DAB_DISPINFO_BIT%1000/100)
			{
			 	dispchar[0]=(DAB_DISPINFO_BIT%1000/100)+0x30;
			 	dispchar[1]=(DAB_DISPINFO_BIT%100/10)+0x30;
			}
			else
			{
				dispchar[0]=' ';
				if(DAB_DISPINFO_BIT%100/10)
				{
					dispchar[1]=(DAB_DISPINFO_BIT%100/10)+0x30;
				}
				else
				{
					dispchar[1]=' ';
				}
			}
			dispchar[2]='0'+(DAB_DISPINFO_BIT%10);
			dispchar[3]=' ';
		#if defined(_FIVE_CHAR_DISPLAY_)
    		dispchar[4]='P';
    		dispchar[5]='S';
    		dispchar[6]=' ';
    		dispchar[7]=' ';
		#else
			dispchar[4]='K';
			dispchar[5]='B';
			dispchar[6]='P';
			dispchar[7]='S';
		#endif
		#endif
			//#if defined(DAB_DISP_AUTO)
			if(++RADIO_INFO_DISP_TIME>=80)// 8S
			{
				RADIO_INFO_DISP_TIME=0;
				DAB_DISP_TEPY=dab_disp_name;
				#if defined(DAB_NAME_USER_16BYTE)
				DAB_DISP_NAME_POINT=0;
				RADIO_INFO_DISP_TIME_NOW=0;
				#endif
			}
			//#endif
			break;
		case dab_disp_dab_error://错误
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0] ='D';
			dispchar[1] ='A';
			dispchar[2] ='B';
			dispchar[3] ='-';
			dispchar[4] ='W';
			dispchar[5] ='O';
			dispchar[6] ='R';
			dispchar[7] ='K';
			dispchar[8] ='-';
			dispchar[9] ='-';
			dispchar[10]='E';
			dispchar[11]='R';
			dispchar[12]='R';
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0] ='D';
    		dispchar[1] ='A';
    		dispchar[2] ='B';
    		dispchar[3] =' ';
    		dispchar[4] =' ';
    		dispchar[5] ='E';
    		dispchar[6] ='R';
    		dispchar[7] ='R';
    		dispchar[8] ='O';
    		dispchar[9] ='R';
    		dispchar[10]=' ';
		#elif defined(_FIVE_CHAR_DISPLAY_)
    		dispchar[0] ='E';
    		dispchar[1] ='R';
    		dispchar[2] ='R';
    		dispchar[3] ='O';
    		dispchar[4] ='R';
    		dispchar[5] =' ';
    		dispchar[6] =' ';
    		dispchar[7] =' ';
		#else
			dispchar[0] ='D';
			dispchar[1] ='a';
			dispchar[2] ='b';
			dispchar[3] ='E';
			dispchar[4] ='r';
			dispchar[5] ='r';
			dispchar[6] ='o';
			dispchar[7] ='r';
		#endif
			break;
		case dab_disp_linking://link中
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0]=' ';
			dispchar[1]=' ';
			dispchar[2]=' ';
			dispchar[3]='L';
			dispchar[4]='I';
			dispchar[5]='N';
			dispchar[6]='K';
			dispchar[7]='.';
			dispchar[8]='.';
			dispchar[9]='.';
			dispchar[10]='.';
			dispchar[11]=' ';
			dispchar[12]=' ';
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0]=' ';
    		dispchar[1]='L';
    		dispchar[2]='I';
    		dispchar[3]='N';
    		dispchar[4]='K';
    		dispchar[5]='.';
    		dispchar[6]='.';
    		dispchar[7]='.';
    		dispchar[8]='.';
    		dispchar[9]=' ';
    		dispchar[10]=' ';
		#else
			dispchar[0]='L';
			dispchar[1]='I';
			dispchar[2]='N';
			dispchar[3]='K';
			dispchar[4]='.';
		#if defined(_FIVE_CHAR_DISPLAY_)
    		dispchar[5] =' ';
    		dispchar[6] =' ';
    		dispchar[7] =' ';
		#else
			dispchar[5]='.';
			dispchar[6]='.';
			dispchar[7]='.';
		#endif
		#endif
			break;
		case dab_disp_linkonoff://link on  off
		    if(SET_LINK)
		    {
			#if defined(_FY7000_SPECIAL_DISPLAY_)
				dispchar[0]=' ';
				dispchar[1]=' ';
				dispchar[2]=' ';
				dispchar[3]='L';
				dispchar[4]='I';
				dispchar[5]='N';
				dispchar[6]='K';
				dispchar[7]=' ';
				dispchar[8]=' ';
				dispchar[9]='O';
				dispchar[10]='N';
				dispchar[11]=' ';
				dispchar[12]=' ';
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
    			dispchar[0]=' ';
    			dispchar[1]='L';
    			dispchar[2]='I';
    			dispchar[3]='N';
    			dispchar[4]='K';
    			dispchar[5]=' ';
    			dispchar[6]=' ';
    			dispchar[7]='O';
    			dispchar[8]='N';
    			dispchar[9]=' ';
    			dispchar[10]=' ';
			#elif defined(_FIVE_CHAR_DISPLAY_)
    			dispchar[0]='L';
    			dispchar[1]='I';
    			dispchar[2]='N';
    			dispchar[3]='O';
    			dispchar[4]='N';
    			dispchar[5]=' ';
    			dispchar[6]=' ';
    			dispchar[7]=' ';
			#else
				dispchar[0]='L';
				dispchar[1]='I';
				dispchar[2]='N';
				dispchar[3]='K';
				dispchar[4]=' ';
				dispchar[5]=' ';
				dispchar[6]='O';
				dispchar[7]='N';
			#endif
			}
			else
			{
			#if defined(_FY7000_SPECIAL_DISPLAY_)
				dispchar[0]=' ';
				dispchar[1]=' ';
				dispchar[2]='L';
				dispchar[3]='I';
				dispchar[4]='N';
				dispchar[5]='K';
				dispchar[6]=' ';
				dispchar[7]=' ';
				dispchar[8]='O';
				dispchar[9]='F';
				dispchar[10]='F';
				dispchar[11]=' ';
				dispchar[12]=' ';
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
    			dispchar[0]=' ';
    			dispchar[1]='L';
    			dispchar[2]='I';
    			dispchar[3]='N';
    			dispchar[4]='K';
    			dispchar[5]=' ';
    			dispchar[6]=' ';
    			dispchar[7]='O';
    			dispchar[8]='F';
    			dispchar[9]='F';
    			dispchar[10]=' ';
			#elif defined(_FIVE_CHAR_DISPLAY_)
				dispchar[0]='L';
				dispchar[1]='I';
				dispchar[2]='N';
				dispchar[3]='O';
				dispchar[4]='F';
				dispchar[5]='F';
				dispchar[6]=' ';
				dispchar[7]=' ';
			#else
				dispchar[0]='L';
				dispchar[1]='I';
				dispchar[2]='N';
				dispchar[3]='K';
				dispchar[4]=' ';
				dispchar[5]='O';
				dispchar[6]='F';
				dispchar[7]='F';
			#endif
			}
			break;
		case dab_disp_dls:
		    if(DAB_DISP_DLS_SIZE)
		    {
		    #if defined(_FY7000_SPECIAL_DISPLAY_)
				if(++RADIO_INFO_DISP_TIME_NOW>=8)
			    {
					RADIO_INFO_DISP_TIME_NOW=0;
				    if(((DAB_DISP_DLS_SIZE-1)-DAB_DISP_DLS_POINT)>=13)
				    {
						memcpy(dispchar,&DAB_DISP_DLS[DAB_DISP_DLS_POINT],13);
				    }
					else
					{
						DAB_DISP_TEPY=dab_disp_name;
						RADIO_INFO_DISP_TIME=0;
						DAB_DISP_DLS_POINT=0;
						#if defined(DAB_NAME_USER_16BYTE)
						DAB_DISP_NAME_POINT=0;
						RADIO_INFO_DISP_TIME_NOW=0;
						#endif
					}
					DAB_DISP_DLS_POINT++;
					if(DAB_DISP_DLS_POINT >=(DAB_DISP_DLS_SIZE-1))
					{
						DAB_DISP_DLS_POINT=(DAB_DISP_DLS_SIZE-1);
					}
			    }
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
    			if(++RADIO_INFO_DISP_TIME_NOW>=7)
    			{
    				RADIO_INFO_DISP_TIME_NOW=0;
    				if(((DAB_DISP_DLS_SIZE-1)-DAB_DISP_DLS_POINT)>=11)
    				{
    					memcpy(dispchar,&DAB_DISP_DLS[DAB_DISP_DLS_POINT],11);
    				}
    				else
    				{
    					DAB_DISP_TEPY=dab_disp_name;
    					RADIO_INFO_DISP_TIME=0;
    					DAB_DISP_DLS_POINT=0;
    				#if defined(DAB_NAME_USER_16BYTE)
    					DAB_DISP_NAME_POINT=0;
    					RADIO_INFO_DISP_TIME_NOW=0;
    				#endif
    				}
    				DAB_DISP_DLS_POINT++;
    				if(DAB_DISP_DLS_POINT >=(DAB_DISP_DLS_SIZE-1))
    				{
    					DAB_DISP_DLS_POINT=(DAB_DISP_DLS_SIZE-1);
    				}
    			}
			#elif defined(_FIVE_CHAR_DISPLAY_)
    			if(++RADIO_INFO_DISP_TIME_NOW>=4)
    			{
    				RADIO_INFO_DISP_TIME_NOW=0;
    				if(((DAB_DISP_DLS_SIZE-1)-DAB_DISP_DLS_POINT)>=6)
    				{
    					memcpy(dispchar,&DAB_DISP_DLS[DAB_DISP_DLS_POINT],6);
    				}
    				else
    				{
    					DAB_DISP_TEPY=dab_disp_name;
    					RADIO_INFO_DISP_TIME=0;
    					DAB_DISP_DLS_POINT=0;
    				#if defined(DAB_NAME_USER_16BYTE)
    					DAB_DISP_NAME_POINT=0;
    					RADIO_INFO_DISP_TIME_NOW=0;
    				#endif
    				}
    				DAB_DISP_DLS_POINT++;
    				if(DAB_DISP_DLS_POINT >=(DAB_DISP_DLS_SIZE-1))
    				{
    					DAB_DISP_DLS_POINT=(DAB_DISP_DLS_SIZE-1);
    				}
    			}
			#else
			    if(++RADIO_INFO_DISP_TIME_NOW>=4)
			    {
					RADIO_INFO_DISP_TIME_NOW=0;
				    if(((DAB_DISP_DLS_SIZE-1)-DAB_DISP_DLS_POINT)>=8)
				    {
						memcpy(dispchar,&DAB_DISP_DLS[DAB_DISP_DLS_POINT],8);
				    }
					else
					{
						DAB_DISP_TEPY=dab_disp_name;
						RADIO_INFO_DISP_TIME=0;
						DAB_DISP_DLS_POINT=0;
						#if defined(DAB_NAME_USER_16BYTE)
						DAB_DISP_NAME_POINT=0;
						RADIO_INFO_DISP_TIME_NOW=0;
						#endif
					}
					DAB_DISP_DLS_POINT++;
					if(DAB_DISP_DLS_POINT >=(DAB_DISP_DLS_SIZE-1))
					{
						DAB_DISP_DLS_POINT=(DAB_DISP_DLS_SIZE-1);
					}
			    }
			#endif

		    }
			else
			{
			#if defined(_FY7000_SPECIAL_DISPLAY_)
				dispchar[0]=' ';
				dispchar[1]=' ';
				dispchar[2]=' ';
				dispchar[3]='N';
				dispchar[4]='O';
				dispchar[5]=' ';
				dispchar[6]=' ';
				dispchar[7]='D';
				dispchar[8]='L';
				dispchar[9]='S';
				dispchar[10]=' ';
				dispchar[11]=' ';
				dispchar[12]=' ';
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
    			dispchar[0]=' ';
    			dispchar[1]=' ';
    			dispchar[2]='N';
    			dispchar[3]='O';
    			dispchar[4]=' ';
    			dispchar[5]=' ';
    			dispchar[6]='D';
    			dispchar[7]='L';
    			dispchar[8]='S';
    			dispchar[9]=' ';
    			dispchar[10]=' ';
			#elif defined(_FIVE_CHAR_DISPLAY_)
    			dispchar[0]='N';
    			dispchar[1]='0';
    			dispchar[2]=' ';
    			dispchar[3]='D';
    			dispchar[4]='L';
    			dispchar[5]='S';
    			dispchar[6]=' ';
    			dispchar[7]=' ';
			#else
				dispchar[0]=' ';
				dispchar[1]='N';
				dispchar[2]='0';
				dispchar[3]=' ';
				dispchar[4]='D';
				dispchar[5]='L';
				dispchar[6]='S';
				dispchar[7]=' ';
			#endif
				if(++RADIO_INFO_DISP_TIME>=40)  // 4S
				{
					RADIO_INFO_DISP_TIME=0;
					DAB_DISP_DLS_POINT=0;
					DAB_DISP_TEPY=dab_disp_name;
					#if defined(DAB_NAME_USER_16BYTE)
					DAB_DISP_NAME_POINT=0;
					RADIO_INFO_DISP_TIME_NOW=0;
					#endif
				}
			}
			break;
		case dab_disp_pty://pty
			{
				DAB_DISPINFO_PTY = DAB_DISPINFO1[RADIO_CURRENT_STATION][0];
			}
			DAB_DISP_PTY_SIZE = strlen(DAB_DISP_PTY_INFO[DAB_DISPINFO_PTY]);
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			memcpy(dispchar,&DAB_DISP_PTY_INFO[DAB_DISPINFO_PTY][DAB_DISP_PTY_POINT],DAB_DISP_PTY_SIZE);
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		if(++RADIO_INFO_DISP_TIME_NOW>=11)
    		{
    			RADIO_INFO_DISP_TIME_NOW=0;
    			if((DAB_DISP_PTY_SIZE-DAB_DISP_PTY_POINT)>=11)
    			{
    				memcpy(dispchar,&DAB_DISP_PTY_INFO[DAB_DISPINFO_PTY][DAB_DISP_PTY_POINT],11);
    			}
    			else
    			{
    				DAB_DISP_PTY_POINT=0;
    				memcpy(dispchar,&DAB_DISP_PTY_INFO[DAB_DISPINFO_PTY][DAB_DISP_PTY_POINT],11);
    			}
    			DAB_DISP_PTY_POINT++;
    		}
		#elif defined(_FIVE_CHAR_DISPLAY_)
    		if(++RADIO_INFO_DISP_TIME_NOW>=13)
    		{
    			RADIO_INFO_DISP_TIME_NOW=0;
    			if((DAB_DISP_PTY_SIZE-DAB_DISP_PTY_POINT)>=6)
    			{
    				memcpy(dispchar,&DAB_DISP_PTY_INFO[DAB_DISPINFO_PTY][DAB_DISP_PTY_POINT],6);
    			}
    			else
    			{
    				DAB_DISP_PTY_POINT=0;
    				memcpy(dispchar,&DAB_DISP_PTY_INFO[DAB_DISPINFO_PTY][DAB_DISP_PTY_POINT],6);
    			}
    			DAB_DISP_PTY_POINT++;
    		}
		#else
			if(++RADIO_INFO_DISP_TIME_NOW>=13)
			{
				RADIO_INFO_DISP_TIME_NOW=0;
			    if((DAB_DISP_PTY_SIZE-DAB_DISP_PTY_POINT)>=8)
			    {
					memcpy(dispchar,&DAB_DISP_PTY_INFO[DAB_DISPINFO_PTY][DAB_DISP_PTY_POINT],8);
			    }
				else
				{
					DAB_DISP_PTY_POINT=0;
					memcpy(dispchar,&DAB_DISP_PTY_INFO[DAB_DISPINFO_PTY][DAB_DISP_PTY_POINT],8);
				}
				DAB_DISP_PTY_POINT++;
			}
		#endif

			if(++RADIO_INFO_DISP_TIME>=80) // 8S
			{
				RADIO_INFO_DISP_TIME=0;
				DAB_DISP_PTY_POINT=0;
				DAB_DISP_TEPY=dab_disp_name;
				#if defined(DAB_NAME_USER_16BYTE)
				DAB_DISP_NAME_POINT=0;
				RADIO_INFO_DISP_TIME_NOW=0;
				#endif
			}
			break;
		case dab_disp_signal://信号强度
		#if defined(_FY7000_SPECIAL_DISPLAY_)
			dispchar[0]=' ';
			dispchar[1]=' ';
			dispchar[2]='S';
			dispchar[3]='I';
			dispchar[4]='G';
			dispchar[5]='N';
			dispchar[6]='A';
			dispchar[7]='L';
			dispchar[8]='=';
			dispchar[9]='0'+(DAB_DISPINFO_SIGNAL_STRENGTH%10);
			dispchar[10]=' ';
			dispchar[11]=' ';
			dispchar[12]=' ';
		#elif defined(_FY5517_SPECIAL_DISPLAY_)
    		dispchar[0]=' ';
    		dispchar[1]='S';
    		dispchar[2]='I';
    		dispchar[3]='G';
    		dispchar[4]='N';
    		dispchar[5]='A';
    		dispchar[6]='L';
    		dispchar[7]='=';
    		dispchar[8]='0'+(DAB_DISPINFO_SIGNAL_STRENGTH%10);
    		dispchar[9]=' ';
    		dispchar[10]=' ';
		#elif defined(_HT867_SPECIAL_DISPLAY_)
    		dispchar[0]='S';
			dispchar[1]='I';
			dispchar[2]='G';
			dispchar[3]='N';
			dispchar[4]='A';
			dispchar[5]='L';
			dispchar[6]='0'+(DAB_DISPINFO_SIGNAL_STRENGTH%10);
		#elif defined(_FIVE_CHAR_DISPLAY_)
    		dispchar[0]='S';
    		dispchar[1]='I';
    		dispchar[2]='G';
    		dispchar[3]='N';
    		dispchar[4]=' ';
    		dispchar[5]='0'+(DAB_DISPINFO_SIGNAL_STRENGTH%10);
    		dispchar[6]=' ';
    		dispchar[7]=' ';
		#else
			dispchar[0]='S';
			dispchar[1]='I';
			dispchar[2]='G';
			dispchar[3]='N';
			dispchar[4]='A';
			dispchar[5]='L';
			dispchar[6]='=';
			dispchar[7]='0'+(DAB_DISPINFO_SIGNAL_STRENGTH%10);
		#endif
			//#if defined(DAB_DISP_AUTO)
			if(++RADIO_INFO_DISP_TIME>=80) // 8S
			{
				RADIO_INFO_DISP_TIME=0;
				DAB_DISP_TEPY=dab_disp_name;

				#if defined(DAB_NAME_USER_16BYTE)
				DAB_DISP_NAME_POINT=0;
				RADIO_INFO_DISP_TIME_NOW=0;
				#endif
			}
			//#endif
			break;
		case dab_disp_downinfo://下载信息
			#if defined(_FY7000_SPECIAL_DISPLAY_)
				dispchar[0]=' ';
				dispchar[1]='D';
				dispchar[2]='O';
				dispchar[3]='W';
				dispchar[4]='N';
				dispchar[5]='L';
				dispchar[6]='O';
				dispchar[7]='A';
				dispchar[8]='D';
				dispchar[9]='.';
				dispchar[10]='.';
				dispchar[11]='.';
				dispchar[12]='.';
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
    			dispchar[0]=' ';
    			dispchar[1]='L';
    			dispchar[2]='O';
    			dispchar[3]='A';
    			dispchar[4]='D';
    			dispchar[5]='.';
    			dispchar[6]='.';
    			dispchar[7]='.';
    			dispchar[8]='.';
    			dispchar[9]=' ';
    			dispchar[10]=' ';
			#elif defined(_FIVE_CHAR_DISPLAY_)
    			dispchar[0]='L';
    			dispchar[1]='O';
    			dispchar[2]='A';
    			dispchar[3]='D';
    			dispchar[4]='.';
    			dispchar[5]=' ';
    			dispchar[6]=' ';
    			dispchar[7]=' ';
			#else
				dispchar[0]='L';
				dispchar[1]='O';
				dispchar[2]='A';
				dispchar[3]='D';
				dispchar[4]='.';
				dispchar[5]='.';
				dispchar[6]='.';
				dispchar[7]='.';
			#endif
				DAB_NOT_SEEK=1;
				if(RADIO_download_OK ==1)
				{
					RADIO_download_OK=0;
					RADIO_SEEK_STATION=0;
					DAB_NOT_SEEK=0;
					DAB_DISP_TEPY=dab_disp_total;
				}
				else if(RADIO_CURRENT_STATION_INFO_OK==0)
				{
					RADIO_SEEK_STATION=0;
					RADIO_CURRENT_STATION_INFO_OK=1;
					W2dab_get_appoint_radio_info_cmd(RADIO_SEEK_STATION+1);//
				}
				else if(RADIO_SEEK_STATION!=RADIO_SEEK_STATION_OLD)
				{
					RADIO_SEEK_STATION_OLD=RADIO_SEEK_STATION;
					RADIO_SEEK_STATION_COUNT=0;
				}
				else if(RADIO_SEEK_STATION==RADIO_SEEK_STATION_OLD)
				{
					if(++RADIO_SEEK_STATION_COUNT>=10)//超时处理
					{
						RADIO_SEEK_STATION_COUNT=0;
						W2dab_get_appoint_radio_info_cmd(RADIO_SEEK_STATION+1);//
					}
				}

			break;
	#if defined(DAB_NUM_SAVE_STATION)
			case dab_disp_save_number:
			#if defined(_FY7000_SPECIAL_DISPLAY_)
				dispchar[0]=' ';
				dispchar[1]=' ';
				dispchar[2]=' ';
				dispchar[3]=' ';
				dispchar[4]=' ';
				dispchar[5]=' ';
				dispchar[6]=' ';
				dispchar[7]=' ';
				dispchar[8]=' ';
				dispchar[9]=' ';
				dispchar[10]=' ';
				dispchar[11]=' ';
				dispchar[12]=' ';
			#elif defined(_FY5517_SPECIAL_DISPLAY_)
    			dispchar[0]=' ';
    			dispchar[1]=' ';
    			dispchar[2]=' ';
    			dispchar[3]=' ';
    			dispchar[4]=' ';
    			dispchar[5]=' ';
    			dispchar[6]=' ';
    			dispchar[7]=' ';
    			dispchar[8]=' ';
    			dispchar[9]=' ';
    			dispchar[10]=' ';
			#else
				dispchar[0]=' ';
				dispchar[1]=' ';
				dispchar[2]=' ';
				dispchar[3]=' ';
				dispchar[4]=' ';
				dispchar[5]=' ';
				dispchar[6]=' ';
				dispchar[7]=' ';
			#endif
				DAB_DISP_TEPY=dab_disp_name;
				ui_show_string(DISP_LEVEL_SETTING, dispchar);
    			ui_disp.now_level = DISP_LEVEL_SETTING;
    			ui_disp.timer = 500;
			break;
		#endif


	}
	ui_show_string(DISP_LEVEL_NORMAL, dispchar);

}


static void dab_dac_channel_off(void)
{
	sys_mute_ctrl(ENABLE, 0);
#if defined(ENABLE_PT2312_CHANNEL)||defined(_ENABLE_DAB_VOLUME_)||defined(ENABLE_DAB_PT2313_CHANNEL)||defined(ENABLE_PT2313_CHANNEL)||defined(_ENABLE_DAB_OT2348_)
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DAB_OFF, TRUE);
#else
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RADIO_OFF, TRUE);
#endif
}

static void dab_info_init(void)
{
#if defined(_SPECIAL_RDS_DELAY_)
	OSTimeDly(10);
#endif
    sys_mute_ctrl(ENABLE, 500);
#if defined(ENABLE_PT2312_CHANNEL)||defined(_ENABLE_DAB_VOLUME_)||defined(ENABLE_DAB_PT2313_CHANNEL)||defined(ENABLE_PT2313_CHANNEL)||defined(_ENABLE_DAB_OT2348_)
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DAB_ON, TRUE);
#else
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RADIO_ON, TRUE);
#endif
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
	if(tuner_Yhx_w2dab_detect())
	{
		dab_puts("dab--->ok\n");
		tuner_Yhx_w2dab_init();
	}
	else
	{
		dab_puts("dab--->error\n");
		DAB_DISP_TEPY=dab_disp_dab_error;//错误
	}

}

static void dab_reset_common(int msg, int type)
{
	if((get_msg_type(type) == MSG_TYPE_KEY)||(get_msg_type(type) == MSG_TYPE_IR))
	{

	}
}

static void dab_timer(void *param)
{
	os_taskq_post_msg(DAB_TASK_NAME, 1, MSG_AUX_TIMER);
}

#ifdef	TD_ADD_SPP
extern u8 gAppRefreshTime ;
void Spp_Tx_dab(void);

#endif

static void dab_task(void *p)
{
    int msg[6];

    dab_puts("\n************************dab TASK********************\n");
    malloc_stats();
	UART_POP = (DAB_BUFFER_t *)malloc_fun(UART_POP ,sizeof(DAB_BUFFER_t), 0);
	if(UART_POP == NULL)
	{
		dab_puts("dab_ctrl err\n");
		while(1);
	}

	disp_send_msg(DISP_EVENT_DAB);
	os_taskq_post_msg(DAB_TASK_NAME, 1, MSG_AUX_INIT); //触发AUX模块初始化
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(dab_timer, NULL, 200);
#else
	timer_reg_func(dab_timer, NULL, DAB_POLLING_TIME);
#endif
#ifdef	TD_ADD_SPP
	Spp_Tx_dab();
#endif
    while(1)
    {
        memset(msg, 0x00, sizeof(msg));
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        clear_wdt();
		dab_reset_common(msg[0], msg[1]);

		switch(msg[0])
        {
            case SYS_EVENT_DEL_TASK:
				ui_show_num(DISP_LEVEL_NORMAL, 0xff);
				free_fun((void**)&UART_POP);
				dab_puts("dab--->SYS_EVENT_DEL_TASK\n");
				tuner_Yhx_w2dab_exit();
	            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
			    #if defined(_SPECIAL_RDS_DELAY_)
			    	OSTimeDly(15);
			    #endif
					timer_unreg_func(dab_timer);
	                dab_dac_channel_off();
					dab_puts("dab_SYS_EVENT_DEL_TASK\n");
					os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case MSG_AUX_INIT:
	            dab_puts(">MSG_dab_INIT\n");
	            dab_info_init();
	            break;

			case MSG_AUX_TIMER:
				//dab_puts(">MSG_dab_TIMER\n");
				break;
			case MSG_FM_AUTO_SAVE://搜台
			    ui_show_num(DISP_LEVEL_NORMAL, 0xff);
				dab_select_flag=0;
				if((DAB_DISP_TEPY!=dab_disp_downinfo)&&(DAB_DISP_TEPY!=dab_disp_serach)&&(DAB_DISP_TEPY!=dab_disp_total))//下载数据和进度时不能搜台
				{
					DAB_NOT_SEEK=1;
					dab_puts("dab_low_power\n");
                #if defined( bole_100_err_set)
                    dab_sys_power_flag=1;
                #endif
					W2dab_CMD_SET_SEEK();
				}
				break;
			case MSG_FM_PREV_SEEK://上一曲
			#if defined(_ENABLE_DAB_SPECIAL_MUTE)
			    sys_mute_ctrl(ENABLE, 1000);
			#endif
			    ui_show_num(DISP_LEVEL_NORMAL, 0xff);
			    dab_select_flag=0;
			    dab_puts("dab_PREV_SEEK\n");
				if(radio_total)
				{
					RADIO_CURRENT_STATION--;
					if(RADIO_CURRENT_STATION<0)
					{
						RADIO_CURRENT_STATION=(radio_total-1);
					}
					DAB_DISP_TEPY=dab_disp_name;//显示节目信息

					RADIO_AUTO_PLAY_TIME=15;// 15后电台自动播放

					DAB_DISP_DLS_SIZE=0;//清除dls缓存信息长度
					DAB_DISP_PLAYPAUSE=0;//清除播放标志
      				#if defined(DAB_CURRENT_STATION_MEMORY)
						setup.DAB_CURRENT_STATION=RADIO_CURRENT_STATION;
 					#endif

					dab_printf("radio_appoint-----%d\n",RADIO_CURRENT_STATION);
					//W2dab_get_appoint_radio_info_cmd(RADIO_CURRENT_STATION);//获取指定节目信息
					//W2dab_set_appoint_radio_play_cmd(RADIO_CURRENT_STATION);//播放
				}
				break;
			case MSG_FM_NEXT_SEEK://下一曲
			#if defined(_ENABLE_DAB_SPECIAL_MUTE)
			    sys_mute_ctrl(ENABLE, 1000);
			#endif
			   ui_show_num(DISP_LEVEL_NORMAL, 0xff);
			   dab_select_flag=0;
			    dab_puts("dab_NEXT_SEEK\n");
				if(radio_total)
				{
					RADIO_CURRENT_STATION++;
					if(RADIO_CURRENT_STATION>(radio_total-1))
					{
						RADIO_CURRENT_STATION=0;
					}
					DAB_DISP_TEPY=dab_disp_name;//显示节目信息
					RADIO_AUTO_PLAY_TIME=15;//
					DAB_DISP_DLS_SIZE=0;//清除dls缓存信息长度
					DAB_DISP_PLAYPAUSE=0;//清除播放标志
					dab_printf("radio_appoint-----%d\n",RADIO_CURRENT_STATION);
					//W2dab_get_appoint_radio_info_cmd(RADIO_CURRENT_STATION);//获取指定节目信息
					//W2dab_set_appoint_radio_play_cmd(RADIO_CURRENT_STATION);//播放

				#if defined(DAB_CURRENT_STATION_MEMORY)
					setup.DAB_CURRENT_STATION=RADIO_CURRENT_STATION;
				#endif
				}
				break;
			case MSG_FM_BAND://查阅信息
				dab_select_flag=0;
			 	dab_puts("dab_BAND\n");
				if(DAB_DISP_TEPY== dab_disp_name)
				{
					RADIO_INFO_DISP_TIME=0;
					DAB_DISP_TEPY=dab_disp_pty;
				}
				else if(DAB_DISP_TEPY==dab_disp_pty)
				{
					RADIO_INFO_DISP_TIME=0;
					DAB_DISP_TEPY=dab_disp_boder;
				}
				else if(DAB_DISP_TEPY==dab_disp_boder)
				{
					RADIO_INFO_DISP_TIME=0;
					DAB_DISP_TEPY=dab_disp_bit;
				}
				else if(DAB_DISP_TEPY==dab_disp_bit)
				{
					RADIO_INFO_DISP_TIME=0;
					DAB_DISP_TEPY=dab_disp_signal;
				}
				else if(DAB_DISP_TEPY==dab_disp_signal)
				{
					RADIO_INFO_DISP_TIME=0;
					DAB_DISP_TEPY=dab_disp_dls;
					DAB_DISP_DLS_POINT=0;
				}
				else if(DAB_DISP_TEPY==dab_disp_dls)
				{
					RADIO_INFO_DISP_TIME=0;
					DAB_DISP_TEPY=dab_disp_name;
					#if defined(DAB_NAME_USER_16BYTE)
					DAB_DISP_NAME_POINT=0;
					RADIO_INFO_DISP_TIME_NOW=0;
					#endif
				}
			break;
			#if defined(DAB_NUM_SAVE_STATION)
			case MSG_FM_M1 ... MSG_FM_M6://数字播放选台
			#if defined(_ENABLE_DAB_SPECIAL_MUTE)
				sys_mute_ctrl(ENABLE, 1000);
			#endif
				ui_show_num(DISP_LEVEL_NORMAL, (msg[0]-MSG_FM_M1+1));

			    if((setup.DAB_NUM_STATION[msg[0]-MSG_FM_M1]<(radio_total-1))&&(radio_total!=0))
			    {
					if(setup.DAB_NUM_STATION[msg[0]-MSG_FM_M1]!=RADIO_CURRENT_STATION)
					{
						RADIO_CURRENT_STATION=setup.DAB_NUM_STATION[msg[0]-MSG_FM_M1];
						DAB_DISP_TEPY=dab_disp_name;//显示节目信息
						RADIO_AUTO_PLAY_TIME=15;//
						DAB_DISP_DLS_SIZE=0;//清除dls缓存信息长度
						DAB_DISP_PLAYPAUSE=0;//清除播放标志
					#if defined(DAB_CURRENT_STATION_MEMORY)
						setup.DAB_CURRENT_STATION=RADIO_CURRENT_STATION;
					#endif
					}
			    }
			break;
			case MSG_FM_S1 ... MSG_FM_S6://数字存台
			    ui_show_num(DISP_LEVEL_NORMAL, (msg[0]-MSG_FM_S1+1));
				setup.DAB_NUM_STATION[msg[0]-MSG_FM_S1]=RADIO_CURRENT_STATION;
				DAB_DISP_TEPY=dab_disp_save_number;
			break;
			#endif
            default:
	            task_msg_default(msg[0], msg[1]);
	            break;
        }
    }
}

static void dab_task_init(void *priv)
{
	RADIO_SEEK_STATION=0;
	RADIO_CURRENT_STATION_INFO_OK=0;
	RADIO_download_OK=0;
#if defined(dab_select_station)
	dab_select_flag=0;
#endif
#if defined(_ENABLE_RADIO_RDS_)
#if defined(_DAB_MODLE_RDS_FIRST_)
    dab_rds_af_save=setup.rds.af;
    dab_rds_ta_save=setup.rds.ta;
    setup.rds.af=0;
    setup.rds.ta=0;
#endif
#endif
	dab_puts("dab_task_init\n");

    u32 err;
    err = os_task_create_ext(dab_task,
						(void*) 0,
						TaskLineinPrio,
						10
					#if OS_TIME_SLICE_EN > 0
			            ,1
					#endif
            			,DAB_TASK_NAME
            			,LINEIN_TASK_STACK_SIZE);

    if (OS_NO_ERR == err)
    {
		dab_puts("333333---dab_key_register\n");
        key_msg_register(DAB_TASK_NAME, KEY_PROJECT_DAB);
    }
}

static void dab_task_exit(void)
{
	dab_puts("dab_task_exit\n");
	key_msg_register(NULL, KEY_PROJECT_MAX);
#if defined(_ENABLE_RADIO_RDS_)
#if defined(_DAB_MODLE_RDS_FIRST_)
	setup.rds.af=dab_rds_af_save;
	setup.rds.ta=dab_rds_ta_save;
#endif
#endif
    if (os_task_del_req(DAB_TASK_NAME) != OS_TASK_NOT_EXIST)
    {

        os_taskq_post_event(DAB_TASK_NAME, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        } while (os_task_del_req(DAB_TASK_NAME) != OS_TASK_NOT_EXIST);
        dab_puts("del_dab_task: succ\n");
		RADIO_SEEK_STATION=0;
		RADIO_CURRENT_STATION_INFO_OK=0;
		RADIO_download_OK=0;
		#if defined(dab_select_station)
			dab_select_flag=0;
		#endif
    }
}

TASK_REGISTER(dab_task_info) =
{
    .name = DAB_TASK_NAME,
    .init = dab_task_init,
    .exit = dab_task_exit,
};

#endif

