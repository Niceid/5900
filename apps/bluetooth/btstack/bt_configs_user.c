
#include "sdk_cfg.h"
#include "bluetooth/avctp_user.h"
#include "aec/aec_api.h"
#include "common/app_cfg.h"
#include "dac/dac_api.h"
#include "vm/vm_api.h"
#include "common/msg.h"
#include "clock.h"
#include "key_drv/key_drv_ad.h"
#include "dac/dac.h"
#include "fcc_test.h"
#include "dev_manage/dev_ctl.h"
#include "dac/dac.h"
#include "dac/eq.h"
#include "rtc.h"
#include "rcsp/rcsp_interface.h"
#include "uart_param.h"
#include "sys_detect.h"
#include "rf_test_uart.h"
#include "music.h"
#include "setup.h"
#include "audio_drv.h"
#include "fm_band.h"
#if defined(_SUPPORT_DISC_)
#include "disc/disc.h"
#endif
#include "disp.h"
#include "ui_api.h"

extern volatile u8 low_power_flag;
extern u8 fast_test;

#if BT_TWS
static u8 tws_channel_select=0;
#endif
static void in_low_pwr_port_deal(u8 mode) sec(.poweroff_text);
static void in_low_pwr_port_deal(u8 mode)
{
	if(mode == 2)   //enter sleep mode
	{
		JL_PORTA->DIR = 0xffff;
		JL_PORTA->PU = 0;
		JL_PORTA->PD = 0;
		JL_PORTA->DIE = 0;

		/* JL_PORTA->DIR |= 0xffdf; */
		/* JL_PORTA->PU &= ~0xffdf; */
		/* JL_PORTA->PD &= ~0xffdf; */
		/* JL_PORTA->DIE &= ~0xffdf; */
	}
	else
	{
		JL_PORTA->DIR |= 0xffe7;
		JL_PORTA->PU &= ~0xffe7;
		JL_PORTA->PD &= ~0xffe7;
		JL_PORTA->DIE &= ~0xffe7;

		JL_PORTA->OUT &= ~BIT(4);  //close led
	}

    JL_PORTB->DIR = 0xffff;
    JL_PORTB->PU  = 0;
    JL_PORTB->PD  = 0;
    JL_PORTB->DIE = 0;

    JL_PORTC->DIR = 0xffff;
    JL_PORTC->PU  = 0;
    JL_PORTC->PD  = 0;
    JL_PORTC->DIE = 0;

	/*注意spi flash的几个脚不能关*/
	JL_PORTD->DIR |= 0xfff0;
	JL_PORTD->PU  &= ~0xfff0;
	JL_PORTD->PD  &= ~0xfff0;
	JL_PORTD->DIE &= ~0xfff0;

    /////////////////usb///////////////////
    USB_DP_DIR(1); //DP设置为输入
    USB_DM_DIR(1); //DP设置为输入
    USB_DP_PD(0);
    USB_DM_PD(0);
    USB_DM_PU(0);
    USB_DP_PU(0);
    JL_USB->CON0 = 0;
}

static u32 sniff_time_cnt =0;
/*不属于用户接口，协议栈回调函数*/
extern void io_key_init(void);
extern void ad_key0_init_io();
extern u8 is_pwrtimer_en();
static void out_low_pwr_deal(u32 time_ms)
{
	/* printf("%d\n",time_ms); */

#if KEY_IO_EN
    io_key_init();
#endif

#if KEY_AD_EN
	ad_key0_init_io();
#endif


	if(is_pwrtimer_en())
	{
		return;
	}

	sniff_time_cnt += time_ms;
	/* printf("%d\n",sniff_time_cnt); */
	if(sniff_time_cnt>=3000)
	{
		sniff_time_cnt =0;
	}else{
	}

   // printf("sniff_time_cnt:%d  time_ms:%d\n",sniff_time_cnt,time_ms);
}
static void bt_updata_run()
{
    extern void bt_update_close_echo();
    bt_update_close_echo();
	puts("-------bt_updata_run\n");
	os_taskq_post_msg("MainTask",3,MSG_UPDATA,BT_UPDATA, NULL);
}
static void low_pwr_deal(u8 mode,u32 timer_ms) sec(.poweroff_text);
static void low_pwr_deal(u8 mode,u32 timer_ms)
{
	if(mode)
    {
		in_low_pwr_port_deal(mode);
	}
	else
	{
		out_low_pwr_deal(timer_ms);
	}
}

///---sdp service record profile- 用户选择支持协议--///
#define USER_SUPPORT_PROFILE_SPP    0

#if defined(_DISABLE_BT_TALKING_)
#define USER_SUPPORT_PROFILE_HFP    0
#else
#define USER_SUPPORT_PROFILE_HFP    1
#endif

#define USER_SUPPORT_PROFILE_A2DP   1
#define USER_SUPPORT_PROFILE_AVCTP  1
#define USER_SUPPORT_PROFILE_HID    0
static void bt_profile_select_init(void)
{
    u8 support_profile;
    support_profile = SPP_CH | HFP_CH | A2DP_CH | AVCTP_CH | HID_CH | AVDTP_CH;
#if (USER_SUPPORT_PROFILE_HFP==0)
    support_profile &= ~HFP_CH;
#endif
#if (USER_SUPPORT_PROFILE_AVCTP==0)
    support_profile &= ~AVCTP_CH;
#endif
#if (USER_SUPPORT_PROFILE_A2DP==0)
    support_profile &= ~A2DP_CH;
    support_profile &= ~AVCTP_CH;
    support_profile &= ~AVDTP_CH;
#endif
#if (USER_SUPPORT_PROFILE_SPP==0)
    support_profile &= ~SPP_CH;
#endif
#if (USER_SUPPORT_PROFILE_HID== 0)
    support_profile &= ~HID_CH;
#endif

    bt_cfg_default_init(support_profile);/*外部设置支持什么协议*/

#if BT_HID_INDEPENDENT_MODE
    set_hid_independent_info();//要在 bt_cfg_default_init()设置后
#endif

}
#if BT_TWS
void set_tws_channel_type(u8 type)
{
	tws_channel_select = type;
}
u8 get_tws_channel_type()
{
	return tws_channel_select;
}
#endif

void bt_save_osc_r_info(u8 r_val)
{
	u8 r_info;
    r_info = r_val;
    vm_write_api(VM_BT_OSC_INT_R,&r_info);
	printf("\n------osc_r_info=0x%x\n",r_info);
}

void bt_save_osc_l_info(u8 l_val)
{
	u8 l_info;
    l_info = l_val;
    vm_write_api(VM_BT_OSC_INT_L,&l_info);
	printf("\n------osc_l_info=0x%x\n",l_info);
}

extern u8 get_osc_internal_r(void);
extern u8 get_osc_internal_l(void);
void set_osc_2_vm(void)
{
	u8 osc_r_val = 0xff;
	u8 osc_l_val = 0xff;

	osc_r_val = get_osc_internal_r();
	osc_l_val = get_osc_internal_l();

	if((osc_r_val <= 0x1f)&&(osc_l_val <= 0x1f))
	{
 		bt_save_osc_r_info(osc_r_val);
 		bt_save_osc_l_info(osc_l_val);
	}
	printf("\n\nwrite_osc_info = 0x%x,0x%x\n",osc_r_val,osc_l_val);
}

#if BT_TWS
//保存对箱做主机还是做从机的信息到vm
static void bt_save_stereo_info(u8 stereo_type)
{
	u8 stereo_info;
    stereo_info = stereo_type;
    vm_write_api(VM_BT_STEREO_INFO,&stereo_info );
	printf("------stereo_info=0x%x\n",stereo_info );
}
#endif
///对箱主连接成功之后，同步系统音量到从机
static void stereo_sys_vol_sync()
{
     stereo_host_cmd_handle(MSG_VOL_STEREO,dac_ctl.sys_vol_l);
#if EQ_EN
     os_taskq_post_msg("btmsg", 1, MSG_BT_SYNC_STEREO_EQ);
#endif
}

#if BT_TWS
///对箱命令处理
static void stereo_deal_cmd(u8 msg,u8 value)
{
    if(msg < MSG_MAIN_MAX)
    {
         os_taskq_post_msg("MainTask", 2, msg,value);
    }
    else if(keymsg_task_name)
    {
        os_taskq_post_msg(keymsg_task_name, 2, msg,value);
    }
}
#endif
#if(BT_MODE == NORMAL_MODE)
	static const char host_name[]="AC69-Classic";
#else
	static const char host_name[]="AC69-RAM";
#endif
static const char hid_name[]="AC69_HID";
extern void __bt_set_background_conn_type_flag(u8 flag);

static void bt_function_select_init(void)
{
    /*蓝牙功能流程选择配置*/
#if(BT_MODE==NORMAL_MODE)
    u8 debug_addr[6] = {0x88, 0x67, 0x66, 0x89, 0x33, 0x11};
	__set_use_default_value(0);
#else
    u8 debug_addr[6] = {0x11, 0x22, 0x33, 0x33, 0x22, 0x11};
	__set_use_default_value(1);
#endif
    __set_host_name(host_name,sizeof(host_name));
    __set_hid_name(hid_name,sizeof(hid_name));
#if defined(_ENABLE_USE_PIN_CODE_)
    __set_pin_code("1234");
#else
    __set_pin_code("0000");
#endif
    __set_bt_mac_addr(debug_addr);
	__set_host_name_idx(0);

    bt_profile_select_init();

    __bt_set_sniff(SNIFF_MODE_CONF,SNIFF_CNT_TIME);/*设置进入sniff是进入poweroff还是powerdown，设置空闲多少s进入sniff*/

#if defined(_DISABLE_BATTERY_DISPLAY_)
	__bt_set_update_battery_time(0); /*设置电量显示发送更新的周期时间，为0表示关闭电量显示功能，单位毫秒，u32, 不能小于5000ms*/
#else
    __bt_set_update_battery_time(10000); /*设置电量显示发送更新的周期时间，为0表示关闭电量显示功能，单位毫秒，u32, 不能小于5000ms*/
#endif
	__bt_set_a2dp_auto_play_flag(0); /*高级音频连接完成自动播放歌曲使能, 0不使能，1使能*/
#if defined(ENABLE_PIN_CODE)
	__set_simple_pair_flag(0);		 /*提供接口外部设置配对方式,1使能简易配对。0使用pin code, 会使用配置文件的值*/
#else
	__set_simple_pair_flag(1);       /*提供接口外部设置配对方式,1使能简易配对。0使用pin code, 会使用配置文件的值*/
#endif

	__set_esco_packet_type(1);//2020.07.15，修改蓝牙挂断会延时

#if BT_TWS
	__set_stereo_scan_key(BT_TWS_SCAN_ENBLE);
    if(get_tws_channel_type()==0)
	{
		__set_stereo_role_disconnect_phone(0);/*从机已被手机连接，当主机来连接从机时，从机是否断开于手机的连接.为0时，从机会打断正在与主机连接的链路，做回主去反回连*/
	}
	else
	{
	    __set_stereo_role_disconnect_phone(1);
        __set_stereo_device_channel(get_tws_channel_type());
	}
	__set_sbc_cap_bitpool(38);
	__set_user_ctrl_conn_num(2);	 /*用户设置支持连接的个数，1 或 2*/
	__set_auto_conn_device_num(2);	 /*该接口用于设置上电回连需要依次搜索设备的个数。0表示上电不回连。大于20无效，直到连上一个*/
#if BT_HID_INDEPENDENT_MODE
	if(__get_hid_independent_flag())
	{
		__set_stereo_device_indicate("HD");/*设置对箱搜索标识,support两个字符标识，inquiry时候用,搜索到相应的标识才允许连接*/
	}
	else
#endif
	{
		__set_stereo_device_indicate("MT");/*设置对箱搜索标识,support两个字符标识，inquiry时候用,搜索到相应的标识才允许连接*/
	}
	__set_searche_timeout_value(12000); /*设置蓝牙搜索的时间。ms单位。蓝牙搜索用*/
	__set_connect_stereo_timeout_value(0); /*设置回连对箱超时时间，0为不超时，直到连接上对箱为止*/
	__set_start_connet_stereo(1);		   /*设置开机是否回连对箱*/
	__set_soundtrack_compound(1);		   /*设置soundtrack,对箱左右声道分离使能*/
	__set_indicate_name_cmp_en(0);		/*对箱相同蓝牙名字不进行配使能*/
#else

	__set_sbc_cap_bitpool(53);
#if defined(TD_ADD_SPP)
	__set_user_ctrl_conn_num(1);	 /*用户设置支持连接的个数，1 或 2*/
	__set_auto_conn_device_num(1);	 /*该接口用于设置上电回连需要依次搜索设备的个数。0表示上电不回连。大于20无效，直到连上一个*/
#else
#if defined(ENABLE_TWO_PHONE_CONNECT)
	__set_user_ctrl_conn_num(2);	 /*用户设置支持连接的个数，1 或 2*/
	__set_auto_conn_device_num(2);	 /*该接口用于设置上电回连需要依次搜索设备的个数。0表示上电不回连。大于20无效，直到连上一个*/
#else
	__set_user_ctrl_conn_num(1);	 /*用户设置支持连接的个数，1 或 2*/
	__set_auto_conn_device_num(1);	 /*该接口用于设置上电回连需要依次搜索设备的个数。0表示上电不回连。大于20无效，直到连上一个*/
#endif
#endif
#endif
	__set_page_timeout_value(5000); /*回连搜索时间长度设置,可使用该函数注册使用，ms单位,u16*/
#if BT_HID_INDEPENDENT_MODE
    __set_hid_auto_disconn_en(0);    /*是否自动断开手机的HID连接,1会断开，0是保留HID连接*/
#else
    __set_hid_auto_disconn_en(1);    /*是否自动断开手机的HID连接,1会断开，0是保留HID连接*/
#endif
    __set_test_masetr_enable(1);     /*接口配置是测试盒模式还是蓝牙SDK*/
    __set_super_timeout_value(8000); /*回连时超时参数设置。ms单位。做主机有效*/
    __set_connect_scan_timeout_value(0);/*//设置开启蓝牙可发现可连接的时间，为了省电，设置一个自动关闭可连接的时间。ms单位。为 0 时不使用自动关闭*/
    //__set_connect_scan_with_key(0);     /*连接上一台之后，连接第二台是否通过按键开启connect_scan*/
	__bt_set_music_back_play_flag(0);
    __set_music_break_in_flag(1);  /* 音频抢断开关，0：不抢断，1：抢断*/
    __set_hfp_switch(1);             /*通话抢断开关，0：不抢断，1：抢断*/
#if BT_TWS
	__bt_set_background_conn_type_flag(0);
#else
	__bt_set_background_conn_type_flag(0);
#endif

//2020.07.12修改SIRI功能有些手机返回状态不对
	__change_hci_class_type(BD_CLASS_WEARABLE_HEADSET);/*改变蓝牙设备类型，可以改变连接成功显示图标*/
	//__change_hci_class_type(BD_CLASS_HEADPHONES);/*改变蓝牙设备类型，可以改变连接成功显示图标*/

	__set_pll_div_mode(0);/*0:integer_div,1:decimal_div*/
}

/*电话时改变音量的状态值，注意是电话音量操作接口使用后会有反馈*/
static void call_vol_change(int vol)
{
	//iphone(0-15level)
    printf("vol %d",vol);
}
/*用接口读取名字时会从该回调函数返回，默认连接上就有一次回调，
要使用远端设备名字的可以copy保存*/
static void bt_read_remote_name(u8 *name)
{
    puts("\n remote name : ");
    puts((char *)name);
    puts("\n");
}

/*返回一个标志，有需要可以添加操作，*/
static void bt_is_auto_conn(u8 auto_conn_flag)
{   //区分回连成功还是被连成功
    if(auto_conn_flag)
    {
        //回连成功
    }else{
        //被连成功
    }
}

extern int spp_mutex_init(void);
extern int spp_mutex_del(void);

extern void ble_enter_mutex(void);
extern void ble_exit_mutex(void);
#ifdef	TD_ADD_NEW_APP
u8 spec_mode=0;
u8 high_light_flag=FALSE;
u8 high_light_channel = 0;
extern u8 crc_checksum(u8 *data,u16 len);
extern void new_app_radio_bandinfo(u8 band,u8 channel,u16 freq);
#endif
#ifdef	TD_ADD_SPP
#include "audio_drv.h"
#include "disp.h"
u8 sppbuf[30];
u8 spp_connect_falg=0;

