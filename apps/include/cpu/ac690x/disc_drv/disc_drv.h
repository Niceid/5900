#ifndef _DISC_DRV_H_
#define _DISC_DRV_H_


#if defined(_SUPPORT_DISC_)

#if defined(_ENABLE_RADIO_TIME_)
#define DISC_POLLING_TIME							(20)
#else
#define DISC_POLLING_TIME							(10)
#endif
#define DetectDiscIn()								DISC_SWA_CHECK()
#define DetectDiscOut()								DISC_SWB_CHECK()
#define DetectDiscStop()							DISC_SWC_CHECK()

#define SERVO_BUFFER_SIZE							200//100
#define SERVO_FRAM_SIZE								20
#define SERVO_QUEUE_SIZE							10//5
#define SPI_ACK_BREAK								1
#define SPI_ACK_CONTINUE							0
#define SPI_STATUS_DELAY							250

typedef enum{
	SERVO_CMD_ENABLE,
	SERVO_CMD_DISABLE,
	SERVO_CMD_SEND,
	SERVO_CMD_RECEIVE,
}SERVO_CMD_e;

typedef enum{
	DISC_MODE_INIT,
	DISC_MODE_RUN,
	DISC_MODE_EXIT,
}DISC_MODE_e;

typedef enum{
	DISC_STATE_IDLE,
	DISC_STATE_OPEN,
	DISC_STATE_CLOSE,
	DISC_STATE_NO_DISC,
}DISC_STATE_e;

typedef enum{
	DISC_OUTPUT_IDLE,
	DISC_OUTPUT_OPEN,
	DISC_OUTPUT_CLOSE,
	DISC_OUTPUT_LOCK,
}DISC_OUTPUT_e;


typedef struct{
	uint8 read;
	uint8 write;
	//uint16 read;
	//uint16 write;
	uint8 buffer[SERVO_BUFFER_SIZE];
}SERVO_BUFFER_t;

typedef struct{
	uint8 length;
	uint8 buffer[SERVO_FRAM_SIZE];
}SERVO_FRAM_BUFFER_t;

typedef struct{
	uint8 read;
	uint8 write;
	SERVO_FRAM_BUFFER_t data[SERVO_QUEUE_SIZE];
}SERVO_FRAM_QUEUE_t;

typedef struct{
	uint8 send_busy;
	uint8 receive_state;
	uint8 packet_length;
	uint8 packet_index;
	uint8 packet_buffer[SERVO_FRAM_SIZE];
}SERVO_MCU_t;

typedef struct{
	char *task_name;
	SERVO_BUFFER_t uart_send;
	SERVO_BUFFER_t uart_recive;
	SERVO_FRAM_QUEUE_t fram_send;
	SERVO_FRAM_QUEUE_t fram_receive;
	SERVO_FRAM_BUFFER_t temp_fram;
	SERVO_MCU_t mcu_ctrl;
}SERVO_CTRL_t;


typedef struct{
	uint8 disable;
	uint16 time;
	DISC_STATE_e state;
	DISC_STATE_e next_state;
	DISC_MODE_e mode;
	uint8 frist;
	uint8 auto_run;
	uint8 power;
	uint8 eject;
	U_FUNC_U eject_func;
	uint8 eject_disp_flag;
	
	struct{
		uint16 time;
		uint8 state;
		uint8 detect_in;
		uint8 detect_out;
		uint8 detect_stop;
	}idle;
	struct{
		uint16 time;
		uint8 state;
	}open;
	struct{
		uint16 time;
		uint16 plase_delay;
		uint8 state;
	}close;
	struct{
		uint16 time;
		uint8 state;
	}no_disc;
	
	uint8 servo_work;
	uint16 reset_time;
	SERVO_CTRL_t *servo_ctrl;
}DISC_DRV_t;

typedef struct{
	V_FUNC_V 	init;
	V_FUNC_V 	run;
	V_FUNC_V	exit;	
}DISC_MODE_FUNC_t;

extern DISC_DRV_t disc_drv;

extern void disc_drv_init(void);
extern void disc_drv_process(void);
extern void disc_drv_key(void);
extern void disc_drv_disable(void);
extern uint8 disc_drv_auto(uint8);
extern void disc_drv_eject(U_FUNC_U);
extern void disc_servo_enable(char *);
extern void disc_servo_disable(void);
extern void disc_servo_process(void);
extern uint8 disc_servo_receive(uint8 *, uint8 *);
extern uint8 disc_servo_send(const uint8 *, uint8);
extern void ServoPushReceive(uint8 data);
extern uint8 ServoPopSend(uint8 *data);

#if defined(_ENABLE_REJECT_)
extern void disc_drv_key1(void);
#endif

#endif

#endif /* _DISC_DRV_H_ */

