#include "sdk_cfg.h"
#include "bluetooth/avctp_user.h"
#include "play_sel/play_sel.h"
#include "dec/decoder_phy.h"
#include "dac/dac_api.h"
#include "dac/dac.h"
#include "dac/eq_api.h"
#include "sys_detect.h"
#include "dac/eq.h"
#include "encode/encode.h"
#include "vm/vm.h"
#include "vm/vm_api.h"
#include "rcsp/rcsp_interface.h"
#include "bluetooth/nfc_api.h"
#include "disp/disp.h"
#include "key_drv.h"
#include "bt_key_deal.h"
#include "bt.h"
#include "timer.h"
#include "echo/echo_api.h"
#include "echo/echo_deal.h"
#include "setup.h"
#include "audio_drv.h"

#if defined(_TALKING_VOICE_DISPLAY_)
#include "ui_api.h"
#endif

u8 fast_test = 0;

#define BT_REC_Hz  120000000L





RECORD_OP_API * rec_bt_api = NULL;
REVERB_API_STRUCT * bt_reverb = NULL;
static uint8 play_phone_income_tone_flag = FALSE;


struct user_ctrl{
    u8 is_phone_number_come;
    u8 play_phone_number_flag;
    u8 phone_prompt_tone_playing_flag;
    u8 phone_num_len;
    u8 income_phone_num[30];
    u8 user_cmd_param_buf[30];
    u8 bt_eq_mode;
    void* tone_by_file_name;
    //u8 auto_connection_counter;
    u16 auto_connection_counter;
    u16 auto_shutdown_cnt;
	u8 esco_close_echo_flag;
	u8 wait_app_updata_flag;
	u8 discover_control_cnt;
	u8 auto_connection_stereo;
};

typedef struct{
	uint8 now_status;
	uint8 connect_status;
	uint8 call_status;
	uint8 music_status;
	uint8 sco_status;
}bt_status_t;

typedef struct{
	uint8 length;
	uint8 *buffer;
}bt_number_t;

typedef struct{
	uint8 bt_icon_state;
	bt_status_t bt_status;
	bt_number_t bt_number;
}bt_engine_ctrl;

static bt_engine_ctrl bt_engine;

#ifdef HAVE_MALLOC
_PLAY_SEL_PARA *psel_p = NULL;
#else
_PLAY_SEL_PARA __psel_p;
#define psel_p (&__psel_p)
#endif

static struct user_ctrl __user_val sec_used(.ram1_data);
#define user_val (&__user_val)

extern tbool mutex_resource_apply(char *resource,int prio ,void (*apply_response)(), void (*release_request)());
extern u8 a2dp_get_status(void);
extern bool get_sco_connect_status(void);
extern void set_esco_sr(u16 sr);

extern char rec_file_name_last[];
extern char rec_let_last;
OS_MUTEX tone_manage_semp;

#if defined(PHONE_NUMBER_OVER_PLAY_PHONE_OUTINCOME_TONE)
u8 PHONE_NUMBER_FLAG=0;
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
#if defined(__EW_DEBUG__)
static void bt_status_printf(void)
{
	STATUS_FOR_USER now_status		= (STATUS_FOR_USER)bt_engine.bt_status.now_status;
	STATUS_FOR_USER connect_status	= (STATUS_FOR_USER)bt_engine.bt_status.connect_status;
	BT_CALL_IND_STA call_status		= (BT_CALL_IND_STA)bt_engine.bt_status.call_status;
	BT_MUSIC_STATE music_status		= (BT_MUSIC_STATE)bt_engine.bt_status.music_status;
	uint8 sco_status				= bt_engine.bt_status.sco_status;


	bt_red_output("%s ---> start\n", __FUNCTION__);
	bt_printf("%s--->bt_now_status:%d, ", __FUNCTION__, now_status);
		 if(now_status == BT_STATUS_POWER_ON)			bt_puts("BT_STATUS_POWER_ON\n");			/*上电*/
    else if(now_status == BT_STATUS_POWER_OFF)			bt_puts("BT_STATUS_POWER_OFF\n");			/*关电*/
    else if(now_status == BT_STATUS_INIT_OK)			bt_puts("BT_STATUS_INIT_OK\n");				/*初始化完成*/
    else if(now_status == BT_STATUS_FIRST_CONNECTED)	bt_puts("BT_STATUS_FIRST_CONNECTED\n");		/*连接成功*/
    else if(now_status == BT_STATUS_SECOND_CONNECTED)	bt_puts("BT_STATUS_SECOND_CONNECTED\n");	/*连接成功*/
    else if(now_status == BT_STATUS_FIRST_DISCONNECT)	bt_puts("BT_STATUS_FIRST_DISCONNECT\n");	/*断开连接*/
    else if(now_status == BT_STATUS_SECOND_DISCONNECT)	bt_puts("BT_STATUS_SECOND_DISCONNECT\n");	/*断开连接*/
    else if(now_status == BT_STATUS_PHONE_INCOME)		bt_puts("BT_STATUS_PHONE_INCOME\n");		/*来电*/
    else if(now_status == BT_STATUS_PHONE_NUMBER)		bt_puts("BT_STATUS_PHONE_NUMBER\n");		/*来电话好吗*/
    else if(now_status == BT_STATUS_PHONE_OUT)			bt_puts("BT_STATUS_PHONE_OUT\n");			/*打出电话*/
    else if(now_status == BT_STATUS_PHONE_ACTIVE)		bt_puts("BT_STATUS_PHONE_ACTIVE\n");		/*接通电话*/
    else if(now_status == BT_STATUS_PHONE_HANGUP)		bt_puts("BT_STATUS_PHONE_HANGUP\n");		/*挂断电话*/
    else if(now_status == BT_STATUS_BEGIN_AUTO_CON)		bt_puts("BT_STATUS_BEGIN_AUTO_CON\n");		/*发起回连*/
    else if(now_status == BT_STATUS_MUSIC_SOUND_COME)	bt_puts("BT_STATUS_MUSIC_SOUND_COME\n");	/*库中加入auto mute判断音乐播放开始*/
    else if(now_status == BT_STATUS_MUSIC_SOUND_GO)		bt_puts("BT_STATUS_MUSIC_SOUND_GO\n");		/*库中加入auto mute判断音乐播放暂停*/
    else if(now_status == BT_STATUS_RESUME)				bt_puts("BT_STATUS_RESUME\n");				/*后台有效，手动切回蓝牙*/
    else if(now_status == BT_STATUS_RESUME_BTSTACK)		bt_puts("BT_STATUS_RESUME_BTSTACK\n");		/*后台有效，后台时来电切回蓝牙*/
    else if(now_status == BT_STATUS_SUSPEND)			bt_puts("BT_STATUS_SUSPEND\n");				/*蓝牙挂起，退出蓝牙*/
    else if(now_status == BT_STATUS_TONE_BY_FILE_NAME)	bt_puts("BT_STATUS_TONE_BY_FILE_NAME\n");	/*直接使用文件名播放提示音*/
	else bt_puts("no default\n");

	bt_printf("%s--->bt_connect_status:%d, ", __FUNCTION__, connect_status);
    	 if(connect_status == BT_STATUS_INITING)		bt_puts("BT_CONNECT_INITING\n");			/*正在初始化*/
    else if(connect_status == BT_STATUS_WAITINT_CONN)	bt_puts("BT_CONNECT_WAITINT_CONN\n");		/*等待连接*/
    else if(connect_status == BT_STATUS_AUTO_CONNECTINT)bt_puts("BT_CONNECT_AUTO_CONNECTINT\n");	/*正在回连*/
    else if(connect_status == BT_STATUS_CONNECTING)		bt_puts("BT_CONNECT_CONNECTING\n");			/*已连接，没有电话和音乐在活动*/
    else if(connect_status == BT_STATUS_TAKEING_PHONE)	bt_puts("BT_CONNECT_TAKEING_PHONE\n");		/*正在电话*/
    else if(connect_status == BT_STATUS_PLAYING_MUSIC)	bt_puts("BT_CONNECT_PLAYING_MUSIC\n");		/*正在音乐*/
	else bt_puts("no default\n");

	bt_printf("%s--->bt_call_status:%d, ", __FUNCTION__, call_status);
		 if(call_status == BT_CALL_BATTERY_CHG)			bt_puts("BT_CALL_BATTERY_CHG\n");			/*电池电量改变*/
    else if(call_status == BT_CALL_SIGNAL_CHG)			bt_puts("BT_CALL_SIGNAL_CHG\n");			/*网络信号改变*/
	else if(call_status == BT_CALL_VOL_CHANGED)			bt_puts("BT_CALL_VOL_CHANGED\n");			/*音量改变*/
    else if(call_status == BT_CALL_INCOMING)			bt_puts("BT_CALL_INCOMING\n");				/*电话打入*/
    else if(call_status == BT_CALL_OUTGOING)			bt_puts("BT_CALL_OUTGOING\n");				/*电话打出*/
    else if(call_status == BT_CALL_ACTIVE)				bt_puts("BT_CALL_ACTIVE\n");				/*接通电话*/
    else if(call_status == BT_CALL_HANGUP)				bt_puts("BT_CALL_HANGUP\n");				/*电话挂断*/
	else if(call_status == BT_CALL_ALERT)				bt_puts("BT_CALL_ALERT\n");					/*远端reach*/
	else bt_puts("no default\n");

	bt_printf("%s--->music_status:%d, ", __FUNCTION__, music_status);
		 if(music_status == BT_MUSIC_STATUS_IDLE)		bt_puts("BT_MUSIC_STATUS_IDLE\n");			/*蓝牙音乐空闲状态*/
    else if(music_status == BT_MUSIC_STATUS_STARTING)	bt_puts("BT_MUSIC_STATUS_STARTING\n");		/*蓝牙音乐播放状态*/
	else if(music_status == BT_MUSIC_STATUS_SUSPENDING)	bt_puts("BT_MUSIC_STATUS_SUSPENDING\n");	/*蓝牙音乐暂停状态*/
	else bt_puts("no default\n");

	bt_printf("%s--->sco_status:%d, ", __FUNCTION__, sco_status);
	if(sco_status)										bt_puts("BT_SCO_CONNECT\n");				/*蓝牙SCO 链路连接*/
    else 												bt_puts("BT_SCO_DISCONNECT\n");				/*蓝牙SCO 链路断开*/

	bt_red_output("%s ---> end\n", __FUNCTION__);
}
#else
#define bt_status_printf(...)
#endif

