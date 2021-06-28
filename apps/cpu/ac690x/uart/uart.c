#include "sdk_cfg.h"
#include "cpu/uart_api.h"
#include "uart_param.h"
#include "dac/eq.h"
#include "fcc_test.h"
#include "clock.h"
#include "disc_drv/disc_drv.h"
#if defined(TD_ADD_W2DAB_UART)
#include "dab\dab.h"
#endif

#if defined(_USER_TFT_DISP_)
#include "AMT630A\AMT630A.h"
#endif

#if defined(_SUPPORT_CAN_UART_)
#include "ew3048\ew3048.h"
#endif

#if defined(_AUDIO_DRIVER_AP3768_)
#include "ap3768\ap3768.h"
#endif

__uart_handle uart0_handle;
__uart_handle uart1_handle;
__uart_handle uart2_handle;

__uart_handle *uart_put_handle;
__uart_handle *uart_updata_handle;
__uart_handle *fcc_uart_handle;
__uart_handle *eq_dbg_handle;

extern void register_handle_printf_putchar(void (*handle)(char a));


/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/

s32 uart_set_baud(u32 baud)
{
	uart_module_ctrl(uart_put_handle, UART_CMD_SET_BAUN_RATE,baud,NULL);
	return 0;
}

static int uart_reset(void)
{
    uart_set_baud(UART_BAUD_RAE);
    return 0;
}

static struct clock_switch clock_switch_uart;

CLOCK_SWITCH_USER_REGISTER(CLOCK_SWITCH_EXIT, &clock_switch_uart, uart_reset, "UART");

#define UART0   	0
#define UART1   	1
#define UART2   	2
#if defined(use_usb_as_uart)
#define UART_NO		UART1
#define UART_IO		UART_USBP_USBM
#elif defined(use_mic_as_uart)
#define UART_NO		UART0
#define UART_IO		UART_TXPA5_RXPA6
#elif defined(_SUPPORT_C912DAB_)||defined(TD_ADD_W2DAB_UART)//使用dab模块是调试打印口
#define UART_NO		UART0
#define UART_IO		UART_TXPC2_RXPC3
#elif defined(songshang_debug)//松尚机器只剩下这两个脚可以打印
#define UART_NO		UART1
#define UART_IO		UART_TXPC0_RXPC1
#elif defined(_SUPPORT_PB6PB7_)
#define UART_NO		UART0
#define UART_IO		UART_TXPB6_RXPB7
#elif defined(_SUPPORT_PA9PA10_)
#define UART_NO		UART2
#define UART_IO		UART_TXPA9_RXPA10
#else
#define UART_NO		UART1
#define UART_IO		UART_USBP_USBM
#endif

#define UART_PORT	JL_PORTB
#define UART_BIT	BIT(12)
s32 uart_init(u32 baud)
{
    s32 status = 0;
	__uart_handle *uart_handle;
	char *uart_name;

    uart_put_handle = NULL;
	if(UART_NO == UART0)
	{
		uart_handle = &uart0_handle;
		uart_name = UART0_HARDWARE_NAME;
	}
	else if(UART_NO == UART1)
	{
		uart_handle = &uart1_handle;
		uart_name = UART1_HARDWARE_NAME;
	}
	else if(UART_NO == UART2)
	{
		uart_handle = &uart2_handle;
		uart_name = UART2_HARDWARE_NAME;
	}
	else
	{
		return UART_DRV_USED_ERR;
	}

    uart_module_on();

#if defined(__EW_DEBUG__)
    status = uart_module_open(uart_handle,uart_name);
    if(!status)
    {
       __uart_param cur_param;
	   memset(&cur_param,0,sizeof(__uart_param));
       cur_param.baud_rate = baud;
       cur_param.io = UART_IO;
       cur_param.workmode = UART_WORKMODE_NORMAL;
       cur_param.custom = 0;
       status = uart_module_init(uart_handle,&cur_param);
	   if(UART_OUTPUT_CHAL == cur_param.io)
	   {
		   UART_PORT->DIR &= ~UART_BIT;
		   UART_PORT->DIE &= ~UART_BIT;
		   UART_PORT->PD |= UART_BIT;
		   UART_PORT->PU |= UART_BIT;
	   }
       if(!status)
       {
           status = uart_module_start(uart_handle);
           if(!status)
           {
               uart_put_handle = uart_handle;
           }
       }
    }
	register_handle_printf_putchar(putchar);//printf重映射
#endif

    return status;
}