u8 gAppRefreshTime=0;
u8 gRefresh_BT_Disp=FALSE;
u8 gRefresh_Audio=0;
void Spp_Tx_Radio_Info(uint16 freq);
extern void ui_show_sel(func_sel_e func_sel);
extern uint8 audio_reg_ctrl(uint8 ctrl, int para);
extern void audio_reg_set(void);

extern void  Notify_data_send(u8 *data,u16 len);

void Spp_Tx_Idle(void)
{
#ifdef	TD_ADD_NEW_APP
	sppbuf[0] = 0xff;
	sppbuf[1] = 0xaa;
	sppbuf[2] = 0x5;
	sppbuf[3] ='Z';
	sppbuf[4] ='B';
	sppbuf[5] =0x0;//0:idle 1:radio 2:disc 3:aux 4:bt 5:usb 6:sd
	sppbuf[6] =0x1;//0:无 1:audio 2:vedio 3:pic
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
    OSTimeDly(15);
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='I';
	sppbuf[4] ='D';
	sppbuf[5] ='L';
	sppbuf[6] ='E';
	bt_spp_send_data((u8 *)sppbuf, 7);
	Notify_data_send(sppbuf,7);
#endif
}
void Spp_Tx_FM(void)
{
#ifdef	TD_ADD_NEW_APP
	sppbuf[0] = 0xff;
	sppbuf[1] = 0xaa;
	sppbuf[2] = 0x5;
	sppbuf[3] ='Z';
	sppbuf[4] ='B';
	sppbuf[5] =0x1;//0:idle 1:radio 2:disc 3:aux 4:bt 5:usb 6:sd
	sppbuf[6] =0x1;//0:无 1:audio 2:vedio 3:pic
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
    OSTimeDly(15);
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='R';
	sppbuf[4] ='A';
	sppbuf[5] ='D';
	sppbuf[6] ='I';
	sppbuf[7] ='O';
	bt_spp_send_data((u8 *)sppbuf, 8);
	Notify_data_send(sppbuf,8);
#endif
}
void Spp_Tx_Usb(void)
{
#ifdef	TD_ADD_NEW_APP
	sppbuf[0] = 0xff;
	sppbuf[1] = 0xaa;
	sppbuf[2] = 0x5;
	sppbuf[3] ='Z';
	sppbuf[4] ='B';
	sppbuf[5] =0x5;//0:idle 1:radio 2:disc 3:aux 4:bt 5:usb 6:sd
	sppbuf[6] =0x1;//0:无 1:audio 2:vedio 3:pic
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
    OSTimeDly(15);
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='U';
	sppbuf[4] ='S';
	sppbuf[5] ='B';
	bt_spp_send_data((u8 *)sppbuf, 6);
	Notify_data_send(sppbuf,6);
#endif
}
void Spp_Tx_Sd(void)
{
#ifdef	TD_ADD_NEW_APP
	sppbuf[0] = 0xff;
	sppbuf[1] = 0xaa;
	sppbuf[2] = 0x5;
	sppbuf[3] ='Z';
	sppbuf[4] ='B';
	sppbuf[5] =0x6;//0:idle 1:radio 2:disc 3:aux 4:bt 5:usb 6:sd
	sppbuf[6] =0x1;//0:无 1:audio 2:vedio 3:pic
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
    OSTimeDly(15);
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='S';
	sppbuf[4] ='D';
	bt_spp_send_data((u8 *)sppbuf, 5);
	Notify_data_send(sppbuf,5);
#endif
}
void Spp_Tx_Music(void)
{
	if(music_device == USB_DEVICE)
	{
		Spp_Tx_Usb();
	}
	else if(music_device == SD1_DEVICE)
	{
		Spp_Tx_Sd();
	}
}
void Spp_Tx_BT(void)
{
#ifdef	TD_ADD_NEW_APP
	sppbuf[0] = 0xff;
	sppbuf[1] = 0xaa;
	sppbuf[2] = 0x5;
	sppbuf[3] ='Z';
	sppbuf[4] ='B';
	sppbuf[5] =0x4;//0:idle 1:radio 2:disc 3:aux 4:bt 5:usb 6:sd
	sppbuf[6] =0x1;//0:无 1:audio 2:vedio 3:pic
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
    OSTimeDly(15);
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='B';
	sppbuf[4] ='L';
	sppbuf[5] ='U';
	sppbuf[6] ='E';
	sppbuf[7] ='T';
	sppbuf[8] ='O';
	sppbuf[9] ='O';
	sppbuf[10] ='T';
	sppbuf[11] ='H';
	bt_spp_send_data((u8 *)sppbuf, 12);
	Notify_data_send(sppbuf,12);
#endif
    gRefresh_BT_Disp =TRUE;
}
void Spp_Tx_Aux(void)
{
#ifdef	TD_ADD_NEW_APP
	sppbuf[0] = 0xff;
	sppbuf[1] = 0xaa;
	sppbuf[2] = 0x5;
	sppbuf[3] ='Z';
	sppbuf[4] ='B';
	sppbuf[5] =0x3;//0:idle 1:radio 2:disc 3:aux 4:bt 5:usb 6:sd
	sppbuf[6] =0x1;//0:无 1:audio 2:vedio 3:pic
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
    OSTimeDly(15);
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='A';
	sppbuf[4] ='U';
	sppbuf[5] ='X';
	bt_spp_send_data((u8 *)sppbuf, 6);
	Notify_data_send(sppbuf,6);
#endif
}
void Spp_Tx_AccOff(void)
{
#ifndef	TD_ADD_NEW_APP
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='A';
	sppbuf[4] ='C';
	sppbuf[5] ='C';
	sppbuf[6] ='O';
	sppbuf[7] ='F';
	sppbuf[8] ='F';
	bt_spp_send_data((u8 *)sppbuf, 9);
	Notify_data_send(sppbuf,9);
#endif
}

void Spp_Tx_dab(void)
{
#ifdef	TD_ADD_NEW_APP
	sppbuf[0] = 0xff;
	sppbuf[1] = 0xaa;
	sppbuf[2] = 0x5;
	sppbuf[3] ='Z';
	sppbuf[4] ='B';
	sppbuf[5] =0x9;//0:idle 1:radio 2:disc 3:aux 4:bt 5:usb 6:sd 9:dab
	sppbuf[6] =0x2;//0:无 1:audio 2:vedio 3:pic
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='D';
	sppbuf[4] ='A';
	sppbuf[5] ='B';
	bt_spp_send_data((u8 *)sppbuf, 6);
	Notify_data_send(sppbuf,6);
#endif
}

#if defined(_SUPPORT_TAPE_)
void Spp_Tx_Tape(void)
{
#ifdef	TD_ADD_NEW_APP
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='T';
	sppbuf[4] ='A';
	sppbuf[5] ='P';
	sppbuf[6] ='E';
	bt_spp_send_data((u8 *)sppbuf, 7);
	Notify_data_send(sppbuf,7);
#endif
}
#endif

#if defined(_SUPPORT_MIC_OK_)
void Spp_Tx_Mic(void)
{
#ifdef	TD_ADD_NEW_APP
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='M';
	sppbuf[4] ='I';
	sppbuf[5] ='C';
	sppbuf[6] ='I';
	sppbuf[7] ='N';
	bt_spp_send_data((u8 *)sppbuf, 8);
	Notify_data_send(sppbuf,8);
#endif
}
#endif

void Spp_Tx_disc(void)
{
#ifdef	TD_ADD_NEW_APP
	sppbuf[0] = 0xff;
	sppbuf[1] = 0xaa;
	sppbuf[2] = 0x5;
	sppbuf[3] ='Z';
	sppbuf[4] ='B';
	sppbuf[5] =0x2;//0:idle 1:radio 2:disc 3:aux 4:bt 5:usb 6:sd
	sppbuf[6] =0x1;//0:无 1:audio 2:vedio 3:pic
	sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='D';
	sppbuf[4] ='I';
	sppbuf[5] ='S';
	sppbuf[6] ='C';
	bt_spp_send_data((u8 *)sppbuf, 7);
	Notify_data_send(sppbuf,7);
#endif
}

void Spp_Tx_WorkMode(void)
{
    if(compare_task_name(IDLE_TASK_NAME))
	{
        Spp_Tx_Idle();
	}
    else if(compare_task_name(FM_TASK_NAME))
	{
        Spp_Tx_FM();
        OSTimeDly(5);
    	Spp_Tx_Radio_Info(GET_BAND_TEMP_FREQ);
	}
    //else if(compare_task_name(MUSIC_TASK_NAME))
	//{
    //    Spp_Tx_Music();
	//}
	else if(compare_task_name(MUSIC_USB_NAME))
	{
        Spp_Tx_Usb();
	}
	else if(compare_task_name(MUSIC_CARD_NAME))
	{
       Spp_Tx_Sd();
	}
    else if(compare_task_name(LINEIN_TASK_NAME))
	{
        Spp_Tx_Aux();
	}
    else if(compare_task_name(BT_TASK_NAME))
	{
        Spp_Tx_BT();
	}
#if defined(DAB_MODLE)
	else if(compare_task_name(DAB_TASK_NAME))
	{
        Spp_Tx_dab();
	}
#endif
#if defined(_SUPPORT_TAPE_)
    else if(compare_task_name(TAPE_TASK_NAME))
    {
    	Spp_Tx_Tape();
    }
#endif
#if defined(_SUPPORT_MIC_OK_)
    else if(compare_task_name(MIC_TASK_NAME))
    {
    	Spp_Tx_Mic();
    }
#endif
#if defined(_SUPPORT_DISC_)
	else if(compare_task_name(PLAYER_DISC_TASK_NAME))
	{
        Spp_Tx_disc();
	}
	else if(compare_task_name(PLAYER_USB_TASK_NAME))
	{
		Spp_Tx_Usb();
	}
	else if(compare_task_name(PLAYER_CARD_TASK_NAME))
	{
		Spp_Tx_Sd();
	}
#endif
}
void Spp_Tx_Ack(void)
{
#ifdef	TD_ADD_NEW_APP
#else
	sppbuf[0] ='A';
	sppbuf[1] ='T';
	sppbuf[2] ='+';
	sppbuf[3] ='X';
	sppbuf[4] ='I';
	sppbuf[5] ='A';
	sppbuf[6] ='F';
	sppbuf[7] ='E';
	sppbuf[8] ='I';
	sppbuf[9] ='H';
	sppbuf[10] ='A';
	sppbuf[11] ='N';
	sppbuf[12] ='G';
	bt_spp_send_data((u8 *)sppbuf,13);
	Notify_data_send(sppbuf,13);
#endif
}

typedef struct{

	/*音量*/
	uint8 aud_vol;
	/*响度开关*/
	uint8 aud_loud;
	/*低音*/
	int8  aud_bass;
	/*高音*/
	int8  aud_treble;
#if defined(_FAD_BAL_FUNTION_)
	int8  aud_fl;
	int8  aud_fr;
	int8  aud_rl;
	int8  aud_rr;
#else
	/*左右平衡*/
	int8  aud_bal;
	/*前后平衡*/
	int8  aud_fad;
#endif
	/*音效*/
	uint8  aud_eq;
}app_info;