static void bt_ap_msg(int msg, void *param)
{
	char *task_name = BT_TASK_NAME;

	if(!strcmp(keymsg_task_name, BT_TASK_NAME))
	{
		os_taskq_post_msg(task_name, 2,  msg, param);
	}
}
void bt_update_close_echo()
{
    echo_exit_api((void**)&bt_reverb);
	exit_rec_api(&rec_bt_api);//exit rec when esco change

}
u8 get_esco_st(u8 sw)//esco call back fun, get esco status, 0 off, 1 on
{
#if BT_KTV_EN
	int msg_echo_start = MSG_ECHO_START;
	int msg_echo_stop = MSG_ECHO_STOP;

	exit_rec_api(&rec_bt_api);//exit rec when esco change
	ladc_pga_gain(2, 0);//2:mic_channel, 0:gain

	if(sw)
	{
		/* puts("***iphone ktv start\n"); */
	}
	else
	{
		puts("***iphone ktv stop\n");
		echo_msg_deal_api((void**)&bt_reverb, &msg_echo_stop);
		os_taskq_post_event("btmsg", 1, MSG_ECHO_START);
		/* echo_msg_deal_api((void**)&bt_reverb, &msg_echo_start); */
	}
	return 1;

#else
	exit_rec_api(&rec_bt_api);//exit rec when esco change

#if	ECHO_EN
	if(sw)//esco on
	{
		echo_exit_api((void **)&bt_reverb);//echo not support when esco on
	}
#endif
	return 0;
#endif//end of BT_KTV_EN
}

u8 get_bt_eq_mode(void)
{
    return user_val->bt_eq_mode;
}

void set_bt_eq_mode(int new_mode)
{
#if EQ_EN
	if(new_mode > 5)
	{
		new_mode = 0;
	}
	user_val->bt_eq_mode = new_mode;
	eq_mode_sw(new_mode);
#endif
}

void create_bt_msg_task(char *task_name)
{
	bt_printf("create_bt_msg_task:%s\n", task_name);

    u32 err;
    err = os_task_create(TaskBtMsgStack,(void *) 0, TaskBTMSGPrio, 30, task_name);
    if(OS_NO_ERR != err)
    {
        bt_puts("create bt msg fail\n");
        while(1);
    }
}

/*提示音开始，资源管理线程接口，用户不能调用*/
static void tone_mutex_play()
{
#if(BT_MODE!=NORMAL_MODE)
	 return;
#endif
	exit_rec_api(&rec_bt_api);//exit rec when src change
    bt_puts("tone_mutex_play\n");
    play_sel("resourse",psel_p->let,psel_p);
    if(psel_p->max_file_number > 3) //报号开始
    {
        user_val->play_phone_number_flag = 1;
    }

    free_fun((void **)&psel_p);
    os_sem_post(&tone_manage_semp);
}

/*资源管理线程接口，用户不能调用*/
static void tone_mutex_stop()
{
    play_sel_stop();
	printf("tone_mutex_stop\n");
    dac_channel_on(BT_CHANNEL, FADE_ON);
    if( user_val->play_phone_number_flag == 1)
    {   //来电报号提示音播完后要播要循环播提示声
        user_val->play_phone_number_flag = 0;
        update_bt_current_status(NULL, BT_STATUS_PHONE_INCOME, 0);
    }
}

u8 wait_phone_number_play = 0;
/*获取到来电电话号码，用于报号功能*/
void hook_hfp_incoming_phone_number(char* number, u16 length)
{
	if(length!=0){
		puts("hook_hfp_incoming_phone_number:");
		put_buf((u8 *)number,length);
	}
    if(user_val->is_phone_number_come == 0)
    {
        user_val->is_phone_number_come = 1;
        wait_phone_number_play = 0;
		if(length > sizeof(user_val->income_phone_num))
		{
			length = sizeof(user_val->income_phone_num);
		}
        user_val->phone_num_len = length;
		
		if(number[0] < 0x30)//修改OPPOR手机来电有+号显示
		{
			memcpy(user_val->income_phone_num, &number[1], length);
		}
		else
		{
			memcpy(user_val->income_phone_num, number, length);
		}
		
		if(!is_bt_in_background_mode())
		{
            wait_phone_number_play = 1;
			if(get_call_status() == BT_CALL_INCOMING)
			{
				update_bt_current_status(NULL, BT_STATUS_PHONE_NUMBER, 0);
			}
			else if(get_call_status() == BT_CALL_ACTIVE)
			{
				update_bt_current_status(NULL, BT_STATUS_PHONE_ACTIVE, 0);
			}
		}
		bt_engine.bt_number.length = length;
		bt_engine.bt_number.buffer = user_val->income_phone_num;
		bt_ap_msg(MSG_BT_INCOM_NUM, &bt_engine.bt_number);
    }
}

/*封装函数根据状态选择提示音*/
void user_ctrl_prompt_tone_play(u8 status,void *ptr)
{
    u8* buf_addr = NULL;
    u32 need_buf;
#if(BT_MODE!=NORMAL_MODE)
	 return;
#endif
    if((bt_power_is_poweroff_post() && (status != BT_STATUS_FIRST_DISCONNECT))
			||is_bt_in_background_mode())  //background not play tone
    {
        return;
    }
    bt_puts("play_tone\n");
    if(get_remote_test_flag()&&(status == BT_STATUS_FIRST_CONNECTED))   //测试盒测试不播放提示音
    {
        return;
    }

	if((going_to_pwr_off == 1) && (status != BT_STATUS_POWER_OFF))   //软关机的时候只播关机提示音
	{
        bt_puts("-----------powering off---------\n");
		return;
	}

    os_sem_pend(&tone_manage_semp,0);

    if(play_sel_busy())
    {
        play_sel_stop();
        bt_puts("------------play_sel_stop----------\n");
    }

    need_buf = sizeof(_PLAY_SEL_PARA) + 30*4;//30 file_name*sizeof(int)
    buf_addr = malloc_fun(buf_addr, need_buf, 0);
    if(buf_addr == NULL)
    {
        return ;
    }
	memset(buf_addr, 0x00, need_buf);
    psel_p = (_PLAY_SEL_PARA *)buf_addr;//malloc_fun(psel_p ,sizeof(_PLAY_SEL_PARA),0);
    buf_addr+= sizeof(_PLAY_SEL_PARA);
    psel_p->file_name = (void *)(buf_addr);//malloc_fun(psel_p->file_name,30*4,0); //电话号码至少需要11，国际号码可能很多位

	switch(status)
    {
        case BT_STATUS_POWER_ON:
            psel_p->max_file_number = 1;
            psel_p->delay = 10;
            psel_p->rpt_time = 1;
            //psel_p->file_name[0] = BPF_BT_MP3;

		#if BT_HID_INDEPENDENT_MODE || USER_SUPPORT_PROFILE_HID
            if(__get_hid_independent_flag())
            {
                //HID独立提示音
                psel_p->file_name[0] = BPF_INIT_HID_MP3;
            }else
		#endif
            {
                psel_p->file_name[0] = BPF_BT_MP3;
            }

            break;

        case BT_STATUS_POWER_OFF:
            psel_p->max_file_number = 1;
            psel_p->delay = 10;
            psel_p->rpt_time = 1;
            psel_p->file_name[0] = BPF_POWER_OFF_MP3;
            break;

        case BT_STATUS_RESUME:
			ew_puts("BT_STATUS_RESUME\n");
            psel_p->max_file_number = 2;
            psel_p->delay = 10;
            psel_p->rpt_time = 1;
            psel_p->file_name[0] = BPF_BT_MP3;

            if(get_curr_channel_state() != 0)
            {
				ew_puts("BT_STATUS_RESUME111\n");
                psel_p->file_name[1] = BPF_CONNECT_MP3;
            }
            else
            {
				ew_puts("BT_STATUS_RESUME222\n");
                psel_p->file_name[1] = BPF_DISCONNECT_MP3;
            }
            break;
        case BT_STATUS_FIRST_CONNECTED:
        case BT_STATUS_SECOND_CONNECTED:
        #if !defined(_DISABLE_BT_CONNECT_VOICE_)
			bt_power_max(0xFF);
            psel_p->max_file_number = 1;
            psel_p->delay = 10;
            psel_p->rpt_time = 1;
		#if BT_HID_INDEPENDENT_MODE || USER_SUPPORT_PROFILE_HID
            if(__get_hid_independent_flag())
            {   //HID独立提示音
                psel_p->file_name[0] = BPF_CONN_HID_MP3;
            }else
		#endif
            {
			#if BT_TWS
               if(BT_CURRENT_CONN_PHONE==get_bt_current_conn_type())
               {
				   puts("------BT_CURRENT_CONN_PHONE\n");
                   psel_p->file_name[0] = BPF_CONNECT_ONLY_MP3;
               }
               else if(BT_CURRENT_CONN_STEREO_MASTER_PHONE==get_bt_current_conn_type())
               {
				   puts("--------BT_CURRENT_CONN_STEREO_MASTER_PHONE\n");
                   psel_p->file_name[0] = BPF_CONNECT_LEFT_MP3;
               }
               else if((BT_CURRENT_CONN_STEREO_MASTER==get_bt_current_conn_type())||(BT_CURRENT_CONN_STEREO_PHONE_MASTER==get_bt_current_conn_type()))
               {
				   puts("--------BT_CURRENT_CONN_STEREO_MASTER\n");
                   psel_p->file_name[0] = BPF_CONNECT_LEFT_MP3;
               }
               else if(BT_CURRENT_CONN_STEREO_SALVE==get_bt_current_conn_type())
               {
				   puts("--------BT_CURRENT_CONN_STEREO_SALVE\n");
                   psel_p->file_name[0] = BPF_CONNECT_RIGHT_MP3;
               }
               else
               {
				   puts("--------BPF_CONNECT_ONLY_MP3\n");
                   psel_p->file_name[0] = BPF_CONNECT_ONLY_MP3;
               }
			#else
                psel_p->file_name[0] = BPF_CONNECT_MP3;
			#endif
            }
		#endif
            break;
        case BT_STATUS_FIRST_DISCONNECT:
        case BT_STATUS_SECOND_DISCONNECT:
		#if !defined(_DISABLE_BT_DISCONNECT_VOICE_)
            psel_p->max_file_number = 1;
            psel_p->delay = 10;
            psel_p->rpt_time = 1;
		#if BT_HID_INDEPENDENT_MODE || USER_SUPPORT_PROFILE_HID
            if(__get_hid_independent_flag())
            {   //HID独立提示音
                psel_p->file_name[0] = BPF_DISCON_HID_MP3;
            }
			else
		#endif
            {
			#if BT_TWS
               if(BT_CURRENT_CONN_PHONE==get_bt_current_conn_type())
			   {
				   puts("----disconn BT_CURRENT_CONN_STEREO_MASTER\n");
                   psel_p->file_name[0] =BPF_DISCONNECT_MP3;
			   }
			   else
			   {
				   puts("-----disconn BPF_DISCONNECT_MP3\n");
                   psel_p->file_name[0] = BPF_DISCONNECT_MP3;
			   }
			#else
			#if defined(_DISABLE_BT_DISCONNECT_MP3)
				if(ACC_CHECK())
			#endif		
                psel_p->file_name[0] = BPF_DISCONNECT_MP3;
			#endif
            }
		#endif
            break;
        case BT_STATUS_TONE_BY_FILE_NAME:
            psel_p->max_file_number = 1;
            psel_p->delay = 10;
            psel_p->rpt_time = 1;
            psel_p->file_name[0] = user_val->tone_by_file_name;
			if(ptr)
			{
				psel_p->let = 'B';//(u8)*(u8*)ptr;
			}
			printf("FILE_NAME = %s\n",psel_p->file_name[0]);
			printf("psel_p->let = %c\n",psel_p->let);
            break;
        case BT_STATUS_PHONE_INCOME:
			ew_puts("BT_STATUS_PHONE_INCOME\n");
            psel_p->max_file_number = 1;
            psel_p->delay = 10;
            psel_p->rpt_time = 0;
            user_val->play_phone_number_flag = 0;
            user_val->phone_prompt_tone_playing_flag = 1;
			ew_printf("get_call_status:%d\n",get_call_status());
			if(get_call_status()==BT_CALL_INCOMING)
			{
				ew_puts("BT_STATUS_PHONE_INCOME111\n");
				psel_p->rpt_time = 0;
	            psel_p->file_name[0] = BPF_WAIT_MP3;
			}
			else if(get_call_status()==BT_CALL_OUTGOING)
			{
				ew_puts("BT_STATUS_PHONE_INCOME222\n");
				psel_p->rpt_time = 2;
	            psel_p->file_name[0] = BPF_RING_MP3;
			}
			else
			{
				ew_puts("BT_STATUS_PHONE_INCOME333\n");
				//g_psel.alive =1;
				play_sel_stop();
				return;
			}
            break;
	#if defined ( BT_PHONE_NUMBER)
		case BT_STATUS_PHONE_NUMBER:
			ew_puts("BT_STATUS_PHONE_NUMBER\n");
			if(user_val->is_phone_number_come)	 //有电话号码就播放
			{
				ew_puts("BT_STATUS_PHONE_NUMBER111\n");
				int i;
				psel_p->max_file_number=0;
			#if defined(_GL_BT_PHONE_DELAY_)
			    psel_p->delay = 50;
			#else
				psel_p->delay = 20;
			#endif
			#if defined(_RINGING_THREE_NUMBER_)
				psel_p->rpt_time = 3;
			#elif defined(_RINGING_ONE_NUMBER_)
				psel_p->rpt_time = 1;//报一次
			#else
				psel_p->rpt_time = 0;//一直报号
			#endif
				for(i = 0;i < user_val->phone_num_len; i++)
				{
					if((user_val->income_phone_num[i] >= 0x30) && (user_val->income_phone_num[i] <= 0x39))
					{
						psel_p->file_name[psel_p->max_file_number] = tone_number_get_name(user_val->income_phone_num[i] - 0x30);
						psel_p->max_file_number++;
					}
				}
				user_val->phone_prompt_tone_playing_flag = 1;
				break;//break要放在if里面，不播放直接free了
			}
	#endif
        default:
			//free_fun((void **)&psel_p->file_name);
            free_fun((void **)&psel_p);
            return;
    }

    mutex_resource_apply("tone",4,tone_mutex_play,tone_mutex_stop);
    os_sem_pend(&tone_manage_semp,0);
    os_sem_post(&tone_manage_semp);
}
static void bt_prompt_play_by_name(char *file_name, char *let)
{
#ifdef BT_PROMPT_EN
    puts("bt_prompt_play\n");
    user_val->tone_by_file_name = file_name;
	user_ctrl_prompt_tone_play(BT_STATUS_TONE_BY_FILE_NAME, let);
#endif
}

