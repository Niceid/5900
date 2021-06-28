#include "sdk_cfg.h"
#include "file_operate/file_op.h"
#include "dev_manage/dev_ctl.h"
#include "vm/vm.h"
#include "rcsp/rcsp_interface.h"
#include "bluetooth/avctp_user.h"
#include "setup.h"
#if defined(_SUPPORT_DISC_)
#include "disc/disc.h"
#endif
#include "key_drv_ad.h"

#if REC_EN
#include "encode/record.h"
#include "encode/encode.h"
#endif

#if defined(_SUPPORT_AUX1_)	
extern const struct task_info linein1_task_info;
#endif

struct task_info *curr_task; ///<当前处于前台运行的任务指针
const struct task_info *last_task_info;


///任务切换
const struct task_info *task_list[] AT(.task_info);
const struct task_info *task_list[] =
{
#if defined(_BL_CHANGE_SPECIAL_MODE_)
#if defined(_SUPPORT_AUX_)
    &linein_task_info,
#endif
#if defined(_SUPPORT_RADIO_)
	&fm_radio_task_info,
#endif
#if defined(DAB_MODLE)
	&dab_task_info,
#endif
#if defined(_SUPPORT_DISC_)
	&player_disc_task_info,
#endif

    &music_usb_task_info,
#if defined(_SUPPORT_SD_)
	&music_card_task_info,
#endif

#if defined(_SUPPORT_DISC_)
	&player_usb_task_info,
	&player_card_task_info,
#endif

#if defined(_SUPPORT_BT_)
    &bluetooth_task_info,
#endif
#if REC_MODE_EN
#if !defined(_ENABLE_RECORD_RADIO_)
#if !defined(_ENABLE_MIC_REC_)
	&rec_task_info, ///<REC
#endif
#endif
#endif
    &idle_task_info,

#elif defined(_BL1803B_SPECIAL_MODE_)
#if defined(_SUPPORT_AUX_)
    &linein_task_info,
#endif

#if defined(_SUPPORT_RADIO_)
	&fm_radio_task_info,
#endif

#if defined(_SUPPORT_BT_)
    &bluetooth_task_info,
#endif

#if defined(DAB_MODLE)
	&dab_task_info,
#endif

    &music_usb_task_info,
#if defined(_SUPPORT_SD_)
	&music_card_task_info,
#endif
#if REC_MODE_EN
#if !defined(_ENABLE_RECORD_RADIO_)
#if !defined(_ENABLE_MIC_REC_)
	&rec_task_info, ///<REC
#endif
#endif
#endif
    &idle_task_info,

#elif defined(_BL6606_SPECIAL_MODE_)
#if defined(_SUPPORT_AUX_)
    &linein_task_info,
#endif

#if defined(_SUPPORT_RADIO_)
	&fm_radio_task_info,
#endif

#if defined(DAB_MODLE)
	&dab_task_info,
#endif

#if defined(_SUPPORT_BT_)
	&bluetooth_task_info,
#endif

    &music_usb_task_info,
#if defined(_SUPPORT_SD_)
	&music_card_task_info,
#endif
#if REC_MODE_EN
#if !defined(_ENABLE_RECORD_RADIO_)
#if !defined(_ENABLE_MIC_REC_)
	&rec_task_info, ///<REC
#endif
#endif
#endif
    &idle_task_info,

#elif defined(_SUPPORT_SPECIAL_MODE_)
#if defined(_SUPPORT_AUX1_)
    &linein1_task_info,
#endif

#if defined(_SUPPORT_RADIO_)
	&fm_radio_task_info,
#endif

#if defined(_SUPPORT_DISC_)
	&player_disc_task_info,
#endif

    &music_usb_task_info,
#if defined(_SUPPORT_SD_)
	&music_card_task_info,
#endif

#if defined(_SUPPORT_DISC_)
	&player_usb_task_info,
	&player_card_task_info,
#endif

#if defined(_SUPPORT_BT_)
    &bluetooth_task_info,
#endif

#if defined(_SUPPORT_AUX_)
    &linein_task_info,
#endif

#if REC_MODE_EN
#if !defined(_ENABLE_RECORD_RADIO_)
#if !defined(_ENABLE_MIC_REC_)
	&rec_task_info, ///<REC
#endif
#endif
#endif
    &idle_task_info,

#else

#if defined(_SUPPORT_BT_)
#if !defined(_DISABLE_BT_MODE_)
    &bluetooth_task_info,
#endif
#endif
#if defined(_SUPPORT_CAN_MODEL_)
	&can_task_info,
#endif

#if defined(_SUPPORT_REVERSE_MODEL_)
	&reverse_task_info,
#endif

#if defined(_SUPPORT_RADIO_)
#if !defined(_DISABLE_RADIO_)
	&fm_radio_task_info,
#endif
#endif
#if defined(DAB_MODLE)
#if !defined (DISENABLE_DAB)
	&dab_task_info,
#endif
#endif
#if defined(_SUPPORT_TAPE_)
	&tape_task_info,
#endif

#if defined(_SUPPORT_MIC_OK_)
	&mic_task_info,
#endif

#if defined(_SUPPORT_DISC_)
	&player_disc_task_info,
#endif

    &music_usb_task_info,
#if defined(_SUPPORT_SD_)
	&music_card_task_info,
#endif

#if defined(_SUPPORT_DISC_)
	&player_usb_task_info,
	&player_card_task_info,
#endif
	
#if defined(_SUPPORT_AUX_)
#if !defined(_DISABLE_AUX_)
    &linein_task_info,
#endif    
#endif

#if REC_MODE_EN
#if !defined(_ENABLE_RECORD_RADIO_)
#if !defined(_ENABLE_MIC_REC_)
	&rec_task_info, ///<REC
#endif	
#endif
#endif
    &idle_task_info,
#endif    
};
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