app_info app;
#ifdef	TD_ADD_NEW_APP
u8 audio_vol_compare(void)
{
	if(app.aud_vol !=setup.aud_vol)//当前音量值
	{
		app.aud_vol =setup.aud_vol;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
#endif

u8  Audio_info_compare(void)
{
	u8 mm=0;
#ifndef	TD_ADD_NEW_APP
	if(app.aud_vol !=setup.aud_vol)//当前音量值
	{
		app.aud_vol =setup.aud_vol;
		mm++;
	}
#endif
	if(app.aud_loud !=setup.aud_loud)//0:没有打开LOUD开关  1:打开LOUD开关
	{
		app.aud_loud =setup.aud_loud;
		mm++;
	}
	if(app.aud_bass !=(setup.aud_bass+AUDIO_BASS_MAX))//低音值 0~+14
	{
		app.aud_bass =(setup.aud_bass+AUDIO_BASS_MAX);
		mm++;
	}
	if(app.aud_treble !=(setup.aud_treble+AUDIO_TREBLE_MAX))//高音0~+14
	{
		app.aud_treble =(setup.aud_treble+AUDIO_TREBLE_MAX);
		mm++;
	}
#if defined(_FAD_BAL_FUNTION_)
    if(app.aud_fl !=setup.aud_fl)
    {
    	app.aud_fl =setup.aud_fl;
    	mm++;
    }
    if(app.aud_fr !=setup.aud_fr)
    {
    	app.aud_fr =setup.aud_fr;
    	mm++;
    }
    if(app.aud_rl !=setup.aud_rl)
    {
    	app.aud_rl =setup.aud_rl;
    	mm++;
    }
    if(app.aud_rr !=setup.aud_rr)
    {
    	app.aud_rr =setup.aud_rr;
    	mm++;
    }
#else
	if(app.aud_bal !=(setup.aud_bal+AUDIO_ATTEN_WIDE))//左右 0~14或者0~+20
	{
		app.aud_bal =(setup.aud_bal+AUDIO_ATTEN_WIDE);
		mm++;
	}
	if(app.aud_fad !=(setup.aud_fad+AUDIO_ATTEN_WIDE))//前后0~14或者0-~+20
	{
		app.aud_fad =(setup.aud_fad+AUDIO_ATTEN_WIDE);
		mm++;
	}
#endif

	if(app.aud_eq !=setup.aud_eq)//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
	{
		app.aud_eq =setup.aud_eq;
		mm++;
	}
	return mm;
}

void Spp_Tx_Audio(void)
{
	printf("Spp_Tx_Audio\n");
#ifdef 	TD_ADD_NEW_APP
#if defined(_AUDIO_DRIVER_AP3768_)
    sppbuf[0] =0xff;
    sppbuf[1] =0xaa;//音效范围初始化信息
    sppbuf[2] = 6;
    sppbuf[3] ='Z';
    sppbuf[4] ='F';
    sppbuf[5] =0x05;//0:无2313 ;1:有2313;2:2348;3:37534;4:7419;5:3768;6:102
    sppbuf[6] =0x00;//
    sppbuf[7] = setup.aud_eq;
    sppbuf[8] = crc_checksum((u8 *)sppbuf,8);
    Notify_data_send(sppbuf,9);
#else
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;//音效范围初始化信息
	sppbuf[2] = 12;
	sppbuf[3] ='Z';
	sppbuf[4] ='C';
	sppbuf[5] =0x01;//0:无2313 1:有2313
	sppbuf[6] =0x00;//
#if defined(TD_ADD_LCD_NUM_4)
    sppbuf[7] =14;
	sppbuf[8] =14;
#else
	sppbuf[7] =20;
	sppbuf[8] =20;
#endif
	//sppbuf[7] =(setup.aud_bal+AUDIO_ATTEN_WIDE);
	//sppbuf[8] =(setup.aud_fad+AUDIO_ATTEN_WIDE);
	sppbuf[9] =setup.aud_loud;//0:没有打开LOUD开关  1:打开LOUD开关
	sppbuf[10] =14;
	sppbuf[11] =14;
	//sppbuf[10] =(setup.aud_bass+AUDIO_BASS_MAX);
	//sppbuf[11] =(setup.aud_treble+AUDIO_TREBLE_MAX);
	sppbuf[12] =0xee;//无mid功能
	sppbuf[13] =setup.aud_eq;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
	sppbuf[14] = crc_checksum((u8 *)sppbuf,14);
	Notify_data_send(sppbuf,15);
#endif
#else
	/*
	app.aud_vol =setup.aud_vol;//当前音量值
	app.aud_loud =setup.aud_loud;//0:没有打开LOUD开关  1:打开LOUD开关
	app.aud_bass =(setup.aud_bass+AUDIO_BASS_MAX);//低音值 0~+14
	app.aud_treble =(setup.aud_treble+AUDIO_TREBLE_MAX);//高音0~+14
	app.aud_bal =(setup.aud_bal+AUDIO_ATTEN_WIDE);//左右 0~14或者0~+20
	app.aud_fad =(setup.aud_fad+AUDIO_ATTEN_WIDE);//前后0~14或者0-~+20
	app.aud_eq =setup.aud_eq;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
	*/
	sppbuf[0] ='B';//B版本
	sppbuf[1] =0x01;//音效相关信息
	sppbuf[2] =11;//命令长度
#if defined(_SUPPORT_AUDIO_DRIVER_)
#if defined(_NO_FAD_FUNTION_)
	sppbuf[3] =0;//0:没有2313  1:有2313
#else
	sppbuf[3] =1;//0:没有2313  1:有2313
#endif
#else
	sppbuf[3] =0;//0:没有2313  1:有2313
#endif
	sppbuf[4] =AUDIO_VOLUME_MAX;//音量最大值
	sppbuf[5] =(AUDIO_ATTEN_WIDE*2);//前后左右最大值 0~20或者0~14
	sppbuf[6] =setup.aud_vol;//当前音量值
	sppbuf[7] =setup.aud_loud;//0:没有打开LOUD开关  1:打开LOUD开关
	sppbuf[8] =(setup.aud_bass+AUDIO_BASS_MAX);//低音值 0~+14
	sppbuf[9] =(setup.aud_treble+AUDIO_TREBLE_MAX);//高音0~+14
	sppbuf[10] =(setup.aud_bal+AUDIO_ATTEN_WIDE);//左右 0~14或者0~+20
	sppbuf[11] =(setup.aud_fad+AUDIO_ATTEN_WIDE);//前后0~14或者0-~+20
	sppbuf[12] =setup.aud_eq;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
	sppbuf[13] =(AUDIO_BASS_MAX*2);//高低音最大值 0~20或者0~14
	bt_spp_send_data((u8 *)sppbuf,14);
	Notify_data_send(sppbuf,14);
#endif
	gRefresh_Audio=1;
}
void Spp_Tx_Audio_ble(void)
{
	printf("Spp_Tx_Audio_ble\n");
#ifdef 	TD_ADD_NEW_APP
#if defined(_AUDIO_DRIVER_AP3768_)
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;//音效范围初始化信息
	sppbuf[2] = 6;
	sppbuf[3] ='Z';
	sppbuf[4] ='F';
	sppbuf[5] =0x05;//0:无2313 ;1:有2313;2:2348;3:37534;4:7419;5:3768;6:102
	sppbuf[6] =0x00;//
	switch(setup.aud_eq)
	{
    	case AUDIO_EQ_OFF:
    		sppbuf[7] =0;
    		break;
    	case AUDIO_EQ_POP:
    		sppbuf[7] =2;
    		break;
    	case AUDIO_EQ_CLASSIC:
    		sppbuf[7] =3;
    		break;
    	case AUDIO_EQ_ROCK:
    		sppbuf[7] =5;
    		break;
    	case AUDIO_EQ_JAZZ:
    		sppbuf[7] =1;
    		break;
    	case AUDIO_EQ_FLAT:
    		sppbuf[7] =4;
    		break;
		case AUDIO_EQ_PEOPLE:
			sppbuf[7] =8;
			break;
		case AUDIO_EQ_SOFT:
			sppbuf[7] =6;
			break;
		case AUDIO_EQ_ELECTRIC:
			sppbuf[7] =7;
			break;
    }
    sppbuf[8] = crc_checksum((u8 *)sppbuf,8);
    Notify_data_send(sppbuf,9);
#else
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;//音效范围初始化信息
	sppbuf[2] = 12;
	sppbuf[3] ='Z';
	sppbuf[4] ='C';
	sppbuf[5] =0x01;//0:无2313 1:有2313
	sppbuf[6] =0x1;//
	sppbuf[7] =(setup.aud_bal+AUDIO_ATTEN_WIDE);
	sppbuf[8] =(setup.aud_fad+AUDIO_ATTEN_WIDE);
	sppbuf[9] =setup.aud_loud;//0:没有打开LOUD开关  1:打开LOUD开关
	sppbuf[10] =(setup.aud_bass+AUDIO_BASS_MAX);
	sppbuf[11] =(setup.aud_treble+AUDIO_TREBLE_MAX);
	sppbuf[12] =0xee;//无mid功能
	switch(setup.aud_eq)
	{
		case AUDIO_EQ_OFF:
			sppbuf[13] =0;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
			break;
		case AUDIO_EQ_POP:
			sppbuf[13] =1;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
			break;
		case AUDIO_EQ_CLASSIC:
			sppbuf[13] =2;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
			break;
		case AUDIO_EQ_ROCK:
			sppbuf[13] =3;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
			break;
		case AUDIO_EQ_JAZZ:
            sppbuf[13] =4;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
            break;
        case AUDIO_EQ_FLAT:
            sppbuf[13] =5;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
            break;
    }
    sppbuf[14] = crc_checksum((u8 *)sppbuf,14);
    Notify_data_send(sppbuf,15);
#endif
#else
	/*
	app.aud_vol =setup.aud_vol;//当前音量值
	app.aud_loud =setup.aud_loud;//0:没有打开LOUD开关  1:打开LOUD开关
	app.aud_bass =(setup.aud_bass+AUDIO_BASS_MAX);//低音值 0~+14
	app.aud_treble =(setup.aud_treble+AUDIO_TREBLE_MAX);//高音0~+14
	app.aud_bal =(setup.aud_bal+AUDIO_ATTEN_WIDE);//左右 0~14或者0~+20
	app.aud_fad =(setup.aud_fad+AUDIO_ATTEN_WIDE);//前后0~14或者0-~+20
	app.aud_eq =setup.aud_eq;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
	*/
	sppbuf[0] ='B';//B版本
	sppbuf[1] =0x01;//音效相关信息
	sppbuf[2] =11;//命令长度
#if defined(_SUPPORT_AUDIO_DRIVER_)
#if defined(_NO_FAD_FUNTION_)
	sppbuf[3] =0;//0:没有2313  1:有2313
#else
	sppbuf[3] =1;//0:没有2313  1:有2313
#endif
#else
	sppbuf[3] =0;//0:没有2313  1:有2313
#endif
	sppbuf[4] =AUDIO_VOLUME_MAX;//音量最大值
	sppbuf[5] =(AUDIO_ATTEN_WIDE*2);//前后左右最大值 0~20或者0~14
	sppbuf[6] =setup.aud_vol;//当前音量值
	sppbuf[7] =setup.aud_loud;//0:没有打开LOUD开关  1:打开LOUD开关
	sppbuf[8] =(setup.aud_bass+AUDIO_BASS_MAX);//低音值 0~+14
	sppbuf[9] =(setup.aud_treble+AUDIO_TREBLE_MAX);//高音0~+14
	sppbuf[10] =(setup.aud_bal+AUDIO_ATTEN_WIDE);//左右 0~14或者0~+20
	sppbuf[11] =(setup.aud_fad+AUDIO_ATTEN_WIDE);//前后0~14或者0-~+20
	sppbuf[12] =setup.aud_eq;//eq 0:eq off 1:pop 2:class 3:rock  4:jazz 5:flat
	sppbuf[13] =(AUDIO_BASS_MAX*2);//高低音最大值 0~20或者0~14
	Notify_data_send(sppbuf,14);
#endif
	gRefresh_Audio=1;
}

void Spp_Tx_Radio_Info(uint16 freq)
{
#if defined(_ENABLE_FEIYIN_APP_)
	uint16 index_freq;
#endif

#ifdef TD_ADD_NEW_APP
	if((high_light_flag == TRUE)&&(high_light_channel != GET_BAND_FREQ_INDEX))
	{
		high_light_flag = FALSE;
		new_app_radio_bandinfo(GET_RADIO_BAND,high_light_channel,GET_BAND_FREQ(high_light_channel));
	}
	sppbuf[0]=0xff;
	sppbuf[1]=0xaa;
	sppbuf[2]=0x6;
	sppbuf[3]='Z';
	sppbuf[4]='D';
	sppbuf[5]=0x2;
	sppbuf[6]=((freq>>8)&0xff);//频率高字节
	sppbuf[7]=(freq&0xff);//频率低字节
	sppbuf[8] = crc_checksum((u8 *)sppbuf,8);
	Notify_data_send(sppbuf,9);
#else
#if defined(_ENABLE_FEIYIN_APP_)
    sppbuf[0] ='B';
    sppbuf[1] =0x02;//收音相关信息
    sppbuf[2] =3;
    sppbuf[3] =GET_RADIO_BAND;//0:FM1 1:FM2 2:FM3 3:AM1 4:AM2
    sppbuf[4] =((freq>>8)&0xff);//频率高字节
    sppbuf[5] =(freq&0xff);//频率低字节
    bt_spp_send_data((u8 *)sppbuf,6);
    Notify_data_send(sppbuf,6);

	sppbuf[0] ='B';
	sppbuf[1] =0x02;//收音相关信息
	sppbuf[2] =3;
    sppbuf[3] =(0x20+GET_BAND_FREQ_INDEX);//0:FM1 1:FM2 2:FM3 3:AM1 4:AM2
	sppbuf[4] =((freq>>8)&0xff);//频率高字节
	sppbuf[5] =(freq&0xff);//频率低字节
	bt_spp_send_data((u8 *)sppbuf,6);
	Notify_data_send(sppbuf,6);

	for(u8 i=1;i<7;i++)
    {
		index_freq = GET_BAND_FREQ(i);
		sppbuf[0] ='B';
		sppbuf[1] =0x02;//收音相关信息
		sppbuf[2] =3;
		sppbuf[3] =(0x10+i);//0:FM1 1:FM2 2:FM3 3:AM1 4:AM2
		sppbuf[4] =((index_freq>>8)&0xff);//频率高字节
		sppbuf[5] =(index_freq&0xff);//频率低字节
		bt_spp_send_data((u8 *)sppbuf,6);
		Notify_data_send(sppbuf,6);
	}
#else
    sppbuf[0] ='B';
    sppbuf[1] =0x02;//收音相关信息
    sppbuf[2] =3;
    sppbuf[3] =GET_RADIO_BAND;//0:FM1 1:FM2 2:FM3 3:AM1 4:AM2
    sppbuf[4] =((freq>>8)&0xff);//频率高字节
    sppbuf[5] =(freq&0xff);//频率低字节
    bt_spp_send_data((u8 *)sppbuf,6);
    Notify_data_send(sppbuf,6);
#endif
#endif
}

void Spp_Tx_MusicInfo(uint8 curdev,uint32 total_trk,uint32 Cur_trk,uint32 total_time,uint32 Curplaytime)
{
	sppbuf[0] ='B';
	sppbuf[1] =0x03;//放音相关信息
	sppbuf[2] =17;
    sppbuf[3] =curdev;////当前设备 1:sd 2:usb 3:disc

	sppbuf[4] =((total_trk>>24)&0xff);//总曲目
	sppbuf[5] =((total_trk>>16)&0xff);
	sppbuf[6] =((total_trk>>8)&0xff);
	sppbuf[7] =(total_trk&0xff);

	sppbuf[8] =((Cur_trk>>24)&0xff);//当前曲目
	sppbuf[9] =((Cur_trk>>16)&0xff);
	sppbuf[10] =((Cur_trk>>8)&0xff);
	sppbuf[11] =(Cur_trk&0xff);

	sppbuf[12] =((total_time>>24)&0xff);//总时间
	sppbuf[13] =((total_time>>16)&0xff);
	sppbuf[14] =((total_time>>8)&0xff);
	sppbuf[15] =(total_time&0xff);

	sppbuf[16] =((Curplaytime>>24)&0xff);//当前时间
	sppbuf[17] =((Curplaytime>>16)&0xff);
	sppbuf[18] =((Curplaytime>>8)&0xff);
	sppbuf[19] =(Curplaytime&0xff);
	sppbuf[20] =0;//媒体类型 0：无  1：Audio  2:Vedio  3:	Pic
	sppbuf[21] =0;//状态 0：暂停  1:播放

	//bt_spp_send_data((u8 *)sppbuf,22);
	Notify_data_send(sppbuf,22);
}

/*
void Spp_Tx_bt_Info(uint8 playFlag,uint32 total_time,uint32 Curplaytime)
{
	sppbuf[0] ='B';
	sppbuf[1] =0x04;//蓝牙相关信息
	sppbuf[2] =9;
    sppbuf[3] =playFlag;//0蓝牙连接上 1:蓝牙暂停 2:蓝牙播放
    sppbuf[4] =((total_time>>24)&0xff);//总时间
	sppbuf[5] =((total_time>>16)&0xff);
	sppbuf[6] =((total_time>>8)&0xff);
	sppbuf[7] =(total_time&0xff);
	sppbuf[8] =((Curplaytime>>24)&0xff);//当前时间
	sppbuf[9] =((Curplaytime>>16)&0xff);
	sppbuf[10] =((Curplaytime>>8)&0xff);
	sppbuf[11] =(Curplaytime&0xff);
	bt_spp_send_data((u8 *)sppbuf,12);
	Notify_data_send(sppbuf,12);
}
*/
void Spp_Tx_bt_Info(uint8 playFlag)
{
#ifdef	TD_ADD_NEW_APP
#else
	sppbuf[0] ='B';
	sppbuf[1] =0x04;//蓝牙相关信息
	sppbuf[2] =9;
	sppbuf[3] =playFlag;//0蓝牙连接上 1:蓝牙暂停 2:蓝牙播放
	bt_spp_send_data((u8 *)sppbuf,4);
	Notify_data_send(sppbuf,4);
#endif
}

char tbl_spp_sig[4] = {0xff,0x55,0x03,0x02};
char tbl_spp_func[2] = {0xaa,0x55};
u8 tbl_spp_msg2[24][3] = {
	{0x01,0x01,0xf9}, // OK		0
	{0x00,0x01,0xfa},//PWR		1
	{0x0a,0x01,0xf0},//vol +		2
	{0x02,0x01,0xf8},//MUTE		3
    {0x09,0x01,0xf1}, //VOL -       7
    {0x03,0x01,0xf7}, //MODE        8
    {0x29,0x01,0xd1},   //SEL       9
    {0x05,0x01,0xf5},//EQ           10
	{0x31,0x01,0xC9}, // LOUD

	{0x06,0x01,0xf4},//PREV		4
	{0x08,0x01,0xf2},//PLAY		5
	{0x07,0x01,0xf3},//NEXT		6
	{0x33,0x01,0xC7},//BAND		11
	{0x25,0x01,0xd5},//AMS		12
	{0x26,0x01,0xd4},//ST		13
	{0x0c,0x01,0xee},// 1		14
	{0x0d,0x01,0xed},// 2		15
	{0x0e,0x01,0xec},// 3		16
	{0x0f,0x01,0xeb},// 4		17
	{0x10,0x01,0xea},// 5 		18
	{0x11,0x01,0xe9},// 6		19
	{0x04,0x01,0xF6},// RPT		20
	{0x30,0x01,0xCA},//RDM		21
	{0x32,0x01,0xC8}, // INT		22
};
int tbl_spp_msg1[9] = {
	SYS_EVENT_SPP_ACK,
	MSG_SYS_POWER,
	MSG_VOL_ADD,
	MSG_SYS_MUTE,
    MSG_VOL_SUB,
    MSG_SYS_MODE,
    MSG_USER_SEL,
    MSG_AUD_EQ,
	MSG_AUD_LOUD,
};
#ifdef	TD_ADD_NEW_APP
int tbl_spp_radiokey[28] = {
	MSG_FM_PREV_SEEK,
	MSG_FM_BROWSE,
	MSG_FM_NEXT_SEEK,
	MSG_FM_BAND,
	MSG_FM_AUTO_SAVE,
	MSG_FM_ST,
	0,
	MSG_FM_M1,
	MSG_FM_M2,
	MSG_FM_M3,
	MSG_FM_M4,
	MSG_FM_M5,
	MSG_FM_M6,
	0,
	0,
	0,
	0,
	0,
	0,
	MSG_FM_NEXT_STEP1,
	MSG_FM_PREV_STEP1,
	0,
	MSG_FM_S1,
	MSG_FM_S2,
	MSG_FM_S3,
	MSG_FM_S4,
	MSG_FM_S5,
	MSG_FM_S6,
};
int tbl_spp_musickey[19] = {
	(int)MSG_MUSIC_PREV_FILE,
	(int)MSG_MUSIC_PP,
	(int)MSG_MUSIC_NEXT_FILE,
	MSG_MUSIC_ID3,
	MSG_MUSIC_SEARCH,
	0,
	0,
	(int)MSG_MUSIC_PP,
	MSG_MUSIC_INT,
	(int)MSG_MUSIC_RPT,
	MSG_MUSIC_RDM,
	MSG_MUSIC_SUB_10,
	MSG_MUSIC_ADD_10,
	0,
	0,
	0,
	(int)MSG_MUSIC_RPT1,
	MSG_MUSIC_RDM,
	MSG_MUSIC_INT,
};
#else
int tbl_spp_radiokey[15] = {
	MSG_FM_PREV_SEEK,
	MSG_FM_SCAN,
	MSG_FM_NEXT_SEEK,
	MSG_FM_BAND,
	MSG_FM_AUTO_SAVE,
	MSG_FM_ST,
	MSG_FM_M1,
	MSG_FM_M2,
	MSG_FM_M3,
	MSG_FM_M4,
	MSG_FM_M5,
	MSG_FM_M6,
	0,
	0,
	0,
};
int tbl_spp_musickey[15] = {
	(int)MSG_MUSIC_PREV_FILE,
	(int)MSG_MUSIC_PP,
	(int)MSG_MUSIC_NEXT_FILE,
	MSG_MUSIC_ID3,
	MSG_MUSIC_SEARCH,
	0,
	(int)MSG_MUSIC_PP,
	MSG_MUSIC_INT,
	(int)MSG_MUSIC_RPT,
	MSG_MUSIC_RDM,
	MSG_MUSIC_SUB_10,
	MSG_MUSIC_ADD_10,
	(int)MSG_MUSIC_RPT,
	MSG_MUSIC_RDM,
	MSG_MUSIC_INT,
};
#endif
u8 tbl_spp_btkey[15] = {
	MSG_BT_PREV_FILE,
	MSG_BT_PP,
	MSG_BT_NEXT_FILE,
	0,
	0,
	0,
	MSG_BT_PP,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};
#if defined(DAB_MODLE)
int tbl_spp_dabkey[15] = {
	MSG_FM_PREV_SEEK,
	0,
	MSG_FM_NEXT_SEEK,
	MSG_FM_BAND,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};
#endif

void spp_rx_msg(u8 *ptr,u8 len)
{
//    printf("len:%d\n",len);
//    printf_buf(ptr,len);

	printf("app->-");
    for(u8 i=0;i<len;i++)
    {
		printf(" %x ",ptr[i]);
	}
	printf("--\n");

	//printf("ptr[len]:%x\n",ptr[len-1]);

	if((global.aud_mute)&&(ptr[len-1] != 0xF8))
	{
		disp_send_msg(DISP_EVENT_MUTE);
	}

    if(len !=7)
		return;
	if(0 == memcmp(&ptr[0],(char *)tbl_spp_sig,4))
	{
		printf("spp_rx_msg111\n");
		for(u8 i=0;i!=24;i++)
		{
            if(0 == memcmp(&ptr[4], (char *)tbl_spp_msg2[i], 3))
            {
				if(i <=8)
				{
				#if defined(SYS_MODE_CHANG_NOPP)
					if(i ==5)
					{
						if(global.app_mode_enable==1)
						{
							global.app_mode_enable=0;
						}
						else
						{
							return;
						}
					}
				#endif
                	os_taskq_post_event(MAIN_TASK_NAME, 1, tbl_spp_msg1[i]);
				}
				else
				{
                    if(compare_task_name(FM_TASK_NAME))
                    {
	                    os_taskq_post_msg((char *)FM_TASK_NAME,1,tbl_spp_radiokey[i-9]);
                    }
                    else if((compare_task_name(MUSIC_USB_NAME))||(compare_task_name(MUSIC_CARD_NAME)))
                    {
	                    os_taskq_post_msg((char *)MUSIC_TASK_NAME,1,tbl_spp_musickey[i-9]);
                    }
                    else if(compare_task_name(BT_TASK_NAME))
                    {
	                    os_taskq_post_msg((char *)BT_TASK_NAME,1,tbl_spp_btkey[i-9]);
                    }
				#if defined(DAB_MODLE)
					else if(compare_task_name(DAB_TASK_NAME))
                    {
						//printf("DAB----%d",i-9);
	                    os_taskq_post_msg((char *)DAB_TASK_NAME,1,tbl_spp_dabkey[i-9]);
                    }
				#endif
				#if defined(_SUPPORT_DISC_)
					else if(compare_task_name(PLAYER_DISC_TASK_NAME))
					{
	                    os_taskq_post_msg((char *)PLAYER_DISC_TASK_NAME,1,tbl_spp_musickey[i-9]);
					}
					else if(compare_task_name(PLAYER_USB_TASK_NAME))
					{
	                    os_taskq_post_msg((char *)PLAYER_USB_TASK_NAME,1,tbl_spp_musickey[i-9]);
					}
					else if(compare_task_name(PLAYER_CARD_TASK_NAME))
					{
	                    os_taskq_post_msg((char *)PLAYER_CARD_TASK_NAME,1,tbl_spp_musickey[i-9]);
					}
				#endif
				}
            }
		}
	}
	else if(0 == memcmp(&ptr[0],(char *)tbl_spp_func,2))
	{
		printf("spp_rx_msg222\n");
		if(ptr[3] > AUDIO_VOLUME_MAX)return;
        if(ptr[2]==0x01)
		{
			setup.aud_vol =ptr[3];//音量
			if(get_cur_vol() <= AUDIO_VOLUME_MAX)
	        {
	        	set_cur_vol(setup.aud_vol);
            	disp_send_msg(DISP_EVENT_VOLUME);
			#if defined(_SUPPORT_AUDIO_DRIVER_)
                audio_reg_ctrl(AUD_CTRL_VOLUME,get_cur_vol());
                audio_reg_set();
			#else
				audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
                //audio_reg_set();
            #endif
	        }
		}
        else if(ptr[2]==0x02)
		{
			printf("spp_rx_msg333\n");
            //setup.aud_bass =ptr[3]-AUDIO_BASS_MAX;//bas
            //setup.aud_treble =ptr[4]-AUDIO_BASS_MAX;//tre
            //setup.aud_bal =ptr[5]-AUDIO_ATTEN_WIDE;//bal
            //setup.aud_fad =ptr[6]-AUDIO_ATTEN_WIDE;//fad

			//audio_reg_ctrl(AUD_CTRL_BASS,setup.aud_bass);
			//audio_reg_ctrl(AUD_CTRL_TREBLE,setup.aud_treble);
			//audio_reg_ctrl(AUD_CTRL_LR,setup.aud_bal);
			//audio_reg_ctrl(AUD_CTRL_FR,setup.aud_fad);
			//audio_reg_set();

			if(setup.aud_bass != (ptr[3]-AUDIO_BASS_MAX))
			{
				setup.aud_bass =ptr[3]-AUDIO_BASS_MAX;//bas
				ui_show_sel(FUNC_SEL_BASS);
			#if defined(_SUPPORT_AUDIO_DRIVER_)
				audio_reg_ctrl(AUD_CTRL_BASS,setup.aud_bass);
			#else
				audio_drv_ctrl(AUD_CTRL_BASS, setup.aud_bass, TRUE);
			#endif
			}
			if(setup.aud_treble !=(ptr[4]-AUDIO_BASS_MAX))
			{
				setup.aud_treble =ptr[4]-AUDIO_BASS_MAX;//tre
				ui_show_sel(FUNC_SEL_TREBLE);
			#if defined(_SUPPORT_AUDIO_DRIVER_)
				audio_reg_ctrl(AUD_CTRL_TREBLE,setup.aud_treble);
			#else
				audio_drv_ctrl(AUD_CTRL_TREBLE,setup.aud_treble, TRUE);
			#endif
			}
		#if !defined(_FAD_BAL_FUNTION_)
			if(setup.aud_bal !=(ptr[5]-AUDIO_ATTEN_WIDE))
			{
				printf("spp_rx_msg444\n");
				setup.aud_bal =ptr[5]-AUDIO_ATTEN_WIDE;//bal
				ui_show_sel(FUNC_SEL_BAL);
			#if defined(_SUPPORT_AUDIO_DRIVER_)
				audio_reg_ctrl(AUD_CTRL_LR,setup.aud_bal);
			#else
				audio_drv_ctrl(AUD_CTRL_LR,setup.aud_bal, TRUE);
			#endif
			}
			if(setup.aud_fad !=(ptr[6]-AUDIO_ATTEN_WIDE))
			{
				printf("spp_rx_msg555\n");
			#if !defined(_NO_FAD_FUNTION_)
				setup.aud_fad =ptr[6]-AUDIO_ATTEN_WIDE;//fad
				ui_show_sel(FUNC_SEL_FAD);
			#if defined(_SUPPORT_AUDIO_DRIVER_)
				audio_reg_ctrl(AUD_CTRL_FR,setup.aud_fad);
			#else
				audio_drv_ctrl(AUD_CTRL_FR,setup.aud_fad, TRUE);
			#endif
			#endif
			}
		#endif
			//ew_printf("setup.aud_bal:%d\n",setup.aud_bal);
			//ew_printf("setup.aud_fad:%d\n",setup.aud_fad);
		#if defined(_SUPPORT_AUDIO_DRIVER_)
			audio_reg_set();
		#endif
		}
	}
}
#endif

#ifdef	TD_ADD_NEW_APP
u8 read_list_ready_flag=FALSE;
u8 get_curr_work_mode(void);
u8 crc_checksum(u8 *data,u16 len)
{
    u16 check_sum=0;
	for(u8 i=2;i<len;i++)
	{
        check_sum += data[i];
        check_sum &= 0xff;
	}
	return ((u8)check_sum);
}

#if defined(_ENABLE_RADIO_RDS_)
void app_link_ok_reply(void)
{//应答app连接OK下发的握手指令
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
	sppbuf[2] =0x07;//长度
	sppbuf[3] ='A';
	sppbuf[4] ='S';
	sppbuf[5] ='O';
	sppbuf[6] ='K';
#if defined(_DISABLE_MUSIC_LIST_)
	sppbuf[7] =0x02;//0:双锭带屏  1:单锭DVD	2:其他
#else
	sppbuf[7] =0x01;//0:双锭带屏  1:单锭DVD	2:其他
#endif
	sppbuf[8] =0x01;//有无RDS
    sppbuf[9] = crc_checksum((u8 *)sppbuf,9);
	Notify_data_send(sppbuf,10);
}

void Spp_Tx_Radio_Rds(uint8 ta,uint8 af,uint8 pty)
{
	printf("ta:%d\n",ta);
	printf("af:%d\n",af);
	printf("pty:%d\n",pty);
	sppbuf[0]=0xff;
	sppbuf[1]=0xaa;
	sppbuf[2]=0x7;
	sppbuf[3]='Z';
	sppbuf[4]='D';
	sppbuf[5]=0x3;
	sppbuf[6]=ta;
	sppbuf[7]=af;
	sppbuf[8]=pty;
	sppbuf[9] = crc_checksum((u8 *)sppbuf,9);
	Notify_data_send(sppbuf,10);
}

#else
void app_link_ok_reply(void)
{//应答app连接OK下发的握手指令
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
	sppbuf[2] =0x06;//长度
	sppbuf[3] ='A';
	sppbuf[4] ='S';
	sppbuf[5] ='O';
	sppbuf[6] ='K';
#if defined(_DISABLE_MUSIC_LIST_)
	sppbuf[7] =0x2;//0:双锭带屏  1:单锭DVD	2:其他
#else
	sppbuf[7] =0x1;//0:双锭带屏  1:单锭DVD	2:其他
#endif
    sppbuf[8] = crc_checksum((u8 *)sppbuf,8);
	Notify_data_send(sppbuf,9);
}
#endif

void app_link_ok_reply_init_data(void)
{//app连接OK 发送初始化数据
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
	sppbuf[2] =0x8;//长度
	sppbuf[3] ='Z';
	sppbuf[4] ='S';
	sppbuf[5] =0x3d;//bit0:radio bit1:disc bit2:aux bit3:bt bit4:usb bit5:sd bit6:tv bit7:ipod
#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
	sppbuf[6] =0x10;//bit0:dab bit1:手机互联 bit2:本地媒体 bit3:云端媒体
#else
    sppbuf[6] =0x0;//bit0:dab bit1:手机互联 bit2:本地媒体 bit3:云端媒体
#endif
    switch(get_curr_work_mode())
    {
		case 0://bt
            sppbuf[7] = 0x04;
			break;
		case 1://radio
            sppbuf[7] = 0x01;
			break;
		case 2://usb
            sppbuf[7] = 0x05;
			break;
		case 3://sd
            sppbuf[7] = 0x06;
			break;
		case 4://aux
            sppbuf[7] = 0x03;
			break;
		case 5://idle
            sppbuf[7] = 0x0;
			break;
		default:
			break;
    }
#if defined(_AUDIO_DRIVER_AP3768_)
	sppbuf[8] =0x05;//0:无2313 ;1:有2313;2:2348;3:37534;4:7419;5:3768;6:102
#else
	sppbuf[8] = 0x01;// 1:2313
#endif
    sppbuf[9] = AUDIO_VOLUME_MAX;
    sppbuf[10] = crc_checksum((u8 *)sppbuf,10);
	Notify_data_send(sppbuf,11);
}
void new_app_radio_init(void)
{
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;//收音范围初始化信息
    sppbuf[2] = 17;
	sppbuf[3] ='Z';
	sppbuf[4] ='D';
    sppbuf[5] = 0x0;//初始化数据
	sppbuf[6] =((RADIO_AM_BAND_SIZE_MAX<<4)|RADIO_FM_BAND_SIZE_MAX);
	sppbuf[7] =18;
	sppbuf[8] =12;
	sppbuf[9] =((GetIndexBandMinFreq(setup.area)>>8)&0xff);
	sppbuf[10] =(GetIndexBandMinFreq(setup.area)&0xff);//fm 最小值
	sppbuf[11] =((GetIndexBandMaxFreq(setup.area)>>8)&0xff);
	sppbuf[12] =(GetIndexBandMaxFreq(setup.area)&0xff);//fm 最大值

	sppbuf[13] =((GetIndexBandMinFreq(setup.area+1)>>8)&0xff);
	sppbuf[14] =(GetIndexBandMinFreq(setup.area+1)&0xff);//am 最小值
	sppbuf[15] =((GetIndexBandMaxFreq(setup.area+1)>>8)&0xff);
	sppbuf[16] =(GetIndexBandMaxFreq(setup.area+1)&0xff);//am 最小值
	sppbuf[17] =GetIndexBandManualStep(setup.area);
	sppbuf[18] =GetIndexBandManualStep(setup.area+1);

    sppbuf[19] = crc_checksum((u8 *)sppbuf,19);
	Notify_data_send(sppbuf,20);
}
void new_app_send_vol(void)
{
	puts("new_app_send_vol\n");
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;//音效范围初始化信息
	sppbuf[2] = 5;
	sppbuf[3] ='Z';
	sppbuf[4] ='E';
	sppbuf[5] =setup.aud_vol;//0:无2313 1:有2313
    if(global.aud_mute ==0)
    {
		sppbuf[6] =1;
    }
    else
    {
		sppbuf[6] =0;
    }
    sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
	Notify_data_send(sppbuf,8);
	gRefresh_Audio=1;
}
void new_app_radio_bandinfo(u8 band,u8 channel,u16 freq)
{
    if(channel==0)
		return;//app没有做0通道
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
    sppbuf[2] = 10;
	sppbuf[3] ='Z';
	sppbuf[4] ='D';
    sppbuf[5] = 0x1;
    switch(band)
    {
		case 0://fm1
            sppbuf[6] = 0x1;
			break;
		case 1://fm2
            sppbuf[6] = 0x2;
			break;
		case 2://fm3
            sppbuf[6] = 0x3;
			break;
		case 3://am1
            sppbuf[6] = 0x10;
			break;
		case 4://am2
            sppbuf[6] = 0x20;
			break;
    }
	
	//printf("band:%d\n",sppbuf[6]);
	//printf("channel:%d\n",channel);
	//printf("GET_BAND_FREQ_INDEX:%d\n",GET_BAND_FREQ_INDEX);
    sppbuf[7] = channel;
    if(channel == GET_BAND_FREQ_INDEX)
    {
        high_light_flag=TRUE;
        high_light_channel = channel;
        sppbuf[8] = 1;
		//puts("OPEN LIGHT\n");
    }
    else
    {
        sppbuf[8] = 0;
		//puts("CLOSE LIGHT\n");
    }
	//printf("band_freq:%d\n",freq);
    sppbuf[9] = ((u8)((freq>>8)&0xff));
    sppbuf[10] = ((u8)(freq&0xff));
    sppbuf[11] = crc_checksum((u8 *)sppbuf,11);
	Notify_data_send(sppbuf,12);
}
void new_app_send_bandinfo(void)
{
    for(u8 i=1;i<7;i++)
    {
		new_app_radio_bandinfo(GET_RADIO_BAND,i,GET_BAND_FREQ(i));
		OSTimeDly(1);//2020.09.14
	}
}
void new_app_MusicInfo(uint8 curdev,uint32 total_trk,uint32 Cur_trk,uint32 total_time,uint32 Curplaytime,u8 rpt_mode,u8 playstate,u32 filesclust)
{
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;//放音相关信息
	sppbuf[2] =18;
	sppbuf[3] ='Z';
	sppbuf[4] ='A';
    sppbuf[5] =curdev;////当前设备 1:sd 2:usb 3:disc

	sppbuf[6] =((Cur_trk>>8)&0xff);//当前曲目
	sppbuf[7] =(Cur_trk&0xff);

	sppbuf[8] =((Curplaytime>>8)&0xff);//当前时间
	sppbuf[9] =(Curplaytime&0xff);

	sppbuf[10] =((total_trk>>8)&0xff);//总曲目
	sppbuf[11] =(total_trk&0xff);

	sppbuf[12] =((total_time>>8)&0xff);//总时间
	sppbuf[13] =(total_time&0xff);

	sppbuf[14] =playstate;//状态 0：暂停  1:播放
    switch(rpt_mode)
    {
		case REPEAT_ALL:
            sppbuf[15]=0x0;
			break;
		case REPEAT_ONE:
            sppbuf[15]=0x1;
			break;
		case REPEAT_FOLDER:
            sppbuf[15]=0x2;
			break;
		case REPEAT_RANDOM:
            sppbuf[15]=0x4;
			break;
		default:
			break;
    }
    sppbuf[16]=(filesclust&0xff);
    sppbuf[17]=((filesclust>>8)&0xff);
    sppbuf[18]=((filesclust>>16)&0xff);
    sppbuf[19]=((filesclust>>24)&0xff);
    sppbuf[20] = crc_checksum((u8 *)sppbuf,20);
	Notify_data_send(sppbuf,21);
}

#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
void new_app_send_light(void)
{
	puts("new_app_send_light\n");
	printf("Auto_On_Off:%d\n",setup.Auto_On_Off);
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
	sppbuf[2] = 9;
	sppbuf[3] ='Z';
	sppbuf[4] ='L';
#if defined(_ENABLE_LIGHT_SHADE_)
	sppbuf[5] =0x01;//调节模式:0跳变,1渐变
#else
	sppbuf[5] =0x00;//调节模式:0跳变,1渐变
#endif
	sppbuf[6] =setup.clour;
	sppbuf[7] =0xFF;
	sppbuf[8] =0xFF;
	sppbuf[9] =0xFF;
	sppbuf[10] =setup.Auto_On_Off;
    sppbuf[11] = crc_checksum((u8 *)sppbuf,11);
	Notify_data_send(sppbuf,12);
}
#endif

#if defined(_ENABLE_LIGHT_SHADE_)
void new_app_shade_light(void)
{
	puts("new_app_shade_light\n");
	printf("Auto_On_Off:%d\n",setup.Auto_On_Off);
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
	sppbuf[2] = 9;
	sppbuf[3] ='Z';
	sppbuf[4] ='L';
	sppbuf[5] =0x01;//调节模式:0跳变,1渐变
	sppbuf[6] =setup.clour;
	sppbuf[7] =setup.red;
	sppbuf[8] =setup.green;
	sppbuf[9] =setup.blue;
	sppbuf[10] =setup.Auto_On_Off;
    sppbuf[11] = crc_checksum((u8 *)sppbuf,11);
	Notify_data_send(sppbuf,12);
}
#endif

#if defined(_FAD_BAL_FUNTION_)
extern int8  fl;
extern int8  fr;
extern int8  rl;
extern int8  rr;
#endif

#if defined(_ENABLE_LIGHT_SHADE_)
extern void LightDriverIrCmd(uint8 cmd,uint8 red,uint8 green,uint8 blue);
#endif


void new_app_rx_msg(u8 *ptr,u8 len)
{
	printf("app_recv<");
    for(u8 i=0;i<len;i++)
    {//打印接收到的app数据
		printf(" %x ",ptr[i]);
	}
	printf(">len:%d\n",len);

	if((global.aud_mute)&&(ptr[5] != 0x02))
	{
		disp_send_msg(DISP_EVENT_MUTE);
	}

	if((ptr[0] == 0xff)&&(ptr[1] == 0xaa)&&(ptr[2] == (len-3)))
	{
		switch(ptr[3])
		{
			case 'A':
                if(ptr[4] == 'S')
                {//app连上蓝牙后主动下发的握手协议
                    os_taskq_post_event(MAIN_TASK_NAME, 1,SYS_EVENT_SPP_ACK);
				#if defined(_ENABLE_A2DP_TIME_)
					global.a2dp_sync = TRUE;
					global.a2dp_count = 0;
				#endif
                }
				break;
			case 'Z':
                switch(ptr[4])
                {
                    case 'A'://获取文件列表
                        os_taskq_post_msg(MUSIC_TASK_NAME,6,SYS_EVENT_GET_FILE_LIST,ptr[5],ptr[6],ptr[7],ptr[8],ptr[9]);
                    	break;
					case 'B'://模式切换指令
                        if(ptr[5] == 0x0)
                        {//idle模式
                            spec_mode = 0x1;
                        }
                        else if(ptr[5] == 0x1)
                        {//radio模式
                            spec_mode = 0x2;
                        }
                        else if(ptr[5] == 0x3)
                        {//aux模式
                            spec_mode = 0x3;
                        }
                        else if(ptr[5] == 0x4)
                        {//bt模式
                            spec_mode = 0x4;
                        }
                        else if(ptr[5] == 0x5)
                        {//usb模式
                            spec_mode = 0x5;
                        }
                        else if(ptr[5] == 0x6)
                        {//sd模式
                            spec_mode = 0x6;
                        }
                        os_taskq_post_msg(MAIN_TASK_NAME, 1,SYS_EVENT_CHANGE_MODE);
						break;
					case 'C'://音效数据
					#if defined(_FAD_BAL_FUNTION_)

					#else
                        if(setup.aud_bal != (ptr[5]-AUDIO_ATTEN_WIDE))
                        {
							setup.aud_bal =ptr[5]-AUDIO_ATTEN_WIDE;//bal
							ui_show_sel(FUNC_SEL_BAL);
							audio_reg_ctrl(AUD_CTRL_LR,setup.aud_bal);
						}
                        if(setup.aud_fad !=(ptr[6]-AUDIO_ATTEN_WIDE))
                        {
                            setup.aud_fad =ptr[6]-AUDIO_ATTEN_WIDE;//fad
                            ui_show_sel(FUNC_SEL_FAD);
                            audio_reg_ctrl(AUD_CTRL_FR,setup.aud_fad);
                        }
					#endif
                        if(setup.aud_loud != ptr[7])
                        {
                            setup.aud_loud =ptr[7];//loud
                            ui_show_sel(FUNC_SEL_LOUD);
                            audio_reg_ctrl(AUD_CTRL_LOUD, setup.aud_loud);
                        }
                        if(setup.aud_bass != (ptr[10]-AUDIO_BASS_MAX))
                        {
                            setup.aud_bass =ptr[10]-AUDIO_BASS_MAX;//bas
                            ui_show_sel(FUNC_SEL_BASS);
                            audio_reg_ctrl(AUD_CTRL_BASS,setup.aud_bass);
                        }
                        if(setup.aud_treble !=(ptr[11]-AUDIO_BASS_MAX))
                        {
                            setup.aud_treble =ptr[11]-AUDIO_BASS_MAX;//tre
                            ui_show_sel(FUNC_SEL_TREBLE);
                            audio_reg_ctrl(AUD_CTRL_TREBLE,setup.aud_treble);
                        }
                        audio_reg_set();
						break;
                    case 'D':
                        if(ptr[5] ==0x2)
                        {
                        #if defined(_ENABLE_APP_FREQ_)
							os_taskq_post_msg(MAIN_TASK_NAME,3,MSG_RADIO_FREQ,ptr[6],ptr[7]);
						#else
                        	os_taskq_post_msg(FM_TASK_NAME,3,MSG_FM_SELECT_FREQ,ptr[6],ptr[7]);
						#endif
                        }
					#if defined(_ENABLE_RADIO_RDS_)
						else if(ptr[5] ==0x3)
						{
							global.rds_info = TRUE;
							//printf("save_rds_ta:%d\n",global.save_rds_ta);
							//printf("save_rds_af:%d\n",global.save_rds_af);
							//printf("save_rds_pty:%d\n",global.save_rds_pty);
							if(global.save_rds_ta != ptr[6])
							{
								global.save_rds_ta = ptr[6];
								os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_RDS_TA);
							}
							if(global.save_rds_af != ptr[7])
							{
								global.save_rds_af = ptr[7];
								os_taskq_post_msg(MAIN_TASK_NAME, 1, MSG_RDS_AF);
							}
							
							printf("save_rds_pty:%d\n",global.save_rds_pty);
							if(global.save_rds_pty != ptr[8])
							{
								global.save_rds_pty = ptr[8];//相同数据只接收一次
								global.save_rds_pty1 = (ptr[8]+1);//显示ID加1
								printf("save_rds_pty111:%d\n",global.save_rds_pty);
								printf("global.pty_type:%d\n",global.pty_type);
								if(global.pty_type == 0)
								{
									if(((ptr[8]<15)&&(ptr[8]>8))||((ptr[8]<28)&&(ptr[8]>22)))
									{
										global.pty_type = 1;
									}
									else
									{
										global.pty_type = 2;
									}
									os_taskq_post_msg(FM_TASK_NAME, 1, MSG_RDS_PTY);
									OSTimeDly(1);
									os_taskq_post_msg(FM_TASK_NAME, 1, MSG_RDS_PTY_TYPE);
								}
								else if(global.pty_type == 1)//音乐组
								{
									if(((ptr[8]<15)&&(ptr[8]>8))||((ptr[8]<28)&&(ptr[8]>22)))
									{
										os_taskq_post_msg(FM_TASK_NAME, 1, MSG_RDS_PTY_TYPE);
									}
									else
									{
										global.pty_type = 2;
										os_taskq_post_msg(FM_TASK_NAME, 1, MSG_RDS_PTY);
										OSTimeDly(1);
										os_taskq_post_msg(FM_TASK_NAME, 1, MSG_RDS_PTY_TYPE);
									}
								}
								else if(global.pty_type == 2)//语言组
								{
									if(((ptr[8]<15)&&(ptr[8]>8))||((ptr[8]<28)&&(ptr[8]>22)))
									{
										global.pty_type = 1;
										os_taskq_post_msg(FM_TASK_NAME, 1, MSG_RDS_PTY);
										OSTimeDly(1);
										os_taskq_post_msg(FM_TASK_NAME, 1, MSG_RDS_PTY_TYPE);
									}
									else
									{
										os_taskq_post_msg(FM_TASK_NAME, 1, MSG_RDS_PTY_TYPE);
									}
								}
							}
						}
					#endif
                    	break;
					case 'E':
                        if(setup.aud_vol !=ptr[5])
                        {
                            setup.aud_vol = ptr[5];
							ui_show_sel(FUNC_SEL_VOLUME);
                            audio_reg_ctrl(AUD_CTRL_VOLUME,setup.aud_vol);
                            if(global.aud_mute)
                            {
                                disp_send_msg(DISP_EVENT_MUTE);
                            }
                        }
                        else if(global.aud_mute != ptr[6])
                        {
                            ui_show_mute(global.aud_mute);
                        }
                        audio_reg_set();
						break;
                    case 'F':
                        if(ptr[5] == 0)
                        {
                            switch(ptr[6])
                            {
							#if defined(_AUDIO_DRIVER_AP3768_)
    							case 0:
    								setup.aud_eq =AUDIO_EQ_OFF;
    								break;
    							case 1:
    								setup.aud_eq =AUDIO_EQ_JAZZ;
    								break;
    							case 2:
    								setup.aud_eq =AUDIO_EQ_POP;
    								break;
    							case 3:
    								setup.aud_eq =AUDIO_EQ_CLASSIC;
    								break;
    							case 4:
    								setup.aud_eq =AUDIO_EQ_FLAT;
    								break;
    							case 5:
    								setup.aud_eq =AUDIO_EQ_ROCK;
    								break;
								case 6:
									setup.aud_eq =AUDIO_EQ_SOFT;
									break;
								case 7:
									setup.aud_eq =AUDIO_EQ_ELECTRIC;
									break;
    							case 8:
    								setup.aud_eq =AUDIO_EQ_PEOPLE;
    								break;
							#else
								case 0:
                                    setup.aud_eq =AUDIO_EQ_OFF;
									break;
								case 1:
                                    setup.aud_eq =AUDIO_EQ_POP;
									break;
								case 2:
                                    setup.aud_eq =AUDIO_EQ_CLASSIC;
									break;
								case 3:
                                    setup.aud_eq =AUDIO_EQ_ROCK;
									break;
								case 4:
                                    setup.aud_eq =AUDIO_EQ_JAZZ;
									break;
								case 5:
                                    setup.aud_eq =AUDIO_EQ_FLAT;
									break;
							#endif
                            }
                            ui_show_sel(FUNC_SEL_EQ);
                            audio_reg_ctrl(AUD_CTRL_EQ,setup.aud_eq);
							audio_reg_set();
                        }
					#if defined(_AUDIO_DRIVER_AP3768_)
                        else if(ptr[5] == 0x01)
                        {
                            extern void recv_ap3768_freq_and_gain(u8 *data);
							recv_ap3768_freq_and_gain(&ptr[6]);
							break;
						}
                        else if(ptr[5] == 0x02)
                        {
                            extern void recv_ap3768_delay_and_reverse(u8 *data);
                            recv_ap3768_delay_and_reverse(&ptr[6]);
							if(ptr[6] == 0x04)
							{
								if(setup.aud_fl != (ptr[7]-0x32))
								{
									fl = setup.aud_fl = (ptr[7]-0x32);
									ui_show_sel(FUNC_SEL_FL);
									audio_reg_ctrl(AUD_CTRL_FL,setup.aud_fl);
								}
							}
							else if(ptr[6] == 0x05)
							{
								if(setup.aud_fr != (ptr[7]-0x32))
								{
									fr = setup.aud_fr = (ptr[7]-0x32);
									ui_show_sel(FUNC_SEL_FR);
									audio_reg_ctrl(AUD_CTRL_FR,setup.aud_fr);
								}
							}
							else if(ptr[6] == 0x02)
							{
								if(setup.aud_rl != (ptr[7]-0x32))
								{
									rl = setup.aud_rl = (ptr[7]-0x32);
									ui_show_sel(FUNC_SEL_RL);
									audio_reg_ctrl(AUD_CTRL_RL,setup.aud_rl);
								}
							}
							else if(ptr[6] == 0x03)
							{
								if(setup.aud_rr != (ptr[7]-0x32))
								{
									rr = setup.aud_rr = (ptr[7]-0x32);
									ui_show_sel(FUNC_SEL_RR);
									audio_reg_ctrl(AUD_CTRL_RR,setup.aud_rr);
								}
							}
							break;
						}
						else if(ptr[5] == 0x05)
						{
							global.app_data = TRUE;

							setup.aud_fl = (ptr[8]-0x32);
							ui_show_sel(FUNC_SEL_FL);
							audio_reg_ctrl(AUD_CTRL_FL,setup.aud_fl);

							//OSTimeDly(2);
							setup.aud_fr = (ptr[9]-0x32);
							ui_show_sel(FUNC_SEL_FR);
							audio_reg_ctrl(AUD_CTRL_FR,setup.aud_fr);

							//OSTimeDly(2);
							setup.aud_rl = (ptr[6]-0x32);
							ui_show_sel(FUNC_SEL_RL);
							audio_reg_ctrl(AUD_CTRL_RL,setup.aud_rl);

							//OSTimeDly(2);
							setup.aud_rr = (ptr[7]-0x32);
							ui_show_sel(FUNC_SEL_RR);
							audio_reg_ctrl(AUD_CTRL_RR,setup.aud_rr);

							audio_reg_set();
						}
					#endif
                    	break;
					case 'I':
                        if(ptr[5] <= 7)
                        {
                        	os_taskq_post_msg(MAIN_TASK_NAME, 1, tbl_spp_msg1[(ptr[5]+1)]);
                        }
                        else if(ptr[5] == 0x24)
                        {
                        	os_taskq_post_msg(MAIN_TASK_NAME, 1,SYS_EVENT_EQ_RESET);
                        }
						else
						{
		                    if(compare_task_name(FM_TASK_NAME))
		                    {
                                if(ptr[5] == 0x25)
                                {
				                    os_taskq_post_msg((char *)FM_TASK_NAME,1,tbl_spp_radiokey[1]);
                                }
                                else
                                {
				                    os_taskq_post_msg((char *)FM_TASK_NAME,1,tbl_spp_radiokey[ptr[5]-8]);
                                }
		                    }
		                    else if((compare_task_name(MUSIC_USB_NAME))||(compare_task_name(MUSIC_CARD_NAME)))
		                    {
			                    os_taskq_post_msg((char *)MUSIC_TASK_NAME,1,tbl_spp_musickey[ptr[5]-8]);
		                    }
		                    else if(compare_task_name(BT_TASK_NAME))
		                    {
			                    os_taskq_post_msg((char *)BT_TASK_NAME,1,tbl_spp_btkey[ptr[5]-8]);
		                    }
						}
						break;
				#if defined(_SUPPORT_PANEL_LIGHT_)||defined(_SUPPORT_30P60_LIGHT_)
					case 'L':
						puts("_SUPPORT_PANEL_LIGHT1111_\n");
						if(ptr[5] == 0x00)//跳变
						{
							if(ptr[6] == 0x00)
							{
								setup.State = PANEL_LIGHT_STATE_RED;
							}
							else if(ptr[6] == 0x01)
							{
								setup.State = PANEL_LIGHT_STATE_GREEN;
							}
							else if(ptr[6] == 0x02)
							{
								setup.State = PANEL_LIGHT_STATE_BLUE;
							}
							else if(ptr[6] == 0x03)
							{
								setup.State = PANEL_LIGHT_STATE_YELLOW;
							}
							else if(ptr[6] == 0x04)
							{
								setup.State = PANEL_LIGHT_STATE_FUCHSINE;
							}
							else if(ptr[6] == 0x05)
							{
								setup.State = PANEL_LIGHT_STATE_CYAN;
							}
							else if(ptr[6] == 0x06)
							{
								setup.State = PANEL_LIGHT_STATE_WHITE;
							}
                            ui_show_sel(FUNC_SEL_7COLOR);
						}
						else if(ptr[5] == 0x01)//渐变
						{
						#if defined(_ENABLE_LIGHT_SHADE_)
							if(setup.State != 8)
							{
								puts("_ENABLE_LIGHT_SHADE_1111111111111\n");
								setup.save_light1 = setup.State;
							}
							setup.State = 8;//表示没有
							LightDriverIrCmd(0x0A,ptr[6],ptr[7],ptr[8]);
							setup.red = ptr[6];
							setup.green = ptr[7];
							setup.blue = ptr[8];
							if(setup.Auto_On_Off)
							{
								setup.save_light1 = setup.save_light2;
								printf("setup.save_light111:%d\n",setup.save_light1);
								setup.Auto_On_Off = 0;
								setup.save_clour = 8;
								//new_app_shade_light();
							}
						#endif
						}
						else if(ptr[5] == 0x02)//自动
						{
							if(ptr[6] == 0x00)//关
							{
								setup.State = setup.save_clour;
							}
							else if(ptr[6] == 0x01)//开
							{
								setup.State = PANEL_LIGHT_STATE_AUTO;
							}
                            ui_show_sel(FUNC_SEL_7COLOR);
						}
						break;
				#endif
                }
				break;
			default:
				break;
		}
	}
}
#endif

static void spp_data_deal(u8 packet_type, u16 channel, u8 *packet, u16 size)
{
    //因为修改了user_send_cmd_prepare接口发送串口数据会peng着发完才返回。
    //但是该回调指针是属于协议栈循环调用的，不能用user_send_cmd_prepare接口发数。
    //要发数用可以直接int spp_data_try_send(const u8* buf, u16 len)接口
	puts("---spp_data_deal---\n");
    switch(packet_type)
    {
        case 1:
             //连接成功
             puts("spp connect\n");

			#ifdef	   TD_ADD_SPP
			 spp_connect_falg=1;
			#endif
		#if (BLE_BREDR_MODE&BT_BLE_EN)
			ble_enter_mutex();
		#endif
    		spp_mutex_init();

		#if defined(EASYESY_APP_ONOFF)
			disp_send_param(DISP_APP_ONOFF,ON);
		#endif
            break;
        case 2:
             //连接断开
             puts("spp disconnect\n");
			#ifdef	   TD_ADD_SPP
			 spp_connect_falg=0;
			#endif

		#if (BLE_BREDR_MODE&BT_BLE_EN)
			ble_exit_mutex();
		#endif
			spp_mutex_del();

		#if defined(EASYESY_APP_ONOFF)
			disp_send_param(DISP_APP_ONOFF,OFF);
		#endif
            break;
        case 7://DATA
            ///puts("SP ");

		#if defined(TD_ADD_NEW_APP)

		#elif defined(TD_ADD_SPP)
			spp_rx_msg(packet,size);
		#else
		if(packet[0]=='9')
		{
		    //os_taskq_post_msg(keymsg_task_name, 1,MSG_BT_SPP_UPDATA);
		}
		#if EQ_EN
            if(packet[0]=='E'&&packet[1]=='Q')
             {
                    update_ef_info(packet+4,size-4,packet[2]);
                    break;
             }
		#endif // EQ_EN
	#endif
             break;
    }
}



#if SUPPORT_APP_RCSP_EN
extern void ble_enter_mutex(void);
extern void ble_exit_mutex(void);

static void spp_rcsp_data_deal(u8 packet_type, u16 channel, u8 *packet, u16 size)
{
    //因为修改了user_send_cmd_prepare接口发送串口数据会peng着发完才返回。
    //但是该回调指针是属于协议栈循环调用的，不能用user_send_cmd_prepare接口发数。
    //要发数用可以直接int spp_data_try_send(const u8* buf, u16 len)接口
    switch(packet_type)
    {
        case 1:
             //连接成功
             puts("spp connect\n");
		#ifdef     TD_ADD_SPP
			 spp_connect_falg=1;
		#endif
    		 spp_mutex_init();
		#if (BLE_BREDR_MODE&BT_BLE_EN)
 			 ble_enter_mutex();
		#endif
 			 rcsp_event_com_start(RCSP_APP_TYPE_SPP);
             rcsp_register_comsend(bt_spp_send_data);

		#if defined(EASYESY_APP_ONOFF)
			 disp_send_param(DISP_APP_ONOFF,ON);
		#endif
             break;

		case 2:
             //连接断开
             puts("spp disconnect\n");
			 #ifdef     TD_ADD_SPP
			 spp_connect_falg=0;
			 #endif
             rcsp_event_com_stop();
		#if (BLE_BREDR_MODE&BT_BLE_EN)
             ble_exit_mutex();
		#endif
			 spp_mutex_del();
		#if defined(EASYESY_APP_ONOFF)
			 disp_send_param(DISP_APP_ONOFF,OFF);
		#endif

             break;

		case 7://DATA
            ///puts("SP ");
            rcsp_comdata_recieve(packet,size);
		#if defined(TD_ADD_NEW_APP)

		#elif defined(TD_ADD_SPP)
			spp_rx_msg(packet,size);
		#endif
             break;
    }
}
#endif

/*set_device_volume,get_dac_vol是高级音频同步手机音量相关的，注意跟电话时候的区分
用户不使用*/
static void set_device_volume(int volume )
{
	u8 dac_max_vol = get_max_sys_vol(1);
    dac_ctl.sys_vol_l  = volume * dac_max_vol / 0x7F;
    dac_ctl.sys_vol_r = dac_ctl.sys_vol_l  ;
    if(is_dac_mute()) {
   		dac_mute(0,1);
    }
#if BT_TWS
	if(!is_check_stereo_slave())
	{
		stereo_host_cmd_handle(MSG_VOL_STEREO,dac_ctl.sys_vol_r);
	}
#endif
    set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_ON);
	printf("vol_sync,dac = %d\n",dac_ctl.sys_vol_l);
}