#if BT_TWS
u8 get_stereo_salve(u8 get_type)
{
	u8 stereo_info;
	if(0 == get_type)
	{
	   return BD_CONN_MODE;
	}
	else
	{
        if (-VM_READ_NO_INDEX == vm_read_api(VM_BT_STEREO_INFO,&stereo_info))
        {
     	   return BD_CONN_MODE;
        }
       	printf("-----is_stereo_salve=0x%x\n",stereo_info );
        if(0xaa ==stereo_info)//′ó?ú
     	{
     		return BD_PAGE_MODE;
     	}
	}
	return BD_CONN_MODE;
}
#endif
void bt_stereo_poweroff_togerther()
{
#if BT_TWS
#if BT_TWS_POWEROFF_TOGETHER
   	if(is_check_stereo_slave())
   	{
       	stereo_slave_cmd_handle(MSG_POWER_OFF,0xff);
   	}
   	else
   	{
       	stereo_host_cmd_handle(MSG_POWER_OFF,0);
   	}
	os_time_dly(5);
#endif
#endif
}
void bt_tws_delete_addr()
{
#if BT_TWS
    delete_stereo_addr();
   	if(is_check_stereo_slave())
   	{
       	stereo_slave_cmd_handle(MSG_BT_TWS_DELETE_ADDR,0);
   	}
   	else
   	{
       	stereo_host_cmd_handle(MSG_BT_TWS_DELETE_ADDR,0);
   	}
#endif

}

static OS_MUTEX spp_mutex;
int spp_mutex_init(void)
{
    return os_mutex_create(&spp_mutex);
}
static inline int spp_mutex_enter(void)
{
    os_mutex_pend(&spp_mutex,0);
    return 0;
}
static inline int spp_mutex_exit(void)
{
    return os_mutex_post(&spp_mutex);
}
int spp_mutex_del(void)
{
    return os_mutex_del(&spp_mutex,OS_DEL_ALWAYS);
}

/*发送串口数据的接口，发送完再返回*/
#ifdef	TD_ADD_SPP
extern u8 spp_connect_falg;
#endif


u32 bt_spp_send_data(u8 *data,u16 len)
{
    u32 err;
#ifdef	TD_ADD_SPP
    //if(get_bt_connect_status()<=BT_STATUS_AUTO_CONNECTINT)
    if(spp_connect_falg==0)
    	return 4;//蓝牙没连接上 不发送SPP命令
#endif
    spp_mutex_enter();
    err = user_send_cmd_prepare(USER_CTRL_SPP_SEND_DATA,len,data);
    spp_mutex_exit();
	printf("=%d\n",err);
	if(err==0)
	{
		printf("spp->-");
		for(u8 i=0;i<len;i++)
		{
			printf(" %x ",data[i]);
		}
		printf("--\n");
	}
    return err;
}

void bt_spp_disconnect(void)
{
    spp_mutex_enter();
    user_send_cmd_prepare(USER_CTRL_SPP_DISCONNECT,0,0);
    spp_mutex_exit();
}

static void bt_status_detect(uint8 flag)
{
	bt_status_t bt_status;

	bt_status.now_status 	 = get_bt_newest_status();
	bt_status.connect_status = get_bt_connect_status();
	bt_status.call_status 	 = get_call_status();
	bt_status.music_status 	 = a2dp_get_status();
	bt_status.sco_status	 = get_sco_connect_status();

	if(flag || memcmp(&bt_status, &bt_engine.bt_status, sizeof(bt_status_t)))
	{
		bt_engine.bt_status = bt_status;
//		bt_status_printf();
		bt_ap_msg(MSG_BT_STATE, &bt_engine.bt_status);
	}
}

/*开关可发现可连接的函数接口*/
static void bt_page_scan(u8 enble)
{
    bt_puts("bt_page_scan\n");
	if(enble)
	{
		if(!is_1t2_connection())
		{
		    bt_puts("--->111<---\n");
			if(enble==3)
			{
				user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
				user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE_KEY,0,NULL);
			}
			else if(enble==2)//通过按键开启page_scan
			{
			    ew_puts("bt_page_scan3\n");
				user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE_KEY,0,NULL);
				user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE_KEY,0,NULL);
			}
			else
			{
			    bt_puts("--->222<---\n");
				user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
				user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE,0,NULL);
			}
		}
  	}
  	else
  	{
  	    bt_puts("--->333<---\n");
     	user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
     	user_send_cmd_prepare(USER_CTRL_WRITE_CONN_DISABLE,0,NULL);
  	}
}
#if defined(vbar_disp_bt_music)
extern u8 vbar_bt_music ;
#endif
#if defined(HuiNuo_6651D_Copy)
u8 HuiNuo_6651D_flag=0;
#endif