/*void fcc_uart_init(void)
{
	u32 status = 0;
	puts("-----fcc_uart_init\n");
    fcc_uart_handle=NULL;
    status=uart_module_open(&uart1_handle,UART1_HARDWARE_NAME);
    if(!status)
    {
       __uart_param fcc_param;
	   memset(&fcc_param,0,sizeof(__uart_param));
       fcc_param.baud_rate = 9600;
       fcc_param.io = UART_USBP_USBM;
       fcc_param.workmode = UART_WORKMODE_NORMAL;
	   fcc_param.custom |= (BIT(14)|BIT(3));
       status = uart_module_init(&uart1_handle,&fcc_param);
	   uart_reg_isr_callback_fun(&uart1_handle,5,fcc_uart_isr_callback);
	   if(status)
	   {
		   puts("uart_module_init err\n");
	   }
       if(!status)
       {
           status=uart_module_start(&uart1_handle);
           if(!status)
           {
               fcc_uart_handle=&uart1_handle;
           }
       }
    }
}

void fcc_uart_write(char a)
{
	if(fcc_uart_handle)
	{
		fcc_uart_handle->putbyte(fcc_uart_handle,a);
	}
	else
	{
		puts("fcc_uart_write err\n");
	}
}
*/
#if UART_UPDATA_EN

static u8 uart_rx_buf[4];
const u8 uart_updata_flag[4] = {0xEA, 0x55 ,0xAA ,0xAE};
void uart_updata_isr_callback(u8 uto_buf,void *p , u8 isr_flag)
{
	u32 ret;
    ret = memcmp(uart_updata_flag, uart_rx_buf,4);
    if(UART_ISR_TYPE_DATA_COME == isr_flag)
    {
        if(0 == ret)
        {
            puts("----MSG_UPDATA MSG_UPDATA----\n");
            os_taskq_post_msg("MainTask",3,MSG_UPDATA,UART_UPDATA, NULL);
        }
        else
        {
            uart_module_ctrl(uart_updata_handle,UART_CMD_DMA_WR_SADR,sizeof(uart_rx_buf),uart_rx_buf);
            uart_module_ctrl(uart_updata_handle,UART_CMD_DMA_WR_CNT,sizeof(uart_rx_buf),uart_rx_buf);
        }
    }
//        printf_buf(uart_rx_buf,4);
//        printf("ret = %x\n",ret);
}

s32 uart_update_init(void)
{
	puts("--------uart_update_init\n");
    s32 status;
    uart_updata_handle=NULL;
    status=uart_module_open(&uart1_handle,UART1_HARDWARE_NAME);
    if(!status)
    {
       __uart_param uart_updata_param;
	   memset(&uart_updata_param,0,sizeof(__uart_param));
       uart_updata_param.baud_rate=460800;
       uart_updata_param.io=UART_INPUT_CHAL;
       uart_updata_param.workmode=UART_WORKMODE_DMA;
       uart_updata_param.custom |= BIT(14)|BIT(5)|BIT(3)|BIT(2);
       uart_updata_param.custom |= BIT(7);
       status=uart_module_init(&uart1_handle,&uart_updata_param);
	   uart_reg_isr_callback_fun(&uart1_handle,5,uart_updata_isr_callback);
	   /* printf("uart_rx_buf=0x%x,size=%d\n",uart_rx_buf,sizeof(uart_rx_buf) ); */
       uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_SADR,sizeof(uart_rx_buf),uart_rx_buf);
       uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_EADR,sizeof(uart_rx_buf),uart_rx_buf);
       uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_CNT,sizeof(uart_rx_buf),uart_rx_buf);
       uart_module_ctrl(&uart1_handle,UART_CMD_OT,0xffff,NULL);

///IOMC2 BIT(29)-BIT(24):
///0x00-0x0f   PA[0:15]
///0x10-0x1f   PB[0:15]
///0x2b-0x2f   PC[11:15]
///0x30-0x37   PD[0:7]
///0x3e        USBDP
///0x3f        USBDM
	   if(UART_INPUT_CHAL==uart_updata_param.io)
	   {
		   /*--------------------PA0----------------------*/
           uart_module_ctrl(&uart1_handle,UART_CMD_INPUT_CHAL,/*0x3e*/0,NULL);//0x3e
           PORTA_PU |= BIT(0);
           PORTA_DIR |= BIT(0);
	   }
       if(!status)
       {
           status=uart_module_start(&uart1_handle);
           if(!status)
           {
              uart_updata_handle = &uart1_handle;
           }
       }
    }
    return status;
}

#endif

#if EQ_EN
#if EQ_UART_BDG
/*if you want to change eq uart port ,you can change the #define vlaue below*/
#define eq_uart_handle uart1_handle
#define EQ_UART_NAME   UART1_HARDWARE_NAME
#define EQ_UART_PORT   UART_TXPA1_RXPA2
static u8 eq_uart_rx_buf[512];