static u32 music_usb_check(void)
{
	if(file_operate_get_total_phydev())
	{
		 task_manage_printf("-----------------music_usb_check11111\n");
		if((dev_get_online_status(usb))
	#ifdef	TD_ADD_IPONE_CHARGE
        &&(usb_ERR==FALSE)
	#endif
        )
		{
			return 1;
		}
	}
	task_manage_printf("---------------music_usb_check2222\n");
 #if !defined(ENBLE_USB_MEMORY)
	setup.usb_break = FALSE;
#endif
	return 0;
}

#if defined(_SUPPORT_SD_)
static u32 music_card_check(void)
{
	if(file_operate_get_total_phydev())
	{
		task_manage_printf("--------------------music_card_check11111\n");
		if(dev_get_online_status(sd1))
		{
			return 1;
		}
	}
	task_manage_printf("--------------------music_card_check2222\n");
#if !defined(ENBLE_SD_MEMORY)
	setup.sd_break = FALSE;
#endif
	return 0;
}
#endif

static u32 bluetooth_check(void)
{
#if 0
	switch(get_bt_connect_status())
	{
		case BT_STATUS_CONNECTING:
		case BT_STATUS_TAKEING_PHONE:
		case BT_STATUS_PLAYING_MUSIC:
			return 1;
	}
	return 0;
#else
    return 1;
#endif
}

#if defined(_SUPPORT_DISC_)
static u32 player_disc_check(void)
{
	printf("DETECT_DISC_CHECK:%d\n",DETECT_DISC_CHECK());
	if(DETECT_DISC_CHECK())
	{
		return 1;
	}
	else
	{
	#if !defined(_DISABLE_DISC_POWER_)
		if(global.disc_detect)
		{
			global.disc_detect = 0;
			return 1;
		}
	#endif
	}
	setup.disc_break &= (~_B0_);
	return 0;
}
static u32 player_usb_check(void)
{
	printf("DETECT_USB_CHECK:%d\n",DETECT_USB_CHECK());
#if defined(_SPECIAL_CHANGE_)
	if(setup.usb_out_delay)
	{
		return 1;
	}
	else
	{
		setup.disc_break &= (~_B1_);
		return 0;
	}
#else
	if(DETECT_USB_CHECK())
	{
		return 1;
	}
	setup.disc_break &= (~_B1_);
	return 0;
#endif
}
static u32 player_card_check(void)
{
	printf("DETECT_SD_CHECK:%d\n",DETECT_SD_CHECK());
	if(DETECT_SD_CHECK())
	{
		return 1;
	}
	setup.disc_break &= (~_B2_);
	return 0;
}
#endif


