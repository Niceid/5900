/*
 *********************************************************************************************************
 *                                            br16
 *                                            btstack
 *                                             CODE
 *
 *                          (c) Copyright 2016-2016, ZHUHAI JIELI
 *                                           All Rights Reserved
 *
 * File : *
 * By   : jamin.li
 * DATE : 2016-04-12 10:17AM    build this file
 *********************************************************************************************************
 */
#include "typedef.h"
//#include "error.h"
#include "sdk_cfg.h"
#include "common/msg.h"
#include "common/app_cfg.h"
#include "bluetooth/le_profile_def.h"
#include "bluetooth/le_server_module.h"
#include "bluetooth/le_finger_spinner.h"
#include "rtos/os_api.h"

#include "rtos/task_manage.h"
#include "rcsp/rcsp_interface.h"
#include "rcsp/rcsp_head.h"
#include "disp.h"


#if(BLE_BREDR_MODE&BT_BLE_EN)
#define SOFTVERSION "2017.11.16_ew5900ble"

#if 0
const u8 VisionTab[]=
{
	"FA1:"__PRODUCT_INFO__"\r\n"
	"FA2:"__DATE__"\r\n"
	"FA3:"__TIME__"\r\n"
};
#endif
const u8 VisionTab[]=
{
	__PRODUCT_INFO__"--"
	__DATE__"--"
	__TIME__
};


extern void Spp_Tx_Ack(void);
extern void Spp_Tx_WorkMode(void);
extern void Spp_Tx_Audio(void);
#ifdef	TD_ADD_NEW_APP
extern u8 spp_connect_falg;
#endif
//static uint16_t server_send_handle;//ATT å‘é€handle
u32 server_send_handle=0;//ATT å‘é€handle

uint16_t ble_conn_handle;//è®¾å¤‡è¿æ¥handle
static u8 ble_mutex_flag;// bleä¸spp äº’æ–¥æ ‡è®°,0:è¡¨ç¤ºbleå¯è¿æ¥ï¼Œ1ï¼šè¡¨ç¤ºbleä¸å¯è¿æ¥

static u32 app_data_send(u8 *data,u16 len);
static void app_data_recieve(u8 *data, u16 len);

void app_server_le_msg_callback(uint16_t msg, uint8_t *buffer, uint16_t buffer_size)
{
    printf("\n%s, msg= 0x%x\n",__FUNCTION__,msg);
    switch(msg)
    {
    case BTSTACK_LE_MSG_CONNECT_COMPLETE:
        ble_conn_handle = buffer[0]+ (buffer[1]<<8);
        printf("conn_handle= 0x%04x\n",ble_conn_handle);

	#if SUPPORT_APP_RCSP_EN
        rcsp_event_com_start(RCSP_APP_TYPE_BLE);
        rcsp_register_comsend(app_data_send);
	#elif  BLE_FINGER_SPINNER_EN
		blefs_com_init();
		blefs_register_comsend(app_data_send);
	#endif // SUPPORT_APP_RCSP_EN
	#if defined(EASYESY_APP_ONOFF)
		disp_send_param(DISP_APP_ONOFF,ON);
	#endif
	#ifdef  TD_ADD_NEW_APP
		spp_connect_falg=1;
        os_taskq_post_msg(MUSIC_TASK_NAME, 2, SYS_BLE_LINK_OK);
	#endif
		break;

    case BTSTACK_LE_MSG_DISCONNECT_COMPLETE:
        printf("disconn_handle= 0x%04x\n",buffer[0]+ (buffer[1]<<8));
        server_send_handle = 0;
		#if BLE_FINGER_SPINNER_EN
		blefs_com_stop();
		#endif

        if(ble_conn_handle != 0)
        {
            ble_conn_handle = 0;

		#if SUPPORT_APP_RCSP_EN
            rcsp_event_com_stop();
		#endif // SUPPORT_APP_RCSP_EN

			if(ble_mutex_flag == 0)
            {
			#if !defined(DISENABLE_BLE_APP)
                server_advertise_enable();
			#endif
            }
        }
	#if defined(EASYESY_APP_ONOFF)
		disp_send_param(DISP_APP_ONOFF,OFF);
	#endif
	#ifdef  TD_ADD_NEW_APP
		spp_connect_falg=1;
		os_taskq_post_msg(MUSIC_TASK_NAME, 2, SYS_BLE_DIS_LINK);
	#endif
        break;

	case BTSTACK_LE_MSG_INIT_COMPLETE:
        puts("init server_adv\n");
	#if defined(DISENABLE_BLE_APP)
		server_advertise_disable();
	#else
		server_advertise_enable();
	#endif
		break;

    default:
        break;
    }

    //puts("exit_le_msg_callback\n");
}