static void eq_dbg_uart_isr_callback(u8 uto_buf, void *p ,u8 isr_flag)    //p rx  buf
{
    static u8 set_eq_flag;
    u8 res;

    if(UART_ISR_TYPE_DATA_COME ==isr_flag)
    {
        if(eq_uart_rx_buf[0]=='E'&&eq_uart_rx_buf[1]=='Q')
        {
            putchar('#');
            set_eq_flag |= BIT(eq_uart_rx_buf[2]);
            if(set_eq_flag&BIT(4))
            {
                 //the last packet length is not the same as before
                res = update_ef_info(eq_uart_rx_buf+4,2140-4*sizeof(eq_uart_rx_buf)-4,eq_uart_rx_buf[2]);
            }else{
                res = update_ef_info(eq_uart_rx_buf+4,sizeof(eq_uart_rx_buf)-4,eq_uart_rx_buf[2]);
            }

            if(set_eq_flag&BIT(3))
            {
                uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_SADR,sizeof(eq_uart_rx_buf),eq_uart_rx_buf);
                uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_EADR,sizeof(eq_uart_rx_buf),eq_uart_rx_buf);
                uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_CNT,2140-4*sizeof(eq_uart_rx_buf) ,eq_uart_rx_buf);   //LEFT DATA SIZE
            }
            if((res))
            {
                puts("eq_info_rev_success!!!\n");
                set_eq_flag = 0;
                uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_SADR,sizeof(eq_uart_rx_buf),eq_uart_rx_buf);
                uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_EADR,sizeof(eq_uart_rx_buf),eq_uart_rx_buf);
                uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_CNT,sizeof(eq_uart_rx_buf),eq_uart_rx_buf);
            }
        }
    }

    if(UART_ISR_TYPE_TIMEOUT ==isr_flag)
    {
        set_eq_flag = 0;
    }
    uart_module_ctrl(&eq_uart_handle,UART_CMD_OT,54321*1000,NULL);
}

s32 EQ_uart_init(void)
{
	s32 status;

	eq_dbg_handle = NULL;
   // puts("----eq uart init\n");
    status=uart_module_open(&eq_uart_handle,EQ_UART_NAME);
    if(!status)
    {
       __uart_param eq_param;
	   memset(&eq_param,0,sizeof(__uart_param));
       eq_param.baud_rate=115200;
       eq_param.io=EQ_UART_PORT;
       eq_param.workmode=UART_WORKMODE_NORMAL;
       eq_param.custom = 0;
  //     UT1_CON = BIT(13) | BIT(12) |BIT(10)| BIT(6)| BIT(5) |BIT(3)|BIT(0);       //rx_ie , CLR RX PND  6:DMA鎺ユ敹涓柇
        //鎺ユ敹瓒呮椂涓柇鍏佽(5),鎺ユ敹涓柇鍏佽(3),鍙戦�佷腑鏂厑璁?2),BIT(7)娓匲T1_HRCNT
       eq_param.custom |= BIT(13) | BIT(12) |BIT(10)| BIT(7)|BIT(6)| BIT(5) |BIT(3)|BIT(0);
       status=uart_module_init(&eq_uart_handle,&eq_param);
       uart_reg_isr_callback_fun(&eq_uart_handle,2,eq_dbg_uart_isr_callback);
	   /* printf("eq_uart_rx_buf=0x%x,size=%d\n",eq_uart_rx_buf,sizeof(eq_uart_rx_buf) ); */
       uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_SADR,sizeof(eq_uart_rx_buf),eq_uart_rx_buf);
       uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_EADR,sizeof(eq_uart_rx_buf),eq_uart_rx_buf);
       uart_module_ctrl(&eq_uart_handle,UART_CMD_DMA_WR_CNT,sizeof(eq_uart_rx_buf),eq_uart_rx_buf);
       uart_module_ctrl(&eq_uart_handle,UART_CMD_OT,80000*1000,NULL);

      if(!status)
       {
           status=uart_module_start(&eq_uart_handle);
           if(!status)
               eq_dbg_handle=&eq_uart_handle;
       }
    }
    return status;
}

#endif
#endif