#if defined(_SUPPORT_TAPE_)
static u32 tape_check(void)
{
	if(TAPE_DETCET_CHECK())
	{
		return 1;
	}
	return 0;
}
#endif

#if defined(_SUPPORT_MIC_OK_)
static u32 mic_check(void)
{
	if(MIC_CHECK())
	{
		return 1;
	}
	return 0;
}
#endif

#if defined(_SL6812_AUX_DETECT_)
static u32 aux_check(void)
{
	if(AUX_DET_CHECK())
	{
		return 1;
	}
	return 0;
}
#endif

static const struct task_dev_info music_usb_task_dev =
{
    .name = MUSIC_USB_NAME,
    .dev_check = music_usb_check,
};

#if defined(_SUPPORT_SD_)
static const struct task_dev_info music_card_task_dev =
{
    .name = MUSIC_CARD_NAME,
    .dev_check = music_card_check,
};
#endif

#if defined(_SUPPORT_TAPE_)
static const struct task_dev_info tape_task_dev =
{
    .name = TAPE_TASK_NAME,
    .dev_check = tape_check,
};
#endif

#if defined(_SUPPORT_MIC_OK_)
static const struct task_dev_info mic_task_dev =
{
    .name = MIC_TASK_NAME,
    .dev_check = mic_check,
};
#endif

#if defined(_SL6812_AUX_DETECT_)
static const struct task_dev_info aux_task_dev =
{
    .name = LINEIN_TASK_NAME,
    .dev_check = aux_check,
};
#endif

#if defined(_SUPPORT_DISC_)
static const struct task_dev_info player_disc_task_dev =
{
    .name = PLAYER_DISC_TASK_NAME,
    .dev_check = player_disc_check,
};
static const struct task_dev_info player_usb_task_dev =
{
    .name = PLAYER_USB_TASK_NAME,
    .dev_check = player_usb_check,
};
static const struct task_dev_info player_card_task_dev =
{
    .name = PLAYER_CARD_TASK_NAME,
    .dev_check = player_card_check,
};
#endif

static const struct task_dev_info bluetooth_task_dev =
{
    .name = BT_TASK_NAME,
    .dev_check = bluetooth_check,
};

const struct task_dev_info record_task_2_dev =
{
    .name = "RECTask",
    .dev_check = file_operate_get_total_phydev,
};


const struct task_dev_info *task_connect_dev[] AT(.task_info);
const struct task_dev_info *task_connect_dev[] =
{
#if defined(_SUPPORT_BT_)
	&bluetooth_task_dev,
#endif
#if defined(_SUPPORT_TAPE_)
	&tape_task_dev,
#endif
#if defined(_SUPPORT_MIC_OK_)
	&mic_task_dev,
#endif
#if defined(_SL6812_AUX_DETECT_)
	&aux_task_dev,
#endif
    &music_usb_task_dev,
#if defined(_SUPPORT_SD_)
	&music_card_task_dev,
#endif
#if defined(_SUPPORT_DISC_)
	&player_disc_task_dev,
	&player_usb_task_dev,
	&player_card_task_dev,
#endif
#if REC_EN
    &record_task_2_dev,
#endif/*REC_EN*/
};
const int task_connect_dev_valsize = sizeof(task_connect_dev);

/*----------------------------------------------------------------------------*/
/**@brief  根据任务名字，获取任务指针
 @param  name：任务名字
 @return 指向传入任务名字的指针或者NULL
 @note
 */