#if defined(_ENABLE_BT_BLE_NAME_)
extern 	char *app_name;
#endif

uint16_t app_server_read_callback(uint16_t attribute_handle, uint8_t * buffer, uint16_t buffer_size)
{
    u16 handle = attribute_handle;

#if 1
    puts("app_server_read_callback\n");
    if(buffer_size > 0)
    {
        printf("\n read_cbk: handle= 0x%04x",handle);
        put_buf(buffer,buffer_size);
    }
//    return 0;///µ×²ãbug£¬»áµ÷ÓÃ2´Î

#endif
    switch (handle)
    {
    #if defined(_ENABLE_BT_BLE_NAME_)
    	case ATT_CHARACTERISTIC_2A00_01_VALUE_HANDLE:
    		puts("Device Name\n");
    		if(buffer_size == 0)
    		{
    			  return	strlen(app_name);
    		}
    		else if( NULL != buffer )
    		{
    			sprintf((char *)buffer,"%s",app_name);
    			//printf("len:%d\n",strlen((char *)buffer));
    			return	strlen(app_name);
    		}
    	break;
    #endif
        case ATT_CHARACTERISTIC_ae02_01_VALUE_HANDLE:
            puts("SoftVersion\n");
            
            #if 1//defined()
			if(buffer_size == 0)
			{
				return  sizeof(VisionTab);
			}
			else if( NULL != buffer )//´«Êı¾İ¸øÊÖ»úapp
			{
				sprintf((char *)buffer,"%s",VisionTab);//½«Êı¾İ×¼±¸ºÃ£¬²¢´«¸øÊÖ»úapp¶Ë
				//buffer = (uint8_t *)&VisionTab[0];
				//printf("len:%d\n",strlen((char *)buffer));
				return  sizeof(VisionTab);
			}

			#else
			if(buffer_size == 0)
			{
				return strlen(SOFTVERSION);
            }
            else if( NULL != buffer )//´«Êı¾İ¸øÊÖ»úapp
			{
                sprintf((char *)buffer,"%s",SOFTVERSION);//½«Êı¾İ×¼±¸ºÃ£¬²¢´«¸øÊÖ»úapp¶Ë
    //          buffer = (uint8_t *)"V0.0.2_11-10-21";
                printf("len:%d\n",strlen((char *)buffer));
                return strlen(SOFTVERSION);
            }
			#endif
        break;
    }
//    puts("app_server_read_callback---end\n");
    return 0;
}

#if BLE_APP_UPDATE_SUPPORT_EN
extern void bt_exit_low_power_mode(void);
static u8 ble_app_update_flag = 0;
u8 get_ble_app_update_flag(void)
{
	return ble_app_update_flag;		
}

void ble_send_update_cmd()
{  
    puts("-------ble_update_run\n");
	ble_app_update_flag = 1;
    os_taskq_post_msg(MAIN_TASK_NAME,2,MSG_UPDATA,BLE_UPDATE);
}  

extern u32 ble_update_save_att_server_info(const u8 *profile_data,u16 profile_len,u16 write_handle,u16 read_handle);
static void ble_update_save_att_server_info_handle(void)
{
	printf("profile_data size:%d\n",sizeof(profile_data));
	u8 ble_update_write_handle = ATT_CHARACTERISTIC_00001532_1212_EFDE_1523_785FEABCD123_01_VALUE_HANDLE; 
	u8 ble_update_notify_handle = ATT_CHARACTERISTIC_00001531_1212_EFDE_1523_785FEABCD123_01_VALUE_HANDLE;	
	ble_update_save_att_server_info(profile_data,sizeof(profile_data),ble_update_write_handle,ble_update_notify_handle);	
}
#endif