static void btstack_status_change_deal(void *ptr)
{
#if BT_KTV_EN
	int msg_echo_start = MSG_ECHO_START;
#endif
	STATUS_FOR_USER now_status = (STATUS_FOR_USER)get_bt_newest_status();
	STATUS_FOR_USER old_status = (STATUS_FOR_USER)get_bt_prev_status();

	puts("btstack_status_change_deal\n");
	if(now_status == old_status)return;
	update_bt_prev_status(now_status);
	bt_status_detect(FALSE);

	while(get_resume_flag())os_time_dly(1);
	bt_printf("%s --->>> now_status:%d, old_status:%d\n", __FUNCTION__, now_status, old_status);
	STATUS_FOR_USER connect_status = (STATUS_FOR_USER)get_bt_connect_status();
	uint8 bt_icon_state = FALSE;
#if defined(HANGUP_DISP_HANGUP)
	if(now_status == BT_STATUS_PHONE_HANGUP)
	{
		disp_send_msg(DISP_EVENT_BT_END);
	}
#endif

	if((connect_status == BT_STATUS_CONNECTING)
	|| (connect_status == BT_STATUS_TAKEING_PHONE)
	|| (connect_status == BT_STATUS_PLAYING_MUSIC))
	{
		bt_icon_state = TRUE;
	}
	if(bt_engine.bt_icon_state != bt_icon_state)
	{
		bt_engine.bt_icon_state = bt_icon_state;
	#if defined(_DISABLE_ACC_OFF_BT_OFF_)
		if(ACC_CHECK())
	#endif
		{
			disp_send_param(DISP_EVENT_ICON_BT, bt_icon_state);
		}
	
	#if defined(_ENABLE_AUTO_A2DP_)
		if(bt_icon_state)
		{
			global.auto_a2dp = TRUE;
			global.auto_a2dp_time = 0;
		}
	#endif
	}

	if(now_status == BT_STATUS_SUSPEND)
	{
		if(old_status == 0)
		{
			puts("now_status == BT_STATUS_SUSPEND\n");
			user_send_cmd_prepare(USER_CTRL_START_CONNECTION,0,NULL);
			return;
		}
	}

	if(old_status == BT_STATUS_SUSPEND)
    {
        if((now_status != BT_STATUS_RESUME)
         &&(now_status != BT_STATUS_RESUME_BTSTACK))
        {
        	puts("old_status == BT_STATUS_SUSPEND\n");
            if((now_status == BT_STATUS_FIRST_CONNECTED)
			 ||(now_status == BT_STATUS_SECOND_CONNECTED))
			{   //当蓝牙处于后台连接成功时给main发送事件通知。用户自行选择处理方式
                os_taskq_post_event(MAIN_TASK_NAME, 1, SYS_EVENT_BT_BACKCON);
			}
            else if((now_status == BT_STATUS_FIRST_DISCONNECT)
			  	  ||(now_status == BT_STATUS_SECOND_DISCONNECT))
			{   //当蓝牙处于后台断开成功时给main发送事件通知。用户自行选择处理方式
                os_taskq_post_event(MAIN_TASK_NAME, 1, SYS_EVENT_BT_BACKDISCON);
			}
			else if(now_status == BT_STATUS_PHONE_INCOME)
			{
				play_phone_income_tone_flag = TRUE;
			}
			return;
        }
    }

	switch(now_status)
	{
		case BT_STATUS_RESUME:
			break;

		case BT_STATUS_RESUME_BTSTACK:
			ew_puts("111BT_STATUS_RESUME_BTSTACK111\n");
			user_val->is_phone_number_come = 0;
			user_send_cmd_prepare(USER_CTRL_HFP_CALL_CURRENT,0,NULL);//发命令获取电话号码
		#if defined ( BT_PHONE_NUMBER)
			if(user_val->is_phone_number_come)
				update_bt_current_status(NULL , BT_STATUS_PHONE_NUMBER,0);
		#else
			/*os_time_dly(5);
	    #if defined(_ENABLE_PHONE_INCOME_)
	    	extern u8 check_support_feature_ag(u16 temp);
	        if(!check_support_feature_ag(0x0008))//苹果手机播自己的铃声，安卓播固定提示音
	        {
	    	    printf("---------check_support_feature_ag\n");
			#if defined(_ENABLE_PHONE_OUTGOING_)
				if(!global.call_flag)
				{
					user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
					global.call_flag = FALSE;
				}
			#else
	    	    user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
			#endif
	        }
	    #else
	    	user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
	    #endif*/  // 2020.07.23屏蔽，待机打电话会进入这里，导致语音会进入来电状态，状态不对，导致待机打电话三次会死机
		#endif
			break;

		case BT_STATUS_POWER_ON:
			break;

		case BT_STATUS_INIT_OK:
			bt_puts("BT_STATUS_INIT_OK\n");
			bt_init_ok=TRUE;
		#if defined(_ENABLE_BT_ON_DELAY_)
		#if defined(_SPECIAL_LOGO_BUG_)
			OSTimeDly(400);
		#else
			OSTimeDly(300);
		#endif
		#endif
        #if defined(_DISABLE_BT_AUTO_CONNECT_)
        	if(!setup.acc_on_flag)
        	{
        		user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
        		user_send_cmd_prepare(USER_CTRL_WRITE_CONN_ENABLE,0,NULL);
        	}
        	else
        #endif
            {
    			user_send_cmd_prepare(USER_CTRL_START_CONNECTION,0,NULL);
    		#if BT_TWS
    			bd_work_mode = get_stereo_salve(TWS_SLAVE_WAIT_CON);
    		#endif
    			if(bd_work_mode == BD_PAGE_MODE)
    			{
    			#if BT_TWS
    				stereo_clear_current_search_index();
    			#endif
    			#if BT_TWS_SCAN_ENBLE
    				bt_page_scan(3);
    			#else
    			#if defined(POWER_DISCONNECT_BT_)||defined(_ENABLE_BT_ON_OFF_)
    				if((setup.power_off_flag)&&(setup.acc_off_flag))
    				{
    					bt_puts("bt_page_scan000\n");
    					bt_page_scan(0);
    				}
    				else
    				{
    					bt_puts("bt_page_scan111\n");
    					bt_page_scan(1);
    				}
    			#else
    				bt_page_scan(1);
    			#endif
    			#endif
    			}
    			else
    			{
    				printf("BT_STATUS_INIT_OK\n");
    				/* while(play_sel_busy()) {
    					os_time_dly(1);
    				}
    				dac_off_control(); */
    				bt_power_max(4);
    				user_send_cmd_prepare(USER_CTRL_START_CONNECTION,0,NULL);
    			#if BT_KTV_EN
    				/* set_esco_sr(8000); */
    				set_esco_sr(16000L);
    				echo_msg_deal_api((void**)&bt_reverb, &msg_echo_start);
    			#endif
    			}
            }
			break;

		case BT_STATUS_FIRST_CONNECTED:
		case BT_STATUS_SECOND_CONNECTED:
			puts("BT_STATUS_FIRST_CONNECTED\n");
			
			extern u8 get_first_time_conn_flag(void);
			if(get_first_time_conn_flag())
			{
				puts("CONNECTED22222\n");
			}
			else
			{
				puts("FIRST_CONNECTED111111\n");
			}
		#if defined(PHONE_NUMBER_OVER_PLAY_PHONE_OUTINCOME_TONE)
			PHONE_NUMBER_FLAG=0;
		#endif
			bt_power_max(0xFF);
			user_ctrl_prompt_tone_play(BT_STATUS_FIRST_CONNECTED, NULL);
			break;

		case BT_STATUS_FIRST_DISCONNECT:
		case BT_STATUS_SECOND_DISCONNECT:
		#if defined(PHONE_NUMBER_OVER_PLAY_PHONE_OUTINCOME_TONE)
			PHONE_NUMBER_FLAG=0;
		#endif
		#if defined(vbar_disp_bt_music)
			vbar_bt_music = ENABLE;
		#endif
			user_ctrl_prompt_tone_play(BT_STATUS_FIRST_DISCONNECT, NULL);
			break;

		case BT_STATUS_PHONE_NUMBER:
			printf("---------BT_STATUS_PHONE_NUMBER\n");
		#if defined ( BT_PHONE_NUMBER)
		#if defined(_DISABLE_CALL_RING_)
			printf("---------global.call_flag=%d\n",global.call_flag);
			if(global.call_flag)
			{
				os_time_dly(5);
				global.call_flag = FALSE;
				user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
			}
			else
			{
				user_ctrl_prompt_tone_play(BT_STATUS_PHONE_NUMBER,NULL);
			}
		#else
			user_ctrl_prompt_tone_play(BT_STATUS_PHONE_NUMBER,NULL);
		#endif
		#else
			os_time_dly(5);
	    #if defined(_ENABLE_PHONE_INCOME_)
	    	extern u8 check_support_feature_ag(u16 temp);
	        if(!check_support_feature_ag(0x0008))//苹果手机播自己的铃声，安卓播固定提示音
	        {
	    	    printf("---------check_support_feature_ag\n");
			#if !defined(_ENABLE_PHONE_OUTGOING_)
	    	    user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
	    	#endif
	        }
	    #else
	    	user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
	    #endif
		#endif
			break;
		case BT_STATUS_PHONE_INCOME:
			printf("111BT_STATUS_PHONE_INCOME111\n");
		#if defined(BT_TALK_VOL)
		#if !defined ( BT_PHONE_NUMBER)
			sys_mute_ctrl(ENABLE, 2000);
		#endif
		#endif
		
		#if defined(PHONE_NUMBER_OVER_PLAY_PHONE_OUTINCOME_TONE)
			if(PHONE_NUMBER_FLAG==0)
			{
				PHONE_NUMBER_FLAG=1;
				printf("---------BT_STATUS_PHONE_INCOME\n");
				//user_val->is_phone_number_come = 0;
				user_send_cmd_prepare(USER_CTRL_HFP_CALL_CURRENT,0,NULL);//发命令获取电话号码
			}
			else
			{
				user_val->play_phone_number_flag = 0;
				user_val->phone_prompt_tone_playing_flag = 0;
				extern u8 check_support_feature_ag(u16 temp);
               // if(!check_support_feature_ag(0x0008))//苹果手机播自己的铃声，安卓播固定提示音
				{
					printf("---------check_support_feature_ag\n");
                    user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
                }
			}
		#else
			printf("---------BT_STATUS_PHONE_INCOME\n");
			//user_val->is_phone_number_come = 0;
			user_send_cmd_prepare(USER_CTRL_HFP_CALL_CURRENT,0,NULL);//发命令获取电话号码
		#endif

		#if defined(_ENABLE_PHONE_INCOME_)
			extern u8 check_support_feature_ag(u16 temp);
		    if(!check_support_feature_ag(0x0008))//苹果手机播自己的铃声，安卓播固定提示音
		    {
			    printf("---------check_support_feature_ag\n");
			    user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
		    }
		#endif
			break;

		case BT_STATUS_PHONE_OUT:
			printf("BT_STATUS_PHONE_OUT\n");
	    #if defined(BT_TALK_VOL)
		#if !defined ( BT_PHONE_NUMBER)
	    	sys_mute_ctrl(ENABLE, 1000);
	    #endif
		#endif
		
		#if defined(_DISABLE_CALL_RING_)||defined(_ENABLE_PHONE_OUTGOING_)
			global.call_flag = TRUE;
		#endif
		
		#if defined(PHONE_NUMBER_OVER_PLAY_PHONE_OUTINCOME_TONE)
			if(PHONE_NUMBER_FLAG==0)
			{
				PHONE_NUMBER_FLAG=1;
				printf("---------BT_STATUS_PHONE_OUT\n");
				user_val->is_phone_number_come = 0;
				user_send_cmd_prepare(USER_CTRL_HFP_CALL_CURRENT,0,NULL);//发命令获取电话号码
			}
			else
			{
				user_val->play_phone_number_flag = 0;
				user_val->phone_prompt_tone_playing_flag = 0;
				//user_ctrl_prompt_tone_play(BT_STATUS_PHONE_INCOME,NULL);
			}
		#else
			printf("---------BT_STATUS_PHONE_OUT\n");
			user_val->is_phone_number_come = 0;
		#if !defined(_DISABLE_CALL_NUMBER_)
			user_send_cmd_prepare(USER_CTRL_HFP_CALL_CURRENT,0,NULL);//发命令获取电话号码
		#endif
		#endif
			break;

		case BT_STATUS_PHONE_ACTIVE:
		#if defined(PHONE_NUMBER_OVER_PLAY_PHONE_OUTINCOME_TONE)
			PHONE_NUMBER_FLAG=0;
		#endif
		#if defined(HuiNuo_6651D_Copy)
			if(setup.aud_vol==32)
			{
				HuiNuo_6651D_flag=1;
				setup.aud_vol=20;
			}
		#endif
			printf("---------BT_STATUS_PHONE_ACTIVE\n");
			user_val->play_phone_number_flag = 0;
			if(user_val->phone_prompt_tone_playing_flag)
			{
				play_sel_stop();
				user_val->phone_prompt_tone_playing_flag = 0;
			}
			break;

		case BT_STATUS_PHONE_HANGUP:	
	    #if defined(_DISABLE_CALL_RING_)||defined(_ENABLE_PHONE_OUTGOING_)
	    	global.call_flag = FALSE;
	    #endif
		
		#if defined(_EVB_REJECT_MUTE_)
			AUDIO_MUTE_ON();
			OSTimeDly(1);
			sys_mute_ctrl(DISABLE, 1000);
		#elif defined(_XM007_REJECT_MUTE_)
			sys_mute_ctrl(ENABLE, 1200);
		#endif
		
		#if defined(HuiNuo_6651D_Copy)
			if((setup.aud_vol==20)&&(HuiNuo_6651D_flag==1))
			{
				HuiNuo_6651D_flag=0;
				setup.aud_vol=26;
			}
		#endif
		#if defined(PHONE_NUMBER_OVER_PLAY_PHONE_OUTINCOME_TONE)
			PHONE_NUMBER_FLAG=0;
		#endif
				
		#if defined(_SUPPORT_IPHONE_SIR_)
			global.sir_flag = FALSE;
		#endif
		
			printf("---------BT_STATUS_PHONE_HANGUP\n");
			digit_auto_mute_set(1,-1,-1,-1); // 开自动mute
			user_val->play_phone_number_flag = 0;
			user_val->is_phone_number_come = 0;
			if(user_val->phone_prompt_tone_playing_flag)
			{
				play_sel_stop();
				user_val->phone_prompt_tone_playing_flag = 0;
			}
			//后台电话回来，电话完了自动退出
			user_send_cmd_prepare(USER_CTRL_BTSTACK_SUSPEND,0,NULL);    //go back to background
		#if defined(_ENABLE_BT_RDS_)
        	if(global.rds_ta)
        	{
				global.bt_rds_flag = TRUE;
				global.bt_rds_time = 0;
        		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RDS_OFF, TRUE);
        	}
		#endif
			break;

        default:
           break;
    }
}
void stereo_led_deal()
{
#if BT_TWS
   if(BT_CURRENT_CONN_PHONE==get_bt_current_conn_type())//?????????
   {

   }
   else if(BT_CURRENT_CONN_STEREO_MASTER==get_bt_current_conn_type())//????????????????
   {

   }
   else if(BT_CURRENT_CONN_STEREO_SALVE==get_bt_current_conn_type())//?????????
   {

   }
   else if(BT_CURRENT_CONN_STEREO_MASTER_PHONE==get_bt_current_conn_type())//???????????????????
   {

   }
   else if(BT_CURRENT_CONN_STEREO_PHONE_MASTER==get_bt_current_conn_type())//?????????????????
   {

   }
   else
   {
   }
#endif
}

