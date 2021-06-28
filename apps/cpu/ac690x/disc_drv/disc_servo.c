#include "includes.h"
#include "msg.h"
#include "disc_drv/disc_drv.h"
#include "disc/disc.h"

#if defined(_SUPPORT_DISC_)

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);

#define MCU_UART_START_FLAG1								(0xFF)
#define MCU_UART_START_FLAG2								(0xAA)

enum{
	MCU_RECEIVE_STATE_START = 0,
	MCU_RECEIVE_STATE_CHECK,
	MCU_RECEIVE_STATE_LENGTH,
	MCU_RECEIVE_STATE_DATA,
};

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

static void ServoPushSend(uint8 data)
{
	SERVO_BUFFER_t *buffer = &disc_drv.servo_ctrl->uart_send;
	buffer->buffer[buffer->write++] = data;
	if(buffer->write >= SERVO_BUFFER_SIZE)
		buffer->write = 0;
}

uint8 ServoPopSend(uint8 *data)
{
	SERVO_BUFFER_t *buffer = &disc_drv.servo_ctrl->uart_send;
	
	if(buffer->read != buffer->write)
	{
		*data = buffer->buffer[buffer->read++];
		if(buffer->read >= SERVO_BUFFER_SIZE)
			buffer->read = 0;
		return TRUE;
	}
	return FALSE;
}

void ServoPushReceive(uint8 data)
{
	SERVO_BUFFER_t *buffer = &disc_drv.servo_ctrl->uart_recive;
	
	buffer->buffer[buffer->write++] = data;
	if(buffer->write >= SERVO_BUFFER_SIZE)
		buffer->write = 0;
}

static uint8 ServoPopReceive(uint8 *data)
{
	SERVO_BUFFER_t *buffer = &disc_drv.servo_ctrl->uart_recive;
	
	if(buffer->read != buffer->write)
	{	
		*data = buffer->buffer[buffer->read++];
		if(buffer->read >= SERVO_BUFFER_SIZE)
			buffer->read = 0;
		return TRUE;
	}
	return FALSE;
}

static void ServoPushSendFram(const SERVO_FRAM_BUFFER_t *data)
{
	SERVO_FRAM_QUEUE_t *queue = &disc_drv.servo_ctrl->fram_send;
	
	queue->data[queue->write++] = *data;
	if(queue->write >= SERVO_QUEUE_SIZE)
		queue->write = 0;
}

static uint8 ServoPopSendFram(SERVO_FRAM_BUFFER_t *data)
{
	SERVO_FRAM_QUEUE_t *queue = &disc_drv.servo_ctrl->fram_send;
	
	if(queue->read != queue->write)
	{	
		*data = queue->data[queue->read++];
		if(queue->read >= SERVO_QUEUE_SIZE)
			queue->read = 0;
		return TRUE;
	}
	return FALSE;
}

static void ServoPushReceiveFram(const SERVO_FRAM_BUFFER_t *data)
{
	SERVO_FRAM_QUEUE_t *queue = &disc_drv.servo_ctrl->fram_receive;
	
	queue->data[queue->write++] = *data;
	if(queue->write >= SERVO_QUEUE_SIZE)
		queue->write = 0;
}