// ATT Client Write Callback for Dynamic Data
// @param attribute_handle to be read
// @param buffer
// @param buffer_size
// @return 0
uint16_t app_server_write_callback(uint16_t attribute_handle, uint8_t * buffer, uint16_t buffer_size)
{
    u16 handle = attribute_handle;
    //u16 copy_len;

	printf("---app_server_write_callback---\n");
#if 1
    if(buffer_size > 0)
    {
        printf("\n write_cbk: handle= 0x%04x",handle);
        put_buf(buffer,buffer_size);
    }
#endif

    switch (handle)
    {
	//    case ATT_CHARACTERISTIC_ae02_01_CLIENT_CONFIGURATION_HANDLE:
    case ATT_CHARACTERISTIC_ae03_01_CLIENT_CONFIGURATION_HANDLE:
		#if 1
        if(buffer[0] != 0)//01 00
        {
			printf("server_send_handle_ON\n"); //ÊÖ»ú¶Ë´ò¿ªÁË¼àÌı
            server_send_handle = handle -1;
			//sprintf((char *)buffer,"%s",SOFTVERSION);//½«Êı¾İ×¼±¸ºÃ£¬²¢´«¸øÊÖ»úapp¶Ë
		    // app_data_send(buffer,70);
			//Spp_Tx_Ack();
            //OSTimeDly(15);
            //Spp_Tx_WorkMode();
            //OSTimeDly(15);
            //Spp_Tx_Audio();
			//OSTimeDly(15);
            //Spp_Tx_WorkMode();
            //OSTimeDly(15);
            //Spp_Tx_Audio();
        }
        else
        {
			printf("server_send_handle_OFF\n");
            server_send_handle = 0;
        }
		#endif
        break;

    case ATT_CHARACTERISTIC_ae01_01_VALUE_HANDLE:

		printf("---app_server_write_callback11111---\n");
        /* printf("\n--write, %d\n",buffer_size); */
        /* put_buf(buffer,buffer_size); */
		//app_data_recieve(buffer,buffer_size);
	#if defined	TD_ADD_NEW_APP
		void new_app_rx_msg(u8 *ptr,u8 len);
		new_app_rx_msg(buffer,buffer_size);
	#elif defined(TD_ADD_SPP)
		extern void spp_rx_msg(u8 *ptr,u8 len);
		spp_rx_msg(buffer,buffer_size);
	#endif
		
        break;

#if BLE_APP_UPDATE_SUPPORT_EN
	case ATT_CHARACTERISTIC_00001531_1212_EFDE_1523_785FEABCD123_01_CLIENT_CONFIGURATION_HANDLE:
		if(buffer[0] != 0)
		{
			ble_send_update_cmd();
		}
		break;
		
	case ATT_CHARACTERISTIC_00001531_1212_EFDE_1523_785FEABCD123_01_VALUE_HANDLE: 
		break;

	case ATT_CHARACTERISTIC_00001532_1212_EFDE_1523_785FEABCD123_01_VALUE_HANDLE:
		break;
#endif

    default:
        break;
    }

	return 0;
}

//ble send data


static u32 app_data_send(u8 *data,u16 len)
{
    int ret_val;

	if(0 == server_send_handle)
	{
	    return 4;// is disconn
	}

    ret_val = server_notify_indicate_send(server_send_handle,data,len);

    if(ret_val !=  0)
    {
        if(ret_val == 0x57)
		{
			puts("stack busy!!!\n");
		}
        puts("\n app_ntfy_fail!!!\n");
        return ret_val;//disconn
    }
    return 0;
}
void  Notify_data_send(u8 *data,u16 len)
{
#ifdef	TD_ADD_NEW_APP
    if(spp_connect_falg ==0)
    {
		return;
    }
#endif
	if(app_data_send(data,len)==0)
	{
		/*printf("ble->-");
	    for(u8 i=0;i<len;i++)
	    {
			printf(" %x ",data[i]);
		}
		printf("--\n");*/
	}
}

extern u8 get_ble_test_key_flag(void);
void ble_server_send_test_key_num(u8 key_num)
{
	if(get_ble_test_key_flag())
	{
		if(!ble_conn_handle)
			return;		

		ble_user_send_test_key_num(ble_conn_handle,key_num);
	}
}

//ble recieve data
static void app_data_recieve(u8 *data, u16 len)
{
#if SUPPORT_APP_RCSP_EN
   rcsp_comdata_recieve(data,len);
#elif BLE_FINGER_SPINNER_EN
   blefs_comdata_parse(data,len);
#endif // SUPPORT_APP_RCSP_EN

} 
extern void server_select_scan_rsp_data(u8 data_type);
extern void server_set_scan_rsp_data(u8 *data);
extern void server_set_advertise_data(const u8 *data);
extern void app_advertisements_set_params(uint16_t adv_int_min, uint16_t adv_int_max, uint8_t adv_type,
                                   uint8_t direct_address_typ, u8 *direct_address, uint8_t channel_map, uint8_t filter_policy);
extern void server_connection_parameter_update(int enable);
extern void app_connect_set_params(uint16_t interval_min,uint16_t interval_max,uint16_t supervision_timeout);
extern void s_att_server_register_conn_update_complete(void (*handle)(uint16_t min,uint16_t max,uint16_t timeout));
/*
 * @brief Set Advertisement Paramters
 * @param adv_int_min
 * @param adv_int_max
 * @param adv_type
 * @param direct_address_type
 * @param direct_address
 * @param channel_map
 * @param filter_policy
 *
 * @note own_address_type is used from gap_random_address_set_mode
 */