static void bt_engine_timer(void *param)
{
	os_taskq_post_msg(BTMSG_TASK_NAME, 1, MSG_BT_TIMER);
}

#if defined(_ENABLE_A2DP_TIME_)&&0
extern void send_bt_id3(u8 type);
#endif

extern void set_osc_2_vm(void);
static void btstack_key_handler(void *ptr,int key)
{
#if BT_TWS
	if(is_check_stereo_slave())
	{
		switch(key)
		{
			case MSG_VOL_DOWN:
			case MSG_VOL_UP:
			case MSG_BT_PP:
			case MSG_BT_NEXT_FILE:
			case MSG_BT_PREV_FILE:
			case MSG_BT_ANSWER_CALL:
			case MSG_BT_CALL_LAST_NO:
			case MSG_BT_CALL_REJECT:
			case MSG_BT_CALL_CONTROL:
				stereo_slave_cmd_handle(msg[0],0);
				return;
			default:
				break;
		}
	}
#endif
	switch(key)
	{
#if BT_TWS
#if EQ_EN
		case  MSG_BT_SYNC_STEREO_EQ:
			if(user_val->bt_eq_mode == 0)
			{
				return;
			}
			if(is_check_stereo_slave())
			{
				stereo_slave_cmd_handle(MSG_BT_MUSIC_EQ,user_val->bt_eq_mode);
			}
			else
			{
				stereo_host_cmd_handle(MSG_BT_MUSIC_EQ,user_val->bt_eq_mode);
			}
			os_taskq_post("btmsg", 2, MSG_BT_MUSIC_EQ,user_val->bt_eq_mode);
			break;
		case  MSG_BT_STEREO_EQ:
			if(is_check_stereo_slave())
			{
				stereo_slave_cmd_handle(MSG_BT_MUSIC_EQ,0);
			}
			else
			{
				stereo_host_cmd_handle(MSG_BT_MUSIC_EQ,0);
			}
			os_taskq_post("btmsg", 1, MSG_BT_MUSIC_EQ);
			break;
#endif

		case MSG_VOL_STEREO:
			/* puts("bt MSG_VOL_STEREO\n"); */
			stereo_host_cmd_handle(msg[0],msg[1]);
			break;
#endif
        case MSG_BT_PP:
			bt_puts("btmsg--->MSG_BT_PP\n");
			if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)
				&&(get_bt_newest_status() != BT_STATUS_PHONE_OUT)
				&&(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE)
				&&(get_bt_connect_status() != BT_STATUS_TAKEING_PHONE))
			{
    	    #if !defined(_WLS_SPECIAL_MUTE_)
			#if !defined(_PLAY_PAUSE_MUTE_)
    	    	sys_mute_ctrl(ENABLE, 0);
			#endif
			#endif
			    bt_puts("btmsg--->MSG_BT_PP111\n");
    	    	user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_PLAY,0,NULL);
    	    #if !defined(_WLS_SPECIAL_MUTE_)
			#if !defined(_PLAY_PAUSE_MUTE_)
    	    	sys_mute_ctrl(DISABLE, 500);
    	    #endif
			#endif
			}
            break;

        case MSG_BT_NEXT_FILE:
            bt_puts("btmsg--->MSG_BT_NEXT_FILE\n");
			if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)
				&&(get_bt_newest_status() != BT_STATUS_PHONE_OUT)
				&&(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE)
				&&(get_bt_connect_status() != BT_STATUS_TAKEING_PHONE))
			{
			#if !defined(_NEXT_PREV_MUTE_)
    			sys_mute_ctrl(ENABLE, 0);
			#endif
                user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_NEXT,0,NULL);
			#if !defined(_NEXT_PREV_MUTE_)
    			sys_mute_ctrl(DISABLE, 500);
			#endif
			}
            break;

        case MSG_BT_PREV_FILE:
            bt_puts("btmsg--->MSG_BT_PREV_FILE\n");
			if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)
				&&(get_bt_newest_status() != BT_STATUS_PHONE_OUT)
				&&(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE)
				&&(get_bt_connect_status() != BT_STATUS_TAKEING_PHONE))
			{
			#if !defined(_NEXT_PREV_MUTE_)
    			sys_mute_ctrl(ENABLE, 0);
			#endif
                user_send_cmd_prepare(USER_CTRL_AVCTP_OPID_PREV,0,NULL);
			#if !defined(_NEXT_PREV_MUTE_)
    			sys_mute_ctrl(DISABLE, 500);
			#endif
			}
            break;

	#if EQ_EN
         case MSG_BT_MUSIC_EQ:
		 	bt_puts("btmsg--->MSG_BT_EQ\n");
			set_bt_eq_mode(user_val->bt_eq_mode+1);
            break;
	#endif

        case MSG_BT_PAGE_SCAN://可发现可连接命令
			bt_puts("btmsg--->MSG_BT_PAGE_SCAN\n");
            bt_page_scan(2);
            break;
	#if BT_TWS
		case MSG_BT_STEREO_SEARCH_DEVICE:
			puts("MSG_BT_STEREO_SEARCH_DEVICE\n");
			if(fast_test)
			{
				puts("fast_test MSG_BT_STEREO_SEARCH_DEVICE\n");
				break;
			}
            if(BT_TWS_CHANNEL_SELECT&&BT_TWS_CHANNEL_SELECT==TWS_CHANNEL_RIGHT)
			{
                puts(" TWS_CHANNEL_RIGHT not search\n");
				break;
			}
			user_send_cmd_prepare(USER_CTRL_SEARCH_DEVICE,0,NULL);
		#if BT_TWS_SCAN_ENBLE
			if(!is_check_stereo_slave())
			{
				bt_page_scan(2);
			}
		#endif
			break;
		case MSG_BT_TWS_DELETE_ADDR:
			puts("MSG_BT_TWS_DELETE_ADDR\n");
			delete_stereo_addr();
			break;
	#endif
	
	#if defined(_YJ3993_ADKEY_CONTROL_)
		case MSG_BT_ANSWER_REJECT:
			if(get_bt_newest_status() == BT_STATUS_PHONE_NUMBER)
			{
				user_send_cmd_prepare(USER_CTRL_HFP_CALL_ANSWER,0,NULL);
			}
			else if((get_call_status() == BT_CALL_INCOMING)&&(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
			{
				user_send_cmd_prepare(USER_CTRL_HFP_CALL_ANSWER,0,NULL);
			}
			//else if((get_bt_newest_status() == BT_STATUS_FIRST_CONNECTED)||(get_bt_newest_status() == BT_STATUS_SECOND_CONNECTED)||(get_bt_newest_status() == BT_STATUS_PHONE_HANGUP))
			//{
			//	user_send_cmd_prepare(USER_CTRL_HFP_CALL_LAST_NO,0,NULL);
			//}
			else if((get_bt_newest_status() == BT_STATUS_PHONE_ACTIVE)||(get_bt_newest_status() == BT_STATUS_PHONE_OUT))
			{
				user_send_cmd_prepare(USER_CTRL_HFP_CALL_HANGUP,0,NULL);
			}
			else // if((get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_COME)||(get_bt_newest_status() == BT_STATUS_MUSIC_SOUND_GO))
			{
				user_send_cmd_prepare(USER_CTRL_HFP_CALL_LAST_NO,0,NULL);
			}

			break;
	#endif

		case MSG_BT_ANSWER_CALL:
            bt_puts("btmsg--->MSG_BT_ANSWER_CALL\n");
    	#if defined(ANSWER_KEY_ONLY_ANSWER)//接听键只用接听功能
			if(get_call_status() == BT_CALL_INCOMING)
			{//接听
				user_send_cmd_prepare(USER_CTRL_HFP_CALL_ANSWER,0,NULL);
			}
		#else
			if(get_call_status() == BT_CALL_HANGUP)
			{
				bt_puts("btmsg--->MSG_BT_ANSWER_CALL111\n");
				if(get_last_call_type()== BT_CALL_OUTGOING)
	            {
					bt_puts("btmsg--->MSG_BT_ANSWER_CALL222\n");
	            #if !defined(_SPECIAL_ANSWER_REJECT_)
				#if !defined(_EVB_ANSWER_REJECT_)
				#if !defined(_SPECIAL_ANSWER_KEY_)
				#if !defined(_SPECIAL_BT_FUNTION_)
				   user_send_cmd_prepare(USER_CTRL_HFP_CALL_LAST_NO,0,NULL);
				#endif
				#endif
				#endif
				#endif
	            }
				else if(get_last_call_type() == BT_CALL_INCOMING)
				{
					bt_puts("btmsg--->MSG_BT_ANSWER_CALL333\n");
				#if !defined(_SPECIAL_ANSWER_REJECT_)
				#if !defined(_EVB_ANSWER_REJECT_)
				#if !defined(_SPECIAL_ANSWER_KEY_)
	                user_send_cmd_prepare(USER_CTRL_DIAL_NUMBER,user_val->phone_num_len,user_val->income_phone_num);
				#endif
				#endif
				#endif
				}
			}
            else if(get_call_status() == BT_CALL_INCOMING)
			{//接听
				bt_puts("btmsg--->MSG_BT_ANSWER_CALL444\n");
                user_send_cmd_prepare(USER_CTRL_HFP_CALL_ANSWER,0,NULL);
            }
			else if(get_call_status() == BT_CALL_ACTIVE)
			{//切换音源
				bt_puts("btmsg--->MSG_BT_ANSWER_CALL555\n");
			#if defined(_TALKING_VOICE_DISPLAY_)
			    ui_show_setting(LANG_ID_BT_TRANSFER, 0);
			#endif
				user_send_cmd_prepare(USER_CTRL_SCO_LINK,0,NULL);
			}
		#endif
            break;
			
    #if defined(_ENABLE_KEY_REDIAL_)
    	case MSG_BT_ANSWER_REDIAL:
    		{
    			ew_puts("btmsg--->MSG_BT_ANSWER_REDIAL\n");
    			if(get_call_status() == BT_CALL_HANGUP)
    			{
    				if(get_last_call_type()== BT_CALL_OUTGOING)
    				{
    					user_send_cmd_prepare(USER_CTRL_HFP_CALL_LAST_NO,0,NULL);
    				}
    				else if(get_last_call_type() == BT_CALL_INCOMING)
    				{
    					user_send_cmd_prepare(USER_CTRL_DIAL_NUMBER,user_val->phone_num_len,user_val->income_phone_num);
    				}
    			}
    			else if(get_call_status() == BT_CALL_INCOMING)
    			{
    				user_send_cmd_prepare(USER_CTRL_HFP_CALL_ANSWER,0,NULL);
    			}
    			else if(get_call_status() == BT_CALL_ACTIVE)
    			{
			    #if defined(_TALKING_VOICE_DISPLAY_)
			    	ui_show_setting(LANG_ID_BT_TRANSFER, 0);
			    #endif
    				user_send_cmd_prepare(USER_CTRL_SCO_LINK,0,NULL);
    			}
    		}
    		break;
    #endif
	
	#if defined(_SUPPORT_IPHONE_SIR_)
    	case MSG_BT_SIR:
    		bt_puts("btmsg--->MSG_BT_SIR\n");
    		if(global.sir_flag)
    		{
				user_send_cmd_prepare(USER_CTRL_HFP_GET_SIRI_CLOSE,0,NULL);
				global.sir_flag = FALSE;
    		}
    		else
    		{
				global.sir_flag = TRUE;
				user_send_cmd_prepare(USER_CTRL_HFP_GET_SIRI_OPEN,0,NULL);
				if(global.sir_mode)
				{
					global.sir_mode = FALSE;
					global.sir_mode1 = TRUE;
				}
    		}
    		break;
	#endif

        case MSG_BT_CALL_LAST_NO:
            bt_puts("btmsg--->MSG_BT_CALL_LAST_NO\n");
            if(get_call_status() == BT_CALL_HANGUP)
            {
                if(get_last_call_type()== BT_CALL_OUTGOING)
	            {
	                user_send_cmd_prepare(USER_CTRL_HFP_CALL_LAST_NO,0,NULL);
	            }
				else if(get_last_call_type() == BT_CALL_INCOMING)
				{
	                user_send_cmd_prepare(USER_CTRL_DIAL_NUMBER,user_val->phone_num_len,user_val->income_phone_num);
	            }
            }
            break;

        case MSG_BT_CALL_REJECT:
            bt_puts("btmsg--->MSG_BT_CALL_REJECT\n");
            if(get_call_status() != BT_CALL_HANGUP)
            {
				bt_puts("btmsg--->MSG_BT_CALL_REJECT111\n");
                user_send_cmd_prepare(USER_CTRL_HFP_CALL_HANGUP,0,NULL);
            }
            break;
			
    #if defined(_TALKING_VOL_CHANGE_)
    	case MSG_BT_VOL_CHANGE:
    		ew_puts("btmsg--->MSG_BT_VOL_CHANGE\n");
    		if(get_call_status() == BT_CALL_ACTIVE)
    		{
    			user_send_cmd_prepare(USER_CTRL_SCO_LINK,0,NULL);
    		}
    		break;
    #endif
	
	#if defined(_SUPPORT_CALL_RECORDS_)
    	case MSG_BT_CALL_ONE_NUMBER:
    		ew_puts("btmsg--->MSG_BT_CALL_ONE_NUMBER\n");
    		if(get_call_status() == BT_CALL_HANGUP)
    		{
				ew_printf("setup.num_buffer:%s\n",global.info.num_buffer[global.info.sum]);
				ew_printf("disp_length:%d\n",global.info.disp_length);
				ew_printf("sum:%d\n",global.info.sum);
    			user_send_cmd_prepare(USER_CTRL_DIAL_NUMBER, global.info.disp_length, global.info.num_buffer[global.info.sum]);
    		}
    		break;
	#endif

		case MSG_BT_REDIAL:
        	bt_puts("btmsg--->MSG_BT_REDIAL\n");			
			if(get_call_status() == BT_CALL_HANGUP)
			{
				if(get_last_call_type()== BT_CALL_OUTGOING)
	            {
				   user_send_cmd_prepare(USER_CTRL_HFP_CALL_LAST_NO,0,NULL);
	            }
				else if(get_last_call_type() == BT_CALL_INCOMING)
				{
	                user_send_cmd_prepare(USER_CTRL_DIAL_NUMBER,user_val->phone_num_len,user_val->income_phone_num);
				}
			}
        	break;

		case MSG_BT_CALL_NUMBER:
			ew_puts("btmsg--->MSG_BT_CALL_NUMBER\n");
			if(get_call_status() == BT_CALL_HANGUP)
			{
				typedef struct{
					uint8 length;
					uint8 buffer[1];
				}num_t;
				num_t *num = (num_t *)ptr;

				uint8 length  = num->length;
				uint8 *buffer = num->buffer;

				ew_puts("btmsg--->MSG_BT_CALL_NUMBER1111\n");
				if(length > sizeof(user_val->income_phone_num))
				{
					length = sizeof(user_val->income_phone_num);
				}
				memcpy(user_val->income_phone_num, buffer, length);
				user_val->phone_num_len = length;

				user_send_cmd_prepare(USER_CTRL_DIAL_NUMBER, user_val->phone_num_len, user_val->income_phone_num);
			}
			break;

        case MSG_BT_CONNECT_CTL:
            bt_puts("btmsg--->MSG_BT_CONNECT_CTL\n");
			{
				STATUS_FOR_USER connect_status = (STATUS_FOR_USER)get_bt_connect_status();

				if((connect_status == BT_STATUS_CONNECTING)||(connect_status == BT_STATUS_TAKEING_PHONE)||(connect_status == BT_STATUS_PLAYING_MUSIC))
				{
					bt_puts("bt_disconnect\n");/*手动断开连接*/
				#if !defined(_WLGK3001_KEY_FUNTION_)
					user_send_cmd_prepare(USER_CTRL_DISCONNECTION_HCI,0,NULL);
				#endif
				}
				else
				{
					bt_puts("bt_connect\n");/*手动连接*/
				#if !defined(_ENABLE_REJECT_CONNECT_)
                	user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR_MANUALLY,0,NULL);
				#endif
				}
			}
            break;

	#if BT_TWS
		case MSG_BT_CONNECT_STEREO_CTL:
			puts("MSG_BT_CONNECT_STEREO_CTL\n");
			if((BT_STATUS_CONNECTING == get_stereo_bt_connect_status())   ||
				(BT_STATUS_TAKEING_PHONE == get_stereo_bt_connect_status())||
				(BT_STATUS_PLAYING_MUSIC == get_stereo_bt_connect_status()))
			{
				puts("bt_disconnect\n");
				user_send_cmd_prepare(USER_CTRL_DISCONNECTIO_STEREO_HCI,0,NULL);
			}
			else
			{
				puts("bt_connect\n");
				user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR_STEREO,0,NULL);
			}
			break;
	#endif

		case MSG_BT_AUDIO_PHONE:
			bt_puts("btmsg--->MSG_BT_AUDIO_PHONE\n");
            if(get_call_status() == BT_CALL_INCOMING)
			{
                user_send_cmd_prepare(USER_CTRL_DISCONN_SCO,0,NULL);
            }
			break;

		case MSG_BT_AUDIO_HOST:
			bt_puts("btmsg--->MSG_BT_AUDIO_HOST\n");
			if(get_call_status() == BT_CALL_ACTIVE)
			{
			    user_send_cmd_prepare(USER_CTRL_CONN_SCO,0,NULL);
			}
			break;
	#if BT_REC_EN
		case MSG_REC_PLAY:
			if(play_sel_busy())
			{
				play_sel_stop();
			}
			else
			{
				exit_rec_api(&rec_bt_api);//exit rec when esco change
				bt_prompt_play_by_name(rec_file_name_last, &rec_let_last);
			}
			break;
	#endif

		case MSG_BT_AUDIO_CHANGE:
			bt_puts("btmsg--->MSG_BT_AUDIO_CHANGE\n");
			if(get_call_status() == BT_CALL_ACTIVE)
			{
			    user_send_cmd_prepare(USER_CTRL_SCO_LINK,0,NULL);
			}
			break;

		case MSG_BT_DISCONNECT:
			bt_puts("btmsg--->MSG_BT_DISCONNECT\n");
			{
				STATUS_FOR_USER connect_status = (STATUS_FOR_USER)get_bt_connect_status();

				switch(connect_status)
				{
					case BT_STATUS_CONNECTING:
					case BT_STATUS_TAKEING_PHONE:
					case BT_STATUS_PLAYING_MUSIC:
						bt_puts("bt_disconnect\n");/*手动断开连接*/
	                	user_send_cmd_prepare(USER_CTRL_DISCONNECTION_HCI,0,NULL);
					#if defined(ENABLE_TWO_PHONE_CONNECT)
						os_time_dly(20);
						bt_puts("bt_disconnect1111\n");/*手动断开连接*/
						user_send_cmd_prepare(USER_CTRL_DISCONNECTION_HCI,0,NULL);
					#endif
						break;

					default:
						break;
				}
			}
			break;

		case MSG_BT_DTMF:
			bt_puts("btmsg--->MSG_BT_DTMF\n");
			if((get_call_status() == BT_CALL_OUTGOING)||(get_call_status() == BT_CALL_ACTIVE))
			{
                //uint8 dtmf = (uint8 )ptr;
            }
			break;

        case MSG_BT_HID_CTRL:
            bt_puts("btmsg--->MSG_BT_HID_CTRL\n");
            if(get_curr_channel_state() & HID_CH)
            {
                user_send_cmd_prepare(USER_CTRL_HID_DISCONNECT,0,NULL);
            }
			else
			{
                user_send_cmd_prepare(USER_CTRL_HID_CONN,0,NULL);
            }
            break;

        case MSG_BT_HID_TAKE_PIC:
            bt_puts("btmsg--->MSG_BT_HID_TAKE_PIC\n");
            if(get_curr_channel_state() & HID_CH)
            {
				user_send_cmd_prepare(USER_CTRL_HID_BOTH,0,NULL);
            }
            break;
	   /*case MSG_PROMPT_PLAY:
			puts("MSG_PROMPT_PLAY\n");
			bt_prompt_play_by_name(BPF_TEST_MP3, NULL);
			break;*/

		case MSG_HALF_SECOND:
			stereo_led_deal();
			if(fast_test)
			{
				user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
				user_send_cmd_prepare(USER_CTRL_WRITE_CONN_DISABLE,0,NULL);
			}
		/*#if BT_TWS
		#if (BT_TWS_SCAN_ENBLE==0)
			if(0 == get_total_connect_dev() )
			{
				if(user_val->discover_control_cnt == 1)
				{
					user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_ENABLE,0,NULL);
				}
				else if(user_val->discover_control_cnt==15)
				{
					user_send_cmd_prepare(USER_CTRL_WRITE_SCAN_DISABLE,0,NULL);
				}
				user_val->discover_control_cnt++;
				if(user_val->discover_control_cnt>=20)
					user_val->discover_control_cnt =1;
			}
		#endif
		#endif
			if((BT_STATUS_CONNECTING == get_bt_connect_status())   ||
			(BT_STATUS_TAKEING_PHONE == get_bt_connect_status())||
			(BT_STATUS_PLAYING_MUSIC == get_bt_connect_status()))
			{
				if(BT_MUSIC_STATUS_STARTING == a2dp_get_status())
				{
				}
				else if(BT_STATUS_TAKEING_PHONE == get_bt_connect_status())
				{
					///puts("bt_phone\n");
				}
				else
				{
					///puts("bt_other\n");
					//puts("BT_CONN\n");
				}
				user_val->auto_shutdown_cnt = AUTO_SHUT_DOWN_TIME;
			}
			else  if(BT_STATUS_WAITINT_CONN == get_bt_connect_status()&&user_val->auto_shutdown_cnt)
			{
				//puts("BT_STATUS_WAITINT_CONN\n");
				user_val->auto_shutdown_cnt--;
				//printf("power cnt:%d\n",user_val->auto_shutdown_cnt);
				if(user_val->auto_shutdown_cnt==0)
				{
					puts("*****shut down*****\n");
					going_to_pwr_off = 1;
					os_taskq_post("MainTask", 2, MSG_POWER_OFF_KEY_MSG ,0x44);
				}
			}
		#if (SNIFF_MODE_CONF&SNIFF_EN)
			if(check_in_sniff_mode())
			{
				printf("check_in_sniff_mode ok\n");
				vm_check_all(0);
				user_send_cmd_prepare(USER_CTRL_SNIFF_IN,0,NULL);
				//user_send_cmd_prepare(USER_CTRL_SNIFF_EXIT,0,NULL);
			}
		#endif
			if((bt_ui_var.ui_bt_connect_sta != get_bt_connect_status())||
			(bt_ui_var.ui_bt_a2dp_sta != a2dp_get_status()))
			{
				bt_ui_var.ui_bt_connect_sta = get_bt_connect_status();
				bt_ui_var.ui_bt_a2dp_sta = a2dp_get_status();
			}*/
			if(user_val->wait_app_updata_flag)
			{
				if(check_bt_app_updata_run())
				{
					user_val->wait_app_updata_flag =0;
				}
			}
			calling_del_page();
			break;

		case MSG_BT_SPP_UPDATA:
			bt_puts("btmsg--->MSG_BT_SPP_UPDATA\n");
            user_send_cmd_prepare(USER_CTRL_SPP_UPDATA_DATA,0,NULL);
            user_val->wait_app_updata_flag = 1;
			break;

        case MSG_BT_STACK_STATUS_CHANGE://所有蓝牙状态的改变 都执行这里
			#if defined(_ACC_OFF_ON_BTPHONE_DUBUG_)
			OSTimeDly(5);
			#endif
            bt_puts("btmsg--->MSG_BT_STACK_STATUS_CHANGE\n");
            btstack_status_change_deal(NULL);
			if((Interrupt ==TRUE)&&(get_bt_connect_status()!=BT_STATUS_TAKEING_PHONE))
			{//解决待机状态 打电话立马关机 不能回到待机状态的问题
				sys_mute_ctrl(ENABLE, 0);
				Interrupt =FALSE;
				os_taskq_post(MAIN_TASK_NAME,1,MSG_LAST_WORKMOD);
			}
            break;

        case SYS_EVENT_DEL_TASK:
            bt_puts("SYS_EVENT_DEL_TASK\n");
            //断开和关闭协议栈
            user_send_cmd_prepare(USER_CTRL_POWER_OFF,0,NULL);
            //删除协议栈
            delete_stack_task();
            bt_puts("del stack ok\n");
            play_sel_stop();
			if(dac_ctl.toggle == 0)
			{
				dac_on_control();
			}
            //删除消息线程
            if (os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
            {
                bt_puts("del_ket_masg\n");
				echo_exit_api((void**)&bt_reverb);
                os_task_del_res_name(OS_TASK_SELF); 	//确认可删除，此函数不再返回
            }
            break;

		case MSG_BT_ENGINE:
			{
				bt_puts("btmsg--->MSG_BT_ENGINE\n");
				if((int )ptr == ENABLE)
				{
					bt_puts("MSG_BT_ENGINE --->ENABLE\n");
			    #if defined(_ENABLE_RADIO_TIME_)
			    	timer_reg_func(bt_engine_timer, NULL, 600);
			    #else
			    	timer_reg_func(bt_engine_timer, NULL, 300);
			    #endif
				}
				else
				{
					bt_puts("MSG_BT_ENGINE--->DISABLE\n");
					timer_unreg_func(bt_engine_timer);
				}
			}
			break;

		case MSG_BT_STATE:
			bt_puts("btmsg--->MSG_BT_STATE\n");
			bt_status_detect(TRUE);
			break;

		case MSG_BT_TIMER:
			bt_status_detect(FALSE);
			break;
		case MSG_BT_FAST_TEST:
			puts("MSG_BT_FAST_TEST \n");
			set_sys_vol(26,26,FADE_ON);
			digit_auto_mute_set(0, -1, -1, -1);
			microphone_open(60,1);
			fast_test = 1;
			break;
			
		case MSG_OSC_INTER_CAP_WRITE:
			puts("MSG_OSC_INTER_CAP_WRITE\n");
			set_osc_2_vm();
			break;
				
    #if defined(_ENABLE_A2DP_TIME_)&&0
    	case MSG_BT_ID3:
    		//puts("MSG_BT_ID3\n");
    		//send_bt_id3(msg[1]);
    		send_bt_id3();
    		break;
    #endif

		default:
			if(get_call_status() != BT_CALL_HANGUP)
			{
				/* puts("phone call ignore echo msg\n"); */
			}
			else
			{
				/* puts("bt_ctl_echo\n"); */
				echo_msg_deal_api((void**)&bt_reverb, msg);
			}

			break;
	}
}