static int get_dac_vol(void)
{
	int dac_max_vol = get_max_sys_vol(1);
	int phone_vol = dac_ctl.sys_vol_l * 0x7F / dac_max_vol;
	printf("vol_sync,phone = %d\n",phone_vol);
    /*The value 0x0 corresponds to 0%. The value 0x7F corresponds to 100%.*/
    return phone_vol;
}

//phone_talking_end,save phone_vol and recover dac_vol
static void phone_sync_sys_dac_vol(u8 vol,u8 phone_vol)
{
	printf("phone_vol:%d, sys_vol:%d\n",dac_ctl.phone_vol, vol);
   	//dac_ctl.sys_vol_l = vol;
   	//dac_ctl.sys_vol_r = vol;
   	//dac_ctl.phone_vol = phone_vol;
    //vm_write_api(VM_PHONE_VOL,&dac_ctl.phone_vol);
}

//phone_talking_start,set phone_vol with vm_phone_vol
#define PHONE_VOL_REMEMBER
static uint8 phone_sync_phone_vol(u8 phone_vol_max)
{
	//u8 phone_vol = 0;
    //vm_read_api(VM_PHONE_VOL,&phone_vol);

  	ew_printf("phone_vol:%d, max_vol:%d\n",dac_ctl.phone_vol,phone_vol_max);
	//if(dac_ctl.phone_vol < 10)
	//dac_ctl.phone_vol = phone_vol_max;

#ifndef	PHONE_VOL_REMEMBER
	//dac_ctl.phone_vol = phone_vol_max;
#endif

#if defined(_ENABLE_BT_DAC_VOL_)
	dac_ctl.phone_vol = dac_ctl.sys_vol_l;
#endif
   	//dac_ctl.sys_vol_l = dac_ctl.phone_vol;
   	//dac_ctl.sys_vol_r = dac_ctl.phone_vol;
	//return dac_ctl.phone_vol;
	return dac_ctl.phone_vol;
}