void app_set_adv_parm(void)
{
// setup advertisements
	uint16_t adv_int_min = 0x0080;
	uint16_t adv_int_max = 0x00a0;
	uint8_t adv_type = 0;
	u8 null_addr[6];
	memset(null_addr, 0, 6);

	app_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
}
//Á¬½Ó²ÎÊıÇëÇó£¬Ö»ÄÜĞŞ¸Ä3¸ö²ÎÊı
void app_set_connect_param(void)
{
// setup connect
	uint16_t conn_interval_min = 50;
	uint16_t conn_interval_max = 120;
	uint16_t conn_supervision_timeout = 550;

	app_connect_set_params(conn_interval_min,conn_interval_max,conn_supervision_timeout);
}


void app_server_conn_update_callback(uint16_t min,uint16_t max,uint16_t timeout)
{
	printf("app_min = %d\n",min);
	printf("app_max = %d\n",max);
	printf("timeout= %d\n",timeout);
}

extern void ble_update_save_att_server_info_handle_register(void (*handle)(void));
void app_server_init(void)
{
    printf("111:\n%s\n",__FUNCTION__);
#if !defined(_ENABLE_BT_BLE_NAME_)
	server_register_profile_data(profile_data);
#endif
    server_set_advertise_data(profile_adv_ind_data);
	server_register_app_callbak(app_server_le_msg_callback,app_server_read_callback,app_server_write_callback);
//	server_register_app_callbak(app_server_le_msg_callback,NULL,app_server_write_callback);
#if defined(_ENABLE_BT_BLE_NAME_)
	server_select_scan_rsp_data(2); 	//scan_rspÀàĞÍÑ¡Ôñ£¬0£ºÄ¬ÈÏ£¬1£º»á×Ô¶¯ÌîĞ´bleÃû×Ö£¬ÆäËû×ÔÓÉÌîĞ´£¬2£ºÈ«²¿ÄÚÈİ×ÔÓÉÌîĞ´
#else
	server_select_scan_rsp_data(0); 	//scan_rspÀàĞÍÑ¡Ôñ£¬0£ºÄ¬ÈÏ£¬1£º»á×Ô¶¯ÌîĞ´bleÃû×Ö£¬ÆäËû×ÔÓÉÌîĞ´£¬2£ºÈ«²¿ÄÚÈİ×ÔÓÉÌîĞ´
	server_set_scan_rsp_data(profile_scan_rsp_data);//×¢²áscan_rsp°üÄÚÈİ
#endif
	app_set_adv_parm();//×¢²á¹ã²¥²ÎÊı
	server_connection_parameter_update(0);//Á¬½Ó²ÎÊıÊ¹ÄÜ£¬0£º²»ĞŞ¸ÄÁ¬½Ó²ÎÊı£¬1£ºĞŞ¸ÄÁ¬½Ó²ÎÊı
	app_set_connect_param();//×¢²áÁ¬½Ó²ÎÊı
	s_att_server_register_conn_update_complete(app_server_conn_update_callback);//×¢²áÁ¬½Ó²ÎÊıÇëÇó»Øµ÷º¯Êı
	ble_mutex_flag = 0;
#if BLE_APP_UPDATE_SUPPORT_EN
	ble_update_save_att_server_info_handle_register(ble_update_save_att_server_info_handle);	
#endif
	puts("-------app_server_init_succeed-------\n");
}

/*
spp å’Œ ble äº’æ–¥è¿æ¥
1ã€å½“spp è¿æ¥åï¼Œbleå˜ä¸ºä¸å¯è¿æ¥
2ã€å½“bleè¿ä¸Šåï¼Œè‹¥æœ‰sppè¿æ¥ä¸Šï¼Œåˆ™æ–­å¼€bleï¼›bleå˜ä¸ºä¸å¯è¿æ¥
 */
void ble_enter_mutex(void)
{
	P_FUNCTION


    if(ble_mutex_flag == 1)
	{
		return;
	}

	puts("in enter mutex\n");
    ble_mutex_flag = 1;
    if(ble_conn_handle != 0)
    {
        ble_hci_disconnect(ble_conn_handle);
		#if SUPPORT_APP_RCSP_EN
            rcsp_event_com_stop();
		#endif // SUPPORT_APP_RCSP_EN
    }
    else
    {
        server_advertise_disable();
    }
    puts("exit_enter_mu\n");
}

void ble_exit_mutex(void)
{
    P_FUNCTION

	if(ble_mutex_flag == 0)
	{
		return;
	}
   
	puts("in_exit mutex\n");
	ble_mutex_flag = 0;
	#if !defined(DISENABLE_BLE_APP)
   	server_advertise_enable();
	#endif
   	puts("exit_exit_mu\n");
}

void ble_server_disconnect(void)
{
    P_FUNCTION
    if(ble_conn_handle != 0)
    {
        printf("server discon handle= 0x%x\n ",ble_conn_handle); 
		ble_hci_disconnect(ble_conn_handle);
    }
    puts("exit_discnt\n");
}



#endif //#if (BLE_BREDR_MODE&BT_BLE_EN)