#if BT_REC_EN
void bt_rec_msg_deal(int *msg)
{
#if BT_KTV_EN
	/* printf("get_bt_connect_status() = 0x%x\n", get_bt_connect_status()); */
	if(msg[0] == MSG_REC_START)
	{
		if((BT_STATUS_TAKEING_PHONE == get_bt_connect_status())
		||(BT_STATUS_PLAYING_MUSIC == get_bt_connect_status()))
		{
			if(!play_sel_busy())//play_sel is not BUSY
			{
				rec_msg_deal_api(&rec_bt_api, msg);
			}
		}
	}
	else
	{
		rec_msg_deal_api(&rec_bt_api, msg);
	}
#else
	if((BT_STATUS_TAKEING_PHONE != get_bt_connect_status())&&(msg[0] == MSG_REC_START))
	if(!get_sco_connect_status()&&(msg[0] == MSG_REC_START))
	{
		return;
	}
	rec_msg_deal_api(&rec_bt_api, msg);
#endif
}
#endif


/*消息处理*/
void TaskBtMsgStack(void *p_arg)
{
    os_sem_create(&tone_manage_semp, 1);

    while(1)
    {
        int msg[6];
		u32 res;
		memset(msg,0x00,sizeof(msg));///need do it
        res = os_taskq_pend(0, ARRAY_SIZE(msg), msg);
        if(res != OS_NO_ERR)
        {
            msg[0] = 0xff;
        }

	#if SUPPORT_APP_RCSP_EN
		rcsp_bt_task_msg_deal_before(msg);
	#endif

        btstack_key_handler((void *)msg[1], msg[0]);
    #if BT_REC_EN
    	bt_rec_msg_deal(msg);
    #endif

	#if SUPPORT_APP_RCSP_EN
		rcsp_bt_task_msg_deal_after(msg);
	#endif
	}
}