/*----------------------------------------------------------------------------*/
static u32 __get_task(const char *name)
{
    u32 max_cnt;
    u32 task_cnt;

    max_cnt = sizeof(task_list) / sizeof(int);//24/4=6

    for (task_cnt = 0; task_cnt < max_cnt; task_cnt++)
    {
        if (0 == strcmp(task_list[task_cnt]->name, name))
        {
            task_manage_printf("get_task_cnt = %d\n", task_cnt);
            return task_cnt;
        }
    }
    return -1;
}

/*----------------------------------------------------------------------------*/
/**@brief  返回 0
 @param
 @return
 @note   for debug
 */
/*----------------------------------------------------------------------------*/
int check_return_false(void)
{
    return 0;
}
/*----------------------------------------------------------------------------*/
/**@brief  过滤外设检测失败的任务
 @param  : task_cnt--将要切换的任务id
 @return  确定运行的任务id
 @note  如果当前任务外设检测失败会自动切换到下一个任务
 */
/*----------------------------------------------------------------------------*/
static u32 skip_task_without_dev(u32 *task_cnt)
{
    u8 i;
    u32 max_cnt;
    u32 dev_cnt;

    max_cnt = (sizeof(task_list) / sizeof(int)) - 1;
    dev_cnt = sizeof(task_connect_dev) / sizeof(int);
    for (i = 0; i < dev_cnt; i++)
    {
        if (!strcmp(task_list[*task_cnt]->name, task_connect_dev[i]->name))
        {
            if(!task_connect_dev[i]->dev_check())
            {
                (*task_cnt)++;
                if(*task_cnt >= max_cnt)
                {
                    *task_cnt = 0;
                }
                if(skip_task_without_dev(task_cnt))
                    return TRUE;
                else
                    return FALSE;
            }
            else
            {
                return TRUE;
			}
        }
    }

    if (i == dev_cnt)
        return TRUE;

    return TRUE;
}