#ifdef TD_ADD_USER_UART
u8 gRecvStep=0;
u8 gRecvData=0;
u8 gRecvTime=0;
u8 gRecvCheckSum=0;
u8 Uart_Recv_Buf[40];
u8 gC912detOk=0;
u8 Disp_dab_buf[32];
__uart_handle *uart_interrupt_receive_handle;
void uart_interrupt_receive_isr_callback(u8 uto_buf,void *p,u8 isr_flag)
{
	if(UART_ISR_TYPE_DATA_COME == isr_flag)
	{
        gRecvData =uto_buf;//ServoPushReceive(gRecvData);
		switch(gRecvStep)
		{
			case 0:
                if(gRecvData =='O')
                {
					gRecvStep =1;
                }
                else if(gRecvData ==0xff)
                {
                    gRecvStep =2;
                }
				break;
			case 1:
                gRecvStep =0;
                if(gRecvData =='N')
                {
                    gC912detOk=1;
					os_taskq_post(MAIN_TASK_NAME,1,MSG_SWITCH_BAUD);
                }
            case 2:
                if(gRecvData ==0x23)
                {
                    gRecvStep =3;
                }
                else
                {
                    gRecvStep =0;
                }
				break;
            case 3:
				if((gRecvData ==0x10)||(gRecvData ==0x08))
				{
					gRecvStep =4;
				}
				else
				{
					gRecvStep =0;
				}
				break;
			case 4:
				if((gRecvData ==0x02)||(gRecvData ==0x01))
				{
					gRecvStep =5;
				}
				else
				{
					gRecvStep =0;
				}
				break;
			case 5:
				if(gRecvData ==0x01)
				{
					gRecvStep =6;
				}
				else
				{
					gRecvStep =0;
				}
				break;
        	default:
                Uart_Recv_Buf[gRecvStep]=gRecvData;
				if(gRecvStep <=37)
				{
					gRecvCheckSum +=Uart_Recv_Buf[gRecvStep];
				}
                gRecvStep ++;
                if((gRecvStep ==39)//长度相等
				&&((gRecvCheckSum +Uart_Recv_Buf[gRecvStep-1])==256))
				{
                    memcpy(Disp_dab_buf,&Uart_Recv_Buf[6],32);
                    gRecvCheckSum =0;
                    gRecvStep =0;
				}
                if(gRecvStep >39)
                {
                    gRecvCheckSum =0;
                    gRecvStep =0;
                }
        		break;
		}
		gRecvTime=50;
//        uart_interrupt_receive_write(uto_buf);
	}
}
void uart_interrupt_receive_write(char a)
{
	if(uart_interrupt_receive_handle)
	{
		uart_interrupt_receive_handle->putbyte(uart_interrupt_receive_handle,a);
	}
	else
	{
		puts("uart_interrupt_receive_write err\n");
        os_taskq_post(MAIN_TASK_NAME,1,MSG_UART_ERR);
	}
}
s32 uart_interrupt_receive_set_baud(u32 baud)
{
	uart_module_ctrl(uart_interrupt_receive_handle,UART_CMD_SET_BAUN_RATE,baud,NULL);
	return 0;
}
void Close_Uart2(void)
{
    uart_unreg_isr_callback_fun(&uart2_handle,1,uart_interrupt_receive_isr_callback);
    uart_module_stop(&uart2_handle);
    uart_module_close(&uart2_handle);
}
s32 uart_interrupt_receive_init(u32 baud)
{
	s32 status=0;
	__uart_handle *uart_handle=&uart2_handle;
	gRecvStep=0;
	gRecvData=0;
	gRecvTime=0;
	gRecvCheckSum=0;
    memset(Uart_Recv_Buf,0, sizeof(Uart_Recv_Buf));
	uart_interrupt_receive_handle =NULL;
	status =uart_module_open(uart_handle,UART2_HARDWARE_NAME);
	if(!status)
	{
		__uart_param cur_param;
		memset(&cur_param,0,sizeof(__uart_param));
		cur_param.baud_rate=baud;
		cur_param.io=UART_TXPC4_RXPC5;
		cur_param.workmode=UART_WORKMODE_NORMAL;
		cur_param.custom |=(BIT(14)|BIT(3));
		status =uart_module_init(uart_handle,&cur_param);
		if(status)
		{
			puts("uart_interrupt_receive_module_init err\n");
		}
		uart_reg_isr_callback_fun(&uart2_handle,1,uart_interrupt_receive_isr_callback);
		if(UART_OUTPUT_CHAL == cur_param.io)//设置要作为串口的GPIO
		{
			JL_PORTA->DIR &=~BIT(9);
			JL_PORTA->DIE &=~BIT(9);
			JL_PORTA->PD |=BIT(9);
			JL_PORTA->PU |=BIT(9);
			JL_PORTA->DIR &=~BIT(10);
			JL_PORTA->DIE &=~BIT(10);
			JL_PORTA->PD |=BIT(10);
			JL_PORTA->PU |=BIT(10);
		}
		if(!status)
		{
			status =uart_module_start(uart_handle);
			if(!status)
			{
				uart_interrupt_receive_handle =uart_handle;
			}
		}
	}
    return status;
}
void printf_dabInfo(void)
{
    printf("printf_dab_info\n");
	for(u8 i=0;i<32;i++)
	{
		printf("Uart_Recv_Buf:0x%02x\n",Disp_dab_buf[i]);
	}
}
#endif

#if defined(TD_ADD_W2DAB_UART)

#if defined(_ENABLE_DAB_UART_)
__uart_handle *w1dab_uart_handle;
#if defined(UART_Servo_Pop)
extern void DABPushReceive(u8 data);
#endif

void W2dab_uart_write(char a)
{
	if(w1dab_uart_handle)
	{
		w1dab_uart_handle->putbyte(w1dab_uart_handle,a);
	}
	else
	{
        os_taskq_post(MAIN_TASK_NAME,1,MSG_UART_ERR);
	}
}