extern u8 get_key_invalid_flag(void);

/*检测是否可以进入低功耗*/
/*1 : 可以进lowpower   0: 不可以进lowpower*/
static u32 check_enter_lowpower(void)
{
	if(fast_test)
	{
		return 0;
	}
#if 0
	//非静音模式不进lowpower模式
    if(!is_auto_mute())
	{
		return 0;
	}

	if(play_sel_busy()) {
		//puts("play_sel_busy\n");
		return 0;
	}

	//playing key_voice,don't enter lowpower
	if(dac_ctl.sniff_check) {
		return 0;
	}

	//mute dac module
	if(!is_dac_mute()) {
		puts("---off----\n");
		os_taskq_post(MAIN_TASK_NAME,1,MSG_DAC_OFF);
		return 0;
	}

	return 1;
#else
	return 0;
#endif
}

#if BLE_APP_UPDATE_SUPPORT_EN
	extern u8 get_ble_app_update_flag(void);
	if(get_ble_app_update_flag())
	{
		 return 0;
	}
#endif

/*默认没有注册，在读取了配置文件后会调用。用户需要对地址做特殊处理的可以参照例子进行修改*/
static void bt_addr_deal(u8 *addr)
{
    /*地址指针，地址只有6个byte，注意越界访问会有问题, 例子*/
    puts("bt_addr_deal\n");
    addr[0] = 0x1A;
    addr[1] = 0x2A;
    addr[2] = 0x3A;
//    addr[3] = 0x1A;
//    addr[4] = 0x2A;
//    addr[5] = 0x3A;
}