#ifdef TD_ADD_NEW_APP
u8 curr_work_mode=0;
u8 get_curr_work_mode(void)
{
	return curr_work_mode;
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief  任务执行
 @param  task_cnt:     任务idle
 @param  priv:     给启动任务的参数
 @param  cur_mode：切换方式
 @return  成功或者错误信息
 @note
 */
/*----------------------------------------------------------------------------*/
static RUN_TASK_STATUS run_task(u32 task_cnt, const void *priv, TASK_SWITCH_MODE cur_mode)
{
    u32 max_cnt;

    max_cnt = (sizeof(task_list) / sizeof(int)) - 1;
    if (cur_mode == SWITCH_SPEC_TASK)
    {
        if (task_list[task_cnt] == curr_task) ///<指定运行的任务，与当前正在运行的任务一致
            return RUN_TASK_ERR_HAS_RPT;

    }
    else if (cur_mode == SWITCH_NEXT_TASK)
    {
        task_cnt++;
        if (task_cnt >= max_cnt)
        {
            task_cnt = 0;
        }
    }
    else if (cur_mode == SWITCH_PRE_TASK)
    {
        task_cnt--;
        if (task_cnt == 0)
        {
            task_cnt = max_cnt - 1;
        }
    }
    else if (cur_mode == SWITCH_LAST_TASK)
    {

    }

    skip_task_without_dev(&task_cnt); //忽略设备不在线的任务
    task_manage_printf("*****Next_Task_Name2:%s\n", task_list[task_cnt]->name);

    task_list[task_cnt]->init((void *) (task_list[task_cnt]->name));
    curr_task = (struct task_info *) task_list[task_cnt];
#ifdef TD_ADD_NEW_APP
	curr_work_mode = task_cnt;
#endif

    return RUN_TASK_SUCC;
}

/*----------------------------------------------------------------------------*/
/**@brief  任务切换
 @param  task_name: 任务的名字
 @param  priv:     给启动任务的参数
 @param  cur_mode：切换方式
 @return  成功或者错误信息
 @note
 */
/*----------------------------------------------------------------------------*/
RUN_TASK_STATUS task_switch(const char *task_name, const void *priv, TASK_SWITCH_MODE cur_mode)
{
    RUN_TASK_STATUS status;
    struct task_info *task;
    u32 task_cnt;

    //GET_SYS_MEM_STAT();

    if (task_name != NULL)
    {
        task_cnt = __get_task(task_name);
        if (-1 == task_cnt)
        {
            task_manage_puts("t_sw_1\n");
            return RUN_TASK_ERR_NO_TAG;
        }
        task = (struct task_info *) task_list[task_cnt];
        if (task == curr_task)
        {
            task_manage_puts("t_sw_2\n");
            return RUN_TASK_ERR_HAS_RPT;
        }
    }
    else
    {
        if (curr_task != NULL)
        {
            task_cnt = __get_task(curr_task->name);
            if (-1 == task_cnt)
            {
                task_manage_puts("t_sw_3\n");
                return RUN_TASK_ERR_NO_TAG;
            }

			if(SWITCH_LAST_TASK == cur_mode)
		   	{
	  			if(!strcmp(last_task_info->name, curr_task->name))
			   	{
			       task_manage_puts("4444 --- RUN_TASK_ERR_HAS_RPT\n");
			       return RUN_TASK_ERR_HAS_RPT;
		       }
	       	}
        }
        else
        {
            ///<当前没有运行任务，也没指定需要运行的任务
            task_manage_puts("5555 --- RUN_TASK_ERR_NO_TAG\n");
            return RUN_TASK_ERR_NO_TAG;
        }

    }
    task_manage_puts("t_sw_4\n");

    if (curr_task != NULL)
    {
        curr_task->exit(); //退出相应任务
        task_manage_puts("\n\ncurr_task exit!!!\n\n");
        GET_SYS_MEM_STAT();
    }

    if (SWITCH_LAST_TASK == cur_mode)
    {
        task_manage_puts("last mode\n");
        task_cnt = __get_task(last_task_info->name);
    }
    else
    {
        last_task_info = curr_task;
    }
    ///dac_mute(0,1);    //要完全退出之前的模式再解mute，不然会上个模式的尾音
    status = run_task(task_cnt, priv, cur_mode);

	vm_check_all(1);

#if SUPPORT_APP_RCSP_EN
	rcsp_app_start(curr_task->name);
#endif

	return status;
}
/*----------------------------------------------------------------------------*/
/**@brief  删除一个任务
 @param  task_name：删除任务的名字
 @return
 @note
 */
/*----------------------------------------------------------------------------*/
void os_task_delete(char *task_name)
{
    if (os_task_del_req(task_name) != OS_TASK_NOT_EXIST)
    {
        os_taskq_post_event(task_name, 1, SYS_EVENT_DEL_TASK);
        do
        {
            OSTimeDly(1);
        }
        while (os_task_del_req(task_name) != OS_TASK_NOT_EXIST);
        task_manage_printf("delete task succ =%s \n", task_name);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief  把任务设置为可删除状态
 @return
 @note   此函数由被删除的任务自己调用设置，在释放完占用的资源后，调用此
 函数，进入此函数后，不再返回，整个任务从这里结束
 */
/*----------------------------------------------------------------------------*/
void os_task_ready_delete()
{
    os_task_del_res(OS_TASK_SELF);
}

/*----------------------------------------------------------------------------*/
/**@brief  比较是否为当前运行的任务
 @param    task_name: 比较任务的名字
 @return   1-yes，0-no
 @note
 */
/*----------------------------------------------------------------------------*/
u32 compare_task_name(const char *task_name)
{
    if (task_name != NULL)
    {
        if(0 == strcmp(task_name,curr_task->name))
        {
            //task_manage_puts("is curr_task\n");
            return 1;
        }
    }
    //task_manage_puts("not curr_task\n");
    return 0;
}

const char *get_last_task_name(void)
{
	return last_task_info->name;
}

void set_last_task_name(const char *task_name)
{
    u32 task_cnt;

    if(task_name)
	{
		task_cnt = __get_task(task_name);
		if(-1 != task_cnt)
		{
			last_task_info = (struct task_info *)task_list[task_cnt];
		}
	}
}

void task_manager_init(void)
{
	curr_task = NULL;
	last_task_info = NULL;
}