void W2dab_uart_receive_callback(u8 uto_buf,void *p,u8 isr_flag)
{
	if(UART_ISR_TYPE_DATA_COME == isr_flag)
	{
	#if defined(UART_Servo_Pop)
		DABPushReceive(uto_buf);
		//printf("%x ",uto_buf);
	#endif
	}
}
s32 W2dab_uart_init(u32 baud)
{
	s32 status=0;
	__uart_handle *uart_handle=&uart1_handle;
	w1dab_uart_handle =NULL;
	status =uart_module_open(uart_handle,UART1_HARDWARE_NAME);
	if(!status)
	{
		__uart_param cur_param;
		memset(&cur_param,0,sizeof(__uart_param));
		cur_param.baud_rate=baud;
		cur_param.io=UART_TXPA1_RXPA2;
		cur_param.workmode=UART_WORKMODE_NORMAL;
		cur_param.custom |=(BIT(14)|BIT(3));
		status =uart_module_init(uart_handle,&cur_param);
		uart_reg_isr_callback_fun(&uart1_handle,1,W2dab_uart_receive_callback);
		if(UART_OUTPUT_CHAL == cur_param.io)//设置要作为串口的GPIO
		{
			JL_PORTA->DIR &=~(BIT(1)|BIT(2));
			JL_PORTA->DIE &=~(BIT(1)|BIT(2));
			JL_PORTA->PD |=(BIT(1)|BIT(2));
			JL_PORTA->PU |=(BIT(1)|BIT(2));
		}
		if(!status)
		{
			status =uart_module_start(uart_handle);
			if(!status)
			{
				w1dab_uart_handle = uart_handle;
			}
		}
	}
    return status;
}

#ifdef DAB_UART_ONOFF

void OPEN_DAB_Uart2(void)
{
    W2dab_uart_init(DAB_UART_BAUD_RAE);
}
void Close_DAB_Uart2(void)
{
    uart_unreg_isr_callback_fun(&uart1_handle,1,W2dab_uart_receive_callback);
    uart_module_stop(&uart1_handle);
    uart_module_close(&uart1_handle);
	DAB_RX_INIT();
    DAB_RX_OUT();
    DAB_RX_LOW();
    DAB_TX_INIT();
    DAB_TX_OUT();
    DAB_TX_LOW();
}
#endif

#else

__uart_handle *w2dab_uart_handle;
#if defined(UART_Servo_Pop)
extern void DABPushReceive(u8 data);
#endif

void W2dab_uart_write(char a)
{
	if(w2dab_uart_handle)
	{
		w2dab_uart_handle->putbyte(w2dab_uart_handle,a);
	}
	else
	{
        os_taskq_post(MAIN_TASK_NAME,1,MSG_UART_ERR);
	}
}

void W2dab_uart_receive_callback(u8 uto_buf,void *p,u8 isr_flag)
{
	if(UART_ISR_TYPE_DATA_COME == isr_flag)
	{
	#if defined(UART_Servo_Pop)
		DABPushReceive(uto_buf);
		//printf("%x ",uto_buf);
	#endif
	}
}
s32 W2dab_uart_init(u32 baud)
{
	s32 status=0;
	__uart_handle *uart_handle=&uart2_handle;
	w2dab_uart_handle =NULL;
	status =uart_module_open(uart_handle,UART2_HARDWARE_NAME);
	if(!status)
	{
		__uart_param cur_param;
		memset(&cur_param,0,sizeof(__uart_param));
		cur_param.baud_rate=baud;
		cur_param.io=UART_TXPC4_RXPC5;
		cur_param.workmode=UART_WORKMODE_NORMAL;
		cur_param.custom |=(BIT(14)|BIT(3));
		status =uart_module_init(uart_handle,&cur_param);
		uart_reg_isr_callback_fun(&uart2_handle,1,W2dab_uart_receive_callback);
		if(UART_OUTPUT_CHAL == cur_param.io)//设置要作为串口的GPIO
		{
		#if 1
			JL_PORTC->DIR &=~(BIT(4)|BIT(5));
			JL_PORTC->DIE &=~(BIT(4)|BIT(5));
			JL_PORTC->PD |=(BIT(4)|BIT(5));
			JL_PORTC->PU |=(BIT(4)|BIT(5));
		#endif
			#if 0
			JL_PORTA->DIR &=~BIT(9);
			JL_PORTA->DIE &=~BIT(9);
			JL_PORTA->PD |=BIT(9);
			JL_PORTA->PU |=BIT(9);
			JL_PORTA->DIR &=~BIT(10);
			JL_PORTA->DIE &=~BIT(10);
			JL_PORTA->PD |=BIT(10);
			JL_PORTA->PU |=BIT(10);
			#endif
		}
		if(!status)
		{
			status =uart_module_start(uart_handle);
			if(!status)
			{
				w2dab_uart_handle = uart_handle;
			}
		}
	}
    return status;
}