static u8 bt_aec_priority()
{
	return (u8)TaskBtAecPrio;
}

//该函数主要是因为有一些宏的值可能会被用户的修改影响到，所以要通过接口获取。
///*特别注意返回值的范围*///
#define BT_TRIM_ALWAYS 1
#define BT_TRIM_ONCE   0
static u8 bt_get_macro_value(BT_STACK_MACRO type)
{
    //be careful :   MAXRO return value in the function must between 1-254;
    switch(type)
    {
        case BT_MACRO_AEC_SD_PRIO:
            return TaskBtAecSDPrio;
        case BT_MACRO_SBC_PRIO:
            return TaskSbcPrio;
        case BT_MACRO_BTLMP_PRIO:
            return TaskBtLmpPrio;
        case BT_MACRO_BTSTACK_PRIO:
            return TaskBtStackPrio;
		case BT_MACRO_BTESCO_PRIO:
			return TaskBtEscoPrio;
        case BT_MACRO_RESOURCE_PRIO:
            return TaskResoursrPrio;
        case BT_MACRO_MAX_SYS_VOL_L:
            return MAX_SYS_VOL_L;
        case BT_MACRO_MAX_SYS_VOL_R:
            return MAX_SYS_VOL_R;
        case BT_MACRO_SYSCFG_REMOTE_DB:
            return SYSCFG_REMOTE_DB;
        case BT_MACRO_SYSCFG_REMOTE_DB_HID:
            return SYSCFG_REMOTE_DB_HID;
        case BT_MACRO_MSG_LAST_WORKMOD:
            return MSG_LAST_WORKMOD;
        case BT_MACRO_SYS_EVENT_TASK_RUN_REQ:
            return SYS_EVENT_TASK_RUN_REQ;
        case BT_MACRO_MSG_BT_STACK_STATUS_CHANGE:
            return MSG_BT_STACK_STATUS_CHANGE;
        case BT_TRIM_MODE:
		#if(BT_MODE == NORMAL_MODE)
            return BT_TRIM_ONCE;//BT_TRIM_ALWAYS
		#else
			return BT_TRIM_ALWAYS;
		#endif
		case BT_ESCO_FILTER_LEVEL:
			return 0;
		case BT_MACRO_A2DP_AAC_EN:
			return 0;
		case BT_MACRO_STEREO_SYSCFG_REMOTE_DB:
			return SYSCFG_REMOTE_DB_STEREO;
		case BT_MACRO_STEREO_SYSCFG_REMOTE_DB_HID:
			return SYSCFG_REMOTE_DB_STEREO_HID;

        default:
            printf("check the lib,0x%x\n",type);
            return 0xff;
    }
}

static void low_pwr_dac_on_control(void)
{
	//os_taskq_post(MAIN_TASK_NAME,1,MSG_DAC_ON);
}

static void low_pwr_dac_off_control(void)
{
	//os_taskq_post(MAIN_TASK_NAME,1,MSG_DAC_OFF);
}

u32 pwr_timer_in(void)
{
	/* putchar('I'); */
	return 1;
}

//u8 led_flag = 0;
u32 pwr_timer_out(void)
{
//	led_flag = !led_flag;
	return 1;
}

void bt_fast_test_handle()
{
	puts("bt_fast_test_handle \n");
	os_taskq_post(BTMSG_TASK_NAME,1,MSG_BT_FAST_TEST);
}

#if defined(_ENABLE_BT_NAME_)
unsigned char char_to_hex(unsigned char hex)
{
	if((hex>=0)&&(hex<=9))
	{
		hex += 0x30;
	}
	else if((hex>=10)&&(hex<=15))//Capital
	{
		hex += 0x37;
	}
	else
	{
		hex = 0xff;
	}
	ew_printf("hex:%c\n",hex);
	return hex;
}

u8 addr1 = 0;
u8 addr2 = 0;
u8 addr1h = 0;
u8 addr1l = 0;
u8 addr2h = 0;
u8 addr2l = 0;
char *mac_name = NULL;
void bt_addr_handle(u8 * bredr_addr,u8 * ble_addr)
{
	char name_buf[13];

	puts("bredr addr : ");
	put_buf(&bredr_addr[4],1);
	put_buf(&bredr_addr[5],1);

	addr1 = bredr_addr[4];
	addr2 = bredr_addr[5];

	ew_printf("addr1:%x\n",addr1);
	ew_printf("addr2:%x\n",addr2);

	addr1h = (addr1&0xF0)>>4;
	addr1l = addr1&0x0F;

	addr2h = (addr2&0xF0)>>4;
	addr2l = addr2&0x0F;

	//ew_printf("addr1h:%x\n",addr1h);
	//ew_printf("addr1l:%x\n",addr1l);
	//ew_printf("addr2h:%x\n",addr2h);
	//ew_printf("addr2l:%x\n",addr2l);

	//由于新SDK4，底层是先发MAC地址，再发蓝牙名称，导致这个地方没进去,将这部分移到获取蓝牙名称函数里面
	if(mac_name != NULL)
	{
		puts("--bt_addr_handle--\n");
    	name_buf[0] = 'P';
    	name_buf[1] = 'r';
    	name_buf[2] = 'o';
    	name_buf[3] = 'l';
    	name_buf[4] = 'o';
    	name_buf[5] = 'g';
    	name_buf[6] = 'y';
    	name_buf[7] = ' ';

    	name_buf[8] = char_to_hex(addr1h);
    	name_buf[9] = char_to_hex(addr1l);
    	name_buf[10] = char_to_hex(addr2h);
    	name_buf[11] = char_to_hex(addr2l);
    	name_buf[12] = 0;
	}
	my_memcpy(mac_name,name_buf,48);

	//put_buf(bredr_addr,6);
	//puts("ble addr : ");
	//put_buf(ble_addr,6);
}
#endif