/*主要处理上电或者有些情况蓝牙处于不能切换模式状态*/
/*不属于用户接口，协议栈回调函数*/
extern int is_bt_stack_cannot_exist(void);
int msg_mask_off_in_bt(void)
{
#if BT_BACKMODE
    return is_bt_stack_cannot_exist();
#else
    return false;
#endif
}

/*通过任务切换进入蓝牙时回调接口*/
/*不属于用户接口，协议栈回调函数*/
void enter_btstack_task(void)
{
	bt_puts("----enter_btstack_task---\n");

    user_val->auto_shutdown_cnt = AUTO_SHUT_DOWN_TIME;
    dac_channel_on(BT_CHANNEL, FADE_ON);

#if EQ_EN
    eq_enable();
    user_val->bt_eq_mode = get_eq_default_mode();
#endif
#if defined(_AUDIO_USE_DECODER_)
	if(setup.aud_eq == 0)
	{
		audio_drv_ctrl(AUD_CTRL_BASS,	setup.aud_bass, 	FALSE);
		audio_drv_ctrl(AUD_CTRL_TREBLE, setup.aud_treble,	FALSE);
	}
	else
	{
		audio_drv_ctrl(AUD_CTRL_EQ, 	setup.aud_eq,		FALSE);
	}
	//audio_drv_ctrl(AUD_CTRL_LOUD,	setup.aud_loud, 	FALSE);
#endif
}