#ifdef DAB_UART_ONOFF

void OPEN_DAB_Uart2(void)
{
    W2dab_uart_init(DAB_UART_BAUD_RAE);
}
void Close_DAB_Uart2(void)
{
    uart_unreg_isr_callback_fun(&uart2_handle,1,W2dab_uart_receive_callback);
    uart_module_stop(&uart2_handle);
    uart_module_close(&uart2_handle);
	DAB_RX_INIT();
    DAB_RX_OUT();
    DAB_RX_LOW();
    DAB_TX_INIT();
    DAB_TX_OUT();
    DAB_TX_LOW();
}
#endif
#endif
#endif

#if defined(_SUPPORT_DISC_)

static u8 updata_rx_buf[1];//注意啦:接受的最大数据为19个字节,总共为20个字节
#if defined(TD_ADD_UPDATA)
const u8 uart_updata_flag[]={"UART_UPDATA!"};
const u8 uart_updata_receive_flag[]={"RECEIVE_CMD!"};


extern void bredr_name_custom_handle(char *name);

void uart_updata_send_data(char *data,u32 len)
{
	u32 i;
	char *arr;
	arr =data;
	for(i=0;i<len;i++)
	{
		if(eq_dbg_handle)
		{
			eq_dbg_handle->putbyte(eq_dbg_handle,*(arr+i));
		}
		else
		{
			puts("uart_updata_err!\n");
		}
	}
}
void uart_updata_receive_cmd(void)
{
	uart_updata_send_data((char *)uart_updata_receive_flag,12);
}
#endif

void uart_interrupt_receive_write(char a)
{
    eq_dbg_handle->putbyte(eq_dbg_handle,a);
}


void uart_interrupt_receive_isr_callback(u8 uto_buf,void *p,u8 isr_flag)
{
	//uint8 lengh,i;
#if 1
	ServoPushReceive(updata_rx_buf[0]);
	//ServoPushReceive(updata_rx_buf[1]);
#else
	if((updata_rx_buf[0] == 0xFF)&&(updata_rx_buf[1] == 0xAA))
	{
		lengh = updata_rx_buf[2];
		ServoPushReceive(0xFF);
		ServoPushReceive(0xAA);
		ServoPushReceive(lengh);
		for(i=0;i<lengh;i++)
		{
			ServoPushReceive(updata_rx_buf[3+i]);
		}
	}
#endif

	memset(updata_rx_buf,0,sizeof(updata_rx_buf));
	if(UART_ISR_TYPE_DATA_COME ==isr_flag)
	{
		uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_SADR,sizeof(updata_rx_buf),updata_rx_buf);
		uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_EADR,sizeof(updata_rx_buf),updata_rx_buf);
		uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_CNT,sizeof(updata_rx_buf),updata_rx_buf);
	}
}

s32 uart_interrupt_receive_init(u32 baud)
{
	s32 status;
	eq_dbg_handle = NULL;
	status=uart_module_open(&uart1_handle,UART1_HARDWARE_NAME);
	if(!status)
	{
		__uart_param eq_param;
		memset(&eq_param,0,sizeof(__uart_param));
		eq_param.baud_rate=baud;
	#if defined(_NEW_DVD_PIN_)
		eq_param.io=UART_TXPB0_RXPB1;
	#else
		eq_param.io=UART_USBP_USBM;
	#endif
		eq_param.workmode=UART_WORKMODE_NORMAL;
		eq_param.custom = 0;
		eq_param.custom |= BIT(13) | BIT(12) |BIT(10)| BIT(7)|BIT(6)| BIT(5) |BIT(3)|BIT(0);
		status=uart_module_init(&uart1_handle,&eq_param);
		uart_reg_isr_callback_fun(&uart1_handle,2,uart_interrupt_receive_isr_callback);
		uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_SADR,sizeof(updata_rx_buf),updata_rx_buf);
		uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_EADR,sizeof(updata_rx_buf),updata_rx_buf);
		uart_module_ctrl(&uart1_handle,UART_CMD_DMA_WR_CNT,sizeof(updata_rx_buf),updata_rx_buf);
		uart_module_ctrl(&uart1_handle,UART_CMD_OT,80000*20,NULL);

        if(!status)
		{
			status=uart_module_start(&uart1_handle);
			if(!status)
			{
				eq_dbg_handle=&uart1_handle;
			}
		}
	}
	return status;
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/


#if defined(_USER_TFT_DISP_)
__uart_handle *TFT_uart_handle;

void TFT_uart_write(char a)
{
	if(TFT_uart_handle)
	{
		TFT_uart_handle->putbyte(TFT_uart_handle,a);
	}
	else
	{
        os_taskq_post(MAIN_TASK_NAME,1,MSG_UART_ERR);
	}
}