#if defined(_ENABLE_BT_BLE_NAME_)
char *app_name = NULL;
u8 BT_NAME_LEN;

extern u8 ble_name_chg_falg;
extern u8 profile_scan_rsp_data[31];
extern void server_set_scan_rsp_data(u8 *data);
extern u8 BT_NAME_LEN;
void set_scan_rsp_content(char *name, u8 name_len)
{
	if((name_len+6)>28)
	{
		printf("ble_name len err\n");
         return;
	}
   	printf("ble name11:%s %d\n",name,name_len);
	memset(profile_scan_rsp_data,0x00,31);
	profile_scan_rsp_data[0]=name_len+1+4;	//
	profile_scan_rsp_data[1]=0x09;
	memcpy(profile_scan_rsp_data+2,name,name_len);
	printf("ble_name set_scan_rsp_content\n");
	server_set_scan_rsp_data(profile_scan_rsp_data);
}

void ble_name_handle(char *name)
{
	u8 len=0;
	len = strlen(name);
	extern void server_register_profile_data(const u8 *data);
	extern const u8 profile_data[];
	set_scan_rsp_content(name,len);
	server_register_profile_data(profile_data);
}

unsigned char char_to_hex(unsigned char hex)
{
	if((hex>=0)&&(hex<=9))
	{
		hex += 0x30;
	}
	else if((hex>=10)&&(hex<=15))//Capital
	{
		hex += 0x37;
	}
	else
	{
		hex = 0xff;
	}
	ew_printf("hex:%c\n",hex);
	return hex;
}

u8 addr1 = 0;
u8 addr2 = 0;
u8 addr1h = 0;
u8 addr1l = 0;
u8 addr2h = 0;
u8 addr2l = 0;
char *mac_name = NULL;
void bt_addr_handle(u8 * bredr_addr,u8 * ble_addr)
{
	char name_buf[13];

	puts("bredr addr : ");
	put_buf(&bredr_addr[4],1);
	put_buf(&bredr_addr[5],1);

	addr1 = bredr_addr[4];
	addr2 = bredr_addr[5];

	ew_printf("addr1:%x\n",addr1);
	ew_printf("addr2:%x\n",addr2);

	addr1h = (addr1&0xF0)>>4;
	addr1l = addr1&0x0F;

	addr2h = (addr2&0xF0)>>4;
	addr2l = addr2&0x0F;

	//ew_printf("addr1h:%x\n",addr1h);
	//ew_printf("addr1l:%x\n",addr1l);
	//ew_printf("addr2h:%x\n",addr2h);
	//ew_printf("addr2l:%x\n",addr2l);

	if(mac_name != NULL)
	{
    	name_buf[0] = char_to_hex(addr1h);
    	name_buf[1] = char_to_hex(addr1l);
    	name_buf[2] = char_to_hex(addr2h);
    	name_buf[3] = char_to_hex(addr2l);
    	name_buf[4] = '\0';
	}
	puts("bredr addr111\n");
	my_memcpy(mac_name,name_buf,strlen(name_buf));
	mac_name[strlen(name_buf)] = '\0';

#if defined(_ENABLE_BT_BLE_NAME_)
	extern u8 BT_NAME_LEN;
	extern void ble_name_handle(char *name);
	//name_buf[4] = '_';
	//name_buf[5] = 'A';
	//name_buf[6] = 'P';
	//name_buf[7] = 'P';
	//name_buf[8] = '\0';
	puts("bredr addr333\n");
	my_memcpy(app_name,name_buf,strlen(name_buf));
	if(strlen(name_buf) < 30){
		app_name[strlen(name_buf)] = '\0';
	}
	ble_name_handle(app_name);
#endif

	//put_buf(bredr_addr,6);
	//puts("ble addr : ");
	//put_buf(ble_addr,6);
}
#endif

void get_phone_time(char *data , u16 len)
{
	put_buf((u8 *)data , len);
	puts(data);
}
extern void hook_get_phone_time_register(void (*handle)(char* data, u16 length));
extern u8 get_esco_st(u8 sw);
/*
蓝牙库用到的一系列可设置参数配置，通过handle的形式。
这样用户就不用考虑调用配置函数的具体位置。需要改变可以注册，不需要设置有默认参数
*/
static void ble_name_custom_handle(char *name)
{
	printf("ble name:%s\n",name);

#if defined(_ENABLE_BT_BLE_NAME_)
	if(app_name == NULL)
	{
		app_name = name;
	}
#endif
}

#if defined(_XM003_BT_NAME_)||defined(_YH1701_BT_NAME_)||defined(_JTI_BT_NAME_)
static u32 random_number1(u32 start, u32 end)
{
	if(end<=start)
	{
		return start;
	}

	//获得一个范围在start到end之间的随机数
	return JL_TIMER0->CNT % (end-start+1) + start;
}
#endif

static void bredr_name_custom_handle(char *name)
{
#if defined(_ENABLE_BT_NAME_)||defined(_ENABLE_BT_BLE_NAME_)
	if(mac_name == NULL)
	{
		mac_name = name;
	}
#endif

#if defined(_ENABLE_BT_NAME_)
#if defined(TD_ADD_NEW_APP)||1
	char name_buf[48];
	my_memset(name_buf,0,48);
	name_buf[0] = 'P';
	name_buf[1] = 'r';
	name_buf[2] = 'o';
	name_buf[3] = 'l';
	name_buf[4] = 'o';
	name_buf[5] = 'g';
	name_buf[6] = 'y';
	name_buf[7] = ' ';

	name_buf[8] = char_to_hex(addr1h);
	name_buf[9] = char_to_hex(addr1l);
	name_buf[10] = char_to_hex(addr2h);
	name_buf[11] = char_to_hex(addr2l);
	name_buf[12] = 0;
	my_memcpy(name,name_buf,48);
#endif
#endif

#if defined(_XM003_BT_NAME_)
	char name_buf[48];
    if(!setup.acc_off)
    {
    	setup.rand_number = random_number1(1,50000);
    }
    setup.save_number = setup.rand_number;

	my_memset(name_buf,0,48);

	name_buf[0] = 'M';
	name_buf[1] = 'P';
	name_buf[2] = '3';
	name_buf[3] = 'B';
	name_buf[4] = 'T';
	name_buf[5]= '-';

	if(setup.save_number < 10)
	{
		name_buf[6] = setup.save_number + '0';
	}
	else if((setup.save_number > 9)&&(setup.save_number < 100))
	{
		name_buf[6] = setup.save_number/10 + '0';
		name_buf[7] = (setup.save_number%10)%10 + '0';
	}
	else if((setup.save_number > 99)&&(setup.save_number < 1000))
	{
		name_buf[6] = setup.save_number/100 + '0';
		name_buf[7] = (setup.save_number%100)/10 + '0';
		name_buf[8] = (setup.save_number%100)%10 + '0';
	}
	else if((setup.save_number > 999)&&(setup.save_number < 10000))
	{
		name_buf[6] = (setup.save_number/100)/10 + '0';
		name_buf[7] = (setup.save_number/100)%10 + '0';
		name_buf[8] = (setup.save_number%100)/10 + '0';
		name_buf[9] = (setup.save_number%100)%10 + '0';
	}
	else if((setup.save_number > 9999)&&(setup.save_number < 50000))
	{
		name_buf[6] = setup.save_number/10000 + '0';
		name_buf[7] = (setup.save_number%10000)/1000 + '0';
		name_buf[8] = ((setup.save_number%10000)%1000)/100 + '0';
		name_buf[9] = (((setup.save_number%10000)%1000)%100)/10 + '0';
		name_buf[10] = (((setup.save_number%10000)%1000)%100)%10 + '0';
	}
	my_memcpy(name,name_buf,48);
	
#elif defined(_JTI_BT_NAME_)
    char name_buf[48];
    if(!setup.acc_off)
    {
    	setup.rand_number = random_number1(1,10000);
    }
    setup.save_number = setup.rand_number;
    
    my_memset(name_buf,0,48);
    
		name_buf[0] = 'S';
		name_buf[1] = 'P';
		name_buf[2] = '2';
	#if defined(_JTI2240_BT_NAME_)
    	name_buf[3] = '2';
    	name_buf[4] = '4';
	#else
		name_buf[3] = '2';
		name_buf[4] = '3';
	#endif
		name_buf[5] = '0';
		name_buf[6] = 'B';
		name_buf[7] = 'T';
		name_buf[8] = '-';
    
	if(setup.save_number < 10)
	{
		name_buf[9] = setup.save_number + '0';
	}
	else if((setup.save_number > 9)&&(setup.save_number < 100))
	{
		name_buf[9] = setup.save_number/10 + '0';
		name_buf[10] = (setup.save_number%10)%10 + '0';
	}
	else if((setup.save_number > 99)&&(setup.save_number < 1000))
	{
		name_buf[9] = setup.save_number/100 + '0';
		name_buf[10] = (setup.save_number%100)/10 + '0';
		name_buf[11] = (setup.save_number%100)%10 + '0';
	}
	else if((setup.save_number > 999)&&(setup.save_number < 10000))
	{
		name_buf[9] = (setup.save_number/100)/10 + '0';
		name_buf[10] = (setup.save_number/100)%10 + '0';
		name_buf[11] = (setup.save_number%100)/10 + '0';
		name_buf[12] = (setup.save_number%100)%10 + '0';
	}
    my_memcpy(name,name_buf,48);

#elif defined(_YH1701_BT_NAME_)
    char name_buf[48];
    if(!setup.acc_off)
    {
    	setup.rand_number = random_number1(1,50000);
    #if defined(_SUPPORT_EXT_MEMORY_)
    	setup.acc_off = TRUE;
    #endif
    }
    setup.save_number = setup.rand_number;
    
    my_memset(name_buf,0,48);
    
	name_buf[0] = 'F';
	name_buf[1] = 'S';
	name_buf[2] = 'B';
	name_buf[3] = 'N';
	name_buf[4] = '2';
	name_buf[5] = '5';
	name_buf[6] = 'M';
	name_buf[7] = '-';
	name_buf[8] = 'B';
	name_buf[9] = 'L';
	name_buf[10]= '-';
	
	if(setup.save_number < 10)
	{
		name_buf[11] = setup.save_number + '0';
	}
	else if((setup.save_number > 9)&&(setup.save_number < 100))
	{
		name_buf[11] = setup.save_number/10 + '0';
		name_buf[12] = (setup.save_number%10)%10 + '0';
	}
	else if((setup.save_number > 99)&&(setup.save_number < 1000))
	{
		name_buf[11] = setup.save_number/100 + '0';
		name_buf[12] = (setup.save_number%100)/10 + '0';
		name_buf[13] = (setup.save_number%100)%10 + '0';
	}
	else if((setup.save_number > 999)&&(setup.save_number < 10000))
	{
		name_buf[11] = (setup.save_number/100)/10 + '0';
		name_buf[12] = (setup.save_number/100)%10 + '0';
		name_buf[13] = (setup.save_number%100)/10 + '0';
		name_buf[14] = (setup.save_number%100)%10 + '0';
	}
	else if((setup.save_number > 9999)&&(setup.save_number < 50000))
	{
		name_buf[11] = setup.save_number/10000 + '0';
		name_buf[12] = (setup.save_number%10000)/1000 + '0';
		name_buf[13] = ((setup.save_number%10000)%1000)/100 + '0';
		name_buf[14] = (((setup.save_number%10000)%1000)%100)/10 + '0';
		name_buf[15] = (((setup.save_number%10000)%1000)%100)%10 + '0';
	}
    my_memcpy(name,name_buf,48);
#endif

	printf("bredr name:%s\n",name);
}
static void bt_osc_cap_save_hanlde(void)
{
	os_taskq_post(BTMSG_TASK_NAME,1,MSG_OSC_INTER_CAP_WRITE);
}

extern void feedback_manufacturer_handle_register(void (*handle)(char *msg,u8 len));
void manufacturer_handle(char *msg , u8 len )
{
	puts(msg);
	putchar('\n');
}

extern void read_vm_ble_name_handle_register(void (*handle)(char *name));
extern void read_vm_bt_name_handle_register(void (*handle)(char *name));

extern void set_sys_pd_ldo_level(u8 level);
extern void set_sys_pd_dvdd_level(u8 level);

#if defined(_ENABLE_A2DP_TIME_)
void Spp_Tx_ID3_TIME(u32 totaltime,u32 Curplaytime)
{
#if defined(TD_ADD_NEW_APP)
	sppbuf[0] =0xff;
	sppbuf[1] =0xaa;
    sppbuf[2] = 12;
	sppbuf[3] ='I';
	sppbuf[4] ='D';
    sppbuf[5] = 0x04;
    sppbuf[6] =((totaltime>>24)&0xff);//总时间
    sppbuf[7] =((totaltime>>16)&0xff);
    sppbuf[8] =((totaltime>>8)&0xff);
    sppbuf[9] =(totaltime&0xff);
    sppbuf[10] =((Curplaytime>>24)&0xff);//当前时间
    sppbuf[11] =((Curplaytime>>16)&0xff);
    sppbuf[12] =((Curplaytime>>8)&0xff);
    sppbuf[13] =(Curplaytime&0xff);
    sppbuf[14] = crc_checksum((u8 *)sppbuf,14);
	Notify_data_send(sppbuf,15);
#else
	sppbuf[0] ='B';
	sppbuf[1] =0x07;//蓝牙相关信息
	sppbuf[2] =8;//蓝牙相关信息
    sppbuf[3] =((totaltime>>24)&0xff);//总时间
	sppbuf[4] =((totaltime>>16)&0xff);
	sppbuf[5] =((totaltime>>8)&0xff);
	sppbuf[6] =(totaltime&0xff);
	sppbuf[7] =((Curplaytime>>24)&0xff);//当前时间
	sppbuf[8] =((Curplaytime>>16)&0xff);
	sppbuf[9] =((Curplaytime>>8)&0xff);
	sppbuf[10] =(Curplaytime&0xff);
	bt_spp_send_data((u8 *)sppbuf,11);
	Notify_data_send(sppbuf,11);
#endif
}

#if defined(_SUPPORT_GET_BT_ID3_INFO_)
// id3 发送数据格式
//类型type: 1-title 2-artist name 3-album names 4-track number 5-total number of tracks 6-genre  7-playing time
// pdata 待发送类型数据
// len 长度
// type ID3数据类型
// 数据格式要求utf-8
void Spp_Tx_ID3_INFO(u8 type,u8 *pdata,u8 len)
{
#if defined(TD_ADD_NEW_APP)
	printf("id3_len:%d\n",len);
	u8 inidx=0;
	u8 index=0;
    sppbuf[inidx++] =0xff;
    sppbuf[inidx++] =0xaa;
	if (len > 100)
	{
		sppbuf[inidx++] = 0x69;//105
	}
	else
	{
		sppbuf[inidx++] = (len+5);
	}
    sppbuf[inidx++] ='I';
    sppbuf[inidx++] ='D';
    sppbuf[inidx++] = 0x05;
    sppbuf[inidx++] = type;
	if(len > 100)//20
	{
		//puts("ERR:over sppbuf limit 20\n");
		len = 100;
	}

	printf("app_id3<");
    for(u8 i=0;i<len;i++)
    {
    	sppbuf[inidx++]=pdata[i];
		printf(" %x ",pdata[i]);
    }

	printf(">len:%d\n",len);

	index = inidx++;

	//printf("id3_index:%d\n",index);

    sppbuf[index] = crc_checksum((u8 *)sppbuf,index);
    Notify_data_send(sppbuf,index+1);

#else
	u8 inidx=0;
	sppbuf[inidx++] ='B';
	sppbuf[inidx++] =0x05;//蓝牙相关信息
	if (len > 100)
	{
		sppbuf[inidx++] = 0x65;//101
	}
	else
	{
		sppbuf[inidx++] =len+1;
	}
	sppbuf[inidx++] =type;
	if (len > 100)//20
	{
		puts("ERR:over sppbuf limit 20\n");
		len = 100;
	}
    for(u8 i=0;i<len;i++)
    {
    	sppbuf[inidx++]=pdata[i];
    }
	bt_spp_send_data((u8 *)sppbuf,(len+4));
	Notify_data_send(sppbuf,(len+4));
#endif
}

