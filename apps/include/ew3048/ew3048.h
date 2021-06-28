#ifndef __EW3048_H__
#define __EW3048_H__

#if defined(_SUPPORT_CAN_UART_)

#define  CMD_SET_COLOR_11      		  0x01 //�߲ʵ�R
#define  CMD_SET_COLOR_12      		  0x02 //�߲ʵ�G
#define  CMD_SET_COLOR_13      		  0x03 //�߲ʵ�B
#define  CMD_SET_MUTE_15      	  	  0x04 //MUTE
#define  CMD_SET_BUS_LIGHT_16      	  0x05 //�����
#define  CMD_SET_EXT_LIGHT_17         0x06 //���ڵ�
#define  CMD_SET_TEMP_POWER_18        0x07 //����
#define  CMD_SET_TEMP_POWER_19        0x08 //����
#define  CMD_SET_ALARM_20             0x09 //����


extern void McuPushReceive(u8 data);

extern u8 McuPopReceive(u8 *data);

extern void McuDriverServer(void);

extern void mcu_disp_init(void);

extern void Mcu_Send_Data(u8 *data,u8 length);

#endif

#endif /* __MCU_H__ */