void TFT_uart_receive_callback(u8 uto_buf,void *p,u8 isr_flag)
{
	if(UART_ISR_TYPE_DATA_COME == isr_flag)
	{
		TFTPushReceive(uto_buf);
		//printf("==%x ",uto_buf);
	}
}
s32 TFT_uart_init(u32 baud)
{
	s32 status=0;
	__uart_handle *uart_handle=&uart2_handle;
	TFT_uart_handle =NULL;
	status =uart_module_open(uart_handle,UART2_HARDWARE_NAME);
	if(!status)
	{
		__uart_param cur_param;
		memset(&cur_param,0,sizeof(__uart_param));
		cur_param.baud_rate=baud;
		cur_param.io=UART_TXPC4_RXPC5;
		cur_param.workmode=UART_WORKMODE_NORMAL;
		cur_param.custom |=(BIT(14)|BIT(3));
		status =uart_module_init(uart_handle,&cur_param);
		uart_reg_isr_callback_fun(&uart2_handle,1,TFT_uart_receive_callback);
		if(UART_OUTPUT_CHAL == cur_param.io)//设置要作为串口的GPIO
		{
			#if 1
			JL_PORTC->DIR &=~(BIT(4)|BIT(5));
			JL_PORTC->DIE &=~(BIT(4)|BIT(5));
			JL_PORTC->PD |=(BIT(4)|BIT(5));
			JL_PORTC->PU |=(BIT(4)|BIT(5));
			#endif
		}
		if(!status)
		{
			status =uart_module_start(uart_handle);
			if(!status)
			{
				TFT_uart_handle =uart_handle;
			}
		}
	}
    return status;
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_CAN_UART_)
__uart_handle *mcu_uart_handle;

void mcu_uart_write(char a)
{
	//ew_printf("mcu_uart_handle:%d\n",mcu_uart_handle);
	if(mcu_uart_handle)
	{
		mcu_uart_handle->putbyte(mcu_uart_handle,a);
	}
	else
	{
        os_taskq_post(MAIN_TASK_NAME,1,MSG_UART_ERR);
	}
}