/*不属于用户接口，协议栈回调函数*/
/*通过任务切换退出蓝牙时回调接口*/
void exist_btstack_task(void)
{
	bt_puts("----exist_btstack_task---\n");

    user_val->play_phone_number_flag = 0;
    user_val->is_phone_number_come = 0;
    if(user_val->phone_prompt_tone_playing_flag)
    {
		play_sel_stop();
		user_val->phone_prompt_tone_playing_flag = 0;
    }
    dac_channel_off(BT_CHANNEL, FADE_ON);

#if EQ_EN
    eq_disable();
#endif // EQ_EN

#if BT_BACKMODE
	background_suspend();
#else
#if NFC_EN
   nfc_close();
#endif
    no_background_suspend();
#endif
	echo_exit_api((void**)&bt_reverb);
}
extern void sys_time_auto_connection();
void sys_time_auto_connection_caback(u8* addr)
{
	bt_puts("sys_time_auto_connection_caback\n");
    if(user_val->auto_connection_counter&&get_bt_prev_status() != BT_STATUS_SUSPEND)
    {
        bt_page_scan(0);
        printf("auto_conn_cnt1:%d\n",user_val->auto_connection_counter);
        user_val->auto_connection_counter--;
        user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR,6,addr);
    }
}

#if defined(POWER_DISCONNECT_BT_)||defined(_ENABLE_BT_ON_OFF_)||defined(_ENABLE_POWER_BT_OFF_)
u8 standby_bt[6] = {0};
static u8 standby_bt_flag = 0;
void save_standby_bt(u8 *addr)
{
	if((global.sys_power)||(addr != NULL))
	{
		memcpy(standby_bt,addr,6);
	}
	standby_bt_flag = 1;
}
void recon_standby_bt(void)
{
	if(standby_bt_flag
	&&(standby_bt[0] != 0)
	&&(standby_bt[1] != 0)
	&&(standby_bt[2] != 0)
	&&(standby_bt[3] != 0)
	&&(standby_bt[4] != 0)
	&&(standby_bt[5] != 0))
	{
		user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR,6,standby_bt);
	}
	standby_bt_flag = 0;
}
#endif

#if defined(_ENABLE_BT_AUTO_CONNECT_)
extern u8 bt_auto_connect;
#endif

#if defined(_BT_ON_MUTE_)
extern u8 get_first_time_conn_flag(void);//判断是回连还是第一次连接
#endif

/*不属于用户接口，协议栈回调函数*/
void bt_discon_complete_handle(u8 *addr , int reason)
{
#if defined(_BT_ON_MUTE_)
    printf("get_first_time_conn_flag:%d\n",get_first_time_conn_flag());
	if(global.sys_power)
	{
		if(!get_first_time_conn_flag())
		{
			sys_mute_ctrl(ENABLE, 1500);
		}
	}
#endif

    printf("bt_discon_complete:0x%x\n",reason);
    if(reason==0||reason==0x40 )
    {   //连接成功
	#if BT_TWS
		if((user_val->auto_connection_stereo)&&reason==0x40)
		{
			puts("-----------not clean auto_connection_stereo1\n");
		}
		else
		{
			user_val->auto_connection_counter = 0;
			user_val->auto_connection_stereo=0;
		}
	#else
        user_val->auto_connection_counter = 0;
	#endif
        return ;
    }
	else if(reason==0xfc)
	{
        //新程序没有记忆地址是无法发起回连
		bt_puts("vm no bd_addr\n");
		bt_page_scan(1);
        return ;
    }
#if BT_TWS
	else if(reason==0x10||reason==0xf||reason==0x13||reason==0x14)
	{
		puts("conneciton accept timeout\n");
		bt_page_scan(1);
		return ;
	}
	else if(reason==0x09)
	{
		puts("bt stereo search device timeout\n");
		return ;
	}
#else
	else if((reason==0x10)||(reason == 0xf)||(reason == 0xd))
	{
		puts("conneciton accept timeout\n");
		bt_page_scan(1);
        return ;
	}
#endif
#if BT_TWS
	clear_a2dP_sink_addr(addr);
#endif

	if (reason == 0x16) 
	{
		puts("Conn Terminated by Local Host\n");
	#if defined(POWER_DISCONNECT_BT_)||defined(_ENABLE_BT_ON_OFF_)||defined(_ENABLE_POWER_BT_OFF_)
		save_standby_bt(addr);
	#endif
	#if BT_TWS
		if(get_current_search_index() >= 1)
		{
			user_send_cmd_prepare(USER_CTRL_START_CONNECTION,0,NULL);
		}else{
			bt_page_scan(1);
		}
	#else
    #if defined(POWER_DISCONNECT_BT_)||defined(_ENABLE_BT_ON_OFF_)
    	if((setup.power_off_flag)&&(setup.acc_off_flag))
    	{
    		bt_puts("bt_page_scan000\n");
    		bt_page_scan(0);
    	}
    	else
    	{
    		bt_puts("bt_page_scan111\n");
			if(setup.power_off_flag)
			{
				bt_page_scan(0);
			}
			else
			{
				bt_page_scan(1);
			}
    	}
    #else
    	bt_page_scan(1);
    #endif
	#endif
		vm_check_all(0);
	}
	else if(reason == 0x08)
	{
		puts("\nconntime timeout\n");
    #if defined(_ENABLE_BT_AUTO_CONNECT_)
    	bt_auto_connect = TRUE;
    #endif
		if(!get_remote_test_flag())
		{
		#if BT_TWS
            if(get_bt_prev_status() != BT_STATUS_SUSPEND &&
                 (get_call_status() == BT_CALL_HANGUP)&&
				 (BD_CONN_MODE == get_stereo_salve(1)))
		#else
            if(get_bt_prev_status() != BT_STATUS_SUSPEND &&
                 (get_call_status() == BT_CALL_HANGUP))
		#endif
            {
                user_val->auto_connection_counter = 65000;
                puts("\nsuper timeout\n");
			#if BT_TWS
                if(BT_STATUS_STEREO_WAITING_CONN== get_stereo_bt_connect_status())///ê??úá??óμ???￡???á?ê??ú??oó?ìD???á?????
				{
                     puts("\nafter auto_connection_stereo\n");
                     after_auto_connection_stereo(1,0);
				     user_send_cmd_prepare(USER_DEL_PAGE_STEREO_HCI,0,NULL);
				     user_val->auto_connection_stereo = 1;
                     user_val->auto_connection_counter = 4;
				}
			#endif
                user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR,6,addr);
            }
		}
		else
		{
    		user_val->auto_connection_counter = 0;
			bt_puts("bt_page_scan222\n");
    		bt_page_scan(1);
		}
	}
	else if(reason == 0x04)
	{
		if(!user_val->auto_connection_counter)
		{
		    bt_puts("page timeout---\n");
		    if(get_current_search_index() >= 1)
		    {    //继续搜索下一个设备
                 user_send_cmd_prepare(USER_CTRL_START_CONNECTION,0,NULL);
		    }
			else
			{
			#if BT_TWS
				if(user_val->auto_connection_stereo)
				{
					user_val->auto_connection_stereo = 0;
					puts("-----------clean auto_connection_stereo2\n");
					if((BT_STATUS_WAITINT_CONN == get_stereo_bt_connect_status()))
					{
						puts("\nstart2 auto_connection_stereo\n");
						bt_page_scan(1);
						after_auto_connection_stereo(0,1);
						return;
					}
				}
				user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR_STEREO,0,NULL);
			#endif
				bt_puts("bt_page_scan333\n");
		    #if defined(POWER_DISCONNECT_BT_)||defined(_ENABLE_BT_ON_OFF_)
		    	if((setup.power_off_flag)&&(setup.acc_off_flag))
		    	{
		    		bt_puts("bt_page_scan000\n");
		    		bt_page_scan(0);
		    	}
		    	else
		    	{
		    		bt_puts("bt_page_scan111\n");
		    		bt_page_scan(1);
		    	}
		    #else
		    	bt_page_scan(1);
		    #endif
				bt_power_max(0xFF);
		    }
		}
		else
		{
		    if(get_bt_prev_status() != BT_STATUS_SUSPEND)
			{
                 bt_printf("auto_conn_cnt:%d\n",user_val->auto_connection_counter);
                 user_val->auto_connection_counter--;
                 if(user_val->auto_connection_counter%2)
				 {
					 bt_puts("bt_page_scan444\n");
                     bt_page_scan(1);
                     sys_time_auto_connection(addr);
                 }
                 else
				 {
                     bt_page_scan(0);
                     user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR,6,addr);
                 }
		    }
		}
	}
	else if(reason == 0x0b)
	{
		bt_puts("Connection Exist\n");
		user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR,6,addr);
	}
	else if(reason == 0x06)
	{
		//connect continue after link missing
		//user_send_cmd_prepare(USER_CTRL_START_CONNEC_VIA_ADDR,6,addr);
	}
}

#if defined(_SUPPORT_LCM_DRIVE)
void LCM_Disp_CallNum(void)
{
    if(user_val->is_phone_number_come)
    {
        memset(lLcdStr,' ',16);
        lLcdStr[0]='T';
        lLcdStr[1]='E';
        lLcdStr[2]='L';
        lLcdStr[3]=':';
        memcpy(&lLcdStr[4],user_val->income_phone_num,user_val->phone_num_len);
        Lcm_Disp_8_16(2,0,lLcdStr,16);
    }
}
#endif

