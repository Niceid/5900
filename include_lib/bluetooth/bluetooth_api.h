#ifndef _BLUETOOTH_API_H_
#define _BLUETOOTH_API_H_

#include "typedef.h"
#include "uart.h"
#include "common/printf.h"

#define UART_BT_LIB_DEG

#ifdef UART_BT_LIB_DEG
    #define bt_puts                 puts
    #define bt_put_u32hex0          put_u32hex0
    #define bt_put_u32hex           put_u32hex
    #define bt_printf               printf

    #define bt_put_u16hex           put_u16hex
    #define bt_put_u8hex            put_u8hex
    #define bt_put_buf              put_buf
    #define bt_printf_buf           printf_buf
    #define bt_putchar              putchar
	#define bt_malloc_stats             malloc_stats
	#define bt_red_output	ew_red_printf
#else
    #define bt_puts(...)
    #define bt_put_u32hex0(...)
    #define bt_put_u32hex(...)
    #define bt_printf(...)

    #define bt_put_u16hex(...)
    #define bt_put_u8hex(...)
    #define bt_put_buf(...)
    #define bt_printf_buf(...)
    #define bt_putchar(...)
    #define bt_malloc_stats()
	#define bt_red_output(...)
#endif

//���ò��Ժв��Թ���
#define NON_TEST         0          ///<ûƵƫ�;������
#define FRE_OFFSET_TEST  BIT(0)     ///<Ƶƫ����
#define DISTANCE_TEST    BIT(1)     ///<�������


#define NORMAL_MODE         0
#define TEST_BQB_MODE       1       ///<����bqb��֤
#define TEST_FCC_MODE       2       ///<����fcc��֤
#define TEST_FRE_OFF_MODE   3       ///<����Ƶƫ(ʹ��Ƶ�׷�����-���������-����Ƶ��Ĭ��2422M)
#define TEST_PERFOR_MODE    4       ///<ָ�����ܲ���(ʹ��MT8852A��������,����оƬ���ܵ�ʱ��ʹ��)
#define TEST_BOX_MODE       5       ///<���Ժв���

//����Low power mode
#define SNIFF_EN                            BIT(0)  ///<SNIFFʹ��
#define SNIFF_TOW_CONN_ENTER_POWERDOWN_EN   BIT(3)  ///<SNIFF �ȴ���̨�����ӲŽ�powerdown
#define SNIFF_CNT_TIME                      10      ///<����10S֮�����sniffģʽ

//����Low power mode
#define BT_POWER_OFF_EN                   BIT(1)  ///<SNIFF ����poweroff
#define BT_POWER_DOWN_EN                  BIT(2)  ///<SNIFF ����powerdown



#define BT_BREDR_EN  BIT(0)
#define BT_BLE_EN    BIT(1)
enum
{
	TWS_CHANNEL_LEFT =1, //�̶�Ϊ���	
	TWS_CHANNEL_RIGHT=2,//�̶�Ϊ�Ҷ�	
};

extern bool get_resume_flag();
extern bool is_bt_in_background_mode();
extern void delete_stack_task(void);
extern void no_background_suspend();
extern void background_suspend();
extern bool get_suspend_flag();
extern void hook_btstack_suspend();
extern void hook_btstack_resume(u8 suspend_flag);
extern bool get_auto_suspend_flag();
extern void clean_auto_suspend_flag();
extern void set_bt_config_check_callback(void (*fun)(void));
extern void set_bt_info_callback(void (*fun)(void));
extern void register_edr_init_handle();

extern void ble_register_init_config(void (*handler)(void));
extern void register_ble_init_handle();
extern void register_stereo_init_handle();

extern u8 get_chip_index();

void power_init(u8 mode , u32 sys_freq, u8 charge_flag);

/*******************************************************************/
/*
 *-------------------LE READ PARAMETER
 */
void ble_controller_suspend(void);

void ble_controller_resume(void);


#endif