void mcu_uart_receive_callback(u8 uto_buf,void *p,u8 isr_flag)
{
	if(UART_ISR_TYPE_DATA_COME == isr_flag)
	{
		//ew_printf("=%x ",uto_buf);
		McuPushReceive(uto_buf);
	}
}
s32 mcu_uart_init(u32 baud)
{
	s32 status=0;
	__uart_handle *uart_handle=&uart2_handle;
	mcu_uart_handle =NULL;
	status =uart_module_open(uart_handle,UART2_HARDWARE_NAME);
	if(!status)
	{
		__uart_param cur_param;
		memset(&cur_param,0,sizeof(__uart_param));
		cur_param.baud_rate=baud;
		cur_param.io=UART_TXPC4_RXPC5;
		cur_param.workmode=UART_WORKMODE_NORMAL;
		cur_param.custom |=(BIT(14)|BIT(3));
		status =uart_module_init(uart_handle,&cur_param);
		uart_reg_isr_callback_fun(&uart2_handle,1,mcu_uart_receive_callback);
		if(UART_OUTPUT_CHAL == cur_param.io)//设置要作为串口的GPIO
		{
		#if 1
			JL_PORTC->DIR &=~(BIT(4)|BIT(5));
			JL_PORTC->DIE &=~(BIT(4)|BIT(5));
			JL_PORTC->PD |=(BIT(4)|BIT(5));
			JL_PORTC->PU |=(BIT(4)|BIT(5));
		#endif
		}
		if(!status)
		{
			status =uart_module_start(uart_handle);
			if(!status)
			{
				mcu_uart_handle =uart_handle;
			}
		}
	}
	return status;
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
#if defined(_AUDIO_DRIVER_AP3768_)
__uart_handle *audio_uart_handle;

void audio_uart_write(char a)
{
	//ew_printf("mcu_uart_handle:%d\n",audio_uart_handle);
	if(audio_uart_handle)
	{
		audio_uart_handle->putbyte(audio_uart_handle,a);
	}
	else
	{
        os_taskq_post(MAIN_TASK_NAME,1,MSG_UART_ERR);
	}
}

void audio_uart_receive_callback(u8 uto_buf,void *p,u8 isr_flag)
{
	if(UART_ISR_TYPE_DATA_COME == isr_flag)
	{
		//ew_printf("=%x ",uto_buf);
		AudioPushReceive(uto_buf);
	}
}
s32 audio_uart_init(u32 baud)
{
	s32 status=0;
	__uart_handle *uart_handle=&uart2_handle;
	audio_uart_handle =NULL;
	status =uart_module_open(uart_handle,UART2_HARDWARE_NAME);
	if(!status)
	{
		__uart_param cur_param;
		memset(&cur_param,0,sizeof(__uart_param));
		cur_param.baud_rate=baud;
		cur_param.io=UART_TXPC4_RXPC5;
		cur_param.workmode=UART_WORKMODE_NORMAL;
		cur_param.custom |=(BIT(14)|BIT(3));
		status =uart_module_init(uart_handle,&cur_param);
		uart_reg_isr_callback_fun(&uart2_handle,1,audio_uart_receive_callback);
		if(UART_OUTPUT_CHAL == cur_param.io)//设置要作为串口的GPIO
		{
		#if 1
			JL_PORTC->DIR &=~(BIT(4)|BIT(5));
			JL_PORTC->DIE &=~(BIT(4)|BIT(5));
			JL_PORTC->PD |=(BIT(4)|BIT(5));
			JL_PORTC->PU |=(BIT(4)|BIT(5));
		#endif
		}
		if(!status)
		{
			status =uart_module_start(uart_handle);
			if(!status)
			{
				audio_uart_handle =uart_handle;
			}
		}
	}
	return status;
}

#if defined(_ENABLE_UART_LOW_)
void Open_Audio_Uart2(void)
{
    audio_uart_init(115200);
}
void Close_Audio_Uart2(void)
{
    uart_unreg_isr_callback_fun(&uart2_handle,1,audio_uart_receive_callback);
    uart_module_stop(&uart2_handle);
    uart_module_close(&uart2_handle);
	AUDIO_RX_INIT();
    AUDIO_RX_OUT();
    AUDIO_RX_LOW();
    AUDIO_TX_INIT();
    AUDIO_TX_OUT();
    AUDIO_TX_LOW();
}
#endif

#endif

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/

#if defined(__EW_DEBUG__)

static void putbyte(char a)
{
	if(uart_put_handle)
	{
		uart_put_handle->putbyte(uart_put_handle,a);
	}
}

static char getbyte(void)
{
	if(uart_put_handle)
	{
       return uart_put_handle->getbyte(uart_put_handle);
	}
	return 0;
}

void putchar(char a)
{
	if(a == '\n')
	{
		putbyte('\r');
	}
	putbyte(a);
}

void put_u4hex(u8 dat)
{
    dat = 0xf & dat;

    if(dat > 9)
    {
        putchar(dat - 10 + 'A');
    }
    else
    {
        putchar(dat + '0');
    }
}

void put_u8hex(u8 dat)
{
    put_u4hex(dat >> 4);
    put_u4hex(dat);
    putchar(' ');
}

void put_u16hex(u16 dat)
{
    putchar('0');
    putchar('x');

    put_u4hex(dat >> 12);
    put_u4hex(dat >> 8);

    put_u4hex(dat >> 4);
    put_u4hex(dat);
    putchar(' ');
}

void put_u32hex(u32 dat)
{
    putchar('0');
    putchar('x');
    put_u4hex(dat >> 28);
    put_u4hex(dat >> 24);

    put_u4hex(dat >> 20);
    put_u4hex(dat >> 16);

    put_u4hex(dat >> 12);
    put_u4hex(dat >> 8);

    put_u4hex(dat >> 4);
    put_u4hex(dat);
    putchar('\n');
}

void put_u64hex(u64 dat)
{
    putchar('0');
    putchar('x');
    put_u32hex0(dat>>32);
    put_u32hex0(dat);
    putchar('\n');
}

void put_u32hex0(u32 dat)
{
    put_u4hex(dat >> 28);
    put_u4hex(dat >> 24);

    put_u4hex(dat >> 20);
    put_u4hex(dat >> 16);

    put_u4hex(dat >> 12);
    put_u4hex(dat >> 8);

    put_u4hex(dat >> 4);
    put_u4hex(dat);
}

void put_buf(u8 *buf, u32 len)
{
    u32 i ;
    for(i = 0 ; i < len ; i++)
    {
        if((i%16)==0)
        {
            putchar('\n') ;
        }
        put_u8hex(buf[i]) ;
    }
    putchar('\n') ;
}

int puts(const char *out)
{
    while(*out != '\0')
    {
        putchar(*out);
        out++;
    }
    return 0;
}

void printf_buf(u8 *buf, u32 len)
{
    u32 i;
    for(i = 0 ; i < len ; i++)
    {
        if((i % 16) == 0)
        {
            putchar('\n') ;
        }
        put_u8hex(buf[i]) ;
    }
    putchar('\n') ;
}

int ew_puts(const char *out)
{
	while(*out != '\0')
    {
        putchar(*out);
        out++;
    }
    return 0;
}

#else

void putchar(char a){}
void put_u4hex(u8 dat){}
void put_u8hex(u8 dat){}
void put_u16hex(u16 dat){}
void put_u32hex(u32 dat){}
void put_u64hex(u64 dat){}
void put_u32hex0(u32 dat){}

void put_buf(u8 *buf, u32 len ){}
int puts(const char *out){return 0;}
void printf_buf(u8 *buf, u32 len){}

#endif