static uint8 ServoPopReceiveFram(SERVO_FRAM_BUFFER_t *data)
{
	SERVO_FRAM_QUEUE_t *queue = &disc_drv.servo_ctrl->fram_receive;
	
	if(queue->read != queue->write)
	{	
		*data = queue->data[queue->read++];
		if(queue->read >= SERVO_QUEUE_SIZE)
			queue->read = 0;
		return TRUE;
	}
	return FALSE;
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

static void ServoDriverSucceed(uint8 send_receive)
{
	if(send_receive)
	{
		//ew_puts("Servo_Send succeed!!!\n");
		if(disc_drv.servo_ctrl->task_name != NULL)
		{
			os_taskq_post_msg(disc_drv.servo_ctrl->task_name, 1, SYS_EVENT_SERVO_SEND);
		}
	}
	else
	{
		//ew_puts("Servo_Receive succeed!!!\n");
		if(disc_drv.servo_ctrl->task_name != NULL)
		{
			os_taskq_post_msg(disc_drv.servo_ctrl->task_name, 1, SYS_EVENT_SERVO_RECEIVE);
		}
	}
}

static void ServoDriverServer(void)
{
	uint8 index;
	uint8 readData;
	SERVO_FRAM_BUFFER_t *fram = &disc_drv.servo_ctrl->temp_fram;
	SERVO_MCU_t *mcu = &disc_drv.servo_ctrl->mcu_ctrl;
	
	while(ServoPopSendFram(fram))
	{
		//ServoPushSend(MCU_UART_START_FLAG1);
		//ServoPushSend(MCU_UART_START_FLAG2);
		//ServoPushSend(fram->length);
		
		uart_interrupt_receive_write(MCU_UART_START_FLAG1);
		uart_interrupt_receive_write(MCU_UART_START_FLAG2);
		uart_interrupt_receive_write(fram->length);
		for(index = 0; index < fram->length; index++)
		{
			//ServoPushSend(fram->buffer[index]);
			uart_interrupt_receive_write(fram->buffer[index]);
		}
		
		/*if(!mcu->send_busy)
		{
			if(ServoPopSend(&readData))
			{
				mcu->send_busy = TRUE;
			}
		}*/
		ServoDriverSucceed(TRUE);
	}
	
	while(ServoPopReceive(&readData))
	{
		//ew_printf("0x%02x ", readData);
		//ew_printf("receive_state:%d\n",mcu->receive_state);
		switch(mcu->receive_state)
		{
			case MCU_RECEIVE_STATE_START:
				if(readData == MCU_UART_START_FLAG1)
				{
					mcu->receive_state = MCU_RECEIVE_STATE_CHECK;
				}
				break;
				
			case MCU_RECEIVE_STATE_CHECK:
				if(readData == MCU_UART_START_FLAG2)
				{
					mcu->receive_state = MCU_RECEIVE_STATE_LENGTH;
				}
				else
				{
					mcu->receive_state = MCU_RECEIVE_STATE_START;
				}
				break;
				
			case MCU_RECEIVE_STATE_LENGTH:
				if(readData <= sizeof(mcu->packet_buffer))
				{
					mcu->packet_length = readData;
					mcu->packet_index = 0;
					mcu->receive_state = MCU_RECEIVE_STATE_DATA;
				}
				else
				{
					mcu->receive_state = MCU_RECEIVE_STATE_START;
				}
				break;
				
			case MCU_RECEIVE_STATE_DATA:
				mcu->packet_buffer[mcu->packet_index++] = readData;
				if(mcu->packet_index == mcu->packet_length)
				{
					fram->length = mcu->packet_length;
					for(index = 0; index < mcu->packet_length; index++)
					{
						fram->buffer[index] = mcu->packet_buffer[index];
					}
					ServoPushReceiveFram(fram);
					ServoDriverSucceed(FALSE);
					mcu->receive_state = MCU_RECEIVE_STATE_START;
				}
				break;	
		}
	}
}

static void ServoDriverEnable(void)
{
	ew_puts("ServoDriverEnable\n");
	
#if defined(_SERVR_USE_8202RQ_D_)
	DISC_POWER_ON();
	DISC_RESET_OFF();
#else
	DISC_POWER_ON();
	DISC_RESET_ON();
#endif

#if defined(_SERVR_USE_8104L_ORDER_)
	disc_drv.reset_time = 700/DISC_POLLING_TIME; 
#else
	disc_drv.reset_time = 500/DISC_POLLING_TIME; 
#endif
	
	disc_drv.servo_work = TRUE;
}

static void ServoDriverDisable(void)
{
	ew_puts("ServoDriverDisable\n");
	
	disc_drv.servo_work = FALSE;
	disc_drv.reset_time = 0;

	if(!disc_drv.power)
	{
		DISC_POWER_OFF();
	}
	DISC_RESET_ON();
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_ACC_OFF_RESET_)
SERVO_CTRL_t SERVO_CTRL_val;
#endif

void disc_servo_enable(char *name)
{
#if defined(_ENABLE_ACC_OFF_RESET_)
	disc_drv.servo_ctrl = &SERVO_CTRL_val;
#else
	if(disc_drv.servo_ctrl == NULL)
	{
		disc_drv.servo_ctrl = (SERVO_CTRL_t *)malloc_fun(disc_drv.servo_ctrl ,sizeof(SERVO_CTRL_t), 0);
		if(disc_drv.servo_ctrl == NULL)
		{
			ew_puts("disc_servo_enable --- memory error!!!\n");
	        return;
		}
	}
#endif
	memset(disc_drv.servo_ctrl, 0, sizeof(SERVO_CTRL_t));
	disc_drv.servo_ctrl->task_name = name;
	
	ServoDriverEnable();
}

void disc_servo_disable(void)
{
	ServoDriverDisable();
	
	if(disc_drv.servo_ctrl != NULL)
	{
	#if !defined(_ENABLE_ACC_OFF_RESET_)
		free_fun((void**)&disc_drv.servo_ctrl);
	#endif
	}
}

void disc_servo_process(void)
{
	if(disc_drv.reset_time)
	{
		if(!--disc_drv.reset_time)
		{
			DISC_RESET_OFF();
			ew_puts("DISC_RESET_OFF\n");
		}
	}
	else if(disc_drv.servo_work)
	{
		ServoDriverServer();
	}
}

uint8 disc_servo_receive(uint8 *buffer, uint8 *length)
{	
	uint8 result = FALSE;
	
	if(disc_drv.servo_work)
	{
		SERVO_FRAM_BUFFER_t data;
		
		if(ServoPopReceiveFram(&data))
		{
			if(*length >= data.length)
			{
				*length = data.length;
				memcpy(buffer, data.buffer, data.length);
				result = TRUE;
			}
		}
	}
	
	return result;
}

uint8 disc_servo_send(const uint8 *buffer, uint8 length)
{
	uint8 result = FALSE;

	if(disc_drv.servo_work)
	{
		SERVO_FRAM_BUFFER_t data;
		
		if(sizeof(data.buffer) >= length)
		{
			data.length = length;
			memcpy(data.buffer, buffer, length);
			ServoPushSendFram(&data);
			result = TRUE;
		}
	}
	
	return result;
}

#endif