#if !defined(TD_ADD_NEW_APP)
void Spp_Tx_BN(void)
{
	sppbuf[0] ='B';
	sppbuf[1] =0x06;//蓝牙相关信息

	bt_spp_send_data((u8 *)sppbuf,2);

	Notify_data_send(sppbuf,2);
}
#endif
#endif


#if defined(TD_ADD_NEW_APP)
void Spp_Tx_Bt_Staust(void)
{
	printf("global.a2dp_flag:%d\n",global.a2dp_flag);
    sppbuf[0] =0xff;
    sppbuf[1] =0xaa;
    sppbuf[2] = 5;
    sppbuf[3] ='I';
    sppbuf[4] ='D';
    sppbuf[5] = 0x06;
    sppbuf[6] = global.a2dp_flag;
    sppbuf[7] = crc_checksum((u8 *)sppbuf,7);
    Notify_data_send(sppbuf,8);
}
#endif

u32  time_buff1=0,time_buff2=0,now_time_buff=0;
u8 typebuff = 0;
u8 numbercheck(u8 *info,u16 len)
{
    for(u16 i=0;i<len;i++)
	{
		if((info[i]>'9')||(info[i]<'0'))
		{
			return FALSE;
		}
	}
	return TRUE;
}

u8 *bt_id3[0x11+1] = {NULL};
u16 bt_id3_len[0x11+1] = {0};
u32 type_time[0x11+1] = {0};

u32 bt_total_time = 0;
u8 total_falg = 0;

extern void bt_music_info_handle_register(void (*handler)(u8 type, u32 time,u8 *info, u16 len));
extern void send_bt_all_id3(void);

void send_user_get_bt_music_info(u8 type , u32 time ,u8 *info ,  u16 len )
{
	//profile define type: 1-title 2-artist name 3-album names 4-track number 5-total number of tracks 6-genre  7-playing time
	//JL define 0x10-total time , 0x11 current play position
	printf("music_info_type %d\n", type );
	printf("music_info_time:%d\n",time);
#if defined(_SUPPORT_GET_BT_ID3_INFO_)
#if !defined(TD_ADD_NEW_APP)
	if((typebuff==0)&&(type>0))
	{
		Spp_Tx_BN();
	}
	typebuff = type;
#endif
#endif

	if((info!=NULL) && (len!=0))
	{
    #if defined(_SUPPORT_GET_BT_ID3_INFO_)
		if(type==5)
		{
			if(numbercheck(info,len))
			{
				Spp_Tx_ID3_INFO(type,info,len);
			}
		}
		else
		{
			Spp_Tx_ID3_INFO(type,info,len);
		}
    #endif
		if(info[len-1]!=0x00)
		{
			info[len-1]=0x00;
		}
		puts((const char *)info);
		//putchar('\n');
	}

	u32 totle_time ,now_time;
	if(time != 0)
	{
		time_buff2=time_buff1;
		time_buff1=time / 1000;
	}
	if (time_buff1>time_buff2)
	{
		totle_time=time_buff1;
		now_time=time_buff2;
	}
	else
	{
		totle_time=time_buff2;
		now_time=time_buff1;
	}

	if(now_time != now_time_buff)
	{
		now_time_buff = now_time;
		printf("total_time:%d\n",bt_total_time/1000);
		printf("now_time:%d\n",now_time);
		if(bt_total_time/1000 < 6000)//100分钟
		{
			Spp_Tx_ID3_TIME(bt_total_time/1000,now_time);
		}
	}
}

void user_get_bt_music_info(u8 type , u32 time ,u8 *info ,  u16 len )
{
	//profile define type: 1-title 2-artist name 3-album names 4-track number
	//5-total number of tracks 6-genre  7-playing time
	//JL define 0x10-total time , 0x11 current play position
	printf("type:%d\n", type );
	printf("time:%d\n", time );

	if(type > 0)
	{
		total_falg = TRUE;
	}

	if(bt_total_time < time)
	{
		bt_total_time = time;
	}

	if(total_falg)
	{
		if(time != 0)
		{
			bt_total_time = time;
			total_falg = FALSE;
		}
	}

	if(info == NULL)
	{
		puts("info_null\n");
	}

	if(type > 0x11)
	{
		return;
	}
	type_time[type] = time;
	if((info != NULL) && (len!=0))
	{
		if(bt_id3[type] != NULL)
		{
			//上一次的type内容没有发送
			if(!compare_task_name("BtUserTask"))
			{
				free(bt_id3[type]);
				bt_id3[type] = NULL;
				bt_id3_len[type] = 0;
				bt_id3[type] = malloc(len);///最好加个信号量
				if(bt_id3[type] == NULL)
				{
					bt_id3_len[type] = 0;
					///没有空间
					return;
				}
				memcpy(bt_id3[type],info,len);
				bt_id3_len[type] = len;
			}
		}
		else
		{
			bt_id3[type] = malloc(len);///最好加个信号量
			if(bt_id3[type] == NULL)
			{
				bt_id3_len[type] = 0;
			///没有空间
				return;
			}
			memcpy(bt_id3[type],info,len);
			bt_id3_len[type] = len;
		}
	}

	if(global.a2dp_count == 4)
	{
		if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)
			&&(get_bt_newest_status() != BT_STATUS_PHONE_OUT)
			&&(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE)
			&&(get_bt_connect_status() != BT_STATUS_TAKEING_PHONE))
		{
			send_bt_all_id3();
		}
	}
	
	if(compare_task_name("BtUserTask"))
	{
		if((get_bt_newest_status() != BT_STATUS_PHONE_INCOME)
			&&(get_bt_newest_status() != BT_STATUS_PHONE_OUT)
			&&(get_bt_newest_status() != BT_STATUS_PHONE_ACTIVE)
			&&(get_bt_connect_status() != BT_STATUS_TAKEING_PHONE))
		{
			os_taskq_post(BT_TASK_NAME,2,MSG_BT_ID3,type);

		}
	}

	//os_taskq_post(BT_TASK_NAME,2,MSG_BT_ID3,type);
}

void send_bt_all_id3(void)
{
	u8 type=0;
	for(type = 0;type<0x11+1;type++)
	{///一次性清空数据，防止丢失消息
		if(bt_id3[type] != NULL)
		{
			send_user_get_bt_music_info(type,type_time[type],bt_id3[type],bt_id3_len[type] );
			free(bt_id3[type]);
			bt_id3[type] = NULL;
			bt_id3_len[type] = 0;
		}
	}
}

void send_bt_id3(u8 type)
{
	//printf("send_bt_id3 type:%d\n",type);
	send_user_get_bt_music_info(type,type_time[type],bt_id3[type],bt_id3_len[type]);
	if(bt_id3[type] != NULL)
	{
		free(bt_id3[type]);
		bt_id3[type] = NULL;
		bt_id3_len[type] = 0;
	}
	u8 i = 0;
	for(i = 0;i<0x11+1;i++)
	{///一次性清空数据，防止丢失消息
		if(bt_id3[type] != NULL)
		{
			send_user_get_bt_music_info(type,type_time[type],bt_id3[type],bt_id3_len[type] );
			free(bt_id3[type]);
			bt_id3[type] = NULL;
			bt_id3_len[type] = 0;
		}
	}
}
#endif

/*
蓝牙库用到的一系列可设置参数配置，通过handle的形式。
这样用户就不用考虑调用配置函数的具体位置。需要改变可以注册，不需要设置有默认参数
*/
static void bredr_handle_register(void)
{
	//设置进入powerdown的时候vddio和rtcvdd降到多少伏
	//0 : 3.53v  1 : 3.34v  2 : 3.18v  3 : 3.04v
	// 4 : 2.87v  5 : 2.73v  6 : 2.62v  7 : 2.52v
	set_sys_pd_ldo_level(1);
	set_sys_pd_dvdd_level(1);
	ew_puts("---------bredr_handle_register----------");

    register_edr_init_handle();
	aec_get_macro_handle_register( bt_aec_priority);    /*must set*/
    set_bt_info_callback(bt_function_select_init);/*相当于默认设置，要在协议栈初始化前设置*/
//    get_battery_value_register(bt_get_battery_value);   /*电量显示获取电量的接口*/
    call_vol_change_handle_register(call_vol_change); /*通话过程中调节音量会有反馈的回调函数*/
    read_remote_name_handle_register(bt_read_remote_name);  /*连接成功后读取手机名字从该函数返回*/
    discon_complete_handle_register(bt_discon_complete_handle);/*断开或者连接上会调用的函数，给客户反馈信息*/

#if SUPPORT_APP_RCSP_EN
	spp_data_deal_handle_register(spp_rcsp_data_deal);    /*简单串口*/
#else
	spp_data_deal_handle_register(spp_data_deal);    /*简单串口*/
#endif
	hook_get_phone_time_register(get_phone_time);
	hook_phone_number_register(hook_hfp_incoming_phone_number);   /*获取解释到电话号码返回*/
    phone_sync_sys_vol_handle_register(phone_sync_sys_dac_vol,phone_sync_phone_vol );  /*通话时同步系统音量*/
    //music_vol_change_handle_register(set_device_volume, get_dac_vol );  /*同步手机音乐音量*/
    //bt_addr_spec_deal_handle_register(bt_addr_deal);   /*可以个性化设置地址*/
	bt_auto_conn_handle_register(bt_is_auto_conn);
	bt_osc_write_register(bt_osc_cap_save_hanlde);
    bt_low_pwr_handle_register(low_pwr_deal,low_pwr_dac_on_control,low_pwr_dac_off_control);
    bt_updata_run_handle_register(bt_updata_run);/*蓝牙无线升级*/
    bt_set_low_power(LOWPOWER_OSC_TYPE,LOWPOWER_OSC_HZ,BT_LOW_POWER_MODE,1,SYS_Hz/1000000L,check_enter_lowpower,NULL); /*蓝牙低功耗设置*/
	bt_chip_io_type_setting(RTCVDD_TYPE , BTAVDD_TYPE);
	bt_set_noconnect_lowpower_fun(1,pwr_timer_in,pwr_timer_out);/*设置没有连接的时候是否进入低功耗模式*/
    bt_set_noconnect_lowpower_led_fun(1);
	esco_handle_register(get_esco_st);
#if BT_TWS
	bt_stereo_register(bt_save_stereo_info,stereo_sys_vol_sync,stereo_deal_cmd,NULL);  /*对箱接口*/
#else
	bt_stereo_register(NULL,NULL,NULL,NULL);  /*对箱接口*/
#endif
	bt_fast_test_handle_register(bt_fast_test_handle);

#if defined(_ENABLE_A2DP_TIME_)
	bt_music_info_handle_register(user_get_bt_music_info);
#endif

#if defined(_ENABLE_BT_NAME_)||defined(_ENABLE_BT_BLE_NAME_)
	bt_addr_handle_register(bt_addr_handle);//获取蓝牙MAC地址
#endif
	read_vm_bt_name_handle_register(bredr_name_custom_handle);
	feedback_manufacturer_handle_register(manufacturer_handle);
}

extern void set_osc_int(u8 r,u8 l);
static void ble_osc_cap_change_handler(u8 osc_l,u8 osc_r,u8 adjust_end_flag)
{
	printf("osc_l:%x osc_r:%x adjust_flag:%x\n",osc_l,osc_r,adjust_end_flag);
	set_osc_int(osc_r,osc_l);
	if(adjust_end_flag)
	{
		os_taskq_post(BTMSG_TASK_NAME,1,MSG_OSC_INTER_CAP_WRITE);
	}
}


extern void ble_osc_cap_change_handle_register(void (*handler)(u8,u8,u8));
extern void bt_addr_spec_deal_handle_register(void (*handle)(u8 *addr));

static void ble_handle_register(void)
{
	read_vm_ble_name_handle_register(ble_name_custom_handle);
    register_ble_init_handle();
#if (BLE_BREDR_MODE == BT_BLE_EN)
	bt_low_pwr_handle_register(low_pwr_deal,low_pwr_dac_on_control,low_pwr_dac_off_control);
	bt_set_low_power(LOWPOWER_OSC_TYPE,LOWPOWER_OSC_HZ,BT_LOW_POWER_MODE,1,SYS_Hz/1000000L,check_enter_lowpower,NULL); /*蓝牙低功耗设置*/
#endif
	//if enable testbox auto adjust frequency offset,this callback must be registered
	ble_osc_cap_change_handle_register(ble_osc_cap_change_handler);
}
static void stereo_handle_register()
{
	register_stereo_init_handle();
}
void reset_osc_int_cfg()
{
	u8 osc_r = 0xff;
	u8 osc_l = 0xff;

    if (VM_READ_NO_INDEX == vm_read_api(VM_BT_OSC_INT_R,&osc_r))
	{
		puts("nvm err1\n");
		return;
	}

    if (VM_READ_NO_INDEX == vm_read_api(VM_BT_OSC_INT_L,&osc_l))
	{
		puts("nvm err2\n");
		return;
	}

	printf("nosc:%d,%d\n",osc_r,osc_l);

	if((osc_r <= 0x1f)&&(osc_l <= 0x1f))
	{
 		set_osc_int(osc_r,osc_l);
	}
}
/*不属于用户接口，协议栈回调函数*/
extern void bt_osc_internal_cfg(u8 sel_l,u8 sel_r);
extern void app_server_init(void);

extern u8 ble_rf_uart_rx_isr(u8 rx_data);
extern void ble_gatt_client_init(void);
#if BT_TWS
static u8 rx_mem[10*1024] sec(.bt_classic_data);
static u8 tx_mem[5*1024] sec(.bt_classic_data);
#else
static u8 rx_mem[5*1024] sec(.bt_classic_data);
static u8 tx_mem[3*1024] sec(.bt_classic_data);
#endif
void bt_app_cfg()
{
	ew_puts("----------bt_app_cfg11111111------------\n");

#if (BT_MODE != NORMAL_MODE)
	u8 (*fcc_uart_isr)(u8);
	u8 (*ble_rf_uart_isr)(u8);
#endif
    u8 poweroff = 0;
    poweroff = bt_power_is_poweroff_post();
    cfg_test_box_and_work_mode(NON_TEST,BT_MODE);
	cfg_ble_work_mode(BT_MODE);
#if (BT_MODE != NORMAL_MODE)
	fcc_uart_isr = NULL;
	ble_rf_uart_isr = NULL;
	rf_test_uart_para_init(UART1_HARDWARE_NAME,9600,UART_USBP_USBM);
#if (BLE_BREDR_MODE&BT_BREDR_EN)
	fcc_uart_isr = fcc_uart_isr_callback;
#endif
#if (BLE_BREDR_MODE&BT_BLE_EN)
	ble_rf_uart_isr = ble_rf_uart_rx_isr;
#endif
	register_rf_test_uart_callback(fcc_uart_isr,ble_rf_uart_isr);
#endif
	cfg_bt_pll_para(OSC_Hz,SYS_Hz,BT_ANALOG_CFG,BT_XOSC);
 	cfg_bredr_mem(rx_mem,sizeof(rx_mem),tx_mem,sizeof(tx_mem));
	/*internal capacitor range:0x00~0x1F*/
//	bt_osc_internal_cfg(0x11,0x11);

    bt_get_macro_handle_register(bt_get_macro_value); /*must set*/

    __set_ble_bredr_mode(BLE_BREDR_MODE);    /*bt enble BT_BLE_EN|BT_BREDR_EN */
#if BT_TWS
    set_tws_channel_type(BT_TWS_CHANNEL_SELECT);
	__set_stereo_mode(1);
	stereo_handle_register();
#else
	__set_stereo_mode(0);
#endif // BT_STEREO
	set_bt_config_check_callback(bt_info_init);   //    bt_info_init();[>读取flash的配置文件<]
#if (BT_MODE != NORMAL_MODE)
	fcc_test_handle_register(rf_test_uart_init_callback,test_fcc_default_info);/*fcc测试函数注册*/
	ble_rf_test_info_register(BLE_BREDR_MODE,test_ble_rf_default_info);
#endif
#if (BLE_BREDR_MODE&BT_BREDR_EN)
    bredr_handle_register();
#endif   //SUPPORT BREDR
#if (BLE_BREDR_MODE&BT_BLE_EN)
    ble_handle_register();
#if BLE_GATT_ROLE_CFG
	extern void app_client_init(void);
	app_client_init();
	/* ble_gatt_client_init(); */
#else
	app_server_init();
#endif
#endif   //SUPPORT BLE
	reset_osc_int_cfg();
ew_puts("----------bt_app_cfg2222------------\n");
}


